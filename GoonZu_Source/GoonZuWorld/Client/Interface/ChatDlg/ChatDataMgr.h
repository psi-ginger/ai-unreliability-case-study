#ifndef _CHATDATAMGR_H_
#define _CHATDATAMGR_H_

#include "ChatDlgBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"

class CChatDataMgr : public CLibListMgr
{
public:
	CChatDataMgr( bool bCircleMode = false );
	~CChatDataMgr();

	void	Initialize();
	void	Destroy();

	void	CreateMaxEmptyObj( int	si32MaxObjNum );

	void	Push( SI16 Msg, TCHAR *strChatData );
	void	PushWhisper( SI16 Msg, TCHAR *strChatData );
	void	PushReject( SI16 Msg, SI32 siCharUnique );
	void	Push( TCHAR *strUserID, TCHAR *strChatRecvData );
	

	bool	Pop( stChatStatusBarData *pChatStatusBarData );
	bool	Pop( TCHAR *strChatData, SI16 txtSize, SI32 *psiCharUnique );

	void	LoadData( TCHAR *strChatData, SI16 txtSize );

private:

};


#endif