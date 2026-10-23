// ExeScript.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "StatisticsTool.h"
#include "ExeScript.h"
#include ".\exescript.h"


// CExeScript 대화 상자입니다.

IMPLEMENT_DYNAMIC(CExeScript, CDialog)
CExeScript::CExeScript(CWnd* pParent /*=NULL*/)
	: CDialog(CExeScript::IDD, pParent)
{
}

CExeScript::~CExeScript()
{
}

void CExeScript::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_edit_Input);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_edit_Output);
}


BEGIN_MESSAGE_MAP(CExeScript, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_EXECUTE, OnBnClickedButtonExecute)
	ON_WM_KEYUP()
END_MESSAGE_MAP()


// CExeScript 메시지 처리기입니다.

void CExeScript::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if( m_hWnd == NULL || m_edit_Input.m_hWnd == NULL || m_edit_Output.m_hWnd == NULL )		return;

	CRect	rc;		GetClientRect(rc);

	CRect	rc_input, rc_output;

	rc.top = 50;

	rc_input.top = rc.top;
	rc_input.left = 0;
	rc_input.right = rc.Width();
	rc_input.bottom = (LONG)(rc_input.top + rc.Height() * 0.7);

	rc_output.top = rc_input.bottom;
	rc_output.left = 0;
	rc_output.right = rc.Width();
	rc_output.bottom = (LONG)(rc_output.top + rc.Height() * 0.3);

	m_edit_Input.MoveWindow( rc_input );
	m_edit_Output.MoveWindow( rc_output );
}

void Lua_Fputs( void* pUserPT, const char* pOutput )
{
	if( pUserPT == NULL )		return;
	CExeScript*		pExeScript = (CExeScript*)pUserPT;

	CString strValue;
	strValue = pOutput;		strValue.Replace("\n", "\r\n");

	CString strOutput;
	pExeScript->GetDlgItemText( pExeScript->m_edit_Output.GetDlgCtrlID(), strOutput );
	strOutput += strValue;
	pExeScript->SetDlgItemText( pExeScript->m_edit_Output.GetDlgCtrlID(), strOutput );
}

BOOL CExeScript::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_clLua.RegistFputs( this, Lua_Fputs );

	m_clLua.DoFile("functions.lua");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CExeScript::OnBnClickedButtonExecute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strInput;
	GetDlgItemText(m_edit_Input.GetDlgCtrlID(), strInput);
	SetDlgItemText(m_edit_Output.GetDlgCtrlID(), "");
	if( m_clLua.DoString( strInput ) == false )
	{
		SetDlgItemText( m_edit_Output.GetDlgCtrlID(), m_clLua.ToString() );
	}
}

void CExeScript::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}
