//
//
//  @ File Name : CGameStateForBattleRoyal.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 게임 상태 클래스
//


#if !defined(_CGAMESTATEFORBATTLEROYAL_H)
#define _CGAMESTATEFORBATTLEROYAL_H

#include "CBattleRoyalState.h"

class CGameStateForBattleRoyal : public CBattleRoyalState {
public:
	CGameStateForBattleRoyal();
	~CGameStateForBattleRoyal();
	BOOL	Handle();
};

#endif  //_CGAMESTATEFORBATTLEROYAL_H
