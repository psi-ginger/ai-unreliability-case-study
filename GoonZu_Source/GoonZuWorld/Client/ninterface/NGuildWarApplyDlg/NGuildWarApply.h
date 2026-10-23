#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "MsgType-Guild.h"

class CNGuildWarApplyDlg : public CDialogController
{
public:
	CNGuildWarApplyDlg();
	~CNGuildWarApplyDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticGuildWarApplyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGuildWarApplyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);
	void Action();

	InterfaceFactory	m_InterfaceFactory;

	void SetExplain( cltGameMsgResponse_GuildWar_ApplyToEnemy* pclinfo );

	clock_t m_ReceivedClock;
	//SI32	m_siHomeGuildUnique;
	//SI32	m_siFieldIndex;
	//SI32	m_siRoomIndex;

	cltGameMsgResponse_GuildWar_ApplyToEnemy m_ApplyInfo;

	void AcceptApplication();
	void DenyApplication();

	void SetLeftTime();
};