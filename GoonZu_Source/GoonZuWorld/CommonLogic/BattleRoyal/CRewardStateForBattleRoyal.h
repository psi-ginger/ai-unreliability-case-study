//
//
//  @ File Name : CRewardStateForBattleRoyal.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 보상 상태 클래스
//


#if !defined(_CREWARDSTATEFORBATTLEROYAL_H)
#define _CREWARDSTATEFORBATTLEROYAL_H

#include "CBattleRoyalState.h"

class CRewardStateForBattleRoyal : public CBattleRoyalState {
public:
	CRewardStateForBattleRoyal();
	~CRewardStateForBattleRoyal();
	BOOL	Handle();
};

#endif  //_CREWARDSTATEFORBATTLEROYAL_H
