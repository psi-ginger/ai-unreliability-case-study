// SelectPCXFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NConvert.h"
#include "SelectPCXFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectPCXFileDlg dialog


CSelectPCXFileDlg::CSelectPCXFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectPCXFileDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectPCXFileDlg)
	m_editComm = _T("");
	m_editFileName = _T("");
	m_editFolder = _T("");
	//}}AFX_DATA_INIT
}


void CSelectPCXFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectPCXFileDlg)
	DDX_Text(pDX, IDC_EDIT_COMM, m_editComm);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_editFileName);
	DDX_Text(pDX, IDC_EDIT_FOLDER, m_editFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectPCXFileDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectPCXFileDlg)
	ON_BN_CLICKED(IDC_BUTTON_FIND, OnButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectPCXFileDlg message handlers

void CSelectPCXFileDlg::OnButtonFind() 
{
	// TODO: Add your control notification handler code here
	
	char szFilters[] = "Pcx Files(*.pcx)|*.pcx|All Files(*.*)|*.*||";

	int j, len;	

	CString temp;

	CFileDialog fileDlg( TRUE, NULL, "*.pcx", OFN_FILEMUSTEXIST, szFilters, this);

	if( IDOK == fileDlg.DoModal() ) {

		temp = fileDlg.GetPathName();
		m_editFileName = fileDlg.GetFileName();

		len = temp.GetLength();

		for( j = len - 1; j >= 0; --j ) {
			if( temp[ j ] == '\\' ) {
				m_editFolder = temp.Left( j + 1 );
				break;
			}
		}
				
		UpdateData( FALSE );

	}
}

int CSelectPCXFileDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	char buf[ 128 ];

	sprintf( buf, "%s파일 변환에 필요한 팔레트 정보가 있는 PCX파일을 설정해주십시오.",
		m_strSrcFileName );
	m_editComm = buf;

	return CDialog::DoModal();
}

void CSelectPCXFileDlg::OnButtonOk() 
{
	// TODO: Add your control notification handler code here

	UpdateData( TRUE );

	EndDialog(IDOK);	
}
