//---------------------------------
// 2003/9/29 김태곤
//---------------------------------
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "..\..\Common\Util\Control\Control.h"
#include "..\..\Resource.h"
#include "House.h"

#include "..\CityHall\TaxPaymentListDlg.h"
#include "MsgType-Structure.h"
//--------------------------------------
// Common
//--------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;


BOOL CALLBACK cltHouseManager::StaticHouseChiefDlgProc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclHouseManager->HouseChiefDlgProc2(hDlg, iMsg, wParam, lParam);
}

BOOL CALLBACK cltHouseManager::HouseChiefDlgProc2(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{
			// 타이머를 설정한다. 
			SetTimer(hDlg, 1, 1000, NULL);

		}
		return TRUE;
		
	case WM_TIMER:
		{

		}
		return TRUE;

	case WM_CHAR:
		{

		}
		return TRUE;

	case WM_DESTROY:
		{
			// 타이머를 해제한다. 
			KillTimer(hDlg, 1);

			DeleteTabDlg();
		}
		return TRUE;

	case WM_NOTIFY:
		{
			
		}
		return TRUE;

	case WM_COMMAND:
		switch( LOWORD(wParam) )
		{
		// 건물을 수리한다
		case IDC_BUTTON_STRREPAIR:
			{
				CreateStructureRepairDlg( NULL, hDlg );
			}
			break;

		case IDC_BUTTON_TAXLIST:
			{
				char* pText = GetTxtFromMgr(370);
				pclTaxPaymentDlg->Create( NULL, hDlg, this, pText );
			}
			break;

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

					cltGameMsgRequest_PayTax clPayTax( siVillageUnique, RANKTYPE_HOUSE, 0, &clMoney );
						
					cltMsg clMsg( GAMEMSG_REQUEST_PAYTAX, sizeof(cltGameMsgRequest_PayTax), (char*)&clPayTax );

					pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
					
				}
				
			}
			break;

			// 시전 행수 사임. 
		case IDC_BUTTON_RESIGN_HOUSE:
			{
				// 서버에 시전행수에서 사임함을 요청한다. 
				SI32 id = cltCharCommon::CharUniqueInfo[CharUnique];
				if( id )
				{
					cltSimpleRank clrank(RANKTYPE_HOUSE, siVillageUnique);
					if(pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == TRUE)
					{

						char buffer[ 256 ];
						char* pText = GetTxtFromMgr(1694);
						strcpy(buffer, pText);

						pText = GetTxtFromMgr(1442);
						if (  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) {
							break;
						}

						cltMsg clMsg( GAMEMSG_REQUEST_RESIGN, 
							sizeof(cltSimpleRank), (char*)&clrank );
						
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

