#include "NMineDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"
#include "../CommonLogic/Msg/MsgType-Mine.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NMineListDlg::NMineListDlg()
{
}

NMineListDlg::~NMineListDlg()
{
	
}

void NMineListDlg::InitDlg()
{
	cltMineManager *pMineManager = ((cltMineManager *)m_pStrManager);

	SetControlMap( LIST_MINELIST, TEXT("listbox_houselist") );

	// 리스트 
	CList *pList = (CList*)m_pControlMap[ LIST_MINELIST ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1695);
	pList->SetColumn( 0, 64, pText );
	pText = GetTxtFromMgr(1696);
	pList->SetColumn( 1, 92, pText );
	pText = GetTxtFromMgr(1697);
	pList->SetColumn( 2, 168, pText );

	pList->Refresh();

	//-------------------------------------
	// 서버로 광산리스트를 요청한다. 
	//-------------------------------------
	cltMsg clMsg( GAMEMSG_REQUEST_MINE_UNITLIST, pMineManager->siVillageUnique );
	pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

	pMineManager->siSelectedMineUnitNumber = -1;

}

void NMineListDlg::Action()
{
	cltMineManager *pMineManager = ((cltMineManager *)m_pStrManager);

	if( pMineManager->bUpdateMineUnitListShowSwitch == true )
	{
		pMineManager->bUpdateMineUnitListShowSwitch = false;
		
		// 광산리스트를 보여준다. 
		ShowMineUnitList();
	}
}

void NMineListDlg::ShowMineUnitList()
{

	cltMineManager *pMineManager = ((cltMineManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_MINELIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();	

	for( i = 0; i < MAX_MINEUNIT_NUMBER; i++ )
	{
		itemdata.Init();

		StringCchPrintf(buffer, 256, TEXT("%d"), i+1);

		itemdata.siParam[0] = i;
		itemdata.Set( 0, buffer );

		if( pMineManager->clClientUnit[i].clPerson.GetPersonID() )
		{
			itemdata.Set( 1, (TCHAR*)pMineManager->clClientUnit[i].clPerson.GetName() );
			if ( ConstServiceArea_China != pclClient->siServiceArea )
			{
				TCHAR* pText;
				pText = GetTxtFromMgr(7532);
				//itemdata.Set( 2, TEXT("임대중") );
				itemdata.Set( 2, pText );
			}
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(3245);
			itemdata.Set( 1, pText );	
		}
 
		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}

void NMineListDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltMineManager *pMineManager = ((cltMineManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_MINELIST ];

	CASE_CONTROL( LIST_MINELIST )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{

				//TCHAR buf[ 256 ];

				int index = pList->GetSelectedIndex();

				if( index < 0 ) return;

				stListBoxItemData *pItemData = pList->GetItemData( index, 0 );

				pMineManager->siSelectedMineUnitNumber = pItemData->siParam[0];

			}
			break;
		}
	}

}