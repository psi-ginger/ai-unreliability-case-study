#include "N2010NewYearDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

//#include "../../CommonLogic/Msg/MsgType-Quest.h"

#include "../../CommonLogic/Msg/MsgType-BokBulBok.h"

CFortuneCookieDlg::CFortuneCookieDlg()
{
	m_pEdit = NULL;
}

CFortuneCookieDlg::~CFortuneCookieDlg()
{
	Destroy();
}

void CFortuneCookieDlg::init()
{
}

void CFortuneCookieDlg::Destroy()
{
	DeleteDialog();
}

void CFortuneCookieDlg::Create()
{
	// 이미 생성되었다면 리턴
	if( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;

	file.LoadFile( TEXT ("Ninterface/Data/DLG_N_NewYear_2010.ddf") );
	file.CreatePopupDialog( this, N2010NEWYEAR_FORTUNE_DLG,	TEXT( "dialog_newyear_fortune" ), StaticNFortuneCookie_DlgProc);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	N2010NEWYEAR_DLG_EDIT_NFORTUNECOOKIE,	this), N2010NEWYEAR_DLG_EDIT_NFORTUNECOOKIE, "editbox_Fortune" );
	 
	m_pEdit = m_InterfaceFactory.GetEdit( N2010NEWYEAR_DLG_EDIT_NFORTUNECOOKIE );
}

void CALLBACK CFortuneCookieDlg::StaticNFortuneCookie_DlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CFortuneCookieDlg *pThis = ( CFortuneCookieDlg * )pControllerMgr;
	pThis->NNFortuneCookie_InfoDlgProc( nEvent, nControlID, pControl);
}

void CALLBACK CFortuneCookieDlg::NNFortuneCookie_InfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	default:
		break;
	}
}

void CFortuneCookieDlg::SetText( TCHAR* pszText )
{
	if ( NULL == m_pEdit ) return;

	m_pEdit->SetTextPrintOption( DT_CENTER | DT_VCENTER );
	m_pEdit->SetText( pszText );
}
