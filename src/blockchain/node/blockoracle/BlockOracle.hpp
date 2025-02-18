// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/container/vector.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <cs_shared_guarded.h>
#include <chrono>
#include <cstddef>
#include <exception>
#include <functional>
#include <future>
#include <iosfwd>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string_view>
#include <tuple>
#include <utility>

#include "blockchain/node/blockoracle/BlockFetcher.hpp"
#include "blockchain/node/blockoracle/Cache.hpp"
#include "core/Worker.hpp"
#include "internal/blockchain/block/Validator.hpp"
#include "internal/blockchain/database/Block.hpp"
#include "internal/blockchain/node/Types.hpp"
#include "internal/blockchain/node/blockoracle/BlockBatch.hpp"
#include "internal/blockchain/node/blockoracle/BlockFetcher.hpp"
#include "internal/blockchain/node/blockoracle/BlockOracle.hpp"
#include "internal/network/zeromq/Types.hpp"
#include "opentxs/blockchain/BlockchainType.hpp"
#include "opentxs/blockchain/Types.hpp"
#include "opentxs/blockchain/block/Hash.hpp"
#include "opentxs/blockchain/block/Position.hpp"
#include "opentxs/blockchain/block/Types.hpp"
#include "opentxs/blockchain/node/BlockOracle.hpp"
#include "opentxs/blockchain/node/Types.hpp"
#include "opentxs/util/Allocated.hpp"
#include "opentxs/util/Bytes.hpp"
#include "opentxs/util/Container.hpp"
#include "opentxs/util/Time.hpp"
#include "opentxs/util/WorkType.hpp"
#include "util/Actor.hpp"
#include "util/Work.hpp"

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace opentxs  // NOLINT
{
// inline namespace v1
// {
namespace api
{
class Session;
}  // namespace api

namespace blockchain
{
namespace bitcoin
{
namespace block
{
class Block;
}  // namespace block
}  // namespace bitcoin

namespace block
{
class Position;
}  // namespace block

namespace node
{
namespace blockoracle
{
class BlockBatch;
class BlockDownloader;
}  // namespace blockoracle

namespace internal
{
class BlockBatch;
class Manager;
struct Config;
}  // namespace internal

class HeaderOracle;
}  // namespace node
}  // namespace blockchain

namespace network
{
namespace zeromq
{
namespace socket
{
class Publish;
}  // namespace socket

class Frame;
class Message;
}  // namespace zeromq
}  // namespace network
// }  // namespace v1
}  // namespace opentxs
// NOLINTEND(modernize-concat-nested-namespaces)

namespace opentxs::blockchain::node::internal
{
class BlockOracle::Imp final : public Actor<Imp, BlockOracleJobs>
{
public:
    auto DownloadQueue() const noexcept -> std::size_t
    {
        return cache_.lock_shared()->DownloadQueue();
    }
    auto Endpoint() const noexcept -> std::string_view
    {
        return submit_endpoint_;
    }
    auto GetBlockBatch(boost::shared_ptr<Imp> me) const noexcept -> BlockBatch;
    auto GetBlockJob() const noexcept -> BlockBatch;
    auto Heartbeat() const noexcept -> void;
    auto LoadBitcoin(const block::Hash& block) const noexcept
        -> BitcoinBlockResult;
    auto LoadBitcoin(const Vector<block::Hash>& hashes) const noexcept
        -> BitcoinBlockResults;
    auto SubmitBlock(const ReadView in) const noexcept -> void;
    auto Tip() const noexcept -> block::Position { return db_.BlockTip(); }
    auto Validate(const bitcoin::block::Block& block) const noexcept -> bool
    {
        return validator_->Validate(block);
    }

    auto Init(boost::shared_ptr<Imp> me) noexcept -> void
    {
        signal_startup(me);
    }
    auto Shutdown() noexcept -> void { return signal_shutdown(); }
    auto StartDownloader() noexcept -> void;

    Imp(const api::Session& api,
        const internal::Manager& node,
        const internal::Config& config,
        const node::HeaderOracle& header,
        database::Block& db,
        const blockchain::Type chain,
        const std::string_view parent,
        const network::zeromq::BatchID batch,
        allocator_type alloc) noexcept;
    Imp() = delete;
    Imp(const Imp&) = delete;
    Imp(Imp&&) = delete;
    auto operator=(const Imp&) -> Imp& = delete;
    auto operator=(Imp&&) -> Imp& = delete;

    ~Imp() final;

private:
    friend Actor<Imp, BlockOracleJobs>;

    using Task = BlockOracleJobs;
    using Cache =
        libguarded::shared_guarded<blockoracle::Cache, std::shared_mutex>;

    const api::Session& api_;
    const internal::Manager& node_;
    const database::Block& db_;
    const CString submit_endpoint_;
    const std::unique_ptr<const block::Validator> validator_;
    std::optional<blockoracle::BlockFetcher> block_fetcher_;
    mutable Cache cache_;

    static auto get_validator(
        const blockchain::Type chain,
        const node::HeaderOracle& headers) noexcept
        -> std::unique_ptr<const block::Validator>;

    auto do_shutdown() noexcept -> void;
    auto do_startup() noexcept -> void;
    auto pipeline(const Work work, Message&& msg) noexcept -> void;
    auto work() noexcept -> bool;

    Imp(const api::Session& api,
        const internal::Manager& node,
        const internal::Config& config,
        const node::HeaderOracle& header,
        database::Block& db,
        const blockchain::Type chain,
        const std::string_view parent,
        const network::zeromq::BatchID batch,
        CString&& submitEndpoint,
        allocator_type alloc) noexcept;
};
}  // namespace opentxs::blockchain::node::internal
