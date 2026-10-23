#pragma once


#include <Directives.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../InterfaceMgr/DialogController.h"
#include "TSpr.h"


class CStatic;
class CButton;
class CRadioButton;
class CEdit;
class COutLine;
class CComboBox;
class cltQuestInfo;


class CNQuestViewerDlg : public CDialogController
{
public:
	CNQuestViewerDlg();
	~CNQuestViewerDlg();

	void					Create( );

	static void CALLBACK	StatiCNQuestViewerDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NQuestViewerDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void ShowQuestView(SI32 QuestUnique, SI32 QuestStep);
	void ShowOtherThings(SI32 QuestUnique, SI32 QuestStep);
	void InitCombo();
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	inline void SetNpcFontIndex(SI32 index)
	{
		m_siNpcFontType = index;
	}
	// cltTSpr	    m_SprNpcFace;
	TSpr	    m_SprNpcFace;

private:
	CStatic *m_pstatictext_SmallQuestStatic1;
	CStatic *m_pstatictext_SmallQuestStatic2;
	CRadioButton *m_pradiobutton_QuestViewer1;
	CRadioButton *m_pradiobutton_QuestViewer2;
	CRadioButton *m_pradiobutton_QuestViewer3;
	CRadioButton *m_pradiobutton_QuestViewer4;
	CRadioButton *m_pradiobutton_QuestViewer5;
	CButton *m_pbutton_QuestViewer1;
	CButton *m_pbutton_QuestViewer2;
	CButton *m_pbutton_QuestViewer3;
	CButton *m_pbutton_QuestViewer4;
	//CEdit *m_peditbox_BigQuestEdit;
	//CEdit *m_peditbox_SmallQuestEdit;
	CEdit *m_peditbox_QuestViewer;
	COutLine *m_poutline_QuestViewer;
	CComboBox *m_pCompobox_QuestViewer;

	SI32		m_siNpcKind;
	SI32		m_sirealNpcKind;
	SI32		m_siNpcFontType;
	SI32        m_siQuestUnique;

	SI16		m_siButtonType;
	bool m_bOpened;

};

