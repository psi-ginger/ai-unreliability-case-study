// Patch.h : main header file for the PATCH application
//

#if !defined(AFX_PATCH_H__D3C6C774_5402_42E9_8DFC_9CD3B2859E36__INCLUDED_)
#define AFX_PATCH_H__D3C6C774_5402_42E9_8DFC_9CD3B2859E36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPatchApp:
// See Patch.cpp for the implementation of this class
//

class CPatchApp : public CWinApp
{
public:
	CPatchApp();
	~CPatchApp();

	class CImpIDispatch* m_pDispOM;

public:
	


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPatchApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CPatchApp theApp;


#define NDelete(p)			{ if(p) { delete (p); (p)=NULL; } }
#define NDelete_Array(p)	{ if(p) { delete [] (p); (p)=NULL; } }
#define NRelease(p)			{ if(p) { (p)->Release(); (p)=NULL; } }

struct NCryptHeader
{
	BYTE	id[ 4 ];

	WORD	version;
	WORD	encodeType;

	DWORD	dwDataSize;

	BYTE	key;
	BYTE	chksum;

	BYTE	reserved[ 10 ];

};

void NByteEncrypt( BYTE *pSrc, BYTE *pDest, int size, BYTE key );
void NByteDecrypt( BYTE *pSrc, BYTE *pDest, int size, BYTE key );

bool NWriteFile( TCHAR *filename, BYTE *pBuffer, int size );
bool NWriteCryptFile( TCHAR *filename, BYTE *pSrc, int size );
bool NLoadCryptFile( TCHAR *filename, BYTE *pDest );

class NFile
{
public:
	NFile();
	~NFile();

	bool			LoadFile( TCHAR *filename );
	unsigned char*	GetBuffer() { return m_pBuffer; };

	unsigned char*	m_pBuffer;

	DWORD			m_dwBufferSize;
	DWORD			m_dwFileSize;
};


//int GetGameVersionInFile();
//bool SetGameVersionInFile(int siVersion);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCH_H__D3C6C774_5402_42E9_8DFC_9CD3B2859E36__INCLUDED_)
