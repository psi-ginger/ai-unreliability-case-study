#include "client.h"


#include "NHouseNameChangeDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "Char\CharManager\CharManager.h"

#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"

#include "../CommonLogic/MsgType-RealEstate.h"
#include "../CommonLogic/House/House.h"
#include "../CommonLogic/MsgType-House.h"

extern cltCommonLogic* pclClient;

CNHouseNameChangeDlg::CNHouseNameChangeDlg()
{
	m_pChangeButton = NULL;
	m_pCancelButton = NULL;
	m_pOldNameEdit = NULL;
	m_pNewNameEdit = NULL;
	m_pTextStatic = NULL;
	m_pText2Static = NULL;
	m_siVillageUnique = 0;
	m_siHouseUnitIndex= 0 ;
}
CNHouseNameChangeDlg::~CNHouseNameChangeDlg()
{
	SAFE_DELETE(m_pChangeButton);
	SAFE_DELETE(m_pCancelButton);
	SAFE_DELETE(m_pOldNameEdit);
	SAFE_DELETE(m_pNewNameEdit);
	SAFE_DELETE(m_pTextStatic);
	SAFE_DELETE(m_pText2Static);
}

void CNHouseNameChangeDlg::Destroy()
{
	DeleteDialog();
}

void CNHouseNameChangeDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NStructureDlg/DLG_NHouseChangeNameDlg.ddf");
	
		NEWCBUTTON(m_pChangeButton);
		NEWCBUTTON(m_pCancelButton);
		NEWCEDIT(m_pOldNameEdit);
		NEWCSTATIC(m_pTextStatic);
		NEWCSTATIC(m_pText2Static);
		NEWCEDIT(m_pNewNameEdit);
	
		file.CreatePopupDialog( this, NHOUSENAMECHANGE_DLG, "dialog_HouseNameChange", StaticCNHouseNameChangeProc);

		file.CreateControl( m_pChangeButton, NHOUSENAMECHANGE_BUTTON_CHANGE,   "button_Change"); 
		file.CreateControl( m_pCancelButton, NHOUSENAMECHANGE_BUTTON_CANCEL,   "button_Cancel"); 
		file.CreateControl( m_pOldNameEdit, NHOUSENAMECHANGE_EDIT_OLDNAME,   "editbox_OldName"); 
		file.CreateControl( m_pTextStatic, NHOUSENAMECHANGE_STATIC_TEXT,   "statictext_Text"); 
		file.CreateControl( m_pText2Static, NHOUSENAMECHANGE_STATIC_TEXT2,   "statictext_Text2"); 
		file.CreateControl( m_pNewNameEdit, NHOUSENAMECHANGE_EDIT_NEWNAME,   "editbox_NewName");
	}

	m_pTextStatic->SetFontColor( RGB( 24, 42, 89 ) );
	m_pTextStatic->SetFontWeight(FW_BOLD);
	m_pTextStatic->SetTextPrintOption( DT_RIGHT | DT_VCENTER | DT_SINGLELINE );
	m_pTextStatic->SetText(GetTxtFromMgr(40375));
	m_pTextStatic->Refresh();	

	m_pText2Static->SetFontColor( RGB( 24, 42, 89 ) );
	m_pText2Static->SetFontWeight(FW_BOLD);
	m_pText2Static->SetTextPrintOption( DT_RIGHT | DT_VCENTER | DT_SINGLELINE );
	m_pText2Static->SetText(GetTxtFromMgr(40376));
	m_pText2Static->Refresh();

	m_pOldNameEdit->SetFontColor( RGB( 24, 42, 89 ) );
	m_pOldNameEdit->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	m_pOldNameEdit->Refresh();

	m_pNewNameEdit->SetTabKeyPerssNotifyUse(TRUE);
	m_pNewNameEdit->SetMaxEditTextLength(MAX_PLAYER_NAME-2);
	m_pNewNameEdit->SetAutoScroll(TRUE);
	m_pNewNameEdit->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	m_pNewNameEdit->Enable(TRUE);
	m_pNewNameEdit->Refresh();

	SetFocus(m_pNewNameEdit);

	//m_pNewNameEdit->SetFocus(TRUE);
	
	return;
}

void CALLBACK CNHouseNameChangeDlg::StaticCNHouseNameChangeProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNHouseNameChangeDlg *pThis = (CNHouseNameChangeDlg*)pControllerMgr;
	if( NULL != pThis)
	{
		pThis->NHouseNameChangeDlgProc( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNHouseNameChangeDlg::NHouseNameChangeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	switch( nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
			break;	
		};
	case NHOUSENAMECHANGE_BUTTON_CANCEL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
					break;
				}
			}
			break;	
		}
	case NHOUSENAMECHANGE_BUTTON_CHANGE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( _tcslen( m_pNewNameEdit->GetText() ) < 1 )
						break;

					TCHAR* pTitle = GetTxtFromMgr(40368);
					NTCHARString256 pText(GetTxtFromMgr(40374));

					TCHAR* pszNameText = m_pNewNameEdit->GetText();

					if(pclClient->m_pCAbuseFilter->CheckAbuseWord(pszNameText))
					{
						TCHAR * pErrText = GetTxtFromMgr(1854);
						TCHAR * pErrTitle = GetTxtFromMgr(1853);

						if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
						{
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pErrTitle , pErrText);						
						}
						return;
					}

					pText.Replace("#housename#", pszNameText);
					
					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 7 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

					break;
				}
			}
			break;	
		}
	}

	return;
}

VOID CNHouseNameChangeDlg::SetVillageUnique(SI32 siVillageUnique)
{
	m_siVillageUnique = siVillageUnique;
}
VOID CNHouseNameChangeDlg::SetHouseUnitIndex(SI32 siHouseUnitIndex)
{
	m_siHouseUnitIndex = siHouseUnitIndex;
}
VOID CNHouseNameChangeDlg::SetOldHouseName(TCHAR *pszOldHouseName)
{
	m_pOldNameEdit->SetText(pszOldHouseName);
	m_pOldNameEdit->Enable(FALSE);
	m_pOldNameEdit->Refresh();
}

VOID CNHouseNameChangeDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 7:	// 계약 연장
		{
			if( RetValue )
			{
				cltGameMsgRequest_ChangeHouseUnitName clGameMsgRequest_ChangeHouseUnitName( m_siVillageUnique, m_siHouseUnitIndex, m_pNewNameEdit->GetText());
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITNAMECHANGE, sizeof(clGameMsgRequest_ChangeHouseUnitName), (BYTE*)&clGameMsgRequest_ChangeHouseUnitName );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

				DeleteDialog();
			}
		}
		break;
	}
}