#include "../Client/NInterface/NHozoDlg/NHozoPrizeDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client.h"


extern cltCommonLogic* pclClient;

CNHozoPrizeDlg::CNHozoPrizeDlg() : CDialogController()
{
	m_pOutLine = NULL;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pStatic[3] = NULL;

	m_pEdit = NULL;

}

CNHozoPrizeDlg::~CNHozoPrizeDlg()
{
	SAFE_DELETE(m_pOutLine);

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[3]);

	SAFE_DELETE(m_pEdit);

}

void CNHozoPrizeDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHozoDlg/DLG_Hozo_Prize.ddf"));
	file.CreateChildDialog( this, NHOZOPRIZE_DLG, TEXT("dialog_Hozo_Prize"), StaticCallBackDialogNHozoPrizeDlg, pParent );

	NEWCOUTLINE(m_pOutLine);

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);
	NEWCSTATIC(m_pStatic[3]);
	
	NEWCEDIT(m_pEdit);
	
	file.CreateControl( m_pStatic[0], NHOZOPRIZE_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHOZOPRIZE_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NHOZOPRIZE_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NHOZOPRIZE_DIALOG_STATIC4, TEXT("statictext_NONAME4") );

	file.CreateControl( m_pEdit, NHOZOPRIZE_DIALOG_EDIT, TEXT("editbox_NONAME1") );

	file.CreateControl( m_pOutLine, NHOZOPRIZE_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	Hide();
}

void CNHozoPrizeDlg::Action()
{

}

void CNHozoPrizeDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHozoPrizeDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHozoPrizeDlg::StaticCallBackDialogNHozoPrizeDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoPrizeDlg *pThis = (CNHozoPrizeDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoPrizeDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoPrizeDlg::CallBackDialogNHozoPrizeDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}
