//---------------------------------
// 2004/3/10 김태곤
//---------------------------------


#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Char\CharCommon\Char-Common.h"



#include "..\..\Common\Util\Control\Control.h"

#include "..\..\Resource.h"

#include "Hunt.h"
#include "..\..\DBManager\GameDBManager\DBMsg-Market.h"
#include "..\..\DBManager\GameDBManager\DBMsg-Hunt.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg\MsgType-Hunt.h"

#include "../../Common/Item/ItemCommon/ItemUnit.h"
#include "../../Common/Char/CharCommon/Char-Common.h"

#include "../CityHall/TaxpaymentListDlg.h"
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"

extern cltCommonLogic* pclClient;

#define HUNT_SETTIMER_VALUE			2000


BOOL CALLBACK cltHuntManager::StaticHuntInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclHuntManager->HuntInfoDlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltHuntManager::HuntInfoDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id=0;
	HWND hwnd =NULL;
	SI32 index=0;
	char buffer[256]="";
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			// 타이머를 설정한다. 
			SetTimer(hDlg, 0, HUNT_SETTIMER_VALUE, NULL);
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
						&pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->clVillageVary.clRankPerson[RANKTYPE_HUNT],
						pclClient->pclVillageManager->pclVillageInfo[siVillageUnique]->RankAccountID[RANKTYPE_HUNT]
						);
				}
			}

			// 보유중인 자금을 표시한다. 
			char* pText = GetTxtFromMgr(611);
 			g_JYLibFunction.SetNumUnit(clClientStrInfo.clCommonInfo.clMoney.itMoney,buffer,pText);
			SetDlgItemText(hDlg, IDC_EDIT_STRMONEY, buffer);

		}
		return TRUE;

	case WM_CHAR:
		return TRUE;

	case WM_DESTROY:
		// 타이머를 해제한다. 
		KillTimer(hDlg, 0);

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
				char* pText = GetTxtFromMgr(1704);
				pclTaxPaymentDlg->Create( NULL, hDlg, this, pText );
			}
			break;
		}

		return FALSE;
	}

	return FALSE;
	
}
