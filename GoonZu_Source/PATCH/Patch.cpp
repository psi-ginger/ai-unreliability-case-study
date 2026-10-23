// Patch.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Patch.h"
#include "PatchDlg.h"


#include <afxocc.h>
#include "custsite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchApp

BEGIN_MESSAGE_MAP(CPatchApp, CWinApp)
	//{{AFX_MSG_MAP(CPatchApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchApp construction

CPatchApp::CPatchApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

}

CPatchApp::~CPatchApp()
{
	if ( m_pDispOM != NULL )
	{
		delete m_pDispOM;
	}
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPatchApp object

CPatchApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPatchApp initialization

BOOL CPatchApp::InitInstance()
{
	CCustomOccManager *pMgr = new CCustomOccManager;

	m_pDispOM = new CImpIDispatch;


	AfxEnableControlContainer(pMgr);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CPatchDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the 
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CPatchApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}

NFile::NFile()
{
	m_pBuffer = NULL;

	m_dwBufferSize = 0;
	m_dwFileSize = 0;

}

NFile::~NFile()
{
	NDelete_Array( m_pBuffer );
}

bool NFile::LoadFile( TCHAR *filename )
{
	HANDLE hFile = NULL;
	hFile = CreateFile(	filename,
		GENERIC_READ,
		0,
		0,
		OPEN_EXISTING,
		0,
		0 );

	if( hFile == INVALID_HANDLE_VALUE ) 
	{
		return false;
	}

	// 파일 크기를 얻어온다
	DWORD dwFileSize;
	DWORD dwFileSizeHigh;
	dwFileSize = GetFileSize( hFile, &dwFileSizeHigh );

	m_dwFileSize = dwFileSize;
	m_dwBufferSize = ( dwFileSize * 4 + 3 ) / 4;

	// 버퍼 생성
	if( m_pBuffer ) delete [] m_pBuffer;
	m_pBuffer = new unsigned char[ m_dwBufferSize ];

	// 파일에서 데이터 읽어오기
	if( hFile )
	{
		DWORD dwBytesRead;
		ReadFile( hFile, (void*)m_pBuffer, dwFileSize, &dwBytesRead, NULL );

		CloseHandle( hFile );
		hFile = NULL;

		if( dwBytesRead != dwFileSize ) {
			NDelete_Array( m_pBuffer );
			return false;
		}
	}

	return true;
}

bool NWriteFile( TCHAR *filename, BYTE *pBuffer, int size )
{
	HANDLE hFile = NULL;
	hFile = CreateFile(	filename,
		GENERIC_WRITE,
		0,
		0,
		CREATE_ALWAYS,
		0,
		0 );

	if( hFile == INVALID_HANDLE_VALUE ) 
	{
		return false;
	}

	DWORD dwWritten;
	WriteFile( hFile, pBuffer, size, &dwWritten, NULL  );
	CloseHandle( hFile );


	

	return true;
}


bool NWriteCryptFile( TCHAR *filename, BYTE *pSrc, int size )
{
	bool bRet;

	// 버퍼 생성
	DWORD dwBufSize = (( sizeof( NCryptHeader ) + size ) * 4 + 3) / 4;
	BYTE *pBuf = new BYTE[ dwBufSize ];

	// 암호화 파일 헤더 설정
	NCryptHeader *pHeader = (NCryptHeader *)pBuf;

	BYTE chksum, key;

	int r = rand() % ('Z' - 'A');

	chksum = 0;
	key = (BYTE)r + 'A';

	memcpy( pHeader->id, "NCR", 4 );

	pHeader->version = 1;
	pHeader->encodeType = 1;

	pHeader->dwDataSize = size;

	pHeader->key = key;
	pHeader->chksum = chksum;

	NByteEncrypt( pSrc, pBuf + sizeof( NCryptHeader ), size, key );

	bRet = NWriteFile( filename, pBuf, dwBufSize );

	NDelete_Array( pBuf );

	return bRet;
}

bool NLoadCryptFile( TCHAR *filename, BYTE *pDest )
{
	NFile file;

	if( file.LoadFile( filename ) == false ) return false;

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	BYTE *pData = (BYTE*)pHeader + sizeof( NCryptHeader );

	int size = pHeader->dwDataSize;
	BYTE key = pHeader->key;

	NByteDecrypt( pData, pDest, size, key );

	return true;
}

void NByteEncrypt( BYTE *pSrc, BYTE *pDest, int size, BYTE key )
{
	BYTE *pCurSrc = pSrc + size - 1;
	BYTE *pCurDest = pDest + size - 1;

	*pCurDest = *pCurSrc ^ key;

	--pCurSrc;
	--pCurDest;

	while( pCurSrc >= pSrc )
	{
		*pCurDest = *pCurSrc ^ *( pCurDest + 1 );
		*pCurDest = *pCurDest + key;

		--pCurSrc;
		--pCurDest;
	}
}


void NByteDecrypt( BYTE *pSrc, BYTE *pDest, int size, BYTE key )
{
	BYTE *pEndSrc = pSrc + size - 1;

	while( pSrc < pEndSrc )
	{
		*pDest = *pSrc - key;
		*pDest = *pDest ^ *( pSrc + 1 );

		++pSrc;
		++pDest;
	}

	*pDest = *pSrc ^ key;
}


bool SetGameVersionInFile(SI32 siVersion)
{
	if(siVersion <= 0)
		return false;

	char buffer[ 4096 ];
	buffer[ 0 ] = NULL;

	sprintf( buffer, TEXT("%d"), siVersion );
	int len = strlen( buffer );

	if( len > 0 ) NWriteCryptFile( TEXT("Version.dat"), (BYTE*)buffer, len );

	return true;
}

SI32 GetGameVersionInFile()
{
	FILE* fp = NULL;
	TCHAR buffer[1024];

	SI32 gameversion = 0;
	SI16 siSuccess = 0;

	NFile file; 

	if(file.LoadFile( TEXT("Version.dat") ) == false)
	{
		return gameversion;
	}

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	if( pHeader->id[ 0 ] == 'N' &&
		pHeader->id[ 1 ] == 'C' &&
		pHeader->id[ 2 ] == 'R' ) 
	{

		NLoadCryptFile( TEXT("Version.dat"),  (BYTE*)buffer );
		buffer[ pHeader->dwDataSize ] = NULL;


	} else {

		fp = _tfopen( TEXT("Version.dat"), TEXT("rt") );
		if(fp == NULL)
		{
			AfxMessageBox( TEXT(" No <Version.dat> file"), MB_ICONWARNING | MB_OK );
			return -1;
		}

		_fgetts(buffer, 1024, fp);
	}

	siSuccess = _stscanf(buffer, TEXT("%d"), &gameversion );
	if( fp )fclose(fp);

	if(siSuccess == 1)
	{

	}
	else
	{
		gameversion = 0;
	}

	return gameversion;
}
