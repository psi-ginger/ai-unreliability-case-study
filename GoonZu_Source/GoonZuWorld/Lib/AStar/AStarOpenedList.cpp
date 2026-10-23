#include "AStarOpenedList.h"
#include "AStarNode.h"

CAStarOpenedList::CAStarOpenedList() : CAStarList()
{
	Initialize();
}

CAStarOpenedList::~CAStarOpenedList()
{
	Destroy();	
}

void CAStarOpenedList::Initialize()
{	
	return;
}

void CAStarOpenedList::Destroy()
{
	CAStarNode *pTemp;
	while ( pTemp = Pop() )
	{
		delete pTemp;
	}

	return;
}

void CAStarOpenedList::AddNode( CAStarNode *pAddNode )
{
	CAStarNode *pTempFirst = GetFirstNode();

	CAStarNode *pPrev = NULL;

	if ( ! pTempFirst )
	{
		SetFirstNode( pAddNode );		
		pAddNode->SetNextNode( NULL );

		return;
	}

	
	while ( pTempFirst )
	{
		if ( pAddNode->GetFitness() > pTempFirst->GetFitness() )
		{
			pPrev = pTempFirst;
			pTempFirst = pTempFirst->GetNextNode();
		}
		else
		{
			if ( pPrev )
			{
				pPrev->SetNextNode( pAddNode );
				pAddNode->SetNextNode( pTempFirst );

				if ( pPrev == GetLastNode() )
				{
					SetLastNode( pAddNode );
				}
			}
			else
			{
				CAStarNode *pTemp = GetFirstNode();
				SetFirstNode( pAddNode );
				GetFirstNode()->SetNextNode( pTemp );
			}

			return;
		}
	}

	pPrev->SetNextNode( pAddNode );
	SetLastNode ( pAddNode );
	
	return;
}

// ÇÏ³ª ²ôÁý¾î ³¿
CAStarNode *CAStarOpenedList::Pop()
{
	CAStarNode *pTemp = GetFirstNode();
	
	if ( pTemp )
	{
		SetFirstNode( pTemp->GetNextNode() );
		
		pTemp->SetNextNode( NULL );
	}

	return pTemp;
}