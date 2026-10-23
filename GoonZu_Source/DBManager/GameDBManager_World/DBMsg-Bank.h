#ifndef _DBMSGBANK_H
#define _DBMSGBANK_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\CommonLogic\Bank\Bank.h"


//------------------------------------------
// 은행에 통장을 개설할 것을 요청. 
//------------------------------------------
class sDBRequest_CreateBankAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// who
	SI32				siPersonID;					// 돈을 입금할 PersonID

	// by where
	SI32				siVillageUnique;			// 은행의 VillageUnique

	sDBRequest_CreateBankAccount(SI32 id, SI32 personid, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_CreateBankAccount );
		packetHeader.usCmd = DBMSG_REQUEST_CREATEBANKACCOUNT;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;
	}
};

class sDBResponse_CreateBankAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과값.

	// who, to where
	cltBankAccount		clBankAccount;				// 입금후의 계좌 정보.

	sDBResponse_CreateBankAccount()
	{
		ZeroMemory(this, sizeof(sDBResponse_CreateBankAccount));
	}


};

//------------------------------------------
// 은행에 통장을 해지할 것을 요청. 
//------------------------------------------
class sDBRequest_CloseBankAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// who
	SI32				siPersonID;					// 돈을 입금할 PersonID

	// by where
	SI32				siVillageUnique;			// 은행의 VillageUnique

	sDBRequest_CloseBankAccount(SI32 id, SI32 personid, SI32 villageunique)
	{
		packetHeader.usSize = sizeof( sDBRequest_CloseBankAccount );
		packetHeader.usCmd = DBMSG_REQUEST_CLOSEBANKACCOUNT;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;
	}
};

class sDBResponse_CloseBankAccount
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과값.

	// who, to where
	cltBankAccount		clBankAccount;				// 계좌 삭제 후의 계좌 정보. 	

	GMONEY				siMoney;					// 계좌 삭제 후의 현금 정보. 

	// by where
	SI32				siVillageUnique;			// 은행의 VillageUnique for LOG

	sDBResponse_CloseBankAccount()
	{
		ZeroMemory(this, sizeof(sDBResponse_CloseBankAccount));
	}
};


//------------------------------------------
// 은행에 돈을 입금할 것을 요청. 
//------------------------------------------
class sDBRequest_InputMoneyToBank
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// who
	SI32				siPersonID;					// 돈을 입금할 PersonID

	// by where
	SI32				siVillageUnique;			// person이 업무를 보고 있는 현재 은행(새로 통장을 만들어야 할 경우 어느 마을에 만들어야 하는지..) 

	// what
	cltMoney			clMoney;					// 입금할 돈의 금액. 

	// fee
	GMONEY				siFee;						// 수수료. 

	sDBRequest_InputMoneyToBank(SI32 id, SI32 personid, SI32 villageunique, cltMoney* pclmoney, GMONEY fee)
	{
		packetHeader.usSize = sizeof( sDBRequest_InputMoneyToBank );
		packetHeader.usCmd = DBMSG_REQUEST_INPUTMONEYTOBANK;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;
		clMoney.Set(pclmoney);	

		siFee			= fee;
	}
};

class sDBResponse_InputMoneyToBank
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;					// 결과값.

	// who, to where
	cltBankAccount		clBankAccount;				// 입금후의 계좌 정보. 	

	// what
	cltMoney			clMoney;					// Person이 보유하고 있는 현금. 

	// to bank info
	cltMoney			clTotalInputMoney;			// 해당은행에 입금된 총 금액. 
	cltMoney			clCurrentLeftMoney;			// 해당은행에 남은 총 금액. 

	// by bank info
	SI32			siByBankVillageUnique;		// 경유지 은행 유니크. 
	cltMoney		clByBankTotalInputMoney;	// 해당은행에 입금된 총 금액. 
	cltMoney		clByBankCurrentLeftMoney;	// 해당은행에 남은 총 금액. 

	// what
	cltMoney			clInputMoney;				// 입금한 돈의 금액. for LOG

	// fee
	GMONEY				siFee;						// 수수료. for LOG
};

//------------------------------------------
// 은행으로부터 돈을 출금할 것을 요청. 
//------------------------------------------
class sDBRequest_OutputMoneyFromBank
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siPersonID;					// 돈을 출금할 PersonID
	SI32				siVillageUnique;			// 출금하는 곳의 VillageUnique
	cltMoney			clMoney;					// 출금할 돈의 금액. 

	GMONEY				siFee;						// 수수료. (출금할 돈과 별도로 계좌에서 빠진다.) 

	sDBRequest_OutputMoneyFromBank(SI32 id, SI32 personid, SI32 villageunique, cltMoney* pclmoney, GMONEY fee)
	{
		packetHeader.usSize = sizeof( sDBRequest_OutputMoneyFromBank );
		packetHeader.usCmd = DBMSG_REQUEST_OUTPUTMONEYFROMBANK;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;
		clMoney.Set(pclmoney);	

		siFee			= fee;
	}
};


class sDBResponse_OutputMoneyFromBank
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	SI32				siResult;			// 결과값.
	cltBankAccount		clBankAccount;		// 출금후의 계좌 정보. 	
	cltMoney			clMoney;			// Person이 보유하고 있는 현금. 

	cltMoney			clTotalInputMoney;	// 해당은행에 입금된 총 금액. 
	cltMoney			clCurrentLeftMoney;	// 해당은행에 남은 총 금액. 

	// by bank info
	SI32				siByBankVillageUnique;		// 경유지 은행 유니크. 
	cltMoney			clByBankTotalInputMoney;	// 해당은행에 입금된 총 금액. 
	cltMoney			clByBankCurrentLeftMoney;	// 해당은행에 남은 총 금액. 
	bool				bCancelStockOrder;			// 매수주문금액보다 전장에 돈이 없으면 True
								
	cltMoney			clOutputMoney;				// 출금한 돈의 금액. For LOG 

	GMONEY				siFee;						// 수수료. (출금할 돈과 별도로 계좌에서 빠진다.) For LOG

	sDBResponse_OutputMoneyFromBank()
	{
		ZeroMemory(this, sizeof(sDBResponse_OutputMoneyFromBank));
	}
};


//------------------------------------------
// 은행수수료를  설정할 것을 요청
//------------------------------------------
class sDBRequest_SetBankFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// Who
	SI32				siPersonID;

	// Where
	SI32				siVillageUnique;	// 은행이 위치한 곳의 VillageUnique

	// What
	cltFee			clFee;			//  설정해야 할 은행의 정보. 


	sDBRequest_SetBankFee()
	{
		ZeroMemory(this, sizeof(sDBRequest_SetBankFee));
	}

	sDBRequest_SetBankFee(SI32 id, SI32 personid, SI32 villageunique, cltFee* pclfee)
	{
		packetHeader.usSize = sizeof( sDBRequest_SetBankFee );
		packetHeader.usCmd = DBMSG_REQUEST_SETBANKFEE;
		usCharID		= id;

		siPersonID		= personid;
		siVillageUnique	= villageunique;
		clFee.Set(pclfee);
	}
};


class sDBResponse_SetBankFee
{
public:
	sPacketHeader		packetHeader;
	UI16				usCharID;

	// result 
	SI32				siResult;			// 결과값.

	// who
	SI32				siPersonID;

	// where
	SI32				siVillageUnique;	// 은행이 위치한 마을의 VillageUnique

	// what
	cltFee			clFee;					// 은행의 수수료 정보. 

	sDBResponse_SetBankFee()
	{
		ZeroMemory(this, sizeof(sDBResponse_SetBankFee));
	}

};


#endif