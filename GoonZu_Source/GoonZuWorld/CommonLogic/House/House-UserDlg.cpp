//---------------------------------
// 2003/6/30 김태곤
//---------------------------------
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Common\Util\Control\Control.h"

#include "..\..\Resource.h"

#include "House.h"

#include "..\..\Client\Music\Music.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;



//계약 조건을 보여준다. 
BOOL cltHouseManager::ShowRentContractInUser(HWND hDlg)
{
	char buffer[1024];

	sprintf(buffer, "%d", clClientStrInfo.clHouseStrInfo.clRentContract.siRentFee);
	SetDlgItemText(hDlg, IDC_EDIT_HOUSERENTFEE, buffer);

	sprintf(buffer, "%d", clClientStrInfo.clHouseStrInfo.clRentContract.siRentPeriod);
	SetDlgItemText(hDlg, IDC_EDIT_HOUSERENTPERIOD, buffer);

	char* pText = GetTxtFromMgr(1703);
	sprintf(buffer, pText);
	SetDlgItemText(hDlg, IDC_EDIT_CONTRACTCONDITION, buffer);
	
	return TRUE;
}
