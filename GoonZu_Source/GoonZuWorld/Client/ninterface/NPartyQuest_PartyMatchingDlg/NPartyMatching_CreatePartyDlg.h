// ÆÄÆ¼¸ÅÄª
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../CommonLogic/PartyMatchingMgr/Info_PartyMatching.h"


class CNCreatePartyDlg : public CDialogController
{

	// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;

public:
	bool bPacketSended;

	SI32 siTempCategory;
	SI32 siTempMission;

public:
	CNCreatePartyDlg( void );
	~CNCreatePartyDlg( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNCreatePartyDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNCreatePartyDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	// interface
public:
	bool Get_Info_PartyMatchingCreate(CInfo_PartyMatching& rInfo);

	// event process
public:
	void EventProcess_Select_QuestList();
};