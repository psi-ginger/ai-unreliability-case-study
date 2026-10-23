
#pragma once

#include "../../../InterfaceMgr/DialogController.h"

#include "Guild/Guild.h"

#define MAX_GUILDMARK_NUMBER			107

class CNMyGuildMasterMark : public CDialogController
{
public:

	CNMyGuildMasterMark();
	~CNMyGuildMasterMark ();

	void	Initialize();
	void	Destroy();

	void	Create( CControllerMgr *pParent );

	static void CALLBACK StaticMyGuildMasterMarkDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildMasterMarkDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	InterfaceFactory m_InterfaceFactory;

	void RefreshWindow();

	virtual void DrawDialogFirst( SI32 ScreenRX, SI32 SreenRY );

	SI32	m_siNewMarkIndex;

};

