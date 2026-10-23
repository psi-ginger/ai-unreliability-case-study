#include "../Client/NStructureDlg/House/UserInfo/NHouseUserNewManagementDlg.h"

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

#include "../NHouseNameChangeDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

CNHouseUserNewManagementDlg::CNHouseUserNewManagementDlg() : CDialogController()
{
	m_siCurrentVillageIndex = 0;
	m_siCurrentHouseIndex = -1;

	m_pList = NULL;
	m_pExtensionButton = NULL;
	m_pChangeNameButton = NULL;
	m_pInfoTextStatic = NULL;
	m_pInfoEdit = NULL;
}

CNHouseUserNewManagementDlg::~CNHouseUserNewManagementDlg()
{
	SAFE_DELETE(m_pList);
	SAFE_DELETE(m_pExtensionButton);
	SAFE_DELETE(m_pChangeNameButton);
	SAFE_DELETE(m_pInfoTextStatic);
	SAFE_DELETE(m_pInfoEdit);
}

void CNHouseUserNewManagementDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NHouseUserInfoDlg/DLG_HouseUserNewManagement.ddf"));
	file.CreateChildDialog( this, NHOUSEUSERNEWMANAGEMENT_DLG, TEXT("dialog_HouseUserNewManagement"), StaticCallBackDialogNHouseUserNewManagementDlg, pParent );

	NEWCLIST(m_pList);
	NEWCBUTTON(m_pExtensionButton);
	NEWCBUTTON(m_pChangeNameButton);
	NEWCSTATIC(m_pInfoTextStatic);
	NEWCEDIT(m_pInfoEdit);

	file.CreateControl( m_pList, NHOUSEUSERNEWMANAGEMENT_DIALOG_LIST, TEXT("listbox_MyHouse") );
	file.CreateControl( m_pExtensionButton, NHOUSEUSERNEWMANAGEMENT_DIALOG_BUTTON_EXTENSION, TEXT("button_Extension") );
	file.CreateControl( m_pChangeNameButton, NHOUSEUSERNEWMANAGEMENTT_DIALOG_BUTTON_CHANGENAME, TEXT("button_ChangeName") );
	file.CreateControl( m_pInfoTextStatic, NHOUSEUSERNEWMANAGEMENTT_DIALOG_STATIC_INFOTEXT, TEXT("statictext_InfoText") );
	file.CreateControl( m_pInfoEdit, NHOUSEUSERNEWMANAGEMENTT_DIALOG_EDIT_INFO, TEXT("editbox_Info") );

	Hide();

	m_pList->SetBorder(true);
	m_pList->SetBKColorUse(true);

	m_pList->SetColumn(0 , 35, GetTxtFromMgr(1641));
	m_pList->SetColumn(1 , 100, GetTxtFromMgr(40372));
	m_pList->SetColumn(2 , 35, GetTxtFromMgr(40778));
	m_pList->SetColumn(3 , 60, GetTxtFromMgr(4333));
	m_pList->SetColumn(4 , 115, GetTxtFromMgr(40373));
	
	m_pList->Refresh();

	m_pExtensionButton->Enable(FALSE);
	m_pChangeNameButton->Enable(FALSE);
}

void CNHouseUserNewManagementDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHouseUserNewManagementDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNHouseUserNewManagementDlg::Action()
{
	TCHAR szBuffer[MAX_PATH] = TEXT("");
	TCHAR szTemp[MAX_PATH] = TEXT("");

	TCHAR* pText = GetTxtFromMgr(2136);
	StringCchPrintf( szTemp, MAX_PATH, pText, pclClient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentFee );
	StringCchCopy( szBuffer, MAX_PATH, szTemp );
	pText = GetTxtFromMgr(2137);
	StringCchPrintf( szTemp, MAX_PATH, pText, pclClient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract.siRentPeriod );
	StringCchCat( szBuffer, MAX_PATH, szTemp );

	m_pInfoEdit->SetText(szBuffer);
}

void CNHouseUserNewManagementDlg::LoadMyHouseList()
{
	TCHAR szRentFee[MAX_PATH] = TEXT("");
	TCHAR szRendPeriod[MAX_PATH] = TEXT("");
	TCHAR szNum[MAX_PATH] = TEXT("");
	TCHAR szExpirationDay[MAX_PATH] = TEXT("");
	cltRentContract* pclRentContract = NULL;

	stListBoxItemData stItemData;

	m_pList->Clear();

	for(SI32 i = 0; i < MAX_HOUSEUNIT_NUMBER; i++ )
	{
		if(pclClient->pclHouseManager->clClientUnit[i].clPerson.GetPersonID() == pclClient->pclCM->CR[1]->pclCI->GetPersonID())
		{
			pclRentContract = &pclClient->pclHouseManager->clClientUnit[i].clContract;

			if(NULL == pclRentContract)
				continue;

			stItemData.Init();
			stItemData.siParam[0] = i;

			StringCchPrintf(szNum, MAX_PATH, TEXT("%d"), i+1);
			StringCchPrintf(szRendPeriod, MAX_PATH, TEXT("%d"), pclRentContract->siRentPeriod);
			StringCchCat(szRendPeriod, MAX_PATH, GetTxtFromMgr(4082));
			StringCchPrintf(szRentFee, MAX_PATH, TEXT("%d"), pclRentContract->siRentFee);
			StringCchPrintf(szExpirationDay, MAX_PATH, GetTxtFromMgr(3808), pclRentContract->clDate.uiYear + pclRentContract->siRentPeriod
				, pclRentContract->clDate.uiMonth
				, pclRentContract->clDate.uiDay
				, pclRentContract->clDate.uiHour);

			stItemData.Set(0 , szNum);
			stItemData.Set(1 , pclClient->pclHouseManager->clClientUnit[i].m_szName);
			stItemData.Set(2 , szRendPeriod);
			stItemData.Set(3 , szRentFee);
			stItemData.Set(4 , szExpirationDay);

			m_pList->AddItem(&stItemData);
		}
	}

	m_pList->Refresh();

	m_pExtensionButton->Enable(FALSE);
	m_pChangeNameButton->Enable(FALSE);
}

void CALLBACK CNHouseUserNewManagementDlg::StaticCallBackDialogNHouseUserNewManagementDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHouseUserNewManagementDlg *pThis = (CNHouseUserNewManagementDlg *)pControllerMgr;
	pThis->CallBackDialogNHouseUserNewManagementDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHouseUserNewManagementDlg::CallBackDialogNHouseUserNewManagementDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHOUSEUSERNEWMANAGEMENT_DIALOG_LIST:
		{
			switch( nEvent )
			{
				case EVENT_LISTBOX_SELECTION:
				{			
					SI32 siIndex = m_pList->GetSelectedIndex();
					if( siIndex < 0 )
					{
						m_pExtensionButton->Enable(FALSE);
						m_pChangeNameButton->Enable(FALSE);
						return;
					}
					else
					{
						m_pExtensionButton->Enable(TRUE);
						m_pChangeNameButton->Enable(TRUE);
					}
					stListBoxItemData *pItemData = m_pList->GetItemData( siIndex, 0 );
					SetHousePositionInfo(pclClient->pclHouseManager->siVillageUnique, pItemData->siParam[0]);
					break;
				}
			}	
			break;
		}
	case NHOUSEUSERNEWMANAGEMENT_DIALOG_BUTTON_EXTENSION:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_siCurrentVillageIndex <= 0)
						break;
					if( m_siCurrentHouseIndex < 0)
						break;

					SI32 siIndex = m_pList->GetSelectedIndex();
					if( siIndex < 0 ) return;

					TCHAR* pTitle = GetTxtFromMgr(2140);
					TCHAR* pText = GetTxtFromMgr(2139);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 7 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			}
			break;
		}
	case NHOUSEUSERNEWMANAGEMENTT_DIALOG_BUTTON_CHANGENAME:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_siCurrentVillageIndex <= 0)
						break;
					if( m_siCurrentHouseIndex < 0)
						break;

					SI32 siIndex = m_pList->GetSelectedIndex();

					if( siIndex < 0 ) return;

					stListBoxItemData *pItemData = m_pList->GetItemData( siIndex, 0 );

					TCHAR* pText = pItemData->strText[1]; 

					if(pclclient->m_pDialog[NHOUSENAMECHANGE_DLG ] != NULL)
						((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->DeleteDialog();

					pclclient->CreateInterface(NHOUSENAMECHANGE_DLG);
					if(pclclient->m_pDialog[NHOUSENAMECHANGE_DLG ] != NULL)
					{
						((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->SetVillageUnique(m_siCurrentVillageIndex);
						((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->SetHouseUnitIndex(m_siCurrentHouseIndex);
						((CNHouseNameChangeDlg*)pclclient->m_pDialog[NHOUSENAMECHANGE_DLG])->SetOldHouseName(pText);
					}
				}
				break;
			}
			break;
		}
	}
}

void CNHouseUserNewManagementDlg::SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer)
{
	m_pHouseUnitBuffer = pHouseUnitBuffer;
}

void CNHouseUserNewManagementDlg::SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex)
{
	m_siCurrentVillageIndex = siCurrentVillageIndex;
	m_siCurrentHouseIndex = siCurrentHouseIndex;
}

