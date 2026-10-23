#ifndef _NPRIVATECHATDLG_H_
#define _NPRIVATECHATDLG_H_

#include "../../InterfaceMgr/DialogController.h"

class CButton;
class CComboBox;
class CStatic;
//class CImageStatic;
class CEdit;
class CChatEdit;

struct stWhisperChatDataSend;

class CNPrivateChatDlg : public CDialogController
{

	 friend class CNPrivateChatDlgMgr;

public:
	CNPrivateChatDlg();
	~CNPrivateChatDlg();

	void Create( SI16 PrivateChatDlgID );

	static void CALLBACK StaticCallBackDialogNPrivateChat( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNPrivateChat( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();
	void RecvChat( TCHAR *PlayerName, TCHAR *ChatText, BOOL bGameMaster );
	void SendChat();

	void Set( SI32 siCharUnique, TCHAR *strUserName ,SI32 level);
	bool IsExist( SI32 siCharUnique, TCHAR *strUserName );
	void Reject();
	
	void Minimi(BOOL bMinimize);
	BOOL isMini(){	return m_bMini;	};
//	void SetMsgBox(stMsgBox* Msgbox);
	void RetMsgBox(bool RetValue,SI16 MsgIndex, BYTE *pData,SI16 DataLen);

	void SaveChatLog(TCHAR* SenderName, TCHAR* RecieverName, TCHAR* ChatText);
	void ReturnFalseSend();
	void SetHelper( bool _Helper );													//[진성] 도우미. 2008-10-13

	bool IsAccept() 
	{
		return m_bAccept;
	}

private:
	CChatEdit	*m_pChatEdit;
	CEdit		*m_pInputEdit;
	
	CButton		*m_pSendChatButton;
	CButton		*m_pRejectButton;
	CButton		*m_pRejectListButton;

	CButton		*m_pMinimizeButton;
	BOOL		m_bMini;

	CStatic		*m_pImageStatic;
	//CImageStatic		*m_pImageMinimized;
	CButton		*m_pImageMinimizedButton;

	CCheckBox	*m_pSaveLogCheckbox;

	
	SI16		m_siOldPosX;
	SI16		m_siOldPosY;
	SI32		m_siOldXSize;
	SI32		m_siOldYSize;

	SI32		m_siCharUnique;
	TCHAR		*m_strUserName;
	TCHAR		m_strTitle[256];

	bool		m_bAccept;
};

#endif
