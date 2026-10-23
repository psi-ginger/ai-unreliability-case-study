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

class NElixirStatusDlg : public CDialogController
{
public:

	NElixirStatusDlg();
	~NElixirStatusDlg();

	void Init();

	void Create();
	void Destroy();
	void Action();

    static void CALLBACK StatiNElixirStatusDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NElixirStatusDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
public:
	void	Calculation( SI32 siStatusType, SI32 siNum );
	void	SetElixirInfo( cltElixir* _pcltElixir = NULL );
	TCHAR*	GetStatusText( SI32 _siType );

private:
	InterfaceFactory	m_InterfaceFactory;

	cltElixir			m_cltElixir;
	cltElixir			m_cltPrevElixir;
};
 