#include "DBGameProtocol.h"
#include "DBMsg-Mine.h"
#include "DBMsg-Structure.h"
//#include "DBMsg-Farming.h"

void DBGameProtocol::GetMineStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_MINE;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MineInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	SI32 siLastChangeDate = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		GetRentContract( &sendMsg.clStrInfo.clMineStrInfo.clRentContract);
		m_odbc.GetData( &siLastChangeDate, 0 ); sendMsg.clStrInfo.clMineStrInfo.clLastChangeDate.MakeFullDateFromDateVary(siLastChangeDate);
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney,	0);
		m_odbc.GetData( sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
	END_FETCHDATA()

	/* 건물 기본정보 */
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siStrDura, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemUnique, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemNum, 0 );
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siUseItemNum, 0 );
			m_odbc.GetData( &(sendMsg.clStrInfo.clCommonInfo.clBaseMoney.itMoney), 0);
			m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siRepairStrDura, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// 세금정보를 얻는다.
	GetTaxList( &sendMsg.clStrInfo.clCommonInfo.clTaxInfo, sendMsg.siVillageUnique, sendMsg.clStrInfo.clCommonInfo.siStrType );


	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::MineUnitManyGet()
{
	// 받은 패킷
	sDBRequest_ManyMineUnit *pRecvMsg = (sDBRequest_ManyMineUnit *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ManyMineUnit sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYMINEUNIT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MineUnitManyGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	ZeroMemory( sendMsg.clMineUnit, sizeof( cltMineUnit ) * MAX_MINEUNIT_NUMBER );

	SI32 siIndex;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &siIndex,					0);
		GetSimplePerson( &sendMsg.clMineUnit[siIndex].clPerson );
		GetRentContract( &sendMsg.clMineUnit[siIndex].clContract );
	END_FETCHDATA()

	sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::MineContractChange()
{
	// 받은 패킷
	sDBRequest_MineChangeContract *pRecvMsg = (sDBRequest_MineChangeContract *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MineChangeContract sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_CHANGECONTRACT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clContract.clDate);
	SI32 siLastChangeDate = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_MineContractChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		SetRentContract( &pRecvMsg->clContract, &clGameDate );
		m_odbc.SetParam( SQL_INTEGER, &siLastChangeDate, sizeof(siLastChangeDate) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		GetRentContract( &sendMsg.clContract );
		m_odbc.GetData( &siLastChangeDate, 0 ); sendMsg.clDate.MakeFullDateFromDateVary(siLastChangeDate);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::MineRentApply()
{
	// 받은 패킷
	sDBRequest_MineRentApply *pRecvMsg = (sDBRequest_MineRentApply *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MineRentApply sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_RENTAPPLY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clContract.clDate);

	// DB Query
	BEGIN_SP( "rsp_MineRentApply" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetRentContract( &pRecvMsg->clContract, &clGameDate);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siIndex,			0);
		GetSimplePerson( &sendMsg.clApplyer );
		GetRentContract( &sendMsg.clContract);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ApplyIndex = %d, RentFee = %I64d, RentPeriod = %d, Time = %d"
			, sendMsg.siVillageUnique, sendMsg.siIndex, sendMsg.clContract.siRentFee
			, sendMsg.clContract.siRentPeriod, sendMsg.clContract.clDate.GetHourVary() );
		sDBLogMsg_QueryString LogMsg("Mine Rent Apply", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::MineRentApplyCancel()
{
	// 받은 패킷
	sDBRequest_MineCancelRentApply *pRecvMsg = (sDBRequest_MineCancelRentApply *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MineCancelRentApply sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_CANCELRENTAPPLY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MineRentApplyCancel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siPersonID,		0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d"
			, sendMsg.siVillageUnique
			);
		sDBLogMsg_QueryString LogMsg("Mine Rent Apply Cancel", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::MineApplyerListGet()
{
	// 받은 패킷
	sDBRequest_MineApplyerList *pRecvMsg = (sDBRequest_MineApplyerList *)m_pPacket;


	// 보낼 패킷
	sDBResponse_MineApplyerList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_APPLYERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MineApplyerListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siIndex, 0);
		if(siIndex >= MAX_APPLY_PER_MINE)
			continue;
		GetRentApplyer( &sendMsg.clApplyer[siIndex]);
	END_FETCHDATA()

	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::MineApplyerDel()
{
	// 받은 패킷
	sDBRequest_MineDelApplyer *pRecvMsg = (sDBRequest_MineDelApplyer *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MineDelApplyer sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_DELAPPLYER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// [추가 : 황진성 2007. 11. 2 임차 희망자 명단을 모조리 삭제하는 것을 수정.]
	sendMsg.siDelSlotNum = pRecvMsg->siDelSlotNum;

	// DB Query
	BEGIN_SP( "rsp_MineApplyerDel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d", sendMsg.siVillageUnique);
		sDBLogMsg_QueryString LogMsg("Mine Rent Applyer Delete", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 시전신청자 삭제
void DBGameProtocol::MineRent()
{
	// 받은 패킷
	sDBRequest_MineRent *pRecvMsg = (sDBRequest_MineRent *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MineRent sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_RENT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clRentContract.clDate);

	// DB Query
	BEGIN_SP( "rsp_MineRent" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex) );
		SetRentContract( &pRecvMsg->clRentContract, &clGameDate );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetSimplePerson( &sendMsg.clPerson );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siIndex, 0 );
		GetRentContract( &sendMsg.clRentContract );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, MineUnitIndex = %d, RentFee = %I64d, RentPeriod = %d, Year = %d, Month = %d, Day = %d, Hour = %d, Minute = %d"
			, sendMsg.siVillageUnique, sendMsg.siIndex, sendMsg.clRentContract.siRentFee, sendMsg.clRentContract.siRentPeriod
			, sendMsg.clRentContract.clDate.uiYear, sendMsg.clRentContract.clDate.uiMonth, sendMsg.clRentContract.clDate.uiDay, sendMsg.clRentContract.clDate.uiHour, sendMsg.clRentContract.clDate.uiMinute
			);
		sDBLogMsg_QueryString LogMsg("Mine Rent", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::MinePayRentFee()
{
	// 받은 패킷
	sDBRequest_MinePayRentFee *pRecvMsg = (sDBRequest_MinePayRentFee *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MinePayRentFee sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_PAYRENTFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MineRentFeePay" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRentFee,			sizeof(pRecvMsg->siRentFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMineIndex,		sizeof(pRecvMsg->siMineIndex) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);

		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.clBankMoney.itMoney, 0);
		m_odbc.GetData( &sendMsg.siRentFee, 0);
		m_odbc.GetData( &sendMsg.siDelayNum, 0);
		m_odbc.GetData( &sendMsg.siDelayMoney, 0);

		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siMineIndex, 0);
		m_odbc.GetData( &sendMsg.clMineMoney.itMoney, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, MineIndex = %d, BankMoney = %I64d, RentFee = %I64d, DeleyNum = %d, DelayMoney = %I64d"
			, sendMsg.siVillageUnique, sendMsg.siMineIndex, sendMsg.clBankMoney.itMoney, sendMsg.siRentFee
			, sendMsg.siDelayNum, sendMsg.siDelayMoney );
		sDBLogMsg_QueryString LogMsg("Mine RentFee Pay", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::MineRecall()
{
	// 받은 패킷
	sDBRequest_MineRecall *pRecvMsg = (sDBRequest_MineRecall *)m_pPacket;

	// 보낼 패킷
	sDBResponse_MineRecall sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MINE_RECALL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_MineRecall" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRecallReason,	sizeof(pRecvMsg->siRecallReason) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMineIndex,		sizeof(pRecvMsg->siMineIndex) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);

		m_odbc.GetData( &sendMsg.siPersonID,		0);
		m_odbc.GetData( &sendMsg.siRecallReason,	0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siMineIndex,		0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, MineIndex = %d, RecallReason = %d"
			, sendMsg.siVillageUnique, sendMsg.siMineIndex, sendMsg.siRecallReason );
		sDBLogMsg_QueryString LogMsg("Mine Recall", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
