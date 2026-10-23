
#ifndef _VOTEDLG_H_
#define _VOTEDLG_H_

#include <Directives.h>
#include "..\..\..\Resource.h"
#include <windows.h>
#include "../../Client/client.h"

struct stCandidature;

class CVoteDlg
{
public:
	HWND m_hDlg;
	
	CVoteDlg();
	~CVoteDlg();

	void Initialize();

	void Create( HINSTANCE hInst, HWND hWnd );

	static	BOOL CALLBACK StaticVoteAdvertiseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK VoteAdvertiseDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	static	BOOL CALLBACK StaticVoteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );
	BOOL	CALLBACK VoteDlgProc( HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam );

	bool IsShow();
	void Show();
	void Hide();
	
	
	void Set( char *VoteExplain, stCandidature *pCandidature );
	
	void LoadCandidatureList();
	void DisplayVoteAdvertise( char *HtmlPage );

private:
	bool m_bShow;


	HWND m_hVoteListCtrl;

	stCandidature *m_pstCandidatureList;

	HWND m_hVoteAdvertiseDlg;
};

#endif

