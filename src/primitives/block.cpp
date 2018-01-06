// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/block.h"

#include "hash.h"
#include "tinyformat.h"
#include "utilstrencodings.h"
#include "crypto/common.h"
#include "crypto/Lyra2Z/Lyra2Z.h"
#include "crypto/Lyra2Z/Lyra2.h"
#include "versionbits.h"


//sha256 hash
uint256 CBlockHeader::Getsha256Hash() const
{
    return SerializeHash(*this);
}


//lyra2z hash
uint256 CBlockHeader::Getlyra2zHash() const
{

    uint256 powHash;

    lyra2z_hash(BEGIN(nVersion), BEGIN(powHash));

    return powHash;
}

uint256 CBlockHeader::GetHash() const
{
    if(nVersion >= VERSIONBITS_TOP_BITS)
    {
        return Getlyra2zHash();
    }
    else
    {
        return Getsha256Hash();
    }
}

std::string CBlock::ToString() const
{
    std::stringstream s;
    s << strprintf("CBlock(hash=%s, ver=0x%08x, hashPrevBlock=%s, hashMerkleRoot=%s, nTime=%u, nBits=%08x, nNonce=%u, vtx=%u)\n",
        GetHash().ToString(),
        nVersion,
        hashPrevBlock.ToString(),
        hashMerkleRoot.ToString(),
        nTime, nBits, nNonce,
        vtx.size());
    for (const auto& tx : vtx) {
        s << "  " << tx->ToString() << "\n";
    }
    return s.str();
}
