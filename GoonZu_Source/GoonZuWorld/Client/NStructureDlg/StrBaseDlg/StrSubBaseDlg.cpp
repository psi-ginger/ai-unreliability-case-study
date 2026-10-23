
#include "StrSubBaseDlg.h"

#include <tchar.h>

#include "Char\CharManager\CharManager.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "client.h"


extern cltCommonLogic* pclClient;

NStrSubBaseDlg::NStrSubBaseDlg()
{
	m_pExplainEdit = NULL;
	m_pPortraitImage = NULL;
	m_pCloseBtn = NULL;
	m_pChiefToggleBtn = NULL;
	

	for( int i = 0; i < MAX_CHILD_DLG; ++i ) 
	{
		m_pUserTabButton[ i ] = NULL;
		m_pUserChildDlg[ i ] = NULL;

		m_pChiefTabButton[ i ] = NULL;
		m_pChiefChildDlg[ i ] = NULL;
		
	}

	m_pStrManager = NULL;

	m_dwActionCount = 0;

//	m_uiUnitSlot = MAX_GUILDUNIT_NUMBER;
}


NStrSubBaseDlg::~NStrSubBaseDlg()
{
	SetTimerOnOff( false );

	if( m_pStrManager ) {
		m_pStrManager->pStrBaseDlg = NULL;
		m_pStrManager = NULL;
	}

	NDelete( m_pExplainEdit );
	NDelete( m_pPortraitImage );
	NDelete( m_pCloseBtn );
	NDelete( m_pChiefToggleBtn );
	
	


	for( int i = 0; i < MAX_CHILD_DLG; ++i )
	{
		NDelete( m_pUserTabButton[ i ] );
		NDelete( m_pUserChildDlg[ i ] );

		NDelete( m_pChiefTabButton[ i ] );
		NDelete( m_pChiefChildDlg[ i ] );
		
		
	}

}

void NStrSubBaseDlg::CreateDlg( SI16 id, cltStructureMngBasic *pStrManager, int portrait )
{
	m_portraitindex = portrait;
	m_bChiefMode = FALSE;
	SetTabInfo( 1, 1 );

	if ( !IsCreate() )
	{

		CInterfaceFile file;
		
		if( file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_NStrSubBaseDlg.ddf")) == false ) return;

		file.CreatePopupDialog( this, id, TEXT("dialog_strbase"), StaticNDlgProc );

		int x = ( ((cltClient *)pclClient)->siClientScreenXsize - GetWidth() ) - 80;
		int y = ( ((cltClient *)pclClient)->siClientScreenYsize - GetHeight() ) / 2;
		MovePos( x, y );
	
		m_pExplainEdit = new CEdit( this );
		m_pPortraitImage = new CImageStatic( this );
		m_pCloseBtn = new CButton( this );
		m_pChiefToggleBtn = new CButton( this );
	


		file.CreateControl( m_pExplainEdit, NSTRSUBBASE_EDIT_EXPLAIN, TEXT("editbox_explain") );
		file.CreateControl( m_pPortraitImage, NSTRSUBBASE_IMAGE_PORTRAIT, TEXT("imagestatic_portrait") );
		file.CreateControl( m_pCloseBtn, NSTRSUBBASE_BUTTON_CLOSE, TEXT("button_close") );
		file.CreateControl( m_pChiefToggleBtn, NSTRSUBBASE_BUTTON_CHIEFTOGGLE, TEXT("button_chief") );
		

		// Set ChildDialog Rect 
		SetRect( &m_ChildRect, 7, 126, GetWidth() - 8, 428 );

		m_pStrManager = pStrManager;

		if(m_pStrManager != NULL )
		{
/*			cltGuildStructManager* pGuildStrMgr = (cltGuildStructManager*)m_pStrManager;
		
			SI32 villageunique = m_pStrManager->pclCM->CR[1]->GetCurrentVillageUnique();
			if(villageunique <=0 || villageunique >=MAX_VILLAGE_NUMBER ) return;

            m_siVillageUnique = villageunique;
			m_uiUnitSlot = pGuildStrMgr->siSelectedGuildStrUnitNumber;*/
		}
			

		GreetingMsg();

		//if( m_pStrManager ) {
		//	m_pStrManager->pStrBaseDlg = this;
		//	m_pStrManager->Do_WM_INITDIALOG( id );
		//}

	} else {

		DeleteDialog();
	}
}

void NStrSubBaseDlg::Action()
{
	++m_dwActionCount;

	int i;

	//if( m_dwActionCount % 3 == 0 ) {
	//	if( m_pStrManager )m_pStrManager->ServerRtnValOperate();
	//}

	if( m_bChiefMode ) {

		for( i = 0; i < MAX_CHILD_DLG; ++i ) {
			if( m_pChiefChildDlg[ i ] && m_pChiefChildDlg[ i ]->IsShow() ) 
				m_pChiefChildDlg[ i ]->Action();
		}
	} else {

		for( i = 0; i < MAX_CHILD_DLG; ++i ) {
			if( m_pUserChildDlg[ i ] && m_pUserChildDlg[ i ]->IsShow() ) 
				m_pUserChildDlg[ i ]->Action();
		}
	}

}

void NStrSubBaseDlg::SetTabInfo( int row, int currentrow )
{
	m_row = row;
	m_currentrow = currentrow;

	if( row == 1 ) {
		
		m_tabHeight = 24;
		
		m_tabX = 5;
		m_tabY = 100;

	} else {

		m_tabHeight = 16;

		m_tabX = 5;
		m_tabY = 96 + ( m_tabHeight * ( currentrow - 1 ) );
	}
}


void NStrSubBaseDlg::AddUserTabDialog( int index, TCHAR *text, int width, NStrSubTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID )
{
	TCHAR *btnfilename[] = {
		TEXT("ninterface/button/button_4x24_00_103.spr"),
		TEXT("ninterface/button/button_4x16_00_103.spr") 
	};	

	width = ( width * 4 + 3 ) / 4;

	if( m_pUserTabButton[ index ] ||
		m_pUserChildDlg[ index ] ) return;

	m_pUserTabButton[ index ] = new CButton( this );

	m_pUserTabButton[ index ]->Create(
		NSTRSUBBASE_USER_TAB_BUTTON + index, 
		text, 
		btnfilename[ m_row - 1 ],
		0,
		true,
		3,
		m_tabX,
		m_tabY,
		width,
		m_tabHeight,
		width,
		m_tabHeight );

	m_pUserTabButton[ index ]->Show( false );

	m_tabX += width;

	m_pUserChildDlg[ index ] = pChildDlg;

	// 차일드 다이얼로그 생성
	if( pChildDlg ) pChildDlg->CreateTabDlg( index, this, filename, szID, FALSE );
}

void NStrSubBaseDlg::AddChiefTabDialog( int index, TCHAR *text, int width, NStrSubTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID )
{
	TCHAR *btnfilename[] = {
		TEXT("ninterface/button/button_4x24_00_103.spr"),
		TEXT("ninterface/button/button_4x16_00_103.spr") 
	};	

	width = ( width * 4 + 3 ) / 4;

	if( m_pChiefTabButton[ index ] ||
		m_pChiefChildDlg[ index ] ) return;

	m_pChiefTabButton[ index ] = new CButton( this );

	m_pChiefTabButton[ index ]->Create(
		NSTRSUBBASE_CHIEF_TAB_BUTTON + index, 
		text, 
		btnfilename[ m_row - 1 ],
		0,
		true,
		3,
		m_tabX,
		m_tabY,
		width,
		m_tabHeight,
		width,
		m_tabHeight );


	m_pChiefTabButton[ index ]->Show( false );

#ifndef _NEW_INTERFACE_
	m_pChiefTabButton[ index ]->SetAniType( true );
#else
	//m_pChiefTabButton[ index ]->SetAniType( true );
#endif

	m_tabX += width;

	m_pChiefChildDlg[ index ] = pChildDlg;

	// 차일드 다이얼로그 생성
	if( pChildDlg ) pChildDlg->CreateTabDlg( index, this, filename, szID, TRUE );
}

void NStrSubBaseDlg::SetChiefMode( BOOL bChief )
{
	HideAllTab();
	ShowAllTabButton( false );

	m_bChiefMode = bChief;

	ShowAllTabButton( true );
	SetActiveTab( 0 );

	if( m_bChiefMode ) {
		TCHAR* pText = GetTxtFromMgr(3900);
		m_pChiefToggleBtn->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	} else {
		TCHAR* pText = GetTxtFromMgr(3901);
		m_pChiefToggleBtn->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}
}

void NStrSubBaseDlg::SetActiveTab( int index )
{
	HideAllTab();

	if( m_bChiefMode ) {

		if( m_pChiefChildDlg[ index ] ) {
			m_pChiefChildDlg[ index ]->Show();

			m_pChiefTabButton[ index ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pChiefTabButton[ index ]->SetFontWeight( FW_BOLD );
			m_pChiefTabButton[ index ]->Refresh();
		}

	} else {

		if( m_pUserChildDlg[ index ] ) {
			m_pUserChildDlg[ index ]->Show();

			m_pUserTabButton[ index ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pUserTabButton[ index ]->SetFontWeight( FW_BOLD );
			m_pUserTabButton[ index ]->Refresh();
		}
	}
}

void NStrSubBaseDlg::HideAllTab()
{
	for( int i = 0; i < MAX_CHILD_DLG; ++i ) {

		if( m_bChiefMode ) {

			if( m_pChiefChildDlg[ i ] ) {
				m_pChiefChildDlg[ i ]->Hide();

				m_pChiefTabButton[ i ]->SetFontColor( RGB( 20, 20, 20 ) );
				m_pChiefTabButton[ i ]->SetFontWeight( FW_NORMAL );
				m_pChiefTabButton[ i ]->Refresh();
			}

		} else {

			if( m_pUserChildDlg[ i ] ) {
				m_pUserChildDlg[ i ]->Hide();

				m_pUserTabButton[ i ]->SetFontColor( RGB( 20, 20, 20 ) );
				m_pUserTabButton[ i ]->SetFontWeight( FW_NORMAL );
				m_pUserTabButton[ i ]->Refresh();
			}
		}
	}
}

void NStrSubBaseDlg::ShowAllTabButton( bool bShow )
{
	for( int i = 0; i < MAX_CHILD_DLG; ++i ) {

		if( m_bChiefMode ) {

			if( m_pChiefTabButton[ i ] ) m_pChiefTabButton[ i ]->Show( bShow );

		} else {

			if( m_pUserTabButton[ i ] ) m_pUserTabButton[ i ]->Show( bShow );
		}
	}
}

void NStrSubBaseDlg::SetMainExplain( TCHAR *text )
{
	if( m_pExplainEdit ) m_pExplainEdit->SetText( text );

	//m_pStrManager->dwLastExplainTick = GetTickCount();
}


void NStrSubBaseDlg::InitDlg()
{
	SetChiefMode( FALSE );
}

void NStrSubBaseDlg::NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	for( int i = 0; i < MAX_CHILD_DLG; ++i ) {

		if( m_bChiefMode ) {

			if( m_pChiefTabButton[ i ] == NULL ) continue;			
			if( nControlID == m_pChiefTabButton[ i ]->GetControllerID() &&
				nEvent == EVENT_BUTTON_CLICKED ) {

					SetActiveTab( i );
				}

		} else {

			if( m_pUserTabButton[ i ] == NULL ) continue;
			if( nControlID == m_pUserTabButton[ i ]->GetControllerID() &&
				nEvent == EVENT_BUTTON_CLICKED ) {

					SetActiveTab( i );
				}
		}
	}

	switch( nControlID )
	{

	case COMMON_DIALOG_CLOSEBUTTON:
	case NSTRSUBBASE_BUTTON_CLOSE:
		{
			switch( nEvent ) {
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				return;
			}			
		}
		break;

	case NSTRSUBBASE_BUTTON_CHIEFTOGGLE:
		{
			switch( nEvent ) {
			case EVENT_BUTTON_CLICKED:
				{
/*					cltClient* pclclient = (cltClient*)pclClient;

					cltGuildRankManager* pGuildRank = pclclient->pclGuildRankManager;
					//cltGuildStructManager* pGuildStrMgr = (cltGuildStructManager*)m_pStrManager;

					//SI32 villageunique = m_pStrManager->pclCM->CR[1]->GetCurrentVillageUnique();
					if(m_siVillageUnique <=0 || m_siVillageUnique >=MAX_VILLAGE_NUMBER ) return;
					if(m_uiUnitSlot < 0 || m_uiUnitSlot >= MAX_GUILDUNIT_NUMBER ) return;

					SI32 chiefid = pGuildRank->clRank[m_siVillageUnique][ m_uiUnitSlot ][ cltGuildRankManager::GUILDRANK_CHIEF ].clPerson.GetPersonID();

					// 자신이 단위 길드 행수로 이미 등록되어 있어야  한다. 
					if( chiefid != m_pStrManager->pclCM->CR[1]->pclCI->GetPersonID() )
					{
						TCHAR* pText = GetTxtFromMgr(5282);
						SetMainExplain(pText);
						return;
					}

					SetChiefMode( !(BOOL)m_bChiefMode );

					if( m_bChiefMode == TRUE ) // user => chief로 갈때
					{
						cltGuildStructManager *pGuilductManager = pclClient->pclGuildStructManager;
						//------------------------------------------
						// 서버로 단위 길드원 희망자 리스트를 요청한다. 
						//------------------------------------------
						pGuilductManager->RequestGuildUnitInfo( 1, m_siVillageUnique, m_uiUnitSlot );
					}*/

				}
				break;
			}
		}
		break;
	}
}


void CALLBACK NStrSubBaseDlg::StaticNDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	((NStrSubBaseDlg*)pControllerMgr)->NDlgProc( nEvent, nControlID, pControl );	
}

void NStrSubBaseDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	int x = ScreenRX + GetX();
	int y = ScreenRY + GetY();

	g_graphics.Box16C( 
		x + m_ChildRect.left, y + m_ChildRect.top,
		x + m_ChildRect.right, y + m_ChildRect.bottom, 
		g_graphics.MakeRGB16( 220, 130, 130 ) 
		);

	TSpr *pspr = pclClient->GetGlobalSpr( GIMG_VILLAGEUNITNPC );

	if( m_portraitindex < 0 || m_portraitindex >= 6 ) 
		m_portraitindex = 0;

	GP.PutSpr(pspr, GetX() + ScreenRX + 12, 
		           GetY() + ScreenRY + 25, m_portraitindex );
}

//----------------------------------------------------------------------------
NStrSubTabDlg::NStrSubTabDlg()
{
	for( int i = 0; i < MAX_CHILD_CONTROL; ++i ) {
		m_pChildControl[ i ] = NULL;
		m_controlid[ i ][ 0 ] = NULL;
		
		m_pControlMap[ i ] = NULL;
		m_nControlID[ i ] = 0;		
	}

	m_controlnum = 0;

	m_pBaseDlg = NULL;

	m_pStrManager = NULL;
	pclCM = NULL;
}


NStrSubTabDlg::~NStrSubTabDlg()
{
	for( int i = 0; i < MAX_CHILD_CONTROL; ++i ) {
		NDelete( m_pChildControl[ i ] );
	}
}

// 탭 다이얼로그를 생성한다
void NStrSubTabDlg::CreateTabDlg( int index, NStrSubBaseDlg *pParent, TCHAR *filename, TCHAR *szID, BOOL bChief )
{

	CInterfaceFile file;

	if( file.LoadFile( filename ) == false ) return;

	if( bChief ) {
		file.CreateChildDialog( this, NSTRSUBBASE_CHIEF_TAB_DIALOG + index, szID, StaticNDlgTabProc, pParent );
	} else {
		file.CreateChildDialog( this, NSTRSUBBASE_USER_TAB_DIALOG + index, szID, StaticNDlgTabProc, pParent );
	}

	MovePos( 0, pParent->m_ChildRect.top );

	for( int i = 0; i < file.m_controlNum; ++i ) 
	{
		file.CreateControl( this, &m_pChildControl[ i ], NSTRSUBBASE_CHILD_CONTROL + i, NULL, i );
		
		memcpy( m_controlid[ i ], file.m_control[ i ].id, 32 );
		m_controlid[ i ][ 31 ] = NULL;

	}
	m_controlnum = file.m_controlNum;

	m_pBaseDlg = pParent;

	if( m_pBaseDlg ) {
		if( m_pBaseDlg->m_pStrManager ) {
			m_pStrManager = m_pBaseDlg->m_pStrManager;
			pclCM = m_pBaseDlg->m_pStrManager->pclCM;
		}
	}
	
	InitDlg();

	Hide();
}

void NStrSubTabDlg::SetControlMap( int nControlMapID, TCHAR *controlid )
{
	cltClient *pclclient = (cltClient*)pclClient;

	for( int i = 0; i < m_controlnum; ++i )
	{
		if( _tcscmp( controlid, m_controlid[ i ] ) == 0 ) {

			m_pControlMap[ nControlMapID ] = m_pChildControl[ i ];
			m_nControlID[ nControlMapID ] = m_pChildControl[ i ]->GetControllerID();
			return;
		}
	}

	TCHAR buffer[ 128 ];
	StringCchPrintf( buffer, 128, TEXT("StrSub setcontrolmap error: %s"), controlid );

	stMsgBox MsgBox;
	MsgBox.Set( this, NULL, buffer, MSGBOX_TYPE_OK, 0 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}

// 탭 다이얼로그 메시지 처리 콜백 함수 
void CALLBACK NStrSubTabDlg::StaticNDlgTabProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	((NStrSubTabDlg*)pControllerMgr)->NDlgTabProc( nEvent, nControlID, pControl );
}


void NStrSubTabDlg::SetMainExplain( TCHAR *pText )
{
	m_pBaseDlg->SetMainExplain( pText );
}


void NStrSubTabDlg::SetEditText( int nControlMapID, TCHAR *text )
{
	((CEdit*)m_pControlMap[ nControlMapID ])->SetText( text );
}

TCHAR* NStrSubTabDlg::GetEditText( int nControlMapID )
{
	return ((CEdit*)m_pControlMap[ nControlMapID ])->GetText();
}

void NStrSubTabDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 확인창
		{
		}
		break;
	}
}

// _LEON_MADE_NCOMBOBOX_SORT
void NStrSubTabDlg::NComboBoxSort(CComboBox* in_pCombo )	// Ninterface ComboBox Sort
{
	if( in_pCombo == NULL ) return;

	stComboBoxItemData	*tmpComboBoxItemDatas = new stComboBoxItemData[ MAX_COMBOBOX_ITEM_NUM ];
	stComboBoxItemData	*pTmpComboBoxItemData = NULL;
	SI16 siCount = 0;

	SI32 i=0;

	for( i= 0; i<MAX_COMBOBOX_ITEM_NUM; i++)
	{
        tmpComboBoxItemDatas[i].Init();
	}

	pTmpComboBoxItemData = in_pCombo->GetItemData( 0 );
	while( pTmpComboBoxItemData != NULL )
	{
		memcpy(tmpComboBoxItemDatas + siCount, pTmpComboBoxItemData, sizeof(stComboBoxItemData));
        siCount++;
		if(siCount >= MAX_COMBOBOX_ITEM_NUM) break;
		pTmpComboBoxItemData = in_pCombo->GetItemData( siCount );
	}

//	TCHAR buffer[256];
	TCHAR text[5000] =TEXT("");

	SI32 clearnum	= 0;
	SI32 totalnum	= 0;
	//데이터를 갱신하기 전에 깨끗이 지운다.
	in_pCombo->Clear();

	SI32  siSortIndexByName[ MAX_COMBOBOX_ITEM_NUM ];

	memset( siSortIndexByName, -1, sizeof(siSortIndexByName));
 
	for( i = 0; i < siCount;i++ )
	{
		siSortIndexByName[i] = i;
	}

	//  i-1로 했으므로 0부터..
	for( i = 0; i < siCount; i++ )
	{
		if( siSortIndexByName[ 0 ] < 0  ) continue;
		const TCHAR* pBuffer =  tmpComboBoxItemDatas[ siSortIndexByName[ 0 ] ].strText;

		for (SI32 j=1; j<siCount - i; j++)
		{
			if( siSortIndexByName[ j ] < 0 ) continue;
			const TCHAR* pname =  tmpComboBoxItemDatas[ siSortIndexByName[ j ] ].strText;
            if( _tcscmp( pBuffer, pname) > 0 )
			{
				SI32 siTmp = 0;
				siTmp = siSortIndexByName[ j-1 ];
				siSortIndexByName[ j-1 ] = siSortIndexByName[ j ];
				siSortIndexByName[ j ] = siTmp;
			}

			pBuffer = tmpComboBoxItemDatas[ siSortIndexByName[ j ] ].strText;
		}
	}

	for(i = 0; i < siCount; i++)
	{
		if( siSortIndexByName[ i ] < 0 ) continue;

		in_pCombo->AddItem( &(tmpComboBoxItemDatas[ siSortIndexByName[ i ] ]) );
	}

	delete [] tmpComboBoxItemDatas;

	in_pCombo->Refresh();
}

// _LEON_MADE_NLISTBOX_SORT
void NStrSubTabDlg::NListSort(CList* in_pList, SI16 in_SortColumn )		// Ninterface ListBox Sort
{
	if( in_pList == NULL || in_SortColumn < 0 || in_SortColumn >= MAX_LISTBOX_COLUMN_NUM ) return;

	stListBoxItemData	*tmpListBoxItemDatas = new stListBoxItemData[ MAX_LISTBOX_ITEM_NUM ];
	stListBoxItemData	*pTmpListBoxItemData = NULL;
	SI16 siCount = 0;

	SI32 i=0;

	for( i= 0; i<MAX_LISTBOX_ITEM_NUM; i++)
	{
        tmpListBoxItemDatas[i].Init();
	}

	pTmpListBoxItemData = in_pList->GetItemData( 0 );
	while( pTmpListBoxItemData != NULL )
	{
		memcpy(tmpListBoxItemDatas + siCount, pTmpListBoxItemData, sizeof(stListBoxItemData));
        siCount++;
		if(siCount >= MAX_LISTBOX_ITEM_NUM) break;
		pTmpListBoxItemData = in_pList->GetItemData( siCount );
	}

//	TCHAR buffer[256];
	TCHAR text[5000] =TEXT("");

	SI32 clearnum	= 0;
	SI32 totalnum	= 0;
	//데이터를 갱신하기 전에 깨끗이 지운다.
	in_pList->Clear();

	SI32  siSortIndexByName[ MAX_LISTBOX_ITEM_NUM ];

	memset( siSortIndexByName, -1, sizeof(siSortIndexByName));
 
	for( i = 0; i < siCount;i++ )
	{
		siSortIndexByName[i] = i;
	}

	//  i-1로 했으므로 0부터..
	for( i = 0; i < siCount; i++ )
	{
		if( siSortIndexByName[ 0 ] < 0  ) continue;
		const TCHAR* pBuffer =  tmpListBoxItemDatas[ siSortIndexByName[ 0 ] ].strText[in_SortColumn];

		for (SI32 j=1; j<siCount - i; j++)
		{
			if( siSortIndexByName[ j ] < 0 ) continue;
			const TCHAR* pname =  tmpListBoxItemDatas[ siSortIndexByName[ j ] ].strText[in_SortColumn];
            if( _tcscmp( pBuffer, pname) > 0 )
			{
				SI32 siTmp = 0;
				siTmp = siSortIndexByName[ j-1 ];
				siSortIndexByName[ j-1 ] = siSortIndexByName[ j ];
				siSortIndexByName[ j ] = siTmp;
			}

			pBuffer = tmpListBoxItemDatas[ siSortIndexByName[ j ] ].strText[in_SortColumn];
		}
	}

	for(i = 0; i < siCount; i++)
	{
		if( siSortIndexByName[ i ] < 0 ) continue;

		in_pList->AddItem( &(tmpListBoxItemDatas[ siSortIndexByName[ i ] ]) );
	}

	delete [] tmpListBoxItemDatas;

	in_pList->Refresh();
}

