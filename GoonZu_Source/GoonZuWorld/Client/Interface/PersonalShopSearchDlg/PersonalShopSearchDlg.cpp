#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "..\..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"

#include "..\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\NotifyMsgDlg\NotifyMsgDlg.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"


#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/Msg/MsgType-Personalshop.h"
#include "../../CommonLogic/Msg/MsgType-ItemMall.h"
#include "../../../lib/WebHTML/WebHTML.h"
#include "..\..\Client\Client.h"
#include "../PersonalShop/PersonalShopDlg.h"
#include "PersonalShopSearchDlg.h"
#include "..\..\Client\Music\Music.h"


extern cltCommonLogic* pclClient;

CPersonalShopSearchDlg	g_PersonalShopSearchDlg;
// TIMER
CPersonalShopSearchDlg::CPersonalShopSearchDlg()
{
	m_bShow = false;
	m_bTimer= false;

	m_hDlg = NULL;
	m_hUserShopShowcase = NULL;
	m_hEditBrush		= NULL;

	m_bShopDataChanged = false;
	
	memset(m_szAccountID, 0, sizeof(m_szAccountID));
	m_siShopShowCaseSelectedIndex			= -1;

	m_hWarpBitmap = NULL;
	
	Initialize();
}

CPersonalShopSearchDlg::~CPersonalShopSearchDlg()
{

}

void CPersonalShopSearchDlg::Initialize()
{
	return;
}

void CPersonalShopSearchDlg::Destroy()
{

	return;
}

void CPersonalShopSearchDlg::Create( HINSTANCE hInst, HWND hWnd )
{

	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_PERSONAL_SHOP_SEARCH), hWnd, StaticPersonalShopSearchDlgProc );
	m_bShow = false;
	Hide();

	return;
}


BOOL CALLBACK CPersonalShopSearchDlg::StaticPersonalShopSearchDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;
	return g_PersonalShopSearchDlg.PersonalShopSearchDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CPersonalShopSearchDlg::PersonalShopSearchDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	HWND hwnd = NULL;
	
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{

			if(pclClient->siServiceArea == ConstServiceArea_Netmarble)
			{
				HWND hTemp = GetDlgItem(hDlg, IDC_BUTTON_MINIHOME);
				ShowWindow(hTemp, SW_HIDE);
			}

			m_hDlg = hDlg;

			m_hUserShopShowcase = GetDlgItem( hDlg, IDC_LIST_SHOP_SHOWCASE );
			if(m_hUserShopShowcase )
			{
				ListView_SetExtendedListViewStyle( m_hUserShopShowcase, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_INFOTIP );
				char* pText = GetTxtFromMgr(2251);
				ListViewInsertColumn( m_hUserShopShowcase, 110, pText,  0, 1);
				pText = GetTxtFromMgr(2252);
				ListViewInsertColumn( m_hUserShopShowcase,  80, pText,  1, 1);
				pText = GetTxtFromMgr(1678);
				ListViewInsertColumn( m_hUserShopShowcase,  80, pText,      2, 2);
				pText = GetTxtFromMgr(1679);
				ListViewInsertColumn( m_hUserShopShowcase,  55, pText,      3, 2);
			}

			// 아이템 종류를 보여준다. 
			cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
			pclitemmanager->SetItemTypeInCombo(hDlg, IDC_COMBO_ITEMINFO_TYPE, false);
			
			//---------------------------------
			// INIT CONTROLS		   --------
			//---------------------------------
			char Tempbuffer[128];
			itoa(MAX_PRICE_IN_MARKET, Tempbuffer, 10);
			SendMessage(GetDlgItem( m_hDlg, IDC_EDIT_SHOP_PRICE_CONDITION),   EM_LIMITTEXT, strlen(Tempbuffer), 0);
			// Default Gray
			SendMessage(GetDlgItem( m_hDlg,  IDC_RADIO_SHOP_TYPE_BUY ),       BM_SETCHECK, BST_CHECKED,   0);
			SendMessage(GetDlgItem( m_hDlg,  IDC_RADIO_SHOP_TYPE_SELL ),      BM_SETCHECK, BST_UNCHECKED, 0);
			SendMessage(GetDlgItem( m_hDlg,  IDC_RADIO_SHOP_TYPE_MAKEITEM ),  BM_SETCHECK, BST_UNCHECKED, 0);
			
			// Clear
			ListView_DeleteAllItems( m_hUserShopShowcase );
			SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_ITEM_EXPLAIN)      , "" );	
			SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_SHOPPER_LOCATION)  , "" );	

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);


			m_hWarpBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/HitButton/PersonalShopWarp.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hWarpBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_PERSONALSHOP_WARP, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hWarpBitmap);
			}
			//---------------------------------
				
		}
		return TRUE;
	case WM_TIMER:
		{			
			if ( IsShopShowCaseChanged() )
			{
				LoadShopShwocase();
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_ITEM_EXPLAIN ) == (HWND)lParam ||
				GetDlgItem( hDlg, IDC_EDIT_SHOPPER_LOCATION ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;

	case WM_NOTIFY:
		{
			LPNMHDR						hdr;
			LPNMITEMACTIVATE			nia;
			LPNMLVGETINFOTIP			ngt;
			LPNMLISTVIEW				nlv;
			hdr = ( LPNMHDR )			lParam;
			ngt = ( LPNMLVGETINFOTIP )	lParam;
			nlv = ( LPNMLISTVIEW )		lParam;
			nia = (LPNMITEMACTIVATE)	lParam;
			
			if ( hdr->hwndFrom == m_hUserShopShowcase )
			{
				switch( hdr->code )
				{
				case LVN_ITEMCHANGED:
					{
						if(nlv->uChanged == LVIF_STATE && nlv->uNewState == (LVIS_SELECTED | LVIS_FOCUSED))
						{
							m_siShopShowCaseSelectedIndex = nia->iItem;
							
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
				case LVN_GETINFOTIP:
					{
						if ( ngt->iItem >= 0 )
						{
							SI32 siListViewIndex = 0;
							
							for ( int i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
							{
								if ( m_clSearhShopData.m_clItem[ i ].siUnique != 0 )
								{
									if ( ngt->iItem == siListViewIndex )
									{
										cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

										if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL) // 물건을 팔때는 정확한 정보를 보여주나.
											pclitemmanager->GetExplain( &m_clSearhShopData.m_clItem[ i ],          ngt->pszText );
										else								// 물건을 살때는 해당 물품만 산다.
											pclitemmanager->GetExplain( m_clSearhShopData.m_clItem[ i ].siUnique,  ngt->pszText );
										
										break;
									}
									else
									{
										++siListViewIndex;
									}
								}
							}							
						}
					}
					break;
				}
			}
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD( wParam ) )
			{
			case IDC_CHECK_SHOP_PRICE:
				{
					HWND hwnd;
					
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_SHOP_PRICE ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						hwnd = GetDlgItem(hDlg, IDC_EDIT_SHOP_PRICE_CONDITION); 
						EnableWindow(hwnd, FALSE);
					}
					else
					{
						hwnd = GetDlgItem(hDlg, IDC_EDIT_SHOP_PRICE_CONDITION); 
						EnableWindow(hwnd, TRUE);
					}
				}
				break;
			case IDC_BUTTON_PERSONALSHOP_SEARCH:
				{
					SearchShopItem();
				}
				break;
			case IDOK:	
				{
					Hide();	
				}
				break;

			case IDC_BUTTON_MINIHOME:
				{
					
					if( m_siShopShowCaseSelectedIndex >=0 &&
						m_siShopShowCaseSelectedIndex < MAX_SEARCHSHOP_ITEM_NUMBER) 
					{
						if ( strlen(m_clSearhShopData.m_szName[m_siShopShowCaseSelectedIndex])>0 )
						{
							cltGameMsgRequest_Userid clUserid( m_clSearhShopData.m_szName[m_siShopShowCaseSelectedIndex] );
							cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (char*)&clUserid );
							cltClient *pclclient = ( cltClient * )pclClient;
							pclclient->SendMsgToServer((char*)&clMsg);
						}	
					}
				}
				break;
			case IDC_RADIO_SHOP_TYPE_BUY:
				{
					char* pText = GetTxtFromMgr(2253);
					SetWindowText(GetDlgItem( m_hDlg,  IDC_CHECK_SHOP_PRICE), pText);
					pText = GetTxtFromMgr(2254);
					SetWindowText(GetDlgItem( m_hDlg,  IDC_STATIC_BELOW),     pText);
				}
				break;

			case IDC_RADIO_SHOP_TYPE_SELL:
				{
					char* pText = GetTxtFromMgr(2253);
					SetWindowText(GetDlgItem( m_hDlg,  IDC_CHECK_SHOP_PRICE), pText);
					pText = GetTxtFromMgr(2255);
					SetWindowText(GetDlgItem( m_hDlg,  IDC_STATIC_BELOW),     pText);
				}
				break;
			case IDC_RADIO_SHOP_TYPE_MAKEITEM:
				{
					char* pText = GetTxtFromMgr(2256);
					SetWindowText(GetDlgItem( m_hDlg,  IDC_CHECK_SHOP_PRICE), pText);
					pText = GetTxtFromMgr(2254);
					SetWindowText(GetDlgItem( m_hDlg,  IDC_STATIC_BELOW),     pText);
				}
				break;
			case IDC_BUTTON_PERSONALSHOP_WARP:
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

					SI32 siMapIndex = 0;
					SI32 siMapPosX = 0;
					SI32 siMapPosY = 0;
					char buffer[512]="";

					// 현재 선택된 상점을 확보한다. 
					if(m_siShopShowCaseSelectedIndex < 0 || m_siShopShowCaseSelectedIndex >= MAX_SEARCHSHOP_ITEM_NUMBER)
					{
						char* pTitle = GetTxtFromMgr(1413);
						char* pText = GetTxtFromMgr(2257);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
						return TRUE;
					}
					else
					{
						siMapIndex = m_clSearhShopData.m_siMapIndex[m_siShopShowCaseSelectedIndex];
						siMapPosX = m_clSearhShopData.m_siMapX[m_siShopShowCaseSelectedIndex];
						siMapPosY = m_clSearhShopData.m_siMapY[m_siShopShowCaseSelectedIndex];

						if( strlen(m_clSearhShopData.m_szName[m_siShopShowCaseSelectedIndex]) <= 0 || 
							(siMapIndex == 0 && siMapPosX == 0 && siMapPosY == 0))
						{
							char* pTitle = GetTxtFromMgr(1413);
							char* pText = GetTxtFromMgr(2257);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return TRUE;
						}
						
						if(siMapIndex< 0 || siMapIndex>= MAX_MAP_NUMBER)
						{
							char* pTitle = GetTxtFromMgr(1413);
							char* pText = GetTxtFromMgr(2257);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
							return TRUE;
						}
					}
					
					// 감옥에서는 워프 불가 
					if( pclClient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))	
					{
						char* pTitle = GetTxtFromMgr(1413);
						char* pText = GetTxtFromMgr(2257);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return TRUE ;
					}

					// 영자나라에 있으면 불가능
					if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
						pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
					{
						return TRUE ;
					}

					if ( siMapIndex == MAPINDEX_BATTLESEA ) // 문무대왕릉
					{
						char* pTitle = GetTxtFromMgr(1413);
						char* pText = GetTxtFromMgr(2260);
						sprintf(buffer, pText);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
						return TRUE;
					}

					// 탑승중이면 워프 불가. 
					if(pclchar->clInnerStatus.siRideShipID)
					{
						char* pTitle = GetTxtFromMgr(1413);
						char* pText = GetTxtFromMgr(1416);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return TRUE;
					}

					// 레벨제한 (마을이 아닌 사냥터는 레벨제한이 있다). 
					if ( pclClient->pclMapManager->pclVillageManager->IsValidVillage(siMapIndex) == false )
					{
						if ( pclClient->pclMapManager->GetLevelLimit(siMapIndex) > pclchar->pclCI->clIP.GetLevel() )
						{
							char* pTitle = GetTxtFromMgr(1413);
							char* pText = GetTxtFromMgr(2261);
							sprintf(buffer, pText,
							pclClient->pclMapManager->GetLevelLimit(siMapIndex));
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, buffer);
							return TRUE;
						}
			 		}

					char* pTitle = NULL ;
					char* pText = NULL ;
				
					if ( !pclchar->pclCI->clBI.bUseNoLimitTicketWarp &&
						 pclchar->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false )
					{
						// 순간이동이용권을 보유하고 있는지 확인한다. 
						SI32 itemunique = 14005 ;  // 순간이동이용권
						SI32 pos = pclchar->pclCI->clCharItem.FindItemInv(itemunique);
						if(pos < 0)
						{
							pTitle = GetTxtFromMgr(1413);
							pText = GetTxtFromMgr(3192);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);

							return TRUE;
						}

						pTitle = GetTxtFromMgr(1418);
						pText = GetTxtFromMgr(2262);

						sprintf(buffer, pText,
							pclClient->pclMapManager->GetMapName(siMapIndex),
							siMapPosX, siMapPosY);
					}
					else
					{
						pTitle = GetTxtFromMgr(3189);
						pText = GetTxtFromMgr(3193);

						sprintf(buffer, pText,
							pclClient->pclMapManager->GetMapName(siMapIndex),
							siMapPosX, siMapPosY);
					}

					if (  MessageBox(NULL, buffer, pTitle, MB_ICONQUESTION | MB_YESNO) == IDNO )
					{
						return TRUE;
					}
					
					
					cltGameMsgRequest_PersonalShopWarp pclInfo(siMapIndex, siMapPosX, siMapPosY);
					cltMsg clMsg( GAMEMSG_REQUEST_PERSONALSHOPWARP, sizeof( pclInfo ), (char*)&pclInfo );
					pclClient->SendMsgToServer((char*)&clMsg);
				}
				break;				
			}
		}
		break;
		
	case WM_CLOSE:
		{
			Hide();
		}
		break;

	case WM_DESTROY:
		{
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush= NULL;
			}
			if ( m_hWarpBitmap )
			{
				DeleteObject(m_hWarpBitmap);
				m_hWarpBitmap = NULL ;
			}

			if(m_bTimer==true) KillTimer( hDlg, 0 );			
			m_bTimer = false;
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

bool CPersonalShopSearchDlg::IsShow()
{
	return m_bShow;
}

void CPersonalShopSearchDlg::Show()
{
	
	if(m_hDlg == NULL)	 return;
	if(m_bShow == false)	ShowWindow( m_hDlg, SW_SHOWNOACTIVATE );
	m_bShow = true;
	
	if(m_bTimer == false) 	SetTimer( m_hDlg, 0, PERSONALSHOPSEARCHDLG_SETTIMER_TIME, NULL );
	m_bTimer = true;

	m_LastSearchTime = pclClient->CurrentClock;
	SetForegroundWindow( m_hDlg );

	/*
	///////////////////////////////////////////////
	// TEMP
	///////////////////////////////////////////////
	m_clSearhShopData.m_siShopMode = PSHOP_MODE_SELL;
	m_clSearhShopData.m_siItemMarketPrice = 10000;
	cltItem clItem;
	clItem.siUnique  = 14040;
	clItem.siItemNum = MAX_ITEM_PILE_NUMBER;
	for(SI32 i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i)
	{
		strcpy(m_clSearhShopData.m_szName[i], (char *)pclClient->pclCM->CR[1]->GetName());
		//strcpy(m_clSearhShopData.m_szName[i], "가나다라마바사아자9");
		strcpy(m_clSearhShopData.m_szName[i], "트로이의목마");
		m_clSearhShopData.m_siLevel[i] = pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel();
		m_clSearhShopData.m_siMapIndex[i] = pclClient->pclCM->CR[1]->GetMapIndex();
		m_clSearhShopData.m_siMapX[i] = pclClient->pclCM->CR[1]->GetX();
		m_clSearhShopData.m_siMapY[i] = pclClient->pclCM->CR[1]->GetY();

		m_clSearhShopData.m_clItem[i].Set(&clItem);
		m_clSearhShopData.m_siPrice[i] = MAX_PRICE_IN_MARKET;//rand()%(MAX_PRICE_IN_MARKET+1);
	}

	m_bShopDataChanged = TRUE;
	///////////////////////////////////////////////
	*/

	return;
}

void CPersonalShopSearchDlg::Hide()
{
	if(m_hDlg == NULL)	 return;

	if(m_bShow==true)	ShowWindow( m_hDlg, SW_HIDE);
	m_bShow = false;

	if(m_bTimer==true) KillTimer( m_hDlg, 0 );			
	m_bTimer = false;
}



bool CPersonalShopSearchDlg::IsShopShowCaseChanged()
{
	if(m_bShopDataChanged) return true;

	return false;
}

void CPersonalShopSearchDlg::SortSearchedData()
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
		SI32 siMinValue = 0;
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
		SI32 siMaxValue = 0;
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

void CPersonalShopSearchDlg::LoadShopShwocase()
{
	if ( m_hDlg == NULL )	{	return;		}

	//=================================================
	// 얻어온 데이터를 Sort 시킨다.
	//=================================================
	SortSearchedData();
	//=================================================

	m_siShopShowCaseSelectedIndex = -1;
	m_bShopDataChanged = false;

	char itemname[128]="";
	char itembuffer[128];
	SI32 siItemCount = 0;
	ListView_DeleteAllItems( m_hUserShopShowcase );

	for (SI32 i = 0; i < MAX_SEARCHSHOP_ITEM_NUMBER; ++i )
	{	
		if(m_clSearhShopData.m_clItem[i].siUnique)
		{
			if(m_clSearhShopData.m_clItem[ i ].GetFullName(pclClient->pclItemManager, itemname) == true)
			{
				ListViewInsertItem(m_hUserShopShowcase, -1 , i, 0, itemname, -1);
				
				ListViewInsertItem(m_hUserShopShowcase, -1 , i, 1, pclClient->pclMapManager->GetMapName(m_clSearhShopData.m_siMapIndex[i]), -1);
			
				char* pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(m_clSearhShopData.m_siPrice[i], itembuffer, pText);
				ListViewInsertItem(m_hUserShopShowcase, -1 , i, 2, itembuffer, -1);

				pText = GetTxtFromMgr(2209);
				sprintf(itembuffer, pText, m_clSearhShopData.m_clItem[ i ].siItemNum);
				ListViewInsertItem(m_hUserShopShowcase, -1 , i, 3, itembuffer, -1);

				siItemCount++;
			}
		}
	}
	
	if(m_clSearhShopData.m_siItemUniqueType)
	{
		strcpy(itemname,"");

		SI32 siItemUniqueType = m_clSearhShopData.m_siItemUniqueType;
		if(siItemUniqueType>0 && siItemUniqueType< MAX_ITEMTYPE_NUMBER)
		{
			if(pclClient->pclItemManager->pclItemTypeInfo[siItemUniqueType])
			{
				strcpy(itemname,pclClient->pclItemManager->pclItemTypeInfo[siItemUniqueType]->GetName());
			}
		}
		SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_SEARCHED_ITEMTYPE) , itemname );	
	}

	g_JYLibFunction.SetNumUnit(m_clSearhShopData.m_siItemSearchPrice, itembuffer,"");
	SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_ITEMSEARCHPRICE) ,	itembuffer);	

	if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_BUY)
	{
		char* pText = GetTxtFromMgr(2263);
		SetWindowText( GetDlgItem(m_hDlg, IDC_STATIC_SEARCHTYPE)  , pText );	
		pText = GetTxtFromMgr(2254);
		SetWindowText( GetDlgItem(m_hDlg, IDC_STATIC_SEARCHBELOW) ,	pText);	
		pText = GetTxtFromMgr(1678);
		ListViewSetColumn( m_hUserShopShowcase,  80, pText,		2, 2);	
	}
	else if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL)
	{
		char* pText = GetTxtFromMgr(2264);
		SetWindowText( GetDlgItem(m_hDlg, IDC_STATIC_SEARCHTYPE)  , pText);	
		pText = GetTxtFromMgr(2255);
		SetWindowText( GetDlgItem(m_hDlg, IDC_STATIC_SEARCHBELOW) ,	pText);	
		pText = GetTxtFromMgr(1678);
		ListViewSetColumn( m_hUserShopShowcase,  80, pText,		2, 2);	
	}
	else 
	{
		char* pText = GetTxtFromMgr(2265);
		SetWindowText( GetDlgItem(m_hDlg, IDC_STATIC_SEARCHTYPE)  , pText );
		pText = GetTxtFromMgr(2254);
		SetWindowText( GetDlgItem(m_hDlg, IDC_STATIC_SEARCHBELOW) ,	pText );	
		pText = GetTxtFromMgr(2195);
		ListViewSetColumn( m_hUserShopShowcase,  80, pText, 2, 2);	
	}

	// EDIT CLEAR
	SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_ITEM_EXPLAIN)      , "" );	

	// 검색 완료 메시지 
	pclClient->PushEffect(EFFECT_SUCCESS, 0, 0);
	if(siItemCount <1)
	{
		char* pText = GetTxtFromMgr(2266);
		sprintf(itembuffer, pText,   itemname);
	}	
	else if(siItemCount < MAX_SEARCHSHOP_ITEM_NUMBER)
	{
		char* pText = GetTxtFromMgr(2267);
		sprintf(itembuffer, pText,    itemname, siItemCount);
	}
	else
	{
		char* pText = GetTxtFromMgr(2268);
		sprintf(itembuffer, pText, itemname);
	}

	SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_SHOPPER_LOCATION)  , itembuffer);	
	return;
}

void CPersonalShopSearchDlg::DisplaySowCaseItemInfo(SI32 index)
{
	char ItemExplain[ 512 ] = "";

	//==========================================
	// IDC_EDIT_ITEM_EXPLAIN
	//==========================================
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	if(m_clSearhShopData.m_siShopMode == PSHOP_MODE_SELL) // 물건을 팔때는 정확한 정보를 보여주나.
		pclitemmanager->GetExplain( &m_clSearhShopData.m_clItem[ index ], ItemExplain );
	else								// 물건을 살때는 해당 물품만 산다.
		pclitemmanager->GetExplain( m_clSearhShopData.m_clItem[ index ].siUnique, ItemExplain );
	
	SI32 silen = strlen(ItemExplain);
	if( silen > 2 && ItemExplain[silen-1] =='\n') {	ItemExplain[silen-1] = 0;	}
	if( silen > 2 && ItemExplain[silen-2] =='\r') {	ItemExplain[silen-2] = 0;	}
	SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_ITEM_EXPLAIN) , ItemExplain );

	//==========================================
	// IDC_EDIT_SHOPPER_LOCATION
	//==========================================
	char szNearInfo[128];
	((cltClient *)pclClient)->GetNearMapInfo(m_clSearhShopData.m_siMapIndex[index], m_clSearhShopData.m_siMapX[index], m_clSearhShopData.m_siMapY[index], szNearInfo);

	char* pText = GetTxtFromMgr(2269);

	sprintf(ItemExplain, pText,
			m_clSearhShopData.m_szName[index], m_clSearhShopData.m_siLevel[index],
			pclClient->pclMapManager->GetMapName(m_clSearhShopData.m_siMapIndex[index]),
			m_clSearhShopData.m_siMapX[index], m_clSearhShopData.m_siMapY[index],
			szNearInfo,
			m_clSearhShopData.m_siHandPoint[index], 
			m_clSearhShopData.m_siSkillLevel[index] 
			);

	SetWindowText( GetDlgItem(m_hDlg, IDC_EDIT_SHOPPER_LOCATION)  , ItemExplain );	

}

void CPersonalShopSearchDlg::SearchShopItem()
{
	// 300 ms 이하에서는 계속 검색을 하지 못하게 한다.
	if(TABS(pclClient->CurrentClock - m_LastSearchTime) < 500) return;
	m_LastSearchTime = pclClient->CurrentClock ;
	
	SI32 siShopMode = PSHOP_MODE_BUY;
	if ( SendMessage( GetDlgItem( m_hDlg, IDC_RADIO_SHOP_TYPE_SELL ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		siShopMode = PSHOP_MODE_SELL;
	}
	else if ( SendMessage( GetDlgItem( m_hDlg, IDC_RADIO_SHOP_TYPE_BUY), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		siShopMode = PSHOP_MODE_BUY;
	}
	else if ( SendMessage( GetDlgItem( m_hDlg, IDC_RADIO_SHOP_TYPE_MAKEITEM), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
	{
		siShopMode = PSHOP_MODE_MAKEITEM;
	}
	
	char szSelText[128];
	SI32 itemtypeunique = 0;
	if(GetWindowText(GetDlgItem(m_hDlg, IDC_COMBO_ITEMINFO_TYPE), szSelText, 100))
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
		if ( SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_SHOP_PRICE ), BM_GETCHECK, 0, 0 ) == BST_CHECKED )
		{
			siPrice = GetDlgItemInt( m_hDlg, IDC_EDIT_SHOP_PRICE_CONDITION,  NULL, FALSE );
		}

		// 검색 요청 
		cltGameMsgRequest_SearchPersonalShopItem clsearchdata( siShopMode, itemtypeunique, siPrice);
		cltMsg clMsg( GAMEMSG_REQUEST_SEARCHPERSONALSHOPITEM, sizeof( clsearchdata ), (char*)&clsearchdata );
		cltClient *pclclient = ( cltClient * )pclClient;
		pclclient->SendMsgToServer((char*)&clMsg);
	}

}
