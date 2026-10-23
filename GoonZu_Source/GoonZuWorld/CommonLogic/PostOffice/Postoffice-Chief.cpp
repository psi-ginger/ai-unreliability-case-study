#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Common\Util\Control\Control.h"

#include "..\..\Resource.h"

#include "Postoffice.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"

#include "../CityHall/TaxpaymentListDlg.h"
//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;


BOOL CALLBACK cltPostOfficeManager::StaticPostOfficeChiefDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return pclClient->pclPostOfficeManager->PostOfficeChiefDlgProc(hDlg, iMsg, wParam, lParam);
}
BOOL CALLBACK cltPostOfficeManager::PostOfficeChiefDlgProc(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	SI32 id = 1;

	
	switch(iMsg)
	{
	case WM_INITDIALOG:
		{

			char buf[ 256 ];
			
			// 송금 수수료 
			sprintf( buf, "%.1f", clClientStrInfo.clPostOfficeStrInfo.clMoneyFee.GetFeeRate() / 10. );
			SetDlgItemText( hDlg, IDC_EDIT_MONEYFEERATE, buf);
/*
			itoa( clClientStrInfo.clPostOfficeStrInfo.clMoneyFee.GetMinFee(), buf, 10 );
			SetDlgItemText( hDlg, IDC_EDIT_MONEYMINFEE, buf );

			itoa( clClientStrInfo.clPostOfficeStrInfo.clMoneyFee.GetMaxFee(), buf, 10 );
			SetDlgItemText( hDlg, IDC_EDIT_MONEYMAXFEE, buf );
*/

			// 물품 전달 수수료
			sprintf( buf, "%.1f", clClientStrInfo.clPostOfficeStrInfo.clItemFee.GetFeeRate() / 10. );
			SetDlgItemText( hDlg, IDC_EDIT_ITEMFEERATE, buf);
/*
			itoa( clClientStrInfo.clPostOfficeStrInfo.clItemFee.GetMinFee(), buf, 10 );
			SetDlgItemText( hDlg, IDC_EDIT_ITEMMINFEE, buf );

			itoa( clClientStrInfo.clPostOfficeStrInfo.clItemFee.GetMaxFee(), buf, 10 );
			SetDlgItemText( hDlg, IDC_EDIT_ITEMMAXFEE, buf );
*/
			// 메시지 전송 수수료
			itoa( clClientStrInfo.clPostOfficeStrInfo.siMsgFee, buf, 10 );
			SetDlgItemText( hDlg, IDC_EDIT_MSGFEE, buf );

			// 수신함 열람 수수료
			itoa( clClientStrInfo.clPostOfficeStrInfo.siRecvFee, buf, 10 );
			SetDlgItemText( hDlg, IDC_EDIT_RECVFEE, buf );

			// 타이머를 설정한다. 
			SetTimer(hDlg, 1, 1000, NULL);
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

					cltGameMsgRequest_PayTax clPayTax( siVillageUnique, RANKTYPE_POSTOFFICE, 0, &clMoney );
						
					cltMsg clMsg( GAMEMSG_REQUEST_PAYTAX, sizeof(cltGameMsgRequest_PayTax), (char*)&clPayTax );

					pclClient->pclCM->CR[id]->SendNetMsg((char*)&clMsg);
					
				}
			}
			break;

		case IDC_BUTTON_SETFEE:
			{
				char buf[ 256 ];
				cltGameMsgRequest_PostOfficeSetFee setfee;

				// 물품 수수료
				GetDlgItemText( hDlg, IDC_EDIT_ITEMFEERATE, buf, 32 );

				if( setfee.clItemFee.SetFeeRate( (int)( atof( buf ) * 10 ) ) == FALSE ) {
					char* pText = GetTxtFromMgr(1709);
					SetMainExplain(m_hStrDlg, pText );
					break;
				}
/*
				if(	setfee.clItemFee.SetMinFee( GetDlgItemInt( hDlg, IDC_EDIT_ITEMMINFEE, NULL, TRUE ) ) == FALSE ||
					setfee.clItemFee.SetMaxFee( GetDlgItemInt( hDlg, IDC_EDIT_ITEMMAXFEE, NULL, TRUE ) ) == FALSE
					) {
						SetMainExplain(m_hStrDlg, "수수료 설정이 잘못되었습니다." );
						break;
					}
*/
				// 송금 수수료
				GetDlgItemText( hDlg, IDC_EDIT_MONEYFEERATE, buf, 32 );

				if( setfee.clMoneyFee.SetFeeRate( (int)( atof( buf ) * 10 ) ) == FALSE ) {
					char* pText = GetTxtFromMgr(1709);
					SetMainExplain(m_hStrDlg, pText );
					break;
				}
/*
				if(	setfee.clMoneyFee.SetMinFee( GetDlgItemInt( hDlg, IDC_EDIT_MONEYMINFEE, NULL, TRUE ) ) == FALSE ||
					setfee.clMoneyFee.SetMaxFee( GetDlgItemInt( hDlg, IDC_EDIT_MONEYMAXFEE, NULL, TRUE ) ) == FALSE
					) {
						SetMainExplain(m_hStrDlg, "수수료 설정이 잘못되었습니다." );
						break;
					}
*/
				
				// 메시지 수수료
				setfee.siMsgFee = GetDlgItemInt( hDlg, IDC_EDIT_MSGFEE, NULL, TRUE );

				cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_SETFEE, sizeof( cltGameMsgRequest_PostOfficeSetFee ), (char *)&setfee );
				pclClient->SendMsgToServer( (char*)&clMsg );
			}
			break;

		case IDC_BUTTON_RESIGN_POSTOFFICE:
			{
				// 서버에 역참행수에서 사임함을 요청한다. 
				SI32 id = 1;
				if(id)
				{
					cltSimpleRank clrank( RANKTYPE_POSTOFFICE, siVillageUnique );
					
					if( pclCM->CR[id]->pclCI->clCharRank.IsSame(&clrank) == TRUE )
					{

						char buffer[ 256 ];
						char* pText = GetTxtFromMgr(1710);
						strcpy(buffer, pText);

						pText = GetTxtFromMgr(1442);

						if (  MessageBox(NULL, buffer, pText, MB_ICONQUESTION | MB_YESNO) == IDNO ) {
							break;
						}

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
