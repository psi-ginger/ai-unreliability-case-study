/* ==========================================================================
	클래스 :		CNHintDlg

	작성일 :		05/04/24
	작성자 :		김태양

	목  적 :		.
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/
#pragma once

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "../../Resource.h"
#include "../../InterfaceMgr/DialogController.h"

#define MAX_HINTCOMBO_TYPE		20
#define MAX_HINTLIST_TYPE		20


class CStatic;
class CButton;
class CComboBox;
class CList;

class NHintDlgExplain : public CDialogController
{
public:
	NHintDlgExplain();
	~NHintDlgExplain();

	//void AddComboType(char *szName,char *szFile);

	void Create();

	static void CALLBACK NHintDlgExplainStaticProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NHintDlgExplainProc( UINT nEvent, int nControlID, CControllerObj* pControl);

	SI32 GetExplainType(char* pExplainType);
	void ShowExplainHint(HWND hDlg, SI32 siHintType);

	void Show(char * szFileName ) ;
	void Set(char * szFileName ) ;

	void ReadListFile();
	void ShowList(char * szFileName);

private:
	//HWND			m_hWnd;
	//HINSTANCE		m_hInst;

	//HWND	m_hDlg;	
	//RECT	m_DlgRect;
	//HBITMAP m_hHitBitmap;
	BOOL	m_DlgRectInitSwitch;

	char	m_szBmpName[MAX_HINTCOMBO_TYPE][128] ;
	char	m_szComboType[MAX_HINTCOMBO_TYPE][256] ;

	char	m_szListType[MAX_HINTLIST_TYPE][128] ;
	char	m_szSubject[MAX_HINTLIST_TYPE][128] ;

	CStatic     	*m_pNHintStatic1; 
	CStatic     	*m_pNHintStatic2;
	CComboBox       *m_pNHintCombo;
	CList     	    *m_pNHintList;
	CButton     	*m_pNHintButton;

};

