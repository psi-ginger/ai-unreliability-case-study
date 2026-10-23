//
//
//  @ File Name : CWaitStateForBattleRoyal.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 대기 상태 클래스
//

#if !defined(_CWAITSTATEFORBATTLEROYAL_H)
#define _CWAITSTATEFORBATTLEROYAL_H

#include "./CBattleRoyalState.h"

class CWaitStateForBattleRoyal : public CBattleRoyalState {
public:
	CWaitStateForBattleRoyal();
	~CWaitStateForBattleRoyal();
	BOOL	Handle();
};

#endif  //_CWAITSTATEFORBATTLEROYAL_H
