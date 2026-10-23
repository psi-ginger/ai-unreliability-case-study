#pragma once

#include "../../InterfaceMgr/DialogController.h"
//#include "..\CommonLogic\PostOffice\PostOffice.h"

class COutLine;
class CButton;
class CStatic;
class CList;
class CEdit;
class CChatEdit;

// 주주 총회 참여 여부
class NGeneralMeeting2Dlg : public CDialogController
{
public:

	CStatic*			m_pStatic;
	CStatic*			m_pStatic1;
	CStatic*			m_pStatic2;
	CStatic*			m_pStatic3;

	CButton*			m_pButtonAgree;
	CButton*			m_pButtonOpposit;
	CButton*			m_pButtonRefresh;

	CList*				m_pListAgree;
	CList*				m_pListOpposit;

	CEdit*				m_pEditAgree;
	CEdit*				m_pEditOpposit;

	CChatEdit*				m_pEditChat;
	CEdit*				m_pEditChatInput;

	COutLine*			m_pOutLine;

	NGeneralMeeting2Dlg();
	~NGeneralMeeting2Dlg();

	void Create();

	void InitVoteList();
	void ShowVoterList();

	void InitInfo();
	void AddInfo( int villageunique );
	void SetText();

	void Action();

	void SetObserveMode(bool bObserve);


	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	static void CALLBACK StaticCallBackDialogNGeneralMeeting2Dlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNGeneralMeeting2Dlg( UINT nEvent, int nControlID, CControllerObj* pControl );


};