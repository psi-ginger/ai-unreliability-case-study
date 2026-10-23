#include "../Client/NInterface/NWarJoinDlg/NWarJoinDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNWarJoinDlg::CNWarJoinDlg() : CDialogController()
{

	for(int i=0; i < 2; ++i)
		m_pBtn[i] = NULL;

	m_pStatic = NULL;
}

CNWarJoinDlg::~CNWarJoinDlg()
{
	for(int i=0; i< 2; ++i)
	{
		if(m_pBtn[i])
		{
			delete m_pBtn[i];
			m_pBtn[i] = NULL;
		}
	}

	if(m_pStatic)
	{
		delete m_pStatic;
		m_pStatic = NULL;
	}
}

void CNWarJoinDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NWarJoinDlg/DLG_WarJoin.ddf"));
		file.CreatePopupDialog( this, NWARJOIN_DLG, TEXT("dialog_WarJoin"), StaticCallBackDialogNWarJoinDlg );

		NEWCBUTTON(m_pBtn[0]);
		NEWCBUTTON(m_pBtn[1]);
		NEWCSTATIC(m_pStatic);

		file.CreateControl( m_pBtn[0], NWARJOIN_DIALOG_JOIN_BUTTON, TEXT("button_NONAME1")); 
		file.CreateControl( m_pBtn[1], NWARJOIN_DIALOG_CANCEL_BUTTON, TEXT("button_NONAME2")); 
		file.CreateControl( m_pStatic, NWARJOIN_DIALOG_STATIC, TEXT("statictext_NONAME1")); 


	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNWarJoinDlg::StaticCallBackDialogNWarJoinDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNWarJoinDlg *pThis = (CNWarJoinDlg *)pControllerMgr;
	pThis->CallBackDialogNWarJoinDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNWarJoinDlg::CallBackDialogNWarJoinDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NWARJOIN_DIALOG_JOIN_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( pclClient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
					{
						TCHAR* pTitle = GetTxtFromMgr(1891);
						TCHAR* pText = GetTxtFromMgr(1892);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return;
					}
					// 영자나라에 있으면 불가능
					if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
						pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
					{
						return ;
					}

					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NWARJOIN_DIALOG_CANCEL_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

