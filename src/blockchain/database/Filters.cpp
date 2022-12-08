// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                     // IWYU pragma: associated
#include "1_Internal.hpp"                   // IWYU pragma: associated
#include "blockchain/database/Filters.hpp"  // IWYU pragma: associated

#include <type_traits>
#include <utility>

#include "blockchain/database/common/Database.hpp"
#include "internal/blockchain/Blockchain.hpp"
#include "internal/blockchain/Params.hpp"
#include "internal/blockchain/block/Factory.hpp"
#include "internal/blockchain/database/Types.hpp"
#include "internal/blockchain/node/Types.hpp"
#include "internal/util/LogMacros.hpp"
#include "opentxs/api/session/Factory.hpp"
#include "opentxs/api/session/Session.hpp"
#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/blockchain/bitcoin/cfilter/GCS.hpp"
#include "opentxs/blockchain/bitcoin/cfilter/Hash.hpp"
#include "opentxs/blockchain/bitcoin/cfilter/Header.hpp"
#include "opentxs/blockchain/block/Hash.hpp"
#include "opentxs/blockchain/block/Header.hpp"
#include "opentxs/util/Log.hpp"
#include "util/LMDB.hpp"

namespace opentxs::blockchain::database
{
Filters::Filters(
    const api::Session& api,
    const common::Database& common,
    const storage::lmdb::LMDB& lmdb,
    const blockchain::Type chain) noexcept
    : api_(api)
    , common_(common)
    , lmdb_(lmdb)
    , blank_position_(make_blank<block::Position>::value(api))
    , lock_()
{
    if (opentxs::blockchain::SupportedChainsNoSync().count(chain)) {
        LogVerbose()(OT_PRETTY_CLASS())("CSPR/ETH chains not supported natively yet").Flush();
    } else {
        import_genesis(chain);
    }
}

auto Filters::CurrentHeaderTip(const cfilter::Type type) const noexcept
    -> block::Position
{
    auto output{blank_position_};
    auto cb = [this, &output](const auto in) {
        output = blockchain::internal::Deserialize(api_, in);
    };
    lmdb_.Load(
        Table::BlockFilterHeaderBest, static_cast<std::size_t>(type), cb);

    return output;
}

auto Filters::CurrentTip(const cfilter::Type type) const noexcept
    -> block::Position
{
    auto output{blank_position_};
    auto cb = [this, &output](const auto in) {
        output = blockchain::internal::Deserialize(api_, in);
    };
    lmdb_.Load(Table::BlockFilterBest, static_cast<std::size_t>(type), cb);

    return output;
}

auto Filters::HaveFilter(const cfilter::Type type, const block::Hash& block)
    const noexcept -> bool
{
    return common_.HaveFilter(type, block.Bytes());
}

auto Filters::HaveFilterHeader(
    const cfilter::Type type,
    const block::Hash& block) const noexcept -> bool
{
    return common_.HaveFilterHeader(type, block.Bytes());
}

auto Filters::import_genesis(const blockchain::Type chain) const noexcept
    -> void
{
    for (const auto& [style, genesis] : params::Filters().at(chain)) {
        const auto needHeader =
            blank_position_.height_ == CurrentHeaderTip(style).height_;
        const auto needFilter =
            blank_position_.height_ == CurrentTip(style).height_;

        if (!(needHeader || needFilter)) { return; }

        const auto pBlock = factory::GenesisBlockHeader(api_, chain);

        OT_ASSERT(pBlock);

        const auto& block = *pBlock;
        const auto& blockHash = block.Hash();
        const auto bytes = api_.Factory().DataFromHex(genesis.second);
        auto gcs = factory::GCS(
            api_,
            style,
            blockchain::internal::BlockHashToFilterKey(blockHash.Bytes()),
            bytes->Bytes(),
            {});  // TODO allocator

        OT_ASSERT(gcs.IsValid());

        auto success{false};

        if (needHeader) {

            cfilter::Header header{};
            auto result = header.DecodeHex(genesis.first);
            OT_ASSERT(result);

            auto headers =
                Vector<CFHeaderParams>{{blockHash, header, gcs.Hash()}};
            success = common_.StoreFilterHeaders(style, headers);

            OT_ASSERT(success);

            success = SetHeaderTip(style, block.Position());

            OT_ASSERT(success);
        }

        if (needFilter) {
            auto filters = Vector<database::Cfilter::CFilterParams>{};
            filters.emplace_back(blockHash, std::move(gcs));
            success = common_.StoreFilters(style, filters);

            OT_ASSERT(success);

            success = SetTip(style, block.Position());

            OT_ASSERT(success);
        }

        const auto loaded =
            LoadFilter(style, blockHash.Bytes(), {});  // TODO allocator

        OT_ASSERT(loaded.IsValid());
    }
}

auto Filters::LoadFilter(
    const cfilter::Type type,
    const ReadView block,
    alloc::Default alloc) const noexcept -> blockchain::GCS
{
    return common_.LoadFilter(type, block, alloc);
}

auto Filters::LoadFilters(
    const cfilter::Type type,
    const Vector<block::Hash>& blocks) const noexcept -> Vector<GCS>
{
    return common_.LoadFilters(type, blocks);
}

auto Filters::LoadFilterHash(const cfilter::Type type, const ReadView block)
    const noexcept -> cfilter::Hash
{
    auto output = cfilter::Hash{};

    if (common_.LoadFilterHash(type, block, output.WriteInto())) {

        return output;
    }

    return cfilter::Hash{};
}

auto Filters::LoadFilterHeader(const cfilter::Type type, const ReadView block)
    const noexcept -> cfilter::Header
{
    auto output = cfilter::Header{};

    if (common_.LoadFilterHeader(type, block, output.WriteInto())) {

        return output;
    }

    return {};
}

auto Filters::SetHeaderTip(
    const cfilter::Type type,
    const block::Position& position) const noexcept -> bool
{
    return lmdb_
        .Store(
            Table::BlockFilterHeaderBest,
            static_cast<std::size_t>(type),
            reader(blockchain::internal::Serialize(position)))
        .first;
}

auto Filters::SetTip(const cfilter::Type type, const block::Position& position)
    const noexcept -> bool
{
    return lmdb_
        .Store(
            Table::BlockFilterBest,
            static_cast<std::size_t>(type),
            reader(blockchain::internal::Serialize(position)))
        .first;
}

auto Filters::StoreFilters(
    const cfilter::Type type,
    const Vector<CFHeaderParams>& headers,
    const Vector<CFilterParams>& filters,
    const block::Position& tip) const noexcept -> bool
{
    auto output = common_.StoreFilters(type, headers, filters);

    if (!output) {
        LogError()(OT_PRETTY_CLASS())("Failed to save filters").Flush();

        return false;
    }

    if (0 > tip.height_) { return true; }

    auto parentTxn = lmdb_.TransactionRW();
    output = lmdb_
                 .Store(
                     Table::BlockFilterHeaderBest,
                     static_cast<std::size_t>(type),
                     reader(blockchain::internal::Serialize(tip)),
                     parentTxn)
                 .first;

    if (!output) {
        LogError()(OT_PRETTY_CLASS())("Failed to set header tip").Flush();

        return false;
    }

    output = lmdb_
                 .Store(
                     Table::BlockFilterBest,
                     static_cast<std::size_t>(type),
                     reader(blockchain::internal::Serialize(tip)),
                     parentTxn)
                 .first;

    if (!output) {
        LogError()(OT_PRETTY_CLASS())("Failed to set filter tip").Flush();

        return false;
    }

    return parentTxn.Finalize(true);
}

auto Filters::StoreFilters(
    const cfilter::Type type,
    Vector<CFilterParams> filters) const noexcept -> bool
{
    return common_.StoreFilters(type, filters);
}

auto Filters::StoreHeaders(
    const cfilter::Type type,
    const Vector<CFHeaderParams> headers) const noexcept -> bool
{
    return common_.StoreFilterHeaders(type, headers);
}
}  // namespace opentxs::blockchain::database
