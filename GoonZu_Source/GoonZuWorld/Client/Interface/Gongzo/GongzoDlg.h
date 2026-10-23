// GongzoDlg.h: interface for the CGongzoDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GONGZODLG_H__41D7F341_1BEF_4CB8_91B1_ACC0751AFC91__INCLUDED_)
#define AFX_GONGZODLG_H__41D7F341_1BEF_4CB8_91B1_ACC0751AFC91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMinisterBase;

#include <Directives.h>
#include <windows.h>
#include <stdio.h>

#include "..\..\..\Resource.h"

class CGongzoDlg  
{
public:
	CGongzoDlg();
	virtual ~CGongzoDlg();

	static BOOL CALLBACK	StatiCGongzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
	BOOL CALLBACK			GongzoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);

	void Create( HINSTANCE hInst, HWND hParent );
	void Set();
	void Show(CMinisterBase *pMini,SI32 pricetime,SI16 leftovervalue,GongzoMember *gongzomember);

private:
	HINSTANCE m_hInst;
	HWND m_hWnd;

	BOOL m_bRectSwitch;
	RECT m_rtDlg;

	HWND m_hDlg;
	HWND m_hGroup;

	char m_szGongzoName[MAX_PLAYER_NAME];
	char m_szGongzoProfile[256];
	char m_szGongzoID[MAX_PLAYER_NAME];

	HBRUSH m_hEditBrush;
	HBRUSH m_hDlgBrush;

	SI32			m_siPriceTime; // 설정 시간
	SI16			m_siLeftOverValue; // 남은 증감 수치
	SI16			m_siSetSkill ; // 설정한 스킬의 유니크
	SI32			m_siIDCEditNum ; // 설정한 스킬의 에디트 박스
	SI32			m_siSpinNum; // 설정한 스킬의 스핀버튼

	GongzoMember	m_stGongzoMember;

	BOOL m_bSet;
	BOOL m_bSetSpin ; // 이미 스핀 버튼을 한번 눌러서 셋업했다.
	BOOL m_bMonthOver; // 설정한지 한달이 지났다.

	void SetSpinButton();
	BOOL SendGongzoMessage();
	void SetSpinDis(BOOL bView);
	void SetSpinValue(SI32 spinunique,SI32 skillunique,SI32 editunique);
	void SetSpinData(SI32 skillunique,SI32 spinunique,SI32 editunique,SI32 staticunique);
};

#endif // !defined(AFX_GONGZODLG_H__41D7F341_1BEF_4CB8_91B1_ACC0751AFC91__INCLUDED_)
