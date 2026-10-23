#pragma once


// CSplitterWndViewToolBar 뷰입니다.

class CSplitterWndViewToolBar : public CView
{
	DECLARE_DYNCREATE(CSplitterWndViewToolBar)

protected:
	CSplitterWndViewToolBar();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSplitterWndViewToolBar();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


