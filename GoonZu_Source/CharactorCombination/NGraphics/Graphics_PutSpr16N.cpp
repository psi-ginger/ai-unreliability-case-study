#include "graphics.h"

#include "..\\nutil\\nutil.h"

void NGraphics::PutSpr16N( int x, int y, UI08 *pSrcImage, int width, int height, UI08 *pDestImage )
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

	int i, j;

	for( i = 0; i < height; ++i ) {

		j = 0;

		while( j < width ) {

			if( *psrc == COLOR_TRANS ) {
				
				++psrc;
				j += *psrc;
				++psrc;
			
			} else {
				pdest[ j ] = pColorTable[ *psrc ];
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
			// pdest[ ebx ] = pColorTable[ *psrc ];
			mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
			mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
			
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


void NGraphics::PutSpr16RN( int x, int y, UI08 *pSrcImage, int width, int height, UI08 *pDestImage )
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

	int i, j;

	for( i = 0; i < height; ++i ) {

		j = width - 1;

		while( j >= 0 ) {

			if( *psrc == COLOR_TRANS ) {
				
				++psrc;
				j -= *psrc;
				++psrc;
			
			} else {
				pdest[ j ] = pColorTable[ *psrc ];
				++psrc;
				--j;
			}
		}

		pdest += lPitch;
	}

#else

	int i;

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
			// ebx = width - 1
			mov		ebx, width
			sub		ebx, 1

			// while( ebx >= 0 )
xloop:
			cmp		ebx, 0
			jl		end_xloop

			// cl = *psrc
			xor		ecx, ecx
			mov		cl, byte ptr [edx]
			
			// if( *psrc == COLOR_TRANS )
			cmp		ecx, COLOR_TRANS
			jne		putpixel

			// ++psrc;
			add		edx, 1

			// ebx -= *psrc;
			mov		cl, byte ptr [edx]
			sub		ebx, ecx
			
			// ++psrc;
			add		edx, 1

			jmp		xloop

putpixel:
			// pdest[ ebx ] = pColorTable[ *psrc ];
			mov		ax, word ptr [esi+ecx*2]			// rgb table에서 rgb값을 읽어온다 
			mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
			
			// ++psrc;
			add		edx, 1

			// --ebx;
			sub		ebx, 1
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


void NGraphics::PutSpr16CN( int x, int y, UI08 *pSrcImage, int width, int height, UI08 *pDestImage )
{
	SI32 x2, y2;
	SI32 trim_x, trim_y1, trim_y2;
	SI32 clip_x1, clip_y1, clip_x2, clip_y2;

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

	int i, j;

	if( x < clip_x1 ) {

		trim_x = clip_x1 - x;

		if( y < clip_y1 ) {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == COLOR_TRANS ) {					
						++psrc; j += *psrc; ++psrc;
					} else {
						if( i >= trim_y1 && j >= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; ++j;
					}
				}
				pdest += lPitch;
			}
		} else {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == COLOR_TRANS ) {					
						++psrc; j += *psrc; ++psrc;
					} else {
						if( j >= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; ++j;
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

					if( *psrc == COLOR_TRANS ) {
						++psrc; j += *psrc; ++psrc;				
					} else {
						if( i >= trim_y1 && j <= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; ++j;
					}
				}
				pdest += lPitch;
			}
		} else {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == COLOR_TRANS ) {
						++psrc; j += *psrc; ++psrc;				
					} else {
						if( j <= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; ++j;
					}
				}
				pdest += lPitch;
			}
		}


	}

#else
	int i;

	lPitch = lPitch << 1;

	if( x < clip_x1 ) {

		trim_x = clip_x1 - x;

		if( y < clip_y1 ) {
		
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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop1

yloop1:			
				// ebx = 0
				xor		ebx, ebx							

				// while( ebx < width )
xloop1:
				cmp		ebx, width
				jge		end_xloop1

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel1

				// ++psrc;
				add		edx, 1

				// ebx += *psrc;
				mov		cl, byte ptr [edx]
				add		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop1

putpixel1:
				// if( i >= trim_y1 && ebx >= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel1
				cmp		ebx, trim_x
				jl		skipputpixel1

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel1:
				// ++psrc;
				add		edx, 1

				// ++ebx;
				add		ebx, 1
				jmp		xloop1

end_xloop1:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop1
				
				jmp		yloop1

end_yloop1:

			}

		} else {

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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop2

yloop2:			
				// ebx = 0
				xor		ebx, ebx							

				// while( ebx < width )
xloop2:
				cmp		ebx, width
				jge		end_xloop2

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel2

				// ++psrc;
				add		edx, 1

				// ebx += *psrc;
				mov		cl, byte ptr [edx]
				add		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop2

putpixel2:
				// if( ebx >= trim_x )
				cmp		ebx, trim_x
				jl		skipputpixel2

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel2:
				// ++psrc;
				add		edx, 1

				// ++ebx;
				add		ebx, 1
				jmp		xloop2

end_xloop2:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop2
				
				jmp		yloop2

end_yloop2:

			}



		}

	} else {

		if( x2 > clip_x2 ) trim_x = clip_x2 - x;
		else trim_x = width;

		if( y < clip_y1 ) {

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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop3

yloop3:			
				// ebx = 0
				xor		ebx, ebx							

				// while( ebx < width )
xloop3:
				cmp		ebx, width
				jge		end_xloop3

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel3

				// ++psrc;
				add		edx, 1

				// ebx += *psrc;
				mov		cl, byte ptr [edx]
				add		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop3

putpixel3:
				// if( i >= trim_y1 && ebx <= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel3
				cmp		ebx, trim_x
				jg		skipputpixel3

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel3:
				// ++psrc;
				add		edx, 1

				// ++ebx;
				add		ebx, 1
				jmp		xloop3

end_xloop3:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop3
				
				jmp		yloop3

end_yloop3:

			}

		} else {

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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop4

yloop4:			
				// ebx = 0
				xor		ebx, ebx							

				// while( ebx < width )
xloop4:
				cmp		ebx, width
				jge		end_xloop4

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel4

				// ++psrc;
				add		edx, 1

				// ebx += *psrc;
				mov		cl, byte ptr [edx]
				add		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop4

putpixel4:
				// if( ebx <= trim_x )
				cmp		ebx, trim_x
				jg		skipputpixel4

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel4:
				// ++psrc;
				add		edx, 1

				// ++ebx;
				add		ebx, 1
				jmp		xloop4

end_xloop4:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop4
				
				jmp		yloop4

end_yloop4:

			}

		}

	}

#endif

}



void NGraphics::PutSpr16CRN( int x, int y, UI08 *pSrcImage, int width, int height, UI08 *pDestImage )
{
	SI32 x2, y2;
	SI32 trim_x, trim_y1, trim_y2;
	SI32 clip_x1, clip_y1, clip_x2, clip_y2;

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

	int i, j;

	if( x < clip_x1 ) {

		trim_x = clip_x1 - x;

		if( y < clip_y1 ) {

			for( i = 0; i < trim_y2; ++i ) {
				j = width - 1;
				while( j >= 0 ) {

					if( *psrc == COLOR_TRANS ) {					
						++psrc; j -= *psrc; ++psrc;
					} else {
						if( i >= trim_y1 && j >= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; --j;
					}
				}
				pdest += lPitch;
			}
		} else {

			for( i = 0; i < trim_y2; ++i ) {
				j = width - 1;
				while( j >= 0 ) {

					if( *psrc == COLOR_TRANS ) {					
						++psrc; j -= *psrc; ++psrc;
					} else {
						if( j >= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; --j;
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
				j = width - 1;
				while( j >= 0 ) {

					if( *psrc == COLOR_TRANS ) {
						++psrc; j -= *psrc; ++psrc;				
					} else {
						if( i >= trim_y1 && j <= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; --j;
					}
				}
				pdest += lPitch;
			}
		} else {

			for( i = 0; i < trim_y2; ++i ) {
				j = width - 1;
				while( j >= 0 ) {

					if( *psrc == COLOR_TRANS ) {
						++psrc; j -= *psrc; ++psrc;				
					} else {
						if( j <= trim_x ) pdest[ j ] = pColorTable[ *psrc ];
						++psrc; --j;
					}
				}
				pdest += lPitch;
			}
		}


	}

#else
	int i;

	lPitch = lPitch << 1;

	if( x < clip_x1 ) {

		trim_x = clip_x1 - x;

		if( y < clip_y1 ) {
		
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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop1

yloop1:			
				// ebx = width - 1
				mov		ebx, width
				sub		ebx, 1
	
				// while( ebx >= 0 )
xloop1:
				cmp		ebx, 0
				jl		end_xloop1

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel1

				// ++psrc;
				add		edx, 1

				// ebx -= *psrc;
				mov		cl, byte ptr [edx]
				sub		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop1

putpixel1:
				// if( i >= trim_y1 && ebx >= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel1
				cmp		ebx, trim_x
				jl		skipputpixel1

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel1:
				// ++psrc;
				add		edx, 1

				// --ebx;
				sub		ebx, 1
				jmp		xloop1

end_xloop1:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop1
				
				jmp		yloop1

end_yloop1:

			}

		} else {

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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop2

yloop2:			
				// ebx = width - 1
				mov		ebx, width
				sub		ebx, 1
	
				// while( ebx >= 0 )
xloop2:
				cmp		ebx, 0
				jl		end_xloop2

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel2

				// ++psrc;
				add		edx, 1

				// ebx -= *psrc;
				mov		cl, byte ptr [edx]
				sub		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop2

putpixel2:
				// if( ebx >= trim_x )
				cmp		ebx, trim_x
				jl		skipputpixel2

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel2:
				// ++psrc;
				add		edx, 1

				// --ebx;
				sub		ebx, 1
				jmp		xloop2

end_xloop2:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop2
				
				jmp		yloop2

end_yloop2:

			}



		}

	} else {

		if( x2 > clip_x2 ) trim_x = clip_x2 - x;
		else trim_x = width;

		if( y < clip_y1 ) {

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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop3

yloop3:			
				// ebx = width - 1
				mov		ebx, width
				sub		ebx, 1
	
				// while( ebx >= 0 )
xloop3:
				cmp		ebx, 0
				jl		end_xloop3

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel3

				// ++psrc;
				add		edx, 1

				// ebx -= *psrc;
				mov		cl, byte ptr [edx]
				sub		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop3

putpixel3:
				// if( i >= trim_y1 && ebx <= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel3
				cmp		ebx, trim_x
				jg		skipputpixel3

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel3:
				// ++psrc;
				add		edx, 1

				// --ebx;
				sub		ebx, 1
				jmp		xloop3

end_xloop3:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop3
				
				jmp		yloop3

end_yloop3:

			}

		} else {

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

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop4

yloop4:			
				// ebx = width - 1
				mov		ebx, width
				sub		ebx, 1
	
				// while( ebx >= 0 )
xloop4:
				cmp		ebx, 0
				jl		end_xloop4

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel4

				// ++psrc;
				add		edx, 1

				// ebx -= *psrc;
				mov		cl, byte ptr [edx]
				sub		ebx, ecx
				
				// ++psrc;
				add		edx, 1

				jmp		xloop4

putpixel4:
				// if( ebx <= trim_x )
				cmp		ebx, trim_x
				jg		skipputpixel4

				// pdest[ ebx ] = pColorTable[ *psrc ];
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel4:
				// ++psrc;
				add		edx, 1

				// --ebx;
				sub		ebx, 1
				jmp		xloop4

end_xloop4:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop4
				
				jmp		yloop4

end_yloop4:

			}

		}

	}

#endif

}
