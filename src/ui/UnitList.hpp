// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: private
// IWYU pragma: friend ".*src/ui/UnitList.cpp"

#pragma once

#include <map>
#include <string>
#include <utility>

#include "1_Internal.hpp"
#include "internal/ui/UI.hpp"
#include "opentxs/SharedPimpl.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/core/Identifier.hpp"
#if OT_BLOCKCHAIN
#include "opentxs/network/zeromq/ListenCallback.hpp"
#include "opentxs/network/zeromq/socket/Dealer.hpp"
#endif  // OT_BLOCKCHAIN
#include "opentxs/ui/UnitList.hpp"
#include "ui/List.hpp"
#include "ui/Widget.hpp"

namespace opentxs
{
namespace api
{
namespace client
{
namespace internal
{
struct Manager;
}  // namespace internal
}  // namespace client
}  // namespace api

namespace identifier
{
class Nym;
}  // namespace identifier

namespace network
{
namespace zeromq
{
namespace socket
{
class Publish;
}  // namespace socket

class Message;
}  // namespace zeromq
}  // namespace network

class Identifier;
}  // namespace opentxs

namespace opentxs::ui::implementation
{
using UnitListList = List<
    UnitListExternalInterface,
    UnitListInternalInterface,
    UnitListRowID,
    UnitListRowInterface,
    UnitListRowInternal,
    UnitListRowBlank,
    UnitListSortKey,
    UnitListPrimaryID>;

class UnitList final : public UnitListList
{
public:
    UnitList(
        const api::client::internal::Manager& api,
        const network::zeromq::socket::Publish& publisher,
        const identifier::Nym& nymID
#if OT_QT
        ,
        const bool qt
#endif
        ) noexcept;

    ~UnitList() final;

private:
#if OT_BLOCKCHAIN
    OTZMQListenCallback blockchain_balance_cb_;
    OTZMQDealerSocket blockchain_balance_;
#endif  // OT_BLOCKCHAIN
    const ListenerDefinitions listeners_;

    auto construct_row(
        const UnitListRowID& id,
        const UnitListSortKey& index,
        const CustomData& custom) const noexcept -> void* final;

    auto process_account(const network::zeromq::Message& message) noexcept
        -> void;
    auto process_account(const Identifier& id) noexcept -> void;
#if OT_BLOCKCHAIN
    auto process_blockchain_balance(
        const network::zeromq::Message& message) noexcept -> void;
#endif  // OT_BLOCKCHAIN
    auto process_unit(const UnitListRowID& id) noexcept -> void;
#if OT_BLOCKCHAIN
    auto setup_listeners(const ListenerDefinitions& definitions) noexcept
        -> void final;
#endif  // OT_BLOCKCHAIN
    auto startup() noexcept -> void;

    UnitList() = delete;
    UnitList(const UnitList&) = delete;
    UnitList(UnitList&&) = delete;
    auto operator=(const UnitList&) -> UnitList& = delete;
    auto operator=(UnitList &&) -> UnitList& = delete;
};
}  // namespace opentxs::ui::implementation
