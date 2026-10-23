//
//
//  @ File Name : CBattleRoyalFactory.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//	@ Desc : 배틀로얄 Factory 클래스
//


#if !defined(_CBATTLEROYALFACTORY_H)
#define _CBATTLEROYALFACTORY_H

#include "../CommonLogic.h"
#include "./CBattleRoyal.h"

class CBattleRoyal;

class CBattleRoyalFactory {

public :
	static const SI32 MABRIUM = 1;			// 마브리움
	static const SI32 GRANDMABRIUM = 2;		// 그랑마브리움

	static CBattleRoyal*	CreateBattleRoyal(SI32 siKind);					  // 배틀로얄 생성
	static BOOL				DestroyBattleRoyal(CBattleRoyal* pclBattleRoyal); // 배틀로얄 제거
};

#endif  //_CBATTLEROYALFACTORY_H
