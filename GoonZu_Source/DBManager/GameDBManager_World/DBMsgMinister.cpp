#include "DBGameProtocol.h"
#include "DBMsg-System.h"
#include "DBMsg-Person.h"

void DBGameProtocol::SetYezoInfo()
{
	// 받은 패킷
	sDBRequest_SetYezoInfo *pRecvMsg = (sDBRequest_SetYezoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetYezoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETYEZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterYezoInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGreeting, sizeof(pRecvMsg->szGreeting) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siJapanSwordSolNum, sizeof(pRecvMsg->siJapanSwordSolNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siJapanGunSolNum, sizeof(pRecvMsg->siJapanGunSolNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siJapanCommanderNum, sizeof(pRecvMsg->siJapanCommanderNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siJapanPorterNum, sizeof(pRecvMsg->siJapanPorterNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siWarVillageUnique, sizeof(pRecvMsg->siWarVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siJapanSwordSolNum, 0 );
		m_odbc.GetData( &sendMsg.siJapanGunSolNum, 0 );
		m_odbc.GetData( &sendMsg.siJapanCommanderNum, 0 );
		m_odbc.GetData( &sendMsg.siJapanPorterNum, 0 );
		m_odbc.GetData( &sendMsg.siWarVillageUnique, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "SwordSolNum = %d, GunSolNum = %d, CommanderNum = %d, PorterNum = %d, WarVillageUnique = %d"
				, sendMsg.siJapanSwordSolNum, sendMsg.siJapanGunSolNum, sendMsg.siJapanCommanderNum, sendMsg.siJapanPorterNum, sendMsg.siWarVillageUnique
				);
		sDBLogMsg_QueryString LogMsg("Minister Yezo Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetYezoInfo()
{
	// 받은 패킷
	sDBRequest_GetYezoInfo *pRecvMsg = (sDBRequest_GetYezoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetYezoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETYEZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterYezoInfoGet" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siJapanSwordSolNum, 0 );
		m_odbc.GetData( &sendMsg.siJapanGunSolNum, 0 );
		m_odbc.GetData( &sendMsg.siJapanCommanderNum, 0 );
		m_odbc.GetData( &sendMsg.siJapanPorterNum, 0 );
		m_odbc.GetData( &sendMsg.siWarVillageUnique, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetByungzoInfo()
{
	// 받은 패킷
	sDBRequest_SetByungzoInfo *pRecvMsg = (sDBRequest_SetByungzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetByungzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETBYUNGZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterByungzoInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGreeting, sizeof(pRecvMsg->szGreeting) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSetTime, sizeof(pRecvMsg->siSetTime) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siWarStartTime, sizeof(pRecvMsg->siWarStartTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSetWarCount, sizeof(pRecvMsg->siSetWarCount) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siBlackArmyNum, sizeof(pRecvMsg->siBlackArmyNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageWarTime, sizeof(pRecvMsg->siVillageWarTime) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siSetTime, 0 );
		m_odbc.GetData( &sendMsg.siWarStartTime, 0 );
		m_odbc.GetData( &sendMsg.siSetWarCount, 0 );
		m_odbc.GetData( &sendMsg.siBlackArmyNum, 0 );
		m_odbc.GetData( &sendMsg.siVillageWarTime, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "SetTime = %d, WarStartTime = %d, SetWarCount = %d, BlackArmyNum = %d, VillageWarTime = %d"
				, pRecvMsg->siSetTime, pRecvMsg->siWarStartTime, pRecvMsg->siSetWarCount, pRecvMsg->siBlackArmyNum, pRecvMsg->siVillageWarTime
				);
		sDBLogMsg_QueryString LogMsg("Minister Byungzo Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetByungzoInfoVillageWarTime()
{
	// 받은 패킷
	sDBRequest_SetByungzoInfo_VillageWarTime *pRecvMsg = (sDBRequest_SetByungzoInfo_VillageWarTime *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetByungzoInfo_VillageWarTime sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETBYUNGZOINFO_VILLAGEWARTIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	MStrCpy(sendMsg.m_szGreeting, pRecvMsg->m_szGreeting, sizeof(pRecvMsg->m_szGreeting));

	// DB Query
	BEGIN_SP( "rsp_MinisterByungzoInfoVillageWarSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageWarTime, sizeof(pRecvMsg->siVillageWarTime) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->m_szGreeting, sizeof(pRecvMsg->m_szGreeting) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageWarTime, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageWarTime = %d"
			, pRecvMsg->siVillageWarTime
			);
		sDBLogMsg_QueryString LogMsg("Minister Byungzo VillageWar Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetByungzoInfo()
{
	// 받은 패킷
	sDBRequest_GetByungzoInfo *pRecvMsg = (sDBRequest_GetByungzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetByungzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETBYUNGZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterByungzoInfoGet" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siSetTime, 0 );
		m_odbc.GetData( &sendMsg.siWarStartTime, 0 );
		m_odbc.GetData( &sendMsg.siSetWarCount, 0 );
		m_odbc.GetData( &sendMsg.siBlackArmyNum, 0 );
		m_odbc.GetData( &sendMsg.siVillageWarTime, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::SetHozoInfo()
{
	// 받은 패킷
	sDBRequest_SetHozoInfo *pRecvMsg = (sDBRequest_SetHozoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetHozoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETHOZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterHozoInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGreeting, sizeof(pRecvMsg->szGreeting) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siResidentNum, sizeof(pRecvMsg->siResidentNum) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siPrizeMoney, sizeof(pRecvMsg->siPrizeMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPrizeSetTime, sizeof(pRecvMsg->siPrizeSetTime) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFixationMoney, sizeof(pRecvMsg->siFixationMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMaxShop, sizeof(pRecvMsg->siMaxShop) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siTutorialVillageDivideend, sizeof(pRecvMsg->siTutorialVillageDivideend) );		
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siResidentNum, 0 );
		m_odbc.GetData( &sendMsg.siPrizeMoney, sizeof(sendMsg.siPrizeMoney) );
		m_odbc.GetData( &sendMsg.siPrizeSetTime, sizeof(sendMsg.siPrizeSetTime) );
		m_odbc.GetData( &sendMsg.siFixationMoney, sizeof(sendMsg.siFixationMoney) );
		m_odbc.GetData( &sendMsg.siMaxShop, sizeof(sendMsg.siMaxShop) );
		m_odbc.GetData( &sendMsg.siTutorialVillageDivideend, sizeof(sendMsg.siTutorialVillageDivideend) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ResidentNum = %d, PrizeMoney = %I64d, PrizeSetTime = %d"
				, sendMsg.siResidentNum, sendMsg.siPrizeMoney, sendMsg.siPrizeSetTime
				);
		sDBLogMsg_QueryString LogMsg("Minister Hozo Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetHozoInfo()
{
	// 받은 패킷
	sDBRequest_GetHozoInfo *pRecvMsg = (sDBRequest_GetHozoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetHozoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETHOZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterHozoInfoGet" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siResidentNum, 0 );
		m_odbc.GetData( &sendMsg.siTotalCharNum, sizeof(sendMsg.siTotalCharNum) );
		m_odbc.GetData( &sendMsg.siCalcTime, sizeof(sendMsg.siCalcTime) );
		m_odbc.GetData( &sendMsg.siPrizeMoney, sizeof(sendMsg.siPrizeMoney) );
		m_odbc.GetData( &sendMsg.siPrizeSetTime, sizeof(sendMsg.siPrizeSetTime) );
		m_odbc.GetData( &sendMsg.siFixationMoney, sizeof(sendMsg.siFixationMoney) );
		m_odbc.GetData( &sendMsg.siMaxShop, sizeof(sendMsg.siMaxShop) );
		m_odbc.GetData( &sendMsg.siBankruptcyVillageNumber, sizeof(sendMsg.siBankruptcyVillageNumber) );
		m_odbc.GetData( &sendMsg.siSetBankruptcyVillageDate, sizeof(sendMsg.siSetBankruptcyVillageDate) );
		m_odbc.GetData( &sendMsg.siTutorialVillageDivideend, sizeof(sendMsg.siTutorialVillageDivideend) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetGoonzuInfo()
{
	// 받은 패킷
	sDBRequest_SetGoonzuInfo *pRecvMsg = (sDBRequest_SetGoonzuInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetGoonzuInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETGOONZUINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterGoonzuInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGreeting, sizeof(pRecvMsg->szGreeting) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetGoonzuInfo()
{
	// 받은 패킷
	sDBRequest_GetGoonzuInfo *pRecvMsg = (sDBRequest_GetGoonzuInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetGoonzuInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETGOONZUINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterGoonzuInfoGet" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.iTotalFinances, 0 );
		m_odbc.GetData( &sendMsg.siLiquidationVillageNumber, 0 );
		m_odbc.GetData( &sendMsg.siBonusTime );
		m_odbc.GetData( &sendMsg.siMakeBonusTime );
		m_odbc.GetData( &sendMsg.siLastChangedBonusTimeYear );
		m_odbc.GetData( &sendMsg.siLastChangedBonusTimeMonth );
		m_odbc.GetData( &sendMsg.siLastChangedBonusTimeDay );
	END_FETCHDATA()

	SI08 siMinisterKind;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siMinisterKind, 0 );
			siMinisterKind -= 2;	// 이조:2, 호조:3, 예조:4, 병조:5, 형조6, 공조:7 이므로 배열에 넣기전에 2씩 뺀다.
			if(siMinisterKind < 0 || (siMinisterKind >= sizeof(sendMsg.siSetMinisterTime) / sizeof(sendMsg.siSetMinisterTime[0])))
				continue;
			m_odbc.GetData( &sendMsg.siSetMinisterTime[siMinisterKind], 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetIzoInfo()
{
	// 받은 패킷
	sDBRequest_SetIzoInfo *pRecvMsg = (sDBRequest_SetIzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetIzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETIZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterIzoInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGreeting, sizeof(pRecvMsg->szGreeting) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPriceValue, sizeof(pRecvMsg->siPriceValue) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPriceTime, sizeof(pRecvMsg->siPriceTime) );

		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bWorldTradeTax, sizeof(pRecvMsg->bWorldTradeTax) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSpecialTaxRange, sizeof(pRecvMsg->siSpecialTaxRange) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSpecialTaxRate, sizeof(pRecvMsg->siSpecialTaxRate) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMarketDownRate, sizeof(pRecvMsg->siMarketDownRate) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siTinTin, sizeof(pRecvMsg->siTinTin) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siPriceValue, 0 );
		m_odbc.GetData( &sendMsg.siPriceTime, 0 );
		m_odbc.GetData( &sendMsg.siTradeTax, 0 );
		m_odbc.GetData( &sendMsg.siTradeLimitMoney, 0 );
		m_odbc.GetData( &sendMsg.bWorldTradeTax, 0 );
		m_odbc.GetData( &sendMsg.siSpecialTaxRange, 0 );
		m_odbc.GetData( &sendMsg.siSpecialTaxRate, 0 );
		m_odbc.GetData( &sendMsg.siMarketDownRate, 0 );
		m_odbc.GetData( &sendMsg.siTinTin, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "PriceValue = %d, PriceTime = %d"
				, sendMsg.siPriceValue, sendMsg.siPriceTime
				);
		sDBLogMsg_QueryString LogMsg("Minister Izo Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetIzoInfo()
{
	// 받은 패킷
	sDBRequest_GetIzoInfo *pRecvMsg = (sDBRequest_GetIzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetIzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETIZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterIzoInfoGet" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siPriceValue, 0 );
		m_odbc.GetData( &sendMsg.siPriceTime, 0 );
		m_odbc.GetData( &sendMsg.siTradeTax, 0 );
		m_odbc.GetData( &sendMsg.siTradeLimitMoney, 0 );
		m_odbc.GetData( &sendMsg.bWorldTradeTax, 0 );
		m_odbc.GetData( &sendMsg.siSpecialTaxRange, 0 );
		m_odbc.GetData( &sendMsg.siSpecialTaxRate, 0 );
		m_odbc.GetData( &sendMsg.siMarketDownRate, 0 );
		m_odbc.GetData( &sendMsg.siTinTin, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siTotalMoney, 0 );
		m_odbc.GetData( &sendMsg.siCalcTime, 0 );
	END_FETCHDATA()
	END_NEXTRECORDSET()
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetGongzoInfo()
{
	// 받은 패킷
	sDBRequest_SetGongzoInfo *pRecvMsg = (sDBRequest_SetGongzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetGongzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETGONGZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterGongzoInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGreeting, sizeof(pRecvMsg->szGreeting) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPriceTime, sizeof(pRecvMsg->siPriceTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siLeftOverValue, sizeof(pRecvMsg->siLeftOverValue) );
		m_odbc.SetParam( SQL_BINARY, &pRecvMsg->siPriceValue[0], sizeof(SI16)  * 7 );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siCraftFund, sizeof(pRecvMsg->siCraftFund) );
		m_odbc.SetParam( SQL_BINARY, &pRecvMsg->siPriceValue[7], sizeof(SI16)  * 7);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siLeftOverValue_Armor, sizeof(pRecvMsg->siLeftOverValue_Armor) );
END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siPriceTime, 0 );
		m_odbc.GetData( &sendMsg.siLeftOverValue, 0 );
		m_odbc.GetData( (BYTE*)&sendMsg.siPriceValue[0], sizeof(SI16)  * 7 );
		m_odbc.GetData( &sendMsg.siCraftFund );
		m_odbc.GetData( (BYTE*)&sendMsg.siPriceValue[7], sizeof(SI16)  * 7 );
		m_odbc.GetData( &sendMsg.siLeftOverValue_Armor, sizeof(sendMsg.siLeftOverValue_Armor) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "PriceTime = %d, LeftOverValue = %d, PriceValue(Sword/Spear/Staff/Axe/Bow/Gun/CraftFund) = %d / %d / %d / %d / %d / %d / %I64d"
				, sendMsg.siPriceTime, sendMsg.siLeftOverValue, sendMsg.siPriceValue[GONGZO_SKILL_SWORD]
				, sendMsg.siPriceValue[GONGZO_SKILL_SPEAR], sendMsg.siPriceValue[GONGZO_SKILL_STAFF]
				, sendMsg.siPriceValue[GONGZO_SKILL_AXE], sendMsg.siPriceValue[GONGZO_SKILL_BOW]
				, sendMsg.siPriceValue[GONGZO_SKILL_GUN], sendMsg.siCraftFund
				);
		sDBLogMsg_QueryString LogMsg("Minister Gongzo Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetGongzoInfo()
{
	// 받은 패킷
	sDBRequest_GetGongzoInfo *pRecvMsg = (sDBRequest_GetGongzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetGongzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETGONGZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterGongzoInfoGet" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siPriceTime, 0 );
		m_odbc.GetData( &sendMsg.siLeftOverValue, 0 );
		m_odbc.GetData( (BYTE*)&sendMsg.siPriceValue[0], sizeof(SI16)  * 7 );
		m_odbc.GetData( &sendMsg.siCraftFund );
		m_odbc.GetData( (BYTE*)&sendMsg.siPriceValue[7], sizeof(SI16)  * 7 );
		m_odbc.GetData( &sendMsg.siLeftOverValue_Armor, sizeof(sendMsg.siLeftOverValue_Armor) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetHyungzoInfo()
{
	// 받은 패킷
	sDBRequest_SetHyungzoInfo *pRecvMsg = (sDBRequest_SetHyungzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetHyungzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETHYUNGZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterHyungzoInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szGreeting, sizeof(pRecvMsg->szGreeting) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siLimitStealNum, sizeof(pRecvMsg->siLimitStealNum) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSuspectTime, sizeof(pRecvMsg->siSuspectTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPrisonTime, sizeof(pRecvMsg->siPrisonTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPrisonerPenalty, sizeof(pRecvMsg->siPrisonerPenalty) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siLimitStealNum, 0 );
		m_odbc.GetData( &sendMsg.siSuspectTime, 0 );
		m_odbc.GetData( &sendMsg.siPrisonTime, 0 );
		m_odbc.GetData( &sendMsg.siPrisonerPenalty, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "LimitStealNum = %d, SuspectTime = %d, PrisonTime = %d"
				, sendMsg.siLimitStealNum, sendMsg.siSuspectTime, sendMsg.siPrisonTime
				);
		sDBLogMsg_QueryString LogMsg("Minister Hyungzo Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetHyungzoInfo()
{
	// 받은 패킷
	sDBRequest_GetHyungzoInfo *pRecvMsg = (sDBRequest_GetHyungzoInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetHyungzoInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETHYUNGZOINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterHyungzoInfoGet" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetMinisterBase( &sendMsg.MinisterBase );
		m_odbc.GetData( &sendMsg.siLimitStealNum, 0 );
		m_odbc.GetData( &sendMsg.siSuspectTime, 0 );
		m_odbc.GetData( &sendMsg.siPrisonTime, 0 );
		m_odbc.GetData( &sendMsg.siPrisonerPenalty, 0 );
	END_FETCHDATA()
		
	BEGIN_NEXTRECORDSET()
	SI32 siCount = 0;
		BEGIN_FETCHDATA()
			if(siCount >= MAX_PRISON_LIST_NUM)
				continue;
			m_odbc.GetData( &sendMsg.stHyungzoMember[siCount].siDate, 0 );
			m_odbc.GetData( &sendMsg.stHyungzoMember[siCount].siPrisonSecond, 0 );
			m_odbc.GetData( sendMsg.stHyungzoMember[siCount].szCharName, sizeof(sendMsg.stHyungzoMember[siCount].szCharName) );
			m_odbc.GetData( sendMsg.stHyungzoMember[siCount].szPrisonReason, sizeof(sendMsg.stHyungzoMember[siCount].szPrisonReason) );
			siCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetGoonzuFinances()
{
	// 받은 패킷
	sDBRequest_SetGoonzuFinances *pRecvMsg = (sDBRequest_SetGoonzuFinances *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetGoonzuFinances sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETGOONZUFINANCES;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterGoonzuFinancesSet" )
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->iTempFinances, sizeof(pRecvMsg->iTempFinances) );
	END_SP()
	
	EXEC_SQL()
	
	INT64 itFinances = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &itFinances, 0 );
	END_FETCHDATA()
		
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "AddFinances = %I64d, AfterFinances = %I64d"
				, pRecvMsg->iTempFinances, itFinances
				);
		sDBLogMsg_QueryString LogMsg("Minister Goonzu Finances Set", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetAngelApplyer()
{
	// 받은 패킷
	sDBRequest_SetAngelApplyer *pRecvMsg = (sDBRequest_SetAngelApplyer *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetAngelApplyer sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETANGELAPPLYER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterAngelApply" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bAdmin, sizeof(pRecvMsg->bAdmin) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siIndex, 0 );
		m_odbc.GetData( &sendMsg.bAdmin, 0 );
		GetAngelInfo( &sendMsg.stHozoAngelApplyer );
	END_FETCHDATA()
		
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "bAdmin = %d"
				, sendMsg.bAdmin
				);
		sDBLogMsg_QueryString LogMsg("Minister Angel Applyer Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetAngelApplyerList()
{
	// 받은 패킷
	sDBRequest_GetAngelApplyerList *pRecvMsg = (sDBRequest_GetAngelApplyerList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetAngelApplyerList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETANGELAPPLYERLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterAngelApplyerListGet" )
	END_SP()
	
	EXEC_SQL()
	
	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &siIndex, 0 );
		if(siIndex >= MAX_YEZO_ANGELLIST)
			continue;
		GetAngelInfo( &sendMsg.stHozoAngelApplyerList[siIndex] );
	END_FETCHDATA()
		
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetAngel()
{
	// 받은 패킷
	sDBRequest_SetAngel *pRecvMsg = (sDBRequest_SetAngel *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetAngel sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETANGEL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterAngel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bAdmin, sizeof(pRecvMsg->bAdmin) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siIndex, 0 );
		m_odbc.GetData( &sendMsg.bAdmin, 0 );
		GetAngelInfo( &sendMsg.stHozoAngel );
	END_FETCHDATA()
		
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "siIndex = %d, bAdmin = %d"
				, sendMsg.siIndex, sendMsg.bAdmin
				);
		sDBLogMsg_QueryString LogMsg("Minister Angel Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetAngelList()
{
	// 받은 패킷
	sDBRequest_GetAngelList *pRecvMsg = (sDBRequest_GetAngelList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetAngelList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETANGELLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterAngelListGet" )
	END_SP()
	
	EXEC_SQL()
	
	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &siIndex, 0 );
		if(siIndex >= MAX_YEZO_ANGELLIST)
			continue;
		GetAngelInfo( &sendMsg.stHozoAngelList[siIndex] );
	END_FETCHDATA()
		
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetMinister()
{
	// 받은 패킷
	sDBRequest_SetMinister *pRecvMsg = (sDBRequest_SetMinister *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetMinister sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETMINISTER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMinister, sizeof(pRecvMsg->siMinister) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szName, sizeof(pRecvMsg->szName) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bMaster, sizeof(pRecvMsg->bMaster) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSetMinisterTime, sizeof(pRecvMsg->siSetMinisterTime) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMinister, 0 );
		m_odbc.GetData( &sendMsg.siMinisterPersonId, 0 );
		m_odbc.GetData( &sendMsg.siTempPersonId, 0 );
	END_FETCHDATA()
		
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "MinisterType = %d, OrderPersonID = %d"
				, sendMsg.siMinister, sendMsg.siPersonID
				);
		sDBLogMsg_QueryString LogMsg("Minister Set", sendMsg.siMinisterPersonId, sendMsg.siTempPersonId, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::SetPolice()
{
	// 받은 패킷
	sDBRequest_SetPolice *pRecvMsg = (sDBRequest_SetPolice *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetPolice sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETPOLICE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI16 siMaxPoliceList = MAX_HYUNGZO_POLICELIST;

	// DB Query
	BEGIN_SP( "rsp_MinisterPoliceSet" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPoliceName, sizeof(pRecvMsg->szPoliceName) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bAdmin, sizeof(pRecvMsg->bAdmin) );
		m_odbc.SetParam( SQL_SMALLINT, &siMaxPoliceList, sizeof(siMaxPoliceList) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.bAdmin, 0 );
		GetPoliceInfo( &sendMsg.stHyungzoPolice );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "bAdmin = %d"
			, sendMsg.bAdmin
			);
		sDBLogMsg_QueryString LogMsg("Minister Police Set", sendMsg.stHyungzoPolice.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetPoliceList()
{
	// 받은 패킷
	sDBRequest_GetPoliceList *pRecvMsg = (sDBRequest_GetPoliceList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetPoliceList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETPOLICELIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterPoliceListGet" )
	END_SP()

	EXEC_SQL()

	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		if(siIndex >= MAX_HYUNGZO_POLICELIST)
			break;
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetPoliceInfo( &sendMsg.stHyungzoPoliceList[siIndex] );
		siIndex++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetCriminal()
{
	// 받은 패킷
	sDBRequest_SetCriminal *pRecvMsg = (sDBRequest_SetCriminal *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetCriminal sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETCRIMINAL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterCriminalSet" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCriminalName, sizeof(pRecvMsg->szCriminalName));
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCriminalReason, sizeof(pRecvMsg->szCriminalReason));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTime, sizeof(pRecvMsg->siTime));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bAdmin, sizeof(pRecvMsg->bAdmin));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.bAdmin, 0 );
		GetCriminalInfo( &sendMsg.stHyungzoCriminal );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "bAdmin = %d, Reason = %s"
			, sendMsg.bAdmin, sendMsg.stHyungzoCriminal.szReason
			);
		sDBLogMsg_QueryString LogMsg("Minister Police Set", sendMsg.stHyungzoCriminal.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetCriminalList()
{
	// 받은 패킷
	sDBRequest_GetCriminalList *pRecvMsg = (sDBRequest_GetCriminalList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetCriminalList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETCRIMINALLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MinisterCriminalListGet" )
	END_SP()

	EXEC_SQL()

	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		if(siIndex >= MAX_HYUNGZO_CRIMINALLIST)
			break;
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetCriminalInfo( &sendMsg.stHyungzoCriminalList[siIndex] );
		siIndex++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GoonzuSetBonusTime()
{
	// 받은 패킷
	sDBRequest_Goonzu_SetBonusTime *pRecvMsg = (sDBRequest_Goonzu_SetBonusTime *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Goonzu_SetBonusTime sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GOONZU_SETBONUSTIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 11. 7 로그에 이용. 군주권한 보너스 타임(1시간)]
	sendMsg.siBonusTime		= pRecvMsg->siBonusTime;
	sendMsg.siMakeBonusTime = pRecvMsg->siMakeBonusTime;

	// DB Query
	BEGIN_SP( "rsp_MinisterGoonzuBonusTimeSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siBonusTime, sizeof(pRecvMsg->siBonusTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMakeBonusTime, sizeof(pRecvMsg->siMakeBonusTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siChangedBonusTimeYear, sizeof(pRecvMsg->siChangedBonusTimeYear) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siChangedBonusTimeMonth, sizeof(pRecvMsg->siChangedBonusTimeMonth) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siChangedBonusTimeDay, sizeof(pRecvMsg->siChangedBonusTimeDay) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "BonusTime = %d, %d/%d/%d"
			, pRecvMsg->siBonusTime, pRecvMsg->siChangedBonusTimeYear, pRecvMsg->siChangedBonusTimeMonth, pRecvMsg->siChangedBonusTimeDay
			);
		sDBLogMsg_QueryString LogMsg("Minister Goonzu Bonus Set", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
