//-----------------------------------------------------------------------------
// 2009/02/26 ÀÌ°­Çü
//-----------------------------------------------------------------------------

#ifndef _NLZF_H
#define _NLZF_H

#include "../../NLib/NDataTypes.h"
extern "C" {
	#include "./libLZF/lzf.h"
}

class NLZF
{
public:
	static UI32 Compress(BYTE* pSrc, UI32 uiSrcSize, BYTE* pDest, UI32 uiDestMaxSize)
	{
		return lzf_compress( pSrc, uiSrcSize, pDest, uiDestMaxSize );
	}
	static UI32 Decompress(BYTE* pSrc, UI32 uiSrcSize, BYTE* pDest, UI32 uiDestMaxSize)
	{
		return lzf_decompress( pSrc, uiSrcSize, pDest, uiDestMaxSize );
	}
};

#endif