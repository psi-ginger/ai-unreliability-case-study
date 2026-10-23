#include "stdafx.h"
//#include "DialogEditorDemo.h"
#include "EditPropertyDlg.h"
#include ".\editpropertydlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditPropertyDlg dialog


CEditPropertyDlg::CEditPropertyDlg(CWnd* pParent /*=NULL*/)
	: CDiagramPropertyDlg(CEditPropertyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditPropertyDlg)
	//}}AFX_DATA_INIT
}


void CEditPropertyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditPropertyDlg)
	DDX_Control(pDX, IDC_EDIT_ID, m_editName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditPropertyDlg, CDialog)
	//{{AFX_MSG_MAP(CEditPropertyDlg)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	//}}AFX_MSG_MAP
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditPropertyDlg message handlers

void CEditPropertyDlg::OnButtonApply() 
{

	if( m_hWnd )
	{
		if( GetEntity() )
		{
			CString name;
			m_editName.GetWindowText( name );

			GetEntity()->SetName( name );
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// CEditPropertyDlg virtuals

void CEditPropertyDlg::SetValues()
{

	if( m_hWnd )
		if( GetEntity() )
			m_editName.SetWindowText( GetEntity()->GetName() );

}


void CEditPropertyDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDiagramPropertyDlg::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
