//#include "stdafx.h"

#include <Graphic.h>

void cltGrp::PutPixel(int x, int y, unsigned char color)
{
	if(IsInClipArea(x, y, x, y) == TRUE)
	{
		PutPixel8(x, y, color);
	}
	else
	{
		PutPixel8C(x, y, color);
	}
}

void cltGrp::Line(int x1, int y1, int x2, int y2, unsigned char color)
{
	if(IsInClipArea(x1, y1, x2, y2) == TRUE)
	{
		Line8(x1, y1, x2, y2, color);
	}
	else
	{
		Line8C(x1, y1, x2, y2, color);
	}
}

void cltGrp::Linex2(int x1, int y1, int x2, int y2, unsigned char color)
{
	if(IsInClipArea(x1, y1, x2, y2) == TRUE)
	{
		Line8x2(x1, y1, x2, y2, color);
	}
	else
	{
		Line8Cx2(x1, y1, x2, y2, color);
	}
}


void cltGrp::Box(int x1, int y1, int x2, int y2, unsigned char color)
{
	if(IsInClipArea(x1, y1, x2, y2) == TRUE)
	{
		Box8(x1, y1, x2, y2, color);
	}
	else
	{
		Box8C(x1, y1, x2, y2, color);
	}
}

void cltGrp::FillBox(int x1, int y1, int x2, int y2, unsigned char color)
{
	if(IsInClipArea(x1, y1, x2, y2) == TRUE)
	{
		FillBox8(x1, y1, x2, y2, color);
	}
	else
	{
		FillBox8C(x1, y1, x2, y2, color);
	}
}

void cltGrp::PutImage(int x,int y,int xsize,int ysize, unsigned char *image)
{
	if(IsInClipArea(x, y, x+xsize-1, y+ysize-1) == TRUE)
	{
		PutImage8(x,y,xsize,ysize, image);
	}
	else
	{
		PutImage8C(x,y,xsize,ysize, image);
	}
}

void cltGrp::GetImage(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* buffer)
{
	if(IsInClipArea(x, y, x+xsize-1, y+ysize-1) == TRUE)
	{
		GetImage8(x, y, xsize, ysize, buffer);

	}
	else
	{
		GetImage8C(x, y, xsize, ysize, buffer);
	}
}

void cltGrp::PutSpr(SI32 x, SI32 y, SI32 xsize, SI32 ysize, UI08* image)
{
	if(IsInClipArea(x, y, x+xsize-1, y+ysize-1) == TRUE)
	{
		PutSpr8(x, y, xsize, ysize, image);

	}
	else
	{
		PutSpr8C(x, y, xsize, ysize, image);
	}
}