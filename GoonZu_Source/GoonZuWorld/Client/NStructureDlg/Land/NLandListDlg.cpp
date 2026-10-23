#include "NLandDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "../CommonLogic/Msg/MsgType-Land.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NLandListDlg::NLandListDlg()
{
}

NLandListDlg::~NLandListDlg()
{
	
}

void NLandListDlg::InitDlg()
{
	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);

	SetControlMap( LIST_LANDLIST, TEXT("listbox_houselist") );

	// 리스트 
	CList *pList = (CList*)m_pControlMap[ LIST_LANDLIST ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(1695);
	pList->SetColumn( 0, 64, pText );
	pText = GetTxtFromMgr(1696);

	pList->SetColumn( 1, 122, pText );
	pText = GetTxtFromMgr(1697);
	pList->SetColumn( 2, 138, pText );
	//긴이름의 유저 일경우 이름이 짤리므로 이름 폭조절 

	pList->Refresh();

	//-------------------------------------
	// 서버로 농장리스트를 요청한다. 
	//-------------------------------------
	cltMsg clMsg( GAMEMSG_REQUEST_LAND_UNITLIST, pLandManager->siVillageUnique );
	pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

	pLandManager->siSelectedLandUnitNumber = -1;

}

void NLandListDlg::Action()
{
	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);

	if( pLandManager->bUpdateLandUnitListShowSwitch == true )
	{
		pLandManager->bUpdateLandUnitListShowSwitch = false;
		
		// 농장리스트를 보여준다. 
		ShowLandUnitList();
	}
}

void NLandListDlg::ShowLandUnitList()
{

	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_LANDLIST ];
	stListBoxItemData itemdata;

	SI32 i;
	TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();	

	for( i = 0; i < MAX_LANDUNIT_NUMBER; i++ )
	{
		itemdata.Init();

		StringCchPrintf(buffer, 256, TEXT("%d"), i+1);

		itemdata.siParam[0] = i;
		itemdata.Set( 0, buffer );

		if( pLandManager->clClientUnit[i].clPerson.GetPersonID() )
		{
			itemdata.Set( 1, (TCHAR*)pLandManager->clClientUnit[i].clPerson.GetName() );
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
			TCHAR* pText = GetTxtFromMgr(1824);
			itemdata.Set( 1, pText );
		}

		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}

void NLandListDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltLandManager *pLandManager = ((cltLandManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_LANDLIST ];

	CASE_CONTROL( LIST_LANDLIST )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{

//				TCHAR buf[ 256 ];

				int index = pList->GetSelectedIndex();

				if( index < 0 ) return;

				stListBoxItemData *pItemData = pList->GetItemData( index, 0 );

				pLandManager->siSelectedLandUnitNumber = pItemData->siParam[0];

			}
			break;
		}
	}
}