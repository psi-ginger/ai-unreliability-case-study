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

#include "../../Promoter/NHNChinaPromoter.h"

class NPromoter_UrlDlg : public CDialogController
{
public:

	NPromoter_UrlDlg();
	~NPromoter_UrlDlg();

	void	Create( CControllerMgr *pParent );
	void 	Destroy();
	void 	Action();

	void	Hide();
	void	Show();

	void	Request( void );
	void	SetURL( TCHAR* _pszURL );


	static void CALLBACK StatiNPromoter_UrlDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NPromoter_UrlDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

   
private:
	InterfaceFactory	m_InterfaceFactory;

	char 				m_code[NHNCHINAPROMOTER_CODE_SIZE];
	char 				m_url[NHNCHINAPROMOTER_URL_SIZE];

};
 