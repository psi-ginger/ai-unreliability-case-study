//-----------------------------------------------------------------------------
// 2005/09/13 이제완
//-----------------------------------------------------------------------------

#ifndef _TSPR_H
#define _TSPR_H

#include "NDataTypes.h"
#include "NDraw.h"

#define TSPR_ENCODETYPE_SPR			0	// 일반 스프라이트
#define TSPR_ENCODETYPE_MPR			1	// 타일용 스프라이트

#define TSPR_VERSION_NOPAL			10
#define TSPR_VERSION_16PAL			400


//---------------------------------------------------------------------------
// TSprImageInfo
//---------------------------------------------------------------------------
struct TSprImageInfo
{
	SI32	iCollInfo;
	SI32	iStartPos;
	SI32	iSize;
};

//---------------------------------------------------------------------------
// TSprHeader
//---------------------------------------------------------------------------

struct TSprHeader
{
public:
	SI32			iVersion;
	SI32			iEncodeType;
	SI32			iXSize;
	SI32			iYSize;
	SI32			iHRes;
	SI32			iVRes;
	SI32			iImageDataSize;

	SI32			reserved[ 8 ];

	SI32			iImageNum;
	TSprImageInfo	ImageInfo[ 256 ];
};

//---------------------------------------------------------------------------
// TSpr
//---------------------------------------------------------------------------
class TSpr
{     
public:
	TSpr();
	TSpr(TCHAR* filename){ pImage = NULL; LoadTSprFromFile(filename); }
	~TSpr();

	bool			LoadTSprFromFile( TCHAR* filename, int rgbmode = RGB16_MODE_565 );
	bool			LoadTSprFromMemory( BYTE* pMemory, int rgbmode = RGB16_MODE_565 );

	inline SI32		GetXSize() { return header.iXSize; }
	inline SI32		GetYSize() { return header.iYSize; }		
	inline SI32		GetImageNum() { return header.iImageNum; }
	inline SI32		GetImageDataSize() { return header.iImageDataSize; }
	inline SI32		GetHRes() { return header.iHRes; }
	inline SI32		GetVRes() { return header.iVRes; }		


public:
	TSprHeader		header;
	UI08*			pImage;

	RGB16			pal[ 256 ];
};


//---------------------------------------------------------------------------
// TPal
//---------------------------------------------------------------------------
class TPal 
{
public:
	TPal();
	~TPal();
	
	bool			LoadPalFromFile( TCHAR* filename, int rgbmode = RGB16_MODE_565 );
	RGB16			pal[ 256 ];
};

#endif