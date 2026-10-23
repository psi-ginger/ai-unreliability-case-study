#pragma once


// CNoticeVirw 대화 상자입니다.

class CNoticeVirw : public CDialog
{
	DECLARE_DYNAMIC(CNoticeVirw)

public:
	CNoticeVirw(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNoticeVirw();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NOTICEVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
