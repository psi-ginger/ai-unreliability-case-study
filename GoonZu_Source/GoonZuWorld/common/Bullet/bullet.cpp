#include "..\Client\Client.h"

#include "bullet.h"
#include "..\Smoke\Smoke.h"
#include "Util\Util.h"

extern cltCommonLogic* pclClient;

cltBulletServer::cltBulletServer() : cltBulletCommon()
{
	HitIDIndex	= 0;

}


void cltBulletServer::Init()
{
	SI32 i;

	HitIDIndex	=	0;
    for(i=0;i<MAX_HIT_ID;i++)
	   HitID[i]= 0;

}


//
// 한번 공격한 캐릭터를 다시 공격하지 않기 위해서.
// 스택을 초기화한다. 
//

void cltBulletServer::InitPushID()
{
	HitIDIndex=0;
}

//
// id = 총알의 id
// 
//

void cltBulletServer::PushHitID(SI32 id)
{

	HitID[HitIDIndex]=id;

	if(HitIDIndex<(MAX_HIT_ID-1))
	   HitIDIndex++;
}

//
// id = 총알의 id
// 총알이 목표에 맞았나?
//

BOOL cltBulletServer::IsAlreadyPushID(SI32 id)
{
	int i=0;

	for(i=0;i<HitIDIndex;i++)
	{
		if(HitID[i] == id )return TRUE;
	}

	return FALSE;
}


