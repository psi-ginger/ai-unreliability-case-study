/*
#ifndef _RAREITEMMSGDLG_H_
#define _RAREITEMMSGDLG_H_

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"
#include "../../Client/client.h"

#define HITBUTTON_RECOMMEND		1
#define HITBUTTON_SUMMON		2
#define HITBUTTON_RAREITEM		3

class CRareItemMsgDlg
{
public:
	CRareItemMsgDlg();
	~CRareItemMsgDlg();

	void					Create( HINSTANCE hInst, HWND hParent );
	void					Set( cltItem *pclItem, char *PlayerName,char *strTitleData, char *strPrintData,
								 SI32 siHorse = 0 ,SI32 siSummon = 0 ,SI32 siHintButton = 0 );

	void					CreateDlg();

	static BOOL CALLBACK	StaticRareItemMsgDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			RareItemMsgDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void					Show();
	void					Hide();

private:
	HWND			m_hWnd;
	HINSTANCE		m_hInst;

	HWND			m_hImageWnd;

	HWND			m_hRareItemMsgDlg;

	HWND			m_hRareItemEdit;
	HBRUSH			m_hEditBrush;

	char			m_strPrintMsg[ 2048 ];
	char			m_strTitleMsg[ 64 ];

	HBITMAP			m_hPortraitBitmap ;

	char			m_strPlayerName[ MAX_PLAYER_NAME ];

	SI32			m_siHintDlg;
	HBITMAP			m_hHitButton;

};

//extern	CRareItemMsgDlg		g_RareItemMsg;

#endif
*/