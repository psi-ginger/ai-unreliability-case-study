//
//
//  @ File Name : CAbstractState.h
//  @ Date : 2009-09-28
//  @ Author : 박기형
//	@ Desc : 추상 상태 클래스
//


#if !defined(_CABSTRACTSTATE_H)
#define _CABSTRACTSTATE_H

#include "../CommonLogic.h"

class CAbstractState {
public:
	virtual BOOL	Handle() = 0;
};

#endif  //_CABSTRACTSTATE_H
