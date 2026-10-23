//---------------------------------------------------------
//	Created By 이강형
//  From [Time and Tales] Game
//---------------------------------------------------------

#include "graphics.h"

#include "nutil.h"
#include <math.h>

#define HQ_PI		3.1415926535897932

void NGraphics::PutSpr16ST( int drawx, int drawy, UI08 *pSrcImage, int Width, int Height, UI08 *pDestImage, FLOAT fDegree, FLOAT fScale)
{
	//=============================================================================
	// 메모리 버퍼
	UI08	SourceBuffer[1024 * 768];
	if (1024 < Width)		return;
	if (768  < Height)		return;
	memset(SourceBuffer, 254, 1024 * 768);
	UI08*	pSrcBuffer = SourceBuffer;
	__asm
	{
		mov		esi, pSrcImage
		mov		edi, pSrcBuffer

		// Line = 0
		xor		eax, eax
yloop:
		cmp		eax, Height
		jge		end_yloop

		xor		ebx, ebx
xloop:
		cmp		ebx, Width
		jge		end_xloop

		xor		ecx, ecx 
		mov		cl, byte ptr [esi]

		cmp		ecx, COLOR_TRANS	// if *pSrcImage == COLOR_TRANS
		jne		put_pixel

		add		esi, 1				// ++pSrcImage;
		mov		cl, byte ptr [esi]
		add		ebx, ecx			// j += *pSrcImage;
		add		esi, 1				// ++pSrcImage;
        
		jmp		xloop

put_pixel:
		mov		dl,  byte ptr [esi]
		mov		byte ptr[edi + ebx], dl			// 	SourceBuffer[j] = *pSrcImage;
		add		esi, 1				// ++pSrcImage;
		add		ebx, 1				// j++
		jmp		xloop

end_xloop:
		add		edi, 1024			// SourceBuffer += 1024;
		add		eax, 1				//Line++;

		jmp		yloop
end_yloop:

	}
	//=============================================================================

	RGB16 *pColorTable;
	RGB16 *pdest;
	UI08 *psrc;
	LONG lPitch;

	int MidX = drawx + Width / 2;
	int MidY = drawy + Height / 2;

	lPitch = m_lPitch >> 1;
	pColorTable = (RGB16 *)m_pCurrentColorTablePtr;

	int	  nScale = (int)(fScale * 0x10000);

	// SinA, CosA 값 계산 : 라디안으로 변경한 뒤 계산
	int		nSin, nCos;
	double fAngle = fDegree * HQ_PI / 180.0;
	nSin = (int)(sin(-fAngle) * 0x10000);
	nCos = (int)(cos(-fAngle) * 0x10000);

	// 회전된 사각형의 위치 계산
	int Left	= ((drawx - MidX) * nScale) >> 16;
	int Right	= ((drawx + Width  - MidX) * nScale) >> 16;;
	int Top		= ((drawy - MidY) * nScale) >> 16;
	int Bottom	= ((drawy + Height - MidY) * nScale) >> 16;

	int x1 = nCos * Left - nSin * Top;
	int y1 = nSin * Left + nCos * Top;
	int x2 = nCos * Right - nSin * Top;
	int y2 = nSin * Right + nCos * Top;
	int x3 = nCos * Left - nSin * Bottom;
	int y3 = nSin * Left + nCos * Bottom;
	int x4 = nCos * Right - nSin * Bottom;
	int y4 = nSin * Right + nCos * Bottom;

	int minx = min(x1, min(x2, min(x3, x4)));
	int miny = min(y1, min(y2, min(y3, y4)));
	int maxx = max(x1, max(x2, max(x3, x4)));
	int maxy = max(y1, max(y2, max(y3, y4)));
	minx = (minx >> 16) + MidX;	
	miny = (miny >> 16) + MidY;
	maxx = (maxx >> 16) + MidX;
	maxy = (maxy >> 16) + MidY;

	// Screen Clipping
	if (minx < m_rcClip.left)		minx = m_rcClip.left;
	if (miny < m_rcClip.top)		miny = m_rcClip.top;
	if (maxx > m_rcClip.right)		maxx = m_rcClip.right;
	if (maxy > m_rcClip.bottom)		maxy = m_rcClip.bottom;

	int DestWidth = maxx - minx;
	int DestHeight = maxy - miny;

	// 고정소수점 연산
	int		duCol = (int)(sin(-fAngle) * (1.0f / fScale) * 0x10000);
	int		dvCol = (int)(cos(-fAngle) * (1.0f / fScale) * 0x10000);
	int		duRow = dvCol;
	int		dvRow = -duCol;	

	// 고정소수점 연산
	int		startingu = ((MidX - drawx) << 16) - ((MidX - minx) * dvCol + (MidY - miny) * duCol);
	int		startingv = ((MidY - drawy) << 16) - ((MidX - minx) * dvRow + (MidY - miny) * duRow);

	psrc = SourceBuffer;
	if( pDestImage ) pdest = (RGB16 *)pDestImage + ( miny * lPitch ) + minx;
	else pdest = (RGB16 *)m_pImage + ( miny * lPitch ) + minx;

#ifndef _GRAPHICS_ASMCODE

	int		rowu = startingu;
	int		rowv = startingv;
	for(int y = 0; y < DestHeight ; y++)
	{
		int u = rowu;
		int v = rowv;
		for(int x = 0; x < DestWidth ; x++)
		{   
			int srcu = u >> 16;
			int srcv = v >> 16;
			if(srcu >= 0 && srcv >= 0 && srcu < Width && srcv < Height)
			{   
				BYTE color = psrc[srcv * SPIN_BUFFER_XSIZE + srcu];
				if (color != COLOR_TRANS)		pdest[ x ] = pColorTable[color];
			}
			u += duRow;
			v += dvRow;
		}
		rowu += duCol;
		rowv += dvCol;

		pdest += lPitch;
	}
#else
	int y = 0;
	int x = 0;

	int	rowu = startingu;
	int	rowv = startingv;

	lPitch = lPitch << 1;

	if (Width == 476 && Height == 687)
		int a = 0;

	__asm
	{
		mov     edi, pdest

height_loop:
		mov     eax, y		
		cmp		eax, DestHeight		// y < DestHeight
		jge     height_end_loop

		mov		dword ptr [x], 0	// x = 0
		mov		ecx, rowu
		mov		edx, rowv

width_loop:
		mov     eax, x
		cmp		eax, DestWidth
		jge     width_end_loop

		push	ecx
		push    edx

		sar     ecx, 10h
		sar     edx, 10h

		cmp     ecx, 0
		jl      do_nothing
		cmp     edx, 0
		jl      do_nothing

		cmp     ecx, Width
		jge     do_nothing
		cmp     edx, Height
		jge     do_nothing

		mov     ebx, edx 
		shl     ebx, 0Ah		// * 1024 -- 버퍼가 바뀌면 이것도 바뀌어야 한다.---
		add     ebx, ecx

		xor     eax, eax
		mov		esi, psrc
		mov		al, byte ptr [esi + ebx ]			// color 값

		cmp     eax, COLOR_TRANS					// 투명값 확인
		je      do_nothing
        
		xor		ebx, ebx
		mov     esi, pColorTable
		mov		bx, word ptr [esi+eax*2]		// color table에서 rgb값을 읽어온다 
		mov     eax, x
		mov		word ptr [edi+eax*2], bx			// dest에 쓴다 

do_nothing:
		pop     edx 
		pop     ecx

		add		ecx, duRow	// u += duRow
		add		edx, dvRow  // v += dvRow

		mov		eax, x
		add     eax, 1
		mov     x, eax		// x++		

		jmp    width_loop
        
width_end_loop:
		mov		eax, rowu
		add		eax, duCol
		mov     rowu, eax	// rowu += duCol

		mov		eax, rowv
		add		eax, dvCol
		mov     rowv, eax	// rowv += dvCol

		add		edi, lPitch	// pdest += lPitch

		mov		eax, y;
		add     eax, 1;
		mov     y, eax;		// y++

		jmp		height_loop

height_end_loop:
	}

#endif
}

void NGraphics::PutSpr16SRT( int drawx, int drawy, UI08 *pSrcImage, int Width, int Height, UI08 *pDestImage, FLOAT fDegree, FLOAT fScale )
{
	//=============================================================================
	// 메모리 버퍼
	UI08	SourceBuffer[1024 * 768];
	if (1024 < Width)		return;
	if (768  < Height)		return;
	memset(SourceBuffer, 254, 1024 * 768);
	UI08*	pSrcBuffer = SourceBuffer;
	__asm
	{
		mov		esi, pSrcImage
		mov		edi, pSrcBuffer

		// Line = 0
		xor		eax, eax
yloop:
		cmp		eax, Height
		jge		end_yloop

		xor		ebx, ebx
xloop:
		cmp		ebx, Width
		jge		end_xloop

		xor		ecx, ecx 
		mov		cl, byte ptr [esi]

		cmp		ecx, COLOR_TRANS	// if *pSrcImage == COLOR_TRANS
		jne		put_pixel

		add		esi, 1				// ++pSrcImage;
		mov		cl, byte ptr [esi]
		add		ebx, ecx			// j += *pSrcImage;
		add		esi, 1				// ++pSrcImage;
        
		jmp		xloop

put_pixel:
		mov		dl,  byte ptr [esi]
		mov		byte ptr[edi + ebx], dl			// 	SourceBuffer[j] = *pSrcImage;
		add		esi, 1				// ++pSrcImage;
		add		ebx, 1				// j++
		jmp		xloop

end_xloop:
		add		edi, 1024			// SourceBuffer += 1024;
		add		eax, 1				//Line++;

		jmp		yloop
end_yloop:

	}
	//=============================================================================

	RGB16 *pColorTable;
	RGB16 *pdest;
	UI08 *psrc;
	LONG lPitch;

	int MidX = drawx + Width / 2;
	int MidY = drawy + Height / 2;

	lPitch = m_lPitch >> 1;
	pColorTable = (RGB16 *)m_pCurrentColorTablePtr;

	int	  nScale = (int)(fScale * 0x10000);

	// SinA, CosA 값 계산 : 라디안으로 변경한 뒤 계산
	int		nSin, nCos;
	double fAngle = fDegree * HQ_PI / 180.0;
	nSin = (int)(sin(-fAngle) * 0x10000);
	nCos = (int)(cos(-fAngle) * 0x10000);

	// 회전된 사각형의 위치 계산
	int Left	= ((drawx - MidX) * nScale) >> 16;
	int Right	= ((drawx + Width  - MidX) * nScale) >> 16;;
	int Top		= ((drawy - MidY) * nScale) >> 16;
	int Bottom	= ((drawy + Height - MidY) * nScale) >> 16;

	int x1 = nCos * Left - nSin * Top;
	int y1 = nSin * Left + nCos * Top;
	int x2 = nCos * Right - nSin * Top;
	int y2 = nSin * Right + nCos * Top;
	int x3 = nCos * Left - nSin * Bottom;
	int y3 = nSin * Left + nCos * Bottom;
	int x4 = nCos * Right - nSin * Bottom;
	int y4 = nSin * Right + nCos * Bottom;

	int minx = min(x1, min(x2, min(x3, x4)));
	int miny = min(y1, min(y2, min(y3, y4)));
	int maxx = max(x1, max(x2, max(x3, x4)));
	int maxy = max(y1, max(y2, max(y3, y4)));
	minx = (minx >> 16) + MidX;	
	miny = (miny >> 16) + MidY;
	maxx = (maxx >> 16) + MidX;
	maxy = (maxy >> 16) + MidY;

	// Screen Clipping
	if (minx < m_rcClip.left)		minx = m_rcClip.left;
	if (miny < m_rcClip.top)		miny = m_rcClip.top;
	if (maxx > m_rcClip.right)		maxx = m_rcClip.right;
	if (maxy > m_rcClip.bottom)		maxy = m_rcClip.bottom;

	int DestWidth = maxx - minx;
	int DestHeight = maxy - miny;

	// 고정소수점 연산
	int		duCol = (int)(sin(-fAngle) * (1.0f / fScale) * 0x10000);
	int		dvCol = (int)(cos(-fAngle) * (1.0f / fScale) * 0x10000);
	int		duRow = dvCol;
	int		dvRow = -duCol;	

	// 고정소수점 연산
	int		startingu = ((MidX - drawx) << 16) - ((MidX - minx) * dvCol + (MidY - miny) * duCol);
	int		startingv = ((MidY - drawy) << 16) - ((MidX - minx) * dvRow + (MidY - miny) * duRow);

	psrc = SourceBuffer;
	if( pDestImage ) pdest = (RGB16 *)pDestImage + ( miny * lPitch ) + minx;
	else pdest = (RGB16 *)m_pImage + ( miny * lPitch ) + minx;

#ifndef _GRAPHICS_ASMCODE

	int		rowu = startingu;
	int		rowv = startingv;
	for(int y = 0; y < DestHeight ; y++)
	{
		int u = rowu;
		int v = rowv;
		for(int x = 0; x < DestWidth ; x++)
		{   
			int srcu = u >> 16;
			int srcv = v >> 16;
			if(srcu >= 0 && srcv >= 0 && srcu < Width && srcv < Height)
			{   
				BYTE color = psrc[srcv * SPIN_BUFFER_XSIZE + (Width - srcu)];
				if (color != COLOR_TRANS)		pdest[ x ] = pColorTable[color];
			}
			u += duRow;
			v += dvRow;
		}
		rowu += duCol;
		rowv += dvCol;

		pdest += lPitch;
	}
#else
	int y = 0;
	int x = 0;

	int	rowu = startingu;
	int	rowv = startingv;

	lPitch = lPitch << 1;

	if (Width == 476 && Height == 687)
		int a = 0;

	__asm
	{
		mov     edi, pdest

height_loop:
		mov     eax, y		
		cmp		eax, DestHeight		// y < DestHeight
		jge     height_end_loop

		mov		dword ptr [x], 0	// x = 0
		mov		ecx, rowu
		mov		edx, rowv

width_loop:
		mov     eax, x
		cmp		eax, DestWidth
		jge     width_end_loop

		push	ecx
		push    edx

		sar     ecx, 10h
		sar     edx, 10h

		cmp     ecx, 0
		jl      do_nothing
		cmp     edx, 0
		jl      do_nothing

		cmp     ecx, Width
		jge     do_nothing
		cmp     edx, Height
		jge     do_nothing

		mov     ebx, edx 
		shl     ebx, 0Ah		// * 1024 -- 버퍼가 바뀌면 이것도 바뀌어야 한다.---
		mov		eax, Width
		sub		eax, ecx		// (Width - srcu)
		add     ebx, eax

		xor     eax, eax
		mov		esi, psrc
		mov		al, byte ptr [esi + ebx ]			// color 값

		cmp     eax, COLOR_TRANS					// 투명값 확인
		je      do_nothing
        
		xor		ebx, ebx
		mov     esi, pColorTable
		mov		bx, word ptr [esi+eax*2]		// color table에서 rgb값을 읽어온다 
		mov     eax, x
		mov		word ptr [edi+eax*2], bx			// dest에 쓴다 

do_nothing:
		pop     edx 
		pop     ecx

		add		ecx, duRow	// u += duRow
		add		edx, dvRow  // v += dvRow

		mov		eax, x
		add     eax, 1
		mov     x, eax		// x++		

		jmp    width_loop
        
width_end_loop:
		mov		eax, rowu
		add		eax, duCol
		mov     rowu, eax	// rowu += duCol

		mov		eax, rowv
		add		eax, dvCol
		mov     rowv, eax	// rowv += dvCol

		add		edi, lPitch	// pdest += lPitch

		mov		eax, y;
		add     eax, 1;
		mov     y, eax;		// y++

		jmp		height_loop

height_end_loop:
	}

#endif
}
