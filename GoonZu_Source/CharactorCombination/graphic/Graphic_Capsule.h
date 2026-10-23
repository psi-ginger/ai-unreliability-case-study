//---------------------------------
// 2004/11/15 성준엽
//---------------------------------

#ifndef _GRCAPSULE_H
#define _GRCAPSULE_H

#define GRA_MODE_16

#include <Windows.h>
#include "../Directives.h"

#include <Graphic.h>

#include "EtcTool.h"

#define SPR_MODE_T			0
#define SPR_MODE_CT			1
#define SPR_MODE_RT			2
#define SPR_MODE_CRT		3

#define PUT_SPR_IMG			0
#define PUT_SPR_SPR			1
#define PUT_SPR_SHADOW		2
#define PUT_SPR_LIGHT		3
#define PUT_SPR_LIGHT1		4
#define PUT_SPR_ADD			5
#define PUT_SPR_SCREEN		6
#define PUT_SPR_MULTIPLY	7
#define PUT_SPR_ALPHA		8

#define PUT_SPRTABLE_IMG	0
#define PUT_SPRTABLE_DOT	1
#define PUT_SPRTABLE_SPR	2

#define PAL_MODE_PAL		0
#define PAL_MODE_CURRENT	1
#define PAL_MODE_ORG		2

class cltGraphic_Capsule{
public:
	cltGraphic_Capsule();
	~cltGraphic_Capsule();

	void PutSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, 
				SI16 putspr, SI16 sprmode );

	void PutSprTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, UI08 *ctable,
				SI16 putsprtable, SI16 sprmode );

	void PutMpr8(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image,
				SI16 mprmode,BOOL bCMmode);

	void PutMpr(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image,
				SI16 mprmode,BOOL bCMmode,SI32 alphavalue = 4);

	// 랜던함 개수를 얻어온다. 
	void GetSprRandPos8(int x, int y, int xsize, int ysize,
						unsigned char *image, SI32* pgetnum, POINT* ppoint,BOOL bRTmode);
	// 랜던함 개수를 얻어온다. (말)
	void GetSprRandPos8H(int x, int y, int xsize, int ysize,
						unsigned char *image, SI32* pgetnum, POINT* ppoint,BOOL bRTmode);

	void FillScreen(unsigned char color);

	void Line(int x1, int y1, int x2, int y2, unsigned char color,BOOL bx2mode);

	void FillBox(int x1, int y1, int x2, int y2, unsigned char color,BOOL b32mode);
	void Box(int x1, int y1, int x2, int y2, unsigned char color);

	void PutImage(int x,int y,int xsize,int ysize, unsigned char *image,BOOL b32mode);
	void PutSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image);
	void PutSpr8(int x,int y,int xsize,int ysize,unsigned char *image,BOOL bCmode);

	void GetImage(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer);

	void PutXpr16( SI32 x, SI32 y, SI32 xsize, SI32 ysize, RGB16* image, RGB16 colorkey );
	void PutXpr16A( SI32 x, SI32 y, SI32 xsize, SI32 ysize, RGB16* image, int alpha, RGB16 colorkey );

	void PutSpr16EF( SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, int effect, BOOL bReverse );

	//------------------------------------------------
	// Spr 관리 
	//------------------------------------------------
	BOOL LoadSpr(char* szfilename, cltTSpr* pspr);
	BOOL LoadTile(char* szfilename, cltTSpr* pspr );
	BOOL FreeSpr(cltTSpr& xspr);
	BOOL EncodeSpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey);
	
	BOOL EncodeMpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey);
	BOOL SaveMpr(CHAR* filename, cltTSpr& spr, UI08 *image);

	int SavePCX(PCX & pcx, char *filename);

public:
	BOOL InitGP(HWND hwnd,SI32 screenmode,LPDIRECTDRAWSURFACE7 & lpbackscreen,SI32 xsize , SI32 ysize, BOOL bFullScreen = FALSE, SI32 screenXSize = 0, SI32 screenYSize = 0 );
	void ReleaseGP();

	UI08* GetPalette8(SI32 mode);
	void LoadPalette();
	void SetGamePalette(const unsigned char *pal);

	void SetClipArea(SI32 x1,SI32 y1,SI32 x2,SI32 y2 );
	void GetClipArea( int& x1, int &y1, int &x2, int &y2 );

	void SetClipFullScreen();

    BOOL IsInClipArea(int x1, int y1, int x2, int y2);
	int GetClipRect(RECT* rect);
	BOOL FitToClipArea(int sx1, int sy1, int sx2, int sy2, int& dx1, int& dy1, int& dx2, int& dy2);

	BOOL CreateSurface(LPDIRECTDRAWSURFACE7 & pSurface, int xsize, int ysize, int colorkeyswitch = TRUE);

	BOOL CreateOffScreen8(LPDIRECTDRAWSURFACE7 & pSurface, int xsize, int ysize, int colorkeyswitch = TRUE);

	UI08* GetSurfacePtr( int x, int y );

	// 손상된 면을 복구한다. 
	void Restore();

	void UpdateScreen(HWND hwnd, LPDIRECTDRAWSURFACE7 pSurface, SI32 xsize, SI32 ysize,
					  SI32 xindent, SI32 yindent, SI32 srcstartx, SI32 srcstarty , bool bwaitrefresh,
					  SI32 screenXSize, SI32 screenYSize );
	
public:
    // 서피스를 락한다. 
    // surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
//    BOOL LockSurface(int surface);

    // 서피스를 언락한다. 
    // surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
//    BOOL UnlockSurface(int surface);

    BOOL LockSurface(LPDIRECTDRAWSURFACE7 pSurface);
    BOOL UnlockSurface(LPDIRECTDRAWSURFACE7 pSurface);

	// 화면을 깨끗이 지운다.
    BOOL	ClearSurface(LPDIRECTDRAWSURFACE7 pSurface);

	// 다이렉트 드로우 서피스가 아니라 임의의 메모리를 락해서 쓸 수 있도록 만든다. 
    BOOL LockSurface(UI08*, int xsize, int ysize);

	// 생성된 lpddPal객체를 가지고 팔레트를 설정한다. 
    HRESULT SetPalette();
    BOOL SetPalette(unsigned char* pal);

public:
	int GetScreenMode();
	int GetScreenXsize();
	int GetScreenYsize();
	void SetScreenXsize(int xsize);
	void SetScreenYsize(int ysize);
	int GetColorDepth();
	int GetSurfaceWidth();



private:
	void PutSprImg(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );
	void PutSprSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );
	void PutSprShadow(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );
	void PutSprLight(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );
	void PutSprLight1(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );
	void PutSprAdd(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );
	void PutSprScreen(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );
	void PutSprMultiply(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, SI16 sprmode );

	void PutSprImgTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, UI08 *ctable, SI16 sprmode );
	void PutSprDotTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, UI08 *ctable, SI16 sprmode );
	void PutSprSprTable(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image, UI08 *ctable, SI16 sprmode );


};

extern cltGraphic_Capsule GP;


inline BOOL cltGraphic_Capsule::IsInClipArea(int x1, int y1, int x2, int y2)
{
	return clGrp.IsInClipArea(x1,y1,x2,y2);
}

inline int cltGraphic_Capsule::GetClipRect(RECT* rect)
{
	return clGrp.GetClipRect(rect);
}

inline BOOL cltGraphic_Capsule::FitToClipArea(int sx1, int sy1, int sx2, int sy2, int& dx1, int& dy1,
									   int& dx2, int& dy2)
{
	return clGrp.FitToClipArea(sx1,sy1,sx2,sy2,dx1,dy1,dx2,dy2);
}

inline UI08* cltGraphic_Capsule::GetSurfacePtr( int x, int y )
{
#ifdef GRA_MODE_16
	
	return (UI08*)g_graphics.m_pImage + ( y * g_graphics.m_lPitch ) + ( x << 1 );

#else

	return (UI08*)clGrp.Screen8 + ( y * clGrp.SurfaceWidth ) + x;

#endif
}

inline void cltGraphic_Capsule::GetImage(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer)
{
#ifdef GRA_MODE_16

#else
	clGrp.GetImage(x,y,xsize,ysize,buffer);

#endif

}

inline BOOL cltGraphic_Capsule::LockSurface(LPDIRECTDRAWSURFACE7 pSurface)
{

	BOOL ret;

#ifdef GRA_MODE_16
	ret = g_graphics.LockSurface(pSurface);
#else
	ret = clGrp.LockSurface(pSurface);
#endif
	return ret;
}

inline BOOL cltGraphic_Capsule::UnlockSurface(LPDIRECTDRAWSURFACE7 pSurface)
{

#ifdef GRA_MODE_16
	g_graphics.UnlockSurface(pSurface);
#else
	clGrp.UnlockSurface(pSurface);
#endif

	return TRUE;
}

// 화면을 깨끗이 지운다.
inline BOOL cltGraphic_Capsule::ClearSurface(LPDIRECTDRAWSURFACE7 pSurface)
{
	return clGrp.ClearSurface(pSurface);
}

// 다이렉트 드로우 서피스가 아니라 임의의 메모리를 락해서 쓸 수 있도록 만든다. 

inline BOOL cltGraphic_Capsule::LockSurface(UI08*buffer, int xsize, int ysize)
{
	return clGrp.LockSurface(buffer,xsize,ysize);
}

inline void cltGraphic_Capsule::GetClipArea( int& x1, int &y1, int &x2, int &y2 )
{
	clGrp.GetClipArea(x1, y1, x2, y2);
}

inline void cltGraphic_Capsule::SetClipArea(SI32 x1,SI32 y1,SI32 x2,SI32 y2 )
{
	clGrp.SetClipArea(x1, y1, x2, y2);
	g_graphics.SetClipRect( x1, y1, x2, y2 );
}


#endif
