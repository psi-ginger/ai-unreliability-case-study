//---------------------------------
// 2003/6/26 김태곤
//---------------------------------
#include <CommonLogic.h>


#include "..\..\Resource.h"

#include "House.h"
//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\Common\Util\Control\Control.h"

extern cltCommonLogic* pclClient;


// 계약 조건을 보여준다. 
BOOL cltHouseManager::ShowRentContract(HWND hDlg)
{
	char buffer[256];

	sprintf(buffer, "%d", clClientStrInfo.clHouseStrInfo.clRentContract.siRentFee);
	SetDlgItemText(hDlg, IDC_EDIT_HOUSESETRENTFEE, buffer);

	sprintf(buffer, "%d", clClientStrInfo.clHouseStrInfo.clRentContract.siRentPeriod);
	SetDlgItemText(hDlg, IDC_EDIT_HOUSESETRENTPERIOD, buffer);

	return TRUE;
}


// 시전 유니트 정보를 보여준다. 
BOOL cltHouseManager::ShowHouseUnitListInChief(HWND hDlg)
{
	SI32 i;
	char buffer[256];

	// 기존 리스트를 지운다. 
	ListDelete(hDlg, IDC_LIST_HOUSEUNITLISTINCHIEF);

	for(i = 0;i < MAX_HOUSEUNIT_NUMBER;i++)
	{
		if(clClientUnit[i].clPerson.GetPersonID() == 0)
		{
			char* pText = GetTxtFromMgr(1691);
			sprintf(buffer, "%d %s", i+1, pText);
			
		}
		else
		{
			sprintf(buffer, "%d %s", i+1, clClientUnit[i].clPerson.GetName());
		}

		ListAddString(hDlg, IDC_LIST_HOUSEUNITLISTINCHIEF, (LPARAM)buffer);
	}

	return TRUE;
}

// 시전 임차 희망자의 리스트를 보여준다. 
BOOL cltHouseManager::ShowHouseApplyerListInChief(HWND hDlg)
{
	SI32 i;
	char buffer[256];

	// 기존 리스트를 지운다. 
	ListDelete(hDlg, IDC_LIST_HOUSEAPPLYER);

	for(i = 0;i < MAX_APPLY_PER_HOUSE;i++)
	{
		if(clClientApplyer[i].clPerson.GetPersonID() == 0)
		{
			char* pText = GetTxtFromMgr(1692);
			sprintf(buffer, "%d %s", i+1, pText);
			
		}
		else
		{
			sprintf(buffer, "%d %s", i+1, clClientApplyer[i].clPerson.GetName());
		}

		ListAddString(hDlg, IDC_LIST_HOUSEAPPLYER, (LPARAM)buffer);
	}

	return TRUE;
}

// 선택된 시전 유니트의 정보를 보여준다. 
BOOL cltHouseManager::ShowSelectedHouseUnitExplain(HWND hDlg, SI32 index)
{
	char buffer[256];
	char date[128];
	if(index < 0)return FALSE;

	// 계약 만기일을 구한다. 
	cltDate cldate;
	cldate.Set(&clClientUnit[index].clContract.clDate);
	cldate.AddMonth(clClientUnit[index].clContract.siRentPeriod*12);
	
	cldate.GetDateText(date);

	char* pText = GetTxtFromMgr(1693);
	sprintf(buffer, pText, index+1, clClientUnit[index].clPerson.GetName(),
		clClientUnit[index].clContract.siRentFee,
		date);

	SetDlgItemText(hDlg, IDC_EDIT_HOUSEUNITEXPLAIN, buffer);
	
	return TRUE;
}
