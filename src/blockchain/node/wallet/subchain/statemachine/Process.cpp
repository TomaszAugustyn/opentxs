// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: no_include <boost/smart_ptr/detail/operator_bool.hpp>

#include "0_stdafx.hpp"    // IWYU pragma: associated
#include "1_Internal.hpp"  // IWYU pragma: associated
#include "blockchain/node/wallet/subchain/statemachine/Process.hpp"  // IWYU pragma: associated

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <algorithm>
#include <memory>
#include <type_traits>
#include <utility>

#include "blockchain/node/wallet/subchain/SubchainStateData.hpp"
#include "internal/blockchain/node/Node.hpp"
#include "internal/blockchain/node/wallet/Types.hpp"
#include "internal/blockchain/node/wallet/subchain/statemachine/Job.hpp"
#include "internal/blockchain/node/wallet/subchain/statemachine/Types.hpp"
#include "internal/network/zeromq/Context.hpp"
#include "internal/network/zeromq/socket/Pipeline.hpp"
#include "internal/network/zeromq/socket/Raw.hpp"
#include "internal/util/LogMacros.hpp"
#include "opentxs/api/network/Network.hpp"
#include "opentxs/api/session/Endpoints.hpp"
#include "opentxs/api/session/Factory.hpp"
#include "opentxs/api/session/Session.hpp"
#include "opentxs/blockchain/BlockchainType.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/network/zeromq/Context.hpp"
#include "opentxs/network/zeromq/Pipeline.hpp"
#include "opentxs/network/zeromq/message/Frame.hpp"
#include "opentxs/network/zeromq/message/FrameSection.hpp"
#include "opentxs/network/zeromq/message/Message.hpp"
#include "opentxs/network/zeromq/socket/SocketType.hpp"
#include "opentxs/network/zeromq/socket/Types.hpp"
#include "opentxs/util/Allocator.hpp"
#include "opentxs/util/Log.hpp"
#include "opentxs/util/Pimpl.hpp"
#include "util/Work.hpp"

namespace opentxs::blockchain::node::wallet
{
Process::Imp::Imp(
    const SubchainStateData& parent,
    const network::zeromq::BatchID batch,
    allocator_type alloc) noexcept
    : Job(LogTrace(),
          parent,
          batch,
          CString{"process", alloc},
          alloc,
          {
              {parent.shutdown_endpoint_, Direction::Connect},
              {CString{
                   parent.api_.Endpoints().BlockchainBlockAvailable(),
                   alloc},
               Direction::Connect},
              {CString{parent.api_.Endpoints().BlockchainMempool(), alloc},
               Direction::Connect},
          },
          {
              {parent.to_process_endpoint_, Direction::Bind},
          },
          {},
          {
              {SocketType::Push,
               {
                   {parent.to_index_endpoint_, Direction::Connect},
               }},
          })
    , to_index_(pipeline_.Internal().ExtraSocket(0))
    , waiting_(alloc)
    , downloading_(alloc)
    , index_(alloc)
    , downloaded_(alloc)
    , txid_cache_()
{
    txid_cache_.reserve(1024);
}

auto Process::Imp::do_startup() noexcept -> void
{
    const auto& oracle = parent_.mempool_oracle_;

    for (const auto& txid : oracle.Dump()) {
        if (auto tx = oracle.Query(txid); tx) {
            parent_.ProcessTransaction(*tx);
        }
    }

    do_work();
}

auto Process::Imp::ProcessReorg(const block::Position& parent) noexcept -> void
{
    txid_cache_.clear();
    waiting_.erase(
        std::remove_if(
            waiting_.begin(),
            waiting_.end(),
            [&](const auto& pos) { return pos > parent; }),
        waiting_.end());

    {
        auto erase{false};
        auto& map = downloading_;

        for (auto i = map.begin(), end = map.end(); i != end;) {
            const auto& [position, future] = *i;

            if (erase || (position > parent)) {
                erase = true;
                index_.erase(position.second);
                i = map.erase(i);
            } else {
                ++i;
            }
        }
    }
}

auto Process::Imp::process_block(const block::Hash& hash) noexcept -> void
{
    if (auto index = index_.find(hash); index_.end() != index) {
        log_(OT_PRETTY_CLASS())(parent_.name_)(" processing block ")(
            hash.asHex())
            .Flush();
        auto& data = index->second;
        const auto& [position, future] = *data;
        const auto block = future.get();

        OT_ASSERT(block);

        parent_.ProcessBlock(position, *block);
        const auto sent = to_index_.SendDeferred([&] {
            auto out = MakeWork(Work::update);
            encode(
                [&] {
                    auto status = Vector<ScanStatus>{get_allocator()};
                    status.emplace_back(ScanState::processed, data->first);

                    return status;
                }(),
                out);

            return out;
        }());

        OT_ASSERT(sent);

        downloading_.erase(data);
        index_.erase(index);
        txid_cache_.emplace(block::pHash{hash});
        do_work();
    }
}

auto Process::Imp::process_mempool(Message&& in) noexcept -> void
{
    const auto body = in.Body();
    const auto chain = body.at(1).as<blockchain::Type>();

    if (parent_.chain_ != chain) { return; }

    const auto txid = parent_.api_.Factory().Data(body.at(2).Bytes());

    // TODO guarantee that already-confirmed transactions can never be processed
    // as mempool transactions even if they are erroneously received from peers
    // on a subsequent run of the application
    if (0u < txid_cache_.count(txid)) {
        log_(OT_PRETTY_CLASS())(parent_.name_)(" transaction ")(txid->asHex())(
            " already process as confirmed")
            .Flush();

        return;
    }

    if (auto tx = parent_.mempool_oracle_.Query(txid->Bytes()); tx) {
        parent_.ProcessTransaction(*tx);
    }
}

auto Process::Imp::process_update(Message&& msg) noexcept -> void
{
    auto dirty = Vector<ScanStatus>{get_allocator()};
    extract_dirty(parent_.api_, msg, dirty);

    for (auto& [type, position] : dirty) {
        waiting_.emplace_back(std::move(position));
    }

    to_index_.SendDeferred(std::move(msg));
    do_work();
}

auto Process::Imp::work() noexcept -> bool
{
    while ((downloading_.size() < download_limit_) && (0u < waiting_.size())) {
        auto& position = waiting_.front();
        log_(OT_PRETTY_CLASS())(parent_.name_)(" adding block ")(
            opentxs::print(position))(" to download queue")
            .Flush();
        auto future = parent_.node_.BlockOracle().LoadBitcoin(position.second);
        auto [it, added] =
            downloading_.try_emplace(std::move(position), std::move(future));
        index_.emplace(it->first.second, it);
        waiting_.pop_front();
    }

    return false;
}
}  // namespace opentxs::blockchain::node::wallet

namespace opentxs::blockchain::node::wallet
{
Process::Process(const SubchainStateData& parent) noexcept
    : imp_([&] {
        const auto& asio = parent.api_.Network().ZeroMQ().Internal();
        const auto batchID = asio.PreallocateBatch();
        // TODO the version of libc++ present in android ndk 23.0.7599858
        // has a broken std::allocate_shared function so we're using
        // boost::shared_ptr instead of std::shared_ptr

        return boost::allocate_shared<Imp>(
            alloc::PMR<Imp>{asio.Alloc(batchID)}, parent, batchID);
    }())
{
    imp_->Init(imp_);
}

auto Process::ChangeState(const State state) noexcept -> bool
{
    return imp_->ChangeState(state);
}

auto Process::ProcessReorg(const block::Position& parent) noexcept -> void
{
    imp_->ProcessReorg(parent);
}

Process::~Process()
{
    if (imp_) { imp_->Shutdown(); }
}
}  // namespace opentxs::blockchain::node::wallet