//---------------------------------
// 2003/9/27 김태곤
//---------------------------------

#ifndef _HEALTH_H
#define _HEALTH_H

#include "../../CommonHeader.h"

#include "..\Disease\Disease.h"

#define HEALTH_ACTION_DELAY		60000	// 60초에 한번씩만 Action을 행한다.



#define HEALTH_STATUS_HUNGRY_1		0x1
#define HEALTH_STATUS_HUNGRY_2		0x2
#define HEALTH_STATUS_HUNGRY_3		0x4
#define HEALTH_STATUS_HUNGRY_4		0x8
#define HEALTH_STATUS_HUNGRY_5		0x10	// 가장 중간 상태. 
#define HEALTH_STATUS_HUNGRY_6		0x20
#define HEALTH_STATUS_HUNGRY_7		0x40
#define HEALTH_STATUS_HUNGRY_8		0x80
#define HEALTH_STATUS_HUNGRY_9		0x100
#define HEALTH_STATUS_HUNGRY_NORMAL	0x200	// 가장 중간 상태. 


#define HEALTH_STATUS_ORG1_SERIOUS		0x1000
#define HEALTH_STATUS_ORG1_BAD			0x2000
#define HEALTH_STATUS_ORG1_NORMAL		0x4000
#define HEALTH_STATUS_ORG1_GOOD			0x8000

#define HEALTH_STATUS_ORG2_SERIOUS		0x10000
#define HEALTH_STATUS_ORG2_BAD			0x20000
#define HEALTH_STATUS_ORG2_NORMAL		0x40000
#define HEALTH_STATUS_ORG2_GOOD			0x80000

#define HEALTH_STATUS_ORG3_SERIOUS		0x100000
#define HEALTH_STATUS_ORG3_BAD			0x200000
#define HEALTH_STATUS_ORG3_NORMAL		0x400000
#define HEALTH_STATUS_ORG3_GOOD			0x800000

#define HEALTH_STATUS_ORG4_SERIOUS		0x1000000
#define HEALTH_STATUS_ORG4_BAD			0x2000000
#define HEALTH_STATUS_ORG4_NORMAL		0x4000000
#define HEALTH_STATUS_ORG4_GOOD			0x8000000


#define MAXORG(level)	(25 + (level) / 2)			// 레벨에 따른 기관의 최대 허용 범위. 

//----------------------------------
// 현재 가지고 있는 질병의 정보. 
//----------------------------------
class cltCurDisease{
public:
	SI16 siOrg1Unique;
	SI16 siOrg2Unique;
	SI16 siOrg3Unique;
	SI16 siOrg4Unique;

	cltCurDisease()
	{
		siOrg1Unique	= 0;
		siOrg2Unique	= 0;
		siOrg3Unique	= 0;
		siOrg4Unique	= 0;
	}

	// 주어진 질병을 가지고 있는가 ?
	bool IsIn(SI32 diseaseunique)
	{
		if(siOrg1Unique == diseaseunique)return true;
		if(siOrg2Unique == diseaseunique)return true;
		if(siOrg3Unique == diseaseunique)return true;
		if(siOrg4Unique == diseaseunique)return true;

		return true;
	}
};


//-----------------------------------
// 건강 클래스. 
//-----------------------------------
class cltHealth
{
public:
	cltHealthInfo* pclInfo;
	DWORD ActionClock;			// Action을 행한 시간. 
	
	cltHealth();

	bool Action(SI32 level, DWORD CurrentClock);

	// 건강 상태를 분석해서 그 결과를 돌려준다. 
	SI32 AnalyzeHealth(SI32 level);

	// 현재의 건강 상태에 대한 Text를 구한다.
	bool GetHealthText(SI32 level, TCHAR* text, SI16 txtSize);

	// 배를 채운다.
	void IncreaseHungry(UI32 amount)
	{
		SI32 hungry = pclInfo->siHungry + amount;
		if(hungry > 300)
		{
			hungry = 300;
		}

		pclInfo->siHungry = hungry;

	}	

	// 배고픔을 증가시킨다. 
	void DecreaseHungry(SI32 amount)
	{
		pclInfo->siHungry -= amount;

		if(pclInfo->siHungry < -300)
		{
			pclInfo->siHungry = -300;
		}
	}
	
	// 현재 가지고 있는 배고품 수치를 보낸다.
	SI16 GetHungry()
	{
		return pclInfo->siHungry;
	}

	// 현재 가지고 있는 질병의 정보를 알려준다. 
	bool GetCurDisease(cltCurDisease* pclcurdisease);

	// 질병을 치료한다. 
	bool CureDisease(SI32 diseaseunique);
	// 현재 질병의 이름을 구한다. 
	bool GetCurDiseaseName(TCHAR* pname, SI16 txtSize);

	

	// 배고픔 상수를 찾는다. 
	SI32 GetHungryStatus(SI32 level);
	SI32 GetHungryMaxLimit(SI32 level);
	SI32 GetBasic(SI32 level);


};



#endif
