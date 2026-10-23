#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"

#include "CityHall.h"

#include "..\..\Common\Util\Control\Control.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Resource.h"
#include "../client/client.h"

#include "CityHall-SubmitDlg.h"
#include "MsgType-Structure.h"

extern cltCommonLogic* pclClient;

extern cltSuggestDlg g_cityhallSuggestdlg;


void cltCityHallManager::InitVoteList( HWND hDlg )
{

	HWND hlc1 = GetDlgItem( hDlg, IDC_LIST_AGREEMENT );
	HWND hlc2 = GetDlgItem( hDlg, IDC_LIST_OPPOSITION );

	ListView_SetExtendedListViewStyle( hlc1, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
	ListView_SetExtendedListViewStyle( hlc2, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );

	LVCOLUMN lvcol;

	int width[] = { 90, 60 };
//	char *text[] = { "이름", "주식수" };

	char* pText[2];

	pText[0] = GetTxtFromMgr(1556);
	pText[1] = GetTxtFromMgr(1475);
	lvcol.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvcol.fmt = LVCFMT_LEFT;

	for( int i = 0; i < 2; ++i ) {
		lvcol.cx = width[ i ];
		lvcol.pszText = pText[i]; //text[ i ];
		lvcol.iSubItem = i;

		SendMessage( hlc1, LVM_INSERTCOLUMN, i, (LPARAM)&lvcol );
		SendMessage( hlc2, LVM_INSERTCOLUMN, i, (LPARAM)&lvcol );
	}


}

void cltCityHallManager::ShowMeetingInfo( HWND hDlg )
{
	char buffer[ 1024 ];
	
	switch( clGeneralMeetingInfo.siMatterType ) {
	case 0:
		{
			char* pText = GetTxtFromMgr(1557);
			strcpy( buffer, pText);

		}
		break;

	case 1:
		{
			if( clGeneralMeetingInfo.siAcceptCandidate == 1 ) {
				
				char* pText = GetTxtFromMgr(1558);
				sprintf( buffer, pText,	clGeneralMeetingInfo.szCandidateName
					);

			} else {

				char* pText = GetTxtFromMgr(1559);
				sprintf( buffer, pText,
					clGeneralMeetingInfo.szCandidateName,
					clGeneralMeetingInfo.szCandidateName
					);
			}
		}
		break;


	}

	SetDlgItemText( hDlg, IDC_EDIT_SUGGESTION, buffer );
}

void cltCityHallManager::ShowVoterList( HWND hDlg )
{
	char buffer[256];

	SI32 agreestock = 0;
	SI32 oppositstock = 0;

	SI32 agreeindex = 0;
	SI32 oppositindex = 0;

	HWND hAgreeList = GetDlgItem( hDlg, IDC_LIST_AGREEMENT );
	HWND hOppositList = GetDlgItem( hDlg, IDC_LIST_OPPOSITION );

	SendMessage( hAgreeList, LVM_DELETEALLITEMS, 0, 0 );
	SendMessage( hOppositList, LVM_DELETEALLITEMS, 0, 0 );	

	if( clGeneralMeetingInfo.siMatterType == 0 ) return;

	LVITEM lvi;

	lvi.mask = LVIF_TEXT;

	for( int i = 0; i < 100; ++i ) {

		if( clGeneralMeetingInfo.clVoter[ i ].siPersonID == 0 ) continue;

		if( clGeneralMeetingInfo.clVoter[ i ].siVoteType == 1 ) {
			
			lvi.iItem = agreeindex;
			
			lvi.iSubItem = 0;
			lvi.pszText = clGeneralMeetingInfo.clVoter[ i ].szPersonName;

			SendMessage( hAgreeList, LVM_INSERTITEM, 0, (LPARAM)&lvi );

			itoa( clGeneralMeetingInfo.clVoter[ i ].siStockAmount, buffer, 10 );

			lvi.iSubItem = 1;
			lvi.pszText = buffer;
			SendMessage( hAgreeList, LVM_SETITEM, 0, (LPARAM)&lvi );

			agreeindex++;

			agreestock += clGeneralMeetingInfo.clVoter[ i ].siStockAmount;

		} else if( clGeneralMeetingInfo.clVoter[ i ].siVoteType == 2 ) {


			lvi.iItem = oppositindex;
			
			lvi.iSubItem = 0;
			lvi.pszText = clGeneralMeetingInfo.clVoter[ i ].szPersonName;

			SendMessage( hOppositList, LVM_INSERTITEM, 0, (LPARAM)&lvi );

			itoa( clGeneralMeetingInfo.clVoter[ i ].siStockAmount, buffer, 10 );

			lvi.iSubItem = 1;
			lvi.pszText = buffer;
			SendMessage( hOppositList, LVM_SETITEM, 0, (LPARAM)&lvi );

			oppositindex++;

			oppositstock += clGeneralMeetingInfo.clVoter[ i ].siStockAmount;
		}
	}

	char* pText = GetTxtFromMgr(1560);

	sprintf( buffer,pText, agreeindex, agreestock );
	SetDlgItemText( hDlg, IDC_EDIT_AGREEMENT, buffer );

	sprintf( buffer, pText, oppositindex, oppositstock );
	SetDlgItemText( hDlg, IDC_EDIT_OPPOSITION, buffer );

}

void cltCityHallManager::ShowLeftTime( HWND hDlg )
{
	if( clGeneralMeetingInfo.siMatterType == 0 ) {
		char* pText = GetTxtFromMgr(1561);
		SetDlgItemText( hDlg, IDC_EDIT_LEFTTIME, pText );
		return;
	}

	char buf[ 256 ];

	cltDate *pclCurrentDate = &((cltClient *)pclClient)->clClientDate;
	cltDate clLeftDate;

	if( pclCurrentDate->IsLaterMonthThan( &pclClient->pclCityHallManager->clGeneralMeetingInfo.clCloseDate ) &&
		pclCurrentDate->IsLaterDayThan( &pclClient->pclCityHallManager->clGeneralMeetingInfo.clCloseDate ) ) 
	{
		char* pText = GetTxtFromMgr(1561);
		SetDlgItemText( hDlg, IDC_EDIT_LEFTTIME, pText );
		pText = GetTxtFromMgr(1557);
		char buffer[ 1024 ];
		strcpy( buffer, pText);
		SetDlgItemText( hDlg, IDC_EDIT_SUGGESTION, buffer );

		pText = GetTxtFromMgr(1560);

		sprintf( buffer,pText, 0, 0 );
		SetDlgItemText( hDlg, IDC_EDIT_AGREEMENT, buffer );

		sprintf( buffer, pText, 0, 0 );
		SetDlgItemText( hDlg, IDC_EDIT_OPPOSITION, buffer );

		HWND hAgreeList = GetDlgItem( hDlg, IDC_LIST_AGREEMENT );
		HWND hOppositList = GetDlgItem( hDlg, IDC_LIST_OPPOSITION );

		SendMessage( hAgreeList, LVM_DELETEALLITEMS, 0, 0 );
		SendMessage( hOppositList, LVM_DELETEALLITEMS, 0, 0 );	

		return;
	}

	clLeftDate.DiffDate( &clGeneralMeetingInfo.clCloseDate, pclCurrentDate );

	if( clLeftDate.uiMonth == 0 ) {
		char* pText = GetTxtFromMgr(1562);
		sprintf( buf, pText, clLeftDate.uiDay, clLeftDate.uiHour );
	} else {
		char* pText = GetTxtFromMgr(1563);
		sprintf( buf, pText, clLeftDate.uiMonth, clLeftDate.uiDay );
	}

	SetDlgItemText( hDlg, IDC_EDIT_LEFTTIME, buf );

}

