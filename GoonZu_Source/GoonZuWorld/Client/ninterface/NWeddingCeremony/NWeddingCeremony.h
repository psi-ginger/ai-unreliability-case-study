#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../CommonLogic/Marriage/WeddingManager.h"

class CNWeddingCeremonyListDlg;

class CNWeddingCeremony : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory			m_InterfaceFactory;

	// 예약 리스트
	cltWeddingReserveSimpleUnit	m_clSimpleList[WEDDING_RESERVE_LIST_NUM];

	// 예약 하려는 정보
	SI16						m_siReserveYear;
	SI16						m_siReserveMonth;
	SI16						m_siReserveDay;
	SI16						m_siReserveHour;

	CNWeddingCeremonyListDlg*	m_pWeddingListDlg;

protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNWeddingCeremony( void );
	~CNWeddingCeremony( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNWeddingCeremony( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNWeddingCeremony( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void OnButtonClickReserve( void );
	void OnButtonClickPropose( void );
	void OnButtonClickApply( void );

public:
	void SetReserveSimpleList( cltWeddingReserveSimpleUnit* pclSimpleList );
	void RefreshReserveList( void );

	void SetReserveList( cltWeddingReserveUnit* pclList );

	void RequestList( void );

public:
	void ChangeTabDialog( SI32 siControllerID );
};