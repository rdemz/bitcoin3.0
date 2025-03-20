// Copyright (c) 2021-2022 The Bitcoin 3.0 Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
//
#ifndef BITCOIN_TEST_UTIL_CHAINSTATE_H
#define BITCOIN_TEST_UTIL_CHAINSTATE_H

#include <clientversion.h>
#include <logging.h>
#include <node/context.h>
#include <node/utxo_snapshot.h>
#include <rpc/blockchain.h>
#include <test/util/setup_common.h>
#include <util/fs.h>
#include <validation.h>

#include <univalue.h>

const auto NoMalleation = [](AutoFile& file, node::SnapshotMetadata& meta){};

/**
 * Create and activate a UTXO snapshot, optionally providing a function to
 * malleate the snapshot.
 *
 * If `reset_chainstate` is true, reset the original chainstate back to the genesis
 * block. This allows us to simulate more realistic conditions in which a snapshot is
 * loaded into an otherwise mostly-uninitialized datadir. It also allows us to test
 * conditions that would otherwise cause shutdowns based on the IBD chainstate going
 * past the snapshot it generated.
 */
template<typename F = decltype(NoMalleation)>
static bool
CreateAndActivateUTXOSnapshot(
    TestingSetup* fixture,
    F malleation = NoMalleation,
    bool reset_chainstate = false,
    bool in_memory_chainstate = false)
{
    node::NodeContext& node = fixture->m_node;
    fs::path root = fixture->m_path_root;

    // Write out a snapshot to the test's tempdir.
    //
    int height;
    WITH_LOCK(::cs_main, height = node.chainman->ActiveHeight());
    fs::path snapshot_path = root / fs::u8path(tfm::format("test_snapshot.%d.dat", height));
    FILE* outfile{fsbridge::fopen(snapshot_path, "wb")};
    AutoFile auto_outfile{outfile};

    UniValue result = CreateUTXOSnapshot(
        node, node.chainman->ActiveChainstate(), auto_outfile, snapshot_path, snapshot_path);
    LogPrintf(
        "Wrote UTXO snapshot to %s: %s", fs::PathToString(snapshot_path.make_preferred()), result.write());

    // Read the written snapshot in and then activate it.
    //
    FILE* infile{fsbridge::fopen(snapshot_path, "rb")};
    AutoFile auto_infile{infile};
    node::SnapshotMetadata metadata;
    auto_infile >> metadata;

    malleation(auto_infile, metadata);

    if (reset_chainstate) {
        {
            // What follows is code to selectively reset chainstate data without
            // disturbing the existing BlockManager instance, which is needed to
            // recognize the headers chain previously generated by the chainstate we're
            // removing. Without those headers, we can't activate the snapshot below.
            //
            // This is a stripped-down version of node::LoadChainstate which
            // preserves the block index.
            LOCK(::cs_main);
            uint256 gen_hash = node.chainman->ActiveChainstate().m_chain[0]->GetBlockHash();
            node.chainman->ResetChainstates();
            node.chainman->InitializeChainstate(node.mempool.get());
            Chainstate& chain = node.chainman->ActiveChainstate();
            Assert(chain.LoadGenesisBlock());
            // These cache values will be corrected shortly in `MaybeRebalanceCaches`.
            chain.InitCoinsDB(1 << 20, true, false, "");
            chain.InitCoinsCache(1 << 20);
            chain.CoinsTip().SetBestBlock(gen_hash);
            chain.setBlockIndexCandidates.insert(node.chainman->m_blockman.LookupBlockIndex(gen_hash));
            chain.LoadChainTip();
            node.chainman->MaybeRebalanceCaches();
        }
        BlockValidationState state;
        if (!node.chainman->ActiveChainstate().ActivateBestChain(state)) {
            throw std::runtime_error(strprintf("ActivateBestChain failed. (%s)", state.ToString()));
        }
        Assert(
            0 == WITH_LOCK(node.chainman->GetMutex(), return node.chainman->ActiveHeight()));
    }

    return node.chainman->ActivateSnapshot(auto_infile, metadata, in_memory_chainstate);
}


#endif // BITCOIN_TEST_UTIL_CHAINSTATE_H
