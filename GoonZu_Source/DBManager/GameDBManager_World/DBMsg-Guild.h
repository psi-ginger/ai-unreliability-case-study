#ifndef _DBMSGGUILD_H
#define _DBMSGGUILD_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "../CommonLogic/Guild/Guild.h"
#include "../CommonLogic/Guild/GuildWar.h"
#include "../CommonLogic/Guild/Guild-Dungeon.h"
#include "../CommonLogic/Guild/GuildHuntMapWarManager.h"

//-----------------------------------------------------
// 길드 생성을 요청한다.
//-----------------------------------------------------
class sDBRequest_Guild_CreateGuild
{
public:
	sPacketHeader		packetHeader;

	SI32			siPersonID;
	SI32			siCharID;

	SI32			siVillageUnique;		// 생성 요청한 길드가 거주할 건물이 있는 마을 유니크
	TCHAR			szGuildName[MAX_GUILD_NAME]; // 길드 이름

	cltDate			clDate; // 신청일 - cltGuildInfo의 clDate에 들어간다.

	sDBRequest_Guild_CreateGuild()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_CreateGuild));
	}

	sDBRequest_Guild_CreateGuild(SI32 personid,SI32 charid , SI32 villageunique,TCHAR *guildname,cltDate *cldate)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_CreateGuild );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CREATEGUILD;

		siPersonID = personid;
		siCharID = charid;

		siVillageUnique = villageunique;
		MStrCpy(szGuildName,guildname,MAX_GUILD_NAME);

		clDate.Set(cldate);
	}
};

class sDBResponse_Guild_CreateGuild
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID;

	cltGuildInfo		clInfo;				// 생성된 길드의 정보

	sDBResponse_Guild_CreateGuild()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_CreateGuild ) );
	}
};

//-----------------------------------------------------
// 길드 해체를 요청한다.
//-----------------------------------------------------
class sDBRequest_Guild_DestroyGuild
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID;

	SI32				siGuildUnique; // 해체할 길드의 인덱스

	SI32				siVillageUnique; // 해체할 길드가 속한 건물이 있는 마을 유니크

	SI08				siReason;

	sDBRequest_Guild_DestroyGuild()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_DestroyGuild));
	}

	sDBRequest_Guild_DestroyGuild(SI32 personid,SI32 charid,SI32 guildunique,SI32 villageunique=0, SI32 reason = 0 )
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_DestroyGuild );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_DESTROYGUILD;

		siPersonID = personid ;
		siCharID = charid ;

		siGuildUnique = guildunique;

		siVillageUnique = villageunique ;

		siReason = reason;
	}
};

class sDBResponse_Guild_DestroyGuild
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID;

	SI32				siGuildUnique;
	SI32				siVillageUnique;

	SI32				siReason;

	sDBResponse_Guild_DestroyGuild()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_DestroyGuild ) );
	}
};

//-----------------------------------------------------
// 길드 가입을 요청한다.
//-----------------------------------------------------
class sDBRequest_Guild_JoinGuild
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID;

	cltGuildInfo		clInfo; // 가입요청한 길드의 정보

	sDBRequest_Guild_JoinGuild()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_JoinGuild));
	}

	sDBRequest_Guild_JoinGuild(SI32 personid,SI32 charid,cltGuildInfo*pclinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_JoinGuild );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_JOINGUILD;

		siPersonID = personid ;
		siCharID = charid ;

		clInfo.Set(pclinfo);
	}
};

class sDBResponse_Guild_JoinGuild
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID;

	cltGuildInfo		clInfo; // 가입요청한 길드의 정보

	sDBResponse_Guild_JoinGuild()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_JoinGuild ) );
	}
};

//-----------------------------------------------------
// 길드 가입을 허가,불허를 결정한다.
//-----------------------------------------------------
class sDBRequest_Guild_ConfirmJoinGuild
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	bool				bConfirm; // TRUE면 허가 , FALSE면 불허

	SI32				siStandingPersonID; // 허가,불허되는 personid

	sDBRequest_Guild_ConfirmJoinGuild()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_ConfirmJoinGuild));
	}

	sDBRequest_Guild_ConfirmJoinGuild(SI32 personid,SI32 charid,bool confirm,SI32 standingpersonid)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_ConfirmJoinGuild );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CONFIRMJOINGUILD;

		siPersonID = personid ;
		siCharID = charid ;

		bConfirm = confirm ;

		siStandingPersonID = standingpersonid ;
	}
};

class sDBResponse_Guild_ConfirmJoinGuild
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	bool				bConfirm;			// TRUE면 허가 , FALSE면 불허

	SI32				siStandingPersonID; // 허가,불허되는 personid

	cltGuildInfo		clInfo; // 변경된 길드의 정보

	sDBResponse_Guild_ConfirmJoinGuild()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_ConfirmJoinGuild ) );
	}
};

//-----------------------------------------------------
// 길드 탈퇴를 요청한다.
//-----------------------------------------------------
class sDBRequest_Guild_LeaveGuild
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	//SI32				siCharID ;

	SI32				siGuildUnique;

	sDBRequest_Guild_LeaveGuild()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_LeaveGuild));
	}

	sDBRequest_Guild_LeaveGuild(SI32 personid,/*SI32 charid,*/SI32 guildunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_LeaveGuild );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_LEAVEGUILD;

		siPersonID = personid ;
		//siCharID = charid ;

		siGuildUnique = guildunique ;
	}
};

class sDBResponse_Guild_LeaveGuild
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	cltGuildInfo		clInfo; // 변경된 길드의 정보

	sDBResponse_Guild_LeaveGuild()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_LeaveGuild ) );
	}
};

//-------------------------------------------------------------------
// 길드 본거지 이동을 요청한다.(다른 마을 길드 건물로 이동 요청)
//-----------------------------------------------------------------------
class sDBRequest_Guild_ChangeVillage
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siGuildUnique;			// 옮길 길드의 유니크
	SI32				siVillageUnique;		// 길드가 옮겨갈 마을의 유니크
	SI32				siLeaveVillageUnique;	// 원래의 길드가 있던 마을 유니크

	sDBRequest_Guild_ChangeVillage()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_ChangeVillage));
	}

	sDBRequest_Guild_ChangeVillage(SI32 personid,SI32 charid,SI32 guildunique,SI32 villageunique,SI32 leavevillageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_ChangeVillage );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CHANGEVILLAGE;

		siPersonID = personid ;
		siCharID = charid ;

		siGuildUnique = guildunique ;
		siVillageUnique = villageunique ;
		siLeaveVillageUnique = leavevillageunique ;
	}
};

class sDBResponse_Guild_ChangeVillage
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	cltGuildInfo		clInfo; // 변경된 길드의 정보

	sDBResponse_Guild_ChangeVillage()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_ChangeVillage ) );
	}
};

//-------------------------------------------------------------------
// 길드의 정보를 요청한다.
//-----------------------------------------------------------------------
class sDBRequest_Guild_GetGuildInfo
{
public:
	sPacketHeader		packetHeader;


	sDBRequest_Guild_GetGuildInfo()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_GetGuildInfo));
	}

	sDBRequest_Guild_GetGuildInfo(bool bStart)
	{
		if ( bStart )
		{
			packetHeader.usSize = sizeof( sDBRequest_Guild_GetGuildInfo );
			packetHeader.usCmd	= DBMSG_REQUEST_GUILD_GETGUILDINFO;
		}
	}
};

class sDBResponse_Guild_GetGuildInfo
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.

	SI32				siStartPos;

	cltGuildInfo		clInfo[MAX_GUILD_PER_DBLIST];

	sDBResponse_Guild_GetGuildInfo()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_GetGuildInfo ) );
	}
};

//class sDBRequest_Guild_ChangeVillageList
//{
//public:
//	sPacketHeader		packetHeader;
//
//	sDBRequest_Guild_ChangeVillageList()
//	{
//		ZeroMemory(this, sizeof(sDBRequest_Guild_ChangeVillageList));
//	}
//
//	sDBRequest_Guild_ChangeVillageList(bool bStart)
//	{
//		if ( bStart )
//		{
//			packetHeader.usSize = sizeof( sDBRequest_Guild_ChangeVillageList );
//			packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CHANGEVILLAGELIST;
//		}
//	}
//};
//
//class sDBResponse_Guild_ChangeVillageList
//{
//public:
//	sPacketHeader		packetHeader;
//
//	SI32				siResult;			// 결과값.
//
//	cltGuildChangeVillageInfo clInfo[MAX_GUILD_NUM];
//
//
//	sDBResponse_Guild_ChangeVillageList()
//	{
//		memset( this, 0, sizeof( sDBResponse_Guild_ChangeVillageList ) );
//	}
//};


//-------------------------------------------------------------------
// 길드 생성을 허가한다.
//-----------------------------------------------------------------------
class sDBRequest_Guild_ConfirmCreateGuild
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siUnique;	// 길드의 유니크
	bool				bConfirm;	// true면 허가 , false면 불허

	GMONEY				siMoney;	// 개설비용 - 개설신청자(길드장)의 소지금에서 빠진다. 이 비용을 들고 있지 않으면 길드개설삭제
	GMONEY				siFee; // 임대료 - 매달 빠져나갈 돈

	cltDate				clDate; // 개설일 - cltGuildInfo의 clDate를 업데이트 시킨다.

	sDBRequest_Guild_ConfirmCreateGuild()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_ConfirmCreateGuild));
	}

	sDBRequest_Guild_ConfirmCreateGuild(SI32 personid,SI32 charid,SI32 unique,bool confirm,GMONEY money,GMONEY fee,cltDate *cldate)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_ConfirmCreateGuild );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CONFIRMCREATEGUILD;

		siPersonID = personid ;
		siCharID = charid ;

		siUnique = unique ;
		
		bConfirm = confirm ;

		siMoney = money ;
		siFee = fee;

		clDate.Set(cldate);
	}
};

class sDBResponse_Guild_ConfirmCreateGuild
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	bool				bConfirm;			// 승인인지 거부인지(서버에서 보낸값)

	SI32				siGuildUnique;		// 취소시에만 사용
	cltGuildInfo		clInfo;

	GMONEY				siMoney;			// 개설비용 - 개설신청자(길드장)의 소지금에서 빠진다. 이 비용을 들고 있지 않으면 길드개설삭제
	GMONEY				siFee;				// 임대료 - 매달 빠져나갈 돈

	GMONEY				siLeftMoney;		// 길드장에게 남은 돈
	cltDate				clDate;				// 길드 개설일

	SI08				siBeforeConfirm;	// 가입 = 0 ,이주 = 2
	bool				bState;				// 승인 = 1, 실패 = 0 ( 돈이 없을 경우....)


	sDBResponse_Guild_ConfirmCreateGuild()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_ConfirmCreateGuild ) );
	}
};

//-------------------------------------------------------------------
// 부길드장을 선출 및 강등한다.
//-----------------------------------------------------------------------
class sDBRequest_Guild_SetSecondMaster
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siSecondMasterPersonID; // 부길드장이 될 personid

	bool				bSet; // true면 선출, false면 강등. 1길드에는 오직 1명의 부길드장이 있다.

	sDBRequest_Guild_SetSecondMaster()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_SetSecondMaster));
	}

	sDBRequest_Guild_SetSecondMaster(SI32 personid,SI32 charid,SI32 secondmasterpersonid,bool set)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_SetSecondMaster );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_SETSECONDMASTER;

		siPersonID = personid ;
		siCharID = charid ;

		siSecondMasterPersonID = secondmasterpersonid ;

		bSet = set ;
	}
};

class sDBResponse_Guild_SetSecondMaster
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siSecondMasterPersonID; // 부길드장이 될 personid

	bool				bSet ;

	cltGuildInfo		clInfo;

	sDBResponse_Guild_SetSecondMaster()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_SetSecondMaster ) );
	}
};

//----------------------------------------------------
// 길드 자금을 납부한다.
//---------------------------------------------
class sDBRequest_Guild_GiveGuildMoney
{
public:
	sPacketHeader		packetHeader;

	SI32				siGuildUnique;
	SI32				siPersonID;
	SI32				siCharID ;

	GMONEY				siMoney ; // 납부할 금액

	sDBRequest_Guild_GiveGuildMoney()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_GiveGuildMoney));
	}

	sDBRequest_Guild_GiveGuildMoney(SI32 guildunique, SI32 personid,SI32 charid,GMONEY money)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_GiveGuildMoney );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_GIVEGUILDMONEY;

		siGuildUnique = guildunique;
		siPersonID = personid ;
		siCharID = charid ;

		siMoney = money ;
	}
};

class sDBResponse_Guild_GiveGuildMoney
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	cltGuildInfo		clinfo;

	GMONEY				siMoney ; // 납부한 금액
	GMONEY				siLeftMoney ; // 납부하고 남은 금액

	sDBResponse_Guild_GiveGuildMoney()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_GiveGuildMoney ) );
	}
};

#define TYPE_GUILDPOINT_ADD_PLAYTIME		1	// 플레이타임
#define TYPE_GUILDPOINT_ADD_LEVELUP			2	// 레벨업
#define TYPE_GUILDPOINT_ADD_MAKERARE		3	// 명품 제작
#define TYPE_GUILDPOINT_ADD_ENCHANT_5		4	// 5단계 인챈트 성공
#define TYPE_GUILDPOINT_ADD_ENCHANT_6		5	// 6단계 인챈트 성공
#define TYPE_GUILDPOINT_ADD_ENCHANT_7		6	// 7단계 인챈트 성공
#define TYPE_GUILDPOINT_ADD_FP				7	// 추천인 포인트 획득
#define TYPE_GUILDPOINT_ADD_CHILD			8	// 20레벨이상 제자 육성
#define TYPE_GUILDPOINT_ADD_SKILLLEVELUP	9	// 스킬 경험치 레벨업
#define TYPE_GUILDPOINT_ADD_GIVEGUILDMONEY	10	// 길드 자금 납부
#define TYPE_GUILDPOINT_REMOVE_DEAD			11	// 사망 - 페널티
#define TYPE_GUILDPOINT_GUILDWAR			12	// 길드전 결과에 따라! - 실제길드 포인트는 바뀌지 않고 길드전 점수만 변경된다.
#define TYPE_GUILDPOINT_GUILDDUNGEON		13  // 사냥터 소유권으로 인해서
#define TYPE_GUILDPOINT_DAILYQUEST			14	// Daily 
#define TYPE_GUILDPOINT_ADD_ENCHANT_8		15	// 8단계 인챈트 성공
#define TYPE_GUILDPOINT_ADD_ENCHANT_9		16	// 9단계 인챈트 성공
#define TYPE_GUILDPOINT_ADD_ENCHANT_10		17	// 10단계 인챈트 성공
#define TYPE_GUILDPOINT_DAILYQUEST2			18	// Daily 
//----------------------------------------------------
// 길드의 GP가 변화되었다.
//---------------------------------------------
class sDBRequest_Guild_ChangeGuildPoint
{
public:
	sPacketHeader		packetHeader;

	SI32				siGuildUnique; // 길드 유니크

	SI32				siPersonID;
	SI32				siExp; // 추가할 exp - 자기 정보랑 길드 정보 2군데 업데이트

	SI32				siType; // 추가하는 이유
	SI32				siMax;	// 최대값. 0이면 무제한

	sDBRequest_Guild_ChangeGuildPoint()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_ChangeGuildPoint));
	}

	sDBRequest_Guild_ChangeGuildPoint(SI32 unique,SI32 personid,SI32 exp,SI32 type,SI32 max = 0 )
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_ChangeGuildPoint );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CHANGEGUILDPOINT;

		siGuildUnique = unique ;

		siPersonID = personid ;
		siExp = exp ;

		siType = type ;
		siMax = max ;
	}
};

class sDBResponse_Guild_ChangeGuildPoint
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	cltGuildInfo		clinfo;				// 변경된 길드 정보

	sDBResponse_Guild_ChangeGuildPoint()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_ChangeGuildPoint ) );
	}
};

//-----------------------------------------------
// 건물 수수료를 납부한다.
//----------------------------------------------
class sDBRequest_Guild_Give_Fee
{
public:
	   sPacketHeader		packetHeader;

	   SI32					siGuildUnique; // 길드 유니크
	   GMONEY				siFee;	// 수수료 - 길드 자금에서 빠진다.
	   cltDate				clDate;	// 수수료 납부한 시간

	   sDBRequest_Guild_Give_Fee()
	   {
		   ZeroMemory(this, sizeof(sDBRequest_Guild_Give_Fee));
	   }

	   sDBRequest_Guild_Give_Fee(SI32 unique,GMONEY fee,cltDate*pcldate)
	   {
		   packetHeader.usSize = sizeof( sDBRequest_Guild_Give_Fee );
		   packetHeader.usCmd	= DBMSG_REQUEST_GUILD_GIVE_FEE;

		   siGuildUnique = unique ;
		   siFee = fee ;
		   clDate.Set(pcldate);
	   }
};

class sDBResponse_Guild_Give_Fee
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	cltGuildInfo		clinfo;				// 변경된 길드 정보

	GMONEY				siLeftStrMoney;		// 변경된 건물 자본금
	
	SI32				siFaultNum;			// 돈이 모자라서 안 내놓았을 경우 실패횟수 - 3번째에 길드 해체
	SI32				siGuildUnique;		// 길드 유니크 - 해체되었을 경우 cltGuildInfo가 NULL이므로 이때 사용

	sDBResponse_Guild_Give_Fee()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_Give_Fee ) );
	}
};

//-----------------------------------------------
// 길드 능력치가 변경되었다.
//----------------------------------------------
class sDBRequest_Guild_ChangeStatus
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siGuildUnique; // 길드 유니크

	cltGuildStatus		clinfo;

	sDBRequest_Guild_ChangeStatus()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_ChangeStatus));
	}

	sDBRequest_Guild_ChangeStatus(SI32 personid,SI32 charid,SI32 unique,cltGuildStatus*pclinfo)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_ChangeStatus );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CHANGESTATUS;

		siPersonID = personid ;
		siCharID = charid ;

		siGuildUnique = unique ;

		clinfo.siBonus = pclinfo->siBonus ;
		clinfo.siDex = pclinfo->siDex ;
		clinfo.siHnd = pclinfo->siHnd ;
		clinfo.siLuk = pclinfo->siLuk ;
		clinfo.siMag = pclinfo->siMag ;
		clinfo.siStr = pclinfo->siStr ;
		clinfo.siVit = pclinfo->siVit ;
	}
};

class sDBResponse_Guild_ChangeStatus
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	cltGuildInfo		clinfo;				// 변경된 길드 정보

	sDBResponse_Guild_ChangeStatus()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_ChangeStatus ) );
	}
};

//-----------------------------------------------
// 길드 건물의 수수료가 변경되었다. - 행수
//----------------------------------------------
class sDBRequest_Guild_SetStrFeeRate
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siVillageUnique;
	SI32				siFeeRate;

	sDBRequest_Guild_SetStrFeeRate()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_SetStrFeeRate));
	}

	sDBRequest_Guild_SetStrFeeRate(SI32 personid,SI32 charid,SI32 village,SI32 feerate)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_SetStrFeeRate );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_SETSTRFEERATE;

		siPersonID = personid ;
		siCharID = charid ;

		siVillageUnique = village ;
		siFeeRate = feerate ;
	}
};

class sDBResponse_Guild_SetStrFeeRate
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siVillageUnique;
	SI32				siFeeRate;

	sDBResponse_Guild_SetStrFeeRate()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_SetStrFeeRate ) );
	}
};

//---------------------------------------------
// 길드가 레벨업을 하였다.
//--------------------------------------------
class sDBRequest_Guild_LevelUP
{
public:
	sPacketHeader		packetHeader;
	SI32				siGuildUnique;

	SI32				siLevel;

	sDBRequest_Guild_LevelUP()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_LevelUP));
	}

	sDBRequest_Guild_LevelUP(SI32 unique,SI32 level)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_LevelUP );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_LEVELUP;

		siGuildUnique = unique ;

		siLevel = level ;
	}
};

class sDBResponse_Guild_LevelUP
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	cltGuildInfo		clinfo;				// 변경된 길드 정보

	sDBResponse_Guild_LevelUP()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_LevelUP ) );
	}
};

//-----------------------------------------------
// 길드 마크를 설정하였다.
//----------------------------------------------
class sDBRequest_Guild_SetMark
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siGuildUnique;
	SI32				siGuildMark;

	sDBRequest_Guild_SetMark()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_SetMark));
	}

	sDBRequest_Guild_SetMark(SI32 personid,SI32 charid,SI32 unique,SI32 mark)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_SetMark );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_SETMARK;

		siPersonID = personid ;
		siCharID = charid ;

		siGuildUnique = unique ;
		siGuildMark = mark ;
	}
};

class sDBResponse_Guild_SetMark
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	cltGuildInfo		clinfo;

	sDBResponse_Guild_SetMark()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_SetMark ) );
	}
};

//-----------------------------------------------
// 이름을 변경하였다
//----------------------------------------------
class sDBRequest_Guild_ChangeName
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siGuildUnique;
	TCHAR				szName[MAX_GUILD_NAME];

	sDBRequest_Guild_ChangeName()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_ChangeName));
	}

	sDBRequest_Guild_ChangeName(SI32 personid,SI32 charid,SI32 unique,TCHAR*pName)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_ChangeName );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CHANGENAME;

		siPersonID = personid ;
		siCharID = charid ;

		siGuildUnique = unique ;
		MStrCpy(szName,pName,MAX_GUILD_NAME);
	}
};

class sDBResponse_Guild_ChangeName
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	cltGuildInfo		clinfo;

	sDBResponse_Guild_ChangeName()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_ChangeName ) );
	}
};

//-----------------------------------------------
// 길드 마스터 변경
//----------------------------------------------
class sDBRequest_Guild_ChangeMaster
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siGuildUnique;
	SI32				siNewPersonID;				// 길드 마스터를 넘겨줄 PersonID

	sDBRequest_Guild_ChangeMaster()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_ChangeMaster));
	}

	sDBRequest_Guild_ChangeMaster(SI32 personid, SI32 charid, SI32 unique, SI32 newpersonid)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_ChangeMaster );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CHANGEMASTER;

		siPersonID = personid ;
		siCharID = charid ;

		siGuildUnique = unique ;
		siNewPersonID = newpersonid;
	}
};
class sDBResponse_Guild_ChangeMaster
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	SI32				siPersonID;
	SI32				siCharID ;

	cltGuildInfo		clInfo;
	SI32				siNewPersonID;

	sDBResponse_Guild_ChangeMaster()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_ChangeMaster ) );
	}
};

class sDBRequest_Guild_CloseGuild
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siGuildUnique;

	TCHAR				szGuildName[MAX_GUILD_NAME];

	sDBRequest_Guild_CloseGuild()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_CloseGuild));
	}

	sDBRequest_Guild_CloseGuild(SI32 personid, SI32 guildunique, TCHAR* guildname )
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_CloseGuild );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CLOSEGUILD;

		siPersonID = personid ;
		siGuildUnique = guildunique ;

		StringCchCopy( szGuildName, MAX_GUILD_NAME, guildname );
	}

};

class sDBResponse_Guild_CloseGuild
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	SI32				siPersonID;
	SI32				siGuildUnique;

	TCHAR				szGuildName[MAX_GUILD_NAME];

	sDBResponse_Guild_CloseGuild()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_CloseGuild ) );
	}
};

class sDBRequest_GuildWar_AddReserve
{
public:
	sPacketHeader	packetHeader;

	SI32			siFieldIndex;
	SI32			siRoomIndex;

	SI32			siHomeGuildUnique;
	SI32			siAwayGUildUnique;

	// PCK : 로그용 패킷
	SI32			siPersonID;			// 길드전을 신청한 사람

	sDBRequest_GuildWar_AddReserve()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildWar_AddReserve));
	}

	sDBRequest_GuildWar_AddReserve(SI32 field, SI32 room, SI32 home, SI32 away, SI32 PersonID)
	{
		packetHeader.usSize = sizeof( sDBRequest_GuildWar_AddReserve );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILDWAR_ADDRESERVE;

		siFieldIndex = field;
		siRoomIndex = room;

		siHomeGuildUnique = home;
		siAwayGUildUnique = away;

		siPersonID = PersonID;
	}
};

class sDBResponse_GuildWar_AddReserve
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;

	// [추가 : 황진성 2007. 11. 7 로그에 이용. 길드전 예약]
	SI32			siFieldIndex;
	SI32			siRoomIndex;

	SI32			siHomeGuildUnique;
	SI32			siAwayGUildUnique;

	// PCK : 로그용 패킷
	SI32			siPersonID;			// 길드전을 신청한 사람

	sDBResponse_GuildWar_AddReserve()
	{
		memset( this, 0, sizeof( sDBResponse_GuildWar_AddReserve ) );
	}
};

class sDBRequest_GuildWar_ReserveList
{
public:
	sPacketHeader	packetHeader;

	SI32			siFieldIndex;

	sDBRequest_GuildWar_ReserveList()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildWar_ReserveList));
	}

	sDBRequest_GuildWar_ReserveList(SI32 field)
	{
		packetHeader.usSize = sizeof( sDBRequest_GuildWar_ReserveList );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILDWAR_RESERVELIST;

		siFieldIndex = field;
	}
};

class sDBResponse_GuildWar_ReserveList
{
public:
	sPacketHeader	packetHeader;

	SI32					siResult;

	SI32					siFieldIndex;
	cltGuildWarReserveUnit	clUnit[GUILDWAR_ROOMNUM];

	sDBResponse_GuildWar_ReserveList()
	{
		memset( this, 0, sizeof( sDBResponse_GuildWar_ReserveList ) );
	}
};
/*=====================================================================
 길드 사냥터 신청이 새로운것인지 아닌지를 타입으로 구분한다.
//=====================================================================*/
enum
{
	MESSAGETYPE_OLD	=	0,
	MESSAGETYPE_NEW,
};
class sDBRequest_GuildDungeon_Apply
{
public:
	sPacketHeader	packetHeader;

	SI32			siGuildUnique;
	SI32			siMapIndex;

	SI32			siCharID;
	SI32			siType	;	// 이 길드 사냥터 신청이 새로운것인지, 기존의 것인지.

	sDBRequest_GuildDungeon_Apply()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildDungeon_Apply));
	}

	sDBRequest_GuildDungeon_Apply(SI32 guildUnique, SI32 mapIndex, SI32 charID, SI32 Type )
	{
		packetHeader.usSize = sizeof( sDBRequest_GuildDungeon_Apply );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILDDUNGEON_APPLY;

		siGuildUnique = guildUnique;
		siMapIndex = mapIndex;
		siCharID = charID;
		siType	=	Type	;
	}
};

class sDBResponse_GuildDungeon_Apply
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			siGuildUnique;
	SI32			siMapIndex;

	cltGuildInfo	clGuildInfo;

	SI32			siCharID;
	SI32			siType	;


	sDBResponse_GuildDungeon_Apply()
	{
		memset( this, 0, sizeof( sDBResponse_GuildDungeon_Apply ) );
	}
};

class sDBRequest_GuildDungeon_SaveStatus
{
public:
	sPacketHeader	packetHeader;

	SI32			siMapIndex;
	SI32			siExp;
	GMONEY			siMoney;

	sDBRequest_GuildDungeon_SaveStatus()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildDungeon_SaveStatus));
	}

	sDBRequest_GuildDungeon_SaveStatus(SI32 mapindex, SI32 exp, GMONEY money )
	{
		packetHeader.usSize = sizeof( sDBRequest_GuildDungeon_SaveStatus );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILDDUNGEON_SAVESTATUS;

		siMapIndex = mapindex;
		siExp = exp;
		siMoney = money;
	}
};
class sDBResponse_GuildDungeon_SaveStatus
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;

	sDBResponse_GuildDungeon_SaveStatus()
	{
		memset( this, 0, sizeof( sDBResponse_GuildDungeon_SaveStatus ) );
	}
};


class sDBRequest_GuildDungeon_LoadStatus
{
public:
	sPacketHeader	packetHeader;

	SI32			siMapIndex;

	sDBRequest_GuildDungeon_LoadStatus()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildDungeon_LoadStatus));
	}

	sDBRequest_GuildDungeon_LoadStatus(SI32 mapindex )
	{
		packetHeader.usSize = sizeof( sDBRequest_GuildDungeon_LoadStatus );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILDDUNGEON_LOADSTATUS;

		siMapIndex = mapindex;
	}
};

class sDBResponse_GuildDungeon_LoadStatus
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult;
	SI32			siMapIndex;
	SI32			siGuildUnique;
	SI32			siExp;
	GMONEY			siMoney;

	sDBResponse_GuildDungeon_LoadStatus()
	{
		ZeroMemory(this, sizeof(sDBResponse_GuildDungeon_LoadStatus) );
	}
};


// 길드 소유권 이전

class sDBRequest_GuildDungeon_TransferOwnership
{
public:
	sPacketHeader	packetHeader;

	float			fGPRatio;			
	float			fMoneyRatio;

	sDBRequest_GuildDungeon_TransferOwnership(float gpratio, float moneyratio )
	{
		packetHeader.usSize = sizeof( sDBRequest_GuildDungeon_TransferOwnership );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILDDUNGEON_TRANSFEROWNERSHIP;

		fGPRatio = gpratio;
		fMoneyRatio = moneyratio;
	}
};

class cltGuildDungeon_ChangedGPMoney
{
public:
	SI32 siGuildUnique;
	SI32 siAddExp;
	SI32 siExp;
	GMONEY siAddMoney;
	GMONEY siMoney;
};
class cltGuildDungeon_NewOwnership
{
public:
	SI32 siMapindex;
	SI32 siGuildUnique;
};

class sDBResponse_GuildDungeon_TransferOwnership
{
public:
	sPacketHeader	packetHeader;

	SI32							siResult;
	cltGuildDungeon_ChangedGPMoney	clChangedGuildInfo[MAX_GUILDDUNGEON];
	cltGuildDungeon_NewOwnership	clNewOwnerInfo[MAX_GUILDDUNGEON];

	sDBResponse_GuildDungeon_TransferOwnership()
	{
		ZeroMemory( this, sizeof(sDBResponse_GuildDungeon_TransferOwnership) );
	}
};
//----------------------------------------------------------------------------
// 미활동 길드 해체 ( 1달에 한번 미활동 길드 삭제)
//----------------------------------------------------------------------------
// 미활동 길드를 찾고 지워달라고 디비로 메세지를 보낸다.
#define OVER30DAYSNOTINTOGAME	30

class sDBRequest_Guild_CheckDestroy_Month
{
public:
	sPacketHeader		packetHeader;

	SYSTEMTIME			stLastConnectTime;		// 이 날 이후로 접속이 없다면 삭제됨.

	sDBRequest_Guild_CheckDestroy_Month()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_CheckDestroy_Month));
	}

	sDBRequest_Guild_CheckDestroy_Month( SYSTEMTIME* stTime )
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_CheckDestroy_Month );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_CHECKDESTROY_MONTH;

		memcpy(&stLastConnectTime, stTime, sizeof(stLastConnectTime));
	}
};

// 미활동 길드 삭제 정보를 받는다.
class sDBResponse_Guild_CheckDestroy_Month
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.

	cltGuildInfo		clInfo[MAX_GUILD_PER_DBLIST];	// 삭제된 길드 정보를 받는다.

	sDBResponse_Guild_CheckDestroy_Month()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_CheckDestroy_Month ) );
	}
};

//-----------------------------------------------
// 한달동안 접속안한 길드 마스터 변경
//----------------------------------------------
class sDBRequest_Guild_NotConnect_ChangeMaster
{
public:
	sPacketHeader		packetHeader;

	SI32				siPersonID;
	SI32				siCharID ;

	SI32				siGuildUnique;
	SYSTEMTIME			stLastConnectTime;		// 마지막 접속 기준일(날짜 이후로 접속이 없다면 성공)

	sDBRequest_Guild_NotConnect_ChangeMaster()
	{
		ZeroMemory(this, sizeof(sDBRequest_Guild_NotConnect_ChangeMaster));
	}

	sDBRequest_Guild_NotConnect_ChangeMaster(SI32 personid, SI32 charid, SI32 unique, SYSTEMTIME* pstTime)
	{
		packetHeader.usSize = sizeof( sDBRequest_Guild_NotConnect_ChangeMaster );
		packetHeader.usCmd	= DBMSG_REQUEST_GUILD_NOTCONNECT_CHANGEMASTER;

		siPersonID = personid ;
		siCharID = charid ;

		siGuildUnique = unique ;
		memcpy(&stLastConnectTime, pstTime, sizeof(SYSTEMTIME));
	}
};

class sDBResponse_Guild_NotConnect_ChangeMaster
{
public:
	sPacketHeader		packetHeader;
	SI32				siResult;			// 결과값.
	SI32				siPersonID;
	SI32				siCharID ;
	
	SI32				siNewPersonID;
	TCHAR				szNewPersonName[MAX_PERSON_NUMBER];

	cltGuildInfo		clInfo;

	sDBResponse_Guild_NotConnect_ChangeMaster()
	{
		memset( this, 0, sizeof( sDBResponse_Guild_NotConnect_ChangeMaster ) );
	}
};
enum
{
	AddList	=	0,
	DelList,
};
// 길드맵 길드전 신청 리스트를 저장 한다.
class sDBRequest_GuildHuntMapWar_SaveRegList
{
public:
	sPacketHeader	packetHeader;
	SI32			siGuildUnique	;	//저장할 길드 유니크
	SI32			siType			;	// 추가인지 삭제인지.

	sDBRequest_GuildHuntMapWar_SaveRegList()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildHuntMapWar_SaveRegList));
	}

	sDBRequest_GuildHuntMapWar_SaveRegList(SI32 GuildUnique,SI32 Type)
	{
		packetHeader.usSize =	sizeof( sDBRequest_GuildHuntMapWar_SaveRegList );
		packetHeader.usCmd	=	DBMSG_REQUEST_GUILDHUNTMAPWAR_REGLIST ;
		siGuildUnique		=	GuildUnique	;
		siType				=	Type		;
	}
};
class sDBResponse_GuildHuntMapWar_SaveRegList
{
public:
	sPacketHeader	packetHeader;

	SI32			siResult		;
	SI32			siGuildUnique	;
	SI32			siType			;
	sDBResponse_GuildHuntMapWar_SaveRegList()
	{
		memset( this, 0, sizeof( sDBResponse_GuildHuntMapWar_SaveRegList ) );
	}
};
// 길드맵 길드전 신청 리스트를 가지고 온다. 서버 처음에 열때 한번 호출 한다.
class sDBRequest_GuildHuntMapWar_SaveRegListGet
{
public:
	sPacketHeader	packetHeader;
	sDBRequest_GuildHuntMapWar_SaveRegListGet()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildHuntMapWar_SaveRegListGet));
		packetHeader.usSize =	sizeof( sDBRequest_GuildHuntMapWar_SaveRegListGet );
		packetHeader.usCmd	=	DBMSG_REQUEST_GUILDHUNTMAPWAR_GET_REGLIST ;
	}
};
class sDBResponse_GuildHuntMapWar_SaveRegListGet
{
public:
	sPacketHeader	packetHeader;
	cltGuildHuntMapWarListData	m_GuildList[MAX_GUILD_NUM];

	sDBResponse_GuildHuntMapWar_SaveRegListGet()
	{
		memset( this, 0, sizeof( sDBResponse_GuildHuntMapWar_SaveRegListGet ) );
	}
};
// 해당 맵의 길드BGP순위 10위까지 받는다.
class sDBRequest_GuildHuntMapWar_Rank_Get
{
public:
	sPacketHeader	packetHeader;
	SI32			siMapIndex	;
	SI32			siPersonID	;
	sDBRequest_GuildHuntMapWar_Rank_Get()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildHuntMapWar_Rank_Get));
	}

	sDBRequest_GuildHuntMapWar_Rank_Get(SI32 Mapindex,SI32 PersonID)
	{
		packetHeader.usSize =	sizeof( sDBRequest_GuildHuntMapWar_Rank_Get );
		packetHeader.usCmd	=	DBMSG_REQUEST_GUILDHUNTMAPWAR_RANK_GET ;
		siMapIndex			=	Mapindex;
		siPersonID			=	PersonID	;

		siMapIndex = Mapindex	;

	}
};
class sDBResponse_GuildHuntMapWar_Rank_Get
{
public:
	sPacketHeader	packetHeader;
	SI32			siPersonID;
	cltGuildHuntMapWarListRankData	m_GuildList[MAX_GUILDHUNTWAR_RANK_NUM];

	sDBResponse_GuildHuntMapWar_Rank_Get()
	{
		memset( this, 0, sizeof( sDBResponse_GuildHuntMapWar_Rank_Get ) );
	}
};
class sDBRequest_GuildHuntMapWar_Result
{
public:
	sPacketHeader	packetHeader;
	SI32			siGuildUnique	;	// 길드 유니크
	SI32			siWinLoseFlag	;	// 승무패 여부
	SI32			siBGP	;			// 획득 BGP
	sDBRequest_GuildHuntMapWar_Result()
	{
		ZeroMemory(this, sizeof(sDBRequest_GuildHuntMapWar_Result));
	}

	sDBRequest_GuildHuntMapWar_Result(SI32 guildunique ,SI32  winloseflag,SI32 bgp )
	{
		packetHeader.usSize =	sizeof( sDBRequest_GuildHuntMapWar_Result );
		packetHeader.usCmd	=	DBMSG_REQUEST_GUILDHUNTMAPWAR_RESULT ;
		siGuildUnique = guildunique	;
		siWinLoseFlag = winloseflag;
		siBGP			= bgp	;

	}
};
class sDBResponse_GuildHuntMapWar_Result
{
public:
	sPacketHeader	packetHeader;
	SI32			siResult;
	SI32			siGuildunique	;
	SI32			siBGP	;
	sDBResponse_GuildHuntMapWar_Result()
	{
		memset( this, 0, sizeof( sDBResponse_GuildHuntMapWar_Result ) );
	}
};

#endif
