// LoginNoticeDlg.h: interface for the CLoginNoticeDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOGINNOTICEDLG_H__61994CB2_A60B_4F91_92A1_01260503D58B__INCLUDED_)
#define AFX_LOGINNOTICEDLG_H__61994CB2_A60B_4F91_92A1_01260503D58B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <stdio.h>

#include "Msg\MsgType-ItemMall.h"
#include "..\..\..\Resource.h"

class CLoginNoticeDlg  
{
public:
	CLoginNoticeDlg();
	virtual ~CLoginNoticeDlg();

	enum
	{
		OTHER_DLG = 0,
		MUSIC_DLG
	};

	SI32					m_siType;
	TCHAR					m_szURL[ MAX_BILL_PAGGE_URL_LENGTH ];
	bool					m_bShow;
	HWND					m_hLoginNoticeDlg;


	void					Create( HINSTANCE hInst, HWND hParent );
	void					Set(TCHAR *url, int width, int height, SI32 Type = OTHER_DLG, TCHAR* Name = NULL );

	static BOOL CALLBACK	StatiCLoginNoticeDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			LoginNoticeDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void					Show();
	void					Hide();

	void					SetShowWindow( bool Flag );
	//HWND			m_hLoginNoticeStatic;
};

extern CLoginNoticeDlg		g_LoginNoticeDlg;

#endif // !defined(AFX_LOGINNOTICEDLG_H__61994CB2_A60B_4F91_92A1_01260503D58B__INCLUDED_)
