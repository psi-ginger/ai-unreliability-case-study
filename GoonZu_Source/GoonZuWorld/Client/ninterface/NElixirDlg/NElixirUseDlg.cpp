#include "NElixirUseDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"
#include "ninterface/NYesOrNoDlg/NYesOrNoDlg.h"

NElixirUseDlg::NElixirUseDlg()
{
	m_siClickItemUnique = 0;
}

NElixirUseDlg::~NElixirUseDlg()
{
	Destroy();	
}

void NElixirUseDlg::Init()
{
	CComboBox*	pCombobox = m_InterfaceFactory.GetComboBox( NELIXIR_USE_COMBO_MULTIPLE );

	stComboBoxItemData stTmpComboBoxItemData;
	for( SI32 siCount = 1; siCount <= cltElixir::LIMIT_MULTIPLE; ++siCount )
	{
		stTmpComboBoxItemData.Init();
		stTmpComboBoxItemData.Set( SI32ToString( siCount ) );

		pCombobox->AddItem( &stTmpComboBoxItemData );
	}

	pCombobox->SetCursel(0); 
	pCombobox->Refresh();
}

void NElixirUseDlg::Destroy()
{


}

void NElixirUseDlg::Create()
{
	if( IsCreate() )
	{
		return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NElixirDlg/NElixirUseDlg.ddf");

	file.CreatePopupDialog( this, NELIXIR_USE_DLG, "dialog_elixirUse" , StatiNElixirUseDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_USE_STATIC_1,		this),  NELIXIR_USE_STATIC_1,		TEXT("statictext_NONAME1")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_USE_STATIC_2,		this),  NELIXIR_USE_STATIC_2,		TEXT("statictext_NONAME2")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_USE_STATIC_3,		this),  NELIXIR_USE_STATIC_3,		TEXT("statictext_NONAME3")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_USE_STATIC_4,		this),  NELIXIR_USE_STATIC_4,		TEXT("statictext_NONAME4")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NELIXIR_USE_STATIC_5,		this),  NELIXIR_USE_STATIC_5,		TEXT("statictext_NONAME5")	);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(COMBOBOX,  	NELIXIR_USE_COMBO_MULTIPLE,	this),  NELIXIR_USE_COMBO_MULTIPLE,	TEXT("combobox_multiple")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_USE_EDIT_POINT_NUM,	this),  NELIXIR_USE_EDIT_POINT_NUM,	TEXT("editbox_point")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NELIXIR_USE_EDIT_WATER_NUM,	this),  NELIXIR_USE_EDIT_WATER_NUM,	TEXT("editbox_water")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	NELIXIR_USE_BUTTON_CANCEL,	this),  NELIXIR_USE_BUTTON_CANCEL,	TEXT("button_cancel")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	NELIXIR_USE_BUTTON_OK,		this),  NELIXIR_USE_BUTTON_OK,		TEXT("button_ok")	);


	Init();

	SetX( 450 ); 
	SetY( 300 );
}

void NElixirUseDlg::Action()
{

}

void CALLBACK NElixirUseDlg::StatiNElixirUseDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NElixirUseDlg *pThis = (NElixirUseDlg *)pControllerMgr;
	pThis->NElixirUseDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NElixirUseDlg::NElixirUseDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NELIXIR_USE_BUTTON_OK:
		{
			if( 0 >= m_siClickItemUnique )	DeleteDialog();

			if( pclClient->pclCM->CR[1]->bElixirSwitch )
			{
				cltClient* pclclient = (cltClient *)pclClient;
				if(NULL == pclclient->m_pDialog[NHELPER_QUESTION_OR_ANSWER_DLG])
				{
					pclclient->CreateInterface( NYESORNODLG );
				}

				CNYesOrNoDlg* pDialog = (CNYesOrNoDlg*)pclclient->m_pDialog[NYESORNODLG];
				if( NULL == pDialog ) return;

				pDialog->SetElixirNew();
			}
			else
			{
				ElixirStart();
			}
		}
		break;

	case NELIXIR_USE_BUTTON_CANCEL:
		{
			DeleteDialog();
		}
		break;


	case NELIXIR_USE_COMBO_MULTIPLE:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{							
					ChangeMultiple();
				}
				break;
			}

		}
		break;


	default:
		{

		}
		break;
	}
}
void NElixirUseDlg::SetItemUnique( SI32 _siClickItemUnique )
{
	m_siClickItemUnique = _siClickItemUnique;
}

void NElixirUseDlg::ChangeMultiple( void )
{
	CComboBox* pCombo = m_InterfaceFactory.GetComboBox( NELIXIR_USE_COMBO_MULTIPLE );
	if( NULL == pCombo )	return;

	int index = pCombo->GetSelectedIndex();
	if( index < 0 ) return;

	TCHAR* szSelText = pCombo->GetText( index );
	if( NULL == szSelText ) return;

	SI32 MultipleValue = _ttoi( szSelText );

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if( NULL == pclChar )	return;

	m_InterfaceFactory.GetEdit( NELIXIR_USE_EDIT_POINT_NUM )->SetText( SI32ToString( MultipleValue * pclChar->pclCI->clIP.GetLevel() )		);
	m_InterfaceFactory.GetEdit( NELIXIR_USE_EDIT_WATER_NUM )->SetText( SI32ToString( MultipleValue * pclChar->pclCI->clIP.GetLevel() * cltElixir::WATER_MILTIPLE ) );
}

void NElixirUseDlg::ElixirStart( void )
{
	cltPItemInfo* pInvenItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;
	SI32 siElixirItemPos = 0;
	SI32 siWaterItemPos  = 0;
	SI32 siWaterItemNum  = 0;
	for( SI32 siPos = PERSONITEM_INV0; siPos < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; siPos++ )
	{			
		if( ITEMUNIQUE(m_siClickItemUnique) == pInvenItemInfo->clItem[siPos].siUnique )
		{
			siElixirItemPos = siPos;
		}

		if( ITEMUNIQUE(2789) == pInvenItemInfo->clItem[siPos].siUnique )
		{
			// 아이템이 분할되 있을지도 모르니까 개수가 많은 위치를 찾는다.
			if( siWaterItemNum < pInvenItemInfo->clItem[siPos].siItemNum )
			{
				siWaterItemNum = pInvenItemInfo->clItem[siPos].siItemNum;
				siWaterItemPos = siPos;
			}
		}
	}

	// 아이템이 없으면 안되.
	if( 0 == siElixirItemPos ) return;

	if( 0 == siWaterItemPos || 0 == siWaterItemNum )	
	{
		cltClient* pclclient = (cltClient*) pclClient;
		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(9750) );
		return;
	}

	// 마력의 비약 소유량과 사용할 개수 비교.
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if( NULL == pclChar )	return;

	CComboBox* pCombo = m_InterfaceFactory.GetComboBox( NELIXIR_USE_COMBO_MULTIPLE );
	if( NULL == pCombo )	return;

	int index = pCombo->GetSelectedIndex();
	if( index < 0 ) return;

	TCHAR* szSelText = pCombo->GetText( index );
	if( NULL == szSelText ) return;

	UI08 uiMultipleValue = (UI08)_ttoi( szSelText );	
	if( 0 > uiMultipleValue || cltElixir::LIMIT_MULTIPLE < uiMultipleValue )	return;

	SI32	siNPCPriceRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate   ? pclClient->pclItemManager->clItemStatistic.siNPCPriceRate   : 1;

	// 마력의 비약 사용량을 클라와 비교한다.
	SI32 UseWaterNum = min( 10000, uiMultipleValue * pclChar->pclCI->clIP.GetLevel() * cltElixir::WATER_MILTIPLE );

	if( siWaterItemNum < UseWaterNum ) 
	{
		cltClient* pclclient = (cltClient*) pclClient;
		pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(9750) );
		return;
	}

	cltElixir_Info	cltElixirInfo; 
	cltElixirInfo.m_uiMultiple = uiMultipleValue;

	// 서버에 엘릭서 사용 배수를 던져 주고 엘릭서 사용을 시작한다.
	cltGameMsgRequest_Elixir_Set clInfo(	&cltElixirInfo,
											siElixirItemPos, 
											pInvenItemInfo->clItem[siElixirItemPos].siUnique,
											siWaterItemPos, 
											pInvenItemInfo->clItem[siWaterItemPos].siUnique,
											UseWaterNum,
											true				);

	cltMsg clMsg( GAMEMSG_REQUEST_ELIXIR_SET, sizeof(clInfo), (BYTE*)&clInfo );
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg); 

}




void NElixirUseDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}



