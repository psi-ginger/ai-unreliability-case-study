#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-Cityhall.h"
#include "DBMsg-Stock.h"
#include "DBMsg-Person.h"
#include "DBMsg-Market.h"
#include "DBMsg-System.h"
#include "DBMsg-HorseMarket.h"
#include "DBMsg-RealEstateMarket.h"
#include "DBMsg-SummonMarket.h"
#include "DBMsg-Guild.h"
#include "DBMsg-itemmall.h"
#include "DBMsg-System.h"
#include "../../CommonLogic/Guild/GuildHuntMapWarManager.h"

void DBGameProtocol::SetItemParam(const cltItem *pclItem)
{
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->siUnique), sizeof(pclItem->siUnique));
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->siItemNum), sizeof(pclItem->siItemNum));
	m_odbc.SetParam( SQL_BINARY, (void*)&(pclItem->cData8), sizeof(pclItem->cData8));
	m_odbc.SetParam( SQL_TINYINT, (void*)&(pclItem->uiDateYear), sizeof(pclItem->uiDateYear));
	m_odbc.SetParam( SQL_TINYINT, (void*)&(pclItem->uiDateMonth), sizeof(pclItem->uiDateMonth));
	m_odbc.SetParam( SQL_TINYINT, (void*)&(pclItem->uiDateDay), sizeof(pclItem->uiDateDay));
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->uiDateUseDay), sizeof(pclItem->uiDateUseDay));
	m_odbc.SetParam( SQL_INTEGER, (void*)&(pclItem->siMakerPersonID), sizeof(pclItem->siMakerPersonID));
	m_odbc.SetParam( SQL_TINYINT, (void*)&(pclItem->Element.siType), sizeof(pclItem->Element.siType));
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->Element.siPower), sizeof(pclItem->Element.siPower));
	m_odbc.SetParam( SQL_INTEGER, (void*)&(pclItem->siSID), sizeof(pclItem->siSID));
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->siPalette), sizeof(pclItem->siPalette));
	m_odbc.SetParam( SQL_TINYINT, (void*)&(pclItem->siUseCrystalNum), sizeof(pclItem->siUseCrystalNum));
	for(SI32 i = 0; i < MAX_CRYSTAL_NUM; i++)
	{
		m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->cltItemCrytal[i].siCrystalType), sizeof(pclItem->cltItemCrytal[i].siCrystalType));
		m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->cltItemCrytal[i].siDurability), sizeof(pclItem->cltItemCrytal[i].siDurability));
		m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclItem->cltItemCrytal[i].siMaxDurability), sizeof(pclItem->cltItemCrytal[i].siMaxDurability));
		m_odbc.SetParam( SQL_TINYINT, (void*)&(pclItem->cltItemCrytal[i].uiCrystalAbility), sizeof(pclItem->cltItemCrytal[i].uiCrystalAbility));
		m_odbc.SetParam( SQL_TINYINT, (void*)&(pclItem->cltItemCrytal[i].uiCrystalAbilityNum), sizeof(pclItem->cltItemCrytal[i].uiCrystalAbilityNum));
	}
}

void DBGameProtocol::GetItemData(cltItem *pclItem)
{
	m_odbc.GetData( &(pclItem->siUnique), 0 );     
	m_odbc.GetData( &(pclItem->siItemNum), 0 );  
	m_odbc.GetData( (pclItem->cData8), sizeof(pclItem->cData8) );
	m_odbc.GetData( &(pclItem->uiDateYear), sizeof(pclItem->uiDateYear) );
	m_odbc.GetData( &(pclItem->uiDateMonth), sizeof(pclItem->uiDateMonth) );
	m_odbc.GetData( &(pclItem->uiDateDay), sizeof(pclItem->uiDateDay) );
	m_odbc.GetData( &(pclItem->uiDateUseDay), sizeof(pclItem->uiDateUseDay) );
	m_odbc.GetData( &(pclItem->siMakerPersonID), sizeof(pclItem->siMakerPersonID) );
	m_odbc.GetData( &(pclItem->Element.siType), sizeof(pclItem->Element.siType) );
	m_odbc.GetData( &(pclItem->Element.siPower), sizeof(pclItem->Element.siPower) );
	m_odbc.GetData( &(pclItem->siSID), sizeof(pclItem->siSID) );

	m_odbc.GetData( &pclItem->siPalette, sizeof(pclItem->siPalette));
	m_odbc.GetData( &pclItem->siUseCrystalNum, sizeof(pclItem->siUseCrystalNum));
	for(SI32 i = 0; i < MAX_CRYSTAL_NUM; i++)
	{
		m_odbc.GetData( &pclItem->cltItemCrytal[i].siCrystalType, sizeof(pclItem->cltItemCrytal[i].siCrystalType));
		m_odbc.GetData( &pclItem->cltItemCrytal[i].siDurability, sizeof(pclItem->cltItemCrytal[i].siDurability));
		m_odbc.GetData( &pclItem->cltItemCrytal[i].siMaxDurability, sizeof(pclItem->cltItemCrytal[i].siMaxDurability));
		m_odbc.GetData( &pclItem->cltItemCrytal[i].uiCrystalAbility, sizeof(pclItem->cltItemCrytal[i].uiCrystalAbility));
		m_odbc.GetData( &pclItem->cltItemCrytal[i].uiCrystalAbilityNum, sizeof(pclItem->cltItemCrytal[i].uiCrystalAbilityNum));
	}
}


void DBGameProtocol::SendMarketAccountInfo(SI32 siPersonID, UI16 uiToID)
{
	// 보낼 패킷
	sDBResponse_MarketAccount sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MARKETACCOUNT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = uiToID;

	// DB Query
	SI32	siMarketItemIndex;

	BEGIN_SP( "rsp_MarketPersonAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(siMarketItemIndex), 0 );
		m_odbc.GetData( &(sendMsg.clMarketAccount.clMoney[siMarketItemIndex]), 0 );
		GetItemData( &sendMsg.clMarketAccount.clItem[siMarketItemIndex] );
	END_FETCHDATA()
	
	sendMsg.siResult = 1;
	
	sendMsg.siPersonID = siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetSimplePerson( const class cltSimplePerson *pclPerson)
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclPerson->siPersonID, sizeof(pclPerson->siPersonID) );
	m_odbc.SetParam( SQL_VARCHAR, (void*)pclPerson->szName, sizeof(pclPerson->szName) );
}

void DBGameProtocol::GetSimplePerson(class cltSimplePerson *pclPerson)
{
	SI32 siPersonID = 0;
	char szName[MAX_PLAYER_NAME];
	ZeroMemory(szName, sizeof(szName));

	m_odbc.GetData( &siPersonID,	0 );
	m_odbc.GetData( szName,	sizeof(szName));

	pclPerson->SetPersonID(siPersonID);
	pclPerson->SetName(szName);
}


void DBGameProtocol::SetRentContract(const class cltRentContract *pclContract, cltGameDate *pclGameDate)
{
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclContract->siRentFee,		sizeof(pclContract->siRentFee) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclContract->siRentPeriod,	sizeof(pclContract->siRentPeriod) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siYear,			sizeof(pclGameDate->siYear) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siMonth,			sizeof(pclGameDate->siMonth) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siDay,			sizeof(pclGameDate->siDay) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siHour,			sizeof(pclGameDate->siHour) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siMinute,		sizeof(pclGameDate->siMinute) );
}


void DBGameProtocol::GetRentContract(class cltRentContract *pclContract)
{
	SI32	siYear, siMonth, siDay, siHour, siMinute;

	m_odbc.GetData( &pclContract->siRentFee,	0);
	m_odbc.GetData( &pclContract->siRentPeriod,	0);
	m_odbc.GetData( &siYear,	0);	pclContract->clDate.uiYear		= (UI32)siYear;
	m_odbc.GetData( &siMonth,	0);	pclContract->clDate.uiMonth		= (UI32)siMonth;
	m_odbc.GetData( &siDay,		0);	pclContract->clDate.uiDay		= (UI32)siDay;
	m_odbc.GetData( &siHour,	0);	pclContract->clDate.uiHour		= (UI32)siHour;
	m_odbc.GetData( &siMinute,	0);	pclContract->clDate.uiMinute	= (UI32)siMinute;
}


void DBGameProtocol::GetRentApplyer(class cltRentApplyer *pclApplyer)
{
	GetSimplePerson( &pclApplyer->clPerson);
	GetRentContract( &pclApplyer->clContract);
}


void DBGameProtocol::GetBankAccount(class cltBankAccount *pclBankAccount)
{
	GMONEY siMoney = 0;
	GMONEY itMoney = 0;
	m_odbc.GetData( &pclBankAccount->clAccountNum.siVillageUnique, 0);
	m_odbc.GetData( &pclBankAccount->clAccountNum.siAccountNum, 0);
	m_odbc.GetData( &pclBankAccount->siPersonID, 0);
	m_odbc.GetData( &itMoney, 0);	pclBankAccount->SetMoney(itMoney);
}


void DBGameProtocol::SendRankInfo(cltSimplePerson *pclPerson)
{
	// 보낼 패킷
	sDBResponse_RankInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RANKINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = 0;

	sendMsg.siResult = 1;
	sendMsg.clRank.clPerson.SetPersonID(pclPerson->GetPersonID());
	sendMsg.clRank.clPerson.SetName(pclPerson->GetName());
	sendMsg.clRank.clRank.SetRankType(0);
	sendMsg.clRank.clRank.SetVillageUnique(0);

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetSimpleRank(const class cltSimpleRank *pclRank)
{
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclRank->siType,	sizeof(pclRank->siType) );
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclRank->siVillageUnique,	sizeof(pclRank->siVillageUnique) );
}

void DBGameProtocol::GetSimpleRank( class cltSimpleRank *pclRank)
{
	m_odbc.GetData( &pclRank->siType,	0 );
	m_odbc.GetData( &pclRank->siVillageUnique,	0 );
}

void DBGameProtocol::SetBasicAbility( const class cltBasicAbility *pclBA )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclBA->siStr, sizeof(pclBA->siStr) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclBA->siDex, sizeof(pclBA->siDex) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclBA->siVit, sizeof(pclBA->siVit) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclBA->siMag, sizeof(pclBA->siMag) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclBA->siHnd, sizeof(pclBA->siHnd) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclBA->siLuk, sizeof(pclBA->siLuk) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclBA->siWis, sizeof(pclBA->siWis) );
}

void DBGameProtocol::GetBasicAbility( class cltBasicAbility *pclBA )
{
	m_odbc.GetData( &pclBA->siStr, 0 );
	m_odbc.GetData( &pclBA->siDex, 0 );
	m_odbc.GetData( &pclBA->siVit, 0 );
	m_odbc.GetData( &pclBA->siMag, 0 );
	m_odbc.GetData( &pclBA->siHnd, 0 );
	m_odbc.GetData( &pclBA->siLuk, 0 );
	m_odbc.GetData( &pclBA->siWis, 0 );
}

void DBGameProtocol::SetHealthInfo( const class cltHealthInfo *pclHI )
{
	//------------------------
	// 기초 상태 
	//------------------------
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclHI->siHungry, sizeof(pclHI->siHungry));

	//-------------------------
	// 기관 상태 
	//-------------------------
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->siInnerOrg1, sizeof(pclHI->siInnerOrg1));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->siInnerOrg2, sizeof(pclHI->siInnerOrg2));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->siInnerOrg3, sizeof(pclHI->siInnerOrg3));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->siInnerOrg4, sizeof(pclHI->siInnerOrg4));

	//------------------------
	// 질병 상태 
	//------------------------
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->uiOrg1Disease, sizeof(pclHI->uiOrg1Disease));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->uiOrg2Disease, sizeof(pclHI->uiOrg2Disease));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->uiOrg3Disease, sizeof(pclHI->uiOrg3Disease));
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->uiOrg4Disease, sizeof(pclHI->uiOrg4Disease));

	//-----------------------
	// 종합 건강 지수.
	//-----------------------
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclHI->uiHealthPoint, sizeof(pclHI->uiHealthPoint));
}

void DBGameProtocol::GetHealthInfo( class cltHealthInfo *pclHI )
{
	//------------------------
	// 기초 상태 
	//------------------------
	m_odbc.GetData( &pclHI->siHungry,		0);

	//-------------------------
	// 기관 상태 
	//-------------------------
	m_odbc.GetData( &pclHI->siInnerOrg1,	0);
	m_odbc.GetData( &pclHI->siInnerOrg2,	0);
	m_odbc.GetData( &pclHI->siInnerOrg3,	0);
	m_odbc.GetData( &pclHI->siInnerOrg4,	0);

	//------------------------
	// 질병 상태 
	//------------------------
	m_odbc.GetData( &pclHI->uiOrg1Disease,	0);
	m_odbc.GetData( &pclHI->uiOrg2Disease,	0);
	m_odbc.GetData( &pclHI->uiOrg3Disease,	0);
	m_odbc.GetData( &pclHI->uiOrg4Disease,	0);

	//-----------------------
	// 종합 건강 지수.
	//-----------------------
	m_odbc.GetData( &pclHI->uiHealthPoint,	0);
}


void DBGameProtocol::SetCharPos( const class cltCharPos *pclPos )
{
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclPos->siMapIndex, sizeof(pclPos->siMapIndex) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclPos->siX, sizeof(pclPos->siX) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclPos->siY, sizeof(pclPos->siY) );
}
void DBGameProtocol::GetCharPos( class cltCharPos *pclPos )
{
	m_odbc.GetData( &pclPos->siMapIndex, 0 );
	m_odbc.GetData( &pclPos->siX, 0 );
	m_odbc.GetData( &pclPos->siY, 0 );
}


void DBGameProtocol::SetPos( const class cltPos *pclPos )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclPos->siX, sizeof(pclPos->siX) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclPos->siY, sizeof(pclPos->siY) );
}
void DBGameProtocol::GetPos( class cltPos *pclPos )
{
	m_odbc.GetData( &pclPos->siX, 0 );
	m_odbc.GetData( &pclPos->siY, 0 );
}


void DBGameProtocol::SetQuestUnitInfo( const class cltPersonQuestUnitInfo *pclQU )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&(pclQU->siQuestState), sizeof(pclQU->siQuestState) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclQU->siCurQuestUnique), sizeof(pclQU->siCurQuestUnique) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclQU->siCurQuestStep), sizeof(pclQU->siCurQuestStep) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclQU->siQuestCmd), sizeof(pclQU->siQuestCmd) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclQU->siQuestPara1), sizeof(pclQU->siQuestPara1) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&(pclQU->siQuestPara2), sizeof(pclQU->siQuestPara2) );

}

void DBGameProtocol::GetQuestUnitInfo( class cltPersonQuestUnitInfo *pclQU )
{
	m_odbc.GetData( &pclQU->siQuestState, 0 );
	m_odbc.GetData( &pclQU->siCurQuestUnique, 0 );
	m_odbc.GetData( &pclQU->siCurQuestStep, 0 );
	m_odbc.GetData( &pclQU->siQuestCmd, 0 );
	m_odbc.GetData( &pclQU->siQuestPara1, 0 );
	m_odbc.GetData( &pclQU->siQuestPara2, 0 );
}

void DBGameProtocol::SetQuestInfoCommon( const class cltQuestInfoCommon *pclQC)
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclQC->siUnique, sizeof(pclQC->siUnique) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclQC->clQuestClearConditionInfo.siFlag, sizeof(pclQC->clQuestClearConditionInfo.siFlag) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclQC->clQuestClearConditionInfo.siPara1, sizeof(pclQC->clQuestClearConditionInfo.siPara1) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclQC->clQuestClearConditionInfo.siPara2, sizeof(pclQC->clQuestClearConditionInfo.siPara2) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclQC->clQuestRewardInfo.siMoney, sizeof(pclQC->clQuestRewardInfo.siMoney) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclQC->clQuestRewardInfo.siExp, sizeof(pclQC->clQuestRewardInfo.siExp) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclQC->clQuestRewardInfo.siItemUnique[0], sizeof(pclQC->clQuestRewardInfo.siItemUnique[0]) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclQC->clQuestRewardInfo.siItemNum[0], sizeof(pclQC->clQuestRewardInfo.siItemNum[0]) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclQC->clQuestRewardInfo.siFameExp, sizeof(pclQC->clQuestRewardInfo.siFameExp) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclQC->clQuestRewardInfo.siNCoin, sizeof(pclQC->clQuestRewardInfo.siNCoin) );
}

void DBGameProtocol::GetQuestInfoCommon( class cltQuestInfoCommon *pclQC)
{
	m_odbc.GetData( &pclQC->siUnique, 0 );
	m_odbc.GetData( &pclQC->clQuestClearConditionInfo.siFlag, 0 );
	m_odbc.GetData( &pclQC->clQuestClearConditionInfo.siPara1, 0 );
	m_odbc.GetData( &pclQC->clQuestClearConditionInfo.siPara2, 0 );
	m_odbc.GetData( &pclQC->clQuestRewardInfo.siMoney, 0 );
	m_odbc.GetData( &pclQC->clQuestRewardInfo.siExp, 0 );
	m_odbc.GetData( &pclQC->clQuestRewardInfo.siItemUnique[0], 0 );
	m_odbc.GetData( &pclQC->clQuestRewardInfo.siItemNum[0], 0 );
	m_odbc.GetData( &pclQC->clQuestRewardInfo.siFameExp, 0 );
	m_odbc.GetData( &pclQC->clQuestRewardInfo.siNCoin, 0 );
}

void DBGameProtocol::SetQuestWordAssemblySwitch(const bool *pbWAS)
{
	for(int i=0; i<MAX_WORD_ASSEMBLY_CHAR_NUMBER; i++)
	{
		m_odbc.SetParam( SQL_TINYINT, (void*)&pbWAS[i], sizeof(pbWAS[i]));
	}
}

void DBGameProtocol::GetQuestWordAssemblySwitch(bool *pbWAS)
{
	for(int i=0; i<MAX_WORD_ASSEMBLY_CHAR_NUMBER; i++)
	{
		m_odbc.GetData( &pbWAS[i], 0);
	}
}

void DBGameProtocol::SetGameDate(const class cltGameDate *pclGameDate)
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siYear,		sizeof(pclGameDate->siYear) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siMonth,		sizeof(pclGameDate->siMonth) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siDay,		sizeof(pclGameDate->siDay) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siHour,		sizeof(pclGameDate->siHour) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclGameDate->siMinute,	sizeof(pclGameDate->siMinute) );
}


void DBGameProtocol::GetGameDate(cltDate *pclDate)
{
	SI32	siYear, siMonth, siDay, siHour, siMinute;

	m_odbc.GetData( &siYear,	0);	pclDate->uiYear		= (UI32)siYear;
	m_odbc.GetData( &siMonth,	0);	pclDate->uiMonth	= (UI32)siMonth;
	m_odbc.GetData( &siDay,		0);	pclDate->uiDay		= (UI32)siDay;
	m_odbc.GetData( &siHour,	0);	pclDate->uiHour		= (UI32)siHour;
	m_odbc.GetData( &siMinute,	0);	pclDate->uiMinute	= (UI32)siMinute;
}

void DBGameProtocol::SetHorseInfo( const class cltHorse *pclHorse, const SI32 *psiBirthDay, const SI32 *psiAddStatusDate
								  , const SI32 *psiPremiumPartsDate_Slot1 , const SI32 *psiPremiumPartsDate_Slot2, const SI32 *psiPremiumPartsDate_Slot3)
{
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siHorseUnique	, sizeof(pclHorse->siHorseUnique) );
	m_odbc.SetParam( SQL_VARCHAR	, (void*)pclHorse->szName			, sizeof(pclHorse->szName) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)psiBirthDay				, sizeof(*psiBirthDay) );

	m_odbc.SetParam( SQL_TINYINT	, (void*)&pclHorse->siSex			, sizeof(pclHorse->siSex) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siLife			, sizeof(pclHorse->siLife) );
	m_odbc.SetParam( SQL_TINYINT	, (void*)&pclHorse->siPregnentPeriod, sizeof(pclHorse->siPregnentPeriod) );

	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siStr			, sizeof(pclHorse->siStr) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siDex			, sizeof(pclHorse->siDex) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siMag			, sizeof(pclHorse->siMag) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siMoveSpeed		, sizeof(pclHorse->siMoveSpeed) );

	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siLastUpdateSecond, sizeof(pclHorse->siLastUpdateSecond) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siLastNeedTime	, sizeof(pclHorse->siLastNeedTime) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siDueTime		, sizeof(pclHorse->siDueTime) );

	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siNeed			, sizeof(pclHorse->siNeed) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siPara1			, sizeof(pclHorse->siPara1) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siPara2			, sizeof(pclHorse->siPara2) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siSupply		, sizeof(pclHorse->siSupply) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siRate			, sizeof(pclHorse->siRate) );

	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siRewardType	, sizeof(pclHorse->siRewardType) );
	m_odbc.SetParam( SQL_SMALLINT	, (void*)&pclHorse->siRewardAmount	, sizeof(pclHorse->siRewardAmount) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siStatusBonus	, sizeof(pclHorse->siStatusBonus) );

	m_odbc.SetParam( SQL_INTEGER	, (void*)psiAddStatusDate	, sizeof(*psiAddStatusDate) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siStatusTicketNum	, sizeof(pclHorse->siStatusTicketNum) );

	// 말튜닝 관련 추가.
	m_odbc.SetParam( SQL_INTEGER	, (void*)psiPremiumPartsDate_Slot1	, sizeof(*psiPremiumPartsDate_Slot1) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siPremiumUnique_Slot1	, sizeof(pclHorse->siPremiumUnique_Slot1) );

	m_odbc.SetParam( SQL_INTEGER	, (void*)psiPremiumPartsDate_Slot2	, sizeof(*psiPremiumPartsDate_Slot2) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siPremiumUnique_Slot2	, sizeof(pclHorse->siPremiumUnique_Slot2) );

	m_odbc.SetParam( SQL_INTEGER	, (void*)psiPremiumPartsDate_Slot3	, sizeof(*psiPremiumPartsDate_Slot3) );
	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siPremiumUnique_Slot3	, sizeof(pclHorse->siPremiumUnique_Slot3) );

	m_odbc.SetParam( SQL_INTEGER	, (void*)&pclHorse->siPremiumPartsNum	, sizeof(pclHorse->siPremiumPartsNum) );
}

void DBGameProtocol::GetHorseInfo( class cltHorse *pclHorse )
{
	SI32 siBirthDay = 0;
	SI32 siAddStatusDate = 0;

	SI32 siAddPremiumPartsDate_Slot1 = 0;
	SI32 siAddPremiumPartsDate_Slot2 = 0;
	SI32 siAddPremiumPartsDate_Slot3 = 0;

	m_odbc.GetData( &pclHorse->siHorseUnique, 0 );
	m_odbc.GetData( pclHorse->szName, sizeof(pclHorse->szName) );

	m_odbc.GetData( &siBirthDay, 0 );	pclHorse->clBirthDate.MakeFullDateFromDateVary(siBirthDay);

	m_odbc.GetData( &pclHorse->siSex, 0 );
	m_odbc.GetData( &pclHorse->siLife, 0 );
	m_odbc.GetData( &pclHorse->siPregnentPeriod, 0 );

	m_odbc.GetData( &pclHorse->siStr, 0 );
	m_odbc.GetData( &pclHorse->siDex, 0 );
	m_odbc.GetData( &pclHorse->siMag, 0 );
	m_odbc.GetData( &pclHorse->siMoveSpeed, 0 );

	m_odbc.GetData( &pclHorse->siLastUpdateSecond, 0 );
	m_odbc.GetData( &pclHorse->siLastNeedTime, 0 );
	m_odbc.GetData( &pclHorse->siDueTime, 0 );

	m_odbc.GetData( &pclHorse->siNeed, 0 );
	m_odbc.GetData( &pclHorse->siPara1, 0 );
	m_odbc.GetData( &pclHorse->siPara2, 0 );
	m_odbc.GetData( &pclHorse->siSupply, 0 );
	m_odbc.GetData( &pclHorse->siRate, 0 );

	m_odbc.GetData( &pclHorse->siRewardType, 0 );
	m_odbc.GetData( &pclHorse->siRewardAmount, 0 );
	m_odbc.GetData( &pclHorse->siStatusBonus, 0 );

	m_odbc.GetData( &siAddStatusDate, sizeof(siAddStatusDate) ); pclHorse->clAddStatusDate.MakeFullDateFromDateVary(siAddStatusDate);
	m_odbc.GetData( &pclHorse->siStatusTicketNum, 0 );
	
	// 말 프리미엄 파츠 정보 가지고 온다.
	m_odbc.GetData( &siAddPremiumPartsDate_Slot1, sizeof(siAddPremiumPartsDate_Slot1) ); pclHorse->clPremiumPartsDate_Slot1.MakeFullDateFromDateVary(siAddPremiumPartsDate_Slot1);
	m_odbc.GetData( &pclHorse->siPremiumUnique_Slot1, 0 );
	m_odbc.GetData( &siAddPremiumPartsDate_Slot2, sizeof(siAddPremiumPartsDate_Slot2) ); pclHorse->clPremiumPartsDate_Slot2.MakeFullDateFromDateVary(siAddPremiumPartsDate_Slot2);
	m_odbc.GetData( &pclHorse->siPremiumUnique_Slot2, 0 );
	m_odbc.GetData( &siAddPremiumPartsDate_Slot3, sizeof(siAddPremiumPartsDate_Slot3) ); pclHorse->clPremiumPartsDate_Slot3.MakeFullDateFromDateVary(siAddPremiumPartsDate_Slot3);
	m_odbc.GetData( &pclHorse->siPremiumUnique_Slot3, 0 );
	m_odbc.GetData( &pclHorse->siPremiumPartsNum, 0 );
}

void DBGameProtocol::SetFeeInfo( const class cltDBFee *pclFee )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclFee->siFeeRate, sizeof(pclFee->siFeeRate) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclFee->siMaxFee, sizeof(pclFee->siMaxFee) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclFee->siMinFee, sizeof(pclFee->siMinFee) );
}

void DBGameProtocol::GetFeeInfo( class cltFee *pclFee )
{
	cltDBFee clDBFee;
	m_odbc.GetData( &clDBFee.siFeeRate, 0 );
	m_odbc.GetData( &clDBFee.siMaxFee, 0 );
	m_odbc.GetData( &clDBFee.siMinFee, 0 );

	pclFee->SetFeeRate( clDBFee.siFeeRate );
	pclFee->SetMaxFee( clDBFee.siMaxFee );
	pclFee->SetMinFee( clDBFee.siMinFee );
}

void DBGameProtocol::GetFeeInfoNew( class cltFee *pclFee )
{
	cltDBFee clDBFee;
	m_odbc.GetData( &clDBFee.siFeeRate, 0 );
	m_odbc.GetData( &clDBFee.siMaxFee,  0 );
	m_odbc.GetData( &clDBFee.siMinFee,  0 );

	pclFee->SetFeeRateNew( clDBFee.siFeeRate );
	pclFee->SetMaxFeeNew(  clDBFee.siMaxFee  );
	pclFee->SetMinFeeNew(  clDBFee.siMinFee  );
}

void DBGameProtocol::SetChildSkillList( const class sDBRequest_DecreaseSkill *pclSkill )
{
	for( int i = 0; i < MAX_CHILD_SKILL_NUM; i++ )
	{
		m_odbc.SetParam( SQL_TINYINT, (void*)&pclSkill->uiChildSkillList[i],	sizeof(pclSkill->uiChildSkillList[i]) );
	}
}

void DBGameProtocol::GetResidentApplyerInfo( class cltResidentApplyer *clApplyer )
{
	GetSimplePerson( &clApplyer->clApplyer );
	m_odbc.GetData( &clApplyer->siLevel, 0 );
	GetGameDate( &clApplyer->clDate );
}

void DBGameProtocol::GetResidentInfo( class cltResident *clApplyer )
{
	GetSimplePerson( &clApplyer->clPerson);
	m_odbc.GetData( &clApplyer->siGrade, 0 );
	GetGameDate( &clApplyer->clDate );
	//level
	m_odbc.GetData( &clApplyer->siLevel, 0);
	// 마지막 접속시간...
	SI32 siDate;
	m_odbc.GetData( &siDate, 0);
	clApplyer->clLastLoginDate.MakeFullDateFromDateVary(siDate);
}

void DBGameProtocol::GetStockBidder( class cltStockBidder *pclStockBidder )
{
	GetSimplePerson( &pclStockBidder->clPerson );
	m_odbc.GetData( &pclStockBidder->siVillageUnique, 0 );
	m_odbc.GetData( &pclStockBidder->siAmount, 0 );
	m_odbc.GetData( &pclStockBidder->siPrice, 0 );
	GetGameDate( &pclStockBidder->clDate );
}

void DBGameProtocol::SetFatherInfo( const class cltPFatherInfo *pclFather )
{
	SetSimplePerson( &pclFather->clFather);
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclFather->siMyLevelForFather, sizeof(pclFather->siMyLevelForFather) );
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclFather->siFatherChangeNumber, sizeof(pclFather->siFatherChangeNumber) );
}

void DBGameProtocol::GetFatherInfo( class cltPFatherInfo *pclFather )
{
	GetSimplePerson( &pclFather->clFather);
	m_odbc.GetData( pclFather->szAccountID, sizeof(pclFather->szAccountID) );
	m_odbc.GetData( &pclFather->siMyLevelForFather, 0 );
	m_odbc.GetData( &pclFather->siFatherChangeNumber, 0 );
	m_odbc.GetData( &pclFather->siTeacherPoint, 0 );
	m_odbc.GetData( &pclFather->bOverTake, 0 );
}

// 마을자금에서 주민배당금을 뺀다
void DBGameProtocol::ResidentShareSetVillage( SI32 siVillageUnique, GMONEY Money, SI32 *psiResult, GMONEY *pitMoney )
{
	// DB Query
	BEGIN_SP( "rsp_ResidentShareSetVillage" )
		m_odbc.SetParam( SQL_INTEGER, &siVillageUnique, sizeof(siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &Money, sizeof(Money) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( psiResult, 0 );
		m_odbc.GetData( pitMoney, 0 );
	END_FETCHDATA()

	// 로그 작성
	//if(*psiResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, VillageUnique = %d, TotalShareMoney = %I64d, LeftCityHallMoney = %I64d"
				,	psiResult, siVillageUnique, Money, *pitMoney
				);
		sDBLogMsg_QueryString LogMsg("ResidentShare Set Village", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 주민배당금을 주민에게 준다
void DBGameProtocol::ResidentShareSetPerson( SI32 siVillageUnique, SI32 PersonID, GMONEY Money, GMONEY *psiMoney )
{
	// DB Query
	BEGIN_SP( "rsp_ResidentShareSetPerson" )
		m_odbc.SetParam( SQL_INTEGER, &PersonID, sizeof(PersonID) );
		m_odbc.SetParam( SQL_BIGINT, &Money, sizeof(Money) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( psiMoney, 0 );
	END_FETCHDATA()

	// 로그 작성
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ShareMoney = %I64d, LeftPersonMoney = %I64d"
				, siVillageUnique, Money, *psiMoney
				);
		sDBLogMsg_QueryString LogMsg("ResidentShare Set Person", PersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetMinisterBase( class CMinisterBase *pclMB)
{
	m_odbc.GetData( pclMB->m_szAccountID, sizeof(pclMB->m_szAccountID) );
	m_odbc.GetData( pclMB->m_szPersonName, sizeof(pclMB->m_szPersonName) );
	m_odbc.GetData( pclMB->m_szGreeting, sizeof(pclMB->m_szGreeting) );
}

void DBGameProtocol::GetHorseOrder( class cltHorseOrder *pclHorseOrder )
{
	SI32 siOrderDateVary;
	m_odbc.GetData( &pclHorseOrder->siIndex, 0 );
	m_odbc.GetData( &pclHorseOrder->siPersonID, 0 );
	m_odbc.GetData( &pclHorseOrder->siPrice, 0 );
	m_odbc.GetData( &siOrderDateVary, 0 ); pclHorseOrder->clDate.MakeFullDateFromDateVary(siOrderDateVary);
	GetHorseInfo( &pclHorseOrder->clHorse );
}

void DBGameProtocol::SetHorseOrder( const class cltHorseOrder *pclHorseOrder, const SI32 *psiOrderDay, const SI32 *psiBirthDay, const SI32 *psiAddStatusDate
								   , const SI32 *psiPremiumPartsDate_Slot1, const SI32 *psiPremiumPartsDate_Slot2, const SI32 *psiPremiumPartsDate_Slot3)
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclHorseOrder->siIndex, sizeof(pclHorseOrder->siIndex) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclHorseOrder->siPersonID, sizeof(pclHorseOrder->siPersonID) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclHorseOrder->siPrice, sizeof(pclHorseOrder->siPrice) );
	m_odbc.SetParam( SQL_INTEGER, (void*)psiOrderDay, sizeof(*psiOrderDay) );
	SetHorseInfo( &pclHorseOrder->clHorse, psiBirthDay, psiAddStatusDate
		,psiPremiumPartsDate_Slot1,psiPremiumPartsDate_Slot2,psiPremiumPartsDate_Slot3);
}

void DBGameProtocol::SetSummon( const class cltSummon *pclSummon )
{
	m_odbc.SetParam( SQL_INTEGER,	(void*)&pclSummon->siSummonID,				sizeof(pclSummon->siSummonID));
	m_odbc.SetParam( SQL_SMALLINT,  (void*)&pclSummon->siKind,					sizeof(pclSummon->siKind));
	m_odbc.SetParam( SQL_VARCHAR,	(void*)pclSummon->szName,					sizeof(pclSummon->szName));
	m_odbc.SetParam( SQL_TINYINT,	(void*)&pclSummon->siStatus,				sizeof(pclSummon->siStatus));
	m_odbc.SetParam( SQL_TINYINT,	(void*)&pclSummon->siAttackType,			sizeof(pclSummon->siAttackType));
	m_odbc.SetParam( SQL_TINYINT,	(void*)&pclSummon->siIdentity,				sizeof(pclSummon->siIdentity));
	SetPIPInfoCommon( &pclSummon->clIP );
	m_odbc.SetParam( SQL_SMALLINT,	(void*)&pclSummon->clScroll.siUnique[0],  	sizeof(pclSummon->clScroll.siUnique[0]));
	m_odbc.SetParam( SQL_SMALLINT,	(void*)&pclSummon->clScroll.siPercent[0], 	sizeof(pclSummon->clScroll.siPercent[0]));
	m_odbc.SetParam( SQL_SMALLINT,	(void*)&pclSummon->clScroll.siUnique[1],	sizeof(pclSummon->clScroll.siUnique[1]));
	m_odbc.SetParam( SQL_SMALLINT,	(void*)&pclSummon->clScroll.siPercent[1],	sizeof(pclSummon->clScroll.siPercent[1]));
	m_odbc.SetParam( SQL_INTEGER,	(void*)&pclSummon->siTakeInMinute,			sizeof(pclSummon->siTakeInMinute));
	m_odbc.SetParam( SQL_SMALLINT,	(void*)&pclSummon->siAttackWearStoneNum,	sizeof(pclSummon->siAttackWearStoneNum));
	m_odbc.SetParam( SQL_SMALLINT,	(void*)&pclSummon->siDefenseWearStoneNum,	sizeof(pclSummon->siDefenseWearStoneNum));
	m_odbc.SetParam( SQL_SMALLINT,	(void*)&pclSummon->m_siTranformStamina,		sizeof(pclSummon->m_siTranformStamina));//[진성] 소환수 변신 스테미너 정보.
}

void DBGameProtocol::GetSummon( class cltSummon *pclSummon )
{
	SI32 siDateVary = 0;
	m_odbc.GetData( &pclSummon->siSummonID,				0 );
	m_odbc.GetData( &pclSummon->siKind,					0 );
	m_odbc.GetData( pclSummon->szName, sizeof(pclSummon->szName) );
	m_odbc.GetData( &pclSummon->siStatus,				0 );
	m_odbc.GetData( &pclSummon->siAttackType,			0 );
	m_odbc.GetData( &pclSummon->siIdentity,				0 );
	GetPIPInfoCommon( &pclSummon->clIP );
	m_odbc.GetData( &pclSummon->clScroll.siUnique[0],	0 );
	m_odbc.GetData( &pclSummon->clScroll.siPercent[0],	0 );
	m_odbc.GetData( &pclSummon->clScroll.siUnique[1],	0 );
	m_odbc.GetData( &pclSummon->clScroll.siPercent[1],	0 );
	m_odbc.GetData( &pclSummon->siTakeInMinute,			0 );
	m_odbc.GetData( &pclSummon->siAttackWearStoneNum,	0 );
	m_odbc.GetData( &pclSummon->siDefenseWearStoneNum,	0 );
	m_odbc.GetData( &pclSummon->m_siTranformStamina,	0 );	//[진성] 소환수 변신 스테미너 정보.
}

void DBGameProtocol::SetSummonOrder( const class cltSummonOrder *pclOrder, const int *psiOrderDateVary )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclOrder->siIndex, sizeof(pclOrder->siIndex) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclOrder->siPersonID, sizeof(pclOrder->siPersonID) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclOrder->siPrice, sizeof(pclOrder->siPrice) );
	m_odbc.SetParam( SQL_INTEGER, (void*)psiOrderDateVary, sizeof(*psiOrderDateVary) );
	SetSummon( &pclOrder->clSummon );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclOrder->siSelectPersonID, sizeof(pclOrder->siSelectPersonID) );
}

void DBGameProtocol::GetSummonOrder( class cltSummonOrder *pclOrder )
{
	SI32 siDateVary = 0;
	m_odbc.GetData( &pclOrder->siIndex, 0 );
	m_odbc.GetData( &pclOrder->siPersonID, 0 );
	m_odbc.GetData( &pclOrder->siPrice, 0 );
	m_odbc.GetData( &siDateVary, 0 ); pclOrder->clDate.MakeFullDateFromDateVary(siDateVary);
	GetSummon( &pclOrder->clSummon );
	m_odbc.GetData( &pclOrder->siSelectPersonID, 0 );
}

void DBGameProtocol::GetSimpleSummon( class cltSimpleSummon *pclSimpleSummon )
{
	m_odbc.GetData( &pclSimpleSummon->siSummonID );
	m_odbc.GetData( &pclSimpleSummon->siKind );
	m_odbc.GetData( &pclSimpleSummon->siLevel );
	m_odbc.GetData( pclSimpleSummon->szName, sizeof(pclSimpleSummon->szName) );
}


void DBGameProtocol::SetPIPInfoCommon( const class cltPIPInfoCommon *pclInfoCommon )
{
	SetBasicAbility( pclInfoCommon );

	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfoCommon->siBonus, sizeof(pclInfoCommon->siBonus));

	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfoCommon->Life, sizeof(pclInfoCommon->Life));
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfoCommon->Mana, sizeof(pclInfoCommon->Mana));
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclInfoCommon->siExp, sizeof(pclInfoCommon->siExp));
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfoCommon->siLevel, sizeof(pclInfoCommon->siLevel));
}

void DBGameProtocol::GetPIPInfoCommon( class cltPIPInfoCommon *pclInfoCommon )
{
	GetBasicAbility( pclInfoCommon );

	m_odbc.GetData( &pclInfoCommon->siBonus, 0 );
	m_odbc.GetData( &pclInfoCommon->Life, 0 );
	m_odbc.GetData( &pclInfoCommon->Mana, 0 );
	m_odbc.GetData( &pclInfoCommon->siExp, 0 );
	m_odbc.GetData( &pclInfoCommon->siLevel, 0 );
}

void DBGameProtocol::SetPIPInfo( const class cltPIPInfo *pclInfo )
{
	SetPIPInfoCommon( pclInfo );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->siSkillBonus, sizeof(pclInfo->siSkillBonus) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siFameExp, sizeof(pclInfo->siFameExp) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->siFameLevel, sizeof(pclInfo->siFameLevel) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclInfo->siMoney, sizeof(pclInfo->siMoney) );
}

void DBGameProtocol::GetPIPInfo( class cltPIPInfo *pclInfo )
{
	GetPIPInfoCommon( pclInfo );
	m_odbc.GetData( &pclInfo->siSkillBonus );
	m_odbc.GetData( &pclInfo->siFameExp );
	m_odbc.GetData( &pclInfo->siFameLevel );
	m_odbc.GetData( &pclInfo->siMoney );
}


void DBGameProtocol::SetLevelInfo( const class cltLevelInfo *pclInfo )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siExp, sizeof(pclInfo->siExp) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->siLevel, sizeof(pclInfo->siLevel) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->siBonus, sizeof(pclInfo->siBonus) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->siSkillBonus, sizeof(pclInfo->siSkillBonus) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siFameExp, sizeof(pclInfo->siFameExp) );
}

void DBGameProtocol::GetLevelInfo( class cltLevelInfo *pclInfo )
{
	m_odbc.GetData( &pclInfo->siExp, 0 );
	m_odbc.GetData( &pclInfo->siLevel, 0 );
	m_odbc.GetData( &pclInfo->siBonus, 0 );
	m_odbc.GetData( &pclInfo->siSkillBonus, 0 );
	m_odbc.GetData( &pclInfo->siFameExp, 0 );
}


void DBGameProtocol::SetRealEstateOrderInfo( const class cltRealEstateOrder *pclInfo, const SI32 *psiDateVary )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siIndex, sizeof(pclInfo->siIndex) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siPersonID, sizeof(pclInfo->siPersonID) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclInfo->siPrice, sizeof(pclInfo->siPrice) );
	m_odbc.SetParam( SQL_INTEGER, (void*)psiDateVary, sizeof(*psiDateVary) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siVillageUnique, sizeof(pclInfo->siVillageUnique) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siHouseIndex, sizeof(pclInfo->siHouseIndex) );
}

void DBGameProtocol::GetRealEstateOrderInfo( class cltRealEstateOrder *pclInfo )
{
	SI32 siDateVary = 0;
	m_odbc.GetData( &pclInfo->siIndex, 0 );
	m_odbc.GetData( &pclInfo->siPersonID, 0 );
	m_odbc.GetData( &pclInfo->siPrice, 0 );
	m_odbc.GetData( &siDateVary, 0 ); pclInfo->clDate.MakeFullDateFromDateVary( siDateVary );
	m_odbc.GetData( &pclInfo->siVillageUnique, 0 );
	m_odbc.GetData( &pclInfo->siHouseIndex, 0 );
}

void DBGameProtocol::SetGuiltyInfo( const class cltGuiltyInfo *pclInfo )
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siTotalPrisonSecond, sizeof(pclInfo->siTotalPrisonSecond) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siLeftPrisonSecond, sizeof(pclInfo->siLeftPrisonSecond) );
	m_odbc.SetParam( SQL_VARCHAR, (void*)pclInfo->szPrisonReason, sizeof(pclInfo->szPrisonReason) );
}

void DBGameProtocol::GetGuiltyInfo( class cltGuiltyInfo *pclInfo )
{
	m_odbc.GetData( &pclInfo->siTotalPrisonSecond, 0 );
	m_odbc.GetData( &pclInfo->siLeftPrisonSecond, 0 );
	m_odbc.GetData( pclInfo->szPrisonReason, sizeof(pclInfo->szPrisonReason) );
	m_odbc.GetData( &pclInfo->siTotalPrisonNum, 0 );
}

void DBGameProtocol::GetAngelInfo( struct HozoAngelList *pclInfo )
{
	m_odbc.GetData( pclInfo->szAccountID, sizeof(pclInfo->szAccountID) );
	m_odbc.GetData( pclInfo->szName, sizeof(pclInfo->szName) );
	m_odbc.GetData( &pclInfo->siLevel, 0 );
	m_odbc.GetData( &pclInfo->siFameLevel, 0 );
	m_odbc.GetData( &pclInfo->siPersonID, 0 );
}

void DBGameProtocol::GetGeneralMeetingVoter( class cltGeneralMeetingVoter *pclVoter )
{
	m_odbc.GetData( &pclVoter->siPersonID, 0);
	m_odbc.GetData( pclVoter->szPersonName, sizeof(pclVoter->szPersonName) );
	m_odbc.GetData( &pclVoter->siStockAmount, 0);
	m_odbc.GetData( &pclVoter->siVoteType, 0);
	m_odbc.GetData( &pclVoter->siParticipate, 0);
}

void DBGameProtocol::SetSimpleGuildRank(const class cltSimpleGuildRank *pclSimpleRank)
{
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclSimpleRank->siVillageUnique, sizeof(pclSimpleRank->siVillageUnique) );
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclSimpleRank->siGuildUnitIndex, sizeof(pclSimpleRank->siGuildUnitIndex) );
	m_odbc.SetParam( SQL_TINYINT, (void*)&pclSimpleRank->siType, sizeof(pclSimpleRank->siType) );
}
void DBGameProtocol::GetSimpleGuildRank(class cltSimpleGuildRank *pclSimpleRank)
{
	m_odbc.GetData( &pclSimpleRank->siType, 0 );
	m_odbc.GetData( &pclSimpleRank->siVillageUnique, 0 );
	m_odbc.GetData( &pclSimpleRank->siGuildUnitIndex, 0 );
	m_odbc.GetData( pclSimpleRank->szGuildName, sizeof(pclSimpleRank->szGuildName) );
	m_odbc.GetData( &pclSimpleRank->uiConfirm, 0 );
	m_odbc.GetData( &pclSimpleRank->siMarkIndex, 0 );
}

void DBGameProtocol::GetPoliceInfo(struct HyungzoPoliceList *pclInfo)
{
	m_odbc.GetData( pclInfo->szAccountID, sizeof(pclInfo->szAccountID));
	m_odbc.GetData( &pclInfo->siPersonID, 0);
	m_odbc.GetData( pclInfo->szName, sizeof(pclInfo->szName));
	m_odbc.GetData( &pclInfo->siLevel, 0);
}

void DBGameProtocol::GetCriminalInfo(struct HyungzoCriminalList *pclInfo)
{
	m_odbc.GetData( pclInfo->szAccountID, sizeof(pclInfo->szAccountID));
	m_odbc.GetData( &pclInfo->siPersonID, 0);
	m_odbc.GetData( pclInfo->szName, sizeof(pclInfo->szName));
	m_odbc.GetData( pclInfo->szReason, sizeof(pclInfo->szReason));
}

void DBGameProtocol::GetItemMallDateItemInfo(class cltItemMallDateItemInfo *pclInfo)
{
	m_odbc.GetData( &pclInfo->siProductKey, 0 );
	m_odbc.GetData( pclInfo->szName, sizeof(pclInfo->szName) );
	m_odbc.GetData( &pclInfo->siPrice, 0 );
}

void DBGameProtocol::GetItemMallListData(class cltItemMallListData *pclData)
{
	char szIconType[4];	// 게임에서는 사용하지 않는다.
	char szImage[50];	// 게임에서는 사용하지 않는다.
	char szStatus[2];	// 게임에서는 사용하지 않는다.
	int siOldPrice;		// 게임에서는 사용하지 않는다.

	m_odbc.GetData( &pclData->siProductKey, 0 );
	m_odbc.GetData( &pclData->siGroupKey, 0 );
	m_odbc.GetData( &pclData->bGiftFlag, 0 );
	m_odbc.GetData( szIconType, sizeof(szIconType) );
	m_odbc.GetData( pclData->szName, sizeof(pclData->szName) );
	m_odbc.GetData( &pclData->siPrice, 0 );
	m_odbc.GetData( &pclData->siEventPrice, 0 );
	m_odbc.GetData( &pclData->siSprFont, 0 );
	m_odbc.GetData( szImage, sizeof(szImage) );
	m_odbc.GetData( pclData->szExplain, sizeof(pclData->szExplain) );
	m_odbc.GetData( szStatus, sizeof(szStatus) );
	m_odbc.GetData( &siOldPrice, sizeof(siOldPrice) );
	m_odbc.GetData( &pclData->bOnlyBuySilverCoin, 0 );
}

void DBGameProtocol::GetStockBidVillageInfo(class cltStockBidVillageInfo *pclInfo)
{
	SI32 siDate = 0;
	m_odbc.GetData( &pclInfo->siVillageUnique_, 0 );
	m_odbc.GetData( &pclInfo->siTotalStockAmount_, 0 );
	m_odbc.GetData( &pclInfo->siTottalIncreaseStockAmount_, 0 );
	m_odbc.GetData( &pclInfo->siLeftIncreaseStockAmount_, 0 );
	m_odbc.GetData( &pclInfo->siCurrentStockPrice_, 0 );
	m_odbc.GetData( &siDate, 0 ); pclInfo->clBiddingDate_.MakeFullDateFromDateVary(siDate);
}

void DBGameProtocol::SetStockBidVillageInfo(const class cltStockBidVillageInfo *pclInfo, SI32 siDate)
{
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siVillageUnique_, sizeof(pclInfo->siVillageUnique_) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siTotalStockAmount_, sizeof(pclInfo->siTotalStockAmount_) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siTottalIncreaseStockAmount_, sizeof(pclInfo->siTottalIncreaseStockAmount_) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->siLeftIncreaseStockAmount_, sizeof(pclInfo->siLeftIncreaseStockAmount_) );
	m_odbc.SetParam( SQL_BIGINT, (void*)&pclInfo->siCurrentStockPrice_, sizeof(pclInfo->siCurrentStockPrice_) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&siDate, sizeof(siDate) );
}

void DBGameProtocol::GetBufInfo(class cltBufInfo *pclInfo, SI32 siIndex)
{
	m_odbc.GetData( &pclInfo->m_stBuf[siIndex].siBufType, 0 );
	m_odbc.GetData( &pclInfo->m_stBuf[siIndex].siPercent, 0 );
	SI16 siAdd = 0;				// 삭제 - by LEEKH 2008-03-08
	//m_odbc.GetData( &pclInfo->m_stBuf[siIndex].siAdd, 0 );
	//m_odbc.GetData( &siAdd, 0 );								//[진성] 삭제. 2008-6-11
	m_odbc.GetData( &pclInfo->m_stBuf[siIndex].siGetType, 0 );	//[진성] 버프를 얻은 타입 추가. 2008-6-11
	m_odbc.GetData( &pclInfo->m_stBuf[siIndex].siOverTime, 0 );
	bool bOverlap = false;		// 삭제 - by LEEKH 2008-03-08
	//m_odbc.GetData( &pclInfo->m_stBuf[siIndex].bOverlap, 0 );
	m_odbc.GetData( &bOverlap, 0 );	
}

void DBGameProtocol::SetBufInfo(const class cltBufInfo *pclInfo, SI32 siIndex)
{
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->m_stBuf[siIndex].siBufType, sizeof(pclInfo->m_stBuf[siIndex].siBufType) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->m_stBuf[siIndex].siPercent, sizeof(pclInfo->m_stBuf[siIndex].siPercent) );
	SI16 siAdd = 0;				// 삭제 - by LEEKH 2008-03-08
	//m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->m_stBuf[siIndex].siAdd, sizeof(pclInfo->m_stBuf[siIndex].siAdd) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&siAdd, sizeof(siAdd) );
	m_odbc.SetParam( SQL_INTEGER, (void*)&pclInfo->m_stBuf[siIndex].siOverTime, sizeof(pclInfo->m_stBuf[siIndex].siOverTime) );
	bool bOverlap = false;		// 삭제 - by LEEKH 2008-03-08
	//m_odbc.SetParam( SQL_TINYINT, (void*)&pclInfo->m_stBuf[siIndex].bOverlap, sizeof(pclInfo->m_stBuf[siIndex].bOverlap) );
	m_odbc.SetParam( SQL_TINYINT, (void*)&bOverlap, sizeof(bOverlap) );
}

void DBGameProtocol::GetGuildPoint(class cltGuildPoint *pclPoint)
{
	m_odbc.GetData( &pclPoint->siLevel );
	m_odbc.GetData( &pclPoint->siPoint );
	m_odbc.GetData( &pclPoint->siMoney );
	m_odbc.GetData( &pclPoint->clDate, sizeof(pclPoint->clDate) );
	m_odbc.GetData( &pclPoint->siFaultNum );
	m_odbc.GetData( &pclPoint->siMarkIndex );
	m_odbc.GetData( &pclPoint->siApplyMapIndex );
	m_odbc.GetData( &pclPoint->siBattleGP );
}

void DBGameProtocol::GetGuildMember(class cltGuildMember *pclMember)
{
	m_odbc.GetData( &pclMember->siPersonID );
	m_odbc.GetData( pclMember->szCharName, sizeof(pclMember->szCharName) );
	m_odbc.GetData( &pclMember->siType );
	m_odbc.GetData( &pclMember->siMoney);
	m_odbc.GetData( &pclMember->siGP);
	m_odbc.GetData( &pclMember->m_siLevel);
}

void DBGameProtocol::GetGuildStandingMember(class cltGuildStandingMemeber *pclMember)
{
	m_odbc.GetData( &pclMember->siPersonID );
	m_odbc.GetData( pclMember->szCharName, sizeof(pclMember->szCharName) );
}

void DBGameProtocol::GetGuildInfo(class cltGuildInfo *pclInfo)
{
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &pclInfo->siUnique );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( pclInfo->szName, sizeof(pclInfo->szName) );
			m_odbc.GetData( &pclInfo->siVillageUnique );
			m_odbc.GetData( &pclInfo->siMasterPersonID );
			GetGuildPoint( &pclInfo->clGPoint );
			m_odbc.GetData( &pclInfo->clDate );
			m_odbc.GetData( &pclInfo->siFee );
			m_odbc.GetData( &pclInfo->uiConfirm );
			GetGuildStatus( &pclInfo->clStatus );
			m_odbc.GetData( &pclInfo->siMoveVillageUnique );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SI32 siGuildMemberIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siGuildMemberIndex >= MAX_GUILD_MEMBER_NUM) continue;
			GetGuildMember(&pclInfo->clMember[siGuildMemberIndex++]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SI32 siGuildStandingMember = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siGuildStandingMember >= MAX_GUILD_MEMBER_NUM) continue;
			GetGuildStandingMember(&pclInfo->clStandingMember[siGuildStandingMember++]);
		END_FETCHDATA()
	END_NEXTRECORDSET()
}

void DBGameProtocol::GetGuildStatus(class cltGuildStatus *pclInfo)
{
	GetBasicAbility( pclInfo );
	m_odbc.GetData(&pclInfo->siBonus);
}

void DBGameProtocol::SetGuildStatus(const class cltGuildStatus *pclInfo)
{
	SetBasicAbility( pclInfo );
	m_odbc.SetParam(SQL_INTEGER, (void*)&pclInfo->siBonus, sizeof(pclInfo->siBonus));
}

void DBGameProtocol::GetGuildWarReserveUnit(class cltGuildWarReserveUnit *pclUnit)
{
	m_odbc.GetData( &pclUnit->m_siRoomIndex );
	m_odbc.GetData( &pclUnit->m_siHomeGuildUnique );
	m_odbc.GetData( pclUnit->m_szHomeGuildName, sizeof(pclUnit->m_szHomeGuildName) );
	m_odbc.GetData( &pclUnit->m_siAwayGuildUnique );
	m_odbc.GetData( pclUnit->m_szAwayGuildName, sizeof(pclUnit->m_szAwayGuildName) );
}

void DBGameProtocol::GetGuildDungeonChangeGPMoney(class cltGuildDungeon_ChangedGPMoney *pclInfo)
{
	m_odbc.GetData( &pclInfo->siGuildUnique, sizeof(pclInfo->siGuildUnique) );
	m_odbc.GetData( &pclInfo->siAddExp, sizeof(pclInfo->siAddExp) );
	m_odbc.GetData( &pclInfo->siExp, sizeof(pclInfo->siExp) );
	m_odbc.GetData( &pclInfo->siAddMoney, sizeof(pclInfo->siAddMoney) );
	m_odbc.GetData( &pclInfo->siMoney, sizeof(pclInfo->siMoney) );
}

void DBGameProtocol::GetGuildDungeonNewOwnership(class cltGuildDungeon_NewOwnership *pclInfo)
{
	m_odbc.GetData( &pclInfo->siMapindex, sizeof(pclInfo->siMapindex) );
	m_odbc.GetData( &pclInfo->siGuildUnique, sizeof(pclInfo->siGuildUnique) );
}

void DBGameProtocol::Convert_SYSTEMTIME_from_TIMESTAMP(_SYSTEMTIME *stST, TIMESTAMP_STRUCT *stTS)
{
	stST->wYear = stTS->year;
	stST->wMonth = stTS->month;
	stST->wDay = stTS->day;
	stST->wHour = stTS->hour;
	stST->wMinute = stTS->minute;
	stST->wSecond = stTS->second;
	stST->wMilliseconds = (WORD)stTS->fraction/1000000;
}

void DBGameProtocol::Convert_TIMESTAMP_from_SYSTEMTIME(TIMESTAMP_STRUCT *stTS, _SYSTEMTIME *stST)
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

void DBGameProtocol::GetSystemRewardInfo(cltSystemRewardInfo *pclInfo)
{
	TIMESTAMP_STRUCT stGiveTime;
	TIMESTAMP_STRUCT stExpireTime;
	ZeroMemory(&stGiveTime, sizeof(stGiveTime));
	ZeroMemory(&stExpireTime, sizeof(stExpireTime));

	m_odbc.GetData( &pclInfo->m_siIndex );
	m_odbc.GetData( &stGiveTime ); Convert_SYSTEMTIME_from_TIMESTAMP(&pclInfo->m_sGiveTime, &stGiveTime);
	m_odbc.GetData( &stExpireTime ); Convert_SYSTEMTIME_from_TIMESTAMP(&pclInfo->m_sExpireTime, &stExpireTime);
	m_odbc.GetData( &pclInfo->m_siMoney );
	GetItemData( &pclInfo->m_clItem );
	m_odbc.GetData( pclInfo->m_szReason, sizeof(pclInfo->m_szReason) );
}

void DBGameProtocol::SetCharOptionInfo(const cltPCharOptionInfo *pclInfo)
{
	m_odbc.SetParam( SQL_BIT, (void*)&pclInfo->bChatSetNormal, sizeof(pclInfo->bChatSetNormal));
	m_odbc.SetParam( SQL_BIT, (void*)&pclInfo->bChatSetVillage, sizeof(pclInfo->bChatSetVillage));
	m_odbc.SetParam( SQL_BIT, (void*)&pclInfo->bChatSetGuild, sizeof(pclInfo->bChatSetGuild));
	m_odbc.SetParam( SQL_BIT, (void*)&pclInfo->bChatSetParty, sizeof(pclInfo->bChatSetParty));
	m_odbc.SetParam( SQL_BIT, (void*)&pclInfo->bChatSetSystem, sizeof(pclInfo->bChatSetSystem));
	m_odbc.SetParam( SQL_BIT, (void*)&pclInfo->bPvpMode, sizeof(pclInfo->bPvpMode));
}

void DBGameProtocol::GetCharOptionInfo(cltPCharOptionInfo *pclInfo)
{
	m_odbc.GetData( &pclInfo->bChatSetNormal);
	m_odbc.GetData( &pclInfo->bChatSetVillage);
	m_odbc.GetData( &pclInfo->bChatSetGuild);
	m_odbc.GetData( &pclInfo->bChatSetParty);
	m_odbc.GetData( &pclInfo->bChatSetSystem);
	m_odbc.GetData( &pclInfo->bPvpMode);
}

void DBGameProtocol::GetServerEventInfo(sDBResponse_Server_Event_Rank_Info *pclSendMsg)
{
	//BufKind
	BEGIN_FETCHDATA()
		m_odbc.GetData(&pclSendMsg->m_ServerEventBufKind);
	END_FETCHDATA()

	// Total Score
	SI32 siTotalIdx = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		if(siTotalIdx >= SERVER_MAX_NUM || siTotalIdx < 0) continue;
		m_odbc.GetData(&pclSendMsg->m_clServerRankInfo[siTotalIdx].m_siServerUnique);
		m_odbc.GetData(&pclSendMsg->m_clServerRankInfo[siTotalIdx].m_siHuntingGroundCount);
		m_odbc.GetData(&pclSendMsg->m_clServerRankInfo[siTotalIdx].m_siScore);
		siTotalIdx++;
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// Daily Score
	SI32 siDailyIdx = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		if(siDailyIdx >= SERVER_MAX_NUM || siDailyIdx < 0) continue;
		m_odbc.GetData(&pclSendMsg->m_clServerDailyRankInfo[siDailyIdx].m_siServerUnique);
		m_odbc.GetData(&pclSendMsg->m_clServerDailyRankInfo[siDailyIdx].m_siHuntingGroundCount);
		m_odbc.GetData(&pclSendMsg->m_clServerDailyRankInfo[siDailyIdx].m_siScore);
		siDailyIdx++;
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// Person Score
	SI32 siUserIdx = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		if(siUserIdx >= USER_MAX_NUM || siUserIdx < 0) continue;
		m_odbc.GetData(&pclSendMsg->m_clPersonalRankInfo[siUserIdx].m_siLevel);
		m_odbc.GetData(&pclSendMsg->m_clPersonalRankInfo[siUserIdx].m_siScore);
		m_odbc.GetData(&pclSendMsg->m_clPersonalRankInfo[siUserIdx].m_siPersonalName, sizeof(pclSendMsg->m_clPersonalRankInfo[siUserIdx].m_siPersonalName));
		siUserIdx++;
	END_FETCHDATA()
	END_NEXTRECORDSET()
}

void DBGameProtocol::GetAmberboxSelectItemInfo(AmberboxSelectItemInfo *pclInfo)
{
	m_odbc.GetData( &pclInfo->m_siSlot );
	m_odbc.GetData( &pclInfo->m_siItemUnique );
	m_odbc.GetData( &pclInfo->m_siItemNum );
}

void DBGameProtocol::SetAmberboxSelectItemInfo(const AmberboxSelectItemInfo *pclInfo)
{
	m_odbc.SetParam( SQL_TINYINT,  (void*)&pclInfo->m_siSlot,		sizeof(pclInfo->m_siSlot)		);
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->m_siItemUnique, sizeof(pclInfo->m_siItemUnique) );
	m_odbc.SetParam( SQL_SMALLINT, (void*)&pclInfo->m_siItemNum,	sizeof(pclInfo->m_siItemNum)	);
}

void DBGameProtocol::GetPVP_League_OtherUserInfo( cltPVP_League_OtherUserInfo* pclInfo )
{
	m_odbc.GetData( &pclInfo->siLevel );
	m_odbc.GetData( &pclInfo->siGrade );
	m_odbc.GetData( &pclInfo->siResult );

	m_odbc.GetData( &pclInfo->siWin );
	m_odbc.GetData( &pclInfo->siDraw );
	m_odbc.GetData( &pclInfo->siLose );
	
	m_odbc.GetData( pclInfo->szName, sizeof(pclInfo->szName) );
}
void DBGameProtocol::GetGuildMapWar_RegList( cltGuildHuntMapWarListData* pclInfo )
{
	SI32 TempGuildUnique = 0 ;
	//m_odbc.GetData( &pclInfo->siGuildUnique );
}
void DBGameProtocol::GetGuildMapWar_GetRank( cltGuildHuntMapWarListRankData* pclInfo )
{
	m_odbc.GetData( &pclInfo->siGuildUnique );
	m_odbc.GetData( &pclInfo->siWin );
	m_odbc.GetData( &pclInfo->siDraw );
	m_odbc.GetData( &pclInfo->siLose );
	m_odbc.GetData( &pclInfo->siBGP );
}
void DBGameProtocol::SendUnionMarketAccountInfo(SI32 siPersonID, UI16 uiToID, SI16 siServerUnique)
{
	sDBRequest_NewMarketAccount *pRecvMsg	= (sDBRequest_NewMarketAccount *)m_pPacket;

	// 보낼 패킷
	sDBResponse_NewMarketAccount sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_NEWMARKETACCOUNT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = uiToID;

	// DB Query

	BEGIN_SP( "rsp_UnionMarketPersonAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &siServerUnique, sizeof(siServerUnique) );
	END_SP()
	
	EXEC_SQL()
	
	SI16 ItemindexCount = 0;
	BEGIN_FETCHDATA()
		if( ItemindexCount >= MAX_NEWMARKET_ORDER_PER_PERSON )
			break;
		m_odbc.GetData( &sendMsg.clMarketAccount.siOrderIndex[ItemindexCount], 0 );
		m_odbc.GetData( &sendMsg.clMarketAccount.clMoney[ItemindexCount], 0 );
		GetItemData( &sendMsg.clMarketAccount.clItem[ItemindexCount] );
		ItemindexCount++;
	END_FETCHDATA()

	sendMsg.siResult		= 1;
	sendMsg.siPersonID		= siPersonID;
	sendMsg.siServerUnique	= siServerUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::SetGoonzuRaccoonEventSetinfo( cGoonzuRaccoonEventData	*m_GoonzuRaccoonEventData)
{
	m_odbc.SetParam( SQL_INTEGER,  &m_GoonzuRaccoonEventData->m_siTime, sizeof(m_GoonzuRaccoonEventData->m_siTime));
	m_odbc.SetParam( SQL_INTEGER,  &m_GoonzuRaccoonEventData->m_siMakeMonsterNum, sizeof(m_GoonzuRaccoonEventData->m_siMakeMonsterNum));
}
void DBGameProtocol::SetGoonzuRaccoonEventSetinfo_Map( RocData *m_RocData)
{
	m_odbc.SetParam( SQL_INTEGER,  &m_RocData->m_siMapIndex, sizeof(m_RocData->m_siMapIndex));
	m_odbc.SetParam( SQL_INTEGER,  &m_RocData->m_siX, sizeof(m_RocData->m_siX));
	m_odbc.SetParam( SQL_INTEGER,  &m_RocData->m_siY, sizeof(m_RocData->m_siY));

}