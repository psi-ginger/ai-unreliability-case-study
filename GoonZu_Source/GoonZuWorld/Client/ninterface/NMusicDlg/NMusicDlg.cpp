#include "NMusicDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../../Client/Interface/LoginNoticeDlg/LoginNoticeDlg.h"

extern cltCommonLogic* pclClient;

CNMusicDlg::CNMusicDlg()
{	
	m_siState = MAKE_MUSICWINDOW;
}

CNMusicDlg::~CNMusicDlg()
{
}

void CNMusicDlg::Create()
{
	if ( !IsCreate() )
	{	
		cltClient* pclclient = (cltClient*)pclClient;

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NMusicDlg/DLG_NMusic.ddf"));
		file.CreatePopupDialog( this, NMUSIC_DLG , TEXT("dialog_Music"), StaticNMusicDlgProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMUSIC_BUTTON , this ), NMUSIC_BUTTON,   "button_NONAME1");
		MovePos(302,68);

		Hide();
	}
	else
	{
		DeleteDialog();
	}
}

void CNMusicDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNMusicDlg::Hide()
{
	CControllerMgr::Show( false );
}

void CNMusicDlg::SetState( SI32 State )
{
	m_siState = State;
}

void CALLBACK CNMusicDlg::StaticNMusicDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMusicDlg *pThis = (CNMusicDlg*) pControllerMgr;
	pThis->NMusicDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNMusicDlg::NMusicDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case NMUSIC_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( MAKE_MUSICWINDOW == m_siState )
					{
						TCHAR* text = GetTxtFromMgr(10202);
						g_LoginNoticeDlg.Set( TEXT("http://radio.hljz.net/"), 412, 138, CLoginNoticeDlg::MUSIC_DLG, text );
						m_siState = HIDE_MUSICWINDOW;
					}
					else if( SHOW_MUSICWINDOW == m_siState )
					{
						if( CLoginNoticeDlg::MUSIC_DLG == g_LoginNoticeDlg.m_siType )
						{
							g_LoginNoticeDlg.SetShowWindow( true );
							m_siState = HIDE_MUSICWINDOW;
						}
					}
					else if( HIDE_MUSICWINDOW == m_siState )
					{
						if( CLoginNoticeDlg::MUSIC_DLG == g_LoginNoticeDlg.m_siType )
						{
							g_LoginNoticeDlg.SetShowWindow( false );
							m_siState = SHOW_MUSICWINDOW;
						}
					}
				}
				break;
			}
		}
		break;
	}
}

