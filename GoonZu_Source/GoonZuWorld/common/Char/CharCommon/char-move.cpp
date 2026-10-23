//---------------------------------
// 2003/10/22 김태곤
//---------------------------------

#include "..\Client\Client.h"


#include "..\..\..\Lib\AStar\AStarListMgr.h"
#include "..\..\Util\Util.h"
#include "..\..\Order\Order.h"

extern cltCommonLogic* pclClient;

//---------------------------------
// 캐릭터를 이동하게 만들기 위한 설정을 한다. 
//---------------------------------
bool cltCharCommon::SetMove(SI32 dx, SI32 dy)
{
	if(dx <  0)						dx	=	0;
	if(dx >= pclMap->MH.MapXsize)	dx	=	pclMap->MH.MapXsize - 1;
    if(dy < 0)						dy	=	0;
 	if(dy >= pclMap->MH.MapYsize)	dy	=	pclMap->MH.MapYsize - 1;

	clMoveInfo.siDX=dx; 
	clMoveInfo.siDY=dy;

	// 이동 경로 초기화. 
	clMovePointList.Init();
	
	//clMoveInfo.dwLastMoveClock = pclClient->CurrentClock;

	return TRUE;
}

//------------------------------------------
// 캐릭터를 이동하게 만든다.
//------------------------------------------
int cltCharCommon::Move(bool bstraightswitch)
{
	int x = GetX(), y= GetY();
	int direction;
	SI32 aniindex = 0;

	// 이동 속성이 아니면 이동하지 않는다. 
	if(IsAtb(ATB_MOVE)==FALSE)	return MOVERTNVAL_MOVEDONE;

	if(clMoveInfo.siMovePercent == 0)
	{
		bool findpathswitch = false;
		
		// 가야할 길이 없는 상태이면 길을 찾는다. 
		if(clMovePointList.siPointNum == 0)
		{
			findpathswitch = true;
		}
		
		if(findpathswitch == true)
		{
			// 이동 경로를 찾기 전에 초기화한다. 
			clMovePointList.Init();

			pclMap->MoveID	= siID;
		
			
			SI32 num	= MAX_CHAR_MOVE_POINT;
			BOOL ret;

			if ( GetCharUnique() < 1000 )
			{
				int a = 0;
			}
			
			if(pclMap->pclPath)
			{
				ret = pclMap->pclPath->FindNextPath( bstraightswitch, GetX(), GetY(), clMoveInfo.siDX, clMoveInfo.siDY, &clMovePointList.clPos[0], &num );
			}
			else
			{
				return MOVERTNVAL_MOVEDONE;
			}

			clMovePointList.siPointNum = num;

			// 전혀 이동하지 못하면 이동을 중단한다.
			if(clMovePointList.siPointNum == 0)return MOVERTNVAL_MOVEDONE;
			else if(clMovePointList.clPos[clMovePointList.siPointNum-1 ].siX == GetX() && clMovePointList.clPos[clMovePointList.siPointNum-1].siY == GetY())
			{
				return MOVERTNVAL_MOVEDONE;
			}
			
			if(TABS(clMovePointList.clPos[clMovePointList.siPointNum-1 ].siX - GetX()) > 1
			|| TABS(clMovePointList.clPos[clMovePointList.siPointNum-1 ].siY - GetY()) > 1 )
			{
				return MOVERTNVAL_MOVEDONE;
			}
		}
	}
	
	// 이동 방향을 초기화한다. 
	direction	=	0;
	
	//---------------------------------------
	// 이동할 위치를 찾는다. 
	//---------------------------------------
	clMoveInfo.siNX	=	GetX();
	clMoveInfo.siNY	=	GetY();
	if(GetX() < clMovePointList.clPos[clMovePointList.siPointNum-1].siX)
	{
		clMoveInfo.siNX++;
		direction|=EAST;
	}
	else if(GetX() > clMovePointList.clPos[clMovePointList.siPointNum-1].siX)
	{
		clMoveInfo.siNX--;
		direction|=WEST;
	}
	if(GetY() < clMovePointList.clPos[clMovePointList.siPointNum-1].siY)
	{
		clMoveInfo.siNY++;
		direction|=SOUTH;
	}
	else if(GetY() > clMovePointList.clPos[clMovePointList.siPointNum-1].siY)
	{
		clMoveInfo.siNY--;
		direction|=NORTH;
	}

	//---------------------------------------
	// 방향을 전환한다. 
	//---------------------------------------
	if(direction)
	{
		if(ChangeDirection(direction)==FALSE)return MOVERTNVAL_MOVING;
	}
	

	//-------------------------------
	// 이동 속도를 구한다. 
	//-------------------------------
	SI32 movespeed = clPB.GetTotalMoveSpeed();
	
	//--------------------------------
	// 일정 %로 이동시킨다. 
	//-------------------------------
	DWORD lastmovedelay	= min((CHAR_ACTION_INTERVAL_FRAME+1)*50, (pclClient->CurrentClock - clMoveInfo.dwLastMoveClock));
	
	// 클라이언트에서는 급작스런 이동이 없어야 한다. 
	if(pclClient->GameMode == GAMEMODE_CLIENT)
	{
		lastmovedelay = min(100, lastmovedelay);
	}

	clMoveInfo.dwLastMoveClock		= 	pclClient->CurrentClock;
	SI32 movedlength = movespeed * lastmovedelay / 50;

	movedlength += min(100, clMoveInfo.siSparePercent);
	clMoveInfo.siSparePercent = 0;
	
	if(clMoveInfo.IncMovePercent(movedlength) >= 50)
	{
		// 이동할 지점이 충돌지역이어서는 안된다. 
		if(IsColl(clMoveInfo.siNX, clMoveInfo.siNY, Block) == false )
		{
			clMoveInfo.siMovePercent = clMoveInfo.siMovePercent-100;
			
			// 이동의 기준을 만든다.
			SetMoveStandardPos(GetX(), GetY(), clMoveInfo.siNX, clMoveInfo.siNY);
			
			// 캐릭터의 위치를 변경한다. 
			SetX(clMoveInfo.siNX);
			SetY(clMoveInfo.siNY);

			if (pclClient->pclMapManager->IsFogMap(GetMapIndex()) )
			{
				FogAction();
			}

			SetUpdateSwitch(UPDATE_BASICINFO, true, 0);
			
			
			// 이웃을 업데이트 한다. 
			SetNeighbour();
		}
		else
		{
			// 길을 다시 찾게 만든다. 
			clMovePointList.siPointNum	= 0;
			clMoveInfo.siMovePercent	= 0;
		}
	}
	// 좌표 전환이 있은지 얼마 동안은, 
	else if(clMoveInfo.siMovePercent <= 0)
	{
	}
	else
	{
		// 이동의 기준을 만든다.
		SetMoveStandardPos(GetX(), GetY(), clMoveInfo.siNX, clMoveInfo.siNY);
	}
	
	// RX, RY를 계산한다.(클라이언트에서만. ) 
	CalculateRXY();

	SetAnimation(ANITYPE_MOVE);
	DoAnimationDelay();
	
	// 중간좌표까지 이상없이 도착
	if(clMoveInfo.siMovePercent == 0)
	{
		// 최종 목적지 도착 
		if(clMoveInfo.siDX == GetX() && clMoveInfo.siDY == GetY())
		{
			return MOVERTNVAL_MOVEDONE;
		}
		// 중간 목적지 도착 
		if(clMovePointList.IsArrive(GetX(), GetY()))
		{
			// 도착처리. 
			clMovePointList.Arrive();

			return MOVERTNVAL_HALFDONE;
		}
	}
	
	return MOVERTNVAL_MOVING;

}

BOOL cltCharCommon::MoveDone()
{
	if(clMoveInfo.siMovePercent)return FALSE;
	return TRUE;
}

BOOL cltCharCommon::CanMoveTo(SI32 x, SI32 y)
{
	// 지도 영역을 벗어났다면 FALSE
	if(pclMap->IsInMapArea(x, y) == false)return FALSE;

	return TRUE;
}

// 같은 지도의 특정 좌표로 이동시킨다. 
BOOL cltCharCommon::MoveTo(SI32 x, SI32 y)
{
	// 지도 영역을 벗어났다면 FALSE
	if(pclMap->IsInMapArea(x, y) == false)return FALSE;

	DeleteID();
	
	SetX(x);
	SetY(y);
	
	SetID();

	return FALSE;

}

// 캐릭터가 x, y위치에 있을 수 있는가 없는가?
bool cltCharCommon::IsColl(SHORT x, SHORT y)
{
	return pclMap->IsColl(x, y, Block);
}

// 캐릭터가 x, y위치의 에 있을 수 있는가 없는가?
bool cltCharCommon::IsColl(SHORT x, SHORT y, SHORT block)
{
	if(pclMap->IsColl(x, y, block))
	{
		return true;
	}
	return false;	
}


// 충돌 영역을 설정한다. 
bool cltCharCommon::SetBlock()
{
	bool bchanged = false;

	//------------------------------------
	// 이벤트 1
	//------------------------------------
	if(pclClient->bEventSwitch[1] == true)
	{
		if((Block & BLOCK_EVENT1) == 0)
		{
			Block |= BLOCK_EVENT1;
			bchanged = true;
		}
	}
	else
	{
		if(Block & BLOCK_EVENT1)
		{
			Block -= BLOCK_EVENT1;
		}
	}


	if(clInnerStatus.clEventInfo.bEvent1Switch)
	{
		if(Block & BLOCK_EVENT1)
		{
			Block -= BLOCK_EVENT1;
		}
	}

	//------------------------------------
	// 이벤트 2
	//------------------------------------
	if(pclClient->bEventSwitch[2] == true)
	{
		if((Block & BLOCK_EVENT2) == 0)
		{
			// 이벤트용 몬스터는 Event2지역에 얼마든지 들어갈 수 있다. 
			if(pclKI == NULL || pclKI->IsAtb(ATB_EVENTXBOX) == false)
			{
				Block |= BLOCK_EVENT2;
				bchanged = true;
			}
		}
	}
	else
	{
		if(Block & BLOCK_EVENT2)
		{
			Block -= BLOCK_EVENT2;
		}
	}


	if(clInnerStatus.clEventInfo.bEvent2Switch)
	{
		if(Block & BLOCK_EVENT2)
		{
			Block -= BLOCK_EVENT2;
		}
	}
	
	//------------------------------------
	// 이벤트 3
	//------------------------------------
	if(pclClient->bEventSwitch[3] == true)
	{
		if((Block & BLOCK_EVENT3) == 0)
		{
			Block |= BLOCK_EVENT3;
			bchanged = true;
		}
	}
	else
	{
		if(Block & BLOCK_EVENT3)
		{
			Block -= BLOCK_EVENT3;
		}
	}

	if(clInnerStatus.clEventInfo.bEvent3Switch)
	{
		if(Block & BLOCK_EVENT3)
		{
			Block -= BLOCK_EVENT3;
		}
	}

	//------------------------------------
	// 이벤트 4
	//------------------------------------
	if(pclClient->bEventSwitch[4] == true)
	{
		if((Block & BLOCK_EVENT4) == 0)
		{
			Block |= BLOCK_EVENT4;
			bchanged = true;
		}
	}
	else
	{
		if(Block & BLOCK_EVENT4)
		{
			Block -= BLOCK_EVENT4;
		}
	}

	if(clInnerStatus.clEventInfo.bEvent4Switch)
	{
		if(Block & BLOCK_EVENT4)
		{
			Block -= BLOCK_EVENT4;
		}
	}

	//------------------------------------
	// 이벤트 5
	//------------------------------------
	if(pclClient->bEventSwitch[5] == true)
	{
		if((Block & BLOCK_EVENT5) == 0)
		{
			Block |= BLOCK_EVENT5;
			bchanged = true;
		}
	}
	else
	{
		if(Block & BLOCK_EVENT5)
		{
			Block -= BLOCK_EVENT5;
		}
	}

	if(clInnerStatus.clEventInfo.bEvent5Switch)
	{
		if(Block & BLOCK_EVENT5)
		{
			Block -= BLOCK_EVENT5;
		}
	}

	//------------------------------------
	// 이벤트 6
	//------------------------------------
	if(pclClient->bEventSwitch[6] == true)
	{
		if((Block & BLOCK_EVENT6) == 0)
		{
			Block |= BLOCK_EVENT6;
			bchanged = true;
		}
	}
	else
	{
		if(Block & BLOCK_EVENT6)
		{
			Block -= BLOCK_EVENT6;
		}
	}

	if(clInnerStatus.clEventInfo.bEvent6Switch)
	{
		if(Block & BLOCK_EVENT6)
		{
			Block -= BLOCK_EVENT6;
		}
	}

	//------------------------------------
	// 이벤트 7
	//------------------------------------
	if(pclClient->bEventSwitch[7] == true)
	{
		if((Block & BLOCK_EVENT7) == 0)
		{
			Block |= BLOCK_EVENT7;
			bchanged = true;
		}
	}
	else
	{
		if(Block & BLOCK_EVENT7)
		{
			Block -= BLOCK_EVENT7;
		}
	}

	if(clInnerStatus.clEventInfo.bEvent7Switch)
	{
		if(Block & BLOCK_EVENT7)
		{
			Block -= BLOCK_EVENT7;
		}
	}

	if(pclClient->GameMode == GAMEMODE_SERVER && bchanged)
	{
		// 선박에 타고 있지않고 충돌하는 영역이라면, 
		if( IsColl(GetX(), GetY()) == true)
		{
			OrderStop(GetID(), BY_COMPUTER);

			cltWarpInfo clWarpInfo(1, GetMapIndex(), false, false, GetX(), GetY(), -1, -1);
			SetWarpInfo(&clWarpInfo, 20);
			Warp();
		}
	}


	return true;
}