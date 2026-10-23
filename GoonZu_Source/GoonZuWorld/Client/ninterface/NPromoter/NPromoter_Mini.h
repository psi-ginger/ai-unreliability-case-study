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

class NPromoter_Mini : public CDialogController
{
public:

	NPromoter_Mini();
	~NPromoter_Mini();

	void Create();
	void Action();

	static void CALLBACK StatiNPromoter_MiniProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK Promoter_MiniProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
private:
	InterfaceFactory	m_InterfaceFactory;
	
	DWORD				m_dwDeleteDialogTickCount;
};
 