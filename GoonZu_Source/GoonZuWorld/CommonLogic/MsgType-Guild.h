#ifndef _MSGTYPEGUILD_H
#define _MSGTYPEGUILD_H

#include "../../NLib/NArray.h"

#include "../CommonLogic/Guild/Guild.h"
#include "../CommonLogic/Guild/GuildWar.h"
#include "../CommonLogic/Guild/Guild-Dungeon.h"
#include "../CommonLogic/Guild//GuildHuntMapWarManager.h"

//-----------------------------------------------------
// 길드 생성을 요청한다.
//-----------------------------------------------------
class cltGameMsgRequest_Guild_CreateGuild
{
public:
	SI32			siVillageUnique;
	TCHAR			szGuildName[MAX_GUILD_NAME];

	cltGameMsgRequest_Guild_CreateGuild(SI32 villageunique,TCHAR* guildname)
	{
		siVillageUnique = villageunique ;
		MStrCpy(szGuildName,guildname,MAX_GUILD_NAME);
	}
};

class cltGameMsgResponse_Guild_CreateGuild
{
public:
	SI32				siResult;			// 결과값.

	cltGuildInfo		clInfo;				// 생성된 길드의 정보

	cltGameMsgResponse_Guild_CreateGuild(SI32 result,cltGuildInfo*pclinfo)
	{
		siResult = result ;

		clInfo.Set(pclinfo);
	}
};

//-----------------------------------------------------
// 길드 해체를 요청한다.
//-----------------------------------------------------
class cltGameMsgRequest_Guild_DestroyGuild
{
public:
	SI32				siGuildUnique; // 해체할 길드의 인덱스

	SI32				siVillageUnique; // 해체할 길드가 속한 건물이 있는 마을 유니크

	cltGameMsgRequest_Guild_DestroyGuild(SI32 guildunique,SI32 villageunique)
	{
		siGuildUnique = guildunique ;

		siVillageUnique = villageunique ;
	}
};

class cltGameMsgResponse_Guild_DestroyGuild
{
public:
	SI32				siResult;			// 결과값.
	SI32				siReason;			

	SI32				siGuildUnique;
	SI32				siVillageUnique;


	cltGameMsgResponse_Guild_DestroyGuild(SI32 result,SI32 guildunique,SI32 villageunique,SI32 Reason)
	{
		siResult = result ;
		siReason = Reason ;

		siGuildUnique = guildunique ;
		siVillageUnique = villageunique ;
	}
};

//-----------------------------------------------------
// 길드 가입을 요청한다.
//-----------------------------------------------------
class cltGameMsgRequest_Guild_JoinGuild
{
public:
	SI32			siGuildUnique;

	cltGameMsgRequest_Guild_JoinGuild(SI32 guildunique)
	{
		siGuildUnique = guildunique ;
	}
};

class cltGameMsgResponse_Guild_JoinGuild
{
public:
	cltGuildInfo		clInfo; // 가입요청한 길드의 정보

	cltGameMsgResponse_Guild_JoinGuild(cltGuildInfo* pclinfo)
	{
		clInfo.Set(pclinfo);
	}
};

//-----------------------------------------------------
// 길드 가입을 허가,불허를 결정한다.
//-----------------------------------------------------
class cltGameMsgRequest_Guild_ConfirmJoinGuild
{
public:
	BOOL				bConfirm; // TRUE면 허가 , FALSE면 불허

	SI32				siStandingPersonID; // 허가,불허되는 personid

	cltGameMsgRequest_Guild_ConfirmJoinGuild(BOOL confirm , SI32 standingpersonid)
	{
		bConfirm = confirm ;

		siStandingPersonID = standingpersonid ;
	}
};

class cltGameMsgResponse_Guild_ConfirmJoinGuild
{
public:
	BOOL				bConfirm;			// TRUE면 허가 , FALSE면 불허

	SI32				siStandingPersonID; // 허가,불허되는 personid

	cltGuildInfo		clInfo; // 변경된 길드의 정보

	cltGameMsgResponse_Guild_ConfirmJoinGuild(BOOL confirm,SI32 standingpersonid,cltGuildInfo*pclinfo)
	{
		bConfirm = confirm ;

		siStandingPersonID = standingpersonid ;

		clInfo.Set(pclinfo);
	}
};

//-----------------------------------------------------
// 길드 탈퇴를 요청한다.
//-----------------------------------------------------
class cltGameMsgRequest_Guild_LeaveGuild
{
public:
	SI32				siGuildUnique;

	cltGameMsgRequest_Guild_LeaveGuild(SI32 guildunique)
	{
		siGuildUnique = guildunique ;
	}
};

class cltGameMsgResponse_Guild_LeaveGuild
{
public:
	cltGuildInfo		clInfo; // 변경된 길드의 정보

	cltGameMsgResponse_Guild_LeaveGuild(cltGuildInfo*pclinfo)
	{
		clInfo.Set(pclinfo);
	}
};

//-------------------------------------------------------------------
// 길드 본거지 이동을 요청한다.(다른 마을 길드 건물로 이동 요청)
//-----------------------------------------------------------------------
class cltGameMsgRequest_Guild_ChangeVillage
{
public:
	SI32				siGuildUnique;			// 옮길 길드의 유니크
	SI32				siVillageUnique;		// 길드가 옮겨갈 마을의 유니크

	cltGameMsgRequest_Guild_ChangeVillage(SI32 guildunique,SI32 villageunique)
	{
		siGuildUnique = guildunique ;
		siVillageUnique = villageunique ;
	}
};

class cltGameMsgResponse_Guild_ChangeVillage
{
public:
	cltGuildInfo		clInfo; // 변경된 길드의 정보

	cltGameMsgResponse_Guild_ChangeVillage(cltGuildInfo*pclinfo)
	{
		clInfo.Set(pclinfo);
	}
};


//----------------------------------------------------------------
// 길드 리스트를 불러온다.
//----------------------------------------------------------------
class cltGameMsgRequest_Guild_GetGuildList
{
public:
	SI32				siVillageUnique;
	SI32				siInterfaceFlag;	// 어떤 인터페이스에서 메시지를 요청한건지 두는 플레그

	cltGameMsgRequest_Guild_GetGuildList(SI32 villageunique,SI32 InterfaceFlag = 0)
	{
		siVillageUnique = villageunique ;
		siInterfaceFlag = InterfaceFlag;
	}
};

class cltGameMsgResponse_Guild_GetGuildList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GUILD_PER_VILLAGE>		siUnique;
	NSafeTArray<SI16, MAX_GUILD_PER_VILLAGE>		siLevel;
	NSafeTArray<SI16, MAX_GUILD_PER_VILLAGE>		siMemberNum;
#else
	SI32				siUnique[MAX_GUILD_PER_VILLAGE]; // 길드 리스트
	SI16				siLevel[MAX_GUILD_PER_VILLAGE]; // 길드 레벨
	SI16				siMemberNum[MAX_GUILD_PER_VILLAGE];
#endif
	TCHAR				szName[MAX_GUILD_PER_VILLAGE][MAX_GUILD_NAME]; // 길드 이름
	TCHAR				szMasterName[MAX_GUILD_PER_VILLAGE][MAX_PLAYER_NAME]; // 길드장 이름

#ifdef _SAFE_MEMORY
	UI08				uiConfirm[MAX_GUILD_PER_VILLAGE]; // 승인 여부
	SI32				siVillageUnique[MAX_GUILD_PER_VILLAGE];			// 길드가 있는 건물 소속 국가
	SI32				siMoveVillageUnique[MAX_GUILD_PER_VILLAGE];
	SI32				siOwnedMapIndex[MAX_GUILD_PER_VILLAGE];			// 소유한 사냥터
#else
	NSafeTArray<UI08, MAX_GUILD_PER_VILLAGE>	uiConfirm;
	NSafeTArray<SI32, MAX_GUILD_PER_VILLAGE>	siVillageUnique;
	NSafeTArray<SI32, MAX_GUILD_PER_VILLAGE>	siMoveVillageUnique;
	NSafeTArray<SI32, MAX_GUILD_PER_VILLAGE>	siOwnedMapIndex;
#endif
	SI32				siInterfaceFlag;

	cltGameMsgResponse_Guild_GetGuildList(cltGuildInfo*pclinfo , SI32* mapindex, SI32 InterfaceFlag)
	{
		siInterfaceFlag = InterfaceFlag;

		for ( SI32 i = 0 ; i < MAX_GUILD_PER_VILLAGE ; i ++ )
		{
			siUnique[i] = pclinfo[i].siUnique ;
			siLevel[i] = pclinfo[i].clGPoint.siLevel ;
			MStrCpy(szName[i],pclinfo[i].szName,MAX_GUILD_NAME);
			uiConfirm[i] = pclinfo[i].uiConfirm ;
			siMemberNum[i] = pclinfo[i].GetMemberNum() ;
			siVillageUnique[i] = pclinfo[i].siVillageUnique;
			siMoveVillageUnique[i] = pclinfo[i].siMoveVillageUnique;
			siOwnedMapIndex[i] = mapindex[i];

			for ( SI32 j = 0 ; j < MAX_GUILD_MEMBER_NUM ; j ++ )
			{
				if ( pclinfo[i].siUnique != 0 && pclinfo[i].siMasterPersonID == pclinfo[i].clMember[j].siPersonID )
				{
					MStrCpy(szMasterName[i],pclinfo[i].clMember[j].szCharName,MAX_PLAYER_NAME);
					break;
				}
			}
		}
	}
};

//----------------------------------------
// 특정 길드의 정보를 불러온다.
//----------------------------------------
class cltGameMsgRequest_Guild_GetGuildInfo
{
public:
	SI32			siUnique; // 불러올 길드 유니크

	cltGameMsgRequest_Guild_GetGuildInfo(SI32 unique)
	{
		siUnique = unique ;
	}
};

class cltGameMsgResponse_Guild_GetGuildInfo
{
public:
	SI32			siResult; // 없는 길드일 수도 있다. 있으면 1 없으면 0
	cltGuildInfo	clInfo;

	cltGameMsgResponse_Guild_GetGuildInfo(SI32 result,cltGuildInfo*pclinfo)
	{
		siResult = result ;

		if ( pclinfo != NULL )
			clInfo.Set(pclinfo);
	}
};

//------------------------------------------------------
//길드 생성을 허가한다.
//------------------------------------------------------
class cltGameMsgRequest_Guild_ConfirmCreateGuild
{
public:
	SI32			siUnique; // 길드 유니크

	bool			bConfirm;

	cltGameMsgRequest_Guild_ConfirmCreateGuild(SI32 unique,bool confirm)
	{
		siUnique = unique ;
		bConfirm = confirm ;
	}
};

class cltGameMsgResponse_Guild_ConfirmCreateGuild
{
public:
	SI32			siResult; // 없는 길드일 수도 있다. 있으면 1 없으면 0

	cltGuildInfo	clInfo;

	bool			bConfirm;

	cltGameMsgResponse_Guild_ConfirmCreateGuild(SI32 result,cltGuildInfo*pclinfo,bool confirm)
	{
		siResult = result ;

		if ( pclinfo != NULL )
			clInfo.Set(pclinfo);

		bConfirm = confirm ;
	}
};

//------------------------------------------------------
// 부 길드장을 선출,강등한다.
//------------------------------------------------------
class cltGameMsgRequest_Guild_SetSecondMaster
{
public:
	SI32			siPersonID; // 길드 유니크

	bool			bSet;

	cltGameMsgRequest_Guild_SetSecondMaster(SI32 personid , bool set)
	{
		siPersonID = personid ;

		bSet = set ;
	}
};

class cltGameMsgResponse_Guild_SetSecondMaster
{
public:
	SI32			siResult; // 없는 길드일 수도 있다. 있으면 1 없으면 0

	cltGuildInfo	clInfo;

	bool			bSet;

	cltGameMsgResponse_Guild_SetSecondMaster(SI32 result,cltGuildInfo*pclinfo,bool set)
	{
		siResult = result ;

		if ( pclinfo != NULL )
			clInfo.Set(pclinfo);

		bSet = set ;
	}
};

//------------------------------------------------------
// 길드에 자금을 납부한다.
//------------------------------------------------------
class cltGameMsgRequest_Guild_GiveGuildMoney
{
public:
	GMONEY			siMoney; // 납부할 금액

	cltGameMsgRequest_Guild_GiveGuildMoney(GMONEY money)
	{
		siMoney = money ;
	}
};

class cltGameMsgResponse_Guild_GiveGuildMoney
{
public:
	SI32			siResult; // 없는 길드일 수도 있다. 있으면 1 없으면 0

	cltGuildInfo	clInfo;

	GMONEY			siMoney;
	GMONEY			siLeftMoney;

	cltGameMsgResponse_Guild_GiveGuildMoney(SI32 result,cltGuildInfo*pclinfo,GMONEY money,GMONEY leftmoney)
	{
		siResult = result ;

		if ( pclinfo != NULL )
			clInfo.Set(pclinfo);

		siMoney = money ;
		siLeftMoney = leftmoney ;
	}
};

//------------------------------------------------------
// 길드 건물 수수료를 설정한다. - 행수
//------------------------------------------------------
class cltGameMsgRequest_Guild_SetStrFeeRate
{
public:
	SI32			siVillageUnique;

	SI32			siFeeRate;

	cltGameMsgRequest_Guild_SetStrFeeRate(SI32 village,SI32 feerate)
	{
		siVillageUnique = village ;
		siFeeRate = feerate ;
	}
};

class cltGameMsgResponse_Guild_SetStrFeeRate
{
public:
	SI32			siResult;

	SI32			siVillageUnique;

	SI32			siFeeRate;

	cltGameMsgResponse_Guild_SetStrFeeRate(SI32 result,SI32 village,SI32 feerate)
	{
		siResult = result ;

		siVillageUnique = village ;
		siFeeRate = feerate ;
	}
};

//------------------------------------------------------
// 길드 능력치를 변경한다.
//------------------------------------------------------
class cltGameMsgRequest_Guild_ChangeStatus
{
public:
	cltGuildStatus		clinfo;

	cltGameMsgRequest_Guild_ChangeStatus(cltGuildStatus*pclinfo)
	{
		clinfo.siBonus = pclinfo->siBonus ;
		clinfo.siDex = pclinfo->siDex ;
		clinfo.siHnd = pclinfo->siHnd ;
		clinfo.siLuk = pclinfo->siLuk ;
		clinfo.siMag = pclinfo->siMag ;
		clinfo.siStr = pclinfo->siStr ;
		clinfo.siVit = pclinfo->siVit ;
	}
};

class cltGameMsgResponse_Guild_ChangeStatus
{
public:
	SI32			siResult;

	cltGuildStatus		clinfo;

	cltGameMsgResponse_Guild_ChangeStatus(SI32 result,cltGuildStatus*pclinfo)
	{
		siResult = result ;

		clinfo.siBonus = pclinfo->siBonus ;
		clinfo.siDex = pclinfo->siDex ;
		clinfo.siHnd = pclinfo->siHnd ;
		clinfo.siLuk = pclinfo->siLuk ;
		clinfo.siMag = pclinfo->siMag ;
		clinfo.siStr = pclinfo->siStr ;
		clinfo.siVit = pclinfo->siVit ;
	}
};

//-------------------------------------------
// 길드의 능력치를 길드원에게 통보한다.
//------------------------------------------
class cltGameMsgResponse_Guild_SetGuildStatus
{
public:
	cltGuildStatus		clinfo;
	cltSimpleGuildRank	clGuild;
	SI32				siCharUnique;

	cltGameMsgResponse_Guild_SetGuildStatus(cltGuildStatus*pclinfo,cltSimpleGuildRank*pclrank,SI32 charunique)
	{
		clinfo.siDex = pclinfo->siDex ;
		clinfo.siHnd = pclinfo->siHnd ;
		clinfo.siLuk = pclinfo->siLuk ;
		clinfo.siMag = pclinfo->siMag ;
		clinfo.siStr = pclinfo->siStr ;
		clinfo.siVit = pclinfo->siVit ;

		clGuild.Set(pclrank);

		siCharUnique = charunique ;
	}
};

//------------------------------------------------------
// 길드 마크를 결정한다.
//------------------------------------------------------
class cltGameMsgRequest_Guild_SetMark
{
public:
	SI32			siMarkIndex;

	cltGameMsgRequest_Guild_SetMark(SI32 index)
	{
		siMarkIndex = index;
	}
};

class cltGameMsgResponse_Guild_SetMark
{
public:
	SI32			siResult;

	SI32			siMarkIndex;

	cltGameMsgResponse_Guild_SetMark(SI32 result,SI32 index)
	{
		siResult = result ;

		siMarkIndex = index ;
	}
};

//------------------------------------------------------
// 길드의 이름을 바꾼다.
//------------------------------------------------------
class cltGameMsgRequest_Guild_ChangeName
{
public:
	TCHAR				szName[MAX_GUILD_NAME];

	cltGameMsgRequest_Guild_ChangeName(TCHAR*pName)
	{
		MStrCpy(szName,pName,MAX_GUILD_NAME);
	}
};

class cltGameMsgResponse_Guild_ChangeName
{
public:
	SI32				siResult;

	TCHAR				szName[MAX_GUILD_NAME];

	cltGameMsgResponse_Guild_ChangeName(SI32 result,TCHAR*pName)
	{
		siResult = result ;

		MStrCpy(szName,pName,MAX_GUILD_NAME);
	}
};


//-------------------------------------------------------
// 길드 마스터 변경
//-------------------------------------------------------
class cltGameMsgRequest_Guild_ChangeMaster
{
public:
	SI32			siToPersonID; // 새로 선출될 길드장 

	cltGameMsgRequest_Guild_ChangeMaster(SI32 toPersonID)
	{
		siToPersonID = toPersonID ;
	}
};
class cltGameMsgResponse_Guild_ChangeMaster
{
public:
	SI32			siResult; // 없는 길드일 수도 있다. 있으면 1 없으면 0

	cltGuildInfo	clInfo;

	cltGameMsgResponse_Guild_ChangeMaster(SI32 result,cltGuildInfo*pclinfo)
	{
		siResult = result ;

		if ( pclinfo != NULL )
			clInfo.Set(pclinfo);
	}
};

//--------------------------------------------------------
// 길드 폐쇄
//--------------------------------------------------------
class cltGameMsgResponse_Guild_CloseGuild
{
public:

	TCHAR			szGuildName[MAX_GUILD_NAME];

	cltGameMsgResponse_Guild_CloseGuild( TCHAR* guildname)
	{
		StringCchCopy( szGuildName, MAX_GUILD_NAME, guildname );
	}
};

//--------------------------------------------------------
// 길드전 예약 리스트 받아오기
//--------------------------------------------------------
class cltGameMsgRequest_GuildWar_ReserveList
{
public:
	SI32			siFieldIndex;

	cltGameMsgRequest_GuildWar_ReserveList( SI32 fieldindex )
	{
		siFieldIndex = fieldindex;
	}
};


class cltGameMsgResponse_GuildWar_ReserveList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltGuildWarReserveUnit, GUILDWAR_ROOMNUM>		clGuildWarList;
#else
	cltGuildWarReserveUnit	clGuildWarList[GUILDWAR_ROOMNUM];
#endif

	cltGameMsgResponse_GuildWar_ReserveList()
	{
	}
};

//---------------------------------------------------------
// 길드전 예약하기 - 더이상 안씀..
//---------------------------------------------------------
class cltGameMsgRequest_GuildWar_ApplyReserve
{
public:
	SI32			siFieldIndex;
	SI32			siRoomIndex;
	TCHAR			szEnemyGuildName[MAX_GUILD_NAME];

	cltGameMsgRequest_GuildWar_ApplyReserve( SI32 fieldindex, SI32 roomindex, TCHAR* enemyguildname )
	{
		siFieldIndex = fieldindex;
		siRoomIndex = roomindex;
		StringCchCopy( szEnemyGuildName, MAX_GUILD_NAME,  enemyguildname);
	}
};

class cltGameMsgResponse_GuildWar_ApplyReserve
{
public:
	SI32			siResult;
	SI32			siFieldIndex;

	cltGameMsgResponse_GuildWar_ApplyReserve( SI32 fieldindex, SI32 result )
	{
		siFieldIndex = fieldindex;
		siResult = result;
	}
};
//---------------------------------------------------------
// 상대 길드에게 길드전 신청
//--------------------------------------------------------- 
class cltGameMsgRequest_GuildWar_ApplyToEnemy
{
public:
	SI32	siFieldIndex;
	SI32	siRoomIndex;
	TCHAR	szEnemyGuildName[MAX_GUILD_NAME];

	cltGameMsgRequest_GuildWar_ApplyToEnemy( SI32 fieldindex, SI32 roomindex, TCHAR* enemyguildname )
	{
		siFieldIndex = fieldindex;
		siRoomIndex = roomindex;
		StringCchCopy( szEnemyGuildName, MAX_GUILD_NAME,  enemyguildname);
	}
};
// 서버에서 상대 길드로 메세지 보냄
class cltGameMsgResponse_GuildWar_ApplyToEnemy
{
public:
	SI32	siFieldIndex;
	SI32	siRoomIndex;
	SI32	siHomeGuildUnique;
	TCHAR	szHomeGuildName[MAX_GUILD_NAME];
	SI32	siAwayGuildUnique;
	TCHAR	szAwayGuildName[MAX_GUILD_NAME];

	cltGameMsgResponse_GuildWar_ApplyToEnemy()
	{
		ZeroMemory(this, sizeof(*this));
	}

	cltGameMsgResponse_GuildWar_ApplyToEnemy( SI32 fieldindex, SI32 roomindex,
		SI32 homeguild, TCHAR* homeguildname,
		SI32 awayguild, TCHAR* awayguildname )
	{
		siFieldIndex = fieldindex;
		siRoomIndex = roomindex;
		siHomeGuildUnique = homeguild;
		siAwayGuildUnique = awayguild;
		StringCchCopy( szHomeGuildName, MAX_GUILD_NAME,  homeguildname);
		StringCchCopy( szAwayGuildName, MAX_GUILD_NAME,  awayguildname);
	}

	void Set(cltGameMsgResponse_GuildWar_ApplyToEnemy* pInfo)
	{
		if(pInfo == NULL)		return;
		memcpy(this, pInfo, sizeof(*this));
	}
};
// 상대 길드에서의 응답
class cltGameMsgRequest_GuildWar_ConfirmApply
{
public:
	bool	bConfirm;

	SI32	siFieldIndex;
	SI32	siRoomIndex;
	SI32	siHomeGuildUnique;
	TCHAR	szHomeGuildName[MAX_GUILD_NAME];
	SI32	siAwayGuildUnique;
	TCHAR	szAwayGuildName[MAX_GUILD_NAME];

	cltGameMsgRequest_GuildWar_ConfirmApply( bool confirm, SI32 fieldindex, SI32 roomindex,
										SI32 homeguild, TCHAR* homeguildname,
										SI32 awayguild, TCHAR* awayguildname )
	{
		bConfirm = confirm;
		siFieldIndex = fieldindex;
		siRoomIndex = roomindex;
		siHomeGuildUnique = homeguild;
		siAwayGuildUnique = awayguild;
		StringCchCopy( szHomeGuildName, MAX_GUILD_NAME,  homeguildname);
		StringCchCopy( szAwayGuildName, MAX_GUILD_NAME,  awayguildname);
	}
};
//class cltGameMsgResponse_GuildWar_ConfirmApply
//{
//public:
//	bool	bConfirm;
//	TCHAR	szAwayGuildName[MAX_GUILD_NAME];
//
//	cltGameMsgResponse_GuildWar_ConfirmApply( bool confirm, TCHAR* guildname)
//	{
//		bConfirm = confirm;
//		StringCchCopy( szAwayGuildName, MAX_GUILD_NAME, guildname );
//	}
//
//};


//---------------------------------------------------------
// 길드전 시작 알림
//--------------------------------------------------------- 
class cltGameMsgResponse_GuildWar_Start
{
public:
	TCHAR		szHomeGuildName[MAX_GUILD_NAME];
	TCHAR		szAwayGuildName[MAX_GUILD_NAME];

	SI16		siHomeGuildMarkIndex;
	SI16		siAwayGuildMarkIndex;


	cltGameMsgResponse_GuildWar_Start( TCHAR* homename, TCHAR* awayname, SI16 homemark, SI16 awaymark )
	{
		StringCchCopy( szHomeGuildName, MAX_GUILD_NAME, homename );
		StringCchCopy( szAwayGuildName, MAX_GUILD_NAME, awayname );

		siHomeGuildMarkIndex = homemark;
		siAwayGuildMarkIndex = awaymark;
	}
};

//----------------------------------------------------------
// 길드전 종료 알림
//----------------------------------------------------------
class cltGameMsgResponse_GuildWar_End
{
public:
	SI32	siRecieveGP;	// 이전에는 받을 gp를 알려줬지만 변경후는 이 값에 길드전 결과를 날려 보냅니다. - 면 패배 + 면 승리.
	SI32	siRecieveMoney;	// 이제 gp나 받을 돈을 보내주는 대신에 승패만을 날려준다.

	cltGameMsgResponse_GuildWar_End( SI32 recvGP, SI32 recvMoney )
	{
		siRecieveGP = recvGP;
		siRecieveMoney = recvMoney;
	}
};

//----------------------------------------------------------
// 길드전 Kill Point 전달
//----------------------------------------------------------
class cltGameMsgResponse_GuildWar_KillPoint
{
public:
	SI32	siHomeGuildKill;
	SI32	siAwayGuildKill;

    cltGameMsgResponse_GuildWar_KillPoint( SI32 homekill, SI32 awaykill )
	{
		siHomeGuildKill = homekill;
		siAwayGuildKill = awaykill;
	}
};

//----------------------------------------------------------
// 사냥터 소유권 신청 리스트 받아오기
//----------------------------------------------------------
class cltGameMsgRequest_GuildDungeon_ApplyList
{
public:
	SI32	siMapIndex;

	cltGameMsgRequest_GuildDungeon_ApplyList( SI32 mapIndex )
	{
		siMapIndex = mapIndex;
	}
};

class cltGameMsgResponse_GuildDungeon_ApplyList
{
public:
	TCHAR	szGuildName[MAX_GUILDDUNGEONAPPLY][MAX_GUILD_NAME];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GUILDDUNGEONAPPLY>		siGuildPoint;
#else
	SI32	siGuildPoint[MAX_GUILDDUNGEONAPPLY];
#endif

	cltGameMsgResponse_GuildDungeon_ApplyList()
	{
		memset( this, 0, sizeof(cltGameMsgResponse_GuildDungeon_ApplyList) );
	}
};

//----------------------------------------------------------
// 사냥터 소유권 신청하기
//----------------------------------------------------------
class cltGameMsgRequest_GuildDungeon_Apply
{
public:
	SI32	siMapIndex;

	cltGameMsgRequest_GuildDungeon_Apply( SI32 mapIndex )
	{
		siMapIndex = mapIndex;
	}
};

class cltGameMsgResponse_GuildDungeon_Apply
{
public:
	bool	bSuccess;
	SI32	siMapIndex;

	cltGameMsgResponse_GuildDungeon_Apply( bool success, SI32 mapindex )
	{
		bSuccess = success;
		siMapIndex = mapindex;
	}
};

//----------------------------------------------------------
// 사냥터 소유권 현황
//----------------------------------------------------------
class cltGameMsgResponse_GuildDungeon_Status
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GUILDDUNGEON>		siMapIndex;
#else
	SI32	siMapIndex[MAX_GUILDDUNGEON];
#endif
	TCHAR	szGuildName[MAX_GUILDDUNGEON][MAX_GUILD_NAME];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI64, MAX_GUILDDUNGEON>		siExp;// SI32->SI64 경험치 오버플로우때문에 수정
	NSafeTArray<GMONEY, MAX_GUILDDUNGEON>	siMoney;
#else
	SI32	siExp[MAX_GUILDDUNGEON];
	GMONEY	siMoney[MAX_GUILDDUNGEON];
#endif

	cltGameMsgResponse_GuildDungeon_Status()
	{
		memset( this, 0, sizeof(cltGameMsgResponse_GuildDungeon_Status) );
	}
};

// 새로운 사냥터 소유권 현황 
class cltGameMsgResponse_GuildDungeon_Status_New
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GUILDDUNGEON>		siMapIndex;
#else
	SI32	siMapIndex[MAX_GUILDDUNGEON];
#endif
	TCHAR	szGuildName[MAX_GUILDDUNGEON][MAX_GUILD_NAME];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI64, MAX_GUILDDUNGEON>		siGP;// 경험치 대신배틀GP를 날린다.
	NSafeTArray<GMONEY, MAX_GUILDDUNGEON>	siMoney;
#else
	SI32	siGP[MAX_GUILDDUNGEON];
	GMONEY	siMoney[MAX_GUILDDUNGEON];
#endif

	cltGameMsgResponse_GuildDungeon_Status_New()
	{
		memset( this, 0, sizeof(cltGameMsgResponse_GuildDungeon_Status_New) );
	}
};

//----------------------------------------------------------
// 길드 멤버 리스트 요청
//----------------------------------------------------------
class cltGameMsgResponse_Guild_MemeberList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GUILD_MEMBER_NUM>		siPersonID;
#else
	SI32	siPersonID[MAX_GUILD_MEMBER_NUM];					// personid
#endif
	TCHAR	szName[MAX_GUILD_MEMBER_NUM][MAX_PLAYER_NAME];		// 이름
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_GUILD_MEMBER_NUM>		siMapIndex;
	NSafeTArray<bool, MAX_GUILD_MEMBER_NUM>		bConnected;
#else
	SI32	siMapIndex[MAX_GUILD_MEMBER_NUM];					// 현재 위치
	bool	bConnected[MAX_GUILD_MEMBER_NUM];					// 접속 여부
#endif
	cltGameMsgResponse_Guild_MemeberList()
	{
		ZeroMemory( this, sizeof(cltGameMsgResponse_Guild_MemeberList) );
	}
};
//----------------------------------------------------------
// 길드건물이 지어진 마을리스트
//----------------------------------------------------------
class cltGameMsgReponse_Guild_VillageList
{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<bool, MAX_VILLAGE_NUMBER>		bBuilt;
#else
	bool	bBuilt[MAX_VILLAGE_NUMBER];
#endif

	cltGameMsgReponse_Guild_VillageList()
	{
		ZeroMemory( this, sizeof(cltGameMsgReponse_Guild_VillageList) );
	}
};
//-----------------------------------------------------------
// 추천 길드 요청
//-----------------------------------------------------------
#define MAX_RECOMMANDGUILDLIST	30
class cltGameMsgResponse_GuildWar_RecommandList
{
public:
	TCHAR	szGuildName[MAX_RECOMMANDGUILDLIST][MAX_GUILD_NAME];
	TCHAR	szMasterName[MAX_RECOMMANDGUILDLIST][MAX_PLAYER_NAME];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_RECOMMANDGUILDLIST>		siLevel;
	NSafeTArray<SI32, MAX_RECOMMANDGUILDLIST>		siMemberCount;
#else
	SI32	siLevel[MAX_RECOMMANDGUILDLIST];
	SI32	siMemberCount[MAX_RECOMMANDGUILDLIST];
#endif

	cltGameMsgResponse_GuildWar_RecommandList()
	{
		ZeroMemory( this,  sizeof(cltGameMsgResponse_GuildWar_RecommandList) );
	}
};
#define MAX_NOTCONNECTMASTER	30
//-------------------------------------------------------
// 길드 마스터 위임 시스템
//-------------------------------------------------------
class cltGameMsgRequest_Guild_NotConnectMaster_ChangeMaster
{
public:
	SI16	siMaxDate;	// 길드장이 접속 안한 최대한의 기간.

	cltGameMsgRequest_Guild_NotConnectMaster_ChangeMaster( SI16 MaxDate )
	{
		siMaxDate = MaxDate ;
	}
};
class cltGameMsgResponse_NotConnectMaster_ChangeMaster
{
public:
	SI16			siResult; // 없는 길드일 수도 있다. 있으면 1 없으면 0

	cltGuildInfo	clInfo;

	cltGameMsgResponse_NotConnectMaster_ChangeMaster(SI32 result,cltGuildInfo*pclinfo)
	{
		siResult = result ;

		if ( pclinfo != NULL )
			clInfo.Set(pclinfo);
	}
};
//----------------------------------------------------------
// 사냥터 소유권 맵 신청 하기
//----------------------------------------------------------
class cltGameMsgRequest_GuildHuntMapWar_MapRegist
{
public:
	SI32	siMapIndex;

	cltGameMsgRequest_GuildHuntMapWar_MapRegist( SI32 mapIndex )
	{
		siMapIndex = mapIndex;
	}
};

class cltGameMsgResponse_GuildHuntMapWar_MapRegist
{
public:
	bool	bSuccess;
	SI32	siMapIndex;

	cltGameMsgResponse_GuildHuntMapWar_MapRegist( bool success, SI32 mapindex )
	{
		bSuccess = success;
		siMapIndex = mapindex;
	}
};
//----------------------------------------------------------
// 사냥터 소유권 맵 신청 하기
//----------------------------------------------------------
class cltGameMsgRequest_GuildHuntMapWar_MapRegister
{
public:
	SI32	siMapIndex;

	cltGameMsgRequest_GuildHuntMapWar_MapRegister( SI32 mapIndex )
	{
		siMapIndex = mapIndex;
	}
};

class cltGameMsgResponse_GuildHuntMapWar_MapRegister
{
public:
	bool	bSuccess;
	SI32	siMapIndex;

	cltGameMsgResponse_GuildHuntMapWar_MapRegister( bool success, SI32 mapindex )
	{
		bSuccess = success;
		siMapIndex = mapindex;
	}
};
//----------------------------------------------------------
// 사냥터 소유권 길드전 신청 하기
//----------------------------------------------------------
class cltGameMsgRequest_GuildHuntMapWarWar_Register
{
public:
	SI32	siGuildUnique;

	cltGameMsgRequest_GuildHuntMapWarWar_Register( SI32 guildunique )
	{
		siGuildUnique = guildunique;
	}
};

class cltGameMsgResponse_GuildHuntMapWarWar_Register
{
public:
	bool	bSuccess;
	SI32	siGuildUnique;

	cltGameMsgResponse_GuildHuntMapWarWar_Register( bool success, SI32 guildunique )
	{
		bSuccess = success;
		siGuildUnique = guildunique;
	}
};

#define  REQUEST_WARP_HUNTMAPGUILDWAR_WARP		0//시작 워프여부 질문
#define  REQUEST_WARP_HUNTMAPGUILDWAR_WARP_OK	1//워프 OK

//----------------------------------------------------------
// 길드전관련 질문 
//----------------------------------------------------------
class cltGameMsgRequest_GuildHuntMapWarWar_Request
{
public:
	SI32	siType	;
	SI32	siPersonID	;

	cltGameMsgRequest_GuildHuntMapWarWar_Request( SI32 Type,	SI32 PersonID )
	{
		siType = Type	;
		siPersonID = PersonID	;	
	}
};
class cltGameMsgResponse_GuildHuntMapWarWar_Request
{
public:
	SI32 siType	;
	SI32 siPersonID	;
	SYSTEMTIME stTime; 

	cltGameMsgResponse_GuildHuntMapWarWar_Request( SI32 Type, SI32 PersonID, SYSTEMTIME Time)
	{
		siType = Type ;
		siPersonID	=	PersonID	;
		stTime = Time	;
	}
};
//---------------------------------------------------------
// 길드맵 길드전 시작 알림
//--------------------------------------------------------- 
class cltGameMsgResponse_GuilHuntMapdWar_Start
{
public:

	SI32		siHomeGuildUnique	;
	SI32		siAwayGuildUnique	;
	TCHAR		szHomeGuildName[MAX_GUILD_NAME];
	TCHAR		szAwayGuildName[MAX_GUILD_NAME];
	SI16		siHomeGuildMarkIndex;
	SI16		siAwayGuildMarkIndex;


	cltGameMsgResponse_GuilHuntMapdWar_Start( SI32 Homeunique, SI32  Awayunique, TCHAR* homename,TCHAR* awayname,SI16 homemark, SI16 awaymark )
	{
		siHomeGuildUnique = Homeunique;
		siAwayGuildUnique = Awayunique;

		StringCchCopy( szHomeGuildName, MAX_GUILD_NAME, homename );
		StringCchCopy( szAwayGuildName, MAX_GUILD_NAME, awayname );

		siHomeGuildMarkIndex = homemark;
		siAwayGuildMarkIndex = awaymark;
	}
};
//----------------------------------------------------------
// 길드사냥터 소유방식변경 길드순위를 보여준다.
//----------------------------------------------------------
class cltGameMsgRequest_GuilHuntMapdWar_GetTopTen
{
public:
	SI32	siMapIndex;
	class cltGameMsgRequest_GuilHuntMapdWar_GetTopTen( SI32 mapIndex )
	{
		siMapIndex = mapIndex;
	}
};

class cltGameMsgResponse_GuilHuntMapdWar_GetTopTen
{
public:
	cltGuildHuntMapWarListRankData	Rank[MAX_GUILDHUNTWAR_RANK_NUM];

	cltGameMsgResponse_GuilHuntMapdWar_GetTopTen(  cltGuildHuntMapWarListRankData* pRank )
	{
		memcpy(Rank,pRank,sizeof(Rank));
	}
};
// 
class cltGameMsgResponse_GuilHuntMapdWar_ADDKillPoint
{
public:
	TCHAR			szKillerName[MAX_PLAYER_NAME];
	TCHAR			szKilledName[MAX_PLAYER_NAME];
	SI32			siHomeAway	;

	cltGameMsgResponse_GuilHuntMapdWar_ADDKillPoint(  TCHAR* KilleeName, TCHAR* KilledName, SI32 HomeAway )
	{
		MStrCpy(szKillerName,KilleeName,MAX_PLAYER_NAME);
		MStrCpy(szKilledName,KilledName,MAX_PLAYER_NAME);
		siHomeAway	=	HomeAway;
	}
};
//class cltGameMsgResponse_SendUserList// 클라이언트에 유저리스트를 날려준다.
//{
//public:
//	InGuidWar HomeUseerList[MAX_INMAPPERSONNUM]	;	
//	InGuidWar AwayUseerList[MAX_INMAPPERSONNUM]	;	
//
//	cltGameMsgResponse_SendUserList( InGuidWar* pHomUserList, InGuidWar* pAwayUserList  )
//	{
//		memcpy(HomeUseerList,pHomUserList,sizeof(InGuidWar)*MAX_INMAPPERSONNUM);
//		memcpy(AwayUseerList,pAwayUserList,sizeof(InGuidWar)*MAX_INMAPPERSONNUM);
//	}
//};
//enum
//{
//	END_WIN	=	0,	// 승 무 패 
//	END_DRAW,
//	END_LOSE,
//	END_NORESULT,
//
//};
//----------------------------------------------------------
// 길드전 종료 알림
//----------------------------------------------------------
class cltGameMsgResponse_GuilHuntMapdWar_End
{
public:
	SI32 siResult ;
	cltEndWarSrarus							pEndData_Home[MAX_GUILD_MEMBER_NUM]	;
	cltEndWarSrarus							pEndData_Away[MAX_GUILD_MEMBER_NUM]	;

	cltGameMsgResponse_GuilHuntMapdWar_End(SI32 Result  , cltEndWarSrarus* EndData_Home, cltEndWarSrarus* EndData_Away  )
	{
		siResult = Result	;
		memcpy(pEndData_Home, EndData_Home, sizeof(cltEndWarSrarus)* MAX_GUILD_MEMBER_NUM)	;
		memcpy(pEndData_Away, EndData_Away, sizeof(cltEndWarSrarus)* MAX_GUILD_MEMBER_NUM)	;
	}
};
//----------------------------------------------------------
// 길드전 매칭결과 알림.
//----------------------------------------------------------
class cltGameMsgResponse_GuilHuntMapdWar_SendMatchingResult
{
public:
	TCHAR			szMyGuildName[MAX_PLAYER_NAME];
	TCHAR			szEnemyName[MAX_PLAYER_NAME];

	cltGameMsgResponse_GuilHuntMapdWar_SendMatchingResult( TCHAR* MyGuildName, TCHAR* EnemyName )
	{
		MStrCpy(szMyGuildName,MyGuildName,MAX_PLAYER_NAME);	// 내 길드 이름
		MStrCpy(szEnemyName,EnemyName,MAX_PLAYER_NAME);		// 적 길드 이름
	}
};
//----------------------------------------------------------
// 시간물어보기
//----------------------------------------------------------
class cltGameMsgRequest_GuildHuntMapWarWar_Time
{
public:	
	SI32 siPersonID	;
	cltGameMsgRequest_GuildHuntMapWarWar_Time( SI32 PersonID)
	{
		siPersonID	= PersonID	;
	}
};
class cltGameMsgResponse_GuildHuntMapWarWar_Time
{
public:
	_SYSTEMTIME	stServerTime	;

	cltGameMsgResponse_GuildHuntMapWarWar_Time( _SYSTEMTIME SerTime )
	{
		stServerTime.wMinute = SerTime.wMinute	;
		stServerTime.wSecond = SerTime.wSecond	;
	}
};
//----------------------------------------------------------
// 맵을 신청 했는지 안했는지 알아 온다
//----------------------------------------------------------
class cltGameMsgRequest_GuildHuntMapWarWar_HaveMapIndex
{
public:	
	SI32 siPersonID	;
	cltGameMsgRequest_GuildHuntMapWarWar_HaveMapIndex( SI32 PersonID)
	{
		siPersonID	= PersonID	;
	}
};
class cltGameMsgResponse_GuildHuntMapWarWar_HaveMapIndex
{
public:
	SI32 siMApIndex ;
	cltGameMsgResponse_GuildHuntMapWarWar_HaveMapIndex( SI32 MApIndex )
	{
		siMApIndex	=	MApIndex	;
	}
};
#endif

