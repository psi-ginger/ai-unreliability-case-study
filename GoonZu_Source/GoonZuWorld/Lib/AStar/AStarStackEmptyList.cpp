#include "AStarStackEmptyList.h"
#include "AStarNode.h"

CAStarStackEmptyList::CAStarStackEmptyList( int Max_Node_Num )
{
	m_pFirst = NULL;

	for ( int i = 0; i < Max_Node_Num; ++i )
	{
		stAStarStack *pTemp = new stAStarStack;
		pTemp->m_pNext = NULL;
		pTemp->m_pNode = NULL;
		Push( pTemp );
	}
	
	Initialize();
}

CAStarStackEmptyList::~CAStarStackEmptyList()
{
	Destroy();
}

void CAStarStackEmptyList::Initialize()
{
	return;
}

void CAStarStackEmptyList::Destroy()
{
	stAStarStack *pTemp = m_pFirst;

	while( pTemp )
	{
		m_pFirst = m_pFirst->m_pNext;
		delete pTemp;
		pTemp = m_pFirst;
	}

	return;
}

void CAStarStackEmptyList::Push( stAStarStack *pStack )
{
	if ( m_pFirst == NULL ) 
	{
		m_pFirst = pStack;
	} 
	else
	{
		pStack->m_pNext = m_pFirst;
		m_pFirst = pStack;
	}

	return;
}

stAStarStack *CAStarStackEmptyList::Pop()
{
	if ( m_pFirst )
	{
		stAStarStack *pTemp = m_pFirst;
		
		m_pFirst = m_pFirst->m_pNext;
		
		return pTemp;
	}
	else
	{
		return NULL;
	}
}

int CAStarStackEmptyList::GetNodeNum()
{
	stAStarStack *pTemp = m_pFirst;

	int siNum = 0;

	while( pTemp )
	{
		++siNum;
		pTemp = pTemp->m_pNext;
	}

	return siNum;
}