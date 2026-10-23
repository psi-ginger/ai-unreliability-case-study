#include "../../InterfaceMgr/InterfaceFile.h"
#include "NDownloadAddon.h"

CNDownloadAddonDlg::CNDownloadAddonDlg()
{
	m_pEditExplian = NULL;
	m_pButtonDownload = NULL;
}

CNDownloadAddonDlg::~CNDownloadAddonDlg()
{
	SAFE_DELETE(m_pEditExplian);
	SAFE_DELETE(m_pButtonDownload);
}

void CNDownloadAddonDlg::Create()
{
	NEWCEDIT(m_pEditExplian);
	NEWCBUTTON(m_pButtonDownload);

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NDownloadAddonDlg/DLG_DownloadAddon.ddf"));
	file.CreatePopupDialog( this, NDOWNLOADADDON_DLG , TEXT("dialog_downloadaddon"), StaticCallBackDialogNDownloadAddon );

	file.CreateControl( m_pEditExplian, NDOWNLOADADDON_DLG_EXPLAIN_EDIT, TEXT("editbox_explain") );
	file.CreateControl( m_pButtonDownload, NDOWNLOADADDON_DLG_DOWNLOAD_BUTTON, TEXT("button_download") );

	TCHAR* pText = TEXT("");
	pText = GetTxtFromMgr(6235);
	m_pEditExplian->SetText( pText );

}
void CALLBACK CNDownloadAddonDlg::StaticCallBackDialogNDownloadAddon( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNDownloadAddonDlg *pThis = (CNDownloadAddonDlg*)pControllerMgr;
	pThis->CallBackDialogNDownloadAddon( nEvent, nControlID, pControl );

}
void CALLBACK CNDownloadAddonDlg::CallBackDialogNDownloadAddon( UINT nEvent, int nControlID, CControllerObj* pControl )
{
    switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		DeleteDialog();
		break;
	case NDOWNLOADADDON_DLG_DOWNLOAD_BUTTON:
		switch( nEvent )
		{
		case EVENT_BUTTON_CLICKED:
			ShellExecute( NULL, NULL, TEXT("http://global.goonzu.com/center/download/download.asp?menu=51"), NULL, NULL, SW_SHOWNORMAL );
			break;
		}
	default:
		break;
	}
}