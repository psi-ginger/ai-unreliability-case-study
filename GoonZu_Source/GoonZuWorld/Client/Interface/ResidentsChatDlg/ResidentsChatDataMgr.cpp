#include "ResidentsChatDataMgr.h"
#include "ResidentsChatDataObj.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"

CResidentsChatDataMgr::CResidentsChatDataMgr( bool bCircleMode ) : CLibListMgr( bCircleMode )
{
}

CResidentsChatDataMgr::~CResidentsChatDataMgr()
{
	Destroy();
}

void CResidentsChatDataMgr::Initialize()
{
	CResidentsChatDataObj *pTemp = NULL;

	while( true )
	{
		pTemp = ( CResidentsChatDataObj * ) PopFrontToExistListAndPushBackToEmptyList();

		if ( pTemp == NULL )
		{
			return;
		}
	}

	return;
}

void CResidentsChatDataMgr::Destroy()
{
	return;
}

void CResidentsChatDataMgr::CreateMaxEmptyObj( int si32MaxObjNum )
{
	CResidentsChatDataObj *pTemp = NULL;

	for ( int i = 0; i < si32MaxObjNum; ++i )
	{
		pTemp = new CResidentsChatDataObj();
		PushBackToEmptyList( pTemp );
	}

	return;
}

void CResidentsChatDataMgr::Push( SI16 Msg, char *strResidentsChatData )
{
	CResidentsChatDataObj *pTemp = ( CResidentsChatDataObj * ) PopFrontToEmptyListAndPushBackToExistList();

	pTemp->GetstChatStatusBarData()->si16Msg = Msg;
	strcpy( pTemp->GetstChatStatusBarData()->strChatData , strResidentsChatData );

	return;
}

void CResidentsChatDataMgr::Push( char *strUserID, char *strResidentsChatRecvData )
{
	CResidentsChatDataObj *pTemp = ( CResidentsChatDataObj * ) PopFrontToEmptyListAndPushBackToExistList();
	
	pTemp->SetChatData( strUserID, strResidentsChatRecvData );

	return;
}

bool CResidentsChatDataMgr::Pop( stChatStatusBarData *pResidentsChatStatusBarData )
{
	CResidentsChatDataObj *pTemp = ( CResidentsChatDataObj * ) PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp == NULL )
	{
		return false;
	}
	
	memcpy( pResidentsChatStatusBarData, pTemp->GetstChatStatusBarData(), sizeof( stChatStatusBarData ) );
	return true;
}

void CResidentsChatDataMgr::LoadData( char *strChatData )
{
	memset( strChatData, 0, ( MAX_PLAYER_NAME + MAX_CHAT_DATA_LENGTH + 10 ) * MAX_CHAT_DATA_RECORD_NUM );
	strcpy( strChatData, "" );
	
	CResidentsChatDataObj *pTemp = ( CResidentsChatDataObj * )GetExistList()->GetFirst();

	while ( pTemp )
	{
		strcat( strChatData, "[" );
		strcat( strChatData, pTemp->GetUserID() );
		strcat( strChatData, "] : " );
		strcat( strChatData, pTemp->GetChatData() );
		strcat( strChatData, "\r\n" );

		pTemp = ( CResidentsChatDataObj * )pTemp->GetNext();
	}

	return;
}

