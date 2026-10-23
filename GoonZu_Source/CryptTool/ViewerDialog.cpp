//=============================================================================
//
//	ViewerDialog.cpp
//
//	Copyright 2005 HQGame
//
//=============================================================================

#include "stdafx.h"
#include "CryptTool.h"
#include "ViewerDialog.h"

#include "D:/ProjectB/Source/HQCore/HQCorePrivate.h"

//=============================================================================
// CViewerDialog 대화 상자입니다.
//=============================================================================
IMPLEMENT_DYNAMIC(CViewerDialog, CDialog)

//=============================================================================
//  @Name   CViewerDialog()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
CViewerDialog::CViewerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CViewerDialog::IDD, pParent)
	, m_strViewer(_T(""))
{
}

//=============================================================================
//  @Name   ~CViewerDialog()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
CViewerDialog::~CViewerDialog()
{
}

//=============================================================================
//  @Name   DoDataExchange()
//! @author LeeKH						
//! @date   2005-12-27
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void CViewerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VIEW, m_strViewer);
	DDX_Control(pDX, IDC_VIEW, m_edtViewer);
}


BEGIN_MESSAGE_MAP(CViewerDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()

//=============================================================================
//  @Name   OnSize()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  다이얼로그 크기가 바뀌면 에디트박스 크기 변경
//=============================================================================
void CViewerDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (m_edtViewer.m_hWnd != NULL)
	{
		// 다이얼로그에 에디트 박스를 꽉 채운다.
		m_edtViewer.MoveWindow(0, 0, cx, cy);
	}
}

//=============================================================================
//  @Name   OnInitDialog()
//! @author LeeKH						
//! @date   2005-12-26
//! @param  X   X
//! @return X
//! @brief  초기화 작업
//=============================================================================
BOOL CViewerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CFileDialog	filedlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"CryptText Files (*.txt) |*.txt||", NULL);

	// 파일 선택창 실행
	if (filedlg.DoModal() == IDOK)
	{
		// 에디트 컨트롤의 내용 삭제
		m_strViewer = "";
//나중에.
/*
		HQCryptTextFileUA crypttxtUA;
		// 암호화 된 Text파일 열기
		if (crypttxtUA.OpenReadable(filedlg.GetPathName()))
		{
			// 파일명을 윈도우 타이틀로 설정
			this->SetWindowText(filedlg.GetFileName());
			// 한 줄씩 읽어서 에디트 컨트롤에 추가
			while(crypttxtUA.ReadLine())
			{
				m_strViewer += crypttxtUA.GetBuffer();
				m_strViewer += "\r\n";
			}
			UpdateData(FALSE);
			DWORD dwSel = 0;
			dwSel = HIWORD(0);
			dwSel = LOWORD(-1);
			m_edtViewer.SetSel(dwSel);
		}
*/
	}
	else
	{
		OnCancel();
	}	

	return TRUE;
}

// CViewerDialog 메시지 처리기입니다.
