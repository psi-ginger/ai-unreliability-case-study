/* ==========================================================================
	클래스 :		CNMonsterInfoDlg

	작성일 :		05/02/25
	작성자 :		정용래

	목  적 :		CNMonsterInfoDlg 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 몬스터 정보 다이얼로그를 생성한다.

	용  법 :		Client.h에 선언된 CNMonsterInfoDlg			*m_pNMonsterInfoDlg로
					컨트롤 한다.

	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/

//#if (defined(__LEON_START_ALL__) || defined(__LEON_NMONSTERINFODLG__)) && defined(_DEBUG)
#pragma once

#include "..\..\..\Common\Char\KindInfo\KindInfo.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/Item/ItemCommon/ItemUnit.h"

#define LEAST_COMMON_MULTILPLE	30

class CStatic;
class CComboBox;
class CRadioButton;
class CCheckBox;
class CImageStatic;
class CEdit;
class CList;

struct stNMonsterInfoDlgData
{
	TCHAR *pData;
	SI16 siParam;
};

class CNMonsterInfoDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_BIG_POS_X = 1024 - 340 + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 150,
		DLG_SMALL_POS_X = 800 - 340 + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 150,
	};

	CNMonsterInfoDlg();
	~CNMonsterInfoDlg();

	void Initialize();
	void Destroy();

	void Create(CControllerMgr * pParent);

	static	void CALLBACK StaticNMonsterInfoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NMonsterInfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	
	void ShowMonsterInfo_Internal( TCHAR* szSelText );
	void ShowMonsterInfo( TCHAR* szSelText );

	void SetData( void *pData );

	bool AddCombo();

	void SortName(SI32 comboindex[]);
	
	void Show();
	void Hide();

	//검색기능을 위해
	void Action();
	void MonsterFind(TCHAR* pStrname);
	void Find(TCHAR* pItemname);
	void AddListItem(TCHAR* findname);
	void Update();
	//void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  );
	//void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
    
private:
	bool m_bComboBoxInit;

	SI16 m_siSelectedIndex;

	TSpr *m_pBackGroundSpr;
	
	SI32			m_timerCount;

	CStatic			*m_pMonsterStatic;
	CComboBox		*m_pMonsterSelectComboBox;
	CRadioButton	*m_pNameSortRadioButton;
	CRadioButton	*m_pLevelSortRadioButton;
	CButton			*m_pSearchDetailButton;
	CImageStatic	*m_pMonsterPortraitImage;
	COutLine		*m_pOutlineInfo;
	CEdit			*m_pMonsterInfoEdit;
	CStatic			*m_pItemListStatic;
	CList			*m_pItemListList;
	CStatic			*m_pSummonListStatic;
	CList			*m_pSummonListList;
	//CStatic			*m_pToleranceListStatic;
	//CList			*m_pToleranceListList;
	// 검색기능
	CList			*m_pListBox_AutoMake;
	CEdit			*m_pEdit_Name;
	CButton			*m_pButton_Accept;
	CImageStatic*	m_pImageStaticback;
	CStatic			*m_pStatic_Back;//백그라운드
	
	SI32	m_siSort; // 소트를 무엇으로 할 것인지 체크 버튼에 사용할 인자값 0 : 이름 1 : 등급
	TCHAR	m_szSortName[MAX_KIND_NUMBER][MAX_KIND_NAME_LENGTH] ;

private:
	NTCHARString64		m_LastInput;

};

//#endif//#if (defined(__LEON_START_ALL__) || defined(__LEON_NMONSTERINFODLG__)) && defined(_DEBUG)
