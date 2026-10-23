
#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"

#include "map.h"
#include "..\Smoke\Smoke.h"
#include "..\Char\KindInfo\KindInfo.h"
#include "..\..\Lib\AStar\AStarListMgr.h"
#include "..\..\Client\SmallMap\SMallMap.h"

#include "..\..\Client\Effect\FireFly\FireFlyMgr.h"				// 반딧불

#include "../../Client/Effect/IceFlash/IceFlashMgr.h"
#include "../../Client/NInterface/NMiniMapDlg/NMiniMapDlg.h"
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"





extern cltCommonLogic* pclClient;

//-----------------------------------------------------
// cltMapHeader
//-----------------------------------------------------
cltMapHeader::cltMapHeader()
{
	ZeroMemory(this, sizeof(cltMapHeader));
}

//-----------------------------------------------------
// cltFieldObjectBitField_Normal
//-----------------------------------------------------
UI32 cltFieldObjectBitField_Normal::GetFieldUnique()const 
{
	return FieldUnique;
}

void cltFieldObjectBitField_Normal::SetFieldUnique(UI32 unique)
{
	FieldUnique = unique;
}

UI32 cltFieldObjectBitField_Normal::GetValidSwitch()const
{
	return ValidSwitch;
}

void cltFieldObjectBitField_Normal::SetValidSwitch(UI32 valid)
{
	ValidSwitch = valid;
}

UI32 cltFieldObjectBitField_Normal::GetRandomValue()const
{
	return RandomValue;
}

void cltFieldObjectBitField_Normal::SetRandomValue(UI32 random)
{
	RandomValue = random;
}

UI32 cltFieldObjectBitField_Normal::GetLengthToBottom()const
{
	return LengthToBottom;
}

void cltFieldObjectBitField_Normal::SetLengthToBottom(UI32 bottom)
{
	LengthToBottom = bottom;
}

UI32 cltFieldObjectBitField_Normal::GetLengthToRight()const
{
	return LengthToRight;
}

void cltFieldObjectBitField_Normal::SetLengthToRight(UI32 right)
{
	LengthToRight = right;
}

//-----------------------------------------------------
// cltItemMapInfo
//-----------------------------------------------------
cltItemMapInfo::cltItemMapInfo()
{
	uiItemUnique	= 0;
	uiItemIndex		= 0;
}

//-----------------------------------------------------
// cltEffectMapInfo
//-----------------------------------------------------
void cltEffectMapInfo::Init()
{
	gimg = 0 ;
	x = 0 ;
	y = 0 ;
	font = 0 ;
	reverse = false ;
}

void cltEffectMapInfo::Set(cltEffectMapInfo* pclinfo)
{
	gimg = pclinfo->gimg ;
	x = pclinfo->x ;
	y = pclinfo->y ;
	font = pclinfo->font ;
	reverse = pclinfo->reverse ;
}

//-----------------------------------------------------
// cltMapGeoInfo
//-----------------------------------------------------
cltMapGeoInfo::cltMapGeoInfo(cltMapCommon* pclmap, SI32 xsize, SI32 ysize)
{
	SI32 i;

	pclMap		= pclmap;
	siXsize		= xsize;
	siYsize		= ysize;

	pclGeoMap	= new UI08 [xsize * ysize];
	if(pclGeoMap == NULL)
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox(NULL, TEXT("fd89jj"), TEXT("Fd3f"), MB_OK|MB_TOPMOST);
	}
	else
	{
		ZeroMemory(pclGeoMap, xsize * ysize);
	}


	for(i = 0;i < MAX_GEO_NUMBER;i++)
	{
		StringCchCopy(szGetText[i], GEO_TEXT_LENGTH, TEXT("NULL"));
	}

	InitGeoPosInfo();

}
cltMapGeoInfo::~cltMapGeoInfo()
{
	if(pclGeoMap)
	{
		delete[] pclGeoMap;
		pclGeoMap	= NULL;
	}
}
void cltMapGeoInfo::SetGeoMap(SI32 x, SI32 y, UI08 data)
{
	if(x < 0 || x >= siXsize)return ;
	if(y < 0 || y >= siYsize)return ;

	if(pclGeoMap == NULL)return ;
	pclGeoMap[x + (y*siXsize)]	= data;
}

UI08 cltMapGeoInfo::GetGeoMap(SI32 x, SI32 y)
{
	if(x < 0 || x >= siXsize)return 0;
	if(y < 0 || y >= siYsize)return 0;

	if(pclGeoMap == NULL)return 0;
	return pclGeoMap[x + (y*siXsize)];
}

// 지형정보의 위치 변수를 초기화한다. 
void 	cltMapGeoInfo::InitGeoPosInfo()
{
	SI32 i;

	for(i = 0;i < MAX_GEO_NUMBER;i++)
	{
		siGeoPosX[i]			= -1;
		siGeoPosY[i]			= -1;
		siGeoPosAccount[i]		= 0;	
	}
}

// 지형정보 위치를 업데이트 한다. 
void cltMapGeoInfo::UpdateGeoPos(SI32 x, SI32 y, SI32 geoinfo)
{
	siGeoPosX[geoinfo]	= (siGeoPosX[geoinfo] + x);
	siGeoPosY[geoinfo]	= (siGeoPosY[geoinfo] + y);

	siGeoPosAccount[geoinfo]++;
}

BOOL cltMapGeoInfo::Write(FILE* fp)
{
	fwrite(&siXsize, sizeof(SI32), 1, fp);
	fwrite(&siYsize, sizeof(SI32), 1, fp);
	fwrite(szGetText, GEO_TEXT_LENGTH, MAX_GEO_NUMBER, fp);
	fwrite(pclGeoMap, sizeof(UI08), siXsize * siYsize, fp);
	return TRUE;
}

BOOL cltMapGeoInfo::Read(FILE* fp)
{
	fread(&siXsize, sizeof(SI32), 1, fp);
	fread(&siYsize, sizeof(SI32), 1, fp);
	fread(szGetText, GEO_TEXT_LENGTH, MAX_GEO_NUMBER, fp);
	fread(pclGeoMap, sizeof(UI08), siXsize * siYsize, fp);
	return TRUE;
}

//-----------------------------------------------------
// cltMapCommon
//-----------------------------------------------------
SHORT cltMapCommon::GetXsize()
{
	return MH.MapXsize; 
}

SHORT cltMapCommon::GetYsize()
{
	return MH.MapYsize; 
}

bool cltMapCommon::IsInMapArea(SI32 x, SI32 y)
{
	if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return false;
	return true;
}	

BOOL cltMapCommon::SetCollMap(SHORT x, SHORT y, SHORT block)
{
	if( pCollMap == NULL )	return TRUE;
	if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return TRUE;

	pCollMap[x+YOff[y]] |= block;
	return TRUE;
}

SHORT	cltMapCommon::GetCollMap(SI32 x, SI32 y)
{
	if( pCollMap == NULL )	return 0;
	if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return 0;

	return pCollMap[x+YOff[y]];
}

// 특정 비트를 지운다. 
void	cltMapCommon::DeleteCollMap(SHORT x, SHORT y, SHORT block)
{
	if( pCollMap == NULL )	return;
	if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return ;

	pCollMap[x+YOff[y]] &= (~block);
}

// 어느지점의 충돌 여부를 알려준다. 
bool	cltMapCommon::IsColl(SHORT x, SHORT y, SHORT block)
{
	if( pCollMap == NULL ) return true;
	if( x<0 || x>=MH.MapXsize || y<0  || y>=MH.MapYsize )	return true;

	if( pCollMap[x+YOff[y]] & block )	return true;

	return false;
}

void	cltMapCommon::SetIDMap(SHORT id, SI32 x, SI32 y)
{
	if( pIDMap == NULL )	return;
	if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;
	pIDMap[x+YOff[y]] = id;
}

SHORT	cltMapCommon::GetIDMap(SI32 x, SI32 y)
{
	if( pIDMap == NULL )	return 0;
	if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return 0;
	return pIDMap[x+YOff[y]];
}

void	cltMapCommon::ClearIDMap(int x, int y)
{
	if( pIDMap == NULL )	return;
	if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return;

	pIDMap[x+YOff[y]] = 0;
}

void cltMapCommon::SetItemMap(SI32 x, SI32 y, UI16 itemunique, UI16 itemlistindex)
{
	if(pclItemMap == NULL)return ;
	if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return ;

	pclItemMap[x + YOff[y]].uiItemUnique	= itemunique;
	pclItemMap[x + YOff[y]].uiItemIndex	= itemlistindex;
}

cltItemMapInfo* cltMapCommon::GetItemMap(SI32 x, SI32 y)
{
	if(pclItemMap == NULL)return NULL;
	if( x<0 || x>=MH.MapXsize || y<0 || y>=MH.MapYsize )	return NULL;

	return &pclItemMap[x + YOff[y]];
}

//-----------------------------------------------
// cltFieldObjDrawData
//-----------------------------------------------
void cltFieldObjDrawData::Set(cltFieldObjDrawData* pcldata)
{
	x	= pcldata->x;
	y	= pcldata->y;
	cx	= pcldata->cx;
	cy	= pcldata->cy;

	founique	= pcldata->founique;
	type		= pcldata->type;

}

//-----------------------------------------------
// cltDrawObjData
//-----------------------------------------------
cltDrawObjData::cltDrawObjData()
{}

cltDrawObjData::cltDrawObjData(SI32 type, SI32 id)
{
	siType	= type;
	siID	= id;
}

cltDrawObjData::cltDrawObjData(SI32 type, cltFieldObjDrawData* pcldata)
{
	siType	= type;
	clFODrawData.Set(pcldata);
}



