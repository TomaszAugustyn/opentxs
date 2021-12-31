// Copyright (c) 2010-2021 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/Version.hpp"  // IWYU pragma: associated

#include "opentxs/api/crypto/Crypto.hpp"

namespace opentxs
{
namespace api
{
namespace crypto
{
class Asymmetric;
class Blockchain;
class Seed;
class Symmetric;
}  // namespace crypto

namespace session
{
namespace internal
{
class Crypto;
}  // namespace internal
}  // namespace session
}  // namespace api
}  // namespace opentxs

namespace opentxs::api::session
{
class OPENTXS_EXPORT Crypto : virtual public api::Crypto
{
public:
    virtual auto Asymmetric() const noexcept -> const crypto::Asymmetric& = 0;
    virtual auto Blockchain() const noexcept -> const crypto::Blockchain& = 0;
    OPENTXS_NO_EXPORT virtual auto InternalSession() const noexcept
        -> const internal::Crypto& = 0;
    virtual auto Seed() const noexcept -> const crypto::Seed& = 0;
    virtual auto Symmetric() const noexcept -> const crypto::Symmetric& = 0;

    OPENTXS_NO_EXPORT virtual auto InternalSession() noexcept
        -> internal::Crypto& = 0;

    OPENTXS_NO_EXPORT ~Crypto() override = default;

protected:
    Crypto() = default;

private:
    Crypto(const Crypto&) = delete;
    Crypto(Crypto&&) = delete;
    auto operator=(const Crypto&) -> Crypto& = delete;
    auto operator=(Crypto&&) -> Crypto& = delete;
};
}  // namespace opentxs::api::session