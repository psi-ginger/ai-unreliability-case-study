#ifndef _SPRFILE_H
#define _SPRFILE_H

#include "directives.h"

//-------------------------------------------------
// Spr 종류 
//-------------------------------------------------
#define SPRTYPE_SPR				0	// 일반 스프라이트 (0번 압축. ) 
#define SPRTYPE_TLE				1	// 타일용 스프라이트 (0번 압축.) 

//-------------------------------------------------
// 출력 속성. 
//-------------------------------------------------
#define SPRATB_DOT				1

//-------------------------------------------------
// cltTSprImgInfo
//-------------------------------------------------
class cltTSprImgInfo{
public:
	SI32 siCollInfo;
	SI32 siStartPos;
	SI32 siLength;

	cltTSprImgInfo()
	{
		siCollInfo	= 0;
		siStartPos	= 0;
		siLength	= 0;
	}

};

//-------------------------------------------------
// cltTSprHeader
//-------------------------------------------------
#define TSPR_VERSION			10
#define NSPR_VERSION			400

#define MAX_IMG_PER_TSPR		256	// Spr 파일 하나당 이미지의 개수.

class cltTSprHeader
{
public:
   SI32 siVersion;
   SI32	siSprType;		// 어떤 타입의 Spr인가 ?
   SI32 siXsize;
   SI32 siYsize;
   SI32 siHRes;
   SI32 siVRes;
   SI32 siTotalLength;

   SI32 Reserved[8];
   
   SI32 siFontNum;
   cltTSprImgInfo clImgInfo[MAX_IMG_PER_TSPR];
   
   cltTSprHeader()
   {
	   //siVersion		= TSPR_VERSION;
	   siVersion		= NSPR_VERSION;
	   siSprType		= SPRTYPE_SPR;
	   siXsize			= 0;
	   siYsize			= 0;	
	   siHRes			= 0;
	   siVRes			= 0;
	   
	   siTotalLength	= 0;
	   siFontNum		= 0;
   }
   
};

//-----------------------------------------------
// cltTSpr
//-----------------------------------------------
class	cltTSpr
{     

public:
	cltTSprHeader	clHeader;
	UI08			*Image;

	UI16			pal[ 256 ];

	cltTSpr();
	~cltTSpr();

	void			ClearImageData();

	BOOL			LoadSpr( char *filename );
};

#define RGB16_565_RMASK		0xF800	
#define RGB16_565_GMASK		0x07E0
#define RGB16_565_BMASK		0x001F

#define RGB16_555_RMASK		0x7C00
#define RGB16_555_GMASK		0x03E0
#define RGB16_555_BMASK		0x001F

#endif