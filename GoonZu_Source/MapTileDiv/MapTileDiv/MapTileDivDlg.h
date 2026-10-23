// MapTileDivDlg.h : 헤더 파일
//

#pragma once

#include "directives.h"


// CMapTileDivDlg 대화 상자
class CMapTileDivDlg : public CDialog
{
// 생성
public:
	CMapTileDivDlg(CWnd* pParent = NULL);	// 표준 생성자

	int EncodeZpr( UI08 *pDestImage, UI08 *pSrcImage, int width, int height );

	void LoadSpr( char *filename, unsigned char **pImg );
	void LoadPCX( char *filename );

	unsigned short m_pal565[ 256 ];
	unsigned short m_pal555[ 256 ];
	unsigned char m_pcxpal[ 768 ];

	unsigned char *m_image;
	int			  m_imagewidth;
	int			  m_imageheight;
	
	unsigned char *m_mask;




// 대화 상자 데이터
	enum { IDD = IDD_MAPTILEDIV_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
