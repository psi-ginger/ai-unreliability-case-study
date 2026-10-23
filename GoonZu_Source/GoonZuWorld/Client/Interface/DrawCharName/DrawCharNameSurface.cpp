#include "DrawCharNameSurface.h"
#include "../../Lib/NGraphics/Graphics.h"

CDrawCharNameSurface::CDrawCharNameSurface()
{
	m_lpSurface = NULL;




	if( g_graphics.CreateSurface16( &m_lpSurface, MAX_CHARNAME_SURFACE_WIDTH + 10, MAX_CHARNAME_SURFACE_HEIGHT + 10 ) == FALSE ) {
		MessageBox( NULL, TEXT("CreateOffScreen Error, CDrawCharNameSurface"), TEXT("fdsf"), MB_OK |MB_TOPMOST);
	}

	Initialize();
}

CDrawCharNameSurface::~CDrawCharNameSurface()
{
	Destroy();


}

void CDrawCharNameSurface::Initialize()
{
	return;
}

void CDrawCharNameSurface::Destroy()
{
	if( m_lpSurface )
	{
		m_lpSurface->Release();
		m_lpSurface = NULL;
	}

	return;
}
	
LPDIRECTDRAWSURFACE7 CDrawCharNameSurface::GetCharNameSurface()
{
	return m_lpSurface;
}
