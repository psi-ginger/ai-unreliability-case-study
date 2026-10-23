#include "NMarketDlg.h"


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

NMarketUserBuyDlgSub::NMarketUserBuyDlgSub()
{
	bSelectedSwitch = false;
	bMakerSwitch    = false;	

	m_siImageIndex  = -1;
	m_siImageType   = -1;

	m_InputDelay	= false;
}

NMarketUserBuyDlgSub::~NMarketUserBuyDlgSub()
{
}



void NMarketUserBuyDlgSub::Create()
{
	if( IsCreate() )
		return;
	
	m_siImageIndex = -1;
	m_siImageType = -1;

	CInterfaceFile file;

	if(pclClient->IsCountrySwitch(Switch_MarketBuyNew))
	{
		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_MarketUserBuySub_New.ddf"));
	}
	else
	{
		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/DLG_MarketUserBuySub.ddf"));
	}
	
	file.CreatePopupDialog( this, NMARKET_SUB_DLG, TEXT("dialog_marketuserbuysub"), StaticCallBackDialogNMarketUserBuyDlgSub);
	
	cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,NMARKET_SUB_STATIC_PRICECOND,this ), NMARKET_SUB_STATIC_PRICECOND,TEXT("statictext_pricecond") );
		
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX ,NMARKET_SUB_COMBO_ITEMTYPE,this ), NMARKET_SUB_COMBO_ITEMTYPE,TEXT("combobox_itemtype") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX,NMARKET_SUB_COMBO_ITEM,this ), NMARKET_SUB_COMBO_ITEM,TEXT("combobox_item") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,NMARKET_SUB_BUTTON_REFRESH,this ), NMARKET_SUB_BUTTON_REFRESH,TEXT("button_refresh") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,NMARKET_SUB_BUTTON_BUY,this ), NMARKET_SUB_BUTTON_BUY,TEXT("button_buy") );
//	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,NMARKET_SUB_BUTTON_ITEMMALL,this ), NMARKET_SUB_STATIC_PRICECOND,TEXT("") );
    		
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,NMARKET_SUB_EDIT_AMOUNT,this ), NMARKET_SUB_EDIT_AMOUNT,TEXT("editbox_amount") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,NMARKET_SUB_EDIT_PRICECOND,this ), NMARKET_SUB_EDIT_PRICECOND,TEXT("editbox_pricecond") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,NMARKET_SUB_EDIT_EXPLAIN,this ), NMARKET_SUB_EDIT_EXPLAIN,TEXT("editbox_explain") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,NMARKET_SUB_LIST_ORDERLIST,this ), NMARKET_SUB_LIST_ORDERLIST,TEXT("listbox_orderlist") );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC ,NMARKET_SUB_IMAGESTATIC_ITEMBACK,this ), NMARKET_SUB_IMAGESTATIC_ITEMBACK,TEXT("imagestatic_NONAME1") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,NMARKET_SUB_EDIT_EXPLAINCHAR,this ), NMARKET_SUB_EDIT_EXPLAINCHAR,TEXT("editbox_NONAME1") );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,NMARKET_SUB_STATIC_ITEMNAME,this ), NMARKET_SUB_STATIC_ITEMNAME,TEXT("statictext_itemname") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,NMARKET_SUB_STATIC_ITEMTYPE,this ), NMARKET_SUB_STATIC_ITEMTYPE,TEXT("statictext_itemtype") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,NMARKET_SUB_STATIC_ITEMINFO,this ), NMARKET_SUB_STATIC_ITEMINFO,TEXT("statictext_imteinfo") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,NMARKET_SUB_STATIC_CHARINFO,this ), NMARKET_SUB_STATIC_CHARINFO,TEXT("statictext_charinfo") );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,NMARKET_SUB_BUTTON_CLOSE,this ), NMARKET_SUB_BUTTON_CLOSE,TEXT("button_close") );
		
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX ,NMARKET_SUB_CHECK_PRICECOND	,this ), NMARKET_SUB_CHECK_ARTIFACT		,TEXT("checkbox_pricecond") );

	if(pclClient->IsCountrySwitch(Switch_MarketBuyNew))
	{
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST		,NMARKET_SUB_LISTBOX_INPUT_LIST	,this ), NMARKET_SUB_LISTBOX_INPUT_LIST	,TEXT("listbox_input") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_RARELEVEL_1	,this ), NMARKET_SUB_CHECK_RARELEVEL_1	,TEXT("checkbox_NONAME1") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_RARELEVEL_2	,this ), NMARKET_SUB_CHECK_RARELEVEL_2	,TEXT("checkbox_NONAME2") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_RARELEVEL_3	,this ), NMARKET_SUB_CHECK_RARELEVEL_3	,TEXT("checkbox_NONAME3") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_INCHANT		,this ), NMARKET_SUB_CHECK_INCHANT		,TEXT("checkbox_NONAME4") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_ARTIFACT		,this ), NMARKET_SUB_CHECK_ARTIFACT		,TEXT("checkbox_NONAME5") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( CHECKBOX 	,NMARKET_SUB_CHECK_NORMAL		,this ), NMARKET_SUB_CHECK_NORMAL		,TEXT("checkbox_NONAME6") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NMARKET_SUB_RADIO_LIST			,this ), NMARKET_SUB_RADIO_LIST			,TEXT("radiobutton_NONAME3") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( RADIOBUTTON ,NMARKET_SUB_RADIO_INPUT		,this ), NMARKET_SUB_RADIO_INPUT		,TEXT("radiobutton_NONAME4") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT		,NMARKET_SUB_EDIT_INPUT			,this ), NMARKET_SUB_EDIT_INPUT			,TEXT("editbox_NONAME2") );
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE		,NMARKET_SUB_OUTLINE2			,this ), NMARKET_SUB_OUTLINE2			,TEXT("outline_NONAME2") );
		 
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

		m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT)->Show(false);
				
		// ���� â���� ����ϴ��Ŷ� �Ⱥ��̰�.
		m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_PRICECOND)->Show(false);
		m_InterfaceFactory.GetCheckBox(NMARKET_SUB_STATIC_PRICECOND)->Show(false);
		m_InterfaceFactory.GetCheckBox(NMARKET_SUB_EDIT_PRICECOND)->Show(false);

		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->SetShowHeader(FALSE);
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->SetColumn(0, 140, NULL);     
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Refresh(); 
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Show(false);
	}
	else
	{
		
	}

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC   ,NMARKET_SUB_STATIC_MATERIAL	,this ), NMARKET_SUB_STATIC_MATERIAL	,TEXT("statictext_Material") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX ,NMARKET_SUB_COMBO_MATERIAL	,this ), NMARKET_SUB_COMBO_MATERIAL		,TEXT("combobox_Material") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT     ,NMARKET_SUB_EDIT_MATERIALPRICE,this ), NMARKET_SUB_EDIT_MATERIALPRICE	,TEXT("editbox_MaterialPrice") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT	 ,NMARKET_SUB_EDIT_ITEMPRICE	,this ), NMARKET_SUB_EDIT_ITEMPRICE		,TEXT("editbox_itemprice") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE  ,NMARKET_SUB_OUTLINE_TOP		,this),	 NMARKET_SUB_OUTLINE_TOP		,TEXT("outline_NONAME1") );
	
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_AMOUNT)->SetNumberMode(true);//[�迵�� �߰�:2007.11.21-���忡�� ���ǻ춧 ���ڸ� �Էµǵ��� �߰�]

	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_PRICECOND)->Show(false);
	m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_PRICECOND)->Show(false);

	// �ֹ� ����Ʈ 
	CList *pList = m_InterfaceFactory.GetList(NMARKET_SUB_LIST_ORDERLIST);    //(CList*)m_pControlMap[ LIST_ORDERLIST ];

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

 	TCHAR* pText = GetTxtFromMgr(1227);
	pList->SetColumn( 0, 150, pText );
	pText = GetTxtFromMgr(1678);
	pList->SetColumn( 1, 110, pText );
	pText = GetTxtFromMgr(1679);
	pList->SetColumn( 2, 60, pText );

	pList->Refresh();

	pMarketManager->m_siSelectedBuyList = -1;

	// ������ ������ �����ش�. 
	{
		CComboBox *pComboItemType =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE); //(CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
		stComboBoxItemData itemdata;

		cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		int i;
		// �޺� �ڽ��� �����.
		pComboItemType->Clear();

		// �޺� �ڽ��� �������� ������ �����Ѵ�. 
		for( i = 0; i < MAX_ITEMTYPE_NUMBER; i++ )
		{	
			cltItemTypeInfo* pinfo = pclitemmanager->pclItemTypeInfo[i];
			if(pinfo == NULL)continue;

			if(pinfo->IsAtb(ITEMATB_NOTRADE))continue;

			// �޺�����Ʈ���� ����� �Ӽ��� �ִٸ�
			if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;

			itemdata.Init();
			itemdata.Set( (TCHAR*)pinfo->GetName() );
			pComboItemType->AddItem( &itemdata );
		}

		pComboItemType->SetCursel(0);
	}

	pMarketManager->ShowResponseOrderSiwtch			= FALSE;
	pMarketManager->bResponseEmptyOrder				= FALSE;
	pMarketManager->CurrentSelectedBuyItemUnique	= 0;
	bSelectedSwitch									= false;
	bMakerSwitch									= false;

	bRequest = FALSE;
		
	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAINCHAR)->SetChangeTextDrawMode(true);

	SetTimerOnOff(true);
	SetActionTimer(300);
}
 void CALLBACK NMarketUserBuyDlgSub::StaticCallBackDialogNMarketUserBuyDlgSub( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{	
	NMarketUserBuyDlgSub *pThis = (NMarketUserBuyDlgSub*) pControllerMgr;
	
	pThis->CallBackDialogNMarketUseBuyDlgSub(nEvent,nControlID,pControl );
}

void CALLBACK NMarketUserBuyDlgSub::CallBackDialogNMarketUseBuyDlgSub( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);
	cltClient *pclclient = (cltClient*)pclClient;

	CList *pList = m_InterfaceFactory.GetList(NMARKET_SUB_LIST_ORDERLIST);		
	//stListBoxItemData *pitemdata;

	CComboBox *pComboItemType =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE);  // (CComboBox *)m_pControlMap[ COMBO_ITEMTYPE ];
	CComboBox *pComboItem = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);

	CCheckBox *pCheckPrice =  m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_PRICECOND); //(CCheckBox *)m_pControlMap[ CHECK_PRICECOND ];

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMARKET_SUB_LIST_ORDERLIST:
		{
		
		switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{		
					TCHAR buf[ 1024 ];
					TCHAR tmpText[ 1024 ];
					memset(buf, 0 , sizeof(buf));
					memset(tmpText, 0, sizeof(tmpText));

					pMarketManager->m_siSelectedBuyList = -1;

					int index = pList->GetSelectedIndex();
					
					if( index < 0 ) return;

					index = pMarketManager->m_siSelectedBuyIndex[ index ];
					if( index < 0 ) return;

					//[����] ���͸� ����� ���鼭 ����Ʈ �迭�� ��ġ�� �ֹ����� �迭�� �ٸ� �� �־ ��.
					if(false == pclClient->IsCountrySwitch(Switch_MarketBuyNew))
					{
						if ( !pList->GetText( index, 2 ) ||
							_tcscmp( pList->GetText( index, 2) , TEXT("") ) == 0  ) 
						{
							return;
						}
					}

					pMarketManager->m_siSelectedBuyList = index;

					cltItem clitem;
					clitem.Set(&pMarketManager->clResponseOrder[index].clItemData);

					// �ش� ������ ������ ǥ���Ѵ�. 
					cltItemManagerClient *pclItemMgr = ( cltItemManagerClient * )pclClient->pclItemManager;
					pclItemMgr->GetExplain(&clitem, buf, true );

					NTCHARString512	kTmp;
					NTCHARString512	kExplain;

					kTmp = buf;

					SI32 siFind;
					// ù���� ������ �̸����� ��������
					siFind = kTmp.Find("\r");

					// ��°���� ������ Ÿ������ ��������
					siFind = kTmp.Find("\r", siFind+1);

					siFind = siFind +1; // ����� ���� ����.

					kExplain = kTmp.SubStr(siFind+1);

					m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAIN)->SetText(kExplain);					

					CEdit *pEdit = m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_AMOUNT);  //(CEdit*)m_pControlMap[ EDIT_AMOUNT ];
					SI32 sivalue =_tstoi(pEdit->GetText());
					if(sivalue==0)
					{
						pEdit->SetText(TEXT("1"));
					}
				}
				break;
			}
		}

	case NMARKET_SUB_CHECK_PRICECOND :
		{	
			if( pCheckPrice->IsCheck() ) {

				m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_PRICECOND)->Show( true);
				m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_PRICECOND)->Show( true);
			} else {
				m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_PRICECOND)->Show( false);
				m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_PRICECOND)->Show( false);
			}
		}
		break;

	case NMARKET_SUB_COMBO_ITEMTYPE  :
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{					
					int index = pComboItemType->GetSelectedIndex();
					if( index < 0 ) return;

					TCHAR *szSelText = pComboItemType->GetText( index );

					SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);

					if(itemtypeunique >=0)
					{
						ShowItemListInCombo( pComboItem, itemtypeunique, false );
						pComboItem->SetCursel( 0 );
						RefreshOrder(pComboItem);
					}
				}
				break;
			}
		}
		break;

	case NMARKET_SUB_COMBO_ITEM  :		
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					if(pclClient->IsCountrySwitch(Switch_MarketBuyNew))
					{
						if(true == m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck())
						{							
							CComboBox*	pComboItem  =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);							
							if( 0 >= pComboItem->GetItemNum())
								return;

							SI32 Index = pComboItem->GetSelectedIndex();
							if(0 > Index)
								return; 

							CEdit*	pEditInput  =  m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT);							
							pEditInput->SetText(pComboItem->GetText(Index));

							SI32	siItemUnique = pclClient->pclItemManager->FindUniqueFromName(pComboItem->GetText(Index));

							if(siItemUnique > 0)
							{
								ShowItemMaterial(m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_MATERIAL), siItemUnique);
								m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_MATERIAL)->SetCursel(0);
							}

							InputRefreshOrder(pComboItem->GetText(Index));
						}
						else
						{
							CComboBox*	pComboItem  =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);

							SI32 Index = pComboItem->GetSelectedIndex();
							if(0 > Index)
								return; 

							SI32 siItemUnique = pComboItem->GetParam(Index);

							if(siItemUnique >=0)
							{
								ShowItemMaterial(m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_MATERIAL), siItemUnique);
								m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_MATERIAL)->SetCursel(0);
							}

							RefreshOrder(pComboItem);
						}
					}
					else
					{
						RefreshOrder(pComboItem);
					}
				}
				break;
			}
		}
		break;

	case NMARKET_SUB_COMBO_MATERIAL:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					CComboBox*	pComboMaterial  =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_MATERIAL);

					RefreshOrder(pComboMaterial);
				}
				break;
			}
		}
		break;

	case NMARKET_SUB_BUTTON_REFRESH :
		{
			CComboBox*	pComboItem		=  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);
			CComboBox*	pComboMaterial  =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_MATERIAL);

			SI32 siItemIndex = pComboItem->GetSelectedIndex();
			SI32 siMaterialIndex = pComboMaterial->GetSelectedIndex();

			cltMarketManager *pMarketManager =  pclClient->pclMarketManager;
			if( pMarketManager->CurrentSelectedBuyItemUnique == 0)	return;

			SI32	siItemUnique = pclClient->pclItemManager->FindUniqueFromName(pComboItem->GetText(siItemIndex));
			SI32	siMaterialUnique = pclClient->pclItemManager->FindUniqueFromName(pComboMaterial->GetText(siMaterialIndex));

			if(pclClient->IsCountrySwitch(Switch_MarketBuyNew))
			{
				if(true == m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck())
				{
					if(pMarketManager->CurrentSelectedBuyItemUnique == siItemUnique)
					{
						TCHAR* ItemName = pComboItem->GetText(siItemIndex);

						InputRefreshOrder(ItemName);
					}
					else if(pMarketManager->CurrentSelectedBuyItemUnique == siMaterialUnique)
					{
						TCHAR* ItemName = pComboMaterial->GetText(siMaterialIndex);

						InputRefreshOrder(ItemName);
					}
				}
				else
				{
					if(pMarketManager->CurrentSelectedBuyItemUnique == siItemUnique)
					{
						RefreshOrder(pComboItem);
					}
					else if(pMarketManager->CurrentSelectedBuyItemUnique == siMaterialUnique)
					{
						RefreshOrder(pComboMaterial);
					}
				}
			}
			else
			{ 
				if(pMarketManager->CurrentSelectedBuyItemUnique == siItemUnique)
				{
					RefreshOrder(pComboItem);
				}
				else if(pMarketManager->CurrentSelectedBuyItemUnique == siMaterialUnique)
				{
					RefreshOrder(pComboMaterial);
				}
			}
		}
		break;

	case NMARKET_SUB_BUTTON_BUY  :
		{
			SI32 id = 1;
			if(id == 0) return;

			//-----------------------------------
			// �������� ������ �ľ��Ѵ�. 
			//-----------------------------------
			if( pMarketManager->m_siSelectedBuyList < 0) return;

			cltTradeOrder* pclorder = &pMarketManager->clResponseOrder[pMarketManager->m_siSelectedBuyList];

			//-----------------------------------
			// �����ϰ��� �ϴ� ������ �ľ��Ѵ�.
			//-----------------------------------
			SI32 buynum = _tstoi(  m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_AMOUNT )->GetText() );
			if(buynum <= 0)
			{
				TCHAR* pText = GetTxtFromMgr(1680);

				if( pclclient->m_pDialog[NMARKET_DLG])
				{
					((NStrBaseDlg*)pclclient->m_pDialog[NMARKET_DLG])->SetMainExplain(pText);
				}
				else
				{
					TCHAR * pTitle = GetTxtFromMgr( 1664 );

					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(pTitle, pText );
				}

				return;
			}
			if(buynum > pclorder->siAmount)
			{
				buynum = pclorder->siAmount;
			}

			//-----------------------------------
			// �ֹ��� �ִ´�. 
			//-----------------------------------
			//PersonID�� 0���� ������ �������� ������ ������ �����Ѵ�.
			cltTradeOrder clOrder;
			clOrder.Set(pclorder);
			clOrder.siAmount = buynum;	// ������  �����Ѵ�. 

			cltMsg clMsg(GAMEMSG_REQUEST_BUYITEM, sizeof(cltTradeOrder), (BYTE*)&clOrder);

			pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			//pMarketManager->m_siSelectedBuyList = -1;
		}
		break;

	case NMARKET_SUB_BUTTON_CLOSE:
		{
            DeleteDialog();
		}
		break;

	case NMARKET_SUB_RADIO_INPUT:
		{
			bool bShow = m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck();
			
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

				SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(szSelText);
				if(itemtypeunique >=0)
				{
					ShowItemListInCombo( pComboItem, itemtypeunique, false );
					pComboItem->SetCursel( 0 );
					RefreshOrder(pComboItem);
				}
			}
		}
		break;

	case NMARKET_SUB_EDIT_INPUT:
		{
			switch(nEvent) 
			{ 
				case EVENT_EDITBOX_CHANGE:
					{
						int a = 0;
					}
					break;
				case EVENT_EDITBOX_ARROW_DOWN:
					{
						int a = 0;
					}
					break;
			}
		}
		break;
		
	case NMARKET_SUB_LISTBOX_INPUT_LIST:
		{
			switch(nEvent) 
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if(true == m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck())
					{							
						CList*	pListItem  =  m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST);							
						if( 0 >= pListItem->GetListNum())
							return;

						SI32 Index = pListItem->GetSelectedIndex();
						if(0 > Index) 
							return; 

						CEdit*	pEditInput  =  m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT);							
						pEditInput->SetText(pListItem->GetText(Index, 0));

						InputRefreshOrder(pListItem->GetText(Index, 0));

						pListItem->Clear();
						pListItem->Refresh();
						pListItem->Show(false);
					} 
				}
				break;
			}
		}
		break;
	}
}

void NMarketUserBuyDlgSub::Action()
{
	TCHAR strText[ 1024 ];
	TCHAR tmpText[ 1024 ];
	memset(strText, 0 , sizeof(strText));
	memset(tmpText, 0, sizeof(tmpText));

	cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);

	// �����κ��� ���� �ֹ� ������ �����ش�.
	ShowResponseOrder();

	// ��ǰ�� �ü��� �����ش�. 
	ShowPrice();

	if(bSelectedSwitch == true )
	{
		bSelectedSwitch = false;
		bRequest = TRUE;
		dwRequestTick = GetTickCount();

		// ������ ��������  �Ǹ� �ֹ� ����Ʈ�� ��û�Ѵ�. 
		RequestMarketOrderList( pMarketManager->CurrentSelectedBuyItemUnique );

		cltItemManagerClient *pclItemMgr = ( cltItemManagerClient * )pclClient->pclItemManager;

		SI32 ref = pclItemMgr->FindItemRefFromUnique(pMarketManager->CurrentSelectedBuyItemUnique);
		if(ref <= 0)return ;

		//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�	 - ����ü�� ��� ������ ����ó�� ���ش�.
		SI32 itemtypeC = pclClient->pclItemManager->pclItemInfo[ref]->siType;

		if(itemtypeC ==ITEMTYPE_CRYSTAL)
		{
			if(pclItemMgr->GetExplainCrystalClient(pMarketManager->CurrentSelectedBuyItemUnique, strText) == FALSE) return;
		}
		else
		{
			if(pclItemMgr->GetExplain( &pclItemMgr->pclItemInfo[ref]->clItem, strText, false,true,false,0,false ) == FALSE) return;
		}

		NTCHARString512	kTmp;
		NTCHARString512	kExplain;

		NTCHARString512	kName;
		NTCHARString512 kType;

		kTmp = strText;

		SI32 siFind1, siFind2;
		// ù���� ������ �̸����� ��������
		siFind1 = kTmp.Find("\r");
		kName = kTmp.SubStr(0, siFind1);
		// ��°���� ������ Ÿ��
		siFind2 = kTmp.Find("\r", siFind1 + 1);

		siFind2 = siFind2 +1; // ����� ���� ����.

		kType = kTmp.SubStr(siFind1 + 2, siFind2 - siFind1 - 1);

		kExplain = kTmp.SubStr(siFind2 + 1);

		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->SetText(kName,DT_CENTER);
		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->Refresh();

		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->SetText(kType,DT_CENTER);
		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->Refresh();
		//m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAIN)->SetText(tmp);
		m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAIN)->SetText(kExplain);

		ShowMyCharInfo(	pMarketManager->CurrentSelectedBuyItemUnique );

		if(pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pMarketManager->CurrentSelectedBuyItemUnique, &m_siImageType, &m_siImageIndex ) )
		{
		}
		else
		{
			m_siImageIndex = -1;
			m_siImageType  = -1;
		}
	}

	if (bMakerSwitch == true)
	{
		bMakerSwitch = false;

		CList *pList =	m_InterfaceFactory.GetList(NMARKET_SUB_LIST_ORDERLIST);			//(CList*)m_pControlMap[ LIST_ORDERLIST ];
		int index = pList->GetSelectedIndex();

		if( index < 0 ) return;

		index = pMarketManager->m_siSelectedBuyIndex[ index ];
		if( index < 0 ) return;

		pMarketManager->m_siSelectedBuyList = index;

		cltItem clitem;
		clitem.Set(&pMarketManager->clResponseOrder[index].clItemData);

		// �ش� ������ ������ ǥ���Ѵ�. 
		cltItemManagerClient *pclItemMgr = ( cltItemManagerClient * )pclClient->pclItemManager;
		pclItemMgr->GetExplain(&clitem, strText, false );

		NTCHARString512	kTmp;
		NTCHARString512	kExplain;

		NTCHARString512	kName;
		NTCHARString512 kType;

		kTmp = strText;

		SI32 siFind1, siFind2;
		// ù���� ������ �̸����� ��������
		siFind1 = kTmp.Find("\r");
		kName = kTmp.SubStr(0, siFind1);
		// ��°���� ������ Ÿ��
		siFind2 = kTmp.Find("\r", siFind1 + 1);
		kType = kTmp.SubStr(siFind1 + 2, siFind2 - siFind1 - 1);

		kExplain = kTmp.SubStr(siFind2 + 1);

		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->SetText(kName,DT_CENTER);
		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMNAME)->Refresh();

		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->SetText(kType,DT_CENTER);
		m_InterfaceFactory.GetStatic(NMARKET_SUB_STATIC_ITEMTYPE)->Refresh();
		m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAIN)->SetText(kExplain);

		if( false == pclClient->pclItemManager->GetItemGImageFileIndexAndFont( clitem.siUnique, &m_siImageType, &m_siImageIndex ) )
		{
			m_siImageIndex = -1;
			m_siImageType  = -1;
		}
	}

	if(true == pclClient->IsCountrySwitch(Switch_MarketBuyNew))
	{
		InputAction();
	}
}

void NMarketUserBuyDlgSub::ShowPrice()
{
	CComboBox*	pComboItem		=  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM);
	CComboBox*	pComboMaterial  =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_MATERIAL);
	
	cltMarketManager *pMarketManager =  pclClient->pclMarketManager;
	if( pMarketManager->CurrentSelectedBuyItemUnique == 0)	return;

	SI32 siItemIndex = pComboItem->GetSelectedIndex();
	SI32 siMaterialIndex = pComboMaterial->GetSelectedIndex();
	
	if(siItemIndex >= 0)
	{
		// ���� ���õ� ������ ����ũ�� pComboItem���� ���õ� ������ ����ũ�϶��� �ü��� ������Ʈ �Ѵ�.
		SI32	siItemUnique = pclClient->pclItemManager->FindUniqueFromName(pComboItem->GetText(siItemIndex));

		if(pMarketManager->CurrentSelectedBuyItemUnique == siItemUnique)
		{
			TCHAR buffer[256];

			cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice(pMarketManager->CurrentSelectedBuyItemUnique);

			if(pclprice)
			{
				TCHAR pricestring[128] = TEXT("") ;
				TCHAR* pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(pclprice->siPrice, pricestring, 128, pText);
				StringCchPrintf(buffer, 256, TEXT("%s"),pricestring);

				m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_ITEMPRICE)->SetText( buffer,  ((cltClient*)pclClient)->GetColorByPrice(pclprice->siPrice)  );

				// �������� ������ �����ش�.
				m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_MATERIALPRICE)->SetText("");
			}
		}
	}

	if(siMaterialIndex >= 0)
	{
		// ���� ���õ� ������ ����ũ�� pComboItem���� ���õ� ������ ����ũ�϶��� �ü��� ������Ʈ �Ѵ�.
		SI32	siMaterialUnique = pclClient->pclItemManager->FindUniqueFromName(pComboMaterial->GetText(siMaterialIndex));

		if(pMarketManager->CurrentSelectedBuyItemUnique == siMaterialUnique)
		{
			TCHAR buffer[256];

			cltItemPriceUnit* pclprice = pclClient->pclItemManager->GetCurrentPrice(pMarketManager->CurrentSelectedBuyItemUnique);

			if(pclprice)
			{
				TCHAR pricestring[128] = TEXT("") ;
				TCHAR* pText = GetTxtFromMgr(611);
				g_JYLibFunction.SetNumUnit(pclprice->siPrice, pricestring, 128, pText);
				StringCchPrintf(buffer, 256, TEXT("%s"),pricestring);

				m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_MATERIALPRICE)->SetText( buffer,  ((cltClient*)pclClient)->GetColorByPrice(pclprice->siPrice)  );
			}
		}
	}
}

BOOL NMarketUserBuyDlgSub::ShowResponseOrder()
{
	cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);

	// �ֹ� ����Ʈ 
	CList *pList = m_InterfaceFactory.GetList(NMARKET_SUB_LIST_ORDERLIST);	 // (CList*)m_pControlMap[ LIST_ORDERLIST ];
	stListBoxItemData itemdata;

	if( bRequest ) {
		if( GetTickCount() - dwRequestTick >= 5000 ) {
			bRequest = FALSE;

			itemdata.Init();
			TCHAR* pText = GetTxtFromMgr(3456);

			itemdata.Set( 0, pText);


			pList->Clear();
			pList->AddItem( &itemdata );
			pList->Refresh();		

		}
	}

	SI32 i;

	if( pMarketManager->bResponseEmptyOrder == TRUE )
	{
		pMarketManager->ShowResponseOrderSiwtch = FALSE;
		pMarketManager->bResponseEmptyOrder = FALSE;
		bRequest = FALSE;

		itemdata.Init();
		TCHAR* pText = GetTxtFromMgr(3456);
		itemdata.Set( 0, pText);

		pList->Clear();
		pList->AddItem( &itemdata );
		pList->Refresh();		


		return FALSE;
	}

	if( pMarketManager->ShowResponseOrderSiwtch == TRUE)
	{
		pMarketManager->ShowResponseOrderSiwtch = FALSE;
		bRequest = FALSE;
	}
	else 
	{
		return FALSE;
	}

	// ������ ����Ʈ�� �����Ѵ�. 
	pList->Clear();

	bool bcheck[ MAX_RESPONSE_ORDER_NUMBER ];
	for(i = 0;i < MAX_RESPONSE_ORDER_NUMBER; i++ )
	{
		bcheck[i] = false;
	}
	
	SI32 Count  = 0;
	SI32 addnum = 0;
	while( addnum < pMarketManager->siResponseOrderIndex )
	{
		GMONEY minprice	= MAX_PRICE_IN_MARKET;	// 50�� �̻� ������ 1000������.
		SI32 minindex	= -1;

		for(i = 0;i < pMarketManager->siResponseOrderIndex;i++)
		{
			if( bcheck[i] == false && pMarketManager->clResponseOrder[i].siPrice <= minprice)
			{
				minprice = pMarketManager->clResponseOrder[i].siPrice;
				minindex = i;
			}
		}

		if(minindex >= 0)
		{
			bcheck[minindex] = true;

			cltItem clitem;
			clitem.Set(&pMarketManager->clResponseOrder[minindex].clItemData);
			
			if(true == pclClient->IsCountrySwitch(Switch_MarketBuyNew))
			{
				if(false == IsCheckItem(&clitem))
				{
					Count--;
					addnum++;
					continue;
				}  
			}

			
			TCHAR itemname[128] = TEXT("") ;

			if( clitem.GetFullName(pclClient->pclItemManager, itemname, 128) == true)
			{
				TCHAR pricestring[128] = TEXT("") ;
				TCHAR amount[128] = TEXT("") ;
				TCHAR* pText = GetTxtFromMgr(611);

				g_JYLibFunction.SetNumUnit(pMarketManager->clResponseOrder[minindex].siPrice, pricestring, 128, pText);
				StringCchPrintf(amount, 128, TEXT("%d"),pMarketManager->clResponseOrder[minindex].siAmount);

				itemdata.Init();

				COLORREF color = ((cltItemManagerClient* )pclClient->pclItemManager)->GetItemNameColor(&clitem);

				itemdata.Set( 0, itemname , color ); 
				itemdata.Set( 1, pricestring, ((cltClient*)pclClient)->GetColorByPrice(pMarketManager->clResponseOrder[minindex].siPrice) );
				itemdata.Set( 2, amount );

				pList->AddItem( &itemdata );

				pMarketManager->m_siSelectedBuyIndex[addnum + Count] = minindex;
				addnum++;
			}
		}
	}

	pList->Refresh();

	pMarketManager->m_siSelectedBuyList = -1;

	return TRUE;
}

void NMarketUserBuyDlgSub::RefreshOrder(CComboBox* pCombo)
{
	cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);

	int index = pCombo->GetSelectedIndex();

	if( index < 0 ) return;

	SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName( pCombo->GetText( index ) );

	if(itemunique)
	{
		// ���Ÿ� �������� ������ �������� �����Ѵ�. 
		pMarketManager->CurrentSelectedBuyItemUnique = itemunique;

		bSelectedSwitch = true;		
	}
	
	ShowRequestMsg();
}

BOOL NMarketUserBuyDlgSub::ShowItemListInCombo( CComboBox *pCombo, SI32 itemtypeunique, bool btradeinterface )
{
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;

	SI32 i;
	// �޺� �ڽ��� �����.
	pCombo->Clear();

	stComboBoxItemData itemdata;

	// �޺� �ڽ��� �������� ������ �����Ѵ�. 
	for( i = 0;i < MAX_ITEMINFO_NUMBER;i++ )
	{	
		cltItemInfo* pinfo = pclitemmanager->pclItemInfo[i];

		if(pinfo == NULL)continue;
		if(pinfo->siType != itemtypeunique)continue;

		// �־��� Ÿ�԰� ���� �����۸� �����Ѵ�. 
		// ����ڿ��� �������� �������� ��쿡��. 
		bool bshow = false;
		if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
		{
			if(btradeinterface == true)
			{
			//	if(pinfo->IsItemInfoAtb(ITEMINFOATB_TRADELIST)  == true)
				{
					bshow = true;
				}
			}
			else
			{
				bshow = true;
			}

			if(bshow)
			{
				itemdata.Init();
				itemdata.Set( (TCHAR *)pinfo->GetName() );
				itemdata.siParam = pinfo->clItem.siUnique;
				pCombo->AddItem( &itemdata );
			}
		}
	}

	return TRUE;
}

void NMarketUserBuyDlgSub::RequestMarketOrderList( SI32 itemunique )
{
	cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);

	// �ֹ� ����Ʈ 
	CList *pList =   m_InterfaceFactory.GetList(NMARKET_SUB_LIST_ORDERLIST);		//(CList*)m_pControlMap[ LIST_ORDERLIST ];

	if(itemunique <= 0)return ;

	SI32 id = 1;
	if(id == 0)return;

	// ������ ����Ʈ�� �����Ѵ�. 
	pList->Clear();
	pList->Refresh();
	pMarketManager->siResponseOrderIndex = 0;
	pMarketManager->m_siSelectedBuyList = -1;

	// ���� ������ Ȯ���Ѵ�. 
	SI32 pricecond	= 0;

	CCheckBox *pCheckBox =	m_InterfaceFactory.GetCheckBox(NMARKET_SUB_CHECK_PRICECOND);			//	(CCheckBox *)m_pControlMap[ CHECK_PRICECOND ];

	if( pCheckBox->IsCheck() )
	{
		pricecond = _tstoi( m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_PRICECOND)->GetText() );
	}
	
	for ( SI32 siIndex=0; siIndex<MAX_RESPONSE_ORDER_NUMBER; siIndex++ )
	{
		pMarketManager->clResponseOrder[siIndex].Init();
	}

	// ������ Ư�� ������(unique)�� �ֹ�����Ʈ�� ���� ���� �䱸�Ѵ�.
	cltGameMsgRequest_MarketOrder clinfo( itemunique, pricecond );
	cltMsg clMsg(GAMEMSG_REQUEST_MARKETORDER, sizeof(clinfo), (BYTE*)&clinfo);

	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NMarketUserBuyDlgSub::ShowRequestMsg()
{
	// �ֹ� ����Ʈ 
	CList *pList = m_InterfaceFactory.GetList(NMARKET_SUB_LIST_ORDERLIST);		
	stListBoxItemData itemdata;

	itemdata.Init();
	TCHAR* pText = GetTxtFromMgr(5614);
	itemdata.Set( 0,pText);

	pList->Clear();
	pList->AddItem( &itemdata );
	pList->Refresh();
}

void NMarketUserBuyDlgSub::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	/*
	if(m_siImageIndex < 0)   
	return;
	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	if ( pSpr )
	{
	GP.PutSpr(pSpr, ScreenRX + GetParentDialog()->GetX() +  GetX() + 290, ScreenRY + GetParentDialog()->GetY() + GetY() + 70, m_siImageIndex );
	}*/

}

void NMarketUserBuyDlgSub::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	if ( ! IsShow() )
		return;

	if(m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->IsOpened()) return;

	if(m_siImageIndex < 0)   
		return;

	cltClient *pclclient = ( cltClient * )pclClient;
	TSpr *pSpr = pclclient->pclGImg->GetGlobalSpr( m_siImageType );

	CImageStatic* pImageStatic = m_InterfaceFactory.GetImageStatic(NMARKET_SUB_IMAGESTATIC_ITEMBACK);

	if ( pSpr )
	{
 		GP.PutSpr( pSpr ,ScreenRX + GetX() + pImageStatic->GetX() + 10, ScreenRY + GetY() + pImageStatic->GetY() + 2, m_siImageIndex);
	} 
}

void NMarketUserBuyDlgSub::ShowMyCharInfo(SI32 itemunique)
{
	// �������� ���� ���ǰ� �ڽ��� �ɷ�ġ�� ���Ͽ� ǥ���� �ش�.

	TCHAR strText[1024] = {'\0',};
	TCHAR strBuf[256] = {'\0',};

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);
	if(ref <= 0)return ;

	cltPersonInfo * pclCI =	pclClient->pclCM->CR[1]->pclCI;
	cltParametaBox * pclPB = &pclClient->pclCM->CR[1]->clPB;

	if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel)
	{
		TCHAR*	pText = GetTxtFromMgr(649);

		// ���� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����

		StringCchPrintf(strBuf,256,pText, pclCI->clIP.GetLevel() );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel > pclCI->clIP.GetLevel())
		{
			//StringCchPrintf( strBuf,256,TEXT("<C08>%s</C>\r"),pText );
			StringCchCat(strText,1024,TEXT("<C08>"));
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("\r\n"));
		}
	}

	if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siStr)
	{
		TCHAR*	pText = GetTxtFromMgr(655);

		// �� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf,256,pText, pclPB->clTotalBA.GetStr() );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siStr > pclPB->clTotalBA.GetStr() )
		{
			//StringCchPrintf( strBuf,256,TEXT("<C08>%s</C>\r"),pText );
			StringCchCat(strText,1024,TEXT("<C08>"));
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("\r\n"));
		}
	}

	if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siDex)
	{
		TCHAR*	pText = GetTxtFromMgr(6301);

		// ��ø ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf,256,pText, pclPB->clTotalBA.GetDex() );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siDex > pclPB->clTotalBA.GetDex())
		{
			//StringCchPrintf( strBuf,256,TEXT("<C08>%s</C>\r"),pText );
			StringCchCat(strText,1024,TEXT("<C08>"));
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("\r\n"));
		}
	}

	if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siMag )
	{
		TCHAR*	pText = GetTxtFromMgr(653);

		// ���� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf,256,pText, pclPB->clTotalBA.GetMag());


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siMag > pclPB->clTotalBA.GetMag() )
		{
			//StringCchPrintf( strBuf,256,TEXT("<C08>%s</C>\r"),pText );
			StringCchCat(strText,1024,TEXT("<C08>"));
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("\r\n"));
		}
	}

	if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siVit )
	{
		TCHAR*	pText = GetTxtFromMgr(6302);

		// ü�� ���� ���ǿ� ���� �ʴ´ٸ� ���� ���ڵ带 ����
		StringCchPrintf(strBuf,256,pText, pclPB->clTotalBA.GetVit() );


		if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siLevel > pclPB->clTotalBA.GetVit() )
		{
			//StringCchPrintf( strBuf,256,TEXT("<C08>%s</C>\r"),pText );
			StringCchCat(strText,1024,TEXT("<C08>"));
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("\r\n"));
		}
	}

	if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siHnd )
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
			//StringCchPrintf( strBuf,256,TEXT("<C08>%s</C>\r"),pText );
			StringCchCat(strText,1024,TEXT("<C08>"));
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("</C>\r\n"));
		}
		else
		{
			StringCchCat(strText,1024,strBuf);
			StringCchCat(strText,1024,TEXT("\r\n"));
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
			SI32 totalskilllevel = pclCI->clCharSkill.GetTotalSkillLevel(skill,0,0,&pclCI->clCharItem.clItem[0], 1); 

			// ��ų ����
			if(pclClient->pclItemManager->pclItemInfo[ref]->clCondition.siSkillPoint > totalskilllevel)
			{
				StringCchPrintf(strBuf, 256,TEXT("<C08>%s: %d</C>\r\n") , SkillName, totalskilllevel );
			}
			else
			{
				StringCchPrintf(strBuf, 256,TEXT("%s: %d\r\n") , SkillName, totalskilllevel );
			}
		}

		StringCchCat( strText , 1024, strBuf );
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
				StringCchPrintf( strBuf , 256, TEXT("<C08>%s</C>\r\n"), pText );
			}
			else
			{
				StringCchPrintf( strBuf , 256, TEXT("%s\r\n"), pText );
			}

			StringCchCat(strText, 1024,strBuf );

		}		
		else if(pclClient->pclCM->CR[1]->GetKind() == KIND_HERO4 
			|| pclClient->pclCM->CR[1]->GetKind() == KIND_HEROINE4)
		{
			TCHAR *  pText = GetTxtFromMgr(6305);
			if(pclClient->pclItemManager->pclItemInfo[ref]->IsItemInfoAtb(CANUSEITEM_TYPE_EAST)  )
			{
				StringCchPrintf( strBuf , 256, TEXT("<C08>%s</C>\r\n"), pText );
			}
			else
			{
				StringCchPrintf( strBuf , 256, TEXT("%s\r\n"), pText );
			}

			StringCchCat(strText, 1024,strBuf );
		}

	}

	// ��, �� üũ

	m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_EXPLAINCHAR)->SetText(strText);
}

void NMarketUserBuyDlgSub::SetProductListFromUnique(SI32 itemunique)
{
	cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);

	if(itemunique <= 0) return;
	
	// ���Ÿ� �������� ������ �������� �����Ѵ�. 
	pMarketManager->CurrentSelectedBuyItemUnique = itemunique;

	bSelectedSwitch = true;		

	// �޺� �ڽ� ����
	SI32 type;
	TCHAR * ptypename = NULL;

	SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(itemunique);

	if(ref > 0)
	{
		type = pclClient->pclItemManager->pclItemInfo[ref]->siType;

		ptypename = (TCHAR *)(pclClient->pclItemManager->pclItemTypeInfo[type]->GetName());
	}
	if(ptypename  == NULL) return;

	m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->FindIndexFromText(ptypename);
	m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->SetCursel(m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->FindIndexFromText(ptypename));

	SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(ptypename);

	int index = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->GetSelectedIndex();
	if( index < 0 ) return;

	//TCHAR *szSelText = m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEMTYPE)->GetText( index );
	TCHAR *pItemName = (TCHAR*)pclClient->pclItemManager->GetName(itemunique);

	if(itemtypeunique >=0)
	{
		ShowItemListInCombo( m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM), itemtypeunique, false );
		m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM)->SetCursel( m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM)->FindIndexFromText(pItemName) );
		//RefreshOrder();
	}
	
	ShowRequestMsg();
}

bool NMarketUserBuyDlgSub::IsCheckItem(cltItem* _pclItem)
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

		if(RARELEVEL_1 != _pclItem->clItemCommon.clCommonInfo.uiRareInfo &&
		   RARELEVEL_2 != _pclItem->clItemCommon.clCommonInfo.uiRareInfo &&
		   RARELEVEL_5 != _pclItem->clItemCommon.clCommonInfo.uiRareInfo &&
		   (0 >= _pclItem->GetEnchantType(pclClient->pclItemManager))	 &&
		   (0 == CrystalCount)													)
		{
			return true;
		}  
	}

	return false;
}

void NMarketUserBuyDlgSub::InputRefreshOrder(TCHAR* ItemName)
{
	if(NULL == ItemName)
		return;

	if(' ' == ItemName[0])
	{
		ItemName++; 
	}
	
	SI32 itemunique = pclClient->pclItemManager->FindUniqueFromName( ItemName );
	if(0 >= itemunique)
	{
		cltClient *pclclient = (cltClient*)pclClient;
		NTCHARString128 kbuf(GetTxtFromMgr(2266));
		kbuf.Replace("[%s]", ItemName);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(GetTxtFromMgr(5614), kbuf);		
		return;
	}

	if(itemunique)
	{
		cltMarketManager *pMarketManager =  pclClient->pclMarketManager; // ((cltMarketManager *)m_pStrManager);
		// ���Ÿ� �������� ������ �������� �����Ѵ�. 
		pMarketManager->CurrentSelectedBuyItemUnique = itemunique;

		bSelectedSwitch = true;	
	}

	ShowRequestMsg();
}

// [����] ����Ʈ �ڽ��� �ѱ۹��ڸ� ������ �޽����� ���� �ʾƼ� ������ ���� ������ Ȯ���� �޶����� ���� ������ ��������.
void NMarketUserBuyDlgSub::InputAction(void)
{
	
	if(false == m_InterfaceFactory.GetRadioButton(NMARKET_SUB_RADIO_INPUT)->IsCheck()) 
	{
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Clear();
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Refresh();
		m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST)->Show(false);	   
		return;
	}

	static SI32 PrevStringLenth = 0; 
	
	SI32   CurStringLenth	= 0;
	TCHAR* ItemName			= m_InterfaceFactory.GetEdit(NMARKET_SUB_EDIT_INPUT)->GetText();	
	
	CList*				pListItem	   =  m_InterfaceFactory.GetList(NMARKET_SUB_LISTBOX_INPUT_LIST); //(CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
	CComboBox*			pComboItem	   =  m_InterfaceFactory.GetComboBox(NMARKET_SUB_COMBO_ITEM); //(CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
	stComboBoxItemData	itemdata;
	stListBoxItemData	Listitemdata;
	const TCHAR*			FindItemName   = NULL;
	cltItemManagerClient* pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
	
	CurStringLenth = strlen(ItemName);
	if(0 == CurStringLenth)
	{
		PrevStringLenth = CurStringLenth;
		pListItem->Clear();
		pListItem->Refresh();
		pListItem->Show(false);	
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
					pListItem->Clear();
					pListItem->Refresh();
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
	if(0 == TextOneSize)
		return;
 
	// �޺� �ڽ��� �������� ������ �����Ѵ�. 
	for(SI32 Count = 0; Count < MAX_ITEMINFO_NUMBER; Count++ )
	{	
		cltItemInfo* pinfo = pclitemmanager->pclItemInfo[Count];
		if(pinfo == NULL)
			continue;

		if(false == pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
			continue;   

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
		pListItem->Clear();
		pListItem->Refresh();
		pListItem->Show(false);	
	}

	//pComboItem->SetCursel(0);
}

SI32 NMarketUserBuyDlgSub::GetTextOneSize(void)
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

void NMarketUserBuyDlgSub::ShowItemMaterial(CComboBox *pCombo, SI32 siItemUnique)
{
	cltItemManagerClient*	pclItemManager = (cltItemManagerClient*)pclClient->pclItemManager;

	pCombo->Clear();

	SI32	siRef = pclItemManager->FindItemRefFromUnique(siItemUnique);

	stComboBoxItemData	ComboBoxItemData;
	cltItemInfo*	pItemInfo = pclItemManager->pclItemInfo[siRef];

	if(pItemInfo == NULL)	return;

	//�������� Unique�� �ٰŷ� �ش� ��Ḧ ���Ѵ�.
	for(SI32 i = 0; i < MAX_MATERIAL_NUMBER; i++)
	{
		SI32	siMaterialUnique = pItemInfo->clProductionInfo.clMaterial[i].siUnique;

		if(siMaterialUnique > 0)
		{
			SI32	siMaterialRef = pclItemManager->FindItemRefFromUnique(siMaterialUnique);
			cltItemInfo*	pMaterialItemInfo = pclItemManager->pclItemInfo[siMaterialRef];
			
			if(pItemInfo == NULL)	continue;

			ComboBoxItemData.Init();
			ComboBoxItemData.Set((TCHAR*)pMaterialItemInfo->GetName());
			pCombo->AddItem(&ComboBoxItemData);
		}
	}
}