#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

#include "../../CommonLogic/SoulGuardMgr/SoulGuardMgr.h"
#include "../../common/SoulGuard/SoulGuard.h"
#include "../../common/Char/CharAni/CharAni.h"

class cltCharClient;
class cltParametaBox;
class cltCharManager;
class cltItem;
class cltPItemInfo;

class CButton;
class CCheckBox;
class CComboBox;
class CEdit;
class CImageStatic;
class CList;
class CRadioButton;
class CStatic;
class COutLine;


class cltNSoulGuardStatus : public CDialogController
{
public:

	cltNSoulGuardStatus(cltCharManager* pclcm);
	~cltNSoulGuardStatus();

	void Create( CControllerMgr *pParent );
	bool IsShow();
	void Show();
	void Hide();
	void Action();
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	cltAniInfo m_clAniInfo;

	static void CALLBACK NSoulGuardStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSoulGuardStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Close();

	void SoulGuardUpdate( void );
	void AllShowControllerAndVariableClear( void );

	BOOL IsSoulGuardHouseUpdate( void );
	void SoulGuardHouseUpdate( void );
	SI32 GetSoulGuardPortraitFontNum( SI16 siKindUnique );
	void SetSoulGuardHouseBackFontIndex( SI32 siSelectIndex );

public:
	void OnButtonClickNameChange( void );
	void OnButtonClickInOut( void );
	void OnButtonClickFree( void );
	void OnButtonClickHouseCreate( void );

	void OnButtonClickHouse( SI32 siControllerID );

public:
	bool				m_bShow;

	cltCharManager*		m_pclCM;
	
	SI32				m_siCharUnique;

	TCHAR				m_szShowSoulGuardName[ MAX_SOULGUARD_NAME ];
	SI16				m_siShowSoulGuardKind;
	SI16				m_siShowStatus;
	SI16				m_siShowFellowShip;

	SI32				m_siFreeSoulGuardID;

public:
	SI16				m_siCheckSoulGuardHouse[ MAX_SOULGUARD_HOUSE_NUM ];
	SI16				m_siShowHouseNum;

	SI32				m_siCurrentSelectHouse;

public:
	CImageStatic*		m_pclImage_Portrait;

	CStatic*			m_pclStatic_Noname1;
	CStatic*			m_pclStatic_Noname2;
	CStatic*			m_pclStatic_Noname3;
	CStatic*			m_pclStatic_Noname4;
	CStatic*			m_pclStatic_FellowShip;
	CStatic*			m_pclStatic_Life;
	CStatic*			m_pclStatic_Noname7;
	CStatic*			m_pclStatic_Noname8;

	CButton*			m_pclButton_NameChange;
	CButton*			m_pclButton_InOut;
	CButton*			m_pclButton_Free;
	CButton*			m_pclButton_HouseCreate;

	CEdit*				m_pclEdit_Name;
	CEdit*				m_pclEdit_Basic;
	CEdit*				m_pclEdit_Special;

	COutLine*			m_pclOutLine_Noname1;
	COutLine*			m_pclOutLine_Noname2;
	COutLine*			m_pclOutLine_Noname3;

	// 수호정령 보관소
public:
	CImageStatic*		m_pclImage_Back[MAX_SOULGUARD_HOUSE_NUM];
	CImageStatic*		m_pclImage_House[MAX_SOULGUARD_HOUSE_NUM];

	CButton*			m_pclButton_House[MAX_SOULGUARD_HOUSE_NUM];


};

