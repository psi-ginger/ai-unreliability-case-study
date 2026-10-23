#ifndef _RICHEDITCHATDATAMGR_H_
#define _RICHEDITCHATDATAMGR_H_

#include "..\..\..\Common\SMLib\LibList\LibListMgr.h"
#include "RichEditChatMgrDlgBaseInfo.h"

class CRichEditChatDataMgr : public CLibListMgr
{
public:
	CRichEditChatDataMgr( bool bRecursive );
	~CRichEditChatDataMgr();

	void	Initialize();
	void	Destroy();
	
	void	CreateMaxEmptyObj();

	void	AddData( char *Data, SI32 DataSize );
	void	Push( SI16 Msg, char *ChatText );
	bool	Pop( stRichEditChatDataSend *pSendData );

private:

};

#endif