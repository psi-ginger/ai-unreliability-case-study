#pragma once


// MyRichEdit

#define default_char_size	9
#define default_char_color	RGB(0,0,0)

class MyRichEdit : public CRichEditCtrl
{
	DECLARE_DYNAMIC(MyRichEdit)

public:
	MyRichEdit();
	virtual ~MyRichEdit();

	void AddText( LPCTSTR msg, COLORREF color = default_char_color, int height = default_char_size );

	CRITICAL_SECTION		m_cs;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


