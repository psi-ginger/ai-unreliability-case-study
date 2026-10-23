#include "NCharMenuBarDlg.h"

#include "../../../Client/ItemInfo/ItemInfo.h"

#include "../../Inventory/ItemInventory.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"
#include "../../Interface/NInventory/NInventory.h"

#include "../../ninterface/NNewinventory/NNewInventory.h"
#include "../../ninterface/ncharstatus/NEquipWindow.h"
#include "../../ninterface/ncharstatus/NCharQuest.h"
//#include "../../Interface/MonsterInfoDlg/MonsterInfoDlg.h"

#include "../../Client.h"

#include "../../Prompt/PromptMgr.h"

#include "../../NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

#include "../../Interface/ResidentsViewDlg/ResidentsViewDlg.h"
#include "../../Interface/FriendDlg/FriendDlg.h"
#include "../../NInterface/NFriendDlg/NFriendDlg.h"

#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../../NInterface/NItemInfoDlg/ItemInfoDlg.h"
#include "../../NInterface/NHomePageDlg/HomePageDlg.h"
#include "../../NInterface/NHuntMapInfoDlg/HuntMapInfoDlg.h"
#include "../../NInterface/NResidentsInfoDlg/ResidentsInfoDlg.h"
#include "../../NInterface/NMagicInfoDlg/MagicInfoDlg.h"
#include "../../NInterface/NTeacherDlg/TeacherDlg.h"
#include "../../NInterface/NTeacherDlg/FatherDlg.h"
#include "../../NInterface/NTeacherDlg/ChildDlg.h"
#include "../../NInterface/NItemMallManager/NItemMallManager1.h"
#include "../../ninterface/ncharmenubardlg/NCharMenuBarDlg.h"

#include "../../NInterface/NCommonMsgDlg/NCommonMsgDlg.h"

#include "../../../Common/Father/Father.h"
#include "../../../common/HuntMapInfo/HuntMapInfo.h"
#include "../../../common/Master/Master.h"
#include "../../NInterface/NMaster/NMaster.h"

#include "../../../common/HintDlg/HintDlg.h"

#include "../../../lib/ScreenShot/CaptureJPG.h"

// 마을 상세 정보 
#include "NInterface/NVillageDetailInfo/NVillageDetailInfo.h"
#include "NInterface/NResidentsDlg/NResidentsDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/Static.h"

#include "Msg/MsgType-PersonalShop.h"
#include "Msg/MsgType-ItemMall.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "NInterface/NCharStatus/NCharStatus.h"
#include "NInterface/NBossMarket/NBossMarketDlg.h"

#include "../lib/MD5/md5.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;

CNCharMenuBarDlg::CNCharMenuBarDlg() : CDialogController()
{
	for ( SI16 i = 0; i < MAX_NCHARMENUBARDLG_BUTTON_NUM; ++i )
	{
		m_pToolBarButton[ i ] = NULL;
	}
}

CNCharMenuBarDlg::~CNCharMenuBarDlg()
{
	for ( SI16 i = 0; i < MAX_NCHARMENUBARDLG_BUTTON_NUM; ++i )
	{
		SAFE_DELETE( m_pToolBarButton[ i ] );
	}
}

void CNCharMenuBarDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;
	//gsiResolution = 2 =  1024 X 768
	//gsiResolution = 4 =  1024x600
	if ( gsiResolution == 2 || gsiResolution == 4)
	{
		SI16 yPos =  pclclient->GetScreenYSize() - 50;
		SI16 ButtonyPos =  3;
		SI16 Height = 26;

		// 1024x600
		if(gsiResolution == 4)
		{
			//Height = 16;
			//ButtonyPos =  -7;
		}

		CDialogController::Create( NCHARMENUBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, TEXT("NInterface/Interface/INTERFACE_616X26_00_000.SPR"), 0, true, 3,
			MAP_TILE_XSIZE + 344 + (g_SoundOnOffDlg.m_bMaxWindowMode ? 32 : 0) ,yPos, 616, Height, TRUE, false, false,true );
		//CDialogController::Create( NCHARMENUBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogToolBar, NULL, TEXT("NInterface/Interface/Interface_1024x49_00_000.SPR"), 0, true, 3, 80, pclclient->GetScreenYSize() - 249, 1024, 49, false, false, false );
		NTCHARString128	kText;
		
		// 상태정보
		kText = GetTxtFromMgr(8331);
		m_pToolBarButton[ 0 ] = new CButton( this );
		m_pToolBarButton[ 0 ]->Create( NCHARMENUBAR_DIALOG_BUTTON_0, kText, TEXT("NInterface/Button/BUTTON_111x20_00_000.SPR"), 0, false, 0, 25, ButtonyPos, 111, 20, 111, 20 );
		m_pToolBarButton[ 0 ]->SetUseToolTip("Alt+A");
		m_pToolBarButton[ 0 ]->SetFontHeight(11);
		m_pToolBarButton[ 0 ]->Refresh();

		// 착용정보
		kText = GetTxtFromMgr(8332);
		m_pToolBarButton[ 1 ] = new CButton( this );
		m_pToolBarButton[ 1 ]->Create( NCHARMENUBAR_DIALOG_BUTTON_0 + 1, kText, TEXT("NInterface/Button/BUTTON_111x20_00_000.SPR"), 4, false, 0, 138, ButtonyPos, 111, 20, 111, 20 );
		m_pToolBarButton[ 1 ]->SetUseToolTip("Alt+E");
		m_pToolBarButton[ 1 ]->SetFontHeight(11);
		m_pToolBarButton[ 1 ]->Refresh();

		// 소지품정보
		kText = GetTxtFromMgr(8333);
		m_pToolBarButton[ 2 ] = new CButton( this );
		m_pToolBarButton[ 2 ]->Create( NCHARMENUBAR_DIALOG_BUTTON_0 + 2, kText, TEXT("NInterface/Button/BUTTON_111x20_00_000.SPR"), 8, false, 0, 251, ButtonyPos, 111, 20, 111, 20 );
		m_pToolBarButton[ 2 ]->SetUseToolTip("Alt+I");
		m_pToolBarButton[ 2 ]->SetFontHeight(11);
		m_pToolBarButton[ 2 ]->Refresh();

		// 게임정보
		kText = GetTxtFromMgr(8334);
		m_pToolBarButton[ 3 ] = new CButton( this );
		m_pToolBarButton[ 3 ]->Create( NCHARMENUBAR_DIALOG_BUTTON_0 + 3, kText, TEXT("NInterface/Button/BUTTON_111x20_00_000.SPR"), 12, false, 0, 364, ButtonyPos, 111, 20, 111, 20 );
		m_pToolBarButton[ 3 ]->SetFontHeight(11);
		m_pToolBarButton[ 3 ]->Refresh();

		// 아이템몰
		kText = GetTxtFromMgr(8335);
		m_pToolBarButton[ 4 ] = new CButton( this );
		m_pToolBarButton[ 4 ]->Create( NCHARMENUBAR_DIALOG_BUTTON_0 + 4, kText, TEXT("NInterface/Button/BUTTON_111x20_00_000.SPR"), 16, false, 0, 477, ButtonyPos, 111, 20, 111, 20 );
		m_pToolBarButton[ 4 ]->SetFontHeight(11);
		m_pToolBarButton[ 4 ]->Refresh();
	}
	// 800 X 600
	else
	{
	
	}

	if(pclClient->IsWhereServiceArea(ConstServiceArea_Korea))
	{
		for(SI32 i=0; i<MAX_NCHARMENUBARDLG_BUTTON_NUM; i++)
		{
			if(m_pToolBarButton[ i ] == NULL)		continue;

			m_pToolBarButton[ i ]->SetFontKind(TEXT("TimeNTales"));
			m_pToolBarButton[ i ]->SetFontHeight(15);
			m_pToolBarButton[ i ]->Refresh();
		}
	}

	SetTopMostWindow( false );
	SetActionTimer( 100 );
	SetTimerOnOff( true );
}


void CNCharMenuBarDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;


	//pclclient->pclLogo->Draw( pclclient->GetScreenXSize(), pclclient->GetScreenYSize(), pclclient->GetScreenRX(), pclclient->GetScreenRY() );

	return;
}

void CALLBACK CNCharMenuBarDlg::StaticCallBackDialogBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNCharMenuBarDlg *pThis = (CNCharMenuBarDlg *)pControllerMgr;
	pThis->CallBackDialogBar( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNCharMenuBarDlg::CallBackDialogBar( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = ( cltClient * ) pclClient;
	switch( nEvent )
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch ( nControlID )
			{
			case NCHARMENUBAR_DIALOG_BUTTON_0:	// 상태정보 버튼
				{
					pclclient->CreateCharStatusDlg();
				}
				break;
			case NCHARMENUBAR_DIALOG_BUTTON_0+1:	// 착용정보 버튼
				{
					if (pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ] )
					{
						if ( pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ]->IsShow() == false )
						{
							((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->Show();
						}
						else
						{
							((CNEquipWindow*)pclclient->m_pDialog[ NNEWEQUIPWINDOW_DLG ])->Hide();
						}
					}
				}
				break;
			case NCHARMENUBAR_DIALOG_BUTTON_0+2:	// 소지품정보 버튼
				{
					if (pclclient->m_pDialog[ NNEWINVENTORY_DLG ] )
					{
						if ( pclclient->m_pDialog[ NNEWINVENTORY_DLG ]->IsShow() == false )
						{
							((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->Show();
						}
						else
						{
							((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->Hide();
						}
					}
				}
				break;
			case NCHARMENUBAR_DIALOG_BUTTON_0+3:	// 게임정보 버튼
				{
					pclclient->CreateInterface(NINFO_DLG);
				}
				break;
			case NCHARMENUBAR_DIALOG_BUTTON_0+4:	// 아이템몰 버튼
				{
					//////////////////////////////////////////////////////////////////////////
					//////////////		GM만 볼수있다		//////////////////////////////
					//////////////////////////////////////////////////////////////////////////
					//	if(pclclient->pclCM->CR[1]->GetGameMasterLevel() != 101 )
					//	break;
					
					// itemmall-
					
					pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_OPENITEMMALL );

					if( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
					{
						if( BASIC_PLAYERMASTER_LEVEL <= pclClient->pclCM->CR[1]->GetGameMasterLevel() )
						{
							TCHAR* pTitle = GetTxtFromMgr(4314);
							TCHAR* pText = GetTxtFromMgr(7547);

							stMsgBox MsgBox;
							MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
							pclclient->SetMsgBox( &MsgBox, NULL, 0 );
							break;
						}
						else
						{
							pclclient->ShowHelpWindow(TEXT("NULL"));
							break;
						}
					}
					else
					{
						if( pclClient->bItemMallUseSwitch == FALSE)
						{	
							TCHAR* pText = GetTxtFromMgr(962);

							if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
							{
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
								break;
							}
						}
						else
						{
							if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
							{
								if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
								{
									cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

									UI08 md5buf[256] = "" ;
									char buffermd5[256] = "" ;

									TCHAR	szURL[1024];
									TCHAR	szBeforeMD5[256];

									sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");

									memset(md5buf, 0, sizeof(md5buf));
									// MD5
									SI32 md5size = _tcslen(szBeforeMD5);
									memcpy(md5buf, szBeforeMD5, md5size);
									md5_string(md5buf, buffermd5, md5size);

									sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
										100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);

									g_LoginNoticeDlg.Set(szURL, 970, 756);
								}
							}
							else
							{
								pclclient->CreateInterface(NNEWITEMMALL_DLG);	
							}
							break;
						}	
					}
				}
				break;			
			}
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
		}
		break;
	}
}

void CNCharMenuBarDlg::Action()
{
	cltClient *pclclient = (cltClient *)pclClient;
	{
		// NSysMenu_Dlg , NCharMenu_Dlg , STATUSBAR_DLG, 외곽선 과의 스냅 체크
		
		SI16 distanceX, distanceY; //대상과의 거리
		SI16 destX, destY; // 대상의 좌표
		destX = MAP_TILE_XSIZE + 344 + (g_SoundOnOffDlg.m_bMaxWindowMode ? 32 : 0);  // 344 는 채팅바다이얼로그의 넓이
		distanceX = GetX() - destX;
		destY = pclclient->GetScreenYSize() - 24 - GetHeight();  // 24는 스테이터스바 높이
		distanceY =  GetY() - destY ;
	
		if(abs(distanceY) < SNAPRANGE ) 
		{
			MovePos(GetX(),destY);
			if(abs(distanceX) < SNAPRANGE )
			{
				MovePos(destX,GetY());
			}
		}	
	}
	return;
}

void CNCharMenuBarDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				if( pclClient->bItemMallUseSwitch == FALSE)
				{	
					TCHAR* pText = GetTxtFromMgr(962);

					if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
					{
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
					}
				}
				else
				{
					if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
					{
						if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
						{
							cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

							UI08 md5buf[256] = "" ;
							char buffermd5[256] = "" ;

							TCHAR	szURL[1024];
							TCHAR	szBeforeMD5[256];

							sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");

							memset(md5buf, 0, sizeof(md5buf));
							// MD5
							SI32 md5size = _tcslen(szBeforeMD5);
							memcpy(md5buf, szBeforeMD5, md5size);
							md5_string(md5buf, buffermd5, md5size);

							sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
								100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);

							g_LoginNoticeDlg.Set(szURL, 970, 756);
						}
					}
					else
					{
						pclclient->CreateInterface(NNEWITEMMALL_DLG);	
					}
				}	
			}
		}
		break;
	}
}
