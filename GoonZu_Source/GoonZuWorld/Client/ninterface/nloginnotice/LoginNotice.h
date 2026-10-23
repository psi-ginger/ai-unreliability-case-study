/*#ifndef _NLOGINNOTICE_H_
#define _NLOGINNOTICE_H_

#include "../../InterfaceMgr/DialogController.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;

class CNLoginNoticeDlg : public CDialogController
{
public:

	CNLoginNoticeDlg();
	~CNLoginNoticeDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNLoginNoticeDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNLoginNoticeDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();

	void Action();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );

private:

};

#endif*/