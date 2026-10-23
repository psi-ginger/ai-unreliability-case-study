// ÆÄÆ¼¸ÅÄª
#pragma once

#include "../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"
#include "../../InterfaceMgr/DialogController.h"


class CNApplytoParty_PartyMatching : public CDialogController
{
public:
	CNApplytoParty_PartyMatching( void );
	~CNApplytoParty_PartyMatching( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNApplytoParty_PartyMatching( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNApplytoParty_PartyMatching( UINT nEvent, int nControlID, CControllerObj* pControl );

private:
	InterfaceFactory m_InterfaceFactory;

public:
	CList* pPartyMatchingList;
	CList* pMemberList;
	CButton* pJoinRequest;
	CList* pStepInfoList;
	CEdit* pGreeting;

	SI32 siTempCategory;

	bool bPacketSended;
	DWORD dwPacketBlockTime;

	// interface
public:
	void AddListItem_PartyMatchingView(CInfo_PartyMatching* pInfo);

	void SetData_PartyMemberView(SI32& rPartyMemberList);
	void SetData_MissionView(SI32 siParentQuest, SI32 siChildQuest);
	void SetData_GreetingMessage(TCHAR* pszGreetingMessage);

	void Clear_PartyMatchingList();

	void Clear_Controls();

	// control event processor
public:
	void EventProcess_SelectItem_PartyMatchingList();
	void EventProcess_ClickButton_PartyJoin();
	void EventProcess_ClickButton_Whisper();
	void EventProcess_ClickButton_CreateNewPartyMatching();
};