#include ".\dormancyequipitemreward.h"
#include "../../Client/InterfaceMgr/DialogController.h"
#include "../../Client/ninterface/NDormancyAccount/NDormancyArmourBoxDlg.h"
#include "../../Client/Client.h"
#include "../CommonLogic/Msg/MsgType-DormancyAccount.h"

CDormancyEquipItemRewardClient::CDormancyEquipItemRewardClient(void)  : CDormancyEquipItemReward()
{
	m_pclClient = (cltClient *)pclClient;
}

CDormancyEquipItemRewardClient::~CDormancyEquipItemRewardClient(void)
{
	m_pclClient = NULL;
}

//	아이템 선택
BOOL	CDormancyEquipItemRewardClient::SelectItemInBox(SI32 siItemIndex,SI32 siBoxUnique, SI32 siBoxPos)
{
	CDormancyBox*	pclDormancyBox = NULL;
	CDormancyEquipItem*	pclDormancyEquipItem = NULL;

	//	보급 박스 얻기
	if( NULL == (pclDormancyBox = m_pclDormancyBoxMgr->GetDormancyBox(siBoxUnique)))
	{
		return FALSE;
	}

	//	보급 박스 안의 장비 아이템 지급
	if( NULL == (pclDormancyEquipItem = pclDormancyBox->GetEquipItem(siItemIndex)))
	{
		return FALSE;
	}

	SI32 siDlgIndex = 0;

	//	보급 상자(방어구)
	switch(siBoxUnique)
	{
	case	ITEMUNIQUE(13353):
		{
			siDlgIndex = NDORMANCYARMOURBOX_DLG;
			break;
		}
	case	ITEMUNIQUE(13354):
		{
			siDlgIndex = NDORMANCYWEAPONBOX_DLG;
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	// 보급 상자 다이얼로그
	CDialogController* pclDialog = (CDialogController*)m_pclClient->m_pDialog[ siDlgIndex ];
	if(NULL == pclDialog)
		return FALSE;

	// 다이얼로그 제거
	((CNDormancyArmourBoxDlg*)pclDialog)->Destroy();

	cltGameMsgRequest_DormancyTakeOutEquipItemInBox clInfo(siBoxUnique,siBoxPos,pclDormancyEquipItem->GetUnique(),siItemIndex);
	cltMsg clMsg( GAMEMSG_REQUEST_DORMANCY_TAKEOUTEQUIPITEMINBOX, sizeof(clInfo), (BYTE*)&clInfo );
	m_pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	
	
	return TRUE;
}

//장비 아이템 선택
BOOL	CDormancyEquipItemRewardClient::UseBox(SI32 siItemUnique, SI32 siPos)
{
	CDialogController* pclDialog = NULL;

	SI32 siDlgIndex = 0;

	//	보급 상자(방어구)
	switch(siItemUnique)
	{
	case	ITEMUNIQUE(13353):
		{
			siDlgIndex = NDORMANCYARMOURBOX_DLG;
			break;
		}
	case	ITEMUNIQUE(13354):
		{
			siDlgIndex = NDORMANCYWEAPONBOX_DLG;
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	pclDialog = (CDialogController*)m_pclClient->m_pDialog[ siDlgIndex ];
	if( NULL == pclDialog )
	{
		m_pclClient->CreateInterface(siDlgIndex, TRUE);
		pclDialog = (CDialogController*)m_pclClient->m_pDialog[ siDlgIndex ];
		if( NULL == pclDialog )
		{
			return FALSE;
		}

		((CNDormancyArmourBoxDlg*)pclDialog)->SetBoxInfo(siItemUnique, siPos);
	}
	else
	{
		pclDialog->Show(TRUE);
	}
	
	return TRUE;
}