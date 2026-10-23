#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Server/ServerEvent/ServerEvent-Quest.h"

class CStatic;


class CNServerEventQuestDlg : public CDialogController
{
	enum SEQD_VARIABLE
	{
		SEQD_VARIABLE_CHAR_WIDTH			= 44,
		SEQD_VARIABLE_CHAR_HEIGHT			= 44,

		SEQD_VARIABLE_CHAR_START_X			= 20,
		SEQD_VARIABLE_CHAR_START_Y			= 24,

		SEQD_VARIABLE_DIALOGMODE_MINIMUN	= 0,
		SEQD_VARIABLE_DIALOGMODE_MAXIMUN	= 1,
	};

	enum SEQD_DRAWSTATE
	{
		SEQD_DRAWSTATE_WAIT		= 0,
		SEQD_DRAWSTATE_ALL,
		SEQD_DRAWSTATE_CHARACTER,

	};
public:
	CNServerEventQuestDlg();
	~CNServerEventQuestDlg();

	void Create();

	void Show();
	void Hide();

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );

	static void CALLBACK StaticServerEventQuestDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NServerEventQuestDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

public:
	void	SetServerEventQuestStart( CNServerEvent_QuestData* pclSE_QuestData, SI16 siCharLimit, SI32 siNowAmount, bool* pbChar );
	void	SetServerEventQuestRunInfo( SI32 siLimiteMinute, SI32 siNowAmount );
	void	SetServerEventQuestGetChar( SI16 siCharIndex );

	TCHAR*	GetIndexToText( SI32 siIndex );

	

protected:
	void CalcWidth( void );
	void SetDialogMode( SI32 siDialogMode );



	void WriteQuestType( SI32 siEventType, SI32 siCondition1, SI32 siCondition2 );

protected:
	InterfaceFactory	m_InterfaceFactory;

	CStatic*			m_pStatic[ Const_ServerEvent_Max_QuestCharacter ];

	TSpr				m_sprBack;
	//
	SI16				m_siCharacterNum;
	SI32				m_siNeedAmount;

	bool				m_bCharacter[ Const_ServerEvent_Max_QuestCharacter ];
	SI32				m_siTxtMgrNumForCharacter[ Const_ServerEvent_Max_QuestCharacter ];
	SI32				m_siDrawCharIndex;

	//
	SI32				m_siDialogMode;

	SI16				m_siDialogWidht;
	SI16				m_siDialogHeight;
	SI16				m_siDialogX;
	SI16				m_siDialogY;

	SI16				m_siDrawState;
};
