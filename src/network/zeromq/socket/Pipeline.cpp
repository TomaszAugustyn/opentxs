// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

// IWYU pragma: no_include <cxxabi.h>

#include "0_stdafx.hpp"                        // IWYU pragma: associated
#include "1_Internal.hpp"                      // IWYU pragma: associated
#include "network/zeromq/socket/Pipeline.hpp"  // IWYU pragma: associated

#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>

#include "internal/network/zeromq/Batch.hpp"
#include "internal/network/zeromq/Context.hpp"
#include "internal/network/zeromq/Thread.hpp"
#include "internal/network/zeromq/message/Message.hpp"  // IWYU pragma: keep
#include "internal/network/zeromq/socket/Factory.hpp"
#include "internal/network/zeromq/socket/Raw.hpp"
#include "internal/util/LogMacros.hpp"
#include "opentxs/network/zeromq/Context.hpp"
#include "opentxs/network/zeromq/ListenCallback.hpp"
#include "opentxs/network/zeromq/ZeroMQ.hpp"
#include "opentxs/network/zeromq/message/Message.hpp"
#include "opentxs/network/zeromq/socket/SocketType.hpp"
#include "opentxs/network/zeromq/socket/Types.hpp"
#include "opentxs/util/Container.hpp"
#include "util/Gatekeeper.hpp"

namespace opentxs::factory
{
auto Pipeline(
    const network::zeromq::Context& context,
    std::function<void(network::zeromq::Message&&)> callback,
    const network::zeromq::EndpointArgs& subscribe,
    const network::zeromq::EndpointArgs& pull,
    const network::zeromq::EndpointArgs& dealer,
    const Vector<network::zeromq::SocketData>& extra) noexcept
    -> opentxs::network::zeromq::Pipeline
{
    return std::make_unique<network::zeromq::implementation::Pipeline>(
               context, std::move(callback), subscribe, pull, dealer, extra)
        .release();
}
}  // namespace opentxs::factory

namespace opentxs::network::zeromq
{
auto swap(Pipeline& lhs, Pipeline& rhs) noexcept -> void { lhs.swap(rhs); }
}  // namespace opentxs::network::zeromq

namespace opentxs::network::zeromq::implementation
{
Pipeline::Pipeline(
    const zeromq::Context& context,
    std::function<void(zeromq::Message&&)> callback,
    const EndpointArgs& subscribe,
    const EndpointArgs& pull,
    const EndpointArgs& dealer,
    const Vector<SocketData>& extra) noexcept
    : Pipeline(
          context,
          std::move(callback),
          MakeArbitraryInproc(),
          MakeArbitraryInproc(),
          subscribe,
          pull,
          dealer,
          extra)
{
}

Pipeline::Pipeline(
    const zeromq::Context& context,
    std::function<void(zeromq::Message&&)> callback,
    const UnallocatedCString internalEndpoint,
    const UnallocatedCString outgoingEndpoint,
    const EndpointArgs& subscribe,
    const EndpointArgs& pull,
    const EndpointArgs& dealer,
    const Vector<SocketData>& extra) noexcept
    : context_(context)
    , gate_()
    , shutdown_(false)
    , handle_(context_.Internal().MakeBatch([&] {
        auto out = UnallocatedVector<socket::Type>{
            socket::Type::Subscribe,  // NOTE sub_
            socket::Type::Pull,       // NOTE pull_
            socket::Type::Pair,       // NOTE outgoing_
            socket::Type::Dealer,     // NOTE dealer_
            socket::Type::Pair,       // NOTE internal_
        };

        for (const auto& [type, args] : extra) { out.emplace_back(type); }

        return out;
    }()))
    , batch_([&]() -> auto& {
        auto& batch = handle_.batch_;
        batch.listen_callbacks_.emplace_back(ListenCallback::Factory(callback));

        return batch;
    }())
    , sub_([&]() -> auto& {
        auto& socket = batch_.sockets_.at(0);
        auto rc = socket.ClearSubscriptions();

        OT_ASSERT(rc);

        apply(subscribe, socket);

        return socket;
    }())
    , pull_([&]() -> auto& {
        auto& socket = batch_.sockets_.at(1);
        apply(pull, socket);

        return socket;
    }())
    , outgoing_([&]() -> auto& {
        auto& socket = batch_.sockets_.at(2);
        const auto rc = socket.Bind(outgoingEndpoint.c_str());

        OT_ASSERT(rc);

        return socket;
    }())
    , dealer_([&]() -> auto& {
        auto& socket = batch_.sockets_.at(3);
        apply(dealer, socket);

        return socket;
    }())
    , internal_([&]() -> auto& {
        auto& socket = batch_.sockets_.at(4);
        const auto rc = socket.Bind(internalEndpoint.c_str());

        OT_ASSERT(rc);

        return socket;
    }())
    , to_dealer_([&] {
        auto socket = factory::ZMQSocket(context_, socket::Type::Pair);
        const auto rc = socket.Connect(outgoingEndpoint.c_str());

        OT_ASSERT(rc);

        return socket;
    }())
    , to_internal_([&] {
        auto socket = factory::ZMQSocket(context_, socket::Type::Pair);
        const auto rc = socket.Connect(internalEndpoint.c_str());

        OT_ASSERT(rc);

        return socket;
    }())
    , thread_(context_.Internal().Start(batch_.id_, [&] {
        auto out = StartArgs{
            {outgoing_.ID(),
             &outgoing_,
             [id = outgoing_.ID(), socket = &dealer_](auto&& m) {
                 socket->Send(std::move(m));
             }},
            {internal_.ID(),
             &internal_,
             [id = internal_.ID(),
              &cb = batch_.listen_callbacks_.at(0).get()](auto&& m) {
                 m.Internal().Prepend(id);
                 cb.Process(std::move(m));
             }},
            {dealer_.ID(),
             &dealer_,
             [id = dealer_.ID(),
              &cb = batch_.listen_callbacks_.at(0).get()](auto&& m) {
                 m.Internal().Prepend(id);
                 cb.Process(std::move(m));
             }},
            {pull_.ID(),
             &pull_,
             [id = pull_.ID(),
              &cb = batch_.listen_callbacks_.at(0).get()](auto&& m) {
                 m.Internal().Prepend(id);
                 cb.Process(std::move(m));
             }},
            {sub_.ID(),
             &sub_,
             [id = sub_.ID(),
              &cb = batch_.listen_callbacks_.at(0).get()](auto&& m) {
                 m.Internal().Prepend(id);
                 cb.Process(std::move(m));
             }},
        };

        OT_ASSERT(batch_.sockets_.size() == fixed_sockets_ + extra.size());

        // NOTE adjust to the last fixed socket because the iterator will be
        // preincremented
        auto s = std::next(batch_.sockets_.begin(), fixed_sockets_ - 1u);

        for (const auto& [type, args] : extra) {
            auto& socket = *(++s);
            apply(args, socket);
            out.emplace_back(
                socket.ID(),
                &socket,
                [id = socket.ID(),
                 &cb = batch_.listen_callbacks_.at(0).get()](auto&& m) {
                    m.Internal().Prepend(id);
                    cb.Process(std::move(m));
                });
        }

        return out;
    }()))
{
    OT_ASSERT(nullptr != thread_);
}

auto Pipeline::apply(const EndpointArgs& endpoint, socket::Raw& socket) noexcept
    -> void
{
    for (const auto& [endpoint, direction] : endpoint) {
        if (socket::Direction::Connect == direction) {
            const auto rc = socket.Connect(endpoint.c_str());

            OT_ASSERT(rc);
        } else {
            const auto rc = socket.Bind(endpoint.c_str());

            OT_ASSERT(rc);
        }
    }
}

auto Pipeline::BatchID() const noexcept -> std::size_t { return batch_.id_; }

auto Pipeline::bind(
    SocketID id,
    const std::string_view endpoint,
    std::function<Message(bool)> notify) const noexcept
    -> std::pair<bool, std::future<bool>>
{
    const auto done = gate_.get();

    if (done) {
        auto null = std::promise<bool>{};
        null.set_value(false);

        return std::make_pair(false, null.get_future());
    }

    return thread_->Modify(
        id,
        [ep = CString{endpoint},
         extra = std::move(notify),
         &cb = batch_.listen_callbacks_.at(0).get()](auto& socket) {
            const auto value = socket.Bind(ep.c_str());

            if (extra) { cb.Process(extra(value)); }
        });
}

auto Pipeline::BindSubscriber(
    const std::string_view endpoint,
    std::function<Message(bool)> notify) const noexcept -> bool
{
    return bind(sub_.ID(), endpoint, std::move(notify)).first;
}

auto Pipeline::Close() const noexcept -> bool
{
    gate_.shutdown();

    if (auto sent = shutdown_.exchange(true); false == sent) {
        batch_.ClearCallbacks();
    }

    return true;
}

auto Pipeline::connect(
    SocketID id,
    const std::string_view endpoint,
    std::function<Message(bool)> notify) const noexcept
    -> std::pair<bool, std::future<bool>>
{
    const auto done = gate_.get();

    if (done) {
        auto null = std::promise<bool>{};
        null.set_value(false);

        return std::make_pair(false, null.get_future());
    }

    return thread_->Modify(
        id,
        [ep = CString{endpoint},
         extra = std::move(notify),
         &cb = batch_.listen_callbacks_.at(0).get()](auto& socket) {
            const auto value = socket.Connect(ep.c_str());

            if (extra) { cb.Process(extra(value)); }
        });
}

auto Pipeline::ConnectDealer(
    const std::string_view endpoint,
    std::function<Message(bool)> notify) const noexcept -> bool
{
    return connect(dealer_.ID(), endpoint, std::move(notify)).first;
}

auto Pipeline::ConnectionIDDealer() const noexcept -> std::size_t
{
    return dealer_.ID();
}

auto Pipeline::ConnectionIDInternal() const noexcept -> std::size_t
{
    return internal_.ID();
}

auto Pipeline::ConnectionIDPull() const noexcept -> std::size_t
{
    return pull_.ID();
}

auto Pipeline::ConnectionIDSubscribe() const noexcept -> std::size_t
{
    return sub_.ID();
}

auto Pipeline::ExtraSocket(std::size_t index) noexcept(false) -> socket::Raw&
{
    if ((batch_.sockets_.size() - fixed_sockets_) < (index + 1u)) {

        throw std::out_of_range{"invalid extra socket index"};
    }

    auto it = std::next(batch_.sockets_.begin(), fixed_sockets_ + index);

    return *it;
}

auto Pipeline::PullFrom(const std::string_view endpoint) const noexcept -> bool
{
    return connect(pull_.ID(), endpoint).first;
}

auto Pipeline::Push(zeromq::Message&& msg) const noexcept -> bool
{
    const auto done = gate_.get();

    if (done) { return false; }

    to_internal_.modify_detach([data = std::move(msg)](auto& socket) mutable {
        socket.Send(std::move(data));
    });

    return true;
}

auto Pipeline::Send(zeromq::Message&& msg) const noexcept -> bool
{
    const auto done = gate_.get();

    if (done) { return false; }

    to_dealer_.modify_detach([data = std::move(msg)](auto& socket) mutable {
        socket.Send(std::move(data));
    });

    return true;
}

auto Pipeline::SubscribeTo(const std::string_view endpoint) const noexcept
    -> bool
{
    return connect(sub_.ID(), endpoint).first;
}

Pipeline::~Pipeline() { Close(); }
}  // namespace opentxs::network::zeromq::implementation

namespace opentxs::network::zeromq
{
Pipeline::Pipeline(Imp* imp) noexcept
    : imp_(imp)
{
    OT_ASSERT(nullptr != imp_);
}

Pipeline::Pipeline(Pipeline&& rhs) noexcept
    : Pipeline(std::make_unique<Imp>().release())
{
    swap(rhs);
}

auto Pipeline::operator=(Pipeline&& rhs) noexcept -> Pipeline&
{
    swap(rhs);

    return *this;
}

auto Pipeline::BatchID() const noexcept -> std::size_t
{
    return imp_->BatchID();
}

auto Pipeline::BindSubscriber(
    const std::string_view endpoint,
    std::function<Message(bool)> notify) const noexcept -> bool
{
    return imp_->BindSubscriber(endpoint, std::move(notify));
}

auto Pipeline::Close() const noexcept -> bool { return imp_->Close(); }

auto Pipeline::ConnectDealer(
    const std::string_view endpoint,
    std::function<Message(bool)> notify) const noexcept -> bool
{
    return imp_->ConnectDealer(endpoint, std::move(notify));
}

auto Pipeline::ConnectionIDDealer() const noexcept -> std::size_t
{
    return imp_->ConnectionIDDealer();
}

auto Pipeline::ConnectionIDInternal() const noexcept -> std::size_t
{
    return imp_->ConnectionIDInternal();
}

auto Pipeline::ConnectionIDPull() const noexcept -> std::size_t
{
    return imp_->ConnectionIDPull();
}

auto Pipeline::ConnectionIDSubscribe() const noexcept -> std::size_t
{
    return imp_->ConnectionIDSubscribe();
}

auto Pipeline::Internal() const noexcept -> const internal::Pipeline&
{
    return *imp_;
}

auto Pipeline::Internal() noexcept -> internal::Pipeline& { return *imp_; }

auto Pipeline::PullFrom(const std::string_view endpoint) const noexcept -> bool
{
    return imp_->PullFrom(endpoint);
}

auto Pipeline::Push(Message&& msg) const noexcept -> bool
{
    return imp_->Push(std::move(msg));
}

auto Pipeline::Send(Message&& msg) const noexcept -> bool
{
    return imp_->Send(std::move(msg));
}

auto Pipeline::SubscribeTo(const std::string_view endpoint) const noexcept
    -> bool
{
    return imp_->SubscribeTo(endpoint);
}

auto Pipeline::swap(Pipeline& rhs) noexcept -> void
{
    std::swap(imp_, rhs.imp_);
}

Pipeline::~Pipeline()
{
    if (nullptr != imp_) {
        delete imp_;
        imp_ = nullptr;
    }
}
}  // namespace opentxs::network::zeromq
