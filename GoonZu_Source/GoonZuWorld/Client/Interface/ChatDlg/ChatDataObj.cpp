#include "ChatDataObj.h"


CChatDataObj::CChatDataObj() : CObj()
{
	memset( &m_stChatData, 0, sizeof( stChatStatusBarData ) );

	Initialize();
}

CChatDataObj::~CChatDataObj()
{
	Destroy();
}

void CChatDataObj::Initialize()
{
	return;
}

void CChatDataObj::Destroy()
{
	return;
}

stChatStatusBarData	*CChatDataObj::GetstChatStatusBarData()
{
	return &m_stChatData;
}

void CChatDataObj::SetChatData( TCHAR *strUserID, TCHAR *strChatData )
{
	memcpy( m_strUserID, strUserID, MAX_PLAYER_NAME );
	m_strUserID[ MAX_PLAYER_NAME - 1 ] = NULL;

	memcpy( m_strChatData, strChatData, MAX_CHAT_DATA_LENGTH );
	m_strChatData[ MAX_CHAT_DATA_LENGTH - 1 ] = NULL;
}

void CChatDataObj::SetChatData( SI16 siMsg, TCHAR *strChatData )
{
	m_siMsg = siMsg;
	memcpy( m_strChatData, strChatData, MAX_CHAT_DATA_LENGTH );
	m_strChatData[ MAX_CHAT_DATA_LENGTH - 1 ] = NULL;

	return;
}

TCHAR *CChatDataObj::GetChatData()
{
	return m_strChatData;
}

TCHAR *CChatDataObj::GetUserID()
{
	return m_strUserID;
}

void CChatDataObj::SetReject( SI16 siMsg, SI32 siCharUnique )
{
	m_siMsg = siMsg;
	m_siCharUnique = siCharUnique;
	return;
}