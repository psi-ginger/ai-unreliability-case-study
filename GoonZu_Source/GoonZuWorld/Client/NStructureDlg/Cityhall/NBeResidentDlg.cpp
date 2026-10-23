/* ==========================================================================
	클래스 :		NBeResidentDlg

	작성일 :		05/04/30
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/
#include "./NBeResidentDlg.h" 

#include "../../resource.h"

#include <tchar.h>
#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "MsgType-Structure.h"
//#include "../StructureClass/StructureClass.h"

#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"
#include "../../../Client/InterfaceMgr/Interface/Button.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

cltStructureMngBasic*	NBeResidentDlg::m_pclStructureManager;

extern cltCommonLogic* pclClient;

NBeResidentDlg::NBeResidentDlg()
{
	m_bShow = false;

	m_pButton_resident	 = NULL; 
	m_pButton_delresident	 = NULL; 
	m_pEdit_name	 = NULL; 
	m_pList_resident	 = NULL; 

}

NBeResidentDlg::~NBeResidentDlg()
{
	if( 	m_pButton_resident	 )  	delete 	m_pButton_resident	;
	if( 	m_pButton_delresident	 )  	delete 	m_pButton_delresident	;
	if( 	m_pEdit_name	 )  	delete 	m_pEdit_name	;
	if( 	m_pList_resident	 )  	delete 	m_pList_resident	;
	
	SetTimerOnOff( false );

}

void NBeResidentDlg::Create( )
{
	m_pclStructureManager = pclClient->pclCityHallManager;

	if( !IsCreate() )
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NBeResidentDlg/DLG_NBeResident.ddf"));

		file.CreatePopupDialog( this, NBERESIDENT_DLG, TEXT("dialog_beresident"), NBeResidentStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_beresident") );

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

		m_pButton_resident	 = new 	CButton	( this )	;
		m_pButton_delresident	 = new 	CButton	( this )	;
		m_pEdit_name	 = new 	CEdit	( this )	;
		m_pList_resident	 = new 	CList	( this )	;

		file.CreateControl( 	m_pButton_resident	, 	NBERESIDENT_BUTTON_RESIDENT	, 	TEXT("button_resident")	 )	;
		file.CreateControl( 	m_pButton_delresident	, 	NBERESIDENT_BUTTON_DELRESIDENT	, 	TEXT("button_delresident")	 )	;
		file.CreateControl( 	m_pEdit_name	, 	NBERESIDENT_EDITBOX_NAME	, 	TEXT("editbox_name")	 )	;
		file.CreateControl( 	m_pList_resident	, 	NBERESIDENT_LISTBOX_RESIDENT	, 	TEXT("listbox_resident")	 )	;

		TCHAR* pTitle = GetTxtFromMgr(3703);
		SetTitle(pTitle);

		TCHAR* pText1 = GetTxtFromMgr(3704);
		m_pButton_resident->SetText(pText1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pText1 = GetTxtFromMgr(3705);
		m_pButton_delresident->SetText(pText1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		
		// 리스트 컨트롤 초기화 
		m_pList_resident->Clear();

		int i;

		int width[] = { 92, 60, 100 };
	//	TCHAR *text[] = { TEXT("아이디"), TEXT("레벨"), TEXT("신청일") };
		
		TCHAR* pText[3];

		pText[0] = GetTxtFromMgr(1473);
		pText[1] = GetTxtFromMgr(1506);
		pText[2] = GetTxtFromMgr(1507);

		m_pList_resident->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		m_pList_resident->SetBorder( true );
		m_pList_resident->SetBKColorUse( true );
		

		for( i = 0; i < 3; ++i ) {

			m_pList_resident->SetColumn( i, width[i], pText[i] );
		}
		m_pList_resident->SetColumn( 3, 18, TEXT("") );

		m_pList_resident->Refresh();

		// 주민 등록 신청자 리스트 요청

		((cltCityHallManager *)m_pclStructureManager)->UpdateResidentApplyerSwitch = FALSE;

		SI32 CharUnique = m_pclStructureManager->CharUnique;
		SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

		SI32 id = 1;
		if(id)
		{
			cltGameMsgRequest_GetResidentApplyerList	msg( siVillageUnique );

			cltMsg clMsg( GAMEMSG_REQUEST_RESIDENTAPPLYERLIST, sizeof(cltGameMsgRequest_GetResidentApplyerList), (BYTE*)&msg );
			
			pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
		}

		SetActionTimer( 500 );
		SetTimerOnOff( true );
}
	else
	{
		DeleteDialog();
	}
}

void NBeResidentDlg::NBeResidentStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
    NBeResidentDlg *pThis = (NBeResidentDlg*)pControllerMgr;
	pThis->NBeResidentProc( nEvent, nControlID, pControl );
}

void NBeResidentDlg::NBeResidentProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NBERESIDENT_LISTBOX_RESIDENT:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					TCHAR buf[ 256 ];
					TCHAR *pTmpChar = NULL;

					SI16 siRow = -1, siCol = -1;
					m_pList_resident->GetSelectedIndex( &siRow, &siCol );
					if( siRow == -1 || siCol == -1 ) return;
					pTmpChar = m_pList_resident->GetText( siRow, 0 );
					if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 32 );
					m_pEdit_name->SetText( buf );
				}
				break;
			}
		}
		break;

	case NBERESIDENT_BUTTON_RESIDENT	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 CharUnique = m_pclStructureManager->CharUnique;
					SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

					SI32 id = 1;
					SI32 personid = 0;

					TCHAR buf[ 256 ];
					TCHAR *pTmpChar = NULL;
					pTmpChar = m_pEdit_name->GetText();
					//if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 32 );

					if(id)
					{

						cltResidentApplyer *pclApplyer = &(((cltCityHallManager *)m_pclStructureManager)->clResidentApplyer[0]);

						for( int i = 0; i < MAX_RESIDENT_APPLYER; ++i ) {

							if( _tcscmp( pclApplyer[ i ].clApplyer.GetName(), buf ) == 0 ) {

								personid = pclApplyer[ i ].clApplyer.GetPersonID();
								break;
							}
						}

						if( personid ) {
						
							cltGameMsgRequest_BeResident msg( siVillageUnique, personid );

							cltMsg clMsg( GAMEMSG_REQUEST_BERESIDENT, sizeof(cltGameMsgRequest_BeResident), (BYTE*)&msg );
							
							pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
						
						}
					}
				}
				break;
			}
		}
		break;
	case NBERESIDENT_BUTTON_DELRESIDENT	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 CharUnique = m_pclStructureManager->CharUnique;
					SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

					SI32 id = 1;
					SI32 personid = 0;
					TCHAR buf[ 256 ];
					TCHAR *pTmpChar = NULL;
					pTmpChar = m_pEdit_name->GetText();
					//if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 32 );

					if(id)
					{

						cltResidentApplyer *pclApplyer = &(((cltCityHallManager *)m_pclStructureManager)->clResidentApplyer[0]);

						for( int i = 0; i < MAX_RESIDENT_APPLYER; ++i ) {

							if( _tcscmp( pclApplyer[ i ].clApplyer.GetName(), buf ) == 0 ) {

								personid = pclApplyer[ i ].clApplyer.GetPersonID();
								break;
							}
						}

						if( personid ) {
						
							cltGameMsgRequest_CancelResident msg( siVillageUnique, personid );

							cltMsg clMsg( GAMEMSG_REQUEST_CANCELRESIDENT, sizeof(cltGameMsgRequest_CancelResident), (BYTE*)&msg );
							
							pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
						
						}
					}
				}
				break;
			}
		}
		break;
	}
}

void NBeResidentDlg::Action()
{
	if( ((cltCityHallManager *)m_pclStructureManager)->UpdateResidentApplyerSwitch ) {
		// 주민 등록 신청자 리스트 보여주기 

		((cltCityHallManager *)m_pclStructureManager)->UpdateResidentApplyerSwitch = FALSE;

		SI32 i;
		TCHAR buffer[256];

		m_pList_resident->Clear();

		cltResidentApplyer *pclApplyer = &(((cltCityHallManager *)m_pclStructureManager)->clResidentApplyer[0]);

		for( i = 0; i < MAX_RESIDENT_APPLYER; i++ )
		{
			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();

			if( pclApplyer[ i ].clApplyer.GetPersonID() == 0 ) break;


			// 아이디 
			tmpListBoxItemData.Set( 0, (TCHAR *)pclApplyer[ i ].clApplyer.GetName());

			// 레벨
			_itot( pclApplyer[ i ].siLevel, buffer, 10 );

			tmpListBoxItemData.Set( 1, (TCHAR *)buffer);

			// 신청일 
			cltDate *pclDate = &pclApplyer[ i ].clDate;
			
			TCHAR* pText = GetTxtFromMgr(1508);
			StringCchPrintf( buffer, 256, pText, pclDate->uiYear, pclDate->uiMonth, pclDate->uiDay );

			tmpListBoxItemData.Set( 2, (TCHAR *)buffer);

			m_pList_resident->AddItem( &tmpListBoxItemData );
		}

		m_pList_resident->Refresh();
	}
}

void NBeResidentDlg::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
}

void NBeResidentDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
}

bool NBeResidentDlg::IsShow()
{
	return m_bShow;
}

//_LEON_MADE_NLISTBOX_FINDDELETE
void NBeResidentDlg::UpdateResidentApplyerList(SI32 personid)
{
	SI32 i = 0;
	SI32 count = 0;
	//TCHAR buffer[256];
	TCHAR DelName[256];

	if( m_pList_resident == NULL || m_pEdit_name == NULL ) return;

	stListBoxItemData	*tmpListBoxItemDatas = new stListBoxItemData[ MAX_LISTBOX_ITEM_NUM ];
	stListBoxItemData	*pTmpListBoxItemData = NULL;

	cltResidentApplyer *pclApplyer = &(((cltCityHallManager *)m_pclStructureManager)->clResidentApplyer[0]);

	for( i = 0; i < MAX_RESIDENT_APPLYER; i++ )
	{
		if( personid == pclApplyer[ i ].clApplyer.GetPersonID() ) 
		{
			StringCchCopy( DelName, 256, pclApplyer[ i ].clApplyer.GetName() );
			break;
		}
	}

	for( i = 0; i < MAX_LISTBOX_ITEM_NUM; i++ )
	{
		pTmpListBoxItemData = m_pList_resident->GetItemData( i );

		if( pTmpListBoxItemData == NULL ) continue;

		if( _tcscmp( pTmpListBoxItemData->strText[0], DelName ) == 0 ) 
		{
			continue;
		}
		
		memcpy(tmpListBoxItemDatas + count, pTmpListBoxItemData, sizeof(stListBoxItemData) );
		count++;
	}

	m_pList_resident->Clear();
	m_pEdit_name->Clear();

	for( i = 0; i < count; i++ )
	{
		m_pList_resident->AddItem( tmpListBoxItemDatas + i );
	}

	delete [] tmpListBoxItemDatas;

	m_pList_resident->Refresh();
}