#include "MonsterInfoDlg.h"

CMonsterInfoDlg g_MonsterInfoDlg;

#include "../../GlobalImgFile/GlobalImgFile.h"

#include "../../Client/client.h"
#include "..\Common\NPCManager\NPC.h"
#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
#include "../../client/ItemInfo/ItemInfo.h"

extern cltCommonLogic* pclClient;

CMonsterInfoDlg::CMonsterInfoDlg()
{
	m_hDlg = NULL;
	m_siSelectedIndex = -1;
	m_hPortraitBitmap = NULL;
	
	m_hSearchDetailBitmap = NULL;

	m_hEditBrush = NULL;
	m_hItemListCtrl = NULL;
	m_hSummonListCtrl = NULL;
	m_hToleranceListCtrl = NULL ;

	Initialize();
}

CMonsterInfoDlg::~CMonsterInfoDlg()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
	}
}

void CMonsterInfoDlg::Initialize()
{
	m_bComboBoxInit = false;
	m_siSort = 0 ;
	return;
}

void CMonsterInfoDlg::Destroy()
{
	return;
}

void CMonsterInfoDlg::Create( HINSTANCE hInst, HWND hWnd )
{
	m_hDlg = ::CreateDialog( hInst, MAKEINTRESOURCE(IDD_DIALOG_MONSTERINFO), hWnd, StaticMonsterInfoDlgProc );
	Hide();
	return;
}

BOOL CALLBACK CMonsterInfoDlg::StaticMonsterInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	return g_MonsterInfoDlg.MonsterInfoDlgProc( hDlg, iMsg, wParam, lParam );
}

BOOL CALLBACK CMonsterInfoDlg::MonsterInfoDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
{
	switch ( iMsg )
	{
	case WM_INITDIALOG:
		{
			cltClient *pclclient = (cltClient *)pclClient;
			m_pBackGroundSpr = pclclient->GetGImgPointer(GIMG_MONSTERINFOBACK);

			// 지식검색버튼 
			m_hSearchDetailBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/SearchDetailDlg/search.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );

			if ( m_hSearchDetailBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_BUTTON_SEARCH_DETAIL, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hSearchDetailBitmap );

				HWND hwnd = GetDlgItem(hDlg, IDC_BUTTON_SEARCH_DETAIL);
				if(hwnd)	MoveWindow(hwnd, 330, 1, 23, 27, TRUE);
			}
			
			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);
			
			
			// ItemList
			m_hItemListCtrl = GetDlgItem( hDlg, IDC_LIST_MONSTER_ITEM );
			if(m_hItemListCtrl)
			{
				//ListView_SetExtendedListViewStyle( m_hItemListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				ListView_SetExtendedListViewStyle( m_hItemListCtrl,  LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
				ListView_SetHoverTime( m_hItemListCtrl, 10 );

				char* pText = GetTxtFromMgr(1831);
				ListViewInsertColumn( m_hItemListCtrl, 120, pText, 0, 1);
				pText = GetTxtFromMgr(1832);
				ListViewInsertColumn( m_hItemListCtrl, 100, pText, 1, 1);	
				pText = GetTxtFromMgr(2362);
				ListViewInsertColumn( m_hItemListCtrl,  80, pText, 2, 1);
			}

			// ToleranceList
			m_hToleranceListCtrl = GetDlgItem(hDlg,IDC_LIST_MONSTER_TOLERANCE);
			if(m_hToleranceListCtrl)
			{
				ListView_SetExtendedListViewStyle( m_hToleranceListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				char* pText = GetTxtFromMgr(1023);
				ListViewInsertColumn( m_hToleranceListCtrl, 100, pText,			0, 1);
				pText = GetTxtFromMgr(2363);
				ListViewInsertColumn( m_hToleranceListCtrl, 150, pText,		1, 1);
			}

			// SummonList
			m_hSummonListCtrl = GetDlgItem( hDlg, IDC_LIST_REGEN_POS );
			if(m_hSummonListCtrl)
			{
				ListView_SetExtendedListViewStyle( m_hSummonListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
				char* pText = GetTxtFromMgr(1611);
				ListViewInsertColumn( m_hSummonListCtrl,  95, pText,			0, 1);
				pText = GetTxtFromMgr(2364);
				ListViewInsertColumn( m_hSummonListCtrl,  75, pText,			1, 1);
				pText = GetTxtFromMgr(2365);
				ListViewInsertColumn( m_hSummonListCtrl, 155, pText,		2, 1);
			}
		}
		return TRUE;
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

			if ( hdr->hwndFrom == m_hItemListCtrl )
			{
				switch( hdr->code )
				{
				case LVN_ITEMACTIVATE:
					{
						char buffer[256]; 
						ListView_GetItemText(m_hItemListCtrl, nia->iItem, 0, buffer, 64 );

						//MessageBox(NULL, buffer, buffer, MB_OK| MB_TOPMOST);
						if(((cltClient*)pclClient)->pclItemDetailInfo)	((cltClient*)pclClient)->pclItemDetailInfo->ShowItemInfo(buffer);
		
					}
					break;
				}
			}
		}
		break;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_MONSTER_INFO ) == (HWND)lParam)
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
		}
		break;

	case WM_PAINT:
		{
			m_siSelectedIndex = (SI16)SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETCURSEL, 0, 0 );
			if ( m_siSelectedIndex > -1 )
			{
				char strMonsterName[ 128 ] = "";
				SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETLBTEXT, m_siSelectedIndex, (LPARAM) strMonsterName );
				ShowMonsterInfo(hDlg, strMonsterName);
			}
		}
		return FALSE;

	case WM_COMMAND:
		{
			switch ( LOWORD(wParam) )
			{
/* 새로 생성하지 않으므로 사용안함 
			case IDOK:
				DestroyWindow(hDlg);
				return TRUE;
				
			case IDCANCEL:
				DestroyWindow(hDlg);
				return TRUE;
*/				
			case IDC_RADIO_NAMESORT:
				{
					if ( m_siSort == 0 ) break ;
					m_siSort = 0 ;
					AddCombo();
				}
				break;
			case IDC_RADIO_LEVELSORT:
				{
					if ( m_siSort == 1 ) break ;
					m_siSort = 1 ;
					AddCombo();
				}
				break;
			case IDC_COMBO_MONSTER_SELECT:
				{
					switch( HIWORD( wParam ) )
					{
					case CBN_SELCHANGE:
						{
							m_siSelectedIndex = (SI16)SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETCURSEL, 0, 0 );
							if ( m_siSelectedIndex > -1 )
							{
								char strMonsterName[ 128 ] = "";
								SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETLBTEXT, m_siSelectedIndex, (LPARAM) strMonsterName );
								ShowMonsterInfo(hDlg, strMonsterName);
							}
						}
						break;
					}
				}
				break;
				case IDC_BUTTON_SEARCH_DETAIL:
				{
					char strMonsterName[ 128 ] = "";
					
					m_siSelectedIndex = (SI16)SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETCURSEL, 0, 0 );
					if ( m_siSelectedIndex > -1 )
					{
						SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETLBTEXT, m_siSelectedIndex, (LPARAM) strMonsterName );
					}
					cltClient *pclclient = (cltClient *)pclClient;
					pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, strMonsterName);
					
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
			
		}
		break;
	}

	return FALSE;

}

bool CMonsterInfoDlg::IsShow()
{
	return m_bShow;
}

void CMonsterInfoDlg::Show()
{
	if ( m_bShow )	{	Hide();	return;	}

	switch(m_siSort)
	{
	case 0: CheckRadioButton(m_hDlg,IDC_RADIO_NAMESORT,IDC_RADIO_LEVELSORT,IDC_RADIO_NAMESORT); break;
	case 1: CheckRadioButton(m_hDlg,IDC_RADIO_NAMESORT,IDC_RADIO_LEVELSORT,IDC_RADIO_LEVELSORT); break;
	}

	if ( ! m_bComboBoxInit )
	{
		if ( AddCombo() )
			m_bComboBoxInit = true;
	}
	m_bShow = true;
	ShowWindow( m_hDlg, SW_SHOWNORMAL );
	SetForegroundWindow( m_hDlg );
	return;
}

void CMonsterInfoDlg::SortName(SI32 comboindex[])
{
	SI32 i;
	SI32 index = 0;

	bool bComboInfo[MAX_KIND_NUMBER];
	SI32 upindex[MAX_KIND_NUMBER];

	for(i = 0;i < MAX_KIND_NUMBER;i++)
	{
		comboindex[i] = -1 ;
		upindex[i] = -1 ;
		bComboInfo[i] = false;
	}

	for(SI32 j = 0;j < MAX_KIND_NUMBER;j++)
	{
		char maxdata[MAX_KIND_NAME_LENGTH] = "ㄱ";
		SI32 maxindex = -1;

		for(i = 0;i < MAX_KIND_NUMBER;i++)
		{
			if(bComboInfo[i] == false && strncmp(maxdata,m_szSortName[i],MAX_KIND_NAME_LENGTH) < 0 )
			{
				strcpy(maxdata,m_szSortName[i]);
				maxindex	= i;
			}
		}

		if(maxindex >= 0)
		{
			bComboInfo[maxindex] = true;
			upindex[index] = maxindex;
			index++;
		}
	}

	for ( i = 0 ; i < index ; i ++ )
	{
		comboindex[i] = upindex[index - i - 1 ] ;
	}
}

bool CMonsterInfoDlg::AddCombo()
{
	ComboDelete(m_hDlg,IDC_COMBO_MONSTER_SELECT);

	cltKindInfo* pclki = NULL;

	if ( m_siSort == 0 )
	{
		SI32 sortindex = 0 ;
		SI32 comboIndex[MAX_KIND_NUMBER] ;

		for ( SI32 i = 0; i < MAX_KIND_NUMBER ; ++i )
		{
			strcpy(m_szSortName[i],"");
			comboIndex[i] = -1 ;

			pclki = pclClient->pclKindInfoSet->pclKI[ i ];

			if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
				continue;

			strcpy(m_szSortName[i],(char*)pclki->GetName());
		}

		SortName(comboIndex);

		for ( i = 0; i < MAX_KIND_NUMBER ; ++i )
		{
			if ( comboIndex[i] < 0 ) continue ;
			if ( !strcmp(m_szSortName[comboIndex[i]],"") ) continue;
			SendDlgItemMessage( m_hDlg, IDC_COMBO_MONSTER_SELECT, CB_ADDSTRING, sortindex, (LPARAM)m_szSortName[comboIndex[i]] );
			sortindex ++ ;
		}
	}
	else
	{
		for ( SI32 j = 1 ; j < MAX_KIND_NUMBER ; j ++ )
		{
			for ( SI32 i = 10; i < MAX_KIND_NUMBER ; ++i )
			{
				pclki = pclClient->pclKindInfoSet->pclKI[ i ];
				//pclClient->pclKindInfoSet->pclKI[ siMonsterKind ]->siAtb == ATB_ATTACK
				if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
					continue;

				if ( pclki->siSortLevel <= 0 || pclki->siSortLevel >= MAX_KIND_NUMBER ) continue ;

				if ( pclki->siSortLevel == j )
				{
					char* pname = (char*)pclki->GetName();

					SendDlgItemMessage( m_hDlg, IDC_COMBO_MONSTER_SELECT, CB_ADDSTRING, j, (LPARAM)pname );
					break ;
				}
			}
		}
	}

	return TRUE ;
}

void CMonsterInfoDlg::ShowMonsterInfo(char *pStrName)
{
	// 화면에 떠있지 않다면 Data를 설정한다.
	if(m_bShow == false)
	{
		switch(m_siSort)
		{
		case 0: CheckRadioButton(m_hDlg,IDC_RADIO_NAMESORT,IDC_RADIO_LEVELSORT,IDC_RADIO_NAMESORT); break;
		case 1: CheckRadioButton(m_hDlg,IDC_RADIO_NAMESORT,IDC_RADIO_LEVELSORT,IDC_RADIO_LEVELSORT); break;
		}

		if ( ! m_bComboBoxInit )
		{
/*			cltKindInfo* pclki = NULL;
			for ( SI32 i = 10; i < MAX_KIND_NUMBER; ++i )
			{
				pclki = pclClient->pclKindInfoSet->pclKI[ i ];
				if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
					continue;
				
				char* pname = (char*)pclki->GetName();
				SendDlgItemMessage( m_hDlg, IDC_COMBO_MONSTER_SELECT, CB_ADDSTRING, 0, (LPARAM)pname );
			}*/
			if ( AddCombo() )
				m_bComboBoxInit = true;
		}
		ShowWindow( m_hDlg, SW_SHOWNORMAL );
	}
	// 몬스터를 찾아 설정한다.

	if(SendMessage( GetDlgItem( m_hDlg, IDC_COMBO_MONSTER_SELECT ), CB_SELECTSTRING, 0, (LPARAM)pStrName ) !=CB_ERR)
	{
		ShowMonsterInfo(m_hDlg, pStrName);
	}

	m_bShow = true;
	return;
}


void CMonsterInfoDlg::Hide()
{
	m_bShow = false;
	ShowWindow( m_hDlg, SW_HIDE );

	return;
}

void CMonsterInfoDlg::ShowMonsterInfo(HWND hDlg, char* szSelText)
{
	//============================================
	// 얻어온 Monster이름으로부터 ATTACK INFO를출력한다.
	//============================================
	SI32 siMonsterKind = pclClient->pclKindInfoSet->FindKindFromName( szSelText );
	char Buffer[ 256 ] = "";
	pclClient->pclKindInfoSet->GetCharAttackInfo( siMonsterKind, Buffer );
	SetWindowText( GetDlgItem( hDlg, IDC_EDIT_MONSTER_INFO ), Buffer );
	
	//============================================
	// PORTRAIT를 출력한다.
	//============================================
	char strFileName[ 128 ] = "";
	if ( m_hPortraitBitmap )	{	DeleteObject( m_hPortraitBitmap );	m_hPortraitBitmap = NULL;	}
	sprintf( strFileName, "Interface/Portrait/%s.bmp",	pclClient->pclKindInfoSet->pclKI[ siMonsterKind ]->szCode );
	m_hPortraitBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), strFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
	if ( m_hPortraitBitmap != NULL )
	{
		SendDlgItemMessage( hDlg, IDC_STATIC_MONSTER_PORTRAIT, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hPortraitBitmap );
	}
	//============================================
	// 몬스터로부터 얻어오는 아이템 정보를 설정한다.
	//============================================
	cltDropItem clDropItem[50];
	SI32		siDropCount = 0;
	if(pclClient->pclKindInfoSet->GetItemFromKindExplain(siMonsterKind, clDropItem, &siDropCount) == true)
	{				
		HWND hCombo = GetDlgItem(hDlg, IDC_LIST_MONSTER_ITEM);
		if(hCombo) 
		{
			// 리스트뷰를 모두 지운다. 
			ListviewDelete(hDlg, IDC_LIST_MONSTER_ITEM);
			
			SI32 siSortIndex = 0;
			for(SI32 i = 0 ; i < siDropCount; ++i)
			{
				siSortIndex = ListViewInsertItem(hCombo, -1 , 0, 0, clDropItem[i].szName,    -1);
				ListViewInsertItem(hCombo, -1,  siSortIndex, 1, clDropItem[i].szItemNum, -1);
				ListViewInsertItem(hCombo, -1,  siSortIndex, 2, clDropItem[i].szPercent, -1);
			}
		}
	}
	else
	{
		HWND hCombo = GetDlgItem(hDlg, IDC_LIST_MONSTER_ITEM);
		if(hCombo) 
		{
			// 리스트뷰를 모두 지운다. 
			ListviewDelete(hDlg, IDC_LIST_MONSTER_ITEM);
		}
	}
	
	//============================================
	// 무기별 내성도을 출력한다.
	//============================================
	HWND hToList = GetDlgItem(hDlg, IDC_LIST_MONSTER_TOLERANCE);
	cltAtkTypeTolerance * pclTolerance = &pclClient->pclKindInfoSet->pclKI[siMonsterKind]->clAtkTypeTolerance ;
	if(hToList && pclTolerance )
	{
		char buffer[256] = "" ;
		// 리스트뷰를 모두 지운다. 
		ListviewDelete(hDlg, IDC_LIST_MONSTER_TOLERANCE);
	
		char* pTitle = GetTxtFromMgr(2366);
		char* pText = GetTxtFromMgr(2367);
		ListViewInsertItem(hToList, -1 , 0, 0, pTitle, -1);
		wsprintf(buffer,pText,pclTolerance->siSwordTolerance);
		ListViewInsertItem(hToList, -1,  0, 1, buffer, -1);

		pTitle = GetTxtFromMgr(2368);
		pText = GetTxtFromMgr(2369);
		ListViewInsertItem(hToList, -1 , 1, 0, pTitle, -1);
		wsprintf(buffer,pText,pclTolerance->siSpearTolerance);
		ListViewInsertItem(hToList, -1,  1, 1, buffer, -1);

		pTitle = GetTxtFromMgr(2370);
		pText = GetTxtFromMgr(2371);
		ListViewInsertItem(hToList, -1 , 2, 0, pTitle, -1);
		wsprintf(buffer,pText,pclTolerance->siAxeTolerance);
		ListViewInsertItem(hToList, -1,  2, 1, buffer, -1);

		pTitle = GetTxtFromMgr(2372);
		pText = GetTxtFromMgr(2373);
		ListViewInsertItem(hToList, -1 , 3, 0, pTitle, -1);
		wsprintf(buffer,pText,pclTolerance->siBowTolerance);
		ListViewInsertItem(hToList, -1,  3, 1, buffer, -1);

		pTitle = GetTxtFromMgr(2374);
		pText = GetTxtFromMgr(2375);
		ListViewInsertItem(hToList, -1 , 4, 0, pTitle, -1);
		wsprintf(buffer,pText,pclTolerance->siGunTolerance);
		ListViewInsertItem(hToList, -1,  4, 1, buffer, -1);

		pTitle = GetTxtFromMgr(2376);
		pText = GetTxtFromMgr(2377);
		ListViewInsertItem(hToList, -1 , 5, 0, pTitle, -1);
		wsprintf(buffer,pText,pclTolerance->siStaffTolerance);
		ListViewInsertItem(hToList, -1,  5, 1, buffer, -1);

		pTitle = GetTxtFromMgr(2378);
		pText = GetTxtFromMgr(2379);
		ListViewInsertItem(hToList, -1 , 6, 0, pTitle, -1);
		wsprintf(buffer,pText,pclTolerance->siCannonTolerance);
		ListViewInsertItem(hToList, -1,  6, 1, buffer, -1);
	}

	//============================================
	// 출몰위치정보를 출력한다.
	//============================================
	cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;
	cltRegenPos	clRegenPos[100];
	SI32	siRegenCount = 0;
	if(pclnpcmanager->GetKindPos(siMonsterKind , clRegenPos, &siRegenCount) == true)
	{	
		HWND hCombo = GetDlgItem(hDlg, IDC_LIST_REGEN_POS);
		if(hCombo) 
		{
			// 리스트뷰를 모두 지운다. 
			ListviewDelete(hDlg, IDC_LIST_REGEN_POS);
		
			SI32 siSortIndex = 0;
			for(SI32 i = 0 ; i < siRegenCount; ++i)
			{
				siSortIndex = ListViewInsertItem(hCombo, -1 , 0, 0, clRegenPos[i].szMapName, -1);
				ListViewInsertItem(hCombo, -1,  siSortIndex, 1, clRegenPos[i].szMapPos,		 -1);
				ListViewInsertItem(hCombo, -1,  siSortIndex, 2, clRegenPos[i].szNearMapInfo, -1);
			}			
		}
	}
	else
	{
		HWND hCombo = GetDlgItem(hDlg, IDC_LIST_REGEN_POS);
		if(hCombo) 
		{
			// 리스트뷰를 모두 지운다. 
			ListviewDelete(hDlg, IDC_LIST_REGEN_POS);
		}
	}
	

}

