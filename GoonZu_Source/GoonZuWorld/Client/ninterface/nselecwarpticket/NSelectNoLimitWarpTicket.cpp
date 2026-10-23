//---------------------------------
// 2005/03/07 성준엽
//---------------------------------
#include "NSelectNoLimitWarpTicket.h"

#include "Client.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../../resource.h"
#include "MsgType-Person.h"

#include "Music/Music.h"
#include "Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"

//------------------------------------
// Lib
//------------------------------------
#include "HashTable/HashTable_StringInt.h"

#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

//--------------------------------------------
// cltNSelectNoLimitWarpTicket
//--------------------------------------------
cltNSelectNoLimitWarpTicketDlg::cltNSelectNoLimitWarpTicketDlg()
{
	m_pButton900 = NULL ;
	m_pButton420 = NULL ;
	m_pButton210 = NULL ;
	m_pButton30  = NULL ;
	m_pButton90	 = NULL ;
}

void cltNSelectNoLimitWarpTicketDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		m_pButton900 = new CButton( this ) ;
		m_pButton420 = new CButton( this ) ;
		m_pButton210 = new CButton( this ) ;
		m_pButton30  = new CButton( this ) ;
		m_pButton90  = new CButton( this ) ;

		file.LoadFile(TEXT("NInterface/Data/DLG_NSelectTicket.ddf"));
		file.CreatePopupDialog( this, NSELECTNOLIMITWARPTICKET_DLG, TEXT("dialog_SELECT_NOLIMITWARP"), StaticDlgProc );

		file.CreateControl( m_pButton900, NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON900, TEXT("button_TICKET_900"));
		m_pButton900->SetBorder(true);
		file.CreateControl( m_pButton420, NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON420, TEXT("button_TICKET_420"));
		m_pButton420->SetBorder(true);
		file.CreateControl( m_pButton210, NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON210, TEXT("button_TICKET_210"));
		m_pButton210->SetBorder(true);
		file.CreateControl( m_pButton30, NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON30, TEXT("button_TICKET_30"));
		m_pButton30->SetBorder(true);
		file.CreateControl( m_pButton90, NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON90, TEXT("button_TICKET_90"));
		m_pButton90->SetBorder(true);
	}
	else
	{
		DeleteDialog();
	}
}

cltNSelectNoLimitWarpTicketDlg::~cltNSelectNoLimitWarpTicketDlg()
{
	if ( m_pButton90 )
	{
		delete m_pButton90 ;
		m_pButton90 = NULL ;
	}
	if ( m_pButton30 )
	{
		delete m_pButton30 ;
		m_pButton30 = NULL ;
	}
	if ( m_pButton210 )
	{
		delete m_pButton210 ;
		m_pButton210 = NULL ;
	}
	if ( m_pButton420 )
	{
		delete m_pButton420 ;
		m_pButton420 = NULL ;
	}
	if ( m_pButton900 )
	{
		delete m_pButton900 ;
		m_pButton900 = NULL ;
	}
}

void CALLBACK cltNSelectNoLimitWarpTicketDlg::StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNSelectNoLimitWarpTicketDlg *pThis = (cltNSelectNoLimitWarpTicketDlg*) pControllerMgr;
	pThis->DlgProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNSelectNoLimitWarpTicketDlg::DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	TCHAR buffer[512] = TEXT("") ;

	switch(nControlID)
	{
	case NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON900:
		{
			SI32 itemunique = ITEMUNIQUE(14190) ; // 순간이동[900일]정액권
			if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
			{
				TCHAR* pText = GetTxtFromMgr(3188);
				StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique),
					pclClient->pclItemManager->GetName(itemunique));

				pText = GetTxtFromMgr(3189);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
				break;
			}

			TCHAR* pTitle = GetTxtFromMgr(3189);
			TCHAR* pText = GetTxtFromMgr(3190);
			StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique));

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );

/*			if (  MessageBox(NULL, buffer,pTitle,
				MB_ICONQUESTION | MB_YESNO) == IDNO )
			{
				break;
			}

			SendUseTicketToServer(0);*/
		}
		break;
	case NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON420:
		{
			SI32 itemunique = 14180 ; // 순간이동[420일]정액권
			if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
			{
				TCHAR* pText = GetTxtFromMgr(3188);
				StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique),
						pclClient->pclItemManager->GetName(itemunique));

				pText = GetTxtFromMgr(3189);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
				break;
			}

			TCHAR* pTitle = GetTxtFromMgr(3189);
			TCHAR* pText = GetTxtFromMgr(3190);
			StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique));

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 1 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );

/*			if (  MessageBox(NULL, buffer,pTitle,
				MB_ICONQUESTION | MB_YESNO) == IDNO )
			{
				break;
			}

			SendUseTicketToServer(1);*/
		}
		break;
	case NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON210:
		{
			SI32 itemunique = 14170 ; // 순간이동[210일]정액권
			if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
			{
				TCHAR* pText = GetTxtFromMgr(3188);
				StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique),
					pclClient->pclItemManager->GetName(itemunique));

				pText = GetTxtFromMgr(3189);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
				break;
			}

			TCHAR* pTitle = GetTxtFromMgr(3189);
			TCHAR* pText = GetTxtFromMgr(3190);
			StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique));

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 2 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );

/*			if (  MessageBox(NULL, buffer,pTitle,
				MB_ICONQUESTION | MB_YESNO) == IDNO )
			{
				break;
			}

			SendUseTicketToServer(2);*/
		}
		break;
	case NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON30:
		{
			SI32 itemunique = 14165 ; // 순간이동[30일]정액권
			if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
			{
				TCHAR* pText = GetTxtFromMgr(3188);
				StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique),
					pclClient->pclItemManager->GetName(itemunique));

				pText = GetTxtFromMgr(3189);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
				break;
			}

			TCHAR* pTitle = GetTxtFromMgr(3189);
			TCHAR* pText = GetTxtFromMgr(3190);
			StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique));

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 3 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );

/*			if (  MessageBox(NULL, buffer,pTitle,
				MB_ICONQUESTION | MB_YESNO) == IDNO )
			{
				break;
			}

			SendUseTicketToServer(3);*/
		}
		break;
	case NSELECTNOLIMITWARPTICKET_DIALOG_BUTTON90:
		{
			SI32 itemunique = 14168 ; // 순간이동[90일]정액권
			if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
			{
				TCHAR* pText = GetTxtFromMgr(3188);
				StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique),
					pclClient->pclItemManager->GetName(itemunique));

				pText = GetTxtFromMgr(3189);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText,buffer);
				break;
			}

			TCHAR* pTitle = GetTxtFromMgr(3189);
			TCHAR* pText = GetTxtFromMgr(3190);
			StringCchPrintf(buffer, 512, pText,pclClient->pclItemManager->GetName(itemunique));

			stMsgBox MsgBox;
			MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 4 );
			pclclient->SetMsgBox( &MsgBox, NULL, 0 );

/*			if (  MessageBox(NULL, buffer,pTitle,
				MB_ICONQUESTION | MB_YESNO) == IDNO )
			{
				break;
			}

			SendUseTicketToServer(4);*/
		}
		break;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}

void cltNSelectNoLimitWarpTicketDlg::SendUseTicketToServer(SI08 data) //
{

	/*
	cltClient* pclclient = (cltClient*)pclClient;

	cltMsg clMsg(GAMEMSG_REQUEST_NOLIMITTICKET_WARP, 1, (BYTE*)&data);
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	*/

	//[영진] 기존의 정액제 워프티켓 사용 방식을 바꾼다
}

void cltNSelectNoLimitWarpTicketDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	if ( MsgIndex >= 0 && MsgIndex < 5 && RetValue )
	{
		SendUseTicketToServer(MsgIndex);
	}
}