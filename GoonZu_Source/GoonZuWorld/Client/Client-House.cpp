//---------------------------------
// 2003/11/17 김태곤
//---------------------------------

#include "Client.h"
#include "Music\Music.h"
#include "..\CommonLogic\MsgType.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "../Common/Skill/Skill-Manager.h"

#include "MsgType-House.h"

#include "./Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "./Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "./NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "./Interface/DrawChat/DrawChat.h"
#include "./InterfaceMgr/InterfaceMgr.h"

#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "NStructureDlg/House/UserInfo/NHouseUserInfoBaseDlg.h"
#include "NStructureDlg/House/userinfo/NHouseUserNewManagementDlg.h"

#include "./NInterface/NCharStatus/NCharStatus.h"
#include "./NInterface/NCharStatus/NCharHorse.h"


// 자신의 시전의 정보를 받았다. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_MYHOUSEUNITLIST(cltMsg* pclMsg, SI32 id)
{
	SI32 i;

	// 받은 정보를 확인한다. 
	cltGameMsgResponse_MyHouseUnitList* pclinfo = (cltGameMsgResponse_MyHouseUnitList*)pclMsg->cData;

	SI32 siHouseIndex = 0;

	// 받은 정보를 시전 관리자에게 전달한다. 
	for(i = 0;i < MAX_RENT_COUNT;i++)
	{
		if(_tcscmp(pclinfo->clMyHouseInfo[i].m_szName,TEXT("")))
		{
			siHouseIndex = pclinfo->clMyHouseInfo[i].m_siIndex;
			pclHouseManager->clClientUnit[siHouseIndex].clContract.Set(&pclinfo->clMyHouseInfo[i].m_clRentContract);
			_tcscpy(pclHouseManager->clClientUnit[siHouseIndex].m_szName, pclinfo->clMyHouseInfo[i].m_szName);
		}
	}

	// 받은 정보를 화면에 표시하도록 한다. 
	pclHouseManager->bUpdateHouseUnitListShowChiefSwitch = true;
	pclHouseManager->bUpdateHouseUnitListShowSwitch		= true;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITNAMECHANGE(cltMsg* pclMsg, SI32 id)
{
	// 받은 정보를 확인한다. 
	cltGameMsgResponse_ChangeHouseUnitName* pclinfo = (cltGameMsgResponse_ChangeHouseUnitName*)pclMsg->cData;

	_tcscpy(pclHouseManager->clClientUnit[pclinfo->m_siHouseIndex].m_szName, pclinfo->m_szHouseName);

	pclHouseManager->bUpdateHouseUnitListShowChiefSwitch = true;
	pclHouseManager->bUpdateHouseUnitListShowSwitch		= true;

	if( NULL == m_pDialog[NHOUSEUSERINFOBASE_DLG])
		return;

	CNHouseUserNewManagementDlg* pclHouseManagerDlg = ((CNHouseUserInfoBaseDlg*)m_pDialog[NHOUSEUSERINFOBASE_DLG])->GetNHouseUserNewManagementDlg();
	if( NULL == pclHouseManagerDlg)
		return;

	if(!pclHouseManagerDlg->IsShow())
		return;

	pclHouseManagerDlg->LoadMyHouseList();
}

// 시전의 기본정보를 받았다. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITLIST(cltMsg* pclMsg, SI32 id)
{
	SI32 i;

	// 받은 정보를 확인한다. 
	cltGameMsgRequest_HouseUnitList* pclinfo = (cltGameMsgRequest_HouseUnitList*)pclMsg->cData;
	
	// 받은 정보를 시전 관리자에게 전달한다. 
	for(i = 0;i < MAX_HOUSEUNIT_NUMBER;i++)
	{
		pclHouseManager->clClientUnit[i].clPerson.Set(&pclinfo->clPerson[i]);

		// 서버에서 넘어온 정보에 판매중인지에 대한 정보도 같이 포함되어 있기 때문에 해당 인덱스의 하우스 유닛에 판매중 정보를 같이 세팅한다.
		pclHouseManager->clClientUnit[i].bSell = pclinfo->bSell[i];
	}
	
	// 받은 정보를 화면에 표시하도록 한다. 
	if( FALSE == IsCountrySwitch(Switch_ChangeHouseList))
	{
		pclHouseManager->bUpdateHouseUnitListShowChiefSwitch = true;
		pclHouseManager->bUpdateHouseUnitListShowSwitch		= true;
	}
}


// 시전의 상세정보를 받았다. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITLIST_DETAIL(cltMsg* pclMsg, SI32 id)
{
	SI32 i;

	// 받은 정보를 확인한다. 
	cltGameMsgRequest_HouseUnitListDetail* pclinfo = (cltGameMsgRequest_HouseUnitListDetail*)pclMsg->cData;
	
	// 받은 정보를 시전 관리자에게 전달한다. 
	for(i = 0;i < MAX_HOUSEUNIT_NUMBER;i++)
	{
		pclHouseManager->clClientUnit[i].clContract.Set(&pclinfo->clContract[i]);
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSECANCELLATIONPENALTYMONEY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HouseCancellationPenaltyMoney* pclinfo = (cltGameMsgResponse_HouseCancellationPenaltyMoney*)pclMsg->cData;
	
	GMONEY siPenaltyMoney = pclinfo->siPenaltyMoney;
	

	cltClient *pclclient = (cltClient *)pclClient;
	TCHAR * pTitle = GetTxtFromMgr(5831);		
	TCHAR * pTextSrc = GetTxtFromMgr( 5833);
	TCHAR pTextDest[256];	

	StringCchPrintf(pTextDest,256,pTextSrc, siPenaltyMoney );
	

	stMsgBox MsgBox;
	MsgBox.Set( pclclient->m_pDialog[NHOUSE_DLG]
			, pTitle , pTextDest, MSGBOX_TYPE_YESNO, 0 );
	
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}




// 고물상 생성
void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITUSEDGOODSBUILDINGCREATE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HouseUnitUsedGoodsBuildingCreate *pclinfo = ( cltGameMsgResponse_HouseUnitUsedGoodsBuildingCreate * )pclMsg->cData;

	pclCM->CR[1]->pclCI->clCharItem.clItem[ pclinfo->siItemPos ].Set( &pclinfo->clLeftItem );

	TCHAR* pTitle = GetTxtFromMgr(813);
	TCHAR* pText  = GetTxtFromMgr(814);

	m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

	if( pclHouseManager->siVillageUnique == pclinfo->siVillageUnique )
	{				
		pclHouseManager->clClientUnit[ pclinfo->siHouseUnitSlot ].clStorageInfo.bCreatedHouseUnitUsedGoodsBuilding = true;
	}
	
	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsInfo *pclinfo = ( cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsInfo * )pclMsg->cData;
	
	if(m_pDialog[ NHOUSEUSERINFOBASE_DLG ])
#ifdef _SAFE_MEMORY
		((CNHouseUserInfoBaseDlg*)m_pDialog[ NHOUSEUSERINFOBASE_DLG ])->ShowMakeItemFromUsedGoodsItem( &pclinfo->clMakeableItem[0] );
#else
		((CNHouseUserInfoBaseDlg*)m_pDialog[ NHOUSEUSERINFOBASE_DLG ])->ShowMakeItemFromUsedGoodsItem( pclinfo->clMakeableItem );
#endif

//	m_pHouseUnitDlg->ShowMakeItemFromUsedGoodsItem( pclinfo->clMakeableItem );

	return;
}

// 고물 해체
void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODS(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoods *pclinfo = ( cltGameMsgResponse_HouseUnitMakeItemFromUsedGoods * )pclMsg->cData;
	
	if( pclHouseManager->siVillageUnique == pclinfo->siVillageUnique )
	{
		pclHouseManager->clClientUnit[ pclinfo->siHouseUnitSlot ].clStorageInfo.clItem[ pclinfo->siSalvageItemPos ].Init();

		for ( SI16 i = 0; i < MAX_MATERIAL_NUMBER; ++i )
		{
			if ( pclinfo->siToHouseUnitStgItemPos[ i ] >= 0 )
			{
				pclHouseManager->clClientUnit[ pclinfo->siHouseUnitSlot ].clStorageInfo.clItem[ pclinfo->siToHouseUnitStgItemPos[ i ] ].Set( &pclinfo->clMakeItem[ i ] );
			}
		}
	}

	TCHAR* pTitle = GetTxtFromMgr(815);
	TCHAR* pText  = GetTxtFromMgr(816);

	m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITMAKEITEMFROMUSEDGOODSNOTIFY(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsNotify *pclInfo = ( cltGameMsgResponse_HouseUnitMakeItemFromUsedGoodsNotify * )pclMsg->cData;

	if ( pclInfo->siCharUnique < 1 )
	{
		return;
	}

	id = cltCharCommon::CharUniqueInfo[ pclInfo->siCharUnique ];

	if ( id < 1 || pclCM->IsValidID( id ) == FALSE )
	{
		return;
	}

	cltCharClient *pclCharClient = (cltCharClient *)pclCM->CR[id];

	if ( pclCharClient == NULL )
	{
		return;
	}

	TCHAR buffer[ 128 ] = TEXT("");
	TCHAR *ItemName = (TCHAR*)pclItemManager->GetName( pclInfo->siItemUnique );
	if ( ItemName )
	{
		TCHAR* pText = GetTxtFromMgr(3923);
		TCHAR prob[128]= TEXT(" ") ;

		StringCchPrintf(prob, 128, TEXT("%d"),pclInfo->siMakePercent);
		Nsprintf(buffer,pText,TEXT("item"),   ItemName ,TEXT("prob"), prob   ,NULL);

		//sprintf(buffer, pText, ItemName, pclInfo->siMakePercent );
		
		if(pclCharClient->GetGameMasterLevel() >= 100 )
		{
		}
		else
			pclCharClient->pclDrawChat->Set( pclDrawChatSurface, buffer, false, PRODUCTION_CHATTING );
	}
}


// 사용자의 말을 시전에 넣는다. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITHORSEIN(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HouseUnitHorseIn *pclinfo = (cltGameMsgResponse_HouseUnitHorseIn *)pclMsg->cData;
	
	if(pclHouseManager->siVillageUnique == pclinfo->siVillageUnique)
	{	
		// 말에서 내린 다음에, 초기화 해줌 ( 캐릭터 조합에서 에러가 날 수 있으므로... )
		pclCM->CR[1]->UnrideHorse();
		pclHouseManager->clClientUnit[ pclinfo->siHouseUnitSlot ].clStorageInfo.clHorse[ pclinfo->siStgHorseIndex ].Set( &pclinfo->clStgHorse );
	}

	//KHY - 1001 - 기승동물 5마리 보유 수정.
	if( m_pDialog[ CLTNCHARSTATUS_DLG ] )
	{
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->m_bChangedInitHorseStatus = TRUE;
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->InitHorseStatusInfo();
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->UpdateHorseStatusInfo();
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_HOUSEUNITHORSEOUT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_HouseUnitHorseOut *pclinfo = (cltGameMsgResponse_HouseUnitHorseOut*)pclMsg->cData;
	
	if(pclHouseManager->siVillageUnique == pclinfo->siVillageUnique)
	{
		// 말에서 내린 다음에, 초기화 해줌 ( 캐릭터 조합에서 에러가 날 수 있으므로... )
		pclCM->CR[1]->UnrideHorse();
		pclHouseManager->clClientUnit[ pclinfo->siHouseUnitSlot ].clStorageInfo.clHorse[ pclinfo->siStgHorseIndex ].Set( &pclinfo->clStgHorse );
		pclCM->CR[1]->DeleteSlave();
		pclCM->CR[1]->pclCI->clHorseInfo.SetHorse(pclinfo->siStgHorseIndex, &pclinfo->clHorse);	
	}

	//KHY - 1001 - 기승동물 5마리 보유 수정.
	if( m_pDialog[ CLTNCHARSTATUS_DLG ] )
	{
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->m_bChangedInitHorseStatus = TRUE;
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->InitHorseStatusInfo();
		((cltNCharStatus*) m_pDialog[ CLTNCHARSTATUS_DLG ])->m_pTabDlg1->UpdateHorseStatusInfo();
	}
}
