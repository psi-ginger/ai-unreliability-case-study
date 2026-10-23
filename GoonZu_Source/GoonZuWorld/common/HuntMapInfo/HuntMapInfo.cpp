// HuntMapInfo.cpp: implementation of the CHuntMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "CommonLogic.h"
#include "../../Client/Client.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../common/NPCManager/NPC.h"
#include "Char\CharManager\CharManager.h"


#include "HuntMapInfo.h"
#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
#include "../../Client/Interface/MonsterInfoDlg//MonsterInfoDlg.h"

#include "../../Client/Client.h"
#include "../../Client/NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"

#include "JwLib/IsCorrectID/IsCorrectID.h"

#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern cltCommonLogic* pclClient;
extern CSoundOnOffDlg g_SoundOnOffDlg;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHuntMapInfo::CHuntMapInfo()
{
	m_hDlg = NULL ;
	m_hMonsterInfo = NULL ;

	m_hStaticBrush = NULL ;

	SetRect(&m_rtDlg,0,0,0,0);
	m_bRect = FALSE ;

	m_siCombo = 0 ;
//	m_siComboLevel = 0 ;
	m_siComboParentMap = 0 ;

	for ( int i = 0 ; i < MAX_MAP_NUMBER ; i ++ )
	{
//		m_siMapLevel[i] = 0 ;
		m_siParentMap[i] = 0 ;
		strcpy( m_szMapTypeName[i],"");
	}

	m_hMapBitmap = NULL ;
	m_hSearchDetailBitmap = NULL ;
	m_hWarpBitmap = NULL ;

	m_siSelectIndex = -1 ;
}

CHuntMapInfo::~CHuntMapInfo()
{
}

BOOL CALLBACK CHuntMapInfo::StatiCHuntMapInfoProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient *pclclient = (cltClient*)pclClient;
	return pclclient->pclHuntMapInfoManager->HuntMapInfoProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CHuntMapInfo::HuntMapInfoProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( iMsg )
	{
	case WM_INITDIALOG:
		{
			m_hStaticBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE );

			char* pText = GetTxtFromMgr(988);
			AddCombo(hDlg,pText,"gunjungpalace",MAPINDEX_PALACE);
			pText = GetTxtFromMgr(5570);
			AddCombo(hDlg,pText,"ChinaBoundary",MAPINDEX_CHINABORDER);
			pText = GetTxtFromMgr(969);
			AddCombo(hDlg,pText,"Beginner",MAPINDEX_BEGINNER1);
			pText = GetTxtFromMgr(969);
			AddCombo(hDlg,pText,"Beginner",MAPINDEX_BEGINNER2);
			pText = GetTxtFromMgr(970);
			AddCombo(hDlg,pText,"Castle",112);
			pText = GetTxtFromMgr(971);
			AddCombo(hDlg,pText,"bear",90);
			pText = GetTxtFromMgr(980);
			AddCombo(hDlg,pText,"SantaOffice",102);
			pText = GetTxtFromMgr(973);
			AddCombo(hDlg,pText,"SmallForest",99);
			pText = GetTxtFromMgr(981);
			AddCombo(hDlg,pText,"koreansweetsvillage",153);
			pText = GetTxtFromMgr(972);
			AddCombo(hDlg,pText,"wood",111);
			pText = GetTxtFromMgr(976);
			AddCombo(hDlg,pText,"porthole",114);
			pText = GetTxtFromMgr(979);
			AddCombo(hDlg,pText,"seapalace",109);
			pText = GetTxtFromMgr(3314);
			AddCombo(hDlg,pText,"hahoivillage",152);
			pText = GetTxtFromMgr(968);
			AddCombo(hDlg,pText,"GoblinHouse",98);
			pText = GetTxtFromMgr(301);
			AddCombo(hDlg,pText,"BattleSea",MAPINDEX_BATTLESEA);
			pText = GetTxtFromMgr(975);
			AddCombo(hDlg,pText,"MiHwangSa",104);
			pText = GetTxtFromMgr(3121);
			AddCombo(hDlg,pText,"MTHanra",115);
			pText = GetTxtFromMgr(3319);
			AddCombo(hDlg,pText,"aozi",157);
			pText = GetTxtFromMgr(978);
			AddCombo(hDlg,pText,"gumhatunnel",103);
			pText = GetTxtFromMgr(984);
			AddCombo(hDlg,pText,"MTBaeckDoo",170);
			pText = GetTxtFromMgr(977);
			AddCombo(hDlg,pText,"ghostvilla",113);
			pText = GetTxtFromMgr(983);
			AddCombo(hDlg,pText,"icecastle",154);
			pText = GetTxtFromMgr(985);
			AddCombo(hDlg,pText,"heaven",155);
			pText = GetTxtFromMgr(5594);
			AddCombo(hDlg,pText,"dosunpalace",178);

			m_hMonsterInfo = GetDlgItem( hDlg, IDC_LIST_MAPINFO_MONSTER );

			if(m_hMonsterInfo)
			{
				ListView_SetExtendedListViewStyle( m_hMonsterInfo,LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
				ListView_SetHoverTime( m_hMonsterInfo, 10 );
                pText = GetTxtFromMgr(3299);
				ListViewInsertColumn( m_hMonsterInfo, 120, pText, 0, 1);
				pText = GetTxtFromMgr(3300);
				ListViewInsertColumn( m_hMonsterInfo, 120, pText, 1, 1);
				ListView_DeleteAllItems(m_hMonsterInfo);
			}

			if ( m_hMapBitmap )
			{
				DeleteObject(m_hMapBitmap);
				m_hMapBitmap = NULL ;
			}

			m_hMapBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "HuntMapInfo/less.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hMapBitmap )
				SendDlgItemMessage( hDlg, IDC_STATIC_HUNTMAPINFO_IMG, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hMapBitmap);

			// 지식검색버튼 
			m_hSearchDetailBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/SearchDetailDlg/search.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hSearchDetailBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_SEARCH_MAPINFO, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hSearchDetailBitmap );

				HWND hwnd = GetDlgItem(hDlg, IDC_SEARCH_MAPINFO);
				if(hwnd)	MoveWindow(hwnd, 330, 1, 23, 27, TRUE);
			}

			m_hWarpBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "interface/HitButton/HuntMapWarp.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hWarpBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_HUNTMAP_WARP, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hWarpBitmap);
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_HUNTMAPINFO ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE );
								
				return (BOOL)m_hStaticBrush;
			}
			/*else
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_DLG_BLUE );

				return (BOOL)m_hStaticBrush;
			}*/
		}
		break;
	case WM_TIMER:
		{
		}
		break;
	case WM_NOTIFY:
		{
			//m_hItemListCtrl
			LPNMHDR						hdr;
			LPNMITEMACTIVATE			nia;
			LPNMLVGETINFOTIP			ngt;
			LPNMLISTVIEW				nlv;
			hdr = ( LPNMHDR )			lParam;
			ngt = ( LPNMLVGETINFOTIP )	lParam;
			nlv = ( LPNMLISTVIEW )		lParam;
			nia = (LPNMITEMACTIVATE)	lParam;

			if ( hdr->hwndFrom == m_hMonsterInfo )
			{
				switch( hdr->code )
				{
				case LVN_ITEMACTIVATE:
					{
						char buffer[256]; 
						ListView_GetItemText(m_hMonsterInfo, nia->iItem, 0, buffer, 64 );

						if ( g_SoundOnOffDlg.m_bFullScreenMode )
						{
							if ( pclclient->m_pDialog[ NMONSTERINFO_DLG ] == NULL )
							{
								pclclient->CreateInterface( NMONSTERINFO_DLG );
							}

							stNMonsterInfoDlgData NMonsterInfoDlgData;
							NMonsterInfoDlgData.pData = buffer;
							NMonsterInfoDlgData.siParam = 1;
							pclclient->m_pDialog[ NMONSTERINFO_DLG ]->SetData( &NMonsterInfoDlgData );
							//((CNMonsterInfoDlg*)pclclient->m_pDialog[ NMONSTERINFO_DLG ])->ShowMonsterInfo(buffer);
						}
						else
							g_MonsterInfoDlg.ShowMonsterInfo(buffer);
					}
					break;
				}
			}
		}
		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDC_BUTTON_HUNTMAP_WARP:
			{
				if( pclclient->pclCM->CR[1]->clInnerStatus.IsStatus(INNERSTATUS_PRISON))
					return TRUE ;

				// 영자나라에 있으면 불가능
				if ( pclclient->pclCM->CR[1]->GetMapIndex() == MAPINDEX_MASTERWORLD &&
					pclclient->pclCM->CR[1]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
				{
					return TRUE ;
				}

				char buffer[256] = "" ;

				if ( m_siSelectIndex < 0 ) return TRUE ;
				SI32 mapindex = SendDlgItemMessage(hDlg, IDC_LIST_MAPINFO, LB_GETITEMDATA,	m_siSelectIndex, 0);
				if ( mapindex <= 0 || mapindex > MAX_MAP_NUMBER ) return TRUE ;
				
				if ( mapindex == MAPINDEX_BATTLESEA ) // 문무대왕릉
				{
					char* pText = GetTxtFromMgr(3755);
					sprintf(buffer,pText);
					pText = GetTxtFromMgr(3326);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer);
					return TRUE;
				}

				if ( !pclclient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp &&
					 pclclient->pclCM->CR[1]->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false)
				{
					SI32 itemunique = 14005 ;  // 순간이동이용권
					if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
					{
						char* pText = GetTxtFromMgr(3756);
						sprintf(buffer,pText);
						pText = GetTxtFromMgr(3326);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pText, buffer);
						return TRUE;
					}
				}

				if ( pclclient->pclMapManager->GetLevelLimit(mapindex) >
					 pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() )
				{
					char* pText = GetTxtFromMgr(3757);
					sprintf(buffer, pText,
						pclclient->pclMapManager->GetLevelLimit(mapindex));
					pText = GetTxtFromMgr(249);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer);
					return TRUE;
				}

				const char * szMapName = pclclient->pclMapManager->GetMapName(mapindex);

				char *pText = GetTxtFromMgr(969);
				if ( !strcmp(szMapName,pText) && pclclient->pclCM->CR[1]->pclCI->clIP.GetLevel() > 10 )
				{
					pText = GetTxtFromMgr(3758);
					sprintf(buffer,pText);
					pText = GetTxtFromMgr(3331);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer);
					return TRUE;
				}

				if ( !pclclient->pclCM->CR[1]->pclCI->clBI.bUseNoLimitTicketWarp &&
					 pclclient->pclCM->CR[1]->pclCI->clBI.IsPCRoom() == false && pclclient->pclCM->CR[1]->pclCI->clBI.uiPoliceSwitch == false )
				{
					pText = GetTxtFromMgr(3759);
					sprintf(buffer,pText,szMapName);
				}
				else
				{
					pText = GetTxtFromMgr(3760);
					sprintf(buffer,pText,szMapName);
				}
				pText = GetTxtFromMgr(3326);
				if (  MessageBox(NULL, buffer,pText,
					MB_ICONQUESTION | MB_YESNO) == IDNO )
				{
					return TRUE;
				}

				//KHY - 0703 - 게임 머니로 귀환하기.클레스 수정.
				cltGameMsgRequest_HuntMapWarp pclInfo(mapindex,0,false);
				cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (char*)&pclInfo );
				pclclient->SendMsgToServer((char*)&clMsg);
			}
			break;
		case IDC_SEARCH_MAPINFO:
			{
				SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_MAPINFO);
				char szSelText[256] = "" ;

				if ( index < 0 || !ListGetString(hDlg,IDC_LIST_MAPINFO,(LPARAM)szSelText) || !strcmp(szSelText,"") )
				{
					ComboGetString(hDlg, IDC_COMBO_MAPINFO, (LPARAM)szSelText);

					if(strcmp(szSelText, "") != 0)
					{
						pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, szSelText);
					}
				}
				else
				{
					pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, szSelText);
				}
			}
			break;
		case IDC_COMBO_MAPINFO:
			{
				switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					{
						char szMapName[MAX_MAP_NAME_SIZE] = "" ;
						SI32 index = SendDlgItemMessage(hDlg, IDC_COMBO_MAPINFO, CB_GETCURSEL, 0, 0);
						ComboGetString(hDlg, IDC_COMBO_MAPINFO, (LPARAM)szMapName);

						ShowListLow(hDlg,index,szMapName);
						m_siSelectIndex = -1 ;
					}
					break;
				}
			}
			break;
		case IDC_LIST_MAPINFO:
			{
				switch(HIWORD(wParam))
				{
				case LBN_SELCHANGE:
					SI32 index = ListGetCurrentSel(hDlg, IDC_LIST_MAPINFO);
					char szMapName[MAX_MAP_NAME_SIZE] = "" ;

					if ( index < 0 ) break ;
					m_siSelectIndex = index ;

					if ( !ListGetString(hDlg,IDC_LIST_MAPINFO,(LPARAM)szMapName) ) break ;
					if ( !strcmp(szMapName,"") ) break ;

					ShowMapInfo(hDlg,szMapName);
				}
				break;
			}
			break;
		case IDOK:
			{
				DestroyWindow(m_hDlg);
			}
			break;
		}
		break;
	case WM_CLOSE:
		{
			DestroyWindow(m_hDlg);
		}
		break;
	case WM_DESTROY:
		{
			if ( m_hWarpBitmap )
			{
				DeleteObject(m_hWarpBitmap);
				m_hWarpBitmap = NULL ;
			}
			if ( m_hSearchDetailBitmap )
			{
				DeleteObject(m_hSearchDetailBitmap);
				m_hSearchDetailBitmap = NULL ;
			}
			if ( m_hStaticBrush )
			{
				DeleteObject( m_hStaticBrush );
				m_hStaticBrush = NULL;
			}
			if ( m_hMapBitmap )
			{
				DeleteObject(m_hMapBitmap);
				m_hMapBitmap = NULL ;
			}

			m_siCombo = 0 ;
//			m_siComboLevel = 0 ;
			m_siComboParentMap = 0 ;

			m_siSelectIndex = -1 ;

			GetWindowRect(hDlg, &m_rtDlg);
			m_bRect = TRUE;

			m_hMonsterInfo = NULL ;
			m_hDlg = NULL;
		}
		break;
	}

	return FALSE;
}

void CHuntMapInfo::Set( HINSTANCE hInst, HWND hParent )
{
	if ( !IsWindow(m_hDlg) )
	{
		m_hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_HUNTMAPINFO), hParent, StatiCHuntMapInfoProc);

		if(m_bRect)
		{
			MoveWindow(m_hDlg, 
				m_rtDlg.left, m_rtDlg.top, 
				m_rtDlg.right - m_rtDlg.left, 
				m_rtDlg.bottom - m_rtDlg.top, FALSE);
		}

		ShowWindow(m_hDlg, SW_SHOW);
	}
	else
	{
		DestroyWindow(m_hDlg);
	}
}

/*void CHuntMapInfo::AddCombo(HWND hDlg,char * szMapType,SI32 level,char * maptypename)
{
	ComboAddString(hDlg,IDC_COMBO_MAPINFO,(LPARAM)szMapType);
	m_siMapLevel[m_siCombo] = level ;
	strcpy( m_szMapTypeName[m_siCombo] , maptypename ) ;
	m_siCombo ++ ;
}*/

void CHuntMapInfo::AddCombo(HWND hDlg,char * szMapType,char * maptypename,SI32 parentmap)
{
	ComboAddString(hDlg,IDC_COMBO_MAPINFO,(LPARAM)szMapType);
	m_siParentMap[m_siCombo] = parentmap ;
	strcpy( m_szMapTypeName[m_siCombo] , maptypename ) ;
	m_siCombo ++ ;
}

void CHuntMapInfo::ShowListLow(HWND hDlg,SI32 index,char * szMapName)
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( index < 0 ) return ;
	if ( szMapName == NULL || !strcmp(szMapName,"")) return ;

	char bmppath[128] = "" ;

	if ( m_hMapBitmap )
	{
		DeleteObject(m_hMapBitmap);
		m_hMapBitmap = NULL ;
	}

	sprintf(bmppath,"HuntMapInfo/Gate_%s.bmp",m_szMapTypeName[index]);

	m_hMapBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), bmppath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

	if ( m_hMapBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_HUNTMAPINFO_IMG, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hMapBitmap);
	}
	else
	{
		m_hMapBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "HuntMapInfo/less.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
		SendDlgItemMessage( hDlg, IDC_STATIC_HUNTMAPINFO_IMG, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hMapBitmap);
	}

	char buffer[256] = "";

	ListDelete(m_hDlg, IDC_LIST_MAPINFO );

//	m_siComboLevel = m_siMapLevel[index] ;
	m_siComboParentMap = m_siParentMap[index] ;

	SI32 listindex = 0 ;

/*	if ( !strcmp(szMapName,"초보자사냥터") )
	{
		listindex = ListAddString(m_hDlg, IDC_LIST_MAPINFO, (LPARAM)"초보자사냥터");
		ListSetItemData(m_hDlg, IDC_LIST_MAPINFO, listindex, 0);
		ListSetItemData(m_hDlg, IDC_LIST_MAPINFO, listindex, 108); // 예외처리
		return ;
	}*/

	for(SI32 i = 0;i < MAX_MAP_NUMBER ; i++)
	{
		//if ( pclclient->pclMapManager->GetMapLevelLimit(i) == m_siComboLevel )
		if ( pclclient->pclMapManager->GetParentMap(i) == m_siComboParentMap )
		{
			//if ( strstr(szMapName,"깨비") && strstr(pclclient->pclMapManager->GetMapName(i),"하회") ) continue ;
			//if ( strstr(szMapName,"하회") && strstr(pclclient->pclMapManager->GetMapName(i),"깨비") ) continue ;
			strcpy(buffer,pclclient->pclMapManager->GetMapName(i));
			if ( !strcmp(buffer,"NoName") ) continue ;
			if ( !strcmp(buffer,"")) continue;
			listindex = ListAddString(m_hDlg, IDC_LIST_MAPINFO, (LPARAM)buffer);
			ListSetItemData(m_hDlg, IDC_LIST_MAPINFO, listindex, i);
		}
	}
}

void CHuntMapInfo::ShowMapInfo(HWND hDlg,char * szMapName)
{
	cltClient *pclclient = (cltClient*)pclClient;
	cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;

	SI32 siKind[MAX_NPC_KIND_PER_GROUP] ;
	SI32 siKindLevel[MAX_NPC_KIND_PER_GROUP] ;

	char strdir[128] = "" ;
	char buffer[1024] = "" ;

	SI32 mapindex = 0 ;

	for ( SI32 i = 0 ; i < MAX_MAP_NUMBER ; i ++ )
	{
		if ( !strcmp(pclclient->pclMapManager->GetMapName(i),szMapName))
		{
			mapindex = i ;
			break ;
		}
	}

	for ( i = 0 ; i < MAX_NPC_KIND_PER_GROUP ; i ++ )
	{
		siKind[i] = 0 ;
		siKindLevel[i] = 0 ;
	}
	pclnpcmanager->GetKindMapInx(mapindex,siKind,siKindLevel);
	ShowListMonster(hDlg,siKind,siKindLevel);

	pclclient->GetNearMapInfo(mapindex,0,0,strdir);

	SI32 mapunique = pclclient->pclMapManager->GetMapUnique(mapindex);

	if ( mapunique <= 0 ) return ;

	char * szMapTypeName = pclclient->pclMapManager->pclMapTypeManager->FindMapTypeNameFromUnique(mapunique);

	if ( szMapTypeName != NULL && strcmp(szMapTypeName,"") )
	{
		char bmppath[128] = "" ;

		if ( m_hMapBitmap )
		{
			DeleteObject(m_hMapBitmap);
			m_hMapBitmap = NULL ;
		}

		sprintf(bmppath,"HuntMapInfo/%s.bmp",szMapTypeName);

		m_hMapBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), bmppath, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

		if ( m_hMapBitmap != NULL )
		{
			SendDlgItemMessage( hDlg, IDC_STATIC_HUNTMAPINFO_IMG, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hMapBitmap);
		}
		else
		{
			m_hMapBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "HuntMapInfo/less.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			SendDlgItemMessage( hDlg, IDC_STATIC_HUNTMAPINFO_IMG, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hMapBitmap);
		}
	}
    char* pText = GetTxtFromMgr(969);
	if(strstr(szMapName, pText))
	{
		pText = GetTxtFromMgr(3761);
		sprintf(buffer,pText,strdir);
		SetWindowText(GetDlgItem(hDlg,IDC_EDIT_HUNTMAPINFO),buffer);
		return ;
	}
	pText = GetTxtFromMgr(3762);
	sprintf(buffer,pText,
			strdir,pclclient->pclMapManager->GetLevelLimit(mapindex));
	SetWindowText(GetDlgItem(hDlg,IDC_EDIT_HUNTMAPINFO),buffer);
}

void CHuntMapInfo::ShowListMonster(HWND hDlg,SI32 siKind[],SI32 siLevel[])
{
	if(hDlg == NULL)return ;

	char buffer[256] = "" ;

	ListView_DeleteAllItems(m_hMonsterInfo);

	SI32 listindex = 0 ;

	for( SI32 i = 0 ; i < MAX_NPC_KIND_PER_GROUP ; i++ )
	{
		if ( siKind[i] <= 0 ) return ;
		if ( siLevel[i] <= 0 ) return ;

		listindex = ListViewInsertItem(m_hMonsterInfo, -1, 0, 0,(char*)pclClient->pclKindInfoSet->GetName(siKind[i]), -1);

		sprintf(buffer,"Lv. %d",siLevel[i] ) ;
		ListViewInsertItem(m_hMonsterInfo, -1, listindex, 1, buffer, -1);
	}
}