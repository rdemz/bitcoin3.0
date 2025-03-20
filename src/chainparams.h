// Copyright (c) 2009-2022 The Bitcoin 3.0 Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_CHAINPARAMS_H
#define BITCOIN_CHAINPARAMS_H

#include <kernel/chainparams.h> // 📌 Inclusion de la bonne version
#include <chainparamsbase.h>
#include <consensus/params.h>
#include <primitives/block.h>
#include <protocol.h>
#include <uint256.h>
#include <vector>
#include <memory>
#include <string>

/**
 * Retourne les paramètres de la chaîne sélectionnée.
 */
const CChainParams& Params();

/**
 * Définit les paramètres en fonction du réseau choisi (mainnet, testnet, regtest).
 */
void SelectParams(const std::string& chain);

/**
 * Crée les paramètres de la chaîne pour un réseau spécifique.
 */
std::unique_ptr<const CChainParams> CreateChainParams(const std::string& chain);

#endif // BITCOIN_CHAINPARAMS_H
