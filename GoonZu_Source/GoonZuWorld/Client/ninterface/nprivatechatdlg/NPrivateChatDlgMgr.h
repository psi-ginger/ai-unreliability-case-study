#ifndef _NPRIVATECHATDLGMGR_H_
#define _NPRIVATECHATDLGMGR_H_

//#include "../../Interface/PrivateChatDlg/PrivateChatBaseInfo.h"
#include "../../InterfaceMgr/NMsgBoxDlg.h"
#include "..\..\StatusBar\ChatStatusBar\ChatStatusBarBaseInfo.h"

#include <deque>
using namespace std;

#define MAX_PRIVATE_CHAT_DLG_NUM		10
#define MAX_PRIVATE_CHAT_DLG_TIMER		1000

#define MAX_REJECT_USER_NUM				100

enum { PRIVATE_CHAT_FAIL = -1, PRIVATE_CHAT_EXCESS_CHAT_DLG_NUM, PRIVATE_CHAT_SUCCESS };

struct stPrivateChatHeader
{
	UI16 uiCommand;
	UI16 uiDataSize;
};

struct stWhisperChatDataSend
{
	stPrivateChatHeader	sHeader;
	SI32				siToID;
	TCHAR				szToCharName[ MAX_PLAYER_NAME ];
	TCHAR				strChatData[ MAX_CHAT_DATA_LENGTH ];

	stWhisperChatDataSend()
	{
		sHeader.uiCommand = WHISPER_CHAT_DATA_SEND;
		sHeader.uiDataSize = sizeof( stWhisperChatDataSend );
		memset( szToCharName, 0, MAX_PLAYER_NAME );
		memset( strChatData, 0, MAX_CHAT_DATA_LENGTH );
	}
};

class CNPrivateChatDlgMgr
{
public:
	CNPrivateChatDlgMgr();
	~CNPrivateChatDlgMgr();

	void Initialize();
	void Destroy();
	
	void SetMinimizeAllDialog(BOOL bMinimize);
	BOOL IsMinimi();

	SI32 Push( BOOL bGameMaster, SI32 siCharUnique, TCHAR *strWhisperData );
	SI32 Push( BOOL bGameMaster, SI32 siCharUnique, TCHAR *strUserName, TCHAR *strWhisperData ,SI32 level ,TCHAR * szAccountID = NULL);
	bool Pop( stWhisperChatDataSend *pWhisperChatDataSend );

	void AddRejectUser( TCHAR *strNickName );
	void DelRejectUser( TCHAR *strNickName );

	bool IsRejectUser( TCHAR *strNickName );

	void SendChat( stWhisperChatDataSend *pWhisperChatDataSend, bool bAutoAnswer = false );
	
	bool SetAcceptRefuseMsg(SI32 siCharUnique,TCHAR* strUserName,TCHAR* buffer); // 귓말 수락 거부
	
	SI16 GetMinimizedSlot(){return m_siMinimizedSlot;};
	void SetMinimizedSlot(SI16 MinimizedSlot){  m_siMinimizedSlot = MinimizedSlot; };
	
	void FaildPush(SI32 CharUnique);

	TCHAR* GetAutoAnswerMsg();
	void SetAutoAnswerMsg( TCHAR* msg );
	bool GetAutoAnswerSwitch();
	void SetAutoAnswerSwitch( bool autoanswer );


	TCHAR m_strRejectUserList[ MAX_REJECT_USER_NUM ][ MAX_PLAYER_NAME ];		// 20 은 NickName 길이

	bool		m_bNewMessage;	// 새 메시지가 왔을 때 Minimize 된 창을 깜박이게 하기위한 플래그

private:
	UI08		m_uiCurrentPrivateChatDlgNum;
	SI32		m_siRejectUserNum;
	SI16		m_siMinimizedSlot;

	TCHAR		m_szAutoAnswerMsg[MAX_CHAT_DATA_LENGTH];
	bool		m_bAutoAnswerSwitch;
	
	//	stMsgBox	MsgBox;
	deque< stWhisperChatDataSend >	m_ChatDataDeque;
};

#endif