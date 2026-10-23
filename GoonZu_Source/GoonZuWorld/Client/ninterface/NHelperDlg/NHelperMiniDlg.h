/* ==========================================================================
클래스 :		 

작성일 :		 
작성자 :		 

변경사항 :		 

차후작업 :		
========================================================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"


class NHelperMiniDlg : public CDialogController
{
public:

	NHelperMiniDlg();
	~NHelperMiniDlg();

	void Init();

	void Create();
	void Destroy();

	void Action();

	static	void CALLBACK StatiNHelperMiniDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
			void CALLBACK NHelperMiniDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

public:


private:
	InterfaceFactory	m_InterfaceFactory;

};
