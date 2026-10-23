//---------------------------------
// 2003/6/16 김태곤
//---------------------------------
#include "Bank-Manager.h"
#include "..\..\Common\Util\Control\Control.h"
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Server\Rank\Rank.h"

#include "MsgType-Structure.h"

#include "..\..\Resource.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;



// 수수료율을 보여준다. 
BOOL cltBankManager::ShowFeeRate(HWND hDlg, SI32 rate)
{
	char buffer[256];
	HWND hwnd;
	
	float feerate = (float)rate/10;
	
	sprintf(buffer, "%3.1f",feerate); 

	hwnd = GetDlgItem( hDlg, IDC_EDIT_BANK_FEERATE );
	SetWindowText( hwnd, buffer);

	return TRUE;
}

// 수수료범위를 보여준다. 
BOOL cltBankManager::ShowFeeRange(HWND hDlg)
{
	char buffer[256]="";
	HWND hwnd=NULL;

	// 최저 수수료. 
	sprintf(buffer, "%d", clClientStrInfo.clBankStrInfo.clFee.GetMinFee());
	
//	hwnd = GetDlgItem( hDlg, IDC_EDIT_BANKMINFEE );
//	SetWindowText( hwnd, buffer);

	// 최고 수수료. 
	sprintf(buffer, "%d", clClientStrInfo.clBankStrInfo.clFee.GetMaxFee());

//	hwnd = GetDlgItem( hDlg, IDC_EDIT_BANKMAXFEE );
//	SetWindowText( hwnd, buffer);

	return TRUE;
}

