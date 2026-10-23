#ifndef _RESIDENTSCHATDATAOBJ_H_
#define _RESIDENTSCHATDATAOBJ_H_

#include "ResidentsChatDlgBaseInfo.h"
#include "..\..\..\Common\SMLib\LibList\Obj.h"

class CResidentsChatDataObj : public CObj
{
public:
	CResidentsChatDataObj();
	~CResidentsChatDataObj();

	void	Initialize();
	void	Destroy();

	stChatStatusBarData	*GetstChatStatusBarData();
	void	SetChatData( char *strUserID, char *strChatData );
	void	SetChatData( SI16 siMsg, char *strChatData );

	void	SetReject( SI16 siMsg, SI32 siCharUnique );

	char	*GetChatData();
	char	*GetUserID();

	SI16					m_siMsg;
	SI32					m_siCharUnique;

private:

	

	stChatStatusBarData		m_stChatData;

	char					m_strUserID[ MAX_PLAYER_NAME ];
	char					m_strChatData[ MAX_CHAT_DATA_LENGTH ];
};



#endif