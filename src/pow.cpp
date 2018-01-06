// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "primitives/block.h"
#include "uint256.h"
#include "chainparams.h"
#include "util.h"
#include "versionbits.h"

//arith_uint256 RRBTC_WORK_LIMIT = UintToArith256(uint256S("00000fffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));
arith_uint256 RRBTC_WORK_LIMIT = UintToArith256(uint256S("0000007fffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

/**
 *@brief powLimit
 *		硬分叉前使用 RBTC 的powLimit， 硬分叉后使用 RRBTC 的powLimit
 *		根据版本号 判断 硬分叉
 */
arith_uint256 GetLimit(int iVersion, const Consensus::Params& params)
{
	if(iVersion >= VERSIONBITS_TOP_BITS)
	{
		return RRBTC_WORK_LIMIT;
	}

	return UintToArith256(params.powLimit);
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);

    int nHeightNext = pindexLast->nHeight + 1;
    unsigned int nProofOfWorkLimit = GetLimit(pblock->nVersion, params).GetCompact();

    if( gArgs.GetArg("-synctype", DEFAULT_SYNC_TYPE) &&
   		(nHeightNext >= params.RRBTCHeight  && nHeightNext < params.RRBTCHeight + params.RRBTCPremineWindow))
    {
//    	LogPrintf("%s %d|premineWindow|nHeightNext=%d|RRBTCHeight=%d|RRBTCPremineWindow=%d|nProofOfWorkLimit=0x%x\n", __FILE__, __LINE__, nHeightNext, params.RRBTCHeight, params.RRBTCPremineWindow, nProofOfWorkLimit );

    	//synctype 为兼容模式，同时区块高度在窗口期，则每个区块都能调难度
    	// Lowest difficulty for Royal Royalbitcoin premining period.
    	return nProofOfWorkLimit;
    }
    else if (nHeightNext % params.DifficultyAdjustmentInterval() != 0)
    {
    	// Difficulty adjustment interval is not finished. Keep the last value.
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    int nHeightFirst = pindexLast->nHeight - (params.DifficultyAdjustmentInterval()-1);
    assert(nHeightFirst >= 0);
    const CBlockIndex* pindexFirst = pindexLast->GetAncestor(nHeightFirst);
    assert(pindexFirst);

    return CalculateNextWorkRequired(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    arith_uint256 bnNew;
    bnNew.SetCompact(pindexLast->nBits);
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, int iVersion, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    arith_uint256 auLimit = GetLimit(iVersion, params);
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > auLimit )
    {
    	LogPrintf("%s %d|iVersion=%d|fNegative=%d|bnTarget=%s|fOverflow=%d|auLimit=%s\n", __FILE__, __LINE__,
    			iVersion, fNegative, bnTarget.ToString(), fOverflow, auLimit.ToString());
        return false;
    }

    // Check proof of work matches claimed amount
    if (UintToArith256(hash) > bnTarget)
    {
    	LogPrintf("%s %d|hash=%s|nBits=0x%x|bnTarget=%s\n", __FILE__, __LINE__,
    			UintToArith256(hash).ToString() ,nBits, bnTarget.ToString() );
        return false;
    }

    return true;
}
