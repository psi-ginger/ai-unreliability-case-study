#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Char\CharCommon\Char-Common.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "..\..\Common\Util\Control\Control.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Resource.h"

#include "HorseMarket.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-HorseMarket.h"

#include "../CityHall/TaxpaymentListDlg.h"

extern cltCommonLogic* pclClient;

void cltHorseMarketManager::ShowMyHorseInfo( HWND hDlg )
{
	char buf[ 1024 ];

	cltHorse *pclHorse = pclCM->CR[ 1 ]->pclCI->clHorseInfo.GetCurrentShowHorse();

	if( pclHorse->siHorseUnique ) {

		pclClient->pclHorseManager->GetText( pclHorse, buf );

	} else {
		char* pText = GetTxtFromMgr(1764);
		strcpy( buf, pText );
	}

	SetDlgItemText( hDlg, IDC_EDIT_MYHORSEINFO, buf );
}

void cltHorseMarketManager::ShowMyAccountInfo( HWND hDlg )
{
	char buf[ 1024 ]="";

	cltHorse *pclHorse = &clMyAccount.clHorse;

	if( pclHorse->siHorseUnique ) 
	{
		if(pclClient->pclHorseManager->GetText( pclHorse, buf ) == false)return ;
	}
	else 
	{
		char* pText = GetTxtFromMgr(1765);
		strcpy( buf, pText );
	}

	SetDlgItemText( hDlg, IDC_EDIT_ORDERHORSEINFO, buf );
	

}


// 수수료 정보를 보여준다.
BOOL cltHorseMarketManager::ShowFee(HWND hDlg)
{
	char buffer[256];
	float feerate = (float)clClientStrInfo.clHorseMarketStrInfo.clFee.GetFeeRate() / 10;

	sprintf(buffer, "%3.1f%%", feerate);
	SetDlgItemText(hDlg, IDC_STATIC_FEERATE, buffer);

	return TRUE;
}

// 실제로 지불해야 할 금액을 보여준다. 
BOOL cltHorseMarketManager::ShowRealFee(HWND hDlg, SI32 price)
{
	char buffer[256];

	sprintf(buffer, "%d", clClientStrInfo.clHorseMarketStrInfo.clFee.CalcFee( price ));
	SetDlgItemText(hDlg, IDC_EDIT_REALFEE, buffer);

	return TRUE;
}