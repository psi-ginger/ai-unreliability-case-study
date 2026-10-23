#include "DBGameProtocol.h"
#include "DBMsg-PVP_League.h"


void DBGameProtocol::PVP_LeagueInfoGet()
{
	// 받은 패킷
	sDBRequest_PVP_League_InfoGet* pRecvMsg = (sDBRequest_PVP_League_InfoGet*)m_pPacket;

	// 보낼 패킷
	sDBResponse_PVP_League_InfoGet sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_PVP_LEAGUE_INFOGET;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);

	TIMESTAMP_STRUCT stTS;
	ZeroMemory(&stTS, sizeof(stTS));

	// DB Query
	BEGIN_SP( "rsp_PVPLeagueInfoGet" );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	SI32 siIndex=0;
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siWin );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siLose );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siDraw );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siStraight_Win );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siStraight_Lose );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siStraight_Draw );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siStraight_Win_Grade );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siStraight_Lose_Grade );
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siGrade );
		m_odbc.GetData( &stTS, sizeof(stTS) );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		if ( PVP_LEAGUE_MAX_LATEST_MATCH_PERSON <= siIndex )
			continue;
		m_odbc.GetData( &sendMsg.m_clPVP_LeaugeInfo.m_siLatestPersonID[siIndex++] );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.m_clPVP_LeaugeInfo.stLastPVPTime, &stTS);

	// send
	SendPacket( (BYTE*)&sendMsg );
}

void DBGameProtocol::PVP_LeagueUIInfoGet()
{
	// 받은 패킷
	sDBRequest_PVP_League_UI_InfoGet* pRecvMsg = (sDBRequest_PVP_League_UI_InfoGet*)m_pPacket;

	// 보낼 패킷
	sDBResponse_PVP_League_UI_InfoGet sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_PVP_LEAGUE_UI_INFOGET;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;

	// DB Query
	BEGIN_SP( "rsp_PVPLeagueUIInfoGet" );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,	sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_siWin );
		m_odbc.GetData( &sendMsg.m_siDraw );
		m_odbc.GetData( &sendMsg.m_siLose );
		m_odbc.GetData( &sendMsg.m_siGroupNum );
	END_FETCHDATA()

	SI32 siLatestIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if ( Const_PVP_League_Latest_Max <= siLatestIndex )
				continue;
			GetPVP_League_OtherUserInfo( &sendMsg.m_clLatest[siLatestIndex++] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SI32 siTopIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if ( Const_PVP_League_Top_Max <= siTopIndex )
				continue;
			GetPVP_League_OtherUserInfo( &sendMsg.m_clTop[siTopIndex++] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE*)&sendMsg );

}

void DBGameProtocol::PVP_LeagueBattleEnd()
{
	// 받은 패킷
	sDBRequest_PVP_League_Battle_End* pRecvMsg = (sDBRequest_PVP_League_Battle_End*)m_pPacket;

	// 보낼 패킷
	sDBResponse_PVP_League_Battle_End sendMsg;
	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_PVP_LEAGUE_BATTLE_END;
	sendMsg.packetHeader.usSize	= sizeof(sendMsg);
	//sendMsg.m_siCharID				= pRecvMsg->m_siCharID;

	TIMESTAMP_STRUCT stTS;
	ZeroMemory(&stTS, sizeof(stTS));

	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->stLAstPVPTime);

	// DB Query
	BEGIN_SP( "rsp_PVPLeagueBattleEnd" );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siMyPersonID,	sizeof(pRecvMsg->siMyPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siEnemyPersonID,	sizeof(pRecvMsg->siEnemyPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siWinOrLose,	sizeof(pRecvMsg->siWinOrLose) );
		m_odbc.SetParam( SQL_TIMESTAMP,	&stTS,	sizeof(stTS) );
	END_SP()

	EXEC_SQL()

	ZeroMemory(&stTS, sizeof(stTS));

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siMyPersonID );
		m_odbc.GetData( &sendMsg.siEnemyPersonID );
		m_odbc.GetData( &sendMsg.siWinOrLose );
		m_odbc.GetData( &stTS, sizeof(stTS) );
	END_FETCHDATA()

	Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.stLAstPVPTime, &stTS);

	// send
	SendPacket( (BYTE*)&sendMsg );
}

void DBGameProtocol::PVP_LeagueBattleChangeGrade()
{
	// 받은 패킷
	sDBRequest_PVP_League_Battle_ChangeGrade* pRecvMsg = (sDBRequest_PVP_League_Battle_ChangeGrade*)m_pPacket;

	// 보낼 패킷
	sDBResponse_PVP_League_Battle_ChangeGrade sendMsg;
	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_PVP_LEAGUE_BATTLE_CHANGEGRADE;
	sendMsg.packetHeader.usSize	= sizeof(sendMsg);
	//sendMsg.m_siCharID				= pRecvMsg->m_siCharID;
	TIMESTAMP_STRUCT stTS;
	ZeroMemory(&stTS, sizeof(stTS));

	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->stLAstPVPTime);

	// DB Query
	BEGIN_SP( "rsp_PVPLeagueBattleChangeGrade" );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siMyPersonID,	sizeof(pRecvMsg->siMyPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siGrade,	sizeof(pRecvMsg->siGrade) );
		m_odbc.SetParam( SQL_TIMESTAMP,	&stTS,	sizeof(stTS) );
	END_SP()

	EXEC_SQL()
		ZeroMemory(&stTS, sizeof(stTS));

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siMyPersonID );
		m_odbc.GetData( &sendMsg.siGrade );
		m_odbc.GetData( &stTS, sizeof(stTS) );
	END_FETCHDATA()
	Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.stLAstPVPTime, &stTS);

	// send
	SendPacket( (BYTE*)&sendMsg );
}
