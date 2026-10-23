#include "AStarNode.h"
#include <math.h>

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CAStarNode::CAStarNode()
//	Desc : 생 성 자
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode::CAStarNode()
{
	m_pParent = NULL;
	m_pNext = NULL;

	m_pEmptyPre = NULL;
	m_pEmptyNext = NULL;

	m_cChildrenNum = 0;

	for ( short i = 0; i < 8; ++ i )
	{
		m_pChildren[ i ] = NULL;
	}

	m_fFitness = 0;
	m_siGoal = 0;
	m_fHeuristic = 0;

	m_uiTileXIndex = 0;
	m_uiTileYIndex = 0;
	
	Initialize();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CAStarNode::~CAStarNode()
//	Desc : 소 멸 자
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode::~CAStarNode()
{
	Destroy();
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Initialize()
//	Desc : 초 기 화
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::Initialize()
{
	m_pParent = NULL;
	m_pNext = NULL;

	m_pEmptyPre = NULL;
	m_pEmptyNext = NULL;

	m_cChildrenNum = 0;

	for ( short i = 0; i < 8; ++ i )
	{
		m_pChildren[ i ] = NULL;
	}
	
	m_fFitness = 0;
	m_siGoal = 0;
	m_fHeuristic = 0;

	m_uiTileXIndex = 0;
	m_uiTileYIndex = 0;

	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 종료 처리
//		   m_pNext 해제 하는 코드 여기 없음. 나중에 추가해 주어야 함.
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::Destroy()
{	
	m_cChildrenNum = 0;

	m_fFitness = 0;
	m_siGoal = 0;
	m_fHeuristic = 0;	

	m_uiTileXIndex = 0;
	m_uiTileYIndex = 0;

	return;
}

// 타일 X 좌표 설정
void CAStarNode::SetTileXIndex( int siTileXIndex )
{
	m_uiTileXIndex = siTileXIndex;
	return;
}

// 타일 Y 좌표 설정
void CAStarNode::SetTileYIndex( int siTileYIndex )
{
	m_uiTileYIndex = siTileYIndex;
	return;
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 적합도 값 얻어옮
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
float CAStarNode::GetFitness()
{
	return m_fFitness;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 적합도 값 설정
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetFitness( float fFitness )
{
	m_fFitness = fFitness;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 한번에 값들 설정하게 하기 위해서 ( 계산 과정을 여기서 모두 처리하는게 좋을 것 같아서임. )
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetFitness( int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex, int siGoal )
{	
	int OffsetX = siStartXIndex - siEndXIndex;
	int OffsetY = siStartYIndex - siEndYIndex;
	m_fHeuristic = sqrt( (float)( OffsetX * OffsetX + OffsetY * OffsetY ) );
	m_siGoal = siGoal;
	m_fFitness =  m_fHeuristic + m_siGoal;

	m_uiTileXIndex = siStartXIndex;
	m_uiTileYIndex = siStartYIndex;

	return;
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 목표 값 얻어옮
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
int CAStarNode::GetGoal()
{
	return m_siGoal;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 목표 값 설정
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetGoal( int siGoal )
{
	m_siGoal = siGoal;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 휴리스틱 값 얻어옮
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
float CAStarNode::GetHeuristic()
{
	return m_fHeuristic;
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::Destroy()
//	Desc : 휴리스틱 값 설정
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
/*
void CAStarNode::SetHeuristic( int siHeuristic )
{
	m_siHeuristic = siHeuristic;
	return;
}
*/

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CAStarNode *CAStarNode::GetParentNode()
//	Desc : Parent Node 포인터 얻어옮
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode *CAStarNode::GetParentNode()
{
	return m_pParent;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetParentNode( CAStarNode *pNode )
//	Desc : m_pParent Node 포인터 설정
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetParentNode( CAStarNode *pNode )
{
	m_pParent = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetNextNode( CAStarNode *pNode )
//	Desc : m_pNext Node 포인터 설정
//		   OpenedList 또는 ClosedList 둘 중 하나는 반드시 추가되어야 하므로
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetNextNode( CAStarNode *pNode )
{
	m_pNext = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetNextNode( CAStarNode *pNode )
//	Desc : m_pEmptyPre 포인터 얻어옮
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode *CAStarNode::GetEmptyPre()
{
	return m_pEmptyPre;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetNextNode( CAStarNode *pNode )
//	Desc : m_pEmptyPre 포인터 설정
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetEmptyPre( CAStarNode *pNode )
{
	m_pEmptyPre = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetNextNode( CAStarNode *pNode )
//	Desc : m_pEmptyNext 포인터 얻어옮
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode *CAStarNode::GetEmptyNext()
{
	return m_pEmptyNext;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetNextNode( CAStarNode *pNode )
//	Desc : m_pEmptyNext 포인터 설정
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetEmptyNext( CAStarNode *pNode )
{
	m_pEmptyNext = pNode;
	return;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetChild( CAStarNode *pNode )
//	Desc : 차일드 얻어옮
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
CAStarNode *CAStarNode::GetChild( char cIndex )
{
	return m_pChildren[ cIndex ];
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetChild( CAStarNode *pNode )
//	Desc : 차일드 배정
//	Date : 2003. 07. 02
//	Update : 
//--------------------------------------------------------------------------------------------
void CAStarNode::SetChild( CAStarNode *pNode )
{
	m_pChildren[ m_cChildrenNum++ ] = pNode;
}


//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - void CAStarNode::SetChild( CAStarNode *pNode )
//	Desc : 차일드 수 리턴
//	Date : 2003. 07. 03
//	Update : 
//--------------------------------------------------------------------------------------------
char CAStarNode::GetChildrenNum()
{
	return m_cChildrenNum;
}
