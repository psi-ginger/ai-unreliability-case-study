#ifndef _ZOO_KEEP_DRAW_
#define _ZOO_KEEP_DRAW_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "../Lib/Graphic/graphic.h"
#include "../lib/ngraphics/graphics.h"

#include "ndataTypes.h"
#include "ntimer.h"
#include "../Client/MiniGame/AniSpr.h"
#include "TSpr.h"

class CZooDraw
{
public:
	
	TSpr			m_sprBlockBack;  // 배경
	TSpr			m_sprBlock;		 // 블럭 모양
	TSpr			m_sprSelectBlock;
	TSpr			m_sprNumber;
	TSpr			m_sprTime1;
	TSpr			m_sprTime2;
	TSpr			m_sprTime3;

	CAniSpr			m_AniBlock;
	CAniSpr			m_AniSpecialBlock;
	CAniSpr			m_AniEffectBlock;

public:
	CZooDraw();
	~CZooDraw();

	bool LoadImage();
	void FreeImage();

	void DrawBlockBack(SI32 x, SI32 y, SI32 imagenum =0); // 배경을 그린다
	void DrawBlock(SI32 x, SI32 y, SI32 imagenum = 0);		 // 블럭을 그린다
	void DrawSelectBlock(SI32 x, SI32 y, SI32 imagenum = 0);
	void DrawRemoveBlock(SI32 x, SI32 y, SI32 imagenum = 0);
	void DrawSelect(SI32 x, SI32 y, SI32 imagenum = 0);
	void DrawNumber(SI32 x, SI32 y, SI32 imagenum = 0);
	void DrawDeleteBlock(SI32 x, SI32 y);
	void DrawSpecialBlock(SI32 x, SI32 y);
	void DrawEffectBlock(SI32 x, SI32 y);
	void DrawTime1(SI32 x, SI32 y, SI32 imagenum =0);
	void DrawTime2(SI32 x, SI32 y, SI32 imagenum =0);
	void DrawTime3(SI32 x, SI32 y, SI32 imagenum =0);
//	void DrawAniBlock(SI32 x, SI32 y, UI08* image, SI32 xsize, SI32 ysize);
};
#endif