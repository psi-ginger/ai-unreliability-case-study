#define STRICT
#include "../Client/Client.h"

#include <Mouse.h>
#include <mainfunction.h>
#include "../Client/Music/Music.h"
#include "../Lib/Sound/SoundMgr.h"
#include "../MapEdit/mapedit.h"
#include "../MapEdit/mapedit.h"
#include "../resource.h"

#include <zmouse.h>
#include "Map/Map.h"
#include "Ask/Ask.h"
#include "Bullet/bullet.h"
#include "Smoke/Smoke.h"
#include "master/master.h"
#include "../Client/NInterface/NMaster/NMaster.h"
#include "Char/CharManager/CharManager.h"

#include "Char/parametaBox/ParametaBox.h"
#include "Util/Util.h"
#include "../Client/Inventory/ItemInventory.h"
#include "../Client/Interface/NPC1/NPCTradeDlg.h"
#include "../Client/Interface/LoginNoticeDlg/LoginNoticeDlg.h"
#include "../Client/NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

//#include "../Client/Interface/ForeignMerchant/ForeignMerchantDlg.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../Client/NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"


#include "../Client/Interface/ForeignMerchant/ForeignMerchantBuyCashItemDlg.h"
#include "../Client/Cursor/Cursor.h"
#include "Char/CharCommon/Char-Common.h"
#include "../Client/Interface/RichEditChatMgrDlg/RichEditChatMgrDlg.h"
#include "../Client/Interface/FriendDlg/FriendDlg.h"
#include "../Client/NInterface/NFriendDlg/NFriendDlg.h"
#include "../Client/NInterface/NStatusBarDlg/NStatusBarDlg.h"
#include "../Client/NInterface/NChatBarDlg/NChatBarDlg.h"

#include "../Client/Interface/ResidentsViewDlg/ResidentsViewDlg.h"
#include "../Client/Interface/OnGameNetEnviroment/OnGameNetEnviromentDlg.h"
//#include "../Client/Interface/PersonalShop/PersonalShopDlg.h"
#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/Interface/NoticeEventMsgDlg/NoticeEventMsgDlg.h"
#include "../Client/Interface/TestDlg/TestDlg.h"
#include "../Client/Interface/NInventory/NInventory.h"
#include "../Client/NInterface/NNewInventory/NNewInventory.h"
#include "../Client/ninterface/ncharstatus/NEquipWindow.h"
#include "../Client/Interface/NExitDlg/NExitDlg.h"
#include "../Client/Interface/MiniGameDlg/MiniGameDlg.h"

#include "../Client/Inventory/UserInventoryDlg.h"
#include "../Client/Inventory/ItemInventory.h"
#include "../Client/StatusBar/ChatStatusBar/ChatStatusBar.h"
#include "../Client/MiniHomeUpdates/PopupCheck.h"

#include "SystemNPC/ForeignMerchantNPC/ForeignMerchantNPC.h"

#include "Msg/MsgType-Horse.h"
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

//#include "School/School.h"
#include "Father/Father.h"

#include "../Client/MiniGame/MiniGameManager.h"
#include "../Client/NInterface/NMagicInfoDlg/MagicInfoDlg.h"
#include "../Client/NInterface/NMagicInfoDlg/NewMagicInfoDlg.h"

#include "../Client/NInterface/NCharStatus/NCharStatus.h"
#include "../Client/NInterface/NCharStatus/NCharSummon.h"
#include "../Client/NInterface/NChatDlg/NChatDlg.h"
#include "../Client/NInterface/NSummonUnitBuyNpcDlg/NSummonUnitBuyNpcDlg.h"
#include "../Client/NInterface/NResidentsDlg/NResidentsDlg.h"

#include "../Client/NInterface/NCommonMsgDlg/NCommonMsgDlg.h"

#include "../Client/ninterface/npartydlg/NPartyDlg.h"
#include "../Client/ninterface/nnoticeadmindlg/NNoticeAdminDlg.h"
#include "../Client/ninterface/NAttendCheck/NAttendNoticeDlg.h"
#include "../Client/ninterface/NQuestInfoDlg/NQuestInfoDlg_Parent.h"
#include "../Client/ninterface/NQuestNoticeDlg/NQuestNoticeDlg.h"

#include "../Client/ninterface/NMykey/NMykey.h"
#include "../Client/ninterface/NMykey/NMykeyMini.h"
#include "../Client/ninterface/NCharRoulette/NCharRoulette.h"
#include "../Client/ninterface/NKillSignDlg/NKillSignDlg.h"
#include "../Client/ninterface/NGuildWarResult/NGuildWarResult.h"

#include "../Client/GawiBawiBoMgr/GawiBawiBoMgr_Client.h"

#include "../Client/ninterface/NMainMapDlg/NMainMapDlg.h"


// 운영자메뉴
#include "ninterface/ngamemasterdlg/NGameMaster.h"

#include "../Client/FeastEffect/FeastEffectManager.h"

#include "../CommonLogic/Msg/MsgType-Summon.h"
#include "../CommonLogic/Msg/Msgtype-itemmall.h"
#include "../CommonLogic/Msg/MsgType-SoulGuard.h"


extern DWORD dwQuitTimer;
extern cltCommonLogic* pclClient;

_InputDevice					IpD;
extern	bool					bActiveSwitch;							

//--------------------------------------------------------------------------
// Name: WindowProc()
// Desc: 윈도우 Procedure
//-------------------------------------------------------------------------- 
LRESULT WINAPI WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HMENU hMenu;
	static int FirstSwitch=TRUE;
	static bool bFlashWindowStatus	 = false;
	SHORT oldkeystatus=0;
	
	//KHY - 0701 - 원키 이벤트 (F1~F12)는 0.3초에 한번씩만 작동하도록 한다.
	static DWORD dwLastUserKeyEventClock = 0;

	if ( pclClient && pclClient->GameMode == GAMEMODE_CLIENT )
	{
		cltClient *pclclient = (cltClient *)pclClient;

		if ( pclclient->m_pInterfaceMgr && pclclient->m_pInterfaceMgr->MsgProc( message, wParam, lParam ) )
		{
			return FALSE;
		}
	}
	
	switch(message)
	{
		// 클라이언트 영역밖에서는 커서가 보인다. 		
	
		
	case WM_SETCURSOR:
		ShowHardwareCursor(TRUE);
		
		if(pclClient && pclClient->pclCursor)
		{
			pclClient->pclCursor->SetCursor( );
		}
		
		
		
		break;
		
	case WM_TIMER:
		
		// 상태란에 정보를 업데이트 한다. 
		if(pclClient)
		{
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				HDC hdc;
				hdc = GetDC(hWnd);
				if(hdc)
				{
					((cltServer*)pclClient)->DrawServerInfo(hdc);
				}

				ReleaseDC(hWnd, hdc);
			}
			else if ( pclClient->GameMode == GAMEMODE_CLIENT )
			{
				cltClient *pclclient = ( cltClient *) pclClient;

				HWND hFocusWnd = GetForegroundWindow();

				if ( hFocusWnd != NULL )
				{
					if ( GetWindow( hFocusWnd, GW_OWNER ) == hWnd || hFocusWnd == hWnd )
					{
						pclclient->bWindowNotice = false;
						bFlashWindowStatus = false;
					}
				}

				if ( pclclient->bWindowNotice )
				{
					if ( ! bFlashWindowStatus )
					{
						FlashWindow( hWnd, TRUE );
						bFlashWindowStatus = !bFlashWindowStatus;
					}
					else
					{
						FlashWindow( hWnd, FALSE );
						bFlashWindowStatus = !bFlashWindowStatus;
					}
				}

				if ( pclclient->GetGameStatus() != GS_GAME )
				{
					clock_t CurClock = clock();
					pclclient->m_pInterfaceMgr->Action( CurClock );
				}
			}
		}
		break;


	case WM_MOUSEMOVE:
		if(pclClient)
		{
			if(pclClient->GameMode != GAMEMODE_SERVER)
			{
 				((cltClient*)pclClient)->MouseMoveAction(LOWORD(lParam), HIWORD(lParam));
			}
		}	

		return 0;
	case WM_LBUTTONDBLCLK:			
		if(pclClient)
		{
			if(pclClient->GetFrame() > 40 )
			{
				cltKey clkey(KEY_MODE_FALSE, KEY_MODE_FALSE, KEY_MODE_TRUE, false, IpD.Mouse_X, IpD.Mouse_Y, 0);
				pclClient->PushKey(&clkey);
			}

		}			
		//IpD.DoubleLeftPressDoneSwich= FALSE;				
		IpD.DoubleLeftPressSwich = TRUE;
		return 0;

	case WM_LBUTTONDOWN:
		if(pclClient)
		{

			if(pclClient->GetFrame() > 40 )
			{
				cltKey clkey(KEY_MODE_TRUE, KEY_MODE_FALSE, KEY_MODE_FALSE,false, IpD.Mouse_X, IpD.Mouse_Y, 0);
				pclClient->PushKey(&clkey);

			}

			if ( pclClient->pclCursor )				
			{
				pclClient->pclCursor->SetCursor( GAME_CURSOR_CLICK );
			}
		}		
		IpD.LeftPressSwitch=TRUE;
		return 0;
	
	case WM_LBUTTONUP:
		if(pclClient)
		{
			if(pclClient->GetFrame() > 40 )
			{
				cltKey clkey(KEY_MODE_FALSE, KEY_MODE_FALSE,KEY_MODE_FALSE, false, IpD.Mouse_X, IpD.Mouse_Y, 0);
				pclClient->PushKey(&clkey);
			}

			if ( pclClient->pclCursor )				
			{
				pclClient->pclCursor->SetCursor( GAME_CURSOR_NORMAL );
			}

		}	

		
		
		if(IpD.DoubleLeftPressSwich == TRUE)
		{
			IpD.DoubleLeftPressSwich= FALSE;
			IpD.DoubleLeftPressDoneSwich = TRUE;
		}
		else
		{
			IpD.LeftPressSwitch=FALSE;
			IpD.LeftPressDoneSwitch=TRUE;
		}
		

		IpD.PrevLeftUp2Clock = IpD.PrevLeftUp1Clock;
		IpD.PrevLeftUp1Clock = pclClient->CurrentClock;

		return 0;
	case WM_RBUTTONDOWN:
		if(pclClient)
		{
			if(pclClient->GetFrame() > 40 )
			{
				cltKey clkey(KEY_MODE_FALSE, KEY_MODE_TRUE,KEY_MODE_FALSE, false, IpD.Mouse_X, IpD.Mouse_Y, 0);
				pclClient->PushKey(&clkey);

			}

		}
		IpD.RightPressSwitch=TRUE;

		return 0;
	case WM_RBUTTONUP:
		if(pclClient)
		{
			if(pclClient->GetFrame() > 40 )
			{
				cltKey clkey(KEY_MODE_FALSE, KEY_MODE_FALSE,KEY_MODE_FALSE, false, IpD.Mouse_X, IpD.Mouse_Y, 0);
				pclClient->PushKey(&clkey);
			}
		}
		IpD.RightPressSwitch=FALSE;
		IpD.RightPressDoneSwitch=TRUE;

		return 0;


	case WM_NOTIFY:
		{
			LPNMTOOLBAR htb;
			htb = (LPNMTOOLBAR)lParam;
			switch(( (LPNMHDR)lParam)->code)
			{
			case TTN_NEEDTEXT:
				{
					((LPTOOLTIPTEXT)lParam)->lpszText = pclClient->GetToolTip(wParam);
				}
				break;
			case TBN_DROPDOWN:
				{
					HMENU hMenu = NULL, hPopup= NULL;
					HWND  hToolbar = NULL;					
					RECT  rc;
					POINT po;

					po.x = 0;
					po.y = 0;

					SI32 siMenuID = 0;
					//================================
					// POPMENU RESOURCE를 선택한다.
					//================================
					switch(htb->iItem)
					{
					case ID_TOOLBAR_PERSONALSHOP:	siMenuID = IDR_MENU_PERSONALSHOP;	break;
					case ID_TOOLBAR_HOMEPAGE:		siMenuID = IDR_MENU_INTERNET;		break;
					}
					//================================
					// MENU가 있다면 Load한후에 설정을 한다.
					//================================
					if(siMenuID)
					{
						// load and display popup menu
						hToolbar = ((cltClient *)pclClient)->pclToolBar->hToolBar;
						hMenu = LoadMenu(((cltClient *)pclClient)->GetInstance(), MAKEINTRESOURCE(siMenuID));
						hPopup= GetSubMenu(hMenu, 0);
						SendMessage(hToolbar, TB_GETRECT, htb->iItem, (LPARAM)&rc);
						po.x = rc.left;	po.y = rc.bottom;
						ClientToScreen(hToolbar, &po);
					}
					//================================
					// POPUP MENU에 BITMAP을 붙인다.
					//================================
					if(hPopup)
					{
						switch(htb->iItem)
						{
						case ID_TOOLBAR_PERSONALSHOP:
							{
								HBITMAP hBitmap = ((cltClient *)pclClient)->pclToolBar->hBmpFindShopItem;
								SetMenuItemBitmaps(hPopup, ID_MENUITEM_SHOPSEARCH, MF_BYCOMMAND, hBitmap, hBitmap);
							}
							break;
						case ID_TOOLBAR_HOMEPAGE:
							{
								HBITMAP *hBitmap = ((cltClient *)pclClient)->pclToolBar->hBmpInternetHome;
								SetMenuItemBitmaps(hPopup, ID_MENUITEM_MYGOMZI,			MF_BYCOMMAND, hBitmap[0], hBitmap[0]);
								SetMenuItemBitmaps(hPopup, ID_MENUITEM_NEWS,			MF_BYCOMMAND, hBitmap[1], hBitmap[1]);
								SetMenuItemBitmaps(hPopup, ID_MENUITEM_KNOWLEDGE,		MF_BYCOMMAND, hBitmap[2], hBitmap[2]);
								SetMenuItemBitmaps(hPopup, ID_MENUITEM_SARANGBANG,		MF_BYCOMMAND, hBitmap[3], hBitmap[3]);
								//SetMenuItemBitmaps(hPopup, ID_MENUITEM_HYUNGZO,			MF_BYCOMMAND, hBitmap[4], hBitmap[4]);
								SetMenuItemBitmaps(hPopup, ID_MENUITEM_VILLAGERESIDENT,	MF_BYCOMMAND, hBitmap[5], hBitmap[5]);
								SetMenuItemBitmaps(hPopup, ID_MENUITEM_MARKET,			MF_BYCOMMAND, hBitmap[6], hBitmap[6]);
							}
							break;
						}
					}
					//================================
					// POPUPMENU를 화면에 보여준다.
					//================================
					if(siMenuID && hToolbar && hMenu && hPopup) // | TPM_VERTICAL
					{
						TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_LEFTBUTTON , po.x, po.y, 0, hToolbar, &rc);
						DestroyMenu(hMenu);
					}

					return 0;
				}
				break;
			}
		}
		break;
		//		return 0;

	case WM_COMMAND:
		{
			if(pclClient == NULL)break;

			if(pclClient->GameMode == GAMEMODE_SERVER )
			{
				cltServer *pclserver = (cltServer*)pclClient;
				switch(LOWORD(wParam))
				{
				case ID_SHORT_VK_F1:	break;
				case ID_SHORT_VK_F2:	break;
				case ID_SHORT_VK_F3:	break;
				case ID_SHORT_VK_F4:	break;
				case ID_SHORT_VK_F5:	break;
				case ID_SHORT_VK_F6:	break;
				case ID_SHORT_VK_F7:	break;
				case ID_SHORT_VK_F8:	
					pclserver->CreateDevCharInfoDlg();
				case ID_SHORT_VK_F9:	break;
				case ID_SHORT_VK_F11:	
					pclserver->CreateDevProfileInfoDlg();
					break;
				case ID_SHORT_VK_F12:	
					pclserver->CreateDevCharInfoDlg();
					break;
				case ID_SHORT_CTRL_1:	break;
				case ID_SHORT_CTRL_2:	break;
				case ID_SHORT_CTRL_3:	break;
				case ID_SHORT_CTRL_4:	break;
				case ID_SHORT_CTRL_5:	break;
				case ID_SHORT_CTRL_6:	break;
				case ID_SHORT_CTRL_7:	break;
				case ID_SHORT_CTRL_8:	break;
				case ID_SHORT_CTRL_9:	break;
				case ID_SHORT_CTRL_0:	break;
				case ID_SHORT_CTRL_M:	break;	// 전체맵열기
				case ID_SHORT_TAB:		break;
				case ID_SHORT_ALT_O:	break;
				case ID_SHORT_SHIFT_TAB: break;
				case ID_SHORT_ALT_W:	break;
				case ID_SHORT_ALT_M:	break;
				case ID_SHORT_ALT_A:	break;
				case ID_SHORT_ALT_I:	break;
				case ID_SHORT_ALT_S:	break;
				case ID_SHORT_ALT_C:	break;
				case ID_SHORT_ALT_V:	break;
				case ID_SHORT_ALT_F:	break;
				case ID_SHORT_ALT_T:	break;
				case ID_SHORT_ALT_Q:	break;
				case ID_SHORT_ALT_E:	break;
				case ID_SHORT_ALT_P:	break;
				case ID_SHORT_ALT_H:	break;
				case ID_SHORT_ALT_L:	break;
				//case ID_SHIFT_B:		break;
				case ID_SHORT_CTRL_Z:	break;
				case ID_SHORT_ALT_X:	break;
					
					
					
				default:
					pclserver->DoMsg_WM_COMMAND(wParam);
					break;
				}

			}
			else if ( pclClient->GameMode == GAMEMODE_CLIENT )
			{
				cltClient *pclclient = (cltClient *)pclClient;
				//KHY - 0701 - 원키 이벤트 (F1~F12)는 0.3초에 한번씩만 작동하도록 한다.
				DWORD nowClock = clock();
				bool bTrueKey = false;

				if(TABS(nowClock - dwLastUserKeyEventClock) > 300 || UserdoubleKeyEvent((SI32)LOWORD(wParam)))
				{
					dwLastUserKeyEventClock = nowClock;
					bTrueKey = true;
				}

				if(bTrueKey)
				{
					switch(LOWORD(wParam))
					{
					case ID_SHORT_VK_F1:
						{
	#ifndef _NEW_INTERFACE_
							//((CNInventory*)pclclient->m_pDialog[ NINVENTORY_DLG ])->UseItem( PERSONITEM_INV0 );
	#else
							SI16 siUnique = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_INV0].siUnique;
							if( ((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->CheckCanEquipItem( siUnique ) )
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->EquipItem( PERSONITEM_INV0 );
							}
							else
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->UseItem( PERSONITEM_INV0 );
							}
	#endif
						}
						break;
					case ID_SHORT_VK_F2:
						{
	#ifndef _NEW_INTERFACE_
							//((CNInventory*)pclclient->m_pDialog[ NINVENTORY_DLG ])->UseItem( PERSONITEM_INV1 );
	#else
							SI16 siUnique = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_INV0+4].siUnique;
							if( ((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->CheckCanEquipItem( siUnique ) )
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->EquipItem( PERSONITEM_INV0+4 );
							}
							else
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->UseItem( PERSONITEM_INV0+4 );
							}

	#endif
							
						}
						break;
					case ID_SHORT_VK_F3:
						{
	#ifndef _NEW_INTERFACE_
							//((CNInventory*)pclclient->m_pDialog[ NINVENTORY_DLG ])->UseItem( PERSONITEM_INV2 );
	#else
							SI16 siUnique = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_INV0 + 8].siUnique;
							if( ((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->CheckCanEquipItem( siUnique ) )
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->EquipItem( PERSONITEM_INV0 + 8);
							}
							else
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->UseItem( PERSONITEM_INV0 + 8);
							}
	#endif
						}
						break;
					case ID_SHORT_VK_F4:
						{
	#ifndef _NEW_INTERFACE_
							//((CNInventory*)pclclient->m_pDialog[ NINVENTORY_DLG ])->UseItem( PERSONITEM_INV3 );
	#else
							SI16 siUnique = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_INV0+12].siUnique;
							if( ((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->CheckCanEquipItem( siUnique ) )
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->EquipItem( PERSONITEM_INV0+12 );
							}
							else
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->UseItem( PERSONITEM_INV0+12 );
							}
	#endif
						}
						break;
					case ID_SHORT_VK_F5:
						{
	#ifndef _NEW_INTERFACE_
							//((CNInventory*)pclclient->m_pDialog[ NINVENTORY_DLG ])->UseItem( PERSONITEM_INV4 );
	#else
							SI16 siUnique = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[PERSONITEM_INV0 + 16].siUnique;
							if( ((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->CheckCanEquipItem( siUnique ) )
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->EquipItem( PERSONITEM_INV0 + 16);
							}
							else
							{
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->UseItem( PERSONITEM_INV0 + 16 );
							}
	#endif
						}
						break;

					case ID_SHORT_VK_F6:
						{
							if( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ] == NULL )
								pclclient->CreateInterface( CLTNCHARSTATUS_DLG );

							if(((cltNCharStatus*)(pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
								((cltNCharStatus*)(pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->SendSummonTakeInOutMessageToServer( 0 ); 
						}
						break;
					case ID_SHORT_VK_F7:	
						{
							if( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ] == NULL )
								pclclient->CreateInterface( CLTNCHARSTATUS_DLG );

							if(((cltNCharStatus*)(pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
								((cltNCharStatus*)(pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->SendSummonTakeInOutMessageToServer( 1 ); 
						}
						break;
					case ID_SHORT_VK_F8:	
						{
							if( pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ] == NULL )
								pclclient->CreateInterface( CLTNCHARSTATUS_DLG );

							if(((cltNCharStatus*)(pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2)
								((cltNCharStatus*)(pclclient->m_pDialog[ CLTNCHARSTATUS_DLG ]))->m_pTabDlg2->SendSummonTakeInOutMessageToServer( 2 ); 
						}
						break;
					case ID_SHORT_VK_F9:
						{
							if ( pclclient->pclCM->CR[1]->GetLife() > 0 )
								pclclient->RideOrUnrideHorse();
						}
						break;
							
					case ID_SHORT_VK_F10:	
						// 운영자 명령어 리스트를 보여준다. 
						if(pclClient->pclCM)
						{
							if(pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
							{
								if( pclclient->m_pDialog[ NGAMEMASTERORDER_DLG ] == NULL )
									pclclient->CreateInterface( NGAMEMASTERORDER_DLG );

								if ( ((CNGameMaster*)pclclient->m_pDialog[ NGAMEMASTERORDER_DLG ])->IsShow() == false )
								{
									((CNGameMaster*)pclclient->m_pDialog[ NGAMEMASTERORDER_DLG ])->Show();
								}
								else
								{
									((CNGameMaster*)pclclient->m_pDialog[ NGAMEMASTERORDER_DLG ])->Hide();
								}
	#ifdef _DEBUG						
								/*
								cltGameMsgRequest_GachaPage_URL clinfo( pclclient->szID, pclclient->szPassword );
								cltMsg clMsg(GAMEMSG_REQUEST_GACHAPAGE_URL,sizeof(clinfo),(BYTE*)&clinfo);
								pclclient->SendMsgToServer( (sPacketHeader*)&clMsg);
								*/
	#endif
							}
						}

						break;
					case ID_SHORT_VK_F11:	
						{
							if(pclClient->pclCM)
							{
								if(pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
								{
									pclClient->CreateDevProfileInfoDlg();
								}
							}

							if(pclClient->pclCM)
							{
								if(pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
								{
									pclClient->CreateDevCharInfoDlg();
								}
							}
						}
						break;

					case ID_SHORT_VK_F12:	
						{
							if(pclClient->pclCM)
							{
								if(pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
								{
									pclClient->CreateDevCharInfoDlg();
								}
							}
						}
						break;

					case ID_SHORT_CTRL_1:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(1);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(1);	
							}
						}
						break;
					case ID_SHORT_CTRL_2:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(2);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(2);	
							}
						}
						break;
					case ID_SHORT_CTRL_3:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(3);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(3);	
							}
						}
						break;
					case ID_SHORT_CTRL_4:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(4);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(4);	
							}
						}
						break;
					case ID_SHORT_CTRL_5:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(5);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(5);	
							}
						}
						break;
					case ID_SHORT_CTRL_6:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(6);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(6);	
							}
						}
						break;
					case ID_SHORT_CTRL_7:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(7);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(7);	
							}
						}
						break;
					case ID_SHORT_CTRL_8:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(8);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(8);	
							}
						}
						break;
					case ID_SHORT_CTRL_9:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(9);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(9);	
							}
						}
						break;
					case ID_SHORT_CTRL_0:
						{
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if( pclclient->m_pDialog[ NNEWMAGICINFO_DLG ] )
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->SetNewMagic(0);	
							}
							else
							{							
								if( pclclient->m_pDialog[ NMAGICINFO_DLG ] )
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->SetNewMagic(0);	
							}
						}
						break;
					case ID_SHORT_CTRL_M:	// 전체맵열기
						{
							if( pclclient->m_pDialog[ NMAINMAP_DLG ] )
							{
								if ( true == ((CMainMapDlg*)pclclient->m_pDialog[ NMAINMAP_DLG ])->IsShow() )
								{
									//((CMainMapDlg*)pclclient->m_pDialog[ NMAINMAP_DLG ])->Show( false );
									((CMainMapDlg*)pclclient->m_pDialog[ NMAINMAP_DLG ])->DeleteDialog();
								}
								else
								{
									((CMainMapDlg*)pclclient->m_pDialog[ NMAINMAP_DLG ])->Show( true );
								}
							}
							else
							{
								pclclient->CreateInterface( NMAINMAP_DLG );
								((CMainMapDlg*)pclclient->m_pDialog[ NMAINMAP_DLG ])->Show( true );
							}
						}
						break;				
					case ID_SHORT_TAB:
						{ 	
							// 채팅창에 포커스가 맞추어져 있다면 무기를 바꾸는 대신 채팅 탭을 옮긴다.
							if( pclclient->m_pDialog[NCHATBAR_DLG] && ((CNChatBarDlg*)pclclient->m_pDialog[NCHATBAR_DLG])->IsInputFocus()) 
							{
								((CNChatBarDlg*)pclclient->m_pDialog[NCHATBAR_DLG])->MoveTabButtonToRight();
							}
							else if (	pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_WEAPON1 ].siUnique != 0 || 
										pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ PERSONITEM_WEAPON2 ].siUnique != 0 )
							{
								// [추가 : 황진성 2008. 1. 4 => 변신상태에서는 무기 교체 불가능.]
								if(true == pclclient->pclCM->CR[1]->clTransFormInfo.bTransFormMode)
								{
									break;
								}

								cltMsg clMsg(GAMEMSG_REQUEST_EXCHANGEWEAPON, 0, NULL);
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

								//((cltClient*)pclClient)->pclCM->CR[1]->SetCurrentOrder(ORDER_STOP);
								((cltClient*)pclClient)->SetSiSelectedOrder(ORDER_STOP);
							}
							// m_siSelectedInventoryItemIndex = -1;

							//			g_ItemInventory.ExchangeWeapon();	

	#ifdef _DEBUG
							pclclient->m_pFeastEffectManager->StopFeastEffect();
	#endif
						}
						break;
					case ID_SHORT_SHIFT_TAB:
						{
							// 채팅창에 포커스가 맞추어져 있다면 무기를 바꾸는 대신 채팅 탭을 옮긴다.
							if( pclclient->m_pDialog[NCHATBAR_DLG] && ((CNChatBarDlg*)pclclient->m_pDialog[NCHATBAR_DLG])->IsInputFocus()) 
							{
								((CNChatBarDlg*)pclclient->m_pDialog[NCHATBAR_DLG])->MoveTabButtonToLeft();
							}
						}
						break;
					//[진성] 소환수 변신.
					//case ID_SHIFT_B:
					//case ID_SHORT_CTRL_Z:
					case ID_SHORT_ALT_X:
						{
							if( false == pclclient->IsCountrySwitch( Switch_SummonTramsform ) ) break;

							cltGameMsgRequest_Summon_Transform clInfo;
							cltMsg clMsg( GAMEMSG_REQUEST_SUMMON_TRANSFORM, sizeof(clInfo), (BYTE*)&clInfo );
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
						break;
					case ID_SHORT_ALT_D:  //의복 , 갑옷 변경 코드.
						{
	#ifndef _NEW_INTERFACE_

							//if(pclclient->m_pDialog[NINVENTORY_DLG])
							//{
		//                      ((CNInventory*)pclclient->m_pDialog[NINVENTORY_DLG])->ExchangeClothes();
							//}
	#else
							if(pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ] )
							{
								((CNEquipWindow*)pclclient->m_pDialog[NNEWEQUIPWINDOW_DLG])->ExchangeClothes();
							}
	#endif
						}
						break;

					//case ID_SHORT_ALT_1:
					//	{
					//		SI32 mapindex = pclclient->pclCM->CR[1]->GetMapIndex() ;
					//		if ( pclclient->pclMapManager->IsInstanceMap(mapindex) )
					//		{
					//			SI32 summonid = pclclient->pclCM->CR[1]->GetSummonID() ;
					//			if ( pclclient->pclCM->IsAlive(summonid) && // 캐릭터가 나와 있고 그 소환수를 선택하지 않았다면
					//				pclclient->pclCM->CR[1]->siSelectedCharIndex != pclclient->pclCM->CR[summonid]->GetSummonIndex() )
					//			{
					//				SI16 summonindex = pclclient->pclCM->CR[summonid]->GetSummonIndex();

					//				cltGameMsgRequest_Select_SummonChar clinfo(summonindex);
					//				cltMsg clMsg(GAMEMSG_REQUEST_SELECT_SUMMONCHAR, sizeof(clinfo), (BYTE*)&clinfo);
					//				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					//			}
					//			else
					//			{
					//				cltGameMsgRequest_Select_SummonChar clinfo(-1);
					//				cltMsg clMsg(GAMEMSG_REQUEST_SELECT_SUMMONCHAR, sizeof(clinfo), (BYTE*)&clinfo);
					//				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					//			}
					//		}
					//	}
					//	break;
					//[진성] 나만의 메뉴. 버튼 액션. => 2008-9-3
					case ID_SHORT_ALT_1: // 내가 써
					case ID_SHORT_ALT_2: // 내가 써
					case ID_SHORT_ALT_3: // 내가 써
					case ID_SHORT_ALT_4: // 내가 써
					case ID_SHORT_ALT_5: // 내가 써
						{
							if( pclClient->IsCountrySwitch( Switch_Mykey ) )
							{
								if( pclclient->m_pDialog[NGACHANEW_DLG] ||
									pclclient->m_pDialog[NGACHA_DLG]	  )	break;
								
								// 설정이 안되있으면.
								if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bMykey == false )
									break;							

								CDialogController* pMykeyDialog = NULL; 
								pMykeyDialog = pclclient->m_pDialog[NMYKEY];

								if( pMykeyDialog )
								{
									pMykeyDialog = pclclient->m_pDialog[NMYKEY];
									((CNMykeyDlg*)pMykeyDialog)->KeyAction( LOWORD(wParam) );
								}
								else if( NULL == pMykeyDialog )
								{
									pclclient->CreateInterface( NMYKEY );
									pclclient->CreateInterface( NMYKEYMINI );

									pMykeyDialog = pclclient->m_pDialog[NMYKEY];
									((CNMykeyDlg*)pMykeyDialog)->Show( false );
									((CNMykeyDlg*)pMykeyDialog)->KeyAction( LOWORD(wParam) );
								}
							}
						}
						break;

					case ID_SHORT_ALT_6:
						{
	#ifdef _DEBUG
							cltCharClient* pclMyChar = pclClient->pclCM->GetCharClient( 1 );
							if ( (pclMyChar) && (pclMyChar->pclSoulGuard) && (FALSE == pclMyChar->pclSoulGuard->IsEmpty()) )
							{
								if ( pclMyChar->pclSoulGuard->GetStatus() == SOULGUARD_STATUS_OUT )
								{
									cltGameMsgRequest_SoulGuardStatusInOut clInfo( pclMyChar->pclSoulGuard->GetSoulGuardID(), SOULGUARD_STATUS_IN );
									cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT, sizeof(clInfo), (BYTE*)&clInfo );
									pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
								}
								else
								{
									cltGameMsgRequest_SoulGuardStatusInOut clInfo( pclMyChar->pclSoulGuard->GetSoulGuardID(), SOULGUARD_STATUS_OUT );
									cltMsg clMsg( GAMEMSG_REQUEST_SOULGUARD_STATUSINOUT, sizeof(clInfo), (BYTE*)&clInfo );
									pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
								}
							}
							
	#endif
						}
						break;

					case ID_SHORT_ALT_7:
						{
	#ifdef _DEBUG
							pclclient->CreateInterface( NSYSTEMREWARDLIST_DLG );
	#endif
						}
						break;

					case ID_SHORT_ALT_8:
						{
	#ifdef _DEBUG
							pclClient->CreateDevNPCInfoDlg();
	#else
							if(pclClient->pclCM)
							{
								if(pclClient->pclCM->CR[1]->GetGameMasterLevel() >= BASIC_GAMEMASTER_LEVEL)
								{
									pclClient->CreateDevCharInfoDlg();
								}
							}
							break;
	#endif 
						}
						break;

					case ID_SHORT_ALT_9:
						{
	#ifdef _DEBUG
							/*
							if ( pclclient->m_pDialog[NNOTICEADMIN_DLG] == NULL )
								pclclient->CreateInterface ( NNOTICEADMIN_DLG );

							TCHAR* pText = GetTxtFromMgr(100);
							((cltNNoticeAdminDlg*)pclclient->m_pDialog[ NNOTICEADMIN_DLG ])->SetData(pText);
							*/

							pclclient->CreateInterface( NEWSELECTBOX_DLG );
	#endif
						}
						break;

					case ID_SHORT_ALT_0:
						{
	#ifdef _DEBUG
							cltMsg clMsg(GAMEMSG_REQUEST_HOPETOFRONT, 0, NULL);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	#endif
						}
						break;

					case ID_SHORT_ALT_W: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_SMALLMAP),    NULL);	break;
					case ID_SHORT_ALT_M:
						{
							
							if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
							{
								if ( ((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->IsShow() == false )
								{
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->Show();
								}
								else
								{
									((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->Hide();
								}
							}
							else
							{
								if ( ((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->IsShow() == false )
								{
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->Show();
								}
								else
								{
									((CNMagicInfoDlg*)pclclient->m_pDialog[ NMAGICINFO_DLG ])->Hide();
								}
							}
						}
						break;
					case ID_SHORT_ALT_O: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_CONFIG),		NULL);	break;
					case ID_SHORT_ALT_A: 
						{
							SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_CHARSTATUS),  NULL);	break;					
						}
						break;
					case ID_SHORT_ALT_B:
						{
	#ifdef _DEBUG
							/*if( pclclient->m_pDialog[SERVER_EVENT_RANK_DLG] == NULL )
							{							
								pclclient->CreateInterface( SERVER_EVENT_RANK_DLG );
							}
							else
							{
								pclclient->DestroyInterface( pclclient->m_pDialog[SERVER_EVENT_RANK_DLG] );
							}*/
	#endif
							if(pclClient->IsCountrySwitch(Switch_Transform))
							{
								if ( pclclient->m_pDialog[CHARTRANSFORMINFO_DLG] == NULL )
								{							
									pclclient->CreateInterface( CHARTRANSFORMINFO_DLG );
								}
								else
								{
									pclclient->DestroyInterface( pclclient->m_pDialog[CHARTRANSFORMINFO_DLG] );
								}
							}
						}
						break;
					case ID_SHORT_ALT_I: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_INVENTORY),   NULL);	break;
					case ID_SHORT_ALT_S: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_SKILL),		NULL);	break;
					case ID_SHORT_ALT_Q: SendMessage(hWnd, WM_COMMAND,  LOWORD(ID_TOOLBAR_QUEST),		NULL);  break;
						
					case ID_SHORT_ALT_C: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_MASTERINFO),			NULL);	break;
					case ID_SHORT_ALT_V:
						{
							if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
							{
								( ( CNResidentsDlg * )pclclient->m_pDialog[ NRESIDENTS_DLG ] )->Show();
							}
						}
						break;
					case ID_SHORT_ALT_F: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_FRIEND),		NULL);	break;
					case ID_SHORT_ALT_T:
						{
							//KHY - 0610 - 스승제자 S 추가수정.
							if ( ConstServiceArea_Japan != pclClient->siServiceArea )
							{
								// 스승이 필요한 레벨이면 후견인 창을 보여준다. 
								if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < pclClient->siMinimumTeacherLevel )
								{
									pclclient->CreateInterface(NCHILD_DLG);
								}
								else
								{	
									pclclient->CreateInterface(NFATHER_DLG);
									pclclient->CreateInterface(NLOWLEVEL_DLG);
								}
							}
							else if ( ConstServiceArea_Japan == pclClient->siServiceArea )
							{
								// 스승이 필요한 레벨이면 후견인 창을 보여준다. 
								if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < FATHER_LEVEL)
								{
									pclclient->CreateInterface(NCHILD_DLG);
									pclclient->CreateInterface(NLOWLEVEL_DLG);
								}
								else
								{	
									pclclient->CreateInterface(NFATHER_DLG);
									pclclient->CreateInterface(NLOWLEVEL_DLG);
								}
							}
						}
						break;
					case ID_SHORT_ALT_E: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_EQUIP),NULL);	break;
					case ID_SHORT_ALT_P: SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_SCREENSHOT),NULL);break;
					case ID_SHORT_ALT_G: 
						{
							if(pclclient->m_pDialog[NPARTY_DLG])
							{
								((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->Minimi(
									!((CNPartyDlg*)pclclient->m_pDialog[NPARTY_DLG])->IsMinimi()	
									);
							}
						}
						break;
					case ID_SHORT_ALT_H: 
						{
							if(pclClient->IsCountrySwitch(Switch_Help))
							{
								SendMessage(hWnd, WM_COMMAND, 	LOWORD(ID_TOOLBAR_HELP),NULL);		break;
							}		
						}
						break;
					//[추가 : 황진성 2008. 2. 4 => 공간 상자 단축키.]
					case ID_SHORT_ALT_Z:
						{
							if(pclClient->IsCountrySwitch(Switch_SpaceBox))
							{
								if ( pclclient->m_pDialog[NSPACE_BOX] == NULL )
								{							
									pclclient->CreateInterface( NSPACE_BOX );
								}
								else
								{
									pclclient->DestroyInterface( pclclient->m_pDialog[NSPACE_BOX] );
								}
							}
						}
						break;

					case ID_SHORT_ALT_L:
						{
							if ( pclClient->IsCountrySwitch(Switch_PVP_League) )
							{
								pclclient->CreateInterface( PVPLEAGUE_DLG );
								
							}
						}
						break;
					default:
						pclClient->DoMsg_WM_COMMAND(wParam);
						break;
					}

				}//KHY - 0701 - 원키 이벤트 (F1~F12)는 0.3초에 한번씩만 작동하도록 한다.
			}
			else if ( pclClient->GameMode == GAMEMODE_MAPEDIT )
			{
				switch(LOWORD(wParam))
				{
				default:
					if(pclClient)
					{
						pclClient->DoMsg_WM_COMMAND(wParam);
					}
					break;
				}
			}
		}
		return 0;
	case WM_KEYUP:
		switch( wParam )
		{
			/*		case VK_UP:
			UpKeyPressSwitch = FALSE;
			return 0;
			case VK_DOWN:
			DownKeyPressSwitch = FALSE;
			return 0;
			case VK_LEFT:
			LeftKeyPressSwitch = FALSE;
			return 0;
			case VK_RIGHT:
			RightKeyPressSwitch = FALSE;
			return 0;
			*/
		case VK_RETURN:
			if( pclClient && pclClient->GameMode == GAMEMODE_CLIENT )
			{
				cltClient *pclclient = (cltClient *)pclClient;

				if( pclclient->m_pDialog[ NCHATBAR_DLG ] )
				{
					((CNChatBarDlg*)pclclient->m_pDialog[ NCHATBAR_DLG ])->SetInputEditBoxFocus();
				}
			}			
			return 0;
		case VK_INSERT:
			if( pclClient && pclClient->GameMode == GAMEMODE_CLIENT )
			{
				cltClient *pclclient = (cltClient *)pclClient;
				if ( pclclient->GetGameStatus() == GS_GAME )
				{	
					//if( pclclient->m_pDialog[NCHARBUFF_ROULETTE_DLG] != NULL &&
					//	pclclient->m_pDialog[NCHARBUFF_ROULETTE_DLG]->IsCreate() )	return	0 ;
					// 변신버프 상태에서는 앉지 못한다. - 몬스터는 앉기 이미지가 없어서 클라이언트가 종료된다.
					//if ( pclClient->pclCM->CR[1]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM ) == false )
					{
						if ( pclClient->pclCM->CR[1]->GetCurrentOrder() == ORDER_SITDOWN )
						{
							cltGameMsgRequest_SitDown clinfo( false, pclClient->pclCM->CR[1]->siDirection );
							cltMsg clMsg(GAMEMSG_REQUEST_SITDOWN, sizeof(clinfo), (BYTE*)&clinfo);
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
						else
						{
							cltGameMsgRequest_SitDown clinfo( true, pclClient->pclCM->CR[1]->siDirection );
							cltMsg clMsg(GAMEMSG_REQUEST_SITDOWN, sizeof(clinfo), (BYTE*)&clinfo);
							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}
				}
			}		
			return 0;
		}

		return 0;

	//cyj 맵에디터 브러쉬기능 키입력부분 추가
	case WM_CHAR:
		{
			if(pclClient->GameMode == GAMEMODE_MAPEDIT)
			{
				switch( wParam )
				{
				case '1':
					if( pclClient->GameMode == GAMEMODE_MAPEDIT ) ((cltMapEdit*)pclClient)->uiCollisionBrush = 1;
					break;
				case '2':
					if( pclClient->GameMode == GAMEMODE_MAPEDIT ) ((cltMapEdit*)pclClient)->uiCollisionBrush = 3;
					break;
				case '3':
					if( pclClient->GameMode == GAMEMODE_MAPEDIT ) ((cltMapEdit*)pclClient)->uiCollisionBrush = 9;
					break;
				}

			}
		}
		break;

	case WM_KEYDOWN:
		{
			switch( wParam )
			{

			case VK_F11:
				{
				
				}
				return FALSE;
			case VK_F12:
				{
					if(pclClient)
					{
						cltClient *pclclient = (cltClient*)pclClient;
#ifdef _DEBUG
						//pclclient->CreateInterface( TEST_DLG );
#endif
					}
				}
				return FALSE;
			
			case VK_ESCAPE:
				{
					
					if(pclClient)
					{
						if(pclClient->GameMode == GAMEMODE_SERVER)
						{
							DestroyWindow(pclClient->GetHwnd());
						}
						else
						{
							if ( pclClient->GameMode == GAMEMODE_MAPEDIT )
							{
								if (  MessageBox(NULL, TEXT("진짜 종료?"), TEXT("진짜 종료?"), MB_ICONQUESTION | MB_YESNO) == IDYES ) 
								{
									dwQuitTimer = 1;
								}
							}
							else
							{
								cltClient* pclclient = (cltClient*)pclClient;

								// 종료 확인을 띄운다. 
								//cyj 죽어있을때는 부활 다이얼로그 띄운다.
								if (pclclient->pclCM->CR[1]->GetLife() <= 0 && pclclient->bReviveRequestSwitch)
								{
									if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
									{
										pclclient->CreateInterface( NCOMMONMSG_DLG );
									}
									((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REVIVE);
								}
								else
									pclclient->CreateInterface( NEXIT_DLG );
							}
						}
					}
				}
				return 0;

			case VK_LWIN:
			case VK_RWIN:
				{
					if( pclClient->IsCountrySwitch(Switch_ChangeFullScreen) )
					{
						ShowWindow( hWnd, SW_SHOWMINNOACTIVE );
						GP.RestoreDisplayToWindow();
					}
				}
				return 0;
			}
		}
		break;

	
	case WM_SYSKEYDOWN:
		{			
			// 키보드 입력을 기억한다.
			// 이전의 키 상태가 FALSE일때에만 (누르고 있는 상태는 한번의 입력만 받는다.)
			if(oldkeystatus==FALSE)
			{
				if(pclClient)
				{
					if(pclClient->GetFrame() > 40 )
					{
						cltKey clkey(KEY_MODE_NONE, KEY_MODE_NONE,KEY_MODE_NONE, false, IpD.Mouse_X, IpD.Mouse_Y, wParam);
						pclClient->PushKey(&clkey);
						
					}
				}
			}
			
			switch( wParam )
			{			
			case VK_F8:
				{
					SI32 a = 0;
				}
				return FALSE;

			
			case VK_ESCAPE:
				{
					if(pclClient)
					{
						if(pclClient->GameMode == GAMEMODE_SERVER)
						{
							DestroyWindow(pclClient->GetHwnd());
						}
						else
						{
							cltClient* pclclient = (cltClient*)pclClient;
							
							// 종료 확인을 띄운다. 
							//cyj 죽어있을때는 부활 다이얼로그 띄운다.
							if (pclclient->pclCM->CR[1]->GetLife() <= 0 && pclclient->bReviveRequestSwitch)
							{
								if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
								{
									pclclient->CreateInterface( NCOMMONMSG_DLG );
								}
								((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REVIVE);
							}
							else
								pclclient->CreateInterface( NEXIT_DLG );

						}
					}
				}
				return 0;
			}
		}
		return 0;

	case WM_CLOSE:		
		if(pclClient)
		{
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				DestroyWindow(pclClient->GetHwnd());
			}
			else
			{
				if(pclClient->GameMode == GAMEMODE_MAPEDIT)
				{
					if (  MessageBox(NULL, TEXT("진짜 종료?"), TEXT("진짜 종료?"), MB_ICONQUESTION | MB_YESNO) == IDYES ) 
					{
						dwQuitTimer = 1;
					}
				}
				else
				{
					cltClient* pclclient = (cltClient*)pclClient;

					//cyj 죽어있을때는 부활 다이얼로그 띄운다.
					if (pclclient->pclCM->CR[1]->GetLife() <= 0 && pclclient->bReviveRequestSwitch)
					{
						if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
						{
							pclclient->CreateInterface( NCOMMONMSG_DLG );
						}
						((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REVIVE);
					}
					else
						pclclient->CreateInterface( NEXIT_DLG );

				}				

			}
		}
		return 0;
			
	case WM_CREATE:
		{
			InitCommonControls();
		}
		
		return 0;
		
	case WM_SIZE:
		if(pclClient)
		{
			if( pclClient->GameMode != GAMEMODE_SERVER)
			{
				//((cltClient*)pclClient)->ComponentReSize(wParam, lParam);
			
				/*
				HBRUSH hBrush,hOldBrush;
				HBITMAP hButtonBitmap,hOldBitmap;
				HDC hDC = GetDC( hWnd );
				HDC memDC;
				RECT rt;
				GetClientRect(hWnd,&rt);
				
				hBrush = (HBRUSH) GetStockBrush(BLACK_BRUSH);
				hOldBrush = (HBRUSH) SelectObject(hDC,hBrush);
				Rectangle(hDC,rt.left,rt.top,rt.right,rt.bottom);
				SelectObject(hDC,hOldBrush);
				DeleteObject(hOldBrush);
				2
				hButtonBitmap = (HBITMAP) LoadImage( ((cltClient *)pclClient)->GetInstance(), TEXT("GImg/loading.bmp"), IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
				memDC = CreateCompatibleDC( hDC );
				
				hOldBitmap = (HBITMAP)SelectObject( memDC, hButtonBitmap );
				BitBlt( hDC, (rt.right-344)/2, (rt.bottom-207)/2, 344, 207, memDC, 0, 0, SRCCOPY );
				
				SelectObject ( memDC, hOldBitmap );						
				DeleteObject(hOldBitmap);
				DeleteDC( memDC );
				ReleaseDC( hWnd, hDC );
				*/
			}
		}
		break;
		
	case WM_ACTIVATE:
		{
			if(g_graphics.m_hWnd == NULL ) return 0;
			bActiveSwitch = (BOOL)wParam && GetForegroundWindow() == hWnd && !IsIconic(hWnd);

			if(bActiveSwitch == true)
			{
				if ( pclClient && pclClient->GameMode == GAMEMODE_CLIENT )
				{
					if(FirstSwitch==FALSE)
					{
						if( g_SoundOnOffDlg.m_bMaxWindowMode == true && 
							pclClient->IsCountrySwitch(Switch_ChangeFullScreen) )
						{
							GP.SetDisplayToGameMode();
						}
						Restore();
					}

					cltClient *pclclient = (cltClient *) pclClient;
					if ( pclclient->bWindowNotice )						
					{
						pclclient->bWindowNotice = false;
						bFlashWindowStatus = false;						
					}
				}

			}
			FirstSwitch=FALSE;
		}

		return 0;

	case WM_ACTIVATEAPP:
		{
			bActiveSwitch = (BOOL)wParam && GetForegroundWindow() == hWnd && !IsIconic(hWnd);

			if(bActiveSwitch == true)
			{
				if ( pclClient && pclClient->GameMode == GAMEMODE_CLIENT )
				{
					if(FirstSwitch==FALSE)
					{
						if( g_SoundOnOffDlg.m_bMaxWindowMode == true && 
							pclClient->IsCountrySwitch(Switch_ChangeFullScreen) )
						{
							GP.SetDisplayToGameMode();
						}
						Restore();
					}

					cltClient *pclclient = (cltClient *) pclClient;
					if ( pclclient->bWindowNotice )
					{
						pclclient->bWindowNotice = false;
						bFlashWindowStatus = false;
					}
				}

			}
			FirstSwitch=FALSE;
		}

		return 0;
		
		
		
	case WM_ERASEBKGND:
		// 배경을 지우라는 메시지 
		// 게임 화면 위에서 다른 윈도우를 움직일 경우
		// 배경색 잔상 남는것을 막기위해서 그냥 리턴해 버린다 
		if(pclClient)
		{
			// 서버 모드일때는 배경을 갱신한다.
			if(pclClient->GameMode != GAMEMODE_SERVER)
			{
				return 0;
			}
		}
		break;
		
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			
			hdc = BeginPaint(hWnd, &ps);
			
			EndPaint(hWnd, &ps);

			if( pclClient ) {
				
				if( pclClient->GameMode == GAMEMODE_CLIENT ) {
					((cltClient *)pclClient)->GameDrawUpdateScreen();
				}
			}			
		}
		break;
				
	case WM_DESTROY:
		
		// 클라이언트 객체를 제거한다. 
		if(pclClient)
		{
			// 서버는 게임을 종료하기 전에 모든 데이터를 저장해야 한다. 
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{
				// 정보를 DB에 모두 저장한다. 
				pclClient->pclCM->CloseAllSocket();
				
				// 잠깐 대기. 
				Sleep(6000);
			}
			
			if(pclClient->GameMode == GAMEMODE_SERVER)
			{

			}
			else if(pclClient->GameMode == GAMEMODE_CLIENT)
			{
				cltClient* pclclient = (cltClient*)pclClient;
				// 한게임 범용보고
				if(pclClient->siServiceArea == ConstServiceArea_NHNChina)
				{
					pclclient->HanAuth_Lib_HanReportSendPosLog_Client_NHNCHINA(pclclient->clSelectedWorldInfo.baseinfo.siServerID ,pclclient->m_szPorteAuth_UserID, HANREPORT_CLIENT_GAMEEND);
				}
				else
				{
					pclclient->HanAuth_Lib_HanReportSendPosLog_Client(pclclient->m_szHanAuth_UserID, HANREPORT_CLIENT_GAMEEND);

				}


#ifdef USE_HANAUTH
				HanAuthMiniExplorer( true );	// 익스플로러 복구
#endif // USE_HANAUTH
				#if defined (USE_HANPOLL)
					if (pclClient->IsCountrySwitch(Switch_PopupCheck) )
					{
						// [ 성웅] 한게임 설문 조사 홈페이지를 띄운다.
						HanPollProcess("");
					}
				#endif

			}

			delete pclClient;
			pclClient = NULL;
		}
		
		CoUninitialize();
		
		// 화일 맵핑 핸들을 해제한다. 
		CloseDupGameHandle();
		
		// 프로파일링을 해제한다.
		
		PostQuitMessage(0);
		return 0;

	case WM_IME_COMPOSITION:
	case WM_IME_STARTCOMPOSITION:
	case WM_IME_ENDCOMPOSITION:
		return FALSE;

	case WM_IME_NOTIFY:
		{
			switch(wParam)
			{
				case IMN_SETOPENSTATUS:
					{
						// A -> あ 혹은 あ -> A  된  ImeState 변경 메시지 
						// Edit.cpp , mainfunction.cpp 쪽 메시지 프로시저에서 처리해주고있음.
						cltClient* pclclient = (cltClient*)pclClient;
						pclclient->m_pInterfaceMgr->CheckToggleState();
					}
					break;
			}
		}
		break;

/*
    case WM_SYSCOMMAND:
        // Prevent moving/sizing and power loss in fullscreen mode
        switch( wParam )
        {
            case SC_MOVE:
            case SC_SIZE:
            case SC_MAXIMIZE:
            case SC_MONITORPOWER:
                if( g_bFullScreen ) return TRUE;
				break;
        }
        break;
*/		
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

//--------------------------------------------------------------------------
// Name: Restore()
// Desc: 미니마이즈된 상태에서 다시 복구할때 사용되는 함수 
//--------------------------------------------------------------------------
BOOL Restore()
{
	GP.Restore();
	
	return FALSE;
}

//KHY - 0701 - 원키 이벤트 (F1~F12)는 0.3초에 한번씩만 작동하도록 한다.
bool UserdoubleKeyEvent(SI32 KeyEvent)
{
	bool breturnKey = false;

	switch(KeyEvent)
	{
	case ID_SHORT_VK_F1:	
	case ID_SHORT_VK_F2:	
	case ID_SHORT_VK_F3:
	case ID_SHORT_VK_F4:	
	case ID_SHORT_VK_F5:	
	case ID_SHORT_VK_F6:	
	case ID_SHORT_VK_F7:	
	case ID_SHORT_VK_F8:	
	case ID_SHORT_VK_F9:	
	case ID_SHORT_VK_F11:	
	case ID_SHORT_VK_F12:
		breturnKey = false;
		break;
	case ID_SHORT_CTRL_1:	
	case ID_SHORT_CTRL_2:	
	case ID_SHORT_CTRL_3:	
	case ID_SHORT_CTRL_4:	
	case ID_SHORT_CTRL_5:	
	case ID_SHORT_CTRL_6:	
	case ID_SHORT_CTRL_7:	
	case ID_SHORT_CTRL_8:	
	case ID_SHORT_CTRL_9:	
	case ID_SHORT_CTRL_0:
	case ID_SHORT_CTRL_M:	// 전체맵열기
	case ID_SHORT_TAB:		
	case ID_SHORT_ALT_O:	
	case ID_SHORT_SHIFT_TAB: 
	case ID_SHORT_ALT_W:	
	case ID_SHORT_ALT_M:	
	case ID_SHORT_ALT_A:	
	case ID_SHORT_ALT_I:	
	case ID_SHORT_ALT_S:	
	case ID_SHORT_ALT_C:	
	case ID_SHORT_ALT_V:	
	case ID_SHORT_ALT_F:	
	case ID_SHORT_ALT_T:	
	case ID_SHORT_ALT_Q:	
	case ID_SHORT_ALT_E:	
	case ID_SHORT_ALT_P:	
	case ID_SHORT_ALT_H:	
	case ID_SHORT_ALT_L:
	//case ID_SHIFT_B:
	case ID_SHORT_CTRL_Z:
	case ID_SHORT_ALT_X:
		
		
	default:
		breturnKey = true;	
		break;
	}

	return breturnKey;
}

