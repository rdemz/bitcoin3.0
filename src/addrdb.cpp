// Copyright (c) 2009-2022 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <addrdb.h>
#include <addrman.h>
#include <chainparams.h>
#include <clientversion.h>
#include <logging.h>
#include <util/fs.h>
#include <util/system.h>
#include <util/translation.h>

const std::string ADDR_FILENAME = "peers.dat";

CAddrDB::CAddrDB()
{
    pathAddr = gArgs.GetDataDirNet() / ADDR_FILENAME;
}

bool CAddrDB::Write(const AddrMan& addr)
{
    int64_t start = GetTimeMillis();
    CAutoFile file(fsbridge::fopen(pathAddr, "wb"), SER_DISK, CLIENT_VERSION);
    if (file.IsNull()) {
        LogPrintf("CAddrDB::Write(): Erreur lors de l'ouverture du fichier %s\n", pathAddr.string());
        return false;
    }

    // Utilisation du magic number du réseau
    std::array<uint8_t, 4> netMagic = Params().MessageStart();  // ✅ Correction ici
    file.write(Span<const std::byte>(reinterpret_cast<const std::byte*>(netMagic.data()), netMagic.size()));

    file << addr;
    file.fclose();

    LogPrintf("CAddrDB::Write(): Enregistrement terminé en %dms\n", GetTimeMillis() - start);
    return true;
}

bool CAddrDB::Read(AddrMan& addr)
{
    int64_t start = GetTimeMillis();
    CAutoFile file(fsbridge::fopen(pathAddr, "rb"), SER_DISK, CLIENT_VERSION);
    if (file.IsNull()) {
        LogPrintf("CAddrDB::Read(): Impossible d'ouvrir %s\n", pathAddr.string());
        return false;
    }

    std::array<uint8_t, 4> netMagic;
    file.read(Span<std::byte>(reinterpret_cast<std::byte*>(netMagic.data()), netMagic.size()));

    if (netMagic != Params().MessageStart()) {  // ✅ Correction ici
        LogPrintf("CAddrDB::Read(): Fichier %s a un mauvais magic number\n", pathAddr.string());
        return false;
    }

    file >> addr;
    LogPrintf("CAddrDB::Read(): Lecture terminée en %dms\n", GetTimeMillis() - start);
    return true;
}
