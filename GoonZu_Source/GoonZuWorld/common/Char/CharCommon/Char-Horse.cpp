//---------------------------------
// 2003/9/15 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"


#include "..\..\Client\Music\Music.h"

//--------------------------
// Common
//-------------------------
#include "PersonInfo.h"

#include "..\Common\Bullet\Bullet.h"
#include "Util\Util.h"


extern cltCommonLogic* pclClient;

// 말을 탄다. 
bool cltCharCommon::RideHorse()
{
	cltHorse* pclhorse = pclCI->clHorseInfo.GetCurrentShowHorse();

	if(pclhorse == NULL || pclhorse->siHorseUnique <= 0)return false;

	//KHY - 1220 - 변신시스템.
	if(clTransFormInfo.bTransFormMode == true ) // 변신 상태면 리턴.
		return false;

	// 타게 된 말의 정보를 획득한다. 
	//pclCI->clHorseInfo.siCurrentHorseIndex	= pclCI->clHorseInfo.siCurrentShowHorse;
	pclCI->clHorseInfo.siCurrentHorseRIDEHORSE	= true;

	return true;
}

// 말에서 내린다.
bool cltCharCommon::UnrideHorse()
{
	//if(pclCI->clHorseInfo.siCurrentHorseIndex	== -1)return false;

	// 타게 된 말의 정보를 획득한다. 
	//pclCI->clHorseInfo.siCurrentHorseIndex	= -1;

	//KHY - 1220 - 변신시스템.
	if(clTransFormInfo.bTransFormMode == true ) // 변신 상태면 리턴.
		return false;
	
	//KHY - 1001 - 기승동물 5마리 보유 수정.
	if(pclCI->clHorseInfo.siCurrentHorseRIDEHORSE	== false)return false;


	pclCI->clHorseInfo.siCurrentHorseRIDEHORSE	=  false;

	return true;
}

// 말을 가지고 있는가?
bool cltCharCommon::HaveHorse()
{
	return pclCI->clHorseInfo.HaveHorse();
}

