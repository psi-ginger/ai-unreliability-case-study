#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-House.h"

void DBGameProtocol::GetHouseStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_HOUSE;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HouseInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siLastChangeDate = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		GetRentContract( &sendMsg.clStrInfo.clHouseStrInfo.clRentContract);
		m_odbc.GetData( &siLastChangeDate, 0 ); sendMsg.clStrInfo.clHouseStrInfo.clLastChangeDate.MakeFullDateFromDateVary(siLastChangeDate);
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

void DBGameProtocol::ChangeHouseContract()
{
	// 받은 패킷
	sDBRequest_ChangeHouseContract *pRecvMsg = (sDBRequest_ChangeHouseContract *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ChangeHouseContract sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGEHOUSECONTRACT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clContract.clDate);
	SI32 siLastChangeDate = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_HouseContractChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	SetRentContract( &pRecvMsg->clContract, &clGameDate);
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

void DBGameProtocol::ManyHouseUnit()
{
	// 받은 패킷
	sDBRequest_ManyHouseUnit *pRecvMsg = (sDBRequest_ManyHouseUnit *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ManyHouseUnit sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MANYHOUSEUNIT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

#define MANYHOUSEUNIT_MULTIPLIER 8

	cltHouseUnit clHouseUnit[MANYHOUSEUNIT_NUMBER*MANYHOUSEUNIT_MULTIPLIER];
	ZeroMemory(clHouseUnit, sizeof(clHouseUnit));
	
	// DB Query
	BEGIN_SP( "rsp_HouseUnitManyGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siIndex;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &siIndex,					0);
		if(siIndex >= MANYHOUSEUNIT_NUMBER*MANYHOUSEUNIT_MULTIPLIER)
			continue;
		GetSimplePerson( &clHouseUnit[siIndex].clPerson );
		GetRentContract( &clHouseUnit[siIndex].clContract );
		m_odbc.GetData( &clHouseUnit[siIndex].clStorageInfo.bCreatedHouseUnitUsedGoodsBuilding, 0 );
		m_odbc.GetData( clHouseUnit[siIndex].m_szName,					sizeof(clHouseUnit[siIndex].m_szName));
	END_FETCHDATA()
	
	// 창고정보
	BEGIN_NEXTRECORDSET()
		SI32 siHouseUnitIndex = 0, siStorageIndex = 0;

		BEGIN_FETCHDATA()
			m_odbc.GetData( &siHouseUnitIndex,	0);
			m_odbc.GetData( &siStorageIndex,	0);
			if(siHouseUnitIndex >= (MANYHOUSEUNIT_NUMBER*MANYHOUSEUNIT_MULTIPLIER) || siStorageIndex >= MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER)
				continue;
			GetItemData( &clHouseUnit[siHouseUnitIndex].clStorageInfo.clItem[siStorageIndex]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// 마굿간정보
	BEGIN_NEXTRECORDSET()
		SI32 siHouseUnitIndex = 0, siHorseIndex = 0;

		BEGIN_FETCHDATA()
			m_odbc.GetData( &siHouseUnitIndex,	0);
			m_odbc.GetData( &siHorseIndex,	0);
			if(siHouseUnitIndex >= (MANYHOUSEUNIT_NUMBER*MANYHOUSEUNIT_MULTIPLIER) || siHorseIndex >= MAX_HOUSEUNIT_HORSENUMBER)
				continue;
			GetHorseInfo( &clHouseUnit[siHouseUnitIndex].clStorageInfo.clHorse[siHorseIndex]);
			clHouseUnit[siHouseUnitIndex].clStorageInfo.bCreatedHouseUnitStable[siHorseIndex] = true;
		END_FETCHDATA()
	END_NEXTRECORDSET()


	sendMsg.siResult = 1;


	// send
	sendMsg.siStartIndex = 0;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );

	sendMsg.siStartIndex = 20;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );

	sendMsg.siStartIndex = 40;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );

	sendMsg.siStartIndex = 60;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );

	sendMsg.siStartIndex = 80;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );

	sendMsg.siStartIndex = 100;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );

	sendMsg.siStartIndex = 120;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );

	sendMsg.siStartIndex = 140;
	memcpy(sendMsg.clHouseUnit, &clHouseUnit[sendMsg.siStartIndex], sizeof(sendMsg.clHouseUnit));
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::GetHouseApplyerList()
{
	// 받은 패킷
	sDBRequest_GetHouseApplyerList *pRecvMsg = (sDBRequest_GetHouseApplyerList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetHouseApplyerList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETHOUSEAPPLYERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	SI32	siIndex = 0, siPersonID = 0;
	char	szName[MAX_PLAYER_NAME]; ZeroMemory(szName, sizeof(szName));

	BEGIN_SP( "rsp_HouseApplyerListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &siIndex,					0);
		GetRentApplyer( &sendMsg.clApply[siIndex]);
	END_FETCHDATA()
	
	sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

// 시전임차신청을 한다
void DBGameProtocol::HouseRentApply()
{
	// 받은 패킷
	sDBRequest_HouseRentApply *pRecvMsg = (sDBRequest_HouseRentApply *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseRentApply sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSERENTAPPLY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clContract.clDate);

	// DB Query
	BEGIN_SP( "rsp_HouseRentApply" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetRentContract( &pRecvMsg->clContract, &clGameDate);
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siIndex,			0);
		GetSimplePerson( &sendMsg.clApply );
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
		sDBLogMsg_QueryString LogMsg("HouseRent Apply", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 시전을 임차한다
void DBGameProtocol::HouseRent()
{
	// 받은 패킷
	sDBRequest_HouseRent *pRecvMsg = (sDBRequest_HouseRent *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseRent sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSERENT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	cltGameDate clGameDate(&pRecvMsg->clRentContract.clDate);

	// DB Query
	SI32 siMaxRealEsatePerPerson = MAX_REAL_ESTATE_NUMBER_PER_PERSON;

	BEGIN_SP( "rsp_HouseRent" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex) );
		SetRentContract( &pRecvMsg->clRentContract, &clGameDate );
		m_odbc.SetParam( SQL_INTEGER, &siMaxRealEsatePerPerson, sizeof(siMaxRealEsatePerPerson) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0 );
		m_odbc.GetData( &sendMsg.siVillageUnique,	0 );

		GetSimplePerson( &sendMsg.clPerson );

		m_odbc.GetData( &sendMsg.siIndex,			0 );

		GetRentContract( &sendMsg.clRentContract );

		m_odbc.GetData( &sendMsg.siApplyerIndex,	0 );

		GetRentApplyer( &sendMsg.clApplyer );

		m_odbc.GetData( sendMsg.m_szName,	sizeof(sendMsg.m_szName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseIndex = %d, ApplyIndex = %d, RentFee = %I64d, RentPeriod = %d, Time = %d"
				, sendMsg.siVillageUnique, sendMsg.siIndex, sendMsg.siApplyerIndex, sendMsg.clApplyer.clContract.siRentFee
				, sendMsg.clApplyer.clContract.siRentPeriod, sendMsg.clApplyer.clContract.clDate.GetHourVary() );
		sDBLogMsg_QueryString LogMsg("HouseRent", sendMsg.clPerson.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HouseRentApplyCancel()
{
	// 받은 패킷
	sDBRequest_HouseRentApplyCancel *pRecvMsg = (sDBRequest_HouseRentApplyCancel *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseRentApplyCancel sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSERENTAPPLYCANCEL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HouseRentApplyCancel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siIndex,			0);
		GetRentApplyer( &sendMsg.clApply );
	END_FETCHDATA()
		
	sendMsg.siResult = 1;
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ApplyIndex = %d, RentFee = %I64d, RentPeriod = %d, Time = %d"
				, sendMsg.siVillageUnique, sendMsg.siIndex, sendMsg.clApply.clContract.siRentFee
				, sendMsg.clApply.clContract.siRentPeriod, sendMsg.clApply.clContract.clDate.GetHourVary() );
		sDBLogMsg_QueryString LogMsg("HouseRent Apply Cancel", sendMsg.clApply.clPerson.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 시전신청자 삭제
void DBGameProtocol::DelHouseApplyer()
{
	// 받은 패킷
	sDBRequest_DelHouseApplyer *pRecvMsg = (sDBRequest_DelHouseApplyer *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DelHouseApplyer sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DELHOUSEAPPLYER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 10. 29 로그에 personID가 필요해서.]
	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// PCK : 임차희망자명단을 모조리 삭제하는것을 수정
	sendMsg.siDelSlotNum = pRecvMsg->siDelSlotNum;

	// DB Query
	BEGIN_SP( "rsp_HouseApplyerDelByOwner" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d", sendMsg.siVillageUnique);
		sDBLogMsg_QueryString LogMsg("Delete HouseRent Applyer", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PayRentFee()
{
	// 받은 패킷
	sDBRequest_PayRentFee *pRecvMsg = (sDBRequest_PayRentFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PayRentFee sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PAYRENTFEE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 11. 2 창고 임대료 지불]
	sendMsg.siRentFee = pRecvMsg->siRentFee;
	
	// DB Query
	BEGIN_SP( "rsp_HouseRentFeePay" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRentFee,			sizeof(pRecvMsg->siRentFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseIndex,		sizeof(pRecvMsg->siHouseIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);

		m_odbc.GetData( &sendMsg.siPersonID,		0);
		m_odbc.GetData( &sendMsg.siBankMoney,		0);
		m_odbc.GetData( &sendMsg.siRentFee,			0);
		m_odbc.GetData( &sendMsg.siDelayNum,		0);
		m_odbc.GetData( &sendMsg.siDelayMoney,		0);

		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siHouseIndex,		0);
		m_odbc.GetData( &sendMsg.siHouseMoney,		0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1 || sendMsg.siResult == -1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseIndex = %d, BankMoney = %I64d, RentFee = %I64d, DeleyNum = %d, DelayMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siHouseIndex, sendMsg.siBankMoney, sendMsg.siRentFee
				, sendMsg.siDelayNum, sendMsg.siDelayMoney );
		sDBLogMsg_QueryString LogMsg("House RentFee Pay", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RecallHouse()
{
	// 받은 패킷
	sDBRequest_RecallHouse *pRecvMsg = (sDBRequest_RecallHouse *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_RecallHouse sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RECALLHOUSE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	MStrCpy(sendMsg.szOwnerName, pRecvMsg->szOwnerName, MAX_PLAYER_NAME);

	// DB Query
	BEGIN_SP( "rsp_HouseRecall" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRecallReason,	sizeof(pRecvMsg->siRecallReason) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseIndex,		sizeof(pRecvMsg->siHouseIndex) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPenaltyMoney,	sizeof(pRecvMsg->siPenaltyMoney) );
	END_SP()
	
	EXEC_SQL()
	
	//SI64 itHouseMoney = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);

		m_odbc.GetData( &sendMsg.siPersonID,		0);
		m_odbc.GetData( &sendMsg.siRecallReason,	0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siHouseIndex,		0);
		m_odbc.GetData( &sendMsg.siPenaltyMoney,	0);
		m_odbc.GetData( &sendMsg.siRemainMoney,		0);
		m_odbc.GetData( &sendMsg.siHouseMoney,		0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseIndex = %d, RecallReason = %d, PenaltyMoney = %I64d, PersonMoney = %I64d, HouseMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siHouseIndex, sendMsg.siRecallReason, sendMsg.siPenaltyMoney, sendMsg.siRemainMoney, sendMsg.siHouseMoney
				);
		sDBLogMsg_QueryString LogMsg("Recall House", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 창고 이름 변경
void DBGameProtocol::ChangHouseUnitName()
{
	// 받은 패킷
	sDBRequest_ChangeHouseUnitName *pRecvMsg = (sDBRequest_ChangeHouseUnitName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ChangeHouseUnitName sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITNAMECHANGE;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_usCharID = pRecvMsg->m_usCharID;

	// DB Query
	BEGIN_SP( "rsp_HouseNameChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siVillageUnique,	sizeof(pRecvMsg->m_siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siIndex,			sizeof(pRecvMsg->m_siIndex) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->m_szName,			sizeof(pRecvMsg->m_szName) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult,			0);
		m_odbc.GetData( &sendMsg.m_siPersonID,			0);
		m_odbc.GetData( &sendMsg.m_siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.m_siIndex,			0);
		m_odbc.GetData( sendMsg.m_szName,			sizeof(sendMsg.m_szName));
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}


// 시전계약기간을 연장한다.
void DBGameProtocol::ExtendContract()
{
	// 받은 패킷
	sDBRequest_ExtendContract *pRecvMsg = (sDBRequest_ExtendContract *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_ExtendContract sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EXTENDCONTRACT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltGameDate clGameDate(&pRecvMsg->clRentContract.clDate);

	// DB Query
	BEGIN_SP( "rsp_HouseRentExtend" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRealEstateType,	sizeof(pRecvMsg->siRealEstateType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIndex,			sizeof(pRecvMsg->siIndex) );
		SetRentContract( &(pRecvMsg->clRentContract), &clGameDate );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);

		GetSimplePerson( &sendMsg.clPerson);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siRealEstateType,	0);
		m_odbc.GetData( &sendMsg.siIndex,			0);
		GetRentContract( &(sendMsg.clRentContract) );
		m_odbc.GetData( &sendMsg.siBankMoney, 0 );
	END_FETCHDATA()

	// send
	if( sendMsg.siRealEstateType ==1 ) SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseIndex = %d, RentFee = %I64d, RentPeriod = %d, Time = %d"
				, sendMsg.siVillageUnique, sendMsg.siIndex, sendMsg.clRentContract.siRentFee
				, sendMsg.clRentContract.siRentPeriod, sendMsg.clRentContract.clDate.GetHourVary() );
		sDBLogMsg_QueryString LogMsg("Extend House Contract", sendMsg.clPerson.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetHouseStgPos()
{
	// 받은 패킷
	sDBRequest_GetHouseStgPos *pRecvMsg = (sDBRequest_GetHouseStgPos *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetHouseStgPos sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETHOUSESTGPOS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_HouseStgPosGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot,	sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgPos,			sizeof(pRecvMsg->siStgPos) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,				0);
		m_odbc.GetData( &sendMsg.siVillageUnique,		0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot,		0);
		m_odbc.GetData( &sendMsg.siStgPos,				0);
		GetItemData( &sendMsg.clItem);
	END_FETCHDATA()
	
	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::MakeItem()
{
	// 받은 패킷
	sDBRequest_MakeItem *pRecvMsg = (sDBRequest_MakeItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_MakeItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MAKEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_HouseMakeItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot,	sizeof(pRecvMsg->siHouseUnitSlot) );
		for( int i=0; i < MAX_MATERIAL_NUMBER; i++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMaterial[i].siStgPos,		sizeof(pRecvMsg->clMaterial[i].siStgPos) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clMaterial[i].siItemUnique,	sizeof(pRecvMsg->clMaterial[i].siItemUnique) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clMaterial[i].siUseItemNum,	sizeof(pRecvMsg->clMaterial[i].siUseItemNum) );
		}
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siOutputPos,	sizeof(pRecvMsg->siOutputPos) );
		SetItemParam( &pRecvMsg->clItem);
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->bCanPileSwitch, sizeof(pRecvMsg->bCanPileSwitch) );
	END_SP()


	EXEC_SQL()

	/* 생산된 물품의 결과 처리 */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,				0);
		m_odbc.GetData( &sendMsg.siPersonID,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,		0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot,		0);
		m_odbc.GetData( &sendMsg.siOutputPos,			0);
		m_odbc.GetData( &sendMsg.siAddedNum,			0);
		GetItemData( &sendMsg.clItem);
	END_FETCHDATA()

	sendMsg.bAlmostFailSwitch = pRecvMsg->bAlmostFailSwitch;

	// send
	SendPacket( (BYTE *)&sendMsg );

	BEGIN_NEXTRECORDSET()
		/* 소모된 물품의 결과 처리 */
		// 보낼 패킷
		sDBResponse_GetHouseStgPos sendMsg2;
		
		sendMsg2.packetHeader.usCmd = DBMSG_RESPONSE_GETHOUSESTGPOS;
		sendMsg2.packetHeader.usSize = sizeof( sendMsg );
		sendMsg2.usCharID = pRecvMsg->usCharID;

		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg2.siResult,				0);
			m_odbc.GetData( &sendMsg2.siPersonID,			0);
			m_odbc.GetData( &sendMsg2.siVillageUnique,		0);
			m_odbc.GetData( &sendMsg2.siHouseUnitSlot,		0);
			m_odbc.GetData( &sendMsg2.siStgPos,				0);
			GetItemData( &sendMsg2.clItem);

			// send
			SendPacket( (BYTE *)&sendMsg2 );
		END_FETCHDATA()
	END_NEXTRECORDSET()
}

void DBGameProtocol::HouseUnitUsedGoodsBuildingCreate()
{
	// 받은 패킷
	sDBRequest_HouseUnitUsedGoodsBuildingCreate *pRecvMsg = (sDBRequest_HouseUnitUsedGoodsBuildingCreate *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseUnitUsedGoodsBuildingCreate sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITUSEDGOODSBUILDINGCREATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_HouseUnitSecondHandShopCreate" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot, sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siHouseUnitSlot, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clLeftItem );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseIndex = %d, InvPos = %d, Amount = %d, AfterItemNum = %d"
				, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot, sendMsg.siItemPos, pRecvMsg->clItem.siItemNum, sendMsg.clLeftItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg("SecondHandShop Create", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HouseUnitMakeItemFromUsedGoods()
{
	// 받은 패킷
	sDBRequest_HouseUnitMakeItemFromUsedGoods *pRecvMsg = (sDBRequest_HouseUnitMakeItemFromUsedGoods *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HouseUnitMakeItemFromUsedGoods sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siMakePercent = pRecvMsg->siMakePercent;
	
	// DB Query
	BEGIN_SP( "rsp_HouseUnitSecondHandShopSalvage" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot, sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromHoustUnitStgItemPos, sizeof(pRecvMsg->siFromHoustUnitStgItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromHouseUnitItemUnique, sizeof(pRecvMsg->siFromHouseUnitItemUnique) );
		for(int i = 0; i < MAX_MATERIAL_NUMBER; i++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToHouseUnitStgItemPos[i], sizeof(pRecvMsg->siToHouseUnitStgItemPos[i]) );
			SetItemParam( &pRecvMsg->clMakeItem[i] );
		}
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siHouseUnitSlot, 0 );
		m_odbc.GetData( &sendMsg.siFromItemPos, 0 );
		m_odbc.GetData( &sendMsg.siFromItemUnique, 0 );
	END_FETCHDATA()

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, HouseIndex = %d, ItemPos = %d, MakePercent = %d%%"
				, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot, sendMsg.siFromItemPos, sendMsg.siMakePercent
				);
		sDBLogMsg_QueryString LogMsg("SecondHandShop Salvage From", sendMsg.siPersonID, NULL, pRecvMsg->siFromHouseUnitItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
	
	BEGIN_NEXTRECORDSET()
		SI32 siItemPos = 0;
		SI32 siBeforeItemNum = 0;
		SI32 siAmount = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siToHouseUnitStgItemPos[siItemPos], 0 );
			GetItemData( &sendMsg.clMakeLeftItem[siItemPos] );
			m_odbc.GetData( &siBeforeItemNum, 0 );
			m_odbc.GetData( &siAmount, 0 );
			// 로그 작성
			if(sendMsg.siResult == 1)
			{
				char log[512] = "";
				sprintf(log, "VillageUnique = %d, HouseIndex = %d, ItemPos = %d, Amount = %d, BeforeItemNum = %d, AfterItemNum = %d"
						, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot, sendMsg.siToHouseUnitStgItemPos[siItemPos]
						, siAmount, siBeforeItemNum, sendMsg.clMakeLeftItem[siItemPos].siItemNum
						);
				sDBLogMsg_QueryString LogMsg("SecondHandShop Salvage To", sendMsg.siPersonID, NULL, sendMsg.clMakeLeftItem[siItemPos].siUnique, log);
				SendLogMsg((sPacketHeader *)&LogMsg);
			}
			siItemPos++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
