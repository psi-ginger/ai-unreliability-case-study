//---------------------------------
// 2004/3/12 김태곤
//---------------------------------

#include "..\Client.h"
#include "..\Music\Music.h"
#include "..\CommonLogic\MsgType.h"
#include "Resource.h"
#include "..\CommonLogic\MsgType-Item.h"
#include "../CommonLogic/MsgType-System.h"
#include "..\NPCRepair\NPCRepair.h"
#include "../NInterface/NNPCRepairDlg\NNPCRepair.h"

#include "../Interface/DrawChat/DrawChat.h"
#include "../Inventory/ItemInventory.h"
#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
//#include "../Interface/MakeItemDlg/MakeItemDlg.h"
#include "../NInterface/NMakeItemDlg/NMakeItemDlg.h"
#include "../Interface/WennyOldMan/WennyOldMan.h"

#include "../Interface/MinigameDlg/MiniGameDlg.h"
#include "../MiniGame/MiniGameManager.h"

#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "../NInterface/NWennyOldMan/NWennyOldMan.h"
#include "../NInterface/NWennyOldMan/NWennyChild1.h"
#include "../NInterface/NWennyOldMan/NWennyChild2.h"

#include "../NInterface/NNewInventory/NNewInventory.h"
#include "../NInterface/NNewEnchantOther/NNewEnchantOther.h"

#include "../ninterface/nnewmakeitem/NNewEnchantChild.h"

#include "../ninterface/NSpaceBoxDlg/NSpaceBoxDlg.h"
#include "../ninterface/NAmberBoxDlg/NAmberBoxDlg.h"
#include "../ninterface/NElixirDlg/NElixirNPCDlg.h"
#include "../ninterface/NNewSelectBoxDlg/NNewSelectBoxDlg.h"
#include "../ninterface/nnewmakeitem/NItemRepair.h"

#include "../ninterface/nnpcrepairdlg/NNewNPCRepair.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

// NPC수리 가격 응답. 
void cltClient::DoMsg_GAMEMSG_RESPONSE_NPCREPAIRPRICE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NPCRepairPrice* pclInfo = (cltGameMsgResponse_NPCRepairPrice*)pclMsg->cData;




	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		if(( m_pDialog[ NNPCREPAIR_DLG  ])==NULL)
		{
			CreateInterface( NNPCREPAIR_DLG );
		}
		((NcltNPCRepairManager*)m_pDialog[ NNPCREPAIR_DLG  ])->SetItemRepairPriceExplain( pclInfo->siItemUnique, pclInfo->siRepairePrice );

	}

}

// clothes 의 Active상태를 바꾼다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_EXCHANGECLOTHES(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ExchageClothes* pclInfo = (cltGameMsgResponse_ExchageClothes*)pclMsg->cData;
	id = 1;
	pclCM->CR[id]->ActiveClothes = pclInfo->siActiveClothes;

	SI16 siUnique = pclCM->CR[id]->pclCI->clCharItem.clItem[pclInfo->siActiveClothes].siUnique;	
	pclCM->CR[id]->ChangeArmCode(pclInfo->siActiveClothes, siUnique);


	pclCM->CR[id]->ActiveHat =  pclInfo->siActiveHat;
	siUnique = pclCM->CR[id]->pclCI->clCharItem.clItem[pclInfo->siActiveHat].siUnique;
	pclCM->CR[id]->ChangeArmCode(pclInfo->siActiveHat, siUnique);




}

// Weapon의 Active상태를 바꾼다.
void cltClient::DoMsg_GAMEMSG_RESPONSE_EXCHANGEWEAPON(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_ExchageWeapon* pclInfo = (cltGameMsgResponse_ExchageWeapon*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharClient* pclchar = pclCM->GetCharClient( id );
	if ( NULL == pclchar )
	{
		return;
	}

	SI16 siActiveWeapon = pclInfo->siActiveWeapon;

	// 케릭터 무기를 변경
	pclchar->ActiveWeapon = siActiveWeapon;

	// 효과음을 연주한다. 
	SI32 siItemUnique = pclchar->pclCI->clCharItem.clItem[ siActiveWeapon ].siUnique;
	if ( siItemUnique )
	{
		pclItemManager->PlaySound( siItemUnique, ITEMSOUND_SELECT );
	}

	// 파라미터박스를 강재로 실행해 준다
	pclchar->ParametaBoxAction();

	// 현재의 ActiveWeapon으로 외형을 변경한다
	pclchar->ChangeArmCode( siActiveWeapon, siItemUnique );

	// 변경된 무기 타입이 기존 무기 타입과 다를경우 버프마법 효과를 초기화 시킨다
	SI32 siWeaponType1 = pclchar->pclCI->clCharItem.clItem[ PERSONITEM_WEAPON1 ].GetType( pclItemManager );
	SI32 siWeaponType2 = pclchar->pclCI->clCharItem.clItem[ PERSONITEM_WEAPON2 ].GetType( pclItemManager );
	if ( siWeaponType1 != siWeaponType2 )
	{
		pclchar->m_clMagicBufMgr.Init();
	}


}

void cltClient::DoMsg_GAMEMSG_RESPONSE_UPGRADEITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_UpgradeItem* pclInfo = (cltGameMsgResponse_UpgradeItem*)pclMsg->cData;

	//id = 1;
	//if ( pclInfo->bSuccess )
	//{		   
	//	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	//	{	
	//		if(( m_pDialog[ NWENNYMAN_DLG  ])!=NULL)	
	//		{
	//		(((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG ])->Wennytabdlg0)->UpgradeItemSuccess( true );			
	//		//(((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG ])->Wennytabdlg0)->LoadUpgradeItemData();
	//		}
	//	}
	//}
	//else
	//{  
	//	if ( g_SoundOnOffDlg.m_bFullScreenMode )
	//	{
	//
	//		if(( m_pDialog[ NWENNYMAN_DLG  ])!=NULL)	
	//		{
	//			(((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG ])->Wennytabdlg0)->UpgradeItemSuccess( false );			
	//			//(((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG ])->Wennytabdlg0)->LoadUpgradeItemData();
	//		}
	//	}
	//}


	//pclCM->CR[id]->pclCI->clCharItem.clItem[ pclInfo->siFromPos ].Set( &pclInfo->clFromItem );
	//pclCM->CR[id]->pclCI->clCharItem.clItem[ pclInfo->siToPos ].Set( &pclInfo->clToItem );

	return;
}

// 아이템 성능 향상 시킴 ( 부적 붙혀서 )
void cltClient::DoMsg_GAMEMSG_RESPONSE_PERFORMANCEUPGRADEITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_PerformanceUpgradeItem* pclInfo = (cltGameMsgResponse_PerformanceUpgradeItem*)pclMsg->cData;	


	//if ( g_SoundOnOffDlg.m_bFullScreenMode )
	//{
	//	
	//	if(( m_pDialog[ NWENNYMAN_DLG  ])!=NULL)	
	//	{
	//		(((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG ])->Wennytabdlg1)->PerformanceUpgradeItemSuccess();
	//		if((((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG ])->Wennytabdlg1)->IsChangedInvItem())
	//			(((CNWennyOldMan*)m_pDialog[NWENNYMAN_DLG ])->Wennytabdlg1)->LoadPerformanceUpgradeItemData();
	//	}
	//}

	//pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siItemPos ].Set( &pclInfo->clItem );
	//pclCM->CR[1]->pclCI->clCharItem.clItem[ pclInfo->siScrollItemPos ].Set( &pclInfo->clScrollItem );

	return;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_MAKEITEMDLG(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_MakeItemDlg* pclInfo = (cltGameMsgResponse_MakeItemDlg*)pclMsg->cData;

	cltGameMsgRequest_MakeItemSwitch clMakeItemSwitch( 0 );
	cltMsg clMsg(GAMEMSG_REQUEST_MAKEITEMSWITCH,sizeof(clMakeItemSwitch), (BYTE*)&clMakeItemSwitch);
	SendMsgToServer((sPacketHeader*)&clMsg);


	if(m_pDialog[ NMAKEITEM_DLG ] == NULL)
		CreateInterface( NMAKEITEM_DLG );	
	((CNMakeItemDlg*)m_pDialog[NMAKEITEM_DLG ])->Set(&pclInfo->clItem,pclInfo->siBaseExp,pclInfo->siSkillUnique,pclInfo->siSkillExp,pclInfo->siMonsterCorp);

}




void cltClient::DoMsg_GAMEMSG_RESPONSE_ENCHANTITEMDLG(cltMsg* pclMsg, SI32 id)
{
  	cltGameMsgResponse_EnchantItemDlg* pclInfo = (cltGameMsgResponse_EnchantItemDlg*)pclMsg->cData;

	if( pclInfo == NULL )				return;

	cltGameMsgRequest_MakeItemSwitch clMakeItemSwitch( 0 );
	cltMsg clMsg(GAMEMSG_REQUEST_MAKEITEMSWITCH,sizeof(clMakeItemSwitch), (BYTE*)&clMakeItemSwitch);
	SendMsgToServer((sPacketHeader*)&clMsg);

	TCHAR Nbuffer[512]		= {'\0'};
	TCHAR sz_item[128]		= {'\0'};
	TCHAR sz_grade[128]		= {'\0'};
	TCHAR sz_element[128]	= {'\0'};
	TCHAR buf[64]			= {'\0'};
	SI32 siItemGrade		= pclInfo->clItem.Element.siPower;
	
	// 인첸트 아이템 관련 정보를 초기화
	pclClient->pclCM->CR[1]->siOrgInvEnchantInx = 0;
	pclClient->pclCM->CR[1]->siMatInvEnchantInx = 0;
	for ( SI32 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++ )
	{
		pclClient->pclCM->CR[1]->siElementEnchantInx[siIndex] = 0;
	}
	
	TCHAR *pTitle =GetTxtFromMgr(5920);
	TCHAR *pText = GetTxtFromMgr(5921);
	if(pclInfo->siSucess == ENCHANT_SUCCESS ) 
	{		
		strcpy(sz_element, ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(buf, pclInfo->clItem.Element.siType));
		strcpy(sz_item, pclItemManager->GetName(pclInfo->clItem.siUnique));
		sprintf(sz_grade, "%d",siItemGrade );
		Nsprintf(Nbuffer,pText,"item", sz_item   ,"element", sz_element, "grade", sz_grade ,NULL);
		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, Nbuffer);
	}
	else if(pclInfo->siSucess == ENCHANT_FAIL )  // 인첸트 실패 ( 등급 유지 )
	{
		pText = GetTxtFromMgr(5922);	

		StringCchPrintf( Nbuffer , 512 , TEXT("%s") , pText );

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, Nbuffer);
	}
	else if(pclInfo->siSucess == ENCHANT_DOWNGRADE )  // 인첸트 실패 ( 다운 그레이드 )
	{
		pText = GetTxtFromMgr(7007);	

		StringCchPrintf( Nbuffer , 512 , TEXT("%s") , pText );

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, Nbuffer);
	}
	else if(pclInfo->siSucess == ENCHANT_BROKEN )  // 인첸트 실패 ( 사라짐 )
	{
		pText = GetTxtFromMgr(7006);	

		StringCchPrintf( Nbuffer , 512 , TEXT("%s") , pText );

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, Nbuffer);
	}
	else if(pclInfo->siSucess == ENCHANT_USEDAYPLUS ) // 기간연장 아이템 사용 성공 메시지
	{
		if( pclInfo->clItem.GetFullName( pclItemManager , sz_item , 128 ) == false ) return;
		
		pText = GetTxtFromMgr(7081);	

		StringCchPrintf( Nbuffer , 512 , pText ,  sz_item );

		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, Nbuffer);
	}
	else if ( pclInfo->siSucess == ENCHANT_CHANGE )	// 인첸트 속성변경 성공 메시지
	{
		NTCHARString256		kMessage		= GetTxtFromMgr(7951);

		
		strcpy(sz_element, ((cltItemManagerClient*)pclItemManager)->GetEnchantTypeName(buf, pclInfo->clItem.Element.siType));

		if( pclInfo->clItem.GetFullName( pclItemManager , sz_item , 128 ) == false ) return;

		kMessage.Replace( TEXT("#itemname#"), sz_item );
		kMessage.Replace( TEXT("#enchanttype#"), sz_element );
		
		m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, kMessage );
	}

	if(m_pDialog[NNEWINVENTORY_DLG])
	{
		CNNewInventory* inventory = (CNNewInventory*) m_pDialog[NNEWINVENTORY_DLG];
		inventory->MakeSetEnchant(true);
	}

	if(m_pDialog[NNEWITEMMAKE_DLG])
	{		
			CNewMakeItemDlg* enchantdlg = (CNewMakeItemDlg* )m_pDialog[NNEWITEMMAKE_DLG];

			/*
			(enchantdlg->NewEnchantTab)->m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ORIGINALITEM)->DeleteAllItems();
			(enchantdlg->NewEnchantTab)->m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM)->DeleteAllItems();			
			(enchantdlg->NewEnchantTab)->m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM)->DeleteAllItems();
			*/
			(enchantdlg->NewEnchantTab)->SetInitInchant();
			//(enchantdlg->NewEnchantTab)->EnableButton(true,true,true,true);
	}

	SendChatMessage(CHATMODE_SYSTEM, Nbuffer);



	if(m_pDialog[ NNEWENCHANTOTHER_DLG ] )
		((CNNewEnchantOther*)m_pDialog[NNEWENCHANTOTHER_DLG ])->PushCancel();

	//((CNMakeItemDlg*)m_pDialog[NMAKEITEM_DLG ])->Set(&pclInfo->clItem,pclInfo->siBaseExp,pclInfo->siSkillUnique,pclInfo->siSkillExp,pclInfo->siMonsterCorp);
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_ISSKILLITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_IsSkillItem* pclinfo = (cltGameMsgResponse_IsSkillItem*)pclMsg->cData;

	if ( pclinfo->siResult == 1 )
	{
		if ( g_SoundOnOffDlg.m_bNotifyMinigame == true )
		{
			cltGameMsgRequest_MonsterCorp_Score clinfo(0);
			cltMsg clMsg(GAMEMSG_REQUEST_MONSTERCORP_SCORE,sizeof(clinfo),(BYTE*)&clinfo);
			SendMsgToServer((sPacketHeader*)&clMsg);
			return ;
		}

		CreateInterface( MONSTERCORPORATION_DLG );
	}

	return ;
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_PREMIUM_INFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Premium_Info* pclinfo = (cltGameMsgResponse_Premium_Info*)pclMsg->cData;

	if ( pclinfo->siType == SETPREMIUM_EXP )
	{
		pclCM->CR[1]->pclCI->clBI.bPremiumService = pclinfo->bPremium ;
		pclCM->CR[1]->pclCI->clBI.siPremiumServiceDate = pclinfo->siPremiumDate ;
	}
	else if ( pclinfo->siType == SETPREMIUM_COMBAT )
	{
		pclCM->CR[1]->pclCI->clBI.bCombatService = pclinfo->bPremium ;
		pclCM->CR[1]->pclCI->clBI.siCombatServiceDate = pclinfo->siPremiumDate ;
	}
	else if ( pclinfo->siType == SETPREMIUM_MAKEITEM )
	{
		pclCM->CR[1]->pclCI->clBI.bMakeItemService = pclinfo->bPremium ;
		pclCM->CR[1]->pclCI->clBI.siMakeItemServiceDate = pclinfo->siMakeItemDate;
	}
	else if ( pclinfo->siType == SETPREMIUM_BULLET )
	{
		pclCM->CR[1]->pclCI->clBI.bBulletService = pclinfo->bPremium ;
		pclCM->CR[1]->pclCI->clBI.siBulletServiceDate= pclinfo->siBulletDate;
	}
	else if ( pclinfo->siType == SETPREMIUM_SUMMONSTIMINA )
	{
		pclCM->CR[1]->pclCI->clBI.bSummonStaminaService = pclinfo->bPremium ;
		pclCM->CR[1]->pclCI->clBI.siSummonStaminaServiceDate= pclinfo->siSummonStaminaDate;
	}

	else return ;

	if ( pclinfo->siItemPos != -1 && pclinfo->siItemPos >= 0 )
		pclCM->CR[1]->pclCI->clCharItem.clItem[pclinfo->siItemPos].Set(&pclinfo->clItem);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SETITEMTYPEINFO( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_SetItemTypeInfo* pclinfo = (cltGameMsgResponse_SetItemTypeInfo*)pclMsg->cData;

	pclCM->CR[1]->m_siSetItemType = pclinfo->GetSetItemTypeInfo();
	pclCM->CR[1]->m_siSetItemStep = pclinfo->GetSetItemStepInfo();

	pclCM->CR[1]->m_siSetItemType2 = pclinfo->GetSetItemTypeInfo2();
	pclCM->CR[1]->m_siSetItemStep2 = pclinfo->GetSetItemStepInfo2();
}

//[추가 : 황진성 2008. 1. 29 =>  공간 상자 리스트 설정 메시지 처리. S->C]
void cltClient::DoMsg_GAMEMSG_RESPONSE_SPACE_BOX_LIST( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_Space_Box_List* pclinfo = (cltGameMsgResponse_Space_Box_List*)pclMsg->cData;

	if(pclCM->CR[1]->pclSpaceBoxItem == NULL)		return;
	if(pclCM->CR[1]->pclSpaceBoxItem == NULL)		return;

	pclCM->CR[1]->pclSpaceBoxItem->Set(&pclinfo->m_SpaceBoxItem);
	pclCM->CR[1]->pclTreasureBoxItem->Set(&pclinfo->m_TreasureBoxItem);

	
	CNSpaceBoxDlg* pNSpaceBoxDlg = (CNSpaceBoxDlg*)m_pDialog[ NSPACE_BOX ];	
	if(NULL == pNSpaceBoxDlg)
		return;

	pNSpaceBoxDlg->SetMyListView();
	pNSpaceBoxDlg->SetTreasureBoxListView();
	pNSpaceBoxDlg->SetSpaceBoxListView();
}
//[추가 : 황진성 2008. 1. 29 =>  공간 상자(보물) 리스트 설정 메시지 처리. S->C]
void cltClient::DoMsg_GAMEMSG_RESPONSE_TRESURE_BOX_LIST( cltMsg* pclMsg, SI32 id )
{
	
}


//[추가 : 황진성 2008. 1. 30 =>  공간 상자 이동 메시지 처리. S->C]
void cltClient::DoMsg_GAMEMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_Keep_Box_Move_Item* pclinfo = (cltGameMsgResponse_Keep_Box_Move_Item*)pclMsg->cData;
	
	cltItem* InvenItem = &pclCM->CR[1]->pclCI->clCharItem.clItem[pclinfo->m_siInvenPos];	
	
	cltItem* BoxItem = NULL;
	if(pclinfo->m_bGacha)
	{
		if(pclCM->CR[1]->pclTreasureBoxItem)	BoxItem = &pclCM->CR[1]->pclTreasureBoxItem->clItem[pclinfo->m_siKeepPos];
	}
	else
	{
		if(pclCM->CR[1]->pclSpaceBoxItem)		BoxItem = &pclCM->CR[1]->pclSpaceBoxItem->clItem[pclinfo->m_siKeepPos];
	}
	if(BoxItem == NULL)			return;

	InvenItem->Init();
	InvenItem->Set(&pclinfo->m_InvenItem);

	BoxItem->Init();
	BoxItem->Set(&pclinfo->m_KeepItem);

	CreateInterface(NSPACE_BOX);

	CNSpaceBoxDlg* pNSpaceBoxDlg = (CNSpaceBoxDlg*)m_pDialog[ NSPACE_BOX ];	
	
	pNSpaceBoxDlg->SetMyListView();

	if( pclinfo->m_bGacha )
	{
		pNSpaceBoxDlg->SetTreasureBoxListView();
	}
	else
	{
		pNSpaceBoxDlg->SetSpaceBoxListView();
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if( pclclient->m_pDialog[NELIXIR_NPC_DLG] )
	{
		((NElixirNPCDlg*)pclclient->m_pDialog[NELIXIR_NPC_DLG])->SetMyInventory();
	}
}

//[추가 : 황진성 2008. 1. 30 =>  공간 상자 이동 메시지 처리. S->C]
void cltClient::DoMsg_GAMEMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_Recovery_Box_Move_Item* pclinfo = (cltGameMsgResponse_Recovery_Box_Move_Item*)pclMsg->cData;

	cltItem* InvenItem = &pclCM->CR[1]->pclCI->clCharItem.clItem[pclinfo->m_siInvenPos];	

	cltItem* BoxItem = NULL;
	if(pclinfo->m_bGacha)
	{
		if(pclCM->CR[1]->pclTreasureBoxItem)	BoxItem = &pclCM->CR[1]->pclTreasureBoxItem->clItem[pclinfo->m_siRecoveryPos];
	}
	else
	{
		if(pclCM->CR[1]->pclSpaceBoxItem)		BoxItem = &pclCM->CR[1]->pclSpaceBoxItem->clItem[pclinfo->m_siRecoveryPos];
	}
	if(BoxItem == NULL)		return;

	InvenItem->Init();
	InvenItem->Set(&pclinfo->m_InvenItem);

	BoxItem->Init();
	BoxItem->Set(&pclinfo->m_RecoveryItem);
	
	CreateInterface(NSPACE_BOX);

	CNSpaceBoxDlg* pNSpaceBoxDlg = (CNSpaceBoxDlg*)m_pDialog[ NSPACE_BOX ];	

	pNSpaceBoxDlg->SetMyListView();

	if( pclinfo->m_bGacha )
	{
		pNSpaceBoxDlg->SetTreasureBoxListView();
	}
	else
	{
		pNSpaceBoxDlg->SetSpaceBoxListView();
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if( pclclient->m_pDialog[NELIXIR_NPC_DLG] )
	{
		((NElixirNPCDlg*)pclclient->m_pDialog[NELIXIR_NPC_DLG])->SetMyInventory();
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_USE_SPACEBOX_ITEM( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_Use_SpaceBox_Item* pclInfo = (cltGameMsgResponse_Use_SpaceBox_Item*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	SI16			siItemPos	= pclInfo->m_siItemPos;
	cltCharClient*	pclchar		= (cltCharClient*)pclCM->CR[ id ];
	if ( NULL == pclchar )
	{
		return;
	}
	if(pclchar->pclSpaceBoxItem == NULL)		return;

	pclchar->pclSpaceBoxItem->clItem[ siItemPos ].Set( &pclInfo->m_clItem );

	CNSpaceBoxDlg* pNSpaceBoxDlg = (CNSpaceBoxDlg*)m_pDialog[ NSPACE_BOX ];	
	if ( NULL == pNSpaceBoxDlg )
	{
		return;
	}

	pNSpaceBoxDlg->SetMyListView();
	pNSpaceBoxDlg->SetSpaceBoxListView();

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_USE_TREASURE_ITEM( cltMsg* pclMsg, SI32 id )
{
	cltGameMsgResponse_Use_Treasure_Item* pclInfo = (cltGameMsgResponse_Use_Treasure_Item*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	SI16			siItemPos	= pclInfo->m_siItemPos;
	cltCharClient*	pclchar		= (cltCharClient*)pclCM->CR[ id ];
	if ( NULL == pclchar )
	{
		return;
	}
	if(pclchar->pclTreasureBoxItem == NULL)		return;

	pclchar->pclTreasureBoxItem->clItem[ siItemPos ].Set( &pclInfo->m_clItem );

	CNSpaceBoxDlg* pNSpaceBoxDlg = (CNSpaceBoxDlg*)m_pDialog[ NSPACE_BOX ];	
	if ( NULL == pNSpaceBoxDlg )
	{
		return;
	}

	pNSpaceBoxDlg->SetMyListView();
	pNSpaceBoxDlg->SetTreasureBoxListView();

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SUMMONCHANGEITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_SummonChangeItem* pclinfo = (cltGameMsgResponse_SummonChangeItem*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	SI32 siSummonIndex	= pclinfo->m_siSummonIndex;
	SI16 siItemPos		= pclinfo->m_siItemPos;

	cltCharClient* pclchar = (cltCharClient*)pclCM->CR[ id ];
	if ( NULL == pclchar )
	{
		return;
	}

	pclchar->pclCI->clSummonInfo.SetSummonEquipItem( siSummonIndex, siItemPos, &pclinfo->m_clItem );

}

// [진성] 호박상자 리스트 주기 => 2008-9-16
void cltClient::DoMsg_GAMEMSG_RESPONSE_AMBER_BOX_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Amber_Box_List* pclinfo = (cltGameMsgResponse_Amber_Box_List*)pclMsg->cData;
	if ( NULL == pclinfo )	return;
	
	if(( m_pDialog[ NAMBER_BOX_DLG ]))
		DestroyInterface(m_pDialog[ NAMBER_BOX_DLG ]);

	if(( m_pDialog[ NAMBER_BOX_DLG ])==NULL)
	{
		CreateInterface( NAMBER_BOX_DLG );
	}
	((CNAmberBoxDlg*)m_pDialog[ NAMBER_BOX_DLG ])->SetItemList( pclinfo->m_siItemList, pclinfo->m_siListNum, pclinfo->m_siUseItemUnique );
	
}

// [진성] 호박상자 선택 아이템 리스트 주기 => 2008-9-16
void cltClient::DoMsg_GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Amber_Box_Select_Item_List* pclinfo = (cltGameMsgResponse_Amber_Box_Select_Item_List*)pclMsg->cData;
	if ( NULL == pclinfo )	return;

	if(( m_pDialog[ NAMBER_BOX_DLG ])==NULL)
	{
		CreateInterface( NAMBER_BOX_DLG );
	}
	((CNAmberBoxDlg*)m_pDialog[ NAMBER_BOX_DLG ])->SetSelectItemList( pclinfo->m_clAmberboxItemList );
}

// [진성] 호박상자 선택 아이템 리스트 주기 => 2008-9-16
void cltClient::DoMsg_GAMEMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_GET(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_Amber_Box_Select_Item_Get* pclInfo = (cltGameMsgResponse_Amber_Box_Select_Item_Get*)pclMsg->cData;
	if ( NULL == pclInfo )	return;

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( id );
	if( NULL == pclchar )	return;

	if( 0 < pclInfo->m_siRemainItemPos && MAX_ITEM_PER_PERSON > pclInfo->m_siRemainItemPos )
	{
		pclchar->pclCI->clCharItem.clItem[pclInfo->m_siRemainItemPos].Set( &pclInfo->m_clRemainItem );
	}
	
	if( 0 < pclInfo->m_siToItemPos	   && MAX_ITEM_PER_PERSON > pclInfo->m_siToItemPos	   )
	{
		pclchar->pclCI->clCharItem.clItem[pclInfo->m_siToItemPos].Set( &pclInfo->m_clToItem );
	}

	/*if(( m_pDialog[ NAMBER_BOX_DLG ]))
	{
		((CNAmberBoxDlg*)m_pDialog[ NAMBER_BOX_DLG ])->SetSelectItemListClear();	
	}*/
	
	// 창을 기냥 닫자.
	if(( m_pDialog[ NAMBER_BOX_DLG ]))
		DestroyInterface(m_pDialog[ NAMBER_BOX_DLG ]);
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_NEWSELECTBOX_SELECT(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NewSelectBox_Select* pclInfo = (cltGameMsgResponse_NewSelectBox_Select*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	if ( m_pDialog[NEWSELECTBOX_DLG] )
	{
		((CNNewSelectBoxDlg*)m_pDialog[NEWSELECTBOX_DLG])->SetSelectInfo( pclInfo->m_siSelectIndex, pclInfo->m_siItemList );
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_NEWSELECTBOX_GETITEM(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NewSelectBox_GetItem* pclInfo = (cltGameMsgResponse_NewSelectBox_GetItem*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( id );
	if( NULL == pclchar )
	{
		return;
	}

	if ( (0 < pclInfo->m_siBoxItemPos) && (MAX_ITEM_PER_PERSON > pclInfo->m_siBoxItemPos) )
	{
		pclchar->pclCI->clCharItem.clItem[pclInfo->m_siBoxItemPos].Set( &pclInfo->m_clBoxItem );
	}

	if ( (0 < pclInfo->m_siGetItemPos) && (MAX_ITEM_PER_PERSON > pclInfo->m_siGetItemPos) )
	{
		pclchar->pclCI->clCharItem.clItem[pclInfo->m_siGetItemPos].Set( &pclInfo->m_clGetItem );
	}

	if ( m_pDialog[NEWSELECTBOX_DLG] )
	{
		((CNNewSelectBoxDlg*)m_pDialog[NEWSELECTBOX_DLG])->SetGetItemInfo( &pclInfo->m_clToItem );
	}
}


void cltClient::DoMsg_GAMEMSG_RESPONSE_NEWNPCREPAIR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NewNpcRepair* pclInfo = (cltGameMsgResponse_NewNpcRepair*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_NEWITEMREPAIR(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NewItemRepair* pclInfo = (cltGameMsgResponse_NewItemRepair*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	switch( pclInfo->m_siType )
	{
	case ITEMREPAIR_MODE_SELF:
		{
			NTCHARString128 kTitle( GetTxtFromMgr(40119) );
			NTCHARString128 kText( "" );

			if( pclInfo->m_siResult == 1 )
				kText = GetTxtFromMgr(40133);
			else
				kText = GetTxtFromMgr(40134);
			kText.Replace(TEXT("#itemname#"), pclItemManager->GetName(pclInfo->m_clItem.siUnique) );
			m_pDialog[ NOTIFYMSG_DLG ]->SetText( kTitle, kText );
		}
		break;

	case ITEMREPAIR_MODE_REQUEST_PRECESS:	// 3:의뢰수리
		{
			SI32 siCharID = pclCM->GetIDFromCharUnique( pclInfo->m_siItemCharUnique );
			cltCharClient* pclItemChar = pclCM->GetCharClient( siCharID );
			cltCharClient* pclchar = pclCM->GetCharClient( 1 );

			NTCHARString128 kTitle( GetTxtFromMgr(40119) );
			NTCHARString128 kText( "" );

			if( pclchar->GetCharUnique() != pclInfo->m_siItemCharUnique )
			{
				if( pclInfo->m_siResult == 1 )
					kText = GetTxtFromMgr(40133);
				else
					kText = GetTxtFromMgr(40134);
				kText.Replace(TEXT("#itemname#"), pclItemManager->GetName(pclInfo->m_clItem.siUnique) );
				m_pDialog[ NOTIFYMSG_DLG ]->SetText( kTitle, kText );
			}

			if( pclInfo->m_siResult == 1 )
			{
				CNewMakeItemDlg* pclNewMakeItemDlg = (CNewMakeItemDlg*)m_pDialog[NNEWITEMMAKE_DLG];
				NItemRepairDlg* pclItemRepairDlg = NULL;
				if( pclNewMakeItemDlg )		pclItemRepairDlg = pclNewMakeItemDlg->NewItemRepair;

				if( pclItemRepairDlg )
				{
					if (pclItemRepairDlg->GetItemCharUnique() == pclInfo->m_siItemCharUnique )
					{
						pclItemRepairDlg->CloseDialog();
						pclNewMakeItemDlg->DeleteDialog();
					}
				}
				
				if( pclItemChar )
				{
					pclItemChar->bRepairShopSwitch = false;

					pclItemChar->pclDrawChat->StopDraw();
				}
			}
		}
		break;
	case ITEMREPAIR_MODE_CANCEL:
		{
			SI32 siCharID = pclCM->GetIDFromCharUnique( pclInfo->m_siItemCharUnique );
			cltCharClient* pclItemChar = pclCM->GetCharClient( siCharID );
			if( pclItemChar )
			{
				pclItemChar->pclDrawChat->StopDraw();

				CNewMakeItemDlg* pclNewMakeItemDlg = (CNewMakeItemDlg*)m_pDialog[NNEWITEMMAKE_DLG]	;

				if(pclNewMakeItemDlg != NULL )// 창이 열려 있으면 닫는다// 열려 있을떄만 닫으면 되니 따로 생성을 안한다 열려있으면 닫고 아니면 말고 
				{
					NItemRepairDlg* pclItemRepairDlg = NULL;
					pclItemRepairDlg = pclNewMakeItemDlg->NewItemRepair;
					pclNewMakeItemDlg->CloseDialog();
				}

				/*CreateInterface( NNEWITEMMAKE_DLG );
				CNewMakeItemDlg* pclNewMakeItemDlg = (CNewMakeItemDlg*)m_pDialog[NNEWITEMMAKE_DLG];
				NItemRepairDlg* pclItemRepairDlg = NULL;
				if( pclNewMakeItemDlg )		pclItemRepairDlg = pclNewMakeItemDlg->NewItemRepair;

				if( pclItemRepairDlg )
				{
				if (pclItemRepairDlg->GetItemCharUnique() == pclInfo->m_siItemCharUnique )
				{
				pclItemRepairDlg->CloseDialog();
				pclNewMakeItemDlg->DeleteDialog();
				}
				}*/
				pclItemChar->bRepairShopSwitch = false;
			}
		}
		break;

	case ITEMREPAIR_MODE_GROWUP_MAX:
		{
			NTCHARString32	strText		= GetTxtFromMgr(389);
			NTCHARString256	strTitle	= GetTxtFromMgr(40170);

			SI32			siMax		= (SI32)GetGlobalValue( "GV_Repair_MaxDurability" );

			strTitle.Replace( TEXT("#value#"), SI32ToString(siMax) );

			m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		}
		break;

	case ITEMREPAIR_MODE_NPC:
		{
			if ( m_pDialog[NNEWNPCREPAIR_DLG] )
			{
				NTCHARString128 strNPCText = GetTxtFromMgr(40133);
				
				strNPCText.Replace(TEXT("#itemname#"), pclItemManager->GetName(pclInfo->m_clItem.siUnique) );

				((NNewNPCRepairDlg*)m_pDialog[NNEWNPCREPAIR_DLG])->SetNPCText( strNPCText );
				((NNewNPCRepairDlg*)m_pDialog[NNEWNPCREPAIR_DLG])->InitDialogExceptNPCText();
			}
		}
		break;
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_NEWITEMREPAIRREQUESTINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_NewItemRepairRequestInfo* pclInfo = (cltGameMsgResponse_NewItemRepairRequestInfo*)pclMsg->cData;
	if ( NULL == pclInfo )
	{
		return;
	}

	id = cltCharCommon::CharUniqueInfo[pclInfo->m_siItemCharUnique];
	cltCharClient* pclChar_Request = pclCM->GetCharClient( id  );
	if( pclChar_Request == NULL )						return;

	cltCharClient* pclchar_me = pclCM->GetCharClient( 1 );
	if( pclchar_me == NULL )							return;

	pclChar_Request->bRepairShopSwitch = true;

	if( pclInfo->m_siType & NEWITEMREPAIR_INFO_BANNER )
	{
		NTCHARString512 kText;
		kText += pclItemManager->GetName( pclInfo->m_clItem.siUnique );
		kText += " (";
		kText += SI32ToString( pclInfo->m_clItem.GetDurability() ) ;
		kText += "/";
		kText += SI32ToString( pclInfo->m_clItem.GetMaxDurability() ) ;
		kText += " )\r";
		kText += GetTxtFromMgr( 40125 );
		kText += " : ";
		kText += SI64ToString( pclInfo->m_money );
		kText += GetTxtFromMgr( 611 );
		pclChar_Request->pclDrawChat->Set(pclDrawChatSurface, kText, true, PERSONALSHOP_BANNER_PREMIUM ,PSHOP_MODE_MAKEITEM, GetTxtFromMgr(40126) );
	}

	if( pclInfo->m_siType & NEWITEMREPAIR_INFO_ITEM )
	{
		CreateInterface( NNEWITEMMAKE_DLG );
		CNewMakeItemDlg* pclNewMakeItemDlg = (CNewMakeItemDlg*)m_pDialog[NNEWITEMMAKE_DLG];
		NItemRepairDlg* pclItemRepairDlg = NULL;
		if( pclNewMakeItemDlg )		pclItemRepairDlg = pclNewMakeItemDlg->NewItemRepair;

		if( pclItemRepairDlg )
		{
			if( pclInfo->m_siItemCharUnique == pclchar_me->GetCharUnique() )
			{
				pclItemRepairDlg->SetMode( 4 );
			}
			else
			{
				pclItemRepairDlg->SetMode( 3 );
			}

			pclItemRepairDlg->SetRepairInfo( pclInfo->m_siItemCharUnique, pclInfo->m_siItemPersonID, 
				pclInfo->m_siPos, &pclInfo->m_clItem,
				pclInfo->m_Material, 
				pclInfo->m_money );

			pclNewMakeItemDlg->ShowRepairTab();


			if(pclchar_me->GetCharUnique() != pclInfo->m_siItemCharUnique || pclchar_me->pclCI->GetPersonID()	!= pclInfo->m_siItemPersonID)
			{
				// 위 2사람과 해당 사항이 없을경우에는 창이 떠 있을 것이다. 그럴땐 한번더 창을 호출해서 창을 죽인다
				CreateInterface( NNEWITEMMAKE_DLG );
			}
			//if ( pclItemRepairDlg->GetMode() <= 0 ) // 모드 셋팅은 안했다는건 내구도랑 관련 없을 테니까 다시 크리에이티브 해서 창을 죽인다.
			//{
			//	CreateInterface( NNEWITEMMAKE_DLG );
			//}
		}
	}
}

