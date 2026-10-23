//---------------------------------
// 2004/2/18 ±èÅÂ°ï
//---------------------------------

#include <CommonLogic.h>
#include "..\..\Server\Server.h"

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "..\..\Common\Util\Control\Control.h"

#include "..\..\Resource.h"

#include "Hunt.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg\MsgType-Hunt.h"

#include "../CityHall/TaxpaymentListDlg.h"

extern cltCommonLogic* pclClient;


void cltHuntManager::LoadHuntUserDlgLineUpInfo( HWND hDlg )
{
	HWND hListView = GetDlgItem( hDlg, IDC_LIST_HUNT_LINEUP_INFO );

	ListView_DeleteAllItems( hListView );

	LVITEM LI;

	SI16 Count = 0;
	LI.mask = LVIF_TEXT; // LVIF_IMAGE;
	LI.state = 0;
	LI.stateMask = 0;
	//LI.iImage = -1;
	char Buffer[ 32 ] = "";

	for ( int i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
	{
		if ( clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind > 0 )
		{
			LI.iItem = Count++;			
			LI.iSubItem = 0;			
			char *MonsterName = (char*)pclClient->pclKindInfoSet->GetName( clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind );
			if ( MonsterName == NULL )
			{
				continue;
			}
			//strcpy( LI.pszText, MonsterName );
			LI.pszText = MonsterName;
			SendMessage( hListView, LVM_INSERTITEM, 0, (LPARAM)&LI );
			
			LI.iSubItem = 1;			
			sprintf( Buffer,  "%d", clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindLevel );
			//strcpy( LI.pszText, Buffer );
			LI.pszText = Buffer;			

			SendMessage( hListView, LVM_SETITEM, 0, (LPARAM)&LI );

			LI.iSubItem = 2;			
			sprintf( Buffer,  "%d", clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindNum );
			//strcpy( LI.pszText, Buffer );
			LI.pszText = Buffer;			
			SendMessage( hListView, LVM_SETITEM, 0, (LPARAM)&LI );
		}
	}


	return;
}
