// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2018 The PIVX developers
// Copyright (c) 2018-2019 The BDCashProtocol developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
(0,  uint256("0x0000ae49910f3ea69ebd9cc2d961908280ecd2b96532e206e8c37361faf1a51e"));

static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1652985653, // * UNIX timestamp of last checkpoint block
    104,     // * total number of transactions between genesis and last checkpoint
           //   (the tx=... number in the SetBestChain debug.log lines)
    1400        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
    boost::assign::map_list_of(0, uint256("0x000066ac3487857739e403a8d9ea432a455786a6b9f0f61d9188f36ddffa16b8"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1652478901,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
    boost::assign::map_list_of(0, uint256("0x000066ac3487857739e403a8d9ea432a455786a6b9f0f61d9188f36ddffa16b8"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1652478901,
    0,
    250};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params(bool useModulusV1) const
{
    assert(this);
    static CBigNum bnHexModulus = 0;
    if (!bnHexModulus)
        bnHexModulus.SetHex(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsHex = libzerocoin::ZerocoinParams(bnHexModulus);
    static CBigNum bnDecModulus = 0;
    if (!bnDecModulus)
        bnDecModulus.SetDec(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParamsDec = libzerocoin::ZerocoinParams(bnDecModulus);

    if (useModulusV1)
        return &ZCParamsHex;

    return &ZCParamsDec;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xCb;
        pchMessageStart[1] = 0x63;
        pchMessageStart[2] = 0x5b;
        pchMessageStart[3] = 0x65;
        vAlertPubKey = ParseHex("04580fe5e61ddd2d91fa29b1581e6d0b9291339e6b8f245da266f1558969e0798b90a801d3559ff19fb2f76e0dac4c9363385873d945d57e5592889d9dbd0d243");
        nDefaultPort = 17293;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // BDCashProtocol starting difficulty is 1 / 2^12
        nMaxReorganizationDepth = 100;
        nMinerThreads = 0;
        nTargetSpacing = 2 * 60;  // BDECO: 1 minutes
        nMaturity = 25;
        nMaxMoneyOut = 210000000 * COIN;

        nSubsidyHalvingBlock = 1000;
        strDevFeeAddress = "BNvmbM7ky7K4UWXramCjnLgUn5SpdnAce4";

        /** Height or Time Based Activations **/
        nLastPOWBlock = 200;
        nModifierUpdateBlock = 0;
        nZerocoinStartHeight = 0;
        nZerocoinStartTime = 1547096400; // Genesis time
        nBlockZerocoinV2 = 20;

        /**
         * Build the genesis block. Note that the output of the genesis coinbase cannot
         * be spent as it did not originally exist in the database.
         *
         * CBlock(hash=00000ffd590b14, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=e0028e, nTime=1390095618, nBits=1e0ffff0, nNonce=28917698, vtx=1)
         *   CTransaction(hash=e0028e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
         *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d01044c5957697265642030392f4a616e2f3230313420546865204772616e64204578706572696d656e7420476f6573204c6976653a204f76657273746f636b2e636f6d204973204e6f7720416363657074696e6720426974636f696e73)
         *     CTxOut(nValue=50.00000000, scriptPubKey=0xA9037BAC7050C479B121CF)
         *   vMerkleTree: e0028e
         */
        const char* pszTimestamp = "New BDCashProtocol Genesis Block mined by Team in 05/2022";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 0 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("043373ee12c4ddbe0d2248a606c1825ef44dd4e01fa93c52d4def644d6366010ad6a514820c39f37a388c7a11fb69608d359c227cd1cc178b945d824122a59c8db") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 4;
        genesis.nTime = 1652478815;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 13533;

        hashGenesisBlock = genesis.GetHash();
        //printf("Genesis mainnet - %s\n",hashGenesisBlock.ToString().c_str());
        //printf("Merkle mainnet - %s\n",genesis.hashMerkleRoot.ToString().c_str());


        assert(hashGenesisBlock == uint256("0x0000ae49910f3ea69ebd9cc2d961908280ecd2b96532e206e8c37361faf1a51e"));
        assert(genesis.hashMerkleRoot == uint256("0xf4b45a6364c91c274d179429a09202a23fe2a6da548c81334d49ae19503c7f28"));

        vSeeds.push_back(CDNSSeedData("seed1.bdcashprotocol.com", "seed1.bdcashprotocol.com"));
        vSeeds.push_back(CDNSSeedData("seed2.bdcashprotocol.com", "seed2.bdcashprotocol.com"));
        vSeeds.push_back(CDNSSeedData("seed3.bdcashprotocol.com", "seed3.bdcashprotocol.com"));
        vSeeds.push_back(CDNSSeedData("coinstake.in", "coinstake.in"));


        // BDCashProtocol addresses start with 'B'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 25);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 26);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 25);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0xB2)(0x47)(0x46).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0xB2)(0x43)(0x08).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x02)(0x62).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "04a572ff675fdefd60a12aa7e1ac3bb678f6830562a1e0fc04a865f361f7c7843caa9b1a2c3c6151fc14f0407430ea5073d242b2412200381a1d6a1c62a9a5efad";
        strObfuscationPoolDummyAddress = "BP2L1hjtFBipitn7Kv5rdFrRAdypv4cThs";
        nStartMasternodePayments = 1547119800; // historical param

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 1;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 4; //Block headers must be this version once zerocoin is active
        nZerocoinRequiredStakeDepth = 200; //The required confirmations for a zbdeco to be stakable

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xCB;
        pchMessageStart[1] = 0x61;
        pchMessageStart[2] = 0x85;
        pchMessageStart[3] = 0x50;
        vAlertPubKey = ParseHex("044e508e93fc331ec5c31e6064b93f8a2a4d8a9a31db450373f5095af1cf496ad02f68e63cd1a1ed73192dd6727521b9ece35690d345112777de2b21d84af19917");
        nDefaultPort = 27293;
        nMinerThreads = 0;
        nTargetSpacing = 2 * 60;
        nLastPOWBlock = 500;
        nMaturity = 15;
        nModifierUpdateBlock = 0;
        nMaxMoneyOut = 210000000 * COIN;
        nZerocoinStartHeight = 15;
        nZerocoinStartTime = 1547096400;
        nBlockZerocoinV2 = 15;

        nSubsidyHalvingBlock = 1600;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1652478901;
        genesis.nNonce = 24539;

        hashGenesisBlock = genesis.GetHash();

        

        assert(hashGenesisBlock == uint256("0x000066ac3487857739e403a8d9ea432a455786a6b9f0f61d9188f36ddffa16b8"));

        vSeeds.push_back(CDNSSeedData("seedtest1.bdcashprotocol.com", "seedtest1.bdcashprotocol.com"));
        vSeeds.push_back(CDNSSeedData("seedtest2.bdcashprotocol.com", "seedtest2.bdcashprotocol.com"));
        vSeeds.push_back(CDNSSeedData("seedtest3.bdcashprotocol.com", "seedtest3.bdcashprotocol.com"));
        

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 85); // Testnet bdcashprotocol addresses start with 'b'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 86);  // Testnet bdcashprotocol script addresses start with 'b' or 'c'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 86);     // Testnet private keys start with 'b' or 'c'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x04)(0x35)(0x87)(0xCF).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x04)(0x35)(0x83)(0x94).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x02)(0x6e).convert_to_container<std::vector<unsigned char> >();

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04954f6895da5881ff8d9ae41678b3b3780f6fa4365ba8f1e7ff03ea2e2582548fc1412fe5e39c9f574779856ec12d3f1b5a253638594613e0f77bcd28d529b04e";
        strObfuscationPoolDummyAddress = "bSQyimHVPYeoptR2fSSNtqow51PwrWdNbe";
        nStartMasternodePayments = 1547119740 + 500 * 120;
        nBudget_Fee_Confirmations = 3;

    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0x31;
        pchMessageStart[1] = 0x45;
        pchMessageStart[2] = 0x27;
        pchMessageStart[3] = 0xA9;
        nMinerThreads = 1;
        nTargetSpacing = 2 * 60;
        nDefaultPort = 37293;

        bnProofOfWorkLimit = ~uint256(0) >> 1;
        genesis.nTime = 1652478901;
        genesis.nBits = 0x1e0ffff0;
        genesis.nNonce = 24539;

        hashGenesisBlock = genesis.GetHash();


        //printf("Genesis RegTest - %s\n",hashGenesisBlock.ToString().c_str());
        //printf("Merkle RegTest - %s\n",genesis.hashMerkleRoot.ToString().c_str());

        assert(hashGenesisBlock == uint256("0x000066ac3487857739e403a8d9ea432a455786a6b9f0f61d9188f36ddffa16b8"));

        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 49293;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingBlock(int anSubsidyHalvingBlock) { nSubsidyHalvingBlock = anSubsidyHalvingBlock; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
