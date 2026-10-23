#include "ChatStatusBarDataObj.h"

CChatStatusBarDataObj::CChatStatusBarDataObj( stChatStatusBarData *pChatStatusBarData )
{
	memcpy( &m_stChatStatusBarData, pChatStatusBarData, sizeof( m_stChatStatusBarData ) );
	m_pPre = NULL;
	m_pNext = NULL;
}

CChatStatusBarDataObj::~CChatStatusBarDataObj()
{
}

CChatStatusBarDataObj* CChatStatusBarDataObj::GetPre()
{
	return m_pPre;
}

void CChatStatusBarDataObj::SetPre( CChatStatusBarDataObj * pTradeData )
{
	m_pPre = pTradeData;
	return;
}
	
CChatStatusBarDataObj* CChatStatusBarDataObj::GetNext()
{
	return m_pNext;
}

void CChatStatusBarDataObj::SetNext( CChatStatusBarDataObj * pTradeData )
{
	m_pNext = pTradeData;
	return;
}

stChatStatusBarData* CChatStatusBarDataObj::GetChatStatusBarData()
{
	return &m_stChatStatusBarData;
}