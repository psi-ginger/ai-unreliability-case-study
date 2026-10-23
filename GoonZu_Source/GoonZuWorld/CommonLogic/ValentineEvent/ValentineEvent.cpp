#include "ValentineEvent.h"
#include "../common/NPCManager/NPC.h"

#include "../Client/Client.h"
#include "../Server/Server.h"
#include "Char/CharManager/CharManager.h"
#include "../Client/ninterface/nnpc1/NNPCTradeDlg.h"
#include "../Client/ninterface/nrightmousemenudlg/NRightMouseMenuDlg.h"
#include "../Client/InterfaceMgr/DialogResource.h"
#include "../common/Event/event.h"
#include "../msg/msgtype-ValentineEvent.h"
#include "../Client/ninterface/NNewNotify/NNewNotifyDlg.h"
#include "../CommonLogic/MsgRval-Define.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"

//============================ CValentineEventMgr ==========================================================//

CValentineEventMgr::CValentineEventMgr()
{
	m_pclCommon = pclClient;
	m_bEventPeriod = false;
	m_bHolding = false;

	m_siUniqueOfBestVillage = 0;
	m_siAllBufIndex = 0;
	m_siVillageBufIndex = 0;
	m_bBufTime = false;
}

CValentineEventMgr::~CValentineEventMgr()
{
	Destroy();
}

void CValentineEventMgr::Create()
{
	m_ValentineNPCList.Resize(INIT_NPCCOUNT,INIT_NPCCOUNT);

	CreateValentineNPC();
}

void CValentineEventMgr::Destroy()
{

}

bool CValentineEventMgr::IsBufTime()
{
	return m_bBufTime;
}

bool CValentineEventMgr::IsHolding()
{
	return m_bHolding;
}
bool CValentineEventMgr::IsEventPeriod()
{
	return m_bEventPeriod;
}

bool CValentineEventMgr::IsValentineNPC(SI32 siCharKind)
{
	if( NULL == GetValentineNPC(siCharKind))
		return false;

	return true;
}

CValentineNPC	*CValentineEventMgr::GetValentineNPC(SI32 siCharKind)
{
	if ( 0 == siCharKind)
		return NULL;

	SI32 siMaxNPCCount = m_ValentineNPCList.GetCount();

	for( SI32 siNPCListIndex = 0 ; siNPCListIndex < siMaxNPCCount; siNPCListIndex++)
	{
		CValentineNPC &rclNPC = m_ValentineNPCList.GetData(siNPCListIndex);

		if( rclNPC.GetCharKind() == siCharKind)
			return &rclNPC;
	}

	return NULL;
}


void CValentineEventMgr::GetValentineBuf(cltCharCommon* pclChar, cltBasicAbility &clValentineAbility)
{
	if( NULL == pclChar)
		return;

	if( false == m_bBufTime)
		return;

	GetBufAbility(m_siAllBufIndex, clValentineAbility);
	
	if( pclChar->pclCI->clBI.siHomeVillage <= 0 || pclChar->pclCI->clBI.siHomeVillage > MAX_VILLAGE_NUMBER)
		return;

	if(pclChar->pclCI->clBI.siHomeVillage == m_siUniqueOfBestVillage)
	{
		GetBufAbility(m_siVillageBufIndex, clValentineAbility);
	}
}

void CValentineEventMgr::GetBufAbility(SI32 siBufIndex, cltBasicAbility &clValentineAbility)
{
	SI32 siCount = m_ValentineNPCList.GetCount();

	if ( siBufIndex < 0 || siBufIndex >= siCount)
		return;

	CValentineNPC& rclNPC = m_ValentineNPCList.GetData(siBufIndex);

	switch( rclNPC.GetBuffType())
	{
	case BUF_STATUE_UP_STR:
		{
			clValentineAbility.IncreaseStr(rclNPC.GetBuffValue());
			break;
		}
	case BUF_STATUE_UP_DEX:
		{
			clValentineAbility.IncreaseDex(rclNPC.GetBuffValue());
			break;
		}
	case BUF_STATUE_UP_HAND:
		{
			clValentineAbility.IncreaseHnd(rclNPC.GetBuffValue());
			break;
		}
	case BUF_STATUE_UP_MAG:
		{
			clValentineAbility.IncreaseMag(rclNPC.GetBuffValue());
			break;
		}
	case BUF_STATUE_UP_VIT:
		{
			clValentineAbility.IncreaseVit(rclNPC.GetBuffValue());
			break;
		}
	}
}

void CValentineEventMgr::CreateValentineNPC()
{
	SI32 siMaxNPCCount = m_ValentineNPCList.GetCount();

	for( SI32 siNPCListIndex = 0 ; siNPCListIndex < siMaxNPCCount; siNPCListIndex++)
	{
		CValentineNPC &rclNPC = m_ValentineNPCList.GetData(siNPCListIndex);

		switch(siNPCListIndex)
		{
		case 0:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_SOPHIA"));
				rclNPC.SetBuffType(BUF_STATUE_UP_HAND);
				rclNPC.SetBuffValue(5);
				rclNPC.SetThxTextIndex(15631);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(false);
				break;
			}
		case 1:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_MERIEL"));
				rclNPC.SetBuffType(BUF_STATUE_UP_MAG);
				rclNPC.SetBuffValue(10);
				rclNPC.SetThxTextIndex(15632);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(false);
				break;
			}
		case 2:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_BRAI"));
				rclNPC.SetBuffType(BUF_STATUE_UP_STR);
				rclNPC.SetBuffValue(10);
				rclNPC.SetThxTextIndex(15633);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(true);
				break;
			}
		case 3:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_BELITA"));
				rclNPC.SetBuffType(BUF_STATUE_UP_DEX);
				rclNPC.SetBuffValue(5);
				rclNPC.SetThxTextIndex(15634);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(false);
				break;
			}
		case 4:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_HINEAS"));
				rclNPC.SetBuffType(BUF_STATUE_UP_STR);
				rclNPC.SetBuffValue(5);
				rclNPC.SetThxTextIndex(15635);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(true);
				break;
			}
		case 5:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_JEFFREY"));
				rclNPC.SetBuffType(BUF_STATUE_UP_MAG);
				rclNPC.SetBuffValue(5);
				rclNPC.SetThxTextIndex(15636);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(true);
				break;
			}
		case 6:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_ELIANA"));
				rclNPC.SetBuffType(BUF_STATUE_UP_HAND);
				rclNPC.SetBuffValue(10);
				rclNPC.SetThxTextIndex(15637);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(true);
				break;
			}
		case 7:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_SEBRINO"));
				rclNPC.SetBuffType(BUF_STATUE_UP_VIT);
				rclNPC.SetBuffValue(5);
				rclNPC.SetThxTextIndex(15638);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(false);
				break;
			}
		case 8:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_LLOYD"));
				rclNPC.SetBuffType(BUF_STATUE_UP_VIT);
				rclNPC.SetBuffValue(10);
				rclNPC.SetThxTextIndex(15639);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(true);
				break;
			}
		case 9:
			{
				rclNPC.SetCharKind(m_pclCommon->GetUniqueFromHash("KIND_ARA"));
				rclNPC.SetBuffType(BUF_STATUE_UP_DEX);
				rclNPC.SetBuffValue(10);
				rclNPC.SetThxTextIndex(15640);
				rclNPC.SetReceivedItemCount(0);
				rclNPC.SetIsSexMale(false);
				break;
			}
		}
	}
}

//============================ CValentineEventServerMgr ==========================================================//

CValentineEventServerMgr::CValentineEventServerMgr()
{
	m_pclServer = (cltServer*)pclClient;
	m_bCreatedStore = false;
	m_pclValentineBuffEventManager = NULL;
	m_pclValentineEventProcess = NULL;
	m_bNPCBuyPresentItem = false;
	m_siValentineStoreNum = 0;
}

CValentineEventServerMgr::~CValentineEventServerMgr()
{
	Destroy();
}

void CValentineEventServerMgr::Create()
{
	CValentineEventMgr::Create();

	m_ValentineStoreList.Resize(INIT_STORECOUNT,INIT_STORECOUNT);

	m_pclValentineBuffEventManager = new CValentineBuffEventManager();
	m_pclValentineEventProcess = new CValentineEventProcess();

	if(NULL == m_pclValentineBuffEventManager)
		return;
	if(NULL == m_pclValentineEventProcess)
		return;

	m_pclValentineBuffEventManager->Create(m_ValentineNPCList);
	m_pclValentineEventProcess->Create(*m_pclValentineBuffEventManager, m_ValentineStoreList);
}

BOOL CValentineEventServerMgr::OperationGMCommandEventState()
{
	BOOL rValue = 0;

	if( m_pclValentineEventProcess == NULL)
		return EVALENTINEEVENT_GMCOMMAND_RETURN_ENDVALENTINE;

	if( true == m_bEventPeriod)
	{
		if( true == m_bHolding)
		{
			rValue =  EVALENTINEEVENT_GMCOMMAND_RETURN_CLOSE;
		}
		else
		{
			rValue = EVALENTINEEVENT_GMCOMMAND_RETURN_HOLDON;
		}
		m_pclValentineEventProcess->EnforseOperation();
	}
	else
	{
		rValue = EVALENTINEEVENT_GMCOMMAND_RETURN_ENDVALENTINE;
	}

	return rValue;
}

BOOL CValentineEventServerMgr::OperationGMCommandBufState()
{
	BOOL rValue = 0;

	if( m_pclValentineBuffEventManager == NULL)
		return EVALENTINEEVENT_GMCOMMAND_RETURN_ENDVALENTINE;

	if( true == m_bEventPeriod)
	{
		if( true == m_pclValentineBuffEventManager->ChangeBufStateToStaticBufORRandBuf())
		{
			rValue =  EVALENTINEEVENT_GMCOMMAND_RETURN_BUF_RAND;
		}
		else
		{
			rValue = EVALENTINEEVENT_GMCOMMAND_RETURN_BUF_STATIC;
		}
	}
	else
	{
		rValue = EVALENTINEEVENT_GMCOMMAND_RETURN_ENDVALENTINE;
	}

	return rValue;
}

void CValentineEventServerMgr::Action()
{
	if(NULL == m_pclValentineBuffEventManager)
		return;
	if(NULL == m_pclValentineEventProcess)
		return;

	CheckEventPeriod();

	if( true == m_bEventPeriod && false == m_bCreatedStore )
	{
		m_siValentineStoreNum = 0;
		cltPos clPosition;
		clPosition.Set(266,367);
		CreateValentineStore(m_ValentineStoreList.GetData(m_siValentineStoreNum++), clPosition, 15615);
		clPosition.Set(330,200);
		CreateValentineStore(m_ValentineStoreList.GetData(m_siValentineStoreNum++), clPosition, 15616);
		clPosition.Set(305,40);
		CreateValentineStore(m_ValentineStoreList.GetData(m_siValentineStoreNum++), clPosition, 15617);
		clPosition.Set(280,600);
		CreateValentineStore(m_ValentineStoreList.GetData(m_siValentineStoreNum++), clPosition, 15618);
		clPosition.Set(585,315);
		CreateValentineStore(m_ValentineStoreList.GetData(m_siValentineStoreNum++), clPosition, 15619);
		clPosition.Set(770,220);
		CreateValentineStore(m_ValentineStoreList.GetData(m_siValentineStoreNum++), clPosition, 15620);

		m_bCreatedStore = true;
	}
	else if( false == m_bEventPeriod && true == m_bCreatedStore)
	{
		DestroyValentineStore(m_ValentineStoreList.GetData(--m_siValentineStoreNum));
		DestroyValentineStore(m_ValentineStoreList.GetData(--m_siValentineStoreNum));
		DestroyValentineStore(m_ValentineStoreList.GetData(--m_siValentineStoreNum));
		DestroyValentineStore(m_ValentineStoreList.GetData(--m_siValentineStoreNum));
		DestroyValentineStore(m_ValentineStoreList.GetData(--m_siValentineStoreNum));
		DestroyValentineStore(m_ValentineStoreList.GetData(--m_siValentineStoreNum));

		m_bCreatedStore = false;
	}
	else if( true == m_bEventPeriod && true == m_bCreatedStore)
	{
		m_pclValentineEventProcess->Action(m_bHolding, m_bNPCBuyPresentItem);

		if( true == m_bNPCBuyPresentItem )
			BuyPresentItemByNPC();

		m_pclValentineBuffEventManager->Action();

		GetBufInfo();
	}
}

void CValentineEventServerMgr::BuyPresentItemByNPC()
{
	SI32 siChocolateCount = 0;
	SI32 siCandyCount = 0;

	SI32 siMaxNPCCount = m_ValentineNPCList.GetCount();

	for( SI32 siNPCListIndex = 0 ; siNPCListIndex < siMaxNPCCount; siNPCListIndex++)
	{
		CValentineNPC &rclNPC = m_ValentineNPCList.GetData(siNPCListIndex);
		
		if( CValentineNPC::MAX_ITEMCOUNT_NOTBUYITEM > rclNPC.GetReceivedItemCount())
		{
			if( rclNPC.IsSexMale())
				siChocolateCount = siChocolateCount + CValentineNPC::MAX_ITEMCOUNT_NOTBUYITEM;
			else
				siCandyCount = siCandyCount + CValentineNPC::MAX_ITEMCOUNT_NOTBUYITEM;
		}
	}

	SI32 siMaxStoreCount = m_ValentineStoreList.GetCount();
	if( siMaxStoreCount == 0 )
		return;

	SI32 siStoreIndex = rand() % m_siValentineStoreNum;
	
	for ( SI32 siStoreListIndex = 0; siStoreListIndex < siMaxStoreCount;  siStoreListIndex++)
	{
		CValentineStore& rclValentineStore = m_ValentineStoreList.GetData(siStoreListIndex);
		if( siStoreIndex == siStoreListIndex)
			rclValentineStore.MakeItem(siChocolateCount, siCandyCount);
		else
			rclValentineStore.MakeItem(0, 0);
	}

	m_bNPCBuyPresentItem = false;
}

void CValentineEventServerMgr::SendNewBufInfo(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return;

	if( NULL == m_pclValentineBuffEventManager)
		return;

	m_pclValentineBuffEventManager->SendNewBufInfoToPerson(pclChar);
}

void CValentineEventServerMgr::GetBufInfo()
{
	if(NULL == m_pclValentineBuffEventManager)
		return;

	if( m_pclValentineBuffEventManager->IsBufTime())
	{
		m_bBufTime = true;
		m_siAllBufIndex = m_pclValentineBuffEventManager->GetAllBufIndex();
		m_siVillageBufIndex = m_pclValentineBuffEventManager->GetVillageBufIndex();
		m_siUniqueOfBestVillage = m_pclValentineBuffEventManager->GetUniqueOfBestVillage();
	}
	else
	{
		m_bBufTime = false;
		m_siAllBufIndex = 0;
		m_siVillageBufIndex = 0;
		m_siUniqueOfBestVillage = 0;
	}
}

void CValentineEventServerMgr::GivePresentItem(cltCharServer* pclChar,SI32 siNPCKind, cltItem &clItem, SI32 siInvenIndexOfPresentItem)
{
	if( NULL == pclChar)
		return;
	if( NULL == m_pclServer->pclGameDBProcess)
		return;
	if( siInvenIndexOfPresentItem < 0 || siInvenIndexOfPresentItem >= MAX_ITEM_PER_PERSON )
		return;

	//---------------------------------------------
	// 예약 되어 있는 아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclChar->ItemUseReserveAction();

	//---------------------------------------------
	// 예약 되어 있는 탄환아이템 사용이 있다면 처리한다. 
	//---------------------------------------------
	pclChar->BulletUseReserveAction();

	cltPItemInfo &rclPItemInfo =  pclChar->pclCI->clCharItem;
	
	cltItem &rclItem = rclPItemInfo.clItem[siInvenIndexOfPresentItem];
	cltItem clItemSource;
	clItemSource.Set(&rclItem);

	if( clItemSource.siUnique != clItem.siUnique)
		return;
	
	if( clItemSource.siUnique != ITEMUNIQUE(3935) && clItemSource.siUnique != ITEMUNIQUE(3936))
		return;

	if( clItemSource.siItemNum < clItem.siItemNum)
		return;
	if( clItem.siItemNum < 0)
		return;

	if( true ==pclChar->IsExistItemOnOpeningPersonShop(siInvenIndexOfPresentItem))
		m_pclServer->SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, pclChar->GetCharID() );

	if( 0 != pclChar->GetNoMoveItemSwitch(siInvenIndexOfPresentItem))
		m_pclServer->SendServerResponseMsg(0, SRVAL_CANNOTMOVEITEM_PERSONALSHOP, 0, 0, pclChar->GetCharID() );

	CValentineNPC *pclNPC = GetValentineNPC(siNPCKind);
	if( NULL == pclNPC)
		return;

	pclNPC->ReceiveItem(pclChar->pclCI->clBI.siHomeVillage,clItemSource.siItemNum - clItem.siItemNum);
	
	cltGameMsgResponse_ValentineEventGivePresentToNPC clSendMsg(pclNPC->GetCharKind());
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_GIVE_PRESENT_TONPC, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	pclChar->SendNeighbourMsg(&clMsg, true);

	if( clItem.siItemNum != 0 )
	{
		sDBRequest_ExchangeItem clSendDBMsg(pclChar->pclCI->GetPersonID(), siInvenIndexOfPresentItem, &clItemSource, &clItem, EXCHANGEITEM_REASON_GIVEPRESENT);
		m_pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clSendDBMsg);
	}
	else
	{
		sDBRequest_OutputItemFromPerson clSendDBMsg(pclChar->GetCharID(), pclChar->pclCI->GetPersonID(), siInvenIndexOfPresentItem, &clItemSource);
		m_pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clSendDBMsg);
	}

	if(m_pclServer->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		m_pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_PRESENT_ITEM_TONPC, 
			0, pclChar, NULL, 0, 0, 0, 
			&clItemSource, clItemSource.siItemNum - clItem.siItemNum, 0, siNPCKind, 0, 0, 0, 
			siInvenIndexOfPresentItem, 0, 0, 0, 0, NULL, NULL);
	}
	
}

bool CValentineEventServerMgr::OperationGMCommandBuyPresent(SI32 siNPCCharUnique,SI32 siItemUnique,SI32 siItemCount)
{
	if( false == m_bHolding)
	{
		return false;
	}

	return BuyPresentToStore(siNPCCharUnique, siItemUnique, siItemCount);
}

SI32 CValentineEventServerMgr::GetRemainedItemCount(SI32 siNPCCharUnique,SI32 siItemUnique)
{
	CValentineStore *pclStore = FindStoreByCharUnique(siNPCCharUnique);
	
	if( NULL == pclStore)
		return 0;

	if( ITEMUNIQUE(3935) == siItemUnique)
	{
		return pclStore->GetRemainedChocolateCount();
	}
	else if( ITEMUNIQUE(3936) == siItemUnique)
	{
		return pclStore->GetRemainedCandyCount();
	}

	return 0;
}

bool CValentineEventServerMgr::BuyPresentToStore(SI32 siCharUnique ,SI32 siItemUnique, SI32 siItemCount)
{
	if(siCharUnique <= 0)return false;
	if(siCharUnique >= MAX_CHAR_NUMBER)return false;

	if( siItemUnique <= 0)
		return false;

	CValentineStore *pclStore = FindStoreByCharUnique(siCharUnique);

	if(NULL == pclStore)
		return false;

	if( ITEMUNIQUE(3935) == siItemUnique)
	{
		if(pclStore->SellChocolate(siItemCount))
			return true;
	}
	else if( ITEMUNIQUE(3936) == siItemUnique)
	{
		if(pclStore->SellCandy(siItemCount))
			return true;
	}

	return false;
}

void CValentineEventServerMgr::DestroyValentineStore(CValentineStore &rclValentineStore)
{
	cltCharServer* pclStore = m_pclServer->pclCM->GetCharServer(m_pclServer->pclCM->GetIDFromCharUnique(rclValentineStore.GetCharUnique()));

	if( NULL == pclStore)
	{
		return;
	}

	SI32 siStoreKind = m_pclServer->GetUniqueFromHash("KIND_CHOCOLATE");

	if (pclStore->GetKind() != siStoreKind)
	{
		return;
	}

	pclStore->SetLife(0);
	rclValentineStore.Destroy();
}

void CValentineEventServerMgr::CreateValentineStore(CValentineStore &rclValentineStore, cltPos& rclPosition, SI32 siStoreNameTextUnqiue)
{
	cltCharServer* pclStore = m_pclServer->pclCM->GetCharServer(m_pclServer->pclCM->GetIDFromCharUnique(rclValentineStore.GetCharUnique()));
	
	if( NULL != pclStore)
	{
		return;
	}
	
	SI32 siStoreKind = m_pclServer->GetUniqueFromHash("KIND_CHOCOLATE");

	SI32 siStoreCharID =  m_pclServer->SetNPCChar(siStoreKind, VILLAGE_NONE, NULL, 1, 0, rclPosition.siX, rclPosition.siY, 0, NPCITEMMODE_NONE, NULL );

	if( !m_pclServer->pclCM->IsValidID(siStoreCharID))
	{
		MsgBox(TEXT("ValentineEvent"), TEXT("Error Create ValentineStore"));
		return;
	}

	rclValentineStore.Create( siStoreCharID,siStoreNameTextUnqiue);
	rclValentineStore.MakeItem(0,0);
}

CValentineStore* CValentineEventServerMgr::FindStoreByCharUnique(SI32 siCharUnique)
{
	SI32 siMaxStorList = m_ValentineStoreList.GetCount();

	for ( SI32 siStoreListIndex = 0; siStoreListIndex < siMaxStorList;  siStoreListIndex++)
	{
		CValentineStore& rclValentineStore = m_ValentineStoreList.GetData(siStoreListIndex);
		if( rclValentineStore.GetCharUnique() == siCharUnique)
		{
			return &rclValentineStore;
		}
	}
	return NULL;
}

void CValentineEventServerMgr::CheckEventPeriod()
{
	if( m_pclServer->pclEventTimeManager->InEventTime("ValentineEvent", &m_pclServer->sTime) )
	{
		m_bEventPeriod = true;
	}
	else
	{
		m_bEventPeriod = false;
		m_bHolding = false;
	}
}


void CValentineEventServerMgr::Destroy()
{
	if ( NULL != m_pclValentineBuffEventManager)
	{
		delete m_pclValentineBuffEventManager;
		m_pclValentineBuffEventManager = NULL;
	}
	if ( NULL != m_pclValentineEventProcess)
	{
		delete m_pclValentineEventProcess;
		m_pclValentineEventProcess = NULL;
	}
}

void CValentineEventServerMgr::SendBufInfo(cltCharServer* pclChar)
{
	if( NULL == pclChar)
		return;

	cltGameMsgResponse_ValentineEventBuf clSendMsg(m_bBufTime, m_siAllBufIndex, m_siVillageBufIndex, m_siUniqueOfBestVillage);
	cltMsg clMsg(GAMEMSG_RESPONSE_VALENTINEEVENT_BUF, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}

//============================ CValentineEventClientMgr ==========================================================//

CValentineEventClientMgr::CValentineEventClientMgr()
{
	m_pclClient = (cltClient*)pclClient;
	m_siSelectedCharKind = 0;
}

CValentineEventClientMgr::~CValentineEventClientMgr()
{
	Destroy();
}

void CValentineEventClientMgr::Create()
{
	CValentineEventMgr::Create();
}

void CValentineEventClientMgr::Destroy()
{

}

void CValentineEventClientMgr::ShowPresentThatStoreSell(SI32 siStoreNameTextUnique)
{
	if(( m_pclClient->m_pDialog[ NNPCTRADE1_DLG ]) == NULL)
	{
		m_pclClient->CreateInterface( NNPCTRADE1_DLG );
		if( m_pclClient->m_pDialog[ NNPCTRADE1_DLG ] != NULL)
		{	
			((CNNPCTradeDlg*)m_pclClient->m_pDialog[ NNPCTRADE1_DLG ])->SetTitle(GetTxtFromMgr(siStoreNameTextUnique));
			((CNNPCTradeDlg*)m_pclClient->m_pDialog[ NNPCTRADE1_DLG ])->Show(m_pclClient->GetUniqueFromHash("KIND_CHOCOLATE"));
		}
	} 

	else
	{
		((CNNPCTradeDlg*)m_pclClient->m_pDialog[ NNPCTRADE1_DLG ])->DeleteDialog(); 
	}
}

void CValentineEventClientMgr::ShowMsgStoreIsReadyForOpening(SI32 siStoreNameTextUnique)
{
	TCHAR * pszTitle = GetTxtFromMgr(siStoreNameTextUnique);
	TCHAR * pszText = GetTxtFromMgr(15622);
	if ( !pszTitle || !pszText ) return ;
	if(( m_pclClient->m_pDialog[ NOTIFYMSG_DLG ]) != NULL)
	{
		(m_pclClient->m_pDialog[ NOTIFYMSG_DLG ])->SetText(pszTitle, pszText );
	}	
}

void CValentineEventClientMgr::ClickNPC(SI32 siCharUnique)
{
	if(siCharUnique <= 0)return;
	if(siCharUnique >= MAX_CHAR_NUMBER)return;

	cltGameMsgRequest_ValentineEventIsHolding clSendMsg(siCharUnique);
	cltMsg clMsg(GAMEMSG_REQUEST_VALENTINEEVENT_ISHOLDING, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	m_pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CValentineEventClientMgr::ShowGivePresentMenu(cltCharClient *pclTargetNPC)
{
	if ( NULL == pclTargetNPC)
		return;

	SI16 siMode = RIGHTMOUSEMENU_ONLY_GIVEPRESENT;

	if ( m_pclClient->m_pDialog[ NRIGHTMOUSEMENU_DLG ] == NULL )
	{
		m_pclClient->CreateInterface( NRIGHTMOUSEMENU_DLG  );
	}

	if ( m_pclClient->m_pDialog[ NRIGHTMOUSEMENU_DLG ] )
	{
		((CNRightMouseMenuDlg *)m_pclClient->m_pDialog[ NRIGHTMOUSEMENU_DLG ])->Set( pclTargetNPC->GetCharID(), pclTargetNPC->GetCharUnique(),m_pclClient->GameMouseX-10, m_pclClient->GameMouseY-40, siMode,pclTargetNPC );
	}

	SetSeletedNPC(pclTargetNPC->GetKind());
}

void CValentineEventClientMgr::HoldEvent()
{
	CValentineStore::AdvertiseOpening();
}
void CValentineEventClientMgr::CloseEvent(SI32 siNPCKind, SI32 siVillageUnique)
{
	CValentineNPC	*pclNPC = GetValentineNPC(siNPCKind);

	if( NULL == pclNPC)
		return;

	pclNPC->ShowImpression(siVillageUnique);
}

void CValentineEventClientMgr::SetSeletedNPC(SI32 siCharKind)
{
	m_siSelectedCharKind = siCharKind;
}
CValentineNPC *CValentineEventClientMgr::GetSeletedNPC()
{
	return GetValentineNPC(m_siSelectedCharKind);
}

void CValentineEventClientMgr::GivePresentItem(cltItem &clItem, SI32 siInvenIndexOfPresentItem)
{
	cltGameMsgRequest_ValentineEventGivePresentToNPC clSendMsg(m_siSelectedCharKind, &clItem, siInvenIndexOfPresentItem);
	cltMsg clMsg(GAMEMSG_REQUEST_VALENTINEEVENT_GIVE_PRESENT_TONPC, sizeof(clSendMsg), (BYTE*)&clSendMsg);
	m_pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CValentineEventClientMgr::ShowEmoticonWhenNpcReceivedItem(cltCharClient* pclNPCChar)
{
	if( NULL == pclNPCChar)
		return;

	const SI32 EMOTICONTYPECOUNT = 3;
	SI32 siEmoticonIndex = rand() % EMOTICONTYPECOUNT;
	pclNPCChar->SetValentineEmoticon(siEmoticonIndex);
}

void CValentineEventClientMgr::ReceiveBuf(bool bBufTime, SI32 siAllBufIndex,SI32 siVillageBufIndex, SI32 siUniqueOfBestVillage)
{
	m_siUniqueOfBestVillage = siUniqueOfBestVillage;
	m_siAllBufIndex = siAllBufIndex;
	m_siVillageBufIndex = siVillageBufIndex;
	m_bBufTime = bBufTime;
}

void CValentineEventClientMgr::ChangedBufInfo(sValentineBuf *sValentineBufList)
{
	if( NULL == sValentineBufList)
		return;

	for( SI32 siBufListIndex = 0; siBufListIndex < INIT_NPCCOUNT ; siBufListIndex++)
	{
		CValentineNPC &clNPC = m_ValentineNPCList.GetData(siBufListIndex);

		clNPC.SetBuffType(sValentineBufList[siBufListIndex].m_siBufType);
		clNPC.SetBuffValue(sValentineBufList[siBufListIndex].m_siBuyValue);
	}
}
