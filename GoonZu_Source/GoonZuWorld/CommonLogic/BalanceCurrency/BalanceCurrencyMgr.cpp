#include "balancecurrencymgr.h"

#include "Char/CharManager/CharManager.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-System.h"
#include "../../CommonLogic/MsgRval-Define.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../Lib/dxutil.h"
#include "../../Server/Server.h"

extern cltCommonLogic* pclClient;

CBalanceCurrencyMgr::CBalanceCurrencyMgr(void)
{
	m_siCanUseMoney			=	0;
	m_siTodayCurrency		=	0;
	m_siYesterdayCurrency	=	0;

	m_siBalanceCurrencyID	=	0;

	m_siUpdateState			=	CURRENCY_UPDATE_NONE;

	m_bNeedToCalculateCurrency	=	true;
}


CBalanceCurrencyMgr::~CBalanceCurrencyMgr(void)
{
	m_siCanUseMoney			=	0;
	m_siTodayCurrency		=	0;
	m_siYesterdayCurrency	=	0;
	m_siBalanceCurrencyID	=	0;
}

bool CBalanceCurrencyMgr::CreateBalanceCurrencyNPC()
{
    cltServer* pclServer = (cltServer*)pclClient;

	if (pclServer == NULL)
		return false;


	SI32 siKindOfNPC = pclClient->GetUniqueFromHash("KIND_BALANCECURRENCY");
	if (0 >= siKindOfNPC)
		return false;

	m_siBalanceCurrencyID = pclServer->SetNPCChar(	siKindOfNPC, 
													VILLAGE_NONE, 
													NULL, 
													1, 
													1, 
													BALANCECURRENCY_X, 
													BALANCECURRENCY_Y, 
													MAPINDEX_PRIVATEMARKETAREA, 
													NPCITEMMODE_NONE, 
													NULL );

	if(m_siBalanceCurrencyID != 0)
		return true;
	else
		return false;
}

//	m_siTodayCurrency - m_siYesterdayCurrency + m_siRemainMoney
void CBalanceCurrencyMgr::CalculateBalanceCurrency()
{
	m_siCanUseMoney += GetTodayCurrency() - GetYesterDayCurrency();
}


GMONEY CBalanceCurrencyMgr::GetBuffCost()
{
	SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

	GMONEY	siPrice = 0;

	if		(npcrate < 100)		siPrice	=	150000;
	else if	(npcrate < 300)		siPrice	=	300000;
	else if	(npcrate < 700)		siPrice	=	500000;
	else if	(npcrate < 1000)	siPrice	=	1000000;
	else if	(npcrate < 1450)	siPrice	=	1800000;
	else if	(npcrate < 1750)	siPrice	=	2900000;
	else if	(npcrate < 2050)	siPrice	=	4200000;
	else if	(npcrate < 2350)	siPrice	=	5800000;
	else if	(npcrate < 3000)	siPrice	=	7800000;
	else if (npcrate < 4000)	siPrice	=	10000000;
	else if (npcrate < 5500)	siPrice	=	13000000;
	else if (npcrate < 8500)	siPrice	=	18000000;
	else if (npcrate < 14500)	siPrice	=	25000000;
	else if (npcrate < 25500)	siPrice	=	40000000;
	else if (npcrate < 35500)	siPrice	=	58000000;
	else						siPrice =	85000000;

	return siPrice;
}

void CBalanceCurrencyMgr::Action()
{
	cltServer* pclServer = (cltServer*)pclClient;

	//	이 함수가 호출된다면 서버에서 NPC 관련정보 등록이 완료된 상태이니 여기서 
	//	CreateBalanceCurrencyNPC 함수를 단 한번만 호출해 준다.
	if(m_siBalanceCurrencyID == 0)
	{
		bool bResult = CreateBalanceCurrencyNPC();		//	여기에 들어온다면 서버가 재가동 됏다는 말과 같다.

		if(bResult == false)
			return;
	}

	//	DB로부터 계산에 필요한 오늘과 어제 통화량을 받아온 상태라면
	if(GetUpdateState() == CURRENCY_UPDATE_ALL_DONE)
	{
		//	매일 7시 20분에 통화량 갱신을 한다.
		_SYSTEMTIME sTime;
		GetLocalTime(&sTime);

		if(sTime.wHour == 7 && sTime.wMinute == 20)
		{
			if(m_bNeedToCalculateCurrency == true)
			{
				CalculateBalanceCurrency();

				//[종호] 통화량 조절 - 사용가능 통화량 저장
				sDBRequest_SetBalanceCanUseCurrency clMsg(GetCanUseMoney());	
				pclServer->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				m_bNeedToCalculateCurrency = false;
				SetUpdateState(CURRENCY_UPDATE_ALL_DONE);
			}
		}
		else
			m_bNeedToCalculateCurrency = true;
	}
}
