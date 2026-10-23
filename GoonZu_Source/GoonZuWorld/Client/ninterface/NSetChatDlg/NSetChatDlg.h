#ifndef _NSETCHATDLG_H_
#define _NSETCHATDLG_H_

#include "../../InterfaceMgr/DialogController.h"
#include "../../ninterface/nchatbardlg/NChatBarDlg.h"


class CNChatBarDlg ;
class CNSetChatDlg : public CDialogController
{
public:
	CNSetChatDlg();
	~CNSetChatDlg();

	void Create();
	void DelDialog();
	static void CALLBACK StaticCallBackDialogNChat( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNChat( UINT nEvent, int nControlID, CControllerObj* pControl );

	void CheckBoxChange( SI32 siObjectID );

private:
	InterfaceFactory	m_InterfaceFactory;


};
#endif
