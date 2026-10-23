#include "RichEditChatDataMgr.h"
#include "RichEditChatDataObj.h"

CRichEditChatDataMgr::CRichEditChatDataMgr( bool bRecursive ) : CLibListMgr( bRecursive )
{
	CreateMaxEmptyObj();
	//Initialize();
}

CRichEditChatDataMgr::~CRichEditChatDataMgr()
{
	Destroy();
}

void CRichEditChatDataMgr::Initialize()
{
	CRichEditChatDataObj *pTemp = NULL;

	while( true )
	{
		pTemp = ( CRichEditChatDataObj * ) PopFrontToExistListAndPushBackToEmptyList();

		if ( pTemp == NULL )
		{
			return;
		}
	}

	return;
}

void CRichEditChatDataMgr::Destroy()
{
	return;
}
	
void CRichEditChatDataMgr::CreateMaxEmptyObj()
{
	CRichEditChatDataObj *pTemp = NULL;

	for ( int i = 0; i < MAX_RICHEDIT_CHAT_RECORD_NUM; ++i )
	{
		pTemp = new CRichEditChatDataObj();
		PushBackToEmptyList( pTemp );
	}
	
	return;
}

void CRichEditChatDataMgr::AddData( char *Data, SI32 DataSize )
{
	CRichEditChatDataObj *pObj = (CRichEditChatDataObj *)PopFrontToEmptyListAndPushBackToExistList();
	pObj->Initialize();
	pObj->SetData( (char*)Data, DataSize );
	return;
}

bool CRichEditChatDataMgr::Pop( stRichEditChatDataSend *pSendData )
{
	CRichEditChatDataObj *pObj = (CRichEditChatDataObj *)PopFrontToExistListAndPushBackToEmptyList();
	
	if ( pObj )
	{
		UI32 DataSize = sizeof( stRichEditChatDataSend );
		pObj->GetData( (char*)pSendData, &DataSize );

		return true;
	}

	return false;
}