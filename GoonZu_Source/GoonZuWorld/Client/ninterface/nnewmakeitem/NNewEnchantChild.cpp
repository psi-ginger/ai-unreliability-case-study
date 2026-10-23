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
#include "../../../CommonLogic/Msg/MsgType-Quest.h"

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
	for(SI32 i=0; i<NOW_ENCHANT_NUMBER; i++)
	{
		m_clEleEnchantItem[i].Init();
		m_clEleEnchantItem[i]=0;
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
		
		/*
		if (pclClient->siServiceArea == ConstServiceArea_Korea) {

			file.LoadFile("NInterface/Data/NNewMakeItem/DLG_NewEnchant_k.ddf");
		}
		else
		{
			file.LoadFile("NInterface/Data/NNewMakeItem/DLG_NewEnchant.ddf");	
		}
		*/
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
		
		//m_InterfaceFactory.GetButton(NENCHANTOTHER_STATIC_UNDATEMAT)->Enable(FALSE);
		//�ʱ�ȭ

		if ( pclClient->siServiceArea == ConstServiceArea_China )
		{
			m_InterfaceFactory.GetStatic( NEWMAKE_STATIC_ENCHANT )->Show(false);
		}

		if (pclClient->siServiceArea == ConstServiceArea_Korea)
		{
			//�ѱ��� ��ư�ȿ� ���� ���� ������ ��ư�� ũ�Ⱑ �޶�����.
			SI16 siEnchantX	=	6,		siEnchantY	=	309,	siEnchantW	=	106,	siEnchantH	=	24;
			SI16 siUpdateX	=	117,	siUpdateY	=	309,	siUpdateW	=	63,		siUpdateH	=	24;
			
			//��þƮ ��ư
			m_InterfaceFactory.GetButton( NEWMAKE_BUTTON_STARTENCHANT )->SetControllerPos( siEnchantX, siEnchantY, siEnchantW, siEnchantH, siEnchantW+2, siEnchantH+2 );
			//�Ⱓ���� ��ư
			m_InterfaceFactory.GetButton( NENCHANTOTHER_STATIC_UNDATEMAT )->SetControllerPos( siUpdateX, siUpdateY, siUpdateW, siUpdateH, siUpdateW+2, siUpdateH+2 );
		}
		SetInitInchant();
		
		SI32 siTempEnchantItemIndex[NOW_ENCHANT_NUMBER] = {0,};
		ZeroMemory( siTempEnchantItemIndex, sizeof(siTempEnchantItemIndex) );

		cltGameMsgRequest_EnchantItem_SelectedIndex clGameMsgRequest_EnchantItem_SelectedIndex( 0, 0, siTempEnchantItemIndex, 0 );
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
			SI32 EleInx[NOW_ENCHANT_NUMBER] = {0,};
			SI32 ElementType = pclclient->pclCM->CR[id]->siElementType;

			CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];

			if( Orginx <= 0 || m_clOrgEnchantItem.siUnique <=0 )
				break;

			// ������ ��þƮ�� �����ϴ�.
			if (personinventory->CheckCanEnchintItem(m_clOrgEnchantItem.siUnique ) == false)
			{
				NTCHARString64 kTitleText(GetTxtFromMgr(5927)); // ���պҰ� ��� �˸�â Ÿ��Ʋ�� �̸� �����صд�.
				NTCHARString64 kExplain(GetTxtFromMgr(5928));	// ���� ���� ���հ����մϴ�
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, kExplain);
				return;
			}

			TCHAR * pTitleText = GetTxtFromMgr(5912); // �⺻������ ���ս��� Ÿ��Ʋ�� �غ��صд�.
			TCHAR * pExplainText = NULL;
			SI32 siMaxGrade = pclClient->pclItemManager->GetItemGrade(m_clOrgEnchantItem.siUnique);
 			SI32 siOrgGrade = m_clOrgEnchantItem.Element.siPower;			

			for (SI32 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
			{
				EleInx[siIndex] = pclclient->pclCM->CR[id]->siElementEnchantInx[siIndex];
			}
		
			/////////////// ���� 2�ܰ������ ���� //////////////////////
			/*
			SI32 siType = pclClient->pclItemManager->GetItemType( m_clOrgEnchantItem.siUnique );
           
			switch(siType) 
			{
			case ITEMTYPE_ARMOUR:
			case ITEMTYPE_BELT:
			case ITEMTYPE_SHOES:
			case ITEMTYPE_HELMET:
				{                    
					siMaxGrade = min(siMaxGrade, 2 );
				}
			} */
			

			/////////////// ���� 2�ܰ������ ���� //////////////////////

			if( siMaxGrade <= siOrgGrade )
			{
				pExplainText = GetTxtFromMgr(5933);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText );
				return;                				
			}			
			if(pclClient->pclItemManager->IsCheckSkill(m_clOrgEnchantItem, 1) == false)
			{	
				stEnchantNeed materialunique1;
				pclClient->pclItemManager->GetEnchantNeedItem(m_clOrgEnchantItem, &materialunique1);
				SI32 siItemType = pclClient->pclItemManager->GetItemType(m_clOrgEnchantItem.siUnique);
				TCHAR szBuffer[256] = {'\0',};
				TCHAR* szSkillName = NULL;
					
				switch(siItemType)
				{
					case ITEMTYPE_SWORD:	szSkillName = GetTxtFromMgr( 2854); break;
					case ITEMTYPE_SPEAR:	szSkillName = GetTxtFromMgr( 2855); break;
					case ITEMTYPE_STAFF:	szSkillName = GetTxtFromMgr( 2857); break;
					case ITEMTYPE_AXE:		szSkillName = GetTxtFromMgr( 2856); break;
					case ITEMTYPE_BOW:		szSkillName = GetTxtFromMgr( 2858); break;
					case ITEMTYPE_GUN:		szSkillName = GetTxtFromMgr( 2859); break;
					case ITEMTYPE_ARMOUR:	szSkillName = GetTxtFromMgr( 2852); break;
					case ITEMTYPE_DRESS:	szSkillName = GetTxtFromMgr( 2873); break;
					case ITEMTYPE_BELT:		szSkillName = GetTxtFromMgr( 2862); break;
					case ITEMTYPE_SHOES:	szSkillName = GetTxtFromMgr( 2861); break;
					case ITEMTYPE_HELMET:	szSkillName = GetTxtFromMgr( 2853); break;
					case ITEMTYPE_HAT:		szSkillName = GetTxtFromMgr( 2874); break;
					default: szSkillName = GetTxtFromMgr( 2854); break;
				}

				pExplainText = GetTxtFromMgr(5911);
				
				StringCchPrintf(szBuffer,256, pExplainText, szSkillName, materialunique1.siNeedSkill);


				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, szBuffer );
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
				return;

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

			//��ᰡ ����� �ִ��� üũ���ֱ� 



			cltGameMsgRequest_EnchantItem_SelectedIndex clGameMsgRequest_EnchantItem_SelectedIndex(Orginx, Matinx, EleInx, ElementType);
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
			if(m_clOrgEnchantItem.siUnique >0)
			{
				EnableButton(true, false,false,false);
				ShowChangeEnchantMsgBox(ENCHANT_TYPE_WATER_A);
			}
		}
		break;
	case NEWMAKE_BUTTON_ENCHANTFIRE:
		{
			if(m_clOrgEnchantItem.siUnique >0)
			{
				EnableButton(false,true,false,false);
				ShowChangeEnchantMsgBox(ENCHANT_TYPE_FIRE_A);
			}
		}
		break;
	case NEWMAKE_BUTTON_ENCHANTWIND:
		{
			if(m_clOrgEnchantItem.siUnique >0)
			{
				EnableButton(false,false,true,false);
				ShowChangeEnchantMsgBox(ENCHANT_TYPE_WIND_A);
			}
		}
		break;
	case NEWMAKE_BUTTON_ENCHANTGROUND:
		{
			if(m_clOrgEnchantItem.siUnique >0)
			{
				EnableButton(false,false,false,true);
				ShowChangeEnchantMsgBox(ENCHANT_TYPE_GROUND_A);
			}
		}
		break;
	case NEWMAKE_BUTTON_WHATENCHANT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclclient->ShowHelpWindow(TEXT("enchant"));
					//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
					//{
					//	pclclient->CreateInterface( NHINT_DLG );
					//}
					//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show("enchant");

				}
			}

		}
		break;

	case NENCHANTOTHER_STATIC_UNDATEMAT:		// �뵵 ���� : ��þƮ�Ƿ� -> �Ⱓ ����
		{

			SI32 Orginx = pclclient->pclCM->CR[id]->siOrgInvEnchantInx;
			SI32 Matinx = pclclient->pclCM->CR[id]->siMatInvEnchantInx;
			SI32 EleInx[NOW_ENCHANT_NUMBER] = {0,};
			SI32 ElementType = pclclient->pclCM->CR[id]->siElementType;

			if( Orginx <= 0 || m_clOrgEnchantItem.siUnique <=0 )
				break;
			bool bIsUsedayPlusItem = false;

			SI32 siUseEleInx = 0;

			for (SI32 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
			{
				EleInx[siIndex] = pclclient->pclCM->CR[id]->siElementEnchantInx[siIndex];
				
				//�Ⱓ ���� ������ �˻�
				if ( bIsUsedayPlusItem == false &&
					pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[ EleInx[siIndex] ].siUnique == ENCHANT_USEDAYPLUS_ITEMUNIQUE )
				{
					siUseEleInx			= EleInx[siIndex];
					bIsUsedayPlusItem	= true;
				}
			}
			
			if( bIsUsedayPlusItem == true )
			{
				bool	bEnablePlus = pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[Orginx].IsPlusUseDate(pclClient->pclItemManager);

				if(bEnablePlus)
				{
					cltGameMsgRequest_UseDayPlus clUseDayPlus( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[Orginx].siUnique  , Orginx  , siUseEleInx );
					cltMsg clMsg(GAMEMSG_REQUEST_ENCHANT_USEDAYPLUS, sizeof(clUseDayPlus), (BYTE*)&clUseDayPlus);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				else
				{
					if(pclClient->IsCountrySwitch(Switch_UseDayPlusRevitalize))
					{
						TCHAR * pTitleText = GetTxtFromMgr(5912); // �⺻������ ���ս��� Ÿ��Ʋ�� �غ��صд�.
						TCHAR * pExplainText = GetTxtFromMgr(8075);

						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText );
						return;
					}
					else
					{
						TCHAR * pTitleText = GetTxtFromMgr(5912); // �⺻������ ���ս��� Ÿ��Ʋ�� �غ��صд�.
						TCHAR * pExplainText = GetTxtFromMgr(7080);

						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText );
						return;
					}
				}

				return;
			}
			// �Ⱓ ���� �������� �߰��� ��//

			/*

			SI32 id = 1;	

			SI32 siOrgindex = pclclient->pclCM->CR[id]->siOrgInvEnchantInx ;
			SI32 siMatindex = pclclient->pclCM->CR[id]->siMatInvEnchantInx;
			SI32 siEleindex1 = pclclient->pclCM->CR[id]->siElementEnchantInx[0];
			SI32 siEleindex2 = pclclient->pclCM->CR[id]->siElementEnchantInx[1];
			SI32 siEleindex3 = pclclient->pclCM->CR[id]->siElementEnchantInx[2];			
			TCHAR * pTitleText = GetTxtFromMgr(5912); // �⺻������ ���ս��� Ÿ��Ʋ�� �غ��صд�.
			TCHAR * pExplainText = NULL;

 			if(m_siOtherCharUnique ==0)
			{
				pExplainText = GetTxtFromMgr(5916);
				pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, pExplainText);
				return;
			}
			
			if(siOrgindex <= 0)
				return;
			

			if( pclclient->pclCM->CR[1]->pclCI->clCharItem.clItem[siOrgindex].Element.siPower < 2)
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
				*/
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
					
					//m_InterfaceFactory.GetStatic(NEWMAKE_STATIC_ENCHANT)->SetText(GetTxtFromMgr(7009));

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

					//m_InterfaceFactory.GetStatic(NEWMAKE_STATIC_ENCHANT)->SetText(GetTxtFromMgr(7010));

					break;
				}
			}	
			MaterialListViewMsgProc( m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM), 0 , nEvent );		
		}
		break;	
	case NEWMAKE_LISTVIEW_ELEMENTITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				for(SI32 i=0; i<NOW_ENCHANT_NUMBER; i++)
				{
					if(pclclient->pclCM->CR[id]->siElementEnchantInx[i] ==0)
					{
						pclclient->pclCM->CR[id]->siElementEnchantInx[i] =  pclclient->pclCM->CR[id]->siMouseInvItem;
						m_clEleEnchantItem[i].Set(&pclclient->pclCM->CR[id]->clMouseInvItem);
						//m_InterfaceFactory.GetStatic(NEWMAKE_STATIC_ENCHANT)->SetText(GetTxtFromMgr(7012));
						break;
					}
				}
				break;
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


	// [����]�Ӽ��� ���� ���� �������� �÷����ִµ� ���� ��ư�� �ѹ��� ������ ���� ���·� �ٽ� �ٲٶ�� �Ѵٸ� �Ӽ� ������ ����
	if(	m_clOrgEnchantItem.Element.siType == ENCHANT_TYPE_DEFAULT
		&& m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWATER)->IsEnable() == bWater
		&& m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTFIRE)->IsEnable() == bFire
		&& m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWIND)->IsEnable() == bWind
		&& m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTGROUND)->IsEnable() == bGround
		)
	{
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWATER)->Enable(true)	;
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTFIRE)->Enable(true);
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWIND)->Enable(true);
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTGROUND)->Enable(true);
		pclClient->pclCM->CR[1]->siElementType = ENCHANT_TYPE_DEFAULT;

		//m_InterfaceFactory.GetStatic(NEWMAKE_STATIC_ENCHANT)->SetText(GetTxtFromMgr(7010));
		return;
	}
		


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

	// ���� ��þƮ ��� �������� ������ ��ῡ ���� ���� ���� ������ ��������

	stEnchantNeed EnchantNeed;
	pclClient->pclItemManager->GetEnchantNeedItem(m_clOrgEnchantItem,&EnchantNeed,0,false,false);   

	bool haveElementStone = false;

	for( SI16 i=0; i<MAX_ENCHANT_NUMBER ; i++ )
	{
		if( EnchantNeed.siElementUnique[i] == ITEMUNIQUE(5350) || 
			EnchantNeed.siElementUnique[i] == ITEMUNIQUE(5355) || 
			EnchantNeed.siElementUnique[i] == ITEMUNIQUE(5360) || 
			EnchantNeed.siElementUnique[i] == ITEMUNIQUE(5365) )
		{
			TCHAR szBuffer[256];
			szBuffer[0] = 0;
			TCHAR * pText = GetTxtFromMgr(7011);
			const TCHAR * pItemName = pclClient->pclItemManager->GetName(EnchantNeed.siElementUnique[i]);
			if(pItemName == NULL) break;
			
			StringCchPrintf(szBuffer, 256 , pText , pItemName , EnchantNeed.siElementNum[i] );
			
			//m_InterfaceFactory.GetStatic(NEWMAKE_STATIC_ENCHANT)->SetText(szBuffer);			
			haveElementStone = true;
			break;
		}
	}

	if( haveElementStone == false )
	{
		//m_InterfaceFactory.GetStatic(NEWMAKE_STATIC_ENCHANT)->SetText(GetTxtFromMgr(7012));
	}

		

	///////////////////////////////////
	// �Ӽ��� �����ϴ� ����Ʈ ���̶��
	//////////////////////////////////

	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_SETENCHANTELEMENT;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true )
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		cltClient* pclclient = (cltClient* )pclClient;
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}


void CNNewEnchantChild::OriginalListViewMsgProc( CListView* pListView, SI16 ItemPos, UINT nEvent )
{
	if ( pListView == NULL )
	{
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;

	// �����̵� ����̶� ������ ������� �ִ�!!
	CListView* m_pInventoryListView = ((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_pInventoryListView;
	SI16 m_siCurrentInventoryIndex =((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_siCurrentInventoryIndex;

	static int TempCounter =0 ;

	switch( nEvent )
	{
	case EVENT_LISTVIEW_RBUTTONUP:
		{

			SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				return;

			SetInitInchant();



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

	// �����̵� ����̶� ������ ������� �ִ�!!
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
                CNNewInventory::clMakeItemContainer.clMatEnchantItem.Init();						
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

	// �����̵� ����̶� ������ ������� �ִ�!!
	CListView* m_pInventoryListView = ((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_pInventoryListView;
	SI16 m_siCurrentInventoryIndex =((CNNewInventory*) pclclient->m_pDialog[ NNEWINVENTORY_DLG])->m_siCurrentInventoryIndex;


	switch( nEvent )
	{
	case EVENT_LISTVIEW_RBUTTONUP:
		{
			SI16 SelectedIndex = pListView->GetSelectedItemIndex();

			if(SelectedIndex < 0)
				return;
			if(SelectedIndex >= NOW_ENCHANT_NUMBER)	// equip->clEleEnchantItem[3] �̴�
				return;

			// ��þƮ �Ӽ����� �������� ���������� ������ �ʱ�ȭ ��Ų��
			if (m_clEleEnchantItem[SelectedIndex].siUnique == ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE)
			{
				SetInitInchant();
				return;
			}

			if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
			{
				CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
				CNNewInventory::clMakeItemContainer.clEleEnchantItem[SelectedIndex].Init();
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
//			SI32 siItemStatus = 0 ;
//			pclClient->pclItemManager->GetExplain( &m_clOrgEnchantItem, ToolTipMsg ,&siItemStatus);
			pclClient->pclItemManager->GetExplain( &m_clOrgEnchantItem, ToolTipMsg);
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
//			SI32 siItemStatus = 0 ;
//			pclClient->pclItemManager->GetExplain( &m_clMatEnchantItem, ToolTipMsg ,&siItemStatus);
			pclClient->pclItemManager->GetExplain( &m_clMatEnchantItem, ToolTipMsg);
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


	memcpy(m_clEleEnchantItem, cltEleItem, sizeof(cltItem)*NOW_ENCHANT_NUMBER);
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM)->DeleteAllItems();

	for(SI32 i =0; i<NOW_ENCHANT_NUMBER ; i++)
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

	BOOL bBlessingStone = FALSE;
	SI16 siBlessingStoneSlot = 0;
	BOOL bSealingUpIgnore = FALSE;

	for( SI16 j = 0;j < NOW_ENCHANT_NUMBER ; j++)
	{
		if(m_clEleEnchantItem[j].siUnique == ITEMUNIQUE(5370))
		{
			bBlessingStone = TRUE;
			siBlessingStoneSlot = j;			
		}
		else if(m_clEleEnchantItem[j].siUnique == ITEMUNIQUE(13607))
		{
			bSealingUpIgnore = TRUE;
		}
	}

    
	stEnchantNeed EnchantNeed;
	pclClient->pclItemManager->GetEnchantNeedItem(m_clOrgEnchantItem,&EnchantNeed,0,bBlessingStone,bSealingUpIgnore);   


	TCHAR * pTitleText = GetTxtFromMgr(5912); //  ���ս��� Ÿ��Ʋ�� �غ��صд�.
	TCHAR * pExplainText = NULL;
	TCHAR szBuffer[256] = {'\0',};

	const TCHAR* pitemname = pclClient->pclItemManager->GetName(EnchantNeed.siElementUnique[1]);

	if(	pclClient->IsCountrySwitch( Switch_EnchantNoSealing) )
	{
		//KHY - 0720 - ��þƮ ���� �ͼ� �����۵� ��æƮ ����.
	}
	else
	{
		// �ͼӹ��� �������� ������� �ʾҴٸ� �ͼӵ� �������� ��þƮ �Ҽ� ����.
		if( bSealingUpIgnore == FALSE )
		{
			if( m_clOrgEnchantItem.IsSealingUp(pclClient->pclItemManager) == false )
			{
				pExplainText = GetTxtFromMgr( 5940 );

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitleText,pExplainText);

				return false;			
			}
		}
	}

	// �ູ�� ���� �����ٸ� ������ �ùٸ��� üũ
	if( bBlessingStone )
	{
		for(SI16 i=0; i<NOW_ENCHANT_NUMBER; i++)
		{
			if( EnchantNeed.siElementUnique[i] ==  ITEMUNIQUE(5370) )
			{
				if(EnchantNeed.siElementNum[i] <=	m_clEleEnchantItem[siBlessingStoneSlot].siItemNum)
				{
					break;
				}
				else
				{
					// �ູ�ǵ� ������ ������ �����ϴ�.

					pExplainText = GetTxtFromMgr( 6608 );

					StringCchPrintf( szBuffer, 256, pExplainText, EnchantNeed.siElementNum[i]  );

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitleText,szBuffer);

					return false;
				}
			}
		}
	}



	SI32 siGrade = m_clOrgEnchantItem.Element.siPower;

	if(siGrade ==0  &&EnchantNeed.siElementUnique[0]!=0 &&EnchantNeed.siElementUnique[0] == m_clMatEnchantItem.siUnique  )
	{
		return true;
	}	

	if(siGrade >= 1 && siGrade < MAX_ENCHANT_GRADE)
	{
		/*
		// 1�� �Ӽ��� ������ �����۰� ����
		if((EnchantNeed.siElementUnique[0] ==m_clMatEnchantItem.siUnique 
			&& EnchantNeed.siElementUnique[1] == m_clEleEnchantItem[0].siUnique) 
			&& EnchantNeed.siElementUnique[0]!=0
			&& EnchantNeed.siElementUnique[1]!=0
			&& EnchantNeed.siElementNum[1] <= m_clEleEnchantItem[0].siItemNum )
			return true;
		// 2�� �Ӽ��� ������ �����۰� ����
		if((EnchantNeed.siElementUnique[0] ==m_clMatEnchantItem.siUnique 
			&& EnchantNeed.siElementUnique[1] == m_clEleEnchantItem[1].siUnique) 
			&& EnchantNeed.siElementUnique[0]!=0
			&& EnchantNeed.siElementUnique[1]!=0
			&& EnchantNeed.siElementNum[1] <= m_clEleEnchantItem[1].siItemNum )
			return true;
		// 3�� �Ӽ��� ������ �����۰� ����
		if((EnchantNeed.siElementUnique[0] ==m_clMatEnchantItem.siUnique 
			&& EnchantNeed.siElementUnique[1] == m_clEleEnchantItem[2].siUnique) 
			&& EnchantNeed.siElementUnique[0]!=0
			&& EnchantNeed.siElementUnique[1]!=0
			&& EnchantNeed.siElementNum[1] <= m_clEleEnchantItem[2].siItemNum )
			return true;
		*/
		for (SI16 siCheckSlot=0; siCheckSlot<NOW_ENCHANT_NUMBER; siCheckSlot++)
		{
			if((EnchantNeed.siElementUnique[0] ==m_clMatEnchantItem.siUnique 
				&& EnchantNeed.siElementUnique[1] == m_clEleEnchantItem[siCheckSlot].siUnique) 
				&& EnchantNeed.siElementUnique[0]!=0
				&& EnchantNeed.siElementUnique[1]!=0
				&& EnchantNeed.siElementNum[1] <= m_clEleEnchantItem[siCheckSlot].siItemNum )
			{
				return true;
			}
		}
	}


	//  ��� �˸�
	//	2 ���� �� ��þƮ �ϱ� ���ؼ� ���� ������ 1���� ���� �Ӽ��� 10���� �ʿ��մϴ�.
	// ��������.


	if( EnchantNeed.siElementUnique[1] > 0 && pitemname != NULL && EnchantNeed.siElementNum[1] > 0 )
	{
		pExplainText = GetTxtFromMgr( 6179 );

		StringCchPrintf( szBuffer , 256 , pExplainText , siGrade +1 , pitemname , EnchantNeed.siElementNum[1] );
	
		pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, szBuffer );
	}
	else 
	{
		pExplainText = GetTxtFromMgr(6185 );
		StringCchPrintf( szBuffer , 256 , pExplainText , siGrade +1 );

		pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(pTitleText, szBuffer );
	}

	return false;
}

void CNNewEnchantChild::SetEnchantCharUnique(SI32 CharUnique)
{
	m_siOtherCharUnique = CharUnique;
}

void CNNewEnchantChild::SetInitInchant()
{
	SI16		siIndex		= 0;
	cltClient	*pclclient	= (cltClient *)pclClient;
	m_siOtherCharUnique =0;


	m_clOrgEnchantItem.Init();
	m_clMatEnchantItem.Init();

	for (siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
	{
		m_clEleEnchantItem[siIndex].Init();
	}


	if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
	{
		CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
		CNNewInventory::clMakeItemContainer.clOrgEnchantItem.Init();	
		CNNewInventory::clMakeItemContainer.clMatEnchantItem.Init();
		for (siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
		{
			CNNewInventory::clMakeItemContainer.clEleEnchantItem[siIndex].Init();
			if(pclClient->pclCM->CR[1]->siElementEnchantInx[siIndex] > 0)
			{
				equip->m_pInventoryListView->SetItemDrawAlphaOption(pclClient->pclCM->CR[1]->siElementEnchantInx[siIndex], false);
			}

		}
		

		if( pclclient->pclCM->CR[1]->siOrgInvEnchantInx > 0 )
			equip->m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siOrgInvEnchantInx,false);
		if( pclclient->pclCM->CR[1]->siMatInvEnchantInx > 0 )
			equip->m_pInventoryListView->SetItemDrawAlphaOption(pclclient->pclCM->CR[1]->siMatInvEnchantInx,false);

	}

	pclclient->pclCM->CR[1]->siMatInvEnchantInx = 0;
	pclclient->pclCM->CR[1]->siOrgInvEnchantInx = 0;
	for (siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
	{
		pclclient->pclCM->CR[1]->siElementEnchantInx[siIndex] = 0;
	}
	pclclient->pclCM->CR[1]->siMouseInvItem = 0;

	/*
	for(SI32 i =0 ;i <3 ; i++)
	{
		pclClient->pclCM->CR[1]->siElementEnchantInx[i]=0;
	}
*/
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ELEMENTITEM)->DeleteAllItems();
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_MATERIALITEM)->DeleteAllItems();
	m_InterfaceFactory.GetListView(NEWMAKE_LISTVIEW_ORIGINALITEM)->DeleteAllItems();

	EnableButton(true,true,true,true);

	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_STARTENCHANT)->Enable( true );
	m_InterfaceFactory.GetButton(NENCHANTOTHER_STATIC_UNDATEMAT)->Enable( true );
	
	//m_InterfaceFactory.GetStatic(NEWMAKE_STATIC_ENCHANT)->SetText(GetTxtFromMgr(7008));
}

void CNNewEnchantChild::SetEnchantOtherState(BOOL bEnchantOther )
{
	if( bEnchantOther)
	{
		m_InterfaceFactory.GetButton(NENCHANTOTHER_STATIC_UNDATEMAT)->Enable(TRUE);
	}
	else
	{
		m_InterfaceFactory.GetButton(NENCHANTOTHER_STATIC_UNDATEMAT)->Enable(FALSE);
	}

}

/////////////////////////////////////////////////////////////////////////////////
//
// ��þƮ �Ӽ� ������� �Լ� ����
//
/////////////////////////////////////////////////////////////////////////////////

// ���� �÷��� ������ �Ӽ��� ���� �Ӽ��� ��ư ����
void CNNewEnchantChild::SetChangeEnchanteButton( SI32 siEnchantType )
{
	bool	bSetButtonEnable[4]	= {true, true, true, true};
	SI32	siButtonNumber		= -1;


	switch ( siEnchantType ) 
	{
		case ENCHANT_TYPE_FIRE_A:
		case ENCHANT_TYPE_FIRE_D:
			{
				siButtonNumber = 0;
			}
			break;

		case ENCHANT_TYPE_WATER_A:
		case ENCHANT_TYPE_WATER_D:
			{
				siButtonNumber = 1;
			}
			break;

		case ENCHANT_TYPE_WIND_A:
		case ENCHANT_TYPE_WIND_D:
			{
				siButtonNumber = 2;
			}
			break;
	
		case ENCHANT_TYPE_GROUND_A:
		case ENCHANT_TYPE_GROUND_D:
			{
				siButtonNumber = 3;
			}
			break;
	}

	if ( siButtonNumber < 0 )
	{
		return;
	}
	
	bSetButtonEnable[ siButtonNumber ] = false;

	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTFIRE)->SetFocus(false);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWATER)->SetFocus(false);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWIND)->SetFocus(false);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTGROUND)->SetFocus(false);
	
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTFIRE)->Enable(bSetButtonEnable[0]);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWATER)->Enable(bSetButtonEnable[1]);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTWIND)->Enable(bSetButtonEnable[2]);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_ENCHANTGROUND)->Enable(bSetButtonEnable[3]);

	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_STARTENCHANT)->Enable( false );
	m_InterfaceFactory.GetButton(NENCHANTOTHER_STATIC_UNDATEMAT)->Enable( false );

}

void CNNewEnchantChild::ShowChangeEnchantMsgBox( SI32 siEnchantType )
{
	bool	bIsEnchantChange	= false;

	// �� �Լ��� ��þƮ �Ӽ����� �������� ������ �������� ����Ǿ�� �Ѵ�.
	for ( SI32 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++ )
	{
		if ( m_clEleEnchantItem[siIndex].siUnique == ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE )
		{
			bIsEnchantChange = true;
			break;
		}
	}

	// ���� ���� ��þƮ �Ӽ����� �������� ������ �ִٸ�
	if ( bIsEnchantChange == true )
	{
		cltClient		*pclclient		= (cltClient*)pclClient;
		if ( pclclient == NULL )		return;

		CControllerMgr	*pParentDialog	= this->GetParentDialog();
		if ( pParentDialog == NULL )	return;

		stMsgBox		MsgBox;
		TCHAR			*pszTitle		= GetTxtFromMgr(7940);
		NTCHARString128	kMessage		= GetTxtFromMgr(7950);
		
		stChangeEnchantMsgPara stPara;
		BYTE			bytPara[128]	= {0, };
		SI32			siParaSize		= sizeof(stChangeEnchantMsgPara);
		SI16			siAddNumber		= 0;

		// siEnchantType�� ����Ӽ��� ��þƮ�� ���� �μ��� ����� ������
		// ���� �Ӽ��� �����ϰ��� �ϴ� �������� ����� siEnchantType�� 1�� �����༭ ������ ��Ÿ����
		if ( (m_clOrgEnchantItem.Element.siType % 2) == 0 ) siAddNumber = 1;
		pclClient->pclCM->CR[1]->siElementType	= siEnchantType+siAddNumber;

		stPara.siUnique = m_clOrgEnchantItem.siUnique;
		// �ƴϿ��� �������� ���� �������� ���ư��� ���� ����
		stPara.siEnchantType = m_clOrgEnchantItem.Element.siType;
		
		for (SI32 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
		{
			if ( m_clEleEnchantItem[siIndex].siUnique == ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE )
			{
				stPara.siElementIndex = siIndex;
				break;
			}
		}

		kMessage.Replace( TEXT("#enchanttype#"), GetTxtFromMgr( (5901+siEnchantType+siAddNumber) ) );

		MsgBox.Set( pParentDialog, pszTitle, kMessage, MSGBOX_TYPE_YESNO, MSGBOX_ID_CHANGEENCHANT );

		memcpy( bytPara, &stPara, siParaSize );

		pclclient->SetMsgBox( &MsgBox, bytPara, siParaSize );
	}
}

void CNNewEnchantChild::CheckChangeEnchantItem( SI16 siItemUnique, SI16 siElementIndex )
{
	// ��ư�� ������ �� ������ ����ũ�� �ٸ��� ����
	if ( (siItemUnique != m_clOrgEnchantItem.siUnique)
		|| (m_clEleEnchantItem[siElementIndex].siUnique != ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE )
	)
	{
		return;
	}

	// �Լ� ���࿡ �ʿ��� ��ü�� ������
	SI32				siID		= 1;
	cltClient*			pclclient	= (cltClient *)pclClient; 
	if ( pclclient == NULL )
	{
		return;
	}

	cltCharCommon*		pclChar		= (cltCharCommon*)pclclient->pclCM->CR[siID];
	if ( pclChar == NULL )
	{
		return;
	}
	
	// ���������� ��þƮ �Ӽ�������� ������ üũ�ϱ�
	SI16	siOrgIndex			= pclChar->siOrgInvEnchantInx; // ��þƮ�� �����ϰ����ϴ� ������ �κ��丮 ��ȣ
	SI16	siEleIndex			= 0;
	SI16	siNowElementType	= pclChar->pclCI->clCharItem.clItem[ siOrgIndex ].GetEnchantType( pclClient->pclItemManager );
	SI16	siElementType		= pclChar->siElementType;
	bool	bIsChangeEnchant	= false;

	// �����ϰ��� �ϴ� �������� �κ��丮 ��ȣ�� Unique�� �̻��Ұ��
	if( (siOrgIndex <= 0) || (m_clOrgEnchantItem.siUnique <= 0) )
	{
		return;
	}
	
	// ���� �÷����� �������� ��þƮ Ÿ�԰� �����ϰ��� �ϴ� ��þƮ Ÿ���� ���ٸ�
	if ( siNowElementType == siElementType )
	{
		return;
	}

	// ��þƮ �Ӽ� ���� �������� �����ϴ��� �˻�
	for (SI32 siIndex=0; siIndex<NOW_ENCHANT_NUMBER; siIndex++)
	{
		siEleIndex = pclChar->siElementEnchantInx[siIndex];
		
		if (pclChar->pclCI->clCharItem.clItem[ siEleIndex ].siUnique == ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE)
		{
			bIsChangeEnchant = true;
			break;
		}
	}

	// ���� �Ӽ� ���� �������� ���ٸ� ����
	if ( (bIsChangeEnchant == false) || (siEleIndex <= 0) )
	{
		return;
	}


	cltGameMsgRequest_ChangeEnchant clChangeEnchant(m_clOrgEnchantItem.siUnique, siOrgIndex, siElementType, siEleIndex);
	cltMsg clMsg(GAMEMSG_REQUEST_CHANGEENCHANT, sizeof(clChangeEnchant), (BYTE*)&clChangeEnchant);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

bool CNNewEnchantChild::CheckEnchantMaterial(SI32 siEnchantType, SI32 siMaterialUnique)
{
	switch(siMaterialUnique)
	{
	case ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE:		{		return true;		}		break;	// ��æƮ Ÿ�� ������ ��
	case ENCHANT_USEDAYPLUS_ITEMUNIQUE:			{		return true;		}		break;	// �Ⱓ ������ ��
	case ITEMUNIQUE(5370):						{		return true;		}		break;	// �ູ�� ��
	case ITEMUNIQUE(13607):						{		return true;		}		break;	// �к� �ӽ� ���� ������
	case ITEMUNIQUE(13606):						{		return true;		}		break;	// Ȯ�� ��� ������
	case ITEMUNIQUE(5350):		// ���� �Ӽ���
		{
			if( siEnchantType == ENCHANT_TYPE_FIRE_A || siEnchantType == ENCHANT_TYPE_FIRE_D )
				return true;
		}
		break;
	case ITEMUNIQUE(5355):		// ���� �Ӽ���
		{
			if( siEnchantType == ENCHANT_TYPE_WATER_A || siEnchantType == ENCHANT_TYPE_WATER_D )
				return true;
		}
		break;
	case ITEMUNIQUE(5360):		// �ٶ��� �Ӽ���
		{
			if( siEnchantType == ENCHANT_TYPE_WIND_A ||	siEnchantType == ENCHANT_TYPE_WIND_D )
				return true;
		}
		break;
	case ITEMUNIQUE(5365):		// ���� �Ӽ���
		{
			if( siEnchantType == ENCHANT_TYPE_GROUND_A || siEnchantType == ENCHANT_TYPE_GROUND_D )
				return true;
		}
		break;
	}

	return false;
}

bool CNNewEnchantChild::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	cltClient* pclclient = (cltClient*)pclClient;
	CNNewInventory* personinventory = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
	if(personinventory == NULL)										return false;
	if(pclClickAndDrop == NULL)										return false;
	if(pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false)	return false;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;

	switch(pclClickAndDrop_Item->m_siSrcDialogID)
	{
		// �κ��丮���� �κ��丮�� �̵��ϴ� ���
	case NNEWINVENTORY_DLG:
	case NNEWINVENTORY_HORSE_DLG:
	case NNEWINVENTORY_BAG0_DLG:
	case NNEWINVENTORY_BAG1_DLG:
		{
			NTCHARString64 kTitleText(GetTxtFromMgr(5927)); // ���պҰ� ��� �˸�â Ÿ��Ʋ�� �̸� �����صд�.
			TCHAR* pExplain = NULL;
			cltItem* pclCharItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0];

			switch(pclClickAndDrop_Item->m_siDstControlID)
			{
			// ���� ������ �ֱ�
			case NEWMAKE_LISTVIEW_ORIGINALITEM:
				{
					// �ƹ�Ÿ ������ �߰�.
					if (personinventory->CheckCanPutonItem(pclCharItem[pclClickAndDrop_Item->m_siPara1].siUnique ) == false)
					{
						NTCHARString64 kExplain(GetTxtFromMgr(5928));	// ���� ���� ���հ����մϴ�
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, kExplain);
					}
					else
						personinventory->ShowEnchantOrg(pclClickAndDrop_Item->m_siPara1);

					return true;
				}				
				break;
			// ��� ������ �ֱ�
			case NEWMAKE_LISTVIEW_MATERIALITEM:
				{
					SI32 org = pclclient->pclCM->CR[1]->siOrgInvEnchantInx;

					// ���� ������ ����
					if(org ==0)
					{ 
						pExplain = GetTxtFromMgr(5929);
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, pExplain);
					}
					// ������������ ���� ���� �� ����
					else if(pclClickAndDrop_Item->m_siPara1 == org)
					{
						pExplain = GetTxtFromMgr(5930);
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, pExplain);
					}
					// ���� ���ɼ� �ִ� �������ΰ�?
					else if (personinventory->CheckCanPutonItem(pclCharItem[pclClickAndDrop_Item->m_siPara1].siUnique ) == false)
					{									
						pExplain = GetTxtFromMgr(5928);
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, pExplain);
					}
					// ���� �����۰� ���� �������ΰ�?
					else if (pclCharItem[org].siUnique != pclCharItem[pclClickAndDrop_Item->m_siPara1].siUnique )
					{
						pExplain = GetTxtFromMgr(5931);
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, pExplain);
					}
					else
						personinventory->ShowEnchantMat(pclClickAndDrop_Item->m_siPara1);

					return true;
				}
				break;
			case NEWMAKE_LISTVIEW_ELEMENTITEM:
				{
					// ��� �������� �����Ǿ� ���� �ʴ�
					if( pclclient->pclCM->CR[1]->siOrgInvEnchantInx <= 0)
					{
						pExplain = GetTxtFromMgr(5929);
						pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, pExplain);
						return true;
					}

					SI32 siEnchantType = 0;

					if( pclCharItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].siUnique > 0 )
					{
						siEnchantType = pclCharItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].GetEnchantType(pclClient->pclItemManager);
					}

					//if( siEnchantType > 0 )
					//{
						SI32 siItemUnique = pclCharItem[pclClickAndDrop_Item->m_siPara1].siUnique;
						if(CheckEnchantMaterial(siEnchantType, siItemUnique))
						{
							// �Ӽ� ���漮�� ���
							if( siItemUnique == ENCHANT_CHANGE_ELEMENT_ITEMUNIQUE )
							{
								kTitleText	= GetTxtFromMgr(7940);
								pExplain	= GetTxtFromMgr(7949);

								if(pclclient->m_pDialog[NNEWITEMMAKE_DLG])
								{
									SI32				siEncnantType	= pclCharItem[pclClient->pclCM->CR[1]->siOrgInvEnchantInx].GetEnchantType(pclClient->pclItemManager);
									CNewMakeItemDlg*	tools			= (CNewMakeItemDlg*)pclclient->m_pDialog[NNEWITEMMAKE_DLG];

									(tools->NewEnchantTab)->SetChangeEnchanteButton(siEncnantType);
								}
								pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, pExplain);
							}
							personinventory->ShowEnchantElem(pclClickAndDrop_Item->m_siPara1);
						}
						else
						{
							pExplain = GetTxtFromMgr(5932);
							pclclient->m_pDialog[NOTIFYMSG_DLG ]->SetText(kTitleText, pExplain);
						}
					//}
					return true;
				}
				break;
			}
		}
		break;
	}

	return false;
}
