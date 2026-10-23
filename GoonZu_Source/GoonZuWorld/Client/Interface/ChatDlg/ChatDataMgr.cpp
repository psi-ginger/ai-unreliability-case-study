#include "ChatDataMgr.h"
#include "ChatDataObj.h"
#include "..\..\..\Common\SMLib\LibList\LibList.h"

CChatDataMgr::CChatDataMgr( bool bCircleMode ) : CLibListMgr( bCircleMode )
{
}

CChatDataMgr::~CChatDataMgr()
{
	Destroy();
}

void CChatDataMgr::Initialize()
{
	CChatDataObj *pTemp = NULL;

	bool loop = true;
	while( loop )
	{
		pTemp = ( CChatDataObj * ) PopFrontToExistListAndPushBackToEmptyList();

		if ( pTemp == NULL )
		{
			return;
		}
	}

	return;
}

void CChatDataMgr::Destroy()
{
	return;
}

void CChatDataMgr::CreateMaxEmptyObj( int si32MaxObjNum )
{
	CChatDataObj *pTemp = NULL;

	for ( int i = 0; i < si32MaxObjNum; ++i )
	{
		pTemp = new CChatDataObj();
		PushBackToEmptyList( pTemp );
	}

	return;
}

void CChatDataMgr::Push( SI16 Msg, TCHAR *strChatData )
{
	CChatDataObj *pTemp = ( CChatDataObj * ) PopFrontToEmptyListAndPushBackToExistList();

	pTemp->GetstChatStatusBarData()->si16Msg = Msg;
	StringCchCopy( pTemp->GetstChatStatusBarData()->strChatData , MAX_CHAT_DATA_LENGTH, strChatData );

	return;
}

void CChatDataMgr::PushWhisper( SI16 Msg, TCHAR *strChatData )
{
	CChatDataObj *pTemp = ( CChatDataObj * ) PopFrontToEmptyListAndPushBackToExistList();
	pTemp->SetChatData( Msg, strChatData );

	return;
}

void CChatDataMgr::PushReject( SI16 Msg, SI32 siCharUnique )
{
	CChatDataObj *pTemp = ( CChatDataObj * ) PopFrontToEmptyListAndPushBackToExistList();
	pTemp->SetReject( Msg, siCharUnique );

	return;
}

void CChatDataMgr::Push( TCHAR *strUserID, TCHAR *strChatRecvData )
{
	CChatDataObj *pTemp = ( CChatDataObj * ) PopFrontToEmptyListAndPushBackToExistList();
	
	pTemp->SetChatData( strUserID, strChatRecvData );

	return;
}

bool CChatDataMgr::Pop( stChatStatusBarData *pChatStatusBarData )
{
	CChatDataObj *pTemp = ( CChatDataObj * ) PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp == NULL )
	{
		return false;
	}
	
	memcpy( pChatStatusBarData, pTemp->GetstChatStatusBarData(), sizeof( stChatStatusBarData ) );
	return true;
}

bool CChatDataMgr::Pop( TCHAR *strChatData, SI16 txtSize, SI32 *psiCharUnique )
{
	CChatDataObj *pTemp = ( CChatDataObj * ) PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp == NULL )
	{
		return false;
	}

	if ( pTemp->m_siMsg == WHISPER_CHAT_REJECT )
	{
		*psiCharUnique = pTemp->m_siCharUnique;
	}
	else if ( pTemp->m_siMsg == WHISPER_CHAT_DATA_SEND )
	{
		StringCchCopy( strChatData, txtSize, pTemp->GetChatData() );
	}	

	return true;
}

void CChatDataMgr::LoadData( TCHAR *strChatData, SI16 txtSize )
{
	strChatData[ 0 ] = NULL;
	
	CChatDataObj *pTemp = ( CChatDataObj * )GetExistList()->GetFirst();

	while ( pTemp )
	{
		StringCchCat( strChatData, txtSize, TEXT("(") );
		StringCchCat( strChatData, txtSize, pTemp->GetUserID() );
		StringCchCat( strChatData, txtSize, TEXT(") : ") );
		StringCchCat( strChatData, txtSize, pTemp->GetChatData() );
		StringCchCat( strChatData, txtSize, TEXT("\r\n") );

		pTemp = ( CChatDataObj * )pTemp->GetNext();
	}

	return;
}

