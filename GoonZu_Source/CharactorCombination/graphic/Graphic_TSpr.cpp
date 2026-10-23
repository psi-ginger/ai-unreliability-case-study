//---------------------------------
// 2003/9/21 김태곤
//---------------------------------

#include "Graphic.h"
#include "Graphic_Capsule.h"


void cltTSpr::PutImgT(SI32 x, SI32 y, SI32 font, bool reverseswitch, UI08* ctable)
{
//	if(font < 0 || font >= clHeader.siFontNum)return ;

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif


	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(ctable)
		{
			GP.PutSprTable( x, y, clHeader.siXsize, clHeader.siYsize,
							&Image[ clHeader.clImgInfo[font].siStartPos], ctable,
							PUT_SPRTABLE_IMG,SPR_MODE_T);
		}
		else
		{
			GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
					   &Image[ clHeader.clImgInfo[font].siStartPos],
					   PUT_SPR_IMG,SPR_MODE_T);
		}
		
	}
	else
	{
		GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
				   &Image[ clHeader.clImgInfo[font].siStartPos],
				   PUT_SPR_IMG,SPR_MODE_CT);
	}
	
#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif


}


void cltTSpr::PutSprT(SI32 x, SI32 y, SI32 font, bool reverseswitch, UI08* ctalbe, SI32 atb)
{
//	if(font < 0 || font >= clHeader.siFontNum)return ;

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

	SI32 xsize		= clHeader.siXsize;
	SI32 ysize		= clHeader.siYsize;
	UI08* pstart	= &Image[ clHeader.clImgInfo[font].siStartPos ];

	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(ctalbe)
		{
			if(atb == 0)
			{
				if(reverseswitch)
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_SPR,SPR_MODE_RT);
				}
				else
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_SPR,SPR_MODE_T);
				}
			}
			if(atb & SPRATB_DOT)
			{
				if(reverseswitch)
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_DOT,SPR_MODE_RT);
				}
				else
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_DOT,SPR_MODE_T);
				}
			}

		}
		else
		{
			if(atb == 0)
			{
				if(reverseswitch)
				{
					GP.PutSpr( x, y, xsize, ysize, pstart,
							   PUT_SPR_SPR,SPR_MODE_RT);
				}
				else
				{
					GP.PutSpr( x, y, xsize, ysize, pstart,
							   PUT_SPR_SPR,SPR_MODE_T);
				}
			}

		}
	}
	else
	{
		if(ctalbe)
		{
			if(atb == 0)
			{
				if(reverseswitch)
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_SPR,SPR_MODE_CRT);
				}
				else
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_SPR,SPR_MODE_CT);
				}
			}
			if(atb & SPRATB_DOT)
			{
				if(reverseswitch)
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_DOT,SPR_MODE_CRT);
				}
				else
				{
					GP.PutSprTable( x, y, xsize, ysize,
									pstart, ctalbe,
									PUT_SPRTABLE_DOT,SPR_MODE_CT);
				}
			}

		}
		else
		{
			if(atb == 0)
			{
				if(reverseswitch)
				{
					GP.PutSpr( x, y, xsize, ysize, pstart,
							   PUT_SPR_SPR,SPR_MODE_CRT);
				}
				else
				{
					GP.PutSpr( x, y, xsize, ysize, pstart,
							   PUT_SPR_SPR,SPR_MODE_CT);
				}
			}

			if(atb & SPRATB_DOT)
			{
			}
		}
	}

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif

}

void cltTSpr::PutSprLight1T(SI32 x, SI32 y, SI32 font, bool reverseswitch)
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_RT ;
		}
		else
		{
			sprmode = SPR_MODE_T ;
		}
	}
	else
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_CRT ;
		}
		else
		{
			sprmode = SPR_MODE_CT ;
		}
	}

	GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
			   &Image[clHeader.clImgInfo[font].siStartPos ],
			   PUT_SPR_LIGHT1,sprmode);

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
}

void cltTSpr::PutSprLightT(SI32 x, SI32 y, SI32 font, bool reverseswitch)
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_RT ;
		}
		else
		{
			sprmode = SPR_MODE_T ;
		}
	}
	else
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_CRT ;
		}
		else
		{
			sprmode = SPR_MODE_CT ;
		}
	}

	GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
			   &Image[clHeader.clImgInfo[font].siStartPos ],
			   PUT_SPR_LIGHT,sprmode);

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
}

void cltTSpr::PutSprShadowT(SI32 x, SI32 y, SI32 font, bool reverseswitch)
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_RT ;
		}
		else
		{
			sprmode = SPR_MODE_T ;
		}
	}
	else
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_CRT ;
		}
		else
		{
			sprmode = SPR_MODE_CT ;
		}
	}

	GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
			   &Image[clHeader.clImgInfo[font].siStartPos ],
			   PUT_SPR_SHADOW,sprmode);

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
}


// JAYCEE EXCHANGED
void cltTSpr::PutSprAddT(SI32 x, SI32 y, SI32 font, bool reverseswitch)
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_RT ;
		}
		else
		{
			sprmode = SPR_MODE_T ;
		}
	}
	else
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_CRT ;
		}
		else
		{
			sprmode = SPR_MODE_CT ;
		}

	}

	GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
			   &Image[clHeader.clImgInfo[font].siStartPos ],
			   PUT_SPR_ADD,sprmode);

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
}

void cltTSpr::PutSprScreenT(SI32 x, SI32 y, SI32 font, bool reverseswitch)
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_RT ;
		}
		else
		{
			sprmode = SPR_MODE_T ;
		}
	}
	else
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_CRT ;
		}
		else
		{
			sprmode = SPR_MODE_CT ;
		}
	}

	GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
			   &Image[clHeader.clImgInfo[font].siStartPos ],
			   PUT_SPR_SCREEN,sprmode);

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
}


void cltTSpr::PutSprMultiplyT(SI32 x, SI32 y, SI32 font, bool reverseswitch)
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_RT ;
		}
		else
		{
			sprmode = SPR_MODE_T ;
		}
	}
	else
	{
		if(reverseswitch)
		{
			sprmode = SPR_MODE_CRT ;
		}
		else
		{
			sprmode = SPR_MODE_CT ;
		}
	}

	GP.PutSpr( x, y, clHeader.siXsize, clHeader.siYsize,
			   &Image[clHeader.clImgInfo[font].siStartPos ],
			   PUT_SPR_MULTIPLY,sprmode);

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
}

void cltTSpr::PutSprAlphaT(SI32 x, SI32 y, SI32 font, int alpha, bool reverseswitch )
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

#ifdef GRA_MODE_16

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			g_graphics.PutSpr16RAT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				alpha );
		}
		else
		{
			g_graphics.PutSpr16AT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				alpha );
		}
	}
	else
	{
		if(reverseswitch)
		{
			g_graphics.PutSpr16CRAT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				alpha );
		}
		else
		{
			g_graphics.PutSpr16CAT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				alpha );
		}
	}

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif


#endif
}
void cltTSpr::PutSprEFT(SI32 x, SI32 y, SI32 font, int effect, bool reverseswitch )
{

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif

#ifdef GRA_MODE_16

	SI32 sprmode = 0 ;
	if(GP.IsInClipArea(x, y, x + clHeader.siXsize - 1, y + clHeader.siYsize -1) == TRUE)
	{
		if(reverseswitch)
		{
			g_graphics.PutSpr16REFT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				effect );
		}
		else
		{
			g_graphics.PutSpr16EFT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				effect );
		}
	}
	else
	{
		if(reverseswitch)
		{
			g_graphics.PutSpr16CREFT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				effect );
		}
		else
		{
			g_graphics.PutSpr16CEFT(
				x, y,
				&Image[clHeader.clImgInfo[font].siStartPos ],
				clHeader.siXsize, clHeader.siYsize,
				effect );
		}
	}

#endif

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
}

void cltTSpr::PutMpr(SI32 x, SI32 y, SI32 font, SI32 mprmode, BOOL bClip ,SI32 alphavalue)
{
	
#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( pal );
	}

#endif
 
	GP.PutMpr( x, y, clHeader.siXsize, clHeader.siYsize,
		&Image[ clHeader.clImgInfo[font].siStartPos ], mprmode, bClip ,alphavalue);

#ifdef GRA_MODE_16

	if( clHeader.siVersion == 400 ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

#endif
  
}


// 숫자를 표시한다. 
#define MAX_DIGIT_NUMBER 20
SI32 cltTSpr::DrawNumber(SI32 x, SI32 y, SI32 startfont, SI32 num)
{
	SI32 i;
	SI32 digit[MAX_DIGIT_NUMBER];
	SI32 digitnum = 0;		// 숫자의 개수. 
	SI32 mod;
	SI32 oldnum;

	while(1)
	{
		oldnum = num;
		mod = num % 10;
		num = num / 10;

		digit[digitnum] = mod;
		digitnum++;
		if(digitnum >= MAX_DIGIT_NUMBER-1)break;

		if(oldnum == mod)break;
	}

	SI32 index = 0;
	for(i = digitnum-1;i >= 0;i--)
	{
		PutSprT(x + index, y, startfont + digit[i]);
		index += clHeader.siXsize; 
	}

	return x + index;
}
