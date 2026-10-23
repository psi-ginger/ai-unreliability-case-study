//---------------------------------
// 2003/6/9 김태곤
//---------------------------------
#include "CityHall.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"


#include "..\..\CommonLogic\CommonLogic.h"
#include "..\..\Server\Candidate\Candidate.h"
#include "TaxPaymentListDlg.h"

#include "..\..\Resource.h"
#include "MsgType-Structure.h"


#include "..\..\CommonLogic\Bank\Bank-Manager.h"
#include "..\..\CommonLogic\Hunt\Hunt.h"
#include "..\..\CommonLogic\PostOffice\PostOffice.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Structure.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-CityHall.h"

#include "..\..\Server\Time\MyTime.h"
#include "..\..\Server\Server.h"

#include "../common/Event/event.h"

extern cltCommonLogic* pclClient;


void cltChatMemberManager::AddChatMember( int charid, int personid )
{

	DelChatMember( personid );

	for( int i = 0; i < 200; ++i )
	{
		if( clChatMember[ i ].charid == 0 ) 
		{
			clChatMember[ i ].charid = charid;
			clChatMember[ i ].personid = personid;
			break;
		}
	}
}

void cltChatMemberManager::DelChatMember( int personid )
{
	for( int i = 0; i < 200; ++i )
	{
		if( clChatMember[ i ].personid == personid ) 
		{
			clChatMember[ i ].charid = 0;
			clChatMember[ i ].personid = 0;
			break;
		}
	}		
}

void cltChatMemberManager::SendMsg( BYTE* msg )
{
	cltServer *pclServer = (cltServer *)pclClient;
	for( int i = 0; i < 200; ++i )
	{
		int id = clChatMember[ i ].charid;
		if( id == 0 ) continue;

		if( pclServer->pclCM->IsValidID(id) == FALSE ||
			pclServer->pclCM->CR[ id ]->pclCI->GetPersonID() 
			!= clChatMember[ i ].personid )
		{

			clChatMember[ i ].charid = 0;
			clChatMember[ i ].personid = 0;

			continue;
		}


		pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)msg );
	}
}



//----------------------------------------------
// cltCityHall
//----------------------------------------------
cltCityHall::cltCityHall(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{
	pclCandidateManager = new cltCityHallCandidateManager();
	
	int i;

	for( i = 0; i < MAX_RESIDENT_APPLYER; i++ ) {
		clResidentApplyer[ i ].Init();
	}

	bPrepareBankruptcy = FALSE;
}

cltCityHall::~cltCityHall()
{

	if( pclCandidateManager )
	{
		delete pclCandidateManager;
		pclCandidateManager = NULL;
	}
}

// Chief 후보를 등록한다. 
BOOL cltCityHall::SetCandidate(SI32 index, cltSimpleRank* pclrank, cltSimplePerson* pclcandidate)
{
	return pclCandidateManager->SetCandidate(index, pclrank->GetRankType(), pclcandidate);
}

// 후보 등록을 취소한다. 
BOOL cltCityHall::DeleteCandidate(SI32 personid)
{
	return pclCandidateManager->DeletePerson(personid);
}

// 특정 신분의 후보 등록을 모두 취소한다. 
BOOL cltCityHall::DeleteCandidate(cltSimpleRank* pclrank)
{
	return pclCandidateManager->DeleteRank(pclrank->siType);
}

cltSimplePerson* cltCityHall::GetCandidateInfoFromPersonID(SI32 ranktype, SI32 personid, SI32* pindex)
{
	if(pclCandidateManager == NULL)return NULL;

	return pclCandidateManager->GetCandidateInfoFromPersonID(ranktype, personid, pindex);
}

// 입후보자의 숫자를 구한다. 
SI32 cltCityHall::GetCandidateNum(SI32 ranktype)
{
	return pclCandidateManager->GetCandidateNum(ranktype);
}


cltSimplePerson* cltCityHall::GetCandidateInfoFromRef(SI32 ranktype, SI32 ref)
{
	return pclCandidateManager->GetCandidateInfoFromRef(ranktype, ref);
}


// 주민 등록 신청자를 추가한다 
BOOL cltCityHall::AddApplyer( cltResidentApplyer *pclApplyer )
{
	SI32 i;

	for( i = 0; i < MAX_RESIDENT_APPLYER; i++ )
	{
		if( clResidentApplyer[ i ].clApplyer.GetPersonID() == 0 )
		{
			clResidentApplyer[ i ].Set( pclApplyer );
			return TRUE;
		}
	}

	return FALSE;
}

// 주민 등록 신청자를 삭제한다
BOOL cltCityHall::DeleteApplyer( SI32 personid )
{
	SI32 i;

	for( i = 0; i < MAX_RESIDENT_APPLYER; i++ )
	{
		if( clResidentApplyer[ i ].clApplyer.GetPersonID() == personid )
		{
			clResidentApplyer[ i ].Init();
			return TRUE;
		}
	}

	return FALSE;
}




void cltCityHall::SetSymbol( SI16 symbolKind, SI32 symbolLife, SI32 symbolMana, SI16 symbolCapa )
{
	clStrInfo.clCityhallStrInfo.siSymbolKind = symbolKind;
	clStrInfo.clCityhallStrInfo.siSymbolLife = symbolLife;
	clStrInfo.clCityhallStrInfo.siSymbolMana = symbolMana;
	clStrInfo.clCityhallStrInfo.siSymbolCapa = symbolCapa;

	sDBRequest_SetVillageSymbol sendMsg( siVillageUnique, symbolKind, symbolLife, symbolMana, symbolCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	// [로그추가 : 황진성 2007. 10. 9] // 마을 수호신.
	//// param1 == 심볼(수호신) 종류, param2 == 심볼(수호신) 체력, param3 == 심볼(수호신) Mana, param4 == 심볼(수호신) 성능.
	//cltServer * pclServer = (cltServer*)pclClient;
	//pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_SYMBOL_SET, 
	//								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//								symbolKind, symbolLife, symbolMana, symbolCapa, 0, NULL, NULL);
	//}
}

void cltCityHall::SetSymbolLife( SI32 symbolLife )
{
	clStrInfo.clCityhallStrInfo.siSymbolLife = symbolLife;

	sDBRequest_SetVillageSymbol sendMsg( siVillageUnique, 
		clStrInfo.clCityhallStrInfo.siSymbolKind, 
		symbolLife,
		clStrInfo.clCityhallStrInfo.siSymbolMana,
		clStrInfo.clCityhallStrInfo.siSymbolCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	// [로그추가 : 황진성 2007. 10. 9] // 마을 수호신.
	//// param1 == 심볼(수호신) 종류, param2 == 심볼(수호신) 체력, param3 == 심볼(수호신) Mana, param4 == 심볼(수호신) 성능.
	//cltServer * pclServer = (cltServer*)pclClient;
	//pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_SYMBOL_SET, 
	//								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//								clStrInfo.clCityhallStrInfo.siSymbolKind, symbolLife,
	//								clStrInfo.clCityhallStrInfo.siSymbolMana, clStrInfo.clCityhallStrInfo.siSymbolCapa, 0, NULL, NULL);
	//}
}

void cltCityHall::SetSymbolMana( SI32 symbolMana )
{
	clStrInfo.clCityhallStrInfo.siSymbolMana = symbolMana;

	sDBRequest_SetVillageSymbol sendMsg( siVillageUnique, 
		clStrInfo.clCityhallStrInfo.siSymbolKind, 
		clStrInfo.clCityhallStrInfo.siSymbolLife, 
		symbolMana,
		clStrInfo.clCityhallStrInfo.siSymbolCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	// [로그추가 : 황진성 2007. 10. 9] // 마을 수호신.
	//// param1 == 심볼(수호신) 종류, param2 == 심볼(수호신) 체력, param3 == 심볼(수호신) Mana, param4 == 심볼(수호신) 성능.
	//cltServer * pclServer = (cltServer*)pclClient;
	//pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_SYMBOL_SET, 
	//								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//								clStrInfo.clCityhallStrInfo.siSymbolKind, 
	//								clStrInfo.clCityhallStrInfo.siSymbolLife, 
	//								symbolMana,
	//								clStrInfo.clCityhallStrInfo.siSymbolCapa, 0, NULL, NULL);
	//}
}

void cltCityHall::SetSymbolCapa( SI16 symbolCapa )
{
	clStrInfo.clCityhallStrInfo.siSymbolCapa = symbolCapa;

	sDBRequest_SetVillageSymbol sendMsg( siVillageUnique, 
		clStrInfo.clCityhallStrInfo.siSymbolKind, 
		clStrInfo.clCityhallStrInfo.siSymbolLife, 
		clStrInfo.clCityhallStrInfo.siSymbolMana,
		symbolCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	// [로그추가 : 황진성 2007. 10. 9] // 마을 수호신.
	//// param1 == 심볼(수호신) 종류, param2 == 심볼(수호신) 체력, param3 == 심볼(수호신) Mana, param4 == 심볼(수호신) 성능.
	//cltServer * pclServer = (cltServer*)pclClient;
	//pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_SYMBOL_SET, 
	//								0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//								clStrInfo.clCityhallStrInfo.siSymbolKind, 
	//								clStrInfo.clCityhallStrInfo.siSymbolLife, 
	//								clStrInfo.clCityhallStrInfo.siSymbolMana,
	//								symbolCapa , 0, NULL, NULL);
	//}
}




void cltCityHall::SetCastle( SI16 castleKind, SI32 castleLife, SI32 castleMana, SI16 castleCapa )
{
	clStrInfo.clCityhallStrInfo.siCastleKind = castleKind;
	clStrInfo.clCityhallStrInfo.siCastleLife = castleLife;
	clStrInfo.clCityhallStrInfo.siCastleMana = castleMana;
	clStrInfo.clCityhallStrInfo.siCastleCapa = castleCapa;

	sDBRequest_SetVillageCastle sendMsg( siVillageUnique, castleKind, castleLife, castleMana, castleCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 9] // 마을밖 성벽의 상태가 변경될 때.
	//	// param1 == 캐슬 종류, param2 ==  캐슬 체력, param3 == 캐슬 Mana, param4 == 캐슬 성능.
	//	cltServer * pclServer = (cltServer*)pclClient;
	//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_CASTLE_SET, 
	//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//									castleKind, castleLife, castleMana, castleCapa, 0, NULL, NULL);
	//}
}

void cltCityHall::SetCastleLife( SI32 castleLife )
{
	clStrInfo.clCityhallStrInfo.siCastleLife = castleLife;

	sDBRequest_SetVillageCastle sendMsg( siVillageUnique, 
		clStrInfo.clCityhallStrInfo.siCastleKind, 
		castleLife,
		clStrInfo.clCityhallStrInfo.siCastleMana,
		clStrInfo.clCityhallStrInfo.siCastleCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 9] // 마을밖 성벽의 상태가 변경될 때.
	//	// param1 == 캐슬 종류, param2 ==  캐슬 체력, param3 == 캐슬 Mana, param4 == 캐슬 성능.
	//	cltServer * pclServer = (cltServer*)pclClient;
	//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_CASTLE_SET, 
	//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//									clStrInfo.clCityhallStrInfo.siCastleKind, 
	//									castleLife,
	//									clStrInfo.clCityhallStrInfo.siCastleMana,
	//									clStrInfo.clCityhallStrInfo.siCastleCapa, 0, NULL, NULL);
	//}
}

void cltCityHall::SetCastleMana( SI32 castleMana )
{
	clStrInfo.clCityhallStrInfo.siCastleMana = castleMana;

	sDBRequest_SetVillageCastle sendMsg( siVillageUnique, 
		clStrInfo.clCityhallStrInfo.siCastleKind, 
		clStrInfo.clCityhallStrInfo.siCastleLife, 
		castleMana,
		clStrInfo.clCityhallStrInfo.siCastleCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 9] // 마을밖 성벽의 상태가 변경될 때.
	//	// param1 == 캐슬 종류, param2 ==  캐슬 체력, param3 == 캐슬 Mana, param4 == 캐슬 성능.
	//	cltServer * pclServer = (cltServer*)pclClient;
	//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_CASTLE_SET, 
	//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//									clStrInfo.clCityhallStrInfo.siCastleKind, 
	//									clStrInfo.clCityhallStrInfo.siCastleLife, 
	//									castleMana,
	//									clStrInfo.clCityhallStrInfo.siCastleCapa, 0, NULL, NULL);
	//}
}

void cltCityHall::SetCastleCapa( SI16 castleCapa )
{
	clStrInfo.clCityhallStrInfo.siCastleCapa = castleCapa;

	sDBRequest_SetVillageCastle sendMsg( siVillageUnique, 
		clStrInfo.clCityhallStrInfo.siCastleKind, 
		clStrInfo.clCityhallStrInfo.siCastleLife, 
		clStrInfo.clCityhallStrInfo.siCastleMana,
		castleCapa );

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [로그추가 : 황진성 2007. 10. 9] // 마을밖 성벽의 상태가 변경될 때.
	//	// param1 == 캐슬 종류, param2 ==  캐슬 체력, param3 == 캐슬 Mana, param4 == 캐슬 성능.
	//	cltServer * pclServer = (cltServer*)pclClient;
	//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_CASTLE_SET, 
	//									0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, siVillageUnique, 0, 0, 
	//									clStrInfo.clCityhallStrInfo.siCastleKind, 
	//									clStrInfo.clCityhallStrInfo.siCastleLife, 
	//									clStrInfo.clCityhallStrInfo.siCastleMana,
	//									castleCapa, 0, NULL, NULL);
	//}
}





//---------------------------------------------------------
// cltCityHallManager
//---------------------------------------------------------
cltCityHallManager::cltCityHallManager(cltCharManager* pclcm, SI32 gamemode, SI32 ranktype, TCHAR *strFileName ) : cltStructureMngBasic(MAX_CITYHALL_TAB_NUMBER, pclcm, ranktype, strFileName )
{

	UpdateWarTreeSwitch = FALSE;

	UpdateInvestSwitch  = FALSE;
	UpdateMayorCandidateSwitch	= FALSE;
	UpdateChiefCandidateSwitch	= FALSE;
	
	UpdateResidentInfoSwitch = FALSE;	
	bResidentInfoValid = FALSE;

	UpdateLevelInfoSwitch = FALSE;

	if(gamemode == GAMEMODE_CLIENT)
	{

		pclClientCandidateManager = new cltCityHallCandidateManager(); 
	}
	else
	{
		pclClientCandidateManager = NULL;
	}

	// 이벤트 등록 AddEvent( eventid, 기본비용, 이벤트명, 설명 )
	TCHAR* pTilte		= TEXT(" ");
	TCHAR* pText1		= TEXT(" ");
	TCHAR* pText2		= TEXT(" ");
	TCHAR pText[512]	= TEXT(" ");

	// 라티우스 대수렵제 이벤트 EVENT_HUNT
	if(	pclClient->IsCountrySwitch( Switch_Event_Hunt ) )
	{
		pTilte = GetTxtFromMgr(1606);
		pText1 = GetTxtFromMgr(1607);
		pText2 = GetTxtFromMgr(6721);
		 
		wsprintf(pText, "%s\r\n%s", pText1, pText2);

		clEventManager.AddEvent( CITYHALL_EVENT_HUNT, 20000000, pTilte, pText );
	}

	// 모후 이벤트 - EVENT_MOFUMOFU	
	if ( pclClient->IsCountrySwitch( Switch_Event_MofuMofu ) )
	{
		pTilte = GetTxtFromMgr(7068);
		pText1 = GetTxtFromMgr(7069);
		pText2 = GetTxtFromMgr(7070);

		wsprintf(pText, "%s\r\n%s", pText1, pText2);

		clEventManager.AddEvent( CITYHALL_EVENT_MOFUMOFU, 20000000 * 1.5, pTilte, pText );
	}

	// [진성] 마을 이벤트 - 몬스터 습격. 2008-3-26	
	if ( pclClient->IsCountrySwitch( Switch_Event_Monster_Attack ) )
	{
		pTilte = GetTxtFromMgr(8117);
		pText1 = GetTxtFromMgr(8118);
	
		
		//SI32 siCost = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate * 500000;

		clEventManager.AddEvent( CITYHALL_EVENT_MONSTER_ATTACK, 20000000 * 1.3, pTilte, pText1 );
	}

//	현재 강화도 이벤트는 쓰고있지 않습니다.

//	pTilte = GetTxtFromMgr(1608);
//	pText = GetTxtFromMgr(1609);

//	clEventManager.AddEvent( 2, 1000000, pTilte, pText );
}


cltCityHallManager::~cltCityHallManager()
{


	if( pclClientCandidateManager ) delete pclClientCandidateManager;

}

void cltCityHallManager::Update()
{
	// 건물에 대한 정보를 서버에 요청한다. 

	SI32 id = 1;

	if(id)
	{
		cltGameMsgRequest_StrInfo clInfo(siVillageUnique, RANKTYPE_CITYHALL);
		cltMsg clMsg(GAMEMSG_REQUEST_STRINFO, sizeof(clInfo), (BYTE*)&clInfo);
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}


// 최초 환영 인사말표시 . 
void cltCityHallManager::GreetingMsg(HWND hDlg)
{
	TCHAR* pText = GetTxtFromMgr(1610);

	SetMainExplain(hDlg, pText);
}

void cltCityHallManager::Do_WM_INITDIALOG(SI32 charunique)
{

	cltStructureMngBasic::Do_WM_INITDIALOG(charunique);

	SI32 id = 1;
	
	bChiefInfo = FALSE;
	bRecvChiefInfo = FALSE;


	// 모든 건물의 행수 정보 요청 
	{
		cltMsg clMsg(GAMEMSG_REQUEST_ALLSTRCHIEFINFO, siVillageUnique );
		pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}	
}

// 상단을  신설한다.
BOOL cltCityHallManager::CreateCityHall(SI32 siunique)
{
	if(pclStruct[siunique] == NULL)
	{
		pclStruct[siunique] = new cltCityHall(siunique);
		return TRUE;
	}
	return FALSE;
}

// Chief 후보를 등록한다. 
BOOL cltCityHallManager::SetCandidate(SI32 index, cltSimpleRank* pclrank, cltSimplePerson* pclcandidate)
{
	if(pclcandidate == NULL) return FALSE;

	SI32 ref = pclrank->GetVillageUnique();
	if(ref >=0 && pclStruct[ref])
	{
		return pclStruct[ref]->SetCandidate(index, pclrank, pclcandidate);
	}

	return FALSE;
}

// 후보 등록을 취소한다. (모든 직책에서..) 
BOOL cltCityHallManager::DeleteCandidate(SI32 villageunique, SI32 personid)
{
	if(personid == 0) return FALSE;
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return FALSE;

	return pclStruct[villageunique]->DeleteCandidate(personid);
}

// 특정 신분의 모든 후보 등록을 취소한다.
BOOL cltCityHallManager::DeleteCandidate(cltSimpleRank* pclrank)
{
	if(pclrank == 0) return FALSE;
	return pclStruct[pclrank->siVillageUnique]->DeleteCandidate(pclrank);
}

void cltCityHallManager::OnTimer()
{

}


// 주기적인 처리 
void cltCityHallManager::Action(cltTime* pcltime)
{
	int i = 0;
	cltStrInfo *pclStrInfo = NULL;
	cltHiredNPC *pclHiredNPC = NULL;
	cltHiredNPC *pclEventHiredNPC = NULL;
	cltCityHall *pclCityHall = NULL;
	
	cltServer *pclServer = (cltServer *)pclClient;

	if(pcltime == NULL) return;

	// DB 초기화 완료될때까지 기다림
	if( ((cltServer *)pclClient)->bCompleteDBInitAll == FALSE ) return;

	bool bdaychangedswitch = pcltime->IsDayChanged();

	if( bdaychangedswitch )
	{
		for( i = 1; i < MAX_VILLAGE_NUMBER; ++i ) 
		{
			// [영훈] 초보자 마을 : 초보자 마을 운영관련 로직을 행하지 않는다
			if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
			{
				if ( i == Const_Beginner_Village)	{									continue;		}
			}

			pclCityHall = (cltCityHall *)pclStruct[ i ];	if( pclCityHall == NULL )	continue;
			pclStrInfo = GetStrInfo( i );					if( pclStrInfo == NULL )	continue;

			// 마을 상태에 따라 체크한다.
			switch( GetCityHallState( i ) )
			{
			case CITYHALL_BEING_BANKRUPT:	// 마을 파산 중
				{
					// 마을 파산이 되어 모든 주식에 대한 처리가 완료되었다면
					if( pclStrInfo->clCityhallStrInfo.siTotalStockAmount == 0 )
					{
						pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_PROCESSING;

						// 마을 상단 상태 정보를 DB에 업데이트 한다. => 초기화 완료됨
						sDBRequest_CityHall_State_Changed clMsg( i, CITYHALL_BEING_BANKRUPT, CITYHALL_BEING_INITIALIZED );
						((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
				break;

			case CITYHALL_BEING_INITIALIZED:// 마을 초기화 상태 중
				{
					if( pclStrInfo->clCityhallStrInfo.siTotalStockAmount == 0 )
					{
						pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_PROCESSING;

						// 마을 상단 상태 정보를 DB에 업데이트 한다. => 안정기 됨
						sDBRequest_CityHall_State_Changed clMsg( i, CITYHALL_BEING_INITIALIZED, CITYHALL_BEING_STABLE );
						((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
				break;
			case CITYHALL_BEING_STABLE:		// 마을 안정기, 안정기에서부터는 주식 공모 및 주식 소유가 가능하다.
				{
					// 대행수가 임명되었는가
					cltRank* pclrank = ((cltServer *)pclClient)->pclRankManager->GetRank(RANKTYPE_CITYHALL, i);

					if(pclrank == NULL || pclrank->clPerson.siPersonID == 0  ) // 대행수가 없으면
						break;

					cltDate cldate;
					cldate.Set( &pclStrInfo->clCityhallStrInfo.clChiefDate );

					cldate.uiYear = cldate.uiYear + CITYHALL_STABLE_YEARS_AFTER_CHIEF_ELECTION;	// 연도로 따짐

					// 안정기(대행수 취임 후 1년)가 다 지나면
					if( cldate.GetDateVary() < ((cltServer *)pclClient)->pclTime->clDate.GetDateVary() ) 
					{
						pclStrInfo->clCityhallStrInfo.usCurStatus = CITYHALL_BEING_PROCESSING;

						// 마을 상단 상태 정보를 DB에 업데이트 한다. => 정상기 됨
						sDBRequest_CityHall_State_Changed clMsg( i, CITYHALL_BEING_STABLE, CITYHALL_BEING_NORMAL );
						((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}
				}
				break;
			case CITYHALL_BEING_NORMAL:		// 마을 정상기
				{
					// 대행수 취임 중이라면
					// 배열로 잡혀있기 때문에 NULL을 기대할 수 없다.
					cltRank*		pclCityHallRank = pclServer->pclRankManager->GetRank(RANKTYPE_CITYHALL, i);
					SI32			siCityHallRankPersonID = pclCityHallRank->GetPersonID();

					// 영주취임일이 없다는 건 영주가 없다는 이야기가 되므로 처리하지 않는다.
					if(pclStrInfo->clCityhallStrInfo.clChiefDate.GetDateVary() == 0 )			break;

					if(pclCityHallRank != NULL && siCityHallRankPersonID > 0 ) // 대행수가 취임 중이라면
					{
						cltDate cldate;
						cldate.Set( &pclStrInfo->clCityhallStrInfo.clChiefDate );

						cldate.uiYear = cldate.uiYear + 2;		// 영주 취임후 2년이 지났으면...						
						if( cldate.GetDateVary() < ((cltServer *)pclClient)->pclTime->clDate.GetDateVary() )
						{
							bool bCityHallRank_Resign = true;
#ifdef _DEBUG
							// ======================== 디버그 버젼 =================================
							// 영주가 운영자권한은 가진 캐릭터라면 자동으로 해임시키지 않는다.							
							// 영주 운영자 권한 체크는 캐릭터 이름이 "NDS"으로 시작하는 캐릭터 이다.
							if( _tcsncmp(pclCityHallRank->GetName(), "NDS", 3 ) == 0 )
							{
								bCityHallRank_Resign = false;
							}
							// ======================================================================
#endif
							if( bCityHallRank_Resign )
							{
								cltSimpleRank clrank( RANKTYPE_CITYHALL, i);

								// Rank정보에서 대상자를 삭제한다. 
								sDBRequest_Resign clMsg( 0, siCityHallRankPersonID, &clrank );
								pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
							}
						}
					}
					else // 대행수가 사임 중이라면
					{
						cltDate cldate;

						//============================================================================
						// 마을 정상기 기간에 마지막 영주가 해임된지 6개월이 지나면 파산 경고를 한다.
						//============================================================================
						cldate.Set( &pclStrInfo->clCityhallStrInfo.clResignDate );				
						cldate.AddMonth( CITYHALL_BANKRUPTCY_NOTIFY_AFTER_MONTHS );	// AddMonth 함수를 쓸 것

						// Letter로 한번만 실시간으로 공지함
                        if( cldate.GetDateVary() == ((cltServer *)pclClient)->pclTime->clDate.GetDateVary() ) 
						{
							// 파산 후 대행수 선출 없고, 주주총회 발의 안했을 때 6개월이면
							// 특정 주주 들에게 알림
							// 해당 주주들의 PersonID를 DB에 요청한다. => Letter Msg로 알림
							sDBRequest_Notify_Cityhall_Bankruptcy clMsg( i, cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTCY_ALERT );
							((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
						}

						//============================================================================
						// 마을 정상기 기간에 마지막 영주가 해임된지 1년이 지나면 마을 파산시킨다.
						//============================================================================
						cldate.Set( &pclStrInfo->clCityhallStrInfo.clResignDate );
						cldate.uiYear = cldate.uiYear + CITYHALL_BANKRUPTCY_AFTER_YEARS;
						
						if( cldate.GetDateVary() <= ((cltServer *)pclClient)->pclTime->clDate.GetDateVary() ) 
						{
							// 주주총회가 없을 때만 파산으로 감
							if( pclCityHall->clGeneralMeetingInfo.siMatterType == 0 )
							{
								// 마을 상단 상태 정보를 DB에 업데이트 한다. => 파산 됨
								sDBRequest_CityHall_State_Changed clMsg( i, CITYHALL_BEING_NORMAL, CITYHALL_BEING_BANKRUPT );
								((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
							}
						}
					}
				}
				break;
			} // switch( GetCityHallState( i ) ) 종료

			// [진성] 주주총회가 진행 중이면 => 2008-7-21
			if( pclCityHall->clGeneralMeetingInfo.siMatterType > 0 ) 
			{
				cltDate cldate;
				cldate.Set( &((cltServer *)pclClient)->pclTime->clDate );

				// [진성] 주총 발의 기간이면.
				if( ( pclCityHall->clGeneralMeetingInfo.clStartDate.GetDateVary() <= cldate.GetDateVary() ) &&
					( pclCityHall->clGeneralMeetingInfo.clCloseDate.GetDateVary() >  cldate.GetDateVary() )		)
				{
					cltDate clAcceptClosedate;	// 동의 종료 시간.				
					clAcceptClosedate.Set(&pclCityHall->clGeneralMeetingInfo.clStartDate);
					clAcceptClosedate.AddMonth(1);

					// 주총 발의 기간으로 부터 1달이 지났다면.
					if(clAcceptClosedate.GetDateVary() < cldate.GetDateVary() )
					{
						// 후보자가 동의하지 않음 - 1달안에 후보자가 동의하지 않으면 주총 종료
						if( pclCityHall->clGeneralMeetingInfo.siAcceptCandidate <= 0 )
						{
							sDBRequest_GeneralMeeting_CloseChief sendMsg( i, 0, &((cltServer *)pclClient)->pclTime->clDate );
							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
						}
					}
				}
				// 주총 발의 시간(주총발의 후 3개월)에서 종료 시간 사이의 시간이 아니면.
				else 
				{
					sDBRequest_GeneralMeeting_CloseChief sendMsg( i, pclCityHall->clGeneralMeetingInfo.GetResult(), &((cltServer *)pclClient)->pclTime->clDate );
					((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
				}
			} 

		} // if ( pclStrInfo ) 종료

		// 한달에 한번 1일에 행해지는 마을 행동
		if( pcltime->clDate.uiDay == 1 ) 
		{
			cltDate clDate;
			clDate.Set( &((cltServer *)pclClient)->pclTime->clDate );
			if ( clDate.uiYear > 2 )
			{
				clDate.uiYear--;
				for ( i = 1; i < MAX_VILLAGE_NUMBER; ++i ) 
				{
					// [영훈] 초보자 마을 : 초보자 마을 운영관련 로직을 행하지 않는다
					if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
					{
						if ( i == Const_Beginner_Village)			{		continue;		}
					}

					pclStrInfo = GetStrInfo( i );
					if( pclStrInfo ) 
					{
						// 1년동안 로그인 하지 않은 주민은 탈퇴시킨다.
						sDBRequest_ManyResidentOut sendMsg( i, &clDate );
						((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
					}
				} // for ( i = 1; i < MAX_VILLAGE_NUMBER; ++i ) 종료
			}
		} // if( pcltime->clDate.uiDay == 1 ) 종료
		
		// NPC 고용취소 - 하루에 한번 체크
		for( SI32 villagenum = 1; villagenum < MAX_VILLAGE_NUMBER; ++villagenum ) 
		{
			// [영훈] 초보자 마을 : 초보자 마을 운영관련 로직을 행하지 않는다
			if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
			{
				if ( villagenum == Const_Beginner_Village)		{		continue;		}
			}

			pclStrInfo = GetStrInfo( villagenum );
			if ( pclStrInfo ) 
			{
				// 일반적인 NPC는 1년이 지나면 고용이 취소된다.
				for ( SI32  npcNum = 0; npcNum < MAX_HIRED_NPC_NUM; ++npcNum ) 
				{
					pclHiredNPC = &pclStrInfo->clCityhallStrInfo.clHiredNPC[ npcNum ];
					if( pclHiredNPC->siNPCKind > 0 ) 
					{
						// 고용 1년이 지났으면 고용 해지
						SI32 hiredate = pclHiredNPC->clHireDate.GetDateVary();
						SI32 currdate = ((cltServer *)pclClient)->pclTime->clDate.GetDateVary();

						if( currdate - hiredate > 360 ) {

							sDBRequest_HireNPC clMsg( 0, villagenum, npcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIRENPC );
							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
						}
					} // if( pclHiredNPC->siNPCKind > 0 ) 종료
				} // for( SI32  npcNum = 0; npcNum < MAX_HIRED_NPC_NUM; ++npcNum ) 종료

				// EVENTNPC는 일반적으로 1년이 지나면 고용이 취소된다.
				// NPC 별로 고용해지가 틀릴 수도 있다.
				for( SI32 eventnpcNum= 0; eventnpcNum < MAX_HIRED_EVENTNPC_NUM; ++eventnpcNum ) 
				{
					pclEventHiredNPC = &pclStrInfo->clCityhallStrInfo.clHiredEventNPC[ eventnpcNum ];
					if( pclClient->pclKindInfoSet->IsValidKind(pclEventHiredNPC->siNPCKind) == false )		continue;

	/*					SYSTEMTIME stevent, enevent;
						ZeroMemory(&stevent, sizeof(stevent));
						ZeroMemory(&enevent, sizeof(enevent));

						stevent.wYear = 2007;	stevent.wMonth = 12;	stevent.wDay = 1;
						enevent.wYear = 2007;	enevent.wMonth = 12;	enevent.wDay = 26;		enevent.wHour = 11;
						if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
						{
							enevent.wYear = 2007;	enevent.wMonth = 12;	enevent.wDay = 27;		enevent.wHour = 6;
						}
						else if(pclClient->IsWhereServiceArea(ConstServiceArea_English))
						{
							enevent.wYear = 2007;	enevent.wMonth = 12;	enevent.wDay = 27;		enevent.wHour = 6;
						}
						else if(pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina))
						{
							enevent.wYear = 2007;	enevent.wMonth = 12;	enevent.wDay = 27;		enevent.wHour = 6;
						}*/

						// 크리스마스 트리는 이벤트 기간 이후에는 사라진다.
						/*if( pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_PINE_TREE")) ||
							pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE")) )
						{
							if( TimeInBounds( &pclServer->sTime, &stevent, &enevent ) == false )
							{
								sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );

								((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
							}
						}
						//[추가 : 황진성 2007. 12. 11 신년 트리 기간이 지나면 죽어.]
						else if( pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_KADOMACH")) || 
									pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"))	)
						{
							stevent.wYear = 2007;	stevent.wMonth = 12;	stevent.wDay = 26;
							enevent.wYear = 2008;	enevent.wMonth =  1;	enevent.wDay = 10;		enevent.wHour = 11;

							if(pclClient->IsWhereServiceArea(ConstServiceArea_English))
							{
								stevent.wYear = 2007;	stevent.wMonth = 12;	stevent.wDay = 27;	stevent.wHour = 7;
							}
							else if(pclClient->IsWhereServiceArea(ConstServiceArea_NHNChina))
							{
								stevent.wYear = 2007;	stevent.wMonth = 12;	stevent.wDay = 27;	stevent.wHour = 7;
							}

							if(false == TimeInBounds( &pclServer->sTime, &stevent, &enevent ))
							{
								sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );

								((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
							}
						}*/
						//[진성] 영웅의 동상 이벤트 기간 후 제거. => 2008-6-11//
					if( pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUEBASE"))  
						|| pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ROULETTESTATUE"))	)
					{
						if(false == ((cltServer *)pclClient)->pclEventTimeManager->InEventTime("Hero_Bronze", &pclServer->sTime) )
						{
							sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );

							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
						}
					}
					//[성웅] 할로윈 촛대 이벤트 기간후 제거 //Switch_HalloweenEvent
					else if( pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEENBASE"))  
							 || pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HALLOWEEN"))	)
					{
						if(false == ((cltServer *)pclClient)->pclEventTimeManager->InEventTime("HalloweenEvent", &pclServer->sTime) )
						{
							sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );

							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
						}
					}
					//KHY - 1113 - 크리스마스 이벤트 -  크리스 마스 나무  기간후 제거.
					else if(    pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREEBASE"))  
							 || pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHRISTMASTREE1"))	)
					{
						if(false == ((cltServer *)pclClient)->pclEventTimeManager->InEventTime("ChristmasEvent", &pclServer->sTime) )
						{
							sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );

							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
						}
					}
					// 춘기 - 신년이벤트 - 신년나무 기간후 제거
					else if( pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREEBASE"))  
						|| pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_NEWYEARTREE"))	)
					{
						if(false == ((cltServer *)pclClient)->pclEventTimeManager->InEventTime("NewYearEvent", &pclServer->sTime) )
						{
							sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );

							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
						}
					}
					// 춘기 - 히나마츠리 - 히나단 기간후 제거
					else if( pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HINADOLL")))
					{
						if(false == ((cltServer *)pclClient)->pclEventTimeManager->InEventTime("Hinamatsuri", &pclServer->sTime) )
						{
							sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );

							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
						}
					}
					else
					{
						SI32 hiredate = pclEventHiredNPC->clHireDate.GetDateVary();
						SI32 currdate = ((cltServer *)pclClient)->pclTime->clDate.GetDateVary();

						// 기본적으로 이벤트 NPC는 1년이 지난뒤에 고용이 취소된다.
						SI32 siMaxHireDate = 360;

						// KIND_RETURNTOWER 이벤트 NPC는 2년이 지난뒤에 고용이 취소된다.
						if( pclEventHiredNPC->siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_RETURNTOWER")) )
							siMaxHireDate = 720;

						if( currdate - hiredate > siMaxHireDate ) 
						{
							sDBRequest_HireNPC clMsg( 0, villagenum, eventnpcNum, 0, 0, 0, &((cltServer *)pclClient)->pclTime->clDate, HIRENPC_HIREEVENTNPC );
							((cltServer *)pclClient)->pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg );
						}
					}
				} // for( SI32 eventnpcNum= 0; eventnpcNum < MAX_HIRED_EVENTNPC_NUM; ++eventnpcNum ) 종료
			} // for villagenum 종료
		} 
		
		if(	((cltServer*)pclClient)->bForceTimeSwitch == false )
		{
			WriteAllCityHallState();
		}
	}// if( bdaychangedswitch )
}

BOOL cltCityHallManager::IsGeneralMeeting( SI32 villageunique )
{
	cltCityHall *pclCityHall = (cltCityHall *)pclStruct[ villageunique ];

	if( pclCityHall && pclCityHall->clGeneralMeetingInfo.siMatterType > 0 ) {
		return TRUE;
	}

	return FALSE;
}

BOOL cltCityHallManager::IsPrepareBankruptcy( SI32 villageunique )
{
	cltCityHall *pclCityHall = (cltCityHall *)pclStruct[ villageunique ];

	if( pclCityHall && pclCityHall->bPrepareBankruptcy ) 
	{
		return TRUE;
	}

	return FALSE;
}

//#if defined(_CITYHALL_BANKRUPTCY)
// 상단의 파산 상태를 얻음
UI08 cltCityHallManager::GetCityHallState( SI32 villageunique )
{
	cltStrInfo* pStrInfo = GetStrInfo( villageunique );

	if( pStrInfo == NULL || villageunique < 1 ) 
		return CITYHALL_UNDEFINED_STATE;
  //  if( pStrInfo->clCityhallStrInfo == NULL )
		//return CITYHALL_UNDEFINED_STATE;	
	
	return pStrInfo->clCityhallStrInfo.usCurStatus;
}

// 파산한 후 마을의 모든 요소가 초기화 되었는가
BOOL cltCityHallManager::IsInitialized( SI32 villageunique )
{
	cltStrInfo* pStrInfo = GetStrInfo( villageunique );

	if( pStrInfo == NULL ) return FALSE;
	if( pStrInfo->clCityhallStrInfo.usCurStatus == CITYHALL_BEING_BANKRUPT
		&& pStrInfo->clCityhallStrInfo.siTotalStockAmount == 0 )
		return TRUE;

	return FALSE;

    // 마을 트리 분리 초기화
	// ...

	// 주식 초기화
	// ...

	// NPC 초기화
	// ...

	// 마을자금 초기화
	// ...

	// 마을지도 초기화
	// ...

	// 기본건물[내구도] 초기화
	// ...

	// 성벽 초기화
	// ...

	// 성황신 초기화
	// ...

	// ... ?
}

// 관청 파산까지 혹은 안정기가 끝나는 DeadLine 정보를 얻는다.
cltDate  cltCityHallManager::GetCityHallBankruptcyDetailInfo( cltCityhallStrInfo* pstrinfo )
{
	cltDate cldate;
	cltDate remaineddate;	// 남은 시간
	memset(&cldate, 0, sizeof(cltDate) );
	memset(&remaineddate, 0, sizeof(cltDate) );

	if( pclClient->GameMode != GAMEMODE_CLIENT ) return remaineddate;
	if( pstrinfo == NULL ) return remaineddate;

    switch( pstrinfo->usCurStatus )
	{
	case CITYHALL_BEING_NORMAL:			// 정상기
		{
			cldate.Set( &pstrinfo->clResignDate );

			cldate.uiYear = cldate.uiYear + CITYHALL_BANKRUPTCY_AFTER_YEARS;
			
			// 대행수 사임 후 1년이 지나고 대행수 선출이 없을 때 마을 파산 선고를 
			// 할 남은 시간 구함
			if( cldate.GetDateVary() >= ((cltClient*)pclClient)->clClientDate.GetDateVary() ) 
			{
				remaineddate.DiffDate( &cldate, &((cltClient*)pclClient)->clClientDate );
			}
		}
		break;
	case CITYHALL_BEING_BANKRUPT:		// 파산기
		{

		}
		break;
	case CITYHALL_BEING_INITIALIZED:	// 초기화기
		{

		}
		break;
	case CITYHALL_BEING_STABLE:			// 안정기
		{
			cldate.Set( &pstrinfo->clChiefDate );

			cldate.uiYear = cldate.uiYear + CITYHALL_STABLE_YEARS_AFTER_CHIEF_ELECTION;	// 연도로 따짐

			if( cldate.GetDateVary() >= ((cltClient*)pclClient)->clClientDate.GetDateVary() ) 
			{
				remaineddate.DiffDate( &cldate, &((cltClient*)pclClient)->clClientDate );
			}
		}
		break;
	default :							// 알 수 없음
		return remaineddate;
	}

	return remaineddate;
}

void cltCityHallManager::WriteAllCityHallState()
{
	SI32 i;

	cltStrInfo *pclStrInfo = NULL;
	cltCityHall *pclCityHall = NULL;

	FILE *fp = _tfopen( TEXT("Config/CityHallState.log"), TEXT("wt") );

	if ( fp == NULL )
	{
		return;
	}

	// 맨 첫줄. 
	// Name : 마을명, State : 마을 상태, Chief : 대행수 유무, Schedule : 남은 파산 예정일
	// GeneralMeeting : 주주총회 중인지 여부, TotalStock : 총주식수
	_ftprintf( fp, TEXT("%s\t\t\t%s\t%s\t%s\t%s\t%s\n"), TEXT("Name"), TEXT("State"), TEXT("DeadLine"), TEXT("Chief"), TEXT("GeneralMeeting"), TEXT("TotalStock"));

	for (i = 0; i < MAX_VILLAGE_NUMBER; ++i )
	{
		pclCityHall = (cltCityHall *)pclStruct[ i ];

		if( pclCityHall == NULL ) continue;	

		pclStrInfo = GetStrInfo( i );
		if( pclStrInfo == NULL) continue;

		TCHAR* pname = pclClient->pclVillageManager->GetName( i );
		if(pname == NULL) return ;

		UI08 uiState = GetCityHallState( i );

		TCHAR* pChief = NULL;
		TCHAR* pGeneralMeeting = NULL;
		TCHAR* pStatename = NULL;
		TCHAR* pSchedule = NULL;

		TCHAR deadline[256];
		memset(deadline, 0, sizeof(deadline));

		// 대행수 여부
		cltRank* pclrank = ((cltServer *)pclClient)->pclRankManager->GetRank(RANKTYPE_CITYHALL, i);

		if(pclrank == NULL || pclrank->clPerson.siPersonID == 0  )
			pChief = TEXT("X");
		else
			pChief = TEXT("O");

		// 주주총회 여부
		if( pclCityHall->clGeneralMeetingInfo.siMatterType == 0 )
			pGeneralMeeting = TEXT("X");
		else
			pGeneralMeeting = TEXT("O");

		StringCchPrintf( deadline, 256, GetTxtFromMgr(1644), 0, 0, 0 );

		switch( uiState )
		{
		case 	CITYHALL_BEING_BANKRUPT	:		// 파산기
			{
				pStatename = GetTxtFromMgr(5240);

				StringCchPrintf( deadline, 256, TEXT("%s%s"), GetTxtFromMgr(5241), GetTxtFromMgr(1223)); 	// "초기화 필요"
			}
			break;
		case 	CITYHALL_BEING_INITIALIZED	:	// 초기화기
			{
				pStatename = GetTxtFromMgr(5241);
			}
			break;
		case 	CITYHALL_BEING_STABLE		:	// 안정기
			{
				pStatename = GetTxtFromMgr(5242);

				if(pclrank == NULL || pclrank->clPerson.siPersonID == 0  ) // 대행수가 없으면
					break;
				
				// 대행수 취임일이 정상적이고 취임일 >= 사임일이어야함, 같은 날 일 수 있음
				//if( pclStrInfo->clCityhallStrInfo.clChiefDate.GetDateVary() >= pclStrInfo->clCityhallStrInfo.clResignDate.GetDateVary() )
				{
					cltDate cldate;			// deadlline date
					cltDate remaineddate;	// 남은 시간
					cldate.Set( 0, 0, 0, 0, 0);
					remaineddate.Set(0, 0, 0, 0, 0);
					pSchedule = GetTxtFromMgr(1644);	// %d년 %d월 %d일

					cldate.Set( &pclStrInfo->clCityhallStrInfo.clChiefDate );

					cldate.uiYear = cldate.uiYear + CITYHALL_STABLE_YEARS_AFTER_CHIEF_ELECTION;	// 연도로 따짐

					if( cldate.GetDateVary() >= ((cltServer *)pclClient)->pclTime->clDate.GetDateVary() ) 
					{
						remaineddate.DiffDate( &cldate, &(((cltServer *)pclClient)->pclTime->clDate) );
					}
					
					StringCchPrintf( deadline, 256, pSchedule, remaineddate.uiYear, remaineddate.uiMonth, remaineddate.uiDay );
				}

			}
			break;
		case 	CITYHALL_BEING_NORMAL		:	// 정상기
			{
				pStatename = GetTxtFromMgr(5239);

				//if(pclrank == NULL || pclrank->clPerson.siPersonID == 0  ) // 대행수가 없으면
				{
					cltDate cldate;
					cltDate remaineddate;	// 남은 시간
					cldate.Set(0, 0, 0, 0, 0 );
					remaineddate.Set(0, 0, 0, 0, 0);
					pSchedule = GetTxtFromMgr(1644);	// %d년 %d월 %d일

					//cldate.Set( &pclStrInfo->clCityhallStrInfo.clResignDate );
					//// AddMonth 함수를 쓸 것
					//cldate.AddMonth( CITYHALL_BANKRUPTCY_NOTIFY_AFTER_MONTHS );

					// ==================================================
					cldate.Set( &pclStrInfo->clCityhallStrInfo.clResignDate );

					cldate.uiYear = cldate.uiYear + CITYHALL_BANKRUPTCY_AFTER_YEARS;
					
					// 대행수 사임 후 1년이 지나고 대행수 선출이 없을 때 마을 파산 선고를 할
					// 남은 시간 구함
					if( cldate.GetDateVary() >= ((cltServer *)pclClient)->pclTime->clDate.GetDateVary() ) 
					{
						remaineddate.DiffDate( &cldate, &(((cltServer *)pclClient)->pclTime->clDate) );
					}

					StringCchPrintf( deadline, 256, pSchedule, remaineddate.uiYear, remaineddate.uiMonth, remaineddate.uiDay );
				}
			}
			break;
		default:
			{
				pStatename = TEXT("Error");			// 오류
			}
			break;
		}

		SI32 siTotalStockAmount = pclStrInfo->clCityhallStrInfo.siTotalStockAmount;

		// TEXT("Name"), TEXT("State"), TEXT("DeadLine"), TEXT("Chief"), TEXT("GeneralMeeting"), TEXT("TotalStock")

		_ftprintf( fp, TEXT("%s\t\t\t%s\t%s\t%s\t%s\t%d\n"), 
			pname,
			pStatename,
			deadline,
			pChief,
			pGeneralMeeting,
			siTotalStockAmount            
			);
	}
	
	fclose( fp );

	return;
}

//#endif
