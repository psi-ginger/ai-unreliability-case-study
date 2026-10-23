
// SiegeWinDlg.h: interface for the CSiegeWinDlg class.
/* ==========================================================================
	클래스 :		CNSiegeWinDlg

	작성일 :		05/03/25
	작성자 :		김태양

	목  적 :		
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/


#pragma once


#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatic;


class CNSiegeWinDlg  : public CDialogController
{
public:
	CNSiegeWinDlg();
	virtual ~CNSiegeWinDlg();

	static void CALLBACK	StatiCNSiegeWinProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NSiegeWinProc( UINT nEvent, int nControlID, CControllerObj* pControl);

	void Create();
	void Set(TCHAR* ptext);

private:
	
	CStatic     	*m_pSiegeWinStatic;

	//HWND m_hSiegeWinDlg;
	//HWND m_hSiegeWinStatic;
};


