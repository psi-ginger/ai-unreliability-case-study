#ifndef _NDUNGEONSTATUS_H
#define _NDUNGEONSTATUS_H

#pragma once

#include "../../Client/client.h"
#include "../../InterfaceMgr/DialogController.h"
#include "../../CommonLogic/Msg/MsgType-Hunt.h"

#define GUILD_MARK_CNT	6


class InstanstDungeonStatusDlg : public CDialogController
{
private:
	InterfaceFactory	m_InterfaceFactory;

	TSpr				m_Spr;					// 길드 마크.
	
	bool				m_bIsInit;

public:
	InstanstDungeonStatusDlg();
	~InstanstDungeonStatusDlg();

	void Create();
	void Destroy();
	
	static void CALLBACK StaticInstanstDungeonStatusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NInstanstDungeonStatusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  );
	void RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen );

	void InitList(void);
	void InitList(cltGameMsgResponse_InstantsDungeonStatus* _GuildInfo);

	void DrawGuildMark(CList* pList, SI32 _ScreenRX, SI32 _ScreenRY, SI32 _StartX, SI32 _StartY, SI32 _ColumnSize, SI32 _ColumnCnt);
};




#endif