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
#include "MsgType-System.h"

class NValentineDayPackingDlg : public CDialogController
{
public:

	NValentineDayPackingDlg();
	~NValentineDayPackingDlg();

	void Create();
	void Destroy();
	void Action();
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void SetItemInfo( void );

    static void CALLBACK StaticNValentineDayPackingDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NValentineDayPackingDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
private:
	InterfaceFactory	m_InterfaceFactory;
	
	cltItem*			m_pclInvenItem;
};
 