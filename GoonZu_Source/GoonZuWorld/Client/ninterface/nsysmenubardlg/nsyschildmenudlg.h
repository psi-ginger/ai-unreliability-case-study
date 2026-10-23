#ifndef _NSYSCHILDMENUBARDLG_H_
#define _NSYSCHILDMENUBARDLG_H_

#include "../../InterfaceMgr/DialogController.h"

#define MAX_NSYSCHILDMENUBARDLG_BUTTON_NUM	 10
#define NSYSCHILD_BUTTON_WIDTH	64
#define NSYSCHILD_BUTTON_HEIGHT	22

enum
{
	CHILDMENU_ID_INFO =0,
	CHILDMENU_ID_COMMUNITY,
    CHILDMENU_ID_WEB,
	CHILDMENU_ID_OPTION
};
class CButton;

class CNSysChildMenuDlg : public CDialogController
{
public:
	CNSysChildMenuDlg();
	~CNSysChildMenuDlg();

	void Create();


	static void CALLBACK StaticCallBackDialogBar( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogBar( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();
	
	void ClearFocus();
	void SetFocus( bool Focus );


	void Set(SI16 SelectedChildMenu);

	void DeleteDialog();

public:

	CButton	*m_pChildMenuButton[ MAX_NSYSCHILDMENUBARDLG_BUTTON_NUM ];
	SI16 m_siSelectedChildMenuId;	
};

#endif	//_NSYSMENUBARDLG_H_