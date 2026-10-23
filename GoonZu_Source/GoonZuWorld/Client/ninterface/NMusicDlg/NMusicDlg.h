#pragma once

#include "../../InterfaceMgr/DialogController.h"

class CNMusicDlg : public CDialogController
{
public:

	enum
	{
		MAKE_MUSICWINDOW = 0,
		SHOW_MUSICWINDOW,
		HIDE_MUSICWINDOW,
	};
	CNMusicDlg();
	~CNMusicDlg();

	void Init();
	void Destroy();

	void Create();

	void Show();
	void Hide();
	void SetState( SI32 State );

	static void CALLBACK StaticNMusicDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMusicDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	SI32 m_siState;
};