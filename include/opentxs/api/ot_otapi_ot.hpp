/************************************************************
*
*  ot_otapi_ot.hpp
*
*/

/************************************************************
 -----BEGIN PGP SIGNED MESSAGE-----
 Hash: SHA1

 *                 OPEN TRANSACTIONS
 *
 *       Financial Cryptography and Digital Cash
 *       Library, Protocol, API, Server, CLI, GUI
 *
 *       -- Anonymous Numbered Accounts.
 *       -- Untraceable Digital Cash.
 *       -- Triple-Signed Receipts.
 *       -- Cheques, Vouchers, Transfers, Inboxes.
 *       -- Basket Currencies, Markets, Payment Plans.
 *       -- Signed, XML, Ricardian-style Contracts.
 *       -- Scripted smart contracts.
 *
 *  Copyright (C) 2010-2013 by "Fellow Traveler" (A pseudonym)
 *
 *  EMAIL:
 *  FellowTraveler@rayservers.net
 *
 *  BITCOIN:  1NtTPVVjDsUfDWybS4BwvHpG2pdS9RnYyQ
 *
 *  KEY FINGERPRINT (PGP Key in license file):
 *  9DD5 90EB 9292 4B48 0484  7910 0308 00ED F951 BB8E
 *
 *  OFFICIAL PROJECT WIKI(s):
 *  https://github.com/FellowTraveler/Moneychanger
 *  https://github.com/FellowTraveler/Open-Transactions/wiki
 *
 *  WEBSITE:
 *  http://www.OpenTransactions.org/
 *
 *  Components and licensing:
 *   -- Moneychanger..A Java client GUI.....LICENSE:.....GPLv3
 *   -- otlib.........A class library.......LICENSE:...LAGPLv3
 *   -- otapi.........A client API..........LICENSE:...LAGPLv3
 *   -- opentxs/ot....Command-line client...LICENSE:...LAGPLv3
 *   -- otserver......Server Application....LICENSE:....AGPLv3
 *  Github.com/FellowTraveler/Open-Transactions/wiki/Components
 *
 *  All of the above OT components were designed and written by
 *  Fellow Traveler, with the exception of Moneychanger, which
 *  was contracted out to Vicky C (bitcointrader4@gmail.com).
 *  The open-source community has since actively contributed.
 *
 *  -----------------------------------------------------
 *
 *   LICENSE:
 *   This program is free software: you can redistribute it
 *   and/or modify it under the terms of the GNU Affero
 *   General Public License as published by the Free Software
 *   Foundation, either version 3 of the License, or (at your
 *   option) any later version.
 *
 *   ADDITIONAL PERMISSION under the GNU Affero GPL version 3
 *   section 7: (This paragraph applies only to the LAGPLv3
 *   components listed above.) If you modify this Program, or
 *   any covered work, by linking or combining it with other
 *   code, such other code is not for that reason alone subject
 *   to any of the requirements of the GNU Affero GPL version 3.
 *   (==> This means if you are only using the OT API, then you
 *   don't have to open-source your code--only your changes to
 *   Open-Transactions itself must be open source. Similar to
 *   LGPLv3, except it applies to software-as-a-service, not
 *   just to distributing binaries.)
 *
 *   Extra WAIVER for OpenSSL, Lucre, and all other libraries
 *   used by Open Transactions: This program is released under
 *   the AGPL with the additional exemption that compiling,
 *   linking, and/or using OpenSSL is allowed. The same is true
 *   for any other open source libraries included in this
 *   project: complete waiver from the AGPL is hereby granted to
 *   compile, link, and/or use them with Open-Transactions,
 *   according to their own terms, as long as the rest of the
 *   Open-Transactions terms remain respected, with regard to
 *   the Open-Transactions code itself.
 *
 *   Lucre License:
 *   This code is also "dual-license", meaning that Ben Lau-
 *   rie's license must also be included and respected, since
 *   the code for Lucre is also included with Open Transactions.
 *   See Open-Transactions/src/otlib/lucre/LUCRE_LICENSE.txt
 *   The Laurie requirements are light, but if there is any
 *   problem with his license, simply remove the Lucre code.
 *   Although there are no other blind token algorithms in Open
 *   Transactions (yet. credlib is coming), the other functions
 *   will continue to operate.
 *   See Lucre on Github:  https://github.com/benlaurie/lucre
 *   -----------------------------------------------------
 *   You should have received a copy of the GNU Affero General
 *   Public License along with this program.  If not, see:
 *   http://www.gnu.org/licenses/
 *
 *   If you would like to use this software outside of the free
 *   software license, please contact FellowTraveler.
 *   (Unfortunately many will run anonymously and untraceably,
 *   so who could really stop them?)
 *
 *   DISCLAIMER:
 *   This program is distributed in the hope that it will be
 *   useful, but WITHOUT ANY WARRANTY; without even the implied
 *   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *   PURPOSE.  See the GNU Affero General Public License for
 *   more details.

 -----BEGIN PGP SIGNATURE-----
 Version: GnuPG v1.4.9 (Darwin)

 iQIcBAEBAgAGBQJRSsfJAAoJEAMIAO35UbuOQT8P/RJbka8etf7wbxdHQNAY+2cC
 vDf8J3X8VI+pwMqv6wgTVy17venMZJa4I4ikXD/MRyWV1XbTG0mBXk/7AZk7Rexk
 KTvL/U1kWiez6+8XXLye+k2JNM6v7eej8xMrqEcO0ZArh/DsLoIn1y8p8qjBI7+m
 aE7lhstDiD0z8mwRRLKFLN2IH5rAFaZZUvj5ERJaoYUKdn4c+RcQVei2YOl4T0FU
 LWND3YLoH8naqJXkaOKEN4UfJINCwxhe5Ke9wyfLWLUO7NamRkWD2T7CJ0xocnD1
 sjAzlVGNgaFDRflfIF4QhBx1Ddl6wwhJfw+d08bjqblSq8aXDkmFA7HeunSFKkdn
 oIEOEgyj+veuOMRJC5pnBJ9vV+7qRdDKQWaCKotynt4sWJDGQ9kWGWm74SsNaduN
 TPMyr9kNmGsfR69Q2Zq/FLcLX/j8ESxU+HYUB4vaARw2xEOu2xwDDv6jt0j3Vqsg
 x7rWv4S/Eh18FDNDkVRChiNoOIilLYLL6c38uMf1pnItBuxP3uhgY6COm59kVaRh
 nyGTYCDYD2TK+fI9o89F1297uDCwEJ62U0Q7iTDp5QuXCoxkPfv8/kX6lS6T3y9G
 M9mqIoLbIQ1EDntFv7/t6fUTS2+46uCrdZWbQ5RjYXdrzjij02nDmJAm2BngnZvd
 kamH0Y/n11lCvo1oQxM+
 =uSzz
 -----END PGP SIGNATURE-----
 **************************************************************/

#ifndef __OT_OTAPI_OT_HPP__
#define __OT_OTAPI_OT_HPP__

#include "OTAPI.hpp"
#include "OT_ME.hpp"

#include <OTStorage.hpp>

#ifndef OT_USE_CXX11
#include <cstdlib>
#endif

#define OT_OTAPI_OT

using std::string;

inline string concat(const string& str1, const string& str2)
{
    return str1 + str2;
}
inline void print(const string& text)
{
    std::cout << text << "\n";
}
inline string to_string(const bool bValue)
{
    return bValue ? "true" : "false";
}
#if defined(OT_USE_CXX11) && !defined(ANDROID)
inline string to_string(const int32_t nValue)
{
    return std::to_string(nValue);
}
inline string to_string(const int64_t nValue)
{
    return std::to_string(nValue);
}
inline int32_t to_int(const string& strValue)
{
    return static_cast<int32_t>(std::stoi(strValue));
}
inline int64_t to_long(const string& strValue)
{
    return std::stoll(strValue);
}
#else
inline string to_string(const int32_t nValue)
{
    return OTAPI_Wrap::LongToString(nValue);
}
inline string to_string(const int64_t nValue)
{
    return OTAPI_Wrap::LongToString(nValue);
}
inline int32_t to_int(const string& strValue)
{
    return static_cast<int32_t>(std::atoi(strValue.c_str()));
}
inline int64_t to_long(const string& strValue)
{
    return OTAPI_Wrap::StringToLong(strValue);
}
#endif

class the_lambda_struct;

typedef std::map<string, opentxs::OTDB::OfferDataNym*> SubMap;
typedef std::map<string, SubMap*> MapOfMaps;
typedef int32_t (*LambdaFunc)(opentxs::OTDB::OfferDataNym& offer_data,
                              const int32_t nIndex, MapOfMaps& map_of_maps,
                              SubMap& sub_map, the_lambda_struct& extra_vals);

EXPORT OT_OTAPI_OT MapOfMaps* convert_offerlist_to_maps(
    opentxs::OTDB::OfferListNym& offerList);
EXPORT OT_OTAPI_OT int32_t
find_strange_offers(opentxs::OTDB::OfferDataNym& offer_data,
                    const int32_t nIndex, MapOfMaps& map_of_maps,
                    SubMap& sub_map,
                    the_lambda_struct& extra_vals); // if 10 offers are printed
                                                    // for the SAME market,
                                                    // nIndex will be 0..9
EXPORT OT_OTAPI_OT int32_t
iterate_nymoffers_maps(MapOfMaps& map_of_maps,
                       LambdaFunc the_lambda); // low level. map_of_maps must be
                                               // good. (assumed.)
EXPORT OT_OTAPI_OT int32_t
iterate_nymoffers_maps(MapOfMaps& map_of_maps, LambdaFunc the_lambda,
                       the_lambda_struct& extra_vals); // low level. map_of_maps
                                                       // must be good.
                                                       // (assumed.)
EXPORT OT_OTAPI_OT int32_t iterate_nymoffers_sub_map(MapOfMaps& map_of_maps,
                                                     SubMap& sub_map,
                                                     LambdaFunc the_lambda);
EXPORT OT_OTAPI_OT int32_t
iterate_nymoffers_sub_map(MapOfMaps& map_of_maps, SubMap& sub_map,
                          LambdaFunc the_lambda, the_lambda_struct& extra_vals);
EXPORT OT_OTAPI_OT opentxs::OTDB::OfferListNym* loadNymOffers(
    const string& serverID, const string& nymID);
EXPORT OT_OTAPI_OT int32_t
output_nymoffer_data(opentxs::OTDB::OfferDataNym& offer_data,
                     const int32_t nIndex, MapOfMaps& map_of_maps,
                     SubMap& sub_map,
                     the_lambda_struct& extra_vals); // if 10 offers are printed
                                                     // for the SAME market,
                                                     // nIndex will be 0..9

extern string Args;
extern string HisAcct;
extern string HisNym;
extern string HisPurse;
extern string MyAcct;
extern string MyNym;
extern string MyPurse;
extern string Server;

typedef enum {
    NO_FUNC = 0,
    CREATE_USER_ACCT = 1,
    DELETE_USER_ACCT = 2,
    CHECK_USER = 3,
    SEND_USER_MESSAGE = 4,
    SEND_USER_INSTRUMENT = 5,
    ISSUE_ASSET_TYPE = 6,
    ISSUE_BASKET = 7,
    CREATE_ASSET_ACCT = 8,
    DELETE_ASSET_ACCT = 9,
    ACTIVATE_SMART_CONTRACT = 10,
    TRIGGER_CLAUSE = 11,
    PROCESS_INBOX = 12,
    EXCHANGE_BASKET = 13,
    DEPOSIT_CASH = 14,
    EXCHANGE_CASH = 15,
    DEPOSIT_CHEQUE = 16,
    WITHDRAW_VOUCHER = 17,
    PAY_DIVIDEND = 18,
    WITHDRAW_CASH = 19,
    GET_CONTRACT = 20,
    SEND_TRANSFER = 21,
    GET_MARKET_LIST = 22,
    CREATE_MARKET_OFFER = 23,
    KILL_MARKET_OFFER = 24,
    KILL_PAYMENT_PLAN = 25,
    DEPOSIT_PAYMENT_PLAN = 26,
    GET_NYM_MARKET_OFFERS = 27,
    GET_MARKET_OFFERS = 28,
    GET_MARKET_RECENT_TRADES = 29,
    GET_MINT = 30,
    QUERY_ASSET_TYPES = 31,
    GET_BOX_RECEIPT = 32,
    ADJUST_USAGE_CREDITS = 33,
} OTAPI_Func_Type;

class the_lambda_struct
{
public:
    std::vector<string> the_vector; // used for returning a list of something.
    string the_asset_acct;    // for newoffer, we want to remove existing offers
                              // for the same accounts in certain cases.
    string the_currency_acct; // for newoffer, we want to remove existing offers
                              // for the same accounts in certain cases.
    string the_scale;         // for newoffer as well.
    string the_price;         // for newoffer as well.
    bool bSelling;            // for newoffer as well.

    the_lambda_struct();
};

class OTAPI_Func
{
public:
    OTAPI_Func_Type funcType;
    string serverID;
    string nymID;
    string nymID2;
    string assetID;
    string assetID2;
    string accountID;
    string accountID2;
    string basket;
    string strData;
    string strData2;
    string strData3;
    string strData4;
    string strData5;
    bool bBool;
    int32_t nData;
    int64_t lData;
    time64_t tData;
    int32_t nTransNumsNeeded;
    int32_t nRequestNum;

    OTAPI_Func();
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID); // 3 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_strParam); // 4 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_strParam,
               const int64_t p_lData); // 5 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_strParam,
               const string& p_strData); // 5 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_nymID2,
               const string& p_strData, const string& p_strData2); // 6 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_accountID,
               const string& p_strParam, const int64_t p_lData,
               const string& p_strData2); // 7 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_accountID,
               const string& p_strParam, const string& p_strData,
               const int64_t p_lData2); // 7 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_accountID,
               const string& p_strParam, const string& p_strData,
               const string& p_strData2); // 7 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& p_assetID,
               const string& p_basket, const string& p_accountID,
               const bool p_bBool, const int32_t p_nTransNumsNeeded); // 8 args
    OTAPI_Func(const OTAPI_Func_Type theType, const string& p_serverID,
               const string& p_nymID, const string& assetAccountID,
               const string& currencyAcctID, const string& scale,
               const string& minIncrement, const string& quantity,
               const string& price, const bool bSelling); // 10 args
    ~OTAPI_Func();

    EXPORT OT_OTAPI_OT static void CopyVariables();
    OT_OTAPI_OT void InitCustom();
    OT_OTAPI_OT int32_t Run();
    OT_OTAPI_OT string
    SendRequest(OTAPI_Func& theFunction, const string& IN_FUNCTION);
    OT_OTAPI_OT int32_t
    SendRequestLowLevel(OTAPI_Func& theFunction, const string& IN_FUNCTION);
    OT_OTAPI_OT string
    SendRequestOnce(OTAPI_Func& theFunction, const string& IN_FUNCTION,
                    const bool bIsTransaction, const bool bWillRetryAfterThis,
                    bool& bCanRetryAfterThis);
    OT_OTAPI_OT string
    SendTransaction(OTAPI_Func& theFunction, const string& IN_FUNCTION);
    OT_OTAPI_OT string SendTransaction(OTAPI_Func& theFunction,
                                       const string& IN_FUNCTION,
                                       const int32_t nTotalRetries);
};

#endif // __OT_OTAPI_OT_HPP__
