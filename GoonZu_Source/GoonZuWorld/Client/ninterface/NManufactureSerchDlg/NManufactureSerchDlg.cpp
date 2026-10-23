//#include "../NPersonalShopDlg/NPersonalShopDlg.h"
#include "./NManufactureSerchDlg.h"
#include <tchar.h>
#include "../../../Common/SMLib/LibList/LibList.h"
#include "../../../Common/PrivateTradeOrderMgr/PrivateTradeOrderObj.h"

#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg\NotifyMsgDlg.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Personalshop.h"
#include "../../CommonLogic/Msg/MsgType-ItemMall.h"
#include "../../CommonLogic/Msg/MsgType-Web.h"
#include "..\..\Client\Client.h"

#include "..\..\Client\Music\Music.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"
#include "../../../Client/InterfaceMgr/Interface/RadioButton.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;


// TIMER
CNManufactureSearchDlg::CNManufactureSearchDlg()
{
	m_pStatictext_itemsearch	 = NULL; 	
	m_pCombobox_iteminfo_type	 = NULL;
	m_pRadiobutton_makeitem	 = NULL;
	m_pRadiobutton_makeitem_want	=	NULL;
	m_pButton_search	 = NULL;
	m_pListbox_shop_showcase	 = NULL; 	
	m_pEditbox_item_explain	 = NULL; 	
	m_pEditbox_shopper_location	 = NULL; 	
	m_pButton_personalshop_warp	 = NULL;
	m_pButton_close	 = NULL; 	
	m_pImageStatic_Item	= NULL;

	m_bShow = false;
	m_bTimer= false;
	m_bShopDataChanged = false;	
	memset(m_szAccountID, 0, sizeof(m_szAccountID));
	m_siShopShowCaseSelectedIndex			= -1;
	m_siImageIndex = -1;
	m_siImageType = -1;

	Initialize();
}

CNManufactureSearchDlg::~CNManufactureSearchDlg()
{
	if(		m_pImageStatic_Item			 )		delete	m_pImageStatic_Item		;
	if( 	m_pStatictext_itemsearch	 )  	delete 	m_pStatictext_itemsearch	;
	if( 	m_pCombobox_iteminfo_type	 )  	delete 	m_pCombobox_iteminfo_type	;	
	if( 	m_pRadiobutton_makeitem	 )  	delete 	m_pRadiobutton_makeitem	;	
	if( 	m_pRadiobutton_makeitem_want	 )  	delete 	m_pRadiobutton_makeitem_want	;	
	
	if( 	m_pButton_search	 )  	delete 	m_pButton_search	;	
	if( 	m_pListbox_shop_showcase	 )  	delete 	m_pListbox_shop_showcase	;
	if( 	m_pEditbox_item_explain	 )  	delete 	m_pEditbox_item_explain	;
	if( 	m_pEditbox_shopper_location	 )  	delete 	m_pEditbox_shopper_location	;
	if( 	m_pButton_personalshop_warp	 )  	delete 	m_pButton_personalshop_warp	;	
	if( 	m_pButton_close	 )  	delete 	m_pButton_close	;
	if(m_bTimer==true) SetTimerOnOff( false );
	m_bTimer = false;
}

void CNManufactureSearchDlg::Initialize()
{
	return;
}

void CNManufactureSearchDlg::Destroy()
{

	return;
}

void CNManufactureSearchDlg::Create()
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NManufactureSearchDlg/NManufactureSearchDlg.ddf"));

		file.CreatePopupDialog( this, MANUFACTURESEARCH_DLG, TEXT("dialog_shop_search"),StaticNManufactureSearchDlgProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_shop_search") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
		{
			siTmpWidth = file.m_control[ siTmp  ].width;
			siTmpWidth += 420;
		}

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pStatictext_itemsearch	 = new 	CStatic	( this )	;
		m_pCombobox_iteminfo_type	 = new 	CComboBox	( this )	;		
		m_pRadiobutton_makeitem	 = new 	CRadioButton	( this )	;
		m_pRadiobutton_makeitem_want	 = new 	CRadioButton	( this )	;		
		m_pButton_search	 = new 	CButton	( this )	;		
		m_pListbox_shop_showcase	 = new 	CList	( this )	;
		m_pEditbox_item_explain	 = new 	CEdit	( this )	;
		m_pEditbox_shopper_location	 = new 	CEdit	( this )	;
		m_pButton_personalshop_warp	 = new 	CButton	( this )	;
		
		m_pButton_close	 = new 	CButton	( this )	;
		m_pImageStatic_Item	= new CImageStatic( this )	;

		file.CreateControl( m_pStatictext_itemsearch, NMANUFACTURESEARCH_STATICTEXT_ITEMSEARCH, TEXT("statictext_itemsearch") );
		file.CreateControl( m_pRadiobutton_makeitem, NMANUFACTURESEARCH_RADIOBUTTON_MAKEITEM, TEXT("radiobutton_makeitem") );
		file.CreateControl( m_pRadiobutton_makeitem_want, NMANUFACTURESEARCH_RADIOBUTTON_MAKEITEM_WANT, TEXT("radiobutton_makeitem_want") );
		file.CreateControl( m_pButton_search, NMANUFACTURESEARCH_BUTTON_SEARCH, TEXT("button_search") );
		file.CreateControl( m_pListbox_shop_showcase, NMANUFACTURESEARCH_LISTBOX_SHOP_SHOWCASE, TEXT("listbox_shop_showcase") );
		file.CreateControl( m_pEditbox_item_explain, NMANUFACTURESEARCH_EDITBOX_ITEM_EXPLAIN, TEXT("editbox_item_explain") );
		file.CreateControl( m_pEditbox_shopper_location, NMANUFACTURESEARCH_EDITBOX_SHOPPER_LOCATION, TEXT("editbox_shopper_location") );
		file.CreateControl( m_pButton_personalshop_warp, NMANUFACTURESEARCH_BUTTON_PERSONALSHOP_WARP, TEXT("button_personalshop_warp") );
		file.CreateControl( m_pButton_close, NMANUFACTURESEARCH_BUTTON_CLOSE, TEXT("button_close") );
		file.CreateControl( m_pCombobox_iteminfo_type, NMANUFACTURESEARCH_COMBOBOX_ITEMINFO_TYPE, TEXT("combobox_iteminfo_type") );
		file.CreateControl(	m_pImageStatic_Item, NMANUFACTURESEARCH_IMAGESTATIC_ITEM, TEXT("imagestatic_NONAME1")	);

		if(m_pListbox_shop_showcase)
		{
			TCHAR* pText = GetTxtFromMgr(2251);
			m_pListbox_shop_showcase->SetColumn( 0, 90, pText );
			pText = GetTxtFromMgr(40838);	// 현제위치
			m_pListbox_shop_showcase->SetColumn( 1, 90, pText );
			pText = GetTxtFromMgr(2195);	// 수수료
			m_pListbox_shop_showcase->SetColumn( 2, 83, pText );
			pText = GetTxtFromMgr(1679);
			m_pListbox_shop_showcase->SetColumn( 3, 55, pText );
			m_pListbox_shop_showcase->SetColumn( 4, 19, TEXT("") );
			m_pListbox_shop_showcase->SetBorder( true );
			m_pListbox_shop_showcase->SetBKColorUse( true );
		}

		m_pListbox_shop_showcase->SetUseToolTip( true );
		m_pListbox_shop_showcase->Refresh();

		// 아이템 종류를 보여준다. 
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		this->SetItemTypeInCombo(false);
		
		////---------------------------------
		//// INIT CONTROLS		   --------
		////---------------------------------

		TCHAR Tempbuffer[128];
		_i64tot(MAX_PRICE_IN_MARKET, Tempbuffer, 10);	
		// Clear
		m_pListbox_shop_showcase->Clear();
		m_pEditbox_item_explain->SetText(TEXT("") );
		m_pEditbox_shopper_location->SetText( TEXT("") );

		// 생성하면서 기본은 제조의뢰
		if( m_pRadiobutton_makeitem->IsCheck() == false )
			m_pRadiobutton_makeitem->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON );

		SetActionTimer( NMANUFACTURESEARCHDLG_SETTIMER_TIME );
		SetTimerOnOff( true );

		m_siImageIndex = -1;
		m_siImageType = -1;
	}
	else
	{
		DeleteDialog();
	}

	return;
}


void CALLBACK CNManufactureSearchDlg::StaticNManufactureSearchDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNManufactureSearchDlg *pThis = (CNManufactureSearchDlg*) pControllerMgr;
	pThis->NManufactureSearchDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNManufactureSearchDlg::NManufactureSearchDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMANUFACTURESEARCH_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				DeleteDialog();
				break;
			}
		}
		break;

	case NMANUFACTURESEARCH_LISTBOX_SHOP_SHOWCASE:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI16 siTmpRow = -1, siTmpCol = -1;
					m_pListbox_shop_showcase->GetSelectedIndex( &siTmpRow, &siTmpCol );

					if(siTmpRow < 0 ) return;

					m_siShopShowCaseSelectedIndex = siTmpRow;
					
					SI32 siListViewIndex = 0;
					
					for ( int i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
					{
						if ( m_clSearhShopData.m_clItem[ i ].siUnique != 0 )
						{
							if ( m_siShopShowCaseSelectedIndex == siListViewIndex )
							{
								DisplaySowCaseItemInfo(i);
								break;
							}
							else
							{
								++siListViewIndex;
							}
						}
					}	
				}
				break;
			}
		}
		break;
	case NMANUFACTURESEARCH_BUTTON_SEARCH:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SearchShopItem();
				}
				break;
			}
		}
		break;
	case NMANUFACTURESEARCH_RADIOBUTTON_MAKEITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{

				}
				break;
			}
		}
		break;
	case NMANUFACTURESEARCH_RADIOBUTTON_MAKEITEM_WANT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{

				}
				break;
			}
		}
		break;

	case NMANUFACTURESEARCH_BUTTON_PERSONALSHOP_WARP:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

					SI32 siMapIndex = 0;
					SI32 siMapPosX = 0;
					SI32 siMapPosY = 0;
					TCHAR buffer[512]= TEXT("");

					// 현재 선택된 상점을 확보한다. 
					if(m_siShopShowCaseSelectedIndex < 0 || m_siShopShowCaseSelectedIndex >= MAX_SEARCHSHOP_ITEM_NUMBER)
					{
						TCHAR* pTitle = GetTxtFromMgr(1413);
						TCHAR* pText = GetTxtFromMgr(2257);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return ;
					}
					else
					{
						siMapIndex = m_clSearhShopData.m_siMapIndex[m_siShopShowCaseSelectedIndex];
						siMapPosX = m_clSearhShopData.m_siMapX[m_siShopShowCaseSelectedIndex];
						siMapPosY = m_clSearhShopData.m_siMapY[m_siShopShowCaseSelectedIndex];

						if( _tcslen(m_clSearhShopData.m_szName[m_siShopShowCaseSelectedIndex]) <= 0 || 
							(siMapIndex == 0 && siMapPosX == 0 && siMapPosY == 0))
						{
							TCHAR* pTitle = GetTxtFromMgr(1413);
							TCHAR* pText = GetTxtFromMgr(2257);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return ;
						}
						
						if(siMapIndex< 0 || siMapIndex>= MAX_MAP_NUMBER)
						{
							TCHAR* pTitle = GetTxtFromMgr(1413);
							TCHAR* pText = GetTxtFromMgr(2257);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return ;
						}
					}
					
					// 워프를 할 수 있는가?
					if( pclclient->pclCM->CR[1]->CanWarpFrom() == false)	return;

					TCHAR* pTitle = NULL ;
					TCHAR* pText = NULL ;

					// 공짜 이동 체크
					if( pclchar->CanFreeWarp( siMapIndex ) == false )
					{
						// 순간이동이용권을 보유하고 있는지 확인한다. 
						SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
						SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
						if(pos < 0)
						{
							pTitle = GetTxtFromMgr(1413);
							pText = GetTxtFromMgr(3192);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

							return ;
						}

						pTitle = GetTxtFromMgr(1418);
						pText = GetTxtFromMgr(2262);

						StringCchPrintf(buffer, 512, pText,
							pclClient->pclMapManager->GetMapName(siMapIndex),
							siMapPosX, siMapPosY);
					}
					else
					{
						pTitle = GetTxtFromMgr(3189);
						pText = GetTxtFromMgr(3193);

						StringCchPrintf(buffer, 512, pText,
							pclClient->pclMapManager->GetMapName(siMapIndex),
							siMapPosX, siMapPosY);
					}

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, buffer, MSGBOX_TYPE_YESNO, 0 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( siMapIndex ) + sizeof( siMapPosX ) + sizeof( siMapPosY );
					memcpy( TempBuffer, &siMapIndex, sizeof( siMapIndex ) );
					memcpy( &TempBuffer[ sizeof( siMapIndex ) ], &siMapPosX, sizeof( siMapPosX ) );
					memcpy( &TempBuffer[ sizeof( siMapIndex ) + sizeof( siMapPosX ) ], &siMapPosY, sizeof( siMapPosY ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				}
				break;				
			}
		}
		break;
	}

	return ;
}

bool CNManufactureSearchDlg::IsShow()
{
	return m_bShow;
}

void CNManufactureSearchDlg::Show()
{
	
	if(m_bShow == false)	CDialogController::Show( true );
	m_bShow = true;
	
	if(m_bTimer == false) 
	{
		SetActionTimer( NMANUFACTURESEARCHDLG_SETTIMER_TIME );
		SetTimerOnOff( true );
	}

	m_bTimer = true;

	m_LastSearchTime = pclClient->CurrentClock;

	return;
}

void CNManufactureSearchDlg::Hide()
{
	if(m_bShow==true)	CDialogController::Show( false );
	m_bShow = false;

	if(m_bTimer==true) SetTimerOnOff( false );
	m_bTimer = false;
}



bool CNManufactureSearchDlg::IsShopShowCaseChanged()
{
	if(m_bShopDataChanged) return true;

	return false;
}

void CNManufactureSearchDlg::SortSearchedData()
{
	//=================================================
	// DATA를 SORT시킨다.
	//=================================================
	cltSearchShopData TempShopData;
	TempShopData.Set(&m_clSearhShopData);

	m_clSearhShopData.Init();
	m_clSearhShopData.CopyGeneralData(&TempShopData);
	
	if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL)
	{
		BOOL bFound = FALSE;
		GMONEY siMinValue = 0;
		SI32 siMinIndex = 0;
		for (SI32 i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
		{	
			siMinValue = MAX_PRICE_IN_MARKET+1;
			siMinIndex = 0;
			bFound     = FALSE;
			
			for (SI32 j = 0; j < MAX_SEARCHSHOP_ITEM_NUMBER; ++j )
			{	
				if(TempShopData.m_clItem[j].siUnique)
				{
					if(TempShopData.m_siPrice[j] <	siMinValue)
					{
						siMinValue = TempShopData.m_siPrice[j] ;
						siMinIndex = j;
						bFound     = TRUE;
					}
				}
			}
			if(bFound)	m_clSearhShopData.MoveData(i, siMinIndex, &TempShopData);
		}
	}
	else	// 구매자를 검색할때는 가격이 높은것부터 
	{
		BOOL bFound = FALSE;
		GMONEY siMaxValue = 0;
		SI32 siMaxIndex = 0;
		for (SI32 i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
		{	
			siMaxValue = -1;
			siMaxIndex = 0;
			bFound     = FALSE;
			
			for (SI32 j = 0; j < MAX_SEARCHSHOP_ITEM_NUMBER; ++j )
			{	
				if(TempShopData.m_clItem[j].siUnique)
				{
					if(TempShopData.m_siPrice[j] >	siMaxValue)
					{
						siMaxValue = TempShopData.m_siPrice[j] ;
						siMaxIndex = j;
						bFound     = TRUE;
					}
				}
			}
			if(bFound)	m_clSearhShopData.MoveData(i, siMaxIndex, &TempShopData);
		}
	}
	//=================================================
}	

void CNManufactureSearchDlg::LoadShopShwocase()
{
	//=================================================
	// 얻어온 데이터를 Sort 시킨다.
	//=================================================
	SortSearchedData();
	//=================================================

	m_siShopShowCaseSelectedIndex = -1;
	m_bShopDataChanged = false;

	TCHAR itemname[128]= TEXT("");
	TCHAR itembuffer[128];
	SI32 siItemCount = 0;
	stListBoxItemData tmpListBoxItemData;

	m_pListbox_shop_showcase->Clear();

	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	TCHAR pTmpChar[ 512 ];

	for (SI32 i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
	{	
		if(m_clSearhShopData.m_clItem[i].siUnique)
		{
			if(m_clSearhShopData.m_clItem[ i ].GetFullName(pclClient->pclItemManager, itemname, 128) == true)
			{
				memset( pTmpChar, 0, 512 );

				tmpListBoxItemData.Init();				
				tmpListBoxItemData.Set( 0, itemname );				
				tmpListBoxItemData.Set( 1, pclClient->pclMapManager->GetMapName(m_clSearhShopData.m_siMapIndex[i]) );
			
				TCHAR* pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(m_clSearhShopData.m_siPrice[i], itembuffer, 128, pText);
				tmpListBoxItemData.Set( 2, itembuffer, ((cltClient*)pclClient)->GetColorByPrice(m_clSearhShopData.m_siPrice[i]) );				

				pText = GetTxtFromMgr(2209);
				StringCchPrintf(itembuffer, 128, pText, m_clSearhShopData.m_clItem[ i ].siItemNum);
				tmpListBoxItemData.Set( 3, itembuffer );
				//  Tooltip 셋팅!!
                tmpListBoxItemData.SetToolTipMsg( 0, pTmpChar );
                m_pListbox_shop_showcase->AddItem( &tmpListBoxItemData );
				siItemCount++;
			}
		}
	}

	m_pListbox_shop_showcase->Refresh();
	
	if(m_clSearhShopData.m_siItemUniqueType)
	{
		StringCchCopy(itemname, 128, TEXT(""));

		SI32 siItemUniqueType = m_clSearhShopData.m_siItemUniqueType;
		if(siItemUniqueType>0 && siItemUniqueType< MAX_ITEMTYPE_NUMBER)
		{
			if(pclClient->pclItemManager->pclItemTypeInfo[siItemUniqueType])
			{
				StringCchCopy(itemname, 128, pclClient->pclItemManager->pclItemTypeInfo[siItemUniqueType]->GetName());
			}
		}
	}

	g_JYLibFunction.SetNumUnit(m_clSearhShopData.m_siItemSearchPrice, itembuffer, 128, TEXT(""));
	TCHAR* pText =	pText = GetTxtFromMgr(2195);
	m_pListbox_shop_showcase->SetColumn( 2, 80, pText );

	// EDIT CLEAR
	m_pEditbox_item_explain->SetText( TEXT("") );

	// 검색 완료 메시지 
	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0);
	if(siItemCount <1)
	{
		TCHAR* pText = GetTxtFromMgr(2266);
		StringCchPrintf(itembuffer, 128, pText,   itemname);
	}	
	else if(siItemCount < MAX_SEARCHSHOP_ITEM_NUMBER)
	{
		TCHAR* pText = GetTxtFromMgr(2267);
		StringCchPrintf(itembuffer, 128, pText,    itemname, siItemCount);
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(2268);
		StringCchPrintf(itembuffer, 128, pText, itemname);
	}
	m_pEditbox_shopper_location->SetText( itembuffer );
	return;
}

void CNManufactureSearchDlg::DisplaySowCaseItemInfo(SI32 index)
{
	TCHAR ItemExplain[ 512 ] = TEXT("");

	//==========================================
	// EDIT_ITEM_EXPLAIN
	//==========================================
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	
	pclitemmanager->GetExplain( m_clSearhShopData.m_clItem[ index ].siUnique, ItemExplain );
	
	SI32 silen = _tcslen(ItemExplain);
	if( silen > 2 && ItemExplain[silen-1] =='\n') {	ItemExplain[silen-1] = 0;	}
	if( silen > 2 && ItemExplain[silen-2] =='\r') {	ItemExplain[silen-2] = 0;	}

	m_pEditbox_item_explain->SetText( ItemExplain );

	SetItemImageData( m_clSearhShopData.m_clItem[ index ].siUnique );

	//SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_ITEM_EXPLAIN) , ItemExplain );

	//==========================================
	// EDIT_SHOPPER_LOCATION
	//==========================================
	TCHAR szNearInfo[128];
	((cltClient *)pclClient)->GetNearMapInfo(m_clSearhShopData.m_siMapIndex[index], m_clSearhShopData.m_siMapX[index], m_clSearhShopData.m_siMapY[index], szNearInfo, 128);

	TCHAR* pText = GetTxtFromMgr(2269);

	StringCchPrintf(ItemExplain, 512, pText,
			m_clSearhShopData.m_szName[index], m_clSearhShopData.m_siLevel[index],
			pclClient->pclMapManager->GetMapName(m_clSearhShopData.m_siMapIndex[index]),
			m_clSearhShopData.m_siMapX[index], m_clSearhShopData.m_siMapY[index],
			szNearInfo,
			m_clSearhShopData.m_siHandPoint[index], 
			m_clSearhShopData.m_siSkillLevel[index] 
			);

	//SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_SHOPPER_LOCATION)  , ItemExplain );	
			m_pEditbox_shopper_location->SetText( ItemExplain );

}

void CNManufactureSearchDlg::SearchShopItem()
{
	// 300 ms 이하에서는 계속 검색을 하지 못하게 한다.
	if(TABS(pclClient->CurrentClock - m_LastSearchTime) < 500) return;
	m_LastSearchTime = pclClient->CurrentClock ;
	
	SI32 siShopMode = PSHOP_MODE_MAKEITEM;
	if( m_pRadiobutton_makeitem->IsCheck() == true )
	{
		siShopMode = PSHOP_MODE_MAKEITEM;
	}
	if( m_pRadiobutton_makeitem_want->IsCheck() == true )
	{
		siShopMode = PSHOP_MODE_MAKEITEM_WANT;
	}
	
	TCHAR szSelText[128];
	SI32 itemtypeunique = 0;
	SI16 siTmp = -1;
	TCHAR* pTmpChar = NULL;
	siTmp = m_pCombobox_iteminfo_type->GetSelectedIndex();
	if(siTmp < 0 ) return;
	
	pTmpChar = m_pCombobox_iteminfo_type->GetText( siTmp );
	if( pTmpChar == NULL || _tcslen(pTmpChar) >= 128  ) return ;

	_tcscpy( szSelText, pTmpChar );	

	itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);

	if(itemtypeunique)
	{
		SI32 siPrice    = 0;
		// 검색 요청 
		cltGameMsgRequest_SearchManufacture clsearchdata( siShopMode, itemtypeunique);
		cltMsg clMsg( GAMEMSG_REQUEST_SEARCHMANUFACTURE, sizeof( clsearchdata ), (BYTE*)&clsearchdata );
		cltClient *pclclient = ( cltClient * )pclClient;
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

// cltItemManagerClient 에서 가져옴 ======================================================================
// 아이템 종류를 보여준다. 
bool CNManufactureSearchDlg::SetItemTypeInCombo( bool bshowall)
{
	cltItemManagerCommon* pclItemManager;
	pclItemManager = pclClient->pclItemManager;

	if(pclItemManager == NULL ) return false;

	SI32 i;
	stComboBoxItemData stTmpComboBoxItemData;
	// 콤보 박스를 지운다.
	m_pCombobox_iteminfo_type->Clear();

	// 콤보 박스에 아이템의 종류를 설정한다. 
	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		cltItemTypeInfo* pinfo = pclItemManager->pclItemTypeInfo[i];
		if(pinfo == NULL)continue;

		// 거래 불가 품종이면 보여주지 않는 설정이라면, 
		if(bshowall == false)
		{
			if(pinfo->IsAtb(ITEMATB_NOTRADE))continue;
		}

		// 콤보리스트에서 숨기는 속성이 있다면
		if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;

		stTmpComboBoxItemData.Init();
		stTmpComboBoxItemData.Set( (TCHAR*)pinfo->GetName());

		m_pCombobox_iteminfo_type->AddItem( &stTmpComboBoxItemData );
	}


	return TRUE;
}

void CNManufactureSearchDlg::Action()
{
	if ( IsShopShowCaseChanged() )
	{
		LoadShopShwocase();
	}
}

void CNManufactureSearchDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch( MsgIndex )
	{
	case 0:	// 검색된 개인 상점으로 이동
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 siMapIndex = *((SI32*)pData);
				SI32 siMapPosX = *((SI32*)(pData + 4 ));
				SI32 siMapPosY = *((SI32*)(pData + 8 ));

				cltGameMsgRequest_PersonalShopWarp pclInfo(siMapIndex, siMapPosX, siMapPosY);
				cltMsg clMsg( GAMEMSG_REQUEST_PERSONALSHOPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}


void CNManufactureSearchDlg::SetItemImageData(SI32 ItemUnique)
{
	if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ItemUnique, &m_siImageType, &m_siImageIndex ) )
	{}
	else
	{
		m_siImageType = -1;
		m_siImageIndex = -1;
	}
}
void CNManufactureSearchDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	if( m_siImageIndex < 0 || m_siImageType < 0 )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	if ( pSpr )
	{
		GP.PutSpr( pSpr ,ScreenRX +  GetX() + 48,ScreenRY +  GetY() + 363, m_siImageIndex);
	}

}