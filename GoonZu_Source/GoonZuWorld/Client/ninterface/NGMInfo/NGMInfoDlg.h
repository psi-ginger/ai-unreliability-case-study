// [영훈] GM에게 관련된 정보 출력창
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"


class CEdit;


class CNGMInfoDlg : public CDialogController
{

//-- 함수 영역
public:
	CNGMInfoDlg();
	~CNGMInfoDlg();

	void Create();
	void Action();

	static void CALLBACK StaticNGMInfoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGMInfoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);


private:
	void WriteGMInfo();


//-- 변수 영역
private:
	CEdit	*m_pEdit; 

	bool	m_bAutoUnblock;
	bool	m_bGodMode;
	bool	m_bOneKill;			// 원킬모드

};
