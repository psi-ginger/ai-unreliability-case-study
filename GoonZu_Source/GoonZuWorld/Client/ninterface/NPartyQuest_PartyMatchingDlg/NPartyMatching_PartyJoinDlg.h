
#pragma once

#include "../../InterfaceMgr/DialogController.h"




/*
,PARTY_INVITE_DLG_BUTTON_ACCEPT
,PARTY_INVITE_DLG_BUTTON_CANCEL
,PARTY_INVITE_DLG_EDITBOX_PARTY_INVITE
*/

class CNPartyMatching_PartyJoinDlg : public CDialogController
{

	// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;
protected:

public:


	// -- Member Function Area
private:

protected:
	SI32 m_siLeaderUnique;
	SI32 m_siMemberUnique;
	SI32 m_siPartyIndex;

public:
	CNPartyMatching_PartyJoinDlg( void );
	~CNPartyMatching_PartyJoinDlg( void );

	void Init();
	void Create();

	void Show();
	void Hide();

	void SetText(TCHAR* pszMessage);

	void SetJoinRequestInfo(SI32 siLeaderUnique, SI32 siMemberUnique, SI32 siPartyIndex);

	static void CALLBACK StaticCallBackDialogNPartyMatching_PartyJoinDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPartyMatching_PartyJoinDlg( UINT nEvent, int nControlID, CControllerObj* pControl );
};