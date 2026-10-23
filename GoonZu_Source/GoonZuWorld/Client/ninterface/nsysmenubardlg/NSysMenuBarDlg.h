//#ifndef _NSYSMENUBARDLG_H_
//#define _NSYSMENUBARDLG_H_
//
//#include "../../InterfaceMgr/DialogController.h"
//
//#define MAX_NSYSMENUBARDLG_BUTTON_NUM	4
//
//class CButton;
//
//class CNSysMenuBarDlg : public CDialogController
//{
//public:
//	CNSysMenuBarDlg();
//	~CNSysMenuBarDlg();
//
//	void Create();
//	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
//
//	static void CALLBACK StaticCallBackDialogBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
//	void CALLBACK CallBackDialogBar( UINT nEvent, int nControlID, CControllerObj* pControl );
//
//	void Action();
//	
//	SI16 GetChildXPos(SI16 childWidth );
//	SI16 GetChildYPos(SI16 childHeight );
//
//	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
//
//
//public:
//
//	CButton	*m_pToolBarButton[ MAX_NSYSMENUBARDLG_BUTTON_NUM ];
//	
//	SI32	m_siNowOpendMenu;
//
//};
//
//#endif	//_NSYSMENUBARDLG_H_