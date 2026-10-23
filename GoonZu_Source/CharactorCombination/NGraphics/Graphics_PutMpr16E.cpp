#include "graphics.h"

#include "..\\nutil\\nutil.h"

void NGraphics::PutMpr16ET( int x, int y, UI08 *pSrcImage, int width, int height, int effect, UI08 *pDestImage )
{

	if( m_bSkipDraw && m_pddsLockedSurface == m_pddsBackBuffer ) return;

	RGB16 *pEffectTable;

	RGB16 *pColorTable;
	RGB16 *pdest;
	UI08 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	if( pDestImage ) pdest = (RGB16 *)pDestImage + ( y * lPitch ) + x;
	else pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;
	
	pColorTable = (RGB16 *)m_pCurrentColorTablePtr;

	pEffectTable = (RGB16 *)m_pEffectTable[ effect ];

#ifndef _GRAPHICS_ASMCODE

	RGB16 p1, p2, r1, g1, b1, r2, g2, b2, r3, g3, b3;

	int i, j, k;

	if( m_rgbmode == RGB16_565 ) {

		for( i = 0; i < height; ++i ) {

			j = 0;

			while( j < width ) {

				if( *psrc == COLOR_TRANS ) {
					
					++psrc;
					j += *psrc;
					++psrc;
				
				} else {

					k = j;
					j += *psrc;
					++psrc;

					for( ; k < j; ++k ) {

						p1 = pdest[ k ];
						p2 = pColorTable[ *psrc ];

						r1 = ( p1 & RGB16_565_RMASK ) >> 11;
						g1 = ( p1 & RGB16_565_GMASK ) >> 6;
						b1 = ( p1 ) & RGB16_565_BMASK;

						r2 = ( p2 & RGB16_565_RMASK ) >> 11;
						g2 = ( p2 & RGB16_565_GMASK ) >> 6;
						b2 = ( p2 ) & RGB16_565_BMASK;

						r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 11;
						g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 6;
						b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

						pdest[ k ] = r3 | g3 | b3;

						++psrc;
					}
				}
			}

			pdest += lPitch;
		}

	} else {

		for( i = 0; i < height; ++i ) {

			j = 0;

			while( j < width ) {

				if( *psrc == COLOR_TRANS ) {
					
					++psrc;
					j += *psrc;
					++psrc;
				
				} else {

					k = j;
					j += *psrc;
					++psrc;

					for( ; k < j; ++k ) {

						p1 = pdest[ k ];
						p2 = pColorTable[ *psrc ];

						r1 = ( p1 & RGB16_555_RMASK ) >> 10;
						g1 = ( p1 & RGB16_555_GMASK ) >> 5;
						b1 = ( p1 ) & RGB16_555_BMASK;

						r2 = ( p2 & RGB16_555_RMASK ) >> 10;
						g2 = ( p2 & RGB16_555_GMASK ) >> 5;
						b2 = ( p2 ) & RGB16_555_BMASK;

						r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 10;
						g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 5;
						b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

						pdest[ k ] = r3 | g3 | b3;

						++psrc;
					}
				}
			}

			pdest += lPitch;
		}

	}

#else

	int i, j;

	RGB16 p1, p2, r3, g3;

	lPitch = lPitch << 1;

	if( m_rgbmode == RGB16_565 ) {

		__asm
		{
			// edi : dest
			// esi : color table
			
			// ebx : x itor
			// edx : psrc

			// cl : *psrc

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
			mov		edx, psrc
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
			mov		psrc, edx

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
			mov		psrc, edx

pp:
			// cl = *psrc
			xor		ecx, ecx
			mov		cl, byte ptr [edx]

			// p1
			mov		ax, word ptr [edi+ebx*2]
			mov		p1, ax

			// p2
			mov		ax, word ptr [esi+ecx*2]
			mov		p2, ax

			xor		eax, eax
			xor		ecx, ecx

			// edx = pEffectTable
			mov		edx, pEffectTable

			// r1
			mov		ax, p1
			and		ax, RGB16_565_RMASK
			shr		ax, 11
			
			// r2
			mov		cx, p2
			and		cx, RGB16_565_RMASK
			shr		cx, 11

			// r3
			shl		ax, 5
			add		cx, ax
			mov		ax, word ptr [edx+ecx*2]
			shl		ax, 11
			mov		r3, ax

			// g1
			mov		ax, p1
			and		ax, RGB16_565_GMASK
			shr		ax, 6

			// g2
			mov		cx, p2
			and		cx, RGB16_565_GMASK
			shr		cx, 6

			// g3
			shl		ax, 5
			add		cx, ax
			mov		ax, word ptr [edx+ecx*2]
			shl		ax, 6
			mov		g3, ax

			// b1
			mov		ax, p1
			and		ax, RGB16_565_BMASK

			// b2
			mov		cx, p2
			and		cx, RGB16_565_BMASK

			// b3
			shl		ax, 5
			add		cx, ax
			mov		ax, word ptr [edx+ecx*2]
			
			or		ax, g3
			or		ax, r3
			
			// pdest[ ebx ] = r3 | g3 | b3
			mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
			
			// ++psrc;
			mov		edx, psrc
			add		edx, 1
			mov		psrc, edx

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
	
	} else {

		__asm
		{
			// edi : dest
			// esi : color table
			
			// ebx : x itor
			// edx : psrc

			// cl : *psrc

			mov		edi, pdest			
			mov		esi, pColorTable			

			// for( i = 0; i < height; ++i )
			mov		i, 0

			mov		eax, height
			cmp		eax, 0
			jle		end_yloop_5

yloop_5:			
			// j = 0
			mov		j, 0						

			// while( j < width )
xloop_5:
			mov		eax, j
			cmp		eax, width
			jge		end_xloop_5

			// cl = *psrc
			xor		ecx, ecx
			mov		edx, psrc
			mov		cl, byte ptr [edx]
			
			// if( *psrc == COLOR_TRANS )
			cmp		ecx, COLOR_TRANS
			jne		putpixel_5

			// ++psrc;
			add		edx, 1

			// j += *psrc;
			mov		cl, byte ptr [edx]
			mov		eax, j
			add		eax, ecx
			mov		j, eax
			
			// ++psrc;
			add		edx, 1
			mov		psrc, edx

			jmp		xloop_5

putpixel_5:
			// ebx = j
			// j += *psrc
			// ++psrc
			mov		ebx, j
			mov		eax, ebx
			add		eax, ecx
			mov		j, eax
			add		edx, 1
			mov		psrc, edx

pp_5:
			// cl = *psrc
			xor		ecx, ecx
			mov		cl, byte ptr [edx]

			// p1
			mov		ax, word ptr [edi+ebx*2]
			mov		p1, ax

			// p2
			mov		ax, word ptr [esi+ecx*2]
			mov		p2, ax

			xor		eax, eax
			xor		ecx, ecx

			// edx = pEffectTable
			mov		edx, pEffectTable

			// r1
			mov		ax, p1
			and		ax, RGB16_555_RMASK
			shr		ax, 10
			
			// r2
			mov		cx, p2
			and		cx, RGB16_555_RMASK
			shr		cx, 10

			// r3
			shl		ax, 5
			add		cx, ax
			mov		ax, word ptr [edx+ecx*2]
			shl		ax, 10
			mov		r3, ax

			// g1
			mov		ax, p1
			and		ax, RGB16_555_GMASK
			shr		ax, 5

			// g2
			mov		cx, p2
			and		cx, RGB16_555_GMASK
			shr		cx, 5

			// g3
			shl		ax, 5
			add		cx, ax
			mov		ax, word ptr [edx+ecx*2]
			shl		ax, 5
			mov		g3, ax

			// b1
			mov		ax, p1
			and		ax, RGB16_555_BMASK

			// b2
			mov		cx, p2
			and		cx, RGB16_555_BMASK

			// b3
			shl		ax, 5
			add		cx, ax
			mov		ax, word ptr [edx+ecx*2]
			
			or		ax, g3
			or		ax, r3
			
			// pdest[ ebx ] = r3 | g3 | b3
			mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
			
			// ++psrc;
			mov		edx, psrc
			add		edx, 1
			mov		psrc, edx

			// ++ebx
			add		ebx, 1

			cmp		ebx, j
			jl		pp_5
			
			jmp		xloop_5

end_xloop_5:

			add		edi, lPitch

			mov		eax, i
			add		eax, 1
			mov		i, eax

			cmp		eax, height
			jge		end_yloop_5
			
			jmp		yloop_5

end_yloop_5:

		}

	}

#endif
}

void NGraphics::PutMpr16CET( int x, int y, UI08 *pSrcImage, int width, int height, int effect, UI08 *pDestImage )
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

	RGB16 *pEffectTable;

	RGB16 *pColorTable;
	RGB16 *pdest;
	UI08 *psrc;
	LONG lPitch;
	
	lPitch = m_lPitch >> 1;
	
	psrc = pSrcImage;

	if( pDestImage ) pdest = (RGB16 *)pDestImage + ( y * lPitch ) + x;
	else pdest = (RGB16 *)m_pImage + ( y * lPitch ) + x;
	
	pColorTable = (RGB16 *)m_pCurrentColorTablePtr;

	pEffectTable = (RGB16 *)m_pEffectTable[ effect ];

#ifndef _GRAPHICS_ASMCODE

	RGB16 p1, p2, r1, g1, b1, r2, g2, b2, r3, g3, b3;

	int i, j, k;

	if( m_rgbmode == RGB16_565 ) {

		if( x < clip_x1 ) {

			trim_x = clip_x1 - x;

			if( y < clip_y1 ) {

				for( i = 0; i < trim_y2; ++i ) {
					j = 0;
					while( j < width ) {

						if( *psrc == COLOR_TRANS ) {					
							++psrc; j += *psrc; ++psrc;
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( i >= trim_y1 && k >= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_565_RMASK ) >> 11;
									g1 = ( p1 & RGB16_565_GMASK ) >> 6;
									b1 = ( p1 ) & RGB16_565_BMASK;

									r2 = ( p2 & RGB16_565_RMASK ) >> 11;
									g2 = ( p2 & RGB16_565_GMASK ) >> 6;
									b2 = ( p2 ) & RGB16_565_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 11;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 6;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
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

						if( *psrc == COLOR_TRANS ) {					
							++psrc; j += *psrc; ++psrc;
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( k >= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_565_RMASK ) >> 11;
									g1 = ( p1 & RGB16_565_GMASK ) >> 6;
									b1 = ( p1 ) & RGB16_565_BMASK;

									r2 = ( p2 & RGB16_565_RMASK ) >> 11;
									g2 = ( p2 & RGB16_565_GMASK ) >> 6;
									b2 = ( p2 ) & RGB16_565_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 11;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 6;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
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

						if( *psrc == COLOR_TRANS ) {
							++psrc; j += *psrc; ++psrc;				
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( i >= trim_y1 && k <= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_565_RMASK ) >> 11;
									g1 = ( p1 & RGB16_565_GMASK ) >> 6;
									b1 = ( p1 ) & RGB16_565_BMASK;

									r2 = ( p2 & RGB16_565_RMASK ) >> 11;
									g2 = ( p2 & RGB16_565_GMASK ) >> 6;
									b2 = ( p2 ) & RGB16_565_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 11;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 6;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
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

						if( *psrc == COLOR_TRANS ) {
							++psrc; j += *psrc; ++psrc;				
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( k <= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_565_RMASK ) >> 11;
									g1 = ( p1 & RGB16_565_GMASK ) >> 6;
									b1 = ( p1 ) & RGB16_565_BMASK;

									r2 = ( p2 & RGB16_565_RMASK ) >> 11;
									g2 = ( p2 & RGB16_565_GMASK ) >> 6;
									b2 = ( p2 ) & RGB16_565_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 11;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 6;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
								}
								++psrc;
							}
						}
					}
					pdest += lPitch;
				}
			}


		}
	
	} else {
		// 555 mode 

		if( x < clip_x1 ) {

			trim_x = clip_x1 - x;

			if( y < clip_y1 ) {

				for( i = 0; i < trim_y2; ++i ) {
					j = 0;
					while( j < width ) {

						if( *psrc == COLOR_TRANS ) {					
							++psrc; j += *psrc; ++psrc;
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( i >= trim_y1 && k >= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_555_RMASK ) >> 10;
									g1 = ( p1 & RGB16_555_GMASK ) >> 5;
									b1 = ( p1 ) & RGB16_555_BMASK;

									r2 = ( p2 & RGB16_555_RMASK ) >> 10;
									g2 = ( p2 & RGB16_555_GMASK ) >> 5;
									b2 = ( p2 ) & RGB16_555_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 10;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 5;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
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

						if( *psrc == COLOR_TRANS ) {					
							++psrc; j += *psrc; ++psrc;
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( k >= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_555_RMASK ) >> 10;
									g1 = ( p1 & RGB16_555_GMASK ) >> 5;
									b1 = ( p1 ) & RGB16_555_BMASK;

									r2 = ( p2 & RGB16_555_RMASK ) >> 10;
									g2 = ( p2 & RGB16_555_GMASK ) >> 5;
									b2 = ( p2 ) & RGB16_555_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 10;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 5;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
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

						if( *psrc == COLOR_TRANS ) {
							++psrc; j += *psrc; ++psrc;				
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( i >= trim_y1 && k <= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_555_RMASK ) >> 10;
									g1 = ( p1 & RGB16_555_GMASK ) >> 5;
									b1 = ( p1 ) & RGB16_555_BMASK;

									r2 = ( p2 & RGB16_555_RMASK ) >> 10;
									g2 = ( p2 & RGB16_555_GMASK ) >> 5;
									b2 = ( p2 ) & RGB16_555_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 10;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 5;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
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

						if( *psrc == COLOR_TRANS ) {
							++psrc; j += *psrc; ++psrc;				
						} else {
							k = j; j += *psrc; ++psrc;
							for( ; k < j; ++k ) {
								if( k <= trim_x ) {
									p1 = pdest[ k ];
									p2 = pColorTable[ *psrc ];

									r1 = ( p1 & RGB16_555_RMASK ) >> 10;
									g1 = ( p1 & RGB16_555_GMASK ) >> 5;
									b1 = ( p1 ) & RGB16_555_BMASK;

									r2 = ( p2 & RGB16_555_RMASK ) >> 10;
									g2 = ( p2 & RGB16_555_GMASK ) >> 5;
									b2 = ( p2 ) & RGB16_555_BMASK;

									r3 = pEffectTable[ ( r1 << 5 ) + r2 ] << 10;
									g3 = pEffectTable[ ( g1 << 5 ) + g2 ] << 5;
									b3 = pEffectTable[ ( b1 << 5 ) + b2 ];

									pdest[ k ] = r3 | g3 | b3;
								}
								++psrc;
							}
						}
					}
					pdest += lPitch;
				}
			}


		}


	}



#else

	int i, j;
	
	RGB16 p1, p2, r3, g3;

	lPitch = lPitch << 1;

	if( m_rgbmode == RGB16_565 ) {

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
				mov		edx, psrc
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
				mov		psrc, edx

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
				mov		psrc, edx

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

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_565_RMASK
				shr		ax, 11
				
				// r2
				mov		cx, p2
				and		cx, RGB16_565_RMASK
				shr		cx, 11

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 11
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_565_GMASK
				shr		ax, 6

				// g2
				mov		cx, p2
				and		cx, RGB16_565_GMASK
				shr		cx, 6

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 6
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_565_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_565_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel1:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

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
				mov		edx, psrc
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
				mov		psrc, edx

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
				mov		psrc, edx

pp2:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( ebx >= trim_x )
				cmp		ebx, trim_x
				jl		skipputpixel2

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_565_RMASK
				shr		ax, 11
				
				// r2
				mov		cx, p2
				and		cx, RGB16_565_RMASK
				shr		cx, 11

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 11
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_565_GMASK
				shr		ax, 6

				// g2
				mov		cx, p2
				and		cx, RGB16_565_GMASK
				shr		cx, 6

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 6
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_565_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_565_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel2:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

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
				mov		edx, psrc
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
				mov		psrc, edx

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
				mov		psrc, edx

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

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_565_RMASK
				shr		ax, 11
				
				// r2
				mov		cx, p2
				and		cx, RGB16_565_RMASK
				shr		cx, 11

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 11
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_565_GMASK
				shr		ax, 6

				// g2
				mov		cx, p2
				and		cx, RGB16_565_GMASK
				shr		cx, 6

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 6
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_565_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_565_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel3:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

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
				mov		edx, psrc
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
				mov		psrc, edx

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
				mov		psrc, edx

pp4:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( ebx <= trim_x )
				cmp		ebx, trim_x
				jg		skipputpixel4

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_565_RMASK
				shr		ax, 11
				
				// r2
				mov		cx, p2
				and		cx, RGB16_565_RMASK
				shr		cx, 11

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 11
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_565_GMASK
				shr		ax, 6

				// g2
				mov		cx, p2
				and		cx, RGB16_565_GMASK
				shr		cx, 6

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 6
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_565_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_565_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel4:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

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

	} else {


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

				mov		edi, pdest			
				mov		esi, pColorTable			

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop1_5

yloop1_5:			
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop1_5:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop1_5

				// cl = *psrc
				xor		ecx, ecx
				mov		edx, psrc
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel1_5

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1
				mov		psrc, edx

				jmp		xloop1_5

putpixel1_5:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1
				mov		psrc, edx

pp1_5:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( i >= trim_y1 && ebx >= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel1_5
				cmp		ebx, trim_x
				jl		skipputpixel1_5

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_555_RMASK
				shr		ax, 10
				
				// r2
				mov		cx, p2
				and		cx, RGB16_555_RMASK
				shr		cx, 10

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 10
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_555_GMASK
				shr		ax, 5

				// g2
				mov		cx, p2
				and		cx, RGB16_555_GMASK
				shr		cx, 5

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 5
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_555_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_555_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel1_5:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp1_5
				
				jmp		xloop1_5

end_xloop1_5:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop1_5
				
				jmp		yloop1_5

end_yloop1_5:

			}

		} else {

			__asm
			{
				// edi : dest
				// esi : color table
				
				// ebx : x itor
				// edx : psrc

				// cl : *psrc

				mov		edi, pdest			
				mov		esi, pColorTable			

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop2_5

yloop2_5:			
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop2_5:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop2_5

				// cl = *psrc
				xor		ecx, ecx
				mov		edx, psrc
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel2_5

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1
				mov		psrc, edx

				jmp		xloop2_5

putpixel2_5:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1
				mov		psrc, edx

pp2_5:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( ebx >= trim_x )
				cmp		ebx, trim_x
				jl		skipputpixel2_5

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_555_RMASK
				shr		ax, 10
				
				// r2
				mov		cx, p2
				and		cx, RGB16_555_RMASK
				shr		cx, 10

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 10
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_555_GMASK
				shr		ax, 5

				// g2
				mov		cx, p2
				and		cx, RGB16_555_GMASK
				shr		cx, 5

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 5
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_555_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_555_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 
				
skipputpixel2_5:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp2_5
				
				jmp		xloop2_5

end_xloop2_5:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop2_5
				
				jmp		yloop2_5

end_yloop2_5:

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

				mov		edi, pdest			
				mov		esi, pColorTable			

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop3_5

yloop3_5:			
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop3_5:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop3_5

				// cl = *psrc
				xor		ecx, ecx
				mov		edx, psrc
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel3_5

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1
				mov		psrc, edx

				jmp		xloop3_5

putpixel3_5:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1
				mov		psrc, edx

pp3_5:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( i >= trim_y1 && ebx <= trim_x )
				mov		eax, i
				cmp		eax, trim_y1
				jl		skipputpixel3_5
				cmp		ebx, trim_x
				jg		skipputpixel3_5

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_555_RMASK
				shr		ax, 10
				
				// r2
				mov		cx, p2
				and		cx, RGB16_555_RMASK
				shr		cx, 10

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 10
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_555_GMASK
				shr		ax, 5

				// g2
				mov		cx, p2
				and		cx, RGB16_555_GMASK
				shr		cx, 5

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 5
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_555_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_555_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel3_5:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp3_5
				
				jmp		xloop3_5

end_xloop3_5:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop3_5
				
				jmp		yloop3_5

end_yloop3_5:

			}

		} else {

			__asm
			{
				// edi : dest
				// esi : color table
				
				// ebx : x itor
				// edx : psrc

				// cl : *psrc

				mov		edi, pdest			
				mov		esi, pColorTable			

				// for( i = 0; i < trim_y2; ++i )
				mov		i, 0

				mov		eax, trim_y2
				cmp		eax, 0
				jle		end_yloop4_5

yloop4_5:			
				// j = 0
				mov		j, 0						

				// while( j < width )
xloop4_5:
				mov		eax, j
				cmp		eax, width
				jge		end_xloop4_5

				// cl = *psrc
				xor		ecx, ecx
				mov		edx, psrc
				mov		cl, byte ptr [edx]
				
				// if( *psrc == COLOR_TRANS )
				cmp		ecx, COLOR_TRANS
				jne		putpixel4_5

				// ++psrc;
				add		edx, 1

				// j += *psrc;
				mov		cl, byte ptr [edx]
				mov		eax, j
				add		eax, ecx
				mov		j, eax
				
				// ++psrc;
				add		edx, 1
				mov		psrc, edx

				jmp		xloop4_5

putpixel4_5:
				// ebx = j
				// j += *psrc
				// ++psrc
				mov		ebx, j
				mov		eax, ebx
				add		eax, ecx
				mov		j, eax
				add		edx, 1
				mov		psrc, edx

pp4_5:
				// cl = *psrc
				xor		ecx, ecx
				mov		cl, byte ptr [edx]

				// if( ebx <= trim_x )
				cmp		ebx, trim_x
				jg		skipputpixel4_5

				// p1
				mov		ax, word ptr [edi+ebx*2]
				mov		p1, ax

				// p2
				mov		ax, word ptr [esi+ecx*2]
				mov		p2, ax

				xor		eax, eax
				xor		ecx, ecx

				// edx = pEffectTable
				mov		edx, pEffectTable

				// r1
				mov		ax, p1
				and		ax, RGB16_555_RMASK
				shr		ax, 10
				
				// r2
				mov		cx, p2
				and		cx, RGB16_555_RMASK
				shr		cx, 10

				// r3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 10
				mov		r3, ax

				// g1
				mov		ax, p1
				and		ax, RGB16_555_GMASK
				shr		ax, 5

				// g2
				mov		cx, p2
				and		cx, RGB16_555_GMASK
				shr		cx, 5

				// g3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				shl		ax, 5
				mov		g3, ax

				// b1
				mov		ax, p1
				and		ax, RGB16_555_BMASK

				// b2
				mov		cx, p2
				and		cx, RGB16_555_BMASK

				// b3
				shl		ax, 5
				add		cx, ax
				mov		ax, word ptr [edx+ecx*2]
				
				or		ax, g3
				or		ax, r3
				
				// pdest[ ebx ] = r3 | g3 | b3
				mov		word ptr [edi+ebx*2], ax			// dest에 쓴다 

skipputpixel4_5:
				// ++psrc;
				mov		edx, psrc
				add		edx, 1
				mov		psrc, edx

				// ++ebx
				add		ebx, 1

				cmp		ebx, j
				jl		pp4_5
				
				jmp		xloop4_5

end_xloop4_5:

				add		edi, lPitch

				mov		eax, i
				add		eax, 1
				mov		i, eax

				cmp		eax, trim_y2
				jge		end_yloop4_5
				
				jmp		yloop4_5

end_yloop4_5:

			}

		}

	}



	}

#endif

}
