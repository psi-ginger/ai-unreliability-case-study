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


class cltNSoulGuardHouseStatus : public CDialogController
{
public:

	cltNSoulGuardHouseStatus();
	~cltNSoulGuardHouseStatus();

	void Create( );
	void Action();
	
	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY );
	
	static void CALLBACK NSoulGuardHouseStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSoulGuardHouseStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void OnButtonClickSummon( void );

public:
	SI16			m_siCurrentSelectIndex;
	cltAniInfo		m_clAniInfo;

public:
	CImageStatic*	m_pclImage_Back;

	CButton*		m_pclButton_Summon;

public:
	void SetCurrentSelectIndex( SI16 siCurrentSelectIndex )	{	m_siCurrentSelectIndex = siCurrentSelectIndex;	}
};

