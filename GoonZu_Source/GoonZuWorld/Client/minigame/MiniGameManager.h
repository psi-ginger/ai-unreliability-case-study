#ifndef _MINI_GAME_MANAGER_
#define _MINI_GAME_MANAGER_

#include "../Client/Client.h"
#include "ndataTypes.h"

class CZooKeeper;
class CZooDraw;

class CRouletteLogic;
class CRouletteDraw;

#define MINI_ZOOKEEPER 0
#define MINI_ROULETTE  1

#define INTERFACR_CREATURE(terminal)									 \
	public:																 \
		virtual bool Init() ##terminal									 \
		virtual void Action() ##terminal								 \
		virtual void Draw(SI32 x, SI32 y) ##terminal					 \
		virtual void KeyAction(SI32 x, SI32 y, SI32 type = 0) ##terminal \
		virtual SI32 GetResult() ##terminal 

#define BASE_CREATURE			INTERFACR_CREATURE(=0;)
#define DERIVED_CREATURE		INTERFACR_CREATURE(;)
class cltCommonLogic;

class CMiniGameManager
{
private:

public:
	CMiniGameManager();
	virtual ~CMiniGameManager();

	BASE_CREATURE;
//	virtual void BackSound() = 0;

};

class CZooKeeperBase: public CMiniGameManager
{
public:
	enum{INIT_BLOCK = 0, CHECK_BLOCK, DOWN_BLOCK, SPECIAL_BLOCK, FALSE_BLOCK, MOUSEINPUT_BLOCK, INPUTCHECK_BLOCK, EFFECT_BLOCK, GAME_OVER}GAME_STATE;

	CZooKeeper* m_ZooKeeperLogic;
	CZooDraw*   m_ZooDraw;

public:
	CZooKeeperBase();
	~CZooKeeperBase();

	DERIVED_CREATURE;
//	virtual void BackSound();

	bool LoadSpr();
	void FreeSpr();

	void Printf(int x, int y, int Color, TCHAR* szStr, ...);
	void DrawTable(SI32 dlgx, SI32 dlgy);
	void DrawScore(SI32 x, SI32 y, SI32 Score);
	void DrawTimeBar(SI32 x, SI32 y, SI32 Time);

	void RunInitBlock();		// 처음 블럭 보여질때 
	void RunCheckBlock();		// 가로 세로 체크
	void RunDownBlock();		//
	void RunTrueBlock();		// 조건이 맞았을때
	void RunFalseBlock();		// 조건이 틀렸을때
	void RunInputCheckBlock();		// 마우스 입력 받기, 대기상대
	void RunSpecialBlock();
	void RunEffectBlock();
	void GameOver();

	void PlaySound(cltCommonLogic* pclClient, int Type);
};

class CRouletteBase: public CMiniGameManager
{
public:
	enum{INIT_ROULETTE = 0, INPUT_ROULETTE, START_ROULETTE, CHECK_ROULETTE, END_ROULETTE}GAME_STATE;

	CRouletteLogic*	m_pRouletteLogic;
	CRouletteDraw*	m_pRouletteDraw;

	DWORD m_dwTotalClock;	// 총 게임 시간
	DWORD m_dwCheckClock;
	DWORD m_dwStartClock;	// 딜레이 계산
	DWORD m_dwLastClock;	
	
	DWORD m_dwNextClock;	// 다음 상태로 넘어가자!!
	SI32  m_siDelay;		// 랜덤 선택 딜레이
	SI32  m_siRandomX[5];
	SI32  m_siRandomY[5];
	SI32  m_siBirdX;

	bool  m_bStart;
	bool  m_bSelect[5];

	SI32  m_siNumber;

public:
	CRouletteBase();
	~CRouletteBase();

	DERIVED_CREATURE;

	void Printf(int x, int y, int Color, TCHAR* szStr, ...);
	
	void DrawRoulette(SI32 dlgx, SI32 dlgy);
	void DrawValue(SI32 x, SI32 y, SI32 value);
	void DrawValue2(SI32 x, SI32 y, SI32 value);
	void DrawUserSelect(SI32 x, SI32 y);
	void DrawCpuSelect(SI32 dlgx, SI32 dlgy);
	void DrawResult(SI32 dlgx, SI32 dlgy);		// 유저선택과 컴퓨터 선택이 일치할때
	void DrawBird();
	void DrawRoll(SI32 x, SI32 y, SI32 delay);

	void SetResult(SI16 randomvalue[], SI16 result, SI16 number);
	
	void RunInitRoulette();
	void RunInputRoulette();
	void RunStartRoulette();
	void RunCheckRoulette();
	void RunEndRoulette();
	void PlaySound(cltCommonLogic* pclClient, int Type);
};

class CMiniGameFactory
{
public:
	
	CMiniGameManager*	CreateMiniGame(SI32 type)
	{
		switch(type)
		{
			case MINI_ZOOKEEPER:
				return new CZooKeeperBase;
			case MINI_ROULETTE:
				return new CRouletteBase;
			default:
				return 0;	// 실패시 없다
		}
	}

};

#endif