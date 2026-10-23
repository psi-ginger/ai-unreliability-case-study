#ifndef _NDUNGEONRUNINFO_H
#define _NDUNGEONRUNINFO_H

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../CommonLogic/Msg/MsgType-Hunt.h"

//#define SPR_CNT	3

class InstanstDungeonRunInfoDlg : public CDialogController
{
	enum{ 
		DRAW_MODE_GUILDMAP_TIME			= 0
		,DRAW_MODE_GUILDMAP_SCORE
		,DRAW_MODE_GUILDMAP_BEST_MARK

		,DRAW_MODE_PARTYMAP_BOARD
		,DRAW_MODE_PARTYMAP_TIME
		,DRAW_MODE_PARTYMAP_KILLCOUNT

		// 이 위에는 배열로 잡아야 하는 것들을 정의하시면 됩니다

		,DRAW_MODE_END	// 여기 항상 고정

		// 이 아래는 배열로 만들 필요없는 것들을 정의하시면 됩니다
		,DRAW_MODE_GUILDMAP_GUILD_MARK	= 100
	};

private:
	InterfaceFactory	m_InterfaceFactory;

	TSpr				m_Spr[DRAW_MODE_END];					// 길드 마크.

	DWORD				m_dwTime;
	DWORD				m_dwStartTick;
	SI32				m_siScore;
	SI32				m_siBestScore;

	SI16				m_siMapType;
	
public:
	InstanstDungeonRunInfoDlg();
	~InstanstDungeonRunInfoDlg();

	void Create();
	void Destroy();

	static void CALLBACK StaticInstanstDungeonRunInfoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NInstanstDungeonRunInfoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );
	
	void DrawSpr( SI32 _ScreenX, SI32 _ScreenY, SI32 _Figure, SI32 _SprType  );

	void Init(cltGameMsgResponse_cInstantsDungeonRunInfo* _RunInfo);
};

















#endif