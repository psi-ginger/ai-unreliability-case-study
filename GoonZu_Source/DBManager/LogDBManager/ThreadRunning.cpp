//=============================================================================
//
//	ThreadRunning.cpp
//
//	Copyright 2007 HQGame
//
//=============================================================================
#include "LogDB.h"
#include "DBLogProtocol.h"

#define BEGIN_SP(a)				sprintf( pThreadParameter->m_szSPName, "%s.dbo.%s", g_dbname, a ); pThreadParameter->m_odbc.BeginSP( pThreadParameter->m_szSPName );
#define END_SP()				pThreadParameter->m_odbc.EndSP();
#define EXEC_SQL()				if(!pThreadParameter->m_odbc.ExecSQL()) return;
#define BEGIN_FETCHDATA()		while(pThreadParameter->m_odbc.FetchData() > 0) {
#define END_FETCHDATA()			}
#define BEGIN_NEXTRECORDSET()	if(pThreadParameter->m_odbc.NextRecordSet() == 1) {
#define END_NEXTRECORDSET()		}

extern DBLogProtocol			g_dap;

extern TCHAR					szModifyDate3[64];// = "2008-07-17";		// 리얼 패치일
extern TCHAR					szModifyDate4[64];// = "2009-02-03";		// 리얼 패치일

void DoMsg_DBMSG_REQUEST_LOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter);
void DoMsg_DBMSG_REQUEST_NEWLOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter);
void DoMsg_DBMSG_REQUEST_NEWLOG3(sPacketHeader* pPacket, NThreadParameter* pThreadParameter);
void DoMsg_DBMSG_REQUEST_NEWLOG4(sPacketHeader* pPacket, NThreadParameter* pThreadParameter);
void DoMsg_DBMSG_REQUEST_TOOLSLOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter);

void ThreadRunning(LPVOID lpParam)
{
	NThreadParameter *pThreadParameter = (NThreadParameter*)lpParam;
	if(pThreadParameter == NULL)		return;

	while( pThreadParameter->m_bThreadRunning )
	{
		Sleep( 10 );

		if(pThreadParameter->m_pClient != NULL)
		{
			if(pThreadParameter->m_pClient->m_bRequestLog)
				DoMsg_DBMSG_REQUEST_LOG( (sPacketHeader*)&pThreadParameter->m_pClient->m_RequestLog, pThreadParameter );

			if(pThreadParameter->m_pClient->m_bRequestNewLog)
			{
				if( strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szStartDate, szModifyDate3, strlen(szModifyDate3)) <= 0 ||
					strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szEndDate,   szModifyDate3, strlen(szModifyDate3)) <= 0 )
				{
					DoMsg_DBMSG_REQUEST_NEWLOG( (sPacketHeader*)&pThreadParameter->m_pClient->m_RequestNewLog, pThreadParameter );
				}

				if( ( strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szStartDate, szModifyDate3, strlen(szModifyDate3)) >= 0 &&
					  strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szStartDate, szModifyDate4, strlen(szModifyDate4)) <= 0 ) ||
					( strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szEndDate,   szModifyDate3, strlen(szModifyDate3)) >= 0 && 
					  strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szEndDate,   szModifyDate4, strlen(szModifyDate4)) <= 0 ) )
				{
					DoMsg_DBMSG_REQUEST_NEWLOG3( (sPacketHeader*)&pThreadParameter->m_pClient->m_RequestNewLog, pThreadParameter );
				}

				if( strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szStartDate, szModifyDate4, strlen(szModifyDate4)) >= 0 ||
					strncmp(pThreadParameter->m_pClient->m_RequestNewLog.szEndDate,   szModifyDate4, strlen(szModifyDate4)) >= 0 )
				{
					DoMsg_DBMSG_REQUEST_NEWLOG4( (sPacketHeader*)&pThreadParameter->m_pClient->m_RequestNewLog, pThreadParameter );
				}

				ITZSession*	pSession = pThreadParameter->m_pClient->GetSession();
				if(pSession == NULL)			return;

				if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
				{
					// 보낼 패킷
					sDBResponse_NewLog sendMsg;

					sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
					sendMsg.packetHeader.usSize = sizeof( sendMsg );

					sendMsg.usCharID		= pThreadParameter->m_pClient->m_RequestNewLog.usCharID;

					sendMsg.siResult = -100;
					pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
				}

				pThreadParameter->m_pClient->m_bRequestNewLog = FALSE;
				ZeroMemory(&pThreadParameter->m_pClient->m_RequestNewLog, sizeof(pThreadParameter->m_pClient->m_RequestNewLog));
			}

			else if( TRUE == pThreadParameter->m_pClient->m_bRequestToolsLog )
				DoMsg_DBMSG_REQUEST_TOOLSLOG( (sPacketHeader*)&pThreadParameter->m_pClient->m_RequestToolsLog, pThreadParameter );
		}
	}
}

void DoMsg_DBMSG_REQUEST_LOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter)
{
	cLogClient* pClient = pThreadParameter->m_pClient;
	if(pClient == NULL)			return;

	ITZSession*	pSession = pClient->GetSession();
	if(pSession == NULL)			return;

	printf( "DBMSG_REQUEST_LOG [%d]\n", pClient->GetCharID() );

	// 받은 패킷
	sDBRequest_Log	*pRecvMsg = (sDBRequest_Log *)pPacket;

	// 보낼 패킷
	sDBResponse_Log sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LOG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.usCharID		= pRecvMsg->usCharID;

	//rsp_PersonalLog
	//	@ServerName	varchar	16
	//	@PersonName	varchar	20
	//	@DstPersonName	varchar	20
	//	@ItemUnique	varchar	10
	//	@StartDate	varchar	24
	//	@EndDate	varchar	24
	//	@Command	varchar	50
	//	@Log	varchar	512
	//	@Cartegory	varchar	512

//	printf("ID:%s, PW:%s, SVR:%s, SD:%s, ED:%s\n", pRecvMsg->szLoginID, pRecvMsg->szLoginPW,
//		pRecvMsg->szServerName, pRecvMsg->szStartDate, pRecvMsg->szEndDate);

	// DB Query
	BEGIN_SP( "rsp_PersonalLog" )
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szLoginID,			sizeof(pRecvMsg->szLoginID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szLoginPW,			sizeof(pRecvMsg->szLoginPW) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szServerName,		sizeof(pRecvMsg->szServerName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szPersonName,		sizeof(pRecvMsg->szPersonName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szDstPersonName,		sizeof(pRecvMsg->szDstPersonName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szItemUnique,		sizeof(pRecvMsg->szItemUnique) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szStartDate,			sizeof(pRecvMsg->szStartDate) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szEndDate,			sizeof(pRecvMsg->szEndDate) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szCommand,			sizeof(pRecvMsg->szCommand) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szLogText,			sizeof(pRecvMsg->szLogText) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szCategory,			sizeof(pRecvMsg->szCategory) );
	END_SP()

	//EXEC_SQL()
	if(!pThreadParameter->m_odbc.ExecSQL())
	{
		//printf( "Total Log Sended [%d] - %d\n", 0);
		printf( "Query Execute Error\n");

		// 패킷 내용 삭제
		ZeroMemory(&sendMsg, sizeof(sendMsg));

		// 패킷 기본값 설정
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LOG;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.usCharID = pRecvMsg->usCharID;

		sendMsg.siResult = -1;
		pClient->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

		pThreadParameter->m_pClient->m_bRequestLog = FALSE;
		ZeroMemory(&pThreadParameter->m_pClient->m_RequestLog, sizeof(pThreadParameter->m_pClient->m_RequestLog));

		return;
	}

//	printf( "Query Execute Success\n" );

	BOOL bSend = FALSE;
	SI32 siCount = 0;
	SI32 siTotalCount = 0;
	SI32 siResult = 0;
	BEGIN_FETCHDATA()
		pThreadParameter->m_odbc.GetData( &siResult,		sizeof(siResult) );
	END_FETCHDATA()

	switch(siResult)
	{
	case 1:				break;
	case -1:	// 아이디 틀림
	case -2:	// 패스워드 틀림
		{
			sendMsg.siResult = siResult;
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );

			printf( "Login Failed [%d] [%s] - %d\n", pClient->GetCharID(), 
							pThreadParameter->m_pClient->m_RequestLog.szLoginID, 
							siResult );

			pThreadParameter->m_pClient->m_bRequestLog = FALSE;
			ZeroMemory(&pThreadParameter->m_pClient->m_RequestLog, sizeof(pThreadParameter->m_pClient->m_RequestLog));

			return;
		}
		break;
	}

//	printf( "Login Success\n" );

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		bSend = FALSE;
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].szDate, sizeof(sendMsg.stLogData[siCount].szDate) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].szCommand, sizeof(sendMsg.stLogData[siCount].szCommand) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].szPersonName, sizeof(sendMsg.stLogData[siCount].szPersonName) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].szDstPersonName, sizeof(sendMsg.stLogData[siCount].szDstPersonName) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].szItemUnique, sizeof(sendMsg.stLogData[siCount].szItemUnique) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].szLogTxt, sizeof(sendMsg.stLogData[siCount].szLogTxt) );

		strncpy( sendMsg.stLogData[siCount].szServerName, pRecvMsg->szServerName, sizeof(sendMsg.stLogData[siCount].szServerName));

		siCount++;
//		printf( "Count : %d\n" , siCount);
		siTotalCount++;
		if(MAX_STLOGDATA_PER_PACKET <= siCount)
		{
			// 다 찾으므로 일단 클라이언트에 전송
			sendMsg.siResult = 1;
			if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
                if(pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg ) == FALSE)		break;
			}
			else
			{
				// 연결이 끊어졌다면 함수를 종료한다.
				printf( "Connection Closed\n" );
				break;
			}

			//네트워크 부하를 줄이기 위해 잠깐 휴식
			Sleep(100);

			// 패킷 내용 삭제
			ZeroMemory(&sendMsg, sizeof(sendMsg));

			// 패킷 기본값 설정
			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LOG;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.usCharID = pRecvMsg->usCharID;

			bSend = TRUE;
			siCount = 0;
		}
	END_FETCHDATA()
	END_NEXTRECORDSET()

	printf( "Total Log Sended [%d] - %d\n", pClient->GetCharID(), siTotalCount );

	if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
	{
		// 마지막은 siResult = -100 이다.
		if(siCount <= MAX_STLOGDATA_PER_PACKET)
			sendMsg.siResult = -100;
		else
			sendMsg.siResult = 1;		
		pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
	}

	pThreadParameter->m_pClient->m_bRequestLog = FALSE;
	ZeroMemory(&pThreadParameter->m_pClient->m_RequestLog, sizeof(pThreadParameter->m_pClient->m_RequestLog));
}

void DoMsg_DBMSG_REQUEST_NEWLOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter)
{
	cLogClient* pClient = pThreadParameter->m_pClient;
	if(pClient == NULL)			return;

	ITZSession*	pSession = pClient->GetSession();
	if(pSession == NULL)			return;

	printf( "DBMSG_REQUEST_NEWLOG [%d]\n", pClient->GetCharID() );

	// 받은 패킷
	sDBRequest_NewLog	*pRecvMsg = (sDBRequest_NewLog *)pPacket;

	// 보낼 패킷
	sDBResponse_NewLog sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.usCharID		= pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonalNewLog" )
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szLoginID,			sizeof(pRecvMsg->szLoginID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szLoginPW,			sizeof(pRecvMsg->szLoginPW) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szServerName,			sizeof(pRecvMsg->szServerName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName,			sizeof(pRecvMsg->szPersonName) );
		pThreadParameter->m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique,		sizeof(pRecvMsg->siItemUnique) );
		pThreadParameter->m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemSID,			sizeof(pRecvMsg->siItemSID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szStartDate,			sizeof(pRecvMsg->szStartDate) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szEndDate,			sizeof(pRecvMsg->szEndDate) );
		pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiCommandType,		sizeof(pRecvMsg->uiCommandType) );
		pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiCommandIndex,		sizeof(pRecvMsg->uiCommandIndex) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szDetailOption,		sizeof(pRecvMsg->szDetailOption) );
	END_SP()

	//EXEC_SQL()
	if(!pThreadParameter->m_odbc.ExecSQL())
	{
		//printf( "Total Log Sended [%d] - %d\n", 0);
		printf( "Query Execute Error\n");

		// 패킷 내용 삭제
		ZeroMemory(&sendMsg, sizeof(sendMsg));

		// 패킷 기본값 설정
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.usCharID = pRecvMsg->usCharID;

		sendMsg.siResult = -1;
		pClient->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

//		pThreadParameter->m_pClient->m_bRequestNewLog = FALSE;
//		ZeroMemory(&pThreadParameter->m_pClient->m_RequestNewLog, sizeof(pThreadParameter->m_pClient->m_RequestNewLog));

		return;
	}

//	printf( "Query Execute Success\n" );

	BOOL bSend = FALSE;
	SI32 siCount = 0;
	SI32 siTotalCount = 0;
	SI32 siResult = 0;
	TIMESTAMP_STRUCT stCDate;

	BEGIN_FETCHDATA()
		pThreadParameter->m_odbc.GetData( &siResult,		sizeof(siResult) );
	END_FETCHDATA()

	switch(siResult)
	{
	case 1:				break;
	case -1:	// 아이디 틀림
	case -2:	// 패스워드 틀림
		{
			sendMsg.siResult = siResult;
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );

			printf( "Login Failed [%d] [%s] - %d\n", pClient->GetCharID(), 
							pThreadParameter->m_pClient->m_RequestNewLog.szLoginID, 
							siResult );

//			pThreadParameter->m_pClient->m_bRequestNewLog = FALSE;
//			ZeroMemory(&pThreadParameter->m_pClient->m_RequestNewLog, sizeof(pThreadParameter->m_pClient->m_RequestNewLog));

			return;
		}
		break;
	}

//	printf( "Login Success\n" );

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		bSend = FALSE;
		pThreadParameter->m_odbc.GetData( &stCDate, sizeof(stCDate) );
		{
			sendMsg.stLogData[siCount].m_stTime.wYear = stCDate.year;
			sendMsg.stLogData[siCount].m_stTime.wMonth = stCDate.month;
			sendMsg.stLogData[siCount].m_stTime.wDay = stCDate.day;
			sendMsg.stLogData[siCount].m_stTime.wHour = stCDate.hour;
			sendMsg.stLogData[siCount].m_stTime.wMinute = stCDate.minute;
			sendMsg.stLogData[siCount].m_stTime.wSecond = stCDate.second;
			sendMsg.stLogData[siCount].m_stTime.wMilliseconds = (WORD)stCDate.fraction/1000000;
		}

		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szIP, sizeof(sendMsg.stLogData[siCount].m_szIP) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siCommandType, sizeof(sendMsg.stLogData[siCount].m_siCommandType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siCommandIndex, sizeof(sendMsg.stLogData[siCount].m_siCommandIndex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_uiReason, sizeof(sendMsg.stLogData[siCount].m_uiReason) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siIDNum, sizeof(sendMsg.stLogData[siCount].m_siIDNum) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szPersonName, sizeof(sendMsg.stLogData[siCount].m_szPersonName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siPersonID, sizeof(sendMsg.stLogData[siCount].m_siPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siLevel, sizeof(sendMsg.stLogData[siCount].m_siLevel) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siExp, sizeof(sendMsg.stLogData[siCount].m_siExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMoney, sizeof(sendMsg.stLogData[siCount].m_siMoney) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMoneySum, sizeof(sendMsg.stLogData[siCount].m_siMoneySum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siUnique, sizeof(sendMsg.stLogData[siCount].m_clItem.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siItemNum, sizeof(sendMsg.stLogData[siCount].m_clItem.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cData8, sizeof(sendMsg.stLogData[siCount].m_clItem.cData8) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateYear, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateYear) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateMonth, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateMonth) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateDay, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateDay) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateUseDay, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateUseDay) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siMakerPersonID, sizeof(sendMsg.stLogData[siCount].m_clItem.siMakerPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.Element.siType, sizeof(sendMsg.stLogData[siCount].m_clItem.Element.siType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.Element.siPower, sizeof(sendMsg.stLogData[siCount].m_clItem.Element.siPower) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siSID, sizeof(sendMsg.stLogData[siCount].m_clItem.siSID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siItemSum, sizeof(sendMsg.stLogData[siCount].m_siItemSum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siUnique, sizeof(sendMsg.stLogData[siCount].m_clSummon.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siKind, sizeof(sendMsg.stLogData[siCount].m_clSummon.siKind) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_clSummon.szName, sizeof(sendMsg.stLogData[siCount].m_clSummon.szName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siStatus, sizeof(sendMsg.stLogData[siCount].m_clSummon.siStatus) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siAttackType, sizeof(sendMsg.stLogData[siCount].m_clSummon.siAttackType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siIdentity, sizeof(sendMsg.stLogData[siCount].m_clSummon.siIdentity) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siStr, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siStr) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siDex, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siDex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siVit, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siVit) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siMag, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siMag) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siLuk, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siLuk) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siExp, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siLevel, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siLevel) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siBonus, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siBonus) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.Life, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.Life) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.Mana, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.Mana) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[0], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[0]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[0], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[0]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[1], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[1]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[1], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[1]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siTakeInMinute, sizeof(sendMsg.stLogData[siCount].m_clSummon.siTakeInMinute) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMonKind, sizeof(sendMsg.stLogData[siCount].m_siMonKind) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szTargetPersonName, sizeof(sendMsg.stLogData[siCount].m_szTargetPersonName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siTargetPersonID, sizeof(sendMsg.stLogData[siCount].m_siTargetPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siVillageUnique, sizeof(sendMsg.stLogData[siCount].m_siVillageUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siNpcKind, sizeof(sendMsg.stLogData[siCount].m_siNpcKind) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siHungry, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siHungry) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg1, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg1) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg2, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg2) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg3, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg3) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg4, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg4) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg1Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg1Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg2Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg2Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg3Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg3Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg4Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg4Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiHealthPoint, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiHealthPoint) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siRPPoint, sizeof(sendMsg.stLogData[siCount].m_siRPPoint) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siFameExp, sizeof(sendMsg.stLogData[siCount].m_siFameExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam1, sizeof(sendMsg.stLogData[siCount].m_siParam1) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam2, sizeof(sendMsg.stLogData[siCount].m_siParam2) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam3, sizeof(sendMsg.stLogData[siCount].m_siParam3) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam4, sizeof(sendMsg.stLogData[siCount].m_siParam4) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam5, sizeof(sendMsg.stLogData[siCount].m_siParam5) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szCharParam1, sizeof(sendMsg.stLogData[siCount].m_szCharParam1) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szCharParam2, sizeof(sendMsg.stLogData[siCount].m_szCharParam2) );

		siCount++;
//		printf( "Count : %d\n" , siCount);
		siTotalCount++;
		if(MAX_STLOGDATA_PER_PACKET <= siCount)
		{
			// 다 찾으므로 일단 클라이언트에 전송
			sendMsg.siResult = 1;
			if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
                if(pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg ) == FALSE)		break;
			}
			else
			{
				// 연결이 끊어졌다면 함수를 종료한다.
				printf( "Connection Closed\n" );
				break;
			}

			//네트워크 부하를 줄이기 위해 잠깐 휴식
			Sleep(100);

			// 패킷 내용 삭제
			ZeroMemory(&sendMsg, sizeof(sendMsg));

			// 패킷 기본값 설정
			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.usCharID = pRecvMsg->usCharID;

			bSend = TRUE;
			siCount = 0;
		}
	END_FETCHDATA()
	END_NEXTRECORDSET()

	printf( "Total NewLog Sended [%d] - %d\n", pClient->GetCharID(), siTotalCount );

	// 10개 미만의 데이터를 보낸다.
	if(siCount > 0 && pSession->GetState() == SESSION_STATE_ESTABLISHED)
	{
		sendMsg.siResult = 1;
		pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
	}
}

void DoMsg_DBMSG_REQUEST_NEWLOG3(sPacketHeader* pPacket, NThreadParameter* pThreadParameter)
{
	cLogClient* pClient = pThreadParameter->m_pClient;
	if(pClient == NULL)			return;

	ITZSession*	pSession = pClient->GetSession();
	if(pSession == NULL)			return;

	printf( "DBMSG_REQUEST_NEWLOG3 [%d]\n", pClient->GetCharID() );

	// 받은 패킷
	sDBRequest_NewLog	*pRecvMsg = (sDBRequest_NewLog *)pPacket;

	// 보낼 패킷
	sDBResponse_NewLog sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.usCharID		= pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonalLog3" )
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szLoginID,			sizeof(pRecvMsg->szLoginID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szLoginPW,			sizeof(pRecvMsg->szLoginPW) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szServerName,			sizeof(pRecvMsg->szServerName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName,			sizeof(pRecvMsg->szPersonName) );
		pThreadParameter->m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique,		sizeof(pRecvMsg->siItemUnique) );
		pThreadParameter->m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemSID,			sizeof(pRecvMsg->siItemSID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szStartDate,			sizeof(pRecvMsg->szStartDate) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szEndDate,			sizeof(pRecvMsg->szEndDate) );
		pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiCommandType,		sizeof(pRecvMsg->uiCommandType) );
		pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiCommandIndex,		sizeof(pRecvMsg->uiCommandIndex) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szDetailOption,		sizeof(pRecvMsg->szDetailOption) );
	END_SP()

	//EXEC_SQL()
	if(!pThreadParameter->m_odbc.ExecSQL())
	{
		//printf( "Total Log Sended [%d] - %d\n", 0);
		printf( "Query Execute Error\n");

		// 패킷 내용 삭제
		ZeroMemory(&sendMsg, sizeof(sendMsg));

		// 패킷 기본값 설정
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.usCharID = pRecvMsg->usCharID;

		sendMsg.siResult = -1;
		pClient->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

		return;
	}

//	printf( "Query Execute Success\n" );

	BOOL bSend = FALSE;
	SI32 siCount = 0;
	SI32 siTotalCount = 0;
	SI32 siResult = 0;
	TIMESTAMP_STRUCT stCDate;

	BEGIN_FETCHDATA()
		pThreadParameter->m_odbc.GetData( &siResult,		sizeof(siResult) );
	END_FETCHDATA()

	switch(siResult)
	{
	case 1:				break;
	case -1:	// 아이디 틀림
	case -2:	// 패스워드 틀림
		{
			sendMsg.siResult = siResult;
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );

			printf( "Login Failed [%d] [%s] - %d\n", pClient->GetCharID(), 
							pThreadParameter->m_pClient->m_RequestNewLog.szLoginID, 
							siResult );

//			pThreadParameter->m_pClient->m_bRequestNewLog = FALSE;
//			ZeroMemory(&pThreadParameter->m_pClient->m_RequestNewLog, sizeof(pThreadParameter->m_pClient->m_RequestNewLog));

			return;
		}
		break;
	}

//	printf( "Login Success\n" );

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		bSend = FALSE;
		pThreadParameter->m_odbc.GetData( &stCDate, sizeof(stCDate) );
		{
			sendMsg.stLogData[siCount].m_stTime.wYear = stCDate.year;
			sendMsg.stLogData[siCount].m_stTime.wMonth = stCDate.month;
			sendMsg.stLogData[siCount].m_stTime.wDay = stCDate.day;
			sendMsg.stLogData[siCount].m_stTime.wHour = stCDate.hour;
			sendMsg.stLogData[siCount].m_stTime.wMinute = stCDate.minute;
			sendMsg.stLogData[siCount].m_stTime.wSecond = stCDate.second;
			sendMsg.stLogData[siCount].m_stTime.wMilliseconds = (WORD)stCDate.fraction/1000000;
		}

		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szIP,											sizeof(sendMsg.stLogData[siCount].m_szIP) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siCommandType,									sizeof(sendMsg.stLogData[siCount].m_siCommandType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siCommandIndex,									sizeof(sendMsg.stLogData[siCount].m_siCommandIndex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_uiReason,										sizeof(sendMsg.stLogData[siCount].m_uiReason) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siIDNum,										sizeof(sendMsg.stLogData[siCount].m_siIDNum) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szPersonName,									sizeof(sendMsg.stLogData[siCount].m_szPersonName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siPersonID,										sizeof(sendMsg.stLogData[siCount].m_siPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siLevel,										sizeof(sendMsg.stLogData[siCount].m_siLevel) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siExp,											sizeof(sendMsg.stLogData[siCount].m_siExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMoney,										sizeof(sendMsg.stLogData[siCount].m_siMoney) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMoneySum,										sizeof(sendMsg.stLogData[siCount].m_siMoneySum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siUnique,								sizeof(sendMsg.stLogData[siCount].m_clItem.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siItemNum,								sizeof(sendMsg.stLogData[siCount].m_clItem.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cData8,									sizeof(sendMsg.stLogData[siCount].m_clItem.cData8) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateYear,								sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateYear) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateMonth, 							sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateMonth) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateDay,								sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateDay) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateUseDay,							sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateUseDay) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siMakerPersonID, 						sizeof(sendMsg.stLogData[siCount].m_clItem.siMakerPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.Element.siType,							sizeof(sendMsg.stLogData[siCount].m_clItem.Element.siType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.Element.siPower, 						sizeof(sendMsg.stLogData[siCount].m_clItem.Element.siPower) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siSID,									sizeof(sendMsg.stLogData[siCount].m_clItem.siSID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siPalette,								sizeof(sendMsg.stLogData[siCount].m_clItem.siPalette) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siUseCrystalNum, 						sizeof(sendMsg.stLogData[siCount].m_clItem.siUseCrystalNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siCrystalType,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siDurability,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siMaxDurability,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbility,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbilityNum,	sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siCrystalType,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siDurability,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siMaxDurability,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbility,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbilityNum,	sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siCrystalType,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siDurability,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siMaxDurability,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbility,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbilityNum,	sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siCrystalType,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siDurability,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siMaxDurability,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbility,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbilityNum,	sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siCrystalType,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siDurability,			sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siMaxDurability,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbility,		sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbilityNum,	sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siItemSum,										sizeof(sendMsg.stLogData[siCount].m_siItemSum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siUnique,								sizeof(sendMsg.stLogData[siCount].m_clSummon.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siKind,								sizeof(sendMsg.stLogData[siCount].m_clSummon.siKind) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_clSummon.szName,									sizeof(sendMsg.stLogData[siCount].m_clSummon.szName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siStatus,								sizeof(sendMsg.stLogData[siCount].m_clSummon.siStatus) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siAttackType,							sizeof(sendMsg.stLogData[siCount].m_clSummon.siAttackType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siIdentity,							sizeof(sendMsg.stLogData[siCount].m_clSummon.siIdentity) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siStr, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siStr) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siDex, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siDex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siVit, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siVit) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siMag, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siMag) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siLuk, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siLuk) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siExp, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siLevel, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siLevel) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siBonus, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siBonus) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.Life, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.Life) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.Mana, 							sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.Mana) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[0],					sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[0]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[0],					sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[0]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[1],					sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[1]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[1],					sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[1]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siTakeInMinute,						sizeof(sendMsg.stLogData[siCount].m_clSummon.siTakeInMinute) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMonKind,										sizeof(sendMsg.stLogData[siCount].m_siMonKind) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szTargetPersonName,								sizeof(sendMsg.stLogData[siCount].m_szTargetPersonName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siTargetPersonID,								sizeof(sendMsg.stLogData[siCount].m_siTargetPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siVillageUnique,								sizeof(sendMsg.stLogData[siCount].m_siVillageUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siNpcKind,										sizeof(sendMsg.stLogData[siCount].m_siNpcKind) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siHungry,							sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siHungry) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg1, 						sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg1) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg2, 						sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg2) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg3, 						sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg3) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg4, 						sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg4) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg1Disease, 					sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg1Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg2Disease, 					sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg2Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg3Disease, 					sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg3Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg4Disease, 					sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg4Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiHealthPoint, 					sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiHealthPoint) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siRPPoint, 										sizeof(sendMsg.stLogData[siCount].m_siRPPoint) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siFameExp, 										sizeof(sendMsg.stLogData[siCount].m_siFameExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam1, 										sizeof(sendMsg.stLogData[siCount].m_siParam1) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam2, 										sizeof(sendMsg.stLogData[siCount].m_siParam2) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam3, 										sizeof(sendMsg.stLogData[siCount].m_siParam3) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam4, 										sizeof(sendMsg.stLogData[siCount].m_siParam4) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam5, 										sizeof(sendMsg.stLogData[siCount].m_siParam5) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szCharParam1, 									sizeof(sendMsg.stLogData[siCount].m_szCharParam1) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szCharParam2, 									sizeof(sendMsg.stLogData[siCount].m_szCharParam2) );

		siCount++;
//		printf( "Count : %d\n" , siCount);
		siTotalCount++;
		if(MAX_STLOGDATA_PER_PACKET <= siCount)
		{
			// 다 찾으므로 일단 클라이언트에 전송
			sendMsg.siResult = 1;
			if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
                if(pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg ) == FALSE)		break;
			}
			else
			{
				// 연결이 끊어졌다면 함수를 종료한다.
				printf( "Connection Closed\n" );
				break;
			}

			//네트워크 부하를 줄이기 위해 잠깐 휴식
			Sleep(100);

			// 패킷 내용 삭제
			ZeroMemory(&sendMsg, sizeof(sendMsg));

			// 패킷 기본값 설정
			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.usCharID = pRecvMsg->usCharID;

			bSend = TRUE;
			siCount = 0;
		}
	END_FETCHDATA()
	END_NEXTRECORDSET()

	printf( "Total NewLog Sended [%d] - %d\n", pClient->GetCharID(), siTotalCount );

	if(siCount > 0 && pSession->GetState() == SESSION_STATE_ESTABLISHED)
	{
		sendMsg.siResult = 1;		
		pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
	}
}

void DoMsg_DBMSG_REQUEST_NEWLOG4(sPacketHeader* pPacket, NThreadParameter* pThreadParameter)
{
	cLogClient* pClient = pThreadParameter->m_pClient;
	if(pClient == NULL)			return;

	ITZSession*	pSession = pClient->GetSession();
	if(pSession == NULL)			return;

	printf( "DBMSG_REQUEST_NEWLOG4 [%d]\n", pClient->GetCharID() );

	// 받은 패킷
	sDBRequest_NewLog	*pRecvMsg = (sDBRequest_NewLog *)pPacket;

	// 보낼 패킷
	sDBResponse_NewLog sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.usCharID		= pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonalLog4" )
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szLoginID,			sizeof(pRecvMsg->szLoginID) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szLoginPW,			sizeof(pRecvMsg->szLoginPW) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szServerName,			sizeof(pRecvMsg->szServerName) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName,			sizeof(pRecvMsg->szPersonName) );
	pThreadParameter->m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique,		sizeof(pRecvMsg->siItemUnique) );
	pThreadParameter->m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemSID,			sizeof(pRecvMsg->siItemSID) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szStartDate,			sizeof(pRecvMsg->szStartDate) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szEndDate,			sizeof(pRecvMsg->szEndDate) );
	pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiCommandType,		sizeof(pRecvMsg->uiCommandType) );
	pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiCommandIndex,		sizeof(pRecvMsg->uiCommandIndex) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szDetailOption,		sizeof(pRecvMsg->szDetailOption) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szIP,					sizeof(pRecvMsg->szIP) );
	pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szAccountID,			sizeof(pRecvMsg->szAccountID) );
	END_SP()

	//EXEC_SQL()
	if(!pThreadParameter->m_odbc.ExecSQL())
	{
		//printf( "Total Log Sended [%d] - %d\n", 0);
		printf( "Query Execute Error\n");

		// 패킷 내용 삭제
		ZeroMemory(&sendMsg, sizeof(sendMsg));

		// 패킷 기본값 설정
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.usCharID = pRecvMsg->usCharID;

		sendMsg.siResult = -1;
		pClient->GetSession()->GetSendBuffer()->Write( (BYTE*)&sendMsg );

		return;
	}

//	printf( "Query Execute Success\n" );

	BOOL bSend = FALSE;
	SI32 siCount = 0;
	SI32 siTotalCount = 0;
	SI32 siResult = 0;
	TIMESTAMP_STRUCT stCDate;

	BEGIN_FETCHDATA()
		pThreadParameter->m_odbc.GetData( &siResult,		sizeof(siResult) );
	END_FETCHDATA()

	switch(siResult)
	{
	case 1:				break;
	case -1:	// 아이디 틀림
	case -2:	// 패스워드 틀림
		{
			sendMsg.siResult = siResult;
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );

			printf( "Login Failed [%d] [%s] - %d\n", pClient->GetCharID(), 
							pThreadParameter->m_pClient->m_RequestNewLog.szLoginID, 
							siResult );

//			pThreadParameter->m_pClient->m_bRequestNewLog = FALSE;
//			ZeroMemory(&pThreadParameter->m_pClient->m_RequestNewLog, sizeof(pThreadParameter->m_pClient->m_RequestNewLog));

			return;
		}
		break;
	}

//	printf( "Login Success\n" );

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		bSend = FALSE;
		pThreadParameter->m_odbc.GetData( &stCDate, sizeof(stCDate) );
		{
			sendMsg.stLogData[siCount].m_stTime.wYear = stCDate.year;
			sendMsg.stLogData[siCount].m_stTime.wMonth = stCDate.month;
			sendMsg.stLogData[siCount].m_stTime.wDay = stCDate.day;
			sendMsg.stLogData[siCount].m_stTime.wHour = stCDate.hour;
			sendMsg.stLogData[siCount].m_stTime.wMinute = stCDate.minute;
			sendMsg.stLogData[siCount].m_stTime.wSecond = stCDate.second;
			sendMsg.stLogData[siCount].m_stTime.wMilliseconds = (WORD)stCDate.fraction/1000000;
		}

		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szIP, sizeof(sendMsg.stLogData[siCount].m_szIP) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siCommandType, sizeof(sendMsg.stLogData[siCount].m_siCommandType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siCommandIndex, sizeof(sendMsg.stLogData[siCount].m_siCommandIndex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_uiReason, sizeof(sendMsg.stLogData[siCount].m_uiReason) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siIDNum, sizeof(sendMsg.stLogData[siCount].m_siIDNum) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szPersonName, sizeof(sendMsg.stLogData[siCount].m_szPersonName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siPersonID, sizeof(sendMsg.stLogData[siCount].m_siPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siLevel, sizeof(sendMsg.stLogData[siCount].m_siLevel) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siExp, sizeof(sendMsg.stLogData[siCount].m_siExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMoney, sizeof(sendMsg.stLogData[siCount].m_siMoney) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMoneySum, sizeof(sendMsg.stLogData[siCount].m_siMoneySum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siUnique, sizeof(sendMsg.stLogData[siCount].m_clItem.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siItemNum, sizeof(sendMsg.stLogData[siCount].m_clItem.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cData8, sizeof(sendMsg.stLogData[siCount].m_clItem.cData8) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateYear, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateYear) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateMonth, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateMonth) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateDay, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateDay) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.uiDateUseDay, sizeof(sendMsg.stLogData[siCount].m_clItem.uiDateUseDay) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siMakerPersonID, sizeof(sendMsg.stLogData[siCount].m_clItem.siMakerPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.Element.siType, sizeof(sendMsg.stLogData[siCount].m_clItem.Element.siType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.Element.siPower, sizeof(sendMsg.stLogData[siCount].m_clItem.Element.siPower) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siSID, sizeof(sendMsg.stLogData[siCount].m_clItem.siSID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siPalette, sizeof(sendMsg.stLogData[siCount].m_clItem.siPalette) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.siUseCrystalNum, sizeof(sendMsg.stLogData[siCount].m_clItem.siUseCrystalNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siCrystalType, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siMaxDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbility, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbilityNum, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[0].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siCrystalType, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siMaxDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbility, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbilityNum, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[1].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siCrystalType, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siMaxDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbility, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbilityNum, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[2].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siCrystalType, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siMaxDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbility, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbilityNum, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[3].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siCrystalType, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siCrystalType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siMaxDurability, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].siMaxDurability) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbility, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbility) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbilityNum, sizeof(sendMsg.stLogData[siCount].m_clItem.cltItemCrytal[4].uiCrystalAbilityNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siItemSum, sizeof(sendMsg.stLogData[siCount].m_siItemSum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siUnique, sizeof(sendMsg.stLogData[siCount].m_clSummon.siUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siKind, sizeof(sendMsg.stLogData[siCount].m_clSummon.siKind) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_clSummon.szName, sizeof(sendMsg.stLogData[siCount].m_clSummon.szName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siStatus, sizeof(sendMsg.stLogData[siCount].m_clSummon.siStatus) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siAttackType, sizeof(sendMsg.stLogData[siCount].m_clSummon.siAttackType) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siIdentity, sizeof(sendMsg.stLogData[siCount].m_clSummon.siIdentity) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siStr, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siStr) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siDex, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siDex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siVit, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siVit) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siMag, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siMag) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siLuk, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siLuk) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siExp, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siLevel, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siLevel) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.siBonus, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.siBonus) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.Life, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.Life) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clIP.Mana, sizeof(sendMsg.stLogData[siCount].m_clSummon.clIP.Mana) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[0], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[0]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[0], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[0]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[1], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siUnique[1]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[1], sizeof(sendMsg.stLogData[siCount].m_clSummon.clScroll.siPercent[1]) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clSummon.siTakeInMinute, sizeof(sendMsg.stLogData[siCount].m_clSummon.siTakeInMinute) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siMonKind, sizeof(sendMsg.stLogData[siCount].m_siMonKind) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szTargetPersonName, sizeof(sendMsg.stLogData[siCount].m_szTargetPersonName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siTargetPersonID, sizeof(sendMsg.stLogData[siCount].m_siTargetPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siVillageUnique, sizeof(sendMsg.stLogData[siCount].m_siVillageUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siNpcKind, sizeof(sendMsg.stLogData[siCount].m_siNpcKind) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siHungry, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siHungry) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg1, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg1) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg2, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg2) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg3, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg3) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg4, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.siInnerOrg4) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg1Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg1Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg2Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg2Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg3Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg3Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg4Disease, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiOrg4Disease) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_clHealthInfo.uiHealthPoint, sizeof(sendMsg.stLogData[siCount].m_clHealthInfo.uiHealthPoint) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siRPPoint, sizeof(sendMsg.stLogData[siCount].m_siRPPoint) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siFameExp, sizeof(sendMsg.stLogData[siCount].m_siFameExp) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam1, sizeof(sendMsg.stLogData[siCount].m_siParam1) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam2, sizeof(sendMsg.stLogData[siCount].m_siParam2) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam3, sizeof(sendMsg.stLogData[siCount].m_siParam3) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam4, sizeof(sendMsg.stLogData[siCount].m_siParam4) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].m_siParam5, sizeof(sendMsg.stLogData[siCount].m_siParam5) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szCharParam1, sizeof(sendMsg.stLogData[siCount].m_szCharParam1) );
		pThreadParameter->m_odbc.GetData( sendMsg.stLogData[siCount].m_szCharParam2, sizeof(sendMsg.stLogData[siCount].m_szCharParam2) );

		siCount++;
//		printf( "Count : %d\n" , siCount);
		siTotalCount++;
		if(MAX_STLOGDATA_PER_PACKET <= siCount)
		{
			// 다 찾으므로 일단 클라이언트에 전송
			sendMsg.siResult = 1;
			if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
                if(pSession->GetSendBuffer()->Write( (BYTE *)&sendMsg ) == FALSE)		break;
			}
			else
			{
				// 연결이 끊어졌다면 함수를 종료한다.
				printf( "Connection Closed\n" );
				break;
			}

			//네트워크 부하를 줄이기 위해 잠깐 휴식
			Sleep(100);

			// 패킷 내용 삭제
			ZeroMemory(&sendMsg, sizeof(sendMsg));

			// 패킷 기본값 설정
			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWLOG;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.usCharID = pRecvMsg->usCharID;

			bSend = TRUE;
			siCount = 0;
		}
	END_FETCHDATA()
	END_NEXTRECORDSET()

	printf( "Total NewLog Sended [%d] - %d\n", pClient->GetCharID(), siTotalCount );

	if(siCount > 0 && pSession->GetState() == SESSION_STATE_ESTABLISHED)
	{
		sendMsg.siResult = 1;		
		pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
	}
}

void DoMsg_DBMSG_REQUEST_TOOLSLOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter)
{ 
	cLogClient* pClient = pThreadParameter->m_pClient;
	if(pClient == NULL)			return;

	ITZSession*	pSession = pClient->GetSession();
	if(pSession == NULL)			return;

	printf( "DBMSG_REQUEST_TOOLSLOG\n" );

    // 받은 패킷
	sDBRequest_ToolsLog	*pRecvMsg = (sDBRequest_ToolsLog *)pPacket;

	//	rsp_ToolsLogSet
	//	@Command	varchar	50
	//	@AccountID	varchar	20
	//	@CharName	varchar	20
	//	@Log		varchar	256
	
	// DB Query
	BEGIN_SP( "rsp_ToolsLogSet" )
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szServerName, sizeof(pRecvMsg->szServerName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szCommand,	 sizeof(pRecvMsg->szCommand) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szAccountID,	 sizeof(pRecvMsg->szAccountID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szCharName,	 sizeof(pRecvMsg->szCharName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR, &pRecvMsg->szLog,		 sizeof(pRecvMsg->szLog) );
	END_SP()

	EXEC_SQL()

	pClient->m_bRequestToolsLog = FALSE;
	ZeroMemory( &pThreadParameter->m_pClient->m_RequestToolsLog, sizeof(pThreadParameter->m_pClient->m_RequestToolsLog) );
}
