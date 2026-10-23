#include "AStarStackList.h"
#include "AStarNode.h"


CAStarStackList::CAStarStackList()
{
	m_pFirst = NULL;
	Initialize();
}

CAStarStackList::~CAStarStackList()
{
	Destroy();
}

void CAStarStackList::Initialize()
{
	m_pFirst = NULL;
	return;
}

void CAStarStackList::Destroy()
{	
	stAStarStack *pTemp = m_pFirst;

	while( pTemp )
	{
		m_pFirst = m_pFirst->m_pNext;
		delete pTemp;
		pTemp = m_pFirst;
	}
}

void CAStarStackList::Push( stAStarStack *pStack )
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

stAStarStack *CAStarStackList::Pop()
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

stAStarStack *CAStarStackList::GetFirst()
{
	return m_pFirst;
}

int CAStarStackList::GetNodeNum()
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