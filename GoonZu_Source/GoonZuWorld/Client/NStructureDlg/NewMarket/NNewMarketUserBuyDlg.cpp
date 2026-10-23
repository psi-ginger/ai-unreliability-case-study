#include "NNewMarketDlg.h"


#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/CheckBox.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "../../../common/Item/ItemCommon\cltItem.h"
#include "../../../common/skill/skill-manager.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;


//////////////////////////////////////////////////
// 생성은 되지만 보여지지는 않는다. NMarketUserBuySubDlg 가 대신한다.
//////////////////////////////////////////////////

NNewMarketUserBuyDlg::NNewMarketUserBuyDlg()
{
	
}

NNewMarketUserBuyDlg::~NNewMarketUserBuyDlg()
{
}

void NNewMarketUserBuyDlg::InitDlg()
{
	SetControlMap( STATIC_PRICECOND,	TEXT("statictext_pricecond") );
	SetControlMap( CHECK_PRICECOND,		TEXT("checkbox_pricecond") );
	SetControlMap( COMBO_ITEMTYPE,		TEXT("combobox_itemtype") );
	SetControlMap( COMBO_ITEM,			TEXT("combobox_item") );

	SetControlMap( BUTTON_REFRESH,			TEXT("button_refresh")		);
	SetControlMap( BUTTON_BUY,				TEXT("button_buy")			);
	SetControlMap( BUTTON_ITEMMALL,			TEXT("button_itemmall")		);
	SetControlMap( EDIT_AMOUNT,				TEXT("editbox_amount")		);
	SetControlMap( EDIT_PRICECOND,			TEXT("editbox_pricecond")	);
	SetControlMap( EDIT_ITEMPRICE,			TEXT("editbox_itemprice")	);
	SetControlMap( EDIT_EXPLAIN,			TEXT("editbox_explain")		);
	SetControlMap( EDIT_EXPLAINCHAR,		TEXT("editbox_NONAME1")		);
	SetControlMap( STATIC_ITEMNAME, 		TEXT("statictext_itemname") );
	SetControlMap( STATIC_ITEMTYPE, 		TEXT("statictext_itemtype") );
	SetControlMap( STATIC_ITEMINFO, 		TEXT("statictext_imteinfo") );
	SetControlMap( STATIC_CHARINFO, 		TEXT("statictext_charinfo") );
	SetControlMap( LIST_ORDERLIST,			TEXT("listbox_orderlist")	);
	SetControlMap( IMAGESTATIC_ITEMBACK,	TEXT("imagestatic_NONAME1") );

	m_pControlMap[ EDIT_PRICECOND ]->Show( false );
	m_pControlMap[ STATIC_PRICECOND ]->Show( false );

	((CEdit*)m_pControlMap[ EDIT_AMOUNT ])->SetNumberMode( true );

	// 주문 리스트 컬럼 초기화. 
	CList *pList = (CList*)m_pControlMap[ LIST_ORDERLIST ];
	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = NULL;
 	pText = GetTxtFromMgr(1227);
	pList->SetColumn( 0, 100, pText );
	pText = GetTxtFromMgr(1678);
	pList->SetColumn( 1,  88, pText );
	pText = GetTxtFromMgr(1679);
	pList->SetColumn( 2,  67, pText );

	pList->Refresh();

	// 아이템 종류를 보여준다. 
	{
		cltItemManagerClient*	pclitemmanager = (cltItemManagerClient*)pclClient->pclItemManager;
		CComboBox*				pComboItemType = (CComboBox*)m_pControlMap[ COMBO_ITEMTYPE ];
		stComboBoxItemData		itemdata;

		
		int i;
		// 콤보 박스를 지운다.
		pComboItemType->Clear();

		// 콤보 박스에 아이템의 종류를 설정한다. 
		for( i = 0; i < MAX_ITEMTYPE_NUMBER; i++ )
		{	
			cltItemTypeInfo* pinfo = pclitemmanager->pclItemTypeInfo[i];
			if(pinfo == NULL)continue;

			if(pinfo->IsAtb(ITEMATB_NOTRADE))continue;

			// 콤보리스트에서 숨기는 속성이 있다면
			if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;


			itemdata.Init();
			itemdata.Set( (TCHAR*)pinfo->GetName() );
			pComboItemType->AddItem( &itemdata );
		}
		pComboItemType->SetCursel(0);
	}
}

void NNewMarketUserBuyDlg::Action()
{
	return;
}

void NNewMarketUserBuyDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CASE_CONTROL( LIST_ORDERLIST )
	{
		switch( nEvent )
		{
		case EVENT_LISTBOX_SELECTION:
			{
				
			}
			break;
		}
	}
	else CASE_CONTROL( CHECK_PRICECOND )
	{
		
	}
	else CASE_CONTROL( COMBO_ITEMTYPE )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{

			}
			break;
		}
	}
	else CASE_CONTROL( COMBO_ITEM )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				
			}
			break;
		}
	}
	else CASE_CONTROL( BUTTON_REFRESH )
	{
	
	}
	else CASE_CONTROL( BUTTON_BUY )
	{
		
	}
	else CASE_CONTROL( BUTTON_ITEMMALL )
	{

	}


}


