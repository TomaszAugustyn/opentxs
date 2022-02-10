// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/network/zeromq/Context.hpp"

namespace opentxs::v1::network::zeromq::socket::internal
{
class Socket : virtual public socket::Socket
{
public:
    virtual auto ID() const noexcept -> std::size_t = 0;
    auto Internal() const noexcept -> const internal::Socket& final
    {
        return *this;
    }

    auto Internal() noexcept -> internal::Socket& final { return *this; }

    ~Socket() override = default;
};
}  // namespace opentxs::v1::network::zeromq::socket::internal
