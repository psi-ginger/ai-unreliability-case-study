// [영훈] 내부 UI : Pannel (케릭터 기본정보)
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Server/ServerEvent/ServerEvent-InstanceMapInfo.h"



class NServerInstanceMapBounusDlg : public CDialogController
{
	enum{ DATA_NUM = 4,	};
	enum{ FIGURE, ROLL_PAPER, IMAGE_NUM};
	enum{ 
			VIEW_TYPE_FIGURE,				// 이벤트중 인던의 진행단계로 인한 조합으로 보너스 알림.
			VIEW_TYPE_DAILY_RANK_1,			// 하루 랭킹 1위를 알려줌.
			VIEW_TYPE_OPEN_THE_GATE,		// 서버 이벤트 인던문이 열림
			VIEW_TYPE_MESSAGE,				// 서
		};


public:
	NServerInstanceMapBounusDlg();
	~NServerInstanceMapBounusDlg();

	void Create(void);
	void Action(void);
	void DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY );
	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY );

	void SetIstanceMapStepCount(SI16* pData);
	void SetDailyBestServerNotice(void);
	void SetGateState( bool bGateMode );
	void SetCharMessage( TCHAR* pszMessage );
	void SetMessage( SI32 _siTextNum );

	static void CALLBACK StaticServerInstanceMapBounusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
		   void CALLBACK NServerInstanceMapBounusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

private:
	InterfaceFactory	m_InterfaceFactory;

	SI16				m_siData[DATA_NUM];
	DWORD				m_dwOpenTickCount;
	SI32				m_RollPaperSize;

	TSpr				m_Spr[IMAGE_NUM];

    SI16				m_siViewType;
};
