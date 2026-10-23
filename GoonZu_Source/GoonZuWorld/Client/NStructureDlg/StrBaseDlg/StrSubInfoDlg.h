#ifndef _STRSUBINFODLG_H
#define _STRSUBINFODLG_H

#include "StrSubBaseDlg.h"

// 건물 기본 정보 탭 다이얼로그 
class NStrSubInfoDlg : public NStrSubTabDlg
{
public:
	enum {
		EDITBOX_BENEFIT,
		EDITBOX_CAPITAL,
		EDITBOX_CHIEF,
		EDITBOX_EXP,
		EDITBOX_LEVEL,
//		LISTBOX_GUILDLIST,
		BUTTON_REFRESH,
	};
	NStrSubInfoDlg();
	virtual ~NStrSubInfoDlg();

	void InitDlg();

	void Action();

	void NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl );

/*	// 단위 길드 정보를 요청한다.
	BOOL SendRequestGuildUnitInfo();

	void ShowGuildUnitListInfo();			// 단위 길드 구성원 List 정보
	void ShowGuildUnitInfo();				// 단위 길드 정보*/

public:
	BOOL					m_bShowTaxInfo;
	SI32					m_siVillageUnique;
//	SI32					m_siUnitSlot;	// 단위 길드 슬롯 번호
//	cltGuildRankManager*	m_pGuildRank;
};


#endif