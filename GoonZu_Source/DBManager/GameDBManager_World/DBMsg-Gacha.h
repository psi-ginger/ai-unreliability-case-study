#ifndef _DBMSGGACHA_H
#define _DBMSGGACHA_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "../../GoonzuWorld/server/GachaManager/GachaManager.h"


//------------------------------------------
// 가차 열쇠로 아이템을 구입할 것을 요청. 
//------------------------------------------
class sDBRequest_GachaBuy
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상  Person
	cltItem				clKeyItem;		// 열쇠 아이템 (소모됨)
	cltItem				clGetItem;		// 취득 아이템 (생성됨)
	bool				bPileSwitch;	// 포개기 가능 여부. 	
	SI16				siKeyItemPos;	// 열쇠 아이템의 인벤내 위치.
	SI16				siGetItemPos;	// 아이템을 넣을 인벤내 위치.
	UI16				uiGachaCnt;
	bool				bBestProduct;
	UI08				uiGachaCntAdd;	// uiGachaCnt에 +될 숫자

	sDBRequest_GachaBuy()
	{
		ZeroMemory(this, sizeof(sDBRequest_GachaBuy));
	}

	sDBRequest_GachaBuy(SI32 id, SI32 personid, cltItem* pclkeyitem, cltItem* pclgetitem, bool bpilewitch,  SI16 keyitempos, SI16 getitempos, UI16 gachacnt, bool bestproduct, UI08 gachacntadd)
	{
		packetHeader.usSize	= sizeof( sDBRequest_GachaBuy);
		packetHeader.usCmd	= DBMSG_REQUEST_GACHABUY;
		usCharID			= id;
		siPersonID			= personid;

		clKeyItem.Set( pclkeyitem );
		clGetItem.Set( pclgetitem );

		bPileSwitch			= bpilewitch;	
		siKeyItemPos		= keyitempos;
		siGetItemPos		= getitempos;
		uiGachaCnt			= gachacnt;		
		bBestProduct		= bestproduct;
		uiGachaCntAdd		= gachacntadd;
	}
}; 


class sDBResponse_GachaBuy
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siKeyItemPos;		// 열쇠 아이템의 인벤내 위치.
	SI16				siGetItemPos;		// 아이템을 넣을 인벤내 위치.
	cltItem				clKeyItem;
	cltItem				clGetItem;			// 남은 아이템.
	UI16				uiGachaCnt;			// 가차에 성공했다면 + 1 되어 돌아온다
	bool				bBestProduct;

	//[추가 : 황진성 2007. 10. 24 로그에 이용]
	cltItem				clGachaGetItem;		// 가차로 순수하게 얻은 아이템.
	cltItem				clUseKeyItem;		// 열쇠 아이템 (소모됨)

	UI08				uiGachaCntAdd;		// uiGachaCnt에 +될 숫자

	sDBResponse_GachaBuy()
	{
		ZeroMemory(this, sizeof(sDBResponse_GachaBuy));
	}
};

#endif