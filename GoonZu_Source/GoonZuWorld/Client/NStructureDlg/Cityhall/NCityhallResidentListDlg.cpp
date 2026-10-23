/* ==========================================================================
	클래스 :		NResidentListDlg

	작성일 :		05/04/28
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include "./NCityhallResidentListDlg.h" 

#include <tchar.h>
#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "MsgType-Structure.h"
#include "Msg/MsgType-Web.h"
#include "MsgType-Person.h"
#include "../../Client/client.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/OutLine.h"
#include "../../../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

cltStructureMngBasic*	NResidentListDlg::m_pclStructureManager;

extern cltCommonLogic* pclClient;

NResidentListDlg::NResidentListDlg()
{
	m_bShow = false;

	m_pStatic_resident_search	 = NULL; 
	m_pStatic_id	 = NULL; 
	m_pStatic_resident_info	 = NULL; 
	m_pStatic_score	 = NULL; 
	m_pButton_residentlist	 = NULL; 
	m_pButton_residentinfo	 = NULL; 
	m_pButton_openminihome	 = NULL; 
	m_pButton_expulsion_resident	 = NULL; 
	m_pButton_setscore	 = NULL; 
	m_pEdit_name	 = NULL; 
	m_pEdit_residentinfo	 = NULL; 
	m_pEdit_score	 = NULL; 
	m_pList_resident	 = NULL; 
	m_pComboBox_sort	 = NULL; 
	m_pOutline_NONAME1	 = NULL; 
	m_pOutline_NONAME2	 = NULL; 
	m_pOutline_NONAME3	 = NULL; 
    
}

NResidentListDlg::~NResidentListDlg()
{
	if( 	m_pStatic_resident_search	 )  	delete 	m_pStatic_resident_search	;
	if( 	m_pStatic_id	 )  	delete 	m_pStatic_id	;
	if( 	m_pStatic_resident_info	 )  	delete 	m_pStatic_resident_info	;
	if( 	m_pStatic_score	 )  	delete 	m_pStatic_score	;
	if( 	m_pButton_residentlist	 )  	delete 	m_pButton_residentlist	;
	if( 	m_pButton_residentinfo	 )  	delete 	m_pButton_residentinfo	;
	if( 	m_pButton_openminihome	 )  	delete 	m_pButton_openminihome	;
	if( 	m_pButton_expulsion_resident	 )  	delete 	m_pButton_expulsion_resident	;
	if( 	m_pButton_setscore	 )  	delete 	m_pButton_setscore	;
	if( 	m_pEdit_name	 )  	delete 	m_pEdit_name	;
	if( 	m_pEdit_residentinfo	 )  	delete 	m_pEdit_residentinfo	;
	if( 	m_pEdit_score	 )  	delete 	m_pEdit_score	;
	if( 	m_pList_resident	 )  	delete 	m_pList_resident	;
	if( 	m_pComboBox_sort	 )  	delete 	m_pComboBox_sort	;
	if( 	m_pOutline_NONAME1	 )  	delete 	m_pOutline_NONAME1	;
	if( 	m_pOutline_NONAME2	 )  	delete 	m_pOutline_NONAME2	;
	if( 	m_pOutline_NONAME3	 )  	delete 	m_pOutline_NONAME3	;

	SetTimerOnOff( false );

}

void NResidentListDlg::Create(  )
{
	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_ResidentListDlg.ddf"));

		file.CreatePopupDialog( this, NRESIDENTLIST_DLG, TEXT("dialog_residentlist"), NResidentListStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_residentlist") );

		if(siTmp < 0 )
			siTmpWidth = DLG_BIG_POS_X;
		else
			siTmpWidth = file.m_control[ siTmp  ].width;

		if(((CNSoundOnOffDlg*)pclclient->m_pDialog[ NSOUNDONOFF_DLG ])->m_bBigDisplayMode == true )
		{
			this->MovePos(DLG_BIG_POS_X - siTmpWidth, DLG_BIG_POS_Y);
		}
		else
		{
			this->MovePos(DLG_SMALL_POS_X - siTmpWidth, DLG_SMALL_POS_Y);
		}

		m_pStatic_resident_search	 = new 	CStatic	( this )	;
		m_pStatic_id	 = new 	CStatic	( this )	;
		m_pStatic_resident_info	 = new 	CStatic	( this )	;
		m_pStatic_score	 = new 	CStatic	( this )	;
		m_pButton_residentlist	 = new 	CButton	( this )	;
		m_pButton_residentinfo	 = new 	CButton	( this )	;
		m_pButton_openminihome	 = new 	CButton	( this )	;
		m_pButton_expulsion_resident	 = new 	CButton	( this )	;
		m_pButton_setscore	 = new 	CButton	( this )	;
		m_pEdit_name	 = new 	CEdit	( this )	;
		m_pEdit_residentinfo	 = new 	CEdit	( this )	;
		m_pEdit_score	 = new 	CEdit	( this )	;
		m_pList_resident	 = new 	CList	( this )	;
		m_pComboBox_sort	 = new 	CComboBox	( this )	;
		m_pOutline_NONAME1	 = new 	COutLine	( this )	;
		m_pOutline_NONAME2	 = new 	COutLine	( this )	;
		m_pOutline_NONAME3	 = new 	COutLine	( this )	;

		file.CreateControl( 	m_pStatic_resident_search	, 	NRESIDENTLIST_STATICTEXT_RESIDENT_SEARCH	, TEXT("statictext_resident_search") )	;
		file.CreateControl( 	m_pStatic_id	, 	NRESIDENTLIST_STATICTEXT_ID	, TEXT("statictext_id") )	;
		file.CreateControl( 	m_pStatic_resident_info	, 	NRESIDENTLIST_STATICTEXT_RESIDENT_INFO	, TEXT("statictext_resident_info") )	;
		file.CreateControl( 	m_pStatic_score	, 	NRESIDENTLIST_STATICTEXT_SCORE	, TEXT("statictext_score") )	;
		file.CreateControl( 	m_pButton_residentlist	, 	NRESIDENTLIST_BUTTON_RESIDENTLIST	, TEXT("button_residentlist") )	;
		file.CreateControl( 	m_pButton_residentinfo	, 	NRESIDENTLIST_BUTTON_RESIDENTINFO	, TEXT("button_residentinfo") )	;
		file.CreateControl( 	m_pButton_openminihome	, 	NRESIDENTLIST_BUTTON_OPENMINIHOME	, TEXT("button_openminihome") )	;
		file.CreateControl( 	m_pButton_expulsion_resident	, 	NRESIDENTLIST_BUTTON_EXPULSION_RESIDENT	, TEXT("button_expulsion_resident") )	;
		file.CreateControl( 	m_pButton_setscore	, 	NRESIDENTLIST_BUTTON_SETSCORE	, TEXT("button_setscore") )	;
		file.CreateControl( 	m_pEdit_name	, 	NRESIDENTLIST_EDITBOX_NAME	, TEXT("editbox_name") )	;
		file.CreateControl( 	m_pEdit_residentinfo	, 	NRESIDENTLIST_EDITBOX_RESIDENTINFO	, TEXT("editbox_residentinfo") )	;
		file.CreateControl( 	m_pEdit_score	, 	NRESIDENTLIST_EDITBOX_SCORE	, TEXT("editbox_score") )	;
		file.CreateControl( 	m_pList_resident	, 	NRESIDENTLIST_LISTBOX_RESIDENT	, TEXT("listbox_resident") )	;
		file.CreateControl( 	m_pComboBox_sort	, 	NRESIDENTLIST_COMBOBOX_SORT	, TEXT("combobox_sort") )	;
		file.CreateControl( 	m_pOutline_NONAME1	, 	NRESIDENTLIST_OUTLINE_NONAME1	, TEXT("outline_NONAME1") )	;
		file.CreateControl( 	m_pOutline_NONAME2	, 	NRESIDENTLIST_OUTLINE_NONAME2	, TEXT("outline_NONAME2") )	;
		file.CreateControl( 	m_pOutline_NONAME3	, 	NRESIDENTLIST_OUTLINE_NONAME3	, TEXT("outline_NONAME3") )	;

		//m_pclStructureManager = pclStructureManager;
		m_pclStructureManager = pclClient->pclCityHallManager;

		m_pEdit_name->SetMaxEditTextLength( 21 );
		m_pEdit_score->SetNumberMode( true );

		TCHAR* pText[8];

		pText[0] = GetTxtFromMgr(1647);
		pText[1] = GetTxtFromMgr(1648);
		pText[2] = GetTxtFromMgr(1649);
		pText[3] = GetTxtFromMgr(1650);
		pText[4] = GetTxtFromMgr(1651);
		pText[5] = GetTxtFromMgr(1652);
		pText[6] = GetTxtFromMgr(1653);
		pText[7] = GetTxtFromMgr(1654);

		// 콤보 박스 초기화 
		for( SI16 i = 0; i < 8; ++i ) {
			stComboBoxItemData tmpComboBoxItemData;
			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( pText[ i ] );

			m_pComboBox_sort->AddItem( &tmpComboBoxItemData );
		}

		m_pComboBox_sort->SetCursel( 0 );
		m_pComboBox_sort->Refresh();


		// 리스트 컨트롤 초기화 

		int width[] = { 125, 30, 45, 84, 85 };
        //긴 아이티 표현과 최근 접속일 의 짤린 글씨 표현을 위해 폭 조정 [2006.06.29 성웅]
//		TCHAR *text[] = { TEXT("아이디"), TEXT("레벨"), TEXT("점수"), TEXT("주민등록일"), TEXT("최근접속일") };

		m_pList_resident->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

		pText[0] = GetTxtFromMgr(1473);
		pText[1] = GetTxtFromMgr(1506);
		pText[2] = GetTxtFromMgr(1586);
		pText[3] = GetTxtFromMgr(1587);
		pText[4] = GetTxtFromMgr(1655);

		for( i = 0; i < 5; ++i ) { 

			m_pList_resident->SetColumn( i, width[ i ], pText[i] ); 
		}

		m_pList_resident->SetColumn( 5, 18, TEXT("") );

		m_pList_resident->SetBorder( true );
		m_pList_resident->SetBKColorUse( true );
		m_pList_resident->Refresh();

		SetActionTimer( 500 );
		SetTimerOnOff( true );


		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pButton_openminihome->Show(false);
		}

		// 주민 리스트 요청
		((cltCityHallManager *)m_pclStructureManager)->UpdateResidentListSwitch = FALSE;

		SI32 CharUnique = m_pclStructureManager->CharUnique;
		SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

		SI32 id = 1;
		if(id)
		{
			cltGameMsgRequest_GetResidentList	msg( siVillageUnique, 0 );

			cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTLIST, sizeof(cltGameMsgRequest_GetResidentList), (BYTE*)&msg );

			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
		}
	}
	else
	{
		DeleteDialog();
	}

}

void NResidentListDlg::NResidentListStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NResidentListDlg *pThis = (NResidentListDlg*) pControllerMgr;
	pThis->NResidentListProc( nEvent, nControlID, pControl );
}

void NResidentListDlg::NResidentListProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pclStructureManager);

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NRESIDENTLIST_LISTBOX_RESIDENT:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI16 siRow =-1, siCol =-1;
					m_pList_resident->GetSelectedIndex( &siRow, &siCol );
					if( siRow == -1 || siCol == -1 ) return;

                    TCHAR buf[ 256 ];
					TCHAR * pTmpChar = NULL;
                    pTmpChar = m_pList_resident->GetText( siRow, 0 ); // 주민 id 얻기

					if(pTmpChar == NULL ) return;

					if( _tcslen( pTmpChar ) >= 32 ) return;
					_tcsncpy( buf, pTmpChar, 32 );
					m_pEdit_name->SetText( buf );

					pCityhallManager->bResidentInfoValid = FALSE;
				}
				break;
			}
		}
		break;

	case NRESIDENTLIST_BUTTON_RESIDENTLIST:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					((cltCityHallManager *)m_pclStructureManager)->UpdateResidentListSwitch = FALSE;

					SI32 CharUnique = m_pclStructureManager->CharUnique;
					SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

					SI32 id = 1;
					if(id)
					{
						SI32 cursel = m_pComboBox_sort->GetSelectedIndex();

						if( cursel < 0 || cursel > 7 ) cursel = 0;

						if( ( cursel % 2 ) == 0 ) cursel += 1; else cursel -= 1;

						cltGameMsgRequest_GetResidentList	msg( siVillageUnique, cursel );

						cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTLIST, sizeof(cltGameMsgRequest_GetResidentList), (BYTE*)&msg );

						pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
					}
				}
				break;
			}
		}
		break;
	case NRESIDENTLIST_BUTTON_RESIDENTINFO:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buf[ 256 ];
					TCHAR* pTmpChar = NULL;

					pTmpChar = m_pEdit_name->GetText();
					if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 20 );

					buf[ 19 ] = NULL;

					cltGameMsgRequest_GetResidentInfo sendMsg( pCityhallManager->siVillageUnique, buf );

					cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTINFO, sizeof( sendMsg ), (BYTE*)&sendMsg );
					cltClient *pclclient = ( cltClient * )pclClient;
					pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NRESIDENTLIST_BUTTON_EXPULSION_RESIDENT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR* pText = GetTxtFromMgr(1656);
					TCHAR* pTitle = GetTxtFromMgr(1590);
					if( pCityhallManager->bResidentInfoValid == FALSE ) {

						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_OK, 0 );
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );

						//MessageBox(NULL, pText , pTitle, MB_OK );
						break;
					}

					pText = GetTxtFromMgr(1591);
					pTitle = GetTxtFromMgr(1590);

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 1 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

					//if( MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO) != IDYES ) break;
					
					//cltGameMsgRequest_CancelResident msg( pCityhallManager->siVillageUnique, 
					//	pCityhallManager->clClientResidentInfo.clPerson.GetPersonID() );

					//cltMsg clMsg( GAMEMSG_REQUEST_CANCELRESIDENT, sizeof(cltGameMsgRequest_CancelResident), (TCHAR*)&msg );
					//	
					//pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;	
	case NRESIDENTLIST_BUTTON_SETSCORE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					int score = 0;
					TCHAR buf[ 20 ];

					_tcsncpy( buf, m_pEdit_score->GetText(), 10 );

					score = _tstoi( buf );

					if( score <= 0 ) break;

					cltGameMsgRequest_SetResidentScore msg( pCityhallManager->siVillageUnique,
						pCityhallManager->clClientResidentInfo.clPerson.GetPersonID(), score );

					cltMsg clMsg( GAMEMSG_REQUEST_SETRESIDENTSCORE, sizeof(cltGameMsgRequest_SetResidentScore), (BYTE*)&msg );
						
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}
				break;
			}
		}
		break;

	case NRESIDENTLIST_BUTTON_OPENMINIHOME:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buf[ 256 ];
					_tcsncpy( buf, m_pEdit_name->GetText(), 32 );

					cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( buf );
					cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

					/*
					cltGameMsgRequest_Userid clUserid( buf );
					cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					*/
				}
				break;
			}
		}
		break;
	}
}

void NResidentListDlg::Action()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pclStructureManager);

	if( pCityhallManager == NULL ) return;

	if( pCityhallManager->UpdateResidentInfoSwitch ) {

		pCityhallManager->UpdateResidentInfoSwitch = FALSE;

		TCHAR buf[ 256 ];

		if( pCityhallManager->bResidentInfoValid ) {

			TCHAR* pText = GetTxtFromMgr(1588);
			StringCchPrintf( buf, 256, pText, 
				pCityhallManager->clClientResidentInfo.clPerson.GetName(),
				pCityhallManager->clClientResidentInfo.siGrade,
				pCityhallManager->clClientResidentInfo.clDate.uiYear,
				pCityhallManager->clClientResidentInfo.clDate.uiMonth,
				pCityhallManager->clClientResidentInfo.clDate.uiDay );
		} else {
			TCHAR* pText = GetTxtFromMgr(1589);
			StringCchCopy( buf, 256, pText );

		}

		m_pEdit_residentinfo->SetText( buf );
	}


	if( pCityhallManager->UpdateResidentListSwitch ) {
		// 주민 리스트 보여주기 

		pCityhallManager->UpdateResidentListSwitch = FALSE;

		SI32 i;
		TCHAR buffer[256];

		m_pList_resident->Clear();

		for( i = 0; i < MAX_RESIDENT_NUMBER; i++ )
		{
			stListBoxItemData tmpListBoxItemData;
			
			tmpListBoxItemData.Init();

			if( pCityhallManager->clResident[ i ].clPerson.GetPersonID() == 0 ) break;

			// 아이디 
			tmpListBoxItemData.Set( 0, (TCHAR *)pCityhallManager->clResident[ i ].clPerson.GetName());

			// 레벨 
			_itot( pCityhallManager->clResident[ i ].siLevel, buffer, 10 );
			tmpListBoxItemData.Set( 1, (TCHAR*) buffer );

			// 점수 
			_itot( pCityhallManager->clResident[ i ].siGrade, buffer, 10 );
			tmpListBoxItemData.Set( 2, (TCHAR*) buffer );

			// 등록일
			cltDate *pclDate = &pCityhallManager->clResident[ i ].clDate;

			TCHAR* pText = GetTxtFromMgr(1508);
			StringCchPrintf( buffer, 256, pText, pclDate->uiYear, pclDate->uiMonth, pclDate->uiDay );

			tmpListBoxItemData.Set( 3, (TCHAR*) buffer );

			// 최근접속일
			pclDate = &pCityhallManager->clResident[ i ].clLastLoginDate;

			StringCchPrintf( buffer, 256, pText, pclDate->uiYear, pclDate->uiMonth, pclDate->uiDay );

			tmpListBoxItemData.Set( 4, (TCHAR*) buffer );

			m_pList_resident->AddItem( &tmpListBoxItemData );
		}

		m_pList_resident->Refresh();
	}
}

void NResidentListDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pclStructureManager);

	switch( MsgIndex )
	{
	case 0:	// 확인 창
		{
		}
		break;
	case 1:	// 주민 퇴거 시키기
		{
			if ( RetValue )
			{
				cltGameMsgRequest_CancelResident msg( pCityhallManager->siVillageUnique, 
					pCityhallManager->clClientResidentInfo.clPerson.GetPersonID() );

				cltMsg clMsg( GAMEMSG_REQUEST_CANCELRESIDENT, sizeof(cltGameMsgRequest_CancelResident), (BYTE*)&msg );
					
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

void NResidentListDlg::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
}

void NResidentListDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
}

bool NResidentListDlg::IsShow()
{
	return m_bShow;
}
