//---------------------------------
// 2003/9/21 김태곤
//---------------------------------

#ifndef _TSPR_H
#define _TSPR_H

#include <Windows.h>
#include "../Directives.h"

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
	   siVersion		= TSPR_VERSION;
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

	   cltTSpr(){Image = NULL;}
	   ~cltTSpr()
	   {
		   if(Image)
		   {
			   delete [] Image;
			   Image = NULL;
		   }
	   }

	   void PutImgT(SI32 x, SI32 y, SI32 font, bool reverseswitch = false, UI08* ctable = NULL);
	   void PutSprT(SI32 x, SI32 y, SI32 font, bool reverseswitch = false, UI08* ctalbe = NULL, SI32 atb = 0);
	   void PutSprLight1T(SI32 x, SI32 y, SI32 font, bool reverseswitch = false);
	   void PutSprLightT(SI32 x, SI32 y, SI32 font, bool reverseswitch = false);
	   void PutSprShadowT(SI32 x, SI32 y, SI32 font, bool reverseswitch = false);
	   void PutSprAddT(SI32 x, SI32 y, SI32 font, bool reverseswitch = false);
	   void PutSprScreenT(SI32 x, SI32 y, SI32 font, bool reverseswitch = false);
	   void PutSprMultiplyT(SI32 x, SI32 y, SI32 font, bool reverseswitch = false);
	   void PutSprAlphaT(SI32 x, SI32 y, SI32 font, int alpha, bool reverseswitch = false);
	   void PutSprEFT(SI32 x, SI32 y, SI32 font, int effect, bool reverseswitch = false);
	   void PutMpr(SI32 x, SI32 y, SI32 font, SI32 mprmode, BOOL bClip,SI32 alphavalue = 4 );

	   // 숫자를 표시한다. 
	   SI32 DrawNumber(SI32 x, SI32 y, SI32 startfont, SI32 num);


};



#endif
