#ifndef _NCHARMENUBARDLG_H_
#define _NCHARMENUBARDLG_H_

#include "../../InterfaceMgr/DialogController.h"

#define MAX_NCHARMENUBARDLG_BUTTON_NUM	5

class CButton;

class CNCharMenuBarDlg : public CDialogController
{
public:
	CNCharMenuBarDlg();
	~CNCharMenuBarDlg();

	void Create();
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	
	static void CALLBACK StaticCallBackDialogBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogBar( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:

	CButton	*m_pToolBarButton[ MAX_NCHARMENUBARDLG_BUTTON_NUM ];

};

#endif	//_NCHARMENUBARDLG_H_