//---------------------------------
// 2004/11/15 성준엽
//---------------------------------

#include "../NGraphics/Graphics.h"

#include "../EtcTool/EtcTool.h"
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
								SI32 xsize , SI32 ysize, BOOL bFullScreen, SI32 screenXSize, SI32 screenYSize )
{

	if( screenXSize == 0 || screenYSize == 0 ) {
		screenXSize = xsize;
		screenYSize = ysize;
	}


#ifdef GRA_MODE_16

	if( bFullScreen ) {

		if( g_graphics.CreateFullScreenGraphics( hwnd, screenXSize, screenYSize, 16, xsize, ysize ) ) {

			lpbackscreen = g_graphics.GetBackBuffer();

			clGrp.lpDD7 = g_graphics.GetDirectDraw();
			clGrp.lpDDSPrimary = g_graphics.GetFrontBuffer();
			clGrp.lpDDSBack = g_graphics.GetBackBuffer();
			clGrp.lpDDSConvertSurface = g_graphics.GetConvertBuffer();

			clGrp.ScreenXsize = xsize;
			clGrp.ScreenYsize = ysize;
			clGrp.ColorDepth = 8;

			LoadPalette();
			clGrp.SetAllPalette();

			clGrp.SetColorTable();

			g_graphics.CreateAlphaTable();
			g_graphics.CreateEffectTable();
			g_graphics.CreateEffectFTable();

			g_graphics.CreateColorTable( 1 );
			g_graphics.LoadColorTableFromFile( "pal\\Monarch.pal" );

		} else {

			MessageBox(NULL, "Display Error", "Error", MB_OK|MB_TOPMOST);
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

			LoadPalette();
			clGrp.SetAllPalette();

			clGrp.SetColorTable();

			g_graphics.CreateAlphaTable();
			g_graphics.CreateEffectTable();
			g_graphics.CreateEffectFTable();

			g_graphics.CreateColorTable( 1 );
			g_graphics.LoadColorTableFromFile( "pal\\Monarch.pal" );

		} else {

			MessageBox(NULL, "Display Error", "Error", MB_OK|MB_TOPMOST);
			return FALSE;
		}	
	}

#else
/*
	if( g_graphics.CreateWindowedGraphics( hwnd, xsize, ysize, 8 ) ) {
		
		lpbackscreen = g_graphics.GetBackBuffer();

		clGrp.lpDD7 = g_graphics.GetDirectDraw();
		clGrp.lpDDSPrimary = g_graphics.GetFrontBuffer();
		clGrp.lpDDSBack = g_graphics.GetBackBuffer();
		clGrp.lpDDSConvertSurface = g_graphics.GetConvertBuffer();

		clGrp.ScreenXsize = xsize;
		clGrp.ScreenYsize = ysize;
		clGrp.ColorDepth = 8;

		LoadPalette();
		clGrp.SetAllPalette();

		clGrp.SetColorTable();

	} else {

	    MessageBox(NULL, "Display Error", "Error", MB_OK|MB_TOPMOST);
		return FALSE;
	}	
*/

	if(clGrp.CreateDDraw(hwnd, screenmode, true, true/*, DDSCL_NORMAL*/) == TRUE)
	{
		LoadPalette();
		clGrp.SetAllPalette();

		if(clGrp.CreateOffScreen(lpbackscreen, screenXSize, screenYSize, FALSE) == FALSE)
		{
			MsgBox("CreateSurface", "lpBackScreen");
		}

		// 16비트 팔레트에 값을 넣는다. 
		clGrp.SetColorTable();


	}
	else
	{
	    MessageBox(NULL, "Display Error", "Error", MB_OK|MB_TOPMOST);
		return FALSE;
	}

#endif

	SetGamePalette(clGrp.Palette);

	// 클리핑 영역을 화면 전체로 설정한다. 
	clGrp.SetClipFullScreen();

	return TRUE ;
}

void cltGraphic_Capsule::PutSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image,
								SI16 putspr, SI16 sprmode)
{
	switch(putspr)
	{
	case PUT_SPR_IMG:
		PutSprImg(x,y,xsize,ysize,image,sprmode);
		break;
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

void cltGraphic_Capsule::PutSprTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, UI08 *ctable,
								SI16 putsprtable,  SI16 sprmode)
{
	switch(putsprtable)
	{
	case PUT_SPRTABLE_IMG:
		PutSprImgTable(x,y,xsize,ysize,image,ctable,sprmode);
		break;
	case PUT_SPRTABLE_DOT:
		PutSprDotTable(x,y,xsize,ysize,image,ctable,sprmode);
		break;
	case PUT_SPRTABLE_SPR:
		PutSprSprTable(x,y,xsize,ysize,image,ctable,sprmode);
		break;
	default: return ;
	}
}

// PUTSPR
void cltGraphic_Capsule::PutSprImg(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16
	switch(sprmode)
	{
	case SPR_MODE_T:
		g_graphics.PutSpr16T(x,y,image,xsize,ysize );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CT(x,y,image,xsize,ysize );
		break;	
	default: return;
	}


#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutImg8T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutImg8CT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif
}

void cltGraphic_Capsule::PutSprSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16

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


#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSpr8T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutSpr8CT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_RT:
		clGrp.PutSpr8RT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSpr8CRT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif

}

void cltGraphic_Capsule::PutSprShadow(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16

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

#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSprShadow8T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutSprShadow8CT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_RT:
		clGrp.PutSprShadow8RT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSprShadow8CRT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif
}

void cltGraphic_Capsule::PutSprLight(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16

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

#else
	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSprLight8T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutSprLight8CT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_RT:
		clGrp.PutSprLight8RT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSprLight8CRT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif
}

void cltGraphic_Capsule::PutSprLight1(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16

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

#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSprLight18T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutSprLight18CT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_RT:
		clGrp.PutSprLight18RT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSprLight18CRT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif
}

void cltGraphic_Capsule::PutSprAdd(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16

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

#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSprAdd8T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutSprAdd8CT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_RT:
		clGrp.PutSprAdd8RT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSprAdd8CRT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif

}

void cltGraphic_Capsule::PutSprScreen(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16

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

#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSprScreen8T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutSprScreen8CT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_RT:
		clGrp.PutSprScreen8RT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSprScreen8CRT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif

}

void cltGraphic_Capsule::PutSprMultiply(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode )
{

#ifdef GRA_MODE_16
/*
	switch(sprmode)
	{

	case SPR_MODE_T:
		g_graphics.PutSpr16AT( x, y, image, xsize, ysize, 4 );
		break;
	case SPR_MODE_CT:
		g_graphics.PutSpr16CAT( x, y, image, xsize, ysize, 4 );
		break;
	case SPR_MODE_RT:
		g_graphics.PutSpr16RAT( x, y, image, xsize, ysize, 4 );
		break;
	case SPR_MODE_CRT:
		g_graphics.PutSpr16CRAT( x, y, image, xsize, ysize, 4 );
		break;
	default: return ;
	}
*/
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


#else


	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSprMultiply8T(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CT:
		clGrp.PutSprMultiply8CT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_RT:
		clGrp.PutSprMultiply8RT(x,y,xsize,ysize,image);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSprMultiply8CRT(x,y,xsize,ysize,image);
		break;
	default: return ;
	}

#endif

}

// PUTSPR TABLE
void cltGraphic_Capsule::PutSprImgTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, 
										UI08 *ctable, SI16 sprmode )
{

#ifdef GRA_MODE_16

#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutImg8T(x,y,xsize,ysize,image,ctable);
		break;
	default: return ;
	}

#endif

}

void cltGraphic_Capsule::PutSprDotTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, 
										UI08 *ctable, SI16 sprmode )
{

#ifdef GRA_MODE_16

#else

	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSprDot8T(x,y,xsize,ysize,image,ctable);
		break;
	case SPR_MODE_CT:
		clGrp.PutSprDot8CT(x,y,xsize,ysize,image,ctable);
		break;
	case SPR_MODE_RT:
		clGrp.PutSprDot8RT(x,y,xsize,ysize,image,ctable);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSprDot8CRT(x,y,xsize,ysize,image,ctable);
		break;
	default: return ;
	}

#endif

}

void cltGraphic_Capsule::PutSprSprTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, 
										UI08 *ctable, SI16 sprmode )
{

#ifdef GRA_MODE_16

#else
	
	switch(sprmode)
	{
	case SPR_MODE_T:
		clGrp.PutSpr8T(x,y,xsize,ysize,image,ctable);
		break;
	case SPR_MODE_CT:
		clGrp.PutSpr8CT(x,y,xsize,ysize,image,ctable);
		break;
	case SPR_MODE_RT:
		clGrp.PutSpr8RT(x,y,xsize,ysize,image,ctable);
		break;
	case SPR_MODE_CRT:
		clGrp.PutSpr8CRT(x,y,xsize,ysize,image,ctable);
		break;
	default: return ;
	}

#endif

}



void cltGraphic_Capsule::ReleaseGP()
{
	clGrp.ReleaseDirectDraw();
}

void cltGraphic_Capsule::LoadPalette()
{
	unsigned char temppal[768];

	SHORT i;

	if(clGrp.LoadOnlyPaletteFile("pal\\Monarch.pal", temppal)==FALSE)
	{
	     MsgBox("LoadPal", "3245");
	}

	for(i=0;i<768;i++)
	{
		clGrp.Palette[i]=temppal[i];
	}

	clGrp.Palette[255*3]  =63;
	clGrp.Palette[255*3+1]=63;
    clGrp.Palette[255*3+2]=63;
}

void cltGraphic_Capsule::SetGamePalette(const unsigned char *pal)
{
    for(SI32 i=0;i<256;i++)
	{
	      clGrp.PalEntries[i].peRed=  pal[i*3]*4;
	      clGrp.PalEntries[i].peGreen=pal[i*3+1]*4;
	      clGrp.PalEntries[i].peBlue= pal[i*3+2]*4;
	} 

	clGrp.SetPaletteEntries(0, 256, clGrp.PalEntries);

	// 현재의 팔레트를 복사해 둔다.
	memcpy(clGrp.CurrentPal, pal, 768);
}

void cltGraphic_Capsule::SetClipFullScreen()
{
	clGrp.SetClipFullScreen();
	g_graphics.SetClipFullScreen();

}

BOOL cltGraphic_Capsule::CreateOffScreen8(LPDIRECTDRAWSURFACE7 & pSurface, int xsize, int ysize, int colorkeyswitch)
{
	return clGrp.CreateOffScreen( pSurface, xsize, ysize ,colorkeyswitch) ;
}

BOOL cltGraphic_Capsule::CreateSurface(LPDIRECTDRAWSURFACE7 & pSurface, int xsize, int ysize, int colorkeyswitch)
{
#ifdef GRA_MODE_16

	return g_graphics.CreateSurface16( &pSurface, xsize, ysize );
#else
	return clGrp.CreateOffScreen( pSurface, xsize, ysize ,colorkeyswitch) ;
#endif
}

UI08* cltGraphic_Capsule::GetPalette8(SI32 mode)
{
	switch(mode)
	{
	case PAL_MODE_PAL:
		return clGrp.Palette ;
		break;
	case PAL_MODE_CURRENT:
		return clGrp.CurrentPal ;
		break;
	case PAL_MODE_ORG:
		return clGrp.OriginalPal ;
		break;
	default: return NULL ;
	}
}



void cltGraphic_Capsule::PutSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image)
{
	clGrp.PutSpr(x,y,xsize,ysize,image);
}

void cltGraphic_Capsule::PutSpr8(int x,int y,int xsize,int ysize,unsigned char *image,BOOL bCmode)
{
	if ( bCmode )
		clGrp.PutSpr8C(x,y,xsize,ysize,image);
	else
		clGrp.PutSpr8(x,y,xsize,ysize,image);
}
/*
// 서피스를 락한다. 
// surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
BOOL cltGraphic_Capsule::LockSurface(int surface)
{
	return LockSurface(surface);
}

// 서피스를 언락한다. 
// surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
BOOL cltGraphic_Capsule::UnlockSurface(int surface)
{
	return clGrp.UnlockSurface(surface);
}
*/



void cltGraphic_Capsule::FillScreen(unsigned char color)
{
#ifdef GRA_MODE_16
#else
	clGrp.FillScreen(color);
#endif
}

void cltGraphic_Capsule::PutImage(int x,int y,int xsize,int ysize, unsigned char *image,BOOL b32mode)
{
	if ( b32mode )
		clGrp.PutImage32(x,y,xsize,ysize,image);
	else
		clGrp.PutImage(x,y,xsize,ysize,image);
}

void cltGraphic_Capsule::FillBox(int x1, int y1, int x2, int y2, unsigned char color,BOOL b32mode)
{
#ifdef GRA_MODE_16

	RGB16 c = *((RGB16 *)g_graphics.m_pCurrentColorTablePtr + color);

	g_graphics.FillBox16C( x1, y1, x2, y2, c );


#else

	if ( b32mode )
		clGrp.FillBox32(x1,y1,x2,y2,color);
	else
		clGrp.FillBox(x1,y1,x2,y2,color);

#endif
}

void cltGraphic_Capsule::Box(int x1, int y1, int x2, int y2, unsigned char color)
{
#ifdef GRA_MODE_16
	
	RGB16 c = *((RGB16 *)g_graphics.m_pCurrentColorTablePtr + color);
	
	g_graphics.Box16C( x1, y1, x2, y2, c );

#else

	clGrp.Box(x1,y1,x2,y2,color);

#endif
}

void cltGraphic_Capsule::Line(int x1, int y1, int x2, int y2, unsigned char color,BOOL bx2mode)
{
#ifdef GRA_MODE_16
	RGB16 c = *((RGB16 *)g_graphics.m_pCurrentColorTablePtr + color);

	g_graphics.Line16( x1, y1, x2, y2, c );
#else
	if ( bx2mode )
		clGrp.Linex2(x1,y1,x2,y2,color);
	else
		clGrp.Line(x1,y1,x2,y2,color);
#endif
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

void cltGraphic_Capsule::PutMpr8(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image,
								SI16 mprmode,BOOL bCMmode)
{
	switch(mprmode)
	{
	case PUT_SPR_SPR:
		{
			if ( bCMmode )
				clGrp.PutSpr8CM(x,y,xsize,ysize,image);
			else
				clGrp.PutSpr8M(x,y,xsize,ysize,image);
		}
		break;
	case PUT_SPR_SCREEN:
		{
			if ( bCMmode )
				clGrp.PutSprScreen8CM(x,y,xsize,ysize,image);
			else
				clGrp.PutSprScreen8M(x,y,xsize,ysize,image);
		}
		break;
	case PUT_SPR_MULTIPLY:
		{
			if ( bCMmode )
				clGrp.PutSprMultiply8CM(x,y,xsize,ysize,image);
			else
				clGrp.PutSprMultiply8M(x,y,xsize,ysize,image);
		}
		break;
	default: return ;
	}
}

void cltGraphic_Capsule::PutMpr(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image,
								SI16 mprmode,BOOL bCMmode,SI32 alphavalue)
{

#ifdef GRA_MODE_16

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

#else

	switch(mprmode)
	{
	case PUT_SPR_SPR:
		{
			if ( bCMmode )
				clGrp.PutSpr8CM(x,y,xsize,ysize,image);
			else
				clGrp.PutSpr8M(x,y,xsize,ysize,image);
		}
		break;
	case PUT_SPR_SCREEN:
		{
			if ( bCMmode )
				clGrp.PutSprScreen8CM(x,y,xsize,ysize,image);
			else
				clGrp.PutSprScreen8M(x,y,xsize,ysize,image);
		}
		break;
	case PUT_SPR_MULTIPLY:
		{
			if ( bCMmode )
				clGrp.PutSprMultiply8CM(x,y,xsize,ysize,image);
			else
				clGrp.PutSprMultiply8M(x,y,xsize,ysize,image);
		}
		break;
	default: return ;
	}

#endif

}

void cltGraphic_Capsule::PutXpr16( SI32 x, SI32 y, SI32 xsize, SI32 ysize, RGB16* image, RGB16 colorkey  )
{
#ifdef GRA_MODE_16
	if( g_graphics.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) ) {
		g_graphics.PutXpr16T( x, y, xsize, ysize, image, colorkey );
	} else {
		g_graphics.PutXpr16CT( x, y, xsize, ysize, image, colorkey );
	}
#endif

	return;
}

void cltGraphic_Capsule::PutXpr16A( SI32 x, SI32 y, SI32 xsize, SI32 ysize, RGB16* image, int alpha, RGB16 colorkey )
{
#ifdef GRA_MODE_16
	if( g_graphics.IsInClipArea( x, y, x + xsize - 1, y + ysize - 1 ) ) {
		g_graphics.PutXpr16AT( x, y, xsize, ysize, image, alpha, colorkey );
	} else {
		g_graphics.PutXpr16CAT( x, y, xsize, ysize, image, alpha, colorkey );
	}
#endif
	return;
}

void cltGraphic_Capsule::PutSpr16EF( SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, int effect, BOOL bReverse )
{
#ifdef GRA_MODE_16
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
#endif
	return;
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
#ifdef GRA_MODE_16
	g_graphics.m_pddsBackBuffer->Restore();
	g_graphics.m_pddsFrontBuffer->Restore();
#else
	clGrp.Restore();
#endif
}

void cltGraphic_Capsule::UpdateScreen(HWND hwnd, LPDIRECTDRAWSURFACE7 pSurface, SI32 xsize, SI32 ysize,
									  SI32 xindent, SI32 yindent, SI32 srcstartx, SI32 srcstarty , bool bwaitrefresh,
									  SI32 screenXSize, SI32 screenYSize )
{
#ifdef GRA_MODE_16
	clGrp.UpdateScreen(hwnd,pSurface,xsize,ysize,xindent,yindent,srcstartx,srcstarty,bwaitrefresh,screenXSize,screenYSize);
#else
	clGrp.UpdateScreen(hwnd,pSurface,xsize,ysize,xindent,yindent,srcstartx,srcstarty,bwaitrefresh,screenXSize,screenYSize);
#endif

}

BOOL cltGraphic_Capsule::LoadSpr(char* szfilename, cltTSpr* pspr)
{
	return clGrp.LoadSpr(szfilename,pspr);
}
BOOL cltGraphic_Capsule::LoadTile(char* szfilename, cltTSpr* pspr)
{
	BOOL ret;

#ifdef GRA_MODE_16
	ret = clGrp.LoadSpr(szfilename,pspr);
	//ret = clGrp.LoadMprToXpr(szfilename, pspr);
#else
	ret = clGrp.LoadSpr(szfilename,pspr);
#endif

	return ret;
}

BOOL cltGraphic_Capsule::FreeSpr(cltTSpr& xspr)
{
	return clGrp.FreeSpr(xspr);
}

BOOL cltGraphic_Capsule::EncodeSpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey)
{
	return clGrp.EncodeSpr(dest,src,xsize,ysize,colorkey);
}

BOOL cltGraphic_Capsule::EncodeMpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey)
{
	return clGrp.EncodeMpr(dest,src,xsize,ysize,colorkey);
}

BOOL cltGraphic_Capsule::SaveMpr(CHAR* filename, cltTSpr& spr, UI08 *image)
{
	return clGrp.SaveMpr(filename,spr,image);
}


// 생성된 lpddPal객체를 가지고 팔레트를 설정한다. 
HRESULT cltGraphic_Capsule::SetPalette()
{
	return clGrp.SetPalette();
}

BOOL cltGraphic_Capsule::SetPalette(unsigned char* pal)
{
	return clGrp.SetPalette(pal);
}

int cltGraphic_Capsule::SavePCX(PCX & pcx, char *filename)
{
	return clGrp.SavePCX(pcx,filename);
}

