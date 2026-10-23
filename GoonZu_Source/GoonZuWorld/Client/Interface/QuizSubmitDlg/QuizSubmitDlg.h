#ifndef _QUIZSUBMITDLG_H_
#define _QUIZSUBMITDLG_H_

#include < Directives.h >

#include "..\..\..\Resource.h"

#include <windows.h>

#include "..\..\..\Common\Quiz\QuizBaseInfo.h"
#include "..\..\..\Common\Quiz\QuizObj.h"

class CQuizSubmitDlg
{
public:
	CQuizSubmitDlg();
	~CQuizSubmitDlg();

	void Initialize();
	void Destroy();

	void Create( HINSTANCE hInst, HWND hWnd );

	
	static	LRESULT CALLBACK StaticAnswerSubmitEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK AnswerSubmitEditSubProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
	
	static	BOOL CALLBACK StaticQuizSubmitDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL	CALLBACK QuizSubmitDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool IsShow();
	void Show();
	void Hide();

	void InterfaceInit();

	bool IsQuizStatus();
	void QuizCreate( SI16 QuizIndex, SI08 QuizCreateKind, SI16 VillageIndex );
	void QuizAnswer( SI32 PersonID, SI32 CharUnique, SI16 AnswerListIndex, char *CharName, char *Answer );
	void QuizStop();	
	void QuizMaxOXUserNum( SI16 QuizIndex, SI16 QuizMaxOXUserNum, SI16 QuizCurrentOXUserNum );
	void QuizCurrentOXUserNum( SI16 QuizIndex, SI16 QuizCurrentOXUserNum );

	HWND		m_hDlg;

	SI16		m_siQuizIndex;	
	SI16		m_siQuizNumber;

private:
	WNDPROC		m_OldAnswerSubmitEditProc;

	bool		m_bShow;

	bool		m_bQuizStatus;

	bool		m_bQuizPauseStatus;

	SI16		m_siVillageIndex;

	SI08		m_siQuizCreateKind;
	
	stAnswer	*m_QuizAnswerList;
	SI16		m_siAnswerListCount;

	SI32		m_siPrizeMoney;
	
	SI32		m_siQuizStartModeSelectIndex;
	
	SI32		m_siQuizMaxOXUserNum;
	SI32		m_siQuizCurrentOXUserNum;
};

#endif