//---------------------------------
// 2003/6/12 김태곤
//---------------------------------

#ifndef _RANK_H
#define _RANK_H

#include <Directives.h>
#include ".\..\Common\CommonHeader.h"


//-----------------------------------------
// 직급정보. 
//-----------------------------------------
class cltRank{
public:
	cltSimpleRank		clRank;			// 신분의 최소 정보. 
	cltSimplePerson		clPerson;		// 대상자의 정보 

public:
	cltRank()
	{
		Init();
	}
	cltRank(cltSimpleRank* pclrank, cltSimplePerson* pclperson)
	{
		clRank.Set(pclrank);
		clPerson.Set(pclperson);
	}

	void Init()
	{
		ZeroMemory(this, sizeof(cltRank));

		clRank.Init();
		clPerson.Init();
	}

	void Set(cltRank* pclrank)
	{
		Set(&pclrank->clRank, &pclrank->clPerson);
	}

	void Set(const cltSimpleRank* pclrank, const cltSimplePerson* pclperson);

	SI32 GetPersonID()const{return clPerson.GetPersonID();}
	SI32 GetRankType()const{return clRank.GetRankType();}

	SI32 GetVillageUnique()const{return clRank.GetVillageUnique();}

	void SetName(const TCHAR* pname){clPerson.SetName(pname);}
	const TCHAR* GetName()const{return clPerson.GetName();}

	const cltSimpleRank* GetRank()const {return &clRank;}
	const cltSimplePerson* GetPerson()const{ return &clPerson;}

	BOOL IsSame(cltSimpleRank* pclrank)const
	{
		return clRank.IsSame(pclrank);
	}
};

//---------------------------------------
// 직급 관리자. 
//---------------------------------------
class cltRankManager{
private:
	// 각 마을별로 존재하는 직급. 
#ifdef _SAFE_MEMORY
	NSafeTArray< NSafeTArray<cltRank, MAX_VILLAGE_NUMBER>, MAX_RANKTYPE_NUMBER>		clRank;
#else
	cltRank clRank[MAX_RANKTYPE_NUMBER][MAX_VILLAGE_NUMBER];		// 대행수 직급 
#endif


public:

	cltRankManager();
	~cltRankManager();

	// 직급을 갖고 있는지 확인한다
	cltRank* GetRankInfo(SI32 personid);
	cltRank* GetRank(SI32 ranktype, SI32 villageunique);


	// 신분을 갖는 사람을 등록한다. 
	SI32 SetRank(const cltSimpleRank* pclrank, const cltSimplePerson* pclperson);
	
	// personid가 특정 신분인지 확인한다. 
	BOOL IsRank(SI32 personid, cltSimpleRank* pclrank);

	// personid의 특정 신분을 삭제한다. 
	BOOL DeleteRank(SI32 personid, cltSimpleRank* pclrank);

};

// 신분의 이름을 얻어온다. 
BOOL GetRankName(cltSimpleRank* pclrank, TCHAR* buffer, SI16 txtSize, UI08 grank = 0 );

//[진성] 전체 직급 얻기(군주, 백작...등) => 2008-4-3
bool GetMisterRankName(UI08 grank, TCHAR* buffer, SI16 txtSize);

//[진성] 마을 직급 얻기(영주, 소영주...등) => 2008-4-3
bool GetVillageRankName(cltSimpleRank* pclrank, TCHAR* buffer, SI16 txtSize);

#endif

