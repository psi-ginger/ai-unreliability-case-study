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


class NHelperQuestionListDlg : public CDialogController
{
public:

	void test(  );

	NHelperQuestionListDlg();
	~NHelperQuestionListDlg();

	void Init();

	void Create();
	void Destroy();

	void Action();

	static	void CALLBACK StatiNHelperQuestionListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
			void CALLBACK NHelperQuestionListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void SetQuestionTileList( SI32* _psiQuestionPersonID, TCHAR** _pszQuestionTitle, TCHAR** _pszName, bool* _pbAleadyAnswer );
	void SetQuestionContents( TCHAR* pContents );
	void GM_Mode( bool _bShow );

private:
	InterfaceFactory	m_InterfaceFactory;

	SI32	m_siQuestionPersonID[ MAX_QUESTION_NUM ];							// 질문자 personID
	TCHAR	m_szQuestionTitle[ MAX_QUESTION_NUM ][TITLE_LETTER_NUM];			// 제목
	TCHAR	m_szName[ MAX_QUESTION_NUM ][MAX_PLAYER_NAME];

	SI32    m_siQuestionListNum;												// 질문 개수;
};
