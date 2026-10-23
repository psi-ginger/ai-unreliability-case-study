
#include "../../Common/CommonHeader.h"
#include "../../CommonLogic/CommonLogic.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../Server.h"

#include "MinisterBase.h"
#include "Minister-Yezo.h"

#include "../../CommonLogic/MsgRval-Define.h"
#include "../../../DBManager/GameDBManager_World/DBMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../CommonLogic/MsgType-System.h"
#include "../../Common/Letter/Letter.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CMinisterYezo::CMinisterYezo() : CMinisterBase()
{

	m_siJapanSwordSolNum	= 10;	// 이도왜구 설정 수	KIND_JAPANSWORDSOL
	m_siJapanGunSolNum		= 10;	// 조총왜구 설정 수	KIND_JAPANGUNSOL
	m_siJapanCommanderNum	= 0;	// 왜구장수 설정 수 KIND_JAPANCOMMANDER
	m_siJapanPorterNum		= 0;	// 잠꾼왜구 설정 수 KIND_JAPANPORTERSOL
	
	m_siWarVillageUnique	= 1;	// 짐꾼왜구가 쳐들어갈 마을 유니크

	//cyj memset(m_stYezoAngelList,0,sizeof(YezoAngelList) * MAX_YEZO_ANGELLIST);
	//cyj memset(m_stYezoAngelApplyerList,0,sizeof(YezoAngelList) * MAX_YEZO_ANGELLIST);
}

CMinisterYezo::~CMinisterYezo()
{
}


void CMinisterYezo::InitializeDB()
{
	// 예조의 정보를 요청한다.
	sDBRequest_GetYezoInfo clMsg;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	// 예조의 정보중 수호천사 신청자 리스트를 요청한다.
	sDBRequest_GetAngelApplyerList clMsg2;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg2);

	// 예조의 정보중 수호천사 리스트를 요청한다.
	sDBRequest_GetAngelList clMsg3;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg3);
}

void  CMinisterYezo::DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket)
{
	sDBResponse_GetYezoInfo* pclMsg = (sDBResponse_GetYezoInfo *)pPacket;
	
	if(pclMsg->siResult)
	{
		StringCchCopy(m_szAccountID, MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szAccountID);
		StringCchCopy(m_szPersonName,	MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szPersonName);
		StringCchCopy(m_szGreeting,	256, pclMsg->MinisterBase.m_szGreeting);

		m_siJapanSwordSolNum	= pclMsg->siJapanSwordSolNum;
		if ( m_siJapanSwordSolNum < 10 || m_siJapanSwordSolNum > 15000 )
			m_siJapanSwordSolNum = 10 ;
		m_siJapanGunSolNum		= pclMsg->siJapanGunSolNum;
		if ( m_siJapanGunSolNum < 10 || m_siJapanGunSolNum > 15000 )
			m_siJapanGunSolNum = 10 ;
		m_siJapanCommanderNum	= pclMsg->siJapanCommanderNum;
		if ( m_siJapanCommanderNum < 0 || m_siJapanCommanderNum > 1000 )
			m_siJapanCommanderNum = 0 ;
		m_siJapanPorterNum		= pclMsg->siJapanPorterNum;
		if ( m_siJapanPorterNum < 0 || m_siJapanPorterNum > 3000 )
			m_siJapanPorterNum = 0 ;
		m_siWarVillageUnique		= pclMsg->siWarVillageUnique;
		if ( m_siWarVillageUnique <= 0 || m_siWarVillageUnique > CUR_VILLAGE_LIMIT )
			m_siWarVillageUnique = 1 ;
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetYezoInfo"));
}

/*cyj
// 수호천사 지원자 목록 요청 응답 - DB서 읽어올 때
void CMinisterYezo::DoMsg_DBMsgResponse_GetAngelApplyerList(sPacketHeader* pPacket)
{
	sDBResponse_GetAngelApplyerList* pclMsg = (sDBResponse_GetAngelApplyerList *)pPacket;

	if(pclMsg->siResult)
	{
		for ( int i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
		{
			memcpy(m_stYezoAngelApplyerList[i].szAccountID,pclMsg->stYezoAngelApplyerList[i].szAccountID,32);
			memcpy(m_stYezoAngelApplyerList[i].szName,pclMsg->stYezoAngelApplyerList[i].szName,MAX_PLAYER_NAME);

			m_stYezoAngelApplyerList[i].siLevel = pclMsg->stYezoAngelApplyerList[i].siLevel ;
			m_stYezoAngelApplyerList[i].siFameLevel = pclMsg->stYezoAngelApplyerList[i].siFameLevel ;

			m_stYezoAngelApplyerList[i].siPersonID = pclMsg->stYezoAngelApplyerList[i].siPersonID ;
		}
	}
}

// 수호천사 목록 요청 응답 - DB서 읽어올 때
void CMinisterYezo::DoMsg_DBMsgResponse_GetAngelList(sPacketHeader* pPacket)
{
	sDBResponse_GetAngelList* pclMsg = (sDBResponse_GetAngelList*)pPacket;

	if(pclMsg->siResult)
	{
		for ( int i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
		{
			memcpy(m_stYezoAngelList[i].szAccountID,pclMsg->stYezoAngelList[i].szAccountID,32);
			memcpy(m_stYezoAngelList[i].szName,pclMsg->stYezoAngelList[i].szName,MAX_PLAYER_NAME);

			m_stYezoAngelList[i].siLevel = pclMsg->stYezoAngelList[i].siLevel ;
			m_stYezoAngelList[i].siFameLevel = pclMsg->stYezoAngelList[i].siFameLevel ;

			m_stYezoAngelList[i].siPersonID = pclMsg->stYezoAngelList[i].siPersonID ;
		}
	}
}
*/

void CMinisterYezo::DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID)
{
	CMinisterBase *pMinister = (CMinisterBase* )(this);

	cltGameMsgResponse_GetYezoInfo	clinfo(pMinister, m_siJapanSwordSolNum, m_siJapanGunSolNum, m_siJapanCommanderNum,m_siJapanPorterNum,m_siWarVillageUnique);
	cltMsg clMsg(GAMEMSG_RESPONSE_GETYEZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
	((cltServer *)pclClient)->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);

}

// Client에서 예조의 정보 새로 설정하기를 원하면 서버에서는 DB에 정보 설정을 요청한다.
void CMinisterYezo::DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID)
{
	cltGameMsgRequest_SetYezoInfo *pclinfo = (cltGameMsgRequest_SetYezoInfo*)pclMsg->cData;
	
	sDBRequest_SetYezoInfo clMsg(siID, ((cltServer *)pclClient)->pclCM->CR[siID]->pclCI->GetPersonID(),
		pclinfo->szGreeting, pclinfo->siJapanSwordSolNum, pclinfo->siJapanGunSolNum, pclinfo->siJapanCommanderNum,pclinfo->siJapanPorterNum,pclinfo->siWarVillageUnique);

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

// DB에서 정보의 쓰기가 완료되면 Client에게 다시 전송하여준다.
void CMinisterYezo::DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket)
{
	sDBResponse_SetYezoInfo* pclMsg = (sDBResponse_SetYezoInfo*)pPacket;

	SI32 id = pclMsg->usCharID;
	if(((cltServer *)pclClient)->pclCM->IsAlive(id) == false)return ;
	
	if(pclMsg->siResult == 1)
	{
		// 동일인이라면 
		if(((cltServer *)pclClient)->pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
		{
			StringCchCopy(m_szAccountID,	MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szAccountID);
			StringCchCopy(m_szPersonName,	MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szPersonName);
			StringCchCopy(m_szGreeting,	256, pclMsg->MinisterBase.m_szGreeting);
			
			m_siJapanSwordSolNum	= pclMsg->siJapanSwordSolNum;
			m_siJapanGunSolNum		= pclMsg->siJapanGunSolNum;		
			m_siJapanCommanderNum	= pclMsg->siJapanCommanderNum;
			m_siJapanPorterNum		= pclMsg->siJapanPorterNum;

			m_siWarVillageUnique	= pclMsg->siWarVillageUnique;
		}
		
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 4] // 예조설정.
			// param1 == 이도왜구 설정 수, param2 == 조총왜구 설정 수, param3 == 왜구장수 설정 수 , param4 == 짐꾼왜구 설정 수,	
			// pszParam1 == 계정. pszParam2 == 이름.	
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_YEZO_SET, 
				0, (cltCharServer*)pclClient->pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siWarVillageUnique, 0, 0, 
				pclMsg->siJapanSwordSolNum,  pclMsg->siJapanGunSolNum, 
				pclMsg->siJapanCommanderNum, pclMsg->siJapanPorterNum, 0, 
				pclMsg->MinisterBase.m_szAccountID,		 pclMsg->MinisterBase.m_szPersonName);
		}
	}
	else
	{
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETYEZOINFO,  0, 0, id);
	}
	
	// 정보를 요청했을때와 같이 돌려준다.
	DoMsg_GameMsgRequest_GetMinisterInfo(id);

}

/*cyj
// Server&Client
// 수호천사 지원자 목록 요청 - 서버의 정보를 보내준다.
void CMinisterYezo::DoMsg_GameMsgRequest_GetAngelApplyerList(cltMsg* pclMsg, SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	cltGameMsgResponse_GetAngelApplyerList clinfo(1,m_stYezoAngelApplyerList);
	cltMsg clMsg(GAMEMSG_RESPONSE_GETANGELAPPLYERLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclServer->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);
}

// 수호천사 목록 요청 - 서버의 정보를 보내준다.
void CMinisterYezo::DoMsg_GameMsgRequest_GetAngelList(cltMsg* pclMsg, SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	cltGameMsgResponse_GetAngelList clinfo(1,m_stYezoAngelList);
	cltMsg clMsg(GAMEMSG_RESPONSE_GETANGELLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclServer->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);
}

// 수호천사 등록신청 및 신청자 삭제
void CMinisterYezo::DoMsg_GameMsgRequest_SetAngelApplyer(cltMsg* pclMsg, SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	cltGameMsgRequest_SetAngelApplyer * pclinfo = (cltGameMsgRequest_SetAngelApplyer*)pclMsg->cData ;

	SI32 personid = 0 ;

	if ( pclinfo->bAdmin == false && pclinfo->siIndex != -1 ) // 예조판서가 해임
	{
		if ( pclinfo->siIndex < 0 || pclinfo->siIndex > MAX_YEZO_ANGELLIST ) return ;
		if ( pclServer->pclCM->CR[siID]->pclCI->clBI.uiGRank != GRANK_MINISTER3 ) return ;

		personid = m_stYezoAngelApplyerList[pclinfo->siIndex].siPersonID ;
	}
	else // 신청이나 스스로 탈퇴
	{
		personid = pclServer->pclCM->CR[siID]->pclCI->GetPersonID() ;

		if ( pclinfo->bAdmin == false ) // 스스로 탈퇴시
		{
			if ( IsValidApplyerFromPersonID(personid) == false ) // 신청자가 맞나 안 맞나
			{
				pclServer->SendServerResponseMsg(0, SRVAL_HOZO_FAIL_SETANGELAPPLYER_FALSE, 0, 0, siID );
				return ;
			}
		}
		else // 신청시
		{
			if ( IsValidApplyerFromPersonID(personid) == true ) // 이미 신청했는지 안 했는지
			{
				pclServer->SendServerResponseMsg(0, SRVAL_HOZO_FAIL_SETANGELAPPLYER, 0, 0, siID );
				return ;
			}
		}
	}

	if ( personid <= 0 ) return ;

	sDBRequest_SetAngelApplyer clMsg(siID,personid,pclinfo->bAdmin) ;
	pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return ;
}

//수호천사 임명 및 해임
void CMinisterYezo::DoMsg_GameMsgRequest_SetAngel(cltMsg* pclMsg, SI32 siID)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	cltGameMsgRequest_SetAngel * pclinfo = (cltGameMsgRequest_SetAngel*)pclMsg->cData ;

	SI32 personid = 0 ;
	if ( pclinfo->bAdmin )
	{
		if ( pclServer->pclCM->CR[siID]->pclCI->clBI.uiGRank != GRANK_MINISTER3 ) return ;
		if ( pclinfo->siIndex < 0 || pclinfo->siIndex > MAX_YEZO_ANGELLIST ) return ;
		if ( !_tcscmp(m_stYezoAngelApplyerList[pclinfo->siIndex].szName, TEXT("")) ) return ;
		if ( m_stYezoAngelApplyerList[pclinfo->siIndex].siLevel <= 0 ) return ;

		personid = m_stYezoAngelApplyerList[pclinfo->siIndex].siPersonID ;
	}
	else
	{
		if ( pclServer->pclCM->CR[siID]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
		{
			if ( pclinfo->siIndex < 0 || pclinfo->siIndex > MAX_YEZO_ANGELLIST ) return ;
			if ( !_tcscmp(m_stYezoAngelList[pclinfo->siIndex].szName, TEXT("")) ) return ;
			if ( m_stYezoAngelList[pclinfo->siIndex].siLevel <= 0 ) return ;

			personid = m_stYezoAngelList[pclinfo->siIndex].siPersonID ;
		}
		else if ( pclServer->pclCM->CR[siID]->pclCI->clBI.uiAngelSwitch && pclinfo->siIndex == -1 )
		{
			personid = pclServer->pclCM->CR[siID]->pclCI->GetPersonID();
		}
	}

	if ( personid <= 0 ) return ;

	sDBRequest_SetAngel clMsg(siID,personid,pclinfo->bAdmin) ;
	pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	return ;
}

// Server&DB
// 수호천사 지원 응답
void CMinisterYezo::DoMsg_DBMsgResponse_SetAngelApplyer(sPacketHeader* pPacket)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	sDBResponse_SetAngelApplyer* pclMsg = (sDBResponse_SetAngelApplyer *)pPacket;

	if(pclMsg->siResult == 1)
	{
		if ( pclMsg->bAdmin ) // 등록
		{
			SI32 index = pclMsg->siIndex ;

			m_stYezoAngelApplyerList[index].siPersonID = pclMsg->stYezoAngelApplyer.siPersonID ;
			m_stYezoAngelApplyerList[index].siFameLevel = pclMsg->stYezoAngelApplyer.siFameLevel ;
			m_stYezoAngelApplyerList[index].siLevel = pclMsg->stYezoAngelApplyer.siLevel ;
			strcpy(m_stYezoAngelApplyerList[index].szAccountID,pclMsg->stYezoAngelApplyer.szAccountID);
			strcpy(m_stYezoAngelApplyerList[index].szName,pclMsg->stYezoAngelApplyer.szName);

			if ( pclServer->pclCM->GetIDFromPersonID(pclMsg->siPersonID) == pclMsg->usCharID &&
				 pclServer->pclCM->IsValidID(pclMsg->usCharID) == true )
			{
				pclServer->SendServerResponseMsg(0, SRVAL_YEZO_SUCCESS_SETANGELAPPLYER, 0, 0, pclMsg->usCharID );
			}
		}
		else // 해지
		{
			SI32 index = pclMsg->siIndex ;

			SI32 deletepersonid = m_stYezoAngelApplyerList[index].siPersonID ;

			m_stYezoAngelApplyerList[index].siPersonID = 0 ;
			m_stYezoAngelApplyerList[index].siFameLevel = 0 ;
			m_stYezoAngelApplyerList[index].siLevel = 0 ;
			strcpy(m_stYezoAngelApplyerList[index].szAccountID, TEXT(""));
			strcpy(m_stYezoAngelApplyerList[index].szName, TEXT(""));

			if ( pclServer->pclCM->CR[pclMsg->usCharID]->pclCI->clBI.uiGRank == GRANK_MINISTER3 &&
				 pclServer->pclCM->IsValidID(pclMsg->usCharID) )
			{
				cltGameMsgResponse_SetAngelApplyer clinfo(pclMsg->bAdmin,pclMsg->bAdmin,index);
				cltMsg clMsg(GAMEMSG_RESPONSE_SETANGELAPPLYER, sizeof(clinfo), (BYTE*)&clinfo);
				pclServer->pclCM->CR[pclMsg->usCharID]->SendNetMsg((sPacketHeader*)&clMsg);

				if ( deletepersonid )
				{
					cltLetterMsg_YezoAngel clletter(3, &pclServer->pclTime->clDate);
					pclServer->SendLetterMsg(deletepersonid, (cltLetterHeader*)&clletter);
				}
			}
			else if ( pclServer->pclCM->GetIDFromPersonID(pclMsg->siPersonID) == pclMsg->usCharID &&
					  pclServer->pclCM->IsValidID(pclMsg->usCharID) )
			{
				pclServer->SendServerResponseMsg(0, SRVAL_YEZO_SUCCESS_SETANGELAPPLYER_FALSE, 0, 0, pclMsg->usCharID );
			}
		}
	}
	else if ( pclServer->pclCM->IsValidID(pclMsg->usCharID) )
	{
		pclServer->SendServerResponseMsg(0, SRVAL_HOZO_FAIL_SETANGELAPPLYER, 0, 0, pclMsg->usCharID );
	}
}

// 수호천사 임명 응답
void CMinisterYezo::DoMsg_DBMsgResponse_SetAngel(sPacketHeader* pPacket)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return ;

	sDBResponse_SetAngel* pclMsg = (sDBResponse_SetAngel *)pPacket;

	SI32 index = -1 ;

	if(pclMsg->siResult == 1)
	{
		if ( pclMsg->bAdmin ) // 임명
		{
			index = pclMsg->siIndex ;

			m_stYezoAngelList[index].siPersonID = pclMsg->stYezoAngel.siPersonID ;
			m_stYezoAngelList[index].siFameLevel = pclMsg->stYezoAngel.siFameLevel ;
			m_stYezoAngelList[index].siLevel = pclMsg->stYezoAngel.siLevel ;
			strcpy(m_stYezoAngelList[index].szAccountID,pclMsg->stYezoAngel.szAccountID);
			strcpy(m_stYezoAngelList[index].szName,pclMsg->stYezoAngel.szName);

			for ( int i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
			{
				if ( m_stYezoAngelApplyerList[i].siPersonID == pclMsg->stYezoAngel.siPersonID )
				{
					m_stYezoAngelApplyerList[i].siPersonID = 0 ;
					m_stYezoAngelApplyerList[i].siFameLevel = 0 ;
					m_stYezoAngelApplyerList[i].siLevel = 0 ;
					strcpy(m_stYezoAngelApplyerList[i].szAccountID, TEXT(""));
					strcpy(m_stYezoAngelApplyerList[i].szName, TEXT(""));
					break;
				}
			}

			if ( pclServer->pclCM->GetIDFromPersonID(pclMsg->siPersonID) == pclMsg->usCharID )
			{
				pclServer->SendServerResponseMsg(0, SRVAL_YEZO_SUCCESS_SETANGEL, 0, 0, pclMsg->usCharID );
			}

			if ( m_stYezoAngelList[index].siPersonID )
			{
				cltLetterMsg_YezoAngel clletter(1, &pclServer->pclTime->clDate);
				pclServer->SendLetterMsg(m_stYezoAngelList[index].siPersonID, (cltLetterHeader*)&clletter);

				SI32 toid = pclServer->pclCM->GetIDFromPersonID(m_stYezoAngelList[index].siPersonID) ;
				if ( IsPC(toid) )
				{
					pclServer->pclCM->CR[toid]->SetAngel(true);
				}
			}
		}
		else // 해임이나 탈퇴
		{
			index = pclMsg->siIndex ;

			if ( m_stYezoAngelList[index].siPersonID &&
				 m_stYezoAngelList[index].siPersonID != pclMsg->siPersonID )
			{
				cltLetterMsg_YezoAngel clletter(2, &pclServer->pclTime->clDate);
				pclServer->SendLetterMsg(m_stYezoAngelList[index].siPersonID, (cltLetterHeader*)&clletter);

				SI32 toid = pclServer->pclCM->GetIDFromPersonID(m_stYezoAngelList[index].siPersonID) ;
				if ( IsPC(toid) )
				{
					pclServer->pclCM->CR[toid]->SetAngel(false);
				}
			}
			else if ( m_stYezoAngelList[index].siPersonID &&
					  m_stYezoAngelList[index].siPersonID == pclMsg->siPersonID )
			{
				SI32 toid = pclServer->pclCM->GetIDFromPersonID(m_stYezoAngelList[index].siPersonID) ;
				if ( IsPC(toid) )
				{
					pclServer->pclCM->CR[toid]->SetAngel(false);
					pclServer->SendServerResponseMsg(0, SRVAL_YEZO_SUCCESS_SETANGEL_FALSE_SELF, 0, 0, toid );
				}
			}
			if ( pclServer->pclCM->GetIDFromPersonID(pclMsg->siPersonID) == pclMsg->usCharID &&
				pclServer->pclCM->CR[pclMsg->usCharID]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
			{
				pclServer->SendServerResponseMsg(0, SRVAL_YEZO_SUCCESS_SETANGELADMIN_FALSE, 0, 0, pclMsg->usCharID );
			}

			m_stYezoAngelList[index].siPersonID = 0 ;
			m_stYezoAngelList[index].siFameLevel = 0 ;
			m_stYezoAngelList[index].siLevel = 0 ;
			strcpy(m_stYezoAngelList[index].szAccountID, TEXT(""));
			strcpy(m_stYezoAngelList[index].szName, TEXT(""));
		}
	}

	if ( pclServer->pclCM->IsValidID(pclMsg->usCharID) == false ) return ;

	cltGameMsgResponse_SetAngel clinfo(pclMsg->bAdmin,index,pclMsg->bAdmin);
	cltMsg clMsg(GAMEMSG_RESPONSE_SETANGEL, sizeof(clinfo), (BYTE*)&clinfo);
	pclServer->pclCM->CR[pclMsg->usCharID]->SendNetMsg((sPacketHeader*)&clMsg);
}

bool CMinisterYezo::IsValidAngelFromPersonID(SI32 personid)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return false ;
	if ( personid <= 0 ) return false ;

	for ( SI32 i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
	{
		if ( m_stYezoAngelList[i].siPersonID == personid )
			return true ;
	}

	return false  ;
}

bool CMinisterYezo::IsValidAngelFromCharID(SI32 charid)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return false ;
	if ( pclServer->pclCM->GetIDFromCharUnique(charid) != charid ) return false ;
	if ( IsPC(pclServer->pclCM->CR[charid]->GetCharUnique()) == false ) return false ;

	SI32 personid = pclServer->pclCM->CR[charid]->pclCI->GetPersonID() ;

	if ( personid <= 0 ) return false ;

	return IsValidAngelFromPersonID(personid) ;
}

bool CMinisterYezo::IsValidAngelFromCharName(const TCHAR * name)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return false ;
	if ( !_tcscmp(TEXT(""),name) || name == NULL ) return false ;

	for ( SI32 i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
	{
		if ( !_tcscmp(m_stYezoAngelList[i].szName,name) )
		{
			return true ;
		}
	}

	return false ;
}

bool CMinisterYezo::IsValidApplyerFromPersonID(SI32 personid)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return false ;
	if ( personid <= 0 ) return false ;

	for ( SI32 i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
	{
		if ( m_stYezoAngelApplyerList[i].siPersonID == personid )
			return true ;
	}

	return false  ;
}

bool CMinisterYezo::IsValidApplyerFromFromCharID(SI32 charid)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return false ;
	if ( pclServer->pclCM->GetIDFromCharUnique(charid) != charid ) return false ;
	if ( IsPC(pclServer->pclCM->CR[charid]->GetCharUnique()) == false ) return false ;

	SI32 personid = pclServer->pclCM->CR[charid]->pclCI->GetPersonID() ;

	if ( personid <= 0 ) return false ;

	return IsValidApplyerFromPersonID(personid) ;
}

bool CMinisterYezo::IsValidApplyerFromFromCharName(const TCHAR * name)
{
	cltServer* pclServer = ((cltServer *)pclClient) ;
	if ( pclServer == NULL ) return false ;
	if ( !_tcscmp(TEXT(""),name) || name == NULL ) return false ;

	for ( SI32 i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
	{
		if ( !_tcscmp(m_stYezoAngelApplyerList[i].szName,name) )
		{
			return true ;
		}
	}

	return false ;
}

SI32 CMinisterYezo::GetAngelNumber()
{
	SI32 count = 0 ;
	for ( SI32 i = 0 ; i < MAX_YEZO_ANGELLIST ; i ++ )
	{
		if ( m_stYezoAngelApplyerList[i].siPersonID > 0 )
		{
			count ++ ;
		}
	}

	return count ;
}
*/