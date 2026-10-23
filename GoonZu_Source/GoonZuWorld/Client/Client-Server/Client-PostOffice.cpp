#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"

#include "..\CommonLogic\Msg\MsgType-PostOffice.h"
#include "..\CommonLogic\PostOffice\PostOffice.h"

#include "Char\CharManager\CharManager.h"
#include "NInterface/NSaveItemListDlg/NSaveItemListDlg.h"
#include "ninterface/nmarketconditionsdlg/NMarketConditionsDlg.h"
#include "NStructureDlg/PostOffice/NPostOfficeGroupListDlg.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_RECVITEMLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PostOfficeRecvItemList *pclInfo = (cltGameMsgResponse_PostOfficeRecvItemList *)pclMsg->cData;

	if ( pclCM->CR[1]->GetCurrentVillageUnique() == pclInfo->siVillageUnique )
	{

		for( int i = 0; i < pclInfo->usCount; ++i ) {
			
			pclPostOfficeManager->AddItemToRecvList( &pclInfo->clRecvItemInfo[ i ] );
		}

		pclPostOfficeManager->m_bAddItemToRecvList = TRUE;

	}
	
	return;	
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_RECVMONEYLIST(cltMsg* pclMsg, SI32 id)
{

	cltGameMsgResponse_PostOfficeRecvMoneyList *pclInfo = (cltGameMsgResponse_PostOfficeRecvMoneyList *)pclMsg->cData;

	if ( pclCM->CR[1]->GetCurrentVillageUnique() == pclInfo->siVillageUnique )
	{

		for( int i = 0; i < pclInfo->usCount; ++i ) {
			
			pclPostOfficeManager->AddMoneyToRecvList( &pclInfo->clRecvMoneyInfo[ i ] );
		}

		pclPostOfficeManager->m_bAddMoneyToRecvList = TRUE;

	}
	
	return;	

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_RECVMSGLIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PostOfficeRecvMsgList *pclInfo = (cltGameMsgResponse_PostOfficeRecvMsgList *)pclMsg->cData;

	if ( pclCM->CR[1]->GetCurrentVillageUnique() == pclInfo->siVillageUnique )
	{

		for( int i = 0; i < pclInfo->usCount; ++i ) {
			
			pclPostOfficeManager->AddMsgToRecvList( &pclInfo->clRecvMsgInfo[ i ] );
		}

		pclPostOfficeManager->m_bAddMsgToRecvList = TRUE;

	}
	
	return;	


}

void cltClient::DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_GETRECORD(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PostOfficeGetRecord *pclInfo = (cltGameMsgResponse_PostOfficeGetRecord *)pclMsg->cData;

	for( int i = 0; i < pclInfo->usCount; ++i ) {
		
		pclPostOfficeManager->m_recordinfo[ i ] = pclInfo->recordinfo[ i ];
	}

	pclPostOfficeManager->m_recordcount = pclInfo->usCount;
	pclPostOfficeManager->bUpdateRecord = TRUE;

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GET_SAVEUSERITEM(cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_Get_SaveUserItem* pclInfo = (cltGameMsgResponse_Get_SaveUserItem*)pclMsg->cData;

	pclCM->CR[1]->pclCI->clIP.SetMoney(pclInfo->siUserMoney);
	cltMsg clMsg(GAMEMSG_REQUEST_GET_SAVEUSERITEMLIST, 0, NULL);
	SendMsgToServer((sPacketHeader*)&clMsg);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_GET_SAVEUSERITEMLIST(cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_Get_SaveUserItemList* pclInfo = (cltGameMsgResponse_Get_SaveUserItemList*)pclMsg->cData;

	if( m_pDialog[ NSAVEITEMLIST_DLG ] )
		((CNSaveItemListDlg*)m_pDialog[ NSAVEITEMLIST_DLG ])->ShowList(pclInfo->clSaveUsersItemInfo, pclInfo->clSaveUsersHorseInfo);

}

// [희영] 시세확인 2008-07-18
void cltClient::DoMsg_GAMEMSG_RESPONSE_MARKETCONDITIONS(cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_MarketConditions* pclInfo = (cltGameMsgResponse_MarketConditions*)pclMsg->cData;

	if( m_pDialog[ NMARKETCONDITIONS_DLG ] )
		((CNMarketConditionsDlg*)m_pDialog[ NMARKETCONDITIONS_DLG ])->ShowList(&pclInfo->clMarketConditionsInfo[0], &pclInfo->sTimeMarketConditionsInfo[0]);

}

//[진성] 단체 메시지 보내기. 단체로 보낼 유저리스트를 얻음. => 2008-8-4 
void cltClient::DoMsg_GAMEMSG_RESPONSE_POSTOFFICE_USER_LIST_GROUP(cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_PostOffice_User_List_Group* pclInfo = (cltGameMsgResponse_PostOffice_User_List_Group*)pclMsg->cData;

	if( m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG] )
		((NPostOfficeGroupListDlg*)m_pDialog[NPOSTOFFICE_GROUP_LIST_DLG])->AddRecvUserGroup( pclInfo );
}

//[진성]  확인된 유저의 이름과 personID를 얻음. => 2008-8-4 
void cltClient::DoMsg_GAMEMSG_RESPONSE_CHAR_NAME_CHECK(cltMsg* pclMsg , SI32 id)
{
	cltGameMsgResponse_Char_Add_Name_Check* pclInfo = (cltGameMsgResponse_Char_Add_Name_Check*)pclMsg->cData;

	if( m_pDialog[ NPOSTOFFICE_GROUP_LIST_DLG ] )
		((NPostOfficeGroupListDlg*)m_pDialog[ NPOSTOFFICE_GROUP_LIST_DLG ])->AddRecvUser( pclInfo );

}


