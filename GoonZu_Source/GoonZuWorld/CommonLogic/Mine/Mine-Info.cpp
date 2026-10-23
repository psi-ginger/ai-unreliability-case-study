#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Common\Util\Control\Control.h"

#include "..\..\Resource.h"

#include "Mine.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Mine.h"

#include "../CityHall/TaxpaymentListDlg.h"

extern cltCommonLogic* pclClient;


BOOL CALLBACK cltMineManager::StaticMineInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclMineManager->MineInfoDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltMineManager::MineInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;

	switch(iMsg)
	{
	case WM_INITDIALOG:
		{

			// 타이머를 설정한다. 
			SetTimer(hDlg, 1, 500, NULL);

		}
		return TRUE;
		
	case WM_TIMER:
		{
			char buffer[256];

			// 건물 내구도를 표시한다
			sprintf( buffer, "%d/%d", clClientStrInfo.clCommonInfo.siStrDura, 10000 );
			SetDlgItemText( hDlg, IDC_EDIT_STRDURA, buffer );

			// 행수의 이름을 표시한다. 
			if(UpdateChiefSwitch == true)
			{
				UpdateChiefSwitch = false;

				// 장을 보여준다. 
				if(siVillageUnique > 0)
				{
					ShowChief(hDlg, IDC_EDIT_STRCHIEF, 
						&pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->clVillageVary.clRankPerson[RANKTYPE_MINE],
						pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->RankAccountID[RANKTYPE_MINE]
						);
				}
			}

			// 보유중인 자금을 표시한다.  
			char* pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit( clClientStrInfo.clCommonInfo.clMoney.itMoney, buffer, pText );
			SetDlgItemText(hDlg, IDC_EDIT_STRMONEY, buffer);				
		}
		return TRUE;

	case WM_CHAR:
		return TRUE;

	case WM_DESTROY:
		// 타이머를 해제한다. 
		KillTimer(hDlg, 1);

		DeleteTabDlg();
		return TRUE;

	case WM_NOTIFY:
		{
		}
		return TRUE;

	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
			case IDC_BUTTON_TAXLIST:
			{
				char* pText = GetTxtFromMgr(3247);
				pclTaxPaymentDlg->Create( NULL, hDlg, this, pText);
			}
			break;		
		default:
			break;
		}
		return FALSE;
	}

	return FALSE;
	
}
