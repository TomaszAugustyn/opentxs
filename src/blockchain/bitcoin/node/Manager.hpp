// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <memory>

#include "blockchain/node/manager/Manager.hpp"
#include "opentxs/blockchain/BlockchainType.hpp"
#include "opentxs/blockchain/Types.hpp"
#include "opentxs/util/Bytes.hpp"
#include "opentxs/util/Container.hpp"

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
namespace block
{
class Header;
}  // namespace block

namespace database
{
namespace common
{
class Database;
}  // namespace common
}  // namespace database

namespace node
{
namespace internal
{
class Manager;
struct Config;
}  // namespace internal
}  // namespace node
}  // namespace blockchain
// }  // namespace v1
}  // namespace opentxs
// NOLINTEND(modernize-concat-nested-namespaces)

namespace opentxs::blockchain::node::base
{
class Bitcoin final : public node::implementation::Base
{
public:
    auto instantiate_header(const ReadView payload) const noexcept
        -> std::unique_ptr<block::Header> final;

    Bitcoin(
        const api::Session& api,
        const Type type,
        const internal::Config& config,
        const UnallocatedCString& seednode,
        const UnallocatedCString& syncEndpoint);
    Bitcoin() = delete;
    Bitcoin(const Bitcoin&) = delete;
    Bitcoin(Bitcoin&&) = delete;
    auto operator=(const Bitcoin&) -> Bitcoin& = delete;
    auto operator=(Bitcoin&&) -> Bitcoin& = delete;

    ~Bitcoin() final;

private:
    using ot_super = node::implementation::Base;
};

class NoSyncSupport final : public node::implementation::Base
{
public:
    auto instantiate_header(const ReadView payload) const noexcept
        -> std::unique_ptr<block::Header> final;

    NoSyncSupport(
        const api::Session& api,
        const Type type,
        const internal::Config& config,
        const UnallocatedCString& seednode,
        const UnallocatedCString& syncEndpoint);
    NoSyncSupport() = delete;
    NoSyncSupport(const NoSyncSupport&) = delete;
    NoSyncSupport(NoSyncSupport&&) = delete;
    auto operator=(const NoSyncSupport&) -> NoSyncSupport& = delete;
    auto operator=(NoSyncSupport&&) -> NoSyncSupport& = delete;

    ~NoSyncSupport() final;

private:
    using ot_super = node::implementation::Base;
};
}  // namespace opentxs::blockchain::node::base
