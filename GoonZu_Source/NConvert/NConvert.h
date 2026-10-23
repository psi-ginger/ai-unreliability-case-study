// NConvert.h : main header file for the NCONVERT application
//

#if !defined(AFX_NCONVERT_H__E7343999_A85D_4ECF_AD8A_BEEFD73A5898__INCLUDED_)
#define AFX_NCONVERT_H__E7343999_A85D_4ECF_AD8A_BEEFD73A5898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNConvertApp:
// See NConvert.cpp for the implementation of this class
//

class CNConvertApp : public CWinApp
{
public:
	CNConvertApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNConvertApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CNConvertApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NCONVERT_H__E7343999_A85D_4ECF_AD8A_BEEFD73A5898__INCLUDED_)
