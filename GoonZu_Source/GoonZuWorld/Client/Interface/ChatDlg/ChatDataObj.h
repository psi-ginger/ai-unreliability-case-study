#ifndef _CHATDATAOBJ_H_
#define _CHATDATAOBJ_H_

#include "ChatDlgBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"

class CChatDataObj : public CObj
{
public:
	CChatDataObj();
	~CChatDataObj();

	void	Initialize();
	void	Destroy();

	stChatStatusBarData	*GetstChatStatusBarData();
	void	SetChatData( TCHAR *strUserID, TCHAR *strChatData );
	void	SetChatData( SI16 siMsg, TCHAR *strChatData );

	void	SetReject( SI16 siMsg, SI32 siCharUnique );

	TCHAR	*GetChatData();
	TCHAR	*GetUserID();

	SI16					m_siMsg;
	SI32					m_siCharUnique;

private:

	

	stChatStatusBarData		m_stChatData;

	TCHAR					m_strUserID[ MAX_PLAYER_NAME ];
	TCHAR					m_strChatData[ MAX_CHAT_DATA_LENGTH ];
};



#endif