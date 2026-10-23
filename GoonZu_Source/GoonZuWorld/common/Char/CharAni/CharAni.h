//---------------------------------
// 2003/5/24 김태곤
//---------------------------------

#ifndef _CHARANI_H
#define _CHARANI_H

#include "TSpr.h"
#include "NArray.h"

//-------------------------------------------------
// AniAtb
//-------------------------------------------------
//  2 Direction Animation - 1 Dir mirror 
#define ANIATB_DIR2								1		
// 16 Direction Animation - 9 Dir mirror
#define ANIATB_DIR16					 	    2		
// Animation ONLY once
#define ANIATB_ONLYONCE							8		
// Single Frame
#define ANIATB_SINGLEFRAME					    16		

//----------------------------------------------------
//  AniType
//----------------------------------------------------
#define MAX_ANINUM_PER_KIND						50		// 한 종류의 캐릭터가 가질 수 있는 최대 애니메이션 수. 

#define ANITYPE_ATTACK							1
#define ANITYPE_DYING							2
#define ANITYPE_HEAL							3
#define ANITYPE_MOVE							4
#define ANITYPE_WAIT0							5
#define ANITYPE_WAIT1							6
#define ANITYPE_EXP								7
#define ANITYPE_HIT								8
#define ANITYPE_MAGIC							9
#define ANITYPE_FISH							10
#define ANITYPE_FARM							11
#define ANITYPE_ATTACKSHADOW					12		// 공격 애니시 같이 찍을 애니메이션
#define ANITYPE_ATTACK1							13
#define ANITYPE_ATTACK2							14
#define ANITYPE_MAGICSHADOW						15		// 마법 애니시 같이 찍을 애니메이션
#define ANITYPE_DYINGSHADOW						16		// 죽을 때 같이 찍을 애니메이션
#define ANITYPE_MOVESHADOW						17		// 움직일 때 같이 찍을 애니메이션
#define ANITYPE_WAIT0SHADOW						18		// 제자리동작 0일 때 같이 찍을 애니메이션
#define ANITYPE_WAIT1SHADOW						19		// 제자리동작 1일 때 같이 찍을 애니메이션
#define ANITYPE_HITSHADOW						20		// 피격시 같이 찍을 애니메이션
#define ANITYPE_MINE							21		// 채굴
#define ANITYPE_ATTACKSHADOW1					22		// 공격 애니시 같이 찍을 애니메이션 
#define ANITYPE_SITDOWN							23		// 앉기 동작
#define ANITYPE_MAKEARMOR						24		// 방어구 제작 동작
#define ANITYPE_MAKEHEAL						25		// 약 제작 동작
#define ANITYPE_MAKEFOOD						26		// 음식 제작 동작
#define ANITYPE_CRY								27		// 과금 소환펫 우는 동작
#define ANITYPE_VERYCRY							28		// 과금 소환펫 펑펑 우는 동작
#define ANITYPE_PLAY							29		// 과금 소환펫 재주 동작
#define ANITYPE_SMILE							30		// 과금 소환펫 웃는 동작
#define ANITYPE_SURPRISE						31		// 과금 소환펫 놀라는 동작
#define ANITYPE_ANGRY							32		// 과금 소환펫 화내는 동작
#define ANITYPE_CATANGRY						33		// 고양이가 땅을 보며 모른척 한다
#define ANITYPE_GOOD							34		// 고양이가 고개를 끄덕인다
#define ANITYPE_SIT								35		// 고양이가 앉는다
#define ANITYPE_HAND							36		// 고양이가 한쪽 손을 내민다
#define ANITYPE_FEATHER							37		// 허공에 깃털이 생기며 고양이가 깃털을 먹음
#define ANITYPE_SLEEP							38		// 이블 - 잔다.
#define ANITYPE_EAT								39		// 루돌프가 주저앉아 도시락을 먹는다.
#define ANITYPE_EMIT							40		// 루돌프의 코에서 빨간빛이 나온다.
#define ANITYPE_ROUDOLPHCRY						41		// 루돌프가 주저앉아 엉엉운다.
#define ANITYPE_ROUDOLPHDANCE					42		// 루돌프가 춤을 춘다.
#define	ANITYPE_ROUDOLPHSMILE					43		// 루돌프가 박수를 치며 환하게 웃는다.
#define	ANITYPE_ROUDOLPHSUPRISE					44		// 루돌프가 깜짝놀란다.
#define ANITYPE_SOULGUARDSF						45		// 수호정령 특수능력 발동 모션
#define ANITYPE_SLEEPCAT						46		// 잠자는 냐옹이 방석 모션
#define ANITYPE_SOULGUARDSFSHADOW				47		// 수호정령 특수능력 발동 모션(이펙트)





#define MAX_IMG_FILE_PER_KIND					6		// 한 종류의 캐릭터가 가질 수 있는 최대 이미지 파일 수. 
#define MAX_ATTACKTYPE_PER_KIND					5		// 최대 공격동작 타입

class cltCharAnimationManager;
class TSpr;

//--------------------------------------------------
// 개별적인 캐릭터가 보유하게 되는 애니메이션 정보. 
//--------------------------------------------------
class cltAniInfo{
public:
	SI32	siAniStep;
	SI32	siAniDelayor;
	SI32	siAniDelay;
	SI32	siAniInstant;
	DWORD	dwElapsedClock;			// 애니 시작부터의 시간 
	DWORD	dwAniStartClock;		// 애니 시작 시간. 

	SI32	siRetryNum;				// 애니를 반복 완료한 회수. 
public:
	
	cltAniInfo();
	~cltAniInfo();

	void Init1();
	BOOL IsInstant()const;

	void SetAniStep(SI32 step);
	SI32 GetAniStep() const;
	void IncreaseAniStep(SI32 amount);
	void DecreaseAniStep(SI32 amount);
	
	BOOL IsLast() const;
	SI32 GetAniDelayor() const;
	void SetAniDelayor(SI32 delayor);
	void IncreaseAniDelayor(SI32 amount);
	void SetAniDelay(SI32 delay);
	SI32 GetAniDelay() const;
	
	void SetAniInstant(SI32 instant);
	SI32 GetAniInstant() const;
};

//----------------------------------------------
// 개별적인 캐릭터 애니메이션 정보 
//----------------------------------------------
class cltCharAnimation {

public:
	BOOL  bDirSwitch;

private:
	cltCharAnimationManager* pclAniMng;

	SI32	siRef;
	SI32	siAniType;
    SI32	siFrameNum;

public:
	SI32	siFont;
	SI32	siInstant;
	SI32	siDelay;
	SI32	siAtb;
	SI32	siEffectFont;
	SI32	siTransMode;
	SI08	siAlphaValue;

	SI32	siShadowAni;
	SI32	siShadowFont;

	SI32	siShadowXIndent;
	SI32	siShadowYIndent;

	DWORD dwLastUseClock;	// 마지막으로 사용된 시간. 

	TSpr* pSpr;

	cltCharAnimation(cltCharAnimationManager* pclAniMng);
	~cltCharAnimation();

	SI32 GetAniType() const;

	void Set(SI32 sianitype, BOOL dirswitch, SI32 framenumber, TCHAR* filename,
			 SI32 font, SI32 instant, SI32 delay, SI32 atb, SI32 EffectFont ,SI32 transmode = 0,
			 SI08 alphavalue = 0 , SI32 ShadowAni = 0 ,SI32 ShadowFont = 0 );

	// 이미지의 가로 세로 사이즈를 구한다. 
	SI32 GetImageXsize(DWORD currentclock);
	SI32 GetImageYsize(DWORD currentclock);

	// 출력할 이미지의 그래픽 정보를 획득한다. 
	void DecideDrawImg(SI32 dir, cltAniInfo claniinfo, DWORD currentclock, TSpr** ppSpr, int* return_font,
					   bool * return_reverseswitch, SI32 *psiEffectFont,
					   SI32 * psiShadowAni=0,SI32 * psiShadowFont=0);
	
	BOOL IsReady(SI32 step);

	void SetShadowIndentXY(SI32 psiX , SI32 psiY) ;
	void GetShadowIndentXY(SI32 * psiX , SI32 * psiY) ;

	SI32 GetFrameNum();

	// 이미지를 로드한다. 이미 로딩된 경우에는 로드하지 않는다. 
	BOOL Load(DWORD currentclock);
};

//-------------------------------------------
// 캐릭터 애니메이션을 관리하는 메니저.
//-------------------------------------------
class cltCharAnimationManager{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<TCHAR*, MAX_IMG_FILE_PER_KIND>		pszFileName;
	NSafeTArray<TSpr*, MAX_IMG_FILE_PER_KIND>		pSpr;
#else
	TCHAR* pszFileName[MAX_IMG_FILE_PER_KIND];
	TSpr* pSpr[MAX_IMG_FILE_PER_KIND];	// 애니메이션이 구현될 실제 이미지 데이터 저장용 
#endif

#ifdef _SAFE_MEMORY
	NSafeTArray<cltCharAnimation*, MAX_ANINUM_PER_KIND>		pclCharAni;
#else
	cltCharAnimation*		pclCharAni[MAX_ANINUM_PER_KIND];
#endif

	cltCharAnimationManager();
	~cltCharAnimationManager();

	BOOL Add(SI32 Ani, BOOL DirSwitch, TCHAR* FileName, SI32 Font, SI32 Frame,
			 SI32 Instant, SI32 Delay, SI32 atb, SI32 EffectFont ,SI32 transmode = 0,SI08 alphavalue = 0 ,
			 SI32 ShadowAni = 0 , SI32 ShadowFont = 0 );
	BOOL DecideDrawImg(SI32 anitype, SI32 dir, cltAniInfo claniinfo, DWORD currentclock, TSpr** ppspr,
					   int* return_font, bool * return_reverseswitch, SI32 *psiEffectFont ,
					   SI32 * psiShadowAni=NULL,SI32 * psiShadowFont=NULL);

	BOOL GetDirSwitch(SI32 anitype);
	SI32 FindRefFromAnitype(SI32 anitype);

	SI32 FindFirstAni();
#ifdef _SAFE_MEMORY
	SI32 GetDelay(SI32 ref);
#else
	SI32 GetDelay(SI32 ref)		const;
#endif
	SI32 GetFrameNum(SI32 ref);
#ifdef _SAFE_MEMORY
	SI32 GetInstant(SI32 ref);
#else
	SI32 GetInstant(SI32 ref)	const;
#endif
	SI32 GetImageXsize(SI32 ref, DWORD currentclock );
	SI32 GetImageYsize(SI32 ref, DWORD currentclock );

	// 사용되지 않는 파일은 제거한다. 
	void DeleteIfNotUse(DWORD curclock);

	BOOL SetShadowIndentXY(SI32 anitype ,SI32 psiX , SI32 psiY) ;
	void GetShadowIndentXY(SI32 anitype ,SI32 * psiX , SI32 * psiY) ;

	static SI32 siLodedSize;

};

#endif

