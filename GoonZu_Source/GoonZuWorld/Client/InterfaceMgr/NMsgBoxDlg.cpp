#include "NMsgBoxDlg.h"

#include "Interface/Button.h"
#include "Interface/Edit.h"

#include <Client.h>
extern cltCommonLogic* pclClient;

CNMsgBoxDlg::CNMsgBoxDlg()
{
	m_pEdit = NULL;
	m_pOKButton = NULL;
	m_pCancelButton = NULL;

	m_siDataLen = 0;
}

CNMsgBoxDlg::~CNMsgBoxDlg()
{
	if ( m_pEdit )
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}

	if ( m_pOKButton )
	{
		delete m_pOKButton;
		m_pOKButton = NULL;
	}

	if ( m_pCancelButton )
	{
		delete m_pCancelButton;
		m_pCancelButton = NULL;
	}
}

void CNMsgBoxDlg::Create()
{
	if ( ! IsCreate() )
	{
		m_pEdit = new CEdit( this );
		m_pOKButton = new CButton( this );
		m_pCancelButton = new CButton( this );

		CDialogController::Create( NMSGBOX_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNMsgBoxDlg, NULL, TEXT("NInterface/Interface/Interface_20x20_00_109.Spr"), 9, true, 9, 350, 300, 280, 120, true, false, true );
		
		m_pEdit->Create( NMSGBOX_DIALOG_EDIT, NULL, TEXT("NInterface/Common/COMMON_16X15_00_109.SPR"), 0, true, 9, 9, 21, 260, 60, 260, 60, true  );
		m_pOKButton->Create( NMSGBOX_DIALOG_BUTTONOK, TEXT("OK"), TEXT("NInterface/Button/buttona_4x24_00_103.SPR"), 0, true, 3, 50, 86, 72, 24, 72, 24 );
		m_pCancelButton->Create( NMSGBOX_DIALOG_BUTTONCANCEL, TEXT("CANCEL"), TEXT("NInterface/Button/buttona_4x24_00_103.SPR"), 0, true, 3, 155, 86, 72, 24, 72, 24 );

		// 화면의 중앙에 위치시킨다.
		cltClient* pclclient = (cltClient*)pclClient;
		SI16 xPos = ( pclclient->GetScreenXSize() - GetWidth() ) / 2 + 40;
		SI16 yPos = ( pclclient->GetScreenYSize() - GetHeight() ) / 2;
		MovePos( xPos, yPos);


		SetTopMostWindow( true );
		SetModalDialog( true );
	}
}

void CALLBACK CNMsgBoxDlg::StaticCallBackDialogNMsgBoxDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMsgBoxDlg *pThis = (CNMsgBoxDlg *)pControllerMgr;
	pThis->CallBackDialogNMsgBoxDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNMsgBoxDlg::CallBackDialogNMsgBoxDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch ( nEvent )
	{
	case EVENT_BUTTON_CLICKED:
		{
			cltClient *pclclient = ( cltClient * )pclClient;

			bool bExistInterface = false;
			//if ( pclclient->m_pInterfaceMgr->IsExistInterface( m_stMsgBox.pControllerMgr ) )			bExistInterface = true;
			if ( pclclient->m_pInterfaceMgr->IsExistChildInterface( m_stMsgBox.pControllerMgr ) )		bExistInterface = true;
			
			if ( bExistInterface == false)
			{
				DeleteDialog();
				return;
			}

			if ( nControlID == NMSGBOX_DIALOG_BUTTONOK )
			{
				m_stMsgBox.pControllerMgr->RetMsgBox( true, m_stMsgBox.siMsgIndex, m_DataBuffer, m_siDataLen );
			}
			else if ( nControlID == NMSGBOX_DIALOG_BUTTONCANCEL || nControlID == COMMON_DIALOG_CLOSEBUTTON )
			{
				m_stMsgBox.pControllerMgr->RetMsgBox( false, m_stMsgBox.siMsgIndex, m_DataBuffer, m_siDataLen );
			}

			DeleteDialog();
		}
		break;
	}

	return;
}

void CNMsgBoxDlg::SetMsgData( BYTE *pBuffer, BYTE *pData, SI16 DataLen )
{
	if ( pBuffer )
	{
		memcpy( &m_stMsgBox, pBuffer, sizeof( stMsgBox ) );
		TCHAR* pText = GetTxtFromMgr(3887);

		switch( m_stMsgBox.siMsgBoxType )
		{
			case MSGBOX_TYPE_YESNO:
				 pText = GetTxtFromMgr(3887);
				m_pOKButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				pText = GetTxtFromMgr(3888);
				m_pCancelButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				break;
			case MSGBOX_TYPE_OKCANCEL:
				pText = GetTxtFromMgr(3889);
				m_pOKButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				pText = GetTxtFromMgr(3890);
				m_pCancelButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				break;

			//cyj Ok버튼만 있는 MsgBoxDlg 추가
			case MSGBOX_TYPE_OK:
				pText = GetTxtFromMgr(3889);
				m_pOKButton->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
				m_pOKButton->SetControllerPos(103, 86, 72, 24, 72, 24);
				m_pCancelButton->Show(FALSE);
				break;
		}

		if ( _tcslen( m_stMsgBox.strCaption ) > 0 )
		{
			SetTitle( m_stMsgBox.strCaption );
		}
		else
		{	
			SetTitle(  TEXT("") );
		}

		if ( _tcslen( m_stMsgBox.strText ) > 0 )
		{
			m_pEdit->SetText( m_stMsgBox.strText );
		}
		else
		{
			m_pEdit->SetText( TEXT("") );
		}
        
		if ( pData && DataLen > 0 && DataLen < 256 )
		{
			memcpy( m_DataBuffer, pData, DataLen );
			m_siDataLen = DataLen;
		}
		else
		{
			memset( m_DataBuffer, 0, 256 );
			m_siDataLen = 0;
		}

		if ( m_pOKButton )
		{
			RequestFocus( m_pOKButton );
		}
		
	}

	return;
}