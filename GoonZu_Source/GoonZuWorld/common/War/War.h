#ifndef _WAR_H
#define _WAR_H

#include <Windows.h>
#include <Directives.h>

#include <DDraw.h>

#define MAX_WAR_UNIT	10

#define WAR_MIN_MON_NUMBER			20		// 이 이하면 10배로 보상. 

class cltWarUnit{
public:
	SI32 siKind;

	SI32 siNum;				// 마을 주변에 나올것들
	SI32 siOtherNum;		// 4방향에서 생겨서 나올것들

	SI32 siRate;
	SI32 siBasicLevel;
	SI32 siRandLevel;

	SI32 siCurNum;
	SI32 siOtherCurNum;

	cltWarUnit()
	{
		siKind		= 0;
		siNum		= 0;
		siOtherNum  = 0;
		siRate		= 0;
		siBasicLevel= 0;
		siRandLevel	= 0;
		siCurNum	= 0;
		siOtherCurNum = 0;
	}

	cltWarUnit(SI32 kind, SI32 num, SI32 othernum, SI32 rate, SI32 curnum, SI32 othercurnum, SI32 basic, SI32 rand)
	{
		siKind		= kind;
		siNum		= num;
		siOtherNum	= othernum;
		siRate		= rate;
		siCurNum	= curnum;
		siOtherCurNum = othercurnum;
		siBasicLevel= basic;
		siRandLevel	= rand;
	}
};

//-----------------------------
// cltWarManager
//-----------------------------
class cltWarManager{
public:
	bool bWarSwitch ;	// 지금 전쟁이 진행중인가?
	bool bWarOsakaSwitch;	// 오사카 전쟁을 시작해야 하나?

	SI32		siTotalUnitNum;		// 전체 병사수. 

	SI32		siCurUnitNum;		// 현재 병사수. 
	SI32		siPorterNum;		// 현재 나와있는 짐꾼 왜구 수. 
	SI32		siMadeNum;			// 현재까지 만들어진 수. 
	cltWarUnit clWarUnit[MAX_WAR_UNIT];
	SI32		siWatUnitNum;		// 유니트는 몇 종이 있는가?
	SI32		siCasulty;			// 사상자 숫자. 
	DWORD		uiLeftTime;			// 남은 시간. (ms) 
	DWORD		dwWarStartClock;
	DWORD		dwWarOsakaClock;	// 왜구 전쟁승리 후 오사카 전쟁을 시작한다
	DWORD		dwNewWarUnitClock;	// 4방향에 나올 왜구의 시간체크~
	SI32		siLeftMon;			// 남은 몬스터 숫자.
	
	SI32		siPorterVillageUnique;	// 짐꾼 왜구가 배치될 마을. 

	//------------
	// 출력 
	//------------
	bool bDrawSwitch;
	SI32 siDrawCmdFont;
	DWORD dwDrawStartClock;
	
	cltWarManager();
	~cltWarManager();

	// 전쟁을 기획한다. 
	bool MakeWar(DWORD clock);
	// 전쟁 몬스터를 추가한다. 
	void AddWarMonNum(SI32 kind, SI32 monnum);

	
	// 전쟁을 종결 짓는다
	void WarEnd();

	// 전쟁 액션 (서버) 
	void Action();
	
	void SendWarMsg(SI32 cmd, SI32 mapindex1, SI32 x1, SI32 y1, SI32 mapindex2, SI32 x2, SI32 y2,SI16 vill1, SI16 kill1, SI16 vill2, SI16 kill2, SI16 vill3, SI16 kill3, SI16 portervillageunique);
	

	void SetDraw(SI32 cmd);
	// 전황을 표시한다. (클라이언트) 
	void Draw(LPDIRECTDRAWSURFACE7 lpsurface, SI32 rx, SI32 ry);

};
#endif