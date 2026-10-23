//---------------------------------
// 2004/3/10 김태곤
//---------------------------------


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


BOOL CALLBACK cltHuntManager::StaticHuntChief2DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclHuntManager->HuntChief2DlgProc(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltHuntManager::HuntChief2DlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
			SetTimer(hDlg, 0, 500, NULL);

			HWND hwnd = GetDlgItem( hDlg, IDC_EDIT_HUNT_PAYTAX );
			SendMessage( hwnd, EM_LIMITTEXT, 63, 0 );

		}
		return TRUE;
		
	case WM_TIMER:
		{	
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
		// 건물을 수리한다
		case IDC_BUTTON_STRREPAIR:
			{
				CreateStructureRepairDlg( NULL, hDlg );
			}
			break;


		case IDC_BUTTON_HUNT_TAXLIST:
			{
				char* pText = GetTxtFromMgr(1704);
				pclTaxPaymentDlg->Create( NULL, hDlg, this, pText );
			}
			break;
		case IDC_BUTTON_HUNT_PAYTAX:
			{
				SI32 id = 1;
				if( id )
				{
					char buf[ 256 ];
					HWND hwnd = GetDlgItem( hDlg, IDC_EDIT_HUNT_PAYTAX );
					GetWindowText( hwnd, buf, 64 );

					int money = atoi( buf );

					if( money < 1 ) 
					{
						char* pText = GetTxtFromMgr(1438);
						SetMainExplain(m_hStrDlg, pText);
						break;
					}

					cltMoney clMoney;
					clMoney.Set( money );

					cltGameMsgRequest_PayTax clPayTax( siVillageUnique, RANKTYPE_HUNT, 0, &clMoney );
					cltMsg clMsg( GAMEMSG_REQUEST_PAYTAX, sizeof(cltGameMsgRequest_PayTax), (char*)&clPayTax );
					pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
				}
			}
			break;
		case IDC_BUTTON_HUNT_RESIGN:
			{
				SI32 id = 1;
				if(id)
				{
					cltSimpleRank clrank( RANKTYPE_HUNT, siVillageUnique );
					
					if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == TRUE )
					{
						cltMsg clMsg( GAMEMSG_REQUEST_RESIGN, sizeof(cltSimpleRank), (char*)&clrank);
						
						pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
					}
				}
			}
			break;
		}

		return FALSE;
	}

	return FALSE;
	
}
