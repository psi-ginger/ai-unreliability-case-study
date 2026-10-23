
#include "..\..\Common\CommonHeader.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "..\..\CommonLogic\CommonLogic.h"
#include "..\Server.h"

#include "MinisterBase.h"
#include "Minister-Hyungzo.h"

#include "..\..\CommonLogic\MsgRval-Define.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\CommonLogic\MsgType-System.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CMinisterHyungzo::CMinisterHyungzo() : CMinisterBase()
{
	m_siLimitStealNum = 5 ;
	m_siSuspectTime = 60 ;
	m_siPrisonTime = 5 ;
	m_siPrisonerPenalty = 20 ;

	for ( int i = 0 ; i < MAX_PRISON_LIST_NUM ; i ++ )
	{
		m_stHyungzoMember[i].siDate = 0 ;
		m_stHyungzoMember[i].siPrisonSecond = 0 ;
		StringCchCopy(m_stHyungzoMember[i].szCharName, MAX_PLAYER_NAME, TEXT(""));
		StringCchCopy(m_stHyungzoMember[i].szPrisonReason, MAX_PLAYER_NAME, TEXT(""));
	}

#ifdef _SAFE_MEMORY
	m_stHyungzoPoliceList.ZeroMem();
	m_stHyungzoCriminalList.ZeroMem();
#else
	memset(m_stHyungzoPoliceList,0,sizeof(HyungzoPoliceList) * MAX_HYUNGZO_POLICELIST);
	memset(m_stHyungzoCriminalList,0,sizeof(HyungzoCriminalList) * MAX_HYUNGZO_CRIMINALLIST);
#endif
}

CMinisterHyungzo::~CMinisterHyungzo()
{
}

// DB에 형조의 정보를 요청한다.
void CMinisterHyungzo::InitializeDB()
{
	// 형조의 정보를 요청한다.
	sDBRequest_GetHyungzoInfo clMsg;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	// 형조의 정보중 포교 리스트를 요청한다.
	sDBRequest_GetPoliceList clMsg2;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg2);

	// 형조의 정보중 현행범 리스트를 요청한다.
	sDBRequest_GetCriminalList clMsg3;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg3);
}

// DB에서 형조의 정보를 받으면 값을 Class에 설정한다.
void  CMinisterHyungzo::DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket)
{

	sDBResponse_GetHyungzoInfo* pclMsg = (sDBResponse_GetHyungzoInfo *)pPacket;

	if(pclMsg->siResult)
	{
		StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szAccountID);
		StringCchCopy(m_szPersonName, MAX_PLAYER_NAME, 	pclMsg->MinisterBase.m_szPersonName);
		StringCchCopy(m_szGreeting,	256, pclMsg->MinisterBase.m_szGreeting);

		m_siLimitStealNum =	pclMsg->siLimitStealNum;
		// 제한 숫자는 3회 이상에 10회 이하다
		if(m_siLimitStealNum<3 || m_siLimitStealNum>10)	m_siLimitStealNum = 5;

		m_siSuspectTime =	pclMsg->siSuspectTime;
		// 판별 시간은 20초 이상 300초 이하다
		if(m_siSuspectTime<20 || m_siSuspectTime>300)	m_siSuspectTime = 60;

		m_siPrisonTime =	pclMsg->siPrisonTime;
		// 수감 시간은 5분 이상 60분 이하다
		if(m_siPrisonTime<5 || m_siPrisonTime>60)	m_siPrisonTime = 5;

		m_siPrisonerPenalty = pclMsg->siPrisonerPenalty;
		// 자동사냥 프로그램 수감자 형벌은 20배에서 100배이다. 20 , 40 , 60 , 80 , 100
		if ( m_siPrisonerPenalty < 20 || m_siPrisonerPenalty > 100 ) m_siPrisonerPenalty = 20 ;
		if ( m_siPrisonerPenalty % 20 ) m_siPrisonerPenalty = 20 ;

		for ( int i = 0 ; i < MAX_PRISON_LIST_NUM ; i ++ )
		{
			if ( pclMsg->stHyungzoMember[i].siDate <= 0 ) continue ;
			m_stHyungzoMember[i].siDate = pclMsg->stHyungzoMember[i].siDate ;
			m_stHyungzoMember[i].siPrisonSecond = pclMsg->stHyungzoMember[i].siPrisonSecond ;
			memcpy(m_stHyungzoMember[i].szCharName,pclMsg->stHyungzoMember[i].szCharName,MAX_PLAYER_NAME) ;
			memcpy(m_stHyungzoMember[i].szPrisonReason,pclMsg->stHyungzoMember[i].szPrisonReason,MAX_PRISON_REASON_SIZE);
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetHyungzoInfo"));
}

// Client에서 형조의 정보를 요청하면 지금 Class에 있는 정보를 전송한다.
void CMinisterHyungzo::DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID)
{
	CMinisterBase *pMinister = (CMinisterBase* )(this);

	cltGameMsgResponse_GetHyungzoInfo clinfo(pMinister, m_siLimitStealNum,m_siSuspectTime,
											 m_siPrisonTime,m_siPrisonerPenalty, &m_stHyungzoMember[0]);

	cltMsg clMsg(GAMEMSG_RESPONSE_GETHYUNGZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
	((cltServer *)pclClient)->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);
}

// Client에서 형조의 정보 새로 설정하기를 원하면 서버에서는 DB에 정보 설정을 요청한다.
void CMinisterHyungzo::DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID)
{
	cltGameMsgRequest_SetHyungzoInfo *pclinfo = (cltGameMsgRequest_SetHyungzoInfo*)pclMsg->cData;
	
	sDBRequest_SetHyungzoInfo clMsg(siID, ((cltServer *)pclClient)->pclCM->CR[siID]->pclCI->GetPersonID(),
		pclinfo->szGreeting, pclinfo->siLimitStealNum,
		pclinfo->siSuspectTime,pclinfo->siPrisonTime,pclinfo->siPrisonerPenalty);

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

// DB에서 정보의 쓰기가 완료되면 Client에게 다시 전송하여준다.
void CMinisterHyungzo::DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket)
{
	sDBResponse_SetHyungzoInfo* pclMsg = (sDBResponse_SetHyungzoInfo*)pPacket;
	SI32 id = pclMsg->usCharID;

	if(((cltServer *)pclClient)->pclCM->IsAlive(id) == false)return ;

	if(pclMsg->siResult == 1)
	{
		// 동일인이라면 
		if(((cltServer *)pclClient)->pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID ||
			pclClient->IsWhereServiceArea( ConstServiceArea_China ) )
		{  
			StringCchCopy(m_szAccountID, MAX_PLAYER_NAME,	pclMsg->MinisterBase.m_szAccountID);
			StringCchCopy(m_szPersonName,	MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szPersonName);
			StringCchCopy(m_szGreeting,	256, pclMsg->MinisterBase.m_szGreeting);
			
			m_siLimitStealNum		= pclMsg->siLimitStealNum;
			if(m_siLimitStealNum<3 || m_siLimitStealNum>10)	m_siLimitStealNum = 5;


			m_siSuspectTime =	pclMsg->siSuspectTime;
			if(m_siSuspectTime<20 || m_siSuspectTime>300)	m_siSuspectTime = 60;

			m_siPrisonTime = pclMsg->siPrisonTime;
			if(m_siPrisonTime<5 || m_siPrisonTime>60) m_siPrisonTime = 5;

			m_siPrisonerPenalty = pclMsg->siPrisonerPenalty;
			if ( m_siPrisonerPenalty < 20 || m_siPrisonerPenalty > 100 ) m_siPrisonerPenalty = 20 ;
			if ( m_siPrisonerPenalty % 20 ) m_siPrisonerPenalty = 20 ;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 10] // 형조설정.	
				// param1 == 스틸 제한 횟수, param2 == 용의자 판별시간, param3 == 용의자 수감시간,
				// param4 == 매크로 사용자 경험치 처벌 강도, pszParam1 == 계정, pszParam2 == 이름.
				cltServer * pclserver = (cltServer*)pclClient;

				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_HYUNGZO_SET, 
					0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					pclMsg->siLimitStealNum, pclMsg->siSuspectTime,	pclMsg->siPrisonTime, pclMsg->siPrisonerPenalty, 0,
					pclMsg->MinisterBase.m_szAccountID, pclMsg->MinisterBase.m_szPersonName);
			}
		}
    }
	else
	{
		// 서버 Error
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETHYUNGZOINFO,  0, 0, id);
	}
	
	// 정보를 요청했을때와 같이 돌려준다.
	DoMsg_GameMsgRequest_GetMinisterInfo(id);

}

void CMinisterHyungzo::DoMsg_GameMsgRequest_SetHyungPrison(cltMsg*pclMsg,SI32 siID)
{
	cltGameMsgRequest_SetHyungzoPrison *pclinfo = (cltGameMsgRequest_SetHyungzoPrison*)pclMsg->cData;

	TCHAR personname[MAX_PLAYER_NAME] = TEXT("") ;
	SI32 time = 0 ;
	TCHAR reason[MAX_PRISON_REASON_SIZE] =  TEXT("") ;

	StringCchCopy( personname , MAX_PLAYER_NAME, pclinfo->stHyungzoMember.szCharName);
	TCHAR* pText1 = GetTxtFromMgr(244);
	TCHAR* pText2 = GetTxtFromMgr(599);
	if ( _tcslen(personname) <= 0 || _tcslen(personname) > MAX_PLAYER_NAME ||
		 !_tcscmp(personname,pText1) || !_tcscmp(personname,pText2) ) // 운영자 캐릭터 예외 처리
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_PRISON_FAIL_NAMEERROR,  0, 0,  siID);
		return ;
	}

	cltCharServer * pclchar = (cltCharServer*)((cltServer*)pclClient)->pclCM->CR[siID] ;

	if ( pclchar == NULL ) return ;

	if ( pclchar->pclCI->clBI.uiGRank != GRANK_MINISTER5 || 
		 pclchar->pclCI->clCharItem.clItem[PERSONITEM_DRESS].siUnique != 23510 )
	{
		return ;
	}

	//[2007.12.31 - 김영훈 추가 : 수감할려는 케릭터가 운영자이면 리턴]

	SI32 siTargetID = pclClient->pclCM->GetIDFromName(pclinfo->stHyungzoMember.szCharName);
	//수감자 이름을 기준으로 ID를 검색 후 0일때(현재 접속중이 아님)는 리턴
	if ( siTargetID == 0 )
	{
		return;
	}

	cltCharServer *pclTarget = (cltCharServer*)((cltServer*)pclClient)->pclCM->CR[siTargetID];
	//수감하려는 케릭터가 운영자일때는 리턴
	if ( pclTarget->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL )
	{
		return;
	}

	//pclchar->DecreaseEquipDur(PERSONITEM_DRESS, 5);

	time = pclinfo->stHyungzoMember.siPrisonSecond ;
	if ( time <= 0 || time > MAX_PRISON_SECOND )
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_PRISON_FAIL_TIMEERROR,  0, 0,  siID);
		return ;
	}
	time *= 60 ;

	StringCchCopy(reason, MAX_PRISON_REASON_SIZE,pclinfo->stHyungzoMember.szPrisonReason);
	if ( _tcslen(reason) <= 0 || _tcslen(reason) > MAX_PRISON_REASON_SIZE )
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_PRISON_FAIL_REASONERROR,  0, 0,  siID);
		return ;
	}

	cltGuiltyInfo pclGuiltyInfo ;
	pclGuiltyInfo.Init();
	pclGuiltyInfo.SetTotalPrisonSecond(time);
	pclGuiltyInfo.SetLeftPrisonSecond(time);
	pclGuiltyInfo.SetPrisonReason(reason);


#if 0 //KHY - 0606
	SI32 curdate = ((cltServer*)pclClient)->pclTime->clDate.GetHourVary();
#else
	SI32 curdate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary();
#endif
	sDBRequest_PrisonUser clMsg(siID,curdate, personname,pclchar->pclCI->GetPersonID(),&pclGuiltyInfo);
	((cltServer*)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);


	return ;
}

void CMinisterHyungzo::DoMsg_DBMsgResponse_GetPoliceList(sPacketHeader* pPacket)
{
	sDBResponse_GetPoliceList* pclMsg = (sDBResponse_GetPoliceList*)pPacket;

	if(pclMsg->siResult)
	{
		for ( int i = 0 ; i < MAX_HYUNGZO_POLICELIST ; i ++ )
		{
			memcpy(m_stHyungzoPoliceList[i].szAccountID,pclMsg->stHyungzoPoliceList[i].szAccountID,32);
			memcpy(m_stHyungzoPoliceList[i].szName,pclMsg->stHyungzoPoliceList[i].szName,MAX_PLAYER_NAME);

			m_stHyungzoPoliceList[i].siPersonID = pclMsg->stHyungzoPoliceList[i].siPersonID ;
			m_stHyungzoPoliceList[i].siLevel = pclMsg->stHyungzoPoliceList[i].siLevel ;
		}
	}
}

void CMinisterHyungzo::DoMsg_DBMsgResponse_SetPolice(sPacketHeader* pPacket)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

    sDBResponse_SetPolice* pclMsg = (sDBResponse_SetPolice *)pPacket;

	if ( pclServer->pclCM->IsValidID(pclMsg->usCharID) == false ) return ;

	SI32 index = 0 ;

	if(pclMsg->siResult == 1)
	{
		if ( pclMsg->bAdmin ) // 임명
		{
			for ( int i = 0 ; i < MAX_HYUNGZO_POLICELIST ; i ++ )
			{
				if(m_stHyungzoPoliceList[i].siPersonID == 0)
				{
					m_stHyungzoPoliceList[i].siPersonID = pclMsg->stHyungzoPolice.siPersonID ;
					m_stHyungzoPoliceList[i].siLevel = pclMsg->stHyungzoPolice.siLevel ;
					StringCchCopy(m_stHyungzoPoliceList[i].szAccountID, 32,pclMsg->stHyungzoPolice.szAccountID);
					StringCchCopy(m_stHyungzoPoliceList[i].szName, MAX_PLAYER_NAME,pclMsg->stHyungzoPolice.szName);
					index = i;
					break;
				}
			}

			if ( m_stHyungzoPoliceList[index].siPersonID )
			{
				cltLetterMsg_HyungzoPolice clletter(1, &pclServer->pclTime->clDate);
				pclServer->SendLetterMsg(m_stHyungzoPoliceList[index].siPersonID, (cltLetterHeader*)&clletter);

				SI32 toid = pclServer->pclCM->GetIDFromPersonID(m_stHyungzoPoliceList[index].siPersonID) ;
				if ( IsPC(toid) )
				{
					pclServer->pclCM->CR[toid]->SetPolice(true);
				}
			}

			pclServer->SendServerResponseMsg(0, SRVAL_HYUNGZO_SUCCESS_SETPOLICE, 0, 0, pclMsg->usCharID );
		}
		else // 해임
		{
			for ( int i = 0 ; i < MAX_HYUNGZO_POLICELIST ; i ++ )
			{
				if(m_stHyungzoPoliceList[i].siPersonID == pclMsg->stHyungzoPolice.siPersonID)
				{
					cltLetterMsg_HyungzoPolice clletter(2, &pclServer->pclTime->clDate);
					pclServer->SendLetterMsg(m_stHyungzoPoliceList[i].siPersonID, (cltLetterHeader*)&clletter);

					SI32 toid = pclServer->pclCM->GetIDFromPersonID(m_stHyungzoPoliceList[i].siPersonID) ;
					if ( IsPC(toid) )
					{
						pclServer->pclCM->CR[toid]->SetPolice(false);
					}

					m_stHyungzoPoliceList[i].siPersonID = 0 ;
					m_stHyungzoPoliceList[i].siLevel = 0;
					StringCchCopy(m_stHyungzoPoliceList[i].szAccountID, 32, TEXT(""));
					StringCchCopy(m_stHyungzoPoliceList[i].szName, MAX_PLAYER_NAME, TEXT(""));
					
					index = i;
					break;
				}
			}

			pclServer->SendServerResponseMsg(0, SRVAL_HOZO_SUCCESS_SETANGELADMIN_FALSE, 0, 0, pclMsg->usCharID );
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [로그추가 : 황진성 2007. 10. 10] // 보안관 임명.
			// param1 == 임명 = 1, 해임 = 0, 
			// param2 == 보안관 레벨
			// pszParam1 == 계정. 
			// pszParam2 == 이름.
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_POLICE_SET, 
				0, (cltCharServer*)pclClient->pclCM->CR[pclMsg->usCharID], NULL, pclMsg->stHyungzoPolice.siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				pclMsg->bAdmin, pclMsg->stHyungzoPolice.siLevel, 0, 0, 0, 
				pclMsg->stHyungzoPolice.szAccountID, pclMsg->stHyungzoPolice.szName);
		}
	}

	cltGameMsgResponse_SetPolice clinfo(pclMsg->siResult,m_stHyungzoPoliceList[index].szName,pclMsg->bAdmin);
	cltMsg clMsg(GAMEMSG_RESPONSE_SETPOLICE, sizeof(clinfo), (BYTE*)&clinfo);
	pclServer->pclCM->CR[pclMsg->usCharID]->SendNetMsg((sPacketHeader*)&clMsg);
}

void CMinisterHyungzo::DoMsg_DBMsgResponse_GetCriminalList(sPacketHeader* pPacket)
{
	sDBResponse_GetCriminalList* pclMsg = (sDBResponse_GetCriminalList*)pPacket;

	if(pclMsg->siResult)
	{
		for ( int i = 0 ; i < MAX_HYUNGZO_CRIMINALLIST ; i ++ )
		{
			memcpy(m_stHyungzoCriminalList[i].szAccountID,pclMsg->stHyungzoCriminalList[i].szAccountID,32);
			memcpy(m_stHyungzoCriminalList[i].szName, pclMsg->stHyungzoCriminalList[i].szName,MAX_PLAYER_NAME);
			memcpy(m_stHyungzoCriminalList[i].szReason, pclMsg->stHyungzoCriminalList[i].szReason, MAX_PRISON_REASON_SIZE);

			m_stHyungzoCriminalList[i].siPersonID = pclMsg->stHyungzoCriminalList[i].siPersonID ;
			m_stHyungzoCriminalList[i].siTime = pclMsg->stHyungzoCriminalList[i].siTime ;
		}
	}
}

void CMinisterHyungzo::DoMsg_DBMsgResponse_SetCriminal(sPacketHeader* pPacket)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

    sDBResponse_SetCriminal* pclMsg = (sDBResponse_SetCriminal *)pPacket;

	if ( pclServer->pclCM->IsValidID(pclMsg->usCharID) == false ) 
		return ;

	SI32 index = 0 ;

	if(pclMsg->siResult == 1)
	{
		if ( pclMsg->bAdmin ) // 임명
		{
			for ( int i = 0 ; i < MAX_HYUNGZO_CRIMINALLIST ; i ++ )
			{
				if(m_stHyungzoCriminalList[i].siPersonID == 0)
				{
					StringCchCopy(m_stHyungzoCriminalList[i].szAccountID, 32,pclMsg->stHyungzoCriminal.szAccountID);
					StringCchCopy(m_stHyungzoCriminalList[i].szName, MAX_PLAYER_NAME,pclMsg->stHyungzoCriminal.szName);
					StringCchCopy(m_stHyungzoCriminalList[i].szReason, MAX_PRISON_REASON_SIZE,pclMsg->stHyungzoCriminal.szReason);

					m_stHyungzoCriminalList[i].siPersonID = pclMsg->stHyungzoCriminal.siPersonID ;
					m_stHyungzoCriminalList[i].siTime	  = pclMsg->stHyungzoCriminal.siTime;
					
					index = i;
					break;
				}
			}
		}
		else // 해임
		{
			for ( int i = 0 ; i < MAX_HYUNGZO_CRIMINALLIST ; i ++ )
			{
				if(m_stHyungzoCriminalList[i].siPersonID == pclMsg->stHyungzoCriminal.siPersonID)
				{
					StringCchCopy(m_stHyungzoCriminalList[i].szAccountID, 32,TEXT(""));
					StringCchCopy(m_stHyungzoCriminalList[i].szName, MAX_PLAYER_NAME,TEXT(""));
					StringCchCopy(m_stHyungzoCriminalList[i].szReason, MAX_PRISON_REASON_SIZE, TEXT(""));

					m_stHyungzoCriminalList[i].siPersonID = 0;
					m_stHyungzoCriminalList[i].siTime	  = 0;
					
					index = i;
					break;
				}
			}
		}
	}

//	cltGameMsgResponse_SetPolice clinfo(pclMsg->siResult,m_stHyungzoPoliceList[index].szName,pclMsg->bAdmin);
	// GM을 수감하려 하는경우
	if ( pclMsg->siResult == -5  )
	{
		cltGameMsgResponse_SetCriminal clinfo(SETCRIMINAL_FAIL_ISGM);
		cltMsg clMsg(GAMEMSG_RESPONSE_SETCRIMINAL, sizeof(clinfo), (BYTE*)&clinfo);
		pclServer->pclCM->CR[pclMsg->usCharID]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else
	{
		cltGameMsgResponse_SetCriminal clinfo( SETCRIMINAL_SUCCESS );
		cltMsg clMsg(GAMEMSG_RESPONSE_SETCRIMINAL, sizeof(clinfo), (BYTE*)&clinfo);
		pclServer->pclCM->CR[pclMsg->usCharID]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void CMinisterHyungzo::DoMsg_GameMsgRequest_GetPoliceList(cltMsg* pclMsg,SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	cltGameMsgResponse_GetPoliceList clinfo(1, &m_stHyungzoPoliceList[0]);
	cltMsg clMsg(GAMEMSG_RESPONSE_GETPOLICELIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclServer->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);
}

void CMinisterHyungzo::DoMsg_GameMsgRequest_SetPolice(cltMsg* pclMsg,SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	if ( pclServer->pclCM->IsValidID(siID) == false ) 
		return ;

	cltGameMsgRequest_SetPolice* pclinfo = (cltGameMsgRequest_SetPolice*)pclMsg->cData;

	if ( pclinfo->bSet )	// 설정
	{
		if ( pclServer->pclCM->CR[siID]->pclCI->clBI.uiGRank != GRANK_MINISTER5 ) 
			return ;

		if( !_tcscmp( pclinfo->szPoliceName, TEXT("")) )
			return;
	}
	else	// 삭제
	{
		if ( pclServer->pclCM->CR[siID]->pclCI->clBI.uiGRank != GRANK_MINISTER5 )
			return;

		if( !_tcscmp( pclinfo->szPoliceName, TEXT("")) )
			return;
	}

	sDBRequest_SetPolice clMsg(siID, pclinfo->szPoliceName, pclinfo->bSet) ;
	pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	
}

void CMinisterHyungzo::DoMsg_GameMsgRequest_GetCriminalList(cltMsg* pclMsg,SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	// 형조의 정보중 현행범 리스트를 요청한다.
	sDBRequest_GetCriminalList clMsg3;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg3);

	cltGameMsgResponse_GetCriminalList clinfo(1, &m_stHyungzoCriminalList[0]);
	cltMsg clMsg(GAMEMSG_RESPONSE_GETCRIMINALLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclServer->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);

}

void CMinisterHyungzo::DoMsg_GameMsgRequest_SetCriminal(cltMsg* pclMsg,SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	if ( pclServer->pclCM->IsValidID(siID) == false ) 
		return ;

	cltGameMsgRequest_SetCriminal* pclinfo = (cltGameMsgRequest_SetCriminal*)pclMsg->cData;

	if ( pclinfo->bSet )	// 설정
	{
		if( pclServer->pclCM->CR[siID]->pclCI->clBI.uiPoliceSwitch == false )
			return;
		if( pclServer->pclCM->CR[siID]->pclCI->clCharItem.clItem[PERSONITEM_HAT].siUnique != ITEMUNIQUE(23490) )
			return;

		if( !_tcscmp( pclinfo->szCriminalName, TEXT("")) )
			return;
		if( !_tcscmp( pclinfo->szCriminalReason, TEXT("")) )
			return;

		SI32 siCriminalID = pclClient->pclCM->GetIDFromName(pclinfo->szCriminalName);
	}
	else	// 삭제
	{
		if ( pclServer->pclCM->CR[siID]->pclCI->clBI.uiGRank != GRANK_MINISTER5 )
			return;

		if( !_tcscmp( pclinfo->szCriminalName, TEXT("")) )
			return;
		if( !_tcscmp( pclinfo->szCriminalReason, TEXT("")) )
			return;
	}

	sDBRequest_SetCriminal clMsg(siID, pclinfo->szCriminalName, pclinfo->szCriminalReason, 60, pclinfo->bSet) ;
	pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

bool CMinisterHyungzo::IsValidPoliceFromPersonID(SI32 personid)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return false ;
	if ( personid <= 0 ) return false ;

	for ( SI32 i = 0 ; i < MAX_HYUNGZO_POLICELIST ; i ++ )
	{
		if ( m_stHyungzoPoliceList[i].siPersonID == personid )
			return true ;
	}

	return false  ;
}
