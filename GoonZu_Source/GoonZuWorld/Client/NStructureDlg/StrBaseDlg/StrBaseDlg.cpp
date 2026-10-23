#include "StrBaseDlg.h"

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

#include "../../CommonLogic/Cityhall/Cityhall.h"

extern cltCommonLogic* pclClient;
extern SI32 gsiResolution;

#define _NEW_INTERFACE_


SI32 NStrBaseDlg::MinimizeFlagBit = 0;


NStrBaseDlg::NStrBaseDlg()
{
	m_pExplainEdit		= NULL;
	m_pPortraitImage	= NULL;
	m_pCloseBtn			= NULL;
	m_pChiefToggleBtn	= NULL;
	
	m_pMinimizeButton	= NULL;
	m_pUnMinimizeButton = NULL;
        
	for( int i = 0; i < MAX_CHILD_DLG; ++i ) 
	{
		m_pUserTabButton[ i ]	= NULL;
		m_pUserChildDlg[ i ]	= NULL;
		m_pChiefTabButton[ i ]	= NULL;
		m_pChiefChildDlg[ i ]	= NULL;
	}

	m_pStrManager = NULL;

	m_dwActionCount		= 0;
	m_bMini				= FALSE;
	m_siOldActiveTab	= 0;
	m_siMinimizedSlot	= 0;
	m_szOldTitle[0]		= NULL;
}


NStrBaseDlg::~NStrBaseDlg()
{
	if(m_siMinimizedSlot )
	{
		SetEmptyMinimizeSlot( m_siMinimizedSlot );
	}

	SetTimerOnOff( false );

	if( m_pStrManager ) 
	{
		m_pStrManager->pStrBaseDlg = NULL;
		m_pStrManager = NULL;
	}
	
	NDelete( m_pMinimizeButton		);
	NDelete( m_pUnMinimizeButton	);
	NDelete( m_pExplainEdit			);
	NDelete( m_pPortraitImage		);
	NDelete( m_pCloseBtn			);
	NDelete( m_pChiefToggleBtn		);

	for( int i = 0; i < MAX_CHILD_DLG; ++i )
	{
		NDelete( m_pUserTabButton[ i ]	);
		NDelete( m_pUserChildDlg[ i ]	);
		NDelete( m_pChiefTabButton[ i ] );
		NDelete( m_pChiefChildDlg[ i ]	);
	}

}

void NStrBaseDlg::CreateDlg( SI16 id, cltStructureMngBasic* pStrManager, int portrait )
{
	m_portraitindex = portrait;
	m_bChiefMode	= FALSE;
	SetTabInfo( 1, 1 );

	if ( !IsCreate() )
	{
		CInterfaceFile file;

		if( file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_NStrBaseDlg.ddf")) == false ) return;

		file.CreatePopupDialog( this, id, TEXT("dialog_strbase"), StaticNDlgProc );
	
		m_pExplainEdit		= new CEdit( this );
		m_pPortraitImage	= new CImageStatic( this );
		m_pCloseBtn			= new CButton( this );
		m_pChiefToggleBtn	= new CButton( this );
		m_pMinimizeButton	= new CButton( this);
		m_pUnMinimizeButton = new CButton( this);
	
		file.CreateControl( m_pExplainEdit,			NSTRBASE_EDIT_EXPLAIN,			TEXT("editbox_explain")			);
		file.CreateControl( m_pPortraitImage,		NSTRBASE_IMAGE_PORTRAIT,		TEXT("imagestatic_portrait")	);
		file.CreateControl( m_pCloseBtn,			NSTRBASE_BUTTON_CLOSE,			TEXT("button_close")			);
		file.CreateControl( m_pChiefToggleBtn,		NSTRBASE_BUTTON_CHIEFTOGGLE,	TEXT("button_chief")			);
		file.CreateControl( m_pMinimizeButton,		NSTRBASE_BUTTON_MINIMIZE,		TEXT("button_minimize")			);
		file.CreateControl( m_pUnMinimizeButton,	NSTRBASE_BUTTON_UNMINIMIZE,		TEXT("button_unminimize")		);
		
		// Set ChildDialog Rect 
		//cyj 차일드 다이얼로그창 20 밑으로 내림
		//SetRect( &m_ChildRect, 7, 126, GetWidth() - 8, 428 );
		SetRect( &m_ChildRect, 7, 146, GetWidth() - 8, 448 );

 		m_pUnMinimizeButton->Show(false);
		m_pMinimizeButton->Show(false);

		m_pStrManager = pStrManager;

		if( m_pStrManager ) 
		{
			m_pStrManager->pStrBaseDlg = this;
			m_pStrManager->Do_WM_INITDIALOG( id );
		}

		SetUnMinimizeButtonImage(id);
	} 
	else 
	{
		DeleteDialog();
	}
}

void NStrBaseDlg::Action()
{
	++m_dwActionCount;

	int i;

	if( m_dwActionCount % 3 == 0 ) 
	{
		if( m_pStrManager )m_pStrManager->ServerRtnValOperate();
	}   

	if( m_bChiefMode ) 
	{
		for( i = 0; i < MAX_CHILD_DLG; ++i ) 
		{
			if( m_pChiefChildDlg[ i ] && m_pChiefChildDlg[ i ]->IsShow() ) 
				m_pChiefChildDlg[ i ]->Action();
		}
	} 
	else 
	{
		for( i = 0; i < MAX_CHILD_DLG; ++i ) 
		{
			if( m_pUserChildDlg[ i ] && m_pUserChildDlg[ i ]->IsShow() ) 
				m_pUserChildDlg[ i ]->Action();
		}
	}

}

void NStrBaseDlg::SetTabInfo( int row, int currentrow )
{
	m_row = row;
	m_currentrow = currentrow;

	if( row == 1 ) 
	{
		m_tabHeight = 24;
		
		m_tabX = 5;
		m_tabY = 100;

	}
	else 
	{
		m_tabHeight = 16;

		m_tabX = 5;
		m_tabY = 96 + ( m_tabHeight * ( currentrow - 1 ) );
	}
}


void NStrBaseDlg::AddUserTabDialog( int index, TCHAR *text, int width, NStrTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID )
{
	TCHAR *btnfilename[] = 
	{
		TEXT("ninterface/button/buttona_4x24_00_103.spr"),
		TEXT("ninterface/button/buttona_4x16_00_103.spr")	// [진성] 보라색 버튼으로 수정. => 2008-4-1
	};	

	width = ( width * 4 + 3 ) / 4;

	if( m_pUserTabButton[ index ] || m_pUserChildDlg[ index ] ) return;

	m_pUserTabButton[ index ] = new CButton( this );

	m_pUserTabButton[ index ]->Create(	NSTRBASE_USER_TAB_BUTTON + index, 
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
	
#ifndef _NEW_INTERFACE_
	m_pUserTabButton[ index ]->SetAniType( true );
#else
	//m_pUserTabButton[ index ]->SetAniType( true );
#endif

	m_tabX += width;

	m_pUserChildDlg[ index ] = pChildDlg;

	// 차일드 다이얼로그 생성
	if( pChildDlg ) pChildDlg->CreateTabDlg( index, this, filename, szID, FALSE );
}

void NStrBaseDlg::AddChiefTabDialog( int index, TCHAR *text, int width, NStrTabDlg *pChildDlg, TCHAR *filename, TCHAR *szID )
{
	TCHAR *btnfilename[] = 
	{
		TEXT("ninterface/button/buttona_4x24_00_103.spr"),
		TEXT("ninterface/button/buttona_4x16_00_103.spr") // [진성] 보라색 버튼으로 수정. => 2008-4-1
	};	

	width = ( width * 4 + 3 ) / 4;

	if( m_pChiefTabButton[ index ] || m_pChiefChildDlg[ index ] ) return;

	m_pChiefTabButton[ index ] = new CButton( this );

	m_pChiefTabButton[ index ]->Create(	NSTRBASE_CHIEF_TAB_BUTTON + index, 
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
	//m_pUserTabButton[ index ]->SetAniType( true );
#endif

	m_tabX += width;

	m_pChiefChildDlg[ index ] = pChildDlg;

	// 차일드 다이얼로그 생성
	if( pChildDlg ) pChildDlg->CreateTabDlg( index, this, filename, szID, TRUE );
}

void NStrBaseDlg::SetChiefMode( BOOL bChief, BOOL bStateShow )
{
	HideAllTab();
	ShowAllTabButton( false );

	m_bChiefMode = bChief;

	ShowAllTabButton( true );
	SetActiveTab( 0 );

	if( m_bChiefMode ) 
	{
		TCHAR* pText = GetTxtFromMgr(3900);
		m_pChiefToggleBtn->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}
	else 
	{
		TCHAR* pText = GetTxtFromMgr(3901);
		m_pChiefToggleBtn->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
	}

//#if defined(_CITYHALL_BANKRUPTCY)
	//if( bStateShow == FALSE ) return;

	if( m_bChiefMode )
	{
		cltClient* pclclient = (cltClient*)pclClient;

		UI08 usCurStatus = 0;
		
		if( pclclient == NULL || pclclient->pclCityHallManager == NULL ) return;

		usCurStatus = pclclient->pclCityHallManager->clClientStrInfo.clCityhallStrInfo.usCurStatus;

		TCHAR buffer[500];

		memset( buffer, 0, sizeof(buffer));

		switch( usCurStatus )
		{
		case CITYHALL_BEING_STABLE	:
			{
				TCHAR buffer2[512];
				memset( buffer2, 0, sizeof(buffer2));
				//TCHAR* pText1 = GetTxtFromMgr(1610);
				TCHAR* pText2 = GetTxtFromMgr(5163);
				//sprintf( buffer, TEXT("%s\r\n%s"), pText1, pText2 );
				StringCchPrintf( buffer, 500, TEXT("%s"), pText2 );
				StringCchPrintf( buffer2, 512, buffer, CITYHALL_STABLE_YEARS_AFTER_CHIEF_ELECTION );

				SetMainExplain( buffer2 );
			}
			break;
		case CITYHALL_BEING_BANKRUPT:
			{
				//TCHAR* pText1 = GetTxtFromMgr(1610);
				TCHAR* pText2 = GetTxtFromMgr(5198);
				//sprintf( buffer, TEXT("%s\r\n\r\n%s"), pText1, pText2 );

				//sprintf( buffer, TEXT("%s\r\n%s"), pText1, pText2 );
				StringCchPrintf( buffer, 500, TEXT("%s"), pText2 );

				SetMainExplain( buffer );
			}
			break;
		}
	}
	else
	{
		cltClient* pclclient = (cltClient*)pclClient;

		UI08 usCurStatus = 0;
		
		if( pclclient == NULL || pclclient->pclCityHallManager == NULL ) return;

		usCurStatus = pclclient->pclCityHallManager->clClientStrInfo.clCityhallStrInfo.usCurStatus;

		TCHAR buffer[500];

		memset( buffer, 0, sizeof(buffer));

		switch( usCurStatus )
		{
		case CITYHALL_BEING_BANKRUPT:
			{
				//TCHAR* pText1 = GetTxtFromMgr(1610);
				TCHAR* pText2 = GetTxtFromMgr(5198);
				//sprintf( buffer, TEXT("%s\r\n\r\n%s"), pText1, pText2 );
				StringCchPrintf( buffer, 500, TEXT("%s"), pText2 );

				SetMainExplain( buffer );
			}
			break;
		}
	}
//#endif
}

void NStrBaseDlg::SetActiveTab( int index )
{
	HideAllTab();
 
	if( m_bChiefMode ) 
	{
		if( m_pChiefChildDlg[ index ] ) 
		{
			m_pChiefChildDlg[ index ]->Show();

			m_pChiefTabButton[ index ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pChiefTabButton[ index ]->SetFontWeight( FW_BOLD );
			m_pChiefTabButton[ index ]->Refresh();
		}

	} 
	else 
	{
		if( m_pUserChildDlg[ index ] ) 
		{
			m_pUserChildDlg[ index ]->Show();

			m_pUserTabButton[ index ]->SetFontColor( RGB( 0, 0, 0 ) );
			m_pUserTabButton[ index ]->SetFontWeight( FW_BOLD );
			m_pUserTabButton[ index ]->Refresh();
		}
	}
	m_siOldActiveTab = index;
}

void NStrBaseDlg::HideAllTab()
{
	for( int i = 0; i < MAX_CHILD_DLG; ++i ) 
	{
		if( m_bChiefMode ) 
		{
			if( m_pChiefChildDlg[ i ] ) 
			{
				m_pChiefChildDlg[ i ]->Hide();

				m_pChiefTabButton[ i ]->SetFontColor( RGB( 20, 20, 20 ) );
				m_pChiefTabButton[ i ]->SetFontWeight( FW_NORMAL );
				m_pChiefTabButton[ i ]->Refresh();
			}
		}
		else 
		{
			if( m_pUserChildDlg[ i ] ) 
			{
				m_pUserChildDlg[ i ]->Hide();
	
				m_pUserTabButton[ i ]->SetFontColor( RGB( 20, 20, 20 ) );
				m_pUserTabButton[ i ]->SetFontWeight( FW_NORMAL );
				m_pUserTabButton[ i ]->Refresh();
			}
		}
	}
}

void NStrBaseDlg::ShowAllTabButton( bool bShow )
{
	for( int i = 0; i < MAX_CHILD_DLG; ++i ) 
	{
		if( m_bChiefMode ) 
		{
			if( m_pChiefTabButton[ i ] ) m_pChiefTabButton[ i ]->Show( bShow );

		}
		else 
		{
			if( m_pUserTabButton[ i ] ) m_pUserTabButton[ i ]->Show( bShow );
		}
	}
}

void NStrBaseDlg::SetMainExplain( TCHAR *text )
{
	if( m_pExplainEdit ) m_pExplainEdit->SetText( text );

	m_pStrManager->dwLastExplainTick = GetTickCount();
}

void NStrBaseDlg::Minimi(BOOL bMini )
{
	if(m_bMini == bMini) return;

	cltClient *pclclient = (cltClient*)pclClient;

	if( bMini )
	{	
		m_siOldPosX = GetX();
		m_siOldPosY = GetY();
		SetMoveable(FALSE);

		// todo MinimizeFlgBit 의 값을 읽어와 최소화 된 버튼의 위치를 결정한다.
		m_siMinimizedSlot =  GetEmptyMinimizeSlot();

		// 빈 슬롯에 따른 x 좌표로 움직여 준다.
		SetDialogPos( pclclient->GetScreenXSize() - 22* m_siMinimizedSlot -5  , 210, 20, 24 );
 		SetFileName(NULL);
		
		StringCchCopy( m_szOldTitle,MAX_PATH, GetTitle()) ;
		SetTitle(NULL);

		m_pExplainEdit->Show(false);
		m_pPortraitImage->Show(false);
		m_pCloseBtn->Show(false);
		m_pChiefToggleBtn->Show(false);
		m_pMinimizeButton->Show(false);
		m_pUnMinimizeButton->Show(false);
		
		
		for( SI32 i = 0; i < MAX_CHILD_DLG; i++)
		{
			if(m_pUserTabButton[ i ])
				m_pUserTabButton[ i ]->Show(false);
			
			if(m_pChiefTabButton[ i ])
				m_pChiefTabButton[ i ]->Show(false);
		}

		HideAllTab();

		m_pMinimizeButton->Show(false);
		m_pUnMinimizeButton->Show(true);
		m_pCloseBtn->Show(false);

	}
	else
	{
		SetMoveable(TRUE);
		SetDialogPos( m_siOldPosX, m_siOldPosY, 360, 480 );
		SetFileName("ninterface/interface/interface_20x20_00_109.spr");
		SetEmptyMinimizeSlot(m_siMinimizedSlot);		
		
		SetTitle(m_szOldTitle);

		m_pExplainEdit->Show(true);
		m_pPortraitImage->Show(true);
		m_pCloseBtn->Show(true);
		m_pChiefToggleBtn->Show(true);
		m_pMinimizeButton->Show(true);
		m_pUnMinimizeButton->Show(true);
		
		if(m_bChiefMode == false)
		{
			for( SI32 i = 0; i < MAX_CHILD_DLG; i++)
			{
				if(m_pUserTabButton[ i ])
					m_pUserTabButton[ i ]->Show(true);
			}
		}
		else
		{
			for( SI32 i = 0; i < MAX_CHILD_DLG; i++)
			{
				if(m_pChiefTabButton[ i ])
					m_pChiefTabButton[ i ]->Show(true);
			}
		}

		SetActiveTab(m_siOldActiveTab);

		m_pMinimizeButton->Show(true);
		m_pUnMinimizeButton->Show(false);
		m_pCloseBtn->Show(true);
		// todo MinimizeFlgBit 중 자신이 점했던 플레그를 해제

	}
	m_bMini = bMini;    
}

void NStrBaseDlg::SetUnMinimizeButtonImage(SI32 id)
{
	
	switch (id)
	{
	case NBANK_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(4);
		}
		break;

	case NSTOCK_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(28);
		}
		break;

	case NMARKET_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(32);
		}
		break;
	case NHOUSE_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(8);
		}
		break;
	case NPOSTOFFICE_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(12);
		}
		break;
	case NHUNT_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(24);
		}
		break;
	case NLAND_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(20);
		}
		break;
	case NMINE_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(16);
		}
		break;
	case NSUMMONHEROMARKET_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(52);
		}
		break;
	case NSUMMONMARKET_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(48);
		}
		break;
	case NFEAST_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(36);
		}
		break;
	case NGUILD_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(40);
		}
		break;
	case NREALESTATEMARKET_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(56);
		}
		break;
	case NHORSEMARKET_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(44);
		}
		break;
	case NNEWMARKET_DLG:
		{
			m_pUnMinimizeButton->SetFontIndex(44);
		}
		break;
	}
}

SI16 NStrBaseDlg::GetEmptyMinimizeSlot()
{
	// 아홉개의 건물 다이얼로그가 최소화 되었을때 위치할 슬롯을 찾기 위해 최소화된 다이얼로그 위치를 기억하는 비트테이블

	SI32 CheckVal = 0x1;
	for( SI16 i = 1; i <= MAX_STRUCTURE_IN_VILLAGE; i++ )
	{
		// 비트테이블과 특정 트루 비트와의 오어연산
		SI32 CheckBit =	MinimizeFlagBit | CheckVal ;
		
		// 오어 연산으로 값이 변했다면 해당 트루 비트가 폴즈 비트를 변화 시킨것이므로 해당 비트는 폴즈였다.
		// 그렇다면 빈 슬롯! 이다.
		if( CheckBit != MinimizeFlagBit  )	
		{
			MinimizeFlagBit = CheckBit;
			return i;
        }
		CheckVal *= 2;
	}
	return 0;
}

void NStrBaseDlg::SetEmptyMinimizeSlot(SI16 emptySlot)
{
	// 건물이 다시 최대화 되었을 경우 점유하고 있던 슬롯 비트를 다시 폴즈

	// 엉뚱한 인자면 바로 끝냄
	if(emptySlot <= 0 || emptySlot > MAX_STRUCTURE_IN_VILLAGE  )return ;


	SI32 CheckVal = 0x1;

	for( SI16 i = 1; i < emptySlot; i++ )
	{
		CheckVal *= 2;
	}
	
    CheckVal = ~CheckVal;	// 비트반전

	MinimizeFlagBit &= CheckVal;	// 앤드 연산으로 빈슬롯 비트를 폴즈로 만듬

    
	return;
}

void NStrBaseDlg::InitDlg(BOOL bStateShow)
{
	SetChiefMode( FALSE, bStateShow );
}

void NStrBaseDlg::NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	for( int i = 0; i < MAX_CHILD_DLG; ++i ) 
	{
		if( m_bChiefMode ) 
		{
			if( m_pChiefTabButton[ i ] == NULL ) continue;			
			if( nControlID == m_pChiefTabButton[ i ]->GetControllerID() && nEvent == EVENT_BUTTON_CLICKED ) 
			{
				SetActiveTab( i );
			}
		} 
		else 
		{
			if( m_pUserTabButton[ i ] == NULL ) continue;
			if( nControlID == m_pUserTabButton[ i ]->GetControllerID() && nEvent == EVENT_BUTTON_CLICKED ) 
			{
				SetActiveTab( i );
			}
		}
	}

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
	case NSTRBASE_BUTTON_CLOSE:
		{
			switch( nEvent ) 
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				return;
			}			
		}
		break;
	case NSTRBASE_BUTTON_MINIMIZE:
		{
			switch( nEvent ) 
			{
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(TRUE);					
				}
				return;
			}	
		}
		break;
	case NSTRBASE_BUTTON_UNMINIMIZE:
		{
			switch( nEvent ) 
			{
			case EVENT_BUTTON_CLICKED:
				{
					Minimi(FALSE);					
				}
				return;
			}	

		}
		break;
	case NSTRBASE_BUTTON_CHIEFTOGGLE:
		{
			switch( nEvent ) 
			{
			case EVENT_BUTTON_CLICKED:
				{

					if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) 
					{
					// 자신이 (대)행수로 이미 등록되어 있어야  한다. 
						cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
						if( m_pStrManager->pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == FALSE )
						{
							TCHAR* pText;
							if( m_pStrManager->siRankType == RANKTYPE_CITYHALL ) {
								pText = GetTxtFromMgr(1547);
							}
							else pText = GetTxtFromMgr(1440);
							SetMainExplain(pText);
							return;
						}
					}
					if (m_pStrManager->siRankType == RANKTYPE_POSTOFFICE) {//우체국 관리자 메뉴로 들어갈때 인사 메뉴 바꿈[2007.07.18 손성웅]
						cltSimpleRank clrank( m_pStrManager->siRankType, m_pStrManager->siVillageUnique);
						if ( m_pStrManager->pclCM->CR[1]->pclCI->clCharRank.IsSame(&clrank) == TRUE) 
						{
							TCHAR* pText;
							pText = GetTxtFromMgr(7550);
							SetMainExplain(pText);
						}
					}

					SetChiefMode( !m_bChiefMode );
				}

				break;
			}
		}
		break;
	}
}


void CALLBACK NStrBaseDlg::StaticNDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	((NStrBaseDlg*)pControllerMgr)->NDlgProc( nEvent, nControlID, pControl );	
}

void NStrBaseDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
 	if(m_bMini == TRUE) return;

	int x = ScreenRX + GetX();
	int y = ScreenRY + GetY();

	g_graphics.Box16C( 	x + m_ChildRect.left, y + m_ChildRect.top,
						x + m_ChildRect.right, y + m_ChildRect.bottom, 
						g_graphics.MakeRGB16(  CONTROLLER_OUTLINE_COLOR ) 
						);

	TSpr *pspr = pclClient->GetGlobalSpr( GIMG_VILLAGEUNITNPC );

	if( m_portraitindex < 0 || m_portraitindex >= 6 ) 
		m_portraitindex = 0;

	GP.PutSpr(pspr, GetX() + ScreenRX + 12, GetY() + ScreenRY + 25, m_portraitindex );
}

// [영훈] 텝다이얼로그 버튼 활성화
void NStrBaseDlg::EnableTabButton( int nIndex, bool bChefMode /* = false  */)
{
	if ( (0 > nIndex) || (MAX_CHILD_DLG >= nIndex) )
	{
		return;
	}

	// 관리자용 텝 다이얼로그 버튼
	if ( bChefMode )
	{
		m_pChiefTabButton[ nIndex ]->Enable( true );

		//m_pChiefTabButton[ nIndex ]->SetFontColor( RGB( 0, 0, 0 ) );
		//m_pChiefTabButton[ nIndex ]->SetFontWeight( FW_BOLD );
		m_pChiefTabButton[ nIndex ]->Refresh();
	}
	// 유저용 텝 다이얼로그 버튼
	else
	{
		m_pUserTabButton[ nIndex ]->Enable( true );

		//m_pUserTabButton[ nIndex ]->SetFontColor( RGB( 0, 0, 0 ) );
		//m_pUserTabButton[ nIndex ]->SetFontWeight( FW_BOLD );
		m_pUserTabButton[ nIndex ]->Refresh();
	}

}

// [영훈] 텝다이얼로그 버튼 비활성화
void NStrBaseDlg::DisableTabButton( int nIndex, bool bChefMode /* = false  */)
{
	if ( (0 > nIndex) || (MAX_CHILD_DLG <= nIndex) )
	{
		return;
	}

	// 관리자용 텝 다이얼로그 버튼
	if ( bChefMode )
	{
		m_pChiefTabButton[ nIndex ]->Enable( false );
	}
	// 유저용 텝 다이얼로그 버튼
	else
	{
		m_pUserTabButton[ nIndex ]->Enable( false );
	}

}


//----------------------------------------------------------------------------
NStrTabDlg::NStrTabDlg()
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


NStrTabDlg::~NStrTabDlg()
{
	for( int i = 0; i < MAX_CHILD_CONTROL; ++i ) {
		NDelete( m_pChildControl[ i ] );
	}
}

// 탭 다이얼로그를 생성한다
void NStrTabDlg::CreateTabDlg( int index, NStrBaseDlg *pParent, TCHAR *filename, TCHAR *szID, BOOL bChief )
{

	CInterfaceFile file;

	if( file.LoadFile( filename ) == false ) return;

	if( bChief ) 
	{
		file.CreateChildDialog( this, NSTRBASE_CHIEF_TAB_DIALOG + index, szID, StaticNDlgTabProc, pParent );
	}
	else 
	{
		file.CreateChildDialog( this, NSTRBASE_USER_TAB_DIALOG + index, szID, StaticNDlgTabProc, pParent );
	}

	MovePos( 0, pParent->m_ChildRect.top );

	for( int i = 0; i < file.m_controlNum; ++i ) 
	{
		file.CreateControl( this, &m_pChildControl[ i ], NSTRBASE_CHILD_CONTROL + i, NULL, i );
		
		memcpy( m_controlid[ i ], file.m_control[ i ].id, 32 );
		m_controlid[ i ][ 31 ] = NULL;

	}
	m_controlnum = file.m_controlNum;

	m_pBaseDlg = pParent;

	if( m_pBaseDlg ) 
	{
		if( m_pBaseDlg->m_pStrManager ) 
		{
			m_pStrManager = m_pBaseDlg->m_pStrManager;
			pclCM = m_pBaseDlg->m_pStrManager->pclCM;
		}
	}
	
	InitDlg();

	Hide();
}

void NStrTabDlg::SetControlMap( int nControlMapID, TCHAR *controlid )
{
	cltClient *pclclient = (cltClient*)pclClient;

	for( int i = 0; i < m_controlnum; ++i )
	{
		if( _tcscmp( controlid, m_controlid[ i ] ) == 0 ) 
		{
			m_pControlMap[ nControlMapID ] = m_pChildControl[ i ];
			m_nControlID[ nControlMapID ] = m_pChildControl[ i ]->GetControllerID();
			return;
		}
	}

	TCHAR buffer[ 128 ];
	StringCchPrintf( buffer, 128, TEXT("setcontrolmap error: %s"), controlid );

	stMsgBox MsgBox;
	MsgBox.Set( this, NULL, buffer, MSGBOX_TYPE_OK, 0 );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

	//MessageBox( NULL, buffer, NULL, NULL );
}

// 탭 다이얼로그 메시지 처리 콜백 함수 
void CALLBACK NStrTabDlg::StaticNDlgTabProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	((NStrTabDlg*)pControllerMgr)->NDlgTabProc( nEvent, nControlID, pControl );
}


void NStrTabDlg::SetMainExplain( TCHAR *pText )
{
	m_pBaseDlg->SetMainExplain( pText );
}


void NStrTabDlg::SetEditText( int nControlMapID, TCHAR *text , COLORREF fontColor)
{
	((CEdit*)m_pControlMap[ nControlMapID ])->SetText( text , fontColor);
}

void NStrTabDlg::SetEditText( int nControlMapID, TCHAR *text )
{
	((CEdit*)m_pControlMap[ nControlMapID ])->SetText( text );
}

TCHAR* NStrTabDlg::GetEditText( int nControlMapID )
{
	return ((CEdit*)m_pControlMap[ nControlMapID ])->GetText();
}

void NStrTabDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
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
void NStrTabDlg::NComboBoxSort(CComboBox* in_pCombo )	// Ninterface ComboBox Sort
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

	//TCHAR buffer[256];
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
void NStrTabDlg::NListSort(CList* in_pList, SI16 in_SortColumn )		// Ninterface ListBox Sort
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

	//TCHAR buffer[256];
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

//void NStrTabDlg::VDirectDrawToBackScreen(LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY)
//{
//    
//}