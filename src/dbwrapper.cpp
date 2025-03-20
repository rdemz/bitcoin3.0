// Copyright (c) 2009-2021 The Bitcoin 3.0 Core developers
// Distributed under the MIT software license

#include <dbwrapper.h>
#include <util/system.h>
#include <util/fs.h>
#include <logging.h>
#include <chainparams.h>
#include <clientversion.h>

// Modification : Correspond à dbwrapper.h
CDBWrapper::CDBWrapper(const DBParams& params) {
    options.create_if_missing = true;
    options.info_log = nullptr;
    options.paranoid_checks = true;

    if (params.memory_only) {
        options.env = nullptr;
    } else {
        fs::create_directories(params.path);
        options.env = nullptr;
    }
}
