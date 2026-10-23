//---------------------------------
// 2005/09/22 이제완
//---------------------------------

#include "../NGraphics/Graphics.h"

#include "EtcTool/EtcTool.h"
#include "Graphic_Capsule.h"

cltGraphic_Capsule GP;

NGraphics	g_graphics;

cltGraphic_Capsule::cltGraphic_Capsule()
{
}

cltGraphic_Capsule::~cltGraphic_Capsule()
{
}

BOOL cltGraphic_Capsule::InitGP(HWND hwnd,SI32 screenmode,LPDIRECTDRAWSURFACE7 & lpbackscreen,
								SI32 xsize , SI32 ysize, BOOL bFullScreen, SI32 screenXSize, SI32 screenYSize,
								bool bNewFullScreen)
{

	if( screenXSize == 0 || screenYSize == 0 ) {
		screenXSize = xsize;
		screenYSize = ysize;
	}

	if( bFullScreen ) {

		if( g_graphics.CreateFullScreenGraphics( hwnd, screenXSize, screenYSize, 16, xsize, ysize, bNewFullScreen ) ) {

			lpbackscreen = g_graphics.GetBackBuffer();

			clGrp.lpDD7 = g_graphics.GetDirectDraw();
			clGrp.lpDDSPrimary = g_graphics.GetFrontBuffer();
			clGrp.lpDDSBack = g_graphics.GetBackBuffer();
			clGrp.lpDDSConvertSurface = g_graphics.GetConvertBuffer();

			clGrp.ScreenXsize = xsize;
			clGrp.ScreenYsize = ysize;
			clGrp.ColorDepth = 8;

			g_graphics.CreateAlphaTable();
			g_graphics.CreateEffectTable();
			g_graphics.CreateEffectFTable();

			g_graphics.CreateColorTable( 1 );
			g_graphics.LoadColorTableFromFile( TEXT("pal\\Monarch.pal") );

		} else {

			MessageBox(NULL, TEXT("Display Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
			return FALSE;
		}

	} else {


		if( g_graphics.CreateWindowedGraphics( hwnd, screenXSize, screenYSize, 16, xsize, ysize ) ) {

			lpbackscreen = g_graphics.GetBackBuffer();

			clGrp.lpDD7 = g_graphics.GetDirectDraw();
			clGrp.lpDDSPrimary = g_graphics.GetFrontBuffer();
			clGrp.lpDDSBack = g_graphics.GetBackBuffer();
			clGrp.lpDDSConvertSurface = g_graphics.GetConvertBuffer();

			clGrp.ScreenXsize = xsize;
			clGrp.ScreenYsize = ysize;
			clGrp.ColorDepth = 8;

			g_graphics.CreateAlphaTable();
			g_graphics.CreateEffectTable();
			g_graphics.CreateEffectFTable();

			g_graphics.CreateColorTable( 1 );
			g_graphics.LoadColorTableFromFile( TEXT("pal\\Monarch.pal") );

		} else {

			MessageBox(NULL, TEXT("Display Error"), TEXT("Error"), MB_OK|MB_TOPMOST);
			return FALSE;
		}	
	}



	// 클리핑 영역을 화면 전체로 설정한다. 
	clGrp.SetClipFullScreen();

	return TRUE ;
}

//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSpr( TSpr *pSpr, int x, int y, int font, BOOL bReverse )
{
	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}
 
	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16RT( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
		} else {
			g_graphics.PutSpr16T( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CRT( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
		} else {
			g_graphics.PutSpr16CT( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
		}

	}

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}

//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprRS( TSpr *pSpr, int x, int y, int font /* = 0 */, BOOL bReverse /* = FALSE */, FLOAT fRotateDegree /* = 0.0f */, FLOAT fScale /* = 1.0f */ )
{
	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			if (fRotateDegree > 0.0f || fScale != 1.0f) {
				g_graphics.PutSpr16SRT( x, y, &pSpr->pImage[ startpos ], xsize, ysize , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16RT( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
			}
			
		} else {
			if (fRotateDegree > 0.0f || fScale != 1.0f) {
				g_graphics.PutSpr16ST( x, y, &pSpr->pImage[ startpos ], xsize, ysize , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16T( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
			}
			
		}

	} else {

		if( bReverse ) {
			if (fRotateDegree > 0.0f || fScale != 1.0f) {	// 기본적으로 클리핑 처리
				g_graphics.PutSpr16SRT( x, y, &pSpr->pImage[ startpos ], xsize, ysize , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16CRT( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
			}			
		} else {
			if (fRotateDegree > 0.0f || fScale != 1.0f) {	// 기본적으로 클리핑 처리
				g_graphics.PutSpr16ST( x, y, &pSpr->pImage[ startpos ], xsize, ysize , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16CT( x, y, &pSpr->pImage[ startpos ], xsize, ysize );
			}
		}

	}

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}

//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprScreen( TSpr *pSpr, int x, int y, int font, BOOL bReverse )
{
	if(pSpr == NULL)			return;
	if(pSpr->pImage == NULL)	return;

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16RET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GSCREEN );
		} else {
			g_graphics.PutSpr16ET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GSCREEN );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CRET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GSCREEN );
		} else {
			g_graphics.PutSpr16CET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GSCREEN );
		}
	}	

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}


//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprMultiply( TSpr *pSpr, int x, int y, int font, BOOL bReverse )
{
	if(pSpr == NULL || pSpr->pImage == NULL)			return;

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}


	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16RET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GMULTIPLY );
		} else {
			g_graphics.PutSpr16ET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GMULTIPLY );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CRET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GMULTIPLY );
		} else {
			g_graphics.PutSpr16CET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GMULTIPLY );
		}
	}	

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}


//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprAdd( TSpr *pSpr, int x, int y, int font, BOOL bReverse )
{

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16RET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GADD );
		} else {
			g_graphics.PutSpr16ET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GADD );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CRET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GADD );
		} else {
			g_graphics.PutSpr16CET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GADD );
		}
	}

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}

//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprLight( TSpr *pSpr, int x, int y, int font, BOOL bReverse )
{

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16RAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, 8 );
		} else {
			g_graphics.PutSpr16AT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, 8 );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CRAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, 8 );
		} else {
			g_graphics.PutSpr16CAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, 8 );
		}
	}

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}


//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprLight1( TSpr *pSpr, int x, int y, int font, BOOL bReverse )
{
	if( pSpr == NULL || pSpr->pImage == NULL)		return;
	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16RET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GLIGHT );
		} else {
			g_graphics.PutSpr16ET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GLIGHT );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CRET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GLIGHT );
		} else {
			g_graphics.PutSpr16CET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GLIGHT );
		}
	}	

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}


//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprShadow( TSpr *pSpr, int x, int y, int font, BOOL bReverse )
{

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutAlphaB16T( x, y, xsize, ysize, &pSpr->pImage[ startpos ] );
		} else {
			g_graphics.PutAlphaB16T( x, y, xsize, ysize, &pSpr->pImage[ startpos ] );
		}

	} else {
		/*
		if( bReverse ) {
		g_graphics.PutSpr16CRET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GLIGHT );
		} else {
		g_graphics.PutSpr16CET( x, y, &pSpr->pImage[ startpos ], xsize, ysize, EFFECT_GLIGHT );
		}
		*/
	}	

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}

}


//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprAlpha( TSpr *pSpr, int x, int y, int alpha, int font, BOOL bReverse )
{

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16RAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
		} else {
			g_graphics.PutSpr16AT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CRAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
		} else {
			g_graphics.PutSpr16CAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
		}
	}	

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}

//---------------------------------------------------------------------------------------
void cltGraphic_Capsule::PutSprAlphaRS( TSpr *pSpr, int x, int y, int alpha, int font /* = 0 */, BOOL bReverse /* = FALSE */, FLOAT fRotateDegree /* = 0.0f */, FLOAT fScale /* = 1.0f */ )
{

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			if (fRotateDegree > 0.0f || fScale != 1.0f)	{
				g_graphics.PutSpr16SRAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16RAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
			}
		} else {
			if (fRotateDegree > 0.0f || fScale != 1.0f)	{
				g_graphics.PutSpr16SAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16AT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
			}
		}

	} else {

		if( bReverse ) {
			if (fRotateDegree > 0.0f || fScale != 1.0f)	{	// 기본적으로 클리핑 처리
				g_graphics.PutSpr16SRAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16CRAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
			}
		} else {
			if (fRotateDegree > 0.0f || fScale != 1.0f)	{	// 기본적으로 클리핑 처리
				g_graphics.PutSpr16SAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha , NULL, fRotateDegree, fScale );
			} else {
				g_graphics.PutSpr16CAT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, alpha );
			}
		}
	}	

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}

void cltGraphic_Capsule::PutSprEF( TSpr *pSpr, int x, int y, int effect, int font, BOOL bReverse )
{
	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	if( GP.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) == TRUE )
	{
		if( bReverse ) {
			g_graphics.PutSpr16REFT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, effect );
		} else {
			g_graphics.PutSpr16EFT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, effect );
		}

	} else {

		if( bReverse ) {
			g_graphics.PutSpr16CREFT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, effect );
		} else {
			g_graphics.PutSpr16CEFT( x, y, &pSpr->pImage[ startpos ], xsize, ysize, effect );
		}
	}	

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}

void cltGraphic_Capsule::PutMpr( TSpr *pSpr, int x, int y, int mprmode, BOOL cmmode, int font, int alpha )
{
	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	PutMpr( x, y, pSpr->GetXSize(), pSpr->GetYSize(), &pSpr->pImage[ startpos ], mprmode, cmmode, alpha );

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( 0 );
	}
}

SI32 cltGraphic_Capsule::DrawNumber( TSpr *pSpr, int x, int y, int startfont, int num )
{
	SI32 i;
	SI32 digit[20];
	SI32 digitnum = 0;		// 숫자의 개수. 
	SI32 mod;
	SI32 oldnum;

	bool loop = true;
	while(loop)
	{
		oldnum = num;
		mod = num % 10;
		num = num / 10;

		digit[ digitnum ] = mod;
		digitnum++;
		if( digitnum >= 20 - 1 ) break;

		if( oldnum == mod ) break;
	}

	SI32 index = 0;
	for( i = digitnum - 1; i >= 0; i-- ) 
	{
		PutSpr( pSpr, x + index, y, startfont + digit[i] );
		index += pSpr->GetXSize();
	}

	return x + index;
}

SI32 cltGraphic_Capsule::DrawNumber( TSpr *pSpr, int x, int y, int startfont, int num, int nDigit )
{
	SI32 i;
	SI32 digit[20];
	SI32 digitnum = 0;		// 숫자의 개수. 
	SI32 mod;
	SI32 oldnum;

	ZeroMemory( digit, sizeof(digit) );

	bool loop = true;
	while(loop)
	{
		oldnum = num;
		mod = num % 10;
		num = num / 10;

		digit[ digitnum ] = mod;
		digitnum++;
		if( digitnum >= 20 - 1 ) break;

		if( oldnum == mod ) break;
	}

	// 설정된 자릿수에 따른 변경
	if ( 20 <= nDigit )
	{
		digitnum = 20;
	}
	else
	{
		digitnum = nDigit;
	}

	SI32 index = 0;
	for( i = digitnum - 1; i >= 0; i-- ) 
	{
		PutSpr( pSpr, x + index, y, startfont + digit[i] );
		index += pSpr->GetXSize();
	}

	return x + index;

}

void cltGraphic_Capsule::PutSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image,
								SI16 putspr, SI16 sprmode)
{
	switch(putspr)
	{
	case PUT_SPR_SPR:
		PutSprSpr(x,y,xsize,ysize,image,sprmode);
		break;
	case PUT_SPR_SHADOW:
		PutSprShadow(x,y,xsize,ysize,image,sprmode);
		break;
	case PUT_SPR_LIGHT:
		PutSprLight(x,y,xsize,ysize,image,sprmode);
		break;
	case PUT_SPR_LIGHT1:
		PutSprLight1(x,y,xsize,ysize,image,sprmode);
		break;
	case PUT_SPR_ADD:
		PutSprAdd(x,y,xsize,ysize,image,sprmode);
		break;
	case PUT_SPR_SCREEN:
		PutSprScreen(x,y,xsize,ysize,image,sprmode);
		break;
	case PUT_SPR_MULTIPLY:
		PutSprMultiply(x,y,xsize,ysize,image,sprmode);
		break;
	default: return ;
	}
}

void cltGraphic_Capsule::PutSprSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutSpr16T( x, y, image, xsize, ysize );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CT( x, y, image, xsize, ysize );
		break;
	case SPR_MODE_RT:
		g_graphics.PutSpr16RT( x, y, image, xsize, ysize );
		break;
	case SPR_MODE_CRT:
		g_graphics.PutSpr16CRT( x, y, image, xsize, ysize );
		break;
	default: return ;
	}

}

void cltGraphic_Capsule::PutSprShadow(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutAlphaB16T( x, y, xsize, ysize, image );
		break;
	case SPR_MODE_CT:
		//g_graphics.PutSpr16CET( x, y, image, xsize, ysize, 8 );
		break;
	case SPR_MODE_RT:
		g_graphics.PutAlphaB16T( x, y, xsize, ysize, image );
		break;
	case SPR_MODE_CRT:
		//g_graphics.PutSpr16CRET( x, y, image, xsize, ysize, 8 );
		break;
	default: return ;
	}

}

void cltGraphic_Capsule::PutSprLight(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{


	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutSpr16AT( x, y, image, xsize, ysize, 8 );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CAT( x, y, image, xsize, ysize, 8 );
		break;
	case SPR_MODE_RT:
		g_graphics.PutSpr16RAT( x, y, image, xsize, ysize, 8 );
		break;
	case SPR_MODE_CRT:
		g_graphics.PutSpr16CRAT( x, y, image, xsize, ysize, 8 );
		break;
	default: return ;
	}

}

void cltGraphic_Capsule::PutSprLight1(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutSpr16ET( x, y, image, xsize, ysize, EFFECT_GLIGHT );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CET( x, y, image, xsize, ysize, EFFECT_GLIGHT );
		break;
	case SPR_MODE_RT:
		g_graphics.PutSpr16RET( x, y, image, xsize, ysize, EFFECT_GLIGHT );
		break;
	case SPR_MODE_CRT:
		g_graphics.PutSpr16CRET( x, y, image, xsize, ysize, EFFECT_GLIGHT );
		break;
	default: return ;
	}

}

void cltGraphic_Capsule::PutSprAdd(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutSpr16ET( x, y, image, xsize, ysize, EFFECT_GADD );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CET( x, y, image, xsize, ysize, EFFECT_GADD );
		break;
	case SPR_MODE_RT:
		g_graphics.PutSpr16RET( x, y, image, xsize, ysize, EFFECT_GADD );
		break;
	case SPR_MODE_CRT:
		g_graphics.PutSpr16CRET( x, y, image, xsize, ysize, EFFECT_GADD );
		break;
	default: return ;
	}


}

void cltGraphic_Capsule::PutSprScreen(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{


	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutSpr16ET( x, y, image, xsize, ysize, EFFECT_GSCREEN );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CET( x, y, image, xsize, ysize, EFFECT_GSCREEN );
		break;
	case SPR_MODE_RT:
		g_graphics.PutSpr16RET( x, y, image, xsize, ysize, EFFECT_GSCREEN );
		break;
	case SPR_MODE_CRT:
		g_graphics.PutSpr16CRET( x, y, image, xsize, ysize, EFFECT_GSCREEN );
		break;
	default: return ;
	}


}

void cltGraphic_Capsule::PutSprMultiply(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{


	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutSpr16ET( x, y, image, xsize, ysize, EFFECT_GMULTIPLY );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CET( x, y, image, xsize, ysize, EFFECT_GMULTIPLY );
		break;
	case SPR_MODE_RT:
		g_graphics.PutSpr16RET( x, y, image, xsize, ysize, EFFECT_GMULTIPLY );
		break;
	case SPR_MODE_CRT:
		g_graphics.PutSpr16CRET( x, y, image, xsize, ysize, EFFECT_GMULTIPLY );
		break;
	default: return ;
	}

}

void cltGraphic_Capsule::ReleaseGP()
{
}

void cltGraphic_Capsule::SetClipFullScreen()
{
	clGrp.SetClipFullScreen();
	g_graphics.SetClipFullScreen();

}


BOOL cltGraphic_Capsule::CreateSurface(LPDIRECTDRAWSURFACE7 & pSurface, int xsize, int ysize, int colorkeyswitch)
{


	return g_graphics.CreateSurface16( &pSurface, xsize, ysize );

}


void cltGraphic_Capsule::FillBox(int x1, int y1, int x2, int y2, unsigned char color,BOOL b32mode)
{


	RGB16 c = *((RGB16 *)g_graphics.m_pCurrentColorTablePtr + color);

	g_graphics.FillBox16C( x1, y1, x2, y2, c );

}

void cltGraphic_Capsule::Box(int x1, int y1, int x2, int y2, unsigned char color)
{


	RGB16 c = *((RGB16 *)g_graphics.m_pCurrentColorTablePtr + color);

	g_graphics.Box16C( x1, y1, x2, y2, c );


}

void cltGraphic_Capsule::Line(int x1, int y1, int x2, int y2, unsigned char color,BOOL bx2mode)
{

	RGB16 c = *((RGB16 *)g_graphics.m_pCurrentColorTablePtr + color);

	g_graphics.Line16( x1, y1, x2, y2, c );

}

void cltGraphic_Capsule::GetSprRandPos8(int x, int y, int xsize, int ysize,
										unsigned char *image, SI32* pgetnum, POINT* ppoint,BOOL bRTmode)
{
	if ( bRTmode )
		clGrp.GetSprRandPos8RT(x,y,xsize,ysize,image,pgetnum,ppoint);
	else
		clGrp.GetSprRandPos8T(x,y,xsize,ysize,image,pgetnum,ppoint);
}

void cltGraphic_Capsule::GetSprRandPos8H(int x, int y, int xsize, int ysize,
										 unsigned char *image, SI32* pgetnum, POINT* ppoint,BOOL bRTmode)
{
	if ( bRTmode )
		clGrp.GetSprRandPos8RTH(x,y,xsize,ysize,image,pgetnum,ppoint);
	else
		clGrp.GetSprRandPos8TH(x,y,xsize,ysize,image,pgetnum,ppoint);
}


void cltGraphic_Capsule::PutMpr(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image,
								SI16 mprmode,BOOL bCMmode,SI32 alphavalue)
{
//	if( NULL == image )
//		return;

	switch(mprmode)
	{
	case PUT_SPR_SPR:
		{
			if ( bCMmode ) {
				g_graphics.PutMpr16CT(x,y,image, xsize,ysize );
			} else {
				g_graphics.PutMpr16T(x,y,image, xsize,ysize );
			}
		}
		break;
	case PUT_SPR_SCREEN:
		{
			if ( bCMmode ) {
				g_graphics.PutMpr16CET(x, y, image, xsize,ysize, EFFECT_GSCREEN );
			} else {
				g_graphics.PutMpr16ET(x, y, image, xsize,ysize, EFFECT_GSCREEN );
			}
		}
		break;
	case PUT_SPR_MULTIPLY:
		{
			if ( bCMmode ) {
				g_graphics.PutMpr16CET(x,y,image, xsize,ysize, EFFECT_GMULTIPLY );
			} else {
				g_graphics.PutMpr16ET(x,y,image, xsize,ysize, EFFECT_GMULTIPLY );
			}
		}
		break;
	case PUT_SPR_ALPHA:
		{
			if ( bCMmode ) {
				g_graphics.PutMpr16CAT(x,y,image, xsize,ysize, alphavalue );
			} else {
				g_graphics.PutMpr16AT(x,y,image, xsize,ysize, alphavalue );
			}
		}
	default: return ;
	}


}

void cltGraphic_Capsule::PutXpr16( SI32 x, SI32 y, SI32 xsize, SI32 ysize, RGB16* image, RGB16 colorkey  )
{
//	if( NULL == image )
//		return;

	if( g_graphics.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) ) {
		g_graphics.PutXpr16T( x, y, xsize, ysize, image, colorkey );
	} else {
		g_graphics.PutXpr16CT( x, y, xsize, ysize, image, colorkey );
	}

	return;
}

void cltGraphic_Capsule::PutXpr16A( SI32 x, SI32 y, SI32 xsize, SI32 ysize, RGB16* image, int alpha, RGB16 colorkey )
{
//	if( NULL == image )
//		return;

	if( g_graphics.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) ) {
		g_graphics.PutXpr16AT( x, y, xsize, ysize, image, alpha, colorkey );
	} else {
		g_graphics.PutXpr16CAT( x, y, xsize, ysize, image, alpha, colorkey );
	}

	return;
}

void cltGraphic_Capsule::PutSpr16EF( SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, int effect, BOOL bReverse )
{
//	if( NULL == image )
//		return;

	if( g_graphics.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) ) {

		if( bReverse ) {
			g_graphics.PutSpr16REFT( x, y, image, xsize, ysize, effect );
		} else {
			g_graphics.PutSpr16EFT( x, y, image, xsize, ysize, effect );
		}
	} else {
		if( bReverse ) {
			g_graphics.PutSpr16CREFT( x, y, image, xsize, ysize, effect );
		} else {
			g_graphics.PutSpr16CEFT( x, y, image, xsize, ysize, effect );
		}
	}

	return;
}

BYTE cltGraphic_Capsule::GetSprColor( TSpr *pSpr, SI32 x, SI32 y, SI32 font, bool bReverse )
{
	if( pSpr->header.iVersion == TSPR_VERSION_16PAL ) {
		g_graphics.SetCurrentColorTable( pSpr->pal );
	} else {
		g_graphics.SetCurrentColorTable( 0 );
	}

	int startpos = pSpr->header.ImageInfo[ font ].iStartPos;

	int xsize = pSpr->header.iXSize;
	int ysize = pSpr->header.iYSize;

	BYTE btRet = COLOR_TRANS;

	if( bReverse )
	{
		btRet = g_graphics.GetSpr16RT( x, y, xsize, ysize, &pSpr->pImage[ startpos ] );
	}
	else 
	{
		btRet = g_graphics.GetSpr16T( x, y, xsize, ysize, &pSpr->pImage[ startpos ] );
	}

	if( pSpr->header.iVersion == TSPR_VERSION_16PAL )
	{
		g_graphics.SetCurrentColorTable( 0 );
	}

	return btRet;
}

int cltGraphic_Capsule::GetScreenMode()
{
	return clGrp.GetScreenMode();
}

int cltGraphic_Capsule::GetScreenXsize()
{
	return clGrp.GetScreenXsize();
}

int cltGraphic_Capsule::GetScreenYsize()
{
	return clGrp.GetScreenYsize();
}

void cltGraphic_Capsule::SetScreenXsize(int xsize)
{
	clGrp.SetScreenXsize(xsize);
}

void cltGraphic_Capsule::SetScreenYsize(int ysize)
{
	clGrp.SetScreenYsize(ysize);
}

int cltGraphic_Capsule::GetColorDepth()
{
	return clGrp.GetColorDepth();
}

int cltGraphic_Capsule::GetSurfaceWidth()
{
	return clGrp.GetSurfaceWidth();
}

void cltGraphic_Capsule::Restore()
{

	g_graphics.m_pddsBackBuffer->Restore();
	g_graphics.m_pddsFrontBuffer->Restore();

}

void cltGraphic_Capsule::RestoreDisplayToWindow()
{
	g_graphics.m_pDD->RestoreDisplayMode();
}

void cltGraphic_Capsule::SetDisplayToGameMode()
{
	if( g_graphics.m_width > 0 && g_graphics.m_height > 0 && g_graphics.m_dwBPP > 0 )
	{
		g_graphics.m_pDD->SetDisplayMode( g_graphics.m_width, g_graphics.m_height, g_graphics.m_dwBPP, 0, 0 );
	}
}

void cltGraphic_Capsule::UpdateScreen(HWND hwnd, LPDIRECTDRAWSURFACE7 pSurface, SI32 xsize, SI32 ysize,
									  SI32 xindent, SI32 yindent, SI32 srcstartx, SI32 srcstarty , bool bwaitrefresh,
									  SI32 screenXSize, SI32 screenYSize , bool bWebDlg, HWND hWebDlg,
									  bool bNewFullScreen )
{

	clGrp.UpdateScreen(hwnd,pSurface,xsize,ysize,xindent,yindent,srcstartx,srcstarty,bwaitrefresh,screenXSize,screenYSize, bWebDlg, hWebDlg, bNewFullScreen );


}

BOOL cltGraphic_Capsule::FreeSpr( TSpr& spr )
{
	if( spr.pImage ) {
		delete [] spr.pImage;
		ZeroMemory( &spr, sizeof( TSpr ) );
	}

	return TRUE;
}

BOOL cltGraphic_Capsule::LoadSprFromFile( TCHAR *filename, TSpr *pSpr )
{
	return pSpr->LoadTSprFromFile( filename, g_graphics.m_rgbmode );
}

BOOL cltGraphic_Capsule::LoadSprFromMemory( BYTE* pMemory, TSpr *pSpr )
{
	return pSpr->LoadTSprFromMemory( pMemory, g_graphics.m_rgbmode );
}

BOOL cltGraphic_Capsule::LoadTileFromFile( TCHAR *filename, TSpr *pSpr )
{
	return pSpr->LoadTSprFromFile( filename, g_graphics.m_rgbmode );
}

BOOL cltGraphic_Capsule::EncodeSpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey)
{
	int i, j;
	UI08 color;
	int whitenumber = 0;
	int length = 0;


	for(i=0;i < ysize;i++)
	{
		for(j=0;j<xsize;j++)
		{
			color=src[j+i*xsize];

			if(color==colorkey)
			{
				whitenumber++;
				// 가로 축의 끝에서 나오면,
				if(j==(xsize-1))
				{
					dest[length] = COLOR_TRANS;
					length++;
					dest[length]=whitenumber;
					length++;
					whitenumber=0;
				}
				// 투명색이 255개가 되면, 
				else if(whitenumber>=255)
				{
					dest[length]=COLOR_TRANS;
					length++;
					dest[length]=whitenumber;
					length++;
					whitenumber=0;
				}

			}
			else
			{
				if(whitenumber)
				{
					dest[length]=COLOR_TRANS;
					length++;
					dest[length]=whitenumber;
					length++;
					whitenumber=0;
				}
				dest[length]=color;
				length++;
			}
		}
	}

	return TRUE;
}

BOOL cltGraphic_Capsule::EncodeMpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey)
{
	int i, j;
	UI08 color;
	int whitenumber = 0;
	int length = 0;
	int colornumber=0;
	int colornumberpos = -1;


	for(i=0;i < ysize;i++)
	{
		for(j=0;j<xsize;j++)
		{
			color=src[j+i*xsize];

			if(color==colorkey)
			{
				whitenumber++;

				if ( colornumberpos > -1 )
				{
					dest[ colornumberpos ] = colornumber;
					colornumber = 0;
					colornumberpos = -1;
				}

				// 가로 축의 끝에서 나오면,
				if(j==(xsize-1))
				{
					dest[length] = COLOR_TRANS;
					length++;
					dest[length]=whitenumber;
					length++;
					whitenumber=0;
				}
				// 투명색이 253개가 되면, 
				else if(whitenumber>=253)
				{
					dest[length]=COLOR_TRANS;
					length++;
					dest[length]=whitenumber;
					length++;
					whitenumber=0;
				}

			}
			else
			{
				++colornumber;

				if ( colornumberpos < 0 )
				{
					if(whitenumber)
					{
						dest[length]=COLOR_TRANS;
						length++;
						dest[length]=whitenumber;
						length++;
						whitenumber=0;
					}

					colornumberpos = length;
					++length;
				}

				// 가로 축의 끝에서 나오면,
				if( j == ( xsize - 1 ) )
				{
					dest[ colornumberpos ] = colornumber;
					dest[length] = color;
					++length;

					colornumber = 0;
					colornumberpos = -1;
				}
				// 색이 253개가 되면, 
				else if( colornumber>=253 )
				{
					dest[ colornumberpos ] = colornumber;
					dest[length] = color;
					++length;

					colornumber = 0;
					colornumberpos = -1;
				}
				else
				{
					dest[length]=color;
					++length;
				}
			}
		}
	}

	return TRUE;

}

int cltGraphic_Capsule::SavePCX(PCX & pcx, TCHAR *filename)
{
	/*
	FILE *fp;
	int i, j;

	fp=_tfopen(filename, TEXT("wb"));
	if(fp==NULL) return 0;

	pcx.Head.maker= 10;
	pcx.Head.ver  =  5;
	pcx.Head.encode= 1;
	pcx.Head.bpp   = 8;
	pcx.Head.xmin  = 0;
	pcx.Head.ymin  = 0;

	for(i=0;i<48;i++)
		pcx.Head.palette[i]=0;

	pcx.Head.vmode   = 0;
	pcx.Head.nplanes = 1;
	pcx.Head.palette_type = 0;

	for(i=0;i<54;i++)
		pcx.Head.unused[i]=0;

	fwrite(&pcx.Head,sizeof(_head),1,fp);	

	unsigned char da1, color, count=0;
	int xsize=pcx.Head.hres;
	int ysize=pcx.Head.vres;
	int color_switch=FALSE;
	int write_switch=FALSE;

	for(i=0;i<ysize;i++)
		for(j=0;j<xsize;j++)
		{
			da1=pcx.Image[j+i*xsize];

			if(color_switch==FALSE)
		 {
			 color=da1;
			 count=1;
			 color_switch=TRUE;
			 if(j==(xsize-1))
			 {
				 record(count, color, fp);
				 color_switch=FALSE;
			 }
		 }
			else if(color==da1)
		 {
			 count++;
			 if(j==(xsize-1)){
				 record(count, color, fp);
				 color_switch=FALSE;
			 }
			 else if(count>=63){
				 record(count, color, fp);
				 color_switch=FALSE;
			 }
		 }
			else 
		 {
			 record(count, color, fp);
			 color=da1;
			 count=1;
			 color_switch=TRUE;
			 if(j==(xsize-1))
			 {
				 record(count, color, fp);
				 color_switch=FALSE;
			 }
		 }
		}

		pcx.Tail.identification=12; // always;

		for(i=0;i<256;i++)
		{
			pcx.Tail.palette[i*3]   = PalEntries[i].peRed;
			pcx.Tail.palette[i*3+1] = PalEntries[i].peGreen;
			pcx.Tail.palette[i*3+2] = PalEntries[i].peBlue;
		}

		fwrite(&pcx.Tail,sizeof(_tail),1,fp);

		fclose(fp);
		*/
			return 1;
}

void cltGraphic_Capsule::record(int count, unsigned char color, FILE* fp)
{
	if(count>1 || color>=192)
	{
		fputc(192+count, fp);
		fputc(color, fp);
	}
	else
	{
		fputc(color, fp);
	}
}


