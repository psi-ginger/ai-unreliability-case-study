//---------------------------------
// 2005/03/22 ¼ºÁØ¿±
//---------------------------------
#ifndef _NSELECTNOLIMITWARPTICKETDLG_H
#define _NSELECTNOLIMITWARPTICKETDLG_H

#include <Directives.h>

#include "InterfaceMgr/DialogController.h"
#include "Item/ItemCommon/ItemUnit.h"

class CButton;

class cltNSelectNoLimitWarpTicketDlg : public CDialogController
{
public:
	CButton		*m_pButton900;
	CButton		*m_pButton420;
	CButton		*m_pButton210;
	CButton		*m_pButton30;
	CButton		*m_pButton90;

	cltNSelectNoLimitWarpTicketDlg();
	~cltNSelectNoLimitWarpTicketDlg();

	static void CALLBACK StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Create();
	void SendUseTicketToServer(SI08 data);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

};

#endif //#if defined(__LEON_START_ALL) || defined(__LEON_NMASTER__) && defined(_DEBUG)







