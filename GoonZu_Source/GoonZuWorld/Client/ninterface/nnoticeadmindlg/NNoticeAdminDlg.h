//---------------------------------
// 2005/03/22 ¼ºÁØ¿±
//---------------------------------
#ifndef _NNOTICEADMINDLG_H
#define _NNOTICEADMINDLG_H

#include <Directives.h>

#include "InterfaceMgr/DialogController.h"

class CEdit;
class CImageStatic;

class cltNNoticeAdminDlg : public CDialogController
{
public:
	CImageStatic*		m_pSpr;
	CEdit*				m_pEdit;

	cltNNoticeAdminDlg();
	~cltNNoticeAdminDlg();

	static void CALLBACK StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();

	void SetData(void *pBuffer);
};

#endif //#if defined(__LEON_START_ALL) || defined(__LEON_NMASTER__) && defined(_DEBUG)







