#include "NPromoter_UrlDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"

#include "../../Promoter/NHNChinaPromoter.h"
#include "../../../CommonLogic/Msg/MsgType-NPC.h"

NPromoter_UrlDlg::NPromoter_UrlDlg()
{
	ZeroMemory( m_code, sizeof(m_code) );
	ZeroMemory( m_url,  sizeof(m_url) );
}

NPromoter_UrlDlg::~NPromoter_UrlDlg()
{
	
}

void NPromoter_UrlDlg::Create( CControllerMgr *pParent )
{
	if ( IsCreate() ) return;
	
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NPromoterDlg/NPromoter_UrlDlg.ddf"));

	SI32 controlindex = file.GetControlIndex( TEXT("NPromoter_UrlDlg") ); 
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreateChildDialog( this, NPROMOTER_URL, TEXT("NPromoter_UrlDlg"), StatiNPromoter_UrlDlgProc, pParent);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NPROMOTER_URL_BUTTON_APPLATION,	this ), NPROMOTER_URL_BUTTON_APPLATION,	"button_applation"	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NPROMOTER_URL_BUTTON_LINK_COPY,	this ), NPROMOTER_URL_BUTTON_LINK_COPY,	"button_link_copy"	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NPROMOTER_URL_EDIT_EXPLAIN,		this ),	NPROMOTER_URL_EDIT_EXPLAIN,		"editbox_explain"	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NPROMOTER_URL_EDIT_URL,			this ),	NPROMOTER_URL_EDIT_URL,			"editbox_url"		); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NPROMOTER_URL_STATIC_URL,		this ),	NPROMOTER_URL_STATIC_URL,		"statictext_url"	);

	// 설명.
	m_InterfaceFactory.GetEdit(NPROMOTER_URL_EDIT_EXPLAIN)->SetText( GetTxtFromMgr(30284) );
	
	// URL 정보 요청.
	//Request();
}

void NPromoter_UrlDlg::Action()
{

}

void CALLBACK NPromoter_UrlDlg::StatiNPromoter_UrlDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPromoter_UrlDlg *pThis = (NPromoter_UrlDlg *)pControllerMgr;
	pThis->NPromoter_UrlDlgProc( nEvent, nControlID, pControl );
}
 
void CALLBACK NPromoter_UrlDlg::NPromoter_UrlDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;

		case NPROMOTER_URL_BUTTON_APPLATION:
			{ 
				ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.newhljz.cn/", NULL, SW_SHOWNORMAL );							
			}  
			break;
	
		case NPROMOTER_URL_BUTTON_LINK_COPY:
			{ 
				TCHAR* str = m_InterfaceFactory.GetEdit(NPROMOTER_URL_EDIT_URL)->GetText();
				if( 0 >= _tcslen(str) ) return;

				HGLOBAL HMen;
				HMen = GlobalAlloc(GHND, _tcslen(str) + sizeof(TCHAR) );

				TCHAR* ptr = (TCHAR*)GlobalLock( HMen );
				{
					memcpy( ptr, str, _tcslen(str) + sizeof(TCHAR) );
				}
				GlobalUnlock( HMen );

				if( OpenClipboard( NULL ) )
				{
					EmptyClipboard();
					{
						SetClipboardData( CF_TEXT, HMen );
					}
					CloseClipboard();
				}

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(30285) );

			}
			break;
			
	}
}

void NPromoter_UrlDlg::Hide()
{
	CControllerMgr::Show(false);

}

void NPromoter_UrlDlg::Show()
{
	CControllerMgr::Show(true);

}

void NPromoter_UrlDlg::Request( void )
{
	cltGameRequest_Promoter_URL_Get clinfo;
	cltMsg clMsg( GAMEMSG_REQUEST_PROMOTER_URL_GET, sizeof(clinfo), (BYTE*)&clinfo );
	((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);
}

void NPromoter_UrlDlg::SetURL( TCHAR* _pszURL )
{
	if( 0 >= _tcslen( _pszURL ) )	return;

	m_InterfaceFactory.GetEdit(NPROMOTER_URL_EDIT_URL)->SetText( _pszURL );
}
