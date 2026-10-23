#ifndef _NPRIVATECHATREJECTDLG_H_
#define _NPRIVATECHATREJECTDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CStatic;
class CList;

struct stWhisperChatDataSend;

class CNPrivateChatRejectDlg : public CDialogController
{
public:
	CNPrivateChatRejectDlg();
	~CNPrivateChatRejectDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNPrivateChatReject( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPrivateChatReject( UINT nEvent, int nControlID, CControllerObj* pControl );

	void LoadRejectList();
private:
	CStatic		*m_pNameStatic;
	CButton		*m_pRejectCancelButton;
	CList		*m_pList;
};

#endif
