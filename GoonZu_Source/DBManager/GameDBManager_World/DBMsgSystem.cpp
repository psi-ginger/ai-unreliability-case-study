#include "DBGameProtocol.h"
//#include "DBMsg.h"
#include "DBMsg-System.h"
#include "DBMsg-House.h"
#include "DBMsg-Vote.h"
#include "DBMsg-Person.h"
#include "DBMsg-WorldMoney.h"
/*
class cltTest
{
public:
	sPacketHeader		packetHeader;
	SI16				siResult;			// 결과값. (성공 1:  실패 : 0)

	SI16				siCharID;
	SI32				siBuyerPersonID;	

	SI16				siShopperCharID;
	SI32				siShopperPersonID;	

	SI16				siBuyerInvPos;		// Buyer에게 구입한 아이템을 넣어야하는 Inventory 위치 
	SI16				siShopperInvPos;	// Shoper의 해당 Inventory 위치 

	cltItem				clBuyerInvItem;		// Buyer의 Inventory에 설정되어야하는 Item 정보 
	cltItem				clShopperInvItem;	// Shopper Inventory에 설정되어야하는 Item 정보 

	SI32				siBuyerMoney;		// Buyer의 보유 현금 
	SI32				siShopperMoney;		// Buyer의 보유 현금 

	SI32				siItemCost;			// 비용 지불 돈 
	SI16				siShowCaseIndex;	// Showcase Index
	SI32				siBuyItemNum;		// clBuyItem.siItemNum 의 정보를 받아 몇개를 샀는지 인지한다.
	
	cltTest()
	{
		ZeroMemory(this, sizeof(cltTest));
	}

};
*/
void DBGameProtocol::SetDefinedNumber()
{
	SI32 siMaxRealEsatePerPerson	= MAX_REAL_ESTATE_NUMBER_PER_PERSON;
	SI32 siMaxInvestNumber			= MAX_INVEST_NUMBER;
	SI32 siMaxStockKindInAccount	= MAX_STOCK_KIND_IN_ACOUNT;
	SI08 siMaxSkillIndex			= MAX_SKILL_NUMBER;

	/*
	cltTest cltest;
	cltest.siCharID = 10203;
	*/
	SI32 siResult = 0;
	// DB Query
	BEGIN_SP( "rsp_GameDefinedNumberSet" )
		m_odbc.SetParam( SQL_INTEGER, &siMaxRealEsatePerPerson, sizeof(siMaxRealEsatePerPerson) );
		m_odbc.SetParam( SQL_INTEGER, &siMaxInvestNumber,		sizeof(siMaxInvestNumber) );
		m_odbc.SetParam( SQL_INTEGER, &siMaxStockKindInAccount,	sizeof(siMaxStockKindInAccount) );
		m_odbc.SetParam( SQL_TINYINT, &siMaxSkillIndex,			sizeof(siMaxSkillIndex) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0);
/*		m_odbc.GetData( &cltest.siResult, 0);
		m_odbc.GetData( &cltest.siBuyerPersonID, 0);
		m_odbc.GetData( &cltest.siShopperPersonID, 0 );
		m_odbc.GetData( &cltest.siBuyerInvPos, 0 );
		m_odbc.GetData( &cltest.siShopperInvPos, 0 );
*/
	END_FETCHDATA()
}

void DBGameProtocol::Ping()
{
	// 받은 패킷 
	sDBRequest_Ping *pRecvMsg = (sDBRequest_Ping *)m_pPacket;

	// 보낼 패킷 
	sDBResponse_Ping sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PING;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.siSelect = pRecvMsg->siSelect;
	sendMsg.dwTick = pRecvMsg->dwTick;

	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::StartGame()
{
	// 받은 패킷
	sDBRequest_StartGame *pRecvMsg = (sDBRequest_StartGame *)m_pPacket;

	// 보낼 패킷
	sDBResponse_StartGame sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_STARTGAME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sDBResponse_WorldMoney_SetAveragePrice sendMsgAcu;
	sendMsgAcu.packetHeader.usCmd = DBMSG_RESPONSE_WORLDMONEY_SETAVERAGEPRICE;
	sendMsgAcu.packetHeader.usSize = sizeof(sendMsgAcu);

	// DB Query
	BEGIN_SP( "rsp_SystemStartGame" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsgAcu.siResult, 0 );
			m_odbc.GetData( &sendMsgAcu.siAverageWorldMoneyPrice, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );	
	SendPacket( (BYTE *)&sendMsgAcu );	
}

// [지연] 군주데이미션 : 미션을 받은 날짜를 디비에 저장
void DBGameProtocol::GoonzuDayMissionSet()
{
	// 받은 패킷
	sDBRequest_GoonzuDayMissionInfo_Set* pRecvMsg = (sDBRequest_GoonzuDayMissionInfo_Set*)m_pPacket;

	sDBResponse_GoonzuDayMissionInfo_Set SendMsg;


	TIMESTAMP_STRUCT stTime;
	ZeroMemory( &stTime, sizeof(stTime) );
	
	Convert_TIMESTAMP_from_SYSTEMTIME( &stTime, &pRecvMsg->m_MissionGetTime );

	// DB Query
	BEGIN_SP( "rsp_GoonzuDayMissionInfoSet" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stTime, sizeof(stTime) );
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siCount,	sizeof(pRecvMsg->siCount) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
	m_odbc.GetData( &SendMsg.m_siPersonID, 0);
	m_odbc.GetData( &stTime, 0);
	m_odbc.GetData( &SendMsg.siCount, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&SendMsg );	
}
 
// [지연] 군주데이미션 : 최종적으로 수행한 군주데이 미션의 수행 날짜를 서버에 알려줌
void DBGameProtocol::GoonzuDayMissionGet()
{
	// 받은 패킷
	sDBRequest_GoonzuDayMissionInfo_Get* pRecvMsg = (sDBRequest_GoonzuDayMissionInfo_Get*)m_pPacket;

	// 보낼 패킷
	sDBResponse_GoonzuDayMissionInfo_Get sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_GOONZUDAYMISSIONINFO_GET;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;
	sendMsg.m_siPersonID			= pRecvMsg->m_siPersonID;

	TIMESTAMP_STRUCT stTimeStamp;
	ZeroMemory( &stTimeStamp, sizeof(stTimeStamp) );

	// DB Query
	BEGIN_SP( "rsp_GoonzuDayMissionInfoGet" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siPersonID );
		/*m_odbc.GetData( &sendMsg.uiYear,	0 );
		m_odbc.GetData( &sendMsg.uiMonth,	0 );
		m_odbc.GetData( &sendMsg.uiDate,	0 );*/
		m_odbc.GetData( &stTimeStamp, sizeof(stTimeStamp) );		// !!!!! Get 하는 데이터 순서 주의
		m_odbc.GetData( &sendMsg.siCount );
	END_FETCHDATA()

	// send
	Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.m_MissionGetTime, &stTimeStamp );
	SendPacket( (BYTE*)&sendMsg );

	// 로그 생략
}


void DBGameProtocol::MasterGiveItem()
{
	sDBRequest_MasterGiveItem *pRecvMsg		= (sDBRequest_MasterGiveItem *)m_pPacket;

	sDBResponse_MasterGiveItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MASTERGIVEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	sendMsg.clItem.Set( &pRecvMsg->clItem );
	memcpy( sendMsg.szMsg, pRecvMsg->szMsg, MAX_POSTOFFICE_MSG_SIZE );
	sendMsg.szMsg[ MAX_POSTOFFICE_MSG_SIZE - 1 ] = NULL;

	// DB Query
	BEGIN_SP( "rsp_SystemLevelRangePersonIDGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->minLevelRange, sizeof(pRecvMsg->minLevelRange) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->maxLevelRange,	sizeof(pRecvMsg->maxLevelRange) );
	END_SP()

	EXEC_SQL()

	BOOL bSend = FALSE;
	sendMsg.usCount = 0;

	BEGIN_FETCHDATA()	
		m_odbc.GetData( &sendMsg.siPersonID[ sendMsg.usCount ], 0 );
		++sendMsg.usCount;

		if( sendMsg.usCount == 1000 ) {
			
			SendPacket( (BYTE *)&sendMsg );
			sendMsg.usCount = 0;

			bSend = TRUE;

		} else {
			bSend = FALSE;
		}
	END_FETCHDATA()

	if( bSend == FALSE && sendMsg.usCount > 0 ) {
		SendPacket( (BYTE *)&sendMsg );
	}
}

// 해당하는 레벨의 유저에게 일괄적으로 아이템을 역참으로 지급한다
void DBGameProtocol::GiveItem()
{
	// 받은 패킷
	sDBRequest_GiveItem *pRecvMsg = (sDBRequest_GiveItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GiveItem sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GIVEITEM;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siDateVary = pRecvMsg->clDate.GetDateVary();

	// PCK : 로그용 패킷 추가
	sendMsg.minLevelRange = pRecvMsg->minLevelRange;
	sendMsg.maxLevelRange = pRecvMsg->maxLevelRange;
	sendMsg.clItem.Set( &pRecvMsg->clItem);
	MStrCpy(sendMsg.szMsg, pRecvMsg->szMsg, sizeof(sendMsg.szMsg));

	// DB Query
	BEGIN_SP( "rsp_PostItemGive" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->minLevelRange, sizeof(pRecvMsg->minLevelRange) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->maxLevelRange, sizeof(pRecvMsg->maxLevelRange) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bCreateSID, sizeof(pRecvMsg->bCreateSID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szMsg, sizeof(pRecvMsg->szMsg) );
		m_odbc.SetParam( SQL_INTEGER, &siDateVary, sizeof(siDateVary) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siSendNumber, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Level : %d ~ %d, ItemNum = %d, SendNumber = %d, SendDate = %d"
				,	pRecvMsg->minLevelRange, pRecvMsg->maxLevelRange, pRecvMsg->clItem.siItemNum, sendMsg.siSendNumber, pRecvMsg->clDate.GetDateVary()
				);
		sDBLogMsg_QueryString LogMsg("Give Item", 0, 0, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::DBSystemInfo()
{
	// 받은 패킷
	sDBRequest_DBSystemInfo *pRecvMsg = (sDBRequest_DBSystemInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DBSystemInfo sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DBSYSTEMINFO;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	TIMESTAMP_STRUCT stTs;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stTs, &pRecvMsg->stTime);

	// DB Query
	BEGIN_SP( "rsp_SystemInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stTs, sizeof(stTs));
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siType, 0 );
		m_odbc.GetData( &sendMsg.siValue, 0 );
		m_odbc.GetData( &stTs ); Convert_SYSTEMTIME_from_TIMESTAMP( &sendMsg.stTime, &stTs);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::VoteInit()
{
	// 받은 패킷
	sDBRequest_VoteInit *pRecvMsg = (sDBRequest_VoteInit *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_VoteInit sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VOTEINIT;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siMasterCharID = pRecvMsg->siMasterCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemVoteInit" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMasterPersonID, sizeof(pRecvMsg->siMasterPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siMasterPersonID, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::DBInitComplete()
{
	sDBRequest_DBInitComplete *pclMsg = (sDBRequest_DBInitComplete *)m_pPacket;
	
	sDBResponse_DBInitComplete sendMsg( pclMsg->siSelect, pclMsg->siStep );
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::UserCharInfo()
{
	// 받은 패킷
	sDBRequest_UserCharInfo *pRecvMsg = (sDBRequest_UserCharInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_UserCharInfo sendMsg;
	
	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_USERCHARINFO;
	sendMsg.packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.usCharID			= pRecvMsg->usCharID;
	sendMsg.bConnectSwitch		= pRecvMsg->bConnectSwitch;
	sendMsg.siRequestType		= pRecvMsg->siRequestType;

	// DB Query
	BEGIN_SP( "rsp_UserCharInfoGet" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName, sizeof(pRecvMsg->szPersonName) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRequestPersonID, sizeof(pRecvMsg->siRequestPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( sendMsg.szAccountID,						sizeof(sendMsg.szAccountID) );
		m_odbc.GetData( sendMsg.szPassword,							sizeof(sendMsg.szPassword) );
		m_odbc.GetData( &sendMsg.clTotalMoney.itMoney );
		sendMsg.bConnectSwitch = pRecvMsg->bConnectSwitch;
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siLevel );
		m_odbc.GetData( &sendMsg.siMoney );
		m_odbc.GetData( &sendMsg.siMapIndex );
		m_odbc.GetData( &sendMsg.siX );
		m_odbc.GetData( &sendMsg.siY );
 		m_odbc.GetData( &sendMsg.clBankMoney.itMoney );
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.m_clBlockLog[0].m_uiWarnMode );
		m_odbc.GetData( &sendMsg.m_clBlockLog[0].m_uiBlockMode );
		m_odbc.GetData( sendMsg.m_clBlockLog[0].m_szBlockReason,	sizeof(sendMsg.m_clBlockLog[0].m_szBlockReason) );
		m_odbc.GetData( sendMsg.m_clBlockLog[0].m_szStartDate,		sizeof(sendMsg.m_clBlockLog[0].m_szStartDate) );
		m_odbc.GetData( sendMsg.m_clBlockLog[0].m_szEndDate,		sizeof(sendMsg.m_clBlockLog[0].m_szEndDate) );
		m_odbc.GetData( sendMsg.szRecommender,						sizeof(sendMsg.szRecommender) );
		m_odbc.GetData( sendMsg.szFatherName,						sizeof(sendMsg.szFatherName) );
		m_odbc.GetData( &sendMsg.siStudentLevel );
		m_odbc.GetData( sendMsg.szMakeDate,							sizeof(sendMsg.szMakeDate) );
	END_FETCHDATA()

	char personname[5][MAX_PLAYER_NAME];
	
	for( int i = 0; i < 5; ++i ) personname[ i ][ 0 ] = NULL;

	
	// 같은 계정의 다른 캐릭터 이름.
	int  count = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( personname[ count++ ], MAX_PLAYER_NAME );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	memcpy( sendMsg.szOtherPersonName1, personname[ 0 ], MAX_PLAYER_NAME );
	memcpy( sendMsg.szOtherPersonName2, personname[ 1 ], MAX_PLAYER_NAME );

	/*
	// 사용자가 보유한 아이템 내역
	SI32 siPos = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siPos, 0 );
		GetItemData( &sendMsg.clItem[siPos] );
	END_FETCHDATA()
	END_NEXTRECORDSET()
	*/

	// 소환수 정보
	SI16 siSummonUnique = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siSummonUnique, 0 );
		if ( MAX_SUMMON_OWN <= siSummonUnique )		continue;
		GetSummon( &sendMsg.clSummonInfo.clSummon[siSummonUnique] );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// 소환수 정보
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siWorldTradeMoney, 0 );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// 유저 블럭로그를 가져온다
	if ( 1 == sendMsg.siResult )
	{
		BEGIN_SP( "rsp_BlockLogGet" )
			m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szPersonName, sizeof(pRecvMsg->szPersonName) );
		END_SP()

		EXEC_SQL()

		SI32 siGetResult	= 0;
		SI32 siGetCount		= 1;	//	0번은 현재 상태이므로 1번부터 시작이다

		BEGIN_FETCHDATA()
			m_odbc.GetData( &siGetResult );
		END_FETCHDATA()

		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				if ( siGetCount > MAX_BLOCKLOG_NUM ) break;
				m_odbc.GetData( &sendMsg.m_clBlockLog[siGetCount].m_uiWarnMode );
				m_odbc.GetData( &sendMsg.m_clBlockLog[siGetCount].m_uiBlockMode );
				m_odbc.GetData( sendMsg.m_clBlockLog[siGetCount].m_szBlockReason,	sizeof(sendMsg.m_clBlockLog[siGetCount].m_szBlockReason) );
				m_odbc.GetData( sendMsg.m_clBlockLog[siGetCount].m_szStartDate,		sizeof(sendMsg.m_clBlockLog[siGetCount].m_szStartDate) );
				m_odbc.GetData( sendMsg.m_clBlockLog[siGetCount].m_szEndDate,		sizeof(sendMsg.m_clBlockLog[siGetCount].m_szEndDate) );
				siGetCount++;
			END_FETCHDATA()
		END_NEXTRECORDSET()

	}

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, ""
				);
		sDBLogMsg_QueryString LogMsg("User Info Get", pRecvMsg->siRequestPersonID, sendMsg.siPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::UserCharItemInfo()
{

	sDBRequest_UserCharItemInfo		*pRecvMsg = (sDBRequest_UserCharItemInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_UserCharItemInfo	sendMsg;
	sDBResponse_UserCharItemInfo2	sendMsg2;

	// 첫번째
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_USERCHARITEMINFO;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;
	sendMsg.m_siPersonID			= pRecvMsg->m_siRequestPersonID;

	// 두번째
	sendMsg2.m_packetHeader.usCmd	= DBMSG_RESPONSE_USERCHARITEMINFO2;
	sendMsg2.m_packetHeader.usSize	= sizeof( sendMsg2 );
	sendMsg2.m_usCharID				= pRecvMsg->m_usCharID;
	sendMsg2.m_siPersonID			= pRecvMsg->m_siRequestPersonID;
	

	// DB Query
	BEGIN_SP( "rsp_PersonIDGetByName" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->m_szPersonName,			sizeof(pRecvMsg->m_szPersonName) );
	END_SP()

	EXEC_SQL()

	SI32 siGetPersonID	= 0;
	SI16 siItemPos		= 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &siGetPersonID );
	END_FETCHDATA()

	if ( 1 != sendMsg.m_siResult )
	{
		return;
	}

	// 인벤토리 아이템
	BEGIN_SP( "rsp_PersonItemGet" )
		m_odbc.SetParam( SQL_INTEGER, &siGetPersonID, sizeof(siGetPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siItemPos, 0);	if(siItemPos >= MAX_ITEM_PER_PERSON) continue;
		GetItemData( &sendMsg.m_clItem[siItemPos] );	
	END_FETCHDATA()

	SendPacket( (BYTE*)&sendMsg );
	
	//==========================================================================================
	// 용량이 커서 두번으로 나눔
	//==========================================================================================
	// 공간상자 아이템
	BEGIN_SP( "rsp_PersonSpaceBoxListGet" )
		m_odbc.SetParam( SQL_INTEGER, &siGetPersonID, sizeof(siGetPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg2.m_siResult );
	END_FETCHDATA()

	// 공간상자
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siItemPos); if(siItemPos >= MAX_ITEM_SPACE_BOX) continue;
			GetItemData( &sendMsg2.m_clSpaceBox[siItemPos] );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// 보물상자
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siItemPos); if(siItemPos >= MAX_ITEM_TREASURE_BOX) continue;
			GetItemData( &sendMsg2.m_clTreasureBox[siItemPos] );
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	// send
	SendPacket( (BYTE*)&sendMsg2 );
}

void DBGameProtocol::CharInfoForHanXNoti()
{
	// 받은 패킷
	sDBRequest_CharInfoForHanXNoti *pRecvMsg = (sDBRequest_CharInfoForHanXNoti *)m_pPacket;

	// 보낼 패킷
	sDBResponse_CharInfoForHanXNoti ResponseMsg;

	ResponseMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHARINFOFORHANXNOTI;
	ResponseMsg.packetHeader.usSize = sizeof( ResponseMsg );
	ResponseMsg.m_siArrayIndex = pRecvMsg->m_siArrayIndex;
	ResponseMsg.m_nIdx = pRecvMsg->m_nIdx;

	memcpy( ResponseMsg.m_szServerID, pRecvMsg->m_szServerID, 50 );
	ResponseMsg.m_szServerID[ 49 ] = NULL;
	memcpy( ResponseMsg.m_szCharName, pRecvMsg->m_szCharName, MAX_PLAYER_NAME );
	ResponseMsg.m_szCharName[ MAX_PLAYER_NAME-1 ] = NULL;

	// 데이터 받는 패킷
	sDBResponse_UserCharInfo sendMsg;

	SI32 siRequestPersonID = 0;
	// DB Query
	BEGIN_SP( "rsp_UserCharInfoGet" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->m_szCharName, sizeof(pRecvMsg->m_szCharName) );
		m_odbc.SetParam( SQL_INTEGER, &siRequestPersonID, sizeof(siRequestPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( sendMsg.szAccountID, sizeof(sendMsg.szAccountID) );
		m_odbc.GetData( sendMsg.szPassword, sizeof(sendMsg.szPassword) );
		m_odbc.GetData( &sendMsg.clTotalMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siLevel, 0 );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &sendMsg.siMapIndex, 0 );
		m_odbc.GetData( &sendMsg.siX, 0 );
		m_odbc.GetData( &sendMsg.siY, 0 );
 		m_odbc.GetData( &sendMsg.clBankMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.m_clBlockLog[0].m_uiWarnMode );
		m_odbc.GetData( &sendMsg.m_clBlockLog[0].m_uiBlockMode );
		m_odbc.GetData( sendMsg.m_clBlockLog[0].m_szBlockReason,	sizeof(sendMsg.m_clBlockLog[0].m_szBlockReason) );
		m_odbc.GetData( sendMsg.m_clBlockLog[0].m_szStartDate,		sizeof(sendMsg.m_clBlockLog[0].m_szStartDate) );
		m_odbc.GetData( sendMsg.m_clBlockLog[0].m_szEndDate,		sizeof(sendMsg.m_clBlockLog[0].m_szEndDate) );
		m_odbc.GetData( sendMsg.szRecommender, sizeof(sendMsg.szRecommender) );
		m_odbc.GetData( sendMsg.szFatherName, sizeof(sendMsg.szFatherName) );
		m_odbc.GetData( &sendMsg.siStudentLevel, 0 );
		m_odbc.GetData( sendMsg.szMakeDate, sizeof(sendMsg.szMakeDate) );
	END_FETCHDATA()

	// DB에선 항상 성공이므로, DB에서 넘어온 아이디 정보로 성공여부를 판단한다.
	if(_tcslen(sendMsg.szAccountID) <= 0)
	{
		sendMsg.siResult = 0;
	}

	char personname[5][MAX_PLAYER_NAME];
	
	for( int i = 0; i < 5; ++i ) personname[ i ][ 0 ] = NULL;

	
	// 같은 계정의 다른 캐릭터 이름.
	int  count = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( personname[ count++ ], MAX_PLAYER_NAME );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// 보낼 데이터 처리
	memcpy( ResponseMsg.szOtherPersonName1, personname[ 0 ], MAX_PLAYER_NAME );
	memcpy( ResponseMsg.szOtherPersonName2, personname[ 1 ], MAX_PLAYER_NAME );
	memcpy( ResponseMsg.m_szAccountID, sendMsg.szAccountID, MAX_PLAYER_NAME );

	ResponseMsg.siPersonID	= sendMsg.siPersonID;
	ResponseMsg.siLevel		= sendMsg.siLevel;

	// 사용자가 보유한 아이템 내역
	/*
	SI32 siPos = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siPos, 0 );
		GetItemData( &sendMsg.clItem[siPos] );
	END_FETCHDATA()
	END_NEXTRECORDSET()
	*/

	// 소환수 정보
	SI16 siSummonUnique = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siSummonUnique, 0 );
		if ( MAX_SUMMON_OWN <= siSummonUnique )		continue;
		GetSummon( &sendMsg.clSummonInfo.clSummon[siSummonUnique] );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// 소환수 정보
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siWorldTradeMoney, 0 );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	ResponseMsg.siResult = sendMsg.siResult;

	// send
	SendPacket( (BYTE *)&ResponseMsg );
}

void DBGameProtocol::DBStartInfo()
{
	// 받은 패킷
	sDBRequest_DBStartInfo *pRecvMsg = (sDBRequest_DBStartInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DBStartInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DBSTARTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_SystemDBStartInfo" )
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.clServerStartInfo.siStockDistributionVillageUnique );
		m_odbc.GetData( &sendMsg.siStartBankruptDateVary );
		m_odbc.GetData( &sendMsg.siFourLeafUnique );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetTime()
{
	// 받은 패킷
	sDBRequest_SetTime *pRecvMsg = (sDBRequest_SetTime *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetTime sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETTIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_GameTimeSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siYesr),		sizeof(pRecvMsg->siYesr) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siMonth),		sizeof(pRecvMsg->siMonth) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siDay),		sizeof(pRecvMsg->siDay) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siHour),		sizeof(pRecvMsg->siHour) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siMinute),	sizeof(pRecvMsg->siMinute) );
	END_SP()

	// Execute SQL
	EXEC_SQL()

	// Get Returned Data
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
	END_FETCHDATA()
	
	sendMsg.dwSetTimeClock = pRecvMsg->dwSetTimeClock;

	// send
	SendPacket( (BYTE *)&sendMsg );
}
 

void DBGameProtocol::GetTime()
{
	// 받은 패킷
	sDBRequest_GetTime *pRecvMsg = (sDBRequest_GetTime *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetTime sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETTIME;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_GameTimeGet" )
	END_SP()
	
	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.siYear), 0);
		m_odbc.GetData( &(sendMsg.siMonth), 0);
		m_odbc.GetData( &(sendMsg.siDay), 0);
		m_odbc.GetData( &(sendMsg.siHour), 0);
		m_odbc.GetData( &(sendMsg.siMinute), 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetUserNum()
{
	// 받은 패킷
	sDBRequest_SetUserNum *pRecvMsg = (sDBRequest_SetUserNum *)m_pPacket;

	// DB Query
	BEGIN_SP( "rsp_SystemUserNumSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siUserNum), sizeof(pRecvMsg->siUserNum) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siHangameUserNum), sizeof(pRecvMsg->siHangameUserNum) );		
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siGamaniaUserNum), sizeof(pRecvMsg->siGamaniaUserNum) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siGamePotUserNum), sizeof(pRecvMsg->siGamePotUserNum) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siNDEUserNum), sizeof(pRecvMsg->siNDEUserNum) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siNEXONUserNum), sizeof(pRecvMsg->siNEXONUserNum) );
	END_SP()
	
	EXEC_SQL()
}

void DBGameProtocol::Block()
{
	// 받은 패킷
	sDBRequest_Block *pRecvMsg = (sDBRequest_Block *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_Block sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BLOCK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_Block" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siBlockMode), sizeof(pRecvMsg->siBlockMode) );
	END_SP()
	
	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.siBlockMode), 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetDailyNewChar()
{
	// 받은 패킷
	sDBRequest_GetDailyNewChar *pRecvMsg = (sDBRequest_GetDailyNewChar *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetDailyNewChar sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETDAILYNEWCHAR;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
//	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_SystemNewCharGet" )
	END_SP()
	
	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.siNewCharNum), 0);
		m_odbc.GetData( &(sendMsg.siDelCharNum), 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetInstanceProfitVillage()
{
	// 받은 패킷
	sDBRequest_SetInstanceProfit_Village *pRecvMsg = (sDBRequest_SetInstanceProfit_Village *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetInstanceProfit_Village sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETINSTANCEPROFIT_VILLAGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemInstanceProfitVillageSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bStart, sizeof(pRecvMsg->bStart));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.siVillageUnique), 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ChangeInstanceProfit()
{
	// 받은 패킷
	sDBRequest_Change_InstanceProfit *pRecvMsg = (sDBRequest_Change_InstanceProfit *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Change_InstanceProfit sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGE_INSTANCEPROFIT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemInstanceProfitChange" )
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bVillage, sizeof(pRecvMsg->bVillage));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
		m_odbc.GetData( &(sendMsg.siMoney), 0);
		m_odbc.GetData( &(sendMsg.siLeftMoney), 0);
		m_odbc.GetData( &(sendMsg.bVillage), 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::VoteKingAggregate()
{
	// 받은 패킷
	sDBRequest_VoteKingAggregate *pRecvMsg = (sDBRequest_VoteKingAggregate *)m_pPacket;

	// 보낼 패킷
	sDBResponse_VoteKingAggregate sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_VOTEKINGAGGREGATE;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;
	sendMsg.m_siType				= pRecvMsg->m_siType;

	// DB Query
	BEGIN_SP( "rsp_SystemVoteKingAggregate" )
	END_SP()

	EXEC_SQL()

	sendMsg.m_siCandidatureNum = 0;
	SI32 siVoteIndex = 0;
	BEGIN_FETCHDATA()
		if(sendMsg.m_siCandidatureNum >= MAX_CANDIDATURE_NUM) continue;
		m_odbc.GetData( &siVoteIndex, 0);
		m_odbc.GetData( &sendMsg.m_siVoteCount[siVoteIndex - 1], 0);
		sendMsg.m_siCandidatureNum++;
	END_FETCHDATA()

	sendMsg.m_siResult	= 1;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ServerStatisticsSet()
{
	// 받은 패킷
	sDBRequest_WriteStatistics *pRecvMsg = (sDBRequest_WriteStatistics *)m_pPacket;

	// DB Query
	BEGIN_SP( "rsp_ServerStatisticsSet" )
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->year, sizeof(pRecvMsg->year));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->month, sizeof(pRecvMsg->month));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->day, sizeof(pRecvMsg->day));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->hour, sizeof(pRecvMsg->hour));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->minute, sizeof(pRecvMsg->minute));

		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->UserNum, sizeof(pRecvMsg->UserNum));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->NPCNum, sizeof(pRecvMsg->NPCNum));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->ItemNum, sizeof(pRecvMsg->ItemNum));
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->MarketOrderNum, sizeof(pRecvMsg->MarketOrderNum));

		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clInputMoney_NPC.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clInputMoney_NPC.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clInputMoney_Quest.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clInputMoney_Quest.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clInputMoney_War.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clInputMoney_War.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clInputMoney_Father.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clInputMoney_Father.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clInputMoney_Recommender.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clInputMoney_Recommender.itMoney));

		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_NPC.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_NPC.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_Dead.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_Dead.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_Repair.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_Repair.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_Stock.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_Stock.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_clMarketTax.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_clMarketTax.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_clStockBidTax.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_clStockBidTax.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_clVillageWar.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_clVillageWar.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_NPCPayment.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_NPCPayment.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clOutputMoney_QuestFee.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clOutputMoney_QuestFee.itMoney));

		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clTotalBudget.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clTotalBudget.itMoney));
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->clCurrencyInfo.clReservedBudget.itMoney, sizeof(pRecvMsg->clCurrencyInfo.clReservedBudget.itMoney));
	END_SP()

	EXEC_SQL()
}

void DBGameProtocol::ServerStatisticsGet()
{

	// 받은 패킷
	sDBRequest_InitStatistics *pRecvMsg = (sDBRequest_InitStatistics *)m_pPacket;

	// 보낼 패킷
	sDBResponse_InitStatistics sendMsg;

	ZeroMemory( &sendMsg, sizeof( sendMsg ));

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_INITSTATISTICS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_ServerStatisticsGet" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.UserNum, 0);
		m_odbc.GetData(&sendMsg.NPCNum, 0);
		m_odbc.GetData(&sendMsg.ItemNum, 0);
		m_odbc.GetData(&sendMsg.MarketOrderNum, 0);

		m_odbc.GetData(&sendMsg.clCurrencyInfo.clInputMoney_NPC.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clInputMoney_Quest.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clInputMoney_War.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clInputMoney_Father.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clInputMoney_Recommender.itMoney, 0);

		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_NPC.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_Dead.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_Repair.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_Stock.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_clMarketTax.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_clStockBidTax.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_clVillageWar.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_NPCPayment.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clOutputMoney_QuestFee.itMoney, 0);

		m_odbc.GetData(&sendMsg.clCurrencyInfo.clTotalBudget.itMoney, 0);
		m_odbc.GetData(&sendMsg.clCurrencyInfo.clReservedBudget.itMoney, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetWarRank()
{
	// 받은 패킷
	sDBRequest_GetWarRankInfo *pRecvMsg = (sDBRequest_GetWarRankInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetWarRankInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETWARRANKINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.clWarRankInfo.m_warmapindex = pRecvMsg->m_warmapindex;

	// DB Query
	BEGIN_SP( "rsp_SystemWarRankGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_warmapindex, sizeof(pRecvMsg->m_warmapindex) );
	END_SP()

	EXEC_SQL()

	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		if(siIndex >= MAX_WARRANK_PERSON_PER_MAP) break;
		m_odbc.GetData( &sendMsg.clWarRankInfo.clRankPerson[siIndex].m_ranknum, 0 );
		m_odbc.GetData( &sendMsg.clWarRankInfo.clRankPerson[siIndex].m_personid, 0 );
		m_odbc.GetData( sendMsg.clWarRankInfo.clRankPerson[siIndex].m_szPlayerName, sizeof(sendMsg.clWarRankInfo.clRankPerson[siIndex].m_szPlayerName) );
		m_odbc.GetData( &sendMsg.clWarRankInfo.clRankPerson[siIndex].m_score, 0 );
		siIndex++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetWarRank()
{
	// 받은 패킷
	sDBRequest_SetWarRankInfo *pRecvMsg = (sDBRequest_SetWarRankInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetWarRankInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETWARRANKINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.clWarRankInfo.m_warmapindex = pRecvMsg->clWarRankInfo.m_warmapindex;

	// DB Query
	BEGIN_SP( "rsp_SystemWarRankSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clWarRankInfo.m_warmapindex, sizeof(pRecvMsg->clWarRankInfo.m_warmapindex) );
		for(int i = 0; i < MAX_WARRANK_PERSON_PER_MAP; i++)
		{
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clWarRankInfo.clRankPerson[i].m_ranknum, sizeof(pRecvMsg->clWarRankInfo.clRankPerson[i].m_ranknum) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clWarRankInfo.clRankPerson[i].m_personid, sizeof(pRecvMsg->clWarRankInfo.clRankPerson[i].m_personid) );
			m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->clWarRankInfo.clRankPerson[i].m_szPlayerName, sizeof(pRecvMsg->clWarRankInfo.clRankPerson[i].m_szPlayerName) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clWarRankInfo.clRankPerson[i].m_score, sizeof(pRecvMsg->clWarRankInfo.clRankPerson[i].m_score) );
		}
	END_SP()

	EXEC_SQL()

	SI32 siIndex = 0;
	BEGIN_FETCHDATA()
		if(siIndex >= MAX_WARRANK_PERSON_PER_MAP) break;
		m_odbc.GetData( &sendMsg.clWarRankInfo.clRankPerson[siIndex].m_ranknum, 0 );
		m_odbc.GetData( &sendMsg.clWarRankInfo.clRankPerson[siIndex].m_personid, 0 );
		m_odbc.GetData( sendMsg.clWarRankInfo.clRankPerson[siIndex].m_szPlayerName, sizeof(sendMsg.clWarRankInfo.clRankPerson[siIndex].m_szPlayerName) );
		m_odbc.GetData( &sendMsg.clWarRankInfo.clRankPerson[siIndex].m_score, 0 );
		siIndex++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetLiquidation()
{
	// 받은 패킷
	sDBRequest_SetLiquidation *pRecvMsg = (sDBRequest_SetLiquidation *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetLiquidation sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETLIQUIDATION;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemSetLiquidation" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siLiquidationVillageNumber, sizeof(pRecvMsg->siLiquidationVillageNumber));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSetLiquidationVillageDate, sizeof(pRecvMsg->siSetLiquidationVillageDate));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &(sendMsg.siResult), 0);
	END_FETCHDATA()

	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::VillageBankruptcy()
{
	// 받은 패킷
	sDBRequest_VillageBankruptcy *pRecvMsg = (sDBRequest_VillageBankruptcy *)m_pPacket;

	// 보낼 패킷
	sDBResponse_VillageBankruptcy sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_VILLAGEBANKRUPTCY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemVillageBankruptcy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siDateVary, sizeof(pRecvMsg->siDateVary));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siType, sizeof(pRecvMsg->siType));
	END_SP()

	EXEC_SQL()

	SI32 siCount = 0;
	BEGIN_FETCHDATA()
		if(siCount >= sizeof(sendMsg.siVillageUnique) / sizeof(sendMsg.siVillageUnique[0])) break;
		m_odbc.GetData( &sendMsg.siVillageUnique[siCount++] );
	END_FETCHDATA()
	
	sendMsg.siDateVary = pRecvMsg->siDateVary;
	sendMsg.siResult = 1;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "DateVary = %d, Type = %d"
			, pRecvMsg->siDateVary, pRecvMsg->siType
			);
		sDBLogMsg_QueryString LogMsg("Set VillageBankruptcy Date", NULL, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::ServerEvnetRank()				
{
	sDBRequest_Server_Event_Rank_Info *pRecvMsg = (sDBRequest_Server_Event_Rank_Info *)m_pPacket;

	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Rank_Info sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_SERVER_EVENT_RANK_INFO;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_siReason			  = pRecvMsg->m_siReason;

	// DB Query
	BEGIN_SP( "rsp_ServerEventRankInfo" )
	END_SP()

	EXEC_SQL()

	GetServerEventInfo(&sendMsg);

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ServerEvnetServerScoreAdd()				
{
	sDBRequest_Server_Event_Server_Score_Add *pRecvMsg = (sDBRequest_Server_Event_Server_Score_Add *)m_pPacket;

	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Rank_Info sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_SERVER_EVENT_RANK_INFO;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	bool bScoreType = SERVER_EVENT_SCORE_TYPE_TOTAL;

	// DB Query
	BEGIN_SP( "rsp_ServerEventScoreAdd" )
		m_odbc.SetParam( SQL_BIT, &bScoreType, sizeof(bScoreType));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siServerUnique, sizeof(pRecvMsg->m_siServerUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siOpenCount, sizeof(pRecvMsg->m_siOpenCount));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->m_siAddScore, sizeof(pRecvMsg->m_siAddScore));
	END_SP()

	EXEC_SQL()

	GetServerEventInfo(&sendMsg);

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ServerEvnetClear()				
{
	sDBRequest_Server_Event_Clear *pRecvMsg = (sDBRequest_Server_Event_Clear *)m_pPacket;

	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Rank_Info sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_SERVER_EVENT_RANK_INFO;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_ServerEventClear" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siServerUnique, sizeof(pRecvMsg->m_siServerUnique));
	END_SP()

	EXEC_SQL()

	//GetServerEventInfo(&sendMsg);

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ServerEvnetServerDailyScoreAdd()				
{
	sDBRequest_Server_Event_Server_Daily_Score_Add *pRecvMsg = (sDBRequest_Server_Event_Server_Daily_Score_Add *)m_pPacket;

	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Rank_Info sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_SERVER_EVENT_RANK_INFO;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_siReason			  = pRecvMsg->m_siReason;

	bool bScoreType = SERVER_EVENT_SCORE_TYPE_DAILY;

	sendMsg.m_siReason = pRecvMsg->m_siReason;

	// DB Query
	BEGIN_SP( "rsp_ServerEventScoreAdd" )
		m_odbc.SetParam( SQL_BIT, &bScoreType, sizeof(bScoreType));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siServerUnique, sizeof(pRecvMsg->m_siServerUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siOpenCount, sizeof(pRecvMsg->m_siOpenCount));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->m_siAddScore, sizeof(pRecvMsg->m_siAddScore));
	END_SP()

	EXEC_SQL()

	GetServerEventInfo(&sendMsg);

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ServerEventServerBufKind()
{
	sDBRequest_Server_Event_Server_Buf_Kind *pRecvMsg = (sDBRequest_Server_Event_Server_Buf_Kind*)m_pPacket;
	
	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Server_Buf_Kind sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_SERVER_EVENT_SERVER_BUF_KIND;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	

	sendMsg.m_siServerBufKind = pRecvMsg->m_siServerBufKind;
	sendMsg.m_siResult = 1;

	BEGIN_SP( "rsp_ServerEventBufKind" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siServerBufKind, sizeof(pRecvMsg->m_siServerBufKind));
	END_SP()

	EXEC_SQL()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ServerEventQuestInfoSet()
{
	sDBRequest_Server_Event_Server_Quest_Info_Set *pRecvMsg = (sDBRequest_Server_Event_Server_Quest_Info_Set *)m_pPacket;

	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Server_Quest_Info_Set sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_SERVER_EVENT_QUEST_INFO_SET;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_ServerEventQuestInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siEventType, sizeof(pRecvMsg->m_siEventType));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siCondition1, sizeof(pRecvMsg->m_siCondition1));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siCondition2, sizeof(pRecvMsg->m_siCondition2));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siNeedClearAmount, sizeof(pRecvMsg->m_siNeedClearAmount));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siRewardItemUnique, sizeof(pRecvMsg->m_siRewardItemUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siRewardItemAmount, sizeof(pRecvMsg->m_siRewardItemAmount));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siLimitMinute, sizeof(pRecvMsg->m_siLimitMinute));
		for(int siCharerIdx = 0; siCharerIdx < Const_ServerEvent_Max_QuestCharacter; siCharerIdx++ )
		{
			m_odbc.SetParam( SQL_BIT, &pRecvMsg->m_bCharacter[siCharerIdx], sizeof(pRecvMsg->m_bCharacter[siCharerIdx]));
		}
		for(int m_siInFailCharacterIdx = 0; m_siInFailCharacterIdx < Const_ServerEvent_Max_QuestCharacter * Const_ServerEvent_MulNum; m_siInFailCharacterIdx++ )
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siInFailCharacter[m_siInFailCharacterIdx], sizeof(pRecvMsg->m_siInFailCharacter[m_siInFailCharacterIdx]));
		}
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siCharacterIndex, sizeof(pRecvMsg->m_siCharacterIndex));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siNowAmount, sizeof(pRecvMsg->m_siNowAmount));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
	END_FETCHDATA()


	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::ServerEventQuestInfoGet()
{
	sDBRequest_Server_Event_Server_Quest_Info_Get *pRecvMsg = (sDBRequest_Server_Event_Server_Quest_Info_Get *)m_pPacket;

	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Server_Quest_Info_Get sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_SERVER_EVENT_QUEST_INFO_GET;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_ServerEventQuestInfoGet" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siEventType);
		m_odbc.GetData(&sendMsg.m_siCondition1);
		m_odbc.GetData(&sendMsg.m_siCondition2);
		m_odbc.GetData(&sendMsg.m_siNeedClearAmount);
		m_odbc.GetData(&sendMsg.m_siRewardItemUnique);
		m_odbc.GetData(&sendMsg.m_siRewardItemAmount);
		m_odbc.GetData(&sendMsg.m_siLimitMinute);
		for(int siCharerIdx = 0; siCharerIdx < Const_ServerEvent_Max_QuestCharacter; siCharerIdx++ )
		{
			m_odbc.GetData(&sendMsg.m_bCharacter[siCharerIdx]);
		}
		for(int m_siInFailCharacterIdx = 0; m_siInFailCharacterIdx < Const_ServerEvent_Max_QuestCharacter * Const_ServerEvent_MulNum; m_siInFailCharacterIdx++ )
		{
			m_odbc.GetData(&sendMsg.m_siInFailCharacter[m_siInFailCharacterIdx]);
		}
		m_odbc.GetData(&sendMsg.m_siCharacterIndex);
		m_odbc.GetData(&sendMsg.m_siNowAmount);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ServerEventPersonScoreSet()
{
	sDBRequest_Server_Event_Server_Person_Score_Set *pRecvMsg = (sDBRequest_Server_Event_Server_Person_Score_Set *)m_pPacket;

	// DB Query
	BEGIN_SP( "rsp_ServerEventPersonScoreSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->m_siScore, sizeof(pRecvMsg->m_siScore));
	END_SP()

	EXEC_SQL()

}


void DBGameProtocol::ServerEventPersonScoreGet()
{
	sDBRequest_Server_Event_Server_Person_Score_Get *pRecvMsg = (sDBRequest_Server_Event_Server_Person_Score_Get *)m_pPacket;

	// 보낼 패킷(모든서버에)
	sDBResponse_Server_Event_Server_Person_Score_Get sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SERVER_EVENT_PERSON_SCORE_GET;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_uiCharID				= pRecvMsg->m_uiCharID;

	// DB Query
	BEGIN_SP( "rsp_ServerEventPersonScoreGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siPersonID);
		m_odbc.GetData(&sendMsg.m_siLevel);
		m_odbc.GetData(&sendMsg.m_siScore);
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::SetFourLeaf()
{
	// 받은 패킷
	sDBRequest_SetFourLeaf *pRecvMsg = (sDBRequest_SetFourLeaf*)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetFourLeaf sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETFOURLEAF;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_SystemFourLeaf" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFourLeafUnique, sizeof(pRecvMsg->siFourLeafUnique));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siFourLeafUnique);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::GetElixir()
{
	// 받은 패킷
	sDBRequest_Elixir_Get *pRecvMsg = (sDBRequest_Elixir_Get*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Elixir_Get sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_ELIXIR_GET;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_uiCharID			  = pRecvMsg->m_uiCharID;

	sendMsg.m_siPersonID		  = pRecvMsg->m_siPersonID;
	
	// DB Query
	BEGIN_SP( "rsp_ElixirGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
		m_odbc.GetData(&sendMsg.m_siPersonID);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siEndMinuteVary);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_uiMultiple);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siPoint);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siStr);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siHnd);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siDex);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siMag);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siWis);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siVit);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetElixir()
{
	// 받은 패킷
	sDBRequest_Elixir_Set *pRecvMsg = (sDBRequest_Elixir_Set*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Elixir_Set sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_ELIXIR_SET;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	
	sendMsg.m_uiCharID			 = pRecvMsg->m_uiCharID;	
	sendMsg.m_siElixirItemUnique = pRecvMsg->m_siElixirItemUnique;	
	sendMsg.m_siElixirItemPos	 = pRecvMsg->m_siElixirItemPos;		
	sendMsg.m_siWaterItemUnique	 = pRecvMsg->m_siWaterItemUnique;	
	sendMsg.m_siWaterItemPos	 = pRecvMsg->m_siWaterItemPos;		
	sendMsg.m_siWaterItemNum	 = pRecvMsg->m_siWaterItemNum;		
	sendMsg.m_bUseItem			 = pRecvMsg->m_bUseItem;

	// DB Query
	BEGIN_SP( "rsp_ElixirSet" )
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->m_clElixirInfo.m_siEndMinuteVary, sizeof(pRecvMsg->m_clElixirInfo.m_siEndMinuteVary)	);
		m_odbc.SetParam( SQL_TINYINT,  &pRecvMsg->m_clElixirInfo.m_uiMultiple,		sizeof(pRecvMsg->m_clElixirInfo.m_uiMultiple)		);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clElixirInfo.m_siPoint,			sizeof(pRecvMsg->m_clElixirInfo.m_siPoint)			);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clElixirInfo.m_siStr, 			sizeof(pRecvMsg->m_clElixirInfo.m_siStr)			);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clElixirInfo.m_siHnd, 			sizeof(pRecvMsg->m_clElixirInfo.m_siHnd)			);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clElixirInfo.m_siDex, 			sizeof(pRecvMsg->m_clElixirInfo.m_siDex)			);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clElixirInfo.m_siMag, 			sizeof(pRecvMsg->m_clElixirInfo.m_siMag)			);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clElixirInfo.m_siWis, 			sizeof(pRecvMsg->m_clElixirInfo.m_siWis)			);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clElixirInfo.m_siVit, 			sizeof(pRecvMsg->m_clElixirInfo.m_siVit)			);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
		m_odbc.GetData(&sendMsg.m_siPersonID);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siEndMinuteVary);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_uiMultiple);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siPoint);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siStr);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siHnd);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siDex);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siMag);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siWis);
		m_odbc.GetData(&sendMsg.m_clElixirInfo.m_siVit);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetBalanceCanUseCurrency()
{
	// 받은 패킷
	sDBRequest_SetBalanceCanUseCurrency		*pRecvMsg = (sDBRequest_SetBalanceCanUseCurrency*)m_pPacket;

	// 보낼 패킷	
	sDBResponse_SetBalanceCanUseCurrency		sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_SETBALANCECANUSECURRENCY;
	sendMsg.packetHeader.usSize	= sizeof( sendMsg );

	sendMsg.m_siCharID = pRecvMsg->m_siCharID;

	// DB Query
	BEGIN_SP( "rsp_BalanceCurrency" )
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siMoney, sizeof(pRecvMsg->m_siMoney));
		m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_bType, sizeof(pRecvMsg->m_bType));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siMoney);
	END_FETCHDATA()

		// send
		SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::GetBalanceCanUseCurrency()
{
	// 받은 패킷
	sDBRequest_GetBalanceCanUseCurrency		*pRecvMsg = (sDBRequest_GetBalanceCanUseCurrency*)m_pPacket;

	// 보낼 패킷	
	sDBResponse_GetBalanceCanUseCurrency	sendMsg;

	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_GETBALANCECANUSECURRENCY;
	sendMsg.packetHeader.usSize	= sizeof( sendMsg );

	sendMsg.m_siCharID = pRecvMsg->m_siCharID;
	sendMsg.m_bCalledGM = pRecvMsg->m_bCalledGM;

	
	// DB Query
	BEGIN_SP( "rsp_BalanceCurrency" )
		m_odbc.SetParam(SQL_BIGINT, &pRecvMsg->m_siMoney, sizeof(pRecvMsg->m_siMoney));
		m_odbc.SetParam(SQL_TINYINT, &pRecvMsg->m_bType,	sizeof(pRecvMsg->m_bType));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult);
		m_odbc.GetData( &sendMsg.m_siMoney);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


// 사용안함, sDBRequest_PersonSimpleInfo이거 사용함
void DBGameProtocol::PromoterInfoGet()
{
	// 받은 패킷
	//sDBReuest_Promoter_Info_Get* pRecvMsg = (sDBReuest_Promoter_Info_Get*)m_pPacket;

	//// 보낼 패킷
	//sDBResponse_Promoter_Info_Get sendMsg;

	//sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_PROMOTER_INFO_GET;
	//sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	//cltSimplePerson clPromoterInfo;

	//// DB Query
	//BEGIN_SP( "rsp_PromoterInfoGet" )
	//	m_odbc.SetParam( SQL_VARCHAR,  &pRecvMsg->m_szPromoterName, sizeof(pRecvMsg->m_szPromoterName));
	//END_SP()

	//EXEC_SQL()

	//BEGIN_FETCHDATA()
	//	m_odbc.GetData(&sendMsg.m_siResult);
	//	GetSimplePerson( &clPromoterInfo );
	//	
	//END_FETCHDATA()

	//m_odbc.GetData(&sendMsg.m_siPersonID);
	//m_odbc.GetData(&sendMsg.m_siCharKind);
	//m_odbc.GetData(&sendMsg.m_siLevel);		

	//// send
	//SendPacket( (BYTE *)&sendMsg );
}
//-------------------------------------------------------------------------------------------------------------------
//[성웅] 라쿤이벤트 군주 이벤트로 변경
//-------------------------------------------------------------------------------------------------------------------
void DBGameProtocol::SetGoonzuRaccoonEvent()
{
	// 받은 패킷
	sDBRequest_GoonzuRaccoonEvent_SaveInfo* pRecvMsg = (sDBRequest_GoonzuRaccoonEvent_SaveInfo*)m_pPacket;
	// 보낼 패킷
	sDBResponse_GoonzuRaccoonEvent_SaveInfo sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_GOONZURACCOONEVENT_SETINFO;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );

	TIMESTAMP_STRUCT stTS;
	ZeroMemory(&stTS, sizeof(stTS));

	SI32 siRaccoonEventIndex	=	0	;
	SI32 siTime = 0 ;
	SI32 siMonsterNum = 0;
	SI32 siMapindex =	0	;
	SI32 MapX =	0	;
	SI32 MapY =	0	;

	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->m_SetTime);

	// DB Query
	BEGIN_SP( "rsp_GoonzuRaccoonEvent_SetInfo" )
		m_odbc.SetParam( SQL_TIMESTAMP,	&stTS,	sizeof(stTS));
		SetGoonzuRaccoonEventSetinfo( &pRecvMsg->m_GoonzuRaccoonEventData[0] )	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[0].m_RocData[0])	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[0].m_RocData[1])	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[0].m_RocData[2])	;

		m_odbc.SetParam( SQL_TIMESTAMP,	&stTS,	sizeof(stTS));
		SetGoonzuRaccoonEventSetinfo( &pRecvMsg->m_GoonzuRaccoonEventData[1] )	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[1].m_RocData[0])	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[1].m_RocData[1])	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[1].m_RocData[2])	;

		m_odbc.SetParam( SQL_TIMESTAMP,	&stTS,	sizeof(stTS));
		SetGoonzuRaccoonEventSetinfo( &pRecvMsg->m_GoonzuRaccoonEventData[2] )	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[2].m_RocData[0])	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[2].m_RocData[1])	;
		SetGoonzuRaccoonEventSetinfo_Map(&pRecvMsg->m_GoonzuRaccoonEventData[2].m_RocData[2])	;
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );	
}
void DBGameProtocol::GetGoonzuRaccoonEvent()
{
	// 받은 패킷
	sDBRequest_GoonzuRaccoonEvent_GetInfo* pRecvMsg = (sDBRequest_GoonzuRaccoonEvent_GetInfo*)m_pPacket;
	// 보낼 패킷
	sDBResponse_GoonzuRaccoonEvent_GetInfo sendMsg;

	sendMsg.m_packetHeader.usCmd  = DBMSG_RESPONSE_GOONZURACCOONEVENT_GETINFO;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	
	SI32 siTime	=	0	;
	SI32 siMonsterNum	=	0	;
	//TCHAR* pMapName	= NULL	;
	SI32 siMapindex =	0	;
	SI32 siRocX	=	0	;
	SI32 siRocY	=	0	;
	SI32 siRaccoonIndex = 0		;

	TIMESTAMP_STRUCT stTS;
	ZeroMemory(&stTS, sizeof(stTS));

	// DB Query
	BEGIN_SP( "rsp_GoonzuRaccoonEvent_GetInfo" )	
	END_SP()
	EXEC_SQL()
		BEGIN_FETCHDATA()
		m_odbc.GetData( &stTS, sizeof(stTS) );	
		m_odbc.GetData(&siTime);
		m_odbc.GetData(&siMonsterNum);
		Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.m_stTime, &stTS);
		sendMsg.m_GoonzuRaccoonEventData[siRaccoonIndex].SetTime(siTime)	;
		sendMsg.m_GoonzuRaccoonEventData[siRaccoonIndex].SetMakeMonsterNum(siMonsterNum)	;
		for ( SI32 siIndex = 0 ; siIndex < 3 ; siIndex++) 
		{
			if(siRaccoonIndex > MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT)	continue	;
			m_odbc.GetData(&siMapindex);
			m_odbc.GetData(&siRocX);
			m_odbc.GetData(&siRocY);
			sendMsg.m_GoonzuRaccoonEventData[siRaccoonIndex].SetMapIndex(siIndex ,siMapindex)	;
			sendMsg.m_GoonzuRaccoonEventData[siRaccoonIndex].SetMapRocX(siIndex ,siRocX)	;
			sendMsg.m_GoonzuRaccoonEventData[siRaccoonIndex].SetMapRocY(siIndex ,siRocY)	;
		}
		siRaccoonIndex++;
		END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );	
}