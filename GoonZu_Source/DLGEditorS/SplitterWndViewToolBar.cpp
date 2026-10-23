// SplitterWndViewToolBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DLGEditorS.h"
#include "SplitterWndViewToolBar.h"


// CSplitterWndViewToolBar

IMPLEMENT_DYNCREATE(CSplitterWndViewToolBar, CView)

CSplitterWndViewToolBar::CSplitterWndViewToolBar()
{
}

CSplitterWndViewToolBar::~CSplitterWndViewToolBar()
{
}

BEGIN_MESSAGE_MAP(CSplitterWndViewToolBar, CView)
END_MESSAGE_MAP()


// CSplitterWndViewToolBar 그리기입니다.

void CSplitterWndViewToolBar::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CSplitterWndViewToolBar 진단입니다.

#ifdef _DEBUG
void CSplitterWndViewToolBar::AssertValid() const
{
	CView::AssertValid();
}

void CSplitterWndViewToolBar::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CSplitterWndViewToolBar 메시지 처리기입니다.
