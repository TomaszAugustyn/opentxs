// Copyright (c) 2010-2021 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "0_stdafx.hpp"                           // IWYU pragma: associated
#include "1_Internal.hpp"                         // IWYU pragma: associated
#include "api/client/blockchain/BalanceTree.tpp"  // IWYU pragma: associated

#include <algorithm>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include "api/client/blockchain/BalanceTree.hpp"
#include "api/client/blockchain/BalanceTreeIndex.hpp"
#include "internal/api/Api.hpp"
#include "internal/api/client/blockchain/Blockchain.hpp"
#include "internal/api/client/blockchain/Factory.hpp"
#include "opentxs/Pimpl.hpp"
#include "opentxs/api/Factory.hpp"
#include "opentxs/api/client/blockchain/AddressStyle.hpp"
#include "opentxs/api/client/blockchain/BalanceNode.hpp"
#include "opentxs/api/client/blockchain/HD.hpp"
#include "opentxs/api/client/blockchain/Subchain.hpp"
#include "opentxs/api/storage/Storage.hpp"
#include "opentxs/core/Data.hpp"
#include "opentxs/core/Identifier.hpp"
#include "opentxs/core/identifier/Nym.hpp"
#include "opentxs/iterator/Bidirectional.hpp"
#include "opentxs/protobuf/Bip47Channel.pb.h"
#include "opentxs/protobuf/HDAccount.pb.h"

#define OT_METHOD                                                              \
    "opentxs::api::client::blockchain::implementation::BalanceTree::"

namespace opentxs::factory
{
auto BlockchainBalanceTree(
    const api::internal::Core& api,
    const api::client::blockchain::internal::BalanceList& parent,
    const api::client::internal::BalanceTreeIndex& index,
    const identifier::Nym& id,
    const std::set<OTIdentifier>& hd,
    const std::set<OTIdentifier>& imported,
    const std::set<OTIdentifier>& pc) noexcept
    -> std::unique_ptr<api::client::blockchain::internal::BalanceTree>
{
    using ReturnType = api::client::blockchain::implementation::BalanceTree;

    return std::make_unique<ReturnType>(
        api, parent, index, id, hd, imported, pc);
}
}  // namespace opentxs::factory

namespace opentxs::api::client::blockchain::implementation
{
BalanceTree::BalanceTree(
    const api::internal::Core& api,
    const internal::BalanceList& parent,
    const client::internal::BalanceTreeIndex& index,
    const identifier::Nym& nym,
    const Accounts& hd,
    const Accounts& imported,
    const Accounts& paymentCode) noexcept
    : api_(api)
    , parent_(parent)
    , account_index_(index)
    , chain_(parent.Chain())
    , nym_id_(nym)
    , account_id_([&] {
        auto out = api_.Factory().Identifier();
        auto preimage = api_.Factory().Data(nym_id_->Bytes());
        const auto chain = parent.Chain();
        preimage->Concatenate(&chain, sizeof(chain));
        out->CalculateDigest(preimage->Bytes());

        return out;
    }())
    , hd_(api_, *this)
    , imported_(api_, *this)
    , payment_code_(api_, *this)
    , node_index_()
    , lock_()
    , unspent_()
    , spent_()
{
    init_hd(hd);
    init_payment_code(paymentCode);
}

auto BalanceTree::NodeIndex::Add(
    const std::string& id,
    internal::BalanceNode* node) noexcept -> void
{
    OT_ASSERT(nullptr != node);

    Lock lock(lock_);
    index_[id] = node;
}

auto BalanceTree::NodeIndex::Find(const std::string& id) const noexcept
    -> internal::BalanceNode*
{
    Lock lock(lock_);

    try {

        return index_.at(id);
    } catch (...) {

        return nullptr;
    }
}

auto BalanceTree::AssociateTransaction(
    const std::vector<Activity>& unspent,
    const std::vector<Activity>& spent,
    std::set<OTIdentifier>& contacts,
    const PasswordPrompt& reason) const noexcept -> bool
{
    using ActivityVector = std::vector<Activity>;
    using ActivityPair = std::pair<ActivityVector, ActivityVector>;
    using ActivityMap = std::map<std::string, ActivityPair>;

    Lock lock(lock_);
    auto sorted = ActivityMap{};
    auto outputs = std::map<std::string, std::map<std::size_t, int>>{};

    for (const auto& [coin, key, amount] : unspent) {
        const auto& [transaction, output] = coin;
        const auto& [account, subchain, index] = key;

        if (1 < ++outputs[transaction][output]) { return false; }
        if (0 >= amount) { return false; }

        sorted[account].first.emplace_back(Activity{coin, key, amount});
    }

    for (const auto& [coin, key, amount] : spent) {
        const auto& [transaction, output] = coin;
        const auto& [account, subchain, index] = key;

        if (1 < ++outputs[transaction][output]) { return false; }
        if (0 >= amount) { return false; }

        sorted[account].second.emplace_back(Activity{coin, key, amount});
    }

    for (const auto& [accountID, value] : sorted) {
        auto* pNode = node_index_.Find(accountID);

        if (nullptr == pNode) {
            LogVerbose(OT_METHOD)(__FUNCTION__)(": Account ")(accountID)(
                " not found")
                .Flush();

            continue;
        }

        const auto& node = *pNode;
        const auto accepted = node.AssociateTransaction(
            value.first, value.second, contacts, reason);

        if (accepted) {
            for (const auto& [coin, key, amount] : value.first) {
                unspent_.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(coin),
                    std::forward_as_tuple(key, amount));
            }

            for (const auto& [coin, key, amount] : value.second) {
                spent_.emplace(
                    std::piecewise_construct,
                    std::forward_as_tuple(coin),
                    std::forward_as_tuple(key, amount));
            }
        } else {
            LogOutput(OT_METHOD)(__FUNCTION__)(
                ": Failed processing transaction")
                .Flush();

            return false;
        }
    }

    return true;
}

auto BalanceTree::ClaimAccountID(
    const std::string& id,
    internal::BalanceNode* node) const noexcept -> void
{
    node_index_.Add(id, node);
    account_index_.Register(AccountID(), nym_id_, chain_);
}

auto BalanceTree::find_next_element(
    Subchain subchain,
    const Identifier& contact,
    const std::string& label,
    const PasswordPrompt& reason) const noexcept(false) -> const Element&
{
    // TODO Add a mechanism for setting a default subaccount in case more than
    // one is present. Also handle cases where only an imported subaccount
    // exists

    for (const auto& account : hd_) {
        const auto index = account.Reserve(subchain, reason, contact, label);

        if (index.has_value()) {

            return account.BalanceElement(subchain, index.value());
        }
    }

    throw std::runtime_error("No available element for selected subchain");
}

auto BalanceTree::GetNextChangeKey(const PasswordPrompt& reason) const
    noexcept(false) -> const Element&
{
    static const auto blank = api_.Factory().Identifier();

    return find_next_element(Subchain::Internal, blank, "", reason);
}

auto BalanceTree::GetNextDepositKey(const PasswordPrompt& reason) const
    noexcept(false) -> const Element&
{
    static const auto blank = api_.Factory().Identifier();

    return find_next_element(Subchain::External, blank, "", reason);
}

auto BalanceTree::GetDepositAddress(
    const AddressStyle style,
    const PasswordPrompt& reason,
    const std::string& memo) const noexcept -> std::string
{
    static const auto blank = api_.Factory().Identifier();

    return GetDepositAddress(style, blank, reason, memo);
}

auto BalanceTree::GetDepositAddress(
    const AddressStyle style,
    const Identifier& contact,
    const PasswordPrompt& reason,
    const std::string& memo) const noexcept -> std::string
{
    const auto& element =
        find_next_element(Subchain::External, contact, memo, reason);

    return element.Address(style);
}

auto BalanceTree::init_hd(const Accounts& accounts) noexcept -> void
{
    for (const auto& accountID : accounts) {
        auto account = proto::HDAccount{};
        const auto loaded =
            api_.Storage().Load(nym_id_->str(), accountID->str(), account);

        if (false == loaded) { continue; }

        auto notUsed = Identifier::Factory();
        hd_.Construct(notUsed, account);
    }
}

auto BalanceTree::init_payment_code(const Accounts& accounts) noexcept -> void
{
    for (const auto& id : accounts) {
        auto account = proto::Bip47Channel{};
        const auto loaded = api_.Storage().Load(nym_id_, id, account);

        if (false == loaded) { continue; }

        auto notUsed = Identifier::Factory();
        payment_code_.Construct(notUsed, account);
    }
}

auto BalanceTree::LookupUTXO(const Coin& coin) const noexcept
    -> std::optional<std::pair<Key, Amount>>
{
    Lock lock(lock_);

    try {

        return unspent_.at(coin);
    } catch (...) {

        return {};
    }
}

auto BalanceTree::Node(const Identifier& id) const noexcept(false)
    -> internal::BalanceNode&
{
    auto* output = node_index_.Find(id.str());

    if (nullptr == output) { throw std::out_of_range("Account not found"); }

    return *output;
}
}  // namespace opentxs::api::client::blockchain::implementation
