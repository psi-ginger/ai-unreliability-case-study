#ifndef _CHATSTATUSBARBASEINFO_H_
#define _CHATSTATUSBARBASEINFO_H_

#include < Directives.h >

#include "..\..\..\Resource.h"
#include "../../../Common/CommonHeader.h"


#define		MAX_CHAT_DATA_RECORD_NUM					150		// 20개 까지의 대화 내용을 기억하고 있음.
#define		MAX_CHAT_DATA_BUFFER_NUM					200		// 최대 50개를 버퍼에 담아 둘 수 있음.



#define		WM_CHAT_DATA_INPUT							WM_USER+500

#define		CHAT_DATA_SEND								26500
#define		CHAT_DATA_RECV								26501

#define		WHISPER_CHAT_DATA_SEND						26511
#define		WHISPER_CHAT_DATA_RECV						26512

#define		WHISPER_CHAT_REJECT							26520

#define		TOWN_CHAT_DATA_SEND							26530		// 마을 공지

#define		RESIDENTSNOTIFY_CHAT_DATA_SEND				26531		// 주민 공지

#define		HEAD_CHAT_DATA_SEND							26540		// 광고 채팅

#define		RESIDENTS_CHAT_DATA_SEND					26550		// 주민 채팅

#define		PARTY_CHAT_DATA_SEND						26560		// 파티 채팅

//#define		ASK_CHAT_DATA_SEND							26570		// 초보자가 물어볼때 사용하는...

#define		KINGNOTIFY_CHAT_DATA_SEND					26580		// 왕(군주)이 공지 할때

#define		NEWS_CHAT_DATA_SEND							26590		// 자막창으로 광고할 때

// _LEON_GUILD_CHAT
#define		GUILD_CHAT_DATA_SEND						26600		// 길드 멤버 채팅
//#define		GUILDNOTIFY_CHAT_DATA_SEND					26601		// 길드 멤버 공지


struct stChatStatusBarData
{
	SI16	si16Msg;
	TCHAR	strChatData[ MAX_CHAT_DATA_LENGTH ];
};

class CChatStatusBar;
//extern	CChatStatusBar	g_ChatStatusBar;

#endif