#include "DBLogMsg.h"
#include "DBMsg.h"
#include "DBMsg-Person.h"

#include "DBLogProtocol.h"
#include "DBGameProtocol.h"

#include "ITZRecvBuffer.h"
#include "ITZSendBuffer.h"

extern DBGameProtocol	g_dgp[ DBCONNECTION_NUMBER ];
extern char				g_GameServerName[ 50 ];

DBLogProtocol::DBLogProtocol()
{
	m_pSession = NULL;
	m_pPacket = NULL;
}

DBLogProtocol::~DBLogProtocol()
{
	m_odbc.Disconnect();
}

void DBLogProtocol::Init( UI16 usPort, char *szDBIP, char *szDatabase, char *szUserID, char *szUserPW, char* szProcedure )
{

	printf( "DBLogProtocol::Init(%d, %s, %s, %s, %s, %s)\n", usPort, szDBIP, szDatabase, szUserID, szUserPW, szProcedure);

	m_odbc.Connect( "SQL Server", szDBIP, szDatabase, szUserID, szUserPW );

	strcpy(m_szProcudure, szProcedure);

	m_SF.CreateFactory( GetSessionManager(), 1, 5000 * 1000, 5000 * 1000 );
	
	GetNetIOCP()->CreateIOCP( GetSessionManager(), 1 );
	
	GetListen()->CreateListen( GetSessionManager(), usPort );

	SetFPS( 15 );
}

void DBLogProtocol::Update()
{

}

void DBLogProtocol::MessageProc()
{
	if( m_pPacket == NULL ) return;

	// 패킷 커맨드에 따른 처리
	switch( m_pPacket->usCmd )
	{
	case DBLOGMSG_QUERYSTRING:
		{
			sDBLogMsg_QueryString	*pRecvMsg = (sDBLogMsg_QueryString	*)m_pPacket;
//#ifdef USE_DBGW
//			BEGIN_SP( "rsp_NHN_Log" )
//#else
			BEGIN_SP( m_szProcudure )
//#endif
				if( strcmp(m_szProcudure, "rsp_NHN_Log") == 0)
				{
					m_odbc.SetParam( SQL_VARCHAR, g_GameServerName, sizeof(g_GameServerName) );
				}
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szTime, sizeof(pRecvMsg->szTime) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCommand, sizeof(pRecvMsg->szCommand) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDstPersonID, sizeof(pRecvMsg->siDstPersonID) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szQuery, sizeof(pRecvMsg->szQuery) );
			END_SP()

			EXEC_SQL()

			/*
#ifdef NHN_ALPHA
			sDBLogMsg_QueryString	*pRecvMsg = (sDBLogMsg_QueryString	*)m_pPacket;
			BEGIN_SP( "rsp_NHN_Log" )
				m_odbc.SetParam( SQL_VARCHAR, g_GameServerName, sizeof(g_GameServerName) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szTime, sizeof(pRecvMsg->szTime) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCommand, sizeof(pRecvMsg->szCommand) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDstPersonID, sizeof(pRecvMsg->siDstPersonID) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szQuery, sizeof(pRecvMsg->szQuery) );
			END_SP()

			EXEC_SQL()
#else
			sDBLogMsg_QueryString	*pRecvMsg = (sDBLogMsg_QueryString	*)m_pPacket;
			BEGIN_SP( "rsp_New_Log" )
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szTime, sizeof(pRecvMsg->szTime) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCommand, sizeof(pRecvMsg->szCommand) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDstPersonID, sizeof(pRecvMsg->siDstPersonID) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szQuery, sizeof(pRecvMsg->szQuery) );
			END_SP()

			EXEC_SQL()
#endif
			*/
		}
		break;

	case DBLOGMSG_REQUEST_PCROOM_LOG:
		{
			sDBRequest_PCRoom_Log	*pRecvMsg = (sDBRequest_PCRoom_Log	*)m_pPacket;

			CHAR szServerID[20];
			CHAR szLoginTime[25];
			CHAR szLogoutTime[25];
			sprintf(szServerID, "GoonzuSpecial_%02d", pRecvMsg->siServerID);
			sprintf(szLoginTime, "%d/%02d/%02d %02d:%02d:%02d.%03d", pRecvMsg->sTime_Login.wYear, pRecvMsg->sTime_Login.wMonth, pRecvMsg->sTime_Login.wDay, pRecvMsg->sTime_Login.wHour, pRecvMsg->sTime_Login.wMinute, pRecvMsg->sTime_Login.wSecond, pRecvMsg->sTime_Login.wMilliseconds);
			sprintf(szLogoutTime, "%d/%02d/%02d %02d:%02d:%02d.%03d", pRecvMsg->sTime_LogOut.wYear, pRecvMsg->sTime_LogOut.wMonth, pRecvMsg->sTime_LogOut.wDay, pRecvMsg->sTime_LogOut.wHour, pRecvMsg->sTime_LogOut.wMinute, pRecvMsg->sTime_LogOut.wSecond, pRecvMsg->sTime_LogOut.wMilliseconds);
			CHAR szPCRoomGrade[2];
			szPCRoomGrade[0] = pRecvMsg->sPCRoomGrade;
			szPCRoomGrade[1] = 0;

			BEGIN_SP( "rsp_PCRoom_Log" )
				m_odbc.SetParam( SQL_VARCHAR, szServerID, sizeof(szServerID) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGameID, sizeof(pRecvMsg->szGameID) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szAccountID, sizeof(pRecvMsg->szAccountID) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharName, sizeof(pRecvMsg->szCharName) );
				m_odbc.SetParam( SQL_VARCHAR, szLoginTime, sizeof(szLoginTime) );
				m_odbc.SetParam( SQL_VARCHAR, szLogoutTime, sizeof(szLogoutTime) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szIP, sizeof(pRecvMsg->szIP) );
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCRMcode, sizeof(pRecvMsg->szCRMcode) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLevel_LogIn, sizeof(pRecvMsg->siLevel_LogIn) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLevel_LogOut, sizeof(pRecvMsg->siLevel_LogOut) );
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPlayMinute, sizeof(pRecvMsg->siPlayMinute) );
				m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siGameMoney, sizeof(pRecvMsg->siGameMoney) );
				m_odbc.SetParam( SQL_VARCHAR, szPCRoomGrade, sizeof(szPCRoomGrade) );
			END_SP()

			EXEC_SQL()
		}
		break;

	case DBLOGMSG_REQUEST_SETHAPPYBEANCLEARINFO_LOG:
		{
			sDBRequest_Log_SetHappyBeanClearInfo *pRecvMsg = (sDBRequest_Log_SetHappyBeanClearInfo *)m_pPacket;

			// 보낼 패킷
			sDBResponse_GetHappyBeanClearInfo sendMsg;

			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETHAPPYBEANQUESTINFO;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.siPersonID			= pRecvMsg->siPersonID;

			SI32 siResult = 0;

			// DB Query
			BEGIN_SP( "rasp_QuestStartCount" )
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szAccountID, sizeof(pRecvMsg->szAccountID));
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szServerName, sizeof(pRecvMsg->szServerName));
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharName, sizeof(pRecvMsg->szCharName));
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siQuestType, sizeof(pRecvMsg->siQuestType));
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siQuestState, sizeof(pRecvMsg->siQuestState));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
				m_odbc.GetData( &siResult );
				m_odbc.GetData( &sendMsg.siQuestType );
				m_odbc.GetData( &sendMsg.siQuest_StartCount );
				m_odbc.GetData( &sendMsg.siQuest_CompleteCount );
			END_FETCHDATA()

			// send
			//KHY - 0822 - 해피빈 퀘스트 추가. 
			if(pRecvMsg->siQuestState == QUEST_HAPPYBEAN_STEP_START) // NON이나 COMPLETE일때는 보내서는 안됩니다.
				g_dgp[3].SendPacket( (BYTE *)&sendMsg );

		}
		break;
	case DBLOGMSG_REQUEST_SETEVENTCOUPONINFO_LOG:
		{
			sDBRequest_Log_SetEventCouponInfo *pRecvMsg = (sDBRequest_Log_SetEventCouponInfo *)m_pPacket;

			// 보낼 패킷
			sDBResponse_GetEventCouponInfo sendMsg;

			sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETEVENTCOUPONINFO;
			sendMsg.packetHeader.usSize = sizeof( sendMsg );
			sendMsg.siPersonID			= pRecvMsg->siPersonID;
			sendMsg.siEventType			= pRecvMsg->siEventType;



			SI32 siResult = 0;

			// DB Query
			BEGIN_SP( "rsp_CouponEventSet" )
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siEventType, sizeof(pRecvMsg->siEventType));
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szAccountID, sizeof(pRecvMsg->szAccountID));
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szServerName, sizeof(pRecvMsg->szServerName));
				m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharName, sizeof(pRecvMsg->szCharName));
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPara, sizeof(pRecvMsg->siPara));
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.siResult, sizeof(sendMsg.siResult) );
			END_FETCHDATA()

			// send
			g_dgp[3].SendPacket( (BYTE *)&sendMsg );
		}
		break;

	case DBLOGMSG_REQUEST_NEWLOG:
		{
			sDBLogMsg_NewLog	*pRecvMsg = (sDBLogMsg_NewLog*)m_pPacket;

			TIMESTAMP_STRUCT stTimeStamp;
			g_dgp[0].Convert_TIMESTAMP_from_SYSTEMTIME(&stTimeStamp, &pRecvMsg->m_stTime);

			bool bUseItemNull = true;
			bool bUseSummonNull = true;
			bool bUseHealthInfoNull = true;
			bool bUseSIParam1Null = true;
			bool bUseSIParam2Null = true;
			bool bUseSIParam3Null = true;
			bool bUseSIParam4Null = true;
			bool bUseSIParam5Null = true;
			bool bUseCharParam1Null = true;
			bool bUseCharParam2Null = true;

			if((pRecvMsg->m_siUseLog & NEWLOG_USE_ITEM) == NEWLOG_USE_ITEM)
			{
				bUseItemNull = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_SUMMON) == NEWLOG_USE_SUMMON)
			{
				bUseSummonNull = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_HEALTHINFO) == NEWLOG_USE_HEALTHINFO)
			{
				bUseHealthInfoNull = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_SIPARAM1) == NEWLOG_USE_SIPARAM1)
			{
				bUseSIParam1Null = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_SIPARAM2) == NEWLOG_USE_SIPARAM2)
			{
				bUseSIParam2Null = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_SIPARAM3) == NEWLOG_USE_SIPARAM3)
			{
				bUseSIParam3Null = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_SIPARAM4) == NEWLOG_USE_SIPARAM4)
			{
				bUseSIParam4Null = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_SIPARAM5) == NEWLOG_USE_SIPARAM5)
			{
				bUseSIParam5Null = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_CHARPARAM1) == NEWLOG_USE_CHARPARAM1)
			{
				bUseCharParam1Null = false;
			}
			if((pRecvMsg->m_siUseLog & NEWLOG_USE_CHARPARAM2) == NEWLOG_USE_CHARPARAM2)
			{
				bUseCharParam2Null = false;
			}

			BEGIN_SP("rsp_Log4")
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siUseLog, sizeof(pRecvMsg->m_siUseLog));
				m_odbc.SetParam(SQL_VARCHAR, (void*)pRecvMsg->m_szServerName, sizeof(pRecvMsg->m_szServerName));
				m_odbc.SetParam(SQL_TIMESTAMP, &stTimeStamp, sizeof(stTimeStamp));
				m_odbc.SetParam(SQL_VARCHAR, (void*)pRecvMsg->m_szIP, sizeof(pRecvMsg->m_szIP));
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_siCommandType, sizeof(pRecvMsg->m_siCommandType));
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_siCommandIndex, sizeof(pRecvMsg->m_siCommandIndex));
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_uiReason, sizeof(pRecvMsg->m_uiReason));
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siIDNum, sizeof(pRecvMsg->m_siIDNum));
				m_odbc.SetParam(SQL_VARCHAR, pRecvMsg->m_szPersonName, sizeof(pRecvMsg->m_szPersonName));
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siLevel, sizeof(pRecvMsg->m_siLevel));
				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siExp, sizeof(pRecvMsg->m_siExp));
				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siMoney, sizeof(pRecvMsg->m_siMoney));
				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siMoneySum, sizeof(pRecvMsg->m_siMoneySum));
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clItem.siUnique, sizeof(pRecvMsg->m_clItem.siUnique), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clItem.siItemNum, sizeof(pRecvMsg->m_clItem.siItemNum), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_BINARY, &pRecvMsg->m_clItem.cData8, sizeof(pRecvMsg->m_clItem.cData8), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clItem.uiDateYear, sizeof(pRecvMsg->m_clItem.uiDateYear), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clItem.uiDateMonth, sizeof(pRecvMsg->m_clItem.uiDateMonth), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clItem.uiDateDay, sizeof(pRecvMsg->m_clItem.uiDateDay), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clItem.uiDateUseDay, sizeof(pRecvMsg->m_clItem.uiDateUseDay), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clItem.siMakerPersonID, sizeof(pRecvMsg->m_clItem.siMakerPersonID), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clItem.Element.siType, sizeof(pRecvMsg->m_clItem.Element.siType), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clItem.Element.siPower, sizeof(pRecvMsg->m_clItem.Element.siPower), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clItem.siSID, sizeof(pRecvMsg->m_clItem.siSID), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam( SQL_SMALLINT, (void*)&(pRecvMsg->m_clItem.siPalette), sizeof(pRecvMsg->m_clItem.siPalette), SQL_PARAM_INPUT, bUseItemNull );
				m_odbc.SetParam( SQL_TINYINT, (void*)&(pRecvMsg->m_clItem.siUseCrystalNum), sizeof(pRecvMsg->m_clItem.siUseCrystalNum), SQL_PARAM_INPUT, bUseItemNull );
				for(SI32 i = 0; i < MAX_CRYSTAL_NUM; i++)
				{
					m_odbc.SetParam( SQL_SMALLINT, (void*)&(pRecvMsg->m_clItem.cltItemCrytal[i].siCrystalType), sizeof(pRecvMsg->m_clItem.cltItemCrytal[i].siCrystalType), SQL_PARAM_INPUT, bUseItemNull );
					m_odbc.SetParam( SQL_SMALLINT, (void*)&(pRecvMsg->m_clItem.cltItemCrytal[i].siDurability), sizeof(pRecvMsg->m_clItem.cltItemCrytal[i].siDurability), SQL_PARAM_INPUT, bUseItemNull );
					m_odbc.SetParam( SQL_SMALLINT, (void*)&(pRecvMsg->m_clItem.cltItemCrytal[i].siMaxDurability), sizeof(pRecvMsg->m_clItem.cltItemCrytal[i].siMaxDurability), SQL_PARAM_INPUT, bUseItemNull );
					m_odbc.SetParam( SQL_TINYINT, (void*)&(pRecvMsg->m_clItem.cltItemCrytal[i].uiCrystalAbility), sizeof(pRecvMsg->m_clItem.cltItemCrytal[i].uiCrystalAbility), SQL_PARAM_INPUT, bUseItemNull );
					m_odbc.SetParam( SQL_TINYINT, (void*)&(pRecvMsg->m_clItem.cltItemCrytal[i].uiCrystalAbilityNum), sizeof(pRecvMsg->m_clItem.cltItemCrytal[i].uiCrystalAbilityNum), SQL_PARAM_INPUT, bUseItemNull );
				}
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siItemSum, sizeof(pRecvMsg->m_siItemSum));

				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.siSummonID, sizeof(pRecvMsg->m_clSummon.siSummonID), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_SMALLINT,  &pRecvMsg->m_clSummon.siKind, sizeof(pRecvMsg->m_clSummon.siKind), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_VARCHAR,  pRecvMsg->m_clSummon.szName, sizeof(pRecvMsg->m_clSummon.szName), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clSummon.siStatus, sizeof(pRecvMsg->m_clSummon.siStatus), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clSummon.siAttackType, sizeof(pRecvMsg->m_clSummon.siAttackType), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clSummon.siIdentity, sizeof(pRecvMsg->m_clSummon.siIdentity), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.clIP.siStr, sizeof(pRecvMsg->m_clSummon.clIP.siStr), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.clIP.siDex, sizeof(pRecvMsg->m_clSummon.clIP.siDex), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.clIP.siVit, sizeof(pRecvMsg->m_clSummon.clIP.siVit), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.clIP.siMag, sizeof(pRecvMsg->m_clSummon.clIP.siMag), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.clIP.siLuk, sizeof(pRecvMsg->m_clSummon.clIP.siLuk), SQL_PARAM_INPUT, bUseSummonNull );

				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_clSummon.clIP.siExp, sizeof(pRecvMsg->m_clSummon.clIP.siExp), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clSummon.clIP.siLevel, sizeof(pRecvMsg->m_clSummon.clIP.siLevel), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clSummon.clIP.siBonus, sizeof(pRecvMsg->m_clSummon.clIP.siBonus), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.clIP.Life, sizeof(pRecvMsg->m_clSummon.clIP.Life), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.clIP.Mana, sizeof(pRecvMsg->m_clSummon.clIP.Mana), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clSummon.clScroll.siUnique[0], sizeof(pRecvMsg->m_clSummon.clScroll.siUnique[0]), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clSummon.clScroll.siPercent[0], sizeof(pRecvMsg->m_clSummon.clScroll.siPercent[0]), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clSummon.clScroll.siUnique[1], sizeof(pRecvMsg->m_clSummon.clScroll.siUnique[1]), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clSummon.clScroll.siPercent[1], sizeof(pRecvMsg->m_clSummon.clScroll.siPercent[1]), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_clSummon.siTakeInMinute, sizeof(pRecvMsg->m_clSummon.siTakeInMinute), SQL_PARAM_INPUT, bUseSummonNull );
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siMonKind, sizeof(pRecvMsg->m_siMonKind));
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siTargetPersonID, sizeof(pRecvMsg->m_siTargetPersonID));
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siVillageUnique, sizeof(pRecvMsg->m_siVillageUnique));
				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_siNpcKind, sizeof(pRecvMsg->m_siNpcKind));

				m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_clHealthInfo.siHungry, sizeof(pRecvMsg->m_clHealthInfo.siHungry), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.siInnerOrg1, sizeof(pRecvMsg->m_clHealthInfo.siInnerOrg1), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.siInnerOrg2, sizeof(pRecvMsg->m_clHealthInfo.siInnerOrg2), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.siInnerOrg3, sizeof(pRecvMsg->m_clHealthInfo.siInnerOrg3), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.siInnerOrg4, sizeof(pRecvMsg->m_clHealthInfo.siInnerOrg4), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.uiOrg1Disease, sizeof(pRecvMsg->m_clHealthInfo.uiOrg1Disease), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.uiOrg2Disease, sizeof(pRecvMsg->m_clHealthInfo.uiOrg2Disease), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.uiOrg3Disease, sizeof(pRecvMsg->m_clHealthInfo.uiOrg3Disease), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.uiOrg4Disease, sizeof(pRecvMsg->m_clHealthInfo.uiOrg4Disease), SQL_PARAM_INPUT, bUseHealthInfoNull );
				m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_clHealthInfo.uiHealthPoint, sizeof(pRecvMsg->m_clHealthInfo.uiHealthPoint), SQL_PARAM_INPUT, bUseHealthInfoNull );

				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siRPPoint, sizeof(pRecvMsg->m_siRPPoint));
				m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->m_siFameExp, sizeof(pRecvMsg->m_siFameExp));

				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siParam1, sizeof(pRecvMsg->m_siParam1), SQL_PARAM_INPUT, bUseSIParam1Null);
				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siParam2, sizeof(pRecvMsg->m_siParam2), SQL_PARAM_INPUT, bUseSIParam2Null);
				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siParam3, sizeof(pRecvMsg->m_siParam3), SQL_PARAM_INPUT, bUseSIParam3Null);
				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siParam4, sizeof(pRecvMsg->m_siParam4), SQL_PARAM_INPUT, bUseSIParam4Null);
				m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siParam5, sizeof(pRecvMsg->m_siParam5), SQL_PARAM_INPUT, bUseSIParam5Null);
				m_odbc.SetParam(SQL_VARCHAR, (void*)pRecvMsg->m_szCharParam1, sizeof(pRecvMsg->m_szCharParam1), SQL_PARAM_INPUT, bUseCharParam1Null);
				m_odbc.SetParam(SQL_VARCHAR, (void*)pRecvMsg->m_szCharParam2, sizeof(pRecvMsg->m_szCharParam2), SQL_PARAM_INPUT, bUseCharParam2Null);
			END_SP()

			EXEC_SQL()
		}
		break;
	}

	return;
}

void DBLogProtocol::SessionProc( ITZSession *pSession )
{
	m_pSession = pSession;

	SI32 count = pSession->GetRecvBuffer()->GetPacketCount();

	// 현재 버퍼에 있는 패킷 개수 만큼 메시지 처리 루틴 반복 
	for( int i = 0; i < count; ++i ) 
	{
		// 버퍼에서 패킷을 하나 꺼내온다 
		m_pPacket = (sPacketHeader *)( pSession->GetRecvBuffer()->GetFirstPacket() );

		if( m_pPacket == NULL ) return;	// 패킷이 없으면 나간다

		MessageProc();

		if(!m_odbc.GetReconnectStat())
			pSession->GetRecvBuffer()->RemoveFirstPacket();

	}

}

void DBLogProtocol::SendPacket( BYTE *pPacket )
{
	m_pSession->GetSendBuffer()->Write( pPacket );
}
