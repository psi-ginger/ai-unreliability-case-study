#include "..\Client\Client.h"

//#include "..\Client\ColorTable\ColorTable.h"
#include "..\Client\Music\Music.h"

#include "bullet.h"
#include "..\Smoke\Smoke.h"
#include "Util\Util.h"

extern cltCommonLogic* pclClient;

#define FRAMESTEP 1
#define SMALLBULLETSTEP 3

cltBulletClient::cltBulletClient() : cltBulletCommon()
{
	SI32 i;

	//---------------------------------
	// 출력 옵션. 
	//---------------------------------
	bDrawSwitch		= true;
	bReverseSwitch	= false;

	siFont			= 0;
	pSpr			= NULL;	
	pLightSpr		= NULL;

	siAnimation		= 0;
	siAniStep		= 0;

	// 이동관련 
	MoveFrameNumber	= 0;

	for(i = 0;i < 9;i++)
	{
		pMoveFileSpr[i]	= NULL;
		MoveFont[i] = 0;
	}

	DrawStartX	=	DrawStartY	= 0;      // 그림이 그려지는 시작 위치 
	LightDrawStartX		=	LightDrawStartY	= 0;      // 그림이 그려지는 시작 위치 

	CurveBulletDrawX	= CurveBulletDrawY = 0;

	TailSmoke		= 0;

	uiAdjustFrame	= 0;
	AdjustPosX		= 0;
	AdjustPosY		= 0;

	DecideDir();

}

void cltBulletClient::Init()
{

	//---------------------------------
	// 출력 옵션. 
	//---------------------------------
	bDrawSwitch		=	true;
	bReverseSwitch	=	false;

	CurveBulletDrawX=	0;
	CurveBulletDrawY=	0;

	siFont				=	pclBKI->Font;
	TailSmoke			=	pclBKI->TailSmoke;

	pSpr		=	(TSpr*)&pclBKI->Spr;
	pLightSpr	=	(TSpr*)&pclBKI->LightSpr;

	uiAdjustFrame		= 0;
	siAniStep			= 0;

	SetFileNFont();

	// 발사시 연기가 나오는 것이라면 연기를 만든다. 
	if(pclBKI->siFireSmoke)
	{
		pclClient->pclSmokeManager->SetSmoke(pclBKI->siFireSmoke, clRealSrcPos.siX, clRealSrcPos.siY, pclMap);
	}

	// 발사시 효과음이 있는 것이라면 효과음을 연주한다.
	if(pclBKI->siFireEffect)
	{
		pclClient->PushEffect(pclBKI->siFireEffect, MapSX, MapSY );
	}

	DecideDir();

}


void cltBulletClient::SetFileNFont()
{
	int i;


	// 이동용 화일과 폰트 
	MoveFrameNumber = pclBKI->siMoveFrameNumber;


	// 각 방향별로 그림이 준비되어 있다. 
	if(pclBKI->bDirSwitch == TRUE)
	{
		SetMoveImage16(pSpr, pclBKI->Font, MoveFrameNumber);

	}
	// 모든 방향이 같은 그림을 사용한다. 
	else
	{
		for(i=0;i<9;i++)
			SetMoveImage(i, pSpr, pclBKI->Font);
	}

}


void cltBulletClient::CalculateDrawStartXY()
{
	SHORT screenx=0, screeny=0;

	cltMapClient* pclmap = (cltMapClient*)pclMap;

	if(pclmap)
	{
		pclmap->TransFromRealDotToScreenDot(clRealSrcPos.siX, clRealSrcPos.siY, screenx, screeny);
	}

	if (AdjustPosX > 0)
	{
		if (TABS(AdjustPosX) > uiAdjustFrame)
			AdjustPosX = AdjustPosX - uiAdjustFrame;
		else
			AdjustPosX = 0;
	}
	else
	{
		if (TABS(AdjustPosX) > uiAdjustFrame)
			AdjustPosX = AdjustPosX + uiAdjustFrame;
		else
			AdjustPosX = 0;
	}
	if (AdjustPosY > 0)
	{
		if (TABS(AdjustPosY) > uiAdjustFrame)
			AdjustPosY = AdjustPosY - uiAdjustFrame;
		else
			AdjustPosY = 0;
	}
	else
	{
		if (TABS(AdjustPosY) > uiAdjustFrame)
			AdjustPosY = AdjustPosY + uiAdjustFrame;
		else
			AdjustPosY = 0;
	}
	uiAdjustFrame = uiAdjustFrame + FRAMESTEP;

	if(pSpr)
	{
		//DrawStartX=screenx-pSpr->GetXSize() /2 + AdjustPosX;
		//DrawStartY=screeny-pSpr->GetYSize() /2 + pclBKI->m_siIndent + AdjustPosY;

		DrawStartX=screenx-pSpr->GetXSize() /2 + AdjustPosX;
		DrawStartY=screeny-pSpr->GetYSize() /2 + AdjustPosY;

	}

	if(pLightSpr)
	{
		//LightDrawStartX		 = screenx-pLightSpr->GetXSize() /2 + AdjustPosX;
		//LightDrawStartY		 = screeny-pLightSpr->GetYSize() /2+32  + pclBKI->m_siIndent + AdjustPosY;

		LightDrawStartX		 = screenx-pLightSpr->GetXSize() /2 + AdjustPosX;
		LightDrawStartY		 = screeny-pLightSpr->GetYSize() /2+32 + AdjustPosY;
	}


	if(CurveSwitch==TRUE)
	{
		pclmap->TransFromRealDotToScreenDot(CurveBulletRealX, CurveBulletRealY, screenx, screeny);
		CurveBulletDrawX=screenx-pSpr->GetXSize() /2;
		CurveBulletDrawY=screeny-pSpr->GetYSize() /2;
	}

}


BOOL cltBulletClient::Draw()
{	
	if(pclClient->pclMapManager->pclCurrentMap != pclMap)return FALSE;

	// 출력과 관계된 계산을 수행한다. 
	CalculateDrawStartXY();

	DecideFont();

	// 어둔 단계를 설정한다. 
	SHORT darkstep = 0;

	if(bDrawSwitch == false)return FALSE;

	//------------------------------------------
	// 총알의 광원찍기
	//------------------------------------------
	if(pLightSpr)
	{
		switch(pclBKI->m_siLightMode)
		{
		case TRANSPARENT_MODE_OLDTRANS:
			GP.PutSpr(pLightSpr,LightDrawStartX, LightDrawStartY,0, bReverseSwitch);		break;
		case TRANSPARENT_MODE_HALFTRANS:
			GP.PutSprLight(pLightSpr, LightDrawStartX, LightDrawStartY, 0, bReverseSwitch);		break;
		case TRANSPARENT_MODE_SCREEN:
			GP.PutSprScreen(pLightSpr, LightDrawStartX, LightDrawStartY, 0, bReverseSwitch);		break;
		case TRANSPARENT_MODE_MULTIPLY:
			GP.PutSprMultiply(pLightSpr, LightDrawStartX, LightDrawStartY, 0, bReverseSwitch);	break;
		case TRANSPARENT_MODE_ADD:
			GP.PutSprAdd(pLightSpr, LightDrawStartX, LightDrawStartY, 0, bReverseSwitch);			break;
		default:	
			GP.PutSpr(pLightSpr, LightDrawStartX, LightDrawStartY, 0, bReverseSwitch);			break;
		}
	}


	//------------------------------------------
	// 
	//------------------------------------------
	if(pSpr)
	{
		SI32 siBuletDrawX = 0, siBuletDrawY = 0;

		if(CurveSwitch==TRUE)
		{
			siBuletDrawX = CurveBulletDrawX;
			siBuletDrawY = CurveBulletDrawY;
		}
		// 곡선 탄도가 아닐때 
		else
		{
			siBuletDrawX = DrawStartX;
			siBuletDrawY = DrawStartY;
		}

		switch(pclBKI->TransparentSwitch)
		{
		case TRANSPARENT_MODE_OLDTRANS:
			GP.PutSprLight1(pSpr,siBuletDrawX, siBuletDrawY,siFont, bReverseSwitch);		break;
		case TRANSPARENT_MODE_HALFTRANS:
			GP.PutSprLight(pSpr, siBuletDrawX, siBuletDrawY, siFont, bReverseSwitch);		break;
		case TRANSPARENT_MODE_SCREEN:
			GP.PutSprScreen(pSpr, siBuletDrawX, siBuletDrawY, siFont, bReverseSwitch);	break;
		case TRANSPARENT_MODE_MULTIPLY:
			GP.PutSprMultiply(pSpr, siBuletDrawX, siBuletDrawY, siFont, bReverseSwitch);	break;
		case TRANSPARENT_MODE_ADD:
			GP.PutSprAdd(pSpr, siBuletDrawX, siBuletDrawY, siFont, bReverseSwitch);		break;
		default:	
			GP.PutSpr(pSpr, siBuletDrawX, siBuletDrawY, siFont, bReverseSwitch);			break;
		}
	}

	// 레이어 출력( 기본 이미지위에 또 출력할 시에 )
	if ( pclBKI->clLayerInfo.siFont != -1 )
	{
		SI32 siBuletDrawX = 0, siBuletDrawY = 0;

		if(CurveSwitch==TRUE)
		{
			siBuletDrawX = CurveBulletDrawX;
			siBuletDrawY = CurveBulletDrawY;
		}
		// 곡선 탄도가 아닐때 
		else
		{
			siBuletDrawX = DrawStartX;
			siBuletDrawY = DrawStartY;
		}
		if ( pclBKI->clLayerInfo.bTransMode )
		{
			GP.PutSprLight1(pSpr, siBuletDrawX, siBuletDrawY, pclBKI->clLayerInfo.siFont + siFont, bReverseSwitch);
		}
		else
		{
			GP.PutSpr(pSpr, siBuletDrawX, siBuletDrawY, pclBKI->clLayerInfo.siFont+ siFont, bReverseSwitch);
		}
	}

	return TRUE;
}


void cltBulletClient::DecideFontMove()
{

	switch(TempDirection)
	{
	case SOUTH:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[2];
		siFont	=	MoveFont[2] + siAniStep;
		break;
	case SOUTH|WEST:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[4];
		siFont	=	MoveFont[4] + siAniStep;
		break;
	case WEST:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[6];
		siFont	=	MoveFont[6]+ siAniStep;
		break;
	case NORTH|WEST:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[8];
		siFont	=	MoveFont[8]+ siAniStep;
		break;
	case NORTH:
		bReverseSwitch	=	true;
		pSpr	=	pMoveFileSpr[6];
		siFont	=	MoveFont[6]+ siAniStep;
		break;
	case NORTH|EAST:
		bReverseSwitch	=	true;
		pSpr	=	pMoveFileSpr[4];
		siFont	=	MoveFont[4]+ siAniStep;
		break;
	case EAST:
		bReverseSwitch	=	true;
		pSpr	=	pMoveFileSpr[2];
		siFont	=	MoveFont[2]+ siAniStep;
		break;
	case EAST|SOUTH:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[0];
		siFont	=	MoveFont[0]+ siAniStep;
		break;
	case SSW:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[3];
		siFont	=	MoveFont[3]+ siAniStep;
		break;
	case EEN:
		bReverseSwitch	=	true;
		pSpr	=	pMoveFileSpr[3];
		siFont	=	MoveFont[3]+ siAniStep;
		break;
	case WWS:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[5];
		siFont	=	MoveFont[5]+ siAniStep;
		break;
	case NNE:
		bReverseSwitch	=	true;
		pSpr	=	pMoveFileSpr[5];
		siFont	=	MoveFont[5]+ siAniStep;
		break;
	case WWN:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[7];
		siFont	=	MoveFont[7]+ siAniStep;
		break;
	case NNW:
		bReverseSwitch	=	true;
		pSpr	=	pMoveFileSpr[7];
		siFont	=	MoveFont[7]+ siAniStep;
		break;
	case SSE:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[1];
		siFont	=	MoveFont[1]+ siAniStep;
		break;
	case EES:
		bReverseSwitch	=	true;
		pSpr	=	pMoveFileSpr[1];
		siFont	=	MoveFont[1]+ siAniStep;
		break;
	default:
		bReverseSwitch	=	false;
		pSpr	=	pMoveFileSpr[2];
		siFont	=	MoveFont[2]+ siAniStep;

		pclClient->pclMessage->SetMsg(TEXT("Invalid Bullet Image"));

		break;
	}

}

void cltBulletClient::DecideDir()
{
	if ( OldTargetDirection == 0 )
	{
		switch(	TargetDirection )
		{
		case SOUTH:
			AdjustPosX = pclBKI->clBulletadjust.clAdjustPos[1].siX;
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[1].siY;
			break;
		case SOUTH|WEST:
			AdjustPosX = pclBKI->clBulletadjust.clAdjustPos[2].siX;
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[2].siY;
			break;
		case WEST:
			AdjustPosX = pclBKI->clBulletadjust.clAdjustPos[3].siX;
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[3].siY;
			break;
		case NORTH|WEST:
			AdjustPosX = pclBKI->clBulletadjust.clAdjustPos[4].siX;
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[4].siY;
			break;
		case NORTH:
			AdjustPosX = -(pclBKI->clBulletadjust.clAdjustPos[3].siX);
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[3].siY;
			break;
		case NORTH|EAST:
			AdjustPosX = -(pclBKI->clBulletadjust.clAdjustPos[2].siX);
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[2].siY;
			break;
		case EAST:
			AdjustPosX = -(pclBKI->clBulletadjust.clAdjustPos[1].siX);
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[1].siY;
			break;
		case EAST|SOUTH:
			AdjustPosX = pclBKI->clBulletadjust.clAdjustPos[0].siX;
			AdjustPosY = pclBKI->clBulletadjust.clAdjustPos[0].siY;
			break;
		}

		OldTargetDirection = TargetDirection;

	}

}


void cltBulletClient::DecideFont()
{
	switch(siAnimation)
	{
	case ANI_BULLET_MOVE:
		DecideFontMove();
		break;
	default:
		break;
	}
}


void cltBulletClient::AniStepAction()
{
	if(max(0, pclClient->CurrentClock / 50) % 2 == 0)
	{

		if (pclBKI->m_siLoopStartFont > 0)
		{
			// 블렛이 작을때는 SMALLBULLETSTEP 프레임동안 그대로 나간다.
			if (uiAdjustFrame / FRAMESTEP >= pclBKI->m_siLoopStartFont * SMALLBULLETSTEP)
			{
				// 루핑 설정이 되어있으면 루핑스타트 위치부터 루핑되도록
				siAniStep = (siAniStep + 1) % (MoveFrameNumber - pclBKI->m_siLoopStartFont) + pclBKI->m_siLoopStartFont;
			}
			else
			{
				//siAniStep = (siAniStep + ((uiAdjustFrame / FRAMESTEP) / 5)) % MoveFrameNumber;
				siAniStep = (uiAdjustFrame / FRAMESTEP) / SMALLBULLETSTEP;
			}
		}
		else
		{
			if( MoveFrameNumber == 0 )
			{
				siAniStep = 0;
			}
			else
			{
				siAniStep = (siAniStep + 1) % MoveFrameNumber;
			}
		}
	}

}

void cltBulletClient::SetMoveImage16(TSpr* pspr, SHORT font, SHORT frame)
{
	SHORT i;

	for(i=0;i<9;i++)
		SetMoveImage(i, pspr, font+frame*i);
}

void cltBulletClient::SetMoveImage(SHORT ani, TSpr* pspr, SHORT font)
{
	pMoveFileSpr[ani]	=	pspr;
	MoveFont[ani]		=	font;
}


//꼬리 연기를 만든다. 
void cltBulletClient::MakeTail(SI32 tempdirection)
{

	// 2003.10.29
	if( TailSmoke == 0 )
		return;


	SI32 smokex, smokey;	


	if(CurveSwitch==TRUE)
	{
		smokex=CurveBulletRealX;
		smokey=CurveBulletRealY;
	}
	else
	{
		smokex = clRealSrcPos.siX;
		smokey = clRealSrcPos.siY;
	}


	// 2003.10.29
	// 꼬리 연기 보여주는 부분 추가
	if(pclBKI->TailSmoke)
	{
		pclClient->pclSmokeManager->SetSmoke( pclBKI->TailSmoke, smokex, smokey, pclMap,tempdirection );
	}




}

// 피격 연기를 만든다. 
void cltBulletClient::MakeHitSmoke()
{
	// 피격시 연기가 나오는 것이라면 연기를 만든다. 
	if(pclBKI->siHitSmoke)
	{
		pclClient->pclSmokeManager->SetSmoke(pclBKI->siHitSmoke, clRealSrcPos.siX, clRealSrcPos.siY, pclMap,0,pclBKI->siHiteGroundSmoke);
	}

}

void cltBulletClient::MakeHitSound()
{
	if ( pclBKI->siHitEffect )
	{
		pclClient->PushEffect( pclBKI->siHitEffect, MapSX, MapSY );
	}
}