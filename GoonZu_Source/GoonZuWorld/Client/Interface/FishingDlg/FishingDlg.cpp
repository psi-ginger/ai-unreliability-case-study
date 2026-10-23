/*
#include "FishingDlg.h"


#include "../../Client/client.h"

#include "../../Common/ShipListMgr/ShipListMgr.h"
#include "../../Common/ShipListMgr/ShipListObj.h"

#include "../../Common/Ship/Ship.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/Msg/MsgType-Village.h"
#include "../../CommonLogic/Msg/MsgType-Fishing.h"
#include "../../CommonLogic/Msg/MsgType-Agriculture.h"
#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"

#include "../../Music/Music.h"
#include "../SoundOnOffDlg/SoundOnOffDlg.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"


#include "../../../lib/WebHTML/WebHTML.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

#include <wininet.h>

extern cltCommonLogic* pclClient;


CFishingDlg::CFishingDlg()
{
	m_hDlg = NULL;
	
	m_hEditBrush = NULL;

	m_bAutoFishing = false;

	m_pToolInfo = new DBToolInfo[ MAX_FISHING_TOOL_NUMBER ];
	memset( m_pToolInfo, 0, sizeof( DBToolInfo ) * MAX_FISHING_TOOL_NUMBER );

	Init();	
}

CFishingDlg::~CFishingDlg()
{
	if ( m_pToolInfo )
	{
		delete [] m_pToolInfo;
		m_pToolInfo = NULL;
	}

	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CFishingDlg::Init()
{
	m_siFishingBaitItemUnique = 0;

	

}

void CFishingDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = CreateDialog( hInst, MAKEINTRESOURCE( IDD_DIALOG_FISHING ), hWnd,  StaticFishingtDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CFishingDlg::StaticFishingtDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = ( cltClient *)pclClient;
	return pclclient->m_pFishingDlg->FishingtDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CFishingDlg::FishingtDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch( iMsg )
	{
	case WM_INITDIALOG:
		{			
			m_hDlg = hDlg;
	
			cltClient *pclclient = (cltClient *)pclClient;
			pclclient->pclImageList->ConnectToListView( GetDlgItem( hDlg, IDC_LIST_FISHING_BAITITEM_IMAGE ), LVSIL_NORMAL );

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);

			SendMessage( GetDlgItem( hDlg, IDC_CHECK_AUTOFISHING ), BM_SETCHECK, BST_UNCHECKED, 0 );

			SetTimer( hDlg, 0, 5000, NULL );
		}
		return TRUE;
	case WM_TIMER:
		{			
			if ( pclClient->pclCM->CR[1]->GetCurrentOrder() == ORDER_FISH )
			{	
				SetFishingExplain();	
				if(!( g_SoundOnOffDlg.m_bFullScreenMode ))
				Show();
				pclClient->PushEffect( EFFECT_FISHING_NOW, 0, 0 );
			}
			else
			{
				Hide();
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_FISHING_EXPLAIN ) == (HWND)lParam ||
				GetDlgItem( hDlg, IDC_EDIT_FISHING_PLACE_INFO ) == (HWND)lParam ||
				GetDlgItem( hDlg, IDC_EDIT_FISHING_PLACE_INFO_FORSUMMON ) == (HWND)lParam
				)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;
	case WM_CLOSE:
		{
			Hide();
		}
		break;
	case WM_COMMAND:
		{
			switch( LOWORD( wParam ) )
			{
			case IDOK:
			case IDCANCEL:
				Hide();
				break;
			case IDC_CHECK_AUTOFISHING:
				{
					if ( SendMessage( GetDlgItem( hDlg, IDC_CHECK_AUTOFISHING ), BM_GETCHECK, 0, 0 ) == BST_UNCHECKED )
					{
						cltGameMsgRequest_AgricultureAutoGain clinfo( AGRICULTURE_TYPE_FISH, false );
						cltMsg clMsg(GAMEMSG_REQUEST_AGRICULTUREAUTOGAIN, sizeof(clinfo), (char*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((char*)&clMsg);
					}
					else
					{
						cltGameMsgRequest_AgricultureAutoGain  clinfo( AGRICULTURE_TYPE_FISH, true );
						cltMsg clMsg(GAMEMSG_REQUEST_AGRICULTUREAUTOGAIN, sizeof(clinfo), (char*)&clinfo);
						pclClient->pclCM->CR[1]->SendNetMsg((char*)&clMsg);
					}
				}
				break;
			}
		}
		break;
	case WM_DESTROY:
		{
			KillTimer( hDlg, 0 );

			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush = NULL;
			}

			m_hDlg = NULL;
		}
		break;
	}
	return FALSE;
}

bool CFishingDlg::IsShow()
{
	return m_bShow;
}

void CFishingDlg::Show()
{
	if ( m_bShow )
	{
		return;
	}

	char Buffer[256 ] = "";
	char* pText = GetTxtFromMgr(2300);

	sprintf( Buffer, pText, 
		pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FISH, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 ));
	SetWindowText( m_hDlg, Buffer );
	
	SetFishingExplain();
	SetFishingPlaceInfoExplain();
	
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOW );	

	return;
}

void CFishingDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );	

	return;
}

void CFishingDlg::Set( SI32 FishingBaitItemUnique, DBToolInfo *pclToolInfo )
{
	m_siFishingBaitItemUnique = FishingBaitItemUnique;

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( pclToolInfo[i].siPos > 0 )
		{
			m_pToolInfo[ i ].Set( &pclToolInfo[ i ] );
		}
	}
	
	cltItem clItem;
	clItem.siItemNum = 1;
	clItem.siUnique = FishingBaitItemUnique;
	
	SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_FISHING_BAITITEM_IMAGE, NULL, &clItem, 5, 5 );

	Show();

	return;
}

void CFishingDlg::SetAutoFishing( bool AutoFishing )
{
	m_bAutoFishing = AutoFishing;

	if ( AutoFishing )
	{
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_AUTOFISHING ), BM_SETCHECK, BST_CHECKED, 0 );
	}
	else
	{
		SendMessage( GetDlgItem( m_hDlg, IDC_CHECK_AUTOFISHING ), BM_SETCHECK, BST_UNCHECKED, 0 );
	}

	return;
}

void CFishingDlg::SetFishingExplain()
{
	if ( m_siFishingBaitItemUnique < 1 )
	{
		return;
	}

	char *FishingBaitName = (char*)pclClient->pclItemManager->GetName( m_siFishingBaitItemUnique );

	if ( FishingBaitName == NULL || strlen ( FishingBaitName ) < 1 )
	{
		return;
	}

	char Buffer[ 1024 ] = "";

	sprintf( Buffer, "%s\r\n", FishingBaitName );

	for ( SI16 i = 0; i < MAX_FISHING_TOOL_NUMBER; ++i )
	{
		if ( m_pToolInfo[ i ].clItem.siUnique > 0 )
		{
			char TempBuffer[256 ] = "";
			char *RodName = (char*)pclClient->pclItemManager->GetName( m_pToolInfo[ i ].clItem.siUnique );
			if ( RodName == NULL || strlen( RodName ) < 1)
			{
				continue;
			}

			//SI16 ItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( m_pToolInfo[ i ].clItem.siUnique );

			SI16 ItemPos = -1;
			if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ pclClient->pclCM->CR[1]->ActiveWeapon ].siUnique == m_pToolInfo[ i ].clItem.siUnique )
			{
				ItemPos = pclClient->pclCM->CR[1]->ActiveWeapon;
			}
			else
			{
				ItemPos = -1;
			}
			
			if ( ItemPos < 0 )
			{
				continue;
			}

			cltItem *pclItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ItemPos ];
			if ( pclItem->siUnique < 1 || pclItem->siItemNum < 1 )
			{
				continue;
			}
			
			SI32 Durability = pclItem->GetDurability(  );
			SI32 MaxDurability = pclItem->GetMaxDurability( pclClient->pclItemManager );

			sprintf( TempBuffer, "%s(%d/%d)\r\n", RodName, Durability, MaxDurability );

			strcat( Buffer, TempBuffer );
		}
	}

	SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FISHING_EXPLAIN ), Buffer );

	return;
}

void CFishingDlg::SetFishingPlaceInfoExplain()
{
	if ( m_siFishingBaitItemUnique < 1 )
	{
		return;
	}
	
	//--------------------------------------------------------------
	// 사람용
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.GetMapIndex() != 0 )
	{
		return;
	}

	SI32 XPos = pclClient->pclCM->CR[1]->pclCI->clBI.GetX();
	SI32 YPos = pclClient->pclCM->CR[1]->pclCI->clBI.GetY();

	char Buffer[ 1024 ] = "";
	
	SI16 *FishKindList = NULL;
	SI16 *FishCatchPercentList = NULL;

	SI16 FishSkillLevel = pclClient->pclCM->CR[1]->pclCI->clCharSkill.GetTotalSkillLevel( SKILL_FISH, 0, 0, &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[0], 1 );

	SI16 CatchFishKindCounter = pclClient->pclFishingMgr->GetFishingKindInPos( 
		FishSkillLevel,
		m_siFishingBaitItemUnique,
		XPos,
		YPos,
		&FishKindList,
		&FishCatchPercentList
		);

	if ( CatchFishKindCounter > 0 )
	{
		for( SI16 i = 0; i < CatchFishKindCounter; ++i )
		{
			if(  FishKindList[ i ] > 0 )
			{	
				char *FishName = (char *)pclClient->pclItemManager->GetName( FishKindList[ i ] );
				
				SI16 Ref = pclClient->pclFishingMgr->GetRefFromFishItemUnique( FishKindList[ i ] );
				if ( Ref >= 0 )
				{					
					char *FishingBaitItemName = (char *)pclClient->pclItemManager->GetName( pclClient->pclFishingMgr->m_pFishKindList[ Ref ]->m_stFishInfo.siBaitItemUniqueList[ 0 ] );
					char TempBuffer[256 ] = "";
					sprintf( TempBuffer, "%s - %d%%\r\n", FishName, FishCatchPercentList[ i ] );
					strcat( Buffer, TempBuffer );
				}
			}
		}

		if ( FishSkillLevel > 0 && pclClient->pclCM->CR[1]->clInnerStatus.siRideShipID > 0 )
		{
			CShipListObj *pShipListObj = pclClient->pclShipListMgr->GetShipListObj( pclClient->pclCM->CR[1]->clInnerStatus.siRideShipID );
			if ( pShipListObj )
			{
				cltShip *pShip = pShipListObj->GetShipDataPtr();
				if ( pShip )
				{
					char* pText = GetTxtFromMgr(3234);
					char TempBuffer[256 ] = "";
					sprintf( TempBuffer, pText, pclClient->pclShipManager->GetIncreaseFishingSkill( pShip->siKind ) );
					strcat( Buffer, TempBuffer );
				}
			}
		}

		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FISHING_PLACE_INFO ), Buffer );
	}
	else
	{
		char* pText = GetTxtFromMgr(2301);
		SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FISHING_PLACE_INFO ), pText );
	}

	if ( FishKindList != NULL )
	{
		delete [] FishKindList;
		FishKindList = NULL;
	}

	if ( FishCatchPercentList != NULL )
	{
		delete [] FishCatchPercentList;
		FishCatchPercentList = NULL;
	}
	//--------------------------------------------------------------



	//--------------------------------------------------------------
	// 소환수용
	SI32 SummonID = pclClient->pclCM->CR[1]->GetSummonID();
	
	// 소환수가 존재하는 경우.
	if( SummonID > 0 && pclClient->pclCM->IsValidID( SummonID ) )
	{
		// 낚시 가능 소환수인지 체크
		if ( pclClient->pclCM->CR[ SummonID ]->pclKI->IsAtb( ATB_FISH ) == false )
		{
			return;
		}

		if ( pclClient->pclCM->CR[ SummonID ]->pclCI->clBI.GetMapIndex() != 0 )
		{
			return;
		}

		char* pText = GetTxtFromMgr(2302);
		strcpy( Buffer, pText );

		XPos = pclClient->pclCM->CR[SummonID]->pclCI->clBI.GetX();
		YPos = pclClient->pclCM->CR[SummonID]->pclCI->clBI.GetY();
		
		FishKindList = NULL;
		FishCatchPercentList = NULL;

		SI16 CatchFishKindCounter = pclClient->pclFishingMgr->GetFishingKindInPos( 
			pclClient->pclCM->CR[SummonID]->pclCI->clIP.GetLevel()/5+pclClient->pclCM->CR[SummonID]->pclKI->siSubSkillLevel,
			m_siFishingBaitItemUnique,
			XPos,
			YPos,
			&FishKindList,
			&FishCatchPercentList
			);
		
		if ( CatchFishKindCounter > 0 )
		{
			for( SI16 i = 0; i < CatchFishKindCounter; ++i )
			{
				if(  FishKindList[ i ] > 0 )
				{	
					char *FishName = (char *)pclClient->pclItemManager->GetName( FishKindList[ i ] );
					
					char TempBuffer[256 ] = "";
					sprintf( TempBuffer, "%s - %d%%\r\n", FishName, FishCatchPercentList[ i ] );
					
					strcat( Buffer, TempBuffer );
				}
			}
			
			SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FISHING_PLACE_INFO_FORSUMMON ), Buffer );
		}
		else
		{
			pText = GetTxtFromMgr(2301);
			SetWindowText( GetDlgItem( m_hDlg, IDC_EDIT_FISHING_PLACE_INFO_FORSUMMON ), pText );
		}
		
		if ( FishKindList != NULL )
		{
			delete [] FishKindList;
			FishKindList = NULL;
		}
		
		if ( FishCatchPercentList != NULL )
		{
			delete [] FishCatchPercentList;
			FishCatchPercentList = NULL;
		}
	}

	return;
}
*/