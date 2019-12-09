// Copyright (c) 2010-2019 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "stdafx.hpp"

#include "Internal.hpp"

#if OT_CRYPTO_USING_LIBSECP256K1
#include "opentxs/api/crypto/Crypto.hpp"
#include "opentxs/api/crypto/Hash.hpp"
#include "opentxs/api/crypto/Util.hpp"
#include "opentxs/core/crypto/OTEnvelope.hpp"
#include "opentxs/core/crypto/OTPassword.hpp"
#include "opentxs/core/Armored.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Log.hpp"
#include "opentxs/core/String.hpp"
#include "opentxs/crypto/key/Asymmetric.hpp"
#include "opentxs/crypto/key/Secp256k1.hpp"
#include "opentxs/crypto/library/Secp256k1.hpp"
#include "opentxs/crypto/library/SymmetricProvider.hpp"
#include "opentxs/Proto.hpp"

#include "AsymmetricProvider.hpp"
#include "EcdsaProvider.hpp"

extern "C" {
#include "secp256k1.h"
#include "secp256k1_ecdh.h"
}

#include <cstdint>
#include <ostream>

#include "Secp256k1.hpp"

#define OT_METHOD "opentxs::Secp256k1::"

namespace opentxs
{
crypto::Secp256k1* Factory::Secp256k1(
    const api::Crypto& crypto,
    const api::crypto::Util& util)
{
    return new crypto::implementation::Secp256k1(crypto, util);
}
}  // namespace opentxs

namespace opentxs::crypto::implementation
{
bool Secp256k1::Initialized_ = false;

Secp256k1::Secp256k1(const api::Crypto& crypto, const api::crypto::Util& ssl)
    : EcdsaProvider(crypto)
    , context_(secp256k1_context_create(
          SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY))
    , ssl_(ssl)
{
}

bool Secp256k1::RandomKeypair(OTPassword& privateKey, Data& publicKey) const
{
    if (nullptr == context_) { return false; }

    bool validPrivkey = false;
    std::uint8_t candidateKey[PrivateKeySize]{};
    std::uint8_t nullKey[PrivateKeySize]{};
    std::uint8_t counter = 0;

    while (!validPrivkey) {
        privateKey.randomizeMemory_uint8(candidateKey, sizeof(candidateKey));
        // We add the random key to a zero value key because
        // secp256k1_privkey_tweak_add checks the result to make sure it's in
        // the correct range for secp256k1.
        //
        // This loop should almost always run exactly one time (about 1/(2^128)
        // chance of randomly generating an invalid key thus requiring a second
        // attempt)
        validPrivkey =
            secp256k1_ec_privkey_tweak_add(context_, candidateKey, nullKey);

        OT_ASSERT(3 > ++counter);
    }

    privateKey.setMemory(candidateKey, sizeof(candidateKey));

    return ScalarBaseMultiply(privateKey, publicKey);
}

bool Secp256k1::Sign(
    const api::internal::Core& api,
    const Data& plaintext,
    const key::Asymmetric& theKey,
    const proto::HashType hashType,
    Data& signature,  // output
    const PasswordPrompt& reason,
    const OTPassword* exportPassword) const
{
    auto hash = Data::Factory();
    bool haveDigest = crypto_.Hash().Digest(hashType, plaintext, hash);

    if (false == haveDigest) {
        LogOutput(OT_METHOD)(__FUNCTION__)(
            ": Failed to obtain the contract hash.")
            .Flush();

        return false;
    }

    if (0 == hash->size()) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid hash").Flush();

        return false;
    }

    hash->resize(32);

    OT_ASSERT(nullptr != hash->data());
    OT_ASSERT(32 == hash->size());

    OTPassword privKey;
    bool havePrivateKey{false};

    // TODO
    OT_ASSERT_MSG(nullptr == exportPassword, "This case is not yet handled.");

    const crypto::key::EllipticCurve* key =
        dynamic_cast<const crypto::key::Secp256k1*>(&theKey);

    if (nullptr == key) { return false; }

    havePrivateKey = AsymmetricKeyToECPrivatekey(api, *key, reason, privKey);

    if (havePrivateKey) {
        OT_ASSERT(nullptr != privKey.getMemory());
        OT_ASSERT(32 == privKey.getMemorySize());

        secp256k1_ecdsa_signature ecdsaSignature{};
        bool signatureCreated = secp256k1_ecdsa_sign(
            context_,
            &ecdsaSignature,
            reinterpret_cast<const unsigned char*>(hash->data()),
            reinterpret_cast<const unsigned char*>(privKey.getMemory()),
            nullptr,
            nullptr);

        if (signatureCreated) {
            signature.Assign(
                ecdsaSignature.data, sizeof(secp256k1_ecdsa_signature));

            return true;
        } else {
            LogOutput(OT_METHOD)(__FUNCTION__)(
                ": Call to secp256k1_ecdsa_sign() failed.")
                .Flush();

            return false;
        }
    } else {
        LogOutput(OT_METHOD)(__FUNCTION__)(
            ": Can not extract ecdsa private key from Asymmetric.")
            .Flush();

        return false;
    }
}

bool Secp256k1::Verify(
    const Data& plaintext,
    const key::Asymmetric& theKey,
    const Data& signature,
    const proto::HashType hashType,
    [[maybe_unused]] const PasswordPrompt& reason) const
{
    auto hash = Data::Factory();
    bool haveDigest = crypto_.Hash().Digest(hashType, plaintext, hash);

    if (false == haveDigest) { return false; }

    OT_ASSERT(nullptr != hash->data());

    const crypto::key::EllipticCurve* key =
        dynamic_cast<const crypto::key::Secp256k1*>(&theKey);

    if (nullptr == key) { return false; }

    auto ecdsaPubkey = Data::Factory();
    const bool havePublicKey = AsymmetricKeyToECPubkey(*key, ecdsaPubkey);

    if (!havePublicKey) { return false; }

    OT_ASSERT(nullptr != ecdsaPubkey->data());

    secp256k1_pubkey point{};
    const bool pubkeyParsed = ParsePublicKey(ecdsaPubkey, point);

    if (!pubkeyParsed) { return false; }

    secp256k1_ecdsa_signature ecdsaSignature{};
    const bool haveSignature = DataToECSignature(signature, ecdsaSignature);

    if (!haveSignature) { return false; }

    return secp256k1_ecdsa_verify(
        context_,
        &ecdsaSignature,
        reinterpret_cast<const unsigned char*>(hash->data()),
        &point);
}

bool Secp256k1::DataToECSignature(
    const Data& inSignature,
    secp256k1_ecdsa_signature& outSignature) const
{
    const std::uint8_t* sigStart =
        static_cast<const std::uint8_t*>(inSignature.data());

    if (nullptr != sigStart) {

        if (sizeof(secp256k1_ecdsa_signature) == inSignature.size()) {
            secp256k1_ecdsa_signature ecdsaSignature;

            for (std::uint32_t i = 0; i < inSignature.size(); i++) {
                ecdsaSignature.data[i] = *(sigStart + i);
            }

            outSignature = ecdsaSignature;

            return true;
        }
    }
    return false;
}

bool Secp256k1::ECDH(
    const Data& publicKey,
    const OTPassword& privateKey,
    OTPassword& secret) const
{
    auto parsedPubkey = secp256k1_pubkey{};
    auto success = ::secp256k1_ec_pubkey_parse(
        context_,
        &parsedPubkey,
        static_cast<const unsigned char*>(publicKey.data()),
        publicKey.size());

    if (0 == success) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Failed to parse public key")
            .Flush();

        return false;
    }

    if (false == privateKey.isMemory()) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid private key format")
            .Flush();

        return false;
    }

    if (32 != privateKey.getMemorySize()) {
        LogOutput(OT_METHOD)(__FUNCTION__)(": Invalid private key size (")(
            privateKey.getMemorySize())(")")
            .Flush();

        return false;
    }

    secret.randomizeMemory(32);

    OT_ASSERT(secret.isMemory());
    OT_ASSERT(32 == secret.getMemorySize());

    return 1 == ::secp256k1_ecdh(
                    context_,
                    static_cast<unsigned char*>(secret.getMemoryWritable()),
                    &parsedPubkey,
                    static_cast<const unsigned char*>(privateKey.getMemory()));
}

void Secp256k1::Init()
{
    OT_ASSERT(false == Initialized_);
    std::uint8_t randomSeed[32]{};
    ssl_.RandomizeMemory(randomSeed, 32);

    OT_ASSERT(nullptr != context_);

    [[maybe_unused]] int randomize =
        secp256k1_context_randomize(context_, randomSeed);
    Initialized_ = true;
}

bool Secp256k1::ParsePublicKey(const Data& input, secp256k1_pubkey& output)
    const
{
    if (nullptr == context_) { return false; }

    return secp256k1_ec_pubkey_parse(
        context_,
        &output,
        reinterpret_cast<const unsigned char*>(input.data()),
        input.size());
}

bool Secp256k1::ScalarBaseMultiply(
    const OTPassword& privateKey,
    Data& publicKey) const
{
    if (nullptr == context_) { return false; }

    if (!privateKey.isMemory()) { return false; }

    secp256k1_pubkey key{};

    const auto created = secp256k1_ec_pubkey_create(
        context_,
        &key,
        static_cast<const unsigned char*>(privateKey.getMemory()));

    if (1 != created) { return false; }

    unsigned char output[PublicKeySize]{};
    size_t outputSize = sizeof(output);

    const auto serialized = secp256k1_ec_pubkey_serialize(
        context_, output, &outputSize, &key, SECP256K1_EC_COMPRESSED);

    if (1 != serialized) { return false; }

    publicKey.Assign(output, outputSize);

    return true;
}

Secp256k1::~Secp256k1()
{
    if (nullptr != context_) {
        secp256k1_context_destroy(context_);
        context_ = nullptr;
    }

    Initialized_ = false;
}
}  // namespace opentxs::crypto::implementation
#endif
