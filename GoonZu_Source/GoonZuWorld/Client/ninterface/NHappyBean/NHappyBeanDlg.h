/*====================================

클래스 : CNHappyBeanDlg
작성일 : 07 .08 . 22
작성자: 손성웅
변경사항:
차후작업:
=====================================*/

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CNHappyBeanDlg : public CDialogController
{
public:
	CNHappyBeanDlg();
	~CNHappyBeanDlg();

	void init();
	void Destroy();

	void Create();

	static void CALLBACK StaticSelectHappyBeanProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSelectHappyBeanProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void SelectHappyBean( );

};