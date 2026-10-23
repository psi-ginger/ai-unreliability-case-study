//#include "NSysMenuBarDlg.h"
//#include "NSysChildMenudlg.h"
//
//
//#include "../../../Client/ItemInfo/ItemInfo.h"
//
//#include "../../Inventory/ItemInventory.h"
//
//#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
//#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
//
//#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"
//
//#include "../../Interface/NInventory/NInventory.h"
////#include "../../Interface/MonsterInfoDlg/MonsterInfoDlg.h"
//
//#include "../../Client.h"
//
//#include "../../Prompt/PromptMgr.h"
//
//#include "../../NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"
//
//#include "../../Interface/ResidentsViewDlg/ResidentsViewDlg.h"
//#include "../../Interface/FriendDlg/FriendDlg.h"
//#include "../../NInterface/NFriendDlg/NFriendDlg.h"
//
//#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
//#include "../../NInterface/NItemInfoDlg/ItemInfoDlg.h"
//#include "../../NInterface/NHomePageDlg/HomePageDlg.h"
//#include "../../NInterface/NHuntMapInfoDlg/HuntMapInfoDlg.h"
//#include "../../NInterface/NResidentsInfoDlg/ResidentsInfoDlg.h"
//#include "../../NInterface/NMagicInfoDlg/MagicInfoDlg.h"
//#include "../../NInterface/NTeacherDlg/TeacherDlg.h"
//#include "../../NInterface/NTeacherDlg/FatherDlg.h"
//#include "../../NInterface/NTeacherDlg/ChildDlg.h"
//#include "../../NInterface/NItemMallManager/NItemMallManager1.h"
//#include "../../ninterface/ncharmenubardlg/NCharMenuBarDlg.h"
//
//#include "../../NInterface/NCommonMsgDlg/NCommonMsgDlg.h"
//
//#include "../../../Common/Father/Father.h"
//#include "../../../common/HuntMapInfo/HuntMapInfo.h"
//#include "../../../common/Master/Master.h"
//#include "../../NInterface/NMaster/NMaster.h"
//
//#include "../../../common/HintDlg/HintDlg.h"
//
//#include "../../../lib/ScreenShot/CaptureJPG.h"
//
//// 마을 상세 정보 
//#include "NInterface/NVillageDetailInfo/NVillageDetailInfo.h"
//#include "NInterface/NResidentsDlg/NResidentsDlg.h"
//
//#include "../../InterfaceMgr/InterfaceMgr.h"
//#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
//#include "../../InterfaceMgr/Interface/ComboBox.h"
//#include "../../InterfaceMgr/Interface/Static.h"
//
//#include "Msg/MsgType-PersonalShop.h"
//#include "Msg/MsgType-ItemMall.h"
//
////-----------------------------
//// Common
////-----------------------------
//#include "PersonInfo.h"
//#include "Char\CharManager\CharManager.h"
//
//#include "NInterface/NCharStatus/NCharStatus.h"
//#include "NInterface/NBossMarket/NBossMarketDlg.h"
//
//#include "../lib/MD5/md5.h"
//
//#include "../../Client.h"
//extern cltCommonLogic* pclClient;
//
//extern SI32		gsiResolution;
//
//CNSysMenuBarDlg::CNSysMenuBarDlg() : CDialogController()
//{
//	for ( SI16 i = 0; i < MAX_NSYSMENUBARDLG_BUTTON_NUM; ++i )
//	{
//		m_pToolBarButton[ i ] = NULL;
//	}
//
//	m_siNowOpendMenu = 0;
//
//}
//
//CNSysMenuBarDlg::~CNSysMenuBarDlg()
//{
//	for ( SI16 i = 0; i < MAX_NSYSMENUBARDLG_BUTTON_NUM; ++i )
//	{
//		SAFE_DELETE( m_pToolBarButton[ i ] );
//	}
//}
//
//void CNSysMenuBarDlg::Create()
//{
//	cltClient *pclclient = (cltClient *)pclClient;
//	// 1024 X 768
//	if ( gsiResolution == 2 )
//	{
//
//		CDialogController::Create( NSYSMENUBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, TEXT("NInterface/Interface/INTERFACE_254x26_SYSMENU.SPR"), 0, true, 3,
//			MAP_TILE_XSIZE + 706 + (g_SoundOnOffDlg.m_bMaxWindowMode ? 32 : 0) , pclclient->GetScreenYSize() - 50, 254, 26, TRUE, false, false, true );
//		
//		m_pToolBarButton[ 0 ] = new CButton( this );
//		m_pToolBarButton[ 0 ]->Create( NSYSMENUBAR_DIALOG_BUTTON_0, NULL, TEXT("NInterface/Button/InterfaceButton_52x22_charmenu.SPR"), 24, false, 0, 25, 2, 52, 22, 52, 22 );
//
//		m_pToolBarButton[ 1 ] = new CButton( this );
//		m_pToolBarButton[ 1 ]->Create( NSYSMENUBAR_DIALOG_BUTTON_0 + 1, NULL, TEXT("NInterface/Button/InterfaceButton_52x22_charmenu.SPR"), 28, false, 0, 79, 2, 52, 22, 52, 22 );
//
//		m_pToolBarButton[ 2 ] = new CButton( this );
//		m_pToolBarButton[ 2 ]->Create( NSYSMENUBAR_DIALOG_BUTTON_0 + 2, NULL, TEXT("NInterface/Button/InterfaceButton_52x22_charmenu.SPR"), 32, false, 0, 133, 2, 52, 22, 52, 22 );
//		
//		m_pToolBarButton[ 3 ] = new CButton( this );
//		m_pToolBarButton[ 3 ]->Create( NSYSMENUBAR_DIALOG_BUTTON_0 + 3, NULL, TEXT("NInterface/Button/InterfaceButton_52x22_charmenu.SPR"), 36, false, 0, 187, 2, 52, 22, 52, 22 );
//
//	
//		//SetAvailableScreenRect(0,0,960,720);
//
//	}
//	else	// 해상도에 따라 다른 코드 추가
//	{
//		
//
//
//	}
//
//	SetTopMostWindow( true );
//	SetActionTimer( 100 );
//	SetTimerOnOff( true );
//
//	return;
//}
//
//
//void CNSysMenuBarDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
//{
//	cltClient *pclclient = ( cltClient * )pclClient;
//
//
//	//pclclient->pclLogo->Draw( pclclient->GetScreenXSize(), pclclient->GetScreenYSize(), pclclient->GetScreenRX(), pclclient->GetScreenRY() );
//
//	return;
//}
//
//void CALLBACK CNSysMenuBarDlg::StaticCallBackDialogBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//	CNSysMenuBarDlg *pThis = (CNSysMenuBarDlg *)pControllerMgr;
//	pThis->CallBackDialogBar( nEvent, nControlID, pControl );
//	return;
//}
//
//void CALLBACK CNSysMenuBarDlg::CallBackDialogBar( UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//	cltClient *pclclient = ( cltClient * ) pclClient;
//	switch( nEvent )
//	{
//	case EVENT_BUTTON_CLICKED:
//		{
//			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
//
//			if(nControlID == NSYSMENUBAR_DIALOG_BUTTON_0 )	// 인포 버튼이면
//			{
//
//				pclclient->CreateInterface(NINFO_DLG);	
//
//				break;
//			}
//			else if(nControlID == NSYSMENUBAR_DIALOG_BUTTON_0 + 2)	// 아이템몰 버튼이면
//			{
//				//////////////////////////////////////////////////////////////////////////
//				//////////////		GM만 볼수있다		//////////////////////////////
//				//////////////////////////////////////////////////////////////////////////
//				//	if(pclclient->pclCM->CR[1]->GetGameMasterLevel() != 101 )
//				//	break;
//
//				// itemmall-
//				if( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
//				{
//					if( BASIC_PLAYERMASTER_LEVEL <= pclClient->pclCM->CR[1]->GetGameMasterLevel() )
//					{
//						TCHAR* pTitle = GetTxtFromMgr(4314);
//						TCHAR* pText = GetTxtFromMgr(7547);
//					
//						stMsgBox MsgBox;
//						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
//						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
//						break;
//					}
//					else
//					{
//						pclclient->ShowHelpWindow(TEXT("NULL"));
//						break;
//					}
//				}
//				else
//				{
//					if( pclClient->bItemMallUseSwitch == FALSE)
//					{	
//						TCHAR* pText = GetTxtFromMgr(962);
//
//						if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
//						{
//							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
//							break;
//						}
//					}
//					else
//					{
//						if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
//						{
//							if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
//							{
//								cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
//
//								UI08 md5buf[256] = "" ;
//								char buffermd5[256] = "" ;
//
//								TCHAR	szURL[1024];
//								TCHAR	szBeforeMD5[256];
//
//								sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");
//
//								memset(md5buf, 0, sizeof(md5buf));
//								// MD5
//								SI32 md5size = _tcslen(szBeforeMD5);
//								memcpy(md5buf, szBeforeMD5, md5size);
//								md5_string(md5buf, buffermd5, md5size);
//
//								sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
//									100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);
//
//								g_LoginNoticeDlg.Set(szURL, 970, 756);
//							}
//						}
//						else
//						{
//							pclclient->CreateInterface(NNEWITEMMALL_DLG);	
//						}
//						break;
//					}	
//				}
//			}
//
//
//			// 이미 열려 있는 메뉴를 한번더 열라고 보내온 거라면
//			// 무조건 닫아준다.
//			if( m_siNowOpendMenu ==   nControlID - NSYSMENUBAR_DIALOG_BUTTON_0  )
//			{
//				if((CNSysChildMenuDlg*)pclclient->m_pDialog[ NSYSCHILDMENU_DLG ])
//				{
//					((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->DeleteDialog();
//				}
//
//				m_siNowOpendMenu = 0;
//
//				break;
//			}
//
//
//			if((CNSysChildMenuDlg*)pclclient->m_pDialog[ NSYSCHILDMENU_DLG ]==NULL)
//			{
//				pclclient->CreateInterface( NSYSCHILDMENU_DLG );
//				// 자식의 위치 설정
//				SI16 childXpos = 0, childYpos = 0;
//				((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->Set( nControlID - NSYSMENUBAR_DIALOG_BUTTON_0 );
//				childXpos = GetChildXPos(((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->GetWidth());
//				childYpos = GetChildYPos(((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->GetHeight());
//				((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->MovePos( childXpos, childYpos);
//
//				m_siNowOpendMenu =  nControlID - NSYSMENUBAR_DIALOG_BUTTON_0 ;
//					
//			}
//			else
//			{
//				//pclclient->CreateInterface( NSYSCHILDMENU_DLG  );
//				//((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->Set( CHILDMENU_ID_INFO );
//				//((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->ReSet( CHILDMENU_ID_INFO );
//				((CNSysChildMenuDlg *)pclclient->m_pDialog[ NSYSCHILDMENU_DLG  ])->DeleteDialog();
//
//				m_siNowOpendMenu = 0;
//			}				
//
//			
//		}
//		break;
//	}
//}
//
//void CNSysMenuBarDlg::Action()
//{
//	cltClient *pclclient = (cltClient *)pclClient;
//
//	{
//		// NSysMenu_Dlg , NCharMenu_Dlg , STATUSBAR_DLG, 외곽선 과의 스냅 체크
//
//		SI16 distanceX, distanceY; //대상과의 거리
//		SI16 destX, destY; // 대상의 좌표
//		destX = MAP_TILE_XSIZE + 706 + (g_SoundOnOffDlg.m_bMaxWindowMode ? 32 : 0);  // 344 는 채팅바다이얼로그의 넓이  362는 캐릭터메뉴바 다이얼로그의 넓이
//		distanceX = GetX() - destX;
//		destY = pclclient->GetScreenYSize() - 24 - GetHeight();  // 24는 스테이터스바 높이
//		distanceY =  GetY() - destY ;
//
//		if(abs(distanceY) < SNAPRANGE )
//		{
//			MovePos(GetX(),destY);
//			if(abs(distanceX) < SNAPRANGE )
//			{
//				MovePos(destX,GetY());
//			}
//		}	
//
//	}
//
//	return;
//}
//
//
//SI16 CNSysMenuBarDlg::GetChildXPos(SI16 childWidth )
//{
//	cltClient *pclclient = ( cltClient * ) pclClient;
//
//	SI16 siChildXpos = 0;
//	// 기본은 중앙으로
//	siChildXpos = GetX() + (GetWidth() >> 1) - (childWidth >>1 );
//	
//	RECT m_WindowRect;
//	GetClientRect(pclclient->GetHwnd(), &m_WindowRect);
//	
//	SI16 startX = MAP_TILE_XSIZE ;// + pclclient->GetScreenRX();
//
//	SI16 endX = startX + m_WindowRect.right;
//
//	if(siChildXpos + childWidth > endX)
//		siChildXpos = endX - childWidth;
//	
//	if(siChildXpos < startX)
//		siChildXpos = startX;
//
//	return  siChildXpos;
//}
//
//
//SI16 CNSysMenuBarDlg::GetChildYPos(SI16 childHeight )
//{
//	SI16 siChildYpos = 0;
//	
//	siChildYpos = GetY() - childHeight;
//	
//	
//	if(GetY() < 350) // 대충 위쪽에 가까우면
//		siChildYpos = GetY() + GetHeight() + 3;	
//
//	return siChildYpos; 
//}
//
//void CNSysMenuBarDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//
//	switch( MsgIndex )
//	{
//	case 0:
//		{
//			if ( RetValue )
//			{
//				if( pclClient->bItemMallUseSwitch == FALSE)
//				{	
//					TCHAR* pText = GetTxtFromMgr(962);
//
//					if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
//					{
//						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
//					}
//				}
//				else
//				{
//					if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
//					{
//						if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
//						{
//							cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
//
//							UI08 md5buf[256] = "" ;
//							char buffermd5[256] = "" ;
//
//							TCHAR	szURL[1024];
//							TCHAR	szBeforeMD5[256];
//
//							sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");
//
//							memset(md5buf, 0, sizeof(md5buf));
//							// MD5
//							SI32 md5size = _tcslen(szBeforeMD5);
//							memcpy(md5buf, szBeforeMD5, md5size);
//							md5_string(md5buf, buffermd5, md5size);
//
//							sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
//								100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);
//
//							g_LoginNoticeDlg.Set(szURL, 970, 756);
//						}
//					}
//					else
//					{
//						pclclient->CreateInterface(NNEWITEMMALL_DLG);	
//					}
//				}	
//			}
//		}
//		break;
//	}
//}