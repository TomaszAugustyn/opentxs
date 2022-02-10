// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/Version.hpp"

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace opentxs  // NOLINT
{
inline namespace v1
{
namespace proto
{
class ServerContext;
}  // namespace proto
}  // namespace v1
}  // namespace opentxs
// NOLINTEND(modernize-concat-nested-namespaces)

namespace opentxs::v1::proto
{
auto CheckProto_1(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_2(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_3(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_4(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_5(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_6(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_7(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_8(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_9(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_10(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_11(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_12(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_13(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_14(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_15(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_16(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_17(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_18(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_19(const ServerContext& context, const bool silent) -> bool;
auto CheckProto_20(const ServerContext& context, const bool silent) -> bool;
}  // namespace opentxs::v1::proto
