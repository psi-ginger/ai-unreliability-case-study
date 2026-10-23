#include "graphics.h"

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "..\\nutil\\nutil.h"

RGB16 NGraphics::MakeRGB16( UI16 r, UI16 g, UI16 b )
{
	if( m_rgbmode == RGB16_565 ) {

		return ((( r << 8 ) & RGB16_565_RMASK ) | 
			    (( g << 3 ) & RGB16_565_GMASK ) |
			    (( b >> 3 ) & RGB16_565_BMASK ) );
	}

	return ((( r << 7 ) & RGB16_555_RMASK ) | 
			(( g << 2 ) & RGB16_555_GMASK ) |
			(( b >> 3 ) & RGB16_555_BMASK ) );
}

void NGraphics::GetRGB16( UI16 c, UI08 *r, UI08 *g, UI08 *b )
{
	
	if( m_rgbmode == RGB16_565 ) {

		*r = (c & RGB16_565_RMASK) >> 8;
		*g = (c & RGB16_565_GMASK) >> 3;
		*b = (c & RGB16_565_BMASK) << 3;

	} else {

		*r = (c & RGB16_555_RMASK) >> 7;
		*g = (c & RGB16_555_GMASK) >> 2;
		*b = (c & RGB16_555_BMASK) << 3;
	}
}


BOOL NGraphics::Lock()
{
	return LockSurface( m_pddsBackBuffer );
}


void NGraphics::Unlock()
{
	UnlockSurface( m_pddsBackBuffer );
}


BOOL NGraphics::LockSurface( LPDIRECTDRAWSURFACE7 lpdds )
{

    HRESULT        hr;

    ZeroMemory( &m_ddsd, sizeof(m_ddsd) );
    m_ddsd.dwSize = sizeof(m_ddsd);

    // Lock the surface to directly write to the surface memory 
    if( FAILED( hr = lpdds->Lock( NULL, &m_ddsd, DDLOCK_WAIT, NULL ) ) )
        return FALSE;

    // Get a pointer into the memory starting at ddsd.lpSurface.
    m_pImage = m_ddsd.lpSurface;
	m_lPitch = m_ddsd.lPitch;

	m_pddsLockedSurface = lpdds;

	return TRUE;
}


void NGraphics::UnlockSurface( LPDIRECTDRAWSURFACE7 lpdds )
{
	lpdds->Unlock( NULL );

	m_pddsLockedSurface = NULL;
}




//-----------------------------------------------------------------------------
// Name: NGraphics()
// Desc:
//-----------------------------------------------------------------------------
NGraphics::NGraphics()
{
	int i;

	m_pDD = NULL;
	m_pddsFrontBuffer = NULL;
	m_pddsBackBuffer = NULL;
	m_pddsConvertBuffer = NULL;

	m_pddPalette = NULL;

	// Color Table
	m_pColorTable = NULL;
	m_pCurrentColorTablePtr = NULL;

	m_siMaxColorTableNum = 0;	

	// Alpha Table
	for( i = 0; i < MAX_ALPHA_LEVEL; ++i ) {
		m_pAlphaTable[ i ] = NULL;
	}

	// EffectF Table
	for( i = 0; i < MAX_EFFECTF; ++i ) {
		m_pEffectFTable[ i ] = NULL;
	}
	
	// Effect Table
	ZeroMemory( m_pEffectTable, sizeof( m_pEffectTable ) );

	_init();
}


//-----------------------------------------------------------------------------
// Name: ~NGraphics()
// Desc:
//-----------------------------------------------------------------------------
NGraphics::~NGraphics()
{
	DestroyObjects();
}


void NGraphics::_init()
{

	m_bLocked = FALSE;

	m_pImage = NULL;

	m_width = 0;
	m_height = 0;

	m_lPitch = 0;
	m_dwBPP = 0;

	// clipping
	ZeroMemory( &m_rcClip, sizeof( m_rcClip ) );

	// 16 bit
	m_rgbmode = 0;

	m_bSkipDraw = FALSE;
}



//-----------------------------------------------------------------------------
// Name: DestroyObjects()
// Desc:
//-----------------------------------------------------------------------------
void NGraphics::DestroyObjects()
{
	int i;

	_release( m_pddsConvertBuffer );
	_release( m_pddsBackBuffer );
    _release( m_pddsFrontBuffer );
	_release( m_pddPalette );

    if( m_pDD ) m_pDD->SetCooperativeLevel( m_hWnd, DDSCL_NORMAL );

    _release( m_pDD );

	_delete_array( m_pColorTable );

	for( i = 0; i < MAX_ALPHA_LEVEL; ++i ) {
		_delete_array( m_pAlphaTable[ i ] );
	}

	for( i = 0; i < MAX_EFFECTF; ++i ) {
		_delete_array( m_pEffectFTable[ i ] );
	}

}

//-----------------------------------------------------------------------------
// 초기화 함수 
//-----------------------------------------------------------------------------
BOOL NGraphics::CreateGraphics( HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, BOOL bWindowed )
{
	BOOL ret = FALSE;

	if( bWindowed ) {
		ret = CreateWindowedGraphics( hWnd, dwWidth, dwHeight, dwBPP );

	} else {

		ret = CreateFullScreenGraphics( hWnd, dwWidth, dwHeight, dwBPP );
	}

	return ret;
}


//-----------------------------------------------------------------------------
// 전체 화면 모드 초기화
//-----------------------------------------------------------------------------
BOOL NGraphics::CreateFullScreenGraphics( HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwBackXSize, DWORD dwBackYSize )
{
	if( dwBackXSize == 0 || dwBackYSize == 0 ) {
		dwBackXSize = dwWidth;
		dwBackYSize = dwHeight;
	}


	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	_init();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) )
		return FALSE;

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
	if( FAILED(hr) ) return FALSE;

	DWORD dwStyle;

	dwStyle = GetWindowStyle( hWnd );
	dwStyle &= ~WS_POPUP;
	dwStyle &= ~(WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX);
	
	SetWindowLong( hWnd, GWL_STYLE, dwStyle );


	// Set the display mode
	if( FAILED( m_pDD->SetDisplayMode( dwWidth, dwHeight, dwBPP, 0, 0 ) ) ) return FALSE;

	// Create primary surface (with convertbuffer attached)
	DDSURFACEDESC2 ddsd;
	
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwBackBufferCount = 1;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) )
		return FALSE;

	// Get a pointer to the back buffer
	DDSCAPS2 ddscaps;
	
	ZeroMemory( &ddscaps, sizeof( ddscaps ) );
	
	ddscaps.dwCaps = DDSCAPS_BACKBUFFER;

	if( FAILED( hr = m_pddsFrontBuffer->GetAttachedSurface( &ddscaps, &m_pddsConvertBuffer ) ) ) 
		return FALSE;

	m_pddsConvertBuffer->AddRef();

	m_pDD->GetDisplayMode( &ddsd );

	if( dwBPP == 16 ) {
		if( ddsd.ddpfPixelFormat.dwRBitMask == RGB16_565_RMASK ) {
			m_rgbmode = RGB16_565;
		} else {
			m_rgbmode = RGB16_555;
		}
	}

	// Create the backbuffer surface
	ZeroMemory( &ddsd, sizeof( ddsd ) );

	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	// Set pixel format
	ZeroMemory(&(ddsd.ddpfPixelFormat),sizeof(DDPIXELFORMAT));

	ddsd.dwWidth = dwBackXSize;
	ddsd.dwHeight = dwBackYSize;

	switch( dwBPP ) {
	case 8:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 8;	
		}
		break;

	case 16:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 16;	

			if( m_rgbmode == RGB16_565 ) {

				ddsd.ddpfPixelFormat.dwRBitMask = RGB16_565_RMASK;
				ddsd.ddpfPixelFormat.dwGBitMask = RGB16_565_GMASK;
				ddsd.ddpfPixelFormat.dwBBitMask = RGB16_565_BMASK;
			
			} else {

				ddsd.ddpfPixelFormat.dwRBitMask = RGB16_555_RMASK;
				ddsd.ddpfPixelFormat.dwGBitMask = RGB16_555_GMASK;
				ddsd.ddpfPixelFormat.dwBBitMask = RGB16_555_BMASK;
			
			}
		}
		break;

	case 24:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 24;	

			ddsd.ddpfPixelFormat.dwRBitMask = 0xFF0000; 
			ddsd.ddpfPixelFormat.dwGBitMask = 0x00FF00;
			ddsd.ddpfPixelFormat.dwBBitMask = 0x0000FF;
		}
		break;

	case 32:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 32;	

			ddsd.ddpfPixelFormat.dwRBitMask = 0x00FF0000; 
			ddsd.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
			ddsd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
		}
		break;
	}


	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) ) {
		return FALSE;
	}

	m_hWnd = hWnd;
	m_bWindowed = FALSE;

	m_dwBPP = dwBPP;
	
	UpdateBounds();

	Lock();
	Unlock();

	SetClipFullScreen();

	return TRUE;
}


//-----------------------------------------------------------------------------
// 윈도우 모드 초기화
//-----------------------------------------------------------------------------
BOOL NGraphics::CreateWindowedGraphics( HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, DWORD dwBackXSize, DWORD dwBackYSize )
{
	if( dwBackXSize == 0 || dwBackYSize == 0 ) {
		dwBackXSize = dwWidth;
		dwBackYSize = dwHeight;
	}

	HRESULT hr;

	// Cleanup anything from a previous call
	DestroyObjects();

	_init();

	// DDraw stuff begins here
	if( FAILED( hr = DirectDrawCreateEx( NULL, (VOID**)&m_pDD, IID_IDirectDraw7, NULL ) ) ) {
		return FALSE;
	}

	// Set cooperative level
	hr = m_pDD->SetCooperativeLevel( hWnd, DDSCL_NORMAL );

	if( FAILED(hr) ) {
		return FALSE;
	}
/*
	RECT rcWork;
	RECT rc;
	DWORD dwStyle;

	// If we are still a WS_POPUP window we should convert to a normal app
	// window so we look like a windows app.
	dwStyle = GetWindowStyle( hWnd );
	dwStyle &= ~WS_POPUP;
	dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
	
	SetWindowLong( hWnd, GWL_STYLE, dwStyle );

	// Set window size
	SetRect( &rc, 0, 0, dwWidth, dwHeight );

	AdjustWindowRectEx( &rc, GetWindowStyle(hWnd), GetMenu(hWnd) != NULL, GetWindowExStyle(hWnd) );

	SetWindowPos( hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
				  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE );

	SetWindowPos( hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
				  SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE );

	//  Make sure our window does not hang outside of the work area
	SystemParametersInfo( SPI_GETWORKAREA, 0, &rcWork, 0 );
	
	GetWindowRect( hWnd, &rc );
	
	if( rc.left < rcWork.left ) rc.left = rcWork.left;
	if( rc.top  < rcWork.top ) rc.top  = rcWork.top;
	
	SetWindowPos( hWnd, NULL, rc.left, rc.top, 0, 0,
				  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );
*/
	LPDIRECTDRAWCLIPPER pcClipper;

	DDSURFACEDESC2 ddsd;

	// Create the primary surface
	ZeroMemory( &ddsd, sizeof( ddsd ) );
	
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if( FAILED( m_pDD->CreateSurface( &ddsd, &m_pddsFrontBuffer, NULL ) ) ) {
		return FALSE;
	}
	
	// Create the convert surface
	ZeroMemory( &ddsd, sizeof( ddsd ) );

	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = dwWidth;
	ddsd.dwHeight = dwHeight;

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsConvertBuffer, NULL ) ) ) {
/*
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

		if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsConvertBuffer, NULL ) ) ) {
			return FALSE;
		}
*/
		return FALSE;
	}

	m_pDD->CreateSurface( &ddsd, &m_pddsConvertBuffer, NULL );

	// Create the backbuffer surface
	ZeroMemory( &ddsd, sizeof( ddsd ) );

	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	// Set pixel format
	ZeroMemory(&(ddsd.ddpfPixelFormat),sizeof(DDPIXELFORMAT));

	ddsd.dwWidth = dwBackXSize;
	ddsd.dwHeight = dwBackYSize;

	switch( dwBPP ) {
	case 8:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB | DDPF_PALETTEINDEXED8;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 8;	
		}
		break;

	case 16:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 16;	

			
			ddsd.ddpfPixelFormat.dwRBitMask = RGB16_565_RMASK;
			ddsd.ddpfPixelFormat.dwGBitMask = RGB16_565_GMASK;
			ddsd.ddpfPixelFormat.dwBBitMask = RGB16_565_BMASK;

			m_rgbmode = RGB16_565; 
			
/*
			ddsd.ddpfPixelFormat.dwRBitMask = RGB16_555_RMASK;
			ddsd.ddpfPixelFormat.dwGBitMask = RGB16_555_GMASK;
			ddsd.ddpfPixelFormat.dwBBitMask = RGB16_555_BMASK;

			m_rgbmode = RGB16_555;
*/
		}
		break;

	case 24:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 24;	

			ddsd.ddpfPixelFormat.dwRBitMask = 0xFF0000; 
			ddsd.ddpfPixelFormat.dwGBitMask = 0x00FF00;
			ddsd.ddpfPixelFormat.dwBBitMask = 0x0000FF;
		}
		break;

	case 32:
		{
			ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
			ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
			ddsd.ddpfPixelFormat.dwRGBBitCount = 32;	

			ddsd.ddpfPixelFormat.dwRBitMask = 0x00FF0000; 
			ddsd.ddpfPixelFormat.dwGBitMask = 0x0000FF00;
			ddsd.ddpfPixelFormat.dwBBitMask = 0x000000FF;
		}
		break;
	}

	if( FAILED( hr = m_pDD->CreateSurface( &ddsd, &m_pddsBackBuffer, NULL ) ) ) {
		return FALSE;
	}

	if( FAILED( hr = m_pDD->CreateClipper( 0, &pcClipper, NULL ) ) ) {
		return FALSE;
	}

	if( FAILED( hr = pcClipper->SetHWnd( 0, hWnd ) ) ) {

		pcClipper->Release();
		return FALSE;
	}

	if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pcClipper ) ) ) {
		
		pcClipper->Release();
		return FALSE;
	}

	// Done with clipper
	pcClipper->Release();

	m_hWnd = hWnd;
	m_bWindowed = TRUE;

	m_dwBPP = dwBPP;

	m_width = dwWidth;
	m_height = dwHeight;

	UpdateBounds();

	Lock();
	Unlock();

	SetClipFullScreen();

	return TRUE;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::CreateSurface( NSurface** ppSurface, DWORD dwWidth, DWORD dwHeight )
{
    if( m_pDD == NULL) return E_POINTER;
    if( ppSurface == NULL ) return E_INVALIDARG;

    HRESULT hr;
    DDSURFACEDESC2 ddsd;
    
	ZeroMemory( &ddsd, sizeof( ddsd ) );
    
	ddsd.dwSize = sizeof( ddsd );
    ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT; 
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
    ddsd.dwWidth = dwWidth;
    ddsd.dwHeight = dwHeight;

    (*ppSurface) = new NSurface();

    if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) ) {

        delete (*ppSurface);
        return hr;
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: NGraphics::CreateSurfaceFromText()
// Desc: Creates a DirectDrawSurface from a text string using hFont or the default 
//       GDI font if hFont is NULL.
//-----------------------------------------------------------------------------
HRESULT NGraphics::CreateSurfaceFromText( NSurface** ppSurface, HFONT hFont, TCHAR* strText, 
                                         COLORREF crBackground, COLORREF crForeground )
{
	HDC hDC = NULL;
	LPDIRECTDRAWSURFACE7 pDDS = NULL;

	HRESULT hr;
	DDSURFACEDESC2 ddsd;
	SIZE sizeText;

	if( m_pDD == NULL || strText == NULL || ppSurface == NULL )
		return E_INVALIDARG;

	*ppSurface = NULL;

	hDC = GetDC( NULL );

	if( hFont ) SelectObject( hDC, hFont );

	GetTextExtentPoint32( hDC, strText, _tcslen(strText), &sizeText );
	ReleaseDC( NULL, hDC );

	// Create a DirectDrawSurface for this bitmap
	ZeroMemory( &ddsd, sizeof(ddsd) );
	
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth = sizeText.cx;
	ddsd.dwHeight = sizeText.cy;

	(*ppSurface) = new NSurface();

	if( FAILED( hr = (*ppSurface)->Create( m_pDD, &ddsd ) ) )
	{
		delete (*ppSurface);
		return hr;
	}

	if( FAILED( hr = (*ppSurface)->DrawText( hFont, strText, 0, 0, 
											 crBackground, crForeground ) ) )
		return hr;

	return S_OK;
}




//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::Present()
{
    HRESULT hr = NULL;

    if( NULL == m_pddsFrontBuffer && NULL == m_pddsBackBuffer ) return E_POINTER;

    while( 1 ) {

        if( m_bWindowed ) {

			HDC hdcDest, hdcSrc;
			
			if( m_pddsBackBuffer->GetDC( &hdcSrc ) == DD_OK ) {

				m_pddsConvertBuffer->GetDC( &hdcDest );

				BitBlt( hdcDest, 0, 0, m_width, m_height, hdcSrc, 0, 0, SRCCOPY);

				m_pddsBackBuffer->ReleaseDC( hdcSrc );
				m_pddsConvertBuffer->ReleaseDC( hdcDest );

				hr = m_pddsFrontBuffer->Blt( &m_rcWindow, m_pddsConvertBuffer, NULL, DDBLT_WAIT, NULL );
				RECT rect;
				SetRect( &rect, 0, 0, 100, 100 );
				//hr = m_pddsFrontBuffer->BltFast( 0, 0, m_pddsConvertBuffer, &rect, DDBLTFAST_WAIT );
/*
				m_pddsFrontBuffer->GetDC( &hdcDest );

				BitBlt( hdcDest, m_rcWindow.left, m_rcWindow.top, m_rcWindow.right, m_rcWindow.bottom, hdcSrc, 0, 0, SRCCOPY);

				m_pddsBackBuffer->ReleaseDC( hdcSrc );
				m_pddsFrontBuffer->ReleaseDC( hdcDest );
*/
			}

//			hr = m_pddsFrontBuffer->Blt( &m_rcWindow, m_pddsBackBuffer, NULL, DDBLT_WAIT, NULL );
			
		}
        else
		{
            hr = m_pddsFrontBuffer->Flip( NULL, 0 );
			//hr = m_pddsFrontBuffer->Blt( &m_rcWindow, m_pddsBackBuffer, NULL, DDBLT_WAIT, NULL );
		}

        if( hr == DDERR_SURFACELOST ) {

            m_pddsFrontBuffer->Restore();
            m_pddsBackBuffer->Restore();
        }

        if( hr != DDERR_WASSTILLDRAWING ) return hr;
    }
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::ColorKeyBlt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds, RECT* prc )
{
	if( NULL == m_pddsBackBuffer ) return E_POINTER;

	return m_pddsBackBuffer->BltFast( x, y, pdds, prc, DDBLTFAST_SRCCOLORKEY );
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::Blt( DWORD x, DWORD y, LPDIRECTDRAWSURFACE7 pdds, RECT* prc, DWORD dwFlags )
{
	if( NULL == m_pddsBackBuffer ) return E_POINTER;

	return m_pddsBackBuffer->BltFast( x, y, pdds, prc, dwFlags );
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::Blt( DWORD x, DWORD y, NSurface* pSurface, RECT* prc )
{
    if( pSurface == NULL ) return E_INVALIDARG;

    if( pSurface->IsColorKeyed() )
        return Blt( x, y, pSurface->GetDDrawSurface(), prc, DDBLTFAST_SRCCOLORKEY );
    else
        return Blt( x, y, pSurface->GetDDrawSurface(), prc, 0L );
}

void NGraphics::PutSurface16T( int x, int y, LPDIRECTDRAWSURFACE7 pdds )
{
//	RECT rect;
//	SetRect( &rect, x, y, x + 200, y + 20 );
	
	m_pddsBackBuffer->BltFast( x, y, pdds, NULL, DDBLTFAST_SRCCOLORKEY );
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::Clear( DWORD dwColor )
{
    if( m_pddsBackBuffer == NULL ) return E_POINTER;

    // Erase the background
    DDBLTFX ddbltfx;

    ZeroMemory( &ddbltfx, sizeof(ddbltfx) );

    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = dwColor;

    return m_pddsBackBuffer->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}

HRESULT NGraphics::ClearSurface( LPDIRECTDRAWSURFACE7 lpdds, DWORD dwColor )
{

	if( lpdds == NULL ) return E_POINTER;

    // Erase the background
    DDBLTFX ddbltfx;

    ZeroMemory( &ddbltfx, sizeof(ddbltfx) );

    ddbltfx.dwSize = sizeof(ddbltfx);
    ddbltfx.dwFillColor = dwColor;

    return lpdds->Blt( NULL, NULL, NULL, DDBLT_COLORFILL, &ddbltfx );
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::SetPalette( LPDIRECTDRAWPALETTE pPalette )
{
	if( NULL == m_pddsFrontBuffer ) return E_POINTER;

	return m_pddsFrontBuffer->SetPalette( pPalette );
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::UpdateBounds()
{

	if( m_bWindowed )
	{		
		GetClientRect( m_hWnd, &m_rcWindow );

		
		RECT rect;
		rect.left	= m_rcWindow.left + 30;
		rect.right = rect.left + 320 - 1;
		rect.top		= m_rcWindow.top + 380;
		rect.bottom = rect.top + 240 - 1;
		

		ClientToScreen( m_hWnd, (POINT*)&m_rcWindow );
		ClientToScreen( m_hWnd, (POINT*)&m_rcWindow + 1 );

	}
	else
	{
		SetRect( &m_rcWindow, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) );
	}

	return S_OK;
}





//-----------------------------------------------------------------------------
// Name: NGraphics::InitClipper
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NGraphics::InitClipper()
{
	LPDIRECTDRAWCLIPPER pClipper;
	HRESULT hr;

	// Create a clipper when using GDI to draw on the primary surface 
	if( FAILED( hr = m_pDD->CreateClipper( 0, &pClipper, NULL ) ) ) return hr;

	pClipper->SetHWnd( 0, m_hWnd );

	if( FAILED( hr = m_pddsFrontBuffer->SetClipper( pClipper ) ) ) return hr;

	// We can release the clipper now since g_pDDSPrimary 
	// now maintains a ref count on the clipper
	_release( pClipper );

	return S_OK;
}


BOOL NGraphics::CreateSurface16( LPDIRECTDRAWSURFACE7 *ppSurface, DWORD dwWidth, DWORD dwHeight, BOOL bColorkeyed, RGB16 colorkey )
{
	*ppSurface = NULL;

	LPDIRECTDRAWSURFACE7	lpdds;

	DDSURFACEDESC2			ddsd;
	DDCOLORKEY				ddck;

	ZeroMemory( &ddsd, sizeof( ddsd ) );

	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;

	ddsd.dwWidth = dwWidth;
	ddsd.dwHeight = dwHeight;

	ZeroMemory(&(ddsd.ddpfPixelFormat),sizeof(DDPIXELFORMAT));

	ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
	ddsd.ddpfPixelFormat.dwFlags = DDPF_RGB;
	ddsd.ddpfPixelFormat.dwRGBBitCount = 16;	

	if( m_rgbmode == RGB16_565 ) {

		ddsd.ddpfPixelFormat.dwRBitMask = RGB16_565_RMASK;
		ddsd.ddpfPixelFormat.dwGBitMask = RGB16_565_GMASK;
		ddsd.ddpfPixelFormat.dwBBitMask = RGB16_565_BMASK;
	} else {
		ddsd.ddpfPixelFormat.dwRBitMask = RGB16_555_RMASK;
		ddsd.ddpfPixelFormat.dwGBitMask = RGB16_555_GMASK;
		ddsd.ddpfPixelFormat.dwBBitMask = RGB16_555_BMASK;
	}

	if( FAILED( m_pDD->CreateSurface( &ddsd, &lpdds, NULL ) ) ) return FALSE;

	if( bColorkeyed ) {

		ddck.dwColorSpaceLowValue = colorkey;
		ddck.dwColorSpaceHighValue = colorkey;

		if( FAILED( lpdds->SetColorKey( DDCKEY_SRCBLT, &ddck ) ) ) {

			lpdds->Release();

			return FALSE;
		}
	}

	*ppSurface = lpdds;

	return TRUE;
}

void NGraphics::EncodeXpr( RGB16 *pDestImage, int width, int height, RGB16 colorkey, int *size )
{

	BOOL bTransRep;
	UI16 repcnt;

	int i, j;

	LONG lPitch;

	lPitch = m_lPitch >> 1;

	RGB16 *pdest = pDestImage;

	RGB16 *psrc = (RGB16*)m_pImage;

	RGB16 *psrcmark;
	
	for( i = 0; i < height; ++i ) {

		if( *psrc == colorkey ) {
			bTransRep = TRUE;
		} else {
			bTransRep = FALSE;
			psrcmark = psrc;
		}

		repcnt = 1;

		for( j = 1; j < width; ++j ) { 

			// 투명반복 
			if( bTransRep ) {

				if( psrc[ j ] == colorkey ) {
					++repcnt;
				} else {

					// 투명 정보 쓰기 
					*pdest = colorkey;
					++pdest;

					*pdest = repcnt;
					++pdest;

					// 유효색상 반복으로 전환
					bTransRep = FALSE;
					repcnt = 1;
					psrcmark = &psrc[ j ];
				}
			} else {
				
				if( psrc[ j ] == colorkey ) {

					// 유효 색상 정보 쓰기 
					*pdest = repcnt;
					++pdest;

					memcpy( pdest, psrcmark, repcnt << 1 );
					pdest += repcnt;

					// 투명 반복으로 전환
					bTransRep = TRUE;
					repcnt = 1;

				} else {

					++repcnt;
				}
			}
		}

		if( bTransRep ) {

			*pdest = colorkey;
			++pdest;

			*pdest = repcnt;
			++pdest;
			
		} else {

			*pdest = repcnt;
			++pdest;

			memcpy( pdest, psrcmark, repcnt << 1 );
			pdest += repcnt;
		}

		psrc += lPitch;
	}


	*size = (int)( (UI08*)pdest - (UI08*)pDestImage );

}

BOOL NGraphics::IsInClipArea( int x1, int y1, int x2, int y2 )
{
	if( x1 >= m_rcClip.left && 
		x2 <= m_rcClip.right &&
		y1 >= m_rcClip.top &&
		y2 <= m_rcClip.bottom ) return TRUE;
	
	return FALSE; 
}


BOOL NGraphics::LoadXpr( char *filename, XSpr *pXSpr )
{

	pXSpr->Destroy();

	HANDLE hFile = CreateFile( filename, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD dwFileSize;

	dwFileSize = GetFileSize( hFile, NULL );

	if( dwFileSize == INVALID_FILE_SIZE ||
		dwFileSize == 0 ) {

		CloseHandle( hFile );
		return FALSE;
	}

	// 헤더 읽기 
	DWORD dwRead;
	if( ReadFile( hFile, &pXSpr->m_header, sizeof( pXSpr->m_header ), &dwRead, NULL ) == FALSE ) {

		CloseHandle( hFile );
		return FALSE;
	}

	// 바디 읽기 ( 이미지 태그 + 이미지 데이터 )
	pXSpr->m_pBody = new unsigned char[ pXSpr->m_header.bodysize + 512 ];

	pXSpr->m_pImageTag = (XSPRImgTag *)pXSpr->m_pBody;
	pXSpr->m_pImage = pXSpr->m_pBody + sizeof( XSPRImgTag ) * pXSpr->m_header.imagenum;
	pXSpr->m_pPal = (unsigned short *)( (UI08*)pXSpr->m_pBody + pXSpr->m_header.bodysize );
	
	if( ReadFile( hFile, pXSpr->m_pBody, pXSpr->m_header.bodysize, &dwRead, NULL ) == FALSE ) {
		
		pXSpr->Destroy();
		CloseHandle( hFile );
		return FALSE;
	}

	if( dwRead != pXSpr->m_header.bodysize ) {
		
		pXSpr->Destroy();
		CloseHandle( hFile );
		return FALSE;
	}

	if( ReadFile( hFile, pXSpr->m_pPal, 512, &dwRead, NULL ) == FALSE ) {

		pXSpr->Destroy();
		CloseHandle( hFile );
		return FALSE;

	}

	if( dwRead != 512 ) {
		
		pXSpr->Destroy();
		CloseHandle( hFile );
		return FALSE;
	}


	CloseHandle( hFile );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
NSurface::NSurface()
{
    m_pdds = NULL;
    m_bColorKeyed = NULL;
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
NSurface::~NSurface()
{
    _release( m_pdds );
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NSurface::Create( LPDIRECTDRAWSURFACE7 pdds )
{
    m_pdds = pdds;

    if( m_pdds ) {

        m_pdds->AddRef();

        // Get the DDSURFACEDESC structure for this surface
        m_ddsd.dwSize = sizeof(m_ddsd);
        m_pdds->GetSurfaceDesc( &m_ddsd );
    }

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NSurface::Create( LPDIRECTDRAW7 pDD, DDSURFACEDESC2* pddsd )
{
    HRESULT hr;

    // Create the DDraw surface
    if( FAILED( hr = pDD->CreateSurface( pddsd, &m_pdds, NULL ) ) ) return hr;

    // Prepare the DDSURFACEDESC structure
    m_ddsd.dwSize = sizeof(m_ddsd);

    // Get the DDSURFACEDESC structure for this surface
    m_pdds->GetSurfaceDesc( &m_ddsd );

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NSurface::Destroy()
{
    _release( m_pdds );

    return S_OK;
}


//-----------------------------------------------------------------------------
// Name: NSurface::DrawBitmap()
// Desc: Draws a bitmap over an entire DirectDrawSurface, stretching the 
//       bitmap if nessasary
//-----------------------------------------------------------------------------
HRESULT NSurface::DrawBitmap( HBITMAP hBMP, DWORD dwBMPOriginX, DWORD dwBMPOriginY, 
							   DWORD dwBMPWidth, DWORD dwBMPHeight )
{
	HDC hDCImage;
	HDC hDC;
	BITMAP bmp;
	DDSURFACEDESC2 ddsd;
	HRESULT hr;

	if( hBMP == NULL || m_pdds == NULL ) return E_INVALIDARG;

	// Make sure this surface is restored.
	if( FAILED( hr = m_pdds->Restore() ) ) return hr;

	// Get the surface.description
	ddsd.dwSize  = sizeof(ddsd);
	m_pdds->GetSurfaceDesc( &ddsd );

	if( ddsd.ddpfPixelFormat.dwFlags == DDPF_FOURCC ) return E_NOTIMPL;

	// Select bitmap into a memoryDC so we can use it.
	hDCImage = CreateCompatibleDC( NULL );
	if( NULL == hDCImage ) return E_FAIL;

	SelectObject( hDCImage, hBMP );

	// Get size of the bitmap
	GetObject( hBMP, sizeof(bmp), &bmp );

	// Use the passed size, unless zero
	dwBMPWidth  = ( dwBMPWidth  == 0 ) ? bmp.bmWidth  : dwBMPWidth;     
	dwBMPHeight = ( dwBMPHeight == 0 ) ? bmp.bmHeight : dwBMPHeight;

	// Stretch the bitmap to cover this surface
	if( FAILED( hr = m_pdds->GetDC( &hDC ) ) ) return hr;

	StretchBlt( hDC, 0, 0, 
				ddsd.dwWidth, ddsd.dwHeight, 
				hDCImage, dwBMPOriginX, dwBMPOriginY,
				dwBMPWidth, dwBMPHeight, SRCCOPY );

	if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) ) return hr;

	DeleteDC( hDCImage );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: NSurface::DrawText()
// Desc: Draws a text string on a DirectDraw surface using hFont or the default
//       GDI font if hFont is NULL.  
//-----------------------------------------------------------------------------
HRESULT NSurface::DrawText( HFONT hFont, TCHAR* strText, DWORD dwOriginX, DWORD dwOriginY,
							 COLORREF crBackground, COLORREF crForeground )
{
    HDC     hDC = NULL;
    HRESULT hr;

    if( m_pdds == NULL || strText == NULL ) return E_INVALIDARG;

    // Make sure this surface is restored.
    if( FAILED( hr = m_pdds->Restore() ) ) return hr;

    if( FAILED( hr = m_pdds->GetDC( &hDC ) ) ) return hr;

    // Set the background and foreground color
    SetBkColor( hDC, crBackground );
    SetTextColor( hDC, crForeground );

    if( hFont ) SelectObject( hDC, hFont );

    // Use GDI to draw the text on the surface
    TextOut( hDC, dwOriginX, dwOriginY, strText, _tcslen(strText) );

    if( FAILED( hr = m_pdds->ReleaseDC( hDC ) ) ) return hr;

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: NSurface::ReDrawBitmapOnSurface()
// Desc: Load a bitmap from a file or resource into a DirectDraw surface.
//       normaly used to re-load a surface after a restore.
//-----------------------------------------------------------------------------
HRESULT NSurface::DrawBitmap( TCHAR* strBMP, 
                              DWORD dwDesiredWidth, DWORD dwDesiredHeight  )
{
	HBITMAP hBMP;
	HRESULT hr;

	if( m_pdds == NULL || strBMP == NULL ) return E_INVALIDARG;

	//  Try to load the bitmap as a resource, if that fails, try it as a file
	hBMP = (HBITMAP) LoadImage( GetModuleHandle(NULL), strBMP, IMAGE_BITMAP, 
								dwDesiredWidth, dwDesiredHeight, 
								LR_CREATEDIBSECTION );

	if( hBMP == NULL ) {

		hBMP = (HBITMAP) LoadImage( NULL, strBMP, IMAGE_BITMAP, 
									dwDesiredWidth, dwDesiredHeight, 
									LR_LOADFROMFILE | LR_CREATEDIBSECTION );
		
		if( hBMP == NULL ) return E_FAIL;
	}

	// Draw the bitmap on this surface
	if( FAILED( hr = DrawBitmap( hBMP, 0, 0, 0, 0 ) ) ) {

		DeleteObject( hBMP );
		return hr;
	}

	DeleteObject( hBMP );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: 
// Desc: 
//-----------------------------------------------------------------------------
HRESULT NSurface::SetColorKey( DWORD dwColorKey )
{
	if( m_pdds == NULL ) return E_POINTER;

	m_bColorKeyed = TRUE;

	DDCOLORKEY ddck;
	ddck.dwColorSpaceLowValue = ConvertGDIColor( dwColorKey );
	ddck.dwColorSpaceHighValue = ConvertGDIColor( dwColorKey );

	return m_pdds->SetColorKey( DDCKEY_SRCBLT, &ddck );
}


//-----------------------------------------------------------------------------
// Name: NSurface::ConvertGDIColor()
// Desc: Converts a GDI color (0x00bbggrr) into the equivalent color on a 
//       DirectDrawSurface using its pixel format.  
//-----------------------------------------------------------------------------
DWORD NSurface::ConvertGDIColor( COLORREF dwGDIColor )
{
	if( m_pdds == NULL ) return 0x00000000;

	COLORREF       rgbT;
	HDC            hdc;
	DWORD          dw = CLR_INVALID;
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	//  Use GDI SetPixel to color match for us
	if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK ) {

		rgbT = GetPixel(hdc, 0, 0);     // Save current pixel value
		SetPixel(hdc, 0, 0, dwGDIColor);       // Set our value
		m_pdds->ReleaseDC(hdc);
	}

	// Now lock the surface so we can read back the converted color
	ddsd.dwSize = sizeof(ddsd);

	hr = m_pdds->Lock( NULL, &ddsd, DDLOCK_WAIT, NULL );
	
	if( hr == DD_OK) {
		
		dw = *(DWORD *) ddsd.lpSurface; 
		
		if( ddsd.ddpfPixelFormat.dwRGBBitCount < 32 ) // Mask it to bpp
			dw &= ( 1 << ddsd.ddpfPixelFormat.dwRGBBitCount ) - 1;  

		m_pdds->Unlock(NULL);
	}

	//  Now put the color that was there back.
	if( dwGDIColor != CLR_INVALID && m_pdds->GetDC(&hdc) == DD_OK ) {

		SetPixel( hdc, 0, 0, rgbT );
		m_pdds->ReleaseDC(hdc);
	}

	return dw;    
}


//-----------------------------------------------------------------------------
// Name: NSurface::GetBitMaskInfo()
// Desc: Returns the number of bits and the shift in the bit mask
//-----------------------------------------------------------------------------
HRESULT NSurface::GetBitMaskInfo( DWORD dwBitMask, DWORD* pdwShift, DWORD* pdwBits )
{
	DWORD dwShift = 0;
	DWORD dwBits  = 0; 

	if( pdwShift == NULL || pdwBits == NULL ) return E_INVALIDARG;

	if( dwBitMask ) {
		
		while( (dwBitMask & 1) == 0 ) {

			dwShift++;
			dwBitMask >>= 1;
		}
	}

	while( (dwBitMask & 1) != 0 ) {

		dwBits++;
		dwBitMask >>= 1;
	}

	*pdwShift = dwShift;
	*pdwBits  = dwBits;

	return S_OK;
}


XSpr::XSpr()
{
	m_pBody = NULL;

	m_pImageTag = NULL;
	m_pImage = NULL;
	m_pPal = NULL;
}

XSpr::~XSpr()
{
	Destroy();
}

void XSpr::Destroy()
{
	if( m_pBody ) delete [] m_pBody;
	m_pBody = NULL;

	m_pImageTag = NULL;
	m_pImage = NULL;
	m_pPal = NULL;
}


