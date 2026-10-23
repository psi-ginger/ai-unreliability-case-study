//// HtmlDlg.cpp: implementation of the CHtmlDlg class.
////
////////////////////////////////////////////////////////////////////////
//
//#include "../../Client/client.h"
//
//#include "../ErrorMsgDlg/ErrorMsgDlg.h"
//#include "../NotifyMsgDlg/NotifyMsgDlg.h"
//
//#include "../../CommonLogic/MsgType.h"
//#include "../../CommonLogic/CommonLogic.h"
//#include "../../CommonLogic/MsgType-System.h"
//
//#include "../../../lib/WebHTML/WebHTML.h"
//
//#include "HtmlDlg.h"
//
//extern cltCommonLogic* pclClient;
//
////////////////////////////////////////////////////////////////////////
//// Construction/Destruction
////////////////////////////////////////////////////////////////////////
//
//cltHtmlDlgManager::cltHtmlDlgManager()
//{
//}
//
//cltHtmlDlgManager::~cltHtmlDlgManager()
//{
//	for ( int i = 0 ; i < MAX_HTMLDLG_NUM ; i ++ )
//	{
//		if ( m_pCHtmlDlg[i] )
//		{
//			delete m_pCHtmlDlg[i] ;
//			m_pCHtmlDlg[i] = NULL ;
//		}
//	}
//}
//
//void cltHtmlDlgManager::Create(HINSTANCE hInst, HWND hParent)
//{
//	for ( int i = 0 ; i < MAX_HTMLDLG_NUM ; i ++ )
//	{
//		m_pCHtmlDlg[i] = NULL ;
//		m_pCHtmlDlg[i] = new CHtmlDlg ;
//
//		m_pCHtmlDlg[i]->Create(hInst,hParent,i);
//	}
//}
//
//void cltHtmlDlgManager::Set(TCHAR * Url,TCHAR * Txt,bool bWeb,SI32 x,SI32 y,SI32 xsize,SI32 ysize)
//{
//	cltClient *pclclient = (cltClient *)pclClient;
//	for ( int i = 0 ; i < MAX_HTMLDLG_NUM ; i ++ )
//	{
//		if ( m_pCHtmlDlg[i]->GetDlgShow() ) continue ;
//
//		m_pCHtmlDlg[i]->Set(Url,Txt,bWeb,x,y,xsize,ysize);
//		return ;
//	}
//
//	TCHAR* pTitle = GetTxtFromMgr(2123);
//	TCHAR* pText = GetTxtFromMgr(2124);
//
//	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
//}