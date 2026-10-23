
#pragma once

#include "../../InterfaceMgr/DialogController.h"

#include "../../CommonLogic/Marriage/WeddingManager.h"
#include "../../CommonLogic/Marriage/MarriageManager.h"

/*

*/

class CNWeddingCeremonyListDlg : public CDialogController
{

// -- Member Variable Area
private:
	InterfaceFactory		m_InterfaceFactory;

	SI32					m_siRequestPage;

	SI16					m_siCancelYear;
	SI16					m_siCancelMonth;
	SI16					m_siCancelDay;
	SI16					m_siCancelHour;

	cltWeddingReserveUnit	m_clList[WEDDING_RESERVE_LIST_PAGE_PER_NUM];
protected:

public:


// -- Member Function Area
private:

protected:

public:
	CNWeddingCeremonyListDlg( void );
	~CNWeddingCeremonyListDlg( void );

	void Init();
	void Create(CControllerMgr* pParent);

	static void CALLBACK StaticCallBackDialogNWeddingCeremonyListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNWeddingCeremonyListDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void Show()	{	CControllerMgr::Show( true );	}
	void Hide()	{	CControllerMgr::Show( false );	}

	void RequestList( void );
	void SetList( cltWeddingReserveUnit* pclList );
	void RefreshList( void );

public:
	void OnButtonClickCancel( void );
	void OnButtonClickEnter( void );
	void OnButtonClickPrevious( void );
	void OnButtonClickNext( void );

};