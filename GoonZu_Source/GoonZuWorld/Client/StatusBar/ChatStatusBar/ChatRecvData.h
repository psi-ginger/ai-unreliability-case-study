#ifndef _CHATRECVDATA_H_
#define _CHATRECVDATA_H_

#include "ChatStatusBarBaseInfo.h"

class CChatRecvData
{
public:
	CChatRecvData();
	~CChatRecvData();

	void			Initialize();
	void			Destroy();
	
	CChatRecvData	*GetPre();
	void			SetPre( CChatRecvData	*pPre );

	CChatRecvData	*GetNext();
	void			SetNext( CChatRecvData	*pNext );

	void			SetChatData( TCHAR *strUserID, TCHAR *strChatData );

	TCHAR			*GetUserID();
	TCHAR			*GetChatData();


private:
	
	CChatRecvData	*m_pPre;
	CChatRecvData	*m_pNext;

	TCHAR			m_strUserID[ MAX_PLAYER_NAME ];
	TCHAR			m_strChatData[ MAX_CHAT_DATA_LENGTH ];

};

#endif
