#ifndef _NKILLSIGNDLG_H_
#define _NKILLSIGNDLG_H_


#include "../../../common/CommonHeader.h"
#include "../../InterfaceMgr/DialogController.h"

class CStatic;

class CNKillSignDlg : public CDialogController
{
enum CLIFETIME_VARIABLE
{
	CMAPNAME_VARIABLE_ENDTIME = 20
};



enum killsign
{
	SIGN_KILL,
	SIGN_DEATH,
	SIGN_END,
};

private:
	static const SI32		m_siMaxLine = 5;	    

	CStatic*				m_pstaticKillSign[SIGN_END][m_siMaxLine];

	SI32					m_siEndValue;
	SI32					m_siCurCount;
	TSpr					m_sprKillSignImg;

	TCHAR					m_szUserName[SIGN_END][m_siMaxLine][MAX_PLAYER_NAME];

public:
	CNKillSignDlg();
	~CNKillSignDlg();

	void Create();
	void Action(void);

	static void CALLBACK StaticCallBackDialogNKillSign( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl );
	void CALLBACK CallBackDialogNKillSign( UINT nEvent, int nControlID, CControllerObj* pControl );

	void SetKillSign(TCHAR* szKillName, TCHAR* szDeathName);
	void DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY);
	void UpdateLine();	//	라인을 한 라인씩 밑으로 갱신
};

#endif