// Copyright (c) 2018 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "stdafx.hpp"

#include "opentxs/core/Log.hpp"
#include "opentxs/network/zeromq/ReplyCallback.hpp"
#include "opentxs/network/zeromq/Frame.hpp"
#include "opentxs/network/zeromq/Message.hpp"
#include "opentxs/network/zeromq/FrameIterator.hpp"

#include "ReplySocket.hpp"

template class opentxs::Pimpl<opentxs::network::zeromq::ReplySocket>;

//#define OT_METHOD "opentxs::network::zeromq::implementation::ReplySocket::"

namespace opentxs::network::zeromq
{
OTZMQReplySocket ReplySocket::Factory(
    const class Context& context,
    const bool client,
    const ReplyCallback& callback)
{
    return OTZMQReplySocket(
        new implementation::ReplySocket(context, client, callback));
}
}  // namespace opentxs::network::zeromq

namespace opentxs::network::zeromq::implementation
{
ReplySocket::ReplySocket(
    const zeromq::Context& context,
    const bool client,
    const ReplyCallback& callback)
    : ot_super(context, SocketType::Reply)
    , CurveServer(lock_, socket_)
    , Receiver(lock_, socket_, true)
    , callback_(callback)
    , client_(client)
{
}

ReplySocket* ReplySocket::clone() const
{
    return new ReplySocket(context_, client_, callback_);
}

bool ReplySocket::have_callback() const { return true; }

void ReplySocket::process_incoming(const Lock& lock, Message& message)
{
    auto output = callback_.Process(message);
    Message& reply = output;
    send_message(lock, reply);
}

bool ReplySocket::Start(const std::string& endpoint) const
{
    Lock lock(lock_);

    if (client_) {

        return start_client(lock, endpoint);
    } else {

        return bind(lock, endpoint);
    }
}
}  // namespace opentxs::network::zeromq::implementation
