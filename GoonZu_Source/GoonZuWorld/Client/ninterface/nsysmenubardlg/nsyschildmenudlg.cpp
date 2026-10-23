#include "NSysChildMenuDlg.h"

#include "../../../Client/ItemInfo/ItemInfo.h"

#include "../../Inventory/ItemInventory.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../Interface/NInventory/NInventory.h"
//#include "../../Interface/MonsterInfoDlg/MonsterInfoDlg.h"

#include "../../Client.h"

#include "../../Prompt/PromptMgr.h"

#include "../../NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

#include "../../Interface/ResidentsViewDlg/ResidentsViewDlg.h"
#include "../../Interface/FriendDlg/FriendDlg.h"
#include "../../NInterface/NFriendDlg/NFriendDlg.h"
#include "../../NInterface/NFriendDlg/NNewFriendDlg.h"

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
#include "../../NInterface/NMiniMapDlg/NMiniMapDlg.h"

#include "../../NInterface/NCommonMsgDlg/NCommonMsgDlg.h"

#include "../../../Common/Father/Father.h"
#include "../../../common/HuntMapInfo/HuntMapInfo.h"
#include "../../../common/Master/Master.h"
#include "../../NInterface/NMaster/NMaster.h"

#include "NInterface/NHintDlg/NHintDlg.h"

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
#include "Msg/MsgType-Web.h"

// 힌트
#include "../../../common/HintDlg/HintDlg.h"
//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include "NInterface/NCharStatus/NCharStatus.h"
#include "NInterface/NBossMarket/NBossMarketDlg.h"
#include "ninterface/nsysmenubardlg/NSysMenuBarDlg.h"

#include "../lib/MD5/md5.h"

#include "../../Client.h"
extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;
extern SI16		g_siChannelingCode;

CNSysChildMenuDlg::CNSysChildMenuDlg() : CDialogController()
{
	for ( SI16 i = 0; i < MAX_NSYSCHILDMENUBARDLG_BUTTON_NUM; ++i )
	{
		m_pChildMenuButton[ i ] = NULL;
	}
}

CNSysChildMenuDlg::~CNSysChildMenuDlg()
{
	for ( SI16 i = 0; i < MAX_NSYSCHILDMENUBARDLG_BUTTON_NUM; ++i )
	{
		SAFE_DELETE( m_pChildMenuButton[ i ] );
	}
}

void CNSysChildMenuDlg::Create()
{
	
	return;
}


void CNSysChildMenuDlg::Set(SI16 SelectedChildMenu)
{
 	m_siSelectedChildMenuId = SelectedChildMenu;

	cltClient *pclclient = (cltClient *)pclClient;
	switch( SelectedChildMenu ) 
	{
		case CHILDMENU_ID_INFO:
			{ 
				CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 420, 26, false, false, false );

				// 버튼 다섯개 만들기
				m_pChildMenuButton[0] = new CButton( this );
				m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,
				TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),0,false,0,0,0,64,22,64,22 );
				
				m_pChildMenuButton[1] = new CButton( this );
				m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,
					TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),4,false,0,71,0,64,22,64,22 );
				
				m_pChildMenuButton[2] = new CButton( this );
				m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,
					TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),8,false,0,142,0,64,22,64,22 );
				
				m_pChildMenuButton[3] = new CButton( this );
				m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,
					TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),12,false,0,213,0,64,22,64,22 );
				
				m_pChildMenuButton[4] = new CButton( this );
				m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,
					TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),16,false,0,284,0,64,22,64,22 );
			}
			break;

		case CHILDMENU_ID_COMMUNITY:
			{
				if( pclClient->siServiceArea == ConstServiceArea_Japan ){
   	 		 		CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, 
						NULL, NULL, 0, false, 0, 100 , 100, 375, 50, false, false, false );
					// 버튼 다섯개 만들기

					m_pChildMenuButton[0] = new CButton( this );
					m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),20,false,0,20,26,64,22,64,22 );
					m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+F"));

					m_pChildMenuButton[1] = new CButton( this );
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),24,false,0,91,26,64,22,64,22 );

					m_pChildMenuButton[2] = new CButton( this );
					m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),28,false,0,162,26,64,22,64,22 );
					m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+T"));

					m_pChildMenuButton[3] = new CButton( this );
					m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),32,false,0,233,26,64,22,64,22 );
					m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+C"));

					m_pChildMenuButton[8] = new CButton( this );
					m_pChildMenuButton[8]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 8, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),76,false,0,304,26,64,22,64,22 );


					m_pChildMenuButton[5] = new CButton( this );
					m_pChildMenuButton[5]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 5, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),60,false,0,50,0,64,22,64,22 );

			
					m_pChildMenuButton[7] = new CButton( this );
					m_pChildMenuButton[7]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 7, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),40,false,0,121,0,64,22,64,22 );
					

					m_pChildMenuButton[4] = new CButton( this );
					m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),36,false,0,192,0,64,22,64,22 );
					m_pChildMenuButton[4]->SetUseToolTip(TEXT("Alt+V"));
					
					/*
					m_pChildMenuButton[6] = new CButton( this );
					m_pChildMenuButton[6]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 6, NULL,
						TEXT("NInterface/Button/InterfaceButton_98x20_sysmenu.SPR"),4,false,0,213,0,98,22,98,22 );
					*/
										
					//m_pChildMenuButton[0] = new CButton( this );
					//m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,
					//	TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),20,false,0,0,26,64,22,64,22 );
					//m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+F"));

					//m_pChildMenuButton[1] = new CButton( this );
					//m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,
					//	TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),24,false,0,71,26,64,22,64,22 );

					//m_pChildMenuButton[2] = new CButton( this );
					//m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,
					//	TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),28,false,0,142,26,64,22,64,22 );
					//m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+T"));
					//
					//m_pChildMenuButton[3] = new CButton( this );
					//m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,
					//	TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),32,false,0,213,26,64,22,64,22 );
					//m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+C"));
					//
					//m_pChildMenuButton[4] = new CButton( this );
					//m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,
					//	TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),36,false,0,0,0,64,22,64,22 );
					//m_pChildMenuButton[4]->SetUseToolTip(TEXT("Alt+V"));

					//
					//m_pChildMenuButton[6] = new CButton( this );
					//m_pChildMenuButton[6]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 6, NULL,
					//	TEXT("NInterface/Button/InterfaceButton_98x20_sysmenu.SPR"),4,false,0,142,0,64,22,64,22 );
				

					//m_pChildMenuButton[7] = new CButton( this );
					//m_pChildMenuButton[7]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 7, NULL,
					//	TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),40,false,0,71,0,64,22,64,22 );		
				}
				else if( pclClient->siServiceArea == ConstServiceArea_Korea )
				{
 					CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 284, 50, false, false, false );
					// 버튼  만들기
					m_pChildMenuButton[0] = new CButton( this );
					m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),16,false,0,0,26,64,22,64,22 );
					m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+F"));

					m_pChildMenuButton[1] = new CButton( this );
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),20,false,0,71,26,64,22,64,22 );

					m_pChildMenuButton[2] = new CButton( this );
					m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),24,false,0,142,0,64,22,64,22 );
					m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+T"));

					m_pChildMenuButton[3] = new CButton( this );
					m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),28,false,0,213,26,64,22,64,22 );
					m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+C"));

					m_pChildMenuButton[4] = new CButton( this );
					m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),32,false,0,0,0,64,22,64,22 );
					m_pChildMenuButton[4]->SetUseToolTip(TEXT("Alt+V"));


					m_pChildMenuButton[6] = new CButton( this );
					m_pChildMenuButton[6]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 6, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),8,false,0,71,0,64,22,64,22 );


					m_pChildMenuButton[7] = new CButton( this );
					m_pChildMenuButton[7]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 7, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),4,false,0,142,26,64,22,64,22 );	

					m_pChildMenuButton[8] = new CButton( this );
					m_pChildMenuButton[8]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 8, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),12,false,0,213,0,64,22,64,22 );

				}
				else if( pclClient->siServiceArea == ConstServiceArea_China || pclClient->siServiceArea == ConstServiceArea_Taiwan )
				{
					CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 284, 50, false, false, false );
					// 버튼 다섯개 만들기
					m_pChildMenuButton[0] = new CButton( this );
					m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),80,false,0,0,26,64,22,64,22 );
					m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+F"));

					m_pChildMenuButton[1] = new CButton( this );
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),24,false,0,71,26,64,22,64,22 );

					m_pChildMenuButton[2] = new CButton( this );
					m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),28,false,0,213,26,64,22,64,22 );
					m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+T"));

					m_pChildMenuButton[3] = new CButton( this );
					m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),32,false,0,142,26,64,22,64,22 );
					m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+C"));

					m_pChildMenuButton[4] = new CButton( this );
					m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),36,false,0,0,0,64,22,64,22 );
					m_pChildMenuButton[4]->SetUseToolTip(TEXT("Alt+V"));


					m_pChildMenuButton[6] = new CButton( this );
					m_pChildMenuButton[6]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 6, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),92,false,0,213,0,64,22,64,22 );

					m_pChildMenuButton[7] = new CButton( this );
					m_pChildMenuButton[7]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 7, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),4,false,0,71,0,64,22,64,22 );
					m_pChildMenuButton[8] = new CButton( this );
					m_pChildMenuButton[8]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 8, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),96,false,0,142,0,64,22,64,22 );

				}
				else
				{

					CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 355, 50, false, false, false );
					// 버튼 다섯개 만들기

					m_pChildMenuButton[0] = new CButton( this );
					m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),20,false,0,0,26,64,22,64,22 );
					m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+F"));


					m_pChildMenuButton[1] = new CButton( this );
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),24,false,0,71,26,64,22,64,22 );

					m_pChildMenuButton[2] = new CButton( this );
					m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),28,false,0,142,26,64,22,64,22 );
					m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+T"));

					m_pChildMenuButton[3] = new CButton( this );
					m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),32,false,0,213,26,64,22,64,22 );
					m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+C"));

					m_pChildMenuButton[4] = new CButton( this );
					m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),36,false,0,284,26,64,22,64,22 );
					m_pChildMenuButton[4]->SetUseToolTip(TEXT("Alt+V"));

					m_pChildMenuButton[5] = new CButton( this );
					m_pChildMenuButton[5]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 5, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),60,false,0,0,0,64,22,64,22 );


					m_pChildMenuButton[7] = new CButton( this );
					m_pChildMenuButton[7]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 7, NULL,
						TEXT("NInterface/Button/InterfaceButton_98x20_sysmenu.SPR"),12,false,0,71,0,98,22,98,22 );

					m_pChildMenuButton[6] = new CButton( this );
					m_pChildMenuButton[6]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 6, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),40,false,0,176,0,64,22,64,22 );


					m_pChildMenuButton[8] = new CButton( this );
					m_pChildMenuButton[8]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 8, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),76,false,0,247,0,64,22,64,22 );

				}

				//KHY - 0220 -  푸치코미 옵션 삭제. - 전국가 적용.
				if(pclClient->IsCountrySwitch(Switch_Puchikomi)) 
				{
					m_pChildMenuButton[9] = new CButton( this );

					m_pChildMenuButton[9]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 9, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),48,false,0,263,0,64,22,64,22 );
				}


			}
			break;
		case CHILDMENU_ID_WEB:
			{
				CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 274, 26, false, false, false );
				// 버튼 세개 만들기
				m_pChildMenuButton[0] = new CButton( this ); // 아이템몰   크기 98
				m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,
					TEXT("NInterface/Button/InterfaceButton_98x20_sysmenu.SPR"),0,false,0,0,0,98,22,98,22 );

				//KHY - 0220 -  푸치코미 옵션 삭제.
				if(pclClient->IsCountrySwitch(Switch_Puchikomi)) 
				{
					m_pChildMenuButton[1] = new CButton( this ); // 푸치코미(꼼지)
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),44,false,0,105,0,64,22,64,22 );
				}

				m_pChildMenuButton[2] = new CButton( this ); // 군주 홈페이지
				m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,
					TEXT("NInterface/Button/InterfaceButton_98x20_sysmenu.SPR"),4,false,0,176,0,98,22,98,22 );


			}
			break;
		case CHILDMENU_ID_OPTION:
			{
				
				if ( pclClient->siServiceArea == ConstServiceArea_China )
				{
					CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 382, 26, false, false, false );
					// 버튼 만들기
					m_pChildMenuButton[0] = new CButton( this );
					m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,	//미니맵
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),52,false,0,0,0,64,22,64,22 );
					m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+W"));

					m_pChildMenuButton[1] = new CButton( this );
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,  //환경설정(게임옵션)
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),56,false,0,71,0,64,22,64,22 );
					m_pChildMenuButton[1]->SetUseToolTip(TEXT("Alt+O"));

					m_pChildMenuButton[2] = new CButton( this );

					m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,  // 스크린샷
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),60,false,0,142,0,64,22,64,22 );
					m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+P"));

					m_pChildMenuButton[3] = new CButton( this );
					m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,  //헬프
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),64,false,0,213,0,64,22,64,22 );
					m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+H"));

					m_pChildMenuButton[4] = new CButton( this );
					m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,  //게임종료
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),68,false,0,284,0,64,22,64,22 );
					m_pChildMenuButton[4]->SetUseToolTip(TEXT("ESC"));

				}			
				else if ( pclClient->siServiceArea == ConstServiceArea_Korea )
				{
 					CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 356, 26, false, false, false );
					// 버튼 만들기
					m_pChildMenuButton[0] = new CButton( this );
					m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,	//미니맵
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),36,false,0,0,0,64,22,64,22 );
					m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+W"));

 					m_pChildMenuButton[1] = new CButton( this );
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,  //환경설정(게임옵션)
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),40,false,0,71,0,64,22,64,22 );
					m_pChildMenuButton[1]->SetUseToolTip(TEXT("Alt+O"));

					m_pChildMenuButton[2] = new CButton( this );

					m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,  // 스크린샷
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),44,false,0,142,0,64,22,64,22 );
					m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+P"));

					m_pChildMenuButton[3] = new CButton( this );
					m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,  //헬프
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),48,false,0,213,0,64,22,64,22 );
					m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+H"));

					m_pChildMenuButton[4] = new CButton( this );
					m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,  //게임종료
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),52,false,0,284,0,64,22,64,22 );
					m_pChildMenuButton[4]->SetUseToolTip(TEXT("ESC"));
				}
				else
				{
					CDialogController::Create( NSYSCHILDMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogBar, NULL, NULL, 0, false, 0, 100 , 100, 382, 26, false, false, false );
					// 버튼 만들기
					m_pChildMenuButton[0] = new CButton( this );
					m_pChildMenuButton[0]->Create( NSYSCHILDMENU_DIALOG_BUTTON_0 , NULL,	//미니맵
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),52,false,0,0,0,64,22,64,22 );
					m_pChildMenuButton[0]->SetUseToolTip(TEXT("Alt+W"));

					m_pChildMenuButton[1] = new CButton( this );
					m_pChildMenuButton[1]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 1, NULL,  //환경설정(게임옵션)
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),56,false,0,71,0,64,22,64,22 );
					m_pChildMenuButton[1]->SetUseToolTip(TEXT("Alt+O"));

					m_pChildMenuButton[2] = new CButton( this );

					m_pChildMenuButton[2]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 2, NULL,  // 스크린샷
						TEXT("NInterface/Button/InterfaceButton_98x20_sysmenu.SPR"),8,false,0,142,0,98,22,98,22 );
					m_pChildMenuButton[2]->SetUseToolTip(TEXT("Alt+P"));

					m_pChildMenuButton[3] = new CButton( this );
					m_pChildMenuButton[3]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 3, NULL,  //헬프
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),64,false,0,247,0,64,22,64,22 );
					m_pChildMenuButton[3]->SetUseToolTip(TEXT("Alt+H"));

					m_pChildMenuButton[4] = new CButton( this );
					m_pChildMenuButton[4]->Create(NSYSCHILDMENU_DIALOG_BUTTON_0 + 4, NULL,  //게임종료
						TEXT("NInterface/Button/InterfaceButton_64x20_sysmenu.SPR"),68,false,0,318,0,64,22,64,22 );
					m_pChildMenuButton[4]->SetUseToolTip(TEXT("ESC"));
				}

			}
			break;

	}
	SetTopMostWindow( true );
}


void CNSysChildMenuDlg::ClearFocus()
{
	CDialogController::ClearFocus();
	DeleteDialog();

	return;
}

void CNSysChildMenuDlg::SetFocus( bool Focus )
{
	CDialogController::SetFocus( Focus );

	if ( ! Focus )
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CNSysChildMenuDlg::StaticCallBackDialogBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNSysChildMenuDlg *pThis = (CNSysChildMenuDlg *)pControllerMgr;
	pThis->CallBackDialogBar( nEvent, nControlID, pControl );
	return;
}


void CALLBACK CNSysChildMenuDlg::CallBackDialogBar( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = ( cltClient * ) pclClient;
	switch( nEvent )
	{
	case EVENT_BUTTON_CLICKED:
		{
			cltClient * pclclient = (cltClient* ) pclClient;
			if( pclclient->m_pDialog[NSYSMENUBAR_DLG] )
			{
				//((CNSysMenuBarDlg*)pclclient->m_pDialog[NSYSMENUBAR_DLG])->m_siNowOpendMenu = 0;
			}

			DeleteDialog();			
			switch( m_siSelectedChildMenuId) 
			{
			case CHILDMENU_ID_INFO: // 현재 선택된 시스템 메뉴
				{ 					
					switch ( nControlID ) // 몇번째 버튼이 선택되었는지
					{

					case NSYSCHILDMENU_DIALOG_BUTTON_0:  // 아이템
						{	
							if ( ((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ]) )
							{
								if ( ((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ])->IsShow() == false )
								{
									((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ])->Show();
								}
								else
								{
									((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ])->Hide();
								}
							}

						}
							break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+1: // 몬스터
						{
							pclclient->CreateInterface( NMONSTERINFO_DLG );
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+2: // 사냥터
						{					
							if ( ((CNHuntMapInfoDlg*)pclclient->m_pDialog[ NHUNTMAPINFO_DLG ])->IsShow() == false )
							{
								((CNHuntMapInfoDlg*)pclclient->m_pDialog[ NHUNTMAPINFO_DLG ])->Show();
							}
							else
							{
								((CNHuntMapInfoDlg*)pclclient->m_pDialog[ NHUNTMAPINFO_DLG ])->Hide();
							}
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+3: // 국가
						{
							pclclient->CreateInterface( NVILLAGEDETAILINFO_DLG );
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+4: // NPC
						{
							if ( ((CNResidentsInfoDlg *)pclclient->m_pDialog[ NRESIDENTSINFO_DLG ])->IsShow() == false )
							{
								((CNResidentsInfoDlg *)pclclient->m_pDialog[ NRESIDENTSINFO_DLG ])->Show();
							}
							else
							{
								((CNResidentsInfoDlg *)pclclient->m_pDialog[ NRESIDENTSINFO_DLG ])->Hide();
							}
						}
						break;
					}
				}
				break;

			case CHILDMENU_ID_COMMUNITY:
				{
					switch ( nControlID ) // 몇번째 버튼이 선택되었는지
					{
					
					case NSYSCHILDMENU_DIALOG_BUTTON_0:  // 
						{	
							if(pclclient->m_pDialog[ NNEWFRIEND_DLG ])
							{	
								((CNNewFriendDlg*)pclclient->m_pDialog[ NNEWFRIEND_DLG ])->Show();
							}
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+1: //     개인 상점
						{
							cltGameMsgRequest_PersonalShopData clinfo(pclClient->pclCM->CR[1]->GetCharUnique(),FALSE,FALSE );// 만드는게 아니니까 무조건 FASLE
							cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);		

							pclClient->pclCM->CR[1]->DoQuestCheckAndAction( QUEST_CLEAR_COND_CLICKPERSONALSHOP );
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+2: // 
						{
							// 스승이 필요한 레벨이면 후견인 창을 보여준다. 
							//KHY - 0610 - 스승제자 S 추가수정. 50레벨 이하, 초보자 리스트 삭제.
							if ( ConstServiceArea_Japan != pclClient->siServiceArea )	
							{
								if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < pclClient->siMinimumTeacherLevel)
								{
									pclclient->CreateInterface(NCHILD_DLG);
									//pclclient->CreateInterface(NLOWLEVEL_DLG);
								}
								else
								{	
									pclclient->CreateInterface(NFATHER_DLG);
									pclclient->CreateInterface(NLOWLEVEL_DLG);
								}
							}
							else
							{
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
					case NSYSCHILDMENU_DIALOG_BUTTON_0+3: //  장인정보
						{
							pclclient->CreateInterface( NMASTERINFO_DLG );
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+4: // 국가채팅
						{
							if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
							{
								( ( CNResidentsDlg * )pclclient->m_pDialog[ NRESIDENTS_DLG ] )->Show();
							}			
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+5: // 나라 입력
						{
							if ( ! pclclient->m_pDialog[ NUSERLISTBYCOUNTRY_DLG ] )
							{
								pclclient->CreateInterface( NUSERLISTBYCOUNTRY_DLG );
							}
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+6: // 나라 입력
						{
							if( pclclient->siServiceArea == ConstServiceArea_English )
							{
								// PCK : 글로벌의 경우 NHN회원과 NDI회원에 따라서 보여지는 링크페이지가 다르다. (09.03.30)			
								NTCHARString32	kID(pclclient->szID);

								if( kID.Find("@nh") >= 0 )		// NHN 회원이면
								{
									ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ijji.com/", NULL, SW_SHOWNORMAL );
								}
								else							// NDI 회원이면
								{
									ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ndoorsgames.com/center/default.asp", NULL, SW_SHOWNORMAL );
								}							
							}
							else if( pclclient->siServiceArea == ConstServiceArea_Japan )
							{
								ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.kunshu.jp", NULL, SW_SHOWNORMAL );				
							}
							else if( pclclient->siServiceArea == ConstServiceArea_China )
							{
								ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.hljz.net/Default_Z1.aspx", NULL, SW_SHOWNORMAL );				
							}
							//KHY -0610 - S 추가.
							else if( pclclient->siServiceArea == ConstServiceArea_Korea )
							{
								ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://goonzus.ndoors.com/", NULL, SW_SHOWNORMAL );
							}
							else if( pclclient->siServiceArea == ConstServiceArea_Taiwan )
							{
								//ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://global.goonzu.com", NULL, SW_SHOWNORMAL );								
							}
							else if( pclclient->siServiceArea == ConstServiceArea_USA )
							{
								ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ijji.com/", NULL, SW_SHOWNORMAL );								
							}
							else if( pclclient->siServiceArea == ConstServiceArea_NHNChina )
							{
								ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.kunshu.jp", NULL, SW_SHOWNORMAL );				
							}
							else if( pclclient->siServiceArea == ConstServiceArea_EUROPE )
							{
								ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.aeriagames.com", NULL, SW_SHOWNORMAL );								
							}

						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+7: // 제조의뢰
						{
							cltGameMsgRequest_PersonalShopData clinfo(pclClient->pclCM->CR[1]->GetCharUnique(),TRUE,FALSE);	// 메뉴에서 선택해서 기본설정은 무조건 FASLE 
							cltMsg clMsg(GAMEMSG_REQUEST_PERSONALSHOPDATA, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);							
						}
						break;
					
					case NSYSCHILDMENU_DIALOG_BUTTON_0+8: // 길드 정보
						{
							pclclient->CreateInterface(NGUILDMEMBER_DLG);
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+9: // 푸치코미
						{
							cltGameMsgRequest_MyPuchikomiURL clinfo(pclclient->szID,pclclient->szPassword);
							cltMsg clMsg(GAMEMSG_REQUEST_MYPUCHIKOMIURL, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
						}

					}

				}
				break;
			case CHILDMENU_ID_WEB:
				{
					switch ( nControlID ) // 몇번째 버튼이 선택되었는지
					{

					case NSYSCHILDMENU_DIALOG_BUTTON_0:  // 아이템몰
						{	
		
							//////////////////////////////////////////////////////////////////////////
							//////////////		GM만 볼수있다		//////////////////////////////
							//////////////////////////////////////////////////////////////////////////
							if(pclclient->pclCM->CR[1]->GetGameMasterLevel() != 101 )
								break;
							

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
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+1: // 푸치코미
						{

						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+2: // 군주 홈피
						{
							switch( pclclient->siServiceArea )
							{
							case ConstServiceArea_English:
								ShellExecute( NULL, NULL, TEXT("http://www.playluminary.com"), NULL, NULL, SW_SHOWNORMAL );
								break;
							case ConstServiceArea_Japan:
								ShellExecute( NULL, NULL, TEXT("http://kunshu.jp"), NULL, NULL, SW_SHOWNORMAL );
								break;
							case ConstServiceArea_China:
								ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://www.hljz.net/Default_Z1.aspx", NULL, SW_SHOWNORMAL );				
								break;
							case ConstServiceArea_NHNChina:
								ShellExecute( NULL, NULL, TEXT("http://www.playluminary.com"), NULL, NULL, SW_SHOWNORMAL );
								break;
							case ConstServiceArea_EUROPE:
								ShellExecute( NULL, NULL, TEXT("http://luminary.aeriagames.com"), NULL, NULL, SW_SHOWNORMAL );
								break;
							}
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+3: //
						{
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+4: //
						{

						}
						break;
					}
				}
				break;
			case CHILDMENU_ID_OPTION:
				{
					switch ( nControlID ) // 몇번째 버튼이 선택되었는지
					{

					case NSYSCHILDMENU_DIALOG_BUTTON_0:  // 미니맵
						{	
							

							if(pclclient->m_pDialog[ NMINIMAP_DLG])
							{
								if( pclclient->m_pDialog[NMINIMAP_DLG]->IsShow() == false )
										pclclient->m_pDialog[NMINIMAP_DLG]->Show(TRUE);

								((CNMiniMapDlg*)pclclient->m_pDialog[ NMINIMAP_DLG])->
								Minimi( !((CNMiniMapDlg*)pclclient->m_pDialog[ NMINIMAP_DLG])->GetMinimi()   );
							}
							else
								pclclient->CreateInterface( NMINIMAP_DLG );
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+1: // 옵션
						{
							if(pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )
							{
								g_SoundOnOffDlg.Hide();
								((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->Show();
							}
							else
							{
								if( pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )
									((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->Hide();
								g_SoundOnOffDlg.Show();
							}
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+2: // 캡쳐
						{
							pclclient->pclCapture->CaptureToJPG(); 

							TCHAR TempWorkDir[MAX_PATH];
							_tgetcwd( TempWorkDir, MAX_PATH );

							TCHAR buffer[256];
							TCHAR* pText = GetTxtFromMgr(960);


							StringCchPrintf(buffer, 256, pText, 
								TempWorkDir, pclclient->pclCapture->GetFileName());

							TCHAR* pTitle = GetTxtFromMgr(961);

							pclclient->pclMessage->SetMsg(buffer);
						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+3: // 도움말
						{
							pclclient->ShowHelpWindow(TEXT("NULL"));
							//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
							//{
							//	pclclient->CreateInterface( NHINT_DLG );
							//}
							//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("NULL"));

						}
						break;
					case NSYSCHILDMENU_DIALOG_BUTTON_0+4: // 종료
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

			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_MENU_CLICK")), 0, 0);
		}
		break;
	}
}

void CNSysChildMenuDlg::DeleteDialog()
{
	

	CDialogController::DeleteDialog();
}

void CNSysChildMenuDlg::Action()
{


	return;
}
