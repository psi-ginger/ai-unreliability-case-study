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

// 프론트서버와 핑테스트를 알아보기 위해서
enum
{
	CNCIDC = 0,
	CTCIDC,
	MAX_IDC
};

const int MAX_FRONTSERVER_NUMBER = 18;

extern DWORD g_dwMyFrontServerClock;
extern DWORD g_dwMyDleayClock;
extern DWORD g_dwSendFrontServerClock[MAX_IDC][MAX_FRONTSERVER_NUMBER];
extern DWORD g_dwDleayClock[MAX_IDC][MAX_FRONTSERVER_NUMBER];

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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCH_H__D3C6C774_5402_42E9_8DFC_9CD3B2859E36__INCLUDED_)
