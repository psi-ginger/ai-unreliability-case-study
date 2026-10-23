#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"

void DBGameProtocol::SetTax()
{
	// 받은 패킷
	sDBRequest_SetTax *pRecvMsg = (sDBRequest_SetTax *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetTax sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETTAX;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_TaxSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique,	sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrType,			sizeof(pRecvMsg->siStrType) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->usYear,			sizeof(pRecvMsg->usYear) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->clDueTax.itMoney,	sizeof(pRecvMsg->clDueTax.itMoney) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siStrType,			0);
	END_FETCHDATA()
	
	sendMsg.usYear = pRecvMsg->usYear;
	sendMsg.clDueTax.Set( pRecvMsg->clDueTax.itMoney );

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::PayTax()
{
	// 받은 패킷
	sDBRequest_PayTax *pRecvMsg = (sDBRequest_PayTax *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PayTax sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PAYTAX;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	SI32 siYear = pRecvMsg->usYear;
	SI32 siDateVary = pRecvMsg->clDate.GetDateVary();
	SI32 siPersonID = 0;

	BEGIN_SP( "rsp_TaxPay" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStrType, sizeof(pRecvMsg->siStrType) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->clPayTax.itMoney, sizeof(pRecvMsg->clPayTax.itMoney) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siChiefPay, sizeof(pRecvMsg->siChiefPay) );
		m_odbc.SetParam( SQL_INTEGER, &siYear, sizeof(siYear) );
		m_odbc.SetParam( SQL_INTEGER, &siDateVary, sizeof(siDateVary) );
	END_SP()
	
	EXEC_SQL()
	
	siYear = siDateVary = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.siStrType,			0);
		m_odbc.GetData( &sendMsg.clPaidTax.itMoney, 0);
		m_odbc.GetData( &sendMsg.siChiefPay, 0 );
		m_odbc.GetData( &siYear, 0 ); sendMsg.usYear = siYear;
		m_odbc.GetData( &siDateVary, 0 ); sendMsg.clDate.MakeFullDateFromDateVary( siDateVary );
		m_odbc.GetData( &sendMsg.clStructureMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.clCityhallMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siPersonMoney, 0 );
		m_odbc.GetData( &siPersonID, 0 );
	END_FETCHDATA()
	
	sendMsg.usYear = pRecvMsg->usYear;
	sendMsg.clPaidTax.Set( pRecvMsg->clPayTax.itMoney );

	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, StrType = %d, PayTax = %I64d, ChiefPay = %I64d, Year = %d, AfterStrMoney = %I64d, AfterCityHallMoney = %I64d, AfterChiefMoney = %I64d"
				, sendMsg.siVillageUnique, sendMsg.siStrType, sendMsg.clPaidTax.itMoney, sendMsg.siChiefPay
				, siYear, sendMsg.clStructureMoney.itMoney, sendMsg.clCityhallMoney.itMoney, sendMsg.siPersonMoney
				);
		sDBLogMsg_QueryString LogMsg("Pay Tax", siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::GetTaxList( class cltTaxInfo *pclTaxInfo, SI32 siVillageUnique, SI32 siStrType )
{
	// DB Query
	BEGIN_SP( "rsp_TaxListGet" )
		m_odbc.SetParam( SQL_INTEGER, &siVillageUnique,	sizeof(siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &siStrType,		sizeof(siStrType) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siCount = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(pclTaxInfo->clTaxUnitInfo[siCount].usYear), 0 );
		m_odbc.GetData( &(pclTaxInfo->clTaxUnitInfo[siCount].clDueTax), 0 );
		m_odbc.GetData( &(pclTaxInfo->clTaxUnitInfo[siCount].clPaidTax), 0 );
		siCount++;
	END_FETCHDATA()
	
	pclTaxInfo->SetCount(siCount);
}