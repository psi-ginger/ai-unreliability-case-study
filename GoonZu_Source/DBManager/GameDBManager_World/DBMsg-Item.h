#ifndef _DBMSGITEM_H
#define _DBMSGITEM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
//#include "..\..\GoonZuWorld\Common\Item\ItemCommon\ItemUnit.h"

//---------------------------------------------
// 인벤토리내에서 아이템 만들기.
//---------------------------------------------
class sDBRequest_MakeItemInv
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// Person
	SI32				siPersonID;				// 물건을 만드는 사람. 
	SI32				siMaterialPersonID;		// 물건을 주문한 사람. (원료가 이 사람에게서 빠지고 완성품도 이사람에게 간다.)

	// Material 
	DBMaterialInfo		clMaterial[MAX_MATERIAL_NUMBER_FOR_DB];
	// Tool 
//	DBToolInfo			clTool[MAX_TOOL_NUMBER];

	// Output
	SI16				siOutputPos;		// 생산품이 위치할 공간. 
	cltItem				clItem;				// 생산품의 정보. (개수 포함. ) 
	bool				bCanPileSwitch;		// 생산품이 집적가능한가 ?
	bool				bAlmostFailSwitch;	// 거의 실패한 제품인가 ?
	bool				bCreateSID;			// 생산품의 SID 적용여부 - by LEEKH 1007.10.11
	SI16				siLuckQuality;		// 행운에 의한 퀄러티 향상. 
	
	GMONEY				siFee;				// 주문자가 있는 경우 주문자의 소지금에서 이 금액을 빼서 제조자에게 전달. 
	SI16				siOrderNumber;		// 주문자가 만들기를 희망한 개수. 
	SI16				siItemUnique;

	bool				bNPCMake;

	SI16				siMakerServerUnique;

	// 제작의뢰 일경우 표시 
	bool				m_bRequest_Want_Make_Item	;	// 아이템 제작을 원한다 -> 제조의뢰 개선 

	sDBRequest_MakeItemInv()
	{
		ZeroMemory(this, sizeof(sDBRequest_MakeItemInv));
	}

	sDBRequest_MakeItemInv(SI32 id, SI32 personid, SI32 materialpersonid, 
						DBMaterialInfo* pclinfo, 
						//DBToolInfo* pcltool,
						SI32 outputpos, cltItem* pclitem, bool bcanpileswitch, bool balmostfailswitch, bool bcreatesid, SI16 luckquality,
						GMONEY fee, SI16 ordernum, SI16 itemunique, bool npcmake = false, SI16 MakerServerUnique = 0,bool bRequest_Want_Make_Item = false )
	{
		packetHeader.usSize = sizeof( sDBRequest_MakeItemInv );
		packetHeader.usCmd	= DBMSG_REQUEST_MAKEITEMINV;

		usCharID		= id;

		siPersonID			= personid;
		siMaterialPersonID	= materialpersonid;

		for(SI32 i = 0;i < MAX_MATERIAL_NUMBER_FOR_DB;i++)
		{
			clMaterial[i].Set(&pclinfo[i]);
		}

/*		for(i = 0;i < MAX_TOOL_NUMBER;i++)
		{
			clTool[i].Set(&pcltool[i]);
		}*/

		siOutputPos	= outputpos;
		clItem.Set(pclitem);
		bCanPileSwitch		= bcanpileswitch;
		bAlmostFailSwitch	= balmostfailswitch;
		bCreateSID			= bcreatesid;
		siLuckQuality		= luckquality;
		siFee				= fee;
		siOrderNumber		= ordernum;
		siItemUnique		= itemunique;
		bNPCMake			= npcmake;

		// 신기품 이상 제작시 제작자의 서버유니클 전송한다.
		siMakerServerUnique = MakerServerUnique;
		// 제조의뢰 개선으로 인한 제작을 원했을 경우 
		m_bRequest_Want_Make_Item	=	bRequest_Want_Make_Item	;
	}
};


class sDBResponse_MakeItemInv
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.

	// Person
	SI32				siPersonID;				// 물건을 만드는 사람. 
	SI32				siMaterialPersonID;		// 물건을 주문한 사람. (원료가 이 사람에게서 빠지고 완성품도 이사람에게 간다.) 

	// Output
	SI16				siOutputPos;		// 생산품이 위치할 공간. 
	SI16				siAddedNum;			// 추가된 아이템 개수. 
	cltItem				clItem;				// 생산품의 정보. (개수 포함. ) 

	bool				bAlmostFailSwitch;	// 거의 실패한 제품인가 ?
	SI16				siLuckQuality;		// 행운에 의한 퀄러티 향상. 

	GMONEY				siFee;				// 주문자가 있는 경우 주문자의 소지금에서 이 금액을 빼서 제조자에게 전달. 
	GMONEY				siPersonIDMoney;
	GMONEY				siMaterialPersonIDMoney;
	SI16				siOrderNumber;		// 주문자가 만들기를 희망한 개수. 
	SI16				siItemUnique;

	bool				bNPCMake;
	// 제작의뢰 일경우 표시 
	bool				m_bRequest_Want_Make_Item	;	// 아이템 제작을 원한다 -> 제조의뢰 개선

	sDBResponse_MakeItemInv()
	{
		ZeroMemory(this, sizeof(sDBResponse_MakeItemInv));
	}
};


//---------------------------------------------
// 인벤토리내에서 아이템 만들기.
//---------------------------------------------
class sDBRequest_EnchantItemInv
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siSuccess;

	// Person
	SI32				siPersonID;				// 물건을 만드는 사람. 
	SI32				siMaterialPersonID;		// 물건을 주문한 사람. (원료가 이 사람에게서 빠지고 완성품도 이사람에게 간다.) 	


	// Material 
	DBMaterialInfo		clMaterial[MAX_ENCHANT_NUMBER];

	// Output
	SI16				siOutputPos;		// 인챈트할 물품의 위치
	cltItem				clItem;				// 바뀐 인챈트물품의 정보 저장

	GMONEY				siFee;				// 주문자가 있는 경우 주문자의 소지금에서 이 금액을 빼서 제조자에게 전달. 

    bool				bCreateSID;

	sDBRequest_EnchantItemInv()
	{
		ZeroMemory(this, sizeof(sDBRequest_MakeItemInv));
	}

	sDBRequest_EnchantItemInv(SI32 id, SI32 Success,SI32 personid,  SI32 materialpersonid, 
		DBMaterialInfo* pclinfo, SI32 outputpos, cltItem* pclitem,GMONEY fee, bool bcreatesid)
	{
		packetHeader.usSize = sizeof( sDBRequest_MakeItemInv );
		packetHeader.usCmd	= DBMSG_REQUEST_ENCHANTITEMINV;

		usCharID		= id;

		siPersonID			= personid;
		siMaterialPersonID	= materialpersonid;

		for(SI32 i = 0;i < MAX_MATERIAL_NUMBER;i++)
		{
			clMaterial[i].Set(&pclinfo[i]);
		}

		siOutputPos	= outputpos;
		clItem.Set(pclitem);		
		siFee				= fee;
		siSuccess = Success;

		bCreateSID = bcreatesid;
	}
};


class sDBResponse_EnchantItemInv
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siSuccess;

	SI32				siResult;				// 결과값.

	// Person
	SI32				siPersonID;				// 물건을 만드는 사람. 
	SI32				siMaterialPersonID;		// 물건을 주문한 사람. (원료가 이 사람에게서 빠지고 완성품도 이사람에게 간다.) 

	// Output
	SI16				siOutputPos;		// 바뀐 물건의 위치
	cltItem				clItem;				// 물건의 정보. (개수 포함. ) 



	GMONEY				siFee;				// 주문자가 있는 경우 주문자의 소지금에서 이 금액을 빼서 제조자에게 전달. 
	GMONEY				siPersonIDMoney;
	GMONEY				siMaterialPersonIDMoney;





	sDBResponse_EnchantItemInv()
	{
		ZeroMemory(this, sizeof(sDBResponse_EnchantItemInv));
	}
};


//------------------------------------------
// 인벤토리내 아이템 기간 연장.
//------------------------------------------

class sDBRequest_EnchantUseDayPlus
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	// Person
	SI32				siPersonID;				// 아이템 주인. 

	// Output
	SI16				siOutputPos;		// 인챈트할 물품의 위치
	cltItem				clItem;				// 바뀔 인챈트물품의 정보 

	SI16				siOutputMaterialPos;
	cltItem				clMaterialItem;

	sDBRequest_EnchantUseDayPlus()
	{
		ZeroMemory(this, sizeof(sDBRequest_EnchantUseDayPlus));
	}

	sDBRequest_EnchantUseDayPlus(SI32 id, SI32 personid,  
		SI16 outputpos, cltItem* pclitem , SI16 outputmaterialpos , cltItem* pclMaterialItem)
	{
		packetHeader.usSize = sizeof( sDBRequest_EnchantUseDayPlus );
		packetHeader.usCmd	= DBMSG_REQUEST_ENCHANT_USEDAYPLUS;

		usCharID		= id;

		siPersonID			= personid;

		siOutputPos	= outputpos;
		clItem.Set(pclitem);
		siOutputMaterialPos = outputmaterialpos;
		clMaterialItem.Set(pclMaterialItem);

	}
};


class sDBResponse_EnchantUseDayPlus
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;				// 결과값.

	// Person
	SI32				siPersonID;				// 물건을 만드는 사람. 

	// Output
	SI16				siOutputPos;		// 바뀐 물건의 위치
	cltItem				clItem;				// 물건의 정보. (개수 포함. ) 

	// OutputMaterial
	SI16				siOutputMaterialPos;	// 바뀐 재료의 위치
	cltItem				clMaterialItem;			// 바뀐 재료의 정보

	sDBResponse_EnchantUseDayPlus()
	{
		ZeroMemory(this, sizeof(sDBResponse_EnchantUseDayPlus));
	}
};

//------------------------------------------
// 아이템 인첸트 속성 변경
//------------------------------------------
class sDBRequest_ChangeEnchant
{
public:
	sPacketHeader	packetHeader;

	UI16			usCharID;				// 서버에 존재하는 ID
	SI32			siPersonID;				// Unique한 ID
	
	SI16			siOutputPos;			// 인첸트 속성이 변경될 아이템의 인벤위치
	cltItem			clItem;					// 인첸트 속성이 변경될 아이템

	SI16			siOutputMaterialPos;	// 인첸트 속성 변경 아이템의 인벤위치
	cltItem			clMaterialItem;			// 인첸트 속성 변경 아이템


	sDBRequest_ChangeEnchant()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeEnchant));
	}

	sDBRequest_ChangeEnchant(
		SI32 id, SI32 personid,  
		SI16 outputpos, cltItem* pclitem, 
		SI16 outputmaterialpos, cltItem* pclMaterialItem
	)
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeEnchant );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGEENCHANT;

		usCharID			= id;
		siPersonID			= personid;

		siOutputPos			= outputpos;
		clItem.Set(pclitem);

		siOutputMaterialPos = outputmaterialpos;
		clMaterialItem.Set(pclMaterialItem);
	}
};

class sDBResponse_ChangeEnchant
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;				// 결과값.

	// Person
	SI32				siPersonID;				// 물건을 만드는 사람. 

	// Output
	SI16				siOutputPos;			// 인첸트 속성이 변경될 아이템의 인벤위치
	cltItem				clItem;					// 인첸트 속성이 변경될 아이템

	// OutputMaterial
	SI16				siOutputMaterialPos;	// 인첸트 속성 변경 아이템의 인벤위치
	cltItem				clMaterialItem;			// 인첸트 속성 변경 아이템


	sDBResponse_ChangeEnchant()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeEnchant));
	}
};

//------------------------------------------
// 인벤토리내 아이템을 수리. 
//------------------------------------------
class sDBRequest_InvRepair
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// from
	SI32				siPersonID;
	SI16				siFromPos;
	
	// Item
	cltItem				clItem;

	// 수리자.(마스터) 
	SI32				siRepairPersonID;
	GMONEY				siRepairMoney;		// 수리비. 
	SI32				siNPCVillageUnique;	// NPC가 수리를 하는경우 NPC가 속한 마을 UNIQUE


	sDBRequest_InvRepair()
	{
		ZeroMemory(this, sizeof(sDBRequest_InvRepair));
	}

	sDBRequest_InvRepair(SI32 id, SI32 personid, SI32 frompos, cltItem* pclitem, SI32 repairpersonid, GMONEY repairmoney, SI32 npcvillageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_InvRepair);
		packetHeader.usCmd = DBMSG_REQUEST_INVREPAIR;
		usCharID		= id;

		siPersonID	= personid;
		siFromPos	= frompos;
	
		clItem.Set(pclitem);

		siRepairPersonID	= repairpersonid;
		siRepairMoney		= repairmoney;
		siNPCVillageUnique	= npcvillageunique;
	}
};

class sDBResponse_InvRepair
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	// from
	SI32				siPersonID;
	SI16				siFromPos;

	// Item
	cltItem				clItem;

	// 수리자.(마스터) 
	SI32				siRepairPersonID;
	GMONEY				siRepairMoney;		// 수리비. 
	GMONEY				siRepairerMoney;	// 수리자의 현재 보유 자금 .

	//의뢰자의 자금. 
	GMONEY				siPersonIDMoney;	// 수리 의뢰자의 보유 자금. 
	SI32				siNPCVillageUnique;	


	sDBResponse_InvRepair()
	{
		ZeroMemory(this, sizeof(sDBResponse_InvRepair));
	}
};


//--------------------------------------
// 아이템 정보를 변경한다.
//--------------------------------------
class sDBResponse_SetItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	SI32				siPersonID;
	SI16				siPos;
	cltItem				clItem;

	sDBResponse_SetItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetItem));
	}
};

//------------------------------------------
// 부적을 장비에 부착시킨다. 
//------------------------------------------
class sDBRequest_Enchant
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// from person
	SI32				siPersonID;

	// from
	SI16				siFromPos;
	SI16				siFromItemUnique;		// 부적의 유니크.

	// to
	SI16				siToPos;				// 장비의 위치. 
	cltItem				clToItem;				// 장비의 정보. 

	sDBRequest_Enchant()
	{
		ZeroMemory(this, sizeof(sDBRequest_Enchant));
	}

	sDBRequest_Enchant(SI32 id, SI32 personid, SI16 frompos, SI16 fromitemunique, SI16 topos, cltItem* pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_Enchant);
		packetHeader.usCmd = DBMSG_REQUEST_ENCHANT;
		usCharID		= id;

		siPersonID	= personid;

		siFromPos			= frompos;
		siFromItemUnique	= fromitemunique;

		siToPos				= topos;
		clToItem.Set(pclitem);
	}
};


class sDBResponse_Enchant
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	// person
	SI32				siPersonID;

	// from 
	SI16				siFromPos;
	cltItem				clFromItem;

	// to
	SI16				siToPos;
	cltItem				clToItem;

	sDBResponse_Enchant()
	{
		ZeroMemory(this, sizeof(sDBResponse_Enchant));
	}
};

//------------------------------------------
// 아이템을 땅에 버린다.
//------------------------------------------
class sDBRequest_DropItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// from
	SI32				siPersonID;
	SI16				siFromPos;

	// Item
	cltItem				clItem;

	// where
	SI16				siMapIndex;
	SI16				siX;
	SI16				siY;

	sDBRequest_DropItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_DropItem));
	}

	sDBRequest_DropItem(SI32 id, SI32 personid, SI32 frompos, cltItem* pclitem, SI16 mapindex, SI16 x, SI16 y)
	{
		packetHeader.usSize = sizeof( sDBRequest_DropItem);
		packetHeader.usCmd = DBMSG_REQUEST_DROPITEM;
		usCharID		= id;

		siPersonID	= personid;
		siFromPos	= frompos;
	
		clItem.Set(pclitem);

		siMapIndex	= mapindex;
		siX			= x;
		siY			= y;
	}
};


class sDBResponse_DropItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	// from
	SI32				siPersonID;
	SI16				siFromPos;

	// Item
	cltItem				clItem;
	cltItem				clChangedItem;		// 변화된 아이템 정보. 

	// where
	SI16				siMapIndex;
	SI16				siX;
	SI16				siY;

	sDBResponse_DropItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_DropItem));
	}
};


//------------------------------------------
// 특정 아이템을 Person의 특정 아이템 위치에  넣어줄 것을 요청. 
//------------------------------------------

//------------------------------------------
// ***** DANGER ***** -> define 값을 바꾸지 마세요
//------------------------------------------

#define INPUTITEMPERSON_OPTION_NONE								0	// 특이 사항 없음. 	
#define INPUTITEMPERSON_OPTION_WAR								1	// 전리품으로 획득한 아이템. 
#define INPUTITEMPERSON_OPTION_MASTER							2	// 게임 마스터에 의해 추가되었다. 
#define INPUTITEMPERSON_OPTION_PICKUP							3	// 땅에서 주웠다. 
#define INPUTITEMPERSON_OPTION_QUEST							4	// 퀘스트를 통해서 얻었다. 
#define INPUTITEMPERSON_OPTION_FAMELEVEL						5	// 명성레벨 향상으로 얻음. 
#define INPUTITEMPERSON_OPTION_SPEECIALQUEST					6	// 흥부박이벤트에서 얻음. 
#define INPUTITEMPERSON_OPTION_FARM								7	// 농사중 얻음. 
#define INPUTITEMPERSON_OPTION_FARMSUMMON						8	// 농사중 얻음. 
#define INPUTITEMPERSON_OPTION_FISH								9	// 낚시중 얻음. 
#define INPUTITEMPERSON_OPTION_FISHSUMMON						10	// 낚시중 얻음. 
#define INPUTITEMPERSON_OPTION_BLACKARMY						11  // 흑의군단 이벤트(외적이벤트). 
#define INPUTITEMPERSON_OPTION_WORDASSEMBLE						12	// 흥부박 단어 조합으로 얻음. 
#define INPUTITEMPERSON_OPTION_FEAST							13  // 내자시에서 나감
#define INPUTITEMPERSON_OPTION_INSTANCEMAP						14  // 인스턴스 던전 보스 잡아서 얻음
#define INPUTITEMPERSON_OPTION_OSAKA							15  // 오사카 전투후 얻음 
#define	INPUTITEMPERSON_OPTION_KANGHWADO						16  // 강화도 방어 후 얻음 
#define INPUTITEMPERSON_OPTION_CRAZYEVENT						17  // 조선의 반격 이벤트로 얻음
#define INPUTITEMPERSON_OPTION_ROULETTEGAME						18  // 박대박 게임으로 얻음
#define INPUTITEMPERSON_OPTION_SEVENDAYEVENT					19  // 이색군주7요일인트로 얻음
#define INPUTITEMPERSON_OPTION_WENNYOLDMAN						20  // 이색군주7요일인트로 얻음
#define INPUTITEMPERSON_OPTION_RECOMMEND						21  // 추천인이 있고 일정 레벨 달성 
#define INPUTITEMPERSON_OPTION_EVENT							22	// 신년 이벤트에 의한 아이템 지급 
#define INPUTITEMPERSON_OPTION_CHIMERA_REWARD					23	// 키메라 한대친 보상
#define INPUTITEMPERSON_OPTION_LEVELUP_REWARD					24  // 레벨업에 대한 보상
#define INPUTITEMPERSON_OPTION_BEGINNERWEAPON					25	// 초보자 무기 퀘스트 의한
#define INPUTITEMPERSON_OPTION_MOFUMOFUEVENT					26	// 모후모후 이벤트 클리어의 의한
#define INPUTITEMPERSON_OPTION_BLACKWAR							27	// 흑의 군단 이벤트
#define INPUTITEMPERSON_OPTION_PARTYHALL_ITEM					28	// [진성] 파티홀. 공짜로 입는 의상 아이템. => 2008. 3. 12
#define INPUTITEMPERSON_OPTION_VOTE								29	// [영훈] 선거 시스템 : 선거 종료후 지급되는 아이템
#define INPUTITEMPERSON_OPTION_MAKEITEM_MATERIAL				30	// [강형] 로그용 - 아이템 생성시 소모되는 재료들의 결과를 전달한다.
#define INPUTITEMPERSON_OPTION_ENCHANT_MATERIAL					31	// [강형] 로그용 - 인챈트시 소모되는 재료들의 결과를 전달한다.
#define INPUTITEMPERSON_OPTION_SERVER_EVENT						32	// [진성] 서버 이벤트로 지급되는 아이템.
#define INPUTITEMPERSON_OPTION_OPENBOX							33	// [강형] 박스 개봉으로 얻은 아이템
#define INPUTITEMPERSON_OPTION_PARTYHALL_GIVE_ITEM				34	// [진성] 파티홀. 공짜로 입는 의상 아이템. => 2008. 3. 12
#define INPUTITEMPERSON_OPTION_DAILYQUEST						35	// [춘기] 데일리 퀘스트로 얻는 아이템
#define INPUTITEMPERSON_OPTION_DAILYQUEST2						36  // [춘기] 데일리 퀘스트2로 얻는 아이템
#define INPUTITEMPERSON_OPTION_DAILYQUEST3						37  // [춘기] 데일리 퀘스트3으로 얻는 아이템
#define INPUTITEMPERSON_OPTION_ITEMMALL							38  // [춘기] 아이템몰에서 얻은 아이템
#define INPUTITEMPERSON_OPTION_MUDANGPRESENT					39  // [희영] 선무당의 선물로 얻은 아이템
#define INPUTITEMPERSON_OPTION_CHAR_ROULETTE					40  // [강형] 캐릭터 룰렛으로 얻은 아이템
#define INPUTITEMPERSON_OPTION_AMBERBOX							41  // [진성] 호박상자.
#define INPUTITEMPERSON_OPTION_HELPER							42  // [진성] 도우미에게 주는 아이템.
#define INPUTITEMPERSON_OPTION_CHRISTMAS_ROULETTE				43  // [희영] 크리스마스 룰렛으로 얻은 아이템
#define INPUTITEMPERSON_OPTION_PVPLEAGUEPRIZE					44  // [성웅] PVP리그보상아이템.
#define INPUTITEMPERSON_OPTION_PVPLEAGUEPRIZE_SPECIAL			45  // [성웅] PVP리그 목표달성 보상아이템.
#define INPUTITEMPERSON_OPTION_NEWYEAREVENT_ROULETTE			46	// [춘기] 신년이벤트 운세룰렛으로 얻은 아이템.
#define INPUTITEMPERSON_OPTION_ELIXIR_CHANGE_ITEM				47	// [진성] 엘릭서 npc에세 교환받은 아이템.
#define INPUTITEMPERSON_OPTION_VALENTINEWAR_PRIZE				48	// [진성] 엘릭서 npc에세 교환받은 아이템.
#define INPUTITEMPERSON_OPTION_TREASUREEVENTITEM				49	// [성웅] 보물찾기 이벤트 아이템
#define INPUTITEMPERSON_OPTION_WEDDINGANNIVERSARY				50	// [영훈] 결혼 기념일 아이템 지급
#define INPUTITEMPERSON_OPTION_WEDDINGHALL_FLOWER				51	// [영훈] 결혼 화관
#define INPUTITEMPERSON_OPTION_WEDDING_FULLDRESS				52	// [영훈] 결혼 예복
#define INPUTITEMPERSON_OPTION_REPAIRITEM_MATERIAL				53	// [강형] 로그용 - 내구도 수리시 사용되는 재료들.
#define INPUTITEMPERSON_OPTION_DORMANCYACCOUNT					54	// [기형] 휴면 계정 보상 아이템
#define INPUTITEMPERSON_OPTION_SEQGMCOMMAND						55  // [춘기] /seq 명령어에 의해서 생성된 아이템
#define INPUTITEMPERSON_OPTION_BATTLEROYAL_WINNER				56  // [기형] 배틀로얄 - 우승자 보상
#define INPUTITEMPERSON_OPTION_BATTLEROYAL_KILLINGUSER			57  // [기형] 배틀로얄 - 유저 죽임
#define INPUTITEMPERSON_OPTION_SKILLBOOK_BUY					58  // [] 
#define INPUTITEMPERSON_OPTION_PARTYQUEST						59  // [종호] 파티퀘스트
#define INPUTITEMPERSON_OPTION_GIANTRACCOON_RESURRECT_EVENT		60  // [진성] 자이언트 라쿤 부활 이벤트로 얻는 아이템.
#define INPUTITEMPERSON_OPTION_ADDITIONALITEM_GET				61	// [지연] 생산/사냥시 원래 얻어지는 아이템 외에 이벤트 기간동안 추가적으로 얻게 되는 아이템 (예. 쿠키조각등)
#define INPUTITEMPERSON_OPTION_LIMITLEVELBOX					62	// [성웅] 레벨 보상 상자에서 아이템을 준다 
#define INPUTITEMPERSON_OPTION_VALENTINEDAY_PAKING				63	// [진성] 발렌타인데이 아이템 포장.
#define INPUTITEMPERSON_OPTION_VALENTINEDAY_BUY					64	// [진성] 발렌타인데이 아이템 구입.


class sDBRequest_InputItemToPerson
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 아이템을 받을 Person
	SI16				siItemPos;		// 아이템을 넣어야 할 위치.
	SI16				siInputItemPersonOption;	// 어떤 이유로 아이템을  넣게 되었는가?

	bool				bPileSwitch;	// 중첩 허용 여부. 	
	cltItem				clItem;			// Person에 넣을 아이템.
	
	SI32				siGiverPersonID;	// 아이템을 준 사용자. 

	bool				bCreateSID;		// SID를 생성해야 하는가?

	sDBRequest_InputItemToPerson()
	{
		ZeroMemory(this, sizeof(sDBRequest_InputItemToPerson));
	}

	sDBRequest_InputItemToPerson(SI32 id, SI32 personid, SI16 itempos, SI16 option, bool bpileswitch, cltItem* pclitem, SI32 giverpersonid, bool createsid)
	{
		packetHeader.usSize = sizeof( sDBRequest_InputItemToPerson );
		packetHeader.usCmd = DBMSG_REQUEST_INPUTITEMTOPERSON;
		usCharID		= id;

		siPersonID				= personid;
		siItemPos				= itempos;
		siInputItemPersonOption	= option;
		bPileSwitch				= bpileswitch;
		clItem.Set(pclitem);

		siGiverPersonID			= giverpersonid;
		bCreateSID				= createsid;
	}
};


class sDBResponse_InputItemToPerson
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		

	cltItem				clInputedItem;				// Person에 넣을 아이템. [2007. 10. 23 새로추가]
	
	SI16				siItemPos;
	SI16				siInputItemPersonOption;	// 어떤 이유로 아이템을  넣게 되었는가?
	SI16				siChangedNum;
	cltItem				clItem;			

	sDBResponse_InputItemToPerson()
	{
		ZeroMemory(this, sizeof(sDBResponse_InputItemToPerson));
	}
};


//------------------------------------------
// 특정 아이템을 Person의 특정 아이템 위치에서 뺀다. 
//------------------------------------------
class sDBRequest_OutputItemFromPerson
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 아이템을 받을 Person
	SI16				siItemPos;		// 아이템을 빼야 할 위치.
	cltItem				clItem;			// Person에 넣을 아이템.

	sDBRequest_OutputItemFromPerson()
	{
		ZeroMemory(this, sizeof(sDBRequest_OutputItemFromPerson));
	}

	sDBRequest_OutputItemFromPerson(SI32 id, SI32 personid, SI32 itempos, cltItem* pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_OutputItemFromPerson);
		packetHeader.usCmd = DBMSG_REQUEST_OUTPUTITEMFROMPERSON;
		usCharID		= id;

		siPersonID	= personid;
		siItemPos	= itempos;
		clItem.Set(pclitem);
	}
};


class sDBResponse_OutputItemFromPerson
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siItemPos;
	cltItem				clItem;			

	sDBResponse_OutputItemFromPerson()
	{
		ZeroMemory(this, sizeof(sDBResponse_OutputItemFromPerson));
	}
};

//------------------------------------------
// Person이 아이템 창간에 아이템을 이동시킨다. (인벤에서 갑옷창이나 그 반대와 같은 경우. ) 
//------------------------------------------
class sDBRequest_MoveItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 PersonID
	SI16				siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				clFromItem;			// 출발위치에 있는 아이템 
	bool				bFromItemPileSwitch;//출발 위치에있는 아이템이 집적이 가능한가 ?
	
	SI16				siToItemPos;		// 이동시킬 목적지.
	cltItem				clToItem;			// 이동시킬 목적지에 있는 아이템 

	sDBRequest_MoveItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_MoveItem));
	}

	sDBRequest_MoveItem(SI32 id, SI32 personid, 
						SI32 itemfrompos, cltItem* pclfromitem, bool fromitempileswitch, 
						SI32 toitempos, cltItem* pcltoitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_MoveItem );
		packetHeader.usCmd = DBMSG_REQUEST_MOVEITEM;
		usCharID		= id;

		siPersonID		= personid;
		siFromItemPos	= itemfrompos;
		clFromItem.Set(pclfromitem);
		bFromItemPileSwitch	= fromitempileswitch;

		siToItemPos		= toitempos;
		clToItem.Set(pcltoitem);

	}
};


class sDBResponse_MoveItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;			// 대상이 되는 PersonID
	SI16				siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				clFromItem;			// 출발위치에 있는 아이템 
	
	SI16				siToItemPos;		// 이동시킬 목적지.
	cltItem				clToItem;			// 이동시킬 목적지에 있는 아이템 

	cltItem				clMoveItem;			// 출발지에 있던 아이템 (이동시키는 아이템 정보) For Log - by LEEKH 2007.10.23

	sDBResponse_MoveItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_MoveItem));
	}
};




//------------------------------------------
// Person간에 아이템을 이동시킨다.
//------------------------------------------
#define MOVEITEMPERSON_NORMAL	1
#define MOVEITEMPERSON_PVP		2

class sDBRequest_MoveItemPerson
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siFromPersonID;		// 출발 Person
	SI16				siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				clFromItem;			// 출발위치에 있는 아이템 
	bool				bFromItemPileSwitch;//출발 위치에있는 아이템이 집적이 가능한가 ?
	TCHAR				szFromUserName[MAX_PLAYER_NAME];
	
	SI32				siToPersonID;		// 목적지 Person
	SI16				siToItemPos;		// 이동시킬 목적지.
	cltItem				clToItem;			// 이동시킬 목적지에 있는 아이템 
	TCHAR				szToUserName[MAX_PLAYER_NAME];

	SI32				siReason;			// 옮기는 이유?
	bool				bDestroy;			// true이면 이동시킬 목적지에 있는 아이템 사라진다.

	sDBRequest_MoveItemPerson()
	{
		ZeroMemory(this, sizeof(sDBRequest_MoveItemPerson));
	}

	sDBRequest_MoveItemPerson(SI32 id, 
							SI32 frompersonid, SI32 itemfrompos, cltItem* pclfromitem, bool fromitempileswitch, TCHAR* FromUserName,
							SI32 topersonid, SI32 toitempos, cltItem* pcltoitem, TCHAR* ToUserName, SI32 Reason, bool Destroy )
	{
		packetHeader.usSize = sizeof( sDBRequest_MoveItemPerson );
		packetHeader.usCmd = DBMSG_REQUEST_MOVEITEMPERSON;
		usCharID		= id;

		siFromPersonID	= frompersonid;
		siFromItemPos	= itemfrompos;
		clFromItem.Set(pclfromitem);
		bFromItemPileSwitch	= fromitempileswitch;
		MStrCpy( szFromUserName, FromUserName, MAX_PLAYER_NAME );

		siToPersonID	= topersonid;
		siToItemPos		= toitempos;
		clToItem.Set(pcltoitem);
		MStrCpy( szToUserName, ToUserName, MAX_PLAYER_NAME );

		siReason = Reason;
		bDestroy = Destroy;

	}
};


class sDBResponse_MoveItemPerson
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siFromPersonID;		// 출발점이 되는 PersonID
	SI16				siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				clFromItem;			// 출발위치에 있는 아이템 
	TCHAR				szFromUserName[MAX_PLAYER_NAME];
    	
	SI32				siToPersonID;		// 목적지가 되는 personID
	SI16				siToItemPos;		// 이동시킬 목적지.
	cltItem				clToItem;			// 이동시킬 목적지에 있는 아이템 
	TCHAR				szToUserName[MAX_PLAYER_NAME];

	SI32				siReason;			// 옮기는 이유?
	bool				bDestroy;			// true이면 이동시킬 목적지에 있는 아이템 사라진다.
	SI16				siDestroyItemUnique; // 옮기는 아이템이 파괴될 수 있어서 아이템 유니크를 따로 기억한다.

	//[추가 : 황진성 2007. 10. 24 주는 아이템.]
	cltItem				clMoveItem;			// 이동 아이템. 

	sDBResponse_MoveItemPerson()
	{
		ZeroMemory(this, sizeof(sDBResponse_MoveItemPerson));
	}
};

#define USEITEM_REASON_NONE						 0 // 일반적으로 사용
#define USEITEM_REASON_DATEOVER					 1 // 기간제 아이템 기간 끝
#define USEITEM_REASON_ROULLET					 2 // 박대박으로 사용
#define USEITEM_REASON_TICKET					 3 // 티켓들 사용
#define USEITEM_REASON_HORSECHANGE				 4 // 말들 변경으로 사용
#define USEITEM_REASON_BELONGITEM				 5 // 귀속아이템 파괴
#define USEITEM_REASON_DISASSEMBLE				 6 // 혹부리(2호식) 아이템 분해
#define USEITEM_REASON_PATYHALL					 7 // [진성] 파티홀. 파티장에서 준 아이템이라 캐릭터 초기화시 제거.
#define USEITEM_REASON_DAILYQUEST2				 8 // 데일리 퀘스트2 아이템 삭제
#define USEITEM_REASON_DAILYQUEST3				 9 // 데일리 퀘스트3 아이템 삭제
#define USEITEM_REASON_MAKE_MATERIAL			10 // 제조시 재료로 삭제
#define USEITEM_REASON_ENCHANT_MATERIAL 		11 // 인챈트시 재료로 삭제
#define USEITEM_REASON_OPENBOX_KEY				12 // 박스 개봉 키 사용
#define USEITEM_REASON_CHANGENAME				13	// 이름 변경이용권 사용
#define USEITEM_REASON_CHANGECHARKIND			14	// 캐릭터 변경 이용권 사용
#define USEITEM_REASON_PARTYHALLGIFT			15	// 캐릭터 변경 이용권 사용
#define USEITEM_REASON_SUMMONEVOLVE				16  // 소환수 진화 이용권 사용
#define USEITEM_REASON_SUMMONREVIVE				17	// 소환수 부활 인형 사용
#define USEITEM_REASON_SUMMONSTATUSINIT			18	// 소환수 스탯 초기화 이용권 사용
#define USEITEM_REASON_PERSONSHOP				19	// 개인상점이용권 사용
#define	USEITEM_REASON_INITSTATUSMODE_TICKET	20	// 캐릭터 스탯 초기화 이용권 사용
#define USEITEM_REASON_SKILLRESET_TICKET		21	// 스킬초기화 이용권 사용
#define USEITEM_REASON_ELIXIR_CHANGE_ITEM		22	//[진성] 엘릭서 NPC에게서 마력의 비약으로 교환된 아이템. 
#define USEITEM_REASON_ELIXIR_START_USE_ITEM	23	//[진성] 엘릭서 NPC에게서 마력의 비약으로 교환된 아이템. 
#define USEITEM_REASON_ARTIFACT_SLOT_CREATE		24	//[강형] 아티펙트 슬롯 결정체 사용
#define USEITEM_REASON_ARTIFACT_MIXTURE			25	//[강형] 아티펙트 조합에 사용
#define USEITEM_REASON_FESTIVAL_FOOD			26	//[진성] 축제 음식.
#define USEITEM_REASON_DURABILITY_ZERO			27  //[영훈] 내구도가 0이라서 삭제한다
#define USEITEM_REASON_CONTRIBUTION_ITEM_INPUT	28  //[진성] 건축물에 기부하여 소모된 아이템.
#define USEITEM_REASON_USEDAYPLUS				29  //[기형] 기간 연장 아이템 사용
#define USEITEM_REASON_VALENTINEDAY				30  //[진성] 발렌타인데이 이벤트용 버프 아이템 사용.
#define USEITEM_REASON_COMMISSION				31	// [기형] 제조 의뢰 이용권

//------------------------------------------
// Person이 아이템을 사용할 것을 요청. 
//------------------------------------------
class sDBRequest_UseItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 아이템을 사용할 Person
	SI16				siItemPos;		// 아이템을 사용할 위치.
	cltItem				clItem;			// Person이 사용할 아이템 정보. 

	SI16				siUseReason;	// 아이템 사용 이유

	sDBRequest_UseItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_UseItem));
	}

	sDBRequest_UseItem(SI32 id, SI32 personid, SI32 itempos, cltItem* pclitem,SI16 reason)
	{
		packetHeader.usSize = sizeof( sDBRequest_UseItem);
		packetHeader.usCmd = DBMSG_REQUEST_USEITEM;
		usCharID		= id;

		siPersonID	= personid;
		siItemPos	= itempos;
		clItem.Set(pclitem);

		siUseReason = reason ;
	}
};


class sDBResponse_UseItem
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siItemPos;
	cltItem				clItem;				// 사용되고 남은 아이템.
	cltItem				clUsedItem;			// 사용된 아이템. 

	SI16				siUseReason;

	sDBResponse_UseItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_UseItem));
	}
};


class sDBRequest_Use_SpaceBox_Item
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 아이템을 사용할 Person
	SI16				siItemPos;		// 아이템을 사용할 위치.
	cltItem				clItem;			// Person이 사용할 아이템 정보. 

	SI16				siUseReason;	// 아이템 사용 이유

	sDBRequest_Use_SpaceBox_Item()
	{
		ZeroMemory(this, sizeof(sDBRequest_Use_SpaceBox_Item));
	}

	sDBRequest_Use_SpaceBox_Item(SI32 id, SI32 personid, SI32 itempos, cltItem* pclitem,SI16 reason)
	{
		packetHeader.usSize = sizeof( sDBRequest_Use_SpaceBox_Item);
		packetHeader.usCmd  = DBMSG_REQUEST_USE_SPACEBOX_ITEM;
		usCharID			= id;

		siPersonID	= personid;
		siItemPos	= itempos;
		clItem.Set(pclitem);

		siUseReason = reason ;
	}
};


class sDBResponse_Use_SpaceBox_Item
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siItemPos;
	cltItem				clItem;				// 사용되고 남은 아이템.
	cltItem				clUsedItem;			// 사용된 아이템. 

	SI16				siUseReason;

	sDBResponse_Use_SpaceBox_Item()
	{
		ZeroMemory(this, sizeof(sDBResponse_Use_SpaceBox_Item));
	}
};

class sDBRequest_Use_Treasure_Item
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 아이템을 사용할 Person
	SI16				siItemPos;		// 아이템을 사용할 위치.
	cltItem				clItem;			// Person이 사용할 아이템 정보. 

	SI16				siUseReason;	// 아이템 사용 이유

	sDBRequest_Use_Treasure_Item()
	{
		ZeroMemory(this, sizeof(sDBRequest_Use_Treasure_Item));
	}

	sDBRequest_Use_Treasure_Item(SI32 id, SI32 personid, SI32 itempos, cltItem* pclitem,SI16 reason)
	{
		packetHeader.usSize = sizeof( sDBRequest_Use_Treasure_Item);
		packetHeader.usCmd	= DBMSG_REQUEST_USE_TREASURE_ITEM;
		usCharID			= id;

		siPersonID	= personid;
		siItemPos	= itempos;
		clItem.Set(pclitem);

		siUseReason = reason ;
	}
};


class sDBResponse_Use_Treasure_Item
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siItemPos;			
	cltItem				clItem;				// 사용되고 남은 아이템.
	cltItem				clUsedItem;			// 사용된 아이템. 

	SI16				siUseReason;

	sDBResponse_Use_Treasure_Item()
	{
		ZeroMemory(this, sizeof(sDBResponse_Use_Treasure_Item));
	}
};

//------------------------------------------
// 아이템을 NPC에게 처분할 것을 요청. 
//------------------------------------------
class sDBRequest_SellNPC
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 아이템을 팔 Person
	SI16				siItemPos;		// 아이템을 팔 위치.
	cltItem				clItem;			// Person이 팔 아이템 정보. 
	GMONEY				siPrice;		// 아이템의 단가. 
	SI32				siVillageUnique;// NPC가 위치한 마을 Unique

	//[추가 : 황진성 2007. 10. 24 로그에 이용할 NPC 판매 정보 추가. ]
	SI32				siNPCKind;		// NPC 종류.

	sDBRequest_SellNPC()
	{
		ZeroMemory(this, sizeof(sDBRequest_SellNPC));
	}

	sDBRequest_SellNPC(SI32 id, SI32 personid, SI32 itempos, cltItem* pclitem, GMONEY price, SI32 villageunique, SI32 NPCKind = 0)
	{
		packetHeader.usSize = sizeof( sDBRequest_SellNPC);
		packetHeader.usCmd = DBMSG_REQUEST_SELLNPC;
		usCharID		= id;

		siPersonID	= personid;
		siItemPos	= itempos;
		clItem.Set(pclitem);
		siPrice		= price;
		siVillageUnique = villageunique;
		
		siNPCKind = NPCKind;
	}
};


class sDBResponse_SellNPC
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siItemPos;
	cltItem				clItem;				// 팔고 남은 아이템.
	GMONEY				siMoney;			// 보유자금. 
	GMONEY				siChangedMoney;		// 판매로 인해 늘어난 자금. 
	SI32				siVillageUnique;	// NPC가 위치한 마을 Unique

	SI16				siIitemUnique;
	SI16				siItemNum;

	//[추가 : 황진성 2007. 10. 24 로그에 이용할 NPC 판매 정보 추가. ]
	cltItem				clSellItem;			// 판매하는 아이템.
	GMONEY				siSellMoney;		// 판매한 금액. 
	SI32				siNPCKind;			// NPC 종류.

	sDBResponse_SellNPC()
	{
		ZeroMemory(this, sizeof(sDBResponse_SellNPC));
	}
};


//------------------------------------------
// 아이템을 NPC에게서 구입할 것을 요청. 
//------------------------------------------
class sDBRequest_BuyNPC
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상  Person
	cltItem				clItem;			// 대상 아이템 정보. 
	bool				bPileSwitch;	// 집적 가능 여부. 
	GMONEY				siPrice;		// 아이템의 단가. 
	SI16				siItemPos;		// 아이템을 얻어올 인벤내 위치.

	//[추가 : 황진성 2007. 10. 24 로그에 이용할 NPC구입 정보 추가. ]
	SI32				siNPCkind;		// NPC 종류.

	sDBRequest_BuyNPC()
	{
		ZeroMemory(this, sizeof(sDBRequest_BuyNPC));
	}

	sDBRequest_BuyNPC(SI32 id, SI32 personid, cltItem* pclitem, bool bpilewitch, GMONEY price, SI32 itempos, SI32 NPCkind = 0)
	{
		packetHeader.usSize = sizeof( sDBRequest_BuyNPC);
		packetHeader.usCmd = DBMSG_REQUEST_BUYNPC;
		usCharID		= id;

		siPersonID	= personid;
		clItem.Set(pclitem);
		bPileSwitch	= bpilewitch;
		siPrice		= price;
		siItemPos	= itempos;

		siNPCkind	= NPCkind;
	}
};


class sDBResponse_BuyNPC
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siItemPos;
	cltItem				clItem;				// 남은 아이템.
	GMONEY				siMoney;			// 보유자금. 

	//[추가 : 황진성 2007. 10. 24 로그에 아용할 NPC구입 정보 추가. ]
	cltItem				clBuyItem;			// 구입하는 아이템.
	GMONEY				siBuyMoney;			// 구입에 사용하는 자금. 
	SI32				siNPCkind;			// NPC 종류.

	sDBResponse_BuyNPC()
	{
		ZeroMemory(this, sizeof(sDBResponse_BuyNPC));
	}
};


//------------------------------------------
// 내구도를 소모하였음을 요청. 
//------------------------------------------
class sDBRequest_WasteDurability
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// from
	SI32				siPersonID;
	SI16				siFromPos;

	// Item
	cltItem				clItem;


	sDBRequest_WasteDurability()
	{
		ZeroMemory(this, sizeof(sDBRequest_WasteDurability));
	}

	sDBRequest_WasteDurability(SI32 id, SI32 personid, SI32 frompos, cltItem* pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_WasteDurability);
		packetHeader.usCmd = DBMSG_REQUEST_WASTEDURABILITY;
		usCharID		= id;

		siPersonID	= personid;
		siFromPos	= frompos;
	
		clItem.Set(pclitem);
	}
};


class sDBResponse_WasteDurability
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	// from
	SI32				siPersonID;
	SI16				siFromPos;

	// Item
	cltItem				clItem;

	sDBResponse_WasteDurability()
	{
		ZeroMemory(this, sizeof(sDBResponse_WasteDurability));
	}
};

//KHY - 0227 - 결정체 (크리스탈) 제조 추가. - 내구도 소모를 DB로 저장.
//------------------------------------------
// 내구도를 소모하였음을 요청. 
//------------------------------------------
class sDBRequest_CrystalDurability
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// from
	SI32				siPersonID;
	SI16				siFromPos;

	// Item
	cltItem				clItem;


	sDBRequest_CrystalDurability()
	{
		ZeroMemory(this, sizeof(sDBRequest_CrystalDurability));
	}

	sDBRequest_CrystalDurability(SI32 id, SI32 personid, SI32 frompos, cltItem* pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_CrystalDurability);
		packetHeader.usCmd = DBMSG_REQUEST_CRYSTALDURABILITY;
		usCharID		= id;

		siPersonID	= personid;
		siFromPos	= frompos;
	
		clItem.Set(pclitem);
	}
};


class sDBResponse_CrystalDurability
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	// from
	SI32				siPersonID;
	SI16				siFromPos;

	// Item
	cltItem				clItem;

	sDBResponse_CrystalDurability()
	{
		ZeroMemory(this, sizeof(sDBResponse_CrystalDurability));
	}
};
class sDBRequest_UpgradeItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	// from
	SI16				siFromPos;
	SI16				siFromItemUnique;		// 혹부리 영감 이용권의 아이템 Unique

	// to
	SI16				siToPos;				// 장비의 위치. 
	cltItem				clToItem;				// 장비의 정보. 
	
	SI32				siVillageUnique;		// 마을에 세금 납부
	GMONEY				siAddCityHallMoney;

	bool				bSuccess;				// 성공 여부

	sDBRequest_UpgradeItem( bool Success, SI32 CharID, SI32 PersonID, SI16 FromPos, SI16 FromItemUnique, SI16 ToPos, cltItem *pclItem, SI32 VillageUnique, GMONEY	AddCityHallMoney )
	{
		packetHeader.usSize = sizeof( sDBRequest_UpgradeItem );
		packetHeader.usCmd = DBMSG_REQUEST_UPGRADEITEM;

		bSuccess = Success;

		siCharID = CharID;
		siPersonID = PersonID;

		siFromPos = FromPos;
		siFromItemUnique = FromItemUnique;

		siToPos = ToPos;
		clToItem.Set( pclItem );

		siVillageUnique = VillageUnique;
		siAddCityHallMoney = AddCityHallMoney;
	}
};

class sDBResponse_UpgradeItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	// from 
	SI16				siFromPos;
	cltItem				clFromItem;

	// to
	SI16				siToPos;
	cltItem				clToItem;

	SI32				siVillageUnique;
	cltMoney			clMoney;

	bool				bSuccess;

	sDBResponse_UpgradeItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_UpgradeItem ) );
	}
};


class sDBRequest_PerformanceUpgradeItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;
	
	SI16				siItemPos;				// 아이템 위치
	cltItem				clPreItem;				// 성능이 변경되기 전의 아이템 정보
	cltItem				clItem;					// 성능이 변경될 아이템 정보

	SI16				siScrollItemPos;		// 부적 아이템 위치
	cltItem				clScrollItem;			// 사용할 부적 아이템 정보

	sDBRequest_PerformanceUpgradeItem( SI32 CharID, SI32 PersonID,
		SI16 ItemPos, cltItem *pclPreItem, cltItem *pclItem,
		SI16 ScrollItemPos, cltItem *pclScrollItem
		)
	{
		packetHeader.usSize = sizeof( sDBRequest_PerformanceUpgradeItem );
		packetHeader.usCmd = DBMSG_REQUEST_PERFORMANCEUPGRADEITEM;

		siCharID = CharID;
		siPersonID = PersonID;

		siItemPos = ItemPos;
		clPreItem.Set( pclPreItem );
		clItem.Set( pclItem );

		siScrollItemPos = ScrollItemPos;
		clScrollItem.Set( pclScrollItem );
	}
};

class sDBResponse_PerformanceUpgradeItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;				// 1 : 성공, other : 실패

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siItemPos;				// 아이템 위치
	cltItem				clItem;					// 성능이 변경될 아이템 정보

	SI16				siLeftScrollItemPos;	// 사용된 부적 아이템 위치
	cltItem				clLeftScrollItem;		// 사용하고 남은 부적 아이템 정보

	sDBResponse_PerformanceUpgradeItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_PerformanceUpgradeItem ) );
	}
};

/*class sDBRequest_Delete_LimitDateItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siItemPos;				// 아이템 위치
	cltItem				clItem;					// 없어진 아이템

	sDBRequest_Delete_LimitDateItem( SI32 CharID, SI32 PersonID,
		SI16 ItemPos, cltItem *pclItem)
	{
		packetHeader.usSize = sizeof( sDBRequest_Delete_LimitDateItem );
		packetHeader.usCmd = DBMSG_REQUEST_DELETE_LIMITDATEITEM;

		siCharID = CharID;
		siPersonID = PersonID;

		siItemPos = ItemPos;
		clItem.Set( pclItem );
	}
};

class sDBResponse_Delete_LimitDateItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;				// 1 : 성공, other : 실패

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siItemUnique;			// 없어진 아이템의 유니크
	SI16				siItemPos;				// 아이템 위치
	cltItem				clItem;					// 아이템 정보

	sDBResponse_Delete_LimitDateItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_Delete_LimitDateItem ) );
	}
};

class sDBRequest_GetLimitUseItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siItemPos;				// 아이템 위치
	cltItem				clItem;					// 줄 아이템
	SI32				siUseDate;

	sDBRequest_GetLimitUseItem( SI32 CharID, SI32 PersonID,
		SI16 ItemPos, cltItem *pclItem,SI32 usedate)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetLimitUseItem );
		packetHeader.usCmd = DBMSG_REQUEST_GETLIMITUSEITEM;

		siCharID = CharID;
		siPersonID = PersonID;

		siItemPos = ItemPos;
		clItem.Set( pclItem );

		siUseDate = usedate;
	}
};

class sDBResponse_GetLimitUseItem
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;				// 1 : 성공, 0 : 이미 있음 -1 : 캐릭터 오류

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siItemPos;				// 아이템 위치
	cltItem				clItem;					// 아이템 정보
	SI32				siUseDate;

	sDBResponse_GetLimitUseItem()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetLimitUseItem ) );
	}
};*/

#define SETPREMIUM_EXP				1
#define SETPREMIUM_COMBAT			2
#define SETPREMIUM_EXPANDCOMBAT		3
#define SETPREMIUM_MAKEITEM			4
#define SETPREMIUM_BULLET			5
#define SETPREMIUM_SUMMONSTIMINA	6
#define SETPREMIUM_MAKETITEM		7

class sDBRequest_SetPremium
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	bool				bPremium;
	SI32				siPremiumUseDate;
	SI32				siCombatUseDate;
	SI32				siMakeItemServiceDate;
	SI32				siBulletServiceDate;
	SI32				siSummonStaminaServiceDate;

	cltItem				clItem;
	SI16				siItemPos;

	SI08				siType;

	sDBRequest_SetPremium( SI32 CharID, SI32 PersonID,bool premium,SI32 premiumdate,SI32 combatdate,SI32 MakeItemdate, 
		                   SI32 Bulletdate, SI32 SummonStaminadate, cltItem*pclitem,SI16 pos,SI08 type )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetPremium );
		packetHeader.usCmd = DBMSG_REQUEST_SETPREMIUM;

		siCharID = CharID;
		siPersonID = PersonID;

		bPremium = premium ;
		siPremiumUseDate = premiumdate ;
		siCombatUseDate = combatdate ;
		siMakeItemServiceDate = MakeItemdate;
		siBulletServiceDate = Bulletdate;
		siSummonStaminaServiceDate = SummonStaminadate;
		
		clItem.Set(pclitem);
		siItemPos = pos ;

		siType = type;
	}
};

class sDBResponse_SetPremium
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;				// 1 : 성공, 0 : 이미 있음 -1 : 캐릭터 오류

	SI32				siCharID;
	SI32				siPersonID;

	bool				bPremium;
	SI32				siPremiumUseDate;
	SI32				siCombatUseDate;
	SI32				siMakeItemDate;
	SI32				siBulletDate;
	SI32				siSummonStaminaDate;
	
	cltItem				clItem;
	SI16				siItemPos;

	SI08				siType;

	sDBResponse_SetPremium()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetPremium ) );
	}
};


//------------------------------------------
// 추천인 점수로 아이템을 구입할 것을 요청. 
//------------------------------------------
class sDBRequest_BuyRecommendProduct
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;		// 대상  Person
	cltItem				clItem;			// 대상 아이템 정보. 
	bool				bPileSwitch;	// 집적 가능 여부. 
	SI32				siNeedPoint;	// 아이템의 단가. 
	SI16				siItemPos;		// 아이템을 얻어올 인벤내 위치.

	sDBRequest_BuyRecommendProduct()
	{
		ZeroMemory(this, sizeof(sDBRequest_BuyRecommendProduct));
	}

	sDBRequest_BuyRecommendProduct(SI32 id, SI32 personid, cltItem* pclitem, bool bpilewitch, SI32 NeedPoint, SI32 itempos)
	{
		packetHeader.usSize = sizeof( sDBRequest_BuyRecommendProduct);
		packetHeader.usCmd = DBMSG_REQUEST_BUYRECOMMENDPRODUCT;
		usCharID		= id;

		siPersonID	= personid;
		clItem.Set(pclitem);
		bPileSwitch	= bpilewitch;
		siNeedPoint		= NeedPoint;
		siItemPos	= itempos;
	}
};


class sDBResonse_BuyRecommendProduct
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;		
	SI16				siItemPos;
	cltItem				clItem;				// 남은 아이템.
	SI32				siUsedPoint;
	SI32				siRecommendPoint;	// 보유포인트. 

	cltItem				clItem_Log;			// FOR LOG

	sDBResonse_BuyRecommendProduct()
	{
		ZeroMemory(this, sizeof(sDBResonse_BuyRecommendProduct));
	}
};


#define EXCHANGEITEM_REASON_NONE			0
#define EXCHANGEITEM_REASON_TINTINBOX		1	//이호식 상자;
#define EXCHANGEITEM_REASON_DORMANCY		2	//휴면 계정 아이템 업그래이드;
#define EXCHANGEITEM_REASON_GIVEPRESENT		3	//선물 주기

class sDBRequest_ExchangeItem
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;		// 대상  Person

	SI16				siItemPos;		// 아이템을 얻어올 인벤내 위치.
	cltItem				clItem;			// 바뀔 아이템 정보
	cltItem				clChangedItem;	// 바꾸어진 아이템 정보
	SI16				siReason;		// 바꾼 이유

	sDBRequest_ExchangeItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_ExchangeItem));
	}

	sDBRequest_ExchangeItem(SI32 personid, SI16 itempos, cltItem* pclitem, cltItem* pclchangeditem, SI16 reason )
	{
		packetHeader.usSize = sizeof( sDBRequest_ExchangeItem);
		packetHeader.usCmd = DBMSG_REQUEST_EXCHANGEITEM;

		siPersonID	= personid;
		siItemPos = itempos;
		clItem.Set(pclitem);
		clChangedItem.Set(pclchangeditem);
		siReason = reason;
	}
};

class sDBResponse_ExchangeItem
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.
	SI32				siPersonID;		

	SI16				siItemPos;
	cltItem				clItem;				
	cltItem				clChangedItem;
	SI16				siReason;

	sDBResponse_ExchangeItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_ExchangeItem));
	}
};

class sDBRequest_OpenTreasureBox
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;		// 대상  Person

	SI16				siItemPos;		// 아이템을 얻어올 인벤내 위치.
	cltItem				clItem;			// 바뀔 아이템 정보

	SI16				siKeyItemPos;	// 키 아이템 위치
	cltItem				clKeyItem;		// 키 아이템 정보

	// 생성될 첫번째 아이템 정보
	SI16				siToItemPos1;
	cltItem				clToItem1;		// 바꾸어진 아이템 정보
	bool				bPileSwitch1;
	bool				bCreateSID1;
	bool				bCanNotice1;	// 대박 알림 여부

	// 생성될 두번째 아이템 정보
	SI16				siToItemPos2;
	cltItem				clToItem2;		// 바꾸어진 아이템 정보
	bool				bPileSwitch2;
	bool				bCreateSID2;
	bool				bCanNotice2;	// 대박 알림 여부

	sDBRequest_OpenTreasureBox()
	{
		ZeroMemory(this, sizeof(sDBRequest_OpenTreasureBox));
	}

	sDBRequest_OpenTreasureBox(SI32 personid, SI16 itempos, cltItem* pclitem, SI16 keyitempos, cltItem* pclkeyitem, SI16 toitempos1, cltItem* pcltoitem1, bool pileswitch1, bool bcreatesid1, bool bcannotice1, SI16 toitempos2, cltItem* pcltoitem2, bool pileswitch2, bool bcreatesid2, bool bcannotice2 )
	{
		packetHeader.usSize = sizeof( sDBRequest_OpenTreasureBox);
		packetHeader.usCmd = DBMSG_REQUEST_OPENTREASUREBOX;

		siPersonID		= personid;

		siItemPos		= itempos;
		clItem.Set(pclitem);

		siKeyItemPos	= keyitempos;
		if(pclkeyitem)	clKeyItem.Set(pclkeyitem);

		siToItemPos1	= toitempos1;
		clToItem1.Set(pcltoitem1);
		bPileSwitch1	= pileswitch1;
		bCreateSID1		= bcreatesid1;
		bCanNotice1		= bcannotice1;

		siToItemPos2	= toitempos2;
		clToItem2.Set(pcltoitem2);
		bPileSwitch2	= pileswitch2;
		bCreateSID2		= bcreatesid2;
		bCanNotice2		= bcannotice2;
	}

	sDBRequest_OpenTreasureBox(SI32 personid, SI16 itempos, cltItem* pclitem, SI16 keyitempos, cltItem* pclkeyitem, SI16 toitempos1, cltItem* pcltoitem1, bool pileswitch1, bool bcreatesid1, bool bcannotice1)
	{
		packetHeader.usSize = sizeof( sDBRequest_OpenTreasureBox);
		packetHeader.usCmd = DBMSG_REQUEST_OPENTREASUREBOX;

		siPersonID		= personid;

		siItemPos		= itempos;
		clItem.Set(pclitem);
		
		siKeyItemPos	= keyitempos;
		if(pclkeyitem)	clKeyItem.Set(pclkeyitem);

		siToItemPos1	= toitempos1;
		clToItem1.Set(pcltoitem1);
		bPileSwitch1	= pileswitch1;
		bCreateSID1		= bcreatesid1;
		bCanNotice1		= bcannotice1;

		siToItemPos2	= 0;
		clToItem2.Init();
		bPileSwitch2	= false;
		bCreateSID2		= false;
		bCanNotice2		= false;
	}

	// 단순히 사용한 아이템의 갯수만 줄이기 위한 생성자
	sDBRequest_OpenTreasureBox(SI32 personid, SI16 itempos, cltItem* pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_OpenTreasureBox);
		packetHeader.usCmd = DBMSG_REQUEST_OPENTREASUREBOX;

		siPersonID		= personid;

		siItemPos		= itempos;
		clItem.Set(pclitem);

		siKeyItemPos	= 0;

		siToItemPos1	= 0;
		clToItem1.Init();
		bPileSwitch1	= false;
		bCreateSID1		= false;
		bCanNotice1		= false;

		siToItemPos2	= 0;
		clToItem2.Init();
		bPileSwitch2	= false;
		bCreateSID2		= false;
		bCanNotice2		= false;
	}
};

class sDBResponse_OpenTreasureBox
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.
	SI32				siPersonID;		

	SI16				siItemPos;
	cltItem				clItem;

	SI16				siKeyItemPos;
	cltItem				clKeyItem;

	SI16				siToItemPos1;
	cltItem				clToItem1;		// 바꾸어진 아이템 정보
	SI32				siGetItemNumber1;
	bool				bCanNotice1;	// 대박 알림 여부

	SI16				siToItemPos2;
	cltItem				clToItem2;		// 바꾸어진 아이템 정보
	SI32				siGetItemNumber2;
	bool				bCanNotice2;	// 대박 알림 여부

	// FOR LOG
	cltItem				clUse_Item;
	cltItem				clUse_KeyItem;
	cltItem				clAdd_ToItem1;	// 바꾸어진 아이템 정보
	cltItem				clAdd_ToItem2;	// 바꾸어진 아이템 정보

	sDBResponse_OpenTreasureBox()
	{
		ZeroMemory(this, sizeof(sDBResponse_OpenTreasureBox));
	}
};

class sDBRequest_GiveVillageItem
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;		// 자신의  Personid
	
	SI32				siVillageUnique;

	cltItem				clItem;			

	TCHAR				szMsg[256];

	// PCK : 아이템 사용기간 추가
	SI32				siUseDate;

	sDBRequest_GiveVillageItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_GiveVillageItem));
	}

	sDBRequest_GiveVillageItem(SI32 personid, SI32 villageUnique , cltItem * pclItem , TCHAR * pMsg, SI32 UseDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_GiveVillageItem);
		packetHeader.usCmd = DBMSG_REQUEST_GIVEVILLAGEITEM;

		siPersonID	= personid;		
		siVillageUnique = villageUnique;
		clItem.Set(pclItem);
		MStrCpy( szMsg , pMsg , 256 );
		siUseDate = UseDate;
	}
};

// PCK : 로그용 패킷 추가
class sDBResponse_GiveVillageItem
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;		// 자신의  Personid

	SI32				siVillageUnique;

	cltItem				clItem;			

	TCHAR				szMsg[256];

	SI32				siUseDate;

	sDBResponse_GiveVillageItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_GiveVillageItem));
	}
};

//********************************************************************************
//
// [추가 : 황진성 2008. 1. 30 => 공간 상자(보물) 저장]
//
//********************************************************************************

//[추가 : 황진성 2008. 1. 30 => 처음 캐릭터 생성때 공간 상자 아이템 리스트를 요청. S->DB]
class sDBRequest_Space_Box_List
{
public:
	sPacketHeader	packetHeader;
	UI16			usCharID;
	SI32			siPersonID;

	sDBRequest_Space_Box_List( UI16	CharID, SI32 personid )
	{
		ZeroMemory( this, sizeof( sDBRequest_Space_Box_List ) );

		packetHeader.usSize = sizeof(sDBRequest_Space_Box_List);
		packetHeader.usCmd  = DBMSG_REQUEST_SPACE_BOX_ITEM_LIST;
		usCharID			= CharID;	
		siPersonID			= personid;
	}
};

//[추가 : 황진성 2008. 1. 30 => 처음 캐릭터 생성때 공간 상자 아이템 리스트. DB->S]
class sDBResponse_Space_Box_List
{
public:
	sPacketHeader packetHeader;
	UI16				usCharID;

	SI32				siResult;
	SI32				siPersonID;

	cltSpaceBox			clSpaceBoxItem;		// 아이템 보유 등과 관련된 정보.
	cltTreasureBox		clTreasureBoxItem;	// 아이템 보유 등과 관련된 정보.


	sDBResponse_Space_Box_List()
	{
		ZeroMemory(this, sizeof(sDBResponse_Space_Box_List));
	}

	sDBResponse_Space_Box_List( SI32 personid )
	{
		ZeroMemory( this, sizeof( sDBResponse_Space_Box_List ) );

		packetHeader.usSize = sizeof(sDBResponse_Space_Box_List);
		packetHeader.usCmd  = DBMSG_RESPONSE_SPACE_BOX_ITEM_LIST;

		siPersonID			= personid;
	}
};




enum{ TYPE_SPACE_BOX, TYPE_TREASURE_BOX };

class sDBRequest_Move_Item
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 PersonID
	
	SI16				siInvenItemPos;		// 인벤토리 아이템의 위치. 
	cltItem				clInvenItem;		// 인벤토리 위치에 있는 아이템 

	SI16				siBoxItemPos;		// 박스 아이템의 위치. 
	cltItem				clBoxItem;			// 박스 위치에 있는 아이템 
	
	bool				type;				// 공간 상자인지 보물 상자인지.
	bool				bCanPile;			// 중첩이 가능한 아이템인가?

	sDBRequest_Move_Item()
	{
		ZeroMemory(this, sizeof(sDBRequest_Move_Item));
	}

	sDBRequest_Move_Item( UI16  _Cmd,		      SI32     _id,		      SI32 _personid,
						   SI32  _siInvenItemPos, cltItem* _clInvenItem, 
						   SI32  _siBoxItemPos,   cltItem* _clBoxItem,	  bool _type, bool _bCanPile )
	{
		ZeroMemory( this, sizeof( sDBRequest_Move_Item ) );

		packetHeader.usSize = sizeof( sDBRequest_Move_Item );
		packetHeader.usCmd  = _Cmd;	// DBMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM, DBMSG_REQUEST_RECOVERY_SPACE_BOX_MOVE_ITEM
		usCharID			= _id;

		siPersonID			= _personid;

		siInvenItemPos		= _siInvenItemPos;
		clInvenItem.Set(_clInvenItem);

		siBoxItemPos		= _siBoxItemPos;
		clBoxItem.Set(_clBoxItem);

		type				= _type;
		bCanPile			= _bCanPile;
	}
};

class sDBResponse_Move_Item
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;
	SI32				siPersonID;			// 대상이 되는 PersonID
	
	SI16				siInvenItemPos;		// 인벤토리 아이템의 위치. 
	cltItem				clInvenItem;		// 인벤토리 위치에 있는 아이템 

	SI16				siBoxItemPos;		// 박스 아이템의 위치. 
	cltItem				clBoxItem;			// 박스 위치에 있는 아이템 

	bool				type;				// 공간 상자인지 보물 상자인지.

	cltItem				clMoveItem;			// For Log - 옮겨지는 아이템

	sDBResponse_Move_Item()
	{
		ZeroMemory( this, sizeof( sDBResponse_Move_Item ) );
	}

	sDBResponse_Move_Item( UI16  _Cmd,		      SI32     _id,		      SI32 _personid,
						   SI32  _siInvenItemPos, cltItem* _clInvenItem, 
						   SI32  _siBoxItemPos,   cltItem* _clBoxItem,	  bool _type )
	{
		ZeroMemory( this, sizeof( sDBResponse_Move_Item ) );

		packetHeader.usSize = sizeof( sDBResponse_Move_Item );
		packetHeader.usCmd  = _Cmd;	// DBMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM, DBMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM
		usCharID			= _id;

		siPersonID			= _personid;
		
		siInvenItemPos		= _siInvenItemPos;
		clInvenItem.Set(_clInvenItem);

		siBoxItemPos		= _siBoxItemPos;
		clBoxItem.Set(_clBoxItem);

		type				= _type;
	}
};

// [영훈] 소환수 장비착용 : 사람에서 소환수로 아이템 이동 요청
class sDBRequest_SummonPushItem
{
public:
	sPacketHeader		m_packetHeader;

	UI16				m_usCharID;
	SI32				m_siPersonID;			// 대상이 되는 PersonID
	SI32				m_siSummonID;			// 소환수 번호

	SI16				m_siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				m_clFromItem;			// 출발위치에 있는 아이템 
	bool				m_bFromItemPileSwitch;	// 출발 위치에있는 아이템이 겹치기 가능한가

	SI16				m_siToItemPos;			// 이동시킬 목적지.
	cltItem				m_clToItem;				// 이동시킬 목적지에 있는 아이템 

	sDBRequest_SummonPushItem()
	{
		ZeroMemory( this, sizeof(sDBRequest_SummonPushItem) );
	}

	sDBRequest_SummonPushItem(
		UI16	usCharID,		SI32		siPersonID,		SI32	siSummonID,
		SI16	siFromItemPos,	cltItem*	pclFromItem,	bool	bFromItemPileSwitch,
		SI16	siToItemPos,	cltItem*	pclToItem
	)
	{
		m_packetHeader.usSize	= sizeof( sDBRequest_SummonPushItem );
		m_packetHeader.usCmd	= DBMSG_REQUEST_SUMMONPUSHITEM;

		m_usCharID				= usCharID;
		m_siPersonID			= siPersonID;			
		m_siSummonID			= siSummonID;		

		m_siFromItemPos			= siFromItemPos;
		m_clFromItem.Set( pclFromItem );
		m_bFromItemPileSwitch	= bFromItemPileSwitch;

		m_siToItemPos			= siToItemPos;
		m_clToItem.Set( pclToItem );
	}

};

class sDBResponse_SummonPushItem
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;				// 결과값

	UI16				m_usCharID;
	SI32				m_siPersonID;			// 대상이 되는 PersonID
	SI32				m_siSummonID;		// 소환수 번호

	SI16				m_siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				m_clFromItem;			// 출발위치에 있는 아이템 
	bool				m_bFromItemPileSwitch;	// 출발 위치에있는 아이템이 겹치기 가능한가

	SI16				m_siToItemPos;			// 이동시킬 목적지.
	cltItem				m_clToItem;				// 이동시킬 목적지에 있는 아이템 

	cltItem				m_clMoveItem;			// 로그용 아이템 정보

	sDBResponse_SummonPushItem()
	{
		ZeroMemory( this, sizeof(sDBResponse_SummonPushItem) );
	}

};	// DBMSG_RESPONSE_SUMMONPUSHITEM


// [영훈] 소환수 장비착용 : 소환수에서 사람으로 아이템 이동 요청
class sDBRequest_SummonPopItem
{
public:
	sPacketHeader		m_packetHeader;

	UI16				m_usCharID;
	SI32				m_siPersonID;			// 대상이 되는 PersonID
	SI32				m_siSummonID;			// 소환수 번호

	SI16				m_siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				m_clFromItem;			// 출발위치에 있는 아이템 
	bool				m_bFromItemPileSwitch;	// 출발 위치에있는 아이템이 겹치기 가능한가

	SI16				m_siToItemPos;			// 이동시킬 목적지.
	cltItem				m_clToItem;				// 이동시킬 목적지에 있는 아이템 

	sDBRequest_SummonPopItem()
	{
		ZeroMemory( this, sizeof(sDBRequest_SummonPopItem) );
	}

	sDBRequest_SummonPopItem(
		UI16	usCharID,		SI32		siPersonID,		SI32	siSummonID,
		SI16	siFromItemPos,	cltItem*	pclFromItem,	bool	bFromItemPileSwitch,
		SI16	siToItemPos,	cltItem*	pclToItem
		)
	{
		m_packetHeader.usSize	= sizeof( sDBRequest_SummonPopItem );
		m_packetHeader.usCmd	= DBMSG_REQUEST_SUMMONPOPITEM;

		m_usCharID				= usCharID;
		m_siPersonID			= siPersonID;			
		m_siSummonID			= siSummonID;		

		m_siFromItemPos			= siFromItemPos;
		m_clFromItem.Set( pclFromItem );
		m_bFromItemPileSwitch	= bFromItemPileSwitch;

		m_siToItemPos			= siToItemPos;
		m_clToItem.Set( pclToItem );
	}

};

class sDBResponse_SummonPopItem
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;				// 결과값

	UI16				m_usCharID;
	SI32				m_siPersonID;			// 대상이 되는 PersonID
	SI32				m_siSummonID;			// 소환수 번호

	SI16				m_siFromItemPos;		// 아이템의 출발 위치. 
	cltItem				m_clFromItem;			// 출발위치에 있는 아이템 
	bool				m_bFromItemPileSwitch;	// 출발 위치에있는 아이템이 겹치기 가능한가

	SI16				m_siToItemPos;			// 이동시킬 목적지.
	cltItem				m_clToItem;				// 이동시킬 목적지에 있는 아이템 

	cltItem				m_clMoveItem;			// 로그용 아이템 정보

	sDBResponse_SummonPopItem()
	{
		ZeroMemory( this, sizeof(sDBResponse_SummonPopItem) );
	}

};	// DBMSG_RESPONSE_SUMMONPOPITEM

enum SUMMONUSEITEM_REASON // 소환수 아이템 사용 이유
{
	SUMMONUSEITEM_REASON_NONE		= 0	// 이런 사용타입은 없다
	,SUMMONUSEITEM_REASON_GENERAL		// 일반적인 아이템 사용
};

class sDBRequest_SummonUseItem
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;		// 소환수 주인

	SI32				m_siSummonID;		// 소환수
	SI16				m_siItemPos;		// 아이템을 사용할 위치.
	cltItem				m_clItem;			// Person이 사용할 아이템 정보. 

	SI16				m_siUseReason;		// 아이템 사용 이유

	sDBRequest_SummonUseItem()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonUseItem));
	}

	sDBRequest_SummonUseItem( SI32 siCharID, SI32 siPersonID, SI32 siSummonID, SI32 siItemPos, cltItem* pclItem, SI16 siReason )
	{
		m_packetHeader.usSize	= sizeof( sDBRequest_SummonUseItem);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SUMMONUSEITEM;

		m_usCharID				= siCharID;
		m_siPersonID			= siPersonID;

		m_siSummonID			= siSummonID;
		m_siItemPos				= siItemPos;
		m_clItem.Set( pclItem );

		m_siUseReason			= siReason;
	}
};


class sDBResponse_SummonUseItem
{
public:
	sPacketHeader		m_packetHeader;
	
	SI32				m_siResult;

	UI16				m_usCharID;
	SI32				m_siPersonID;		// 소환수 주인

	SI32				m_siSummonID;		// 소환수
	SI16				m_siItemPos;		// 아이템 위치
	cltItem				m_clItem;			// 사용한 후의 아이템
	cltItem				m_clUsedItem;		// 사용되기전 아이템

	SI16				m_siUseReason;		// 아이템 사용 이유

	sDBResponse_SummonUseItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonUseItem));
	}
};	// DBMSG_RESPONSE_SUMMONUSEITEM

// 소환수 아이템 상태 변화
enum SUMMONITEM_STATUSCHANGE_REASON				// 소환수 아이템 상태 변화 이유
{
	SUMMONITEM_STATUSCHANGE_REASON_NONE	= 0		// 이런 사용타입은 없다
	,SUMMONITEM_STATUSCHANGE_REASON_DECREASE	// 결정체 내구도 감소
};


class sDBRequest_SummonItem_StatusChange
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;		// 소환수 주인

	SI32				m_siSummonID;		// 소환수
	SI16				m_siItemPos;		// 상태변화될 아이템 위치
	cltItem				m_clItem;			// 상태변화될 아이템

	SI16				m_siReason;			// 상태 변환 이유

	sDBRequest_SummonItem_StatusChange()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonItem_StatusChange));
	}

	sDBRequest_SummonItem_StatusChange( SI32 siCharID, SI32 siPersonID, SI32 siSummonID, SI32 siItemPos, cltItem* pclItem, SI16 siReason )
	{
		m_packetHeader.usSize	= sizeof( sDBRequest_SummonItem_StatusChange);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SUMMONITEM_STATUSCHANGE;

		m_usCharID				= siCharID;
		m_siPersonID			= siPersonID;

		m_siSummonID			= siSummonID;
		m_siItemPos				= siItemPos;
		m_clItem.Set( pclItem );

		m_siReason				= siReason;
	}
};


class sDBResponse_SummonItem_StatusChange
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siResult;

	UI16				m_usCharID;
	SI32				m_siPersonID;		// 소환수 주인

	SI32				m_siSummonID;		// 소환수
	SI16				m_siItemPos;		// 아이템 위치
	cltItem				m_clItem;			// 상태가 변화된 아이템
	cltItem				m_clOldItem;		// 상태가 변화되기전 아이템(로그용)

	SI16				m_siReason;			// 아이템 사용 이유

	sDBResponse_SummonItem_StatusChange()
	{
		ZeroMemory(this, sizeof(sDBResponse_SummonItem_StatusChange));
	}
};	// DBMSG_RESPONSE_SUMMONITEM_STATUSCHANGE

class sDBRequest_Amber_Box_Select_Item_List
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siPersonID;		
	SI32				m_siCharID;
	
	SI32				m_siUseItemUnique;		// 어떤 아이템을 사용하여 아이템이 선택 되었는지 알기위해.

	sDBRequest_Amber_Box_Select_Item_List(SI32 _siCharID, SI32 _siPersonID, SI32 _siUseItemUnique)
	{
		ZeroMemory(this, sizeof(sDBRequest_Amber_Box_Select_Item_List));

		m_packetHeader.usSize	= sizeof( sDBRequest_Amber_Box_Select_Item_List);
		m_packetHeader.usCmd	= DBMSG_REQUEST_AMBER_BOX_SELECT_ITEM_LIST;

		m_siCharID				= _siCharID;
		m_siPersonID			= _siPersonID;
		m_siUseItemUnique		= _siUseItemUnique;

	}
};


class sDBResponse_Amber_Box_Select_Item_List
{
public:
	sPacketHeader			m_packetHeader;
	SI32					m_siPersonID;				// 소환수 주인
	SI32					m_siCharID;

	AmberboxSelectItemInfo	m_clAmberboxItemList[3];	// 렌덤하게 선택된 아이템 정보.	

	sDBResponse_Amber_Box_Select_Item_List()
	{
		ZeroMemory(this, sizeof(sDBResponse_Amber_Box_Select_Item_List));
	}
};


class sDBRequest_Amber_Box_Select_Item_Add
{
public:
	sPacketHeader			m_packetHeader;

	SI32					m_siResult;

	SI32					m_siPersonID;		
	SI32					m_siCharID;

	AmberboxSelectItemInfo	m_clAmberboxItemList;	// 렌덤하게 선택된 아이템 정보.	

	SI32					m_siUseItemUnique;		// 어떤 아이템을 사용하여 아이템이 선택 되었는지 알기위해.

	sDBRequest_Amber_Box_Select_Item_Add(SI32 _siCharID, SI32 _siPersonID, AmberboxSelectItemInfo* _pclAmberboxItemList, SI32 _siUseItemUnique)
	{
		ZeroMemory(this, sizeof(sDBRequest_Amber_Box_Select_Item_Add));

		m_packetHeader.usSize	= sizeof( sDBRequest_Amber_Box_Select_Item_Add );
		m_packetHeader.usCmd	= DBMSG_REQUEST_AMBER_BOX_SELECT_ITEM_ADD;

		m_siCharID				= _siCharID;
		m_siPersonID			= _siPersonID;

		m_clAmberboxItemList.m_siItemNum	= _pclAmberboxItemList->m_siItemNum;	
		m_clAmberboxItemList.m_siItemUnique	= _pclAmberboxItemList->m_siItemUnique;
		m_clAmberboxItemList.m_siSlot		= _pclAmberboxItemList->m_siSlot;		

		m_siUseItemUnique		= _siUseItemUnique;
	}
};


class sDBRequest_Amber_Box_Select_Item_Get
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siPersonID;		
	SI32				m_siCharID;

	UI08				m_SlotIndex;
	
	SI16				m_siUseItemPos;		// 사용하는 아이템의 인벤 위치. 
	cltItem				m_clUseItem;		// 사용하는 아이템 정보.
	
	SI16				m_siGetItemPos;		// 얻은 아이템의 인벤 위치. 
	cltItem				m_clGetItem;		// 얻은 아이템 정보.

	bool				m_bCanPile;			// 중복가능 여부.

	sDBRequest_Amber_Box_Select_Item_Get(   SI32 _siCharID, SI32 _siPersonID, UI08 _SlotIndex, 
											SI16 _siUseItemPos,		cltItem* _pclUseItem,
											SI16 _siGetItemPos,		cltItem* _pclGetItem, 
											bool _bCanPile)
	{
		ZeroMemory(this, sizeof(sDBRequest_Amber_Box_Select_Item_Get));

		m_packetHeader.usSize	= sizeof( sDBRequest_Amber_Box_Select_Item_Get );
		m_packetHeader.usCmd	= DBMSG_REQUEST_AMBER_BOX_SELECT_ITEM_GET;

		m_siCharID				= _siCharID;
		m_siPersonID			= _siPersonID;

		m_SlotIndex				= _SlotIndex;	
		
		m_siUseItemPos			= _siUseItemPos;
		m_clUseItem.Set(_pclUseItem);

		m_siGetItemPos			= _siGetItemPos;
		m_clGetItem.Set(_pclGetItem);

		m_bCanPile			    = _bCanPile;
	}
};

class sDBResponse_Amber_Box_Select_Item_Get
{
public:
	sPacketHeader			m_packetHeader;
	
	SI32					m_siResult;

	SI32					m_siPersonID;		
	SI32					m_siCharID;

	SI16					m_siRemainItemPos;		// 사용하고 남은 아이템의 인벤 위치. 
	cltItem					m_clRemainItem;			// 사용하고 남은 아이템 정보.

	SI16					m_siToItemPos;			// 얻은 아이템의 인벤 위치. 
	cltItem					m_clToItem;				// 얻은 아이템 정보(기존 아이템이 있으면 누적된 정보).

	// 명품창 공지에 사용.
	cltItem					m_clUseItem;			// 사용한 아이템 정보..
	cltItem					m_clGetItem;			// 얻은   아이템 정보..

	bool					m_bCanPile;				// 중복가능 여부.
	
	sDBResponse_Amber_Box_Select_Item_Get()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

class sDBRequest_NewSelectBox_Item_Get
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siPersonID;
	UI16				m_uiCharID;

	SI16				m_siSelectIndex;	// 로그용

	cltItem				m_clBoxItem;
	SI16				m_siBoxItemPos;

	cltItem				m_clGetItem;
	SI16				m_siGetItemPos;
	bool				m_bGetItemCanPile;
	SI16				m_siCanNotice;		// 대박알림창 여부
	

	sDBRequest_NewSelectBox_Item_Get()
	{
		ZeroMemory( this, sizeof(sDBRequest_NewSelectBox_Item_Get) );
	}

	sDBRequest_NewSelectBox_Item_Get( SI32 siPersonID, UI16 uiCharID, SI16 siSelectIndex,
									cltItem* pclBoxItem, SI16 siBoxItemPos,
									cltItem* pclGetItem, SI16 siGetITemPos, bool bGetItemCanPile, SI16 siCanNotice)
	{
		ZeroMemory( this, sizeof(sDBRequest_NewSelectBox_Item_Get) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_NEWSELECTBOX_ITEM_GET;
		m_packetHeader.usSize	= sizeof(sDBRequest_NewSelectBox_Item_Get);

		m_siPersonID			= siPersonID;
		m_uiCharID				= uiCharID;

		m_siSelectIndex			= siSelectIndex;

		if ( pclBoxItem )
		{
			m_clBoxItem.Set( pclBoxItem );
			m_siBoxItemPos		= siBoxItemPos;
		}

		if ( pclGetItem )
		{
			m_clGetItem.Set( pclGetItem );
			m_siGetItemPos		= siGetITemPos;
			m_bGetItemCanPile	= bGetItemCanPile;
			m_siCanNotice		= siCanNotice;
		}

	}

};

class sDBResponse_NewSelectBox_Item_Get
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siPersonID;
	UI16				m_uiCharID;

	SI32				m_siResult;
	
	// 메시지 및 로그용
	cltItem				m_clUseItem;	// 사용한 아이템(박스아이템)
	cltItem				m_clToItem;		// 받은 아이템
	SI16				m_siSelectIndex;// 로그용 

	cltItem				m_clBoxItem;	// 사용되고 남은 박스 아이템
	SI16				m_siBoxItemPos;	

	cltItem				m_clGetItem;	// 현재 내가 받은 아이템
	SI16				m_siGetItemPos;
	SI16				m_siCanNotice;
	


	sDBResponse_NewSelectBox_Item_Get()
	{
		ZeroMemory( this, sizeof(sDBResponse_NewSelectBox_Item_Get) );
	}

};	// DBMSG_RESPONSE_NEWSELECTBOX_ITEM_GET


//------------------------------------------
// 아이템을 수리하는 기능
// 1. NPC를 통한 수리
// 2. 스스로 수리
// 3. 위탁 수리
//------------------------------------------

class sDBRequest_RepairItem
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;

	// 수리하는 캐릭터
	SI32				m_siPersonID;

	// 수리하는 아이템 (수리된 정보를 DB로 보낸다)
	SI16				m_siPos;
	cltItem				m_clItem;

	// 수리 재료 아이템 : 수리석 + 기타재료
	RepairItemExtraMaterial	m_Material;

	// 수리비.
	GMONEY				m_siRepairMoney;
	
	// 위탁수리인 경우 수리자 캐릭터
	SI32				m_siRepairPersonID;

	// 수리되기전의 아이템 상태	- for Log
	cltItem				m_clBeforeItem;

	SI16				m_siType;	// 1:직접수리, 2:수리요청, 3:의뢰수리, 4:수리요청 취소, 5:NPC수리

	sDBRequest_RepairItem( SI32 id, SI32 personid, SI32 pos, cltItem* pclbeforeitem, cltItem* pclitem, RepairItemExtraMaterial& Materials, GMONEY repairmoney, SI32 siRepairPersonID, SI16 siType )
	{
		m_packetHeader.usSize = sizeof( sDBRequest_RepairItem );
		m_packetHeader.usCmd = DBMSG_REQUEST_REPAIRITEM;
		m_usCharID			= id;

		m_siPersonID		= personid;

		m_siPos				= pos;
		m_clBeforeItem.Set(pclbeforeitem);
		m_clItem.Set(pclitem);

		for( SI32 i=0; i<m_Material.GetSize(); i++ )
		{
			m_Material[i].Set( &Materials[i] );
		}
		m_siRepairMoney		= repairmoney;

		m_siRepairPersonID	= siRepairPersonID;

		m_siType			= siType;
	}
};

class sDBResponse_RepairItem
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;

	SI32				m_siResult;
	// 수리하는 캐릭터
	SI32				m_siPersonID;

	// 수리된 아이템
	SI16				m_siPos;
	cltItem				m_clItem;

	// 수리하는 캐릭터 자금
	GMONEY				m_siRepairMoney;		// 수리비. 
	GMONEY				m_siRepairerMoney;	// 수리자의 현재 보유 자금 .

	// 수리자
	SI32				m_siRepairPersonID;		// 위탁수리인 경우 수리자 캐릭터
	GMONEY				m_siRepairPersonMoney;	// 수리자의 자금

	// 수리되기전의 아이템 상태	- for Log
	cltItem				m_clBeforeItem;

	SI16				m_siType;	// 1:직접수리, 2:수리요청, 3:의뢰수리, 4:수리요청 취소, 5:NPC수리

	sDBResponse_RepairItem()
	{
		ZeroMemory(this, sizeof(sDBResponse_RepairItem));
	}
};



#endif
