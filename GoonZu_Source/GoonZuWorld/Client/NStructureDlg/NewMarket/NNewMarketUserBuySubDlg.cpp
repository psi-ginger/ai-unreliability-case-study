#include "NNewMarketDlg.h"


#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "../../../common/Item/ItemCommon\cltItem.h"
#include "../../../common/skill/skill-manager.h"
#include "../../../common/Item/ItemCommon/ItemMallData.h"

#include "../../../../NLib/NTCHARString.h"


extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NNewMarketUserBuyDlgSub::NNewMarketUserBuyDlgSub()
{
	m_siImageIndex			 = -1;
	m_siImageType			 = -1;
	m_siRequestPage			 =  1;	// ó�� 1�������� ����.
	m_siRequestItemUnique	 =  0;
	m_siSelectOrderIndex	 = -1;	// ���õ� �ֹ� ������ �ε���.
	m_bActionRequestItemList = false;
}

NNewMarketUserBuyDlgSub::~NNewMarketUserBuyDlgSub()
{
}


void NNewMarketUserBuyDlgSub::Init( void )
{
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_RARELEVEL_1)->SetCheck(true);
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_RARELEVEL_2)->SetCheck(true);
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_RARELEVEL_3)->SetCheck(true);
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_INCHANT)->SetCheck(true);
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_ARTIFACT)->SetCheck(true);
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_NORMAL)->SetCheck(true);

	m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_LIST)->SetCheck(true);
	m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_LIST)->SetTextPrintOption(DT_LEFT);
	m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_LIST)->Refresh();
	m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->SetCheck(false);
	m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->SetTextPrintOption(DT_LEFT); 
	m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->Refresh(); 

	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_PAGE)->SetTextPrintOption(DT_CENTER); 
	

	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT)->Show(false);

	// ���� â���� ����ϴ��Ŷ� �Ⱥ��̰�.
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_PRICECOND)->Show(false);
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_STATIC_PRICECOND)->Show(false);
	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_EDIT_PRICECOND)->Show(false);

	m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->SetShowHeader(FALSE);
	m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->SetColumn(0, 140, NULL);     
	m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Refresh(); 
	m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Show(false);
	
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_AMOUNT)->SetNumberMode(true);//[�迵�� �߰�:2007.11.21-���忡�� ���ǻ춧 ���ڸ� �Էµǵ��� �߰�]
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAINCHAR)->SetChangeTextDrawMode(true);
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_PRICECOND)->Show(false);

	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_PRICECOND)->Show(false);

	// �ֹ� ����Ʈ 
	CList *pList = m_InterfaceFactory.GetList(NMARKET_SUB_LIST_ORDERLIST);

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = NULL;
	pText = GetTxtFromMgr(1227);
	pList->SetColumn( 0, 150, pText );
	pText = GetTxtFromMgr(1678);
	pList->SetColumn( 1, 110, pText );
	pText = GetTxtFromMgr(1679);
	pList->SetColumn( 2,  60, pText );
	pList->Refresh();

	// ������ Ÿ�� ����Ʈ.
	ShowItemTypeListInCombo();
}

void NNewMarketUserBuyDlgSub::Create()
{
	if( IsCreate() )
		return;
	
	CInterfaceFile file;
	file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_MarketUserBuySub_New.ddf"));

	file.CreatePopupDialog( this, NNEWMARKET_SUB_DLG, TEXT("dialog_marketuserbuysub"), StaticCallBackDialogNNewMarketUserBuyDlgSub);

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON		,NMARKET_SUB_BUTTON_LEFT_PAGE,		this ), NMARKET_SUB_BUTTON_LEFT_PAGE,		TEXT("button_left_page") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON		,NMARKET_SUB_BUTTON_RIGHT_PAGE,		this ), NMARKET_SUB_BUTTON_RIGHT_PAGE,		TEXT("button_right_page") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC		,NMARKET_SUB_STATIC_PAGE,			this ),	NMARKET_SUB_STATIC_PAGE,			TEXT("statictext_page") );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC		,NMARKET_SUB_STATIC_PRICECOND,		this ), NMARKET_SUB_STATIC_PRICECOND,		TEXT("statictext_pricecond") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX	,NMARKET_SUB_COMBO_ITEMTYPE,		this ), NMARKET_SUB_COMBO_ITEMTYPE,			TEXT("combobox_itemtype") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX	,NMARKET_SUB_COMBO_ITEM,			this ), NMARKET_SUB_COMBO_ITEM,				TEXT("combobox_item") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON		,NMARKET_SUB_BUTTON_REFRESH,		this ), NMARKET_SUB_BUTTON_REFRESH,			TEXT("button_refresh") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON		,NMARKET_SUB_BUTTON_BUY,			this ), NMARKET_SUB_BUTTON_BUY,				TEXT("button_buy") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT		,NMARKET_SUB_EDIT_AMOUNT,			this ), NMARKET_SUB_EDIT_AMOUNT,			TEXT("editbox_amount") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT		,NMARKET_SUB_EDIT_PRICECOND,		this ), NMARKET_SUB_EDIT_PRICECOND,			TEXT("editbox_pricecond") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT		,NMARKET_SUB_EDIT_EXPLAIN,			this ), NMARKET_SUB_EDIT_EXPLAIN,			TEXT("editbox_explain") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST		,NMARKET_SUB_LIST_ORDERLIST,		this ), NMARKET_SUB_LIST_ORDERLIST,			TEXT("listbox_orderlist") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NMARKET_SUB_IMAGESTATIC_ITEMBACK,	this ), NMARKET_SUB_IMAGESTATIC_ITEMBACK,	TEXT("imagestatic_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT		,NMARKET_SUB_EDIT_EXPLAINCHAR,		this ), NMARKET_SUB_EDIT_EXPLAINCHAR,		TEXT("editbox_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC		,NMARKET_SUB_STATIC_ITEMNAME,		this ), NMARKET_SUB_STATIC_ITEMNAME,		TEXT("statictext_itemname") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC		,NMARKET_SUB_STATIC_ITEMTYPE,		this ), NMARKET_SUB_STATIC_ITEMTYPE,		TEXT("statictext_itemtype") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC		,NMARKET_SUB_STATIC_ITEMINFO,		this ), NMARKET_SUB_STATIC_ITEMINFO,		TEXT("statictext_imteinfo") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC		,NMARKET_SUB_STATIC_CHARINFO,		this ), NMARKET_SUB_STATIC_CHARINFO,		TEXT("statictext_charinfo") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON		,NMARKET_SUB_BUTTON_CLOSE,			this ), NMARKET_SUB_BUTTON_CLOSE,			TEXT("button_close") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX	,NMARKET_SUB_CHECK_PRICECOND,		this ), NMARKET_SUB_CHECK_ARTIFACT,			TEXT("checkbox_pricecond") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST		,NMARKET_SUB_LISTBOX_INPUT_LIST,	this ), NMARKET_SUB_LISTBOX_INPUT_LIST,		TEXT("listbox_input") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_RARELEVEL_1,		this ), NMARKET_SUB_CHECK_RARELEVEL_1,		TEXT("checkbox_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_RARELEVEL_2,		this ), NMARKET_SUB_CHECK_RARELEVEL_2,		TEXT("checkbox_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_RARELEVEL_3,		this ), NMARKET_SUB_CHECK_RARELEVEL_3,		TEXT("checkbox_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_INCHANT,			this ), NMARKET_SUB_CHECK_INCHANT,			TEXT("checkbox_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_ARTIFACT,		this ), NMARKET_SUB_CHECK_ARTIFACT,			TEXT("checkbox_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_NORMAL,			this ), NMARKET_SUB_CHECK_NORMAL,			TEXT("checkbox_NONAME6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NMARKET_SUB_RADIO_LIST,			this ), NMARKET_SUB_RADIO_LIST,				TEXT("radiobutton_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NMARKET_SUB_RADIO_INPUT,			this ), NMARKET_SUB_RADIO_INPUT,			TEXT("radiobutton_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT		,NMARKET_SUB_EDIT_INPUT,			this ), NMARKET_SUB_EDIT_INPUT,				TEXT("editbox_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE		,NMARKET_SUB_OUTLINE2,				this ), NMARKET_SUB_OUTLINE2,				TEXT("outline_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT		,NMARKET_SUB_EDIT_ITEMPRICE,		this ), NMARKET_SUB_EDIT_ITEMPRICE,			TEXT("editbox_itemprice") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE		,NMARKET_SUB_OUTLINE_TOP,			this ),	NMARKET_SUB_OUTLINE_TOP,			TEXT("outline_NONAME1") );

	SetTimerOnOff(true);
	SetActionTimer(1000);

	Init();
}

void CALLBACK NNewMarketUserBuyDlgSub::StaticCallBackDialogNNewMarketUserBuyDlgSub( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	NNewMarketUserBuyDlgSub *pThis = (NNewMarketUserBuyDlgSub*) pControllerMgr;
	
	pThis->CallBackDialogNNewMarketUseBuyDlgSub(nEvent,nControlID,pControl );
}

void CALLBACK NNewMarketUserBuyDlgSub::CallBackDialogNNewMarketUseBuyDlgSub( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltNewMarketManager *pMarketManager =  pclClient->pclNewMarketManager; 
	cltClient *pclclient = (cltClient*)pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMARKET_SUB_BUTTON_LEFT_PAGE:
		{
			if( m_InterfaceFactory.GetButton(NMARKET_SUB_BUTTON_REFRESH)->IsEnable() )
			{
				if( 1 >= m_siRequestPage ) return;

				--m_siRequestPage;

				RequestNewMarketOrderList( m_siRequestItemUnique );
			}
		}
		break;
	case NMARKET_SUB_BUTTON_RIGHT_PAGE:
		{
			if( m_InterfaceFactory.GetButton(NMARKET_SUB_BUTTON_REFRESH)->IsEnable() )
			{
				if( m_siMaxPage <= m_siRequestPage ) return;

				++m_siRequestPage;

				RequestNewMarketOrderList( m_siRequestItemUnique );
			}
		}
		break;

	case NMARKET_SUB_LIST_ORDERLIST:
		{
			switch(nEvent) 
			{
			case EVENT_LISTBOX_SELECTION:
				{
					CList*	pList  =  m_InterfaceFactory.GetList( NMARKET_SUB_LIST_ORDERLIST );							
					
					TCHAR buf[ 1024 ];
					TCHAR tmpText[ 1024 ];
					memset(buf, 0 , sizeof(buf));
					memset(tmpText, 0, sizeof(tmpText));

					int index = pList->GetSelectedIndex();

					if( index < 0 ) return;

					m_siSelectOrderIndex = index;

					cltItem clitem;
					clitem.Set(&clNewTradeOrder[index].clItemData);

					// �ش� ������ ������ ǥ���Ѵ�. 
					ShowItemExpalan( clitem.siUnique, clNewTradeOrder[index].siServerUnique );
					
					m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_AMOUNT)->SetText( SI32ToString(clitem.siItemNum) );
				}
				break;
			}
		}
		break;

	case NMARKET_SUB_CHECK_PRICECOND:
		{	
			
		}
		break;

	case NMARKET_SUB_COMBO_ITEMTYPE:
		{
			switch( nEvent )
			{
				case EVENT_COMBOBOX_SELECTION_CHANGED:
					{
						CComboBox *pComboItemType =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE);  
						CComboBox *pComboItem     =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);  
						
						int siIndex = pComboItemType->GetSelectedIndex();
						if( siIndex < 0 ) return;

						TCHAR* pszSelText = pComboItemType->GetText( siIndex );

						SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName( pszSelText );
						if(itemtypeunique >=0)
						{
							ShowItemListInCombo( pComboItem, itemtypeunique );
							pComboItem->SetCursel( 0 );
							RefreshOrder();
						}
					}
					break;
			}
		}
		break;
	case NMARKET_SUB_COMBO_ITEM:		
		{
			switch( nEvent )
			{
				case EVENT_COMBOBOX_SELECTION_CHANGED:
					{
						m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Show( false );

						CComboBox *pCombo = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);	
						SI16 siIndex = pCombo->GetSelectedIndex();
						if( siIndex < 0 )	return;
						
						SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( pCombo->GetText( siIndex ) );
						
						// �̹� ��û�� �ߴ� �������̸� �ٽ� ��û���� �ʴ´�.
						if( m_siRequestItemUnique == siItemUnique ) return;
						
						if( siItemUnique )
                            pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &m_siImageType, &m_siImageIndex );

						if( m_InterfaceFactory.GetButton(NMARKET_SUB_BUTTON_REFRESH)->IsEnable() )
						{
							m_siRequestPage = 1; // ���ο� �������� ����Ʈ�� ��û�ϹǷ� 1������ ����.
							RequestNewMarketOrderList( siItemUnique );
						}
						else
						{
							m_bActionRequestItemList = true;
						}
					}
					break;
			}
		}
		break;
	case NMARKET_SUB_BUTTON_REFRESH:
		{
			RequestNewMarketOrderList( m_siRequestItemUnique );
		}
		break;
	case NMARKET_SUB_EDIT_AMOUNT:
		{
			CList* pList = m_InterfaceFactory.GetList( NMARKET_SUB_LIST_ORDERLIST );
			int siSelectItemIndex = pList->GetSelectedIndex();
			if( 0 > siSelectItemIndex ) return;
			
			SI32 buynum = _tstoi( m_InterfaceFactory.GetEdit( NMARKET_SUB_EDIT_AMOUNT )->GetText() );
			if( buynum > clNewTradeOrder[siSelectItemIndex].siAmount )
			{
				buynum = clNewTradeOrder[siSelectItemIndex].siAmount;
				m_InterfaceFactory.GetEdit( NMARKET_SUB_EDIT_AMOUNT )->SetText( SI32ToString(buynum) ); 
			}
		}
		break;
	case NMARKET_SUB_BUTTON_BUY:
		{
			SI32 id = 1;

			CList* pList = m_InterfaceFactory.GetList( NMARKET_SUB_LIST_ORDERLIST );
			int siSelectItemIndex = pList->GetSelectedIndex();

			//-----------------------------------
			// �������� ������ �ľ��Ѵ�. 
			//-----------------------------------
			if( siSelectItemIndex < 0 || siSelectItemIndex >= MAX_NEWMARKET_ITEMLIST ) 
				return;

			//-----------------------------------
			// �����ϰ��� �ϴ� ������ �ľ��Ѵ�.
			//-----------------------------------
			SI32 buynum = atoi( m_InterfaceFactory.GetEdit( NMARKET_SUB_EDIT_AMOUNT )->GetText() );
			if(buynum <= 0)
			{ 
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(1680) );
				return;
			}
			if( buynum > clNewTradeOrder[siSelectItemIndex].siAmount )
			{
				buynum = clNewTradeOrder[siSelectItemIndex].siAmount;
			}

			//-----------------------------------
			// �ֹ��� �ִ´�. 
			//-----------------------------------
			//PersonID�� 0���� ������ �������� ������ ������ �����Ѵ�.
			cltNewTradeOrder clOrder;
			clOrder.Set(&clNewTradeOrder[siSelectItemIndex]);
			clOrder.siAmount = buynum;	// ������  �����Ѵ�. 

			cltMsg clMsg(GAMEMSG_REQUEST_NEWBUYITEM, sizeof(cltNewTradeOrder), (BYTE*)&clOrder);
			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		
		}
		break;
	case NMARKET_SUB_BUTTON_CLOSE:
		{
            DeleteDialog();
		}
		break;

	case NMARKET_SUB_RADIO_INPUT:
		{
			bool		bShow			= m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck();
			CComboBox*	pComboItemType	= m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE);  
			CComboBox*	pComboItem		= m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);

			m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT)->Show(bShow);		
			m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT)->SetText("");
			pComboItemType->Show(!bShow);		 

			if(true == bShow)
			{
				pComboItem->Clear(); 
			}
			else
			{
				int index = pComboItemType->GetSelectedIndex();
				if( index < 0 ) return;

				TCHAR *szSelText = pComboItemType->GetText( index );

				SI32 siItemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);
				if(siItemtypeunique >=0)
				{
					ShowItemListInCombo( pComboItem, siItemtypeunique );
					pComboItem->SetCursel( 0 );
				}
			}
		}
		break;
	case NMARKET_SUB_EDIT_INPUT:
		{
		
		}
		break;
		
	case NMARKET_SUB_LISTBOX_INPUT_LIST:
		{
			switch(nEvent) 
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if( true == m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck() )
					{							
						CList*	pListItem  =  m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST);							
						if( 0 >= pListItem->GetListNum())
							return;

						SI32 Index = pListItem->GetSelectedIndex();
						if(0 > Index) 
							return; 

						CEdit*	pEditInput  =  m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT);							
						pEditInput->SetText(pListItem->GetText(Index, 0));

						pListItem->Clear();  
						pListItem->Show(false);

						SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( pEditInput->GetText() );
						
						if( siItemUnique )
                            pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &m_siImageType, &m_siImageIndex );

						RequestNewMarketOrderList( siItemUnique );
					} 
				}
				break;
			}
		}
		break;
	}
}


void NNewMarketUserBuyDlgSub::Action()
{
	InputAction();	

	if( m_bActionRequestItemList )
	{
		if( m_InterfaceFactory.GetButton(NMARKET_SUB_BUTTON_REFRESH)->IsEnable() )
		{
			CComboBox *pCombo = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);	
			SI16 siIndex = pCombo->GetSelectedIndex();
			if( siIndex < 0 )	return;

			SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( pCombo->GetText( siIndex ) );

			// �̹� ��û�� �ߴ� �������̸� �ٽ� ��û���� �ʴ´�.
			if( m_siRequestItemUnique == siItemUnique ) return;

			if( siItemUnique )
				pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &m_siImageType, &m_siImageIndex );

			ShowItemExpalan( siItemUnique ); 

			m_siRequestPage = 1; // ���ο� �������� ����Ʈ�� ��û�ϹǷ� 1������ ����.
			RequestNewMarketOrderList( siItemUnique );
			m_bActionRequestItemList = false;
		}
	}
}

BOOL NNewMarketUserBuyDlgSub::ShowItemTypeListInCombo( void )
{
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	if( NULL == pclitemmanager ) return FALSE;

	// ������ ������ �����ش�. 
	stComboBoxItemData	itemdata;

	// �޺� �ڽ��� �����.
	CComboBox* pComboItemType = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE);
	pComboItemType->Clear();

	cltItemTypeInfo* pinfo = NULL;

	// �޺� �ڽ��� �������� ������ �����Ѵ�. 
	for( int i = 0; i < MAX_ITEMTYPE_NUMBER; i++ )
	{	
		pinfo = pclitemmanager->pclItemTypeInfo[i];
		if(pinfo == NULL)	continue;

		if(pinfo->IsAtb(ITEMATB_NOTRADE))	continue;

		// �޺�����Ʈ���� ����� �Ӽ��� �ִٸ�
		if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;

		itemdata.Init();
		itemdata.Set( (TCHAR*)pinfo->GetName() );
		pComboItemType->AddItem( &itemdata );
	}

	pComboItemType->SetCursel(0);

	return TRUE;
}


BOOL NNewMarketUserBuyDlgSub::ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique )
{
	pCombo->Clear();	// �޺� �ڽ��� �����.

	cltItemManagerClient*	pclitemmanager	= (cltItemManagerClient*)pclClient->pclItemManager;
	cltItemInfo*			pinfo			= NULL;
	stComboBoxItemData		stItemData;

	// �޺� �ڽ��� �������� ������ �����Ѵ�. 
	for( SI32 i = 0; i < MAX_ITEMINFO_NUMBER; i++ )
	{	
		pinfo = pclitemmanager->pclItemInfo[i];

		if( pinfo == NULL)						continue;
		if( pinfo->siType != itemtypeunique )	continue;
		if( false == pclClient->pclItemManager->IsNewMarketTrade( pinfo->clItem.siUnique ) ) continue;

		if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
		{
			stItemData.Init();
			stItemData.Set( (TCHAR *)pinfo->GetName() );
			pCombo->AddItem( &stItemData );
		}
	}

	pCombo->SetCursel(0);

	return TRUE;

}

void NNewMarketUserBuyDlgSub::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	if( !IsShow() )															     return;
	if( m_siImageIndex < 0)   													 return;
	if( m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->IsOpened())  return;
	
	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	CImageStatic* pImageStatic = m_InterfaceFactory.GetImageStatic(NMARKET_SUB_IMAGESTATIC_ITEMBACK);

	if ( pSpr )
	{
		GP.PutSpr( pSpr ,ScreenRX + GetX() + pImageStatic->GetX() + 10, ScreenRY + GetY() + pImageStatic->GetY() + 2, m_siImageIndex);
	} 
}


void NNewMarketUserBuyDlgSub::InputAction(void)
{

	if( false == m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck() ) 
	{
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Clear();
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Refresh();
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Show(false);	   
		return;
	}

	static SI32 PrevStringLenth = 0; 

	TCHAR*					ItemName	   = m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT)->GetText();	
	CList*					pListItem	   = m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST); //(CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
	CComboBox*				pComboItem	   = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM); //(CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
	const TCHAR*				FindItemName   = NULL;
    cltItemManagerClient*	pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	SI32					CurStringLenth = 0;
	stComboBoxItemData		itemdata;
	stListBoxItemData		Listitemdata;
	
	CurStringLenth = strlen(ItemName);
	if(0 == CurStringLenth)
	{
		PrevStringLenth = CurStringLenth;
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Show(false);	
		return;
	}
	else if(PrevStringLenth == CurStringLenth)
	{
		if(1 == pListItem->GetListNum())
		{
			if(strlen(pListItem->GetText(0, 0)) == strlen(ItemName))
			{
				if(0 != _tcsstr(pListItem->GetText(0, 0), ItemName))
				{
					pListItem->Show(false);
				}
			}
		}

		return;
	}
	else
	{
		PrevStringLenth = CurStringLenth;
	}


	pListItem->Clear(); 
	// �޺� �ڽ��� �����.
	pComboItem->Clear();

	SI32 TextOneSize = GetTextOneSize();
	if(0 == TextOneSize)	return;

	// �޺� �ڽ��� �������� ������ �����Ѵ�. 
	for(SI32 Count = 0; Count < MAX_ITEMINFO_NUMBER; Count++ )
	{	
		cltItemInfo* pinfo = pclitemmanager->pclItemInfo[Count];
		if(pinfo == NULL)		continue;

		if(false == pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))		continue;   

		bool Find = true;
		for(SI32 i = 0; i < CurStringLenth; i += TextOneSize)
		{
			FindItemName = _tcsstr(pinfo->GetName(), &ItemName[i]);
			if(NULL == FindItemName)
			{
				Find = false;
				i = CurStringLenth; 
			}
			if(FindItemName)
			{
				if(strlen(FindItemName) == (size_t)CurStringLenth)
				{
					FindItemName = _tcsstr(pinfo->GetName(), ItemName);
					if(FindItemName)
					{
						i = CurStringLenth;  
					}
				}
			}
		}

		if(true == Find)
		{
			itemdata.Init();
			itemdata.Set( (TCHAR*)pinfo->GetName() );
			pComboItem->AddItem( &itemdata );

			Listitemdata.Init();
			Listitemdata.Set(0, (TCHAR*)pinfo->GetName() ); 
			pListItem->AddItem( &Listitemdata );
			pListItem->Refresh(); 
		}
	}

	if(0 < pListItem->GetListNum())
	{
		// ����Ʈ ������ ���δ�.
		pListItem->Show(true);	
	}	 
	else
	{
		pListItem->Show(false);	
	}
}

SI32 NNewMarketUserBuyDlgSub::GetTextOneSize(void)
{
	switch ( pclClient->siServiceArea )
	{
		case ConstServiceArea_Korea:	return 2;
		case ConstServiceArea_China:	return 2;
		case ConstServiceArea_Japan:	return 2;
		case ConstServiceArea_English:	return 1;
		case ConstServiceArea_Taiwan:	return 2;
		case ConstServiceArea_USA:		return 1;
		case ConstServiceArea_NHNChina:	return 1;
		case ConstServiceArea_EUROPE:	return 1;
	}

	return 0;
}

void NNewMarketUserBuyDlgSub::RefreshItemExpalan( void )
{
	if( 0 > m_siSelectOrderIndex) return;

	cltItem clitem;
	clitem.Set( &clNewTradeOrder[m_siSelectOrderIndex].clItemData );

	SI32 _siItemUnique   = clNewTradeOrder[m_siSelectOrderIndex].clItemData.siUnique;
	SI32 _siServerUnique = clNewTradeOrder[m_siSelectOrderIndex].siServerUnique;

	cltItemManagerClient* pclItemMgr = (cltItemManagerClient*)pclClient->pclItemManager;

	SI32 ref = pclItemMgr->FindItemRefFromUnique( _siItemUnique );
	if(ref <= 0)return ;

	TCHAR strText[ 1024 ];
	TCHAR tmpText[ 1024 ];
	memset(strText, 0 , sizeof(strText));
	memset(tmpText, 0, sizeof(tmpText));

	//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�	 - ����ü�� ��� ������ ����ó�� ���ش�.
	SI32 itemtypeC = pclClient->pclItemManager->pclItemInfo[ref]->siType;
	if(itemtypeC ==ITEMTYPE_CRYSTAL)
	{
		if(pclItemMgr->GetExplainCrystalClient(_siItemUnique, strText) == FALSE) return;
	}
	else
	{
		if(pclItemMgr->GetExplain( &clitem, strText, false,true,false,0,false ) == FALSE) return;
	}

	NTCHARString512	kTmp;
	NTCHARString512	kExplain;
	NTCHARString512	kName;
	NTCHARString512 kType;

	kTmp = strText;

	SI32 siFind1, siFind2, siFind3;
	// ù���� ������ �̸����� ��������
	siFind1 = kTmp.Find( "\r" );
	kName   = kTmp.SubStr( 0, siFind1 );
	
	// ��°���� ������ Ÿ��
	siFind2 = kTmp.Find( "\r", siFind1 + 1 );
	siFind2 = siFind2 +1; // ����� ���� ����.

	siFind3 = kTmp.Find( "\r", kTmp.Length() - 2 );
	
	kType    = kTmp.SubStr( siFind1 + 2, siFind2 - siFind1 - 1 );
	kExplain = kTmp.SubStr( siFind2 + 1, siFind3 - siFind2 - 1 );

	if( _siServerUnique )
	{
		// ���� �̸�.
		TCHAR tmpbuf[32] = "";
		StringCchPrintf( tmpbuf, sizeof(tmpbuf), TEXT("\r\n%s: %s\r"), GetTxtFromMgr(1131), ((cltClient*)pclClient)->GetServerNameFromUnique( _siServerUnique ) );
		StringCchCat( kExplain, sizeof(kExplain), tmpbuf ); 
	}
	

	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->SetText( kName,DT_CENTER );
	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->Refresh();
	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->SetText( kType,DT_CENTER );
	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->Refresh();
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAIN)->SetText( kExplain );
}

void NNewMarketUserBuyDlgSub::ShowItemExpalan( SI32 _siItemUnique, SI32 _siServerUnique )
{
	cltItemManagerClient* pclItemMgr = (cltItemManagerClient*)pclClient->pclItemManager;

	SI32 ref = pclItemMgr->FindItemRefFromUnique( _siItemUnique );
	if(ref <= 0)return ;

	TCHAR strText[ 1024 ];
	TCHAR tmpText[ 1024 ];
	memset(strText, 0 , sizeof(strText));
	memset(tmpText, 0, sizeof(tmpText));

	//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�	 - ����ü�� ��� ������ ����ó�� ���ش�.
	SI32 itemtypeC = pclClient->pclItemManager->pclItemInfo[ref]->siType;
	if(itemtypeC ==ITEMTYPE_CRYSTAL)
	{
		if(pclItemMgr->GetExplainCrystalClient(_siItemUnique, strText) == FALSE) return;
	}
	else
	{
		// �ֹ� ����Ʈ�� �����ߴٸ�.
		if( -1 < m_siSelectOrderIndex )
		{
			cltItem clitem;
			clitem.Set( &clNewTradeOrder[m_siSelectOrderIndex].clItemData );

			if(pclItemMgr->GetExplain( &clitem, strText, false,true,false,0,false ) == FALSE) 
				return;
		}
		else
		{
			pclItemMgr->GetExplain( _siItemUnique, strText );
		}
	}

	NTCHARString512	kTmp;
	NTCHARString512	kExplain;
	NTCHARString512	kName;
	NTCHARString512 kType;

	kTmp = strText;

	SI32 siFind1, siFind2, siFind3;
	// ù���� ������ �̸����� ��������
	siFind1 = kTmp.Find( "\r" );
	kName   = kTmp.SubStr( 0, siFind1 );
	
	// ��°���� ������ Ÿ��
	siFind2 = kTmp.Find( "\r", siFind1 + 1 );
	siFind2 = siFind2 +1; // ����� ���� ����.
 
	siFind3 = kTmp.Find( "\r", kTmp.Length() - 2 );
	 
	kType    = kTmp.SubStr( siFind1 + 2, siFind2 - siFind1 - 1 );
	kExplain = kTmp.SubStr( siFind2 + 1, siFind3 - siFind2 - 1 );

	if( -1 < m_siSelectOrderIndex )
	{
		// ���� �̸�.
		TCHAR tmpbuf[32] = "";
		if( '\n' == kExplain[kExplain.Length()-1] || 0 == kExplain.Length())
		{
			StringCchPrintf( tmpbuf, sizeof(tmpbuf), TEXT("%s: %s"), GetTxtFromMgr(1131), ((cltClient*)pclClient)->GetServerNameFromUnique( _siServerUnique ) );
		}
		else
		{
			StringCchPrintf( tmpbuf, sizeof(tmpbuf), TEXT("\r\n%s: %s"), GetTxtFromMgr(1131), ((cltClient*)pclClient)->GetServerNameFromUnique( _siServerUnique ) );
		}
		StringCchCat( kExplain, sizeof(kExplain), tmpbuf ); 
	}

	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->SetText( kName,DT_CENTER );
	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->Refresh();
	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->SetText( kType,DT_CENTER );
	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->Refresh();
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAIN)->SetText( kExplain );
}

void NNewMarketUserBuyDlgSub::ShowMyCharInfo( SI32 _siItemUnique )
{
	// �������� ���� ���ǰ� �ڽ��� �ɷ�ġ�� ���Ͽ� ǥ���� �ش�.

	TCHAR strText[1024] = {'\0', };
	TCHAR strBuf[256]   = {'\0', };

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( _siItemUnique );
	if(ref <= 0)return ;

	cltPersonInfo*	pclCI =	pclClient->pclCM->CR[1]->pclCI;
	cltParametaBox* pclPB = &pclClient->pclCM->CR[1]->clPB;

	if( pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel)
	{
		TCHAR*	pText = GetTxtFromMgr(649);

		// ���� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf, sizeof(strBuf), pText, pclCI->clIP.GetLevel() );

		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel > pclCI->clIP.GetLevel())
		{
			StringCchCat(strText, sizeof(strText), TEXT("<C08>"));
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText,sizeof(strText),strBuf);
			StringCchCat(strText,sizeof(strText),TEXT("\r\n"));
		}
	}

	if( pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siStr)
	{
		TCHAR*	pText = GetTxtFromMgr(655);

		// �� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf, sizeof(strBuf), pText, pclPB->clTotalBA.GetStr() );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siStr > pclPB->clTotalBA.GetStr() )
		{
			StringCchCat(strText,sizeof(strText),TEXT("<C08>"));
			StringCchCat(strText,sizeof(strText),strBuf);
			StringCchCat(strText,sizeof(strText),TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("\r\n"));
		}
	}

	if( pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siDex)
	{
		TCHAR*	pText = GetTxtFromMgr(6301);

		// ��ø ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf, sizeof(strBuf), pText, pclPB->clTotalBA.GetDex() );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siDex > pclPB->clTotalBA.GetDex())
		{
			StringCchCat(strText, sizeof(strText), TEXT("<C08>"));
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("\r\n"));
		}
	}

	if( pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siMag )
	{
		TCHAR*	pText = GetTxtFromMgr(653);

		// ���� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf, sizeof(strBuf), pText, pclPB->clTotalBA.GetMag());


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siMag > pclPB->clTotalBA.GetMag() )
		{
			StringCchCat(strText, sizeof(strText),	TEXT("<C08>"));
			StringCchCat(strText, sizeof(strText),	strBuf);
			StringCchCat(strText, sizeof(strText),	TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("\r\n"));
		}
	}

	if( pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siVit )
	{
		TCHAR*	pText = GetTxtFromMgr(6302);

		// ü�� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf, sizeof(strBuf), pText, pclPB->clTotalBA.GetVit() );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel > pclPB->clTotalBA.GetVit() )
		{
			//StringCchPrintf( strBuf,256,TEXT("<C08>%s</C>\r"),pText );
			StringCchCat(strText, sizeof(strText), TEXT("<C08>"));
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("\r\n"));
		}
	}

	if( pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siHnd )
	{
		SI32	siHndOrLuk	= 0;
		TCHAR*	pText		= NULL;

		if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
		{
			siHndOrLuk	= pclPB->clTotalBA.GetLuk();
			pText		= GetTxtFromMgr(6065);
		}
		else
		{
			siHndOrLuk	= pclPB->clTotalBA.GetHnd();
			pText		= GetTxtFromMgr(6303);
		}

		// ������ ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf( strBuf, 256, pText, siHndOrLuk );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siHnd > siHndOrLuk )
		{
			StringCchCat(strText, sizeof(strText), TEXT("<C08>"));
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText, sizeof(strText), strBuf);
			StringCchCat(strText, sizeof(strText), TEXT("\r\n"));
		}
	}

	// ��ų ���ǿ� ���� ��ų���� ǥ��

	SI32 skill = 0;

	switch(pclClient->pclItemManager->pclItemInfo[ref]->siType)
	{
		case ITEMTYPE_BOW:	skill = SKILL_USEBOW1;		break;
		case ITEMTYPE_GUN:	skill = SKILL_USEGUN1;		break;
		case ITEMTYPE_AXE:	skill = SKILL_USEAXE1;		break;
		case ITEMTYPE_STAFF:skill = SKILL_USESTAFF1;	break;
		case ITEMTYPE_SPEAR:skill = SKILL_USESPEAR1;	break;
		case ITEMTYPE_SWORD:skill = SKILL_USESWORD1;	break;		
	}

	if( skill )
	{
		TCHAR SkillName[256] = {'\0',};
		if(pclClient->pclSkillManager->GetSkillName( skill, SkillName ) ==  false)return;

		if(skill == pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siSkillKind)
		{
			SI32 totalskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(skill,0,0,&pclCI->clCharItem.clItem[0]); 

			// ��ų ����
			if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siSkillPoint > totalskilllevel)
			{
				StringCchPrintf(strBuf, sizeof(strBuf),TEXT("<C08>%s: %d</C>\r\n") , SkillName, totalskilllevel );
			}
			else
			{
				StringCchPrintf(strBuf, sizeof(strBuf),TEXT("%s: %d\r\n") , SkillName, totalskilllevel );
			}
		}

		StringCchCat( strText , sizeof(strText), strBuf );
	}

	// �����, ����� üũ
	if(pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_EAST | CANUSEITEM_TYPE_WEST ) )
	{
		TCHAR *  pText = GetTxtFromMgr(6304);

		if(pclClient->pclCM->CR[1]->GetKind() == KIND_HERO3 
			|| pclClient->pclCM->CR[1]->GetKind() == KIND_HEROINE3)
		{
			TCHAR * pText = GetTxtFromMgr(6304);

			if(pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_WEST)  )
			{
				StringCchPrintf( strBuf , sizeof(strBuf), TEXT("<C08>%s</C>\r\n"), pText );
			}
			else
			{
				StringCchPrintf( strBuf , sizeof(strBuf), TEXT("%s\r\n"), pText );
			}

			StringCchCat(strText, sizeof(strText), strBuf );

		}		
		else if(pclClient->pclCM->CR[1]->GetKind() == KIND_HERO4 
			|| pclClient->pclCM->CR[1]->GetKind() == KIND_HEROINE4)
		{
			TCHAR *  pText = GetTxtFromMgr(6305); 
			if(pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_EAST)  )
			{
				StringCchPrintf( strBuf , sizeof(strBuf), TEXT("<C08>%s</C>\r\n"), pText );
			}
			else
			{
				StringCchPrintf( strBuf , sizeof(strBuf), TEXT("%s\r\n"), pText );
			}

			StringCchCat(strText, sizeof(strText),strBuf );
		}

	}

	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAINCHAR)->SetText(strText);
}

void NNewMarketUserBuyDlgSub::RefreshOrder()
{ 
	cltNewMarketManager*	pMarketManager	=  pclClient->pclNewMarketManager; 
	CComboBox*			pCombo			= m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);

	int siIndex = pCombo->GetSelectedIndex();

	if( siIndex < 0 ) return;

	SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( pCombo->GetText( siIndex ) );
	
	ShowMyCharInfo( siItemUnique );
	ShowItemExpalan( siItemUnique );
}

void NNewMarketUserBuyDlgSub::SetProductListFromUnique( SI32 _siItemunique )
{
	cltNewMarketManager *pMarketManager =  pclClient->pclNewMarketManager; 

	if( _siItemunique <= 0 ) return;

	// �޺� �ڽ� ����
	SI32	siItemType = 0;
	TCHAR*  pTypeName  = NULL;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique( _siItemunique );
	if(ref > 0)
	{
		siItemType = pclClient->pclItemManager->pclItemInfo[ref]->siType;
		pTypeName  = (TCHAR *)(pclClient->pclItemManager->pclItemTypeInfo[siItemType]->GetName());
	}
	if( NULL == pTypeName ) return;

	m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->FindIndexFromText( pTypeName );
	m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->SetCursel( m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->FindIndexFromText( pTypeName ) );

	SI32 siItemTypeUnique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName( pTypeName );

	int siIndex = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->GetSelectedIndex();
	if( siIndex < 0 ) return;

	TCHAR *pItemName = (TCHAR*)pclClient->pclItemManager->GetName( _siItemunique );

	if( 0 <= siItemTypeUnique )
	{
		ShowItemListInCombo( m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM), siItemTypeUnique );
		m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM)->SetCursel( m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM)->FindIndexFromText( pItemName ) );
		RefreshOrder();
	}
}

void NNewMarketUserBuyDlgSub::RequestNewMarketOrderList( SI32 itemunique )
{
	if( itemunique <= 0 )	return;

	m_siRequestItemUnique = itemunique;	// ���� ��û�ϴ� �������� ����ũ ����. 

	if( 1 > m_siRequestPage || m_siRequestPage > m_siMaxPage )	
		m_siRequestPage = 1;
			
	// ������ Ư�� ������(unique)�� �ֹ�����Ʈ�� ���� ���� �䱸�Ѵ�.
	cltGameMsgRequest_NewMarketItemList clinfo( itemunique, m_siRequestPage, cltGameMsgRequest_NewMarketItemList::TYPE_NORMAL_LIST );
	cltMsg clMsg(GAMEMSG_REQUEST_NEWMARKETITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	m_InterfaceFactory.GetButton(NMARKET_SUB_BUTTON_REFRESH)->Enable(false);
	m_siSelectOrderIndex = -1;
}

void NNewMarketUserBuyDlgSub::RequestNewMarketOrderListRefresh( void )
{
	if( m_siRequestItemUnique <= 0 )	return;

	if( 1 > m_siRequestPage || m_siRequestPage > m_siMaxPage )	
		m_siRequestPage = 1;

	// ������ Ư�� ������(unique)�� �ֹ�����Ʈ�� ���� ���� �䱸�Ѵ�.
	cltGameMsgRequest_NewMarketItemList clinfo( m_siRequestItemUnique, m_siRequestPage, cltGameMsgRequest_NewMarketItemList::TYPE_NORMAL_LIST );
	cltMsg clMsg(GAMEMSG_REQUEST_NEWMARKETITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);

	m_InterfaceFactory.GetButton(NMARKET_SUB_BUTTON_REFRESH)->Enable(false);
	m_siSelectOrderIndex = -1;
}



void NNewMarketUserBuyDlgSub::SetItemList( cltGameMsgResponse_NewMarketItemList* pNewMarketItemList )
{
	m_InterfaceFactory.GetButton(NMARKET_SUB_BUTTON_REFRESH)->Enable( true );

	// �ִ� ������ �� ����.
	SetPage( pNewMarketItemList->siMaxPage );

	// �ֹ� ����Ʈ 
	CList *pList = m_InterfaceFactory.GetList( NMARKET_SUB_LIST_ORDERLIST );
	stListBoxItemData itemdata;
	
	pList->Clear();
	
	cltItem		clitem;	
	char		itemname[128]	 = {"\0"};
	char		pricestring[128] = {"\0"};
	char		amount[128]		 = {"\0"};
	char*		pText			 = GetTxtFromMgr(611);

	for( SI16 i = 0; i < MAX_NEWMARKET_ITEMLIST; ++i )
	{
		clNewTradeOrder[i].Init();

		clitem.Set( &pNewMarketItemList->clNewTradeOrder[i].clItemData );
		if( 0 < clitem.siUnique )
		{
			if( false == IsCheckItem(&clitem) ) continue;
			
			if( true == clitem.GetFullName(pclClient->pclItemManager, itemname, sizeof(itemname) ) )
			{
				g_JYLibFunction.SetNumUnit(pNewMarketItemList->clNewTradeOrder[i].siPrice, pricestring, sizeof(pricestring), pText);
				sprintf(amount,"%d",pNewMarketItemList->clNewTradeOrder[i].siAmount);

				itemdata.Init();
				itemdata.Set( 0, itemname );
				itemdata.Set( 1, pricestring );
				itemdata.Set( 2, amount );

				pList->AddItem( &itemdata );

				clNewTradeOrder[i].Set( &pNewMarketItemList->clNewTradeOrder[i] );
			}
		}
	}
	pList->Refresh();


	char buffer[128] = {"\0"};
	g_JYLibFunction.SetNumUnit( pNewMarketItemList->siCurrentPrice ? pNewMarketItemList->siCurrentPrice : 0, 
								pricestring, 
								sizeof(pricestring), 
								GetTxtFromMgr(611) );
		
	StringCchPrintf(buffer, sizeof(buffer), "%s",pricestring );
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_ITEMPRICE)->SetText( buffer );

	CComboBox *pCombo = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);	
	SI16 siIndex = pCombo->GetSelectedIndex();
	if( siIndex < 0 )	return;

	SI32 siItemUnique = pclClient->pclItemManager->FindUniqueFromName( pCombo->GetText( siIndex ) );
	if( siItemUnique )
		pclClient->pclItemManager->GetItemGImageFileIndexAndFont( siItemUnique, &m_siImageType, &m_siImageIndex );

	ShowItemExpalan( siItemUnique ); 
}

bool NNewMarketUserBuyDlgSub::IsCheckItem( cltItem* _pclItem )
{
	if(true == m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_RARELEVEL_2)->IsCheck())
	{
		if(RARELEVEL_1 == _pclItem->clItemCommon.clCommonInfo.uiRareInfo)
		{
			return true;
		}
	}
	if(true == m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_RARELEVEL_3)->IsCheck())
	{
		if(RARELEVEL_2 == _pclItem->clItemCommon.clCommonInfo.uiRareInfo)
		{
			return true;
		}
	}
	if(true == m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_RARELEVEL_1)->IsCheck())
	{
		if(RARELEVEL_5 == _pclItem->clItemCommon.clCommonInfo.uiRareInfo)
		{
			return true;
		}
	}
	if(true == m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_INCHANT)->IsCheck())
	{
		if(0 < _pclItem->GetEnchantType(pclClient->pclItemManager))
		{
			return true;
		}
	}
	if(true == m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_ARTIFACT)->IsCheck())
	{
		if(0 < _pclItem->GetUseCrystalNum())
		{
			for(SI32 Count = 0; Count < _pclItem->GetUseCrystalNum(); ++Count)
			{
				// ���Կ� ����ü�� ���� ���.
				if(0 <_pclItem->cltItemCrytal[Count].GetCrystalType())
				{
					return true;
				}
			}
		}
	}
	if(true == m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_NORMAL)->IsCheck())
	{
		SI32 CrystalCount = 0;
		if(0 < _pclItem->GetUseCrystalNum())
		{
			for(SI32 Count = 0; Count < _pclItem->GetUseCrystalNum(); ++Count)
			{
				// ���Կ� ����ü�� ���� ���.
				if(0 <_pclItem->cltItemCrytal[Count].GetCrystalType())
				{
					++CrystalCount;
				}
			}
		}

		if( RARELEVEL_1 != _pclItem->clItemCommon.clCommonInfo.uiRareInfo &&
			RARELEVEL_2 != _pclItem->clItemCommon.clCommonInfo.uiRareInfo &&
			RARELEVEL_5 != _pclItem->clItemCommon.clCommonInfo.uiRareInfo &&
			(0 >= _pclItem->GetEnchantType(pclClient->pclItemManager))	  &&
			(0 == CrystalCount)													)
		{
			return true;
		}  
	}

	return false;
}

void NNewMarketUserBuyDlgSub::SetPage( SI32 MaxPage )
{
	m_siMaxPage = MaxPage;

	CStatic* pStatic = m_InterfaceFactory.GetStatic( NMARKET_SUB_STATIC_PAGE );
	
	TCHAR szBuffer[8];
	StringCchPrintf( szBuffer, sizeof(szBuffer), "%d / %d", m_siRequestPage, MaxPage );

	pStatic->SetText( szBuffer );
}