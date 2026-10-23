//---------------------------------
// 2003/5/17 김태곤
//---------------------------------
//#include <Graphic.h>
#include "Bank.h"
#include "..\..\CommonLogic\CommonLogic.h"

#include "..\..\Resource.h"

extern cltCommonLogic* pclClient;

//-------------------------------------------
// cltBankAccount
//-------------------------------------------
void cltBankAccount::GetGMSectionData(GMSectionData* pkGMSectionData)
{
	if( pkGMSectionData == NULL )		return;

	pkGMSectionData->BeginData( "BankAccount" );
	{
		pkGMSectionData->AddDataInt("money",		siMoney);
		pkGMSectionData->AddDataInt("village",		clAccountNum.siVillageUnique);
		pkGMSectionData->AddDataInt("number",		clAccountNum.siAccountNum);
	}
	pkGMSectionData->EndData( "BankAccount" );
}

//-------------------------------------------
// cltBank
//-------------------------------------------
cltBank::cltBank(SI32 sivillageunique) : cltStructureBasic(sivillageunique)
{

	siAcountCount	= 0;

}

cltBank::~cltBank()
{

}


// 수수료 정보를 설정한다.
BOOL cltBank::SetFee(cltFee* pclfee)
{
	clStrInfo.clBankStrInfo.clFee.Set(pclfee);
	return  TRUE;
}


// 수수료를 계산한다. 
GMONEY cltBank::CalcFee(GMONEY money)
{
	return clStrInfo.clBankStrInfo.clFee.CalcFee(money);
}




