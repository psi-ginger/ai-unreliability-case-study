#include "../Client/NStructureDlg/House/UserInfo/NHouseUserSecondhandDlg.h"

#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/Interface/List.h"
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

CNHouseUserSecondhandDlg::CNHouseUserSecondhandDlg() : CDialogController()
{
	m_siCurrentVillageIndex = 0;
    m_siCurrentHouseIndex = 0;
	m_siUsedGoodsItemSelectedIndex = -1;
	m_siUsedGoodsItemRealSelectedIndex = -1;

	m_pclHouseUnit = NULL;

	m_pEdit[0] = NULL;
	m_pEdit[1] = NULL;

	m_pList = NULL;

	m_pButton[0] = NULL;	
	m_pButton[1] = NULL;
}

CNHouseUserSecondhandDlg::~CNHouseUserSecondhandDlg()
{
	SAFE_DELETE(m_pEdit[0]);
	SAFE_DELETE(m_pEdit[1]);

	SAFE_DELETE(m_pList);

	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);

}

void CNHouseUserSecondhandDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/NHouseUserInfoDlg/DLG_HouseUserSecondhand.ddf"));
	file.CreateChildDialog( this, NHOUSEUSERSECONDHAND_DLG, TEXT("dialog_HouseUserSecondhand"), StaticCallBackDialogNHouseUserSecondhandDlg, pParent );

	NEWCEDIT(m_pEdit[0]);
	NEWCEDIT(m_pEdit[1]);

	NEWCLIST(m_pList);

	NEWCBUTTON(m_pButton[0]);
	NEWCBUTTON(m_pButton[1]);

	file.CreateControl( m_pEdit[0], NHOUSEUSERSECONDHAND_DIALOG_EDIT1, TEXT("editbox_NONAME1") );
	file.CreateControl( m_pEdit[1], NHOUSEUSERSECONDHAND_DIALOG_EDIT2, TEXT("editbox_NONAME2") );
	
	TCHAR* pText = GetTxtFromMgr(3272);
	m_pEdit[0]->SetText(pText);

	file.CreateControl( m_pList, NHOUSEUSERSECONDHAND_DIALOG_LIST, TEXT("listbox_NONAME1") );
	pText = GetTxtFromMgr(1195);
	m_pList->SetColumn(0, 100, pText);
	pText = GetTxtFromMgr(598); //내구력
	m_pList->SetColumn(1, 60, pText);
	m_pList->SetColumn(2, 16, TEXT(""));
	m_pList->SetBKColorUse(TRUE);
	m_pList->SetBorder(TRUE);
	m_pList->Refresh();


	file.CreateControl( m_pButton[0], NHOUSEUSERSECONDHAND_DIALOG_BUTTON1, TEXT("button_NONAME1") );
	file.CreateControl( m_pButton[1], NHOUSEUSERSECONDHAND_DIALOG_BUTTON2, TEXT("button_NONAME2") );

	Hide();
}

void CNHouseUserSecondhandDlg::Hide()
{
	CControllerMgr::Show( false );

}

void CNHouseUserSecondhandDlg::Show()
{
	CControllerMgr::Show( true );
}

void CNHouseUserSecondhandDlg::Action()
{
	cltHouseUnitStorageInfo *pStgInfo = NULL;

	if(m_pclHouseUnit)
	{
		pStgInfo = &m_pclHouseUnit->clStorageInfo;
	}
	
	if ( pStgInfo == NULL )
	{
		return;
	}

	if ( ! pStgInfo->bCreatedHouseUnitUsedGoodsBuilding )
	{
		return;
	}			

	if ( IsWareHouseInventoryChanged() )
	{
		LoadWareHouseInventory1();
	}
}

void CALLBACK CNHouseUserSecondhandDlg::StaticCallBackDialogNHouseUserSecondhandDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHouseUserSecondhandDlg *pThis = (CNHouseUserSecondhandDlg *)pControllerMgr;
	pThis->CallBackDialogNHouseUserSecondhandDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHouseUserSecondhandDlg::CallBackDialogNHouseUserSecondhandDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;
	TCHAR Nbuffer[512]=TEXT(" ");

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NHOUSEUSERSECONDHAND_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 TempListIndex = m_pList->GetSelectedIndex();

					if(TempListIndex < 0)
						return;

					m_siUsedGoodsItemSelectedIndex = TempListIndex;

					for ( int i = 0; i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i )
					{
						if ( m_clWareHouseItem[ i ].siUnique > 0 && 
							( pclClient->pclItemManager->GetItemAtb( m_clWareHouseItem[ i ].siUnique ) & ITEMATB_OBJECT ) &&
							( m_clWareHouseItem[ i ].GetMaxDurability() <= DUR_FOR_DISMANTLE ||
							  pclclient->pclCM->CR[1]->GetGameMasterLevel() > BASIC_GAMEMASTER_LEVEL )
							)
						{
							stListBoxItemData *pItemData = m_pList->GetItemData( m_siUsedGoodsItemSelectedIndex, 0 );
						    m_siUsedGoodsItemRealSelectedIndex = pItemData->siParam[0];

							if ( m_siUsedGoodsItemRealSelectedIndex >= 0 && m_siUsedGoodsItemRealSelectedIndex < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER )
							{
								cltGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo clGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo( &m_clWareHouseItem[ m_siUsedGoodsItemRealSelectedIndex ] );
								cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODSINFO, sizeof(clGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo), (BYTE*)&clGameMsgRequest_HouseUnitMakeItemFromUsedGoodsInfo );
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
							}
							break;
						}
					}	
				}
				break;
			}	
		}
		break;

	case NHOUSEUSERSECONDHAND_DIALOG_BUTTON1:	// 고물상 짓기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 소유주가 틀린경우 그냥 return 함
					if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						TCHAR* pTitle = GetTxtFromMgr(370); // 시전
						TCHAR* pText = GetTxtFromMgr(1196);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					if ( pStgInfo->bCreatedHouseUnitUsedGoodsBuilding )
					{
						return;
					}

					SI32 ItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( ITEMUNIQUE(24025) );

					if ( ItemPos < 0 || pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ItemPos ].siItemNum < HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM )
					{
						TCHAR* pTitle = GetTxtFromMgr(1197); 
						TCHAR* pText = GetTxtFromMgr(1198);
						TCHAR num[128]= TEXT(" ") ;
						StringCchPrintf(num, 128, TEXT("%d"),HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM);
						Nsprintf(Nbuffer,pText,TEXT("item"),  pclClient->pclItemManager->GetName( 24025 )  ,TEXT("num"),  num  ,NULL);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);
						


						//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, pclClient->pclItemManager->GetName( 24025 ), HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM);
						return;
					}

					TCHAR* pText = GetTxtFromMgr(1199);


					TCHAR FeeExplain[128] = TEXT("");
					TCHAR num[128]= TEXT(" ") ;
					StringCchPrintf(num, 128, TEXT("%d"),HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM);
					Nsprintf(Nbuffer,pText,TEXT("item"),  pclClient->pclItemManager->GetName( ITEMUNIQUE(24025) )  ,TEXT("num"),num    ,NULL);

					//sprintf( FeeExplain, pText, pclClient->pclItemManager->GetName( ITEMUNIQUE(24025) ), HOUSEUNIT_CREATEUSEDGOODSBUILDING_USEITEMNUM );

					pText = GetTxtFromMgr(813);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pText, Nbuffer, MSGBOX_TYPE_YESNO, 5 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

					//if( MessageBox(NULL, FeeExplain, pText, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

					//cltGameMsgRequest_HouseUnitUsedGoodsBuildingCreate clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
					//cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE, sizeof(clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate), (TCHAR*)&clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate );
					//pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NHOUSEUSERSECONDHAND_DIALOG_BUTTON2:	// 물품해체
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 소유주가 틀린경우 그냥 return 함
					if ( m_pclHouseUnit->clPerson.GetPersonID() != pclClient->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						TCHAR* pTitle = GetTxtFromMgr(370);
						TCHAR* pText = GetTxtFromMgr(1196);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					if( m_siUsedGoodsItemRealSelectedIndex < 0 )
					{
						return;
					}

					cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
					if ( pStgInfo == NULL )
					{
						return;
					}

					if ( ! pStgInfo->bCreatedHouseUnitUsedGoodsBuilding )
					{
						TCHAR* pTitle = GetTxtFromMgr(815);
						TCHAR* pText = GetTxtFromMgr(1200);


						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return;
					}

					if ( m_clWareHouseItem[ m_siUsedGoodsItemRealSelectedIndex ].siUnique < 1 )
					{
						return;
					}

					if ( ! ( pclClient->pclItemManager->GetItemAtb( m_clWareHouseItem[ m_siUsedGoodsItemRealSelectedIndex ].siUnique ) & ITEMATB_OBJECT ) )
					{
						return;
					}

					TCHAR Explain[128] = TEXT("");
					TCHAR* pTitle = GetTxtFromMgr(1202);
					TCHAR* pText = GetTxtFromMgr(1201);

					StringCchPrintf( Explain, 128, pText, pclClient->pclItemManager->GetName( m_clWareHouseItem[ m_siUsedGoodsItemRealSelectedIndex ].siUnique ) );

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle, Explain, MSGBOX_TYPE_YESNO, 6 );
					pclclient->SetMsgBox( &MsgBox, NULL, 1 );

					//if( MessageBox(NULL, Explain, pTitle, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;

					//cltGameMsgRequest_HouseUnitMakeItemFromUsedGoods clGameMsgRequest_HouseUnitMakeItemFromUsedGoods(m_siCurrentVillageIndex, m_siCurrentHouseIndex, m_siUsedGoodsItemRealSelectedIndex, &m_clWareHouseItem[ m_siUsedGoodsItemRealSelectedIndex ] );
					//cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS, sizeof(clGameMsgRequest_HouseUnitMakeItemFromUsedGoods), (TCHAR*)&clGameMsgRequest_HouseUnitMakeItemFromUsedGoods );
					//pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;
	}
}


void CNHouseUserSecondhandDlg::EnableButton(CButton* pButton, BOOL Flag)
{
	if(Flag)
	{
		pButton->SetFontColor(RGB(0, 0, 0));
		pButton->Enable(true);
	}
	else
	{
		pButton->SetFontColor(RGB(225, 225, 225));
		pButton->Enable(false);
	}
}

void CNHouseUserSecondhandDlg::SetHouseUnitBuffer(CHouseUnitBuffer* pHouseUnitBuffer)
{
	m_pHouseUnitBuffer = pHouseUnitBuffer;
}

void CNHouseUserSecondhandDlg::SetHousePositionInfo(SI32 siCurrentVillageIndex, SI32 siCurrentHouseIndex)
{
	m_siCurrentVillageIndex = siCurrentVillageIndex;
	m_siCurrentHouseIndex = siCurrentHouseIndex;
}

void CNHouseUserSecondhandDlg::SetHouseUserItem(cltHouseUnit *pclHouseUnit)
{
	m_pclHouseUnit	  = pclHouseUnit;
//	LoadWareHouseInventory1();
}

void CNHouseUserSecondhandDlg::LoadWareHouseInventory1()
{
	m_siUsedGoodsItemSelectedIndex = -1;

	m_pList->Clear();
	m_pEdit[1]->SetText(TEXT(""));

	//----- 고물상이 없는 경우 계속 Timer 돌 필요 없음으로... 건너 뛰기 위해서..
	cltHouseUnitStorageInfo *pStgInfo = &m_pclHouseUnit->clStorageInfo;
	if ( pStgInfo == NULL )
	{
		return;
	}

	if ( ! pStgInfo->bCreatedHouseUnitUsedGoodsBuilding )
	{
		// 고물상 짓기 버튼 활성화, 해체 버튼 비활성화
		EnableButton(m_pButton[0], TRUE);
		EnableButton(m_pButton[1], FALSE);

		return;
	}
	//----------------------------------------------------------

	LVITEM Li;
	Li.mask = LVIF_TEXT;
	Li.state = 0;
	Li.stateMask = 0;	

	SI16 Counter = 0;
	TCHAR szItemDurability[ 16 ];

	stListBoxItemData Data;
	TCHAR buffer[10] = TEXT("");

	for ( int i = 0; i < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++i )
	{
		m_clWareHouseItem[ i ] = m_pclHouseUnit->clStorageInfo.clItem[ i ];
		if ( m_clWareHouseItem[ i ].siUnique > 0 )
		{
			SI64 atb = pclClient->pclItemManager->GetItemAtb( m_clWareHouseItem[ i ].siUnique );

			if ( (atb & ITEMATB_OBJECT) && ( m_clWareHouseItem[ i ].GetDurability() <= DUR_FOR_DISMANTLE ||
											 pclClient->pclCM->CR[1]->GetGameMasterLevel() > BASIC_GAMEMASTER_LEVEL ) )
			{
				Data.Init();
				
				Data.siParam[0] = i;

				Data.Set(0, (TCHAR*)pclClient->pclItemManager->GetName( m_clWareHouseItem[ i ].siUnique ));
				StringCchPrintf( szItemDurability, 16, TEXT("%d/%d"), m_clWareHouseItem[ i ].GetDurability(  ), m_clWareHouseItem[ i ].GetMaxDurability( pclClient->pclItemManager ) );
				Data.Set(1, (TCHAR*)szItemDurability);
				
				m_pList->AddItem(&Data);
				
			}
		}
	}

	m_pList->Refresh();

	// 고물상 짓기 버튼 비활성화, 해체 버튼 활성화
	EnableButton(m_pButton[0], FALSE);
	EnableButton(m_pButton[1], TRUE);

	return;
}
bool CNHouseUserSecondhandDlg::IsWareHouseInventoryChanged()
{
	if ( m_pclHouseUnit == NULL )
	{
		return FALSE;
	}

	for ( int j = 0; j < MAX_HOUSEUNIT_STORAGE_ITEM_NUMBER; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clWareHouseItem[ j ];
		BYTE* dst = (BYTE*)&m_pclHouseUnit->clStorageInfo.clItem[ j ];

		for( int i = 0; i < size; ++i )
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

void CNHouseUserSecondhandDlg::ShowMakeItemFromUsedGoodsItem( cltItem *pMakeableItemList )
{
	if ( m_siUsedGoodsItemSelectedIndex < 0 )
	{
	//	SetWindowText( GetDlgItem( m_hHouseUnitUsedGoodsDlg, IDC_EDIT_HOUSEUNIT_MAKEITEMFROMUSEDGOODS ), TEXT("") );
		m_pEdit[1]->SetText(TEXT("")); // 고물 내용 없다
		return;
	}

	TCHAR* pTitle = GetTxtFromMgr(1193);
	TCHAR TempBuffer[2024] = TEXT("");
	StringCchPrintf( TempBuffer, 2024, TEXT("%s%s\r\n"), pTitle, TEXT("이조판서 상인 구매 비율 적용 수치입니다.") );
	TCHAR Buffer[ 2024 ] = TEXT("") ;
	MStrCpy(Buffer,TempBuffer,2024);
	TCHAR szTemp[ 256 ] = TEXT("");
	SI16 i = 0;
	for ( i = 0; i < MAX_MATERIAL_NUMBER; ++i )
	{
		if ( pMakeableItemList[ i ].siUnique > 0 )
		{
			TCHAR* pText = GetTxtFromMgr(1194);

			StringCchPrintf( szTemp, 256, pText, (TCHAR*)pclClient->pclItemManager->GetName( pMakeableItemList[ i ].siUnique ), pMakeableItemList[ i ].siItemNum ) ;
			StringCchCat( Buffer, 2024, szTemp );
		}
	}

//	SetWindowText( GetDlgItem ( m_hHouseUnitUsedGoodsDlg, IDC_EDIT_HOUSEUNIT_MAKEITEMFROMUSEDGOODS ), Buffer );
	m_pEdit[1]->SetText(Buffer); // 고물 내용있다

	return;
}

void CNHouseUserSecondhandDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 고물상 짓기
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitUsedGoodsBuildingCreate clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate( m_siCurrentVillageIndex, m_siCurrentHouseIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITUSEDGOODSBUILDINGCREATE, sizeof(clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate), (BYTE*)&clGameMsgRequest_HouseUnitUsedGoodsBuildingCreate );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1: // 물품 해체
		{
			if( RetValue )
			{
				cltGameMsgRequest_HouseUnitMakeItemFromUsedGoods clGameMsgRequest_HouseUnitMakeItemFromUsedGoods(m_siCurrentVillageIndex, m_siCurrentHouseIndex, m_siUsedGoodsItemRealSelectedIndex, &m_clWareHouseItem[ m_siUsedGoodsItemRealSelectedIndex ] );
				cltMsg clMsg( GAMEMSG_REQUEST_HOUSEUNITMAKEITEMFROMUSEDGOODS, sizeof(clGameMsgRequest_HouseUnitMakeItemFromUsedGoods), (BYTE*)&clGameMsgRequest_HouseUnitMakeItemFromUsedGoods );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}
