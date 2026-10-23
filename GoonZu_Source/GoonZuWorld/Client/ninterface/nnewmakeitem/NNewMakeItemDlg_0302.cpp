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

#include "NInterface/NNewInventory/NNewInventory.h"


extern cltCommonLogic* pclClient;

CNewMakeItemDlg::CNewMakeItemDlg()
{	
	NewMakeTab = NULL;
	NewEnchantTab= NULL; 

}

CNewMakeItemDlg::~CNewMakeItemDlg()
{	
	SetTimerOnOff(false);
	SAFE_DELETE(NewMakeTab);
	SAFE_DELETE(NewEnchantTab);
}

void CNewMakeItemDlg::Create()
{
	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NNewMakeItem/DLG_NNewMakeItem.ddf"));
		file.CreatePopupDialog( this, NNEWITEMMAKE_DLG, TEXT("dialog_NewMakeItem"), StatiCNewMakeItemDlgProc);
	
	//	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NEWMAKE_STATIC , this ), NEWMAKE_STATIC,   "statictext_NONAME1"); 	
  		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON2, this ), NEWMAKE_BUTTON2,   "button_enchant"); 	
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NEWMAKE_BUTTON1, this ), NEWMAKE_BUTTON1,   "button_makeitem"); 
		
 
		NewEnchantTab = new CNNewEnchantChild();
		NewEnchantTab->Create(this);
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->SetFontIndex(4);
		m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->Enable(false);

		NewEnchantTab->Hide();
		

		NewMakeTab = new CNNewMakeChild();
		NewMakeTab->Create(this);				
		NewMakeTab->Show();					
		MovePos(650,205);

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
					equip->cltools[i].Init();
					equip->m_sitools[i] = 0;
				}	
				for(SI32 i =0 ; i <25; i++)
				{
					equip->clMaterials[i].Init();
					equip->m_siMaterials[i] = 0;
				}
		
				cltItem clOrgEnchantItem;		//인챈트 시킬 아이템
				cltItem clMatEnchantItem;		//인챈트로 인해 소진될 아이템
				cltItem clEleEnchantItem[3];	//인챈트 속성돌아이템
				
				clOrgEnchantItem.Init();
				clMatEnchantItem.Init();
				clEleEnchantItem[0].Init();
				clEleEnchantItem[1].Init();
				clEleEnchantItem[2].Init();

				SI32			siOrgInvEnchantInx;  
				SI32			siMatInvEnchantInx;
				SI32			siElementEnchantInx[3];
				SI32			siElementType;

				pclclient->pclCM->CR[1]->siMatInvEnchantInx = 0;
				pclclient->pclCM->CR[1]->siOrgInvEnchantInx = 0;
				pclclient->pclCM->CR[1]->siElementType = 0;
				pclclient->pclCM->CR[1]->siElementEnchantInx[0] = 0;
				pclclient->pclCM->CR[1]->siElementEnchantInx[1] = 0;
				pclclient->pclCM->CR[1]->siElementEnchantInx[2] = 0;

				equip->LoadItemData();
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
					
					if(NewMakeTab->IsShow())
						break;
					if(NewEnchantTab->IsShow())
						NewEnchantTab->Hide();
					NewMakeTab->Show();
	
               
					m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->Enable(false);
					m_InterfaceFactory.GetButton(NEWMAKE_BUTTON2)->Enable(true);
					

				}
				break;
			}
		}
		break;

	case NEWMAKE_BUTTON2:
		{

			/////////////// 제조 봉인 /////////////////
			//return;

			switch(nEvent)	
			{				
			case EVENT_BUTTON_CLICKED:
				{
					m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->SetFontIndex(4);
					if(NewEnchantTab->IsShow())
						break;
					if(NewMakeTab->IsShow())
						NewMakeTab->Hide();
					NewEnchantTab->Show();				
					
					
					m_InterfaceFactory.GetButton(NEWMAKE_BUTTON2)->Enable(false);
					m_InterfaceFactory.GetButton(NEWMAKE_BUTTON1)->Enable(true);
				}
				break;
			}
		}
		break;

	}
	return ;
}

