/* ==========================================================================
클래스 :		 

작성일 :		 
작성자 :		 

변경사항 :		 

차후작업 :		
========================================================================*/
 
#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-System.h"

class NValentineDayConfessionAnswerDlg : public CDialogController
{
public:

	NValentineDayConfessionAnswerDlg();
	~NValentineDayConfessionAnswerDlg();

	void Create();
	void Destroy();
	void Action();

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	void SetQuestion( SI32 _siPersonID, TCHAR* _pszName, TCHAR* _pszMsg );

	static void CALLBACK StaticNValentineDayConfessionAnswerDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NValentineDayConfessionAnswerDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
private:
	InterfaceFactory	m_InterfaceFactory;

	SI32			m_siPersonID;							// 고백한 사람의 personID
};
 