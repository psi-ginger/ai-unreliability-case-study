#include "NHozoBaseDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/CommonLogic.h"
#include "../CommonLogic/MsgType-System.h"
#include "../CommonLogic/MsgType-Person.h"
#include "../common/Char/CharCommon/Char-Common.h"

extern cltCommonLogic* pclClient;

CNHozoBankruptcyDlg::CNHozoBankruptcyDlg() : CDialogController()
{
	m_siSelectNumber = 0;
}

CNHozoBankruptcyDlg::~CNHozoBankruptcyDlg()
{
}

void CNHozoBankruptcyDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile("NInterface/Data/NHozoDlg/DLG_Hozo_Bankruptcy.ddf");
	file.CreateChildDialog( this, NGOONZUPOLICY_DLG, "dialog_Bankruptcy", StaticCallBackDialogNHozoBankruptcyDlg, pParent );

	file.CreateControl( AddInterface( STATIC,   NHOZOBANKRUPTCY_STATIC1 ), NHOZOBANKRUPTCY_STATIC1, "statictext_NONAME1"); 
	file.CreateControl( AddInterface( STATIC,   NHOZOBANKRUPTCY_STATIC2 ), NHOZOBANKRUPTCY_STATIC2, "statictext_NONAME2"); 
	file.CreateControl( AddInterface( EDIT,     NHOZOBANKRUPTCY_EDIT ), NHOZOBANKRUPTCY_EDIT, "editbox_NONAME1"); 
	file.CreateControl( AddInterface( BUTTON,   NHOZOBANKRUPTCY_BUTTON ), NHOZOBANKRUPTCY_BUTTON, "button_NONAME1"); 
	file.CreateControl( AddInterface( COMBOBOX, NHOZOBANKRUPTCY_COMOBOBOX ), NHOZOBANKRUPTCY_COMOBOBOX, "combobox_NONAME1"); 
	file.CreateControl( AddInterface( OUTLINE,  NHOZOBANKRUPTCY_OUTLINE ), NHOZOBANKRUPTCY_OUTLINE, "outline_NONAME1"); 

	// 정리 대상 마을 설정
	stComboBoxItemData ComboBoxItemData;

	ComboBoxItemData.Set( "2" );
	m_InterfaceFactory_.GetComboBox( NHOZOBANKRUPTCY_COMOBOBOX )->AddItem( &ComboBoxItemData );

	ComboBoxItemData.Set( "3" );
	m_InterfaceFactory_.GetComboBox( NHOZOBANKRUPTCY_COMOBOBOX )->AddItem( &ComboBoxItemData );

	ComboBoxItemData.Set( "4" );
	m_InterfaceFactory_.GetComboBox( NHOZOBANKRUPTCY_COMOBOBOX )->AddItem( &ComboBoxItemData );

	ComboBoxItemData.Set( "5" );
	m_InterfaceFactory_.GetComboBox( NHOZOBANKRUPTCY_COMOBOBOX )->AddItem( &ComboBoxItemData );
	m_InterfaceFactory_.GetComboBox( NHOZOBANKRUPTCY_COMOBOBOX )->SetCursel( 0 );

	Hide();
}

void CNHozoBankruptcyDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHozoBankruptcyDlg::Show()
{
	CControllerMgr::Show( true );

}

CControllerObj* CNHozoBankruptcyDlg::AddInterface( SI16 Type, SI32 TypeID )
{
	return m_InterfaceFactory_.CreateUserInterface( Type, TypeID, this );
}


void CALLBACK CNHozoBankruptcyDlg::StaticCallBackDialogNHozoBankruptcyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHozoBankruptcyDlg *pThis = (CNHozoBankruptcyDlg *)pControllerMgr;
	pThis->CallBackDialogNHozoBankruptcyDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHozoBankruptcyDlg::CallBackDialogNHozoBankruptcyDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHOZOBANKRUPTCY_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI16 tempSelectIndex = m_InterfaceFactory_.GetComboBox( NHOZOBANKRUPTCY_COMOBOBOX )->GetSelectedIndex();
					if( tempSelectIndex < 0 )
						return;

					m_siSelectNumber = atoi( m_InterfaceFactory_.GetComboBox( NHOZOBANKRUPTCY_COMOBOBOX )->GetText( tempSelectIndex ) );

					TCHAR *pTitle = GetTxtFromMgr(6055);
					TCHAR Text[256] = {'\0'};
					TCHAR *pText = GetTxtFromMgr(6056);

					sprintf( Text, pText, m_siSelectNumber);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					MsgBox.Set( pParent, pTitle, Text, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			}
		}
		break;
	}
}

void CNHozoBankruptcyDlg::SetBankCruptcyVillageInfo( SI16 LiquidationVillageNumber, SI32 Date )
{
	TCHAR tempBuffer[256] = {'\0'};
	TCHAR *pText = GetTxtFromMgr(6057);

	cltDate clDate;
	clDate.MakeFullDateFromDateVary(Date);
	StringCchPrintf( tempBuffer, 256, pText, LiquidationVillageNumber, clDate.uiYear, clDate.uiMonth, clDate.uiDay  );
	m_InterfaceFactory_.GetEdit(NHOZOBANKRUPTCY_EDIT)->SetText( tempBuffer );

	clDate.AddMonth(12);
	cltClient *pclclient = (cltClient*)pclClient;
	if( pclclient->clClientDate.GetDateVary() >= clDate.GetDateVary() )
	{
		m_InterfaceFactory_.GetButton(NHOZOBANKRUPTCY_BUTTON)->Enable(true);
	}
	else
	{
		m_InterfaceFactory_.GetButton(NHOZOBANKRUPTCY_BUTTON)->Enable(false);
	}
}

void CNHozoBankruptcyDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:	
		{
			if ( RetValue )
			{
				cltGameMsgRequest_Goonzu_SetLiquidation clinfo( m_siSelectNumber );
				cltMsg clMsg(GAMEMSG_REQUEST_GOONZU_SETLIQUIDATION,sizeof(clinfo),(BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

				TCHAR tempBuffer[256] = {'\0'};
				TCHAR *pText = GetTxtFromMgr(6057);

				cltDate clDate;
				clDate.Set( &pclclient->clClientDate );
				StringCchPrintf( tempBuffer, 256, pText, m_siSelectNumber, clDate.uiYear, clDate.uiMonth, clDate.uiDay  );
				m_InterfaceFactory_.GetEdit(NHOZOBANKRUPTCY_EDIT)->SetText( tempBuffer );
			}
		}
		break;
	}
}