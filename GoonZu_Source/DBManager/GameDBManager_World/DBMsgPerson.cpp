#include "DBGameProtocol.h"
#include "DBMsg-Person.h"
#include "DBMsg-System.h"
#include "DBMsg-Vote.h"
#include "DBMsg-ItemMall.h"
#include "DBMsg-Item.h"
#include <stdio.h>
#include <stdarg.h>


// �� Person�� �����
void DBGameProtocol::PersonTotalInfoCreate()
{
	// ���� ��Ŷ
	sDBRequest_PersonTotalInfoCreate *pRecvMsg = (sDBRequest_PersonTotalInfoCreate *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PersonTotalInfoCreate sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFOCREATE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	
	SI32	siPersonID = 0, siResult = 0;
	
	// cltPersonBasicInfo	- Person�� �⺻ ����.
	BEGIN_SP( "rsp_PersonInfoCreate" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->info.BasicInfo.siIDNum),			sizeof(pRecvMsg->info.BasicInfo.siIDNum) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->info.BasicInfo.uiIndex),			sizeof(pRecvMsg->info.BasicInfo.uiIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->info.BasicInfo.uiKind),			sizeof(pRecvMsg->info.BasicInfo.uiKind) );
		SetSimplePerson( &pRecvMsg->info.BasicInfo.clPerson );
		SetCharPos( &pRecvMsg->info.BasicInfo.clPos );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMakeNumber, 0 );
		m_odbc.GetData( sendMsg.szAccountID, sizeof(sendMsg.szAccountID) );
		m_odbc.GetData( sendMsg.szName, sizeof(sendMsg.szName) );
	END_FETCHDATA()

	sendMsg.siPersonID = siPersonID;
	
	// PersonID�� 0�̸� �����Ѵ�.
	if(siPersonID<=0)
	{
		sendMsg.siResult = siPersonID;

		SendPacket( (BYTE *)&sendMsg );
		return;
	}
	
	// cltPersonIPInfo		- Person�� ImportantParameta ����.
	BEGIN_SP( "rsp_IPCreate" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID,		sizeof(siPersonID) );
		SetPIPInfo( &pRecvMsg->info.IPInfo );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
		m_odbc.GetData( &sendMsg.siMakeNumber, 0 );
	END_FETCHDATA()
	
	
/*
	// cltSimpleRank		- Person�� �ź� ����.
	SI32 siType				= pRecvMsg->info.RankInfo.GetRankType();
	SI32 siVillageUnique	= pRecvMsg->info.RankInfo.GetVillageUnique();
	
	BEGIN_SP( "rsp_SimpleRankCreate" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID,		sizeof(siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &siType,			sizeof(siType) );
		m_odbc.SetParam( SQL_INTEGER, &siVillageUnique,	sizeof(siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
	END_FETCHDATA()
*/	

/*
	// cltPersonSkillInfo	- Person�� ��� ����.
	SI08	siMaxSkillIndex = MAX_SKILL_NUMBER;
	
	BEGIN_SP( "rsp_SkillInfoCreate" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID,		sizeof(siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &siMaxSkillIndex, sizeof(siMaxSkillIndex) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
	END_FETCHDATA()
*/	
	// cltPersonItemInfo	- Person�� ������ ����.
	for(SI16 siItemPos = 0; siItemPos < MAX_ITEM_PER_PERSON; siItemPos++)
	{
		BEGIN_SP( "rsp_PersonItemCreate" )
			m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
			m_odbc.SetParam( SQL_SMALLINT, &siItemPos, sizeof(siItemPos) ); // pos
			SetItemParam( &pRecvMsg->info.ItemInfo.clItem[siItemPos] );
		END_SP()
		
		EXEC_SQL()
		
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siResult, 0 );
		END_FETCHDATA()
	}

	// cltHealthInfo	- Person�� �ǰ�����.
	BEGIN_SP("rsp_HealthInfoCreate")
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
		SetHealthInfo( &pRecvMsg->info.clHealthInfo );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
	END_FETCHDATA()
	
//	// cltQuestInfo	- Person�� ����Ʈ����.
//	for( SI16 siQuestIndex = 0; siQuestIndex < MAX_QUEST_TYPE; siQuestIndex++ )
//	{
//		BEGIN_SP("rsp_QuestInfoCreate")
//			m_odbc.SetParam( SQL_INTEGER, &siPersonID,		sizeof(siPersonID) );
//			m_odbc.SetParam( SQL_SMALLINT, &siQuestIndex,	sizeof(siQuestIndex) );
//			SetQuestUnitInfo( &pRecvMsg->info.clQuestInfo.clUnit[siQuestIndex] );
//		END_SP()
//
//		EXEC_SQL()
//
//		BEGIN_FETCHDATA()
//			m_odbc.GetData( &siResult, 0 );
//		END_FETCHDATA()
//	}

	sendMsg.siResult = 1;
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Index = %d, Kind = %d"
				, pRecvMsg->info.BasicInfo.uiIndex, pRecvMsg->info.BasicInfo.uiKind
				);
		sDBLogMsg_QueryString LogMsg("Create Person", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::PersonTotalInfoGet()
{
	// ���� ��Ŷ
	sDBRequest_PersonTotalInfoGet *pRecvMsg = (sDBRequest_PersonTotalInfoGet *)m_pPacket;
	
	PersonTotalInfoGetCommon( pRecvMsg->siPersonID, pRecvMsg->usCharID );
}


void DBGameProtocol::PersonTotalInfoGetByIDNum()
{
	// ���� ��Ŷ
	sDBRequest_PersonTotalInfoGetByIDNum *pRecvMsg = (sDBRequest_PersonTotalInfoGetByIDNum *)m_pPacket;

	SI32	siPersonID = 0;
	
	// DB Query
	BEGIN_SP( "rsp_PersonIDGet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siIDNum), sizeof(pRecvMsg->siIDNum) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siIndex), sizeof(pRecvMsg->siIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData(&siPersonID, 0);
	END_FETCHDATA()
	
	if(siPersonID == 0)
	{
		// ���� ��Ŷ
		sDBResponse_PersonTotalInfoGet sendMsg;
		
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFOGETBYIDNUM;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.usCharID = pRecvMsg->usCharID;

		sendMsg.siResult = 0;
		sendMsg.bTiredSystem = pRecvMsg->bTiredSystem;
		sendMsg.dwPlayingTime = pRecvMsg->dwPlayingTime;
		SendPacket( (BYTE *)&sendMsg );
		return;
	}
	
	try
	{
		PersonTotalInfoGetCommon( siPersonID, pRecvMsg->usCharID,pRecvMsg->bDormancyState, pRecvMsg->szIPAddress, pRecvMsg->bTiredSystem, pRecvMsg->dwPlayingTime );
	}
	catch( ... )
	{
		// ���� ��Ŷ
		sDBResponse_PersonTotalInfoGet sendMsg;

		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFOGETBYIDNUM;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );
		sendMsg.usCharID = pRecvMsg->usCharID;

		sendMsg.siResult = 0;
		sendMsg.bTiredSystem = pRecvMsg->bTiredSystem;
		sendMsg.dwPlayingTime = pRecvMsg->dwPlayingTime;
		SendPacket( (BYTE *)&sendMsg );
		return;
	}
}

static void GZTrace( const char *pszFmt, ... )
{
	char szBuf[1024];
	va_list args;
	va_start( args, pszFmt );
	_vsnprintf( szBuf, sizeof(szBuf), pszFmt, args );
	va_end( args );
	szBuf[sizeof(szBuf)-1] = 0;

	FILE *fp = fopen( "C:\\Users\\Home\\Documents\\Online_work\\New Era Live\\GameDBManager\\GZTrace_PersonTotalInfo.log", "a" );
	if( fp )
	{
		fprintf( fp, "%s\n", szBuf );
		fclose( fp );
	}
}

void DBGameProtocol::PersonTotalInfoGetCommon(SI32 siPersonID, UI16 usCharID,bool bDormancyState, char szIPAddress[20], bool bTiredSystem, DWORD dwPlayingTime)
{
	GZTrace( "ENTER PersonTotalInfoGetCommon personID=%d charID=%d", siPersonID, usCharID );

	// ���� ��Ŷ
	sDBResponse_PersonTotalInfoGet sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFOGETBYIDNUM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = usCharID;
	
	SI32 siDormancyState = bDormancyState;

	// cltPersonBasicInfo		BasicInfo;		// Person�� �⺻ ����.
	BEGIN_SP( "rsp_PersonInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &siDormancyState, sizeof(siDormancyState) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.clInfo.BasicInfo.siIDNum), 0);
		m_odbc.GetData( &(sendMsg.clInfo.BasicInfo.uiIndex), 0);
		m_odbc.GetData( &(sendMsg.clInfo.BasicInfo.uiKind), 0 );
		GetSimplePerson( &sendMsg.clInfo.BasicInfo.clPerson );
		GetCharPos( &sendMsg.clInfo.BasicInfo.clPos );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siPlaySecond, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siPlayEventSecond, 0 );
		GetPos( &sendMsg.clInfo.BasicInfo.clLastMainMapPos );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siSchoolUnique, 0);
		GetFatherInfo( &sendMsg.clInfo.BasicInfo.clFatherInfo);
		GetSimplePerson( &sendMsg.clInfo.BasicInfo.clRecommenderInfo );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siVoteKingSelectionIndex, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.uiGRank, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.uiSex, 0 );
		m_odbc.GetData( sendMsg.clInfo.BasicInfo.szAccountID, sizeof(sendMsg.clInfo.BasicInfo.szAccountID) );
		m_odbc.GetData( &sendMsg.siHomeVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clSummonInfo.siSelectAutoHeal, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clSummonInfo.bAutoHeal, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bWennyOldManUpgrade, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bQuestEvolve, 0 );
		m_odbc.GetData( sendMsg.clInfo.BasicInfo.szGreeting, sizeof(sendMsg.clInfo.BasicInfo.szGreeting) );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bPremiumService, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siPremiumServiceDate, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bCombatService, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siCombatServiceDate, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bMakeItemService, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siMakeItemServiceDate, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bBulletService, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siBulletServiceDate, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bSummonStaminaService, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siSummonStaminaServiceDate, 0 );
		m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bReceiveBeginnerWeapon, 0 );
		m_odbc.GetData( &sendMsg.clInfo.uiGachaCnt, 0 );
	END_FETCHDATA()

	if( siPersonID != sendMsg.clInfo.BasicInfo.clPerson.siPersonID )
	{
		// send
		sendMsg.siResult = 0;
		SendPacket( (BYTE *)&sendMsg );
		return;
	}

	// TicketService
	UI08 uiServiceType = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &uiServiceType );
			switch (uiServiceType)
			{
			case NOLIMIT_TICKET:
				sendMsg.clInfo.BasicInfo.bUseNoLimitTicketWarp = 1;
				m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siNoLimitTicketWarpDateVary );
				break;
			case MARKET_TICKET:
				sendMsg.clInfo.BasicInfo.bMarKetPremiumService = 1;
				m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siMarKetPremiumServiceDate);
				break;
			case SPACEBOX_TICKET:
				sendMsg.clInfo.BasicInfo.bSpaceBoxPremiumService = 1;
				m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siSpaceBoxPremiumServiceDate);
				break;
			case SUMMON_SPACE_TICKET:
				sendMsg.clInfo.BasicInfo.bSummonExtraSlot = 1;
				m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siSummonExtraSlotDate);
				break;
			}
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// PVPResult
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siPVPWinNumber, 0 );
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siPVPLoseNumber, 0 );
			m_odbc.GetData( sendMsg.clInfo.BasicInfo.szPVPWinHighLevelUserName, sizeof(sendMsg.clInfo.BasicInfo.szPVPWinHighLevelUserName) );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ��������
	SI32 siBufIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siBufIndex, 0); if(siBufIndex >= MAX_BUF_COUNT) continue;
			GetBufInfo(&sendMsg.clInfo.BasicInfo.clBufInfo, siBufIndex);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// �����ΰ� �ƴѰ�
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.uiPoliceSwitch, sizeof(sendMsg.clInfo.BasicInfo.uiPoliceSwitch) );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ��������
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetGuiltyInfo( &sendMsg.clInfo.clGuiltyInfo );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// �ڻ�
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siWealthRank, 0 );
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.clTotalWealthMoney.itMoney, 0 );
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.siWealthCalcDay, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ���
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetSimpleGuildRank(&sendMsg.clInfo.BasicInfo.clGuildRank);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// �������� ������
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.bExistentItem, 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ��ť����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.clInfo.clWorldMoney.siWorldMoney, 0);
			m_odbc.GetData(&sendMsg.clInfo.clWorldMoney.siTradeWorldMoney, 0);
			m_odbc.GetData(&sendMsg.clInfo.clWorldMoney.siTradeWorldMoneyPrice, 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ���� ����Ʈ
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.clInfo.clPersonNaming.m_siSelectedIndex, 0);
			m_odbc.GetData(&sendMsg.clInfo.clPersonNaming.m_siSelectedLevel, 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SI32 siNamingIdx = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siNamingIdx, 0); if(siNamingIdx >= MAX_NAMINGQUEST_INDEX) continue;
			m_odbc.GetData(&sendMsg.clInfo.clPersonNaming.m_siLevel[siNamingIdx], 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	// ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.clEmpireInfo.m_siEmpireRank, 0);
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.clEmpireInfo.m_siEmpireUnique, 0);
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.clGuildRank.siEmpireUnique, 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.clInfo.clTransFormInfo.uiNowTransFormKind);
			m_odbc.GetData(&sendMsg.clInfo.clTransFormInfo.bTransFormMode);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SI32 siTransFormKind = 0;
	SI32 siIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if( siIndex >= MAX_TRANSFORM_CHAR )		continue;
			m_odbc.GetData(&siTransFormKind);
			sendMsg.clInfo.clTransFormInfo.clTransForm[siIndex].uiCanTransFormKind = siTransFormKind;
			m_odbc.GetData(&sendMsg.clInfo.clTransFormInfo.clTransForm[siIndex].siUseTotalSecond);
			siIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ������
	SI16 siTimeMeterRateType = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siTimeMeterRateType); if(siTimeMeterRateType >= MAX_TIMEMETERRATE) continue;
			sendMsg.clInfo.BasicInfo.clTimeMeterRateInfo.m_clTimeMeterRate[siTimeMeterRateType].siTimeMeterRateType = siTimeMeterRateType;
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.clTimeMeterRateInfo.m_clTimeMeterRate[siTimeMeterRateType].siLeftTimeSec);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ���� ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( sendMsg.clInfo.clWorldPassportForDB.m_szPassportName, sizeof(sendMsg.clInfo.clWorldPassportForDB.m_szPassportName) );
			m_odbc.GetData( &sendMsg.clInfo.clWorldPassportForDB.m_siIssueDateVary );
			m_odbc.GetData( &sendMsg.clInfo.clWorldPassportForDB.m_siExpireDateVary );
		END_FETCHDATA()
	END_NEXTRECORDSET()
	

	////////////////////////////////////////////
	// ������ʹ� AccountDB�� ���� ������ �޴°�
	////////////////////////////////////////////
	// ���ӷα���
	SI32 siAttendType = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siAttendType);
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bAttendToday[siAttendType], 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			// �̺�Ʈ
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bNewAccount, 0 );
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bGameEvent, 0 );
			m_odbc.GetData( &sendMsg.clInfo.BasicInfo.bWinPrizeEvent, 0 );
			// ����
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.siCountryID, 0);
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.bShowCountry, 0);
			// ��Ÿ����
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.bShowPuchikomi, 0);
			// ����������
			TIMESTAMP_STRUCT stTS;
			m_odbc.GetData(&stTS); Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.clInfo.BasicInfo.siFirstCreatTimeDateVary, &stTS);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ��ī��
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.bPCRoom, 0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// [����] �޸� ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(sendMsg.clInfo.m_clDormancyInfo.GetGiveDormancyEquipItemPointer(), 0);
			m_odbc.GetData(sendMsg.clInfo.m_clDormancyInfo.GetDormancyPointPointer(), 0);
			m_odbc.GetData(sendMsg.clInfo.m_clDormancyInfo.GetRemainedDormancyPlayTimePointer(), 0);
			// �޸� ���� ���� ��
			TIMESTAMP_STRUCT stTS;
			m_odbc.GetData(&stTS); Convert_SYSTEMTIME_from_TIMESTAMP( sendMsg.clInfo.m_clDormancyInfo.GetStartedDate(), &stTS);
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	sendMsg.clInfo.m_clDormancyInfo.SetDormancyState(bDormancyState);

	// ���� ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.clInfo.BasicInfo.m_uiRealAge);
		END_FETCHDATA()
	END_NEXTRECORDSET()
    
	// cltPersonIPInfo			IPInfo;			// Person�� ImportantParameta ����. 
	GZTrace( "  reached rsp_IPGet block (PersonInfoGet result-set walk completed)" );
	BEGIN_SP( "rsp_IPGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		GetPIPInfo(&sendMsg.clInfo.IPInfo);
		GZTrace( "  IPInfo READ: level=%d life=%d mana=%d exp=%I64d money=%I64d str=%d vit=%d",
			sendMsg.clInfo.IPInfo.siLevel, sendMsg.clInfo.IPInfo.Life, sendMsg.clInfo.IPInfo.Mana,
			sendMsg.clInfo.IPInfo.siExp, sendMsg.clInfo.IPInfo.siMoney,
			sendMsg.clInfo.IPInfo.siStr, sendMsg.clInfo.IPInfo.siVit );
	END_FETCHDATA()
	

	// cltHealthInfo
	SI32 siResult1, siResult2;
	BEGIN_SP( "rsp_HealthInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult1, 0 );
		m_odbc.GetData( &siResult2, 0 );
		GetHealthInfo( &sendMsg.clInfo.clHealthInfo );
		GZTrace( "  HealthInfo READ: hungry=%d org1=%d hp=%d", sendMsg.clInfo.clHealthInfo.siHungry, sendMsg.clInfo.clHealthInfo.siInnerOrg1, sendMsg.clInfo.clHealthInfo.uiHealthPoint );
	END_FETCHDATA()

	// cltQuestInfo
	BEGIN_SP( "rsp_QuestTotalInfoGet" ) // ĳ���� �α��� �� ȣ��.
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()

	EXEC_SQL()

	SI16 siQuestIndex;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siQuestIndex, 0 );
		if(siQuestIndex >= MAX_QUEST_TYPE)
			continue;
		GetQuestUnitInfo( &(sendMsg.clInfo.clQuestInfo.clUnit[siQuestIndex]) );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
		GetQuestInfoCommon( &sendMsg.clInfo.clQuestInfo.clSpecialQuestInfo);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
		GetQuestWordAssemblySwitch( sendMsg.clInfo.clQuestInfo.bWordAssemblySwitch );
		m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.siClearWordSpecialQuest, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.siEventClearWordSpecialQuest, 0 ); // ��� ����� �̺�Ʈ ���� Ƚ �� ����.
		END_FETCHDATA()
	END_NEXTRECORDSET()


	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.siUnique );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestClearConditionInfo.siFlag );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestClearConditionInfo.siPara1 );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestClearConditionInfo.siPara2 );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siMoney );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siExp );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siFameExp );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siNCoin );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.bMoveBuf );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siNamingIndex );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siNamingLevel );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SI32 siRewardIdx = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siRewardIdx);if(siRewardIdx >= MAX_QUEST_REWARD_NUMBER) continue;
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siItemUnique[siRewardIdx]);
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siItemNum[siRewardIdx]);
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clNamingQuestInfo.clQuestRewardInfo.siItemUseDate[siRewardIdx]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetQuestInfoCommon( &sendMsg.clInfo.clQuestInfo.clHappyBeanQuestInfo);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SI32 siLeftTimeSec = 0;

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.siUnique );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siFlag );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara1 );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestClearConditionInfo.siPara2 );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siMoney );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siExp );
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siFameExp );
			m_odbc.GetData( &siLeftTimeSec );	sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.SetLeftTime(siLeftTimeSec);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siRewardIdx);				if(siRewardIdx >= MAX_QUEST_REWARD_NUMBER)	continue;			
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siItemUnique[siRewardIdx]);
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siItemNum[siRewardIdx]);
			m_odbc.GetData( &sendMsg.clInfo.clQuestInfo.clMissionQuestInfo.clQuestRewardInfo.siItemUseDate[siRewardIdx]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// cltSimpleRank			RankInfo;		// Person�� �ź� ����. 
	SI32 siType, siVillageUnique;
	
	BEGIN_SP( "rsp_SimpleRankGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siType, 0);			sendMsg.clInfo.RankInfo.SetRankType(siType);
		m_odbc.GetData( &siVillageUnique, 0);	sendMsg.clInfo.RankInfo.SetVillageUnique(siVillageUnique);
	END_FETCHDATA()	
	
	
	// cltPersonSkillInfo		SkillInfo;		// Person�� ��� ����. 
	UI08	siSkillUnique;
	
	BEGIN_SP( "rsp_SkillInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siSkillUnique, 0); if(siSkillUnique >= MAX_SKILL_NUMBER) continue;
		m_odbc.GetData( &(sendMsg.clInfo.SkillInfo.clSkill[siSkillUnique].uiLevel), 0);
		m_odbc.GetData( &(sendMsg.clInfo.SkillInfo.clSkill[siSkillUnique].siSkillExp), 0);
	END_FETCHDATA()
	
	
	// cltPersonItemInfo		ItemInfo;		// Person�� ������ ����. 
	SI16	siItemPos;
	
	BEGIN_SP( "rsp_PersonItemGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siItemPos, 0);								// pos
		if(siItemPos >= MAX_ITEM_PER_PERSON)
			continue;
		GetItemData( &sendMsg.clInfo.ItemInfo.clItem[siItemPos]);	// ItemInfo
	END_FETCHDATA()
	
	// cltBankAccount		clBankAccount;		// Bank ����
	SI64 itMoney = 0;
	BEGIN_SP( "rsp_BankAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.clInfo.clBankInfo.clAccountNum.siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.clInfo.clBankInfo.clAccountNum.siAccountNum, 0);
		m_odbc.GetData( &sendMsg.clInfo.clBankInfo.siPersonID, 0);
		m_odbc.GetData( &itMoney, 0);	sendMsg.clInfo.clBankInfo.SetMoney(itMoney);
	END_FETCHDATA()
	
	// cltPersonStockInfo	clStockInfo;
	// cltStockAccount		clInfo;
	BEGIN_SP( "rsp_StockPersonAccountInfoGet" )
		m_odbc.SetParam(                 SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	int		i = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clInfo.clUnit[i].siVillageUnique), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clInfo.clUnit[i].siAmount), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clInfo.clUnit[i].bBuySwitch), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clInfo.clUnit[i].siLimitBuyBidStockAmount), 0);
		i++;
	END_FETCHDATA()
	
	
	// cltCharOrderInfo	clOrderInfo;
	BEGIN_SP( "rsp_StockPersonOrderInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	i = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].siOrderMode), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siIndex), 0);
		sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siType		= TRADEORDER_TYPE_STOCK;
		sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siPersonID	= siPersonID;
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siPrice), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siAmount), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siVillageUnique), 0);
		m_odbc.GetData( &(sendMsg.clInfo.clStockInfo.clOrderInfo.clOrderUnit[i].clOrder.siDate), 0);
		i++;
	END_FETCHDATA()
	
	// cltPersonMarketInfo	clMarketInfo;
	// cltMarketAccount		clInfo;
	SI32	siMarketItemIndex;

	BEGIN_SP( "rsp_MarketPersonAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siMarketItemIndex, 0);
		m_odbc.GetData( &(sendMsg.clInfo.clMarketInfo.clInfo.clMoney[siMarketItemIndex]), 0 );
		GetItemData( &sendMsg.clInfo.clMarketInfo.clInfo.clItem[siMarketItemIndex]);
	END_FETCHDATA()
	
	
	// cltCharRealEstate		clRealEstateInfo;
	BEGIN_SP( "rsp_HouseEstateListGet" )
		m_odbc.SetParam( SQL_INTEGER, &(siPersonID), sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()

	SI32 siEstateIndex = 0;

	BEGIN_FETCHDATA()
		if( siEstateIndex >= MAX_REAL_ESTATE_NUMBER_PER_PERSON )
			break;
		sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siType = REALESTATE_TYPE_HOUSE;
		m_odbc.GetData( &sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siIndex, 0);
		GetRentContract( &(sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].clContract) );
		siEstateIndex++;
	END_FETCHDATA()

	// cltCharRealEstate		clRealEstateInfo;
	BEGIN_SP( "rsp_LandEstateListGet" )
		m_odbc.SetParam( SQL_INTEGER, &(siPersonID), sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()

	BEGIN_FETCHDATA()
		if( siEstateIndex >= MAX_REAL_ESTATE_NUMBER_PER_PERSON )
			break;
		sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siType = REALESTATE_TYPE_LAND;
		m_odbc.GetData( &sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siIndex, 0);
		GetRentContract( &(sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].clContract) );
		siEstateIndex++;
	END_FETCHDATA()

	// cltCharRealEstate		clRealEstateInfo;
	BEGIN_SP( "rsp_MineEstateListGet" )
		m_odbc.SetParam( SQL_INTEGER, &(siPersonID), sizeof(siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		if( siEstateIndex >= MAX_REAL_ESTATE_NUMBER_PER_PERSON )
			break;
		sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siType = REALESTATE_TYPE_MINE;
		m_odbc.GetData( &sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siVillageUnique, 0);
		m_odbc.GetData( &sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].siIndex, 0);
		GetRentContract( &(sendMsg.clInfo.clRealEstateInfo.clRealEstate[siEstateIndex].clContract) );
		siEstateIndex++;
	END_FETCHDATA()

	// cltPersonHorseInfo		clHorseInfo;
	BEGIN_SP( "rsp_HorseTotalInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &(siPersonID), sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.clInfo.clHorseInfo.siCurrentShowHorse, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clHorseInfo.siCurrentHorseRIDEHORSE, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clHorseInfo.bAutoFoodSwitch, 0 );
	END_FETCHDATA()

	cltHorse clHorse;
	BEGIN_NEXTRECORDSET()
		SI32 siHorseIndex = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siHorseIndex, 0); if(siHorseIndex >= MAX_HORSE_PER_PERSON) continue;
			GetHorseInfo( &clHorse );	
			sendMsg.clInfo.clHorseInfo.SetHorse(siHorseIndex, &clHorse);
			//GetHorseInfo( &sendMsg.clInfo.clHorseInfo.clHorse[siHorseIndex] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// cltPFriendInfo		clFriendInfo;
	// By LEEKH 2007.10.03 - ģ������ CharCommon���� �ű�
/*	BEGIN_SP( "rsp_FriendInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &(siPersonID), sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()

	SI32 siGroupIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siGroupIndex );
		if(siGroupIndex >= MAX_FRIEND_GROUP )
			continue;
		m_odbc.GetData( sendMsg.clInfo.clFriendInfo.szGroupName[siGroupIndex], sizeof(sendMsg.clInfo.clFriendInfo.szGroupName[siGroupIndex]) );
	END_FETCHDATA()

	SI32 siFriendIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siFriendIndex >= MAX_FRIEND_NUMBER )
				break;
			m_odbc.GetData( &sendMsg.clInfo.clFriendInfo.siGroupID[siFriendIndex], sizeof(sendMsg.clInfo.clFriendInfo.siGroupID[siFriendIndex]) );
			GetSimplePerson( &sendMsg.clInfo.clFriendInfo.clFriend[siFriendIndex]);
			m_odbc.GetData( sendMsg.clInfo.clFriendInfo.szAccountID[siFriendIndex++], sizeof(sendMsg.clInfo.clFriendInfo.szAccountID[siFriendIndex++]) );
		END_FETCHDATA()
	END_NEXTRECORDSET()
*/

	// cltPFriendInfo		clFriendInfo;
	BEGIN_SP( "rsp_DailyQuestGet" )
		m_odbc.SetParam( SQL_INTEGER, &(siPersonID), sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()

	// DailyQuest
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siMonsterKind, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siMonsterNum, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siKillMonsterNum, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siRewardItemUnique, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siRewardItemNum, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siRewardExp, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siFee, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siQuestTime, 0 );
		m_odbc.GetData( &sendMsg.clInfo.clDailyQuestInfo.siLeftTime, 0 );
	END_FETCHDATA()

	// DailyQuest2
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siMonsterKind, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siMonsterNum, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siKillMonsterNum, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siRewardItemUnique, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siRewardItemNum, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siRewardExp, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siFee, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siQuestTime, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siLeftTime, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.siRewardMoney, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.bMoneySelected, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest2Info.bHiddenQuest, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// DailyQuest3
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siItemUnique, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siItemNum, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siMadeItemNum, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siRewardItemUnique, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siRewardItemNum, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siRewardExp, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siFee, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siQuestTime, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.siLeftTime, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.bMoneySelected, 0 );
			m_odbc.GetData( &sendMsg.clInfo.clDailyQuest3Info.bHiddenQuest, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()


	// Summon
	BEGIN_SP( "rsp_SummonInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &(siPersonID), sizeof(siPersonID) );
	END_SP()
	
	EXEC_SQL()


	UI08 uiSummonIndex = 0;
	UI08 uiSummonSlot = 0;
	SI16 siSummonItemPos = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &uiSummonIndex );
		if(uiSummonIndex >= MAX_SUMMON_OWN) continue;
		GetSummon( &sendMsg.clInfo.clSummonInfo.clSummon[uiSummonIndex] );
	END_FETCHDATA()


	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &uiSummonIndex);
		m_odbc.GetData( &siSummonItemPos );
		if(uiSummonIndex >= MAX_SUMMON_OWN || siSummonItemPos >= MAX_SHAPE_ITEM) continue;
		GetItemData( &sendMsg.clInfo.clSummonInfo.clItemInfo[uiSummonIndex].clEquipItem[siSummonItemPos] );
	END_FETCHDATA()
	END_NEXTRECORDSET()


	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &uiSummonSlot );
		if(uiSummonSlot >= MAX_HAVE_SUMMON) continue;
		GetSimpleSummon( &sendMsg.clInfo.clSummonInfo.clSummonBase[uiSummonSlot] );
	END_FETCHDATA()
	END_NEXTRECORDSET()



	// [����] ������
	BEGIN_SP( "rsp_ElixirGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID));
	END_SP()

	EXEC_SQL()

    BEGIN_FETCHDATA()
		SI32 siResult;
		m_odbc.GetData(&siResult);
		m_odbc.GetData(&siPersonID);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siEndMinuteVary);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_uiMultiple);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siPoint);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siStr);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siHnd);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siDex);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siMag);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siWis);
		m_odbc.GetData(&sendMsg.clInfo.m_clElixirInfo.m_siVit);
	END_FETCHDATA()

	//[����] ���ո���.
	SI32	siSuperMarketItemIndex;
	BEGIN_SP( "rsp_MarketPersonAccountInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siSuperMarketItemIndex, 0);
		//m_odbc.GetData( &sendMsg.clInfo.clNewMarketInfo.clNewMarketTradeOrder[siSuperMarketItemIndex].siPrice, 0 );
		//GetItemData( &sendMsg.clInfo.clSuperMarketInfo.clSuperMarketTradeOrder[siSuperMarketItemIndex].m_clItem);
	END_FETCHDATA()



	// �Ƿι�����-
	sendMsg.bTiredSystem = bTiredSystem;
	sendMsg.dwPlayingTime = dwPlayingTime;
	//---------------------------------------------------------
	// send
	//---------------------------------------------------------
	sendMsg.siResult = 1;

	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1 && szIPAddress != NULL)
	{
		char log[512] = "";
		sprintf(log, "%s, CashMoney = %I64d, BankMoney = %I64d"
				, szIPAddress, sendMsg.clInfo.IPInfo.siMoney, sendMsg.clInfo.clBankInfo.GetMoney()
				);
		sDBLogMsg_QueryString LogMsg("Login GameServer", siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::PersonListInfoGet()
{
	// ���� ��Ŷ
	sDBRequest_PersonListInfoGet *pRecvMsg = (sDBRequest_PersonListInfoGet *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_PersonListInfoGet sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONLISTINFOGET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	UI08 uiMaxShapePos = MAX_SHAPE_ITEM;
	TIMESTAMP_STRUCT stDeleteTime;

	// DB Query
	BEGIN_SP( "rsp_PersonListInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->idnum), sizeof(pRecvMsg->idnum) );
		m_odbc.SetParam( SQL_TINYINT, &(uiMaxShapePos), sizeof(uiMaxShapePos) );
	END_SP()

	EXEC_SQL()

	int index = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &index, sizeof(SI32)); 
		
		if(index >= MAX_TOTAL_CHAR_PER_USER || index < 0) continue;

		m_odbc.GetData( &(sendMsg.personInfo[index].uiKind), 0 );
		GetSimplePerson( &sendMsg.personInfo[index].clPerson);
		GetCharPos( &(sendMsg.personInfo[index].clPos) );
		m_odbc.GetData( &sendMsg.siLevel[index], 0 );

		ZeroMemory(&stDeleteTime, sizeof(stDeleteTime));
		m_odbc.GetData( &stDeleteTime, 0 ); Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.stTime[index], &stDeleteTime);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	/* [����] ���� �������� �������� �����Ƿ� ����
	index = 0;
	int siItemPos = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &index, 0 );
			m_odbc.GetData( &siItemPos, 0);
			if(index >= MAX_CHAR_PER_USER || index < 0 || siItemPos >= MAX_SHAPE_ITEM || siItemPos < 0)
				continue;
			GetItemData( &sendMsg.personIteminfo[index][siItemPos] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
	
	*/


	/*cyj
	// ���� ��Ŷ
	sDBRequest_PersonListInfoGet *pRecvMsg = (sDBRequest_PersonListInfoGet *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PersonListInfoGet sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONLISTINFOGET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_PersonListInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->idnum), sizeof(pRecvMsg->idnum) );
	END_SP()
	
	EXEC_SQL()
	
	int i = 0;
	int index = 0;

	BEGIN_FETCHDATA()
		if( i < 3 ) {
			m_odbc.GetData( &index, sizeof(SI32));
			m_odbc.GetData( &(sendMsg.personInfo[index].uiKind), 0 );
			GetSimplePerson( &sendMsg.personInfo[index].clPerson);
			GetCharPos( &(sendMsg.personInfo[index].clPos) );
			m_odbc.GetData( &sendMsg.siLevel[index], 0 );
			i++;
		}
	END_FETCHDATA()
	
	// send
	SendPacket( (char *)&sendMsg );
	*/
}


void DBGameProtocol::PersonTotalInfoDelete()
{
	// ���� ��Ŷ
	sDBRequest_PersonTotalInfoDelete *pRecvMsg = (sDBRequest_PersonTotalInfoDelete *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PersonTotalInfoDelete sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFODELETE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[�߰� : Ȳ���� 2007. 11. 5 �����ϴ� ĳ������ person id]
	sendMsg.siPersonID = pRecvMsg->siPersonID;

	SI32 siOrderIndex;
	for( siOrderIndex = 0; siOrderIndex < MAX_MARKET_ORDER_PER_PERSON; siOrderIndex++)
	{
		sendMsg.clOrder[siOrderIndex].siIndex = -1;
	}

	// DB Query
	BEGIN_SP( "rsp_PersonTotalInfoDelete" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siIDNum), sizeof(pRecvMsg->siIDNum) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siIndex), sizeof(pRecvMsg->siIndex) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, (pRecvMsg->szIdenNum), sizeof(pRecvMsg->szIdenNum) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()
	
	siOrderIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siOrderIndex >= MAX_MARKET_ORDER_PER_PERSON) break;
			m_odbc.GetData( &sendMsg.clOrder[siOrderIndex].siIndex );
			sendMsg.clOrder[siOrderIndex].siPersonID = 0;
			sendMsg.clOrder[siOrderIndex].siPrice = 0;
			sendMsg.clOrder[siOrderIndex].siAmount = 0;
			sendMsg.clOrder[siOrderIndex].siDate = 0;
			sendMsg.clOrder[siOrderIndex].siType = TRADEORDER_TYPE_OBJECT;
			sendMsg.clOrder[siOrderIndex].bBankError = 0;
			siOrderIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// ������� ���
	GetGuildInfo(&sendMsg.clinfo);

	// �α� �ۼ�.
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Index = %d, IP = %s"
				, pRecvMsg->siIndex, pRecvMsg->szIP
				);
		sDBLogMsg_QueryString LogMsg("Delete Person", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PersonTotalInfoPreDelete()
{
	// ���� ��Ŷ
	sDBRequest_PersonTotalInfoPreDelete *pRecvMsg = (sDBRequest_PersonTotalInfoPreDelete *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_PersonTotalInfoPreDelete sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFOPREDELETE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[�߰� : Ȳ���� 2007. 11. 5 �����ϴ� ĳ������ person id]
	sendMsg.siPersonID = pRecvMsg->siPersonID;

	TIMESTAMP_STRUCT stDeleteTime;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stDeleteTime, &pRecvMsg->systemTime);

	// DB Query
	BEGIN_SP( "rsp_PersonTotalInfoPreDelete" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siIDNum), sizeof(pRecvMsg->siIDNum) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siIndex), sizeof(pRecvMsg->siIndex) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, (pRecvMsg->szIdenNum), sizeof(pRecvMsg->szIdenNum) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stDeleteTime, sizeof(stDeleteTime) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->predeltype, sizeof(pRecvMsg->predeltype) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::PersonTotalInfoSet()
{
	// ���� ��Ŷ
	sDBRequest_PersonTotalInfoSet *pRecvMsg = (sDBRequest_PersonTotalInfoSet *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PersonTotalInfoSet sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONTOTALINFOSET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siResult = 0;
	SI32 siLife			= pRecvMsg->clInfo.IPInfo.GetLife();
	SI32 siMana			= pRecvMsg->clInfo.IPInfo.GetMana();

	// DB Query
	BEGIN_SP( "rsp_PersonTotalInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->clInfo.BasicInfo.clPerson.siPersonID), sizeof(pRecvMsg->clInfo.BasicInfo.clPerson.siPersonID) );
		SetCharPos( &(pRecvMsg->clInfo.BasicInfo.clPos) );
		m_odbc.SetParam( SQL_INTEGER, &siLife, sizeof(siLife) );
		m_odbc.SetParam( SQL_INTEGER, &siMana, sizeof(siMana) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
	END_FETCHDATA()

	// send
	sendMsg.siResult = 1;
	SendPacket( (BYTE *)&sendMsg );

}

// �� Person�� ������ ������ ������Ų��
void DBGameProtocol::ChangeMoney()
{
	// ���� ��Ŷ
	sDBRequest_ChangeMoney *pRecvMsg = (sDBRequest_ChangeMoney *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_ChangeMoney sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGEMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_MoneyChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason,			sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siChangedMoney,	sizeof(pRecvMsg->siChangedMoney) );
	END_SP()
	
	EXEC_SQL()
	
	GMONEY siBeforeMoney = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		sendMsg.siReason = pRecvMsg->siReason;
		m_odbc.GetData( &sendMsg.siChangedMoney, 0 );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &siBeforeMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, Monster = %s BeforeMoney = %I64d, ChangedMoney = %I64d, LeftMoney = %I64d"
				, sendMsg.siReason, pRecvMsg->strReason, siBeforeMoney, sendMsg.siChangedMoney, sendMsg.siMoney
				);
		sDBLogMsg_QueryString LogMsg("Change Money", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::UserID()
{
	// ���� ��Ŷ
	sDBRequest_Userid *pRecvMsg = (sDBRequest_Userid *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_Userid sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_USERID;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonUserIDGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siToPersonID,		sizeof(pRecvMsg->siToPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szToPersonName,	sizeof(pRecvMsg->szToPersonName) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( sendMsg.strUserID, sizeof(sendMsg.strUserID) );
		m_odbc.GetData( &sendMsg.bShowPuchikomi, 0 );
	END_FETCHDATA()

	sendMsg.siPersonID = pRecvMsg->siMyPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

//Ư��������� �α����� ����ϰų� ���´�
void DBGameProtocol::BlockUser()
{
	// ���� ��Ŷ
	sDBRequest_BlockUser *pRecvMsg = (sDBRequest_BlockUser *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_BlockUser sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BLOCKUSER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_BlockUser" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siBlockMode,	sizeof(pRecvMsg->siBlockMode) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName,	sizeof(pRecvMsg->szPersonName) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDay,			sizeof(pRecvMsg->siDay) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szBlockReason,	sizeof(pRecvMsg->szBlockReason) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siBlockMode, 0 );
		m_odbc.GetData( sendMsg.szPersonName, sizeof(sendMsg.szPersonName) );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siYear, 0 );
		m_odbc.GetData( &sendMsg.siMonth, 0 );
		m_odbc.GetData( &sendMsg.siDay, 0 );
		m_odbc.GetData( &sendMsg.siHour, 0 );
		m_odbc.GetData( &sendMsg.siMin, 0 );
		m_odbc.GetData( sendMsg.szBlockReason, sizeof(sendMsg.szBlockReason) );
		m_odbc.GetData( &sendMsg.siWarnMode, sizeof(sendMsg.siWarnMode) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "BlockMode = %d/%d, WarnMode = %d, %d-%d-%d %d:%d, Reason = %s"
			, pRecvMsg->siBlockMode, sendMsg.siBlockMode, sendMsg.siWarnMode, sendMsg.siYear, sendMsg.siMonth, sendMsg.siDay, sendMsg.siHour, sendMsg.siMin, sendMsg.szBlockReason
				);
		sDBLogMsg_QueryString LogMsg("Block User", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::MoveMoney()
{
	// ���� ��Ŷ
	sDBRequest_MoveMoney *pRecvMsg = (sDBRequest_MoveMoney *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_MoveMoney sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MOVEMONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonMoneyMove" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siFromPersonID, sizeof(pRecvMsg->siFromPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siToPersonID, sizeof(pRecvMsg->siToPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMovedMoney, sizeof(pRecvMsg->siMovedMoney) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siFromPersonID, 0 );
		m_odbc.GetData( &sendMsg.siFromPersonMoney, 0 );
		m_odbc.GetData( &sendMsg.siToPersonID, 0 );
		m_odbc.GetData( &sendMsg.siToPersonMoney, 0 );
		m_odbc.GetData( &sendMsg.siReason, 0 );
		m_odbc.GetData( &sendMsg.siMovedMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, MovedMoney = %I64d, LeftFromMoney = %I64d, leftToMoney = %I64d"
				, sendMsg.siReason, sendMsg.siMovedMoney, sendMsg.siFromPersonMoney, sendMsg.siToPersonMoney
				);
		sDBLogMsg_QueryString LogMsg("Move Money", sendMsg.siFromPersonID, sendMsg.siToPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::SetSchool()
{
	// ���� ��Ŷ
	sDBRequest_SetSchool *pRecvMsg = (sDBRequest_SetSchool *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_SetSchool sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSCHOOL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonSchoolSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSchoolUnique, sizeof(pRecvMsg->siSchoolUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::PersonSimpleInfo()
{
	// ���� ��Ŷ
	sDBRequest_PersonSimpleInfo *pRecvMsg = (sDBRequest_PersonSimpleInfo *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_PersonSimpleInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONSIMPLEINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	memcpy(sendMsg.szGetCharInfoName,	pRecvMsg->szGetCharInfoName,	sizeof(sendMsg.szGetCharInfoName));
	memcpy(sendMsg.szCharInfoName,		pRecvMsg->szCharInfoName,		sizeof(sendMsg.szCharInfoName));

	// DB Query
	BEGIN_SP( "rsp_PersonSimpleInfoGet" )
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->siPersonID,			sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->szGetCharInfoName,	sizeof(pRecvMsg->szGetCharInfoName));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason,				sizeof(pRecvMsg->siReason) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,			0 );
		m_odbc.GetData( &sendMsg.siPersonID,		0 );
		m_odbc.GetData( &sendMsg.siReason,			0 );
		m_odbc.GetData( &sendMsg.siLevel,			0 );
		m_odbc.GetData( &sendMsg.siGrandPersonID,	0 );
		m_odbc.GetData( &sendMsg.siCharKind,		0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::IncreaseChildNumber()
{
	// ���� ��Ŷ
	sDBRequest_IncreaseChildNumber *pRecvMsg = (sDBRequest_IncreaseChildNumber *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_IncreaseChildNumber sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_INCREASECHILDNUMBER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonChildNumIncrease" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIncNum, sizeof(pRecvMsg->siIncNum) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siChildNumber, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::GetChildList()
{
	// ���� ��Ŷ
	sDBRequest_GetChildList *pRecvMsg = (sDBRequest_GetChildList *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_GetChildList sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETCHILDLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonChildListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	SI32 siCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetSimplePerson( &sendMsg.clChildInfo[siCount].clPerson );
			m_odbc.GetData( &sendMsg.clChildInfo[siCount].siLevel, 0 );
			m_odbc.GetData( sendMsg.clChildInfo[siCount].szAccountID, sizeof(sendMsg.clChildInfo[siCount].szAccountID) );
			siCount++;
			if(siCount >= MAX_CHILDINFO_NUMBER)
			{
				SendPacket( (BYTE *)&sendMsg );
				ZeroMemory( sendMsg.clChildInfo, sizeof(cltChildInfo) * MAX_CHILDINFO_NUMBER );
				siCount = 0;
			}
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::RecommendPerson()
{
	// ���� ��Ŷ
	sDBRequest_RecommendPerson *pRecvMsg = (sDBRequest_RecommendPerson *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_RecommendPerson sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RECOMMENDPERSON;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonRecommend" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strUserName, sizeof(pRecvMsg->strUserName) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIDNum, sizeof(pRecvMsg->siIDNum) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiSlot, sizeof(pRecvMsg->uiSlot) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( sendMsg.strRecommenderUserName, sizeof(sendMsg.strRecommenderUserName) );
		m_odbc.GetData( sendMsg.strUserName, sizeof(sendMsg.strUserName) );
		m_odbc.GetData( &sendMsg.siIDNum, 0 );
		m_odbc.GetData( &sendMsg.uiSlot, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siRecommendPersonID, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::VoteKingSelection()
{
	// ���� ��Ŷ
	sDBRequest_VoteKingSelection *pRecvMsg = (sDBRequest_VoteKingSelection *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_VoteKingSelection sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VOTEKINGSELECTION;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonVoteSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siVoteListIndex, sizeof(pRecvMsg->siVoteListIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVoteListIndex, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::LogoutRecord()
{
	// ���� ��Ŷ
	sDBRequest_LogOutRecord *pRecvMsg = (sDBRequest_LogOutRecord *)m_pPacket;

	// DB Query
	BEGIN_SP( "rsp_PersonLogout" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()

	char log[512] = "";
	sprintf(log, "IP = %s, NormalQuit = %d"
			, pRecvMsg->szIP, pRecvMsg->uiNormalQuitSwitch
		);
	sDBLogMsg_QueryString LogMsg("Log Out", pRecvMsg->siPersonID, NULL, NULL, log);
	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::AccountInfo()
{
	// ���� ��Ŷ
	sDBRequest_AccountInfo *pRecvMsg = (sDBRequest_AccountInfo *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_AccountInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ACCOUNTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siRequestType = pRecvMsg->siRequestType;

	// DB Query
	BEGIN_SP( "rsp_AccountInfoGet" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szAccountID, sizeof(pRecvMsg->szAccountID) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siIndex, 0 );
		switch(siIndex)
		{
		case 0:
			m_odbc.GetData( sendMsg.szPersonName1, sizeof(sendMsg.szPersonName1) );
			break;
		case 1:
			m_odbc.GetData( sendMsg.szPersonName2, sizeof(sendMsg.szPersonName1) );
			break;
		case 2:
			m_odbc.GetData( sendMsg.szPersonName3, sizeof(sendMsg.szPersonName1) );
			break;
		}
		m_odbc.GetData( sendMsg.szPersonName1, 0 );
	END_FETCHDATA()

	memcpy( sendMsg.szAccountID, pRecvMsg->szAccountID, 32 );
	sendMsg.szAccountID[ 31 ] = NULL;

	sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::AccountInfoForHanXNoti()
{
	// ���� ��Ŷ
	sDBRequest_AccountInfoForHanXNoti *pRecvMsg = (sDBRequest_AccountInfoForHanXNoti *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_AccountInfoForHanXNoti sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ACCOUNTINFOFORHANXNOTI;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_siArrayIndex = pRecvMsg->m_siArrayIndex;
	sendMsg.m_nIdx = pRecvMsg->m_nIdx;

	// DB Query
	BEGIN_SP( "rsp_AccountInfoGet" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->m_szAccountID, sizeof(pRecvMsg->m_szAccountID) );
	END_SP()

	EXEC_SQL()

	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siIndex, 0 );
		switch(siIndex)
		{
		case 0:
			m_odbc.GetData( sendMsg.szPersonName1, sizeof(sendMsg.szPersonName1) );
			break;
		case 1:
			m_odbc.GetData( sendMsg.szPersonName2, sizeof(sendMsg.szPersonName1) );
			break;
		case 2:
			m_odbc.GetData( sendMsg.szPersonName3, sizeof(sendMsg.szPersonName1) );
			break;
		}
		m_odbc.GetData( sendMsg.szPersonName1, 0 );
	END_FETCHDATA()

	memcpy( sendMsg.szServerID, pRecvMsg->m_szServerID, 50 );
	sendMsg.szServerID[ 49 ] = NULL;
	memcpy( sendMsg.szAccountID, pRecvMsg->m_szAccountID, 32 );
	sendMsg.szAccountID[ 31 ] = NULL;

	sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ChangeName()
{
	// ���� ��Ŷ
	sDBRequest_ChangeName *pRecvMsg = (sDBRequest_ChangeName *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_ChangeName sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGENAME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.clitem_Log.Set(&pRecvMsg->clitem);
	sendMsg.siCharID =  pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonNameChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szName, sizeof(pRecvMsg->szName) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPos, sizeof(pRecvMsg->siPos) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->m_bUseItem, sizeof(pRecvMsg->m_bUseItem) );
		SetItemParam( &pRecvMsg->clitem );
		
	END_SP()
	
	EXEC_SQL()
	
	char szOldName[MAX_PLAYER_NAME] = "";

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( sendMsg.szName, sizeof(sendMsg.szName) );
		m_odbc.GetData( &sendMsg.siPos, sizeof(sendMsg.siPos) );
		m_odbc.GetData( &sendMsg.m_bUseItem, sizeof(sendMsg.m_bUseItem) );
		GetItemData( &sendMsg.clitem );
		m_odbc.GetData( sendMsg.szOldName, sizeof(sendMsg.szOldName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	char log[512] = "";
	sprintf(log, "OldName = %s, ItemPos = %d, BeforeItemNum = %d, AfterItemNum = %d"
			, sendMsg.szOldName, pRecvMsg->siPos, pRecvMsg->clitem.siItemNum, sendMsg.clitem.siItemNum
		);
	sDBLogMsg_QueryString LogMsg("Change Name", pRecvMsg->siPersonID, NULL, pRecvMsg->clitem.siUnique, log);
	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::GetChangedName()
{
	// ���� ��Ŷ
	sDBRequest_GetChangedName *pRecvMsg = (sDBRequest_GetChangedName *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GetChangedName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETCHANGEDNAME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonNameChangedListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPage, sizeof(pRecvMsg->siPage) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMaxNum, sizeof(pRecvMsg->siMaxNum) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		if(siIndex >= MAX_HOZO_CHANGEDNAME_LIST)
			continue;
		m_odbc.GetData( &sendMsg.stChangedNameChar[siIndex].siDate, 0 );
		m_odbc.GetData( sendMsg.stChangedNameChar[siIndex].szOldName, sizeof(sendMsg.stChangedNameChar[siIndex].szOldName) );
		m_odbc.GetData( sendMsg.stChangedNameChar[siIndex++].szNewName, sizeof(sendMsg.stChangedNameChar[siIndex++].szNewName) );
	END_FETCHDATA()
	
	sendMsg.siResult = (siIndex == 0)? 0 : 1;
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.siPage = pRecvMsg->siPage;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ChangeCharKind()
{
	// ���� ��Ŷ
	sDBRequest_Change_CharKind *pRecvMsg = (sDBRequest_Change_CharKind *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_Change_CharKind sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGE_CHARKIND;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.clitem_Log.Set(&pRecvMsg->clitem);

	// DB Query
	BEGIN_SP( "rsp_PersonKindChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siKind, sizeof(pRecvMsg->siKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siPos, sizeof(pRecvMsg->siPos) );
		SetItemParam( &pRecvMsg->clitem );
	END_SP()
	
	EXEC_SQL()

	SI16 siOldKind = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siKind, 0 );
		m_odbc.GetData( &sendMsg.siPos, sizeof(sendMsg.siPos) );
		GetItemData( &sendMsg.clitem );
		m_odbc.GetData( &siOldKind, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	char log[512] = "";
	sprintf(log, "NewKind = %d, OldKind = %d, ItemPos = %d, UseItemNum = %d, AfterItemNum = %d"
			, pRecvMsg->siKind, siOldKind, pRecvMsg->siPos, pRecvMsg->clitem.siItemNum, sendMsg.clitem.siItemNum
		);
	sDBLogMsg_QueryString LogMsg("Change Kind", pRecvMsg->siPersonID, NULL, pRecvMsg->clitem.siUnique, log);
	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::GameEvent()
{
	// ���� ��Ŷ
	sDBRequest_Prize_GameEvent *pRecvMsg = (sDBRequest_Prize_GameEvent *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Prize_GameEvent sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PRIZE_GAMEEVENT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonGameEventSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bGameEvent, sizeof(pRecvMsg->bGameEvent) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bWinPrizeEvent, sizeof(pRecvMsg->bWinPrizeEvent) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.bGameEvent, 0 );
		m_odbc.GetData( &sendMsg.bWinPrizeEvent, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	char log[512] = "";
	sprintf(log, "GameEvent = %d, WinPrizeEvent = %d"
		, pRecvMsg->bGameEvent, pRecvMsg->bWinPrizeEvent
		);
	sDBLogMsg_QueryString LogMsg("Game Event", pRecvMsg->siPersonID, NULL, NULL, log);
	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::GreetingSet()
{
	// ���� ��Ŷ
	sDBRequest_SetGreeting *pRecvMsg = (sDBRequest_SetGreeting *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetGreeting sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETGREETING;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonGreetingSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szText, sizeof(pRecvMsg->szText) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( sendMsg.szText, sizeof(sendMsg.szText) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GreetingGet()
{
	// ���� ��Ŷ
	sDBRequest_GetGreeting *pRecvMsg = (sDBRequest_GetGreeting *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GetGreeting sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETGREETING;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonGreetingGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName, sizeof(pRecvMsg->szPersonName) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( sendMsg.szPersonName, sizeof(sendMsg.szPersonName) );
		m_odbc.GetData( sendMsg.szText, sizeof(sendMsg.szText) );
	END_FETCHDATA()

	sendMsg.siParam = pRecvMsg->siParam;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetPersonNameNew()
{
	// ���� ��Ŷ
	sDBRequest_GetItemMakerNameNew *pRecvMsg = (sDBRequest_GetItemMakerNameNew *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GetItemMakerNameNew sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_ITEMMAKERNAMENEW;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID			= pRecvMsg->siCharID;
	sendMsg.siDupSerialID		= pRecvMsg->siDupSerialID;

	// DB Query
	BEGIN_SP( "rsp_ItemMakerNameGet" )
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->siSerialID,		sizeof(pRecvMsg->siSerialID)	 );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siSerialID,		0 );
		m_odbc.GetData( &sendMsg.siServerUnique,	0 );	
		m_odbc.GetData( sendMsg.szName, sizeof(sendMsg.szName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetPersonName()
{
	// ���� ��Ŷ
	sDBRequest_GetItemMakerName *pRecvMsg = (sDBRequest_GetItemMakerName *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GetItemMakerName sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ITEMMAKERNAME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonNameGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( sendMsg.szName, sizeof(sendMsg.szName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ChangeRecommendPoint()
{
	// ���� ��Ŷ
	sDBRequest_ChangeRecommendPoint *pRecvMsg = (sDBRequest_ChangeRecommendPoint *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_ChangeRecommendPoint sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGERECOMMENDPOINT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siPersonID= pRecvMsg->siPersonID;
	sendMsg.siReason = pRecvMsg->siReason;


	// DB Query
	BEGIN_SP( "rsp_RecommendPointChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szCharName, sizeof(pRecvMsg->szCharName));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRecommendPersonID, sizeof(pRecvMsg->siRecommendPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLevel, sizeof(pRecvMsg->siLevel));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siChangedRecommentPoint, sizeof(pRecvMsg->siChangedRecommentPoint));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( sendMsg.szCharName, sizeof(sendMsg.szCharName) );
		m_odbc.GetData( &sendMsg.siRecommendPersonID);
		m_odbc.GetData( &sendMsg.siChangedRecommendPoint );
		m_odbc.GetData( &sendMsg.siLevel );
		m_odbc.GetData( &sendMsg.siRecommendPoint );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, ChangePoint = %d, Level = %d, RecommendPoint = %d",
			pRecvMsg->siReason, pRecvMsg->siChangedRecommentPoint, pRecvMsg->siLevel, sendMsg.siRecommendPoint
			);
		sDBLogMsg_QueryString LogMsg("Recommend Point Set", pRecvMsg->siPersonID, pRecvMsg->siRecommendPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetRecommendPoint()
{
	// ���� ��Ŷ
	sDBRequest_GetRecommendPoint *pRecvMsg = (sDBRequest_GetRecommendPoint *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GetRecommendPoint sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETRECOMMENDPOINT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_RecommendPointGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siRecommendPoint );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::RecommendPointList()
{
	// ���� ��Ŷ
 	sDBRequest_RecommendPointList *pRecvMsg = (sDBRequest_RecommendPointList *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_RecommendPointList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RECOMMENDPOINT_LIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiCharID = pRecvMsg->usCharID;
	sendMsg.siPersonID = pRecvMsg->siPersonID;

	SI16 siListPerPage = MAX_RPLISTDATA_PER_PAGE;

	// DB Query
	BEGIN_SP( "rsp_RecommendPointList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siCurrentPage, sizeof(pRecvMsg->siCurrentPage));
		m_odbc.SetParam( SQL_SMALLINT, &siListPerPage, sizeof(siListPerPage));
	END_SP()

	EXEC_SQL()

	SI32 siCount = 0;
	BEGIN_FETCHDATA()
		if(siCount >= MAX_RPLISTDATA_PER_PAGE) break;
		m_odbc.GetData( sendMsg.sRPListData[siCount].szCharName, sizeof(sendMsg.sRPListData[siCount].szCharName) );
		m_odbc.GetData( &sendMsg.sRPListData[siCount].siLevel );
		siCount++;
	END_FETCHDATA()

 	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetCountry()
{
	// ���� ��Ŷ
	sDBRequest_SetCountry *pRecvMsg = (sDBRequest_SetCountry *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetCountry sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETCOUNTRY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_CountrySet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siCountryID, sizeof(pRecvMsg->siCountryID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siCountryID );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetShowCountry()
{
	// ���� ��Ŷ
	sDBRequest_SetShowCountry *pRecvMsg = (sDBRequest_SetShowCountry *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetShowCountry sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSHOWCOUNTRY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_CountryShowSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bShowCountry, sizeof(pRecvMsg->bShowCountry));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.bShowCountry );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GIveFP()
{
	// ���� ��Ŷ
	sDBRequest_GiveFP *pRecvMsg = (sDBRequest_GiveFP *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GiveFP sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSHOWCOUNTRY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siReason = pRecvMsg->siReason;

	// DB Query
	BEGIN_SP( "rsp_RecommendGIveFP" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRecommendPoint, sizeof(pRecvMsg->siRecommendPoint));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siRecommendPoint );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "AddPoint = %d, RecommendPoint = %d"
			, pRecvMsg->siRecommendPoint, sendMsg.siRecommendPoint
			);
		sDBLogMsg_QueryString LogMsg("Recommend Point Give", pRecvMsg->siPersonID, 0, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::AddBuf()
{
	// ���� ��Ŷ
	sDBRequest_AddBuf *pRecvMsg = (sDBRequest_AddBuf *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_AddBuf sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ADDBUF;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.clUsedItem.Set(&pRecvMsg->clItem);
	
	SI16 siAdd = 0;				// ���� - by LEEKH 2008-03-08
	bool bOverlap = false;		// ���� - by LEEKH 2008-03-08

	// DB Query
	BEGIN_SP( "rsp_PersonBufAdd" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_stBuf.siBufType, sizeof(pRecvMsg->m_stBuf.siBufType));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_stBuf.siPercent, sizeof(pRecvMsg->m_stBuf.siPercent));
		//m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_stBuf.siAdd, sizeof(pRecvMsg->m_stBuf.siAdd));
		
		//m_odbc.SetParam( SQL_SMALLINT, &siAdd, sizeof(siAdd));											// [����] ����.				 2008-6-10 
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_stBuf.siGetType, sizeof(pRecvMsg->m_stBuf.siGetType));	// [����] �����귿 �����̰�. 2008-6-10 
		
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_stBuf.siOverTime, sizeof(pRecvMsg->m_stBuf.siOverTime));
		//m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->m_stBuf.bOverlap, sizeof(pRecvMsg->m_stBuf.bOverlap));
		m_odbc.SetParam( SQL_TINYINT, &bOverlap, sizeof(bOverlap));												
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bUseItem, sizeof(pRecvMsg->bUseItem));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos));
		SetItemParam( &pRecvMsg->clItem);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siIndex );
		m_odbc.GetData( &sendMsg.m_stBuf.siBufType );
		m_odbc.GetData( &sendMsg.m_stBuf.siPercent );
		
		//m_odbc.GetData( &sendMsg.m_stBuf.siAdd );		
		//m_odbc.GetData( &siAdd );							// [����] ����. 2008-6-10
		m_odbc.GetData( &sendMsg.m_stBuf.siGetType );		// [����] �����귿 �����̰�. 2008-6-10 

		m_odbc.GetData( &sendMsg.m_stBuf.siOverTime );
		m_odbc.GetData( &bOverlap );						
		
		m_odbc.GetData( &sendMsg.bUseItem );
		m_odbc.GetData( &sendMsg.siItemPos );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	sendMsg.money = pRecvMsg->money;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "BufIndex = %d, BufType = %d, Percent = %d, Add = %d, OverTime = %d, Overlap = %d, ItemPos = %d, ItemNum = %d"
			//, pRecvMsg->siIndex, pRecvMsg->m_stBuf.siBufType, pRecvMsg->m_stBuf.siPercent, pRecvMsg->m_stBuf.siAdd, pRecvMsg->m_stBuf.siOverTime, pRecvMsg->m_stBuf.bOverlap
			, pRecvMsg->siIndex, pRecvMsg->m_stBuf.siBufType, pRecvMsg->m_stBuf.siPercent, siAdd, pRecvMsg->m_stBuf.siOverTime, bOverlap
			, pRecvMsg->siItemPos, pRecvMsg->clItem.siItemNum
			);
		sDBLogMsg_QueryString LogMsg("Buf Add", pRecvMsg->siPersonID, 0, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::DeleteBuf()
{
	// ���� ��Ŷ
	sDBRequest_DeleteBuf *pRecvMsg = (sDBRequest_DeleteBuf *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_DeleteBuf sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DELETEBUF;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_stBuf.Set(&pRecvMsg->m_stBuf);

	// DB Query
	BEGIN_SP( "rsp_PersonBufDelete" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siIndex );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "BufIndex = %d, BufType = %d, OverTime = %d"
			, pRecvMsg->siIndex, pRecvMsg->m_stBuf.siBufType, pRecvMsg->m_stBuf.siOverTime
			);
		sDBLogMsg_QueryString LogMsg("Buf Delete", pRecvMsg->siPersonID, 0, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetShowPuchikomi()
{
	// ���� ��Ŷ
	sDBRequest_SetShowPuchikomi *pRecvMsg = (sDBRequest_SetShowPuchikomi *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetShowPuchikomi sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSHOWPUCHIKOMI;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_PersonShowPuchikomiSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bShowPuchikomi, sizeof(pRecvMsg->bShowPuchikomi));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.bShowPuchikomi );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ShowPuchikomi = %d"
			, pRecvMsg->bShowPuchikomi
			);
		sDBLogMsg_QueryString LogMsg("Puchikomi Set", pRecvMsg->siPersonID, 0, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetFather()
{
	// ���� ��Ŷ
	sDBRequest_SetFather *pRecvMsg = (sDBRequest_SetFather *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetFather sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETFATHER;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_TeacherSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szFatherName, sizeof(pRecvMsg->szFatherName) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMyLevel, sizeof(pRecvMsg->siMyLevel) );
		m_odbc.SetParam( SQL_SMALLINT, (void*)&MAX_PUPILNUMBER, sizeof(MAX_PUPILNUMBER) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMyLevel, 0 );
		m_odbc.GetData( &sendMsg.siFatherPersonID, 0 );
		m_odbc.GetData( sendMsg.szFatherName, sizeof(sendMsg.szFatherName) );
		m_odbc.GetData( &sendMsg.siPrevFatherPersonID, 0 );
		m_odbc.GetData( &sendMsg.siChangedFatherNumber, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, ""
			);
		sDBLogMsg_QueryString LogMsg("Teacher Set", pRecvMsg->siPersonID, sendMsg.siFatherPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetTeacherPoint()
{
	// ���� ��Ŷ
	sDBRequest_SetTeacherPoint *pRecvMsg = (sDBRequest_SetTeacherPoint *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetTeacherPoint sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETTEACHERPOINT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );


	// DB Query
	BEGIN_SP( "rsp_TeacherPointSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTeacherPersonID, sizeof(pRecvMsg->siTeacherPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siAddPoint, sizeof(pRecvMsg->siAddPoint));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siTeacherPersonID );
		m_odbc.GetData( &sendMsg.siTotalTeacherPoint );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "AddPoint = %d, TotalTeacherPoint = %d"
			, pRecvMsg->siAddPoint, sendMsg.siTotalTeacherPoint
			);
		sDBLogMsg_QueryString LogMsg("TeacherPoint Set", pRecvMsg->siTeacherPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetItemFromTeacherPoint()
{
	// ���� ��Ŷ
	sDBRequest_GetItem_From_TeacherPoint *pRecvMsg = (sDBRequest_GetItem_From_TeacherPoint *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GetItem_From_TeacherPoint sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETITEM_FROM_TEACHERPOINT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siTotalTeacherPoint = 0;

	// DB Query
	BEGIN_SP( "rsp_TeacherPointUse" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTeacherPersonID, sizeof(pRecvMsg->siTeacherPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siAddPoint, sizeof(pRecvMsg->siAddPoint));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siAddPoint);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Point = %d, TotalTeacherPoint = %d"
			, pRecvMsg->siAddPoint, siTotalTeacherPoint
			);
		sDBLogMsg_QueryString LogMsg("TeacherPoint Use", pRecvMsg->siTeacherPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::RenouncePupil()
{
	// ���� ��Ŷ
	sDBRequest_RenouncePupil *pRecvMsg = (sDBRequest_RenouncePupil *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_RenouncePupil sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RENOUNCEPUPIL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siTeacherPersonID = 0;

	// DB Query
	BEGIN_SP( "rsp_TeacherRenouncePupil" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPupilPersonID, sizeof(pRecvMsg->siPupilPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPupilPersonID );
		m_odbc.GetData( &siTeacherPersonID );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, ""
			);
		sDBLogMsg_QueryString LogMsg("Teacher Renounce Pupil", siTeacherPersonID, pRecvMsg->siPupilPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::OverTakeTeacherLevel()
{
	// ���� ��Ŷ
	sDBRequest_OverTakeTeacherLevel *pRecvMsg = (sDBRequest_OverTakeTeacherLevel *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_OverTakeTeacherLevel sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_OVERTAKETEACHERLEVEL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siTeacherPersonID = 0;

	// DB Query
	BEGIN_SP( "rsp_TeacherOvertakeLevel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siTeacherPersonID, sizeof(pRecvMsg->siTeacherPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMyLevel, sizeof(pRecvMsg->siMyLevel) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPupilPersonID, sizeof(pRecvMsg->siPupilPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( sendMsg.szTeacherName, sizeof(sendMsg.szTeacherName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, ""
			);
		sDBLogMsg_QueryString LogMsg("OverTake TeacherLevel", pRecvMsg->siPupilPersonID, pRecvMsg->siTeacherPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PVPResult()
{
	// ���� ��Ŷ
	sDBRequest_PVPResult *pRecvMsg = (sDBRequest_PVPResult *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_PVPResult sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PVPRESULT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.siWinCharID = pRecvMsg->siWinCharID;
	sendMsg.siLoseCharID = pRecvMsg->siLoseCharID;
	MStrCpy(sendMsg.szWinUserName, pRecvMsg->szWinUserName, MAX_PLAYER_NAME);
	MStrCpy(sendMsg.szLoseUserName, pRecvMsg->szLoseUserName, MAX_PLAYER_NAME);

	// DB Query
	BEGIN_SP( "rsp_PersonPVPResult" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siWinPersonID, sizeof(pRecvMsg->siWinPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLosePersonID, sizeof(pRecvMsg->siLosePersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( sendMsg.szPVPWinHighLevelUserName, sizeof(sendMsg.szPVPWinHighLevelUserName) );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Win"
			);
		sDBLogMsg_QueryString LogMsg("PVP Result", pRecvMsg->siWinPersonID, pRecvMsg->siLosePersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::Attend()
{
	// ���� ��Ŷ
	sDBRequest_Attend *pRecvMsg = (sDBRequest_Attend *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Attend sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ATTEND;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.usCharID = pRecvMsg->usCharID;

	TIMESTAMP_STRUCT stEventStart;
	TIMESTAMP_STRUCT stEventEnd;

	ZeroMemory(&stEventStart, sizeof(stEventStart));
	ZeroMemory(&stEventEnd, sizeof(stEventEnd));

	stEventStart.year = pRecvMsg->clEventStartDay.m_uiYear;
	stEventStart.month = pRecvMsg->clEventStartDay.m_uiMonth;
	stEventStart.day = pRecvMsg->clEventStartDay.m_uiDay;

	stEventEnd.year = pRecvMsg->clEventEndDay.m_uiYear;
	stEventEnd.month = pRecvMsg->clEventEndDay.m_uiMonth;
	stEventEnd.day = pRecvMsg->clEventEndDay.m_uiDay;

	// DB Query
	BEGIN_SP( "rsp_PersonAttend" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stEventStart, sizeof(stEventStart) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stEventEnd, sizeof(stEventEnd) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siType );
		m_odbc.GetData( &sendMsg.siAttendanceDay );
		m_odbc.GetData( &sendMsg.siRewardDay );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Type = %d, AttendanceDay = %d"
			, sendMsg.siType, sendMsg.siAttendanceDay
			);
		sDBLogMsg_QueryString LogMsg("Attend", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetAttend()
{
	// ���� ��Ŷ
	sDBRequest_SetAttend *pRecvMsg = (sDBRequest_SetAttend *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetAttend sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_SETATTEND;
	sendMsg.packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.uiCharID			= pRecvMsg->uiCharID;
	sendMsg.siPersonID			= pRecvMsg->siPersonID;
	sendMsg.siType				= pRecvMsg->siType;

	TIMESTAMP_STRUCT stAttendDay;

	ZeroMemory(&stAttendDay, sizeof(stAttendDay));

	stAttendDay.year = pRecvMsg->kAttendDay.m_uiYear;
	stAttendDay.month = pRecvMsg->kAttendDay.m_uiMonth;
	stAttendDay.day = pRecvMsg->kAttendDay.m_uiDay;

	// DB Query
	BEGIN_SP( "rsp_PersonAttendSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stAttendDay, sizeof(stAttendDay) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
	END_FETCHDATA()

	sendMsg.siType = pRecvMsg->siType;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::TransForm_Set()
{
	// ���� ��Ŷ
	sDBRequest_TransForm_Set *pRecvMsg = (sDBRequest_TransForm_Set *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_TransForm_Set sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRANSFORM_SET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiCharID = pRecvMsg->uiCharID;


	// DB Query
	BEGIN_SP( "rsp_TransFormSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->uiNowTransFormKind, sizeof(pRecvMsg->uiNowTransFormKind));
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bTransFormMode, sizeof(pRecvMsg->bTransFormMode));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "NowTransFormKind = %d, TransFormMode = %d"
			, pRecvMsg->uiNowTransFormKind, pRecvMsg->bTransFormMode
			);
		sDBLogMsg_QueryString LogMsg("TransForm Set", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TransForm_AddChar()
{
	// ���� ��Ŷ
	sDBRequest_Transform_AddChar *pRecvMsg = (sDBRequest_Transform_AddChar *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Transform_AddChar sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRANSFORM_ADDCHAR;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiCharID = pRecvMsg->uiCharID;


	// DB Query
	BEGIN_SP( "rsp_TransFormAddChar" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siCharKind, sizeof(pRecvMsg->siCharKind));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siTimeMeterRateType, sizeof(pRecvMsg->siTimeMeterRateType));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siAddTime, sizeof(pRecvMsg->siAddTime));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos));
		SetItemParam(&pRecvMsg->clItem);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
		m_odbc.GetData(&sendMsg.siCharKind);
		m_odbc.GetData(&sendMsg.siTimeMeterRateType);
		m_odbc.GetData(&sendMsg.siAddTime);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetItemData(&sendMsg.clLeftItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siItemPos = pRecvMsg->siItemPos;
	sendMsg.clItem.Set(&pRecvMsg->clItem);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "CharKind = %d, ItemPos = %d"
			, pRecvMsg->siCharKind, pRecvMsg->siItemPos
			);
		sDBLogMsg_QueryString LogMsg("TransForm Add Char", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}
void DBGameProtocol::TransForm_AddTime()
{
	// ���� ��Ŷ
	sDBRequest_Transform_AddTime *pRecvMsg = (sDBRequest_Transform_AddTime *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Transform_AddTime sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TRANSFORM_ADDTIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiCharID = pRecvMsg->uiCharID;


	// DB Query
	BEGIN_SP( "rsp_TransFormAddTime" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siCharKind, sizeof(pRecvMsg->siCharKind));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siAddTime, sizeof(pRecvMsg->siAddTime));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
		m_odbc.GetData(&sendMsg.siCharKind);
		m_odbc.GetData(&sendMsg.siAddTime);
		m_odbc.GetData(&sendMsg.siTotalTime);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "CharKind = %d, AddTime = %d, TotalTime = %d"
			, pRecvMsg->siCharKind, pRecvMsg->siAddTime, sendMsg.siTotalTime
			);
		sDBLogMsg_QueryString LogMsg("TransForm AddTime", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TimeMeterRate_AddTime()
{
	// ���� ��Ŷ
	sDBRequest_TimeMeterRate_AddTime *pRecvMsg = (sDBRequest_TimeMeterRate_AddTime *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_TimeMeterRate_AddTime sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TIMEMETERRATE_ADDTIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiCharID = pRecvMsg->uiCharID;


	// DB Query
	BEGIN_SP( "rsp_TimeMeterRateTimeAdd" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siTimeMeterRateType, sizeof(pRecvMsg->siTimeMeterRateType));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siAddTime, sizeof(pRecvMsg->siAddTime));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos));
		SetItemParam(&pRecvMsg->clItem);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
		m_odbc.GetData(&sendMsg.siTimeMeterRateType);
		m_odbc.GetData(&sendMsg.siAddTime);
		m_odbc.GetData(&sendMsg.siLeftTime);
		m_odbc.GetData(&sendMsg.siItemPos);
		GetItemData(&sendMsg.clItem);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetItemData(&sendMsg.clLeftItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siItemPos = pRecvMsg->siItemPos;
	sendMsg.clItem.Set(&pRecvMsg->clItem);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "TimeMeterRateType = %d, AddTime = %d, LeftTime = %d, ItemPos = %d"
			, sendMsg.siTimeMeterRateType, pRecvMsg->siAddTime, sendMsg.siLeftTime, pRecvMsg->siItemPos
			);
		sDBLogMsg_QueryString LogMsg("TimeMeterRate AddTime", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::TimeMeterRate_useTime()
{
	// ���� ��Ŷ
	sDBRequest_TimeMeterRate_UseTime *pRecvMsg = (sDBRequest_TimeMeterRate_UseTime *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_TimeMeterRate_UseTime sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_TIMEMETERRATE_USETIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.uiCharID = pRecvMsg->uiCharID;


	// DB Query
	BEGIN_SP( "rsp_TimeMeterRateTimeUse" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siTimeMeterRateType, sizeof(pRecvMsg->siTimeMeterRateType));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siUseTime, sizeof(pRecvMsg->siUseTime));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
		m_odbc.GetData(&sendMsg.siTimeMeterRateType);
		m_odbc.GetData(&sendMsg.siUseTime);
		m_odbc.GetData(&sendMsg.siLeftTime);
	END_FETCHDATA()

	sendMsg.bUpdate = pRecvMsg->bUpdate;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "TimeMeterRateType = %d, UseTime = %d, LeftTime = %d"
			, sendMsg.siTimeMeterRateType, pRecvMsg->siUseTime, sendMsg.siLeftTime
			);
		sDBLogMsg_QueryString LogMsg("TimeMeterRate UseTime", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::CharOption()
{
	// ���� ��Ŷ
	sDBRequest_Char_Option *pRecvMsg = (sDBRequest_Char_Option *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Char_Option sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHAR_OPTION;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;


	// DB Query
	BEGIN_SP( "rsp_PersonOptionSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		SetCharOptionInfo(&pRecvMsg->clPCharOptioninfo);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
	END_FETCHDATA()

	memcpy(&sendMsg.clPCharOptioninfo, &pRecvMsg->clPCharOptioninfo, sizeof(sendMsg.clPCharOptioninfo));

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::CharGetOption()
{
	// ���� ��Ŷ
	sDBRequest_Char_Get_Option *pRecvMsg = (sDBRequest_Char_Get_Option *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Char_Get_Option sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHAR_GET_OPTION;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;


	// DB Query
	BEGIN_SP( "rsp_PersonOptionGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
		GetCharOptionInfo(&sendMsg.clPCharOptioninfo);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetHomePage()
{
	// ���� ��Ŷ
	sDBRequest_Set_HomePageURL *pRecvMsg = (sDBRequest_Set_HomePageURL *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Set_HomePageURL sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SET_HOMEPAGE_URL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;


	// DB Query
	BEGIN_SP( "rsp_PersonHomePageSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szText, sizeof(pRecvMsg->szText));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
	END_FETCHDATA()

	sendMsg.siPersonID = pRecvMsg->siPersonID;
	MStrCpy(sendMsg.szText, pRecvMsg->szText, sizeof(sendMsg.szText));

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetHomePage()
{
	// ���� ��Ŷ
	sDBRequest_Get_HomePageURL *pRecvMsg = (sDBRequest_Get_HomePageURL *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Get_HomePageURL sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GET_HOMEPAGE_URL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;


	// DB Query
	BEGIN_SP( "rsp_PersonHomePageGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(sendMsg.szText, sizeof(sendMsg.szText));
	END_FETCHDATA()

	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::HelperDailyCountSet()
{
	//// ���� ��Ŷ
	sDBRequest_Question_Daily_Count_Set* pRecvMsg = (sDBRequest_Question_Daily_Count_Set *)m_pPacket;

	//// ���� ��Ŷ
	sDBResponse_Question_Daily_Count_Set sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_QUESTION_DAILY_COUNT_SET;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_siCharID			  = pRecvMsg->m_siCharID;


	// DB Query
	BEGIN_SP( "rsp_PersonHelperInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->m_DailyCount, sizeof(pRecvMsg->m_DailyCount));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siPersonID);
	END_FETCHDATA()

	sendMsg.m_DailyCount = pRecvMsg->m_DailyCount;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::HelperDailyCountGet()
{
	//// ���� ��Ŷ
	sDBRequest_Question_Daily_Count_Get* pRecvMsg = (sDBRequest_Question_Daily_Count_Get*)m_pPacket;

	//// ���� ��Ŷ
	sDBResponse_Question_Daily_Count_Get sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_QUESTION_DAILY_COUNT_GET;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_siCharID			  = pRecvMsg->m_siCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonHelperInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siPersonID);
		m_odbc.GetData(&sendMsg.m_DailyCount);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::HelperDailyCountDel()
{
	// ���� ��Ŷ
	sDBRequest_Question_Daily_Count_Del *pRecvMsg = (sDBRequest_Question_Daily_Count_Del*)m_pPacket;

	// ���� ��Ŷ
	sDBRequest_Question_Daily_Count_Del sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_QUESTION_DAILY_COUNT_DEL;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_PersonHelperDailyCountDel" )
	END_SP()

	EXEC_SQL()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetMuteTime()
{
	// ���� ��Ŷ
	sDBRequest_SetMuteTime *pRecvMsg = (sDBRequest_SetMuteTime*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetMuteTime sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_SETMUTETIME;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	sendMsg.m_uiCharID = pRecvMsg->m_uiCharID;

	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->m_stMuteTime);

	// DB Query
	BEGIN_SP( "rsp_PersonMuteTimeSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siTargetPersonID, sizeof(pRecvMsg->m_siTargetPersonID));
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
	END_FETCHDATA()

	sendMsg.m_siTargetPersonID = pRecvMsg->m_siTargetPersonID;
	memcpy(&sendMsg.m_stMuteTime, &pRecvMsg->m_stMuteTime, sizeof(sendMsg.m_stMuteTime));

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetMuteTime()
{
	// ���� ��Ŷ
	sDBRequest_GetMuteTime *pRecvMsg = (sDBRequest_GetMuteTime*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GetMuteTime sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_GETMUTETIME;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	sendMsg.m_uiCharID = pRecvMsg->m_uiCharID;

	TIMESTAMP_STRUCT stTS;
	// DB Query
	BEGIN_SP( "rsp_PersonMuteTimeGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &stTS, sizeof(stTS) );
	END_FETCHDATA()

	sendMsg.m_siPersonID = pRecvMsg->m_siPersonID;
	Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.m_stMuteTime, &stTS);

	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::SetValentineWarInfo()
{
	// ���� ��Ŷ
	sDBRequest_Valantine_Request *pRecvMsg = (sDBRequest_Valantine_Request*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Valantine_Request sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_VALANTINE_REQUEST;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	// ������ ����.
	TIMESTAMP_STRUCT stTS;
	SI32 siCouple =	pRecvMsg->clValentineWarInfo.GetCouple()	;	// Ŀ������ �ƴ���
	bool bPrize =	pRecvMsg->clValentineWarInfo.GetPrize()		;	// ��ǰ�� �޾Ҵ��� �ȹ޾Ҵ���.
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->clValentineWarInfo.GetAnswerTime());	// ��Ͻð��� ��������.

	// DB Query
	BEGIN_SP( "rsp_ValentineCoupleInfoSet" )
	m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	m_odbc.SetParam( SQL_INTEGER, &siCouple, sizeof(siCouple));
	m_odbc.SetParam( SQL_BIT, &bPrize, sizeof(bPrize));
	m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS));
	END_SP()
		EXEC_SQL()
		BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
	END_FETCHDATA()
	sendMsg.clValentineWarInfo = pRecvMsg->clValentineWarInfo	;
	sendMsg.m_siPersonID = pRecvMsg->m_siPersonID	;
	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::GetValentineWarInfo()
{
	// ���� ��Ŷ
	sDBRequest_Valantine_Info_Get *pRecvMsg = (sDBRequest_Valantine_Info_Get*)m_pPacket;
	
	sDBResponse_Valantine_Info_Get sendMsg;

	bool bPrize		=	false ;
	SI32 siCouple	=	0;
	TIMESTAMP_STRUCT	stTempTime;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_VALANTINE_INFO_GET;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_ValentineCoupleInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	END_SP()
		EXEC_SQL()
		BEGIN_FETCHDATA()
		m_odbc.GetData( &siCouple);
		m_odbc.GetData( &bPrize);
		m_odbc.GetData( &stTempTime);
	END_FETCHDATA()
		SYSTEMTIME stTS;
		Convert_SYSTEMTIME_from_TIMESTAMP(&stTS, &stTempTime );
		sendMsg.m_clValantineWarInfo.SetCouple( siCouple)	;
		sendMsg.m_clValantineWarInfo.SetPrize( bPrize)		;
		sendMsg.m_clValantineWarInfo.SetAnswerTime( stTS )	;
		sendMsg.m_siPersonID	=	pRecvMsg->m_siPersonID	;
	SendPacket( (BYTE *)&sendMsg );
}
void DBGameProtocol::ValentineWarGetPrize()
{
	// ���� ��Ŷ
	sDBRequest_Valantine_Get_Prize *pRecvMsg = (sDBRequest_Valantine_Get_Prize*)m_pPacket;
	sDBResponse_Valantine_Get_Prize sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_VALANTINE_GET_PRIZE;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_ValentineCouplePrizeSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bGetPrize, sizeof(pRecvMsg->bGetPrize));
	END_SP()
		EXEC_SQL()
		BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResule );
		m_odbc.GetData( &sendMsg.bGetPrize );
	END_FETCHDATA()
	
	// �׽�Ʈ �ڵ�
	sendMsg.m_siPersonID = pRecvMsg->m_siPersonID	;
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetCellularPhoneAuth()
{
	// ���� ��Ŷ
	sDBRequest_Get_Cellular_Phone_Auth *pRecvMsg = (sDBRequest_Get_Cellular_Phone_Auth*)m_pPacket;
	
	sDBResponse_Get_Cellular_Phone_Auth sendMsg;
	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_CELLULAR_PHONE_AUTH;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	sendMsg.m_siCharID   = pRecvMsg->m_siCharID;
	sendMsg.m_siPersonID = pRecvMsg->m_siPersonID;

	// DB Query
	BEGIN_SP( "rsp_RecommendPhoneAuth" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_bIsCellularPhoneAuth );
		m_odbc.GetData( &sendMsg.m_siRecommendPersonID );
		m_odbc.GetData( sendMsg.m_szRecommendPersonName);
		m_odbc.GetData( &sendMsg.m_bFirstCharCreate);
	END_FETCHDATA()
	
		
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetMarry()
{
	// ���� ��Ŷ
	sDBRequest_Set_Marry*	pRecvMsg = (sDBRequest_Set_Marry*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Set_Marry	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SET_MARRY;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_MarrySet" )
		// ûȥ��
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siSourcePersonID),	sizeof(pRecvMsg->m_siSourcePersonID) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siSourceItemPos),		sizeof(pRecvMsg->m_siSourceItemPos) );
		SetItemParam( &pRecvMsg->m_clSourceItem );
		// ûȥ�������
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siDestPersonID),		sizeof(pRecvMsg->m_siDestPersonID) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siDestItemPos),		sizeof(pRecvMsg->m_siDestItemPos) );
		SetItemParam( &pRecvMsg->m_clDestItem );
		// ������
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siLoveLevel),			sizeof(pRecvMsg->m_siLoveLevel) );
		// ��ȥ �����
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siWeddingDateVary),	sizeof(pRecvMsg->m_siWeddingDateVary) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		// �����
		m_odbc.GetData( &sendMsg.m_siResult );
		// ������
		m_odbc.GetData( &sendMsg.m_siLoveLevel );
		// ��ȥ �����
		m_odbc.GetData( &sendMsg.m_siWeddingDateVary );
	END_FETCHDATA()
	
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			// ûȥ��
			m_odbc.GetData( &sendMsg.m_siSourcePersonID );
			m_odbc.GetData( sendMsg.m_szSourceName, sizeof(sendMsg.m_szSourceName) );
			m_odbc.GetData( &sendMsg.m_siSourceItemPos );
			GetItemData( &sendMsg.m_clSourceItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			// ûȥ�������
			m_odbc.GetData( &sendMsg.m_siDestPersonID );
			m_odbc.GetData( sendMsg.m_szDestName, sizeof(sendMsg.m_szDestName) );
			m_odbc.GetData( &sendMsg.m_siDestItemPos );
			GetItemData( &sendMsg.m_clDestItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::GetMarryInfo()
{
	// ���� ��Ŷ
	sDBRequest_Get_Marry_Info*	pRecvMsg = (sDBRequest_Get_Marry_Info*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Get_Marry_Info	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_GET_MARRY_INFO;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siPersonID			= pRecvMsg->m_siPersonID;

	// DB Query
	BEGIN_SP( "rsp_MarryGetInfo" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),	sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		// �� PersonID
		m_odbc.GetData( &sendMsg.m_siPersonID );
		// �����
		m_odbc.GetData( &sendMsg.m_siMatePersonID );
		m_odbc.GetData( sendMsg.m_szMateName, sizeof(sendMsg.m_szMateName) );
		// ��ȥ�����
		m_odbc.GetData( &sendMsg.m_siWeddingDateVary );
		m_odbc.GetData( &sendMsg.m_siItemGetDateVary );	// ��ȥ ����� ������ ���� ��
		// ������ ����
		m_odbc.GetData( &sendMsg.m_siUseDateVary );		// ������ ���ð�
		m_odbc.GetData( &sendMsg.m_siLimitDateVary );	// ������ �ð�
		m_odbc.GetData( &sendMsg.m_siLoveLevel );		// ������
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::MarryItemGetSet()
{
	// ���� ��Ŷ
	sDBRequest_Marry_ItemGet_Set*	pRecvMsg = (sDBRequest_Marry_ItemGet_Set*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Marry_ItemGet_Set	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_MARRY_ITEMGET_SET;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_Marry_ItemGetDateVary_Set" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),			sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siItemGetDateVary),	sizeof(pRecvMsg->m_siItemGetDateVary) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		// �� PersonID
		m_odbc.GetData( &sendMsg.m_siPersonID );
		// �����
		m_odbc.GetData( &sendMsg.m_siItemGetDateVary );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::MarryLoveSet()
{
	// ���� ��Ŷ
	sDBRequest_Marry_Love_Set*	pRecvMsg = (sDBRequest_Marry_Love_Set*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Marry_Love_Set	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_MARRY_LOVE_SET;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_Marry_LoveSet" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),			sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siUseHourVary),		sizeof(pRecvMsg->m_siUseHourVary) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siLimitHourVary),		sizeof(pRecvMsg->m_siLimitHourVary) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siLoveLevel),			sizeof(pRecvMsg->m_siLoveLevel) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		// �� PersonID
		m_odbc.GetData( &sendMsg.m_siPersonID );
		// ������ ���� ����
		m_odbc.GetData( &sendMsg.m_siUseHourVary );
		m_odbc.GetData( &sendMsg.m_siLimitHourVary );
		m_odbc.GetData( &sendMsg.m_siLoveLevel );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::PersonSlotNumChange()
{
	// ���� ��Ŷ
	sDBRequest_PersonSlotNumChange*	pRecvMsg = (sDBRequest_PersonSlotNumChange*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_PersonSlotNumChange	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_PERSONSLOTNUMCHANGE;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );

	sendMsg.m_uiCharID				= pRecvMsg->m_uiCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonSlotNumChange" )
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->m_siFromIndex),		sizeof(pRecvMsg->m_siFromIndex) );
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->m_siToIndex),		sizeof(pRecvMsg->m_siToIndex) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),		sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );

}



void DBGameProtocol::SkillBookMagicInfoGet()
{
	// ���� ��Ŷ
	sDBRequest_SkillBook_MagicInfo_Get*	pRecvMsg = (sDBRequest_SkillBook_MagicInfo_Get*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SkillBook_MagicInfo_Get	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SKILLBOOK_MAGICINFO_GET;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;

	TIMESTAMP_STRUCT stStartTime;

	// DB Query
	BEGIN_SP( "rsp_SkillBookMagicInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
	END_FETCHDATA()

	SI32 siCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if( SKILLBOOK_MAGIC_NUM <= siCount ) break;
			m_odbc.GetData( &sendMsg.m_iIndex[siCount] );
			m_odbc.GetData( &sendMsg.m_clAddSkillBookMagic[siCount].m_siSkillUnique );
			m_odbc.GetData( &sendMsg.m_clAddSkillBookMagic[siCount].m_siMagicKind );
			ZeroMemory( &stStartTime, sizeof(stStartTime) );
			m_odbc.GetData( &stStartTime, 0 ); 
			Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_clAddSkillBookMagic[siCount].m_stTime, &stStartTime );
			siCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	siCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if( MAX_SKILLBOOK_SUBMAGIC_NUM <= siCount ) break;
			m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic[siCount].m_siSkillUnique );
			m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic[siCount].m_siParentMagicKind );
			m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic[siCount].m_siMagicKind );
			ZeroMemory( &stStartTime, sizeof(stStartTime) );
			m_odbc.GetData( &stStartTime, 0 ); 
			Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_clAddSkillBookSubMagic[siCount].m_stTime, &stStartTime );
			siCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::SkillBookMagicAdd()
{
	// ���� ��Ŷ
	sDBRequest_SkillBook_Magic_Add*	pRecvMsg = (sDBRequest_SkillBook_Magic_Add*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SkillBook_Magic_Add	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SKILLBOOK_MAGIC_ADD;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;
			
	TIMESTAMP_STRUCT stStartTime;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stStartTime, &pRecvMsg->m_clAddSkillBookMagic.m_stTime );

	// DB Query 
	BEGIN_SP( "rsp_SkillBookMagicSet" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,							sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->m_iIndex,								sizeof(pRecvMsg->m_iIndex	) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookMagic.m_siSkillUnique,	sizeof(pRecvMsg->m_clAddSkillBookMagic.m_siSkillUnique	) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookMagic.m_siMagicKind,		sizeof(pRecvMsg->m_clAddSkillBookMagic.m_siMagicKind	) );
		m_odbc.SetParam( SQL_TIMESTAMP,	&stStartTime,										sizeof(stStartTime) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_iIndex );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookMagic.m_siSkillUnique );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookMagic.m_siMagicKind );
		ZeroMemory( &stStartTime, sizeof(stStartTime) );
		m_odbc.GetData( &stStartTime ); 
		Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_clAddSkillBookMagic.m_stTime, &stStartTime );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SkillBookMagicDel()
{
	// ���� ��Ŷ
	sDBRequest_SkillBook_Magic_Del*	pRecvMsg = (sDBRequest_SkillBook_Magic_Del*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SkillBook_Magic_Del	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SKILLBOOK_MAGIC_DEL;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;

	TIMESTAMP_STRUCT stStartTime;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stStartTime, &pRecvMsg->m_clAddSkillBookMagic.m_stTime );

	// DB Query
	BEGIN_SP( "rsp_SkillBookMagicDel" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,							sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->m_iIndex,								sizeof(pRecvMsg->m_iIndex	) );
        m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookMagic.m_siSkillUnique,	sizeof(pRecvMsg->m_clAddSkillBookMagic.m_siSkillUnique	) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookMagic.m_siMagicKind,		sizeof(pRecvMsg->m_clAddSkillBookMagic.m_siMagicKind	) );
		m_odbc.SetParam( SQL_TIMESTAMP,	&stStartTime,										sizeof(stStartTime) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_iIndex );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookMagic.m_siSkillUnique );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookMagic.m_siMagicKind );
		ZeroMemory( &stStartTime, sizeof(stStartTime) );
		m_odbc.GetData( &stStartTime, 0 ); 
		Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_clAddSkillBookMagic.m_stTime, &stStartTime );
	END_FETCHDATA()
	
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SkillBookSubMagicAdd()
{
	// ���� ��Ŷ
	sDBRequest_SkillBook_SubMagic_Add*	pRecvMsg = (sDBRequest_SkillBook_SubMagic_Add*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SkillBook_SubMagic_Add	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SKILLBOOK_SUBMAGIC_ADD;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;

	TIMESTAMP_STRUCT stStartTime;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stStartTime, &pRecvMsg->m_clAddSkillBookSubMagic.m_stTime );

	// DB Query
	BEGIN_SP( "rsp_SkillBookSubMagicAdd" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,									sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookSubMagic.m_siSkillUnique,		sizeof(pRecvMsg->m_clAddSkillBookSubMagic.m_siSkillUnique	) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookSubMagic.m_siParentMagicKind,	sizeof(pRecvMsg->m_clAddSkillBookSubMagic.m_siParentMagicKind	) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookSubMagic.m_siMagicKind,			sizeof(pRecvMsg->m_clAddSkillBookSubMagic.m_siMagicKind	) );
		m_odbc.SetParam( SQL_TIMESTAMP,	&stStartTime,												sizeof(stStartTime) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic.m_siSkillUnique );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic.m_siParentMagicKind );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic.m_siMagicKind );
		ZeroMemory( &stStartTime, sizeof(stStartTime) );
		m_odbc.GetData( &stStartTime, 0 ); 
		Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_clAddSkillBookSubMagic.m_stTime, &stStartTime );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SkillBookSubMagicDel()
{
	// ���� ��Ŷ
	sDBRequest_SkillBook_SubMagic_Del*	pRecvMsg = (sDBRequest_SkillBook_SubMagic_Del*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SkillBook_SubMagic_Del	sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SKILLBOOK_SUBMAGIC_DEL;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;
	sendMsg.m_bTimeOver				= pRecvMsg->m_bTimeOver;
	
	TIMESTAMP_STRUCT stStartTime;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stStartTime, &pRecvMsg->m_clAddSkillBookSubMagic.m_stTime );

	// DB Query
	BEGIN_SP( "rsp_SkillBookSubMagicDel" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,									sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookSubMagic.m_siSkillUnique,		sizeof(pRecvMsg->m_clAddSkillBookSubMagic.m_siSkillUnique	) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookSubMagic.m_siParentMagicKind,	sizeof(pRecvMsg->m_clAddSkillBookSubMagic.m_siParentMagicKind	) );
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_clAddSkillBookSubMagic.m_siMagicKind,			sizeof(pRecvMsg->m_clAddSkillBookSubMagic.m_siMagicKind	) );
		m_odbc.SetParam( SQL_TIMESTAMP,	&stStartTime,												sizeof(stStartTime) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic.m_siSkillUnique );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic.m_siParentMagicKind );
		m_odbc.GetData( &sendMsg.m_clAddSkillBookSubMagic.m_siMagicKind );
		ZeroMemory( &stStartTime, sizeof(stStartTime) );
		m_odbc.GetData( &stStartTime, 0 ); 
		Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_clAddSkillBookSubMagic.m_stTime, &stStartTime );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}
