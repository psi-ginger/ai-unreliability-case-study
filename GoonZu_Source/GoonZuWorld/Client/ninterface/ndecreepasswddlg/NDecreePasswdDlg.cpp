////#ifdef __SUN_PASSWORD__
//
//
//
//// DecreePasswdDlg.cpp: implementation of the CNDecreePasswdDlg class.
////
////////////////////////////////////////////////////////////////////////
//
//#include "../../Client/client.h"
//
//#include "../../CommonLogic/MsgType.h"
//#include "../../CommonLogic/CommonLogic.h"
//#include "../../CommonLogic/MsgType-System.h"
//
//#include "../../../lib/WebHTML/WebHTML.h"
//
//#include "../../InterfaceMgr/interface/Static.h"
//#include "../../InterfaceMgr/Interface/Button.h"
//
//
//#include "../../InterfaceMgr/InterfaceMgr.h"
//#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
//
//#include "../../InterfaceMgr/InterfaceFile.h"
//
//
//
//#include "NDecreePasswdDlg.h"
//
//extern cltCommonLogic* pclClient;
//
////////////////////////////////////////////////////////////////////////
//// Construction/Destruction
////////////////////////////////////////////////////////////////////////
//
//CNDecreePasswdDlg::CNDecreePasswdDlg()
//{
//		
//
//	m_pDecreePasswdStatic = NULL ;
//	m_pDeathButton1 = NULL ;
//	m_pDeathButton2 = NULL ;
//
//	StringCchCopy(m_szAccountID, 64, TEXT(""));
//
//	m_siXsize = 0 ;
//	m_siYsize = 0 ;
//}
//
//CNDecreePasswdDlg::~CNDecreePasswdDlg()
//{
//	SAFE_DELETE(m_pDecreePasswdStatic);
//	SAFE_DELETE(m_pDeathButton1);
//	SAFE_DELETE(m_pDeathButton2);	
//
//}
//
//void CNDecreePasswdDlg::Create()
//{
//	
//	if(!IsCreate())
//	{
//		CInterfaceFile file;
//
//		file.LoadFile(TEXT("NInterface/Data/NDecreePass/DLG_NDecreePass.ddf"));
//		file.CreatePopupDialog( this, NDECREEPASSWD_DLG, TEXT("dialog_DecreePass"),StatiCNDecreePasswdProc);
//
//		m_pDecreePasswdStatic = new CStatic( this ); 
//		m_pDeathButton1 = new CButton( this );  
//		m_pDeathButton2 = new CButton( this );	     
//
//		file.CreateControl(m_pDecreePasswdStatic, NDECREEPASSWD_DIALOG_STATIC , TEXT("statictext_DecreePass"));
//		file.CreateControl(m_pDeathButton1, NDECREEPASSWD_DIALOG_BUTTON1, TEXT("button_DecreePass1"));	
//		file.CreateControl(m_pDeathButton2, NDECREEPASSWD_DIALOG_BUTTON2 , TEXT("button_DecreePass2"));
//
//		m_pDecreePasswdStatic->SetFileName(TEXT("Interface/EventNotice/decree.SPR"));
//		m_pDecreePasswdStatic->SetFontIndex(0);
//	}
//
//
//}
//
//void CALLBACK CNDecreePasswdDlg::StatiCNDecreePasswdProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
//{		
//	CNDecreePasswdDlg *pThis = (CNDecreePasswdDlg*) pControllerMgr;
//	pThis->NDecreePasswdProc( nEvent, nControlID, pControl );
//}
//
//
//
//void CALLBACK CNDecreePasswdDlg::NDecreePasswdProc(  UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//	
//	cltClient *pclclient = (cltClient*)pclClient;
//	switch( nControlID)
//	{			
//	case COMMON_DIALOG_CLOSEBUTTON:	
//		{
//			DeleteDialog();
//		}
//		break;
//
//	case NDECREEPASSWD_DIALOG_BUTTON1:
//		{
//			switch(nEvent)
//			{
//			case EVENT_BUTTON_CLICKED:
//				{
//					//TCHAR url[ MAX_PATH ];
//					//pclclient->GetCPasswdUrl( url, m_szAccountID );
//					//ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), url, NULL, SW_SHOWNORMAL );
//			
//				}
//				break;		
//			}
//		}break;
//
//	  
//	
//	case NDECREEPASSWD_DIALOG_BUTTON2:
//		{
//		
//			switch(nEvent)
//			{			
//			case EVENT_BUTTON_CLICKED:
//				{
//					DeleteDialog();
//				}break;
//			}
//		}break;  
//  
//  }	
//  return ;
//}
//
//
//
//
//
//BOOL CNDecreePasswdDlg::Set(TCHAR * accountID , TCHAR * passwd)
//{
//	
//	if ( CorrectPasswdRule(accountID,passwd) == true ) 
//	{
//		DeleteDialog();
//		return FALSE ;	
//	}
//
//	if(! IsCreate()){
//
//		TCHAR* pText = GetTxtFromMgr(3382);
//		CInterfaceFile file;
//
//		file.LoadFile(TEXT("NInterface/Data/NDecreePass/DLG_NDecreePass.ddf"));
//		file.CreatePopupDialog( this, NDECREEPASSWD_DLG, TEXT("dialog_DecreePass"), StatiCNDecreePasswdProc );
//
//		m_pDecreePasswdStatic = new CStatic( this ); 
//		m_pDeathButton1 = new CButton( this );  
//		m_pDeathButton2 = new CButton( this );	     
//
//		file.CreateControl(m_pDecreePasswdStatic, NDECREEPASSWD_DIALOG_STATIC , TEXT("statictext_DecreePass"));
//		file.CreateControl(m_pDeathButton1, NDECREEPASSWD_DIALOG_BUTTON1, TEXT("button_DecreePass1"));	
//		file.CreateControl(m_pDeathButton2, NDECREEPASSWD_DIALOG_BUTTON2 , TEXT("button_DecreePass2"));
//
//		m_pDecreePasswdStatic->SetFileName(TEXT("Interface/EventNotice/decree.SPR"));
//		m_pDecreePasswdStatic->SetFontIndex(0);
//	
//		
//		
//		return TRUE;
//	}
//    
//	
//	return FALSE ;
//}
//
//bool CNDecreePasswdDlg::CorrectPasswdRule(TCHAR * accountID , TCHAR * passwd)
//{
//	TCHAR accountid[FILE_NAME_SIZE] = TEXT("") ;
//	TCHAR password[FILE_NAME_SIZE] = TEXT("") ;
//
//	StringCchCopy(accountid, FILE_NAME_SIZE, accountID);
//	StringCchCopy(m_szAccountID, 64, accountid);
//	_tcslwr(accountid);
//
//	StringCchCopy(password, FILE_NAME_SIZE, passwd);
//	_tcslwr(password);
//
//	if ( !_tcsicmp(accountid,password) ) return false ;
//	if ( _tcsstr(accountid,password) ) return false ;
//	if ( _tcslen(password) < 4 ) return false ;
//
//	bool bNumber = false ;
//	bool bString = false ;
//
//	for ( SI32 i = 0 ; i < FILE_NAME_SIZE ; i++ )
//	{
//		if ( password[i] == '\0' || password[i] == NULL )
//			continue ;
//
//		if ( password[i] >= '0' && password[i] <= '9' && !bNumber )
//			bNumber = true ;
//
//		if ( password[i] >= 'a' && password[i] <= 'z' && !bString )
//			bString = true ;
//
//		if ( bNumber && bString )
//			return true ;
//	}
//
//	return false ;
//}
//
////#endif