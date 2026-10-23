
#pragma once

#include "../../InterfaceMgr/DialogController.h"
#include "../../common/SystemNPC/RPMerchantNPC/RPMerchantNPC.h"


class CNRPList : public CDialogController
{
public:

	CNRPList();
	~CNRPList();

	void	Initialize();
	void	Destroy();

	void	Create( CControllerMgr *pParent );

	static void CALLBACK StatiCNRPListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl);
	void CALLBACK NRPListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl);

	void	SetRecommandPointUserList( stRPListData * pRPListData  );
	
	SI32	GetAccuRPFromLevel( SI32 Level );
	
	void	Hide();
	void	Show();

	InterfaceFactory m_InterfaceFactory;

private:
	SI16	m_CurrentPage;	 
	SI32	m_SelectedListIndex;
	stRecommandProduct	m_RecommandProduct[MAX_RECOMMAND_PRODUCT];


};

