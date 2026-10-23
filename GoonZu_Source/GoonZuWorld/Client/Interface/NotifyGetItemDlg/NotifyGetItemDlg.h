#ifndef _NOTIFYGETITEMDLG_H_
#define _NOTIFYGETITEMDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class cltItem;
class CStatic;

class CNotifyGetItemDlg : public CDialogController
{
public:
	CNotifyGetItemDlg();
	~CNotifyGetItemDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNotifyGetItem( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNotifyGetItem( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Hide();
	void Show( cltItem *pclItem, GMONEY pricevalue = 0 );
	
	void SetText( TCHAR *Title, TCHAR *strPrintData, ... );
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );

	void Action();

private:
	cltItem	*m_pItem;
	CStatic *m_pTextStatic;	

	SI16	m_siCurrentFrame;
};

#endif