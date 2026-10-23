//---------------------------------
// 2003/7/25 김태곤
//---------------------------------
#include "Logo.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "../Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "Client.h"

extern cltCommonLogic* pclClient;
cltLogo::cltLogo()
{
	// 필요한 파일을 불러온다. 
	GP.LoadSprFromFile(TEXT("Logo\\Logo.Spr"), &Spr);
}

cltLogo::~cltLogo()
{
	if(Spr.pImage)
	{
		GP.FreeSpr(Spr);
	}
}


void cltLogo::Draw(SI32 screenxsize, SI32 screenysize, SI32 rx, SI32 ry )
{
	cltClient *pclclient = (cltClient *)pclClient;

	/*
	GP.PutSpr(&Spr, pclclient->GetScreenXSize() - Spr.GetXSize() + rx, pclclient->GetScreenYSize() - Spr.GetYSize() + ry, 0);
	*/
}