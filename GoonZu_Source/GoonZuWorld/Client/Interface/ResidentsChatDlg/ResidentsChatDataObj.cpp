#include "ResidentsChatDataObj.h"

CResidentsChatDataObj::CResidentsChatDataObj() : CObj()
{
	memset( &m_stChatData, 0, sizeof( stChatStatusBarData ) );

	Initialize();
}

CResidentsChatDataObj::~CResidentsChatDataObj()
{
	Destroy();
}

void CResidentsChatDataObj::Initialize()
{
	return;
}

void CResidentsChatDataObj::Destroy()
{
	return;
}

stChatStatusBarData	*CResidentsChatDataObj::GetstChatStatusBarData()
{
	return &m_stChatData;
}

void CResidentsChatDataObj::SetChatData( char *strUserID, char *strChatData )
{
	memcpy( m_strUserID, strUserID, MAX_PLAYER_NAME );
	m_strUserID[ MAX_PLAYER_NAME - 1 ] = NULL;

	memcpy( m_strChatData, strChatData, MAX_CHAT_DATA_LENGTH );
	m_strChatData[ MAX_CHAT_DATA_LENGTH - 1 ] = NULL;

}

void CResidentsChatDataObj::SetChatData( SI16 siMsg, char *strChatData )
{
	m_siMsg = siMsg;
	memcpy( m_strChatData, strChatData, MAX_CHAT_DATA_LENGTH );
	m_strChatData[ MAX_CHAT_DATA_LENGTH - 1 ] = NULL;
	return;
}

char *CResidentsChatDataObj::GetChatData()
{
	return m_strChatData;
}

char *CResidentsChatDataObj::GetUserID()
{
	return m_strUserID;
}

void CResidentsChatDataObj::SetReject( SI16 siMsg, SI32 siCharUnique )
{
	m_siMsg = siMsg;
	m_siCharUnique = siCharUnique;
	return;
}
