#include "NNewMakeItemDlg.h"



#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "Char\CharManager\CharManager.h"
#include "../../../CommonLogic/Msg/MsgType-Quest.h"
#include "../../../CommonLogic/MsgType-Item.h"

#include "NInterface/NNewInventory/NNewInventory.h"


extern cltCommonLogic* pclClient;

CNewMakeItemDlg::CNewMakeItemDlg()
{	
	NewMakeTab = NULL;
	NewEnchantTab= NULL; 
	NewItemRepair = NULL;
}

CNewMakeItemDlg::~CNewMakeItemDlg()
{	
	SetTimerOnOff(false);
	SAFE_DELETE(NewMakeTab);
	SAFE_DELETE(NewEnchantTab);
	SAFE_DELETE(NewItemRepair);
}

void CNewMakeItemDlg::Create()
{
	if ( !IsCreate() )
	{
		CInterfaceFile file;

		if (pclClient->siServiceArea == ConstServiceArea_Korea){

			file.LoadFile(TEXT("NInterface/Data/NNewMakeItem/DLG_NNewMakeItem_k.ddf"));

		}

		else{
			file.LoadFile(TEXT("NInterface/Data/NNewMakeItem/DLG_NNewMakeItem.ddf"));
		}

		file.CreatePopupDialog( this, NNEWITEMMAKE_DLG, TEXT("dialog_NewMakeItem"), StatiCNewMakeItemDlgProc);
	
	//	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEWMAKE_STATIC , this ), NEWMAKE_STATIC,   "statictext_NONAME1"); 	
  		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON2, this ), NEWMAKE_BUTTON2,   "button_enchant"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON1, this ), NEWMAKE_BUTTON1,   "button_makeitem");
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON_REPAIR, this ), NEWMAKE_BUTTON_REPAIR,   "button_Repair"); 

		if( pclClient->IsCountrySwitch( Switch_NewDurability) )
		{

		}
		else
		{
			CButton* pclButton_Repair = m_InterfaceFactory.GetButton( NEWMAKE_BUTTON_REPAIR );
			if( pclButton_Repair )			pclButton_Repair->Show( false );
		}
	
		NewItemRepair = new NItemRepairDlg();
		NewItemRepair->Create(this);
		NewItemRepair->Show( false );

		NewItemRepair->SetMode( 1 );

		NewEnchantTab = new CNNewEnchantChild();
		NewEnchantTab->Create(this);
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->SetFontIndex(4);
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->Enable(false);

		NewEnchantTab->Hide();
		
		NewMakeTab = new CNNewMakeChild();
		NewMakeTab->Create(this);				
		NewMakeTab->Show();					
		MovePos(635,205);

		SetTimerOnOff(true);

	}
}


void CALLBACK CNewMakeItemDlg::StatiCNewMakeItemDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNewMakeItemDlg *pThis = (CNewMakeItemDlg*) pControllerMgr;
	pThis->NNewMakeItemDlgProc( nEvent, nControlID, pControl );
}



void CALLBACK CNewMakeItemDlg::NNewMakeItemDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
 			if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
			{	
				CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
				for(SI32 i =0 ; i <4; i++)
				{
					CNNewInventory::clMakeItemContainer.cltools[i].Init();
					CNNewInventory::clMakeItemContainer.m_sitools[i] = 0;
				}	
				for(SI32 i =0 ; i <25; i++)
				{
					CNNewInventory::clMakeItemContainer.clMaterials[i].Init();
					CNNewInventory::clMakeItemContainer.m_siMaterials[i] = 0;
				}
				
				CNNewInventory::clMakeItemContainer.clOrgEnchantItem.Init();
				CNNewInventory::clMakeItemContainer.clMatEnchantItem.Init();
				CNNewInventory::clMakeItemContainer.clEleEnchantItem[0].Init();
				CNNewInventory::clMakeItemContainer.clEleEnchantItem[1].Init();
				CNNewInventory::clMakeItemContainer.clEleEnchantItem[2].Init();

				
				pclclient->pclCM->CR[1]->siMatInvEnchantInx = 0;
				pclclient->pclCM->CR[1]->siOrgInvEnchantInx = 0;
				pclclient->pclCM->CR[1]->siElementType = 0;
				pclclient->pclCM->CR[1]->siElementEnchantInx[0] = 0;
				pclclient->pclCM->CR[1]->siElementEnchantInx[1] = 0;
				pclclient->pclCM->CR[1]->siElementEnchantInx[2] = 0;

				equip->LoadItemData();
				((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG])->LoadItemData();
				((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG])->LoadItemData();
				((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG])->LoadItemData();
			
			}
			DeleteDialog();
			

		}
		break;

	case NEWMAKE_BUTTON1:
		{				
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					ShowMakeTab();
				}
				break;
			}
		}
		break;

	case NEWMAKE_BUTTON2:
		{
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					ShowEnchantTab();
				}
				break;
			}
		}
		break;

	case NEWMAKE_BUTTON_REPAIR:
		{
			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
					if( pclchar )
					{
						if( pclchar->bRepairShopSwitch )
						{
							cltGameMsgRequest_NewItemRepairRequestInfo clinfo( pclClient->pclCM->CR[1]->GetCharUnique(), NEWITEMREPAIR_INFO_ITEM );
							cltMsg clMsg(GAMEMSG_REQUEST_NEWITEMREPAIRREQUESTINFO, sizeof(clinfo), (BYTE*)&clinfo);
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

							NewItemRepair->SetMode( 4 );
						}
						else
						{
							NewItemRepair->SetMode( 1 );
						}
					}
					ShowRepairTab();
				}
				break;
			}
		}
		break;
	}
	return ;
}

void CNewMakeItemDlg::ShowMakeTab()
{
	if(NewMakeTab->IsShow())
		return;
	if(NewEnchantTab->IsShow())
		NewEnchantTab->Hide();
	if(NewItemRepair->IsShow())
		NewItemRepair->Show( false );
	NewMakeTab->Show();


	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->Enable(false);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON2)->Enable(true);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_REPAIR)->Enable(true);
}

void CNewMakeItemDlg::ShowEnchantTab()
{
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->SetFontIndex(4);
	
	if(NewEnchantTab->IsShow())
		return;
	if(NewMakeTab->IsShow())
		NewMakeTab->Hide();
	if(NewItemRepair->IsShow())
		NewItemRepair->Show( false );
	NewEnchantTab->Show();				

	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON2)->Enable(false);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->Enable(true);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_REPAIR)->Enable(true);

	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_OPENENCHANTITEM;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		cltClient* pclclient = (cltClient* )pclClient;
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}

void CNewMakeItemDlg::ShowRepairTab()
{
//	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->SetFontIndex(4);

	if(NewItemRepair && NewItemRepair->IsShow())		return;

	if( NewMakeTab && NewMakeTab->IsShow() )		NewMakeTab->Hide();
	if( NewEnchantTab && NewEnchantTab->IsShow() )	NewEnchantTab->Hide();
	
	NewItemRepair->Show( true );

	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON2)->Enable(true);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->Enable(true);
	m_InterfaceFactory.GetButton(NEWMAKE_BUTTON_REPAIR)->Enable(false);
}

void CNewMakeItemDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch (MsgIndex) 
	{
		case MSGBOX_ID_CHANGEENCHANT:
			{
				if (RetValue == true) // YES¸¦ ´­·¶À»¶§
				{
					stChangeEnchantMsgPara stPara;
					memcpy( &stPara, pData, DataLen );

					NewEnchantTab->CheckChangeEnchantItem( stPara.siUnique, stPara.siElementIndex );
				}
				else
				{
					stChangeEnchantMsgPara stPara;
					memcpy( &stPara, pData, DataLen );
					
					NewEnchantTab->SetChangeEnchanteButton( stPara.siEnchantType );
				}
			}
			break;
	}
}
void CNewMakeItemDlg::CloseDialog()
{
	cltClient *pclclient = (cltClient*)pclClient;

	if(pclclient->m_pDialog[NNEWINVENTORY_DLG])
	{	
		CNNewInventory* equip = (CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG];
		for(SI32 i =0 ; i <4; i++)
		{
			CNNewInventory::clMakeItemContainer.cltools[i].Init();
			CNNewInventory::clMakeItemContainer.m_sitools[i] = 0;
		}	
		for(SI32 i =0 ; i <25; i++)
		{
			CNNewInventory::clMakeItemContainer.clMaterials[i].Init();
			CNNewInventory::clMakeItemContainer.m_siMaterials[i] = 0;
		}

		CNNewInventory::clMakeItemContainer.clOrgEnchantItem.Init();
		CNNewInventory::clMakeItemContainer.clMatEnchantItem.Init();
		CNNewInventory::clMakeItemContainer.clEleEnchantItem[0].Init();
		CNNewInventory::clMakeItemContainer.clEleEnchantItem[1].Init();
		CNNewInventory::clMakeItemContainer.clEleEnchantItem[2].Init();


		pclclient->pclCM->CR[1]->siMatInvEnchantInx = 0;
		pclclient->pclCM->CR[1]->siOrgInvEnchantInx = 0;
		pclclient->pclCM->CR[1]->siElementType = 0;
		pclclient->pclCM->CR[1]->siElementEnchantInx[0] = 0;
		pclclient->pclCM->CR[1]->siElementEnchantInx[1] = 0;
		pclclient->pclCM->CR[1]->siElementEnchantInx[2] = 0;

		equip->LoadItemData();
		((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_DLG	])->LoadItemData();
		((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_HORSE_DLG])->LoadItemData();
		((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG0_DLG])->LoadItemData();
		((CNNewInventory*)pclclient->m_pDialog[NNEWINVENTORY_BAG1_DLG])->LoadItemData();

	}
	DeleteDialog();
}