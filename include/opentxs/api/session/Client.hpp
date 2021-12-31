// Copyright (c) 2010-2021 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/Version.hpp"  // IWYU pragma: associated

#include <string>

#include "opentxs/api/session/Session.hpp"

namespace opentxs
{
namespace api
{
namespace network
{
class ZMQ;
}  // namespace network

namespace session
{
namespace internal
{
class Client;
}  // namespace internal

class Activity;
class Contacts;
class OTX;
class UI;
class Workflow;
}  // namespace session
}  // namespace api
}  // namespace opentxs

namespace opentxs::api::session
{
class OPENTXS_EXPORT Client : virtual public api::Session
{
public:
    virtual auto Activity() const -> const session::Activity& = 0;
    virtual auto Contacts() const -> const api::session::Contacts& = 0;
    OPENTXS_NO_EXPORT virtual auto InternalClient() const noexcept
        -> const internal::Client& = 0;
    virtual auto OTX() const -> const session::OTX& = 0;
    virtual auto UI() const -> const session::UI& = 0;
    virtual auto Workflow() const -> const session::Workflow& = 0;
    virtual auto ZMQ() const -> const network::ZMQ& = 0;

    OPENTXS_NO_EXPORT virtual auto InternalClient() noexcept
        -> internal::Client& = 0;

    OPENTXS_NO_EXPORT ~Client() override = default;

protected:
    Client() = default;

private:
    Client(const Client&) = delete;
    Client(Client&&) = delete;
    auto operator=(const Client&) -> Client& = delete;
    auto operator=(Client&&) -> Client& = delete;
};
}  // namespace opentxs::api::session