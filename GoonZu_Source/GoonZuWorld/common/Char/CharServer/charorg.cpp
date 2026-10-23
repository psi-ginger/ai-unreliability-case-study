#include "..\Client\Client.h"
#include "Char\CharServer\Char-Server.h"

#include "..\CharManager\CharManager.h"
#include "..\..\Map\Map.h"
#include "..\..\Order\Order.h"
#include "..\..\Util\Util.h"

extern cltCommonLogic* pclClient;

//-----------------------------------------------------------------------------
//  빈 자리 찾기 함수 
//-----------------------------------------------------------------------------
// 빈자리를 찾아 x, y에 넣어 준다.
BOOL cltCharServer::FindEmptyArea(SHORT& x, SHORT &y)
{
   int i, j;
   SHORT tempx, tempy;

	  for(i=-1;i<=1;i++)
		 for(j=-1;j<=1;j++)
		 {
			 if(j==-1 || j==1 || i==-1 || i==1)
			 {
				 tempx=GetX()+j;
				 tempy=GetY()+i;

				 // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			     if(pclMap->IsInMapArea(tempx, tempy) == true
				 && pclMap->IsEmpty(tempx, tempy))
				{
				   x=tempx;
				   y=tempy;
				   return TRUE;
				}
			 }
		 }
   return FALSE;
}

BOOL cltCharServer::FindEmptyArea(SHORT& x, SHORT &y, SHORT xsize, SHORT ysize)
{
   SHORT i, j;
   int length, min_length=10000;
   SHORT min_x = 0, min_y = 0;
   SHORT tempx, tempy;
   BOOL skipswitch ;

	  for(i=-4;i<=4;i++)
		  for(j=-4;j<=4;j++)
		  {
			  tempx=GetX()+j;
			  tempy=GetY()+i;
			  skipswitch = FALSE;
			  
			  // 캐릭터를 배치할 수 있는 여유가 있는 공간인지 확인한다. 
			  if(pclMap->IsEmpty(tempx, tempy, xsize, ysize, GetX(), GetY())==TRUE)
			  {
				  
				  if(skipswitch == FALSE)
				  {
					  length=(tempx-GetX())*(tempx-GetX())+(tempy-GetY())*(tempy-GetY());
					  
					  if(length < min_length)
					  {
						  min_x=tempx;
						  min_y=tempy;
						  min_length=length;
					  }
				  }
			  }
		  }

		 // 빈자리가 없으면, 
		 if(min_length==10000)
		 {
			 return FALSE;
		 }
		 else  // 빈자리가 있으면, 
		 {
			 x=min_x; y=min_y;
			 return TRUE;
		 }

   //return FALSE;
}



//------------------------------------------------------------------------------
// 인공지능 관련. 
//-----------------------------------------------------------------------------


// 일을 할 준비가 되어 있는가?
BOOL cltCharServer::CanWork()
{

   return TRUE;
}


// 적에 대한 대처 
bool cltCharServer::FindEnemyAction(SI32 id)
{
	
	if(pclCM->IsAlive(id) == FALSE)return false;


	if(CanAttack(id) == TRUE)
	{
		OrderAttack(siID, id, BY_COMPUTER);
		return true;
	}
 
	return false;
}


// 특정 방향으로 캐릭터를 밀어낸다. 
BOOL cltCharServer::Pull(SI32 dir)
{

	SI32 x = 0, y = 0;

	switch(dir)
	{
	case SOUTH:			x = GetX() ;  y = GetY()+1;	break;
	case SOUTH|WEST:	x = GetX()-1; y = GetY()+1;	break;
	case WEST:			x = GetX()-1; y = GetY();		break;
	case WEST|NORTH:    x = GetX()-1; y = GetY()-1;	break;
	case NORTH:         x = GetX() ;  y = GetY()-1;	break;
	case NORTH|EAST:    x = GetX()+1; y = GetY()-1;	break;
	case EAST:          x = GetX()+1; y = GetY();		break;
	case EAST|SOUTH:    x = GetX()+1; y = GetY()+1;	break;
	default:
		MsgBox(TEXT("CharPull"), TEXT("kind:%d dir:%d"), GetKind(), dir);
	}

	// 이동지역이 비어 있다면,
	if(IsColl(x, y, Block) == false)
	{
		SetX(x);
		SetY(y);
	}

	return TRUE;
}




