// EventNotice.h: interface for the CNEventNotice class.
//
//////////////////////////////////////////////////////////////////////

/* ==========================================================================
	클래스 :		CNNPCTradeDlg

	작성일 :		2005/04/08
	작성자 :		김태양

	목  적 :		.
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/


#pragma once


#include <Directives.h>
#include <stdio.h>

#include "../../../Resource.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


class CStatic;
class CEdit;

class CNEventNoticeDlg : public CDialogController  
{     
public:
	CNEventNoticeDlg();
	 ~CNEventNoticeDlg();


	void                    Create();
	void                    Set(TCHAR * szGift, SI16 txtSize, TCHAR * Txt);
	static void CALLBACK	StatiCNEventNoticeDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NEventNoticeDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	
	

private:
	

	BOOL                    m_bRectSwitch;
	RECT                    m_rtDlg;
	CStatic			        *m_pNEventNoticeStatic;
	CEdit					*m_pNEventNoticeEdit;

	

	
};

