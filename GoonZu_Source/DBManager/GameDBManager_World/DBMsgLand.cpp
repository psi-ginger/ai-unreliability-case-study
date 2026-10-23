#include "DBGameProtocol.h"
#include "DBMsg-Land.h"
#include "DBMsg-Structure.h"
#include "DBMsg-Agriculture.h"

void DBGameProtocol::GetLandStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_LAND;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_LandInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siLastChangeDate = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		GetRentContract( &sendMsg.clStrInfo.clLandStrInfo.clRentContract);
		m_odbc.GetData( &siLastChangeDate, 0 ); sendMsg.clStrInfo.clLandStrInfo.clLastChangeDate.MakeFullDateFromDateVary(siLastChangeDate);
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

void DBGameProtocol::LandUnitManyGet()
{
	// 받은 패킷
	sDBRequest_ManyLandUnit *pRecvMsg = (sDBRequest_ManyLandUnit *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ManyLandUnit sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYLANDUNIT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_LandUnitManyGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	ZeroMemory( sendMsg.clLandUnit, sizeof( cltLandUnit ) * MAX_LANDUNIT_NUMBER );

	SI32 siIndex;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &siIndex,					0);
		GetSimplePerson( &sendMsg.clLandUnit[siIndex].clPerson );
		GetRentContract( &sendMsg.clLandUnit[siIndex].clContract );
	END_FETCHDATA()


	sendMsg.siResult = 1;
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::LandContractChange()
{
	// 받은 패킷
	sDBRequest_LandChangeContract *pRecvMsg = (sDBRequest_LandChangeContract *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_LandChangeContract sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_CHANGECONTRACT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clContract.clDate);
	SI32 siLastChangeDate = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_LandContractChange" )
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

void DBGameProtocol::LandRentApply()
{
	// 받은 패킷
	sDBRequest_LandRentApply *pRecvMsg = (sDBRequest_LandRentApply *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_LandRentApply sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_RENTAPPLY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clContract.clDate);

	// DB Query
	BEGIN_SP( "rsp_LandRentApply" )
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
		sDBLogMsg_QueryString LogMsg("Land Rent Apply", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::LandRentApplyCancel()
{
	// 받은 패킷
	sDBRequest_LandCancelRentApply *pRecvMsg = (sDBRequest_LandCancelRentApply *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_LandCancelRentApply sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_CANCELRENTAPPLY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_LandRentApplyCancel" )
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
		sDBLogMsg_QueryString LogMsg("Land Rent Apply Cancel", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::LandApplyerListGet()
{
	// 받은 패킷
	sDBRequest_LandApplyerList *pRecvMsg = (sDBRequest_LandApplyerList *)m_pPacket;
	

	// 보낼 패킷
	sDBResponse_LandApplyerList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_APPLYERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_LandApplyerListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()

	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siIndex,					0);
		if(siIndex >= MAX_APPLY_PER_LAND)
			continue;
		GetRentApplyer( &sendMsg.clApplyer[siIndex]);
	END_FETCHDATA()
	
	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::LandApplyerDel()
{
	// 받은 패킷
	sDBRequest_LandDelApplyer *pRecvMsg = (sDBRequest_LandDelApplyer *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_LandDelApplyer sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_DELAPPLYER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siDelSlotNum = pRecvMsg->siDelSlotNum;

	// DB Query
	BEGIN_SP( "rsp_LandApplyerDel" )
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
		sDBLogMsg_QueryString LogMsg("Land Rent Applyer Delete", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 시전신청자 삭제
void DBGameProtocol::LandRent()
{
	// 받은 패킷
	sDBRequest_LandRent *pRecvMsg = (sDBRequest_LandRent *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_LandRent sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_RENT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 10. 31 로그에 이용. 농장임대 허가 관련.]
	sendMsg.clRentContract.Set(&pRecvMsg->clRentContract);
	sendMsg.siIndex				= pRecvMsg->siIndex;
	sendMsg.siVillageUnique		= pRecvMsg->siVillageUnique;
	sendMsg.clPerson.siPersonID = pRecvMsg->siPersonID;

	cltGameDate clGameDate(&pRecvMsg->clRentContract.clDate);

	// DB Query
	BEGIN_SP( "rsp_LandRent" )
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
		sprintf(log, "VillageUnique = %d, LandUnitIndex = %d, RentFee = %i64d, RentPeriod = %d, Year = %d, Month = %d, Day = %d, Hour = %d, Minute = %d"
				, sendMsg.siVillageUnique, sendMsg.siIndex, sendMsg.clRentContract.siRentFee, sendMsg.clRentContract.siRentPeriod
				, sendMsg.clRentContract.clDate.uiYear, sendMsg.clRentContract.clDate.uiMonth, sendMsg.clRentContract.clDate.uiDay, sendMsg.clRentContract.clDate.uiHour, sendMsg.clRentContract.clDate.uiMinute
				);
		sDBLogMsg_QueryString LogMsg("Land Rent", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::LandPayRentFee()
{
	// 받은 패킷
	sDBRequest_LandPayRentFee *pRecvMsg = (sDBRequest_LandPayRentFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_LandPayRentFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_PAYRENTFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_LandRentFeePay" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRentFee,			sizeof(pRecvMsg->siRentFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLandIndex,		sizeof(pRecvMsg->siLandIndex) );
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
		m_odbc.GetData( &sendMsg.siLandIndex, 0);
		m_odbc.GetData( &sendMsg.clLandMoney.itMoney, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1 || sendMsg.siResult == -1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, VillageUnique = %d, LandIndex = %d, BankMoney = %I64d, RentFee = %I64d, DeleyNum = %d, DelayMoney = %I64d"
				, sendMsg.siResult, sendMsg.siVillageUnique, sendMsg.siLandIndex, sendMsg.clBankMoney.itMoney, sendMsg.siRentFee
				, sendMsg.siDelayNum, sendMsg.siDelayMoney );
		sDBLogMsg_QueryString LogMsg("Land RentFee Pay", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::LandRecall()
{
	// 받은 패킷
	sDBRequest_LandRecall *pRecvMsg = (sDBRequest_LandRecall *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_LandRecall sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_LAND_RECALL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_LandRecall" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRecallReason,	sizeof(pRecvMsg->siRecallReason) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLandIndex,		sizeof(pRecvMsg->siLandIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);

		m_odbc.GetData( &sendMsg.siPersonID,		0);
		m_odbc.GetData( &sendMsg.siRecallReason,	0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siLandIndex,		0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, LandIndex = %d, RecallReason = %d"
				, sendMsg.siVillageUnique, sendMsg.siLandIndex, sendMsg.siRecallReason );
		sDBLogMsg_QueryString LogMsg("Land Recall", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::AgriculturePayMoneyForRent()
{
	// 받은 패킷
	sDBRequest_AgriculturePayMoneyForRentLand *pRecvMsg = (sDBRequest_AgriculturePayMoneyForRentLand *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_AgriculturePayMoneyForRentLand sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_AGRICULTUREPAYMONEYFORRENTLAND;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;


	// DB Query
	BEGIN_SP( "rsp_AgriculturePayMoneyForRent" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStructType, sizeof(pRecvMsg->siStructType) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPayMoney, sizeof(pRecvMsg->siPayMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->uiLeaseExpire, sizeof(pRecvMsg->uiLeaseExpire) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.siStructType, 0);
		m_odbc.GetData( &sendMsg.siPayMoney, 0);
		m_odbc.GetData( &sendMsg.siLeftMoney, 0);
		m_odbc.GetData( &sendMsg.uiLeaseExpire, 0);
	END_FETCHDATA()
		
	sendMsg.siAgricultureItemUnique = pRecvMsg->siAgricultureItemUnique;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StructType = %d, PayMoney = %I64d, LeftMoney = %I64d, LeaseExpire = %d"
				, sendMsg.siVillageUnique, sendMsg.siStructType, sendMsg.siPayMoney, sendMsg.siLeftMoney, sendMsg.uiLeaseExpire);
		sDBLogMsg_QueryString LogMsg("Working PayMoney For Rent", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetAgricultureLeaseInfo()
{
	// 받은 패킷
	sDBRequest_GetAgricultureLeaseInfo *pRecvMsg = (sDBRequest_GetAgricultureLeaseInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetAgricultureLeaseInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETAGRICULTURELEASEINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;


	// DB Query
	BEGIN_SP( "rsp_AgricultureLeaseInfo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
	END_FETCHDATA()

	SI32 siCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if( siCount >= MAX_AGRICULTURE_COUNT )			continue;
			m_odbc.GetData( &sendMsg.siVillageUnique[siCount] );
			m_odbc.GetData( &sendMsg.uiExpire[siCount] );
			siCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ExpiredAgricultureLease()
{
	// 받은 패킷
	sDBRequest_ExpiredAgricultureLease *pRecvMsg = (sDBRequest_ExpiredAgricultureLease *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ExpiredAgricultureLease sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EXPIREDAGRICULTERLEASE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	
	sendMsg.uiExpireTime = pRecvMsg->uiExpireTime;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// DB Query
	BEGIN_SP( "rsp_AgricultureLeaseExpire" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, ""
			);
		sDBLogMsg_QueryString LogMsg("Agriculture Expire", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
