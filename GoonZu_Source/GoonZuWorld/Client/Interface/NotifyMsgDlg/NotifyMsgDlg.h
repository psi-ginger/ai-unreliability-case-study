#ifndef _NOTIFYMSGDLG_H_
#define _NOTIFYMSGDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CStatic;
class CButton;
class CEdit;

class CNotifyMsgDlg : public CDialogController
{
public:
	CNotifyMsgDlg();
	~CNotifyMsgDlg();
	
	void Create();

	static void CALLBACK StaticCallBackDialogNotifyMsg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNotifyMsg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Hide();
	void Show();
	
	void SetText( TCHAR *Title, TCHAR *strPrintData, ... );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	CEdit *m_pTextStatic;

private:
	SI32	m_siOriginalWidth;
	SI32	m_siOriginalHeight;
};

#endif