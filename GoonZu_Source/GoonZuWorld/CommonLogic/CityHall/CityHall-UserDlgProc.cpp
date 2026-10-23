//---------------------------------
// 2003/6/12 ±èÅÂ°ï
//---------------------------------
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "CityHall.h"

#include "..\..\Common\Util\Control\Control.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Server\Candidate\Candidate.h"
#include "..\..\Server\Rank\Rank.h"
#include "AppointmentMayorDlg.h"
#include "ResidentListDlg.h"
#include "MsgType-Person.h"


#include "..\..\Resource.h"

#include "..\..\CommonLogic\MsgType-Structure.h"

extern cltCommonLogic* pclClient;

void cltCityHallManager::ShowChiefDate( HWND hDlg )
{
	char buffer[256 ];

	if( bRecvChiefInfo ) {
		
		strcpy( buffer, clChiefInfo[ 1 ].szID );
		SetDlgItemText( hDlg, IDC_EDIT_CHIEFNAME, buffer );

		char* pText = GetTxtFromMgr(1561);

		if( strcmp( clChiefInfo[ 1 ].szID, pText) == 0 ) {
			SetDlgItemText( hDlg, IDC_EDIT_BEGINCHIEF, buffer );
			SetDlgItemText( hDlg, IDC_EDIT_ENDCHIEF, buffer );
			return;
		}


		cltDate clDate;
		clDate.Set( &clClientStrInfo.clCityhallStrInfo.clChiefDate );

		pText = GetTxtFromMgr(1605);

		sprintf( buffer, pText, clDate.uiYear, clDate.uiMonth, clDate.uiDay );
		SetDlgItemText( hDlg, IDC_EDIT_BEGINCHIEF, buffer );

		clDate.uiYear = clDate.uiYear + 2;

		sprintf( buffer, pText, clDate.uiYear, clDate.uiMonth, clDate.uiDay );
		SetDlgItemText( hDlg, IDC_EDIT_ENDCHIEF, buffer );

	} else {


		char* pText = GetTxtFromMgr(1561);

		strcpy( buffer, pText );

		SetDlgItemText( hDlg, IDC_EDIT_CHIEFNAME, buffer );
		SetDlgItemText( hDlg, IDC_EDIT_BEGINCHIEF, buffer );
		SetDlgItemText( hDlg, IDC_EDIT_ENDCHIEF, buffer );
	}

}

