//-----------------------------------------------------------------------------
// 2005/09/20 ¿Ã¡¶øœ
//-----------------------------------------------------------------------------

#include "NDIB.h"
#include "NDraw.h"

unsigned short MakeRGB16( unsigned short r, unsigned short g, unsigned short b )
{
	return (( r << 8 ) & RGB16_565_RMASK ) |
		   (( g << 3 ) & RGB16_565_GMASK ) |
		   (( b >> 3 ) & RGB16_565_BMASK );
}

NDIB::NDIB()
{
	m_hBitmap = NULL;
	m_pBMI = NULL;
}

NDIB::~NDIB()
{
	if( m_hBitmap ) {
		DeleteObject( m_hBitmap );
		m_hBitmap = NULL;
	}
}

void NDIB::CreateDIB( HDC hdc, int width, int height )
{
	m_pBMI = (BITMAPINFO*) new BYTE[ sizeof( BITMAPINFOHEADER ) + sizeof( DWORD ) * 3 ];

	m_pBMI->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	m_pBMI->bmiHeader.biWidth = width;
	m_pBMI->bmiHeader.biHeight = height;
	m_pBMI->bmiHeader.biPlanes = 1;
	m_pBMI->bmiHeader.biBitCount = 16;
	m_pBMI->bmiHeader.biCompression = BI_BITFIELDS;
	m_pBMI->bmiHeader.biSizeImage = 0;
	m_pBMI->bmiHeader.biXPelsPerMeter = 0;
	m_pBMI->bmiHeader.biYPelsPerMeter = 0;
	m_pBMI->bmiHeader.biClrUsed = 0;
	m_pBMI->bmiHeader.biClrImportant = 0;

	DWORD *pMask = (DWORD*)m_pBMI->bmiColors;
	pMask[ 0 ] = (DWORD)RGB16_565_RMASK;
	pMask[ 1 ] = (DWORD)RGB16_565_GMASK;
	pMask[ 2 ] = (DWORD)RGB16_565_BMASK;

	m_hBitmap = ::CreateDIBSection( hdc, m_pBMI, DIB_RGB_COLORS, (void**)&m_pImage, NULL, NULL );

	m_width = width;
	m_height = height;


	unsigned short c = MakeRGB16( 0, 255, 0 );

	for( int i = 0; i < 400; ++i ) {
		m_pImage[ i ] = c;
	}
}

void NDIB::Put( HDC hdc, int x, int y )
{
	::StretchDIBits( hdc, x, y, m_width, m_height, 0, 0, m_width, m_height, m_pImage, m_pBMI, DIB_RGB_COLORS, SRCCOPY);
}