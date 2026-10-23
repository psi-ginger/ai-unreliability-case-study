//---------------------------------
// 2005/3/11 김태곤
//---------------------------------
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "Map\Map.h"

#include "..\Resource.h"

#include "Client.h"
#include "Music\Music.h"

extern cltCommonLogic* pclClient;

// 포그 처리. 
void cltMapClient::FogRecoverAction()
{
	SI32 i, j;

	if(pclClient->GameMode == GAMEMODE_CLIENT)
	{

		// 지도의 유효 범위안에 있는 FOG_NONE을 FOG_HALF로 설정한다. 
		for(i = ValidRect.top;i <= ValidRect.bottom;i++)
			for(j = ValidRect.left; j<= ValidRect.right;j++)
			{
				if(GetFogMap(j, i) < FOG_HALF)
				{
					PutFog(j, i, FOG_HALF);
				}
			};
	}	
}


UI08 cltMapClient::GetFogData(SI32 x, SI32 y, SI32 fog)
{
	UI08 data = 0;

	if((SI32)GetFogMap(x, y+1) >= fog)
	{
		data |= FOG_SOUTH;
	}
	if((SI32)GetFogMap(x-1, y) >= fog)
	{
		data |= FOG_WEST;
	}
	if((SI32)GetFogMap(x, y-1) >= fog)
	{
		data |= FOG_NORTH;
	}
	if((SI32)GetFogMap(x+1, y) >= fog)
	{
		data |= FOG_EAST;
	}
	if((SI32)GetFogMap(x-1, y+1) >= fog)
	{
		data |= FOG_SW;
	}
	if((SI32)GetFogMap(x+1, y+1) >= fog)
	{
		data |= FOG_SE;
	}
	if((SI32)GetFogMap(x-1, y-1) >= fog)
	{
		data |= FOG_NW;
	}
	if((SI32)GetFogMap(x+1, y-1) >= fog)
	{
		data |= FOG_NE;
	}

	return data;



}


// 특정 위치의 포그 정보를 계산하여 출력하여야 할 포그 정보를 리턴한다. 
bool cltMapClient::CalcFogData(SI32 x, SI32 y, SI32* pfont)
{
	static SI32 fogarray[]={
		0, 7, 4, 7,		8, 0, 8, 11,	3, 0, 0, 0,		8, 0, 8, 11,// 0
			5, 0, 0, 0,		0, 0, 0, 0,		5, 0, 0, 0,		12, 0, 12, 0, // 16
			2, 0, 14,0,		0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 32
			5, 0, 0, 0,		0, 0, 12,0,		5, 0, 0, 0,		12, 0, 12,0, // 48
			6, 10,0, 10,	0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 64
			0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 80
			6, 10,0, 10,	0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 96
			9, 0, 0, 0,		0, 0, 0, 0,		9, 0, 9, 0,		9, 0, 0, 0, // 112
			1, 7, 0, 7,		0, 0, 0, 11,		13,0, 0, 0,		0, 11,0, 11, // 128
			0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 144
			0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 160
			0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 176
			6, 10,0, 10,	0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 192
			0, 0, 0, 0,		0, 0, 0, 0,		9, 0, 0, 0,		0, 0, 0, 0, // 208
			6, 10,0, 10,	0, 0, 0, 0,		0, 0, 0, 0,		0, 0, 0, 0, // 224
			9, 0, 0, 0,		0, 0, 0, 0,		9, 0, 0, 0,		9, 0, 0, 0 //  240
	};

	switch(GetFogMap(x, y))
	{
	case FOG_BLACK:
		{
			*pfont = 15;		// 완전 검은 색을 그린다. 
			return true;
		}
	case FOG_HALF:
		{
			SI32 fog = GetFogData(x, y, FOG_BLACK);
			if(fog)
			{
				*pfont = 15 + fogarray[fog];
				return true;
			}
			else
			{
				*pfont = 0;
				return true;
			}
		}
	case FOG_NONE:
		{
			SI32 fog = GetFogData(x, y, FOG_HALF);
			if(fog)
			{
				*pfont = fogarray[ fog ];
				return true;
			}
			else
			{
				*pfont = -1;
				return false;
			}
		}
	}

	return false;
}


// 특정 타일에 포그 관련 작업을 수행한다. 
void cltMapClient::PutFog(SI32 x, SI32 y, SI32 fog)
{
	if(IsInMapArea(x, y) == false)return ;

	// 자신의 정보는 변경되지 않는다. 
	SetFogMap(x, y, fog);

	if(fog == FOG_NONE)
	{
		// 주변의 포그 상황을 파악한다. 
		if(GetFogMap(x, y+1) >= FOG_BLACK)PutFog(x, y+1, FOG_HALF);
		if(GetFogMap(x, y-1) >= FOG_BLACK)PutFog(x, y-1, FOG_HALF);
		if(GetFogMap(x+1, y) >= FOG_BLACK)PutFog(x+1, y, FOG_HALF);
		if(GetFogMap(x-1, y) >= FOG_BLACK)PutFog(x-1, y, FOG_HALF);
		if(GetFogMap(x-1, y-1) >= FOG_BLACK)PutFog(x-1, y-1, FOG_HALF);
		if(GetFogMap(x+1, y-1) >= FOG_BLACK)PutFog(x+1, y-1, FOG_HALF);
		if(GetFogMap(x-1, y+1) >= FOG_BLACK)PutFog(x-1, y+1, FOG_HALF);
		if(GetFogMap(x+1, y+1) >= FOG_BLACK)PutFog(x+1, y+1, FOG_HALF);

		// 자신의 정보는 변경되지 않는다. 
		SetFogMap(x, y, fog);
	}
	else if(fog == FOG_HALF)
	{
	}
	else if(fog == FOG_BLACK)
	{
		// 주변의 포그 상황을 파악한다. 
		if(GetFogMap(x, y+1) < FOG_HALF)PutFog(x, y+1, FOG_HALF);
		if(GetFogMap(x, y-1) < FOG_HALF)PutFog(x, y-1, FOG_HALF);
		if(GetFogMap(x+1, y) < FOG_HALF)PutFog(x+1, y, FOG_HALF);
		if(GetFogMap(x-1, y) < FOG_HALF)PutFog(x-1, y, FOG_HALF);
		if(GetFogMap(x-1, y-1) < FOG_HALF)PutFog(x-1, y-1, FOG_HALF);
		if(GetFogMap(x+1, y-1) < FOG_HALF)PutFog(x+1, y-1, FOG_HALF);
		if(GetFogMap(x-1, y+1) < FOG_HALF)PutFog(x-1, y+1, FOG_HALF);
		if(GetFogMap(x+1, y+1) < FOG_HALF)PutFog(x+1, y+1, FOG_HALF);
	}




}