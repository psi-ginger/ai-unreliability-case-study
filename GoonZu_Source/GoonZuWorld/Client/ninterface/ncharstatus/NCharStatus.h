//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		cltNCharStatus

	작성일 :		05/03/14
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#pragma once

//#define _LEON_NCHARSTATUS_

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

#define MAX_CHARSTATUS_PAGE_NUMBER	5

#define WANTIED_ANIMAL_CHANGE_TYPE	2

#define LEAST_COMMON_MULTILPLE	30	// 각 탭의 Action 반복 횟수 최소공배수값
#define	QUEST_TAB_ACTION_INTERVAL_VALUE 7 // 퀘스트 창 갱신 주기 값
#define	ASSET_TAB_ACTION_INTERVAL_VALUE 25 // 자산 창 갱신 주기 값

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
class CControllerObj;

class cltNPersonStatus;	
class cltNHorseStatus	;	
class cltNSummonStatus;	
class cltNAssetStatus	;	
class cltNQuestStatus	;
class cltNSoulGuardStatus;

// 상태 정보 창 메인


class cltNCharStatus : public CDialogController
{
public:
	

	enum
	{
		BIG_DISPLAY_MODE_DELTA_X	= 16,
		SMALL_DISPLAY_MODE_DELTA_Y	= 80,        
		DLG_BIG_POS_X				= 1024 - 300 + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y				= 100,
		DLG_SMALL_POS_X				= 800 - 300 + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y				= 100,
	};

	struct stTabTextInfo
	{
		CControllerObj	*st_pControllerObj;
		COLORREF		st_Color;
		SI16			st_siWeight;

		stTabTextInfo()
		{
			Init();
		}

		void Init()
		{
			memset( this, 0, sizeof( stTabTextInfo ));
		}

		void Set( CControllerObj* in_pControllerObj, COLORREF in_Color, SI16 in_siWeight )
		{
			st_pControllerObj	= in_pControllerObj;
			st_Color			= in_Color;
			st_siWeight			= in_siWeight;
		}

	};

	SI32 m_timerCount;	// Timer Counting 


	//CDialogController* m_pTabDlg[MAX_CHARSTATUS_PAGE_NUMBER];
	cltNPersonStatus	*m_pTabDlg0;
	cltNHorseStatus		*m_pTabDlg1;
	cltNSummonStatus	*m_pTabDlg2;
	cltNAssetStatus		*m_pTabDlg3;
	cltNSoulGuardStatus	*m_pTabDlg4;


	BOOL DlgRectInitSwitch;
	RECT DlgRect;

	TCHAR strHorseName[ MAX_HORSE_NAME ];
	TCHAR strSummonName[ MAX_SUMMON_NAME ];

	SI32 CharUnique;

	clock_t			m_LastTakeInOutTime;
	SI32 m_siCurrentStatusPage;
	//bool bBonusButtonEnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	//bool bBonusButton2EnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	BOOL BonusSummonButtonEnableSwitch;		// 소환수 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	BOOL BonusHorseButtonEnableSwitch;		// 말 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수.
	
	BOOL m_bChangedInitSummonStatus;
	BOOL m_siLastSummonComboList;			// 마지막으로 Combo List로 설정된 소환수 
	SI32 m_siPortraitSummonKind;


	cltCharManager* pclCM;
	//cltNHorseTypeChange* pclHorseTypeChange;
	//cltNCharNameChange* pclCharNameChange;
	//cltNCharKindChange* pclCharKindChange;

	CButton			*m_pButton_person;
	CButton			*m_pButton_horse;
	CButton			*m_pButton_summon;
	CButton			*m_pButton_stock;
	//CButton			*m_pButton_quest;
	CButton			*m_pButton_soulguard;
	CButton			*m_pButton_ok;
	CButton			*m_pButton_warp;
	bool m_bShow;

	//cltNCharStatus(cltCharManager* pclCM);
	cltNCharStatus();
	~cltNCharStatus();

	void Create();

	static void CALLBACK NCharStatusDlgStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NCharStatusDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	bool IsShow();
	void Show();
	void Hide();
	void Action();	
	void SetTabTextInfo( stTabTextInfo* in_SelectedText );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	//KHY - 0703 - 게임 머니로 귀환하기.	
	SI32 WarpMoney();
	void ChangeButtonText();

	void ShowCurrentTab( SI32 siTabNumber );

	// 패어런트 종류에 따라 차일드 위치가 다르게 된다
	SI16			m_siChildDlgXPos;
	SI16			m_siChildDlgYPos;

	SI16			GetChildDlgXPos( void )	{	return m_siChildDlgXPos;	}
	SI16			GetChildDlgYPos( void )	{	return m_siChildDlgYPos;	}

};


//#endif
