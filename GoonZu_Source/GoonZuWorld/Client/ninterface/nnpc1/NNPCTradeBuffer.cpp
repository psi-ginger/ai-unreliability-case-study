
#ifdef __SUN_NNPCTRADE1__

#include "NPCTradeBuffer.h"
#include "NPCTradeData.h"

CNPCTradeBuffer::CNPCTradeBuffer()
{
	Initialize();
}

CNPCTradeBuffer::~CNPCTradeBuffer()
{
	Destroy();	
}

void CNPCTradeBuffer::Initialize()
{
	m_siNPCTradeDataCount = 0;
	m_pFirst = NULL;
	m_pLast = NULL;

	return;
}

void CNPCTradeBuffer::Destroy()
{
	CNPCTradeData *pTemp = m_pFirst;
	CNPCTradeData *pTemp1 = NULL;
	
	while ( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = pTemp->GetNext();
		delete pTemp1;
		pTemp1 = NULL;
		--m_siNPCTradeDataCount;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siNPCTradeDataCount = 0;

	return;
}

void CNPCTradeBuffer::Push( SI16 Msg, SI32 lParam, SI32 NPCCharUnique, cltItem clItem )
{
	stNPCTradeData temp;
	
	temp.si16Msg = Msg;
	temp.si32lParam = lParam;
	temp.siNPCCharUnique = NPCCharUnique;
	temp.clItem = clItem;

	CNPCTradeData *pTemp = new CNPCTradeData( &temp );
	
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

	++m_siNPCTradeDataCount;

	return;
}

bool CNPCTradeBuffer::Pop( stNPCTradeData *pNPCTradeData )
{
	if ( m_siNPCTradeDataCount == 0 )
	{
		return false;
	}
	
	CNPCTradeData *pTemp = m_pFirst;
	memcpy( pNPCTradeData, pTemp->GetTradeData(), sizeof( stNPCTradeData ) );
	
	if ( m_siNPCTradeDataCount > 1 )
	{
		m_pFirst->GetNext()->SetPre( NULL );
		m_pFirst = m_pFirst->GetNext();
	}
	else
	{
		m_pFirst = NULL;
		m_pLast = NULL;
	}

	--m_siNPCTradeDataCount;
	

	delete pTemp;
	pTemp = NULL;

	return true;
}

#endif
