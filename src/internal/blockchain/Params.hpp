// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: no_include "opentxs/blockchain/BlockchainType.hpp"
// IWYU pragma: no_include "opentxs/blockchain/bitcoin/cfilter/FilterType.hpp"
// IWYU pragma: no_include "opentxs/blockchain/crypto/AddressStyle.hpp"
// IWYU pragma: no_include "opentxs/identity/wot/claim/ClaimType.hpp"
// IWYU pragma: no_include "opentxs/core/UnitType.hpp"
// IWYU pragma: no_include "opentxs/crypto/Bip44Type.hpp"
// IWYU pragma: no_include <boost/intrusive/detail/iterator.hpp>

#pragma once

#include <boost/container/flat_map.hpp>
#include <boost/container/vector.hpp>
#include <boost/cstdint.hpp>
#include <boost/move/algo/move.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <string_view>
#include <tuple>
#include <utility>

#if OT_BLOCKCHAIN
#include "internal/blockchain/p2p/bitcoin/Bitcoin.hpp"
#endif  // OT_BLOCKCHAIN
#include "opentxs/Version.hpp"
#include "opentxs/blockchain/Types.hpp"
#include "opentxs/blockchain/bitcoin/cfilter/Types.hpp"
#include "opentxs/blockchain/block/Types.hpp"
#include "opentxs/blockchain/crypto/Types.hpp"
#include "opentxs/blockchain/p2p/Types.hpp"
#include "opentxs/core/Amount.hpp"
#include "opentxs/core/Types.hpp"
#include "opentxs/crypto/Types.hpp"
#include "opentxs/util/Container.hpp"

namespace opentxs::blockchain::params
{
struct Checkpoint {
    block::Height height_{};
    std::string_view block_hash_{};
    std::string_view previous_block_hash_{};
    std::string_view filter_header_{};
};

struct Data {
    using Style = blockchain::crypto::AddressStyle;
    using ScriptMap = boost::container::flat_map<Style, bool>;
    using StylePref = UnallocatedVector<std::pair<Style, std::string_view>>;

    inline static const bool SUPPORTED = true;
    inline static const bool FULL_SYNC_SUPPORTED = true;
    inline static const bool TESTNET = true;
    inline static const bool SEGWIT_SUPPORTED = true;
    inline static const bool NOT_SUPPORTED = false;
    inline static const bool FULL_SYNC_NOT_SUPPORTED = false;
    inline static const bool MAINNET = false;
    inline static const bool SEGWIT_NOT_SUPPORTED = false;

    bool supported_{};
    bool full_sync_support_{};
    bool testnet_{};
    bool segwit_{};
    unsigned segwit_scale_factor_{};
    UnitType itemtype_{};
    Bip44Type bip44_{};
    std::int32_t nBits_{};
    std::string_view genesis_header_hex_{};
    std::string_view genesis_hash_hex_{};
    std::string_view genesis_block_hex_{};
    Checkpoint checkpoint_{};
    cfilter::Type default_filter_type_{};
    p2p::Protocol p2p_protocol_{};
    p2p::bitcoin::ProtocolVersion p2p_protocol_version_{};
    std::uint32_t p2p_magic_bits_{};
    std::uint16_t default_port_{};
    UnallocatedVector<std::string_view> dns_seeds_{};
    // Satoshis per 1000 bytes. Something we have to check the reference node
    // implementation for, to see what fee rates will be relayed by default. It
    // can change from time to time though so it needs to be periodically
    // re-checked.
    Amount default_fee_rate_{};
    // A safety feature we use to ensure that we don't try to load too many
    // blocks in memory as we download them.
    std::size_t block_download_batch_{};
    ScriptMap scripts_{};
    StylePref styles_{};
    // Justus: it is always 100 for every blockchain that I know about, and it's
    // part of a blockchain's consensus definition.
    block::Height maturation_interval_{};
    // Used to seed the adaptive scan interval algorithm. It's something we have
    // to derive ourselves and its purpose is to keep the algorithm from
    // attempting to scan too many cfilters on its first run, before it has
    // collected any data regarding the average cfilter size.
    std::size_t cfilter_element_count_estimate_{};
};

using ChainData = boost::container::flat_map<blockchain::Type, Data>;

auto Chains() noexcept -> const ChainData&;

#if OT_BLOCKCHAIN
using FilterData = boost::container::flat_map<
    blockchain::Type,
    boost::container::
        flat_map<cfilter::Type, std::pair<std::string_view, std::string_view>>>;
using FilterTypes = boost::container::
    flat_map<Type, boost::container::flat_map<cfilter::Type, std::uint8_t>>;
using ServiceBits = std::map<
    blockchain::Type,
    boost::container::flat_map<p2p::bitcoin::Service, p2p::Service>>;

auto Bip158() noexcept -> const FilterTypes&;
auto Filters() noexcept -> const FilterData&;
auto Services() noexcept -> const ServiceBits&;
#endif  // OT_BLOCKCHAIN
}  // namespace opentxs::blockchain::params
