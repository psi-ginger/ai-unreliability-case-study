
/* ==========================================================================
	클래스 :		CNNFishingDlg

	작성일 :		05/03/25
	작성자 :		김태양

	목  적 :		.
	                

	용  법 :		
	변경사항 :		없음

	차후작업 :		cpp 작업 완료 요

   ========================================================================*/

#pragma once

#include <Directives.h>
#include "../../InterfaceMgr/DialogController.h"
#include "../../NLib/NUpdateTimer.h"

#define MAX_FISH_NUMBER	5
#define MAX_FISH_NAME_LENGTH	60

class DBToolInfo;
class CStatic;
class CEdit;
class CCheckBox;
class CImageStatic;
class TSpr;
class NUpdateTimer;

class CNFishingDlg : public CDialogController
{
public:
	
	CNFishingDlg();
	~CNFishingDlg();

	void Init();

	void Create( );

	static	void CALLBACK StaticNFishingtDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NFishingtDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	bool IsShow();
	void Show();
	void Hide();
	void Action();
	
	void Set( SI32 FishingBaitItemUnique, DBToolInfo *pclToolInfo );

	void SetFishingExplain();
	void SetFishingPlaceInfoExplain();

	void SetAutoFishing( bool AutoFishing );

	void SetFishingInfo(TCHAR* strFishName, SI16 nFishNum);
	
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	BOOL ReturnAutoFishing();

	void DrawFishingDelay(SI32 rx, SI32 ry);

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	
	bool GetCheckBox ();

	void ShowFishinginfo();

	DWORD	m_dwFishingStartClock;
	DWORD	m_dwFishiingEndClock;

	bool m_bStopFishing;
	SI32 m_siFishingBaitItemUnique;
	bool m_bAutoFishing;
private:	
	bool m_bShow;

	DBToolInfo *m_pToolInfo;
	
	TCHAR	m_strFishName[MAX_FISH_NUMBER][MAX_FISH_NAME_LENGTH];
	TCHAR	m_strBuffer[1024];
	SI16	m_siFishNum[MAX_FISH_NUMBER];

	SI08	m_siNowFontNum;
	SI32	m_GlobalImageIndex;
	SI32	m_GlobalImageFont;

	CImageStatic    *m_pFishingImageStatic; 
	CStatic			*m_pFishingStatic1;
	CStatic     	*m_pFishingStatic2;
	CStatic     	*m_pFishingStatic3;
	CStatic			*m_pFishingStatic4;
	CStatic			*m_pFishingStatic5;
	CCheckBox     	*m_pFishingCheckBox;
	CEdit           *m_pFishingEdit1;
	CEdit           *m_pFishingEdit2;
	CEdit			*m_pFishingEdit3;
	COutLine		*m_pOutline;
	
	clock_t			m_PreClock;

	TSpr	m_SprFishGage;

	NUpdateTimer	kUpdateTimer_DrawImage;
};

