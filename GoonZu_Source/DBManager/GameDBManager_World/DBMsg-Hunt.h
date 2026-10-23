#ifndef _DBMSGHUNT_H
#define _DBMSGHUNT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\Hunt\Hunt.h"

//-----------------------------------------------------
// 수렵장에 넣을 동물과 마리수를 입력한다. ( 리스트에 추가 )
//-----------------------------------------------------
class sDBRequest_HuntMonsterFieldListAdd
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID;

	SI32				siVillageUnique;

	SI16				siAddListIndex;

	SI16				siMonsterKind;
	SI16				siMonsterNum;
	SI16				siMonsterLevel;

	sDBRequest_HuntMonsterFieldListAdd()
	{
		ZeroMemory(this, sizeof(sDBRequest_HuntMonsterFieldListAdd));
	}

	sDBRequest_HuntMonsterFieldListAdd( SI32 PersonID, SI32 CharID, SI32 VillageUnique, SI16 AddListIndex, SI16 MonsterKind, SI16 MonsterNum, SI16 MonsterLevel )
	{
		packetHeader.usSize = sizeof( sDBRequest_HuntMonsterFieldListAdd );
		packetHeader.usCmd	= DBMSG_REQUEST_HUNTMONSTERFIELDLISTADD;

		siPersonID = PersonID;
		siCharID = CharID;

		siVillageUnique = VillageUnique;
		siAddListIndex = AddListIndex;

		siMonsterKind = MonsterKind;
		siMonsterNum = MonsterNum;
		siMonsterLevel = MonsterLevel;
	}
};

//-----------------------------------------------------
// 결과를 리턴함.
//-----------------------------------------------------
class sDBResponse_HuntMonsterFieldListAdd
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 시전행수 PersonID
	SI32				siCharID;			// 시전행수 CharUnique

	SI32				siVillageUnique;

	SI16				siAddListIndex;

	SI16				siMonsterKind;
	SI16				siMonsterNum;
	SI16				siMonsterLevel;

	sDBResponse_HuntMonsterFieldListAdd()
	{
		memset( this, 0, sizeof( sDBResponse_HuntMonsterFieldListAdd ) );
	}
};


//-----------------------------------------------------
// 수렵장에 넣어진 동물과 마리수를 리스트에서 삭제한다. ( 리스트에서 삭제 )
//-----------------------------------------------------
class sDBRequest_HuntMonsterFieldListDel
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID;

	SI32				siVillageUnique;

	SI16				siDelListIndex;

	sDBRequest_HuntMonsterFieldListDel()
	{
		ZeroMemory(this, sizeof(sDBRequest_HuntMonsterFieldListAdd));
	}

	sDBRequest_HuntMonsterFieldListDel( SI32 PersonID, SI32 CharID, SI32 VillageUnique, SI16 DelListIndex )
	{
		packetHeader.usSize = sizeof( sDBRequest_HuntMonsterFieldListDel );
		packetHeader.usCmd	= DBMSG_REQUEST_HUNTMONSTERFIELDLISTDEL;

		siPersonID = PersonID;
		siCharID = CharID;

		siVillageUnique = VillageUnique;

		siDelListIndex = DelListIndex;	
	}
};

//-----------------------------------------------------
// 결과를 리턴함.
//-----------------------------------------------------
class sDBResponse_HuntMonsterFieldListDel
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 시전행수 PersonID
	SI32				siCharID;			// 시전행수 CharUnique

	SI32				siVillageUnique;

	SI16				siDelListIndex;

	sDBResponse_HuntMonsterFieldListDel()
	{
		memset( this, 0, sizeof( sDBResponse_HuntMonsterFieldListDel ) );
	}
};

//-----------------------------------------------------
// 수렵장 창고에 동물을 넣는다.
//-----------------------------------------------------
class sDBRequest_HuntStorageMonsterIn
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siPersonID;
	SI32				siCharID;
	
	SI16				siFromPersonInventoryItemIndex;
	SI16				siToHuntStorageInventoryItemIndex;

	SI32				siVillageUnique;
	SI16				siItemUnique;
	SI16				siItemNum;

	sDBRequest_HuntStorageMonsterIn()
	{
		ZeroMemory(this, sizeof(sDBRequest_HuntStorageMonsterIn));
	}

	sDBRequest_HuntStorageMonsterIn( SI32 PersonID, SI32 CharID,
						SI16 FromPersonInventoryItemIndex, SI16 ToHuntStorageInventoryItemIndex,
						SI32 VillageUnique, SI16 ItemUnique, SI16 ItemNum )
	{
		packetHeader.usSize = sizeof( sDBRequest_HuntStorageMonsterIn );
		packetHeader.usCmd	= DBMSG_REQUEST_HUNTSTORAGEMONSTERIN;

		siCharID		= CharID;
		siPersonID		= PersonID;

		siToHuntStorageInventoryItemIndex = ToHuntStorageInventoryItemIndex;
		siFromPersonInventoryItemIndex = FromPersonInventoryItemIndex;

		siVillageUnique	= VillageUnique;

		siItemUnique	= ItemUnique;
		siItemNum		= ItemNum;
	}
};


class sDBResponse_HuntStorageMonsterIn
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 시전행수 PersonID
	SI32				siCharID;			// 시전행수 CharUnique

	SI32				siVillageUnique;
	
	SI16				siFromPersonInventoryItemIndex;
	SI16				siFromPersonInventoryItemUnique;
	SI16				siFromPersonInventoryItemNum;

	SI16				siToHuntStorageInventoryItemIndex;
	SI16				siToHuntStorageInventoryItemUnique;
	SI16				siToHuntStorageInventoryItemNum;
	
	sDBResponse_HuntStorageMonsterIn()
	{
		ZeroMemory(this, sizeof(sDBResponse_HuntStorageMonsterIn));
	}	
};

//-----------------------------------------------------
// 수렵장 창고에 들어가 있는 동물을 빼낸다.
//-----------------------------------------------------
class sDBRequest_HuntStorageMonsterOut
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siPersonID;
	SI32				siCharID;
	
	SI16				siFromHuntStorageInventoryItemIndex;
	SI16				siToPersonInventoryItemIndex;
	
	SI32				siVillageUnique;
	SI16				siItemUnique;
	SI16				siItemNum;

	sDBRequest_HuntStorageMonsterOut()
	{
		ZeroMemory(this, sizeof(sDBRequest_HuntStorageMonsterOut));
	}

	sDBRequest_HuntStorageMonsterOut( SI32 PersonID, SI32 CharID,
						SI16 FromHuntStorageInventoryItemIndex, SI16 ToPersonInventoryItemIndex,
						SI32 VillageUnique, SI16 ItemUnique, SI16 ItemNum )
	{
		packetHeader.usSize = sizeof( sDBRequest_HuntStorageMonsterOut );
		packetHeader.usCmd	= DBMSG_REQUEST_HUNTSTORAGEMONSTEROUT;

		siPersonID		= PersonID;
		siCharID		= CharID;

		siFromHuntStorageInventoryItemIndex = FromHuntStorageInventoryItemIndex;
		siToPersonInventoryItemIndex = ToPersonInventoryItemIndex;

		siVillageUnique	= VillageUnique;

		siItemUnique	= ItemUnique;
		siItemNum		= ItemNum;
	}
};

class sDBResponse_HuntStorageMonsterOut
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 시전행수 PersonID
	SI32				siCharID;			// 시전행수 CharUnique

	SI32				siVillageUnique;
		
	SI16				siFromHuntStorageInventoryItemIndex;
	SI16				siFromHuntStorageInventoryItemUnique;
	SI16				siFromHuntStorageInventoryItemNum;

	SI16				siToPersonInventoryItemIndex;
	SI16				siToPersonInventoryItemUnique;
	SI16				siToPersonInventoryItemNum;

	sDBResponse_HuntStorageMonsterOut()
	{
		ZeroMemory(this, sizeof(sDBResponse_HuntStorageMonsterOut));
	}	
};

class sDBRequest_HuntStorageDecreaseDollItemNum
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;

	SI16				siHuntStorageItemUnique[ MAX_HUNT_LINEUP_NUMBER ];
	SI16				siHuntStorageItemIndex[ MAX_HUNT_LINEUP_NUMBER ];
	SI16				siHuntStorageItemDecreaseNum[ MAX_HUNT_LINEUP_NUMBER ];

	sDBRequest_HuntStorageDecreaseDollItemNum()
	{
		ZeroMemory(this, sizeof(sDBRequest_HuntStorageDecreaseDollItemNum));
	};

	sDBRequest_HuntStorageDecreaseDollItemNum( 
		SI32 VillageUnique, 
		SI16 *pHuntStorageItemUnique,
		SI16 *pHuntStorageItemIndex,
		SI16 *pHuntStorageItemDecreaseNum
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_HuntStorageDecreaseDollItemNum );
		packetHeader.usCmd	= DBMSG_REQUEST_HUNTSTORAGEDECREASEDOLLITEMNUM;

		siVillageUnique = VillageUnique;
		
		memcpy( siHuntStorageItemUnique, pHuntStorageItemUnique, sizeof( SI16 ) * MAX_HUNT_LINEUP_NUMBER );
		memcpy( siHuntStorageItemIndex, pHuntStorageItemIndex, sizeof( SI16 ) * MAX_HUNT_LINEUP_NUMBER );
		memcpy( siHuntStorageItemDecreaseNum, pHuntStorageItemDecreaseNum, sizeof( SI16 ) * MAX_HUNT_LINEUP_NUMBER );
	}
};

class sDBResponse_HuntStorageDecreaseDollItemNum
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;

	SI32				siVillageUnique;

	SI16				siHuntStorageItemUnique[ MAX_HUNT_LINEUP_NUMBER ];
	SI16				siHuntStorageItemIndex[ MAX_HUNT_LINEUP_NUMBER ];	
	SI16				siHuntStorageItemNum[ MAX_HUNT_LINEUP_NUMBER ];

	sDBResponse_HuntStorageDecreaseDollItemNum()
	{
		memset( this, 0, sizeof( sDBResponse_HuntStorageDecreaseDollItemNum ) );
	}
};

//-----------------------------------------------------
// 수렵장에 들어가서 죽이고 남은 동물들을 창고에 아이템으로 다시 넣어준다.
//-----------------------------------------------------
class sDBRequest_HuntLeftMonsterStgIn
{
public:
	sPacketHeader		packetHeader;

	SI32				siVillageUnique;
	
	SI16				siToHuntStorageInventoryItemIndex;

	SI16				siItemUnique;
	SI16				siItemNum;

	sDBRequest_HuntLeftMonsterStgIn( SI32 VillageUnique, SI16 ToHuntStorageInventoryItemIndex, SI16 ItemUnique, SI16 ItemNum )
	{
		packetHeader.usSize = sizeof( sDBRequest_HuntLeftMonsterStgIn );
		packetHeader.usCmd	= DBMSG_REQUEST_HUNTLEFTMONSTERSTGIN;

		siVillageUnique = VillageUnique;

		siToHuntStorageInventoryItemIndex = ToHuntStorageInventoryItemIndex;

		siItemUnique = ItemUnique;
		siItemNum = ItemNum;
	}
};

class sDBResponse_HuntLeftMonsterStgIn
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siResult;

	SI32				siVillageUnique;

	SI16				siToHuntStorageInventoryItemIndex;
	
	SI16				siItemUnique;
	SI16				siItemNum;

	sDBResponse_HuntLeftMonsterStgIn()
	{
		ZeroMemory( this, sizeof( sDBResponse_HuntLeftMonsterStgIn ) );
	}
};

enum INSTANCEMAPPOINT_TYPE
{
	INSTANCEMAPPOINT_TYPE_GUILD = 1,
};

//인스턴스 던전 명예의 전당 리스트
//명예의 전당 리스트를 가지고 온다.
struct HOFList
{
	// 군주력 - 년도
	UI08				uiYear;
	//명예의 전당 길드 유니크
	SI32				siGuildUnique;
	//명예의 전당 길드 마크 인덱스
	SI32				siGuildMarkindex;
	//명예의 전당 길드 이름
	TCHAR				szHOFGuildName[MAX_GUILD_NAME];
	//명예의 전당 획득 점수
	SI32				siClearPoint;
	//명예의 전당 클리어 시간
	SI32			ClearTime;
	//명예의 전당 클리어 날짜
	SYSTEMTIME			ClearDateTime;

	HOFList()
	{
		ZeroMemory(this, sizeof(HOFList));
	}
};

#define MAX_HOF_PAGE 10//최대 패이지
#define MAX_HOF_RANKCOUNT 10//최대 랭킹 10위까지.

class sDBRequest_HOFList
{
public:
	sPacketHeader		packetHeader;
	UI16				uiCharID;

	SI16				siType;					// 타입
	SI32				siRequestPageNum;		// 요청 페이지 번호
	SI08				siPageMaxCount;			// 페이당 개수

	sDBRequest_HOFList( UI16 charid, SI16 type, SI32 RequestPage )
	{
		packetHeader.usSize = sizeof( sDBRequest_HOFList );
		packetHeader.usCmd	= DBMSG_REQUEST_HOFLIST;

		uiCharID			= charid;

		siType				= type;
		siRequestPageNum	= RequestPage;
		siPageMaxCount		= MAX_HOF_PAGE;
	}
};
class sDBResponse_HOFList
{
public:
	sPacketHeader		packetHeader;
	UI16				uiCharID;

	SI32				siResult;

	SI16				siType;				// 타입
	
	SI32				siRequestPage;		// 요청한 페이지	
	HOFList    HOFrList[MAX_HOF_PAGE];		// 결과 데이터
	SI32				siMaxPage;

	sDBResponse_HOFList()
	{
		ZeroMemory(this, sizeof(sDBResponse_HOFList));
	}
};

class sDBRequest_YearRanking
{
public:
	sPacketHeader		packetHeader;
	UI16				uiCharID;
	SI16				siType;			// 타입 INSTANCEMAPPOINT_TYPE_GUILD
	UI08				uiYear;			// 해당 년도 (게임시간)
	bool				bReward;		// 보상을 해야하는가? (서버에서 사용하는 값)
	SI32				sRequest_Page;

	sDBRequest_YearRanking( UI16 charid, SI16 type, UI08 Year, bool reward, SI32 Request_Page)
	{
		packetHeader.usSize = sizeof( sDBRequest_YearRanking );
		packetHeader.usCmd = DBMSG_REQUEST_YEARRANKING;
		uiCharID		= charid;
		siType			= type;
		uiYear			= Year;//현제 군주력
		bReward			= reward;
		sRequest_Page	= Request_Page;//요청 페이지 
	}
};

class sDBResponse_YearRanking
{
public:
	sPacketHeader		packetHeader;
	UI16				uiCharID;
	SI32				siResult;
	SI16				siType;			// 타입 INSTANCEMAPPOINT_TYPE_GUILD
	UI08				uiYear;			// 해당 년도 (게임시간)
	// 결과 데이터
	HOFList    HOFrList[MAX_HOF_RANKCOUNT];

	bool				bReward;		// 보상을 해야하는가? (서버에서 사용하는 값)
	SI32				siRequestPage;	// 요청한 페이지	
	SI32				siMaxPage;		// 최대 페이지
	sDBResponse_YearRanking()
	{
		ZeroMemory(this, sizeof(sDBResponse_YearRanking));
	}
};

class sDBRequest_InstanceMapPoint_Insert
{
public:
	sPacketHeader		packetHeader;

	SI16				siType;			// 타입 INSTANCEMAPPOINT_TYPE_GUILD
	SI32				siUnique;		// 유니크 (길드 or )
	SI32				siPoint;		// 점수
	UI08				uiYear;			// 해당 년도 (게임시간) 
	SI32				siClearTime;	// 맵을 클리어하는데 걸린시간 
	SYSTEMTIME			stClearTime;	// 클리어한 시각 (현실시간)

	sDBRequest_InstanceMapPoint_Insert( SI16 type, SI32 unique, SI32 point, UI08 year, SI32 cleartime, SYSTEMTIME& Time )
	{
		packetHeader.usSize = sizeof( sDBRequest_InstanceMapPoint_Insert );
		packetHeader.usCmd = DBMSG_REQUEST_INSTANCEMAPPOINT_INSERT;

		siType		= type;
		siUnique	= unique;
		siPoint		= point;
		uiYear		= year;
		siClearTime	= cleartime;
		memcpy(&stClearTime, &Time, sizeof(stClearTime));
	}
};

class sDBResponse_InstanceMapPoint_Insert
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	sDBResponse_InstanceMapPoint_Insert()
	{
		ZeroMemory(this, sizeof(sDBResponse_InstanceMapPoint_Insert));
	}
};

#endif