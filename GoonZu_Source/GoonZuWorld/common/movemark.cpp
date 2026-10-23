#include "..\Client\Client.h"



#include "MoveMark.h"


extern cltCommonLogic* pclClient;

_MoveMark::_MoveMark(cltMapClient* pclmap)
{
	pclMap = pclmap;

	X=Y=0;
	MoveMarkSwitch=FALSE;
	Step=0;
	MaxStep = 13;
}

_MoveMark::~_MoveMark()
{
}


void _MoveMark::SetMoveMark(SHORT x, SHORT y)
{
    MoveMarkSwitch = TRUE;
	Step = 0;
	X=x; Y=y;

	pclMap->GetXYDotFromXYCord(X, Y, RealX, RealY);
}

// real 좌표로 설정한다. 
void _MoveMark::SetMoveMarkReal(SHORT x, SHORT y)
{
    MoveMarkSwitch=TRUE;
	Step=0;
	RealX=x; RealY=y;

}


void _MoveMark::Action()
{
    if(MoveMarkSwitch==FALSE)return ;
	
   if(Step<(MaxStep-1))Step++;
   else 
   {
	   MoveMarkSwitch=FALSE;
   }
}

// lock하지 않고 그린다. 
void _MoveMark::DrawMoveMark()
{
	if(MoveMarkSwitch==FALSE)return ;
	
	TSpr* pclspr = pclClient->GetGlobalSpr(GIMG_CLICKPOINT);
	if(pclspr == NULL)return ;

	int font = Step;

	SHORT xsize = pclspr->GetXSize();
	SHORT ysize = pclspr->GetYSize();
	SHORT x=0;
	SHORT y=0;

	// 절대좌표를 화면좌표로 옮긴다. 
	pclMap->TransFromRealDotToScreenDot(RealX, RealY, x, y);

	x-=(xsize/2);
	y-=(ysize/2)+20;

	GP.PutSpr(pclspr,x, y, font);
	GP.PutSprLight1(pclspr, x, y, font+13);
	//Draw_PutSprLightImjinT(x, y, xsize, ysize, &GameSpr[file], font);	
	
}



