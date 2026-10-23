#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/RacoonDodgeEvent/RacoonDodgeEvent.h"
#include "../../CommonLogic/Msg/MsgType-RacoonDodgeEvent.h"

class NRacoonDodgeEventReserveListDlg : public CDialogController
{
public:
	NRacoonDodgeEventReserveListDlg();
	~NRacoonDodgeEventReserveListDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticRacoonDodgeEventReserveListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NRacoonDodgeEventReserveListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	ReserveData	m_clReserveListInfo[MAX_RESERVER_SHOW_NUM];

	void GetReserveList( SI32 siPersonID );					//리스트에 넣을 데이터를 요청한다
	void SetReserveList();				
	void SetListData( cltGameMsgResponse_ReserveList * pMsg );//리스트에 데이터 입력
};