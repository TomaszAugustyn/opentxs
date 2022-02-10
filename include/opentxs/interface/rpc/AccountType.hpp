// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/Version.hpp"              // IWYU pragma: associated
#include "opentxs/interface/rpc/Types.hpp"  // IWYU pragma: associated

#include <limits>

namespace opentxs::v1::rpc
{
enum class AccountType : TypeEnum {
    error = 0,
    normal = 1,
    issuer = 2,
    blockchain = 3,
};
}  // namespace opentxs::v1::rpc
