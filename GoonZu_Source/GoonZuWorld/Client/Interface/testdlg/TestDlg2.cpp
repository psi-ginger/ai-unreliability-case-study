#include "TestDlg2.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ChatEdit.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

CTestDlg2::CTestDlg2() : CDialogController()
{
	m_pChatEdit = NULL;
	m_pButton = NULL;
}

CTestDlg2::~CTestDlg2()
{
	if ( m_pChatEdit )
	{
		delete m_pChatEdit;
		m_pChatEdit = NULL;
	}

	if ( m_pButton )
	{
		delete m_pButton;
		m_pButton = NULL;
	}
}


void CTestDlg2::Create( CControllerMgr *pParent )
{	
	CDialogController::Create( TEST_DIALOG2, CONTROLLER_KIND_CHILDDIALOG, pParent, StaticCallBackDialogTest2, NULL, NULL, 0, false, 0, 30, 80, 400, 250, false, false, false );

	m_pChatEdit = new CChatEdit( this );
	m_pChatEdit->Create( TEST_CHATEDIT, NULL, NULL, 0, false, 0, 170, 20, 100, 250, 100, 250, 14, true ); 
	m_pChatEdit->SetBorder( true );
	m_pChatEdit->SetBKColorUse( true );

	stChatEditData ChatEditData;
	ChatEditData.siChatDataColorIndex = 32;
	ChatEditData.siNameDataColorIndex = 1;
	_tcscpy( ChatEditData.strNameData, TEXT("戚雌肯") );

	for( SI16 i = 0; i < 100; ++i )
	{
		StringCchPrintf( ChatEditData.strChatData, CHATEDIT_CHATDATA_LENGTH, TEXT("%d: asdfぁいぇぉけげさしじずぜそぞたちっづでに"), i );
		m_pChatEdit->Insert(&ChatEditData);
	}
	


	Hide();

	return;
}

void CALLBACK CTestDlg2::StaticCallBackDialogTest2( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CTestDlg2 *pThis = (CTestDlg2 *)pControllerMgr;
	pThis->CallBackDialogTest2( nEvent, nControlID, pControl );
}

void CALLBACK CTestDlg2::CallBackDialogTest2( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	return;
}

void CTestDlg2::Hide()
{
	CControllerMgr::Show( false );

	return;
}

void CTestDlg2::Show()
{
	CControllerMgr::Show( true );

	return;
}