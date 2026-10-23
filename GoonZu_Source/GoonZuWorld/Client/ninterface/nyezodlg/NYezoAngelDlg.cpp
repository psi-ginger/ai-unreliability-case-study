#include "../Client/NInterface/NYezodlg/NYezoAngelDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/OutLine.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../common/Char/CharManager/CharManager.h"

extern cltCommonLogic* pclClient;

CNYezoAngelDlg::CNYezoAngelDlg() : CDialogController()
{
	m_pStatic	= NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pOutLine = NULL;
}

CNYezoAngelDlg::~CNYezoAngelDlg()
{
	SAFE_DELETE(m_pStatic);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);

	SAFE_DELETE(m_pOutLine);
}

void CNYezoAngelDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NYezoDlg/DLG_Yezo_Angel.ddf"));
	file.CreateChildDialog( this, NYEZOANGEL_DLG, TEXT("dialog_Yezo_Angel"), StaticCallBackDialogNYezoAngelDlg, pParent );

	NEWCSTATIC(m_pStatic);

	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);

	NEWCOUTLINE(m_pOutLine);

	file.CreateControl( m_pStatic, NYEZOANGEL_DIALOG_STATIC, TEXT("statictext_NONAME1") );

	file.CreateControl( m_pButton[0], NYEZOANGEL_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NYEZOANGEL_DIALOG_BUTTON2, TEXT("button_NONAME2") );

	file.CreateControl( m_pOutLine, NYEZOANGEL_DIALOG_OUTLINE, TEXT("outline_NONAME1") );

	Hide();
}

void CNYezoAngelDlg::Action()
{

}

void CNYezoAngelDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNYezoAngelDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNYezoAngelDlg::StaticCallBackDialogNYezoAngelDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNYezoAngelDlg *pThis = (CNYezoAngelDlg *)pControllerMgr;
	pThis->CallBackDialogNYezoAngelDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNYezoAngelDlg::CallBackDialogNYezoAngelDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case NYEZOANGEL_DIALOG_BUTTON1:	// 임명하기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( pclclient->pclCM->CR[1]->pclCI->clBI.uiGRank == GRANK_MINISTER3 )
					{
						cltMsg clMsg(GAMEMSG_REQUEST_GETANGELAPPLYERLIST,0,NULL);
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					else
					{
						TCHAR* pTitle = GetTxtFromMgr(2100);
						TCHAR* pText = GetTxtFromMgr(2101);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
				}
				break;
			}
		}
		break;

	case NYEZOANGEL_DIALOG_BUTTON2:	// 수호천사 목록 부르기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltMsg clMsg(GAMEMSG_REQUEST_GETANGELLIST,0,NULL);
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	}
}
