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

#include "HorseMarket.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-HorseMarket.h"

#include "../CityHall/TaxpaymentListDlg.h"

extern cltCommonLogic* pclClient;


void cltHorseMarketManager::ShowResponseOrder( HWND hDlg )
{
	int i;
	char buffer[256 ];
	char pricestring[64];

	// 기존의 리스트를 삭제한다. 
	//ListDelete(hDlg, IDC_LIST_HORSE );
	SendDlgItemMessage( hDlg, IDC_LIST_HORSE, LVM_DELETEALLITEMS, 0, 0 );


	bool bcheck[MAX_HORSEMARKET_RESPONSE_ORDER_NUMBER];
	for( i = 0; i < MAX_HORSEMARKET_RESPONSE_ORDER_NUMBER; i++ )
	{
		bcheck[i] = false;
	}


	LVITEM lvi;

	SI32 addnum = 0 ;
	while(addnum < siTotalResponseOrderNumber)
	{
		SI32 minprice	= 2100000000;
		SI32 minindex	= -1;	

		for( i = 0; i < siTotalResponseOrderNumber; i++ )
		{
			if( bcheck[i] == false && clResponseOrder[i].siPrice < minprice)
			{
				minprice	= clResponseOrder[i].siPrice;
				minindex	= i;
			}
		}
		
		if(minindex >= 0)
		{
			bcheck[minindex] = true;
			
			char* pText = GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit(clResponseOrder[minindex].siPrice, pricestring, pText);

			int len = strlen( clResponseOrder[minindex].clHorse.szName );
			
			if( len > 0 ) {
				strcpy( buffer, clResponseOrder[minindex].clHorse.szName );
			} else {
				pText = GetTxtFromMgr(1754);
				strcpy( buffer, pText );
			}

			lvi.iItem = addnum;

			lvi.mask = LVIF_TEXT | LVIF_PARAM;;
			lvi.iSubItem = 0;
			lvi.pszText = buffer;
			lvi.lParam = minindex;			

			SendDlgItemMessage( hDlg, IDC_LIST_HORSE, LVM_INSERTITEM, 0, (LPARAM)&lvi );

			lvi.mask = LVIF_TEXT;
			lvi.iSubItem = 1;
			lvi.pszText = pricestring;
			SendDlgItemMessage( hDlg, IDC_LIST_HORSE, LVM_SETITEM, 0, (LPARAM)&lvi );

			addnum++;
		}
	}

	siSelectedIndex = -1;

}