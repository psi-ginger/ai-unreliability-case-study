//---------------------------------
// 2003/8/5 김태곤
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\..\CommonLogic\Bank\Bank-Manager.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\..\Common\Map\Map.h"
#include "Char\CharManager\CharManager.h"

#include "..\../../Server/AuthServer/AuthMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Bank.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Stock.h"

#include "MsgRval-Define.h"
#include "MsgType-Stock.h"


// 계좌를 개설할 것. 
void cltServer::DoMsg_DBMSG_RESPONSE_CREATEBANKACCOUNT(sPacketHeader* pPacket)
{
	sDBResponse_CreateBankAccount* pclMsg = (sDBResponse_CreateBankAccount*)pPacket;

	SI32 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE)return ;

	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
		returnval = SRVAL_BANK_FAILCREATEACCOUNT;
		break;
	case 1:
		{
			//------------------------------
			// 계좌 정보 업데이트. 
			//------------------------------
			// 계좌 정보를 확보한다. 
			pclCM->CR[id]->pclCI->clBank.Set(&pclMsg->clBankAccount);
			
			// 클라이언트로 정보를 보낸다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);

			returnval = SRVAL_BANK_SUCCESSCREATEACCOUNT;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 9. 4] // 통장 개설 쓰기.
				// param1 == 계좌번호.
				cltCharServer* pclchar = NULL;
				if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BANK, LOGCOMMAND_INDEX_BANK_CREATEBANKACCOUNT, 
										0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->clBankAccount.clAccountNum.GetVillageUnique(), 0, 0, 
										pclMsg->clBankAccount.clAccountNum.siAccountNum, 0, 0, 0, 0, NULL, NULL);
			}
		}
		break;
	case -1:
		returnval = SRVAL_BANK_FAILCREATEACCOUNT_ALREADY;
		break;
	}
	

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_BANK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

// 계좌를 해지할 것. 
void cltServer::DoMsg_DBMSG_RESPONSE_CLOSEBANKACCOUNT(sPacketHeader* pPacket)
{
	sDBResponse_CloseBankAccount* pclMsg = (sDBResponse_CloseBankAccount*)pPacket;

	SI32 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE)return ;

	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
		returnval = SRVAL_BANK_FAILCLOSEACCOUNT;
		break;
	case 1:
		{
			//------------------------------
			// 계좌 정보 업데이트. 
			//------------------------------
			// 계좌 정보를 확보한다. 
			pclCM->CR[id]->pclCI->clBank.Set(&pclMsg->clBankAccount);
			
			// 클라이언트로 정보를 보낸다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);
			
			//-----------------------------
			// 현금 정보 업데이트 
			//-----------------------------
			pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siMoney);
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

			returnval = SRVAL_BANK_SUCCESSCLOSEACCOUNT;

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 16] // 통장 해지 쓰기. 
				cltCharServer* pclchar = NULL;
				if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BANK, LOGCOMMAND_INDEX_BANK_CLOSEBANKACCOUNT, 
					0, pclchar, NULL, 0, 0, pclMsg->siMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
					0, 0, 0, 0, 0, NULL, NULL);
			}
		}

		break ;
	case -2:
		{
			returnval = SRBAL_BANK_FAILCLOSEACCOUNT_OTHERVILLAGE;
		}
		break;
	case -3:
		{
			returnval = SRBAL_BANK_FAILCLOSEACCOUNT_CHECKMARKET;
		}
		break;
	}
		
		
	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_BANK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

// 전장에 돈을 입금하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_INPUTMONEYTOBANK(sPacketHeader* pPacket)
{
	sDBResponse_InputMoneyToBank* pclMsg = (sDBResponse_InputMoneyToBank*)pPacket;

	SI32 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE)return ;

	// 결과를 확인한다. 
	
	SI32 returnval = 0;
	switch(pclMsg->siResult)
	{
	case 0:
		return;
	case 1:	// 입금 성공. 
		{
			if( pclMsg->clBankAccount.GetMoney() == 0 ) break;
			returnval	= SRVAL_BANK_SUCCESSINPUTMONEY;

			//------------------------------
			// 계좌 정보 업데이트. 
			//------------------------------
			// 계좌 정보를 확보한다. 
			pclCM->CR[id]->pclCI->clBank.Set(&pclMsg->clBankAccount);

			// 클라이언트로 정보를 보낸다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);

			//-------------------------------
			// 현금 정보 업데이트 
			//-------------------------------
			// 현금 정보를 확보한다. 
			pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->clMoney.itMoney);

			// 클라이언트로 정보를 보낸다. 
			pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

			//-------------------------------
			// ToBank 업데이트. 
			//-------------------------------
			SI32 villageunique = pclMsg->clBankAccount.clAccountNum.GetVillageUnique();
			if(villageunique > 0)
			{
				cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
				if(pclinfo)
				{
					pclinfo->clBankStrInfo.clTotalInputMoney.Set(&pclMsg->clTotalInputMoney);
					pclinfo->clCommonInfo.clMoney.Set(&pclMsg->clCurrentLeftMoney);
				}

			}

			//-----------------------------
			// ByBank 업데이트 
			//-----------------------------
			villageunique = pclMsg->siByBankVillageUnique;
			if(villageunique > 0)
			{
				cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
				if(pclinfo)
				{
					pclinfo->clBankStrInfo.clTotalInputMoney.Set(&pclMsg->clByBankTotalInputMoney);
					pclinfo->clCommonInfo.clMoney.Set(&pclMsg->clByBankCurrentLeftMoney);
				}
			}

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{ 
				// [로그추가 : 황진성 2007. 10. 16] // 은행 입금 쓰기.
				// param1 == 수수료.
				// param2 == 은행 잔액.
				cltCharServer* pclchar = NULL;
				if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BANK, LOGCOMMAND_INDEX_BANK_INPUTMONEYTOBANK, 
					0, pclchar, NULL, 0, pclMsg->clInputMoney.itMoney, pclMsg->clMoney.itMoney, NULL, 0, 0, 0, pclMsg->siByBankVillageUnique, 0, 0, 
					pclMsg->siFee, pclMsg->clBankAccount.GetMoney(), 0, 0, 0, NULL, NULL);
			}
		}
		break;


	case -1: // 계좌 없어 입금 실패. 
		returnval	= SRVAL_BANK_FAILINPUTMONEY_NOACCOUNT;
		break;	

	default:
		return ;
	}

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_BANK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}


// 전장에서 돈을 출금하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_OUTPUTMONEYFROMBANK(sPacketHeader* pPacket)
{
	sDBResponse_OutputMoneyFromBank* pclMsg = (sDBResponse_OutputMoneyFromBank*)pPacket;

	SI32 id	= pclMsg->usCharID;

	if(pclCM->IsValidID(id) == FALSE)return ;

	// 결과를 확인한다. 
	SI32 returnval = 0;
	switch(pclMsg->siResult)
	{
	case 0:
		return ;
	case 1:	// 출금 성공. 
		returnval	= SRVAL_BANK_SUCCESSOUTPUTMONEY;

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 16] // 출금 입금 쓰기. 
			// param1 == 수수료.
			// param2 == 은행 잔액.
			cltCharServer* pclchar = NULL;
			if(pclCM->IsValidID(id))		pclchar = (cltCharServer*)pclCM->CR[id];

			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BANK, LOGCOMMAND_INDEX_BANK_OUTPUTMONEYFROMBANK, 
				0, pclchar, NULL, 0, pclMsg->clOutputMoney.itMoney, pclMsg->clMoney.itMoney, NULL, 0, 0, 0, pclMsg->siByBankVillageUnique, 0, 0, 
				pclMsg->siFee, pclMsg->clBankAccount.GetMoney(), 0, 0, 0, NULL, NULL);
		}
		break;
	case -1: // 계좌가 없다. 
		returnval	= SRVAL_BANK_FAILOUTPUTMONEY_NOACCOUNT;
		break;
	case -2:	// 잔액 부족. 
		returnval	= SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHMONEYINACCOUNT;
		break;
	case -3:	// 출금 성공-계좌삭제. 
		returnval	= SRVAL_BANK_SUCCESSOUTPUTMONEY_DELACCOUNT;
		break;
	case -4:	// 출금 실패. - 수수료 부족. 	
		returnval	= SRVAL_BANK_FAILOUTPUTMONEY_NOTENOUGHFEE;
		break;
	default:
		MsgBox( TEXT("1fd9jfd"), TEXT("fld9j:%d"), pclMsg->siResult);
		return ;
	}

	//----------------------------------
	// 계좌 정보 업데이트 
	//----------------------------------
	// 계좌 정보를 확보한다. 
	pclCM->CR[id]->pclCI->clBank.Set(&pclMsg->clBankAccount);

	// 클라이언트로 정보를 보낸다. 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_BANK, true, 0);

	//----------------------------------
	// 현금 정보 업데이트 
	//----------------------------------
	// 현금 정보를 확보한다. 
	pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->clMoney.itMoney);

	// 클라이언트로 정보를 보낸다. 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_BANK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

	if(pclMsg->bCancelStockOrder)
	{
		// PersonID를 구한다. 
		SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();

		// 주문을 취소하라는 요청을 DB로 보낸다.
		sDBRequest_CancelStockOrder clMsg(id, personid);
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		cltGameMsgResponse_CancelStockOrder clcancelstockorder;
		cltMsg clMsg2(GAMEMSG_RESPONSE_CANCEL_STOCKORDER, sizeof(clcancelstockorder), (BYTE*)&clcancelstockorder);

		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);


	}

	//-------------------------------
	// ToBank 업데이트. 
	//-------------------------------
	SI32 villageunique = pclMsg->clBankAccount.clAccountNum.GetVillageUnique();
	if(villageunique > 0)
	{
		cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
		if(pclinfo)
		{
			pclinfo->clBankStrInfo.clTotalInputMoney.Set(&pclMsg->clTotalInputMoney);
			pclinfo->clCommonInfo.clMoney.Set(&pclMsg->clCurrentLeftMoney);
		}
	}

	//-----------------------------
	// ByBank 업데이트 
	//-----------------------------
	villageunique = pclMsg->siByBankVillageUnique;
	if(villageunique > 0)
	{
		cltStrInfo* pclinfo = pclBankManager->GetStrInfo(villageunique);
		if(pclinfo)
		{
			pclinfo->clBankStrInfo.clTotalInputMoney.Set(&pclMsg->clByBankTotalInputMoney);
			pclinfo->clCommonInfo.clMoney.Set(&pclMsg->clByBankCurrentLeftMoney);
		}
	}
}

// 전장 수수료 정보를 설정하라는 요청에 대한 응답. 
void cltServer::DoMsg_DBMSG_RESPONSE_SETBANKFEE(sPacketHeader* pPacket)
{
	sDBResponse_SetBankFee* pclMsg = (sDBResponse_SetBankFee*)pPacket;

	// 결과를 확인한다. 
	SI32 returnval = 0;
	switch(pclMsg->siResult)
	{
	case 0:
		return ;
	case 1:	// 수수료 변경 성공. 
		returnval	= SRVAL_BANK_SUCCESSSETFEE;
		break;
	default:
		MsgBox( TEXT("vc984j"), TEXT("fdd:%d"), pclMsg->siResult);
	}

	// 서버의 해당 전장의 정보를 업데이트 한다. 
	cltStrInfo* pclinfo = pclBankManager->GetStrInfo(pclMsg->siVillageUnique);
	if(pclinfo == NULL)return ;
	pclinfo->clBankStrInfo.clFee.Set(&pclMsg->clFee);

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(returnval)
	{
		// 클라이언트로 서버의 응답값을 보낸다. 
		SendServerResponseMsg(RANKTYPE_BANK, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

		// [로그추가 : 황진성 2007. 10. 22] // 은행 수수료 세팅.
		// param1 == 타행 입출금 수수료,  
		// param2 == 최저 수수료, 
		// param3 == 최대 수수료 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_BANK, LOGCOMMAND_INDEX_BANK_SET_FEE, 
							 0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							 pclMsg->clFee.GetFeeRate(),
							 pclMsg->clFee.GetMinFee(), 
							 pclMsg->clFee.GetMaxFee(), 
							 0, 0, NULL, NULL);
	}
}