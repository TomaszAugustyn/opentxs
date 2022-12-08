// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                        // IWYU pragma: associated
#include "1_Internal.hpp"                      // IWYU pragma: associated
#include "internal/blockchain/Blockchain.hpp"  // IWYU pragma: associated
#include "internal/blockchain/Params.hpp"      // IWYU pragma: associated

#include <boost/container/vector.hpp>
#include <string_view>
#include <type_traits>

#include "opentxs/api/crypto/Hash.hpp"
#include "opentxs/api/session/Crypto.hpp"
#include "opentxs/api/session/Session.hpp"
#include "opentxs/blockchain/Blockchain.hpp"
#include "opentxs/blockchain/BlockchainType.hpp"
#include "opentxs/blockchain/Types.hpp"
#include "opentxs/blockchain/bitcoin/cfilter/FilterType.hpp"
#include "opentxs/blockchain/crypto/AddressStyle.hpp"
#include "opentxs/core/Amount.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Types.hpp"
#include "opentxs/core/UnitType.hpp"
#include "opentxs/core/display/Definition.hpp"
#include "opentxs/crypto/Bip44Type.hpp"
#include "opentxs/crypto/HashType.hpp"
#include "opentxs/util/Bytes.hpp"
#include "opentxs/util/Container.hpp"
#include "opentxs/util/Pimpl.hpp"

namespace opentxs
{
auto BlockchainToUnit(const blockchain::Type type) noexcept -> UnitType
{
    try {
        return blockchain::params::Chains().at(type).itemtype_;
    } catch (...) {
        return UnitType::Unknown;
    }
}

auto UnitToBlockchain(const UnitType type) noexcept -> blockchain::Type
{
    using Map = UnallocatedMap<opentxs::UnitType, opentxs::blockchain::Type>;

    static const auto build = []() -> auto
    {
        auto output = Map{};

        for (const auto& [chain, data] : blockchain::params::Chains()) {
            output.emplace(data.itemtype_, chain);
        }

        return output;
    };
    static const auto map{build()};

    try {
        return map.at(type);
    } catch (...) {
        return blockchain::Type::Unknown;
    }
}
}  // namespace opentxs

namespace opentxs::blockchain
{
auto BlockHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return api.Crypto().Hash().Digest(
                opentxs::crypto::HashType::Sha256D, input, output);
        }
    }
}

auto DefinedChains() noexcept -> const UnallocatedSet<Type>&
{
    static const auto output = [] {
        auto output = UnallocatedSet<Type>{};

        for (const auto& [chain, data] : params::Chains()) {
            output.emplace(chain);
        }

        return output;
    }();

    return output;
}

auto FilterHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return BlockHash(api, chain, input, output);
        }
    }
}

auto HasSegwit(const Type type) noexcept -> bool
{
    try {

        return params::Chains().at(type).segwit_;
    } catch (...) {

        return false;
    }
}

auto IsTestnet(const Type type) noexcept -> bool
{
    try {

        return params::Chains().at(type).testnet_;
    } catch (...) {

        return false;
    }
}

auto MerkleHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return BlockHash(api, chain, input, output);
        }
    }
}

auto P2PMessageHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return api.Crypto().Hash().Digest(
                opentxs::crypto::HashType::Sha256DC, input, output);
        }
    }
}

auto print(Type type) noexcept -> std::string_view
{
    return opentxs::print(BlockchainToUnit(type));
}

auto ProofOfWorkHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Litecoin:
        case Type::Litecoin_testnet4: {
            return api.Crypto().Hash().Scrypt(
                input, input, 1024, 1, 1, 32, output);
        }
        case Type::UnitTest:
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        default: {
            return BlockHash(api, chain, input, output);
        }
    }
}

auto PubkeyHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return api.Crypto().Hash().Digest(
                opentxs::crypto::HashType::Bitcoin, input, output);
        }
    }
}

auto ScriptHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return api.Crypto().Hash().Digest(
                opentxs::crypto::HashType::Bitcoin, input, output);
        }
    }
}

auto ScriptHashSegwit(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return api.Crypto().Hash().Digest(
                opentxs::crypto::HashType::Sha256, input, output);
        }
    }
}

auto SupportedChains() noexcept -> const UnallocatedSet<Type>&
{
    static const auto output = [] {
        auto output = UnallocatedSet<Type>{};

        for (const auto& [chain, data] : params::Chains()) {
            if (data.supported_) { output.emplace(chain); }
        }

        return output;
    }();

    return output;
}

auto SupportedChainsNoSync() noexcept -> const UnallocatedSet<Type>&
{
    static const auto output = [] {
        auto output = UnallocatedSet<Type>{};

        for (const auto& [chain, data] : params::Chains()) {
            if (!data.full_sync_support_) { output.emplace(chain); }
        }

        return output;
    }();

    return output;
}

auto SupportedChainsFullSync() noexcept -> const UnallocatedSet<Type>&
{
    static const auto output = [] {
        auto output = UnallocatedSet<Type>{};

        for (const auto& [chain, data] : params::Chains()) {
            if (data.full_sync_support_) { output.emplace(chain); }
        }

        return output;
    }();

    return output;
}

auto TickerSymbol(const Type type) noexcept -> UnallocatedCString
{
    return UnallocatedCString{
        display::GetDefinition(BlockchainToUnit(type)).ShortName()};
}

auto TransactionHash(
    const api::Session& api,
    const Type chain,
    const ReadView input,
    const AllocateOutput output) noexcept -> bool
{
    switch (chain) {
        case Type::Unknown:
        case Type::Bitcoin:
        case Type::Bitcoin_testnet3:
        case Type::BitcoinCash:
        case Type::BitcoinCash_testnet3:
        case Type::Ethereum_frontier:
        case Type::Ethereum_ropsten:
        case Type::Litecoin:
        case Type::Litecoin_testnet4:
        case Type::PKT:
        case Type::PKT_testnet:
        case Type::BitcoinSV:
        case Type::BitcoinSV_testnet3:
        case Type::eCash:
        case Type::eCash_testnet3:
        case Type::Casper:
        case Type::Casper_testnet:
        case Type::UnitTest:
        default: {
            return BlockHash(api, chain, input, output);
        }
    }
}
}  // namespace opentxs::blockchain

namespace opentxs::blockchain::block
{
auto BlankHash() noexcept -> pHash
{
    auto out = Data::Factory();
    out->DecodeHex(
        "0x0000000000000000000000000000000000000000000000000000000000000000");

    return out;
}
}  // namespace opentxs::blockchain::block

namespace opentxs::blockchain::internal
{
auto Format(const Type chain, const opentxs::Amount& amount) noexcept
    -> UnallocatedCString
{
    try {
        const auto& definition =
            display::GetDefinition(BlockchainToUnit(chain));

        return definition.Format(amount);
    } catch (...) {

        return {};
    }
}
}  // namespace opentxs::blockchain::internal

namespace opentxs::blockchain::params
{
#if OT_BLOCKCHAIN
auto Bip158() noexcept -> const FilterTypes&
{
    static const auto data = FilterTypes{
        {Type::Bitcoin,
         {
             {cfilter::Type::Basic_BIP158, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::Bitcoin_testnet3,
         {
             {cfilter::Type::Basic_BIP158, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::BitcoinCash,
         {
             {cfilter::Type::Basic_BCHVariant, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::BitcoinCash_testnet3,
         {
             {cfilter::Type::Basic_BCHVariant, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::Ethereum_frontier, {}},
        {Type::Ethereum_ropsten, {}},
        {Type::Casper, {}},
        {Type::Casper_testnet, {}},
        {Type::Litecoin,
         {
             {cfilter::Type::Basic_BIP158, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::Litecoin_testnet4,
         {
             {cfilter::Type::Basic_BIP158, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::PKT,
         {
             {cfilter::Type::Basic_BIP158, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::PKT_testnet,
         {
             {cfilter::Type::Basic_BIP158, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::BitcoinSV,
         {
             {cfilter::Type::Basic_BCHVariant, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::BitcoinSV_testnet3,
         {
             {cfilter::Type::Basic_BCHVariant, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::eCash,
         {
             {cfilter::Type::Basic_BCHVariant, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::eCash_testnet3,
         {
             {cfilter::Type::Basic_BCHVariant, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
        {Type::UnitTest,
         {
             {cfilter::Type::Basic_BIP158, 0x0},
             {cfilter::Type::ES, 0x58},
         }},
    };

    return data;
}
#endif  // OT_BLOCKCHAIN

auto Chains() noexcept -> const ChainData&
{
    using namespace std::literals;
    static const auto data = ChainData{
        {blockchain::Type::Bitcoin,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_SUPPORTED,
             4u,
             opentxs::UnitType::Btc,
             Bip44Type::BITCOIN,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c"sv,
             "6fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {688000,
              "d305263e3bc034a3395437e316437c5a5347c18e6f5201000000000000000000"sv,
              "dd08dde8736a06996ab3fc3d884d84a5d369246183c409000000000000000000"sv,
              "2b9004f975a42b8b911731ff2f703ed12f0bafdcbcbf71fc12d2cfef9b0ce0b6"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             3652501241,
             8333,
             {
                 "seed.bitcoin.sipa.be"sv,
                 "dnsseed.bluematt.me"sv,
                 "dnsseed.bitcoin.dashjr.org"sv,
                 "seed.bitcoinstats.com"sv,
                 "seed.bitcoin.jonasschnelli.ch"sv,
                 "seed.btc.petertodd.org"sv,
                 "seed.bitcoin.sprovoost.nl"sv,
                 "dnsseed.emzy.de"sv,
             },
             25000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, true},
                 {Data::Style::P2WSH, true},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2WPKH, "P2WPKH"sv},
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             250,
         }},
        {blockchain::Type::Bitcoin_testnet3,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_SUPPORTED,
             4u,
             opentxs::UnitType::Tnbtc,
             Bip44Type::TESTNET,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae18"sv,
             "43497fd7f826957108f4a30fd9cec3aeba79972084e90ead01ea330900000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae180101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {2005000,
              "15ff88d0b983325bb99a6375538b7c0e60427373754c591dccfb5c5c00000000"sv,
              "b5f3c6cab9d4fadc9c679ad19e46da9be74d33ec640d5de00700000000000000"sv,
              "072fffbbd1723b1796b92743fb8af34f8e62173985dd3249b663c2e62da6e8a9"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             118034699,
             18333,
             {
                 "testnet-seed.bitcoin.jonasschnelli.ch"sv,
                 "seed.tbtc.petertodd.org"sv,
                 "seed.testnet.bitcoin.sprovoost.nl"sv,
                 "testnet-seed.bluematt.me"sv,
                 "testnet-seed.bitcoin.schildbach.de"sv,
             },
             4000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, true},
                 {Data::Style::P2WSH, true},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2WPKH, "P2WPKH"sv},
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::BitcoinCash,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Bch,
             Bip44Type::BITCOINCASH,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c"sv,
             "6fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {693000,
              "ce0acf276d39e75741808afdde9c95b75c2fef935bc254010000000000000000"sv,
              "c83139029d4efddeda60ede13474276cdf1267fff61269000000000000000000"sv,
              "fb2e542d6e1aa15d763a3a1add8262fa7cb609063d8047372deae34118f8836a"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             3908297187,
             8333,
             {
                 "seed.bitcoinabc.org"sv,
                 "seed-abc.bitcoinforks.org"sv,
                 "btccash-seeder.bitcoinunlimited.info"sv,
                 "seed.deadalnix.me"sv,
                 "seed.bchd.cash"sv,
                 "dnsseed.electroncash.de"sv,
             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::BitcoinCash_testnet3,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Tnbch,
             Bip44Type::TESTNET,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae18"sv,
             "43497fd7f826957108f4a30fd9cec3aeba79972084e90ead01ea330900000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae180101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {1452000,
              "5b9a0ecfcb3c18484bb5ce4d39e6bb29c204f482bcda89947a00000000000000"sv,
              "b7c4df50f1c631aaf16baf75167c27a8ed755766ac67acef7500000000000000"sv,
              "fb2e542d6e1aa15d763a3a1add8262fa7cb609063d8047372deae34118f8836a"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             4109624820,
             18333,
             {
                 "testnet-seed.bitcoinabc.org"sv,
                 "testnet-seed-abc.bitcoinforks.org"sv,
                 "testnet-seed.bitprim.org"sv,
                 "testnet-seed.deadalnix.me"sv,
                 "testnet-seed.bchd.cash"sv,
             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::Ethereum_frontier,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_NOT_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Eth,
             Bip44Type::ETHER,
             0,
             ""sv,
             "d4e56740f876aef8c010b86a40d5f56745a118d0906a34e69aec8c0db1cb8fa3"sv,
             ""sv,
             {0, ""sv, ""sv, ""sv},
             {},
             p2p::Protocol::ethereum,
             0,
             0,
             30303,
             {},
             0,
             0,
             {
                 {Data::Style::P2PKH, false},
                 {Data::Style::P2SH, false},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, true},
             },
             {
                 {Data::Style::ChecksummedHex, "ChecksummedHex"sv},
             },
             {},
         }},
        {blockchain::Type::Ethereum_ropsten,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_NOT_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Ethereum_ropsten,
             Bip44Type::TESTNET,
             0,
             ""sv,
             "41941023680923e0fe4d74a34bdac8141f2540e3ae90623718e47d66d1ca4a2d"sv,
             ""sv,
             {0, ""sv, ""sv, ""sv},
             {},
             p2p::Protocol::ethereum,
             0,
             0,
             30303,
             {},
             0,
             0,
             {
                 {Data::Style::P2PKH, false},
                 {Data::Style::P2SH, false},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, true},
             },
             {},
             {},
         }},
        {blockchain::Type::Casper,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_NOT_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Cspr,
             Bip44Type::CSPR,
             0,
             ""sv,
             "2fe9630b7790852e4409d815b04ca98f37effcdf9097d317b9b9b8ad658f47c8"sv,
             ""sv,
             {0, ""sv, ""sv, ""sv},
             {},
             p2p::Protocol::casper,
             0,
             0,
             35000,
             {},
             0,
             0,
             {
                 {Data::Style::P2PKH, false},
                 {Data::Style::P2SH, false},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, true},
             },
             {},
             {},
         }},
        {blockchain::Type::Casper_testnet,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_NOT_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::TnCspr,
             Bip44Type::TESTNET,
             0,
             ""sv,
             "975ea4de188c2a67946188cea0ec8a93d2d38286d7642526d50c45839a291210"sv,
             ""sv,
             {0, ""sv, ""sv, ""sv},
             {},
             p2p::Protocol::casper,
             0,
             0,
             35000,
             {},
             0,
             0,
             {
                 {Data::Style::P2PKH, false},
                 {Data::Style::P2SH, false},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, true},
             },
             {
                 {Data::Style::ChecksummedHex, "ChecksummedHex"sv},
             },
             {},
         }},
        {blockchain::Type::Litecoin,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_SUPPORTED,
             4u,
             opentxs::UnitType::Ltc,
             Bip44Type::LITECOIN,
             504365040,  // 0x1e0ffff0
             "010000000000000000000000000000000000000000000000000000000000000000000000d9ced4ed1130f7b7faad9be25323ffafa33232a17c3edf6cfd97bee6bafbdd97b9aa8e4ef0ff0f1ecd513f7c"sv,
             "e2bf047e7e5a191aa4ef34d314979dc9986e0f19251edaba5940fd1fe365a712"sv,
             "010000000000000000000000000000000000000000000000000000000000000000000000d9ced4ed1130f7b7faad9be25323ffafa33232a17c3edf6cfd97bee6bafbdd97b9aa8e4ef0ff0f1ecd513f7c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4804ffff001d0104404e592054696d65732030352f4f63742f32303131205374657665204a6f62732c204170706c65e280997320566973696f6e6172792c2044696573206174203536ffffffff0100f2052a010000004341040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9ac00000000"sv,
             {2074000,
              "35602501052d8e861ee18040558ca6c7abf910512cc4e8e52eaaea137700b562"sv,
              "9d9491a5cc4d4f1aead008449a372cab7b96b5e0116b93336df0143e882d2a56"sv,
              "bfff6ef3acccc9d4d4e593864514eeaa8c78aa1b0e29efb3aa92228a4f97fea1"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             3686187259,
             9333,
             {
                 "seed-a.litecoin.loshan.co.uk"sv,
                 "dnsseed.thrasher.io"sv,
                 "dnsseed.litecointools.com"sv,
                 "dnsseed.litecoinpool.org"sv,
                 "dnsseed.koin-project.com"sv,
             },
             2000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, true},
                 {Data::Style::P2WSH, true},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2WPKH, "P2WPKH"sv},
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::Litecoin_testnet4,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_SUPPORTED,
             4u,
             opentxs::UnitType::Tnltx,
             Bip44Type::TESTNET,
             504365040,  // 0x1e0ffff0
             "010000000000000000000000000000000000000000000000000000000000000000000000d9ced4ed1130f7b7faad9be25323ffafa33232a17c3edf6cfd97bee6bafbdd97f60ba158f0ff0f1ee1790400"sv,
             "a0293e4eeb3da6e6f56f81ed595f57880d1a21569e13eefdd951284b5a626649"sv,
             "010000000000000000000000000000000000000000000000000000000000000000000000d9ced4ed1130f7b7faad9be25323ffafa33232a17c3edf6cfd97bee6bafbdd97f60ba158f0ff0f1ee17904000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4804ffff001d0104404e592054696d65732030352f4f63742f32303131205374657665204a6f62732c204170706c65e280997320566973696f6e6172792c2044696573206174203536ffffffff0100f2052a010000004341040184710fa689ad5023690c80f3a49c8f13f8d45b8c857fbcbc8bc4a8e4d3eb4b10f4d4604fa08dce601aaf0f470216fe1b51850b4acf21b179c45070ac7b03a9ac00000000"sv,
             {1941000,
              "461cb388a237e4da64bae7de29fdd34299e637aeeea2e61a874bbe562b859195"sv,
              "a88a1bff3883708a8ef2b1f9429c548866e3666ae0bcdc78f360af3b6fddc61b"sv,
              "85ca75a1084b9ab7bd7dde196260e175d841064fccc7a247bf2766eda2b0194b"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             4056470269,
             19335,
             {
                 "testnet-seed.litecointools.com"sv,
                 "seed-b.litecoin.loshan.co.uk"sv,
                 "dnsseed-testnet.thrasher.io"sv,
             },
             2000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, true},
                 {Data::Style::P2WSH, true},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2WPKH, "P2WPKH"sv},
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::PKT,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_SUPPORTED,
             4u,
             opentxs::UnitType::Pkt,
             Bip44Type::PKT,
             521142271,  // 0x1f0fffff
             "000000000000000000000000000000000000000000000000000000000000000000000000df345ba23b13467eec222a919d449dab6506abc555ef307794ecd3d36ac891fb00000000ffff0f1f00000000"sv,
             "852d43936f4c9606a4a063cf356c454f2d9c43b07a41cf52e59461a41217dc0b"sv,
             "000000000000000000000000000000000000000000000000000000000000000000000000df345ba23b13467eec222a919d449dab6506abc555ef307794ecd3d36ac891fb00000000ffff0f1f0000000001fd04160000000000000000df345ba23b13467eec222a919d449dab6506abc555ef307794ecd3d36ac891fb00096e88ffff0f1f0300000000000000347607000098038000000000ffff0f2000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e79d06f72d778459a76a989dbdded6d45b5e4358943c9aab1eb4e42a9c67f9ac317b762fe60198c3861255552928a179a5e9a6b9b7b7f4b44e02fc3519f92964fbbfb576d1e9ff3c588c60fb2643602ae1f5695f89460608d3250e57a7755385aaa0de52409159387de4145d92533cd5f2a0d6d2a21b653311a40bd2556493171cf1beedf894a090626577d8042e72f9cdab8ab212b2d6ee5ca7b22169a01bf903ab05b248fb8ed5de5a2bb0cd3901fc2e3270ffa524ed3adfc9d7fe109d0e2755f016386a09eda81bd9707bf681d75cef829f3f8ee0903bfdb2c86ff44628df573143ec832f41ae17e575e31848d9cf430930d81f41b0d803251b81f8181e543cb25c7dca4f2454f8f8bb86987db019ceffe7f0a2be807767f85dc903d3b843af448d14d5214b6ad5812b4d82b8cbea25c69c6b87d667f9c18c2993d500ed902d4c539a7d06ab0ca95afd946fd3702554e4bf9f76a1f087dccf33356b7efa9149fa6b4949159d03cb6e7d13efe91134a9ed8adc7c7325d39201cb2c2c1e2191c5e9d3d71dc5d1232e4cfc603fc5aa994d7bb8d190ca3d7c0e2fb9abb68df80c2cdfd8d119aec1a9c62c0ef7af9375e56c0330263332c4c879bcda52de73fea26781eb3dfa19dd2399b605050198fca80467bdca4a50980a3a37aa552f65caf9634b18fca475551d0a37dceab5f78c1cfdb48917122137cb74e236800c0684936b9cc0ca563025cb68609be37869fa8e95bb6fdcd15320b3d5b2fabe9524f464dbfabe36ef958170b5d7f25c40938bd287a5540b00e06ccb40f558958b72541e8ca4f4f965e4f78898085b34fdb6e33b1f588b6d0abc4cb119a8f54e0d949a08afb87979d4c69165ac6bd9e694369a3903ec24c1e3a52c401c88e035a9f6aed6909f3a2b6dbe60e6fa842400c4164c21dc4c8b2325b70ad1829bed742717776ff28457b384f4bdd0bf48b2db2d18f89af671c58ecded320cf289b8fa9cfd53fcd7352de1cff3c41d2f7f8ec6f280d8a9d6933da42b66a6a3d30d46742e9cd793388a07e5e15b9b220b4209415537214447d386abce2c74a24b7dc60ff9ce04a7cad19ab679d0234ac95e535abd57d3ac91747b2f2cfe1f01bb944502b827fc8d2c5e8f920fb1451880271991e5f5db796ea8d392138cd18f602dc6deb3149c44e5085fbd77dc997571e4652009b555253eefd215fb009b14e0e880f67d45e85a8252e457ddd0ace7cfdd5eec6cee070125b50307b7ab0f3983f32f58b75fb02133f3e0778c089484d07058e76025855909ff64b7c2ace114b6c302a087acc140be90679fe1d0a75300573dc000000000ffff0f20000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000007594c5be146f727d7fb815193044fb2596ceca3a9b62252e5259ed56b7fb63cd2fe906fac0f3ff25658998198d9431a48a0be55a0a84333fbdabab0c318930b97d3bb1fa8a8ddeb1587f97c531f81963c70784089465e2ef4f465b8d6bb9bbb27f36971c87b98ccae3f8d445181b03c97a84ac8a12241b47d9845f966cedade1c31faa857bf2cafae9c71041dd23124d4cd4d6dff24cf632e94dd68831639b0f3aba27219cd8a869936605760ae408cadeef02c410fc2eeb412bdd7e411614e7830f54ebe0ea6eadae5fe226a67c0b310d4d4b5d10b47dfe2f165191e69c96e617ef8c3cf763fa49662deb82a2270b49816f11d56a3493c5e74b0eafbd9492e5fbaa0e0d6600c179a75c2c134e1d6a1c3721616b6241273b904aec0ef516c402649d032d5e4de8a1fb15bbeb250f5b6993b6bf5a39314e626d177578fedcc3f7935307321f8f25ae008855b1f19ddf26bcfa1636b3db132a737b4e1ec50ac9b223670f04a746be5c06e1de90115385c706af7eb947b9b712f9c14998d31b977ace19a1f2051799fe7aa47bc02f358f2d839891854825a7e7491e343eb5aa2d468e787989abf9961e21956c5ced5c6a5375e809ad958235fc91989fa4141230c42ccbf6a50c6ca61e3780d65dbfc112a104cc1da5b1dd7ea024d2e37db0bb10ab6f06242589cb5383927ac5d130b189d32e4731ec1e8b675caf6c4da531db3c598c5da69aa8ddcecae67cefd633fd80f994cec4ad28c2f1421b316999c1043c749b14a645f785dd56e8fdbc959ff03648336b0c9c9ca3c86bb96738750b855dffa0b74c9c492580dcbbb892b91d76359aedc0a3d89a447b23f5449433bb7c4554eb6f0eb8ee63b9df12287f92eb23b3956d3933eeccf88ca9d9fe19a9a29a2821909f3a2b6dbe60e6fa842400c4164c21dc4c8b2325b70ad1829bed742717776ff28457b384f4bdd0bf48b2db2d18f89af671c58ecded320cf289b8fa9cfd53fcd7352de1cff3c41d2f7f8ec6f280d8a9d6933da42b66a6a3d30d46742e9cd793388a07e5e15b9b220b4209415537214447d386abce2c74a24b7dc60ff9ce04a7cad19ab679d0234ac95e535abd57d3ac91747b2f2cfe1f01bb944502b827fc8d2c5e8f920fb1451880271991e5f5db796ea8d392138cd18f602dc6deb3149c44e5085fbd77dc997571e4652009b555253eefd215fb009b14e0e880f67d45e85a8252e457ddd0ace7cfdd5eec6cee070125b50307b7ab0f3983f32f58b75fb021ace16c1a11a478a77f48ec8beda4f4912aa3337010343c14412cbc2f6d8ceb38dc88989cfee876ab00042a8000000000ffff0f20000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000009653aa497eb0bf1f7b9170967201419b6ced537def4363a0b2869d974a91d4458b4099f8d9a5f8555219c9b6efd193e1c745636d42cd705557c48e47598648c42e1c94318744855d037b3de60b626de12f06be4ec366527100b35ea8d4626eac5c2461d733c072811aa87bb5a39edf46d13a318f948367fe7a130359cd2a1ed04a60ee497723623b258cecd2581a4d7cc3d7e9d05ae4d63ffcecdd16a19decb7dcffc9a9faccb2084177e736170f191b99446049304f95a2dad137670c0944a41dd36cd356ad70f65eaba46732e7976b4d252980db9e82ff554a599aae46dd27886e61a22adf51dbf26be34bbc766510ddebb15a9bef63ba3052fe7f71252807582e08fa1301fd78138917fec593f50758f103966bcf45c32071a279367c90d2728d9d13a90c3ee64682b86b80738f4ad1cc94e8d2c98d70bc99e72b45a68f4719465bd291177ef8675eb9ab2cca7599bb8470180137e6d0e92dcd13fd60dfa8569175055e76d0df50c79447df8a0d6c64d1d240aae79168de62becc24097a5da77de3d860efbf3fbb7a737275944899df27a45b9a7203d813dad5c6ebd0986535a260589a51843ae43bf17902282439ce50ae75ab4ad8f994530750fc1b30d7dc364828b76275e3536950834c0afeb17ad04a0a3090cd4e1165b65727b08c939e355a5c992d87bd80c3a41465bf1b41d304646fbbfb6b350208282945b68d3a0440bb8d2dabf1b3767ccc02174499f4084be56f7733052ac65bec5401b9e627bb4094c8c5fad47a0afb5ab1a7db4de6e318f535013c8db58d16e5455fb0d2aa32a4d8e4d403412db7ecc718e459e81f09fde3523436ef6104f96201f1fa8c4251033198d39d0c5a87eae9b9499eb2b3551d4e579103de55354c95b4c3b0cee177cb443e85e4936100efb659bb7356a52f5d51682673e9cf655c9cec51d100979ffbf74922dfeaecf1bf1ac55933c73d5f3fe927674fd5afc5d5a85e5b8d9779d7352de1cff3c41d2f7f8ec6f280d8a9d6933da42b66a6a3d30d46742e9cd793388a07e5e15b9b220b4209415537214447d386abce2c74a24b7dc60ff9ce04a7cad19ab679d0234ac95e535abd57d3ac91747b2f2cfe1f01bb944502b827fc8d2c5e8f920fb1451880271991e5f5db796ea8d392138cd18f602dc6deb3149c44e5085fbd77dc997571e4652009b555253eefd215fb009b14e0e880f67d45e85a8252e457ddd0ace7cfdd5eec6cee070125b50307b7ab0f3983f32f58b75fb0213ab54f4815c5fb0803d5ddd6d4278fc7105e5a15aff36d31ba05dd094c5d2b1f59974dd4d04c369300cb318000000000ffff0f2000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000d120d39a00a6aeb9703eaa6410db4990a504e21cdc0ccc4f913441b647104b4f0b8b87661db287ccaa443f2920759e0b9524babb4e227c7cc6a0ee765ff26b15ac81d3e764d6e4f8527edf236288ca56196d55a51a8c2a7cb9f9fd7f235a459fb9f77454c0a0cfbd71605850dcb3ad5428614ef576b3cc358a2286bd7089a0459aea9c86741eb0e4e295ec976b94efcb4441e998c8e51758de78301ed490f799867355ecd7c57c1d6adfcb2f789f53f47ddd22fb6dad62b4d1b7315001c5b341a265587a38265e0e3ea811e53fbee01786efedc6bab28d0ece33016c96a7a52cc1c77cb8eb932020b883222dbb8a3c9209b7a8e9ef54828b205a63ce185fa813409d4589c203b782fae087f59141aca33b8a89af33314de4b215fb61821c03d76f0ac07d2d97e5cad8fe5864de4269ddb23e0cbf4b53170a4b43da80e7d128f07a471f4ed7e81a9d4ab038cd4cb570c810bd4386b882b29d965824d651fdade58fa18a231a2ad288ed5fb0a1716c45c24b80a332d5d8cd56d6f663b5b5bec1854bb2477b43bfa482d32577ebe6f775f1349c71fb98c49eccd2a6a984b29da8664e0715ce25b520e58622a207fd6f58b95a37b095308e25672bca89d742faebbf8e397d5847a50266d4c8f76bdb9306d105a8a7d83d20ab07a8769fc1c64ae92233115a91352458a11f329b2b227b07e7aac5439354fd30e4c1ef22ed6061bdd65020347eb495e40f7ed2d5e5dd6e6cbd34dcdb1078f771c3c93c8e2f989fd4af8e4704acdae9f0a71e154bf6d0ada9efd1fc6a176299a3ef71fa650484d1d7062835a92def53df596633bf39bf0383f30674ea81003187222c48d8d91989bfd41d40edde7b07c29f8da3e0446cc6f5c58f2941af4418658bc55c20dec60859c8e8f8545263179afdf5c1b48aedc0fb4b71bf00cd0e53e86d3af5350ba6ed0b283e2fbbe3333a2856b81f4db572f5193ef5c7561dd6c22e3c0b411fd711529e69bf05811b2e8ed4fcec0080b506394154245190535ebdf909fbaae9ced09b8f63f925e9170701598f9757e4db71546f4a4bbe4ad32be2f551f3841e3125881a4750ad6684076e0cf8a9565c3dfe5140b7b40f3578867a19cf652bef184f9ed2ad63bfa62e16bd8bb52232d76b171559acaa7c51d56103a83735f0d5b1ae3bc720e5085fbd77dc997571e4652009b555253eefd215fb009b14e0e880f67d45e85a8252e457ddd0ace7cfdd5eec6cee070125b50307b7ab0f3983f32f58b75fb0218228bfd8f3d022cd5a99786769f3a3e038e68fc7021fd54e8745ea09380d112f5846acb6b0b693a1ad015ae6d04e43116192dc9edcdcdf52b2ece486afccac3a84da182bc48b69b3dec842c1d5f76abe2f9155a322a03808f708af8b589bdd206c338a2fefa693bc9dc232bdb3c03d1fa32b1da8a4514de4fccb2df8c0ffa2036dc15a92cd13bcb938f3d76853db406ece5f3bbfc6adb556855af805acdf2b1784fba6e61c1288024f8609b9cee016f3b09c07b1e3257c03fc6f6a2bf40fd597d326d3eb2bb10c6a4412cb8e260153008a482f7315f2235a3ae044df7004944fddec3a3eba0095fcb7432c07752f662e57559217925a030083452f8322f71a201497ceb1aa8efea84504687932b1630f8440cd8b5b835424a99a6ba6ef531f0039c96dc9df6ddb1da17db6192d68265aa69fe8e7591d29f883799f4e8530085220cfe3d522c74c00ec447082de3f07f03e4cf6f427b0f2e54fa73d0ee631d7e632101d487173ab63a5a014250a34f900730eb4554c4fcaff9e11e9051a3d7142d74708aadc2e29e3dec6fa67563527027c92a77e85f39702b90f869548e8d203f4b9166fd7ea1032e793228ea8ed223fa6d69ffef6c9ceca87df21a33bf16d0095ccd7de5c20364a71f63933bc5e9f3269497e6bdc1969d6f4e2106a5ed1adcd971f9af95e595d00953c1527674ba6b82b0f8f6ce97ded33774c8defd97c5ff1efc54617984d68bde405e946062e16004f841e6d1cb21d25f844c947d9db391b6394537f0ee65b2670abcb51acb86515aa98155916420e00dadfa924a79604be0074b78bdba7439f6ac8a0b028c43947f32cf1bde6af3dc9ffc3b36837c2e20083968aa01025b298c3f70f00028c0ed271ba1f8a425d46a81e480ad932dce9f46a84d6ccfe205403ad32dc1b571683788d29b2db5a793410d9a5843fb29d60ab294e0ccc2f35bfe1593e112a44dd3408760054899838af83022b08c6b224b92da9961cf8e5c518c082f07b037c87f56d1c711e4564c8c3061b57767b6ffd2cb2f782d8a02db34ba0d94f6a0f8664af79fff0eac78b47b753df86cdb06ebe88017a391df9656bf69eac1536d4237d19b601b632f65c35b264d0b634d17e2d8882af7cf5859b752801210e474f50eb15a8e67cb2be55332de8c389d1beeddfc275a3efeeb25ef6eadc57f4ab65436f7600d93cd72a0ee92af81941141ba58b6e361510f10bf66ff61ca2a3b6e0c83114d96bf382431fa21c00c9d818dc76721ed0ed09838560630ce2e2fc3ff2796727f0ded2147f68c040bf0b06c99184f0b53b13e966dd46b6224663f591dcb06be2c15398ad79af6155478d888c0cec4d0f008f0469a084a21a006ad610832938232cd672079fd672c29cfe44a9fe28029e4474b1d0efdf09ca6c99958969864e1a0483236c9a496f6753bd1dae2169f4a4a665d28907e5347aa30b181fa891a3d13c97612292424a7d21f89806e9ae3161be2e1067f7e5821c352cf985af08d990b2d5595dcf6aee29ba8f6a906990bb2407447e64dc31fdbb925dba728427683ef16e6fcde7b982390314a10cc5bd8c3a3fc9d4b1544a966301dbfda478712ea9de748ed1120bd864dab49694680dfdf647cb5d263d0a591c737fd3815475cbf0006bf0b638870865f9118936e144b4e7315763a5e526450325e1966ed32af3ec4f5c07231e161f4f006d0b61cd3a747951d29a6af505a27264206786b8de5339ea1972c7e11027e77f90a5c9b11f5d2800490da63f1a94ffbb0bccc057f1be13eeae5cc8da783d3b84e2ae3aa424f54a663a4a9f9e67810f00b833ec0156377a6b96eb8b53e335f018af4b8be94118485b2d3b53652e890526d1a41bded7141400a8cc33116507392c3db3dddf3291d97543c77e9a2c616dfe130f23d0bc3733b0f2843d32c51d0e04e7932ad21ec5e9be6dd6b86e541e2323ccf8b209ad0940b7222d4aaa91d8837fe42cf46b785af711ea8c6600320be68fcd657241e8efb16dde17e25f5adcf601aed934acfb3a82a2245a46f8b224527eb3ca48beab1f052a044b9a7ef7d12a11c7e81bc72b0d3fce26f522a6180a762742d1e0ea79950a000f653cf348876d1b2a42b4c7524dc906089023d96eff593c6eb9f0f4ecbd324800000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff0100ffffffff020000008011040000220020d5c1005c0d4012d3ae2672319e7f9eb15a57516aeefabbbc062265f67e308f2b0000000000000000326a3009f91102ffff0f20f935b3001ef51ba8f24921a404bc376a0c713274bd1cc68c2c57f66f5c0be7ca001000000000000000000000"sv,
             {981000,
              "b9c7b3d95bdd5dbd5cc396ea64ced3011dee94d19a480790bd5288c81a8d7325"sv,
              "9fbaa91731883143a066b8aa239e71c6095efe07caa4501af2ff664de9894b34"sv,
              "ad3f11fe94a2c7f6b18f0c206712c7bab44eaeb8a4a469daaa009043eec092e1"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             137298172,
             64764,
             {
                 "seed.cjd.li"sv,
                 "seed.anode.co"sv,
                 "pktdseed.pkt.world"sv,
             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, true},
                 {Data::Style::P2WSH, true},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2WPKH, "P2WPKH"sv},
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::PKT_testnet,
         {
             Data::NOT_SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_SUPPORTED,
             4u,
             opentxs::UnitType::Tnpkt,
             Bip44Type::TESTNET,
             521142271,  // 0x1f0fffff
             "000000000000000000000000000000000000000000000000000000000000000000000000df345ba23b13467eec222a919d449dab6506abc555ef307794ecd3d36ac891fb00000000ffff0f1f00000000"sv,
             "852d43936f4c9606a4a063cf356c454f2d9c43b07a41cf52e59461a41217dc0b"sv,
             "TODO genesis block goes here"sv,
             {0,
              "852d43936f4c9606a4a063cf356c454f2d9c43b07a41cf52e59461a41217dc0b"sv,
              "0000000000000000000000000000000000000000000000000000000000000000526b0656def40fcb65ef87a75337001fae57a1d17dc17e103fb536cfddedd36c"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             118034940,
             64764,
             {
                 "testseed.cjd.li"sv,
                 "testseed.anode.co"sv,
                 "testseed.gridfinity.com"sv,
             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, true},
                 {Data::Style::P2WSH, true},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2WPKH, "P2WPKH"sv},
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::BitcoinSV,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Bsv,
             Bip44Type::BITCOINSV,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c"sv,
             "6fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {556767,
              "2b66b102d8f8966ca9d04cdaafe000fc6fd915427156d9010000000000000000"sv,
              "22938d4ece739b34d65de82f58c72c7a80d09bde4fd902010000000000000000"sv,
              "fb2e542d6e1aa15d763a3a1add8262fa7cb609063d8047372deae34118f8836a"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70016,
             3908297187,
             8333,
             {
                 "seed.bitcoinsv.io"sv,
                 "seed.cascharia.com"sv,
                 "seed.satoshisvision.network"sv,
             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             250,
         }},
        {blockchain::Type::BitcoinSV_testnet3,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Tnbsv,
             Bip44Type::TESTNET,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae18"sv,
             "43497fd7f826957108f4a30fd9cec3aeba79972084e90ead01ea330900000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae180101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {1479000,
              "940feeb5ea744fad8ec32ed620023bf6de3718d3d7108cad2203000000000000"sv,
              "173edd921667f2c301ad5fa37d1243ddf1789dad012ee2bde501000000000000"sv,
              "fb2e542d6e1aa15d763a3a1add8262fa7cb609063d8047372deae34118f8836a"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70016,
             4109624820,
             18333,
             {
                 "testnet-seed.bitcoinsv.io"sv,
                 "testnet-seed.cascharia.com"sv,
                 "testnet-seed.bitcoincloud.net"sv,
             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::eCash,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::MAINNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Xec,
             Bip44Type::ECASH,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c"sv,
             "6fe28c0ab6f1b372c1a6a246ae63f74f931e8365e15a089c68d6190000000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4a29ab5f49ffff001d1dac2b7c0101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {733000,
              "930e178e29b92a361958398535280c9f8f519be812c8d7000000000000000000"sv,
              "0bb90bbf107f7589fb751f1a462cb9cf8ab57388f77732160000000000000000"sv,
              "29da76259e5cbca15861b9fe33674e82373c9db1c2e6a98ce681d78ee2221034"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             3908297187,
             8333,
             {
                 "seed.bitcoinabc.org"sv,
                 "btccash-seeder.bitcoinunlimited.info"sv,
                 "seeder.jasonbcox.com"sv,
                 "seed.deadalnix.me"sv,
                 "seed.bchd.cash"sv,
                 "seeder.fabien.cash"sv,
                 "seeder.status.cash"sv,
             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::eCash_testnet3,
         {
             Data::SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::TnXec,
             Bip44Type::TESTNET,
             486604799,  // 0x1d00ffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae18"sv,
             "43497fd7f826957108f4a30fd9cec3aeba79972084e90ead01ea330900000000"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff001d1aa4ae180101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {1498000,
              "30c4eb7e3fb6a2ec8dac104e359508b157e42d8d566bd9451a5ae60b00000000"sv,
              "240c99f761a4880156f53e42dea2b80d661143fbdbf0b19719bac41000000000"sv,
              "a415dc83c631b6d020ea6695af5e2bef7b33830d4bc279b82a73218e3bc65b86"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             4109624820,
             18333,
             {
                 "testnet-seed.bitcoinabc.org"sv,
                 "testnet-seed.deadalnix.me"sv,
                 "testnet-seed.bchd.cash"sv,
                 "testnet-seeder.fabien.cash"sv,
                 "testnet-seeder.status.cash"sv,

             },
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, false},
                 {Data::Style::P2WSH, false},
                 {Data::Style::P2TR, false},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             100,
             25,
         }},
        {blockchain::Type::UnitTest,
         {
             Data::NOT_SUPPORTED,
             Data::FULL_SYNC_SUPPORTED,
             Data::TESTNET,
             Data::SEGWIT_NOT_SUPPORTED,
             0,
             opentxs::UnitType::Regtest,
             Bip44Type::TESTNET,
             545259519,  // 0x207fffff
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff7f2002000000"sv,
             "06226e46111a0b59caaf126043eb5bbf28c34f3a5e332a1fc7b2b73cf188910f"sv,
             "0100000000000000000000000000000000000000000000000000000000000000000000003ba3edfd7a7b12b27ac72c3e67768f617fc81bc3888a51323a9fb8aa4b1e5e4adae5494dffff7f20020000000101000000010000000000000000000000000000000000000000000000000000000000000000ffffffff4d04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73ffffffff0100f2052a01000000434104678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5fac00000000"sv,
             {0,
              "06226e46111a0b59caaf126043eb5bbf28c34f3a5e332a1fc7b2b73cf188910f"sv,
              "0000000000000000000000000000000000000000000000000000000000000000"sv,
              "5e0aa302450f931bc2e4fab27632231a06964277ea8dfcdd93c19149a24fe788"sv},
             cfilter::Type::ES,
             p2p::Protocol::bitcoin,
             70015,
             3669344250,
             18444,
             {},
             1000,
             100,
             {
                 {Data::Style::P2PKH, true},
                 {Data::Style::P2SH, true},
                 {Data::Style::P2WPKH, true},
                 {Data::Style::P2WSH, true},
                 {Data::Style::P2TR, true},
                 {Data::Style::ChecksummedHex, false},
             },
             {
                 {Data::Style::P2WPKH, "P2WPKH"sv},
                 {Data::Style::P2PKH, "P2PKH"sv},
             },
             10,
             25,
         }},
    };

    return data;
}

#if OT_BLOCKCHAIN
auto Filters() noexcept -> const FilterData&
{
    using namespace std::literals;
    static const auto data = FilterData{
        {blockchain::Type::Bitcoin,
         {
             {cfilter::Type::Basic_BIP158,
              {"9f3c30f0c37fb977cf3e1a3173c631e8ff119ad3088b6f5b2bced0802139c202"sv,
               "017fa880"sv}},
             {cfilter::Type::ES,
              {"fad52acc389a391c1d6d94e8984fe77323fbda24fb31299b88635d7bee0278e8"sv,
               "049dc75e903561289b0029337bcf4e6720"sv}},
         }},
        {blockchain::Type::BitcoinCash,
         {
             {cfilter::Type::Basic_BCHVariant,
              {"9f3c30f0c37fb977cf3e1a3173c631e8ff119ad3088b6f5b2bced0802139c202"sv,
               "017fa880"sv}},
             {cfilter::Type::ES,
              {"fad52acc389a391c1d6d94e8984fe77323fbda24fb31299b88635d7bee0278e8"sv,
               "049dc75e903561289b0029337bcf4e6720"sv}},
         }},
        {blockchain::Type::Litecoin,
         {
             {cfilter::Type::Basic_BIP158,
              {"8aa75530308cf8247a151c37c24e7aaa281ae3b5cecedb581aacb3a0d07c2451"sv,
               "019e8738"sv}},
             {cfilter::Type::ES,
              {"23b8dae37cf04c8a278bd50bcbcf23a03051ea902f67c4760eb35be96d428320"sv,
               "049de896b2cc882671e81f336fdf119b00"sv}},
         }},
        {blockchain::Type::Bitcoin_testnet3,
         {
             {cfilter::Type::Basic_BIP158,
              {"50b781aed7b7129012a6d20e2d040027937f3affaee573779908ebb779455821"sv,
               "019dfca8"sv}},
             {cfilter::Type::ES,
              {"995cfe5d055c9158c5a388b71fb2ddbe292c9ca2d30dca91359d8cbbe4603e02"sv,
               "04e2f5880d851afd74c662d38d49e29130"sv}},
         }},
        {blockchain::Type::BitcoinCash_testnet3,
         {
             {cfilter::Type::Basic_BCHVariant,
              {"50b781aed7b7129012a6d20e2d040027937f3affaee573779908ebb779455821"sv,
               "019dfca8"sv}},
             {cfilter::Type::ES,
              {"995cfe5d055c9158c5a388b71fb2ddbe292c9ca2d30dca91359d8cbbe4603e02"sv,
               "04e2f5880d851afd74c662d38d49e29130"sv}},
         }},
        {blockchain::Type::Litecoin_testnet4,
         {
             {cfilter::Type::Basic_BIP158,
              {"02d023da9d271b849f717089aad7e03a515dac982c9fb2cfd952e2ce1c618792"sv,
               "014c8c60"sv}},
             {cfilter::Type::ES,
              {"ad242bb97aaf6a8f973dc2054d5356a4fcc87f575b29bbb3e0d953cfaedff8c6"sv,
               "048b3d60cc5692c061eb30ca191005f1c0"sv}},
         }},
        {blockchain::Type::PKT,
         {
             {cfilter::Type::Basic_BIP158,
              {"526b0656def40fcb65ef87a75337001fae57a1d17dc17e103fb536cfddedd36c"sv,
               "01902168"sv}},
             {cfilter::Type::ES,
              {"155e1700eff3f9019ba1716316295a8753ec44d2a7730eee1c1c73e2b511e134"sv,
               "02649a42b26e818d40"sv}},
         }},
        {blockchain::Type::PKT_testnet,
         {
             {cfilter::Type::Basic_BIP158,
              {"526b0656def40fcb65ef87a75337001fae57a1d17dc17e103fb536cfddedd36c"sv,
               "01902168"sv}},
             {cfilter::Type::ES,
              {"155e1700eff3f9019ba1716316295a8753ec44d2a7730eee1c1c73e2b511e134"sv,
               "02649a42b26e818d40"sv}},
         }},
        {blockchain::Type::BitcoinSV,
         {
             {cfilter::Type::Basic_BCHVariant,
              {"9f3c30f0c37fb977cf3e1a3173c631e8ff119ad3088b6f5b2bced0802139c202"sv,
               "017fa880"sv}},
             {cfilter::Type::ES,
              {"fad52acc389a391c1d6d94e8984fe77323fbda24fb31299b88635d7bee0278e8"sv,
               "049dc75e903561289b0029337bcf4e6720"sv}},
         }},
        {blockchain::Type::BitcoinSV_testnet3,
         {
             {cfilter::Type::Basic_BCHVariant,
              {"50b781aed7b7129012a6d20e2d040027937f3affaee573779908ebb779455821"sv,
               "019dfca8"sv}},
             {cfilter::Type::ES,
              {"995cfe5d055c9158c5a388b71fb2ddbe292c9ca2d30dca91359d8cbbe4603e02"sv,
               "04e2f5880d851afd74c662d38d49e29130"sv}},
         }},
        {blockchain::Type::eCash,
         {
             {cfilter::Type::Basic_BCHVariant,
              {"9f3c30f0c37fb977cf3e1a3173c631e8ff119ad3088b6f5b2bced0802139c20"
               "2",
               "017fa880"}},
             {cfilter::Type::ES,
              {"fad52acc389a391c1d6d94e8984fe77323fbda24fb31299b88635d7bee0278e"
               "8",
               "049dc75e903561289b0029337bcf4e6720"}},
         }},
        {blockchain::Type::eCash_testnet3,
         {
             {cfilter::Type::Basic_BCHVariant,
              {"50b781aed7b7129012a6d20e2d040027937f3affaee573779908ebb77945582"
               "1",
               "019dfca8"}},
             {cfilter::Type::ES,
              {"995cfe5d055c9158c5a388b71fb2ddbe292c9ca2d30dca91359d8cbbe4603e0"
               "2",
               "04e2f5880d851afd74c662d38d49e29130"}},
         }},
        {blockchain::Type::UnitTest,
         {
             {cfilter::Type::Basic_BIP158,
              {"2b5adc66021d5c775f630efd91518cf6ce3e9f525bbf54d9f0d709451e305e48"sv,
               "014756c0"sv}},
             {cfilter::Type::Basic_BCHVariant,
              {"2b5adc66021d5c775f630efd91518cf6ce3e9f525bbf54d9f0d709451e305e48"sv,
               "014756c0"sv}},
             {cfilter::Type::ES,
              {"5e0aa302450f931bc2e4fab27632231a06964277ea8dfcdd93c19149a24fe788"sv,
               "042547f61f786604db036044c4f7f36fe0"sv}},
         }},
    };

    return data;
}

auto Services() noexcept -> const ServiceBits&
{
    static const auto data = ServiceBits{
        {blockchain::Type::Bitcoin,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit8, p2p::Service::Segwit2X},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
         }},
        {blockchain::Type::Bitcoin_testnet3,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit8, p2p::Service::Segwit2X},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
         }},
        {blockchain::Type::BitcoinCash,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::Graphene},
             {p2p::bitcoin::Service::Bit8, p2p::Service::WeakBlocks},
             {p2p::bitcoin::Service::Bit9, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit10, p2p::Service::XThinner},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
             {p2p::bitcoin::Service::Bit25, p2p::Service::Avalanche},
         }},
        {blockchain::Type::BitcoinCash_testnet3,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::Graphene},
             {p2p::bitcoin::Service::Bit8, p2p::Service::WeakBlocks},
             {p2p::bitcoin::Service::Bit9, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit10, p2p::Service::XThinner},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
             {p2p::bitcoin::Service::Bit25, p2p::Service::Avalanche},
         }},
        {blockchain::Type::Litecoin,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit8, p2p::Service::Segwit2X},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
         }},
        {blockchain::Type::Litecoin_testnet4,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit8, p2p::Service::Segwit2X},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
         }},
        {blockchain::Type::PKT,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit8, p2p::Service::Segwit2X},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
         }},
        {blockchain::Type::PKT_testnet,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit8, p2p::Service::Segwit2X},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
         }},
        {blockchain::Type::BitcoinSV,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::Graphene},
             {p2p::bitcoin::Service::Bit8, p2p::Service::WeakBlocks},
             {p2p::bitcoin::Service::Bit9, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit10, p2p::Service::XThinner},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
             {p2p::bitcoin::Service::Bit25, p2p::Service::Avalanche},
         }},
        {blockchain::Type::BitcoinSV_testnet3,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::Graphene},
             {p2p::bitcoin::Service::Bit8, p2p::Service::WeakBlocks},
             {p2p::bitcoin::Service::Bit9, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit10, p2p::Service::XThinner},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
             {p2p::bitcoin::Service::Bit25, p2p::Service::Avalanche},
         }},
        {blockchain::Type::eCash,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::Graphene},
             {p2p::bitcoin::Service::Bit8, p2p::Service::WeakBlocks},
             {p2p::bitcoin::Service::Bit9, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit10, p2p::Service::XThinner},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
             {p2p::bitcoin::Service::Bit25, p2p::Service::Avalanche},
         }},
        {blockchain::Type::eCash_testnet3,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::Graphene},
             {p2p::bitcoin::Service::Bit8, p2p::Service::WeakBlocks},
             {p2p::bitcoin::Service::Bit9, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit10, p2p::Service::XThinner},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
             {p2p::bitcoin::Service::Bit25, p2p::Service::Avalanche},
         }},
        {blockchain::Type::UnitTest,
         {
             {p2p::bitcoin::Service::None, p2p::Service::None},
             {p2p::bitcoin::Service::Bit1, p2p::Service::Network},
             {p2p::bitcoin::Service::Bit2, p2p::Service::UTXO},
             {p2p::bitcoin::Service::Bit3, p2p::Service::Bloom},
             {p2p::bitcoin::Service::Bit4, p2p::Service::Witness},
             {p2p::bitcoin::Service::Bit5, p2p::Service::XThin},
             {p2p::bitcoin::Service::Bit6, p2p::Service::BitcoinCash},
             {p2p::bitcoin::Service::Bit7, p2p::Service::CompactFilters},
             {p2p::bitcoin::Service::Bit8, p2p::Service::Segwit2X},
             {p2p::bitcoin::Service::Bit11, p2p::Service::Limited},
         }},
    };

    return data;
}
#endif  // OT_BLOCKCHAIN
}  // namespace opentxs::blockchain::params
