// Copyright (c) 2010-2022 The Open-Transactions developers
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "opentxs/Version.hpp"                   // IWYU pragma: associated
#include "opentxs/identity/wot/claim/Types.hpp"  // IWYU pragma: associated

#include <cstdint>

namespace opentxs::v1::identity::wot::claim
{
enum class ClaimType : std::uint32_t {
    Error = 0,
    Individual = 1,
    Organization = 2,
    Business = 3,
    Government = 4,
    Server = 5,
    Prefix = 6,
    Forename = 7,
    Middlename = 8,
    Surname = 9,
    Pedigree = 10,
    Suffix = 11,
    Nickname = 12,
    Commonname = 13,
    Passport = 14,
    National = 15,
    Provincial = 16,
    Military = 17,
    Pgp = 18,
    Otr = 19,
    Ssl = 20,
    Physical = 21,
    Official = 22,
    Birthplace = 23,
    Home = 24,
    Website = 25,
    Opentxs = 26,
    Phone = 27,
    Email = 28,
    Skype = 29,
    Wire = 30,
    Qq = 31,
    Bitmessage = 32,
    Whatsapp = 33,
    Telegram = 34,
    Kik = 35,
    Bbm = 36,
    Wechat = 37,
    Kakaotalk = 38,
    Facebook = 39,
    Google = 40,
    Linkedin = 41,
    Vk = 42,
    Aboutme = 43,
    Onename = 44,
    Twitter = 45,
    Medium = 46,
    Tumblr = 47,
    Yahoo = 48,
    Myspace = 49,
    Meetup = 50,
    Reddit = 51,
    Hackernews = 52,
    Wikipedia = 53,
    Angellist = 54,
    Github = 55,
    Bitbucket = 56,
    Youtube = 57,
    Vimeo = 58,
    Twitch = 59,
    Snapchat = 60,
    Vine = 61,
    Instagram = 62,
    Pinterest = 63,
    Imgur = 64,
    Flickr = 65,
    Dribble = 66,
    Behance = 67,
    Deviantart = 68,
    Spotify = 69,
    Itunes = 70,
    Soundcloud = 71,
    Askfm = 72,
    Ebay = 73,
    Etsy = 74,
    Openbazaar = 75,
    Xboxlive = 76,
    Playstation = 77,
    Secondlife = 78,
    Warcraft = 79,
    Alias = 80,
    Acquaintance = 81,
    Friend = 82,
    Spouse = 83,
    Sibling = 84,
    Member = 85,
    Colleague = 86,
    Parent = 87,
    Child = 88,
    Employer = 89,
    Employee = 90,
    Citizen = 91,
    Photo = 92,
    Gender = 93,
    Height = 94,
    Weight = 95,
    Hair = 96,
    Eye = 97,
    Skin = 98,
    Ethnicity = 99,
    Language = 100,
    Degree = 101,
    Certification = 102,
    Title = 103,
    Skill = 104,
    Award = 105,
    Likes = 106,
    Sexual = 107,
    Political = 108,
    Religious = 109,
    Birth = 110,
    Secondarygraduation = 111,
    Universitygraduation = 112,
    Wedding = 113,
    Accomplishment = 114,
    Btc = 115,
    Eth = 116,
    Xrp = 117,
    Ltc = 118,
    Dao = 119,
    Xem = 120,
    Dash = 121,
    Maid = 122,
    Lsk = 123,
    Doge = 124,
    Dgd = 125,
    Xmr = 126,
    Waves = 127,
    Nxt = 128,
    Sc = 129,
    Steem = 130,
    Amp = 131,
    Xlm = 132,
    Fct = 133,
    Bts = 134,
    Usd = 135,
    Eur = 136,
    Gbp = 137,
    Inr = 138,
    Aud = 139,
    Cad = 140,
    Sgd = 141,
    Chf = 142,
    Myr = 143,
    Jpy = 144,
    Cny = 145,
    Nzd = 146,
    Thb = 147,
    Huf = 148,
    Aed = 149,
    Hkd = 150,
    Mxn = 151,
    Zar = 152,
    Php = 153,
    Sek = 154,
    Tnbtc = 155,
    Tnxrp = 156,
    Tnltx = 157,
    Tnxem = 158,
    Tndash = 159,
    Tnmaid = 160,
    Tnlsk = 161,
    Tndoge = 162,
    Tnxmr = 163,
    Tnwaves = 164,
    Tnnxt = 165,
    Tnsc = 166,
    Tnsteem = 167,
    Philosophy = 168,
    Met = 169,
    Fan = 170,
    Supervisor = 171,
    Subordinate = 172,
    Contact = 173,
    Refreshed = 174,
    Bot = 175,
    Bch = 176,
    Tnbch = 177,
    Owner = 178,
    Property = 179,
    Unknown = 180,
    Ethereum_olympic = 181,
    Ethereum_classic = 182,
    Ethereum_expanse = 183,
    Ethereum_morden = 184,
    Ethereum_ropsten = 185,
    Ethereum_rinkeby = 186,
    Ethereum_kovan = 187,
    Ethereum_sokol = 188,
    Ethereum_poa = 189,
    Pkt = 190,
    Tnpkt = 191,
    Regtest = 192,
    Bnb = 193,    // Binance Coin
    Sol = 194,    // Solana
    Usdt = 195,   // Tether
    Ada = 196,    // Cardano
    Dot = 197,    // Polkadot
    Usdc = 198,   // USD Coin
    Shib = 199,   // SHIBA INU
    Luna = 200,   // Terra
    Avax = 201,   // Avalanche
    Uni = 202,    // Uniswap
    Link = 203,   // Chainlink
    Wbtc = 204,   // Wrapped Bitcoin
    Busd = 205,   // Binance USD
    Matic = 206,  // Polygon
    Algo = 207,   // Algorand
    Vet = 208,    // VeChain
    Axs = 209,    // Axie Infinity
    Icp = 210,    // Internet Computer
    Cro = 211,    // Crypto.com Coin
    Atom = 212,   // Cosmos
    Theta = 213,  // THETA
    Fil = 214,    // Filecoin
    Trx = 215,    // TRON
    Ftt = 216,    // FTX Token
    Etc = 217,    // Ethereum Classic
    Ftm = 218,    // Fantom
    Dai = 219,    // Dai
    Btcb = 220,   // Bitcoin BEP2
    Egld = 221,   // Elrond
    Hbar = 222,   // Hedera
    Xtz = 223,    // Tezos
    Mana = 224,   // Decentraland
    Near = 225,   // NEAR Protocol
    Grt = 226,    // The Graph
    Cake = 227,   // PancakeSwap
    Eos = 228,    // EOS
    Flow = 229,   // Flow
    Aave = 230,   // Aave
    Klay = 231,   // Klaytn
    Ksm = 232,    // Kusama
    Xec = 233,    // eCash
    Miota = 234,  // IOTA
    Hnt = 235,    // Helium
    Rune = 236,   // THORChain
    Bsv = 237,    // Bitcoin SV
    Leo = 238,    // UNUS SED LEO
    Neo = 239,    // Neo
    One = 240,    // Harmony
    Qnt = 241,    // Quant
    Ust = 242,    // TerraUSD
    Mkr = 243,    // Maker
    Enj = 244,    // Enjin Coin
    Chz = 245,    // Chiliz
    Ar = 246,     // Arweave
    Stx = 247,    // Stacks
    Btt = 248,    // BitTorrent
    Hot = 249,    // Holo
    Sand = 250,   // The Sandbox
    Omg = 251,    // OMG Network
    Celo = 252,   // Celo
    Zec = 253,    // Zcash
    Comp = 254,   // Compound
    Tfuel = 255,  // Theta Fuel
    Kda = 256,    // Kadena
    Lrc = 257,    // Loopring
    Qtum = 258,   // Qtum
    Crv = 259,    // Curve DAO Token
    Ht = 260,     // Huobi Token
    Nexo = 261,   // Nexo
    Sushi = 262,  // SushiSwap
    Kcs = 263,    // KuCoin Token
    Bat = 264,    // Basic Attention Token
    Okb = 265,    // OKB
    Dcr = 266,    // Decred
    Icx = 267,    // ICON
    Rvn = 268,    // Ravencoin
    Scrt = 269,   // Secret
    Rev = 270,    // Revain
    Audio = 271,  // Audius
    Zil = 272,    // Zilliqa
    Tusd = 273,   // TrueUSD
    Yfi = 274,    // yearn.finance
    Mina = 275,   // Mina
    Perp = 276,   // Perpetual Protocol
    Xdc = 277,    // XDC Network
    Tel = 278,    // Telcoin
    Snx = 279,    // Synthetix
    Btg = 280,    // Bitcoin Gold
    Afn = 281,    // Afghanistan Afghani
    All = 282,    // Albania Lek
    Amd = 283,    // Armenia Dram
    Ang = 284,    // Netherlands Antilles Guilder
    Aoa = 285,    // Angola Kwanza
    Ars = 286,    // Argentina Peso
    Awg = 287,    // Aruba Guilder
    Azn = 288,    // Azerbaijan Manat
    Bam = 289,    // Bosnia and Herzegovina Convertible Mark
    Bbd = 290,    // Barbados Dollar
    Bdt = 291,    // Bangladesh Taka
    Bgn = 292,    // Bulgaria Lev
    Bhd = 293,    // Bahrain Dinar
    Bif = 294,    // Burundi Franc
    Bmd = 295,    // Bermuda Dollar
    Bnd = 296,    // Brunei Darussalam Dollar
    Bob = 297,    // Bolivia Bolíviano
    Brl = 298,    // Brazil Real
    Bsd = 299,    // Bahamas Dollar
    Btn = 300,    // Bhutan Ngultrum
    Bwp = 301,    // Botswana Pula
    Byn = 302,    // Belarus Ruble
    Bzd = 303,    // Belize Dollar
    Cdf = 304,    // Congo/Kinshasa Franc
    Clp = 305,    // Chile Peso
    Cop = 306,    // Colombia Peso
    Crc = 307,    // Costa Rica Colon
    Cuc = 308,    // Cuba Convertible Peso
    Cup = 309,    // Cuba Peso
    Cve = 310,    // Cape Verde Escudo
    Czk = 311,    // Czech Republic Koruna
    Djf = 312,    // Djibouti Franc
    Dkk = 313,    // Denmark Krone
    Dop = 314,    // Dominican Republic Peso
    Dzd = 315,    // Algeria Dinar
    Egp = 316,    // Egypt Pound
    Ern = 317,    // Eritrea Nakfa
    Etb = 318,    // Ethiopia Birr
    Fjd = 319,    // Fiji Dollar
    Fkp = 320,    // Falkland Islands (Malvinas) Pound
    Gel = 321,    // Georgia Lari
    Ggp = 322,    // Guernsey Pound
    Ghs = 323,    // Ghana Cedi
    Gip = 324,    // Gibraltar Pound
    Gmd = 325,    // Gambia Dalasi
    Gnf = 326,    // Guinea Franc
    Gtq = 327,    // Guatemala Quetzal
    Gyd = 328,    // Guyana Dollar
    Hnl = 329,    // Honduras Lempira
    Hrk = 330,    // Croatia Kuna
    Htg = 331,    // Haiti Gourde
    Idr = 332,    // Indonesia Rupiah
    Ils = 333,    // Israel Shekel
    Imp = 334,    // Isle of Man Pound
    Iqd = 335,    // Iraq Dinar
    Irr = 336,    // Iran Rial
    Isk = 337,    // Iceland Krona
    Jep = 338,    // Jersey Pound
    Jmd = 339,    // Jamaica Dollar
    Jod = 340,    // Jordan Dinar
    Kes = 341,    // Kenya Shilling
    Kgs = 342,    // Kyrgyzstan Som
    Khr = 343,    // Cambodia Riel
    Kmf = 344,    // Comorian Franc
    Kpw = 345,    // Korea (North) Won
    Krw = 346,    // Korea (South) Won
    Kwd = 347,    // Kuwait Dinar
    Kyd = 348,    // Cayman Islands Dollar
    Kzt = 349,    // Kazakhstan Tenge
    Lak = 350,    // Laos Kip
    Lbp = 351,    // Lebanon Pound
    Lkr = 352,    // Sri Lanka Rupee
    Lrd = 353,    // Liberia Dollar
    Lsl = 354,    // Lesotho Loti
    Lyd = 355,    // Libya Dinar
    Mad = 356,    // Morocco Dirham
    Mdl = 357,    // Moldova Leu
    Mga = 358,    // Madagascar Ariary
    Mkd = 359,    // Macedonia Denar
    Mmk = 360,    // Myanmar (Burma) Kyat
    Mnt = 361,    // Mongolia Tughrik
    Mop = 362,    // Macau Pataca
    Mru = 363,    // Mauritania Ouguiya
    Mur = 364,    // Mauritius Rupee
    Mvr = 365,    // Maldives (Maldive Islands) Rufiyaa
    Mwk = 366,    // Malawi Kwacha
    Mzn = 367,    // Mozambique Metical
    Nad = 368,    // Namibia Dollar
    Ngn = 369,    // Nigeria Naira
    Nio = 370,    // Nicaragua Cordoba
    Nok = 371,    // Norway Krone
    Npr = 372,    // Nepal Rupee
    Omr = 373,    // Oman Rial
    Pab = 374,    // Panama Balboa
    Pen = 375,    // Peru Sol
    Pgk = 376,    // Papua New Guinea Kina
    Pkr = 377,    // Pakistan Rupee
    Pln = 378,    // Poland Zloty
    Pyg = 379,    // Paraguay Guarani
    Qar = 380,    // Qatar Riyal
    Ron = 381,    // Romania Leu
    Rsd = 382,    // Serbia Dinar
    Rub = 383,    // Russia Ruble
    Rwf = 384,    // Rwanda Franc
    Sar = 385,    // Saudi Arabia Riyal
    Sbd = 386,    // Solomon Islands Dollar
    Scr = 387,    // Seychelles Rupee
    Sdg = 388,    // Sudan Pound
    Shp = 389,    // Saint Helena Pound
    Sll = 390,    // Sierra Leone Leone
    Sos = 391,    // Somalia Shilling
    Spl = 392,    // Seborga Luigino
    Srd = 393,    // Suriname Dollar
    Stn = 394,    // São Tomé and Príncipe Dobra
    Svc = 395,    // El Salvador Colon
    Syp = 396,    // Syria Pound
    Szl = 397,    // eSwatini Lilangeni
    Tjs = 398,    // Tajikistan Somoni
    Tmt = 399,    // Turkmenistan Manat
    Tnd = 400,    // Tunisia Dinar
    Top = 401,    // Tonga Pa'anga
    Try = 402,    // Turkey Lira
    Ttd = 403,    // Trinidad and Tobago Dollar
    Tvd = 404,    // Tuvalu Dollar
    Twd = 405,    // Taiwan New Dollar
    Tzs = 406,    // Tanzania Shilling
    Uah = 407,    // Ukraine Hryvnia
    Ugx = 408,    // Uganda Shilling
    Uyu = 409,    // Uruguay Peso
    Uzs = 410,    // Uzbekistan Som
    Vef = 411,    // Venezuela Bolívar
    Vnd = 412,    // Viet Nam Dong
    Vuv = 413,    // Vanuatu Vatu
    Wst = 414,    // Samoa Tala
    Xaf = 415,    // Communauté Financière Africaine (BEAC) CFA Franc BEAC
    Xcd = 416,    // East Caribbean Dollar
    Xdr = 417,    // International Monetary Fund (IMF) Special Drawing Rights
    Xof = 418,    // Communauté Financière Africaine (BCEAO) Franc
    Xpf = 419,    // Comptoirs Français du Pacifique (CFP) Franc
    Yer = 420,    // Yemen Rial
    Zmw = 421,    // Zambia Kwacha
    Zwd = 422,    // Zimbabwe Dollar
    Custom = 423,
};
}  // namespace opentxs::v1::identity::wot::claim
