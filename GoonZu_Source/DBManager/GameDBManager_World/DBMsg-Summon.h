#ifndef _DBMSGSUMMON_H
#define _DBMSGSUMMON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//-----------------------------------------
// 소환수 한마리의 정보를 저장한다. 
//-----------------------------------------
#define SETSUMMON_REASON_MAKE		1	// 소환수를 신규로 생성. 
#define SETSUMMON_REASON_UPDATE		2	// 소환수의 정보 업데이트. 
#define SETSUMMON_REASON_FREE		3	// 소환수 풀어주기.
#define SETSUMMON_REASON_DIE		4	// 소환수 죽이기 
//#define SETSUMMON_REASON_REVIVE	5	// 소환수를 부활시킨다. 
#define SETSUMMON_REASON_TAKEIN		6	// 소환수를 집어넣는다.
#define SETSUMMON_REASON_TAKEOUT	7	// 소환수를 꺼낸다.
#define SETSUMMON_REASON_GMEVOLVE	8	// 운영자가 소환수를 진화시킨다.
#define SETSUMMON_REASON_PARTINIT	9	// [부분]상태초기화를 이용한 정보 업데이트
#define	SETSUMMON_REASON_CHARGESTAMINA	10	// 스태미너 충전 사용
#define	SETSUMMON_REASON_SUMMONSTON	11	// 아이템장착석 사용

class sDBRequest_SetSummon
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI16				siReason;		// 소환수를 설정하는 사유. 

	UI08				siSummonIndex;	// 소환수의 배열에서의 위치
	cltSummon			clSummon;		// 소환수의 정보. 	

	sDBRequest_SetSummon()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetSummon));
	}

	sDBRequest_SetSummon(UI16 charid, SI32 personid, SI16 reason, UI08 summonindex, cltSummon* pclSummon)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetSummon );
		packetHeader.usCmd	= DBMSG_REQUEST_SETSUMMON;

		usCharID		= charid;
		siPersonID		= personid;
		siReason		= reason;

		siSummonIndex	= summonindex;

		clSummon.Set(pclSummon);
	}
};

class sDBResponse_SetSummon
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;		// 결과값.

	UI16				usCharID;
	SI32				siPersonID;
	SI16				siReason;		// 소환수를 설정하는 사유. 
	
	UI08				siSummonIndex;	// 소환수의 서버내 위치 정보
	cltSummon			clSummon;		// 소환수의 정보. 	

	//[추가 : 황진성 2007. 10. 26 로그에 이용. 소환수 풀어줄때 사용할 정보.]
	cltSummon			clFreeSummon;
	cltSimpleSummon		clSimpleSummon;

	sDBResponse_SetSummon()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetSummon));
	}
};


//------------------------------------------
// 소환수를 부활시키기위해서 소환인형을 소비시킨다.
//------------------------------------------
class sDBRequest_SummonRevive
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI16				siItemPos;
	SI16				siItemUnique;
	SI16				siItemNum;
	
	cltSummon			clSummon;	

	cltItem				clItem;

	sDBRequest_SummonRevive()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonRevive));
	}

	sDBRequest_SummonRevive(SI32 id, SI32 personid, SI32 villageunique, SI16 itempos, SI16 itemunique, SI16 itemnum, 
							cltSummon *pclSummon, cltItem *pclItem)
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonRevive );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONREVIVE;
		
		siCharID	= id;
		siPersonID	= personid;

		siVillageUnique = villageunique;
		siItemPos		= itempos;
		siItemUnique	= itemunique;
		siItemNum		= itemnum;

		clSummon.Set(pclSummon);
		clItem.Set(pclItem);
	}

};

class sDBResponse_SummonRevive
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
	
	SI32				siVillageUnique;
	SI16				siItemNum;			

	SI16				siItemPos;
	cltItem				clItem;
	
	cltSummon			clSummon;

	// PCK : 로그용 패킷 추가
	cltItem				clItem_Log;

	sDBResponse_SummonRevive()
	{
		ZeroMemory( this, sizeof( sDBResponse_SummonRevive ) );
	}
};

//------------------------------------------
// 소환수를 진화시키기위해서 소환수진화 이용권을 소비시킨다.
//------------------------------------------
#define SUMMONEVOLVE_EVOLVE			0 // 진화
#define SUMMONEVOLVE_DEVOLUTION		1 // 퇴화
#define SUMMONEVOLVE_TRANSFORM		2 // 변환

class sDBRequest_SummonEvolve
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI16				siItemPos;
	SI16				siItemUnique;
	SI16				siItemNum;
	
	cltSummon			clSummon;

	bool				bQuestEvolve;
	bool				bItemUse;

	SI16				siMode;

	cltItem				clItem;
	SI16				siSummonKind;	// 진화되기 이전의 소환수 카인드

	sDBRequest_SummonEvolve()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonEvolve));
	}

	sDBRequest_SummonEvolve(SI32 id, SI32 personid, SI32 villageunique, SI16 itempos, SI16 itemunique, SI16 itemnum,
							cltSummon	*pclSummon,bool questevolve,bool itemuse, SI16 mode, cltItem *pclItem, SI16 SummonKind)
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonEvolve );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONEVOLVE;
		
		siCharID	= id;
		siPersonID	= personid;

		siVillageUnique = villageunique;
		siItemPos		= itempos;
		siItemUnique	= itemunique;
		siItemNum		= itemnum;

		clSummon.Set(pclSummon);

		bQuestEvolve	= questevolve ;
		bItemUse		= itemuse ;

		siMode			= mode ;
		clItem.Set(pclItem);
		siSummonKind = siSummonKind;
	}

};

class sDBResponse_SummonEvolve
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
	
	SI32				siVillageUnique;
	SI16				siItemNum;			

	SI16				siItemPos;
	cltItem				clItem;
	
	cltSummon			clSummon;

	bool				bQuestEvolve;
	bool				bItemUse;

	SI16				siMode ;

	SI16				siUsedItemUnique;
	
	// PCK : 로그용 패킷
	cltItem				clItem_Log;
	SI16				siSummonKind;

	sDBResponse_SummonEvolve()
	{
		ZeroMemory( this, sizeof( sDBResponse_SummonEvolve ) );
	}
};

//------------------------------------------
// 소환수를 강화시키기위해서 부적을 소모시킨다.
//------------------------------------------
class sDBRequest_SummonScroll
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siVillageUnique;
	SI16				siItemPos;
	SI16				siItemUnique;
	SI16				siItemNum;
	
	cltSummon			clSummon;

	sDBRequest_SummonScroll()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonScroll));
	}

	sDBRequest_SummonScroll(SI32 id, SI32 personid, SI32 villageunique, SI16 itempos, SI16 itemunique, SI16 itemnum, cltSummon	*pclSummon)
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonScroll );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONSCROLL;
		
		siCharID	= id;
		siPersonID	= personid;

		siVillageUnique = villageunique;
		siItemPos		= itempos;
		siItemUnique	= itemunique;
		siItemNum		= itemnum;

		clSummon.Set(pclSummon);
	}

};

class sDBResponse_SummonScroll
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
	
	SI32				siVillageUnique;
	SI16				siItemNum;			

	SI16				siItemPos;
	cltItem				clItem;
	
	cltSummon			clSummon;
	
	sDBResponse_SummonScroll()
	{
		ZeroMemory( this, sizeof( sDBResponse_SummonScroll ) );
	}
};

//------------------------------------------
// 소환수의 상태를 초기화하고 상태초기화이용권을 소비한다.
//------------------------------------------
class sDBRequest_Summon_StatusInit
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siItemPos;
	cltItem				clItem;

	cltSummon			clSummon;

	sDBRequest_Summon_StatusInit()
	{
		ZeroMemory(this, sizeof(sDBRequest_Summon_StatusInit));
	}

	sDBRequest_Summon_StatusInit(SI32 id, SI32 personid,SI16 itempos,cltItem * pclItem, cltSummon	*pclSummon)
	{
		packetHeader.usSize = sizeof( sDBRequest_Summon_StatusInit );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMON_STATUSINIT;

		siCharID	= id;
		siPersonID	= personid;

		siItemPos		= itempos;
		clItem.Set(pclItem);

		clSummon.Set(pclSummon);
	}

};

class sDBResponse_Summon_StatusInit
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siItemPos;
	cltItem				clItem;

	cltSummon			clSummon;

	// PCK - 로그용 패킷 추가
	cltItem				clItem_Log;

	sDBResponse_Summon_StatusInit()
	{
		ZeroMemory( this, sizeof( sDBResponse_Summon_StatusInit ) );
	}
};

//------------------------------------------
// 소환수 자동회복의 상태를 고친다.
//------------------------------------------
class sDBRequest_Summon_SetAutoHeal
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siSelectAutoHeal;
	bool				bAutoHeal;

	sDBRequest_Summon_SetAutoHeal()
	{
		ZeroMemory(this, sizeof(sDBRequest_Summon_SetAutoHeal));
	}

	sDBRequest_Summon_SetAutoHeal(SI32 id, SI32 personid,SI16 status,bool autoheal)
	{
		packetHeader.usSize = sizeof( sDBRequest_Summon_SetAutoHeal );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMON_SETAUTOHEAL;

		siCharID	= id;
		siPersonID	= personid;

		siSelectAutoHeal	= status ;
		bAutoHeal			= autoheal ;
	}

};

class sDBResponse_Summon_SetAutoHeal
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siSelectAutoHeal;
	bool				bAutoHeal;

	sDBResponse_Summon_SetAutoHeal()
	{
		ZeroMemory( this, sizeof( sDBResponse_Summon_SetAutoHeal ) );
	}
};

class sDBRequest_SummonTrade
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	cltSummon			clSummon;		// 소환수의 정보.

	GMONEY				siMoney;

	sDBRequest_SummonTrade()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonTrade));
	}

	sDBRequest_SummonTrade(UI16 charid, SI32 personid, cltSummon* pclSummon,GMONEY money)
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonTrade );
		packetHeader.usCmd	= DBMSG_REQUEST_SUMMONTRADE;

		usCharID	= charid;
		siPersonID	= personid;

		clSummon.Set(pclSummon);

		siMoney = money ;
	}
};

class sDBResponse_SummonTrade
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	UI16				usCharID;
	SI32				siPersonID;

	cltSummon			clSummon;		// 소환수의 정보.

	GMONEY				siMoney;		// 소환수의 가격
	GMONEY				siLeftMoney;	// NPC에게 판 뒤의 가진 돈

	sDBResponse_SummonTrade()
	{
		ZeroMemory( this, sizeof( sDBResponse_SummonTrade ) );
	}
};
// 무기 장착석 사용 
class sDBRequest_SummonUseSton
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI32				siSummonIndex;
	SI32				siStonType;
	SI32				siStonNum;
	sDBRequest_SummonUseSton()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonUseSton));
	}

	sDBRequest_SummonUseSton(SI32 id, SI32 personid, SI32 summonindex, SI32 siStonType, SI32 siStonNum )
	{
		packetHeader.usSize = sizeof( sDBRequest_SummonUseSton );
		packetHeader.usCmd = DBMSG_REQUEST_SUMMONUSESTON;

		siCharID	= id;
		siPersonID	= personid;

		siSummonIndex	= summonindex;
		siStonType	= siStonType;
		siStonNum	= siStonNum;
	}

};

class sDBResponse_SummonUseSton
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	cltSummon			clSummon;

	sDBResponse_SummonUseSton()
	{
		ZeroMemory( this, sizeof( sDBResponse_SummonUseSton ) );
	}
};

//  Base에 있는 해당 소환수를 Slot으로 넣는다.
class sDBRequest_SummonSlotIn
{
public:
	sPacketHeader		packetHeader	;
	SI08				siSummonIndex	;		// 빼낸 3개 인덱스
	
	SI32				siCharID		;
	SI32				siPersonID		;
	SI32				siSummonID	;
	sDBRequest_SummonSlotIn()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonSlotIn))	;
	}

	sDBRequest_SummonSlotIn(SI32 id, SI32 personid, SI32 summonindex, SI32 SummonID )
	{
		packetHeader.usSize	=	sizeof( sDBRequest_SummonSlotIn )	;
		packetHeader.usCmd	=	DBMSG_REQUEST_SUMMONSLOTIN		;
		siSummonIndex		=	summonindex	;

		siCharID			=	id			;
		siPersonID			=	personid	;
		siSummonID			=	SummonID	;

	}

};

class sDBResponse_SummonSlotIn
{
public:
	sPacketHeader		packetHeader		;
	SI08				siSummonIndex		;	// 해당 소환수가 Base로부터 들어가야할 인덱스
	SI32				siResult			;
	SI32				siCharID			;
	SI32				siPersonID			;

	cltSummon			clSummon			;
	cltSummonItemInfo	clSummonItemInfo	;

	sDBResponse_SummonSlotIn()
	{
		ZeroMemory( this, sizeof( sDBResponse_SummonSlotIn ) );
	}
};

//해당 소환수를 Slot에서 빼낸다.
class sDBRequest_SummonSlotOut
{
public:
	sPacketHeader		packetHeader	;

	SI32				siCharID		;
	SI32				siPersonID		;
	SI32				siSummonID	;
	SI32 			siSlotIndex;

	sDBRequest_SummonSlotOut()
	{
		ZeroMemory(this, sizeof(sDBRequest_SummonSlotOut))	;
	}

	sDBRequest_SummonSlotOut(SI32 id, SI32 personid,SI32 SummonID ,SI32 SlotIndex)
	{
		packetHeader.usSize	=	sizeof( sDBRequest_SummonSlotOut )	;
		packetHeader.usCmd	=	DBMSG_REQUEST_SUMMONSLOTOUT		;
		siCharID			=	id			;
		siPersonID			=	personid	;
		siSummonID			=	SummonID	;
		siSlotIndex			=	SlotIndex;
	}

};

class sDBResponse_SummonSlotOut
{
public:
	sPacketHeader		packetHeader		;
	SI32				siSummonIndex		;
	SI32				siResult			;
	SI32				siCharID			;
	SI32				siPersonID			;
	sDBResponse_SummonSlotOut()
	{
		ZeroMemory( this, sizeof( sDBResponse_SummonSlotOut ) );
	}
};

#endif
