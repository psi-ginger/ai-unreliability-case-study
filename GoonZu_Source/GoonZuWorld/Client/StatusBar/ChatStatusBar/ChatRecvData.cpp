#include "ChatRecvData.h"

CChatRecvData::CChatRecvData()
{
	m_pPre = NULL;
	m_pNext = NULL;
}

CChatRecvData::~CChatRecvData()
{
	m_pPre = NULL;
	m_pNext = NULL;
}

void CChatRecvData::Initialize()
{
	m_pPre = NULL;
	m_pNext = NULL;

	memset( m_strUserID, 0, MAX_PLAYER_NAME );
	memset( m_strChatData, 0, MAX_CHAT_DATA_LENGTH );
}

void CChatRecvData::Destroy()
{
	m_pPre = NULL;
	m_pNext = NULL;
}

CChatRecvData* CChatRecvData::GetPre()
{
	return m_pPre;
}

void CChatRecvData::SetPre( CChatRecvData	*pPre )
{
	m_pPre = pPre;
	return;
}

CChatRecvData* CChatRecvData::GetNext()
{
	return m_pNext;
}

void CChatRecvData::SetNext( CChatRecvData	*pNext )
{
	m_pNext = pNext;
	return;
}

void CChatRecvData::SetChatData( TCHAR *strUserID, TCHAR *strChatData )
{
	memcpy( m_strUserID, strUserID, MAX_PLAYER_NAME );
	m_strUserID[ MAX_PLAYER_NAME - 1 ] = NULL;

	memcpy( m_strChatData, strChatData, MAX_CHAT_DATA_LENGTH );
	m_strChatData[ MAX_CHAT_DATA_LENGTH - 1 ] = NULL;
}

TCHAR* CChatRecvData::GetUserID()
{
	return m_strUserID;
}

TCHAR* CChatRecvData::GetChatData()
{
	return m_strChatData;
}