#include "DBGameProtocol.h"
#include "DBMsg-Person.h"
#include "DBMsg-Item.h"
#include "DBMsg-Summon.h"

void DBGameProtocol::SetSummon()
{
	// 받은 패킷
	sDBRequest_SetSummon *pRecvMsg = (sDBRequest_SetSummon *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetSummon sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSUMMON;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siSummonIndex = pRecvMsg->siSummonIndex;

	//[추가 : 황진성 2007. 10. 26 로그에 이용. 소환수 풀어줄때 사용할 정보.]
	sendMsg.clFreeSummon.Set(&pRecvMsg->clSummon);

	// DB Query
	BEGIN_SP( "rsp_SummonSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siSummonIndex, sizeof(pRecvMsg->siSummonIndex) );
		SetSummon( &pRecvMsg->clSummon );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siReason, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siSummonIndex );
		GetSummon( &sendMsg.clSummon );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	if(pRecvMsg->siReason == 1)
	{
		sendMsg.clSimpleSummon.siKind = sendMsg.clSummon.siKind;
		sendMsg.clSimpleSummon.siLevel = sendMsg.clSummon.clIP.siLevel;
		sendMsg.clSimpleSummon.siSummonID = sendMsg.clSummon.siSummonID;
		MStrCpy(sendMsg.clSimpleSummon.szName, sendMsg.clSummon.szName, sizeof(sendMsg.clSimpleSummon.szName));
	}

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, SummonID = %d, Kind = %d, Name = %s, Status = %d, AttackType = %d, Identity = %d, Str/Dex/Vit/Mag = %d/%d/%d/%d, Life/Mana = %d/%d, Exp/Level/Bonus = %d/%d/%d, Scroll1 = %d/%d, Scroll2 = %d/%d, TakeIn = %d"
				, sendMsg.siReason, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName, sendMsg.clSummon.siStatus
				, sendMsg.clSummon.siAttackType, sendMsg.clSummon.siIdentity
				, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
				, sendMsg.clSummon.clIP.Life, sendMsg.clSummon.clIP.Mana
				, sendMsg.clSummon.clIP.siExp, sendMsg.clSummon.clIP.siLevel, sendMsg.clSummon.clIP.siBonus
				, sendMsg.clSummon.clScroll.siUnique[0], sendMsg.clSummon.clScroll.siPercent[0]
				, sendMsg.clSummon.clScroll.siUnique[1], sendMsg.clSummon.clScroll.siPercent[1]
				, sendMsg.clSummon.siTakeInMinute
				);
		sDBLogMsg_QueryString LogMsg("Summon Set", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::SummonRevive()
{
	// 받은 패킷
	sDBRequest_SummonRevive *pRecvMsg = (sDBRequest_SummonRevive *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonRevive sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONREVIVE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.clItem.Set( &pRecvMsg->clItem );

	// DB Query
	BEGIN_SP( "rsp_SummonRevive" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
		SetSummon( &pRecvMsg->clSummon );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemNum, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		GetSummon( &sendMsg.clSummon );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ItemPos = %d, Amount = %d, SummonID = %d, Kind = %d, Name = %s, Status = %d, AttackType = %d, Identity = %d, Str/Dex/Vit/Mag = %d/%d/%d/%d, Life/Mana = %d/%d, Exp/Level/Bonus = %d/%d/%d, Scroll1 = %d/%d, Scroll2 = %d/%d, TakeIn = %d"
				, pRecvMsg->siVillageUnique, pRecvMsg->siItemPos, pRecvMsg->siItemNum, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName, sendMsg.clSummon.siStatus
				, sendMsg.clSummon.siAttackType, sendMsg.clSummon.siIdentity
				, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
				, sendMsg.clSummon.clIP.Life, sendMsg.clSummon.clIP.Mana
				, sendMsg.clSummon.clIP.siExp, sendMsg.clSummon.clIP.siLevel, sendMsg.clSummon.clIP.siBonus
				, sendMsg.clSummon.clScroll.siUnique[0], sendMsg.clSummon.clScroll.siPercent[0]
				, sendMsg.clSummon.clScroll.siUnique[1], sendMsg.clSummon.clScroll.siPercent[1]
				, sendMsg.clSummon.siTakeInMinute
				);
		sDBLogMsg_QueryString LogMsg("Summon Revive", pRecvMsg->siPersonID, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonEvolve()
{
	// 받은 패킷
	sDBRequest_SummonEvolve *pRecvMsg = (sDBRequest_SummonEvolve *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonEvolve sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONEVOLVE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.clItem_Log.Set( &pRecvMsg->clItem);
	sendMsg.siSummonKind = pRecvMsg->siSummonKind;

	// DB Query
	BEGIN_SP( "rsp_SummonEvolve" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, 	&pRecvMsg->siItemPos,		sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, 	&pRecvMsg->siItemUnique,	sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, 	&pRecvMsg->siItemNum,		sizeof(pRecvMsg->siItemNum) );
		SetSummon( &pRecvMsg->clSummon );
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->bQuestEvolve,	sizeof(pRecvMsg->bQuestEvolve) );
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->bItemUse,		sizeof(pRecvMsg->bItemUse) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0 );
		m_odbc.GetData( &sendMsg.siPersonID,		0 );
		m_odbc.GetData( &sendMsg.siVillageUnique,	0 );
		m_odbc.GetData( &sendMsg.siItemNum,			0 );
		m_odbc.GetData( &sendMsg.siItemPos,			0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		GetSummon( &sendMsg.clSummon );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.bQuestEvolve		= pRecvMsg->bQuestEvolve;
	sendMsg.bItemUse			= pRecvMsg->bItemUse;
	sendMsg.siUsedItemUnique	= pRecvMsg->siItemUnique;
	sendMsg.siMode				= pRecvMsg->siMode;			// [영훈] 진화모드(진화,퇴화,변환) 값 입력 - 2008.01.28

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Mode = %d, VillageUnique = %d, ItemPos = %d, Amount = %d, SummonID = %d, Kind = %d, Name = %s, Status = %d, AttackType = %d, Identity = %d, Str/Dex/Vit/Mag = %d/%d/%d/%d, Life/Mana = %d/%d, Exp/Level/Bonus = %d/%d/%d, Scroll1 = %d/%d, Scroll2 = %d/%d, TakeIn = %d"
				, pRecvMsg->siMode, pRecvMsg->siVillageUnique, pRecvMsg->siItemPos, pRecvMsg->siItemNum, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName, sendMsg.clSummon.siStatus
				, sendMsg.clSummon.siAttackType, sendMsg.clSummon.siIdentity
				, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
				, sendMsg.clSummon.clIP.Life, sendMsg.clSummon.clIP.Mana
				, sendMsg.clSummon.clIP.siExp, sendMsg.clSummon.clIP.siLevel, sendMsg.clSummon.clIP.siBonus
				, sendMsg.clSummon.clScroll.siUnique[0], sendMsg.clSummon.clScroll.siPercent[0]
				, sendMsg.clSummon.clScroll.siUnique[1], sendMsg.clSummon.clScroll.siPercent[1]
				, sendMsg.clSummon.siTakeInMinute
				);
		sDBLogMsg_QueryString LogMsg("Summon Evolve", pRecvMsg->siPersonID, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonScroll()
{
	// 받은 패킷
	sDBRequest_SummonScroll *pRecvMsg = (sDBRequest_SummonScroll *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SummonScroll sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONSCROLL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_SummonScroll" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
		SetSummon( &pRecvMsg->clSummon );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemNum, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		GetSummon( &sendMsg.clSummon );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ItemPos = %d, Amount = %d, SummonID = %d, Kind = %d, Name = %s, Status = %d, AttackType = %d, Identity = %d, Str/Dex/Vit/Mag = %d/%d/%d/%d, Life/Mana = %d/%d, Exp/Level/Bonus = %d/%d/%d, Scroll1 = %d/%d, Scroll2 = %d/%d, TakeIn = %d"
				, pRecvMsg->siVillageUnique, pRecvMsg->siItemPos, pRecvMsg->siItemNum, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName, sendMsg.clSummon.siStatus
				, sendMsg.clSummon.siAttackType, sendMsg.clSummon.siIdentity
				, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
				, sendMsg.clSummon.clIP.Life, sendMsg.clSummon.clIP.Mana
				, sendMsg.clSummon.clIP.siExp, sendMsg.clSummon.clIP.siLevel, sendMsg.clSummon.clIP.siBonus
				, sendMsg.clSummon.clScroll.siUnique[0], sendMsg.clSummon.clScroll.siPercent[0]
				, sendMsg.clSummon.clScroll.siUnique[1], sendMsg.clSummon.clScroll.siPercent[1]
				, sendMsg.clSummon.siTakeInMinute
				);
		sDBLogMsg_QueryString LogMsg("Summon Scroll", pRecvMsg->siPersonID, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonStatusInit()
{
	// 받은 패킷
	sDBRequest_Summon_StatusInit *pRecvMsg = (sDBRequest_Summon_StatusInit *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Summon_StatusInit sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMON_STATUSINIT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.clItem_Log.Set(&pRecvMsg->clItem);

	// DB Query
	BEGIN_SP( "rsp_SummonStatusInit" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
		SetSummon( &pRecvMsg->clSummon );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetSummon( &sendMsg.clSummon );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ItemPos = %d, Amount = %d, SummonID = %d, Kind = %d, Name = %s, Status = %d, AttackType = %d, Identity = %d, Str/Dex/Vit/Mag = %d/%d/%d/%d, Life/Mana = %d/%d, Exp/Level/Bonus = %d/%d/%d, Scroll1 = %d/%d, Scroll2 = %d/%d, TakeIn = %d"
			, pRecvMsg->siItemPos, pRecvMsg->clItem.siItemNum, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName, sendMsg.clSummon.siStatus
			, sendMsg.clSummon.siAttackType, sendMsg.clSummon.siIdentity
			, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
			, sendMsg.clSummon.clIP.Life, sendMsg.clSummon.clIP.Mana
			, sendMsg.clSummon.clIP.siExp, sendMsg.clSummon.clIP.siLevel, sendMsg.clSummon.clIP.siBonus
			, sendMsg.clSummon.clScroll.siUnique[0], sendMsg.clSummon.clScroll.siPercent[0]
			, sendMsg.clSummon.clScroll.siUnique[1], sendMsg.clSummon.clScroll.siPercent[1]
			, sendMsg.clSummon.siTakeInMinute
			);
			sDBLogMsg_QueryString LogMsg("Summon StatusInit", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonSetAutoHeal()
{
	// 받은 패킷
	sDBRequest_Summon_SetAutoHeal *pRecvMsg = (sDBRequest_Summon_SetAutoHeal *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Summon_SetAutoHeal sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMON_SETAUTOHEAL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_SummonSetAutoHeal" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSelectAutoHeal, sizeof(pRecvMsg->siSelectAutoHeal) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bAutoHeal, sizeof(pRecvMsg->bAutoHeal) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siSelectAutoHeal, 0 );
		m_odbc.GetData( &sendMsg.bAutoHeal, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		SI32 persent = 0;
		if( 0 == sendMsg.siSelectAutoHeal )
			persent = 30;
		else if( 1 == sendMsg.siSelectAutoHeal )
			persent = 50;
		else if( 2 == sendMsg.siSelectAutoHeal )
			persent = 70;

		char log[512] = "";
		sprintf(log, "Limit = %d%%, AutoHeal = %d"
			, persent, sendMsg.bAutoHeal
			);
			sDBLogMsg_QueryString LogMsg("Summon Set AutoHeal", pRecvMsg->siPersonID, NULL, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonTrade()
{
	// 받은 패킷
	sDBRequest_SummonTrade *pRecvMsg = (sDBRequest_SummonTrade *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SummonTrade sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONTRADE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI64 siBeforeMoney = 0;

	// DB Query
	BEGIN_SP( "rsp_SummonTrade" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetSummon( &pRecvMsg->clSummon );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &siBeforeMoney, 0 );
	END_FETCHDATA()

	memcpy(&sendMsg.clSummon, &pRecvMsg->clSummon, sizeof(pRecvMsg->clSummon) );

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "SummonID = %d, Kind = %d, Name = %s, Status = %d, AttackType = %d, Identity = %d, Str/Dex/Vit/Mag = %d/%d/%d/%d, Life/Mana = %d/%d, Exp/Level/Bonus = %d/%d/%d, Scroll1 = %d/%d, Scroll2 = %d/%d, Price = %I64d, LeftMoney = %I64d, BeforeMoney = %I64d, TakeIn = %d"
			, sendMsg.clSummon.siSummonID, sendMsg.clSummon.siKind, sendMsg.clSummon.szName, sendMsg.clSummon.siStatus
			, sendMsg.clSummon.siAttackType, sendMsg.clSummon.siIdentity
			, sendMsg.clSummon.clIP.siStr, sendMsg.clSummon.clIP.siDex, sendMsg.clSummon.clIP.siVit, sendMsg.clSummon.clIP.siMag
			, sendMsg.clSummon.clIP.Life, sendMsg.clSummon.clIP.Mana
			, sendMsg.clSummon.clIP.siExp, sendMsg.clSummon.clIP.siLevel, sendMsg.clSummon.clIP.siBonus
			, sendMsg.clSummon.clScroll.siUnique[0], sendMsg.clSummon.clScroll.siPercent[0]
			, sendMsg.clSummon.clScroll.siUnique[1], sendMsg.clSummon.clScroll.siPercent[1]
			, sendMsg.clSummon.siTakeInMinute
			, sendMsg.siMoney, sendMsg.siLeftMoney, siBeforeMoney
			);
		sDBLogMsg_QueryString LogMsg("Summon Trade", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonSlotOut()
{
	// 받은 패킷
	sDBRequest_SummonSlotOut *pRecvMsg = (sDBRequest_SummonSlotOut *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SummonSlotOut sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONSLOTOUT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siSummonIndex = pRecvMsg->siSlotIndex;

	// DB Query
	BEGIN_SP( "rsp_SummonSlotOut" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSummonID, sizeof(pRecvMsg->siSummonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SummonSlotIn()
{
	// 받은 패킷
	sDBRequest_SummonSlotIn *pRecvMsg = (sDBRequest_SummonSlotIn *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SummonSlotIn sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SUMMONSLOTIN;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI16 siSummonItemPos = 0;

	// DB Query
	BEGIN_SP( "rsp_SummonSlotIn" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSummonID, sizeof(pRecvMsg->siSummonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siSummonIndex, sizeof(pRecvMsg->siSummonIndex) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siSummonIndex, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		GetSummon( &sendMsg.clSummon );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siSummonItemPos );
		if(siSummonItemPos >= MAX_SHAPE_ITEM) continue;
		GetItemData( &sendMsg.clSummonItemInfo.clEquipItem[siSummonItemPos] );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
