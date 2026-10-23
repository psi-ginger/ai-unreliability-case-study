#include "TestDlg1.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CTestDlg1::CTestDlg1() : CDialogController()
{
	m_pComboBox1 = NULL;
	m_pComboBox2 = NULL;
	m_pComboBox3 = NULL;
	m_pEdit = NULL;
}

CTestDlg1::~CTestDlg1()
{
	if ( m_pEdit )
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}

	if ( m_pComboBox1 )
	{
		delete m_pComboBox1;
		m_pComboBox1 = NULL;
	}

	if ( m_pComboBox2 )
	{
		delete m_pComboBox2;
		m_pComboBox2 = NULL;
	}

	if ( m_pComboBox3 )
	{
		delete m_pComboBox3;
		m_pComboBox3 = NULL;
	}
}


void CTestDlg1::Create( CControllerMgr *pParent )
{	
	CDialogController::Create( TEST_DIALOG1, CONTROLLER_KIND_CHILDDIALOG, pParent, StaticCallBackDialogTest1, NULL, NULL, 0, false,	0, 30, 80, 400, 250, false, false, false );

	m_pEdit = new CEdit( this );
	m_pEdit->Create( TEST_EDIT1, NULL, NULL, 0, false, 0, 170, 20, 250, 200, 250, 200, true, false, 14, 512/*, EDIT_INPUT_MODE_CHANGETEXTDRAW */);
	m_pEdit->SetAutoScroll( true );
	m_pEdit->SetText( TEXT("\r\n") );

	m_pComboBox3 = new CComboBox( this );
	m_pComboBox3->Create( TEST_COMBOBOX2, TEXT("NInterface/Common/Common_4X17_00_103.Spr"), 0, true, 3, 20, 80, 50, 17, 50, 187, 170 );

	m_pComboBox2 = new CComboBox( this );
	m_pComboBox2->Create( TEST_COMBOBOX2, TEXT("NInterface/Common/Common_4X17_00_103.Spr"), 0, true, 3, 20, 50, 50, 17, 50, 187, 170 );

	m_pComboBox1 = new CComboBox( this );
	m_pComboBox1->Create( TEST_COMBOBOX1, TEXT("NInterface/Common/Common_4X17_00_103.Spr"), 0, true, 3, 20, 20, 148, 17, 148, 187, 170 );

	stComboBoxItemData	ItemData;
	StringCchCopy( ItemData.strText, 256, TEXT("À¸¶ä9") );
	ItemData.siParam = 0;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("À¸¶ä10") );
	ItemData.siParam = 1;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("Á¤9") );
	ItemData.siParam = 2;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("Á¤10") );
	ItemData.siParam = 3;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("¹Î9") );
	ItemData.siParam = 4;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("¹Î10") );
	ItemData.siParam = 5;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("°¡À»9") );
	ItemData.siParam = 6;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("°¡À»10") );
	ItemData.siParam = 7;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("°³¼º9") );
	ItemData.siParam = 8;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("°³¼º10") );
	ItemData.siParam = 9;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("³×¸ðµüÁö12") );
	ItemData.siParam = 10;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("µµ´ã9") );
	ItemData.siParam = 11;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("µÕ±ÙµüÁö12") );
	ItemData.siParam = 12;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("¸ð¸®½º9") );
	ItemData.siParam = 13;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("¸ð¾Æ12") );
	ItemData.siParam = 14;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("¹Ì·Î12") );
	ItemData.siParam = 15;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("¹Î12b") );
	ItemData.siParam = 16;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("»ç°¢12") );
	ItemData.siParam = 17;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("¼Ò¼³12") );
	ItemData.siParam = 18;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("Á¤12b") );
	ItemData.siParam = 19;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("Å°´Ù¸®9") );
	ItemData.siParam = 20;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("Å»ÇÇ12") );
	ItemData.siParam = 21;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("Åä¸®9") );
	ItemData.siParam = 22;
	m_pComboBox1->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("Åä¸®10") );
	ItemData.siParam = 23;
	m_pComboBox1->AddItem( &ItemData );



	StringCchCopy( ItemData.strText, 256, TEXT("0") );
	ItemData.siParam = 0;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("6") );
	ItemData.siParam = 6;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("7") );
	ItemData.siParam = 7;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("8") );
	ItemData.siParam = 8;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("9") );
	ItemData.siParam = 9;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("10") );
	ItemData.siParam = 10;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("11") );
	ItemData.siParam = 11;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("12") );
	ItemData.siParam = 12;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("13") );
	ItemData.siParam = 13;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("14") );
	ItemData.siParam = 14;
	m_pComboBox2->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("15") );
	ItemData.siParam = 15;
	m_pComboBox2->AddItem( &ItemData );



	StringCchCopy( ItemData.strText, 256, TEXT("-1") );
	ItemData.siParam = -1;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("0") );
	ItemData.siParam = 0;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("6") );
	ItemData.siParam = 6;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("7") );
	ItemData.siParam = 7;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("8") );
	ItemData.siParam = 8;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("9") );
	ItemData.siParam = 9;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("10") );
	ItemData.siParam = 10;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("11") );
	ItemData.siParam = 11;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("12") );
	ItemData.siParam = 12;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("13") );
	ItemData.siParam = 13;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("14") );
	ItemData.siParam = 14;
	m_pComboBox3->AddItem( &ItemData );

	StringCchCopy( ItemData.strText, 256, TEXT("15") );
	ItemData.siParam = 15;
	m_pComboBox3->AddItem( &ItemData );

	m_pEdit->SetFontHeight( 0 );

	Hide();

	return;
}

void CALLBACK CTestDlg1::StaticCallBackDialogTest1( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CTestDlg1 *pThis = (CTestDlg1 *)pControllerMgr;
	pThis->CallBackDialogTest1( nEvent, nControlID, pControl );
}

void CALLBACK CTestDlg1::CallBackDialogTest1( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	if( nControlID == TEST_COMBOBOX1 )
	{
		stComboBoxItemData *pItemData = m_pComboBox1->GetItemData(m_pComboBox1->GetSelectedIndex());
		if ( pItemData )
		{
			TCHAR FileName[128] = TEXT("");
			StringCchPrintf( FileName, 128, TEXT("Font/%s.ttf"), pItemData->strText );
			pclclient->ChangeTextFont( FileName, pItemData->strText );
			m_pEdit->Refresh();
		}
	}
	else if ( nControlID == TEST_COMBOBOX2 )
	{
		stComboBoxItemData *pItemData = m_pComboBox2->GetItemData(m_pComboBox2->GetSelectedIndex());
		if ( pItemData )
		{
			m_pEdit->SetFontHeight( pItemData->siParam );
			m_pEdit->Refresh();
		}
	}
	else if ( nControlID == TEST_COMBOBOX3 )
	{
		stComboBoxItemData *pItemData = m_pComboBox3->GetItemData(m_pComboBox3->GetSelectedIndex());
		if ( pItemData )
		{
			m_pEdit->SetFontWidth( pItemData->siParam );
			m_pEdit->Refresh();
		}
	}

	return;
}

void CTestDlg1::Hide()
{
	CControllerMgr::Show( false );

	return;
}

void CTestDlg1::Show()
{
	CControllerMgr::Show( true );

	return;
}