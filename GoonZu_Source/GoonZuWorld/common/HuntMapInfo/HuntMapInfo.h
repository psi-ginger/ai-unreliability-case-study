	// HuntMapInfo.h: interface for the CHuntMapInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HUNTMAPINFO_H__7D4F376B_6529_48F9_BD6E_3E3323B01F56__INCLUDED_)
#define AFX_HUNTMAPINFO_H__7D4F376B_6529_48F9_BD6E_3E3323B01F56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "../../Resource.h"

#include "../Map/MapManager/MapManager.h"

class CHuntMapInfo  
{
public:
	CHuntMapInfo();
	virtual ~CHuntMapInfo();

	static BOOL CALLBACK	StatiCHuntMapInfoProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			HuntMapInfoProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Set( HINSTANCE hInst, HWND hParent );

	HWND GetDlgHwnd(){return m_hDlg;};

private:
	HWND	m_hDlg;
	HWND	m_hMonsterInfo;

	HBRUSH m_hStaticBrush;

	RECT	m_rtDlg;
	BOOL	m_bRect;

	SI32	m_siCombo; // High 콤보 버튼 인덱스
//	SI32	m_siComboLevel; // Low 콤보 버튼에 들어가는 맵들의 출입레벨
	SI32	m_siComboParentMap;

//	SI32	m_siMapLevel[MAX_MAP_NUMBER];
	SI32	m_siParentMap[MAX_MAP_NUMBER];
	char	m_szMapTypeName[MAX_MAP_NUMBER][MAX_MAP_NAME_SIZE];

	SI32	m_siSelectIndex;

	HBITMAP	m_hMapBitmap;	// 맵 지도 bmp 핸들
	HBITMAP m_hSearchDetailBitmap; 
	HBITMAP m_hWarpBitmap;

//	void	AddCombo(HWND hDlg,char * szMapType,SI32 level,char * maptypename);
	void	AddCombo(HWND hDlg,char * szMapType,char * maptypename,SI32 parentmap);

	void	ShowListLow(HWND hDlg,SI32 index,char * szMapName);
	void	ShowListMonster(HWND hDlg,SI32 siKind[],SI32 siLevel[]);

	void	ShowMapInfo(HWND hDlg,char * szMapName);

};

#endif // !defined(AFX_HUNTMAPINFO_H__7D4F376B_6529_48F9_BD6E_3E3323B01F56__INCLUDED_)
