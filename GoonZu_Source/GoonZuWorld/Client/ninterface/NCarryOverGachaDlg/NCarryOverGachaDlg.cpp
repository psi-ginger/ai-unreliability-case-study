
#include "NCarryOverGachaDlg.h"
#include "NCarryOver_ProductDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "../../Client/Music/music.h"

#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../Client/Music/music.h"

#include "../../lib/MD5/md5.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

#include "Msg/msgType-Gacha.h"


extern cltCommonLogic* pclClient;

CNCarryOverGachaDlg::CNCarryOverGachaDlg( void )
{
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_22X27_00_000.SPR"),	&m_sprNumber ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_32X366_00_000.SPR"),	&m_sprGage ); 

	m_siSelectGachaProductIndex		= -1;
	m_siSelectGachaProductUnique	= 0;

	m_siKeyControllerID				= 0;
	m_siKeyKind						= 0;

	m_siGachaGage					= 0;

	m_pText							= NULL;

	m_siSelectMode					= CARRYOVER_SELECT_MODE_GACHA;

	ZeroMemory( m_siCarryOverItems, sizeof(m_siCarryOverItems) );

	ZeroMemory( m_siGachaProducts, sizeof(m_siGachaProducts) );

}

CNCarryOverGachaDlg::~CNCarryOverGachaDlg( void )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( pclclient )
	{
		pclclient->PlayBackgroundMusic( pclclient->pclCM->CR[1]->GetMapIndex() );

		if ( pclclient->m_pDialog[NGACHAPRODUCT_DLG] )
		{
			pclclient->m_pDialog[NGACHAPRODUCT_DLG]->DeleteDialog();
		}

		if ( pclclient->m_pDialog[NOPENBOX] )
		{
			pclclient->m_pDialog[NOPENBOX]->DeleteDialog();
		}

		if ( pclclient->m_pDialog[NGACHARESULT_DLG] )
		{
			pclclient->m_pDialog[NGACHARESULT_DLG]->DeleteDialog();
		}
	}

	if ( m_sprNumber.pImage )
	{
		GP.FreeSpr( m_sprNumber );
	}

	if ( m_sprGage.pImage )
	{
		GP.FreeSpr( m_sprGage );
	}

}

void CNCarryOverGachaDlg::Init( )
{
		
}

void CNCarryOverGachaDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NCarryOverGacha/DLG_NCarryOverGachaDlg.ddf" ) );
	file.CreatePopupDialog( this, NCARRYOVERGACHA_DLG, TEXT( "dialog_carryovergacha" ), StaticCallBackDialogNCarryOverGachaDlg );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM0,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM0,			TEXT("button_item0") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM1,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM1,			TEXT("button_item1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM2,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM2,			TEXT("button_item2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM3,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM3,			TEXT("button_item3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM4,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM4,			TEXT("button_item4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM5,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM5,			TEXT("button_item5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM6,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM6,			TEXT("button_item6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM7,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM7,			TEXT("button_item7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ITEM8,			this),	CARRYOVERGACHA_DLG_BUTTON_ITEM8,			TEXT("button_item8") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_CLOSE,			this),	CARRYOVERGACHA_DLG_BUTTON_CLOSE,			TEXT("button_close") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_GAMESTART,		this),	CARRYOVERGACHA_DLG_BUTTON_GAMESTART,		TEXT("button_gamestart") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_KEY_BUY,			this),	CARRYOVERGACHA_DLG_BUTTON_KEY_BUY,			TEXT("button_key_buy") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_GOLDKEY,			this),	CARRYOVERGACHA_DLG_BUTTON_GOLDKEY,			TEXT("button_goldkey") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_SILVERKEY,		this),	CARRYOVERGACHA_DLG_BUTTON_SILVERKEY,		TEXT("button_silverkey") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_BRONZEKEY,		this),	CARRYOVERGACHA_DLG_BUTTON_BRONZEKEY,		TEXT("button_bronzekey") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ALLROUNDGOLD,		this),	CARRYOVERGACHA_DLG_BUTTON_ALLROUNDGOLD,		TEXT("button_allroundgold") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ALLROUNDSILVER,	this),	CARRYOVERGACHA_DLG_BUTTON_ALLROUNDSILVER,	TEXT("button_allroundsilver") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,		CARRYOVERGACHA_DLG_BUTTON_ALLROUNDBRONZE,	this),	CARRYOVERGACHA_DLG_BUTTON_ALLROUNDBRONZE,	TEXT("button_allroundbronze") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_EXLAIN,			this),	CARRYOVERGACHA_DLG_EDITBOX_EXLAIN,			TEXT("editbox_exlain") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME0,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME0,	TEXT("editbox_give_item_name0") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME1,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME1,	TEXT("editbox_give_item_name1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME2,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME2,	TEXT("editbox_give_item_name2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME3,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME3,	TEXT("editbox_give_item_name3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME4,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME4,	TEXT("editbox_give_item_name4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME5,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME5,	TEXT("editbox_give_item_name5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME6,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME6,	TEXT("editbox_give_item_name6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME7,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME7,	TEXT("editbox_give_item_name7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME8,	this),	CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME8,	TEXT("editbox_give_item_name8") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_RAREITEM,		this),	CARRYOVERGACHA_DLG_EDITBOX_RAREITEM,		TEXT("editbox_rareitem") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_EXPLAIN2,		this),	CARRYOVERGACHA_DLG_EDITBOX_EXPLAIN2,		TEXT("editbox_explain2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		CARRYOVERGACHA_DLG_EDITBOX_TOPNOTICE,		this),	CARRYOVERGACHA_DLG_EDITBOX_TOPNOTICE,		TEXT("editbox_topnotice") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM0,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM0,		TEXT("imagestatic_item0") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM1,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM1,		TEXT("imagestatic_item1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM2,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM2,		TEXT("imagestatic_item2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM3,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM3,		TEXT("imagestatic_item3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM4,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM4,		TEXT("imagestatic_item4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM5,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM5,		TEXT("imagestatic_item5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM6,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM6,		TEXT("imagestatic_item6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM7,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM7,		TEXT("imagestatic_item7") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM8,		this),	CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM8,		TEXT("imagestatic_item8") );
	
	// 열쇠 버튼 활성화 설정
	ToggleKeyButtonEnable( false, false );

	// 게임시작버튼 비활성화
	SetEnableGameStartButton( false );

	// 다이얼로그 창 위치 관련 설정
	SetTopMostWindow( true );

	if( g_bFullScreen)	
	{
		MovePos( 80, 40 );
	}
	else		 		
	{
		MovePos( 45, 20 );
	}

	// 배경음악을 변경한다
	pclclient->pclMusic->pclSoundManager->BackGroundSoundStop();
	pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_TugOfWar.ogg"), true, false );

	// 텍스트 설정
	CEdit* pclExplain = m_InterfaceFactory.GetEdit( CARRYOVERGACHA_DLG_EDITBOX_EXLAIN );
	if ( pclExplain )
	{
		pclExplain->SetTextPrintOption( DT_VCENTER | DT_CENTER );
		pclExplain->SetText( GetTxtFromMgr(9955) );
		pclExplain->Refresh();
	}

	CEdit* pclRareItem = m_InterfaceFactory.GetEdit( CARRYOVERGACHA_DLG_EDITBOX_RAREITEM );
	if ( pclRareItem )
	{
		pclRareItem->SetTextPrintOption( DT_VCENTER | DT_CENTER );
		pclRareItem->SetFontSize( 13 );
		pclRareItem->SetFontWeight( FW_BOLD );
		pclRareItem->SetText( GetTxtFromMgr(7025), RGB(255,255,255) );
	}

	CEdit* pclTopNotice = m_InterfaceFactory.GetEdit( CARRYOVERGACHA_DLG_EDITBOX_TOPNOTICE );
	if ( pclTopNotice )
	{
		pclTopNotice->SetChangeTextDrawMode( true );
		pclTopNotice->SetFontSize( 16 );
		pclTopNotice->SetText( GetTxtFromMgr(7077), RGB(255,255,255) );
	}

	SetExplain2( GetTxtFromMgr(7022) );

	// 텍스트 옵션 설정
	for ( SI32 siIndex=0; siIndex<TOTAL_GACHA_PRODUCT; siIndex++ )
	{
		CEdit* pclItem = m_InterfaceFactory.GetEdit( CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME0 + siIndex );
		if ( pclItem )
		{
			pclItem->SetTextPrintOption( DT_CENTER );
			pclItem->Refresh();
		}
	}

	// 이달/이월 가챠 상품 리스트 요청
	cltMsg clMsg( GAMEMSG_REQUEST_CARRYOVER_GACHA_LIST, 0 , NULL );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

}

void CALLBACK CNCarryOverGachaDlg::StaticCallBackDialogNCarryOverGachaDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNCarryOverGachaDlg* pThis = (CNCarryOverGachaDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNCarryOverGachaDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNCarryOverGachaDlg::CallBackDialogNCarryOverGachaDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{

	switch(nControlID)
	{
	case CARRYOVERGACHA_DLG_BUTTON_CLOSE:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:	
				{
					DeleteDialog(); 
				}
				break;
			}
		}
		break;

	case CARRYOVERGACHA_DLG_BUTTON_ITEM0:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM1:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM2:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM3:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM4:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM5:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM6:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM7:
	case CARRYOVERGACHA_DLG_BUTTON_ITEM8:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:	
				{
					OnClickGachaProduct( nControlID );
				}
				break;
			}
		}
		break;

	case CARRYOVERGACHA_DLG_BUTTON_GOLDKEY:
	case CARRYOVERGACHA_DLG_BUTTON_SILVERKEY:
	case CARRYOVERGACHA_DLG_BUTTON_BRONZEKEY:
	case CARRYOVERGACHA_DLG_BUTTON_ALLROUNDGOLD:
	case CARRYOVERGACHA_DLG_BUTTON_ALLROUNDSILVER:
	case CARRYOVERGACHA_DLG_BUTTON_ALLROUNDBRONZE:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:	
				{
					OnClickKey( nControlID );
				}
				break;
			}
		}
		break;

	case CARRYOVERGACHA_DLG_BUTTON_GAMESTART:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:	
				{
					OnClickGameStart();
				}
				break;
			}
		}
		break;

	case CARRYOVERGACHA_DLG_BUTTON_KEY_BUY:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:	
				{
					OnClickKeyBuy();
				}
				break;
			}
		}
		break;
	}
}

void CNCarryOverGachaDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( !IsShow() )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// ====================================================================================
	//									대박확률 그리기 관련
	// ====================================================================================
	SI32 siGachaGageImageNum	= m_sprGage.GetImageNum();
	SI32 siGachaGageRate		= min( MAX_GACHA_BONUS_VALUE, m_siGachaGage ) * siGachaGageImageNum / MAX_GACHA_BONUS_VALUE;

	if ( 0 < siGachaGageRate )
	{
		SI32 siGageX		= 249 + GetX() + ScreenRX;

		SI32 siGageYSize	= m_sprGage.GetYSize();
		SI32 siGageY		= 237 + GetY() + ScreenRY + (siGachaGageImageNum-1) * siGageYSize;
		
		SI32 siFontIndex	= 0;

		for ( SI32 siIndex=0; siIndex<siGachaGageRate; siIndex++ )
		{
			siFontIndex = siGachaGageImageNum - siIndex - 1;

			GP.PutSpr( &m_sprGage, siGageX, siGageY - (siGageYSize*siIndex), siFontIndex );
		}
	}

	// ====================================================================================
	//									가챠 키 그리기 관련
	// ====================================================================================

	// 보물상자 가챠키 갯수
	SI32 siGoldKeyNum		= pclchar->pclCI->clCharItem.GetItemNum( GACHA_GOLDKEY_ITEMUNIQUE );
	SI32 siSilverKeyNum		= pclchar->pclCI->clCharItem.GetItemNum( GACHA_SILVERKEY_ITEMUNIQUE );
	SI32 siBronzeKeyNum		= pclchar->pclCI->clCharItem.GetItemNum( GACHA_BRONZEKEY_ITEMUNIQUE );

	if ( 1000 <= siGoldKeyNum )		siGoldKeyNum		= 999;
	if ( 1000 <= siSilverKeyNum )	siSilverKeyNum		= 999;
	if ( 1000 <= siBronzeKeyNum )	siBronzeKeyNum		= 999;

	// 만능 가챠키 갯수
	SI32 siARGoldKeyNum		= pclchar->pclCI->clCharItem.GetItemNum( CARRYOVER_GOLDKEY_ITEMUNIQUE );
	SI32 siARSilverKeyNum	= pclchar->pclCI->clCharItem.GetItemNum( CARRYOVER_SILVERKEY_ITEMUNIQUE );
	SI32 siARBronzeKeyNum	= pclchar->pclCI->clCharItem.GetItemNum( CARRYOVER_BRONZEKEY_ITEMUNIQUE );

	if ( 1000 <= siARGoldKeyNum )	siARGoldKeyNum		= 999;
	if ( 1000 <= siARSilverKeyNum )	siARSilverKeyNum	= 999;
	if ( 1000 <= siARBronzeKeyNum )	siARBronzeKeyNum	= 999;

	SI32 siKeyDrawX = 148 + GetX() + ScreenRX;
	SI32 siKeyDrawY = 246 + GetY() + ScreenRY;

	// 열쇠 갯수 그림
	GP.DrawNumber( &m_sprNumber, siKeyDrawX, siKeyDrawY, 0, siGoldKeyNum,			3 );	siKeyDrawY += 43;
	GP.DrawNumber( &m_sprNumber, siKeyDrawX, siKeyDrawY, 20, siSilverKeyNum,		3 );	siKeyDrawY += 41;
	GP.DrawNumber( &m_sprNumber, siKeyDrawX, siKeyDrawY, 30, siBronzeKeyNum,		3 );

	siKeyDrawY = 426 + GetY() + ScreenRY;
	GP.DrawNumber( &m_sprNumber, siKeyDrawX, siKeyDrawY, 0, siARGoldKeyNum,			3 );	siKeyDrawY += 43;
	GP.DrawNumber( &m_sprNumber, siKeyDrawX, siKeyDrawY, 20, siARSilverKeyNum,		3 );	siKeyDrawY += 41;
	GP.DrawNumber( &m_sprNumber, siKeyDrawX, siKeyDrawY, 30, siARBronzeKeyNum,		3 );

	// 가차 선택 모드일때만 작동한다
	if ( CARRYOVER_SELECT_MODE_GACHA == m_siSelectMode )
	{
		// ====================================================================================
		//							선택된 상품 테두리 그리기 관련
		// ====================================================================================
		if ( (0 <= m_siSelectGachaProductIndex) && (TOTAL_GACHA_PRODUCT > m_siSelectGachaProductIndex) )
		{
			SI32			siControllerID	= CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM0 + m_siSelectGachaProductIndex;
			CImageStatic*	pclImage		= m_InterfaceFactory.GetImageStatic( siControllerID );

			if ( pclImage )
			{
				SI32 siX1 = pclImage->GetX() + GetX() + ScreenRX;
				SI32 siY1 = pclImage->GetY() + GetY() + ScreenRY;
				SI32 siX2 = siX1 + pclImage->GetWidth();
				SI32 siY2 = siY1 + pclImage->GetHeight();

				GP.Box( siX1, siY1, siX2, siY2, 41 );
			}
		}

		// ====================================================================================
		//							선택된 키 테두리 그리기 관련
		// ====================================================================================
		if ( (CARRYOVERGACHA_DLG_BUTTON_GOLDKEY <= m_siKeyControllerID) && (CARRYOVERGACHA_DLG_BUTTON_ALLROUNDBRONZE >= m_siKeyControllerID) )
		{
			CButton* pclButton = m_InterfaceFactory.GetButton( m_siKeyControllerID );
			if ( pclButton )
			{
				SI32 siX1 = pclButton->GetX() + GetX() + ScreenRX + 5;
				SI32 siY1 = pclButton->GetY() + GetY() + ScreenRY + 5;
				SI32 siX2 = siX1 + pclButton->GetWidth() - 12;
				SI32 siY2 = siY1 + pclButton->GetHeight() - 12;

				GP.Box( siX1, siY1, siX2, siY2, 41 );
			}
		}
	}

	// ====================================================================================
	//							설명 텍스트 그리기 관련
	// ====================================================================================
	if ( m_pText )
	{
		Text( ScreenRX, ScreenRY );
	}
}

void CNCarryOverGachaDlg::OnClickGachaProduct( const SI32 siControllerID )
{
	if ( CARRYOVER_SELECT_MODE_GACHA != m_siSelectMode )
	{
		return;
	}

	if ( (CARRYOVERGACHA_DLG_BUTTON_ITEM0 > siControllerID) || (CARRYOVERGACHA_DLG_BUTTON_ITEM8 < siControllerID) )
	{
		return;
	}

	SI32 siRealIndex = siControllerID - CARRYOVERGACHA_DLG_BUTTON_ITEM0;
	if ( (0 > siRealIndex) || (TOTAL_GACHA_PRODUCT <= siRealIndex) )
	{
		return;
	}

	// 같은 인덱스라면 처리할 필요없음
	if ( siRealIndex == m_siSelectGachaProductIndex )
	{
		return;
	}

	// 새로운 상품이 선택되었으므로 키관련 변수는 초기화
	m_siKeyControllerID	= 0;
	m_siKeyKind			= 0;

	// 선택된 인덱스로 설정함
	m_siSelectGachaProductIndex = siRealIndex;

	// 이곳에 아이템이 실제 있는지 검사
	m_siSelectGachaProductUnique = m_siCarryOverItems[siRealIndex];
	if ( 0 >= m_siSelectGachaProductUnique )
	{
		m_siSelectGachaProductIndex = -1;

		ToggleKeyButtonEnable( false, false );

		return;
	}
	
	// 이달의 가챠 상품일 경우
	if ( 0 == m_siSelectGachaProductIndex )
	{
		ToggleKeyButtonEnable( true, false );
	}
	// 이월 가챠 상품일 경우
	else
	{
		ToggleKeyButtonEnable( false, true );
	}

	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);

}

void CNCarryOverGachaDlg::OnClickKey( const SI32 siControllerID )
{
	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	SI32 siKeyUnique	= 0;
	SI32 siKeyKind		= 0;

	switch ( siControllerID )
	{
	case CARRYOVERGACHA_DLG_BUTTON_GOLDKEY:			
		{	
			siKeyUnique = GACHA_GOLDKEY_ITEMUNIQUE;			
			siKeyKind	= GACHA_USE_GOLDKEY;
		}	
		break;
	case CARRYOVERGACHA_DLG_BUTTON_SILVERKEY:		
		{	
			siKeyUnique = GACHA_SILVERKEY_ITEMUNIQUE;		
			siKeyKind	= GACHA_USE_SILVERKEY;
		}	
		break;
	case CARRYOVERGACHA_DLG_BUTTON_BRONZEKEY:		
		{	
			siKeyUnique = GACHA_BRONZEKEY_ITEMUNIQUE;	
			siKeyKind	= GACHA_USE_BRONZEKEY;
		}	
		break;
	case CARRYOVERGACHA_DLG_BUTTON_ALLROUNDGOLD:	
		{	
			siKeyUnique = CARRYOVER_GOLDKEY_ITEMUNIQUE;		
			siKeyKind	= CARRYOVER_USE_GOLDKEY;
		}	
		break;
	case CARRYOVERGACHA_DLG_BUTTON_ALLROUNDSILVER:	
		{	
			siKeyUnique = CARRYOVER_SILVERKEY_ITEMUNIQUE;	
			siKeyKind	= CARRYOVER_USE_SILVERKEY;
		}	
		break;
	case CARRYOVERGACHA_DLG_BUTTON_ALLROUNDBRONZE:	
		{	
			siKeyUnique = CARRYOVER_BRONZEKEY_ITEMUNIQUE;	
			siKeyKind	= CARRYOVER_USE_BRONZEKEY;
		}	
		break;
	default:
		{
			return;
		}
		break;
	}

	bool bHaveKey = false;

	// 가챠 동열쇠는 이벤트 동열쇠도 같이 체크한다
	if ( CARRYOVERGACHA_DLG_BUTTON_BRONZEKEY == siControllerID )
	{
		if ( 0 < pclchar->pclCI->clCharItem.GetItemNum(GACHA_BRONZEKEY_EVENT_ITEMUNIQUE) )
		{
			bHaveKey = true;
		}
	}

	// 현재 선택한 가챠 키가 없으면 나간다
	if ( (0 >= pclchar->pclCI->clCharItem.GetItemNum(siKeyUnique)) && (false == bHaveKey) )
	{
		SetEnableGameStartButton( false );
		
		m_siKeyControllerID	= 0;
		m_siKeyKind			= 0;

		return;
	}

	m_siKeyControllerID	= siControllerID;
	m_siKeyKind			= siKeyKind;

	SetEnableGameStartButton( true );

	pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);

}

void CNCarryOverGachaDlg::OnClickKeyBuy( void )
{
	// 아이템몰 가기 버튼 작동

	DeleteDialog(); // 우선 나는 없애버린다

	// 웹 아이템몰이 열려야 하는경우
	if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
	{
		if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
		{
			cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
			if ( NULL == pclchar )
			{
				return;
			}

			UI08	md5buf[256]		= "" ;
			char	buffermd5[256]	= "" ;

			TCHAR	szURL[1024];
			TCHAR	szBeforeMD5[256];

			sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");

			memset(md5buf, 0, sizeof(md5buf));
			// MD5
			SI32 md5size = _tcslen(szBeforeMD5);
			memcpy(md5buf, szBeforeMD5, md5size);
			md5_string(md5buf, buffermd5, md5size);

			sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
				100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);

			g_LoginNoticeDlg.Set(szURL, 970, 756);
		}
	}
	// 아이템몰 다이얼로그가 열려야 하는경우
	else
	{
		((cltClient*)pclClient)->CreateInterface(NNEWITEMMALL_DLG);	
	}
}

void CNCarryOverGachaDlg::OnClickGameStart( void )
{
	if ( (0 > m_siSelectGachaProductIndex) || (0 >= m_siSelectGachaProductUnique) )
	{
		return;
	}

	if ( (0 >= m_siKeyControllerID) || (0 >= m_siKeyKind) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	m_siSelectMode = CARRYOVER_SELECT_MODE_PRODUCT;

	HideItemControllers();

	pclclient->CreateInterface( NGACHAPRODUCT_DLG );

	CButton* pclButton = m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_GAMESTART );
	if ( pclButton )
	{
		ClearFocus();
	}

	// 서버로 리스트 요청
	cltGameMsgRequest_CarryOver_SelectProduct_List clInfo( m_siSelectGachaProductIndex, m_siSelectGachaProductUnique, m_siKeyKind );
	cltMsg clMsg( GAMEMSG_REQUEST_CARRYOVER_SELECTPRODUCT_LIST, sizeof(clInfo), (BYTE*)&clInfo );
	pclclient->SendMsgToServer( (sPacketHeader*)&clMsg );


}

void CNCarryOverGachaDlg::ToggleKeyButtonEnable( const bool bGachaKeyEnable, const bool bAllRoundKeyEnable )
{
	CButton* pclGoldKey		= m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_GOLDKEY );
	CButton* pclSilverKey	= m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_SILVERKEY );
	CButton* pclBronzeKey	= m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_BRONZEKEY );

	if ( (pclGoldKey) && (pclSilverKey) && (pclBronzeKey) )
	{
		pclGoldKey->Enable( bGachaKeyEnable );
		pclSilverKey->Enable( bGachaKeyEnable );
		pclBronzeKey->Enable( bGachaKeyEnable );
	}


	CButton* pclARGoldKey	= m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_ALLROUNDGOLD );
	CButton* pclARSilverKey	= m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_ALLROUNDSILVER );
	CButton* pclARBronzeKey	= m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_ALLROUNDBRONZE );

	if ( (pclARGoldKey) && (pclARSilverKey) && (pclARBronzeKey) )
	{
		pclARGoldKey->Enable( bAllRoundKeyEnable );
		pclARSilverKey->Enable( bAllRoundKeyEnable );
		pclARBronzeKey->Enable( bAllRoundKeyEnable );
	}

}

void CNCarryOverGachaDlg::Text( SI32 ScreenRX, SI32 ScreenRY )
{
	cltClient *pclclient = ( cltClient * )pclClient;

	HDC hdc;
	HFONT hFont;

	if(pclclient->lpBackScreen->GetDC(&hdc) != DD_OK) return;

	if( pclClient->IsCountrySwitch( Switch_NONATIALIASED_QUALITY ) )
	{
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, NONANTIALIASED_QUALITY, 0, TEXT("굴림") );
	}
	else 
	{
		hFont = CreateFont( 20, 0, 0, 0, FW_BOLD, 0, 0, 0, pclclient->GetCharSet(), 0, 0, 0, 0, TEXT("굴림") );
	}

	HFONT hOldFont = NULL;
	if ( hFont != NULL )
	{
		hOldFont = (HFONT)SelectObject( hdc, hFont );
	}	

	SetBkMode(hdc, TRANSPARENT);

	// 각 창에 관련된 간략한 설명을 그려준다.
	RECT rect = { 453 , 260 , 725 , 317 };

	OffsetRect( &rect , GetX()  + ScreenRX , GetY() + ScreenRY - 2 );

	RECT temprc;

	SetTextColor(hdc, RGB(255, 255, 255));

	// 외각선 그리기	
	CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  0);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  0);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0, -1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  0,  1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );

	CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1, -1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1, -1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc,  1,  1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );
	CopyRect(&temprc, &rect);	OffsetRect(&temprc, -1,  1);	DrawText( hdc, m_pText, lstrlen(m_pText), &temprc, DT_CENTER | DT_VCENTER );

	SetTextColor(hdc, RGB(50, 146, 255));
	DrawText( hdc, m_pText, lstrlen(m_pText), &rect, DT_CENTER | DT_VCENTER );


	if(hOldFont)
	{
		DeleteObject(SelectObject(hdc, hOldFont)) ;
	} 

	pclclient->lpBackScreen->ReleaseDC(hdc);
}

void CNCarryOverGachaDlg::SetExplain2( TCHAR* pText )
{
	if ( pText )
	{
		m_pText = pText;
	}
}

void CNCarryOverGachaDlg::SetTotalGachaListAndGachaCount( SI32* psiTotalGachaList, SI32 siGachaCount )
{
	if ( NULL == psiTotalGachaList )
	{
		return;
	}

	// 맴버 변수 셋팅
	memcpy( m_siCarryOverItems, psiTotalGachaList, sizeof(m_siCarryOverItems) );

	m_siGachaGage = siGachaCount;

	// 가챠 목록 보여주기
	ShowItemList( m_siCarryOverItems, TOTAL_GACHA_PRODUCT );
}

void CNCarryOverGachaDlg::SetGachaProducts( SI32* psiGachaProducts )
{
	if ( NULL == psiGachaProducts )
	{
		return;
	}

	cltClient* pclclent = (cltClient*)pclClient;
	if ( NULL == pclclent )
	{
		return;
	}

	// 맴버 변수 셋팅
	memcpy( m_siGachaProducts, psiGachaProducts, sizeof(m_siGachaProducts) );

	// 가차 설명 설정
	if ( (0 < m_siGachaProducts[0]) && (pclclent->m_pDialog[NGACHAPRODUCT_DLG]) )
	{
		((CNCarryOver_ProductDlg*)pclclent->m_pDialog[NGACHAPRODUCT_DLG])->SetInfo( m_siGachaProducts[0], m_siKeyKind, m_siSelectGachaProductIndex, m_siSelectGachaProductUnique );
	}

	// 가차 상품들 보여주기
	ShowItemList( m_siGachaProducts, MAX_GACHA_PRODUCT );
}

void CNCarryOverGachaDlg::ShowItemList( const SI32* const psiItemList, const SI32 siItemCount )
{
	if ( (NULL == psiItemList) || (0 >= siItemCount) )
	{
		return;
	}

	// 아이템 이미지&이름 설정
	SI32			siGImageIndex	= 0;
	SI32			siGImageFont	= 0;

	SI32			siGachaUnique	= 0;

	TCHAR*			pszItemName		= NULL;

	CImageStatic*	pclImage		= NULL;
	CEdit*			pclEdit			= NULL;


	for ( SI32 siIndex=0; siIndex<siItemCount; siIndex++ )
	{
		siGachaUnique = psiItemList[siIndex];
		if ( 0 >= siGachaUnique )
		{
			continue;
		}

		pszItemName = (TCHAR*)pclClient->pclItemManager->GetName( siGachaUnique );
		if ( NULL == pszItemName )
		{
			continue;
		}

		pclImage = m_InterfaceFactory.GetImageStatic( CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM0 + siIndex );
		if ( NULL == pclImage )
		{
			continue;
		}

		pclEdit = m_InterfaceFactory.GetEdit( CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME0 + siIndex );
		if ( NULL == pclEdit )
		{
			continue;
		}

		// 이달의 가챠 상품일 경우
		if ( 0 == siIndex )
		{
			TCHAR* pszFileName = NULL;

			cltItemManagerClient* pclItemManagerClient = (cltItemManagerClient*)pclClient->pclItemManager;
			if ( NULL == pclItemManagerClient )
			{
				continue;
			}

			if ( false == pclItemManagerClient->GetItemImageBigFileName(siGachaUnique, &pszFileName, &siGImageFont) )
			{
				continue;
			}	

			pclImage->SetFileName( pszFileName );
			pclImage->SetFontIndex( siGImageFont );

			pclEdit->SetText( pszItemName, RGB(255,255,255) );
		}
		// 이월 가챠 상품일 경우
		else
		{
			if( false == pclClient->pclItemManager->GetItemGImageFileIndexAndFont(siGachaUnique, &siGImageIndex, &siGImageFont) )
			{
				continue;
			}

			pclImage->SetItemSprType( siGImageIndex );
			pclImage->SetFontIndex( siGImageFont );

			pclEdit->SetText( pszItemName );
		}

		pclImage->Show( true );
		pclEdit->Show( true );
	}
}

void CNCarryOverGachaDlg::HideItemControllers( void )
{
	CImageStatic*	pclItemImage	= NULL;
	CEdit*			pclItemEdit		= NULL;

	for ( SI32 siIndex=0; siIndex<TOTAL_GACHA_PRODUCT; siIndex++ )
	{
		pclItemImage = m_InterfaceFactory.GetImageStatic( CARRYOVERGACHA_DLG_IMAGESTATIC_ITEM0 + siIndex );
		if ( pclItemImage )
		{
			pclItemImage->Show( false );
		}

		pclItemEdit = m_InterfaceFactory.GetEdit( CARRYOVERGACHA_DLG_EDITBOX_GIVE_ITEM_NAME0 + siIndex );
		if ( pclItemEdit )
		{
			pclItemEdit->Show( false );
		}
	}

}

void CNCarryOverGachaDlg::SetEnableGameStartButton( bool bEnable )
{
	CButton* pclButton = m_InterfaceFactory.GetButton( CARRYOVERGACHA_DLG_BUTTON_GAMESTART );
	if ( pclButton )
	{
		pclButton->Enable( bEnable );
	}
}

void CNCarryOverGachaDlg::OnEventBack( void )
{
	m_siSelectGachaProductIndex		= -1;
	m_siSelectGachaProductUnique	= 0;

	m_siKeyControllerID				= 0;
	m_siKeyKind						= 0;

	m_siSelectMode					= CARRYOVER_SELECT_MODE_GACHA;

	ToggleKeyButtonEnable( false, false );

	SetEnableGameStartButton( false );

	ShowItemList( m_siCarryOverItems, TOTAL_GACHA_PRODUCT );
}

bool CNCarryOverGachaDlg::IsHaveKey( void )
{
	cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return false;
	}

	SI32 siKeyUnique = 0;
	switch ( m_siKeyKind )
	{
	case GACHA_USE_GOLDKEY:			{	siKeyUnique = GACHA_GOLDKEY_ITEMUNIQUE;			}	break;
	case GACHA_USE_SILVERKEY:		{	siKeyUnique = GACHA_SILVERKEY_ITEMUNIQUE;		}	break;
	case GACHA_USE_BRONZEKEY:		{	siKeyUnique = GACHA_BRONZEKEY_ITEMUNIQUE;		}	break;
	case CARRYOVER_USE_GOLDKEY:		{	siKeyUnique = CARRYOVER_GOLDKEY_ITEMUNIQUE;		}	break;
	case CARRYOVER_USE_SILVERKEY:	{	siKeyUnique = CARRYOVER_SILVERKEY_ITEMUNIQUE;	}	break;
	case CARRYOVER_USE_BRONZEKEY:	{	siKeyUnique = CARRYOVER_BRONZEKEY_ITEMUNIQUE;	}	break;
	default:
		{
			return false;
		}
		break;
	}

	bool bHaveKey = false;

	// 가챠 동열쇠는 이벤트 동열쇠도 같이 체크한다
	if ( GACHA_USE_BRONZEKEY == m_siKeyKind )
	{
		if ( 0 < pclchar->pclCI->clCharItem.GetItemNum(GACHA_BRONZEKEY_EVENT_ITEMUNIQUE) )
		{
			bHaveKey = true;
		}
	}

	// 현재 선택한 가챠 키가 없으면 나간다
	if ( (0 >= pclchar->pclCI->clCharItem.GetItemNum(siKeyUnique)) && (false == bHaveKey) )
	{
		return false;
	}

	return true;
}