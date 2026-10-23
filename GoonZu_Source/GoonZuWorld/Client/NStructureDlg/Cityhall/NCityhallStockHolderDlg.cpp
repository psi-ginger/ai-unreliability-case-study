#include "NCityhallDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
//#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Web.h"

#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

#include "MsgType-Person.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallStockHolderDlg::NCityhallStockHolderDlg()
{
}

NCityhallStockHolderDlg::~NCityhallStockHolderDlg()
{
}

void NCityhallStockHolderDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( LIST_STOCKHOLDER, TEXT("listbox_stockholder") );
	SetControlMap( EDIT_STOCKHOLDERNAME, TEXT("editbox_stockholdername") );
		
	SetControlMap( BUTTON_OPENMINIHOME, TEXT("button_openminihome") );

	//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
	if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
	{
		m_pControlMap[BUTTON_OPENMINIHOME]->Show(false);
	}
	
	// 리스트
	CList *pList = (CList*)m_pControlMap[ LIST_STOCKHOLDER ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

//	TCHAR *text[] = { TEXT("아이디"), TEXT("보유주식") };
	TCHAR* pText[2];
	pText[0] = GetTxtFromMgr(1473);
	pText[1] = GetTxtFromMgr(1593);

	pList->SetColumn( 0, 164, pText[ 0 ] );
	pList->SetColumn( 1, 164, pText[ 1 ] );
	pList->Refresh();

	SI32 id = 1;
	if(id)
	{
		// 서버에 투자자 리스트를 요청한다. 
		{
			cltMsg clMsg(GAMEMSG_REQUEST_INVESTLIST, 0, NULL);
			
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}

	}

}

void NCityhallStockHolderDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 3 != 0 ) return;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);
	CList *pList = (CList*)m_pControlMap[ LIST_STOCKHOLDER ];
	stListBoxItemData itemdata;

	if( pCityhallManager->UpdateInvestSwitch )
	{
		pCityhallManager->UpdateInvestSwitch = FALSE;

		TCHAR buffer[256];

		pList->Clear();
		
		if( pCityhallManager->siVillageUnique < 0 )return;

		if( pclClient->pclVillageManager->pclVillageInfo[ pCityhallManager->siVillageUnique] == NULL ) return;

		for( int i = 0;i < MAX_INVEST_NUMBER;i++)
		{
			SI32 personid		= pclClient->pclVillageManager->pclVillageInfo[pCityhallManager->siVillageUnique]->clVillageVary.clInvestInfo.clInvestInfo[i].clPerson.GetPersonID();
			TCHAR* pname			= (TCHAR *)pclClient->pclVillageManager->pclVillageInfo[pCityhallManager->siVillageUnique]->clVillageVary.clInvestInfo.clInvestInfo[i].clPerson.GetName();
			SI32  stock			= pclClient->pclVillageManager->pclVillageInfo[pCityhallManager->siVillageUnique]->clVillageVary.clInvestInfo.clInvestInfo[i].GetStockNum();

			if( stock == 0 ) continue;

			itemdata.Init();
			itemdata.Set( 0, pname );

			TCHAR* pText = GetTxtFromMgr(1594);
			StringCchPrintf(buffer, 256, pText, stock );

			itemdata.Set( 1, buffer );
			pList->AddItem( &itemdata );

		}

		pList->Refresh();
	}
}

void NCityhallStockHolderDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	CASE_CONTROL( LIST_STOCKHOLDER )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				CList *pList = (CList*)m_pControlMap[ LIST_STOCKHOLDER ];

				if(pList == NULL ) return;
				SI16 siRow = -1, siCol = -1;
				pList->GetSelectedIndex( &siRow, &siCol );

				if( siRow == -1 || siCol == -1 ) return;

				TCHAR* pSelText = NULL;

				pSelText = pList->GetText( siRow, 0 );

				if( pSelText == NULL ) return;

				SetEditText( EDIT_STOCKHOLDERNAME, pSelText );                
			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_OPENMINIHOME )
	{
 		TCHAR* buf=NULL;
		TCHAR* tmpText = NULL;
		buf = GetEditText( EDIT_STOCKHOLDERNAME );
		tmpText = GetTxtFromMgr( 1561 );

		if( _tcscmp( buf, tmpText ) == 0 ) return;

		cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( buf );
		cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		/*
		cltGameMsgRequest_Userid clUserid( buf );
		cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		*/
	}
}