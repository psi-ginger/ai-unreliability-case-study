//
//
//  @ File Name : CBattleRoyalStadium.h
//  @ Date : 2009-09-25
//  @ Author : 박기형
//
//
#if !defined(_CBATTLEROYALSTADIUM_H)
#define _CBATTLEROYALSTADIUM_H

#include "../CommonLogic.h"
#include "./CBattleRoyalPersonChangedData.h"

class cltCommonLogic;
class cltServer;
class cltClient;

class CBattleRoyalStadium {

public:
	CBattleRoyalStadium();
	virtual ~CBattleRoyalStadium();
	
	virtual BOOL	Init();			// 데이터 초기화
	BOOL	IsOpenedGate();	// 경기장 입구가 열려 있는지?
	virtual void	OpenGate();		// 경기장 입구를 연다.
	void	CloseGate();	// 경기장 입구를 닫는다.
	BOOL	NoticeResult();	// 결과를 알린다.
	virtual BOOL DeleteGamer(SI32 siPersonID);
	// 입장 가능 최소 레벨
	UI32	GetCanEnterMinLevel();
	// 입장 가능 최대 레벨
	UI32	GetCanEnterMaxLevel();

	SI32	GetMapIndex();

	cltCharCommon* GetWinner();
	// 최대 유닛 수
	const static SI32 MAX_UNIT_COUNT = 30;	
protected:
	
	// 공통 로직 포인터
	cltCommonLogic*	m_pclCommon;

	// 맵 인덱스
	SI32	m_siMapIndex;
	// 몹 최대 수
	SI32	m_siMobCount;

	// 경기장의 문이 열려 있는지?
	bool	m_bOpenGame;

	// 우승자 정보
	cltCharCommon* m_pclWinner;

	// 입장 가능 최소 레벨
	UI32	m_uiCanEnterMinLevel;
	// 입장 가능 최대 레벨
	UI32	m_uiCanEnterMaxLevel;
	// 몬스터 종류
	SI32	m_siKindBlackarmySword;
	SI32	m_siKindBlackarmyRifle;
	SI32	m_siKindBlackarmyKatar;

};

class CBattleRoyalStadiumServer : public CBattleRoyalStadium {

public:
	CBattleRoyalStadiumServer();
	virtual ~CBattleRoyalStadiumServer();

	// 최대 참가자 수
	const static SI32 MAX_GAMER_COUNT = 20;	

	BOOL	Init();
	void	InitData();			// 데이터 초기화
	BOOL	IsLimitedGamerNum();// 참가자 수가 한계치인가?
	UI32	GetWaitGamerNum();	// 경기에 대기 중인 유저 수를 보여준다.
	virtual BOOL	NoticeWait() = 0;	// 배틀로얄 대기를 알린다.
	virtual BOOL	NoticeOpen() = 0;	// 경기장이 열림을 알린다.
	BOOL	SaveGamerList();// 경기에 참가자 리스트를 저장한다.
	BOOL	CreateMob();	// 몹을 생성한다.
	BOOL	RemoveMob();	// 몹을 제거한다.
	BOOL	BanGamer();		// 참가자를 경기장에서 강퇴시킨다.
	BOOL	NoticeResult();	// 결과를 알린다.
	BOOL	WasJoinedLastGame(SI32 m_siPersonId); // 마지막 경기에 참가 했었는지?
	UI32	GetGamerNum();	// 경기에 참가할 유저 수
	UI32	GetGamingGamerNum();	// 경기에 참가가 중인 유저 수
	UI32	GetMobNum();	// 경기에 존재하는 몹의 수
	BOOL	ProcessResult();// 결과를 처리한다.
	void	DieMob();		// 몹 죽음
	BOOL	DeleteGamer(SI32 siPersonID);
	void	ProcessLogoutCharByIndex(SI32 siIndex);

	NkVector<CBattleRoyalPersonChangedData>& GetPersonChangedData();
	NkVector<SI32>& GetMobData();

	cltCharServer*	GetGamingCharByIndex(SI32 siIndex);
	
	virtual void OpenGate() = 0;
protected:
	// 몹 캐릭터 ID
	NkVector<SI32>	m_MobCharIDArray;
	// 전 경기 참가자 배열
	NkVector<SI32>	m_BeforeGamerArray;
	// 참가자 배열
	NkVector<CBattleRoyalPersonChangedData>	m_GamerArray;

	// 서버 로직 포인터
	cltServer*	m_pclServer;
	
	// 전 경기 참가자 수
	SI32	m_siBeforeGamerCount;
	
	// 참가자 수
	SI32	m_siGamerCount;
	// 게임중인 참자가 수
	SI32	m_siGamingUserCount;

	BOOL	Notice(TCHAR* pszContentText);		// 공지하기
};

class CBattleRoyalStadiumClient : public CBattleRoyalStadium{

public:
	CBattleRoyalStadiumClient();
	virtual ~CBattleRoyalStadiumClient();
	BOOL	DeleteGamer(SI32 siPersonID);	// 참가자 퇴장
	BOOL	NoticeResult();	// 결과를 알린다.
protected:
	// 클라이언트 로직 포인터
	cltClient*	m_pclClient;
};


#endif  //_CBATTLEROYALSTADIUM_H
