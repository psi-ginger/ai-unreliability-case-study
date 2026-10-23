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

class NElixirUseDlg : public CDialogController
{
public:

	NElixirUseDlg();
	~NElixirUseDlg();

	void Init();

	void Create();
	void Destroy();
	void Action();

	static void CALLBACK StatiNElixirUseDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NElixirUseDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

public:
	void SetItemUnique( SI32 _siClickItemUnique );
	void ChangeMultiple( void );
	void ElixirStart( void );
	

private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siClickItemUnique;
};
 