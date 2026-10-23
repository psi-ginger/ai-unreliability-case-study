//---------------------------------
// 2005/03/07 ¼ºÁØ¿±
//---------------------------------
#include "NNoticeAdminDlg.h"

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

#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

extern cltCommonLogic* pclClient;

//--------------------------------------------
// cltNNoticeAdmin
//--------------------------------------------
cltNNoticeAdminDlg::cltNNoticeAdminDlg()
{
	m_pSpr = NULL ;
	m_pEdit = NULL ;
}

void cltNNoticeAdminDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		TCHAR* pText = GetTxtFromMgr(3380);

		CInterfaceFile file;

		m_pSpr = new CImageStatic( this ) ;
		m_pEdit = new CEdit( this ) ;

		file.LoadFile(TEXT("NInterface/Data/DLG_NNoticeAdmin.ddf"));
		file.CreatePopupDialog( this, NNOTICEADMIN_DLG, TEXT("dialog_NOTICE_ADMIN"), StaticDlgProc );

		file.CreateControl( m_pSpr, NNOTICEADMIN_DIALOG_SPR, TEXT("imagestatic_notice"));
		m_pSpr->SetBorder(true);
		file.CreateControl( m_pEdit, NNOTICEADMIN_DIALOG_EDIT, TEXT("editbox_explain"));
	}
	else
	{
		DeleteDialog();
	}
}

cltNNoticeAdminDlg::~cltNNoticeAdminDlg()
{
	if ( m_pEdit )
	{
		delete m_pEdit ;
		m_pEdit = NULL ;
	}
	if ( m_pSpr )
	{
		delete m_pSpr ;
		m_pSpr = NULL ;
	}
}

void CALLBACK cltNNoticeAdminDlg::StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	cltNNoticeAdminDlg *pThis = (cltNNoticeAdminDlg*) pControllerMgr;
	pThis->DlgProc( nEvent, nControlID, pControl );
}

void CALLBACK cltNNoticeAdminDlg::DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	//cltClient *pclclient = (cltClient *)pclClient;
	//TCHAR buffer[512] = TEXT("") ;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}

void cltNNoticeAdminDlg::SetData(void *pBuffer)
{
	TCHAR * buffer = (TCHAR*)pBuffer ;

	if ( buffer == NULL )
	{
		DeleteDialog();
		return ;
	}

	if ( pclClient->siServiceArea == ConstServiceArea_English )
	{
		m_pEdit->SetLineHeight( 14 );
		m_pEdit->SetFontKind(TEXT("Times New Roman"));
		m_pEdit->SetFontHeight(14);
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_NHNChina )
	{
		m_pEdit->SetLineHeight( 14 );
		m_pEdit->SetFontKind(TEXT("Times New Roman"));
		m_pEdit->SetFontHeight(14);
	}
	else if( pclClient->siServiceArea == ConstServiceArea_China )
	{
		m_pEdit->SetLineHeight( 18 );
		m_pEdit->SetFontKind(TEXT("KaiTi_GB2312"));
		m_pEdit->SetFontHeight( 18 );
	}
	else if ( pclClient->siServiceArea == ConstServiceArea_EUROPE )
	{
		m_pEdit->SetLineHeight( 14 );
		m_pEdit->SetFontKind(TEXT("Times New Roman"));
		m_pEdit->SetFontHeight(14);
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(3381);
		m_pEdit->SetLineHeight( 20 );
		m_pEdit->SetFontKind(pText);
		m_pEdit->SetFontHeight(20);
	}

	m_pSpr->SetFileName(TEXT("Interface/EventNotice/NoticeAdmin.SPR"));
	m_pSpr->SetFontIndex(0);
	m_pEdit->SetBorder(true);
	m_pEdit->SetFontWeight( FW_BOLD );
	m_pEdit->SetFontColor( RGB( 0, 0, 255 ) );
	m_pEdit->SetText(buffer) ;

	m_pEdit->Refresh() ;
}