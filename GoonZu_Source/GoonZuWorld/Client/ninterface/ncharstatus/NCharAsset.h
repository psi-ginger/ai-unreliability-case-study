//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		cltNAssetStatus

	작성일 :		05/03/14
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
//#if defined(__LEON_NCHARSTATUS__) && defined(_DEBUG)

#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"


#define MAX_CHARSTATUS_PAGE_NUMBER	5

#define WANTIED_ANIMAL_CHANGE_TYPE	2

class cltCharClient;
class cltParametaBox;
class cltSummonScroll;
class cltCharManager;
class cltItem;

class CButton;
class CCheckBox;
class CComboBox;
class CEdit;
class CImageStatic;
class CList;
class CRadioButton;
class CStatic;
class COutLine;

// 자산 창
class cltNAssetStatus : public CDialogController
{
public:

	CStatic			*m_pStatictext_asset;
	CStatic			*m_pStatictext_serverrank;
	CEdit			*m_pEditbox_serverrank;
	CStatic			*m_pStatictext_totalwealth;
	CEdit			*m_pEditbox_totalwealth;
	CStatic			*m_pStatictext_bank;
	CStatic			*m_pStatictext_bankmoney;
	CEdit			*m_pEditbox_bankmoney;
	CStatic			*m_pStatictext_bankvillage;
	CEdit			*m_pEditbox_bankvillage;
	CStatic			*m_pStatictext_stock;
	CList			*m_pListbox_stock;
	CStatic			*m_pStatictext_house;
	CList			*m_pListbox_house;
	CStatic			*m_pStatictext_wealthupdate	;
	COutLine		*m_pOutline_noname1	;
	COutLine		*m_pOutline_noname2	;
	COutLine		*m_pOutline_noname3	;
	COutLine		*m_pOutline_noname4	;

	// 상단 정보
	COutLine		*m_pOutline_noname6	;
	CStatic			*m_pStatic_guildinfo;
	CStatic			*m_pStatic_guildpos	;
	CStatic			*m_pStatic_guildname;
	CStatic			*m_pStatic_guildrank;
	CEdit			*m_pEditbox_guildpos;
	CEdit			*m_pEditbox_guildname;
	CEdit			*m_pEditbox_guildrank;

	// 상단 주식
	COutLine		*m_pOutLine_guildstock;
	CStatic			*m_pStatic_guildstock;
	CList			*m_pListbox_guildstock;

	// 결투장 전적
	CStatic			*m_pStatic_battlescore;
	CStatic			*m_pStatic_battlewin;
	CStatic			*m_pStatic_defeatscore;
	CStatic			*m_pStatic_battlemaxwin;
	CEdit			*m_pEdit_battlewin;
	CEdit			*m_pEdit_defeatscore;
	CEdit			*m_pEdit_battlemaxwin;

	//아큐
	COutLine		*m_pOutLine_acu;
	CStatic			*m_pStatic_acu;
	CEdit			*m_pEditbox_acu;
    

	cltNAssetStatus(cltCharManager* pclcm);
	~cltNAssetStatus();

	void Create( CControllerMgr *pParent );
	bool IsShow();
	void Show();
	void Hide();
	void Action();
	
	cltCharManager* pclCM;
	SI32 CharUnique;
	bool m_bShow;

	static void CALLBACK NAssetStatusStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NAssetStatusProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	// 상태 정보에 길드 정보를 표시한다.
	void ShowMyGuildInfo( SI32 villageunique, TCHAR* guildname, TCHAR* guildrankname );

	void Close();
};

//#endif
