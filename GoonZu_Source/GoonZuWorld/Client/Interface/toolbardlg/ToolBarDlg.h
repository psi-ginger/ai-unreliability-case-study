#ifndef _TOOLBARDLG_H_
#define _TOOLBARDLG_H_

#include "../../InterfaceMgr/DialogController.h"

#define MAX_TOOLBARDLG_BUTTON_NUM	7

class CButton;
class CStatic;

class CToolBarDlg : public CDialogController
{
public:
	CToolBarDlg();
	~CToolBarDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogToolBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogToolBar( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();
	void SetDate( TCHAR *strDate );
	void SetPosition( TCHAR *strDate );

public:
	CButton	*m_pToolBarButton[ MAX_TOOLBARDLG_BUTTON_NUM ];

	CButton *m_pQuitButton;

	CStatic	*m_pDateStatic;
	CStatic	*m_pMoneyStatic;
	CStatic	*m_pPositionStatic;
};

#endif