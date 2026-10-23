#include "AStarStackListMgr.h"
#include "AStarNode.h"
#include "AStarStackList.h"
#include "AStarStackEmptyList.h"

CAStarStackListMgr::CAStarStackListMgr( int Max_Node_Num )
{
	m_pStackList = new CAStarStackList();
	m_pStackEmptyList = new CAStarStackEmptyList( Max_Node_Num );	
	
	Initialize();
}

CAStarStackListMgr::~CAStarStackListMgr()
{
	Destroy();
}

void CAStarStackListMgr::Initialize()
{
	stAStarStack *pStack = NULL;
	
	while( pStack = m_pStackList->Pop() )
	{
		pStack->m_pNext = NULL;
		pStack->m_pNode = NULL;

		m_pStackEmptyList->Push( pStack );
	}

	return;
}

void CAStarStackListMgr::Destroy()
{
	stAStarStack *pStack = NULL;
	
	while( pStack = m_pStackList->Pop() )
	{
		pStack->m_pNext = NULL;
		pStack->m_pNode = NULL;

		delete pStack;

		pStack = NULL;
	}

	while( pStack = m_pStackEmptyList->Pop() )
	{
		pStack->m_pNext = NULL;
		pStack->m_pNode = NULL;

		delete pStack;

		pStack = NULL;
	}
	
	delete m_pStackList;
	delete m_pStackEmptyList;

	return;
}

void CAStarStackListMgr::Push( CAStarNode *pNode )
{
	stAStarStack *pStack = m_pStackEmptyList->Pop();
	pStack->m_pNext = NULL;
	pStack->m_pNode = pNode;
	m_pStackList->Push( pStack );
}

CAStarNode *CAStarStackListMgr::Pop()
{
	stAStarStack *pStack = m_pStackList->Pop();
	m_pStackEmptyList->Push( pStack );
	return pStack->m_pNode;
}

CAStarStackList *CAStarStackListMgr::GetStackList()
{
	return m_pStackList;
}

CAStarStackEmptyList *CAStarStackListMgr::GetStackEmptyList()
{
	return m_pStackEmptyList;
}

int CAStarStackListMgr::GetStackNum()
{
	return m_pStackList->GetNodeNum() + m_pStackEmptyList->GetNodeNum();
}