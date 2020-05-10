// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"    // IWYU pragma: associated
#include "1_Internal.hpp"  // IWYU pragma: associated
#include "blockchain/p2p/bitcoin/message/Nopayload.hpp"  // IWYU pragma: associated

#include "Factory.hpp"
#include "blockchain/p2p/bitcoin/Header.hpp"
#include "internal/blockchain/p2p/bitcoin/message/Message.hpp"  // IWYU pragma: keep

namespace bitcoin = opentxs::blockchain::p2p::bitcoin;
namespace message = bitcoin::message;

namespace opentxs
{
auto Factory::BitcoinP2PFilterclear(
    const api::internal::Core& api,
    std::unique_ptr<blockchain::p2p::bitcoin::Header> header)
    -> blockchain::p2p::bitcoin::message::internal::Filterclear*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Filterclear>;

    return new ReturnType(api, std::move(header));
}

auto Factory::BitcoinP2PFilterclear(
    const api::internal::Core& api,
    const blockchain::Type network)
    -> blockchain::p2p::bitcoin::message::internal::Filterclear*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Filterclear>;

    return new ReturnType(api, network, bitcoin::Command::filterclear);
}
auto Factory::BitcoinP2PGetaddr(
    const api::internal::Core& api,
    std::unique_ptr<blockchain::p2p::bitcoin::Header> header)
    -> blockchain::p2p::bitcoin::message::internal::Getaddr*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Getaddr>;

    return new ReturnType(api, std::move(header));
}

auto Factory::BitcoinP2PGetaddr(
    const api::internal::Core& api,
    const blockchain::Type network)
    -> blockchain::p2p::bitcoin::message::internal::Getaddr*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Getaddr>;

    return new ReturnType(api, network, bitcoin::Command::getaddr);
}

auto Factory::BitcoinP2PMempool(
    const api::internal::Core& api,
    std::unique_ptr<blockchain::p2p::bitcoin::Header> header)
    -> blockchain::p2p::bitcoin::message::internal::Mempool*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Mempool>;

    return new ReturnType(api, std::move(header));
}

auto Factory::BitcoinP2PMempool(
    const api::internal::Core& api,
    const blockchain::Type network)
    -> blockchain::p2p::bitcoin::message::internal::Mempool*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Mempool>;

    return new ReturnType(api, network, bitcoin::Command::mempool);
}

auto Factory::BitcoinP2PSendheaders(
    const api::internal::Core& api,
    std::unique_ptr<blockchain::p2p::bitcoin::Header> header)
    -> blockchain::p2p::bitcoin::message::internal::Sendheaders*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Sendheaders>;

    return new ReturnType(api, std::move(header));
}

auto Factory::BitcoinP2PSendheaders(
    const api::internal::Core& api,
    const blockchain::Type network)
    -> blockchain::p2p::bitcoin::message::internal::Sendheaders*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Sendheaders>;

    return new ReturnType(api, network, bitcoin::Command::sendheaders);
}

auto Factory::BitcoinP2PVerack(
    const api::internal::Core& api,
    std::unique_ptr<blockchain::p2p::bitcoin::Header> header)
    -> blockchain::p2p::bitcoin::message::internal::Verack*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Verack>;

    return new ReturnType(api, std::move(header));
}

auto Factory::BitcoinP2PVerack(
    const api::internal::Core& api,
    const blockchain::Type network)
    -> blockchain::p2p::bitcoin::message::internal::Verack*
{
    using ReturnType =
        message::implementation::Nopayload<message::internal::Verack>;

    return new ReturnType(api, network, bitcoin::Command::verack);
}
}  // namespace opentxs
