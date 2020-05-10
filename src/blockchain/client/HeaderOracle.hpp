// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "internal/blockchain/client/Client.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/core/Data.hpp"

namespace opentxs
{
namespace api
{
namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace blockchain
{
namespace block
{
class Header;
}  // namespace block

namespace client
{
class UpdateTransaction;
}  // namespace client
}  // namespace blockchain

class Factory;
}  // namespace opentxs

namespace opentxs::blockchain::client::implementation
{
class HeaderOracle final : virtual public internal::HeaderOracle
{
public:
    auto BestChain() const noexcept -> block::Position final;
    auto BestHash(const block::Height height) const noexcept
        -> block::pHash final;
    auto CommonParent(const block::Position& position) const noexcept
        -> std::pair<block::Position, block::Position> final;
    auto GetCheckpoint() const noexcept -> block::Position final;
    auto IsInBestChain(const block::Hash& hash) const noexcept -> bool final;
    auto LoadHeader(const block::Hash& hash) const noexcept
        -> std::unique_ptr<block::Header> final;
    auto RecentHashes() const noexcept -> std::vector<block::pHash> final
    {
        return database_.RecentHashes();
    }
    auto Siblings() const noexcept -> std::set<block::pHash> final;

    auto AddCheckpoint(
        const block::Height position,
        const block::Hash& requiredHash) noexcept -> bool final;
    auto AddHeader(std::unique_ptr<block::Header> header) noexcept
        -> bool final;
    auto AddHeaders(std::vector<std::unique_ptr<block::Header>>&) noexcept
        -> bool final;
    auto DeleteCheckpoint() noexcept -> bool final;

    HeaderOracle(
        const api::internal::Core& api,
        const internal::Network& network,
        const internal::HeaderDatabase& database,
        const blockchain::Type type) noexcept;

    ~HeaderOracle() final = default;

private:
    friend opentxs::Factory;

    struct Candidate {
        bool blacklisted_{false};
        std::vector<block::Position> chain_{};
    };

    using Candidates = std::vector<Candidate>;

    static const std::
        map<blockchain::Type, std::pair<block::Height, std::string>>
            checkpoints_;

    const api::internal::Core& api_;
    const internal::Network& network_;
    const internal::HeaderDatabase& database_;
    const blockchain::Type chain_;
    mutable std::mutex lock_;

    static auto evaluate_candidate(
        const block::Header& current,
        const block::Header& candidate) noexcept -> bool;

    auto best_chain(const Lock& lock) const noexcept -> block::Position;
    auto is_in_best_chain(const Lock& lock, const block::Hash& hash) const
        noexcept -> bool;

    auto add_header(
        const Lock& lock,
        UpdateTransaction& update,
        std::unique_ptr<block::Header> header) noexcept -> bool;
    auto apply_checkpoint(
        const Lock& lock,
        const block::Height height,
        UpdateTransaction& update) noexcept -> bool;
    auto choose_candidate(
        const block::Header& current,
        const Candidates& candidates,
        UpdateTransaction& update) noexcept(false) -> std::pair<bool, bool>;
    void connect_children(
        const Lock& lock,
        block::Header& parentHeader,
        Candidates& candidates,
        Candidate& candidate,
        UpdateTransaction& update);
    // Returns true if the child is checkpoint blacklisted
    auto connect_to_parent(
        const Lock& lock,
        const UpdateTransaction& update,
        const block::Header& parent,
        block::Header& child) noexcept -> bool;
    auto initialize_candidate(
        const Lock& lock,
        const block::Header& best,
        const block::Header& parent,
        UpdateTransaction& update,
        Candidates& candidates,
        block::Header& child,
        const block::Hash& stopHash = Data::Factory()) noexcept(false)
        -> Candidate&;
    auto is_disconnected(
        const block::Hash& parent,
        UpdateTransaction& update) noexcept -> const block::Header*;
    void stage_candidate(
        const Lock& lock,
        const block::Header& best,
        Candidates& candidates,
        UpdateTransaction& update,
        block::Header& child) noexcept(false);

    HeaderOracle() = delete;
    HeaderOracle(const HeaderOracle&) = delete;
    HeaderOracle(HeaderOracle&&) = delete;
    auto operator=(const HeaderOracle&) -> HeaderOracle& = delete;
    auto operator=(HeaderOracle &&) -> HeaderOracle& = delete;
};
}  // namespace opentxs::blockchain::client::implementation
