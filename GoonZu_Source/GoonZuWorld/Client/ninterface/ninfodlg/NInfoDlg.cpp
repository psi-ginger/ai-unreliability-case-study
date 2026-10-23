

#include "..\Client\Client.h"
#include "NInfoDlg.h"

#include "..\resource.h"

#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"
#include "../Client/ninterface/nvillagedetailinfo/NVillageDetailInfo.h"
//-----------------------------
// Common
//-----------------------------

//#include "HintDlg/HintDlg.h"

//-----------------------------
// CommonLogic
//-----------------------------


#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/InterfaceFile.h"


#include "../Client/NInterface/NItemInfoDlg/ItemInfoDlg.h"
#include "../Client/Ninterface/nmonsterinfodlg/NMonsterInfoDlg.h"

#include "../Client/Ninterface/nresidentsinfodlg/ResidentsInfoDlg.h"
#include "../Client/Ninterface/nhuntmapinfodlg/HuntMapInfoDlg.h"

#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Msg/MsgType-ItemMall.h"

extern cltCommonLogic* pclClient;

cltNInfoDlg::cltNInfoDlg()
{

	m_siCurrentPage = 0;
	m_timerCount = 0;
	

	m_pTabDlg0 = NULL;
	m_pTabDlg1 = NULL;
	m_pTabDlg2 = NULL;
	m_pTabDlg3 = NULL;
	m_pTabDlg4 = NULL;

	DlgRectInitSwitch = FALSE;

	m_LastTakeInOutTime = 0;

	m_pButton_item	 = NULL; 
	m_pButton_monster	 = NULL; 
	m_pButton_hunting	 = NULL; 
	m_pButton_village	 = NULL; 
	m_pButton_npc	 = NULL; 

	//추가
	//m_pComboBox_Type	= NULL;
	//m_pEdit_Name		= NULL;
	//m_pButton_Accept	= NULL;
	//m_pListBox_AutoMake	= NULL;
}

cltNInfoDlg::~cltNInfoDlg()
{
	if(m_pTabDlg0)
	{
		delete m_pTabDlg0;
		m_pTabDlg0 = NULL;
	}
	if(m_pTabDlg1)
	{
		delete m_pTabDlg1;
		m_pTabDlg1 = NULL;
	}
	if(m_pTabDlg2)
	{
		delete m_pTabDlg2;
		m_pTabDlg2 = NULL;
	}
	if(m_pTabDlg3)
	{
		delete m_pTabDlg3;
		m_pTabDlg3 = NULL;
	}
	if(m_pTabDlg4)
	{
		delete m_pTabDlg4;
		m_pTabDlg4 = NULL;
	}

	if( m_pButton_item )		delete m_pButton_item;
	if( m_pButton_monster )		delete m_pButton_monster;
	if( m_pButton_hunting )		delete m_pButton_hunting;
	if( m_pButton_village )		delete m_pButton_village;
	if( m_pButton_npc )			delete m_pButton_npc;
	//추가
	/*if (m_pComboBox_Type)		delete m_pComboBox_Type;
	if (m_pEdit_Name)			delete m_pEdit_Name;
	if (m_pButton_Accept)		delete m_pButton_Accept;
	if (m_pListBox_AutoMake)	delete m_pListBox_AutoMake;*/

	SetTimerOnOff( false );
	

}

void cltNInfoDlg::Create()// ( SI32  nDialogID )
{
	stTabTextInfo tmp_stTabTextInfo;
	cltClient *pclclient = (cltClient*)pclClient;

	if ( ! IsCreate() )
	{
	
		CInterfaceFile file;
		if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
		file.LoadFile(TEXT("NInterface/Data/NInfoDlg/dlg_Ninfo_parent.ddf"));
		}
		else{
			file.LoadFile(TEXT("NInterface/Data/NInfoDlg/dlg_Ninfo_parent_J.ddf"));
		}
		file.CreatePopupDialog( this, NINFO_DLG , TEXT("dialog_info"), NInfoDlgStaticProc );


		m_pButton_item			= new 	CButton	( this );
		m_pButton_monster		= new 	CButton	( this );
		m_pButton_hunting		= new 	CButton	( this );
		m_pButton_village		= new 	CButton	( this );
		m_pButton_npc			= new 	CButton	( this );
		////추가.
		//m_pComboBox_Type		= new 	CComboBox	( this );
		//m_pEdit_Name			= new 	CEdit		( this );
		//m_pButton_Accept		= new 	CButton		( this );
		//m_pListBox_AutoMake		= new	CList	( this );


		/*file.CreateControl( m_pListBox_AutoMake, NINFODLG_LISTBOX_AUTOBOX, TEXT("listbox_automake") );	*/

		file.CreateControl( m_pButton_item, NINFODLG_BUTTON_ITEM, TEXT("button_item") );
		file.CreateControl( m_pButton_monster, NINFODLG_BUTTON_MONSTER, TEXT("button_monster") );
		file.CreateControl( m_pButton_hunting, NINFODLG_BUTTON_HUNT, TEXT("button_hunt") );
		file.CreateControl( m_pButton_village, NINFODLG_BUTTON_VILLAGE, TEXT("button_village") );
		file.CreateControl( m_pButton_npc, NINFODLG_BUTTON_NPC, TEXT("button_npc") );
		//추가
		//file.CreateControl( m_pComboBox_Type, NINFODLG_COMBOBOX_TYPE, TEXT("combobox_type") );
		/*file.CreateControl( m_pEdit_Name, NINFODLG_EDIT_NAME, TEXT("editbox_name") );
		file.CreateControl( m_pButton_Accept, NINFODLG_BUTTON_ACCEPT, TEXT("button_accept") );*/

		MakeComboBox();
	
		m_siCurrentPage	= 0;

		if( !m_pTabDlg0 )
		{
			m_pTabDlg0 = new CNItemInfoDlg;
		}
		if( !m_pTabDlg1 )
		{
			m_pTabDlg1 = new CNMonsterInfoDlg;
		}	
		if( !m_pTabDlg2 )
		{
			m_pTabDlg2 = new CNHuntMapInfoDlg;
		}
		if( !m_pTabDlg3 )
		{
			m_pTabDlg3 = new CNVillageDetailInfo;
		}
		if( !m_pTabDlg4 )
		{
			m_pTabDlg4 = new CNResidentsInfoDlg;
		}

		if( !m_pTabDlg0->IsCreate() )
		{
			m_pTabDlg0->Create( this );		
		//	m_pTabDlg0->Hide();
		}
		if( !m_pTabDlg1->IsCreate() )
		{
			m_pTabDlg1->Create( this );
			m_pTabDlg1->Hide();
		}
		if( !m_pTabDlg2->IsCreate() )
		{
			m_pTabDlg2->Create( this );
			m_pTabDlg2->Hide();
		}
		if( !m_pTabDlg3->IsCreate() )
		{
			m_pTabDlg3->Create( this );
			m_pTabDlg3->Hide();
		}
		if( !m_pTabDlg4->IsCreate() )
		{
			m_pTabDlg4->Create( this );
			m_pTabDlg4->Hide();
		}


		if(m_siCurrentPage == 0 )
		{
			//m_pTabDlg0->Show( );

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_item, CONTROLLER_BUTTONTEXT_COLOR , FW_BOLD );
			SetTabTextInfo( &tmp_stTabTextInfo );

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_monster, CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL );
			SetTabTextInfo( &tmp_stTabTextInfo );

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_npc, CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL );
			SetTabTextInfo( &tmp_stTabTextInfo );

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_village, CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL );
			SetTabTextInfo( &tmp_stTabTextInfo );

			tmp_stTabTextInfo.Init();
			tmp_stTabTextInfo.Set( m_pButton_hunting, CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL );
			SetTabTextInfo( &tmp_stTabTextInfo );
		}


		m_pButton_item->Refresh();
		m_pButton_monster->Refresh();
		m_pButton_npc->Refresh();
		m_pButton_village->Refresh();
		m_pButton_hunting->Refresh();
		//m_pComboBox_Type->Refresh();
		//m_pEdit_Name->Refresh();
		//m_pButton_Accept->Refresh();
		/*m_pListBox_AutoMake->SetColumn(0,1,NULL);
		m_pListBox_AutoMake->SetColumn(1,160,NULL);
		m_pListBox_AutoMake->SetShowHeader(FALSE);
		m_pListBox_AutoMake->Show(false);*/
		SetActionTimer( 400 );
		SetTimerOnOff( true );
	
		Hide();
	}
	else
	{
 		if( IsShow() )
		{
			//Hide();
			if(pclclient->m_pDialog[NINFO_SUB_DLG])
				((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Hide();

			if(pclclient->m_pDialog[NINFO_DLG])
				((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->Hide();

		}
		else
			Show();
	}
}

void CALLBACK cltNInfoDlg::NInfoDlgStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNInfoDlg *pThis = (cltNInfoDlg*)pControllerMgr;
	pThis->NInfoDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK cltNInfoDlg::NInfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
 	cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case NINFODLG_BUTTON_ITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SelectTab(NINFODLG_BUTTON_ITEM);			

				}
				break;
			}
		}
		break;
	case NINFODLG_BUTTON_MONSTER:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SelectTab(NINFODLG_BUTTON_MONSTER);
				}
				break;
			}
		}
		break;
	case NINFODLG_BUTTON_HUNT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SelectTab(NINFODLG_BUTTON_HUNT);
				}
				break;
			}
		}
		break;
	case NINFODLG_BUTTON_VILLAGE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SelectTab(NINFODLG_BUTTON_VILLAGE);
				}
				break;
			}
		}
		break;
	case NINFODLG_BUTTON_NPC:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SelectTab(NINFODLG_BUTTON_NPC);
				}
				break;
			}
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//m_pListBox_AutoMake->Show(false);

					if(GetDialogControllerID() == NINFO_DLG)
					{

						if(pclclient->m_pDialog[NINFO_SUB_DLG])
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Hide();

						if(pclclient->m_pDialog[NINFO_DLG])
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->Hide();
					}
					else if(GetDialogControllerID() == NINFO_SUB_DLG)
					{
						if(pclclient->m_pDialog[NINFO_SUB_DLG])
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Hide();


					}
				}
				break;
			}
		}
		break;
	//case NINFODLG_COMBOBOX_TYPE:
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_COMBOBOX_SELECTION_CHANGED://콤보박스에서 해당 탭 선택
	//			{
	//			//	SI16 SelectIndex = m_pComboBox_Type->GetSelectedIndex();
	//			//	if (SelectIndex < 0)	return;
	//			//	//SelectTab(SelectIndex);

	//			}
	//		}
	//	}break;
	/*case NINFODLG_EDIT_NAME:
		{
			switch(nEvent) {
				case EVENT_EDITBOX_CHANGE:{
					}break;
				case EVENT_EDITBOX_STRING:{
					SI16 Selectindex = m_pComboBox_Type->GetSelectedIndex();
					TCHAR* pItemname = m_pEdit_Name->GetText();
					if (Selectindex < 0)	return;
					if (pItemname == NULL)	return;					
					Find(Selectindex,pItemname);
					}break;
			}
		}*/
	/*case NINFODLG_BUTTON_ACCEPT:
		{
			switch(nEvent) {
				
				case EVENT_BUTTON_CLICKED:{
					SI16 Selectindex = m_pComboBox_Type->GetSelectedIndex();
					TCHAR* pItemname = m_pEdit_Name->GetText();
					if (Selectindex < 0)	return;
					if (pItemname == NULL)	return;
					Find(Selectindex,pItemname);
										}break;
			}
		}*/
	/*case NINFODLG_LISTBOX_AUTOBOX:
		{
			switch(nEvent) {
				case EVENT_LISTBOX_SELECTION:
					{
					 SI16 SelectIndex= m_pListBox_AutoMake->GetSelectedIndex();
					 if(SelectIndex >= 0)
					 {
						 TCHAR* pItemname = m_pListBox_AutoMake->GetText(SelectIndex,1);
						 m_pEdit_Name->SetText(pItemname);
						 m_pEdit_Name->Refresh();
					 }
					 m_pListBox_AutoMake->Show(false);
					}break;
			}

		}*/
	case WM_TIMER:
		{
		}
	}
}

void cltNInfoDlg::SetTabTextInfo( stTabTextInfo* in_SelectedText )
{
	if( in_SelectedText && in_SelectedText->st_pControllerObj )
	{
		in_SelectedText->st_pControllerObj->SetFontColor( in_SelectedText->st_Color );
		in_SelectedText->st_pControllerObj->SetFontWeight( in_SelectedText->st_siWeight );
		in_SelectedText->st_pControllerObj->Refresh();
	}
}

void cltNInfoDlg::Action()
{
	if( false == IsShow() )
		return;

	if( m_timerCount >= LEAST_COMMON_MULTILPLE + 1 )
		m_timerCount = 1;

	
	switch( m_siCurrentPage )
	{
	case 0:	// 아이템 탭
		if(m_pTabDlg0 && m_pTabDlg0->IsCreate() && m_pTabDlg0->IsShow())
			m_pTabDlg0->Action();
		break;

	case 1:	// 몬스터 탭
		if(m_pTabDlg1 && m_pTabDlg1->IsCreate() && m_pTabDlg1->IsShow())
			m_pTabDlg1->Action();
		break;
	case 2:	// 사냥터 탭
		if(m_pTabDlg2 && m_pTabDlg2->IsCreate() && m_pTabDlg2->IsShow())
		{
			m_pTabDlg2->Action();
		}
		break;
	case 3:	// 국가 정보 탭
		if(m_pTabDlg3 && m_pTabDlg3->IsCreate() && m_pTabDlg3->IsShow())
		{
			//if( m_timerCount % ASSET_TAB_ACTION_INTERVAL_VALUE == 0 )
			//{
				m_pTabDlg3->Action();
			//}
		}
		break;
	case 4:	// npc 정보 탭
		if(m_pTabDlg4 && m_pTabDlg4->IsCreate() && m_pTabDlg4->IsShow())
		{
			m_pTabDlg4->Action();
		}
		break;

	}	

	m_timerCount++;
	Update();
}

void cltNInfoDlg::Hide()
{
	m_bShow = false;
	CControllerMgr::Show( false );
}

void cltNInfoDlg::Show()
{
	m_bShow = true;

	switch( m_siCurrentPage )
	{
	case 0:	// 아이템
		if(m_pTabDlg0 && m_pTabDlg0->IsCreate() && m_pTabDlg0->IsShow())
		{
			m_pTabDlg0->Show();
			m_pTabDlg0->Action();
		}
		break;

	case 1:	// 몬스터
		if(m_pTabDlg1 && m_pTabDlg1->IsCreate() && m_pTabDlg1->IsShow())
			m_pTabDlg1->Action();
		break;
	case 2:	// 사냥터
		if(m_pTabDlg2 && m_pTabDlg2->IsCreate() && m_pTabDlg2->IsShow())
		{
			m_pTabDlg2->Action();
		}
		break;
	case 3:	// 마을
		if(m_pTabDlg3 && m_pTabDlg3->IsCreate() && m_pTabDlg3->IsShow())
		{
			m_pTabDlg3->Action();
		}
		break;
	case 4:	// NPC
		//if(m_pTabDlg4 && m_pTabDlg4->IsCreate() && m_pTabDlg4->IsShow())
		//{
			m_pTabDlg4->Action();
		//}
		break;		
	}

	CControllerMgr::Show( true );
}

bool cltNInfoDlg::IsShow()
{
	return m_bShow;
}

void cltNInfoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 3:
		{
            m_pTabDlg2->RetMsgBox(RetValue,MsgIndex,pData,DataLen);
		}
		break;
	case 4:
		{
			m_pTabDlg3->RetMsgBox(RetValue,MsgIndex,pData,DataLen);
		}
		break;
	case 5:
		{
			m_pTabDlg4->RetMsgBox(RetValue,MsgIndex,pData,DataLen);
		}
		break;

	default:
		break;
	}
}

void cltNInfoDlg::SelectTab( SI32 ControlID )
{
	m_timerCount = 0;	// Timer Counting Initialization!

	if( ControlID == NINFODLG_BUTTON_ITEM  )
	{
		m_pTabDlg0->Show();
		m_siCurrentPage = 0;

		m_pButton_item->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_item->SetFontWeight(FW_BOLD);		
		m_pButton_item->Refresh();
	}
	else
	{
		m_pTabDlg0->Hide();
		m_pButton_item->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_item->SetFontWeight(FW_NORMAL);		
		m_pButton_item->Refresh();
	}

	if( ControlID == NINFODLG_BUTTON_MONSTER  )
	{
		m_pTabDlg1->Show();
		m_siCurrentPage = 1;

		m_pButton_monster->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_monster->SetFontWeight(FW_BOLD);
		m_pButton_monster->Refresh();
	}
	else
	{
		m_pTabDlg1->Hide();
		m_pButton_monster->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_monster->SetFontWeight(FW_NORMAL);		
		m_pButton_monster->Refresh();
	}

	if( ControlID == NINFODLG_BUTTON_HUNT  )
	{
		m_pTabDlg2->Show();
		m_siCurrentPage = 2;

		m_pButton_hunting->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_hunting->SetFontWeight(FW_BOLD);
		m_pButton_hunting->Refresh();
	}
	else
	{
		m_pTabDlg2->Hide();
		m_pButton_hunting->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_hunting->SetFontWeight(FW_NORMAL);
		m_pButton_hunting->Refresh();
	}

	if( ControlID == NINFODLG_BUTTON_VILLAGE  )
	{
		m_pTabDlg3->Show();
		m_siCurrentPage = 3;

		m_pButton_village->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_village->SetFontWeight(FW_BOLD);
		m_pButton_village->Refresh();
	}
	else
	{
		m_pTabDlg3->Hide();
		m_pButton_village->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_village->SetFontWeight(FW_NORMAL);
		m_pButton_village->Refresh();
	}

	if( ControlID == NINFODLG_BUTTON_NPC  )
	{
		m_pTabDlg4->Show();
		m_siCurrentPage = 4;

		m_pButton_npc->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_npc->SetFontWeight(FW_BOLD);
		m_pButton_npc->Refresh();
	}
	else
	{
		m_pTabDlg4->Hide();    
		m_pButton_npc->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_npc->SetFontWeight(FW_NORMAL);
		m_pButton_npc->Refresh();
	}
}

void cltNInfoDlg::GeneralMeetingSuggestInfo(void)
{
	SelectTab( NINFODLG_BUTTON_VILLAGE );
	m_pTabDlg3->GeneralMeetingSuggestInfo();
}
void cltNInfoDlg::MakeComboBox()
{
	//TCHAR buffer[256];
	//stComboBoxItemData ComboBoxItemData;
	//TCHAR* pTextItem = GetTxtFromMgr(1928);//아이템
	//TCHAR* pTextMonster = GetTxtFromMgr(3291);
	//TCHAR* pTextHuntmap = GetTxtFromMgr(6085);
	//TCHAR* pTextVillage = GetTxtFromMgr(6086);
	//TCHAR* pTextNPC = GetTxtFromMgr(6087);

	//sprintf( buffer,pTextItem);
	//ComboBoxItemData.Init();
	//ComboBoxItemData.Set( buffer );
	//m_pComboBox_Type->AddItem( &ComboBoxItemData );
	//m_pComboBox_Type->Refresh();

	//sprintf( buffer,pTextMonster);
	//ComboBoxItemData.Init();
	//ComboBoxItemData.Set( buffer );
	//m_pComboBox_Type->AddItem( &ComboBoxItemData );
	//m_pComboBox_Type->Refresh();
	//
	//sprintf( buffer,pTextHuntmap);
	//ComboBoxItemData.Init();
	//ComboBoxItemData.Set( buffer );
	//m_pComboBox_Type->AddItem( &ComboBoxItemData );
	//m_pComboBox_Type->Refresh();
	//
	//sprintf( buffer,pTextVillage);
	//ComboBoxItemData.Init();
	//ComboBoxItemData.Set( buffer );
	//m_pComboBox_Type->AddItem( &ComboBoxItemData );
	//m_pComboBox_Type->Refresh();
	//
	//sprintf( buffer,pTextNPC);
	//ComboBoxItemData.Init();
	//ComboBoxItemData.Set( buffer );
	//m_pComboBox_Type->AddItem( &ComboBoxItemData );
	//m_pComboBox_Type->Refresh();
}
void cltNInfoDlg :: SelectTab(SI16 Selectindex)//콤보박스를 위해 추가
{
	if( Selectindex == 0  )
	{
		m_pTabDlg0->Show();
		m_siCurrentPage = 0;
		m_pButton_item->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_item->SetFontWeight(FW_BOLD);		
		m_pButton_item->Refresh();
	}
	else
	{
		m_pTabDlg0->Hide();
		m_pButton_item->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_item->SetFontWeight(FW_NORMAL);		
		m_pButton_item->Refresh();
	}

	if( Selectindex == 1  )
	{
		m_pTabDlg1->Show();
		m_siCurrentPage = 1;

		m_pButton_monster->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_monster->SetFontWeight(FW_BOLD);
		m_pButton_monster->Refresh();
	}
	else
	{
		m_pTabDlg1->Hide();
		m_pButton_monster->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_monster->SetFontWeight(FW_NORMAL);		
		m_pButton_monster->Refresh();
	}

	if( Selectindex == 2  )
	{
		m_pTabDlg2->Show();
		m_siCurrentPage = 2;

		m_pButton_hunting->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_hunting->SetFontWeight(FW_BOLD);
		m_pButton_hunting->Refresh();
	}
	else
	{
		m_pTabDlg2->Hide();
		m_pButton_hunting->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_hunting->SetFontWeight(FW_NORMAL);
		m_pButton_hunting->Refresh();
	}

	if( Selectindex == 3  )
	{
		m_pTabDlg3->Show();
		m_siCurrentPage = 3;

		m_pButton_village->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_village->SetFontWeight(FW_BOLD);
		m_pButton_village->Refresh();
	}
	else
	{
		m_pTabDlg3->Hide();
		m_pButton_village->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_village->SetFontWeight(FW_NORMAL);
		m_pButton_village->Refresh();
	}

	if( Selectindex == 4  )
	{
		m_pTabDlg4->Show();
		m_siCurrentPage = 4;

		m_pButton_npc->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_npc->SetFontWeight(FW_BOLD);
		m_pButton_npc->Refresh();
	}
	else
	{
		m_pTabDlg4->Hide();    
		m_pButton_npc->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_npc->SetFontWeight(FW_NORMAL);
		m_pButton_npc->Refresh();
	}
}
void cltNInfoDlg :: Find(SI16 Selectindex, TCHAR* pItemname)//검색기능을 위해
{
	//아이템검색
	if (Selectindex == 0) {
		m_pTabDlg0->Show();
		m_siCurrentPage = 0;
		m_pButton_item->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_item->SetFontWeight(FW_BOLD);		
		m_pButton_item->Refresh();
		m_pTabDlg0->ItemFind(pItemname);
	}
	else
	{
		m_pTabDlg0->Hide();
		m_pButton_item->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_item->SetFontWeight(FW_NORMAL);		
		m_pButton_item->Refresh();
	}
	//몬스터 검색
	if( Selectindex == 1  )
	{
		m_pTabDlg1->Show();
		m_siCurrentPage = 1;

		m_pButton_monster->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_monster->SetFontWeight(FW_BOLD);
		m_pButton_monster->Refresh();
		m_pTabDlg1->MonsterFind(pItemname);
	}
	else
	{
		m_pTabDlg1->Hide();
		m_pButton_monster->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_monster->SetFontWeight(FW_NORMAL);		
		m_pButton_monster->Refresh();
	}
	//사냥터 검색
	if( Selectindex == 2  )
	{
		m_pTabDlg2->Show();
		m_siCurrentPage = 2;

		m_pButton_hunting->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_hunting->SetFontWeight(FW_BOLD);
		m_pButton_hunting->Refresh();
		m_pTabDlg2->FindHuntMap(pItemname);
	}
	else
	{
		m_pTabDlg2->Hide();
		m_pButton_hunting->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_hunting->SetFontWeight(FW_NORMAL);
		m_pButton_hunting->Refresh();
	}
	//마을 검색
	if( Selectindex == 3  )
	{
		m_pTabDlg3->Show();
		m_siCurrentPage = 3;

		m_pButton_village->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_village->SetFontWeight(FW_BOLD);
		m_pButton_village->Refresh();
		m_pTabDlg3->FindVillage(pItemname);
	}
	else
	{
		m_pTabDlg3->Hide();
		m_pButton_village->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_village->SetFontWeight(FW_NORMAL);
		m_pButton_village->Refresh();
	}
	//npc검색
	if( Selectindex == 4  )
	{
		m_pTabDlg4->Show();
		m_siCurrentPage = 4;

		m_pButton_npc->SetFontColor(CONTROLLER_BUTTONTEXT_COLOR);
		m_pButton_npc->SetFontWeight(FW_BOLD);
		m_pButton_npc->Refresh();
		m_pTabDlg4->FindNpc(pItemname);
	}
	else
	{
		m_pTabDlg4->Hide();    
		m_pButton_npc->SetFontColor(CONTROLLER_BACK_BUTTONTEXT_COLOR);
		m_pButton_npc->SetFontWeight(FW_NORMAL);
		m_pButton_npc->Refresh();
	}
}

//자동완성 기능을 위해서 
void cltNInfoDlg ::Update()
{
	//cltClient *pclclient = (cltClient *)pclClient;
	//NTCHARString64		kInputText(m_pEdit_Name->GetText());

	//if( m_pEdit_Name->IsFocus() == false && 
	//	!(m_pListBox_AutoMake->IsFocus() == true || m_pListBox_AutoMake->GetScroll()->IsFocus() == true) )
	//{
	//	m_pListBox_AutoMake->Clear();
	//	m_pListBox_AutoMake->Refresh();
	//	m_pListBox_AutoMake->Show(false);
	//	//pclClient->pclLog->LogPrint("Lose Focus\n");
	//	return;
	//}

	//if (kInputText.IsEmpty())
	//{
	//	m_pListBox_AutoMake->Clear();
	//	m_pListBox_AutoMake->Refresh();
	//	m_pListBox_AutoMake->Show(false);
	//	m_LastInput = kInputText;
	//	return;
	//}

	//// 같은 내용은 처리하지 않는다.
	//if (m_LastInput == kInputText)		return;

	//m_pListBox_AutoMake->Clear();

	//switch(m_pComboBox_Type->GetSelectedIndex()) {
	//case 0://아이템
	//	{
	//		for(SI32 i = 0; i <MAX_ITEMINFO_NUMBER; i++) {
	//			cltItemInfo* pinfo = pclclient->pclItemManager->pclItemInfo[i];
	//			if(pinfo == NULL)continue;			
	//			TCHAR* findname = _tcsstr(pinfo->GetName(), kInputText);
	//			if(findname)		AddListItem((TCHAR*)pinfo->GetName());
	//		}
	//	}
	//	break;
	//case 1://몬스터
	//	{
	//		cltKindInfo* pclki= NULL;
	//		TCHAR	m_szSortName[MAX_KIND_NUMBER][MAX_KIND_NAME_LENGTH] ;
	//		for ( SI32 i = 0; i < MAX_KIND_NUMBER ; ++i )
	//		{
	//			StringCchCopy(m_szSortName[i], MAX_KIND_NAME_LENGTH, TEXT(""));
	//			pclki = pclClient->pclKindInfoSet->pclKI[ i ];

	//			if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
	//				continue;

	//			switch (pclClient->siServiceArea)
	//			{
	//			case ConstServiceArea_Korea :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_China :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_Japan :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_English :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_Taiwan :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_USA :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
	//					continue;
	//				break;
	//			}
	//			if ( ConstServiceArea_China == pclClient->siServiceArea )
	//			{
	//				if ( pclki->siSortLevel <= 0 || pclki->siSortLevel >= MAX_KIND_NUMBER ) continue ;
	//			}
	//			StringCchCopy(m_szSortName[i], MAX_KIND_NAME_LENGTH, (TCHAR*)pclki->GetName());
	//			TCHAR* findname = _tcsstr(m_szSortName[i], kInputText);
	//			if(findname)		AddListItem(m_szSortName[i]);
	//		}			
	//	}
	//	break;
	//case 2://사냥터
	//	{

	//		NDataLoader	dataloader;
	//		BOOL bLoadFile=FALSE;

	//		//KHY - 0810 - 텍스트 리소스 암호화.
	//		if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	//		{
	//			bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/HuntmapInfo.txt") );
	//		}
	//		else
	//		{
	//			bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/HuntmapInfo.txt") );
	//		}

	//		if (!bLoadFile)
	//		{
	//			MsgBox( TEXT("Not LoadFromFile HuntmapInfo"),TEXT("Error"));
	//			return;
	//		}

	//		TCHAR maptypename[256] = TEXT(""), mapunique[256] = TEXT("");
	//		SI32 ptextnum, mapinx, imapunique;

	//		NDataTypeInfo pDataTypeInfo[] =
	//		{
	//			NDATA_INT32,	&ptextnum,   4,		
	//				NDATA_MBSTRING,	mapunique,	256,
	//				NDATA_INT32,	&mapinx,	4,
	//				0,0,0
	//		};

	//		dataloader.RegDataType( pDataTypeInfo );


	//		while( !dataloader.IsEndOfData() ) {

	//			if( dataloader.ReadData() ) 
	//			{
	//				if ( mapunique[ 0 ] != NULL)
	//				{
	//					if (pclClient->pclHashTableString->Find(mapunique, imapunique))
	//					{
	//						TCHAR* findname = _tcsstr(GetTxtFromMgr(ptextnum), kInputText);
	//						if(findname)		AddListItem(GetTxtFromMgr(ptextnum));
	//						

	//					}
	//				}
	//			}
	//		}			
	//	}
	//	break;
	//case 3://마을
	//	{
	//		for (SI32 i = 1 ;i < MAX_VILLAGE_NUMBER; i++ ) {
	//			cltVillageInfo* pInfo = pclclient->pclVillageManager->pclVillageInfo[i];
	//			if(pInfo == NULL) continue;
	//			TCHAR* findname = _tcsstr(pInfo->szName, kInputText);				
	//			if(findname)	AddListItem(pInfo->szName);
	//		}	
	//	}
	//	break;
	//case 4://npc
	//	{
	//		cltKindInfo* pclki = NULL;
	//		for ( SI32 i = 10; i < MAX_KIND_NUMBER; ++i )
	//		{
	//			pclki = pclClient->pclKindInfoSet->pclKI[ i ];
	//			//pclClient->pclKindInfoSet->pclKI[ siMonsterKind ]->siAtb == ATB_ATTACK

	//			if( pclki == NULL || pclki->IsAtb( ATB_SYSTEMNPC ) == false ) continue;
	//			if(pclki->IsAtb( ATB_NONPCLIST ) )continue;

	//			switch (pclClient->siServiceArea)
	//			{
	//			case ConstServiceArea_Korea :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_China :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_Japan :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_English :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_Taiwan :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
	//					continue;
	//				break;
	//			case ConstServiceArea_USA :
	//				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
	//					continue;
	//				break;

	//			}
	//			TCHAR* findname = _tcsstr((TCHAR*)pclki->GetName(), kInputText);
	//			if(findname)		AddListItem((TCHAR*)pclki->GetName());
	//		}
	//	}
	//	break;
	//}

	//m_pListBox_AutoMake->Refresh();
	//m_pListBox_AutoMake->Show(true);

	//if( m_pListBox_AutoMake->GetListNum() == 1 )
	//{
	//	NTCHARString64 kText(m_pListBox_AutoMake->GetText(0, 1));
	//	if( kText == kInputText)
	//	{
	//		m_pListBox_AutoMake->Show(false);
	//	}
	//}

	//m_LastInput = kInputText;
}

void cltNInfoDlg :: AddListItem(TCHAR* findname)
{/*
	if(findname == NULL)	return;
	SI16 cnt = 0;
	stListBoxItemData Data;
	
	Data.Init();
	Data.Set( 1, findname );
	m_pListBox_AutoMake->AddItem(&Data);*/
}

void cltNInfoDlg :: Close()
{
	cltClient * pclclient = (cltClient*) pclClient;

	if(GetDialogControllerID() == NINFO_DLG)
	{

		if(pclclient->m_pDialog[NINFO_SUB_DLG])
			((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Hide();

		if(pclclient->m_pDialog[NINFO_DLG])
			((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->Hide();
	}
	else if(GetDialogControllerID() == NINFO_SUB_DLG)
	{
		if(pclclient->m_pDialog[NINFO_SUB_DLG])
			((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Hide();
	}
}


//#endif

