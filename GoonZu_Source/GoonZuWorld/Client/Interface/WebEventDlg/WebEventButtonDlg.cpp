////---------------------------------
//// 2004/10/01 ¼ºÁØ¿±
////---------------------------------
//
//#include "CommonLogic.h"
//#include "../../Client/Client.h"
//
//#include "WebEventDlg.h"
//
////-----------------------------
//// Common
////-----------------------------
//#include "PersonInfo.h"
//#include "../../Resource.h"
//#include "Char\CharManager\CharManager.h"
//
//#include "../../lib/WebHTML/WebHTML.h"
//
//#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
//
//#include "MsgType-System.h"
//#include "Msg/MsgType-ItemMall.h"
//
//extern cltCommonLogic* pclClient;
//
//CWebEventButtonDlg::CWebEventButtonDlg(SI32 x , SI32 y)
//{
//	m_hWndButton = NULL ;
//	m_hInstButton = NULL ;
//
//	m_hButtonDlg = NULL ;
//	m_hBitmapButton = NULL ;
//
//	for ( int i = 0 ; i < 2 ; i ++ )
//		m_hButtonBitmap[i] = NULL ;
//
//	m_siDrawX = x ;
//	m_siDrawY = y ;
//}
//
//CWebEventButtonDlg::~CWebEventButtonDlg()
//{
//	if ( m_hButtonDlg )
//	{
//		DestroyWindow(m_hButtonDlg);
//		m_hButtonDlg = NULL ;
//	}
//}
//
//void CWebEventButtonDlg::ShowWebEventButton(HWND hParent, HINSTANCE hInst,SI32 wx,SI32 wy)
//{/*
//	if ( !IsWindow(m_hButtonDlg) )
//	{
//		m_hWndButton = hParent ;
//		m_hInstButton = hInst;
//
//		m_hButtonDlg = CreateDialog( m_hInstButton, MAKEINTRESOURCE(IDD_DIALOG_ITEMMALL_BUTTON), m_hWndButton, StaticWebEventButtonMsgProc );
//	}
//
//	if ( !m_hButtonBitmap[0] || !m_hButtonBitmap[1] || !m_hBitmapButton ) return ;
//
//	SI32 drawx = m_siDrawX + wx ;
//	SI32 drawy = m_siDrawY + wy ;
//
//	MoveWindow(m_hButtonDlg,drawx,drawy,ITEMMALL_BUTTON_WIDTH+4,ITEMMALL_BUTTON_HEIGHT+4,TRUE);
//
//	static SI32 frame = 0 ;
//
//	if ( frame == 0 )
//	{
//		SendDlgItemMessage( m_hButtonDlg, IDC_BUTTON_ITEMMALL_POPUP, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hButtonBitmap[0] );
//		MoveWindow(m_hBitmapButton, 0, 0, ITEMMALL_BUTTON_WIDTH+4, ITEMMALL_BUTTON_HEIGHT+4, TRUE);
//	}
//	else if ( frame == 10 )
//	{
//		SendDlgItemMessage( m_hButtonDlg, IDC_BUTTON_ITEMMALL_POPUP, BM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)m_hButtonBitmap[1] );
//		MoveWindow(m_hBitmapButton, 0, 0, ITEMMALL_BUTTON_WIDTH+4, ITEMMALL_BUTTON_HEIGHT+4, TRUE);
//	}
//	else if ( frame == 19 )
//	{
//		frame = -1 ;
//	}
//	frame ++ ;
//
//	ShowWindow(m_hButtonDlg,SW_SHOW);
//	*/
//}
//
//BOOL CALLBACK CWebEventButtonDlg::StaticWebEventButtonMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	return pclclient->m_pWebEventDlg->m_pWebEventButtonDlg->WebEventButtonButtonMsgProc( hDlg, iMsg, wParam, lParam );
//}
//
//BOOL CALLBACK CWebEventButtonDlg::WebEventButtonButtonMsgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//
//	switch(iMsg)
//	{
//	case WM_INITDIALOG:
//		{
//			m_hButtonBitmap[0] = (HBITMAP)LoadImage( m_hInstButton, TEXT("ItemMall/1.bmp"),IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
//			m_hButtonBitmap[1] = (HBITMAP)LoadImage( m_hInstButton, TEXT("ItemMall/2.bmp"),IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE );
//
//			m_hBitmapButton = GetDlgItem(hDlg,IDC_BUTTON_ITEMMALL_POPUP) ;
//		}
//		break;
//	case WM_COMMAND:
//		{
//			switch(LOWORD(wParam))
//			{
//			case IDC_BUTTON_ITEMMALL_POPUP:
//				{
//					if ( pclClient->siServerUnique == 9 && pclclient->m_pWebEventDlg )
//					{
//						TCHAR url[256];
//						pclclient->m_pWebEventDlg->GetSBSSchoolEventUrl(url, pclclient->pclCM->CR[1]->pclCI->clBI.szAccountID,
//																		pclclient->pclCM->CR[1]->GetName());
//						pclclient->m_pWebEventDlg->Set(url,420,500);
//					}
//				}
//				break;
//			}
//		}
//		break;
//	case WM_CLOSE:
//		{
//			DestroyWindow(m_hButtonDlg);
//		}
//		break;
//	case WM_DESTROY:		
//		{
//			for ( int i = 0 ; i < 2 ; i ++ )
//			{
//				if ( m_hButtonBitmap[i] )
//				{
//					DeleteObject(m_hButtonBitmap[i]);
//					m_hButtonBitmap[i] = NULL ;
//				}
//			}
//
//			m_hBitmapButton = NULL ;
//
//			m_hButtonDlg = NULL ;
//		}
//		break;
//	}
//
//	return FALSE ;
//}