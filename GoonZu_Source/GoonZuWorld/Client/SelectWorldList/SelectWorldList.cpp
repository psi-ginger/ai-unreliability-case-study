//---------------------------------
// 2003/7/29 이상민
//---------------------------------
#include "..\Client.h"
#include "..\..\resource.h"

#include "../../lib/WebHTML/WebHTML.h"
#include "../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"
#include "../Client/NInterface/NSelectWorldListDlg/SelectWorldListDlg.h"

extern cltCommonLogic* pclClient;

#define TESTSERVER_ID		11

// 적절한 월드가  선택되었는지 확인한다. 
bool cltClient::SelectWorldList()
{
	if( g_SoundOnOffDlg.m_bFullScreenMode )
	{
		if( ((CNSelectWorldListDlg*)m_pDialog[ NSELECTWORLDLIST_DLG ])->IsShow() == false)
		{
			return true;
		}
	}
	else
	{
		if( SelectWorldListDlg == NULL )
		{
			return true;
		}
	}
	
//	if( SelectWorldListDlg == NULL )
//	{
//		return true;
//	}

	return false;
}



bool cltClient::CreateSelectWorldListDlg(bool bShowBanner)
{
	// 클라이언트인 경우에만 처리한다.(맵에디터에서는 사용하지 않는다. ) 
	if( GameMode != GAMEMODE_CLIENT )
	{
		return true;
	}

	if( clWorldList.GetCount() == 0 )
	{
		return false;
	}

	// 0번 월드의 정보를 얻어온다. 
	//clWorldList.GetWorldInfo( 1, &clSelectedWorldInfo );

	//	CreateInterface( NSELECTWORLDLIST_DLG );

	if(g_SoundOnOffDlg.m_bFullScreenMode)
	{
		if(m_pDialog[ NSELECTWORLDLIST_DLG ] && ((CNSelectWorldListDlg*)m_pDialog[ NSELECTWORLDLIST_DLG ])->IsShow() == false)
		{
			((CNSelectWorldListDlg*)m_pDialog[ NSELECTWORLDLIST_DLG ])->Show();

			// 배너를 보여준다
			if(bShowBanner)				
				CreateInterface( NOPENINGBANNER_DLG );

			return true;
		}
	}
	
	// 대화상자가 없는 상태라면 만든다. 
/*	if( ! IsWindow( SelectWorldListDlg ) )
	{
		SelectWorldListDlg = CreateDialog( GetInstance(), MAKEINTRESOURCE( IDD_DIALOG_SELECTWORLDLIST ), GetHwnd(), (DLGPROC)StaticSelectWorldListDlgProc );
		
		ShowWindow(SelectWorldListDlg, SW_SHOW);
		return true;
	}
*/

	return false;
}


