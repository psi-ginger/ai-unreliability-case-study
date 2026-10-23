//---------------------------------
// 2003/6/12 김태곤
//---------------------------------
#ifndef _CANDIDATE_H
#define _CANDIDATE_H

#include <Directives.h>

#include "..\..\Common\CommonHeader.h"
#include "..\Rank\Rank.h"

#define MAX_CANDIDATE_NUMBER	50		// 최대 입후보 가능수. 




//-----------------------------
// 입후보자 관리자.
//-----------------------------
class cltCandidateManager{
public:

#ifdef _SAFE_MEMORY
	NSafeTArray<cltSimplePerson*, MAX_CANDIDATE_NUMBER>		pclCandidate;
#else
	cltSimplePerson* pclCandidate[MAX_CANDIDATE_NUMBER];
#endif

	cltCandidateManager();
	~cltCandidateManager();

	// 후보를 등록한다. 
	BOOL Set(SI32 index, cltSimplePerson* pclcandidate);
	// 후보 등록을 취소한다. 
	BOOL DeletePerson(SI32 personid);

	// 모든 후보 등록을 취소한다. 
	BOOL Delete();

	//personid에 해당하는 후보자의 정보를 얻어온다 
	cltSimplePerson* GetCandidateInfoFromPersonID(SI32 personid, SI32* pindex);

	// 입후보자의 숫자를 알아낸다. 
	SI32 GetCandidateNum();

	//Ref에 해당하는 후보자의 정보를 얻어온다 
	cltSimplePerson* GetCandidateInfoFromRef(SI32 ref);


};

//---------------------------------
// 상단과 관련된 후보자 관리자. 
//---------------------------------
class cltCityHallCandidateManager{
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltCandidateManager*, MAX_RANKTYPE_NUMBER>		pclCandidateManager;
#else
		cltCandidateManager* pclCandidateManager[MAX_RANKTYPE_NUMBER];
#endif

public:


	cltCityHallCandidateManager();
	~cltCityHallCandidateManager();

	// 후보를 설정한다. 
	BOOL SetCandidate(SI32 index, SI32 ranktype, cltSimplePerson* pclcandidate);

	// 모든 후보 등록을 취소한다. 
	BOOL Delete();

	// 특정 Personid를 모든 후보 등록에서  취소한다. 
	BOOL DeletePerson(SI32 personid);

	// 특정 신분의 후보들을 모두 삭제한다.
	BOOL DeleteRank(SI32 ranktype);


	//personid에 해당하는 후보자의 정보를 얻어온다 
	cltSimplePerson* GetCandidateInfoFromPersonID(SI32 ranktype, SI32 personid, SI32* pindex);

	// 특정 신분의 입후보자 숫자를 얻어온다. 
	SI32 GetCandidateNum(SI32 ranktype);

	//Ref에 해당하는 후보자의 정보를 얻어온다 
	cltSimplePerson* GetCandidateInfoFromRef(SI32 ranktype, SI32 ref);


};

#endif