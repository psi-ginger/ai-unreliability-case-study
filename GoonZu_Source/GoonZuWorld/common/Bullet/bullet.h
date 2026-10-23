//---------------------------------
// 2003/7/4 김태곤
//---------------------------------
#ifndef _BULLET_H
#define _BULLET_H

#include <CommonHeader.h>
#include "..\Map\Map.h"
#include <TSpr.h>

#define MAX_SHOOT_RANGE							200     

//------------------------------------------------------------------------------------------------
// 총알의 작동 
//------------------------------------------------------------------------------------------------
#define ORDER_BULLET_MOVE						1
#define ORDER_BULLET_EXP     					2

//------------------------------------------------------------------------------------------------
// 총알의 애니메이션 동작. 
//------------------------------------------------------------------------------------------------
#define ANI_BULLET_MOVE							1
#define ANI_BULLET_WAIT							2



//------------------------------------------------------------------------------------------------
// 총알 종류 
//------------------------------------------------------------------------------------------------
// [영훈] 기존 define를 enum으로 변경

enum BULLETKIND
{
	BULLETKIND_NONE					= 0	// 아무것도 아님

	,BULLETKIND_ARROW				= 1
	,BULLETKIND_GUN						
	,BULLETKIND_CAKE					
	,BULLETKIND_EGG						
	,BULLETKIND_CANNONBALL				
	,BULLETKIND_TURTLEBACKSHELL			
	,BULLETKIND_CATFISH					
	,BULLETKIND_LADYBUGFAN				
	,BULLETKIND_SNOW					
	,BULLETKIND_STAFF1					

	,BULLETKIND_MADOGHOSTFIRE			
	,BULLETKIND_JAPANHEADER				
	,BULLETKIND_TOHANG					
	,BULLETKIND_SKULLBATFIRE			
	,BULLETKIND_FIREGOBLIN				
	,BULLETKIND_CASTLECANNONBALL		
	,BULLETKIND_SELFBOMB				
	,BULLETKIND_KNIFE					
	,BULLETKIND_GREENFIRE				
	,BULLETKIND_BLACKINK				

	,BULLETKIND_WINDBLADE				
	,BULLETKIND_SHELLFISHBOMB			
	,BULLETKIND_MAGICFIREBALL			
	,BULLETKIND_MAGICBIGWIND			
	,BULLETKIND_MAGICPOISON				
	,BULLETKIND_MAGICICE				
	,BULLETKIND_MAGICLIGHTINGFIST		
	,BULLETKIND_BOOMERANG				// 임수로 부메랑을 하나 만들었따.
	,BULLETKIND_TREEMON					// 괴수목 - 가짜총알 사용(SUNKEN)
	,BULLETKIND_VINE					// 외눈넝쿨 - 가짜총알 사용(SUNKEN)

	,BULLETKIND_GHOSTFIRE				// 도령귀신
	,BULLETKIND_SIMTORYONG				// 심토룡
	,BULLETKIND_KINGCUTTLEFISH			// 대왕오적어
	,BULLETKIND_CRANE					// 탄광삽차
	,BULLETKIND_SELFBOMBWHALE			// 폭경 자폭
	,BULLETKIND_MONKEYDRILL				// 탄광원숭이
	,BULLETKIND_MAGICCURSEDEFENSE		// 방어력   마법 총알 
	,BULLETKIND_MAGICCURSEMOVESPEED		// 이동속도 마법 총알
	,BULLETKIND_BUTCHER					// 제주소잡이
	,BULLETKIND_STONEOLDMAN				// 돌하르방

	,BULLETKIND_MAGICTHROWKNIFE_RIGHT	// 표창닌자 마법 총알 오른쪽
	,BULLETKIND_MAGICTHROWKNIFE_LEFT	// 표창닌자 마법 총알 왼쪽
	,BULLETKIND_KINGCRAB				// 대왕소라게
	,BULLETKIND_FIREARROW				// 불화살
	,BULLETKIND_LOBSTER					// 잠수가재
	,BULLETKIND_DEADMESSENGER			// 저승사자
	,BULLETKIND_MASKMONK				// 중탈
	,BULLETKIND_PERSONCANNON			// 일인대포
	,BULLETKIND_CANNON_MAGIC			// 최무선 마법
	,BULLETKIND_BOW_SCROLL				// 궁술부적(1장) 발렷을 때

	,BULLETKIND_GUN_SCROLL				// 궁술부적(1장) 발렷을 때
	,BULLETKIND_STAFF_SCROLL			// 궁술부적(1장) 발렷을 때
	,BULLETKIND_FOAM_FIRE				// 먹깨비
	,BULLETKIND_FIREBALL				// 일반공격
	,BULLETKIND_MORTAL					// 근거리 공격할 때 주변부로 퍼지는 것처럼 보이게 함 - 회전절구차
	,BULLETKIND_WATERARROW				// 물화살
	,BULETKIND_MAGICPALMWIND			// 사명대사 장풍마법
	,BULLETKIND_STAFFWIND				// 사명대사 장풍
	,BULLETKIND_FLYVINE					// 날아가는 넝쿨(덩쿨?)
	,BULLETKIND_MAGICSKELLGHOST			// 날아가는 해골유령

	,BULLETKIND_SKELLFISH				// 날아가는 생선뼈
	,BULLETKIND_MAGICFROZEN				// 보스 얼음 공격
	,BULLETKIND_ICESTONE				// 얼음성 보스
	,BULLETKIND_TWINSPEAR				// 양날창
	,BULLETKIND_NORTHWIND				// 삭풍
	,BULLETKIND_NEW_BOW_SCROLL			// 궁술부적(2장) 발렷을 때
	,BULLETKIND_NEW_GUN_SCROLL			// 궁술부적(2장) 발렷을 때
	,BULLETKIND_NEW_STAFF_SCROLL		// 궁술부적(2장) 발렷을 때
	,BULLETKIND_FIRESTORM				
	,BULLETKIND_ENERGYARROW				// 독안괴
	,BULLETKIND_SPEAR_MANA				// 관우 일반 공격
	,BULLETKIND_SPEAR_DRAGON			// 관우 마법 공격

	,BULLETKIND_BLACKBOSS				// 검은 전사 두목
	,BULLETKIND_OCTOPUS					// 문어
	,BULLETKIND_OTOHIME					
	,BULLETKIND_BLACKSPIDER				// 흑거미
	,BULLETKIND_EVILEYE					// 이블아이
	,BULLETKIND_GARGOYLE				// 가고일 마법
	,BULLETKIND_CERBERUS				// 켈베로스 마법
	,BULLETKIND_FROZENSORCERER			// 프로즌 소서러 일반
	,BULLETKIND_MARIONETTE				
	,BULLETKIND_FROKEN					// 기계공 프로켄

	,BULLETKIND_ICEHOUND				// 아이스하운드
	,BULLETKIND_FROZENKNIGHT			// 프로즌 나이트
	,BULLETKIND_FROZENSORCERER_MAGIC	// 프로즌 소서러 마법
	,BULLETKIND_NINJAPANDA				// 닌자판다 마법
	,BULLETKIND_RACCOONDOG				// 둔갑너구리 마법
	,BULLETKIND_BOSSMONKEY				// 보스 원숭이 마법
	,BULLETKIND_CANNONMOL				// 캐논몰(토행요괴)
	,BULLETKIND_BLOODBAT				// 흡혈박쥐
	,BULLETKIND_WEREWOLF				// 늑대인간
	,BULLETKIND_DEVILOUS				// 데빌러스

	,BULLETKIND_GANGSI					// 강시
	,BULLETKIND_PRIEST					// 타락성직자
	,BULLETKIND_HARPY					// 타락성직자
	,BULLETKIND_GIRAFFE					// 기린~
	,BULLETKIND_PAPAGOBLIN				// 기린~
	,BULLETKIND_MINEGIRL				// 탄광소녀
	,BULLETKIND_GREENTURTLE				// 초원거북
	,BULLETKIND_MAGICALRACCOON			// 메지컬모후
	,BULLETKIND_MAMARACCOON				// 마마모후
	,BULLETKIND_RACCOONTOWER			// 모후타워

	,BULLETKIND_CHICKENBIG				// 왕달구
	,BULLETKIND_MOONBLADE				// 평강공주
	,BULLETKIND_EMPERORARCHER			// 용갱궁
	,BULLETKIND_NINJAMAN				// 일지매(남)
	,BULLETKIND_PIRATEARCHER			// 해적선해골궁병(화살)
	,BULLETKIND_PIRATEARCHER_MAGIC		// 해적선해골궁병(도끼)
	,BULLETKIND_PIRATESOLDIER			// 해적선해골병사
	,BULLETKIND_EMPERORCAVALRY			// 용갱마병
	,BULLETKIND_EMPERORMASTER			// 용갱장
	,BULLETKIND_GREATWALLSOLDIER		// 괴수손

	,BULLETKIND_GREATWALLMAGICIAN		// 머리만 - 머리던지기
	,BULLETKIND_CATRANGE				// 가챠 고양이 (원거리)

	,BULLETKIND_EMPERORWARRIOR			// 골렘병사
	,BULLETKIND_GREATWALLSPIDER			// 수라
	,BULLETKIND_GREATWALLTWINHEAD		// 두머리
	,BULLETKIND_BEARTANG				// 탱이 - 공던지기
	,BULLETKIND_BEARTANG_MAGIC			// 탱이 - 벌날리기
	,BULLETKIND_SUPERBEARTANG			// 슈퍼탱이 - 공던지기
	,BULLETKIND_SUPERBEARTANG_MAGIC		// 슈퍼탱이 - 벌날리기
	,BULLETKIND_HALLOWEENGHOST			// 할로윈
	,BULLETKIND_TRIPLEDEATH_MAGIC		// 활 마법(트리플데스)
	,BULLETKIND_COSMOSRABBIT			// 우주토끼 원거리공격
	,BULLETKIND_CHRISTMASRANGE			// 크리스마스 원거리변신
	,BULLETKIND_RUDOLPHPET				// 루돌프 소환펫 - 선물폭탄
	,BULLETKIND_GREATWALLASURA			// 버려진요새 아수라 - 불꽃 쏘기.
	,BULLETKIND_AMBERSAURUS				// 잃어버린세계 엠버사우루스
	,BULLETKIND_ROLLINGSTEGO			// 잃어버린세계 롤링스테고
	,BULLETKIND_FIGHTINGMENRANGE		// 원거리변신 스피릿 - 거너
	,BULLETKIND_FAIRYRANGE				// 요정스피릿(여) - 원거리
	,BULLETKIND_SNOWQUEEN				// 가챠 - 12월 스피릿 상자	눈의 여왕 사스콰치

	,BULLETKIND_FLAMEBUSTER				// [진성] 스킬북 
	,BULLETKIND_FLAMEWHIRL				// [진성] 스킬북 
	,BULLETKIND_WETFOG					// [진성] 스킬북 
	,BULLETKIND_SLING					// [진성] 스킬북 
	,BULLETKIND_WINDCUTER				// [진성] 스킬북 

	,BULLETKIND_CLOWN					// [춘기] 안개미궁 - 어릿광대
	,BULLETKIND_CLOWN_MAGIC				// [춘기] 안개미궁 - 어릿광대(마법)
	,BULLETKIND_BOUQUET					// [춘기] 안개신부 마법(부케던지기)

	//**************************************************************//
	//
	// [영훈] 블렛 추가는 이 위로 해주세요
	//
	//**************************************************************//
	,MAX_BULLET_KIND_NUMBER
};
/*
#define MAX_BULLET_KIND_NUMBER       			120

#define BULLETKIND_ARROW						1
#define BULLETKIND_GUN							2
#define BULLETKIND_CAKE							3
#define BULLETKIND_EGG							4
#define BULLETKIND_CANNONBALL					6
#define BULLETKIND_TURTLEBACKSHELL				7
#define BULLETKIND_CATFISH						8
#define BULLETKIND_LADYBUGFAN					9
#define BULLETKIND_SNOW							10
#define BULLETKIND_STAFF1						11
#define BULLETKIND_MADOGHOSTFIRE				12
#define BULLETKIND_JAPANHEADER					13
#define BULLETKIND_TOHANG						14
#define BULLETKIND_SKULLBATFIRE					15
#define BULLETKIND_FIREGOBLIN					16
#define BULLETKIND_CASTLECANNONBALL				17
#define BULLETKIND_SELFBOMB						18
#define BULLETKIND_KNIFE						19
#define BULLETKIND_GREENFIRE					20
#define BULLETKIND_BLACKINK						21
#define BULLETKIND_WINDBLADE					22
#define BULLETKIND_SHELLFISHBOMB				23

#define BULLETKIND_MAGICFIREBALL				24
#define BULLETKIND_MAGICBIGWIND					25
#define BULLETKIND_MAGICPOISON					26
#define BULLETKIND_MAGICICE						27
#define BULLETKIND_MAGICLIGHTINGFIST			28

#define BULLETKIND_BOOMERANG					29 // 임수로 부메랑을 하나 만들었따.

#define BULLETKIND_TREEMON						30 // 괴수목 - 가짜총알 사용(SUNKEN)
#define BULLETKIND_VINE							31 // 외눈넝쿨 - 가짜총알 사용(SUNKEN)

#define BULLETKIND_GHOSTFIRE					32 // 도령귀신
#define BULLETKIND_SIMTORYONG					33 // 심토룡
#define BULLETKIND_KINGCUTTLEFISH				34 // 대왕오적어
#define BULLETKIND_CRANE						35 // 탄광삽차
#define BULLETKIND_SELFBOMBWHALE				36 // 폭경 자폭
#define BULLETKIND_MONKEYDRILL					37 // 탄광원숭이

#define BULLETKIND_MAGICCURSEDEFENSE			38 // 방어력   마법 총알 
#define BULLETKIND_MAGICCURSEMOVESPEED			39 // 이동속도 마법 총알

#define BULLETKIND_BUTCHER						40 // 제주소잡이
#define BULLETKIND_STONEOLDMAN					41 // 돌하르방

#define BULLETKIND_MAGICTHROWKNIFE_RIGHT		42 // 표창닌자 마법 총알 오른쪽
#define BULLETKIND_MAGICTHROWKNIFE_LEFT			43 // 표창닌자 마법 총알 왼쪽

#define BULLETKIND_KINGCRAB						44 // 대왕소라게

#define BULLETKIND_FIREARROW					45 // 불화살

#define BULLETKIND_LOBSTER						46 // 잠수가재
#define BULLETKIND_DEADMESSENGER				47 // 저승사자
#define BULLETKIND_MASKMONK						48 // 중탈

#define BULLETKIND_PERSONCANNON					49 // 일인대포
#define BULLETKIND_CANNON_MAGIC					50 // 최무선 마법

#define BULLETKIND_BOW_SCROLL					51 // 궁술부적(1장) 발렷을 때
#define BULLETKIND_GUN_SCROLL					52 // 궁술부적(1장) 발렷을 때
#define BULLETKIND_STAFF_SCROLL					53 // 궁술부적(1장) 발렷을 때

#define BULLETKIND_FOAM_FIRE					54 // 먹깨비
#define BULLETKIND_FIREBALL						55 // 일반공격
#define BULLETKIND_MORTAL						56 // 근거리 공격할 때 주변부로 퍼지는 것처럼 보이게 함 - 회전절구차

#define BULLETKIND_WATERARROW					57 // 물화살

#define BULETKIND_MAGICPALMWIND					58 // 사명대사 장풍마법
#define BULLETKIND_STAFFWIND					59 // 사명대사 장풍

#define BULLETKIND_FLYVINE						60 // 날아가는 넝쿨(덩쿨?)

#define BULLETKIND_MAGICSKELLGHOST				61 // 날아가는 해골유령

#define BULLETKIND_SKELLFISH					62 // 날아가는 생선뼈

#define BULLETKIND_MAGICFROZEN					63 // 보스 얼음 공격
#define BULLETKIND_ICESTONE						64 // 얼음성 보스
#define BULLETKIND_TWINSPEAR					65 // 양날창

#define BULLETKIND_NORTHWIND					66 // 삭풍

#define BULLETKIND_NEW_BOW_SCROLL				67 // 궁술부적(2장) 발렷을 때
#define BULLETKIND_NEW_GUN_SCROLL				68 // 궁술부적(2장) 발렷을 때
#define BULLETKIND_NEW_STAFF_SCROLL				69 // 궁술부적(2장) 발렷을 때

#define BULLETKIND_FIRESTORM					70
#define BULLETKIND_ENERGYARROW					71 // 독안괴

#define BULLETKIND_SPEAR_MANA					72 // 관우 일반 공격
#define BULLETKIND_SPEAR_DRAGON					73 // 관우 마법 공격

#define BULLETKIND_BLACKBOSS					74 // 검은 전사 두목
#define BULLETKIND_OCTOPUS						75 // 문어

#define BULLETKIND_OTOHIME						76

#define BULLETKIND_BLACKSPIDER					77	// 흑거미
#define BULLETKIND_EVILEYE						78	// 이블아이

#define BULLETKIND_GARGOYLE						79	// 가고일 마법
#define BULLETKIND_CERBERUS						80	// 켈베로스 마법

#define BULLETKIND_FROZENSORCERER				81	// 프로즌 소서러 일반
#define BULLETKIND_MARIONETTE					82
#define BULLETKIND_FROKEN						83	// 기계공 프로켄
#define BULLETKIND_ICEHOUND						84	// 아이스하운드
#define BULLETKIND_FROZENKNIGHT					85	// 프로즌 나이트
#define BULLETKIND_FROZENSORCERER_MAGIC			86	// 프로즌 소서러 마법

#define BULLETKIND_NINJAPANDA					87	// 닌자판다 마법
#define BULLETKIND_RACCOONDOG					88	// 둔갑너구리 마법

#define BULLETKIND_BOSSMONKEY					89	// 보스 원숭이 마법
#define BULLETKIND_CANNONMOL					90	// 캐논몰(토행요괴)

#define BULLETKIND_BLOODBAT						91	// 흡혈박쥐
#define BULLETKIND_WEREWOLF						92	// 늑대인간
#define BULLETKIND_DEVILOUS						93	// 데빌러스
#define	BULLETKIND_GANGSI						94	// 강시
#define BULLETKIND_PRIEST						95	// 타락성직자

#define BULLETKIND_HARPY						96	// 타락성직자
#define BULLETKIND_GIRAFFE						97	// 기린~

#define BULLETKIND_PAPAGOBLIN						98	// 기린~

#define BULLETKIND_MINEGIRL						99 // 탄광소녀

#define BULLETKIND_GREENTURTLE						100 // 초원거북
#define BULLETKIND_MAGICALRACCOON				101	// 메지컬모후
#define	BULLETKIND_MAMARACCOON					102 // 마마모후
#define	BULLETKIND_RACCOONTOWER					103	// 모후타워
#define BULLETKIND_CHICKENBIG					104 // 왕달구
#define BULLETKIND_MOONBLADE					105 // 평강공주
#define BULLETKIND_EMPERORARCHER				106 // 용갱궁
#define BULLETKIND_NINJAMAN						107 // 일지매(남)
#define BULLETKIND_PIRATEARCHER					108 // 해적선해골궁병
#define BULLETKIND_PIRATESOLDIER				109 // 해적선해골병사


//#define MAX_BULLET_KIND_NUMBER       			80
*/
//-----------------------------------------------------
// 총알 속성 
//-----------------------------------------------------
#define BLTATB_RANGE1				0x1 
#define BLTATB_RANGE2				0x2 
#define BLTATB_RANGE3				0x4 
#define BLTATB_FRAGMENT				0x8
#define BLTATB_CURVE				0x10			// 곡선 탄도 총알. 
#define BLTATB_BOOMERANG			0x20			// 부메랑 


#define MAX_HIT_ID								25		// 5*5 사이즈에 준하여 , 
// 공격에 의하여 피해를 입는 캐릭터의 스택 사이즈 

#define MAX_BULLET_FILE_NAME_LENGTH				64

#define MAX_DIRECTION	5

class cltBulletadjust
{
public :

	cltPos clAdjustPos[MAX_DIRECTION];	// 0 : 남, 1 : 남서, 2 : 서, 3 : 서북, 4 : 북

	cltBulletadjust()
	{
		Init();
	}

	cltBulletadjust(SI32 x1, SI32 y1, SI32 x2, SI32 y2, SI32 x3, SI32 y3, 
		SI32 x4, SI32 y4, SI32 x5, SI32 y5 )
	{
		Init();

		clAdjustPos[0].siX = x1;
		clAdjustPos[0].siY = y1;

		clAdjustPos[1].siX = x2;
		clAdjustPos[1].siY = y2;

		clAdjustPos[2].siX = x3;
		clAdjustPos[2].siY = y3;

		clAdjustPos[3].siX = x4;
		clAdjustPos[3].siY = y4;

		clAdjustPos[4].siX = x5;
		clAdjustPos[4].siY = y5;

	}

	void Init()
	{
		for (SI32 i = 0; i < MAX_DIRECTION; i++)
		{
			clAdjustPos[i].Init();
		}
	}
};


class cltBulletSoundInfo
{
private:
	SI32	m_si32AttackSoundIndex;
	SI32	m_si32HitSoundIndex;

public:

	cltBulletSoundInfo()
	{
		Init();
	}

	cltBulletSoundInfo( SI32 AttackSoundIndex, SI32 HitSoundIndex )
	{
		Set( AttackSoundIndex, HitSoundIndex );
	}

	void Init()
	{
		m_si32AttackSoundIndex = 0;
		m_si32HitSoundIndex = 0;
	}

	void Set( SI32 AttackSoundIndex, SI32 HitSoundIndex )
	{
		m_si32AttackSoundIndex = AttackSoundIndex;
		m_si32HitSoundIndex = HitSoundIndex;
	}

	SI32 GetAttackSoundIndex()
	{
		return m_si32AttackSoundIndex;
	}

	SI32 GetHitSoundIndex()
	{
		return m_si32HitSoundIndex;
	}
};

class cltBulletDrawInfo
{
public:
	TCHAR	m_strFileName[ _MAX_PATH ];				// 파일명
	TCHAR	m_strLightFileName[ _MAX_PATH ];		// 총알 LIGHT 파일명
	SI16	m_StartFont;							// 시작 폰트
	SI32	m_EachDirectionFrameNum;				// 각 방향별 이미지 개수

	SI32	m_siTailSmoke;							// 꼬리 연기 종류 ( 0 은 꼬리 연기 없음. )
	bool	m_bDirectionSwitch;						// 방향성이 있느냐

	//bool	bChaseSwitch;							// 총알이 추적해 가느냐?
	//bool	bCurveSwitch;							// 총알이 커브로 날아가는 것

	SI32	m_TransParentMode;						// 투명 효과
	SI32	m_siLightMode;

	SI32	m_siIndent;								// Y좌표 추가
	SI16	m_siLoopStartFont;						// 루필할 스타트 폰트

	cltBulletadjust	clBulletadjust;					// 총알 나가는 위치 설정

	cltBulletDrawInfo()
	{
		Init();
	}

	void Init()
	{
		memset( m_strFileName,		0, _MAX_PATH );
		memset( m_strLightFileName, 0, _MAX_PATH );
		m_StartFont = 0;
		m_EachDirectionFrameNum = 0;
		m_siTailSmoke = 0;
		m_bDirectionSwitch = false;
		m_TransParentMode = 0;
		m_siLightMode = 0;
		m_siIndent = 0;
		m_siLoopStartFont = 0;
	}

	cltBulletDrawInfo( TCHAR *strFileName, TCHAR *strLightFileName, cltBulletadjust bulletadjust, SI16 StartFont, SI32 EachDirectionFrameNum, SI32 siTailSmoke, bool bDirectionSwitch, SI32 TransParentMode, SI32 siLightMode , SI32 siIndent )
	{
		Set( strFileName, strLightFileName, bulletadjust, StartFont, EachDirectionFrameNum, false, false, 0 , siLightMode, siIndent );
	}

	void Set( TCHAR *strFileName, TCHAR *strLightFileName, cltBulletadjust bulletadjust, SI16 StartFont, 
		SI32 EachDirectionFrameNum, SI32 siTailSmoke = 0, bool bDirectionSwitch = false, 
		SI32 TransParentMode = 0, SI32 siLightMode=0 , SI32 siIndent = 0 , SI16 LoopStartFont = 0)
	{
		memset( m_strFileName,		0, _MAX_PATH );
		memset( m_strLightFileName, 0, _MAX_PATH );

		if(strFileName)			StringCchCopy( m_strFileName, _MAX_PATH, strFileName );
		if(strLightFileName)	StringCchCopy( m_strLightFileName, _MAX_PATH, strLightFileName );

		m_StartFont			= StartFont;
		m_EachDirectionFrameNum = EachDirectionFrameNum;

		clBulletadjust		= bulletadjust;

		m_siTailSmoke		= siTailSmoke;
		m_bDirectionSwitch	= bDirectionSwitch;
		m_TransParentMode	= TransParentMode;
		m_siLightMode		= siLightMode;
		m_siIndent			= siIndent;

		m_siLoopStartFont	= LoopStartFont;
	}
};

class cltBulletLayerInfo{
public:
	SI32 siFont;
	bool bTransMode;

	cltBulletLayerInfo()
	{
		Init();
	}

	cltBulletLayerInfo(SI32 font, bool transmode)
	{
		Set(font, transmode);
	}

	void Init()
	{
		siFont		= -1;
		bTransMode	= false;
	}

	void Set(cltBulletLayerInfo* pclinfo)
	{
		Set(pclinfo->siFont, pclinfo->bTransMode);
	}

	void Set(SI32 font, bool transmode)
	{
		siFont		= font;
		bTransMode	= transmode;
	}

};

//------------------------------------
// cltBulletKindInfo
//------------------------------------
class cltBulletKindInfo
{
public:
	void Init(SHORT stepwidth, SHORT chaseswitch, SHORT curveswitch, SI32 attacktype, SI32 BulletAtb, SI32 BulletSmokeAtb, SI32 sidelay,
		SI32 firesmoke, SI32 hitsmoke, 
		cltBulletSoundInfo* pclSoundInfo,
		cltBulletLayerInfo* pcllayerinfo,
		cltBulletDrawInfo* pclDrawInfo,
		SI32 hitegroundsmoke = 0
		);

	void LoadSpr();

	SHORT StepWidth;
	TCHAR szFileName[MAX_BULLET_FILE_NAME_LENGTH];
	TCHAR szLightFileName[MAX_BULLET_FILE_NAME_LENGTH];
	SI32 Font;
	SI32  siMoveFrameNumber;


	cltBulletLayerInfo	clLayerInfo;

	// 방향마다 다른 그림들이 존재하는가(TRUE), 한가지 그림만 존재하는가 ?(FALSE)
	BOOL bDirSwitch;                          

	BOOL TransparentSwitch;
	BOOL ChaseSwitch, CurveSwitch;			

	// 2003.10.29
	SI32 TailSmoke;


	// 총알의 LIGHT
	SI32 m_siLightMode;

	// 총알 높이 조절
	SI32 m_siIndent;
	cltBulletadjust clBulletadjust;
	SI16 m_siLoopStartFont;

	//----------------------
	// 공격 타입. 
	//----------------------
	SI32 siAttackType;		


	SI32 siAtb;
	SI32 siSmokeAtb;

	SI32 siDelay;

	TSpr Spr;
	TSpr LightSpr;

	//-----------------------
	// 연기 
	//-----------------------
	SI32 siFireSmoke;		// 발사시 연기
	SI32 siHitSmoke;		// 맞을때 연기
	SI32 siHiteGroundSmoke; // 맞을때 연기 뒤에 땅에 깔리는 것

	//-----------------------
	// 효과음 
	//-----------------------
	SI32 siFireEffect;		// 공격시 사운드
	SI32 siHitEffect;		// 피격시 사운드

	cltBulletKindInfo();
	~cltBulletKindInfo();

};

//--------------------------------------
// cltBulletCommon
//--------------------------------------
class cltBulletCommon{
public:
	cltMapCommon*				pclMap;
	const cltBulletKindInfo*	pclBKI;

	//------------------
	// 자신의 신분 정보
	//------------------
	SI32 Kind;                        // 어떤 종류의 obj인가?
	SI32 siMagicKind;				  // 마법에의한 총알이라면 마법을 설정한다.

	SI32 Type;

	//-----------------
	// 현재 작업 상황 
	//-----------------
	SI32  CurrentOrder;               // 현재 캐릭터가 하고 있는 작업 

	//------------------
	// 좌표 관련 
	//------------------
	cltPos clRealSrcPos;
	cltPos clRealDestPos;
	SI32 CurveBulletRealX, CurveBulletRealY;
	SI32 MapSX, MapSY;
	SI32  MapDX, MapDY;
	SI32  X, Y;                        // obj의 중심 위치 
	SI32 CenterX, CenterY;

	//------------------
	// 공격자 정보 
	//------------------
	SI32 ShooterID;
	SI32 Apower;

	//-------------------
	// 공격 목표 관련 
	//--------------------
	cltAttackTarget clAttackTarget;		// 총알이 공격해야 할 대상. 
	int TargetDirection;               // 목표의 방위 
	int OldTargetDirection;			   // 목표의 방위 예전

	//--------------------
	// 움지임 정보 
	//--------------------
	int TempDirection;                // 16방위로 존재하는가?
	SI32 CurveRate;					 // 얼마나 곡선을 이루는가?
	BOOL CurveSwitch;

	SI32 MoveStep;
	SI32 MaxMoveStep;

	SI32 StepWidth;                    // 이동 단위

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_SHOOT_RANGE>		CourseX;
	NSafeTArray<SI32, MAX_SHOOT_RANGE>		CourseY;
#else
	SI32 CourseX[MAX_SHOOT_RANGE];
	SI32 CourseY[MAX_SHOOT_RANGE];
#endif

	//-----------------------
	// 추적 기능 
	//-----------------------
	BOOL ChaseSwitch;    // 추적할 것인가 말것인가?
	BOOL ChaseRepeatNumber;

public:
	virtual void Init(){};
	virtual void SetAnimation(SI32 ani){}
	virtual void AniStepAction(){}
	virtual	BOOL Draw(){return FALSE;}
	virtual void MakeTail(SI32 tempdirection){};
	virtual void MakeHitSmoke(){};
	virtual void MakeHitSound(){};

	cltBulletCommon();

	bool Init(cltMapCommon* pclmap, SHORT kind, SHORT magickind, SHORT id, 
		SI32 shooter_id, SHORT apower, 
		const cltPos* pcltilestartpos, 
		const cltPos* pcldotstartpos, 
		const cltAttackTarget* pcltarget, 
		const cltPos* pcldotdestpos, 
		cltBulletKindInfo* pclbki);

	void DecideDirection(SHORT x1, SHORT y1, SHORT x2, SHORT y2);

	void GetCourse( double SpeedIncTime = 0.3 );

	BOOL Action();

	void CalculateRealXY();

	BOOL Hit();
	BOOL HitNormal();
	BOOL HitFragment(); 

};

//---------------------------------------
// cltBulletServer
//---------------------------------------
class cltBulletServer : public cltBulletCommon{

public:


	SHORT BallSize;


	SI32 HitIDIndex;
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_HIT_ID>		HitID;
#else
	SI32 HitID[MAX_HIT_ID];
#endif

public:
	cltBulletServer();

	void Init();

	// 한번 공격한 캐릭터를 다시 공격하지 않기 위해서.
	// 스택을 초기화한다. 
	void InitPushID();
	void PushHitID(SI32 id);
	BOOL IsAlreadyPushID(SI32 id);

};

//---------------------------------------
// cltBulletClient
//---------------------------------------
class cltBulletClient : public cltBulletCommon{
public:

	//---------------------------------
	// 출력 옵션. 
	//---------------------------------
	bool bDrawSwitch;				   // 출력 여부. 
	bool bReverseSwitch;                // 미러링 여부. 

	SI32 siFont;

	TSpr* pSpr;
	TSpr* pLightSpr;

	SI32 siAnimation;									// 어떤 종류의 애니메이션을 수행하는가?
	SI32 siAniStep;

	// 이동관련 
	SI08		MoveFrameNumber;
#ifdef _SAFE_MEMORY
	NSafeTArray<TSpr*, 9>		pMoveFileSpr;

	NSafeTArray<SI32, 9>		MoveFont;
#else
	TSpr*	pMoveFileSpr[9];

	SI32		MoveFont[9];
#endif

	BOOL TransparentSwitch;
	SI32  DrawStartX, DrawStartY;      // 그림이 그려지는 시작 위치 
	SI32  LightDrawStartX, LightDrawStartY;

	SI32 CurveBulletDrawX, CurveBulletDrawY;

	// 2003.10.29
	SI32 TailSmoke;                    // 꼬리 연기

	UI16 uiAdjustFrame;				   // 위치보정을 위한 값
	SI32 AdjustPosX;
	SI32 AdjustPosY;

	cltBulletClient();
	void Init();
	void SetFileNFont();
	// 찍을 위치를 계산한다.
	void CalculateDrawStartXY();
	BOOL Draw();

	// 찍어야 할 그림을 선정한다. 
	void DecideFont();
	void DecideFontMove();
	void DecideDir();

	void SetAnimation(SI32 ani){siAnimation = ani;}
	SI32 GetAnimation()const{return siAnimation;}

	void AniStepAction();

	void SetMoveImage16(TSpr* pspr, SHORT font, SHORT frame);

	void SetMoveImage(SHORT ani, TSpr* pspr, SHORT font);

	//꼬리 연기를 만든다. 
	void MakeTail(SI32 tempdirection);

	// 피격 연기를 만든다. 
	void MakeHitSmoke();

	// 피격 사운드를 출력한다.
	void MakeHitSound();

};

//---------------------------------------------
// cltBulletManager
//--------------------------------------------
class cltBulletManager{
private:
#ifdef _SAFE_MEMORY
	NSafeTArray<cltBulletKindInfo, MAX_BULLET_KIND_NUMBER>	BKI;
#else
	cltBulletKindInfo BKI[MAX_BULLET_KIND_NUMBER];
#endif

#ifdef _SAFE_MEMORY
	NSafeTArray<cltBulletCommon*, MAX_BULLET_NUMBER>		pclBullet;
	NSafeTArray<SI32, MAX_BULLET_NUMBER>				BulletInfo;
#else
	cltBulletCommon*	pclBullet[MAX_BULLET_NUMBER]; // 총알용 테이터 
	SI32				BulletInfo[MAX_BULLET_NUMBER]; // 총알에 대한 정보를 저장할 공간 
#endif

private:
	SI32 FindEmptyBullet();
	void DeleteBullet(SI32 id);
	void InitBulletKindInfo();

public:

	cltBulletManager(SI32 gamemode);
	~cltBulletManager();

	void DeleteBullet();

	SI32 SetBullet(cltMapCommon* pclmap, SHORT kind, SHORT magickind,
		SI32 shooter_id, 
		SHORT apower, 
		const cltPos* pcltilestartpos, 
		const cltPos* pcldotstartpos, 
		const cltAttackTarget* pcltarget, 
		const cltPos* pcldotdestpos);

	BOOL Action();
	BOOL Draw();

};




#endif

