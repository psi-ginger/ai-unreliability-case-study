#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../NLib/NUpdateTimer.h"

class CNWeddingAnniversaryDlg : public CDialogController
{
private:
	CButton*		m_pclGetItem;

	NUpdateTimer	m_updateTimer;

public:
	CNWeddingAnniversaryDlg( void );
	~CNWeddingAnniversaryDlg( void );

	void Init();
	void Create();

	void Action();

	static void CALLBACK StaticCallBackDialogNWeddingAnniversary( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNWeddingAnniversary( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void OnClickButtonGetItem( void );

};
