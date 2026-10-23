#ifndef _ERRORMSGDLG_H_
#define _ERRORMSGDLG_H_

#include <Directives.h>
#include <stdio.h>

#include "../../DBManager/GameDBManager_World/DBGameLogMsg.h"

#include "..\..\..\Resource.h"

#define MAX_BIG_ERRORMSG_NUM 20480

class CErrorMsgDlg
{
public:
	CErrorMsgDlg();
	~CErrorMsgDlg();

	void					Create( HINSTANCE hInst, HWND hParent );
	void					Set( bool Activate, TCHAR *strTitleData, TCHAR *strPrintData, ... );
	void					BigSet( TCHAR *strTitleData, TCHAR *strPrintData, ... );
	void					BigAppend( TCHAR *strPrintData );

	static BOOL CALLBACK	StaticErrorMsgDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			ErrorMsgDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	static BOOL CALLBACK	StaticErrorMsgDlgProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			ErrorMsgDlgProc1(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void					Show( bool Activate );
	void					Hide();

	void					Show1();
	void					Hide1();

	void					SetLogMsg(SI32 Result,TCHAR * strLogData,SI32 page);
	void					ShowLogMsg();

private:
	HWND		m_hErrorMsgDlg;
	HWND		m_hErrorMsgDlg1;
	
	HWND		m_hErrorEdit;
	HWND		m_hErrorEdit1;

	TCHAR		m_strPrintMsg[ 7168 ];
	TCHAR*		m_pstrPrintBigMsg;
	TCHAR		m_strTitleMsg[ 1024 ];

	bool		m_bShowFlag;
	bool		m_bShowFlag1;
	
	HBRUSH		m_hDlgBrush;
	HBRUSH		m_hDlgBrush1;

	TCHAR		m_strLogMsg[sizeof(stLOGData) * MAX_STLOGDATA_PER_PACKET * MAX_RESPONSING_PERSONAL_LOG + 512 ];

};

extern	CErrorMsgDlg	g_ErrorMsg;

#endif