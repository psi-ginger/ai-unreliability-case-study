//
//#include "..\..\..\Resource.h"
//
//#include "../../Client/client.h"
//
//#include "../../CommonLogic/MsgType.h"
//#include "../../CommonLogic/CommonLogic.h"
//#include "../../CommonLogic/MsgType-System.h"
//#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
//
//
//#include "SearchDetailDlg.h"
//#include "../../../lib/WebHTML/WebHTML.h"
//#include "../../ToolBar/ToolBar.h"
//
//
////-----------------------------
//// Common
////-----------------------------
//#include "PersonInfo.h"
//#include "Char\CharManager\CharManager.h"
//
//extern cltCommonLogic* pclClient;
//
////////////////////////////////////////////////////////////////////////
//// Construction/Destruction
////////////////////////////////////////////////////////////////////////
//
//#define MAX_TOOLBAR_SEARCH  30
//
//CSearchDetailDlg::CSearchDetailDlg()
//{
//	m_hDlg				= NULL;
//	m_hDlgChild			= NULL;
//	m_hToolBar			= NULL;				// 툴바의 핸들 
//	m_bShow				= false;
//	m_siBulletinBoardType = 0;
//
//	m_pToolBarGameInfo	= new cltToolBarInfo[MAX_TOOLBAR_SEARCH];
//	
//	SI32 siIndex = 0;
//
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	TCHAR* pText = GetTxtFromMgr(1935);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	0, ID_TOOLBAR_BACK,		pText);
//
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	pText = GetTxtFromMgr(1936);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	1, ID_TOOLBAR_FORWARD,	pText);
//
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	pText = GetTxtFromMgr(1937);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	2, ID_TOOLBAR_STOP,		pText);
//
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	pText = GetTxtFromMgr(1938);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	3, ID_TOOLBAR_REFRESH,	pText);
//
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	pText = GetTxtFromMgr(1939);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	4, ID_TOOLBAR_HOME,		pText);
//
//	//===========================================================
//	// 각 게시판 직접가기를 위한 툴바 
//	//===========================================================
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	pText = GetTxtFromMgr(1940);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	5, ID_TOOLBAR_BULLETIN_KNOWLEDGE,		pText);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	
//	pText = GetTxtFromMgr(1941);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	6, ID_TOOLBAR_BULLETIN_SARANGBANG,		pText);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	pText = GetTxtFromMgr(1942);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	7, ID_TOOLBAR_BULLETIN_HYUNGZO,			pText);
//	SetToolBar(siIndex++, TBSTYLE_SEP,		0, 0, NULL);
//	pText = GetTxtFromMgr(1943);
//	SetToolBar(siIndex++, TBSTYLE_BUTTON,	8, ID_TOOLBAR_BULLETIN_VILLAGERESIDENT,	pText);
//	//===========================================================
//
//}
//
//CSearchDetailDlg::~CSearchDetailDlg()
//{
//	if(m_hDlg) DestroyWindow(m_hDlg);
//	
//	if(m_pToolBarGameInfo) delete [] m_pToolBarGameInfo;
//	m_pToolBarGameInfo = NULL;
//	m_hDlg				= NULL;
//	m_hDlgChild			= NULL;
//	m_hToolBar			= NULL;				// 툴바의 핸들 
//	m_bShow				= false;
//
//}
//
//void CSearchDetailDlg::SetToolBar(SI32 index, SI32 style, SI32 bitmap, SI32 idcommand, TCHAR *Tooltip)
//{
//	if(index>=MAX_TOOLBAR_SEARCH)	return;
//	
//	if(style == TBSTYLE_SEP)
//	{
//		m_pToolBarGameInfo[index].iBimap	= 0;
//		m_pToolBarGameInfo[index].idCommand = 0;
//		m_pToolBarGameInfo[index].fsStyle	= TBSTYLE_SEP;
//		index++;
//		m_siToolBarNumber	= index;
//	}
//	else if(style == TBSTYLE_BUTTON) 
//	{
//		
//		m_pToolBarGameInfo[index].iBimap		= bitmap;
//		m_pToolBarGameInfo[index].idCommand		= idcommand;
//		m_pToolBarGameInfo[index].fsStyle		= TBSTYLE_BUTTON;
//		if(Tooltip)	StringCchCopy(m_pToolBarGameInfo[index].szTip, 128,	Tooltip);
//		index++;
//		m_siToolBarNumber	= index;
//	}
//}
//
//void CSearchDetailDlg::Create(HINSTANCE hInst, HWND hParent )
//{
//	m_hInst = hInst ;
//	m_hWnd  = hParent ;
//	WNDCLASSEX		wc;
//	
//	ZeroMemory(&wc, sizeof(WNDCLASSEX));
//	
//	wc.cbSize		= sizeof(WNDCLASSEX);
//	wc.hInstance	= m_hInst;
//	wc.lpfnWndProc	= StaticSearchDetailDlgProc;
//	wc.style		= CS_CLASSDC|CS_HREDRAW|CS_VREDRAW|CS_PARENTDC|CS_BYTEALIGNCLIENT|CS_DBLCLKS;//CS_HREDRAW|CS_VREDRAW; 
//	TCHAR* pText = GetTxtFromMgr(1944);
//	wc.lpszClassName= pText;
//	wc.hIcon		= LoadIcon( m_hInst, MAKEINTRESOURCE(IDI_ICON_GOONZU));
//	wc.hCursor		= LoadCursor(m_hInst, IDC_ARROW);
//	wc.hbrBackground= (HBRUSH)GetStockObject(WHITE_BRUSH);
//	
//	
//	if(RegisterClassEx(&wc)==NULL)
//	{
//		MessageBox(NULL, TEXT("ERROR WEB SEARCH"), TEXT("WEB"), MB_OK|MB_TOPMOST);
//	}
//
//
//	wc.lpfnWndProc	= StaticBrowserWindowProc;
//	wc.style		= CS_HREDRAW|CS_VREDRAW;
//	pText = GetTxtFromMgr(1945);
//	wc.lpszClassName = pText;
//
//	if(RegisterClassEx(&wc)==NULL)
//	{
//		MessageBox(NULL, TEXT("ERROR WEB SEARCH CHILD"), TEXT("WEB"), MB_OK|MB_TOPMOST);
//	}
//	
//	TCHAR* pTitle = GetTxtFromMgr(1944);
//	pText = GetTxtFromMgr(1946);
//	if ((m_hDlg = CreateWindowEx(0, pTitle, pText, WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU,//WS_CHILD|WS_VISIBLE,
//			CW_USEDEFAULT, CW_USEDEFAULT, 495, 505+40, HWND_DESKTOP, NULL, m_hInst, 0))) // HWND_DESKTOP
//		{
//			// 클라이언트용 툴바. 
//			TBBUTTON ToolBtn[MAX_TOOLBAR_SEARCH];
//			
//			for(SI32 i = 0; i < m_siToolBarNumber ;i++)
//			{
//				ToolBtn[i].iBitmap		= m_pToolBarGameInfo[i].iBimap;
//				ToolBtn[i].idCommand	= m_pToolBarGameInfo[i].idCommand;
//				ToolBtn[i].fsState		= TBSTATE_ENABLED;
//				ToolBtn[i].fsStyle		= m_pToolBarGameInfo[i].fsStyle;
//				ToolBtn[i].dwData		= 0;
//				ToolBtn[i].iString		= 0;
//			}
//
//			m_hToolBar  = CreateToolbarEx(m_hDlg, WS_CHILD|WS_BORDER|WS_VISIBLE|TBSTYLE_TOOLTIPS,
//				ID_TOOLBAR_SEARCH, 
//				5,							// 툴 버튼 비트맵의 갯수. 
//				m_hInst,					// 부모의 인스턴스 
//				IDB_BITMAP_SEARCH,			// 비트맵의 핸들 
//				ToolBtn,					// 툴버튼에 대한 정보를 가진 TBBUTTON구조체 배열 
//				m_siToolBarNumber,			// 툴버튼 갯수. 
//				16, 16,						// 버튼의 가로 세로 크기. 		
//				20/*GetToolBarXsize()*/,	// 비트맵의 크기. (X)
//				20/*GetToolBarYsize()*/,	// 비트맵의 크기. (Y)
//				sizeof(TBBUTTON));
//
//			
//			
//			// Create a child window to host the browser object. 
//			// NOTE: We embed the browser object duing our WM_CREATE handling for this child window.
//			pText = GetTxtFromMgr(1945);
//			if ((m_hDlgChild = CreateWindowEx(0, pText, 0, WS_CHILD|WS_VISIBLE, 0, 40, 0, 0, m_hDlg, NULL, m_hInst, 0)))
//			{
//				// Show the Main window.
//				UpdateWindow(m_hDlg);
//			}
//			else
//			{
//				DestroyWindow(m_hDlg);
//			}
//	}
//	
//	
//
//}
//
//LRESULT CALLBACK CSearchDetailDlg::StaticBrowserWindowProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	return pclclient->m_pSearchDetailDlg->BrowserWindowProc( hDlg, iMsg, wParam, lParam );
//}
//
//
//LRESULT CALLBACK CSearchDetailDlg::StaticSearchDetailDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	return pclclient->m_pSearchDetailDlg->SearchDetailDlgProc( hDlg, iMsg, wParam, lParam );
//}
//
//
//LRESULT CALLBACK CSearchDetailDlg::BrowserWindowProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
//{
//	switch (iMsg)
//	{
//		case WM_CREATE:
//		{
//			if (EmbedBrowserObject(hDlg)) return(-1);
//			return(0);
//		}		
//		case WM_SIZE:
//		{
//			// Resize the browser object to fit the window
//			ResizeBrowser(hDlg, LOWORD(lParam), HIWORD(lParam));
//			return(0);
//		}
//		case WM_DESTROY:
//		{
//			// Detach the browser object from this window, and free resources.
//			UnEmbedBrowserObject(hDlg);
//			m_hDlgChild = 0;
//		}
//	}
//
//	return(DefWindowProc(hDlg, iMsg, wParam, lParam));
//}
//
//LRESULT CALLBACK CSearchDetailDlg::SearchDetailDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam )
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	switch (iMsg)
//	{
//	case WM_SIZE:
//		{
//			if(m_hDlgChild)MoveWindow(m_hDlgChild, 0, 40, LOWORD(lParam), HIWORD(lParam) - 40, TRUE);
//
//			return(0);
//		}
//	case WM_CLOSE:
//		{
//			m_bShow = false;
//			ShowWindow(hDlg, SW_HIDE);
//		}
//		return TRUE;
//	
//	case WM_DESTROY:
//		{
//			m_hDlg = NULL ;
//		}
//		break;
//	case WM_NOTIFY:
//		{
//			switch(( (LPNMHDR)lParam)->code)
//			{
//			case TTN_NEEDTEXT:
//				if(m_pToolBarGameInfo)
//				{
//					for(SI32 i = 0; i < m_siToolBarNumber; ++i)
//					{
//						if(m_pToolBarGameInfo[i].idCommand == (SI32)wParam)
//						{
//							((LPTOOLTIPTEXT)lParam)->lpszText = m_pToolBarGameInfo[i].szTip;
//							break;
//						}
//					}
//				}
//				break;
//
//			default:
//				break;
//			}
//
//		}
//		break;
//	case WM_COMMAND:
//		{
//			if(m_hDlgChild)
//			{
//				
//				switch (LOWORD(wParam))
//				{
//				case ID_TOOLBAR_BACK:		// Back button
//					{
//						DoPageAction(m_hDlgChild, WEBPAGE_GOBACK);
//						break;
//					}
//				case ID_TOOLBAR_FORWARD:	// Forward button	
//					{
//						DoPageAction(m_hDlgChild, WEBPAGE_GOFORWARD);
//						break;
//					}
//				case ID_TOOLBAR_STOP:		// Stop button
//					{
//						DoPageAction(m_hDlgChild, WEBPAGE_STOP);
//						break;
//					}
//				case ID_TOOLBAR_REFRESH:	// Refresh button
//					{
//						DoPageAction(m_hDlgChild, WEBPAGE_REFRESH);
//						break;
//					}
//				case ID_TOOLBAR_HOME:
//					{
//						GoURL(m_siBulletinBoardType, NULL);
//						break;
//					}
//				
//				case ID_TOOLBAR_BULLETIN_KNOWLEDGE:
//					{
//						GoURL(BULLETINBOARD_KNOWLEDGE, NULL);
//						break;
//					}
//				case ID_TOOLBAR_BULLETIN_SARANGBANG:
//					{
//						GoURL(BULLETINBOARD_SARANGBANG, NULL);
//						break;
//					}
//				case ID_TOOLBAR_BULLETIN_HYUNGZO:
//					{
//						GoURL(BULLETINBOARD_HYUNGZO, NULL);
//						break;
//					}
//				case ID_TOOLBAR_BULLETIN_VILLAGERESIDENT:
//					{
//						//GoURL(BULLETINBOARD_VILLAGERESIDENT, NULL);
//						cltPBasicInfo * pclBI = &pclclient->pclCM->CR[1]->pclCI->clBI ;
//						if ( pclBI == NULL ) break ;
//
//						if ( pclBI->siHomeVillage > 0 && pclBI->siHomeVillage <= CUR_VILLAGE_LIMIT &&
//							 pclclient->pclVillageManager->GetName(pclBI->siHomeVillage) != NULL )
//							GoURL(BULLETINBOARD_VILLAGERESIDENT, NULL);
//						else
//						{
//							cltMsg clMsg(GAMEMSG_REQUEST_VILLAGEBOARD,0,NULL);
//							pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
//						}
//						break;
//					}
//				}
//			}
//
//		}
//		break;
//	}
//	
//	return(DefWindowProc(hDlg, iMsg, wParam, lParam));
//}
//
//
//void CSearchDetailDlg::Show(SI32 siBulletinBoardType, TCHAR *pKeyWord,SI32 siVillage)
//{
//	if(m_bShow == false)
//	{
//		ShowWindow(m_hDlg, SW_SHOW);
//		m_bShow=true;
//	}
//	
//	if(m_hDlg)
//	{
//		GoURL(siBulletinBoardType, pKeyWord,siVillage);
//
//		m_siBulletinBoardType = siBulletinBoardType;
//
//		SetForegroundWindow(m_hDlg);
//	}
//	
//}
//
//
//void CSearchDetailDlg::GoURL(SI32 siBoardType, TCHAR *pKeyword,SI32 siVillage)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//
//	TCHAR url[ MAX_PATH ];
//	TCHAR SearchURL[MAX_PATH];	
//
//	switch(siBoardType)
//	{
//	case BULLETINBOARD_KNOWLEDGE:
//		{
//			if(pKeyword == NULL || _tcslen(pKeyword) <=0)
//			{
//				StringCchPrintf(SearchURL, MAX_PATH, TEXT("center/kboard/client_kboard_main.asp"));
//			}
//			else
//			{
//				//sprintf(SearchURL,TEXT("client_kboard_list.asp?search_type=N&search_text=%s"), pKeyword);
//				StringCchPrintf(SearchURL, MAX_PATH, TEXT("center/kboard/client_kboard_list.asp?search_type=Y&search_text=%s"), pKeyword );
//			}
//
//			pclClient->GetSubSiteUrl( url, SearchURL, pclClient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//		}
//		break;
//	case BULLETINBOARD_SARANGBANG:
//		{
//			pclClient->GetSubSiteUrl( url, TEXT("center/hq/list.asp?board_idty=201"), pclClient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//		}
//		break;
//	case BULLETINBOARD_HYUNGZO:
//		{
//			pclClient->GetSubSiteUrl( url,TEXT("center/hq/list.asp?board_idty=211"),	pclClient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//		}
//		break;
//	case BULLETINBOARD_VILLAGERESIDENT:
//		{
//			TCHAR buffer[512] = TEXT("") ;
//			TCHAR szServer[20] = TEXT("") ;
//			TCHAR szRankType[16] = TEXT("") ;
//			TCHAR szVillage[20] = TEXT("") ;
//			TCHAR szVillageCategory[20] = TEXT("") ;
//
//			SI32 siHomeVillage = pclClient->pclCM->CR[1]->pclCI->clBI.siHomeVillage ;
//			if ( pclClient->pclVillageManager->GetName(siHomeVillage) != NULL )
//				StringCchCopy(szVillage, 20, pclClient->pclVillageManager->GetName(siHomeVillage));
//
//			if ( siVillage > 0 && siVillage < MAX_VILLAGE_NUMBER )
//			{
//				if ( pclClient->pclVillageManager->GetName(siVillage) != NULL )
//					StringCchCopy(szVillageCategory, 20, pclClient->pclVillageManager->GetName(siVillage));
//			}
//			else
//			{
//				StringCchCopy(szVillageCategory, 20, szVillage);
//
//				if ( pclClient->pclCM->CR[1]->pclCI->clCharRank.GetRankType() == RANKTYPE_CITYHALL )
//				{
//					TCHAR* pText = GetTxtFromMgr(1947);
//					StringCchCopy(szRankType, 16, pText);
//				}
//				else 
//				{
//					TCHAR* pText = GetTxtFromMgr(1216);
//					StringCchCopy(szRankType, 16, pText);
//				}
//			}
//
//			StringCchCopy(buffer, 512, pclclient->clSelectedWorldInfo.baseinfo.worldname );
//			int i = 0 ;
//			while(true)
//			{
//				if ( buffer[i] == '(' || i == 10 )
//					break ;
//				i ++ ;
//			}
//			memcpy(szServer,buffer,i);
//			szServer[i] = NULL ;
//#ifdef _DEBUG
//			StringCchCopy(szServer, 20, TEXT("태조"));
//#endif
//			StringCchPrintf(buffer, 512, TEXT("center/hq/village/list.asp?game_server=%s")
//				   TEXT("&game_character=%s&game_area=%s&game_job_area=%s")
//				   TEXT("&game_job_class=%s&category_server=%s&category_area=%s"),
//				   szServer,pclClient->pclCM->CR[1]->GetName(),
//				   szVillage,szVillage,szRankType,szServer,szVillageCategory);
//
//			pclClient->GetSubSiteUrl( url,buffer,	pclClient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//			// 마을 게시판 열기 퀘스트 진행중인가 ?
//			if(pclClient->pclCM && pclClient->pclCM->CR[1])
//			{
//				pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENVILLAGEBOARD);
//			}
//		}
//		break;
//	case BULLETINBOARD_BROADCAST:
//		{
//			pclClient->GetSubSiteUrl( url,TEXT("center/hq/list.asp?menu=new&board_type=3&board_idty=801"),
//									  pclClient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//		}
//		break;
//	case BULLETINBOARD_MARKET:
//		{
//			pclClient->GetSubSiteUrl( url,TEXT("center/hq/list.asp?board_idty=207"),
//				pclClient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
//		}
//		break;
//	}	
//
//	DisplayHTMLPage(m_hDlgChild, url);
//
//}