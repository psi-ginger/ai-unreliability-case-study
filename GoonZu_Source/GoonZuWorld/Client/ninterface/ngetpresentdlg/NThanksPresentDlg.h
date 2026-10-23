
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/CommonHeader.h"

struct stAnswerData
{
	SI32 siFromServerUnique;
	TCHAR szFromPlayerName[MAX_PLAYER_NAME];

	TCHAR szComment[200];

	stAnswerData( SI32 FromServerUnique, TCHAR * FromPlayerName, TCHAR * Comment )
	{
		siFromServerUnique = FromServerUnique;

		StringCchCopy(szFromPlayerName,MAX_PLAYER_NAME,FromPlayerName);
		StringCchCopy(szComment,200,Comment);
	}

	stAnswerData()
	{
		Init();
	}

	void Init()
	{
		memset( this, 0, sizeof( stAnswerData ) );
	}
};

class CNThanksPresentDlg  : public CDialogController
{
public:
	CNThanksPresentDlg();
	~CNThanksPresentDlg();

	static void CALLBACK	StatiCNThanksPresentDlg(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NThanksPresentDlgdProc(UINT nEvent, int nControlID, CControllerObj* pControl );
	void Create();	


	void PreButtonClicked();
	void NextButtonClicked();
	void SetCurAnswerData();

	void SetRecvLetterData( SI32 FromServerUnique, TCHAR * FromPlayerName, TCHAR * Comment );
	void SetSendLetterData( SI32 ToServerUnique, TCHAR * ToPlayerName, TCHAR * ToPlayerAccount);
	
	void Action();

	private:

	InterfaceFactory m_InterfaceFactory;

	SI32	m_siServerUnique;
	TCHAR	m_szPlayerName[MAX_PLAYER_NAME];
	TCHAR	m_szPlayerAccount[MAX_PLAYER_NAME];
	
	BOOL	m_bIsForMsgSend;

	SI32	m_siLetterNum;
	SI32	m_siCurLetter;

	list< stAnswerData > m_AnswerList;
	list< stAnswerData >::iterator m_AnswerListItr;

};