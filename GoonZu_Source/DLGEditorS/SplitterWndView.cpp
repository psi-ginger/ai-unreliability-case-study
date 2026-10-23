// SplitterWndView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DLGEditorS.h"
#include "SplitterWndView.h"


// CSplitterWndView

IMPLEMENT_DYNCREATE(CSplitterWndView, CFormView)

CSplitterWndView::CSplitterWndView()
	: CFormView(CSplitterWndView::IDD)
{
}

CSplitterWndView::~CSplitterWndView()
{
}

void CSplitterWndView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSplitterWndView, CFormView)
END_MESSAGE_MAP()


// CSplitterWndView 진단입니다.

#ifdef _DEBUG
void CSplitterWndView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSplitterWndView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CSplitterWndView 메시지 처리기입니다.
