#include "client.h"


#include "NNewMakeItemDlg.h"
#include "NNewEnchantChild.h"

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

#include "ninterface/NNewInventory/NNewInventory.h"
#include "../../Inventory/ItemInventoryBuffer.h"
#include "NInterface/NHintDlg/NHintDlg.h"


extern cltCommonLogic* pclClient;


CNNewEnchantChild::CNNewEnchantChild()
{
	m_clOrgEnchantItem.Init();
	m_clMatEnchantItem.Init();
	m_siEnchantableItemList=0;
	m_siEnchantableItemCompare = 0;
	for(SI32 i =0 ; i<3 ; i++)
	{
		m_clEleEnchantItem[i]=0;
		m_clEleEnchantItem[i].Init();
	}

	m_siOtherCharUnique =0;



}
CNNewEnchantChild::~CNNewEnchantChild()
{
}

void CNNewEnchantChild::Create(CControllerMgr *pParent)
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NNewMakeItem/DLG_NewEnchant.ddf");
		file.CreateChildDialog( this, NNEWENCHANTCHILD_DLG, "dialog_Enchant",StatiCNNewEnchantChildProc, pParent);


		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NEWMAKE_LISTVIEW_ORIGINALITEM, this ), NEWMAKE_LISTVIEW_ORIGINALITEM,   "listview_OriginalItem"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NEWMAKE_LISTVIEW_MATERIALITEM, this ), NEWMAKE_LISTVIEW_MATERIALITEM,   "listview_MaterialItem"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW, NEWMAKE_LISTVIEW_ELEMENTITEM, this ), NEWMAKE_LISTVIEW_ELEMENTITEM,   "listview_ElementItem"); 

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON_ENCHANTWATER, this ), NEWMAKE_BUTTON_ENCHANTWATER,   "button_Water"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON_ENCHANTFIRE, this ), NEWMAKE_BUTTON_ENCHANTFIRE,   "button_Fire"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON_ENCHANTWIND, this ), NEWMAKE_BUTTON_ENCHANTWIND,   "button_Wind"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON_ENCHANTGROUND, this ), NEWMAKE_BUTTON_ENCHANTGROUND,   "button_Ground"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON_STARTENCHANT, this ), NEWMAKE_BUTTON_STARTENCHANT,   "button_StartEnchant"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON_WHATENCHANT, this ), NEWMAKE_BUTTON_WHATENCHANT,   "button_WhatEnchant"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEWMAKE_STATIC_ENCHANT, this ), NEWMAKE_STATIC_ENCHANT,   "statictext_enchant"); 
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NENCHANTOTHER_STATIC_UNDATEMAT, this ), NENCHANTOTHER_STATIC_UNDATEMAT,   "button_UpdateMaterial"); 

		//초기화
		SetInitInchant();

		cltGameMsgRequest_EnchantItem_SelectedIndex clGameMsgRequest_EnchantItem_SelectedIndex(0,0,0,0,0,0 );
		cltMsg clMsg(GAMEMSG_REQUEST_ENCHANTITEMINVINDEX, sizeof(clGameMsgRequest_EnchantItem_SelectedIndex), (BYTE*)&clGameMsgRequest_EnchantItem_SelectedIndex);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		cltGameMsgRequest_Enchant_StartOrEnd clGameMsgRequest_Enchant_StartOrEnd(true);
		cltMsg clMsg1(GAMEMSG_REQUEST_ENCHANTITEMSTARTOREND, sizeof(clGameMsgRequest_Enchant_StartOrEnd), (BYTE*)&clGameMsgRequest_Enchant_StartOrEnd);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg1);
	}

}

void CALLBACK CNNewEnchantChild::StatiCNNewEnchantChildProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNewEnchantChild *pThis = (CNNewEnchantChild*)pControllerMgr;
	pThis->NNewEnchantChildProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNewEnchantChild::NNewEnchantChildProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

	SI32 id = 1;

	cltClient *pclclient = (cltClient *)pclClient;
	switch( nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
		}
		break;
	case NEWMAKE_BUTTON_STARTENCHANT:
		{
			SI32 Orginx = pclclient->pclCM->CR[id]->siOrgInvEnchantInx;
			SI32 Matinx = pclclient->pclCM->CR[id]->siMatInvEnchantInx;
			SI32 EleInx1 = pclclient->pclCM->CR[id]->siElementEnchantInx[0];
			SI32 EleInx2 = pclclient->pclCM->CR[id]->siElementEnchantInx[1];
			SI32 EleInx3 = pclclient->pclCM->CR[id]->siElementEnchantInx[2];
			SI32 ElementType = pclclient->pclCM->CR[id]->siElementType;
			TCHAR * pTitleText = GetTxtFromMgr(5912); // 기본값으로 조합실패 타이틀을 준비해둔다.
			TCHAR * pExplainText = NULL;
			SI32 siMaxGrade = pclClient->pclItemManager->GetItemGrade(m_clOrgEnchantItem.siUnique);
			SI32 siOrgGrade = pclClient->pclItemManager->GetEnchantGrade(m_clOrgEnchantItem.Element.siPower);
			
			siMaxGrade = 2;	//2등급까지만 인첸트 가능

			if( siMaxGrade <= siOrgGrade )
			{
				pExplainText = GetTxtFromMgr(5933);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText );
				return;                				
			}			
			if(pclClient->pclItemManager->IsCheckSkill(m_clOrgEnchantItem, 1) == false)
			{	
				pExplainText = GetTxtFromMgr(5911);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText );
				return;
			}

			if(m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWATER)->IsEnable() 
				&& m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTFIRE)->IsEnable() 
				&& m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWIND)->IsEnable() 
				&& m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTGROUND)->IsEnable() 
				)
			{
				pExplainText = GetTxtFromMgr(5913);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText);
				return;				
			}	
			if(pclclient->m_pDialog[NNEWENCHANTOTHER_DLG])
			{
				pExplainText = GetTxtFromMgr(5914);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText);

			}
			if(Orginx == 0 )
			{
				pExplainText = GetTxtFromMgr(5915);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText);
				return;	

			}
			if( pclClient->bEnchantSwitch == false)
			{			 
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, "ENCHANT_COMMING_SOON");
				m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_STARTENCHANT)->Enable(false);
				return;
			}
			else
			{
				m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_STARTENCHANT)->Enable(true);
			}

			if(IsCheckMaterial()==false)
			{

				return;
			}

			//재료가 제대로 있는지 체크해주기 



			cltGameMsgRequest_EnchantItem_SelectedIndex clGameMsgRequest_EnchantItem_SelectedIndex(Orginx,Matinx,EleInx1,EleInx2,EleInx3, ElementType);
			cltMsg clMsg(GAMEMSG_REQUEST_ENCHANTITEMINVINDEX, sizeof(clGameMsgRequest_EnchantItem_SelectedIndex), (BYTE*)&clGameMsgRequest_EnchantItem_SelectedIndex);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);


			CNNewInventory* newinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
			CItemInventoryBuffer* m_pBuffer = newinventory->m_pBuffer;

			SI16 MakeItemNum = 1;
			//if(m_siEnchantableItemCompare != m_siEnchantableItemList)
			//	return;

			m_pBuffer->Push( INV_ENCHANT_ITEM, m_siEnchantableItemList, MakeItemNum );

		}
		break;
	case NEWMAKE_BUTTON_ENCHANTWATER:
		{
			EnableButton(true, false,false,false);  
		}
		break;
	case NEWMAKE_BUTTON_ENCHANTFIRE:
		{
			EnableButton(false,true,false,false);
		}
		break;
	case NEWMAKE_BUTTON_ENCHANTWIND:
		{
			EnableButton(false,false,true,false);
		}
		break;
	case NEWMAKE_BUTTON_ENCHANTGROUND:
		{
			EnableButton(false,false,false,true);
		}
		break;
	case NEWMAKE_BUTTON_WHATENCHANT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
					{
						pclclient->CreateInterface( NHINT_DLG );
					}
					((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show("enchant");

				}
			}

		}
		break;

	case NENCHANTOTHER_STATIC_UNDATEMAT:
		{

			SI32 id = 1;	

			SI32 siOrgindex = pclclient->pclCM->CR[id]->siOrgInvEnchantInx ;
			SI32 siMatindex = pclclient->pclCM->CR[id]->siMatInvEnchantInx;
			SI32 siEleindex1 = pclclient->pclCM->CR[id]->siElementEnchantInx[0];
			SI32 siEleindex2 = pclclient->pclCM->CR[id]->siElementEnchantInx[1];
			SI32 siEleindex3 = pclclient->pclCM->CR[id]->siElementEnchantInx[2];			
			TCHAR * pTitleText = GetTxtFromMgr(5912); // 기본값으로 조합실패 타이틀을 준비해둔다.
			TCHAR * pExplainText = NULL;

			if(m_siOtherCharUnique ==0)
			{
				pExplainText = GetTxtFromMgr(5916);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText);
				return;
			}

			if( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[siOrgindex].Element.siPower < 30)
			{

				pExplainText = GetTxtFromMgr(5917);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText);
				return;
			}


			if( IsCheckMaterial()== false)
			{
				return;
			}
			if(siOrgindex ==0 || siMatindex ==0)
			{
				return;
			}

			cltGameMsgRequest_EnchantOther clGameMsgRequest_EnchantOther(siOrgindex,siMatindex,siEleindex1,siEleindex2,siEleindex3, m_siOtherCharUnique);
			cltMsg clMsg(GAMEMSG_REQUEST_ENCHANTOTHERSHOW, sizeof(clGameMsgRequest_EnchantOther), (BYTE*)&clGameMsgRequest_EnchantOther);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		}
		break;

	case NEWMAKE_LISTVIEW_ORIGINALITEM:
		{
			if(!g_SoundOnOffDlg.m_bFullScreenMode)
			{
				switch( nEvent )
				{
				case EVENT_LISTVIEW_LBUTTONUP:
					pclclient->pclCM->CR[id]->siOrgInvEnchantInx = pclclient->pclCM->CR[id]->siMouseInvItem;
					m_clOrgEnchantItem.Set(&pclclient->pclCM->CR[id]->clMouseInvItem);
					ShowOrgEnchantItem(m_clOrgEnchantItem);
					break;
				}
			}				
			OriginalListViewMsgProc( m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ORIGINALITEM), 0 , nEvent );		
		}
		break;
	case NEWMAKE_LISTVIEW_MATERIALITEM:
		{
			if(!g_SoundOnOffDlg.m_bFullScreenMode)
			{
				switch( nEvent )
				{
				case EVENT_BUTTON_CLICKED:
					pclclient->pclCM->CR[id]->siMatInvEnchantInx = pclclient->pclCM->CR[id]->siMouseInvItem;
					m_clMatEnchantItem.Set(&pclclient->pclCM->CR[id]->clMouseInvItem);
					ShowMatEnchantItem(m_clMatEnchantItem);
					break;
				}
			}	
			MaterialListViewMsgProc( m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM), 0 , nEvent );		
		}
		break;	
	case NEWMAKE_LISTVIEW_ELEMENTITEM:
		{
			if(!g_SoundOnOffDlg.m_bFullScreenMode)
			{
				switch( nEvent )
				{
				case EVENT_BUTTON_CLICKED:
					for(SI32 i=0;i < 3;i++)
					{
						if(pclclient->pclCM->CR[id]->siElementEnchantInx[i] ==0)
						{
							pclclient->pclCM->CR[id]->siElementEnchantInx[i] =  pclclient->pclCM->CR[id]->siMouseInvItem;
							m_clEleEnchantItem[i].Set(&pclclient->pclCM->CR[id]->clMouseInvItem);
							break;
						}
					}
					break;
				}
			}	
			ElementListViewMsgProc( m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM), 0 , nEvent );		
		}
		break;	
	}
	return ;
}

void CNNewEnchantChild::EnableButton(bool bWater, bool bFire, bool bWind, bool bGround)
{
	//if(m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ORIGINALITEM)->GetSelectedItemIndex() < 0)
	//	 return;
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWATER)->Enable(bWater)	;
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTFIRE)->Enable(bFire);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWIND)->Enable(bWind);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTGROUND)->Enable(bGround);

	SI32 id = 1;
	if(m_clOrgEnchantItem.Element.siType!=0 && pclClient->pclItemManager->GetItemType(m_clOrgEnchantItem.siUnique) ==ITEMTYPE_RING )
	{
		pclClient->pclCM->CR[id]->siElementType =m_clOrgEnchantItem.Element.siType;
	}
	else if(bWater)
		pclClient->pclCM->CR[id]->siElementType = ENCHANT_TYPE_WATER_A;
	else if(bFire)
		pclClient->pclCM->CR[id]->siElementType = ENCHANT_TYPE_FIRE_A;
	else if(bWind)
		pclClient->pclCM->CR[id]->siElementType = ENCHANT_TYPE_WIND_A;
	else if(bGround)
		pclClient->pclCM->CR[id]->siElementType = ENCHANT_TYPE_GROUND_A;
	else
	{
		pclClient->pclCM->CR[id]->siElementType = ENCHANT_TYPE_DEFAULT;
	}
}


void CNNewEnchantChild::OriginalListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent )
{
	if ( pListView == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	// 쇼하이드 방식이라 무조건 만들어져 있다!!
	CListView* m_pInventoryListView = ((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_pInventoryListView;
	SI16 m_siCurrentInventoryIndex =((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_siCurrentInventoryIndex;

	static TempCounter =0 ;

	switch( nEvent )
	{
	case EVENT_LISTVIEW_RBUTTONUP:
		{

			SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				return;

			if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
			{
				CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
				equip->clOrgEnchantItem.Init();	
				equip->m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siOrgInvEnchantInx,false);
				equip->m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siMatInvEnchantInx,false);
				if(pclClient->pclCM->CR[1]->siElementEnchantInx[0] > 0)			
					equip->m_pInventoryListView->SetItemDrawAlphaOption(pclClient->pclCM->CR[1]->siElementEnchantInx[0], false);
				if(pclClient->pclCM->CR[1]->siElementEnchantInx[1] > 0)			
					equip->m_pInventoryListView->SetItemDrawAlphaOption(pclClient->pclCM->CR[1]->siElementEnchantInx[1], false);
				if(pclClient->pclCM->CR[1]->siElementEnchantInx[2] > 0)			
					equip->m_pInventoryListView->SetItemDrawAlphaOption(pclClient->pclCM->CR[1]->siElementEnchantInx[2], false);
			}
			m_clOrgEnchantItem.Init();		
			m_clMatEnchantItem.Init();
			m_clEleEnchantItem[0].Init();
			m_clEleEnchantItem[1].Init();
			m_clEleEnchantItem[2].Init();
			
			pListView->DeleteItem(SelectedIndex);
			pclClient->pclCM->CR[1]->siOrgInvEnchantInx =0;
			m_siEnchantableItemList=0; 

			m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM)->DeleteAllItems();
			m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM)->DeleteAllItems();
			
			pclClient->pclCM->CR[1]->siMatInvEnchantInx = 0;
			pclClient->pclCM->CR[1]->siElementEnchantInx[0] = 0;
			pclClient->pclCM->CR[1]->siElementEnchantInx[1] = 0;
			pclClient->pclCM->CR[1]->siElementEnchantInx[2] = 0;

			EnableButton(true, true,true,true);  



		}
		break;
	}
}


void CNNewEnchantChild::MaterialListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent )
{
	if ( pListView == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	// 쇼하이드 방식이라 무조건 만들어져 있다!!
	CListView* m_pInventoryListView = ((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_pInventoryListView;
	SI16 m_siCurrentInventoryIndex =((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_siCurrentInventoryIndex;


	switch( nEvent )
	{
	case EVENT_LISTVIEW_RBUTTONUP:
		{
			SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				return;



			if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
			{
				CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
                equip->clMatEnchantItem.Init();						
				equip->m_pInventoryListView->SetItemDrawAlphaOption(pclClient->pclCM->CR[1]->siMatInvEnchantInx,false);
			}	
			m_clMatEnchantItem.Init();		

			pListView->DeleteItem(SelectedIndex);

			pclClient->pclCM->CR[1]->siMatInvEnchantInx =0;
			m_siEnchantableItemCompare =0;


		}
		break;
	}
}


void CNNewEnchantChild::ElementListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent )
{
	if ( pListView == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	// 쇼하이드 방식이라 무조건 만들어져 있다!!
	CListView* m_pInventoryListView = ((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_pInventoryListView;
	SI16 m_siCurrentInventoryIndex =((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_siCurrentInventoryIndex;


	switch( nEvent )
	{
	case EVENT_LISTVIEW_RBUTTONUP:
		{
			SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				return;
			if(SelectedIndex >= 3)	// equip->clEleEnchantItem[3] 이다
				return;

			if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
			{
				CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
				equip->clEleEnchantItem[SelectedIndex].Init();
				equip->m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siElementEnchantInx[SelectedIndex],false);
			}	
			m_clEleEnchantItem[SelectedIndex].Init();		
			pclclient->pclCM->CR[1]->siElementEnchantInx[SelectedIndex] = 0;
			pListView->DeleteItem(SelectedIndex);



		}
		break;
	}

	return;
}


void CNNewEnchantChild::ShowOrgEnchantItem(cltItem cltOrgItem)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;

	m_siEnchantableItemList = cltOrgItem.siUnique;
	m_clOrgEnchantItem.Init();
	m_clOrgEnchantItem.Set(&cltOrgItem);


	//memcpy(&m_clOrgEnchantItem,&cltOrgItem, sizeof(cltItem));
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ORIGINALITEM)->DeleteAllItems();

	if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clOrgEnchantItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
	{
		char *ItemName = (char*)pclClient->pclItemManager->GetName( m_clOrgEnchantItem.siUnique );

		if ( ItemName )
		{
			char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
			SI32 siItemStatus = 0 ;
			pclClient->pclItemManager->GetExplain( &m_clOrgEnchantItem, ToolTipMsg ,&siItemStatus);
			SI32 size = sizeof(ToolTipMsg);
			SI32 ddd = strlen(ToolTipMsg);


			stListViewItemData ListViewItemData;
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 , m_clOrgEnchantItem.siItemNum, ItemName, ToolTipMsg );
			m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ORIGINALITEM)->InsertItem(&ListViewItemData);				
		}
	}

	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ORIGINALITEM)->Refresh();




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
void CNNewEnchantChild::ShowMatEnchantItem(cltItem cltMatItem)
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	m_siEnchantableItemCompare = cltMatItem.siUnique;


	memcpy(&m_clMatEnchantItem,&cltMatItem, sizeof(cltItem));
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM)->DeleteAllItems();

	if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clMatEnchantItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
	{
		char *ItemName = (char*)pclClient->pclItemManager->GetName( m_clMatEnchantItem.siUnique );

		if ( ItemName )
		{
			char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
			SI32 siItemStatus = 0 ;
			pclClient->pclItemManager->GetExplain( &m_clMatEnchantItem, ToolTipMsg ,&siItemStatus);
			SI32 size = sizeof(ToolTipMsg);
			SI32 ddd = strlen(ToolTipMsg);


			stListViewItemData ListViewItemData;
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0 , m_clMatEnchantItem.siItemNum, ItemName, ToolTipMsg );
			m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM)->InsertItem(&ListViewItemData);				
		}
	}

	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM)->Refresh();
}
void CNNewEnchantChild::ShowEleEnchantItem(cltItem cltEleItem[])
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;


	memcpy(m_clEleEnchantItem,cltEleItem, sizeof(cltItem)*3);
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM)->DeleteAllItems();

	for(SI32 i =0; i<3 ; i++)
	{
		if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clEleEnchantItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			char *ItemName = (char*)pclClient->pclItemManager->GetName( m_clEleEnchantItem[ i ].siUnique );

			if ( ItemName )
			{
				char ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = "";
				SI32 siItemStatus = 0 ;
				pclClient->pclItemManager->GetExplain( &m_clEleEnchantItem[ i ], ToolTipMsg ,&siItemStatus);
				SI32 size = sizeof(ToolTipMsg);
				SI32 ddd = strlen(ToolTipMsg);


				stListViewItemData ListViewItemData;
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i , m_clEleEnchantItem[ i ].siItemNum, ItemName, ToolTipMsg );
				m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM)->InsertItem(&ListViewItemData);				
			}
		}
	}
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM)->Refresh();	
}


void CNNewEnchantChild::Show()
{	
	CDialogController::Show( true );
}

void CNNewEnchantChild::Hide()
{
	CDialogController::Show( false );	
}

bool CNNewEnchantChild::IsCheckMaterial()
{
	cltClient *pclclient = (cltClient *)pclClient;
	stEnchantNeed EnchantNeed;
	EnchantNeed = pclClient->pclItemManager->GetEnchantNeedItem(m_clOrgEnchantItem);   

	SI32 siGrade = pclClient->pclItemManager->GetEnchantGrade(m_clOrgEnchantItem.Element.siPower);

	if(siGrade ==1 || siGrade ==4 || siGrade ==5 || siGrade ==6 || siGrade ==2 || siGrade == 3)
	{
		// 1번 속성석 슬롯의 아이템과 재료비교
		if((EnchantNeed.siElementUnique[0] ==m_clMatEnchantItem.siUnique 
			&& EnchantNeed.siElementUnique[1] == m_clEleEnchantItem[0].siUnique) 
			&& EnchantNeed.siElementUnique[0]!=0
			&&EnchantNeed.siElementUnique[1]!=0
			&& EnchantNeed.siElementNum[1] <= m_clEleEnchantItem[0].siItemNum )
			return true;
		// 2번 속성석 슬롯의 아이템과 재료비교
		if((EnchantNeed.siElementUnique[0] ==m_clMatEnchantItem.siUnique 
			&& EnchantNeed.siElementUnique[1] == m_clEleEnchantItem[1].siUnique) 
			&& EnchantNeed.siElementUnique[0]!=0
			&&EnchantNeed.siElementUnique[1]!=0
			&& EnchantNeed.siElementNum[1] <= m_clEleEnchantItem[1].siItemNum )
			return true;
		// 3번 속성석 슬롯의 아이템과 재료비교
		if((EnchantNeed.siElementUnique[0] ==m_clMatEnchantItem.siUnique 
			&& EnchantNeed.siElementUnique[1] == m_clEleEnchantItem[2].siUnique) 
			&& EnchantNeed.siElementUnique[0]!=0
			&&EnchantNeed.siElementUnique[1]!=0
			&& EnchantNeed.siElementNum[1] <= m_clEleEnchantItem[2].siItemNum )
			return true;



	}
	else if(siGrade ==0  &&EnchantNeed.siElementUnique[0]!=0 &&EnchantNeed.siElementUnique[0] == m_clMatEnchantItem.siUnique  )
	{
		return true;
	}	

	TCHAR * pTitleText = GetTxtFromMgr(5912); //  조합실패 타이틀을 준비해둔다.
	TCHAR * pExplainText = NULL;

	if(EnchantNeed.siElementUnique[0] ==0)
	{
		pExplainText = GetTxtFromMgr(5918);
		pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText );
	}
	else
	{
		pExplainText = GetTxtFromMgr(5919);
		pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText );
	}


	return false;
}

void CNNewEnchantChild::SetEnchantCharUnique(SI32 CharUnique)
{
	m_siOtherCharUnique = CharUnique;
}

void CNNewEnchantChild::SetInitInchant()
{
	m_siOtherCharUnique =0;
	cltClient *pclclient = (cltClient *)pclClient;
	SI32 id = 1;	
	pclclient->pclCM->CR[id]->siMatInvEnchantInx = 0;
	pclclient->pclCM->CR[id]->siOrgInvEnchantInx = 0;
	pclclient->pclCM->CR[id]->siElementEnchantInx[0] = 0;
	pclclient->pclCM->CR[id]->siElementEnchantInx[1] = 0;
	pclclient->pclCM->CR[id]->siElementEnchantInx[2] = 0;
	pclclient->pclCM->CR[id]->siMouseInvItem = 0;
	for(SI32 i =0 ;i <3 ; i++)
	{
		pclClient->pclCM->CR[1]->siElementEnchantInx[i]=0;
	}

}

