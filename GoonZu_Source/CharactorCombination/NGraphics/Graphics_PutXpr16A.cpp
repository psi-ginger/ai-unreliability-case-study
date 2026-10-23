#include "graphics.h"

#include "..\\nutil\\nutil.h"

void NGraphics::PutXpr16AT( int x, int y, int width, int height, RGB16 *pSrcImage, int alpha, RGB16 colorkey )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	RGB16 *pSrcAlphaTable;
	RGB16 *pDestAlphaTable;

	RGB16 *pdest;
	RGB16 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;

	pSrcAlphaTable = (RGB16 *)m_pAlphaTable[ alpha ];
	pDestAlphaTable = (RGB16 *)m_pAlphaTable[ MAX_ALPHA_LEVEL - 1 - alpha ];

//#ifndef _GRAPHICS_ASMCODE

	int i, j, k;

	for( i = 0; i < height; ++i ) {

		j = 0;

		while( j < width ) {

			if( *psrc == colorkey ) {
				
				++psrc;
				j += *psrc;
				++psrc;
			
			} else {

				k = j;
				j += *psrc;
				++psrc;

				for( ; k < j; ++k ) {
					pdest[ k ] = pSrcAlphaTable[ *psrc ] +
								 pDestAlphaTable[ pdest[ k ] ];

					++psrc;
				}
			}
		}

		pdest += lPitch;
	}
/*
#else

	int i, j;
	RGB16 t;

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
			xor		ecx, ecx
			mov		cl, byte ptr [edx]

			// pdest[ ebx ] = pSrcAlphaTable[ pColorTable[ *psrc ] ] +
			//				  pDestAlphaTable[ pdest[ ebx ] ];
			xor		eax, eax
			mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
			mov		ecx, pSrcAlphaTable
			lea		ecx, [ ecx + eax * 2 ]
			mov		ax, word ptr [ecx]
			mov		t, ax

			mov		ax, word ptr [edi+ebx*2]
			mov		ecx, pDestAlphaTable
			lea		ecx, [ ecx + eax * 2 ]
			mov		ax, word ptr [ecx]
			
			mov		cx, t
			add		cx, ax

			mov		word ptr [edi+ebx*2], cx			// dest에 쓴다 
			
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
*/
}

void NGraphics::PutXpr16CAT( int x, int y, int width, int height, RGB16 *pSrcImage, int alpha, RGB16 colorkey )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;
	
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

	RGB16 *pSrcAlphaTable;
	RGB16 *pDestAlphaTable;

	RGB16 *pdest;
	RGB16 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;

	pSrcAlphaTable = (RGB16 *)m_pAlphaTable[ alpha ];
	pDestAlphaTable = (RGB16 *)m_pAlphaTable[ MAX_ALPHA_LEVEL - 1 - alpha ];

//#ifndef _GRAPHICS_ASMCODE

	int i, j, k;

	if( x < clip_x1 ) {

		trim_x = clip_x1 - x;

		if( y < clip_y1 ) {

			for( i = 0; i < trim_y2; ++i ) {
				j = 0;
				while( j < width ) {

					if( *psrc == colorkey ) {					
						++psrc; j += *psrc; ++psrc;
					} else {
						k = j; j += *psrc; ++psrc;
						for( ; k < j; ++k ) {
							if( i >= trim_y1 && k >= trim_x ) {
								pdest[ k ] = pSrcAlphaTable[ *psrc ] + 
											 pDestAlphaTable[ pdest[ k ] ];
							}
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
							if( k >= trim_x ) {
								pdest[ k ] = pSrcAlphaTable[ *psrc ] + 
											 pDestAlphaTable[ pdest[ k ] ];
							}
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
							if( i >= trim_y1 && k <= trim_x ) {
								pdest[ k ] = pSrcAlphaTable[ *psrc ] + 
											 pDestAlphaTable[ pdest[ k ] ];
							}
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
							if( k <= trim_x ) {
								pdest[ k ] = pSrcAlphaTable[ *psrc ] + 
											 pDestAlphaTable[ pdest[ k ] ];
							}
							++psrc;
						}
					}
				}
				pdest += lPitch;
			}
		}


	}


/*
#else

	int i, j;
	RGB16 t;

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
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop1:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop1

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel1

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1

				jmp		xloop1

putpixel1:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1

pp1:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( i >= trim_y1 && ebx >= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel1
				cmp		ebx, trim_x
				jl		skipputpixel1

				// pdest[ ebx ] = pSrcAlphaTable[ pColorTable[ *psrc ] ] +
				//				  pDestAlphaTable[ pdest[ ebx ] ];
				xor		eax, eax
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		ecx, pSrcAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				mov		t, ax

				mov		ax, word ptr [edi+ebx*2]
				mov		ecx, pDestAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				
				mov		cx, t
				add		cx, ax

				mov		word ptr [edi+ebx*2], cx			// dest에 쓴다 
				
skipputpixel1:
				// ++psrc;
				add		edx, 1

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp1
				
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
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop2:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop2

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel2

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1

				jmp		xloop2

putpixel2:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1

pp2:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( ebx >= trim_x )
				cmp		ebx, trim_x
				jl		skipputpixel2

				// pdest[ ebx ] = pSrcAlphaTable[ pColorTable[ *psrc ] ] +
				//				  pDestAlphaTable[ pdest[ ebx ] ];
				xor		eax, eax
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		ecx, pSrcAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				mov		t, ax

				mov		ax, word ptr [edi+ebx*2]
				mov		ecx, pDestAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				
				mov		cx, t
				add		cx, ax

				mov		word ptr [edi+ebx*2], cx			// dest에 쓴다 
				
skipputpixel2:
				// ++psrc;
				add		edx, 1

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp2
				
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
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop3:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop3

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel3

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1

				jmp		xloop3

putpixel3:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1

pp3:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( i >= trim_y1 && ebx <= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel3
				cmp		ebx, trim_x
				jg		skipputpixel3

				// pdest[ ebx ] = pSrcAlphaTable[ pColorTable[ *psrc ] ] +
				//				  pDestAlphaTable[ pdest[ ebx ] ];
				xor		eax, eax
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		ecx, pSrcAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				mov		t, ax

				mov		ax, word ptr [edi+ebx*2]
				mov		ecx, pDestAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				
				mov		cx, t
				add		cx, ax

				mov		word ptr [edi+ebx*2], cx			// dest에 쓴다 

skipputpixel3:
				// ++psrc;
				add		edx, 1

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp3
				
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
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop4:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop4

				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel4

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1

				jmp		xloop4

putpixel4:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1

pp4:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( ebx <= trim_x )
				cmp		ebx, trim_x
				jg		skipputpixel4

				// pdest[ ebx ] = pSrcAlphaTable[ pColorTable[ *psrc ] ] +
				//				  pDestAlphaTable[ pdest[ ebx ] ];
				xor		eax, eax
				mov		ax, word ptr [esi+ecx*2]			// color table에서 rgb값을 읽어온다 
				mov		ecx, pSrcAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				mov		t, ax

				mov		ax, word ptr [edi+ebx*2]
				mov		ecx, pDestAlphaTable
				lea		ecx, [ ecx + eax * 2 ]
				mov		ax, word ptr [ecx]
				
				mov		cx, t
				add		cx, ax

				mov		word ptr [edi+ebx*2], cx			// dest에 쓴다 

skipputpixel4:
				// ++psrc;
				add		edx, 1

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp4
				
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
*/
}
