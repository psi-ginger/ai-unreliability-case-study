//---------------------------------
// 2003/10/16 김태곤
//---------------------------------
#ifndef _CHARClIENT_H
#define _CHARClIENT_H

#include "Char\CharCommon\Char-Common.h"

#define MAX_RANKMARK_WIDTH		600
#define MAX_RANKMARK_HEIGHT		500
#define MAX_NPCKINDFORRANKMARK  200

#define JEWELEFFECT_RARE1	0x0000001	// 명품이펙트
#define JEWELEFFECT_RARE2	0x0000002	// 명품이펙트
#define JEWELEFFECT_WATER1	0x0000004	// 인챈트 이펙트 물속성 1단계
#define JEWELEFFECT_WATER2	0x0000008	// 인챈트 이펙트 물속성 2단계
#define JEWELEFFECT_WATER3	0x0000010	// 인챈트 이펙트 물속성 3단계
#define JEWELEFFECT_GROUND1	0x0000020	// 인챈트 이펙트 땅속성 1단계
#define JEWELEFFECT_GROUND2	0x0000040	// 인챈트 이펙트 땅속성 2단계
#define JEWELEFFECT_GROUND3	0x0000080	// 인챈트 이펙트 땅속성 3단계
#define JEWELEFFECT_WIND1	0x0000100	// 인챈트 이펙트 바람속성 1단계
#define JEWELEFFECT_WIND2	0x0000200	// 인챈트 이펙트 바람속성 2단계
#define JEWELEFFECT_WIND3	0x0000400	// 인챈트 이펙트 바람속성 3단계
#define JEWELEFFECT_FIRE1	0x0000800	// 인챈트 이펙트 불속성 1단계
#define JEWELEFFECT_FIRE2	0x0001000	// 인챈트 이펙트 불속성 2단계
#define JEWELEFFECT_FIRE3	0x0002000	// 인챈트 이펙트 불속성 3단계

#define MARRIED_FULLDRESS	0x0004000	// 결혼예복 이펙트

#define	JEWELEFFECT_GRADE1	2
#define	JEWELEFFECT_GRADE2	7
#define	JEWELEFFECT_GRADE3	10

class CCCDSurfaceMgr;
class CDrawCharName;

class CCharJim;
class CCharPartyEmoticon;

class cltSystemNPC;
//------------------------------------------
// cltCharClient : Client용 
//------------------------------------------
class cltCharClient	: public cltCharCommon
{
public:

	TSpr*			pImgSpr;
	SI32 			Font;                       													// 출력되는 그림이 저장된 폰트 번호 
	SI32			siEffectFont;				// Animation에 있어서의 특수 효과

	SI32			siShadowAni;				// 애니랑 같이 찍어줄 동작
	SI32			siShadowFont;				// 동작의 폰트

	BOOL 			DrawSwitch;																		// 그려줄 것인가 말것인가?
	bool			bDrawReadySwitch;

	SI32 			RealImageXPercent, RealImageYPercent;  											// 실제 이미지가 가로 세로로 각 몇 %를 점하는가?
	DWORD			dwDrawFrame;         																// 마지막으로 그려준 프레임 

	DWORD			dwLastWaitFrame;

	SI16			TransMode;									// 캐릭터를 어떤 모드로 찍어줄지 설정한다.
	SI08			AlphaValue;									// 알파값
	SI32			ImageXsize, ImageYsize;       				// 액션시에 사용되는 그림의 가로 세로 사이즈 

	SI32 			VibrateY;                    													// 상하흔들림의 정도 
	BOOL 			VibrateSwitch;               													// 상하 흔들림이 있는가?

	// 캐릭터 선택관련 
	bool			bMouseOverSwitch;
	bool 			bNewSelectedSwitch;
	SI32 			NewSelectedDelay;
	SI32			MouseOverDelay;
	SI32			DrawStartX, DrawStartY;      				// 그림이 그려지는 시작 위치 

	//------------------------------
	// 출력을 위한 장비 코드 
	//------------------------------
	TCHAR		szHelmetCode[32];
	TCHAR		szArmourCode[32];
	TCHAR		szWeaponCode[32];
	TCHAR		szMantleCode[32];
	TCHAR		szHorseCode[32];	

	//-----------------------------
	// 미러 출력 관련 
	//-----------------------------
	bool			bReverseSwitch;                				// 뒤집어서 찍을 것인가?

	//-----------------------------
	// 어두운 정도. 8
	//-----------------------------
	SI32 			DarkStep;

	//-----------------------------
	// 이미지 사이즈 관련 
	//-----------------------------
	SI32	RealImageStartX, RealImageEndX;        											// 실제 이미지가 점하는 가로 세로 좌표 
	SI32	RealImageStartY, RealImageEndY; 
	RECT	RealRect;

protected:
	//------------------------------
	// 사망관련 
	//------------------------------
	bool			bDyingOperationSwitch;
	bool			bGhostShowSwitch;

public:
	//-----------------------
	// 이동 관련 
	//-----------------------
	SI32			siRX, siRY;  			// 도트 단위로 이동하는 캐릭터의 변위 
	SI32			MOX, MOY;  				// 이동의 기준이 되는 이전 좌표 
	SI32			MDX, MDY;  				// 이동의 기준이 되는 다음 좌표 


	//------------------------
	// 대행수, 행수 동작 처리. ( 바닥에 원 그리는 것 )
	//-------------------------
	bool			bBoss;
	CBossEffect		*pclBoss;

	//------------------------
	// Regeneration 관련
	//-------------------------
	bool	bRegeneration;					// 리젠 되었음을 알려줘야 하는가?
	CRegeneration	*pclRegeneration;	// 리젠을 나타내는 클래스

	//-----------------------------------------
	//자기 마을에 트리가 건설되어 있는지 
	//-----------------------------------------
#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_HIRED_EVENTNPC_NUM>		siEventNPCKind;
#else
	SI32	siEventNPCKind[MAX_HIRED_EVENTNPC_NUM];//기존의 이벤트 엔피씨를 확장 해서 이것도 배열로 확장해본다.
#endif

	//------------------------
	// 개인상점 모드 
	//------------------------
	SI16			siShopMode;
	TCHAR			szPersonalShopText[MAX_PERSONAL_SHOP_TEXT_SIZE];
	TCHAR			szPersonalShopTitle[MAX_SHOP_TITLE_LENGTH];

	//------------------------
	// 피격 이미지 관련 
	//-------------------------
	 bool bHitDrawSwitch;					// 피격되었음을 알려줘야 하는가?
	 CCharCut*	pclCharCut;					// 베어짐을 나타내는 클래스.


	//------------------------
	// 레벨업 이미지 관련 
	//-------------------------
//	 bool bLevelUpDrawSwitch;				// 레벨업이 되었음을 알려줘야 하는가?
	 CLevelUpEffect	*pclLevelUp;			// 레벨업을 나타내는 클래스.

	 //------------------------
	 // 하트 이펙트 관련 
	 //-------------------------
	 CHeartEffect *pclHeartEffect;

	 //------------------------
	 // 네잎 클로버 이펙트 관련 
	 //-------------------------
	 CFourLeafEffect *pclFourLeafEffect;

	 CCushionEffect	 *pclCushionEffect;
	 CCushionCatEffect	 *pclCushionCatEffect;

	 //------------------------
	 // 쿠션 출력 관련
	 //------------------------
	 SI32 siCushionUnique;

	 //------------------------
	 // MVP 이미지 관련 
	 //-------------------------
	 bool bMVPEffectDrawSwitch;				// 레벨업이 되었음을 알려줘야 하는가?
	 CMVPEffect	*pclMVPEffect;			// 레벨업을 나타내는 클래스.

	 //------------------------
	 // Enchant 이펙트 관련
	 //-------------------------
	 bool bEnchantEffDrawSwitch;
	 CEnchantEff	*pclEnchantEff;	

	 //------------------------
	 // Quest 이펙트 관련
	 //-------------------------
	 //bool bQuestEffDrawSwitch;
	 CQuestEff	*pclQuestEff;	

	 //------------------------
	 // 스킬 레벨업 이미지 관련
	 //------------------------
	 bool bSkillLevelUpDrawSwitch;			// 스킬 레벨업이 되었는지 안 되었는지
	 CSkillLevelUpEffect *pclSkillLevelUp;	// 스킬 레벨업 클래스


	 //------------------------
	 // 이모티콘 이미지 관련
	 //------------------------
	 CEmoticonEffect* pclEmoticonEff;

	 //------------------------
	 // 발렌타인 이모티콘 이미지 관련
	 //------------------------
	 CEmoticonEffect* m_pclValentineEmoticonEff;

	 //------------------------
	 // 경험치 표시 
	 //------------------------
	 CExpCounter*	pclExpCounter;			// 경험치 표시. 

	 //------------------------
	 // 치료  표시 
	 //------------------------
	 CHealCounter*	pclHealCounter;			// 치료  표시. 

	 //------------------------
	 // 필살기 표시
	 //------------------------
	 CCharCriticalEffect	*pclCriticalEffect;

	 //------------------------
	 // 스틸 방지를 위한 찜 표시
	 //------------------------
	 CCharJim				*pclJim;

	 //------------------------
	 //	파티 동료 표시를 위한 것.
	 //------------------------
	 CCharPartyEmoticon		*pclPartyEmoticon;

	 //-----------------------
	 // 말풍선 관련 
	 //-----------------------
	 CDrawChat*	pclDrawChat;
	 
	 //-----------------------
	 // 캐릭터 이름 출력 관련
	 //-----------------------
	 CDrawCharName *pclDrawCharName;

	 //---------------------------------
	 // 조합형 캐릭터용 서피스 인덱스. 
	 //--------------------------------
	 SI32 siHeroSurfaceIndex;

	 //-----------------------------
	 // 추종자 캐릭터 정보.
	 //-----------------------------
	 SI32 siHostID;
	 SI32 siSlaveID;

	 //--------------------------
	 // 캐릭터 이름 정보 표시 
	 //--------------------------
	 DWORD dwLastMakeCharNameFrame;
	 TCHAR szCharNameInfo[128];

	 //--------------------------
	 // 대기동작이 마지막으로 바뀐시간
	 //--------------------------
	 clock_t lastWaitAniChanged;

	 //--------------------------
	 // 치료 효과. 
	 //--------------------------
	 DWORD dwHealClock;

	 //------------------------
	 // 물품 생산 효과. 
	 //------------------------
	 bool bStartMakeItemSwitch;
	 DWORD dwStartMakeItemClock;
	 DWORD dwDelayMakeItemClock;

	 // 개인 상점 체크하기 위해서
	cltShopTradeData	clMyShopData;
	BOOL				bMyShopStart;

	//--------------------------
	// 수호정령 특수모션 이펙트
	//--------------------------
	DWORD	dwSoulGuardSF_Clock;
	SI16	siSoulGuardSF_Kind;

	//--------------------------
	 // 이름 과 그림파일 인덱스
	 //--------------------------

	 
	 //TCHAR m_NPCName[MAX_NPCKINDFORRANKMARK][64];
	 //SI32 m_NPCKindIndex[MAX_NPCKINDFORRANKMARK];
	 //SI32 m_NPCImgIndex[MAX_NPCKINDFORRANKMARK];

	RGB16	*m_RankMarkImgEncBuffer;
	SI32     m_RankMarkEncHres;
	SI32     m_RankMarkEncVres;

	SI32	m_siSetItemImgAnimationCounter1;
	SI32	m_siSetItemImgAnimationCounter2;

	SI32	m_siWeddingImgAnimationCounter;
	
	cltCharClient();
	virtual ~cltCharClient();

	void InitCharClient();

	SI32 GetRX() const;
	SI32 GetRY() const;

	//---------------------------------------------------------------
	// 조합형 캐릭터. 
	//---------------------------------------------------------------
	// 파라메타로 주어진 아이템에 따라 Person의 복장을 바꿔주게 한다.
	bool ChangeArmCode(SI32 itempos, SI32 siunique);
	// 말 코드를 변경하여 조치한다.
	void ChangeHorseCode( SI32 code,cltDate* cldate = NULL , SI32 ticket = 0);

	//---------------------------------------
	// 캐릭터를 화면에 그려준다. 
	//---------------------------------------
	//Game Direction을 CDD Direction으로 변경한다.
	SI32 TransDir(SI32 gamedir);

	bool Draw();
	bool DrawHero();
	// DrawHero 내에서 장비별 명품, 인첸트 이펙트를 체크해서 반환하는 함수[영진]
	SI32 CheckJewelEffect( UI32 rare , SI32 enchanttype , SI32 enchantpower, bool Flag = false );	
	bool DrawNormal();
	DWORD GetDrawFrame(); 

	BOOL OperateWaitClient();

	// 사망 행위. 
	bool Action_Dying();

	void SetFileNFont();

	// 찍을 좌표를 계산한다. 
    void CalculateRealXY();

	void NewSelectedAction();

	// 캐릭터의 이름을 만든다.
	bool MakeCharName(cltCharClient* pclchar);
	void DrawName();

	// 말 풍선을 그린다. 
	void DrawChatBalloon();
	// 신분 표시를 한다. 
	void DrawRankMark(SI32 font);
	// 최고 장인 표시를 한다. 
	void DrawMakeMasterMark(SI32 skillunique);
	// GM마크를 표시한다
	void DrawGMMark();

	// [지연] 복불복 이벤트 : 키몹, 원킬몹, 선물몹 머리위에 숫자 출력해서 테스트 쉽게
	void DrawBBBSpecialIndicator(SI32 siType);

	// 복불복 키몬스터에 마크 표시
	void DrawKeyMonsterMark();
	
	// [추가 : 황진성 2008. 1. 15 => 최강길드 마크 그려주기.]
	void DrawBestGuildMark(void);

	// PC방 마크를 그린다
	void DrawPCRoomMark( void );
	// 프리미엄 파츠를 그린다
	void DrawPremiumPartsEffect(SI32 siHorseUnique ,SI32 siPremiumUnique);
	// 프리미엄 파츠 풀튜닝된 이팩트를 그린다.
	void DrawPremiumPartsFULLEffect(SI32 siHorseUnique);

	// 초보자 도움 화살표시.(내시, 내금위, 장금이, 역관위에)
	void DrawArrow();
	// 받아야할 퀘스트 알림 표시.(내시, 내금위, 장금이, 역관위에)
	void DrawQuestion();
	// 진행중 퀘스트 알림 표시.(내시, 내금위, 장금이, 역관위에)
	void DrawRedExclamation();
	// 완료된 퀘스트 알림 표시.(내시, 내금위, 장금이, 역관위에)
	void DrawBlueExclamation();

	// 새롭게 변경된 퀘스트 마크 그리기 함수
	void DrawQuestionMark( SI16 siMode );

	//npc 의 이름을 text 파일에서 얻어온다
	void NPCMgr();

	BOOL DrawShadow();

	// 지휘효과 출력 함수.
	BOOL DrawGeneralEft();

	// 찍을 좌표를 계산한다. 
    void CalculateDrawStartXY();
	void CalculateDrawStartXYInMapCaptureMode();

	void GetBottomPoint(cltPos* pclpos);

	void SetNewSelectedSwitch(bool mode);
	void SetMouseOverSwitch(bool mode);

    void DecideFont();

	void SetDrawReadySwitch(bool mode);

	//------------------------
	// 피격 이미지 관련 
	//-------------------------
	void SetHitSwitch(bool mode);
	void SetHitDrawSwitch(bool mode);

	//---------------------------------------
	// 점과 오브젝트의 일치 여부 
	//---------------------------------------
	// 어떤 한 점이 obj 위에 있는지 여부를 알려주는 함수 (절대 좌표)
    BOOL			IsAbsPtInObj(SHORT x, SHORT y, SHORT& length);

	///----------------------------------
	// Session 생성시 호출 함수. 
	//-----------------------------------
	void Init();
	void Close();

	//-------------------------------------------
	//이동 관련 함수.(CharClient-Move.cpp)
	//-------------------------------------------
	void CalculateRXY();
	void SetMoveStandardPos(SI32 mox, SI32 moy, SI32 mdx, SI32 mdy);
	//지금 좌표와 다음 좌표간의 백분율 비 변위를 구한다. 
    void GetMoveRatio(SI32 startx, SI32 starty, SI32 endx, SI32 endy, SI32 movepercent, SI32& rx, SI32& ry);

	//--------------------------------
	// 대행수, 행수 동작 처리. ( 바닥에 원 그리는 것 )
	//--------------------------------
	void CharBossAction();

	//--------------------------------
	// Regeneration 동작 처리. 
	//--------------------------------
	void CharRegenerationAction();
	void SetCharRegeneration();

	//--------------------------------
	// 베기 동작 처리. 
	//--------------------------------
	void CharCutAction();
	void SetCharCut(SI32 mode);

	//--------------------------------
	// 레벨업 동작 처리. 
	//--------------------------------
	void SetLevelUp();

	//--------------------------------
	// MVP 이펙트 동작 처리
	//--------------------------------
	void SetMVPEffect();

	//--------------------------------
	// Enchant 이펙트 동작 처리
	//--------------------------------
	void SetEnchantEffect(SI08 AniType);

	//--------------------------------
	// Quest 이펙트 동작 처리
	//--------------------------------
	void SetQuestEffectReceive();
	void SetQuestEffectClear();

	//--------------------------------
	// 스킬 레벨업 동작 처리. 
	//--------------------------------
	void SetSkillLevelUp();

	//--------------------------------
	// 이모티콘 동작 처리.
	//--------------------------------
	void SetEmoticon( SI16 index );

	//--------------------------------
	// 발렌타인 이모티콘 동작 처리.
	//--------------------------------
	void SetValentineEmoticon( SI16 index );

	//----------------------------
	// 클라이언트 캐릭터의 동작. 
	//----------------------------
	void CharClientAction();

	//-----------------------------------
	// 말 관련 
	//-----------------------------------
	bool RideHorse();		// 말을 탄다. 
	bool UnrideHorse();		// 말에서 내린다.
	
	void DeleteOperation();	// 삭제에 필요한 조치를 취한다. 
	void DeleteSlave();		// 추종자를 삭제한다. 

	bool AfterCharAction();
	void SetJimShow();		// 찜정보에 따라 화면에 표시하거나 표시하지 않는다. 
	
	//-----------------------------------
	// 물품 생산. 
	//-----------------------------------
	bool SetMakeDelayDraw(DWORD delayclock);
	void DrawMakeDelay(LPDIRECTDRAWSURFACE7 lpSurface, SI32 rx, SI32 ry);		// 물품 생산 지연을 표시한다. 
	
	
	void DrawHealthBar();		// Health Bar를 그려준다.
	void DrawKingEffect();		// King Effect를 그려준다.
	void DrawGodEffect();		// 성황신 Effect를 그려준다.
	void DrawSummonEffect();	// 소환수 Effect를 그려준다.
	void DrawLoveEffect();		// 바닥에 하트 Effect를 그려준다.
	void DrawGachaSummonEffect();	// 소환펫 Effect를 그려준다.

	//KHY - 1220 - 변신시스템. - //해당 변신 캐릭터 싱크가 100%이면.
	void DrawTransformSyc100Effect();

	void FogAction();			// 포그 처리를 한다. 
	
	void DoQuestCheckAndAction(SI32 questclearcond);		// 캐릭터가 진행하고 있는 퀘스트를 점검하여 종결시킨다. 
	void MakeRankMarkEncBuffer();
	
	void DrawGate(SI32 id);		// 오사카 성문 불꽃
	bool NewYearEventDay(void);

	// 퀘스트 부여 이미지를 갖고 온다
	SI16 GetQuestDisplayMark( cltSystemNPC* pclSystemNPCInfo );

	bool IsBlankInv(SI32 invnum, SI32 invmode);

};



#endif 
