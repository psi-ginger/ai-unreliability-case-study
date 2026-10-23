#include "..\Client\Client.h"

//#include "..\..\Client\ColorTable\ColorTable.h"


#include "smoke.h"
#include "Char\CharAni\CharAni.h"
#include "Map\Map.h"


extern cltCommonLogic* pclClient;

cltSmokeClient::cltSmokeClient()
{
	StartFont	= 0;
	pSpr		= NULL;
	Font		= 0;
	TransparentMode	= 0;
	DrawSwitch	= true;

	ReverseSwitch	= false;
	DrawStartX		= 0;
	DrawStartY		= 0;
	siAlphaValue	= 0;
}


void cltSmokeClient::Init()
{
	StartFont			=	pclSKI->StartFont;
	pSpr				=	&pclSKI->Spr;
    MoveFrameNumber		=	pclSKI->MoveFrameNumber;
	TransparentMode		=	pclSKI->TransparentMode;
	DrawSwitch			=	true;
	DirectionMode		=	pclSKI->bDirection;
	siAlphaValue		=	pclSKI->siAlpha;
}


void cltSmokeClient::DecideFont()
{
	switch(Animation)
	{
	case ANITYPE_EXP:
		{
			if ( !DirectionMode )
			{
				Font=StartFont+Step;
			}
			else if ( DirectionMode )
			{
				DecideFontMove();
			}
		}
		break;
	default:
		break;
	}
	
}

void cltSmokeClient::DecideFontMove()
{
	
	switch(TempDirection)
	{
	case SOUTH:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 2 + Step;
		break;
	case SOUTH|WEST:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 4 + Step;
		break;
	case WEST:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 6 + Step;
		break;
	case NORTH|WEST:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 8 + Step;
		break;
	case NORTH:
		ReverseSwitch	=	true;
		Font	=	MoveFrameNumber * 6 + Step;
		break;
	case NORTH|EAST:
		ReverseSwitch	=	true;
		Font	=	MoveFrameNumber * 4 + Step;
		break;
	case EAST:
		ReverseSwitch	=	true;
		Font	=	MoveFrameNumber * 2 + Step;
		break;
	case EAST|SOUTH:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 0 + Step;
		break;
	case SSW:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 3 + Step;
		break;
	case EEN:
		ReverseSwitch	=	true;
		Font	=	MoveFrameNumber * 3 + Step;
		break;
	case WWS:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 5 + Step;
		break;
	case NNE:
		ReverseSwitch	=	true;
		Font	=	MoveFrameNumber * 5 + Step;
		break;
	case WWN:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 7 + Step;
		break;
	case NNW:
		ReverseSwitch	=	true;
		Font	=	MoveFrameNumber * 7 + Step;
		break;
	case SSE:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber + Step;
		break;
	case EES:
		ReverseSwitch	=	true;
		Font	=	MoveFrameNumber + Step;
		break;
	default:
		ReverseSwitch	=	false;
		Font	=	MoveFrameNumber * 2 + Step;
		
		pclClient->pclMessage->SetMsg(TEXT("Invalid Smoke Image"));
		
		break;
	}
}

BOOL cltSmokeClient::Draw()
{
	if(pclClient->pclMapManager->pclCurrentMap != pclMap)return FALSE;
	if(DrawSwitch==false)return false;
	
	// 출력과 관계된 계산을 수행한다. 
	CalculateDrawStartXY();
	DecideFont();
	
	switch(TransparentMode)
	{
	case TRANSPARENT_MODE_OLDTRANS:
		GP.PutSprLight1(pSpr, DrawStartX, DrawStartY,Font, ReverseSwitch);	break;
	case TRANSPARENT_MODE_HALFTRANS:
		GP.PutSprLight(pSpr, DrawStartX, DrawStartY, Font, ReverseSwitch);	break;
	case TRANSPARENT_MODE_SCREEN:
		GP.PutSprScreen(pSpr, DrawStartX, DrawStartY, Font, ReverseSwitch);   break;
	case TRANSPARENT_MODE_MULTIPLY:
		GP.PutSprMultiply(pSpr, DrawStartX, DrawStartY, Font, ReverseSwitch);	break;
	case TRANSPARENT_MODE_ADD:
		GP.PutSprAdd(pSpr, DrawStartX, DrawStartY, Font, ReverseSwitch);		break;
	case TRANSPARENT_MODE_ALPHA:
		GP.PutSprAlpha(pSpr, DrawStartX, DrawStartY, siAlphaValue, Font, ReverseSwitch);		break;
	default:
		GP.PutSpr(pSpr, DrawStartX, DrawStartY, Font, ReverseSwitch);			break;
	}

	return TRUE;
}


void cltSmokeClient::CalculateDrawStartXY()
{
	SHORT screenx=0, screeny=0;

	cltMapClient* pclmap = (cltMapClient*)pclMap;

	pclmap->TransFromRealDotToScreenDot(RealX, RealY, screenx, screeny);
	DrawStartX=screenx-pSpr->GetXSize() /2;
	DrawStartY=screeny-pSpr->GetYSize() /2-pclSKI->siYIndent;

}


