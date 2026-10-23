#include "NMenuDlg.h"

#include "../../../Client/ItemInfo/ItemInfo.h"

#include "../../Inventory/ItemInventory.h"

#include "../../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../Interface/NInventory/NInventory.h"
#include "../../NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

#include "../../NInterface/NFriendDlg/NFriendDlg.h"
#include "../../NInterface/NFriendDlg/NNewFriendDlg.h"

#include "../../NInterface/NItemInfoDlg/ItemInfoDlg.h"
#include "../../NInterface/NHomePageDlg/HomePageDlg.h"
#include "../../NInterface/NHuntMapInfoDlg/HuntMapInfoDlg.h"
#include "../../NInterface/NResidentsInfoDlg/ResidentsInfoDlg.h"
#include "../../NInterface/NMagicInfoDlg/MagicInfoDlg.h"
#include "../../NInterface/NTeacherDlg/TeacherDlg.h"
#include "../../NInterface/NTeacherDlg/FatherDlg.h"
#include "../../NInterface/NTeacherDlg/ChildDlg.h"

#include "../../NInterface/NMiniMapDlg/NMiniMapDlg.h"
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
#include "NInterface/NHintDlg/NHintDlg.h"

#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/Static.h"

#include "Msg/MsgType-PersonalShop.h"
#include "Msg/MsgType-Quest.h"


#include "../../InterfaceMgr/Interface/List.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"


#include "../../Client.h"
extern cltCommonLogic* pclClient;

extern SI32		gsiResolution;

CNMenuDlg::CNMenuDlg() : CDialogController()
{
	m_pList = NULL;
}

CNMenuDlg::~CNMenuDlg()
{
	SAFE_DELETE( m_pList );
}

void CNMenuDlg::Create()
{
	if ( IsCreate() )
	{
		DeleteDialog();
	}
	else
	{
		// 1024 X 768
		if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
		{
			cltClient *pclclient = (cltClient *)pclClient;

			CDialogController::Create( NMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNMenu, NULL,  TEXT("NInterface/Common/Common_24X24_00_109.Spr"), 27, true, 9, 490, pclclient->GetScreenYSize() - 49 - 135 + 35 , 120, 110, false, false, false );

		}
		// 800 X 600
		else
		{

			CDialogController::Create( NMENU_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNMenu, NULL,  TEXT("NInterface/Common/Common_24X24_00_109.Spr"), 27, true, 9, 490, 498, 120, 110, false, false, false );
		}
		m_pList = new CList( this );

		m_pList->Create( NMENU_DIALOG_LIST, NULL, 0, false, 0, 0, 0, 120, 108, 120, 108, false, false, false, false, 20, false, 1 );

		m_pList->SetEachLineOffset( 2, 2 );

		m_pList->SetMouseOverImageFileName(  TEXT("NInterface/Image/Image_20X20_00_000.Spr") );
		m_pList->SetMouseOverImageFontIndex( 2 );
		m_pList->SetMouseOverImageTileNum( 1 );

		m_pList->SetColumn( 0, 118,  TEXT("Menu") );
		m_pList->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		CControllerMgr::SetFocus( m_pList );
	}
}

void CALLBACK CNMenuDlg::StaticCallBackDialogNMenu( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMenuDlg *pThis = (CNMenuDlg*)pControllerMgr;
	pThis->CallBackDialogNMenu( nEvent, nControlID, pControl );
	return;
}

void CALLBACK CNMenuDlg::CallBackDialogNMenu( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NMENU_DIALOG_LIST:
		{
			SI16 SelectedIndex = m_pList->GetSelectedIndex();
			if ( SelectedIndex > -1 )
			{
				stListBoxItemData *pItemData = m_pList->GetItemData( SelectedIndex );
				if ( pItemData )
				{
					Execute( pItemData->siParam[0] );
					DeleteDialog();
				}
			}
		}
		break;
	}

	return;
}

void CNMenuDlg::Set( SI16 Index )
{
	if ( Index < 0 )
	{
		return;
	}

	if ( m_siSelectedMenuIndex != Index )
	{
		m_pList->Clear();

		m_siSelectedMenuIndex = -1;
	}

	m_siSelectedMenuIndex = Index;

	stListBoxItemData ListBoxItemData;
	ListBoxItemData.siSmallIconFileIndex[0] = 3;

	if ( Index == 1 || Index == 5  )
	{
		return;
	}
	
	// 1024 X 768
	if(gsiResolution == 2 ||gsiResolution == 4) // 1024x768 || 1024x600
	{
		if( Index == 6)
			MovePos( 490 + 42 *5, GetY() + 5 );
		else
			MovePos( 490 + 42 *Index, GetY() + 5 );
	}
	// 800 X 600
	else
	{
		if( Index == 6)
			MovePos( 490 + 42 *5, GetY() );
		else
			MovePos( 490 + 42 *Index, GetY() + 5 );
	}

	/*	// 1024 X 768
	if ( gsiResolution == 2 )
	{
	MovePos( 562 + 32 *Index, GetY() );
	}
	// 800 X 600
	else
	{	
	MovePos( 447 + 32 *Index, GetY() );
	}
	*/
	switch ( Index )
	{
	case 0:
		{	

			TCHAR* pText = GetTxtFromMgr(3427);
			ListBoxItemData.siSmallIconFontIndex[0] = 132;
			ListBoxItemData.siParam[0] = 0;
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 144;
			ListBoxItemData.siParam[0] = 1;
			pText = GetTxtFromMgr(3429);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			/*cyj 퀴즈 삭제
			ListBoxItemData.siSmallIconFontIndex[0] = 216;
			ListBoxItemData.siParam[0] = 2;
			pText = GetTxtFromMgr(3431);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );
			*/

			ListBoxItemData.siSmallIconFontIndex[0] = 168;
			ListBoxItemData.siParam[0] = 3;
			pText = GetTxtFromMgr(3428);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );
		}
		break;

	case 2:
		{
			/*cyj 아이템정보 NPC위로 이동
			ListBoxItemData.siSmallIconFontIndex[0] = 180;
			ListBoxItemData.siParam[0] = 4;
			TCHAR *pText = GetTxtFromMgr(3421);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );
			*/

			ListBoxItemData.siSmallIconFontIndex[0] = 192;
			ListBoxItemData.siParam[0] = 5;
			TCHAR* pText = GetTxtFromMgr(3423);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 136;
			ListBoxItemData.siParam[0] = 6;
			pText = GetTxtFromMgr(3426);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			//미니맵
			ListBoxItemData.siSmallIconFontIndex[0] = 220;
			ListBoxItemData.siParam[0] = 7;
			pText = GetTxtFromMgr(3415);
			ListBoxItemData.Set( 0, pText);
			m_pList->AddItem( &ListBoxItemData );


		}
		break;
	case 3:
		{
			ListBoxItemData.siSmallIconFontIndex[0] = 180;
			ListBoxItemData.siParam[0] = 4;
			TCHAR *pText = GetTxtFromMgr(3421);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 148;
			ListBoxItemData.siParam[0] = 8;
			pText = GetTxtFromMgr(3425);
			ListBoxItemData.Set( 0, pText);
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 160;
			ListBoxItemData.siParam[0] = 9;
			pText = GetTxtFromMgr(3422);
			ListBoxItemData.Set( 0, pText);
			m_pList->AddItem( &ListBoxItemData );

			/*cyj 선박상세정보 삭제
			ListBoxItemData.siSmallIconFontIndex[0] = 172;
			ListBoxItemData.siParam[0] = 10;
			pText = GetTxtFromMgr(3424);
			ListBoxItemData.Set( 0, pText);
			m_pList->AddItem( &ListBoxItemData );
			*/

			ListBoxItemData.siSmallIconFontIndex[0] = 156;
			ListBoxItemData.siParam[0] = 11;
			pText = GetTxtFromMgr(3430);
			ListBoxItemData.Set( 0, pText);
			m_pList->AddItem( &ListBoxItemData );
		}
		break;

	case 4:
		{
			ListBoxItemData.siSmallIconFontIndex[0] = 164;
			ListBoxItemData.siParam[0] = 12;
			TCHAR *pText = GetTxtFromMgr(4314);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 176;
			ListBoxItemData.siParam[0] = 13;
			pText = GetTxtFromMgr(4428);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 188;
			ListBoxItemData.siParam[0] = 14;
			pText = GetTxtFromMgr(3436);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 200;
			ListBoxItemData.siParam[0] = 15;
			pText = GetTxtFromMgr(3433);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );
		}
		break;
	case 6:
		{
			ListBoxItemData.siSmallIconFontIndex[0] = 184;
			ListBoxItemData.siParam[0] = 16;
			TCHAR *pText = GetTxtFromMgr(3437);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 196;
			ListBoxItemData.siParam[0] = 17;
			pText = GetTxtFromMgr(3438);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 140;
			ListBoxItemData.siParam[0] = 18;
			pText = GetTxtFromMgr(3432);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );

			ListBoxItemData.siSmallIconFontIndex[0] = 152;
			ListBoxItemData.siParam[0] = 19;
			pText = GetTxtFromMgr(5616);
			ListBoxItemData.Set( 0, pText );
			m_pList->AddItem( &ListBoxItemData );
		}
		break;
	default:
		m_siSelectedMenuIndex = -1;
		return;
	}
	/*	switch ( Index )
	{
	case 0:
	{	
	TCHAR* pText = GetTxtFromMgr(3415);
	ListBoxItemData.siSmallIconFontIndex[0] = 0;
	ListBoxItemData.siParam[0] = 0;
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 4;
	ListBoxItemData.siParam[0] = 1;
	pText = GetTxtFromMgr(3416);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 12;
	ListBoxItemData.siParam[0] = 2;
	pText = GetTxtFromMgr(3417);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 16;
	ListBoxItemData.siParam[0] = 3;
	pText = GetTxtFromMgr(3418);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 44;
	ListBoxItemData.siParam[0] = 4;
	pText = GetTxtFromMgr(3419);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 8;
	ListBoxItemData.siParam[0] = 5;
	pText = GetTxtFromMgr(3420);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );
	}
	break;
	case 1:
	{	
	ListBoxItemData.siSmallIconFontIndex[0] = 20;
	ListBoxItemData.siParam[0] = 6;
	TCHAR* pText = GetTxtFromMgr(3421);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 24;
	ListBoxItemData.siParam[0] = 7;
	pText = GetTxtFromMgr(3422);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 28;
	ListBoxItemData.siParam[0] = 8;
	pText = GetTxtFromMgr(3423);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 36;
	ListBoxItemData.siParam[0] = 9;
	pText = GetTxtFromMgr(3424);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 32;
	ListBoxItemData.siParam[0] = 10;
	pText = GetTxtFromMgr(3425);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 36;
	ListBoxItemData.siParam[0] = 11;
	pText = GetTxtFromMgr(3426);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );
	}
	break;
	case 2:
	{
	ListBoxItemData.siSmallIconFontIndex[0] = 48;
	ListBoxItemData.siParam[0] = 12;
	TCHAR *pText = GetTxtFromMgr(3427);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 76;
	ListBoxItemData.siParam[0] = 13;
	pText = GetTxtFromMgr(3428);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 52;
	ListBoxItemData.siParam[0] = 14;
	pText = GetTxtFromMgr(3429);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 40;
	ListBoxItemData.siParam[0] = 15;
	pText = GetTxtFromMgr(3430);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 56;
	ListBoxItemData.siParam[0] = 16;
	pText = GetTxtFromMgr(3431);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 72;
	ListBoxItemData.siParam[0] = 17;
	pText = GetTxtFromMgr(3432);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );
	}
	break;
	case 3:
	{

	if(pclClient->siServiceArea == ConstServiceArea_Netmarble)
	{
	ListBoxItemData.siSmallIconFontIndex[0] = 80;
	ListBoxItemData.siParam[0] = 18;
	TCHAR* pText = GetTxtFromMgr(3433);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 68;
	ListBoxItemData.siParam[0] = 19;
	pText = GetTxtFromMgr(3434);
	//	pText=TEXT("현재 준비중입니다.");
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 60;
	ListBoxItemData.siParam[0] = 22;
	pText = GetTxtFromMgr(3437);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 64;
	ListBoxItemData.siParam[0] = 23;
	pText = GetTxtFromMgr(3438);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );
	}
	else
	{

	ListBoxItemData.siSmallIconFontIndex[0] = 80;
	ListBoxItemData.siParam[0] = 18;
	TCHAR* pText = GetTxtFromMgr(3433);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 68;
	ListBoxItemData.siParam[0] = 19;
	pText = GetTxtFromMgr(3434);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 100;
	ListBoxItemData.siParam[0] = 20;
	pText = GetTxtFromMgr(3435);
	ListBoxItemData.Set( 0, pText);
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 104;
	ListBoxItemData.siParam[0] = 21;
	pText = GetTxtFromMgr(3436);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 60;
	ListBoxItemData.siParam[0] = 22;
	pText = GetTxtFromMgr(3437);
	ListBoxItemData.Set( 0, pText );
	m_pList->AddItem( &ListBoxItemData );

	ListBoxItemData.siSmallIconFontIndex[0] = 64;
	ListBoxItemData.siParam[0] = 23;
	pText = GetTxtFromMgr(3438);
	ListBoxItemData.Set( 0, pText);			
	m_pList->AddItem( &ListBoxItemData );

	}

	}
	break;
	default:
	m_siSelectedMenuIndex = -1;
	return;
	}
	*/
	m_pList->Refresh();
}

void CNMenuDlg::Execute( SI16 Param )
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	switch ( Param )
	{
	case 0:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode && pclclient->m_pDialog[ NNEWFRIEND_DLG ])
			{	
				((CNNewFriendDlg*)pclclient->m_pDialog[ NNEWFRIEND_DLG ])->Show();

				SetPosion( NNEWFRIEND_DLG );
			}
		}
		break;
	case 1:
		{
			// 스승이 필요한 레벨이면 후견인 창을 보여준다. 
			//KHY - 0610 - 스승제자 S 추가수정.
			if ( ConstServiceArea_Japan != pclClient->siServiceArea )
			{
				if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < pclClient->siMinimumTeacherLevel)
				{
					pclclient->CreateInterface(NCHILD_DLG);

					SetPosion( NCHILD_DLG );
				}
				else
				{	
					pclclient->CreateInterface(NFATHER_DLG);

					SetPosion( NFATHER_DLG );
				}
			}
			else
			{
				if(pclClient->pclCM->CR[1]->pclCI->clIP.GetLevel() < FATHER_LEVEL)
				{
					pclclient->CreateInterface(NCHILD_DLG);

					SetPosion( NCHILD_DLG );
				}
				else
				{	
					pclclient->CreateInterface(NFATHER_DLG);

					SetPosion( NFATHER_DLG );
				}
			}
		}
		break;
	case 2:
		{
		}
		break;
	case 3:
		{
			if ( pclclient->m_pDialog[ NRESIDENTS_DLG ] )
			{
				( ( CNResidentsDlg * )pclclient->m_pDialog[ NRESIDENTS_DLG ] )->Show();

				SetPosion( NRESIDENTS_DLG );
			}
		}
		break;
	case 4:
		{
			if ( g_SoundOnOffDlg.m_bFullScreenMode && ((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ]) )
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
	case 5:
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
	case 6:
		{
			pclclient->CreateInterface( NVILLAGEDETAILINFO_DLG );

			SetPosion( NVILLAGEDETAILINFO_DLG );
		}
		break;
	case 7:
		{
			pclclient->bSmallMapSwitch	= (pclclient->bSmallMapSwitch == true)? false: true;
			
			if ( pclclient->m_pDialog[ NMINIMAP_DLG ] )
			{
				pclclient->m_pDialog[ NMINIMAP_DLG ]->DeleteDialog();
			}
			else
			{
				pclclient->CreateInterface( NMINIMAP_DLG );

				TCHAR buffer[FILE_NAME_SIZE];

				TCHAR *pmaptype = NULL;

				SI32 MapUnique = pclclient->pclMapManager->GetMapUnique( pclclient->pclCM->CR[1]->GetMapIndex() );

				if ( MapUnique > 0 )
				{
					pmaptype = pclclient->pclMapManager->pclMapTypeManager->GetMapTypeNameFromUnique(MapUnique);
					if(pmaptype)
					{
						StringCchPrintf(buffer, FILE_NAME_SIZE,  TEXT("map\\%s\\%s.Spr"), pmaptype, pmaptype);

						((CNMiniMapDlg *)pclclient->m_pDialog[ NMINIMAP_DLG ])->Set( buffer, (cltMapClient *)(pclclient->pclMapManager->pclCurrentMap), pclClient->pclCM->CR[1]->GetMapIndex() );
					}
				}
			}
		}
		break;
	case 8:
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
	case 9:
		{
			pclclient->CreateInterface( NMONSTERINFO_DLG );
			/*
			if( pclclient->m_pDialog[ NMONSTERINFO_DLG ] )
			{
			pclclient->CreateInterface( NMONSTERINFO_DLG );
			}
			*/
		}
		break;
	case 10:
		{	
		}
		break;
	case 11:
		{
			pclclient->CreateInterface( NMASTERINFO_DLG );

			SetPosion( NMASTERINFO_DLG );
		}
		break;
	case 12:
		{
			/*
			if( pclClient->bItemMallUseSwitch == FALSE)
			{	
				TCHAR* pText = GetTxtFromMgr(962);

				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"), pText, pclclient->clSelectedWorldInfo.baseinfo.worldname );
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
	case 13:
		{
			/*
			SI32 siCharUnique = pclClient->pclCM->CR[1]->GetCharUnique();
			cltGameMsgRequest_Userid clUserid( siCharUnique );
			cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
			pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			*/
		}
		break;
	case 14:
		{
			/*
			TCHAR url[ MAX_PATH ];
			pclclient->GetSubSiteUrl( url, TEXT("center/board/media_center.asp?menu=new"), pclclient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );

			ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );

			//-----------------------------------------------------------
			// 게임 홈페이지로 가는 퀘스트가 진행중이면, 
			// 게임 서버로 퀘스트가 완료되었음을 알려준다.
			//-----------------------------------------------------------
			SI16 questtype, questunique, para1, para2;
			SI16 questclearcond =  QUEST_CLEAR_COND_NEWS;
			if( pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			*/
		}
		break;
	case 15:
		{
			/*
			TCHAR url[ MAX_PATH ];

			if( pclClient->siServiceArea == ConstServiceArea_Korea )
				pclClient->GetGoonZuUrl( url, pclClient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );

			ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );
			*/
		}
		break;
	case 16:
		{
			if(g_SoundOnOffDlg.m_bFullScreenMode && pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )
			{
				g_SoundOnOffDlg.Hide();
				((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->Show();

				SetPosion( NSOUNDONOFF_DLG );
			}
			else
			{
				if( pclclient->m_pDialog[ NSOUNDONOFF_DLG ] )
					((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->Hide();
				g_SoundOnOffDlg.Show();

				SetPosion( NSOUNDONOFF_DLG );
			}
		}
		break;
	case 17:
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
			//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, buffer);
		}
		break;
	case 18:
		{
			pclclient->ShowHelpWindow(TEXT("NULL"));
			//if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
			//{
			//	pclclient->CreateInterface( NHINT_DLG );
			//}
			//((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("NULL"));

			//SetPosion( NHINT_DLG );
		}
		break;
	case 19:
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

	return;

}

void CNMenuDlg::ClearFocus()
{
	CDialogController::ClearFocus();
	DeleteDialog();

	return;
}

void CNMenuDlg::SetFocus( bool Focus )
{
	CDialogController::SetFocus( Focus );

	if ( ! Focus )
	{
		DeleteDialog();
	}

	return;
}

void CNMenuDlg::SetPosion(SI32 id, SI32 mode)	// id는 다이얼로그 id , mode는 0이면 800 1이면 1024
{
	/*
	cltClient *pclclient = ( cltClient * ) pclClient;

	if( pclclient->m_pDialog[ id ] )
	{
		if(gsiResolution == 1)		// 800 X 600
			pclclient->m_pDialog[ id ]->MovePos(DLG_POSITION_X800, DLG_POSITION_Y800);
		else if(gsiResolution == 2) // 1024 X 768
		{
			if( mode == 0)
				pclclient->m_pDialog[ id ]->MovePos(DLG_POSITION_X1024, DLG_POSITION_Y1024);
			else if( mode == 1 )
				pclclient->m_pDialog[ id ]->MovePos(DLG_POSITION_X1024, DLG_POSITION_Y1024);
		}
	}
	*/
}
