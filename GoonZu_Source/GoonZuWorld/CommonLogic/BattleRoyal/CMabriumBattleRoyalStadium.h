//
//
//  @ File Name : CMabriumBattleRoyalStadium.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//
//


#if !defined(_CMABRIUMBATTLEROYALSTADIUM_H)
#define _CMABRIUMBATTLEROYALSTADIUM_H

#include "CBattleRoyalStadium.h"

class CMabriumBattleRoyalStadiumServer : public CBattleRoyalStadiumServer{
public:
	CMabriumBattleRoyalStadiumServer();
	~CMabriumBattleRoyalStadiumServer();
	virtual BOOL	NoticeWait();	// 배틀로얄 대기를 알린다.
	virtual BOOL	NoticeOpen();	// 경기장이 열림을 알린다.
	virtual void	OpenGate();
};

class CMabriumBattleRoyalStadiumClient : public CBattleRoyalStadiumClient{
public:
	CMabriumBattleRoyalStadiumClient();
	~CMabriumBattleRoyalStadiumClient();
};

#endif  //_CMABRIUMBATTLEROYALSTADIUM_H
