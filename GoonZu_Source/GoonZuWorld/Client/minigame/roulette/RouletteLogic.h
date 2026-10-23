#pragma once

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "ndataTypes.h"
#include "ntimer.h"

const int ROULETTE_X		  = 150;
const int ROULETTE_Y		  = 250;
const int ROULETTE_WIDTH	  = 54;
const int ROULETTE_HEIGHT	  = 54;

struct RouletteTable
{
	bool m_bUserSelect;	// 유저가 선택
	bool m_bCpuSelect;	// 컴터가 선택
	bool m_bResult;		// 결과값 false :실패 true는 성공

	SI16 m_siX;
	SI16 m_siY;

	SI16 m_siValue;		// 룰렛 번호

	void Set(bool bUserSelect, bool bCpuSelect, bool bResult, SI16 siX, SI16 siY, SI16 siValue )
	{
		m_bUserSelect = bUserSelect;
		m_bCpuSelect = bCpuSelect;
		m_bResult = bResult;

		m_siX = siX;
		m_siY = siY;
		
		m_siValue = siValue;
	};
};

class CRouletteLogic
{
private:

	RouletteTable	m_stRouletteTable[5][5];

	POINT			m_Mouse;
	SI16			m_siRandomValue[5];		// 랜덤 번호 정하자
	SI32			m_siBettingCount;		// 흥부박을 건 수
	bool			m_bStart;

public:

	CRouletteLogic();
	~CRouletteLogic();

	void Init();	
	void StartRoulette();
	void CheckRoulette();			// 먼저 룰렛 체크
	void StopRoulette();
//	bool CheckLottoRoulette();		// 대박인지 체크

	bool SetMouseXY(SI32 x, SI32 y);
	void SetRandomValue(SI16 randomvalue[]);
	SI32 GetGourdNumber();


	inline void SetBettingCount(SI32 count)		{ m_siBettingCount = count; }
	inline void SetStart(bool value)			{ m_bStart = value;	}


	inline SI32	GetXPosition(SI32 y, SI32 x)	{ return m_stRouletteTable[y][x].m_siX; }
	inline SI32	GetYPosition(SI32 y, SI32 x)	{ return m_stRouletteTable[y][x].m_siY; }

	// 유저가 선택한 룰렛
	inline bool GetUserSelect(SI32 y, SI32 x)	{ return m_stRouletteTable[y][x].m_bUserSelect; }
	// CPU가 선택한 룰렛
	inline bool GetCpuSelect(SI32 y, SI32 x)	{ return m_stRouletteTable[y][x].m_bCpuSelect; }
	// 해당 룰렛 숫자판의 결과
	inline bool GetResult(SI32 y, SI32 x)		{ return m_stRouletteTable[y][x].m_bResult; }	
	// 해당 룰렛의 숫자
	inline SI32 GetValue(SI32 y, SI32 x)		{ return m_stRouletteTable[y][x].m_siValue; }	
	// 랜덤 숫자 5개 얻기
	inline SI16 GetRandom(SI32 index)			{ return m_siRandomValue[index]; }	
	inline SI32 GetBettingCount()				{ return m_siBettingCount; }	
	inline bool GetStart()						{ return m_bStart; }

	
};