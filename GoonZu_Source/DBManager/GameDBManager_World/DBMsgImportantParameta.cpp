#include "DBGameProtocol.h"
//#include "DBMsg.h"
#include "DBMsg-Person.h"

void DBGameProtocol::SetLevelInfo()
{
	// 받은 패킷
	sDBRequest_SetLevelInfo *pRecvMsg = (sDBRequest_SetLevelInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetLevelInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETLEVELINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI64 siIncExp;
	SI32 siIncFameExp;
	SI16 siIncLevel, siIncBonus, siIncSkillBonus, siIncFameLevel;

	// DB Query
	BEGIN_SP( "rsp_IPLevelInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,	sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siExp,			sizeof(pRecvMsg->siExp) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siLevel,		sizeof(pRecvMsg->siLevel) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFameExp,		sizeof(pRecvMsg->siFameExp) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFameLevel,	sizeof(pRecvMsg->siFameLevel) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siBonus,		sizeof(pRecvMsg->siBonus) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSkillBonus,	sizeof(pRecvMsg->siSkillBonus) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPlayTime,	sizeof(pRecvMsg->siPlayTime) );
	END_SP()	
	
	EXEC_SQL()
	
	GMONEY itCashMoney	= 0;
	GMONEY itBankMoney	= 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,	0);
		m_odbc.GetData( &siIncExp,			0);
		m_odbc.GetData( &siIncLevel,		0);
		m_odbc.GetData( &siIncFameExp,		0);
		m_odbc.GetData( &siIncFameLevel,	0);
		m_odbc.GetData( &siIncBonus,		0);
		m_odbc.GetData( &siIncSkillBonus,	0);
		m_odbc.GetData( &itCashMoney,		0);
		m_odbc.GetData( &itBankMoney,		0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, PlaySecond = %d, Exp = %d, Level = %d, FameExp = %d, FameLevel = %d, Bonus = %d, SkillBonus = %d, BeforeExp = %d, BeforeLevel = %d, BeforeFameExp = %d, BeforeFameLevel = %d, BeforeBonus = %d, BeforeSkillBonus = %d, CashMoney = %I64d, BankMoney = %I64d"
			, sendMsg.siResult, pRecvMsg->siPlayTime, pRecvMsg->siExp, pRecvMsg->siLevel, pRecvMsg->siFameExp, pRecvMsg->siFameLevel, pRecvMsg->siBonus, pRecvMsg->siSkillBonus
			, siIncExp, siIncLevel, siIncFameExp, siIncFameLevel, siIncBonus, siIncSkillBonus
			, itCashMoney, itBankMoney
			);
		sDBLogMsg_QueryString LogMsg("Set Level Info", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetBonusInfo()
{
	// 받은 패킷
	sDBRequest_SetBonusInfo *pRecvMsg = (sDBRequest_SetBonusInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetBonusInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETBONUSINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// PCK : 로그용 보낼 패킷 추가
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.siBonus = pRecvMsg->siBonus;
	sendMsg.cIBA.Set( &pRecvMsg->clBA);

	cltBasicAbility clBA;
	SI32 siBonus = 0;

	// DB Query
	BEGIN_SP( "rsp_IPBonusInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetBasicAbility( &pRecvMsg->clBA );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siBonus, sizeof(pRecvMsg->siBonus) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		GetBasicAbility( &clBA );
		m_odbc.GetData( &siBonus, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Str = %d, Dex = %d, Vit = %d, Mag = %d, Hnd = %d, Luk = %d, Bonus = %d, BeforeStr = %d, BeforeDex = %d, BeforeVit = %d, BeforeMag = %d, BeforeHnd = %d, BeforeBonus = %d"
				, pRecvMsg->clBA.siStr, pRecvMsg->clBA.siDex, pRecvMsg->clBA.siVit, pRecvMsg->clBA.siMag, pRecvMsg->clBA.siHnd, pRecvMsg->clBA.siLuk, pRecvMsg->siBonus
				, clBA.siStr, clBA.siDex, clBA.siVit, clBA.siMag, clBA.siHnd, siBonus
				);
		sDBLogMsg_QueryString LogMsg("Set Bonus Info", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetPosLifeMana()
{
	// 받은 패킷
	sDBRequest_SetPosLifeMana *pRecvMsg = (sDBRequest_SetPosLifeMana *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetPosLifeMana sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETPOSLIFEMANA;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// PCK : 로그용 보낼 패킷 추가.
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.clPos.Set(&pRecvMsg->clPos);
	sendMsg.siLife = pRecvMsg->siLife;
	sendMsg.siMana = pRecvMsg->siMana;
	sendMsg.siPlaySecond = pRecvMsg->siPlaySecond;
	sendMsg.siPlayEventSecond = pRecvMsg->siPlayEventSecond;
	sendMsg.clLastMainMapPos.Set(&pRecvMsg->clLastMainMapPos);

	// DB Query
	BEGIN_SP( "rsp_IPPosLifeManaSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetCharPos( &pRecvMsg->clPos );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLife, sizeof(pRecvMsg->siLife) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMana, sizeof(pRecvMsg->siMana) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPlaySecond, sizeof(pRecvMsg->siPlaySecond) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPlayEventSecond, sizeof(pRecvMsg->siPlayEventSecond) );
		SetPos( &pRecvMsg->clLastMainMapPos );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MapIndex = %d, X = %d, Y = %d, Life = %d, Mana = %d, PlaySecond = %d, PlayEventSecond = %d"
				, pRecvMsg->clPos.siMapIndex, pRecvMsg->clPos.siX, pRecvMsg->clPos.siY
				, pRecvMsg->siLife, pRecvMsg->siMana, pRecvMsg->siPlaySecond, pRecvMsg->siPlayEventSecond);
		sDBLogMsg_QueryString LogMsg("Set PosLifeMana", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ChangeExp()
{
	// 받은 패킷
	sDBRequest_ChangeExp *pRecvMsg = (sDBRequest_ChangeExp *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ChangeExp sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGEEXP;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_IPExpChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siChangedExp, sizeof(pRecvMsg->siChangedExp) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siReason, 0 );
		m_odbc.GetData( &sendMsg.siChangedExp, 0 );
		m_odbc.GetData( &sendMsg.siExp, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, ChangedExp = %d, ResultExp = %d"
				, sendMsg.siReason, sendMsg.siChangedExp, sendMsg.siExp);
		sDBLogMsg_QueryString LogMsg("Exp Change", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 스텟 초기화
void DBGameProtocol::InitStatus()
{
	// 받은 패킷
	sDBRequest_InitStatus *pRecvMsg = (sDBRequest_InitStatus *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_InitStatus sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_INITSTATUS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// PCK : 로그용 보낼 패킷 추가.
	sendMsg.clItem_Log.Set(&pRecvMsg->clItem);

	// DB Query
	BEGIN_SP( "rsp_IPStatusInit" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMode, sizeof(pRecvMsg->siMode) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMode, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Mode = %d, InvPos = %d, LeftItemNum = %d"
				, sendMsg.siMode, pRecvMsg->siItemPos, sendMsg.clItem.siItemNum
			);
		sDBLogMsg_QueryString LogMsg("Init Status", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


// Person의 명성 경험치를 변경한다.
void DBGameProtocol::ChangeFameExp()
{
	// 받은 패킷
	sDBRequest_ChangeFameExp *pRecvMsg = (sDBRequest_ChangeFameExp *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ChangeFameExp sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGEFAMEEXP;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_IPChangeFameExp" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siChangedFameExp, sizeof(pRecvMsg->siChangedFameExp) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siBeforeFameExp = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siReason, 0 );
		m_odbc.GetData( &sendMsg.siChangedFameExp, 0 );
		m_odbc.GetData( &sendMsg.siFameExp, 0 );
		m_odbc.GetData( &siBeforeFameExp, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, BeforeFameExp = %d, ChangedFameExp = %d, FameExp = %d"
			, sendMsg.siReason, siBeforeFameExp, sendMsg.siChangedFameExp, sendMsg.siFameExp
			);
		sDBLogMsg_QueryString LogMsg("Change Fame Exp", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}