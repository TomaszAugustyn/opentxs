# Copyright (c) 2010-2022 The Open-Transactions developers
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

if(OT_BLOCKCHAIN_EXPORT)
  add_subdirectory(blockchain)
endif()

add_subdirectory(api)
add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(integration)
add_subdirectory(paymentcode)
add_subdirectory(rpc)
add_subdirectory(ui)
add_subdirectory(zeromq)
