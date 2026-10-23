#include "NGachaDlgNew.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Msg/msgType-Gacha.h"
#include "../../../common/Char/CharManager/CharManager.h"
#include "../../../Client/Music/music.h"

#include "../../lib/MD5/md5.h"
#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

CNGachaDlgNew::CNGachaDlgNew()
{
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_32X366_00_000.SPR"),	&m_GachaGageImg ); 
	GP.LoadSprFromFile( TEXT("Ninterface/Image/Image_22X27_00_000.SPR"),	&m_NumberImg	); 
	m_uiGachaCnt = 0;
}

CNGachaDlgNew::~CNGachaDlgNew()
{
	cltClient* pclclient = (cltClient*)pclClient;
	pclclient->PlayBackgroundMusic(pclclient->pclCM->CR[1]->GetMapIndex());

	Destroy();
}


void CNGachaDlgNew::Destroy()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if( pclclient->m_pDialog[ NCHOISE_ITEM_GAME_START_DLG ] )
		pclclient->m_pDialog[ NCHOISE_ITEM_GAME_START_DLG ]->DeleteDialog();

	if( pclclient->m_pDialog[ NOPENBOX ] )
		pclclient->m_pDialog[ NOPENBOX ]->DeleteDialog();

	if( pclclient->m_pDialog[ RECIEVED_ITEM_DLG ] )
		pclclient->m_pDialog[ RECIEVED_ITEM_DLG ]->DeleteDialog();

	if( m_GachaGageImg.pImage) GP.FreeSpr( m_GachaGageImg );
	if( m_NumberImg.pImage)	   GP.FreeSpr( m_NumberImg	  );
}

void CNGachaDlgNew::Create()
{
	if( IsCreate() )
	{
		return;
	}

	SetTopMostWindow( true );

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NGachaDlg/DLG_NGachaDlgNew.ddf");
	file.CreatePopupDialog( this , NGACHANEW_DLG, "dialog_GachaNew" , StatiCNGachaDlgNewProc );

	if( g_bFullScreen)	MovePos( 80, 40 );
	else		 		MovePos( 45, 20 );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,	NGACHANEW_IMAGESTATIC_BEST_ITEM,		this), NGACHANEW_IMAGESTATIC_BEST_ITEM,		TEXT("imagestatic_bestitem"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	 	NGACHANEW_BUTTON_KEY_BUY,				this), NGACHANEW_BUTTON_KEY_BUY,			TEXT("button_key_buy"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	 	NGACHANEW_BUTTON_CLOSE,					this), NGACHANEW_BUTTON_CLOSE,				TEXT("button_close"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	 	NGACHANEW_BUTTON_GAMESTART_ITEMMALL,	this), NGACHANEW_BUTTON_GAMESTART_ITEMMALL,	TEXT("button_gamestart"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	 	NGACHANEW_EDIT_EXPLAIN2,				this), NGACHANEW_EDIT_EXPLAIN2,			TEXT("editbox_explain2"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	 	NGACHANEW_EDIT_EXPLAIN,				this), NGACHANEW_EDIT_EXPLAIN,			TEXT("editbox_exlain"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	 	NGACHANEW_EDIT_TOPNOTICE,				this), NGACHANEW_EDIT_TOPNOTICE,			TEXT("editbox_topnotice"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,		NGACHANEW_EDIT_RAREITEM,				this), NGACHANEW_EDIT_RAREITEM,			TEXT("editbox_rareitem"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,   		NGACHANEW_EDIT_RAREITEM_NAME,			this), NGACHANEW_EDIT_RAREITEM_NAME,		TEXT("editbox_rareitem_name"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW, 	NGACHANEW_LISTVIEW_RAREITEM,			this), NGACHANEW_LISTVIEW_RAREITEM,			TEXT("listview_rareitem"));
	m_InterfaceFactory.GetListView(NGACHANEW_LISTVIEW_RAREITEM)->Enable(false);


	TCHAR szControlName[128] = "";
	for(SI32 Count = 0; Count < MAX_GACHA_PRODUCT_ITEM; ++Count)
	{
		StringCchPrintf(szControlName, sizeof(szControlName),  "listView_give_item%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW, NGACHANEW_LISTVIEW_GIVEITEM1 + Count,	this), NGACHANEW_LISTVIEW_GIVEITEM1 + Count, szControlName);
		m_InterfaceFactory.GetListView(NGACHANEW_LISTVIEW_GIVEITEM1 + Count)->Enable(false);

		StringCchPrintf(szControlName, sizeof(szControlName),  "editbox_give_item_name%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT, NGACHANEW_EDIT_GIVEITEM1 + Count,	this), NGACHANEW_EDIT_GIVEITEM1  + Count, szControlName);
		m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_GIVEITEM1 + Count)->SetTextPrintOption(DT_CENTER);
		m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_GIVEITEM1 + Count)->Refresh();   
	}

	SetExplain2( GetTxtFromMgr(7022) ); 
	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN2)->SetFontSize( 25 );  
	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN2)->SetTextPrintOption(DT_CENTER | DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN2)->Refresh(); 

	//m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN)->SetTextPrintOption(DT_VCENTER);// | DT_CENTER | DT_SINGLELINE);
	//m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN)->Refresh();

	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_RAREITEM)->SetTextPrintOption(DT_VCENTER | DT_CENTER | DT_SINGLELINE);
	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_RAREITEM)->Refresh();  

	//m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_RAREITEM_NAME)->SetTextPrintOption(DT_CENTER);
	//m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_RAREITEM_NAME)->Refresh();

	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_TOPNOTICE)->SetChangeTextDrawMode( true );
	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_TOPNOTICE)->SetFontSize(16);
	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_TOPNOTICE)->SetText( GetTxtFromMgr(7077),    RGB(255,255,255) );

	//m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN)->SetText( GetTxtFromMgr(7017) );
	//m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN)->Refresh(); 

	m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_RAREITEM)->SetText( GetTxtFromMgr(7025) );



	cltClient * pclclient = (cltClient*) pclClient;
	cltMsg clMsg( GAMEMSG_REQUEST_GACHAPRODUCT_LIST, 0 , NULL );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);


	SetTopMostWindow( true );

	pclclient->pclMusic->pclSoundManager->BackGroundSoundStop();
	pclclient->pclMusic->pclSoundManager->PlayBackGroundFile( TEXT("./Music/Music_TugOfWar.ogg"), true, false );

}

void CALLBACK CNGachaDlgNew::StatiCNGachaDlgNewProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGachaDlgNew *pThis = (CNGachaDlgNew *)pControllerMgr;
	pThis->NGachaDlgNewProc( nEvent, nControlID, pControl );
}
void CALLBACK CNGachaDlgNew::NGachaDlgNewProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case NGACHANEW_BUTTON_CLOSE:
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
	case NGACHANEW_BUTTON_GAMESTART_ITEMMALL:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					//[진성] 인벤토리가 다 차있으면 가차 창을 닫아 버린다.
					cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
					if(false == pclchar->IsBlankInv(1, INVMODE_ALL) )
					{
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
						DeleteDialog();
					}

					if ( NULL == pclclient->m_pDialog[NCHOISE_ITEM_GAME_START_DLG] )
					{							
						pclclient->CreateInterface( NCHOISE_ITEM_GAME_START_DLG );
					}
					else
					{
						pclclient->DestroyInterface( pclclient->m_pDialog[NCHOISE_ITEM_GAME_START_DLG] );
					}

					m_InterfaceFactory.GetButton(NGACHANEW_BUTTON_GAMESTART_ITEMMALL)->SetFocus(false);
				}
				break;
			case EVENT_BUTTON_RCLICKED:
				{
					m_InterfaceFactory.GetButton(NGACHANEW_BUTTON_GAMESTART_ITEMMALL)->SetFocus(false);
				}
				break;
			}
		}
		break;
	case NGACHANEW_BUTTON_KEY_BUY:
		{
			DeleteDialog();
			if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
			{
				if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
				{
					cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

					UI08 md5buf[256] = "" ;
					char buffermd5[256] = "" ;

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
			else
			{
				((cltClient*)pclClient)->CreateInterface(NNEWITEMMALL_DLG);	
			}
		}
		break;
	default:
		{
		}
		break;
	}
}

void CNGachaDlgNew::Action()
{


}

void CNGachaDlgNew::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;

	//------------------------------------------------------------------------->
	// 게이지.
	//------------------------------------------------------------------------->
	SI32 GachaGageImgMaxNum = m_GachaGageImg.GetImageNum();
	SI32 GachaGageRate = 0;

	GachaGageRate = min(MAX_GACHA_BONUS_VALUE , m_uiGachaCnt) * GachaGageImgMaxNum / MAX_GACHA_BONUS_VALUE;

	if( GachaGageRate > 0 )
	{
		SI32 GageYSize =  m_GachaGageImg.GetYSize();

		for( SI16 i = 0 ; i < GachaGageRate ; i ++ )
		{
			SI32 imageFont = GachaGageImgMaxNum - i - 1;			
			GP.PutSpr( &m_GachaGageImg , 249 +GetX()  + ScreenRX , 237 + GageYSize * imageFont +GetY() + ScreenRY, imageFont );
		}
	}
	//-------------------------------------------------------------------------<

	//------------------------------------------------------------------------->
	// 가차 열쇠, 조각 갯수 그려줌
	//------------------------------------------------------------------------->
	SI32 m_myGachaGoldKeyNum	= pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_GOLDKEY_ITEMUNIQUE );
	SI32 m_myGachaSilverKeyNum	= pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_SILVERKEY_ITEMUNIQUE );
	SI32 m_myGachaBronzeKeyNum	= pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_BRONZEKEY_ITEMUNIQUE );
	SI32 m_myGachaPieceNum		= pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum( GACHA_KEYPIECE_ITEMUNIQUE );

	if(1000 <= m_myGachaGoldKeyNum)		m_myGachaGoldKeyNum		= 999;
	if(1000 <= m_myGachaSilverKeyNum)	m_myGachaSilverKeyNum	= 999;
	if(1000 <= m_myGachaBronzeKeyNum)	m_myGachaBronzeKeyNum	= 999;
	if(1000 <= m_myGachaPieceNum)		m_myGachaPieceNum		= 999;


	SI32 GoldKeyNum[3];
	GoldKeyNum[0] = m_myGachaGoldKeyNum  / 100;
	GoldKeyNum[1] = (m_myGachaGoldKeyNum % 100) / 10;
	GoldKeyNum[2] = m_myGachaGoldKeyNum  %  10;

	SI32 SilverKeyNum[3];
	SilverKeyNum[0] = m_myGachaSilverKeyNum  / 100;
	SilverKeyNum[1] = (m_myGachaSilverKeyNum % 100) / 10;
	SilverKeyNum[2] = m_myGachaSilverKeyNum  %  10;

	SI32 BronzeKeyNum[3];
	BronzeKeyNum[0] = m_myGachaBronzeKeyNum  / 100;
	BronzeKeyNum[1] = (m_myGachaBronzeKeyNum % 100) / 10;
	BronzeKeyNum[2] = m_myGachaBronzeKeyNum  %  10;

	SI32 KeyPieceNum[3];
	KeyPieceNum[0] = m_myGachaPieceNum  / 100;
	KeyPieceNum[1] = (m_myGachaPieceNum % 100) / 10;
	KeyPieceNum[2] = (m_myGachaPieceNum %  10);


	for( SI16 i = 0 ; i < 3 ; i ++)
	{    
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX , 282 +GetY() + ScreenRY, GoldKeyNum[i] );
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX , 332 +GetY() + ScreenRY, SilverKeyNum[i] + 20 );
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX , 382 +GetY() + ScreenRY, BronzeKeyNum[i] + 30);
		GP.PutSpr( &m_NumberImg , 148 + i * 22 +GetX() + ScreenRX,  490 +GetY() + ScreenRY, KeyPieceNum[i]  + 10 );
	}
	//-------------------------------------------------------------------------<
}

void CNGachaDlgNew::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if( ! IsShow() )
		return;

	cltClient *pclclient = ( cltClient * )pclClient;

	Text( ScreenRX, ScreenRY );
}

void CNGachaDlgNew::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{


}


void CNGachaDlgNew::SetGachaProductAndCount( cltItem * pclItem , UI16 GachaCnt )
{
	m_uiGachaCnt = GachaCnt;

	CListView*		pListView   = NULL;
	CEdit*			peditbox	= NULL;
	TCHAR*			pItemName   = NULL;

	SI32			GlobalImageIndex = -1;
	SI32			GlobalImageFont	 = -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;



	for( SI32 Count = 0 ; Count < MAX_GACHA_PRODUCT; Count++ )
	{
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pclItem[Count].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			m_clGachaProductItem[Count].Set( &pclItem[Count] );

			pclClient->pclItemManager->GetExplain( &pclItem[Count], ToolTipMsg );
			pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclItem[Count].siUnique );

			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	pclItem[Count].siItemNum, pItemName, ToolTipMsg );

			if(0 == Count)
			{
				pListView = m_InterfaceFactory.GetListView( NGACHANEW_LISTVIEW_RAREITEM );
				pListView->DeleteAllItems();
				pListView->InsertItem( &ListViewItemData );   
				pListView->SetItemNumShow(false);

				peditbox  = m_InterfaceFactory.GetEdit( NGACHANEW_EDIT_RAREITEM_NAME );
				peditbox->SetText( pItemName );

				// PCK : 가챠 레어아이템 소개 텍스트 수정 (09.03.03)
				NTCHARString256	kText( GetTxtFromMgr(7017) );
				NTCHARString64	kGachaRareItemName( TEXT("") );

				kGachaRareItemName = (TCHAR*)pclClient->pclItemManager->GetName(pclItem[Count].siUnique);
				kText.Replace( TEXT("#gacharareitemname#"), kGachaRareItemName );

				peditbox = m_InterfaceFactory.GetEdit( NGACHANEW_EDIT_EXPLAIN );
				peditbox->SetText( kText );

				// 최고 상품은 큰 이미지로 그린다. 
				TCHAR* pFileName    = NULL;
				SI32   siImageIndex = -1;
				cltItemManagerClient* pItemManager = (cltItemManagerClient*)pclClient->pclItemManager;
				if( pItemManager->GetItemImageBigFileName( pclItem[Count].siUnique, &pFileName, &siImageIndex ) )
				{
					m_InterfaceFactory.GetImageStatic(NGACHANEW_IMAGESTATIC_BEST_ITEM)->SetFileName( pFileName );
					m_InterfaceFactory.GetImageStatic(NGACHANEW_IMAGESTATIC_BEST_ITEM)->SetFontIndex( siImageIndex );
				}	
			}
			else
			{
				pListView = m_InterfaceFactory.GetListView( NGACHANEW_LISTVIEW_GIVEITEM1 + Count - 1 ); // 0번째 컨트롤에 아이템 정보를 넣기위해 -1.
				pListView->DeleteAllItems();
				pListView->InsertItem( &ListViewItemData );   
				pListView->SetItemNumShow(false);

				peditbox  = m_InterfaceFactory.GetEdit( NGACHANEW_EDIT_GIVEITEM1 + Count - 1 ); // 0번째 컨트롤에 아이템 정보를 넣기위해 -1.
				peditbox->SetText( pItemName );
			}
		}
	}
}

void CNGachaDlgNew::SetGachaCount( UI16 _uiGachaCnt )
{
	m_uiGachaCnt = _uiGachaCnt;
}

void CNGachaDlgNew::SetGachaUseKeyKind( SI32 _siGachaUseKeyKind )
{
	m_siGachaUseKeyKind = _siGachaUseKeyKind;	
}
SI32 CNGachaDlgNew::GetGachaUseKeyKind()
{
	return m_siGachaUseKeyKind;	
}


void CNGachaDlgNew::SetExplain2(TCHAR* _pText)
{
	//m_InterfaceFactory.GetEdit(NGACHANEW_EDIT_EXPLAIN2)->SetText( _pText );

	m_pText = _pText;
}

void CNGachaDlgNew::Text( SI32 ScreenRX, SI32 ScreenRY )
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


