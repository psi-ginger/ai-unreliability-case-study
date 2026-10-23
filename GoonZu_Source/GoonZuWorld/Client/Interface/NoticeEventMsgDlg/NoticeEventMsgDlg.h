#ifndef _NOTICEEVENTMSGDLG_H_
#define _NOTICEEVENTMSGDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CNoticeEvnetMsgDlg : public CDialogController
{
public:
	CNoticeEvnetMsgDlg ();
	~CNoticeEvnetMsgDlg ();

	void Create();

	static void CALLBACK StaticCallBackDialogNoticeEventMsg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNoticeEventMsg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Hide();
	void Show();

	void SetGlobalImage( TCHAR *Title, SI32 GlobalImageIndex );
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	
private:
	SI32	m_siGlobalImageIndex;
};

#endif