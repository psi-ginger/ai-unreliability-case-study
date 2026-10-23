#include "..\Client\Client.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "..\Client\Music\Music.h"

#include "bullet.h"
#include "..\Smoke\Smoke.h"
#include "Util\Util.h"


extern cltCommonLogic* pclClient;

cltBulletKindInfo::cltBulletKindInfo()
{


}

cltBulletKindInfo::~cltBulletKindInfo()
{
	if(Spr.pImage)
	{
		GP.FreeSpr(Spr);
	}

	if(LightSpr.pImage)
	{
		GP.FreeSpr(LightSpr);
	}
	
}


void cltBulletKindInfo::Init(SHORT stepwidth, SHORT chaseswitch, SHORT curveswitch, 
							SI32 attacktype, 
							SI32 BulletAtb, SI32 BulletSmokeAtb, SI32 sidelay,
							SI32 firesmoke, SI32 hitsmoke, 
							cltBulletSoundInfo	*pclSoundInfo,
							cltBulletLayerInfo* pcllayerinfo,
							cltBulletDrawInfo* pclDrawInfo,
							SI32 hitegroundsmoke
							)
{
	StepWidth=stepwidth;

	
	memset(szFileName,		0, MAX_BULLET_FILE_NAME_LENGTH);
	memset(szLightFileName, 0, MAX_BULLET_FILE_NAME_LENGTH);

	if (pclDrawInfo->m_strFileName != NULL)
		MStrCpy(szFileName,		pclDrawInfo->m_strFileName,		 MAX_BULLET_FILE_NAME_LENGTH );

	if (pclDrawInfo->m_strLightFileName != NULL)
		MStrCpy(szLightFileName,pclDrawInfo->m_strLightFileName, MAX_BULLET_FILE_NAME_LENGTH );

	// JAYCEE
	/*
	if(GP.LoadSpr(szFileName, &Spr) == FALSE)
	{
		MsgBox(TEXT("cltBulletKindInfo::Init()"), TEXT("fdad98j:%s"), szFileName);
	}
	if(_tcslen(szLightFileName) && GP.LoadSpr(szLightFileName, &LightSpr) == FALSE)
	{
		MsgBox(TEXT("cltBulletKindInfo::Init()"), TEXT("LightFile"), szLightFileName);
	}
	*/
	 

	Font=pclDrawInfo->m_StartFont;
	
	siMoveFrameNumber = pclDrawInfo->m_EachDirectionFrameNum;

	bDirSwitch		= pclDrawInfo->m_bDirectionSwitch;
	TailSmoke		= pclDrawInfo->m_siTailSmoke;
	ChaseSwitch		= chaseswitch;
	CurveSwitch		= curveswitch;
	TransparentSwitch=pclDrawInfo->m_TransParentMode;
	m_siLightMode	= pclDrawInfo->m_siLightMode;
	m_siIndent		= pclDrawInfo->m_siIndent;
	m_siLoopStartFont = pclDrawInfo->m_siLoopStartFont;
	clBulletadjust  = pclDrawInfo->clBulletadjust;

	//------------------------------
	// 공격 타입을 지정한다. 
	//------------------------------
	siAttackType	= attacktype;

	siAtb			= BulletAtb;
	siSmokeAtb		= BulletSmokeAtb;
	siDelay			= sidelay;

	// 연기 
	siFireSmoke		= firesmoke;
	siHitSmoke		= hitsmoke;
	siHiteGroundSmoke = hitegroundsmoke;

	// 공격시 효과음 
	siFireEffect	= pclSoundInfo->GetAttackSoundIndex();

	// 피격시 효과음
	siHitEffect		= pclSoundInfo->GetHitSoundIndex();

	clLayerInfo.Set(pcllayerinfo);
}

void cltBulletKindInfo::LoadSpr()
{
	if(Spr.pImage == NULL)
	{
		if(_tcslen(szFileName) && GP.LoadSprFromFile(szFileName, &Spr) == FALSE)
		{
			if ( pclClient->GameMode == GAMEMODE_CLIENT )
			{
				MsgBox(TEXT("cltBulletKindInfo::Init()"), TEXT("fdad98j:%s"), szFileName);
			}
		}
	}

	if(LightSpr.pImage == NULL)
	{
		if(_tcslen(szLightFileName) && GP.LoadSprFromFile(szLightFileName, &LightSpr) == FALSE)
		{
			if ( pclClient->GameMode == GAMEMODE_CLIENT )
			{
				MsgBox(TEXT("cltBulletKindInfo::Init()"), TEXT("LightFile:%s"), szLightFileName);
			}
		}
	}
}