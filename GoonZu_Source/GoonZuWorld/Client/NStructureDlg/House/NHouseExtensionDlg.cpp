#include "client.h"


#include "NHouseExtensionDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "Char\CharManager\CharManager.h"

#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"

#include "../CommonLogic/MsgType-RealEstate.h"
#include "../CommonLogic/House/House.h"

extern cltCommonLogic* pclClient;

CNHouseExtensionDlg::CNHouseExtensionDlg()
{
	m_pYesButton = NULL;
	m_pNoButton = NULL;
	m_pContractStatic = NULL;
	m_pTextStatic = NULL;
	m_pContractStaticLine = NULL;
	m_pTextStaticLine = NULL;
	m_siVillageUnique = 0;
	m_siHouseUnitIndex= 0 ;
}
CNHouseExtensionDlg::~CNHouseExtensionDlg()
{
	SAFE_DELETE(m_pYesButton);
	SAFE_DELETE(m_pNoButton);
	SAFE_DELETE(m_pContractStatic);
	SAFE_DELETE(m_pTextStatic);
	SAFE_DELETE(m_pContractStaticLine);
	SAFE_DELETE(m_pTextStaticLine);
}

void CNHouseExtensionDlg::Destroy()
{
	DeleteDialog();
}

void CNHouseExtensionDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	if ( !IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile("NInterface/Data/NStructureDlg/DLG_NHouseExtensionDlg.ddf");
	
		NEWCBUTTON(m_pYesButton);
		NEWCBUTTON(m_pNoButton);
		NEWCSTATIC(m_pContractStatic);
		NEWCSTATIC(m_pTextStatic);
		NEWCSTATIC(m_pContractStaticLine);
		NEWCSTATIC(m_pTextStaticLine);
	
		file.CreatePopupDialog( this, NHOUSEEXTENSION_DLG, "dialog_HouseExtension", StaticCNHouseExtensionProc);

		file.CreateControl( m_pYesButton, NHOUSEEXTENSION_BUTTON_YES,   "button_Yes"); 
		file.CreateControl( m_pNoButton, NHOUSEEXTENSION_BUTTON_NO,   "button_No"); 
		file.CreateControl( m_pContractStatic, NHOUSEEXTENSION_STATIC_CONTRACT,   "statictext_Contract"); 
		file.CreateControl( m_pTextStatic, NHOUSEEXTENSION_STATIC_TEXT,   "statictext_Text");
		file.CreateControl( m_pContractStaticLine, NHOUSEEXTENSION_STATIC_CONTRACTLINE,   "statictext_Contract_Line"); 
		file.CreateControl( m_pTextStaticLine, NHOUSEEXTENSION_STATIC_TEXTLINE,   "statictext_Text_Line");
	}

	m_pTextStatic->SetText(GetTxtFromMgr(40371));

	return;
}

void CALLBACK CNHouseExtensionDlg::StaticCNHouseExtensionProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNHouseExtensionDlg *pThis = (CNHouseExtensionDlg*)pControllerMgr;
	if( NULL != pThis)
	{
		pThis->NHouseExtensionDlgProc( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNHouseExtensionDlg::NHouseExtensionDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;
	
	switch( nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
			break;	
		};
	case NHOUSEEXTENSION_BUTTON_NO:
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
	case NHOUSEEXTENSION_BUTTON_YES:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 필요 자격 조건을 충족시키고 있는지 확인한다. 
					if(pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() < LEVELLIMIT_USE_HOUSE)
					{
						pclclient->PushEffect(pclclient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
						TCHAR* pTitle = GetTxtFromMgr(1050);
						TCHAR* pText = GetTxtFromMgr(1051);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, LEVELLIMIT_USE_HOUSE);
						break;
					}
					
					// 게임서버로 계약 연장 요청을 보낸다. 
					cltGameMsgRequest_ExtendContract clinfo(m_siVillageUnique, REALESTATE_TYPE_HOUSE, m_siHouseUnitIndex, &pclclient->pclHouseManager->clClientStrInfo.clHouseStrInfo.clRentContract);

					cltMsg clMsg(GAMEMSG_REQUEST_EXTENDCONTRACT,
						sizeof(clinfo), (BYTE*)&clinfo);

					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

					DeleteDialog();
					break;
				}
			}
			break;	
		}
	}
	return;
}

// 임대 정보 
VOID CNHouseExtensionDlg::SetExtensionInfoText(TCHAR* pszText)
{
	m_pContractStatic->SetText(pszText);
}

VOID CNHouseExtensionDlg::SetVillageUnique(SI32 siVillageUnique)
{
	m_siVillageUnique = siVillageUnique;
}
VOID CNHouseExtensionDlg::SetHouseUnitIndex(SI32 siHouseUnitIndex)
{
	m_siHouseUnitIndex = siHouseUnitIndex;
}
