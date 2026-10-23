#include "NCityhallDlg.h"

#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-Web.h"
#include "..\..\Client\Music\Music.h"

#include "../../Common/HintDlg/HintDlg.h"

#include "../Lib/JYLibFunction.h"


#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "MsgType-Person.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

extern cltRankCandidate clRankCandidate[];

NCityhallInfoDlg::NCityhallInfoDlg()
{
}

NCityhallInfoDlg::~NCityhallInfoDlg()
{
}

void NCityhallInfoDlg::InitDlg()
{
	TCHAR* pText1 = GetTxtFromMgr(5004);
	for(int sun_count=0; sun_count<16;sun_count++)
	{
		pText1 = GetTxtFromMgr(5004+sun_count);
		StringCchCopy(clRankCandidate[sun_count].szStructureName, 64,pText1);

		pText1 = GetTxtFromMgr(5020+sun_count);
		StringCchCopy(clRankCandidate[sun_count].szRankName, 64, pText1);

		pText1 = GetTxtFromMgr(5036+sun_count);
		StringCchCopy(clRankCandidate[sun_count].szExplain, 256, pText1);
	}

	pText1 = GetTxtFromMgr(5360);	// 상단관리소
	StringCchCopy(clRankCandidate[sun_count].szStructureName, 64, pText1);
	pText1 = GetTxtFromMgr(5361);
	StringCchCopy(clRankCandidate[sun_count].szRankName, 64, pText1);
	pText1 = GetTxtFromMgr(5362);
	StringCchCopy(clRankCandidate[sun_count].szExplain, 256, pText1);
	++sun_count;

	// [진성] 통합마켓.
	pText1 = GetTxtFromMgr(9832);	
	StringCchCopy(clRankCandidate[sun_count].szStructureName, 64, pText1);
	pText1 = GetTxtFromMgr(9835);	
	StringCchCopy(clRankCandidate[sun_count].szRankName, 64, pText1);
	pText1 = GetTxtFromMgr(9836);	
	StringCchCopy(clRankCandidate[sun_count].szExplain, 256, pText1);
	++sun_count;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( EDIT_VILLAGE_SCORE, TEXT("editbox_village_score") );
	SetControlMap( EDIT_RESIDENTNUM, TEXT("editbox_residentnum") );
	SetControlMap( EDIT_STRDURA, TEXT("editbox_strdura") );
	SetControlMap( EDIT_CHIEFNAME, TEXT("editbox_chiefname") );

	SetControlMap( LIST_RANKLIST, TEXT("listbox_ranklist") );
		
	SetControlMap( BUTTON_BOARD, TEXT("button_board") );

	SetControlMap( BUTTON_OPENMINIHOME, TEXT("button_openminihome") );

	//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
	if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
	{
		m_pControlMap[BUTTON_OPENMINIHOME]->Show(false);
	}
	
	CButton *pButton = (CButton*)m_pControlMap[BUTTON_BOARD];
	pButton->Show(FALSE);

	cltClient *pclclient = (cltClient*)pclClient;

	
	// 리스트
	CList *pList = (CList*)m_pControlMap[ LIST_RANKLIST ];
	stListBoxItemData itemdata;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

//	int i;

//	TCHAR *text[] = { TEXT("시설명"), TEXT("직책"), TEXT("아이디") };	
	TCHAR* pText[3];

	pText[0] = GetTxtFromMgr(1583);
	pText[1] = GetTxtFromMgr(1584);
	pText[2] = GetTxtFromMgr(1473);

	pList->SetColumn( 0, 110, pText[ 0 ] );
	pList->SetColumn( 1, 130, pText[ 1 ] );
	pList->SetColumn( 2, 80, pText[ 2 ] );
/*
	for( i = 0; i < 16; ++i ) {

		itemdata.Init();
		itemdata.Set( 0, clRankCandidate[ i ].szStructureName );
		itemdata.Set( 1, clRankCandidate[ i ].szRankName );

		pList->AddItem( &itemdata );
	}
*/
	pList->Refresh();

	//------------------------
	cltMsg clMsg(GAMEMSG_REQUEST_VILLAGELEVELDETAILINFO, 0);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

}

void NCityhallInfoDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 3 != 0 ) return;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	// 리스트
	CList *pList = (CList*)m_pControlMap[ LIST_RANKLIST ];
	stListBoxItemData itemdata;

	TCHAR buf[ 256 ];

	if( pCityhallManager->bChiefInfo == TRUE ) {
		pCityhallManager->bChiefInfo = FALSE;

		pList->Clear();

		int i, j;
		for( i = 1; i < MAX_RANKTYPE_NUMBER; ++i ) {

			for( j = 0; j < MAX_RANKTYPE_NUMBER; ++j ) {				
				
				if( pCityhallManager->clChiefInfo[ i ].ranktype &&
					pCityhallManager->clChiefInfo[ i ].ranktype == 
					clRankCandidate[ j ].siRankType ) {

					itemdata.Init();
					itemdata.Set( 0, clRankCandidate[ j ].szStructureName );
					itemdata.Set( 1, clRankCandidate[ j ].szRankName );
					itemdata.Set( 2, pCityhallManager->clChiefInfo[ i ].szID );

					pList->AddItem( &itemdata );

				}
			}
		}

		pList->Refresh();
	}

	// 마을 주민 수를 보여준다 
	_itot( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siResidentNum, buf, 10 );
	SetEditText( EDIT_RESIDENTNUM, buf );
	
	//// 부모 마을을 보여준다 
	//if( pCityhallManager->clClientStrInfo.clCityhallStrInfo.siParentVillage == 0 ) {
	//	TCHAR* pText = GetTxtFromMgr(1561);
	//	StringCchCopy( buf, 256, pText );
	//} else {
	//	StringCchCopy( buf, 256, pclClient->pclVillageManager->pclVillageInfo[ pCityhallManager->clClientStrInfo.clCityhallStrInfo.siParentVillage ]->szName );
	//}
	//SetEditText( EDIT_VILLAGE_SCORE, buf );

	// 마을 점수를 보여준다.
	if (pCityhallManager->siVillageUnique > 0)
	{
		SI32 villageScore = pclClient->pclVillageManager->pclVillageInfo[ pCityhallManager->siVillageUnique ]->clVillageVary.siTotalVillageScore;

		g_JYLibFunction.SetNumUnit( villageScore, buf, 256);


//		StringCchPrintf( buf, 256, TEXT("%d"), pclClient->pclVillageManager->pclVillageInfo[ pCityhallManager->siVillageUnique ]->clVillageVary.siTotalVillageScore);

	}
	else
	{
		StringCchPrintf( buf, 256, TEXT("%d"), 0);
	}
	
	SetEditText( EDIT_VILLAGE_SCORE, buf );


	// 건물 내구도를 표시한다
	SI32 siDura = pCityhallManager->clClientStrInfo.clCommonInfo.siStrDura;
	TCHAR dura[128] = TEXT("");
	g_JYLibFunction.SetNumUnit( siDura, dura, 128 );
	StringCchPrintf( buf, 256, TEXT("%s/%s"), dura, TEXT("10,000") );
	SetEditText( EDIT_STRDURA, buf );

}

void NCityhallInfoDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);


	//CASE_CONTROL( BUTTON_VILLAGETREE )
	//{
	//	//pCityhallManager->pclDeclareWarDlg->Create( NULL, pclClient->GetHwnd(), pCityhallManager );

	//	cltClient *pclclient = (cltClient*)pclClient;

	//	if( pclclient->m_pDialog[ NVILLAGETREE_DLG ] == NULL )
	//		pclclient->CreateInterface( NVILLAGETREE_DLG );
	//}

	//else CASE_CONTROL( BUTTON_LEVEL_EXPLAIN )
	//{
	//	cltClient *pclclient = (cltClient*)pclClient;
	//	
	//	
	//	//if ( g_SoundOnOffDlg.m_bFullScreenMode )
	//	//{
	//		if((pclclient->m_pDialog[ NHINT_DLG ])==NULL)
	//		{
	//			pclclient->CreateInterface( NHINT_DLG );
	//		}
	//		((NHintDlgExplain*)pclclient->m_pDialog[ NHINT_DLG])->Show(TEXT("townlevel"));

	//	//}
	//	//else
	//	//{
	//	//	if ( ((cltClient*)pclClient)->m_pHintDlg )
	//	//	{
	//	//		((cltClient*)pclClient)->m_pHintDlg->Show(TEXT("townlevel"));
	//	//	}
	//	//}

	//}

	CASE_CONTROL( BUTTON_BOARD )
	{
		cltClient *pclclient = (cltClient *)pclClient;

		/*cyj 마을게시판 임시로 동작안하도록 수정
		if ( pclclient->pclCM->CR[1]->pclCI->clBI.siHomeVillage == 
			pCityhallManager->siVillageUnique )
			pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_VILLAGERESIDENT, TEXT(""));

		else pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_VILLAGERESIDENT, TEXT(""),
			pCityhallManager->siVillageUnique);
		*/
		

	}
	
	else CASE_CONTROL( LIST_RANKLIST )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				CList *pList = (CList*)m_pControlMap[ LIST_RANKLIST ];

				if(pList == NULL ) return;
				SI16 siRow = -1, siCol = -1;
				pList->GetSelectedIndex( &siRow, &siCol );

				if( siRow == -1 || siCol == -1 ) return;

				TCHAR* pSelText = NULL;

				pSelText = pList->GetText( siRow, 2 );

				if( pSelText == NULL ) return;

				SetEditText( EDIT_CHIEFNAME, pSelText );                
			}
			break;
		}
	}

	else CASE_CONTROL( BUTTON_OPENMINIHOME )
	{
 		TCHAR* buf=NULL;
		TCHAR* tmpText = NULL;
		buf = GetEditText( EDIT_CHIEFNAME );
		tmpText = GetTxtFromMgr( 1561 );

		if( _tcscmp( buf, tmpText ) == 0 ) return;

		/*
		cltGameMsgRequest_Userid clUserid( buf );
		cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		*/

		cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( buf );
		cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	}
}