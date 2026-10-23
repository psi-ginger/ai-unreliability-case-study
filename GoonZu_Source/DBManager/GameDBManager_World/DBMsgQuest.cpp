#include "DBGameProtocol.h"
//#include "DBMsg.h"
#include "DBMsg-Person.h"
#include "DBMsg-DailyQuest.h"
#include "DBMsg-DailyQuest2.h"
#include "DBMsg-DailyQuest3.h"

void DBGameProtocol::SetQuestInfo()
{
	// 받은 패킷
	sDBRequest_SetQuestInfo *pRecvMsg = (sDBRequest_SetQuestInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetQuestInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETQUESTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.siQuestType = pRecvMsg->siQuestType;
	sendMsg.clQuestInfo.Set(&pRecvMsg->clQuestInfo);
	
	// DB Query
	BEGIN_SP( "rsp_QuestInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siQuestType), sizeof(pRecvMsg->siQuestType) );
		SetQuestUnitInfo( &(pRecvMsg->clQuestInfo) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char binary[50]="";
		itoa(pRecvMsg->clQuestInfo.siQuestState, binary, 2);
		char log[512] = "";
		sprintf(log, "QuestIndex = %d, State = %032s, CurUnique = %d, CurStep = %d, CurCmd = %d, Para1 = %d, Para2 = %d"
			, pRecvMsg->siQuestType, binary, pRecvMsg->clQuestInfo.siQuestState
			, pRecvMsg->clQuestInfo.siCurQuestUnique, pRecvMsg->clQuestInfo.siCurQuestStep, pRecvMsg->clQuestInfo.siQuestCmd
			, pRecvMsg->clQuestInfo.siQuestPara1, pRecvMsg->clQuestInfo.siQuestPara2
			);
		sDBLogMsg_QueryString LogMsg("QuestInfo Set", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트를 진행한다.
void DBGameProtocol::DailyQuestAdd()
{
	// 받은 패킷
	sDBRequest_DailyQuestAdd *pRecvMsg = (sDBRequest_DailyQuestAdd *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DailyQuestAdd sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUESTADD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_DailyQuestAdd" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonsterKind, sizeof(pRecvMsg->siMonsterKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonsterNum, sizeof(pRecvMsg->siMonsterNum) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRewardItemUnique, sizeof(pRecvMsg->siRewardItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRewardItemNum, sizeof(pRecvMsg->siRewardItemNum) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRewardMoney, sizeof(pRecvMsg->siRewardMoney ));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRewardExp, sizeof(pRecvMsg->siRewardExp) );

		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siQuestTime, sizeof(pRecvMsg->siQuestTime) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLeftTime, sizeof(pRecvMsg->siLeftTime) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siVillagePayFeeMoney, sizeof(pRecvMsg->siVillagePayFeeMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bMoneySelected, sizeof(pRecvMsg->bMoneySelected));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );

		m_odbc.GetData( &sendMsg.siMonsterKind, 0 );
		m_odbc.GetData( &sendMsg.siMonsterNum, 0 );

		m_odbc.GetData( &sendMsg.siRewardItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siRewardItemNum, 0 );
		m_odbc.GetData( &sendMsg.siRewardMoney, 0 );
		m_odbc.GetData( &sendMsg.siRewardExp, 0 );

		m_odbc.GetData( &sendMsg.siFee, 0 );
		m_odbc.GetData( &sendMsg.siQuestTime, 0 );
		m_odbc.GetData( &sendMsg.siLeftTime, 0 );

		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.clStrMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.bMoneySelected, 0 );
	END_FETCHDATA()

	sendMsg.siQuestListIndex	= pRecvMsg->siQuestListIndex;
	sendMsg.siStrType			= pRecvMsg->siStrType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Fee = %I64d, LeftPersonMoney = %I64d, QuestTime = %d, LeftTime = %d, MonsterKind = %d, MonsterNum = %d, RewardItemUnique = %d, RewardItemNum = %d, RewardExp = %d, RewardMoney = %I64d, SelectedMoney = %d"
				,	sendMsg.siFee, sendMsg.siLeftMoney, sendMsg.siQuestTime, sendMsg.siLeftTime, sendMsg.siMonsterKind, sendMsg.siMonsterNum
				,	sendMsg.siRewardItemUnique, sendMsg.siRewardItemNum, sendMsg.siRewardExp
				,	sendMsg.siRewardMoney, sendMsg.bMoneySelected
				);
		sDBLogMsg_QueryString LogMsg("DailyQuest Add", sendMsg.siPersonID, NULL, sendMsg.siRewardItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트를 종료한다.
void DBGameProtocol::DailyQuestDel()
{
	// 받은 패킷
	sDBRequest_DailyQuestDel *pRecvMsg = (sDBRequest_DailyQuestDel *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DailyQuestDel sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUESTDEL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.clRewardItem.Set(&pRecvMsg->clRewardItem);

	// DB Query
	BEGIN_SP( "rsp_DailyQuestDel" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiSuccess, sizeof(pRecvMsg->uiSuccess) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siInventoryIndex, sizeof(pRecvMsg->siInventoryIndex) );
		SetItemParam( &pRecvMsg->clRewardItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiPileSwitch, sizeof(pRecvMsg->uiPileSwitch) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRewardMoney, sizeof(pRecvMsg->siRewardMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bMoneySelected, sizeof(pRecvMsg->bMoneySelected));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRewardExp, sizeof(pRecvMsg->siRewardExp) );
	END_SP()

	EXEC_SQL()


	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.uiSuccess, 0 );
		m_odbc.GetData( &sendMsg.siAddMoney );
		m_odbc.GetData( &sendMsg.siExp, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siInventoryItemIndex, 0 );
		GetItemData( &sendMsg.clInventoryItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()


	sendMsg.siExp = pRecvMsg->siRewardExp;
	sendMsg.siChangedItemNum = pRecvMsg->clRewardItem.siItemNum;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성 - 실패도 로그에 남겨본다 - LEEKH - 2007.04.02
	// Result = -2 : 서버가 요청한 위치에 아이템이 존재 (같은 유니크면 합치는건 가능)
	// Result = -1 : 오류 (경험치, 퀘스트정보 삭제)
	// Result =  0 : 프로시져 에러
	// Result =  1 : 성공
	//if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, Success = %d, CurrentExp = %d, RewardItemNum = %d, LeftInvItemNum = %d, AddMoney = %I64d"
				, sendMsg.siResult, sendMsg.uiSuccess, sendMsg.siExp, pRecvMsg->clRewardItem.siItemNum, sendMsg.clInventoryItem.siItemNum, sendMsg.siAddMoney
				);
		sDBLogMsg_QueryString LogMsg("DailyQuest End", sendMsg.siPersonID, NULL, sendMsg.clInventoryItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트진행중 변경사항을 저장한다.
void DBGameProtocol::DailyQuestChange()
{
	// 받은 패킷
	sDBRequest_DailyQuestChange *pRecvMsg = (sDBRequest_DailyQuestChange *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_DailyQuestChange sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUESTCHANGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siMonsterKind = pRecvMsg->siMonsterKind;

	// DB Query
	BEGIN_SP( "rsp_DailyQuestChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siKillMonsterNum, sizeof(pRecvMsg->siKillMonsterNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLeftTime, sizeof(pRecvMsg->siLeftTime) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siKillMonsterNum, 0 );
		m_odbc.GetData( &sendMsg.siLeftTime, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "LeftTime = %d, KillNumber = %d"
				,	sendMsg.siLeftTime, sendMsg.siKillMonsterNum
				);
		sDBLogMsg_QueryString LogMsg("DailyQuest Change", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 복불복 누적된금액 저장 (보상금액)
void DBGameProtocol::Set_bbbRewardAccumPrice()
{
	// 받은 패킷
	sDBRequest_BBB_Set_RewardMoney *pRecvMsg = (sDBRequest_BBB_Set_RewardMoney*)m_pPacket;

	// 보낼 패킷
	sDBResponse_BBB_Set_RewardMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BBB_SET_REWARD_MONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_bbbTotalRewardMoneySet" )
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->m_siTotalRewardPriceDeposit, sizeof(pRecvMsg->m_siTotalRewardPriceDeposit) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siTotalRewardPriceDeposit, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성 - 생략
}

// 복불복 누적된금액 불러오기 (보상금액)
void DBGameProtocol::Get_bbbRewardAccumPrice()
{
	// 받은 패킷
	sDBRequest_BBB_Get_RewardMoney *pRecvMsg = (sDBRequest_BBB_Get_RewardMoney*)m_pPacket;

	// 보낼 패킷
	sDBResponse_BBB_Get_RewardMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BBB_GET_REWARD_MONEY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_bbbTotalRewardMoneyGet" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
	m_odbc.GetData( &sendMsg.m_siTotalRewardPriceWithDraw, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성 - 생략
}

void DBGameProtocol::SetSpecialQuestInfo()
{
	// 받은 패킷
	sDBRequest_SetSpecialQuestInfo *pRecvMsg = (sDBRequest_SetSpecialQuestInfo *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetSpecialQuestInfo sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETSPECIALQUESTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.clSpecialQuestInfo.Set( &pRecvMsg->clSpecialQuestInfo);

	// DB Query
	BEGIN_SP( "rsp_QuestSpecialInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetQuestInfoCommon( &pRecvMsg->clSpecialQuestInfo );
		SetQuestWordAssemblySwitch( pRecvMsg->bWordAssemblySwitch );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bClearWordSpecialQuest, sizeof(pRecvMsg->bClearWordSpecialQuest) ); // 단어조합이 완성이 됬는지 확인.
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bEventClearWordSpecialQuest, sizeof(pRecvMsg->bEventClearWordSpecialQuest) );
		
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.bClearWordSpecialQuest, 0 );
		m_odbc.GetData( &sendMsg.siClearWordSpecialQuestNum, 0 );	   // 기존 단어 조합 완성 횟 수.	
		m_odbc.GetData( &sendMsg.siEventClearWordSpecialQuestNum, 0 ); // 이벤트 단어 조합 완성 횟 수 - 디비에서 이벤트 기간동안의 퀘 수행 횟수를 저장한것 얻어오는 변수.
																	 
		                                                        
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "QuestUnique = %d, Flag = %d, Para1 = %d, Para2 = %d, Money = %I64d, Exp = %d, ItemNum = %d, FameExp = %d, NCoin = %d, WordAssembly = %d%d%d%d%d%d%d%d%d%d"
				, pRecvMsg->clSpecialQuestInfo.siUnique
				, pRecvMsg->clSpecialQuestInfo.clQuestClearConditionInfo.siFlag
				, pRecvMsg->clSpecialQuestInfo.clQuestClearConditionInfo.siPara1
				, pRecvMsg->clSpecialQuestInfo.clQuestClearConditionInfo.siPara2
				, pRecvMsg->clSpecialQuestInfo.clQuestRewardInfo.siMoney
				, pRecvMsg->clSpecialQuestInfo.clQuestRewardInfo.siExp
				, pRecvMsg->clSpecialQuestInfo.clQuestRewardInfo.siItemNum[0]
				, pRecvMsg->clSpecialQuestInfo.clQuestRewardInfo.siFameExp
				, pRecvMsg->clSpecialQuestInfo.clQuestRewardInfo.siNCoin
				, pRecvMsg->bWordAssemblySwitch[0]
				, pRecvMsg->bWordAssemblySwitch[1]
				, pRecvMsg->bWordAssemblySwitch[2]
				, pRecvMsg->bWordAssemblySwitch[3]
				, pRecvMsg->bWordAssemblySwitch[4]
				, pRecvMsg->bWordAssemblySwitch[5]
				, pRecvMsg->bWordAssemblySwitch[6]
				, pRecvMsg->bWordAssemblySwitch[7]
				, pRecvMsg->bWordAssemblySwitch[8]
				, pRecvMsg->bWordAssemblySwitch[9]
				);
		sDBLogMsg_QueryString LogMsg("Quest SpecialInfo Set", pRecvMsg->siPersonID, NULL, pRecvMsg->clSpecialQuestInfo.clQuestRewardInfo.siItemUnique[0], log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetNamingQuestInfo()
{
	// 받은 패킷
	sDBRequest_SetNamingQuestInfo *pRecvMsg = (sDBRequest_SetNamingQuestInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetNamingQuestInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETNAMINGQUESTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.clNamingQuestInfo.Set(&pRecvMsg->clNamingQuestInfo);

	// DB Query
	BEGIN_SP( "rsp_QuestNamingInfoSet" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.siUnique, sizeof(pRecvMsg->clNamingQuestInfo.siUnique));
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siFlag, sizeof(pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siFlag));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siPara1, sizeof(pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siPara1));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siPara2, sizeof(pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siPara2));
		m_odbc.SetParam( SQL_BIGINT,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siMoney, sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siMoney));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siExp, sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siExp));
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siFameExp, sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siFameExp));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNCoin, sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNCoin));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.bMoveBuf, sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.bMoveBuf));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingIndex, sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingIndex));
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingLevel, sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingLevel));
		for(int i = 0; i < MAX_QUEST_REWARD_NUMBER; i++)
		{
			m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemUnique[i],		sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemUnique[i]));
			m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemNum[i],		sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemNum[i]));
			m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemUseDate[i],	sizeof(pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemUseDate[i]));
		}
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
		{
			char log[512] = "";
			sprintf(log, "Unique = %d, Flag = %d, Para1 = %d, Para2 = %d, Money = %I64d, Exp = %d, Fameexp = %d, NCoin = %d, MoveBuf = %d, NamingIndex = %d, NamingLevel = %d"
				,	pRecvMsg->clNamingQuestInfo.siUnique, pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siFlag, pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siPara1, pRecvMsg->clNamingQuestInfo.clQuestClearConditionInfo.siPara2
				,	pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siMoney, pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siExp, pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siFameExp, pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNCoin
				,	pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.bMoveBuf, pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingIndex, pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingLevel
				);
			sDBLogMsg_QueryString LogMsg("NamingQuest Info Set", pRecvMsg->siPersonID, NULL, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
		for(int j=0; j < MAX_QUEST_REWARD_NUMBER; j++)
		{
			char log[512] = "";
			sprintf(log, "ItemNum = %d, ItemUseDate = %d"
				,	pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemNum[j], pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemUseDate[j]
				);
			sDBLogMsg_QueryString LogMsg("NamingQuest Info Set", pRecvMsg->siPersonID, NULL, pRecvMsg->clNamingQuestInfo.clQuestRewardInfo.siItemUnique[j], log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}
}

void DBGameProtocol::SetPersonNamingInfo()
{
	// 받은 패킷
	sDBRequest_SetPersonNamingInfo *pRecvMsg = (sDBRequest_SetPersonNamingInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetPersonNamingInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETPERSONNAMINGINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_QuestNamingPersonSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLevel, sizeof(pRecvMsg->siLevel));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siIndex );
		m_odbc.GetData( &sendMsg.siLevel );
	END_FETCHDATA()

	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "index = %d, Level = %d"
			,	pRecvMsg->siIndex, pRecvMsg->siLevel
			);
		sDBLogMsg_QueryString LogMsg("NamingQuest Set", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetPersonCurrentNaming()
{
	// 받은 패킷
	sDBRequest_SetPersonCurrentNaming *pRecvMsg = (sDBRequest_SetPersonCurrentNaming *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetPersonCurrentNaming sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETPERSONCURRENTNAMING;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_QuestNamingPersonCurrentSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSelectedIndex, sizeof(pRecvMsg->siSelectedIndex));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSelectedLevel, sizeof(pRecvMsg->siSelectedLevel));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siSelectedIndex );
		m_odbc.GetData( &sendMsg.siSelectedLevel );
	END_FETCHDATA()

	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "index = %d, Level = %d"
			,	pRecvMsg->siSelectedIndex, pRecvMsg->siSelectedLevel
			);
		sDBLogMsg_QueryString LogMsg("NamingQuest Set Current", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetHappyBeanQuestInfo()
{
	// 받은 패킷
	sDBRequest_SetHappyBeanQuestInfo *pRecvMsg = (sDBRequest_SetHappyBeanQuestInfo *)m_pPacket;
/*
	// 보낼 패킷
	sDBResponse_GetHappyBeanClearInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETHAPPYBEANQUESTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;
*/

	SI32 siResult = 0;

	// DB Query
	BEGIN_SP( "rsp_QuestHappyBeanSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		SetQuestInfoCommon( &pRecvMsg->clHappyBeanQuestInfo );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult );
	END_FETCHDATA()

	// send
//	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "QuestUnique = %d, Flag = %d, Para1 = %d, Para2 = %d, Money = %I64d, Exp = %d, ItemNum = %d, FameExp = %d, NCoin = %d"
			, pRecvMsg->clHappyBeanQuestInfo.siUnique
			, pRecvMsg->clHappyBeanQuestInfo.clQuestClearConditionInfo.siFlag
			, pRecvMsg->clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara1
			, pRecvMsg->clHappyBeanQuestInfo.clQuestClearConditionInfo.siPara2
			, pRecvMsg->clHappyBeanQuestInfo.clQuestRewardInfo.siMoney
			, pRecvMsg->clHappyBeanQuestInfo.clQuestRewardInfo.siExp
			, pRecvMsg->clHappyBeanQuestInfo.clQuestRewardInfo.siItemNum[0]
			, pRecvMsg->clHappyBeanQuestInfo.clQuestRewardInfo.siFameExp
			, pRecvMsg->clHappyBeanQuestInfo.clQuestRewardInfo.siNCoin
			);
		sDBLogMsg_QueryString LogMsg("Quest HappyBeanInfo Set", pRecvMsg->siPersonID, NULL, pRecvMsg->clHappyBeanQuestInfo.clQuestRewardInfo.siItemUnique[0], log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GetHappyBeanClearInfo()
{
	// 받은 패킷
	sDBRequest_SetHappyBeanClearInfo *pRecvMsg = (sDBRequest_SetHappyBeanClearInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetHappyBeanClearInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETHAPPYBEANQUESTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;


	SI32 siResult = 0;

	// DB Query
	BEGIN_SP( "rsp_QuestHappyBeanClear" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siQuestType, sizeof(pRecvMsg->siQuestType));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siQuestState, sizeof(pRecvMsg->siQuestState));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siQuestType );
		m_odbc.GetData( &sendMsg.siQuest_StartCount );
		m_odbc.GetData( &sendMsg.siQuest_CompleteCount );
	END_FETCHDATA()
/*
	//if(siResult < 0)
	//	sendMsg.siResult = 0;
	//else
	//	sendMsg.siResult = siResult;
*/
	// send

	//KHY - 0822 - 해피빈 퀘스트 추가. 
	if(pRecvMsg->siQuestState == QUEST_HAPPYBEAN_STEP_START) // NON이나 COMPLETE일때는 보내서는 안됩니다.
		SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	//if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "QuestType = %d, QuestState = %d, Result = %d, StartCount = %d, CompleteCount = %d"
		,	pRecvMsg->siQuestType, pRecvMsg->siQuestState, siResult, sendMsg.siQuest_StartCount, sendMsg.siQuest_CompleteCount
		);
		sDBLogMsg_QueryString LogMsg("Quest HappyBean Clear", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트를 진행한다.
void DBGameProtocol::DailyQuest2Add()
{
	// 받은 패킷
	sDBRequest_DailyQuest2Add *pRecvMsg = (sDBRequest_DailyQuest2Add *)m_pPacket;

	// 보낼 패킷
	sDBResponse_DailyQuest2Add sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUEST2ADD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_DailyQuest2Add" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonsterKind, sizeof(pRecvMsg->siMonsterKind) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMonsterNum, sizeof(pRecvMsg->siMonsterNum) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRewardItemUnique, sizeof(pRecvMsg->siRewardItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRewardItemNum, sizeof(pRecvMsg->siRewardItemNum) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRewardMoney, sizeof(pRecvMsg->siRewardMoney ));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRewardExp, sizeof(pRecvMsg->siRewardExp) );

		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siQuestTime, sizeof(pRecvMsg->siQuestTime) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLeftTime, sizeof(pRecvMsg->siLeftTime) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siVillagePayFeeMoney, sizeof(pRecvMsg->siVillagePayFeeMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bMoneySelected, sizeof(pRecvMsg->bMoneySelected));
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bHiddenQuest, sizeof(pRecvMsg->bHiddenQuest));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );

		m_odbc.GetData( &sendMsg.siMonsterKind, 0 );
		m_odbc.GetData( &sendMsg.siMonsterNum, 0 );

		m_odbc.GetData( &sendMsg.siRewardItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siRewardItemNum, 0 );
		m_odbc.GetData( &sendMsg.siRewardMoney, 0 );
		m_odbc.GetData( &sendMsg.siRewardExp, 0 );

		m_odbc.GetData( &sendMsg.siFee, 0 );
		m_odbc.GetData( &sendMsg.siQuestTime, 0 );
		m_odbc.GetData( &sendMsg.siLeftTime, 0 );

		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.clStrMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.bHiddenQuest, 0 );
		m_odbc.GetData( &sendMsg.bMoneySelected, 0 );
	END_FETCHDATA()

	sendMsg.siQuestListIndex	= pRecvMsg->siQuestListIndex;
	sendMsg.siStrType			= pRecvMsg->siStrType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Fee = %I64d, LeftPersonMoney = %I64d, QuestTime = %d, LeftTime = %d, MonsterKind = %d, MonsterNum = %d, RewardItemUnique = %d, RewardItemNum = %d, RewardExp = %d, RewardMoney = %I64d, SelectedMoney = %d, Hidden = %d"
			,	sendMsg.siFee, sendMsg.siLeftMoney, sendMsg.siQuestTime, sendMsg.siLeftTime, sendMsg.siMonsterKind, sendMsg.siMonsterNum
			,	sendMsg.siRewardItemUnique, sendMsg.siRewardItemNum, sendMsg.siRewardExp
			,	sendMsg.siRewardMoney, sendMsg.bMoneySelected, sendMsg.bHiddenQuest
			);
		sDBLogMsg_QueryString LogMsg("DailyQuest2 Add", sendMsg.siPersonID, NULL, sendMsg.siRewardItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트를 종료한다.
void DBGameProtocol::DailyQuest2Del()
{
	// 받은 패킷
	sDBRequest_DailyQuest2Del *pRecvMsg = (sDBRequest_DailyQuest2Del *)m_pPacket;

	// 보낼 패킷
	sDBResponse_DailyQuest2Del sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUEST2DEL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.clRewardItem.Set( &pRecvMsg->clRewardItem);

	// DB Query
	BEGIN_SP( "rsp_DailyQuest2Del" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiSuccess, sizeof(pRecvMsg->uiSuccess) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siInventoryIndex, sizeof(pRecvMsg->siInventoryIndex) );
		SetItemParam( &pRecvMsg->clRewardItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiPileSwitch, sizeof(pRecvMsg->uiPileSwitch) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRewardMoney, sizeof(pRecvMsg->siRewardMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bMoneySelected, sizeof(pRecvMsg->bMoneySelected));
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bHiddenQuest, sizeof(pRecvMsg->bHiddenQuest) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.uiSuccess, 0 );
		m_odbc.GetData( &sendMsg.siAddMoney );
		m_odbc.GetData( &sendMsg.siExp, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siInventoryItemIndex, 0 );
			GetItemData( &sendMsg.clInventoryItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siChangedItemNum = pRecvMsg->clRewardItem.siItemNum;
	sendMsg.bHiddenQuest = pRecvMsg->bHiddenQuest;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성 - 실패도 로그에 남겨본다 - LEEKH - 2007.04.02
	// Result = -2 : 서버가 요청한 위치에 아이템이 존재 (같은 유니크면 합치는건 가능)
	// Result = -1 : 오류 (경험치, 퀘스트정보 삭제)
	// Result =  0 : 프로시져 에러
	// Result =  1 : 성공
	//if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, Success = %d, CurrentExp = %d, RewardItemNum = %d, LeftInvItemNum = %d, AddMoney = %I64d, Hidden = %d"
			, sendMsg.siResult, sendMsg.uiSuccess, sendMsg.siExp, pRecvMsg->clRewardItem.siItemNum, sendMsg.clInventoryItem.siItemNum, sendMsg.siAddMoney, sendMsg.bHiddenQuest
			);
		sDBLogMsg_QueryString LogMsg("DailyQuest2 End", sendMsg.siPersonID, NULL, sendMsg.clInventoryItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트진행중 변경사항을 저장한다.
void DBGameProtocol::DailyQuest2Change()
{
	// 받은 패킷
	sDBRequest_DailyQuest2Change *pRecvMsg = (sDBRequest_DailyQuest2Change *)m_pPacket;

	// 보낼 패킷
	sDBResponse_DailyQuest2Change sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUEST2CHANGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siMonsterKind = pRecvMsg->siMonsterKind;

	// DB Query
	BEGIN_SP( "rsp_DailyQuest2Change" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siKillMonsterNum, sizeof(pRecvMsg->siKillMonsterNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLeftTime, sizeof(pRecvMsg->siLeftTime) );
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bHiddenQuest, sizeof(pRecvMsg->bHiddenQuest) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siKillMonsterNum, 0 );
		m_odbc.GetData( &sendMsg.siLeftTime, 0 );
		m_odbc.GetData( &sendMsg.bHiddenQuest, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "LeftTime = %d, KillNumber = %d"
			,	sendMsg.siLeftTime, sendMsg.siKillMonsterNum, sendMsg.bHiddenQuest
			);
		sDBLogMsg_QueryString LogMsg("DailyQuest2 Change", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트를 진행한다.
void DBGameProtocol::DailyQuest3Add()
{
	// 받은 패킷
	sDBRequest_DailyQuest3Add *pRecvMsg = (sDBRequest_DailyQuest3Add *)m_pPacket;

	// 보낼 패킷
	sDBResponse_DailyQuest3Add sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUEST3ADD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_DailyQuest3Add" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRewardItemUnique, sizeof(pRecvMsg->siRewardItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRewardItemNum, sizeof(pRecvMsg->siRewardItemNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siRewardExp, sizeof(pRecvMsg->siRewardExp) );

		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siQuestTime, sizeof(pRecvMsg->siQuestTime) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLeftTime, sizeof(pRecvMsg->siLeftTime) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siVillagePayFeeMoney, sizeof(pRecvMsg->siVillagePayFeeMoney) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bMoneySelected, sizeof(pRecvMsg->bMoneySelected));
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bHiddenQuest, sizeof(pRecvMsg->bHiddenQuest));
	END_SP()

		EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );

		m_odbc.GetData( &sendMsg.siItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemNum, 0 );

		m_odbc.GetData( &sendMsg.siRewardItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siRewardItemNum, 0 );
		m_odbc.GetData( &sendMsg.siRewardExp, 0 );

		m_odbc.GetData( &sendMsg.siFee, 0 );
		m_odbc.GetData( &sendMsg.siQuestTime, 0 );
		m_odbc.GetData( &sendMsg.siLeftTime, 0 );

		m_odbc.GetData( &sendMsg.siLeftMoney, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.clStrMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.bHiddenQuest, 0 );
		m_odbc.GetData( &sendMsg.bMoneySelected, 0 );
	END_FETCHDATA()

	sendMsg.siQuestListIndex	= pRecvMsg->siQuestListIndex;
	sendMsg.siStrType			= pRecvMsg->siStrType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Fee = %I64d, LeftPersonMoney = %I64d, QuestTime = %d, LeftTime = %d, MonsterKind = %d, MonsterNum = %d, RewardItemUnique = %d, RewardItemNum = %d, RewardExp = %d, SelectedMoney = %d, Hidden = %d"
			,	sendMsg.siFee, sendMsg.siLeftMoney, sendMsg.siQuestTime, sendMsg.siLeftTime, sendMsg.siItemUnique, sendMsg.siItemNum
			,	sendMsg.siRewardItemUnique, sendMsg.siRewardItemNum, sendMsg.siRewardExp
			,	sendMsg.bMoneySelected, sendMsg.bHiddenQuest
			);
		sDBLogMsg_QueryString LogMsg("DailyQuest3 Add", sendMsg.siPersonID, NULL, sendMsg.siRewardItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트를 종료한다.
void DBGameProtocol::DailyQuest3Del()
{
	// 받은 패킷
	sDBRequest_DailyQuest3Del *pRecvMsg = (sDBRequest_DailyQuest3Del *)m_pPacket;

	// 보낼 패킷
	sDBResponse_DailyQuest3Del sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUEST3DEL;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.clRewardItem.Set(&pRecvMsg->clRewardItem);

	// DB Query
	BEGIN_SP( "rsp_DailyQuest3Del" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiSuccess, sizeof(pRecvMsg->uiSuccess) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siInventoryIndex, sizeof(pRecvMsg->siInventoryIndex) );
		SetItemParam( &pRecvMsg->clRewardItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->uiPileSwitch, sizeof(pRecvMsg->uiPileSwitch) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bMoneySelected, sizeof(pRecvMsg->bMoneySelected));
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bHiddenQuest, sizeof(pRecvMsg->bHiddenQuest) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.uiSuccess, 0 );
		m_odbc.GetData( &sendMsg.siExp, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siInventoryItemIndex, 0 );
			GetItemData( &sendMsg.clInventoryItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siChangedItemNum = pRecvMsg->clRewardItem.siItemNum;
	sendMsg.bHiddenQuest = pRecvMsg->bHiddenQuest;
	sendMsg.siItemUnique = pRecvMsg->siItemUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성 - 실패도 로그에 남겨본다 - LEEKH - 2007.04.02
	// Result = -2 : 서버가 요청한 위치에 아이템이 존재 (같은 유니크면 합치는건 가능)
	// Result = -1 : 오류 (경험치, 퀘스트정보 삭제)
	// Result =  0 : 프로시져 에러
	// Result =  1 : 성공
	//if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, Success = %d, CurrentExp = %d, RewardItemNum = %d, LeftInvItemNum = %d, AddMoney = %I64d, Hidden = %d"
			, sendMsg.siResult, sendMsg.uiSuccess, sendMsg.siExp, pRecvMsg->clRewardItem.siItemNum, sendMsg.clInventoryItem.siItemNum, sendMsg.siAddMoney, sendMsg.bHiddenQuest
			);
		sDBLogMsg_QueryString LogMsg("DailyQuest3 End", sendMsg.siPersonID, NULL, sendMsg.clInventoryItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// 모병관퀘스트진행중 변경사항을 저장한다.
void DBGameProtocol::DailyQuest3Change()
{
	// 받은 패킷
	sDBRequest_DailyQuest3Change *pRecvMsg = (sDBRequest_DailyQuest3Change *)m_pPacket;

	// 보낼 패킷
	sDBResponse_DailyQuest3Change sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DAILYQUEST3CHANGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_DailyQuest3Change" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMakedItem, sizeof(pRecvMsg->siMakedItem) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siLeftTime, sizeof(pRecvMsg->siLeftTime) );
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bHiddenQuest, sizeof(pRecvMsg->bHiddenQuest) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMakedItem, 0 );
		m_odbc.GetData( &sendMsg.siLeftTime, 0 );
		m_odbc.GetData( &sendMsg.bHiddenQuest, 0 );
	END_FETCHDATA()

		// send
		SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "LeftTime = %d, MakeNum = %d"
			,	sendMsg.siLeftTime, sendMsg.siMakedItem
			);
		sDBLogMsg_QueryString LogMsg("DailyQuest3 Change", sendMsg.siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetMissionQuestInfo()
{
	// 받은 패킷
	sDBRequest_SetMissionQuestInfo *pRecvMsg = (sDBRequest_SetMissionQuestInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetMissionQuestInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETMISSIONQUESTINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siPersonID = pRecvMsg->siPersonID;
	sendMsg.clMissionQuestInfo.Set(&pRecvMsg->clMissionQuestInfo);	

	SI32 siResult = 0;

	SI32 siLeftTimeSec = pRecvMsg->clMissionQuestInfo.GetLeftTime();
	// DB Query
	BEGIN_SP( "rsp_QuestMissionInfoSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));

		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->clMissionQuestInfo.siUnique, sizeof(pRecvMsg->clMissionQuestInfo.siUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMissionQuestInfo.clQuestClearConditionInfo.siFlag, sizeof(pRecvMsg->clMissionQuestInfo.clQuestClearConditionInfo.siFlag));
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->clMissionQuestInfo.clQuestClearConditionInfo.siPara1, sizeof(pRecvMsg->clMissionQuestInfo.clQuestClearConditionInfo.siPara1));
		m_odbc.SetParam( SQL_INTEGER,  &pRecvMsg->clMissionQuestInfo.clQuestClearConditionInfo.siPara2, sizeof(pRecvMsg->clMissionQuestInfo.clQuestClearConditionInfo.siPara2));
		m_odbc.SetParam( SQL_BIGINT,   &pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siMoney, sizeof(pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siMoney));
		m_odbc.SetParam( SQL_INTEGER,   &pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siExp, sizeof(pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siExp));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siFameExp, sizeof(pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siFameExp));
		m_odbc.SetParam( SQL_INTEGER,  &siLeftTimeSec, sizeof(siLeftTimeSec));
		for(int i = 0; i < MAX_QUEST_REWARD_NUMBER; i++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siItemUnique[i], sizeof(pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siItemUnique[i]));
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siItemNum[i], sizeof(pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siItemNum[i]));
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siItemUseDate[i], sizeof(pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.siItemUseDate[i]));
		}
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.bDeBuff, sizeof(pRecvMsg->clMissionQuestInfo.clQuestRewardInfo.bDeBuff));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult );
	END_FETCHDATA()

	sendMsg.siResult = siResult;

	// send
	SendPacket( (BYTE *)&sendMsg );
}
