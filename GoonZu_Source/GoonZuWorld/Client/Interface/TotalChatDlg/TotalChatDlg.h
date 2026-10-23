
#ifndef _TOTALCHAT_H_
#define _TOTALCHAT_H_

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

#define MAX_TOTALCHAT_PAGE_NUMBER 4
class CTotalChatTabDlg1;
class CTotalChatTabDlg2;
class CTotalChatTabDlg3;
class CTotalChatTabDlg4;

class CTotalChatDlg 
{
public:
	CTotalChatDlg();
	~CTotalChatDlg();


	void					Create(HINSTANCE hInst, HWND hParent );

	

	static BOOL CALLBACK	StatiCTotalChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			TotalChatDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);


	void ComboSetting(HWND hDlg);




	
	void					Show();
	void					Hide();
	void                    ShowTabDialog(int index);
	CTotalChatTabDlg1*  m_pCTotalChatTabDlg1;
	CTotalChatTabDlg2*  m_pCTotalChatTabDlg2;
	CTotalChatTabDlg3*  m_pCTotalChatTabDlg3;
	CTotalChatTabDlg4*  m_pCTotalChatTabDlg4;


private:
	HWND			m_hTabCtrl;
	HWND			m_hTotalChatDlg;
	HWND			TabDlg[MAX_TOTALCHAT_PAGE_NUMBER];
	HINSTANCE		m_hInst;
	HWND			m_hWnd;

	HWND			m_hDlg;
	
	SI16			m_CurrentTabIndex;				// 현재선택되어져 있는 TabControl의 Index
	HWND			m_hTotalChatTabDlg1;
	SI32			CurrentPage;


	//HWND			m_hToEdit;

};





//extern	CDeathDlg		g_DeathDlg;

#endif
