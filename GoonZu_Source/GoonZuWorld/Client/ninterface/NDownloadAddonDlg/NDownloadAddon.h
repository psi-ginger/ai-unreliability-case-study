#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../CommonLogic/TextMgr/TextMgr.h"



class CNDownloadAddonDlg : public CDialogController
{
public:
	CNDownloadAddonDlg();
	~CNDownloadAddonDlg();

	void Create();

	static void CALLBACK StaticCallBackDialogNDownloadAddon( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK CallBackDialogNDownloadAddon( UINT nEvent, int nControlID, CControllerObj* pControl );

	CEdit*		m_pEditExplian;
	CButton*	m_pButtonDownload;

private:

};