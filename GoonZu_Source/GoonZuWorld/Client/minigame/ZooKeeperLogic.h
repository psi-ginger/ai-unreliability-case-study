#ifndef _ZOO_KEEPER_LOGIC
#define _ZOO_KEEPER_LOGIC

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "ndataTypes.h"
#include "ntimer.h"

const int GAME_TIME       = 120;
const int SPECIAL_RAND    = 999;
const int EMPTY_TABLE	  = -1;
const int SPECIAL_BLOCK_VALUE = 7;
const int TABLE_X		  = 150;
const int TABLE_Y		  = 116;
const int BLOCK_WIDTH	  = 52;
const int BLOCK_HEIGHT	  = 52;
const int BLOCK_DOWNSPEED = 2;
const int BLOCK_SWAPSPEED = 5;
const int SCORE			  = 10;
const int TIMEPLUS		  = 2;
const int PENALTYSCORE    = 20;
const int PENALTYTIME     = 2;

const int MOVSTOP		 = 0;
const int MOVLEFT		 = 1;
const int MOVRIGHT		 = 2;
const int MOVUP			 = 3;
const int MOVDOWN		 = 4;

struct BlockTable
{
	SI32 m_X;
	SI32 m_Y;

	SI32 m_destX;
	SI32 m_destY;

	SI32 m_Value;
	SI32 m_movtype;
	BOOL m_bSelect;			// 선택 커서
	BOOL m_bSelectImage;	// 선택 이미지
	
};

class CZooKeeper
{
public:
	
	SI32			m_xcount;
	SI32			m_ycount;

	SI32			m_BlockDownMoveValue;
	SI32			m_BlockSwapMoveValue;
	SI32			m_BlockWidth;
	SI32			m_BlockHeight;

	POINT			m_Mouse;
	SI32			m_FirstClickBlockValue;
	SI32			m_SecondClickBlockValue;
	SI32			m_FirstClickBlockX;
	SI32			m_FirstClickBlockY;
	SI32			m_SecondClickBlockX;
	SI32			m_SecondClickBlockY;
	BOOL			m_bClickState;
	BOOL			m_GameRunState;
	
	SI32			m_MoveY;
	REAL32			m_GameTime;
	BOOL			m_bXCheckState;
	BOOL			m_bYCheckState;
	SI32			m_GameScore;
	SI32			m_TrueGameScore;
	SI32			m_Combo;
	SI32			m_DeleteBlockCount;
	BOOL			m_bSpecialBlockLive;

	BlockTable		m_BlockTableXY[9][9];
	BlockTable		m_BlockTableX[9][9];
	BlockTable		m_BlockTableY[9][9];

public:

	CZooKeeper();
	~CZooKeeper();

	void	Init();
	void	SetBlockTable();
	
	BOOL    VerticalCheck();		// 세로
	BOOL	HorizonalCheck();		// 가로
	void	SpecialCheck();			// 특별 체크
	void	BlockDown();
	void	ResetTable();
	void	ResetXYTable();
	void	CreateBlock();
	
	BOOL	SetMouseXY(SI32 x, SI32 y);					// 첫번째 클릭
	BOOL	SwapBlockCheck(SI32 x, SI32 y);				// 두번째 클릭
	void	SwapBlockValue(SI32* FirstValue, SI32* SecondValue);

	void	InputMoveBlock();
	void	InputMoveBlock2();
	void	MovBlocks();
	void	MovBlocks2();
	BOOL	IsMovStopAllBlock();
	void	ClickMouseDataInit();
	void	SetBlockPositionInit();
	void	PenaltyData();

	BOOL	CheckScore();
	void	SetScore(SI32 score, SI32 combo);
	void	SendComboInfo();
	void	SetTime(SI32 time);

	inline SI32 GetGameScore()						{ return m_GameScore; }
	inline SI32 GetTrueGameScore()					{ return m_TrueGameScore; }

	inline SI32	GetBlockXPosition(SI32 y, SI32 x)	{ return m_BlockTableXY[y][x].m_X; }
	inline SI32	GetBlockYPosition(SI32 y, SI32 x)	{ return m_BlockTableXY[y][x].m_Y; }
	inline SI32 GetBlockXXPosition(SI32 y, SI32 x)	{ return m_BlockTableX[y][x].m_X; }
	inline SI32	GetBlockYYPosition(SI32 y, SI32 x)	{ return m_BlockTableX[y][x].m_Y; }
	inline SI32	GetBlockValue(SI32 y, SI32 x)		{ return m_BlockTableXY[y][x].m_Value; }
	inline SI32	GetBlockMovType(SI32 y, SI32 x)		{ return m_BlockTableXY[y][x].m_movtype; }
	inline BOOL	GetBlockIsSelect(SI32 y, SI32 x)	{ return m_BlockTableXY[y][x].m_bSelect; }

	inline BOOL	GetHorizonalCheck()					{ return m_bXCheckState; }
	inline BOOL	GetVerticalCheck()					{ return m_bYCheckState; }
	
	inline SI32 GetFirstClickBlockX()				{ return m_FirstClickBlockX; }
	inline SI32 GetFirstClickBlockY()				{ return m_FirstClickBlockY; }
	inline REAL32 GetGameTime()						{ return m_GameTime;	}
	inline SI32 GetBlockCombo()						{ return m_Combo; }

	inline void SetBlockCombo(SI32 count)			{ m_Combo = count; }
	inline void	SetHorizonalCheck(BOOL bCheck)		{ m_bXCheckState = bCheck; }
	inline void	SetVerticalCheck(BOOL bCheck)		{ m_bYCheckState = bCheck; }
	
	// 나중에 바꾸자

};
#endif