//
//
//  @ File Name : CBattleRoyalReward.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//
//
#if !defined(_CBATTLEROYALREWARD_H)
#define _CBATTLEROYALREWARD_H

#include "../CommonLogic.h"

class CBattleRoyalReward {

public :
	CBattleRoyalReward();
	~CBattleRoyalReward();
	BOOL	Create();										// 생성
	BOOL	GiveRewardToWinner(cltCharCommon* pclChar);		// 우승자에게 보상하기
	BOOL	GiveRewardToKillingUser(cltCharCommon* pclChar);// 참가한 유저를 죽인 유저에게 보상하기 

protected :
	BOOL	GiveReward( UI32 uiCharID, UI32 uiItemUnique, UI32 uiItemNum, SI32 siInputMode);

	CRewardTypeItemByProbMgr* m_pclRewardItemToWinner;		// 우승 보상 아이템 매니저
	CRewardTypeItemByProbMgr* m_pclRewardToKillingUser;		// 유저 죽임 보상 아이템 매니저

};

#endif  //_CBATTLEROYALREWARD_H
