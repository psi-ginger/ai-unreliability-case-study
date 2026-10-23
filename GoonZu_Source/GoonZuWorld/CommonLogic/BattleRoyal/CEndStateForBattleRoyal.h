//
//
//  @ File Name : CEndStateForBattleRoyal.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 종료 상태 클래스
//


#if !defined(_CENDSTATEFORBATTLEROYAL_H)
#define _CENDSTATEFORBATTLEROYAL_H

#include "CBattleRoyalState.h"

class CEndStateForBattleRoyal : public CBattleRoyalState {

public:
	CEndStateForBattleRoyal();
	~CEndStateForBattleRoyal();
	BOOL	Handle();
};

#endif  //_CENDSTATEFORBATTLEROYAL_H
