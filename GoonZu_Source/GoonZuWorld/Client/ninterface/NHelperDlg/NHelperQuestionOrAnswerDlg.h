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

enum
{
	 QUESTION_DLG
	,ANSWER_DLG
};

class NHelperQuestionOrAnswerDlg : public CDialogController
{
public:

	NHelperQuestionOrAnswerDlg();
	~NHelperQuestionOrAnswerDlg();

	void Init();

	void Create();
	void Destroy();

	void Action();

	static	void CALLBACK StaticNHelperQuestionOrAnswerProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
			void CALLBACK NHelperQuestionOrAnswerProc( UINT nEvent, int nControlID, CControllerObj* pControl );

public:
	void SetQuestionDlg( bool _bQuestionDlg );
	void SetQuestionCharInfo( const TCHAR* _pszName, TCHAR* _pszQuestionTitle, SI32 _siQuestionPersonID );

private:
	InterfaceFactory	m_InterfaceFactory;

	TCHAR				m_szTitle[ TITLE_LETTER_NUM ];							// 질문이나 답변 제목
	TCHAR				m_szContents[ CONTENTS_LETTER_NUM ];					// 질문이나 답변 내용.

	bool				m_bQuestionDlg;											// 질문창인가 답변창인가.

	TCHAR				m_szQuestionCharName[MAX_PLAYER_NAME];					// 질문자 이름.
	SI32				m_siQuestionPersonID;									// 질문자 personID

	DWORD				m_dwPrevTime;											// 답변할때 시간 카운트.
};
