#include "ChatStatusBarBuffer.h"
#include "ChatStatusBarDataObj.h"

CChatStatusBarBuffer::CChatStatusBarBuffer()
{	
	Initialize();
}

CChatStatusBarBuffer::~CChatStatusBarBuffer()
{
	Destroy();	
}

void CChatStatusBarBuffer::Initialize()
{
	m_siChatStatusBarDataObjCount = 0;
	m_pFirst = NULL;
	m_pLast = NULL;

	return;
}

void CChatStatusBarBuffer::Destroy()
{
	CChatStatusBarDataObj *pTemp = m_pFirst;
	CChatStatusBarDataObj *pTemp1 = NULL;
	
	while ( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = pTemp->GetNext();
		delete pTemp1;
		pTemp1 = NULL;
		--m_siChatStatusBarDataObjCount;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siChatStatusBarDataObjCount = 0;

	return;
}

void CChatStatusBarBuffer::Push( SI16 Msg, TCHAR *strChatData )
{	

	stChatStatusBarData temp;
	
	temp.si16Msg = Msg;
	memcpy( temp.strChatData, strChatData, MAX_CHAT_DATA_LENGTH - 1 );
	temp.strChatData[ MAX_CHAT_DATA_LENGTH - 1 ] = NULL;

	CChatStatusBarDataObj *pTemp = new CChatStatusBarDataObj( &temp );
	
	if ( m_pFirst )
	{
		m_pLast->SetNext( pTemp );
		pTemp->SetPre( m_pLast );
		m_pLast = pTemp;
	}
	else
	{
		m_pFirst = pTemp;
		m_pLast = pTemp;
	}

	++m_siChatStatusBarDataObjCount;

	return;
}

bool CChatStatusBarBuffer::Pop( stChatStatusBarData *pChatStatusBarData )
{
	if ( m_siChatStatusBarDataObjCount == 0 )
	{
		return false;
	}

	
	CChatStatusBarDataObj *pTemp = m_pFirst;
	memcpy( pChatStatusBarData, pTemp->GetChatStatusBarData(), sizeof( stChatStatusBarData ) );
	
	if ( m_siChatStatusBarDataObjCount > 1 )
	{
		m_pFirst->GetNext()->SetPre( NULL );
		m_pFirst = m_pFirst->GetNext();
	}
	else
	{
		m_pFirst = NULL;
		m_pLast = NULL;
	}

	--m_siChatStatusBarDataObjCount;

	
	delete pTemp;
	pTemp = NULL;

	return true;
}