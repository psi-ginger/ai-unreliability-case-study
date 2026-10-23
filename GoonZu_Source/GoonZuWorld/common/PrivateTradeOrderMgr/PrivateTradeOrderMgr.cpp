#include "PrivateTradeOrderMgr.h"
#include "PrivateTradeOrderObj.h"

#include "..\SMLib\LibList\LibList.h"

#include "../../CommonLogic/Msg/MsgType-PrivateTrade.h"
#include "../../Server/Server.h"

#include "../Char/CharCommon/Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "..\CommonLogic\CommonLogic.h"
extern cltCommonLogic *pclClient;

CPrivateTradeOrderMgr::CPrivateTradeOrderMgr() : CLibListMgr()
{
	CreateEmptyObj();

	Initialize();
}

CPrivateTradeOrderMgr::~CPrivateTradeOrderMgr()
{
	Destroy();
}

void CPrivateTradeOrderMgr::Initialize()
{
	return;
}

void CPrivateTradeOrderMgr::Destroy()
{
	return;
}

void CPrivateTradeOrderMgr::CreateEmptyObj()
{
	CPrivateTradeOrderObj *pTemp = NULL;

	for ( int i = 0; i < MAX_PRIVATETRADEOBJ_NUM; ++i )
	{
		pTemp = new CPrivateTradeOrderObj( i + 1 );		// index 값이 0 인 경우를 피하게 하기 위해서.. 
		PushBackToEmptyList( pTemp );
	}

	return;
}

// 리턴 값이 1 이상의 값이면, 
// 리턴 값이 0 이면 거래가 불가능한 경우임( 허용 불가능 PersonID, 거래 가능개수 초과 )
bool CPrivateTradeOrderMgr::Set( SI32 siPerson1ID, SI32 siPerson2ID, SI16 *psiMgrIndex, SI16 *psiCountIndex, SI16 TradeMode )
{
	if ( siPerson1ID <= 0  || siPerson2ID <= 0 )
	{
		return false;
	}

	//-----------------------------------------------------
	// Person1ID 또는 Person2ID 가 현재 거래중이냐를 체크
	//-----------------------------------------------------
	CPrivateTradeOrderObj *pTemp1 = (CPrivateTradeOrderObj *)GetExistList()->GetFirst();
	
	while( pTemp1 )
	{
		if ( pTemp1->IsTradeStatus( siPerson1ID, siPerson2ID ) )			// 현재 두명중 한명이 거래중이 상태이다.
		{
			return false;
		}

		pTemp1 = (CPrivateTradeOrderObj *)pTemp1->GetNext();
	}



	//-----------------------------------------------------
	// 이 아래 부터는 둘 다 거래중이지 않음을 확신한다.
	//-----------------------------------------------------
	CPrivateTradeOrderObj *pTemp = (CPrivateTradeOrderObj *)PopFrontToEmptyListAndPushBackToExistList();

	if ( pTemp == NULL )		// 거래 가능 개수 초과
	{
		return false;
	}
	
	pTemp->Set( siPerson1ID, siPerson2ID, psiMgrIndex, psiCountIndex, TradeMode );
	
	return true;
}


void CPrivateTradeOrderMgr::Action( cltTime *pTime )
{
	return;
}

CPrivateTradeOrderObj *CPrivateTradeOrderMgr::FindTradeOrderObj( SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = (CPrivateTradeOrderObj *)GetExistList()->GetFirst();

	while( pTemp )
	{
		if ( pTemp->IsSame( siMgrIndex, siCountIndex ) )			// 현재 두명중 한명이 거래중이 상태이다.
		{
			return pTemp;
		}

		pTemp = (CPrivateTradeOrderObj *)pTemp->GetNext();
	}
	
	return NULL;
}

CPrivateTradeOrderObj *CPrivateTradeOrderMgr::FindTradeOrderObj( SI32 siPersonID )
{
	CPrivateTradeOrderObj *pTemp = (CPrivateTradeOrderObj *)GetExistList()->GetFirst();

	while( pTemp )
	{
		if ( pTemp->IsExist( siPersonID ) )			// 거래 상태 중이었다.
		{
			return pTemp;
		}

		pTemp = (CPrivateTradeOrderObj *)pTemp->GetNext();
	}
	
	return NULL;
}

// 리턴 값은 거래 리스트에 추가된 Index
// 주의할 것은 Person1ID 가 아이템을 추가한다는 것임.
// Person2ID 는 거래자가 맞는지 다시한번 확인하기 위해서 하는 것임.
// -1 인 경우 에러, 0 이상 성공
// return 값이 -1 인 아닌 값들은 아이템이 추가된 위치를 뜻함.
SI16 CPrivateTradeOrderMgr::AddItem( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, cltItem *pclItem, SI16 siItemPos )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return -1;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return -1;
	}

	return pTemp->AddItem( siPerson1ID, pclItem, siItemPos );
}

// 주의할 것은 Person1ID 가 돈을 설정한다는 것임.
// Person2ID 는 거래자가 맞는지 다시한번 확인하기 위해서 하는 것임.
bool CPrivateTradeOrderMgr::PrivateTradeSetPrice( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, GMONEY siPrice )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	return pTemp->PrivateTradeSetPrice( siPerson1ID, siPrice );
}

bool CPrivateTradeOrderMgr::SetStock( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 siVillageUnique, SI32 siStockNum )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->SetStock( siPerson1ID, siVillageUnique, siStockNum );

	return true;
}

bool CPrivateTradeOrderMgr::SetHouseUnit( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 HouseVillageUnique, SI32 HouseUnitSlot )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->SetHouseUnit( siPerson1ID, HouseVillageUnique, HouseUnitSlot );

	return true;

}

bool CPrivateTradeOrderMgr::SetHorse( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI16 siHorseIndex, cltHorse *pclTradeHorse )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->SetHorse( siPerson1ID, siHorseIndex, pclTradeHorse );
	
	return true;
}

// 한명이 창을 닫았는 경우
void CPrivateTradeOrderMgr::Cancel( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return;
	}

	pTemp->Initialize();

	GetExistList()->Pop( pTemp );
	PushBackToEmptyList( pTemp );
	
	return;
}

// 한명이 종료 했는 경우
void CPrivateTradeOrderMgr::Cancel( SI32 siPerson1ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	if ( ! pTemp->IsExist( siPerson1ID ) )
	{
		return;
	}

	pTemp->Initialize();

	GetExistList()->Pop( pTemp );
	PushBackToEmptyList( pTemp );

	return;
}

void CPrivateTradeOrderMgr::Cancel( SI32 siPersonID )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siPersonID );

	if ( pTemp == NULL )
	{
		return;
	}

	pTemp->Initialize();

	GetExistList()->Pop( pTemp );
	PushBackToEmptyList( pTemp );

	return;
}

bool CPrivateTradeOrderMgr::GetOrderIndexItem( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siOrderListIndex, SI16 siMgrIndex, SI16 siCountIndex, cltItem* pclItem )
{
	if ( pclItem == NULL )
	{
		return false;
	}

	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}


	// 아이템이 그 위치에 들어 있다면... unique 값이 1 미만의 값이 아니겠지...
	if ( pTemp->GetOrderIndexItem( siPerson1ID, siOrderListIndex, pclItem ) )
	{
		return true;
	}

	return false;
}

bool CPrivateTradeOrderMgr::GetOrderItemList( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, cltItem *pclPerson1Item, cltItem *pclPerson2Item, SI16 *psiPerson1ItemPosList, SI16 *psiPerson2ItemPosList )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->GetOrderItemList( siPerson1ID, pclPerson1Item, psiPerson1ItemPosList );
	pTemp->GetOrderItemList( siPerson2ID, pclPerson2Item, psiPerson2ItemPosList );

	return true;
}

bool CPrivateTradeOrderMgr::DelOrderListIndex( SI32 Person1ID, SI32 Person2ID, SI16 DelType, SI16 Param, SI16 MgrIndex, SI16 CountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( MgrIndex, CountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( Person1ID, Person2ID ) )
	{
		return false;
	}

	return pTemp->DelOrderListIndex( Person1ID, DelType, Param );
}

bool CPrivateTradeOrderMgr::DelOrderIndexItem( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siOrderListIndex, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->DelOrderIndexItem( siPerson1ID, siOrderListIndex );

	return true;
}

bool CPrivateTradeOrderMgr::IsTradeStatus( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	return true;
}

bool CPrivateTradeOrderMgr::SetMyAccept( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->SetMyAccept( siPerson1ID );

	return true;
}

bool CPrivateTradeOrderMgr::SetMyCancel( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->SetMyCancel( siPerson1ID );

	return true;
}

bool CPrivateTradeOrderMgr::IsAllAccept( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	return pTemp->IsAllAccept();	
}

bool CPrivateTradeOrderMgr::IsAcceptStatus( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	return pTemp->IsAcceptStatus( siPerson1ID );
}

SI16 CPrivateTradeOrderMgr::GetOrderIndexItemInventoryPos( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 siOrderIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return 0;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return 0;
	}

	return pTemp->GetOrderIndexItemInventoryPos( siPerson1ID, siOrderIndex );
}

bool CPrivateTradeOrderMgr::IsAvailableMoney( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, GMONEY Person1Money, GMONEY Person2Money )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	if ( ! pTemp->IsAvailableMoney( siPerson1ID, Person1Money ) )
	{
		return false;
	}

	if ( ! pTemp->IsAvailableMoney( siPerson2ID, Person2Money ) )
	{
		return false;
	}

	return true;
}

bool CPrivateTradeOrderMgr::IsAvailableStock( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 Char1ID, SI32 Char2ID )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	if ( ! pTemp->IsAvailableStock( siPerson1ID, Char1ID ) )
	{
		return false;
	}

	if ( ! pTemp->IsAvailableStock( siPerson2ID, Char2ID ) )
	{
		return false;
	}

	return true;
}

bool CPrivateTradeOrderMgr::IsAvailableHorse( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 Char1ID, SI32 Char2ID )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	if ( ! pTemp->IsAvailableHorse( siPerson1ID, Char1ID ) )
	{
		return false;
	}

	if ( ! pTemp->IsAvailableHorse( siPerson2ID, Char2ID ) )
	{
		return false;
	}

	return true;
}

bool CPrivateTradeOrderMgr::IsSameAcceptCounter( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI16 siAcceptCounter )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}


	if ( pTemp->GetAcceptCounter() == siAcceptCounter )
	{
		return true;
	}

	return false;
}

GMONEY CPrivateTradeOrderMgr::GetPersonMoney( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return -1;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return -1;
	}


	return pTemp->GetPersonMoney( siPerson1ID );
}


bool CPrivateTradeOrderMgr::GetStockInfo ( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 *pStockVillageUnique1, SI32 *pStockNum1, SI32 *pStockVillageUnique2, SI32 *pStockNum2 )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->GetStockInfo( siPerson1ID, pStockVillageUnique1, pStockNum1 );
	pTemp->GetStockInfo( siPerson2ID, pStockVillageUnique2, pStockNum2 );

	return true;
}

bool CPrivateTradeOrderMgr::GetHouseUnitInfo ( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI32 *pHouseVillageUnique1, SI32 *pHouseUnit1, SI32 *pHouseVillageUnique2, SI32 *pHouseUnit2 )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->GetHouseUnitInfo( siPerson1ID, pHouseVillageUnique1, pHouseUnit1 );
	pTemp->GetHouseUnitInfo( siPerson2ID, pHouseVillageUnique2, pHouseUnit2 );
	
	return true;
}

bool CPrivateTradeOrderMgr::GetHorseInfo( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex, SI16 *Person1HorseTrade, SI16 *Person2HorseTrade )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	pTemp->GetHorseInfo( siPerson1ID, Person1HorseTrade );
	pTemp->GetHorseInfo( siPerson2ID, Person2HorseTrade );

	return true;
}

void CPrivateTradeOrderMgr::IncreaseAcceptCounter( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return;
	}


	pTemp->IncreaseAcceptCounter();

	return;
}

bool CPrivateTradeOrderMgr::IsAvailableAccept( SI32 siPerson1ID, SI32 siPerson2ID, SI16 siMgrIndex, SI16 siCountIndex )
{
	CPrivateTradeOrderObj *pTemp = FindTradeOrderObj( siMgrIndex, siCountIndex );

	if ( pTemp == NULL )
	{
		return false;
	}

	// 같은 사람이 아닌데, 거래를 할려는 요상한 경우
	if ( ! pTemp->IsTradeStatus( siPerson1ID, siPerson2ID ) )
	{
		return false;
	}

	return pTemp->IsAvailableAccpet();
}


void CPrivateTradeOrderMgr::PrivateTradeCancel( SI32 Person1ID, SI32 Person2ID, SI32 id1, SI32 id2, SI16 MgrIndex, SI16 CountIndex, SI16 Reason )
{
	cltGameMsgResponse_PrivateTrade_MyCancel clPrivateTrade_MyCancel( 1 );
	cltMsg clMsg1( GAMEMSG_RESPONSE_PRIVATETRADE_MYCANCEL, sizeof( clPrivateTrade_MyCancel ), (BYTE*)&clPrivateTrade_MyCancel );
	pclClient->pclCM->CR[id1]->SendNetMsg((sPacketHeader*)&clMsg1);
	
	cltGameMsgResponse_PrivateTrade_OtherCancel clPRivateTrade_OtherCancel( 1 );
	cltMsg clMsg2( GAMEMSG_RESPONSE_PRIVATETRADE_OTHERCANCEL, sizeof( clPRivateTrade_OtherCancel ), (BYTE*)&clPRivateTrade_OtherCancel );
	pclClient->pclCM->CR[id2]->SendNetMsg((sPacketHeader*)&clMsg2);
	
	SetMyCancel( Person1ID, Person2ID, MgrIndex, CountIndex );
	SetMyCancel( Person2ID, Person1ID, MgrIndex, CountIndex );
	
	IncreaseAcceptCounter( Person1ID, Person2ID, MgrIndex, CountIndex );

	cltServer *pclserver = (cltServer *)pclClient;
	pclserver->SendServerResponseMsg(0, Reason,  0, 0, id1);
	pclserver->SendServerResponseMsg(0, Reason,  0, 0, id2);
	
	return;
}