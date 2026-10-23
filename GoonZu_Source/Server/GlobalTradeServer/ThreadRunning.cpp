//=============================================================================
//
//	ThreadRunning.cpp
//
//	Copyright 2007 HQGame
//
//=============================================================================
#include "DBLogProtocol.h"

extern DBLogProtocol			g_dap;

#define LOG_BEGIN_SP(a)				sprintf( pThreadParameter->m_szSPName, "%s.dbo.%s", g_dbname, a ); pThreadParameter->m_odbc.BeginSP( pThreadParameter->m_szSPName );
#define LOG_END_SP()				pThreadParameter->m_odbc.EndSP();
#define LOG_EXEC_SQL()				if(!pThreadParameter->m_odbc.ExecSQL()) return;
#define LOG_BEGIN_FETCHDATA()		while(pThreadParameter->m_odbc.FetchData() > 0) {
#define LOG_END_FETCHDATA()			}
#define LOG_BEGIN_NEXTRECORDSET()	if(pThreadParameter->m_odbc.NextRecordSet() == 1) {
#define LOG_END_NEXTRECORDSET()		}

void DoMsg_DBMSG_REQUEST_SELLED_LOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter);
void DoMsg_DBMSG_REQUEST_TRADE_LOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter);

void ThreadRunning(LPVOID lpParam)
{
	NThreadParameter *pThreadParameter = (NThreadParameter*)lpParam;
	if(pThreadParameter == NULL)		return;

	while( pThreadParameter->m_bThreadRunning )
	{
		Sleep( 10 );

		if(pThreadParameter->m_pClient != NULL)
		{
			if( pThreadParameter->m_pClient->m_bSelledItemLog )
			{
				DoMsg_DBMSG_REQUEST_SELLED_LOG( (sPacketHeader*)&pThreadParameter->m_pClient->m_SelledItemLog, pThreadParameter );

				pThreadParameter->m_pClient->m_bSelledItemLog = FALSE;
				ZeroMemory(&pThreadParameter->m_pClient->m_SelledItemLog, sizeof(pThreadParameter->m_pClient->m_SelledItemLog));
			}
			else if( pThreadParameter->m_pClient->m_bTradeItemLog )
			{
				DoMsg_DBMSG_REQUEST_TRADE_LOG( (sPacketHeader*)&pThreadParameter->m_pClient->m_TradeItemLog, pThreadParameter );

				pThreadParameter->m_pClient->m_bTradeItemLog = FALSE;
				ZeroMemory(&pThreadParameter->m_pClient->m_TradeItemLog, sizeof(pThreadParameter->m_pClient->m_TradeItemLog));
			}
		}
	}
}

void DoMsg_DBMSG_REQUEST_SELLED_LOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter)
{
	cLogClient* pClient = pThreadParameter->m_pClient;
	if(pClient == NULL)			return;

	ITZSession*	pSession = pClient->GetSession();
	if(pSession == NULL)			return;

	// 받은 패킷
	sDBRequest_SelledItemLog	*pRecvMsg = (sDBRequest_SelledItemLog *)pPacket;

	// 보낼 패킷
	sDBResponse_SelledItemLog sendMsg;

	sendMsg.packetHeader.usCmd = TRADELOG_DBMSG_RESPONSE_SELLED_LOG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	LOG_BEGIN_SP( "rsp_GetSeledItemInfo" )
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szBuyServerNation,	sizeof(pRecvMsg->szBuyServerNation) );
		pThreadParameter->m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siBuyServerIndex,	sizeof(pRecvMsg->siBuyServerIndex) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szBuyID,			sizeof(pRecvMsg->szBuyID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szBuyCharName,		sizeof(pRecvMsg->szBuyCharName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szSellServerNation,	sizeof(pRecvMsg->szSellServerNation) );
		pThreadParameter->m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSellServerIndex,	sizeof(pRecvMsg->siSellServerIndex) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szSellID,			sizeof(pRecvMsg->szSellID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szSellCharName,		sizeof(pRecvMsg->szSellCharName) );
		pThreadParameter->m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique,		sizeof(pRecvMsg->siItemUnique) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szStartDate,		sizeof(pRecvMsg->szStartDate) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szEndDate,			sizeof(pRecvMsg->szEndDate) );
		pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siSendFlag,			sizeof(pRecvMsg->siSendFlag) );
		pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siCancelFlag,		sizeof(pRecvMsg->siCancelFlag) );
		pThreadParameter->m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siAcceptFlag,		sizeof(pRecvMsg->siAcceptFlag) );
	LOG_END_SP()

	if(!pThreadParameter->m_odbc.ExecSQL())
	{
		if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
		{
			sendMsg.siResult = -10;
			pSession->GetSendBuffer()->Write( (char*)&sendMsg );
		}
		return;
	}

	SI32 siCount = 0;
	TIMESTAMP_STRUCT stCDate;
	LOG_BEGIN_FETCHDATA();
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siSendFlag,		sizeof(sendMsg.stLogData[siCount].siSendFlag) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siCancelFlag,		sizeof(sendMsg.stLogData[siCount].siCancelFlag) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siItemAcceptFlag,	sizeof(sendMsg.stLogData[siCount].siItemAcceptFlag) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szBuyServerNation,sizeof(sendMsg.stLogData[siCount].szBuyServerNation) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siBuyServerIndex,	sizeof(sendMsg.stLogData[siCount].siBuyServerIndex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siBuyCharNation,	sizeof(sendMsg.stLogData[siCount].siBuyCharNation) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szBuyUserID,		sizeof(sendMsg.stLogData[siCount].szBuyUserID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szBuyCharName,	sizeof(sendMsg.stLogData[siCount].szBuyCharName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siBuyPersonID,	sizeof(sendMsg.stLogData[siCount].siBuyPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szSellServerNation,sizeof(sendMsg.stLogData[siCount].szSellServerNation) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siSellServerIndex,sizeof(sendMsg.stLogData[siCount].siSellServerIndex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siSellCharNation,	sizeof(sendMsg.stLogData[siCount].siSellCharNation) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szSellUserID,		sizeof(sendMsg.stLogData[siCount].szSellUserID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szSellCharName,	sizeof(sendMsg.stLogData[siCount].szSellCharName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siSellPersonID,	sizeof(sendMsg.stLogData[siCount].siSellPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siItemUnique,		sizeof(sendMsg.stLogData[siCount].siItemUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siItemNum,		sizeof(sendMsg.stLogData[siCount].siItemNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siPrice,			sizeof(sendMsg.stLogData[siCount].siPrice) );

		pThreadParameter->m_odbc.GetData( &stCDate, sizeof(stCDate) );
		{
			sendMsg.stLogData[siCount].stSellDate.wYear = stCDate.year;
			sendMsg.stLogData[siCount].stSellDate.wMonth = stCDate.month;
			sendMsg.stLogData[siCount].stSellDate.wDay = stCDate.day;
			sendMsg.stLogData[siCount].stSellDate.wHour = stCDate.hour;
			sendMsg.stLogData[siCount].stSellDate.wMinute = stCDate.minute;
			sendMsg.stLogData[siCount].stSellDate.wSecond = stCDate.second;
			sendMsg.stLogData[siCount].stSellDate.wMilliseconds = (WORD)stCDate.fraction/1000000;
		}
		pThreadParameter->m_odbc.GetData( &stCDate, sizeof(stCDate) );
		{
			sendMsg.stLogData[siCount].stBuyDate.wYear = stCDate.year;
			sendMsg.stLogData[siCount].stBuyDate.wMonth = stCDate.month;
			sendMsg.stLogData[siCount].stBuyDate.wDay = stCDate.day;
			sendMsg.stLogData[siCount].stBuyDate.wHour = stCDate.hour;
			sendMsg.stLogData[siCount].stBuyDate.wMinute = stCDate.minute;
			sendMsg.stLogData[siCount].stBuyDate.wSecond = stCDate.second;
			sendMsg.stLogData[siCount].stBuyDate.wMilliseconds = (WORD)stCDate.fraction/1000000;
		}

		siCount++;

		if( siCount >= MAX_STLOGDATA_PER_PACKET)
		{
			sendMsg.siResult = 1;
			if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
			{
				if(pSession->GetSendBuffer()->Write( (char *)&sendMsg ) == FALSE)		break;
			}

			Sleep(100);

			// 패킷 내용 삭제
			ZeroMemory(&sendMsg, sizeof(sendMsg));

			// 패킷 기본값 설정
			sendMsg.packetHeader.usCmd = TRADELOG_DBMSG_RESPONSE_SELLED_LOG;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.usCharID = pRecvMsg->usCharID;

			siCount = 0;
		}
	LOG_END_FETCHDATA();
    
	// 마지막 패킷을 보낸다.
	if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
	{
		sendMsg.siResult = -100;
		pSession->GetSendBuffer()->Write( (char*)&sendMsg );
	}
}

void DoMsg_DBMSG_REQUEST_TRADE_LOG(sPacketHeader* pPacket, NThreadParameter* pThreadParameter)
{
	cLogClient* pClient = pThreadParameter->m_pClient;
	if(pClient == NULL)			return;

	ITZSession*	pSession = pClient->GetSession();
	if(pSession == NULL)			return;

	// 받은 패킷
	sDBRequest_TradeItemLog	*pRecvMsg = (sDBRequest_TradeItemLog *)pPacket;

	// 보낼 패킷
	sDBResponse_TradeItemLog sendMsg;

	sendMsg.packetHeader.usCmd = TRADELOG_DBMSG_RESPONSE_TRADE_LOG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	LOG_BEGIN_SP( "rsp_GetTradeItemInfo" )
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szServerNation,	sizeof(pRecvMsg->szServerNation) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->siServerIndex,	sizeof(pRecvMsg->siServerIndex) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szID,			sizeof(pRecvMsg->szID) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szCharName,		sizeof(pRecvMsg->szCharName) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->siItemUnique,	sizeof(pRecvMsg->siItemUnique) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szStartDate,	sizeof(pRecvMsg->szStartDate) );
		pThreadParameter->m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szEndDate,		sizeof(pRecvMsg->szEndDate) );
	LOG_END_SP();

	if(!pThreadParameter->m_odbc.ExecSQL())
	{
		if(pSession->GetState() == SESSION_STATE_ESTABLISHED)
		{
			sendMsg.siResult = -10;
			pSession->GetSendBuffer()->Write( (char*)&sendMsg );
		}
		return;
	}

	SI32 siCount = 0;
	TIMESTAMP_STRUCT stCDate;
	LOG_BEGIN_FETCHDATA();
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siOrderIndex,		sizeof(sendMsg.stLogData[siCount].siOrderIndex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szServerNation,	sizeof(sendMsg.stLogData[siCount].szServerNation) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siServerIndex,	sizeof(sendMsg.stLogData[siCount].siServerIndex) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siCharNation,		sizeof(sendMsg.stLogData[siCount].siCharNation) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szUserID,			sizeof(sendMsg.stLogData[siCount].szUserID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].szCharName,		sizeof(sendMsg.stLogData[siCount].szCharName) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siPersonID,		sizeof(sendMsg.stLogData[siCount].siPersonID) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siItemUnique,		sizeof(sendMsg.stLogData[siCount].siItemUnique) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siItemNum,		sizeof(sendMsg.stLogData[siCount].siItemNum) );
		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siPrice,			sizeof(sendMsg.stLogData[siCount].siPrice) );

		pThreadParameter->m_odbc.GetData( &stCDate, sizeof(stCDate) );
		{
			sendMsg.stLogData[siCount].stSellDate.wYear = stCDate.year;
			sendMsg.stLogData[siCount].stSellDate.wMonth = stCDate.month;
			sendMsg.stLogData[siCount].stSellDate.wDay = stCDate.day;
			sendMsg.stLogData[siCount].stSellDate.wHour = stCDate.hour;
			sendMsg.stLogData[siCount].stSellDate.wMinute = stCDate.minute;
			sendMsg.stLogData[siCount].stSellDate.wSecond = stCDate.second;
			sendMsg.stLogData[siCount].stSellDate.wMilliseconds = (WORD)stCDate.fraction/1000000;
		}

		pThreadParameter->m_odbc.GetData( &sendMsg.stLogData[siCount].siHoldAcu,			sizeof(sendMsg.stLogData[siCount].siHoldAcu) );
	LOG_END_FETCHDATA();

	// 마지막 패킷을 보낸다.
	if( pSession->GetState() == SESSION_STATE_ESTABLISHED )
	{
		sendMsg.siResult = -100;
		pSession->GetSendBuffer()->Write( (char*)&sendMsg );
	}
}