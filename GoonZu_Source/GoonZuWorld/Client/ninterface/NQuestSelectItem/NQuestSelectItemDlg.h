#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/Quest/Quest.h"


class CNQuestSelectItemDlg : public CDialogController
{
public:
	CNQuestSelectItemDlg();
	~CNQuestSelectItemDlg();

	void Init();
	void Destroy();

	void Create();

	static void CALLBACK StaticQuestSelectItemDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NQuestSelectItemDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	InterfaceFactory	m_InterfaceFactory;

	void SetItemList( cltQuestRewardInfo* pclinfo, SI16 questtype, SI16 questunique );
	void RecvItem();


	cltQuestRewardInfo* m_pclInfo;
	SI16 m_siQuestType;
	SI16 m_siQuestUnique;
};

