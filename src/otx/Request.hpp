// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "Proto.hpp"
#include "core/contract/Signable.hpp"
#include "internal/util/Flag.hpp"
#include "internal/util/Mutex.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/identifier/Generic.hpp"
#include "opentxs/core/identifier/Notary.hpp"
#include "opentxs/core/identifier/Nym.hpp"
#include "opentxs/identity/Types.hpp"
#include "opentxs/otx/Request.hpp"
#include "opentxs/otx/ServerRequestType.hpp"
#include "opentxs/otx/Types.hpp"
#include "opentxs/util/Bytes.hpp"
#include "opentxs/util/Container.hpp"
#include "opentxs/util/Numbers.hpp"
#include "serialization/protobuf/OTXEnums.pb.h"
#include "serialization/protobuf/ServerRequest.pb.h"

// NOLINTBEGIN(modernize-concat-nested-namespaces)
namespace opentxs  // NOLINT
{
// inline namespace v1
// {
namespace api
{
class Session;
}  // namespace api

namespace proto
{
class Signature;
}  // namespace proto

class PasswordPrompt;
// }  // namespace v1
}  // namespace opentxs
// NOLINTEND(modernize-concat-nested-namespaces)

namespace opentxs::otx::implementation
{
class Request final : public otx::Request,
                      public opentxs::contract::implementation::Signable
{
public:
    auto Initiator() const -> const identifier::Nym& final
    {
        return initiator_;
    }
    auto Number() const -> RequestNumber final;
    auto Serialize(AllocateOutput destination) const -> bool final;
    auto Serialize(proto::ServerRequest& serialized) const -> bool final;
    auto Server() const -> const identifier::Notary& final { return server_; }
    auto Type() const -> otx::ServerRequestType final { return type_; }

    auto SetIncludeNym(const bool include, const PasswordPrompt& reason)
        -> bool final;

    ~Request() final = default;

private:
    friend otx::Request;

    const OTNymID initiator_;
    const OTNotaryID server_;
    const otx::ServerRequestType type_;
    const RequestNumber number_;
    OTFlag include_nym_;

    static auto extract_nym(
        const api::Session& api,
        const proto::ServerRequest serialized) -> Nym_p;

    auto clone() const noexcept -> Request* final { return new Request(*this); }
    auto GetID(const Lock& lock) const -> OTIdentifier final;
    auto full_version(const Lock& lock) const -> proto::ServerRequest;
    auto id_version(const Lock& lock) const -> proto::ServerRequest;
    auto Name() const noexcept -> UnallocatedCString final { return {}; }
    auto Serialize() const noexcept -> OTData final;
    auto serialize(const Lock& lock, proto::ServerRequest& serialized) const
        -> bool;
    auto signature_version(const Lock& lock) const -> proto::ServerRequest;
    auto update_signature(const Lock& lock, const PasswordPrompt& reason)
        -> bool final;
    auto validate(const Lock& lock) const -> bool final;
    auto verify_signature(const Lock& lock, const proto::Signature& signature)
        const -> bool final;

    Request(
        const api::Session& api,
        const Nym_p signer,
        const identifier::Nym& initiator,
        const identifier::Notary& server,
        const otx::ServerRequestType type,
        const RequestNumber number);
    Request(const api::Session& api, const proto::ServerRequest serialized);
    Request() = delete;
    Request(const Request& rhs);
    Request(Request&& rhs) = delete;
    auto operator=(const Request& rhs) -> Request& = delete;
    auto operator=(Request&& rhs) -> Request& = delete;
};
}  // namespace opentxs::otx::implementation
