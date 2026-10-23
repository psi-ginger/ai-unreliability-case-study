#include "NPrivateTradeDlg.h"
#include "NPTradeChild1.h"
#include "Resource.h"

#include "..\..\..\Common\SMLib\LibList\LibList.h"
#include "..\..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"

//#include "..\..\Interface\PrivateChatDlg\PrivateChatDlgMgr.h"

#include "..\..\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Interface\NotifyMsgDlg\NotifyMsgDlg.h"


#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/House/House.h"

#include "../../../lib/WebHTML/WebHTML.h"



#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"
#include "../../InterfaceMgr/interface/Static.h"



#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"


#include "Client.h"

extern cltCommonLogic* pclClient;



CNTradeChild1::CNTradeChild1()
{	
	m_pNPTradeListView1  = NULL;
    m_pNPTrade1Edit1 = NULL;
	m_pNPTrade1Edit2 = NULL;
	m_pNPTrade1Button1 = NULL;
	m_pNPTrade1Button2 = NULL;
	m_pNPTrade1Button3 = NULL;
	m_pNPTrade1Static1 = NULL;
	m_pNPTrade1Static2 = NULL;
	m_pParent= NULL;

	m_siInvSelectedIndex = -1;
}

CNTradeChild1::~CNTradeChild1()
{	
	SAFE_DELETE(m_pNPTradeListView1);
	SAFE_DELETE( m_pNPTrade1Edit1);
	SAFE_DELETE( m_pNPTrade1Edit2);
	SAFE_DELETE( m_pNPTrade1Button1);
	SAFE_DELETE( m_pNPTrade1Button2);
	SAFE_DELETE( m_pNPTrade1Button3);
	SAFE_DELETE( m_pNPTrade1Static1);
	SAFE_DELETE( m_pNPTrade1Static2);
	//SAFE_DELETE( m_pParent);


}

void CNTradeChild1::Create(CControllerMgr *pParent)
{
	if(!IsCreate())
	{
		m_pParent =(CNPrivateTradeDlg*)pParent;
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPrivateTrade/DLG_PTradeChild1.ddf"));
		file.CreateChildDialog( this, NPTRADECHILD1_DLG, TEXT("dialog_PTradeChild1"),StaticNTradeChild1Proc, pParent);
		

		m_pNPTradeListView1  = new CListView( this );
		m_pNPTrade1Edit1 = new CEdit(this);
		m_pNPTrade1Edit2 = new CEdit(this);
		m_pNPTrade1Button1 = new CButton(this);
		m_pNPTrade1Button2 = new CButton(this);
		m_pNPTrade1Button3 = new CButton(this);
		m_pNPTrade1Static1 = new CStatic(this);
		m_pNPTrade1Static2 = new CStatic(this);
		
		

		file.CreateControl(m_pNPTradeListView1, NPTRADECHILD1_LISTVIEW , TEXT("listview_PTradeChild1"));	
		file.CreateControl(m_pNPTrade1Edit1, NPTRADECHILD1_EDIT1 , TEXT("editbox_PTradeChild1_1"));	
		file.CreateControl(m_pNPTrade1Edit2, NPTRADECHILD1_EDIT2 , TEXT("editbox_PTradeChild1_2"));	
		file.CreateControl(m_pNPTrade1Button1, NPTRADECHILD1_BUTTON1 , TEXT("button_PTradeChild1_1"));
		file.CreateControl(m_pNPTrade1Button2, NPTRADECHILD1_BUTTON2 , TEXT("button_PTradeChild1_2"));	
		file.CreateControl(m_pNPTrade1Button3, NPTRADECHILD1_BUTTON3 , TEXT("button_PTradeChild1_3"));	
		file.CreateControl(m_pNPTrade1Static1, NPTRADECHILD1_STATIC1 , TEXT("statictext_PTradeChild1_1"));	
		file.CreateControl(m_pNPTrade1Static2, NPTRADECHILD1_STATIC2 , TEXT("statictext_PTradeChild1_2"));	

		m_pNPTrade1Edit1->SetText(TEXT("0"));
		m_pNPTrade1Edit2->SetText(TEXT("0"));
		m_pNPTradeListView1->SetBKColorUse(true);
		m_pNPTradeListView1->SetBorder(true);
		m_pNPTrade1Edit1->SetNumberMode( EDIT_INPUT_MODE_NUMBER );
		//m_pNPTrade1Edit2->SetNumberMode( EDIT_INPUT_MODE_NUMBER );
	
	
		LoadInventoryData();
		Hide();
	}

	
}

	
void  CNTradeChild1::Show()
{
	CDialogController::Show( true );
}

void  CNTradeChild1::Hide()
{
	CDialogController::Show( false );

}

void CALLBACK CNTradeChild1::StaticNTradeChild1Proc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNTradeChild1 *pThis = (CNTradeChild1*) pControllerMgr;
	pThis->NTradeChild1Proc( nEvent, nControlID, pControl );

}


void CALLBACK CNTradeChild1::NTradeChild1Proc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
		
	cltClient *pclclient = (cltClient*)pclClient; 
	SI16 m_siSelectedModeIndex = m_pParent->m_siSelectedModeIndex ;
	//UI32 m_uiLastAcceptFrame= (((CNPrivateTradeDlg*)pclclient->m_pDialog[ NPRIVATETRADE_DLG]))->m_uiLastAcceptFrame;	// 거래에 주어지는 간격
	SI32 m_siLastAcceptTime= (((CNPrivateTradeDlg*)pclclient->m_pDialog[ NPRIVATETRADE_DLG]))->m_siLastAcceptTime;	// 거래에 주어지는 간격

	switch (nControlID  )
	{

	case NPTRADECHILD1_EDIT2:
		{
			switch( nEvent ) {
			case EVENT_EDITBOX_CHANGE :
				{
					//개인거래 제한
					NTCHARString512	kRawPrice( m_pNPTrade1Edit2->GetText() );
					kRawPrice.ReplaceAll(",", "");	// , 삭제
					TCHAR		szPrice[64] = { '\0', };
					GMONEY		siPrice = _tstoi64(kRawPrice);
					if( siPrice > pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit"))
						siPrice = pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit");
					if( kRawPrice.Length() > 0 )
						g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
					m_pNPTrade1Edit2->SetText( szPrice);

					// 왜 콤마를 찍으면 3자리 이상 쓰지를 못할까!

				}
				break;
			}
		}
	

	case NPTRADECHILD1_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					TCHAR buffer[256]= TEXT("") ;
					m_siInvSelectedIndex= m_pNPTradeListView1->GetSelectedItemIndex();
					if ( m_siInvSelectedIndex >= 0 )
						{
							stListViewItemData *pItemData = m_pNPTradeListView1->GetListViewItemData(m_siInvSelectedIndex- PERSONITEM_INV0);
							if ( pItemData )
							{	
								StringCchPrintf( buffer, 256, TEXT("%d"),pItemData->ItemNum );
								m_pNPTrade1Edit1->SetText(buffer);
							}
						}					
				}
				break;
			}
		}break;
	case NPTRADECHILD1_BUTTON1:   //선택 물품 올리기 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: 
				{
					if(m_siInvSelectedIndex < 0)			return;

					/*
					
					// 물품 수리 시에는 한명만 아이템 한개만을 올릴수 있음.
					if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
					{
						// 상대방 물품 목록에 아이템이 올라가 있는지 체크
						stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
						
						for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
						{
							if ( pTradeData->m_clItem[ i ].siUnique != 0 )
							{
								TCHAR* pText = GetTxtFromMgr(2314);
								//SetWindowText( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ), pText );								
								//return FALSE;
								break;
							}
						}
						
						// 내 물품 목록에 아이템이 올라가 있는지 체크
						pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
						
						for ( i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
						{
							if ( pTradeData->m_clItem[ i ].siUnique != 0 )
							{
								TCHAR* pText = GetTxtFromMgr(2314);
								//SetWindowText( GetDlgItem( hDlg, IDC_EDIT_EXPLAIN ), pText );
								//return FALSE;
								break;
							}
						}						
					}
					*/
					
					/*m_uiLastAcceptFrame = pclClient->GetFrame();	*/
					m_siLastAcceptTime	=	pclClient->CurrentClock	;
					(m_pParent->m_pNPTradeButton9)->Enable(false);
					m_pParent->PushAddItem();
					
					m_pParent->PrivateTradeOtherListReload();


				}
				break;
			}			

		}
		break;

	case NPTRADECHILD1_BUTTON2:   //입력
		{				
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: 
				{
                    m_pParent->PushPrice();
				}
				break;
			}		
		}
		break;

	case NPTRADECHILD1_BUTTON3:
		{			
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:   //입력옆의 물음표 
				{
					TCHAR* pText = GetTxtFromMgr(2315);
					
					(m_pParent->m_pNPTradeEdit)->SetText(pText);
					(m_pParent->m_pNPTradeEdit)->SetTextPrintOption(DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

				}
				break;
			}
		}
		break;


	}

	return;

}

void CNTradeChild1::LoadInventoryData()
{/*
	m_siSelectedIndex = -1;

	for ( SI32 i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		m_clItem[ i ] = m_pPersonItemInfo->clItem[ i ];
	}*/

	cltClient *pclclient = (cltClient*)pclClient;
	m_pPersonItemInfo =& pclclient->pclCM->CR[1]->pclCI->clCharItem;
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	m_siSelectIndex =-1;
	SI32 StartPos=15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
	SI32 id = 1;
	m_pNPTradeListView1->DeleteAllItems();
	m_pNPTradeListView1->SetStartItemPos( StartPos );	
	SI16 Counter = 0;
	TCHAR ItemName[128]=TEXT("");
	TCHAR buffer[ 256 ] = TEXT("");

      
		for ( SI32 i = 0; i < MAX_ITEM_PER_PERSON; ++i )
		{
			m_clUserItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );			
			if ( m_clUserItem[ i ].siUnique > 0 )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clUserItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clUserItem[ i ].siUnique );

					if ( ItemName )
					{
						TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
						pclClient->pclItemManager->GetExplain( &m_clUserItem[ i ], ToolTipMsg );

						if ( i >= StartPos && i < (StartPos + MAX_ITEM_PER_PERSON   )/*AX_ITEM_PER_INVENTORY*/ )
						{

							stListViewItemData ListViewItemData;
							ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_clUserItem[ i ].siItemNum, ItemName, ToolTipMsg );														
							m_pNPTradeListView1->InsertItem( &ListViewItemData );
							
						}
					}
				}
			}
		}//end for			

	return;
}


bool CNTradeChild1::IsInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, 10 부터 함.
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clUserItem[ j ];
		BYTE* dst = (BYTE*)&m_pPersonItemInfo->clItem[ j];

		for(int i = 0;i < size; i++)
		{
			if(src[i] != dst[i])
			{
				return true;
			}
		}
	}
	return false;
}







