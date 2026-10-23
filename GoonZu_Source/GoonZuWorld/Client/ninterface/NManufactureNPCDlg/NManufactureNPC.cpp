#include "NManufactureNPC.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "../../../Common/Char/CharManager/CharManager.h"

#include "MsgType-System.h"
#include "Msg/MsgType-NPC.h"
#include "MsgType.h"

extern cltCommonLogic* pclClient;

CNManufactureNPCDlg::CNManufactureNPCDlg()
{
	Init();
}

CNManufactureNPCDlg::~CNManufactureNPCDlg()
{
	Destroy();
}

void CNManufactureNPCDlg::Init()
{

}

void CNManufactureNPCDlg::Destroy()
{

}

void CNManufactureNPCDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NManufactureNPC/DLG_NManufactureNPC.ddf"));
	file.CreatePopupDialog( this, NMANUFACTURENPC_DLG, TEXT("dialog_manufacturenpc"),StaticManufactureNPCDlgProc );

    // 컨트롤 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMANUFACTURENPC_STATIC_NPCIMAGE, this) ,NMANUFACTURENPC_STATIC_NPCIMAGE, TEXT("statictext_npcimage"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMANUFACTURENPC_EDIT_EXPLAIN, this)  , NMANUFACTURENPC_EDIT_EXPLAIN , TEXT("editbox_explain"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NMANUFACTURENPC_IMAGE_ITEMBACK, this), NMANUFACTURENPC_IMAGE_ITEM, TEXT("imagestatic_item_back"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NMANUFACTURENPC_IMAGE_ITEM, this), NMANUFACTURENPC_IMAGE_ITEM, TEXT("imagestatic_item"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURENPC_STATIC_ITEMNAME, this), NMANUFACTURENPC_STATIC_ITEMNAME, TEXT("statictext_itemname"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURENPC_STATIC_NONAME1, this), NMANUFACTURENPC_STATIC_NONAME1, TEXT("statictext_NONAME1"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, NMANUFACTURENPC_EDIT_REGISTPAY, this), NMANUFACTURENPC_EDIT_REGISTPAY, TEXT("editbox_regist_pay"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURENPC_STATIC_NONAME2, this), NMANUFACTURENPC_STATIC_NONAME2, TEXT("statictext_NONAME2"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURENPC_BUTTON_REGISTCANCEL, this), NMANUFACTURENPC_BUTTON_REGISTCANCEL, TEXT("button_regist_cancel"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURENPC_STATIC_NONAME3, this), NMANUFACTURENPC_STATIC_NONAME3, TEXT("statictext_NONAME3"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NMANUFACTURENPC_STATIC_NONAME4, this), NMANUFACTURENPC_STATIC_NONAME4, TEXT("statictext_NONAME4"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX, NMANUFACTURENPC_COMBO_ITEMNAME, this), NMANUFACTURENPC_COMBO_ITEMNAME, TEXT("combobox_itemname"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURENPC_BUTTON_REGIST, this), NMANUFACTURENPC_BUTTON_REGIST, TEXT("button_regist"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURENPC_BUTTON_REQUESTSTART, this), NMANUFACTURENPC_BUTTON_REQUESTSTART, TEXT("button_request_start"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURENPC_BUTTON_REQUESTCANCEL, this), NMANUFACTURENPC_BUTTON_REQUESTCANCEL, TEXT("button_request_cancel"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NMANUFACTURENPC_BUTTON_CLOSE, this), NMANUFACTURENPC_BUTTON_CLOSE, TEXT("button_close"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NMANUFACTURENPC_OUTLINE_NONAME1, this), NMANUFACTURENPC_OUTLINE_NONAME1, TEXT("outline_NONAME1"));


	CStatic* pStatic = m_InterfaceFactory.GetStatic(NMANUFACTURENPC_STATIC_NPCIMAGE);
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_GUYAJA")), szPortrait, sizeof(szPortrait), &siPortraitFont);
	pStatic->SetFileName(szPortrait);
	pStatic->SetFontIndex(siPortraitFont);

	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

	if( pclchar->pclCI->clIP.GetLevel() <= 30 )
	{
		TCHAR* pText = GetTxtFromMgr(6854);
		SetExplain( pText );
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(6855);
		SetExplain( pText );

		m_InterfaceFactory.GetButton(NMANUFACTURENPC_BUTTON_REGISTCANCEL)->Enable( false );
		m_InterfaceFactory.GetButton(NMANUFACTURENPC_BUTTON_REGIST)->Enable( false );
		m_InterfaceFactory.GetComboBox(NMANUFACTURENPC_COMBO_ITEMNAME)->Enable( false );
	}

	m_InterfaceFactory.GetButton(NMANUFACTURENPC_BUTTON_REQUESTSTART)->Enable( false );
	m_InterfaceFactory.GetButton(NMANUFACTURENPC_BUTTON_REQUESTCANCEL)->Enable( false );


	SetMakeableItemList();

}

void CALLBACK CNManufactureNPCDlg::StaticManufactureNPCDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNManufactureNPCDlg *pThis = (CNManufactureNPCDlg*) pControllerMgr;
	pThis->NManufactureNPCDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNManufactureNPCDlg::NManufactureNPCDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NMANUFACTURENPC_BUTTON_CLOSE:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				DeleteDialog();
			}
			break;
		}
	case NMANUFACTURENPC_BUTTON_REGIST:
		{
            if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				// 등록!
				RegistItem();
			}
		}
		break;
	case NMANUFACTURENPC_BUTTON_REGISTCANCEL:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				// 등록 취소
				CancelRegistItem();
			}
		}
		break;
	case NMANUFACTURENPC_BUTTON_REQUESTSTART:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
                // 제조 시작
				RequestStart();
			}
		}
		break;
	case NMANUFACTURENPC_BUTTON_REQUESTCANCEL:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				// 제조 취소
				RequestStop();
			}
		}
	default:
		break;
	}
}

void CNManufactureNPCDlg::SetMakeableItemList()
{
	stComboBoxItemData tmpComboBoxItemData;

	// 콤보 박스를 지운다.
	CComboBox * pComboBox =	m_InterfaceFactory.GetComboBox(NMANUFACTURENPC_COMBO_ITEMNAME);

	pComboBox->Clear();


	SI32 siMakeableItemNum = 0;
	SI32* pMakeableItemList = NULL;
	SI32 id = 1;


	// 아이템 종류를 보여준다. 

	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	if(pclitemmanager->FindMakeableItem(
		NULL, 
		&pclClient->pclCM->CR[id]->pclCI->clCharItem,
		NULL,
		MAX_INVITEM_PER_PERSON,
		&pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[PERSONITEM_INV0],
		&siMakeableItemNum,
		&pMakeableItemList,
		true) == true)
	{
		TCHAR buffer[256];
		for(SI32 i = 0; i < siMakeableItemNum; ++i)
		{
			// 무기, 갑옷만..
			if  ( pclitemmanager->CheckCanPutonItem(pMakeableItemList[i*2]) == false )
				continue;

			// 30레벨이 넘는 장비는 보여주지 않는다.
			SI32 ref = pclitemmanager->FindItemRefFromUnique( pMakeableItemList[i*2] );
			SI32 equiplevel = pclitemmanager->pclItemInfo[ref]->clCondition.siLevel;
			if ( equiplevel > 30 )
				continue;


			TCHAR* pText = GetTxtFromMgr(2246);
			StringCchPrintf(buffer, 256, pText, pclitemmanager->GetName(pMakeableItemList[i*2]), pMakeableItemList[i*2 + 1]);
			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( pMakeableItemList[i*2], buffer );
			pComboBox->AddItem( &tmpComboBoxItemData );
			//SendMessage( hCombobox, CB_ADDSTRING, 0, (LPARAM)buffer );
		}

	}

	pComboBox->Refresh();

	if(pMakeableItemList)
	{
		delete []pMakeableItemList;
		pMakeableItemList = NULL;
	}
}

void CNManufactureNPCDlg::RegistItem()
{
	CComboBox* pCombo = m_InterfaceFactory.GetComboBox(NMANUFACTURENPC_COMBO_ITEMNAME);

	if ( pCombo == NULL ) return;
    
	SI32 selectedindex = pCombo->GetSelectedIndex();
	if ( selectedindex < -1 ) return;

	SI32 itemUnique = pCombo->GetParam( selectedindex );
	m_siManufactureItemUnique = itemUnique;

	RequestSelectedItemPrice( itemUnique );



	TCHAR* itemName = (TCHAR*)pclClient->pclItemManager->GetName( itemUnique );

	CStatic* pStatic = m_InterfaceFactory.GetStatic( NMANUFACTURENPC_STATIC_ITEMNAME );
	if( pStatic && itemName )
	{
		pStatic->SetText( itemName );
	}

	SetItemImg( itemUnique );

	CButton* pButton = m_InterfaceFactory.GetButton( NMANUFACTURENPC_BUTTON_REGIST );
	if ( pButton )
	{
		pButton->Enable(false);
	}

	pButton = m_InterfaceFactory.GetButton( NMANUFACTURENPC_BUTTON_REQUESTSTART );
	if( pButton )
	{
        pButton->Enable( true );	
	}
}
void CNManufactureNPCDlg::CancelRegistItem()
{
	SetItemImg( 0 );

	CStatic* pStatic = m_InterfaceFactory.GetStatic( NMANUFACTURENPC_STATIC_ITEMNAME );
	if( pStatic )
		pStatic->SetText( TEXT("") );

	CEdit* pEdit = m_InterfaceFactory.GetEdit( NMANUFACTURENPC_EDIT_REGISTPAY );
	if ( pEdit )
		pEdit->SetText( TEXT("") );

	CButton* pButton = m_InterfaceFactory.GetButton( NMANUFACTURENPC_BUTTON_REGIST );
	if ( pButton )
	{
		pButton->Enable(true);
	}

	pButton = m_InterfaceFactory.GetButton( NMANUFACTURENPC_BUTTON_REQUESTSTART );
	if( pButton )
	{
		pButton->Enable( false );	
	}

	m_siManufactureItemUnique = 0 ;

}
void CNManufactureNPCDlg::RequestStart()
{
	if ( m_siManufactureItemUnique == 0 ) return;

	cltGameMsgRequest_ManufactureNPC clinfo( m_siManufactureItemUnique );
	cltMsg clMsg( GAMEMSG_REQUEST_MANUFACTURENPC, sizeof(clinfo), (BYTE*)&clinfo );
	((cltClient*)pclClient)->SendMsgToServer((sPacketHeader*)&clMsg);
}
void CNManufactureNPCDlg::RequestStop()
{

}
void CNManufactureNPCDlg::SetItemImg(SI32 unique )
{
	CImageStatic* pImage = m_InterfaceFactory.GetImageStatic( NMANUFACTURENPC_IMAGE_ITEM );


	if ( pImage )
	{
		if ( unique == 0 )
		{
			pImage->Show(false);
			return;
		}

		SI32 itemtype = 0;
		SI32 fontindex = -1;

		pclClient->pclItemManager->GetItemGImageFileIndexAndFont( unique, &itemtype, &fontindex );

		pImage->SetItemSprType( itemtype );
		pImage->SetFontIndex( fontindex );
		pImage->Show( true );
	}
}
void CNManufactureNPCDlg::RequestSelectedItemPrice( SI32 unique )
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGameMsgRequest_ProductPrice clinfo( unique, this->GetDialogControllerID() );
	cltMsg clMsg( GAMEMSG_REQUEST_PRODUCTPRICE, sizeof(clinfo), (BYTE*)&clinfo );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
}
void CNManufactureNPCDlg::SetMenuFactureFee(GMONEY price )
{
	TCHAR buffer[64];
	StringCchPrintf( buffer, 64, TEXT("%d"), price / 10 );

	CEdit* pEdit = m_InterfaceFactory.GetEdit( NMANUFACTURENPC_EDIT_REGISTPAY );
	if ( pEdit )
	{
		pEdit->SetText( buffer );
	}
}
void CNManufactureNPCDlg::SetExplain(TCHAR* text )
{
	CEdit* pEdit = m_InterfaceFactory.GetEdit(NMANUFACTURENPC_EDIT_EXPLAIN);
	
	if ( pEdit )
	{
		pEdit->SetText( text );
	}
}