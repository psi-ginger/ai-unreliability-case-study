
#pragma once

#include "../../InterfaceMgr/DialogController.h"

enum WARNDURABILITY_SHOW_COLOR
{
	WARNDURABILITY_SHOW_COLOR_NONE = 0
	,WARNDURABILITY_SHOW_COLOR_01
	,WARNDURABILITY_SHOW_COLOR_02
	,WARNDURABILITY_SHOW_COLOR_03
};


typedef struct _stWarnController
{
	SI32	m_siInventoryPos;	// 현재 검색하고자 하는 아이템의 인벤토리 위치
	SI32	m_siControllerID;	// 인벤토리 위치에 해당하는 컨트롤러
	SI32*	m_psiCompareValue;	// 동일한 품목(ex 귀걸이1, 귀걸이2)중 어떤것이 더 내구도 소모가 심한지 비교하기 위함(둘중 가장 작은값이 나타나야 되기 때문이다)
} stWarnController;


class CNWarnDurabilityDlg : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory m_InterfaceFactory;
protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNWarnDurabilityDlg( void );
	~CNWarnDurabilityDlg( void );

	void Init();
	void Create();

	void Action();

	static void CALLBACK StaticCallBackDialogNWarnDurabilityDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNWarnDurabilityDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void SetAllControllersShowMode( bool bShowMode );
	void WarnDurabilityForAction( void );
};