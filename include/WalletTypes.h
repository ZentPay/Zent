// Copyright (c) 2018-2019, The TurtleCoin Developers
//
// Please see the included LICENSE file for more information.

#pragma once

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

#include <CryptoNote.h>
#include <errors/Errors.h>
#include <JsonHelper.h>
#include <optional>
#include <string>
#include <unordered_map>

namespace WalletTypes
{
    struct KeyOutput
    {
        Crypto::PublicKey key;
        uint64_t amount;
        /* Daemon doesn't supply this, blockchain cache api does. */
        std::optional<uint64_t> globalOutputIndex;
    };

    /* A coinbase transaction (i.e., a miner reward, there is one of these in
       every block). Coinbase transactions have no inputs. We call this a raw
       transaction, because it is simply key images and amounts */
    struct RawCoinbaseTransaction
    {
        /* The outputs of the transaction, amounts and keys */
        std::vector<KeyOutput> keyOutputs;

        /* The hash of the transaction */
        Crypto::Hash hash;

        /* The public key of this transaction, taken from the tx extra */
        Crypto::PublicKey transactionPublicKey;

        /* When this transaction's inputs become spendable. Some genius thought
           it was a good idea to use this field as both a block height, and a
           unix timestamp. If the value is greater than
           CRYPTONOTE_MAX_BLOCK_NUMBER (In cryptonoteconfig) it is treated
           as a unix timestamp, else it is treated as a block height. */
        uint64_t unlockTime;

        size_t memoryUsage() const
        {
            return keyOutputs.size() * sizeof(KeyOutput) + sizeof(keyOutputs) + sizeof(hash)
                   + sizeof(transactionPublicKey) + sizeof(unlockTime);
        }
    };

    /* A raw transaction, simply key images and amounts */
    struct RawTransaction : RawCoinbaseTransaction
    {
        /* The transaction payment ID - may be an empty string */
        std::string paymentID;

        /* The inputs used for a transaction, can be used to track outgoing
           transactions */
        std::vector<CryptoNote::KeyInput> keyInputs;

        size_t memoryUsage() const
        {
            return paymentID.size() * sizeof(char) + sizeof(paymentID) + keyInputs.size() * sizeof(CryptoNote::KeyInput)
                   + sizeof(keyInputs) + RawCoinbaseTransaction::memoryUsage();
        }
    };

    /* A 'block' with the very basics needed to sync the transactions */
    struct WalletBlockInfo
    {
        /* The coinbase transaction. Optional, since we can skip fetching
           coinbase transactions from daemon. */
        std::optional<RawCoinbaseTransaction> coinbaseTransaction;

        /* The transactions in the block */
        std::vector<RawTransaction> transactions;

        /* The block height (duh!) */
        uint64_t blockHeight;

        /* The hash of the block */
        Crypto::Hash blockHash;

        /* The timestamp of the block */
        uint64_t blockTimestamp;

        size_t memoryUsage() const
        {
            const size_t txUsage = std::accumulate(
                transactions.begin(), transactions.end(), sizeof(transactions), [](const auto acc, const auto item) {
                    return acc + item.memoryUsage();
                });
            return coinbaseTransaction ? coinbaseTransaction->memoryUsage()
                                       : sizeof(coinbaseTransaction) + txUsage + sizeof(blockHeight) + sizeof(blockHash)
                                             + sizeof(blockTimestamp);
        }
    };

    struct TransactionInput
    {
        /* The key image of this amount */
        Crypto::KeyImage keyImage;

        /* The value of this key image */
        uint64_t amount;

        /* The block height this key images transaction was included in
           (Need this for removing key images that were received on a forked
           chain) */
        uint64_t blockHeight;

        /* The transaction public key that was included in the tx_extra of the
           transaction */
        Crypto::PublicKey transactionPublicKey;

        /* The index of this input in the transaction */
        uint64_t transactionIndex;

        /* The index of this output in the 'DB' */
        std::optional<uint64_t> globalOutputIndex;

        /* The transaction key we took from the key outputs */
        Crypto::PublicKey key;

        /* If spent, what height did we spend it at. Used to remove spent
           transaction inputs once they are sure to not be removed from a
           forked chain. */
        uint64_t spendHeight;

        /* When does this input unlock for spending. Default is instantly
           unlocked, or blockHeight + CRYPTONOTE_MINED_MONEY_UNLOCK_WINDOW
           for a coinbase/miner transaction. Users can specify a custom
           unlock height however. */
        uint64_t unlockTime;

        /* The transaction hash of the transaction that contains this input */
        Crypto::Hash parentTransactionHash;

        /* The private ephemeral generated along with the key image */
        std::optional<Crypto::SecretKey> privateEphemeral;

        bool operator==(const TransactionInput &other)
        {
            return keyImage == other.keyImage;
        }

        /* Converts the class to a json object */
        void toJSON(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
        {
            writer.StartObject();
            {
                writer.Key("keyImage");
                keyImage.toJSON(writer);

                writer.Key("amount");
                writer.Uint64(amount);

                writer.Key("blockHeight");
                writer.Uint64(blockHeight);

                writer.Key("transactionPublicKey");
                transactionPublicKey.toJSON(writer);

                writer.Key("transactionIndex");
                writer.Uint64(transactionIndex);

                writer.Key("globalOutputIndex");
                writer.Uint64(globalOutputIndex.value_or(0));

                writer.Key("key");
                key.toJSON(writer);

                writer.Key("spendHeight");
                writer.Uint64(spendHeight);

                writer.Key("unlockTime");
                writer.Uint64(unlockTime);

                writer.Key("parentTransactionHash");
                parentTransactionHash.toJSON(writer);

                if (privateEphemeral)
                {
                    writer.Key("privateEphemeral");
                    privateEphemeral->toJSON(writer);
                }
            }
            writer.EndObject();
        }

        /* Initializes the class from a json string */
        void fromJSON(const JSONValue &j)
        {
            keyImage.fromString(getStringFromJSON(j, "keyImage"));
            amount = getUint64FromJSON(j, "amount");
            blockHeight = getUint64FromJSON(j, "blockHeight");
            transactionPublicKey.fromString(getStringFromJSON(j, "transactionPublicKey"));
            transactionIndex = getUint64FromJSON(j, "transactionIndex");
            globalOutputIndex = getUint64FromJSON(j, "globalOutputIndex");
            key.fromString(getStringFromJSON(j, "key"));
            spendHeight = getUint64FromJSON(j, "spendHeight");
            unlockTime = getUint64FromJSON(j, "unlockTime");
            parentTransactionHash.fromString(getStringFromJSON(j, "parentTransactionHash"));

            if (j.HasMember("privateEphemeral"))
            {
                Crypto::SecretKey tmp;
                tmp.fromString(getStringFromJSON(j, "privateEphemeral"));
                privateEphemeral = tmp;
            }
        }
    };

    /* Includes the owner of the input so we can sign the input with the
       correct keys */
    struct TxInputAndOwner
    {
        TxInputAndOwner(
            const TransactionInput input,
            const Crypto::PublicKey publicSpendKey,
            const Crypto::SecretKey privateSpendKey):
            input(input),
            publicSpendKey(publicSpendKey),
            privateSpendKey(privateSpendKey)
        {
        }

        TransactionInput input;

        Crypto::PublicKey publicSpendKey;

        Crypto::SecretKey privateSpendKey;
    };

    struct TransactionDestination
    {
        /* The public spend key of the receiver of the transaction output */
        Crypto::PublicKey receiverPublicSpendKey;
        /* The public view key of the receiver of the transaction output */
        Crypto::PublicKey receiverPublicViewKey;
        /* The amount of the transaction output */
        uint64_t amount;
    };

    struct GlobalIndexKey
    {
        uint64_t index;
        Crypto::PublicKey key;
    };

    struct ObscuredInput
    {
        /* The outputs, including our real output, and the fake mixin outputs */
        std::vector<GlobalIndexKey> outputs;

        /* The index of the real output in the outputs vector */
        uint64_t realOutput;

        /* The real transaction public key */
        Crypto::PublicKey realTransactionPublicKey;

        /* The index in the transaction outputs vector */
        uint64_t realOutputTransactionIndex;

        /* The amount being sent */
        uint64_t amount;

        /* The owners keys, so we can sign the input correctly */
        Crypto::PublicKey ownerPublicSpendKey;
        Crypto::SecretKey ownerPrivateSpendKey;

        /* The key image of the input */
        Crypto::KeyImage keyImage;

        /* The private ephemeral generated along with the key image */
        std::optional<Crypto::SecretKey> privateEphemeral;
    };

    class Transaction
    {
      public:
        //////////////////
        /* Constructors */
        //////////////////

        Transaction() {};

        Transaction(
            /* Mapping of public key to transaction amount, can be multiple
               if one transaction sends to multiple subwallets */
            const std::unordered_map<Crypto::PublicKey, int64_t> transfers,
            const Crypto::Hash hash,
            const uint64_t fee,
            const uint64_t timestamp,
            const uint64_t blockHeight,
            const std::string paymentID,
            const uint64_t unlockTime,
            const bool isCoinbaseTransaction):
            transfers(transfers),
            hash(hash),
            fee(fee),
            timestamp(timestamp),
            blockHeight(blockHeight),
            paymentID(paymentID),
            unlockTime(unlockTime),
            isCoinbaseTransaction(isCoinbaseTransaction)
        {
        }

        Transaction(const Transaction &) = default;
        Transaction(Transaction &&) = default;
        Transaction &operator=(const Transaction &) = default;
        Transaction &operator=(Transaction &&) = default;


        /////////////////////////////
        /* Public member functions */
        /////////////////////////////

        int64_t totalAmount() const
        {
            int64_t sum = 0;
            for (const auto [pubKey, amount] : transfers)
            {
                sum += amount;
            }
            return sum;
        }

        /* It's worth noting that this isn't a conclusive check for if a
           transaction is a fusion transaction - there are some requirements
           it has to meet - but we don't need to check them, as the daemon
           will handle that for us - Any transactions that come to the
           wallet (assuming a non malicious daemon) that are zero and not
           a coinbase, is a fusion transaction */
        bool isFusionTransaction() const
        {
            return fee == 0 && !isCoinbaseTransaction;
        }

        /////////////////////////////
        /* Public member variables */
        /////////////////////////////

        /* A map of public keys to amounts, since one transaction can go to
           multiple addresses. These can be positive or negative, for example
           one address might have sent 10,000 TRTL (-10000) to two recipients
           (+5000), (+5000)

           All the public keys in this map, are ones that the wallet container
           owns, it won't store amounts belonging to random people */
        std::unordered_map<Crypto::PublicKey, int64_t> transfers;

        /* The hash of the transaction */
        Crypto::Hash hash;

        /* The fee the transaction was sent with (always positive) */
        uint64_t fee;

        /* The blockheight this transaction is in */
        uint64_t blockHeight;

        /* The timestamp of this transaction (taken from the block timestamp) */
        uint64_t timestamp;

        /* The paymentID of this transaction (will be an empty string if no pid) */
        std::string paymentID;

        /* When does the transaction unlock */
        uint64_t unlockTime;

        /* Was this transaction a miner reward / coinbase transaction */
        bool isCoinbaseTransaction;

        /* Converts the class to a json object */
        void toJSON(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
        {
            writer.StartObject();
            writer.Key("transfers");
            writer.StartArray();
            for (const auto &[publicKey, amount] : transfers)
            {
                writer.StartObject();
                writer.Key("publicKey");
                publicKey.toJSON(writer);
                writer.Key("amount");
                writer.Int64(amount);
                writer.EndObject();
            }
            writer.EndArray();
            writer.Key("hash");
            hash.toJSON(writer);
            writer.Key("fee");
            writer.Uint64(fee);
            writer.Key("blockHeight");
            writer.Uint64(blockHeight);
            writer.Key("timestamp");
            writer.Uint64(timestamp);
            writer.Key("paymentID");
            writer.String(paymentID);
            writer.Key("unlockTime");
            writer.Uint64(unlockTime);
            writer.Key("isCoinbaseTransaction");
            writer.Bool(isCoinbaseTransaction);
            writer.EndObject();
        }

        /* Initializes the class from a json string */
        void fromJSON(const JSONValue &j)
        {
            for (const auto &x : getArrayFromJSON(j, "transfers"))
            {
                Crypto::PublicKey publicKey;
                publicKey.fromString(getStringFromJSON(x, "publicKey"));
                transfers[publicKey] = getInt64FromJSON(x, "amount");
            }
            hash.fromString(getStringFromJSON(j, "hash"));
            fee = getUint64FromJSON(j, "fee");
            blockHeight = getUint64FromJSON(j, "blockHeight");
            timestamp = getUint64FromJSON(j, "timestamp");
            paymentID = getStringFromJSON(j, "paymentID");
            unlockTime = getUint64FromJSON(j, "unlockTime");
            isCoinbaseTransaction = getBoolFromJSON(j, "isCoinbaseTransaction");
        }
    };

    struct WalletStatus
    {
        /* The amount of blocks the wallet has synced */
        uint64_t walletBlockCount;
        /* The amount of blocks the daemon we are connected to has synced */
        uint64_t localDaemonBlockCount;
        /* The amount of blocks the daemons on the network have */
        uint64_t networkBlockCount;
        /* The amount of peers the node is connected to */
        uint32_t peerCount;
        /* The hashrate (based on the last block the daemon has synced) */
        uint64_t lastKnownHashrate;
    };

    /* A structure just used to display locked balance, due to change from
       sent transactions. We just need the amount and a unique identifier
       (hash+key), since we can't spend it, we don't need all the other stuff */
    struct UnconfirmedInput
    {
        /* The amount of the input */
        uint64_t amount;

        /* The transaction key we took from the key outputs */
        Crypto::PublicKey key;

        /* The transaction hash of the transaction that contains this input */
        Crypto::Hash parentTransactionHash;

        /* Converts the class to a json object */
        void toJSON(rapidjson::Writer<rapidjson::StringBuffer> &writer) const
        {
            writer.StartObject();
            writer.Key("amount");
            writer.Uint64(amount);
            writer.Key("key");
            key.toJSON(writer);
            writer.Key("parentTransactionHash");
            parentTransactionHash.toJSON(writer);
            writer.EndObject();
        }

        /* Initializes the class from a json string */
        void fromJSON(const JSONValue &j)
        {
            amount = getUint64FromJSON(j, "amount");
            key.fromString(getStringFromJSON(j, "key"));
            parentTransactionHash.fromString(getStringFromJSON(j, "parentTransactionHash"));
        }
    };

    struct TopBlock
    {
        Crypto::Hash hash;
        uint64_t height;
    };

    class FeeType
    {
        public:
            /* Fee will be specified as fee per byte, for example, 1 atomic TRTL per byte. */
            bool isFeePerByte = false;

            /* Fee for each byte, in atomic units. Allowed to be a double, since
             * we will truncate it to an int upon performing the chunking. */
            double feePerByte = 0;

            /* Fee will be specified as a fixed fee */
            bool isFixedFee = false;

            /* Total fee to use */
            uint64_t fixedFee = 0;

            /* Fee will not be specified, use the minimum possible */
            bool isMinimumFee = false;

            static FeeType MinimumFee()
            {
                FeeType fee;
                fee.isMinimumFee = true;
                return fee;
            }

            static FeeType FeePerByte(const double feePerByte)
            {
                FeeType fee;
                fee.isFeePerByte = true;
                fee.feePerByte = feePerByte;
                return fee;
            }

            static FeeType FixedFee(const uint64_t fixedFee)
            {
                FeeType fee;
                fee.isFixedFee = true;
                fee.fixedFee = fixedFee;
                return fee;
            }

        private:
            FeeType() = default;
    };

    struct TransactionResult
    {
        /* The error, if any */
        Error error;

        /* The raw transaction */
        CryptoNote::Transaction transaction;

        /* The transaction outputs, before converted into boost uglyness, used
           for determining key inputs from the tx that belong to us */
        std::vector<WalletTypes::KeyOutput> outputs;

        /* The random key pair we generated */
        CryptoNote::KeyPair txKeyPair;
    };

    struct PreparedTransactionInfo
    {
        uint64_t fee;
        std::string paymentID;
        std::vector<WalletTypes::TxInputAndOwner> inputs;
        std::string changeAddress;
        uint64_t changeRequired;
        TransactionResult tx;
        Crypto::Hash transactionHash;
    };

    inline void to_json(nlohmann::json &j, const TopBlock &t)
    {
        j = {{"hash", t.hash}, {"height", t.height}};
    }

    inline void from_json(const nlohmann::json &j, TopBlock &t)
    {
        t.hash = j.at("hash").get<Crypto::Hash>();
        t.height = j.at("height").get<uint64_t>();
    }

    inline void to_json(nlohmann::json &j, const WalletBlockInfo &w)
    {
        j = {{"transactions", w.transactions},
             {"blockHeight", w.blockHeight},
             {"blockHash", w.blockHash},
             {"blockTimestamp", w.blockTimestamp}};
        if (w.coinbaseTransaction)
        {
            j["coinbaseTX"] = *(w.coinbaseTransaction);
        }
    }

    inline void from_json(const nlohmann::json &j, WalletBlockInfo &w)
    {
        if (j.find("coinbaseTX") != j.end())
        {
            w.coinbaseTransaction = j.at("coinbaseTX").get<RawCoinbaseTransaction>();
        }
        w.transactions = j.at("transactions").get<std::vector<RawTransaction>>();
        w.blockHeight = j.at("blockHeight").get<uint64_t>();
        w.blockHash = j.at("blockHash").get<Crypto::Hash>();
        w.blockTimestamp = j.at("blockTimestamp").get<uint64_t>();
    }

    inline void to_json(nlohmann::json &j, const RawCoinbaseTransaction &r)
    {
        j = {{"outputs", r.keyOutputs},
             {"hash", r.hash},
             {"txPublicKey", r.transactionPublicKey},
             {"unlockTime", r.unlockTime}};
    }

    inline void from_json(const nlohmann::json &j, RawCoinbaseTransaction &r)
    {
        r.keyOutputs = j.at("outputs").get<std::vector<KeyOutput>>();
        r.hash = j.at("hash").get<Crypto::Hash>();
        r.transactionPublicKey = j.at("txPublicKey").get<Crypto::PublicKey>();

        /* We need to try to get the unlockTime from an integer in the json
           however, if that fails because we're talking to a blockchain
           cache API that encodes unlockTime as a string (due to json
           integer encoding limits), we need to attempt this as a string */
        try
        {
            r.unlockTime = j.at("unlockTime").get<uint64_t>();
        }
        catch (const nlohmann::json::exception &)
        {
            r.unlockTime = std::stoull(j.at("unlockTime").get<std::string>());
        }
    }

    inline void to_json(nlohmann::json &j, const RawTransaction &r)
    {
        j = {{"outputs", r.keyOutputs},
             {"hash", r.hash},
             {"txPublicKey", r.transactionPublicKey},
             {"unlockTime", r.unlockTime},
             {"paymentID", r.paymentID},
             {"inputs", r.keyInputs}};
    }

    inline void from_json(const nlohmann::json &j, RawTransaction &r)
    {
        r.keyOutputs = j.at("outputs").get<std::vector<KeyOutput>>();
        r.hash = j.at("hash").get<Crypto::Hash>();
        r.transactionPublicKey = j.at("txPublicKey").get<Crypto::PublicKey>();

        /* We need to try to get the unlockTime from an integer in the json
           however, if that fails because we're talking to a blockchain
           cache API that encodes unlockTime as a string (due to json
           integer encoding limits), we need to attempt this as a string */
        try
        {
            r.unlockTime = j.at("unlockTime").get<uint64_t>();
        }
        catch (const nlohmann::json::exception &)
        {
            r.unlockTime = std::stoull(j.at("unlockTime").get<std::string>());
        }
        r.paymentID = j.at("paymentID").get<std::string>();
        r.keyInputs = j.at("inputs").get<std::vector<CryptoNote::KeyInput>>();
    }

    inline void to_json(nlohmann::json &j, const KeyOutput &k)
    {
        j = {{"key", k.key}, {"amount", k.amount}};
    }

    inline void from_json(const nlohmann::json &j, KeyOutput &k)
    {
        k.key = j.at("key").get<Crypto::PublicKey>();
        k.amount = j.at("amount").get<uint64_t>();

        /* If we're talking to a daemon or blockchain cache
           that returns the globalIndex as part of the structure
           of a key output, then we need to load that into the
           data structure. */
        if (j.find("globalIndex") != j.end())
        {
            k.globalOutputIndex = j.at("globalIndex").get<uint64_t>();
        }
    }

    inline void to_json(nlohmann::json &j, const UnconfirmedInput &u)
    {
        j = {{"amount", u.amount}, {"key", u.key}, {"parentTransactionHash", u.parentTransactionHash}};
    }

    inline void from_json(const nlohmann::json &j, UnconfirmedInput &u)
    {
        u.amount = j.at("amount").get<uint64_t>();
        u.key = j.at("key").get<Crypto::PublicKey>();
        u.parentTransactionHash = j.at("parentTransactionHash").get<Crypto::Hash>();
    }
}
