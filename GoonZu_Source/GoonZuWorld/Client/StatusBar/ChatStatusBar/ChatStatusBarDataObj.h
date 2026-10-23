#ifndef _CHATSTATUSBARDATAOBJ_H_
#define _CHATSTATUSBARDATAOBJ_H_

#include "ChatStatusBarBaseInfo.h"

class CChatStatusBarDataObj
{
public:
	CChatStatusBarDataObj( stChatStatusBarData *pChatStatusBarData );
	~CChatStatusBarDataObj();

	CChatStatusBarDataObj*	GetPre();
	void					SetPre( CChatStatusBarDataObj *pChatStatusBarData );
	
	CChatStatusBarDataObj*	GetNext();
	void					SetNext( CChatStatusBarDataObj *pChatStatusBarData );

	stChatStatusBarData*	GetChatStatusBarData();

private:
	stChatStatusBarData		m_stChatStatusBarData;

	CChatStatusBarDataObj	*m_pPre;
	CChatStatusBarDataObj	*m_pNext;

};


#endif