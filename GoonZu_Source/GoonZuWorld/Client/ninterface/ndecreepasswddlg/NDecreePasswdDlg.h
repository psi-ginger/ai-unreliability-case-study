//
//
///* ==========================================================================
//	클래스 :		CNNDecreePasswdProc
//
//	작성일 :		05/03/24
//	작성자 :		김태양
//
//	목  적 :		
//
//	용  법 :		
//	변경사항 :		없음
//
//	차후작업 :		cpp 작업 완료 요
//
//   ========================================================================*/
//
//
//#pragma once
//#include <Directives.h>
//#include <windows.h>
//#include <stdio.h>
//
//#include "../../../Resource.h"
//#include "../../InterfaceMgr/DialogController.h"
//
//class CStatic;
//class CButton;
//
//
//
//class CNDecreePasswdDlg  : public CDialogController
//{
//public:
//	CNDecreePasswdDlg();
//	    ~CNDecreePasswdDlg();
//
//	static void CALLBACK	StatiCNDecreePasswdProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
//	void CALLBACK			NDecreePasswdProc( UINT nEvent, int nControlID, CControllerObj* pControl);
//
//
//	void Create();
//
//	BOOL Set(TCHAR * accountID , TCHAR * passwd);
//
//	bool CorrectPasswdRule(TCHAR * accountID , TCHAR * passwd);
//
//private:
//
//	TCHAR	m_szAccountID[64];
//	CStatic     	*m_pDecreePasswdStatic;
//	CButton			*m_pDeathButton1;
//	CButton			*m_pDeathButton2;
//
//	SI32	m_siXsize;
//	SI32	m_siYsize;
//
//};
//
