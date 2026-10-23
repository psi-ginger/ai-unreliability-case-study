
#include "Char\CharManager\CharManager.h"
#include "..\..\CommonLogic\CommonLogic.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "..\Server.h"

#include "MinisterBase.h"
#include "Minister-Izo.h"

#include "..\..\CommonLogic\MsgRval-Define.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "..\CommonLogic\MsgType-System.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"

extern cltCommonLogic* pclClient;

CMinisterIzo::CMinisterIzo() : CMinisterBase()
{
	m_siPriceValue = 70;
	m_siPriceTime = 0 ;
	m_siTotalMoney = 0 ;
	m_siCalcTime = 0 ;
	m_siTradeTax = 5 ;
	m_siTradeLimitMoney = 10000000 ;

	m_bWorldTradeTax = true;
	m_siSpecialTaxRange = Const_SpecialTaxRange_Min;
	m_siSpecialTaxRate = Const_SpecialTaxRate_Min;

	m_siMarketDownRate = Const_MarketDownRate_Min;
}

CMinisterIzo::~CMinisterIzo()
{
}

// DB에 이조의 정보를 요청한다.
void CMinisterIzo::InitializeDB()
{
	// 이조의 정보를 요청한다.
	sDBRequest_GetIzoInfo clMsg;
	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

}

// DB에서 이조의 정보를 받으면 값을 Class에 설정한다.
void  CMinisterIzo::DoMsg_DBMsgResponse_GetMinisterInfo(sPacketHeader* pPacket)
{

	sDBResponse_GetIzoInfo* pclMsg = (sDBResponse_GetIzoInfo *)pPacket;

	if(pclMsg->siResult)
	{
		StringCchCopy(m_szAccountID, MAX_PLAYER_NAME,	pclMsg->MinisterBase.m_szAccountID);
		StringCchCopy(m_szPersonName,	MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szPersonName);
		StringCchCopy(m_szGreeting,	256, pclMsg->MinisterBase.m_szGreeting);

		// 매입율은 50보다 크며 75보다 작다
		m_siPriceValue =	pclMsg->siPriceValue;
		if(m_siPriceValue<50 || m_siPriceValue>75)	m_siPriceValue = 70;

		m_siPriceTime =		pclMsg->siPriceTime;
		if(m_siPriceTime <= 0 ) m_siPriceTime = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() - 30 ;

		m_siTotalMoney =	pclMsg->siTotalMoney;
		if ( m_siTotalMoney < 0 ) m_siTotalMoney = 0 ;
		
		m_siCalcTime =		pclMsg->siCalcTime;
		if ( m_siCalcTime <= 0 ) m_siCalcTime = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary() ;
		
		// 관세는 5에서 20까지이다.
		m_siTradeTax = pclMsg->siTradeTax ;
		if(m_siTradeTax < 5 || m_siTradeTax > 20 ) m_siTradeTax = 5 ;

		// 최소매입가는 100만냥에서 1000만냥이다.
		m_siTradeLimitMoney = pclMsg->siTradeLimitMoney ;
		if( m_siTradeLimitMoney < 1000000 || m_siTradeLimitMoney > 10000000 ) m_siTradeLimitMoney = 10000000 ;

		m_bWorldTradeTax = pclMsg->bWorldTradeTax;			// 무역_세금 적용 유무

		m_siSpecialTaxRange = pclMsg->siSpecialTaxRange;		// 무역_특별세 적용범위( 100% ~ 150% )
		if(m_siSpecialTaxRange < Const_SpecialTaxRange_Min || m_siSpecialTaxRange > Const_SpecialTaxRange_Max )
			m_siSpecialTaxRange = Const_SpecialTaxRange_Min;

		m_siSpecialTaxRate = pclMsg->siSpecialTaxRate;			// 무역_특별세율( 10% ~ 100% )
		if(m_siSpecialTaxRate < Const_SpecialTaxRate_Min || m_siSpecialTaxRate > Const_SpecialTaxRate_Max )
			m_siSpecialTaxRate = Const_SpecialTaxRate_Min;

		m_siMarketDownRate = pclMsg->siMarketDownRate;		//  마켓 자동할인 시스템 할인율 ( 3% ~ 15% )
		if(m_siMarketDownRate < Const_MarketDownRate_Min || m_siMarketDownRate > Const_MarketDownRate_Max )
			m_siMarketDownRate = Const_MarketDownRate_Min;

		m_siTinTin = pclMsg->siTinTin;

	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetIzoInfo"));
}

// Client에서 이조의 정보를 요청하면 지금 Class에 있는 정보를 전송한다.
void CMinisterIzo::DoMsg_GameMsgRequest_GetMinisterInfo(SI32 siID)
{
	CMinisterBase *pMinister = (CMinisterBase* )(this);
	cltGameMsgResponse_GetIzoInfo clinfo(pMinister,
		((cltServer *)pclClient)->pclItemManager->clItemStatistic.siItemPriceIndex,
		m_siPriceValue,m_siPriceTime,m_siTotalMoney,m_siCalcTime, m_siTradeTax, m_siTradeLimitMoney
		,m_bWorldTradeTax, m_siSpecialTaxRange, m_siSpecialTaxRate, m_siMarketDownRate, m_siTinTin );

	cltMsg clMsg(GAMEMSG_RESPONSE_GETIZOINFO, sizeof(clinfo), (BYTE*)&clinfo);
	((cltServer *)pclClient)->pclCM->CR[siID]->SendNetMsg((sPacketHeader*)&clMsg);

}

// Client에서 이조의 정보 새로 설정하기를 원하면 서버에서는 DB에 정보 설정을 요청한다.
void CMinisterIzo::DoMsg_GameMsgRequest_SetMinisterInfo(cltMsg* pclMsg, SI32 siID)
{
	cltGameMsgRequest_SetIzoInfo *pclinfo = (cltGameMsgRequest_SetIzoInfo*)pclMsg->cData;
	
	// 한달 이내에는 재처리하지 않는다. 
	SI32 curdate = ((cltServer*)pclClient)->pclTime->clDate.GetDateVary();
	if(m_siPriceTime + 30 >  curdate)return ;

	sDBRequest_SetIzoInfo clMsg(siID, ((cltServer *)pclClient)->pclCM->CR[siID]->pclCI->GetPersonID(),
		pclinfo->szGreeting, pclinfo->siPriceValue, curdate, pclinfo->bWorldTradeTax,
		pclinfo->siSpecialTaxRange, pclinfo->siSpecialTaxRate, pclinfo->siMarketDownRate,
		pclinfo->siTinTin);

	((cltServer *)pclClient)->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
}

// DB에서 정보의 쓰기가 완료되면 Client에게 다시 전송하여준다.
void CMinisterIzo::DoMsg_DBMsgResponse_SetMinisterInfo(sPacketHeader* pPacket)
{
	sDBResponse_SetIzoInfo* pclMsg = (sDBResponse_SetIzoInfo*)pPacket;
	SI32 id = pclMsg->usCharID;

	if(((cltServer *)pclClient)->pclCM->IsAlive(id) == false)return ;

	if(pclMsg->siResult == 1)
	{
		// 동일인이라면 
		if(((cltServer *)pclClient)->pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
		{
			StringCchCopy(m_szAccountID,	MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szAccountID);
			StringCchCopy(m_szPersonName,	MAX_PLAYER_NAME, pclMsg->MinisterBase.m_szPersonName);
			StringCchCopy(m_szGreeting,	256, pclMsg->MinisterBase.m_szGreeting);

			// 매입율은 50보다 크며 75보다 작다
			m_siPriceValue		= pclMsg->siPriceValue;
			if(m_siPriceValue<50 || m_siPriceValue>75)	m_siPriceValue = 70;
			m_siPriceTime		= pclMsg->siPriceTime;

			// 관세는 5보다 크며 20보다 작다
			m_siTradeTax		= pclMsg->siTradeTax;
			if(m_siTradeTax< 5 || m_siTradeTax > 20 ) m_siTradeTax = 5 ;

			// 최소매입가는 100만냥에서 1000만냥이다.
			m_siTradeLimitMoney = pclMsg->siTradeLimitMoney ;
			if( m_siTradeLimitMoney < 1000000 || m_siTradeLimitMoney > 10000000 ) m_siTradeLimitMoney = 10000000 ;

			m_bWorldTradeTax = pclMsg->bWorldTradeTax;				// 무역_세금 적용 유무

			m_siSpecialTaxRange = pclMsg->siSpecialTaxRange;		// 무역_특별세 적용범위( 100% ~ 150% )
			if(m_siSpecialTaxRange < Const_SpecialTaxRange_Min || m_siSpecialTaxRange > Const_SpecialTaxRange_Max )
				m_siSpecialTaxRange = Const_SpecialTaxRange_Min;

			m_siSpecialTaxRate = pclMsg->siSpecialTaxRate;			// 무역_특별세율( 10% ~ 100% )
			if(m_siSpecialTaxRate < Const_SpecialTaxRate_Min || m_siSpecialTaxRate > Const_SpecialTaxRate_Max )
				m_siSpecialTaxRate = Const_SpecialTaxRate_Min;

			m_siMarketDownRate = pclMsg->siMarketDownRate;			//  마켓 자동할인 시스템 할인율 ( 3% ~ 15% )
			if(m_siMarketDownRate < Const_MarketDownRate_Min || m_siMarketDownRate > Const_MarketDownRate_Max )
				m_siMarketDownRate = Const_MarketDownRate_Min;

			m_siTinTin = pclMsg->siTinTin;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 10] // 이조설정.
				// param1    == 매입율, param2    == PriceTime, param3 == 무역세, param4 == 무역 제한 금액, 
				// pszParam1 == 계정,   pszParam2 == 이름.
				cltServer * pclserver = (cltServer*)pclClient;
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

				TCHAR szCharParam1[50] = { '\0', };
				TCHAR szCharParam2[50] = { '\0', };

				StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("ID:%s, NAME:%s"), pclMsg->MinisterBase.m_szAccountID, pclMsg->MinisterBase.m_szPersonName );
				StringCchPrintf(szCharParam2, sizeof(szCharParam2), TEXT("TRADE [ %d - Range:%d, TaxRate:%d ], MARKET [ DownRate:%d ]"), pclMsg->bWorldTradeTax, pclMsg->siSpecialTaxRange, pclMsg->siSpecialTaxRate, pclMsg->siMarketDownRate );

				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_IZO_SET, 
					0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					pclMsg->siPriceValue, pclMsg->siPriceTime, 
					pclMsg->siTradeTax, pclMsg->siTradeLimitMoney, 0, 
					szCharParam1, szCharParam2 );
			}
		}
	}
	else
	{
		// 서버 Error
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETIZOINFO,  0, 0, id);
	}
	
	// 정보를 요청했을때와 같이 돌려준다.
	DoMsg_GameMsgRequest_GetMinisterInfo(id);

}
