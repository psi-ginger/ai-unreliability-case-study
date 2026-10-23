#ifndef _VOTEBASEINFO_H_
#define _VOTEBASEINFO_H_

#define MAX_CANDIDATURE_NUM				20		// 최대 입후보자수

#define MAX_VOTEEXPLAIN_LENGTH			256		// 어떤 투표임을 알릴 필요가 있으므로.. 그것에 대한 설명에 대한 text 길이

const SI32 Const_MapIndex_PollMap	=	96;		// 군주 투표소 맵 인덱스

#if defined(_USA)
	const SI32 Const_CanVote_MinLevel	=	30;		// 투표를 하기위한 최소 레벨 - 미국 서비스
#else
	const SI32 Const_CanVote_MinLevel	=	40;		// 투표를 하기위한 최소 레벨
#endif

#endif	// end of _VOTEBASEINFO_H_