/* ==========================================================================
	클래스 :		CNPersonalInfoDlg

	작성일 :		05/03/30
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
#pragma once

//#define _LEON_NPERSONALINFO_

//#if defined(_DEBUG) && defined(_LEON_NPERSONALINFO_)

#include <Directives.h>

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatic;							
class CImageStatic;						
class CEdit;							

class CNPersonalInfoDlg : public CDialogController
{
public:
	enum
	{
		BIG_DISPLAY_MODE_DELTA_X = 16,
		SMALL_DISPLAY_MODE_DELTA_Y = 80,        
		DLG_WIDTH = 0,	// DDF 파일에서 읽어서 처리
		DLG_HEIGHT = 0,	// DDF 파일에서 읽어서 처리
		DLG_BIG_POS_X = 1024 - DLG_WIDTH + BIG_DISPLAY_MODE_DELTA_X,
		DLG_BIG_POS_Y = 100,
		DLG_SMALL_POS_X = 800 - DLG_WIDTH + SMALL_DISPLAY_MODE_DELTA_Y,
		DLG_SMALL_POS_Y = 100,
	};

	CNPersonalInfoDlg();
	~CNPersonalInfoDlg();

	void					Create( );
	static void CALLBACK	StaticNPersonalInfoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK			NPersonalInfoMsgDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );
	void					Show();
	void					Hide();
	bool					IsShow();

	bool					m_bShow;
	// Controls
	CImageStatic	*m_pImageStatic_profile	;
	CEdit			*m_pEdit_intro	;
	CStatic			*m_pStatic_name	;
	CImageStatic	*m_pImageStatic_gomzi	;
	CEdit			*m_pEdit_name	;
	CStatic			*m_pStatic_teacher	;
	CEdit			*m_pEdit_teacher	;
	CStatic			*m_pStatic_famelevel	;
	CEdit			*m_pEdit_famelevel	;
	CStatic			*m_pStatic_country;
	CEdit			*m_pEdit_country;
	CImageStatic	*m_pImageStatic_country;
	CStatic			*m_pStatic_rank	;
	CEdit			*m_pEdit_rank	;
	CStatic			*m_pStatic_greeting	;
	CEdit			*m_pEdit_greeting	;

	// PCK : 상세 정보 창 변경 (08.11.05)
	CStatic			*m_pStatic_NameLevel;
	CEdit			*m_pEdit_NameLevel;
	CStatic			*m_pStatic_GuildName;
	CEdit			*m_pEdit_GuildName;
	CStatic			*m_pStatic_VillageName;
	CEdit			*m_pEdit_VillageName;

	CStatic			*m_pStatic[3];	// 0: 결투장에서 이긴 횟수 1:결투장에서 쓰러트린 상대수 2:시간제한 결투 최고 기록
	CEdit			*m_pEdit[3];

	CButton			*m_pButton_Report;

	void			set(TCHAR * szPersonalName,TCHAR * szFatherName , SI32 ChangeNum ,
						SI16 Level,SI16 FameLevel , SI32 Country, bool ShowCountry,
						TCHAR * szAccountID, SI32 wealthrank,cltMoney money, 
						TCHAR* greeting, SI32 PVPWinNumber, SI32 PVPLoseNumber, 
						TCHAR* PVPWinHighLevelUserName, SI32 siNameLevel, TCHAR* szGuildName, SI32 siVillageUnique );

	void			ReportUser();
	void			RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
private:

	TCHAR			m_szAccountID[MAX_PLAYER_NAME];
	TCHAR			m_szCharName[MAX_PLAYER_NAME];
};

