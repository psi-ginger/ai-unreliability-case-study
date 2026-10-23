//
//
//  @ File Name : CStartStateForBattleRoyal.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 시작 상태 클래스
//


#if !defined(_CSTARTSTATEFORBATTLEROYAL_H)
#define _CSTARTSTATEFORBATTLEROYAL_H

#include "CBattleRoyalState.h"

class CStartStateForBattleRoyal : public CBattleRoyalState {
public:
	CStartStateForBattleRoyal();
	~CStartStateForBattleRoyal();
	BOOL	Handle();
};

#endif  //_CSTARTSTATEFORBATTLEROYAL_H
