#ifndef _FONTMANAGER_H_
#define _FONTMANAGER_H_

#define	MAX_FONT_NUM			10

#include <Directives.h>
#include "../../../NLib/NTCHARString.h"

#define MAX_NUM_FONT		256
#define	MAX_INSTALL_FONT	10


class CFontManager
{
public:
	CFontManager();
	~CFontManager();

	void	Initialize();
	void	Destroy();

	bool	IsExistFont( SI32 si32Height, TCHAR *strFontKind, int &si32ExistIndex );

	void	SetFont( HWND hDlg, SI32 si32ResourceID, SI32 si32Height = 13, TCHAR *strFontKind = TEXT("±¼¸²") );
	HFONT	SetFont( SI32 si32Height = 13, TCHAR *strFontKind = TEXT("±¼¸²") );

private:
	SI32	m_si32FontHeight[ MAX_FONT_NUM ];
	HFONT	m_hFont[ MAX_FONT_NUM ];
	TCHAR	m_strFontKind[ MAX_FONT_NUM ][ 32 ];
	SI32	m_si32FontNum;

public:
	BOOL InstallCustomFont(HWND hWnd);

	//===================================================================
	// FONT INSTALL
	//===================================================================
protected:
	static BOOL				m_bInstallFont[MAX_INSTALL_FONT];
	static NTCHARString64		m_kFontFileName[MAX_INSTALL_FONT];
	static NTCHARString64		m_kValueName[MAX_INSTALL_FONT];
	static NTCHARString64		m_kFontName[MAX_INSTALL_FONT];

	BOOL InstallFont(TCHAR* pszFileName, TCHAR* pszValueName);

	BOOL UnInstallFont(TCHAR* pszFileName, TCHAR* pszValueName);
	BOOL UnInstallCustomFont(HWND hWnd);

	static SI32 CALLBACK EnumFamCallBack(ENUMLOGFONT FAR *lpelf, NEWTEXTMETRIC FAR *lpntm, SI32  siFontType, LPARAM lParam);

};

extern	CFontManager	g_FontManager;

#endif