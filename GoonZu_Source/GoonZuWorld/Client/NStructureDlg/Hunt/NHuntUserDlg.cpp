#include "NHuntDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
//#include "../StrBaseDlg/StrInventory.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg\MsgType-Hunt.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NHuntUserDlg::NHuntUserDlg()
{

}


NHuntUserDlg::~NHuntUserDlg()
{
	
}


void NHuntUserDlg::InitDlg()
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);


	SetControlMap( EDIT_EXPLAIN, TEXT("editbox_explain") );
	SetControlMap( LIST_MONSTERINFO, TEXT("listbox_monsterinfo") );
	SetControlMap( BUTTON_ENTER, TEXT("button_enter") );

	// ¸®½ºÆ® 
	CList *pList = (CList*)m_pControlMap[ LIST_MONSTERINFO ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1705);
	pList->SetColumn( 0, 160, pText );

	pText = GetTxtFromMgr(1506);
	pList->SetColumn( 1, 88, pText );

	pText = GetTxtFromMgr(1706);
	pList->SetColumn( 2, 88, pText );

	pList->Refresh();

}

void NHuntUserDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 5 ) return;

	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	TCHAR buffer[ 256 ];

	if( pHuntManager->siHuntAddRate )
	{
		TCHAR* pText = GetTxtFromMgr(1707);
		StringCchPrintf( buffer, 256, pText, pHuntManager->siHuntAddRate );
	}
	else
	{
		TCHAR* pText = GetTxtFromMgr(1708);
		StringCchPrintf(buffer, 256, pText);
	}

	SetEditText( EDIT_EXPLAIN, buffer );
	
	//LoadHuntUserDlgLineUpInfo( hDlg );	

	CList *pList = (CList*)m_pControlMap[ LIST_MONSTERINFO ];
	stListBoxItemData itemdata;

	pList->Clear();

	SI16 count = 0;
	for ( int i = 0; i < MAX_HUNT_LINEUP_NUMBER; ++i )
	{
		if ( pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind > 0 )
		{
			count++;			
			
			TCHAR *MonsterName = (TCHAR*)pclClient->pclKindInfoSet->GetName( pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKind );
			if( MonsterName == NULL ) continue;

			itemdata.Init();
			itemdata.Set( 0, MonsterName );
			
			StringCchPrintf( buffer,  256, TEXT("%d"), pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindLevel );
			itemdata.Set( 1, buffer );

			StringCchPrintf( buffer,  256, TEXT("%d"), pHuntManager->clClientStrInfo.clHuntStrInfo.clHuntLineUp[ i ].siLineUpKindNum );
			itemdata.Set( 2, buffer );

			pList->AddItem( &itemdata );
		}
	}

	pList->Refresh();

}


void NHuntUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltHuntManager *pHuntManager = ((cltHuntManager *)m_pStrManager);

	CASE_CONTROL( BUTTON_ENTER )
	{
		if(pclCM->IsAlive(1) == false) return;

		cltGameMsgRequest_EnterHunt clinfo(pclCM->CR[1]->GetCurrentVillageUnique());
		cltMsg clMsg( GAMEMSG_REQUEST_ENTERHUNT, sizeof(clinfo), (BYTE*)&clinfo );
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	}

}