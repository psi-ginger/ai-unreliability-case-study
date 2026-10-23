#include "../Client/NInterface/NHozoDlg/NHozoNewCharRateDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client.h"

extern cltCommonLogic* pclClient;

CNHozoNewCharRateDlg::CNHozoNewCharRateDlg() : CDialogController()
{
	m_pOutLine = NULL;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;
	m_pStatic[2] = NULL;
	m_pStatic[3] = NULL;
	m_pStatic[4] = NULL;
	m_pStatic[5] = NULL;

}

CNHozoNewCharRateDlg::~CNHozoNewCharRateDlg()
{
	SAFE_DELETE(m_pOutLine);

	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[2]);
	SAFE_DELETE(m_pStatic[3]);
	SAFE_DELETE(m_pStatic[4]);
	SAFE_DELETE(m_pStatic[5]);

}

void CNHozoNewCharRateDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHozoDlg/DLG_Hozo_NewCharRate.ddf"));
	file.CreateChildDialog( this, NHOZONEWCHARRATE_DLG, TEXT("dialog_Hozo_NewCharRate"), StaticCallBackDialogNHozoNewCharRateDlg, pParent );

	NEWCOUTLINE(m_pOutLine);

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCSTATIC(m_pStatic[2]);
	NEWCSTATIC(m_pStatic[3]);
	NEWCSTATIC(m_pStatic[4]);
	NEWCSTATIC(m_pStatic[5]);

	file.CreateControl( m_pOutLine, NHOZONEWCHARRATE_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	file.CreateControl( m_pStatic[0], NHOZONEWCHARRATE_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHOZONEWCHARRATE_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NHOZONEWCHARRATE_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NHOZONEWCHARRATE_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
	file.CreateControl( m_pStatic[4], NHOZONEWCHARRATE_DIALOG_STATIC5, TEXT("statictext_NONAME5") );
	file.CreateControl( m_pStatic[5], NHOZONEWCHARRATE_DIALOG_STATIC6, TEXT("statictext_NONAME6") );

	Hide();
}

void CNHozoNewCharRateDlg::Action()
{

}

void CNHozoNewCharRateDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHozoNewCharRateDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHozoNewCharRateDlg::StaticCallBackDialogNHozoNewCharRateDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoNewCharRateDlg *pThis = (CNHozoNewCharRateDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoNewCharRateDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoNewCharRateDlg::CallBackDialogNHozoNewCharRateDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{	
}
