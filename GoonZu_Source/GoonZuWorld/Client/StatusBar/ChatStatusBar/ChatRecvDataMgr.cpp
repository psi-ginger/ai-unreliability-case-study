#include "ChatRecvDataMgr.h"
#include "ChatRecvData.h"

CChatRecvDataMgr::CChatRecvDataMgr()
{
	m_siRecvDataNum = 0;
	m_pFirst = NULL;
	m_pLast = NULL;

	Initialize();
}

CChatRecvDataMgr::~CChatRecvDataMgr()
{
	Destroy();	
}

void CChatRecvDataMgr::Initialize()
{
	m_siRecvDataNum = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
}

void CChatRecvDataMgr::Destroy()
{
	m_siRecvDataNum = 0;

	CChatRecvData *pTemp = m_pLast;

	while( pTemp )
	{
		pTemp = m_pLast->GetPre();
		delete m_pLast;
		m_pLast = pTemp;
	}

	m_pFirst = NULL;
	m_pLast = NULL;
}

void CChatRecvDataMgr::AddChatRecvData( TCHAR *strUserID, TCHAR *strChatRecvData )
{	

	if ( m_siRecvDataNum > MAX_CHAT_DATA_RECORD_NUM )
	{
		PushFirstToLast( strUserID, strChatRecvData );
	}
	else
	{
		CChatRecvData *pTemp = new CChatRecvData();
		pTemp->SetChatData( strUserID, strChatRecvData );
		PushBack( pTemp );

		++m_siRecvDataNum;
	}
	
	return;
}


void CChatRecvDataMgr::PushBack( CChatRecvData *pChatRecvData )
{
	if ( m_pFirst )
	{
		m_pLast->SetNext( pChatRecvData );
		pChatRecvData->SetPre( m_pLast );
		m_pLast = pChatRecvData;
	}
	else
	{
		m_pFirst = pChatRecvData;
		m_pLast = pChatRecvData;
	}

	return;
}

void CChatRecvDataMgr::PushFirstToLast( TCHAR *strUserID, TCHAR *strChatRecvData )
{
	CChatRecvData	*pMoveNode = m_pFirst;

	m_pFirst->GetNext()->SetPre( NULL );
	m_pFirst = m_pFirst->GetNext();

	
	pMoveNode->Initialize();
	pMoveNode->SetChatData( strUserID, strChatRecvData );

	m_pLast->SetNext( pMoveNode );
	pMoveNode->SetPre( m_pLast );
	m_pLast = pMoveNode;

	return;
}

SI32 CChatRecvDataMgr::GetRecvDataNum()
{
	return m_siRecvDataNum;
}

void CChatRecvDataMgr::GetIndexData( SI32 siIndex, TCHAR *strPrintData , SI16 txtSize )
{
	CChatRecvData	*pTemp = m_pFirst;

	for ( int i = 0; i < siIndex; ++i )
	{
		pTemp = pTemp->GetNext();
	}

	StringCchPrintf( strPrintData, txtSize, TEXT("%s : %s"), pTemp->GetUserID(), pTemp->GetChatData() );

	return;
}
