#include "DBAuthProtocol.h"
#include "AccountDBMsg.h"

#define MAX_LEVEL_VALUE		100
#define DUMPCASE			10

DBAuthProtocol::DBAuthProtocol()
{
}

DBAuthProtocol::~DBAuthProtocol()
{
	m_odbc.Disconnect();
}

void DBAuthProtocol::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW )
{
	m_odbc.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );

	m_SF.CreateFactory( GetSessionManager(), 1, 2 * 1000 * 1000, 2 * 1000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	
	GetListen()->CreateListen( GetSessionManager(), usPort );

	SetFPS( 200 );
}

void DBAuthProtocol::Convert_SYSTEMTIME_from_TIMESTAMP(_SYSTEMTIME *stST, TIMESTAMP_STRUCT *stTS)
{
	stST->wYear = stTS->year;
	stST->wMonth = stTS->month;
	stST->wDay = stTS->day;
	stST->wHour = stTS->hour;
	stST->wMinute = stTS->minute;
	stST->wSecond = stTS->second;
	stST->wMilliseconds = (WORD)stTS->fraction/1000000;
}

void DBAuthProtocol::Convert_TIMESTAMP_from_SYSTEMTIME(TIMESTAMP_STRUCT *stTS, _SYSTEMTIME *stST)
{
	stTS->year = stST->wYear;
	stTS->month = stST->wMonth;
	stTS->day = stST->wDay;
	stTS->hour = stST->wHour;
	stTS->minute = stST->wMinute;
	stTS->second = stST->wSecond;
	stTS->fraction = (SQLUINTEGER)stST->wMilliseconds * 1000000;

	if ( stTS->year == 0 )
	{
		stTS->year = 9999;
		stTS->month = 12;
		stTS->day = 31;
	}
}

void DBAuthProtocol::Update()
{
}

void DBAuthProtocol::SessionProc( ITZSession *pSession )
{

	int i, count;
	sPacketHeader *pPacket;

	count = pSession->GetRecvBuffer()->GetPacketCount();

	for( i = 0; i < count; ++i ) {

		// ���ۿ��� ��Ŷ�� �ϳ� �����´� 
		pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );
																								
		if( pPacket == NULL ) return;	// ��Ŷ�� ������ ������
		
		// ��Ŷ Ŀ�ǵ忡 ���� ó��
		switch( pPacket->usCmd ) {
		case DBMSG_REQUEST_IDNUMGET:
			{
				printf( "DBMSG_REQUEST_IDNUMGET\n" );

				// ���� ��Ŷ
				sDBRequest_IDNumGet	*pRecvMsg = (sDBRequest_IDNumGet *)pPacket;

				pRecvMsg->userid[ sizeof(pRecvMsg->userid) - 1 ] = NULL;
				pRecvMsg->userpw[ sizeof(pRecvMsg->userpw) - 1 ] = NULL;
				pRecvMsg->userIP[ sizeof(pRecvMsg->userIP) - 1 ] = NULL;
				pRecvMsg->jumin[ sizeof(pRecvMsg->jumin) - 1 ] = NULL;
				pRecvMsg->macaddress[ sizeof(pRecvMsg->macaddress) - 1 ] = NULL;

				// ���� ��Ŷ
				sDBResponse_IDNumGet sendMsg;

				ZeroMemory( &sendMsg, sizeof( sendMsg ) );

				sendMsg.idnum = -1;

				sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_IDNUMGET;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );
				
				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

				memcpy( sendMsg.userid, pRecvMsg->userid,			sizeof(sendMsg.userid) );
				memcpy( sendMsg.userpw, pRecvMsg->userpw,			sizeof(sendMsg.userpw) );
				memcpy( sendMsg.userIP, pRecvMsg->userIP,			sizeof(sendMsg.userIP) );
				memcpy( sendMsg.macaddress, pRecvMsg->macaddress,	sizeof(sendMsg.macaddress) );

				printf( "pRecvMsg = Id: %s, Pwd: %s,  userIP: %s \n", pRecvMsg->userid , pRecvMsg->userpw , pRecvMsg->userIP);
				printf( "sendMsg = Id: %s, Pwd: %s,  userIP: %s \n", sendMsg.userid, sendMsg.userpw , sendMsg.userIP);

				BEGIN_SP( "rsp_IDNumGet" )
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userid,		 	sizeof(pRecvMsg->userid) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userpw,		 	sizeof(pRecvMsg->userpw) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userIP,		 	sizeof(pRecvMsg->userIP) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->jumin,		 	sizeof(pRecvMsg->jumin) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->macaddress,  	sizeof(pRecvMsg->macaddress) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->requestTime, 	sizeof(pRecvMsg->requestTime) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->mkey1,		 	sizeof(pRecvMsg->mkey1) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->mkey2,		 	sizeof(pRecvMsg->mkey2) );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->key,			sizeof(pRecvMsg->key));
					m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->accountstatus,	sizeof(pRecvMsg->accountstatus));
				END_SP()

				EXEC_SQL()
	
				sendMsg.szBlockReason[ 0 ] = NULL;

				BEGIN_FETCHDATA()
					m_odbc.GetData( &sendMsg.idnum, 0 );
					m_odbc.GetData( &sendMsg.siGameMasterLevel, 0 );
					m_odbc.GetData( &sendMsg.siBlockMode, 0);
					m_odbc.GetData( &sendMsg.siBlockYear, 0);
					m_odbc.GetData( &sendMsg.siBlockMonth, 0);
					m_odbc.GetData( &sendMsg.siBlockDay, 0);
					m_odbc.GetData( &sendMsg.siBlockHour, 0);
					m_odbc.GetData( &sendMsg.siBlockMin, 0);
					m_odbc.GetData( sendMsg.szBlockReason, sizeof(sendMsg.szBlockReason));
					m_odbc.GetData( &sendMsg.bAlreadyMakeCharSwitch, 0 );
					m_odbc.GetData( sendMsg.szServerName, sizeof(sendMsg.szServerName));
					m_odbc.GetData( &sendMsg.siAccountStatus, 0 );
					m_odbc.GetData( sendMsg.userid, sizeof(sendMsg.userid));
					m_odbc.GetData( &sendMsg.m_uiRealAge, 0 );
				END_FETCHDATA()
				
				printf( "FrontCharID:%d, Userid:%s, IDNum:%d, MasterLevel:%d, Blockmode:%d AccountStatus:%d IP:%s\n",
				pRecvMsg->usFrontCharID, 
				pRecvMsg->userid,
				sendMsg.idnum,
				sendMsg.siGameMasterLevel,
				sendMsg.siBlockMode,
				sendMsg.siAccountStatus,
				sendMsg.userIP
				);
				fflush(stdout);


				// send
				pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );

			}
			break;

		case DBMSG_REQUEST_GAMEAUTH:
			{
				printf( "DBMSG_REQUEST_GAMEAUTH\n" );

				char ip[ 20 ];
				ip[ 0 ] = NULL;

				// ���� ��Ŷ
				sDBRequest_GameAuth	*pRecvMsg = (sDBRequest_GameAuth *)pPacket;

				pRecvMsg->userid[ sizeof(pRecvMsg->userid) - 1 ] = NULL;
				pRecvMsg->userpw[ sizeof(pRecvMsg->userpw) - 1 ] = NULL;

				// ���� ��Ŷ
				sDBResponse_GameAuth sendMsg;
				
				ZeroMemory( &sendMsg, sizeof( sendMsg ) );
				
				sendMsg.idnum = -1;

				sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GAMEAUTH;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usGameServerID = pRecvMsg->usGameServerID;
				sendMsg.usCharID = pRecvMsg->usCharID;
				sendMsg.uiIDNo   = pRecvMsg->uiIDNo;	// �Ѱ��ӿ� IDNO - by LEEKH 2007.11.11

				BEGIN_SP( "rsp_GameAuth" )
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userid, sizeof(pRecvMsg->userid) - 1 );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userpw, sizeof(pRecvMsg->userpw) - 1 );
					m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->idnum, sizeof(pRecvMsg->idnum));
				END_SP()

				printf("[ACCTDB] GAMEMAUTH req idnum=%d usCharID=%d uiIDNo=%d\n",
					(int)pRecvMsg->idnum, (int)pRecvMsg->usCharID, (int)pRecvMsg->uiIDNo);
				printf("[ACCTDB] LAYOUT respSize=%d off(idnum)=%d off(siSex)=%d off(siGameMasterLevel)=%d off(siBlockMode)=%d off(szBlockReason)=%d off(siAccountStatus)=%d off(siDormancyState)=%d off(userid)=%d sizeof(resp.userid)=%d sizeof(req.userid)=%d\n",
					(int)sizeof(sDBResponse_GameAuth),
					(int)((char*)&sendMsg.idnum - (char*)&sendMsg),
					(int)((char*)&sendMsg.siSex - (char*)&sendMsg),
					(int)((char*)&sendMsg.siGameMasterLevel - (char*)&sendMsg),
					(int)((char*)&sendMsg.siBlockMode - (char*)&sendMsg),
					(int)((char*)&sendMsg.szBlockReason - (char*)&sendMsg),
					(int)((char*)&sendMsg.siAccountStatus - (char*)&sendMsg),
					(int)((char*)&sendMsg.siDormancyState - (char*)&sendMsg),
					(int)((char*)&sendMsg.userid - (char*)&sendMsg),
					(int)sizeof(sendMsg.userid),
					(int)sizeof(pRecvMsg->userid));
				fflush(stdout);

				EXEC_SQL()

				printf("[ACCTDB] GAMEMAUTH EXEC_SQL done isSuccess=%d retcode=%d\n",
					(int)m_odbc.IsSuccess(), m_odbc.GetRetcode());
				fflush(stdout);

				sendMsg.szBlockReason[ 0 ] = NULL;

				int rcGaIdnum = -999;
				int rcGaGm = -999;
				int rcGaSex = -999;
				int fetchRowsGa = 0;

				SI32	tmpGaIdnum = -1;
				SI32	tmpGaSex  = 0;

				BEGIN_FETCHDATA()
					fetchRowsGa++;
					rcGaIdnum = m_odbc.GetData( &tmpGaIdnum, sizeof(tmpGaIdnum) );
					rcGaGm = m_odbc.GetData( &sendMsg.siGameMasterLevel, 0 );
					rcGaSex = m_odbc.GetData( &tmpGaSex, sizeof(tmpGaSex) );
					m_odbc.GetData( &sendMsg.siBlockMode, 0);
					m_odbc.GetData( &sendMsg.siBlockYear, 0);
					m_odbc.GetData( &sendMsg.siBlockMonth, 0);
					m_odbc.GetData( &sendMsg.siBlockDay, 0);
					m_odbc.GetData( &sendMsg.siBlockHour, 0);
					m_odbc.GetData( &sendMsg.siBlockMin, 0);
					m_odbc.GetData( sendMsg.szBlockReason, sizeof(sendMsg.szBlockReason));
					m_odbc.GetData( &sendMsg.siAccountStatus, 0);
					m_odbc.GetData( &sendMsg.siPlayingTime, 0);
					m_odbc.GetData( &sendMsg.siDormancyState, 0);
				END_FETCHDATA()

				sendMsg.idnum = tmpGaIdnum;
				sendMsg.siSex = (SI16)tmpGaSex;

				printf("[ACCTDB] GAMEMAUTH rows=%d rcIdnum=%d rcGm=%d rcSex=%d retcode=%d\n"
					   "[ACCTDB]   -> idnum=%d (tmp=%d) gmLevel=%d sex=%d (tmp=%d) blockMode=%d accountStatus=%d playingTime=%d dormancy=%d\n",
					fetchRowsGa, rcGaIdnum, rcGaGm, rcGaSex, m_odbc.GetRetcode(),
					(int)sendMsg.idnum, (int)tmpGaIdnum,
					(int)sendMsg.siGameMasterLevel, (int)sendMsg.siSex, (int)tmpGaSex,
					(int)sendMsg.siBlockMode, (int)sendMsg.siAccountStatus,
					(int)sendMsg.siPlayingTime, (int)sendMsg.siDormancyState);
				fflush(stdout);

				// KHY - 
				//sendMsg.siPlayingTime = pRecvMsg->siPlayingTime;
				sendMsg.uiIDNo			= pRecvMsg->uiIDNo;		// For USA

				//KHY - 0218 - �߱� NHN �Ƿε� ��� ���� - �ʿ���.
				memcpy(sendMsg.userid, pRecvMsg->userid, sizeof(sendMsg.userid));

				printf( "GameCharID:%d, userid:%s, idnum:%d, gameMasterLevel:%d, Sex:%d, BlockMode = %d, IDNo = %d, DormancyState = %d \n", 
					pRecvMsg->usCharID, 
					pRecvMsg->userid,
					sendMsg.idnum,
					sendMsg.siGameMasterLevel,
					sendMsg.siSex,
					sendMsg.siBlockMode,
					sendMsg.uiIDNo, 
					sendMsg.siDormancyState);

				// send
				pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
			}
			break;

		case DBMSG_REQUEST_GMTOOLLOGIN:
			{
				printf( "DBMSG_REQUEST_GMTOOLLOGIN\n" );

				char ip[ 20 ];
				ip[ 0 ] = NULL;

				// ���� ��Ŷ
				sDBRequest_GMTOOLLogin	*pRecvMsg = (sDBRequest_GMTOOLLogin *)pPacket;

				pRecvMsg->userid[ sizeof(pRecvMsg->userid) - 1 ] = NULL;
				pRecvMsg->userpw[ sizeof(pRecvMsg->userpw) - 1 ] = NULL;

				// ���� ��Ŷ
				sDBResponse_GMTOOLLogin sendMsg;

				ZeroMemory( &sendMsg, sizeof( sendMsg ) );

				sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GMTOOLLOGIN;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usGameServerID = pRecvMsg->usGameServerID;
				sendMsg.usCharID = pRecvMsg->usCharID;
				memcpy(sendMsg.userid, pRecvMsg->userid, sizeof(sendMsg.userid));
				sendMsg.userid[ sizeof(sendMsg.userid) - 1 ] = NULL;

				BEGIN_SP( "rsp_IsManager" )
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userid, sizeof(pRecvMsg->userid) - 1 );
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userpw, sizeof(pRecvMsg->userpw) - 1 );
				END_SP()

				EXEC_SQL()

				BEGIN_FETCHDATA()
					m_odbc.GetData( &sendMsg.siGameMasterLevel, 0 );
				END_FETCHDATA()

				printf( "userid:%s, gameMasterLevel:%d, charid:%d\n", 
							pRecvMsg->userid,
							sendMsg.siGameMasterLevel,
							sendMsg.usCharID );

				// send
				pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
			}
			break;

		// PCK : IOVATION ���� �߰� (2009.02.03)
		case DBMSG_REQUEST_ENGLISH_IDNUMGET:
			{
				printf( "DBMSG_REQUEST_ENGLISH_IDNUMGET\n" );

				// ���� ��Ŷ
				sDBRequest_English_IDNumGet	*pRecvMsg = (sDBRequest_English_IDNumGet *)pPacket;

				pRecvMsg->userid[ sizeof(pRecvMsg->userid) - 1 ] = NULL;
				pRecvMsg->userpw[ sizeof(pRecvMsg->userpw) - 1 ] = NULL;
				pRecvMsg->userIP[ sizeof(pRecvMsg->userIP) - 1 ] = NULL;
				pRecvMsg->jumin[ sizeof(pRecvMsg->jumin) - 1 ] = NULL;
				pRecvMsg->macaddress[ sizeof(pRecvMsg->macaddress) - 1 ] = NULL;
				pRecvMsg->Blackbox[ sizeof(pRecvMsg->Blackbox) - 1 ] = NULL;

				// ���� ��Ŷ
				sDBResponse_English_IDNumGet sendMsg;

				ZeroMemory( &sendMsg, sizeof( sendMsg ) );

				sendMsg.idnum = -1;

				sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ENGLISH_IDNUMGET;
				sendMsg.packetHeader.usSize = sizeof( sendMsg );

				sendMsg.usPacketUnique = pRecvMsg->usPacketUnique;
				sendMsg.usFrontCharID = pRecvMsg->usFrontCharID;

				memcpy( sendMsg.userid, pRecvMsg->userid, sizeof(sendMsg.userid) );
				memcpy( sendMsg.userpw, pRecvMsg->userpw, sizeof(sendMsg.userpw) );
				memcpy( sendMsg.userIP, pRecvMsg->userIP, sizeof(sendMsg.userIP) );
				memcpy( sendMsg.macaddress, pRecvMsg->macaddress, sizeof(sendMsg.macaddress) );
				memcpy( sendMsg.Blackbox, pRecvMsg->Blackbox, sizeof(sendMsg.Blackbox) );

				printf( "pRecvMsg = Id: %s, Pwd: %s,  userIP: %s \n", pRecvMsg->userid , pRecvMsg->userpw , pRecvMsg->userIP);
				printf( "sendMsg = Id: %s, Pwd: %s,  userIP: %s \n", sendMsg.userid, sendMsg.userpw , sendMsg.userIP);

				BEGIN_SP( "rsp_IDNumGet" )
					m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userid, sizeof(pRecvMsg->userid) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userpw, sizeof(pRecvMsg->userpw) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->userIP, sizeof(pRecvMsg->userIP) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->jumin, sizeof(pRecvMsg->jumin) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->macaddress, sizeof(pRecvMsg->macaddress) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->requestTime, sizeof(pRecvMsg->requestTime) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->mkey1, sizeof(pRecvMsg->mkey1) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->mkey2, sizeof(pRecvMsg->mkey2) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->key, sizeof(pRecvMsg->key));
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->accountstatus, sizeof(pRecvMsg->accountstatus));
				END_SP()

				EXEC_SQL()

				printf("[ACCTDB] EXEC_SQL done isSuccess=%d retcode=%d\n",
					(int)m_odbc.IsSuccess(), m_odbc.GetRetcode());
				fflush(stdout);

				sendMsg.szBlockReason[ 0 ] = NULL;

				int rcIdnum = -999;
				int fetchRows = 0;

				BEGIN_FETCHDATA()
					fetchRows++;
					rcIdnum = m_odbc.GetData( &sendMsg.idnum, 0 );
				m_odbc.GetData( &sendMsg.siGameMasterLevel, 0 );
				m_odbc.GetData( &sendMsg.siBlockMode, 0);
				m_odbc.GetData( &sendMsg.siBlockYear, 0);
				m_odbc.GetData( &sendMsg.siBlockMonth, 0);
				m_odbc.GetData( &sendMsg.siBlockDay, 0);
				m_odbc.GetData( &sendMsg.siBlockHour, 0);
				m_odbc.GetData( &sendMsg.siBlockMin, 0);
				m_odbc.GetData( sendMsg.szBlockReason, sizeof(sendMsg.szBlockReason));
				m_odbc.GetData( &sendMsg.bAlreadyMakeCharSwitch, 0 );
				m_odbc.GetData( sendMsg.szServerName, sizeof(sendMsg.szServerName));
				m_odbc.GetData( &sendMsg.siAccountStatus, 0 );
				m_odbc.GetData( sendMsg.userid, sizeof(sendMsg.userid));
				m_odbc.GetData( sendMsg.Blackbox, sizeof(sendMsg.Blackbox));
				END_FETCHDATA()

				printf("[ACCTDB] ENGLISH_IDNUMGET rows=%d rcIdnum=%d retcode=%d\n"
					   "[ACCTDB]   -> idnum=%d gmLevel=%d blockMode=%d alreadyMade=%d accountStatus=%d\n"
					   "[ACCTDB]   -> userid='%s' serverName='%s'\n",
					fetchRows, rcIdnum, m_odbc.GetRetcode(),
					(int)sendMsg.idnum, (int)sendMsg.siGameMasterLevel, (int)sendMsg.siBlockMode,
					(int)sendMsg.bAlreadyMakeCharSwitch, (int)sendMsg.siAccountStatus,
					sendMsg.userid, sendMsg.szServerName);
				fflush(stdout);


					printf( "FrontCharID:%d, Userid:%s, IDNum:%d, MasterLevel:%d, Blockmode:%d AccountStatus:%d IP:%s\n",
					pRecvMsg->usFrontCharID, 
					pRecvMsg->userid,
					sendMsg.idnum,
					sendMsg.siGameMasterLevel,
					sendMsg.siBlockMode,
					sendMsg.siAccountStatus,
					sendMsg.userIP
					);

				// send
				pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
			}
			break;
		}

		pSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

void DBAuthProtocol::Statistics_UserCount(SYSTEMTIME stTime)
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// �ְ� ������, �ּ� ������, ������ ���
	SI32	siMax = 0, siMin = 0, siAverage = 0;
	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_UserCount.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date\tMax\tMin\tAverage\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_UserCount" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
				m_odbc.GetData( &siMax,		0 );
				m_odbc.GetData( &siMin,		0 );
				m_odbc.GetData( &siAverage,	0 );
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d\t", stTime.wYear, stTime.wMonth, stTime.wDay);
		fprintf(fp, "%d\t%d\t%d\n", siMax, siMin, siAverage);

		fclose(fp);
	}
}


void DBAuthProtocol::Statistics_UserInfo(SYSTEMTIME stTime)
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// �����̿���, ��ü �̿���, �ű� ������, ��Ƽ�� �̿���, �ְ� �̿���
	SI32	siTodayUser = 0, siTotalUser = 0, siNewUser = 0, siActiveUser = 0, siWeekUser = 0;
	// ���� �÷��̽ð�, ���� �÷��̽ð�, ��� �÷��� �ð�
	SI64	siTodayPlayTime = 0, siTotalPlayTime = 0, siAveragePlayTime = 0;

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_UserInfo.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date\tTodayUser\tTotalUser\tNewUser\tActiveUser\tWeekUser\tTodayPlayTime\tTotalPlayTime\tAveragePlayTime\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_UserInfo" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
				m_odbc.GetData( &siTodayUser,		0 );
				m_odbc.GetData( &siTotalUser,		0 );
				m_odbc.GetData( &siNewUser,			0 );
				m_odbc.GetData( &siActiveUser,		0 );
				m_odbc.GetData( &siWeekUser,		0 );
				m_odbc.GetData( &siTodayPlayTime,	0 );
				m_odbc.GetData( &siTotalPlayTime,	0 );
				m_odbc.GetData( &siAveragePlayTime,	0 );
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d\t", stTime.wYear, stTime.wMonth, stTime.wDay);
		fprintf(fp, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n", siTodayUser, siTotalUser, siNewUser, siActiveUser, siWeekUser, siTodayPlayTime, siTotalPlayTime, siAveragePlayTime);

		fclose(fp);
	}
}

void DBAuthProtocol::Statistics_TotalUserLevel(SYSTEMTIME stTime)
{

}

void DBAuthProtocol::Statistics_WeekUserLevel(SYSTEMTIME stTime)
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// ������ ����
	SI32	siLevelInfo[MAX_LEVEL_VALUE];
	SI32	i = 0;

	ZeroMemory(siLevelInfo, sizeof(siLevelInfo));

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_WeekUserLevel.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date");
			for(i=0; i<MAX_LEVEL_VALUE; i++)		fprintf(fp, "\t%d", i+1);
			fprintf(fp, "\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_WeekUserLevel" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
					m_odbc.GetData( &i );

					// 1������ [0]��, 100������ [99] �� ���Ƿ� 1�� ����
					--i;

					if(i < 0 || i >= MAX_LEVEL_VALUE)
						continue;

					m_odbc.GetData( &siLevelInfo[i],		0 );
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d", stTime.wYear, stTime.wMonth, stTime.wDay);
		for(i=0; i<MAX_LEVEL_VALUE; i++)		fprintf(fp, "\t%d", siLevelInfo[i]);
		fprintf(fp, "\n");

		fclose(fp);
	}
}

void DBAuthProtocol::Statistics_NewUserLevel(SYSTEMTIME stTime)
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// ������ ����
	SI32	siLevelInfo[MAX_LEVEL_VALUE];
	SI32	i = 0;
	ZeroMemory(siLevelInfo, sizeof(siLevelInfo));

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_NewUserLevel.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date");
			for(i=0; i<MAX_LEVEL_VALUE; i++)		fprintf(fp, "\t%d", i+1);
			fprintf(fp, "\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_NewUserLevel" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
				m_odbc.GetData( &i );

				// 1������ [0]��, 100������ [99] �� ���Ƿ� 1�� ����
				--i;

				if(i < 0 || i >= MAX_LEVEL_VALUE)
					continue;

				m_odbc.GetData( &siLevelInfo[i],		0 );
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d", stTime.wYear, stTime.wMonth, stTime.wDay);
		for(i=0; i<MAX_LEVEL_VALUE; i++)		fprintf(fp, "\t%d", siLevelInfo[i]);
		fprintf(fp, "\n");

		fclose(fp);
	}
}

void DBAuthProtocol::Statistics_ActiveUserLevel(SYSTEMTIME stTime)
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// ������ ����
	SI32	siLevelInfo[MAX_LEVEL_VALUE];
	SI32	i = 0;
	ZeroMemory(siLevelInfo, sizeof(siLevelInfo));

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_ActiveUserLevel.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date");
			for(i=0; i<MAX_LEVEL_VALUE; i++)		fprintf(fp, "\t%d", i+1);
			fprintf(fp, "\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_ActiveUserLevel" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
				m_odbc.GetData( &i );

				// 1������ [0]��, 100������ [99] �� ���Ƿ� 1�� ����
				--i;

				if(i < 0 || i >= MAX_LEVEL_VALUE)
					continue;

				m_odbc.GetData( &siLevelInfo[i],		0 );
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d", stTime.wYear, stTime.wMonth, stTime.wDay);
		for(i=0; i<MAX_LEVEL_VALUE; i++)		fprintf(fp, "\t%d", siLevelInfo[i]);
		fprintf(fp, "\n");

		fclose(fp);
	}
}

void DBAuthProtocol::Statistics_TotalUserAgeGender(SYSTEMTIME stTime)	// 7. ��ü �̿��� ���� �� ���� ����
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// ���� �� ���̺���
	// 12������, 13~15, 16~18, 19~24, 25~29, 30~34, 35~39, 40���̻�
	SI32	siAge_Male[8];
	SI32	siAge_Female[8];
	SI32	i = 0;
	SI32	siGender;
	ZeroMemory(siAge_Male, sizeof(siAge_Male));
	ZeroMemory(siAge_Female, sizeof(siAge_Female));

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_TotalUserAgeGender.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date\t");
			fprintf(fp, "[M]<=12\t[M]13~15\t[M]16~18\t[M]19~24\t[M]25~29\t[M]30~34\t[M]35~39\t[M]>=40\t");
			fprintf(fp, "[F]<=12\t[F]13~15\t[F]16~18\t[F]19~24\t[F]25~29\t[F]30~34\t[F]35~39\t[F]>=40\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_TotalUserAgeGender" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
			m_odbc.GetData( &siGender );
			for(i=0; i<8; i++)
			{
				if(siGender == 1)
				{
					m_odbc.GetData( &siAge_Male[i],		0 );
				}
				else if(siGender == 2)
				{
					m_odbc.GetData( &siAge_Female[i],	0 );
				}
			}
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d", stTime.wYear, stTime.wMonth, stTime.wDay);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Male[i]);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Female[i]);
		fprintf(fp, "\n");

		fclose(fp);
	}
}

void DBAuthProtocol::Statistics_WeekUserAgeGender(SYSTEMTIME stTime)	// 8. �ְ� �̿��� ���� �� ���� ����
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// ���� �� ���̺���
	// 12������, 13~15, 16~18, 19~24, 25~29, 30~34, 35~39, 40���̻�
	SI32	siAge_Male[8];
	SI32	siAge_Female[8];
	SI32	i = 0;
	SI32	siGender;

	ZeroMemory(siAge_Male, sizeof(siAge_Male));
	ZeroMemory(siAge_Female, sizeof(siAge_Female));

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_WeekUserAgeGender.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date\t");
			fprintf(fp, "[M]<=12\t[M]13~15\t[M]16~18\t[M]19~24\t[M]25~29\t[M]30~34\t[M]35~39\t[M]>=40\t");
			fprintf(fp, "[F]<=12\t[F]13~15\t[F]16~18\t[F]19~24\t[F]25~29\t[F]30~34\t[F]35~39\t[F]>=40\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_WeekUserAgeGender" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
			m_odbc.GetData( &siGender );
			for(i=0; i<8; i++)
			{
				if(siGender == 1)
				{
					m_odbc.GetData( &siAge_Male[i],		0 );
				}
				else if(siGender == 2)
				{
					m_odbc.GetData( &siAge_Female[i],	0 );
				}
			}
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d", stTime.wYear, stTime.wMonth, stTime.wDay);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Male[i]);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Female[i]);
		fprintf(fp, "\n");

		fclose(fp);
	}
}

void DBAuthProtocol::Statistics_NewUserAgeGender(SYSTEMTIME stTime)		// 9. �ű� ���� ���� �� ���� ����
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// ���� �� ���̺���
	// 12������, 13~15, 16~18, 19~24, 25~29, 30~34, 35~39, 40���̻�
	SI32	siAge_Male[8];
	SI32	siAge_Female[8];
	SI32	i = 0;
	SI32	siGender;

	ZeroMemory(siAge_Male, sizeof(siAge_Male));
	ZeroMemory(siAge_Female, sizeof(siAge_Female));

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_NewUserAgeGender.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date\t");
			fprintf(fp, "[M]<=12\t[M]13~15\t[M]16~18\t[M]19~24\t[M]25~29\t[M]30~34\t[M]35~39\t[M]>=40\t");
			fprintf(fp, "[F]<=12\t[F]13~15\t[F]16~18\t[F]19~24\t[F]25~29\t[F]30~34\t[F]35~39\t[F]>=40\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_NewUserAgeGender" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
			m_odbc.GetData( &siGender );
			for(i=0; i<8; i++)
			{
				if(siGender == 1)
				{
					m_odbc.GetData( &siAge_Male[i],		0 );
				}
				else if(siGender == 2)
				{
					m_odbc.GetData( &siAge_Female[i],	0 );
				}
			}
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d", stTime.wYear, stTime.wMonth, stTime.wDay);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Male[i]);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Female[i]);
		fprintf(fp, "\n");

		fclose(fp);
	}
}
	
void DBAuthProtocol::Statistics_ActiveUserAgeGender(SYSTEMTIME stTime)	// 10. ��Ƽ�� �̿��� ���� �� ���� ����
{
	TIMESTAMP_STRUCT stTimeStamp;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &stTime);

	// ���� �� ���̺���
	// 12������, 13~15, 16~18, 19~24, 25~29, 30~34, 35~39, 40���̻�
	SI32	siAge_Male[8];
	SI32	siAge_Female[8];
	SI32	i = 0;
	SI32	siGender;

	ZeroMemory(siAge_Male, sizeof(siAge_Male));
	ZeroMemory(siAge_Female, sizeof(siAge_Female));

	TCHAR	szFileName[MAX_PATH];
	_tcscpy(szFileName, "Statistics_ActiveUserAgeGender.log");

	FILE* fp = fopen(szFileName, "r");
	if(fp == NULL)
	{
		fp = fopen(szFileName, "a+");
		if(fp)
		{
			// �÷� ����
			fprintf(fp, "Date\t");
			fprintf(fp, "[M]<=12\t[M]13~15\t[M]16~18\t[M]19~24\t[M]25~29\t[M]30~34\t[M]35~39\t[M]>=40\t");
			fprintf(fp, "[F]<=12\t[F]13~15\t[F]16~18\t[F]19~24\t[F]25~29\t[F]30~34\t[F]35~39\t[F]>=40\n");
			fclose(fp);
		}
	}

	switch(DUMPCASE)
	{
	case DUMPCASE:
		{
			BEGIN_SP( "rsp_Statistics_ActiveUserAgeGender" )
				m_odbc.SetParam( SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
			m_odbc.GetData( &siGender );
			for(i=0; i<8; i++)
			{
				if(siGender == 1)
				{
					m_odbc.GetData( &siAge_Male[i],		0 );
				}
				else if(siGender == 2)
				{
					m_odbc.GetData( &siAge_Female[i],	0 );
				}
			}
			END_FETCHDATA()
		}
		break;
	}

	fp = fopen(szFileName, "a+");

	// ������ ����
	if(fp)
	{
		fprintf(fp, "%04d-%02d-%02d", stTime.wYear, stTime.wMonth, stTime.wDay);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Male[i]);
		for(i=0; i<8; i++)		fprintf(fp, "\t%d", siAge_Female[i]);
		fprintf(fp, "\n");

		fclose(fp);
	}
}
