//---------------------------------
// 2003/9/21 김태곤
//---------------------------------

#include <Graphic.h>
#include "EtcTool.h"

// 그래픽 라이브러리 객체 
cltGrp clGrp;


cltGrp::cltGrp()
{
	ZeroMemory(this, sizeof(cltGrp));

	ScreenMode  = MODE_GRAPHICS800_600;
    ScreenXsize = 800;
    ScreenYsize = 600;
    ColorDepth  = 8;
	
	SurfaceWidth = ScreenXsize;
	SurfaceHeight = ScreenYsize;

	// Surface가 잠겨 있는가 ?
	bLockSwitch		=	false;

	nDisplayModes	=	0;

	lpDD7 = NULL;
	lpDDSPrimary = NULL;
	lpDDSBack			= NULL;
	lpDDSConvertSurface = NULL;

	lpClipper			= NULL;
	

}

BOOL cltGrp::CreateDDraw(HWND hwnd, SI32 siMode, bool bBackSurface, bool bCliperSwitch/*, SI32 Flag*/ )
{
	HRESULT ddrval;
	
	SI32 Flag = clGrp.ScreenFlag;//DDSCL_NORMAL;
	//ScreenFlag = DDSCL_NORMAL;
	ScreenFlag = Flag;
	
	ddrval	= DirectDrawCreateEx(NULL, (VOID**)&lpDD7, IID_IDirectDraw7, NULL);

	//윈도우 핸들을 저장한다.
	hWnd	= hwnd;		

	if( lpDD7 )
	{

       // exclusive mode를 얻기
       ddrval = lpDD7->SetCooperativeLevel( hWnd, ScreenFlag );

	   ShowWindow(hWnd, SW_SHOW);

	   // 지원되는 디스플레이 모드의 갯수와 종류를 기록한다. 
	   GetDisplayModeCounts();


       if(ddrval == DD_OK)
	   {  
		   
		   if( SetVideoMode(siMode, ScreenFlag, bBackSurface, bCliperSwitch ) == TRUE )
		   {
			   return TRUE;
           }
		   else 
		   {
			   MsgBox("SetVideoMode()", "cdsarc");
			   return FALSE;
		   }
	   }
	   else
	   {
		   MsgBox("CooperativeLevel", "fds3dsf");
		   return FALSE;
	   }

	}

	return FALSE;
}

void cltGrp::ReleaseDirectDraw()
{

#ifdef GRA_MODE_16
	return;
#else
	if( lpDDSBack ) {
		lpDDSBack->Release();
		lpDDSBack = NULL;
	}


	if( lpDDSConvertSurface ) {
		lpDDSConvertSurface->Release();
		lpDDSConvertSurface = NULL;
	}


	if(lpDDSPrimary	!=	NULL)
	{
		lpDDSPrimary->Release();
		lpDDSPrimary=NULL;
	}

	/*
	if( lpClipper )	
	{
		lpClipper->Release();
		lpClipper=NULL;
    }
*/


	if(lpDDPal) {
	    lpDDPal->Release();
		lpDDPal = NULL;
	}

	
	if(lpDD7	!=	NULL)
	{
		lpDD7->Release();
		lpDD7=NULL;
	}
#endif
}


BOOL	cltGrp::SetVideoMode(SI32 siMode, SI32 siCooperation, BOOL bCreateBackSurface, BOOL bCliperSwitch)
{
	HRESULT             ddrval;
	DDSURFACEDESC2      ddsd;
	int i;
	int xsize = 0, ysize = 0;
	
	ScreenMode	=	siMode;
	
	switch(siMode)
	{

	case MODE_GRAPHICS640_480:
		ScreenXsize = 640;	ScreenYsize = 480;	ColorDepth = 8;
		break;

	case MODE_GRAPHICS800_600:
		ScreenXsize = 320;	ScreenYsize = 240;	ColorDepth = 8;
		break;

	case MODE_GRAPHICS1024_768:
		ScreenXsize = 1024;	ScreenYsize = 768;	ColorDepth = 8;
		break;

	case MODE_GRAPHICS1280_1024:
		ScreenXsize = 1280;	ScreenYsize = 1024;	ColorDepth = 8;
		break;
	}
	
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	
	ddsd.dwSize = sizeof(ddsd);
	
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	
	ddrval = lpDD7->CreateSurface(&ddsd, &lpDDSPrimary, NULL);
	
	if(ddrval != DD_OK)
	{
		MsgBox("CreateSurface", "ds3dsfsd");
		return FALSE;
	}
	
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.dwHeight = ScreenYsize;
	ddsd.dwWidth = ScreenXsize;
	
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	
	lpDD7->CreateSurface(&ddsd, &lpDDSConvertSurface, NULL);
	
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ZeroMemory(&(ddsd.ddpfPixelFormat),sizeof(DDPIXELFORMAT));
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
	ddsd.ddpfPixelFormat.dwRGBBitCount = 8;	
	ddsd.dwHeight = ScreenYsize;
	ddsd.dwWidth = ScreenXsize;
	
	ddrval = lpDD7->CreateSurface( &ddsd, &lpDDSBack, NULL );//lpSurface
	
	if(ddrval != DD_OK)
	{
		MsgBox("CreateSurface", "d2ef21e");
		return FALSE;
	}
	
	ddrval = lpDD7->CreateClipper(0, &lpClipper, NULL);
	
	if(ddrval != DD_OK)
	{
		MsgBox("CreateClipper", "f23fdsf");
		return FALSE;
	}
	
	// 클리퍼 객체를 만들고 서피스에 연결한다.
	ddrval = lpClipper->SetHWnd(0, hWnd);
	
	if(ddrval != DD_OK)
	{
		MsgBox("SetHwnd", "dwe2d");
		return FALSE;
	}
	
	ddrval = lpDDSPrimary->SetClipper(lpClipper);
	
	if(ddrval != DD_OK)
	{
		MsgBox("SetCliper", "d32df");
		return FALSE;
	}
	
	
	lpClipper->Release();
	
	
	
	if(ddrval != DD_OK)
	{
		MsgBox("SetClipper", "d32d");
		return FALSE;
	}
	
	
	if( LockSurface( lpDDSBack )==TRUE )
	{
		xsize=SurfaceDesc.lPitch/(ColorDepth/8);
		ysize=SurfaceDesc.dwHeight;
		UnlockSurface( lpDDSBack );
	} else {
		return FALSE;
	}
	
	ZeroMemory(&ddsd, sizeof(ddsd));
	
	ddsd.dwSize=sizeof(ddsd);
	ddsd.dwFlags=DDSD_PIXELFORMAT;
	
	lpDDSBack->GetSurfaceDesc(&ddsd);
	
	for(i=0;i<=ScreenYsize;i++)
	{
		YOFF[i]=i*xsize;
	}
	
	if(ddrval!=DD_OK)
	{
		MsgBox("SetVideoMode", "ad43fff");
		return FALSE;
	}
	
	return TRUE;
}


// 현재그랙픽 드라이버가 지원하는 모드의 갯수를 알려준다. 
int cltGrp::GetDisplayModeCounts()
{
	nDisplayModes=0;

	HRESULT hr=lpDD7->EnumDisplayModes( 0, NULL, NULL, EnumDisplayModesCallBack );
	
	if( hr != DD_OK )
	{
		return -1;
	}

	return nDisplayModes;
}   

//-------------------------------------------
// 화면 모드를 추가한다. 
//-------------------------------------------
HRESULT cltGrp::PushDisplayMode( LPDDSURFACEDESC2 pddsd )
{
	if( nDisplayModes == MAX_DISPLAYMODE_NUM - 1 )
	{
		return DDENUMRET_CANCEL;
	}

	clDisplayMode[ nDisplayModes ].Set(pddsd->ddpfPixelFormat.dwRGBBitCount, pddsd->dwWidth, pddsd->dwHeight);

	nDisplayModes++;

	return DDENUMRET_OK;
}

BOOL cltGrp::CreateOffScreen( LPDIRECTDRAWSURFACE7 & pSurface, int xsize, int ysize, int colorkeyswitch)
{
    HRESULT				ddrval = NULL ;
    DDSURFACEDESC2      ddsd;
    DDCOLORKEY			ddColorKey;
	BOOL				bFailedVideoSurfaceOrNonVideoMemory = TRUE;

	// 다이렉트 드로우 오브젝트가 준비되어 있지 않다면 FALSE를 리턴한다.
	if( lpDD7 == NULL )	return FALSE;
	
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ZeroMemory(&ddsd.ddpfPixelFormat,sizeof(DDPIXELFORMAT));
	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
	ddsd.ddpfPixelFormat.dwRGBBitCount = 8;	
	ddsd.dwHeight = ysize;
	ddsd.dwWidth = xsize;
	
	lpDD7->CreateSurface(&ddsd, &pSurface, NULL);
	if(pSurface == NULL)
	{
		return FALSE;
	}
	
	// 윈도우모드에서 8bit 서피스가 dc를 사용하려면 팔레트가 필요하다!
	if (lpDDPal != NULL) 
	{
		pSurface->SetPalette( lpDDPal );
	}
	
	if(colorkeyswitch==TRUE)
	{
		///// 투명색을 설정한다. 
		ddColorKey.dwColorSpaceLowValue = COLOR_TRANS;
		ddColorKey.dwColorSpaceHighValue = COLOR_TRANS;
		
		//// 투명색을 어프스크린에 지정한다. 
		if( pSurface->SetColorKey( DDCKEY_SRCBLT, &ddColorKey ) != DD_OK )
		{
			return FALSE;
		}
	}
	

	return TRUE;
}






BOOL cltGrp::Error(char * szCaption)
{
	char buffer[256];
	char text[128];
	DWORD errcode = GetLastError();
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode, 0, buffer,  128, NULL);
	wsprintf(text, "ErrorCode=%d Message=%s", errcode, buffer);
	MsgBox(text, szCaption);

	return TRUE;
}
 

/////////////////////////////////////////////////////
// 일반 드로잉 함수
/////////////////////////////////////////////////////

unsigned char cltGrp::GetPixel(int x, int y)
{
   return Screen8[x+y*SurfaceWidth];
}

void cltGrp::PutPixel8(int x, int y, unsigned char color)
{
   Screen8[x+y*SurfaceWidth]=color;
}


void cltGrp::PutPixel8C(int x, int y, unsigned char color)
{
	if(x<ClipX1)return ;
	if(x>ClipX2)return ;
	if(y<ClipY1)return ;
	if(y>ClipY2)return ;

	Screen8[x+y*SurfaceWidth]=color;
}


void cltGrp::FillScreen(unsigned char color)
{
    int i, j;
	int repeat=SurfaceWidth/4;
	int newcolor=color|(color<<8)|(color<<16)|(color<<24);
	//unsigned char newcolor=254;
	int * vidptr;

	vidptr=(int*)Screen8;

	for(i=0;i<SurfaceHeight;i++)
	{
		for(j=0;j<repeat;j++)
		{

			*vidptr=newcolor;
		  vidptr++;
		}
	}
}

void cltGrp::Line8(int x1, int y1, int x2, int y2, unsigned char color)
{
	int t, distance;
	int xerr=0, yerr=0, delta_x, delta_y;
	int incx, incy;

	// X, Y방향의 변위를 계산
	delta_x=x2-x1;
	delta_y=y2-y1;

	// 증가의 방향을 정함 -0은 수직선 또는 수평선을 의미 
	if(delta_x>0)       incx=1;
	else if(delta_x==0) incx=0;
	else                incx=-1;

	if(delta_y>0)       incy=1;
	else if(delta_y==0) incy=0;
	else                incy=-1;

	// 어느 방향의 증가가 더 큰가 결정
	delta_x=TABS(delta_x);
	delta_y=TABS(delta_y);
	if(delta_x > delta_y)distance=delta_x;
	else distance=delta_y;

	// 선을 그린다. 
	for(t=0;t<=distance+1;t++)
	{
		PutPixel(x1, y1, color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			x1+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			y1+=incy;
		}
	}
}


void cltGrp::Line8C(int x1, int y1, int x2, int y2, unsigned char color)
{
	int t, distance;
	int xerr=0, yerr=0, delta_x, delta_y;
	int incx, incy;

	// X, Y방향의 변위를 계산
	delta_x=x2-x1;
	delta_y=y2-y1;

	// 증가의 방향을 정함 -0은 수직선 또는 수평선을 의미 
	if(delta_x>0)       incx=1;
	else if(delta_x==0) incx=0;
	else                incx=-1;

	if(delta_y>0)       incy=1;
	else if(delta_y==0) incy=0;
	else                incy=-1;

	// 어느 방향의 증가가 더 큰가 결정
	delta_x=TABS(delta_x);
	delta_y=TABS(delta_y);
	if(delta_x > delta_y)distance=delta_x;
	else distance=delta_y;

	// 선을 그린다. 
	for(t=0;t<=distance+1;t++)
	{
		PutPixel(x1, y1, color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			x1+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			y1+=incy;
		}
	}
}

void cltGrp::Line8x2(int x1, int y1, int x2, int y2, unsigned char color)
{
	int t, distance;
	int xerr=0, yerr=0, delta_x, delta_y;
	int incx, incy;

	// X, Y방향의 변위를 계산
	delta_x=x2-x1;
	delta_y=y2-y1;

	// 증가의 방향을 정함 -0은 수직선 또는 수평선을 의미 
	if(delta_x>0)       incx=1;
	else if(delta_x==0) incx=0;
	else                incx=-1;

	if(delta_y>0)       incy=1;
	else if(delta_y==0) incy=0;
	else                incy=-1;

	// 어느 방향의 증가가 더 큰가 결정
	delta_x=TABS(delta_x);
	delta_y=TABS(delta_y);
	if(delta_x > delta_y)distance=delta_x;
	else distance=delta_y;

	// 선을 그린다. 
	for(t=0;t<=distance+1;t++)
	{
		PutPixel(x1-1, y1, color);
		PutPixel(x1, y1, color);
		PutPixel(x1, y1-1, color);
		PutPixel(x1, y1+1, color);
		PutPixel(x1+1, y1, color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			x1+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			y1+=incy;
		}
	}
}


void cltGrp::Line8Cx2(int x1, int y1, int x2, int y2, unsigned char color)
{
	int t, distance;
	int xerr=0, yerr=0, delta_x, delta_y;
	int incx, incy;

	// X, Y방향의 변위를 계산
	delta_x=x2-x1;
	delta_y=y2-y1;

	// 증가의 방향을 정함 -0은 수직선 또는 수평선을 의미 
	if(delta_x>0)       incx=1;
	else if(delta_x==0) incx=0;
	else                incx=-1;

	if(delta_y>0)       incy=1;
	else if(delta_y==0) incy=0;
	else                incy=-1;

	// 어느 방향의 증가가 더 큰가 결정
	delta_x=TABS(delta_x);
	delta_y=TABS(delta_y);
	if(delta_x > delta_y)distance=delta_x;
	else distance=delta_y;

	// 선을 그린다. 
	for(t=0;t<=distance+1;t++)
	{
		PutPixel(x1-1, y1, color);
		PutPixel(x1, y1, color);
		PutPixel(x1, y1-1, color);
		PutPixel(x1, y1+1, color);
		PutPixel(x1+1, y1, color);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			x1+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			y1+=incy;
		}
	}
}
void cltGrp::HLine8C(int x1, int y1, int x2, unsigned char color)
{

	int i;
	int temp, xsize;
	int s1=1, s2=1, s3=1;

	if(x2<x1){temp=x2; x2=x1; x1=temp;}

	if(x1>ClipX2)return ;
	if(x2<ClipX1)return ;
	if(y1>ClipY2)return ;

	if(x1<ClipX1){x1=ClipX1;s1=0;}
	if(y1<ClipY1){y1=ClipY1;s2=0;}
	if(x2>ClipX2){x2=ClipX2;s3=0;}

	xsize=x2-x1+1;

	if(s2)
	{
		for(i=x1;i<=x2;i++)
		   Screen8[i+y1*SurfaceWidth]=color;
	} 
}


void cltGrp::VLine8C(int x1, int y1, int y2, unsigned char color)
{

	int i;
	int temp;
	int s1=1, s2=1, s4=1;

	if(y2<y1){temp=y2; y2=y1; y1=temp;}

	if(x1>ClipX2)return ;
	if(y1>ClipY2)return ;
	if(y2<ClipY1)return ;

	if(x1<ClipX1){x1=ClipX1;s1=0;}
	if(y1<ClipY1){y1=ClipY1;s2=0;}
	if(y2>ClipY2){y2=ClipY2;s4=0;}

	for(i=y1;i<=y2;i++)
	{
	   if(s1)
		  Screen8[x1+i*SurfaceWidth]=color;
	}
}

void cltGrp::Box8(int x1, int y1, int x2, int y2, unsigned char color)
{

	int i;
	int temp, xsize;


	xsize=x2-x1+1;

	if(x2<x1){temp=x2; x2=x1; x1=temp;};
	if(y2<y1){temp=y2; y2=y1; y1=temp;};

	for(i=x1;i<=x2;i++)
	{
       Screen8[i+y1*SurfaceWidth]=color;
	   Screen8[i+y2*SurfaceWidth]=color;
	}

	for(i=y1;i<=y2;i++)
	{
       Screen8[x1+i*SurfaceWidth]=color;
	   Screen8[x2+i*SurfaceWidth]=color;
	}

}

void cltGrp::Box8C(int x1, int y1, int x2, int y2, unsigned char color)
{

	int i;
	int temp, xsize;
	int s1=1, s2=1, s3=1, s4=1;

	if(x2<x1){temp=x2; x2=x1; x1=temp;}
	if(y2<y1){temp=y2; y2=y1; y1=temp;}

	if(x1>ClipX2)return ;
	if(x2<ClipX1)return ;
	if(y1>ClipY2)return ;
	if(y2<ClipY1)return ;

	if(x1<ClipX1){x1=ClipX1;s1=0;}
	if(y1<ClipY1){y1=ClipY1;s2=0;}
	if(x2>ClipX2){x2=ClipX2;s3=0;}
	if(y2>ClipY2){y2=ClipY2;s4=0;}

	xsize=x2-x1+1;


	if(s2)
	{
		for(i=x1;i<=x2;i++)
		   Screen8[i+y1*SurfaceWidth]=color;
	} 
	if(s4)
	{
		for(i=x1;i<=x2;i++)
		   Screen8[i+y2*SurfaceWidth]=color;

	}

	for(i=y1;i<=y2;i++){
	   if(s1)
		  Screen8[x1+i*SurfaceWidth]=color;
	   if(s3)
		  Screen8[x2+i*SurfaceWidth]=color;
	}
}


void cltGrp::FillBox8(int x1, int y1, int x2, int y2, unsigned char color)
{
	int i, j,  xsize;
	int temp;

	if(x1>=SurfaceWidth)return ;
	if(x1<0)return ;
	if(x2>=SurfaceWidth)return ;
	if(x2<0)return ;
	if(y1>=SurfaceHeight)return ;
	if(y1<0)return ;
	if(y2>=SurfaceHeight)return ;
	if(y2<0)return ;
	if(color>=256)return ;

	if(x2<x1){temp=x2; x2=x1; x1=temp;};
	if(y2<y1){temp=y2; y2=y1; y1=temp;};

	xsize=x2-x1+1;

	for(i=y1;i<=y2;i++)
	{
		for(j=x1;j<=x2;j++)
		   Screen8[j+i*SurfaceWidth]=color;
	}
}



void cltGrp::FillBox32(int x1, int y1, int x2, int y2, unsigned char color)
{
	int temp;

	if(x2<x1){temp=x2; x2=x1; x1=temp;};
	if(y2<y1){temp=y2; y2=y1; y1=temp;};

	int i, j;
	int ysize=y2-y1+1;
	int xsize=x2-x1+1;

	int repeat=xsize/(32/ColorDepth);
	int skip=SurfaceWidth/(32/ColorDepth)-xsize/(32/ColorDepth);
	
	int newcolor;
	  newcolor=color|(color<<8)|(color<<16)|(color<<24);

	int * vidptr;

	vidptr=(int*)(Screen8+(x1+y1*SurfaceWidth));

	for(i=0;i<ysize;i++)
	{
		for(j=0;j<repeat;j++)
		{
           *vidptr=newcolor;
		  vidptr++;
		}
		vidptr+=skip;
	}
	
}

void cltGrp::FillBox8C(int x1, int y1, int x2, int y2, unsigned char color)
{
	int i, j, xsize;
	int temp;

	if(x2<x1){temp=x2; x2=x1; x1=temp;};
	if(y2<y1){temp=y2; y2=y1; y1=temp;};

	if(x2<ClipX1)return ;
	if(x1>ClipX2)return ;

	if(y2<ClipY1)return ;
	if(y1>ClipY2)return ;

	if(x1<ClipX1)x1=ClipX1;
	if(y1<ClipY1)y1=ClipY1;

	if(x2>ClipX2)x2=ClipX2;
	if(y2>ClipY2)y2=ClipY2;

	xsize=x2-x1+1;

    for(i=y1;i<=y2;i++)
	{
		for(j=x1;j<=x2;j++)
		   Screen8[j+i*SurfaceWidth]=color;
	}
}



void cltGrp::PutImage8(int x,int y,int xsize,int ysize, unsigned char *image)
{
	int i,j,skipbytes;
	UI08 *vidptr;

	vidptr    = Screen8+SurfaceWidth*y + x;
	skipbytes = SurfaceWidth-xsize;

	 for(i=0;i<ysize;i++)
	 {
	   for(j=0;j<xsize;j++)
	   {
		 *vidptr=*image;
		 image++;vidptr++;
	   }
	  vidptr+=skipbytes;
	}
}

VOID cltGrp::PutImage8C(SHORT x, SHORT y, SHORT xsize, SHORT ysize, UI08* image)
{
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;

	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
				vidptr[j]=image[index];
			
			j++; index++;
		}
		vidptr+=SurfaceWidth;
	}
}




void cltGrp::PutImage32(int x, int y, int xsize, int ysize, UI08* image)
{
	int i, j, skip;
	SHORT continuenumber=(4/(ColorDepth/8));
	int x_repeat=xsize/continuenumber;
	int * src=(int*)image;
	int * vidptr;

	vidptr=(int*)(Screen8+y*SurfaceWidth+x);
	skip=SurfaceWidth/continuenumber-xsize/continuenumber;

	for(i=0;i<ysize;i++)
	{
		for(j=0;j<x_repeat;j++)
	    {
		  *vidptr=*src;
		  src++; vidptr++;
	    }

	vidptr+=skip;
	}
}


void cltGrp::GetImage8C(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer)
{
   int i, j;
   int sourcex, sourcey;

   for(i=0;i<ysize;i++)
	  for(j=0;j<xsize;j++)
      {
		  sourcex=x+j;
		  sourcey=y+i;

		  if(sourcex>=0 && sourcex<SurfaceWidth
		  && sourcey>=0 && sourcey<SurfaceHeight)
		  {
			  buffer[j+i*xsize]=Screen8[sourcex+sourcey*SurfaceWidth];
		  }
	  }
}




void cltGrp::GetImage8(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer)
{
   int i, j;

   for(i=0;i<ysize;i++)
   {
	  for(j=0;j<xsize;j++)
      {
		  buffer[j+i*xsize]=Screen8[x+j+(y+i)*SurfaceWidth];
	  }
   }
}


void cltGrp::PutSpr8(int x,int y,int xsize,int ysize,unsigned char *image)
{
	int i,j,skipbytes;
	UI08 *vidptr;

	vidptr=Screen8+y*SurfaceWidth + x;
	skipbytes = SurfaceWidth-xsize;

	 for(i=0;i<ysize;i++)
	 {
	   for(j=0;j<xsize;j++)
	   {
		 if(*image!=COLOR_TRANS) *vidptr=*image;
		 image++;vidptr++;
	   }
	  vidptr+=skipbytes;
	}
}

void cltGrp::PutSpr8C(int x,int y,int xsize,int ysize,unsigned char *image)
{
	
	int i, j, index=0;
	int up_skip=0, down_skip=0;
	int left_skip=0, right_skip=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth+x;

	if(x+xsize-1<ClipX1)return ;
	if(y+ysize-1<ClipY1)return ;
	if(x>ClipX2)return ;
	if(y>ClipY2)return ;

	if(y<ClipY1)up_skip=ClipY1-y;
	if(y+ysize>ClipY2)down_skip=y+ysize-ClipY2-1;
	if(x<ClipX1)left_skip=ClipX1-x;
	if(x+xsize>ClipX2)right_skip=x+xsize-ClipX2-1;

	for(i=0;i<ysize-down_skip;i++)
	{
		j=0;
		while(j<xsize)
		{
			if(i>=up_skip && j>=left_skip && j<xsize-right_skip)
			{
				if(image[index] != COLOR_TRANS)	vidptr[j]=image[index];
			}
			
			j++; index++;
		}
		vidptr+=SurfaceWidth;
	}

}




// image에서 sx1, sy1부분부터 ~ sxsize, sysize만큼까지를 찍는다. 
void cltGrp::PutSpr8(int x,int y,int xsize,int ysize, int sx1,int sy1, int sxsize, int sysize, unsigned char *image)
{
	int i,j,skipbytes;
	UI08 *vidptr;
	unsigned char* temp;

    temp=&image[sx1+sy1*xsize];

	vidptr=Screen8+y*SurfaceWidth + x;
	skipbytes = SurfaceWidth-sxsize;

	 for(i=0;i<sysize;i++)
	 {
	   for(j=0;j<sxsize;j++)
	   {
		 if(*temp!=COLOR_TRANS) *vidptr=*temp;
		 temp++;
		 vidptr++;
	   }
	   temp+=(xsize-sxsize);
	   vidptr+=skipbytes;
	}
}


// 랜던함 개수를 얻어온다. 
void cltGrp::GetSprRandPos8TH(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	SI32 maxnum = *pgetnum;
	*pgetnum = 0;

	for(i = 0;i < ysize;i++)
	{
		j = 0;

		while(j < xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				if(rand() % 100 == 0 && (*pgetnum) < maxnum && i > 115)
				{
					ppoint[*pgetnum].x = x + j;
					ppoint[*pgetnum].y = y + i;
					(*pgetnum)++;
				}

				index++;
				j++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}

// 랜던함 개수를 얻어온다. 
void cltGrp::GetSprRandPos8RTH(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	SI32 maxnum = *pgetnum;
	*pgetnum = 0;

	for(i = 0;i < ysize;i++)
	{
		j = 0;

		while(j < xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
				index++;
				j+=image[index];
				index++;
			}
			else
			{
				if(rand() % 100 == 0 && (*pgetnum) < maxnum && i > 115 )
				{
					ppoint[*pgetnum].x = x + xsize-j-1;
					ppoint[*pgetnum].y = y + i;
					(*pgetnum)++;
				}

				index++;
				j++;
			}
		}
		vidptr+=SurfaceWidth;
	}
}


// 랜던함 개수를 얻어온다. 
void cltGrp::GetSprRandPos8T(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	SI32 maxnum = *pgetnum;
	*pgetnum = 0;

	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
				if(rand() % 200 == 0 && (*pgetnum) < maxnum)
				{
					ppoint[*pgetnum].x = x + j;
					ppoint[*pgetnum].y = y + i;
					(*pgetnum)++;
				}

			   index++;
			   j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}

// 랜던함 개수를 얻어온다. 
void cltGrp::GetSprRandPos8RT(int x, int y, int xsize, int ysize, unsigned char *image, SI32* pgetnum, POINT* ppoint)
{
	int i, j,index=0;
	UI08 *vidptr=Screen8+y*SurfaceWidth + x;

	SI32 maxnum = *pgetnum;
	*pgetnum = 0;

	 for(i = 0;i < ysize;i++)
	 {
		j = 0;

		while(j < xsize)
		{
			if(image[index]==COLOR_TRANS)
			{
			   index++;
			   j+=image[index];
			   index++;
			}
			else
			{
				if(rand() % 200 == 0 && (*pgetnum) < maxnum)
				{
					ppoint[*pgetnum].x = x + xsize-j-1;
					ppoint[*pgetnum].y = y + i;
					(*pgetnum)++;
				}

			   index++;
			   j++;
			}
		}
		vidptr+=SurfaceWidth;
	 }
}


HRESULT cltGrp::BltFast(DWORD surface,  DWORD dwX,  DWORD dwY,                            
                        LPDIRECTDRAWSURFACE7 lpDDSrcSurface,  LPRECT lpSrcRect, DWORD dwTrans )
{

	switch(surface)
	{

	   case SURFACE_PRIMARY:
		   if( lpDDSPrimary )
		     return lpDDSPrimary->BltFast(dwX, dwY, lpDDSrcSurface, lpSrcRect, dwTrans); 
		  break;

	   case SURFACE_BACK:
		   if( lpDDSBack )
		     return lpDDSBack->BltFast(dwX, dwY, lpDDSrcSurface, lpSrcRect, dwTrans); 
		  break;

	   default:
		   MsgBox("BltFast", "fsd3fdf");
		   break;
	}

	return 0;

}




static HRESULT CALLBACK EnumDisplayModesCallBack( LPDDSURFACEDESC2 pddsd, LPVOID Context )
{
	return clGrp.PushDisplayMode(pddsd);
}


BOOL cltGrp::ClearSurface(LPDIRECTDRAWSURFACE7 pSurface)
{
	DDBLTFX ddbltfx;
	ZeroMemory( &ddbltfx, sizeof(ddbltfx) );
	ddbltfx.dwSize      = sizeof(ddbltfx);
	ddbltfx.dwFillColor = 0;
	
	if (pSurface) 
	{
		return pSurface->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
	}
	
	return FALSE;
}


void cltGrp::UpdateScreen( HWND hwnd, LPDIRECTDRAWSURFACE7 pSurface, 
							SI32 xsize, SI32 ysize, SI32 xindent, SI32 yindent, 
						   SI32 srcstartx, SI32 srcstarty, bool bwaitrefresh,  SI32 screenXSize, SI32 screenYSize )
{
	 
	DWORD					dwTrans  = DDBLT_WAIT;
	LPDIRECTDRAWCLIPPER		pClipper = NULL;

	if(pSurface == NULL)
	{
		return ;
	}	

	// 제형이가 임시로 넣었다. 웅 웅 웅 
	if(IsIconic(hwnd) == TRUE) return;


	if( g_graphics.IsWindowed() ) {

		HDC destHdc;
		HDC hdc;

		RECT rect;
		GetClientRect( hwnd, &rect );

		// 얻어온 클라이언트 좌표 2개를 스크린 좌표로 변경한다.
		if(ClientToScreen( hwnd, (POINT*) &rect )	== FALSE) return; 
		if(ClientToScreen( hwnd, (POINT*) &rect + 1)== FALSE) return; 

		// 제형이가 임시로 넣었다.
		if(rect.bottom <-30000 ||rect.left<-30000 || rect.right <-30000 || rect.top<-30000) return;
		
		rect.left += xindent;
		rect.top  += yindent;
		
		// 찍어야 할 원본의 이미지를 항상 정해진 사이즈로 유지하게 한다. 
		//rect.right = rect.left + xsize;
		//rect.bottom = rect.top + ysize;


		RECT srcrect = rect;
		srcrect.left	= 0;
		srcrect.right = srcrect.left + xsize;
		
		srcrect.top		= 0;
		srcrect.bottom = srcrect.top + ysize;
		

		/*
		rect.left	= 26;
		rect.right = srcrect.left + 320 - 1;
		rect.top		= 351;
		rect.bottom = srcrect.top + 240 - 1;
		*/

		rect.left	= rect.left + 30;
		rect.right	= rect.left + xsize;
		rect.top	= rect.top + 350;
		rect.bottom = rect.top + ysize;

		if ( pSurface->GetDC( &hdc ) == DD_OK )
		{

			lpDDSConvertSurface->GetDC(&destHdc);
			BitBlt(destHdc, 0, 0, xsize, ysize,	hdc, 0, 0, SRCCOPY);
			
			pSurface->ReleaseDC(hdc);
			lpDDSConvertSurface->ReleaseDC(destHdc);
		}

		if(bwaitrefresh)
		{
			lpDD7->WaitForVerticalBlank( DDWAITVB_BLOCKBEGIN, NULL );
		}	
		
		lpDDSPrimary->Blt( &rect, clGrp.lpDDSConvertSurface, &srcrect, DDBLT_WAIT, NULL );		


	} else {

		RECT rect;
	
		SetRect( &rect, 0, 0, screenXSize, screenYSize );

		RECT srcrect = rect;

		srcrect.left += srcstartx;
		srcrect.top += srcstarty;
		srcrect.right += srcstartx;
		srcrect.bottom += srcstarty;

/*
		// FLIP방식
		lpDDSConvertSurface->Blt( &rect, pSurface, &srcrect, DDBLT_WAIT, NULL );
		lpDDSPrimary->Flip( NULL, 0 );
*/

		lpDDSPrimary->Blt( &rect, pSurface, &srcrect, DDBLT_WAIT, NULL );
	}

}	

