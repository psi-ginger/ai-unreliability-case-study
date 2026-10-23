//---------------------------------
// 2003/5/17 김태곤
//---------------------------------

#ifndef _BANK_H
#define _BANK_H

#include "..\StructureClass\StructureClass.h"
#include "..\..\Common\CommonHeader.h"
#include "..\Account\Account.h"

//-----------------------------------------------
// Definition
//-----------------------------------------------
#define MAX_BANK_TAB_NUMBER	3
#define MAX_ACOUNT_PER_BANK	10000		// 전장이 가질 수 있는 총 구좌 수. 
#define MAX_MONEY_IN_ACOUNT	1000000000	// 한 계정이  가질 수 있는 최대 금액. 

class cltBankAccount {
private:
	GMONEY siMoney;

public:
	cltAccountNum	clAccountNum;
	SI32			siPersonID;		// 계좌 주인 PersonID

	cltBankAccount()
	{
		siMoney		= 0;
		siPersonID	= 0;
	}

	cltBankAccount(cltAccountNum* pclaccountnum, SI32 sipersonid, GMONEY money)
	{
		clAccountNum.Set(pclaccountnum);
		siPersonID		= sipersonid;
		siMoney			= money;
	};

	void Set(cltBankAccount* pclaccount)
	{
		clAccountNum.Set(&pclaccount->clAccountNum);
		siPersonID		= pclaccount->siPersonID;
		siMoney			= pclaccount->siMoney;

	}

	void SetMoney(GMONEY money)
	{
		siMoney = money;
	}

	GMONEY GetMoney()const{return siMoney;}

	BOOL IncreaseMoney(GMONEY money)
	{
		if((siMoney + money) > MAX_MONEY_IN_ACOUNT)return FALSE;

		siMoney += money;

		return TRUE;
	}

	BOOL DecreaseMoney(GMONEY money)
	{
		if(money > siMoney)return FALSE;
		 siMoney -= money;

		 return TRUE;
	}

	void GetGMSectionData(GMSectionData* pkGMSectionData);
};


//------------------------------------
// 각 뱅크의 클래스 
//------------------------------------
class cltBank : public cltStructureBasic {
private:
	SI32 siAcountCount;			// 통장 개수. 

public:

	cltBank(SI32 sivillageunique);
	~cltBank();

	// 수수료 정보를 설정한다.
	BOOL SetFee(cltFee* pclfee);

	// 수수료를 계산한다. 
	GMONEY CalcFee(GMONEY money);


};


#endif
