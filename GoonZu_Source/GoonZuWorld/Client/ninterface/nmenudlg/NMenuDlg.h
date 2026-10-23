#ifndef _NMENUDLG_H_
#define _NMENUDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CList;

class CNMenuDlg : public CDialogController
{
public:
	CNMenuDlg();
	~CNMenuDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNMenu( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNMenu( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Set( SI16 Index );

	void Execute( SI16 Param );

	void ClearFocus();
	void SetFocus( bool Focus );

	void SetPosion(SI32 id, SI32 mode = 0);	// id는 다이얼로그 id , mode는 0이면 1024화면에서 왼쪽 1이면 오른쪽

private:
	CList	*m_pList;
	SI16	m_siSelectedMenuIndex;

};

#endif