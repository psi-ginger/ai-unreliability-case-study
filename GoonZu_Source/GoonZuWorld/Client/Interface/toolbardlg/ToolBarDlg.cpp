#include "ToolBarDlg.h"

#include "../../../Client/ItemInfo/ItemInfo.h"

#include "../../Inventory/ItemInventory.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../Interface/NInventory/NInventory.h"
//#include "../../Interface/MonsterInfoDlg/MonsterInfoDlg.h"

#include "../../Client.h"
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
#include "../../NInterface/NMagicInfoDlg/NewMagicInfoDlg.h"
#include "../../NInterface/NTeacherDlg/TeacherDlg.h"
#include "../../NInterface/NTeacherDlg/FatherDlg.h"
#include "../../NInterface/NTeacherDlg/ChildDlg.h"
#include "../../NInterface/NItemMallManager/NItemMallManager1.h"

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

#include "../../Client.h"
extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;

CToolBarDlg::CToolBarDlg() : CDialogController()
{
	for ( SI16 i = 0; i < MAX_TOOLBARDLG_BUTTON_NUM; ++i )
	{
		m_pToolBarButton[ i ] = NULL;
	}

	m_pDateStatic = NULL;
	m_pMoneyStatic = NULL;
	m_pPositionStatic = NULL;
	m_pQuitButton = NULL;
}

CToolBarDlg::~CToolBarDlg()
{
	for ( SI16 i = 0; i < MAX_TOOLBARDLG_BUTTON_NUM; ++i )
	{
		SAFE_DELETE( m_pToolBarButton[ i ] );
	}

	SAFE_DELETE( m_pQuitButton );

	SAFE_DELETE( m_pDateStatic );
	SAFE_DELETE( m_pMoneyStatic );
	SAFE_DELETE( m_pPositionStatic );
}

void CToolBarDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;
	// 1024 X 768
	if ( gsiResolution == 2 )
	{

		CDialogController::Create( TOOLBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogToolBar, NULL, TEXT("NInterface/Interface/Interface_752x34_00_000.SPR"), 0, true, 3, 80, 40, 752, 34, false, false, false );

		// 상태정보
		m_pToolBarButton[ 0 ] = new CButton( this );
		m_pToolBarButton[ 0 ]->Create( TOOLBAR_DIALOG_BUTTON_0, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 0, false, 0, 252, 0, 34, 33, 34, 33 );

		// 인벤
		m_pToolBarButton[ 1 ] = new CButton( this );
		m_pToolBarButton[ 1 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 1, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 4, false, 0, 286, 0, 34, 33, 34, 33 );

		// 스킬
		m_pToolBarButton[ 2 ] = new CButton( this );
		m_pToolBarButton[ 2 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 2, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 8, false, 0, 320, 0, 34, 33, 34, 33 );

		// 아이템몰
		m_pToolBarButton[ 3 ] = new CButton( this );
		m_pToolBarButton[ 3 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 3, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 12, false, 0, 354, 0, 34, 33, 34, 33 );

		// 아이템 수령
		m_pToolBarButton[ 4 ] = new CButton( this );
		m_pToolBarButton[ 4 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 4, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 16, false, 0, 388, 0, 34, 33, 34, 33 );

		m_pToolBarButton[ 5 ] = new CButton( this );
		m_pToolBarButton[ 5 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 5, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 20, false, 0, 422, 0, 34, 33, 34, 33 );

		m_pToolBarButton[ 6 ] = new CButton( this );
		m_pToolBarButton[ 6 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 6, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 24, false, 0, 456, 0, 34, 33, 34, 33 );


		m_pQuitButton = new CButton( this );
		m_pQuitButton->Create( TOOLBAR_DIALOG_BUTTON_QUIT, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 112, false, 0, pclclient->GetScreenXSize() - 214 - 80 - 30, 4, 20, 20, 20, 20 );
		m_pQuitButton->Show(false);

		m_pDateStatic = new CStatic( this );
		if( pclClient->siServiceArea == ConstServiceArea_Korea )
		{
			m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, NULL, 0, true, 0, 15, 3, 123, 15, 123, 15 );
		}
		else if( pclClient->siServiceArea == ConstServiceArea_China )
		{
			m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, NULL, 0, true, 0, 15, 3, 123, 15, 123, 15 );
		}		
		else
		{
			m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, NULL, 0, true, 0, 15, 3, 123, 15, 123, 15 );
		}

		m_pMoneyStatic = new CStatic( this );
		m_pMoneyStatic->Create( TOOLBAR_DIALOG_MONEY_STATIC, NULL, NULL, 0, true, 0, 130, 3, 132, 15, 132, 15 );

		m_pPositionStatic = new CStatic( this );
		m_pPositionStatic->Create( TOOLBAR_DIALOG_POSITION_STATIC, NULL, NULL, 0, true, 3, 520, 0, 102, 15, 102, 15 );
		

		TCHAR* pText = GetTxtFromMgr(5069);
		m_pToolBarButton[ 0 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5070);
		m_pToolBarButton[ 1 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5071);
		m_pToolBarButton[ 2 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5109);
		m_pToolBarButton[ 3 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5072);
		m_pToolBarButton[ 4 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5073);
		m_pToolBarButton[ 5 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5253);
		m_pToolBarButton[ 6 ]->SetUseToolTip( pText );

	}
	// 800 X 600
	else
	{

		CDialogController::Create( TOOLBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogToolBar, NULL, TEXT("NInterface/Interface/Interface_592x34_00_000.SPR"), 0, true, 3, 80, 40, 592, 34, false, false, false );

		// 상태정보
		m_pToolBarButton[ 0 ] = new CButton( this );
		m_pToolBarButton[ 0 ]->Create( TOOLBAR_DIALOG_BUTTON_0, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 0, false, 0, 252, 0, 34, 33, 34, 33 );

		// 인벤
		m_pToolBarButton[ 1 ] = new CButton( this );
		m_pToolBarButton[ 1 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 1, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 4, false, 0, 286, 0, 34, 33, 34, 33 );

		// 스킬
		m_pToolBarButton[ 2 ] = new CButton( this );
		m_pToolBarButton[ 2 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 2, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 8, false, 0, 320, 0, 34, 33, 34, 33 );

		// 아이템몰
		m_pToolBarButton[ 3 ] = new CButton( this );
		m_pToolBarButton[ 3 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 3, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 12, false, 0, 354, 0, 34, 33, 34, 33 );

		// 아이템 수령
		m_pToolBarButton[ 4 ] = new CButton( this );
		m_pToolBarButton[ 4 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 4, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 16, false, 0, 388, 0, 34, 33, 34, 33 );

		m_pToolBarButton[ 5 ] = new CButton( this );
		m_pToolBarButton[ 5 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 5, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 20, false, 0, 422, 0, 34, 33, 34, 33 );

		m_pToolBarButton[ 6 ] = new CButton( this );
		m_pToolBarButton[ 6 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 6, NULL, TEXT("NInterface/Button/Button_34x33_00_000.SPR"), 24, false, 0, 456, 0, 34, 33, 34, 33 );


		m_pQuitButton = new CButton( this );
		m_pQuitButton->Create( TOOLBAR_DIALOG_BUTTON_QUIT, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 112, false, 0, pclclient->GetScreenXSize() - 214 - 80 - 30, 4, 20, 20, 20, 20 );
		m_pQuitButton->Show(false);

		m_pDateStatic = new CStatic( this );
		if( pclClient->siServiceArea == ConstServiceArea_Korea )
		{
			m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, NULL, 0, true, 0, 15, 3, 123, 15, 123, 15 );
		}
		else if( pclClient->siServiceArea == ConstServiceArea_China )
		{
			m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, NULL, 0, true, 0, 15, 3, 123, 15, 123, 15 );
		}		
		else
		{
			m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, NULL, 0, true, 0, 15, 3, 123, 15, 123, 15 );
		}

		m_pMoneyStatic = new CStatic( this );
		m_pMoneyStatic->Create( TOOLBAR_DIALOG_MONEY_STATIC, NULL, NULL, 0, true, 0, 130, 3, 132, 15, 132, 15 );

		m_pPositionStatic = new CStatic( this );
		m_pPositionStatic->Create( TOOLBAR_DIALOG_POSITION_STATIC, NULL, TEXT("NInterface/Common/Common_4X17_00_103.SPR"), 3, true, 3, 446, 7, 102, 15, 102, 15 );
		m_pPositionStatic->Show(false);

		TCHAR* pText = GetTxtFromMgr(5069);
		m_pToolBarButton[ 0 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5070);
		m_pToolBarButton[ 1 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5071);
		m_pToolBarButton[ 2 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5109);
		m_pToolBarButton[ 3 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5072);
		m_pToolBarButton[ 4 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5073);
		m_pToolBarButton[ 5 ]->SetUseToolTip( pText );

		pText = GetTxtFromMgr(5253);
		m_pToolBarButton[ 6 ]->SetUseToolTip( pText );
		/*		CDialogController::Create( TOOLBAR_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogToolBar, NULL, TEXT("NInterface/Interface/Interface_10x27_00_103.SPR"), 3, true, 3, 80, 40, 586, 27, false, false, false );

		// 상태정보
		m_pToolBarButton[ 0 ] = new CButton( this );
		m_pToolBarButton[ 0 ]->Create( TOOLBAR_DIALOG_BUTTON_0, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 4, false, 0, 318, 4, 20, 20, 20, 20 );

		// 인벤
		m_pToolBarButton[ 1 ] = new CButton( this );
		m_pToolBarButton[ 1 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 1, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 12, false, 0, 342, 4, 20, 20, 20, 20 );

		// 스킬
		m_pToolBarButton[ 2 ] = new CButton( this );
		m_pToolBarButton[ 2 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 2, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 16, false, 0, 366, 4, 20, 20, 20, 20 );

		// 아이템몰
		m_pToolBarButton[ 3 ] = new CButton( this );
		m_pToolBarButton[ 3 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 3, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 108, false, 0, 390, 4, 20, 20, 20, 20 );

		// 아이템 수령
		m_pToolBarButton[ 4 ] = new CButton( this );
		m_pToolBarButton[ 4 ]->Create( TOOLBAR_DIALOG_BUTTON_0 + 4, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 68, false, 0, 414, 4, 20, 20, 20, 20 );

		m_pQuitButton = new CButton( this );
		m_pQuitButton->Create( TOOLBAR_DIALOG_BUTTON_QUIT, NULL, TEXT("NInterface/Button/BUTTON_20X20_00_000.SPR"), 112, false, 0, pclclient->GetScreenXSize() - 214 - 80 - 30, 4, 20, 20, 20, 20 );


		m_pDateStatic = new CStatic( this );
		if( pclClient->siServiceArea == ConstServiceArea_Korea )
		{
		m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, TEXT("NInterface/Common/Common_4X17_00_103.SPR"), 3, true, 3, 31, 7, 123, 15, 123, 15 );
		}
		else if( pclClient->siServiceArea == ConstServiceArea_China )
		{
		m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, TEXT("NInterface/Common/Common_4X17_00_103.SPR"), 3, true, 3, 3 ,7, 160, 15, 160, 15 );
		}		
		else
		{
		m_pDateStatic->Create( TOOLBAR_DIALOG_DATE_STATIC, NULL, TEXT("NInterface/Common/Common_4X17_00_103.SPR"), 3, true, 3, 3 ,7, 160, 15, 160, 15 );
		}

		m_pMoneyStatic = new CStatic( this );
		m_pMoneyStatic->Create( TOOLBAR_DIALOG_MONEY_STATIC, NULL, TEXT("NInterface/Common/Common_4X17_00_103.SPR"), 3, true, 3, 175, 7, 132, 15, 132, 15 );

		m_pPositionStatic = new CStatic( this );
		m_pPositionStatic->Create( TOOLBAR_DIALOG_POSITION_STATIC, NULL, TEXT("NInterface/Common/Common_4X17_00_103.SPR"), 3, true, 3, 446, 7, 102, 15, 102, 15 );
		*/
	}

	SetTopMostWindow( true );
	SetTimerOnOff( true );

	return;
}

void CALLBACK CToolBarDlg::StaticCallBackDialogToolBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CToolBarDlg *pThis = (CToolBarDlg *)pControllerMgr;
	pThis->CallBackDialogToolBar( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CToolBarDlg::CallBackDialogToolBar( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = ( cltClient * ) pclClient;
	switch( nEvent )
	{
	case EVENT_BUTTON_CLICKED:
		{
			switch ( nControlID )
			{
			case TOOLBAR_DIALOG_BUTTON_0:
				{
					// _LEON_NCHARSTATUS_
					pclclient->CreateCharStatusDlg();
				}
				break;
			case TOOLBAR_DIALOG_BUTTON_0+1:
				{
					//if ( g_SoundOnOffDlg.m_bFullScreenMode && pclclient->m_pDialog[ NINVENTORY_DLG ] )
					//{
					//	if ( pclclient->m_pDialog[ NINVENTORY_DLG ]->IsShow() == false )
					//	{
					//		((CNInventory*)pclclient->m_pDialog[ NINVENTORY_DLG ])->Show();
					//	}
					//	else
					//	{
					//		((CNInventory*)pclclient->m_pDialog[ NINVENTORY_DLG ])->Hide();
					//	}
					//}
				}
				break;
			case TOOLBAR_DIALOG_BUTTON_0+2:
				{
					if ( pclclient->GetGameStatus() == GS_GAME )
					{
						if( pclclient->pclSkillManager)
						{
							if ( g_SoundOnOffDlg.m_bFullScreenMode )
							{
								pclclient->CreateInterface( NSKILL_DLG );
							}
							else
							{
								pclclient->CreateSkillDlg(0);
							}
						}
					}
				}
				break;
			case TOOLBAR_DIALOG_BUTTON_0+3:
				{
					if( pclClient->IsCountrySwitch(Switch_SkillBook) )	
					{
						/*
						if ( ((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->IsShow() == false )
						{
							((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->Show();
						}
						else
						{
							((CNNewMagicInfoDlg*)pclclient->m_pDialog[ NNEWMAGICINFO_DLG ])->Hide();
						}
						*/
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
			case TOOLBAR_DIALOG_BUTTON_0+4:
				{	
					/*
					if( pclClient->bItemMallUseSwitch == FALSE)
					{	
					TCHAR* pText = GetTxtFromMgr(962);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
					}
					else
					{
					if( g_bFullScreen )
					{
					ShowWindow( pclclient->GetHwnd(), SW_MINIMIZE );
					}
					pclClient->pclItemMallManager->CreateItemMallDlg( pclclient->GetHwnd(), pclclient->GetInstance(), 0 );
					}
					*/
				}
				break;
			case TOOLBAR_DIALOG_BUTTON_0+5:
				{	
					/*
					if( pclClient->bItemMallUseSwitch == FALSE)
					{	
					TCHAR* pText = GetTxtFromMgr(962);

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
					}
					else
					{
					if ( g_SoundOnOffDlg.m_bFullScreenMode )
					{						
					if(!pclclient->m_pDialog[NITEMMALL_DLG])
					{
					pclclient->CreateInterface(NITEMMALL_DLG);
					}

					cltGameMsgRequest_ItemMallItemList clinfo(1);
					cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					else
					{

					if(pclClient->pclItemMallManager->CreateItemMallDlg( pclclient->GetHwnd(), pclclient->GetInstance(), 1 ) )
					{				
					cltGameMsgRequest_ItemMallItemList clinfo(1);
					cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
					}
					}
					*/
				}
				break;

			case TOOLBAR_DIALOG_BUTTON_0+6: 
				{
					/*
					if ( g_bFullScreen )
					{
					ShowWindow( pclClient->GetHwnd(), SW_MINIMIZE );
					}

					ShellExecute( pclClient->GetHwnd(), NULL, TEXT("Miniwinamp/MiniWinamp.exe"), NULL, NULL, SW_SHOWNORMAL );
					*/
				}
				break;
			case TOOLBAR_DIALOG_BUTTON_QUIT:
				{
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
				break;
			}
		}
		break;
	}
}

// 돈 정보랑 시간 정보는 수시 업데이트 해야 되기 땜시..
void CToolBarDlg::Action()
{
	// 주기적( 1 초당 한번 정도 )으로 업데이트 시키면 되므로...
	if ( pclClient && pclClient->pclCM )
	{
		// 현재 가진 돈
		GMONEY Money = pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney();
		TCHAR Buffer[256 ] = TEXT("");
		//sprintf( Buffer, TEXT("%d"), Money );
		TCHAR* pText = GetTxtFromMgr(4837);
		g_JYLibFunction.SetNumUnit( Money, Buffer, 256, pText );

		m_pMoneyStatic->SetText( Buffer, DT_CENTER|DT_VCENTER|DT_SINGLELINE );
	}

	return;
}

void CToolBarDlg::SetDate( TCHAR *strDate )
{
	m_pDateStatic->SetText( strDate, DT_CENTER|DT_VCENTER|DT_SINGLELINE );

	return;
}

void CToolBarDlg::SetPosition( TCHAR *strDate )
{

	if( pclClient->siServiceArea == ConstServiceArea_English	||
		pclClient->siServiceArea == ConstServiceArea_NHNChina	||
		pclClient->siServiceArea == ConstServiceArea_EUROPE
	)
	{
		m_pPositionStatic->SetFontHeight(10);
	}

	m_pPositionStatic->SetText( strDate, DT_CENTER|DT_VCENTER|DT_SINGLELINE );    


	return;
}

