// DialogSettings.cpp : implementation file
//

#include "stdafx.h"
#include "DialogSettings.h"
#include ".\dialogsettings.h"
#include "StdGrfx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings dialog


CDialogSettings::CDialogSettings(CWnd* pParent /*=NULL*/)
: CDialog(CDialogSettings::IDD, pParent) //: CDiagramButton()//CDialogSettings(CDialogSettings::IDD, pParent)//: CDialog(CDialogSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSettings)
	//m_height = 0;
	//m_width = 0;
	m_gridHeight = 0;
	m_gridWidth = 0;
	m_marginLeft = 0;
	m_marginTop = 0;
	m_marginRight = 0;
	m_marginBottom = 0;
	
	m_defaultImagePath = GOONZU_DEFAULT_ROOT_PATH;
	m_defaultTxtPath = GOONZU_DEFAULT_TEXTMGR_PATH;
	//}}AFX_DATA_INIT
	
}

CDialogSettings::~CDialogSettings()
{
}

void CDialogSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSettings)
	// GRID PROPERTY
	DDX_Text(pDX, IDC_EDIT_GRID_HEIGHT, m_gridHeight);
	DDX_Text(pDX, IDC_EDIT_GRID_WIDTH, m_gridWidth);

	// MARGIN PROPERTY
	DDX_Text(pDX, IDC_EDIT_LEFT, m_marginLeft);
	DDX_Text(pDX, IDC_EDIT_TOP, m_marginTop);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_marginRight);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_marginBottom);

	// 디폴트 루트 경로
	// leon add
	DDX_Text(pDX, IDC_EDIT_DEFAULT_PATH, m_defaultImagePath);
	DDX_Text(pDX, IDC_EDIT_DEFAULT_TEXTMGR_PATH, m_defaultTxtPath);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSettings, CDialog)
	//{{AFX_MSG_MAP(CDialogSettings)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings message handlers
