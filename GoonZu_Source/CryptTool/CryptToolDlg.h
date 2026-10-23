//=============================================================================
//
//	CryptToolDlg.h
//
//	Copyright 2005 HQGame
//
//=============================================================================
#ifndef _CRYPTTOOLDLG_H_
#define _CRYPTTOOLDLG_H_

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "D:/ProjectB/Source/HQCore/HQCorePrivate.h"	// HQGame의 메인 Include

//=============================================================================
//	CLASS   CCryptToolDlg
//! @brief  CCryptToolDlg 대화 상자
//=============================================================================
class CCryptToolDlg : public CDialog
{
	// 생성
public:
	CCryptToolDlg(CWnd* pParent = NULL);	// 표준 생성자
	~CCryptToolDlg()						// 소멸자
	{
		m_imgIcons.DeleteImageList();
	}

	//=============================================================================
	// 대화 상자 데이터
	//=============================================================================
	enum { IDD = IDD_CRYPTTOOL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

protected:
	HICON m_hIcon;

	//=============================================================================
	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	//=============================================================================

public:
	//=============================================================================
	// 컨트롤 변수
	//=============================================================================
	CListCtrl m_lstResult;
	CListBox m_lstFolders;

	//=============================================================================
	// 텍스트 컨트롤과 연결된 변수
	//=============================================================================
	CString m_wKeyR;
	CString m_wKeyC1;
	CString m_wKeyC2;

	//=============================================================================
	// 체크박스 컨트롤과 연결된 변수
	//=============================================================================
	BOOL m_bDelText;

	//=============================================================================
	// 일반 변수
	//=============================================================================
	CImageList	m_imgIcons;

	//=============================================================================
	// 이벤트 함수들
	//=============================================================================
	afx_msg void OnBnClickedBtnprocess();
	afx_msg void OnBnClickedBtnfindfolder();
	afx_msg void OnBnClickedBtnview();
	afx_msg void OnBnClickedBtnadd();
	afx_msg void OnBnClickedBtndel();
	afx_msg void OnBnClickedBtndecode();
	afx_msg void OnBnClickedBtndecodeprocess();
};

#endif

