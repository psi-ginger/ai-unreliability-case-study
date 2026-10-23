#ifndef _LOGINCLIENT_H_
#define _LOGINCLIENT_H_

#include <Directives.h>

#include "../../Resource.h"
#include "../../Common/CommonHeader.h"

#define	LOGIN_ERROR_MSG_LENGTH			2048

struct	stLoginData
{
	TCHAR	m_strUserID[ MAX_PLAYER_NAME ];
	TCHAR	m_strUserPassword[ MAX_PLAYER_NAME ];
};

//
#define MAX_REGISTSITE_NUM				50

struct stRegistSiteInfo
{
	TCHAR	m_strSiteName[ MAX_PLAYER_NAME ];	// 사이트 명
	SI16	m_siIndex;							// 사이트 고유의 값 ( 단, goonzu 은 1 임 ) - 1부터 시작
	SI32	m_siDefaultSite;					// bool 형태인데.. _ftscanf 할려고... ㅡㅡ
};

enum	{ LOGIN_READY, LOGIN_OK, LOGIN_CANCEL, LOGIN_ID_REGIST };
/*
class CLoginClient
{
public:
	CLoginClient();
	~CLoginClient();

	void	Initialize();
	void	Destroy();

	void	Create( HINSTANCE hInst, HWND hParent );

	static	LRESULT CALLBACK StaticIDInputEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK IDInputEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK StaticPasswordInputEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK PasswordInputEditSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK StaticOKButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OKButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK StaticCancelButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK CancelButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	LRESULT CALLBACK StaticRegistButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK RegistButtonSubProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static	BOOL CALLBACK StaticLoginClientDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL	CALLBACK LoginClientDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	bool	GetExistLoginID();

	void	SetLoginData( TCHAR *strID, TCHAR *strPassword );
	void	GetLoginData( stLoginData *pLoginData );

	int		GetStatus();
	
	void	SmallShow();
	void	BigShow();

	void	Show( TCHAR *strErrorMsg = NULL );
	void	Hide();

	bool	IsShow();

	void	DrawTransparentBitmap(HDC hdc, HBITMAP hBitmap, short xStart, short yStart, COLORREF cTransparentColor);

	void	ShowMiniHomePage();

	SI16		m_siCurrentSelectedSiteIndex;
	stRegistSiteInfo *m_pRegistSiteInfoList;

private:	
	HWND		m_hDlg;

	HWND		m_hIDInputEdit;
	HWND		m_hPasswordInputEdit;
	HWND		m_hOKButton;
	HWND		m_hCancelButton;
	HWND		m_hRegistButton;
	
	HBITMAP		m_hLoginButtonBitmap;
	
	HWND		m_hComboSite;
	HWND		m_hExplainEdit;

	//HBITMAP		m_hAllUserUsingPossibleBitmap;
	HBITMAP		m_hLoginBackGroundBitmap;
	
	stLoginData	m_stLoginData;

	TCHAR		m_strErrorMsg[ LOGIN_ERROR_MSG_LENGTH ];

	bool		m_bErrorExist;

	SI32		m_si32Status;

	bool		m_bFlag;

	WNDPROC		m_OldIDInputEditProc;
	WNDPROC		m_OldPasswordInputEditProc;
	WNDPROC		m_OldOKButtonProc;
	WNDPROC		m_OldCancelButtonProc;
	WNDPROC		m_OldRegistButtonProc;	
	
	
};

extern	CLoginClient g_LoginClient;*/

#endif