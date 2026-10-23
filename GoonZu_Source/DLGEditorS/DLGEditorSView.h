// DLGEditorSView.h : iCDLGEditorSView 클래스의 인터페이스
//


#pragma once

// --- DiagramEditor ---
#include "./DialogEditor/DEditor.h"

#define DRAW_OBJECT_NONE		0
#define DRAW_OBJECT_BUTTON		1
#define DRAW_OBJECT_EDIT		2
#define DRAW_OBJECT_STATIC		3
#define DRAW_OBJECT_CHECKBOX	4
#define DRAW_OBJECT_COMBOBOX	5
#define DRAW_OBJECT_GROUPBOX	6
#define DRAW_OBJECT_LISTBOX		7
#define DRAW_OBJECT_RADIOBUTTON	8
/**
leon add
* new controls
*/
#define DRAW_OBJECT_IMAGESTATIC 9
#define DRAW_OBJECT_LISTVIEW 10
#define DRAW_OBJECT_OUTLINE 11
#define DRAW_OBJECT_CHATEDIT 12



class CDLGEditorSView : public CView
{
protected: // serialization에서만 만들어집니다.
	CDLGEditorSView();
	DECLARE_DYNCREATE(CDLGEditorSView)

// 특성
public:
	CDLGEditorSDoc* GetDocument() const;

// 작업
public:

// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현
public:
	virtual ~CDLGEditorSView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CDialogEditorDemoView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditCopy();
	afx_msg void OnUpdateEditCopy(CCmdUI* pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnUpdateEditCut(CCmdUI* pCmdUI);
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditPaste(CCmdUI* pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnSettings();
	afx_msg void OnProperties();
	afx_msg void On100perc();
	afx_msg void On110perc();
	afx_msg void On120perc();
	afx_msg void On130perc();
	afx_msg void On140perc();
	afx_msg void On150perc();
	afx_msg void On200perc();
	afx_msg void On25perc();
	afx_msg void On50perc();
	afx_msg void On60perc();
	afx_msg void On70perc();
	afx_msg void On80perc();
	afx_msg void On90perc();
	afx_msg void OnUpdateProperties(CCmdUI* pCmdUI);
	afx_msg void OnSelectAll();
	afx_msg void OnAddButton();
	afx_msg void OnAddEdit();
	afx_msg void OnAddStatic();
	afx_msg void OnAddCheckbox();
	afx_msg void OnAddCombobox();
	afx_msg void OnAddGroupbox();
	afx_msg void OnAddListbox();
	afx_msg void OnAddRadiobutton();
	
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnUpdateZoomIn(CCmdUI* pCmdUI);
	afx_msg void OnUpdateZoomOut(CCmdUI* pCmdUI);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowGrid();
	afx_msg void OnUpdateShowGrid(CCmdUI* pCmdUI);
	afx_msg void OnSnapToGrid();
	afx_msg void OnUpdateSnapToGrid(CCmdUI* pCmdUI);
	afx_msg void OnMargins();
	afx_msg void OnUpdateMargins(CCmdUI* pCmdUI);
	afx_msg void OnRestraints();
	afx_msg void OnUpdateRestraints(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSelectAll(CCmdUI* pCmdUI);
	afx_msg void OnBottom();
	afx_msg void OnUpdateBottom(CCmdUI* pCmdUI);
	afx_msg void OnBottomAlign();
	afx_msg void OnUpdateBottomAlign(CCmdUI* pCmdUI);
	afx_msg void OnDown();
	afx_msg void OnUpdateDown(CCmdUI* pCmdUI);
	afx_msg void OnFront();
	afx_msg void OnUpdateFront(CCmdUI* pCmdUI);
	afx_msg void OnLeftAlign();
	afx_msg void OnUpdateLeftAlign(CCmdUI* pCmdUI);
	afx_msg void OnMakeSameSize();
	afx_msg void OnUpdateMakeSameSize(CCmdUI* pCmdUI);
	afx_msg void OnRightAlign();
	afx_msg void OnUpdateRightAlign(CCmdUI* pCmdUI);
	afx_msg void OnTopAlign();
	afx_msg void OnUpdateTopAlign(CCmdUI* pCmdUI);
	afx_msg void OnUp();
	afx_msg void OnUpdateUp(CCmdUI* pCmdUI);
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	afx_msg void OnAddNone();
	afx_msg void OnUpdateAddNone(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddButton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddCheckbox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddCombobox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddGroupbox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddListbox(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddRadiobutton(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddStatic(CCmdUI* pCmdUI);
	afx_msg void OnButtonMultidraw();
	afx_msg void OnUpdateButtonMultidraw(CCmdUI* pCmdUI);

	afx_msg void OnAddImageStatic();
	afx_msg void OnAddListView();
	afx_msg void OnUpdateAddImageStatic(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddListView(CCmdUI* pCmdUI);

	afx_msg void OnAddOutLine();
	afx_msg void OnUpdateAddOutLine(CCmdUI* pCmdUI);

	afx_msg void OnAddChatEdit();
	afx_msg void OnUpdateAddChatEdit(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// --- DiagramEditor ---
private:

	// 다이얼로그 에디터 멤버 변수
	CDEditor	m_editor;

	int			m_screenResolutionX;	// Used for scaling printouts
	int			m_drawObject;			// What type of object we are drawing


	HBITMAP		m_bitmap;
public:
	afx_msg void OnDlgProperty();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnViewMagnetMode();
	afx_msg void OnUpdateViewMagnetMode(CCmdUI *pCmdUI);
	afx_msg void OnViewMagnetMode();
};

#ifndef _DEBUG  // DLGEditorSView.cpp의 디버그 버전
inline CDLGEditorSDoc* CDLGEditorSView::GetDocument() const
   { return reinterpret_cast<CDLGEditorSDoc*>(m_pDocument); }
#endif

