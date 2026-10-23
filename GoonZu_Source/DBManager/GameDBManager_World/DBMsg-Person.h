#ifndef _DBMSGPERSON_H
#define _DBMSGPERSON_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"

//----------------------------------------------
// Common
//----------------------------------------------
#include "../../GoonZuWorld/Common/CommonHeader.h"
#include "../../GoonZuWorld/Common/PersonInfo.h"

#include "../../GoonZuWorld/Common/Char/CharCommon/Char-Common.h"
#include "../../GoonZuWorld/Common/Char/ParametaBox/ParametaBox.h"
#include "../../GoonZuWorld/Common/Item/ItemCommon/ItemUnit.h"
#include "../../GoonZuWorld/Server/Rank/Rank.h"
#include "../../GoonZuWorld/Common/Skill/Skill.h"
#include "../../GoonZuWorld/Common/Quest/Quest.h"
#include "../../GoonZuWorld/Common/Horse/Horse.h"
#include "../../GoonZuWorld/Common/Char/CharCommon/Char-EconomyInfo.h"
#include "../../GoonZuWorld/Common/SystemReward/SystemReward.h"


#include "../../GoonZuWorld/CommonLogic/Bank/Bank.h"
#include "../../GoonZuWorld/CommonLogic/Guild/Guild.h"
#include "../../GoonZuWorld/common/Family/Family.h"
#include "../../GoonZuWorld/CommonLogic/DormancyAccountSystem/DormancyInfo.h"

#include "DBMsg-System.h"

//------------------------
// Person의 전체 정보 
//------------------------
// Person의 전체 정보. 
class sDBPersonTotalInfo
{
public:

	cltPBasicInfo			BasicInfo;			// Person의 기본 정보. 
	cltPIPInfo				IPInfo;				// Person의 ImportantParameta 정보. 
	cltPSkillInfo			SkillInfo;			// Person의 기술 정보. 
	cltPItemInfo			ItemInfo;			// Person의 아이템 정보. 

	SI32					siGameMasterLevel;	// 운영자 권한 등급. (0 : 일반 유저, ) 
	cltSimpleRank			RankInfo;			// Person의 신분 정보. 
	cltBankAccount			clBankInfo;			// Person의 은행정보. 
	cltPStockInfo			clStockInfo;
	//_LEON_GUILD_STOCK
//	cltPGuildStockInfo		clGuildStockInfo;	// Persond의 길드 주식 정보
	cltWorldMoneyAccount	clWorldMoney;		// Person의 세계통화(아큐?) 정보

	cltPMarketInfo			clMarketInfo;
	cltCharRealEstate		clRealEstateInfo;	// 부동산 정보(주택...) 
	cltHealthInfo			clHealthInfo;		// 건강 정보. 
	cltPQuestInfo			clQuestInfo;		// 퀘스트 정보.
	cltPersonHorseInfo		clHorseInfo;		// 말 정보. 
	cltGuiltyInfo			clGuiltyInfo;		// 전과 기록. 
	cltPDailyQuestInfo		clDailyQuestInfo;	// Daily 퀘스트 정보
	cltPDailyQuest2Info		clDailyQuest2Info;	// Daily 퀘스트 정보
	cltPDailyQuest3Info		clDailyQuest3Info;	// Daily 퀘스트 정보
	cltSummonInfo			clSummonInfo;		// 소환수 정보 
	//cltPLimitDateItemInfo	clLimitDateItemInfo;// 기간제 아이템 정보
	UI16					uiGachaCnt;			// 가차 누적 횟수
	cltPersonNaming			clPersonNaming;	
	
	//KHY - 1220 - 변신시스템.
	cltTransFormInfo		clTransFormInfo;

	// [영훈] 여권관련 정보
	cltWorldPassportForDB	clWorldPassportForDB;

	cltElixir_Info			 m_clElixirInfo;			// 엘릭서 정보.

	// [기형] 휴면 계정 정보
	CDormancyInfo			m_clDormancyInfo;

	sDBPersonTotalInfo()
	{
		ZeroMemory(this, sizeof(sDBPersonTotalInfo));
	}
};


//----------------------------------------------------
// Person의 전체 정보를 얻어올 것을 요청하는 클래스. (IDNUm을 근거로. ) 
//----------------------------------------------------
class sDBRequest_PersonTotalInfoGetByIDNum
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siIDNum;	
	SI32				siIndex;

	TCHAR				szIPAddress[20];		// 접속자의 ip주소. 

	bool				bTiredSystem;
	DWORD				dwPlayingTime;

	bool				bDormancyState;			// 휴면 계정 여부.

	sDBRequest_PersonTotalInfoGetByIDNum()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonTotalInfoGetByIDNum));
	}

	sDBRequest_PersonTotalInfoGetByIDNum(SI32 id, SI32 idnum, SI32 index, TCHAR* pip, bool DormancyState, bool TiredSystem = false, DWORD PlayingTime = 0 )
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonTotalInfoGetByIDNum );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONTOTALINFOGETBYIDNUM;
		usCharID			= id;

		siIDNum	= idnum;
		siIndex	= index;

		StringCchCopy(szIPAddress, 20, pip);

		bDormancyState = DormancyState;

		bTiredSystem = TiredSystem;
		dwPlayingTime = PlayingTime;
	}
};

//----------------------------------------------------
// Person의 아이템 정보를 얻어올 것을 요청하는 클래스. 
//----------------------------------------------------
class sDBRequest_PersonItemInfoGet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;	

	sDBRequest_PersonItemInfoGet()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonItemInfoGet));
	}

	sDBRequest_PersonItemInfoGet(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonItemInfoGet );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONITEMINFOGET;
		usCharID			= id;

		siPersonID	= personid;
	}
};

class sDBResponse_PersonItemInfoGet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;	

	cltPItemInfo		ItemInfo;			// Person의 아이템 정보. 

	
	sDBResponse_PersonItemInfoGet()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonItemInfoGet));
	}
};


//----------------------------------------------------
// Person의 전체 정보를 얻어올 것을 요청하는 클래스. 
//----------------------------------------------------
class sDBRequest_PersonTotalInfoGet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;

	sDBRequest_PersonTotalInfoGet()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonTotalInfoGet));
	}

	sDBRequest_PersonTotalInfoGet(SI32 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonTotalInfoGet );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONTOTALINFOGET;
		usCharID			= id;

		siPersonID	= personid;
	}
};

class sDBResponse_PersonTotalInfoGet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값. (1: 성공. 0:실패) 
	sDBPersonTotalInfo	clInfo;
	SI32				siHomeVillageUnique;
	bool				bExistentItem;		// 시전 연장 못해서 시스템에서 회수한 아이템이 있는가?
	bool				bTiredSystem;
	DWORD				dwPlayingTime;

	sDBResponse_PersonTotalInfoGet()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonTotalInfoGet));
		
		clInfo.clSummonInfo.Init();
	}
};


//----------------------------------------------------
// Person의 전체 정보를 저장할  것을 요청하는 클래스. 
//----------------------------------------------------
class sDBRequest_PersonTotalInfoSet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	sDBPersonTotalInfo	clInfo;

	sDBRequest_PersonTotalInfoSet()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonTotalInfoSet));
	}

	sDBRequest_PersonTotalInfoSet(SI32 id, sDBPersonTotalInfo* pclinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonTotalInfoSet );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONTOTALINFOSET;
		usCharID			= id;

		clInfo	= *pclinfo;
	}
};

class sDBResponse_PersonTotalInfoSet
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값. (1: 성공. 0:실패) 

	sDBResponse_PersonTotalInfoSet()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonTotalInfoSet));
	}
};


//----------------------------------------------------
// Person의 전체 정보를 삭제할 것을 요청하는 클래스. 
//----------------------------------------------------
class sDBRequest_PersonTotalInfoDelete
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siIDNum;
	SI32				siIndex;
	SI32				siPersonID;		// 대상의 personid
	TCHAR				szIdenNum[ 12 ];		// 주민등록 번호 뒷자리 	
	TCHAR				szIP[ 20 ];

	sDBRequest_PersonTotalInfoDelete()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonTotalInfoDelete));
	}

	sDBRequest_PersonTotalInfoDelete(SI32 id, SI32 idnum, SI32 index, SI32 personid, TCHAR* idennum, TCHAR *ip )
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonTotalInfoDelete );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONTOTALINFODELETE;
		usCharID			= id;

		siIDNum	= idnum;

		siIndex	= index;
		siPersonID	= personid;

		StringCchCopy( szIdenNum, 12, idennum );

		memcpy( szIP, ip, 20 );
		szIP[ 19 ] = NULL;
	}
};

class sDBResponse_PersonTotalInfoDelete
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값. 
											//  1: 성공. 
											//  0: 일반적인 실패 
											// -1: 주민등록번호뒷자리 불일지 

	cltGuildInfo		clinfo;	// 길드가 있는 캐릭이면 길드 정보를 업데이트한다.
	cltTradeOrder		clOrder[MAX_MARKET_ORDER_PER_PERSON];			// DB에 저장된 주문의 정보. 

	//[추가 : 황진성 2007. 11. 5 삭제하는 캐릭터의 person id]
	SI32				siPersonID;		// 대상의 personid


	sDBResponse_PersonTotalInfoDelete()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonTotalInfoDelete));
	}
};

//----------------------------------------------------
// Person의 전체 정보를 삭제대기를 요청하는 클래스. 
//----------------------------------------------------
class sDBRequest_PersonTotalInfoPreDelete
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siIDNum;
	SI32				siIndex;
	SI32				siPersonID;		// 대상의 personid
	TCHAR				szIdenNum[ 12 ];		// 주민등록 번호 뒷자리 	
	TCHAR				szIP[ 20 ];
	SYSTEMTIME			systemTime;
	SI16 predeltype;//1이면 삭제 0이면 삭제유예기간 제거


	sDBRequest_PersonTotalInfoPreDelete()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonTotalInfoDelete));
	}

	sDBRequest_PersonTotalInfoPreDelete(SI32 id, SI32 idnum, SI32 index, SI32 personid, TCHAR* idennum, SYSTEMTIME stTime, TCHAR *ip , SI16 deltype)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonTotalInfoPreDelete );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONTOTALINFOPREDELETE;
		usCharID			= id;

		siIDNum	= idnum;

		siIndex	= index;

		systemTime = stTime;
		siPersonID	= personid;

		StringCchCopy( szIdenNum, 12, idennum );

		memcpy( szIP, ip, 20 );
		szIP[ 19 ] = NULL;
		predeltype = deltype;
	}
};

class sDBResponse_PersonTotalInfoPreDelete
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값. 
	//  1: 성공. 
	//  0: 일반적인 실패 
	// -1: 주민등록번호뒷자리 불일지 
	//[추가 : 황진성 2007. 11. 5 삭제하는 캐릭터의 person id]
	SI32				siPersonID;		// 대상의 personid

	sDBResponse_PersonTotalInfoPreDelete()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonTotalInfoPreDelete));
	}
};


//-----------------------------------------------------------------
// Person의 보너스 분배 관련 정보를 기록하도록 요청한다.
//-----------------------------------------------------------------
class sDBRequest_SetBonusInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siPersonID;			// 대상이 되는 PersonID

	cltBasicAbility		clBA;				// 기본 정보. 
	SI16				siBonus;

	sDBRequest_SetBonusInfo(SI32 id, SI32 personid, cltBasicAbility* pclba, SI16 bonus)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetBonusInfo);
		packetHeader.usCmd = DBMSG_REQUEST_SETBONUSINFO;

		siPersonID		= personid;

		clBA.Set(pclba);
		siBonus			= bonus;
	}
};


class sDBResponse_SetBonusInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	// PCK : 로그용 패킷 추가
	cltBasicAbility		cIBA;
	SI32				siPersonID;
	SI16				siBonus;

	sDBResponse_SetBonusInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetBonusInfo));
	}
};

/*-------------------------------------------------------------------
분류: Person Info
설명: Person 정보를 얻어온다 [요청]
시간: 2003-07-29
-------------------------------------------------------------------*/
class sDBRequest_PersonListInfoGet
{
public:

	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				idnum;

	sDBRequest_PersonListInfoGet(UI16 charid, SI32 siidnum)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonListInfoGet );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONLISTINFOGET;
		usCharID	= charid;
		idnum		= siidnum;
	}

};

class sDBResponse_PersonListInfoGet
{
public:	
	sPacketHeader		packetHeader;
	UI16				usCharID;	

	cltPBasicInfo	personInfo[ MAX_TOTAL_CHAR_PER_USER ];
	//cltItem			personIteminfo[ MAX_CHAR_PER_USER ][ MAX_SHAPE_ITEM ];	
	SI32			siLevel[ MAX_TOTAL_CHAR_PER_USER ];
	SYSTEMTIME		stTime[ MAX_TOTAL_CHAR_PER_USER ];

	sDBResponse_PersonListInfoGet()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonListInfoGet));
	}
};


//-----------------------------------------------------------------
// Person의 Level관련 정보를 기록하도록 요청한다.
//-----------------------------------------------------------------
class sDBRequest_SetLevelInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;			// 대상이 되는 PersonID

	SI64				siExp;	// sI32-> Si64 로 변경하면서 강제로 형변경 차후 지워야함
	SI16				siLevel;
	SI32				siFameExp;
	SI16				siFameLevel;
	SI16				siBonus;
	SI16				siSkillBonus;
	SI32				siPlayTime;

	sDBRequest_SetLevelInfo(SI32 id, SI32 personid, SI64 exp, SI16 level, SI32 fameexp, SI16 famelevel, SI16 bonus, SI16 skillbonus, SI32 PlayTime )// sI32-> Si64 로 변경하면서 강제로 형변경 차후 지워야함
	{
		packetHeader.usSize = sizeof( sDBRequest_SetLevelInfo);
		packetHeader.usCmd = DBMSG_REQUEST_SETLEVELINFO;

		usCharID		= id;
		siPersonID		= personid;
		siExp			= exp;// sI32-> Si64 로 변경하면서 강제로 형변경 차후 지워야함
		siLevel			= level;
		siFameExp		= fameexp;
		siFameLevel		= famelevel;
		siBonus			= bonus;
		siSkillBonus	= skillbonus;
		siPlayTime		= PlayTime;
	}
};


class sDBResponse_SetLevelInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;

	sDBResponse_SetLevelInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetLevelInfo));
	}
};



//---------------------------------------
// 돈을 사람간에 옮긴다. 
//--------------------------------------
class sDBRequest_MoveMoney
{
public:
	sPacketHeader		packetHeader;
	SI32				siFromPersonID;						
	SI32				siToPersonID;
	UI16				usCharID;
	SI16				siReason;						// 돈을 옮긴 사유. 
	GMONEY				siMovedMoney;					// 옮겨진 금액

	sDBRequest_MoveMoney()
	{
		ZeroMemory(this, sizeof(sDBRequest_MoveMoney));
	}

	sDBRequest_MoveMoney(UI16 charid, SI32 personfromid, SI32 persontoid, SI16 reason, GMONEY movedmoney)
	{
		packetHeader.usSize = sizeof( sDBRequest_MoveMoney );
		packetHeader.usCmd	= DBMSG_REQUEST_MOVEMONEY;

		siFromPersonID	= personfromid;
		siToPersonID	= persontoid;
		usCharID		= charid;
		siReason		= reason;
		siMovedMoney	= movedmoney;
	}
};

class sDBResponse_MoveMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siFromPersonID;						
	GMONEY				siFromPersonMoney;

	SI32				siToPersonID;						
	GMONEY				siToPersonMoney;

	UI16				usCharID;
	SI16				siReason;						// 현금이 증감된 사유. 
	GMONEY				siMovedMoney;					// 옮겨진 금액. 
	
	sDBResponse_MoveMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_MoveMoney));
	}
};

//------------------------------------------
// 스킬을 감소시킨다.
//------------------------------------------
#define DECSKILLMODE_LEVEL			1	// 레벨업에 의한 기술 감소. 
#define DECSKILLMODE_TICKET			2	// 이용권에 의한 기술 감소
#define DECSKILLMODE_WEAPONSKILL	3	// 이용권에 의한 전투향상술 기술 감소
// 기술 전체 초기화 이용권 - 2009.11.24 손성웅
#define DECSKILLMODE_ALLSKILLPOINT	4	// 기술 전체 초기화 

class sDBRequest_DecreaseSkill
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// Who
	SI32				siPersonID;					//  PersonID
	SI32				siSkillIndex;				// 몇 번째 스킬을 변경할 것인가 ?

	// what
	UI08				uiSkillUnique;				// 설정할 스킬 유니크. 
	UI08				uiDecSkillLevel;			// 감소시켜야 할 스킬 레벨 
	UI08				uiIncreaseSkillPoint;		// 다시 돌려줘야할 스킬보너스 포인트

	UI08				uiChildSkillList[MAX_CHILD_SKILL_NUM];
	
	SI16				siMode;
	SI16				siItemPos;
	cltItem				clItem;

	
	sDBRequest_DecreaseSkill(SI32 id, SI32 personid, SI32 skillindex, UI08 skillunique, UI08 decskilllevel, UI08 IncreaseSkillPoint, UI08* pcllist, SI16 mode, SI16 pos, cltItem* pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_DecreaseSkill );
		packetHeader.usCmd = DBMSG_REQUEST_DECREASESKILL;
		usCharID		= id;

		siPersonID		= personid;
		siSkillIndex	= skillindex;

		uiSkillUnique	= skillunique;
		uiDecSkillLevel	= decskilllevel;
		uiIncreaseSkillPoint = IncreaseSkillPoint;

		for(SI32 i = 0;i < MAX_CHILD_SKILL_NUM;i++)
		{
			uiChildSkillList[i] = pcllist[i];
		}

		siMode		= mode;
		siItemPos	= pos;
		if(pclitem)
		{
			clItem.Set(pclitem);
		}
		else
		{
			clItem.Init();
		}
	}
};

//------------------------------------------
// 스킬을 업그레이드 시킨다.
//------------------------------------------
class sDBRequest_IncreaseSkill
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// Who
	SI32				siPersonID;					// 돈을 입금할 PersonID

	// what
	UI08				uiSkillUnique;				// 설정할 스킬 유니크. 
	UI08				uiIncSkillLevel;			// 증가시켜야 할 스킬 레벨 
	
	UI08				uiMaxSkillLevel;			// 허용가능한 최대의 스킬레벨. 
	UI08				uiDecreaseSkillPoint;		// 감소되는 스킬 포인트

	//[추가 : 황진성 2007. 10. 25 	로그에 이용. 사용한 아이템.]
	cltItem				clUseItem;

	sDBRequest_IncreaseSkill(SI32 id, SI32 personid, UI08 skillunique, UI08 incskilllevel, UI08 maxskilllevel, UI08 decreaseskillpoint, cltItem	&UseItem)
	{
		packetHeader.usSize = sizeof( sDBRequest_IncreaseSkill );
		packetHeader.usCmd = DBMSG_REQUEST_INCREASESKILL;
		usCharID		= id;

		siPersonID		= personid;

		uiSkillUnique	= skillunique;
		uiIncSkillLevel	= incskilllevel;
		
		uiMaxSkillLevel	= maxskilllevel;
		uiDecreaseSkillPoint = decreaseskillpoint;
		
		clUseItem.Set(&UseItem);
	}
};
// 스킬 증가인지 감소인지 판단 : PCK
#define INCREASESKILL			1	// 스킬증가
#define DECREASESKILL			2	// 스킬감소

class sDBResponse_IncreaseSkill
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// result
	SI32				siResult;			// 결과값.
	
	// who
	SI32				siPersonID;

	// what
	UI08				uiSkillUnique;				// 설정된 스킬 유니크. 
	UI08				uiSkillLevel;				// 설정된 스킬 레벨 

	SI32				siSkillBonus;				// 결과로 변경된 스킬보너스 포인트.

	SI16				siMode;
	SI16				siItemPos;
	cltItem				clItem;

	//[추가 : 황진성 2007. 10. 25 	로그에 이용. 사용한 아이템.]
	cltItem				clUseItem;

	// PCK : 로그용 패킷 추가 (스킬업인가 다운인가)
	SI08				siReason;

	sDBResponse_IncreaseSkill()
	{
		ZeroMemory(this, sizeof(sDBResponse_IncreaseSkill));
	}

};

//------------------------------------------
// 스킬을 업그레이드 시킨다.
//------------------------------------------
class sDBRequest_IncreaseMasterSkill
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;

	// Who
	SI32				siPersonID;					// PersonID

	// what
	UI08				uiSkillUnique;				// 설정할 스킬 유니크. 
	UI08				uiIncSkillLevel;			// 증가시켜야 할 스킬 레벨 
	
	UI08				uiMaxSkillLevel;			// 허용가능한 최대의 스킬레벨. 

	sDBRequest_IncreaseMasterSkill(SI32 id, SI32 personid, UI08 skillunique, UI08 incskilllevel, UI08 maxskilllevel)
	{
		packetHeader.usSize = sizeof( sDBRequest_IncreaseMasterSkill );
		packetHeader.usCmd = DBMSG_REQUEST_INCREASEMASTERSKILL;
		usCharID		= id;

		siPersonID		= personid;

		uiSkillUnique	= skillunique;
		uiIncSkillLevel	= incskilllevel;
		
		uiMaxSkillLevel	= maxskilllevel;

	}
};

class sDBResponse_IncreaseMasterSkill
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// result
	SI32				siResult;			// 결과값.
	
	// who
	SI32				siPersonID;

	// what
	UI08				uiSkillUnique;				// 설정된 스킬 유니크. 
	UI08				uiSkillLevel;				// 설정된 스킬 레벨 

	sDBResponse_IncreaseMasterSkill()
	{
		ZeroMemory(this, sizeof(sDBResponse_IncreaseMasterSkill));
	}

};

//------------------------------------------
// 증가된 스킬을 설정한다.
//------------------------------------------
class sDBRequest_SetSkillExp
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	// Who
	SI32				siPersonID;					// 돈을 입금할 PersonID
	// what
	UI08				uiSkillUnique;				// 설정할 스킬 유니크. 
	SI32				siSkillExp;					// 설정할 기술 경험치 

	sDBRequest_SetSkillExp(SI32 id, SI32 personid, UI08 skillunique, SI32 skillexp)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetSkillExp );
		packetHeader.usCmd	= DBMSG_REQUEST_SETSKILLEXP;
		usCharID			= id;

		siPersonID		= personid;

		uiSkillUnique	= skillunique;
		siSkillExp		= skillexp;
	}
};

class sDBResponse_SetSkillExp
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// result
	SI32				siResult;			// 결과값.

	// PCK : 로그용 패킷 추가

	SI32				siPersonID;					// 돈을 입금할 PersonID

	UI08				uiSkillUnique;				// 설정할 스킬 유니크. 
	SI32				siSkillExp;					// 설정할 기술 경험치

	sDBResponse_SetSkillExp()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetSkillExp));
	}
};

//--------------------------
// 전투향상술 스킬을 올린다.
//---------------------------
class sDBRequest_Set_WeaponSkill
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;

	UI08				uiSkillUnique;
	UI08				uiSkillLevel;
	UI08				uiNeedSkillPoint;

	GMONEY				siMoney;

	cltItem				clNeedItem;
	SI16				siNeedItemPos;

	sDBRequest_Set_WeaponSkill(SI32 siCharID, SI32 personID, UI08 siSkillUnique, UI08 siSkillLevel, UI08 uiSkillPoint, GMONEY gmCost, cltItem* pclNeedItem, SI16 siItemPos )
	{
		packetHeader.usSize	= sizeof( sDBRequest_Set_WeaponSkill );
		packetHeader.usCmd	= DBMSG_REQUEST_SET_WEAPONSKILL;
		usCharID			= siCharID;

		siPersonID			= personID;

		uiSkillUnique		= siSkillUnique;
		uiSkillLevel		= siSkillLevel;
		uiNeedSkillPoint	= uiSkillPoint;

		siMoney				= gmCost;

		if ( pclNeedItem )	
		{
			clNeedItem.Set( pclNeedItem );
		}
		siNeedItemPos		= siItemPos;
	}
};

class sDBResponse_Set_WeaponSkill
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;

	SI32				siResult;			// 결과값.

	UI08				uiSkillUnique;		// 배운 스킬 유니크
	UI08				uiSkillLevel;		// 레벨업된 레벨

	SI16				siLeftBonus;		// 남은 스킬 보너스
	UI08				siNeedSkillBonus;	// 로그용(배울때 소모된 스킬 포인트)

	GMONEY				siLeftMoney;		// 일반 스킬배울시 남음 돈
	GMONEY				siMoney;			// 로그용(일반 스킬배울시 소모된 돈)

	cltItem				clLeftItem;			// 히든 스킬배울시 남은 아이템
	SI16				siItemPos;			// 아이템 포지션
	cltItem				clNeedItem;			// 로그용(히든 스킬배울시 소모된 아이템)

	sDBResponse_Set_WeaponSkill()
	{
		ZeroMemory(this, sizeof(sDBResponse_Set_WeaponSkill));
	}
};

//-----------------------------------------
// 타고 있는 말의 인덱스를 저장한다. 
//KHY - 1001 - 기승동물 5마리 보유 수정.
//-----------------------------------------
class sDBRequest_SetRideHorse
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI16				siIndex;		// 몇 번째 말을 타고 있는가 ?(-1 인경우 말에서 내린것임) 
	bool	siCurrentHorseRIDEHORSE;

	sDBRequest_SetRideHorse()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetRideHorse));
	}

	sDBRequest_SetRideHorse(UI16 charid, SI32 personid, SI16 index , bool CurrentHorseRIDEHORSE )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetRideHorse );
		packetHeader.usCmd	= DBMSG_REQUEST_SETRIDEHORSE;

		usCharID	= charid;
		siPersonID	= personid;
		siIndex		= index;
		siCurrentHorseRIDEHORSE = CurrentHorseRIDEHORSE;
	}
};

class sDBResponse_SetRideHorse
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	UI16				usCharID;

	SI32				siPersonID;
	SI16				siIndex;		// 몇 번째 말을 타고 있는가 ?(-1 인경우 말에서 내린것임) 
	bool	siCurrentHorseRIDEHORSE;


	sDBResponse_SetRideHorse()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetRideHorse));
	}
};


//-----------------------------------------
// 말의 자동 먹기 기능을 설정한다. 
//-----------------------------------------
class sDBRequest_SetAutoFood
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	bool				bAutoFoodSwitch;

	sDBRequest_SetAutoFood()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetAutoFood));
	}

	sDBRequest_SetAutoFood(UI16 charid, SI32 personid, bool bauto)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetAutoFood );
		packetHeader.usCmd	= DBMSG_REQUEST_SETAUTOFOOD;

		usCharID		= charid;
		siPersonID		= personid;
		bAutoFoodSwitch	= bauto;
	}
};

class sDBResponse_SetAutoFood
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	UI16				usCharID;

	SI32				siPersonID;
	bool				bAutoFoodSwitch;

	sDBResponse_SetAutoFood()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetAutoFood));
	}
};



// 말의 시세를 설정한다.  - Insert 또는 Update
class sDBRequest_SetCurrentHorsePrice
{
public:
	sPacketHeader		packetHeader;

	SI16				siHorseUnique;				// 말의 종류( 품종 )
	GMONEY				siHorseCurrentPrice;		// 말의 현재 시세

	sDBRequest_SetCurrentHorsePrice( SI16 HorseUnique, GMONEY HorseCurrentPrice )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetCurrentHorsePrice );
		packetHeader.usCmd	= DBMSG_REQUEST_SETCURRENTHORSEPRICE;
	
		siHorseUnique = HorseUnique;
		siHorseCurrentPrice = HorseCurrentPrice;
	}
};

class sDBResponse_SetCurrentHorsePrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;					// 1 : 성공, Other : 실패

	SI16				siHorseUnique;				// 말의 종류( 품종 )
	GMONEY				siHorseCurrentPrice;		// 말의 현재 시세

	sDBResponse_SetCurrentHorsePrice()
	{
		memset(this, 0, sizeof(sDBResponse_SetCurrentHorsePrice));
	}
};

// 말의 시세를 얻어온다.
class sDBRequest_GetCurrentHorsePrice
{
public:
	sPacketHeader		packetHeader;

	SI16				siHorseUnique;				// 말의 종류( 품종 )	

	sDBRequest_GetCurrentHorsePrice( SI16 HorseUnique )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetCurrentHorsePrice );
		packetHeader.usCmd	= DBMSG_REQUEST_GETCURRENTHORSEPRICE;
	
		siHorseUnique = HorseUnique;
	}
};

class sDBResponse_GetCurrentHorsePrice
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;					// 1 : 성공, Other : 실패

	SI16				siHorseUnique;				// 말의 종류( 품종 )
	GMONEY				siHorseCurrentPrice;		// 말의 현재 시세

	sDBResponse_GetCurrentHorsePrice()
	{
		memset(this, 0, sizeof(sDBResponse_GetCurrentHorsePrice));
	}
};


//-----------------------------------------
// 말 한마리의 정보를 저장한다. 
//-----------------------------------------
#define SETHORSE_REASON_MAKE		1	// 말 신규로 생성. 
#define SETHORSE_REASON_UPDATE		2	// 말 정보 업데이트. 
#define SETHORSE_REASON_FREE		3	// 말 풀어주기.
#define SETHORSE_REASON_DIE			4	// 말 죽이기 
#define SETHORSE_REASON_CHANGETYPE	5	// 말 품종 변경
#define SETHORSE_REASON_GAMEMASTER	6	// 운영자 명령에 의해. 
#define SETHORSE_REASON_TICKET		7	// 말 능력 향상이용권에 의해서 변화
#define SETHORSE_REASON_PREMIUM_PARTS		8	// 말 능력상승 프리미엄 파츠 사용에 대한 변화

class sDBRequest_SetHorse
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI16				siReason;		// 말을 설정하는 사유. 
	SI16				siIndex;		// 어느 말의 정보를 업데이트 하는가.	

	cltHorse			clHorse;		// 말의 정보. 	

	sDBRequest_SetHorse()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetHorse));
	}

	sDBRequest_SetHorse(UI16 charid, SI32 personid, SI16 reason, SI16 index, cltHorse* pclhorse)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetHorse );
		packetHeader.usCmd	= DBMSG_REQUEST_SETHORSE;

		usCharID	= charid;
		siPersonID	= personid;
		siReason	= reason;
		siIndex		= index;

		if(pclhorse)	clHorse.Set(pclhorse);
	}
};

class sDBResponse_SetHorse
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;		// 결과값.

	UI16				usCharID;
	SI32				siPersonID;
	SI16				siReason;		// 말을 설정하는 사유.
	SI16				siIndex;		// 어느 말의 정보를 업데이트 하는가.

	// PCK : 로그용 패킷추가
	cltHorse			cIHorse;

	sDBResponse_SetHorse()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetHorse));
	}
};

//-----------------------------------------
// 현금액을 증감시킨다.
//-----------------------------------------
// [영훈] define에서 enum으로 변경
enum CHANGE_MONEY_REASON
{
	CHANGE_MONEY_REASON_NOTDEFINED			= 1		// 정의되지 않은 이유.
	,CHANGE_MONEY_REASON_QUESTREWARD		// 퀘스트 보상에 의한 증가. 
	,CHANGE_MONEY_REASON_DEATH				// 사망에 의한 감소. 
	,CHANGE_MONEY_REASON_TRADE				// 물품 교환에 의한 변경. 
	,CHANGE_MONEY_REASON_WARWIN				// 전쟁에서 승리하여 돈을 받음. 
	,CHANGE_MONEY_REASON_GAMEMASTER			// 운영자에 의해서 돈을 지급받음. 
	,CHANGE_MONEY_REASON_REPAIRPAY			// 수리비로 지불. 
	,CHANGE_MONEY_REASON_REPAIRGET			// 수리비로 얻음.
	//---------------------------------------------------------------------------
	// 중간에 빠진 숫자는 원래부터 그렇습니다. 왜그런지 이유를 몰라서 그냥 나뒀음
	//---------------------------------------------------------------------------
	,CHANGE_MONEY_REASON_HUNTFEE			= 11		// 수렵장 입장료 지불. 
	,CHANGE_MONEY_REASON_HUNTREWARD						// 수렵장 포상금 받음. 
	,CHANGE_MONEY_REASON_FATHERREWARD					// 스승 포상금. 
	,CHANGE_MONEY_REASON_RECOMMENDER					// 추천인 포상. 
	,CHANGE_MONEY_REASON_USEMARKET						// 객주 이용에 따른 세금 납부. 
	,CHANGE_MONEY_REASON_NPCREPAIRPAY					// NPC(장영실) 에게 수리비로 지불. 
	,CHANGE_MONEY_REASON_INVESTMENTINFO					// 투자상담자(김선달) 조회비로 지불
	,CHANGE_MONEY_REASON_MAKE							// 제조의뢰로 수수료 지불. 
	,CHANGE_MONEY_REASON_FIZATION						// 신입회원 정착 지원금 지불.
	,CHANGE_MONEY_REASON_BLACKARMYAPPLY					// 흑의군대 전쟁 참가비 지불
	,CHANGE_MONEY_REASON_BUYBIDSTOCK					// 주식 구입비 지불
	,CHANGE_MONEY_REASON_WORLDTRADEBUY					// 무역시 세금 지불
	,CHANGE_MONEY_REASON_CRAFTFUND						// 장인 육성 지원금
	,CHANGE_MONEY_REASON_PUPILREWARD					// 제자가 50레벨이 되어서 자신이 돈을 받는다.
	,CHANGE_MONEY_REASON_GRANDFATHERREWARD				// 내 제자의 제자가 50레벨이 되어서 내가 포상금을 받는다. 
	,CHANGE_MONEY_REASON_MOFUMOFUEVENT_WARP				// 모후모후 이벤트 자동워프에 지불
	//KHY - 0703 - 게임 머니로 귀환하기.
	,CHANGE_MONEY_REASON_MONEY_WARP						// 돈으로 워프 지불
	,CHANGE_MONEY_REASON_WORDSPECIALQUEST				// 요술램프 퀘스트 완료
	,CHANGE_MONEY_REASON_BV_STOCKDIVIDEND				// [영훈] 초보자 마을 : 주주 배당
	,CHANGE_MONEY_REASON_BUFF_ROULETTE					// [진성] 영웅의 동상 이벤트 룰렛으로 버프를 얻고 지불 => 2008-6-11
	,CHANGE_MONEY_REASON_MARKETCONDITIONS				// [희영] 시세확인으로 지불.
	,CHANGE_MONEY_REASON_CHAR_ROULETTE					// [강형] 할로윈 이벤트 변신 룰렛으로 버프를 얻고 지불 => 2008-6-11
	,CHANGE_MONEY_REASON_NEWYEAREVENT_ROULETTE			// [춘기] 신년이벤트 운세룰렛으로 버프를 얻고 지불 => 2008.11.25
	,CHANGE_MONEY_REASON_USENEWMARKET					// 통합시장 이용
	,CHANGE_MONEY_REASON_DONATION08_DONATE				// 통합시장 이용
	,CHANGE_MONEY_REASON_GAWIBAWIBO_CHARGED_MONEY		//  [8/12/2009 종호_GAWIBAWIBO] 축제-가위바위보
	,CHANGE_MONEY_REASON_GAWIBAWIBO_REWARD_MONEY		//  [8/12/2009 종호_GAWIBAWIBO] 축제-가위바위보
	,CHANGE_MONEY_REASON_GAWIBAWIBO_REWARD_MAX_MONEY	//  [8/22/2009 종호_GAWIBAWIBO] 축제-가위바위보 최고금액
	,CHANGE_MONEY_REASON_RACOONDODGE_USEMONEY			//  라쿤 피하기 이벤트로 돈을 사용 
	,CHANGE_MONEY_REASON_RACOONDODGE_REWARD				//  라쿤 피하기 이벤트로 돈을 사용 
	,CHANGE_MONEY_REASON_BOKBULBOK_ENTRANCE_FEE			//	복불복 입장료
	,CHANGE_MONEY_REASON_BOKBULBOK_REWARD				//	복불복 보상
	,CHANGE_MONEY_REASON_LEVELLIMITBOX_REWARD			// [성웅] 레벨 보상 상자에서 아이템을 준다 
	,CHANGE_MONEY_REASON_VALENTINEDAY_BUY_ITEM			// [진성] 발렌타인데이에 NPC가 판매하는 아이템을 구입하여 소비되 금액

};
/*
#define CHANGE_MONEY_REASON_NOTDEFINED	1	// 정의되지 않은 이유.
#define CHANGE_MONEY_REASON_QUESTREWARD	2	// 퀘스트 보상에 의한 증가. 
#define CHANGE_MONEY_REASON_DEATH		3	// 사망에 의한 감소. 
#define CHANGE_MONEY_REASON_TRADE		4	// 물품 교환에 의한 변경. 
#define CHANGE_MONEY_REASON_WARWIN		5	// 전쟁에서 승리하여 돈을 받음. 
#define CHANGE_MONEY_REASON_GAMEMASTER	6	// 운영자에 의해서 돈을 지급받음. 
#define CHANGE_MONEY_REASON_REPAIRPAY	7	// 수리비로 지불. 
#define CHANGE_MONEY_REASON_REPAIRGET	8	// 수리비로 얻음.
#define CHANGE_MONEY_REASON_HUNTFEE		11	// 수렵장 입장료 지불. 
#define CHANGE_MONEY_REASON_HUNTREWARD	12	// 수렵장 포상금 받음. 
#define CHANGE_MONEY_REASON_FATHERREWARD 13	// 스승 포상금. 
#define CHANGE_MONEY_REASON_RECOMMENDER	 14	// 추천인 포상. 
#define CHANGE_MONEY_REASON_USEMARKET	15  // 객주 이용에 따른 세금 납부. 
#define CHANGE_MONEY_REASON_NPCREPAIRPAY	16	// NPC(장영실) 에게 수리비로 지불. 
#define CHANGE_MONEY_REASON_INVESTMENTINFO	17  // 투자상담자(김선달) 조회비로 지불
#define CHANGE_MONEY_REASON_MAKE			18  // 제조의뢰로 수수료 지불. 
#define CHANGE_MONEY_REASON_FIZATION		19  // 신입회원 정착 지원금 지불.
#define CHANGE_MONEY_REASON_BLACKARMYAPPLY	20	// 흑의군대 전쟁 참가비 지불
#define CHANGE_MONEY_REASON_BUYBIDSTOCK		21  // 주식 구입비 지불
#define CHANGE_MONEY_REASON_WORLDTRADEBUY	22	// 무역시 세금 지불
#define	CHANGE_MONEY_REASON_CRAFTFUND		23	// 장인 육성 지원금
#define CHANGE_MONEY_REASON_PUPILREWARD		24	// 제자가 50레벨이 되어서 자신이 돈을 받는다.
#define CHANGE_MONEY_REASON_GRANDFATHERREWARD		25	// 내 제자의 제자가 50레벨이 되어서 내가 포상금을 받는다. 
#define CHANGE_MONEY_REASON_MOFUMOFUEVENT_WARP		26	// 모후모후 이벤트 자동워프에 지불
//KHY - 0703 - 게임 머니로 귀환하기.
#define CHANGE_MONEY_REASON_MONEY_WARP		27	// 돈으로 워프 지불
#define CHANGE_MONEY_REASON_WORDSPECIALQUEST	28	// 요술램프 퀘스트 완료
*/

class sDBRequest_ChangeMoney
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;						// 퀘스트 대상자. 
	UI16				usCharID;
	SI16				siReason;						// 현금이 증감된 사유. 
	GMONEY				siChangedMoney;					// 증감된 금액. (+: 증가, -:감소) 
	TCHAR				strReason[ 128 ];

	sDBRequest_ChangeMoney()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeMoney));
	}

	sDBRequest_ChangeMoney(UI16 charid, SI32 personid, SI16 reason, GMONEY changedmoney, TCHAR *pReason = NULL )
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeMoney );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGEMONEY;

		siPersonID	= personid;
		usCharID	= charid;
		siReason	= reason;
		siChangedMoney = changedmoney;

		if ( pReason )
		{
			MStrCpy( strReason, pReason, 128 );
		}
		else
		{
			strReason[ 0 ] = NULL;
		}
	}
};

class sDBResponse_ChangeMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;						
	UI16				usCharID;
	SI16				siReason;						// 현금이 증감된 사유. 
	GMONEY				siChangedMoney;					// 증감된 금액. (+: 증가, -:감소) 
	GMONEY				siMoney;						// 최종 현금 보유액. 
	
	SI32				siTargetPersonID;				// 거래 대상자

	sDBResponse_ChangeMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeMoney));
	}
};

//-----------------------------------------
// 퀘스트 정보를 저장한다.
//-----------------------------------------
class sDBRequest_SetQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;						// 퀘스트 대상자. 
	SI16				siQuestType;					// 어떤 종류의 퀘스트인가?
	cltPersonQuestUnitInfo clQuestInfo;					// 저장해야할 퀘스트 정보. 

	sDBRequest_SetQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetQuestInfo));
	}

	sDBRequest_SetQuestInfo(SI32 personid, SI16 questtype, cltPersonQuestUnitInfo* pclinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetQuestInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETQUESTINFO;

		siPersonID	= personid;
		siQuestType	= questtype;
		clQuestInfo.Set(pclinfo);
	}
};

class sDBResponse_SetQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	// PCK - 로그용 패킷
	SI32				siPersonID;
	SI16				siQuestType;
	cltPersonQuestUnitInfo	clQuestInfo;

	sDBResponse_SetQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetQuestInfo));
	}
};

// [지연] : 복불복 - 누적보상금 저장
class sDBRequest_BBB_Set_RewardMoney
{
public:
	sPacketHeader	packetHeader;

	SI64			m_siTotalRewardPriceDeposit;	// 최종 누적 금액

	sDBRequest_BBB_Set_RewardMoney()
	{
		ZeroMemory(this, sizeof(sDBRequest_BBB_Set_RewardMoney));
	}

	sDBRequest_BBB_Set_RewardMoney(SI64 siTotalRewardPriceDeposit)
	{
		packetHeader.usSize = sizeof( sDBRequest_BBB_Set_RewardMoney );
		packetHeader.usCmd	= DBMSG_REQUEST_BBB_SET_REWARD_MONEY;

		m_siTotalRewardPriceDeposit = siTotalRewardPriceDeposit;
	}
};

class sDBResponse_BBB_Set_RewardMoney
{
public:
	sPacketHeader	packetHeader;

	SI64	m_siTotalRewardPriceDeposit;	// 최종 누적 금액

	sDBResponse_BBB_Set_RewardMoney()
	{
		ZeroMemory( this, sizeof(sDBResponse_BBB_Set_RewardMoney) );
	}
};

// [지연] : 복불복 - 누적보상금 얻기
class sDBRequest_BBB_Get_RewardMoney 
{
public:
	sPacketHeader	packetHeader;

	SI64			m_siTotalRewardPriceWithDraw;	// 최종 누적 금액

	sDBRequest_BBB_Get_RewardMoney()
	{
		ZeroMemory(this, sizeof(sDBRequest_BBB_Get_RewardMoney));
	}

	sDBRequest_BBB_Get_RewardMoney(SI64 siTotalRewardPriceWithDraw)
	{
		packetHeader.usSize = sizeof( sDBRequest_BBB_Get_RewardMoney );
		packetHeader.usCmd	= DBMSG_REQUEST_BBB_GET_REWARD_MONEY;

		m_siTotalRewardPriceWithDraw = siTotalRewardPriceWithDraw;
	}
};


// [지연] : 복불복 - 누적보상금 얻기 - Response
class sDBResponse_BBB_Get_RewardMoney
{
public:
	sPacketHeader	packetHeader;

	SI64			m_siTotalRewardPriceWithDraw;	// 최종 누적 금액

	sDBResponse_BBB_Get_RewardMoney()
	{
		ZeroMemory(this, sizeof(sDBResponse_BBB_Get_RewardMoney));
	}

	sDBResponse_BBB_Get_RewardMoney(SI64 siTotalRewardPriceWithDraw)
	{
		packetHeader.usSize = sizeof( sDBResponse_BBB_Get_RewardMoney );
		packetHeader.usCmd	= DBMSG_RESPONSE_BBB_GET_REWARD_MONEY;

		m_siTotalRewardPriceWithDraw = siTotalRewardPriceWithDraw;
	}
};
//-----------------------------------------
// 특수 퀘스트 정보를 저장한다.
//-----------------------------------------
class sDBRequest_SetSpecialQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;						// 퀘스트 대상자. 

	cltQuestInfoCommon	clSpecialQuestInfo;
	bool bWordAssemblySwitch[MAX_WORD_ASSEMBLY_CHAR_NUMBER];

	bool bClearWordSpecialQuest;					// 단어 조합을 완료했는가?
	bool bEventClearWordSpecialQuest;				// 이벤트기간인지 체크하는 변수 (제이)

	sDBRequest_SetSpecialQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetSpecialQuestInfo));
	}

	sDBRequest_SetSpecialQuestInfo(SI32 personid, cltQuestInfoCommon* pclinfo, bool* pbword,
								   bool clearwordquest=false, bool eventclearwordquest=false) //제이
	{
		packetHeader.usSize = sizeof( sDBRequest_SetSpecialQuestInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETSPECIALQUESTINFO;

		siPersonID	= personid;
		clSpecialQuestInfo.Set(pclinfo);

		for(SI32 i = 0;i < MAX_WORD_ASSEMBLY_CHAR_NUMBER;i++)
		{
			bWordAssemblySwitch[i] = pbword[i];
		}

		bClearWordSpecialQuest = clearwordquest ;
		bEventClearWordSpecialQuest = eventclearwordquest;		//제이
		
	}
};

class sDBResponse_SetSpecialQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;

	bool				bClearWordSpecialQuest;			// 단어 조합을 완료했는가?
	bool				bEventClearWordSpecialQuest;	// 이벤트기간인지 체크하는 변수 (제이)

	SI32				siClearWordSpecialQuestNum;		// 흥부박 단어 조합 성공한 수
	SI32				siEventClearWordSpecialQuestNum;// 요술램프 이벤트 단어 조합 성공한 수.

	// PCK :  로그용 패킷 추가
	cltQuestInfoCommon	clSpecialQuestInfo;
	bool bWordAssemblySwitch[MAX_WORD_ASSEMBLY_CHAR_NUMBER];
	
	sDBResponse_SetSpecialQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetSpecialQuestInfo));
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------
//	신년이벤트 - 흉괘없애는 퀘스트 추가.
//---------------------------------------
class sDBRequest_SetRemoveUnluck_QuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;					// 퀘스트 대상자
	cltQuestInfoCommon	clRemoveUnLuckQuestInfo;

	sDBRequest_SetRemoveUnluck_QuestInfo()
	{
		ZeroMemory( this, sizeof(sDBRequest_SetRemoveUnluck_QuestInfo) );
	}

	sDBRequest_SetRemoveUnluck_QuestInfo(SI32 personid, cltQuestInfoCommon* pclinfo )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetRemoveUnluck_QuestInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETREMOVEUNLUCKQUESTINFO;

		siPersonID	= personid;
		clRemoveUnLuckQuestInfo.Set(pclinfo);
	}
};

class sDBResponse_SetRemoveUnluck_QuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32		siPersonID;
	SI32		siresult;		// 결과값


	sDBResponse_SetRemoveUnluck_QuestInfo()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetRemoveUnluck_QuestInfo) );
	}

};

//////////////////////////////////////////////////////////////////////////////////////////////////////

// naming - 네이밍 퀘스트 저장
class sDBRequest_SetNamingQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;						// 퀘스트 대상자. 
	cltQuestInfoCommon	clNamingQuestInfo;

	sDBRequest_SetNamingQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetNamingQuestInfo));
	}

	sDBRequest_SetNamingQuestInfo(SI32 personid, cltQuestInfoCommon* pclinfo )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetNamingQuestInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETNAMINGQUESTINFO;

		siPersonID	= personid;
		clNamingQuestInfo.Set(pclinfo);
	}
};

class sDBResponse_SetNamingQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;

	// PCK : 로그용 패킷 추가
	cltQuestInfoCommon clNamingQuestInfo;

	sDBResponse_SetNamingQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetNamingQuestInfo));
	}
};

class sDBRequest_SetPersonNamingInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siIndex;
	SI32				siLevel;

	sDBRequest_SetPersonNamingInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetPersonNamingInfo));
	}

	sDBRequest_SetPersonNamingInfo(SI32 personid, SI32 index, SI32 level )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetPersonNamingInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETPERSONNAMINGINFO;

		siPersonID	= personid;
		siIndex = index;
		siLevel = level;
	}
};

class sDBResponse_SetPersonNamingInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siResult;			// 결과값.
	SI32				siIndex;
	SI32				siLevel;

	sDBResponse_SetPersonNamingInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetPersonNamingInfo));
	}
};

class sDBRequest_SetPersonCurrentNaming
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siSelectedIndex;
	SI32				siSelectedLevel;

	sDBRequest_SetPersonCurrentNaming()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetPersonCurrentNaming));
	}

	sDBRequest_SetPersonCurrentNaming(SI32 personid, SI32 index, SI32 level )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetPersonNamingInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETPERSONCURRENTNAMING;

		siPersonID	= personid;
		siSelectedIndex = index;
		siSelectedLevel = level;
	}
};

class sDBResponse_SetPersonCurrentNaming
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siResult;			// 결과값.
	SI32				siSelectedIndex;
	SI32				siSelectedLevel;

	sDBResponse_SetPersonCurrentNaming()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetPersonCurrentNaming));
	}
};

//KHY - 0822 - 해피빈 퀘스트 추가.
class sDBRequest_SetHappyBeanQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;						// 퀘스트 대상자. 
	cltQuestInfoCommon	clHappyBeanQuestInfo;

	sDBRequest_SetHappyBeanQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetHappyBeanQuestInfo));
	}

	sDBRequest_SetHappyBeanQuestInfo(SI32 personid, cltQuestInfoCommon* pclinfo )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetHappyBeanQuestInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETHAPPYBEANQUESTINFO;

		siPersonID	= personid;
		clHappyBeanQuestInfo.Set(pclinfo);
	}
};

//KHY - 0822 - 해피빈 퀘스트 추가. 이벤트 진행 정보를 DB로 보내 진행 여부를확인한다.
class sDBRequest_SetHappyBeanClearInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siQuestType; // QUEST_TYPE_HAPPYBEAN
	SI32				siQuestState; // QUEST_HAPPYBEAN_STEP_NON, QUEST_HAPPYBEAN_STEP_START, QUEST_HAPPYBEAN_STEP_COMPLETE

	sDBRequest_SetHappyBeanClearInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetHappyBeanClearInfo));
	}

	sDBRequest_SetHappyBeanClearInfo(SI32 personid, SI32 questtype, SI32 QuestState )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetHappyBeanClearInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_GETHAPPYBEANQUESTINFO;

		siPersonID	= personid;
		siQuestType = questtype;
		siQuestState = QuestState;
	}
};

//KHY - 0822 - 해피빈 퀘스트 추가. 이벤트 진행관련 정보를 DB로 부터 전달 받아 확인한다.
class sDBResponse_GetHappyBeanClearInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siQuestType; // QUEST_TYPE_HAPPYBEAN
	SI32				siQuest_StartCount; //  QUEST_HAPPYBEAN_STEP_START 의 카운터. 초기화 시키지 않음.
	SI32				siQuest_CompleteCount; //  QUEST_HAPPYBEAN_STEP_COMPLETE 의 카운터. 1일에 한번 초기화.

//	SI32				siResult;			// 결과값. 0 = 퀘스트 불허. 1= 퀘스트 진행 허가. 2= 퀘스트 완료가 성공적으로 저장 되었음.


	sDBResponse_GetHappyBeanClearInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetHappyBeanClearInfo));
	}
};

//------------------------------------------------------------------------------
// class : sDBRequest_SetNamingQuestInfo
// desc  : 미션 퀘스트 정보 저장
//------------------------------------------------------------------------------
class sDBRequest_SetMissionQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;						// 퀘스트 대상자. 
	cltMissionQuestInfo	clMissionQuestInfo;

	sDBRequest_SetMissionQuestInfo(SI32 personid, cltMissionQuestInfo* pclinfo )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetMissionQuestInfo );
		packetHeader.usCmd	= DBMSG_REQUEST_SETMISSIONQUESTINFO;

		siPersonID	= personid;
		clMissionQuestInfo.Set(pclinfo);
	}
};

class sDBResponse_SetMissionQuestInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	SI32				siPersonID;

	// LEEKH : 로그용 패킷 추가
	cltMissionQuestInfo clMissionQuestInfo;

	sDBResponse_SetMissionQuestInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetMissionQuestInfo));
	}
};

/*-------------------------------------------------------------------
분류: Person Info
설명: Person 생성 [요청]
시간: 2003-07-29
-------------------------------------------------------------------*/
class sDBRequest_PersonTotalInfoCreate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	sDBPersonTotalInfo	info;

	sDBRequest_PersonTotalInfoCreate()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonTotalInfoCreate));
	}

	sDBRequest_PersonTotalInfoCreate(SI32 id, sDBPersonTotalInfo* pclinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonTotalInfoCreate );
		packetHeader.usCmd = DBMSG_REQUEST_PERSONTOTALINFOCREATE;
		usCharID	= id;

		info = *pclinfo;
	}
};

/*-------------------------------------------------------------------
분류: Person Info
Person 생성 [응답]
시간: 2003-07-29
-------------------------------------------------------------------*/
class sDBResponse_PersonTotalInfoCreate
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;	

	SI32				siMakeNumber;		// 이 계정에서 캐릭터가 생성된 횟수. 

	SI32				siResult;			// 결과값. (1: 성공. 0:실패) 

	//KHY - 1024 - 추가 정보.
	TCHAR				szAccountID[MAX_PLAYER_NAME];	// 이 Person이 소속된 계정의 아이디. 
	TCHAR 				szName[MAX_PLAYER_NAME];		// 캐릭터 ID.
	SI32				siPersonID;						// 캐릭터 PersonID

	sDBResponse_PersonTotalInfoCreate()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonTotalInfoCreate));
	}

};

//-----------------------------------------
// 건강 정보를 저장한다. 
//-----------------------------------------
class sDBRequest_SetHealth
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	cltHealthInfo		clHealthInfo;

	sDBRequest_SetHealth()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetHealth));
	}

	sDBRequest_SetHealth(SI32 personid, cltHealthInfo* pclhealthinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetHealth );
		packetHeader.usCmd	= DBMSG_REQUEST_SETHEALTH;

		siPersonID	= personid;
		clHealthInfo.Set(pclhealthinfo);
	}
};

class sDBResponse_SetHealth
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	sDBResponse_SetHealth()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetHealth));
	}
};





//-----------------------------------------
// Pos, Life, Mana, PlaySecond를 저장하도록 한다. 
//-----------------------------------------
class sDBRequest_SetPosLifeMana
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	cltCharPos			clPos;
	SI32				siLife;
	SI32				siMana;
	SI32				siPlaySecond;
	SI32				siPlayEventSecond;
	cltPos				clLastMainMapPos;

	sDBRequest_SetPosLifeMana()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetPosLifeMana));
	}

	sDBRequest_SetPosLifeMana(SI32 personid, cltCharPos* pclpos, SI32 life, SI32 mana, SI32 playsecond, SI32 playeventsecond, cltPos* pcllastmainmappos)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetPosLifeMana );
		packetHeader.usCmd	= DBMSG_REQUEST_SETPOSLIFEMANA;

		siPersonID	= personid;
		clPos.Set(pclpos);
		siLife	= life;
		siMana	= mana;
		siPlaySecond	= playsecond;
		siPlayEventSecond	= playeventsecond;
		clLastMainMapPos.Set(pcllastmainmappos);
	}
};

class sDBResponse_SetPosLifeMana
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	// PCK : 로그에 남길 패킷 추가

	SI32				siPersonID;
	cltCharPos			clPos;
	SI32				siLife;
	SI32				siMana;
	SI32				siPlaySecond;
	SI32				siPlayEventSecond;
	cltPos				clLastMainMapPos;

	sDBResponse_SetPosLifeMana()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetPosLifeMana));
	}
};



//-----------------------------------------
// 군주 계정 아이디를 받아온다.
//-----------------------------------------
class sDBRequest_Userid
{
public:	
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siMyPersonID;
	SI32				siToPersonID;
	TCHAR				szToPersonName[ MAX_PLAYER_NAME ];

	sDBRequest_Userid( UI16 CharID, SI32 MyPersonID, SI32 ToPersonID )
	{
		packetHeader.usSize = sizeof( sDBRequest_Userid );
		packetHeader.usCmd	= DBMSG_REQUEST_USERID;

		usCharID = CharID;
		siMyPersonID = MyPersonID;
		siToPersonID = ToPersonID;

		szToPersonName[ 0 ] = NULL;
	}	

	sDBRequest_Userid( UI16 CharID, SI32 MyPersonID, TCHAR *szname )
	{
		packetHeader.usSize = sizeof( sDBRequest_Userid );
		packetHeader.usCmd	= DBMSG_REQUEST_USERID;

		usCharID = CharID;
		siMyPersonID = MyPersonID;
		siToPersonID = 0;

		memcpy( szToPersonName, szname, MAX_PLAYER_NAME );
		szToPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
	}
};

class sDBResponse_Userid
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;	
	SI32				siPersonID;
	SI32				siResult;			// 결과값.
	TCHAR				strUserID[ 20 ];

	bool				bShowPuchikomi;

	sDBResponse_Userid()
	{
		ZeroMemory(this, sizeof(sDBResponse_Userid));
	}
};

//----------------------------------------
// 학교등록. 
//----------------------------------------
class sDBRequest_SetSchool
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI16				siSchoolUnique;		// 학교의 유니크. 

	sDBRequest_SetSchool()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetSchool));
	}

	sDBRequest_SetSchool(UI16 charid, SI32 personid, SI16 schoolunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetSchool );
		packetHeader.usCmd	= DBMSG_REQUEST_SETSCHOOL;

		usCharID		= charid;
		siPersonID		= personid;
		siSchoolUnique	= schoolunique;
	}
};

class sDBResponse_SetSchool
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	sDBResponse_SetSchool()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetSchool));
	}
};



//----------------------------------------
// 스승 등록. 
//----------------------------------------
class sDBRequest_SetFather
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	TCHAR				szFatherName[MAX_PLAYER_NAME];
	SI16				siMyLevel;						// 스승을 등록할 당시의 내 레벨 

	sDBRequest_SetFather()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetFather));
	}

	sDBRequest_SetFather(UI16 charid, SI32 personid, TCHAR* pfathername, SI16 mylevel)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetFather );
		packetHeader.usCmd	= DBMSG_REQUEST_SETFATHER;

		usCharID		= charid;
		siPersonID		= personid;

		MStrCpy( szFatherName, pfathername, MAX_PLAYER_NAME );

		siMyLevel	= mylevel;
	}
};

class sDBResponse_SetFather
{
public:
	sPacketHeader		packetHeader;
	

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;
	SI32				siFatherPersonID;				// 새로 설정된 스승 아이디. 
	SI16				siMyLevel;						// 스승 설정 당시의 내 레벨 
	TCHAR				szFatherName[MAX_PLAYER_NAME];	// 새로 설정된 스승 이름. 

	SI32				siPrevFatherPersonID;			// 이전 스승 아이디. (이전 스승에게 쪽지를 보내기 위해 ) 
	SI16				siChangedFatherNumber;			// 스승이 변경된 횟수. 
	TCHAR				szAccountID[MAX_PLAYER_NAME];

	sDBResponse_SetFather()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetFather));
	}
};

//----------------------------------------
// Person의 간략 정보 요청. 
//----------------------------------------
#define PERSONSIMPLEINFO_REASON_FATHERREWARD			1		// 스승 포상을 위한 간략 정보 요청. 
#define PERSONSIMPLEINFO_REASON_PROMOTER_INFO			2		// [진성] 프로모터 정보.
#define PERSONSIMPLEINFO_REASON_RECOMMEND_INTRODUCE		3		// [진성] 추천인이 피추천인에게 인사말.
#define PERSONSIMPLEINFO_REASON_RECOMMENDEE_INTRODUCE	4		// [진성] 피추천인이 추천인에게 인사말

class sDBRequest_PersonSimpleInfo
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;
	SI16				siReason;

	TCHAR				szGetCharInfoName[MAX_PLAYER_NAME];	// [진성] 정보를 얻고자 하는 캐릭터.
	TCHAR				szCharInfoName[MAX_PLAYER_NAME];	// [진성] szGetCharInfoName 요청한 캐릭터 이름.

	sDBRequest_PersonSimpleInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_PersonSimpleInfo));
	}

	sDBRequest_PersonSimpleInfo(UI16 charid, SI32 personid, SI16 reason, const TCHAR* pszGetCharInfoName = NULL, const TCHAR* pszCharInfoName = NULL )
	{
		packetHeader.usSize = sizeof( sDBRequest_PersonSimpleInfo);
		packetHeader.usCmd	= DBMSG_REQUEST_PERSONSIMPLEINFO;

		usCharID		= charid;
		siPersonID		= personid;
		siReason		= reason;

		if( pszCharInfoName )
			StringCchCopy( szCharInfoName, sizeof(szCharInfoName), pszCharInfoName );

		if( pszGetCharInfoName )
			StringCchCopy( szGetCharInfoName, sizeof(szGetCharInfoName), pszGetCharInfoName );
	}
};

class sDBResponse_PersonSimpleInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;
	SI16				siReason;
	SI16				siLevel;
	SI32				siGrandPersonID;	// 내 스승의 스승의 PersonID 
	SI16				siCharKind;

	TCHAR				szGetCharInfoName[MAX_PLAYER_NAME];	// [진성] 정보를 얻고자 하는 캐릭터.
	TCHAR				szCharInfoName[MAX_PLAYER_NAME];	// [진성] szGetCharInfoName 요청한 캐릭터 이름.

	sDBResponse_PersonSimpleInfo()
	{
		ZeroMemory(this, sizeof(sDBResponse_PersonSimpleInfo));
	}
};


//----------------------------------------
// 양성한 제자수를 늘일 것을 요청. 
//----------------------------------------
class sDBRequest_IncreaseChildNumber
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI16				siIncNum;		// 늘여야 할 수. 

	sDBRequest_IncreaseChildNumber()
	{
		ZeroMemory(this, sizeof(sDBRequest_IncreaseChildNumber));
	}

	sDBRequest_IncreaseChildNumber(SI32 personid, SI16 incnum)
	{
		packetHeader.usSize = sizeof( sDBRequest_IncreaseChildNumber);
		packetHeader.usCmd	= DBMSG_REQUEST_INCREASECHILDNUMBER;

		siPersonID		= personid;
		siIncNum		= incnum;
	}
};

class sDBResponse_IncreaseChildNumber
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI16				siChildNumber;		// 양성된 제자 수. 

	sDBResponse_IncreaseChildNumber()
	{
		ZeroMemory(this, sizeof(sDBResponse_IncreaseChildNumber));
	}
};


//----------------------------------------
// 제자의 리스트를  구한다. 
//----------------------------------------
class sDBRequest_GetChildList
{
public:
	sPacketHeader		packetHeader;

	UI16				usCharID;
	SI32				siPersonID;

	sDBRequest_GetChildList()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetChildList));
	}

	sDBRequest_GetChildList(UI16 id, SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetChildList);
		packetHeader.usCmd	= DBMSG_REQUEST_GETCHILDLIST;

		usCharID		= id;
		siPersonID		= personid;
	}
};

class sDBResponse_GetChildList
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;

	cltChildInfo		clChildInfo[MAX_CHILDINFO_NUMBER];

	sDBResponse_GetChildList()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetChildList));
	}
};

//-----------------------------------------
// 경험치를  증감시킨다.
//-----------------------------------------
#define CHANGE_EXP_REASON_FATHER	1	// 후견 성공으로 경험치 지급. 
#define CHANGE_EXP_REASON_HUNTCHIEF	2	// 수렵장 행수에게 경험치 지급. 
class sDBRequest_ChangeExp
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;						// 퀘스트 대상자. 
	SI16				siReason;						// 증감된 사유. 
	SI32				siChangedExp;					// 증감된 수치 (+: 증가, -:감소) 

	sDBRequest_ChangeExp()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeExp));
	}

	sDBRequest_ChangeExp(SI32 personid, SI16 reason, SI32 changedexp)
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeExp );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGEEXP;

		siPersonID		= personid;
		siReason		= reason;
		siChangedExp	= changedexp;
	}
};

class sDBResponse_ChangeExp
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;						// 대상자. 
	SI16				siReason;						// 증감된 사유. 
	SI32				siChangedExp;					// 증감된 금액. (+: 증가, -:감소) 
	SI64				siExp;							// 최종 경험치 보유값. // sI32-> Si64 로 변경하면서 강제로 형변경 차후 지워야함
	
	sDBResponse_ChangeExp()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeExp));
	}
};


//-----------------------------------------
// 명성 경험치를  증감시킨다.
//-----------------------------------------
#define CHANGE_FAMEEXP_REASON_FATHER		1	// 후견 성공으로 경험치 지급. 
#define CHANGE_FAMEEXP_REASON_RECOMMEND		2	// 추천인으로 경험치 지급. 

class sDBRequest_ChangeFameExp
{
public:
	sPacketHeader		packetHeader;
	SI32				siPersonID;						// 퀘스트 대상자. 
	SI16				siReason;						// 증감된 사유. 
	SI32				siChangedFameExp;				// 증감된 수치 (+: 증가, -:감소) 

	sDBRequest_ChangeFameExp()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeFameExp));
	}

	sDBRequest_ChangeFameExp(SI32 personid, SI16 reason, SI32 changedfameexp)
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeFameExp );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGEFAMEEXP;

		siPersonID			= personid;
		siReason			= reason;
		siChangedFameExp	= changedfameexp;
	}
};

class sDBResponse_ChangeFameExp
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siPersonID;			// 대상자. 
	SI16				siReason;			// 증감된 사유. 
	SI32				siChangedFameExp;	// 증감된 금액. (+: 증가, -:감소) 
	SI32				siFameExp;			// 최종 경험치 보유값. 
	
	sDBResponse_ChangeFameExp()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeFameExp));
	}
};
//-----------------------------------------
// 스테이터스 초기화 요청 
//-----------------------------------------
#define INITSTATUSMODE_LEVEL_10 			1		// 10 레벨 도달에 의한 초기화. 
#define INITSTATUSMODE_TICKET				2		// 이용권 사용한 초기화.
#define INITSTATUSMODE_LEVEL_30 			3		// 30 레벨 도달에 의한 초기화. (추가됨[영진])
#define INITSTATUSMODE_CHINA_EVENTTICKET	4
#define INITSTATUSMODE_GMTOOL				5

class sDBRequest_InitStatus
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siPersonID;						// 초기화  대상자. 

	SI16				siMode;
	cltItem				clItem;							// 삭제할 아이템의 유니크. 
	SI16				siItemPos;						// 삭제할 물품이 있다면,, 

	sDBRequest_InitStatus()
	{
		ZeroMemory(this, sizeof(sDBRequest_InitStatus));
	}

	sDBRequest_InitStatus(UI16 charid, SI32 personid, SI16 mode, SI16 itempos, cltItem* pclitem)
	{
		packetHeader.usSize = sizeof( sDBRequest_InitStatus );
		packetHeader.usCmd	= DBMSG_REQUEST_INITSTATUS;

		usCharID		= charid;
		siPersonID		= personid;
		siMode			= mode;
		siItemPos		= itempos;
		if(pclitem)
		{
			clItem.Set(pclitem);
		}
		else
		{
			clItem.Init();

		}
	}
};

class sDBResponse_InitStatus
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	UI16				usCharID;
	SI32				siPersonID;						// 대상자. 

	SI16				siMode;
	cltItem				clItem;							// 삭제할 아이템의 유니크. 
	SI16				siItemPos;						// 삭제할 물품이 있다면,, 

	// PCK : 로그용 패킷 추가
	cltItem				clItem_Log;

	sDBResponse_InitStatus()
	{
		ZeroMemory(this, sizeof(sDBResponse_InitStatus));
	}
};

//-----------------------------------------
// 추천인 제도
//-----------------------------------------
class sDBRequest_RecommendPerson
{
public:
	sPacketHeader		packetHeader;
	
	TCHAR				strUserName[ MAX_PLAYER_NAME ];
	SI32				siIDNum;
	UI08				uiSlot;

	SI32				siPersonID;

	SI32				siCharID;

	sDBRequest_RecommendPerson( TCHAR *UserName, SI32 IDNum, UI08 slot,SI32 PersonID ,SI32 CharID )
	{
		packetHeader.usSize = sizeof( sDBRequest_RecommendPerson );
		packetHeader.usCmd	= DBMSG_REQUEST_RECOMMENDPERSON;

		MStrCpy( strUserName, UserName, MAX_PLAYER_NAME );

		siIDNum = IDNum ;
		uiSlot = slot ;

		siPersonID = PersonID;

		siCharID = CharID;
	}
};

class sDBResponse_RecommendPerson
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	TCHAR				strRecommenderUserName[ MAX_PLAYER_NAME ];
	TCHAR				strUserName[ MAX_PLAYER_NAME ];
	SI32				siIDNum;
	UI08				uiSlot;

	SI32				siPersonID;

	SI32				siCharID;
	SI32				siRecommendPersonID;
	
	sDBResponse_RecommendPerson()
	{
		ZeroMemory(this, sizeof(sDBResponse_RecommendPerson));
	}
};

class sDBRequest_HorseNameChange
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siHorseIndex;
	TCHAR				strHorseName[ MAX_HORSE_NAME ];

	sDBRequest_HorseNameChange( SI32 CharID, SI32 PersonID, SI16 HorseIndex, TCHAR *HorseName )
	{
		packetHeader.usSize = sizeof( sDBRequest_HorseNameChange );
		packetHeader.usCmd	= DBMSG_REQUEST_HORSENAMECHANGE;

		siCharID = CharID;
		siPersonID = PersonID;

		siHorseIndex = HorseIndex;
		MStrCpy( strHorseName, HorseName, MAX_HORSE_NAME );
		strHorseName[ MAX_HORSE_NAME -1 ] = '\0';
	}	
};

class sDBResponse_HorseNameChange
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	SI16				siHorseIndex;
	TCHAR				strHorseName[ MAX_HORSE_NAME ];

	sDBResponse_HorseNameChange()
	{
		memset( this, 0, sizeof( sDBResponse_HorseNameChange ) );
	}
};

//-----------------------------------------
// DB에 범죄 정보를 저장하기 위해서.
//-----------------------------------------
class sDBRequest_GuiltyInfoRecord
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;
	cltGuiltyInfo		clGuiltyInfo;

	sDBRequest_GuiltyInfoRecord( SI32 CharID, SI32 PersonID, cltGuiltyInfo *pclguiltyinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_GuiltyInfoRecord );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILTYINFORECORD;

		siCharID = CharID;
		siPersonID = PersonID;

		clGuiltyInfo.Set(pclguiltyinfo);
	}
	
};

class sDBResponse_GuiltyInfoRecord
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;

	// PCK : 로그용 패킷 추가
	cltGuiltyInfo		cIGuiltyInfo;

	sDBResponse_GuiltyInfoRecord()
	{
		memset( this, 0, sizeof( sDBResponse_GuiltyInfoRecord ) );
	}
};

//-----------------------------------------
// DB에 로그아웃 정보를 저장하기 위해서.
//-----------------------------------------
class sDBRequest_LogOutRecord
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siPersonID;						// 퀘스트 대상자. 
	TCHAR				szIP[64];
	UI08				uiNormalQuitSwitch;

	sDBRequest_LogOutRecord( SI32 PersonID, TCHAR* IP, bool NormalQuitSwitch )
	{
		packetHeader.usSize = sizeof( sDBRequest_LogOutRecord );
		packetHeader.usCmd	= DBMSG_REQUEST_LOGOUTRECORD;
		siPersonID = PersonID;
		memcpy( szIP, IP, 64 );
		szIP[ 63 ] = NULL;

		if ( NormalQuitSwitch )
		{
			uiNormalQuitSwitch = 1;
		}
		else
		{
			uiNormalQuitSwitch = 0;
		}
	}
};

//--------------------------------------------------
// DB에 이름 변경을 요청한다.
//-------------------------------------------------
class sDBRequest_ChangeName
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;		// 이름 변경할 대상자
	SI32				siCharID;		// 로그인시 캐릭터명이 변경될 경우필요한 캐릭터 id

	TCHAR				szName[MAX_PLAYER_NAME];

	SI16				siPos;
	cltItem				clitem;

	bool				m_bUseItem;	// [종호_NHN->NDOORS] 아이템을 이용해서 캐릭터명 변경하는 경우

	sDBRequest_ChangeName(SI32 personid,TCHAR * name,SI32 pos ,cltItem* pclItem, bool bUseItem)
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeName));

		packetHeader.usSize = sizeof( sDBRequest_ChangeName );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGENAME;

		siPersonID = personid ;

		MStrCpy( szName, name, MAX_PLAYER_NAME );

		siPos = pos ;
		clitem.Set(pclItem);

		m_bUseItem = bUseItem;
	}
};


#define CHANGENAME_REASON_SPECIALCHAR_CHANGENAME	1  //[종호] 캐릭터명이 특수문자 인경우 캐릭터명 변경

class sDBResponse_ChangeName
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siPersonID;		// 이름 변경할 대상자
	SI32				siCharID;		// 로그인시 캐릭터명이 변경될 경우필요한 캐릭터 id

	TCHAR				szName[MAX_PLAYER_NAME];
	TCHAR				szOldName[MAX_PLAYER_NAME];

	SI16				siPos;
	cltItem				clitem;

	// PCK - 로그용
	cltItem				clitem_Log;

	bool				m_bUseItem;	// [종호_NHN->NDOORS] 로그인시 캐릭터명 변경하는 경우

	sDBResponse_ChangeName()
	{
		memset( this, 0, sizeof( sDBResponse_ChangeName ) );
	}
};

class sDBRequest_Change_CharKind
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;

	SI16				siKind;

	SI16				siPos;
	cltItem				clitem;

	sDBRequest_Change_CharKind(SI32 personid,SI16 kind,SI32 pos ,cltItem* pclItem)
	{
		packetHeader.usSize = sizeof( sDBRequest_Change_CharKind );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGE_CHARKIND;

		siPersonID = personid ;

		siKind = kind ;

		siPos = pos ;
		clitem.Set(pclItem);
	}
};

class sDBResponse_Change_CharKind
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siPersonID;

	SI16				siKind;

	SI16				siPos;
	cltItem				clitem;

	// PCK - 로그용 패킷
	cltItem				clitem_Log;

	sDBResponse_Change_CharKind()
	{
		memset( this, 0, sizeof( sDBResponse_Change_CharKind ) );
	}
};

class sDBRequest_SetMinister
{
public:
	sPacketHeader		packetHeader;

	SI16				siMinister;
	
	SI32				siPersonID;

	TCHAR				szName[MAX_PLAYER_NAME];

	bool				bMaster;
	SI32				siSetMinisterTime;

	sDBRequest_SetMinister(SI16 minister,SI32 personid,TCHAR * name,bool master,SI32 time)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetMinister );
		packetHeader.usCmd	= DBMSG_REQUEST_SETMINISTER;

		siMinister = minister ;

		siPersonID = personid ;

		MStrCpy( szName, name, MAX_PLAYER_NAME );
		szName[MAX_PLAYER_NAME - 1] = '\0';

		bMaster = master ;
		siSetMinisterTime = time ;
	}
};

class sDBResponse_SetMinister
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI16				siMinister;

	SI32				siPersonID;

	SI32				siTempPersonId;
	SI32				siMinisterPersonId;

	sDBResponse_SetMinister()
	{
		memset( this, 0, sizeof( sDBResponse_SetMinister ) );
	}
};

// 백호(호조)가 퇴출 마을 설정
class sDBRequest_SetLiquidation
{
public:
	sPacketHeader		packetHeader;

	SI32	siPersonID;

	SI16	siLiquidationVillageNumber;
	SI32	siSetLiquidationVillageDate;

	sDBRequest_SetLiquidation( SI32 personid, SI16 LiquidationVillageNumber, SI32 SetLiquidationVillageDate )
	{
		packetHeader.usSize = sizeof( sDBRequest_SetLiquidation );
		packetHeader.usCmd	= DBMSG_REQUEST_SETLIQUIDATION;

		siPersonID = personid;
		siLiquidationVillageNumber = LiquidationVillageNumber;
		siSetLiquidationVillageDate = SetLiquidationVillageDate;
	}
};

class sDBResponse_SetLiquidation
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siPersonID;

	sDBResponse_SetLiquidation()
	{
		memset( this, 0, sizeof( sDBResponse_SetLiquidation ) );
	}
};

// 군주가 보너스 타임을 설정
class sDBRequest_Goonzu_SetBonusTime
{
public:
	sPacketHeader		packetHeader;

	SI32	siPersonID;

	SI16	siBonusTime;
	SI16	siMakeBonusTime;
	
	SI16    siChangedBonusTimeYear;
	SI16    siChangedBonusTimeMonth;
	SI16    siChangedBonusTimeDay;


	sDBRequest_Goonzu_SetBonusTime( SI32 personid, SI16 BonusTime , SI16 MakeBonusTime , SI16 ChangedBonusTimeYear , SI16 ChangedBonusTimeMonth, SI16 ChangedBonusTimeDay )
	{
		packetHeader.usSize = sizeof( sDBRequest_Goonzu_SetBonusTime );
		packetHeader.usCmd	= DBMSG_REQUEST_GOONZU_SETBONUSTIME;

		siPersonID = personid;
		
		siBonusTime = BonusTime;
		siMakeBonusTime = MakeBonusTime;

		siChangedBonusTimeYear = ChangedBonusTimeYear;
		siChangedBonusTimeMonth = ChangedBonusTimeMonth;
		siChangedBonusTimeDay = ChangedBonusTimeDay;
	}
};


class sDBResponse_Goonzu_SetBonusTime
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siPersonID;

	//[추가 : 황진성 2007. 11. 7 로그에 이용. 군주권한 보너스 타임(1시간)]
	SI16				siBonusTime;
	SI16				siMakeBonusTime;

	sDBResponse_Goonzu_SetBonusTime()
	{
		memset( this, 0, sizeof( sDBResponse_Goonzu_SetBonusTime ) );
	}
};

class sDBRequest_Prize_GameEvent
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	UI16				usCharID;

	UI08				bGameEvent;
	bool				bWinPrizeEvent;

	sDBRequest_Prize_GameEvent(SI32 personid,UI16 charid,UI08 gameevent , bool winprizeevent )
	{
		packetHeader.usSize = sizeof( sDBRequest_Prize_GameEvent );
		packetHeader.usCmd	= DBMSG_REQUEST_PRIZE_GAMEEVENT;

		siPersonID = personid ;
		usCharID = charid ;

		bGameEvent = gameevent ;
		bWinPrizeEvent = winprizeevent ;
	}
};

class sDBResponse_Prize_GameEvent
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siPersonID;
	UI16				usCharID;

	bool				bGameEvent;
	bool				bWinPrizeEvent;

	sDBResponse_Prize_GameEvent()
	{
		memset( this, 0, sizeof( sDBResponse_Prize_GameEvent ) );
	}
};

class sDBRequest_CountAttack_Info
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	UI16				usCharID;

	cltItem				clitem;
	SI16				siItemPos;

	sDBRequest_CountAttack_Info(SI32 personid,UI16 charid,cltItem *pclitem,SI16 pos)
	{
		packetHeader.usSize = sizeof( sDBRequest_CountAttack_Info );
		packetHeader.usCmd	= DBMSG_REQUEST_COUNTATTACK_INFO;

		siPersonID = personid ;
		usCharID = charid ;

		clitem.Set(pclitem);
		siItemPos = pos ;
	}

};

class sDBResponse_CountAttack_Info
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siPersonID;
	UI16				usCharID;

	cltItem				clitem;
	SI16				siItemPos;

	SI32				siFlagNum;
	SI32				siBeforeNum;

	sDBResponse_CountAttack_Info()
	{
		memset( this, 0, sizeof( sDBResponse_CountAttack_Info ) );
	}
};

// 인사말 설정 
class sDBRequest_SetGreeting
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;
		
	TCHAR				szText[ 128 ];

	sDBRequest_SetGreeting(SI32 charid, SI32 personid, TCHAR* pText)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetGreeting );
		packetHeader.usCmd	= DBMSG_REQUEST_SETGREETING;

		siCharID   = charid;
		siPersonID = personid;
		MStrCpy(szText, pText, 128);
	}
	
};

// 인사말 설정
class sDBResponse_SetGreeting
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
		
	TCHAR				szText[ 128 ];

	sDBResponse_SetGreeting()
	{
		ZeroMemory( this, sizeof( sDBResponse_SetGreeting ) );
	}
};

// 인사말 얻어오기
class sDBRequest_GetGreeting
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;
		
	SI32				siParam;

	TCHAR				szPersonName[ MAX_PLAYER_NAME ];

	sDBRequest_GetGreeting(SI32 charid, SI32 personid, SI32 paramid, TCHAR* personname)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetGreeting );
		packetHeader.usCmd	= DBMSG_REQUEST_GETGREETING;

		siCharID   = charid;
		siPersonID = personid;
		siParam    = paramid;
		MStrCpy(szPersonName, personname, MAX_PLAYER_NAME);
	}

};

// 인사말 얻어오기
class sDBResponse_GetGreeting
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
		
	SI32				siParam;

	TCHAR				szText[ 128 ];

	TCHAR				szPersonName[ MAX_PLAYER_NAME ];

	sDBResponse_GetGreeting()
	{
		ZeroMemory( this, sizeof( sDBResponse_GetGreeting ) );
	}
};

// 홈페이지 URL 얻어오기
class sDBRequest_Get_HomePageURL
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	sDBRequest_Get_HomePageURL(SI32 charid, SI32 personid, TCHAR* personname)
	{
		packetHeader.usSize = sizeof( sDBRequest_Get_HomePageURL );
		packetHeader.usCmd	= DBMSG_REQUEST_GET_HOMEPAGE_URL;

		siCharID   = charid;
		siPersonID = personid;
	}

};

// 홈페이지 URL 얻어오기
class sDBResponse_Get_HomePageURL
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
		
	TCHAR				szText[ 128 ];

	sDBResponse_Get_HomePageURL()
	{
		ZeroMemory( this, sizeof( sDBResponse_Get_HomePageURL ) );
	}
};

// 홈페이지 URL 설정하기
class sDBRequest_Set_HomePageURL
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;
		
	TCHAR				szText[ 128 ];

	sDBRequest_Set_HomePageURL(SI32 charid, SI32 personid, TCHAR* pText)
	{
		packetHeader.usSize = sizeof( sDBRequest_Set_HomePageURL );
		packetHeader.usCmd	= DBMSG_REQUEST_SET_HOMEPAGE_URL;

		siCharID   = charid;
		siPersonID = personid;
		MStrCpy( szText, pText, sizeof(szText) );
	}
	
};

// 홈페이지 URL 설정하기
class sDBResponse_Set_HomePageURL
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siCharID;
	SI32				siPersonID;
		
	TCHAR				szText[ 128 ];

	sDBResponse_Set_HomePageURL()
	{
		ZeroMemory( this, sizeof( sDBResponse_Set_HomePageURL ) );
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////
class sDBRequest_GetItemMakerNameNew
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siSerialID;

	SI32				siDupSerialID; // DB쪽에서 해당 시리얼을 못찾으면 0으로 리턴하기 때문에 원래 요청한 시리얼 번호를 여기서 갖고 있는다


	sDBRequest_GetItemMakerNameNew( SI32 charid, SI32 SerialID )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetItemMakerNameNew );
		packetHeader.usCmd	= DBMSG_REQUEST_ITEMMAKERNAMENEW;

		siCharID		= charid;
		siSerialID		= SerialID;
		siDupSerialID	= SerialID;
	}
};

class sDBResponse_GetItemMakerNameNew
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siSerialID;
	SI32				siDupSerialID;
	SI16				siServerUnique;

	TCHAR				szName[ MAX_PLAYER_NAME ];


	sDBResponse_GetItemMakerNameNew()
	{
		ZeroMemory( this, sizeof(sDBResponse_GetItemMakerNameNew));
	}

};

class sDBRequest_GetItemMakerName
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siCharID;
	SI32				siPersonID;


	sDBRequest_GetItemMakerName( SI32 charid, SI32 personid )
	{
		packetHeader.usSize = sizeof( sDBRequest_GetItemMakerName );
		packetHeader.usCmd	= DBMSG_REQUEST_ITEMMAKERNAME;

		siCharID   = charid;
		siPersonID = personid;
	}

};

class sDBResponse_GetItemMakerName
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;

	char				szName[ MAX_PLAYER_NAME ];


	sDBResponse_GetItemMakerName()
	{
		ZeroMemory( this, sizeof(sDBResponse_GetItemMakerName));
	}

};

enum
{
	RPGIVE_REASON_LEVELUP = 1,
	RPGIVE_REASON_ITEMMALL
};
class sDBRequest_ChangeRecommendPoint
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;						// 보낸사람. 	
	TCHAR				szCharName[MAX_PLAYER_NAME];	// 보낸사람이름.

	SI32				siRecommendPersonID;			// 받는사람.

	SI32				siLevel;
	SI32				siChangedRecommentPoint;		// 증감된 수치 (+: 증가, -:감소) 

	SI32				siReason;

	sDBRequest_ChangeRecommendPoint()
	{
		ZeroMemory(this, sizeof(sDBRequest_ChangeRecommendPoint));
	}

	sDBRequest_ChangeRecommendPoint( SI32 CharID, SI32 PersonID  , TCHAR * CharName, SI32 level , SI32 RecommendPersonID, SI32 ChangedRecommentPoint, SI32 Reason)
	{
		packetHeader.usSize = sizeof( sDBRequest_ChangeRecommendPoint );
		packetHeader.usCmd	= DBMSG_REQUEST_CHANGERECOMMENDPOINT;
		
        StringCchCopy(szCharName,MAX_PLAYER_NAME, CharName);
		siLevel				= level;
		siCharID			= CharID;
		siPersonID			= PersonID;		
		siChangedRecommentPoint	= ChangedRecommentPoint; 
		siRecommendPersonID	= RecommendPersonID;
		siReason			= Reason;
	}
};

class sDBResponse_ChangeRecommendPoint
{
public:
	sPacketHeader		packetHeader;
	
	SI32				siCharID;
	SI32				siPersonID;						// 보낸사람. 	

	SI32				siResult;					// 결과값.
	
	TCHAR				szCharName[MAX_PLAYER_NAME];// 보낸사람이름

	SI32				siRecommendPersonID;// 받는사람.

	SI32				siChangedRecommendPoint;	// 증감된 금액. (+: 증가, -:감소) 

	SI32				siLevel;
	SI32				siRecommendPoint;			// 최종 경험치 보유값. 

	// PCK - 로그용 패킷추가
	SI32				siReason;

	sDBResponse_ChangeRecommendPoint()
	{
		ZeroMemory(this, sizeof(sDBResponse_ChangeRecommendPoint));
	}
};


class sDBRequest_GetRecommendPoint
{
public:
	sPacketHeader		packetHeader;
	SI32				siCharID;
	SI32				siPersonID;						// 대상자. 	

	sDBRequest_GetRecommendPoint()
	{
		ZeroMemory(this, sizeof(sDBRequest_GetRecommendPoint));
	}

	sDBRequest_GetRecommendPoint(SI32 charid	,SI32 personid)
	{
		packetHeader.usSize = sizeof( sDBRequest_GetRecommendPoint );
		packetHeader.usCmd	= DBMSG_REQUEST_GETRECOMMENDPOINT;

		siCharID			= charid;
		siPersonID			= personid;
	}
};


class sDBResponse_GetRecommendPoint
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	
	SI32				siCharID;
	SI32				siPersonID;			// 대상자. 
	SI32				siRecommendPoint;			// 추천 포인트

	sDBResponse_GetRecommendPoint()
	{
		ZeroMemory(this, sizeof(sDBResponse_GetRecommendPoint));
	}
};

class sDBRequest_SetCountry
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCountryID;

	sDBRequest_SetCountry()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetCountry));
	}
	sDBRequest_SetCountry( SI32 personid, SI32 countryid )
	{
		packetHeader.usSize = sizeof(sDBRequest_SetCountry);
		packetHeader.usCmd	= DBMSG_REQUEST_SETCOUNTRY;

		siPersonID = personid;
		siCountryID = countryid;
	}
};

class sDBResponse_SetCountry
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siPersonID;
	SI32				siCountryID;

	sDBResponse_SetCountry()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetCountry));
	}
};

class sDBRequest_SetShowCountry
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	bool				bShowCountry;

	sDBRequest_SetShowCountry()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetShowCountry));
	}
	sDBRequest_SetShowCountry( SI32 personid, bool showcountry )
	{
		packetHeader.usSize = sizeof(sDBRequest_SetShowCountry);
		packetHeader.usCmd	= DBMSG_REQUEST_SETSHOWCOUNTRY;

		siPersonID = personid;
		bShowCountry = showcountry;
	}
};

class sDBResponse_SetShowCountry
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siPersonID;
	bool				bShowCountry;

	sDBResponse_SetShowCountry()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetShowCountry));
	}
};


class sDBRequest_SetShowPuchikomi
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	bool				bShowPuchikomi;

	sDBRequest_SetShowPuchikomi()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetShowPuchikomi));
	}
	sDBRequest_SetShowPuchikomi( SI32 personid, bool showpuchikomi )
	{
		packetHeader.usSize = sizeof(sDBRequest_SetShowPuchikomi);
		packetHeader.usCmd	= DBMSG_REQUEST_SETSHOWPUCHIKOMI;

		siPersonID = personid;
		bShowPuchikomi = showpuchikomi;
	}
};

class sDBResponse_SetShowPuchikomi
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siPersonID;
	bool				bShowPuchikomi;

	sDBResponse_SetShowPuchikomi()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetShowPuchikomi));
	}
};

enum
{
	RPGIVE_REASON_FROMGM = 1
};
class sDBRequest_GiveFP
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siRecommendPoint;

	SI16				siReason;

	sDBRequest_GiveFP( SI32 personid, SI32	RecommendPoint, SI16 Reason )
	{
		packetHeader.usSize = sizeof(sDBRequest_GiveFP);
		packetHeader.usCmd	= DBMSG_REQUEST_GIVEFP;

		siPersonID = personid;
		siRecommendPoint = RecommendPoint;
		siReason = Reason;
	}
};

class sDBResponse_GiveFP
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;
	SI32				siPersonID;

	SI32				siRecommendPoint;			// 최종 포인트 보유값. 

	// PCK - 로그용 패킷 추가
	SI16				siReason;

	sDBResponse_GiveFP()
	{
		ZeroMemory(this, sizeof(sDBResponse_GiveFP));
	}
};

class sDBRequest_AddBuf
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID; 

	SI16			siIndex;
	stBuf			m_stBuf;

	bool			bUseItem;
	SI16			siItemPos;		// 아이템을 사용할 위치.
	cltItem			clItem;			// Person이 사용할 아이템 정보. 

	GMONEY			money;

	sDBRequest_AddBuf( SI32 personid, SI16 index, stBuf*  pbuf, bool useitem = false, SI16 itempos = 0, cltItem* pclitem = NULL, GMONEY _money = 0)
	{
		packetHeader.usSize = sizeof(sDBRequest_AddBuf);
		packetHeader.usCmd	= DBMSG_REQUEST_ADDBUF;

		siPersonID = personid;
		siIndex = index;
		m_stBuf.siBufType = pbuf->siBufType;
		m_stBuf.siPercent = pbuf->siPercent;
		//m_stBuf.siAdd = pbuf->siAdd;
		m_stBuf.siOverTime = pbuf->siOverTime;
		//m_stBuf.bOverlap = pbuf->bOverlap;

		m_stBuf.siGetType = pbuf->siGetType;

		bUseItem = useitem;
		siItemPos = itempos;
		if(pclitem)
		{
			clItem.Set(pclitem);
		}
		else
		{
			clItem.Init();
		}

		money = _money;
	}

};

class sDBResponse_AddBuf
{
public:
	sPacketHeader	packetHeader;
	
	SI32			siResult;
	SI32			siPersonID; 

	SI16			siIndex;

	stBuf			m_stBuf;

	bool			bUseItem;
	SI16			siItemPos;		// 아이템을 사용할 위치.
	cltItem			clItem;			// Person이 사용할 아이템 정보. 
	cltItem			clUsedItem;			// 사용된 아이템. 

	GMONEY			money;

	sDBResponse_AddBuf()
	{
		ZeroMemory( this, sizeof(sDBResponse_AddBuf) );
	}

};

class sDBRequest_DeleteBuf
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	SI16			siIndex;

	stBuf			m_stBuf;		// 삭제되는 버프의 정보	- For LOG

	sDBRequest_DeleteBuf( SI32 personid, SI16 index, stBuf* pBuf )
	//sDBRequest_DeleteBuf( SI32 personid, SI16 index)
	{
		packetHeader.usSize = sizeof(sDBRequest_AddBuf);
		packetHeader.usCmd	= DBMSG_REQUEST_DELETEBUF;

		siPersonID = personid;
		siIndex = index;

		m_stBuf.Set(pBuf);
	}
};

class sDBResponse_DeleteBuf
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			siPersonID;
	SI16			siIndex;

	stBuf			m_stBuf;		// 삭제되는 버프의 정보	- For LOG

	sDBResponse_DeleteBuf()
	{
		ZeroMemory( this, sizeof(sDBResponse_DeleteBuf) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_SetTeacherPoint
//
// Last Modified 	: 2207 / 02 / 22
// Created		 	: 김광명
//
// Function			: [요청]스승점수를 변경한다.
//
//***************************************************************************************************
class sDBRequest_SetTeacherPoint
{
public:
	sPacketHeader	packetHeader;

	// 스승점수를 변경할 ID
	SI32			siTeacherPersonID;
	// 더해지거나 빼질 점수
	SI32			siAddPoint;

	sDBRequest_SetTeacherPoint( SI32 TeacherPersonID, SI32 AddPoint )
	{
		packetHeader.usSize = sizeof(sDBRequest_SetTeacherPoint);
		packetHeader.usCmd	= DBMSG_REQUEST_SETTEACHERPOINT;

		siTeacherPersonID = TeacherPersonID;
		siAddPoint = AddPoint;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_SetTeacherPoint
//
// Last Modified 	: 2207 / 02 / 22
// Created		 	: 김광명
//
// Function			: [응답]스승점수를 변경한다.(스승이 접속중이면 클라로 점수 변경해준다.)
//
//***************************************************************************************************
class sDBResponse_SetTeacherPoint
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			siTeacherPersonID;
	SI32			siTotalTeacherPoint;

	sDBResponse_SetTeacherPoint()
	{
		ZeroMemory( this, sizeof(sDBResponse_SetTeacherPoint) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_GetItem_From_TeacherPoint
//
// Last Modified 	: 2207 / 02 / 22
// Created		 	: 김광명
//
// Function			: [요청]스승점수로 아이템을 얻는다.
//
//***************************************************************************************************
class sDBRequest_GetItem_From_TeacherPoint
{
public:
	sPacketHeader	packetHeader;

	SI32			siCharID;
	// 스승점수를 변경할 ID
	SI32			siTeacherPersonID;
	// 더해지거나 빼질 점수
	SI32			siAddPoint;

	sDBRequest_GetItem_From_TeacherPoint( SI32 CharID, SI32 TeacherPersonID, SI32 AddPoint )
	{
		packetHeader.usSize = sizeof(sDBRequest_GetItem_From_TeacherPoint);
		packetHeader.usCmd	= DBMSG_REQUEST_GETITEM_FROM_TEACHERPOINT;

		siCharID = CharID;
		siTeacherPersonID = TeacherPersonID;
		siAddPoint = AddPoint;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_GetItem_From_TeacherPoint
//
// Last Modified 	: 2207 / 02 / 22
// Created		 	: 김광명
//
// Function			: [응답]스승점수로 아이템을 얻는다.
//
//***************************************************************************************************
class sDBResponse_GetItem_From_TeacherPoint
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			siCharID;
	SI32			siAddPoint;

	sDBResponse_GetItem_From_TeacherPoint()
	{
		ZeroMemory( this, sizeof(sDBResponse_GetItem_From_TeacherPoint) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_RenouncePupil
//
// Last Modified 	: 2007 / 02 / 22
// Created		 	: 
//
// Function			: [요청]스승이 제자와의 관계를 끊는다.
//
//***************************************************************************************************
class sDBRequest_RenouncePupil
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPupilPersonID;
	
	sDBRequest_RenouncePupil( SI32 CharID, SI32 PupilPersonID )
	{
		packetHeader.usSize = sizeof( sDBRequest_RenouncePupil );
		packetHeader.usCmd	= DBMSG_REQUEST_RENOUNCEPUPIL;

		siCharID		= CharID;
		siPupilPersonID	= PupilPersonID;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_RenouncePupil
//
// Last Modified 	: 2207 / 02 / 22
// Created		 	: 김광명
//
// Function			: [응답]스승이 제자와의 관계를 끊는다.
//
//***************************************************************************************************
class sDBResponse_RenouncePupil
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			siCharID;
	SI32			siPupilPersonID;

	sDBResponse_RenouncePupil()
	{
		ZeroMemory( this, sizeof(sDBResponse_RenouncePupil) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_OverTakeTeacherLevel
//
// Last Modified 	: 2207 / 02 / 27
// Created		 	: 김광명
//
// Function			: [요청]제자가 스승레벨을 따라잡았는가?
//
//***************************************************************************************************
class sDBRequest_OverTakeTeacherLevel
{
public:
	sPacketHeader	packetHeader;

	SI32			siCharID;
	// 현재 내 스승 PersonID
	SI32			siTeacherPersonID;
	// 레벨업 한 나의 레벨
	SI32			siMyLevel;
	// 제자 Person|D
	SI32			siPupilPersonID;

	sDBRequest_OverTakeTeacherLevel( SI32 CharID, SI32 TeacherPersonID, SI32 MyLevel, SI32 PupilPersonID )
	{
		packetHeader.usSize = sizeof(sDBRequest_OverTakeTeacherLevel);
		packetHeader.usCmd	= DBMSG_REQUEST_OVERTAKETEACHERLEVEL;

		siCharID = CharID;
		siTeacherPersonID = TeacherPersonID;
		siMyLevel = MyLevel;
		siPupilPersonID = PupilPersonID;
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_OverTakeTeacherLevel
//
// Last Modified 	: 2207 / 02 / 27
// Created		 	: 김광명
//
// Function			: [응답]제자가 스승레벨을 따라잡았는가?
//
//***************************************************************************************************
class sDBResponse_OverTakeTeacherLevel
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			siCharID;
	TCHAR			szTeacherName[MAX_PLAYER_NAME];

	sDBResponse_OverTakeTeacherLevel()
	{
		ZeroMemory( this, sizeof(sDBResponse_OverTakeTeacherLevel) );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBRequest_PVPResult
//
// Last Modified 	: 2007 / 06 / 25
// Created		 	: 김광명
//
// Function			: [요청]PVP결과를 DB로 보낸다.
//
//***************************************************************************************************
class sDBRequest_PVPResult
{
public:
	sPacketHeader	packetHeader;

	SI32			siWinCharID;
	SI32			siLoseCharID;

	SI32			siWinPersonID;
	SI32			siLosePersonID;

	TCHAR			szWinUserName[MAX_PLAYER_NAME];
	TCHAR			szLoseUserName[MAX_PLAYER_NAME];

	sDBRequest_PVPResult( SI32 WinCharID, SI32 LoseCharID, SI32 WinPersonID, SI32 LosePersonID,
		                  TCHAR* WinUserName, TCHAR* LoseUserName )
	{
		packetHeader.usSize = sizeof(sDBRequest_PVPResult);
		packetHeader.usCmd	= DBMSG_REQUEST_PVPRESULT;

		siWinCharID = WinCharID;
		siLoseCharID = LoseCharID;

		siWinPersonID = WinPersonID;
		siLosePersonID = LosePersonID;

		MStrCpy( szWinUserName, WinUserName, MAX_PLAYER_NAME );
		MStrCpy( szLoseUserName, LoseUserName, MAX_PLAYER_NAME );
	}
};

//***************************************************************************************************
//
// Class Name 		: sDBResponse_PVPResult
//
// Last Modified 	: 2007 / 06 / 25
// Created		 	: 김광명
//
// Function			: [응답]PVP결과를 받는다.
//
//***************************************************************************************************
class sDBResponse_PVPResult
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;

	SI32			siWinCharID;
	TCHAR			szPVPWinHighLevelUserName[MAX_PLAYER_NAME];

	SI32			siLoseCharID;

	TCHAR			szWinUserName[MAX_PLAYER_NAME];
	TCHAR			szLoseUserName[MAX_PLAYER_NAME];

	sDBResponse_PVPResult()
	{
		ZeroMemory( this, sizeof(sDBResponse_PVPResult) );
	}
};

//--------------------------------------------------------
// 보상엔피씨가 계정에 따라 받을수 있도록 수정
//--------------------------------------------------------
#define GIVEREWARD_ACCOUNT	1
#define GIVEREWARD_SERVER	2
#define GIVEREWARD_CHAR		3


class sDBRequest_SystemRewardList
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	UI16			usCharID;
	SYSTEMTIME		servertime;
	SI32			m_siCurPage;

	sDBRequest_SystemRewardList( SI32 PersonID, UI16 CharID, SYSTEMTIME ServerTime, SI32 siCurPage )
	{
		packetHeader.usSize = sizeof(sDBRequest_SystemRewardList);
		packetHeader.usCmd	= DBMSG_REQUEST_SYSTEMREWARDLIST;

		siPersonID = PersonID;
		usCharID = CharID;
		servertime = ServerTime;
		m_siCurPage = siCurPage;
	}		
};

class sDBResponse_SystemRewardList
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;

	SI32			siPersonID;
	UI16			usCharID;

	SI32			m_siCurPage;
	SI32			m_siMaxPage;

	cltSystemRewardInfo	clInfo[MAX_SYSTEMREWARD_LIST];


	sDBResponse_SystemRewardList()
	{
		ZeroMemory( this, sizeof(sDBResponse_SystemRewardList) );
	}
};

class sDBRequest_RecieveSystemReward
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	UI16			usCharID;

	SI32			siIndex;

	SI16			siItemPos;
	bool			bPileSwitch;	// 중첩 허용 여부. 	
	bool			bCreateSID;		// SID 생성 여부

	sDBRequest_RecieveSystemReward( SI32 PersonID, UI16 CharID, SI32 Index, SI32 ItemPos, bool PileSwitch, bool CreateSID )
	{
		packetHeader.usSize = sizeof(sDBRequest_RecieveSystemReward);
		packetHeader.usCmd	= DBMSG_REQUEST_RECIEVESYSTEMREWARD;

		siPersonID = PersonID;
		usCharID = CharID;
		siIndex = Index;
		siItemPos = ItemPos;
		bPileSwitch = PileSwitch;
		bCreateSID = CreateSID;
	}
};

class sDBResponse_RecieveSystemReward
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;

	SI32			siPersonID;
	UI16			usCharID;

	cltSystemRewardInfo	clInfo;

	SI16			siItemPos;			// 받은 아이템의 인벤토리내 위치
	cltItem			clItem;				// 받은 아이템 정보

	GMONEY			siMoney;			// 받은 후에 변화된 돈

	sDBResponse_RecieveSystemReward()
	{
		ZeroMemory( this, sizeof(sDBResponse_RecieveSystemReward) );
	}
};

class sDBRequest_GiveSystemReward
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	UI16			usCharID;
	SI32			siType;	// GIVEREWARD_ACCOUNT, GIVEREWARD_SERVER, GIVEREWARD_CHAR
	cltSystemRewardInfo	clInfo;


	sDBRequest_GiveSystemReward( SI32 RewardType, SI32 PersonID, UI16 CharID, cltSystemRewardInfo* pclinfo )
	{
		packetHeader.usSize = sizeof(sDBRequest_GiveSystemReward);
		packetHeader.usCmd	= DBMSG_REQUEST_GIVESYSTEMREWARD;

		siPersonID = PersonID;
		usCharID = CharID;
		siType = RewardType;

		clInfo.Set( pclinfo );
	}
};

class sDBResponse_GiveSystemReward
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;

	SI32			siPersonID;
	UI16			usCharID;

	cltSystemRewardInfo	clInfo;

	sDBResponse_GiveSystemReward()
	{
		ZeroMemory( this, sizeof(sDBResponse_GiveSystemReward) );
	}
};

// 출석 체크 하기
class sDBRequest_Attend
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	UI16			usCharID;

	NDate			clEventStartDay;
	NDate			clEventEndDay;

	SI08			siType;

	sDBRequest_Attend( SI32 PersonID, UI16 CharID, 
						NDate* pclStartDay, NDate* pclEndDay, SI08 siAttendType )
	{
		packetHeader.usSize = sizeof(sDBRequest_Attend);
		packetHeader.usCmd	= DBMSG_REQUEST_ATTEND;

		siPersonID = PersonID;
		usCharID = CharID;

		clEventStartDay.Set( pclStartDay );
		clEventEndDay.Set( pclEndDay );

		siType = siAttendType;
	}
};

class sDBResponse_Attend
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	UI16			usCharID;

	SI32			siResult;
	SI32			siAttendanceDay;		// 총 출석일수

	SI32			siRewardDay;			// 총 보상받은 일수

	SI08			siType;					// 이벤트 타입 1=출석, 2=PC방네잎클로버지급, 3=시즌2출석이벤트

	sDBResponse_Attend()
	{
		ZeroMemory( this, sizeof(sDBResponse_Attend) );
	}
};

// [영훈] 현재 날짜로 출석을 했다고 보낸다
class sDBRequest_SetAttend
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	UI16			uiCharID;

	NDate			kAttendDay;	// 출석하는 날짜
	
	SI08			siType;		// 출석 타입(ATTEND_EVENTTYPE 값중 하나 선택)

	sDBRequest_SetAttend( SI32 PersonID, UI16 CharID, NDate* pkAttendDay,  SI08 siAttendType )
	{
		packetHeader.usSize		= sizeof(sDBRequest_SetAttend);
		packetHeader.usCmd		= DBMSG_REQUEST_SETATTEND;

		siPersonID				= PersonID;
		uiCharID				= CharID;

		kAttendDay.Set( pkAttendDay );
		
		siType					= siAttendType;
	}
};

class sDBResponse_SetAttend
{
public:
	sPacketHeader	packetHeader;

	SI32			siPersonID;
	UI16			uiCharID;

	SI32			siResult;

	SI08			siType;					// 이벤트 타입 1=출석, 2=PC방네잎클로버지급, 3=시즌2출석이벤트

	sDBResponse_SetAttend()
	{
		ZeroMemory( this, sizeof(sDBResponse_SetAttend) );
	}
};
//KHY - 0906 -해당계정의 해당이벤트 타입에 따른 쿠폰을 발급여부를 요청한다.
/*
class sDBRequest_GetEventCouponInfo
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siEventType;

	sDBRequest_GetEventCouponInfo( SI32 personid, SI32 evettype)
	{
		packetHeader.usSize = sizeof(sDBRequest_GetEventCouponInfo );
		packetHeader.usCmd = DBMSG_REQUEST_GETEVENTCOUPONINFO;

		siPersonID = personid;
		siEventType = evettype;
	}
};
*/
class sDBResponse_GetEventCouponInfo
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siEventType;
//	SI32 siEventCount;
	SI32	siResult; // 0 = 실패. 1 = 성공.


	//NOW_COM_EVENT (6) 	:   siResult = 1  //신규 캐릭터 첫 접속. - 성공.
	//						    siResult = 2  //10레벨 달성 이벤트 클리어. - 성공.
	//						    siResult = 3  //스승이 생겼음. = 제자가 되었음. - 성공.
	// siPara 값을 그대로 전달해 주세요.

	sDBResponse_GetEventCouponInfo()
	{
		ZeroMemory( this, sizeof(sDBResponse_GetEventCouponInfo) );		
	}
};

class sDBResponse_GetAUCTIONEventCouponInfo
{
public:
	sPacketHeader packetHeader;

	SI32 siPersonID;
	SI32 siEventType;
	//	SI32 siEventCount;
	SI32	siResult; // 0 = 실패. 1 ~6까지 쿠폰종류.

	sDBResponse_GetAUCTIONEventCouponInfo()
	{
		ZeroMemory( this, sizeof(sDBResponse_GetAUCTIONEventCouponInfo) );		
	}
};
////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------
// class : sDBRequest_TimeMeterRate_AddTime
// desc  : 종량제 시스템의 사용시간을 충전한다.
//-----------------------------------------------------------
class sDBRequest_TimeMeterRate_AddTime
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI16				siTimeMeterRateType;
	SI32				siAddTime;

	cltItem				clItem;
	SI16				siItemPos;

	sDBRequest_TimeMeterRate_AddTime(UI16 charid, SI32 personid, SI16 timemeterratetype, SI32 addtime ,cltItem* pclItem, SI16 itempos)
	{
		packetHeader.usSize = sizeof( sDBRequest_TimeMeterRate_AddTime );
		packetHeader.usCmd	= DBMSG_REQUEST_TIMEMETERRATE_ADDTIME;

		uiCharID		= charid;
		siPersonID		= personid;

		siTimeMeterRateType = timemeterratetype;
		siAddTime			= addtime;

		if(pclItem)		clItem.Set(pclItem);
		siItemPos		= itempos;
	}
};

class sDBResponse_TimeMeterRate_AddTime
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI32				siResult;

	SI32				siLeftTime;

	cltItem				clLeftItem;		// 인벤토리 정보

	SI16				siTimeMeterRateType;	// For Log
	SI32				siAddTime;				// For Log

	cltItem				clItem; // For Log
	SI16				siItemPos; // For Log

	sDBResponse_TimeMeterRate_AddTime()
	{
		ZeroMemory(this, sizeof(sDBResponse_TimeMeterRate_AddTime));
	}
};

//-----------------------------------------------------------
// class : sDBRequest_TimeMeterRate_AddTime
// desc  : 종량제 시스템의 사용시간을 차감한다.
//-----------------------------------------------------------
class sDBRequest_TimeMeterRate_UseTime
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI16				siTimeMeterRateType;
	SI32				siUseTime;
	bool				bUpdate;		// DB정보를 서버에 적용할 것인가?

	sDBRequest_TimeMeterRate_UseTime(UI16 charid, SI32 personid, SI16 timemeterratetype, SI32 usetime, bool update )
	{
		packetHeader.usSize = sizeof( sDBRequest_TimeMeterRate_UseTime );
		packetHeader.usCmd	= DBMSG_REQUEST_TIMEMETERRATE_USETIME;

		uiCharID		= charid;
		siPersonID		= personid;

		siTimeMeterRateType = timemeterratetype;
		siUseTime			= usetime;
		bUpdate				= update;
	}
};

class sDBResponse_TimeMeterRate_UseTime
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI32				siResult;

	SI32				siLeftTime;

	SI16				siTimeMeterRateType;// For Log
	SI32				siUseTime;			// For Log
	bool				bUpdate;			// DB정보를 서버에 적용할 것인가?

	sDBResponse_TimeMeterRate_UseTime()
	{
		ZeroMemory(this, sizeof(sDBResponse_TimeMeterRate_UseTime));
	}
};

//-----------------------------------------------------------
// class : sDBRequest_Transform_AddChar
// desc  : 변신가능 캐릭터를 등록한다.
//-----------------------------------------------------------
class sDBRequest_Transform_AddChar
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI16				siCharKind;

	cltItem				clItem;
	SI16				siItemPos;

	SI16				siTimeMeterRateType; // 시간 충전 타입. - TIMEMETERRATE_TRANSFORM  로 고정이다.
	SI32				siAddTime; 			  // 3시간  - 10800초 고정이다.

	sDBRequest_Transform_AddChar(UI16 charid, SI32 personid, SI16 charkind, cltItem* pclItem, SI16 itempos,
											SI16	 TimeMeterRateType, SI32 AddTime)
	{
		packetHeader.usSize = sizeof( sDBRequest_Transform_AddChar );
		packetHeader.usCmd	= DBMSG_REQUEST_TRANSFORM_ADDCHAR;

		uiCharID		= charid;
		siPersonID		= personid;

		siCharKind		= charkind;

		if(pclItem)		clItem.Set(pclItem);
		siItemPos		= itempos;

		siTimeMeterRateType = TimeMeterRateType;
		siAddTime = AddTime;
	}
};


class sDBResponse_Transform_AddChar
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI32				siResult;

	cltItem				clLeftItem;		// 인벤토리 정보

	SI16				siCharKind;		
	cltItem				clItem;			// For Log
	SI16				siItemPos;		// For Log

	SI16				siTimeMeterRateType;
	SI32				siAddTime;
	
	sDBResponse_Transform_AddChar()
	{
		ZeroMemory(this, sizeof(sDBResponse_Transform_AddChar));
	}
};

//-----------------------------------------------------------
// class : sDBRequest_TimeMeterRate_AddTime
// desc  : 변신 캐릭터 사용시간을 증가시킨다.
//-----------------------------------------------------------
class sDBRequest_Transform_AddTime
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI16				siCharKind;
	SI32				siAddTime;

	sDBRequest_Transform_AddTime(UI16 charid, SI32 personid, SI16 charkind, SI32 addtime)
	{
		packetHeader.usSize = sizeof( sDBRequest_Transform_AddTime );
		packetHeader.usCmd	= DBMSG_REQUEST_TRANSFORM_ADDTIME;

		uiCharID		= charid;
		siPersonID		= personid;

		siCharKind		= charkind;
		siAddTime		= addtime;
	}
};

class sDBResponse_Transform_AddTime
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI32				siResult;

	SI16				siCharKind;
	SI32				siTotalTime;

	SI32				siAddTime;		// For Log

	sDBResponse_Transform_AddTime()
	{
		ZeroMemory(this, sizeof(sDBResponse_Transform_AddTime));
	}
};

//-----------------------------------------------------------
// class : sDBRequest_SetTransForm
// desc  : 변신 상태 등록
//-----------------------------------------------------------
//KHY - 1220 - 변신시스템.
class sDBRequest_TransForm_Set
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	UI16 	uiNowTransFormKind;	// 마지막(혹은 현재)변신중인 캐릭터의 종류.
	bool 	bTransFormMode;		// 변신 상태 여부. - True = 변신

	sDBRequest_TransForm_Set(UI16 charid, SI32 personid, UI16 NowTransFormKind, bool TransFormMode)
	{
		packetHeader.usSize = sizeof( sDBRequest_TransForm_Set );
		packetHeader.usCmd	= DBMSG_REQUEST_TRANSFORM_SET;

		uiCharID   = charid;
		siPersonID = personid;

		uiNowTransFormKind = NowTransFormKind;
		bTransFormMode = TransFormMode;
	}
};

class sDBResponse_TransForm_Set
{
public:
	sPacketHeader		packetHeader;

	UI16				uiCharID;
	SI32				siPersonID;

	SI32				siResult;

	sDBResponse_TransForm_Set()
	{
		ZeroMemory(this, sizeof(sDBResponse_TransForm_Set));
	}
};
// 케릭터 옵션
class sDBRequest_Char_Option
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;
	SI32				siPersonID;						// 초기화  대상자. 

	cltPCharOptionInfo		clPCharOptioninfo;

	sDBRequest_Char_Option()
	{
		ZeroMemory(this, sizeof(sDBRequest_Char_Option));
	}

	sDBRequest_Char_Option(UI16 charid, SI32 personid, cltPCharOptionInfo CharOptioninfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_Char_Option );
		packetHeader.usCmd	= DBMSG_REQUEST_CHAR_OPTION;
		usCharID = charid ;
		siPersonID = personid ;


		clPCharOptioninfo.Set(&CharOptioninfo);
	}
};

class sDBResponse_Char_Option
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;		// 결과값.
	UI16				usCharID;
	SI32				siPersonID;		// 대상자. 

	cltPCharOptionInfo		clPCharOptioninfo;

	sDBResponse_Char_Option()
	{
		ZeroMemory(this, sizeof(sDBResponse_Char_Option));
	}
};
// 게임 옵션 정보를 요청한다.
class sDBRequest_Char_Get_Option
{
public:
	sPacketHeader		packetHeader;

	SI32				siCharID;
	SI32				siPersonID;
	SI08				siLogin;		// 1: 로그인, 0: 기타


	sDBRequest_Char_Get_Option( SI32 CharID, SI32 PersonID, SI08 Login )
	{
		packetHeader.usSize = sizeof( sDBRequest_Char_Get_Option );
		packetHeader.usCmd	= DBMSG_REQUEST_CHAR_GET_OPTION;

		siCharID	= CharID;
		siPersonID	= PersonID;

		siLogin		= Login;
	}
};
// 게임 옵션 정보를 요정한다.
class sDBResponse_Char_Get_Option
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;		// 결과값.
	SI32				siCharID;		
	SI32				siPersonID;

	cltPCharOptionInfo		clPCharOptioninfo;

	sDBResponse_Char_Get_Option()
	{
		ZeroMemory( this,sizeof( sDBResponse_Char_Get_Option ) );
	}
};

class sDBRequest_Question_Daily_Count_Set
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;		
	SI32				m_siPersonID;			

	UI08				m_DailyCount;

	sDBRequest_Question_Daily_Count_Set( SI32 _siCharID, SI32 _siPersonID, UI08 _DailyCount )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Question_Daily_Count_Set );
		m_packetHeader.usCmd	= DBMSG_REQUEST_QUESTION_DAILY_COUNT_SET;
		
		m_siCharID		= _siCharID;	
		m_siPersonID	= _siPersonID;
		m_DailyCount	= _DailyCount;
	}
};

class sDBResponse_Question_Daily_Count_Set
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;		
	SI32				m_siPersonID;			

	UI08				m_DailyCount;

	sDBResponse_Question_Daily_Count_Set( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

class sDBRequest_Question_Daily_Count_Get
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;		
	SI32				m_siPersonID;			

	sDBRequest_Question_Daily_Count_Get(  SI32 _siCharID, SI32 _siPersonID )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Question_Daily_Count_Get );
		m_packetHeader.usCmd	= DBMSG_REQUEST_QUESTION_DAILY_COUNT_GET;

		m_siCharID		= _siCharID;	
		m_siPersonID	= _siPersonID;
	}
};

class sDBResponse_Question_Daily_Count_Get
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siCharID;		
	SI32				m_siPersonID;			

	UI08				m_DailyCount;

	sDBResponse_Question_Daily_Count_Get( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

//[진성] 
class sDBRequest_Question_Daily_Count_Del
{
public:
	sPacketHeader		m_packetHeader;
	
	sDBRequest_Question_Daily_Count_Del( void )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Question_Daily_Count_Del );
		m_packetHeader.usCmd	= DBMSG_REQUEST_QUESTION_DAILY_COUNT_DEL;
	}
};

//[진성] 도우미 정보 얻기. 
class sDBResponse_Question_Daily_Count_Del
{
public:
	sPacketHeader		m_packetHeader;
	
	sDBResponse_Question_Daily_Count_Del( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};
//--------------------------------------------------
// 발렌타인 데이 전투를 위해 가지고 있어야 하는 정보
//--------------------------------------------------
class sDBRequest_Valantine_Info_Get
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;

	sDBRequest_Valantine_Info_Get( )
	{
		ZeroMemory( this, sizeof(sDBRequest_Valantine_Info_Get) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Valantine_Info_Get );
		m_packetHeader.usCmd	= DBMSG_REQUEST_VALANTINE_INFO_GET;
	}

	sDBRequest_Valantine_Info_Get( SI32 siPersonID )
	{
		ZeroMemory( this, sizeof(sDBRequest_Valantine_Info_Get) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Valantine_Info_Get );
		m_packetHeader.usCmd	= DBMSG_REQUEST_VALANTINE_INFO_GET;

		m_siPersonID			= siPersonID;
	}
};

class sDBResponse_Valantine_Info_Get
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siPersonID;
	cltValentineWarInfo	m_clValantineWarInfo;

	sDBResponse_Valantine_Info_Get( )
	{
		ZeroMemory( this, sizeof(sDBResponse_Valantine_Info_Get) );
	}
};
//// 이사람
//enum ValentineWarStatus
//{
//	VALENTINEWAR_NONE	= 0,
//	VALENTINEWAR_COUPLE,
//	VALENTINEWAR_SOLO
//};
//--------------------------------------------------
// 발렌타인 데이 전투 솔로인지 커플인지 결정한것을 서버에 저장 한다.
//--------------------------------------------------
class sDBRequest_Valantine_Request
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;
	cltValentineWarInfo clValentineWarInfo;


	sDBRequest_Valantine_Request( )
	{
		ZeroMemory( this, sizeof(sDBRequest_Valantine_Request) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Valantine_Request );
		m_packetHeader.usCmd	= DBMSG_REQUEST_VALANTINE_REQUEST;
	}

	sDBRequest_Valantine_Request( SI32 siPersonID, cltValentineWarInfo ValentineWarinfo)
	{
		ZeroMemory( this, sizeof(sDBRequest_Valantine_Request) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Valantine_Request );
		m_packetHeader.usCmd	= DBMSG_REQUEST_VALANTINE_REQUEST;

		m_siPersonID			= siPersonID	;
		clValentineWarInfo	= ValentineWarinfo;
	}
};

class sDBResponse_Valantine_Request
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;
	SI32				m_siPersonID;
	cltValentineWarInfo clValentineWarInfo;


	sDBResponse_Valantine_Request( )
	{
		ZeroMemory( this, sizeof(sDBResponse_Valantine_Request) );
	}
};
// 전투로 인해 상품을 받았다.
class sDBRequest_Valantine_Get_Prize
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;
	bool			bGetPrize	;
	
	sDBRequest_Valantine_Get_Prize( )
	{
		ZeroMemory( this, sizeof(sDBRequest_Valantine_Get_Prize) );

		m_packetHeader.usSize	= sizeof( sDBRequest_Valantine_Get_Prize );
		m_packetHeader.usCmd	= DBMSG_REQUEST_VALANTINE_GET_PRIZE;
	}

	sDBRequest_Valantine_Get_Prize( SI32 siPersonID, bool Getprize)
	{
		ZeroMemory( this, sizeof(sDBRequest_Valantine_Get_Prize) );

		m_packetHeader.usSize	=	sizeof( sDBRequest_Valantine_Get_Prize );
		m_packetHeader.usCmd	=	DBMSG_REQUEST_VALANTINE_GET_PRIZE;
		m_siPersonID			=	siPersonID	;
		bGetPrize				=	Getprize	;
	}
};

class sDBResponse_Valantine_Get_Prize
{
public:
	sPacketHeader		m_packetHeader;

	SI32				m_siPersonID	;
	SI32				siResule		;
	bool				bGetPrize	;

	sDBResponse_Valantine_Get_Prize( )
	{
		ZeroMemory( this, sizeof(sDBResponse_Valantine_Get_Prize) );
	}
};


// [진성] 핸드폰 인증 확인.
class sDBRequest_Get_Cellular_Phone_Auth
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;
	SI32			m_siCharID;		

	sDBRequest_Get_Cellular_Phone_Auth( SI32 _siCharID, SI32 _siPersonID )
	{
		ZeroMemory( this, sizeof( sDBRequest_Get_Cellular_Phone_Auth ) );

		m_packetHeader.usSize = sizeof(sDBRequest_Get_Cellular_Phone_Auth);
		m_packetHeader.usCmd  = DBMSG_REQUEST_CELLULAR_PHONE_AUTH;
		
		m_siCharID			  = _siCharID;	
		m_siPersonID		  = _siPersonID;
	}
};

class sDBResponse_Get_Cellular_Phone_Auth
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siResult;

	SI32			m_siPersonID;
	SI32			m_siCharID;		
	
	bool			m_bIsCellularPhoneAuth;
	SI32			m_siRecommendPersonID;
	TCHAR			m_szRecommendPersonName[MAX_PLAYER_NAME];
	bool			m_bFirstCharCreate;
	
	sDBResponse_Get_Cellular_Phone_Auth()
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

// [영훈] 결혼을 한다
class sDBRequest_Set_Marry
{
public:
	sPacketHeader	m_packetHeader;

	// 청혼한사람
	SI32			m_siSourcePersonID;
	SI16			m_siSourceItemPos;
	cltItem			m_clSourceItem;
	// 청혼 받은 사람
	SI32			m_siDestPersonID;
	SI16			m_siDestItemPos;
	cltItem			m_clDestItem;
		
	// 애정도
	SI16			m_siLoveLevel;
	// 결혼일
	SI32			m_siWeddingDateVary;

	sDBRequest_Set_Marry( 
		SI32 siSourcePersonID,	cltItem*	pclSourceItem,		SI16 siSourceItemPos,
		SI32 siDestPersonID,	cltItem*	pclDestItem,		SI16 siDestItemPos,
		SI16 siLoveLevel,		SI32		siWeddingDateVary
	)
	{
		// 초기화
		ZeroMemory( this, sizeof(sDBRequest_Set_Marry) );

		// 패킷 설정
		m_packetHeader.usSize	= sizeof(sDBRequest_Set_Marry);
		m_packetHeader.usCmd	= DBMSG_REQUEST_SET_MARRY;

		// 실제 데이터 설정
		m_siSourcePersonID		= siSourcePersonID;
		if ( pclSourceItem )
		{
			m_siSourceItemPos	= siSourceItemPos;
			m_clSourceItem.Set( pclSourceItem );
		}

		m_siDestPersonID		= siDestPersonID;
		if ( pclDestItem )
		{
			m_siDestItemPos		= siDestItemPos;
			m_clDestItem.Set( pclDestItem );
		}
		m_siLoveLevel			= siLoveLevel;

		m_siWeddingDateVary		= siWeddingDateVary;	
	}
};

// [영훈] 결혼을 한다 응답
class sDBResponse_Set_Marry
{
public:
	sPacketHeader	m_packetHeader;

	// 결과
	SI32			m_siResult;

	// 청혼한사람
	SI32			m_siSourcePersonID;
	TCHAR			m_szSourceName[MAX_PLAYER_NAME];
	SI16			m_siSourceItemPos;
	cltItem			m_clSourceItem;

	// 청혼 받은 사람
	SI32			m_siDestPersonID;
	TCHAR			m_szDestName[MAX_PLAYER_NAME];
	SI16			m_siDestItemPos;
	cltItem			m_clDestItem;

	// 애정도
	SI16			m_siLoveLevel;

	// 결혼일
	SI32			m_siWeddingDateVary;	// (군주력)

	sDBResponse_Set_Marry( void )
	{
		ZeroMemory( this, sizeof(sDBResponse_Set_Marry) );
	}
};	// DBMSG_RESPONSE_SET_MARRY

// [영훈] 결혼 정보를 요청한다
class sDBRequest_Get_Marry_Info
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;	// 요청한 사람

	sDBRequest_Get_Marry_Info( SI32 siPersonID )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_GET_MARRY_INFO;
		m_packetHeader.usSize	= sizeof( sDBRequest_Get_Marry_Info );

		m_siPersonID			= siPersonID;
	}
};

class sDBResponse_Get_Marry_Info
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;			// 요청한 사람

	SI32			m_siMatePersonID;		// 배우자 PersonID
	TCHAR			m_szMateName[MAX_PLAYER_NAME];

	SI32			m_siWeddingDateVary;	// 결혼기념일(군주력)
	SI32			m_siItemGetDateVary;	// 결혼기념일 아이템 받은 날(군주력)

	SI32			m_siLimitDateVary;		// 애정도 아이템 끝나는 시간(군주력)
	SI32			m_siUseDateVary;		// 애정도 아이템 사용한 시간(군주력)
	SI16			m_siLoveLevel;			// 현재 애정도

	sDBResponse_Get_Marry_Info( void )
	{
		ZeroMemory( this, sizeof(sDBResponse_Get_Marry_Info) );
	}

};	// DBMSG_RESPONSE_GET_MARRY_INFO

// [영훈] 결혼기념일 아이템 받은 날 설정 요청
class sDBRequest_Marry_ItemGet_Set
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;			// 요청한 사람
	SI32			m_siItemGetDateVary;	// 아이템 받은 날(군주력)

	sDBRequest_Marry_ItemGet_Set( SI32 siPersonID, SI32 siItemGetDateVary )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_MARRY_ITEMGET_SET;
		m_packetHeader.usSize	= sizeof( sDBRequest_Marry_ItemGet_Set );

		m_siPersonID			= siPersonID;		
		m_siItemGetDateVary		= siItemGetDateVary;
	}
};

class sDBResponse_Marry_ItemGet_Set
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;			// 요청한 사람
	SI32			m_siItemGetDateVary;	// 아이템 받은 날(군주력)

	sDBResponse_Marry_ItemGet_Set( void )
	{
		ZeroMemory( this, sizeof(sDBResponse_Marry_ItemGet_Set) );
	}
};	// DBMSG_RESPONSE_MARRY_ITEMGET_SET

// [영훈] 결혼 애정도 관련 설정
class sDBRequest_Marry_Love_Set
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;		// 요청한 사람

	SI32			m_siUseHourVary;	// 사용한 시간
	SI32			m_siLimitHourVary;	// 제한시간

	SI16			m_siLoveLevel;		// 애정도

	sDBRequest_Marry_Love_Set( SI32 siPersonID, SI32 siUseHourVary, SI32 siLimitHourVary, SI16 siLoveLevel )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_MARRY_LOVE_SET;
		m_packetHeader.usSize	= sizeof(sDBRequest_Marry_Love_Set);

		m_siPersonID			= siPersonID;		

		m_siUseHourVary			= siUseHourVary;	
		m_siLimitHourVary		= siLimitHourVary;	

		m_siLoveLevel			= siLoveLevel;		
	}
};

class sDBResponse_Marry_Love_Set
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;		// 요청한 사람

	SI32			m_siUseHourVary;	// 사용한 시간
	SI32			m_siLimitHourVary;	// 제한시간

	SI16			m_siLoveLevel;		// 애정도

	sDBResponse_Marry_Love_Set( void )
	{
		ZeroMemory( this, sizeof(sDBResponse_Marry_Love_Set) );
	}
};
// [성웅] 가족 정보를 요청한다.
class sDBRequest_Get_FamilyInfo
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;	// 요청한 사람

	sDBRequest_Get_FamilyInfo( SI32 siPersonID )
	{
		m_packetHeader.usCmd	= DBMSG_REQUEST_GET_FAMILYINFO;
		m_packetHeader.usSize	= sizeof( sDBRequest_Get_FamilyInfo );

		m_siPersonID			= siPersonID;
	}
};

class sDBResponse_Get_FamilyInfo
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;			// 요청한 사람
	CFamily			m_cFamily	;			// 나의 가족 정보
	sDBResponse_Get_FamilyInfo( void )
	{
		ZeroMemory( this, sizeof(sDBResponse_Get_FamilyInfo) );
	}

};
// [성웅] 쿠키 사용
class sDBRequest_Set_My_CookeEffect
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID		;	// 요청한 사람
	SI32			m_siEndDateVary		;	// 쿠키 사용 종료 시간
	SI32			m_siUseLimitDateVary;	// 쿠키사용제한시간
	SI32			m_siCookieLevel		;	// 요청한 사람

	sDBRequest_Set_My_CookeEffect( SI32 siPersonID, SI32 siEndDateVary, SI32 siUseLimitDateVary,SI32 siCookieLevel )
	{

		m_packetHeader.usCmd	= DBMSG_REQUEST_FAMILY_SET_COOKIE_EFFECT;
		m_packetHeader.usSize	= sizeof( sDBRequest_Set_My_CookeEffect );

		m_siPersonID			=	siPersonID;
		m_siEndDateVary			=	siEndDateVary;
		m_siUseLimitDateVary	=	siUseLimitDateVary;
		m_siCookieLevel			=	siCookieLevel;
	}
};

class sDBResponse_Set_My_CookeEffect
{
public:
	sPacketHeader	m_packetHeader;

	SI32			m_siPersonID;			// 요청한 사람
	CFamily			m_cFamily	;			// 나의 가족 정보
	sDBResponse_Set_My_CookeEffect( void )
	{
		ZeroMemory( this, sizeof(sDBResponse_Set_My_CookeEffect) );
	}
};

//[영훈] 캐릭터 보관소에서 캐릭터를 가져온다
class sDBRequest_PersonSlotNumChange
{
public:
	sPacketHeader	m_packetHeader;
	UI16			m_uiCharID;

	SI08			m_siFromIndex;	// 여기에서(유효범위는 3~14)
	SI08			m_siToIndex;	// 여기로 이동(유효범위는 0~2)

	SI32			m_siPersonID;

	sDBRequest_PersonSlotNumChange( UI16 uiCharID, SI08 siFromIndex, SI08 siToIndex, SI32 siPersonID )
	{
		
		m_packetHeader.usCmd	= DBMSG_REQUEST_PERSONSLOTNUMCHANGE;
		m_packetHeader.usSize	= sizeof( sDBRequest_PersonSlotNumChange );

		m_uiCharID				= uiCharID;

		m_siFromIndex			= siFromIndex;
		m_siToIndex				= siToIndex;

		m_siPersonID			= siPersonID;
	}
};

class sDBResponse_PersonSlotNumChange
{
public:
	sPacketHeader	m_packetHeader;
	UI16			m_uiCharID;

	SI32			m_siResult;

	sDBResponse_PersonSlotNumChange( void )
	{
		ZeroMemory( this, sizeof(sDBResponse_PersonSlotNumChange) );
	}
};


//[진성] 스킬북 마법 추가.
class sDBRequest_SkillBook_MagicInfo_Get
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;	

	sDBRequest_SkillBook_MagicInfo_Get( UI16 _usCharID, SI32 _siPersonID )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_SKILLBOOK_MAGICINFO_GET;
		m_packetHeader.usSize	= sizeof(sDBRequest_SkillBook_MagicInfo_Get);

		m_siPersonID			= _siPersonID;
		m_usCharID				= _usCharID;
	}

	~sDBRequest_SkillBook_MagicInfo_Get()
	{

	}
};

//[진성] 스킬북 마법 추가.
class sDBResponse_SkillBook_MagicInfo_Get
{
public:
	sPacketHeader			m_packetHeader;
	SI32					m_siResult;			// 결과값.
	UI16					m_usCharID;
	SI32					m_siPersonID;	

	SI08					m_iIndex[SKILLBOOK_MAGIC_NUM];
	cltSkillBookMagic		m_clAddSkillBookMagic[SKILLBOOK_MAGIC_NUM];
	cltSkillBookSubMagic	m_clAddSkillBookSubMagic[MAX_SKILLBOOK_SUBMAGIC_NUM];

	sDBResponse_SkillBook_MagicInfo_Get( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};


//[진성] 스킬북 마법 추가.
class sDBRequest_SkillBook_Magic_Add
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;				

	SI08				m_iIndex;
	cltSkillBookMagic	m_clAddSkillBookMagic;

	sDBRequest_SkillBook_Magic_Add( UI16 _usCharID, SI32 _siPersonID, SI08 _iIndex, cltSkillBookMagic* _pclAddSkillBookMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_SKILLBOOK_MAGIC_ADD;
		m_packetHeader.usSize	= sizeof(sDBRequest_SkillBook_Magic_Add);

		m_siPersonID			= _siPersonID;
		m_usCharID				= _usCharID;

		m_iIndex				= _iIndex;
		memcpy( &m_clAddSkillBookMagic, _pclAddSkillBookMagic, sizeof(m_clAddSkillBookMagic) );
	}
};

class sDBResponse_SkillBook_Magic_Add
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;			// 결과값.

	UI16				m_usCharID;
	SI32				m_siPersonID;				

	SI08				m_iIndex;
	cltSkillBookMagic	m_clAddSkillBookMagic;

	sDBResponse_SkillBook_Magic_Add( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

//[진성] 스킬북 마법.
class sDBRequest_SkillBook_Magic_Del
{
public:
	sPacketHeader			m_packetHeader;
	UI16					m_usCharID;
	SI32					m_siPersonID;				
	
	SI08					m_iIndex;
	cltSkillBookMagic		m_clAddSkillBookMagic;

	sDBRequest_SkillBook_Magic_Del( UI16 _usCharID, SI32 _siPersonID, SI08 _iIndex, cltSkillBookMagic* _pclAddSkillBookMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_SKILLBOOK_MAGIC_DEL;
		m_packetHeader.usSize	= sizeof(sDBRequest_SkillBook_Magic_Del);

		m_siPersonID			= _siPersonID;
		m_usCharID				= _usCharID;

		m_iIndex				= _iIndex;
		m_clAddSkillBookMagic.Set( _pclAddSkillBookMagic );
	}
};

class sDBResponse_SkillBook_Magic_Del
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;			// 결과값.

	UI16				m_usCharID;
	SI32				m_siPersonID;				

	SI08				m_iIndex;
	cltSkillBookMagic	m_clAddSkillBookMagic;

	sDBResponse_SkillBook_Magic_Del( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

//[진성] 스킬북 보조마법 추가.
class sDBRequest_SkillBook_SubMagic_Add
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;				

	cltSkillBookSubMagic	m_clAddSkillBookSubMagic;

	sDBRequest_SkillBook_SubMagic_Add( UI16 _usCharID, SI32 _siPersonID, cltSkillBookSubMagic* _pclAddSkillBookSubMagic )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_SKILLBOOK_SUBMAGIC_ADD;
		m_packetHeader.usSize	= sizeof(sDBRequest_SkillBook_SubMagic_Add);

		m_siPersonID			= _siPersonID;
		m_usCharID				= _usCharID;

		memcpy( &m_clAddSkillBookSubMagic, _pclAddSkillBookSubMagic, sizeof(m_clAddSkillBookSubMagic) );
	}
};

class sDBResponse_SkillBook_SubMagic_Add
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;			// 결과값.

	UI16				m_usCharID;
	SI32				m_siPersonID;				

	cltSkillBookSubMagic	m_clAddSkillBookSubMagic;

	sDBResponse_SkillBook_SubMagic_Add( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

//[진성] 스킬북 보조마법 추가.
class sDBRequest_SkillBook_SubMagic_Del
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;				

	cltSkillBookSubMagic	m_clAddSkillBookSubMagic;
	bool					m_bTimeOver;

	sDBRequest_SkillBook_SubMagic_Del( UI16 _usCharID, SI32 _siPersonID, cltSkillBookSubMagic* _pclAddSkillBookSubMagic, bool _bTimeOver )
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_SKILLBOOK_SUBMAGIC_DEL;
		m_packetHeader.usSize	= sizeof(sDBRequest_SkillBook_SubMagic_Del);

		m_siPersonID			= _siPersonID;
		m_usCharID				= _usCharID;

		memcpy( &m_clAddSkillBookSubMagic, _pclAddSkillBookSubMagic, sizeof(m_clAddSkillBookSubMagic) );
		m_bTimeOver = _bTimeOver;
	}
};

class sDBResponse_SkillBook_SubMagic_Del
{
public:
	sPacketHeader		m_packetHeader;
	SI32				m_siResult;			// 결과값.

	UI16				m_usCharID;
	SI32				m_siPersonID;				

	cltSkillBookSubMagic	m_clAddSkillBookSubMagic;
	bool					m_bTimeOver;

	sDBResponse_SkillBook_SubMagic_Del( void )
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

// (REQUEST) [지연] 군주데이미션 : 군주데이 미션을 받았을때 디비에 저장하길 요청
class sDBRequest_GoonzuDayMissionInfo_Set
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	SYSTEMTIME			m_MissionGetTime;

	//UI16 uiYear;			
	//UI08 uiMonth;			
	//UI08 uiDate;			
	SI16 siCount;			// 미션 카운트

	sDBRequest_GoonzuDayMissionInfo_Set(UI16 _usCharID, SI32 _siPersonID, UI16 _uiYear, UI08 _uiMonth, UI08 _uiDate, SI16 _siCount)
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_GOONZUDAYMISSIONINFO_SET;
		m_packetHeader.usSize	= sizeof(sDBRequest_GoonzuDayMissionInfo_Set);

		m_siPersonID			= _siPersonID;
		m_usCharID				= _usCharID;

		//
		m_MissionGetTime.wYear = _uiYear;
		m_MissionGetTime.wMonth = _uiMonth;
		m_MissionGetTime.wDay = _uiDate;
		siCount = _siCount;
	}
};


class sDBResponse_GoonzuDayMissionInfo_Set
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	SYSTEMTIME			m_MissionGetTime;
		
	SI16 siCount;			// 미션 카운트

	sDBResponse_GoonzuDayMissionInfo_Set()
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

// (REQUEST) [지연] 군주데이미션 : 군주데이 미션을 받은적이 있는지 디비에 물어본다
class sDBRequest_GoonzuDayMissionInfo_Get
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	sDBRequest_GoonzuDayMissionInfo_Get(UI16 _usCharID, SI32 _siPersonID)
	{
		ZeroMemory( this, sizeof(*this) );

		m_packetHeader.usCmd	= DBMSG_REQUEST_GOONZUDAYMISSIONINFO_GET;
		m_packetHeader.usSize	= sizeof(sDBRequest_GoonzuDayMissionInfo_Get);

		m_siPersonID			= _siPersonID;
		m_usCharID				= _usCharID;
	}
};

// (RESPONSE) [지연] 군주데이미션 : 군주데이 미션을 받은적이 있는지에 대한 디비의 응답
class sDBResponse_GoonzuDayMissionInfo_Get
{
public:
	sPacketHeader		m_packetHeader;
	UI16				m_usCharID;
	SI32				m_siPersonID;

	SYSTEMTIME			m_MissionGetTime;

	//UI16 uiYear;			
	//UI08 uiMonth;			
	//UI08 uiDate;			
	SI16 siCount;			// 미션 카운트

	sDBResponse_GoonzuDayMissionInfo_Get()
	{
		ZeroMemory( this, sizeof(*this) );
	}
};

#endif