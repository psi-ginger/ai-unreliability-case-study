#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-Hunt.h"

void DBGameProtocol::GetHuntStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_HUNT;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HuntInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()
	
	EXEC_SQL()
	
	/* 수렵장 기본정보 */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siVillageUnique,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney,	0);
		m_odbc.GetData( &sendMsg.clStrInfo.clHuntStrInfo.siEnterFee,	0);
		m_odbc.GetData( sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
	END_FETCHDATA()

	/* 수렵장 몬스터 창고정보 */
	BEGIN_NEXTRECORDSET()
		SI16 siIndex = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siIndex, 0);
			m_odbc.GetData( &sendMsg.clStrInfo.clHuntStrInfo.clHuntStg[siIndex].siItemUnique,	0);
			m_odbc.GetData( &sendMsg.clStrInfo.clHuntStrInfo.clHuntStg[siIndex].siItemNum,	0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	/* 수렵장 전선배치 정보 */
	BEGIN_NEXTRECORDSET()
		SI32 siIndex = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siIndex, 0);
			m_odbc.GetData( &sendMsg.clStrInfo.clHuntStrInfo.clHuntLineUp[siIndex].siLineUpKind,	0);
			m_odbc.GetData( &sendMsg.clStrInfo.clHuntStrInfo.clHuntLineUp[siIndex].siLineUpKindNum,	0);
			m_odbc.GetData( &sendMsg.clStrInfo.clHuntStrInfo.clHuntLineUp[siIndex].siLineUpKindLevel,	0);
		END_FETCHDATA()
	END_NEXTRECORDSET()

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


void DBGameProtocol::HuntStorageMonsterIn()
{
	// 받은 패킷
	sDBRequest_HuntStorageMonsterIn *pRecvMsg = (sDBRequest_HuntStorageMonsterIn *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HuntStorageMonsterIn sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HUNTSTORAGEMONSTERIN;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_HuntStorageMonsterIn" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromPersonInventoryItemIndex, sizeof(pRecvMsg->siFromPersonInventoryItemIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToHuntStorageInventoryItemIndex, sizeof(pRecvMsg->siToHuntStorageInventoryItemIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siFromPersonInventoryItemIndex, 0 );
			m_odbc.GetData( &sendMsg.siFromPersonInventoryItemUnique, 0 );
			m_odbc.GetData( &sendMsg.siFromPersonInventoryItemNum, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siToHuntStorageInventoryItemIndex, 0 );
			m_odbc.GetData( &sendMsg.siToHuntStorageInventoryItemUnique, 0 );
			m_odbc.GetData( &sendMsg.siToHuntStorageInventoryItemNum, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ItemUnique = %d, Amount = %d"
				,	pRecvMsg->siVillageUnique, sendMsg.siToHuntStorageInventoryItemUnique, sendMsg.siToHuntStorageInventoryItemNum
				);
		sDBLogMsg_QueryString LogMsg("Hunt Stg Input", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

}

void DBGameProtocol::HuntStorageMonsterOut()
{
	// 받은 패킷
	sDBRequest_HuntStorageMonsterOut *pRecvMsg = (sDBRequest_HuntStorageMonsterOut *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HuntStorageMonsterOut sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HUNTSTORAGEMONSTEROUT;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_HuntStorageMonsterOut" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromHuntStorageInventoryItemIndex, sizeof(pRecvMsg->siFromHuntStorageInventoryItemIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToPersonInventoryItemIndex, sizeof(pRecvMsg->siToPersonInventoryItemIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siFromHuntStorageInventoryItemIndex, 0 );
			m_odbc.GetData( &sendMsg.siFromHuntStorageInventoryItemUnique, 0 );
			m_odbc.GetData( &sendMsg.siFromHuntStorageInventoryItemNum, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siToPersonInventoryItemIndex, 0 );
			m_odbc.GetData( &sendMsg.siToPersonInventoryItemUnique, 0 );
			m_odbc.GetData( &sendMsg.siToPersonInventoryItemNum, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Villageunique = %d, InvPos = %d, ItemUnique = %d, Amount = %d"
				,	pRecvMsg->siVillageUnique, sendMsg.siToPersonInventoryItemIndex, sendMsg.siToPersonInventoryItemUnique, sendMsg.siToPersonInventoryItemNum
				);
		sDBLogMsg_QueryString LogMsg("Hunt Stg Output", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HuntMonsterFieldListAdd()
{
	// 받은 패킷
	sDBRequest_HuntMonsterFieldListAdd *pRecvMsg = (sDBRequest_HuntMonsterFieldListAdd *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HuntMonsterFieldListAdd sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HUNTMONSTERFIELDLISTADD;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_HuntMonsterFieldListAdd" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siAddListIndex, sizeof(pRecvMsg->siAddListIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonsterKind, sizeof(pRecvMsg->siMonsterKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonsterNum, sizeof(pRecvMsg->siMonsterNum) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonsterLevel, sizeof(pRecvMsg->siMonsterLevel) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
			m_odbc.GetData( &sendMsg.siAddListIndex, 0 );
			m_odbc.GetData( &sendMsg.siMonsterKind, 0 );
			m_odbc.GetData( &sendMsg.siMonsterNum, 0 );
			m_odbc.GetData( &sendMsg.siMonsterLevel, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, MonsterKind = %d, MonsterNum = %d, MonsterLevel = %d"
				,	sendMsg.siVillageUnique, sendMsg.siMonsterKind, sendMsg.siMonsterNum, sendMsg.siMonsterLevel
				);
		sDBLogMsg_QueryString LogMsg("Hunt Monster Add", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HuntMonsterFieldListDel()
{
	// 받은 패킷
	sDBRequest_HuntMonsterFieldListDel *pRecvMsg = (sDBRequest_HuntMonsterFieldListDel *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HuntMonsterFieldListDel sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HUNTMONSTERFIELDLISTDEL;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siMonsterKind = 0;

	// DB Query
	BEGIN_SP( "rsp_HuntMonsterFieldListDel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siDelListIndex, sizeof(pRecvMsg->siDelListIndex) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siDelListIndex, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, MonsterKind = %d"
				,	sendMsg.siVillageUnique, siMonsterKind
				);
		sDBLogMsg_QueryString LogMsg("Hunt Monster Del", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::HuntStorageDecreaseDollItemNum()
{
	// 받은 패킷
	sDBRequest_HuntStorageDecreaseDollItemNum *pRecvMsg = (sDBRequest_HuntStorageDecreaseDollItemNum *)m_pPacket;

	// 보낼 패킷
	sDBResponse_HuntStorageDecreaseDollItemNum sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HUNTSTORAGEDECREASEDOLLITEMNUM;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
//	sendMsg.siCharID = pRecvMsg->siCharID;


	// DB Query
	for(SI32 siCount = 0; siCount < MAX_HUNT_LINEUP_NUMBER; siCount++)
	{
		if(pRecvMsg->siHuntStorageItemUnique[siCount] > 0)
		{
			BEGIN_SP( "rsp_HuntStgDecreaseItemNum" )
				m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
				m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHuntStorageItemUnique[siCount], sizeof(pRecvMsg->siHuntStorageItemUnique[siCount]) );
				m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHuntStorageItemIndex[siCount], sizeof(pRecvMsg->siHuntStorageItemIndex[siCount]) );
				m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHuntStorageItemDecreaseNum[siCount], sizeof(pRecvMsg->siHuntStorageItemDecreaseNum[siCount]) );
			END_SP()

			EXEC_SQL()

			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
				m_odbc.GetData( &sendMsg.siHuntStorageItemUnique[siCount], 0 );
				m_odbc.GetData( &sendMsg.siHuntStorageItemIndex[siCount], 0 );
				m_odbc.GetData( &sendMsg.siHuntStorageItemNum[siCount], 0 );
			END_FETCHDATA()
		}
	}

	sendMsg.siResult = 1;
	// send
	SendPacket( (BYTE *)&sendMsg );
}



void DBGameProtocol::HuntLeftMonsterStgIn()
{
	// 받은 패킷
	sDBRequest_HuntLeftMonsterStgIn *pRecvMsg = (sDBRequest_HuntLeftMonsterStgIn *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HuntLeftMonsterStgIn sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HUNTLEFTMONSTERSTGIN;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
//	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_HuntLeftMonsterStgIn" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToHuntStorageInventoryItemIndex, sizeof(pRecvMsg->siToHuntStorageInventoryItemIndex) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siToHuntStorageInventoryItemIndex, 0 );
		m_odbc.GetData( &sendMsg.siItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemNum, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

}

void DBGameProtocol::InstanceMapPoint_Insert()
{
	// 받은 패킷
	sDBRequest_InstanceMapPoint_Insert *pRecvMsg = (sDBRequest_InstanceMapPoint_Insert *)m_pPacket;

	// 보낼 패킷
	sDBResponse_InstanceMapPoint_Insert sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_INSTANCEMAPPOINT_INSERT;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	//sendMsg.uiCharID = pRecvMsg->uiCharID;

	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME( &stTS, &pRecvMsg->stClearTime);

	// DB Query
	BEGIN_SP( "rsp_InstantMapYearRankingSet" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siUnique, sizeof(pRecvMsg->siUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPoint, sizeof(pRecvMsg->siPoint) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiYear, sizeof(pRecvMsg->uiYear) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siClearTime, sizeof(pRecvMsg->siClearTime) );
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	return;
}


void DBGameProtocol::InstantMapYearRanking()
{
	// 받은 패킷
	sDBRequest_YearRanking *pRecvMsg = (sDBRequest_YearRanking *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_YearRanking sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_YEARRANKING;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.uiCharID = pRecvMsg->uiCharID;
	
	UI08 uiListPerPage = MAX_HOF_RANKCOUNT;

	// DB Query
	BEGIN_SP( "rsp_InstantMapYearRankingGet" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiYear, sizeof(pRecvMsg->uiYear) );
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bReward, sizeof(pRecvMsg->bReward) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->sRequest_Page, sizeof(pRecvMsg->sRequest_Page) );
		m_odbc.SetParam( SQL_TINYINT, &uiListPerPage, sizeof(uiListPerPage) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siType );
		m_odbc.GetData( &sendMsg.uiYear );
		m_odbc.GetData( &sendMsg.bReward );
		m_odbc.GetData( &sendMsg.siRequestPage );
		m_odbc.GetData( &sendMsg.siMaxPage );
	END_FETCHDATA()

	SI32 siIndex = 0;
	TIMESTAMP_STRUCT stTS;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].uiYear); if(siIndex >= MAX_HOF_RANKCOUNT) break;
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].siGuildUnique);
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].siGuildMarkindex);
			m_odbc.GetData(sendMsg.HOFrList[siIndex].szHOFGuildName, sizeof(sendMsg.HOFrList[siIndex].szHOFGuildName));
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].siClearPoint);
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].ClearTime);
			m_odbc.GetData(&stTS); Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.HOFrList[siIndex].ClearDateTime, &stTS);
			siIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

		// 로그 작성
	if(sendMsg.siResult == 1 && true == sendMsg.bReward)
	{
		char log[512] = "";
		sprintf(log, "Year = %d, Rank1 = %s, Rank2 = %s, Rank3 = %s"
					,sendMsg.uiYear
					,sendMsg.HOFrList[0].szHOFGuildName
					,sendMsg.HOFrList[1].szHOFGuildName
					,sendMsg.HOFrList[2].szHOFGuildName
			);
		sDBLogMsg_QueryString LogMsg("InstantMap NewYear Ranking", 0, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	return;
}

void DBGameProtocol::InstantMapHOFList()
{
	// 받은 패킷
	sDBRequest_HOFList *pRecvMsg = (sDBRequest_HOFList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_HOFList sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HOFLIST;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.uiCharID = pRecvMsg->uiCharID;

	// DB Query
	BEGIN_SP( "rsp_InstantMapHOFList" )
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRequestPageNum, sizeof(pRecvMsg->siRequestPageNum) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siPageMaxCount, sizeof(pRecvMsg->siPageMaxCount) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siType );
		m_odbc.GetData( &sendMsg.siRequestPage );
		m_odbc.GetData( &sendMsg.siMaxPage );
	END_FETCHDATA()

	SI32 siIndex = 0;
	TIMESTAMP_STRUCT stTS;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].uiYear); if(siIndex >= MAX_HOF_RANKCOUNT) break;
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].siGuildUnique);
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].siGuildMarkindex);
			m_odbc.GetData(sendMsg.HOFrList[siIndex].szHOFGuildName, sizeof(sendMsg.HOFrList[siIndex].szHOFGuildName));
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].siClearPoint);
			m_odbc.GetData(&sendMsg.HOFrList[siIndex].ClearTime);
			m_odbc.GetData(&stTS); Convert_SYSTEMTIME_from_TIMESTAMP(&sendMsg.HOFrList[siIndex].ClearDateTime, &stTS);
			siIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	return;
}

