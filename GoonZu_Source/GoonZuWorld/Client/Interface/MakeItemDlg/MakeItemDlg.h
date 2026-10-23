// MakeItemDlg.h: interface for the CMakeItemDlg class.
//
//////////////////////////////////////////////////////////////////////
/*
#if !defined(AFX_MAKEITEMDLG_H__C68DCD45_67F1_41B5_8AE0_2A03FE0C5121__INCLUDED_)
#define AFX_MAKEITEMDLG_H__C68DCD45_67F1_41B5_8AE0_2A03FE0C5121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../Client/client.h"

class CMakeItemDlg  
{
public:
	CMakeItemDlg();
	virtual ~CMakeItemDlg();

	void					Create( HWND hParent , HINSTANCE hInst );
	void					Set( cltItem *pclItem, SI32 baseexp, SI32 skillunique,SI32 skillexp,SI32 monstercorp);

	void					CreateDlg();

	static BOOL CALLBACK	StaticMakeItemDlgMsgDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			MakeItemDlgMsgDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void					Show();
	void					Hide();

private:
	HWND			m_hWnd;
	HINSTANCE		m_hInst;

	HWND			m_hImage;
	HWND			m_hDlg;
	HWND			m_hEdit;

	HBRUSH			m_hEditBrush;
	HBRUSH			m_hDlgBrush;

};

#endif // !defined(AFX_MAKEITEMDLG_H__C68DCD45_67F1_41B5_8AE0_2A03FE0C5121__INCLUDED_)
*/