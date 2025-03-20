// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <chainparams.h>
#include <chainparamsseeds.h>
#include <consensus/merkle.h>
#include <deploymentinfo.h>
#include <hash.h>
#include <script/interpreter.h>
#include <util/string.h>
#include <util/system.h>

#include <assert.h>

// ======================= PARAMÈTRES DE BITCOIN 3.0 =========================
class CBitcoin3Params : public CChainParams {
public:
    CBitcoin3Params() {
        strNetworkID = "main";

        // Récompense 5 fois plus élevée que Bitcoin
        consensus.nSubsidyHalvingInterval = 210000;
        consensus.nPowTargetSpacing = 10 * 60; // 10 minutes par bloc
        consensus.nPowTargetTimespan = 14 * 24 * 60 * 60; // 2 semaines

        // Algorithme de minage (KawPow pour favoriser le GPU)
        consensus.nPowAlgorithm = "KawPow";
        consensus.nPowLimit = uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");

        // Magic Bytes (Identification du réseau)
        pchMessageStart[0] = 0xf9;
        pchMessageStart[1] = 0xbe;
        pchMessageStart[2] = 0xb4;
        pchMessageStart[3] = 0xd9;

        // Ports réseau
        nDefaultPort = 8333;
        nTestNetDefaultPort = 18333;
        nRegtestDefaultPort = 18444;

        // Bloc de genèse (à recalculer si nécessaire)
        genesis = CreateGenesisBlock(
            "Bitcoin 3.0 Genesis Block - 2025",  // Timestamp
            CScript() << ParseHex("04ffff001d0104"),  // Script public
            1700000000,  // Timestamp
            2,  // Nonce ajusté pour le minage
            0x1e0ffff0,  // Bits de difficulté initiale
            1,  // Version du bloc
            250 * COIN  // Récompense de 250 BTC3
        );

        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0x00000...")); // Remplace avec le vrai hash

        vFixedSeeds.clear();
        vSeeds.clear();

        // Préfixes pour les adresses (ex: PUBKEY_ADDRESS commence par "3")
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 0x26);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 0x32);
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 0x80);
    }
};

// ===================== CRÉATION DES PARAMÈTRES ======================
static std::unique_ptr<const CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<const CChainParams> CreateChainParams(const ArgsManager& args, const std::string& chain) {
    if (chain == CBaseChainParams::MAIN) {
        return std::make_unique<CBitcoin3Params>();
    } else if (chain == CBaseChainParams::TESTNET) {
        return CChainParams::TestNet();
    } else if (chain == CBaseChainParams::SIGNET) {
        auto opts = CChainParams::SigNetOptions{};
        ReadSigNetArgs(args, opts);
        return CChainParams::SigNet(opts);
    } else if (chain == CBaseChainParams::REGTEST) {
        auto opts = CChainParams::RegTestOptions{};
        ReadRegTestArgs(args, opts);
        return CChainParams::RegTest(opts);
    }
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network) {
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(gArgs, network);
}
