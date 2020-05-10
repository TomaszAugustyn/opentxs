// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <cstdint>
#include <iosfwd>
#include <map>
#include <mutex>
#include <string>
#include <tuple>

#include "internal/blockchain/p2p/bitcoin/Bitcoin.hpp"
#include "opentxs/Forward.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/api/Core.hpp"
#include "opentxs/api/Factory.hpp"
#include "opentxs/core/Data.hpp"

namespace opentxs
{
namespace api
{
namespace internal
{
struct Core;
}  // namespace internal
}  // namespace api

namespace network
{
namespace zeromq
{
class Frame;
}  // namespace zeromq
}  // namespace network

class Factory;
}  // namespace opentxs

namespace ot = opentxs;
namespace zmq = opentxs::network::zeromq;

namespace opentxs::blockchain::p2p::bitcoin
{
class Header final
{
public:
    struct BitcoinFormat {
        MagicField magic_;
        CommandField command_;
        PayloadSizeField length_;
        ChecksumField checksum_;

        auto Checksum() const noexcept -> OTData;
        auto Command() const noexcept -> bitcoin::Command;
        auto Magic() const noexcept -> bitcoin::Magic;
        auto PayloadSize() const noexcept -> std::size_t;

        BitcoinFormat(const Data& in) noexcept(false);
        BitcoinFormat(const zmq::Frame& in) noexcept(false);
        BitcoinFormat(
            const blockchain::Type network,
            const bitcoin::Command command,
            const std::size_t payload,
            const OTData checksum) noexcept(false);

    private:
        BitcoinFormat(const void* data, const std::size_t size) noexcept(false);
    };

    static auto Size() noexcept -> std::size_t { return sizeof(BitcoinFormat); }

    auto Command() const noexcept -> bitcoin::Command { return command_; }
    OPENTXS_EXPORT auto Encode() const noexcept -> OTData;
    auto Network() const noexcept -> blockchain::Type { return chain_; }
    auto PayloadSize() const noexcept -> std::size_t { return payload_size_; }
    auto Checksum() const noexcept -> const opentxs::Data& { return checksum_; }

    void SetChecksum(const std::size_t payload, OTData&& checksum) noexcept;

    Header(
        const api::internal::Core& api,
        const blockchain::Type network,
        const bitcoin::Command command) noexcept;

    ~Header() = default;

private:
    friend opentxs::Factory;

    blockchain::Type chain_{};
    bitcoin::Command command_{};
    std::size_t payload_size_{};
    OTData checksum_;

    Header(
        const api::internal::Core& api,
        const blockchain::Type network,
        const bitcoin::Command command,
        const std::size_t payload,
        const OTData checksum) noexcept;
    Header() = delete;
    Header(const Header&) = delete;
    Header(Header&&) = delete;
    auto operator=(const Header&) -> Header& = delete;
    auto operator=(Header &&) -> Header& = delete;
};
}  // namespace opentxs::blockchain::p2p::bitcoin
