#ifndef _MSGTYPE_EMPIRE_H
#define _MSGTYPE_EMPIRE_H

#include "../../common/CommonHeader.h"
#include "../../Server/Empire/EmpireManager.h"

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_InviteEmpire
//
// Last Modified 	: 2007 / 10 / 19 
// Created		 	: 김광명
//
// Function			: [요청]제국에 초대한다.
//
//*************************************************************************************************** 
class cltGameMsgRequest_InviteEmpire
{
public:

	enum { VILLAGE = 0, GUILD };
	/// 제국으로 초대하는 종류 
	SI08	siInviteType;
	/// 제국으로 초대할 사람의 이름
	TCHAR	szUserName[MAX_PLAYER_NAME];
	
	cltGameMsgRequest_InviteEmpire( SI08 InviteType , TCHAR* UserName )
	{
		siInviteType = InviteType;
		StringCchCopy( szUserName, MAX_PLAYER_NAME, UserName );
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_InviteEmpire
//
// Last Modified 	: 2007 / 10 / 19 
// Created		 	: 김광명
//
// Function			: [요청]제국에 초대한다.
//
//*************************************************************************************************** 
class cltGameMsgResponse_InviteEmpire
{
public:

	enum { VILLAGE = 0, GUILD, EMPEROR };
	/// 제국으로 초대하는 종류 
	SI08	siInviteType;
	/// 초대메시지를 보낸 제국 유니크
	SI32	siEmpireUnique;
	/// 초대메시지를 보내는 제국 마을 이름
	SI32	siEmproerVillageUnique;
	/// 초대를 보낸 사람의 PersonID
	SI32	siFromPersonID;

	cltGameMsgResponse_InviteEmpire( SI08 InviteType , SI32 EmpireUnique, SI32 FromPersonID, SI32 EmproerVillageUnique )
	{
		siInviteType = InviteType;
		siEmpireUnique = EmpireUnique;
		siFromPersonID = FromPersonID;
		siEmproerVillageUnique = EmproerVillageUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_JoinEmpire
//
// Last Modified 	: 2007 / 10 / 24 
// Created		 	: 김광명
//
// Function			: [요청] 제국 초대에 응한다.
//
//*************************************************************************************************** 
class cltGameMsgRequest_JoinEmpire
{
public:

	enum { VILLAGE = 0, GUILD };
	/// 제국으로 초대되는 종류 
	SI08	siInviteType;
	/// 초대한 사람 PersonID
	SI32	siFromPersonID;
	/// 초대메시지를 보낸 제국 유니크
	SI32	siEmpireUnique;


	cltGameMsgRequest_JoinEmpire( SI08 InviteType , SI32 FromPersonID, SI32 EmpireUnique )
	{
		siInviteType = InviteType;
		siFromPersonID = FromPersonID;
		siEmpireUnique = EmpireUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_JoinEmpire
//
// Last Modified 	: 2007 / 10 / 24 
// Created		 	: 김광명
//
// Function			: [응답] 제국 초대에 응한다.
//
//*************************************************************************************************** 
class cltGameMsgResponse_JoinEmpire
{
public:

	enum { EMPEROR = 0, OTHERUSER };
	/// 초대한 사람과 초대된 사람을 구별하기 위해
	SI08	siSendType;
	/// 초대된 제국유니크
	SI32	siEmpireUnique;
	/// 초대된 마을유니크
	SI32	siVillageUnique;
	/// 초대된 길드유니크
	SI32	siGuildUnique;
	

	cltGameMsgResponse_JoinEmpire( SI08 SendType , SI32 EmpireUnique, SI32 VillageUnique, SI32 GuildUnique )
	{
		siSendType = SendType;
		siEmpireUnique = EmpireUnique;
		siVillageUnique = VillageUnique;
		siGuildUnique = GuildUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_UpdateEmpireInfo
//
// Last Modified 	: 2007 / 10 / 22 
// Created		 	: 김광명
//
// Function			: [응답] 클라이언트로 내 자신의 제국 정보를 업데이트 시킨다.
//
//*************************************************************************************************** 
class cltGameMsgResponse_UpdateEmpireInfo
{
public:

	enum { VILLAGE = 0, GUILD };
	// 제국 정보가 업데이트 된 사람
	SI16		siCharUnique;		
	SI08		siUpdateType;
	CEmpireInfo clEmpireInfo;

	cltGameMsgResponse_UpdateEmpireInfo( SI16 CharUnique, SI08 UpdateType, SI08 RankType, SI32 EmpireUnique )
	{
		siCharUnique = CharUnique;
		siUpdateType = UpdateType;
		clEmpireInfo.SetEmpireInfo( RankType, EmpireUnique );
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_EmpireInfo
//
// Last Modified 	: 2007 / 10 / 22 
// Created		 	: 김광명
//
// Function			: [요청] 제국 정보를 요청한다.
//
//*************************************************************************************************** 
class cltGameMsgRequest_EmpireInfo
{
public:

	/// 요청이 처음인가?( 처음 요청할때는 bRequestFirst = true, 마을유니크는 0
	bool bRequestFirst;
	/// 일반 유저의 요청인가, 대행수의 요청인가?
	bool bVillageChief;
	/// 요청할 마을유니크
	SI32 siVillageUnique;

	cltGameMsgRequest_EmpireInfo( bool RequestFirst, bool VillageChief, SI32 VillageUnique )
	{
		bRequestFirst = RequestFirst;
		bVillageChief = VillageChief;
		siVillageUnique = VillageUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_EmpireInfo
//
// Last Modified 	: 2007 / 10 / 22 
// Created		 	: 김광명
//
// Function			: [응답] 제국 정보를 요청한다.
//
//*************************************************************************************************** 
class cltGameMsgResponse_EmpireInfo
{
public:
	bool bRequestFirst;
	bool bVillageChief;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_EMPIRENUMBER>				siEmperprVillageUnique;
#else
	SI32 siEmperprVillageUnique[MAX_EMPIRENUMBER];
#endif
	CEmpireUnitInfo clEmpireInfo;
	TCHAR szVillageChiefName[MAX_EMPIRE_IN_VILLAGE][MAX_PLAYER_NAME];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_EMPIRE_IN_VILLAGE>		siVillageMemberNumber;
#else
	SI32 siVillageMemberNumber[MAX_EMPIRE_IN_VILLAGE];
#endif
	TCHAR szGuildChiefName[MAX_EMPIRE_IN_GUILD][MAX_PLAYER_NAME];
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_EMPIRE_IN_GUILD>			siGuildMemberNumber;
#else
	SI32 siGuildMemberNumber[MAX_EMPIRE_IN_GUILD];
#endif

	cltGameMsgResponse_EmpireInfo( bool RequestFirst, bool VillageChief, SI32* EmperprVillageUnique, CEmpireUnitInfo* EmpireInfo,
		                           TCHAR VillageChiefName[][MAX_PLAYER_NAME], SI32* VillageMemberNumber,
								   TCHAR GuildChiefName[][MAX_PLAYER_NAME], SI32* GuildMemberNumber )
	{
		bRequestFirst = RequestFirst;
		bVillageChief = VillageChief;

		SI32 i;
		if( NULL == EmperprVillageUnique )
		{
			for( i = 0; i < MAX_EMPIRENUMBER; ++i )
			{
				siEmperprVillageUnique[i] = 0;
			}
		}
		else
		{
			for( i = 0; i < MAX_EMPIRENUMBER; ++i )
			{
				siEmperprVillageUnique[i] = EmperprVillageUnique[i];
			}
		}

		if( NULL == EmpireInfo )
		{
			clEmpireInfo.InitEmpireUnitInfo();
		}
		else
		{
			clEmpireInfo.SetEmpireUnitInfo( EmpireInfo );
		}

		for( i = 0; i < MAX_EMPIRE_IN_VILLAGE; ++i )
		{
			if( NULL == VillageChiefName )
			{
				StringCchCopy( szVillageChiefName[i], MAX_PLAYER_NAME, "" );
			}
			else
			{
				StringCchCopy( szVillageChiefName[i], MAX_PLAYER_NAME, VillageChiefName[i] );
			}

			if( NULL == VillageMemberNumber )
			{
				siVillageMemberNumber[i] = 0;
			}
			else
			{
				siVillageMemberNumber[i] = VillageMemberNumber[i];
			}
		}

		for( i = 0; i < MAX_EMPIRE_IN_GUILD; ++i )
		{
			if( NULL == GuildChiefName )
			{
				StringCchCopy( szGuildChiefName[i], MAX_PLAYER_NAME, "" );
			}
			else
			{
				StringCchCopy( szGuildChiefName[i], MAX_PLAYER_NAME, GuildChiefName[i] );
			}

			if( NULL == GuildMemberNumber )
			{
				siGuildMemberNumber[i] = 0;
			}
			else
			{
				siGuildMemberNumber[i] = GuildMemberNumber[i];
			}
		}
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_SecedeEmpire
//
// Last Modified 	: 2007 / 10 / 25 
// Created		 	: 김광명
//
// Function			: [요청]제국에서 탈퇴한다.
//
//*************************************************************************************************** 
class cltGameMsgRequest_SecedeEmpire
{
public:

	enum { EMPEROR = 0, VILLAGE_CHIEF, GUILD_CHIEF };
	/// 제국에서 탈퇴할려고 요청한 사람
	SI08 siSecedeType;
	/// 제국에서 탈퇴할 마을유니크 
	SI32 siVillageUnique;
	/// 제국에서 탈퇴할 제국유니크
	SI32 siGuildUnique;

	cltGameMsgRequest_SecedeEmpire( SI08 SecedeType , SI32 VillageUnique, SI32 GuildUnique  )
	{
		siSecedeType = SecedeType;
		siVillageUnique = VillageUnique;
		siGuildUnique = GuildUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgResponse_SecedeEmpire
//
// Last Modified 	: 2007 / 10 / 19 
// Created		 	: 김광명
//
// Function			: [응답]제국에서 탈퇴한다.
//
//*************************************************************************************************** 
class cltGameMsgResponse_SecedeEmpire
{
public:

	enum { VILLAGE = 0, GUILD, EMPEROR };
	/// 제국으로 초대하는 종류 
	SI08	siInviteType;
	/// 초대메시지를 보낸 제국 유니크
	SI32	siEmpireUnique;
	/// 초대메시지를 보내는 제국 마을 이름
	SI32	siEmproerVillageUnique;
	/// 초대를 보낸 사람의 PersonID
	SI32	siFromPersonID;

	cltGameMsgResponse_SecedeEmpire( SI08 InviteType , SI32 EmpireUnique, SI32 FromPersonID, SI32 EmproerVillageUnique )
	{
		siInviteType = InviteType;
		siEmpireUnique = EmpireUnique;
		siFromPersonID = FromPersonID;
		siEmproerVillageUnique = EmproerVillageUnique;
	}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_SetEmpireMoney
//
// Last Modified 	: 2007 / 10 / 25 
// Created		 	: 김광명
//
// Function			: [요청]제국 자본금을 납부한다.( 국가 이익금으로 가능 )
//
//*************************************************************************************************** 
class cltGameMsgRequest_SetEmpireMoney
{
public:

	/// 제국 자본금에 넣을 돈
	SI64 siInputMoney;
	
	cltGameMsgRequest_SetEmpireMoney( SI64 InputMoney ) : siInputMoney( InputMoney ) {}
};

//***************************************************************************************************
//
// Class Name 		: cltGameMsgRequest_SetEmpireMoney
//
// Last Modified 	: 2007 / 10 / 25 
// Created		 	: 김광명
//
// Function			: [응답]제국 자본금을 납부한다.( 국가 이익금으로 가능 )
//
//*************************************************************************************************** 
class cltGameMsgResponse_SetEmpireMoney
{
public:

	/// 제국 자본금에 넣을 돈
	SI64 siInputMoney;

	cltGameMsgResponse_SetEmpireMoney( SI64 InputMoney ) : siInputMoney( InputMoney ) {}
};

#endif 