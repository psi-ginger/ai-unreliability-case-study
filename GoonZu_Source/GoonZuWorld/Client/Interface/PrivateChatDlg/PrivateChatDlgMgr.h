#ifndef _PRIVATECHATDLGMGR_H_
#define _PRIVATECHATDLGMGR_H_

#include "PrivateChatBaseInfo.h"

class CPrivateChatDlgMgr
{
public:
	CPrivateChatDlgMgr();
	~CPrivateChatDlgMgr();

	void Initialize();
	void Destroy();

	void Create( HWND hWnd, HINSTANCE hInst );
	
	SI32 Push( SI32 siCharUnique, char *strWhisperData );
	SI32 Push( SI32 siCharUnique, char *strUserName, char *strWhisperData ,SI32 level ,char * szAccountID = NULL);
	bool Pop( stWhisperChatDataSend *pWhisperChatDataSend );

	void ReadRejectUser();
	void WriteRejectUser();

	void AddRejectUser( char *strNickName );
	void DelRejectUser( char *strNickName );

	bool IsRejectUser( char *strNickName );
	
	char		m_strRejectUserList[ MAX_REJECT_USER_NUM ][ MAX_PLAYER_NAME ];		// 20 ¿∫ NickName ±Ê¿Ã

private:
	HINSTANCE	m_hInst;

	UI08		m_uiCurrentPrivateChatDlgNum;
	
	SI32		m_siRejectUserNum;

	HWND		m_hWhisperProfile[MAX_PRIVATE_CHAT_DLG_NUM];

};

#endif