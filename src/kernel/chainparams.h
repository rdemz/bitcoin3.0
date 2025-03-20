// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_KERNEL_CHAINPARAMS_H
#define BITCOIN_KERNEL_CHAINPARAMS_H

#include <consensus/params.h>
#include <uint256.h>
#include <vector>
#include <string>

/**
 * Définition des paramètres de la blockchain Bitcoin 3.0.
 *
 * Ce fichier gère les paramètres spécifiques au protocole de consensus,
 * la validation des blocs et les règles de réseau.
 */

class CChainParams {
public:
    // Retourne les paramètres de consensus
    const Consensus::Params& GetConsensus() const { return consensus; }

    // Retourne le hash du bloc Genesis
    const uint256& GenesisBlockHash() const { return consensus.hashGenesisBlock; }

    // Retourne la chaîne de caractères définissant le réseau (mainnet, testnet, regtest)
    const std::string& NetworkID() const { return strNetworkID; }

protected:
    CChainParams() {}

    std::string strNetworkID;
    Consensus::Params consensus;
};

#endif // BITCOIN_KERNEL_CHAINPARAMS_H
