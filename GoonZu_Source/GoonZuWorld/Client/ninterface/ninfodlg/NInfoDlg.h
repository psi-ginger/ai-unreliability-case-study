 
/* ==========================================================================
클래스 :		
작성일 :		06/04/11
작성자 :		김영진
차후작업 :		
========================================================================*/

#pragma once

#include <directives.h>

#include "CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../Client/Ninterface/nvillagedetailinfo/NVillageDetailInfo.h"

#define MAX_INFO_PAGE_NUMBER	5

#define LEAST_COMMON_MULTILPLE	30	// 각 탭의 Action 반복 횟수 최소공배수값


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
class CControllerObj;

class CNMonsterInfoDlg;	
class CNItemInfoDlg	;	
class CNResidentsInfoDlg;	
class CNVillageDetailInfo;	
class CNHuntMapInfoDlg	;

// 상세 정보 창 메인


class cltNInfoDlg : public CDialogController
{
public:
	
	struct stTabTextInfo
	{
		CControllerObj	*st_pControllerObj;
		COLORREF	st_Color;
		SI16		st_siWeight;

		stTabTextInfo()
		{
			Init();
		}

		void Init()
		{
			memset( this, 0, sizeof( stTabTextInfo ));
		}

		void Set( CControllerObj* in_pControllerObj, COLORREF in_Color, SI16 in_siWeight )
		{
			st_pControllerObj = in_pControllerObj;
			st_Color = in_Color;
			st_siWeight = in_siWeight;
		}

	};

	
	CNItemInfoDlg	*m_pTabDlg0;	
	CNMonsterInfoDlg*m_pTabDlg1;	
	CNHuntMapInfoDlg *m_pTabDlg2;
	CNVillageDetailInfo *m_pTabDlg3;	
	CNResidentsInfoDlg *m_pTabDlg4;	


	BOOL DlgRectInitSwitch;
	RECT DlgRect;

	clock_t			m_LastTakeInOutTime;
	SI32			m_timerCount;
	SI32			m_siCurrentPage;



	CButton			*m_pButton_item;
	CButton			*m_pButton_monster;
	CButton			*m_pButton_hunting;
	CButton			*m_pButton_village;		
	CButton			*m_pButton_npc;

	/*CComboBox		*m_pComboBox_Type;
	CEdit			*m_pEdit_Name;
	CButton			*m_pButton_Accept;
	CList			*m_pListBox_AutoMake;*/
	bool m_bShow;

	//cltNInfoDlg(cltCharManager* pclCM);
	cltNInfoDlg();
	~cltNInfoDlg();

//	void Create( SI32  nDialogID );
	void Create();

	static void CALLBACK NInfoDlgStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NInfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Show();
	void Hide();
	bool IsShow();

	void Action();	
	void SetTabTextInfo( stTabTextInfo* in_SelectedText );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
	void SelectTab(SI32 ControlID);

	void MakeComboBox();//콤보박스를 위해 추가
	void SelectTab(SI16 Selectindex);// 콤보박스를 위해 추가
	void Find(SI16 Selectindex, TCHAR* pItemname);// 검색기능을 위해

	void GeneralMeetingSuggestInfo(void);

	void Update();
	void AddListItem(TCHAR* findname);
	void Close();							//[진성] 나만의 메뉴에서 사용할 닫기. 

private:
	NTCHARString64		m_LastInput;
};


