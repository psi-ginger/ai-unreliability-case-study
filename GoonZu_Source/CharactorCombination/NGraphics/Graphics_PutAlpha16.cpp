#include "graphics.h"

#include "..\\nutil\\nutil.h"

void NGraphics::PutAlphaB16T( int x, int y, int width, int height, UI08 *pSrcImage )
{
	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;


	RGB16** pAlphaTable;

	RGB16 *pdest;
	UI08 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;

	pAlphaTable = m_pAlphaTable;

#ifndef _GRAPHICS_ASMCODE

	UI16 alpha;

	int i, j;

	for( i = 0; i < height; ++i ) {

		j = 0;

		while( j < width ) {

			if( *psrc == COLOR_TRANS ) {
				
				++psrc;
				j += *psrc;
				++psrc;
			
			} else {
				alpha = (*psrc >> 4);

				pdest[ j ] = m_pAlphaTable[ 16 - alpha ][ pdest[ j ] ];
				++psrc;
				++j;
			}
		}

		pdest += lPitch;
	}


#else
	int i;

	lPitch = lPitch << 1;

		__asm
		{
			// esi : pAlphaTable
			// edi : dest
			
			// ebx : x itor
			// edx : psrc

			// cl : *psrc
			
			mov		esi, pAlphaTable
			mov		edx, psrc
			mov		edi, pdest	
			
			// for( i = 0; i < height; ++i )
			mov		i, 0

			mov		eax, height
			cmp		eax, 0
			jle		end_yloop

yloop:			
			// ebx = 0
			xor		ebx, ebx							

			// while( ebx < width )
xloop:
			cmp		ebx, width
			jge		end_xloop

			// cl = *psrc
			xor		ecx, ecx
			mov		cl, byte ptr [edx]
			
			// if( *psrc == COLOR_TRANS )
			cmp		ecx, COLOR_TRANS
			jne		putpixel

			// ++psrc;
			add		edx, 1

			// ebx += *psrc;
			mov		cl, byte ptr [edx]
			add		ebx, ecx
			
			// ++psrc;
			add		edx, 1

			jmp		xloop

putpixel:
			shr		ecx, 4

			mov		eax, 16
			sub		eax, ecx
			mov		ecx, eax

			xor		eax, eax
			mov		ax, word ptr [edi+ebx*2]			// dest color 값을 얻어온다
			
			mov		ecx, dword ptr [esi+ecx*4]

			mov		ax, word ptr [ecx+eax*2]			
			mov		word ptr [edi+ebx*2], ax

			// ++psrc;
			add		edx, 1

			// ++ebx;
			add		ebx, 1
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

