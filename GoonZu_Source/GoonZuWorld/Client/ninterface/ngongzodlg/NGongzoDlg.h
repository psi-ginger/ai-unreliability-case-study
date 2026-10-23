#pragma once 

#include "../../InterfaceMgr/DialogController.h"
#include "../common/CommonHeader.h"
#include "../Server/Minister/Minister-Hozo.h"
#include "../Lib/Directives.h"

class CImageStatic;
class CButton;
class CComboBox;
class CListView;
class CEdit;
class CStatic;
class CList;
class COutLine;


enum
{
	GOONGZO_SETMODE_NONE,
	GOONGZO_SETMODE_WEAPON,
	GOONGZO_SETMODE_ARMOR
};

class CNGongzoDlg : public CDialogController
{
public:

	CNGongzoDlg();
	~CNGongzoDlg();

	void Create();
	void Action();

	static void CALLBACK StaticCallBackDialogNGongzoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNGongzoDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetGongzo(CMinisterBase *pMini,SI32 pricetime,SI16 leftovervalue,GongzoMember *gongzomember,SI16 leftovervalue_armor);

	void SetSpinButton();
	void SendGongzoMessage();
	void SetSpinDis(BOOL bView);
	void SetSpinValue(SI32 skillunique,SI32 editunique);
	void SetSpinData(SI32 skillunique,SI32 editunique,SI32 staticunique);

	void UpWeaponExp(SI32 Index);
	void DownWeaponExp(SI32 Index);

	void SetCraftFund(SI32 inc);

	void EnableButton(CButton* pButton, BOOL Flag);	// 버튼 활성화 비활성화
	void SetButtonTextColor(CButton* pButton, BOOL Flag);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
private:

	TCHAR m_szGongzoName[MAX_PLAYER_NAME];
	TCHAR m_szGongzoProfile[256];
	TCHAR m_szGongzoID[MAX_PLAYER_NAME];

	SI32			m_siPriceTime; // 설정 시간
	SI16			m_siLeftOverValue; // 남은 증감 수치
	SI16			m_siSetSkill ; // 설정한 스킬의 유니크
	SI32			m_siIDCEditNum ; // 설정한 스킬의 에디트 박스
	SI32			m_siSpinNum; // 설정한 스킬의 스핀버튼
	SI16			m_siLeftOverValue_Armor; // 남은 증감 수치

	GongzoMember	m_stGongzoMember;

	BOOL m_bSet;
	BOOL m_bSetSpin ; // 이미 스핀 버튼을 한번 눌러서 셋업했다.
	BOOL m_bMonthOver; // 설정한지 한달이 지났다.
    BOOL 	m_bGongzoCheck;
	BOOL m_bSoundCheck;
	BOOL m_bGongzoReturnValue; 

	CStatic*	m_pStatic[33];
	CButton*	m_pButton[19];
	CEdit*		m_pEdit[8];
	COutLine*	m_pOutLine[4];

	CButton*	m_pButton_Weapon;
	CButton*	m_pButton_Armor;

	SI32		m_siSetMode;
};