// NConvertDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NConvert.h"
#include "NConvertDlg.h"
#include "common.h"
#include "SelectPCXFileDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNConvertDlg dialog

CNConvertDlg::CNConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNConvertDlg)
	m_edit = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNConvertDlg)
	DDX_Text(pDX, IDC_EDIT1, m_edit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNConvertDlg, CDialog)
	//{{AFX_MSG_MAP(CNConvertDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_PCXTRANS, OnButtonPcxtrans)
	ON_BN_CLICKED(IDC_BUTTON_CONVSPR, OnButtonConvspr)
	ON_BN_CLICKED(IDC_BUTTON_CONVMPR, OnButtonConvmpr)
	ON_BN_CLICKED(IDC_BUTTON_MAKEALPHAPCX, OnButtonMakealphapcx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNConvertDlg message handlers

BOOL CNConvertDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_edit = "새로운 파일 형식(16비트로컬팔레트)으로 바꾸는 과정\r\n\r\n";
	m_edit += "1. 원본 파일 PCX를 제작한다\r\n\r\n";
	m_edit += "2. PCX파일 팔레트 투명색 조정을 한다\r\n\r\n";
	m_edit += "3. 스프라이트 툴에서 PCX를 SPR이나 MPR로 변환한다\r\n";
	m_edit += "(이때 로컬팔레트이기 때문에 스프라이트 툴에서는 깨져서 보일수 있다)\r\n\r\n";
	m_edit += "4. 스프라이트 툴에서 변환한 SPR이나 MPR을 새로운 형식의 파일로 변환한다\r\n";
	m_edit += "(이때 로컬팔레트 정보를 쓰기위해서 PCX파일이 필요하므로 투명색 조정된 PCX파일을 보존해 두어야한다)\r\n";

	UpdateData( FALSE );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNConvertDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNConvertDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNConvertDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNConvertDlg::OnButtonPcxtrans() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	
	char buffer[ 128 ];
	
	char szFilters[] = "Pcx Files(*.pcx)|(*.pcx)|All Files(*.*)|*.*||";

	int j, len;
	POSITION pos;

	CString strNextPathName;

	char filenamebuffer[ 102400 ];

	strcpy( filenamebuffer, "*.pcx" );

	CFileDialog fileDlg( TRUE, NULL, filenamebuffer, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, szFilters, this);

	fileDlg.m_ofn.nMaxFile = 102400;
	fileDlg.m_ofn.lpstrFile = filenamebuffer;


	if( IDOK == fileDlg.DoModal() ) {

		int total = 0;
		int complete = 0;

		if( (pos = fileDlg.GetStartPosition()) != NULL ) {

			while( pos ) {
				strNextPathName = fileDlg.GetNextPathName( pos );
				
				len = strNextPathName.GetLength();

				for( j = len - 1; j >= 0; --j ) {
					if( strNextPathName[ j ] == '\\' ) {
						m_strFileName = strNextPathName.Right( len - j - 1 );
						m_strPathName = strNextPathName.Left( j + 1 );
						break;
					}
				}

				if( PcxTrans() == FALSE ) {
					sprintf( buffer, "%s파일 팔레트 조정 실패", m_strFileName );
					MessageBox( buffer );
				} else {
					++complete;
				}

				++total;
			}
		}

		sprintf( buffer, "%d개중 %d개의 파일 변환을 완료하였습니다", total, complete );
		MessageBox( buffer );

	}
	
}

void CNConvertDlg::OnButtonConvspr() 
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	char buffer[ 128 ];
	
	char szFilters[] = "Spr Files(*.spr)|*.spr|All Files(*.*)|*.*||";

	int j, len;
	POSITION pos;

	CString strNextPathName;
	
	char filenamebuffer[ 102400 ];

	strcpy( filenamebuffer, "*.spr" );

	CFileDialog fileDlg( TRUE, NULL, filenamebuffer, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, szFilters, this);

	fileDlg.m_ofn.nMaxFile = 102400;
	fileDlg.m_ofn.lpstrFile = filenamebuffer;

	if( IDOK == fileDlg.DoModal() ) {

		BOOL bSuccess = TRUE;

		if( (pos = fileDlg.GetStartPosition()) != NULL ) {

			while( pos ) {
				strNextPathName = fileDlg.GetNextPathName( pos );
				
				len = strNextPathName.GetLength();

				for( j = len - 1; j >= 0; --j ) {
					if( strNextPathName[ j ] == '\\' ) {
						m_strFileName = strNextPathName.Right( len - j - 1 );
						m_strPathName = strNextPathName.Left( j + 1 );
						break;
					}
				}

				if( ConvSpr() == FALSE ) {
					bSuccess = FALSE;
					sprintf( buffer, "%s파일 변환 실패 ", m_strFileName );
					MessageBox( buffer );
				}

			}

		}

		if( bSuccess ) {
			MessageBox( "파일 변환을 완료 하였습니다" );
		}


	}

	
}

void CNConvertDlg::OnButtonConvmpr() 
{
	// TODO: Add your control notification handler code here

	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	char buffer[ 128 ];

	char szFilters[] = "Mpr Files(*.mpr)|*.mpr|All Files(*.*)|*.*||";

	int j, len;
	POSITION pos;

	CString strNextPathName;	

	char filenamebuffer[ 102400 ];

	strcpy( filenamebuffer, "*.mpr" );

	CFileDialog fileDlg( TRUE, NULL, filenamebuffer, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST, szFilters, this);

	fileDlg.m_ofn.nMaxFile = 102400;
	fileDlg.m_ofn.lpstrFile = filenamebuffer;

	if( IDOK == fileDlg.DoModal() ) {

		BOOL bSuccess = TRUE;

		if( (pos = fileDlg.GetStartPosition()) != NULL ) {

			while( pos ) {
				strNextPathName = fileDlg.GetNextPathName( pos );
				
				len = strNextPathName.GetLength();

				for( j = len - 1; j >= 0; --j ) {
					if( strNextPathName[ j ] == '\\' ) {
						m_strFileName = strNextPathName.Right( len - j - 1 );
						m_strPathName = strNextPathName.Left( j + 1 );
						break;
					}
				}

				if( ConvMpr() == FALSE ) {
					bSuccess = FALSE;
					sprintf( buffer, "%s파일 변환 실패 ", m_strFileName );
					MessageBox( buffer );
				}

			}
		}

		if( bSuccess ) {
			MessageBox( "파일 변환을 완료 하였습니다" );
		}

	}
	

}


BOOL CNConvertDlg::PcxTrans()
{
	char message[ 128 ];

	unsigned char *buffer;
	unsigned char *pImage;
	unsigned char *canvas;

	char filename[ 128 ];

	PCX pcx;

	sprintf( filename, "%s%s", m_strPathName, m_strFileName );
	
	HANDLE hFile = CreateFile( filename, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD dwFileSize;

	dwFileSize = GetFileSize( hFile, NULL );

	if( dwFileSize == INVALID_FILE_SIZE ||
		dwFileSize == 0 ) {

		CloseHandle( hFile );
		return FALSE;
	}


	buffer = new unsigned char[ dwFileSize ];

	DWORD dwRead;
	if( ReadFile( hFile, buffer, dwFileSize, &dwRead, NULL ) == FALSE ) {
		delete [] buffer;
		CloseHandle( hFile );
		return FALSE;
	}

	if( dwRead != dwFileSize ) {
		delete [] buffer;
		CloseHandle( hFile );
		return FALSE;
	}
	
	CloseHandle( hFile );

	memcpy( &pcx.Head, buffer, sizeof( _head ) );

	pImage = buffer + sizeof( _head );

	int xsize, ysize;

	xsize = pcx.Head.xmax + 1;
	ysize = pcx.Head.ymax + 1;

	canvas = new unsigned char[ xsize * ysize ];

	int i, j, k, count;

	for( i = 0; i < ysize; ++i ) {

		j = 0;

		while( j < xsize ) {
	
			if( *pImage > 191 ) {
			
				count = *pImage - 192;
				++pImage;

				for( k = 0; k < count; ++k ) {

					canvas[ i * xsize + j ] = *pImage;
					++j;
				}

				++pImage;
			} else {

				canvas[ i * xsize + j ] = *pImage;
				++pImage;
				++j;
			}

		}

	}

	memcpy( &pcx.Tail, pImage, sizeof( _tail ) );
	pImage += sizeof( _tail );

	if( dwFileSize != (DWORD)pImage - (DWORD)buffer ) {
		delete [] canvas;
		delete [] buffer;
		return FALSE;
	}

	// 투명 칼라 바꾸기 
	
	PCXPAL palTrans;
	unsigned char ci;


	palTrans.r = 240;
	palTrans.g = 0;
	palTrans.b = 255;

	// 투명색이 제 위치에 있으면,
	if( pcx.Tail.palette[ 254 * 3 ] == palTrans.r &&
		pcx.Tail.palette[ 254 * 3 + 1 ] == palTrans.g &&
		pcx.Tail.palette[ 254 * 3 + 2 ] == palTrans.b ) {
		delete [] canvas;
		delete [] buffer;
		return TRUE;
	}

	// 투명색 위치 찾기 
	for( i = 0; i < 256; ++i ) {
		if( pcx.Tail.palette[ i * 3 ] == palTrans.r &&
			pcx.Tail.palette[ i * 3 + 1 ] == palTrans.g &&
			pcx.Tail.palette[ i * 3 + 2 ] == palTrans.b ) {

			ci = i;
			break;
		}
	}

	if( i == 256 ) {

		sprintf( message, "%s 파일 투명색이 없습니다. 투명색RGB(240,0,255)", m_strFileName );
		MessageBox( message );

		delete [] canvas;
		delete [] buffer;
		return FALSE;
	}

	// 색상 바꾸기 
	for( i = 0; i < ysize; ++i ) {
		for( j = 0; j < xsize; ++j ) {
			if( canvas[ i * xsize + j ] == ci ) {
				canvas[ i * xsize + j ] = 254;
			} else if( canvas[ i * xsize + j ] == 254 ) {
				canvas[ i * xsize + j ] = ci;
			}
		}
	}

	// 팔레트 바꾸기 
	pcx.Tail.palette[ ci * 3 ] = pcx.Tail.palette[ 254 * 3 ];
	pcx.Tail.palette[ ci * 3 + 1] = pcx.Tail.palette[ 254 * 3 + 1 ];
	pcx.Tail.palette[ ci * 3 + 2] = pcx.Tail.palette[ 254 * 3 + 2 ];

	pcx.Tail.palette[ 254 * 3 ] = palTrans.r;
	pcx.Tail.palette[ 254 * 3 + 1] = palTrans.g;
	pcx.Tail.palette[ 254 * 3 + 2] = palTrans.b;


	// pcx 저장하기 

	hFile = CreateFile( filename, 
						GENERIC_WRITE, 
						NULL, 
						NULL, 
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD dwWrite;

	if( WriteFile( hFile, &pcx.Head, sizeof( _head ), &dwWrite, NULL ) == FALSE ) {
		CloseHandle( hFile );

		delete [] canvas;
		delete [] buffer;

		return FALSE;
	}

	count = 0;

	unsigned char da1, color;
	int color_switch=FALSE;
	int write_switch=FALSE;

	for(i=0;i<ysize;i++)
	for(j=0;j<xsize;j++)
	{
	 da1=canvas[j+i*xsize];

	 if(color_switch==FALSE)
	 {
		color=da1;
		count=1;
		color_switch=TRUE;
		if(j==(xsize-1))
		{
		  pcxrecord( hFile, count, color );
		  color_switch=FALSE;
		}
	 }
	 else if(color==da1)
	 {
		count++;
		if(j==(xsize-1)){
		  pcxrecord( hFile, count, color );
		  color_switch=FALSE;
		}
		else if(count>=63){
		  pcxrecord( hFile, count, color );
		  color_switch=FALSE;
		}
	 }
	 else 
	 {
	   pcxrecord( hFile, count, color );
	   color=da1;
	   count=1;
	   color_switch=TRUE;
	   if(j==(xsize-1))
	   {
		  pcxrecord( hFile, count, color );
		  color_switch=FALSE;
		}
	 }
	}

	if( WriteFile( hFile, &pcx.Tail, sizeof( _tail ), &dwWrite, NULL ) == FALSE ) {
		CloseHandle( hFile );

		delete [] canvas;
		delete [] buffer;

		return FALSE;
	}

	CloseHandle( hFile );

	delete [] canvas;
	delete [] buffer;

	return TRUE;

}

void CNConvertDlg::pcxrecord( HANDLE hFile, int count, unsigned char color )
{
	DWORD dwWrite;
	unsigned char ch[ 2 ];
	
	if( count > 1 || color >= 192 )
	{
		ch[ 0 ] = 192 + count;
		ch[ 1 ] = color;

		WriteFile( hFile, ch, 2, &dwWrite, NULL );

	} else {
		ch[ 0 ] = color;
		WriteFile( hFile, ch, 1, &dwWrite, NULL );
	}
}

BOOL CNConvertDlg::ConvMpr()
{
	int i, len;
	
	char buf[ 128 ];

	char filename[ 128 ];
	char pcxfilename[ 128 ];
	unsigned char *buffer;

	CString strfilenamePre;
	CString strfilenamePost;


	// MPR파일 로딩 
	sprintf( filename, "%s%s", m_strPathName, m_strFileName );
	
	HANDLE hFile = CreateFile( filename, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD dwFileSize;

	dwFileSize = GetFileSize( hFile, NULL );

	if( dwFileSize == INVALID_FILE_SIZE ||
		dwFileSize == 0 ) {

		CloseHandle( hFile );
		return FALSE;
	}

	buffer = new unsigned char[ dwFileSize ];

	DWORD dwRead;
	if( ReadFile( hFile, buffer, dwFileSize, &dwRead, NULL ) == FALSE ) {
		delete [] buffer;
		CloseHandle( hFile );
		return FALSE;
	}

	if( dwRead != dwFileSize ) {
		delete [] buffer;
		CloseHandle( hFile );
		return FALSE;
	}
	
	CloseHandle( hFile );


	len = m_strFileName.GetLength();

	for( i = len - 1; i >= 0; --i ) {

		if( m_strFileName[ i ] == '.' ) {
			strfilenamePre = m_strFileName.Left( i );
			strfilenamePost = m_strFileName.Right( len - i - 1 );
			break;
		}
	}

	if( strfilenamePre.IsEmpty() ) {
		return FALSE;
	}

	cltTSprHeader *pHeader = (cltTSprHeader *)buffer;

	// 이미 변환된 파일이면,
	if( pHeader->siVersion == 400 ) {
		return FALSE;
	}

	// PCX파일 선택 
	CSelectPCXFileDlg dlg;

	dlg.m_strSrcFileName = m_strFileName;
	dlg.m_strSrcPathName = m_strPathName;
	
	sprintf( buf, "%s.pcx", strfilenamePre );
	dlg.m_editFileName = buf;
	dlg.m_editFolder = m_strPathName;

	if( dlg.DoModal() != IDOK ) {
		return FALSE;
	} 

	len = dlg.m_editFolder.GetLength();
	if( dlg.m_editFolder[ len - 1 ] != '\\' ) {
		dlg.m_editFolder += '\\';
	}

	sprintf( pcxfilename, "%s%s", dlg.m_editFolder, dlg.m_editFileName );

	// pcx 팔레트 추출 
	if( GetPCXPal16( pcxfilename ) == FALSE ) {
		return FALSE;
	}


	// 새로운 MPR저장하기 
	DWORD dwWrite;
	hFile = CreateFile( filename, 
						GENERIC_WRITE, 
						NULL, 
						NULL, 
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	pHeader->siVersion = 400;												// 로컬 16bit 팔레트: spr, mpr 파일의 버전을 400으로 바꾼다!

	WriteFile( hFile, buffer, dwFileSize, &dwWrite, NULL );					// 기존 포멧 데이터 그대로 쓰기 ( 버전만 바뀜 )
	WriteFile( hFile, m_pal565, sizeof( m_pal565 ), &dwWrite, NULL );		// 565 팔레트 쓰기
	WriteFile( hFile, m_pal555, sizeof( m_pal555 ), &dwWrite, NULL );		// 555 팔레트 쓰기
	CloseHandle( hFile );

	delete [] buffer;

	return TRUE;
}

BOOL CNConvertDlg::ConvSpr()
{
	// 기존 파일 포멧뒤에 16bit팔레트만 갖다 붙이는것이므로 spr이나 mpr나 변환방식은 같다
	return ConvMpr();
}


BOOL CNConvertDlg::GetPCXPal16( char *filename )
{
	unsigned char *buffer;
	unsigned char *pImage;
	unsigned char *canvas;

	PCX pcx;
	
	HANDLE hFile = CreateFile( filename, 
							GENERIC_READ, 
							FILE_SHARE_READ, 
							NULL, 
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return FALSE;

	DWORD dwFileSize;

	dwFileSize = GetFileSize( hFile, NULL );

	if( dwFileSize == INVALID_FILE_SIZE ||
		dwFileSize == 0 ) {

		CloseHandle( hFile );
		return FALSE;
	}


	buffer = new unsigned char[ dwFileSize ];

	DWORD dwRead;
	if( ReadFile( hFile, buffer, dwFileSize, &dwRead, NULL ) == FALSE ) {
		delete [] buffer;
		CloseHandle( hFile );
		return FALSE;
	}

	if( dwRead != dwFileSize ) {
		delete [] buffer;
		CloseHandle( hFile );
		return FALSE;
	}
	
	CloseHandle( hFile );

	memcpy( &pcx.Head, buffer, sizeof( _head ) );

	pImage = buffer + sizeof( _head );

	int xsize, ysize;

	xsize = pcx.Head.xmax + 1;
	ysize = pcx.Head.ymax + 1;

	canvas = new unsigned char[ xsize * ysize ];

	int i, j, k, count;

	for( i = 0; i < ysize; ++i ) {

		j = 0;

		while( j < xsize ) {
	
			if( *pImage > 192 ) {
			
				count = *pImage - 192;
				++pImage;

				for( k = 0; k < count; ++k ) {
					canvas[ i * xsize + j ] = *pImage;
					++j;
				}

				++pImage;
			} else {

				canvas[ i * xsize + j ] = *pImage;
				++pImage;
				++j;
			}

		}

	}

	memcpy( &pcx.Tail, pImage, sizeof( _tail ) );
	pImage += sizeof( _tail );

	if( dwFileSize != (DWORD)pImage - (DWORD)buffer ) {
		delete [] canvas;
		delete [] buffer;
		return FALSE;
	}

	UI16 r, g, b;

	// 565 팔레트 만들기
	for( i = 0; i < 256; ++i ) {

		r = pcx.Tail.palette[ i * 3 ] >> 3;
		g = pcx.Tail.palette[ i * 3 + 1 ] >> 2;
		b = pcx.Tail.palette[ i * 3 + 2 ] >> 3;

		r = RGB16_565_RMASK & ( r << 11);
		g = RGB16_565_GMASK & ( g << 5);
		b = RGB16_565_BMASK & ( b );
		
		m_pal565[ i ] = r | g | b;
	}

	// 555 팔레트 만들기
	for( i = 0; i < 256; ++i ) {

		r = pcx.Tail.palette[ i * 3 ] >> 3;
		g = pcx.Tail.palette[ i * 3 + 1 ] >> 3;
		b = pcx.Tail.palette[ i * 3 + 2 ] >> 3;

		r = RGB16_555_RMASK & ( r << 10);
		g = RGB16_555_GMASK & ( g << 5);
		b = RGB16_555_BMASK & ( b );
		
		m_pal555[ i ] = r | g | b;
	}

	delete [] canvas;
	delete [] buffer;

	return TRUE;
}

void CNConvertDlg::OnButtonMakealphapcx() 
{
	// TODO: Add your control notification handler code here

	int i;

	PCX pcx;

	HANDLE hFile = CreateFile( "alpha.pcx", 
						GENERIC_WRITE, 
						NULL, 
						NULL, 
						CREATE_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL );

	if( hFile == INVALID_HANDLE_VALUE ) return;


	pcx.Head.maker= 10;
	pcx.Head.ver  =  5;
	pcx.Head.encode= 1;
	pcx.Head.bpp   = 8;
	pcx.Head.xmin  = 0;
	pcx.Head.ymin  = 0;
	pcx.Head.xmax  = 1;
	pcx.Head.ymax  = 1;

	for(i=0;i<48;i++)
	  pcx.Head.palette[i]=0;

	pcx.Head.vmode   = 0;
	pcx.Head.nplanes = 1;
	pcx.Head.palette_type = 0;

	for(i=0;i<54;i++)
	  pcx.Head.unused[i]=0;

	DWORD dwWrite;
	WriteFile( hFile, &pcx.Head, sizeof( _head ), &dwWrite, NULL );

	unsigned char ch = 0;
	WriteFile( hFile, &ch, 1, &dwWrite, NULL );
	WriteFile( hFile, &ch, 1, &dwWrite, NULL );
	WriteFile( hFile, &ch, 1, &dwWrite, NULL );
	WriteFile( hFile, &ch, 1, &dwWrite, NULL );

	pcx.Tail.identification=12; // always;	

	int step;
	for( i = 0; i < 256; ++i ) {
		step = ( i * 8 ) / 256;

		pcx.Tail.palette[ i * 3 ] = (UI08)((float)( step / 8. ) * 255);
		pcx.Tail.palette[ i * 3 + 1 ] = (UI08)((float)( step / 8. ) * 255);
		pcx.Tail.palette[ i * 3 + 2 ] = (UI08)((float)( step / 8. ) * 255);
	}

	// Trans
	pcx.Tail.palette[ 254 * 3 ] = 255;
	pcx.Tail.palette[ 254 * 3 + 1 ] = 255;
	pcx.Tail.palette[ 254 * 3 + 2 ] = 255;

	WriteFile( hFile, &pcx.Tail, sizeof( _tail ), &dwWrite, NULL );

	CloseHandle( hFile );
}
