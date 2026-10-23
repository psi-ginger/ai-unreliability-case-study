//---------------------------------
// 2005/03/07 성준엽
//---------------------------------
#include "NBobusangDlg.h"

#include "Client.h"

#include "Char/CharCommon/Char-Common.h"
#include "Char/CharManager/CharManager.h"

#include "../../../resource.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-Quest.h"

#include "Music/Music.h"
#include "Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../Common/SystemNPC/SystemNPC.h"

//------------------------------------
// Lib
//------------------------------------
#include "HashTable/HashTable_StringInt.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/ListView.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

//--------------------------------------------
// cltNBobusangDlg
//--------------------------------------------
cltNBobusangDlg::cltNBobusangDlg()
{
	//m_pPortrait = NULL ;

	//m_pEdit = NULL ;

	//m_pListView = NULL ;

	//m_pOKButton = NULL ; // 수락

	//m_pButton[0] = NULL;
	//m_pButton[1] = NULL;
	//m_pButton[2] = NULL;
}

void cltNBobusangDlg::Create()
{
//	cltClient *pclclient = (cltClient *)pclClient;
//
//	if ( ! IsCreate() )
//	{
//		CInterfaceFile file;
//
//		m_pPortrait = new CImageStatic( this );
//
//		m_pEdit = new CEdit( this );
//
//		m_pListView = new CEdit( this );
//
//		m_pOKButton = new CButton( this ) ;
//
//		NEWCBUTTON(m_pButton[0]); // 독도
//		NEWCBUTTON(m_pButton[1]); // 학교등록
//		NEWCBUTTON(m_pButton[2]); // 군주캡틴 포인트
//
//		file.LoadFile(TEXT("NInterface/Data/DLG_NBobusang.ddf"));
//		file.CreatePopupDialog( this, NBOBUSANG_DLG, TEXT("dialog_BOBUSANG"), StaticDlgProc );
//
//		file.CreateControl(m_pPortrait,NBOBUSANG_DIALOG_PORTRAIT,TEXT("imagestatic_PORTRAIT"));
//		m_pPortrait->SetBorder(true);
//		m_pPortrait->SetFileName(TEXT("Interface/Portrait/PortraitIcon.SPR"));
//		m_pPortrait->SetFontIndex(pclClient->GetUniqueFromHash(TEXT("KIND_BOBUSANG"))-1);
//
//		TCHAR* pText = GetTxtFromMgr(3373);
//		file.CreateControl( m_pEdit, NBOBUSANG_DIALOG_EDIT, TEXT("editbox_BOBUSANG_EDIT"));
//		m_pEdit->SetBorder(true);
//		m_pEdit->SetText(pText);
//
//		file.CreateControl( m_pOKButton, NBOBUSANG_DIALOG_BUTTON, TEXT("button_OK"));
//		m_pOKButton->SetBorder(true);
//
//		file.CreateControl( m_pButton[0], NBOBUSANG_DIALOG_BUTTON1, TEXT("button_NONAME1"));
//		file.CreateControl( m_pButton[1], NBOBUSANG_DIALOG_BUTTON2, TEXT("button_NONAME2"));
//		file.CreateControl( m_pButton[2], NBOBUSANG_DIALOG_BUTTON3, TEXT("button_NONAME3"));
//
//#ifdef _DEBUG
//		m_pButton[0]->Enable(TRUE);
//		m_pButton[1]->Enable(TRUE);
//		m_pButton[2]->Enable(TRUE);
//#else
//		m_pButton[0]->Enable(TRUE);
//		m_pButton[1]->Enable(FALSE);
//		m_pButton[2]->Enable(FALSE);
//#endif
////		m_pButton[1]->Enable(FALSE);
////		m_pButton[2]->Enable(FALSE);
//
//		file.CreateControl( m_pListView, NBOBUSANG_DIALOG_LISTVIEW, TEXT("editbox_EVENT_EDIT"));
//		m_pListView->SetBorder(true);
//		m_pListView->Show( FALSE );
//
//
//		//cltMsg clMsg(GAMEMSG_REQUEST_PRIZE_GAMEEVENT,0,NULL);
//		//pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	//}
	//else
	//{
	//	/*DeleteDialog();*/
	//}
}

cltNBobusangDlg::~cltNBobusangDlg()
{
	//SAFE_DELETE(m_pButton[2]);
	//SAFE_DELETE(m_pButton[1]);
	//SAFE_DELETE(m_pButton[0]);

	//if ( m_pOKButton )
	//{
	//	delete m_pOKButton ;
	//	m_pOKButton = NULL ;
	//}
	//if ( m_pListView )
	//{
	//	delete m_pListView ;
	//	m_pListView = NULL ;
	//}
	//if ( m_pEdit )
	//{
	//	delete m_pEdit ;
	//	m_pEdit = NULL ;
	//}
	//if ( m_pPortrait )
	//{
	//	delete m_pPortrait ;
	//	m_pPortrait = NULL ;
	//}
}

void cltNBobusangDlg::SetData( void *pData )
{
	//stBobusang* pclinfo = (stBobusang*)pData;

	//if ( pclinfo->siType == 0 )
	//{
	//	TCHAR* pText = GetTxtFromMgr(3377);
	//	if ( pclinfo->bGameEvent == true )
	//	{
	//		m_pListView->SetText(pText);
	//	}
	//	else
	//	{
	//		pText = GetTxtFromMgr(3376);
	//		m_pListView->SetText(pText);
	//	}

	//	m_pListView->Show(true);
	//	m_pButton[0]->Show(false);
	//	m_pButton[1]->Show(false);
	//	m_pButton[2]->Show(false);
	//}
	//else if ( pclinfo->siType == 1 )
	//{
	//}
}

void CALLBACK cltNBobusangDlg::StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	//cltNBobusangDlg *pThis = (cltNBobusangDlg*) pControllerMgr;
	//pThis->DlgProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNBobusangDlg::DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
//	cltClient *pclclient = (cltClient *)pclClient;
//
//	switch(nControlID)
//	{
////	case NBOBUSANG_DIALOG_BUTTON:
//	case COMMON_DIALOG_CLOSEBUTTON:
//		{
//			DeleteDialog();
//		}
//		break;
//
//	case NBOBUSANG_DIALOG_BUTTON:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					DeleteDialog();
//				}
//				break;
//			}
//		}
//		break;
//
//	case NBOBUSANG_DIALOG_BUTTON1:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					/*cltMsg clMsg(GAMEMSG_REQUEST_COUNTATTACK_INFO,0,NULL);
//					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);*/
//					cltMsg clMsg(GAMEMSG_REQUEST_PRIZE_GAMEEVENT,0,NULL);
//					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
//				}
//				break;
//			}
//		}
//		break;
//
//	case NBOBUSANG_DIALOG_BUTTON2:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					//cyj SCHOOLList 삭제
//					//pclclient->CreateInterface(NSCHOOLLIST_DLG);
//				}
//				break;
//			}
//		}
//		break;
//
//	case NBOBUSANG_DIALOG_BUTTON3:
//		{
//			switch( nEvent )
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//				}
//				break;
//			}
//		}
//		break;
//	}
}
