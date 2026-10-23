// FieldObjectMergeDlg.h : 헤더 파일
//

#pragma once


class CFieldObject
{
public:
	CString		m_strName;
	CString		m_ZipName;
};

// CFieldObjectMergeDlg 대화 상자
class CFieldObjectMergeDlg : public CDialog
{
// 생성
public:
	CFieldObjectMergeDlg(CWnd* pParent = NULL);	// 표준 생성자
	~CFieldObjectMergeDlg();

// 대화 상자 데이터
	enum { IDD = IDD_FIELDOBJECTMERGE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	CPtrArray		m_arrFieldObject;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExecute();
};
