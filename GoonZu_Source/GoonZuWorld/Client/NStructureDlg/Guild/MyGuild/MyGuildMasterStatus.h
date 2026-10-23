
#pragma once

#include "../../../InterfaceMgr/DialogController.h"

#include "Guild/Guild.h"

class CNMyGuildStatusStatus : public CDialogController
{
public:

	CNMyGuildStatusStatus();
	~CNMyGuildStatusStatus ();

	void	Initialize();
	void	Destroy();

	void	Create( CControllerMgr *pParent );

	static void CALLBACK StaticMyGuildMasterStatusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NMyGuildMasterStatusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	InterfaceFactory m_InterfaceFactory;

	cltGuildStatus	clStatus;
	cltGuildStatus	clChangedStatus;				// 스테이터스 분배를 위한 임시 저장 장소

	void	LoadStatus();
	void	RefreshStatus();

	void	ChangeStatus( SI32 controlID, SI16 index );

	void	SendStatusToServer();

	enum{
		CHANGESTATUS_PLUS = 0,
		CHANGESTATUS_MINUS,
	};


private:



};

