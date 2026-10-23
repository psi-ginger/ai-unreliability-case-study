// StatisticsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "goonzumc.h"
#include "StatisticsDlg.h"
#include "StatisticsData.h"
#include "StatisticsGraph.h"

#include "../MasterServer/MasterMsg.h"

#include "MasterProcess.h"
#include "ITZSendBuffer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg dialog

CStatisticsDlg::CStatisticsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatisticsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatisticsDlg)
	//}}AFX_DATA_INIT

	m_bResponseData = FALSE;
	m_siResponseLayer = 0;

	m_pStatisticsGraph = NULL ;
	m_pStatisticsGraph = new CStatisticsGraph();

	InitializeCriticalSection( &m_CS );
}

BOOL CStatisticsDlg::OnInitDialog()
{
	


	// 리스트 컨트롤 초기화 
	SendDlgItemMessage( IDC_LIST_USERNUM, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES );

	int i;

	LVCOLUMN lvcol;

	int width[] = { 122, 130 };
	char *text[] = { "Time", "User Count(5min Diff)" };

	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;

	for( i = 0; i < 2; ++i ) {
		lvcol.cx = width[ i ];
		lvcol.pszText = text[ i ];
		lvcol.iSubItem = i;
		
		SendDlgItemMessage( IDC_LIST_USERNUM, LVM_INSERTCOLUMN, i, (LPARAM)&lvcol );
	}


	m_graphX = 295;
	m_graphY = 20;

	m_pStatisticsGraph->Init( 480, 350 );
	m_pStatisticsGraph->SetGraphProperty( 45, 330, 425, 320 );

	char buf[ 32 ];

	for( i = 2004; i <= 2010; ++i ) {
		itoa( i, buf, 10 );
		SendDlgItemMessage( IDC_COMBO_YEAR1, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_YEAR2, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_YEAR3, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_YEAR4, CB_ADDSTRING, 0, (LPARAM)buf );
	}

	for( i = 1; i <= 12; ++i ) {
		itoa( i, buf, 10 );
		SendDlgItemMessage( IDC_COMBO_MONTH1, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_MONTH2, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_MONTH3, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_MONTH4, CB_ADDSTRING, 0, (LPARAM)buf );
	}

	for( i = 1; i <= 31; ++i ) {
		itoa( i, buf, 10 );
		SendDlgItemMessage( IDC_COMBO_DAY1, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_DAY2, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_DAY3, CB_ADDSTRING, 0, (LPARAM)buf );
		SendDlgItemMessage( IDC_COMBO_DAY4, CB_ADDSTRING, 0, (LPARAM)buf );
	}

	SYSTEMTIME systime;
	GetLocalTime( &systime );

	char year[ 10 ], month[ 10 ], day[ 10 ];
	itoa( systime.wYear, year, 10 );
	itoa( systime.wMonth, month, 10 );
	itoa( systime.wDay, day, 10 );

	SendDlgItemMessage( IDC_COMBO_YEAR1, CB_SELECTSTRING, -1, (LPARAM)year );
	SendDlgItemMessage( IDC_COMBO_YEAR2, CB_SELECTSTRING, -1, (LPARAM)year );
	SendDlgItemMessage( IDC_COMBO_YEAR3, CB_SELECTSTRING, -1, (LPARAM)year );
	SendDlgItemMessage( IDC_COMBO_YEAR4, CB_SELECTSTRING, -1, (LPARAM)year );

	SendDlgItemMessage( IDC_COMBO_MONTH1, CB_SELECTSTRING, -1, (LPARAM)month );
	SendDlgItemMessage( IDC_COMBO_MONTH2, CB_SELECTSTRING, -1, (LPARAM)month );
	SendDlgItemMessage( IDC_COMBO_MONTH3, CB_SELECTSTRING, -1, (LPARAM)month );
	SendDlgItemMessage( IDC_COMBO_MONTH4, CB_SELECTSTRING, -1, (LPARAM)month );

	SendDlgItemMessage( IDC_COMBO_DAY1, CB_SELECTSTRING, -1, (LPARAM)day );
	SendDlgItemMessage( IDC_COMBO_DAY2, CB_SELECTSTRING, -1, (LPARAM)day );
	SendDlgItemMessage( IDC_COMBO_DAY3, CB_SELECTSTRING, -1, (LPARAM)day );
	SendDlgItemMessage( IDC_COMBO_DAY4, CB_SELECTSTRING, -1, (LPARAM)day );

	SetTimer( 77, 50, NULL );

	return TRUE ;
}

void CStatisticsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatisticsDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialog)
	//{{AFX_MSG_MAP(CStatisticsDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR1, OnButtonClear1)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR2, OnButtonClear2)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR3, OnButtonClear3)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR4, OnButtonClear4)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST1, OnButtonRequest1)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST2, OnButtonRequest2)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST3, OnButtonRequest3)
	ON_BN_CLICKED(IDC_BUTTON_REQUEST4, OnButtonRequest4)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON_1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON_3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON_4, OnButton4)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatisticsDlg message handlers

void CStatisticsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
	m_pStatisticsGraph->Draw();
	m_pStatisticsGraph->Put( m_graphX, m_graphY, &dc);

}
 

void CStatisticsDlg::OnDestroy() 
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here

	KillTimer( 77 );

	if ( m_pStatisticsGraph ) delete m_pStatisticsGraph;
	m_pStatisticsGraph = NULL ;
}

void CStatisticsDlg::Clear( int layer )
{

	if( layer < 0 || layer >= MAX_LAYER_NUMBER ) return;

	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();

	if( pApp->m_pStatisticsData ) {

		pApp->m_pStatisticsData->Clear( layer );

		DrawGraph();
	}
}

void CStatisticsDlg::Request( int year, int month, int day, int layer )
{

	if( layer < 0 || layer >= MAX_LAYER_NUMBER ) return;

	sRequest_GMCDAYUSERNUM sendMsg( year, month, day, layer );
	
	CGoonZuMCApp *pApp = (CGoonZuMCApp *)AfxGetApp();
	pApp->m_pMasterProcess->m_pMasterSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );
}


void CStatisticsDlg::OnButtonClear1() 
{
	// TODO: Add your control notification handler code here
	Clear( 0 );
	
}

void CStatisticsDlg::OnButtonClear2() 
{
	// TODO: Add your control notification handler code here
	Clear( 1 );

}

void CStatisticsDlg::OnButtonClear3() 
{
	// TODO: Add your control notification handler code here
	Clear( 2 );
}

void CStatisticsDlg::OnButtonClear4() 
{
	// TODO: Add your control notification handler code here
	Clear( 3 );
	
}

void CStatisticsDlg::OnButtonRequest1() 
{
	// TODO: Add your control notification handler code here
	
	int year, month, day;
	char buf[ 32 ];

	GetDlgItemText( IDC_COMBO_YEAR1, buf, 32 );
	year = atoi( buf );

	GetDlgItemText( IDC_COMBO_MONTH1, buf, 32 );
	month = atoi( buf );

	GetDlgItemText( IDC_COMBO_DAY1, buf, 32 );
	day = atoi( buf );

	Request( year, month, day, 0 );
}

void CStatisticsDlg::OnButtonRequest2() 
{
	// TODO: Add your control notification handler code here

	int year, month, day;
	char buf[ 32 ];

	GetDlgItemText( IDC_COMBO_YEAR2, buf, 32 );
	year = atoi( buf );

	GetDlgItemText( IDC_COMBO_MONTH2, buf, 32 );
	month = atoi( buf );

	GetDlgItemText( IDC_COMBO_DAY2, buf, 32 );
	day = atoi( buf );

	Request( year, month, day, 1 );
	
}

void CStatisticsDlg::OnButtonRequest3() 
{
	// TODO: Add your control notification handler code here

	int year, month, day;
	char buf[ 32 ];

	GetDlgItemText( IDC_COMBO_YEAR3, buf, 32 );
	year = atoi( buf );

	GetDlgItemText( IDC_COMBO_MONTH3, buf, 32 );
	month = atoi( buf );

	GetDlgItemText( IDC_COMBO_DAY3, buf, 32 );
	day = atoi( buf );

	Request( year, month, day, 2 );
	
}

void CStatisticsDlg::OnButtonRequest4() 
{
	// TODO: Add your control notification handler code here
	
	int year, month, day;
	char buf[ 32 ];

	GetDlgItemText( IDC_COMBO_YEAR4, buf, 32 );
	year = atoi( buf );

	GetDlgItemText( IDC_COMBO_MONTH4, buf, 32 );
	month = atoi( buf );

	GetDlgItemText( IDC_COMBO_DAY4, buf, 32 );
	day = atoi( buf );

	Request( year, month, day, 3 );

}

void CStatisticsDlg::DrawGraph()
{
	CClientDC dc(this);

	m_pStatisticsGraph->Draw();
	m_pStatisticsGraph->Put( m_graphX, m_graphY, &dc);
}

void CStatisticsDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);

	m_pStatisticsGraph->PutEx( m_graphX, m_graphY, &dc, point.x - m_graphX, point.y - m_graphY );

	CDialog::OnMouseMove(nFlags, point);
}


void CStatisticsDlg::ShowUserNum( int layer )
{
	
	char buf[ 64 ];

	SendDlgItemMessage( IDC_LIST_USERNUM, LVM_DELETEALLITEMS, 0, 0 );	
	
	CStatisticsData *pData = NULL;

	pData = ((CGoonZuMCApp *)AfxGetApp())->m_pStatisticsData;

	if( pData == NULL ) return;
	if( pData->m_bVisible[ layer ] == FALSE ) return;

	LVITEM lvi;
	lvi.mask = LVIF_TEXT;		
	
	pData->CriticalEnter();

	int item = 0;

	for ( int i = 0 ; i < MAX_DATA_NUMBER; i++ )
	{
		lvi.iItem = item++;
		
		lvi.iSubItem = 0;

		sprintf( buf, "%s %2d:%2d", pData->m_szDate[ layer ], i / 12, ( i % 12 ) * 5 );

		lvi.pszText = buf;
		
		SendDlgItemMessage( IDC_LIST_USERNUM, LVM_INSERTITEM, 0, (LPARAM)&lvi );


		lvi.iSubItem = 1;

		if( i > 0 ) {
			sprintf( buf, "%d (%d)", pData->m_siUserNum[ layer ][ i ],
				pData->m_siUserNum[ layer ][ i ] - pData->m_siUserNum[ layer ][ i - 1 ] );
		} else {
			itoa( pData->m_siUserNum[ layer ][ i ], buf, 10 );
		}

		lvi.pszText = buf;

		SendDlgItemMessage( IDC_LIST_USERNUM, LVM_SETITEM, 0, (LPARAM)&lvi );


	}

	pData->CritcalLeave();


}

void CStatisticsDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	ShowUserNum( 0 );
}

void CStatisticsDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	ShowUserNum( 1 );
}

void CStatisticsDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here
	ShowUserNum( 2 );
}

void CStatisticsDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	ShowUserNum( 3 );
}

void CStatisticsDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if( m_bResponseData == TRUE ) {

		m_bResponseData = FALSE;

		DrawGraph();

		ShowUserNum( m_siResponseLayer );
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CStatisticsDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == 13 ) {
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}
