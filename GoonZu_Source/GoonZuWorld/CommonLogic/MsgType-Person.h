#ifndef _MSGTYPEPERSON_H
#define _MSGTYPEPERSON_H


//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "../common/Family/Family.h"
#include "../common/Family/Marriage.h"
#include "../../Client/InterfaceMgr/Interface//ChatEdit.h"

//-----------------------------
// ImportantParameta정보를 보낸다. 
//-------------------------------
class cltGameMsgResponse_IP{
public:
	SI16 siCharUnique;		// 정보의 주인공. 
	cltPIPInfo clIP;		// 메인 정보. 

	cltGameMsgResponse_IP(SI16 charunique, cltPIPInfo* pip)
	{
		siCharUnique	= charunique;
		clIP.Set(pip);
	}

};

//----------------------------
// 캐릭터 삭제 요청
//-----------------------------
class cltGameMsgRequest_DelUserChar{
public:
	SI08 siCharSlot;


	TCHAR szIdentification[12];

	//SI32 siIdentification;

	cltGameMsgRequest_DelUserChar(SI08 charslot, TCHAR* identification)
	{
		siCharSlot		 = charslot;
		StringCchCopy( szIdentification, 12, identification );
		//		siIdentification = identification;
	}
};

class cltGameMsgResponse_DelUserChar{
public:
	SI08 siResult;			// 성공 실패 
	SI08 siReason;			// 실패시 실패 이유 
	SI08 siCharSlot;		// Delete Slot
	

	cltGameMsgResponse_DelUserChar(SI08 result, SI08 reason, SI08 charslot)
	{
		siResult		= result;
		siReason		= reason;
		siCharSlot		= charslot;
	}
};
//----------------------------
// 캐릭터 삭제 유예기간. 요청
//-----------------------------
class cltGameMsgRequest_PreDelUserChar{
public:
	SI08 siCharSlot;
	TCHAR szIdentification[12];
	SI16 predeltype;//1이면 삭제유예기간적용 0이면 삭제유예기간 삭제.
	//SI32 siIdentification;

	cltGameMsgRequest_PreDelUserChar(SI08 charslot, TCHAR* identification, SI16 deltype)
	{
		siCharSlot		 = charslot;
		StringCchCopy( szIdentification, 12, identification );
		predeltype = deltype;
		//		siIdentification = identification;
	}
};

class cltGameMsgResponse_PreDelUserChar{
public:
	SI08 siResult;			// 성공 실패 
	SI08 siReason;			// 실패시 실패 이유 
	SI08 siCharSlot;		// Delete Slot


	cltGameMsgResponse_PreDelUserChar(SI08 result, SI08 reason, SI08 charslot)
	{
		siResult		= result;
		siReason		= reason;
		siCharSlot		= charslot;
	}
};

//-----------------------------
// 제자 명단 업데이트 요청 
//-----------------------------
class cltGameMsgRequest_UpdateChildList{
public:
	SI16 siCharUnique;

	cltGameMsgRequest_UpdateChildList(SI16 charunique)
	{
		siCharUnique = charunique;
	}
};

class cltGameMsgResponse_UpdateChildList{
public:
	SI16	siCharUnique;

#ifdef _SAFE_MEMORY
	NSafeTArray<cltChildInfo, MAX_CHILDINFO_NUMBER>		clChildInfo;
#else
	cltChildInfo		clChildInfo[MAX_CHILDINFO_NUMBER];
#endif

	cltGameMsgResponse_UpdateChildList(SI16 charunique, cltChildInfo*		pclChildInfo)
	{
		siCharUnique = charunique;

		for(SI32 i = 0;i < MAX_CHILDINFO_NUMBER;i++)
		{
			clChildInfo[i].Set(&pclChildInfo[i]);
		}
	}
};

//-----------------------------
// 스승 등록 신청자 명단을 통보한다. 
//-----------------------------
class cltGameMsgRequest_FatherApplyerList{
public:
	SI16 siCharUnique;

	cltGameMsgRequest_FatherApplyerList(SI16 sicharunique)
	{
		siCharUnique = sicharunique;
	}
};

class cltGameMsgResponse_FatherApplyerList{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltFatherApplyerInfo, MAX_FATHER_APPLYER_LIST_NUMBER>	clApplyerList;
#else
	cltFatherApplyerInfo clApplyerList[MAX_FATHER_APPLYER_LIST_NUMBER];
#endif

	cltGameMsgResponse_FatherApplyerList(cltFatherApplyerInfo* pclApplyerList)
	{
		SI32 i;

		for(i = 0;i < MAX_FATHER_APPLYER_LIST_NUMBER;i++)
		{
			clApplyerList[i].Set(&pclApplyerList[i]);
		}
	}
};

//-----------------------------
// 스승 등록 신청한다. 
//-----------------------------
class cltGameMsgRequest_ApplyFather{
public:
	SI16 siCharUnique;

	cltGameMsgRequest_ApplyFather(SI16 sicharunique)
	{
		siCharUnique = sicharunique;
	}
};

//-----------------------------
// 스승 정보 통보 
//-----------------------------
class cltGameMsgResponse_ChildInfo{
public:

	SI16	siCharUnique;

	SI16	siChildNumber;				// 양성한 제자 수.

	cltGameMsgResponse_ChildInfo(SI16 charunique, SI16 childnumber)
	{
		siCharUnique	= charunique;
		siChildNumber	= childnumber;
	}
};

//-----------------------------
// 개인 정보 요청 
//-----------------------------
class cltGameMsgRequest_PersonPrivateInfo{
public:
	SI16 siCharUnique;

	cltGameMsgRequest_PersonPrivateInfo(SI16 sicharunique)
	{
		siCharUnique = sicharunique;
	}
};

class cltGameMsgRequest_PersonPrivateInfoByPersonID
{
public:
	SI32 siPersonID;
	cltGameMsgRequest_PersonPrivateInfoByPersonID( SI32 PersonID )
	{
		siPersonID = PersonID;
	}
};

class cltGameMsgResponse_PersonPrivateInfo{
public:

	SI16	siCharUnique;

	TCHAR			szAccountID[MAX_PLAYER_NAME];
	cltSimplePerson clPerson;
	cltSimplePerson clFather;
	//SI16			siSchoolUnique;
	SI16			siLevel;				// 레벨 
	SI16			siFameLevel;			// 명성등급. 
	SI32			siCountry;				// 국가
	bool			bShowCountry;
	SI16			siChangeFatherNumber;	// 스승을 교체한 회수. 
	SI32			siWealthRank;			// 자산 순위
	cltMoney		clMoney;				// 총 전장 금액

	TCHAR			szGreeting[128];

	SI32			siPVPWinNumber;
	SI32			siPVPLoseNumber;
	TCHAR			szPVPWinHighLevelUserName[MAX_PLAYER_NAME];
	
	// pck : 사용자 상세정보창 수정 (08.11.05)
	SI32			siNameLevel;						// 명칭 레벨
	TCHAR			szGuildName[MAX_GUILD_NAME];		// 가입 길드 이름
	SI32			siVillageUnique;					// 가입 마을 유니크

	cltGameMsgResponse_PersonPrivateInfo(TCHAR* paccountid, cltSimplePerson* pclperson,
										 cltSimplePerson* pclfather, /*SI16 sischoolunique,*/
										 SI16 level, SI16 famelevel, SI32 country, bool showcountry,
										 SI16 changefathernumber,SI32 wealthrank,cltMoney money, 
										 TCHAR* pgreeting, SI32 PVPWinNumber, SI32 PVPLoseNumber, TCHAR* PVPWinHighLevelUserName,
										 SI32 NameLevel, TCHAR* GuildName, SI32 VillageUnique)
	{
		MStrCpy( szAccountID, paccountid, MAX_PLAYER_NAME );

		clPerson.Set(pclperson);
		clFather.Set(pclfather);
		
		siLevel					= level;
		siFameLevel				= famelevel;
		siCountry				= country;
		bShowCountry			= showcountry;
		siChangeFatherNumber	= changefathernumber;
		siWealthRank			= wealthrank;
		clMoney					= money ;

		MStrCpy(szGreeting, pgreeting, 128);

		siPVPWinNumber = PVPWinNumber;
		siPVPLoseNumber = PVPLoseNumber;
		MStrCpy( szPVPWinHighLevelUserName, PVPWinHighLevelUserName, MAX_PLAYER_NAME );

		siNameLevel = NameLevel;
		MStrCpy( szGuildName, GuildName, MAX_GUILD_NAME);
		siVillageUnique = VillageUnique;
	}
};

//-----------------------------
// 접속중인 학교 사용자 수.
//-----------------------------
class cltGameMsgRequest_SchoolUserNum{
public:
	SI32	siSchoolUnique;

	cltGameMsgRequest_SchoolUserNum(SI32 unique)
	{
		siSchoolUnique = unique;
	}
};

class cltGameMsgResponse_SchoolUserNum{
public:
	SI32	siSchoolUnique;
	SI32	siUserNum;

	cltGameMsgResponse_SchoolUserNum(SI32 unique, SI32 usernum)
	{
		siSchoolUnique	= unique;
		siUserNum		= usernum;
	}
};


//-----------------------------
// 스승등록 
//-----------------------------
class cltGameMsgRequest_SetFather{
public:
	SI32 siPersonID;
	UI08 uiType;		// 0 : 일본 스승신청 1: 글로벌, 중국
	UI08 uiMode;		// 0 : 제자가 신청 1: 스승이 수락 2: 스승이 거부

	cltGameMsgRequest_SetFather(SI32 personid, UI08 Type = 0, UI08 Mode = 0)
	{
		siPersonID = personid;
		uiType = Type;
		uiMode = Mode;
	}
};

class cltGameMsgResponse_SetFather{
public:
	cltSimplePerson clFather;
	SI16			siMyLevel;
	TCHAR			szAccountID[MAX_PLAYER_NAME];

	cltGameMsgResponse_SetFather(cltSimplePerson* pclperson, SI16 mylevel,TCHAR *accountID)
	{
		clFather.Set(pclperson);
		siMyLevel	= mylevel;
		MStrCpy( szAccountID, accountID, MAX_PLAYER_NAME );
	}
};

class cltGameMsgResponse_Request_SetFather
{
public:
	SI32 siPupilPersonID;
	TCHAR szPupilName[MAX_PLAYER_NAME];

	cltGameMsgResponse_Request_SetFather( SI32 PupilPersonID, TCHAR* PupilName )
	{
		siPupilPersonID = PupilPersonID;
		MStrCpy( szPupilName, PupilName, MAX_PLAYER_NAME );
	}
};

//-----------------------------
// 학교등록 
//-----------------------------
class cltGameMsgRequest_SetSchool{
public:
	SI16 siSchoolUnique;

	cltGameMsgRequest_SetSchool(SI16 unique)
	{
		siSchoolUnique = unique;
	}
};

class cltGameMsgResponse_SetSchool{
public:
	SI16 siSchoolUnique;

	cltGameMsgResponse_SetSchool(SI16 unique)
	{
		siSchoolUnique = unique;
	}
};


//-----------------------------
// 변수 설정. 
//-----------------------------
#define VARYSET_NOFIRSTATTACK		1		// 선공 방지 스위치. 
class cltGameMsgResponse_VarySet{
public:
	SI16 siCharUnique;		//대상자 
	SI16 siVaryType;		//변수종류. 
	bool bVary;				//변수 상태. 

	cltGameMsgResponse_VarySet(SI16 sicharunique, SI16 varytype, bool bvary)
	{
		siCharUnique	= sicharunique;
		siVaryType		= varytype;
		bVary			= bvary;
	}
};

//-----------------------------
// 지휘효과 통보. 
//-----------------------------
class cltGameMsgResponse_GeneralEft{
public:
	SI16 siCharUnique;		//대상자 
	SI16 siDefRate;			//방어력 향상률. 
	SI16 siDamageRate;		//데미지 향상률. 

	cltGameMsgResponse_GeneralEft(SI16 sicharunique, SI16 defrate, SI16 damagerate)
	{
		siCharUnique	= sicharunique;
		siDefRate		= defrate;
		siDamageRate	= damagerate;
	}
};

//-----------------------------
// 경험치 획득 통보 
//-----------------------------
#define GETEXP_REASON_VILLAGEWAR1	1		// 상위 마을 점령. 
#define GETEXP_REASON_VILLAGEWAR2	2		// 하위 마을 점령. 
#define GETEXP_REASON_MAKEITEM		4		// 물품 제조로 경험치 획득. 
#define GETEXP_REASON_FAMEFATHER	8		// 제자 양성 성공으로 명성 경험치 획득. 
#define GETEXP_REASON_RECOMMEND		32		// 추천인으로 명성 경험치 획득. 
#define GETEXP_REASON_DISMANTLE		64		// 물품 해체로 경험치 획득. 
#define GETEXP_REASON_FAMEMASGER	128		// 운영자 명령으로 명성 경험치 획득.
#define GETEXP_REASON_MEDAL			256		// 명성훈장을 통해 명성경험치 획득. 
#define GETEXP_REASON_VILLAGEWARKILLSCORE	512	//공성전 전적 점수로 경험히 획득.
#define GETEXP_REASON_EVENT	1024			// 이벤트를 통한 경험치 획득
class cltGameMsgResponse_GetExp{
public:
	SI16 siCharUnique;	// 경험치 획득자. 
	SI16 siReason;		// 경험치 획득 사유. 
	SI32 siExp;			// 획득한 경험치. 
	SI32 siFameExp;		// 획득한 명성 경험치. 

	cltGameMsgResponse_GetExp(SI16 sicharunique, SI32 siexp, SI32 fameexp, SI16 sireason)
	{
		siCharUnique	= sicharunique;
		siExp			= siexp;
		siReason		= sireason;
		siFameExp		= fameexp;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_Hit
//
// Last Modified 	: 2007 / 02 / 14
// Created		 	: 김광명
//
// Function			: 타격 정보를 클라이언트로 보낸다.
//
//***************************************************************************************************
class cltGameMsgResponse_Hit
{
public:
	SI16 CharUnique;		// 피격된 캐릭터의 CharUnique
	SI32 Damage;			// 피격된 생명력
	SI32 DoubleDamage;		// 추가타로 피격된 생명력
	SI32 Life;				// 남은 생명력. 	

	SI32 Mode;				// 무엇에 맞았느냐?

	UI08 SpecialAttackSwitch;	// 필살기로 작동 여부. (0 : 일반 공격, 1 : 크리티컬, 2 : 추가타 (수박버프))

	bool bIndure;			// 인듀어 작동 여부

	bool bMissHit;
	bool bDodgeHit;
	UI08 uiType;			// 0 : 몬스터 1: 유저

	cltGameMsgResponse_Hit( SI16 charunique, SI32 damage, SI32 doubledamage, SI32 life,SI32 mode , UI08 special, bool indure, bool MissHit = false, bool DodgeHit = false, UI08 Type = 0 )
	{
		CharUnique	= charunique;
		Damage		= damage;
		DoubleDamage = doubledamage;
		Life		= life;
		Mode		= mode;
		SpecialAttackSwitch	= special;

		bIndure = indure;
		bMissHit  = MissHit;
		bDodgeHit = DodgeHit;
		uiType = Type;
	}
};

//------------------------------------------
// 플레이 시간 요청 메시지. 
//------------------------------------------
class cltGameMsgRequest_PlayTime{
public:
	SI16	siCharUnique;		

	cltGameMsgRequest_PlayTime(SI16 charunique)
	{
		siCharUnique	= charunique;
	}
};

//------------------------------------------
// 플레이 시간 응답 메시지. 
//------------------------------------------
class cltGameMsgResponse_PlayTime{
public:
	SI32	siPlaySecond;			//총 플레이 시간
	SI32	siPlayEventSecond;		//이벤트 후 경과 시간

	cltGameMsgResponse_PlayTime(SI32 playsecond, SI32 playeventsecond)
	{
		siPlaySecond		= playsecond;
		siPlayEventSecond	= playeventsecond;	
	}
};

//------------------------------------------
// 학교 설정 통보 메시지. 
//------------------------------------------
class cltGameMsgResponse_School{
public:
	SI16	siCharUnique;		// 캐릭터의 유니크. 
	SI16	siSchoolUnique;		// 학교 유니크. 

	cltGameMsgResponse_School(SI16 charunique, SI16 schoolunique)
	{
		siCharUnique	= charunique;
		siSchoolUnique	= schoolunique;
	}
};

//------------------------------------------
// 주소지 설정 통보 메시지. 
//------------------------------------------
class cltGameMsgResponse_HomeVillage{
public:
	SI16	siCharUnique;		// 주소지 설정한 캐릭터의 유니크. 
	SI16	siHomeVillage;		// 주소지 마을 유니크. 

	cltGameMsgResponse_HomeVillage(SI16 charunique, SI16 homevillage)
	{
		siCharUnique	= charunique;
		siHomeVillage	= homevillage;
	}
};

//------------------------------------------
// 찜 통보 메시지. 
//------------------------------------------
class cltGameMsgResponse_Jim{
public:
	SI16	siCharUnique;		// 찜당한 캐릭터의 유니크. 
	SI16	siJimCharUnique;	// 찜 한 캐릭터의 유니크. 

	cltGameMsgResponse_Jim(SI16 charunique, SI16 jimcharunique)
	{
		siCharUnique	= charunique;
		siJimCharUnique	= jimcharunique;
	}
};

//------------------------------------------
// 전과 통보 메시지. 
//------------------------------------------
class cltGameMsgResponse_WarKill{
public:

	UI08	uiSendMode;			// 0 : 흑의군단 전체이벤트, 1 : 흑의군단 맵이벤트
	SI16	siKillNumber;		// 처치한 적수. 
	SI16	siKilledNumber;		// 사망한 수. 
	SI16	siWarMapIndex;
	SI16	siKillScore;			// [추가 : 황진성 2008. 1. 21 => 죽여서 얻은 점수]
	SI32	siKillVillageScore;		// [추가 : 황진성 2008. 1. 21 => 죽여서 얻은 마을 점수]

	cltGameMsgResponse_WarKill(UI08	SendMode, SI16 killnumber, SI16 killednum, SI16 warmapindex, SI16 KillScore = 0, SI16 KillVillageScore = 0)
	{
		uiSendMode			= SendMode;
		siKillNumber		= killnumber;
		siKilledNumber		= killednum;
		siWarMapIndex	    = warmapindex;
		siKillScore		    = KillScore;
		siKillVillageScore  = KillVillageScore;
	}
};


//-------------------------------------------
// 소지금이 변경되었따. 
//-------------------------------------------
class cltGameMsgResponse_ChangeMoney{
public:
	GMONEY	siMoney;
	GMONEY	siChangedMoney;
	SI32	siReason;

	cltGameMsgResponse_ChangeMoney(GMONEY money, GMONEY changedmoney, SI32 reason)
	{
		siMoney = money;
		siChangedMoney	= changedmoney;
		siReason		= reason;
	}

};

//-------------------------------------------
// 한 캐릭터의 모든 정보를 담는다. 
//-------------------------------------------
class cltGameMsgResponse_AllCharInfo{
public:
	cltPersonInfo clInfo;
	SI16		  siSetItemType;
	SI16		  siSetItemStep;

	SI16		  siSetItemType2;
	SI16		  siSetItemStep2;

	cltGameMsgResponse_AllCharInfo( cltPersonInfo* pclinfo, SI16 SetItemType, SI16 SetItemStep, SI16 SetItemType2, SI16 SetItemStep2 )
	{
		clInfo.Set(pclinfo);
		siSetItemType = SetItemType;
		siSetItemStep = SetItemStep;
		siSetItemType2 = SetItemType2;
		siSetItemStep2 = SetItemStep2;
	}

};

//-------------------------------------------
// 말 풀어주기. 
//-------------------------------------------
class cltGameMsgResponse_HorseFree{
public:
	SI16	siCharUnique;

	cltGameMsgResponse_HorseFree(SI16 charunique)
	{
		siCharUnique = charunique;
	}
};

//KHY - 1001 - 기승동물 5마리 보유 수정.
class cltGameMsgRequest_ChangeHorse{
public:
	SI16 	siChangeHorseIndex;

	cltGameMsgRequest_ChangeHorse(SI16 HorseIndex)
	{
		siChangeHorseIndex = HorseIndex;
	}
};

class cltGameMsgResponse_ChangeHorse{
public:
	SI16	siChangeHorseIndex;
	cltHorse cltChangeHorse;

	SI16	siCharUnique;

	cltGameMsgResponse_ChangeHorse(SI16 HorseIndex , cltHorse * pclhorse,SI16 charunique)
	{
		siChangeHorseIndex = HorseIndex;
		if(pclhorse)	cltChangeHorse.Set( pclhorse);

		siCharUnique = charunique;
	}
};

//KHY - 1111 - 오토바이 추가.
class cltGameMsgRequest_UseOillingItem
{
public:
	 SI32 siItemUnique;

	cltGameMsgRequest_UseOillingItem( SI32 ItemUnique )
	{
		siItemUnique = ItemUnique;
	}
};

//-------------------------------------------
// 스킬 점수를 복구하겠다는 요청
//-------------------------------------------
#define INIT_SKILLMODE_LEVEL		0		// 초기화 레벨
#define INIT_SKILLMODE_TICKET		1		// 아이템 사용
#define INIT_SKILLMODE_WEAPONSKILL	2		// 아이템 사용해서 전투향상술 스킬 줄이기
// 3을 쓰면 무조건 1만 차감 그외에는 디크리스하는 스킬레벨 만큼 차감 
// 기술 전체 초기화 이용권 - 2009.11.24 손성웅
#define INIT_SKILLMODE_TICKET_ALL	4		// 아이템 사용해서 전투향상술 스킬 줄이기-전체포인트


class cltGameMsgRequest_DecreaseSkillBonus{
public:
	SI16	siSkillUnique;
	SI16	siInitMode; // 초기화 레벨때 하는 것이냐 아이템을 사용하는 것이냐

	cltGameMsgRequest_DecreaseSkillBonus(SI16 skillunique,SI16 mode)
	{
		siSkillUnique = skillunique;
		siInitMode = mode ;
	}
};


//-------------------------------------------
// 생명력 증가, 감소. 
//-------------------------------------------
#define INCLIFE_REASON_HEALTH		1	// 건강으로 인한 체력 증감. 
#define INCLIFE_REASON_HEAL			2	// 약 복용에 의한 체력 증감. 
#define INCLIFE_REASON_TIMEOUT		3   // 시간만료에 의한 체력 감소 
#define INCLIFE_REASON_SOULGUARD	4	// 수호정령에 의한 체력 증감
#define INCLIFE_REASON_EVENT_END	5	// 이벤트 시간종료에 의한 체력 감소 


class cltGameMsgResponse_IncLife{
public:
	SI16 siCharUnique;
	SI16 siReason;
	SI32 siLife;
	SI32 siIncLife;
	SI32 siReservedLife;		// 예약된 체력 회복량. 
	SI32 siIncReservedLife;		// 체력 회복량 증가, 감소분. 

	cltGameMsgResponse_IncLife(SI16 charunique, SI32 life, SI32 inclife, SI32 reservedlife, SI32 increservedlife, SI16 reason)
	{
		siCharUnique		= charunique;
		siLife				= life;
		siIncLife			= inclife;
		siReservedLife		= reservedlife;
		siIncReservedLife	= increservedlife;
		siReason			= reason;
	}
};

//-------------------------------------------
// 마법력 증가. 
//-------------------------------------------
#define INCMANA_REASON_HEALTH		1	// 건강으로 인한 마나 증감. 
#define INCMANA_REASON_HEAL			2	// 약 복용에 의한 마나  증감. 
#define INCMANA_REASON_SOULGUARD	3	// 수호정령에 의한 마나 증감

class cltGameMsgResponse_IncMana{
public:
	SI16 siCharUnique;
	SI16 siReason;
	SI16 siMana;
	SI16 siIncMana;

	cltGameMsgResponse_IncMana(SI16 charunique, SI16 mana, SI16 incmana, SI16 reason)
	{
		siCharUnique	= charunique;
		siMana			= mana;
		siIncMana		= incmana;
		siReason		= reason;	
	}
};


//-------------------------------------------
// 음식을 섭취하여 배가 불러짐을 통보. 
//-------------------------------------------
class cltGameMsgResponse_IncHungry{
public:
	SI16 siFoodUnique;	// 섭취한 음식의 유니크. 
	SI16 siHungry;		// 최종적인 포만감 

	cltGameMsgResponse_IncHungry(SI16 foodunique, SI16 hungry)
	{
		siFoodUnique	= foodunique;
		siHungry		= hungry;
	}
};


//-------------------------------------------
// 미니 홈페이지를 받아오기 위해 아이디 요청
//-------------------------------------------
class cltGameMsgRequest_Userid{
public:
	SI32 siToPersonCharUnique;
	TCHAR szToPersonName[ MAX_PLAYER_NAME ];

	cltGameMsgRequest_Userid( SI32 ToPersonCharUnique )
	{
		siToPersonCharUnique = ToPersonCharUnique;
		szToPersonName[ 0 ] = NULL;
	}

	cltGameMsgRequest_Userid( TCHAR *szname )
	{
		siToPersonCharUnique = 0;
		memcpy( szToPersonName, szname, MAX_PLAYER_NAME );
		szToPersonName[ MAX_PLAYER_NAME - 1 ] = NULL;
	}
};


//-------------------------------------------
// 미니 홈페이지를 받아오기 위해 아이디 응답
//-------------------------------------------
class cltGameMsgResponse_Userid
{
public:
	TCHAR strUserID[ MAX_PLAYER_NAME ];

	cltGameMsgResponse_Userid( TCHAR *UserID )
	{
		MStrCpy( strUserID, UserID, MAX_PLAYER_NAME );
	}
};

class cltGameMsgResponse_RecommendPersonAvailable
{
public:
	bool bAvailable;

	cltGameMsgResponse_RecommendPersonAvailable( bool Available )
	{
		bAvailable = Available;
	};
};

// 유저 추천(추천인 제도)
class cltGameMsgRequest_RecommendPerson
{
public:
	TCHAR strUserName[ MAX_PLAYER_NAME ];
	
	UI08 uiSlot;

	cltGameMsgRequest_RecommendPerson( TCHAR *strName ,UI08 slot)
	{
		MStrCpy( strUserName, strName, MAX_PLAYER_NAME );

		uiSlot = slot ;
	}
};


class cltGameMsgResponse_RecommendPerson
{
public:
	TCHAR strUserName[ MAX_PLAYER_NAME ];

	cltGameMsgResponse_RecommendPerson( TCHAR *strName )
	{
		MStrCpy( strUserName, strName, MAX_PLAYER_NAME );
	}
};

// 자막 종류 변경
class cltGameMsgRequest_CaptionKindChange
{
public:
	SI16 siCaptionKind;

	cltGameMsgRequest_CaptionKindChange( SI16 CaptionKind )
	{
		siCaptionKind = CaptionKind;
	}
};


class cltGameMsgResponse_CaptionKindChange
{
public:
	SI16 siCaptionKind;

	cltGameMsgResponse_CaptionKindChange( SI16 CaptionKind )
	{
		siCaptionKind = CaptionKind;
	}
};


class cltGameMsgResponse_AddInfo
{
public:
	SI16	siCharUnique;
	SI32	siLife;
	SI32	siMana;

	cltCharAddStatusInfo	AddInfo;

	cltGameMsgResponse_AddInfo( SI16 charunique, cltCharAddStatusInfo *paddinfo, SI32 life, SI32 mana )
	{
		siCharUnique = charunique;
		AddInfo.Set(paddinfo);
		siLife	= life;
		siMana	= mana;
	}
};

//---------------------------------------------
// 자신이 보유한 소환수의 상태 변화에 대한 통보 
//---------------------------------------------
class cltGameMsgResponse_ChangeSummonStatus
{
public:
	SI16		siCharUnique;		// 주인의 캐릭터 유니크. 
	SI16		siChildCharUnique;	// 소환수의 캐릭터 유니크. 
	SI16		siSummonIndex;

	cltGameMsgResponse_ChangeSummonStatus( SI16 charunique, SI16 childcharunique, SI16 summonindex)
	{
		siCharUnique		= charunique;
		siChildCharUnique	= childcharunique;
		siSummonIndex		= summonindex;
	}
};



//----------------------------
// 상태변경정보를 알린다.
//----------------------------
class cltGameMsgRequest_StatusInfo
{
public:
	cltCharChangeStatusInfo	ChangeInfo;

	cltGameMsgRequest_StatusInfo(cltCharChangeStatusInfo *pchangeinfo )
	{
		ChangeInfo.Set(pchangeinfo);
	}
};

class cltGameMsgResponse_StatusInfo
{
public:
	SI16					siCharUnique;

	cltCharChangeStatusInfo	ChangeInfo;

	cltGameMsgResponse_StatusInfo( SI16 charunique, cltCharChangeStatusInfo *pchangeinfo )
	{
		siCharUnique = charunique;
		ChangeInfo.Set(pchangeinfo);
	}
};

class cltGameMsgResponse_CurseInfo
{
public:
	SI16					m_siCharUnique;
	cltCharCurseEffect		m_clCharCurseEffect;

	cltGameMsgResponse_CurseInfo( SI16 _siCharunique, cltCharCurseEffect* _pclCharCurseEffect )
	{
		m_siCharUnique = _siCharunique;
		m_clCharCurseEffect.SetCurse( _pclCharCurseEffect, 0 );
	}
};

//----------------------------
// InnerStatus정보를 통보한다.
//----------------------------
class cltGameMsgResponse_InnerStatus
{
public:
	SI16					siCharUnique;

	cltInnerStatusInfo	clInnerStatusInfo;

	cltGameMsgResponse_InnerStatus( SI16 charunique, cltInnerStatusInfo *pclinnerstatus)
	{
		siCharUnique = charunique;
		clInnerStatusInfo.Set(pclinnerstatus);
	}
};

class cltGameMsgRequest_HorseNameChange
{
public:
	SI16 siHorseIndex;
	TCHAR strHorseName[ MAX_HORSE_NAME ];
	cltGameMsgRequest_HorseNameChange( SI16 HorseIndex, TCHAR *HorseName )
	{
		siHorseIndex = HorseIndex;
		MStrCpy( strHorseName, HorseName, MAX_HORSE_NAME );
	}
};

class cltGameMsgResponse_HorseNameChange
{
public:
	SI16 siHorseIndex;
	TCHAR strHorseName[ MAX_HORSE_NAME ];

	cltGameMsgResponse_HorseNameChange( SI16 HorseIndex, TCHAR *HorseName )
	{
		siHorseIndex = HorseIndex;
		MStrCpy( strHorseName, HorseName, MAX_HORSE_NAME );
	}

};

class cltGameMsgRequest_NolimitTicket_Warp
{
public:
	SI32 siItemUnique;
	
	cltGameMsgRequest_NolimitTicket_Warp( SI32 ItemUnique )
	{
		siItemUnique = ItemUnique;
	}

};

//------------------------------------
// Person의 이벤트 정보를 보낸다. 
//------------------------------------
class cltGameMsgResponse_EventInfo
{
public:
	SI16 siCharUnique;
	cltEventCharInfo clEventInfo;

	cltGameMsgResponse_EventInfo( SI16 charunique, cltEventCharInfo *pcleventinfo)
	{
		siCharUnique = charunique;
		clEventInfo.Set(pcleventinfo);
	}

};

//-------------------------------------------------
// 전쟁에서 승리시 얻은 돈과 경험치를 통보한다.
//-------------------------------------------------
class cltGameMsgResponse_WinWarPrize
{
public:
	SI16		siGrade;			// 내가 속한 마을의 순위. (마을이 없는 경우에는 -1 ) 
	GMONEY		siMoney;
	SI32		siBaseExp;
	SI32		siFameExp;			// 마을 종합 성적에 의한 명성 경험치 획득량. 	
	SI32		siPersonalFameExp;	// 개인 전공에 의한 명성 경험치 획득량 
	SI32		siItemUnique;
	SI32		siItemNum;
	bool		bBlackWarSwitch;	// 전체지도 흑의군단 이벤트인가?

	cltGameMsgResponse_WinWarPrize(SI16 grade, GMONEY money,SI32 baseexp, SI32 fameexp, SI32 personalfameexp, SI32 itemunique, SI32 itemnum, bool flag = false )
	{
		siGrade				= grade;
		siMoney				= money;
		siBaseExp			= baseexp ;
		siFameExp			= 	fameexp;
		siPersonalFameExp	= personalfameexp;
		siItemUnique		= itemunique;
		siItemNum			= itemnum;
		bBlackWarSwitch		= flag;
	}
};


//-------------------------------------------------
// 유료아이템으로 워프했음을 알린다. 
//-------------------------------------------------
class cltGameMsgResponse_WarpEffect
{
public:
	SI16		siCharUnique;	// 워프한 사람의 유니크. 

	cltGameMsgResponse_WarpEffect(SI16 charunique)
	{
		siCharUnique		= charunique;
	}
};


//-------------------------------------------------
// 스테이터스 변경 요청 메시지. 
//-------------------------------------------------
class cltGameMsgResponse_UseBonus
{
public:
	SI32 siPos;
	SI16 siAmount;

	cltGameMsgResponse_UseBonus(SI32 pos, SI16 amount)
	{
		siPos		= pos;
		siAmount	= amount;

	}
};


//-------------------------------------------------
// 특수임무 포상 내용 통보 메시지. 
//-------------------------------------------------
class cltGameMsgResponse_SpeicalQuestReward
{
public:
	cltQuestRewardInfo clQuestRewardInfo;
	SI32 siExp;
	SI32 siWord;
	UI08 uiTriedState;

	cltGameMsgResponse_SpeicalQuestReward(cltQuestRewardInfo* pclinfo, SI32 word, UI08 TriedState)
	{
		clQuestRewardInfo.Set(pclinfo);
		siWord = word;
		uiTriedState = TriedState;
	}
};


//-------------------------------------------------
// 특수임무가 설정되었음을 알린다. 
//-------------------------------------------------
class cltGameMsgResponse_SpeicalQuestMade
{
public:
	cltQuestInfoCommon	clSpecialQuestInfo;

	cltGameMsgResponse_SpeicalQuestMade(cltQuestInfoCommon*	pclSpecialQuestInfo)
	{
		clSpecialQuestInfo.Set(pclSpecialQuestInfo);
	}
};

//-----------------------------------------------------------------
// 신년이벤트 - 흉괘시 수행할 수 있는 퀘스트가 설정되었음을 알린다.
//-----------------------------------------------------------------
class cltGameMsgResponse_RemoveUnluckQuestMade
{
public:
	cltQuestInfoCommon	clRemoveUnLuckQuestInfo;

	cltGameMsgResponse_RemoveUnluckQuestMade(cltQuestInfoCommon* pclRemoveUnLuckQuestInfo)
	{
		clRemoveUnLuckQuestInfo.Set(pclRemoveUnLuckQuestInfo);
	}
};

//----------------------------------------------------------------------------
// 군주데이미션: 수행할 수 있는 퀘스트가 설정되었음을 알린다.
//----------------------------------------------------------------------------
class cltGameMsgResponse_GoonzuDayMissionMade
{
public:
	cltQuestInfoCommon clGoonzuDayMissionInfo;
	
	cltGameMsgResponse_GoonzuDayMissionMade(cltQuestInfoCommon* pclGoonzuDayMissionInfo)
	{
		clGoonzuDayMissionInfo.Set( pclGoonzuDayMissionInfo );
	}
};

//-------------------------------------------------
// 특수임무가 설정되었음을 알린다. 
//-------------------------------------------------
class cltGameMsgResponse_QuestuserInfo
{
public:
	SI16			siQuestType;
	cltPQuestInfo	clQuestInfo;

	cltGameMsgResponse_QuestuserInfo(SI16 questtype, cltPQuestInfo* pclquestinfo)
	{
		siQuestType	= questtype;
		clQuestInfo.Set(pclquestinfo);
	}
};

//-------------------------------------------------
// 한가지 기술 정보를 보낸다. 
//-------------------------------------------------
class cltGameMsgResponse_OneSkill
{
public:
	SI16				siSkillUnique;
	cltSkillSaveUnit	clSkill;

	cltGameMsgResponse_OneSkill(SI16 skillunique, cltSkillSaveUnit*	pclskill)
	{
		siSkillUnique	= skillunique;
		clSkill.Set(pclskill);
	}
};

//-------------------------------------------
// 단어 조합 완성수를 통보함
//-------------------------------------------
class cltGameMsgResponse_Clear_SpecialWord
{
public:
	SI32			siClearSpecialWord;
	SI32			siEventClearSpecialWord;
	GMONEY			siIncreaseMoney;
	GMONEY			siLeftMoney;

	cltGameMsgResponse_Clear_SpecialWord(SI32 clearspecialword, GMONEY increasemoney, GMONEY leftmoney, SI32 eventclearspecialword = 0)
	{
		siClearSpecialWord = clearspecialword ;
		siEventClearSpecialWord = eventclearspecialword;
		siIncreaseMoney = increasemoney;
		siLeftMoney = leftmoney;
	}
};
/// 케릭터 환경설정 옵션타입
enum{
	CHTFILLTER_NORMAL = 1,
	CHTFILLTER_VILLAGE	,
	CHTFILLTER_GUILD	,
	CHTFILLTER_PARTY	,
	CHTFILLTER_SYSTEM	,
	CHTFILLTER_END
};
class cltGameMsgRequest_Char_Option
{
public:
    cltPCharOptionInfo	clCharOptionInfo;// 케릭터 옵션 클래스

	cltGameMsgRequest_Char_Option( cltPCharOptionInfo	CharOptionInfo )
	{
		clCharOptionInfo.Set( &CharOptionInfo );
	}
};

class cltGameMsgResponse_Char_Option
{
public:
	cltPCharOptionInfo	clCharOptionInfo;// 케릭터 옵션 클래스

	cltGameMsgResponse_Char_Option( cltPCharOptionInfo	CharOptionInfo  )
	{
		clCharOptionInfo.Set( &CharOptionInfo );
	}
};

class cltGameMsgResponse_Char_Get_Option
{
public:
	cltPCharOptionInfo	clCharOptionInfo;

	cltGameMsgResponse_Char_Get_Option( )
	{
		ZeroMemory(this, sizeof(cltPCharOptionInfo));
	}
};

//----------------------------------------------
// 이벤트에 당첨되었는지 안되었는지를 보낸다.
//-----------------------------------------------
class cltGameMsgResponse_Prize_GameEvent
{
public:
	bool			bNewAccount;
	bool			bGameEvent;
	bool			bWinPrize;

	cltGameMsgResponse_Prize_GameEvent(bool newaccount,bool gameevent,bool winprize)
	{
		bNewAccount = newaccount ;
		bGameEvent = gameevent ;
		bWinPrize = winprize ;
	}
};

//----------------------------------------------
// 전투향상술 스킬을 설정한다.
//----------------------------------------------
class cltGameMsgRequest_Set_WeaponSkill
{
public:
	SI32			siSkillUnique;
	SI32			siSkillLevel;

	cltGameMsgRequest_Set_WeaponSkill(SI32 unique , SI32 level)
	{
		siSkillUnique = unique ;
		siSkillLevel = level ;
	}
};

class cltGameMsgResponse_Set_WeaponSkill
{
public:
	SI32		siResult;
	SI32		siSkillUnique;
	SI32		siSkillLevel;

	cltItem		clLeftItem;
	SI16		siItemPos;

	cltGameMsgResponse_Set_WeaponSkill(SI32 result , SI32 unique , SI32 level, cltItem* pclLeftItem, SI16 itemPos)
	{
		siResult		= result;
		siSkillUnique	= unique;
		siSkillLevel	= level;

		if ( pclLeftItem )
		{
			clLeftItem.Set( pclLeftItem );
		}
		siItemPos		= itemPos;
	}
};

class cltGameMsgRequest_Get_WeaponSkillCost
{
public:
	SI32		siSkillUnique;
	SI32		siSkillLevel;

	cltGameMsgRequest_Get_WeaponSkillCost(SI32 unique,SI32 level)
	{
		siSkillUnique = unique ;
		siSkillLevel = level ;
	}
};

class cltGameMsgResponse_Get_WeaponSkillCost
{
public:
	SI32		siSkillUnique;
	SI32		siSkillLevel;
	GMONEY		siMoney;

	cltGameMsgResponse_Get_WeaponSkillCost(SI32 unique,SI32 level,GMONEY money)
	{
		siSkillUnique = unique ;
		siSkillLevel = level ;
		siMoney = money ;
	}
};

class cltGameMsgResponse_CountAttack_Info
{
public:
	SI32 siFlagNum;

	cltItem clitem;

	SI32	siItemPos;
	cltItem clFlagitem;

	cltGameMsgResponse_CountAttack_Info(SI32 flag,cltItem *pclitem,SI32 pos,cltItem* pclflagitem)
	{
		siFlagNum = flag ;
		
		clitem.Set(pclitem);

		siItemPos = pos ;
		clFlagitem.Set(pclflagitem);
	}
};

#define MAX_GREETING_LENGTH		100

//--------------------------------
// 캐릭터 인사말 설정
//---------------------------------
class cltGameMsgRequest_SetGreeting
{
public:
	TCHAR				szGreeting[128];

	cltGameMsgRequest_SetGreeting(TCHAR  *greeting)
	{
		MStrCpy(szGreeting, greeting, 128);
	}
};

class cltGameMsgRequest_GetGreeting{
public:
	SI32 siPersonID;
	SI16 siParam;

	cltGameMsgRequest_GetGreeting(SI32 personid, SI16 kind)
	{
		siPersonID = personid;
		siParam = kind;
	}
};

class cltGameMsgResponse_GetGreeting
{
public:

	SI16 siParam;

	TCHAR szName[MAX_PLAYER_NAME];
	TCHAR szGreeting[128];

	cltGameMsgResponse_GetGreeting(SI16 kind, TCHAR* name, TCHAR* greeting)
	{
		siParam = kind;	
		MStrCpy(szName, name, MAX_PLAYER_NAME);
		MStrCpy(szGreeting, greeting, 128);
	}
};

// 홈페이지 설정을 가져온다. - 설정한다.
class cltGameMsgRequest_SetHomepage_URL
{
public:
	TCHAR				szHomepageURL[128];

	cltGameMsgRequest_SetHomepage_URL(TCHAR  *homepageUrl)
	{
		MStrCpy(szHomepageURL, homepageUrl, 128);
	}
};

class cltGameMsgRequest_GetHomepage_URL{
public:
	SI32 siPersonID;

	cltGameMsgRequest_GetHomepage_URL(SI32 personid )
	{
		siPersonID = personid;
	}
};

class cltGameMsgResponse_GetHomepage_URL
{
public:

	TCHAR szHomepageURL[128];

	cltGameMsgResponse_GetHomepage_URL( TCHAR* homepageUrl)
	{
		MStrCpy(szHomepageURL, homepageUrl, 128);
	}
};

class cltGameMsgRequest_SitDown
{
public:
	bool bSitDown;
	SI32 siDirection;
	cltGameMsgRequest_SitDown( bool SitDown, SI32 Direction )
	{
		bSitDown = SitDown;
		siDirection = Direction;
	}
};

class cltGameMsgResponse_NoticeItemUseDayOver
{
public:
	SI32 siItemUnique;
	SI32 siUseableDay;	// 남은 사용가능기한

	cltGameMsgResponse_NoticeItemUseDayOver(SI32 itemUnique, SI32 useableDay)
	{
		siItemUnique = itemUnique;
		siUseableDay = useableDay; 
	}
};

#define NORMAL	1	// 기존 죽었던 것들
#define PVP_DIE	2	// PVP로 죽은 것들
//-------------------------------------------
// 서버에 부활을 요청
//-------------------------------------------
class cltGameMsgRequest_Revive{
public:
	SI16 siCharUnique	;
	SI16 siParam		;
	SI16 siDieMode		;

	cltGameMsgRequest_Revive(SI16 CharUnique, SI16 param, SI16 DieMode)
	{
		siCharUnique	=	CharUnique	;
		siParam			=	param		;
		siDieMode		=	DieMode		;
	}
};
class cltGameMsgResponse_Revive{
public:
	SI16 siCharUnique	;
	SI16 siParam		;
	SI16 siDieMode		;

	cltGameMsgResponse_Revive(SI16 CharUnique, SI16 param, SI16 DieMode)
	{
		siCharUnique = CharUnique	;
		siParam = param				;
		siDieMode		=	DieMode	;
	}
};


//-------------------------------------------
// 서버에 제자리 즉시 부활을 요청
//-------------------------------------------
class cltGameMsgRequest_Resurrection{
public:
	SI16 siCharUnique	;
	SI16 siParam		;
	SI16 siDieMode		;

	cltGameMsgRequest_Resurrection(SI16 CharUnique, SI16 param, SI16 DieMode)
	{
		siCharUnique	=	CharUnique	;
		siParam			=	param		;
		siDieMode		=	DieMode		;
	}
};

class cltGameMsgRequest_GetNameFromItemSerialID{
public:
	SI32 siSerialID;

	cltGameMsgRequest_GetNameFromItemSerialID(SI32 SerialID)
	{
		siSerialID = SerialID;
	}

};

class cltGameMsgResponse_GetNameFromItemSerialID
{
public:
	SI32	siSerialID;
	TCHAR	szPlayerName[MAX_PLAYER_NAME];
	SI16	siServerUnique;

	cltGameMsgResponse_GetNameFromItemSerialID()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_GetNameFromItemSerialID));
	}

	cltGameMsgResponse_GetNameFromItemSerialID(SI32 SerialID, TCHAR* playername, SI16 ServerUnique)
	{
		siSerialID = SerialID;
		StringCchCopy(szPlayerName, MAX_PLAYER_NAME, playername);
		siServerUnique = ServerUnique;
	}
};
//-------------------------------------------
// 서버에 캐릭터명(From PersonID)을 요청
//-------------------------------------------
class cltGameMsgRequest_GetNameFromPersonID{
public:
	SI32 siPersonID;

	cltGameMsgRequest_GetNameFromPersonID(SI32 personid)
	{
		siPersonID = personid;
	}

};

class cltGameMsgResponse_GetNameFromPersonID{
public:
	SI32	siPersonID;
	TCHAR	szPlayerName[MAX_PLAYER_NAME];

	cltGameMsgResponse_GetNameFromPersonID()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_GetNameFromPersonID));
	}

	cltGameMsgResponse_GetNameFromPersonID(SI32 personid, TCHAR* playername)
	{
		siPersonID = personid;
		StringCchCopy(szPlayerName, MAX_PLAYER_NAME, playername);
	}
};
class cltGameMsgResponse_CountryInfo
{
public:
	SI32	siCountryID;
	bool	bShowCountry;

	cltGameMsgResponse_CountryInfo()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_CountryInfo));
	}
	cltGameMsgResponse_CountryInfo( SI32 countryid, bool showcountry )
	{
		siCountryID = countryid;
		bShowCountry = showcountry;
	}
};
class cltGameMsgRequest_SetCountryInfo
{
public:
	SI32	siCountryID;
	bool	bShowCountry;

	cltGameMsgRequest_SetCountryInfo()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_SetCountryInfo));
	}
	cltGameMsgRequest_SetCountryInfo( SI32 countryid, bool showcountry )
	{
		siCountryID = countryid;
		bShowCountry = showcountry;
	}
};
class cltGameMsgRequest_SetShowCountry
{
public:
	bool	bShowCountry;

	cltGameMsgRequest_SetShowCountry()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_SetShowCountry));
	}
	cltGameMsgRequest_SetShowCountry( bool showcountry )
	{
		bShowCountry = showcountry;
	}
};

class cltGameMsgRequest_SetShowPuchikomi
{
public:
	bool	bShowPuchikomi;

	cltGameMsgRequest_SetShowPuchikomi()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_SetShowPuchikomi));
	}
	cltGameMsgRequest_SetShowPuchikomi( bool showpuchikomi )
	{
		bShowPuchikomi = showpuchikomi;
	}
};


class cltGameMsgResponse_SetShowPuchikomi
{
public:
	bool	bShowPuchikomi;

	cltGameMsgResponse_SetShowPuchikomi()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_SetShowPuchikomi));
	}
	cltGameMsgResponse_SetShowPuchikomi( bool showpuchikomi )
	{
		bShowPuchikomi = showpuchikomi;
	}
};


class cltGameMsgRequest_UserListByCounty
{
public:
	SI32	siCountryID;

	cltGameMsgRequest_UserListByCounty()
	{
		ZeroMemory(this, sizeof(cltGameMsgRequest_UserListByCounty));
	}
	cltGameMsgRequest_UserListByCounty( SI32 countryid )
	{
		siCountryID = countryid;
	}
};
class cltGameMsgResponse_UserListByCounty
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_USERLIST_NUM>		siPersonId;
#else
	SI32	siPersonId[MAX_USERLIST_NUM];
#endif
	TCHAR	szCharName[MAX_USERLIST_NUM][MAX_PLAYER_NAME];

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_USERLIST_NUM>		siCharLevel;
#else
	SI32	siCharLevel[MAX_USERLIST_NUM];
#endif
	TCHAR	szVillageName[MAX_USERLIST_NUM][32];

	cltGameMsgResponse_UserListByCounty()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_UserListByCounty));
	}
	cltGameMsgResponse_UserListByCounty( SI32* personid, TCHAR** charname, SI32* charlevel, TCHAR** villagename)
	{
		memcpy( szCharName, charname, MAX_USERLIST_NUM * MAX_PLAYER_NAME * sizeof(TCHAR) );
		memcpy( szVillageName, villagename, MAX_USERLIST_NUM * 32 * sizeof(TCHAR) );
		for ( SI32 i=0; i<MAX_USERLIST_NUM ; ++i )
		{
			siPersonId[i] = personid[i];
			siCharLevel[i] = charlevel[i];
		}
	}
};
class cltGameMsgResponse_NoticeCountryUserMake
{
public:
	SI32	m_siPersonId;
	SI32	m_siCountryId;

	cltGameMsgResponse_NoticeCountryUserMake()
	{
		ZeroMemory(this, sizeof(cltGameMsgResponse_NoticeCountryUserMake));
	}
	cltGameMsgResponse_NoticeCountryUserMake( SI32 siPersonId, SI32 siCountyId )
	{
		m_siPersonId = siPersonId;
		m_siCountryId = siCountyId;
	}
};
//-----------------------------
// 20미만 레벨 유저 요청
//-----------------------------
class cltGameMsgRequest_LowLevelList{
public:
	SI16 siCharUnique;

	cltGameMsgRequest_LowLevelList(SI16 sicharunique)
	{
		siCharUnique = sicharunique;
	}
};

class cltGameMsgResponse_LowLevelList{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltChildInfo, MAX_LOWLEVEL_LIST_NUMBER-1>		clLowLevelList;
#else
	cltChildInfo clLowLevelList[MAX_LOWLEVEL_LIST_NUMBER-1];
#endif

	cltGameMsgResponse_LowLevelList(cltChildInfo* pclLowLevelList)
	{
		SI32 i;

		for(i = 0;i < MAX_LOWLEVEL_LIST_NUMBER-1;i++)
		{
			clLowLevelList[i].Set(&pclLowLevelList[i]);
		}
	}
};

#define QUEST_EFFECT_RECEIVE	1
#define QUEST_EFFECT_CLEAR		2

class cltGameMsgResponse_QuestEffect
{
public:
	SI32	m_siCharUnique;
	SI16	m_siEffectType;
	
	cltGameMsgResponse_QuestEffect(SI32 CharUnique , SI16 EffectType)
	{
		m_siCharUnique = CharUnique;
		m_siEffectType = EffectType;
	}
};


class cltGameMsgResponse_AddBuf
{
public:
	SI16	m_siCharUnique;
	SI16	m_siIndex;
	stBuf	m_stBuf;

	cltGameMsgResponse_AddBuf( SI16 siCharUnique, SI16 index, stBuf buf )
	{
		m_siCharUnique = siCharUnique;
		m_siIndex = index;
		m_stBuf = buf;
	}
};

class cltGameMsgResponse_DeleteBuf
{
public:
	SI16	m_siCharUnique;
	SI16	m_siIndex;

	cltGameMsgResponse_DeleteBuf( SI16 siCharUnique, SI16 index )
	{
		m_siCharUnique = siCharUnique;
		m_siIndex	 = index; 
	}
};

// 이벤트용 폭죽을 받겠다는 메시지
class cltGameMsgRequest_Event_FireCracker
{
public:
	cltGameMsgRequest_Event_FireCracker()
	{
	}	
};

class cltGameMsgRequest_SetEmoticon
{
public:
	SI16	m_siIndex;

	cltGameMsgRequest_SetEmoticon( SI16 index )
	{
		m_siIndex = index;
	}
};

class cltGameMsgResponse_SetEmoticon
{
public:
	SI16	m_siIndex;
	SI16	m_siCharUnique;

	cltGameMsgResponse_SetEmoticon( SI16 index, SI16 charunique )
	{
		m_siIndex = index;
		m_siCharUnique = charunique;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_SetTeacherPoint
//
// Last Modified 	: 2007 / 02 / 23
// Created		 	: 김광명
//
// Function			: [응답]클라이언트로 스승점수를 업데이트 시켜준다
//
//***************************************************************************************************
class cltGameMsgResponse_SetTeacherPoint
{
private:
	SI32	m_siTotalTeacherPoint;

public:
	cltGameMsgResponse_SetTeacherPoint( SI32 TotalTeacherPoint ) : m_siTotalTeacherPoint(TotalTeacherPoint)
	{
	}

	SI32 GetTotalTeacherPoint()
	{
		return m_siTotalTeacherPoint;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_RenouncePupil
//
// Last Modified 	: 2007 / 02 / 27
// Created		 	: 김광명
//
// Function			: [요청]스승이 제자를 짜른다.
//
//***************************************************************************************************
class cltGameMsgRequest_RenouncePupil
{
private:
	SI32	m_siPupilPersonID;

public:
	cltGameMsgRequest_RenouncePupil( SI32 PupilPersonID ) : m_siPupilPersonID(PupilPersonID)
	{
	}

	SI32 GetPupilPersonID()
	{
		return m_siPupilPersonID;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_RenouncePupil
//
// Last Modified 	: 2007 / 02 / 27
// Created		 	: 김광명
//
// Function			: [응답]스승이 제자를 짜른다.
//
//***************************************************************************************************
class cltGameMsgResponse_RenouncePupil
{
private:
	SI32	m_siResult;

public:
	cltGameMsgResponse_RenouncePupil( SI32 Result ) : m_siResult(Result)
	{
	}

	SI32 GetResult()
	{
		return m_siResult;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_OverTakeTeacherLevel
//
// Last Modified 	: 2007 / 02 / 28
// Created		 	: 김광명
//
// Function			: [응답]제자가 스승레벨을 앞질렀다(청출어람)
//
//***************************************************************************************************
class cltGameMsgResponse_OverTakeTeacherLevel
{
public:
	cltItem m_clItem;
	TCHAR	m_szPupilName[MAX_PLAYER_NAME];
	TCHAR	m_szTeaCherName[MAX_PLAYER_NAME];

public:
	cltGameMsgResponse_OverTakeTeacherLevel( cltItem* pItem, TCHAR* pPupilName, TCHAR* pTeacherName )
	{
		m_clItem.Set( pItem );
		MStrCpy( m_szPupilName, pPupilName, MAX_PLAYER_NAME );
		MStrCpy( m_szTeaCherName, pTeacherName, MAX_PLAYER_NAME );
	}

	TCHAR* GetPupilName()
	{
		return m_szPupilName;
	}

	TCHAR* GetTeacherName()
	{
		return m_szTeaCherName;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_AllCharInfoOne
//
// Last Modified 	: 2007 / 03 / 08
// Created		 	: 김광명
//
// Function			: 한 캐릭터의 정보(1)를 나눠서 담는다.
//
//***************************************************************************************************
class cltGameMsgResponse_AllCharInfoOne
{
public:
	cltPersonInfoOne clInfo;
	SI16		  siSetItemType;
	SI16		  siSetItemStep;

	SI16		  siSetItemType2;
	SI16		  siSetItemStep2;

	cltGameMsgResponse_AllCharInfoOne( cltPersonInfoOne* pclinfo, SI16 SetItemType, SI16 SetItemStep, SI16 SetItemType2, SI16 SetItemStep2 )
	{
		clInfo.Set(pclinfo);
		siSetItemType = SetItemType;
		siSetItemStep = SetItemStep;
		
		siSetItemType2 = SetItemType2;
		siSetItemStep2 = SetItemStep2;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_AllCharInfoTwo
//
// Last Modified 	: 2007 / 03 / 08
// Created		 	: 김광명
//
// Function			: 한 캐릭터의 정보(2)를 나눠서 담는다.
//
//***************************************************************************************************
class cltGameMsgResponse_AllCharInfoTwo
{
public:
	cltPersonInfoTwo clInfo;

	cltGameMsgResponse_AllCharInfoTwo(cltPersonInfoTwo* pclinfo)
	{
		clInfo.Set(pclinfo);
	}

};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_InitStatus
//
// Last Modified 	: 2007 / 04 / 06
// Created		 	: 박춘기
//
// Function			: 캐릭터 레벨이 10 또는 30이 되면 스테이터스 초기화 요청을 보낸다.
//
//***************************************************************************************************
class cltGameMsgRequest_InitStatus
{
public:
	SI16		siMode;

	cltGameMsgRequest_InitStatus(SI16 Mode)
	{
		siMode = Mode;
	}
};

/*
class cltGameMsgResponse_IshaveCushion
{
public:
	bool		bHaveCushion;

	cltGameMsgResponse_IshaveCushion(bool HaveCushion)
	{
		bHaveCushion = HaveCushion;
	}
};*/

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_PVPResult
//
// Last Modified 	: 2007 / 06 / 26
// Created		 	: 김광명
//
// Function			: PVP결과를 알려준다.
//
//***************************************************************************************************
class cltGameMsgResponse_PVPResult
{
public:
	bool	bWin;
	TCHAR	szUserName[MAX_PLAYER_NAME];

	SI32	siPVPWinNumber;
	SI32	siPVPLoseNumber;
	TCHAR	szPVPWinHighLevelUserName[MAX_PLAYER_NAME];

	cltGameMsgResponse_PVPResult( bool Win, TCHAR* UserName, SI32 PVPWinNumber, SI32 PVPLoseNumber, TCHAR* pszPVPWinHighLevelUserName )
	{
		bWin = Win;
		MStrCpy( szUserName, UserName, MAX_PLAYER_NAME );

		siPVPWinNumber	= PVPWinNumber;
		siPVPLoseNumber = PVPLoseNumber;

		szPVPWinHighLevelUserName[0] = '\0';
		if(pszPVPWinHighLevelUserName != NULL)
			MStrCpy(szPVPWinHighLevelUserName, pszPVPWinHighLevelUserName, sizeof(szPVPWinHighLevelUserName));
	}
};

class cltGameMsgRequest_Attend
{
public:
	SI08	siType;

	cltGameMsgRequest_Attend( SI08 siEventType )
	{
		siType = siEventType;
	}
};

class cltGameMsgResponse_Attend
{
public:
	SI32	siAttendanceDay;
	SI32	siEventDay;

	SI08	siType;

	cltGameMsgResponse_Attend( SI32 AttendanceDay, SI32 EventDay, SI08 siEventType )
	{
		siAttendanceDay	= AttendanceDay;
		siEventDay		= EventDay;
		siType			= siEventType;
	}
};

// [영훈] 출석체크 창에 표시될 정보를 보여줌(패킷이름은 신경쓰지 마세요)
class cltGameMsgResponse_DemandAttend
{
public:
	SI16	m_siAttendanceDay;	// 현재까지 출석일 수
	SI16	m_siNextRewardDay;	// 다음에 보상아이템 받을 일 수

	SI08	m_siType;			// 이 메시지를 보낸 타입

	bool	m_bShowDialog;		// 출석창을 보여줄지 안보여 줄지 여부를 결정한다


	cltGameMsgResponse_DemandAttend( SI16 siAttendanceDay, SI16 siNextRewardDay, SI08 siType, bool bShowDialog )
	{
		m_siAttendanceDay	= siAttendanceDay;
		m_siNextRewardDay	= siNextRewardDay;
		
		m_siType			= siType;

		m_bShowDialog		= bShowDialog;
	}

};

// 해당 이벤트 타입의 보상 아이템을 줄것을 요청
class cltGameMsgRequest_AttendRewardItem
{
public:
	SI08	siType;

	cltGameMsgRequest_AttendRewardItem( SI08 siEventType )
	{
		siType = siEventType;
	}
};



	#ifdef USE_GAME_GUARD_CLIENT

		class cltGameMsgResponse_GameGuardAuth
		{
		public:
			GG_AUTH_DATA ggData;
			cltGameMsgResponse_GameGuardAuth( GG_AUTH_DATA * pggdata )
			{
				memcpy( &ggData , pggdata , sizeof(GG_AUTH_DATA) );
			}

		};

		class cltGameMsgRequest_GameGuardAuth
		{
		public:
			GG_AUTH_DATA ggData;
			cltGameMsgRequest_GameGuardAuth( GG_AUTH_DATA * pggdata )
			{
				memcpy( &ggData , pggdata , sizeof(GG_AUTH_DATA) );
			}
		};

	#endif

	#ifdef USE_GAME_GUARD_SERVER

		class cltGameMsgResponse_GameGuardAuth
		{
		public:
			GG_AUTH_DATA ggData;
			cltGameMsgResponse_GameGuardAuth( GG_AUTH_DATA * pggdata )
			{
				memcpy( &ggData , pggdata , sizeof(GG_AUTH_DATA) );
			}

		};

		class cltGameMsgRequest_GameGuardAuth
		{
		public:
			GG_AUTH_DATA ggData;
			cltGameMsgRequest_GameGuardAuth( GG_AUTH_DATA * pggdata )
			{
				memcpy( &ggData , pggdata , sizeof(GG_AUTH_DATA) );
			}
		};

	#endif

//KHY - 0910 - 클라이언트로 캐릭터형태의 리턴값을 보낸다.
class cltGameMsgRequest_CharServerRTN
{
public:
	SI32	 siResult;

	cltGameMsgRequest_CharServerRTN( SI32	 result)
	{
		siResult = result;
	}
};

//KHY - 1220 - 변신시스템.
class cltGameMsgRequest_SetTransForm
{
public:
	SI32	m_siTransFormKind;

	cltGameMsgRequest_SetTransForm( SI16 TransFormKind )
	{
		m_siTransFormKind = TransFormKind;
	}
};

class cltGameMsgResponse_SetTransForm
{
public:
	cltTransFormInfo clTransFormInfo;

	cltGameMsgResponse_SetTransForm( cltTransFormInfo *pclTransFormInfo )
	{
		clTransFormInfo.Set(pclTransFormInfo);
	}
};

class cltGameResponse_TimeMeterRate_Update
{
public:
	cltTimeMeterRate	clTimeMeterRate;

	cltGameResponse_TimeMeterRate_Update(cltTimeMeterRate*	pclTimeMeterRate)
	{
		if(pclTimeMeterRate)
		{
			clTimeMeterRate.siTimeMeterRateType = pclTimeMeterRate->siTimeMeterRateType;
			clTimeMeterRate.Set(pclTimeMeterRate);
		}
	}
};
class cltGameMsgRequest_UseTransFormItem
{
public:
	 SI32 siItemUnique;

	cltGameMsgRequest_UseTransFormItem( SI32 ItemUnique )
	{
		siItemUnique = ItemUnique;
	}
};

class cltGameMsgResponse_MaxSyncro_Attainment
{
public:
	cltItem				m_clItem;			// 변신용 아이템 - 광고용
	UI16				m_uiCharKind;		// 100% 달성한 변신 캐릭터 KIND
	NTCHARStringName	m_kCharName;		// 달성한 캐릭터 이름

	cltGameMsgResponse_MaxSyncro_Attainment(cltItem* pclItem, UI16 uiCharKind, NTCHARStringName CharName)
	{
		m_clItem.Init();
		if(pclItem)		m_clItem.Set(pclItem);

		m_uiCharKind = uiCharKind;
		m_kCharName = CharName;
	}
};

class cltGameResponse_Health_Change
{
public:
	SI16 				m_siHungry	 ;	
	SI08 				m_siInnerOrg1;	
	SI08 				m_siInnerOrg2;	
	SI08 				m_siInnerOrg3;	
	SI08 				m_siInnerOrg4;	

	cltGameResponse_Health_Change(SI16 _siHungry,	   SI08 _siInnerOrg1, SI08 _siInnerOrg2, 
								  SI08 _siInnerOrg3,   SI08 _siInnerOrg4							)
	{
		m_siHungry	  = _siHungry;	
		m_siInnerOrg1 = _siInnerOrg1;
		m_siInnerOrg2 = _siInnerOrg2;
		m_siInnerOrg3 = _siInnerOrg3;
		m_siInnerOrg4 = _siInnerOrg4;
	}
};



//----------------------------
// [진성] 영웅의 동상 이벤트 버프룰렛 돌릴때 사용하는 패킷. => 2008-6-10
//----------------------------
class cltGameMsgRequest_Buff_Roulette
{
public:
	bool	m_bAction;

	SI16	m_BuffKind;
	GMONEY  m_Money;

	cltGameMsgRequest_Buff_Roulette(bool _bAction, SI16	_BuffKind = 0, GMONEY  _Money = 0)
	{
		m_bAction   = _bAction;
		m_BuffKind  = _BuffKind;
		m_Money		= _Money;
	}
};

class cltGameMsgResponse_Buff_Roulette
{
public:
	bool	m_bAction;
	SI32	m_siCharUnique;
	SI32	m_CompleteBuffKind;

	cltGameMsgResponse_Buff_Roulette(bool _bAction, SI32 _siCharUnique, SI32 _CompleteBuffKind)
	{ 
		m_bAction			= _bAction;
		m_CompleteBuffKind	= _CompleteBuffKind;
		m_siCharUnique		= _siCharUnique;
	}
};


//	[종호_BALANCECURRENCY] 통화량 조절 관련 패킷
class cltGameMsgRequest_BalanceCurrency_Buff
{
public:
	bool	m_bAction;
	SI16	m_BuffKind;
	GMONEY  m_Money;

	cltGameMsgRequest_BalanceCurrency_Buff(bool _bAction, SI16	_BuffKind = 0, GMONEY  _Money = 0)
	{
		m_bAction		= _bAction;
		m_BuffKind		= _BuffKind;
		m_Money			= _Money;
	}
};

class cltGameMsgResponse_BalanceCurrency_Buff
{
public:
	bool	m_bAction;
	SI32	m_siCharUnique;
	SI32	m_CompleteBuffKind;

	cltGameMsgResponse_BalanceCurrency_Buff(bool _bAction, SI32 _siCharUnique, SI32 _CompleteBuffKind)
	{ 
		m_bAction			= _bAction;
		m_CompleteBuffKind	= _CompleteBuffKind;
		m_siCharUnique		= _siCharUnique;
	}
};


//-------------------------------------------------
// 캐릭터 룰렛
//-------------------------------------------------
enum CHAR_ROULETTE
{
	CHAR_ROULETTE_NONE,
	CHAR_ROULETTE_START,
	CHAR_ROULETTE_END,
	CHAR_ROULETTE_ADDBUF,
};

class cltGameMsgRequest_Char_Roulette
{
public:
	SI08		m_siBufRoulette;		// 캐릭터 룰렛 실행 종류
	SI16		m_siRequestKind;		// 캐릭터 룰렛 요청 카인드
	bool 	b_bUseTicketItem;  // 소모성 아이템 유니크 - > 있다면 우선처리다.

	cltGameMsgRequest_Char_Roulette( SI08 siBufRoulette, SI16 siRequestKind ,bool bUseTicketItem)
	{
		m_siBufRoulette = siBufRoulette;
		m_siRequestKind	= siRequestKind;
		b_bUseTicketItem = bUseTicketItem;
	}
};

class cltGameMsgResponse_Char_Roulette
{
public:
	SI16		m_siCharUnique;			// 캐릭터 룰렛을 사용하는 캐릭터
	SI08		m_siBufRoulette;		// 캐릭터 룰렛 실행 종류
	SI16		m_siResultKind;			// 캐릭터 룰렛 결과 카인드
	SI16		m_siRouletteKinds[30];	// 캐릭터 룰렛용 카인드들
	SI16		m_siCount;

	cltGameMsgResponse_Char_Roulette( SI16 siCharUnique, SI08 siBufRoulette, SI16 siResultKind, SI16* psiRouletteKinds, SI16 siCount )
	{
		m_siCharUnique = siCharUnique;
		m_siBufRoulette = siBufRoulette;
		m_siResultKind = siResultKind;

		if(psiRouletteKinds)		memcpy(m_siRouletteKinds, psiRouletteKinds, min(sizeof(SI16) * siCount, sizeof(m_siRouletteKinds)));
		m_siCount = siCount;
	}
};

//-------------------------------------------
//		일반필드에서 pvp 요청
//-------------------------------------------

#define REQUEST_PVP		0	// pvp 신청
#define START_PVP		1	// pvp 승낙
#define REFUSE_PVP		2	// pvp 거절
#define END_PVP			3	// pvp 종료
#define WIN_PVP			4	// pvp 승리
#define LOSE_PVP		5	// pvp 패바
#define READY_PVP		6	// pvp 5초대기
#define DRAW_PVP		7	// PVP Draw
#define ILLPLACE_PVP	8	// PVP Draw
#define PVP_LEAGUESTART	9	// PVP Draw
#define PVP_LEAGUEBEFORE1M	10	// PVP 리그 1분전
#define PVP_LEAGUE	11	// PVP 리그 머리위에 그림 띄워주기 위해서 


#define PVPWAITTIME	5	// pvp 대기 시간.

class cltGameMsgRequest_PVP_Request
{
public:
	SI16	siRequestType	;	// 현제 이 메세지 요청이 어떤 의미로 보내는가.
	SI32	siPara1		;	// pvp 대상ID, 승리 여부 등등 보내야할 내용들
	

	cltGameMsgRequest_PVP_Request(	SI16 RequestType , SI32 Para1 )
	{		
		siPara1			=	Para1	;
		siRequestType	=	RequestType	;
	}
};
		
class cltGameMsgResponse_PVP_Request
{
public:
	SI16	siRequestType	;	// 현제 이 메세지 요청이 어떤 의미로 보내는가.
	SI32	siPara1			;	//pvp 대상ID, 승리 여부 등등 보내야할 내용들
	SI32	siPara2			;	//pvp 대상ID, 승리 여부 등등 보내야할 내용들
	NTCHARStringName m_kCharName	;			// 신청한 사람이름


	cltGameMsgResponse_PVP_Request( SI16 RequestType , SI32 Para1 , SI32 Para2, NTCHARStringName _kCharName)
	{
		siRequestType	=	RequestType	;
		siPara1			=	Para1	;
		siPara2			=	Para2	;
		m_kCharName		=  _kCharName	;
	}
};

//[진성] 마을 주민 초대하기.
class cltGameMsgRequest_Village_Resident_Invite
{
public:
	NTCHARStringName m_kCharName;	// 초대 당하는 제자의 이름.

	cltGameMsgRequest_Village_Resident_Invite( NTCHARStringName _kCharName )
	{
		m_kCharName = _kCharName;
	}
};

//[진성] 마을 주민 초대하기.
class cltGameMsgResponse_Village_Resident_Invite
{
public:
	NTCHARStringName m_kCharName;			// 초대하는 스승의 이름.
	SI32			 m_siVillageUnique;		// 초대하는 스승의 마을 유니크.

	cltGameMsgResponse_Village_Resident_Invite( NTCHARStringName _kCharName, SI32 _siVillageUnique )
	{
		m_kCharName			= _kCharName;
		m_siVillageUnique	= _siVillageUnique;
	}
};

// 

enum PVP_LEAGUE_APPLY_RESULT
{
	PVP_LEAGUE_APPLY_RESULT_NONE = 0,
	PVP_LEAGUE_APPLY_RESULT_SUCCESS,	// 성공
	PVP_LEAGUE_APPLY_RESULT_ALREADY,	// 이미 신청했음
	PVP_LEAGUE_APPLY_RESULT_FULL		// 신청인원이 다 참
};



// [영훈] PVP 무한대전 개인 정보(요청 자체가 없다)
class cltGameMsgResponse_PVP_League_InfoGet
{
public:
	cltPVP_LeagueInfo	m_clPVP_LeaugeInfo;

	cltGameMsgResponse_PVP_League_InfoGet()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_PVP_League_InfoGet) );
	}

	cltGameMsgResponse_PVP_League_InfoGet( cltPVP_LeagueInfo* pclInfo )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_PVP_League_InfoGet) );

		if ( pclInfo )
		{
			m_clPVP_LeaugeInfo.Set( pclInfo );
		}
	}
};


// [영훈] PVP 무한대전 창 정보(요청 패킷은 없다)
//GAMEMSG_REQUEST_PVP_LEAGUE_UI_INFOGET
class cltGameMsgResponse_PVP_League_UI_InfoGet
{
public:
	SI32				m_siWin;
	SI32				m_siLose;
	SI32				m_siDraw;

	SI32				m_siGroupNum;

	cltPVP_League_OtherUserInfo	m_clLatest[Const_PVP_League_Latest_Max];	// 최근 5경기 기록
	cltPVP_League_OtherUserInfo	m_clTop[Const_PVP_League_Top_Max];			// 상위 10명


	cltGameMsgResponse_PVP_League_UI_InfoGet()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_PVP_League_UI_InfoGet) );
	}

	cltGameMsgResponse_PVP_League_UI_InfoGet( SI32 siWin, SI32 siLose, SI32 siDraw, SI32 siGroupNum, cltPVP_League_OtherUserInfo* pclLatest, cltPVP_League_OtherUserInfo* pclTop )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_PVP_League_UI_InfoGet) );

		m_siWin			= siWin;
		m_siLose		= siLose;
		m_siDraw		= siDraw;

		m_siGroupNum	= siGroupNum;

		if ( pclLatest )
		{
			memcpy( m_clLatest, pclLatest, sizeof(m_clLatest) );
		}

		if ( pclTop )
		{
			memcpy( m_clTop, pclTop, sizeof(m_clTop) );
		}
	}
};	// GAMEMSG_RESPONSE_PVP_LEAGUE_INFOGET

// 경기에 매칭되지 못해 다시 예약할것인지에 대한 답변을 담은 패킷
class cltGameMsgRequest_PVP_League_Again_Reserve
{
public:
	bool	m_bAnswer;	// true=다시 예약함, fasle=예약 에서 삭제함

	cltGameMsgRequest_PVP_League_Again_Reserve()
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_PVP_League_Again_Reserve) );
	}

	cltGameMsgRequest_PVP_League_Again_Reserve( bool bAnswer )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_PVP_League_Again_Reserve) );

		m_bAnswer = bAnswer;
	}
};	// GAMEMSG_REQUEST_PVP_LEAGUE_AGAIN_RESERVE

// 내 PVP무한대전 등급이 변경 되었음
class cltGameMsgResponse_PVP_League_Battle_ChangeGrade
{
public:
	SI16	m_siGrade;

	cltGameMsgResponse_PVP_League_Battle_ChangeGrade()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_PVP_League_Battle_ChangeGrade) );
	}

	cltGameMsgResponse_PVP_League_Battle_ChangeGrade( SI16 siGrade )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_PVP_League_Battle_ChangeGrade) );

		m_siGrade = siGrade;
	}
};

//-------------------------------------------------
// 신년운세 룰렛
//-------------------------------------------------
enum DIVINATION_ROULETTE
{
	DIVINATION_ROULETTE_NONE,
	DIVINATION_ROULETTE_START,
	DIVINATION_ROULETTE_END,
	DIVINATION_ROULETTE_ADDBUF,
};

enum SELECT_BUFKIND
{
	BUFKIND_NONE,
	BUFKIND_CHAR,	// 캐릭터 카인드
	BUFKIND_ITEM,		// 아이템
	BUFKIND_BUF,		// 버프
};

class cltGameMsgRequest_Divination_Roulette
{
public:
	SI08		m_siDivinationRoulette;		// 신년운세 룰렛 실행 종류
	SI16		m_siRequestDivination;		// 신년운세 룰렛 요청 운세
	SI16		m_siBufKind_Type;			// 신년운세 버프 종류


	cltGameMsgRequest_Divination_Roulette( SI08 siDivinationRoulette, SI16 siRequestDivination, SI16 siBufKind_Type = BUFKIND_NONE )
	{
		m_siDivinationRoulette = siDivinationRoulette;
		m_siRequestDivination  = siRequestDivination;
		m_siBufKind_Type	   = siBufKind_Type;
	}
};

class cltGameMsgResponse_Divination_Roulette
{
public:
	SI16		m_siCharUnique;						// 신년운세 룰렛을 사용하는 캐릭터
	SI08		m_siDivinationRoulette;				// 신년운세 룰렛 실행 종류
	SI16		m_siResultDivination;				// 신년운세 룰렛 결과 운세
	SI16		m_siRouletteDivinations[50];		// 신년운세 룰렛용 카인드들
	SI16		m_siCount;
	SI16		m_siBufKind_Type;					// 신년운세 버프종류

	cltGameMsgResponse_Divination_Roulette( SI16 siCharUnique, SI08 siDivinationRoulette, SI16 siResultDivination, SI16* psiRouletteDivinations, SI16 siCount, SI16 siBufKind_Type = BUFKIND_NONE )
	{
		m_siCharUnique = siCharUnique;
		m_siDivinationRoulette = siDivinationRoulette;
		m_siResultDivination = siResultDivination;

		if(psiRouletteDivinations)		memcpy(m_siRouletteDivinations, psiRouletteDivinations, min(sizeof(SI16) * siCount, sizeof(m_siRouletteDivinations)));
		m_siCount = siCount;
		m_siBufKind_Type = siBufKind_Type;
	}
};
class cltGameRequest_ValentineWar_Info_get
{
public:
	SI32 sicharunique	;

	cltGameRequest_ValentineWar_Info_get( SI32 charunique )
	{
		sicharunique	=	charunique ;
	}
};
class cltGameResponse_ValentineWar_Info_get
{
public:
	cltValentineWarInfo clValentinewarInfo;

	cltGameResponse_ValentineWar_Info_get( cltValentineWarInfo* pclValentinewarInfo )
	{
		clValentinewarInfo.Set(pclValentinewarInfo )	;
	}
};
// 솔로인지 커플인지 답변 안했는지 날렸다.
class cltGameMsgRequest_ValentineWar_Request
{
public:
	SI16 siCouple		;	// 내가 솔로인지 커플인지 대답 안한건지 여부
	SI32 siCharUnique	;	// 대답한사람의 케릭터 유니크
	cltGameMsgRequest_ValentineWar_Request(SI32 CharUnique , SI16 Coulpe )
	{	
		siCouple = Coulpe	;
		siCharUnique = siCharUnique	;
	}
};
class cltGameMsgResponse_ValentineWar_Request
{
public:
	SI32 siCharUnique	;	// 대답한사람의 케릭터 유니크
	cltValentineWarInfo clValentinewarInfo;
	cltGameMsgResponse_ValentineWar_Request( SI32 CharUnique, cltValentineWarInfo ValentinewarInfo )
	{
		siCharUnique = CharUnique	;
		clValentinewarInfo = ValentinewarInfo	;

	}
};
class cltGameResponse_ValentineWar_OpenDlg
{
public:
	bool m_bOpen	;
	cltGameResponse_ValentineWar_OpenDlg( bool bOpen )
	{
		m_bOpen = bOpen	;
	}
};

class cltGameResponse_ValentineWar_PrizeGet
{
public:
	SI32 m_siCharUnique	;
	bool m_bGet	;
	cltGameResponse_ValentineWar_PrizeGet( SI32 siCharUnique ,bool bGet )
	{
		m_siCharUnique = siCharUnique	;
		m_bGet = bGet	;
	}
};

// [영훈] 프로포즈 질문
class cltGameRequest_Propose_Question
{
public:
	TCHAR	m_szCharName[MAX_PLAYER_NAME];
	TCHAR	m_szMessage[Const_Max_Propose_Message];

	cltGameRequest_Propose_Question( TCHAR* pszCharName, TCHAR* pszMessage )
	{
		ZeroMemory( this, sizeof(cltGameRequest_Propose_Question) );

		if ( pszCharName )
		{
			StringCchCopy( m_szCharName, MAX_PLAYER_NAME, pszCharName );
		}

		if ( pszMessage )
		{
			StringCchCopy( m_szMessage, Const_Max_Propose_Message, pszMessage );
		}
	}
};	// GAMEMSG_REQUEST_PROPOSE_QUESTION


// [영훈] 프로포즈받을 캐릭터에게 질문이 간다
class cltGameResponse_Propose_Question
{
public:
	TCHAR	m_szCharName[MAX_PLAYER_NAME];
	TCHAR	m_szMessage[Const_Max_Propose_Message];

	cltGameResponse_Propose_Question( TCHAR* pszCharName, TCHAR* pszMessage )
	{
		ZeroMemory( this, sizeof(cltGameResponse_Propose_Question) );

		if ( pszCharName )
		{
			StringCchCopy( m_szCharName, MAX_PLAYER_NAME, pszCharName );
		}

		if ( pszMessage )
		{
			StringCchCopy( m_szMessage, Const_Max_Propose_Message, pszMessage );
		}
	}
};	// GAMEMSG_RESPONSE_PROPOSE_QUESTION

// [영훈] 프로포즈에 대한 승낙/거절 여부
class cltGameRequest_Propose_Answer
{
public:
	bool m_bAccept;

	cltGameRequest_Propose_Answer( bool bAccept )
	{
		m_bAccept = bAccept;
	}
};	// GAMEMSG_REQUEST_PROPOSE_ANSWER

// [영훈] 결혼 했다
class cltGameResponse_Set_Marry
{
public:
	SI32	m_siMatePersonID;		// 배우자
	TCHAR	m_szMateName[MAX_PLAYER_NAME];

	SI32	m_siWeddingDateVary;	// 결혼한 날짜

	SI16	m_siLoveLevel;			// 애정지수
	SI16	m_siConnectMode;		// 배우자 접속 모드
	
	cltItem	m_clItem;				// 사용한 아이템
	SI16	m_siItemPos;			// 사용한 아이템 위치

	cltGameResponse_Set_Marry(
		SI32		siMatePersonID,		TCHAR* pszMateName,
		SI32		siWeddingDateVary,
		SI16		siLoveLevel,		SI16 siConnectMode,
		cltItem*	pclItem,			SI16 siItemPos
	)
	{
		ZeroMemory( this, sizeof(cltGameResponse_Set_Marry) );

		m_siMatePersonID	= siMatePersonID;
		if ( pszMateName )
		{
			StringCchCopy( m_szMateName, sizeof(m_szMateName), pszMateName );
		}

		m_siWeddingDateVary	= siWeddingDateVary;

		m_siLoveLevel		= siLoveLevel;
		m_siConnectMode		= siConnectMode;

		if ( pclItem )
		{
			m_clItem.Set( pclItem );
			m_siItemPos		= siItemPos;
		}
	}
};	// GAMEMSG_RESPONSE_SET_MARRY


class cltGameResponse_Get_Marry_Info
{
public:
	SI32	m_siMatePersonID;		// 배우자
	TCHAR	m_szMateName[MAX_PLAYER_NAME];
	
	SI32	m_siWeddingDateVary;	// 결혼한 날짜
	SI32	m_siItemGetDateVary;	// 결혼기념일 아이템 받은 날짜

	SI16	m_siLoveLevel;			// 애정지수
	SI32	m_siUseHourVary;		// 애정도 아이템 사용한 시간
	SI32	m_siLimitHourVary;		// 애정도 아이템 제한 시간

	SI16	m_siConnectMode;		// 배우자 접속 모드

	cltGameResponse_Get_Marry_Info( SI32 siMatePersonID,	TCHAR* pszMateName,
		SI32 siWeddingDateVary,	SI32 siItemGetDateVary,
		SI16 siLoveLevel,		SI32 siUseHourVary,			SI32 siLimitHourVary,
		SI16 siConnectMode
	)
	{
		m_siMatePersonID	= siMatePersonID;
		if ( pszMateName )
		{
			StringCchCopy( m_szMateName, sizeof(m_szMateName), pszMateName );
		}

		m_siWeddingDateVary	= siWeddingDateVary;
		m_siItemGetDateVary	= siItemGetDateVary;

		m_siLoveLevel		= siLoveLevel;		
		m_siUseHourVary		= siUseHourVary;	
		m_siLimitHourVary	= siLimitHourVary;

		m_siConnectMode		= siConnectMode;
	}

};	// GAMEMSG_RESPONSE_GET_MARRY_INFO

class cltGameResponse_Marry_ItemGet_Set
{
public:
	SI32	m_siItemGetDateVary;	// 결혼기념일 아이템 받은 날짜

	cltGameResponse_Marry_ItemGet_Set( SI32 siItemGetDateVary )
	{
		m_siItemGetDateVary = siItemGetDateVary;
	}
	
};	// GAMEMSG_RESPONSE_MARRY_ITEMGET_SET

class cltGameResponse_Marry_Love_Set
{
public:
	SI16	m_siLoveLevel;			// 애정도
	SI32	m_siUseHourVary;		// 애정도 아이템 최종 사용시간
	SI32	m_siLimitHourVary;		// 애정도 아이템 제한 시간

	cltGameResponse_Marry_Love_Set( SI16 siLoveLevel, SI32 siUseHourVary, SI32 siLimitHourVary )
	{
		m_siLoveLevel		= siLoveLevel;		
		m_siUseHourVary		= siUseHourVary;	
		m_siLimitHourVary	= siLimitHourVary;
	}

};	// GAMEMSG_RESPONSE_MARRY_LOVE_SET


// 보물찾기 이벤트- 오카라 10개로 아무짝에 쓸모없는ㅂ ㅓ프 받기!
class cltGameMsgRequest_TreasureEvent_Use_Okara{
public:
	SI16 siCharUnique	;

	cltGameMsgRequest_TreasureEvent_Use_Okara(SI16 CharUnique)
	{
		siCharUnique	=	CharUnique	;
	}
};
class cltGameMsgResponse_TreasureEvent_Use_Okara{
public:
	SI16 siCharUnique	;

	cltGameMsgResponse_TreasureEvent_Use_Okara(SI16 CharUnique)
	{
		siCharUnique = CharUnique	;
	}
};
//가족
// [성웅] 자녀를 등록요청
class cltGameRequest_Request_Add_Children
{
public:
	TCHAR	m_szCharName[MAX_PLAYER_NAME];

	cltGameRequest_Request_Add_Children( TCHAR* pszCharName )
	{
		ZeroMemory( this, sizeof(cltGameRequest_Request_Add_Children) );

		if ( pszCharName )
		{
			StringCchCopy( m_szCharName, MAX_PLAYER_NAME, pszCharName );
		}
	}
};
class cltGameResponse_Request_Add_Children
{
public:
	SI32	m_siParentPersonID	;
	TCHAR	m_szParentCharName[MAX_PLAYER_NAME];	// 자식등록을 요청한 부모케릭터
	SI32	m_siMatePersonID	;
	TCHAR	m_szMateCharName[MAX_PLAYER_NAME];	// 자식등록을 요청한 부모케릭터
	SI32	m_siChildrenPersonID	;
	TCHAR	m_szChildrenCharName[MAX_PLAYER_NAME];	// 자식등록을 요청받은 케릭터

	cltGameResponse_Request_Add_Children( SI32 siParentPersonID, TCHAR* szParentCharName ,SI32 siMatePersonID, TCHAR* szMateCharName , SI32 siChildrenPersonID, TCHAR* szChildrenCharName )
	{
		ZeroMemory( this, sizeof(cltGameResponse_Request_Add_Children) );

		m_siParentPersonID= siParentPersonID	;
		if ( szParentCharName )
		{
			StringCchCopy( m_szParentCharName, MAX_PLAYER_NAME, szParentCharName );
		}
		m_siMatePersonID= siMatePersonID	;
		if ( szParentCharName )
		{
			StringCchCopy( m_szMateCharName, MAX_PLAYER_NAME, szMateCharName );
		}
		m_siChildrenPersonID= siChildrenPersonID	;
		if ( szChildrenCharName )
		{
			StringCchCopy( m_szChildrenCharName, MAX_PLAYER_NAME, szChildrenCharName );
		}
	}
};
// [성웅] 자녀를 등록한다
class cltGameRequest_Add_Children
{
public:
	SI32	m_siParentPersonID	;

	cltGameRequest_Add_Children( SI32	siParentPersonID )
	{
		ZeroMemory( this, sizeof(cltGameRequest_Add_Children) );
		m_siParentPersonID	=	siParentPersonID	;
	}
};
class cltGameResponse_Add_Children
{
public:
	TCHAR	m_szCharName[MAX_PLAYER_NAME];

	cltGameResponse_Add_Children( TCHAR* pszCharName)
	{
		ZeroMemory( this, sizeof(cltGameResponse_Add_Children) );

		if ( pszCharName )
		{
			StringCchCopy( m_szCharName, MAX_PLAYER_NAME, pszCharName );
		}
	}
};
// [성웅] 자녀를 삭제 한다.
class cltGameRequest_Del_Children
{
public:
	SI32 m_siPersonID	;	// 탈퇴할 personID

	cltGameRequest_Del_Children( SI32 siPersonID )
	{
		ZeroMemory( this, sizeof(cltGameRequest_Del_Children) );
		m_siPersonID	=	siPersonID	;

	}
};
class cltGameResponse_Del_Children
{
public:
	TCHAR	m_szCharName[MAX_PLAYER_NAME];

	cltGameResponse_Del_Children( TCHAR* pszCharName)
	{
		ZeroMemory( this, sizeof(cltGameResponse_Del_Children) );

		if ( pszCharName )
		{
			StringCchCopy( m_szCharName, MAX_PLAYER_NAME, pszCharName );
		}
	}
};
class cltGameResponse_Get_Familyinfo
{
public:
	CFamily m_cFamily	;
	CMarriage m_cMarriage ;

	cltGameResponse_Get_Familyinfo( CFamily* Family , CMarriage* cMarriage)
	{
		ZeroMemory( this, sizeof(cltGameResponse_Get_Familyinfo) );

		m_cFamily.Set(Family)	;
		m_cMarriage.Set(cMarriage)	;

	}
};

class cltGameResponse_UpdateFamily
{
public:
	CFamily m_cFamily	;
	CMarriage m_cMarriage ;

	cltGameResponse_UpdateFamily( CFamily* Family, CMarriage* cMarriage )
	{
		ZeroMemory( this, sizeof(cltGameResponse_UpdateFamily) );
		m_cFamily.Set(Family)	;
		m_cMarriage.Set(cMarriage)	;

	}
};

// [성웅] 가족에게 워프 한다.
class cltGameRequest_WarpFamily
{
public:
	SI32 m_siPersonID	;	//워프할 가족

	cltGameRequest_WarpFamily( SI32 siPersonID )
	{
		ZeroMemory( this, sizeof(cltGameRequest_WarpFamily) );
		m_siPersonID	=	siPersonID	;

	}
};
class cltGameResponse_WarpFamily
{
public:
	TCHAR	m_szCharName[MAX_PLAYER_NAME];

	cltGameResponse_WarpFamily( TCHAR* pszCharName)
	{
		ZeroMemory( this, sizeof(cltGameResponse_WarpFamily) );

		if ( pszCharName )
		{
			StringCchCopy( m_szCharName, MAX_PLAYER_NAME, pszCharName );
		}
	}
};
// [성웅] 해당 가족의 효과를 연장 한다
class cltGameRequest_Plus_FamilyEffectDate
{
public:
	SI32 m_siPersonID	;	//워프할 가족

	cltGameRequest_Plus_FamilyEffectDate( SI32 siPersonID )
	{
		ZeroMemory( this, sizeof(cltGameRequest_Plus_FamilyEffectDate) );
		m_siPersonID	=	siPersonID	;

	}
};
class cltGameResponse_Plus_FamilyEffectDate
{
public:
	CFamily m_cltFamily	;

	cltGameResponse_Plus_FamilyEffectDate( CFamily* cltFamily )
	{
		ZeroMemory( this, sizeof(cltGameResponse_Plus_FamilyEffectDate) );

		m_cltFamily.Set(cltFamily ) ;

	}
};

// [성웅] 가족 채팅을 날린다.
class cltGameRequest_ChatFamily
{
public:
	TCHAR	m_szCharName[MAX_PLAYER_NAME];
	TCHAR	m_szChatData[MAX_CHAT_DATA_LENGTH];

	cltGameRequest_ChatFamily( TCHAR* pszCharName,TCHAR* szChatData )
	{
		ZeroMemory( this, sizeof(cltGameRequest_ChatFamily) );
		if ( m_szCharName )
		{
			StringCchCopy( m_szCharName, MAX_PLAYER_NAME, pszCharName );
		}
		if ( m_szChatData )
		{
			StringCchCopy( m_szChatData, MAX_CHAT_DATA_LENGTH, szChatData );
		}
	}
};
class cltGameResponse_ChatFamily
{
public:
	stChatEditData m_szchatdata	;

	cltGameResponse_ChatFamily( stChatEditData* szchatdata )
	{
		ZeroMemory( this, sizeof(cltGameResponse_ChatFamily) );

		memcpy( &m_szchatdata, szchatdata, sizeof(stChatEditData) );
	}
};
	
#endif

