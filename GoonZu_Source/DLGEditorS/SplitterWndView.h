#pragma once


#include "resource.h"
// CSplitterWndView 폼 뷰입니다.

class CSplitterWndView : public CFormView
{
	DECLARE_DYNCREATE(CSplitterWndView)

protected:
	CSplitterWndView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSplitterWndView();

public:
	enum { IDD = IDD_SPLITTERWNDVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


