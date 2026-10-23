//
//
//  @ File Name : CGrandMabriumBattleRoyalStadium.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//
//


#if !defined(_CGRANDMABRIUMBATTLEROYALSTADIUM_H)
#define _CGRANDMABRIUMBATTLEROYALSTADIUM_H

#include "CBattleRoyalStadium.h"

class CGrandMabriumBattleRoyalStadiumServer : public CBattleRoyalStadiumServer{
public:
	CGrandMabriumBattleRoyalStadiumServer();
	~CGrandMabriumBattleRoyalStadiumServer();
	virtual BOOL	NoticeWait();	// 배틀로얄 대기를 알린다.
	virtual BOOL	NoticeOpen();	// 경기장이 열림을 알린다.
	virtual void	OpenGate();
};

class CGrandMabriumBattleRoyalStadiumClient : public CBattleRoyalStadiumClient{
public:
	CGrandMabriumBattleRoyalStadiumClient();
	~CGrandMabriumBattleRoyalStadiumClient();
};

#endif  //_CGRANDMABRIUMBATTLEROYALSTADIUM_H
