/* ==========================================================================
	클래스 :		NAppointmentChiefDlg

	작성일 :		05/05/02
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include "./NAppointmentChiefDlg.h"

//#if defined(_NAPPOINTMENTCHIEF_DLG)

#include <tchar.h>
#include "./NCityHallDlg.h"

#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "..\..\Server\Candidate\Candidate.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"
#include "../../../Client/InterfaceMgr/Interface/ComboBox.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"


cltCityHallManager* NAppointmentChiefDlg::m_pclCityHallManager;
TCHAR				NAppointmentChiefDlg::m_szStructure[];

extern cltCommonLogic* pclClient;

NAppointmentChiefDlg::NAppointmentChiefDlg()
{
	m_bShow = false;
	m_pList_candidate= NULL;    
	m_pButton_appointment= NULL;
	m_pButton_delall= NULL;     
}

NAppointmentChiefDlg::~NAppointmentChiefDlg()
{
	if(m_pList_candidate) delete m_pList_candidate;    
	if(m_pButton_appointment) delete m_pButton_appointment;
	if(m_pButton_delall) delete m_pButton_delall;     

	SetTimerOnOff( false );
}

void NAppointmentChiefDlg::Create( )
{

	TCHAR buf[ 256 ];

	if(!IsCreate())
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NAppointmentDlg/DLG_NAppointment.ddf"));

		file.CreatePopupDialog( this, NAPPOINTMENTCHIEF_DLG, TEXT("dialog_appointment"), NAppointmentChiefStaticProc);

		TCHAR* pTitle = GetTxtFromMgr(3683);
		SetTitle( pTitle);
		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_appointment") );

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

		m_pclCityHallManager = pclClient->pclCityHallManager;

		// 선택된 시설의 이름을 확인한다. 
		if( pclclient == NULL || 
			pclclient->m_pDialog[ NCITYHALL_DLG ] == NULL || 
			((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ] == NULL) return;

		CComboBox *pTmpCombo = NULL;
		//pTmpCombo = ((CComboBox*)((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ]->m_pChildControl[ 15 ])  ;
		pTmpCombo = ((CComboBox*)((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ]->m_pControlMap[ NCityhallChiefDlg::COMBO_STRUCT ]);

		if(pTmpCombo == NULL ) return;

		SI32 index = pTmpCombo->GetSelectedIndex();
		if(index < 0 ) return;
		TCHAR* pTmpChar = NULL;
		pTmpChar = pTmpCombo->GetText( index );
		if(pTmpChar == NULL ) return;
		_tcsncpy( m_szStructure, pTmpChar, sizeof(m_szStructure));
		//strcpy( m_szStructure, szStructure );

		TCHAR* pText = GetTxtFromMgr(1503);
		StringCchPrintf( buf, 256, pText, m_szStructure );

		this->SetTitle( buf );

		m_pList_candidate = new CList( this );    
		m_pButton_appointment = new CButton(this );
		m_pButton_delall = new CButton( this);     

		file.CreateControl( 	m_pList_candidate	, 	NAPPOINTMENTCHIEF_LISTBOX_CANDIDATE	, TEXT("listbox_candidate") )	;
		file.CreateControl( 	m_pButton_appointment	, 	NAPPOINTMENTCHIEF_BUTTON_APPOINTMENT	, TEXT("button_appointment") )	;
		file.CreateControl( 	m_pButton_delall	, 	NAPPOINTMENTCHIEF_BUTTON_DELALL	, TEXT("button_delall") )	;

		pText = GetTxtFromMgr(3683);
		m_pButton_appointment->SetText(pText, DT_CENTER | DT_VCENTER | DT_WORDBREAK);
		pText = GetTxtFromMgr(3684);
		m_pButton_delall->SetText(pText, DT_CENTER | DT_VCENTER | DT_WORDBREAK);



		// 리스트 컨트롤 초기화 

		int i;

		int width[] = {175 };
	//	TCHAR *text[] = { "아이디" };

		pText = GetTxtFromMgr(1473);
		m_pList_candidate->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		m_pList_candidate->SetBorder( true );
		m_pList_candidate->SetBKColorUse( true );

		for( i = 0; i < 1; ++i ) {
			m_pList_candidate->SetColumn( 0, width[i], pText );
		}
		m_pList_candidate->SetColumn( 1, 22, TEXT("") );

		m_pList_candidate->Refresh();

		// 

		SI32 CharUnique = m_pclCityHallManager->CharUnique;
		SI32 siVillageUnique = m_pclCityHallManager->siVillageUnique;

		SI32 id = 1;

		if(id)
		{

			SI32 ranktype = m_pclCityHallManager->GetRankTypeFromStructureName( m_szStructure );
			if(ranktype)
			{
				cltSimpleRank clrank(ranktype, siVillageUnique);
				
				cltMsg clMsg(GAMEMSG_REQUEST_CANDIDATELIST,
					sizeof(cltSimpleRank), (BYTE*)&clrank);
				
				pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}

		SetActionTimer( 500 );
		SetTimerOnOff( true );

	}
	else
	{
		DeleteDialog();
	}
}


void NAppointmentChiefDlg::NAppointmentChiefStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NAppointmentChiefDlg *pThis = (NAppointmentChiefDlg*) pControllerMgr;
	pThis->NAppointmentChiefProc( nEvent, nControlID, pControl );
}

void NAppointmentChiefDlg::NAppointmentChiefProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NAPPOINTMENTCHIEF_BUTTON_APPOINTMENT:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 CharUnique = m_pclCityHallManager->CharUnique;
					SI32 siVillageUnique = m_pclCityHallManager->siVillageUnique;

					// 서버에 행수취임 요청을 한다.
					SI32 id = 1;
					if(id)
					{
						// 선택된 후보자의 PersonID를 구한다. 
						//SI32 index = SendDlgItemMessage( hDlg, IDC_LIST_CANDIDATE, LVM_GETNEXTITEM, (WPARAM)-1, (LPARAM)( LVNI_ALL | LVNI_SELECTED ) );
						SI32 index = -1;

						GET_LISTBOX_ITEMDATA( m_pList_candidate, index );

						if( index > -1 ) {

							// 선택된 시설의 책임자 신분코드를 얻는다.
							SI32 ranktype = m_pclCityHallManager->GetRankTypeFromStructureName( m_szStructure );

							if( ranktype ) {


								//LVITEM lvi;

								//lvi.mask = LVIF_PARAM;
								//lvi.iItem = index;
								//lvi.iSubItem = 0;

								//SendDlgItemMessage( hDlg, IDC_LIST_CANDIDATE, LVM_GETITEM, 0, (LPARAM)&lvi );
								//
								//SI32 personid = (SI32)lvi.lParam;
								SI32 personid = index;

								cltSimplePerson clperson(personid, TEXT(""));
								cltSimpleRank clsimplerank( ranktype, siVillageUnique);

								cltRank clrank(&clsimplerank, &clperson);
								
								if(personid > 0)
								{
									cltMsg clMsg(GAMEMSG_REQUEST_BECHIEF, sizeof(cltRank), (BYTE*)&clrank);
									
									pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

									DeleteDialog();
								}

							}

						}

					}
				}
				break;
			}
		}
		break;
	case NAPPOINTMENTCHIEF_BUTTON_DELALL:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SI32 CharUnique = m_pclCityHallManager->CharUnique;
					SI32 siVillageUnique = m_pclCityHallManager->siVillageUnique;

					SI32 id = 1;

					// 선택된 시설의 이름을 확인한다. 
					TCHAR szSelText[128];

					if( pclclient == NULL || 
						pclclient->m_pDialog[ NCITYHALL_DLG ] == NULL || 
						((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ] == NULL) return;

					CComboBox *pTmpCombo = NULL;
					//pTmpCombo = ((CComboBox*)((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ]->m_pChildControl[ 15 ])  ;
					pTmpCombo = ((CComboBox*)((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ]->m_pControlMap[ NCityhallChiefDlg::COMBO_STRUCT ]);

					if(pTmpCombo == NULL ) return;

					//HWND hwnd = GetDlgItem( m_pclCityHallManager->TabDlg[ MAX_CITYHALL_TAB_NUMBER - MAX_CITYHALL_CHIEF_TAB_NUMBER ], IDC_NCityhallChiefDlg::COMBO_STRUCT );

					//SI32 index = SendMessage(hwnd, CB_GETCURSEL, 0, 0);
					//SendMessage(hwnd, CB_GETLBTEXT, index, (LPARAM)szSelText);
					SI32 index = pTmpCombo->GetSelectedIndex();
					if(index < 0 ) return;
					TCHAR* pTmpChar = NULL;
					pTmpChar = pTmpCombo->GetText( index );
					if(pTmpChar == NULL ) return;
					_tcsncpy( szSelText, pTmpChar, sizeof(szSelText ));
                    					
					// 선택된 시설의 책임자 신분코드를 얻는다.
					SI32 ranktype = m_pclCityHallManager->GetRankTypeFromStructureName(szSelText);

					if( ranktype ) {

						if(id)
						{
							// 삭제할 신분을 설정한다. 
							cltSimpleRank clrank( ranktype, siVillageUnique);
							cltMsg clMsg(GAMEMSG_REQUEST_DELCANDIDATE, sizeof(clrank), (BYTE*)&clrank);
							
							pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

							DeleteDialog();
						}
					
					}
				}
				break;
			}
		}
		break;
	}
}


void NAppointmentChiefDlg::ShowChiefCandidate()
{
	cltClient *pclclient = (cltClient*)pclClient;

	SI32 i;
	TCHAR buffer[256]=TEXT("");
	TCHAR szSelText[128];

	if( pclclient == NULL || 
		pclclient->m_pDialog[ NCITYHALL_DLG ] == NULL || 
		((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ] == NULL) return;

	CComboBox *pTmpCombo = NULL;
	//pTmpCombo = ((CComboBox*)((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ]->m_pChildControl[ 15 ] ) ;
	pTmpCombo = ((CComboBox*)((NCityhallDlg*)pclclient->m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[ 0 ]->m_pControlMap[ NCityhallChiefDlg::COMBO_STRUCT ]);

	if(pTmpCombo == NULL ) return;

	SI32 index = pTmpCombo->GetSelectedIndex();
	if(index < 0 ) return;
	TCHAR* pTmpChar = NULL;
	pTmpChar = pTmpCombo->GetText( index );
	if(pTmpChar == NULL ) return;
	_tcsncpy( szSelText, pTmpChar, sizeof(szSelText ));
	
	//HWND hwnd = GetDlgItem(  m_pclCityHallManager->TabDlg[ MAX_CITYHALL_TAB_NUMBER - MAX_CITYHALL_CHIEF_TAB_NUMBER ], IDC_NCityhallChiefDlg::COMBO_STRUCT );

	//SI32 index = SendMessage( hwnd, CB_GETCURSEL, 0, 0);
	//SendMessage(hwnd, CB_GETLBTEXT, index, (LPARAM)szSelText);

	SI32 ranktype = m_pclCityHallManager->GetRankTypeFromStructureName(szSelText);

	// 행수 후보자를 보여주는 리스트 박스를 지운다.
	//SendDlgItemMessage( m_hWnd, IDC_LIST_CANDIDATE, LVM_DELETEALLITEMS, 0, 0 );
	m_pList_candidate->Clear();

	for( i = 0; i < MAX_CANDIDATE_NUMBER; i++ )
	{
		cltSimplePerson* pclPerson = 
			m_pclCityHallManager->pclClientCandidateManager->GetCandidateInfoFromRef(ranktype, i);

		if(pclPerson->GetPersonID())
		{
			stListBoxItemData tmpListBoxItemData;
			tmpListBoxItemData.Init();
			tmpListBoxItemData.siParam[0] = pclPerson->GetPersonID();
			tmpListBoxItemData.Set( 0, (TCHAR *)pclPerson->GetName());
			m_pList_candidate->AddItem( &tmpListBoxItemData );

			index++;
		}
	}
	m_pList_candidate->Refresh();
}

void NAppointmentChiefDlg::Action()
{
	if( m_pclCityHallManager->UpdateChiefCandidateSwitch == TRUE)
	{
		m_pclCityHallManager->UpdateChiefCandidateSwitch = FALSE;

		ShowChiefCandidate();
	}
}

void NAppointmentChiefDlg::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
	SetActionTimer( 500 );
	SetTimerOnOff( true );
}

void NAppointmentChiefDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
	SetTimerOnOff( false );
}

bool NAppointmentChiefDlg::IsShow()
{
	return m_bShow;
}

//#endif