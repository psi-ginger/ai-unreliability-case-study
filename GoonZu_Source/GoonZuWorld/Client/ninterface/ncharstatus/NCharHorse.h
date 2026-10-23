//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		cltNHorseStatus

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

// 말 상태 창
class cltNHorseStatus : public CDialogController
{
public:

	CImageStatic	*m_pImagestatic_portrait;
	CStatic			*m_pStatictext_horsename;
	CEdit			*m_pEditbox_horsename;
	CButton			*m_pButton_changename;
	CStatic			*m_pStatictext_horsekind;
	CStatic			*m_pStatictext_horsekindname;
	CStatic			*m_pStatictext_birthday;
	
	//CButton			*m_pButton_changetype;
	
	CStatic			*m_pStatictext_birthdayname;
	CStatic			*m_pStatictext_age;
	CStatic			*m_pStatictext_agevalue;
	CStatic			*m_pStatictext_life;
	CStatic			*m_pStatictext_lifevalue;
	CStatic			*m_pStatictext_sex;
	CStatic			*m_pStatictext_sexkind;
	CStatic			*m_pStatictext_speed;
	CStatic			*m_pStatictext_strength;
	CStatic			*m_pStatictext_dex;
	CStatic			*m_pStatictext_magic;
	CStatic			*m_pStatictext_bonus;
	CEdit			*m_pEditbox_speed;
	CEdit			*m_pEditbox_strength;
	CEdit			*m_pEditbox_dex;
	CEdit			*m_pEditbox_magic;
	CEdit			*m_pEditbox_bonus;
	CButton			*m_pButton_speed;
	CButton			*m_pButton_strength;
	CButton			*m_pButton_dex;
	CButton			*m_pButton_magic;
	CButton			*m_pButton_ridehorse;
	CButton			*m_pButton_explain;
	
	//CButton			*m_pButton_init;

	
	CButton			*m_pButton_addstatus;
	CCheckBox		*m_pCheckbox_autofood;
	CButton			*m_pButton_free;
	CStatic			*m_pStatictext_free;
	CEdit			*m_pEditbox_need;

	COutLine		*m_pOutline_noname1	;
	COutLine		*m_pOutline_noname2	;
	COutLine		*m_pOutline_noname3	;

	//KHY - 1001 - 기승동물 5마리 보유 수정.
	CComboBox		*m_pCombobox_horse;

	CButton			*m_pButton_oilling;
	
	BOOL 		m_bChangedInitHorseStatus;
	SI16 		m_siLastHorseComboList;	//마지막으로 타고 있는 기승동물

	
	cltNHorseStatus(cltCharManager* pclCM);
	~cltNHorseStatus();

	void Create( CControllerMgr *pParent );
	bool IsShow();
	void Show();
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
	//KHY - 1001 - 기승동물 5마리 보유 수정.
	void InitHorseStatusInfo();
	
	void UpdateHorseStatusInfo();
	void SendUseHorseBonusMessageToServer(SI08 data);

	cltCharManager* pclCM;
	bool m_bShow;
	bool bBonusButtonEnableSwitch;			// 보너스 올려주는 버튼이 활성화 되어 있는지 확인하는 변수. 
	bool BonusHorseButtonEnableSwitch;
	TCHAR strHorseName[ MAX_HORSE_NAME ];

	SI32 CharUnique;

	static void CALLBACK NHorseStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NHorseStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Close();

	SI32 GetPremiumStatusHorse_AGI( SI32 siSlotUnique );	// 프리미엄 파츠로 인해서 얻는 스탯
	SI32 GetPremiumStatusHorse_STR( SI32 siSlotUnique );	// 프리미엄 파츠로 인해서 얻는 스탯
	SI32 GetPremiumStatusHorse_WIS( SI32 siSlotUnique );	// 프리미엄 파츠로 인해서 얻는 스탯
};

//#endif
