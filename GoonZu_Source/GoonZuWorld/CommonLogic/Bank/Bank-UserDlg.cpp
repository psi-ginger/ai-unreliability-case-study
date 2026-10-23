#include "Bank-Manager.h"
#include "..\..\Common\Util\Control\Control.h"
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Server\Rank\Rank.h"

#include "..\..\Resource.h"

#include "../../CommonLogic/MsgType-Bank.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"

//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;


void cltBankManager::ShowAccountInfo( HWND hDlg )
{

	char buffer[256];
	char szVillage[ 32 ];
	char szAccountNum[ 32 ];
	char szMoney[ 32 ];
	char szCurrentMoney[ 32 ];

	SI32 id = 1;
	char* pText = NULL;
	if(id)
	{
		pText = GetTxtFromMgr(1466);
		strcpy( szVillage, pText );
		pText = GetTxtFromMgr(1287);
		strcpy( szAccountNum, pText );
		strcpy( szMoney, "0" );

		if(pclCM->CR[id]->pclCI->clBank.clAccountNum.IsValid() == FALSE)
		{
		}
		else
		{
			SI32 unique = pclCM->CR[id]->pclCI->clBank.clAccountNum.GetVillageUnique();
			if(unique >= 0)
			{
				strcpy( szVillage, pclClient->pclVillageManager->pclVillageInfo[unique]->szName );
				sprintf( szAccountNum, "%d-%d", pclCM->CR[id]->pclCI->clBank.clAccountNum.GetAccountNum(), unique );
				pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(pclCM->CR[id]->pclCI->clBank.GetMoney(), szMoney, pText);

			}
			else
			{
				return ;
			}
		}

		// 계좌 정보를 업데이트 한다. 
		HWND hwnd;

		hwnd = GetDlgItem( hDlg, IDC_EDIT_BANK_VILLAGE );
		SetWindowText( hwnd, szVillage );

		hwnd = GetDlgItem( hDlg, IDC_EDIT_BANK_ACCOUNTNUM );
		SetWindowText( hwnd, szAccountNum );
		
		hwnd = GetDlgItem( hDlg, IDC_EDIT_BANK_MONEY );
		SetWindowText( hwnd, szMoney );

		pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit(pclCM->CR[id]->pclCI->clIP.GetMoney(), szCurrentMoney, pText);
		hwnd = GetDlgItem( hDlg, IDC_EDIT_CURRENTMONEY );
		SetWindowText( hwnd, szCurrentMoney );

	}
	

	float feerate = (float)clClientStrInfo.clBankStrInfo.clFee.GetFeeRate() / 10;

	sprintf( buffer, "%3.1f%%", feerate, 
		clClientStrInfo.clBankStrInfo.clFee.GetMinFee(), 
		clClientStrInfo.clBankStrInfo.clFee.GetMaxFee() );

	SetDlgItemText( hDlg, IDC_EDIT_FEERATE, buffer );
}