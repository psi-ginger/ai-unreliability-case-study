#ifndef _DBMSG_FISHING_H_
#define _DBMSG_FISHING_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\Common\Fishing\FishingBaseInfo.h"

// 낚시 도구의 내구도 소모
class sDBRequest_FishingDecreaseToolDurability
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siCharID;

	// Tool - 내구도 닳게
	DBToolInfo clFishingTool[MAX_FISHING_TOOL_NUMBER];

	sDBRequest_FishingDecreaseToolDurability
		(
		SI32 PersonID,
		SI32 CharID,
		DBToolInfo* pclFishingTool
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_FishingDecreaseToolDurability );
		packetHeader.usCmd	= DBMSG_REQUEST_FISHINGDECREASETOOLDURABILITY;

		siPersonID = PersonID;
		siCharID = CharID;

		for( SI32 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
		{
			if ( pclFishingTool[i].clItem.siUnique > 0 )
			{
				clFishingTool[i].Set(&pclFishingTool[i]);
			}
		}
	}
};

class sDBResponse_FishingDecreaseToolDurability
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	// Tool - 내구도 닳게
	DBToolInfo clFishingTool[MAX_FISHING_TOOL_NUMBER];

	sDBResponse_FishingDecreaseToolDurability()
	{
		memset( this, 0, sizeof( sDBResponse_FishingDecreaseToolDurability ) );
	}
};

class sDBRequest_FishingGetMarineProducts
{
public:
	sPacketHeader packetHeader;	

	SI32 siPersonID;
	SI32 siCharID;
	
	// Output
	SI16 siAcquireMarineInvPos;					// 획득한 수산물을 넣을 인벤토리 위치
	cltItem clAcquireMarineProductItem;			// 획득한 수산물
	bool bCreateSID;							// 획득한 수산물의 SID 적용여부

	SI16 siSkillUnique;							// 경험치를 획득할 스킬의 유니크
	SI16 siIncFishingSkillExp;					// 획득한 경험치
	
	SI32 siCharUnique;							// 낚은 사람 또는 소환수의 CharUnique
	SI32 siKind;								// 누가 낚았는지... 주인이냐? 소환수냐?
			
	sDBRequest_FishingGetMarineProducts( SI32 PersonID,	SI32 CharID,
		SI16 AcquireMarineInvPos, cltItem *pclAcquireMarineProductItem, bool bcreatesid,
		SI16 SkillUnique, SI16 IncFishingSkillExp,
		SI32 CharUnique, SI32 Kind
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_FishingGetMarineProducts );
		packetHeader.usCmd	= DBMSG_REQUEST_FISHINGGETMARINEPRODUCTS;
		
		siPersonID = PersonID;
		siCharID = CharID;
		
		siAcquireMarineInvPos = AcquireMarineInvPos;
		clAcquireMarineProductItem.Set( pclAcquireMarineProductItem );
		bCreateSID = bcreatesid;

		siSkillUnique = SkillUnique;
		siIncFishingSkillExp = IncFishingSkillExp;

		siCharUnique = CharUnique;
		siKind = Kind;

	}
};

class sDBResponse_FishingGetMarineProducts
{
public:
	sPacketHeader packetHeader;

	SI32 siResult;

	SI32 siPersonID;
	SI32 siCharID;

	cltItem clAcquireMarineProductItem;			// 획득한 수산물	- For LOG

	SI16 siAcquireMarineInvPos;			// 수산물이 들어간 위치
	SI16 siAddedItemNum;				// 추가된 아이템의 개수
	cltItem clLeftItem;					// 최종 아이템 정보

	SI16 siIncSkillUnique;				// 기술 경험치 오른 Skill Unique
	SI32 siSkillTotalExp;				// 기술 경험치 최종...

	SI32 siCharUnique;					// 서버에서 보낸값 그대로 돌려주면 됨
	SI32 siKind;						// 서버에서 보낸값 그대로 돌려주면 됨
	
	sDBResponse_FishingGetMarineProducts()
	{
		memset( this, 0, sizeof( sDBResponse_FishingGetMarineProducts ) );
	}
};


#endif