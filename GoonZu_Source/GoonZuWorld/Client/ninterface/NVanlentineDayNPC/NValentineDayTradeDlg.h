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

class NValentineDayTradeDlg : public CDialogController
{
public:

	NValentineDayTradeDlg();
	~NValentineDayTradeDlg();

	void Create();
	void Destroy();
	void Action();
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void SetMyInven( void );
	void SetNPCInven( void );

	static void CALLBACK StaticNValentineDayTradeDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NValentineDayTradeDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
private:
	InterfaceFactory	m_InterfaceFactory;
	SI16				m_siNPCInvSelectedIndex;
	cltNPCMerchantInfo* m_pNPCMerchantInfo;
};
 