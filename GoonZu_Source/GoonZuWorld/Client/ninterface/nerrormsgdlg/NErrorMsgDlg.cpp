/* ==========================================================================
	클래스 :		CNErrorMsgPrint, CNErrorMsgPrint1

	작성일 :		05/05/03
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include "../../../Common/CommonHeader.h"
#include "../../Client.h"

#include "./NErrorMsgDlg.h"

#include "../../../Client/InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include <tchar.h>
//#include "../../lib/WebHTML/WebHTML.h"

extern cltCommonLogic* pclClient;

//CNErrorMsgDlg	g_ErrorMsg;

// CNErrorMsgPrint -----------------------------------------------------------------

CNErrorMsgPrint::CNErrorMsgPrint()
{
	m_bShow = false;
	m_pEdit_error_print = NULL;
}

CNErrorMsgPrint::~CNErrorMsgPrint()
{
	if(m_pEdit_error_print ) delete m_pEdit_error_print;
}

void CNErrorMsgPrint::Create()
{
	if(!IsCreate())
	{
		cltClient* pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile( TEXT("NInterface/Data/NErrorMsgDlg/DLG_ErrorPrint.ddf"));

		file.CreatePopupDialog( this, NERRORMSGPRINT_DLG,  TEXT("dialog_error_print"), NErrorMsgPrintStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID(  TEXT("dialog_error_print") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if (pclclient->m_pDialog[ NSOUNDONOFF_DLG ])
		{
			if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
			{
				this->MovePos(/*DLG_BIG_POS_X -*/ siTmpWidth, DLG_BIG_POS_Y);
			}
			else
			{
				this->MovePos(/*DLG_SMALL_POS_X -*/ siTmpWidth, DLG_SMALL_POS_Y);
			}

		}

		m_pEdit_error_print = new CEdit( this );

		file.CreateControl( m_pEdit_error_print, NERRORMSGPRINT_EDITBOX_ERROR_PRINT,  TEXT("editbox_error_print") );

		m_pEdit_error_print->SetMaxEditTextLength( 7168 );

	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK CNErrorMsgPrint::NErrorMsgPrintStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
    CNErrorMsgPrint *pThis = (CNErrorMsgPrint*)pControllerMgr;
	pThis->NErrorMsgPrintProc( nEvent, nControlID, pControl );

}

void CALLBACK CNErrorMsgPrint::NErrorMsgPrintProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}

void CNErrorMsgPrint::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
}

void CNErrorMsgPrint::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
}

bool CNErrorMsgPrint::IsShow()
{
	return m_bShow;
}

void CNErrorMsgPrint::SetEditText( TCHAR* in_pTmp )
{
    if(m_pEdit_error_print == NULL ) return;
	m_pEdit_error_print->SetText( in_pTmp );
}

void CNErrorMsgPrint::SetTitleCaption( TCHAR* in_pTmp )
{
	if( _tcslen(in_pTmp) >= 260 ) return;
    this->SetTitle( in_pTmp );
}

// CNErrorMsgPrint1 ----------------------------------------------------------------
CNErrorMsgPrint1::CNErrorMsgPrint1()
{
	m_bShow = false;
	m_pEdit_error_print1 = NULL;
}

CNErrorMsgPrint1::~CNErrorMsgPrint1()
{
	if(m_pEdit_error_print1 ) delete m_pEdit_error_print1;
}

void CNErrorMsgPrint1::Create()
{
	if(!IsCreate())
	{
		cltClient* pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile( TEXT("NInterface/Data/NErrorMsgDlg/DLG_ErrorPrint1.ddf"));

		file.CreatePopupDialog( this, NERRORMSGPRINT1_DLG,  TEXT("dialog_error_print1"), NErrorMsgPrint1StaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_error_print1") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(/*DLG_BIG_POS_X -*/ siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(/*DLG_SMALL_POS_X -*/ siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pEdit_error_print1 = new CEdit( this );

		file.CreateControl( m_pEdit_error_print1, NERRORMSGPRINT1_EDITBOX_ERROR_PRINT1,  TEXT("editbox_error_print1") );

		m_pEdit_error_print1->SetMaxEditTextLength( 20480 );
		//m_pEdit_error_print1->SetReadOnly( true );

	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK CNErrorMsgPrint1::NErrorMsgPrint1StaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
    CNErrorMsgPrint1 *pThis = (CNErrorMsgPrint1*)pControllerMgr;
	pThis->NErrorMsgPrint1Proc( nEvent, nControlID, pControl );

}

void CALLBACK CNErrorMsgPrint1::NErrorMsgPrint1Proc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
}

void CNErrorMsgPrint1::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
}

void CNErrorMsgPrint1::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
}

bool CNErrorMsgPrint1::IsShow()
{
	return m_bShow;
}

void CNErrorMsgPrint1::SetEditText( TCHAR* in_pTmp )
{
    if(m_pEdit_error_print1 == NULL ) return;
	m_pEdit_error_print1->SetText( in_pTmp );
}

void CNErrorMsgPrint1::SetTitleCaption( TCHAR* in_pTmp )
{
	if( _tcslen(in_pTmp) >= 260 ) return;
    this->SetTitle( in_pTmp );
}


