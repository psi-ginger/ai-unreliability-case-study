//---------------------------------
// 2003/11/19 김태곤
//---------------------------------

#include <CommonHeader.h>
#include "Disease.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

//---------------------------------------------------------------
// 질병 
//---------------------------------------------------------------
cltDiseaseManager::cltDiseaseManager()
{
	SI32 i;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		pclDisease[i] = NULL;
	}

	// 유니크, 어느계질환인지, 질병에걸릴수있는정도치, 해악, 해당질병이름, 질병타입
	TCHAR* pText = GetTxtFromMgr(2824);
	Add(100,	ORG_1, 1,		10,		pText,	"DISEASE_DYSPEPSIA");			// 소화제
	pText = GetTxtFromMgr(2825);
	Add(200,	ORG_1, 10,		15,		pText,	"DISEASE_GASTRITIS");		// 위염약
	pText = GetTxtFromMgr(2826);
	Add(300,	ORG_1, 20,		30,		pText,	"DISEASE_STOMOCHCANCER");	// 항암제
	
	pText = GetTxtFromMgr(2827);
	Add(400,	ORG_2, 1,		10,		pText,	"DISEASE_HYPERTENSION" );	// 고혈압약
//	Add(500,	ORG_2, 10,		15,		"동맥경화");
//	Add(600,	ORG_2, 20,		30,		"당뇨병");
	
	pText = GetTxtFromMgr(2828);
	Add(700,	ORG_3, 1,		10,		pText,	"DISEASE_COLD");				// 감기약
//	Add(800,	ORG_3, 10,		15,		"폐렴");
//	Add(900,	ORG_3, 20,		30,		"폐암");

	pText = GetTxtFromMgr(2829);
	Add(1100,	ORG_4, 1,		10,		pText,	"DISEASE_HEADACHE");		// 두통약
//	Add(1200,	ORG_4, 10,		15,		"간질");
//	Add(1300,	ORG_4, 20,		30,		"치매");
}

cltDiseaseManager::~cltDiseaseManager()
{
	SI32 i;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		if(pclDisease[i])
		{
			delete pclDisease[i];
			pclDisease[i] = NULL;
		}
		
	}
}

// 질병 정보를 추가한다. 
bool cltDiseaseManager::Add(SI32 unique, SI32 type, SI32 level, SI32 penalty, TCHAR* name, TCHAR* DiseaseType )
{
	SI32 i;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		if(pclDisease[i] == NULL)
		{
			pclDisease[i] = new cltDiseaseInfo(unique, type, level, penalty, name, DiseaseType );
			return true;
		}
	}

	return false;

}

// 질병의 정보를 찾는다. (질병에 걸릴 수치가 맞는지..)
cltDiseaseInfo* cltDiseaseManager::FindDisease(SI32 org, SI32 silevel)
{
	SI32 i;

	cltDiseaseInfo* prtn	= NULL;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		if(pclDisease[i])
		{
			if(pclDisease[i]->siType == org)
			{
				if(silevel >= pclDisease[i]->siLevel)
				{
					prtn = pclDisease[i];
				}
			}
		}
	}

	return prtn;
}


// 질병의 이름을 유니크를 구한다.
SI32 cltDiseaseManager::FindUniqueFromName(TCHAR* pname)
{
	SI32 i;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		if(pclDisease[i])
		{
			if(_tcscmp(pclDisease[i]->szName, pname) == 0)
			{
				return pclDisease[i]->siUnique;
			}
		}
	}

	return 0;
}

// 질병의 유니크로 이름을 구한다. 
bool cltDiseaseManager::GetNameFromdUnique(SI32 unique, TCHAR* pname, SI16 txtSize)
{

	SI32 i;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		if(pclDisease[i]) 
		{
			if(pclDisease[i]->siUnique == unique)
			{
				StringCchCopy(pname, txtSize, pclDisease[i]->szName);
				return true;
			}
		}
	}

	return false;
}

// 질병타입으로 유니크를 구한다.
SI32 cltDiseaseManager::FindUniqueFromDiseaseType(TCHAR* pDiseaseType)
{
	SI32 i;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		if(pclDisease[i])
		{
			// Disease 탭의 질병타입을 찾아 해당 유니크를 찾는다.
			if(_tcscmp(pclDisease[i]->szDiseaseType, pDiseaseType) == 0)
			{
				return pclDisease[i]->siUnique;
			}
		}
	}

	return 0;
}

// 질병의 유니크로 질병타입을 구한다.
bool cltDiseaseManager::GetDiseaseTypeFromdUnique(SI32 unique, TCHAR* pDiseaseType, SI16 txtSize)
{

	SI32 i;

	for(i = 0;i < MAX_DISEASE_NUMBER;i++)
	{
		if(pclDisease[i]) 
		{
			if(pclDisease[i]->siUnique == unique)
			{
				StringCchCopy(pDiseaseType, txtSize, pclDisease[i]->szDiseaseType);
				return true;
			}
		}
	}

	return false;
}

//============================================
// cltDiseaseInfo
//============================================
cltDiseaseInfo::cltDiseaseInfo(SI32 unique, SI32 type, SI32 level, SI32 penalty, TCHAR* name, TCHAR* DiseaseType )
{
	siUnique	=	unique;
	siType		=	type;
	siLevel		=	level;
	siPenalty	=	penalty;

	MStrCpy( szName, name, DISEASE_NAME );
	MStrCpy( szDiseaseType, DiseaseType, DISEASE_TYPE );
}

cltDiseaseInfo::~cltDiseaseInfo()
{}


