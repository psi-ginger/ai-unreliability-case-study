#include "List.h"
#include "AStarNode.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CAStarList::CAStarList()
//	Desc : 생 성 자
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarList::CAStarList()
{
	m_pFirst = NULL;
	m_pLast = NULL;

	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CAStarList::~CAStarList()
//	Desc : 소 멸 자
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarList::~CAStarList()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarList::Initialize()
//	Desc : 초 기 화
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarList::Initialize()
{
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarList::Destroy()
//	Desc : 종료 처리
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarList::Destroy()
{
	ListClear();
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarList::ListClear()
//	Desc : 리스트 내의 노드들 완전 삭제
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarList::ListClear()
{
	CAStarNode *pTemp1 = m_pFirst;
	CAStarNode *pTemp2 = pTemp1;

	while ( pTemp1 )
	{
		pTemp1 = pTemp1->GetNextNode();
		delete pTemp2;
		pTemp2 = pTemp1;
	}
}

/*
//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarList::AddNode( CAStarNode *pNode )
//	Desc : 리스트에 노드 추가
//		   노드 추가라는 것이 포인터를 추가한다는 뜻임.
//		   실제 객체 생성은 이 함수를 호출하기 전에 객체 생성 및 설정까지 끝낸 뒤에 노드 추가하는 것임
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarList::AddNode( CAStarNode *pNode )
{
	if ( m_pFirst )
	{
		m_pLast->SetNextNode( pNode );
		m_pLast = pNode;
		m_pLast->SetNextNode( NULL );
	}
	else
	{
		m_pFirst = pNode;
		m_pLast = pNode;
		m_pLast->SetNextNode( NULL );
	}

	return;
}
*/


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CAStarNode* CAStarList::IsExist( unsigned int uiTileX, unsigned int uiTileY )
//	Desc : 리스트에 노드가 존재하는 지를 체크
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode* CAStarList::IsExist( int uiTileX, int uiTileY )
{
	CAStarNode *pTemp = m_pFirst;

	while ( pTemp )
	{
		if ( (int)pTemp->GetTileXIndex() == uiTileX && (int)pTemp->GetTileYIndex() == uiTileY )
		{
			return pTemp;
		}
		
		pTemp = pTemp->GetNextNode();
	}

	return NULL;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CAStarNode *CAStarList::GetFirstNode()
//	Desc : 리스트 제일 앞의 노드의 포인터를 리턴함.
//	Date : 2003. 07. 03
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode *CAStarList::GetFirstNode()
{
	return m_pFirst;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarList::SetFirstNode( CAStarNode * pNode )
//	Desc : 리스트 제일 앞의 노드의 포인터를 설정함
//		   단방향 링크드 리스트는 아래와 같이 처리하는 게 제일 빠를 듯
//	Date : 2003. 07. 03
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarList::SetFirstNode( CAStarNode * pNode )
{
	if ( pNode == NULL )
	{
		m_pFirst = pNode;
		m_pLast = NULL;
	}
	else
	{
		//pNode->SetNextNode( m_pFirst );		
		//pNode->SetParentNode( NULL );				//9월 17일 수정
		m_pFirst = pNode;
	}

	return;
}

// 리스트 제일 뒤의 노드의 포인터를 리턴함.
CAStarNode *CAStarList::GetLastNode()
{
	return m_pLast;
}

// 리스트 제일 뒤의 노드의 포인터를 설정함
void CAStarList::SetLastNode( CAStarNode * pNode )
{
	m_pLast = pNode;
	m_pLast->SetNextNode( NULL );

	return;
}

// 리스트에 객체가 총 몇 개인지...
int CAStarList::GetNodeNum()
{
	CAStarNode *pTemp = m_pFirst;

	int siNum = 0;

	while( pTemp )
	{
		++siNum;
		pTemp = pTemp->GetNextNode();
	}

	return siNum;
}