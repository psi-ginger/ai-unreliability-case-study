#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-Guild.h"
#include "../../CommonLogic/Guild/GuildHuntMapWarManager.h"

void DBGameProtocol::GetGuildStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_GUILD;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		GetFeeInfo( &sendMsg.clStrInfo.clGuildStrInfo.clFee );
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney, 0);
		m_odbc.GetData( sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
	END_FETCHDATA()

	// 건물 기본정보
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siStrDura );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemUnique );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemNum );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siUseItemNum );
			m_odbc.GetData( &(sendMsg.clStrInfo.clCommonInfo.clBaseMoney.itMoney) );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siRepairStrDura );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// 세금정보를 얻는다.
	GetTaxList( &sendMsg.clStrInfo.clCommonInfo.clTaxInfo, sendMsg.siVillageUnique, sendMsg.clStrInfo.clCommonInfo.siStrType );

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::CreateGuild()
{
	// 받은 패킷
	sDBRequest_Guild_CreateGuild *pRecvMsg = (sDBRequest_Guild_CreateGuild *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_CreateGuild sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CREATEGUILD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siMAX_GUILD_NUM = MAX_GUILD_NUM;
	SI32 siMAX_GUILD_PER_VILLAGE = MAX_GUILD_PER_VILLAGE;

	// DB Query
	BEGIN_SP( "rsp_GuildCreate" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGuildName, sizeof(pRecvMsg->szGuildName) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clDate, sizeof(pRecvMsg->clDate) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_GUILD_NUM, sizeof(siMAX_GUILD_NUM) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_GUILD_PER_VILLAGE, sizeof(siMAX_GUILD_PER_VILLAGE) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
	END_FETCHDATA()

	// 길드정보를 받는다.
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		TCHAR log[512] = "";
		sprintf(log, "VillageUnique = %d, GuildName = %s"
			, pRecvMsg->siVillageUnique, pRecvMsg->szGuildName
			);
		sDBLogMsg_QueryString LogMsg("Guild Create", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::DestroyGuild()
{
	// 받은 패킷
	sDBRequest_Guild_DestroyGuild *pRecvMsg = (sDBRequest_Guild_DestroyGuild *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_DestroyGuild sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_DESTROYGUILD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	char szGuildName[MAX_GUILD_NAME] = "";

	// DB Query
	BEGIN_SP( "rsp_GuildDestroy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( szGuildName, sizeof(szGuildName));
	END_FETCHDATA()

	sendMsg.siGuildUnique = pRecvMsg->siGuildUnique;
	sendMsg.siReason = pRecvMsg->siReason;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildName = %s, GuildUnique = %d, VillageUnique = %d"
			, szGuildName, pRecvMsg->siGuildUnique,  pRecvMsg->siVillageUnique
			);
		sDBLogMsg_QueryString LogMsg("Guild Destroy", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildChangeVillage()
{
	// 받은 패킷
	sDBRequest_Guild_ChangeVillage *pRecvMsg = (sDBRequest_Guild_ChangeVillage *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_ChangeVillage sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CHANGEVILLAGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	SI32 siMAX_GUILD_PER_VILLAGE = MAX_GUILD_PER_VILLAGE;

	// DB Query
	BEGIN_SP( "rsp_GuildChangeVillage" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLeaveVillageUnique, sizeof(pRecvMsg->siLeaveVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_GUILD_PER_VILLAGE, sizeof(siMAX_GUILD_PER_VILLAGE) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildName = %s, GuildUnique = %d, ChangeVillageUnique = %d"
			, sendMsg.clInfo.szName, pRecvMsg->siGuildUnique, pRecvMsg->siVillageUnique
			);
		sDBLogMsg_QueryString LogMsg("Guild Change Village", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ConfirmJoinGuild()
{
	// 받은 패킷
	sDBRequest_Guild_ConfirmJoinGuild *pRecvMsg = (sDBRequest_Guild_ConfirmJoinGuild *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_ConfirmJoinGuild sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CONFIRMJOINGUILD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siMAX_GUILD_MEMBER_NUM = MAX_GUILD_MEMBER_NUM;

	// DB Query
	BEGIN_SP( "rsp_GuildJoinConfirm" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bConfirm, sizeof(pRecvMsg->bConfirm) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStandingPersonID, sizeof(pRecvMsg->siStandingPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_GUILD_MEMBER_NUM, sizeof(siMAX_GUILD_MEMBER_NUM) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.bConfirm );
		m_odbc.GetData( &sendMsg.siStandingPersonID );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, GuildName = %s, Confirm = %d"
			, sendMsg.clInfo.siUnique, sendMsg.clInfo.szName, sendMsg.bConfirm
			);
		sDBLogMsg_QueryString LogMsg("Guild Join Confirm", sendMsg.siPersonID, sendMsg.siStandingPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::LeaveGuild()
{
	// 받은 패킷
	sDBRequest_Guild_LeaveGuild *pRecvMsg = (sDBRequest_Guild_LeaveGuild *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_LeaveGuild sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_LEAVEGUILD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildLeave" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, GuildName = %s"
			, pRecvMsg->siGuildUnique, sendMsg.clInfo.szName
			);
		sDBLogMsg_QueryString LogMsg("Guild Leave", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::JoinGuild()
{
	// 받은 패킷
	sDBRequest_Guild_JoinGuild *pRecvMsg = (sDBRequest_Guild_JoinGuild *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_JoinGuild sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_JOINGUILD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siMAX_GUILD_MEMBER_NUM = MAX_GUILD_MEMBER_NUM;

	// DB Query
	BEGIN_SP( "rsp_GuildJoin" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clInfo.siUnique, sizeof(pRecvMsg->clInfo.siUnique) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->clInfo.szName, sizeof(pRecvMsg->clInfo.szName) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_GUILD_MEMBER_NUM, sizeof(siMAX_GUILD_MEMBER_NUM) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, GuildName = %s"
			, pRecvMsg->clInfo.siUnique, sendMsg.clInfo.szName
			);
		sDBLogMsg_QueryString LogMsg("Guild Join", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetGuildInfo()
{
	// 받은 패킷
	sDBRequest_Guild_GetGuildInfo *pRecvMsg = (sDBRequest_Guild_GetGuildInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_GetGuildInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_GETGUILDINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.siCharID = pRecvMsg->siCharID;

	cltGuildInfo clGuild;

	// DB Query
	BEGIN_SP( "rsp_GuildTotalInfoGet" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	// 길드정보 얻기
	SI32 siIndex = 0;
	SI32 siCount = 0;
	sendMsg.siStartPos = 0;

	while(siIndex < MAX_GUILD_NUM)
	{
		clGuild.Init();

		GetGuildInfo(&clGuild);
		sendMsg.clInfo[siCount].Set(&clGuild);

		siCount++;
		siIndex++;

		if(siCount >= MAX_GUILD_PER_DBLIST)
		{
			// send
			SendPacket( (BYTE *)&sendMsg );

			siCount = 0;
			sendMsg.siStartPos = siIndex;
		}
	}
}

void DBGameProtocol::ConfirmCreateGuild()
{
	// 받은 패킷
	sDBRequest_Guild_ConfirmCreateGuild *pRecvMsg = (sDBRequest_Guild_ConfirmCreateGuild *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_ConfirmCreateGuild sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siGuildUnique = pRecvMsg->siUnique;

	SI32 siGMPersonID = 0;
	SI32 siMAX_GUILD_PER_VILLAGE = MAX_GUILD_PER_VILLAGE;

	// DB Query
	BEGIN_SP( "rsp_GuildCreateConfirm" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siUnique, sizeof(pRecvMsg->siUnique) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bConfirm, sizeof(pRecvMsg->bConfirm) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clDate, sizeof(pRecvMsg->clDate) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_GUILD_PER_VILLAGE, sizeof(siMAX_GUILD_PER_VILLAGE) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.bConfirm );
		m_odbc.GetData( &sendMsg.siMoney );
		m_odbc.GetData( &sendMsg.siFee );
		m_odbc.GetData( &sendMsg.siLeftMoney );
		m_odbc.GetData( &sendMsg.clDate );
		m_odbc.GetData( &siGMPersonID );
		m_odbc.GetData( &sendMsg.siBeforeConfirm );
		m_odbc.GetData( &sendMsg.bState );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, GuildName = %s, Confirm = %d, State = %d, BeforeConfirm = %d, MoneyState = %d, Money = %I64d, LeftMoney = %I64d, Fee = %I64d"
			, pRecvMsg->siUnique, sendMsg.clInfo.szName, sendMsg.bConfirm, sendMsg.bState, sendMsg.siBeforeConfirm, sendMsg.bState, sendMsg.siMoney, sendMsg.siLeftMoney, sendMsg.siFee
			);
		sDBLogMsg_QueryString LogMsg("Guild ConfirmCreate", siGMPersonID, sendMsg.siPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GiveGuildMoney()
{
	// 받은 패킷
	sDBRequest_Guild_GiveGuildMoney *pRecvMsg = (sDBRequest_Guild_GiveGuildMoney *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_Guild_GiveGuildMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_GIVEGUILDMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildMoneyGive" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siMoney );
		m_odbc.GetData( &sendMsg.siLeftMoney );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clinfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, GiveMoney = %I64d, LeftMoney = %I64d, GuildUnique = %d, GuildName = %s"
			, pRecvMsg->siGuildUnique, sendMsg.siMoney, sendMsg.siLeftMoney, sendMsg.clinfo.siUnique, sendMsg.clinfo.szName
			);
		sDBLogMsg_QueryString LogMsg("Guild GiveMoney", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);


	}
}

void DBGameProtocol::GuildSetSecondMaster()
{
	// 받은 패킷
	sDBRequest_Guild_SetSecondMaster *pRecvMsg = (sDBRequest_Guild_SetSecondMaster *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_SetSecondMaster sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_SETSECONDMASTER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildSecondMasterSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSecondMasterPersonID, sizeof(pRecvMsg->siSecondMasterPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bSet, sizeof(pRecvMsg->bSet) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siSecondMasterPersonID );
		m_odbc.GetData( &sendMsg.bSet );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, " Set = %d, GuildUnique = %d, GuildName = %s"
			, sendMsg.bSet, sendMsg.clInfo.siUnique, sendMsg.clInfo.szName
			);
		sDBLogMsg_QueryString LogMsg("Guild SetSecondMaster", sendMsg.siSecondMasterPersonID, sendMsg.siPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildSetStrFeeRate()
{
	// 받은 패킷
	sDBRequest_Guild_SetStrFeeRate *pRecvMsg = (sDBRequest_Guild_SetStrFeeRate *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_SetStrFeeRate sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_SETSTRFEERATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildStrFeeRateSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFeeRate, sizeof(pRecvMsg->siFeeRate) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siVillageUnique);
		m_odbc.GetData( &sendMsg.siFeeRate );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, FeeRate = %d"
			, sendMsg.siVillageUnique, sendMsg.siFeeRate
			);
		sDBLogMsg_QueryString LogMsg("Guild Set StrFeeRate", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ChangeGuildPoint()
{
	// 받은 패킷
	sDBRequest_Guild_ChangeGuildPoint *pRecvMsg = (sDBRequest_Guild_ChangeGuildPoint *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_ChangeGuildPoint sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CHANGEGUILDPOINT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siLeftGP = 0, siLeftBattleGP = 0;

	// DB Query
	BEGIN_SP( "rsp_GuildPointChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siExp, sizeof(pRecvMsg->siExp) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMax, sizeof(pRecvMsg->siMax) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &siLeftGP );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clinfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, GuildUnique = %d, Type = %d, Max = %d, AddGP = %d, LeftGP = %d, LeftGuildPoint = %d, LeftBattleGP = %d"
			, sendMsg.siResult, pRecvMsg->siGuildUnique, pRecvMsg->siType, pRecvMsg->siMax, pRecvMsg->siExp, siLeftGP, sendMsg.clinfo.clGPoint.siPoint, sendMsg.clinfo.clGPoint.siBattleGP
			);
		sDBLogMsg_QueryString LogMsg("Guild Change GuildPoint", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildGiveFee()
{
	// 받은 패킷
	sDBRequest_Guild_Give_Fee *pRecvMsg = (sDBRequest_Guild_Give_Fee *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_Give_Fee sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_GIVE_FEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildGiveFee" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clDate, sizeof(pRecvMsg->clDate) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siLeftStrMoney);
		m_odbc.GetData( &sendMsg.siFaultNum);
		m_odbc.GetData( &sendMsg.siGuildUnique );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clinfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, Fee = %I64d, LeftStrMoney = %I64d, FaultNum = %d"
			, sendMsg.siGuildUnique, pRecvMsg->siFee, sendMsg.siLeftStrMoney, sendMsg.siFaultNum
			);
		sDBLogMsg_QueryString LogMsg("Guild Give Fee", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildChangeStatus()
{
	// 받은 패킷
	sDBRequest_Guild_ChangeStatus *pRecvMsg = (sDBRequest_Guild_ChangeStatus *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_ChangeStatus sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CHANGESTATUS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildStatusChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		SetGuildStatus( &pRecvMsg->clinfo );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clinfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, Str = %d, Dex = %d, Vit = %d, Mag = %d, Hnd = %d, Luck = %d, Bonus = %d"
			, pRecvMsg->siGuildUnique, sendMsg.clinfo.clStatus.siStr, sendMsg.clinfo.clStatus.siDex, sendMsg.clinfo.clStatus.siVit, sendMsg.clinfo.clStatus.siMag, sendMsg.clinfo.clStatus.siHnd, sendMsg.clinfo.clStatus.siLuk, sendMsg.clinfo.clStatus.siBonus
			);
		sDBLogMsg_QueryString LogMsg("Guild Status Change", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildLevelUp()
{
	// 받은 패킷
	sDBRequest_Guild_LevelUP *pRecvMsg = (sDBRequest_Guild_LevelUP *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_LevelUP sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_LEVELUP;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildLevelUp" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLevel, sizeof(pRecvMsg->siLevel) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clinfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, Level = %d"
			, pRecvMsg->siGuildUnique, pRecvMsg->siLevel
			);
		sDBLogMsg_QueryString LogMsg("Guild Status Change", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildChangeName()
{
	// 받은 패킷
	sDBRequest_Guild_ChangeName *pRecvMsg = (sDBRequest_Guild_ChangeName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_ChangeName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CHANGENAME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	TCHAR szName[MAX_GUILD_NAME];

	// DB Query
	BEGIN_SP( "rsp_GuildChangeName" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szName, sizeof(pRecvMsg->szName) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( szName, sizeof(szName) );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clinfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, Name = %s, BeforeName = %s"
			, pRecvMsg->siGuildUnique, pRecvMsg->szName
			);
		sDBLogMsg_QueryString LogMsg("Guild Name Change", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildSetMark()
{
	// 받은 패킷
	sDBRequest_Guild_SetMark *pRecvMsg = (sDBRequest_Guild_SetMark *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_SetMark sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_SETMARK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siBeforeGuildMark;

	// DB Query
	BEGIN_SP( "rsp_GuildSetMark" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildMark, sizeof(pRecvMsg->siGuildMark) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &siBeforeGuildMark );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clinfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, GuildMark = %d, BeforeMark = %d"
			, pRecvMsg->siGuildUnique, pRecvMsg->siGuildMark, siBeforeGuildMark
			);
		sDBLogMsg_QueryString LogMsg("Guild Name Change", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildChangeMaster()
{
	// 받은 패킷
	sDBRequest_Guild_ChangeMaster *pRecvMsg = (sDBRequest_Guild_ChangeMaster *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_ChangeMaster sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CHANGEMASTER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildMasterChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siNewPersonID, sizeof(pRecvMsg->siNewPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siNewPersonID );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, GuildUnique = %d"
			, pRecvMsg->siGuildUnique
			);
		sDBLogMsg_QueryString LogMsg("Guild Master Change", pRecvMsg->siNewPersonID, pRecvMsg->siPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildWarAddReserve()
{
	// 받은 패킷
	sDBRequest_GuildWar_AddReserve *pRecvMsg = (sDBRequest_GuildWar_AddReserve *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GuildWar_AddReserve sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILDWAR_ADDRESERVE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// [추가 : 황진성 2007. 11. 7 로그에 이용. 길드전 예약]
	sendMsg.siFieldIndex		= pRecvMsg->siFieldIndex;
	sendMsg.siRoomIndex			= pRecvMsg->siRoomIndex;
	sendMsg.siHomeGuildUnique	= pRecvMsg->siHomeGuildUnique;
	sendMsg.siAwayGUildUnique	= pRecvMsg->siAwayGUildUnique;
	sendMsg.siPersonID			= pRecvMsg->siPersonID;

	// DB Query
	BEGIN_SP( "rsp_GuildWarAddReserve" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFieldIndex, sizeof(pRecvMsg->siFieldIndex));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRoomIndex, sizeof(pRecvMsg->siRoomIndex));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHomeGuildUnique, sizeof(pRecvMsg->siHomeGuildUnique));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siAwayGUildUnique, sizeof(pRecvMsg->siAwayGUildUnique));

	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "FieldIndex = %d, RoomIndex = %d, HomeGuild = %d, AwayGuild = %d"
			, pRecvMsg->siFieldIndex, pRecvMsg->siRoomIndex, pRecvMsg->siHomeGuildUnique, pRecvMsg->siAwayGUildUnique
			);
		sDBLogMsg_QueryString LogMsg("GuildWar Add Reserve", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildWarReserveList()
{
	// 받은 패킷
	sDBRequest_GuildWar_ReserveList *pRecvMsg = (sDBRequest_GuildWar_ReserveList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GuildWar_ReserveList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILDWAR_RESERVELIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildWarReserveList" )
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siFieldIndex, sizeof(pRecvMsg->siFieldIndex));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siFieldIndex);
	END_FETCHDATA()

	SI32 siRoomNum = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siRoomNum >= GUILDWAR_ROOMNUM) break;
			GetGuildWarReserveUnit( &sendMsg.clUnit[siRoomNum++] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GuildDungeonApply()
{
	// 받은 패킷
	sDBRequest_GuildDungeon_Apply *pRecvMsg = (sDBRequest_GuildDungeon_Apply *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GuildDungeon_Apply sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILDDUNGEON_APPLY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildDungeonApply" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMapIndex, sizeof(pRecvMsg->siMapIndex));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siGuildUnique );
		m_odbc.GetData( &sendMsg.siMapIndex );
	END_FETCHDATA()

	// 길드정보 얻기
	GetGuildInfo(&sendMsg.clGuildInfo);
	// 리퀘스트때 받은 타입을 넘겨 준다.// 이타입은 그냥 해당 메세지의 타입을 결정
	sendMsg.siType = pRecvMsg->siType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GuildUnique = %d, MapIndex = %d"
			, pRecvMsg->siGuildUnique, pRecvMsg->siMapIndex
			);
		sDBLogMsg_QueryString LogMsg("GuildDungeon Apply", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildDungeonSaveStatus()
{
	// 받은 패킷
	sDBRequest_GuildDungeon_SaveStatus *pRecvMsg = (sDBRequest_GuildDungeon_SaveStatus *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GuildDungeon_SaveStatus sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILDDUNGEON_SAVESTATUS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildDungeonStatusSave" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMapIndex, sizeof(pRecvMsg->siMapIndex));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siExp, sizeof(pRecvMsg->siExp));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MapIndex = %d, Exp = %d, Money = %I64d"
			, pRecvMsg->siMapIndex, pRecvMsg->siExp, pRecvMsg->siMoney
			);
		sDBLogMsg_QueryString LogMsg("GuildDungeon SaveStatus", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GuildDungeonLoadStatus()
{
	// 받은 패킷
	sDBRequest_GuildDungeon_LoadStatus *pRecvMsg = (sDBRequest_GuildDungeon_LoadStatus *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GuildDungeon_LoadStatus sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILDDUNGEON_LOADSTATUS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_GuildDungeonStatusLoad" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMapIndex, sizeof(pRecvMsg->siMapIndex));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siMapIndex );
		m_odbc.GetData( &sendMsg.siGuildUnique );
		m_odbc.GetData( &sendMsg.siExp );
		m_odbc.GetData( &sendMsg.siMoney );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GuildDungeonTransferOwnership()
{
	// 받은 패킷
	sDBRequest_GuildDungeon_TransferOwnership *pRecvMsg = (sDBRequest_GuildDungeon_TransferOwnership *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GuildDungeon_TransferOwnership sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILDDUNGEON_TRANSFEROWNERSHIP;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siGuildDungeon = 0;

	// DB Query
	BEGIN_SP( "rsp_GuildDungeonTransferOwnership" )
		m_odbc.SetParam( SQL_REAL, &pRecvMsg->fGPRatio, sizeof(pRecvMsg->fGPRatio));
		m_odbc.SetParam( SQL_REAL, &pRecvMsg->fMoneyRatio, sizeof(pRecvMsg->fMoneyRatio));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	SI32 siPersonID = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siGuildDungeon >= MAX_GUILDDUNGEON) break;
			GetGuildDungeonChangeGPMoney( &sendMsg.clChangedGuildInfo[siGuildDungeon] );
			GetGuildDungeonNewOwnership( &sendMsg.clNewOwnerInfo[siGuildDungeon] );
			m_odbc.GetData( &siPersonID );
			// 로그 작성
			if(sendMsg.siResult == 1)
			{
				char log[512] = "";
				sprintf(log, "DungeonIndex = %d, GPRatio = %f, MoneyRatio = %f, NewGuildUnique = %d, OldGuildUnique = %d, AddExp = %d, Exp = %d, AddMoney = %d, LeftBankMoney = %d"
					, siGuildDungeon, pRecvMsg->fGPRatio, pRecvMsg->fMoneyRatio, sendMsg.clNewOwnerInfo[siGuildDungeon].siGuildUnique, sendMsg.clChangedGuildInfo[siGuildDungeon].siGuildUnique
					, sendMsg.clChangedGuildInfo[siGuildDungeon].siAddExp, sendMsg.clChangedGuildInfo[siGuildDungeon].siExp
					, sendMsg.clChangedGuildInfo[siGuildDungeon].siAddMoney, sendMsg.clChangedGuildInfo[siGuildDungeon].siMoney
					);
				sDBLogMsg_QueryString LogMsg("GuildDungeon Transfer Ownership", siPersonID, NULL, NULL, log);
				SendLogMsg((sPacketHeader *)&LogMsg);
			}
			siGuildDungeon++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GuildCheckDestroyMonth()
{
	// 받은 패킷
	sDBRequest_Guild_CheckDestroy_Month *pRecvMsg = (sDBRequest_Guild_CheckDestroy_Month *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_CheckDestroy_Month sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GUILD_CHECKDESTROY_MONTH;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siResultGuildNum = 0;
	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->stLastConnectTime);

	// DB Query
	BEGIN_SP( "rsp_GuildCheckDestroyMonth" )
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResultGuildNum ); // 검색된 총 길드수
	END_FETCHDATA()

	if(siResultGuildNum > 0)
		sendMsg.siResult = 1;

	if(siResultGuildNum > MAX_GUILD_PER_DBLIST)		siResultGuildNum = MAX_GUILD_PER_DBLIST;

	// 길드정보를 받는다.
	for(SI32 i = 0; i < siResultGuildNum; i++)
	{
		GetGuildInfo(&sendMsg.clInfo[i]);
	}

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GuildNotCOnnectChangeMaster()
{
	// 받은 패킷
	sDBRequest_Guild_NotConnect_ChangeMaster *pRecvMsg = (sDBRequest_Guild_NotConnect_ChangeMaster *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_NotConnect_ChangeMaster sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_GUILD_NOTCONNECT_CHANGEMASTER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID			= pRecvMsg->siCharID;
	sendMsg.siPersonID			= pRecvMsg->siPersonID;

	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->stLastConnectTime);

	// DB Query
	BEGIN_SP( "rsp_GuildNotConnectChangeMaster" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siNewPersonID );
		m_odbc.GetData( sendMsg.szNewPersonName, sizeof(sendMsg.szNewPersonName) );
	END_FETCHDATA()


	// 길드정보를 받는다.
	GetGuildInfo(&sendMsg.clInfo);

	// send
	SendPacket( (BYTE *)&sendMsg );
}

//길드맵 전쟁을 신청한 길드의 리스트를 저장하거나 삭제 한다.
/*

*/
void DBGameProtocol::SaveGuildHuntMapWarList()
{
	// 받은 패킷
	sDBRequest_GuildHuntMapWar_SaveRegList *pRecvMsg = (sDBRequest_GuildHuntMapWar_SaveRegList *)m_pPacket;

	// 보낼 패킷	
	sDBResponse_GuildHuntMapWar_SaveRegList sendMsg;
	
	sendMsg.packetHeader.usCmd	=	DBMSG_RESPONSE_GUILDHUNTMAPWAR_REGLIST;
	sendMsg.packetHeader.usSize	=	sizeof( sendMsg )		;
	sendMsg.siGuildUnique		=	pRecvMsg->siGuildUnique	;
	sendMsg.siType				=	pRecvMsg->siType		;

	// DB Query
	BEGIN_SP( "rsp_GuildHuntMapWar_SaveReg" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
	END_SP()
	EXEC_SQL()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()
	// send
	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::GetGuildHuntMapWarList()//길드맵리스트를 받는다.
{
	// 받을패킷
	sDBRequest_GuildHuntMapWar_SaveRegListGet *pRecvMsg = (sDBRequest_GuildHuntMapWar_SaveRegListGet *)m_pPacket	;
	//보낼패킷
	sDBResponse_GuildHuntMapWar_SaveRegListGet sendMsg;
	sendMsg.packetHeader.usCmd	=	DBMSG_RESPONSE_GUILDHUNTMAPWAR_GET_REGLIST;
	sendMsg.packetHeader.usSize	=	sizeof( sendMsg )		;
	// DB Query
	BEGIN_SP( "rsp_GuildHuntMapWar_GetRegList" )
	END_SP()
	EXEC_SQL()
	SI32 siLatestIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
		if ( MAX_GUILDHUNTWAR_REQUEST_NUM <= siLatestIndex )
			continue;
		GetGuildMapWar_RegList( &sendMsg.m_GuildList[siLatestIndex++] );
		END_FETCHDATA()
	END_NEXTRECORDSET()
	// send
	SendPacket( (BYTE*)&sendMsg );
}
void DBGameProtocol::GetGuildHuntMapRank()//BGP순위
{
	// 받을패킷
	sDBRequest_GuildHuntMapWar_Rank_Get *pRecvMsg = (sDBRequest_GuildHuntMapWar_Rank_Get *)m_pPacket	;
	//보낼패킷
	sDBResponse_GuildHuntMapWar_Rank_Get sendMsg;
	sendMsg.packetHeader.usCmd	=	DBMSG_RESPONSE_GUILDHUNTMAPWAR_RANK_GET;
	sendMsg.packetHeader.usSize	=	sizeof( sendMsg )		;
	sendMsg.siPersonID			=	pRecvMsg->siPersonID	;
	// DB8 Query
	BEGIN_SP( "rsp_GuildHuntMapWar_Rank_Get" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMapIndex, sizeof(pRecvMsg->siMapIndex) );
	END_SP()
		EXEC_SQL()

		SI32 siLatestIndex = 0;

	BEGIN_FETCHDATA()
		if ( MAX_GUILDHUNTWAR_RANK_NUM <= siLatestIndex )
			continue;
	GetGuildMapWar_GetRank( &sendMsg.m_GuildList[siLatestIndex++] );
	END_FETCHDATA()
	// send
	SendPacket( (BYTE*)&sendMsg );
}
void DBGameProtocol::EndGuildHuntMapWar()
{
	// 받은 패킷
	sDBRequest_GuildHuntMapWar_Result *pRecvMsg = (sDBRequest_GuildHuntMapWar_Result *)m_pPacket;

	// 보낼 패킷	
	sDBResponse_GuildHuntMapWar_Result sendMsg;

	sendMsg.packetHeader.usCmd	=	DBMSG_RESPONSE_GUILDHUNTMAPWAR_RESULT;
	sendMsg.packetHeader.usSize	=	sizeof( sendMsg )		;

	// DB Query
	BEGIN_SP( "rsp_GuildHuntMapWar_End" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siWinLoseFlag, sizeof(pRecvMsg->siWinLoseFlag) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siBGP, sizeof(pRecvMsg->siBGP) );
	END_SP()
		EXEC_SQL()
		BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()
	// 날리기 전에 길드 유니크랑 길드 BGP를 넘겨 준다.
	sendMsg.siGuildunique	=	pRecvMsg->siGuildUnique	;
	sendMsg.siBGP			=	pRecvMsg->siBGP	;
	// send
	SendPacket( (BYTE *)&sendMsg );
}