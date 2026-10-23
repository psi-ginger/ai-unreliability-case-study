// ÆÄÆ¼¸ÅÄª
#pragma once

#include "../../CommonLogic/PartyMatchingMgr/Client_PartyMatchingMgr.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../CommonLogic/PartyQuestMgr/PartyQuestMgr_Common.h"





class CNSelectQuest_PartyMatching : public CDialogController
{
public:
	CNSelectQuest_PartyMatching( void );
	~CNSelectQuest_PartyMatching( void );

private:
	void Init();

public:
	void Create();

	static void CALLBACK StaticCallBackDialogNSelectQuest_PartyMatching( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNSelectQuest_PartyMatching( UINT nEvent, int nControlID, CControllerObj* pControl );


private:
	InterfaceFactory m_InterfaceFactory;

	CButton* paButtonList[MAX_PARTYQUEST_CATEGORY_NUM];
	CImageStatic	*m_pImageStatic_portrait;
};