#include "TestDlg.h"
#include "TestDlg1.h"
#include "TestDlg2.h"
#include "TestDlg3.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CTestDlg::CTestDlg() : CDialogController()
{
	m_pButton1 = NULL;
	m_pButton2 = NULL;
	m_pButton3 = NULL;

	m_pCheckBox = NULL;
	m_pRadioButton1 = NULL;
	m_pRadioButton2 = NULL;

	m_pDlg1 = NULL;
	m_pDlg2 = NULL;
	//m_pDlg3 = NULL;
	/*
	m_pEdit = NULL;
	
	m_pListView = NULL;

	m_pInsertButton = NULL;
	m_pDeleteButton = NULL;
	
	m_pList = NULL;
	*/
}

CTestDlg::~CTestDlg()
{
	if ( m_pButton1 )
	{
		delete m_pButton1;
		m_pButton1 = NULL;
	}

	if ( m_pButton2 )
	{
		delete m_pButton2;
		m_pButton2 = NULL;
	}

	if ( m_pButton3 )
	{
		delete m_pButton3;
		m_pButton3 = NULL;
	}


	if ( m_pDlg1 )
	{
		delete m_pDlg1;
		m_pDlg1 = NULL;
	}

	if ( m_pDlg2 )
	{
		delete m_pDlg2;
		m_pDlg2 = NULL;
	}

	//if ( m_pDlg3 )
//	{
//		delete m_pDlg3;
//		m_pDlg3 = NULL;
//	}


	if ( m_pCheckBox )
	{
		delete m_pCheckBox;
		m_pCheckBox = NULL;
	}

	if ( m_pRadioButton1 )
	{
		delete m_pRadioButton1;
		m_pRadioButton1 = NULL;
	}

	if ( m_pRadioButton2 )
	{
		delete m_pRadioButton2;
		m_pRadioButton2 = NULL;
	}
	/*
	if ( m_pEdit )
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}

	if( m_pListView )
	{
		delete m_pListView;
		m_pListView = NULL;
	}

	if ( m_pInsertButton )
	{
		delete m_pInsertButton;
		m_pInsertButton = NULL;
	}

	if ( m_pDeleteButton )
	{
		delete m_pDeleteButton;
		m_pDeleteButtn = NULL;
	}

	if ( m_pList )
	{
		delete m_pList;
		m_pList = NULL;
	}
	*/
}


void CTestDlg::Create()
{	
	if ( !IsCreate() )
	{
		CDialogController::Create( TEST_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogTest, TEXT("TestDlg"), TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 9, 300, 100, 500, 500 );

		m_pButton1 = new CButton( this );
		m_pButton1->Create( TEST_DIALOG_BUTTON1, TEXT("1번"), NULL, 0, false, 0, 30, 30, 120, 50, 120, 50 );

		m_pButton2 = new CButton( this );
		m_pButton2->Create( TEST_DIALOG_BUTTON2, TEXT("2번"), NULL, 0, false, 0, 200, 30, 120, 50, 120, 50 );

		//m_pButton3 = new CButton( this );
		//m_pButton3->Create( TEST_DIALOG_BUTTON3, TEXT("3번"), NULL, 0, false, 0, 350, 30, 120, 50, 120, 50 );



		/*
		m_pCheckBox = new CCheckBox( this );
		m_pCheckBox->Create( TEST_DIALOG_CHECKBOX, TEXT("체크박스"), NULL, 0, false, 0, 30, 100, 120,50, 120, 50 );
		m_pCheckBox->SetCheck( true );

		m_pRadioButton1 = new CRadioButton( this );
		m_pRadioButton1->Create( TEST_DIALOG_RADIOBUTTON1, TEXT("라디오버튼1"), NULL, 0, false, 0, 30, 30, 120, 50, 120, 50, 1 );

		m_pRadioButton2 = new CRadioButton( this );
		m_pRadioButton2->Create( TEST_DIALOG_RADIOBUTTON2, TEXT("라디오버튼2"), NULL, 0, false, 0, 200, 30, 120, 50, 120, 50, 1 );


		m_pRadioButton1->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON );
		*/


		m_pDlg1 = new CTestDlg1();
		m_pDlg1->Create( this );

		m_pDlg2 = new CTestDlg2();
		m_pDlg2->Create( this );


       // m_pDlg3 = new CTestDlg3();
		//m_pDlg3->Create( this );



		/*
		m_pEdit = new CEdit( this );
		m_pEdit->Create( TEST_EDIT, NULL, NULL, 0, false, 0, 5, 150, 300, 180, 496, 180, true, false );


		m_pListView = new CListView( this );
		m_pListView->Create( TEST_LISTVIEW, TEXT("ListView.Spr"), 0, false, 0, 2, 299, 496, 200, 496, 200, 0, 0, 0, 0, 48, 48, true, true, true, 50, 50 );

		m_pInsertButton = new CButton( this );
		m_pInsertButton->Create( TEST_DIALOG_INSERTBUTTON, NULL, TEXT("InsertButton.Spr"), 0, false, 0, 0, 0, 100, 50, 100, 50 );

		m_pDeleteButton = new CButton( this );
		m_pDeleteButton->Create( TEST_DIALOG_DELETEBUTTON, NULL, TEXT("DeleteButton.Spr"), 0, false, 0, 100, 0, 100, 50, 100, 50 );

		m_pList = new CList( this );
		m_pList->Create( TEST_DIALOG_LIST, NULL, 0, false, 0, 100, 100, 300, 200, 300, 200, true, false, true, true );
		m_pList->SetFontSize( 12 );
		m_pList->SetFontWeight( 0 );

		m_pList->SetColumn( 0, 70, TEXT("이상민") );
		m_pList->SetColumn( 1, 70, TEXT("이제완") );
		m_pList->SetColumn( 2, 70, TEXT("ㅎㅎㅎ") );


		stListBoxItemData Data;

		Data.Set( 0, TEXT("ㅎㅎ11") );
		Data.Set( 1, TEXT("ㅎㅎ12") );
		Data.Set( 2, TEXT("ㅎㅎ13") );

		Data.siSmallIconFileIndex = 0;
		Data.siSmallIconFontIndex = 0;

		m_pList->AddItem( &Data );

		Data.siSmallIconFontIndex = 1;
		m_pList->AddItem( &Data );

		Data.siSmallIconFontIndex = 2;
		m_pList->AddItem( &Data );

		Data.siSmallIconFontIndex = 3;
		m_pList->AddItem( &Data );

		Data.siSmallIconFontIndex = 4;
		m_pList->AddItem( &Data );

		Data.siSmallIconFontIndex = 5;
		m_pList->AddItem( &Data );

		Data.siSmallIconFontIndex = 6;
		m_pList->AddItem( &Data );

		m_pList->Refresh();
		*/
	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CTestDlg::StaticCallBackDialogTest( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CTestDlg *pThis = (CTestDlg *)pControllerMgr;
	pThis->CallBackDialogTest( nEvent, nControlID, pControl );
}

void CALLBACK CTestDlg::CallBackDialogTest( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case TEST_DIALOG_BUTTON1:
		{
			m_pDlg2->Hide();
			//m_pDlg3->Hide();
			m_pDlg1->Show();
		}
		break;
	case TEST_DIALOG_BUTTON2:
		{
			m_pDlg1->Hide();
			//m_pDlg3->Hide();
			m_pDlg2->Show();
		}
		break;
/*
	case TEST_DIALOG_BUTTON3:
		{
			m_pDlg1->Hide();
			m_pDlg2->Hide();
			m_pDlg3->Show();
		}
		break;

		*/
	case TEST_DIALOG_CHECKBOX:
		{
			if(  m_pCheckBox->IsCheck() )
			{

			}
			else
			{

			}
		}
		break;
	case TEST_DIALOG_RADIOBUTTON1:
		{
			if(  m_pRadioButton1->IsCheck() )
			{

			}
			else
			{

			}
		}
		break;
	case TEST_DIALOG_RADIOBUTTON2:
		{
			if(  m_pRadioButton2->IsCheck() )
			{

			}
			else
			{

			}
		}
		break;
	}
	/*
	static int PreItemDragIndex = -1;
	
	switch ( nControlID )
	{
	case TEST_DIALOG_INSERTBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					stListViewItemData ItemData;
					ItemData.ImageFileIndex = GIMG_ITEMIMG0;
					ItemData.ImageFontIndex = rand()%100;
					_tcscpy( ItemData.ItemName, TEXT("메롱아탬") );
					ItemData.ItemNum = rand() % 3000;
					ItemData.ItemListViewPos = rand() % 20;
					_tcscpy( ItemData.ToolTipMsg, TEXT("에롱") );
					m_pListView->InsertItem( &ItemData );
				}
				break;
			default:
				break;
			}
		}
		break;
	case TEST_DIALOG_DELETEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_pListView->GetSelectedItemIndex() != -1 )
					{
						m_pListView->DeleteItem( m_pListView->GetSelectedItemIndex() );
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case TEST_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{	
					SI16 SelectedIndex = m_pListView->GetSelectedItemIndex();

					if ( m_siSelectedIndex >= 0 )
					{
						SI16 LastSelectedIndex = m_pListView->GetLastSelectedItemIndex();

						if ( pclclient->m_pInterfaceMgr->IsItemDragMoveStatus() )
						{
							if ( m_siSelectedIndex != LastSelectedIndex )
							{
								m_pListView->ChangeItemPos( m_siSelectedIndex, LastSelectedIndex );
								m_pListView->ClearSelectedItem();
								m_siSelectedIndex = -1;
							}

							pclclient->m_pInterfaceMgr->ItemEndDrag();
						}
						else
						{
							PreItemDragIndex = -1;
						}
					}
					else
					{
                        m_siSelectedIndex = SelectedIndex;
						PreItemDragIndex = -1;
					}
				}
				break;
			case EVENT_LISTVIEW_LBUTTONUP:
				{
					SI16 SelectedIndex = m_pListView->GetSelectedItemIndex();

					if ( SelectedIndex >= 0 && SelectedIndex == m_siSelectedIndex )
					{
						m_siSelectedIndex = SelectedIndex;
						
						if ( PreItemDragIndex != SelectedIndex  )
						{
							PreItemDragIndex = SelectedIndex;
							pclclient->m_pInterfaceMgr->ItemBeginDrag( GIMG_ITEMIMG0, 0 );
						}
						else
						{
							PreItemDragIndex = -1;
						}
					}
					else
					{
						m_siSelectedIndex = -1;
						PreItemDragIndex = -1;
					}
				}
				break;
			case EVENT_LISTVIEW_RBUTTONDOWN:
				{	
				}
				break;
			case EVENT_LISTVIEW_RBUTTONUP:
				{
				}
				break;
			default:
				break;
			}
		}
		break;
	}
	*/

	return;
}

void CTestDlg::Hide()
{
	CDialogController::Show( false );

	return;
}

void CTestDlg::Show()
{
	CDialogController::Show( true );

	return;
}