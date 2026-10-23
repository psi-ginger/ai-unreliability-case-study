/* ==========================================================================
	클래스 :		NConstructDlg

	작성일 :		05/04/29
	작성자 :		정용래
	
	변경사항 :		없음

	차후작업 :		

   ========================================================================*/

#include "./NCityhallConstructDlg.h" 

//#if defined(_DEBUG) && defined(_NCITYHALLCONSTRUCTOR_)

#include <tchar.h>
#include <CommonLogic.h>

#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"
#include "Map\FieldObject\FieldObject.h"

#include "MsgType-Structure.h"

#include "../../../Client/InterfaceMgr/Interface/Button.h"
#include "../../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../../Client/InterfaceMgr/Interface/OutLine.h"
#include "../../../Client/InterfaceMgr/Interface/Static.h"
#include "../../../Client/InterfaceMgr/Interface/List.h"
#include "../../../Client/InterfaceMgr/Interface/ImageButton.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

cltStructureMngBasic*	NConstructDlg::m_pclStructureManager;
BOOL					NConstructDlg::m_bUpdateSwitch;
DWORD					NConstructDlg::m_dwLastTick;

SI32					NConstructDlg::m_currentIndex;
SI32					NConstructDlg::m_currentUnique;
SI32					NConstructDlg::m_buildUnique;

extern cltCommonLogic* pclClient;

NConstructDlg::NConstructDlg()
{
	m_bShow = false;
	m_bUpdateSwitch = FALSE;


	m_pStatic_able_structure = NULL; 
	m_pButton_destruction	 = NULL; 
	m_pButton_construction	 = NULL; 
	m_pEdit_explain			 = NULL; 
	m_pList_able_structure	 = NULL; 
	m_pStatic_Background	 = NULL;

	m_currentIndex = -1;
	m_currentUnique = 0;

	for ( SI32 i = 0; i<MAX_CONSTRUCTION_NUM; ++i )
	{
		m_pButton_SelectPostion[i] = NULL;
		m_siStructureUnique[i] = 0;
	}
	//m_pList_structure	 = NULL; 
	//m_pStatic_NONAME1	 = NULL; 
	//m_pOutLine_NONAME2	 = NULL; 
	//m_pStatic_structure	 = NULL; 
	//m_pStatic_destruction	 = NULL; 
	//m_pStatic_construction	 = NULL; 
	//m_pEdit_destruction	 = NULL; 
	//m_pEdit_position	 = NULL; 
	//m_pEdit_construction	 = NULL; 
	//m_pOutLine_NONAME3	 = NULL; 

}

NConstructDlg::~NConstructDlg()
{
	if( 	m_pStatic_able_structure	 )  	delete 	m_pStatic_able_structure	;
	if( 	m_pButton_destruction	 )  	delete 	m_pButton_destruction	;
	if( 	m_pButton_construction	 )  	delete 	m_pButton_construction	;
	if( 	m_pEdit_explain	 )  	delete 	m_pEdit_explain	;
	if( 	m_pList_able_structure	 )  	delete 	m_pList_able_structure	;
	SAFE_DELETE( m_pStatic_Background );

	for ( SI32 i = 0; i<MAX_CONSTRUCTION_NUM; ++i )
	{
		SAFE_DELETE( m_pButton_SelectPostion[i] );
	}

	//if( 	m_pList_structure	 )  	delete 	m_pList_structure	;	
	//if( 	m_pStatic_NONAME1	 )  	delete 	m_pStatic_NONAME1	;
	//if( 	m_pOutLine_NONAME2	 )  	delete 	m_pOutLine_NONAME2	;
	//if( 	m_pStatic_structure	 )  	delete 	m_pStatic_structure	;
	//if( 	m_pStatic_destruction	 )  	delete 	m_pStatic_destruction	;
	//if( 	m_pStatic_construction	 )  	delete 	m_pStatic_construction	;
	//if( 	m_pEdit_destruction	 )  	delete 	m_pEdit_destruction	;
	//if( 	m_pEdit_position	 )  	delete 	m_pEdit_position	;
	//if( 	m_pEdit_construction	 )  	delete 	m_pEdit_construction	;
	//if( 	m_pOutLine_NONAME3	 )  	delete 	m_pOutLine_NONAME3	;

	SetTimerOnOff( false );

}

// Create() 함수 호출 후 반드시 SetStructureManager() 함수를 호출 해야한다.
void NConstructDlg::Create( )
{

	//m_pclStructureManager = pclStructureManager; => SetStructureManager()
	m_pclStructureManager = pclClient->pclCityHallManager;

	if(!IsCreate())
	{

		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_CityHallConstructDlg.ddf"));

		file.CreatePopupDialog( this, NCITYHALLCONSTRUCT_DLG, TEXT("dialog_cityhallconstruct"), NConstructStaticProc);

		SI32 siTmp = 0, siTmpWidth = 0;

		siTmp = file.GetInfoWithStringID( TEXT("dialog_cityhallconstruct") );

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

		m_pStatic_able_structure	= new 	CStatic	( this );
		m_pButton_destruction		= new 	CButton	( this );
		m_pButton_construction		= new 	CButton	( this );
		m_pEdit_explain				= new 	CEdit	( this );
		m_pList_able_structure		= new 	CList	( this );
		NEWCSTATIC( m_pStatic_Background );
		for ( SI32 i = 0; i<MAX_CONSTRUCTION_NUM; ++i )
		{
			NEWCIMAGEBUTTON( m_pButton_SelectPostion[i] );
		}
		
		file.CreateControl( 	m_pStatic_able_structure, 	NCITYHALLCONSTRUCT_STATICTEXT_ABLE_STRUCTURE,	TEXT("statictext_able_structure") )	;
		file.CreateControl( 	m_pButton_destruction, 		NCITYHALLCONSTRUCT_BUTTON_DESTRUCTION,			TEXT("button_destruction") )	;
		file.CreateControl( 	m_pButton_construction, 	NCITYHALLCONSTRUCT_BUTTON_CONSTRUCTION,			TEXT("button_construction") )	;
		file.CreateControl( 	m_pEdit_explain, 			NCITYHALLCONSTRUCT_EDITBOX_EXPLAIN,				TEXT("editbox_explain") )	;
		file.CreateControl( 	m_pList_able_structure, 	NCITYHALLCONSTRUCT_LISTBOX_ABLE_STRUCTURE,		TEXT("listbox_able_structure") )	;

		m_pButton_SelectPostion[0]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT1, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 16 , 127, 46, 43, 46, 43 );
		m_pButton_SelectPostion[1]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT2, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 150 , 175, 46, 43, 46, 43 );
		m_pButton_SelectPostion[2]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT3, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 216 , 175, 46, 43, 46, 43 );
		m_pButton_SelectPostion[3]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT4, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 83 , 127, 46, 43, 46, 43 );
		m_pButton_SelectPostion[4]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT5, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 16 , 79, 46, 43, 46, 43 );
		m_pButton_SelectPostion[5]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT6, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 150 , 127, 46, 43, 46, 43 );
		m_pButton_SelectPostion[6]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT7, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 216 , 79, 46, 43, 46, 43 );
		m_pButton_SelectPostion[7]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT8, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 216 , 127, 46, 43, 46, 43 );
		m_pButton_SelectPostion[8]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT9, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 83 , 175, 46, 43, 46, 43 );
		m_pButton_SelectPostion[9]->Create( NCITYHALLCONSTRUCT_BUTTON_SELECT10, NULL, 
			TEXT("ninterface/button/BUTTON_62X43_00_000.spr"), 0, false, 0, 16 , 175, 46, 43, 46, 43 );


		file.CreateControl( m_pStatic_Background, NCITYHALLCONSTRUCT_OUTLINE_BACKGROUND, TEXT("statictext_background"));

		// 초기화
		m_currentIndex = 0;
		m_currentUnique = 0;
		m_buildUnique = 0;

		m_dwLastTick = GetTickCount();
		m_bUpdateSwitch = FALSE;

		TCHAR buf[ 256 ];

		{
			int width[] = { 90, 106, 80 };
		//	TCHAR *text[] = { TEXT("건물명"), TEXT("필요레벨"), TEXT("건설비용") };

			m_pList_able_structure->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );

			TCHAR* pText[3];

			pText[0] = GetTxtFromMgr(1612);
			pText[1] = GetTxtFromMgr(1615);
			pText[2] = GetTxtFromMgr(9859);

			for( SI32 i = 0; i < 3; ++i ) 
			{
				m_pList_able_structure->SetColumn( i, width[ i ], pText[i] ); 
			}
		}

		m_pList_able_structure->SetBorder( true );
		m_pList_able_structure->SetBKColorUse( true );

		{
			cltVillageStructureInfo *pStructureInfo;
			SI32 index = 0;

			// 마을 건물 가격 조정
			SI32 npcRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
			SI32 siPenalty = 0;
				 if ( npcRate >= 500 ) 	siPenalty = 100;
			else if ( npcRate >= 400 ) 	siPenalty =  90;
			else if ( npcRate >= 300 ) 	siPenalty =  80;
			else if ( npcRate >= 200 ) 	siPenalty =  70;
			else						siPenalty =  60;


			for( SI32 i = 0; i < MAX_VILLAGE_STRUCTURE_NUMBER; ++i)
			{
				pStructureInfo = pclClient->pclVillageStructureSet->pclVillageStructureInfo[ i ];

				if( pStructureInfo ) 
				{
					//[진성] 통합마켓을 관청에서 건설 가능한 건물을 리스트 등록 및 제외
					if( false == pclClient->IsCountrySwitch( Switch_NewMarket ) )
					{
						if( NEWMARKET_UNIQUE == pStructureInfo->siUnique ) continue;					
					}

					if( pStructureInfo->bDestroyable ) 
					{
						stListBoxItemData tmpListBoxItemData;
						tmpListBoxItemData.Init();
						tmpListBoxItemData.siParam[0] = pStructureInfo->siUnique;
						tmpListBoxItemData.Set( 0, pStructureInfo->szName );

						_itot( pStructureInfo->siNeedMoney * siPenalty / 100, buf, 10 );
						tmpListBoxItemData.Set( 1, buf );

						_itot( pStructureInfo->siNeedVillageScore, buf, 10 );
						tmpListBoxItemData.Set( 2, buf );

						m_pList_able_structure->AddItem( &tmpListBoxItemData );

						index++;
					}
				}
			}

			m_pList_able_structure->Refresh();
		}

		if( m_pclStructureManager != NULL )
		{
			// 건물 리스트 요청 
			SI32 CharUnique = m_pclStructureManager->CharUnique;
			SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

			SI32 id = 1;
			if(id)
			{
				cltMsg clMsg( GAMEMSG_REQUEST_GETSTRUCTURELIST, siVillageUnique );
				
				pclClient->pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
			}

			m_bUpdateSwitch = TRUE;
		}
		else
			m_bUpdateSwitch = FALSE;

//#if defined(_CITYHALL_BANKRUPTCY)
		{
			UI08 usCurStatus = 0;
			
			if( pclclient == NULL || pclclient->pclCityHallManager == NULL ) return;

			usCurStatus = pclclient->pclCityHallManager->clClientStrInfo.clCityhallStrInfo.usCurStatus;

			TCHAR buffer[500];

			memset( buffer, 0, sizeof(buffer));

			switch( usCurStatus )
			{
			case CITYHALL_BEING_STABLE	:
				{
					TCHAR* pText1 = GetTxtFromMgr(5168);
					TCHAR* pText2 = GetTxtFromMgr(5169);
					StringCchPrintf( buffer, 500, TEXT("%s\r\n%s"), pText1, pText2 );

					m_pEdit_explain->SetText( buffer );
				}
				break;
			default:
				{

				}
				break;
			}
		}
//#endif

		m_pButton_destruction->Enable( false );
		m_pButton_construction->Enable( false );

		SetActionTimer( 600 );
		SetTimerOnOff( true );
	}
	else
	{
		DeleteDialog();
	}
}

void NConstructDlg::NConstructStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NConstructDlg *pThis = (NConstructDlg*) pControllerMgr;
	pThis->NConstructProc( nEvent, nControlID, pControl );
}

void NConstructDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
}

void NConstructDlg::NConstructProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NCITYHALLCONSTRUCT_BUTTON_SELECT1:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT2:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT3:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT4:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT5:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT6:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT7:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT8:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT9:
	case NCITYHALLCONSTRUCT_BUTTON_SELECT10:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				for ( SI32 i = 0; i < MAX_CONSTRUCTION_NUM ; ++i )
				{
					m_pButton_SelectPostion[i]->SetFontIndex( 0 );
				}


				m_currentIndex = nControlID - NCITYHALLCONSTRUCT_BUTTON_SELECT1;
				m_pButton_SelectPostion[m_currentIndex]->SetFontIndex(1);

				cltVillageStructureInfo *pStructureInfo;

				SI32 ref = pclClient->pclVillageStructureSet->FindRefFromUnique( m_siStructureUnique[m_currentIndex] );
				
				m_currentUnique = m_siStructureUnique[ m_currentIndex ];

				if ( m_currentUnique )
				{
					m_pButton_construction->Enable( false );
				}
				else
				{
					m_pButton_construction->Enable( true );
				};

				if( ref < 0 )
				{
					TCHAR* pText = GetTxtFromMgr( 3961 );
					m_pButton_destruction->SetText( pText );
					m_pButton_destruction->Enable( false );
					break;
				}

				pStructureInfo = pclClient->pclVillageStructureSet->pclVillageStructureInfo[ ref ];

				if ( pStructureInfo )
				{
					if ( pStructureInfo->bDestroyable )
					{
						TCHAR* pText = GetTxtFromMgr( 3961 );
						m_pButton_destruction->SetText( pText );
						m_pButton_destruction->Enable( true );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr( 10044 );
						m_pButton_destruction->SetText( pText );
						m_pButton_destruction->Enable( false );
					}
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr( 3961 );
					m_pButton_destruction->SetText( pText );
					m_pButton_destruction->Enable( false );
				}
 
			}
		}
		break;

	case NCITYHALLCONSTRUCT_LISTBOX_ABLE_STRUCTURE:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					TCHAR buf[ 256 ];

					stListBoxItemData *pListBoxItemData = NULL;
					SI16 siRow = -1, siCol = -1;

					m_pList_able_structure->GetSelectedIndex( &siRow, &siCol );
					if( siRow == -1 || siCol == -1 ) return;

					pListBoxItemData = m_pList_able_structure->GetItemData( siRow, 0 );
					if(pListBoxItemData == NULL) return;

					TCHAR* pTmpChar = NULL;
					pTmpChar = m_pList_able_structure->GetText( siRow, 0 );
					if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 32 );
					//m_pEdit_construction->SetText( buf );

					m_buildUnique = pListBoxItemData->siParam[0];
				}
				break;
			}
		}
		break;

	case NCITYHALLCONSTRUCT_BUTTON_CONSTRUCTION:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[256 ];

					TCHAR* pText = GetTxtFromMgr(1622);
					StringCchPrintf( buffer, 256, pText );

					pText = GetTxtFromMgr(1623);

					stMsgBox MsgBox;
					MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

				}
				break;
			}
		}
		break;

	case NCITYHALLCONSTRUCT_BUTTON_DESTRUCTION:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[256 ];
					TCHAR* pText = GetTxtFromMgr(1627);
		            
					StringCchPrintf( buffer, 256, pText );

					pText = GetTxtFromMgr(1628);

					stMsgBox MsgBox;
					MsgBox.Set( this, pText, buffer, MSGBOX_TYPE_YESNO, 1 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );
				}
				break;
			}
		}
		break;
	}
}

void NConstructDlg::SetExplain( TCHAR *szText )
{
	if(m_pEdit_explain == NULL ) return;

	m_pEdit_explain->SetText( TEXT("") );

//#if defined(_CITYHALL_BANKRUPTCY)
	{
		cltClient *pclclient = (cltClient*)pclClient;

		UI08 usCurStatus = 0;
		
		if( pclclient == NULL || pclclient->pclCityHallManager == NULL ) return;

		usCurStatus = pclclient->pclCityHallManager->clClientStrInfo.clCityhallStrInfo.usCurStatus;

		TCHAR buffer[500];

		memset( buffer, 0, sizeof(buffer));

		switch( usCurStatus )
		{
		case CITYHALL_BEING_STABLE	:
			{
				TCHAR* pText1 = GetTxtFromMgr(5168);
				TCHAR* pText2 = GetTxtFromMgr(5169);
				if( _tcslen( szText ) < 1 )
					StringCchPrintf( buffer, 500, TEXT("%s\r\n%s"), pText1, pText2 );
				else
					StringCchPrintf( buffer, 500, TEXT("%s\r\n\r\n%s\r\n%s"), szText, pText1, pText2 );

				m_pEdit_explain->SetText( buffer );
			}
			break;
		default:
			{
//#endif
				m_pEdit_explain->SetText( szText );                
//#if defined(_CITYHALL_BANKRUPTCY)
			}
			break;
		}
	}
//endif

	//m_pEdit_explain->SetText( szText );

	m_dwLastTick = GetTickCount();
}

void NConstructDlg::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
	SetActionTimer( 600 );
	SetTimerOnOff( true );
}

void NConstructDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
	SetTimerOnOff( false );
}

bool NConstructDlg::IsShow()
{
	return m_bShow;
}


void NConstructDlg::Action()
{
	int i;

	if( GetTickCount() - m_dwLastTick > 6000 ) {

		SetExplain( TEXT("") );
		//m_pEdit_explain->SetText( TEXT("") );
	}


	if( m_bUpdateSwitch ) 
	{
		m_bUpdateSwitch = FALSE;	

		int ref;

		SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;
		SI32 siStructureUnique;

		cltVillageInfo *pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ siVillageUnique ];
		cltVillageStructureInfo *pStructureInfo;

		if( pVillageInfo ) 
		{
			// 현재 마을 목록
			for( i = 0; i < MAX_CONSTRUCTION_NUM; ++i ) 
			{
				siStructureUnique = pVillageInfo->clVillageVary.clStructureInfo.Get( i );
				if( siStructureUnique ) 
				{
					ref = pclClient->pclVillageStructureSet->FindRefFromUnique( siStructureUnique );
					if( ref != -1 ) 
					{
						pStructureInfo = pclClient->pclVillageStructureSet->pclVillageStructureInfo[ ref ];
						if( pStructureInfo ) 
						{
							SI32 imgfontindex = 0;

							switch (pStructureInfo->siUnique)
							{
							case 1:		imgfontindex =  0; break;	// 관청
							case 10:	imgfontindex =  4; break;	// 은행
							case 20:	imgfontindex =  3; break;	// 증권소
							case 40:	imgfontindex =  1; break;	// 마켓
							case 50:	imgfontindex =  2; break;	// 저장고
							case 60:	imgfontindex = 10; break;	// 수렵장
							case 70:	imgfontindex =  7; break;	// 우체국
							case 90:	imgfontindex = 12; break;	// 사복시
							case 100:	imgfontindex =  6; break;	// 부동산
							case 130:	imgfontindex =  8; break;	// 농장
							case 140:	imgfontindex = 11; break;	// 내자시
							case 160:	imgfontindex =  9; break;	// 광산
							case 170:	imgfontindex =  5; break;	// 길드상단
							case 110:	imgfontindex = 13; break;	// 길드상단
							case 180:	imgfontindex =  1; break;	// [진성] 통합마켓
							default: 
								{
									continue;
								}
							}
							
							m_pButton_SelectPostion[i]->SetInImageFileName(TEXT("ninterface/Image/IMAGE_46X43_00_000.SPR"));
							m_pButton_SelectPostion[i]->SetInImageFontIndex( imgfontindex );
							m_pButton_SelectPostion[i]->SetInImagePos( 0, 0 );

							m_siStructureUnique[i] = pStructureInfo->siUnique;
						}
					}
				}
			}
		}
	}
}

void NConstructDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	TCHAR* pText = NULL;

	switch( MsgIndex )
	{
	case 0:	// 건설 창
		{
			if ( RetValue )
			{
				SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

				// 
				if( m_buildUnique == 0 ) {
					pText = GetTxtFromMgr(1624);
					SetExplain( pText );
					return ; 
				}

				// 이미 건설된 건물인가 
				if( pclClient->pclVillageManager->pclVillageInfo[ siVillageUnique ]->clVillageVary.clStructureInfo.IsDuplicate( m_buildUnique ) == TRUE ) {
					pText = GetTxtFromMgr(1625);
					SetExplain( pText );
					return ;
				}
				
				// 빈 슬롯인가
				if( m_currentUnique ) {
					pText = GetTxtFromMgr(1626);
					SetExplain( pText );
					return ;
				}

				// [진성] 소환수 거래소. => 2008-7-15
				if( SUMMONMARKET_UNIQUE == m_buildUnique ) 
				{
					cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pclStructureManager);
					SI32 residentnum = pCityhallManager->clClientStrInfo.clCityhallStrInfo.siResidentNum;//주민수
				}
				
				// [진성] 통합마켓.
				if( NEWMARKET_UNIQUE == m_buildUnique ) 
				{
					if( 20 < pclClient->pclVillageManager->GetVillageRank(siVillageUnique) )
					{
						pText = GetTxtFromMgr(9844);
						SetExplain( pText );
						return;
					}
				}
								

				cltGameMsgRequest_BuildStructure msg( m_currentIndex, m_buildUnique );

				cltMsg clMsg( GAMEMSG_REQUEST_BUILDSTRUCTURE, sizeof( cltGameMsgRequest_BuildStructure ), (BYTE *)&msg );

				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );			

				DeleteDialog();
			}
		}
		break;
	case 1:	// 해체 창
		{
			if( RetValue )
			{
				// 
				if( m_currentUnique == 0 ) 
				{
					pText = GetTxtFromMgr(1629);
					SetExplain(  pText );
					return ;
				}

				// 해체 가능한 건물인가
				SI32 ref = pclClient->pclVillageStructureSet->FindRefFromUnique( m_currentUnique );
				if( ref == -1 ) {
					pText = GetTxtFromMgr(1629);
					SetExplain(  pText);
					return ;
				}

				if( pclClient->pclVillageStructureSet->pclVillageStructureInfo[ ref ]->bDestroyable == FALSE ) {
					pText = GetTxtFromMgr(1630);
					SetExplain(  pText );
					return ;
				}


				cltGameMsgRequest_DestroyStructure msg( m_currentIndex );
				
				cltMsg clMsg( GAMEMSG_REQUEST_DESTROYSTRUCTURE, sizeof( cltGameMsgRequest_DestroyStructure ), (BYTE *)&msg );

				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

				DeleteDialog();
			}
		}
		break;
	}
}

// Structure Manager Setting!
void NConstructDlg::SetStructureManager( cltStructureMngBasic *pclStructureManager )
{
	m_pclStructureManager = pclStructureManager;

	if( m_pclStructureManager != NULL )
	{
		// 건물 리스트 요청 
		SI32 CharUnique = m_pclStructureManager->CharUnique;
		SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;

		SI32 id = 1;
		if(id)
		{
			cltMsg clMsg( GAMEMSG_REQUEST_GETSTRUCTURELIST, siVillageUnique );
			
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
		}

		m_bUpdateSwitch = TRUE;
	}
}

void NConstructDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	SI32 siVillageUnique = m_pclStructureManager->siVillageUnique;
	SI32 siStructureUnique;

	cltVillageInfo			*pVillageInfo	= pclClient->pclVillageManager->pclVillageInfo[ siVillageUnique ];
	cltVillageStructureInfo *pStructureInfo = NULL;

	if( pVillageInfo ) 
	{
		SI32	ref				= 0;
		SI32	imgfontindex	= 0;
		TCHAR*	text			= NULL;
		SI32	x				= 0;
		SI32	y				= 0;
		SI32	X_pos_minus		= 0;		// 글자 수에 따라 x좌표를 왼쪽으로 이동하기 위해.(가운데 맞추기가...ㄷㄷ)

		// 현재 마을 목록
		for( SI32 i = 0; i < MAX_CONSTRUCTION_NUM; ++i ) 
		{
			siStructureUnique = pVillageInfo->clVillageVary.clStructureInfo.Get( i );
			if( siStructureUnique ) 
			{
				ref = pclClient->pclVillageStructureSet->FindRefFromUnique( siStructureUnique );
				if( ref != -1 ) 
				{
					pStructureInfo = pclClient->pclVillageStructureSet->pclVillageStructureInfo[ ref ];
					if( pStructureInfo ) 
					{
						switch (siStructureUnique)
						{
							case  CITYHALL_UNIQUE:			imgfontindex =  0; //관청
															text = GetTxtFromMgr(2830);
															break;	
							case  BANK_UNIQUE:				imgfontindex =  4; //은행
															text = GetTxtFromMgr(1437);
															break;	
							case  STOCK_UNIQUE:				imgfontindex =  3;	//증권소
															text = GetTxtFromMgr(8600);
															break;	
							case  MARKET_UNIQUE:			imgfontindex =  1; 	//마켓
															text = GetTxtFromMgr(1664);
															break;	
							case  HOUSE_UNIQUE:				imgfontindex =  2; 	//저장고
															text = GetTxtFromMgr(370);
															break;	
							case  HUNT_UNIQUE:				imgfontindex = 10; 	//수렵장
															text = GetTxtFromMgr(8601);
															break;	
							case  POSTOFFICE_UNIQUE:		imgfontindex =  7; 	//우체국
															text = GetTxtFromMgr(8602);
															break;	
							case  HORSEMARKET_UNIQUE:		imgfontindex = 12; 	//사복시(탈것 거래소)
															text = GetTxtFromMgr(8603);
															break;	
							case  REALESTATEMARKET_UNIQUE:	imgfontindex =  6; 	//부동산  
															text = GetTxtFromMgr(8604);
															break;	
							case  SUMMONMARKET_UNIQUE:		imgfontindex = 13; 	//[진성] 소환수 거래소 이미지 13번으로... => 2008-7-16
															text = GetTxtFromMgr(8605);
															break;	
							case  LAND_UNIQUE:				imgfontindex =  8; 	//농장
															text = GetTxtFromMgr(1826);
															break;	
							case  FEAST_UNIQUE:				imgfontindex = 11; 	//내자시
															text = GetTxtFromMgr(1836);
															break;	
							//case SUMMONHEROMARKET_UNIQUE:	imgfontindex = 52; break;	//비변사
															//text = GetTxtFromMgr(2830);
															//break;	
							case MINE_UNIQUE:				imgfontindex =  9; 	//광산
															text = GetTxtFromMgr(2832);
															break;	
							case GUILD_UNIQUE:				imgfontindex =  5; 	//길드상단
															text = GetTxtFromMgr(5360);
															break;	
							case NEWMARKET_UNIQUE:			imgfontindex =  1; 	// [진성] 통합마켓.
															text = GetTxtFromMgr(9832);	
															break;	
							default: 
								{
									continue;
								}
						}

						HDC hdc;
						if(lpBackScreen->GetDC(&hdc) == DD_OK)
						{
							// 폰트를 설정한다. 
							HFONT hFont;
							if ( pclClient->siServiceArea == ConstServiceArea_English 
								|| pclClient->siServiceArea == ConstServiceArea_NHNChina
								|| pclClient->siServiceArea == ConstServiceArea_EUROPE
							)
							{
								hFont = g_FontManager.SetFont( 11, TEXT("Tahoma") );
							}
							else
							{
								hFont = g_FontManager.SetFont(11);
							}      

							HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

							SetBkMode(hdc, TRANSPARENT);

							x = m_pButton_SelectPostion[i]->GetX();
							y = m_pButton_SelectPostion[i]->GetY();

							X_pos_minus  = lstrlen(text)/2;
  
							SetTextColor( hdc, RGB(0, 0, 0));
							TextOut(hdc, ScreenRX + GetX()+x+25 - pow(X_pos_minus, 2), ScreenRY + GetY()+y+30, text, lstrlen(text));
   
							SetTextColor( hdc, RGB(255, 255, 255));
							TextOut(hdc, ScreenRX + GetX()+x+26 - pow(X_pos_minus, 2), ScreenRY + GetY()+y+31, text, lstrlen(text));

							SelectObject( hdc, hOldFont );  
   
							lpBackScreen->ReleaseDC(hdc);  
						}
					}
				}
			}
		}
	}
}

//#endif
