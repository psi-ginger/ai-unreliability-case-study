#include "DBGameProtocol.h"
#include "DBMsg-Log.h"
#include "DBMsg-ItemMall.h"

void DBGameProtocol::BasicInfoByName()
{
	// 받은 패킷
	sDBRequest_BasicInfoByName *pRecvMsg = (sDBRequest_BasicInfoByName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_BasicInfoByName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GMTOOL_CHARBASICINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPAGENum = pRecvMsg->siPAGENum;
	sendMsg.siClientID = pRecvMsg->siClientID;
	MStrCpy(sendMsg.Charname, pRecvMsg->szCharname, sizeof(sendMsg.Charname));

	///////////////////////////////////////////////////////////////////////////////////////////
	// cltPBasicInfo
	///////////////////////////////////////////////////////////////////////////////////////////
	// DB Query
	BEGIN_SP( "rsp_GMTool_Basic" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharname, sizeof(pRecvMsg->szCharname));
	END_SP()

	EXEC_SQL()

	// tbl_Person
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.BasicInfo.uiIndex );
		m_odbc.GetData( &sendMsg.BasicInfo.uiKind );
		m_odbc.GetData( &sendMsg.BasicInfo.siPlaySecond );
		m_odbc.GetData( &sendMsg.BasicInfo.siPlayEventSecond );
		GetCharPos( &sendMsg.BasicInfo.clPos );
		m_odbc.GetData( &sendMsg.BasicInfo.clLastMainMapPos.siX );
		m_odbc.GetData( &sendMsg.BasicInfo.clLastMainMapPos.siY );
		m_odbc.GetData( &sendMsg.BasicInfo.siSchoolUnique );
		GetSimplePerson( &sendMsg.BasicInfo.clPerson);
		m_odbc.GetData( &sendMsg.BasicInfo.siVoteKingSelectionIndex );
	END_FETCHDATA()

	if(sendMsg.BasicInfo.clPerson.siPersonID > 0)
	{
		// tbl_Account
		BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.siIDNum );
				m_odbc.GetData( sendMsg.BasicInfo.szAccountID, sizeof(sendMsg.BasicInfo.szAccountID) );
				m_odbc.GetData( &sendMsg.BasicInfo.uiSex );
				GetSimplePerson( &sendMsg.BasicInfo.clRecommenderInfo);
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_Resident
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.siHomeVillage );
				m_odbc.GetData( &sendMsg.BasicInfo.clVillageScoreInfo.siPlayTime );
				m_odbc.GetData( &sendMsg.BasicInfo.clVillageScoreInfo.siLevelUpCount );
				m_odbc.GetData( &sendMsg.BasicInfo.clVillageScoreInfo.siWarKillNum );
				m_odbc.GetData( &sendMsg.BasicInfo.clVillageScoreInfo.siMakeRareItemCount );
				m_odbc.GetData( &sendMsg.BasicInfo.clVillageScoreInfo.siMonthlyScore );
				m_odbc.GetData( &sendMsg.BasicInfo.clVillageScoreInfo.siTotalScore );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_GRank
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.uiGRank );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_Angel
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.uiAngelSwitch );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_Police
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.uiPoliceSwitch );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbla_Assets
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.siWealthRank );
				m_odbc.GetData( &sendMsg.BasicInfo.clTotalWealthMoney.itMoney );
				m_odbc.GetData( &sendMsg.BasicInfo.siWealthCalcDay );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_PersonFather
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				GetFatherInfo( &sendMsg.BasicInfo.clFatherInfo );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_PersonStatus
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.bWennyOldManUpgrade );
				m_odbc.GetData( &sendMsg.BasicInfo.bQuestEvolve );
				m_odbc.GetData( sendMsg.BasicInfo.szGreeting, sizeof(sendMsg.BasicInfo.szGreeting) );
				m_odbc.GetData( &sendMsg.BasicInfo.bReceiveBeginnerWeapon);
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_TicketService
		UI08 uiServiceType = 0;
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &uiServiceType );
				switch (uiServiceType)
				{
				case NOLIMIT_TICKET:
					sendMsg.BasicInfo.bUseNoLimitTicketWarp = 1;
					m_odbc.GetData( &sendMsg.BasicInfo.siNoLimitTicketWarpDateVary );
					break;
				case MARKET_TICKET:
					sendMsg.BasicInfo.bMarKetPremiumService = 1;
					m_odbc.GetData( &sendMsg.BasicInfo.siMarKetPremiumServiceDate);
					break;
				case SPACEBOX_TICKET:
					sendMsg.BasicInfo.bSpaceBoxPremiumService = 1;
					m_odbc.GetData( &sendMsg.BasicInfo.siSpaceBoxPremiumServiceDate);
					break;
				case SUMMON_SPACE_TICKET:
					sendMsg.BasicInfo.bSummonExtraSlot = 1;
					m_odbc.GetData( &sendMsg.BasicInfo.siSummonExtraSlotDate);
					break;
				}
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_GuildMember, tbl_Guild
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				GetSimpleGuildRank( &sendMsg.BasicInfo.clGuildRank );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// tbl_PersonPremium
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.bPremiumService );
				m_odbc.GetData( &sendMsg.BasicInfo.siPremiumServiceDate );
				m_odbc.GetData( &sendMsg.BasicInfo.bCombatService );
				m_odbc.GetData( &sendMsg.BasicInfo.siCombatServiceDate );
				m_odbc.GetData( &sendMsg.BasicInfo.bMakeItemService );
				m_odbc.GetData( &sendMsg.BasicInfo.siMakeItemServiceDate );
				m_odbc.GetData( &sendMsg.BasicInfo.bBulletService );
				m_odbc.GetData( &sendMsg.BasicInfo.siBulletServiceDate );
				m_odbc.GetData( &sendMsg.BasicInfo.bSummonStaminaService );
				m_odbc.GetData( &sendMsg.BasicInfo.siSummonStaminaServiceDate );
				m_odbc.GetData( &sendMsg.BasicInfo.bSummonExtraSlot );
				m_odbc.GetData( &sendMsg.BasicInfo.siSummonStaminaServiceDate );
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// 버프정보
		SI32 siBufIndex = 0;
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData(&siBufIndex, 0); if(siBufIndex >= MAX_BUF_COUNT) continue;
				GetBufInfo(&sendMsg.BasicInfo.clBufInfo, siBufIndex);
			END_FETCHDATA()
		END_NEXTRECORDSET()

		// PVPResult
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.siPVPWinNumber);
				m_odbc.GetData( &sendMsg.BasicInfo.siPVPLoseNumber);
				m_odbc.GetData( sendMsg.BasicInfo.szPVPWinHighLevelUserName, sizeof(sendMsg.BasicInfo.szPVPWinHighLevelUserName) );
			END_FETCHDATA()
		END_NEXTRECORDSET()


		// From GZAccountDB
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.BasicInfo.bAttendToday);
				m_odbc.GetData( &sendMsg.BasicInfo.bNewAccount );
				m_odbc.GetData( &sendMsg.BasicInfo.bGameEvent );
				m_odbc.GetData( &sendMsg.BasicInfo.bWinPrizeEvent );
				m_odbc.GetData( &sendMsg.BasicInfo.siCountryID );
				m_odbc.GetData( &sendMsg.BasicInfo.bShowCountry );
				m_odbc.GetData( &sendMsg.BasicInfo.bShowPuchikomi );
//#ifdef USE_DBGW
//				ZeroMemory(&sendMsg.BasicInfo.siFirstCreatTimeDateVary, sizeof(sendMsg.BasicInfo.siFirstCreatTimeDateVary));
//#else
				m_odbc.GetData( &sendMsg.BasicInfo.siFirstCreatTimeDateVary);
//#endif

			END_FETCHDATA()
		END_NEXTRECORDSET()

		///////////////////////////////////////////////////////////////////////////////////////////
		// cltPIPInfo
		///////////////////////////////////////////////////////////////////////////////////////////
		BEGIN_SP( "rsp_IPGet" )
		m_odbc.SetParam( SQL_INTEGER, &sendMsg.BasicInfo.clPerson.siPersonID, sizeof(sendMsg.BasicInfo.clPerson.siPersonID) );
		END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
			GetPIPInfo( &sendMsg.clIp );
		END_FETCHDATA()

		///////////////////////////////////////////////////////////////////////////////////////////
		// cltHealthInfo
		///////////////////////////////////////////////////////////////////////////////////////////
		SI32 siDummyResult;
		BEGIN_SP( "rsp_HealthInfoGet" )
			m_odbc.SetParam( SQL_INTEGER, &sendMsg.BasicInfo.clPerson.siPersonID, sizeof(sendMsg.BasicInfo.clPerson.siPersonID) );
		END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
			m_odbc.GetData( &siDummyResult, 0 );
			m_odbc.GetData( &siDummyResult, 0 );
			GetHealthInfo( &sendMsg.clHealthInfo );
		END_FETCHDATA()

		///////////////////////////////////////////////////////////////////////////////////////////
		// cltPStockInfo
		///////////////////////////////////////////////////////////////////////////////////////////
		// cltPersonStockInfo	clStockInfo;
		// cltStockAccount		clInfo;
		int		i = 0;
		BEGIN_SP( "rsp_StockPersonAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &sendMsg.BasicInfo.clPerson.siPersonID, sizeof(sendMsg.BasicInfo.clPerson.siPersonID) );
		END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
			m_odbc.GetData( &(sendMsg.clStockInfo.clInfo.clUnit[i].siVillageUnique), 0);
			m_odbc.GetData( &(sendMsg.clStockInfo.clInfo.clUnit[i].siAmount), 0);
			m_odbc.GetData( &(sendMsg.clStockInfo.clInfo.clUnit[i].bBuySwitch), 0);
			m_odbc.GetData( &(sendMsg.clStockInfo.clInfo.clUnit[i].siLimitBuyBidStockAmount), 0);
			i++;
		END_FETCHDATA()

		// cltCharOrderInfo	clOrderInfo;
		i = 0;
		BEGIN_SP( "rsp_StockPersonOrderInfoGet" )
			m_odbc.SetParam( SQL_INTEGER, &sendMsg.BasicInfo.clPerson.siPersonID, sizeof(sendMsg.BasicInfo.clPerson.siPersonID) );
		END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
			m_odbc.GetData( &(sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].siOrderMode), 0);
			m_odbc.GetData( &(sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siIndex), 0);
			sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siType		= TRADEORDER_TYPE_STOCK;
			sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siPersonID	= sendMsg.BasicInfo.clPerson.siPersonID;
			m_odbc.GetData( &(sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siPrice), 0);
			m_odbc.GetData( &(sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siAmount), 0);
			m_odbc.GetData( &(sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siVillageUnique), 0);
			m_odbc.GetData( &(sendMsg.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siDate), 0);
			i++;
		END_FETCHDATA()

		///////////////////////////////////////////////////////////////////////////////////////////
		// cltBankAccount
		///////////////////////////////////////////////////////////////////////////////////////////
		SI64 itMoney = 0;
		BEGIN_SP( "rsp_BankAccountInfoGet" )
			m_odbc.SetParam( SQL_INTEGER, &sendMsg.BasicInfo.clPerson.siPersonID, sizeof(sendMsg.BasicInfo.clPerson.siPersonID) );
		END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clBankaccount.clAccountNum.siVillageUnique, 0);
			m_odbc.GetData( &sendMsg.clBankaccount.clAccountNum.siAccountNum, 0);
			m_odbc.GetData( &sendMsg.clBankaccount.siPersonID, 0);
			m_odbc.GetData( &itMoney, 0);	sendMsg.clBankaccount.SetMoney(itMoney);
		END_FETCHDATA()
	}

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::SkillByName()
{
	// 받은 패킷
	sDBRequest_SkillByName *pRecvMsg = (sDBRequest_SkillByName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SkillByName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GMTOOL_SKILLINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPAGENum = pRecvMsg->siPAGENum;
	sendMsg.siClientID = pRecvMsg->siClientID;
	MStrCpy(sendMsg.szCharname, pRecvMsg->szCharname, sizeof(sendMsg.szCharname));


	UI08	siSkillUnique;
	SI16	siItemPos;

	BEGIN_SP( "rsp_GMTool_Skill" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharname, sizeof(pRecvMsg->szCharname));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &siSkillUnique, 0); if(siSkillUnique >= MAX_SKILL_NUMBER) continue;
		m_odbc.GetData( &(sendMsg.SkillInfo.clSkill[siSkillUnique].uiLevel), 0);
		m_odbc.GetData( &(sendMsg.SkillInfo.clSkill[siSkillUnique].siSkillExp), 0);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siItemPos, 0);
			if(siItemPos >= MAX_ITEM_PER_PERSON)
				continue;	
			GetItemData( &sendMsg.clItemInfo.clItem[siItemPos]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::AnimalByName()
{
	// 받은 패킷
	sDBRequest_AnimalByName *pRecvMsg = (sDBRequest_AnimalByName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_AnimalByName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GMTOOL_ANIMALINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPAGENum = pRecvMsg->siPAGENum;
	sendMsg.siClientID = pRecvMsg->siClientID;
	MStrCpy(sendMsg.Charname, pRecvMsg->szCharname, sizeof(sendMsg.Charname));


	SI32 siHorseIndex = 0;

	BEGIN_SP( "rsp_GMTool_Animal" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharname, sizeof(pRecvMsg->szCharname));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.clHorseInfo.siCurrentShowHorse, 0 );
		m_odbc.GetData( &sendMsg.clHorseInfo.siCurrentHorseRIDEHORSE, 0 );
		m_odbc.GetData( &sendMsg.clHorseInfo.bAutoFoodSwitch, 0 );
	END_FETCHDATA()

	cltHorse clHorse;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siHorseIndex ); if(siHorseIndex >= MAX_HORSE_PER_PERSON) continue;
			GetHorseInfo( &clHorse );	
			sendMsg.clHorseInfo.SetHorse(siHorseIndex, &clHorse);		
			//GetHorseInfo( &sendMsg.clHorseInfo.clHorse[siHorseIndex] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::InventoryByName()
{
	// 받은 패킷
	sDBRequest_InventoryByName *pRecvMsg = (sDBRequest_InventoryByName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_InventoryByName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GMTOOL_INVENTORYINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPAGENum = pRecvMsg->siPAGENum;
	sendMsg.siClientID = pRecvMsg->siClientID;
	MStrCpy(sendMsg.Charname, pRecvMsg->szCharname, sizeof(sendMsg.Charname));

	SI16	siItemPos;

	BEGIN_SP( "rsp_GMTool_Inventory" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharname, sizeof(pRecvMsg->szCharname));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &siItemPos, 0);
		if(siItemPos >= MAX_ITEM_PER_PERSON)
			continue;	
		GetItemData( &sendMsg.clItemInfo.clItem[siItemPos]);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::RealestateByName()
{
	// 받은 패킷
	sDBRequest_RealestateByNAme *pRecvMsg = (sDBRequest_RealestateByNAme *)m_pPacket;

	// 보낼 패킷
	sDBResponse_RealestateByName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GMTOOL_REALESTATEINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPAGENum = pRecvMsg->siPAGENum;
    sendMsg.siClientID = pRecvMsg->siClientID;
	MStrCpy(sendMsg.Charname, pRecvMsg->szCharname, sizeof(sendMsg.Charname));

	SI32 siEstateIndex = 0;

	BEGIN_SP( "rsp_GMTool_Realestate" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharname, sizeof(pRecvMsg->szCharname));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		if( siEstateIndex >= MAX_REAL_ESTATE_NUMBER_PER_PERSON )
			break;
		sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siType = REALESTATE_TYPE_HOUSE;
		m_odbc.GetData( &sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siIndex, 0);
		GetRentContract( &(sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].clContract) );
		siEstateIndex++;
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if( siEstateIndex >= MAX_REAL_ESTATE_NUMBER_PER_PERSON )
				break;
			sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siType = REALESTATE_TYPE_LAND;
			m_odbc.GetData( &sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siVillageUnique, 0);
			m_odbc.GetData( &sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siIndex, 0);
			GetRentContract( &(sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].clContract) );
			siEstateIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if( siEstateIndex >= MAX_REAL_ESTATE_NUMBER_PER_PERSON )
				break;
			sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siType = REALESTATE_TYPE_LAND;
			m_odbc.GetData( &sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siVillageUnique, 0);
			m_odbc.GetData( &sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].siIndex, 0);
			GetRentContract( &(sendMsg.clRealEstateInfo.clRealEstate[siEstateIndex].clContract) );
			siEstateIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SummonByName()
{
	// 받은 패킷
	sDBRequest_SummonByName *pRecvMsg = (sDBRequest_SummonByName *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SummonByName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GMTOOL_SUMMONINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPAGENum = pRecvMsg->siPAGENum;
	sendMsg.siClientID = pRecvMsg->siClientID;
	MStrCpy(sendMsg.Charname, pRecvMsg->szCharname, sizeof(sendMsg.Charname));

	SI16 siSummonUnique = 0;

	BEGIN_SP( "rsp_GMTool_Summon" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharname, sizeof(pRecvMsg->szCharname));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.clSummonInfo.siSelectAutoHeal );
		m_odbc.GetData( &sendMsg.clSummonInfo.bAutoHeal );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siSummonUnique, 0 );
			if ( MAX_SUMMON_OWN <= siSummonUnique )		continue;
			GetSummon( &sendMsg.clSummonInfo.clSummon[siSummonUnique] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}
