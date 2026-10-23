#include "Graphics.h"

#include <tchar.h>
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>

#include "Helper.h"


void NGraphics::PutSpr16M( int x, int y, UI08 *pSrcImage, int width, int height, UI08 *pDestImage )
{

	RGB16 *pColorTable;
	RGB16 *pdest;
	UI08 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	if( pDestImage ) pdest = (RGB16 *)pDestImage + ( y * lPitch ) + x;
	else pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;
	
	pColorTable = (RGB16 *)m_pCurrentColorTablePtr;

#ifndef _GRAPHICS_ASMCODE

	int i, j, k;
	int m;

	for( i = 0; i < height; ++i ) {

		j = 0;

		while( j < width ) {

			if( *psrc == COLOR_TRANS ) {
				
				++psrc;
				j += *psrc;
				++psrc;
			
			} else {

				m = j;
				j += *psrc;
				++psrc;

				for( k = m; k < j; ++k ) {
					pdest[ k ] = pColorTable[ *psrc ];
					++psrc;
				}
			}
		}

		pdest += lPitch;
	}

#else

	int i, j;

	lPitch = lPitch << 1;

		__asm
		{
			// edi : dest
			// esi : color table
			
			// ebx : x itor
			// edx : psrc

			// cl : *psrc

			mov		edx, psrc
			mov		edi, pdest			
			mov		esi, pColorTable			

			// for( i = 0; i < height; ++i )
			mov		i, 0

			mov		eax, height
			cmp		eax, 0
			jle		end_yloop

yloop:			
			// j = 0
			mov		j, 0						

			// while( j < width )
xloop:
			mov		eax, j
			cmp		eax, width
			jge		end_xloop

			// cl = *psrc
			xor		ecx, ecx
			mov		cl, byte ptr [edx]
			
			// if( *psrc == COLOR_TRANS )
			cmp		ecx, COLOR_TRANS
			jne		putpixel

			// ++psrc;
			add		edx, 1

			// j += *psrc;
			mov		cl, byte ptr [edx]
			mov		eax, j
			add		eax, ecx
			mov		j, eax
			
			// ++psrc;
			add		edx, 1

			jmp		xloop

putpixel:
			// ebx = j
			// j += *psrc
			// ++psrc
			mov		ebx, j
			mov		eax, ebx
			add		eax, ecx
			mov		j, eax
			add		edx, 1

pp:
			// cl = *psrc
			mov		cl, byte ptr [edx]

			// pdest[ ebx ] = pColorTable[ *psrc ];
			mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
			mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
			
			// ++psrc;
			add		edx, 1

			// ++ebx
			add		ebx, 1

			cmp		ebx, j
			jl		pp
			
			jmp		xloop

end_xloop:

			add		edi, lPitch

			mov		eax, i
			add		eax, 1
			mov		i, eax

			cmp		eax, height
			jge		end_yloop
			
			jmp		yloop

end_yloop:

		}



#endif
}

void NGraphics::PutSpr16CM( int x, int y, UI08 *pSrcImage, int width, int height, UI08 *pDestImage )
{
#ifndef _GRAPHICS_ASMCODE



#else



#endif
}
