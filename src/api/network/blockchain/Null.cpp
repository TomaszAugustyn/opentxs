// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                      // IWYU pragma: associated
#include "1_Internal.hpp"                    // IWYU pragma: associated
#include "internal/api/network/Factory.hpp"  // IWYU pragma: associated

namespace opentxs::v1::factory
{
auto BlockchainNetworkAPI(
    const api::Session&,
    const api::session::Endpoints&,
    const opentxs::network::zeromq::Context&) noexcept
    -> api::network::Blockchain::Imp*
{
    return BlockchainNetworkAPINull();
}
}  // namespace opentxs::v1::factory
