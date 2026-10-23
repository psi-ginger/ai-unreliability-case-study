#pragma once

#include "../../InterfaceMgr/DialogController.h"

class cltGameMsgResponse_Guild_MemeberList;

class CNGuildMemberDlg : public CDialogController
{
public:

	CNGuildMemberDlg();
	~CNGuildMemberDlg();

	void	Initialize();
	void	Destroy();

	void	Create();

	static void CALLBACK StaticNGuildMemberDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NGuildMemberDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	Hide();
	void	Show();

	void	Action();

	void	RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	InterfaceFactory m_InterfaceFactory;

	void	SetMemberList( cltGameMsgResponse_Guild_MemeberList* pclinfo );
	SI16	GetSelectedListIndex();
	void	WarpToGuildMember();

	SI32	m_siMemberPersonID[MAX_GUILD_MEMBER_NUM];

	bool	m_bConnected[MAX_GUILD_MEMBER_NUM];

	clock_t	m_RefreshClock;
};