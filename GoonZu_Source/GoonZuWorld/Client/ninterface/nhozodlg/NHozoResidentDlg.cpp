#include "../Client/NInterface/NHozoDlg/NHozoResidentDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Client.h"

extern cltCommonLogic* pclClient;

CNHozoResidentDlg::CNHozoResidentDlg() : CDialogController()
{
	m_pOutLine = NULL;

	for(SI32 i=0;i<10;++i)
		m_pStatic[i] = NULL;

	m_pEdit[0] = NULL;
    m_pEdit[1] = NULL;
}

CNHozoResidentDlg::~CNHozoResidentDlg()
{
	SAFE_DELETE(m_pOutLine);
	for(SI32 i=0;i<10;++i)
		SAFE_DELETE(m_pStatic[i]);

	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);
	
}

void CNHozoResidentDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHozoDlg/DLG_Hozo_Resident.ddf"));
	file.CreateChildDialog( this, NHOZORESIDENT_DLG, TEXT("dialog_Hozo_Resident"), StaticCallBackDialogNHozoResidentDlg, pParent );

	m_pOutLine = new COutLine( this );

	for(SI32 i=0;i<10;++i)
		m_pStatic[i] = new CStatic( this );

	m_pEdit[0]= new CEdit( this );
	m_pEdit[1]= new CEdit( this );

	file.CreateControl( m_pStatic[0], NHOZORESIDENT_DIALOG_STATIC1, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHOZORESIDENT_DIALOG_STATIC2, TEXT("statictext_NONAME2") );
	file.CreateControl( m_pStatic[2], NHOZORESIDENT_DIALOG_STATIC3, TEXT("statictext_NONAME3") );
	file.CreateControl( m_pStatic[3], NHOZORESIDENT_DIALOG_STATIC4, TEXT("statictext_NONAME4") );
	file.CreateControl( m_pStatic[4], NHOZORESIDENT_DIALOG_STATIC5, TEXT("statictext_NONAME5") );
	file.CreateControl( m_pStatic[5], NHOZORESIDENT_DIALOG_STATIC6, TEXT("statictext_NONAME6") );
	file.CreateControl( m_pStatic[6], NHOZORESIDENT_DIALOG_STATIC7, TEXT("statictext_NONAME7") );
	file.CreateControl( m_pStatic[7], NHOZORESIDENT_DIALOG_STATIC8, TEXT("statictext_NONAME8") );
	file.CreateControl( m_pStatic[8], NHOZORESIDENT_DIALOG_STATIC9, TEXT("statictext_NONAME9") );
	file.CreateControl( m_pStatic[9], NHOZORESIDENT_DIALOG_STATIC10, TEXT("statictext_NONAME10") );

	file.CreateControl( m_pEdit[0], NHOZORESIDENT_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
	file.CreateControl( m_pEdit[1], NHOZORESIDENT_DIALOG_EDIT2, TEXT("editbox_NONAME2") );

	file.CreateControl( m_pOutLine, NHOZORESIDENT_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	Hide();
}

void CNHozoResidentDlg::Action()
{

}

void CNHozoResidentDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHozoResidentDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHozoResidentDlg::StaticCallBackDialogNHozoResidentDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoResidentDlg *pThis = (CNHozoResidentDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoResidentDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoResidentDlg::CallBackDialogNHozoResidentDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
}


