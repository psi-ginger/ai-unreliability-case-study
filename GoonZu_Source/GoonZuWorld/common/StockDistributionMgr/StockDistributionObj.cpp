#include "StockDistributionObj.h"

#include "StockDistributionBaseInfo.h"

#include "CommonLogic.h"
#include "..\..\Client\Client.h"
#include "..\..\Server\Server.h"

#include "MsgType-Stock.h"


#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;


#define MAX_AVAILABLE_FRAME		6000			//9000Frame = ¾à 8 ~ 9 ºÐ

CStockDistributionObj::CStockDistributionObj( SI32 Index ) : CObj()
{
	m_siStockDistributionObjIndex = Index;
		
	Initialize();
}

CStockDistributionObj::~CStockDistributionObj()
{
	Destroy();
}

void CStockDistributionObj::Initialize()
{
	m_siCurrentStatus = STOCK_DISTRIBUTION_STOP_STATUS;
	
	m_siVillageUnique = 0;
	m_siMaxDistributionStockNum = 0;
	m_siDistributionStockUnit = 0;
	m_siDistributionStockPrice = 0;
	m_siCurrentDistributionStockNum = 0;

	m_siCurrentFrame = 0;

	m_bVillageWarStockDistribution = false;

	return;
}

void CStockDistributionObj::Destroy()
{
	return;
}

SI16 CStockDistributionObj::GetCurrentStatus()
{
	return m_siCurrentStatus;
}

SI32 CStockDistributionObj::GetVillageUnique()
{
	if ( m_siCurrentStatus == STOCK_DISTRIBUTION_PLAY_STATUS )
	{
		return m_siVillageUnique;
	}

	return -1;
}

SI32 CStockDistributionObj::Set( SI32 VillageUnique, SI32 MaxDistributionStockNum, SI32 DistributionStockUnit, GMONEY DistributionStockPrice, bool VillageWarStockDistribution, SI32 VillageWarWinVillageUnique )
{
	m_siVillageUnique = VillageUnique;

	m_siMaxDistributionStockNum = MaxDistributionStockNum;
	m_siDistributionStockUnit = DistributionStockUnit;
	m_siDistributionStockPrice = DistributionStockPrice;

	m_bVillageWarStockDistribution = VillageWarStockDistribution;

	m_siCurrentDistributionStockNum = 0;

	m_siCurrentStatus = STOCK_DISTRIBUTION_PLAY_STATUS;

	m_siCurrentFrame = 0;

	if ( m_bVillageWarStockDistribution )
	{
		if ( m_siVillageUnique > 0 )
		{
			cltGameMsgResponse_StockDistributionSet clGameMsgResponse_StockDistributionSet( VillageUnique, MaxDistributionStockNum, DistributionStockUnit, DistributionStockPrice, VillageWarStockDistribution );
			cltMsg clMsg( GAMEMSG_RESPONSE_STOCKDISTRIBUTIONSET, sizeof( clGameMsgResponse_StockDistributionSet ), (BYTE*)&clGameMsgResponse_StockDistributionSet );
			pclClient->pclCM->SendAllPersonHomeMsg( (sPacketHeader*)&clMsg, VillageWarWinVillageUnique );
		}
	}
	else
	{
		cltGameMsgResponse_StockDistributionSet clGameMsgResponse_StockDistributionSet( VillageUnique, MaxDistributionStockNum, DistributionStockUnit, DistributionStockPrice, VillageWarStockDistribution );
		cltMsg clMsg( GAMEMSG_RESPONSE_STOCKDISTRIBUTIONSET, sizeof( clGameMsgResponse_StockDistributionSet ), (BYTE*)&clGameMsgResponse_StockDistributionSet );	
		pclClient->pclCM->SendAllPersonMsg( (sPacketHeader*)&clMsg );
	}

	return m_siStockDistributionObjIndex;
}

bool CStockDistributionObj::Action()
{
	if ( ++m_siCurrentFrame > MAX_AVAILABLE_FRAME )
	{
		return true;
	}

	return false;
}

SI32 CStockDistributionObj::IncreaseDistributionStockNum( SI32 IncreaseDistributionStockNum )
{
	if ( m_siCurrentDistributionStockNum + IncreaseDistributionStockNum > m_siMaxDistributionStockNum )
	{
		IncreaseDistributionStockNum = m_siMaxDistributionStockNum - m_siCurrentDistributionStockNum;
	}

	m_siCurrentDistributionStockNum += IncreaseDistributionStockNum;
	
	if ( m_siCurrentDistributionStockNum >= m_siMaxDistributionStockNum )
	{
		return -1;
	}

	return IncreaseDistributionStockNum;
}

SI32 CStockDistributionObj::GetDistributionStockUnit()
{
	return m_siDistributionStockUnit;
}

GMONEY CStockDistributionObj::GetDistributionStockPrice()
{
	return m_siDistributionStockPrice; 
}

SI32 CStockDistributionObj::GetLeftDistributionStockNum()
{
	return m_siMaxDistributionStockNum - m_siCurrentDistributionStockNum;
}

SI32 CStockDistributionObj::GetMaxDistributionStockNum()
{
	return m_siMaxDistributionStockNum;
}

bool CStockDistributionObj::IsVillageWarStockDistribution()
{
	return m_bVillageWarStockDistribution;
}
