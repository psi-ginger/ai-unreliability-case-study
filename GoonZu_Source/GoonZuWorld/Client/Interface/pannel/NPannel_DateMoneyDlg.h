// [¿µÈÆ] ³»ºÎ UI : Pannel (³¯Â¥, µ· Ç¥½Ã)
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"


class CNPannel_DateMoneyDlg : public CDialogController
{

public:
	CNPannel_DateMoneyDlg();
	~CNPannel_DateMoneyDlg();


	void Create();

	static void CALLBACK StaticPannel_DateMoneyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPannel_DateMoneyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Show();
	void Hide();

	void SetDateMoney( TCHAR* pszDate, TCHAR* pszMoney );


private:
	InterfaceFactory	m_InterfaceFactory;

};
