// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                      // IWYU pragma: associated
#include "1_Internal.hpp"                    // IWYU pragma: associated
#include "crypto/library/sodium/Sodium.hpp"  // IWYU pragma: associated

extern "C" {
#include <sodium.h>
}

#include <array>
#include <functional>
#include <string_view>

#include "internal/util/LogMacros.hpp"
#include "opentxs/OT.hpp"
#include "opentxs/api/Context.hpp"
#include "opentxs/api/Factory.hpp"
#include "opentxs/core/Secret.hpp"
#include "opentxs/crypto/HashType.hpp"
#include "opentxs/crypto/Parameters.hpp"
#include "opentxs/crypto/SecretStyle.hpp"
#include "opentxs/util/Container.hpp"
#include "opentxs/util/Log.hpp"
#include "opentxs/util/Pimpl.hpp"
#include "util/Sodium.hpp"

namespace opentxs::crypto::implementation
{
auto Sodium::PubkeyAdd(
    [[maybe_unused]] const ReadView pubkey,
    [[maybe_unused]] const ReadView scalar,
    [[maybe_unused]] const AllocateOutput result) const noexcept -> bool
{
    LogError()(OT_PRETTY_CLASS())("Not implemented").Flush();

    return false;
}

auto Sodium::RandomKeypair(
    const AllocateOutput privateKey,
    const AllocateOutput publicKey,
    const opentxs::crypto::key::asymmetric::Role,
    const Parameters&,
    const AllocateOutput) const noexcept -> bool
{
    auto seed = Context().Factory().Secret(0);
    seed->Randomize(crypto_sign_SEEDBYTES);

    return sodium::ExpandSeed(seed->Bytes(), privateKey, publicKey);
}

auto Sodium::ScalarAdd(
    const ReadView lhs,
    const ReadView rhs,
    const AllocateOutput result) const noexcept -> bool
{
    if (false == bool(result)) {
        LogError()(OT_PRETTY_CLASS())("Invalid output allocator").Flush();

        return false;
    }

    if (crypto_core_ed25519_SCALARBYTES != lhs.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid lhs scalar").Flush();

        return false;
    }

    if (crypto_core_ed25519_SCALARBYTES != rhs.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid rhs scalar").Flush();

        return false;
    }

    auto key = result(crypto_core_ed25519_SCALARBYTES);

    if (false == key.valid(crypto_core_ed25519_SCALARBYTES)) {
        LogError()(OT_PRETTY_CLASS())("Failed to allocate space for result")
            .Flush();

        return false;
    }

    ::crypto_core_ed25519_scalar_add(
        key.as<unsigned char>(),
        reinterpret_cast<const unsigned char*>(lhs.data()),
        reinterpret_cast<const unsigned char*>(rhs.data()));

    return true;
}

auto Sodium::ScalarMultiplyBase(
    const ReadView scalar,
    const AllocateOutput result) const noexcept -> bool
{
    if (false == bool(result)) {
        LogError()(OT_PRETTY_CLASS())("Invalid output allocator").Flush();

        return false;
    }

    if (crypto_scalarmult_ed25519_SCALARBYTES != scalar.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid scalar").Flush();

        return false;
    }

    auto pub = result(crypto_scalarmult_ed25519_BYTES);

    if (false == pub.valid(crypto_scalarmult_ed25519_BYTES)) {
        LogError()(OT_PRETTY_CLASS())("Failed to allocate space for public key")
            .Flush();

        return false;
    }

    return 0 == ::crypto_scalarmult_ed25519_base(
                    pub.as<unsigned char>(),
                    reinterpret_cast<const unsigned char*>(scalar.data()));
}

auto Sodium::SharedSecret(
    const ReadView pub,
    const ReadView prv,
    const SecretStyle style,
    Secret& secret) const noexcept -> bool
{
    if (SecretStyle::Default != style) {
        LogError()(OT_PRETTY_CLASS())("Unsupported secret style").Flush();

        return false;
    }

    if (crypto_sign_PUBLICKEYBYTES != pub.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid public key ").Flush();
        LogError()(OT_PRETTY_CLASS())("Expected: ")(crypto_sign_PUBLICKEYBYTES)
            .Flush();
        LogError()(OT_PRETTY_CLASS())("Actual:   ")(pub.size()).Flush();

        return false;
    }

    if (crypto_sign_SECRETKEYBYTES != prv.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid private key").Flush();
        LogError()(OT_PRETTY_CLASS())("Expected: ")(crypto_sign_SECRETKEYBYTES)
            .Flush();
        LogError()(OT_PRETTY_CLASS())("Actual:   ")(prv.size()).Flush();

        return false;
    }

    auto privateEd = Context().Factory().Secret(0);
    auto privateBytes = privateEd->WriteInto(Secret::Mode::Mem)(
        crypto_scalarmult_curve25519_BYTES);
    auto secretBytes =
        secret.WriteInto(Secret::Mode::Mem)(crypto_scalarmult_curve25519_BYTES);
    auto publicEd =
        std::array<unsigned char, crypto_scalarmult_curve25519_BYTES>{};

    OT_ASSERT(privateBytes.valid(crypto_scalarmult_curve25519_BYTES));
    OT_ASSERT(secretBytes.valid(crypto_scalarmult_curve25519_BYTES));

    if (0 != ::crypto_sign_ed25519_pk_to_curve25519(
                 publicEd.data(),
                 reinterpret_cast<const unsigned char*>(pub.data()))) {
        LogError()(OT_PRETTY_CLASS())(
            "crypto_sign_ed25519_pk_to_curve25519 error")
            .Flush();

        return false;
    }

    if (0 != ::crypto_sign_ed25519_sk_to_curve25519(
                 reinterpret_cast<unsigned char*>(privateBytes.data()),
                 reinterpret_cast<const unsigned char*>(prv.data()))) {
        LogError()(OT_PRETTY_CLASS())(
            "crypto_sign_ed25519_sk_to_curve25519 error")
            .Flush();

        return false;
    }

    OT_ASSERT(crypto_scalarmult_SCALARBYTES == privateEd->size());
    OT_ASSERT(crypto_scalarmult_BYTES == publicEd.size());
    OT_ASSERT(crypto_scalarmult_BYTES == secret.size());

    return 0 == ::crypto_scalarmult(
                    static_cast<unsigned char*>(secretBytes.data()),
                    static_cast<const unsigned char*>(privateBytes.data()),
                    publicEd.data());
}

auto Sodium::Sign(
    const ReadView plaintext,
    const ReadView priv,
    const crypto::HashType hash,
    const AllocateOutput signature) const -> bool
{
    if (crypto::HashType::Blake2b256 != hash) {
        LogVerbose()(OT_PRETTY_CLASS())("Unsupported hash function: ")(
            value(hash))
            .Flush();

        return false;
    }

    if (nullptr == priv.data() || 0 == priv.size()) {
        LogError()(OT_PRETTY_CLASS())("Missing private key").Flush();

        return false;
    }

    if (crypto_sign_SECRETKEYBYTES != priv.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid private key").Flush();
        LogError()(OT_PRETTY_CLASS())("Expected: ")(crypto_sign_SECRETKEYBYTES)
            .Flush();
        LogError()(OT_PRETTY_CLASS())("Actual:   ")(priv.size()).Flush();

        return false;
    }

    if (priv == blank_private()) {
        LogError()(OT_PRETTY_CLASS())("Blank private key").Flush();

        return false;
    }

    if (false == bool(signature)) {
        LogError()(OT_PRETTY_CLASS())("Invalid output allocator").Flush();

        return false;
    }

    auto output = signature(crypto_sign_BYTES);

    if (false == output.valid(crypto_sign_BYTES)) {
        LogError()(OT_PRETTY_CLASS())("Failed to allocate space for signature")
            .Flush();

        return false;
    }

    const auto success =
        0 == ::crypto_sign_detached(
                 output.as<unsigned char>(),
                 nullptr,
                 reinterpret_cast<const unsigned char*>(plaintext.data()),
                 plaintext.size(),
                 reinterpret_cast<const unsigned char*>(priv.data()));

    if (false == success) {
        LogError()(OT_PRETTY_CLASS())("Failed to sign plaintext.").Flush();
    }

    return success;
}

auto Sodium::Verify(
    const ReadView plaintext,
    const ReadView pub,
    const ReadView signature,
    const crypto::HashType type) const -> bool
{
    if (crypto::HashType::Blake2b256 != type) {
        LogVerbose()(OT_PRETTY_CLASS())("Unsupported hash function: ")(
            value(type))
            .Flush();

        return false;
    }

    if (crypto_sign_BYTES != signature.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid signature").Flush();

        return false;
    }

    if (nullptr == pub.data() || 0 == pub.size()) {
        LogError()(OT_PRETTY_CLASS())("Missing public key").Flush();

        return false;
    }

    if (crypto_sign_PUBLICKEYBYTES != pub.size()) {
        LogError()(OT_PRETTY_CLASS())("Invalid public key").Flush();
        LogError()(OT_PRETTY_CLASS())("Expected: ")(crypto_sign_PUBLICKEYBYTES)
            .Flush();
        LogError()(OT_PRETTY_CLASS())("Actual:   ")(pub.size()).Flush();

        return false;
    }

    const auto success =
        0 == ::crypto_sign_verify_detached(
                 reinterpret_cast<const unsigned char*>(signature.data()),
                 reinterpret_cast<const unsigned char*>(plaintext.data()),
                 plaintext.size(),
                 reinterpret_cast<const unsigned char*>(pub.data()));

    if (false == success) {
        LogVerbose()(OT_PRETTY_CLASS())("Failed to verify signature").Flush();
    }

    return success;
}
}  // namespace opentxs::crypto::implementation
