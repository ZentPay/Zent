// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2014-2018, The Monero Project
// Copyright (c) 2018-2020, The TurtleCoin Developers
// Copyright (c) 2019-2023, The Zent Cash Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include <boost/uuid/uuid.hpp>
#include <crypto/hash.h>
#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <limits>
#include <string>

namespace CryptoNote
{
    namespace parameters
    {
        const uint64_t DIFFICULTY_TARGET = 60; // seconds

        const uint32_t CRYPTONOTE_MAX_BLOCK_NUMBER = 500000000;

        const size_t CRYPTONOTE_MAX_BLOCK_BLOB_SIZE = 500000000;

        const size_t CRYPTONOTE_MAX_TX_SIZE = 1000000000;

        const uint64_t CRYPTONOTE_PUBLIC_ADDRESS_BASE58_PREFIX = 4419;

        const uint32_t CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW = 10;

        const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT = 60 * 60 * 2;

        const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V3 = 3 * DIFFICULTY_TARGET;

        const uint64_t CRYPTONOTE_BLOCK_FUTURE_TIME_LIMIT_V4 = 6 * DIFFICULTY_TARGET;

        const size_t BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW = 60;

        const size_t BLOCKCHAIN_TIMESTAMP_CHECK_WINDOW_V3 = 11;

        // MONEY_SUPPLY - total number coins to be generated
        const uint64_t MONEY_SUPPLY = UINT64_C(750000000000);

        const uint32_t ZAWY_DIFFICULTY_BLOCK_INDEX = 0;

        const size_t ZAWY_DIFFICULTY_V2 = 2;

        const uint8_t ZAWY_DIFFICULTY_DIFFICULTY_BLOCK_VERSION = 3;

        const uint64_t LWMA_2_DIFFICULTY_BLOCK_INDEX = 1;

        const uint64_t LWMA_2_DIFFICULTY_BLOCK_INDEX_V2 = 2;

        const uint64_t LWMA_2_DIFFICULTY_BLOCK_INDEX_V3 = 3;

        const unsigned EMISSION_SPEED_FACTOR = 21;

        static_assert(EMISSION_SPEED_FACTOR <= 8 * sizeof(uint64_t), "Bad EMISSION_SPEED_FACTOR");

        const char GENESIS_COINBASE_TX_HEX[] = "010a01ff000180d88ee16f02f17fd6187a65a5844ec8fc5aa30e088aebb13951cbb6710c1aafd662353bbd4221013f233d1b74de62b435533d6e28c374f0f8568c5c25fd1130eceeb639133dbd19";

        static_assert(
            sizeof(GENESIS_COINBASE_TX_HEX) / sizeof(*GENESIS_COINBASE_TX_HEX) != 1,
            "GENESIS_COINBASE_TX_HEX must not be empty.");

        /* This is the unix timestamp of the first "mined" block (technically block 2, not the genesis block)
           You can get this value by doing "print_block 2" in TurtleCoind. It is used to know what timestamp
           to import from when the block height cannot be found in the node or the node is offline. */
        const uint64_t GENESIS_BLOCK_TIMESTAMP = 1550759396;

        const size_t CRYPTONOTE_REWARD_BLOCKS_WINDOW = 100;

        const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE = 100000; 

        const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V2 = 20000;

        const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V1 = 10000;

        const size_t CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE;

        const size_t CRYPTONOTE_COINBASE_BLOB_RESERVED_SIZE = 600;

        const size_t CRYPTONOTE_DISPLAY_DECIMAL_POINT = 2;

        const uint64_t MINIMUM_FEE = UINT64_C(10);

        /* Fee per byte is rounded up in chunks. This helps makes estimates
         * more accurate. It's suggested to make this a power of two, to relate
         * to the underlying storage cost / page sizes for storing a transaction. */
        const uint64_t FEE_PER_BYTE_CHUNK_SIZE = 256;

        const double MINIMUM_FEE_PER_BYTE_V1 = 25.6 / FEE_PER_BYTE_CHUNK_SIZE;
        
        /* Height for our first fee to byte change to take effect. */
        const uint64_t MINIMUM_FEE_PER_BYTE_V1_HEIGHT = 2200000;

        /* This section defines our minimum and maximum mixin counts required for transactions */
        const uint64_t MINIMUM_MIXIN_V1 = 3;

        const uint64_t MAXIMUM_MIXIN_V1 = 3;

        const uint64_t MINIMUM_MIXIN_V2 = 0;

        const uint64_t MAXIMUM_MIXIN_V2 = 3;

        const uint64_t MINIMUM_MIXIN_V3 = 1;

        const uint64_t MAXIMUM_MIXIN_V3 = 3;

        /* The heights to activate the mixin limits at */
        const uint32_t MIXIN_LIMITS_V1_HEIGHT = 0;

        const uint32_t MIXIN_LIMITS_V2_HEIGHT = 11000;

        const uint32_t MIXIN_LIMITS_V3_HEIGHT = 150000;

        /* The mixin to use by default with zedwallet and turtle-service */
        /* DEFAULT_MIXIN_V0 is the mixin used before MIXIN_LIMITS_V1_HEIGHT is started */
        const uint64_t DEFAULT_MIXIN_V0 = 0;

        const uint64_t DEFAULT_MIXIN_V1 = MAXIMUM_MIXIN_V1;

        const uint64_t DEFAULT_MIXIN_V2 = MAXIMUM_MIXIN_V2;

        const uint64_t DEFAULT_MIXIN_V3 = MAXIMUM_MIXIN_V3;

        const uint64_t DEFAULT_DUST_THRESHOLD = UINT64_C(10);

        const uint64_t DEFAULT_DUST_THRESHOLD_V2 = UINT64_C(0);

        const uint32_t DUST_THRESHOLD_V2_HEIGHT = MIXIN_LIMITS_V2_HEIGHT;

        const uint32_t FUSION_DUST_THRESHOLD_HEIGHT_V2 = 800000;

        const uint64_t EXPECTED_NUMBER_OF_BLOCKS_PER_DAY = 24 * 60 * 60 / DIFFICULTY_TARGET;

        const size_t DIFFICULTY_WINDOW = 17;

        const size_t DIFFICULTY_WINDOW_V1 = 2880;

        const size_t DIFFICULTY_WINDOW_V2 = 2880;

        const uint64_t DIFFICULTY_WINDOW_V3 = 60;

        const uint64_t DIFFICULTY_BLOCKS_COUNT_V3 = DIFFICULTY_WINDOW_V3 + 1;

        const size_t DIFFICULTY_CUT = 0; // timestamps to cut after sorting
        const size_t DIFFICULTY_CUT_V1 = 60;

        const size_t DIFFICULTY_CUT_V2 = 60;

        const size_t DIFFICULTY_LAG = 0; // !!!
        const size_t DIFFICULTY_LAG_V1 = 15;

        const size_t DIFFICULTY_LAG_V2 = 15;

        static_assert(2 * DIFFICULTY_CUT <= DIFFICULTY_WINDOW - 2, "Bad DIFFICULTY_WINDOW or DIFFICULTY_CUT");

        const size_t MAX_BLOCK_SIZE_INITIAL = 100000;

        const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_NUMERATOR = 100 * 1024;

        const uint64_t MAX_BLOCK_SIZE_GROWTH_SPEED_DENOMINATOR = 365 * 24 * 60 * 60 / DIFFICULTY_TARGET;

        const uint64_t MAX_EXTRA_SIZE = 140000;

        const uint64_t MAX_EXTRA_SIZE_V2 = 1024;

        const uint64_t MAX_EXTRA_SIZE_V2_HEIGHT = 1300000;

        /* 1875 million ZTC -> Max supply / mixin+1 outputs */
        /* This is enforced on the daemon side. An output > 1875 million causes
         * an invalid block. */
        const uint64_t MAX_OUTPUT_SIZE_NODE   = 1'875'000'000'00;

        /* 400 million ZTC */
        /* This is enforced on the client side. An output > 400 million will not
         * be created in a transaction */
        const uint64_t MAX_OUTPUT_SIZE_CLIENT = 400'000'000'00;

        const uint64_t MAX_OUTPUT_SIZE_HEIGHT = 2000000;

        /* For new projects forked from this code base, the values immediately below
           should be changed to 0 to prevent issues with transaction processing
           and other possible unexpected behavior */
        const uint64_t TRANSACTION_SIGNATURE_COUNT_VALIDATION_HEIGHT = 1400000;

        const uint64_t BLOCK_BLOB_SHUFFLE_CHECK_HEIGHT = 1600000;

        const uint64_t TRANSACTION_INPUT_BLOCKTIME_VALIDATION_HEIGHT = 1600000;

        /* Coinbase transactions must include the recipient address + tx priv
         * key in tx_extra to verify the outputs go to that address after this
         * height. */
        const uint64_t COINBASE_TRANSACTION_OUTPUT_CLAIMING_HEIGHT = 28000000;

        /* This describes how many blocks of "wiggle" room transactions have regarding
           when the outputs can be spent based on a reasonable belief that the outputs
           would unlock in the current block period */
        const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS = 1;

        const uint64_t CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_SECONDS =
            DIFFICULTY_TARGET * CRYPTONOTE_LOCKED_TX_ALLOWED_DELTA_BLOCKS;

        const uint64_t CRYPTONOTE_MEMPOOL_TX_LIVETIME = 60 * 60 * 24; // seconds, one day
        const uint64_t CRYPTONOTE_MEMPOOL_TX_FROM_ALT_BLOCK_LIVETIME = 60 * 60 * 24 * 7; // seconds, one week
        const uint64_t CRYPTONOTE_NUMBER_OF_PERIODS_TO_FORGET_TX_DELETED_FROM_POOL = 7; 

        const size_t FUSION_TX_MAX_SIZE = CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_CURRENT * 30 / 100;

        const size_t FUSION_TX_MIN_INPUT_COUNT = 12;

        const size_t FUSION_TX_MIN_IN_OUT_COUNT_RATIO = 4;

        /* This sets the maximum number of fusion transactions that can be present in the pool
           at any given time. Incoming fusion transactions that attempt to exceed this limit
           will be rejected from the pool and will not be added. This mechanism is in place
           to help curtail fusion transaction spam. */
        const size_t FUSION_TX_MAX_POOL_COUNT = 70;

        const size_t NORMAL_TX_MAX_OUTPUT_COUNT_V1 = 90;

        const size_t NORMAL_TX_MAX_OUTPUT_COUNT_V1_HEIGHT = 2200000;

        const uint32_t UPGRADE_HEIGHT_V2 = 1;

        const uint32_t UPGRADE_HEIGHT_V3 = 2;

        const uint32_t UPGRADE_HEIGHT_V4 = 3; // Upgrade height for CN-Lite Variant 1 switch.

        const uint32_t UPGRADE_HEIGHT_V5 = 450000; // Upgrade height for CN-Turtle Variant 2 switch.

        const uint32_t UPGRADE_HEIGHT_V6 = 1800000; // Upgrade height for Chukwa switch.

        const uint32_t UPGRADE_HEIGHT_CURRENT = UPGRADE_HEIGHT_V6;

        const unsigned UPGRADE_VOTING_THRESHOLD = 90; // percent
        const uint32_t UPGRADE_VOTING_WINDOW = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY; // blocks
        const uint32_t UPGRADE_WINDOW = EXPECTED_NUMBER_OF_BLOCKS_PER_DAY; // blocks
        static_assert(0 < UPGRADE_VOTING_THRESHOLD && UPGRADE_VOTING_THRESHOLD <= 100, "Bad UPGRADE_VOTING_THRESHOLD");
        static_assert(UPGRADE_VOTING_WINDOW > 1, "Bad UPGRADE_VOTING_WINDOW");

        /* Block heights we are going to have hard forks at */
        const uint64_t FORK_HEIGHTS[] = {
            0,        //  0 -  Zawy Difficulty Block Index, Mixin Limits V1 Height
            1,        //  1 -  Upgrade Height V2 CN-Classic, Lwma 2 Difficulty Block Index
            2,        //  2 -  Upgrade Height V3 CN-Classic, Lwma 2 Difficulty Block Index V2, Zawy Difficulty V2
            3,        //  3 -  Upgrade Height V4 CN-Lite, Lwma 2 Difficulty Block Index V3
            11000,    //  4 -  Mixin Limits Height V2
            150000,   //  5 -  Mixin Limits Height V3
            450000,   //  6 -  Upgrade Height V5 CN-Turtle
            800000,   //  7 -  Fusion Dust Threshold V2 Height
            1000000,  //  8 -  Free Space
            1300000,  //  9 -  Max Extra Size V2 Height
            1400000,  //  10 - Transation Signature Count Validation Height
            1600000,  //  11 - Block Blob Shuffle Check Height, Transaction Input Blocktime Validation Height
            1800000, // 12 - Upgrade Height V6 Argon2Id Chukwa
            2000000, // 13 - Max Output Size
            2200000, // 14 - Minimum Fee Per Byte, Normal TX Max Output Count
            2400000, // 15 -  Free Space 
            2600000, // 16 -  Free Space
            28000000, // 17 -  Coinbase Transaction Output Claiming
            30000000, // 18 -  Free Space
        };

        /* MAKE SURE TO UPDATE THIS VALUE WITH EVERY MAJOR RELEASE BEFORE A FORK */
        const uint64_t SOFTWARE_SUPPORTED_FORK_INDEX = 17;

        const uint64_t FORK_HEIGHTS_SIZE = sizeof(FORK_HEIGHTS) / sizeof(*FORK_HEIGHTS);

        /* The index in the FORK_HEIGHTS array that this version of the software will
           support. For example, if CURRENT_FORK_INDEX is 6, this version of the
           software will support the fork at 400,000 blocks.

           This will default to zero if the FORK_HEIGHTS array is empty, so you don't
           need to change it manually. */
        const uint8_t CURRENT_FORK_INDEX = FORK_HEIGHTS_SIZE == 0 ? 0 : SOFTWARE_SUPPORTED_FORK_INDEX;

        /* Make sure CURRENT_FORK_INDEX is a valid index, unless FORK_HEIGHTS is empty */
        static_assert(
            FORK_HEIGHTS_SIZE == 0 || CURRENT_FORK_INDEX < FORK_HEIGHTS_SIZE,
            "CURRENT_FORK_INDEX out of range of FORK_HEIGHTS!");

        const char CRYPTONOTE_BLOCKS_FILENAME[] = "blocks.bin";

        const char CRYPTONOTE_BLOCKINDEXES_FILENAME[] = "blockindexes.bin";

        const char CRYPTONOTE_POOLDATA_FILENAME[] = "poolstate.bin";

        const char P2P_NET_DATA_FILENAME[] = "p2pstate.bin";

        const char MINER_CONFIG_FILE_NAME[] = "miner_conf.json";
    } // namespace parameters

    const char CRYPTONOTE_NAME[] = "Zent";

    const uint8_t TRANSACTION_VERSION_1 = 1;

    const uint8_t TRANSACTION_VERSION_2 = 2;

    const uint8_t CURRENT_TRANSACTION_VERSION = TRANSACTION_VERSION_1;

    const uint8_t BLOCK_MAJOR_VERSION_1 = 1; /* From zero */
    const uint8_t BLOCK_MAJOR_VERSION_2 = 2; /* UPGRADE_HEIGHT_V2 */
    const uint8_t BLOCK_MAJOR_VERSION_3 = 3; /* UPGRADE_HEIGHT_V3 */
    const uint8_t BLOCK_MAJOR_VERSION_4 = 4; /* UPGRADE_HEIGHT_V4 */
    const uint8_t BLOCK_MAJOR_VERSION_5 = 5; /* UPGRADE_HEIGHT_V5 */
    const uint8_t BLOCK_MAJOR_VERSION_6 = 6; /* UPGRADE_HEIGHT_V6 */

    const uint8_t BLOCK_MINOR_VERSION_0 = 0;

    const uint8_t BLOCK_MINOR_VERSION_1 = 1;

    const std::unordered_map<uint8_t, std::function<void(const void *data, size_t length, Crypto::Hash &hash)>>
        HASHING_ALGORITHMS_BY_BLOCK_VERSION = {
            {BLOCK_MAJOR_VERSION_1, Crypto::cn_slow_hash_v0}, /* From zero */
            {BLOCK_MAJOR_VERSION_2, Crypto::cn_slow_hash_v0}, /* UPGRADE_HEIGHT_V2 */
            {BLOCK_MAJOR_VERSION_3, Crypto::cn_slow_hash_v0}, /* UPGRADE_HEIGHT_V3 */
            {BLOCK_MAJOR_VERSION_4, Crypto::cn_lite_slow_hash_v1}, /* UPGRADE_HEIGHT_V4 */
            {BLOCK_MAJOR_VERSION_5, Crypto::cn_turtle_lite_slow_hash_v2}, /* UPGRADE_HEIGHT_V5 */
            {BLOCK_MAJOR_VERSION_6, Crypto::chukwa_slow_hash_v1} /* UPGRADE_HEIGHT_V6 */
    };

    const size_t BLOCKS_IDS_SYNCHRONIZING_DEFAULT_COUNT = 10000; // by default, blocks ids count in synchronizing
    const uint64_t BLOCKS_SYNCHRONIZING_DEFAULT_COUNT = 100; // by default, blocks count in blocks downloading
    const size_t COMMAND_RPC_GET_BLOCKS_FAST_MAX_COUNT = 1000;

    const int P2P_DEFAULT_PORT = 21688;

    const int RPC_DEFAULT_PORT = 21698;

    const int SERVICE_DEFAULT_PORT = 21699;

    const size_t P2P_LOCAL_WHITE_PEERLIST_LIMIT = 1000;

    const size_t P2P_LOCAL_GRAY_PEERLIST_LIMIT = 5000;

    // P2P Network Configuration Section - This defines our current P2P network version
    // and the minimum version for communication between nodes
    const uint8_t P2P_CURRENT_VERSION = 14;

    const uint8_t P2P_MINIMUM_VERSION = 13;

    // This defines the minimum P2P version required for lite blocks propogation
    const uint8_t P2P_LITE_BLOCKS_PROPOGATION_VERSION = 13;

    // This defines the number of versions ahead we must see peers before we start displaying
    // warning messages that we need to upgrade our software.
    const uint8_t P2P_UPGRADE_WINDOW = 1;

    const size_t P2P_CONNECTION_MAX_WRITE_BUFFER_SIZE = 32 * 1024 * 1024; // 32 MB
    const uint32_t P2P_DEFAULT_CONNECTIONS_COUNT = 8;

    const size_t P2P_DEFAULT_WHITELIST_CONNECTIONS_PERCENT = 70;

    const uint32_t P2P_DEFAULT_HANDSHAKE_INTERVAL = 60; // seconds
    const uint32_t P2P_DEFAULT_PACKET_MAX_SIZE = 50000000; // 50000000 bytes maximum packet size
    const uint32_t P2P_DEFAULT_PEERS_IN_HANDSHAKE = 250;

    const uint32_t P2P_DEFAULT_CONNECTION_TIMEOUT = 5000; // 5 seconds
    const uint32_t P2P_DEFAULT_PING_CONNECTION_TIMEOUT = 2000; // 2 seconds
    const uint64_t P2P_DEFAULT_INVOKE_TIMEOUT = 60 * 2 * 1000; // 2 minutes
    const size_t P2P_DEFAULT_HANDSHAKE_INVOKE_TIMEOUT = 5000; // 5 seconds
    const char P2P_STAT_TRUSTED_PUB_KEY[] = "";

    const uint64_t ROCKSDB_WRITE_BUFFER_MB = 512; // 512 MB
    const uint64_t ROCKSDB_READ_BUFFER_MB = 256; // 256 MB
    const uint64_t ROCKSDB_MAX_OPEN_FILES = 125; // 125 files
    const uint64_t ROCKSDB_BACKGROUND_THREADS = 4; // 4 DB threads

    const uint64_t LEVELDB_WRITE_BUFFER_MB = 64; // 64 MB
    const uint64_t LEVELDB_READ_BUFFER_MB = 64; // 64 MB
    const uint64_t LEVELDB_MAX_OPEN_FILES = 128; // 128 files
    const uint64_t LEVELDB_MAX_FILE_SIZE_MB = 1024; // 1024MB = 1GB

    const char LATEST_VERSION_URL[] = "https://github.com/ZentCashFoundation/Zent/releases";

    const std::string LICENSE_URL = "https://github.com/ZentCashFoundation/Zent/blob/master/LICENSE";

    const static boost::uuids::uuid CRYPTONOTE_NETWORK = {
        {0xc6, 0x1c, 0x4d, 0x6c, 0xcc, 0x12, 0x42, 0x9c, 0x33, 0xd6, 0x35, 0xa5, 0xee, 0xc1, 0x43, 0xd8}};

    const char *const SEED_NODES[] = {
        "138.124.183.11:21688", /// USA
 	"185.74.222.212:21688", // Hong Kong
        "seedpro1.zent.cash:21688", // seedpro1.zent.cash
        "seedpro2.zent.cash:21688", // seedpro2.zent.cash
        "seedpro3.zent.cash:21688", // seedpro3.zent.cash
    };
} // namespace CryptoNote
