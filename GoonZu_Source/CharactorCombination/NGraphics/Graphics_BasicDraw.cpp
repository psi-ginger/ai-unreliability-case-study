#include "graphics.h"

#include "..\\nutil\\nutil.h"

void NGraphics::FillBox16( int x1, int y1, int x2, int y2, RGB16 color )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	int i, j, width;

	RGB16 *pdest;

	LONG lPitch;
	
	lPitch = m_lPitch >> 1;

	pdest = (RGB16 *)m_pImage + ( y1 * lPitch ) + x1;

	width = x2 - x1 + 1;

    for( i = y1; i <= y2; ++i ) {
		for( j = 0; j < width; ++j ) {
			pdest[ j ] = color;
		}

		pdest += lPitch;
	}

}

void NGraphics::FillBox16C( int x1, int y1, int x2, int y2, RGB16 color )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	if( IsInClipArea( x1, y1, x2, y2 ) ) {
		FillBox16( x1, y1, x2, y2, color );
		return;
	}

	SI32 clip_x1, clip_y1, clip_x2, clip_y2;

	clip_x1 = m_rcClip.left;
	clip_y1 = m_rcClip.top;
	clip_x2 = m_rcClip.right;
	clip_y2 = m_rcClip.bottom;

	if( x1 > clip_x2 || x2 < clip_x1 || 
		y1 > clip_y2 || y2 < clip_y1 ) return ;

	if( x1 < clip_x1 ) x1 = clip_x1;
	if( y1 < clip_y1 ) y1 = clip_y1;
	if( x2 > clip_x2 ) x2 = clip_x2;
	if( y2 > clip_y2 ) y2 = clip_y2;

	int i, j, width;

	RGB16 *pdest;

	LONG lPitch;
	
	lPitch = m_lPitch >> 1;

	pdest = (RGB16 *)m_pImage + ( y1 * lPitch ) + x1;

	width = x2 - x1 + 1;

    for( i = y1; i <= y2; ++i ) {
		for( j = 0; j < width; ++j ) {
			pdest[ j ] = color;
		}

		pdest += lPitch;
	}

}

void NGraphics::Box16( int x1, int y1, int x2, int y2, RGB16 color )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	int i, width, height;

	RGB16 *pdest1, *pdest2;

	LONG lPitch;
	
	lPitch = m_lPitch >> 1;

	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	pdest1 = (RGB16 *)m_pImage + ( y1 * lPitch ) + x1;
	pdest2 = (RGB16 *)m_pImage + ( y2 * lPitch ) + x1;

	for( i = 0; i < width; ++i ) {
		pdest1[ i ] = color;
		pdest2[ i ] = color;
	}

	pdest2 = pdest1 + width - 1;

	for( i = 0; i < height; ++i ) {
		*pdest1 = color;
		*pdest2 = color;

		pdest1 += lPitch;
		pdest2 += lPitch;
	}

}

void NGraphics::Box16C( int x1, int y1, int x2, int y2, RGB16 color )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	if( IsInClipArea( x1, y1, x2, y2 ) ) {
		Box16( x1, y1, x2, y2, color );
		return;
	}

	SI32 clip_x1, clip_y1, clip_x2, clip_y2;

	clip_x1 = m_rcClip.left;
	clip_y1 = m_rcClip.top;
	clip_x2 = m_rcClip.right;
	clip_y2 = m_rcClip.bottom;

	if( x1 > clip_x2 || x2 < clip_x1 || 
		y1 > clip_y2 || y2 < clip_y1 ) return ;

	BOOL bX1 = 1, bX2 = 1, bY1 = 1, bY2 = 1;

	if( x1 < clip_x1 ) { bX1 = 0; x1 = clip_x1; }
	if( y1 < clip_y1 ) { bY1 = 0; y1 = clip_y1; }
	if( x2 > clip_x2 ) { bX2 = 0; x2 = clip_x2; }
	if( y2 > clip_y2 ) { bY2 = 0; y2 = clip_y2; }

	int i, width, height;

	RGB16 *pdest1, *pdest2;

	LONG lPitch;
	
	lPitch = m_lPitch >> 1;

	width = x2 - x1 + 1;
	height = y2 - y1 + 1;

	pdest1 = (RGB16 *)m_pImage + ( y1 * lPitch ) + x1;
	pdest2 = (RGB16 *)m_pImage + ( y2 * lPitch ) + x1;

	for( i = 0; i < width; ++i ) {
		if( bY1 ) pdest1[ i ] = color;
		if( bY2 ) pdest2[ i ] = color;
	}

	pdest2 = pdest1 + width - 1;

	for( i = 0; i < height; ++i ) {
		if( bX1 ) *pdest1 = color;
		if( bX2 ) *pdest2 = color;

		pdest1 += lPitch;
		pdest2 += lPitch;
	}

}


void NGraphics::Line16( int x1, int y1, int x2, int y2, RGB16 color )
{
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;

	if( !IsInClipArea( min(x1,x2), min(y1,y2), max(x1,x2), max(y1,y2) ) ) return;

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
	delta_x= abs(delta_x);
	delta_y= abs(delta_y);
	if(delta_x > delta_y)distance=delta_x;
	else distance=delta_y;

	// 선을 그린다. 
	for(t=0;t<=distance+1;t++)
	{
		*((RGB16 *)m_pImage + ( y1 * lPitch ) + x1) = color;

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

void NGraphics::HLine16( int x, int y, int l, RGB16 color )
{


}

void NGraphics::VLine16( int x, int y, int l, RGB16 color )
{


}