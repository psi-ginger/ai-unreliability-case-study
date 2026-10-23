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



BOOL cltHorseMarketManager::ShowFeeRate(HWND hDlg, SI32 rate)
{
	char buffer[256];
	HWND hwnd;
	
	float feerate = (float)rate/10;
	
	sprintf(buffer, "%3.1f",feerate); 

	hwnd = GetDlgItem( hDlg, IDC_EDIT_FEERATE );
	SetWindowText( hwnd, buffer);

	return TRUE;
}