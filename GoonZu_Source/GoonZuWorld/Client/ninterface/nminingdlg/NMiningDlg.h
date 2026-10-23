#ifndef _NMININGDLG_H_
#define _NMININGDLG_H_

#include <Directives.h>
#include "../../InterfaceMgr/DialogController.h"
#include "../../NLib/NUpdateTimer.h"

#define MAX_MINE_NUMBER	20
#define MAX_MINE_NAME_LENGTH	60

class CStatic;
class CButton;
class CEdit;
class CImageStatc;
class TSpr;
class NUpdateTimer;

class CNMiningDlg : public CDialogController
{
public:
	bool	m_bStopMining;
	SI16	m_siMiningItemUnique;
	SI32	m_uiLeaseLeftTime;

	CNMiningDlg();
	~CNMiningDlg();

	void Create();

	static	void CALLBACK StaticNMiningDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void	CALLBACK NMiningDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl );

	void Action();
	void Init();

	void SetMiningItem( SI16 ItemUnique, SI16 SuccessPercent, SI32 DelayTime );
	void SetMiningExplain();

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );

	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void SetMinePos( SI32 PosX, SI32 PosY);

	void SetMiningInfo(TCHAR* strMineName, SI16 nMineNum);

	BOOL ReturnAutoMining();

	void DrawMiningDelay(SI32 rx, SI32 ry);

	DWORD	m_dwMiningStartClock;
	DWORD	m_dwMiningEndClock;

private:
	CStatic *m_pExplainStatic;
	CStatic *m_pStatic;
	CStatic *m_pminieralinfoStatic;
	
	CButton	*m_pStartButton;

	CEdit	*m_pStatusEdit;
	CEdit	*m_pMineinfoEdit;

	CImageStatic *m_pImageStatic;

	COutLine*	m_pOutline;

	TCHAR	m_strMineName[MAX_MINE_NUMBER][MAX_MINE_NAME_LENGTH];
	SI16	m_siMineNum[MAX_MINE_NUMBER];

	TSpr	m_SprMineGage;
	SI16	m_siSuccessPercent;
	SI32	m_siDelayTime;

	SI16	m_siEffcectCounter;

	SI32	m_siMinePosX;
	SI32	m_siMinePosY;

	NUpdateTimer	kUpdateTimer_DrawImage;

	SI08	m_siNowFontNum;
};

#endif