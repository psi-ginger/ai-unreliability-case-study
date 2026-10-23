//---------------------------------
// 2003/9/21 김태곤
//---------------------------------

#ifndef _GRAPHIC_H
#define _GRAPHIC_H

#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <windows.h>

#include <ddraw.h>
#include "../Directives.h"

#include "Graphic_TSpr.h"

#include "../EtcTool/EtcTool.h"

#include "../NGraphics/Graphics.h"

#define GRA_MODE_16

class NGraphics;
extern NGraphics g_graphics;

#define MAX_DISPLAYMODE_NUM		200		// 그래픽 카드가 가질 수 있는 화면 모드의 최대 개수. 

//------------------------
// 색상단계 
//------------------------
#define MAX_LIGHT_STEP			10			// 밝은 단계.
#define MAX_DARK_STEP			32			// 어두운 단계. 

//------------------------
// 색정의 
//------------------------
#define COLOR_BLACK				0				// 검은색
#define COLOR_WHITE				255				// 흰색. 
#define COLOR_TRANS				254				// 투명색 


//------------------------
// 화면 모드 정의  
//------------------------
#define MODE_GRAPHICS640_480     1
#define MODE_GRAPHICS800_600     2
#define MODE_GRAPHICS1024_768    3
#define MODE_GRAPHICS1280_1024   4

//-------------------------
// 서피스 정의 
//-------------------------
#define SURFACE_PRIMARY			0
#define SURFACE_BACK			1

///////////////////////////
// PCX 정의 
///////////////////////////
struct PCXPAL {
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

class _head
 {
   public:
   char maker;
   char  ver;
   char encode;
   char bpp;

   short int xmin,ymin;
   short int xmax,ymax;		//!!가로 세로 최대값
   short int hres,vres;		//!!가로 세로 해상도

   char palette[48];

   char  vmode;
   char  nplanes;

   short int bpl;			//!!한 줄당 몇 바이트 ..
   short int palette_type;
   short int shres;			//!!해상도  참조 hres
   short int svres;			//!!해상도  참조 vres

   char unused[54];
};

class _tail 
{
   public:
	char identification;          
	unsigned char palette[768];    //!!팔레트..
};

struct PCX 
{
	 _head Head;
	 _tail Tail;
	 
	 unsigned char *Image;
};


//-----------------------
// 화면 모드 
//-----------------------
class cltDisplayMode 
{
public:
	SI32 siBPP;
	SI32 siWidth;
	SI32 siHeight;

	cltDisplayMode()
	{
		siBPP		= 0;
		siWidth		= 0;
		siHeight	= 0;
	}
	cltDisplayMode(SI32 sibpp, SI32 siwidth, SI32 siheight)
	{
		siBPP		= sibpp;
		siWidth		= siwidth;
		siHeight	= siheight;
	}

	void Set(SI32 sibpp, SI32 siwidth, SI32 siheight)
	{
		siBPP		= sibpp;
		siWidth		= siwidth;
		siHeight	= siheight;
	}

	bool IsSame(const cltDisplayMode* pclinfo)
	{
		if(siBPP != pclinfo->siBPP)return false;
		if(siWidth != pclinfo->siWidth)return false;
		if(siHeight != pclinfo->siHeight)return false;

		return true;
	}
};



class cltGrp {
public:
	HWND	hWnd;				// 윈도우 핸들

	int ScreenMode;
    int ScreenXsize;
    int ScreenYsize;
    int ColorDepth;
    int SurfaceWidth, SurfaceHeight;
    cltDisplayMode clDisplayMode[MAX_DISPLAYMODE_NUM];
    int nDisplayModes;

    bool bLockSwitch;			// Surface가 잠겨 있는가 ?

    int ClipX1, ClipY1, ClipX2, ClipY2;
	RECT ClipRect;
    int YOFF[1024];
	

    LPDIRECTDRAWPALETTE     lpDDPal;        // Direct Draw Palette

public:
    LPDIRECTDRAWCLIPPER lpClipper;  


public:


	LPDIRECTDRAWSURFACE7   lpDDSConvertSurface;

	RECT   m_rcWindow;

	 

    UI08 *Screen8;
	
    LPDIRECTDRAW7           lpDD7; 
    LPDIRECTDRAWSURFACE7     lpDDSPrimary;   // DirectDraw primary surface
    LPDIRECTDRAWSURFACE7     lpDDSBack;      // DirectDraw back surface
	
    DDSURFACEDESC2   SurfaceDesc;
    PALETTEENTRY PalEntries[256];
    
	UI16 TempPaletteTable[256];

    UI08 Palette[768];
    UI08 CurrentPal[768];   // 현재 활성화 되어 있는 팔레트를 저장한다. 
    UI08 OriginalPal[768];

	UI08	DarkTable[MAX_DARK_STEP][256];
	UI08	DarkAlpha[256];
	UI08	ShadowAlpha[256];
	
    // 16비트용 팔레트를 저장한다. 
    UI08	Palette16[64][64][64];
    UI08	PaletteRGB[256][3];
    
    // 광원효과 1의 변수 (어두운 색 계통이 비쳐 보인다.)
    SHORT LightEffect1[64][64];

	// JAYCEE MODIFIED : Add, Screen, Multiply Overlay 팔래트 
	UI08	AddTable[256][256];		//	[BASE][BLEND]
	UI08	ScreenTable[256][256];	//	[BASE][BLEND]
	UI08	MultiplyTable[256][256];//	[BASE][BLEND]
	

	
	SI32 ScreenFlag;

	int GetScreenMode()   { return ScreenMode; }
	int GetScreenXsize()  { return ScreenXsize; }
	int GetScreenYsize()  { return ScreenYsize; }
	void SetScreenXsize(int xsize) { ScreenXsize = xsize; }
	void SetScreenYsize(int ysize) { ScreenYsize = ysize; }
	int GetColorDepth()   { return ColorDepth; }
	int GetSurfaceWidth() { return SurfaceWidth; }
	
	
	// 생성자 함수 
	cltGrp();
	
	/////////////////////////////////////////////////////////////////
	//  기타 처리함수 
	////////////////////////////////////////////////////////////////
	BOOL Error(char * szCaption);

	int GetDisplayModeCounts();
	
    // 서피스를 락한다. 
    // surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
    BOOL LockSurface(int surface);
	
    // 서피스를 언락한다. 
    // surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
    BOOL UnlockSurface(int surface);
	
    BOOL LockSurface(LPDIRECTDRAWSURFACE7 pSurface);
    BOOL UnlockSurface(LPDIRECTDRAWSURFACE7 pSurface);
	
	// 화면을 깨끗이 지운다.
    BOOL	ClearSurface(LPDIRECTDRAWSURFACE7 pSurface);

	
	// 다이렉트 드로우 서피스가 아니라 임의의 메모리를 락해서 쓸 수 있도록 만든다. 
    BOOL LockSurface(UI08*, int xsize, int ysize);

    
	//-----------------------------
	// 화면 모드 관련. 
	//-----------------------------
	// 화면 모드를 추가한다. 
	HRESULT PushDisplayMode(LPDDSURFACEDESC2 pddsd);
	
	
	void GetClipArea( int& x1, int &y1, int &x2, int &y2);
    void SetClipArea( int x1,  int y1,  int x2,  int y2);
	
	// 전체 화면을 클립핑 영역으로 설정한다. 
	void SetClipFullScreen();
	
    BOOL IsInClipArea(int x1, int y1, int x2, int y2);
	int GetClipRect(RECT* rect);
	BOOL FitToClipArea(int sx1, int sy1, int sx2, int sy2, int& dx1, int& dy1, int& dx2, int& dy2);
	
	// OffScreenSurface를 만든다.
	BOOL CreateOffScreen(LPDIRECTDRAWSURFACE7 & pSurface, int xsize, int ysize, int colorkeyswitch = TRUE);
	
	void WaitForVerticalBlank();
	
	//	DirectDraw객체를 생성한다. 
    BOOL	CreateDDraw(HWND hwnd, SI32 siMode, bool bBackSurface, bool bCliperSwitch/*, SI32 Flag */);
	
	// 비디오 모드를 설정하고 다이렉트 드로우 서피스를 새로이 만든다.
    BOOL	SetVideoMode(SI32 siMode, SI32 siCooperation, BOOL bCreateBackSurface = TRUE, BOOL bCliperSwitch = TRUE);
	
	// 다이렉트 드로우의 모든 객체를 제거한다. 
	void ReleaseDirectDraw();
	
	// 손상된 면을 복구한다. 
	void Restore();
	
	void UpdateScreen(HWND hwnd, LPDIRECTDRAWSURFACE7 pSurface, 
		              SI32 xsize, SI32 ysize, SI32 xindent, SI32 yindent, 
					  SI32 srcstartx, SI32 srcstarty, bool bwaitrefresh, 
					  SI32 screenXSize, SI32 screenYSize );

	//--------------------------------------------
	//  팔레트 함수 
	//---------------------------------------------
    LPDIRECTDRAWPALETTE CreateDirectDrawPalette(LPDIRECTDRAW7 pDirectDrawObj, unsigned char* pal);
	
    BOOL LoadOnlyPaletteFile( char* filename, unsigned char *pal );
	
	//palette 화일을 불러온다.
    BOOL LoadOnlyPaletteFile( char* filename );
	
	
	void InitDarkAlpha(const unsigned char *pal);
	void InitShadowAlpha(const unsigned char *pal);
	void InitDarkTable(const unsigned char *pal);

	// JAYCEE MODIFIED
	void InitAddTable(char* filename);
	void InitScreenTable(char* filename);
	void InitMultiplyTable(char* filename);

    // 팔레트를 객체를 만들고 설정한다.
    BOOL SetAllPalette();
    
	// 생성된 lpddPal객체를 가지고 팔레트를 설정한다. 
    HRESULT SetPalette();
    BOOL SetPalette(unsigned char* pal);
    // 팔레트의 특정 위치 색들만 변경한다. 
    HRESULT SetPaletteEntries(DWORD dwStartingEntry, DWORD dwCount,  LPPALETTEENTRY lpEntries);
	
	void SetColorTable();
    
	
	UI08 FindNearColor(UI08 r, UI08 g, UI08 b, const UI08 *pal);

	
	// 16비트 팔레트에 값을 넣는다. 
	BOOL SetPalette16(char* filename);
	UI16 GetPalette16(int r, int g, int b);
	// 광원효과 1 배열 초기화 
	void InitLightEffect1();
	
	// rgb값을 Palette16에 넣을 index값으로 변환 시켜준다. 
	void GetArrayIndexFromColor( int r, int g, int b, int& r1, int &g1, int &b1 );
	

	//----------------------------------------
	// 기본 출력 함수. 
	//----------------------------------------
	void PutPixel8(int x, int y, unsigned char color);
	void PutPixel8C(int x, int y, unsigned char color);

	void HLine8C(int x1, int y1, int x2, unsigned char color);
	void VLine8C(int x1, int y1, int y2, unsigned char color);

	void Line8(int x1, int y1, int x2, int y2, unsigned char color);
	void Line8C(int x1, int y1, int x2, int y2, unsigned char color);

	void Line8x2(int x1, int y1, int x2, int y2, unsigned char color);
	void Line8Cx2(int x1, int y1, int x2, int y2, unsigned char color);

	void Box8(int x1, int y1, int x2, int y2, unsigned char color);
	void Box8C(int x1, int y1, int x2, int y2, unsigned char color);
	
	void FillBox8(int x1, int y1, int x2, int y2, unsigned char color);
	void FillBox8C(int x1, int y1, int x2, int y2, unsigned char color);

	void PutImage8(int x,int y,int xsize,int ysize, unsigned char *image);
	void PutImage8C(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);

	void GetImage8(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer);
	void GetImage8C(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer);

	void PutSpr8(int x,int y,int xsize,int ysize,unsigned char *image);
	void PutSpr8C(int x,int y,int xsize,int ysize,unsigned char *image);

	void PutSpr8(int x,int y,int xsize,int ysize, int sx1,int sy1, int sxsize, int sysize, unsigned char *image);

	void FillScreen(unsigned char color);

	unsigned char GetPixel(int x, int y);
	void PutPixel(int x, int y, unsigned char color);

	void Line(int x1, int y1, int x2, int y2, unsigned char color);
	void Linex2(int x1, int y1, int x2, int y2, unsigned char color);

	void Box(int x1, int y1, int x2, int y2, unsigned char color);

	void FillBox(int x1, int y1, int x2, int y2, unsigned char color);
	void FillBox32(int x1, int y1, int x2, int y2, unsigned char color);

	void PutImage(int x,int y,int xsize,int ysize, unsigned char *image);
	void PutImage32(int x, int y, int xsize, int ysize, UI08* image);

	void GetImage(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer);

	void PutSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image);
	
	//--------------------------------------
	// Spr 출력 함수. 
	//--------------------------------------

	void PutImg8T(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutImg8CT(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutImg8T(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable);

	void PutSprDot8T(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable);
	void PutSprDot8CT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable);
	void PutSprDot8RT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable);
	void PutSprDot8CRT(int x, int y, int xsize, int ysize, unsigned char *image, unsigned char *ctable);
	
	void PutSpr8T(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutSpr8CT(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutSpr8RT(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutSpr8CRT(int x, int y, int xsize, int ysize, unsigned char* image);
	
	void PutSpr8T(int x, int y, int xsize, int ysize, unsigned char* image, unsigned char *ctable);
	void PutSpr8CT(int x, int y, int xsize, int ysize, unsigned char* image, unsigned char *ctable);
	void PutSpr8RT(int x, int y, int xsize, int ysize, unsigned char* image, unsigned char *ctable);
	void PutSpr8CRT(int x, int y, int xsize, int ysize, unsigned char* image, unsigned char *ctable);

	void PutSprShadow8T(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutSprShadow8CT(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutSprShadow8RT(int x, int y, int xsize, int ysize, unsigned char* image);
	void PutSprShadow8CRT(int x, int y, int xsize, int ysize, unsigned char* image);

	void PutSprLight8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprLight8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprLight8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprLight8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);

	void PutSprLight18T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprLight18CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprLight18RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprLight18CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);

	// JAYCEE MODIFIED
	void PutSprAdd8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprAdd8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprAdd8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprAdd8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);

	void PutSprScreen8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprScreen8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprScreen8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprScreen8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);

	void PutSprMultiply8T(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprMultiply8CT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprMultiply8RT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprMultiply8CRT(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);


	// 랜던함 개수를 얻어온다. 
	void GetSprRandPos8T(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint);
	// 랜던함 개수를 얻어온다. 
	void GetSprRandPos8RT(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint);
	// 랜던함 개수를 얻어온다. (말)
	void GetSprRandPos8TH(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint);
	// 랜던함 개수를 얻어온다. (말)
	void GetSprRandPos8RTH(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint);


	
	//------------------------------------------------------------------------------
	// Mpr 출력 함수. 
	//------------------------------------------------------------------------------
	void PutSpr8M(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSpr8CM(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);

	void PutSprScreen8M(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprScreen8CM(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	
	void PutSprMultiply8M(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);
	void PutSprMultiply8CM(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image);

	
public:

	HRESULT BltFast(DWORD surface,  DWORD dwX,  DWORD dwY,                            
		LPDIRECTDRAWSURFACE7 lpDDSrcSurface,  LPRECT lpSrcRect, DWORD dwTrans );
	
	
	//------------------------------------------------
	// Spr 관리 
	//------------------------------------------------
	BOOL LoadSpr(char* szfilename, cltTSpr* pspr);
	BOOL LoadMprToXpr(char* szfilename, cltTSpr* pspr);
	BOOL FreeSpr(cltTSpr& xspr);
	BOOL EncodeSpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey);
	
	BOOL SaveSpr(CHAR* filename, cltTSpr& spr);
	BOOL SaveSpr(CHAR* filename, cltTSpr& spr, UI08 *image);

	BOOL EncodeMpr(UI08* dest, UI08* src, int xsize, int ysize, UI08 colorkey);
	BOOL SaveMpr(CHAR* filename, cltTSpr& spr, UI08 *image);
	


	void record(int count, unsigned char color, FILE* fp);
	int SavePCX(PCX & pcx, char *filename);

};

static HRESULT CALLBACK EnumDisplayModesCallBack(LPDDSURFACEDESC2 pddsd, LPVOID Context);

extern		cltGrp clGrp;


inline BOOL cltGrp::LockSurface(UI08* buffer, int xsize, int ysize)
{
	Screen8 = (UI08*) buffer;
	
	SurfaceWidth =xsize;
	
	SurfaceHeight=ysize;
	
	return TRUE;
}

// 서피스를 락한다. 
// surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
inline BOOL cltGrp::LockSurface(int surface)
{
	switch(surface)
	{
	case SURFACE_PRIMARY:
		return LockSurface( lpDDSPrimary );
		break;
	case SURFACE_BACK:
		return LockSurface( lpDDSBack );
		break;
	default:
		MsgBox("LockSurface", "fd3ff");

	}

	return TRUE;
}

// 서피스를 언락한다. 
// surface: 서피스 상수 SURFACE_PRIMARY, SURFACE_BACK
inline BOOL cltGrp::UnlockSurface(int surface)
{
	switch(surface)
	{
	case SURFACE_PRIMARY:
		return UnlockSurface( lpDDSPrimary );
		break;
	case SURFACE_BACK:
		return UnlockSurface( lpDDSBack );
		break;
	default:
		MsgBox("UnlockSurface", "f3rfssa");

	}
	return TRUE;
}

////////////////////////////////////////////////////////
///  Surface 객체를 lock시켜서 그림을 그릴 수 있게 한다.
////////////////////////////////////////////////////////
inline BOOL cltGrp::LockSurface(LPDIRECTDRAWSURFACE7 pSurface)
{
	BOOL success = FALSE;
	HRESULT result;

	ZeroMemory( &SurfaceDesc, sizeof( SurfaceDesc ) );
	SurfaceDesc.dwSize=sizeof( SurfaceDesc );

	if( bLockSwitch == true)
	{
		MsgBox( "Locked", "fds3ff");
	    return FALSE;
	}

	bLockSwitch = true;

	BOOL exitloop = FALSE;

	do
	{
		result = pSurface->Lock( NULL, &SurfaceDesc, DDLOCK_WAIT, NULL );

		if( result == DDERR_SURFACELOST )
		{
			pSurface->Restore();
		}

		else if( result != DDERR_WASSTILLDRAWING )
		{
			exitloop=TRUE;
		}
	}
	while( ! exitloop );

	if( result == DD_OK )
	{
		Screen8 = (UI08*) SurfaceDesc.lpSurface;

	    SurfaceWidth = SurfaceDesc.lPitch / ( ColorDepth / 8 );
		SurfaceHeight = SurfaceDesc.dwHeight;

		return TRUE;
	}
	else
	{
	    MsgBox("LockSurface", "Error");
		
		if(result==DDERR_SURFACEBUSY)
		{
			MsgBox("SurfaceBusy", "dsa2ds");
		}
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////
// Lock된 면 객체를 풀어준다.
////////////////////////////////////////////////////////////////
inline BOOL cltGrp::UnlockSurface(LPDIRECTDRAWSURFACE7 pSurface)
{
	if(bLockSwitch == true)
	{
		bLockSwitch = false;
	}
	else
	{
		MsgBox("Unlocked", "sf3fdf");
	}
	
	pSurface->Unlock(NULL);
	return TRUE;
}

inline void cltGrp::GetClipArea(int&x1, int &y1, int&x2, int&y2)
{
	x1=ClipX1;
	y1=ClipY1;
	x2=ClipX2;
	y2=ClipY2;
}

inline int cltGrp::GetClipRect(RECT* rect)
{
	rect->left   = ClipX1;
	rect->right  = ClipX2;
	rect->top    = ClipY1;
	rect->bottom = ClipY2;

	return 0;
}

inline void cltGrp::SetClipArea(int x1, int y1, int x2, int y2)
{
	ClipX1=x1;
	ClipX2=x2;
	ClipY1=y1;
	ClipY2=y2;
}

inline void cltGrp::SetClipFullScreen()
{
	ClipX1 = 0;
	ClipX2 = ScreenXsize-1;
	ClipY1 = 0;
	ClipY2 = ScreenYsize-1;
}

inline BOOL cltGrp::IsInClipArea(int x1, int y1, int x2, int y2)
{
	if(ClipX1 <= x1 && x2 <= ClipX2 && ClipY1 <= y1 && y2 <= ClipY2 )
		return TRUE;
	
	return FALSE; 
}

inline BOOL cltGrp::FitToClipArea(int sx1, int sy1, int sx2, int sy2, int& dx1, int& dy1, int& dx2, int& dy2)
{
	dx1=max(sx1, ClipX1);
	dx2=min(sx2, ClipX2);
	dy1=max(sy1, ClipY1);
	dy2=min(sy2, ClipY2);
	return TRUE;
}

inline void cltGrp::Restore()
{
	if(lpDD7)
	{
		 lpDD7->RestoreAllSurfaces();
	}
}



#endif

