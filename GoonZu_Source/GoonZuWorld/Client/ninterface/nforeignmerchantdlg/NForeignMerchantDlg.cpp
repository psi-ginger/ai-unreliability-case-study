#include "../Client/NInterface/NForeignMerchantDlg/NForeignMerchantDlg.h"

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

#include "../../Client.h"

#include "../CommonLogic/TextMgr/TextMgr.h"
#include "../Client/NInterface/NForeignMerchantDlg/NForeignMerchantBuyItemDlg.h"
#include "../Client/NInterface/NForeignMerchantDlg/NForeignMerchantBuyCashItemDlg.h"

extern cltCommonLogic* pclClient;


CNForeignMerchantDlg::CNForeignMerchantDlg() : CDialogController()
{
	m_siNPCCharUnique = 0;

	m_pStatic			  = NULL;
	m_pEdit				  = NULL;
	m_pButtonNomalItemBuy = NULL;
	m_pButtonCashItemBuy  = NULL;
}

CNForeignMerchantDlg::~CNForeignMerchantDlg()
{
	SAFE_DELETE(m_pStatic);
	SAFE_DELETE(m_pEdit);
	SAFE_DELETE(m_pButtonNomalItemBuy);
	SAFE_DELETE(m_pButtonCashItemBuy);
}

void CNForeignMerchantDlg::Create()
{
	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NForeignMerchantDlg/DLG_ForeignMerchant.ddf"));
		file.CreatePopupDialog( this, NFOREIGNMERCHANT_DLG, TEXT("dialog_Foreign_Merchant"), StaticCallBackDialogNForeignMerchantDlg );

		m_pStatic			  = new CStatic( this );
		m_pEdit				  = new CEdit( this );
		m_pButtonNomalItemBuy = new CButton( this );
		m_pButtonCashItemBuy  = new CButton( this );

		file.CreateControl( m_pStatic, NFOREIGNMERCHANT_DIALOG_STATIC, TEXT("statictext_NONAME1") );
		m_pStatic->SetFontIndex( pclClient->GetUniqueFromHash(TEXT("KIND_CHINAMERCHANT")) - 1);

		file.CreateControl( m_pEdit, NFOREIGNMERCHANT_DIALOG_EDIT, TEXT("editbox_NONAME1") );
		file.CreateControl( m_pButtonNomalItemBuy, NFOREIGNMERCHANT_DIALOG_BUTTON1, TEXT("button_NONAME1") );
		file.CreateControl( m_pButtonCashItemBuy, NFOREIGNMERCHANT_DIALOG_BUTTON2, TEXT("button_NONAME2") );

		//if ( pclClient->siServiceArea != ConstServiceArea_Korea) 
		//	m_pButtonCashItemBuy->Show(false);
		/*else */m_pButtonCashItemBuy->Show(true);

		TCHAR* pText = GetTxtFromMgr(3226);
		m_pEdit->SetText(pText);
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNForeignMerchantDlg::StaticCallBackDialogNForeignMerchantDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNForeignMerchantDlg *pThis = (CNForeignMerchantDlg *)pControllerMgr;
	pThis->CallBackDialogNForeignMerchantDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNForeignMerchantDlg::CallBackDialogNForeignMerchantDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NFOREIGNMERCHANT_DIALOG_BUTTON1:	// 일반 물품 팔기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclclient->CreateInterface( NFOREIGNMERCHANTBUYITEM_DLG );
					((CNForeignMerchantBuyItemDlg*)pclclient->m_pDialog[ NFOREIGNMERCHANTBUYITEM_DLG ])->SetNormalItemBuyMode(m_siNPCCharUnique);
					DeleteDialog();
				}
				break;
			}
		}
		break;

	case NFOREIGNMERCHANT_DIALOG_BUTTON2:	// 캐쉬 아이템 팔기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					pclclient->CreateInterface( NFOREIGNMERCHANTBUYCASHITEM_DLG );
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

void CNForeignMerchantDlg::SetNPCCharUnique(SI32 NPCCharUnique)
{
	m_siNPCCharUnique = NPCCharUnique;
}