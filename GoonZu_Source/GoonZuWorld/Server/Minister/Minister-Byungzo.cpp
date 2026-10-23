
#include "Char\CharManager\CharManager.h"

#include "..\..\CommonLogic\CommonLogic.h"
#include "..\Server.h"

#include "MinisterBase.h"
#include "Minister-Byungzo.h"

#include "..\..\CommonLogic\MsgRval-Define.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\CommonLogic\MsgType-System.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

CMinisterByungzo::CMinisterByungzo() : CMinisterBase()
{

	m_siVillageWarTime = 40;

	m_siPersonID = 0;
	m_siSetTime = 0;
	m_siWarStartTime = 0;
	m_siSetWarCount = 0;
	m_siBlackArmyNum = 600;

}



CMinisterByungzo::~CMinisterByungzo()
{

}


// DB에 병조의 정보를 요청한다.
void CMinisterByungzo::InitializeDB()
{

	// 병조의 정보를 요청한다.
	sDBRequest_GetByungzoInfo clMsg;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

// DB에서 병조의 정보를 받으면 값을 Class에 설정한다.
void  CMinisterByungzo::DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket)
{

	sDBResponse_GetByungzoInfo* pclMsg = (sDBResponse_GetByungzoInfo *)pPacket;

	if(pclMsg->siResult)
	{
		StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szAccountID);
		StringCchCopy(m_szPersonName, MAX_PLAYER_NAME, 	pclMsg->MinisterBase.m_szPersonName);
		StringCchCopy(m_szGreeting, 256, pclMsg->MinisterBase.m_szGreeting);

		m_siVillageWarTime =	pclMsg->siVillageWarTime;

		// 공성시간은  40분보다 작을수 없고 60분 보다 클수 없다.
		if(m_siVillageWarTime<40 || m_siVillageWarTime>60)	m_siVillageWarTime = 40;

		((cltServer *)pclClient)->dwVillageWarTimeMin	= m_siVillageWarTime;
		((cltServer *)pclClient)->dwVillageWarFinishMin	= ((cltServer *)pclClient)->dwVillageWarStartMin + ((cltServer *)pclClient)->dwVillageWarTimeMin;

		m_siPersonID = pclMsg->siPersonID;
		m_siSetTime = pclMsg->siSetTime;
		m_siWarStartTime = pclMsg->siWarStartTime;
		m_siSetWarCount = pclMsg->siSetWarCount;
		m_siBlackArmyNum = pclMsg->siBlackArmyNum;
        		
        if ( m_siBlackArmyNum< 100 || m_siBlackArmyNum > 600 )
			m_siBlackArmyNum = 600 ;

		// 흑의군단 전쟁시각을 체크하여 유효한 시각이 아니면, 삭제한다.
		SYSTEMTIME stServerTime;
		memcpy(&stServerTime, &((cltServer *)pclClient)->sTime, sizeof(stServerTime));
		SYSTEMTIME stWarStartTime = pclClient->GetTimeFromsMinute(m_siWarStartTime);

		// 전쟁시작시간이 서버 시간보다 이전이면,
		if( TimeCompare(&stWarStartTime, &stServerTime) < 0 )
		{
			// 전쟁시작시간 초기화
			m_siWarStartTime = 0;

			// DB에 신호를 보냄
			SI32 siID = 0;
			sDBRequest_SetByungzoInfo clMsg(siID, m_siPersonID,
											m_szGreeting, m_siVillageWarTime, m_siSetTime, m_siWarStartTime, 
											m_siSetWarCount, m_siBlackArmyNum);
			((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetByungzoInfo"));
}

// Client에서 병조의 정보를 요청하면 지금 Class에 있는 정보를 전송한다.
void CMinisterByungzo::DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID)
{
	CMinisterBase *pMinister = (CMinisterBase* )(this);
	cltGameMsgResponse_GetByungzoInfo clinfo(pMinister, m_siVillageWarTime, 
			m_siSetTime, m_siWarStartTime, m_siSetWarCount, m_siBlackArmyNum);

	cltMsg clMsg(GAMEMSG_RESPONSE_GETBYUNGZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
	((cltServer *)pclClient)->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);
}

// Client에서 병조의 정보 새로 설정하기를 원하면 서버에서는 DB에 정보 설정을 요청한다.
void CMinisterByungzo::DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID)
{
	cltGameMsgRequest_SetByungzoInfo *pclinfo = (cltGameMsgRequest_SetByungzoInfo*)pclMsg->cData;

	cltServer *pclserver = (cltServer*)pclClient;

	if (m_siSetWarCount >= MAX_SETWARCOUNT)
		return;

	SI32 settime = pclClient->GetMinuteFromsTime( pclserver->sTime );
	
	sDBRequest_SetByungzoInfo clMsg(siID, ((cltServer *)pclClient)->pclCM->CR[siID]->pclCI->GetPersonID(),
		pclinfo->szGreeting, pclinfo->siVillageWarTime , settime, pclinfo->siWarStartTime, 
		pclinfo->siSetWarCount, pclinfo->siBlackArmyNum);

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
}

// DB에서 정보의 쓰기가 완료되면 Client에게 다시 전송하여준다.
void CMinisterByungzo::DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket)
{
	sDBResponse_SetByungzoInfo* pclMsg = (sDBResponse_SetByungzoInfo*)pPacket;

	SI32 id = pclMsg->usCharID;

	if(pclMsg->siResult == 1)
	{

		//청룡관이 설정한 전쟁이 끝나면 warstarttime이 초기화되고 setwarcount는 하나 증가한다
		//그러나 서버에서 직접 설정을 바꾸는것이기 때문에 siID값은 0으로 들어옴
		//그리하여 아래의 ID로 검색하는부분은 주석처리

		//// 동일인이라면 
		//if(((cltServer *)pclClient)->pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
		//{
			StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, 	pclMsg->MinisterBase.m_szAccountID);
			StringCchCopy(m_szPersonName, MAX_PLAYER_NAME,	pclMsg->MinisterBase.m_szPersonName);
			StringCchCopy(m_szGreeting, 256,	pclMsg->MinisterBase.m_szGreeting);
			
			// 공성시간은 20분보다 작을수 없고 60분 보다 클수 없다.
			if(m_siVillageWarTime<40 || m_siVillageWarTime>60)	m_siVillageWarTime = 40;

			m_siVillageWarTime		= pclMsg->siVillageWarTime;
			((cltServer *)pclClient)->dwVillageWarTimeMin	= m_siVillageWarTime;
			((cltServer *)pclClient)->dwVillageWarFinishMin	= ((cltServer *)pclClient)->dwVillageWarStartMin + ((cltServer *)pclClient)->dwVillageWarTimeMin;

			m_siPersonID = pclMsg->siPersonID;
			m_siSetTime = pclMsg->siSetTime;
			m_siWarStartTime = pclMsg->siWarStartTime;
			m_siSetWarCount = pclMsg->siSetWarCount;
			m_siBlackArmyNum = pclMsg->siBlackArmyNum;
			m_siPersonID = pclMsg->siPersonID;
		//}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 4] // 병조설정.
			// param1		 == 전쟁설정 횟수, 
			// param2		 == 흑의 군단 수, 
			// param3		 == 전쟁시간, 
			// pszCharParam1 == 전쟁설정시간
			// pszCharParam2 == 전쟁시작시간
			cltServer * pclserver = (cltServer*)pclClient;
			cltCharServer* pclchar = NULL;
			if(pclClient->pclCM->IsValidID(id))			pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			_SYSTEMTIME SetTime;
			_SYSTEMTIME WarStartTime;
			SetTime		 = pclClient->GetTimeFromsMinute(pclMsg->siSetTime);
			WarStartTime = pclClient->GetTimeFromsMinute(pclMsg->siWarStartTime);

			TCHAR	pszCharParam1[50] = {'\0', };
			TCHAR	pszCharParam2[50] = {'\0', };
			StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), "%d-%d-%d", 
				SetTime.wYear,
				SetTime.wMonth,
				SetTime.wDay	);
			StringCchPrintf(pszCharParam1, sizeof(pszCharParam1), "%d-%d-%d", 
				WarStartTime.wYear,
				WarStartTime.wMonth,
				WarStartTime.wDay	);

			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_BYUNGZO_SET, 
				0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				pclMsg->siSetWarCount, pclMsg->siBlackArmyNum, pclMsg->siVillageWarTime, 0, 0,
				pszCharParam1, pszCharParam2);
		}
	}
	else
	{
		if(((cltServer *)pclClient)->pclCM->IsAlive(id) == false)return ;

		// 서버 Error
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETBYUNGZOINFO,  0, 0, id);
	}

	if (pclClient->pclCM->IsValidID(id))
	{
		// 정보를 요청했을때와 같이 돌려준다.
		DoMsg_GameMsgRequest_GetMinisterInfo(id);
	}

}
