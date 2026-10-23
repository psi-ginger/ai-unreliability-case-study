#ifndef _SMOKE_H
#define _SMOKE_H

/*
#define MAX_SMOKE_KIND_NUMBER		90

#define SMOKE_GUN 					1
#define SMOKE_SINK      			2
#define SMOKE_EXP1					3
#define SMOKE_EXP2     				4
#define SMOKE_GROUNDFIRE			5
#define SMOKE_KIMBOMB1				6
#define SMOKE_KIMBOMB2				7
#define SMOKE_KIMBOMB3				8
#define SMOKE_LIGHTNING  			9
#define SMOKE_GHOST					10
#define SMOKE_HORSEDUST				11
#define SMOKE_CANNONEXP				12
#define SMOKE_FIRESMOKE				13			// 불연기 // 2003.10.29
#define SMOKE_RAREITEMSMOKE1		14			// 명품 아이템 반짝이
#define SMOKE_RAREITEMSMOKE2		15			// 명품 아이템 반짝이
#define SMOKE_RAREITEMSMOKE3		16			// 명품 아이템 반짝이
#define SMOKE_RAREITEMSMOKE4		17			// 명품 아이템 반짝이
#define SMOKE_FIREBALLHIT			18
#define SMOKE_CASTLEBOOM			19
#define SMOKE_SELFDESTUCT			20
#define SMOKE_ICESMOKE				21
#define SMOKE_JARSMOKE				22
#define SMOKE_WINDBLADE				23			// 하회탈
#define SMOKE_TREEMON				24			// 괴수목
#define SMOKE_VINE					25			// 외눈넝쿨
#define SMOKE_GHOSTFIRE				26			// 도령귀신
#define SMOKE_DRAGONEXPLOSION		27
#define SMOKE_DRAGONEXPTAIL			28
#define SMOKE_SIMTORYONGSTONE		29			// 심토룡
#define SMOKE_KINGCUTTLEFISH		30			// 대왕오적어
#define SMOKE_BEGINWARP				31			// WARP 시작
#define SMOKE_ENDWARP				32			// WARP 종료 
#define SMOKE_CRANE_TAIL			33			// 탄광삽차 꼬리
#define SMOKE_CRANE_HIT				34			// 탄광삽차 타격
#define SMOKE_SELFBOMBWHALE			35			// 폭경 자폭
#define SMOKE_THROWKNIFE_RIGHT		36			// 표창닌자 마법 꼬리 오른쪽
#define SMOKE_THROWKNIFE_LEFT		37			// 표창닌자 마법 꼬리 왼쪽
#define SMOKE_KINGCRAB_TAIL			38			// 대왕소라게 꼬리
#define SMOKE_DEADMESSENGER			39			// 저승사자
#define SMOKE_FIRGROUNDTAIL			40	
#define SMOKE_FIREGROUNDEXPLOSION	41
#define SMOK_MASKMONK_STONE			42			// 중탈 떨어지는 돌
#define SMOK_MASKMONK_DROPSTONE		43			// 중탈 떨어진 돌
#define SMOKE_PERSONCANNON_HIT		44			// 일인포탄 터지는 것
#define SMOKE_MAGICFREEZEEXPLOSION	45			// 대포마법 터지는 것

#define SMOKE_STAFFSCROLL_HIT		46			// 지팡이술 발린 지팡이에 맞았을 때
#define SMOKE_STAFFSCROLL_TAIL		47			// 지팡이술 발린 지팡이에서 나가는 공격의 꼬리

#define SMOKE_GUNSCROLL_HIT			48			// 총술 발린 지팡이에 맞았을 때
#define SMOKE_GUNSCROLL_TAIL		49			// 총술 발린 지팡이에서 나가는 공격의 꼬리

#define SMOKE_BOWSCROLL_HIT			50			// 궁술 발린 지팡이에 맞았을 때
#define SMOKE_BOWSCROLL_TAIL		51			// 궁술 발린 지팡이에서 나가는 공격의 꼬리

#define SMOKE_FOAM_HIT				52			// 먹깨비 히트
#define SMOKE_MORTAL_FIRE			53			// 회전절구차 공격

#define SMOKE_WATERARROW_TAIL		54			// 물화살 꼬리
#define SMOKE_WATERARROW_HIT		55			// 물화살 히트

#define SMOKE_MAGIC_PALMWIND_HIT	56			// 사명대사 장풍마법 히트

#define SMOKE_DOWNING_VINE			57			// 넝쿨 흩어지는 모습

#define SMOKE_PALMWIND_TAIL			58			// 사명대사 장풍 꼬리

#define SMOKE_HORSE_TICKET_EFFECT	59			// 말능력향상이용권 썼을 대 타면

#define SMOKE_MAGIC_SKELLGHOST_TAIL 60			// 해골유령 마법 꼬리
#define SMOKE_MAGIC_SKELLGHOST_FIRE	61			// 해골유령 마법 발사

#define SMOKE_SWORDMETEOR_EXPLOSION	62			// 낙하수리검 터지는 거
#define SMOKE_SWORDMETEOR_TAIL		63			// 낙하수리검 꼬리
#define SMOKE_SWORDMETEOR_HIT		64			// 낙하수리검 꼽힌 것

#define SMOKE_MAIGCFROZEN_TAIL		65			// 보스의 얼음 마법 꼬리
#define SMOKE_ICESTONE_HIT			66			// 얼음성 보스의 타격

#define SMOKE_TWINSPEAR_TAIL		67			// 양날창 꼬리

#define SMOKE_FIREWHIRLWIND_HIT		68			// 돌개바람 마법 맞음

#define SMOKE_NORTHWIND_HIT			69			// 삭풍 맞음

#define SMOKE_NEW_STAFFSCROLL_HIT		70			// 지팡이술(2장) 발린 지팡이에 맞았을 때
#define SMOKE_NEW_STAFFSCROLL_TAIL		71			// 지팡이술(2장) 발린 지팡이에서 나가는 공격의 꼬리

#define SMOKE_NEW_GUNSCROLL_HIT			72			// 총술(2장) 발린 지팡이에 맞았을 때
#define SMOKE_NEW_GUNSCROLL_TAIL		73			// 총술(2장) 발린 지팡이에서 나가는 공격의 꼬리

#define SMOKE_NEW_BOWSCROLL_HIT			74			// 궁술(2장) 발린 지팡이에 맞았을 때
#define SMOKE_NEW_BOWSCROLL_TAIL		75			// 궁술(2장) 발린 지팡이에서 나가는 공격의 꼬리

#define SMOKE_FIRESTORM_TAIL			76			// 심화조 마법 꼬리

#define SMOKE_SPEAR_MANA_TAIL			77			// 관우 공격 꼬리
#define SMOKE_SPEAR_DRAGON_TAIL			78			// 관우 마법 꼬리
#define SMOKE_SPEAR_DRAGON_HIT			79			// 관우 공격 히트

#define SMOKE_BLACKSPIDER_HIT			80			// 흑거미 공격 히트
#define SMOKE_PRIEST_HIT				81			// 타락 성직자 공격 히트
#define SMOKE_GIRAFFE_HIT				82			// 기린 공격 히트

#define SMOKE_MINEGIRL_HIT				83			// 탄광소녀 공격 히트

#define SMOKE_CATRANGE_HIT				84			// 원거리 고양이 공격 히트
*/
	
//#define MAX_SMOKE_KIND_NUMBER		80

enum SMOKE_TYPE
{
	SMOKE_GUN					= 1
	,SMOKE_SINK
	,SMOKE_EXP1
	,SMOKE_EXP2
	,SMOKE_GROUNDFIRE
	,SMOKE_KIMBOMB1
	,SMOKE_KIMBOMB2
	,SMOKE_KIMBOMB3
	,SMOKE_LIGHTNING
	,SMOKE_GHOST

	,SMOKE_HORSEDUST
	,SMOKE_CANNONEXP
	,SMOKE_FIRESMOKE					// 불연기 // 2003.10.29
	,SMOKE_RAREITEMSMOKE1				// 명품 아이템 반짝이
	,SMOKE_RAREITEMSMOKE2				// 명품 아이템 반짝이
	,SMOKE_RAREITEMSMOKE3				// 명품 아이템 반짝이
	,SMOKE_RAREITEMSMOKE4				// 명품 아이템 반짝이
	,SMOKE_FIREBALLHIT	
	,SMOKE_CASTLEBOOM	
	,SMOKE_SELFDESTUCT

	,SMOKE_ICESMOKE	
	,SMOKE_JARSMOKE	
	,SMOKE_WINDBLADE					// 하회탈
	,SMOKE_TREEMON						// 괴수목
	,SMOKE_VINE							// 외눈넝쿨
	,SMOKE_GHOSTFIRE					// 도령귀신
	,SMOKE_DRAGONEXPLOSION
	,SMOKE_DRAGONEXPTAIL
	,SMOKE_SIMTORYONGSTONE				// 심토룡
	,SMOKE_KINGCUTTLEFISH				// 대왕오적어

	,SMOKE_BEGINWARP					// WARP 시작
	,SMOKE_ENDWARP						// WARP 종료 
	,SMOKE_CRANE_TAIL					// 탄광삽차 꼬리
	,SMOKE_CRANE_HIT					// 탄광삽차 타격
	,SMOKE_SELFBOMBWHALE				// 폭경 자폭
	,SMOKE_THROWKNIFE_RIGHT				// 표창닌자 마법 꼬리 오른쪽
	,SMOKE_THROWKNIFE_LEFT				// 표창닌자 마법 꼬리 왼쪽
	,SMOKE_KINGCRAB_TAIL				// 대왕소라게 꼬리
	,SMOKE_DEADMESSENGER				// 저승사자
	,SMOKE_FIRGROUNDTAIL

	,SMOKE_FIREGROUNDEXPLOSION
	,SMOK_MASKMONK_STONE				// 중탈 떨어지는 돌
	,SMOK_MASKMONK_DROPSTONE			// 중탈 떨어진 돌
	,SMOKE_PERSONCANNON_HIT				// 일인포탄 터지는 것
	,SMOKE_MAGICFREEZEEXPLOSION			// 대포마법 터지는 것
	,SMOKE_STAFFSCROLL_HIT				// 지팡이술 발린 지팡이에 맞았을 때
	,SMOKE_STAFFSCROLL_TAIL				// 지팡이술 발린 지팡이에서 나가는 공격의 꼬리
	,SMOKE_GUNSCROLL_HIT				// 총술 발린 지팡이에 맞았을 때
	,SMOKE_GUNSCROLL_TAIL				// 총술 발린 지팡이에서 나가는 공격의 꼬리
	,SMOKE_BOWSCROLL_HIT				// 궁술 발린 지팡이에 맞았을 때

	,SMOKE_BOWSCROLL_TAIL				// 궁술 발린 지팡이에서 나가는 공격의 꼬리
	,SMOKE_FOAM_HIT						// 먹깨비 히트
	,SMOKE_MORTAL_FIRE					// 회전절구차 공격
	,SMOKE_WATERARROW_TAIL				// 물화살 꼬리
	,SMOKE_WATERARROW_HIT				// 물화살 히트
	,SMOKE_MAGIC_PALMWIND_HIT			// 사명대사 장풍마법 히트
	,SMOKE_DOWNING_VINE					// 넝쿨 흩어지는 모습
	,SMOKE_PALMWIND_TAIL				// 사명대사 장풍 꼬리
	,SMOKE_HORSE_TICKET_EFFECT			// 말능력향상이용권 썼을 대 타면
	,SMOKE_MAGIC_SKELLGHOST_TAIL		// 해골유령 마법 꼬리

	,SMOKE_MAGIC_SKELLGHOST_FIRE		// 해골유령 마법 발사
	,SMOKE_SWORDMETEOR_EXPLOSION		// 낙하수리검 터지는 거
	,SMOKE_SWORDMETEOR_TAIL				// 낙하수리검 꼬리
	,SMOKE_SWORDMETEOR_HIT				// 낙하수리검 꼽힌 것
	,SMOKE_MAIGCFROZEN_TAIL				// 보스의 얼음 마법 꼬리
	,SMOKE_ICESTONE_HIT					// 얼음성 보스의 타격
	,SMOKE_TWINSPEAR_TAIL				// 양날창 꼬리
	,SMOKE_FIREWHIRLWIND_HIT			// 돌개바람 마법 맞음
	,SMOKE_NORTHWIND_HIT				// 삭풍 맞음
	,SMOKE_NEW_STAFFSCROLL_HIT			// 지팡이술(2장) 발린 지팡이에 맞았을 때

	,SMOKE_NEW_STAFFSCROLL_TAIL			// 지팡이술(2장) 발린 지팡이에서 나가는 공격의 꼬리
	,SMOKE_NEW_GUNSCROLL_HIT			// 총술(2장) 발린 지팡이에 맞았을 때
	,SMOKE_NEW_GUNSCROLL_TAIL			// 총술(2장) 발린 지팡이에서 나가는 공격의 꼬리
	,SMOKE_NEW_BOWSCROLL_HIT			// 궁술(2장) 발린 지팡이에 맞았을 때
	,SMOKE_NEW_BOWSCROLL_TAIL			// 궁술(2장) 발린 지팡이에서 나가는 공격의 꼬리
	,SMOKE_FIRESTORM_TAIL				// 심화조 마법 꼬리
	,SMOKE_SPEAR_MANA_TAIL				// 관우 공격 꼬리
	,SMOKE_SPEAR_DRAGON_TAIL			// 관우 마법 꼬리
	,SMOKE_SPEAR_DRAGON_HIT				// 관우 공격 히트
	,SMOKE_BLACKSPIDER_HIT				// 흑거미 공격 히트

	,SMOKE_PRIEST_HIT					// 타락 성직자 공격 히트
	,SMOKE_GIRAFFE_HIT					// 기린 공격 히트
	,SMOKE_MINEGIRL_HIT					// 탄광소녀 공격 히트
	,SMOKE_CATRANGE_HIT					// 원거리 고양이 공격 히트
	,SMOKE_BEARTANG_HIT					// 탱이 공격 히트
	,SMOKE_SUPERBEARTANG_HIT			// 슈퍼탱이 공격 히트
	//,SMOKE_GREATWALLASURA_HIT			// 버려진요새 아수라 공격 히트
	,SMOKE_MARRIED_FULLDRESS			// 결혼 예복 반짝이 이펙트 	

	//**************************************************************//
	//
	// [영훈] 스모크 추가는 이 위로 해주세요
	//
	//**************************************************************//
	,MAX_SMOKE_KIND_NUMBER
};

// 연기의 애니메이션 속도의 상수.
#define	SMOKE_DELAY_1	1
#define SMOKE_DELAY_2	2
#define SMOKE_DELAY_3	3
#define	SMOKE_DELAY_4	4
#define	SMOKE_DELAY_5	5
#define	SMOKE_DELAY_10	10

#define ORDER_SMOKE_EXP     1

#define MAX_SMOKE_FILE_NAME_LENGTH	64

#include <Graphic.h>
#include <Graphic_Capsule.h>

class cltMapCommon;

//-------------------------------------------
// cltSmokeKindInfo
//-------------------------------------------
class cltSmokeKindInfo
{
public:
	cltSmokeKindInfo();
	~cltSmokeKindInfo();
	void Init( TCHAR *filename, SHORT startfont, SHORT moveframenumber, SHORT maxrepeatnumber, SI32 siStepDelay, SHORT risingswitch, SI32 transparentmode, SI08 siAlpha, SI32 siyindent,bool direction = false);
	void LoadSpr();

	TCHAR szFileName[MAX_SMOKE_FILE_NAME_LENGTH];
	TSpr Spr;

	SI32  StartFont;
	SHORT MoveFrameNumber;
	SHORT MaxRepeatNumber;
	SHORT RisingSwitch;
	SI32  TransparentMode;
	SI32  m_siStepDelay;
	SI32 siYIndent;
	SI08 siAlpha;
	bool bDirection; // 16방위로 해야하는가
};

//--------------------------------------
// cltSmokeCommon
//--------------------------------------
class cltSmokeCommon{
public:
	cltSmokeKindInfo* pclSKI;
	cltMapCommon* pclMap;

	//----------------
	// 신분관련 
	//----------------
	SI32	ID;
	SI32	Kind;
	SI32	GroundKind; // 이놈이 존재하면 이놈을 한번 더 그려준다.

	//--------------------
	// 좌표 
	///-------------------
	SI32 RealX, RealY;
	SI32 X, Y;
	SI32 CenterX, CenterY;

	//--------------------
	// 행동 
	//--------------------
	SI32  CurrentOrder;               // 현재 캐릭터가 하고 있는 작업 
	SI32	Animation;
	SI32	Step;

	//--------------------
	// 생명 기간.
	//--------------------
	SI32 MaxRepeatNumber;
	SI32 RepeatNumber;
	SI32  m_siStepDelay;			// 애니메이션의 속도.
	DWORD LastChangedFrame;           // 애니메이션이 변경된 프레임 
	SI32 MoveFrameNumber;

	//--------------------
	// 공격 
	//--------------------
	SI32 AttackID;
	SI32 Apower;

	//--------------------
	// 옵션 
	//--------------------
	BOOL RisingSwitch;
	
	//--------------------
	// 방향성
	//-------------------
	SI32 TempDirection;

public:
	virtual void Init(){};
	virtual BOOL Draw(){return FALSE;};

	
	cltSmokeCommon();
	void Init(SHORT kind, SHORT id, SHORT realx, SHORT realy, cltMapCommon* pmap,
			  cltSmokeKindInfo* pclinfo,SI32 tempdirection = 0 ,SI32 groundkind = 0 );
	BOOL Action();
	void CalculateRealXY();

};

//---------------------------------------
// cltSmokeServer
//---------------------------------------
class cltSmokeServer : public cltSmokeCommon
{
public:

	cltSmokeServer();

	void Init();


};

//---------------------------------------
// cltSmokeClient
//---------------------------------------
class cltSmokeClient : public cltSmokeCommon
{
public:
	SI32 StartFont;
	TSpr* pSpr;

	SI32	Font;
	SI32  TransparentMode;          // 광원효과를 넣을 것인가 ?
	bool DrawSwitch;                   // 그려줄 것인가 말것인가?
	bool ReverseSwitch;                // 뒤집어서 찍을 것인가?
	SI32  DrawStartX, DrawStartY;      // 그림이 그려지는 시작 위치
	bool DirectionMode;					// 16방위로 찍어야 하는가?
	SI08 siAlphaValue;				// Alpha 값

	cltSmokeClient();

	void Init();
	BOOL Draw();
	void DecideFont();
	void DecideFontMove();
	void CalculateDrawStartXY();

};

//----------------------------------------------
// cltSmokeManager
//----------------------------------------------
class cltSmokeManager{
public:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltSmokeCommon*, MAX_SMOKE_NUMBER>			pclSmoke;
	NSafeTArray<cltSmokeKindInfo, MAX_SMOKE_KIND_NUMBER>	SKI;

	NSafeTArray<SI32, MAX_SMOKE_NUMBER>						SmokeInfo;
#else
	cltSmokeCommon*		pclSmoke[MAX_SMOKE_NUMBER];   // 연기용 데이터 
	cltSmokeKindInfo	SKI[MAX_SMOKE_KIND_NUMBER];

	SI32 SmokeInfo[MAX_SMOKE_NUMBER]; // 연기에 대한 정보를 저장할 공간 
#endif

private:
	void InitSmokeKindInfo();
	void DeleteSmoke(SI32 id);
	SI32 FindEmptySmoke();

public:
	cltSmokeManager(SI32 gamemode);
	~cltSmokeManager();
	SI32 SetSmoke(SI16 siKind, SI16 x, SI16 y, cltMapCommon* pmap,SI32 tempdirection = 0 ,SI32 groundkind = 0 );
	void Draw();
	void Action();
	void DeleteSmoke();

	int TempDirection; // 16방위

};

#endif









