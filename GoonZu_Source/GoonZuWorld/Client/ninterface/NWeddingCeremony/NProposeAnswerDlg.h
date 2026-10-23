
#pragma once

#include "../../InterfaceMgr/DialogController.h"



class CNProposeAnswerDlg : public CDialogController
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
	CNProposeAnswerDlg( void );
	~CNProposeAnswerDlg( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNProposeAnswerDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNProposeAnswerDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void OnButtonClickYes( void );
	void OnButtonClickNo( void );

public:
	void SetPropose( TCHAR* pszCharName, TCHAR* pszMessage );

	void AnswerThePropose( bool bAccept );
};