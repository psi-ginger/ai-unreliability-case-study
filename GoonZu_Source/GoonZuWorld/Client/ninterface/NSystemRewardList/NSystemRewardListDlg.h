#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../Common/SystemReward/SystemReward.h"

class CNSystemRewardListDlg : public CDialogController
{
private:
	SI32 m_siCurPage;
	SI32 m_siMaxPage;

public:
	CNSystemRewardListDlg();
	~CNSystemRewardListDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticSystemRewardListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NSystemRewardListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	cltSystemRewardInfo	m_clSystemRewardInfo[MAX_SYSTEMREWARD_LIST];

	void SetRewardList( cltSystemRewardInfo* pclinfo );				//리스트에 데이터 입력.
	void RequestRewardList();			//보상 리스트를 요청한다.
	void RequestRecieve();				//보상을 받아온다.
	void ShowDetailInfo();

	void ExchangeStock();

	void UpdatePageInfo(SI32  siCurPage, SI32  siMaxPage);				// [종호_NHN->NDOORS] 팅팅 페이징 추가
};