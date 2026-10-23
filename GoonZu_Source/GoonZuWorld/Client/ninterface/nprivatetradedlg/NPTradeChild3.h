#pragma once

//¸» 
#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"


class CEachPersonTradeOrderInfo;
class CNPrivateTradeDlg;
class CLibListMgr;
//class CListView;
class CStatic;
class CButton;



class CNTradeChild3 : public CDialogController
{
	
public:
	CNTradeChild3();
	~CNTradeChild3();


	
	static void CALLBACK StaticNTradeChild3Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NTradeChild3Proc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void Create(CControllerMgr *pParent);
	void Show();
	void Hide();
	CNPrivateTradeDlg* m_pParent;

private:

	CButton  *m_pNPTrade3Button;
	CStatic  *m_pNPTrade3Static;

};