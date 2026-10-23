/* ==========================================================================
	클래스 :		CNPersonalShopSearchDlg

	작성일 :		05/04/06
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		1. html 부분 재작업 필요
					

   ========================================================================*/

#include "../NPersonalShopDlg/NPersonalShopDlg.h"
#include "./NPersonalShopSearchDlg.h"

//#if defined(_DEBUG) && defined(_LEON_NPERSONALSHOPSEARCH_)

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
CNPersonalShopSearchDlg::CNPersonalShopSearchDlg()
{
	m_pStatictext_itemsearch	 = NULL; 	
	m_pCombobox_iteminfo_type	 = NULL; 	
	m_pRadiobutton_sell	 = NULL; 	
	m_pRadiobutton_buy	 = NULL; 	
	m_pRadiobutton_makeitem	 = NULL; 	
	m_pCheckbox_shop_price	 = NULL; 	
	m_pEditbox_price_condition	 = NULL; 	
	m_pStatictext_above	 = NULL; 	
	m_pButton_search	 = NULL; 	
	m_pStatictext_item_tag	 = NULL; 	
	m_pStatictext_searchtype	 = NULL; 	
	m_pEditbox_searched_itemtype	 = NULL; 	
	m_pEditbox_itemsearchprice	 = NULL; 	
	m_pStatictext_searchbelow	 = NULL; 	
	m_pListbox_shop_showcase	 = NULL; 	
	m_pEditbox_item_explain	 = NULL; 	
	m_pEditbox_shopper_location	 = NULL; 	
	m_pButton_personalshop_warp	 = NULL; 	
	m_pButton_minihome	 = NULL; 	
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

CNPersonalShopSearchDlg::~CNPersonalShopSearchDlg()
{
	if(		m_pImageStatic_Item			 )		delete	m_pImageStatic_Item		;
	if( 	m_pStatictext_itemsearch	 )  	delete 	m_pStatictext_itemsearch	;
	if( 	m_pCombobox_iteminfo_type	 )  	delete 	m_pCombobox_iteminfo_type	;
	if( 	m_pRadiobutton_sell	 )  	delete 	m_pRadiobutton_sell	;
	if( 	m_pRadiobutton_buy	 )  	delete 	m_pRadiobutton_buy	;
	if( 	m_pRadiobutton_makeitem	 )  	delete 	m_pRadiobutton_makeitem	;
	if( 	m_pCheckbox_shop_price	 )  	delete 	m_pCheckbox_shop_price	;
	if( 	m_pEditbox_price_condition	 )  	delete 	m_pEditbox_price_condition	;
	if( 	m_pStatictext_above	 )  	delete 	m_pStatictext_above	;
	if( 	m_pButton_search	 )  	delete 	m_pButton_search	;
	if( 	m_pStatictext_item_tag	 )  	delete 	m_pStatictext_item_tag	;
	if( 	m_pStatictext_searchtype	 )  	delete 	m_pStatictext_searchtype	;
	if( 	m_pEditbox_searched_itemtype	 )  	delete 	m_pEditbox_searched_itemtype	;
	if( 	m_pEditbox_itemsearchprice	 )  	delete 	m_pEditbox_itemsearchprice	;
	if( 	m_pStatictext_searchbelow	 )  	delete 	m_pStatictext_searchbelow	;
	if( 	m_pListbox_shop_showcase	 )  	delete 	m_pListbox_shop_showcase	;
	if( 	m_pEditbox_item_explain	 )  	delete 	m_pEditbox_item_explain	;
	if( 	m_pEditbox_shopper_location	 )  	delete 	m_pEditbox_shopper_location	;
	if( 	m_pButton_personalshop_warp	 )  	delete 	m_pButton_personalshop_warp	;
	if( 	m_pButton_minihome	 )  	delete 	m_pButton_minihome	;
	if( 	m_pButton_close	 )  	delete 	m_pButton_close	;

	if(m_bTimer==true) SetTimerOnOff( false );
	m_bTimer = false;
}

void CNPersonalShopSearchDlg::Initialize()
{
	return;
}

void CNPersonalShopSearchDlg::Destroy()
{

	return;
}

void CNPersonalShopSearchDlg::Create()
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPersonalShopSearchDlg/DLG_NPersonalShopSearch.ddf"));

		file.CreatePopupDialog( this, NPERSONALSHOPSEARCH_DLG, TEXT("dialog_shop_search"),StaticNPersonalShopSearchDlgProc);

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
		m_pRadiobutton_sell	 = new 	CRadioButton	( this )	;
		m_pRadiobutton_buy	 = new 	CRadioButton	( this )	;
		m_pRadiobutton_makeitem	 = new 	CRadioButton	( this )	;
		m_pCheckbox_shop_price	 = new 	CCheckBox	( this )	;
		m_pEditbox_price_condition	 = new 	CEdit	( this )	;
		m_pStatictext_above	 = new 	CStatic	( this )	;
		m_pButton_search	 = new 	CButton	( this )	;
		m_pStatictext_item_tag	 = new 	CStatic	( this )	;
		m_pStatictext_searchtype	 = new 	CStatic	( this )	;
		m_pEditbox_searched_itemtype	 = new 	CEdit	( this )	;
		m_pEditbox_itemsearchprice	 = new 	CEdit	( this )	;
		m_pStatictext_searchbelow	 = new 	CStatic	( this )	;
		m_pListbox_shop_showcase	 = new 	CList	( this )	;
		m_pEditbox_item_explain	 = new 	CEdit	( this )	;
		m_pEditbox_shopper_location	 = new 	CEdit	( this )	;
		m_pButton_personalshop_warp	 = new 	CButton	( this )	;
		m_pButton_minihome	 = new 	CButton	( this )	;
		m_pButton_close	 = new 	CButton	( this )	;
		m_pImageStatic_Item	= new CImageStatic( this )	;

		file.CreateControl( m_pStatictext_itemsearch, NPERSONALSHOPSEARCH_STATICTEXT_ITEMSEARCH, TEXT("statictext_itemsearch") );
		file.CreateControl( m_pRadiobutton_sell, NPERSONALSHOPSEARCH_RADIOBUTTON_SELL, TEXT("radiobutton_sell") );
		file.CreateControl( m_pRadiobutton_buy, NPERSONALSHOPSEARCH_RADIOBUTTON_BUY, TEXT("radiobutton_buy") );
		file.CreateControl( m_pRadiobutton_makeitem, NPERSONALSHOPSEARCH_RADIOBUTTON_MAKEITEM, TEXT("radiobutton_makeitem") );
		file.CreateControl( m_pCheckbox_shop_price, NPERSONALSHOPSEARCH_CHECKBOX_SHOP_PRICE, TEXT("checkbox_shop_price") );
		file.CreateControl( m_pEditbox_price_condition, NPERSONALSHOPSEARCH_EDITBOX_PRICE_CONDITION, TEXT("editbox_price_condition") );
		file.CreateControl( m_pStatictext_above, NPERSONALSHOPSEARCH_STATICTEXT_ABOVE, TEXT("statictext_above") );
		file.CreateControl( m_pButton_search, NPERSONALSHOPSEARCH_BUTTON_SEARCH, TEXT("button_search") );
		file.CreateControl( m_pStatictext_item_tag, NPERSONALSHOPSEARCH_STATICTEXT_ITEM_TAG, TEXT("statictext_item_tag") );
		file.CreateControl( m_pStatictext_searchtype, NPERSONALSHOPSEARCH_STATICTEXT_SEARCHTYPE, TEXT("statictext_searchtype") );
		file.CreateControl( m_pEditbox_searched_itemtype, NPERSONALSHOPSEARCH_EDITBOX_SEARCHED_ITEMTYPE, TEXT("editbox_searched_itemtype") );
		file.CreateControl( m_pEditbox_itemsearchprice, NPERSONALSHOPSEARCH_EDITBOX_ITEMSEARCHPRICE, TEXT("editbox_itemsearchprice") );
		file.CreateControl( m_pStatictext_searchbelow, NPERSONALSHOPSEARCH_STATICTEXT_SEARCHBELOW, TEXT("statictext_searchbelow") );
		file.CreateControl( m_pListbox_shop_showcase, NPERSONALSHOPSEARCH_LISTBOX_SHOP_SHOWCASE, TEXT("listbox_shop_showcase") );
		file.CreateControl( m_pEditbox_item_explain, NPERSONALSHOPSEARCH_EDITBOX_ITEM_EXPLAIN, TEXT("editbox_item_explain") );
		file.CreateControl( m_pEditbox_shopper_location, NPERSONALSHOPSEARCH_EDITBOX_SHOPPER_LOCATION, TEXT("editbox_shopper_location") );
		file.CreateControl( m_pButton_personalshop_warp, NPERSONALSHOPSEARCH_BUTTON_PERSONALSHOP_WARP, TEXT("button_personalshop_warp") );
		file.CreateControl( m_pButton_minihome, NPERSONALSHOPSEARCH_BUTTON_MINIHOME, TEXT("button_minihome") );
		file.CreateControl( m_pButton_close, NPERSONALSHOPSEARCH_BUTTON_CLOSE, TEXT("button_close") );
		file.CreateControl( m_pCombobox_iteminfo_type, NPERSONALSHOPSEARCH_COMBOBOX_ITEMINFO_TYPE, TEXT("combobox_iteminfo_type") );
		file.CreateControl(	m_pImageStatic_Item, NPERSONALSHOPSEARCH_IMAGESTATIC_ITEM, TEXT("imagestatic_NONAME1")	);

		//m_pButton_minihome->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_WORDBREAK );
		//m_pButton_minihome->Refresh();

		if(m_pListbox_shop_showcase)
		{
			//ListView_SetExtendedListViewStyle( m_hUserShopShowcase, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_INFOTIP );
			TCHAR* pText = GetTxtFromMgr(2251);
			m_pListbox_shop_showcase->SetColumn( 0, 90, pText );
			//ListViewInsertColumn( m_hUserShopShowcase, 110, pText,  0, 1);
			pText = GetTxtFromMgr(2252);
			m_pListbox_shop_showcase->SetColumn( 1, 80, pText );
			//ListViewInsertColumn( m_hUserShopShowcase,  80, pText,  1, 1);
			pText = GetTxtFromMgr(1678);
			m_pListbox_shop_showcase->SetColumn( 2, 80, pText );
			//ListViewInsertColumn( m_hUserShopShowcase,  80, pText,      2, 2);
			pText = GetTxtFromMgr(1679);
			m_pListbox_shop_showcase->SetColumn( 3, 55, pText );
			//ListViewInsertColumn( m_hUserShopShowcase,  55, pText,      3, 2);
			m_pListbox_shop_showcase->SetColumn( 4, 18, TEXT("") );
			m_pListbox_shop_showcase->SetBorder( true );
			m_pListbox_shop_showcase->SetBKColorUse( true );
		}

		m_pListbox_shop_showcase->SetUseToolTip( true );
		m_pListbox_shop_showcase->Refresh();

		// 아이템 종류를 보여준다. 
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		this->SetItemTypeInCombo(false);
		
		//---------------------------------
		// INIT CONTROLS		   --------
		//---------------------------------
		m_pEditbox_price_condition->SetNumberMode( true );
		m_pEditbox_price_condition->SetMaxEditTextLength( 11 );

		TCHAR Tempbuffer[128];
		_i64tot(MAX_PRICE_IN_MARKET, Tempbuffer, 10);
		//SendMessage(GetDlgItem( m_hDlg, IDC_EDIT_SHOP_PRICE_CONDITION),   EM_LIMITTEXT, _tcslen(Tempbuffer), 0);
		// Default Gray
		if( m_pRadiobutton_sell->IsCheck() == false )
			m_pRadiobutton_sell->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON );
	
		// Clear
		m_pListbox_shop_showcase->Clear();
		m_pEditbox_item_explain->SetText(TEXT("") );
		m_pEditbox_shopper_location->SetText( TEXT("") );

		SetActionTimer( NPERSONALSHOPSEARCHDLG_SETTIMER_TIME );
		SetTimerOnOff( true );

		m_siImageIndex = -1;
		m_siImageType = -1;

		//m_bShow = false;
		//Hide();

		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pButton_minihome->Show(false);
		}
		// [성웅] 제조 의뢰 인터페이스 개선
		// 제조 의뢰 인터페이스가 들어가면 제작의뢰 radiobutton 은 안보이게 한다
		if ( pclClient->IsCountrySwitch( Switch_Change_manufacture_request_interface ) == true	) 
		{
			if(m_pRadiobutton_makeitem)
			{
				m_pRadiobutton_makeitem->Show(false)	;
			}
		}

	}
	else
	{
		DeleteDialog();
	}

	return;
}


void CALLBACK CNPersonalShopSearchDlg::StaticNPersonalShopSearchDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPersonalShopSearchDlg *pThis = (CNPersonalShopSearchDlg*) pControllerMgr;
	pThis->NPersonalShopSearchDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNPersonalShopSearchDlg::NPersonalShopSearchDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NPERSONALSHOPSEARCH_BUTTON_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				DeleteDialog();
				break;
			}
		}
		break;

	case NPERSONALSHOPSEARCH_LISTBOX_SHOP_SHOWCASE:
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

	case NPERSONALSHOPSEARCH_CHECKBOX_SHOP_PRICE:
		{
			switch ( nEvent )
			{
			case EVENT_CHECKBOX_CHANGED:
				{
					if(m_pCheckbox_shop_price->IsCheck() == false )
					{
						m_pEditbox_price_condition->Enable( false );
					}
					else
					{
						m_pEditbox_price_condition->Enable( true );
					}
				}
				break;
			}
		}
		break;

	case NPERSONALSHOPSEARCH_BUTTON_SEARCH:
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
	case NPERSONALSHOPSEARCH_BUTTON_MINIHOME:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_siShopShowCaseSelectedIndex >=0 &&
						m_siShopShowCaseSelectedIndex < MAX_SEARCHSHOP_ITEM_NUMBER) 
					{
						if ( _tcslen(m_clSearhShopData.m_szName[m_siShopShowCaseSelectedIndex])>0 )
						{
                            
							cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( m_clSearhShopData.m_szName[m_siShopShowCaseSelectedIndex] );
							cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
							cltClient *pclclient = ( cltClient * )pclClient;
							pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );

							/*
							cltGameMsgRequest_Userid clUserid( m_clSearhShopData.m_szName[m_siShopShowCaseSelectedIndex] );
							cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
							cltClient *pclclient = ( cltClient * )pclClient;
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
							*/
						}	
					}
				}
				break;
			}
		}
		break;
	case NPERSONALSHOPSEARCH_RADIOBUTTON_BUY:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_pRadiobutton_buy->IsCheck() == true )
					{
						TCHAR* pText = GetTxtFromMgr(2253);
						m_pCheckbox_shop_price->SetText( pText, DT_LEFT | DT_VCENTER );
						pText = GetTxtFromMgr(2254);
						m_pStatictext_searchbelow->SetText( pText, DT_LEFT | DT_VCENTER );
					}
				}
				break;
			}
		}
		break;

	case NPERSONALSHOPSEARCH_RADIOBUTTON_SELL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_pRadiobutton_sell->IsCheck() == true )
					{
						TCHAR* pText = GetTxtFromMgr(2253);
						m_pCheckbox_shop_price->SetText( pText, DT_LEFT | DT_VCENTER );
						pText = GetTxtFromMgr(2255);
						m_pStatictext_searchbelow->SetText( pText, DT_LEFT | DT_VCENTER );
					}
				}
				break;
			}
		}
		break;

	case NPERSONALSHOPSEARCH_RADIOBUTTON_MAKEITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR* pText = GetTxtFromMgr(2256);
					m_pCheckbox_shop_price->SetText( pText, DT_LEFT | DT_VCENTER );
					pText = GetTxtFromMgr(2254);
					m_pStatictext_searchbelow->SetText( pText, DT_LEFT | DT_VCENTER );
				}
				break;
			}
		}
		break;

	case NPERSONALSHOPSEARCH_BUTTON_PERSONALSHOP_WARP:
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
					if( pclclient->pclCM->CR[1]->CanWarpFrom() == false)
						return;

					// 레벨제한 (마을이 아닌 사냥터는 레벨제한이 있다). 
					if ( pclClient->pclMapManager->pclVillageManager->IsValidVillage(siMapIndex) == false )
					{
						if(pclClient->pclMapManager->IsEnterLevel(siMapIndex, pclchar->pclCI->clIP.GetLevel()) == false)
						{
							TCHAR* pTitle = GetTxtFromMgr(1413);
							TCHAR* pText = GetTxtFromMgr(2261);
							StringCchPrintf(buffer, 512, pText,
							pclClient->pclMapManager->GetLevelLimit(siMapIndex));
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
							return ;
						}
			 		}

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

bool CNPersonalShopSearchDlg::IsShow()
{
	return m_bShow;
}

void CNPersonalShopSearchDlg::Show()
{
	
	if(m_bShow == false)	CDialogController::Show( true );
	m_bShow = true;
	
	if(m_bTimer == false) 
	{
		SetActionTimer( NPERSONALSHOPSEARCHDLG_SETTIMER_TIME );
		SetTimerOnOff( true );
	}

	m_bTimer = true;

	m_LastSearchTime = pclClient->CurrentClock;

	return;
}

void CNPersonalShopSearchDlg::Hide()
{
	if(m_bShow==true)	CDialogController::Show( false );
	m_bShow = false;

	if(m_bTimer==true) SetTimerOnOff( false );
	m_bTimer = false;
}



bool CNPersonalShopSearchDlg::IsShopShowCaseChanged()
{
	if(m_bShopDataChanged) return true;

	return false;
}

void CNPersonalShopSearchDlg::SortSearchedData()
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

void CNPersonalShopSearchDlg::LoadShopShwocase()
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

	// Tooltip Setting!!
	//		case LVN_GETINFOTIP:
	//			{
	//				if ( ngt->iItem >= 0 )
	//				{
	//					SI32 siListViewIndex = 0;
	//					
	//					for ( int i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
	//					{
	//						if ( m_clSearhShopData.m_clItem[ i ].siUnique != 0 )
	//						{
	//							if ( ngt->iItem == siListViewIndex )
	//							{
	//								cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

	//								if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL) // 물건을 팔때는 정확한 정보를 보여주나.
	//									pclitemmanager->GetExplain( &m_clSearhShopData.m_clItem[ i ],          ngt->pszText );
	//								else								// 물건을 살때는 해당 물품만 산다.
	//									pclitemmanager->GetExplain( m_clSearhShopData.m_clItem[ i ].siUnique,  ngt->pszText );
	//								
	//								break;
	//							}
	//							else
	//							{
	//								++siListViewIndex;
	//							}
	//						}
	//					}							
	//				}
	//			}
	//			break;
	//		}
	//	}
	//}

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
				//ListViewInsertItem(m_hUserShopShowcase, -1 , i, 0, itemname, -1);
				tmpListBoxItemData.Set( 0, itemname );
				
				//ListViewInsertItem(m_hUserShopShowcase, -1 , i, 1, pclClient->pclMapManager->GetMapName(m_clSearhShopData.m_siMapIndex[i]), -1);
				tmpListBoxItemData.Set( 1, pclClient->pclMapManager->GetMapName(m_clSearhShopData.m_siMapIndex[i]) );
			
				TCHAR* pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(m_clSearhShopData.m_siPrice[i], itembuffer, 128, pText);
				tmpListBoxItemData.Set( 2, itembuffer, ((cltClient*)pclClient)->GetColorByPrice(m_clSearhShopData.m_siPrice[i]) );
				//ListViewInsertItem(m_hUserShopShowcase, -1 , i, 2, itembuffer, -1);

				pText = GetTxtFromMgr(2209);
				StringCchPrintf(itembuffer, 128, pText, m_clSearhShopData.m_clItem[ i ].siItemNum);
				tmpListBoxItemData.Set( 3, itembuffer );
				//ListViewInsertItem(m_hUserShopShowcase, -1 , i, 3, itembuffer, -1);

				//  Tooltip 셋팅!!
				if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL) // 물건을 팔때는 정확한 정보를 보여주나.
					pclitemmanager->GetExplain( &m_clSearhShopData.m_clItem[ i ],          pTmpChar );
				else								// 물건을 살때는 해당 물품만 산다.
					pclitemmanager->GetExplain( m_clSearhShopData.m_clItem[ i ].siUnique,  pTmpChar );

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
		m_pEditbox_searched_itemtype->SetText( itemname );
	}

	g_JYLibFunction.SetNumUnit(m_clSearhShopData.m_siItemSearchPrice, itembuffer, 128, TEXT(""));

	m_pEditbox_itemsearchprice->SetText( itembuffer, ((cltClient*)pclClient)->GetColorByPrice(m_clSearhShopData.m_siItemSearchPrice) );

	if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_BUY)
	{
		TCHAR* pText = GetTxtFromMgr(2263);
		m_pStatictext_searchtype->SetText( pText, DT_LEFT | DT_VCENTER );
		pText = GetTxtFromMgr(2254);
		m_pStatictext_searchbelow->SetText( pText, DT_LEFT | DT_VCENTER );
		pText = GetTxtFromMgr(1678);
		m_pListbox_shop_showcase->SetColumn( 2, 80, pText );
		//ListViewSetColumn( m_hUserShopShowcase,  80, pText,		2, 2);	
	}
	else if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL)
	{
		TCHAR* pText = GetTxtFromMgr(2264);
		m_pStatictext_searchtype->SetText( pText, DT_LEFT | DT_VCENTER );
		
		// [수정 : 황진성 2007.8.31]
		// 개인상점_물품 검색 창의 가격 조건의 조건 표시가 틀림
		// "골드이상" 을 검색하는데 판매물품라인 끝의 정보는 "골드이하" 라고 보여짐.
		//pText = GetTxtFromMgr(2255) -> pText = GetTxtFromMgr(2254); 수정
		pText = GetTxtFromMgr(2254);
		m_pStatictext_searchbelow->SetText( pText, DT_LEFT | DT_VCENTER );
		pText = GetTxtFromMgr(1678);
		m_pListbox_shop_showcase->SetColumn( 2, 80, pText );
		//ListViewSetColumn( m_hUserShopShowcase,  80, pText,		2, 2);	
	}
	else 
	{
		TCHAR* pText = GetTxtFromMgr(2265);
		m_pStatictext_searchtype->SetText( pText, DT_LEFT | DT_VCENTER );
		pText = GetTxtFromMgr(2254);
		m_pStatictext_searchbelow->SetText( pText, DT_LEFT | DT_VCENTER );
		pText = GetTxtFromMgr(2195);
		m_pListbox_shop_showcase->SetColumn( 2, 80, pText );
		//ListViewSetColumn( m_hUserShopShowcase,  80, pText, 2, 2);	
	}

	// EDIT CLEAR
	//SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_ITEM_EXPLAIN)      , TEXT("") );	
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

	//SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_SHOPPER_LOCATION)  , itembuffer);	
	m_pEditbox_shopper_location->SetText( itembuffer );
	return;
}

void CNPersonalShopSearchDlg::DisplaySowCaseItemInfo(SI32 index)
{
	TCHAR ItemExplain[ 512 ] = TEXT("");

	//==========================================
	// EDIT_ITEM_EXPLAIN
	//==========================================
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL) // 물건을 팔때는 정확한 정보를 보여주나.
		pclitemmanager->GetExplain( &m_clSearhShopData.m_clItem[ index ], ItemExplain );
	else								// 물건을 살때는 해당 물품만 산다.
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

void CNPersonalShopSearchDlg::SearchShopItem()
{
	// 300 ms 이하에서는 계속 검색을 하지 못하게 한다.
	if(TABS(pclClient->CurrentClock - m_LastSearchTime) < 500) return;
	m_LastSearchTime = pclClient->CurrentClock ;
	
	SI32 siShopMode = PSHOP_MODE_BUY;
	//if ( SendMessage( GetDlgItem( m_hDlg, IDC_RADIO_SHOP_TYPE_SELL ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	if( m_pRadiobutton_sell->IsCheck() == true )
	{
		siShopMode = PSHOP_MODE_SELL;
	}
	else if( m_pRadiobutton_buy->IsCheck() == true )
	//else if ( SendMessage( GetDlgItem( m_hDlg, IDC_RADIO_SHOP_TYPE_BUY), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		siShopMode = PSHOP_MODE_BUY;
	}
	else if( m_pRadiobutton_makeitem->IsCheck() == true )
	//else if ( SendMessage( GetDlgItem( m_hDlg, IDC_RADIO_SHOP_TYPE_MAKEITEM), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		siShopMode = PSHOP_MODE_MAKEITEM;
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

	//if(GetWindowText(GetDlgItem(m_hDlg, IDC_COMBO_ITEMINFO_TYPE), szSelText, 100))
	{
		itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);
	}
	
	if(itemtypeunique)
	{
		SI32 siPrice    = 0;
		//----------------------------------------
		// 아이템의 기타 정보를 표시한다 
		//----------------------------------------
		// 체크되어있다면 가격을 얻어온다.
		//if ( SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_SHOP_PRICE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		if(m_pCheckbox_shop_price->IsCheck() == true )
		{
			//siPrice = GetDlgItemInt( m_hDlg, IDC_EDIT_SHOP_PRICE_CONDITION,  NULL, FALSE );
			siPrice = _tstoi( m_pEditbox_price_condition->GetText() );

			if(siPrice <= 0) return;
		}

		// 검색 요청 
		cltGameMsgRequest_SearchPersonalShopItem clsearchdata( siShopMode, itemtypeunique, siPrice);
		cltMsg clMsg( GAMEMSG_REQUEST_SEARCHPERSONALSHOPITEM, sizeof( clsearchdata ), (BYTE*)&clsearchdata );
		cltClient *pclclient = ( cltClient * )pclClient;
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}

// cltItemManagerClient 에서 가져옴 ======================================================================
// 아이템 종류를 보여준다. 
bool CNPersonalShopSearchDlg::SetItemTypeInCombo( bool bshowall)
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

void CNPersonalShopSearchDlg::Action()
{
	if ( IsShopShowCaseChanged() )
	{
		LoadShopShwocase();
	}
}

void CNPersonalShopSearchDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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


void CNPersonalShopSearchDlg::SetItemImageData(SI32 ItemUnique)
{

	if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( ItemUnique, &m_siImageType, &m_siImageIndex ) )
	{}
	else
	{
		m_siImageType = -1;
		m_siImageIndex = -1;
	}

}


void CNPersonalShopSearchDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
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


//#endif
