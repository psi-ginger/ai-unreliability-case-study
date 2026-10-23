#ifndef _CHATSTATUSBARBUFFER_H_
#define _CHATSTATUSBARBUFFER_H_

#include "ChatStatusBarBaseInfo.h"

class CChatStatusBarDataObj;

class CChatStatusBarBuffer
{
public:
	CChatStatusBarBuffer();
	~CChatStatusBarBuffer();

	void	Initialize();
	void	Destroy();

	void	Push( SI16 Msg, TCHAR *strChatData );		
	bool	Pop( stChatStatusBarData *pChatStatusBarData );

private:

	CChatStatusBarDataObj	*m_pFirst;
	CChatStatusBarDataObj	*m_pLast;

	SI32					m_siChatStatusBarDataObjCount;	

};


#endif