#include "AStarListMgr.h"
#include "AStarOpenedList.h"
#include "AStarClosedList.h"
#include "AStarNotUsingNodeList.h"
#include "AStarNode.h"
#include "AStarStackListMgr.h"
#include "AStarStackList.h"
#include "AStarStackEmptyList.h"

#include "../../Client/client.h"
extern cltCommonLogic* pclClient;


CAStarListMgr::CAStarListMgr( void *pParent )
{
	m_pBest = NULL;
	IsBlocking = NULL;

	m_pOpenedList = new CAStarOpenedList();
	m_pClosedList = new CAStarClosedList();
	m_pNotUsingNodeList = new CAStarNotUsingNodeList( MAX_NODE_NUM );

	m_pStackListMgr = new CAStarStackListMgr( MAX_NODE_NUM );

	m_siStartXIndex = 0;
	m_siStartYIndex = 0;

	m_siEndXIndex = 0;
	m_siEndYIndex = 0;

	m_pParentPtr = pParent;

	m_bMode = true;

	m_siMaxFindPathNum = 10;

	Initialize();
}

CAStarListMgr::~CAStarListMgr()
{
	Destroy();
}

// 초 기 화
void CAStarListMgr::Initialize()
{
	m_pBest = NULL;

	CAStarNode *pTemp = NULL;

	while( pTemp = m_pOpenedList->Pop() )
	{
		pTemp->Initialize();
		m_pNotUsingNodeList->AddNode( pTemp );
	}

	m_pOpenedList->Initialize();

	while( pTemp = m_pClosedList->Pop() )
	{
		pTemp->Initialize();
		m_pNotUsingNodeList->AddNode( pTemp );
	}

	m_pClosedList->Initialize();

	m_pStackListMgr->Initialize();

	m_siEndXIndex = 0;
	m_siEndYIndex = 0;

	m_siStartXIndex = 0;
	m_siStartYIndex = 0;

	return;
}

// 종료처리
void CAStarListMgr::Destroy()
{
	if ( m_pOpenedList )
	{
		delete m_pOpenedList;
		m_pOpenedList = NULL;
	}

	if ( m_pClosedList )
	{
		delete m_pClosedList;
		m_pClosedList = NULL;
	}

	if ( m_pNotUsingNodeList )
	{
		delete m_pNotUsingNodeList;
		m_pNotUsingNodeList= NULL;
	}

	if ( m_pStackListMgr )
	{
		delete m_pStackListMgr;
		m_pStackListMgr = NULL;
	}


	return;
}

// OpendList 포인터 얻어옮
CAStarOpenedList *CAStarListMgr::GetOpenedList()
{
	return m_pOpenedList;
}

// ClosedList 포인터 얻어옮
CAStarClosedList *CAStarListMgr::GetClosedList()
{
	return m_pClosedList;
}

bool CAStarListMgr::FindNextPath( int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex, int *siNextXIndex, int *siNextYIndex, bool (*IsBlocked)( int, int, void* ) )
{
	/*
	int AStarNodeNum = m_pOpenedList->GetNodeNum() + m_pClosedList->GetNodeNum() + m_pNotUsingNodeList->GetNodeNum();
	int AStarStackNum = m_pStackListMgr->GetStackList()->GetNodeNum() + m_pStackListMgr->GetStackEmptyList()->GetNodeNum();

	CString temp;
	temp.Format( TEXT("1\nOpendListNum = %d, ClosedListNum = %d, NotUsingNodeListNum = %d, AStarNodeSumNum = %d\nStackNum = %d, StackEmptyNum = %d, AStarStackSumNum = %d"), m_pOpenedList->GetNodeNum(), m_pClosedList->GetNodeNum(), m_pNotUsingNodeList->GetNodeNum(), AStarNodeNum, m_pStackListMgr->GetStackList()->GetNodeNum(), m_pStackListMgr->GetStackEmptyList()->GetNodeNum(), AStarStackNum );
	AfxMessageBox( temp );
	*/

	Initialize();

	m_siStartXIndex = siStartXIndex;
	m_siStartYIndex = siStartYIndex;

	/*
	AStarNodeNum = m_pOpenedList->GetNodeNum() + m_pClosedList->GetNodeNum() + m_pNotUsingNodeList->GetNodeNum();
	AStarStackNum = m_pStackListMgr->GetStackList()->GetNodeNum() + m_pStackListMgr->GetStackEmptyList()->GetNodeNum();
	temp.Format( TEXT("2\nOpendListNum = %d, ClosedListNum = %d, NotUsingNodeListNum = %d, AStarNodeSumNum = %d\nStackNum = %d, StackEmptyNum = %d, AStarStackSumNum = %d"), m_pOpenedList->GetNodeNum(), m_pClosedList->GetNodeNum(), m_pNotUsingNodeList->GetNodeNum(), AStarNodeNum, m_pStackListMgr->GetStackList()->GetNodeNum(), m_pStackListMgr->GetStackEmptyList()->GetNodeNum(), AStarStackNum );
	AfxMessageBox( temp );
	*/


	CAStarNode *pTemp = m_pNotUsingNodeList->GetLastNodeAndMoveLastPtr();

	if ( pTemp == NULL )
	{
		//AfxMessageBox(  TEXT("error") );
		return false;
	}

	// 시작 지점까지는 비용이 들지 않으므로 Goal 은 0 이 됨
	pTemp->SetFitness( siStartXIndex, siStartYIndex, siEndXIndex, siEndYIndex, 0 );

	m_siEndXIndex = siEndXIndex;
	m_siEndYIndex = siEndYIndex;

	m_pOpenedList->AddNode( pTemp );

	int siRetVal = 0;

	while ( siRetVal == 0 )
	{
		siRetVal = Step();
	};

	int siOriHeuristic = 0;
	int siNewHeuristic = 0;

	SI16 OffSetX = 0;
	SI16 OffSetY = 0;

	//int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex
	if ( siRetVal == -1 || ! m_pBest ) 
	{			
		//GetNextPathNotFoundEndPos( siStartXIndex, siStartYIndex, siNextXIndex, siNextYIndex );

		OffSetX = siStartXIndex - m_siEndXIndex;
		OffSetY = siStartYIndex - m_siEndYIndex;
		siOriHeuristic = (int)(sqrt( (float)(OffSetX*OffSetX+OffSetY*OffSetY) ));

		OffSetX = (*siNextXIndex) - m_siEndXIndex;
		OffSetY = (*siNextYIndex) - m_siEndYIndex;
		siNewHeuristic = (int)(sqrt( (float)(OffSetX*OffSetX+OffSetY*OffSetY) ));

		if ( siOriHeuristic < siNewHeuristic )
		{
			*siNextXIndex = siStartXIndex;
			*siNextYIndex = siStartYIndex;
		}

		return false;
	}

	GetNextPath( m_pBest, siStartXIndex, siStartYIndex, siNextXIndex, siNextYIndex );

	OffSetX = siStartXIndex - m_siEndXIndex;
	OffSetY = siStartYIndex - m_siEndYIndex;
	siOriHeuristic = (int)(sqrt( (float)(OffSetX*OffSetX+OffSetY*OffSetY) ));
	OffSetX = (*siNextXIndex) - m_siEndXIndex;
	OffSetY = (*siNextYIndex) - m_siEndYIndex;
	siNewHeuristic = (int)(sqrt( (float)(OffSetX*OffSetX+OffSetY*OffSetY) ));

	if ( siOriHeuristic < siNewHeuristic )
	{
		*siNextXIndex = siStartXIndex;
		*siNextYIndex = siStartYIndex;
	}

	return true;
}

bool CAStarListMgr::FindNextPath( bool bNotUseAStar, int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex, cltPos *pNextPos, int *psiRetNum )
{
	if ( psiRetNum == NULL || *psiRetNum > 10 || *psiRetNum < 1 )
	{
#ifdef _DEBUG
		MessageBox( NULL, TEXT("psiRetNum Error"), TEXT("psiRetNum Error"), MB_OK );
#endif
		return false;
	}

	m_siStartXIndex = siStartXIndex;
	m_siStartYIndex = siStartYIndex;

	m_siMaxFindPathNum = *psiRetNum;

	if ( bNotUseAStar )
	{
		int TempXIndex = siStartXIndex;
		int TempYIndex = siStartYIndex;
		int TempXIndex1 = siStartXIndex;
		int TempYIndex1 = siStartYIndex;

		int TempXPos[10], TempYPos[10];

		int i = 0;

		for ( i = 0; i < m_siMaxFindPathNum; ++i )
		{
			if ( TempXIndex == siEndXIndex && TempYIndex == siEndYIndex )
			{
				//*psiRetNum = i;
				break;
				//return true;
			}

			TempXIndex1 = TempXIndex;
			TempYIndex1 = TempYIndex;

			if ( TempXIndex > siEndXIndex )
			{
				--TempXIndex;
			}
			else if ( TempXIndex < siEndXIndex )
			{
				++TempXIndex;
			}

			if ( TempYIndex > siEndYIndex )
			{
				--TempYIndex;
			}
			else if ( TempYIndex < siEndYIndex )
			{
				++TempYIndex;
			}

			if ( IsBlocking( TempXIndex, TempYIndex, TempXIndex1, TempYIndex1, m_pParentPtr ) )
			{
				//*psiRetNum = i;
				break;

				//return false;
			}
			else
			{

				TempXPos[ i ] = TempXIndex;
				TempYPos[ i ] = TempYIndex;
				//pNextPos[ i ].siX = TempXIndex;
				//pNextPos[ i ].siY = TempYIndex;
			}
		}

		*psiRetNum = i;

		for ( int j = 0; j < i; ++j )
		{
			pNextPos[ j ].siX = TempXPos[ i - j - 1 ];
			pNextPos[ j ].siY = TempYPos[ i - j - 1 ];
		}

		return true;
	}


	/*
	int AStarNodeNum = m_pOpenedList->GetNodeNum() + m_pClosedList->GetNodeNum() + m_pNotUsingNodeList->GetNodeNum();
	int AStarStackNum = m_pStackListMgr->GetStackList()->GetNodeNum() + m_pStackListMgr->GetStackEmptyList()->GetNodeNum();

	CString temp;
	temp.Format( TEXT("1\nOpendListNum = %d, ClosedListNum = %d, NotUsingNodeListNum = %d, AStarNodeSumNum = %d\nStackNum = %d, StackEmptyNum = %d, AStarStackSumNum = %d"), m_pOpenedList->GetNodeNum(), m_pClosedList->GetNodeNum(), m_pNotUsingNodeList->GetNodeNum(), AStarNodeNum, m_pStackListMgr->GetStackList()->GetNodeNum(), m_pStackListMgr->GetStackEmptyList()->GetNodeNum(), AStarStackNum );
	AfxMessageBox( temp );
	*/

	Initialize();

	/*
	AStarNodeNum = m_pOpenedList->GetNodeNum() + m_pClosedList->GetNodeNum() + m_pNotUsingNodeList->GetNodeNum();
	AStarStackNum = m_pStackListMgr->GetStackList()->GetNodeNum() + m_pStackListMgr->GetStackEmptyList()->GetNodeNum();
	temp.Format( TEXT("2\nOpendListNum = %d, ClosedListNum = %d, NotUsingNodeListNum = %d, AStarNodeSumNum = %d\nStackNum = %d, StackEmptyNum = %d, AStarStackSumNum = %d"), m_pOpenedList->GetNodeNum(), m_pClosedList->GetNodeNum(), m_pNotUsingNodeList->GetNodeNum(), AStarNodeNum, m_pStackListMgr->GetStackList()->GetNodeNum(), m_pStackListMgr->GetStackEmptyList()->GetNodeNum(), AStarStackNum );
	AfxMessageBox( temp );
	*/


	CAStarNode *pTemp = m_pNotUsingNodeList->GetLastNodeAndMoveLastPtr();

	if ( pTemp == NULL )
	{
		//AfxMessageBox(  TEXT("error") );
		return false;
	}

	// 시작 지점까지는 비용이 들지 않으므로 Goal 은 0 이 됨
	pTemp->SetFitness( siStartXIndex, siStartYIndex, siEndXIndex, siEndYIndex, 0 );

	m_siEndXIndex = siEndXIndex;
	m_siEndYIndex = siEndYIndex;

	m_pOpenedList->AddNode( pTemp );

	int siRetVal = 0;

	while ( siRetVal == 0 )
	{
		siRetVal = Step();
	};

	int siOriHeuristic = 0;
	int siNewHeuristic = 0;

	//int siStartXIndex, int siStartYIndex, int siEndXIndex, int siEndYIndex
	if ( siRetVal == -1 || ! m_pBest )			// 최종 목적지에 갈 수 없거나, 노드수가 모자려서 더 이상 갈 수 없는 경우임
	{	
		//m_pBest = NULL;

		//GetNextPathNotFoundEndPos( siStartXIndex, siStartYIndex, &pNextPos[0].siX, &pNextPos[0].siY );
		GetNextPathNotFoundEndPos( siStartXIndex, siStartYIndex, pNextPos, psiRetNum );

		/*
		int OffsetX = siStartXIndex - m_siEndXIndex;
		int OffsetY = siStartYIndex - m_siEndYIndex;
		siOriHeuristic = sqrt( (float)( OffsetX*OffsetX + OffsetY*OffsetY ) );
		OffsetX = (pNextPos[0].siX) - m_siEndXIndex;
		OffsetY = (pNextPos[0].siY) - m_siEndYIndex;
		siNewHeuristic = sqrt( (float)( OffsetX*OffsetX + OffsetY*OffsetY ) );

		if ( siOriHeuristic <= siNewHeuristic )
		{
		pNextPos[0].siX = siStartXIndex;
		pNextPos[0].siY = siStartYIndex;
		*psiRetNum = 1;
		}
		*/



		return false;
	}

	GetNextPath( m_pBest, siStartXIndex, siStartYIndex, pNextPos, psiRetNum);
	return true;
}

void CAStarListMgr::GetNextPathNotFoundEndPos( int siStartXIndex, int siStartYIndex, cltPos *pNextPos, int *psiRetNum)
{
	if ( m_pClosedList->GetFirstNode() == m_pClosedList->GetLastNode() )
	{
		/*
		*siNextXIndex  = siStartXIndex;
		*siNextYIndex  = siStartYIndex;
		*/
		pNextPos[0].siX = siStartXIndex;
		pNextPos[0].siY = siStartYIndex;
		*psiRetNum = 1;

		return;
	}

	CAStarNode *pClosedFirst = m_pClosedList->GetFirstNode();
	CAStarNode *pTemp = NULL;

	float	SmallestHeuristic = 1000.0f;

	while( pClosedFirst )
	{	
		if ( SmallestHeuristic > pClosedFirst->GetHeuristic() )
		{
			SmallestHeuristic = pClosedFirst->GetHeuristic();
			pTemp = pClosedFirst;
		}
		else if ( SmallestHeuristic == pClosedFirst->GetHeuristic() )
		{
			/*
			OffsetX = siStartXIndex - pClosedFirst->GetTileXIndex();
			OffsetY = siStartYIndex - pClosedFirst->GetTileYIndex();
			// 현재 서있는 지점에서의 Heuristic
			if ( SmallestHeuristic > sqrt( (float)OffsetX*OffsetX+OffsetY*OffsetY ) )
			{
			SmallestHeuristic = pClosedFirst->GetHeuristic();
			pTemp = pClosedFirst;
			}
			*/
			if ( pTemp && pClosedFirst->GetGoal() < pTemp->GetGoal() )
			{
				SmallestHeuristic = pClosedFirst->GetHeuristic();
				pTemp = pClosedFirst;
			}
		}

		pClosedFirst = pClosedFirst->GetNextNode();
	}

	if ( pTemp )
	{
		CAStarNode *pParentBest = pTemp;
		m_siPathListNum = 0;

		CAStarNode *pRememberBestTen = NULL;
		pRememberBestTen = pParentBest;

		while( pParentBest )
		{
			if ( pParentBest->GetParentNode() == NULL )
			{
				break;
			}
			else
			{
				++m_siPathListNum;
				pParentBest = pParentBest->GetParentNode();
			}

			if ( pTemp == pParentBest )
			{
				pclClient->pclLog->FilePrint(TEXT("SMLee.txt"), TEXT("AStar Loop"));
				break;
			}
		}

		if ( m_siPathListNum == 0 )
		{
			pNextPos[0].siX = siStartXIndex;
			pNextPos[0].siY = siStartYIndex;
			*psiRetNum = 1;
		}
		else
		{

			if ( m_siPathListNum > m_siMaxFindPathNum )
			{
				if ( m_siPathListNum - m_siMaxFindPathNum >= 0 )
				{
					for ( int i = 0; i < m_siPathListNum - m_siMaxFindPathNum; ++i )
					{
						pRememberBestTen = pRememberBestTen->GetParentNode();
					}
				}

				m_siPathListNum = 0;

				while( pRememberBestTen )
				{
					if ( pRememberBestTen->GetParentNode() == NULL )
					{
						break;
					}

					pNextPos[ m_siPathListNum ].siX = pRememberBestTen->GetTileXIndex();
					pNextPos[ m_siPathListNum ].siY = pRememberBestTen->GetTileYIndex();

					++m_siPathListNum;
					pRememberBestTen = pRememberBestTen->GetParentNode();
				}

				*psiRetNum = m_siMaxFindPathNum;
			}
			else
			{
				m_siPathListNum = 0;

				while( pRememberBestTen )
				{
					if ( pRememberBestTen->GetParentNode() == NULL )
					{
						break;
					}

					pNextPos[ m_siPathListNum ].siX = pRememberBestTen->GetTileXIndex();
					pNextPos[ m_siPathListNum ].siY = pRememberBestTen->GetTileYIndex();

					++m_siPathListNum;
					pRememberBestTen = pRememberBestTen->GetParentNode();
				}

				*psiRetNum = m_siPathListNum;
			}
		}



	}
	else
	{
		pNextPos[0].siX = siStartXIndex;
		pNextPos[0].siY = siStartYIndex;
		*psiRetNum = 1;

		return;
	}

	return;
}


void CAStarListMgr::GetNextPath( CAStarNode *pNode, int siStartXIndex, int siStartYIndex, int *siNextXIndex, int *siNextYIndex )
{
	CAStarNode *pParentBest = pNode;
	CAStarNode *pTemp = NULL;

	while( pParentBest )
	{
		if ( pParentBest->GetParentNode() == NULL )
		{
			break;
		}
		else
		{
			pTemp = pParentBest;
			pParentBest = pParentBest->GetParentNode();
		}
	}

	if ( pTemp == NULL )
	{
		*siNextXIndex = siStartXIndex;
		*siNextYIndex = siStartYIndex;
	}
	else
	{
		*siNextXIndex = pTemp->GetTileXIndex();
		*siNextYIndex = pTemp->GetTileYIndex();
	}

	return;
}

void CAStarListMgr::GetNextPath( CAStarNode *pNode, int siStartXIndex, int siStartYIndex, cltPos *pNextPos, int *psiRetNum )
{
	CAStarNode *pParentBest = pNode;

	CAStarNode *pRememberBestTen = NULL;		// 상위 10개를 	뽑아내기 위해서...
	m_siPathListNum = 0;

	pRememberBestTen = pParentBest;

	while( pParentBest )
	{
		if ( pParentBest->GetParentNode() == NULL )
		{
			break;
		}
		else
		{
			//m_pPathList[ m_siPathListNum ++ ] = pParentBest;
			/*
			pNextPos[ m_siPathListNum ].siX = pParentBest->GetTileXIndex();
			pNextPos[ m_siPathListNum ].siY = pParentBest->GetTileYIndex();

			m_siPathListNum++;

			if ( m_siPathListNum == 10 )
			{
			break;
			}
			*/			

			++m_siPathListNum;

			/*
			if ( ++m_siPathListNum > 10 ) 
			{
			pRememberBestTen = pRememberBestTen->GetParentNode();
			}
			*/

			pParentBest = pParentBest->GetParentNode();
		}
	}

	if ( m_siPathListNum == 0 )
	{
		pNextPos[0].siX = siStartXIndex;
		pNextPos[0].siY = siStartYIndex;
		*psiRetNum = 1;
	}
	else
	{
		//pRememberBestTen = pRememberBestTen->GetParentNode();

		if ( m_siPathListNum > m_siMaxFindPathNum )
		{
			for ( int i = 0; i < m_siPathListNum - m_siMaxFindPathNum; ++i )
			{
				pRememberBestTen = pRememberBestTen->GetParentNode();
			}

			m_siPathListNum = 0;

			while( pRememberBestTen )
			{
				if ( pRememberBestTen->GetParentNode() == NULL )
				{
					break;
				}

				pNextPos[ m_siPathListNum ].siX = pRememberBestTen->GetTileXIndex();
				pNextPos[ m_siPathListNum ].siY = pRememberBestTen->GetTileYIndex();

				++m_siPathListNum;
				pRememberBestTen = pRememberBestTen->GetParentNode();
			}

			*psiRetNum = m_siMaxFindPathNum;
		}
		else
		{
			m_siPathListNum = 0;

			while( pRememberBestTen )
			{
				if ( pRememberBestTen->GetParentNode() == NULL )
				{
					break;
				}

				pNextPos[ m_siPathListNum ].siX = pRememberBestTen->GetTileXIndex();
				pNextPos[ m_siPathListNum ].siY = pRememberBestTen->GetTileYIndex();				
				++m_siPathListNum;
				pRememberBestTen = pRememberBestTen->GetParentNode();
			}

			*psiRetNum = m_siPathListNum;			
		}
	}

	/*
	if (m_siPathListNum == NULL )
	{
	pNextPos[0].siX = siStartXIndex;
	pNextPos[0].siY = siStartYIndex;
	*psiRetNum = 1;

	return;
	}
	else
	{
	if ( m_siPathListNum < *psiRetNum )
	{
	for ( int i = 0; i < m_siPathListNum; ++i )
	{
	pNextPos[ i ].siX = m_pPathList[ i ]->GetTileXIndex();
	pNextPos[ i ].siY = m_pPathList[ i ]->GetTileYIndex();
	}
	*psiRetNum = m_siPathListNum;

	return;
	}
	else
	{
	for ( int i = 0; i < *psiRetNum; ++i )
	{
	pNextPos[ i ].siX = m_pPathList[ i ]->GetTileXIndex();
	pNextPos[ i ].siY = m_pPathList[ i ]->GetTileYIndex();
	}

	return;
	}
	}
	*/

	return;
}

int CAStarListMgr::Step()
{
	if (! ( m_pBest = GetBest() ) )
	{
		return -1;
	}

	//if (m_pBest->number == m_iDNum) 
	if ( (int)m_pBest->GetTileXIndex() == m_siEndXIndex && (int)m_pBest->GetTileYIndex() == m_siEndYIndex )
	{
		return 1;
	}

	if( ! CreateChildren(m_pBest) )
	{
		return -1;
	}

	return 0;
}


// 연결될 수 있는 노드만을 생성 ( 8 방향 중에서 Block된 곳을 제외하고 생성.)
// *m_pChildren[8] 도 여기서 연결해 줌
bool CAStarListMgr::CreateChildren( CAStarNode *pParent )
{
	CAStarNode *pTemp = NULL;

	CAStarNode *pTempOpened = NULL;
	CAStarNode *pTempClosed = NULL;

	int Goal = pParent->GetGoal() + 1;

	int siTileX;
	int siTileY;

	// 8 방향
	if ( m_bMode )
	{
		for ( int i = -1; i < 2; ++i ) 
		{
			for ( int j = -1; j < 2; ++j ) 
			{
				if ( i == 0 && j == 0 )
				{
					continue;
				}

				siTileX = pParent->GetTileXIndex() + i;
				siTileY = pParent->GetTileYIndex() + j;

				if ( ! IsBlocking( siTileX, siTileY, pParent->GetTileXIndex(), pParent->GetTileYIndex(), m_pParentPtr ) )
				{
					pTempOpened = m_pOpenedList->IsExist( siTileX, siTileY );
					pTempClosed = m_pClosedList->IsExist( siTileX, siTileY );


					if ( pTempOpened != NULL )
					{
						if ( Goal < pTempOpened->GetGoal() )
						{
							pTempOpened->SetParentNode( pParent );
							pTempOpened->SetGoal( Goal );
							pTempOpened->SetFitness( Goal + pTempOpened->GetHeuristic() );
						}
					}
					else if ( pTempClosed != NULL )
					{
						pParent->SetChild( pTempClosed );

						if ( Goal < pTempClosed->GetGoal() )
						{
							pTempClosed->SetParentNode( pParent );
							pTempClosed->SetGoal( Goal );
							pTempClosed->SetFitness( Goal + pTempClosed->GetHeuristic() );

							UpdateParents( pTempClosed );
						}
					}
					else
					{
						pTemp = m_pNotUsingNodeList->GetLastNodeAndMoveLastPtr();

						if ( pTemp == NULL )
						{
							///////////////////////////////
							// 생성한 노드를 모두 사용한 경우... 여기서 끝을 봐야지... 더 이상 가면 서버 부하가 생기므로...
							// 길 찾기 멈추는 코드 여기 삽입해야 함
							//////////////////////////////

							//AfxMessageBox(" 노드 모자란다. ");


							return false;
						}
						else
						{
							pTemp->SetParentNode( pParent ) ;

							pTemp->SetFitness( siTileX, siTileY, m_siEndXIndex, m_siEndYIndex, Goal );

							m_pOpenedList->AddNode( pTemp );

							pParent->SetChild( pTemp );
						}
					}
				}
			}
		}
	}
	else			// 4방향
	{
		for ( int i = -1; i < 2; ++i ) 
		{
			for ( int j = -1; j < 2; ++j ) 
			{
				if ( ( i == 0 && j == 0 ) || ( i != 0 && j != 0 ) )
					//if ( i == 0 || j == 0 )
				{
					continue;
				}

				siTileX = pParent->GetTileXIndex() + i;
				siTileY = pParent->GetTileYIndex() + j;

				if ( ! IsBlocking( siTileX, siTileY, pParent->GetTileXIndex(), pParent->GetTileYIndex(), m_pParentPtr ) )
				{
					pTempOpened = m_pOpenedList->IsExist( siTileX, siTileY );
					pTempClosed = m_pClosedList->IsExist( siTileX, siTileY );


					if ( pTempOpened != NULL )
					{
						if ( Goal < pTempOpened->GetGoal() )
						{
							pTempOpened->SetParentNode( pParent );
							pTempOpened->SetGoal( Goal );
							pTempOpened->SetFitness( Goal + pTempOpened->GetHeuristic() );
						}
					}
					else if ( pTempClosed != NULL )
					{
						pParent->SetChild( pTempClosed );

						if ( Goal < pTempClosed->GetGoal() )
						{
							pTempClosed->SetParentNode( pParent );
							pTempClosed->SetGoal( Goal );
							pTempClosed->SetFitness( Goal + pTempClosed->GetHeuristic() );

							UpdateParents( pTempClosed );
						}
					}
					else
					{
						pTemp = m_pNotUsingNodeList->GetLastNodeAndMoveLastPtr();

						if ( pTemp == NULL )
						{
							///////////////////////////////
							// 생성한 노드를 모두 사용한 경우... 여기서 끝을 봐야지... 더 이상 가면 서버 부하가 생기므로...
							// 길 찾기 멈추는 코드 여기 삽입해야 함
							//////////////////////////////

							//AfxMessageBox(" 노드 모자란다. ");


							return false;
						}
						else
						{
							pTemp->SetParentNode( pParent ) ;

							pTemp->SetFitness( siTileX, siTileY, m_siEndXIndex, m_siEndYIndex, Goal );

							m_pOpenedList->AddNode( pTemp );

							pParent->SetChild( pTemp );
						}
					}
				}
			}
		}
	}

	return true;
}

// 부모 변경
void CAStarListMgr::UpdateParents( CAStarNode *pNode )
{
	int siGoal = pNode->GetGoal();
	char cChildrenNum = pNode->GetChildrenNum();


	CAStarNode *pKid = NULL;

	for ( char i = 0; i < cChildrenNum; ++i )
	{
		pKid = pNode->GetChild( i );

		if ( siGoal + 1 < pKid->GetGoal() )
		{
			pKid->SetGoal( siGoal + 1 );
			pKid->SetFitness( pKid->GetGoal() + pKid->GetHeuristic() );
			pKid->SetParentNode( pNode );

			m_pStackListMgr->Push( pKid );
		}
	}

	CAStarNode *pParent = NULL;

	while( m_pStackListMgr->GetStackList()->GetFirst() )
	{
		pParent = m_pStackListMgr->Pop();

		cChildrenNum = pParent->GetChildrenNum();

		for ( char i = 0 ; i < cChildrenNum; ++i )
		{
			pKid = pParent->GetChild( i );

			if ( pParent->GetGoal() + 1  < pKid->GetGoal() )
			{
				pKid->SetGoal( pParent->GetGoal() + 1 );
				pKid->SetFitness( pKid->GetGoal() + pKid->GetHeuristic() );
				pKid->SetParentNode( pParent );
				m_pStackListMgr->Push( pKid );
			}
		}
	}

}

// m_pOpendList 에서 제일 앞에 있는 노드를 리턴하고, 그 노드를 m_pClosedList 제일 앞에 넣음.
CAStarNode *CAStarListMgr::GetBest()
{
	CAStarNode *pOpenedTemp = m_pOpenedList->GetFirstNode();

	if ( ! pOpenedTemp )
	{
		return NULL;
	}

	CAStarNode *pClosedTemp = m_pClosedList->GetFirstNode();

	m_pOpenedList->SetFirstNode( pOpenedTemp->GetNextNode() );

	m_pClosedList->SetFirstNode( pOpenedTemp );
	m_pClosedList->GetFirstNode()->SetNextNode( pClosedTemp );

	return pOpenedTemp;
}

// m_pBestPtr의 포인터 리턴해줌
CAStarNode *CAStarListMgr::GetBestPtr()
{
	return m_pBest;
}