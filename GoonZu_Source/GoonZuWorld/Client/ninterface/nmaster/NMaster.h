//---------------------------------
// 2004/1/31 김태곤
//---------------------------------
/* ==========================================================================
클래스 :		CNMasterInfo

작성일 :		05/03/03
작성자 :		정용래

목  적 :		CNMasterInfo 클래스는  전체 화면 모드 실행시 NInterface에
따라 게임 화면 안에서 장인 정보 다이얼로그를 생성한다.

용  법 :		

변경사항 :		없음

차후작업 :		cpp 참고

========================================================================*/

//#if (defined(__LEON_START_ALL) || defined(__LEON_NMASTER__)) && defined(_DEBUG)
#define _LEON_NMASTER_

#pragma once

#include <Directives.h>
#include "Skill\Skill.h"

#include "../../InterfaceMgr/DialogController.h"
#include "../../../Common/Item/ItemCommon/ItemUnit.h"

#define MAX_NMASTER_PER_SKILL	20			// 각 스킬당 마스터 수. 

class CStatic;
class CComboBox;
class CList;
class CButton;
class CEdit;

#include "../../..//Common/Master/Master.h"
class cltMasterUnitInfo;

class cltNMasterUnitInfo{
public:

	SI16 siCharUnique;
	TCHAR szName[MAX_PLAYER_NAME];
	TCHAR szAccountID[MAX_PLAYER_NAME];
	SI16 siSkillLevel;
	SI16 siHnd;
	SI16 siLuk;
	SI16 siMapIndex;
	SI32 siPersonID;

	cltNMasterUnitInfo()
	{
		Init();

	}

	cltNMasterUnitInfo(SI16 charunique, TCHAR* pname, TCHAR* accountid, SI16 skilllevel, SI16 hnd, SI16 luk, SI16 mapindex, SI32 personid)
	{
		siCharUnique	= charunique;
		MStrCpy(szName,			pname,	MAX_PLAYER_NAME );
		MStrCpy(szAccountID, accountid, MAX_PLAYER_NAME );
		siSkillLevel	= skilllevel;
		siHnd			= hnd;
		siLuk			= luk;
		siMapIndex		= mapindex;
		siPersonID		= personid;

	}

	void Init()//초기화.
	{
		siCharUnique	= 0;
		StringCchCopy(szName, MAX_PLAYER_NAME, TEXT(""));
		StringCchCopy(szAccountID, MAX_PLAYER_NAME, TEXT(""));
		siSkillLevel	= 0;
		siHnd			= 0;
		siLuk			= 0;
		siMapIndex		= 0;
		siPersonID		= 0;
	}

	void Set(cltNMasterUnitInfo* pclinfo)
	{
		siCharUnique	= pclinfo->siCharUnique;
		MStrCpy( szName, pclinfo->szName, MAX_PLAYER_NAME );
		MStrCpy( szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME );
		siSkillLevel	= pclinfo->siSkillLevel;
		siHnd			= pclinfo->siHnd;
		siLuk			= pclinfo->siLuk;
		siMapIndex		= pclinfo->siMapIndex;
		siPersonID		= pclinfo->siPersonID;
	}

	void Set(cltMasterUnitInfo* pclinfo)
	{
		siCharUnique	= pclinfo->siCharUnique;
		MStrCpy( szName, pclinfo->szName, MAX_PLAYER_NAME );
		MStrCpy( szAccountID, pclinfo->szAccountID, MAX_PLAYER_NAME );
		siSkillLevel	= pclinfo->siSkillLevel;
		siHnd			= pclinfo->siHnd;
		siLuk			= pclinfo->siLuk;
		siMapIndex		= pclinfo->siMapIndex;
		siPersonID		= pclinfo->siPersonID;
	}

};


//----------------------------
// 특정 기술에 대한 장인 정보. 
//----------------------------
class cltNMasterInfo{
public:
	SI32 siRegistedNumber;
	cltNMasterUnitInfo clUnit[MAX_NMASTER_PER_SKILL];

	cltNMasterInfo()
	{
		siRegistedNumber = 0;
	}

	~cltNMasterInfo()
	{

	}

	void Init()
	{
		SI32 i;

		siRegistedNumber = 0;

		for(i = 0;i < MAX_NMASTER_PER_SKILL;i++)
		{
			clUnit[i].Init();
		}
	}

};



class CNMasterInfo : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_BIG_POS_X = 1024 - 320 + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 120,
		DLG_SMALL_POS_X = 800 - 320 + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 120,
	};

	RECT DlgRect;
	bool bDlgRectInitSwitch;

	SI32 siSkillInfo[MAX_NMASTER_PER_SKILL];

	SI32			siCurrentSkillUnique;
	SI32			m_siSelectedIndex;
	cltNMasterInfo clMasterInfo[MAX_SKILL_NUMBER];

	CNMasterInfo();
	~CNMasterInfo();

	void Create();
	static void CALLBACK StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	// 내림차순으로 정렬한다. 
	void Sort(SI32 sisortdata[]);
	SI32 Sort(TCHAR* psortname[], SI32* punique);

	// 장인 명단을 보여준다. 
	void ShowMasterList(SI32 skillunique);

	void SetSkillList();

	//bool IsShow();
	//void Show();
	//void Hide();

	void Action();

	void SetGreetingText(TCHAR* pName, TCHAR* pText);
	//bool		m_bShow;

	CStatic		*m_pMasterSkillStatic;
	CStatic		*m_pMasterNameStatic;
	CEdit		*m_pMasterGreeting;
	CComboBox	*m_pMasterSkillComboBox;
	CList		*m_pMasterList;
	//CStatic		*m_pProfileStatic;	// html 사용안함
	//CStatic		*m_pIntroStatic;	// html 사용안함
	CButton		*m_pGGOMZIButton;
	CButton		*m_pMasterChatButton;
	CButton		*m_pCloseButton;

};
//#endif //#if defined(__LEON_START_ALL) || defined(__LEON_NMASTER__) && defined(_DEBUG)
