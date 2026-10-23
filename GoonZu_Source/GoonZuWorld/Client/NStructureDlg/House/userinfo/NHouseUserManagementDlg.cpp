#include "../Client/NStructureDlg/House/UserInfo/NHouseUserManagementDlg.h"

#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../Client/Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../Client/Interface/HouseUnit/HouseUnitBuffer.h"
#include "../common/Char/CharCommon/Char-EconomyInfo.h"

#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"

//#include "../../FontManager/FontManager.h"
//#include "../../../lib/WebHTML/WebHTML.h"

#include "../../CommonLogic/MsgType-House.h"
#include "../../CommonLogic/Village/Village.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNHouseUserManagementDlg::CNHouseUserManagementDlg() : CDialogController()
{
	m_siCurrentVillageIndex = 0;
	m_siCurrentHouseIndex = 0;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;

	m_pEdit[0] = NULL;
	m_pEdit[0] = NULL;

	m_pButton = NULL;
}

CNHouseUserManagementDlg::~CNHouseUserManagementDlg()
{
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);
	SAFE_DELETE(m_pButton);

}

void CNHouseUserManagementDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NHouseUserInfoDlg/DLG_HouseUserManagement.ddf"));
	file.CreateChildDialog( this, NHOUSEUSERMANAGEMENT_DLG, TEXT("dialog_HouseUserManagement"), StaticCallBackDialogNHouseUserManagementDlg, pParent );

	NEWCSTATIC(m_pStatic[0]);
	NEWCSTATIC(m_pStatic[1]);
	NEWCEDIT(m_pEdit[0]);
	NEWCEDIT(m_pEdit[1]);
	NEWCBUTTON(m_pButton);

	file.CreateControl( m_pStatic[0], NHOUSEUSERMANAGEMENT_DIALOG_STATIC, TEXT("statictext_NONAME1") );
	file.CreateControl( m_pStatic[1], NHOUSEUSERMANAGEMENT_DIALOG_STATIC, TEXT("statictext_NONAME2") );

	file.CreateControl( m_pEdit[0], NHOUSEUSERMANAGEMENT_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
	file.CreateControl( m_pEdit[1], NHOUSEUSERMANAGEMENT_DIALOG_EDIT2, TEXT("editbox_NONAME2") );

	file.CreateControl( m_pButton, NHOUSEUSERMANAGEMENT_DIALOG_BUTTON, TEXT("button_NONAME1") );

	Hide();
}

void CNHouseUserManagementDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHouseUserManagementDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNHouseUserManagementDlg::Action()
{
	TCHAR temp[1024] = TEXT("");
	TCHAR temp1[1024] = TEXT("");
	TCHAR buffer[1024] = TEXT("");

	SI32 id = 1;
	cltRealEstateUnit* pclinfo = pclClient->pclCM->CR[id]->pclCI->clRealEstate.Find(m_siCurrentVillageIndex, REALESTATE_TYPE_HOUSE, m_siCurrentHouseIndex );

	if(pclinfo)
	{
		TCHAR* pText = GetTxtFromMgr(2136);

		StringCchPrintf( temp, 1024, pText, pclinfo->clContract.siRentFee );
		StringCchCopy( buffer, 1024, temp );

		pText = GetTxtFromMgr(2137);
		StringCchPrintf( temp, 1024, pText, pclinfo->clContract.siRentPeriod );
		StringCchCat( buffer, 1024, temp );

		if ( pclinfo->clContract.clDate.GetDateText( temp ) == TRUE )
		{
			pText = GetTxtFromMgr(2138);
			StringCchPrintf( temp1, 1024, pText, temp );
			StringCchCat( buffer, 1024, temp1 );
		}

	//	SetDlgItemText(NULL, IDC_EDIT_MY_RETAL_HOUSE_INFO, buffer);
		m_pEdit[0]->SetText(buffer);
	}

	memset( buffer, 0, 1024 );
	TCHAR* pText = GetTxtFromMgr(2136);
	StringCchPrintf( temp, 1024, pText, pclClient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentFee );
	StringCchCopy( buffer, 1024, temp );
	pText = GetTxtFromMgr(2137);
	StringCchPrintf( temp, 1024, pText, pclClient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentPeriod );
	StringCchCat( buffer, 1024, temp );

//	SetDlgItemText(hDlg, IDC_EDIT_HOUSE_RETAL_INFO, buffer);
	m_pEdit[1]->SetText(buffer);
}

void CALLBACK CNHouseUserManagementDlg::StaticCallBackDialogNHouseUserManagementDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHouseUserManagementDlg *pThis = (CNHouseUserManagementDlg *)pControllerMgr;
	pThis->CallBackDialogNHouseUserManagementDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHouseUserManagementDlg::CallBackDialogNHouseUserManagementDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHOUSEUSERMANAGEMENT_DIALOG_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR* pTitle = GetTxtFromMgr(2140);
					TCHAR* pText = GetTxtFromMgr(2139);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 7 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

					//if(MessageBox(NULL, pText, pTitle, MB_YESNO|MB_ICONEXCLAMATION )  == IDYES )
					//{
					//	m_pHouseUnitBuffer->Push( HOUSEUNIT_RETAL_EXTENSION, pclClient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract, m_siCurrentVillageIndex, m_siCurrentHouseIndex, NULL );
					//}
				}
				break;
			}
		}
		break;
	}
}

void CNHouseUserManagementDlg::SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer)
{
	m_pHouseUnitBuffer = pHouseUnitBuffer;
}

void CNHouseUserManagementDlg::SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex)
{
	m_siCurrentVillageIndex = siCurrentVillageIndex;
	m_siCurrentHouseIndex = siCurrentHouseIndex;
}

