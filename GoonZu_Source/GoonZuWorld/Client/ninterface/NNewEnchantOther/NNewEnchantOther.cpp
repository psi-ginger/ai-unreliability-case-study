#include "client.h"


#include "NNewEnchantOther.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "Char\CharManager\CharManager.h"

#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"

#include "NInterface/NNewInventory/NNewInventory.h"
#include "../../Inventory/ItemInventoryBuffer.h"
#include "../NPrivateTradeDlg/PrivateTradeBaseInfo.h"
#include "..\..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"
#include "NInterface/NNewMakeItem/NNewMakeItemDlg.h"



extern cltCommonLogic* pclClient;


CNNewEnchantOther::CNNewEnchantOther()
{
	m_clOrgEnchantItem.Init();
	m_clMatEnchantItem.Init();
	m_siEnchantableItemList=0;
	m_siEnchantableItemCompare = 0;

	m_bShow = false;
	m_bMyAccept =false;
	for(SI32 i =0 ; i<3 ; i++)
	{
		m_clEleEnchantItem[i]=0;
		m_clEleEnchantItem[i].Init();
	}

	m_pPrivateTradeBufferMgr = NULL;
	m_pEachPersonTradeOrderInfo = NULL;
	m_pEachPersonTradeOrderInfo = new CEachPersonTradeOrderInfo();	
	m_siOtherCharUnique =0;

}
CNNewEnchantOther::~CNNewEnchantOther()
{
	SAFE_DELETE(m_pPrivateTradeBufferMgr);
	SAFE_DELETE(m_pEachPersonTradeOrderInfo);
}

void CNNewEnchantOther::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( !IsCreate() )
	{
		CInterfaceFile file;

		if (pclClient->siServiceArea == ConstServiceArea_Korea) {

			file.LoadFile("NInterface/Data/NNewEnchantOther/DLG_NewEnchantOther_K.ddf");


		}
		else
		{
			file.LoadFile("NInterface/Data/NNewEnchantOther/DLG_NewEnchantOther.ddf");

		}


		


		file.CreatePopupDialog( this, NNEWENCHANTOTHER_DLG, "dialog_Enchant", StatiCNNewEnchantOtherProc);



		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NENCHANTOTHER_LISTVIEW_ORIGINALITEM, this ), NENCHANTOTHER_LISTVIEW_ORIGINALITEM,   "listview_OriginalItem"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NENCHANTOTHER_LISTVIEW_MATERIALITEM, this ), NENCHANTOTHER_LISTVIEW_MATERIALITEM,   "listview_MaterialItem"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NENCHANTOTHER_LISTVIEW_ELEMENTITEM, this ), NENCHANTOTHER_LISTVIEW_ELEMENTITEM,   "listview_ElementItem"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NENCHANTOTHER_BUTTON_ENCHANTWATER, this ), NENCHANTOTHER_BUTTON_ENCHANTWATER,   "button_Water"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NENCHANTOTHER_BUTTON_ENCHANTFIRE, this ), NENCHANTOTHER_BUTTON_ENCHANTFIRE,   "button_Fire"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NENCHANTOTHER_BUTTON_ENCHANTWIND, this ), NENCHANTOTHER_BUTTON_ENCHANTWIND,   "button_Wind"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NENCHANTOTHER_BUTTON_ENCHANTGROUND, this ), NENCHANTOTHER_BUTTON_ENCHANTGROUND,   "button_Ground"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NENCHANTOTHER_BUTTON_STARTENCHANT, this ), NENCHANTOTHER_BUTTON_STARTENCHANT,   "button_StartEnchant"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NENCHANTOTHER_STATIC_HELPENCHANT, this ), NENCHANTOTHER_STATIC_HELPENCHANT,   "statictext_Fee"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NENCHANTOTHER_STATIC_ENCHANT, this ), NENCHANTOTHER_STATIC_ENCHANT,   "statictext_enchant"); 




		if ( m_pPrivateTradeBufferMgr == NULL )
		{
			m_pPrivateTradeBufferMgr = new CLibListMgr();
			m_pPrivateTradeBufferMgr->CreateMaxEmptyObj( MAX_PRIVATETRADE_OBJ_NUM, MAX_PRIVATETRADE_OBJ_BUFFER_SIZE );
		}

		m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_STARTENCHANT)->Enable(false);


	}

}

void CALLBACK CNNewEnchantOther::StatiCNNewEnchantOtherProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNewEnchantOther *pThis = (CNNewEnchantOther*)pControllerMgr;
	pThis->NNewEnchantOtherProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNewEnchantOther::NNewEnchantOtherProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

	SI32 id = 1;

	cltClient *pclclient = (cltClient *)pclClient;
	switch( nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			//DeleteDialog();
			PushCancel();
		}
		break;		
	case NENCHANTOTHER_LISTVIEW_ORIGINALITEM:
		{
			//if(!g_SoundOnOffDlg.m_bFullScreenMode)
			//{
			//	switch( nEvent )
			//	{

			//	}
			//}				

		}
		break;
	case NENCHANTOTHER_LISTVIEW_MATERIALITEM:
		{
			//if(!g_SoundOnOffDlg.m_bFullScreenMode)
			//{
			//	switch( nEvent )
			//	{

			//	}
			//}	

		}
		break;	
	case NENCHANTOTHER_LISTVIEW_ELEMENTITEM:
		{

		}
		break;	

	case NENCHANTOTHER_BUTTON_STARTENCHANT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					CNNewInventory* newinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
					CItemInventoryBuffer* m_pBuffer = newinventory->m_pBuffer;

					SI16 MakeItemNum = 1;
					//if(m_siEnchantableItemCompare != m_siEnchantableItemList)
					//	return;
					
					SI32 siMaxGrade = pclClient->pclItemManager->GetItemGrade(m_clOrgEnchantItem.siUnique);
					SI32 siOrgGrade = m_clOrgEnchantItem.Element.siPower;

					if( siMaxGrade <= siOrgGrade )
					{
						TCHAR * pTitle = GetTxtFromMgr(5912);
						TCHAR * pText = GetTxtFromMgr(5933);
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						return;                				
					}		

					if(IsCheckSkill(m_clOrgEnchantItem) == false)
					{
						 TCHAR * pTitle = GetTxtFromMgr(5912);
						 TCHAR * pText = GetTxtFromMgr(6044);

						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitle, pText );
						return;
					}

					m_pBuffer->Push( INV_ENCHANT_ITEM, m_siEnchantableItemList, MakeItemNum );

					m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_STARTENCHANT)->Enable(false);

				}
				break;
			}
		}
		break;	
	}
	return;
}

void CNNewEnchantOther::EnableButton(bool bWater, bool bFire, bool bWind, bool bGround)
{
	//if(m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_ORIGINALITEM)->GetSelectedItemIndex() < 0)
	//	 return;
	m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_ENCHANTWATER)->Enable(bWater)	;
	m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_ENCHANTFIRE)->Enable(bFire);
	m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_ENCHANTWIND)->Enable(bWind);
	m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_ENCHANTGROUND)->Enable(bGround);
}



void CNNewEnchantOther::ShowOrgEnchantItem(cltItem cltOrgItem)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	m_siEnchantableItemList = cltOrgItem.siUnique;
	m_clOrgEnchantItem.Init();
	m_clOrgEnchantItem.Set(&cltOrgItem);


	cltGameMsgResponse_EnchantPrice clGameMsgResponse_EnchantPrice(cltOrgItem.siUnique);
	cltMsg clMsg(GAMEMSG_REQUEST_ENCHANTOTHERPRICE, sizeof(clGameMsgResponse_EnchantPrice), (BYTE*)&clGameMsgResponse_EnchantPrice);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);




	//memcpy(&m_clOrgEnchantItem,&cltOrgItem, sizeof(cltItem));
	m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_ORIGINALITEM)->DeleteAllItems();

	if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clOrgEnchantItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
	{
		char *ItemName = (char*)pclClient->pclItemManager->GetName( m_clOrgEnchantItem.siUnique );

		if ( ItemName )
		{
			TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
//			SI32 siItemStatus = 0 ;
//			pclClient->pclItemManager->GetExplain( &m_clOrgEnchantItem, ToolTipMsg ,&siItemStatus);
			pclClient->pclItemManager->GetExplain( &m_clOrgEnchantItem, ToolTipMsg );
			SI32 size = sizeof(ToolTipMsg);
			SI32 ddd = strlen(ToolTipMsg);


			stListViewItemData ListViewItemData;
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 , m_clOrgEnchantItem.siItemNum, ItemName, ToolTipMsg );
			m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_ORIGINALITEM)->InsertItem(&ListViewItemData);				
		}
	}

	m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_ORIGINALITEM)->Refresh();

	pclclient->pclCM->CR[1]->siElementType = cltOrgItem.Element.siType;

	if(!(pclclient->pclCM->CR[1]->siElementType % 2 ))
		pclclient->pclCM->CR[1]->siElementType = cltOrgItem.Element.siType - 1;

	if(cltOrgItem.Element.siType == ENCHANT_TYPE_FIRE_A || cltOrgItem.Element.siType == ENCHANT_TYPE_FIRE_D)
		EnableButton(false, true,false,false);  
	else if(cltOrgItem.Element.siType == ENCHANT_TYPE_WATER_A || cltOrgItem.Element.siType == ENCHANT_TYPE_WATER_D)
		EnableButton(true, false,false,false);  
	else if(cltOrgItem.Element.siType == ENCHANT_TYPE_WIND_A || cltOrgItem.Element.siType == ENCHANT_TYPE_WIND_D)
		EnableButton(false, false,true,false);  
	else if(cltOrgItem.Element.siType == ENCHANT_TYPE_GROUND_A || cltOrgItem.Element.siType == ENCHANT_TYPE_GROUND_D)
		EnableButton(false, false,false,true);  

}
void CNNewEnchantOther::ShowMatEnchantItem(cltItem cltMatItem)
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	m_siEnchantableItemCompare = cltMatItem.siUnique;


	memcpy(&m_clMatEnchantItem,&cltMatItem, sizeof(cltItem));
	m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_MATERIALITEM)->DeleteAllItems();

	if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clMatEnchantItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
	{
		char *ItemName = (char*)pclClient->pclItemManager->GetName( m_clMatEnchantItem.siUnique );

		if ( ItemName )
		{
			char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
//			SI32 siItemStatus = 0 ;
//			pclClient->pclItemManager->GetExplain( &m_clMatEnchantItem, ToolTipMsg ,&siItemStatus);
			pclClient->pclItemManager->GetExplain( &m_clMatEnchantItem, ToolTipMsg );
			SI32 size = sizeof(ToolTipMsg);
			SI32 ddd = strlen(ToolTipMsg);


			stListViewItemData ListViewItemData;
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 , m_clMatEnchantItem.siItemNum, ItemName, ToolTipMsg );
			m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_MATERIALITEM)->InsertItem(&ListViewItemData);				
		}
	}

	m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_MATERIALITEM)->Refresh();
}
void CNNewEnchantOther::ShowEleEnchantItem(cltItem cltEleItem[])
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;


	memcpy(m_clEleEnchantItem,cltEleItem, sizeof(cltItem)*3);
	m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_ELEMENTITEM)->DeleteAllItems();

	for(SI32 i =0; i<3 ; i++)
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clEleEnchantItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			char *ItemName = (char*)pclClient->pclItemManager->GetName( m_clEleEnchantItem[ i ].siUnique );

			if ( ItemName )
			{
				char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
//				SI32 siItemStatus = 0 ;
//				pclClient->pclItemManager->GetExplain( &m_clEleEnchantItem[ i ], ToolTipMsg ,&siItemStatus);
				pclClient->pclItemManager->GetExplain( &m_clEleEnchantItem[ i ], ToolTipMsg );
				SI32 size = sizeof(ToolTipMsg);
				SI32 ddd = strlen(ToolTipMsg);


				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i , m_clEleEnchantItem[ i ].siItemNum, ItemName, ToolTipMsg );
				m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_ELEMENTITEM)->InsertItem(&ListViewItemData);				
			}
		}
	}
	m_InterfaceFactory.GetListView(NENCHANTOTHER_LISTVIEW_ELEMENTITEM)->Refresh();	
}

void CNNewEnchantOther::Hide()
{
	CDialogController::Show( false );	
}


void CNNewEnchantOther::Show( SI32 siOtherCharUnique,SI16 mode, SI16 Level, char *Name, char *AccountID, SI16 siPrivateTradeOrderMgrIndex, SI16 siPrivateTradeOrderCountIndex, SI32 OtherCharLevel )
{
	// PrivateTrdeOrderMgr로 부터 할당받지 못한 경우
	// siOtherCharUnique 가 잘못된 값인 경우
	cltClient *pclclient = (cltClient *)pclClient;


	if ( siPrivateTradeOrderMgrIndex < 1 || siOtherCharUnique < 1 )		
	{
		return;
	}



	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	m_siOtherCharUnique = siOtherCharUnique;

	m_siOtherCharLevel = Level;


	m_siPrivateTrdeOrderMgrIndex = siPrivateTradeOrderMgrIndex;
	m_siPrivateTrdeOrderCountIndex = siPrivateTradeOrderCountIndex;

	char Buffer[256 ] = "";





	if ( Name != NULL && strlen( Name ) > 0 )
	{
		TCHAR* pText = GetTxtFromMgr(6042);
		sprintf( Buffer, pText, Name, OtherCharLevel );
		SetTitle(Buffer);
		MStrCpy( m_strOtherCharName, Name, MAX_PLAYER_NAME );
	}
	else
	{
		
	}




	// 내 계정 아이디
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID == NULL || strlen ( pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID ) < 1 )
	{
		return;
	}



	// 다른 사람 계정 아이디
	if ( AccountID == NULL || strlen( AccountID ) < 1 )
	{
		return;
	}

	//m_siAccpetCounter = 0;

	//m_siPrivateTrdeOrderMgrIndex = siPrivateTradeOrderMgrIndex;
	//m_siPrivateTrdeOrderCountIndex = siPrivateTradeOrderCountIndex;



	//m_siMyPrivateTradeListSelectedIndex = -1;




	m_bMyAccept = false;
	//m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_STARTENCHANT)->Enable(true);
	SetActionTimer(200);		
	m_bShow = true;
	CDialogController::Show( true );

	return ;

}

bool CNNewEnchantOther::IsShow()
{
	return m_bShow;
}


void CNNewEnchantOther::PrivateTradeEnd( SI32 siResult, 
										SI32 RecvStockVillageUnique, SI32 RecvStockNum, 
										SI32 SendStockVillageUnique, SI32 SendStockNum, 
										SI32 RecvHouseVillageUnique, SI32 RecvHouseUnitSlot, 
										cltRentContract *pclRecvRentContract,
										SI32 SendHouseVillageUnique, SI32 SendHouseUnitSlot, 
										SI16 HorseIndex, cltHorse *pclHorse )
{


	DeleteDialog();
	return;
}


void CNNewEnchantOther::PushCancel()
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_Cancel sCancel( m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex );

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sCancel, sCancel.sHeader.uiSize );
	}

	return;
}

bool CNNewEnchantOther::Pop( BYTE *pData )
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp )
	{
		UI32 uiDataSize;
		pTemp->GetData( (BYTE*)pData, &uiDataSize );
		return true;
	}

	return false;
}

void CNNewEnchantOther::SetUpdateButton(bool updatebutton)
{
	//m_InterfaceFactory.GetButton(NENCHANTOTHER_STATIC_UNDATEMAT)->Enable(updatebutton);
}

void CNNewEnchantOther::SetStartButton(bool Startbutton)
{
	m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_STARTENCHANT)->Enable(Startbutton);
}
bool CNNewEnchantOther::GetStartButton()
{
	bool bButton =  m_InterfaceFactory.GetButton(NENCHANTOTHER_BUTTON_STARTENCHANT)->IsEnable();
	return bButton;
}
bool CNNewEnchantOther::GetUpdateButton()
{
	bool bButton =  m_InterfaceFactory.GetButton(NENCHANTOTHER_STATIC_UNDATEMAT)->IsEnable();
	return bButton;
}

SI32 CNNewEnchantOther::GetMatOtherCharUnique()
{
	return m_siOtherCharUnique;
}
void CNNewEnchantOther::SetPriceText(SI64 sifee)
{
	cltClient *pclclient = (cltClient *)pclClient;
	// 해당 물품의 시세를 구한다. 
	//cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice(cltOrgItem.siUnique);
	sifee = (SI64)(sifee * 0.05);
	TCHAR buf[128] = TEXT(" ");
	TCHAR * pText = GetTxtFromMgr(6043);

	sprintf(buf,pText, sifee);
 	m_InterfaceFactory.GetStatic(NENCHANTOTHER_STATIC_HELPENCHANT)->SetBKColorUse(true);
	m_InterfaceFactory.GetStatic(NENCHANTOTHER_STATIC_HELPENCHANT)->SetText(buf, DT_WORDBREAK | DT_CENTER);
	

	SI64 mymoney =  pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney();

	if(sifee > mymoney )
	{
		if(GetStartButton() ==true)	return;
		
        TCHAR * pTitle = GetTxtFromMgr(1260);
		TCHAR * pText = GetTxtFromMgr(5925);
		pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitle, pText);
		PushCancel();
	}


}
bool CNNewEnchantOther::IsCheckSkill(cltItem clOutputItem)
{


	SI32 siItemType = pclClient->pclItemManager->GetItemType(clOutputItem.siUnique);
	bool simakeflag=true;
	cltPItemInfo*  pclmaterialinv = &pclClient->pclCM->CR[1]->pclCI->clCharItem;
	stEnchantNeed materialunique1;
	pclClient->pclItemManager->GetEnchantNeedItem(clOutputItem, &materialunique1);

	switch(siItemType)
	{
	case ITEMTYPE_SWORD:	
		{
			SI32 skillunique  = SKILL_MAKESWORD1;

			SI32 level  = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;
		}
		break;
	case ITEMTYPE_SPEAR:
		{
			SI32 skillunique  = SKILL_MAKESPEAR1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_STAFF:			
		{
			SI32 skillunique  = SKILL_MAKESTAFF1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_AXE:			
		{
			SI32 skillunique  = SKILL_MAKEAXE1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_BOW:			
		{
			SI32 skillunique  = SKILL_MAKEBOW1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;

		}
		break;
	case ITEMTYPE_GUN:
		{
			SI32 skillunique  = SKILL_MAKEGUN1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;		
	case ITEMTYPE_ARMOUR:
		{
			SI32 skillunique  = SKILL_MAKEARMOUR1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;	
	case ITEMTYPE_DRESS:
		{
			SI32 skillunique  = SKILL_MAKEDRESS1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_BELT:
		{
			SI32 skillunique  = SKILL_MAKEBELT1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_SHOES:
		{
			SI32 skillunique  = SKILL_MAKESHOES1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_HELMET:
		{
			SI32 skillunique  = SKILL_MAKEHELMET1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;


		}
		break;
	case ITEMTYPE_HAT:
		{
			SI32 skillunique  = SKILL_MAKEHAT1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;		


		}			
		break;
	case ITEMTYPE_CRYSTAL:
		{
			SI32 skillunique  = SKILL_ARTIFACT;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;		


		}			
		break;
		/*
	case ITEMTYPE_RING:
		{
			SI32 skillunique  = SKILL_MAKERING1;
			SI32 level  =pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclmaterialinv->clItem[0], 1);
			SI32 MaxLevel = materialunique1.siNeedSkill;
			if(level < MaxLevel) simakeflag = false;		
		}
	*/
	}



	return simakeflag;
}
