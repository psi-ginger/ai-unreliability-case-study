//---------------------------------
// 2003/5/29 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg-Bank.h"
#include "..\CommonLogic\MsgType-Bank.h"
#include "MsgRval-Define.h"

#include "..\CommonLogic\Bank\Bank-Manager.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

// 계좌를 개설하라는 요청.
void cltServer::DoMsg_GAMEMSG_REQUEST_CREATEBANKACCOUNT(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	cltGameMsgRequest_CreateBankAccount* pclInfo = (cltGameMsgRequest_CreateBankAccount*)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;

	// [추가 : 황진성 2008. 1. 7 => 내구도가 0 일때 은행 개설 불가.]
	cltStrInfo* pStrInfo = pclBankManager->GetStrInfo(villageunique);
	if( pStrInfo == NULL )
	{
		return;
	}
	if( 0 == pStrInfo->clCommonInfo.siStrDura ) 
	{
		// [추가 : 황진성 2008. 1. 9 => 내구도가 0 일때 은행 개설 불가.]
		cltMsg clMsg( GAMEMSG_RESPONSE_CREATEBANKACCOUNT_CANCEL, NULL, NULL );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		return;
	}

	//-----------------------------------
	// DB에 계좌를 개설할 것을 요청한다. 
	//-----------------------------------
	sDBRequest_CreateBankAccount clMsg(id, personid, villageunique);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

// 계좌를 해지하라는 요청.
void cltServer::DoMsg_GAMEMSG_REQUEST_CLOSEBANKACCOUNT(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	cltGameMsgRequest_CloseBankAccount* pclInfo = (cltGameMsgRequest_CloseBankAccount*)pclMsg->cData;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0 || villageunique >= MAX_VILLAGE_NUMBER)return ;

	//---------------------------------
	// DB에 계좌를 해지할 것을 요청한다. 
	//----------------------------------
	sDBRequest_CloseBankAccount clMsg(id, personid, villageunique);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}


void cltServer::DoMsg_GAMEMSG_REQUEST_INPUTTOBANK(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	GMONEY fee = 0;

	SI32 villageunique  = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)
	{
		return ;
	}

	cltGameMsgRequest_GMoney* pclmoney = (cltGameMsgRequest_GMoney*)pclMsg->cData;
			
	// 입금하도록 요구되는 금액이 합당한지 확인한다. 
	GMONEY money = pclmoney->m_money;
	if(money  <= 0)
	{

		cltMoney clmoney;
		clmoney.Init();
		sDBRequest_InputMoneyToBank clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, &clmoney, fee);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);

		return ;
	}
	if(pclCM->CR[id]->pclCI->clIP.GetMoney() < money)
	{
		SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILINPUTMONEY_OUTOFMONEY, 0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	
	//------------------------------------------
	// 수수료를 결정한다. 
	//------------------------------------------		
	if( pclCM->CR[id]->GetCurrentVillageUnique() 
		!= pclCM->CR[id]->pclCI->clBank.clAccountNum.GetVillageUnique() ) {	


		cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
		if(pclinfo == NULL)return ;
		fee = pclinfo->clBankStrInfo.clFee.CalcFee(money);


		if( pclCM->CR[id]->pclCI->clIP.GetMoney() <= money + fee ) {
			SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILINPUTMONEY_NOTENOUGHFEE, 0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}
	}
	

	//-------------------------------------
	//DB로 입금하라는 메시지를 보낸다. 
	//-------------------------------------
	cltMoney clmoney;
	clmoney.Set(money);
	sDBRequest_InputMoneyToBank clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, &clmoney, fee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);
	
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

void cltServer::DoMsg_GAMEMSG_REQUEST_INPUTTOBANKALLMONEY(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	GMONEY money;

		
	money = pclCM->CR[id]->pclCI->clIP.GetMoney();

	if( money == 0 ) return;

	SI32 villageunique  = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0) return ;

	//------------------------------------------
	// 수수료를 결정한다. 
	//------------------------------------------
	GMONEY fee = 0;

	if( pclCM->CR[id]->GetCurrentVillageUnique() 
		!= pclCM->CR[id]->pclCI->clBank.clAccountNum.GetVillageUnique() ) {	
		
		cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
		if(pclinfo == NULL)return ;
		fee = pclinfo->clBankStrInfo.clFee.CalcFee(money);

		money -= fee;

		if( money <= 0 ) {
			SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILINPUTMONEY_NOTENOUGHFEE, 0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}
	}

	//-------------------------------------
	//DB로 입금하라는 메시지를 보낸다. 
	//-------------------------------------
	cltMoney clmoney;
	clmoney.Set(money);
	sDBRequest_InputMoneyToBank clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, &clmoney, fee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	pclDelMoneyStatistics->Add(DELMONEY_SERVICECHARGE, fee);

//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

void cltServer::DoMsg_GAMEMSG_REQUEST_OUTPUTFROMBANK(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	SI32 returnval = 0;

	cltGameMsgRequest_GMoney* pclmoney = (cltGameMsgRequest_GMoney*)pclMsg->cData;

	//----------------------------------------------------
	// 출금하도록  요구되는 금액이 합당한지 확인한다. 
	//----------------------------------------------------
	GMONEY money = pclmoney->m_money;
	if(money  <= 0)
	{
		return ;
	}
	// Person이 파악하고 있는 잔액보다 큰 금액은 인출할 수 없다.
	if(pclCM->CR[id]->pclCI->clBank.GetMoney() < money)
	{
		SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHMONEYINACCOUNT, 0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	SI32 villageunique  = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)
	{
		return ;
	}

	//------------------------------------------
	// 수수료를 결정한다. 
	//------------------------------------------
	GMONEY fee = 0;


	if( pclCM->CR[id]->GetCurrentVillageUnique() 
		!= pclCM->CR[id]->pclCI->clBank.clAccountNum.GetVillageUnique() ) {	

		cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
		if(pclinfo == NULL)return ;

		fee = pclinfo->clBankStrInfo.clFee.CalcFee(money);

		if( pclCM->CR[id]->pclCI->clBank.GetMoney() < money + fee ) {
			SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHFEE, 0, 0, pclCM->CR[id]->GetCharUnique());
			return;
		}
	}

	//------------------------------------
	//DB로 출금하라는 메시지를 보낸다. 
	//------------------------------------
	cltMoney clmoney;
	clmoney.Set(money);
	sDBRequest_OutputMoneyFromBank clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, &clmoney, fee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

void cltServer::DoMsg_GAMEMSG_REQUEST_OUTPUTFROMBANKALLMONEY(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	SI32 returnval = 0;

	//----------------------------------------------------
	// 출금하도록  요구되는 금액이 합당한지 확인한다. 
	//----------------------------------------------------

	cltGameMsgRequest_GMoney* pclmoney = (cltGameMsgRequest_GMoney*)pclMsg->cData;

	GMONEY money = pclmoney->m_money;
	if(money  <= 0)
	{
		return ;
	}
	// Person이 파악하고 있는 잔액보다 큰 금액은 인출할 수 없다.
	if(pclCM->CR[id]->pclCI->clBank.GetMoney() < money)
	{
		SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHMONEYINACCOUNT, 0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	SI32 villageunique  = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique <= 0)
	{
		return ;
	}

	//------------------------------------------
	// 수수료를 결정한다. 
	//------------------------------------------
	GMONEY fee = 0;


	if( pclCM->CR[id]->GetCurrentVillageUnique() 
		!= pclCM->CR[id]->pclCI->clBank.clAccountNum.GetVillageUnique() ) {	

			cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
			if(pclinfo == NULL)return ;

			fee = pclinfo->clBankStrInfo.clFee.CalcFee(money);

			if( pclCM->CR[id]->pclCI->clBank.GetMoney() < money ) {
				SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHFEE, 0, 0, pclCM->CR[id]->GetCharUnique());
				return;
			}
		}

		GMONEY requsetMoney = money - fee;
		//------------------------------------
		//DB로 출금하라는 메시지를 보낸다. 
		//------------------------------------
		cltMoney clmoney;
		clmoney.Set(requsetMoney);
		sDBRequest_OutputMoneyFromBank clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), villageunique, &clmoney, fee);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

void cltServer::DoMsg_GAMEMSG_REQUEST_SETBANKFEE(cltMsg* pclMsg, SI32 id)
{
//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	SI32 returnval = 0;

	SI32 PersonID = pclCM->CR[id]->pclCI->GetPersonID();
	
	// Person이 위치한 마을의 Unique를 구한다. 
	SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();
	if(villageunique == 0)return ;
	
	// 수수료 정보를 확보한다. 
	cltFee* pclfee = (cltFee*)pclMsg->cData;
	// 수수료가 적절한지 확인한다. 
	if(pclfee->IsValid(1000) == FALSE)return;
	
	//----------------------------------
	// 요청자가 전장행수인지 확인한다. 
	//----------------------------------
	cltSimpleRank clrank(RANKTYPE_BANK, villageunique);
	if(pclRankManager->IsRank(PersonID, &clrank) == FALSE)
	{
		// 전장행수가 아니라 수수료 설정에 실패. 
		SendServerResponseMsg(RANKTYPE_BANK, SRVAL_BANK_FAILSETFEE_NOTCHIEF,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}
	//-----------------------------------
	// DB로 전장 정보를 보낸다. 
	//-----------------------------------
	// DB 로 보낸다.

	sDBRequest_SetBankFee clMsg(id, PersonID, villageunique, pclfee);
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

//----------------------------------------------------------------
// 서버만 컴파일 됨
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

