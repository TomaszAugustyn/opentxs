# Copyright (c) 2010-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

add_subdirectory(blockoracle)

if(OT_BLOCKCHAIN_EXPORT)
  add_subdirectory(filteroracle)
  add_subdirectory(manager)
  add_subdirectory(p2p)
  add_subdirectory(peermanager)
  add_subdirectory(wallet)
endif()

target_sources(opentxs-common PRIVATE "Genesis.cpp")
set(cxx-install-headers
    "${opentxs_SOURCE_DIR}/include/opentxs/blockchain/node/SendResult.hpp"
    "${opentxs_SOURCE_DIR}/include/opentxs/blockchain/node/Types.hpp"
)

if(OT_BLOCKCHAIN_EXPORT)
  target_sources(
    opentxs-common
    PRIVATE
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/Config.hpp"
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/Endpoints.hpp"
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/Factory.hpp"
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/HeaderOracle.hpp"
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/Job.hpp"
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/Mempool.hpp"
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/SpendPolicy.hpp"
      "${opentxs_SOURCE_DIR}/src/internal/blockchain/node/Types.hpp"
      "Common.cpp"
      "Config.cpp"
      "Endpoints.cpp"
      "HeaderOracle.cpp"
      "HeaderOracle.hpp"
      "Job.cpp"
      "Mempool.cpp"
      "Mempool.hpp"
      "UpdateTransaction.cpp"
      "UpdateTransaction.hpp"
  )
  list(
    APPEND
    cxx-install-headers
    "${opentxs_SOURCE_DIR}/include/opentxs/blockchain/node/HeaderOracle.hpp"
    "${opentxs_SOURCE_DIR}/include/opentxs/blockchain/node/Manager.hpp"
    "${opentxs_SOURCE_DIR}/include/opentxs/blockchain/node/TxoState.hpp"
    "${opentxs_SOURCE_DIR}/include/opentxs/blockchain/node/TxoTag.hpp"
    "${opentxs_SOURCE_DIR}/include/opentxs/blockchain/node/Wallet.hpp"
  )

  target_link_libraries(opentxs PUBLIC Boost::system)
  target_include_directories(
    opentxs-common SYSTEM
    PRIVATE "${opentxs_SOURCE_DIR}/deps/robin-hood/src/include"
  )

  if(PACKETCRYPT_EXPORT)
    target_sources(
      opentxs-common PRIVATE "blockoracle/GetValidator_packetcrypt.cpp"
    )
  else()
    target_sources(opentxs-common PRIVATE "blockoracle/GetValidator.cpp")
  endif()
endif()

target_sources(opentxs-common PRIVATE ${cxx-install-headers})

if(OPENTXS_STANDALONE)
  install(
    FILES ${cxx-install-headers}
    DESTINATION include/opentxs/blockchain/node
    COMPONENT dev
  )
endif()
