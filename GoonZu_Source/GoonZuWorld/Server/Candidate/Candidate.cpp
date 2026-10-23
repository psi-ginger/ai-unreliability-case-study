//---------------------------------
// 2003/6/12 김태곤
//---------------------------------

#include "Candidate.h"

cltCandidateManager::cltCandidateManager()
{
	SI32 i;

	for(i = 0;i < MAX_CANDIDATE_NUMBER;i++)
	{
		pclCandidate[i] = NULL;
	}


	for(i = 0;i < MAX_CANDIDATE_NUMBER;i++)
	{
		pclCandidate[i] = new cltSimplePerson();
	}

}

cltCandidateManager::~cltCandidateManager()
{
	SI32 i;

	for(i = 0;i < MAX_CANDIDATE_NUMBER;i++)
	{
		if(pclCandidate[i])
		{
			delete pclCandidate[i];
			pclCandidate[i] = NULL;
		}
	}
}

// 후보를 등록한다. 
BOOL cltCandidateManager::Set(SI32 index, cltSimplePerson* pclcandidate)
{
	pclCandidate[index]->Set(pclcandidate);
	return TRUE;
}

// 후보 등록을 취소한다. 
BOOL cltCandidateManager::DeletePerson(SI32 personid)
{
	SI32 i;

	if(personid == 0)return FALSE;

	// 기존에 이미 등록되어 있는지 확인한다. 
	for(i = 0; i < MAX_CANDIDATE_NUMBER;i++)
	{
		if(pclCandidate[i]->GetPersonID() == personid)
		{
			pclCandidate[i]->Init();
			return TRUE;
		}
	}

	return TRUE;
}

// 후보 등록을 모두 취소한다. 
BOOL cltCandidateManager::Delete()
{
	SI32 i;

	// 기존에 이미 등록되어 있는지 확인한다. 
	for(i = 0; i < MAX_CANDIDATE_NUMBER;i++)
	{
		if(pclCandidate[i]->GetPersonID())
		{
			pclCandidate[i]->Init();
		}
	}

	return TRUE;
}

//personid에 해당하는 후보자의 정보를 얻어온다 
cltSimplePerson* cltCandidateManager::GetCandidateInfoFromPersonID(SI32 personid, SI32* pindex)
{
	SI32 i;

	if(personid == 0)return NULL;

	for(i = 0; i < MAX_CANDIDATE_NUMBER;i++)
	{
		if(pclCandidate[i] && pclCandidate[i]->GetPersonID() == personid)
		{
			*pindex = i;
			return pclCandidate[i];
		}
	}

	return NULL;

}

// 입후보자의 숫자를 알아낸다. 
SI32 cltCandidateManager::GetCandidateNum()
{
	SI32 i;
	SI32 num = 0;

	for(i = 0; i < MAX_CANDIDATE_NUMBER;i++)
	{
		if(pclCandidate[i]->GetPersonID())
		{
			num++;
		}
	}

	return num;

}


//Ref에 해당하는 후보자의 정보를 얻어온다 
cltSimplePerson* cltCandidateManager::GetCandidateInfoFromRef(SI32 ref)
{
	return pclCandidate[ref];
}


//---------------------------------
// 상단과 관련된 후보자 관리자. 
//---------------------------------
cltCityHallCandidateManager::cltCityHallCandidateManager()
{
	SI32 i;

	for(i = 0;i < MAX_RANKTYPE_NUMBER;i++)
	{
		pclCandidateManager[i] = new cltCandidateManager();
	}
}

cltCityHallCandidateManager::~cltCityHallCandidateManager()
{
	SI32 i;

	for(i = 0;i < MAX_RANKTYPE_NUMBER;i++)
	{
		if(pclCandidateManager[i])
		{
			delete pclCandidateManager[i];
			pclCandidateManager[i] = NULL;
		}
	}

}

// 후보를 설정한다. 
BOOL cltCityHallCandidateManager::SetCandidate(SI32 index, SI32 ranktype, cltSimplePerson* pclcandidate)
{
	if(ranktype ==0)return FALSE;
	if(pclcandidate == NULL)return FALSE;

	pclCandidateManager[ranktype]->pclCandidate[index]->Set(pclcandidate);
	return TRUE;
}



// 모든 후보 등록을 취소한다. 
BOOL cltCityHallCandidateManager::Delete()
{
	SI32 i;

	for(i = 0;i < MAX_RANKTYPE_NUMBER;i++)
	{
		if(pclCandidateManager[i])
		{
			pclCandidateManager[i]->Delete();
		}
	}

	return TRUE;
}

// 특정 Personid를 모든 후보 등록에서  취소한다. 
BOOL cltCityHallCandidateManager::DeletePerson(SI32 personid)
{
	SI32 i;

	for(i = 0;i < MAX_RANKTYPE_NUMBER;i++)
	{
		if(pclCandidateManager[i])
		{
			pclCandidateManager[i]->DeletePerson(personid);
		}
	}

	return TRUE;
}

// 특정 신분의 후보들을 모두 삭제한다.
BOOL cltCityHallCandidateManager::DeleteRank(SI32 ranktype)
{
	if(pclCandidateManager[ranktype])
	{
		pclCandidateManager[ranktype]->Delete();
		return TRUE;
	}

	return FALSE;
}


//personid에 해당하는 후보자의 정보를 얻어온다 
cltSimplePerson* cltCityHallCandidateManager::GetCandidateInfoFromPersonID(SI32 ranktype, SI32 personid, SI32* pindex)
{
	if(pclCandidateManager[ranktype] == NULL)return NULL;

	return pclCandidateManager[ranktype]->GetCandidateInfoFromPersonID(personid, pindex);

}

// 특정 신분의 입후보자 숫자를 얻어온다. 
SI32 cltCityHallCandidateManager::GetCandidateNum(SI32 ranktype)
{
	return pclCandidateManager[ranktype]->GetCandidateNum();
}

//Ref에 해당하는 후보자의 정보를 얻어온다 
cltSimplePerson* cltCityHallCandidateManager::GetCandidateInfoFromRef(SI32 ranktype, SI32 ref)
{
	return pclCandidateManager[ranktype]->GetCandidateInfoFromRef(ref);
}
