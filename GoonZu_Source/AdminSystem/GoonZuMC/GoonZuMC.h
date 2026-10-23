// GoonZuMC.h : main header file for the GOONZUMC application
//

#if !defined(AFX_GOONZUMC_H__4070E7F2_6DB1_4F25_ADB4_41824D3E3E10__INCLUDED_)
#define AFX_GOONZUMC_H__4070E7F2_6DB1_4F25_ADB4_41824D3E3E10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "NTextManager.h"

class ServerInfo;
class MasterProcess;
class CGMOrderLogManager;
class CStatisticsData;

/////////////////////////////////////////////////////////////////////////////
// CGoonZuMCApp:
// See GoonZuMC.cpp for the implementation of this class
//

class CGoonZuMCApp : public CWinApp
{
public:
	CGoonZuMCApp();

	MasterProcess*		m_pMasterProcess;
	ServerInfo*			m_pServerInfo;
	CGMOrderLogManager*	m_pGMOrderLogManager;

	CStatisticsData*	m_pStatisticsData;

	char				m_szID[ 32 ];
	char				m_szIP[ 10 ][ 32 ];
	int					m_ipindex;

	BOOL				m_bRun;

	HANDLE				m_hExitEvent;

	void				Alarm( int type );

	BOOL				m_bMaster;
	int					m_nMasterLevel;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGoonZuMCApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGoonZuMCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GOONZUMC_H__4070E7F2_6DB1_4F25_ADB4_41824D3E3E10__INCLUDED_)
