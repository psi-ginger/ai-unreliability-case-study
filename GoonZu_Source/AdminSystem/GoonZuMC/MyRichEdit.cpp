// MyRichEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MyRichEdit.h"


// MyRichEdit

IMPLEMENT_DYNAMIC(MyRichEdit, CRichEditCtrl)
MyRichEdit::MyRichEdit()
{
	InitializeCriticalSection( &m_cs );	
}

MyRichEdit::~MyRichEdit()
{
	DeleteCriticalSection( &m_cs );
}


BEGIN_MESSAGE_MAP(MyRichEdit, CRichEditCtrl)
	ON_WM_CREATE()
END_MESSAGE_MAP()



// MyRichEdit 메시지 처리기입니다.


int MyRichEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	int nSize = default_char_size * 20;			// atoi for tchar
	CHARFORMAT cf;

	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_FACE | CFM_COLOR | CFM_ITALIC | CFM_BOLD | CFM_SIZE;
	cf.dwEffects	=(unsigned long) ~( CFE_AUTOCOLOR| CFE_BOLD|CFE_ITALIC);
	//cf.dwEffects|= CFE_BOLD;
	//cf.dwEffects|= CFE_ITALIC;
	cf.yHeight		= nSize;
	cf.crTextColor	= default_char_color;
	strcpy(cf.szFaceName, "굴림"); 

	SetWordCharFormat(cf);

	return 0;
}

void MyRichEdit::AddText( LPCTSTR msg, COLORREF color, int height )
{

	EnterCriticalSection( &m_cs );

	int iTotalTextLength = GetWindowTextLength();
	int iStartPos = iTotalTextLength;

	// Get size in Twips
	int nSize = height * 20;			// atoi for tchar

	CHARFORMAT cf;
	cf.cbSize		= sizeof(CHARFORMAT);
	cf.dwMask		= CFM_FACE | CFM_COLOR | CFM_ITALIC | CFM_BOLD | CFM_SIZE;
	cf.dwEffects	=(unsigned long) ~( CFE_AUTOCOLOR| CFE_BOLD|CFE_ITALIC);
	//cf.dwEffects|= CFE_BOLD;
	//cf.dwEffects|= CFE_ITALIC;
	cf.yHeight		= nSize;
	cf.crTextColor	= color;
	strcpy(cf.szFaceName, "굴림"); 

	SetWordCharFormat(cf);
	
	SetSel(iTotalTextLength, iTotalTextLength);
	ReplaceSel(msg);
	LineScroll(2);

	LeaveCriticalSection( &m_cs );

}
