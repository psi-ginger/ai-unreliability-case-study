#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../Common/Event/MofuMofu/MofuMofu.h"

class CList;
class CButton;

class CNMofuMofuListDlg : public CDialogController
{
public:
	CNMofuMofuListDlg();
	~CNMofuMofuListDlg();

	void Create();
	
	static	void CALLBACK StaticNMofuMofuListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl );
	void CALLBACK NMofuMofuListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	SI32	m_siFieldIndex;
	clock_t m_ReceivedClock;
	
	void RefreshList();
	void SetReserveList(SI32 siCount, cltMofuMofuEventReserveUnit* pReserveList);

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetViewerMode()
	{
		m_pMofuMofuListbutton->Enable(false);
	}

private:
	CList	*m_pMofuMofuListBox;
	CButton	*m_pMofuMofuListbutton;
	CButton	*m_pMofuMofuListbutton_close;

	cltMofuMofuEventReserveUnit m_clUnit[MOFUMOFUEVENT_ROOMNUM];

	SI16	m_siYear;
	SI16	m_siMonth;
	SI16	m_siDay;
	SI16	m_siHour;
};