//---------------------------------
// 2004/5/20 김태곤
//---------------------------------
#ifndef _MSGTYPERANK_H
#define _MSGTYPERANK_H

//------------------------------------------------------
// 후보자 정보를 클라이언트로 통보. 
//------------------------------------------------------
class cltGameMsgResponse_CandidateList{
public:
	SI32			siIndex;				// 이 cltCandidate데이터가 어느 슬롯에 들어가야 하는가?
	cltSimpleRank	clRank;				// 입후보한 신분의 정보. 

	cltSimplePerson clPerson;

	cltGameMsgResponse_CandidateList()
	{
		siIndex	= 0;
		clRank.Init();
	}

	cltGameMsgResponse_CandidateList(SI32 index, cltSimpleRank* pclrank, cltSimplePerson* pclperson)
	{
		siIndex	= index;
		clRank.Set(pclrank);
		clPerson.Set(pclperson);
	}
};



#endif
