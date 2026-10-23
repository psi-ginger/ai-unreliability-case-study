
#pragma once

#include "../../InterfaceMgr/DialogController.h"



class CNProposeQuestionDlg : public CDialogController
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
	CNProposeQuestionDlg( void );
	~CNProposeQuestionDlg( void );

	void Init();
	void Create();

	static void CALLBACK StaticCallBackDialogNProposeQuestionDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNProposeQuestionDlg( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void OnButtonClickPropose( void );

public:
	BOOL CheckCondition( void );

};