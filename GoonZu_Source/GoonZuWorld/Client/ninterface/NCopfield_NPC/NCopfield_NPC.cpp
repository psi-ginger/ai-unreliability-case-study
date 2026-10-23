#include "NCopfield_NPC.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/MsgType-Item.h"

#include "../../../CommonLogic/MagicMgr/MagicMgr.h"
#include "../../../CommonLogic/MagicMgr/MagicAttribute/MagicAttribute.h"


NCopfield_NPC::NCopfield_NPC()
{
	m_clUseItem.Init();
	m_siUseItemInvenPos = -1;
	m_siUseMoney = 0;
}


NCopfield_NPC::~NCopfield_NPC()
{
}

void NCopfield_NPC::Create()
{
	if( IsCreate() )
	{
		return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NCopfield_NPCDlg/NCopfield_NPCDlg.ddf");

	file.CreatePopupDialog( this , NCOPFIELD_DLG, "NCopfield_NPCDlg", StaticNCopfield_NPCProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LISTVIEW, 	NCOPFIELD_LISTVIEW_USE_ITEM,		this),  NCOPFIELD_LISTVIEW_USE_ITEM,		TEXT("listview_use_item")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NCOPFIELD_STATIC_PORTRAIT_IMAGE,	this),  NCOPFIELD_STATIC_PORTRAIT_IMAGE,	TEXT("statictext_portrait_image")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NCOPFIELD_STATIC_NONAME1,			this),  NCOPFIELD_STATIC_NONAME1,			TEXT("statictext_NONAME1")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NCOPFIELD_STATIC_NONAME2,			this),  NCOPFIELD_STATIC_NONAME2,			TEXT("statictext_NONAME2")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,  	NCOPFIELD_STATIC_ATTRIBUTE_TYPE,	this),  NCOPFIELD_STATIC_ATTRIBUTE_TYPE,	TEXT("statictext_attribute_type")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,  		NCOPFIELD_EDIT_EXPLAIN,				this),  NCOPFIELD_EDIT_EXPLAIN,				TEXT("editbox_explian")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(COMBOBOX, 	NCOPFIELD_COMBOBOX_ATTRIBUTE_TYPE,	this),  NCOPFIELD_COMBOBOX_ATTRIBUTE_TYPE,	TEXT("combobox_attribute_type")		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST, 		NCOPFIELD_LISTBOX_ITEM_LIST,		this),  NCOPFIELD_LISTBOX_ITEM_LIST,		TEXT("listbox_item_list")			);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NCOPFIELD_BUTTON_BUY_ITEM,			this),  NCOPFIELD_BUTTON_BUY_ITEM,			TEXT("button_buy_item")				);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON, 		NCOPFIELD_BUTTON_CLOSE,				this),  NCOPFIELD_BUTTON_CLOSE,				TEXT("button_close")				);

	Init(); 
}
 
void NCopfield_NPC::Init()
{
	//초상화
	TCHAR	szPortrait[256];
	SI32	siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_COPFIELD")), szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_InterfaceFactory.GetStatic( NCOPFIELD_STATIC_PORTRAIT_IMAGE )->SetFileName(szPortrait);
	m_InterfaceFactory.GetStatic( NCOPFIELD_STATIC_PORTRAIT_IMAGE )->SetFontIndex( siPortraitFont );

	NTCHARString256 szExplain( GetTxtFromMgr( 30508 ) );
	szExplain += GetTxtFromMgr( 30509 );
	szExplain += GetTxtFromMgr( 30510 );
	m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( szExplain );

	// 리스트 창 초기화.
	CList* pList = m_InterfaceFactory.GetList( NCOPFIELD_LISTBOX_ITEM_LIST );
	if( NULL == pList ) return;

	TCHAR* pText = NULL;

	
	pText = GetTxtFromMgr( 1227 ); 
	pList->SetColumn( 0, 130, pText );
	pText = GetTxtFromMgr( 1678 );
	pList->SetColumn( 1, 90, pText ); 
	pList->Refresh();
	pText = GetTxtFromMgr( 40195 );
	pList->SetColumn( 2, 165, pText );
	pList->Refresh(); 
	
	// 리스트 창 초기화.
	CComboBox* pComboBox = m_InterfaceFactory.GetComboBox( NCOPFIELD_COMBOBOX_ATTRIBUTE_TYPE );
	if( NULL == pComboBox ) return;
	
	stComboBoxItemData	clItemData;
	for( SI32 iCount = cltSkillBookManager::ATTRIBUTE_FIRE; iCount < cltSkillBookManager::ATTRIBUTE_NUM; ++iCount )
	{
		StringCchCopy( clItemData.strText, sizeof(clItemData.strText), GetTxtFromMgr(15608 + (iCount-1)) ); 
		clItemData.siParam = iCount;
		pComboBox->AddItem( &clItemData );
	}
	pComboBox->SetCursel( 0 );
	
	SetItemList();
}

void NCopfield_NPC::SetItemList( void )
{
	// 리스트 창 초기화.
	CList* pList = m_InterfaceFactory.GetList( NCOPFIELD_LISTBOX_ITEM_LIST );
	if( NULL == pList ) return;
	pList->Clear();

	// 리스트 창 초기화. 
	CComboBox* pComboBox = m_InterfaceFactory.GetComboBox( NCOPFIELD_COMBOBOX_ATTRIBUTE_TYPE );
	if( NULL == pComboBox ) return;

	stComboBoxItemData*	clItemData = pComboBox->GetItemData( pComboBox->GetSelectedIndex() );
	if( NULL == clItemData ) return;

	SI32 siAttribute = clItemData->siParam;

	stListBoxItemData		clListBoxItemData;
	cltSkillBookInfo		clSkillBookInfo[20];
	cltSkillBookManager*	pclSkillBookManager = pclClient->pclItemManager->pclSkillBookManager;
	if( NULL == pclSkillBookManager ) return;

	NTCHARString64	clString;
	if( pclSkillBookManager->GetSkillBookInfoFromAttribute( siAttribute, sizeof(clSkillBookInfo)/sizeof(cltSkillBookInfo), clSkillBookInfo ) )
	{
		for( SI32 iCount = 0; iCount < sizeof(clSkillBookInfo)/sizeof(cltSkillBookInfo); ++iCount )
		{
			if( 0 >= clSkillBookInfo[iCount].m_siItemUnique ) break;

			clListBoxItemData.Set( 0, (TCHAR*)pclClient->pclItemManager->GetName( clSkillBookInfo[iCount].m_siItemUnique ) );
			clListBoxItemData.Set( 1, SI64ToString( clSkillBookInfo[iCount].m_siCost * (REAL64)pclClient->pclItemManager->clItemStatistic.siNPCPriceRate ) );

			clString = GetTxtFromMgr( 30511 );
			clString.Replace( "#grade#", pclSkillBookManager->GetGradeText( clSkillBookInfo[iCount].m_siBookGrade ) );
			clString.Replace( "#num#",	 SI32ToString( clSkillBookInfo[iCount].m_siBookNum ) );
			clListBoxItemData.Set( 2, clString ); 
			
			clListBoxItemData.siParam[0] = clSkillBookInfo[iCount].m_siItemUnique;
			clListBoxItemData.siParam[1] = clSkillBookInfo[iCount].m_siBookGrade;
			clListBoxItemData.siParam[2] = clSkillBookInfo[iCount].m_siBookNum;   
 
			pList->AddItem( &clListBoxItemData );
		}
	}
	pList->Refresh();
}
void NCopfield_NPC::DelUseItem( void )
{
	CListView* pListView = m_InterfaceFactory.GetListView( NCOPFIELD_LISTVIEW_USE_ITEM );
	pListView->DeleteAllItems();

	m_clUseItem.Init();
	m_siUseItemInvenPos = -1;
	m_siUseMoney = 0;
}


void CALLBACK NCopfield_NPC::StaticNCopfield_NPCProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NCopfield_NPC *pThis = (NCopfield_NPC *)pControllerMgr;
	pThis->NCopfield_NPCProc( nEvent, nControlID, pControl );
}


void CALLBACK NCopfield_NPC::NCopfield_NPCProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NCOPFIELD_BUTTON_CLOSE: 
		{
			DeleteDialog();
		}
		break;
	case NCOPFIELD_LISTVIEW_USE_ITEM: 
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_RBUTTONDOWN:
				{
					DelUseItem();
				}
				break;
			}
		}
		break;
	case NCOPFIELD_COMBOBOX_ATTRIBUTE_TYPE:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:	
				{
					DelUseItem();
					
					SetItemList();
				}
				break;
			}
		}
		break;
	case NCOPFIELD_LISTBOX_ITEM_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:	
				{
					DelUseItem();
				}
			}
		}
		break;
	case NCOPFIELD_BUTTON_BUY_ITEM:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:	
				{
					BuyItem();
				}
				break;
			}
		}
		break;
		
	default:
		{

		}
		break;
	}

}

bool NCopfield_NPC::EndClickAndDrop( cltClickAndDrop* pclClickAndDrop )
{
	static cltClient* pclclient = (cltClient*)pclClient;

	if( NULL == pclClickAndDrop )									return true;
	if( pclClickAndDrop->IsType(CLICKANDDROP_TYPE_ITEM) == false )	return true;

	cltClickAndDrop_Item* pclClickAndDrop_Item = (cltClickAndDrop_Item*)pclClickAndDrop;
	if( NULL == pclClickAndDrop_Item ) return true;

	switch ( pclClickAndDrop->m_siDstControlID )
	{
	case NCOPFIELD_LISTVIEW_USE_ITEM:
		{
			// 리스트 창 초기화.
			CList* pList = m_InterfaceFactory.GetList( NCOPFIELD_LISTBOX_ITEM_LIST );
			if( NULL == pList ) return true;
			stListBoxItemData* pstListBoxItemData = pList->GetItemData( pList->GetSelectedIndex() );
			if( NULL == pstListBoxItemData ) 
			{
				m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(30515) );
				return true;
			}

			cltSkillBookManager* pclSkillBookManager = pclClient->pclItemManager->pclSkillBookManager;
			if( NULL == pclSkillBookManager ) return true;

			// 선택된 아이템과 넣을 아이템의 유니크 검사.
			if( false == pclSkillBookManager->IsSkillBookItemGrade( pstListBoxItemData->siParam[1], pclClickAndDrop_Item->GetItem()->siUnique ) )	
			{
				m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(30514) );
				return true;
			}

			// 선택된 아이템과 넣을 아이템의 개수 검사.
			if( pclClickAndDrop_Item->GetItem()->siItemNum < pstListBoxItemData->siParam[2] )	
			{
				m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(30514) );
				return true;
			}

			// 구입시 사용될 아이템을 등록 시킨다.
			m_clUseItem.Set( pclClickAndDrop_Item->GetItem() );
			m_clUseItem.siItemNum = pstListBoxItemData->siParam[2]; 
			
			m_siUseItemInvenPos = pclClickAndDrop_Item->m_siPara1;

			SI32	GlobalImageIndex = -1;
			SI32	GlobalImageFont	 = -1;
			if( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clUseItem.siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				CListView* pListView = m_InterfaceFactory.GetListView( NCOPFIELD_LISTVIEW_USE_ITEM );
				pListView->DeleteAllItems();

				TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
				pclClient->pclItemManager->GetExplain( &m_clUseItem, ToolTipMsg );

				TCHAR* pItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clUseItem.siUnique );

				stListViewItemData	ListViewItemData;
				ListViewItemData.Init();
				ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, 0,	m_clUseItem.siItemNum, pItemName, ToolTipMsg );
				pListView->InsertItem( &ListViewItemData );
			}
		}
		break;
	}

	return true;
}

void NCopfield_NPC::BuyItem( void )
{
	static cltClient* pclclient = (cltClient*)pclClient;

	if( 0 >= m_clUseItem.siUnique || 0 >= m_clUseItem.siItemNum || 0 >= m_siUseItemInvenPos )
	{
		m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(30514) );
		return;
	}

	// 리스트 창 초기화.
	CList* pList = m_InterfaceFactory.GetList( NCOPFIELD_LISTBOX_ITEM_LIST );
	if( NULL == pList ) return;
	stListBoxItemData* pstListBoxItemData = pList->GetItemData( pList->GetSelectedIndex() );
	if( NULL == pstListBoxItemData ) 
	{
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(30515) );
		return;
	}

	cltSkillBookManager* pclSkillBookManager = pclClient->pclItemManager->pclSkillBookManager;
	if( NULL == pclSkillBookManager ) return;

	// 선택된 아이템
	if( false == pclSkillBookManager->IsSkillBookItemGrade( pstListBoxItemData->siParam[1], m_clUseItem.siUnique ) )	
	{
		m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(30514) );
		return;
	}

	// 선택된 아이템과 넣을 아이템의 개수 검사.
	if( m_clUseItem.siItemNum < pstListBoxItemData->siParam[2] )	
	{
		m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(30514) );
		return;
	}

	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( 1 );
	if( NULL == pclChar ) return;

	stListBoxItemData* pstListBoxItemData2 = pList->GetItemData( pList->GetSelectedIndex(), 1 );
	if( NULL == pstListBoxItemData2 ) return;

	m_siUseMoney = _tstoi64( pstListBoxItemData->strText[1] );
	if( pclChar->pclCI->clIP.GetMoney() < m_siUseMoney )
	{
		m_InterfaceFactory.GetEdit( NCOPFIELD_EDIT_EXPLAIN )->SetText( GetTxtFromMgr(30513) );
		return;
	}

	cltItem clBuyItem;
	clBuyItem.Init();
	clBuyItem.siUnique  = pstListBoxItemData->siParam[0];
	clBuyItem.siItemNum = 1;

	cltGameMsgRequest_SkillBook_BuyItem clSkillBookBuyItem( pstListBoxItemData->siParam[1], &clBuyItem, &m_clUseItem, m_siUseItemInvenPos, m_siUseMoney );
	cltMsg clMsg( GAMEMSG_REQUEST_SKILLBOOK_BUYITEM, PACKET(clSkillBookBuyItem) );
	pclClient->SendMsgToServer( (sPacketHeader*)&clMsg ); 
	
	DelUseItem();
}