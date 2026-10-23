#ifndef _PRIVATECHATBASEINFO_H_
#define _PRIVATECHATBASEINFO_H_

#include <Directives.h>
#include <windows.h>
#include "..\..\..\Resource.h"

#include "..\..\StatusBar\ChatStatusBar\ChatStatusBarBaseInfo.h"

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
	char				strChatData[ MAX_CHAT_DATA_LENGTH ];

	stWhisperChatDataSend()
	{
		sHeader.uiCommand = WHISPER_CHAT_DATA_SEND;
		sHeader.uiDataSize = sizeof( stWhisperChatDataSend );
		memset( strChatData, 0, MAX_CHAT_DATA_LENGTH );
	}
};

struct stWhisperChatDataRecv
{
	stPrivateChatHeader	sHeader;
	SI32				siFromID;
	char				strChatData[ MAX_CHAT_DATA_LENGTH ];

	stWhisperChatDataRecv()
	{
		sHeader.uiCommand = WHISPER_CHAT_DATA_RECV;
		sHeader.uiDataSize = sizeof( stWhisperChatDataRecv );
		memset( strChatData, 0, MAX_CHAT_DATA_LENGTH );
	}
};

class	CPrivateChatDlg;
extern	CPrivateChatDlg	g_PrivateChatDlg[ MAX_PRIVATE_CHAT_DLG_NUM ];

class CPrivateChatRejectViewDlg;
extern	CPrivateChatRejectViewDlg g_PrivateChatRejectViewDlg;
#endif
