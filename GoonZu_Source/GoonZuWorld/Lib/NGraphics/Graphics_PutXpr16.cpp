#include "graphics.h"

#include "nutil.h"

void NGraphics::PutXpr16T( int x, int y, int width, int height, RGB16 *pSrcImage, RGB16 colorkey )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	RGB16 *pdest;
	RGB16 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;


#ifndef _GRAPHICS_ASMCODE

	int i, j;

	for( i = 0; i < height; ++i ) {

		j = 0;

		while( j < width ) {

			if( *psrc == colorkey ) {
				
				++psrc;
				j += *psrc;
				++psrc;
			
			} else {

				memcpy( &pdest[ j ], psrc + 1, *psrc << 1);
				
				j += *psrc;
				psrc += *psrc + 1;
			}
		}

		pdest += lPitch;
	}

#else

	int i;

	lPitch = lPitch << 1;

		__asm
		{
			// eax, edi : dest
			// esi : psrc
			
			// ebx : x itor
			// edx : colorkey

			mov		esi, psrc
			mov		eax, pdest	
			
			xor		edx, edx
			mov		dx, colorkey

			// for( i = 0; i < height; ++i )
			mov		i, 0

			mov		ecx, height
			cmp		ecx, 0
			jle		end_yloop

yloop:			
			// ebx = 0
			xor		ebx, ebx							

			// while( ebx < width )
xloop:
			cmp		ebx, width
			jge		end_xloop

			// if( *psrc == colorkey )
			cmp		word ptr [esi], dx
			jne		putpixel

			// ++psrc;
			add		esi, 2

			// ebx += *psrc;
			xor		ecx, ecx
			mov		cx, word ptr [esi]
			add		ebx, ecx
			
			// ++psrc;
			add		esi, 2

			jmp		xloop

putpixel:
			cld
			xor		ecx, ecx
			
			mov		cx, word ptr [esi]
			add		esi, 2

			lea		edi, [eax+ebx*2]

			// ebx += ecx;
			add		ebx, ecx			

			rep		movsw

			jmp		xloop

end_xloop:

			add		eax, lPitch

			mov		ecx, i
			add		ecx, 1
			mov		i, ecx

			cmp		ecx, height
			jge		end_yloop
			
			jmp		yloop

end_yloop:

		}



#endif

}




void NGraphics::PutXpr16CT( int x, int y, int width, int height, RGB16 *pSrcImage, RGB16 colorkey )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	SI32 x2 = 0, y2 = 0;
	SI32 trim_x = 0, trim_x2 =0 , trim_y1 = 0, trim_y2 = 0;
	SI32 clip_x1 = 0, clip_y1 = 0, clip_x2 = 0, clip_y2 = 0;

	x2 = x + width - 1;
	y2 = y + height - 1;

	clip_x1 = m_rcClip.left;
	clip_y1 = m_rcClip.top;
	clip_x2 = m_rcClip.right;
	clip_y2 = m_rcClip.bottom;

	if( x > clip_x2 || x2 < clip_x1 || 
		y > clip_y2 || y2 < clip_y1 ) return ;

	if( y < clip_y1 ) trim_y1 = clip_y1 - y;
	if( y2 > clip_y2 ) trim_y2 = clip_y2 - y + 1;
	else trim_y2 = height;

	RGB16 *pdest;
	RGB16 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;

//#ifndef _GRAPHICS_ASMCODE

	int i, j, k;

	if( x < clip_x1 ) {

		trim_x = clip_x1 - x;

		if( x2 > clip_x2 ) trim_x2 = width - x2 + clip_x2;
		else trim_x2 = width;

		if( y < clip_y1 ) {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == colorkey ) {					
						++psrc; j += *psrc; ++psrc;
					} else {
						k = j; j += *psrc; ++psrc;
						for( ; k < j; ++k ) {
							if( i >= trim_y1 && k >= trim_x  && k <= trim_x2 ) pdest[ k ] = *psrc;
							++psrc;
						}
					}
				}
				pdest += lPitch;
			}
		} else {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == colorkey ) {					
						++psrc; j += *psrc; ++psrc;
					} else {
						k = j; j += *psrc; ++psrc;
						for( ; k < j; ++k ) {
							if( k >= trim_x && k <= trim_x2 ) pdest[ k ] = *psrc;
							++psrc;
						}
					}
				}
				pdest += lPitch;
			}
		}

	} else {

		if( x2 > clip_x2 ) trim_x = clip_x2 - x;
		else trim_x = width;

		if( y < clip_y1 ) {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == colorkey ) {
						++psrc; j += *psrc; ++psrc;				
					} else {
						k = j; j += *psrc; ++psrc;
						for( ; k < j; ++k ) {
							if( i >= trim_y1 && k <= trim_x ) pdest[ k ] = *psrc;
							++psrc;
						}
					}
				}
				pdest += lPitch;
			}
		} else {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == colorkey ) {
						++psrc; j += *psrc; ++psrc;				
					} else {
						k = j; j += *psrc; ++psrc;
						for( ; k < j; ++k ) {
							if( k <= trim_x ) pdest[ k ] = *psrc;
							++psrc;
						}
					}
				}
				pdest += lPitch;
			}
		}

	}

//#else


//#endif

}
