#include "..\Client\Client.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "smoke.h"
#include "Char\CharAni\CharAni.h"
#include "Map\Map.h"


cltSmokeKindInfo::cltSmokeKindInfo()
{
}

cltSmokeKindInfo::~cltSmokeKindInfo()
{
	if(Spr.pImage)
	{
		GP.FreeSpr(Spr);
	}
}

void cltSmokeKindInfo::Init(TCHAR *filename, SHORT startfont, SHORT moveframenumber, SHORT maxrepeatnumber, SI32 siStepDelay, SHORT risingswitch, SI32 transparentmode, SI08 Alpha, SI32 siyindent,bool direction)
{
	
	MStrCpy(szFileName, filename, MAX_SMOKE_FILE_NAME_LENGTH );


	StartFont=startfont;
	MoveFrameNumber=moveframenumber;
	MaxRepeatNumber=maxrepeatnumber;
	RisingSwitch=risingswitch;
	TransparentMode	=	transparentmode;
	m_siStepDelay	=	siStepDelay;
	siAlpha			=	Alpha;
	siYIndent = siyindent;
	bDirection = direction;
	
}

void cltSmokeKindInfo::LoadSpr()
{
	if(Spr.pImage == NULL)
	{
		if(GP.LoadSprFromFile(szFileName, &Spr) == FALSE)
		{
			MsgBox(TEXT("cltSmokeKindInfo::Init()"), TEXT("fd98j:%s"), szFileName);
		}
	}
}