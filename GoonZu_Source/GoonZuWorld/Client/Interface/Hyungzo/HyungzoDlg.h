// HyungzoDlg.h: interface for the CHyungzoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HYUNGZODLG_H__F092633B_4BE6_4015_83B3_F48944C709CD__INCLUDED_)
#define AFX_HYUNGZODLG_H__F092633B_4BE6_4015_83B3_F48944C709CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

#define MAX_HYUNGZO_TAB		4

class CMinisterBase;
struct HyungzoPoliceList;
struct HyungzoCriminalList;

class CHyungzoDlg  
{
public:
	CHyungzoDlg();
	virtual ~CHyungzoDlg();

	static BOOL CALLBACK	StatiCHyungzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HyungzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHyungzoMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HyungzoMainProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHyungzoUserProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HyungzoUserProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StatiCHyungzoAnnouncementProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HyungzoAnnouncementProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	// 포교
	static BOOL CALLBACK	StatiCHyungzoPoliceProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HyungzoPoliceProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	void Create( HINSTANCE hInst, HWND hParent );
	void Set();
	void Show(CMinisterBase *pMini,SI16 stealnum,SI16 suspecttime,SI16 prisontime,
			  SI16 prisonerpenalty, HyungzoMember *pHyungzo);

	HWND GetDlgHwnd()
	{
		if ( m_hDlg == NULL )
			return NULL ;

		return m_hDlg;
	}
	
	void ShowListDlg(HyungzoPoliceList* stHyungzoPoliceList);
	void ShowListDlg(HyungzoCriminalList* stHyungzoCriminalList);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	HWND m_hLogList;
	HWND m_hPoliceList;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;
	HWND m_hTabDlg[MAX_HYUNGZO_TAB];
	HWND m_hGroup;

	char m_szHyungzoName[MAX_PLAYER_NAME];
	char m_szHyungzoProfile[256];
	char m_szHyungzoID[MAX_PLAYER_NAME];

	SI16 m_siStealNum; // 악의적 스틸 제한 횟수
	SI16 m_siSuspectTime; // 매크로 프로그램 체크 시간
	SI16 m_siPrisonTime; // 매크로 사용자 수감 시간
	SI16 m_siPrisonerPenalty; // 매크로 사용자 형벌 정도

	SI32 m_siViewDlg ;

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	BOOL m_bSet;

	HBITMAP m_hAccusationHit;

};


#endif // !defined(AFX_HYUNGZODLG_H__F092633B_4BE6_4015_83B3_F48944C709CD__INCLUDED_)
