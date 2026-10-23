// NPCMenuDlg.h: interface for the CNPCMenuDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPCMENUDLG_H__D730027E_85C1_451D_B1E0_B4E80FF26B62__INCLUDED_)
#define AFX_NPCMENUDLG_H__D730027E_85C1_451D_B1E0_B4E80FF26B62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CNPCMenuDlg  
{
public:
	CNPCMenuDlg();
	virtual ~CNPCMenuDlg();

	void Create(HINSTANCE hinst, HWND hwnd );
	void CreateDlg(SI32 charkind,SI32 charunique,char * txt1 , char * txt2 ,
				   bool button1,bool button2,char * txt);

	static BOOL CALLBACK	StatiCNPCMenuProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			NPCMenuProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	HWND GetHwnd(){return m_hDlg;}
	void Close();

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	HBITMAP m_hPortraitBitmap;

	HWND m_hDlg;

	SI32 m_siCharKind;
	SI32 m_siCharUnique;
	
	RECT	m_rtDlg;

	BOOL	m_bRect;

};

#endif // !defined(AFX_NPCMENUDLG_H__D730027E_85C1_451D_B1E0_B4E80FF26B62__INCLUDED_)
