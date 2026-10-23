//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		cltNPersonStatus

	작성일 :		05/03/14
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

#define MAX_CHARSTATUS_PAGE_NUMBER	5

#define WANTIED_ANIMAL_CHANGE_TYPE	2

class cltCharClient;
class cltParametaBox;
class cltSummonScroll;
class cltCharManager;
class cltItem;

class CButton;
class CCheckBox;
class CComboBox;
class CEdit;
class CImageStatic;
class CList;
class CRadioButton;
class CStatic;
class COutLine;

// 캐릭터 상태 창
class cltNPersonStatus : public CDialogController
{
public:
	enum CHARSTATUS_VARIABLE
	{
		 CHARSTATUS_HP_NUM			= 0		
		,CHARSTATUS_MP_NUM			= 4		
		,CHARSTATUS_EXP_NUM			= 8		

		// 게이지의 세로 길이		  
		,CHARSTATUS_BAR_WIDTH		= 144	
		// HP 게이지 시작위치		 
		,CHARSTATUS_HP_START_X		= 123	
		,CHARSTATUS_HP_START_Y		= 46	
		// MP 게이지 시작위치		 
		,CHARSTATUS_MP_START_X		= 123	
		,CHARSTATUS_MP_START_Y		= 58	
		// EXP 게이지 시작위치		 
		,CHARSTATUS_EXP_START_X		= 123	
		,CHARSTATUS_EXP_START_Y		= 70	
	};

	CImageStatic	*m_pImagestatic_portrait;
	CStatic			*m_pStatictext_level;
	CButton			*m_pButton_changename;
	CStatic			*m_pStatictext_exp;
	CStatic			*m_pStatictext_expvalue;
	CStatic			*m_pStatictext_magic1;
	CStatic			*m_pStatictext_magic1value;
	CStatic			*m_pStatictext_health;
	CStatic			*m_pStatictext_healthvalue;
	CStatic			*m_pStatictext_life;
	CStatic			*m_pStatictext_lifevalue;
	CStatic			*m_pStatictext_fame;
	CStatic			*m_pStatictext_famevalue;
	CStatic			*m_pStatictext_country;
	CStatic			*m_pStatictext_countryex;
	CStatic			*m_pStatictext_life2;
	CStatic			*m_pStatictext_strength;
	CStatic			*m_pStatictext_dex;
	CStatic			*m_pStatictext_magic;
	CStatic			*m_pStatictext_bonus;
	CEdit			*m_pEditbox_life2;
	CEdit			*m_pEditbox_strength;
	CEdit			*m_pEditbox_dex;
	CEdit			*m_pEditbox_magic;
	CEdit			*m_pEditbox_bonus;
	CButton			*m_pButton_life2_plus;
	CButton			*m_pButton_strength_plus;
	CButton			*m_pButton_dex_plus;
	CButton			*m_pButton_magic_plus;
	CButton			*m_pButton_changekind;
	CButton			*m_pButton_greeting;
	CButton			*m_pButton_naming;
	CButton			*m_pButton_explain;
	CButton			*m_pButton_init;
	CEdit			*m_pEditbox_health_explain;
	CStatic			*m_pStatictext_levelvalue;
	CStatic			*m_pStatictext_hand;
	CEdit			*m_pEditbox_hand;
	CButton			*m_pButton_hand_plus;

	CStatic			*m_pStatictext_luk;
	CEdit			*m_pEditbox_luk;
	CButton			*m_pButton_luk_plus;
	CButton			*m_pButton_luk_minus;
	CComboBox		*m_pCombobox_luk;

	CButton			*m_pButton_strength_minus;
	CButton			*m_pButton_dex_minus;
	CButton			*m_pButton_magic_minus;
	CButton			*m_pButton_life2_minus;
	CButton			*m_pButton_hand_minus;
	CComboBox			*m_pCombobox_strength;
	CComboBox			*m_pCombobox_dex;
	CComboBox			*m_pCombobox_magic;
	CComboBox			*m_pCombobox_life2;
	CComboBox			*m_pCombobox_hand;
	CStatic			*m_pStatictext_totalspeed;
	CStatic			*m_pStatictext_totaldamage;
	CStatic			*m_pStatictext_armourdefense;
	CEdit			*m_pEditbox_totalspeed;
	CEdit			*m_pEditbox_totaldamage;
	CEdit			*m_pEditbox_armourdefense;
	CComboBox			*m_pCombobox_detail_speed;
	CComboBox			*m_pCombobox_apowername;
	CComboBox			*m_pCombobox_dpowername;
	CStatic				*m_pStatictext_rank	;
	CStatic				*m_pStatictext_rank2 ;
	CButton				*m_pButton_nolimitwarp	;
	COutLine			*m_pOutline_noname1;
	COutLine			*m_pOutline_noname2;

	// 스태터스 분배 취소, 확인 버튼
	CButton			*m_pButton_StatusClear;
	CButton			*m_pButton_StatusApply;

	CStatic*		m_pStatictext_name;		
	CStatic*		m_pStatictext_namevalue;

	COutLine*		m_pOutline_NONAME3;
	COutLine*		m_pOutline_NONAME4;
	CButton*		m_pButton_ability_detail;
	CButton*		m_pButton_Wisdom_plus;
	CButton*		m_pButton_Wisdom_minus;
	CButton*		m_pButton_Elixir;		//[진성] 엘릭서.

	CStatic*		m_pStatictext_physical_attack; 		
	CEdit*			m_pEditbox_physical_attack;			
	CStatic*		m_pStatictext_pysical_defense;		
	CEdit*			m_pEditbox_pysical_defense;			
	CStatic*		m_pStatictext_masic_attack;			
	CEdit*			m_pEditbox_magic_attack;			
	CStatic*		m_pStatictext_magic_defense;		
	CEdit*			m_pEditbox_magic_defense;			
	CStatic*		m_pStatictext_pysical_hit_rank;		
	CEdit*			m_pEditbox_pysical_hit_rank;		
	CStatic*		m_pStatictext_pysical_dodge_rank;	
	CEdit*			m_pEditbox_pysical_dodge_rank;		
	CStatic*		m_pStatictext_magic_hit_rank;		
	CEdit*			m_pEditbox_magic_hit_rank;			
	CStatic*		m_pStatictext_magic_dodge_rank;		
	CEdit*			m_pEditbox_magic_dodge_rank;		
	CStatic*		m_pStatictext_move_speed;				
	CEdit*			m_pEditbox_move_speed;				
	CImageStatic*   m_pImageStatic_bonus;

	CEdit*			m_pStaticEditbox_life2;	
	CEdit*			m_pStaticEditbox_strength;
	CEdit*			m_pStaticEditbox_dex;	 
	CEdit*			m_pStaticEditbox_magic;	
	CEdit*			m_pStaticEditbox_hand;	
	CEdit*			m_pStaticEditbox_luk;	
	CEdit*			m_pStaticEditbox_Wisdom;
	CEdit*			m_pEditbox_Wisdom;

	CStatic*		m_pStatictext_HP;		
	CStatic*		m_pStatictext_MP;
	CStatic*		m_pStatictext_EXP;		
	CStatic*		m_pStatictext_FULL_EXP;		

	SI32 CharUnique;

	SI16 m_siTempStr;
	SI16 m_siTempDex;
	SI16 m_siTempMag;
	SI16 m_siTempVit;
	SI16 m_siTempHnd;
	SI16 m_siTempLuk;
	SI16 m_siTempWisdom;


	cltNPersonStatus(cltCharManager* pclcm);
	~cltNPersonStatus();

	void Create( CControllerMgr *pParent );
	bool IsShow();
	void Show();
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void UpdateCharStatusInfo();
	void ShowDetailStrInfo( cltParametaBox* pclPB );
	void ShowDetailMagInfo( cltParametaBox* pclPB );
	void ShowDetailDexInfo( cltParametaBox* pclPB );
	void ShowDetailVitInfo( cltParametaBox* pclPB );
	void ShowDetailHndInfo( cltParametaBox* pclPB );
	void ShowDetailLukInfo( cltParametaBox* pclPB );
	void ShowDetailSpeedInfo( cltParametaBox* pclPB, cltCharClient* pclchar);
	void ShowDetailDamageInfo( cltParametaBox* pclPB, cltItem* pclitemlist, SI32 activesweapon, cltCharCommon* pclchar);
	void ShowDetailArmourInfo( cltParametaBox* pclPB );
	void ShowDetailWisdomInfo( cltParametaBox* pclPB );
	
	SI32 GetTotalMinDamage(cltParametaBox* pclPB, cltItem* pclitemlist, SI32 activesweapon );
	SI32 GetTotalMaxDamage(cltParametaBox* pclPB, cltItem* pclitemlist, SI32 activesweapon );

	void SetTempStatusPoint(SI32 pos, SI32 amount);
	void ClearTempStatusPoint();
	void ApplyTempStatusPoint();

	void SendUseBonusMessageToServer(SI32 pos, SI32 amount);

	//void SendUseHorseBonusMessageToServer(SI08 data);

	bool m_bShow;

	bool bBonusButtonEnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	bool bBonusButton2EnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 

	cltCharManager* pclCM;


	static void CALLBACK NPersonStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NPersonStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Close();

	//[추가 : 황진성 2008. 2. 22 => 변신후 관련 능력치 증가부분 값 얻기.]
	SI32 AddTransformAttackDamage(void);
	SI32 AddTransformDeffense(void);

	//[진성] 게이지 이미지 그리기. 2008-3-24
	TSpr m_sprGageBack;
	TSpr m_sprGage;	

	//[진성] 캐릭터 정보창 개선 이미지 그리기. 2008-3-24
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );	
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	void DrawCharStatus( SI32 ScreenRX, SI32 ScreenRY );

	//[진성] 새로운 능력치 보여주기.
	void SetNewAbility( cltParametaBox* pclPB );
	
	//[진성] 추가 정보 표시부에 보여줄 정보.
	void SetStatusExplain( TCHAR* _pExplain, SI32 _siSize );

	//[진성] 기본 정보 표시에 투팁 추가. 근력, 손재주등의 글자에 툴팁이 나오게. => 2008-4-4
	void SetBaseStatusToolTip(void);
};

//#endif
