#ifndef _GENERALMEETING_H
#define _GENERALMEETING_H

#include "CommonHeader.h"

class cltGeneralMeetingVoter
{
public:
	cltGeneralMeetingVoter();
	~cltGeneralMeetingVoter();

	void			Init();
	void			Set( cltGeneralMeetingVoter* pclVoter );

public:
	SI32			siPersonID;											// 투표자 personid
	TCHAR			szPersonName[ MAX_PLAYER_NAME ];					// 투표자 이름 
	SI32			siStockAmount;										// 보유 주식 수
	SI32			siVoteType;											// 투표 내용  1: 찬성,  2: 반대
	SI32			siParticipate;										// 참여 여부  0: 무의사,  1: 참여,  2:참여안함
};


class cltGeneralMeetingInfo
{
public:
	cltGeneralMeetingInfo();
	~cltGeneralMeetingInfo();

	void			Init();

	void			SetVoter( cltGeneralMeetingVoter voter[] );

	BOOL			IsVoter( SI32 personid );
	BOOL			IsAleadyVote( SI32 personid );
	BOOL			Vote( SI32 personid, SI32 votetype );
	
	SI32			GetTotalAgree();
	SI32			GetTotalOpposition();
	SI32			GetTotalNotParticipate();//반대인원 토탈 구함[2007.07.10 성웅]

	SI32			GetResult();

	//void			Action( SI32 _siVillageUnique );

public:
	SI32			siVillageUnique;
	SI32			siMatterType;										
	SI32			siPersonID;											// 발의자 personid
	SI32			siTotalStock;										// 마을 총 주식 수 
	cltDate			clStartDate;										// 주주총회 시작일

	cltDate			clCloseDate;										// 주주총회 종료일자

	SI32			siCandidatePersonID;								// 대행수 후보자 personid
	TCHAR			szCandidateName[ MAX_PLAYER_NAME ];					// 대행수 후보자 이름

	SI32			siAcceptCandidate;									// 본인동의 여부 

#ifdef _SAFE_MEMORY
	NSafeTArray<cltGeneralMeetingVoter, 100>	clVoter;
#else
	cltGeneralMeetingVoter	clVoter[ 100 ];								// 투표자 정보
#endif
};


#endif