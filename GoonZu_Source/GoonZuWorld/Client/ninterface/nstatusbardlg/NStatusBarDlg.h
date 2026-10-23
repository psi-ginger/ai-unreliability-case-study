#ifndef _NSTATUSBARDLG_H_
#define _NSTATUSBARDLG_H_

#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


#define HITBUTTON_RECOMMEND		1
#define HITBUTTON_SUMMON		2
#define HITBUTTON_RAREITEM		3

class CStatic;
class CButton;
class CEdit;
class CComboBox;
class CChatEdit;

struct stChatStatusBarData;
class CChatStatusBarBuffer;


#define MAX_BROADCASTINGMUSICBUTTON_NUM		3

struct stNStatusBarDlgData
{
	TCHAR *strText;
	SI16 siParam;

	void Set( TCHAR *Text, SI16 Param )
	{
		strText = Text;
		siParam = Param;
	}
};

//-----------------------------------------------------------------------------------
// CNStatusBarPopupParentDlg
//-----------------------------------------------------------------------------------
class CNStatusBarPopupParentDlg : public CDialogController
{
protected:
	CButton*		m_pclButton_Skill;
	CButton*		m_pclButton_Magic;
	CButton*		m_pclButton_Quest;
	CButton*		m_pclButton_Community;
    CButton*		m_pclButton_System;
	CButton*		m_pclButton_HomePage;

	CButton*		m_pUrlLink;

public:
	CNStatusBarPopupParentDlg();
	~CNStatusBarPopupParentDlg();

	void Create();

	void ShowStatusBarPopupParentDialog(bool bShow);

	static void CALLBACK	StatiCallBackDialogNStatusBarPopupParentProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			CallBackDialogNStatusBarPopupParentProc(UINT nEvent, int nControlID, CControllerObj* pControl);
};

class cltNPopupButtonInfo
{
public:
	SI32			m_siControlID;
	NTCHARString32	m_kText;

public:
	cltNPopupButtonInfo(SI32 siControlID, TCHAR* pszText)
	{
		m_siControlID	= siControlID;
		m_kText			= pszText;
	}

	~cltNPopupButtonInfo()
	{
	}
};

#define NSTATUSBAR_POPUPCHILD_MAXBUTTON		10

//-----------------------------------------------------------------------------------
// CNStatusBarPopupChildCommunityDlg
//-----------------------------------------------------------------------------------
class CNStatusBarPopupChildCommunityDlg : public CDialogController
{
public:
	CNStatusBarPopupChildCommunityDlg();
	~CNStatusBarPopupChildCommunityDlg();

	void Create();

	static void CALLBACK	StatiCallBackDialogNStatusBarPopupChildCommunityProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			CallBackDialogNStatusBarPopupChildCommunityProc(UINT nEvent, int nControlID, CControllerObj* pControl);

protected:
	NSafeTArray<cltNPopupButtonInfo*, NSTATUSBAR_POPUPCHILD_MAXBUTTON> 		m_pclButtons;
	NSafeTArray<CButton*, NSTATUSBAR_POPUPCHILD_MAXBUTTON>					m_pclButton_Buttons;
};

//-----------------------------------------------------------------------------------
// CNStatusBarPopupChildSystemDlg
//-----------------------------------------------------------------------------------
class CNStatusBarPopupChildSystemDlg : public CDialogController
{
public:
	CNStatusBarPopupChildSystemDlg();
	~CNStatusBarPopupChildSystemDlg();

	void Create();

	static void CALLBACK	StatiCallBackDialogNStatusBarPopupChildSystemProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			CallBackDialogNStatusBarPopupChildSystemProc(UINT nEvent, int nControlID, CControllerObj* pControl);

protected:
	cltNPopupButtonInfo*	m_pclButtons[NSTATUSBAR_POPUPCHILD_MAXBUTTON];
	CButton*				m_pclButton_Buttons[NSTATUSBAR_POPUPCHILD_MAXBUTTON];
};

class CNStatusBarDlg : public CDialogController
{
public:
	CNStatusBarDlg();
	~CNStatusBarDlg();

	void Create();

	static void CALLBACK	StatiCallBackDialogNStatusBarProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			CallBackDialogNStatusBarProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void SetData( void *pBuffer );

	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	//---------------------------------------------------------------
	// 채팅 관련
	//---------------------------------------------------------------
	void CharacterLogin();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	void ChatDataSend();
	void Push( SI16 ChatDataKind, TCHAR *ChatText );
	bool Pop( stChatStatusBarData * pChatStatusBarData );
	void SetInputEditBoxFocus();

	void SmallShow();
	void BigShow();

	bool CheckData( TCHAR *PlayerName, TCHAR *ChatText);

	void ChatRecv( TCHAR *PlayerName, TCHAR *ChatText, SI32 Kind = 0 );
	void MasterNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void VillageNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void ResidentsNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void KingNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );
	void GoodsProductionNotifyChatRecv( TCHAR *PlayerName, TCHAR *ChatText );

	void SetOpenedChatDlg( bool Open );
	void ChatMessage(SI16 chatmode, SI32 systemtxt, TCHAR* pmsg);       //client 에 있던 메세지 박스 처리를 위해서 

	SI32 CompareKey(TCHAR *data, SI32 type);
	
	void SetVersionAndServerName(TCHAR * VersionAndServerName);

private:
	TCHAR		m_szVersionAndServerName[256];
    
//	CButton		*m_pButton[7];

//	CButton		*m_pWinampButton;

	// 채팅쪽
//	CChatEdit	*m_pChatEdit;
//	CEdit		*m_pInputEdit;
	//CComboBox	*m_pComboBox;
//	CButton		*m_pOpenButton;

	CButton*	m_pButton_Menu;
	CStatic*	m_pStatic_ServerName;

	TCHAR		*m_strChatData;
	CChatStatusBarBuffer	*m_pChatStatusBarBuffer;

	bool		m_bOpened;

	CButton*	m_pUrlLink;
};


#endif