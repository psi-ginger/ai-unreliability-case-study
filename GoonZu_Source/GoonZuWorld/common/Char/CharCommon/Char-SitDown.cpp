#include "..\Common\CommonHeader.h"
#include "Char-Common.h"
#include "..\Common\Order\Order.h"
#include "..\Common\Util\Util.h"
#include "../CharAni/CharAni.h"

bool cltCharCommon::SitDown(SI32 SitDownAnytype)
{	
	SetAnimation( SitDownAnytype );
	DoAnimationDelay();

	if(clAniInfo.GetAniStep() == 0 )//&& clAniInfo.siRetryNum > 0)
	{
		bHitDoneSwitch	= false;
		bBusySwitch		= false;

		return true;
	}
	else 
		return false;

//	return true;
}