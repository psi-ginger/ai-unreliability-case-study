//---------------------------------
// 2003/10/1 김태곤
//---------------------------------
#include "..\Client.h"
#include "ItemInfo.h"

#include "..\..\Resource.h"
#include "Msg\MsgType-Quest.h"
#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
#include "../../Client/Interface/MonsterInfoDlg/MonsterInfoDlg.h"

#include "../../Client/Client.h"
#include "../../Client/NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

extern CSoundOnOffDlg g_SoundOnOffDlg;
extern cltCommonLogic* pclClient;
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

WNDPROC OldEditProc;

cltItemDetailInfo::cltItemDetailInfo()
{
	m_hDlg = NULL;
	DlgRectInitSwitch	= FALSE;

	hItemImage				= NULL;
	m_hSearchDetailBitmap	= NULL;
	
	m_hEditBrush		= NULL;
	m_hComboListCtrl	= NULL;
	m_hMonsterListCtrl	= NULL;
	m_hMakeItemListCtrl	= NULL;
	m_hImgSM			= NULL;

	m_bShow = false;
}

cltItemDetailInfo::~cltItemDetailInfo()
{
	if ( m_hDlg )
	{
		DestroyWindow( m_hDlg );
		m_hDlg = NULL;
	}
}

BOOL cltItemDetailInfo::Create(HINSTANCE hinst, HWND hwnd)
{
	// 대화상자가 없는 상태라면 만든다. 
	if(!IsWindow(m_hDlg))
	{
		m_hDlg = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DIALOG_ITEMINFO), hwnd, StaticItemInfoDlgProc);
		if(DlgRectInitSwitch == TRUE)
		{
			MoveWindow(m_hDlg, DlgRect.left, DlgRect.top, DlgRect.right - DlgRect.left, DlgRect.bottom - DlgRect.top, FALSE);
		}
	}
	Hide();
	return TRUE;
}


void cltItemDetailInfo::Show()
{
	if ( m_bShow )	{	Hide();	return;	}

	if(m_hDlg && m_bShow ==false)	
	{
		m_bShow = true;
		ShowWindow( m_hDlg, SW_SHOWNORMAL );
		//-----------------------------------------------------------
		// 캐릭터 창을 여는 퀘스트가 진행중이라면 
		// 게임 서버로 퀘스트가 완료되었음을 알려준다.
		//-----------------------------------------------------------
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_ITEMINFODLG;
		if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
			cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (char*)&clinfo);
			cltClient* pclclient = (cltClient* )pclClient;
			pclclient->SendMsgToServer((char*)&clMsg);
		}
	}
	SetForegroundWindow( m_hDlg );
	return;
}

void cltItemDetailInfo::Hide()
{
	
	if(m_hDlg && m_bShow)	
	{
		m_bShow = false;
		ShowWindow( m_hDlg, SW_HIDE );
	}

	return;
}

void cltItemDetailInfo::ShowItemInfo(char* pStrname)
{
	if(pStrname ==NULL) return;

	// 대화상자가 안보인다면 보이게한다.
	if ( m_bShow == false )	
	{
		m_bShow = true;
		ShowWindow( m_hDlg, SW_SHOWNORMAL );
	}
	
	if(m_hDlg)	
	{
	
		HWND hType = GetDlgItem( m_hDlg, IDC_COMBO_ITEMINFO_TYPE );
		HWND hName = GetDlgItem( m_hDlg, IDC_COMBO_ITEMINFO_NAME );
		
		SI32 type;
		char* ptypename = NULL;
		SI32 itemunique	= pclClient->pclItemManager->FindUniqueFromName(pStrname) ;
		if(itemunique > 0)
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
			
			if(ref > 0)
			{
				type = pclClient->pclItemManager->pclItemInfo[ref]->siType;
				
				ptypename = (char *)(pclClient->pclItemManager->pclItemTypeInfo[type]->GetName());
			}
		}

		if(ptypename  ==NULL) return;
		
		SendMessage( hType, CB_SELECTSTRING, 0, (LPARAM)ptypename ); // !=CB_ERR) &&

		SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(ptypename);
		if(itemtypeunique >=0)
		{
			// itemtypeunique에 해당하는 아이템 리스트를 보여준다. 
			cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
			pclitemmanager->ShowItemListInCombo(m_hDlg, IDC_COMBO_ITEMINFO_NAME, itemtypeunique, false);
		}
		SendMessage( hName, CB_SELECTSTRING, 0, (LPARAM)pStrname );  // !=CB_ERR))
		SetItemInfo(m_hDlg, pStrname);
	}
}

BOOL CALLBACK cltItemDetailInfo::StaticItemInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	cltClient* pclclient = (cltClient*)pclClient;

	return pclclient->pclItemDetailInfo->ItemInfoDlgProc(hDlg, iMsg, wParam, lParam);
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_KEYDOWN:
		if(wParam == VK_RETURN)
		{
			cltClient* pclclient = (cltClient*)pclClient;
			HWND hmain = pclclient->pclItemDetailInfo->m_hDlg;
			SendMessage(hmain, WM_USER+1, 0, 0);
			return 0;
		}

	case WM_KEYUP:
	case WM_CHAR:
		if(wParam == VK_RETURN)
		{
			return 0;
		}
	}

	return CallWindowProc(OldEditProc, hWnd, iMessage, wParam, lParam);
}
BOOL CALLBACK cltItemDetailInfo::ItemInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hEdit;
	static HWND hCombo;
	char szSelText[128];

	cltClient *pclclient = (cltClient *)pclClient;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			hCombo = GetDlgItem(hDlg, IDC_COMBO_ITEMINFO_NAME);
			POINT pt;
			pt.x = 5;
			pt.y = 5;
			hEdit	= ChildWindowFromPoint(hCombo, pt);
			OldEditProc = (WNDPROC)SetWindowLong(hEdit, GWL_WNDPROC, (DWORD)EditProc);


			// 아이템 종류를 보여준다. 
			cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
			pclitemmanager->SetItemTypeInCombo(hDlg, IDC_COMBO_ITEMINFO_TYPE, true);
			

			// 지식검색버튼 
			m_hSearchDetailBitmap = (HBITMAP)LoadImage( pclClient->GetInstance(), "Interface/SearchDetailDlg/search.bmp", IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
			if ( m_hSearchDetailBitmap != NULL )
			{
				SendDlgItemMessage( hDlg, IDC_SEARCH_ITEMINFO, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hSearchDetailBitmap );

				HWND hwnd = GetDlgItem(hDlg, IDC_SEARCH_ITEMINFO);
				if(hwnd)	MoveWindow(hwnd, 330, 1, 23, 27, TRUE);
			}

			// EditBrush 
			m_hEditBrush = CreateSolidBrush( COLOR_DIALOG_WHITEBLUE);
		
			
			// IconList
			m_hComboListCtrl = GetDlgItem( hDlg, IDC_EDIT_ITEMINFO_COMBO );

			m_hImgSM	= ImageList_LoadBitmap(pclClient->GetInstance(), MAKEINTRESOURCE(IDB_BITMAP_ITEMINFO_NEED), 16, 1, RGB(255,255,255) );
			SendMessage(m_hComboListCtrl, LVM_SETIMAGELIST, (WPARAM)LVSIL_SMALL, (LPARAM)m_hImgSM);
			
			if(m_hComboListCtrl)
			{
				ListView_SetExtendedListViewStyle( m_hComboListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_GRIDLINES );

				char* pText = GetTxtFromMgr(1222);
				
				ListViewInsertColumn( m_hComboListCtrl, 100, pText, 0, 1);		
				pText = GetTxtFromMgr(1223);
				
				ListViewInsertColumn( m_hComboListCtrl,  36, pText, 1, 1);		
				pText = GetTxtFromMgr(1224);
				
				ListViewInsertColumn( m_hComboListCtrl,  36, pText, 2, 1);		
			}

			// IconList
			m_hMonsterListCtrl = GetDlgItem( hDlg, IDC_EDIT_ITEMINFO_MONSTER );
			if(m_hMonsterListCtrl)
			{
				//ListView_SetExtendedListViewStyle( m_hMonsterListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);
				ListView_SetExtendedListViewStyle( m_hMonsterListCtrl,  LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
				ListView_SetHoverTime( m_hMonsterListCtrl, 10 );

				char* pText = GetTxtFromMgr(1225);
				
				ListViewInsertColumn( m_hMonsterListCtrl,  70, pText, 0, 1);		
				pText = GetTxtFromMgr(1226);
				
				ListViewInsertColumn( m_hMonsterListCtrl,  85, pText,   1, 1);		
			}

			// IconList
			m_hMakeItemListCtrl = GetDlgItem( hDlg, IDC_EDIT_ITEMINFO_MAKEITEM );
			if(m_hMakeItemListCtrl)
			{
				ListView_SetExtendedListViewStyle( m_hMakeItemListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);
				//ListView_SetExtendedListViewStyle( m_hMakeItemListCtrl,  LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
				ListView_SetHoverTime( m_hMakeItemListCtrl, 10 );

				char* pText = GetTxtFromMgr(1227);
				
				ListViewInsertColumn( m_hMakeItemListCtrl,  120, pText,    0, 1);		
				pText = GetTxtFromMgr(1223);
				
				ListViewInsertColumn( m_hMakeItemListCtrl,   52, pText,      1, 1);		
			}
			

			// 타이머를 설정한다.
			SetTimer(hDlg, 1, 10000, NULL);
		}
		return TRUE;
	case WM_CTLCOLORSTATIC:
		{
			if ( GetDlgItem( hDlg, IDC_EDIT_ITEMINFO_EXPLAIN ) == (HWND)lParam ||
				GetDlgItem( hDlg, IDC_EDIT_ITEMINFO_FEATURE ) == (HWND)lParam  ||
				GetDlgItem( hDlg, IDC_EDIT_ITEMINFO_PRODUCTION ) == (HWND)lParam )
			{
				HDC hdc = (HDC)wParam;
				SetTextColor( hdc, COLOR_DIALOG_BLACK );
				SetBkColor( hdc, COLOR_DIALOG_WHITEBLUE);
				return (BOOL)m_hEditBrush;
			}
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

			if ( hdr->hwndFrom == m_hMonsterListCtrl )
			{
				switch( hdr->code )
				{
				case LVN_ITEMACTIVATE:
					{
						char buffer[256]; 
						ListView_GetItemText(m_hMonsterListCtrl, nia->iItem, 0, buffer, 64 );

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
	case WM_TIMER:
		{
			
			GetWindowText(GetDlgItem(hDlg, IDC_COMBO_ITEMINFO_NAME), szSelText, 100);
			SetItemInfo(hDlg, szSelText);
			
		}

		return TRUE;

	case WM_USER+1:
		GetWindowText(GetDlgItem(hDlg, IDC_COMBO_ITEMINFO_NAME), szSelText, 100);
		SetItemInfo(hDlg, szSelText);
		return 0;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			Hide();
			return TRUE;
			
		case IDCANCEL:
			Hide();
			return TRUE;

		case IDC_COMBO_ITEMINFO_TYPE:
			{
				switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					{

						ComboGetString(hDlg, IDC_COMBO_ITEMINFO_TYPE, (LPARAM)szSelText);

						SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);
						if(itemtypeunique >=0)
						{
							// itemtypeunique에 해당하는 아이템 리스트를 보여준다. 
							cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
							pclitemmanager->ShowItemListInCombo(hDlg, IDC_COMBO_ITEMINFO_NAME, itemtypeunique, false);

						}
					}
					break;
				}
			}
			break;

		case IDC_COMBO_ITEMINFO_NAME:
			{
				switch(HIWORD(wParam))
				{
				case CBN_SELCHANGE:
					{
						
						char szSelText[128]="";

						ComboGetString(hDlg, IDC_COMBO_ITEMINFO_NAME, (LPARAM)szSelText);

						// 아이템 정보를 표시한다.
						SetItemInfo(hDlg, szSelText);

					}
					break;
				}
			}
			break;
		case IDC_SEARCH_ITEMINFO:
			{
				char szSelText[128]="";
				
				ComboGetString(hDlg, IDC_COMBO_ITEMINFO_NAME, (LPARAM)szSelText);
				
				if(strcmp(szSelText, "") != 0)
				{
					cltClient *pclclient = (cltClient *)pclClient;
					pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, szSelText);
				}
			}
			break;
			


		}
		return TRUE;
	case WM_CLOSE:
		{
			Hide();
		}
		break;
		
	case WM_DESTROY:
		SetWindowLong(hEdit, GWL_WNDPROC, (LONG)OldEditProc);

		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);

		if(hItemImage)
		{
			DeleteObject(hItemImage);
			hItemImage = NULL;
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
		if ( m_hImgSM )
		{
			ImageList_Destroy( m_hImgSM );
			m_hImgSM = NULL;
		}
		// 윈도우의 위치를 기억한다. 
		GetWindowRect(hDlg, &DlgRect);
		DlgRectInitSwitch = TRUE;

		m_hComboListCtrl = NULL;
		m_hMonsterListCtrl = NULL;
		m_hDlg = NULL;
		return TRUE;

	}

	return FALSE;
}

void cltItemDetailInfo::SetItemInfo(HWND hDlg, char* szSelText)
{

	SI32 id = 1;


	SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName(szSelText);
	if(itemunique)
	{
		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		char explaintext[512]="";
		char temp[256]="";
		
		//-----------------------------------------
		// 아이템의 이미지를 보여준다. 
		//-----------------------------------------
		if(hItemImage)
		{
			DeleteObject(hItemImage);
			hItemImage = NULL;
		}
		
		HDC hdc= GetDC(hDlg);
		hItemImage = pclitemmanager->MakeDDBFromDIB(hdc, pclitemmanager-> GetImage(itemunique));
		//ImageList_AddMasked( hImageList, hItemImage, CLR_DEFAULT );
		SendDlgItemMessage( hDlg, IDC_STATIC_ITEMINFO_IMG, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hItemImage);
		ReleaseDC(hDlg, hdc);
		
		//----------------------------------------
		// 아이템의 기타 정보를 표시한다 
		//----------------------------------------
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
		if(ref > 0)
		{
			SI32 explainunique = pclClient->pclItemManager->pclItemInfo[ref]->siExplainUnique;
			if(explainunique)
			{
				char* ptext = GetTxtFromMgr(explainunique);
				if(ptext)
				{
					strcpy(explaintext, ptext);
					strcat(explaintext, "\r\n");
				}
			}
		}
		
		if(pclitemmanager->GetItemAtb(itemunique) & ITEMATB_OBJECT)
		{
			if(pclitemmanager->CanMakableItem(itemunique) == true)
			{
				
				SI32 qualityrate = pclitemmanager->GetQualityRate(itemunique, id);
				char* pText = GetTxtFromMgr(1228);
				
				sprintf(temp, pText, qualityrate, '%', '%');
			}
			else
			{
				char* pText = GetTxtFromMgr(1229);
				

				sprintf(temp, pText);
			}
		}
		else
		{
			char* pText = GetTxtFromMgr(1230);
			

			sprintf(temp, pText);
		}
		strcat(explaintext, temp);
		
		SetDlgItemText(hDlg, IDC_EDIT_ITEMINFO_EXPLAIN, explaintext);
		
		
		
		//-----------------------------------------
		// 아이템의 일반 정보를 얻어온다. 
		//-----------------------------------------
		if(pclitemmanager->GetExplain(itemunique, explaintext) == TRUE)
		{
			SI32 silen = strlen(explaintext);
			if( silen > 2 && explaintext[silen-1] =='\n') {	explaintext[silen-1] = 0;	}
			if( silen > 2 && explaintext[silen-2] =='\r') {	explaintext[silen-2] = 0;	}
			SetDlgItemText(hDlg, IDC_EDIT_ITEMINFO_FEATURE, explaintext);
		}
		
		
		//-----------------------------------------
		// 아이템의 생산 정보를 보여준다. 
		//-----------------------------------------
		if(pclitemmanager->GetProductionExplain(itemunique, &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, explaintext) == TRUE)
		{
			SI32 silen = strlen(explaintext);
			if( silen > 2 && explaintext[silen-1] =='\n') {	explaintext[silen-1] = 0;	}
			if( silen > 2 && explaintext[silen-2] =='\r') {	explaintext[silen-2] = 0;	}
			SetDlgItemText(hDlg, IDC_EDIT_ITEMINFO_PRODUCTION, explaintext);
		}
		//-----------------------------------------
		// 아이템의 생산 물품 정보를 보여준다. 
		//-----------------------------------------
		cltMaterialExplain clMaterialExp[100];
		SI32 siMaterialCount= 0;
		if(pclitemmanager->GetProductionItemExplain(itemunique, &pclClient->pclCM->CR[id]->pclCI->clCharSkill, &pclClient->pclCM->CR[id]->pclCI->clCharItem, clMaterialExp, &siMaterialCount) == TRUE)
		{
			HWND hCombo = GetDlgItem(hDlg, IDC_EDIT_ITEMINFO_COMBO);
			if(hCombo) 
			{
				// 리스트뷰를 모두 지운다. 
				ListviewDelete(hDlg, IDC_EDIT_ITEMINFO_COMBO);

				SI32 ImageIndex = -1;
				SI32 siCount;
				char szHaveCount[50];
				for(SI32 i = 0; i < siMaterialCount; ++i)
				{
					ImageIndex = 0;
					strcpy(szHaveCount, "");
					if( clMaterialExp[i].siUnique == 0 || strstr(clMaterialExp[i].szName, "==="))
					{
						ImageIndex = 0;
						strcpy(szHaveCount, "");
					}
					else
					{
						siCount = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(clMaterialExp[i].siUnique);
						// 0 : BLANK, 1: BLUE, 2:YELLOW, 3:RED
						if(siCount <= 0)									{	ImageIndex = 3;	}
						else if(siCount < atoi(clMaterialExp[i].szCount))	{	ImageIndex = 2;	}
						else if(siCount >= atoi(clMaterialExp[i].szCount))	{	ImageIndex = 1;	}
						itoa(siCount, szHaveCount, 10);
					}
					ListViewInsertItem(hCombo,  ImageIndex , i, 0, clMaterialExp[i].szName, -1);
					ListViewInsertItem(hCombo,  -1,          i, 1, clMaterialExp[i].szCount,-1);
					ListViewInsertItem(hCombo,  -1,          i, 2, szHaveCount,				-1);
				}
			}
		}
		
		//-----------------------------------------
		// 아이템을 주는 몬스터 정보를 보여준다. 
		//-----------------------------------------
		cltDropItem dropitem[100];
		SI32	siMonterCount = 100;
		if(pclClient->pclKindInfoSet->GetItemMonsterExplain(itemunique, dropitem, &siMonterCount) == true)
		{
			HWND hCombo = GetDlgItem(hDlg, IDC_EDIT_ITEMINFO_MONSTER);
			if(hCombo) 
			{
				// 리스트뷰를 모두 지운다. 
				ListviewDelete(hDlg, IDC_EDIT_ITEMINFO_MONSTER);
				
				SI32 siSortIndex = 0;
				for(SI32 i = 0; i < siMonterCount; ++i)
				{
					siSortIndex  = ListViewInsertItem(hCombo,  -1 , 0,	0, dropitem[i].szName, -1);
					ListViewInsertItem(hCombo,  -1,  siSortIndex, 1, dropitem[i].szItemNum,	-1);
				}
			}
			
		}
		//-----------------------------------------
		// 해당 아이템으로 물건을 만들수 있는 물품 정보를 보여준다.
		//-----------------------------------------
		cltMakeItem makeitem[100];
		SI32	siMakeItemCount = 100;
		if(pclitemmanager->GetProductionMakeItemExplain(itemunique, makeitem, &siMakeItemCount) == TRUE)
		{
			HWND hCombo = GetDlgItem(hDlg, IDC_EDIT_ITEMINFO_MAKEITEM);
			if(hCombo) 
			{
				// 리스트뷰를 모두 지운다. 
				ListviewDelete(hDlg, IDC_EDIT_ITEMINFO_MAKEITEM);
				
				SI32 siSortIndex = 0;
				for(SI32 i = 0; i < siMakeItemCount; ++i)
				{
					siSortIndex  = ListViewInsertItem(hCombo,  -1 , 0,	0, makeitem[i].szName, -1);
					ListViewInsertItem(hCombo,  -1,  siSortIndex, 1, makeitem[i].szItemNum,	-1);
				}
			}
		}
		
		//-----------------------------------------------------------
		// 캐릭터 창을 여는 퀘스트가 진행중이라면 
		// 게임 서버로 퀘스트가 완료되었음을 알려준다.
		//-----------------------------------------------------------
		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_ITEMINFO;
		if(pclClient->pclCM->CR[id]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			if(para1 == itemunique)
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (char*)&clinfo);
				cltClient* pclclient = (cltClient* )pclClient;
				pclclient->SendMsgToServer((char*)&clMsg);
			}
		}
		
	}
	
}