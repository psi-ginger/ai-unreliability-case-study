#include "../Client/NInterface/NItemInfoDlg/ItemInfoDlg.h"

#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/ListView.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/ninterface/ninfodlg/NInfoDlg.h"

#include "../../Client.h"

#include "../Client/ItemInfo/ItemInfo.h"

#include "..\..\Resource.h"
#include "Msg\MsgType-Quest.h"
#include "../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
//#include "../Client/Interface/MonsterInfoDlg//MonsterInfoDlg.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../Client/NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

extern cltCommonLogic* pclClient;

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNItemInfoDlg::CNItemInfoDlg() : CDialogController()
{
	m_siTypeIndex = 0;
	m_siItemIndex = 0;
	m_timerCount = 0;

	m_siSelectMonsterIndex = -1;

	m_siImageType = -1;
	m_siImageIndex = -1;

	SI32 i;
	for( i=0; i<2; ++i )
		m_pComboBox[i] = NULL;

	for( i=0; i<7; ++i )
		m_pStatic[i] = NULL;

	for( i=0; i<3; ++i)
		m_pEdit[i] = NULL;

	for( i=0; i<3; ++i)
		m_pList[i] = NULL;

	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		m_arrTypeIndex[i] = 0;
	}
	m_pImageStatic = NULL;

	m_pQuestButton = NULL;
	m_pQuestButtonProperty	= NULL;
	m_siCurrentSelectedItemUnique = 0;
	// �˻����
	m_timerCount = 0;
	m_pListBox_AutoMake=NULL;
	m_pEdit_Name=NULL;
	m_pButton_Accept=NULL;
	m_pStatic_Back	= NULL;//��׶���
}

CNItemInfoDlg::~CNItemInfoDlg()
{
	SI32 i;

	SAFE_DELETE(m_pQuestButton);
	SAFE_DELETE(m_pQuestButtonProperty);
	SAFE_DELETE(m_pImageStatic);
	// �˻����
	if(m_pListBox_AutoMake)
		delete m_pListBox_AutoMake;
	if(m_pEdit_Name)
		delete m_pEdit_Name;
	if(m_pButton_Accept)
		delete m_pButton_Accept;
	// �˻����
	m_pListBox_AutoMake=NULL;
	m_pEdit_Name=NULL;
	m_pButton_Accept = NULL;

	SAFE_DELETE(m_pStatic_Back);
	//m_pStatic_Back = NULL; // ��׶���

	for( i=0; i<2; ++i )
	{
		if(m_pComboBox[i])
		{
			delete m_pComboBox[i];
			m_pComboBox[i] = NULL;
		}
	}

	for( i=0; i<7; ++i )
	{
		if(m_pStatic[i])
		{
			delete m_pStatic[i];
			m_pStatic[i] = NULL;
		}
	}

	for( i=0; i<3; ++i)
	{
		if(m_pEdit[i])
		{
			delete m_pEdit[i];
			m_pEdit[i] = NULL;
		}
	}

	for( i=0; i<3; ++i)
	{
		if(m_pList[i])
		{
			delete m_pList[i];
			m_pList[i] = NULL;
		}
	}
}

void CNItemInfoDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
	file.LoadFile(TEXT("NInterface/Data/DLG_NItemInfo.ddf"));
	}
	else{
		file.LoadFile(TEXT("NInterface/Data/DLG_NItemInfo_J.ddf"));
	}
	//file.CreatePopupDialog( this, NITEMINFO_DLG, TEXT("dialog_ITEMINFO"), StaticCallBackDialogNItemInfoDlg );
	file.CreateChildDialog(this,NITEMINFO_DLG,TEXT("dialog_ITEMINFO"),StaticCallBackDialogNItemInfoDlg,pParent);

	m_pStatic[0] = new CStatic( this );
	m_pStatic[1] = new CStatic( this );
	m_pStatic[2] = new CStatic( this );
	m_pStatic[3] = new CStatic( this );
	m_pStatic[4] = new CStatic( this );
	m_pStatic[5] = new CStatic( this );
	m_pStatic[6] = new CStatic( this );

	m_pEdit[0] = new CEdit( this );
	m_pEdit[1] = new CEdit( this );
	m_pEdit[2] = new CEdit( this );

	m_pList[0] = new CList( this );
	m_pList[1] = new CList( this );
	m_pList[2] = new CList( this );

	m_pComboBox[0] = new CComboBox( this );
	m_pComboBox[1] = new CComboBox( this );
	m_pImageStatic = new CImageStatic( this );
	m_pQuestButton = new CButton( this );
	
	NEWCBUTTON( m_pQuestButtonProperty );

	// �˻����
	m_pListBox_AutoMake = new CList(this);
	m_pEdit_Name=new CEdit(this);
	m_pButton_Accept = new CButton(this);
	m_pStatic_Back = new CStatic( this );	// ��׶���
	
	if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
		// �˻����
		file.CreateControl( m_pEdit_Name, NITEMINFO_EDIT_NAME, TEXT("editbox_name") );
		file.CreateControl( m_pListBox_AutoMake, NITEMINFO_LISTBOX_AUTOBOX , TEXT("listbox_automake") );
		file.CreateControl( m_pButton_Accept, NITEMINFO_BUTTON_ACCEPT, TEXT("button_accept") );
		file.CreateControl( m_pStatic_Back, NITEMINFO_DIALOG_CSTATICBACK,   TEXT("statictext_Back")); // ��׶��� 
		//---------------------------------------------------------------------------------------------------------
	}
	

	file.CreateControl( m_pQuestButton, NITEMINFO_DIALOG_BUTTON_QUEST,	TEXT("button_NONAME1"));
	file.CreateControl( m_pQuestButtonProperty, NITEMINFO_DIALOG_BUTTON_QUESTPROPERTY, TEXT("button_NONAME2"));

	file.CreateControl( m_pStatic[0], NITEMINFO_DIALOG_CSTATIC1,   TEXT("statictext_NONAME1")); 
	file.CreateControl( m_pStatic[1], NITEMINFO_DIALOG_CSTATIC2,   TEXT("statictext_NONAME2")); 
	file.CreateControl( m_pStatic[2], NITEMINFO_DIALOG_CSTATIC3,   TEXT("statictext_NONAME3")); 
	file.CreateControl( m_pStatic[3], NITEMINFO_DIALOG_CSTATIC4,   TEXT("statictext_NONAME4")); 
	file.CreateControl( m_pStatic[4], NITEMINFO_DIALOG_CSTATIC5,   TEXT("statictext_NONAME5")); 
	file.CreateControl( m_pStatic[5], NITEMINFO_DIALOG_CSTATIC6,   TEXT("statictext_NONAME6")); 
	file.CreateControl( m_pStatic[6], NITEMINFO_DIALOG_CSTATIC7,   TEXT("statictext_NONAME7")); 

	file.CreateControl( m_pEdit[0], NITEMINFO_DIALOG_EDIT1, TEXT("editbox_NONAME1"));  
	file.CreateControl( m_pEdit[1], NITEMINFO_DIALOG_EDIT2, TEXT("editbox_NONAME2"));  
	file.CreateControl( m_pEdit[2], NITEMINFO_DIALOG_EDIT3, TEXT("editbox_NONAME3"));  

	file.CreateControl( m_pList[0], NITEMINFO_DIALOG_LIST1, TEXT("listbox_NONAME1"));  
	file.CreateControl( m_pImageStatic, NITEMINFO_DIALOG_IMAGESTATIC, TEXT("imagestatic_NONAME1"));
	
	


	TCHAR* pText= GetTxtFromMgr(3288);
	m_pList[0]->SetColumn(0, 104, pText);
	pText= GetTxtFromMgr(3289);
	m_pList[0]->SetColumn(1,  36, pText);
	pText= GetTxtFromMgr(3290);
	m_pList[0]->SetColumn(2,  36, pText);
	m_pList[0]->SetColumn(3,  16, TEXT(""));
	m_pList[0]->SetBorder(TRUE);
	m_pList[0]->SetBKColorUse(TRUE);
	m_pList[0]->Refresh();

	//
	file.CreateControl( m_pList[1], NITEMINFO_DIALOG_LIST2, TEXT("listbox_NONAME2")); 
	
	pText= GetTxtFromMgr(3291);	
	m_pList[1]->SetColumn(0, 89, pText);//���� Į�� �� ���� [2007.06.29 ����]    
	pText= GetTxtFromMgr(3292);
	m_pList[1]->SetColumn(1,  68, pText);//���� Į�� �� ���� [2007.06.29 ����]		

	m_pList[1]->SetColumn(2,  16, TEXT(""));
	m_pList[1]->SetBorder(TRUE);
	m_pList[1]->SetBKColorUse(TRUE);
	m_pList[1]->SetListBoxColor( LISTBOX_COLOR_RED );
	m_pList[1]->SetDrawLine( true );
	m_pList[1]->SetDrawLineColor( LISTBOX_LINECOLOR_RED );
	m_pList[1]->Refresh();

	file.CreateControl( m_pList[2], NITEMINFO_DIALOG_LIST3, TEXT("listbox_NONAME3"));  
	pText= GetTxtFromMgr(3293);
	m_pList[2]->SetColumn(0, 104, pText);
	pText= GetTxtFromMgr(3294);
	m_pList[2]->SetColumn(1,  40, pText);
	m_pList[2]->SetColumn(2,  16, TEXT(""));
	m_pList[2]->SetBorder(TRUE);
	m_pList[2]->SetBKColorUse(TRUE);
	m_pList[2]->SetListBoxColor( LISTBOX_COLOR_GREEN );
	m_pList[2]->SetDrawLine( true );
	m_pList[2]->SetDrawLineColor( LISTBOX_LINECOLOR_GREEN );
	m_pList[2]->Refresh();

	file.CreateControl( m_pComboBox[0], NITEMINFO_DIALOG_TYPE_COMBOBOX, TEXT("combobox_TYPE"));  
	file.CreateControl( m_pComboBox[1], NITEMINFO_DIALOG_NAME_COMBOBOX, TEXT("combobox_NAME"));  

	SetItemTypeInComboBox(true);

	m_pComboBox[0]->SetCursel(0);
	m_pComboBox[1]->SetCursel(0);

	m_pQuestButton->Show(false);

	SetMaterialRefresh();
	SetItemInfo(m_pComboBox[1]->GetText(m_siItemIndex));

	// ���̾�α� �ʱⰪ ����
		// �˻����
	m_pEdit_Name->Refresh();
	m_pButton_Accept->Refresh();
	m_pListBox_AutoMake->SetColumn(0,1,NULL);
	m_pListBox_AutoMake->SetColumn(1,160,NULL);
	m_pListBox_AutoMake->SetShowHeader(FALSE);
	m_pListBox_AutoMake->Show(false);
	SetActionTimer( 400 );
	SetTimerOnOff( true );

	//Hide();
}


void CALLBACK CNItemInfoDlg::StaticCallBackDialogNItemInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNItemInfoDlg *pThis = (CNItemInfoDlg *)pControllerMgr;
	pThis->CallBackDialogNItemInfoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNItemInfoDlg::CallBackDialogNItemInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			m_pListBox_AutoMake->Show(false);
			Hide();
		}
		break;
	//������ ǰ��
	case NITEMINFO_DIALOG_TYPE_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siTypeIndex = m_arrTypeIndex[m_pComboBox[0]->GetSelectedIndex()];

					if(m_pComboBox[0]->GetSelectedIndex() == -1)	break;

					m_pComboBox[1]->Clear();

					if ( m_siTypeIndex != -1 )
					{
						ShowItemListInComboBox(m_pComboBox[1], m_siTypeIndex, false);
					}

					SetMaterialRefresh();
					SetItemInfo(m_pComboBox[1]->GetText(m_siItemIndex));
				}
				break;
			}
		}
		break;
	//������ ����
	case NITEMINFO_DIALOG_NAME_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siItemIndex = m_pComboBox[1]->GetSelectedIndex();

					if( m_siItemIndex == -1)
						break;

					SetMaterialRefresh();
					SetItemInfo(m_pComboBox[1]->GetText(m_siItemIndex));
				}
				break;
			}
		}
		break;
	case NITEMINFO_DIALOG_LIST1:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if( GetParentDialog()->GetDialogControllerID() == NINFO_SUB_DLG )
					{
						break;
					}
					
					TCHAR buffer[256]; 
					SI16 tmpRow = 0, tmpColumn = 0;
					stListBoxItemData tmpListBoxItemData;
					tmpListBoxItemData.Init();

					m_pList[0]->GetSelectedIndex( &tmpRow, &tmpColumn );

					if( tmpColumn == 0 || tmpColumn == 1 || tmpColumn == 2 )
					{
						StringCchCopy( buffer, 256, m_pList[0]->GetText( tmpRow, 0 ));

						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->SetItemInfo(buffer);
						//((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->m_pComboBox[1]->Clear();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->ShowItemInfo(buffer);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_ITEM);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );
					}

				}
				break;
			}
		}
		break;
	case NITEMINFO_DIALOG_LIST2:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if( GetParentDialog()->GetDialogControllerID() == NINFO_SUB_DLG )
					{
						break;
					}
					//	TCHAR buffer[256]; 
					//	ListView_GetItemText(m_hMonsterListCtrl, nia->iItem, 0, buffer, 64 );

					if ( g_SoundOnOffDlg.m_bFullScreenMode )
					{
						//if ( pclclient-w>m_pDialog[ NMONSTERINFO_DLG ] == NULL )
						//{
						//	pclclient->CreateInterface( NMONSTERINFO_DLG );
						//}

						stNMonsterInfoDlgData NMonsterInfoDlgData;
						NMonsterInfoDlgData.pData = m_pList[1]->GetText(m_pList[1]->GetSelectedIndex(), 0);
						NMonsterInfoDlgData.siParam = 1;

						//pclclient->m_pDialog[ NMONSTERINFO_DLG ]->SetData( &NMonsterInfoDlgData );

						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg1->SetData( &NMonsterInfoDlgData);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab( NINFODLG_BUTTON_MONSTER );
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );

						//((CNMonsterInfoDlg*)pclclient->m_pDialog[ NMONSTERINFO_DLG ])->ShowMonsterInfo(buffer);
					}
				}
				break;
			}
		}
		break;
	case NITEMINFO_DIALOG_LIST3:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if( GetParentDialog()->GetDialogControllerID() == NINFO_SUB_DLG )
					{
						break;
					}

					TCHAR buffer[256]; 
					SI16 tmpRow = 0, tmpColumn = 0;
					stListBoxItemData tmpListBoxItemData;
					tmpListBoxItemData.Init();

					m_pList[2]->GetSelectedIndex( &tmpRow, &tmpColumn );

					if( tmpColumn == 0 || tmpColumn == 1 )
					{
						StringCchCopy( buffer, 256, m_pList[2]->GetText( tmpRow, 0 ));

 						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->SetItemInfo(buffer);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->ShowItemInfo(buffer);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_ITEM);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );
					}

				}
				break;
			}
		}
		break;
	case NITEMINFO_DIALOG_BUTTON_QUEST:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI16 questtype, questunique, para1, para2;
					SI16 questclearcond =  QUEST_CLEAR_COND_CLICKITEMINFO;
					if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
						&& para1 == m_siCurrentSelectedItemUnique )
					{
						cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
						cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
						cltClient* pclclient = (cltClient* )pclClient;
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
				break;
			}
		}
		break;
	case NITEMINFO_DIALOG_BUTTON_QUESTPROPERTY:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				SI16 questtype, questunique, para1, para2;
				SI16 questclearcond =  QUEST_CLEAR_COND_CLICKITEMPROPERTY;
				if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
					&& para1 == m_siCurrentSelectedItemUnique )
				{
					cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
					cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
					cltClient* pclclient = (cltClient* )pclClient;
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
			}
		}
		break;
		//ã�� ���
	case NITEMINFO_EDIT_NAME:
		{
			switch(nEvent) 
			{
			case EVENT_EDITBOX_CHANGE:{
									}break;
			case EVENT_EDITBOX_STRING:{
					TCHAR* pItemname = m_pEdit_Name->GetText();
					if ( pItemname == NULL )	return;
					Find(pItemname);
				}break;
			}
		}
	case NITEMINFO_LISTBOX_AUTOBOX:
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
						m_pListBox_AutoMake->Clear();
						m_pListBox_AutoMake->Refresh();
						m_pListBox_AutoMake->Show(false);
					}break;
			}
		}
	case NITEMINFO_BUTTON_ACCEPT:
		{
			switch(nEvent) {
				case EVENT_BUTTON_CLICKED:
					{
						TCHAR* pItemname = m_pEdit_Name->GetText();
						if (pItemname == NULL)	return;
						Find(pItemname);
						
					}break;
			}
		}break;
	}
}
void CNItemInfoDlg::Hide()
{
	SetTimerOnOff(false);
	CControllerMgr::Show( false );

	return;
}

void CNItemInfoDlg::Show()
{
 	SetTimerOnOff(true);
	SetActionTimer(5000);

	//-----------------------------------------------------------
	// ĳ���� â�� ���� ����Ʈ�� �������̶�� 
	// ���� ������ ����Ʈ�� �Ϸ�Ǿ����� �˷��ش�.
	//-----------------------------------------------------------
	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_ITEMINFODLG;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		cltClient* pclclient = (cltClient* )pclClient;
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

	SetMaterialRefresh();

	if(CDialogController::IsShow())
	{	return;	}


	CControllerMgr::Show( true );

	return;
}

void CNItemInfoDlg::Action()
{
	//---------------------------
	// ������ ������ Ŭ���϶�� ����Ʈ �����̸� �ش� ����Ʈ ��Ʈ���� ����� �ٲپ� �ش�


	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_CLICKITEMINFO;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
		&& para1 == m_siCurrentSelectedItemUnique )
	{		
		m_pEdit[2]->SetFontIndex(72);
		m_pQuestButton->Show(true);			
	}	
	else
	{
		m_pEdit[2]->SetFontIndex(0);
		m_pQuestButton->Show(false);
	}
	///////////////////////////////
	// ������ �Ӽ��� Ŭ���϶�� ����Ʈ
	questclearcond =  QUEST_CLEAR_COND_CLICKITEMPROPERTY;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
		&& para1 == m_siCurrentSelectedItemUnique )
	{		
		m_pEdit[1]->SetFontIndex(72);
		m_pQuestButtonProperty->Show(true);			
	}	
	else
	{
		m_pEdit[1]->SetFontIndex(0);
		m_pQuestButtonProperty->Show(false);
	}



	m_siItemIndex = m_pComboBox[1]->GetSelectedIndex();

	if( m_siItemIndex == -1)
		return;

	if(m_bRequestMaterialRefresh)
	{
		m_bRequestMaterialRefresh = false;
		m_pList[0]->Clear();

		//-----------------------------------------
		// �������� ���� ��ǰ ������ �����ش�. 
		//-----------------------------------------
		cltClient *pclclient = (cltClient *)pclClient;

		SI32 id = 1;

		SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName( m_pComboBox[1]->GetText(m_siItemIndex) );

		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

		cltMaterialExplain clMaterialExp[100];
		SI32 siMaterialCount= 0;
		if(pclitemmanager->GetProductionItemExplain(itemunique, &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, clMaterialExp, &siMaterialCount) == TRUE)
		{
			SI32 ImageIndex = -1;
			SI32 siCount;
			TCHAR szHaveCount[50];

			for(SI32 i = 0; i < siMaterialCount; ++i)
			{
				ImageIndex = 0;
				StringCchCopy(szHaveCount, 50, TEXT("")); 
				if( clMaterialExp[i].siUnique == 0 || _tcsstr(clMaterialExp[i].szName, TEXT("===")))
				{
					ImageIndex = -1;
					StringCchCopy(szHaveCount, 50, TEXT(""));
				}
				else
				{
					siCount = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(clMaterialExp[i].siUnique);
					// 0 : BLANK, 1: BLUE, 2:YELLOW, 3:RED
					if(siCount <= 0)									{	ImageIndex = 10;	}
					else if(siCount < _tstoi(clMaterialExp[i].szCount))	{	ImageIndex = 8;	}
					else if(siCount >= _tstoi(clMaterialExp[i].szCount))	{	ImageIndex = 9;	}
					_itot(siCount, szHaveCount, 10);
				}
				//	ListViewInsertItem(hCombo,  ImageIndex , i, 0, clMaterialExp[i].szName, -1);
				//	ListViewInsertItem(hCombo,  -1,          i, 1, clMaterialExp[i].szCount,-1);
				//	ListViewInsertItem(hCombo,  -1,          i, 2, szHaveCount,				-1);

				if(m_pList[0])
				{
					stListBoxItemData Data;

					Data.Set( 0, clMaterialExp[i].szName );
					Data.Set( 1, clMaterialExp[i].szCount );
					Data.Set( 2, szHaveCount );

					if(ImageIndex >= 0)
					{
						Data.siSmallIconFileIndex[ 0 ] = 0;
						Data.siSmallIconFontIndex[ 0 ] = ImageIndex;
					}
					else
					{
						Data.siSmallIconFileIndex[ 0 ] = -1;
						Data.siSmallIconFontIndex[ 0 ] = 0;
					}


					m_pList[0]->AddItem( &Data );
				}
			}
		}
		m_pList[0]->Refresh();
	}

	//KHY - 0609 - ����Ȯ�� ���÷���.
	SetItemInfo(m_pComboBox[1]->GetText(m_siItemIndex));

	if( false == IsShow() )	return;
	if( m_timerCount >= LEAST_COMMON_MULTILPLE + 1 )
		m_timerCount = 1;
	m_timerCount++;
	Update();

}

void CNItemInfoDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{


}

void CNItemInfoDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
	if ( m_pListBox_AutoMake->IsShow() )	return;
	}
	if(m_siImageIndex < 0) 
		return;
 	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	if ( pSpr )
	{
		//������ �̹����� �׸�
		if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
			GP.PutSpr(pSpr, ScreenRX + GetX() + 27, ScreenRY + GetY() + 107, m_siImageIndex );//���� 83
		}
		else{
			GP.PutSpr(pSpr, ScreenRX + GetX() + 27, ScreenRY + GetY() + 83, m_siImageIndex );//���� 83
		}
	}
}

void CNItemInfoDlg::SetItemTypeInComboBox(BOOL bshowall)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i;
	SI32 cnt = 0;
	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		cltItemTypeInfo* pinfo = pclclient->pclItemManager->pclItemTypeInfo[i];
		if(pinfo == NULL)continue;

		// �ŷ� �Ұ� ǰ���̸� �������� �ʴ� �����̶��, 
		if(bshowall == false)
		{
			if(pinfo->IsAtb(ITEMATB_NOTRADE))continue;
		}

		// �޺�����Ʈ���� ����� �Ӽ��� �ִٸ�
		if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;


		stComboBoxItemData ComboBoxItemData;
		ComboBoxItemData.Init();
		ComboBoxItemData.Set( (TCHAR*)pinfo->GetName() );					

		m_pComboBox[0]->AddItem( &ComboBoxItemData );   // Ÿ�԰��� �ִ´�
		m_arrTypeIndex[cnt] = i;
		cnt++;
	}
}

BOOL CNItemInfoDlg::ShowItemListInComboBox(CComboBox* pComboBox, SI32 typeindex, bool btradeinterface)
{
	cltClient *pclclient = (cltClient *)pclClient;

	cltItemTypeInfo* pinfo = pclclient->pclItemManager->pclItemTypeInfo[typeindex]; 

	if ( pinfo == NULL )	return false;

	SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(pinfo->GetName());
	if(itemtypeunique >=0)
	{
		int i;
		// �޺� �ڽ��� �������� ������ �����Ѵ�. 
		for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{	
			cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[i];
			if(pinfo == NULL)continue;
			if(pinfo->siType != itemtypeunique)continue;

			// �־��� Ÿ�԰� ���� �����۸� �����Ѵ�. 
			// ����ڿ��� �������� �������� ��쿡��. 
			bool bshow = false;
			if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
			{
				if(btradeinterface == true)
				{
				//	if(pinfo->IsItemInfoAtb(ITEMINFOATB_TRADELIST)  == true)
					{
						bshow = true;
					}
				}
				else
				{
					bshow = true;
				}

				if(bshow)
				{
					stComboBoxItemData ComboBoxItemData;
					ComboBoxItemData.Init();
					ComboBoxItemData.Set( (TCHAR*)pinfo->GetName() );					

					pComboBox->AddItem( &ComboBoxItemData );
				}
			}
		}
	}

	return TRUE;
}
void CNItemInfoDlg::ShowItemInfo(TCHAR* pStrname)
{
	if(pStrname ==NULL) return;

	// ��ȭ���ڰ� �Ⱥ��δٸ� ���̰��Ѵ�.
	if ( CDialogController::IsShow() == false )	
	{
		CControllerMgr::Show( true );
	}

	SI32 type;
	TCHAR* ptypename = NULL;
	SI32 itemunique	= pclClient->pclItemManager->FindUniqueFromName(pStrname) ;
	if(itemunique > 0)
	{
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);

		if(ref > 0)
		{
			type = pclClient->pclItemManager->pclItemInfo[ref]->siType;

			ptypename = (TCHAR *)(pclClient->pclItemManager->pclItemTypeInfo[type]->GetName());
		}
	}

	if(ptypename  ==NULL) return;


	m_siTypeIndex = m_arrTypeIndex[m_pComboBox[0]->FindIndexFromText(ptypename)];
	m_pComboBox[0]->SetCursel(m_pComboBox[0]->FindIndexFromText(ptypename));

	SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(ptypename);
	if(itemtypeunique >=0)
	{
		// itemtypeunique�� �ش��ϴ� ������ ����Ʈ�� �����ش�. 
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		m_pComboBox[1]->Clear();
		ShowItemListInComboBox(m_pComboBox[1], m_siTypeIndex, false);
	}
	m_siItemIndex = m_pComboBox[1]->FindIndexFromText(pStrname);
	m_pComboBox[1]->SetCursel(m_siItemIndex);
	SetItemInfo(pStrname);
}

void CNItemInfoDlg::SetItemInfo(TCHAR* pStrname)
{
	if(pStrname == NULL)			return;

	if(m_bRequestMaterialRefresh)
	{
		m_pList[0]->Clear();
		m_pList[1]->Clear();
		m_pList[2]->Clear();
	}

	SI32 id = 1;

	SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(pStrname);

	if(itemunique)
	{
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		TCHAR explaintext[ 1024 ]= TEXT("");
		TCHAR temp[256]= TEXT("");

		cltClient *pclclient = (cltClient *)pclClient;

		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( itemunique, &m_siImageType, &m_siImageIndex ) )
		{}
		else return;

		m_siCurrentSelectedItemUnique = itemunique;

		//----------------------------------------
		// �������� ��Ÿ ������ ǥ���Ѵ� 
		//----------------------------------------
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
		if(ref > 0)
		{
			SI32 explainunique = pclClient->pclItemManager->pclItemInfo[ref]->siExplainUnique;
			if(explainunique)
			{
				TCHAR* ptext = GetTxtFromMgr(explainunique);
				if(ptext)
				{
					StringCchCopy(explaintext, 1024, ptext);
					StringCchCat(explaintext, 1024, TEXT("\r\n"));
				}
			}
		}

		if(pclitemmanager->GetItemAtb(itemunique) & ITEMATB_OBJECT)
		{
			if(pclitemmanager->CanMakableItem(itemunique) == true)
			{
				SI32 addlevel = 0;  // ������� �ϴ� ����� ���� �ڽ��� ��ų ����	
				SI32 qualityrate = pclitemmanager->GetQualityRate(itemunique, id, &addlevel);
				TCHAR* pText = GetTxtFromMgr(1228);

				StringCchPrintf(temp, 256, pText, qualityrate, '%', '%');
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(1229);
				StringCchPrintf(temp, 256, pText);
			}
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1230);		// "������ ������ �ʴ� ��ǰ�Դϴ�."
			StringCchPrintf(temp, 256, pText);
		}
		StringCchCat(explaintext, 1024, temp);

		m_pEdit[0]->SetText(	explaintext);

		//-----------------------------------------
		// �������� �Ϲ� ������ ���´�. 
		//-----------------------------------------
		//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�	 - ����ü�� ��� ������ ����ó�� ���ش�.
		SI32 itemtypeC = pclClient->pclItemManager->pclItemInfo[ref]->siType;

		if(itemtypeC ==ITEMTYPE_CRYSTAL)
		{
			if(pclitemmanager->GetExplainCrystalClient(itemunique, explaintext) == true)
			{
				SI32 silen = _tcslen(explaintext);
				if( silen > 2 && explaintext[silen-1] =='\n') {	explaintext[silen-1] = 0;	}
				if( silen > 2 && explaintext[silen-2] =='\r') {	explaintext[silen-2] = 0;	}

				m_pEdit[1]->SetText(	explaintext);
			}
		}
		else
		{
			if(pclitemmanager->GetExplain(itemunique, explaintext) == TRUE)
			{
				SI32 silen = _tcslen(explaintext);
				if( silen > 2 && explaintext[silen-1] =='\n') {	explaintext[silen-1] = 0;	}
				if( silen > 2 && explaintext[silen-2] =='\r') {	explaintext[silen-2] = 0;	}

				m_pEdit[1]->SetText(	explaintext);
			}
		}
		//-----------------------------------------
		// �������� ���� ������ �����ش�. 
		//-----------------------------------------
		if(pclitemmanager->CanMakableItem(itemunique) == true)
		{
			// ���� ������ ��ǰ�� ���� ������ ǥ���Ѵ�. - by LEEKH 2007.06.28
			if(pclitemmanager->GetProductionExplain(itemunique, &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, explaintext, 1024) == TRUE)
			{
				SI32 silen = _tcslen(explaintext);
				if( silen > 2 && explaintext[silen-1] =='\n') {	explaintext[silen-1] = 0;	}
				if( silen > 2 && explaintext[silen-2] =='\r') {	explaintext[silen-2] = 0;	}

				m_pEdit[2]->SetText(	explaintext);
			}
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(1229);	// "[���� �Ұ�]"	- by LEEKH 2007.06.28 ������ �Ұ��� �ϴٰ� ǥ��
			m_pEdit[2]->SetText(pText);
		}

		//-----------------------------------------
		// �������� ���� ��ǰ ������ �����ش�. 
		//-----------------------------------------
		cltMaterialExplain clMaterialExp[100];
		SI32 siMaterialCount= 0;
		if(pclitemmanager->GetProductionItemExplain(itemunique, &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, clMaterialExp, &siMaterialCount) == TRUE)
		{
			SI32 ImageIndex = -1;
			SI32 siCount;
			TCHAR szHaveCount[50];

			for(SI32 i = 0; i < siMaterialCount; ++i)
			{
				ImageIndex = 0;
				StringCchCopy(szHaveCount, 50, TEXT("")); 
				if( clMaterialExp[i].siUnique == 0 || _tcsstr(clMaterialExp[i].szName, TEXT("===")))
				{
					ImageIndex = -1;
					StringCchCopy(szHaveCount, 50, TEXT(""));
				}
				else
				{
					siCount = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(clMaterialExp[i].siUnique);
					// 0 : BLANK, 1: BLUE, 2:YELLOW, 3:RED
					if(siCount <= 0)									{	ImageIndex = 10;	}
					else if(siCount < _tstoi(clMaterialExp[i].szCount))	{	ImageIndex = 8;	}
					else if(siCount >= _tstoi(clMaterialExp[i].szCount))	{	ImageIndex = 9;	}
					_itot(siCount, szHaveCount, 10);
				}
				//	ListViewInsertItem(hCombo,  ImageIndex , i, 0, clMaterialExp[i].szName, -1);
				//	ListViewInsertItem(hCombo,  -1,          i, 1, clMaterialExp[i].szCount,-1);
				//	ListViewInsertItem(hCombo,  -1,          i, 2, szHaveCount,				-1);

				if(m_pList[0] && m_bRequestMaterialRefresh)
				{
					stListBoxItemData Data;

					Data.Set( 0, clMaterialExp[i].szName );
					Data.Set( 1, clMaterialExp[i].szCount );
					Data.Set( 2, szHaveCount );

					if(ImageIndex >= 0)
					{
						Data.siSmallIconFileIndex[ 0 ] = 0;
						Data.siSmallIconFontIndex[ 0 ] = ImageIndex;
					}
					else
					{
						Data.siSmallIconFileIndex[ 0 ] = -1;
						Data.siSmallIconFontIndex[ 0 ] = 0;
					}


					m_pList[0]->AddItem( &Data );
				}
			}
		}
		if(m_bRequestMaterialRefresh)		m_pList[0]->Refresh();

		//-----------------------------------------
		// �������� �ִ� ���� ������ �����ش�. 
		//-----------------------------------------
		cltDropItem dropitem[100];
		SI32	siMonterCount = 100;

		if( m_bRequestMaterialRefresh == TRUE &&
			pclClient->pclKindInfoSet->GetItemMonsterExplain(itemunique, dropitem, &siMonterCount) == true)
		{
			SI32 siSortIndex = 0;
			for(SI32 i = 0; i < siMonterCount; ++i)
			{
				//	siSortIndex  = ListViewInsertItem(hCombo,  -1 , 0,	0, dropitem[i].szName, -1);
				//	ListViewInsertItem(hCombo,  -1,  siSortIndex, 1, dropitem[i].szItemNum,	-1);

				if(m_pList[1])
				{
					stListBoxItemData Data;

					Data.Set( 0, dropitem[i].szName );
					Data.Set( 1, dropitem[i].szItemNum );

					m_pList[1]->AddItem( &Data );
				}
			}
		}

		m_pList[1]->Refresh();
		//-----------------------------------------
		// �ش� ���������� ������ ����� �ִ� ��ǰ ������ �����ش�.
		//-----------------------------------------
		cltMakeItem makeitem[100];
		SI32	siMakeItemCount = 100;
		if( m_bRequestMaterialRefresh == TRUE &&
			pclitemmanager->GetProductionMakeItemExplain(itemunique, makeitem, &siMakeItemCount) == TRUE)
		{

			SI32 siSortIndex = 0;
			for(SI32 i = 0; i < siMakeItemCount; ++i)
			{
				//	siSortIndex  = ListViewInsertItem(hCombo,  -1 , 0,	0, makeitem[i].szName, -1);
				//	ListViewInsertItem(hCombo,  -1,  siSortIndex, 1, makeitem[i].szItemNum,	-1);

				if(m_pList[2])
				{
					stListBoxItemData Data;

					Data.Set( 0, makeitem[i].szName );
					Data.Set( 1, makeitem[i].szItemNum );

					m_pList[2]->AddItem( &Data );
				}
			}
		}

		m_pList[2]->Refresh();

		//-----------------------------------------------------------
		// �������� �˻��ϴ� ����Ʈ�� �������̶�� 
		// ���� ������ ����Ʈ�� �Ϸ�Ǿ����� �˷��ش�.
		//-----------------------------------------------------------
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_ITEMINFO;
		if(pclClient->pclCM->CR[id]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			if(para1 == itemunique)
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				cltClient* pclclient = (cltClient* )pclClient;
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
	}

} 


SI32 CNItemInfoDlg::GetItemInfoPosX()
{	
	return GetX() + m_pEdit[2]->GetX() + m_pEdit[2]->GetWidth()/2   ;
}
SI32 CNItemInfoDlg::GetItemInfoPosY()
{
	return  GetY() +  m_pEdit[2]->GetY()  ;
}
// ������ �˻�
void CNItemInfoDlg::ItemFind(TCHAR* pStrname)
{
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	if(pStrname == NULL)			return;
	TCHAR* Itemname = TEXT("");
	// ������ �̸��� �ٰŷ� �������� ����ũ�� ���Ѵ�.
	SI32 ItemUnique = pclitemmanager->FindUniqueFromName(pStrname);
	// ref�� ���Ѵ�.
	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( ItemUnique );
	if ( ref == NULL ) return;
	// ref�� ���� ItemType�� ���Ѵ�
	SI32 ItemType = pclitemmanager->pclItemInfo[ref]->siType;
	// ItemType�� ���� ������ Ÿ�ӳ����� ���ϰ� �ش� Ÿ�Գ����� �޺��ڽ����� ã�´�.
	SI16 FindIndexType = m_pComboBox[0]->FindIndexFromText( pclitemmanager->pclItemTypeInfo[ ItemType ]->szName );
	m_pComboBox[0]->SetCursel(FindIndexType);
	//�������̸����� �ε����� ã�Ƽ�
	SI16 FindIndex = m_pComboBox[1]->FindIndexFromText(pStrname);
	//�ش� �ε����� ž���� 
	m_pComboBox[1]->SetCursel(FindIndex);
}
void CNItemInfoDlg :: Find(TCHAR* pItemname)
{
	if ( strlen(pItemname) == 0) return;// ���ڿ��� ���� ����
	Show();
	ItemFind(pItemname);
	m_pEdit_Name->Clear();
}
void CNItemInfoDlg ::Update(){

	cltClient *pclclient = (cltClient *)pclClient;
	NTCHARString64		kInputText(m_pEdit_Name->GetText());

	if( m_pListBox_AutoMake->IsShow() == true &&
		m_pEdit_Name->IsFocus() == false)
	{
		if( ( m_pListBox_AutoMake->IsFocus() == false && m_pListBox_AutoMake->GetScroll()->IsFocus() == false) )
		{
			m_pListBox_AutoMake->Clear();
			m_pListBox_AutoMake->Refresh();
			m_pListBox_AutoMake->Show(false);
			//pclClient->pclLog->LogPrint("Lose Focus\n");
			return;
		}
	}
	if (kInputText.IsEmpty())
	{
		m_pListBox_AutoMake->Clear();
		m_pListBox_AutoMake->Refresh();
		m_pListBox_AutoMake->Show(false);
		m_LastInput = kInputText;
		return;
	}

	// ���� ������ ó������ �ʴ´�.
	if (m_LastInput == kInputText)		return;

	m_pListBox_AutoMake->Clear();

	for(SI32 i = 0; i <MAX_ITEMINFO_NUMBER; i++) {
        cltItemInfo* pinfo = pclclient->pclItemManager->pclItemInfo[i];
		if(pinfo == NULL)continue;
		if (pinfo->IsItemInfoAtb( ITEMINFOATB_SHOWLIST ) )
		{
			const TCHAR* findname = _tcsstr(pinfo->GetName(), kInputText);
			if(findname)		AddListItem((TCHAR*)pinfo->GetName());
		}		
	}
	m_pListBox_AutoMake->Refresh();
	m_pListBox_AutoMake->Show(true);

	if( m_pListBox_AutoMake->GetListNum() <= 1 )
	{
		NTCHARString64 kText(m_pListBox_AutoMake->GetText(0, 1));
		if( kText == kInputText)
		{
			m_pListBox_AutoMake->Clear();
			m_pListBox_AutoMake->Show(false);
		}
	}

	m_LastInput = kInputText;
}
void CNItemInfoDlg :: AddListItem(TCHAR* findname){
	if(findname == NULL)	return;
	SI16 cnt = 0;
	stListBoxItemData Data;

	Data.Init();
	Data.Set( 1, findname );
	m_pListBox_AutoMake->AddItem(&Data);
}
