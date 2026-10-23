#include <CommonLogic.h>
#include "..\..\Server\Server.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\Common\Util\Control\Control.h"

#include "..\..\Resource.h"

#include "Mine.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Mine.h"

extern cltCommonLogic* pclClient;


//계약 조건을 보여준다. 
BOOL cltMineManager::ShowRentContractInUser(HWND hDlg)
{
	char buffer[1024];

	sprintf(buffer, "%d", clClientStrInfo.clMineStrInfo.clRentContract.siRentFee);
	SetDlgItemText(hDlg, IDC_EDIT_RENTFEE, buffer);

	sprintf(buffer, "%d", clClientStrInfo.clMineStrInfo.clRentContract.siRentPeriod);
	SetDlgItemText(hDlg, IDC_EDIT_RENTPERIOD, buffer);

	char* pText = GetTxtFromMgr(3249);
	sprintf(buffer, pText);
	SetDlgItemText(hDlg, IDC_EDIT_CONTRACTCONDITION, buffer);
	
	return TRUE;
}
