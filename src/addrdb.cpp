#include <addrdb.h>
#include <addrman.h>  // ✅ Ajouté pour AddrMan
#include <chainparams.h>
#include <clientversion.h>
#include <hash.h>
#include <logging.h>
#include <random.h>
#include <streams.h>
#include <tinyformat.h>
#include <util/system.h>  // ✅ Ajouté pour GetDataDir()
#include <util/time.h>
#include <util/translation.h>
#include <univalue.h>
#include <util/fs.h>  // ✅ Corrigé pour Bitcoin Core récent

const std::string ADDRDB_FILENAME = "peers.dat";

CAddrDB::CAddrDB() {}

bool CAddrDB::Write(const AddrMan& addr)
{
    int64_t nStart = GetTimeMillis();
    CDataStream ssPeers(SER_DISK, CLIENT_VERSION);
    ssPeers << addr;

    std::array<uint8_t, 4> netMagic = Params().DiskMagic(); // ✅ Correction ici

    CAutoFile file(fsbridge::fopen(GetDataDir() / ADDRDB_FILENAME, "wb"), SER_DISK, CLIENT_VERSION);
    if (file.IsNull()) {
        return error("%s: Failed to open file %s", __func__, ADDRDB_FILENAME);
    }

    file.write((char*)netMagic.data(), netMagic.size()); // ✅ Correction ici
    file << ssPeers;

    FileCommit(file.Get()); // ✅ Assurez-vous que `FileCommit` est bien inclus
    file.fclose();

    LogPrint(BCLog::ADDRMAN, "Written %d addresses to peers.dat  %dms\n",
             addr.Size(), GetTimeMillis() - nStart);
    return true;
}

bool CAddrDB::Read(AddrMan& addr)
{
    int64_t nStart = GetTimeMillis();
    CAutoFile file(fsbridge::fopen(GetDataDir() / ADDRDB_FILENAME, "rb"), SER_DISK, CLIENT_VERSION);
    if (file.IsNull()) {
        return error("%s: Failed to open file %s", __func__, ADDRDB_FILENAME);
    }

    std::array<uint8_t, 4> netMagic;
    file.read((char*)netMagic.data(), netMagic.size()); // ✅ Correction ici

    if (netMagic != Params().DiskMagic()) { // ✅ Correction ici
        return error("%s: Invalid network magic number", __func__);
    }

    CDataStream ssPeers(SER_DISK, CLIENT_VERSION);
    ssPeers << file;
    file.fclose();

    addr.Reset(); // ✅ Remplace `Clear()` par `Reset()` si nécessaire
    ssPeers >> addr;

    LogPrint(BCLog::ADDRMAN, "Loaded %d addresses from peers.dat  %dms\n",
             addr.Size(), GetTimeMillis() - nStart);
    return true;
}
