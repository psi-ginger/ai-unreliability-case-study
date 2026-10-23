#ifndef _MSGTYPESYSTEM_H
#define _MSGTYPESYSTEM_H

#include "../../Common/Ask/Ask.h"
#include "../../Common/Master/Master.h"

#include "../Server/server.h"
#include "../Server/Minister/MinisterBase.h"
#include "../Server/Minister/Minister-Yezo.h"
#include "../Server/Minister/Minister-Hozo.h"
#include "../Server/Minister/Minister-Hyungzo.h"
#include "../Server/Event/KingGuri/KingGuri.h"
#include "../Server/Event/SantaRaccoon/SantaRaccoonEvent.h"

#include "../Server/ServerEvent/ServerEvent_Manager.h"
#include "../Server/ServerEvent/ServerEvent-Quest.h"

#include "../../Common/WhisperChatMgr/WhisperChatBaseDefine.h"
#include "../../Common/WhisperChatMgr/WhisperChatObj.h"

#include "../../CommonLogic/WarRank/WarRank.h"

#include "../../DBManager/GameDBManager_World/DBGameLogMsg.h"

#include "../../Client/ninterface/nchatbardlg/NChatBarDlg.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"

#include "../Server/Event/GoonzuKingGuri/GoonzuKingGuri.h"

//-----------------------------
// PersonID로 귓말 채널 만들기. 
//-----------------------------
class cltGameMsgRequest_MakeSecretChatChannel{
public:
	SI32 siPersonID;
	SI16 siCharUnique;
	TCHAR szCharName[MAX_PLAYER_NAME];
	TCHAR szChatBuffer[256];


	cltGameMsgRequest_MakeSecretChatChannel(SI32 personid, SI16 charunique,TCHAR * name ,TCHAR * buf = NULL )
	{


		siPersonID		= personid;
		siCharUnique	= charunique;
		
		MStrCpy(szCharName,name, MAX_PLAYER_NAME);
		MStrCpy(szChatBuffer, buf, 256 );
	}
};



class cltGameMsgResponse_MakeSecretChatChannel{
public:
	bool	bGameMaster;
	SI16	siCharUnique;
	SI32	siLevel;
	TCHAR	szName[MAX_PLAYER_NAME];
	TCHAR	szAccountID[MAX_PLAYER_NAME];
	TCHAR	szChatBuffer[256];
	
	bool	bHelp;							//[진성] 도우미. 2008-10-13

	cltGameMsgResponse_MakeSecretChatChannel( bool gamemaster, SI16 charunique, SI32 level, TCHAR* pname, TCHAR* accountid, TCHAR* chatbuf, bool _bHelp = false )
	{
		bGameMaster = gamemaster;
		siCharUnique = charunique;
		siLevel = level;
		
		MStrCpy(szName, pname, MAX_PLAYER_NAME );
		MStrCpy( szAccountID, accountid, MAX_PLAYER_NAME );

		if ( chatbuf != NULL )
		{
			MStrCpy(szChatBuffer, chatbuf, 256);
		}
		else
		{
			szChatBuffer[ 0 ] = NULL;
		}

		bHelp = _bHelp;
	}
};
class cltGameMsgRequest_AcceptSecretChatChannel{//[영진]
public:
//	SI32 siPersonID;
	SI16 siCharUnique;
	TCHAR szCharName[MAX_PLAYER_NAME];
	bool	bAccept;
	cltGameMsgRequest_AcceptSecretChatChannel(bool Accept, SI16 charunique,TCHAR * name  )
	{
		bAccept = Accept;
		//siPersonID		= personid;
		siCharUnique	= charunique;

		MStrCpy(szCharName,name, MAX_PLAYER_NAME);

	}
};

class cltGameMsgResponse_AcceptSecretChatChannel{ //[영진]
public:
	SI16	siCharUnique;
	SI32	siLevel;
	TCHAR	szName[MAX_PLAYER_NAME];
	bool    bAccept;

	cltGameMsgResponse_AcceptSecretChatChannel(bool Accept,SI16 charunique, TCHAR* pname)
	{
		bAccept = Accept;

		siCharUnique = charunique;

		MStrCpy(szName, pname, MAX_PLAYER_NAME );

	}
};


const SI32 RAREITEM_NOTICE_SWITCH_MADE				= 1;
const SI32 RAREITEM_NOTICE_SWITCH_WENNYOLDMAN		= 2;
const SI32 RAREITEM_NOTICE_SWITCH_GACHA				= 3;
const SI32 RARAITEM_NOTICE_SWITCH_SUMMONDOLL		= 4;
const SI32 RARAITEM_NOTICE_SWITCH_HALLOWEENMANTLE	= 5;
const SI32 RARAITEM_NOTICE_SWITCH_CHRISTMASEVENT	= 6;
const SI32 RARAITEM_NOTICE_SWITCH_PVPLEAGUEPRIZE	= 7;
const SI32 RARAITEM_NOTICE_SWITCH_LEVELUPREWARD		= 8;
const SI32 RAREITEM_NOTICE_SWITCH_ENCHANTSUCCESS	= 9;

//------------------------------------------
// 명품 제조 통보.
//------------------------------------------
class cltGameMsgResponse_RareItemMade{
public:
	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 만든 사람 이름. 
	SI32	siPersonID;						// 명품 제조자의 PersonID
	cltItem clItem;							// 만들어진 물품 정보. 

	SI32	siRareSwitch;		 		// 명품을 획득한 경로.
	SI32	siIncFameExp;					// 명성 경험치증가량. 

	cltGameMsgResponse_RareItemMade()
	{
		siPersonID = 0;
		//StringCchCopy(szPersonName, MAX_PLAYER_NAME, TEXT(""));
		szPersonName[ 0 ] = NULL;
		siRareSwitch	= 0;
		siIncFameExp	= 0;
	}

	cltGameMsgResponse_RareItemMade(TCHAR* ppersonname, SI32 personid, cltItem* pclitem, SI32 rareswitch, SI32 incfameexp)
	{
		MStrCpy( szPersonName, ppersonname, MAX_PLAYER_NAME );
		siPersonID	= personid;
		clItem.Set(pclitem);
		siRareSwitch	= rareswitch;
		siIncFameExp	= incfameexp;
	}
};

//------------------------------------------
// 대박 전리품 획득 통보.
//------------------------------------------

class cltGameMsgResponse_BigWarItem{
public:
	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 만든 사람 이름. 
	SI32	siPersonID;						// 명품 제조자의 PersonID
	cltItem clItem;							// 만들어진 물품 정보. 

	SI32	siIncFameExp;					// 명성 경험치증가량. 
	
	SI16	siReason;						// 이 아이템을 받게 된 사유. 
	SI16	siPara1;						// 파라메타 1

	cltGameMsgResponse_BigWarItem()
	{
		siPersonID = 0;
		//StringCchCopy(szPersonName, MAX_PLAYER_NAME, TEXT(""));
		szPersonName[ 0 ] = 0;
		siIncFameExp	= 0;
		siReason		= 0;
		siPara1			= 0;
	}

	cltGameMsgResponse_BigWarItem(const TCHAR* ppersonname, SI32 personid, cltItem* pclitem, SI32 incfameexp, SI16 reason, SI16 para1)
	{
		MStrCpy(szPersonName, ppersonname, MAX_PLAYER_NAME );
		siPersonID	= personid;
		clItem.Set(pclitem);
		siIncFameExp	= incfameexp;
		siReason		= reason;	
		siPara1			= para1;
	}
};

//------------------------------------------
// 적토마, 흑마, 백마 만들어짐.
//------------------------------------------
class cltGameMsgResponse_GoodHorse{
public:
	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 만든 사람 이름. 
	SI32	siPersonID;						// 명품 제조자의 PersonID
	SI16	siHorseUnique;					// 만들어진 말의 유니크. 

	SI32	siIncFameExp;					// 명성 경험치증가량. 

	cltGameMsgResponse_GoodHorse()
	{
		siPersonID = 0;
		StringCchCopy(szPersonName, MAX_PLAYER_NAME,  TEXT(""));
		siHorseUnique	= 0;
		siIncFameExp	= 0;
	}

	cltGameMsgResponse_GoodHorse(TCHAR* ppersonname, SI32 personid, SI16 horseunique, SI32 incfameexp)
	{
		MStrCpy( szPersonName, ppersonname, MAX_PLAYER_NAME );
		siPersonID	= personid;
		siHorseUnique	= horseunique;
		siIncFameExp	= incfameexp;
	}
};
////2차 가챠.
//class cltGameMsgResponse_TanGwangGirl{
//public:
//	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 만든 사람 이름. 
//	SI32	siPersonID;						// 명품 제조자의 PersonID
//	SI16	siItemUnique;					// 만들어진 말의 유니크. 
//
//	SI32	siIncFameExp;					// 명성 경험치증가량. 
//
//	cltGameMsgResponse_TanGwangGirl()
//	{
//		siPersonID = 0;
//		StringCchCopy(szPersonName, MAX_PLAYER_NAME,  TEXT(""));
//		siItemUnique	= 0;
//		siIncFameExp	= 0;
//	}
//
//	cltGameMsgResponse_TanGwangGirl(TCHAR* ppersonname, SI32 personid, SI16 itemunique, SI32 incfameexp)
//	{
//		MStrCpy( szPersonName, ppersonname, MAX_PLAYER_NAME );
//		siPersonID	= personid;
//		siItemUnique	= itemunique;
//		siIncFameExp	= incfameexp;
//	}
//};

//KHY - 1001 - 기승동물 5마리 보유 수정.
class cltGameMsgResponse_NoAddHorse{
public:
	SI16	siResult;					// 결과값.

	cltGameMsgResponse_NoAddHorse()
	{
		siResult = 0;
	}

	cltGameMsgResponse_NoAddHorse(SI16	Result)
	{
		siResult = Result;
	}
};

//------------------------------------------
// 높은 단계 소환수 출연 
//------------------------------------------
class cltGameMsgResponse_HighSummon{
public:
	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 만든 사람 이름. 
	SI32	siPersonID;						// 명품 제조자의 PersonID
	SI16	siSummonKind;					// 만들어진 소환수 KIND

	SI32	siIncFameExp;					// 명성 경험치증가량. 

	cltGameMsgResponse_HighSummon()
	{
		siPersonID = 0;
		StringCchCopy(szPersonName, MAX_PLAYER_NAME, TEXT(""));
		siSummonKind	= 0;
		siIncFameExp	= 0;
	}

	cltGameMsgResponse_HighSummon(TCHAR* ppersonname, SI32 personid, SI16 summonkind, SI32 incfameexp)
	{
		MStrCpy( szPersonName, ppersonname, MAX_PLAYER_NAME );
		siPersonID		= personid;
		siSummonKind	= summonkind;
		siIncFameExp	= incfameexp;
	}
};


//------------------------------------------
// 사용자 레벨업 통보.
//------------------------------------------
class cltGameMsgResponse_UserLevelUp{
public:
	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 레벨업 대상자. 
	TCHAR	szFatherName[MAX_PLAYER_NAME];	// 스승 이름. 
	SI16	siLevel;

	cltGameMsgResponse_UserLevelUp()
	{
		StringCchCopy(szPersonName, MAX_PLAYER_NAME, TEXT(""));
		StringCchCopy(szFatherName, MAX_PLAYER_NAME, TEXT(""));
		siLevel	= 0;
	}

	cltGameMsgResponse_UserLevelUp(TCHAR* ppersonname, TCHAR* pfathername, SI16 level)
	{
		MStrCpy(szPersonName, ppersonname, MAX_PLAYER_NAME );
		MStrCpy(szFatherName, pfathername, MAX_PLAYER_NAME );
		siLevel	= level;
	}
};

//------------------------------------------
// 기술 장인 등록/해지 요청 
//------------------------------------------
class cltGameMsgRequest_RegistMaster{
public:
	SI16	siSkillUnique;			// 해당 기술. 

	cltGameMsgRequest_RegistMaster()
	{
		siSkillUnique	= 0;
	}

	cltGameMsgRequest_RegistMaster(SI16 skillunique)
	{
		siSkillUnique	= skillunique;
	}
};


//------------------------------------------
// 기술 장인 명단을 요청한다.
//------------------------------------------
class cltGameMsgRequest_SkillMasterList{
public:
	SI16	siSkillUnique;

	cltGameMsgRequest_SkillMasterList()
	{
		siSkillUnique	= 0;
	}

	cltGameMsgRequest_SkillMasterList(SI16 skillunique)
	{
		siSkillUnique = skillunique;
	}
};

class cltGameMsgResponse_SkillMasterList{
public:
	SI16			siSkillUnique;
	SI16			siDataNum;			// 장인의 개수. 
	cltMasterUnitInfo clUnit[MAX_MASTER_PER_SKILL];

	cltGameMsgResponse_SkillMasterList()
	{
		siSkillUnique	= 0;
		siDataNum		= 0;
	}

	cltGameMsgResponse_SkillMasterList(SI16 skillunique)
	{
		siSkillUnique	= skillunique;
		siDataNum		= 0;
	}

	SI32 GetTotalSize()
	{
		return sizeof(SI16) + sizeof(SI16) + sizeof(cltMasterUnitInfo) * siDataNum;
	}
	
	void Add(cltMasterUnitInfo* pclinfo)
	{
		if(siDataNum >= MAX_MASTER_PER_SKILL)return ;

		clUnit[siDataNum].Set(pclinfo);
		siDataNum++;
	}
};

/*

//------------------------------------------
// 로그인 공지를 보낸다. 
//------------------------------------------
class cltGameMsgResponse_NoticeLogin{
public:
	SI16	siSize;
	TCHAR	szText[2048];

	cltGameMsgResponse_NoticeLogin()
	{
		siSize	= 0;
		strcpy(szText, TEXT(""));
	}

	cltGameMsgResponse_NoticeLogin(TCHAR* pnotice)
	{
		siSize	= min(2048, lstrlen(pnotice) + 1);
		strncpy(szText, pnotice, 2048-1);
	}

	SI32 GetTotalSize()
	{
		return sizeof(SI32);
		return sizeof(SI32) + siSize;
	}

};
*/

//------------------------------------------
// 모든 마을의 점령 상태를 통보한다.
//------------------------------------------
class cltGameMsgResponse_EnemyCapture{
public:
	SI08	siVillageUnique;
	bool	bCapture;

	cltGameMsgResponse_EnemyCapture()
	{
		siVillageUnique = 0;
		bCapture		= 0;
	}

	cltGameMsgResponse_EnemyCapture(SI08 villageunique, bool bcapture)
	{
		siVillageUnique = villageunique;
		bCapture		= bcapture;
	}

};

//------------------------------------------
// 모든 마을의 점령 상태를 통보한다.
//------------------------------------------
class cltGameMsgResponse_EnemyCaptureAll{
public:
	SI32	siBit[3];

	cltGameMsgResponse_EnemyCaptureAll()
	{
		for(SI32 i = 0;i < 3;i++)
		{
			siBit[i] = 0;
		}
	}

	void Set(bool mode, SI32 villageunique)
	{
		SI32 index	= villageunique / 32;
		if(index >= 3)return ;

		SI32 bit	= villageunique % 32;
		if(mode == true)
		{
			siBit[index] |= (1 << bit);
		}
		else
		{
			SI32 data = (1 << bit);
			if(siBit[index] & data)
			{
				siBit[index] -= data;
			}
		}
	}

};

//------------------------------------------
// 특정 사용자의 질문을 삭제하도록 요청한다. 
//------------------------------------------
class cltGameMsgResponse_DelAsk{
public:
	SI16	siPersonID;

	cltGameMsgResponse_DelAsk()
	{
		siPersonID	= 0;
	}
	cltGameMsgResponse_DelAsk(SI32 personid)
	{
		siPersonID	= personid;
	}

};

//------------------------------------------
// 질문 리스트를 보낸다.  
//------------------------------------------
class cltGameMsgResponse_AskListUnit{
public:
	SI16	siCharUnique;
	SI32	siPersonID;
	TCHAR	szAskPersonName[MAX_PLAYER_NAME];
	TCHAR	szAskText[MAX_ASK_TEXT_SIZE];

	cltGameMsgResponse_AskListUnit()
	{
		void Init();
	}

	cltGameMsgResponse_AskListUnit(SI16 charunique, SI32 personid, TCHAR* ppersonname, TCHAR* asktext)
	{
		siCharUnique	= charunique;
		siPersonID		= personid;
		MStrCpy(szAskPersonName, ppersonname, MAX_PLAYER_NAME );
		MStrCpy(szAskText, asktext, MAX_ASK_TEXT_SIZE );
	}
	void Set(cltGameMsgResponse_AskListUnit* pclinfo)
	{
		siCharUnique	= pclinfo->siCharUnique;
		siPersonID		= pclinfo->siPersonID;
		MStrCpy( szAskPersonName, pclinfo->szAskPersonName, MAX_PLAYER_NAME );
		MStrCpy( szAskText, pclinfo->szAskText, MAX_ASK_TEXT_SIZE );
	}

	void Init()
	{
		siCharUnique	= 0;
		siPersonID		= 0;
	}
};

#define MAX_RESPONSE_ASK_LIST	20
class cltGameMsgResponse_AskList{
public:

	cltGameMsgResponse_AskListUnit clList[MAX_RESPONSE_ASK_LIST];

	cltGameMsgResponse_AskList()
	{
		Init();
	}

	void Init()
	{
		for(SI32 i = 0; i < MAX_RESPONSE_ASK_LIST;i++)
		{
			clList[i].Init();
		}
	}
};

//------------------------------------------
// 운영자가 돈을 준 것에 대한 응답. 
//------------------------------------------
class cltGameMsgResponse_GMGiveMoney{
public:

	 TCHAR	szPersonName[MAX_PLAYER_NAME];
	 GMONEY	siMoney;

	cltGameMsgResponse_GMGiveMoney(TCHAR* szname, GMONEY money)
	{
		MStrCpy( szPersonName, szname, MAX_PLAYER_NAME );
		siMoney			= money;
	}
};

//------------------------------------------
// 물품 가격 정보. 
//------------------------------------------
class cltGameMsgRequest_ProductPrice
{
public:

	SI16	siItemUnique;
	SI32	siDialogID;

	cltGameMsgRequest_ProductPrice( SI16 itemunique, SI32 dialogID = 0 )
	{
		siItemUnique = itemunique;
		siDialogID = dialogID;
	}
};
class cltGameMsgResponse_ProductPrice{
public:

	 SI16	siItemUnique;
	 GMONEY	siPrice;
	 SI32	siDialogID;

	cltGameMsgResponse_ProductPrice(SI16 itemunique, GMONEY price, SI32 dialogID = 0 )
	{
		siItemUnique	= itemunique;
		siPrice			= price;
		siDialogID		= dialogID;
	}
};

//------------------------------------------
// GM이 사용자 캐릭터에 대한 정보를 요청한다. 
//------------------------------------------
class cltGameMsgResponse_UserCharInfo
{
public:

	TCHAR			szAccountID[30];
	TCHAR			szPassword[30];
	bool			bConnectSwitch;		// 접속 여부. 

    
	cltMoney		clTotalMoney;		// 소지금
	cltMoney		clBankMoney;		// 전장금액

	SI64			siWorldTradeMoney;
	
	cltSummonInfo	clSummonInfo;		// 소환수 정보
	SI32			siVillageUnique;	// 가입 마을

	SI32			siPersonID;
	SI16			siCharUnique;		//캐릭터 유니크 넘버(1~3000).
	SI16			siLevel;
	GMONEY			siMoney;

	SI16			siMapIndex;
	SI16			siX;
	SI16			siY;
	SI08			siGoonzuFriends;
	
	TCHAR			szOtherPersonName1[ MAX_PLAYER_NAME ];
	TCHAR			szOtherPersonName2[ MAX_PLAYER_NAME ];

	TCHAR			szRecommender[MAX_PLAYER_NAME]; // 추천인
	TCHAR			szFatherName[MAX_PLAYER_NAME]; // 스승
	SI16			siStudentLevel;					// 제자가 되었을 때의 레벨

	TCHAR			szMakeDate[24];	// 캐릭터 생성일

	cltUserBlockLog	m_clUserBlockLog[MAX_BLOCKLOG_NUM+1];


//	cltItem clItem[MAX_ITEM_PER_PERSON];

	cltGameMsgResponse_UserCharInfo(TCHAR* szaccountid, TCHAR* password, cltMoney* pclmoney, cltMoney* pclbankmoney, SI64 worldtrademoney,
		bool connectswitch, SI32 personid, SI16 charUnique, SI16 level, GMONEY money, SI16 mapindex, SI16 x, SI16 y,
		SI08 goonzufriends, TCHAR *otherPerson1, TCHAR *otherPerson2,
		cltSummonInfo*	pclsummoninfo, SI32 villageunique, TCHAR* recommender,
		TCHAR *fathername,SI16 studentlevel,TCHAR* makedate, cltUserBlockLog* pclUserBlockLog)
	{
		MStrCpy( szAccountID, szaccountid, MAX_PLAYER_NAME );
		MStrCpy( szPassword, password, MAX_PLAYER_NAME );
		bConnectSwitch	= connectswitch;
		siPersonID		= personid;
		siCharUnique	= charUnique;		
		siLevel			= level;
		siMoney			= money;
		siMapIndex		= mapindex;
		siX				= x;
		siY				= y;
		siGoonzuFriends	= goonzufriends;

		clTotalMoney.Set(pclmoney);

		MStrCpy( szOtherPersonName1, otherPerson1, MAX_PLAYER_NAME );

		MStrCpy( szOtherPersonName2, otherPerson2, MAX_PLAYER_NAME );

		clBankMoney.Set( pclbankmoney );

		siWorldTradeMoney = worldtrademoney;
		
		memcpy( &clSummonInfo, pclsummoninfo, sizeof(cltSummonInfo));

		siVillageUnique = villageunique;

		MStrCpy( szRecommender, recommender, 24 );
		MStrCpy( szFatherName, fathername,MAX_PLAYER_NAME);
		siStudentLevel = studentlevel ;
		MStrCpy( szMakeDate, makedate,24);

		if ( pclUserBlockLog )	memcpy( m_clUserBlockLog, pclUserBlockLog, (sizeof(cltUserBlockLog) * (MAX_BLOCKLOG_NUM+1)) );
	}
};

//=====================================================================================
// 용량이 커서 2개로 분리함
//=====================================================================================
class cltGameMsgResponse_UserCharItemInfo
{
public:
	// 인벤토리
	cltItem		m_clItem[MAX_ITEM_PER_PERSON];

	cltGameMsgResponse_UserCharItemInfo()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_UserCharItemInfo) );
	}

	cltGameMsgResponse_UserCharItemInfo( cltItem* pclItemList )
	{
		SI32 siIndex = 0;

		if (pclItemList)
		{
			for ( siIndex=0 ;siIndex<MAX_ITEM_PER_PERSON; ++siIndex )
			{
				m_clItem[siIndex].Set( &pclItemList[siIndex] );
			}
		}
	}
};	// GAMEMSG_RESPONSE_USERCHARITEMINFO

class cltGameMsgResponse_UserCharItemInfo2
{
public:
	// 공간상자 아이템
	cltItem		m_clSpaceBox[MAX_ITEM_SPACE_BOX];
	// 보물상자 아이템
	cltItem		m_clTreasureBox[MAX_ITEM_TREASURE_BOX];

	cltGameMsgResponse_UserCharItemInfo2()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_UserCharItemInfo2) );
	}

	cltGameMsgResponse_UserCharItemInfo2( cltItem* pclSpaceBox, cltItem* pclTreasureBox )
	{
		SI32 siIndex = 0;

		if ( pclSpaceBox )
		{
			for ( siIndex=0 ;siIndex<MAX_ITEM_SPACE_BOX; ++siIndex )
			{
				m_clSpaceBox[siIndex].Set( &pclSpaceBox[siIndex] );
			}
		}

		if ( pclTreasureBox )
		{
			for ( siIndex=0 ;siIndex<MAX_ITEM_TREASURE_BOX; ++siIndex )
			{
				m_clTreasureBox[siIndex].Set( &pclTreasureBox[siIndex] );
			}
		}
	}
};	// GAMEMSG_RESPONSE_USERCHARITEMINFO2


class cltGameMsgRequest_AccountInfo
{
public:

	TCHAR	szAccountID[ 32 ];

	cltGameMsgRequest_AccountInfo( TCHAR *accountid )
	{
		memcpy( szAccountID, accountid, 32 );
		szAccountID[ 31 ] = NULL;
	}
};


class cltGameMsgResponse_AccountInfo
{
public:
	TCHAR	szAccountID[ 32 ];
	TCHAR	szPersonName1[ MAX_PLAYER_NAME ];
	TCHAR	szPersonName2[ MAX_PLAYER_NAME ];
	TCHAR	szPersonName3[ MAX_PLAYER_NAME ];
	
	cltGameMsgResponse_AccountInfo( TCHAR *accountid, TCHAR *personname1, TCHAR *personname2, TCHAR *personname3 )
	{
		memcpy( szAccountID, accountid, 32 );
		szAccountID[ 31 ] = NULL;

		memcpy( szPersonName1, personname1, MAX_PLAYER_NAME );
		szPersonName1[ MAX_PLAYER_NAME - 1 ] = NULL;

		memcpy( szPersonName2, personname2, MAX_PLAYER_NAME );
		szPersonName2[ MAX_PLAYER_NAME - 1 ] = NULL;

		memcpy( szPersonName3, personname3, MAX_PLAYER_NAME );
		szPersonName3[ MAX_PLAYER_NAME - 1 ] = NULL;

	}
};


//------------------------------------------
// 전쟁 메시지. 
//------------------------------------------
#define WAR_CMD_BREAK		1		// 전쟁 발발. 
#define WAR_CMD_WIN			2		// 전쟁 종결. (승리) 
#define WAR_CMD_LOSE		3		// 전쟁 종결. (패배) 
#define WAR_CMD_GOING		4		// 전쟁중. 

#define BLACKWAR_CMD_BREAK		5	// 흑의군단 전쟁 발발. 
#define BLACKWAR_CMD_WIN		6	// 흑의군단 전쟁 종결. (승리) 
#define BLACKWAR_CMD_LOSE		7	// 흑의군단 전쟁 종결. (패배) 
#define BLACKWAR_CMD_GOING		8	// 흑의군단 전쟁중. 

class cltGameMsgResponse_BlackWar
{
public:

	SI32 siCmd;			

	DWORD uiLeftTime;		// 전쟁 남은 시간. 
	SI32 siLeftEnemy;
	SI32 siTotalEnemy;
	SI32 siCasualty;		// 사상자. 
	SI32 siMapIndex;
	SI32 siX;
	SI32 siY;
	bool bMakeBoss;

	TCHAR szSortPlayerNames[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];
	SI32 siWarScores[MAX_WARRANK_PERSON_PER_MAP];
	
	//[추가 : 황진성 2008. 1. 21 => 마을 점수관련해서 클라에게 줄 정보.]
	TCHAR szSortVillageNames[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];
	SI32 siWarVillageScores[MAX_WARRANK_PERSON_PER_MAP];

	cltGameMsgResponse_BlackWar( SI32 Cmd, DWORD LeftTime, SI32 LeftEnemy, SI32 TotalEnemy, SI32 Casualty, SI32 Mapindex, 
		SI32 X, SI32 Y, bool MakeBoss, TCHAR** PlayerNames, SI32* WarScores, TCHAR** VillageNames, SI32* WarVillageScores )
	{
		siCmd		 = Cmd;

		uiLeftTime	 = LeftTime;
		siLeftEnemy	 = LeftEnemy;
		siTotalEnemy = TotalEnemy;
		siCasualty	 = Casualty;

		siMapIndex	 = Mapindex;
		siX			 = X;
		siY			 = Y;
		bMakeBoss	 = MakeBoss;

		memcpy(szSortPlayerNames, PlayerNames, MAX_PLAYER_NAME * MAX_WARRANK_PERSON_PER_MAP * sizeof(TCHAR));
		memcpy(siWarScores, WarScores, MAX_WARRANK_PERSON_PER_MAP * sizeof(SI32));

		memcpy(szSortVillageNames, VillageNames,	 MAX_PLAYER_NAME * MAX_WARRANK_PERSON_PER_MAP * sizeof(TCHAR));
		memcpy(siWarVillageScores, WarVillageScores, MAX_WARRANK_PERSON_PER_MAP * sizeof(SI32));
	}
};

//[추가 : 황진성 2008. 1. 23 => 흑의 군단 개편 이벤트 종료 후 결과를 알릴 패킷.]  
class cltGameMsgResponse_Black_War_Reform_Char_Result
{
public:
	SI32	siCharScore;
	GMONEY	siCharRewardMoney;
	bool	bWin;
	SI32	siRank;
	
	cltGameMsgResponse_Black_War_Reform_Char_Result(SI32	CharScore, GMONEY CharRewardMoney, bool Win, SI32	Rank)
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Black_War_Reform_Char_Result ) );
		
		siCharScore			 =	CharScore;
		siCharRewardMoney	 = 	CharRewardMoney;
		bWin			     =  Win;
		siRank				 =  Rank;
	}
};

//[추가 : 황진성 2008. 1. 23 => 흑의 군단 개편 이벤트 종료 후 결과를 알릴 패킷.]  
class cltGameMsgResponse_Black_War_Reform_Village_Result
{
public:
	SI32	siVillageScore;
	SI08	siVillageRank;
	GMONEY	siVillageRewardMoney;
	bool	bWin;

	cltGameMsgResponse_Black_War_Reform_Village_Result(SI32 VillageScore,	SI08 VillageRank, GMONEY VillageRewardMoney, bool	Win)
	{
		ZeroMemory( this, sizeof( cltGameMsgResponse_Black_War_Reform_Village_Result ) );

		siVillageScore		 =	VillageScore;
		siVillageRank		 =	VillageRank;
		siVillageRewardMoney =	VillageRewardMoney;
		bWin			     =  Win;
	}
};

class cltGameMsgResponse_BlackArmy_War{
public:
	SI16 siCmd;			

	DWORD uiLeftTime;		// 전쟁 남은 시간. 
	SI32 siWarLeftMon;		// 전쟁 남은 몬스터.
	SI32 siWarMapIndex;		// 전쟁맵인덱스

	TCHAR szSortPlayerNames[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];
	SI32 siWarScores[MAX_WARRANK_PERSON_PER_MAP];

	cltGameMsgResponse_BlackArmy_War(SI32 cmd, DWORD lefttime, SI32 warmapindex, SI32 warleftmon, TCHAR** PlayerNames, SI32* WarScores)
	{
		siCmd			= cmd;

		uiLeftTime		= lefttime;
		siWarMapIndex	= warmapindex;
		siWarLeftMon	= warleftmon;

		memcpy(szSortPlayerNames, PlayerNames, MAX_PLAYER_NAME * MAX_WARRANK_PERSON_PER_MAP * sizeof(TCHAR));
		memcpy(siWarScores, WarScores, MAX_WARRANK_PERSON_PER_MAP * sizeof(SI32));

	}
};


//-------------------------------------------
// NPC위치 정보 요청. 
//-------------------------------------------
class cltGameMsgRequest_NPCPos{
public:
	SI16 siKind;	

	cltGameMsgRequest_NPCPos(SI16 kind)
	{
		siKind = kind;
	}
};

class cltGameMsgResponse_NPCPos{
public:
	SI16 siKind;	
	SI16 siMapNumber;

	UI08 uiMapIndex[MAX_MAP_NUMBER];



	cltGameMsgResponse_NPCPos(SI16 kind, SI16 mapnumber, UI08* pmapindex)
	{
		siKind		= kind;
		siMapNumber	= mapnumber;

		for(SI32 i = 0;i < mapnumber;i++)
		{
			uiMapIndex[i] = pmapindex[i];
		}
	}

	SI32 GetTotalSize()
	{
		return sizeof(SI16) + sizeof(SI16) + siMapNumber;
	}
};




//-------------------------------------------
// 머릿말 통보. 
//-------------------------------------------
class cltGameMsgRequest_HeadChat{
public:
	SI16 siCharUnique;	
	cltGameMsgRequest_HeadChat(SI16 charunique)
	{
		siCharUnique = charunique;
	}
};

//-------------------------------------------
// 머릿말 통보. 
//-------------------------------------------
class cltGameMsgResponse_HeadChat{
public:
	SI16 siCharUnique;		// 머릿말을 사용하는 캐릭터의 유니크. 
	SI16 siTextSize;
	TCHAR szText[MAX_HEAD_CHAT_TEXT_SIZE];

	cltGameMsgResponse_HeadChat(SI16 charunique, TCHAR* ptext)
	{
		siCharUnique = charunique;
		memcpy( szText, ptext, MAX_HEAD_CHAT_TEXT_SIZE );
		szText[MAX_HEAD_CHAT_TEXT_SIZE-1] = NULL;

		siTextSize = _tcslen( szText ) + 1;		
	}
	
	SI32 GetTotalSize()
	{
		return sizeof(SI16) + sizeof(SI16) + siTextSize + 1;
	}
};


//-------------------------------------------
// 귓말 채팅 성공/실패 여부 통보. 
//-------------------------------------------
#define SECRETCHAT_FAIL_INVALIDNAME		-1	// 이름 불분명으로 실패. 
#define SECRETCHAT_FAIL					0	// 원인 모름 실패. 
#define SECRETCHAT_FAIL_NOTUSER			-2  // 해당자가 접속을 안함.
#define SECRETCHAT_SUCCESS				1	// 성공. 



class cltGameMsgResponse_SecretChatResult{
public:
	SI16 siResult;					// 성공 , 실패. 
	TCHAR szName[MAX_PLAYER_NAME];	// 귓말 대상자 이름. 
	SI32 siCharUnique;

	cltGameMsgResponse_SecretChatResult(SI16 result, TCHAR* pname,SI32 CharUnique = 0)
	{
		siResult	= result;
		memcpy( szName, pname, MAX_PLAYER_NAME );
		szName[ MAX_PLAYER_NAME - 1 ] = NULL;
		siCharUnique = CharUnique;
	}
};

//------------------------------------------
// 귓말 전송. 
//------------------------------------------
#define MAX_SECRET_CHAT_TEXT_LENGTH 100 
class cltGameMsgSecretChat
{
public:
	SI16 siCharUnique;
	TCHAR szCharName[MAX_PLAYER_NAME];
	TCHAR szChatText[MAX_SECRET_CHAT_TEXT_LENGTH];

	cltGameMsgSecretChat( SI16 charunique, TCHAR* name, TCHAR* chattext )
	{
		siCharUnique = charunique;
		MStrCpy( szCharName, name, MAX_PLAYER_NAME ); 
		MStrCpy( szChatText, chattext, MAX_SECRET_CHAT_TEXT_LENGTH ); 
	}
};


//-------------------------------------------
// 현재/최대 사용자 숫자. 
//-------------------------------------------
class cltGameMsgResponse_UserNum{
public:
	SI16 siCurUserNum;			// 현재 사용자 숫자. 
	SI16 siMaxUserNum;			// 최대 사용자 숫자. 


	bool bDBFailSwitch;			// DB에 문제가 있는지 여부. 
	
	cltGameMsgResponse_UserNum(SI16 curnum, SI16 maxnum, bool bdb)
	{
		siCurUserNum	= curnum;
		siMaxUserNum	= maxnum;
		bDBFailSwitch	= bdb;
	}
};


//-------------------------------------------
// 사망 페널티 정보
//-------------------------------------------
class cltGameMsgResponse_DeathPenalty{
public:
	GMONEY siMoney;
	SI32 siExp;
	SI16 siKilledMode;
	TCHAR szKillMeCharName[MAX_PLAYER_NAME];

	cltGameMsgResponse_DeathPenalty(GMONEY money, SI32 exp, SI16 killmode, TCHAR* pname)
	{
		siMoney			= money;
		siExp			= exp;
		siKilledMode	= killmode;
		MStrCpy( szKillMeCharName, pname, MAX_PLAYER_NAME );
		szKillMeCharName[MAX_PLAYER_NAME-1] = '\0';

	}

};

//-----------------------------------------
//채팅 메시지. 
//-----------------------------------------
#define CHATMODE_NORMAL		1	// 일반 채팅. 
#define CHATMODE_SECRET		2	// 귓말 채팅. 
#define CHATMODE_TOWN		3	// 마을 공지. 
#define CHATMODE_HEAD		4	// 머릿말 
#define CHATMODE_HOME		5	// 주민 채팅. 
#define CHATMODE_ASK		6	// 질문. . 
#define CHATMODE_SYSTEM		8	// 시스템이 만드는 채팅. 
#define CHATMODE_VILLAGER	9	// 주민 공지. (주민 모두에게 공지. ) 
#define CHATMODE_PARTY		10	// 파티 채팅
#define CHATMODE_KING		11	// 군주 공지
#define CHATMODE_NEWSITEM	12	// 자막광고 ( 아이템 필요 )
//_LEON_GUILD_CHAT
#define CHATMODE_GUILD		13	// 길드 멤버 채팅
//#define CHATMODE_GUILDALL	14	// 길드 공지

class cltGameMsgRequest_Chat{
public:
	SI16 siDataSize;
	SI16 siChatMode;
	SI32 siSystemTxt;	// 시스템 메시지인 경우 그 txtmgr의 번호를 보내야 한다. 없으면 0

	TCHAR szChatText[256];

	cltGameMsgRequest_Chat(SI16 chatmode,SI32 systemtxt, TCHAR* chattext)
	{
		StringCchCopy( szChatText, 256, chattext );
	
		siChatMode = chatmode;
		siSystemTxt = systemtxt ;

		siDataSize = sizeof(SI16) + sizeof(SI16) + sizeof(SI32) + _tcslen( szChatText ) + 1;
	}
};

//----------------------------------------------------------------
// 채팅 응답. 
//----------------------------------------------------------------

class cltGameMsgResponse_Chat
{
public:
	SI16 siDataSize;
	SI16 siCharUnique;		// 채팅을 보낸 사용자의 CharUnique
	SI16 PlayerNameLength;
	SI16 siMode;			// 채팅 모드. 

	bool bGameMaster;		// 게임마스터 여부
	SI32 siSystemTxt;		// 시스템 txt
	SI32 siLevel;			// 레벨

	TCHAR szChatText[256];

	cltGameMsgResponse_Chat(SI16 charunique, SI16 mode, TCHAR* playername, TCHAR*text,bool GameMaster = false, SI32 level = 0 ,SI32 systemtxt = 0)
	{
		siDataSize = sizeof(SI16) + sizeof(SI16) + sizeof(SI16) + sizeof(SI16) + sizeof( BOOL ) + sizeof( SI32 )+ sizeof( SI32 );

		siCharUnique		= charunique;
		siMode				= mode;

		bGameMaster			= GameMaster;
		siLevel				= level ;
		siSystemTxt			= systemtxt ;


		// 플레이어 이름을 붙인다.
		memcpy( szChatText, playername, MAX_PLAYER_NAME );
		szChatText[ MAX_PLAYER_NAME - 1 ] = NULL;

		PlayerNameLength	= _tcslen( szChatText );

		memcpy( &szChatText[ PlayerNameLength ], text, 100 );
		szChatText[ PlayerNameLength + 100 ] = NULL;

		siDataSize += _tcslen( szChatText ) + 1;
	}
};



//----------------------------------------------------------------
// 서버의 일반적인 정보들을 통보 
//----------------------------------------------------------------

class cltGameMsgResponse_ServerInfo
{
public:
	
	SI16 siServiceArea;
	SI16 siServerUnique;
	bool bItemMallUseSwitch;

	bool bVillageWarSwitch;		// 공성전 시간대 
	bool bBonusTimeSwitch;		// 보너스 타임 여부. 
	bool bBonus2TimeSwitch;		// 제조 경험 보너스 타임 여부. 
	bool bSpecialQuestBonusTimeSwitch;
	bool bBroadcastSwitch;		// 방송중인지 여부. 
	SI08 siBestVillageLevel;	// 최고 마을 레벨 
	SI32 siNPCPriceRate;		// NPC 가격 비율. 	
	GMONEY siItemPriceIndex;	// 무언가
	
	bool bEventSwitch[MAX_EVENT_NUMBER];

	SI32 siMinimumTeacherLevel;

	SI16 siServiceAge;			// 한국 한게임용 변수 - 서버의 서비스 나이
	SI08 siEnablePVP;			// PVP가 가능한 서버인가? 1: 가능, 0: 불가능

	bool bAutoUnblockMode;		// AutoUnblock 실행 여부

	bool bVoteCampaign;			// 선거 유세기간임을 알림

	bool bEventToggleSwitch[EVENT_TOGGLE_SWITCH_MAX];	// 각종 제어관련 스위치

	cltGameMsgResponse_ServerInfo(SI16 servicearea, SI16 serverunique, bool bitemmall, bool villagewarswitch, 
		bool premiumswitch, bool bbonus2switch, bool bspecialquestbonus, bool bbroadcast, 
		SI08 villagelevel, SI32 npcpricerate, GMONEY ItemPriceIndex, bool* pevent, SI32 MinimumTeacherLevel,
		SI16 serviceage, SI08 enablepvp, bool AutoUnblockMode, bool voteCampaign, bool* pbEventToggleSwitch )
	{
		siServiceArea		= servicearea;
		siServerUnique		= serverunique;
		bItemMallUseSwitch	= bitemmall;
		bVillageWarSwitch	= villagewarswitch;
		bBonusTimeSwitch	= premiumswitch;
		bBonus2TimeSwitch	= bbonus2switch;
		bSpecialQuestBonusTimeSwitch	= bspecialquestbonus;
		bBroadcastSwitch	= bbroadcast;
		siBestVillageLevel	= villagelevel;
		siNPCPriceRate		= npcpricerate;
		siItemPriceIndex	= ItemPriceIndex;

		for(SI32 i = 0;i < MAX_EVENT_NUMBER;i++)
		{
			bEventSwitch[i] = pevent[i];	
		}

		siMinimumTeacherLevel	= MinimumTeacherLevel;

		siServiceAge			= serviceage;
		siEnablePVP				= enablepvp;

		bAutoUnblockMode		= AutoUnblockMode;
		bVoteCampaign			= voteCampaign;

		for ( SI32 siCount=0; siCount<EVENT_TOGGLE_SWITCH_MAX; ++siCount )
		{
			bEventToggleSwitch[siCount] = pbEventToggleSwitch[siCount];
		}
	}
};


//----------------------------------------------------------------
// 공성전의 시작과 종료를 알림
//----------------------------------------------------------------

class cltGameMsgResponse_WarVillage
{
public:
	// 공성전 시간대 
	bool bVillageWarSwitch;

	cltGameMsgResponse_WarVillage(bool villagewarswitch)
	{
		bVillageWarSwitch = villagewarswitch;
	}
};

//----------------------------------------------------------------
// 보너스 타임의 시작과 종료를 알림
//----------------------------------------------------------------
#define BONUSTIMEMODE_NORMAL		1		// 일반 보너스 타임. 
#define BONUSTIMEMODE_MAKE			2		// 제조  보너스 타임. 
#define BONUSTIMEMODE_SPECIALQUEST	3		// 흥부박 보너스 타임. 
class cltGameMsgResponse_BonusTime
{
public:
	SI16 siBonusTimeMode;
	bool bBonusTimeSwitch;

	cltGameMsgResponse_BonusTime(SI16 mode, bool bswitch)
	{
		siBonusTimeMode	= mode;
		bBonusTimeSwitch = bswitch;
	}
};

//----------------------------------------------------------------
// 육조 판서의 정보를 얻어온다.
//----------------------------------------------------------------
class cltGameMsgResponse_GetByungzoInfo{
public:
	CMinisterBase		MinisterBase;
	SI32				siVillageWarTime;	// 전쟁시간 

	SI32				siSetTime;				// 전쟁설정시간
	SI32				siWarStartTime;			// 전쟁시작시간
	SI16				siSetWarCount;			// 전쟁설정 횟수
	SI32				siBlackArmyNum;			// 전쟁 몬스터 설정 수

	cltGameMsgResponse_GetByungzoInfo(CMinisterBase *pMinister, SI32 vilagewartime,
								SI32 settime, SI32 warstarttime, SI16 setwarcount, UI32 blackarmynum)
	{
		MinisterBase.Set(pMinister);
		siVillageWarTime = vilagewartime;

		siSetTime = settime;
		siWarStartTime = warstarttime;
		siSetWarCount = setwarcount;
		siBlackArmyNum = blackarmynum;

	}

};

class cltGameMsgResponse_GetYezoInfo{
public:
	CMinisterBase		MinisterBase;
	SI32				siJapanSwordSolNum;	// 이도왜구 설정 수	KIND_JAPANSWORDSOL
	SI32				siJapanGunSolNum;	// 조총왜구 설정 수	KIND_JAPANGUNSOL
	SI32				siJapanCommanderNum;// 왜구장수 설정 수 KIND_JAPANCOMMANDER
	SI32				siJapanPorterNum;	// 짐꾼왜구 설정 수 KIND_JAPANPORTERSOL

	SI32				siWarVillageUnique; // 짐꾼왜구가 쳐들어갈 마을 유니크

	cltGameMsgResponse_GetYezoInfo(CMinisterBase *pMinister, SI32 swordsolnum, SI32 gunsolnum, SI32 commandsolnum,SI32 portersolnum,SI32 villageunique)
	{
		MinisterBase.Set(pMinister);
		siJapanSwordSolNum	= swordsolnum;	
		siJapanGunSolNum	= gunsolnum;	
		siJapanCommanderNum	= commandsolnum;
		siJapanPorterNum	= portersolnum;

		siWarVillageUnique	= villageunique;
	}
};

class cltGameMsgResponse_GetHozoInfo{
public:
	CMinisterBase		MinisterBase;
	SI32				siResidentNum;	// 주민수 
	SI32				siMaxShop;		// 마을 내 총 개인상점수

	SI32				siTotalCharNum; // 총 백성수
	SI32				siCalcTime;		// 총 백성수 계산한 시간

	GMONEY				siPrizeMoney; // 포상금
	SI32				siPrizeSetTime; // 설정 시간
	GMONEY				siFixationMoney;

	SI32				siNewCharRate;

	// 파산 마을 관련
	SI16				siBankruptcyVillageNumber;
	SI32				siSetBankruptcyVillageDate;

	SI16				siTutorialVillageDivideend;

	cltGameMsgResponse_GetHozoInfo(CMinisterBase *pMinister, SI32 residentNum,SI32 maxshop,
									SI32 totalcharnum,SI32 calctime,GMONEY prizemoney,
									SI32 prizesettime,GMONEY fixationmoney,SI32 newcharrate,
									SI16 BankruptcyVillageNumber, SI32 SetBankruptcyVillageDate,
									SI16 TutorialVillageDivideend)
	{
		MinisterBase.Set(pMinister);
		siResidentNum = residentNum;
		siMaxShop = maxshop ;

		siTotalCharNum = totalcharnum;
		siCalcTime = calctime;

		siPrizeMoney = prizemoney ;
		siPrizeSetTime = prizesettime ;
		siFixationMoney = fixationmoney ;

		siNewCharRate = newcharrate ;
		siBankruptcyVillageNumber = BankruptcyVillageNumber;
		siSetBankruptcyVillageDate = SetBankruptcyVillageDate;

		siTutorialVillageDivideend  = TutorialVillageDivideend;
	}
};

class cltGameMsgResponse_GetGoonzuInfo{
public:
	CMinisterBase		MinisterBase;
	
	INT64				iTotalFinances;

	SI16				siLiquidationVillageNumber;

	cltGameMsgResponse_GetGoonzuInfo(CMinisterBase *pMinister,SI64 finances, SI16 LiquidationVillageNumber )
	{
		MinisterBase.Set(pMinister);

		iTotalFinances = finances ;
		siLiquidationVillageNumber = LiquidationVillageNumber;		
	}
};

class cltGameMsgResponse_GetIzoInfo{
public:
	CMinisterBase		MinisterBase;

	SI32				siPriceTime;	// 설정 시간

	SI32				siPriceInx;		// 물품 평균 시세
	SI32				siPriceValue;	// 매입율
	GMONEY				siTotalMoney;	// 총통화량
	SI32				siCalcTime;		// 통화량 계산한 시간
	SI32				siTradeTax;		// 관세율
	GMONEY				siTradeLimitMoney; // 거래 최소가격

	bool				bWorldTradeTax;			// 무역_세금 적용 유무
	SI32				siSpecialTaxRange;		// 무역_특별세 적용범위( 100% ~ 150% )
	SI32				siSpecialTaxRate;		// 무역_특별세율( 10% ~ 100% )
	SI32				siMarketDownRate;		//  마켓 자동할인 시스템 할인율 ( 3% ~ 15% )
	SI16				siTinTin;				// 틴틴 추가 분해율

	cltGameMsgResponse_GetIzoInfo(CMinisterBase *pMinister, SI32 priceinx,SI32 pricevalue,SI32 pricetime,
								  SI64 money,SI32 calctime, SI32 tradetax,GMONEY tradelimitmoney,
								  bool worldtradetax, SI32 specialtaxrange, SI32 specialtaxrate,
								  SI32 marketdownrate, SI16 tintin )
	{
		MinisterBase.Set(pMinister);
		siPriceInx		= priceinx ;
		siPriceValue	= pricevalue;
		siPriceTime		= pricetime;
		siTotalMoney	= money;
		siCalcTime		= calctime;
		siTradeTax		= tradetax;
		siTradeLimitMoney = tradelimitmoney;

		bWorldTradeTax		= worldtradetax;
		siSpecialTaxRange	= specialtaxrange;
		siSpecialTaxRate	= specialtaxrate;

		siMarketDownRate	= marketdownrate;

		siTinTin			= tintin;
	}
};

class cltGameMsgResponse_GetGongzoInfo{
public:
	CMinisterBase		MinisterBase;

	SI32				siPriceTime;
	SI16				siLeftOverValue;
	SI16				siLeftOverValue_Armor;
	GongzoMember		stGongzoMember;

	cltGameMsgResponse_GetGongzoInfo(CMinisterBase *pMinister,SI32 pricetime,SI16 leftovervalue,GongzoMember *gongzomember, SI16 LeftOverValue_Armor )
	{
		MinisterBase.Set(pMinister);
		siPriceTime = pricetime ;
		siLeftOverValue = leftovervalue ;
		siLeftOverValue_Armor = LeftOverValue_Armor;

		for ( int i = GONGZO_SKILL_START ; i < GONGZO_SKILL_TOTAL ; i ++ )
		{
			stGongzoMember.siPriceValue[i] = gongzomember->siPriceValue[i] ;
			stGongzoMember.siSkillValue[i] = gongzomember->siSkillValue[i] ;
		}
		stGongzoMember.siCraftFund = gongzomember->siCraftFund;
	}
};

class cltGameMsgResponse_GetHyungzoInfo{
public:
	CMinisterBase		MinisterBase;

	SI16				siLimitStealNum;
	SI16				siSuspectTime;
	SI16				siPrisonTime;
	SI16				siPrisonerPenalty;

	HyungzoMember		stHyungzoMember[MAX_PRISON_LIST_NUM];

	cltGameMsgResponse_GetHyungzoInfo(CMinisterBase *pMinister,SI16 limitstealnum,SI16 suspecttime,
									  SI16 prisontime,SI16 prisonerpenalty,HyungzoMember *hyungzomember)
	{
		MinisterBase.Set(pMinister);

		siLimitStealNum		= limitstealnum ;
		siSuspectTime		= suspecttime ;
		siPrisonTime		= prisontime ;
		siPrisonerPenalty	= prisonerpenalty ;

		for ( int i = 0 ; i < MAX_PRISON_LIST_NUM ; i ++ )
		{
			stHyungzoMember[i].siDate = hyungzomember[i].siDate ;
			stHyungzoMember[i].siPrisonSecond = hyungzomember[i].siPrisonSecond ;
			memcpy(stHyungzoMember[i].szCharName,hyungzomember[i].szCharName,MAX_PLAYER_NAME);
			memcpy(stHyungzoMember[i].szPrisonReason,hyungzomember[i].szPrisonReason,MAX_PRISON_REASON_SIZE);
		}
	}
};

//----------------------------------------------------------------
// 육조 판서의 정보를 설정한다.
//----------------------------------------------------------------
class cltGameMsgRequest_SetByungzoInfo{
public:
	TCHAR				szGreeting[256];
	SI32				siVillageWarTime;	// 전쟁시간 

	SI32				siSetTime;				// 전쟁설정시간
	SI32				siWarStartTime;			// 전쟁시작시간
	SI16				siSetWarCount;			// 전쟁설정 횟수
	SI32				siBlackArmyNum;			// 전쟁 몬스터 설정 수

	cltGameMsgRequest_SetByungzoInfo(TCHAR  *greeting, SI32 vilagewartime,
				SI32 settime, SI32 warstarttime, SI16 setwarcount, SI32 blackarmynum)
	{
		StringCchCopy(szGreeting, 256, greeting);
		siVillageWarTime = vilagewartime;

		siSetTime = settime;
		siWarStartTime = warstarttime;
		siSetWarCount = setwarcount;
		siBlackArmyNum = blackarmynum;

	}
};

class cltGameMsgRequest_SetByungzoInfoVillageWarTime{
public:	
	TCHAR				szGreeting[256];
	SI32				siVillageWarTime;	// 전쟁시간 

	cltGameMsgRequest_SetByungzoInfoVillageWarTime(TCHAR * greeting,SI32 vilagewartime	)
	{
		StringCchCopy(szGreeting, 256, greeting);
		siVillageWarTime = vilagewartime;
	}
};
 
class cltGameMsgRequest_SetYezoInfo{
public:
	TCHAR				szGreeting[256];
	SI32				siJapanSwordSolNum;	// 이도왜구 설정 수	KIND_JAPANSWORDSOL
	SI32				siJapanGunSolNum;	// 조총왜구 설정 수	KIND_JAPANGUNSOL
	SI32				siJapanCommanderNum;// 왜구장수 설정 수 KIND_JAPANCOMMANDER
	SI32				siJapanPorterNum;	// 짐꾼왜구 설정 수 KIND_JAPANPORTERSOL

	SI32				siWarVillageUnique; // 짐꾼왜구가 쳐들어갈 마을 유니크

	cltGameMsgRequest_SetYezoInfo(TCHAR  *greeting, SI32 swordsolnum, SI32 gunsolnum, SI32 commandsolnum,SI32 portersolnum,SI32 villageunique)
	{
		StringCchCopy(szGreeting, 256, greeting);
		siJapanSwordSolNum	= swordsolnum;	
		siJapanGunSolNum	= gunsolnum;	
		siJapanCommanderNum	= commandsolnum;
		siJapanPorterNum	= portersolnum;

		siWarVillageUnique	= villageunique ;
	}
};

class cltGameMsgRequest_SetHozoInfo{
public:
	TCHAR				szGreeting[256];
	SI32				siResidentNum;	// 주민수
	SI32				siMaxShop; // 마을내 총 개인상점수

	GMONEY				siPrizeMoney; // 포상금
	GMONEY				siFixationMoney;

	GMONEY				siTutorialVillageDivideend;	// 앙카란 주식 배당금

	cltGameMsgRequest_SetHozoInfo(TCHAR  *greeting, SI32 residentNum,SI32 maxshop,
								  GMONEY prizemoney,GMONEY fixationmoney, GMONEY TutorialVillageDivideend)
	{
		StringCchCopy(szGreeting, 256, greeting);
		siResidentNum = residentNum;
		siMaxShop = maxshop ;

		siPrizeMoney = prizemoney ;
		siFixationMoney = fixationmoney ;

		siTutorialVillageDivideend = TutorialVillageDivideend;  
	}
};

class cltGameMsgRequest_SetGoonzuInfo{
public:
	TCHAR				szGreeting[256];

	cltGameMsgRequest_SetGoonzuInfo(TCHAR  *greeting)
	{
		StringCchCopy(szGreeting, 256, greeting);
	}
};

class cltGameMsgRequest_SetIzoInfo{
public:
	TCHAR				szGreeting[256];
	SI32				siPriceValue;			// 매입율
	bool				bWorldTradeTax;			// 무역_세금 적용 유무
	SI32				siSpecialTaxRange;		// 무역_특별세 적용범위( 100% ~ 150% )
	SI32				siSpecialTaxRate;		// 무역_특별세율( 10% ~ 100% )
	SI32				siMarketDownRate;		//  마켓 자동할인 시스템 할인율 ( 3% ~ 15% )
	SI32				siTinTin;				// 틴틴 추가 분해율


	cltGameMsgRequest_SetIzoInfo(TCHAR  *greeting,SI32 pricevalue, bool worldtradetax, 
		SI32 specialtaxrange, SI32 specialtaxrate, SI32 marketdownrate, SI32 tintin)
	{
		StringCchCopy(szGreeting, 256, greeting);
		siPriceValue	= pricevalue;

		bWorldTradeTax		= worldtradetax;
		siSpecialTaxRange	= specialtaxrange;
		siSpecialTaxRate	= specialtaxrate;
		siMarketDownRate	= marketdownrate;
		siTinTin			= tintin;
	}
};

class cltGameMsgRequest_SetGongzoInfo{
public:
	TCHAR				szGreeting[256];

	SI16				siLeftOverValue;
	SI16				siPriceValue[GONGZO_SKILL_TOTAL];
	SI16				siLeftOverValue_Armor;

	GMONEY				siCraftFund;

	cltGameMsgRequest_SetGongzoInfo(TCHAR  *greeting,SI16 leftovervalue,SI16 *pricevalue, GMONEY craftfund, SI32 leftovervalue_armor )
	{
		StringCchCopy(szGreeting, 256, greeting);

		siLeftOverValue = leftovervalue;
		siLeftOverValue_Armor = leftovervalue_armor;

		for ( int i = GONGZO_SKILL_START ; i < GONGZO_SKILL_TOTAL ; i ++ )
		{
			siPriceValue[i] = pricevalue[i] ;
		}
		siCraftFund = craftfund;
	}
};

class cltGameMsgRequest_SetHyungzoInfo{
public:
	TCHAR				szGreeting[256];

	SI16				siLimitStealNum;
	SI16				siSuspectTime;
	SI16				siPrisonTime;
	SI16				siPrisonerPenalty;

	cltGameMsgRequest_SetHyungzoInfo(TCHAR * greeting,SI16 limitstealnum,
									 SI16 suspecttime,SI16 prisontime,SI16 prisonerpenalty)
	{
		StringCchCopy(szGreeting, 256, greeting);

		siLimitStealNum		= limitstealnum ;
		siSuspectTime		= suspecttime ;
		siPrisonTime		= prisontime ;
		siPrisonerPenalty	= prisonerpenalty ;
	}
};

class cltGameMsgRequest_SetHyungzoPrison{
public:
	HyungzoMember			stHyungzoMember;

	cltGameMsgRequest_SetHyungzoPrison(HyungzoMember *hyungzomember)
	{
		memcpy(&stHyungzoMember,hyungzomember,sizeof(HyungzoMember));
	}
};

// 예조판서 책상에서 수호천사들을 설정하고 불러오고 한다.

class cltGameMsgRequest_SetAngelApplyer // 수호천사 등록 신청
{
public:
	bool	bAdmin ; // true면 신청 , false면 해지

	SI32	siIndex ; // -1 이면 신청자가 직접 신청한 것 , 예조판서가 해임시 사용

	cltGameMsgRequest_SetAngelApplyer(bool admin,SI32 index = -1 )
	{
		bAdmin = admin ;

		siIndex = index ;
	}
};

class cltGameMsgResponse_SetAngelApplyer // 수호천사 등록 신청 응답
{
public:
	SI32	siResult ; // 1이면 성공 , 0이면 실패

	bool	bAdmin ; // true면 신청에 대한 응답 , false면 해지에 대한 응답
	
	SI32	siIndex ; // -1 이면 신청자가 직업 신청한 것 , 예조판서가 해임시 사용

	cltGameMsgResponse_SetAngelApplyer(SI32 result,bool admin,SI32 index = -1 )
	{
		siResult = result ;

		bAdmin = admin;

		siIndex = index ;
	}
};

class cltGameMsgRequest_SetAngel // 수호천사 임명 및 해임
{
public:
	SI32	siIndex ; // 선택한 캐릭터 인덱스

	bool	bAdmin ; // true면 임명 , false면 해임

	cltGameMsgRequest_SetAngel(SI32 index,bool admin)
	{
		siIndex = index ;

		bAdmin = admin ;
	}
};

class cltGameMsgResponse_SetAngel // 수호천사 임명 및 해임
{
public:
	SI32	siResult ; // 1이면 성공 , 0이면 실패

	SI32	siIndex ; // 선택한 캐릭터 인덱스
	bool	bAdmin ; // true면 임명 , false면 해임

	cltGameMsgResponse_SetAngel(SI32 result,SI32 index, bool admin)
	{
		siResult = result ;

		siIndex = index ;
		bAdmin = admin;
	}
};

class cltGameMsgResponse_GetAngelList // 수호천사 목록 요청 응답
{
public:
	SI32	siResult;

	HozoAngelList	stHozoAngelList[MAX_HOZO_ANGELLIST];

	cltGameMsgResponse_GetAngelList(SI32 result,HozoAngelList * yezoangellist)
	{
		siResult = result ;

		for ( int i = 0 ; i < MAX_HOZO_ANGELLIST ; i ++ )
		{
			memcpy(stHozoAngelList[i].szAccountID,yezoangellist[i].szAccountID,32);
			memcpy(stHozoAngelList[i].szName,yezoangellist[i].szName,MAX_PLAYER_NAME);

			stHozoAngelList[i].siLevel = yezoangellist[i].siLevel ;
			stHozoAngelList[i].siFameLevel = yezoangellist[i].siFameLevel ;
			
			stHozoAngelList[i].siPersonID = yezoangellist[i].siPersonID ;
		}
	}
};

class cltGameMsgResponse_GetAngelApplyerList // 수호천사 신청자 목록 요청 응답
{
public:
	SI32	siResult;

	HozoAngelList	stHozoAngelApplyerList[MAX_HOZO_ANGELLIST];

	cltGameMsgResponse_GetAngelApplyerList(SI32 result,HozoAngelList * yezoangelapplyerlist)
	{
		siResult = result ;

		for ( int i = 0 ; i < MAX_HOZO_ANGELLIST ; i ++ )
		{
			memcpy(stHozoAngelApplyerList[i].szAccountID,yezoangelapplyerlist[i].szAccountID,32);
			memcpy(stHozoAngelApplyerList[i].szName,yezoangelapplyerlist[i].szName,MAX_PLAYER_NAME);

			stHozoAngelApplyerList[i].siLevel = yezoangelapplyerlist[i].siLevel ;
			stHozoAngelApplyerList[i].siFameLevel = yezoangelapplyerlist[i].siFameLevel ;

			stHozoAngelApplyerList[i].siPersonID = yezoangelapplyerlist[i].siPersonID ;
		}
	}
};

class cltGameMsgResponse_GetPoliceList
{
public:
	SI32	siResult;

	HyungzoPoliceList	stHyungPoliceList[MAX_HYUNGZO_POLICELIST];

	cltGameMsgResponse_GetPoliceList(SI32 result,HyungzoPoliceList * pHyungzoPoliceList)
	{
		siResult = result ;

		for ( int i = 0 ; i < MAX_HYUNGZO_POLICELIST ; i ++ )
		{
			memcpy(stHyungPoliceList[i].szAccountID,pHyungzoPoliceList[i].szAccountID,32);
			memcpy(stHyungPoliceList[i].szName,pHyungzoPoliceList[i].szName,MAX_PLAYER_NAME);

			stHyungPoliceList[i].siPersonID = pHyungzoPoliceList[i].siPersonID ;
			stHyungPoliceList[i].siLevel = pHyungzoPoliceList[i].siLevel ;
		}
	}
};

class cltGameMsgRequest_SetPolice	// 형조가 포교를 설정한다
{
public:
	TCHAR szPoliceName[ MAX_PLAYER_NAME ];	// 설정될 이름
	bool bSet;						// true : 설정 false : 삭제
	
	cltGameMsgRequest_SetPolice(TCHAR* pName, bool bFlag)
	{
		MStrCpy(szPoliceName, pName, MAX_PLAYER_NAME);
		bSet = bFlag;
	}

};

class cltGameMsgResponse_SetPolice	// 형조가 포교를 설정한다
{
public:
	SI32 siResult;
	TCHAR szPoliceName[ MAX_PLAYER_NAME ];	// 설정될 이름
	bool bSet;						// true : 설정 false : 삭제

	cltGameMsgResponse_SetPolice(SI32 result, TCHAR* pName, bool bFlag)
	{
		siResult = result;
		MStrCpy(szPoliceName, pName, MAX_PLAYER_NAME);
		bSet = bFlag;
	}
};

class cltGameMsgResponse_GetCriminalList
{
public:
	SI32	siResult;

	HyungzoCriminalList	stHyungzoCriminalList[MAX_HYUNGZO_CRIMINALLIST];

	cltGameMsgResponse_GetCriminalList(SI32 result,HyungzoCriminalList * pHyungzoCriminalList)
	{
		siResult = result ;

		for ( int i = 0 ; i < MAX_HYUNGZO_CRIMINALLIST ; i ++ )
		{
			memcpy(stHyungzoCriminalList[i].szAccountID,pHyungzoCriminalList[i].szAccountID,32);
			memcpy(stHyungzoCriminalList[i].szName, pHyungzoCriminalList[i].szName,MAX_PLAYER_NAME);
			memcpy(stHyungzoCriminalList[i].szReason, pHyungzoCriminalList[i].szReason, MAX_PRISON_REASON_SIZE);

			stHyungzoCriminalList[i].siPersonID = pHyungzoCriminalList[i].siPersonID ;
			stHyungzoCriminalList[i].siTime = pHyungzoCriminalList[i].siTime ;
		}
	}
};

class cltGameMsgRequest_SetCriminal	// 형조가 포교를 설정한다
{
public:
	TCHAR szCriminalName[ MAX_PLAYER_NAME ];	// 설정될 이름
	TCHAR szCriminalReason[ MAX_PRISON_REASON_SIZE ];	// 설정될 이유
	bool bSet;						// true : 설정 false : 삭제
	
	cltGameMsgRequest_SetCriminal(TCHAR* pName, TCHAR* pReason, bool bFlag)
	{
		MStrCpy(szCriminalName, pName, MAX_PLAYER_NAME);
		MStrCpy(szCriminalReason, pReason, MAX_PRISON_REASON_SIZE);
		bSet = bFlag;
	}
};

class cltGameMsgResponse_SetCriminal
{
public:
	SI16 siResult;

	cltGameMsgResponse_SetCriminal( SI16 result )
	{
		siResult = result;
	}
};

/*
class cltGameMsgResponse_SetPolice	// 형조가 포교를 설정한다
{
public:
	SI32 siResult;

	cltGameMsgResponse_SetPolice(SI32 result)
	{
		siResult = result;
	}
};*/

// 서버의 이벤트 정보를 클라이언트에게 보낸다. 
class cltGameMsgResponse_ServerEventInfo{
public:
	bool bEventSwitch[MAX_EVENT_NUMBER];

	cltGameMsgResponse_ServerEventInfo(bool* pevent)
	{
		SI32 i;

		for(i = 0;i < MAX_EVENT_NUMBER;i++)
		{
			bEventSwitch[i] = pevent[i];
		}
	}
};

class cltGameMsgResponse_GiftNT
{
public:
	TCHAR strGiftName[ 32 ];
	TCHAR strMasterNoticeMsg[ 128 ];
	cltGameMsgResponse_GiftNT( TCHAR *GiftName, TCHAR *MasterNoticeMsg )
	{
		MStrCpy( strGiftName, GiftName, 32 );
		MStrCpy( strMasterNoticeMsg, MasterNoticeMsg, 128 );
	}
};

class cltGameMsgRequest_WhisperChat_Reject
{
public:
	bool bWhisperChatReject;
	cltGameMsgRequest_WhisperChat_Reject( bool WhisperChatReject )
	{
		bWhisperChatReject = WhisperChatReject;
	}
};

class cltGameMsgResponse_WhisperChat_Reject
{
public:
	bool bWhisperChatReject;
	cltGameMsgResponse_WhisperChat_Reject( bool WhisperChatReject )
	{
		bWhisperChatReject = WhisperChatReject;
	}
};

class cltGameMsgRequest_PrivateTrade_Reject
{
public:
	bool bPrivateTradeReject;
	cltGameMsgRequest_PrivateTrade_Reject( bool PrivateTradeReject )
	{
		bPrivateTradeReject = PrivateTradeReject;
	}
};

class cltGameMsgResponse_PrivateTrade_Reject
{
public:
	bool bPrivateTradeReject;
	cltGameMsgResponse_PrivateTrade_Reject( bool PrivateTradeReject )
	{
		bPrivateTradeReject = PrivateTradeReject;
	}
};

class cltGameMsgRequest_PartyInvite_Reject
{
public:
	bool bPartyInviteReject;
	cltGameMsgRequest_PartyInvite_Reject( bool PartyInviteReject )
	{
		bPartyInviteReject = PartyInviteReject;
	}
};

class cltGameMsgRequest_FriendAdd_Reject
{
public:
	bool bFriendAddReject;
	cltGameMsgRequest_FriendAdd_Reject( bool FriendAddReject )
	{
		bFriendAddReject = FriendAddReject;
	}
};

// 보스 등장 알림 메시지. 
class cltGameMsgResponse_BossAppear
{
public:
	SI16	siBossKind;		// 보스 종류 
	SI32	siLeftSecond;	// 출현까지 남은 시간. 

	cltGameMsgResponse_BossAppear( SI16 bosskind, SI32 leftsecond)
	{
		siBossKind		= bosskind;
		siLeftSecond	= leftsecond;
	}
};

class cltGameMsgRequest_WhisperChat_Create
{
public:
	TCHAR strOtherCharName[ MAX_PLAYER_NAME ];
	cltGameMsgRequest_WhisperChat_Create( TCHAR *OtherCharName )
	{
		memcpy( strOtherCharName, OtherCharName, MAX_PLAYER_NAME - 1 );
		strOtherCharName [ MAX_PLAYER_NAME - 1 ] = NULL;
	}
};

class cltGameMsgResponse_WhisperChat_Create
{
public:
	SI32 siWhisperChatIndex;
	stWhisperChatMember sWhisperChatMember;
	
	cltGameMsgResponse_WhisperChat_Create( SI32 WhisperChatIndex, stWhisperChatMember *pWhisperChatMember )
	{
		siWhisperChatIndex = WhisperChatIndex;
		sWhisperChatMember.Set( pWhisperChatMember );
	}
};

class cltGameMsgRequest_WhisperChat_Invite
{
public:
	SI32 siWhisperChatIndex;
	TCHAR strInviteCharName[ MAX_PLAYER_NAME ];		// 초대할려는 사람의 캐릭터명
	
	cltGameMsgRequest_WhisperChat_Invite( SI32 WhisperChatIndex, TCHAR *InviteCharName )
	{
		siWhisperChatIndex = WhisperChatIndex;
		memcpy( strInviteCharName, InviteCharName, MAX_PLAYER_NAME - 1 );
		strInviteCharName[ MAX_PLAYER_NAME - 1 ] = NULL;
	}
};

class cltGameMsgResponse_WhisperChat_Invite
{
public:
	SI32 siWhisperChatIndex;

	cltGameMsgResponse_WhisperChat_Invite( SI32 WhisperChatIndex )
	{
		siWhisperChatIndex = WhisperChatIndex;
	}
};

class cltGameMsgRequest_WhisperChat_UserList
{
public:
	SI32 siWhisperChatIndex;
	
	cltGameMsgRequest_WhisperChat_UserList( SI32 WhisperChatIndex )
	{
		siWhisperChatIndex = WhisperChatIndex;
	}
};

class cltGameMsgResponse_WhisperChat_UserList
{
public:
	SI32 siWhisperChatIndex;

	stWhisperChatMember sWhisperChatMemberList[ MAX_WHISPERCHAT_OBJ_USERNUM ];

	cltGameMsgResponse_WhisperChat_UserList( SI32 WhisperChatIndex, stWhisperChatMember *pWhisperChatMemberList )
	{
		siWhisperChatIndex = WhisperChatIndex;
		memcpy ( sWhisperChatMemberList, pWhisperChatMemberList, sizeof( stWhisperChatMember ) * MAX_WHISPERCHAT_OBJ_USERNUM );
	}
};


class cltGameMsgRequest_WhisperChat_Out
{
public:
	SI32 siWhisperChatIndex;

	cltGameMsgRequest_WhisperChat_Out( SI32 WhisperChatIndex )
	{
		siWhisperChatIndex = WhisperChatIndex;
	}
	
};

class cltGameMsgResponse_WhisperChat_Out
{
public:
	SI32 siWhisperChatIndex;
	SI32 siPersonID;

	cltGameMsgResponse_WhisperChat_Out( SI32 WhisperChatIndex, SI32 PersonID )
	{
		siWhisperChatIndex = WhisperChatIndex;
		siPersonID = PersonID;
	}
};

class cltGameMsgRequest_Personal_Log
{
public:
	SI32				siLogType; // 로그의 종류

	TCHAR				szServerName[MAX_DBLOG_SIZE_SERVERNAME]; // 서버 이름
	TCHAR				szPersonName[MAX_PLAYER_NAME]; // 찾을 캐릭터의 이름

	SI32				siItemUnique;

	TCHAR				szDate[MAX_DBLOG_SIZE_DATE]; // 검색 시간
	TCHAR				szStartTime[MAX_DBLOG_SIZE_DATE]; // 검색할 시간
	TCHAR				szEndTime[MAX_DBLOG_SIZE_DATE]; // 검색 끝낼 시간

	cltGameMsgRequest_Personal_Log( SI32 logtype,TCHAR * server, TCHAR * person,
		SI32 itemunique,TCHAR * date, TCHAR * start, TCHAR * end)
	{
		siLogType = logtype;
		
		memcpy(szServerName,server,MAX_DBLOG_SIZE_SERVERNAME);
		memcpy(szPersonName,person,MAX_PLAYER_NAME);

		siItemUnique = itemunique ;

		memcpy(szDate,date,MAX_DBLOG_SIZE_DATE);
		memcpy(szStartTime,start,MAX_DBLOG_SIZE_DATE);
		memcpy(szEndTime,end,MAX_DBLOG_SIZE_DATE);
	}
};

class cltGameMsgResponse_Personal_Log
{
public:
	SI32			siResult; // 1이면 계속 -1이면 끝
	stLOGData		stLogData[MAX_STLOGDATA_PER_PACKET];

	cltGameMsgResponse_Personal_Log( SI32 Result,stLOGData * logdata )
	{
		siResult = Result ;
		for ( int i = 0 ; i < MAX_STLOGDATA_PER_PACKET ; i ++ )
			stLogData[i].Set(&logdata[i]);
	}
};

//-----------------------------------------
// 불가사리 사냥 정보. 
//-----------------------------------------
class cltGameMsgResponse_HuntingInfo
{
public:
	SI16	siVillageUnique;		// 이벤트 참여 마을. 
	SI32	siLeftSecond;			// 남은 시간. 
	
	SI32	siMode;					// 승리(1), 패배(-1), 진행(0)


	cltGameMsgResponse_HuntingInfo(SI16 villageunique, SI32 leftsecond, SI16 mode)
	{
		siVillageUnique	= villageunique;
		siLeftSecond	= leftsecond;
		siMode			= mode;	
	}
};

//-----------------------------------------
// 불가사리 사냥 보상 정보. 
//-----------------------------------------
class cltGameMsgResponse_HuntingReward
{
public:
	SI32	siExp;				// 획득 경험치. 
	SI32	siFameExp;			// 획득 명성 경험치.
	SI32	siRewardItemUnique;	// 얻은 아이템 유니크
	SI32	siRewardItemNum;	// 얻은 아이템 갯수

	 cltGameMsgResponse_HuntingReward(SI32 exp, SI32 fameexp, SI32 rewarditemunique = 0, SI32 rewarditemnum = 0)
	{
		siExp				= exp;
		siFameExp			= fameexp;
		siRewardItemUnique	= rewarditemunique;
		siRewardItemNum		= rewarditemnum;
	}
};


//-----------------------------------------
// 고구려 유적 방어 정보. 
//-----------------------------------------
class cltGameMsgResponse_GoguInfo
{
public:
	SI16	siVillageUnique;		// 이벤트 참여 마을. 
	SI32	siLeftSecond;			// 남은 시간. 
	
	SI32	siMode;					// 승리(1), 패배(-1), 진행(0)


	cltGameMsgResponse_GoguInfo(SI16 villageunique, SI32 leftsecond, SI16 mode)
	{
		siVillageUnique	= villageunique;
		siLeftSecond	= leftsecond;
		siMode			= mode;	
	}
};

//-----------------------------------------
// 고구려 유적 방어 보상 
//-----------------------------------------
class cltGameMsgResponse_GoguReward
{
public:
	SI32	siExp;				// 획득 경험치. 
	SI32	siFameExp;			// 획득 명성 경험치. 
	GMONEY	siMoney;			// 획득 자금 

	cltGameMsgResponse_GoguReward(SI32 exp, SI32 fameexp, GMONEY money)
	{
		siExp			= exp;
		siFameExp		= fameexp;
		siMoney			= money;
	}
};

//-----------------------------------------
// 오사카성 공격 정보. 
//-----------------------------------------
class cltGameMsgResponse_OsakaInfo
{
public:
	SI32	siMapIndex;
	SI32	siLeftSecond;			// 남은 시간. 
	SI16	siMode;					// 승리(1), 패배(-1), 진행(0)
	
	cltGameMsgResponse_OsakaInfo(SI32 mapindex, SI32 leftsecond, SI16 mode)
	{
		siMapIndex		= mapindex;
		siLeftSecond	= leftsecond;
		siMode			= mode;	
	}
};

//-----------------------------------------
// 오사카성 공격 보상 
//-----------------------------------------
class cltGameMsgResponse_OsakaReward
{
public:
	SI32	siExp;				// 획득 경험치. 
	SI32	siFameExp;			// 획득 명성 경험치. 
	GMONEY	siMoney;			// 획득 자금 

	cltGameMsgResponse_OsakaReward(SI32 exp, SI32 fameexp, GMONEY money)
	{
		siExp			= exp;
		siFameExp		= fameexp;
		siMoney			= money;
	}
};

//-----------------------------------------
// 강화도 공격 정보. 
//-----------------------------------------
class cltGameMsgResponse_KanghwadoInfo
{
public:
	SI32	siVillageUnique;
	SI32	siLeftSecond;			// 남은 시간. 
	SI16	siMode;					// 승리(1), 패배(-1), 진행(0)


	cltGameMsgResponse_KanghwadoInfo(SI32 villageunique, SI32 leftsecond, SI16 mode)
	{
		siVillageUnique	= villageunique;
		siLeftSecond	= leftsecond;
		siMode			= mode;	
	}
};

//-----------------------------------------
// 강화도 공격 보상 
//-----------------------------------------
class cltGameMsgResponse_KanghwadoReward
{
public:
	SI32	siExp;				// 획득 경험치. 
	SI32	siFameExp;			// 획득 명성 경험치. 
	GMONEY	siMoney;			// 획득 자금 

	cltGameMsgResponse_KanghwadoReward(SI32 exp, SI32 fameexp, GMONEY money)
	{
		siExp			= exp;
		siFameExp		= fameexp;
		siMoney			= money;
	}
};

class cltGameMsgRequest_InvestmentGetInfo
{
public:
	SI32		siNPCCharUnique;

	cltGameMsgRequest_InvestmentGetInfo(SI32 npcunique)
	{
		siNPCCharUnique = npcunique ;
	}
};

class cltGameMsgResponse_InvestmentGetInfo
{
public:
	InvestmentLines		stInvestmentLines[MAX_INVESTMENT_ITEM_NUMBER];

	cltGameMsgResponse_InvestmentGetInfo(InvestmentLines *investmentlines)
	{
		for ( SI32 i = 0 ; i < MAX_INVESTMENT_ITEM_NUMBER ; i ++ )
		{
			memcpy(&stInvestmentLines[i],&investmentlines[i],sizeof(InvestmentLines)) ;
		}
	}
};

class cltGameMsgResponse_ItemInfoGet
{
public:
	cltItem		clItem;

	cltGameMsgResponse_ItemInfoGet(cltItem* pclitem)
	{
		clItem.Set(pclitem);
	}
};

class cltGameMsgRequest_AutoHunting
{
public:
	TCHAR	szSuspectName[MAX_PLAYER_NAME]; // 용의자 이름

	cltGameMsgRequest_AutoHunting(TCHAR * suspect)
	{
		MStrCpy(szSuspectName, suspect, MAX_PLAYER_NAME );
	}
};

class cltGameMsgResponse_AutoHunting
{
public:
	SI32	siTimeLimit;
	SI32	siSlot;
	SI16	siPrisonTime;
	SI16	siPrisonerPenalty;

	TCHAR	szRequestTxt[256];

	cltGameMsgResponse_AutoHunting(SI32 timelimit,SI32 slot,
								   SI16 prisontime,SI16 prisonerpenalty,const TCHAR* txt)
	{
		siTimeLimit = timelimit;
		siSlot = slot;
		siPrisonTime = prisontime;
		siPrisonerPenalty = prisonerpenalty;

		StringCchCopy(szRequestTxt, 256, txt);
	}
};

class cltGameMsgRequest_AutoHunting_Ask
{
public:
	SI32		siSlot;

	TCHAR		szAnswer[256];

	cltGameMsgRequest_AutoHunting_Ask(SI32 slot,const TCHAR* txt)
	{
		siSlot = slot;

		StringCchCopy(szAnswer, 256, txt);
	}

};

class cltGameMsgResponse_AutoHunting_Ask
{
public:
	bool		bSuccess;
	SI32		siLeft;
	SI16		siPrisonerPenalty;

	cltGameMsgResponse_AutoHunting_Ask(bool success,SI32 left,SI16 prisonerpenalty)
	{
		bSuccess = success ;

		siLeft = left;
		siPrisonerPenalty = prisonerpenalty ;
	}
};

class cltGameMsgResponse_AutoHunting_TimeUpdate
{
public:
	SI32		siMinusTime;
	SI32		siTotalTime;

	cltGameMsgResponse_AutoHunting_TimeUpdate(SI32 minustime,SI32 totaltime)
	{
		siMinusTime = minustime ;
		siTotalTime = totaltime ;
	}
};

// 추천인 포상을 받았음을 전체 공지. 
class cltGameMsgResponse_RecommendReward
{
public:

	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 수령자. 
	SI16	siLevel;						// 레벨 얼마가 되어 포상을 받았는가. 
	cltItem clItem;							// 보상품 정보. 
	SI32	siIncFameExp;
	GMONEY	siMoney;						// 포상금 (왕실자산에서 지급) 

	cltGameMsgResponse_RecommendReward(TCHAR* pname, SI16 level, cltItem* pclitem, SI32 incfameexp, GMONEY money)
	{
		MStrCpy( szPersonName, pname, MAX_PLAYER_NAME );
		siLevel		= level;
		clItem.Set(pclitem);
		siIncFameExp = incfameexp;
		siMoney			= money;	
	}

};

// 명성 레벨 향상에 의한 포상 공지. 
class cltGameMsgResponse_FameLevelReward
{
public:

	TCHAR	szPersonName[MAX_PLAYER_NAME];	// 수령자. 
	SI16	siFameLevel;					// 명성레벨 얼마가 되어 포상을 받았는가. 
	cltItem clItem;							// 보상품 정보. 

	cltGameMsgResponse_FameLevelReward(TCHAR* pname, SI16 level, cltItem* pclitem)
	{
		MStrCpy( szPersonName, pname, MAX_PLAYER_NAME );
		siFameLevel		= level;
		clItem.Set(pclitem);
	}

};


class cltGameMsgResponse_AnotherAuth
{
public:

	TCHAR		szIP[64] ;
	SYSTEMTIME	stTime;
	BOOL		bNewUser;

	cltGameMsgResponse_AnotherAuth(TCHAR * ip , SYSTEMTIME time, BOOL NewUser)
	{
		bNewUser = NewUser;
		StringCchCopy(szIP, 64, ip);
		memcpy(&stTime,&time,sizeof(SYSTEMTIME));
	}
};

class cltGameMsgResponse_NotGamePotAuth
{
public:

	cltGameMsgResponse_NotGamePotAuth()
	{
	}
};

class cltGameMsgResponse_CannotLogin_Age
{
public:

	cltGameMsgResponse_CannotLogin_Age()
	{
	}
};

class cltGameMsgResponse_CannotLogin_Error
{
public:

	cltGameMsgResponse_CannotLogin_Error()
	{
	}
};

// 게임실행도중에도 나이제한 체크 - PCK : 2007.09.19
class cltGameMsgResponse_GameDisconnect_Age
{
public:

	cltGameMsgResponse_GameDisconnect_Age()
	{
	}
};

class cltGameMsgResponse_GetGameMaster_ServerInfo
{
public:
	GMServerInfo		stGMServerInfo;

	cltGameMsgResponse_GetGameMaster_ServerInfo(GMServerInfo* gmserverinfo)
	{
		memcpy(&stGMServerInfo,gmserverinfo,sizeof(GMServerInfo));
	}
};

class cltGameMsgRequest_GetChangedName
{
public:
	SI32		siPage;

	cltGameMsgRequest_GetChangedName(SI32 page)
	{
		siPage = page ;
	}
};

class cltGameMsgResponse_GetChangedName
{
public:
	SI32				siResult;
	SI32				siPage;

	ChangedNameChar		stChangedNameChar[MAX_HOZO_CHANGEDNAME_LIST];

	cltGameMsgResponse_GetChangedName(SI32 result,SI32 page,ChangedNameChar * changednamechar)
	{
		siResult = result;
		siPage = page;

		//memcpy(stChangedNameChar,changednamechar,sizeof(ChangedNameChar) * MAX_HOZO_CHANGEDNAME_LIST );
		for ( int i = 0 ; i < MAX_HOZO_CHANGEDNAME_LIST ; i ++ )
		{
			stChangedNameChar[i].siDate = changednamechar[i].siDate ;

			MStrCpy(stChangedNameChar[i].szOldName, changednamechar[i].szOldName, MAX_PLAYER_NAME );
			MStrCpy(stChangedNameChar[i].szNewName, changednamechar[i].szNewName, MAX_PLAYER_NAME );
		}
	}
};


//------------------------------------------------------------------------
// 개인상점 제조의뢰 내역 클라이언트로 통보 
//------------------------------------------------------------------------
class cltGameMsgResponse_PersonalShopMakeInfo
{
public:
	TCHAR szName[MAX_PLAYER_NAME];
	SI16 siItemUnique;
	SI32 siItemNum;
	GMONEY siFee;
	SI32 siSkillLevel;
	SI32 siHand;

	cltGameMsgResponse_PersonalShopMakeInfo(TCHAR* pname, SI16 itemunique, SI32 itemnum, GMONEY fee, SI32 skilllevel, SI32 hand)
	{
		StringCchCopy(szName, MAX_PLAYER_NAME, pname);
		siItemUnique	= itemunique;
		siItemNum		= itemnum;
		siFee			= fee;
		siSkillLevel	= skilllevel;
		siHand			= hand;
	}
};

//------------------------------------
//중요한 공지를 받았다.
//-------------------------------------
class cltGameMsgResponse_NoticeAdmin
{
public:
	TCHAR szText[256] ;

	cltGameMsgResponse_NoticeAdmin(TCHAR * txt)
	{
		MStrCpy(szText , txt , 256) ;
	}
};

//--------------------------
// 학교 이벤트
//-----------------------
class cltGameMsgRequest_SetSchoolEvent
{
public:
	SI32 siUnique;
	SI32 siGrade;
	SI32 siClass;

	cltGameMsgRequest_SetSchoolEvent(SI32 _unique , SI32 _grade , SI32 _class)
	{
		siUnique = _unique ;
		siGrade = _grade ;
		siClass = _class ;
	}
};

//--------------------------------
// 인스턴스 던전 공지
//---------------------------------
class cltGameMsgResponse_InstanceMapInfo
{
public:
	SI32 siLeftTime;
	SI32 siLeftMon;

	cltGameMsgResponse_InstanceMapInfo(SI32 lefttime,SI32 leftmon)
	{
		siLeftTime = lefttime ;
		siLeftMon = leftmon ;
	}
};

class cltGameMsgRequest_Goonzu_SetMinister
{
public:
	SI32	siMinisterInx;
	TCHAR	szCharName[MAX_PLAYER_NAME];

	bool	bLeave;

	cltGameMsgRequest_Goonzu_SetMinister(SI32 index,TCHAR* name,bool leave)
	{
		siMinisterInx = index ;
		MStrCpy(szCharName,name,MAX_PLAYER_NAME);

		bLeave = leave ; // true면 해임시키기 , false면 임명하기
	}
};

class cltGameMsgResponse_Goonzu_SetMinister
{
public:
	SI32	siResult; // 1만 성공 나머지는 실패

	SI32	siMinisterInx;
	TCHAR	szCharName[MAX_PLAYER_NAME];

	cltGameMsgResponse_Goonzu_SetMinister(SI32 result,SI32 index , TCHAR* name)
	{
		siResult = result;

		siMinisterInx = index ;
		MStrCpy(szCharName,name,MAX_PLAYER_NAME);
	}
};

class cltGameMsgRequest_Goonzu_SetLiquidation
{
public:
	SI32	siLiquidationVillageNumber;

	cltGameMsgRequest_Goonzu_SetLiquidation( SI32 LiquidationVillageNumber )
	{
		siLiquidationVillageNumber = LiquidationVillageNumber;
	}
};



class cltGameMsgRequest_Goonzu_GetBonusTime
{
public:
	cltGameMsgRequest_Goonzu_GetBonusTime()
	{		
	}
};

class cltGameMsgResponse_Goonzu_GetBonusTime
{
public:
	SI16 siBonusTime;
	SI16 siMakeBonusTime;
	
	bool bChangeAble;

	cltGameMsgResponse_Goonzu_GetBonusTime(SI16 BonusTime , SI16 MakeBonusTime, bool ChangeAble )
	{		
		siBonusTime = BonusTime;
		siMakeBonusTime = MakeBonusTime;
		bChangeAble = ChangeAble;
	}
};

class cltGameMsgRequest_Goonzu_SetBonus
{
public:
	SI16 siBonusTime;	
	SI16 siMakeBonusTime;

	cltGameMsgRequest_Goonzu_SetBonus( SI16 BonusTime , SI16 MakeBonusTime )
	{		
		siBonusTime = BonusTime;
		siMakeBonusTime = MakeBonusTime;
	}
};

class cltGameMsgResponse_Goonzu_SetBonus
{
public:
	cltGameMsgResponse_Goonzu_SetBonus()
	{		
	}
};


class cltGameMsgRequest_BlackArmy_Apply
{
public :
	cltGameMsgRequest_BlackArmy_Apply()
	{
	}
	
};

class cltGameMsgResponse_BlackArmy_Apply
{
public :
	SI32 siResult; 
	SI32 siApplyUnique;

	cltGameMsgResponse_BlackArmy_Apply(SI32 Result, SI32 ApplyUnique)
	{
		siResult = Result;
		siApplyUnique = ApplyUnique;
	}
};

class cltGameMsgRequest_BlackArmy_RankInfo
{
public :
	SI32 siWarMapIndex;

	cltGameMsgRequest_BlackArmy_RankInfo(SI32 warmapindex)
	{
		siWarMapIndex = warmapindex;
	}

};

class cltGameMsgResponse_BlackArmy_RankInfo
{
public :
	SI32 siResult; 
	SI32 siWarMapIndex;
	SI32 siPoints[MAX_WARRANK_PERSON_PER_MAP];
	TCHAR szPlayes[MAX_WARRANK_PERSON_PER_MAP][MAX_PLAYER_NAME];

	cltGameMsgResponse_BlackArmy_RankInfo(SI32 warmapindex, SI32 *points, TCHAR **players)
	{
		siWarMapIndex = warmapindex;
		memcpy(siPoints, points, sizeof(SI32) * MAX_WARRANK_PERSON_PER_MAP);
		memcpy(szPlayes, players, sizeof(TCHAR) * MAX_WARRANK_PERSON_PER_MAP * MAX_PLAYER_NAME);
	}
};

class cltGameMsgRequest_BlackArmy_WarpToGate
{
public :
	SI32 siGateUnique;

	cltGameMsgRequest_BlackArmy_WarpToGate(SI32 gateunique)
	{
		siGateUnique = gateunique;
	}

};

class cltGameMsgResponse_BlackArmy_MVPEffect
{
public :
	SI16 siCharUnique;		// MVP가 된 캐릭터

	cltGameMsgResponse_BlackArmy_MVPEffect(SI16 charunique)
	{
		siCharUnique = charunique;
	}
};

class cltGameMsgResponse_ServerTime
{
public :
	_SYSTEMTIME sTime;

	cltGameMsgResponse_ServerTime(_SYSTEMTIME time)
	{
		sTime = time;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_Hit
//
// Last Modified 	: 2007 / 03 / 09
// Created		 	: 김광명
//
// Function			: 클라이언트에서 타격요청을 서버로 한다.
//
//***************************************************************************************************
class cltGameMsgRequest_Hit
{
public :

	// 타격을 시도하는 캐릭터
	SI32				siCharID;
	// 공격을 당할 대상
	cltAttackTarget		clAttackTarget;

	cltGameMsgRequest_Hit( SI32 CharID, cltAttackTarget* pAttackTarget ) 
	{
		siCharID = CharID;
		clAttackTarget.Set( pAttackTarget );
	}
	~cltGameMsgRequest_Hit()
	{
	}

	SI32 GetCharID()
	{
		return siCharID;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_SetBullet
//
// Last Modified 	: 2007 / 03 / 09
// Created		 	: 김광명
//
// Function			: 클라이언트에서 서버로 원거리 공격의 요청을 한다.
//
//***************************************************************************************************
class cltGameMsgRequest_SetBullet
{
public :

	// 타격을 시도하는 캐릭터
	SI32				siCharID;
	SI32				siBulletUnique;
	SI32				siMagicKind;
	SI32				siPower;
	cltPos				clTileStartPos;
	cltPos				clDotStartPos;
	cltAttackTarget		clAttackTarget;
	cltPos				clDotDestPos;

	cltGameMsgRequest_SetBullet( SI32  CharID,
		SI32  BulletUnique,
		SI32  MagicKind,
		SI32  Power,
		const cltPos* pTileStartPos, 
		const cltPos* pDotStartPos, 
		const cltAttackTarget* pAttackTarget, 
		const cltPos* pDotDestPos ) 
	{
		siCharID	   = CharID;
		siBulletUnique = BulletUnique;
		siMagicKind	   = MagicKind;
		siPower		   = Power;

		clTileStartPos.Set( pTileStartPos->siX, pTileStartPos->siY );
		clDotStartPos.Set( pDotStartPos->siX, pDotStartPos->siY );
		clAttackTarget.Set( pAttackTarget );
		clDotDestPos.Set( pDotDestPos->siX, pDotDestPos->siY );
	}
};

class cltGameMsgResponse_KingGuriInfo
{
public:

	SI32	siCmd;	// 현재의 상황 0:시작 1: 진행 2:끝
	DWORD	dwLeftTime;
	TCHAR	szAreaNmae[MAX_KINGGURI_NUM][32];
	TCHAR	szUserName[MAX_PLAYER_NAME];
	
	/// 나올 좌표
	POINT m_ptPosition[MAX_POSITION_NUM];
	
	//[추가 : 황진성 2007. 11. 13 대왕 구리댁이 나온 지역의 인덱스번호]
	SI32 m_siAreaIndex[MAX_KINGGURI_NUM];

	//[추가 : 황진성 2007. 11. 13 대왕 구리댁을 죽이고 나온 아이템 정보와 유저 정보.]
	TCHAR szBuffer[MAX_MESSAGE_SIZE];

	cltGameMsgResponse_KingGuriInfo( SI32 CMD, DWORD LeftTime, TCHAR AreaName[MAX_KINGGURI_NUM][32], TCHAR* UserName, POINT ptPosition[MAX_POSITION_NUM], SI32 siAreaIndex[MAX_KINGGURI_NUM])
	{

		siCmd = CMD;
		dwLeftTime = LeftTime;
		for( SI32 i = 0; i < MAX_KINGGURI_NUM; ++i )
		{
			StringCchCopy( szAreaNmae[i], 32, AreaName[i] );
		}

		if( NULL == UserName)
		{
			StringCchCopy( szUserName, MAX_PLAYER_NAME, "" );
		}
		else
		{
			StringCchCopy( szUserName, MAX_PLAYER_NAME, UserName );
		}
		
		memcpy(m_ptPosition,  ptPosition,  sizeof(m_ptPosition));
		memcpy(m_siAreaIndex, siAreaIndex, sizeof(m_siAreaIndex));

	}

	cltGameMsgResponse_KingGuriInfo( SI32 _CMD, TCHAR _Buffer[MAX_MESSAGE_SIZE])
	{
		siCmd = _CMD;
		StringCchCopy( szBuffer, MAX_MESSAGE_SIZE, _Buffer );
	}
};

//[추가 : 황진성 2007. 11. 29 산타라쿤 이벤트 메시지 추가.]
class cltGameMsgResponse_SantaRaccoonInfo
{
public:

	SI32	siCmd;	// 현재의 상황 0:시작 1: 진행 2:끝
	DWORD	dwLeftTime;
	TCHAR	szAreaNmae[MAX_SANTARACCOON_NUM][32];
	TCHAR	szUserName[MAX_PLAYER_NAME];

	/// 나올 좌표
	POINT m_ptPosition[MAX_SANTARACCOON_POSITION_NUM];

	//[추가 : 황진성 2007. 11. 29 산타라쿤이 나온 지역의 인덱스번호]
	SI32 m_siAreaIndex[MAX_SANTARACCOON_POSITION_NUM];

	//[추가 : 황진성 2007. 11. 29 산타라쿤을 죽이고 나온 아이템 정보와 유저 정보.]
	TCHAR szBuffer[MAX_SANTARACCOON_MESSAGE_SIZE];

	cltGameMsgResponse_SantaRaccoonInfo( SI32 CMD, DWORD LeftTime, TCHAR AreaName[MAX_SANTARACCOON_NUM][32], TCHAR* UserName, POINT ptPosition[MAX_SANTARACCOON_POSITION_NUM], SI32 siAreaIndex[MAX_SANTARACCOON_POSITION_NUM])
	{
		siCmd = CMD;
		dwLeftTime = LeftTime;
		for( SI32 i = 0; i < MAX_SANTARACCOON_NUM; ++i )
		{
			StringCchCopy( szAreaNmae[i], 32, AreaName[i] );
		}

		if( NULL == UserName)
		{
			StringCchCopy( szUserName, MAX_PLAYER_NAME, "" );
		}
		else
		{
			StringCchCopy( szUserName, MAX_PLAYER_NAME, UserName );
		}

		memcpy(m_ptPosition,  ptPosition,  sizeof(m_ptPosition));
		memcpy(m_siAreaIndex, siAreaIndex, sizeof(m_siAreaIndex));

	}

	cltGameMsgResponse_SantaRaccoonInfo( SI32 _CMD, TCHAR _Buffer[MAX_SANTARACCOON_POSITION_NUM])
	{
		siCmd = _CMD;
		StringCchCopy( szBuffer, MAX_SANTARACCOON_POSITION_NUM, _Buffer );
	}
};

class cltGameMsgResponse_KiteRaccoonInfo
{
public:
	SI08	m_siType;						// KITERACCOON_EVENT_START, KITERACCOON_EVENT_KILL, KITERACCOON_EVENT_END 
	SI32	m_siMonsterKind;				// 몬스터 유니크
	SI32	m_siItemUnique;					// 아이템 유니크
	
	TCHAR	m_tszUserName[MAX_PLAYER_NAME];	// 케릭터 이름

	
	cltGameMsgResponse_KiteRaccoonInfo( SI08 siType, SI32 siMonsterKind=0, SI32 siItemUnique=0, TCHAR *ptszUserName=NULL )
	{
		m_siType			= siType;
		m_siMonsterKind		= siMonsterKind;
		m_siItemUnique		= siItemUnique;

		ZeroMemory( m_tszUserName, MAX_PLAYER_NAME );

		if ( ptszUserName )
		{
			StringCchCopy( m_tszUserName, MAX_PLAYER_NAME, ptszUserName );
		}
	}

};


class cltGameMsgRequest_FileCRCCheck
{
public:
	SI32			siCount;
	NCRCFileCheck	CheckFileList[MAX_CRCFILECHECK_COUNT];

	cltGameMsgRequest_FileCRCCheck()
	{
		ZeroMemory(this, sizeof(*this));
	}

	SI32 GetSize()
	{
		return sizeof(siCount) + sizeof(NCRCFileCheck) * siCount;
	}
};

class cltGameMsgResponse_FileCRCCheck
{
public:
	SI32	siResult;

	cltGameMsgResponse_FileCRCCheck( SI32 in_siResult )
	{
		siResult = in_siResult;
	}
};


//[진성] 서버 이벤트로 관련된 정보를 클라가 서버로 요청 패킷. => 2008-6-20
class cltGameMsgRequest_Server_Event_Rank_Info
{
public:
	cltGameMsgRequest_Server_Event_Rank_Info()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

//[진성] 서버 이벤트로 관련된 정보를 클라로 보낼 패킷. => 2008-6-19
class cltGameMsgResponse_Server_Event_Rank_Info
{
public:
	CNServerEvent_Rank_Record	m_clServerEvent_Rank_Record;
	SI16						m_ServerEventBufKind;				// 서버 이벤트 버프 적용 종류(0이면 없다, 0크면 있다.)

	cltGameMsgResponse_Server_Event_Rank_Info()
	{	
		ZeroMemory(this, sizeof(*this));
	}
	cltGameMsgResponse_Server_Event_Rank_Info(CNServerEvent_Rank_Record& _clServerEvent_Rank_Record, SI16 _ServerEventBufKind)
	{
		ZeroMemory(this, sizeof(*this));
		memcpy(&m_clServerEvent_Rank_Record, &_clServerEvent_Rank_Record, sizeof(m_clServerEvent_Rank_Record));

		m_ServerEventBufKind = _ServerEventBufKind;
	}
};


//[진성] 서버 이벤트로 서버랭킹 정보를 담아 클라에 보낼 패킷.(서버 정보 전체를 담는다) => 2008-6-19
class cltGameMsgResponse_Server_Event_Server_Info
{
public:
	CNServerRankInfo	m_clServerRankInfo[SERVER_MAX_NUM];
	
	cltGameMsgResponse_Server_Event_Server_Info(CNServerRankInfo& _clServerRankInfo)
	{
		memcpy(&m_clServerRankInfo, &_clServerRankInfo, sizeof(m_clServerRankInfo));
	}
};

//[진성] 서버 이벤트로 서버랭킹 정보를 담아 클라에 보낼 패킷.(서버 정보 전체를 담는다) => 2008-6-19
class cltGameMsgResponse_Server_Event_Daily_Server_Info
{
public:
	CNServerDailyRankInfo	m_clServerDailyRankInfo[SERVER_MAX_NUM];

	cltGameMsgResponse_Server_Event_Daily_Server_Info(CNServerDailyRankInfo& _clServerDailyRankInfo)
	{
		memcpy(&m_clServerDailyRankInfo, &_clServerDailyRankInfo, sizeof(m_clServerDailyRankInfo));
	}
};


//[진성] 서버 이벤트로 개인랭킹 정보를 담아 클라에 보낼 패킷.(개인 정보 전체를 담는다) => 2008-6-19
class cltGameMsgResponse_Server_Event_Personal_Info
{
public:
	CNPersonalRankInfo	m_clPersonalRankInfo[SERVER_MAX_NUM];

	cltGameMsgResponse_Server_Event_Personal_Info(CNPersonalRankInfo& _clPersonalRankInfo)
	{
		memcpy(&m_clPersonalRankInfo, &_clPersonalRankInfo, sizeof(m_clPersonalRankInfo));
	}
};

//[진성] 서버 이벤트로 개인랭킹 정보를 담아 클라에 보낼 패킷.(개인 정보 전체를 담는다) => 2008-6-19
class cltGameMsgResponse_Server_Event_InstanceMap_Bonus_Figure
{
public:
	SI16	m_siBonusFigure[4];

	cltGameMsgResponse_Server_Event_InstanceMap_Bonus_Figure(SI16*	_psiBonusFigure)
	{
		memcpy(&m_siBonusFigure, _psiBonusFigure, sizeof(m_siBonusFigure));
	}
};

// [영훈] 서버 이벤트 퀘스트 시작 정보를 요청함
class cltGameMsgRequest_Server_Event_Quest_Start
{
public:
	cltGameMsgRequest_Server_Event_Quest_Start()
	{
		ZeroMemory(this, sizeof(*this));
	}
};

// [영훈] 서버 이벤트 퀘스트 시작 정보를 보냄
class cltGameMsgResponse_Server_Event_Quest_Start
{
public:
	CNServerEvent_QuestData	m_clQuestData;		// 퀘스트 정보	
	SI16					m_siMaxCharacter;	// 이 이벤트의 몇개의 문자가 퀘스트에 포함되는가?
	SI32					m_siNowAmount;		// 현재 몇개의 필요 갯수를 채웠는가?
	bool					m_bCharacter[Const_ServerEvent_Max_QuestCharacter];
	
	cltGameMsgResponse_Server_Event_Quest_Start( CNServerEvent_QuestData* pclQuestData, SI16 siMaxCharacter, SI32 siNowAmount, bool bCharacter[] )
	{
		m_clQuestData.SetServerEvent_QuestData( pclQuestData );

		m_siMaxCharacter	= siMaxCharacter;
		m_siNowAmount		= siNowAmount;

		memcpy( m_bCharacter, bCharacter, sizeof(m_bCharacter) );
	}

};

// [영훈] 서버 이벤트 퀘스트가 진행되는 정보를 보냄
class cltGameMsgResponse_Server_Event_Quest_RunInfo
{
public:
	SI32	m_siLimiteMinute;		// 서버 이벤트 퀘스트가 몇분 남았는가?

	SI32	m_siNowAmount;			// 현재 증가횟수

	cltGameMsgResponse_Server_Event_Quest_RunInfo( SI32 siLimitMinute, SI32 siNowAmount )
	{
		m_siLimiteMinute	= siLimitMinute;
		m_siNowAmount		= siNowAmount;
	}
};

// [영훈] 서버 이벤트 퀘스트 클리어 상태를 보냄
class cltGameMsgResponse_Server_Event_Quest_ClearStatus
{
public:
	SI16	m_siMessageType;	// 현재 메시지가 어떤 상황에서 발생되는 메시지 인가? /* SERVEREVENT_MESSAGETYPE */
	SI16	m_siCharacterIndex;	// 퀘스트를 클리어 해서 얻는 문자

	cltGameMsgResponse_Server_Event_Quest_ClearStatus( SI16 siMessageType, SI16 siCharacterIndex=-1 )
	{
		m_siMessageType		= siMessageType;
		m_siCharacterIndex	= siCharacterIndex;
	}
	
};

// [영훈] 서버 이벤트 퀘스트 클리어 상태를 보냄
class cltGameMsgResponse_EventTime_Update
{
public:
	TCHAR			m_szEventName[48];
	bool			m_bEventActivated;

	cltGameMsgResponse_EventTime_Update( TCHAR* pszEventName, bool bEventActivated )
	{
		StringCchCopy(m_szEventName, sizeof(m_szEventName), pszEventName);
		m_bEventActivated	= bEventActivated;
	}

};

//-----------------------------------------------------------------------------------------------------
// ************************************프로세스 체크 구동방식******************************************
//
// 서버에서 Response를 통해 어떠한 실행타입 모듈이름을 보내주면
// 클라이언트에서 Request를 통해 실행 모듈 개수와 맥어드레스를 보내준다
// 일반적인 1.Request 2.Response가 아닌 1.Response 2.Request의 순서임
//-----------------------------------------------------------------------------------------------------


// [영훈] 프로세스 체크 응답
class cltGameMsgRequest_CheckProcess
{
public:
	TCHAR	m_szMacAddress[20];	// 멕 어드레스
	SI16	m_siProcessCount;	// 모듈 개수

	cltGameMsgRequest_CheckProcess( void )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_CheckProcess) );
	}

	cltGameMsgRequest_CheckProcess( TCHAR* pszMacAddress, SI16 siProcessCount )
	{
		StringCchCopy( m_szMacAddress, 20, pszMacAddress );
		
		m_siProcessCount = siProcessCount;
	}

};

// [영훈] 프로세스 체크 요청
class cltGameMsgResponse_CheckProcess
{
public:
	TCHAR	m_szModuleName[64];	// 체크하려는 모듈 이름
	SI16	m_siType;			// 체크 타입

	cltGameMsgResponse_CheckProcess( void )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_CheckProcess) );
	}

	cltGameMsgResponse_CheckProcess( TCHAR* pszModuleName, SI16 siType )
	{
		StringCchCopy( m_szModuleName, 64, pszModuleName );

		m_siType = siType;
	}

};

// [영훈] 프로세스 체크 결과값
class cltGameMsgResponse_CheckProcess_Result
{
public:
	TCHAR	m_szModuleName[64];	// 검사한 모듈이름(메크로 검사때만 쓰임)
	SI16	m_siMultiList[20];	// 멀티클라이언트 리스트
	SI16	m_siType;			// 체크 타입

	cltGameMsgResponse_CheckProcess_Result( void )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_CheckProcess_Result) );
	}

	cltGameMsgResponse_CheckProcess_Result( TCHAR* pszModuleName, SI16* psiMultiList, SI16 siType )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_CheckProcess_Result) );

		if ( pszModuleName )
		{
			StringCchCopy( m_szModuleName, 64, pszModuleName );
		}

		if ( psiMultiList )
		{
			memcpy( m_siMultiList, psiMultiList, sizeof(m_siMultiList) );
		}

		m_siType = siType;
	}

};

// [진성] 질문 리스트 요청. 
class cltGameMsgResponse_Question_List_Get
{
public:
	bool	m_bAleadyAnswer[ SENDMSG_LIST_NUM ];								// 답변자가 있는지 없는지.
	SI32	m_siQuestionPersonID[ SENDMSG_LIST_NUM ];							// 질문자 personID
	TCHAR	m_szQuestionTitle[ SENDMSG_LIST_NUM ][ TITLE_LETTER_NUM ];			// 제목
	TCHAR	m_szName[ SENDMSG_LIST_NUM ][ MAX_PLAYER_NAME ];

	cltGameMsgResponse_Question_List_Get( SI32* _psiQuestionPersonID, TCHAR** _pszQuestionTitle, TCHAR** _pszName, bool* _pbAleadyAnswer )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Question_List_Get) );

		memcpy(m_siQuestionPersonID, _psiQuestionPersonID,	sizeof(m_siQuestionPersonID));
		memcpy(m_szQuestionTitle,	 _pszQuestionTitle,		sizeof(m_szQuestionTitle)	);
		memcpy(m_szName,			 _pszName,				sizeof(m_szName)			);
		memcpy(m_bAleadyAnswer,		 _pbAleadyAnswer,		sizeof(m_bAleadyAnswer)		);
	}
};


// [진성] 질문 리스트 요청. 
class cltGameMsgRequest_Add_Question
{
public:
	TCHAR	m_szQuestionTitle[ TITLE_LETTER_NUM ];					// 제목
	TCHAR	m_szQuestionContents[ CONTENTS_LETTER_NUM ];			// 내용.

	cltGameMsgRequest_Add_Question( TCHAR* _pszQuestionTitle, TCHAR* _pszQuestionContents )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Add_Question) );

		memcpy(m_szQuestionTitle,	 _pszQuestionTitle,		sizeof(m_szQuestionTitle)	);
		memcpy(m_szQuestionContents, _pszQuestionContents,	sizeof(m_szQuestionContents));
	}
};

// [진성] 답변자 등록.
class cltGameMsgRequest_Add_Answer
{
public:
	SI32	m_siQuestionPersonID;

	cltGameMsgRequest_Add_Answer( SI32 _siQuestionPersonID )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Add_Answer) );

		m_siQuestionPersonID = _siQuestionPersonID;
	}
};

class cltGameMsgResponse_Add_Answer
{
public:
	TCHAR	m_szName[MAX_PLAYER_NAME];
	TCHAR	m_szQuestionTitle[ TITLE_LETTER_NUM ];					// 제목
	SI32	m_siQuestionPersonID;									// 질문자 personID
	
	cltGameMsgResponse_Add_Answer( const TCHAR* _pszName, TCHAR* _pszQuestionTitle, SI32 _siQuestionPersonID )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Add_Answer) );

		memcpy(m_szName,			_pszName,			sizeof(m_szName)		 );
		memcpy(m_szQuestionTitle,	_pszQuestionTitle,	sizeof(m_szQuestionTitle));
		
		m_siQuestionPersonID = _siQuestionPersonID;
	}
};

// [진성] 답변완료.
class cltGameMsgRequest_Add_Answer_Complete
{
public:
	SI32	m_siQuestionPersonID;									// 질문자 personID
    TCHAR	m_szQuestionTitle[ TITLE_LETTER_NUM ];					// 제목
	TCHAR	m_szQuestionContents[ CONTENTS_LETTER_NUM ];			// 내용.


	cltGameMsgRequest_Add_Answer_Complete( SI32 _siQuestionPersonID, TCHAR* _pszQuestionTitle, TCHAR* _pszQuestionContents )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Add_Answer_Complete) );

		m_siQuestionPersonID = _siQuestionPersonID;
		memcpy(m_szQuestionTitle,	 _pszQuestionTitle,		sizeof(m_szQuestionTitle)	);
		memcpy(m_szQuestionContents, _pszQuestionContents,	sizeof(m_szQuestionContents));
	}
};

// [진성] 완료된 답변 질문자에게 보내주기.
class cltGameMsgResponse_Add_Answer_Complete
{
public:
	SI32	m_siAnswerPersonID;										// 답변자 personID.
	TCHAR	m_szAnswerCharName[ CONTENTS_LETTER_NUM ];				// 답변자 이름.
	TCHAR	m_szQuestionTitle[ TITLE_LETTER_NUM ];					// 제목.
	TCHAR	m_szQuestionContents[ CONTENTS_LETTER_NUM ];			// 내용.
	
	cltGameMsgResponse_Add_Answer_Complete( SI32 _siAnswerPersonID, TCHAR* _szAnswerCharName, TCHAR* _pszQuestionTitle, TCHAR* _pszQuestionContents )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Add_Answer_Complete) );

		m_siAnswerPersonID = _siAnswerPersonID;
		memcpy(m_szAnswerCharName,	 _szAnswerCharName,		sizeof(m_szAnswerCharName)	);
		memcpy(m_szQuestionTitle,	 _pszQuestionTitle,		sizeof(m_szQuestionTitle)	);
		memcpy(m_szQuestionContents, _pszQuestionContents,	sizeof(m_szQuestionContents));
	}
};


// [진성] 답변완료.
class cltGameMsgRequest_Add_Answer_Complete_Result
{
public:
	bool	m_bResult;											// true : 만족 , false : 불만족

	SI32	m_siAnswerCharPersonID;									// 답변자 personID
	
	cltGameMsgRequest_Add_Answer_Complete_Result( bool	_bResult, SI32 _siAnswerCharPersonID )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Add_Answer_Complete_Result) );

		m_bResult		       = _bResult;
		m_siAnswerCharPersonID = _siAnswerCharPersonID;
	}
};

// [진성] 완료된 답변 질문자에게 보내주기.
class cltGameMsgResponse_Add_Answer_Complete_Result
{
public:
	bool	m_bResult;											// true : 만족 , false : 불만족
	
	SI32	m_siItemUnique;										// 받은 아이템 유니크.
	SI16	m_siItemNum;										// 아이템 개수.

	TCHAR	m_szQuestionCharName[ TITLE_LETTER_NUM ];			// 질문자 이름
		
	cltGameMsgResponse_Add_Answer_Complete_Result( bool _bResult, SI32 _siItemUnique, SI16 _siItemNum, TCHAR* _szQuestionCharName )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Add_Answer_Complete_Result) );
		
		m_bResult	   = _bResult;
		m_siItemUnique = _siItemUnique;	
		m_siItemNum	   = _siItemNum;	

		memcpy(m_szQuestionCharName, _szQuestionCharName,	sizeof(m_szQuestionCharName));
		

	}
};



// [진성] 질문 내용 요청. 
class cltGameMsgRequest_Question_Contents_Get
{
public:
	SI32	m_siPersonID;

	cltGameMsgRequest_Question_Contents_Get( SI32	_siPersonID )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Question_Contents_Get) );

		m_siPersonID = _siPersonID;
	}
};

// [진성] 질문 내용 주기. 
class cltGameMsgResponse_Question_Contents_Get
{
public:
	TCHAR	m_szQuestionContents[ CONTENTS_LETTER_NUM ];			// 내용.
	
	cltGameMsgResponse_Question_Contents_Get( TCHAR* _pszQuestionContents )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Question_Contents_Get) );

		memcpy(m_szQuestionContents, _pszQuestionContents,	sizeof(m_szQuestionContents));
	}
};


class cltGameResponse_SetMute
{
public:
    NTCHARStringName		m_kCharName;
	SYSTEMTIME				m_stMuteTime;
	bool					m_bMaster;

	cltGameResponse_SetMute( const TCHAR* pszCharName, SYSTEMTIME* pstMuteTime, bool bMaster)
	{
		m_kCharName		= pszCharName;
		memcpy(&m_stMuteTime, pstMuteTime, sizeof(m_stMuteTime));

		m_bMaster		= bMaster;
	}
};

class cltGameResponse_MuteList
{
public:
	NTCHARStringName	m_szPersonNames[100];		// 채팅블록 유저
	SI16				m_siCount;

	cltGameResponse_MuteList( )
	{
		ZeroMemory( this, sizeof(cltGameResponse_MuteList) );
	}
};


//[진성] 엘릭서 아이템 타입별 리스트 정보 요청.
class cltGameMsgRequest_Elixir_Npc_Item_Type_List_Get
{
public:
	SI32	m_siItemType;

	cltGameMsgRequest_Elixir_Npc_Item_Type_List_Get( SI32 _siItemType )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Elixir_Npc_Item_Type_List_Get) );

		m_siItemType = _siItemType;
	}
};

#define ELIXIR_LIST_NUM	48
class cltGameMsgResponse_Elixir_Npc_Item_Type_List_Get
{
public:
	SI32	m_siItemUnique[ELIXIR_LIST_NUM];
	SI32	m_siWaterNum[ELIXIR_LIST_NUM];

	cltGameMsgResponse_Elixir_Npc_Item_Type_List_Get()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Elixir_Npc_Item_Type_List_Get) );
	}

	cltGameMsgResponse_Elixir_Npc_Item_Type_List_Get( SI32*	_psiItemUnique, SI32* _psiWaterNum )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Elixir_Npc_Item_Type_List_Get) );
		
		memcpy(m_siItemUnique,	_psiItemUnique, sizeof(m_siItemUnique));
		memcpy(m_siWaterNum,	_psiWaterNum,	sizeof(m_siWaterNum));
	}
};


// 사용하는 장비류 아이템의 유니크.
class cltGameMsgRequest_Elixir_Npc_Change_Item
{
public:
	SI32	m_siItemUnique;
	SI32	m_siItemPos;

	cltGameMsgRequest_Elixir_Npc_Change_Item( SI32 _siItemUnique, SI32	_siItemPos )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Elixir_Npc_Change_Item) );

		m_siItemUnique  = _siItemUnique;
		m_siItemPos		= _siItemPos;
	}
};

//[진성] 엘릭서 사용 시작.
class cltGameMsgRequest_Elixir_Set
{
public:
	SI32				m_siElixirItemUnique;	// 사용하는 엘릭서 아이템의 유니크.
	SI32				m_siElixirItemPos;		// 사용하는 엘릭서 아이템의 인벤 위치.
	
	SI32				m_siWaterItemUnique;	// 사용하는 마력의 비약 아이템의 유니크.
	SI32				m_siWaterItemPos;		// 사용하는 마력의 비약 아이템의 인벤 위치.
	
	SI32				m_siWaterItemNum;		// 사용하는 마력의 비약 아이템의 개수.

	cltElixir_Info		m_pcltElixirInfo;		// 세팅할 엘릭서 스테이터스 정보.

	bool				m_bElixirStart;			// 엘릭서를 처음 사용하는가.	
	
	cltGameMsgRequest_Elixir_Set( cltElixir* _pclElixir )
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Elixir_Set) );
		
		m_pcltElixirInfo.m_siEndMinuteVary	= _pclElixir->GetMinuteVary();
		m_pcltElixirInfo.m_uiMultiple 		= _pclElixir->GetMultiple();
		m_pcltElixirInfo.m_siPoint	  		= _pclElixir->GetPoint();
		m_pcltElixirInfo.m_siStr	  		= _pclElixir->GetStr();
		m_pcltElixirInfo.m_siDex	  		= _pclElixir->GetDex();
		m_pcltElixirInfo.m_siVit	  		= _pclElixir->GetVit();
		m_pcltElixirInfo.m_siMag	  		= _pclElixir->GetMag();
		m_pcltElixirInfo.m_siHnd	  		= _pclElixir->GetHnd();
		m_pcltElixirInfo.m_siWis	  		= _pclElixir->GetWis();
	}
		
	cltGameMsgRequest_Elixir_Set(	cltElixir_Info*	_pcltElixirInfo,
									SI32 			_siElixirItemPos	= 0, 
									SI32 			_siElixirItemUnique	= 0,
									SI32 			_siWaterItemPos		= 0,  
									SI32 			_siWaterItemUnique	= 0,
									SI32 			_siWaterItemNum		= 0,									
									bool			_bElixirStart		= false		)
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_Elixir_Set) );

		_pcltElixirInfo ? memcpy( &m_pcltElixirInfo, _pcltElixirInfo, sizeof(m_pcltElixirInfo) ) : 0;

		m_siElixirItemPos    = _siElixirItemPos;   		
		m_siElixirItemUnique = _siElixirItemUnique;	

		m_siWaterItemPos    = _siWaterItemPos;   		
		m_siWaterItemUnique = _siWaterItemUnique;	

		m_siWaterItemNum	= _siWaterItemNum;

		m_bElixirStart = _bElixirStart;
	}
};

class cltGameMsgResponse_Elixir_Set
{
public:
	SI32				m_siCharUnique;
	cltElixir_Info		m_pcltElixirInfo;

	cltGameMsgResponse_Elixir_Set( SI32	_siCharUnique, cltElixir_Info* _pcltElixirInfo )
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Elixir_Set) );

		m_siCharUnique = _siCharUnique;
		memcpy( &m_pcltElixirInfo, _pcltElixirInfo, sizeof(m_pcltElixirInfo) );
	}
};




//	[종호_BALANCECURRENCY] 사용가능통화량 응답 Server -> Client
class cltGameMsgResponse_BalanceCanUseCurrency
{
public:
	GMONEY	m_siCanUseMoney;
	bool	m_bNeedShowMSG;	// 남은사용가능 금액을 클라이언트에서 보여줄것인가

	cltGameMsgResponse_BalanceCanUseCurrency(GMONEY siCanUseMoney, bool bNeedShowMSG = false)
	{
		ZeroMemory(this, sizeof(*this));

		m_siCanUseMoney = siCanUseMoney;
		m_bNeedShowMSG  = bNeedShowMSG;
	}
};
//[성웅] 라쿤이벤트 군주 이벤트로 변경
// 정보를 셋팅 한다 .
class cltGameMsgRequest_SetRaccoonEventToGoonzu
{
public:
	cGoonzuRaccoonEventData	m_GoonzuRaccoonEventData[MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT];

	cltGameMsgRequest_SetRaccoonEventToGoonzu(cGoonzuRaccoonEventData* pGoonzuRaccoonEventData)
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_SetRaccoonEventToGoonzu) );

		memcpy(m_GoonzuRaccoonEventData,	pGoonzuRaccoonEventData,	sizeof(m_GoonzuRaccoonEventData));
	}
};
//[성웅] 라쿤이벤트 군주 이벤트로 변경
class cltGameMsgResponse_SetRaccoonEventToGoonzu{
public:
	cGoonzuRaccoonEventData	m_GoonzuRaccoonEventData[MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT];

	cltGameMsgResponse_SetRaccoonEventToGoonzu(cGoonzuRaccoonEventData* pGoonzuRaccoonEventData)
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_SetRaccoonEventToGoonzu) );

		memcpy(m_GoonzuRaccoonEventData,	pGoonzuRaccoonEventData,	sizeof(m_GoonzuRaccoonEventData));
	}
};
//[성웅] 라쿤이벤트 군주 이벤트로 변경
// 다이얼로그를 열면서 정보를 가지고온다 .
class cltGameMsgRequest_GoonzuRaccoonEventInfo
{
public:
	SI32 m_siPersonID	;
	cltGameMsgRequest_GoonzuRaccoonEventInfo()
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_GoonzuRaccoonEventInfo) );
	}
	cltGameMsgRequest_GoonzuRaccoonEventInfo(SI32 siPersonID)
	{

		m_siPersonID	=	siPersonID	;
	}
};
//[성웅] 라쿤이벤트 군주 이벤트로 변경
class cltGameMsgResponse_GoonzuRaccoonEventInfo
{
public:
	cGoonzuRaccoonEventData	m_GoonzuRaccoonEventData[MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT];

	cltGameMsgResponse_GoonzuRaccoonEventInfo(cGoonzuRaccoonEventData* pGoonzuRaccoonEventData)
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_GoonzuRaccoonEventInfo) );

		memcpy(m_GoonzuRaccoonEventData,	pGoonzuRaccoonEventData,	sizeof(m_GoonzuRaccoonEventData));
	}
};
//[성웅] 라쿤월드 이동 
class cltGameMsgRequest_GoRaccoonWorld
{
public:
	SI32 m_siPersonID	;
	cltGameMsgRequest_GoRaccoonWorld()
	{
		ZeroMemory( this, sizeof(cltGameMsgRequest_GoRaccoonWorld) );
	}
	cltGameMsgRequest_GoRaccoonWorld(SI32 siPersonID)
	{

		m_siPersonID	=	siPersonID	;
	}
};
//[성웅] 라쿤월드 이동 
class cltGameMsgResponse_GoRaccoonWorld
{
public:
	SI32	m_siResult;

	cltGameMsgResponse_GoRaccoonWorld( SI32 siResult)
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_GoRaccoonWorld) );
		m_siResult	=	siResult	;
	}
};


#endif