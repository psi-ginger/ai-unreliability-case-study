#ifndef _CHATRECVDATAMGR_H_
#define _CHATRECVDATAMGR_H_

#include "ChatStatusBarBaseInfo.h"

class CChatRecvData;

class CChatRecvDataMgr
{
public:
	CChatRecvDataMgr();
	~CChatRecvDataMgr();

	void	Initialize();
	void	Destroy();

	void	AddChatRecvData( TCHAR *strUserID, TCHAR *strChatRecvData );

	void	PushBack( CChatRecvData *pChatRecvData );
	void	PushFirstToLast( TCHAR *strUserID, TCHAR *strChatRecvData );

	SI32	GetRecvDataNum();

	void	GetIndexData( SI32 siIndex, TCHAR *strPrintData , SI16 txtSize);


private:
	CChatRecvData		*m_pFirst;
	CChatRecvData		*m_pLast;

	SI32				m_siRecvDataNum;
};

#endif