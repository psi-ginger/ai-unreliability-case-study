#ifndef _RESIDENTSCHATDATAMGR_H_
#define _RESIDENTSCHATDATAMGR_H_

#include "ResidentsChatDlgBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"

class CResidentsChatDataMgr : public CLibListMgr
{
public:
	CResidentsChatDataMgr( bool bCircleMode = false );
	~CResidentsChatDataMgr();

	void	Initialize();
	void	Destroy();

	void	CreateMaxEmptyObj( int	si32MaxObjNum );

	void	Push( SI16 Msg, char *strChatData );
	void	PushWhisper( SI16 Msg, char *strChatData );
	void	PushReject( SI16 Msg, SI32 siCharUnique );
	void	Push( char *strUserID, char *strChatRecvData );
	

	bool	Pop( stChatStatusBarData *pChatStatusBarData );
	bool	Pop( char *strChatData, SI32 *psiCharUnique );

	void	LoadData( char *strChatData );

private:

};


#endif