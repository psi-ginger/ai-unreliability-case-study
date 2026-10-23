#include "../Client/NInterface/NHozoDlg/NHozoChangedNameDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"


#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/MsgType-System.h"

extern cltCommonLogic* pclClient;

CNHozoChangedNameDlg::CNHozoChangedNameDlg() : CDialogController()
{
	m_siListPage = 1;

	m_pStatic= NULL;
	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pEdit= NULL;
	m_pList= NULL;
}

CNHozoChangedNameDlg::~CNHozoChangedNameDlg()
{
	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pList);

}

void CNHozoChangedNameDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NHozoDlg/DLG_Hozo_ChangedName.ddf"));
	file.CreateChildDialog( this, NHOZOCHANGEDNAME_DLG, TEXT("dialog_ChangedName"), StaticCallBackDialogNHozoChangedNameDlg, pParent );

	NEWCSTATIC(m_pStatic);
	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);
	NEWCEDIT(m_pEdit);
	NEWCLIST(m_pList);

	file.CreateControl( m_pStatic, NHOZOCHANGEDNAME_DIALOG_STATIC, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pButton[0], NHOZOCHANGEDNAME_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NHOZOCHANGEDNAME_DIALOG_BUTTON2, TEXT("button_NONAME2") );
	file.CreateControl( m_pEdit, NHOZOCHANGEDNAME_DIALOG_EDIT, TEXT("editbox_NONAME1") );
	file.CreateControl( m_pList, NHOZOCHANGEDNAME_DIALOG_LIST, TEXT("listbox_NONAME1") );

	TCHAR* pText = GetTxtFromMgr(1564);
	m_pList->SetColumn(0, 72, pText );
	pText = GetTxtFromMgr(2082);
	m_pList->SetColumn(1, 110, pText );
	pText = GetTxtFromMgr(2083);
	m_pList->SetColumn(2, 110, pText );
	m_pList->SetBKColorUse( true );
	m_pList->SetBorder( true );
	m_pList->Refresh();

	m_siListPage = 1 ;
	TCHAR buffer[256] = TEXT("") ;
	pText = GetTxtFromMgr(2084);
	StringCchPrintf(buffer, 256, pText, m_siListPage) ;
	m_pEdit->SetTextPrintOption(DT_CENTER | DT_VCENTER |DT_SINGLELINE);
	m_pEdit->SetText(buffer);
	
	Hide();
}

void CNHozoChangedNameDlg::Action()
{

}

void CNHozoChangedNameDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHozoChangedNameDlg::Show()
{
	CControllerMgr::Show( true );

}

void CALLBACK CNHozoChangedNameDlg::StaticCallBackDialogNHozoChangedNameDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoChangedNameDlg *pThis = (CNHozoChangedNameDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoChangedNameDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoChangedNameDlg::CallBackDialogNHozoChangedNameDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nControlID )
	{
	case NHOZOCHANGEDNAME_DIALOG_BUTTON1:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siListPage <= 1 ) break ;
					cltGameMsgRequest_GetChangedName clinfo(m_siListPage-1);
					cltMsg clMsg( GAMEMSG_REQUEST_GETCHANGEDNAME, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	case NHOZOCHANGEDNAME_DIALOG_BUTTON2:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltGameMsgRequest_GetChangedName clinfo(m_siListPage+1);
					cltMsg clMsg( GAMEMSG_REQUEST_GETCHANGEDNAME, sizeof( clinfo ), (BYTE*)&clinfo );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	}
}
