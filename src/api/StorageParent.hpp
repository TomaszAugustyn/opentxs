// Copyright (c) 2010-2020 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "internal/api/storage/Storage.hpp"
#include "opentxs/Types.hpp"
#include "opentxs/Version.hpp"
#include "opentxs/core/String.hpp"
#include "opentxs/crypto/key/Symmetric.hpp"
#include "storage/StorageConfig.hpp"

namespace opentxs
{
namespace api
{
class Crypto;
class Factory;
class HDSeed;
class Legacy;
class Settings;
}  // namespace api

class Flag;
}  // namespace opentxs

namespace opentxs::api::implementation
{
class StorageParent
{
protected:
    const api::Crypto& crypto_;
    const api::Settings& config_;
    const ArgList args_;
    const std::chrono::seconds gc_interval_{0};
    const std::string data_folder_;
#if OT_QT
    const bool enable_qt_;
#endif
    StorageConfig storage_config_;
    bool migrate_storage_{false};
    OTString migrate_from_;
    OTString primary_storage_plugin_;
    OTString archive_directory_;
    OTString encrypted_directory_;
    std::unique_ptr<api::storage::StorageInternal> storage_;
#if OT_CRYPTO_WITH_BIP32
    OTSymmetricKey storage_encryption_key_;
#endif
    void init(
        const api::Factory& factory
#if OT_CRYPTO_WITH_BIP32
        ,
        const api::HDSeed& seeds
#endif  // OT_CRYPTO_WITH_BIP32
    );
    void start();

    StorageParent(
        const Flag& running,
        const ArgList& args,
        const api::Crypto& crypto,
        const api::Settings& config,
        const api::Legacy& legacy,
        const std::string& dataFolder);

    virtual ~StorageParent() = default;

private:
    static auto extract_arg(const std::string& name, const ArgList& args)
        -> OTString;
    static auto extract_archive_directory(const ArgList& args) -> OTString;
    static auto extract_encrypted_directory(const ArgList& args) -> OTString;
    static auto extract_primary_storage_plugin(const ArgList& args) -> OTString;
#if OT_QT
    static bool extract_qt(const ArgList& args);
#endif
    static auto get_primary_storage_plugin(
        const api::Settings& config,
        const StorageConfig& storageConfig,
        const ArgList args,
        bool& migrate,
        String& previous) -> OTString;

    StorageParent() = delete;
    StorageParent(const StorageParent&) = delete;
    StorageParent(StorageParent&&) = delete;
    auto operator=(const StorageParent&) -> StorageParent& = delete;
    auto operator=(StorageParent &&) -> StorageParent& = delete;
};
}  // namespace opentxs::api::implementation
