#include "NAmberBoxDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../CommonLogic/MsgType-Item.h"
#include "../../../common/Char/CharManager/CharManager.h"

#include "../Client/Gametip/GameTip.h"

#define WIDTHNUM  6

CNAmberBoxDlg::CNAmberBoxDlg() : CDialogController()
{
	if(false == m_Spr.LoadTSprFromFile("NInterface/image/AmberBox_42X42_Line_out.SPR"))
	{
		MsgBox("파일열기 실패", "AmberBox_42X42_Line_out.SPR");
	}

	if(false == m_SprListViewBack.LoadTSprFromFile("NInterface/image/ListView_Item_Slot_44X44.SPR"))
	{
		MsgBox("파일열기 실패", "ListView_Item_Slot_44X44.SPR");
	}

	m_bInit				= false;
	m_bClickSlot		= -1;
	m_siRandItemListNum = 0;
	m_siUseItemUnique	= 0;

	memset( m_siSelectItemIndex, -1, sizeof(m_siSelectItemIndex) );
}

CNAmberBoxDlg::~CNAmberBoxDlg()
{
	if(m_Spr.pImage)
	{
		GP.FreeSpr(m_Spr);
	}
	
	if(m_SprListViewBack.pImage)
	{
		GP.FreeSpr(m_SprListViewBack);
	}
}

void CNAmberBoxDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NAmberBoxDlg/NAmberBoxDlg.ddf" ) );  
	file.CreatePopupDialog( this, NAMBER_BOX_DLG, TEXT( "dialog_amberbox" ), StaticNAmberBoxDlgProc );

	const SI32 const_ItemKindNum = 3;	// 렌덤하게 나오는 아이템의 개수.
	TCHAR szControlName[128] = "";
	for( SI32 Count = 0; Count < const_ItemKindNum; ++Count )
	{
		StringCchPrintf(szControlName, sizeof(szControlName),  "statictext_itemname%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, AMBER_BOX_STATIC_ITEMNAME0 + Count,		this), AMBER_BOX_STATIC_ITEMNAME0	+ Count, szControlName);
		m_InterfaceFactory.GetStatic(AMBER_BOX_STATIC_ITEMNAME0 + Count)->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE ); 
		m_InterfaceFactory.GetStatic(AMBER_BOX_STATIC_ITEMNAME0 + Count)->SetText( GetTxtFromMgr( 1619 ) ); 
		m_InterfaceFactory.GetStatic(AMBER_BOX_STATIC_ITEMNAME0 + Count)->Refresh();

		StringCchPrintf(szControlName, sizeof(szControlName),  "statictext_itemnum%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC, AMBER_BOX_STATIC_ITEMNUM0 + Count,		this), AMBER_BOX_STATIC_ITEMNUM0	+ Count, szControlName);	
		m_InterfaceFactory.GetStatic(AMBER_BOX_STATIC_ITEMNUM0 + Count)->Show( false ); // 수량은 사라지게..

		StringCchPrintf(szControlName, sizeof(szControlName),  "button_itemget%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	AMBER_BOX_BUTTON_ITEM_GET0 + Count,		this), AMBER_BOX_BUTTON_ITEM_GET0	+ Count, szControlName);	

		StringCchPrintf(szControlName, sizeof(szControlName),  "button_itemclick%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, AMBER_BOX_BUTTON_ITEM_CLICK0 + Count,	this), AMBER_BOX_BUTTON_ITEM_CLICK0  + Count, szControlName);	

		StringCchPrintf(szControlName, sizeof(szControlName),  "listview_item%d", Count);
		file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW, AMBER_BOX_LISTVIEW_ITEM0 + Count,		this), AMBER_BOX_LISTVIEW_ITEM0		+ Count, szControlName);
		m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEM0 + Count)->Enable(false); 
	}  

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW, AMBER_BOX_LISTVIEW_ITEMLIST, this), AMBER_BOX_LISTVIEW_ITEMLIST,	"listview_itemlist");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,	 AMBER_BOX_OUTLINE_No0,		  this), AMBER_BOX_OUTLINE_No0,			"outline_NONAME1");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,	 AMBER_BOX_EDIT_EXPLAIN,	  this), AMBER_BOX_EDIT_EXPLAIN,		"editbox_explain");
	m_InterfaceFactory.GetEdit(AMBER_BOX_EDIT_EXPLAIN)->SetText(GetTxtFromMgr(8861)); 
	m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST)->Enable(false);
}

void CNAmberBoxDlg::Mix()
{
	if( 0 >= m_siRandItemListNum )	return;

	SI32 Count;
	for( Count = 0; Count < AMBER_BOX_LIST_NUM; ++Count )
		m_siMix[Count] = Count;

	SI32 RandIndex = 0;
	for( Count = 0; Count < m_siRandItemListNum; ++Count )
	{
		RandIndex = rand()%m_siRandItemListNum;	// 렌덤하게 나올수 있는 아이템만 섞는다.
		if( RandIndex != Count )
			m_siMix[Count] ^= m_siMix[RandIndex] ^= m_siMix[Count] ^= m_siMix[RandIndex];
	}
}

void CNAmberBoxDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  )
{ 
	SI16 siListViewX = m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST)->GetX();
	SI16 siListViewY = m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST)->GetY();
	
	SI16 X = GetX() + ScreenRX + siListViewX + m_SprListViewBack.GetXSize(); 
	SI16 Y = GetY() + ScreenRY + siListViewY; 
	
	SI32 siListNum  = (WIDTHNUM * (m_siRandItemListNum / WIDTHNUM));
	siListNum	   += (m_siRandItemListNum % WIDTHNUM) ? WIDTHNUM : 0; 

	for(SI32 Count = 0; Count < siListNum; ++Count)
	{
		SI16 X = GetX() + ScreenRX + siListViewX + ((Count % WIDTHNUM) * 44);  
		SI16 Y = GetY() + ScreenRY + siListViewY + ((Count / WIDTHNUM) * 44); 
 
		GP.PutSpr( &m_SprListViewBack, X, Y, 0 );
	}
}


void CNAmberBoxDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	SI16 siListViewX = m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST)->GetX();
	SI16 siListViewY = m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST)->GetY();

	// 랜덤하게 선택된 아이템에 빨간색 사각박스를 씌운다. 
	for(SI32 Count = 0; Count < AMBER_BOX_SELECT_ITEM_LIST_NUM; ++Count)
	{
		if( -1 >= m_siSelectItemIndex[Count] || m_siRandItemListNum <= m_siSelectItemIndex[Count]) continue;

		SI16 X = GetX() + ScreenRX + siListViewX + ((m_siSelectItemIndex[Count] % WIDTHNUM) * 44);
		SI16 Y = GetY() + ScreenRY + siListViewY + ((m_siSelectItemIndex[Count] / WIDTHNUM) * 44);

		GP.PutSpr( &m_Spr, X, Y, 0 );
	}

	if( 0 > m_bClickSlot || AMBER_BOX_SELECT_ITEM_LIST_NUM <= m_bClickSlot ) return;
	if( m_siRandItemListNum <= m_siRandItemNum )							 return;
	
	SI16 X = GetX() + ScreenRX + siListViewX + ((m_siMix[m_siRandItemNum] % WIDTHNUM) * 44);
	SI16 Y = GetY() + ScreenRY + siListViewY + ((m_siMix[m_siRandItemNum] / WIDTHNUM) * 44);

	GP.PutSpr( &m_Spr, X, Y, 0 );
	 
	if( 50 < GetTickCount() - m_dwPrevTickCount )
	{		
		//사운드
		pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_STATUE_BUFF")), 0, 0);
		
		m_dwPrevTickCount = GetTickCount();

		++m_siRandItemNum;
		if( m_siRandItemListNum <= m_siRandItemNum )
		{
			//사운드
			pclClient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_GACHA_ROLLOVER_BOX")), 0, 0);

			SendMsgSelectItemAdd( m_bClickSlot );
		}
	}
}

void CALLBACK CNAmberBoxDlg::StaticNAmberBoxDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNAmberBoxDlg *pThis = (CNAmberBoxDlg*) pControllerMgr;
	pThis->NAmberBoxDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNAmberBoxDlg::NAmberBoxDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case AMBER_BOX_BUTTON_ITEM_CLICK0:
			{
				ClickSlot(0);
			}
			break;
		case AMBER_BOX_BUTTON_ITEM_CLICK1:
			{
				ClickSlot(1);
			}
			break;
		case AMBER_BOX_BUTTON_ITEM_CLICK2:
			{
				ClickSlot(2);
			}
			break;
		case AMBER_BOX_BUTTON_ITEM_GET0:
			{
				SendMsgSelectItemGet( 0 );	
			}
			break;
		case AMBER_BOX_BUTTON_ITEM_GET1:
			{
				SendMsgSelectItemGet( 1 );
			}
			break;
		case AMBER_BOX_BUTTON_ITEM_GET2:
			{
				SendMsgSelectItemGet( 2 );
			}
			break;
	}
}

void CNAmberBoxDlg::SetItemList( SI32* _psiItemList, SI32 _siItemNum, SI16 _siUseItemUnique )
{
	if( 0 >= _siItemNum || AMBER_BOX_LIST_NUM < _siItemNum )	return;
	if ( NULL == _psiItemList )									return;

	m_siRandItemListNum = _siItemNum;
	m_siUseItemUnique	= _siUseItemUnique;

	SI32 siItemBoxList[AMBER_BOX_LIST_NUM] = { 0, };
	memcpy( siItemBoxList, _psiItemList, sizeof(siItemBoxList[0])*_siItemNum );

	CListView*			pListView		 = m_InterfaceFactory.GetListView( AMBER_BOX_LISTVIEW_ITEMLIST );
	TCHAR*				pItemName		 = NULL;
	SI32				GlobalImageIndex = -1;
	SI32				GlobalImageFont	 = -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;
	cltItem				clItem;

	pListView->DeleteAllItems();

	for( SI32 Count = 0; Count < _siItemNum; ++Count )
	{
		clItem.Init();
		clItem.siUnique  = siItemBoxList[Count];
		clItem.siItemNum = 1; // 안보여지기때문에 임의값 설정

		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( clItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			pclClient->pclItemManager->GetExplain( &clItem, ToolTipMsg );
			pItemName = (TCHAR*)pclClient->pclItemManager->GetName( clItem.siUnique );

			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, Count,	clItem.siItemNum, pItemName, ToolTipMsg );
			pListView->InsertItem( &ListViewItemData );   
		}
	}
		
	pListView->SetItemNumShow(false);

	// 아이템의 개수에 따라 창의 크기 변경.
	//SI32 MinusHeight = (5 - (_siItemNum / WIDTHNUM)) * 44;
	////if(0 >  MinusHeight)	MinusHeight   =  0; 
	//if(0 == _siItemNum % WIDTHNUM) MinusHeight  += 44; 
	//	    
	//// 리스트뷰의 크기 변경. 
	//SI32 siHeight = 0;
	//siHeight  = m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST)->GetHeight();
	//siHeight -=  MinusHeight;
	//m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST)->SetHeight(siHeight);
	//
	//// 메인창의 크기 변경.
	//siHeight  = GetHeight();
	//siHeight -=  MinusHeight;
	//SetHeight(siHeight); 
}


void CNAmberBoxDlg::SetSelectItemList( AmberboxSelectItemInfo* pclAmberboxSelectItemInfo )
{
	m_bInit		 = true;
	m_bClickSlot = -1;		// 클릭이 안된 상태로 초기화.

	if( NULL == pclAmberboxSelectItemInfo )	return;

	ZeroMemory( m_clAmberboxSelectItemInfo, sizeof(m_clAmberboxSelectItemInfo) );
	memcpy( m_clAmberboxSelectItemInfo, pclAmberboxSelectItemInfo, sizeof(m_clAmberboxSelectItemInfo) );

	CListView*			pListView		 = NULL;
	TCHAR*				pItemName		 = NULL;
	SI32				GlobalImageIndex = -1;
	SI32				GlobalImageFont	 = -1;

	TCHAR				ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
	stListViewItemData	ListViewItemData;
	cltItem				clItem;
	
	for( SI32 Count = 0; Count < AMBER_BOX_SELECT_ITEM_LIST_NUM; ++Count )
	{
		clItem.Init();
		clItem.siUnique  = m_clAmberboxSelectItemInfo[Count].m_siItemUnique;
		clItem.siItemNum = m_clAmberboxSelectItemInfo[Count].m_siItemNum;
		if(0 >= clItem.siUnique || 0 >= clItem.siItemNum) continue;

		pListView = m_InterfaceFactory.GetListView( AMBER_BOX_LISTVIEW_ITEM0 + m_clAmberboxSelectItemInfo[Count].m_siSlot );
		pListView->DeleteAllItems();
 
		if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( clItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
		{
			pclClient->pclItemManager->GetExplain( &clItem, ToolTipMsg );
			pItemName = (TCHAR*)pclClient->pclItemManager->GetName( clItem.siUnique );

			ListViewItemData.Init();
			ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	clItem.siItemNum, pItemName, ToolTipMsg );
			pListView->InsertItem( &ListViewItemData );   

			m_InterfaceFactory.GetStatic( AMBER_BOX_STATIC_ITEMNAME0   + m_clAmberboxSelectItemInfo[Count].m_siSlot )->SetText( pItemName );
			m_InterfaceFactory.GetButton( AMBER_BOX_BUTTON_ITEM_CLICK0 + m_clAmberboxSelectItemInfo[Count].m_siSlot )->Show( false );
		}
		else
		{
			m_InterfaceFactory.GetStatic( AMBER_BOX_STATIC_ITEMNAME0   + m_clAmberboxSelectItemInfo[Count].m_siSlot )->SetText( GetTxtFromMgr( 1619 ) );
			m_InterfaceFactory.GetButton( AMBER_BOX_BUTTON_ITEM_CLICK0 + m_clAmberboxSelectItemInfo[Count].m_siSlot )->Show( true );
		}
	}
	

	// 선택 된 아이템에 사각형 이미지를 그리기 위해 리스트뷰 상에서 해당 번호를 얻어오자.
	CListView*			pItemList			= m_InterfaceFactory.GetListView(AMBER_BOX_LISTVIEW_ITEMLIST);
	stListViewItemData* pListViewItemData	= NULL;
	SI16				siItemUnique		= 0;
	memset( m_siSelectItemIndex, -1, sizeof(m_siSelectItemIndex) );
	
	for( SI32 SelectItemCount = 0; SelectItemCount < AMBER_BOX_SELECT_ITEM_LIST_NUM; ++SelectItemCount )
	{
		for( SI32 ItemListCount = 0; ItemListCount < pItemList->GetListViewItemCounter(); ++ItemListCount )
		{
			pListViewItemData = pItemList->GetListViewItemData( ItemListCount );
			if(pListViewItemData	==	NULL	)continue	;
			siItemUnique = pclClient->pclItemManager->FindUniqueFromName(pListViewItemData->ItemName);

			if( siItemUnique == m_clAmberboxSelectItemInfo[SelectItemCount].m_siItemUnique )
			{
				m_siSelectItemIndex[SelectItemCount] = ItemListCount;
			}
		}
	}
}

void CNAmberBoxDlg::SetSelectItemListClear( void )
{	
	m_bClickSlot = -1;	// 클릭이 안된 상태로 초기화.

	for( SI32 Count = 0; Count < AMBER_BOX_SELECT_ITEM_LIST_NUM; ++Count )
	{
		m_InterfaceFactory.GetListView( AMBER_BOX_LISTVIEW_ITEM0 + Count )->DeleteAllItems();
		m_InterfaceFactory.GetStatic( AMBER_BOX_STATIC_ITEMNAME0 + Count )->SetText( GetTxtFromMgr( 1619 ) );
		m_InterfaceFactory.GetButton( AMBER_BOX_BUTTON_ITEM_CLICK0 + Count )->Show( true );
	}
}

void CNAmberBoxDlg::SendMsgSelectItemAdd(SI32 _siSlotIndex)
{
	if( 0 > _siSlotIndex && AMBER_BOX_SELECT_ITEM_LIST_NUM <= _siSlotIndex)	return;

	cltGameMsgRequest_Amber_Box_Select_Item_Add Select_Item_Add( _siSlotIndex , m_siUseItemUnique);
	cltMsg clMsg(GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_ADD, sizeof(cltGameMsgRequest_Amber_Box_Select_Item_Add), (BYTE*)&Select_Item_Add);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNAmberBoxDlg::SendMsgSelectItemGet(SI32 _siSlotIndex)
{
	if(	false == m_bInit )											return;
	if( 0 <= m_bClickSlot && AMBER_BOX_SELECT_ITEM_LIST_NUM > m_bClickSlot)	return;

	cltClient* pclclient = (cltClient*)pclClient;

	for( SI32 Count = 0; Count < AMBER_BOX_SELECT_ITEM_LIST_NUM; ++Count )
	{
		if( 0 >= m_clAmberboxSelectItemInfo[Count].m_siItemUnique ||
			0 >= m_clAmberboxSelectItemInfo[Count].m_siItemNum		)	    
		{
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(8860) );
			return;
		}
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient(1);
	if(false == pclchar->IsBlankInv(1, INVMODE_ALL) )
	{ 
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(6634) );
		return;
	}

	if( 0 >= m_siUseItemUnique ) return;

	cltGameMsgRequest_Amber_Box_Select_Item_Get Select_Item_Get( ITEMUNIQUE(m_siUseItemUnique), &m_clAmberboxSelectItemInfo[_siSlotIndex] );
	cltMsg clMsg(GAMEMSG_REQUEST_AMBER_BOX_SELECT_ITEM_GET, sizeof(cltGameMsgRequest_Amber_Box_Select_Item_Get), (BYTE*)&Select_Item_Get);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNAmberBoxDlg::ClickSlot(SI32 _siSlotIndex)
{
	if(	false == m_bInit )													return;
	if( 0 <= m_bClickSlot && AMBER_BOX_SELECT_ITEM_LIST_NUM > m_bClickSlot)	return;

	m_bClickSlot	  = _siSlotIndex;
	
	m_dwPrevTickCount = GetTickCount();
	m_siRandItemNum	  = 0;

	Mix();
}