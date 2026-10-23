#ifndef _PERSONALINFODLG_H_
#define _PERSONALINFODLG_H_

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CPersoanlInfoDlg
{
public:
	CPersoanlInfoDlg();
	~CPersoanlInfoDlg();

	void					Create( HINSTANCE hInst, HWND hParent );
	void					set(char * szPersonalName,char * szFatherName , SI32 ChangeNum ,
								SI16 Level,SI16 FameLevel ,/*char * szSchoolName,*/ char * szAccountID,
								SI32 wealthrank,cltMoney money, char* guildname, char* guildranktype,
								SI32 battlewin,SI32 totalkill,SI32 timekill,SI32 winrank,SI32 killrank);

	static BOOL CALLBACK	StatiCPersoanlInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			PersoanlInfoMsgDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void					Show();
	void					Hide();

private:
	
	HBRUSH			m_hDlgBrush;
	HWND			m_hPersoanlInfoMsgDlg;
	char			m_szAccountID[MAX_PLAYER_NAME];

};

extern	CPersoanlInfoDlg		g_PersoanlInfoMsg;

#endif