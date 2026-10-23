#include "SystemNPCInfoDlg.h"

CSystemNPCInfoDlg g_SystemNPCInfoDlg;

#include "Char/CharManager/CharManager.h"

#include "../../GlobalImgFile/GlobalImgFile.h"
#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
#include "../../Client/client.h"
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "../Common/NPCManager/NPC.h"

#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

extern cltCommonLogic* pclClient;

CSystemNPCInfoDlg::CSystemNPCInfoDlg()
{
	m_hDlg = NULL;
	m_siSelectedIndex = -1;
	m_hPortraitBitmap = NULL;
	
	m_hSearchDetailBitmap = NULL;
	m_hEditBrush		= NULL;

	Initialize();

	m_iSelectVillageInx = 0 ;

	m_hWarpBitmap = NULL ;
}

CSystemNPCInfoDlg::~CSystemNPCInfoDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CSystemNPCInfoDlg::Initialize()
{
	m_bComboBoxInit = false;
	return;
}

void CSystemNPCInfoDlg::Destroy()
{
	return;
}

void CSystemNPCInfoDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_SYSTEMNPCINFO), hWnd, StaticSystemNPCInfoDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CSystemNPCInfoDlg::StaticSystemNPCInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_SystemNPCInfoDlg.SystemNPCInfoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CSystemNPCInfoDlg::SystemNPCInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			// 지식검색버튼 
			m_hSearchDetailBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/SearchDetailDlg/search.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hSearchDetailBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_SEARCH_SYSTEMNPC, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hSearchDetailBitmap );

				HWND hwnd = GetDlgItem(hDlg, IDC_BUTTON_SEARCH_SYSTEMNPC);
				if(hwnd)	MoveWindow(hwnd, 280, 1, 23, 27, TRUE);
			}
			
			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);

			m_hWarpBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/HitButton/VillageWarp.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hWarpBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_REGEN_POS, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hWarpBitmap);
			}
			
		}
		return TRUE;		
	case WM_CTLCOLORSTATIC:
		{
			if( GetDlgItem( hDlg, IDC_EDIT_SYSTEMNPC_INFO ) == (HWND)lParam )
				//GetDlgItem( hDlg, IDC_EDIT_REGEN_POS ) == (HWND)lParam)	
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;

	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
			case IDC_BUTTON_REGEN_POS:
				{
					if ( pclClient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
					{
						char* pTitle = GetTxtFromMgr(1891);
						char* pText = GetTxtFromMgr(1892);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
						return FALSE;
					}
					// 영자나라에 있으면 불가능
					if ( pclClient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
						pclClient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
					{
						return FALSE ;
					}

					char buffer[256] = "" ;
					char caption[256] = "" ;

					char* pTitle = NULL ;
					char* pText = NULL ;

					if ( m_iSelectVillageInx > 0 && m_iSelectVillageInx <= CUR_VILLAGE_LIMIT )
					{
						if ( !pclclient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp &&
							 pclclient->pclCM->CR[1]->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false )
						{
							SI32 itemunique = 14005 ;  // 순간이동이용권
							if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
							{
								pTitle = GetTxtFromMgr(1891);
								pText = GetTxtFromMgr(2381);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
								break;
							}

							pTitle = GetTxtFromMgr(1893);
							pText = GetTxtFromMgr(2380);
							wsprintf(caption,pTitle,pclclient->pclVillageManager->GetName(m_iSelectVillageInx));
							wsprintf(buffer,pText);
						}
						else
						{
							pTitle = GetTxtFromMgr(3189);
							pText = GetTxtFromMgr(1893);
							wsprintf(caption,pTitle);
							wsprintf(buffer,pText,pclclient->pclVillageManager->GetName(m_iSelectVillageInx));
						}

						if (  MessageBox(NULL, buffer, caption, MB_ICONQUESTION | MB_YESNO) == IDYES )
						{
							//KHY - 0703 - 게임 머니로 귀환하기.클레스 수정.
							cltGameMsgRequest_HuntMapWarp pclInfo(m_iSelectVillageInx,0,false);
							cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (char*)&pclInfo );
							pclclient->SendMsgToServer((char*)&clMsg);
						}
					}
					else
					{
						pTitle = GetTxtFromMgr(1891);
						pText = GetTxtFromMgr(2382);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
				}
				break;
			case IDC_LIST_REGEN_POS:
				{
					switch(HIWORD(wParam))
					{
					case LBN_SELCHANGE:
						SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_REGEN_POS);
						if ( index < 0 ) break ;

						m_iSelectVillageInx = SendDlgItemMessage(hDlg, IDC_LIST_REGEN_POS, LB_GETITEMDATA, index, 0);
					}
					break;
				}
				break;
			case IDC_BUTTON_SEARCH_SYSTEMNPC:
				{
					if ( m_siSelectedIndex > -1 )
					{
						char strSystemNPCName[ 128 ] = "";
						SendMessage( GetDlgItem( hDlg, IDC_COMBO_SYSTEMNPC_SELECT ), CB_GETLBTEXT, m_siSelectedIndex, (LPARAM) strSystemNPCName );
						
						pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, strSystemNPCName);
					}
				}
				break;
			case IDC_COMBO_SYSTEMNPC_SELECT:
				{
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							m_siSelectedIndex = (SI16)SendMessage( GetDlgItem( hDlg, IDC_COMBO_SYSTEMNPC_SELECT ), CB_GETCURSEL, 0, 0 );

							if ( m_siSelectedIndex > -1 )
							{
								char strSystemNPCName[ 128 ] = "";
								SendMessage( GetDlgItem( hDlg, IDC_COMBO_SYSTEMNPC_SELECT ), CB_GETLBTEXT, m_siSelectedIndex, (LPARAM) strSystemNPCName );
								SI32 siSystemNPCKind = pclClient->pclKindInfoSet->FindKindFromName( strSystemNPCName );
																								
								if ( m_hPortraitBitmap )
								{
									DeleteObject( m_hPortraitBitmap );
									m_hPortraitBitmap = NULL;
								}

								char strFileName[ 128 ] = "";
								sprintf( strFileName, "Interface/Portrait/%s.bmp",	pclClient->pclKindInfoSet->pclKI[ siSystemNPCKind ]->szCode );
								m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

								if ( m_hPortraitBitmap != NULL )
								{
									SendDlgItemMessage( hDlg, IDC_STATIC_SYSTEMNPC_PORTRAIT, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
								}
								else
								{
									strcpy( strFileName, "Interface/Portrait/KIND_WAITINGMAN.BMP" );
									m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
									SendDlgItemMessage( hDlg, IDC_STATIC_SYSTEMNPC_PORTRAIT, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
								}								

								char szexplain[512];
								if(pclClient->pclQuestManager->GetQuestExplain(siSystemNPCKind, szexplain))
								{
									strcpy ( m_strBaseInfo, szexplain );
									SetWindowText ( GetDlgItem( hDlg, IDC_EDIT_SYSTEMNPC_INFO ), m_strBaseInfo );
								}
								else
								{
									SetWindowText ( GetDlgItem( hDlg, IDC_EDIT_SYSTEMNPC_INFO ), "" );
								}

								//SetWindowText ( GetDlgItem( m_hDlg, IDC_EDIT_REGEN_POS ), "" );

								if ( siSystemNPCKind == KIND_WAITINGMAN2 )
								{
									char* pText = GetTxtFromMgr(2383);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_RUSSIATRADER )
								{
									char* pText = GetTxtFromMgr(299);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_CHINATRADER )
								{
									char* pText = GetTxtFromMgr(300);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_SUMMONMERCHANT )
								{
									char* pText = GetTxtFromMgr(300);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_HAMEL )
								{
									char* pText = GetTxtFromMgr(2384);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_JANGGILSAN )
								{
									char* pText = GetTxtFromMgr(2385);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_BATTLEZONE )
								{
									char* pText = GetTxtFromMgr(5497);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_MERCHANTPOTTER )
								{
									char* pText = GetTxtFromMgr(518);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_JAPANTRADER )
								{
									char* pText = GetTxtFromMgr(518);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_PRISONER )
								{
									char* pText = GetTxtFromMgr(2386);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_MERCHANTHORSE )
								{
									char* pText = GetTxtFromMgr(299);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_BOBUSANG )
								{
									char* pText = GetTxtFromMgr(988);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_WANGCHO )
								{
									char* pText = GetTxtFromMgr(988);
									SetRegenPos(pText);
									break;
								}
								else if ( siSystemNPCKind == KIND_HONGGILDONG )
								{
									char* pText = GetTxtFromMgr(5235);
									SetRegenPos(pText);
									break;
								}

								//---------------------------
								// 서버로 NPC위치를 요청한다.
								//----------------------------
								cltGameMsgRequest_NPCPos clinfo(siSystemNPCKind);
								cltMsg clMsg(GAMEMSG_REQUEST_NPCPOS, sizeof(clinfo), (char*)&clinfo);
								cltClient* pclclient = (cltClient*)pclClient;

								pclclient->SendMsgToServer((char*)&clMsg);							

							}
						}
						break;
					}
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
			m_hDlg = NULL;

			if ( m_hWarpBitmap )
			{
				DeleteObject(m_hWarpBitmap);
				m_hWarpBitmap = NULL ;
			}

			if ( m_hPortraitBitmap )
			{
				DeleteObject( m_hPortraitBitmap );
				m_hPortraitBitmap = NULL;
			}

			if(m_hSearchDetailBitmap)
			{
				DeleteObject( m_hSearchDetailBitmap);
				m_hSearchDetailBitmap = NULL;
			}
			if ( m_hEditBrush )
			{
				DeleteObject( m_hEditBrush );
				m_hEditBrush= NULL;
			}

			m_iSelectVillageInx = 0 ;
		}
		break;
	}

	return FALSE;

}

bool CSystemNPCInfoDlg::IsShow()
{
	return m_bShow;
}

void CSystemNPCInfoDlg::Show()
{
	if ( m_bShow )
	{
		Hide();
		return;
	}

	if ( ! m_bComboBoxInit )
	{
		cltKindInfo* pclki = NULL;

		for ( SI32 i = 10; i < MAX_KIND_NUMBER; ++i )
		{
			pclki = pclClient->pclKindInfoSet->pclKI[ i ];
			//pclClient->pclKindInfoSet->pclKI[ siMonsterKind ]->siAtb == ATB_ATTACK

			if( pclki == NULL || pclki->IsAtb( ATB_SYSTEMNPC ) == false ) continue;
			if(pclki->IsAtb( ATB_NONPCLIST ) )continue;
			
			char* pname = (char*)pclki->GetName();
			SendDlgItemMessage( m_hDlg, IDC_COMBO_SYSTEMNPC_SELECT, CB_ADDSTRING, 0, (LPARAM)pname );
		}

		m_bComboBoxInit = true;
	}

	//SetWindowText ( GetDlgItem( m_hDlg, IDC_EDIT_REGEN_POS ), "" );
	ListDelete(m_hDlg, IDC_LIST_REGEN_POS);

	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOWNORMAL );
	SetForegroundWindow( m_hDlg );

	

	

	return;
}

void CSystemNPCInfoDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CSystemNPCInfoDlg::SetBaseInfo( char *text )
{
	SetWindowText ( GetDlgItem( m_hDlg, IDC_EDIT_SYSTEMNPC_INFO ), text );	
	return;
}

void CSystemNPCInfoDlg::SetRegenPos( char *text )
{
	char* pText = GetTxtFromMgr(1287);
	if ( !strcmp(text,"") ) strcpy(text, pText);
	//SetWindowText ( GetDlgItem( m_hDlg, IDC_EDIT_REGEN_POS ), text );
	ListDelete(m_hDlg, IDC_LIST_REGEN_POS);
	ListAddString(m_hDlg, IDC_LIST_REGEN_POS, (LPARAM)text);
	return;
}

void CSystemNPCInfoDlg::SetRegenPos( UI08 mapindex [] , SI16 maxmap )
{
	SI32 i;
	cltClient *pclclient = (cltClient*)pclClient;

	char buffer[256] = "";

	ListDelete(m_hDlg, IDC_LIST_REGEN_POS);

	for(i = 0;i < maxmap ;i++)
	{
		if ( mapindex[i] <= 0 ) continue ;

		if ( mapindex[i] > 0 && mapindex[i] <= CUR_VILLAGE_LIMIT )
			sprintf(buffer,"%s",pclclient->pclVillageManager->GetName(mapindex[i]));
		else
			sprintf(buffer,"%s",pclclient->pclMapManager->GetMapName(mapindex[i]));
		if ( !strcmp(buffer,"") ) continue;

		SI32 index = ListAddString(m_hDlg, IDC_LIST_REGEN_POS, (LPARAM)buffer);

		if ( mapindex[i] > 0 && mapindex[i] <= CUR_VILLAGE_LIMIT )
			ListSetItemData(m_hDlg, IDC_LIST_REGEN_POS, index, mapindex[i]);
	}
}

void CSystemNPCInfoDlg::AddRegenPos( char *text )
{
	if ( !strcmp(text,"") ) return ;
	ListAddString(m_hDlg, IDC_LIST_REGEN_POS, (LPARAM)text);
}
