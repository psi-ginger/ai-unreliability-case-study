#pragma once


#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


class CEachPersonTradeOrderInfo;
class CLibListMgr;
class CList;
class CStatic;
class CButton;
class COutLine;
class CEdit;

class cltItem;
class cltRentContract;
class cltHorse;


class CNPrivateTradeAcceptDlg :public CDialogController	// 거래 요청 메시지 박스의 이벤트 헨들러역할
{
public:
	CNPrivateTradeAcceptDlg();
	~CNPrivateTradeAcceptDlg();

	void Create();
//	void Set(SI32 CommonDlg_mode);
	void Action();
//void LogOut();

	static void CALLBACK StaticNPrivateTradeAcceptDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK NPrivateTradeAcceptDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	void RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE* pData, SI16 DataLen);

private:
	SI32 siCommonDlg_mode;
	SI32 siExitCount;

	CEdit	*m_pEdit;
	CButton	*m_pOKButton;
	CButton	*m_pCancelButton;

};

