#include "ItemInventoryBuffer.h"
#include "MoveDataObj.h"

CItemInventoryBuffer::CItemInventoryBuffer()
{
	Initialize();
}

CItemInventoryBuffer::~CItemInventoryBuffer()
{
	Destroy();	
}

void CItemInventoryBuffer::Initialize()
{
	m_siMoveDataCount = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
	return;
}

void CItemInventoryBuffer::Destroy()
{
	CMoveDataObj *pTemp = m_pFirst;
	CMoveDataObj *pTemp1 = NULL;
	
	while ( pTemp )
	{
		pTemp1 = pTemp;
		pTemp = pTemp->GetNext();
		delete pTemp1;
		pTemp1 = NULL;
		--m_siMoveDataCount;
	}

	m_pFirst = NULL;
	m_pLast = NULL;

	m_siMoveDataCount = 0;


	return;
}

CMoveDataObj* CItemInventoryBuffer::GetFirst()
{
	return m_pFirst;
}

void CItemInventoryBuffer::SetFirst( CMoveDataObj	*pMoveDataObj )
{
	m_pFirst = pMoveDataObj;
	return;
}

CMoveDataObj* CItemInventoryBuffer::GetLast()
{
	return m_pLast;
}

void CItemInventoryBuffer::SetLast( CMoveDataObj	*pMoveDataObj )
{
	m_pLast = pMoveDataObj;
	return;
}

void CItemInventoryBuffer::Push( SI16 Msg, SI32 lParam, cltItem *pclItem )
//void CItemInventoryBuffer::Push( SI32 siStartPos, SI32 siEndPos, cltItem clItem )
{	
	stMoveData temp;
	/*
	temp.siStartPos	= siStartPos;
	temp.siEndPos	= siEndPos;
	temp.clMoveItem	= clItem;
	*/
	temp.si16Message = Msg;
	temp.si32lParam = lParam;

	if(  pclItem )
	{
		memcpy( &temp.clMoveItem, pclItem, sizeof( cltItem ) );
	}

	CMoveDataObj *pTemp = new CMoveDataObj( &temp );
	
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

	++m_siMoveDataCount;
	
	return;
}

void CItemInventoryBuffer::Push( SI16 Msg, SI32 lParam, SI32 lParam1 )
{
	stMoveData temp;
	
	temp.si16Message = Msg;
	temp.si32lParam = lParam;
	temp.si32lParam1 = lParam1;

	CMoveDataObj *pTemp = new CMoveDataObj( &temp );
	
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

	++m_siMoveDataCount;

	return;
}

void CItemInventoryBuffer::Push( SI16 Msg, SI32 lParam, SI32 lParam1, SI16 lParam2, cltItem *pclItem )
{
	stMoveData temp;
	
	temp.si16Message = Msg;
	temp.si32lParam = lParam;
	temp.si32lParam1 = lParam1;
	temp.si16lParam2 = lParam2;
	
	if(  pclItem )
	{
		memcpy( &temp.clMoveItem, pclItem, sizeof( cltItem ) );
	}

	CMoveDataObj *pTemp = new CMoveDataObj( &temp );
	
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

	++m_siMoveDataCount;
	
	return;
}

void CItemInventoryBuffer::Push( BYTE *pData, SI32 siSize )
{
	CMoveDataObj *pTemp = new CMoveDataObj( pData, siSize );
	
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

	++m_siMoveDataCount;

	return;
}

bool CItemInventoryBuffer::Pop( stMoveData *pMoveData )
{
	if ( m_siMoveDataCount == 0 )
	{
		return false;
	}

	CMoveDataObj *pTemp = m_pFirst;
	memcpy( pMoveData, &pTemp->m_stMoveData, sizeof( stMoveData ) );
	
	if ( m_siMoveDataCount > 1 )
	{
		m_pFirst->GetNext()->SetPre( NULL );
		m_pFirst = m_pFirst->GetNext();
	}
	else
	{
		m_pFirst = NULL;
		m_pLast = NULL;
	}

	--m_siMoveDataCount;
	

	delete pTemp;
	pTemp = NULL;

	return true;
}


bool CItemInventoryBuffer::Pop( BYTE *pData )
{
	if ( m_siMoveDataCount == 0 )
	{
		return false;
	}

	CMoveDataObj *pTemp = m_pFirst;
	pTemp->GetData( pData );
	
	if ( m_siMoveDataCount > 1 )
	{
		m_pFirst->GetNext()->SetPre( NULL );
		m_pFirst = m_pFirst->GetNext();
	}
	else
	{
		m_pFirst = NULL;
		m_pLast = NULL;
	}

	--m_siMoveDataCount;
	
	delete pTemp;
	pTemp = NULL;

	return true;
}


SI32 CItemInventoryBuffer::GetDataCount()
{
	return m_siMoveDataCount;
}
