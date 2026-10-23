#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
/**************************************************************************************/
//
//	[추가 : 황진성 2007. 12. 11 신년 이벤트 버튼창 클레스 추가.]
//
/**************************************************************************************/

class CNNewYearDlg : public CDialogController
{ 
public:
	CNNewYearDlg();
	~CNNewYearDlg();

	void Init();
	void Destroy();
	void Create();
	void Action();

	static void CALLBACK StaticNewYearDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NNewYearDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);


private:
	InterfaceFactory	m_InterfaceFactory;
};

