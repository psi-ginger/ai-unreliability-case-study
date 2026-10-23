//---------------------------------
// 2003/11/19 김태곤
//---------------------------------

#ifndef _DISEASE_H
#define _DISEASE_H

#include <Directives.h>

#define MAX_DISEASE_NUMBER				20		// 질병의 최대 종류 
#define DISEASE_NAME					32		
#define DISEASE_TYPE					32		// 질병타입


//--------------------------------------
// 질병 정보. 
//--------------------------------------
class cltDiseaseInfo
{
public:
	SI32 siUnique;		// 질병의 유니크. 
	SI32 siType;		// 어느계의 질환인가 ?
	SI32 siLevel;		// clInfo.uiOrg1Disease 수치가 얼마 이상이 되면 이 질병에 걸리는가?
	SI32 siPenalty;		// 건강에 미치는 해악. (0 ~ 100 : 0이면 아무런 해악이 없다. ) 

	TCHAR szName[DISEASE_NAME];
	
	TCHAR szDiseaseType[DISEASE_TYPE];

	cltDiseaseInfo(SI32 unique, SI32 type, SI32 level, SI32 penalty, TCHAR* name, TCHAR* DiseaseType );
	~cltDiseaseInfo();
};

//--------------------------------------
// 질병 매니저. 
//-------------------------------------
class cltDiseaseManager{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltDiseaseInfo*, MAX_DISEASE_NUMBER>		pclDisease;
#else
	cltDiseaseInfo* pclDisease[MAX_DISEASE_NUMBER];
#endif
	
	cltDiseaseManager();
	~cltDiseaseManager();

	// 질병 정보를 추가한다.
	bool Add(SI32 unique, SI32 type, SI32 level, SI32 penalty, TCHAR* name, TCHAR* DiseaseType);

	// 질병의 정보를 찾는다. 
	cltDiseaseInfo* FindDisease(SI32 org, SI32 level);

	// 질병의 이름을 유니크를 구한다.
	SI32 FindUniqueFromName(TCHAR* pname);

	// 질병의 유니크로 이름을 구한다. 
	bool GetNameFromdUnique(SI32 unique, TCHAR* pname, SI16 txtSize);


	// 질병타입으로 유니크를 구한다.
	SI32 FindUniqueFromDiseaseType(TCHAR* pDiseaseType);

	// 질병의 유니크로 질병타입을 구한다. 
	bool GetDiseaseTypeFromdUnique(SI32 unique, TCHAR* pDiseaseType, SI16 txtSize);

};


#endif
