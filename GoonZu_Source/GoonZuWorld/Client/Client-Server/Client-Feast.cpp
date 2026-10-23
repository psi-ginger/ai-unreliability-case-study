/* ==========================================================================
	설명   :		내자시 관련 Client 메세지 함수

	작성일 :		05/06/03
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(_LEON_FEAST_EFFECT)

#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-Feast.h"
#include "../FeastEffect/FeastEffectManager.h"
//#include "..\CommonLogic\ \ .h"

#include "Char\CharManager\CharManager.h"
#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "../Client/ninterface/NPartyhallDlg/DLG_PartyHall_In_Invite_Chief.h"
#include "../Client/ninterface/NPartyhallDlg/DLG_PartyHall_In_Base.h"
#include "../Client/NStructureDlg/Feast/NFeastDlg.h"

extern cltCommonLogic* pclClient;

// 연회자의 실행으로 인한 연회시 폭죽 뿌려주기!!
void cltClient::DoMsg_GAMEMSG_RESPONSE_FEAST_USEEFFECT(cltMsg *pclMsg,SI32 id)
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGameMsgResponse_FeastUseEffect* pclInfo = (cltGameMsgResponse_FeastUseEffect*)pclMsg->cData;

#ifdef _SAFE_MEMORY
#else
	if( pclInfo->siEffectNum == NULL ) return;
#endif

	pclclient->m_pFeastEffectManager->StopFeastEffect();

	for(SI16 i=0; i<cltGameMsgRequest_FeastUseEffect::MAX_EFFECT_NUM_AT_ONCE; i++)
	{
		if( pclInfo->siEffectNum[ i ] < 0 || pclInfo->siEffectNum[ i ] >= cltFeastManager::MAX_FEAST_EFFECT_NUM )
			continue;

		pclclient->m_pFeastEffectManager->SetAction( pclInfo->siEffectNum[ i ], pclInfo->siEffectStartPosX, pclInfo->siEffectStartPosY );
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_GET_COSTUME( cltMsg* pclMsg, SI32 id )
{	




}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_PartyHall_Entrance_Application* pclInfo = (cltGameMsgResponse_PartyHall_Entrance_Application*)pclMsg->cData;

	if( NULL == m_pDialog[ PARTYHALL_IN_INVITE_CHIEF_DLG ] )
		CreateInterface( PARTYHALL_IN_INVITE_CHIEF_DLG );

	
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ENTRANCE_APPLICATION_LIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_PartyHall_Entrance_Application_List* pclInfo = (cltGameMsgResponse_PartyHall_Entrance_Application_List*)pclMsg->cData;

	if( NULL == m_pDialog[ PARTYHALL_IN_BASE_DLG ] )
		CreateInterface( PARTYHALL_IN_BASE_DLG );
	
	((NPartyHall_In_BaseDlg*)m_pDialog[ PARTYHALL_IN_BASE_DLG ])->m_NPartyHall_In_Firecracker_Chief_Dlg.SetEntranceApplicationList( pclInfo->m_ApplicationArray );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ADD_DECORATION( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_PartyHall_Add_Decoration* pclInfo = (cltGameMsgResponse_PartyHall_Add_Decoration*)pclMsg->cData;
	
	PartyHall_Decoration* pDecorationList = &pclClient->pclFeastManager->clDecoration[0];

	pDecorationList[pclInfo->m_siDecorationSlot].m_DacorationKind = pclInfo->m_siDecorationKind;
	pDecorationList[pclInfo->m_siDecorationSlot].m_DacorationNum  = pclInfo->m_siDecorationNum;

	
	// 장식이 추가된 정보 보이기.	
	//???

	if( m_pDialog[ NFEAST_DLG ] )
	{
		NPartyHallChiefDlg* pPartyHallChiefDlg = (NPartyHallChiefDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ]->GetChildDialog(5));
		pPartyHallChiefDlg->SetChangeItemMaterialInfo(pclInfo->m_DeleteItem);
		pPartyHallChiefDlg->ShowDecorationList();
		
		NPartyHallChief2Dlg* pPartyHallChief2Dlg = (NPartyHallChief2Dlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ]->GetChildDialog(4));
		pPartyHallChief2Dlg->LoadFeastStg();

		NPartyHallReservationDlg* pPartyHallReservationDlg = (NPartyHallReservationDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ]->GetChildDialog(2));
		pPartyHallReservationDlg->SetDecorationList();
	}

}
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_TOTAL_INFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyHall_Total_Info* pclinfo = (cltGameMsgResponse_PartyHall_Total_Info*)pclMsg->cData;

	SI32 OnePageNum = cltGameMsgResponse_PartyHall_Total_Info::ONE_PAGE_NUM;
	SI32 ArrayNum	= cltGameMsgResponse_PartyHall_Total_Info::ONE_PAGE_NUM * pclinfo->m_Page;

	if( MAX_PARTYHALL_MAP <= pclinfo->m_PartyHallIndex )
		return;

	for(SI32 Count = 0; Count < OnePageNum; ++Count) 
	{
		if( MAX_PARTYHALL_RESERVATION_INFO_LIST <= Count + ArrayNum )
			break;
		pclClient->pclFeastManager->clPartyHallResvInfo[pclinfo->m_PartyHallIndex][Count + ArrayNum] 
		
		= pclinfo->m_clPartyHallResvInfoList[Count];
	}

		
	//if( 0 == pclinfo->m_PartyHallIndex &&
	//	pclinfo->m_Page == cltGameMsgResponse_PartyHall_Total_Info::PAGE_NUM - 1 )
	//{
	//	if( m_pDialog[ NFEAST_DLG ] )
	//	{
	//		NPartyHallInfoDlg* pPartyHallInfoDlg = (NPartyHallInfoDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ]->GetChildDialog(0));
	//		pPartyHallInfoDlg->SetPartyInfo();

	//		NPartyHallReservationDlg* pPartyHallReservationDlg = (NPartyHallReservationDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ]->GetChildDialog(2));
	//		pPartyHallReservationDlg->SetReservationList();

	//		NPartyHallPresentDlg* pPartyHallPresentDlg = (NPartyHallPresentDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ]->GetChildDialog(1));
	//		//pPartyHallPresentDlg->SetMyInventory();
	//		pPartyHallPresentDlg->SetPresentItem();
	//	}
	//}
}

// 파티장 예약.			=> 2008. 3. 15
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_RESERVATION(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyHall_Reservation* pclinfo = (cltGameMsgResponse_PartyHall_Reservation*)pclMsg->cData;
	
	//SI32 ResvIndex = pclClient->pclFeastManager->GetReveIndex(pclinfo->m_siPartyHallIndex, pclinfo->m_siPersonID);
	
	cltPatyHallResvInfo* pPatyHallResvInfo = (cltPatyHallResvInfo*)&pclClient->pclFeastManager->clPartyHallResvInfo[pclinfo->m_siPartyHallIndex][pclinfo->m_siHour];
	
	pPatyHallResvInfo->m_siPersonID			=	pclinfo->m_siPersonID;						
	pPatyHallResvInfo->m_SystemTime.wYear	=	pclinfo->m_siYear;									
	pPatyHallResvInfo->m_SystemTime.wMonth	=	pclinfo->m_siMonth;								
	pPatyHallResvInfo->m_SystemTime.wDay	=	pclinfo->m_siDay;									
	pPatyHallResvInfo->m_SystemTime.wHour	=	pclinfo->m_siHour;									
	pPatyHallResvInfo->m_SystemTime.wMinute	=	pclinfo->m_siMinute;								
	pPatyHallResvInfo->m_siEntranceType		=	pclinfo->m_siEntranceType;					
	pPatyHallResvInfo->m_siDecorationKind	=	pclinfo->m_siDecorationKind;					
	pPatyHallResvInfo->m_siValueType		=	pclinfo->m_siValueType;				

	memcpy(pPatyHallResvInfo->m_szPersonName,  pclinfo->m_szPersonName,		sizeof(pPatyHallResvInfo->m_szPersonName));
	memcpy(pPatyHallResvInfo->m_szReason,	   pclinfo->m_szReason,			sizeof(pPatyHallResvInfo->m_szReason));

	pclCM->CR[1]->pclCI->clIP.SetMoney(pclinfo->m_siUserMoney);


	if( m_pDialog[ NFEAST_DLG ] )
	{
		NPartyHallReservationDlg* pPartyHallReservationDlg = (NPartyHallReservationDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ]->GetChildDialog(2));
		pPartyHallReservationDlg->SetReservationList();
		pPartyHallReservationDlg->SetReservationSuccess();
	}

}
// 파티장 선물 등록.	=> 2008. 3. 15
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_ADD_PRESENT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyHall_Add_Present* pclInfo = (cltGameMsgResponse_PartyHall_Add_Present*)pclMsg->cData;

	SI32 ResvIndex = pclClient->pclFeastManager->GetReveIndex(pclInfo->m_siPartyHallIndex, pclInfo->m_siPersonID);

	cltPatyHallResvInfo* pPatyHallResvInfo = &pclClient->pclFeastManager->clPartyHallResvInfo[pclInfo->m_siPartyHallIndex][ResvIndex];

	pPatyHallResvInfo->m_PresentItem[pclInfo->m_siItemSlotIndex].m_siItemUnique	= pclInfo->m_siItemUnique;
	pPatyHallResvInfo->m_PresentItem[pclInfo->m_siItemSlotIndex].m_siItemNum	= pclInfo->m_siItemNum;
	
	cltItem* pSelectItem = &pclCM->CR[1]->pclCI->clCharItem.clItem[pclInfo->m_siPresentItemInvenPos];
	pSelectItem->siItemNum -= pclInfo->m_siItemNum;
	if(0 == pSelectItem->siItemNum)
	{
		pSelectItem->Init();
	}

	if( m_pDialog[ NFEAST_DLG ] )
	{
		NPartyHallPresentDlg* pPartyHallPresentDlg = (NPartyHallPresentDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ])->GetChildDialog(1);
		pPartyHallPresentDlg->SetMyInventory();
		pPartyHallPresentDlg->SetPresentItem();
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_GIVEITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyHall_GiveItem* pclInfo = (cltGameMsgResponse_PartyHall_GiveItem*)pclMsg->cData;
	


}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_USEEFFECT(cltMsg* pclMsg, SI32 id)
{
	
	

}
// 장식물 리스트 주기.
void cltClient::DoMsg_GAMEMSG_RESPONSE_PARTYHALL_DECORATION_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PartyHall_Decoration_List* pclInfo = (cltGameMsgResponse_PartyHall_Decoration_List*)pclMsg->cData;
	

	PartyHall_Decoration* pDecorationList = &pclClient->pclFeastManager->clDecoration[0];
	for(SI32 Count = 0; Count < MAX_FEAST_STG_NUMBER; ++Count)
	{
		pDecorationList[Count].m_DacorationKind = pclInfo->m_clDecoration[Count].m_siDecorationKind;
		pDecorationList[Count].m_DacorationNum  = pclInfo->m_clDecoration[Count].m_siDecorationNum;
	}

	if( m_pDialog[ NFEAST_DLG ] )
	{
		NPartyHallChiefDlg* pPartyHallChiefDlg  = (NPartyHallChiefDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ])->GetChildDialog(5);
		pPartyHallChiefDlg->ShowDecorationList();

		NPartyHallInfoDlg* pPartyHallInfoDlg = (NPartyHallInfoDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ])->GetChildDialog(0);
		pPartyHallInfoDlg->SetPartyInfo();

		NPartyHallReservationDlg* pPartyHallReservationDlg = (NPartyHallReservationDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ])->GetChildDialog(2);
		pPartyHallReservationDlg->SetReservationList();
		pPartyHallReservationDlg->SetDecorationList();

		NPartyHallPresentDlg* pPartyHallPresentDlg = (NPartyHallPresentDlg*)((NFeastDlg*)m_pDialog[ NFEAST_DLG ])->GetChildDialog(1);
		//pPartyHallPresentDlg->SetMyInventory();
		pPartyHallPresentDlg->SetPresentItem();
	}

}


//#endif