// Copyright (c) 2022 The Bitcoin 3.0 Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <node/database_args.h>

#include <dbwrapper.h>
#include <util/system.h>

namespace node {
void ReadDatabaseArgs(const ArgsManager& args, DBOptions& options)
{
    // Settings here apply to all databases (chainstate, blocks, and index
    // databases), but it'd be easy to parse database-specific options by adding
    // a database_type string or enum parameter to this function.
    if (auto value = args.GetBoolArg("-forcecompactdb")) options.force_compact = *value;
}
} // namespace node
