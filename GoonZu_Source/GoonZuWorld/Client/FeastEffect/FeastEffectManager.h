#pragma once

#include <Directives.h>
#include <vector>

class CFeastEffect;

typedef std::vector<CFeastEffect*>  FeastEffectVector;
typedef FeastEffectVector::iterator FeastEffectVectorIter;

#define DOWN_EFFECT_NUM 20

struct stEffectInfo
{
	SI32				m_ArrayX[10];
	SI32				m_ArrayY[10];
	bool				m_bEffectPlay;
	REAL32				m_fEffectCurrentTime;
	REAL32				m_fEffectLastTime;
	REAL32				m_fEffectDelay;			// 이펙트 생성 딜레이
	SI08				m_siEffectCreateType;	//	0: 한번에 여러개 생성 1: 시간차로 생성
	SI16				m_siEffectLifeTime;		// 이펙트 반복횟수
	SI16				m_siEffectLifeCount;	// 이펙트 반복횟수를 계산
	SI32				m_siEffectCount;		
	SI32				m_siEffectMaxCount;		// 이펙트 최대 갯수

	SI32				m_siStartPositionX;		// 이펙트가 뿌려질 시작 위치 X
	SI32				m_siStartPositionY;		// 이펙트가 뿌려질 시작 위치 Y

	void Init()
	{
		for(SI32 i=0; i<10; ++i)
		{
			m_ArrayX[i] = 0;
			m_ArrayY[i] = 0;
		}
		m_bEffectPlay			= false;
		m_fEffectCurrentTime	= 0.0f;
		m_fEffectLastTime		= 0.0f;
		m_fEffectDelay			= 0.0f;
		m_siEffectCreateType	= 0;
		m_siEffectLifeTime		= 0;
		m_siEffectLifeCount		= 0;
		m_siEffectCount			= 0;
		m_siEffectMaxCount		= 0;

		m_siStartPositionX		= 0;
		m_siStartPositionY		= 0;
	};
};

class CFeastEffectManager
{
private:

	SI32				m_arrayPositionX[DOWN_EFFECT_NUM];// = { 150, 350, 550, 750, 200, 400, 600, 800,	250, 450, 650, 850,	300, 500, 700, 900 };
	SI32				m_arrayPositionY[DOWN_EFFECT_NUM];// = { -50, -50, -50, -50,	-200, -200, -200, -200,	-350, -350, -350, -350,	-500, -500, -500, -500 };	

	SI32				m_Type3PositionX[5];	//러브불꽃
	SI32				m_Type3PositionY[5];

	SI32				m_Type4PositionX[6];	//박수
	SI32				m_Type4PositionY[6];

	SI32				m_Type5PositionX[5];	//사방형불꽃
	SI32				m_Type5PositionY[5];

	SI32				m_Type6PositionX[5];	//상승불꽃
	SI32				m_Type6PositionY[5];

	SI32				m_Type7PositionX[9];	//하트불꽃2
	SI32				m_Type7PositionY[9];

	SI32				m_Type8PositionX[5];	//박
	SI32				m_Type8PositionY[5];

	SI32				m_Type9PositionX[3];	//상자
	SI32				m_Type9PositionY[3];

	SI32				m_Type10PositionX[6];	//폭죽
	SI32				m_Type10PositionY[6];

	FeastEffectVector	m_FeastEffectVector;

	SI32				m_siActionType;
	SI32				m_siActionKind;

	REAL32				m_fEffectCurrentTime[11];
	REAL32				m_fEffectLastTime[11];
	bool				m_bEffectPlay[11];
	SI32				m_siEffectCount[11];
	SI32				m_siEffectLifeTime[11];
	
	stEffectInfo		m_stEffectInfo[11];

public:

	BOOL				m_bEffectLive;

	CFeastEffectManager();
	~CFeastEffectManager();

	void AddEffect(SI32 siKind, SI32 siStartPositionX=0, SI32 siStartPositionY=0);
	void DeleteEffect();

    void SetAction( SI32 siKind, SI32 siStartPositionX=0, SI32 siStartPositionY=0);
	void UpdateEffectOne(stEffectInfo* pEffectInfo, SI32 siKind);
	void UpdateEffectTwo(stEffectInfo* pEffectInfo, SI32 siKind);
	void UpdateEffectThree(stEffectInfo* pEffectInfo, SI32 siKind);
	void Action();
	void StopFeastEffect();
	void Draw();

	void LoadText(TCHAR* pFineName, stEffectInfo* pEffectInfo);

};