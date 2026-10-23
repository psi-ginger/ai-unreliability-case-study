//
//
//  @ File Name : CGrandMabriumBattleRoyal.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 그랑마브 배틀로얄 처리 클래스
//


#if !defined(_CGRANDMABRIUMBATTLEROYAL_H)
#define _CGRANDMABRIUMBATTLEROYAL_H

#include "CBattleRoyal.h"

class CGrandMabriumBattleRoyal : public CBattleRoyal {
public:
	CGrandMabriumBattleRoyal();
	virtual ~CGrandMabriumBattleRoyal();

	virtual	BOOL	Create();	// 생성
};

#endif  //_CGRANDMABRIUMBATTLEROYAL_H
