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

#include "Mine.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Mine.h"

extern cltCommonLogic* pclClient;


BOOL CALLBACK cltMineManager::StaticMineChiefDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclMineManager->MineChiefDlgProc(hDlg, iMsg, wParam, lParam);
}
BOOL CALLBACK cltMineManager::MineChiefDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
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
		// 건물을 수리한다
		case IDC_BUTTON_STRREPAIR:
			{
				CreateStructureRepairDlg( NULL, hDlg );
			}
			break;

		// 세금을 납부한다 
		case IDC_BUTTON_PAYTAX:
			{
				SI32 id = 1;
				if( id )
				{
					char buf[ 256 ];
					HWND hwnd = GetDlgItem( hDlg, IDC_EDIT_PAYTAX );
					GetWindowText( hwnd, buf, 32 );

					int money = atoi( buf );
					
					if( money <= 0 ) {
						char* pText = GetTxtFromMgr(1438);
						SetMainExplain(m_hStrDlg, pText);
						break;
					}

					cltMoney clMoney;

					clMoney.Set( money );

					cltGameMsgRequest_PayTax clPayTax( siVillageUnique, RANKTYPE_MINE, 0, &clMoney );
						
					cltMsg clMsg( GAMEMSG_REQUEST_PAYTAX, sizeof(cltGameMsgRequest_PayTax), (char*)&clPayTax );

					pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
					
				}
			}
			break;


		case IDC_BUTTON_RESIGN_MINE:
			{
				// 서버에 행수에서 사임함을 요청한다. 
				SI32 id = 1;
				if(id)
				{
					cltSimpleRank clrank( RANKTYPE_MINE, siVillageUnique );
					
					if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == TRUE )
					{
						cltMsg clMsg( GAMEMSG_REQUEST_RESIGN, sizeof(cltSimpleRank), (char*)&clrank);
						
						pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
					}
				}
			}
			break;


		default:
			break;
		}
		return FALSE;
	}

	return FALSE;
	
}
