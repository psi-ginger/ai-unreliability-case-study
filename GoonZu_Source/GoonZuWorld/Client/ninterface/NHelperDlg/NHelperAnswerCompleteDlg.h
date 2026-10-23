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
#include "../../Server/HelperManager/HelperManager.h"

class NHelperAnswerCompleteDlg : public CDialogController
{
public:

	NHelperAnswerCompleteDlg();
	~NHelperAnswerCompleteDlg();

	void Init();

	void Create();
	void Destroy();

	void Action();

	static	void CALLBACK StaticNHelperAnswerCompleteDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
			void CALLBACK NHelperAnswerCompleteDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void SetAnswer( SI32 _siAnswerPersonID, TCHAR* _pAnswerCharName, TCHAR* _pTitle, TCHAR* _pContents );


private:
	InterfaceFactory	m_InterfaceFactory;

	SI32				m_siAnswerCharPersonID;									// 답변자 personID
};
