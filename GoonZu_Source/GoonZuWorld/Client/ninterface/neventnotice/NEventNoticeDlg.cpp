// EventNotice.cpp: implementation of the CNEventNotice class.
//
//////////////////////////////////////////////////////////////////////



#include "NEventNoticeDlg.h"
#include "../../Client/client.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/interface/Edit.h"



#include "..\..\FontManager\FontManager.h"
#include "../../../lib/WebHTML/WebHTML.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"



extern cltCommonLogic* pclClient;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CNEventNoticeDlg::CNEventNoticeDlg()
{
	
	m_bRectSwitch = FALSE ;
	m_pNEventNoticeStatic= NULL;
	m_pNEventNoticeEdit = NULL;

	
}


CNEventNoticeDlg::~CNEventNoticeDlg()
{
	SAFE_DELETE(m_pNEventNoticeStatic)	;
	SAFE_DELETE(m_pNEventNoticeEdit)	;

}

void CNEventNoticeDlg::Create( )
{
	if(!IsCreate())
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NEventNotice/DLG_NEventNotice.ddf"));
		file.CreatePopupDialog( this, NEVENTNOTIC_DLG, TEXT("dialog_EventNoticeDlg"), StatiCNEventNoticeDlgProc);		

		m_pNEventNoticeStatic = new CStatic( this );             
		m_pNEventNoticeEdit = new CEdit( this );       

		file.CreateControl(m_pNEventNoticeStatic, NEVENTNOTIC_DIALOG_STATIC , TEXT("statictext_EventNoticeStatic"));
		file.CreateControl(m_pNEventNoticeEdit, NEVENTNOTIC_DIALOG_EDIT , TEXT("editbox_EventNoticeEdit"));		

	}

}

void CALLBACK CNEventNoticeDlg::StatiCNEventNoticeDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	
	CNEventNoticeDlg *pThis = (CNEventNoticeDlg *) pControllerMgr;
	pThis->NEventNoticeDlgProc( nEvent, nControlID, pControl );
}




void CALLBACK CNEventNoticeDlg::NEventNoticeDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl  )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}

	return ;
}

void CNEventNoticeDlg::Set( TCHAR * szGift, SI16 txtSize, TCHAR * Txt)
{
	if(!IsCreate())
	{				
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NEventNotice/DLG_NEventNotice.ddf"));
		file.CreatePopupDialog( this, NEVENTNOTIC_DLG, TEXT("dialog_EventNoticeDlg"), StatiCNEventNoticeDlgProc);		
		

		m_pNEventNoticeStatic = new CStatic( this );             
		m_pNEventNoticeEdit = new CEdit( this );       

		file.CreateControl(m_pNEventNoticeStatic, NEVENTNOTIC_DIALOG_STATIC , TEXT("statictext_EventNoticeStatic"));
		file.CreateControl(m_pNEventNoticeEdit, NEVENTNOTIC_DIALOG_EDIT , TEXT("editbox_EventNoticeEdit"));
		
	}

	/*  spr 파일 입력후 열기 바람

	*/
	TCHAR buf[256]=TEXT("");
	StringCchCopy(szGift, txtSize, TEXT("woofer"));
	StringCchPrintf(buf, 256, TEXT("Interface\\EventNotice\\%s.spr"), szGift);	
	m_pNEventNoticeStatic->SetFileName(buf);
	//m_pNEventNoticeStatic1->SetFontIndex(0);

	if (pclClient->siServiceArea != ConstServiceArea_English 
		|| pclClient->siServiceArea != ConstServiceArea_NHNChina
		|| pclClient->siServiceArea != ConstServiceArea_EUROPE
	)
	{
        m_pNEventNoticeEdit->SetFontKind(TEXT("굴림"));
	}

	m_pNEventNoticeEdit->SetFontWeight( FW_BOLD );
	m_pNEventNoticeEdit->SetFontHeight(20);
	m_pNEventNoticeEdit->SetLineHeight(20);
	m_pNEventNoticeEdit->SetText(Txt);





	m_pNEventNoticeEdit->Refresh();

}

