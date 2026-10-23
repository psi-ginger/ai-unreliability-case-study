#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNEmoticonDlg : public CDialogController
{
public:
	CNEmoticonDlg();
	~CNEmoticonDlg();

	void Initialize();
	void Destroy();

	void Create();
	void Action();

	static void CALLBACK StaticEmoticonDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NEmoticonDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	InterfaceFactory m_InterfaceFactory;

private:
	SI16		m_siEmoticonCount;
	SI16		m_siUnderMouseIndex;
};