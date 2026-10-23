#include "NPrivateRepairDlg.h"

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



#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/List.h"
#include "../../InterfaceMgr/interface/ListView.h"
#include "../../InterfaceMgr/interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"



#include "Client.h"

extern cltCommonLogic* pclClient;

stListBoxItemData prepairListBoxItemData;
stListBoxItemData prepair_otherListBoxItemData;
//CNPrivateRepairDlg g_NPrivateRepairDlg;

CNPrivateRepairDlg::CNPrivateRepairDlg()
{	
	m_pNTRepairStatic1 = NULL;
	m_pNTRepairStatic2 = NULL;
	m_pNTRepairStatic3 = NULL;
	m_pNTRepairList1 = NULL;
	m_pNTRepairList2 = NULL;
	m_pNTRepairEdit1 = NULL;
	m_pNTRepairEdit2 = NULL;
	m_pNTRepairEdit3 = NULL;
	m_pNTRepairEdit4 = NULL;
	m_pNTRepairListView = NULL;
	m_pNTRepairButton1 = NULL;
	//m_pNTRepairButton2 = NULL;
	m_pNTRepairButton3 = NULL;
	m_pNTRepairButton4 = NULL;
	m_pNTRepairButton5 = NULL;
	m_pNTRepairButton6 = NULL;
	m_pNTRepairButton7 = NULL;
	m_pNTRepairButton8 = NULL;


	
	SetActionTimer( 200);
	SetTimerOnOff( true );
	m_siBitmapAnimationIndex = 0;
	m_pPrivateTradeBufferMgr = NULL;
	m_pEachPersonTradeOrderInfo = new CEachPersonTradeOrderInfo();

	m_siInvSelectedIndex = -1;

}

CNPrivateRepairDlg::~CNPrivateRepairDlg()
{	
	
	SetTimerOnOff( false );

	SAFE_DELETE(m_pNTRepairStatic1);
	SAFE_DELETE(m_pNTRepairStatic2);
	SAFE_DELETE(m_pNTRepairStatic3);
	SAFE_DELETE(m_pNTRepairList1);
	SAFE_DELETE(m_pNTRepairList2);
	SAFE_DELETE(m_pNTRepairEdit1);
	SAFE_DELETE(m_pNTRepairEdit2);
	SAFE_DELETE(m_pNTRepairEdit3);
	SAFE_DELETE(m_pNTRepairEdit4);
	SAFE_DELETE(m_pNTRepairListView);
	SAFE_DELETE(m_pNTRepairButton1);
	//SAFE_DELETE(m_pNTRepairButton2);
	SAFE_DELETE(m_pNTRepairButton3);
	SAFE_DELETE(m_pNTRepairButton4);
	SAFE_DELETE(m_pNTRepairButton5);
	SAFE_DELETE(m_pNTRepairButton6);
	SAFE_DELETE(m_pNTRepairButton7);
	SAFE_DELETE(m_pNTRepairButton8);

	SAFE_DELETE(m_pPrivateTradeBufferMgr);
	SAFE_DELETE(m_pEachPersonTradeOrderInfo);
	


}


void CNPrivateRepairDlg::Create()
{
	if(!IsCreate())
	{

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NPrivateRepair/DLG_NPrivateRepair.ddf"));
		file.CreatePopupDialog( this, NPTRADEREPRAIR_DLG, TEXT("dialog_NTRepair"),StaticNPrivateRepairDlgProc);
		

		m_pNTRepairStatic1 = new CStatic(this);
		m_pNTRepairStatic2 = new CStatic(this);
		m_pNTRepairStatic3 = new CStatic(this);
		m_pNTRepairList1 = new CList(this);
		m_pNTRepairList2 = new CList(this);
		m_pNTRepairEdit1 = new CEdit(this);
		m_pNTRepairEdit2 = new CEdit(this);
		m_pNTRepairEdit3 = new CEdit(this);
		m_pNTRepairEdit4 = new CEdit(this);
		m_pNTRepairListView = new CListView(this);
		m_pNTRepairButton1 = new CButton(this);
		//m_pNTRepairButton2 = new CButton(this);
		m_pNTRepairButton3 = new CButton(this);
		m_pNTRepairButton4 = new CButton(this);
		m_pNTRepairButton5 = new CButton(this);
		m_pNTRepairButton6 = new CButton(this);
		m_pNTRepairButton7 = new CButton(this);
		m_pNTRepairButton8 = new CButton(this);

	
		file.CreateControl(m_pNTRepairStatic1, NPTRADEREPRAIR_STATIC1 , TEXT("statictext_NTRepair1"));	
		file.CreateControl(m_pNTRepairStatic2, NPTRADEREPRAIR_STATIC2 , TEXT("statictext_NTRepair2"));
		file.CreateControl(m_pNTRepairStatic3, NPTRADEREPRAIR_STATIC3 , TEXT("statictext_NTRepair3"));
		file.CreateControl(m_pNTRepairList1, NPTRADEREPRAIR_LISTBOX1 , TEXT("listbox_NTRepair1"));
		file.CreateControl(m_pNTRepairList2, NPTRADEREPRAIR_LISTBOX2 , TEXT("listbox_NTRepair2"));
		file.CreateControl(m_pNTRepairEdit1, NPTRADEREPRAIR_EDIT1 , TEXT("editbox_NTRepair1"));
		file.CreateControl(m_pNTRepairEdit2, NPTRADEREPRAIR_EDIT2 , TEXT("editbox_NTRepair2"));
		file.CreateControl(m_pNTRepairEdit3, NPTRADEREPRAIR_EDIT3 , TEXT("editbox_NTRepair3"));
		file.CreateControl(m_pNTRepairEdit4, NPTRADEREPRAIR_EDIT4 , TEXT("editbox_NTRepair4"));		
		file.CreateControl(m_pNTRepairListView , NPTRADEREPRAIR_LISTVIEW , TEXT("listview_NTRepair") ); 
		file.CreateControl(m_pNTRepairButton1 , NPTRADEREPRAIR_BUTTON1 , TEXT("button_NTRepair1") );
		//file.CreateControl(m_pNTRepairButton2 , NPTRADEREPRAIR_BUTTON2 , TEXT("button_NTRepair2") );
		file.CreateControl(m_pNTRepairButton3 , NPTRADEREPRAIR_BUTTON3 , TEXT("button_NTRepair3") );
		file.CreateControl(m_pNTRepairButton4 , NPTRADEREPRAIR_BUTTON4 , TEXT("button_NTRepair4") );
		file.CreateControl(m_pNTRepairButton5 , NPTRADEREPRAIR_BUTTON5 , TEXT("button_NTRepair5") );
		file.CreateControl(m_pNTRepairButton6 , NPTRADEREPRAIR_BUTTON6 , TEXT("button_NTRepair6") );
		file.CreateControl(m_pNTRepairButton7 , NPTRADEREPRAIR_BUTTON7 , TEXT("button_NTRepair7") );
		file.CreateControl(m_pNTRepairButton8 , NPTRADEREPRAIR_BUTTON8 , TEXT("button_NTRepair8") );
		
		
		m_pPersonItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;

		if ( m_pPrivateTradeBufferMgr == NULL )
		{
			m_pPrivateTradeBufferMgr = new CLibListMgr();
			m_pPrivateTradeBufferMgr->CreateMaxEmptyObj( MAX_PRIVATETRADE_OBJ_NUM, MAX_PRIVATETRADE_OBJ_BUFFER_SIZE );
		}

		m_pNTRepairList1->SetBorder(true);
		m_pNTRepairList1->SetBKColorUse(true);
		m_pNTRepairList2->SetBorder(true);
		m_pNTRepairList2->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr(2334);
		m_pNTRepairList1->SetColumn(0,185,pText);
		m_pNTRepairList1->Refresh();
		pText = GetTxtFromMgr(2335);
        m_pNTRepairList2->SetColumn(0,185,pText);
		m_pNTRepairList2->Refresh();

		pText = GetTxtFromMgr(3411);
		m_pNTRepairEdit1->SetText(pText);
		m_pNTRepairListView->SetBKColorUse(true);
		m_pNTRepairListView->SetBorder(true);	

		m_pNTRepairEdit4->SetNumberMode( EDIT_INPUT_MODE_NUMBER );

/*
		
		
		m_bShow = false;

		return;
		*/
	}


}

void CNPrivateRepairDlg::Action()
{
	
	//if(m_siSelectedModeIndex==PRIVATETRADE_REPAIR_MODE)
	{
			
		m_pNTRepairStatic1->SetFileName(TEXT("Interface/PrivateTrade/repair.SPR"));
		m_pNTRepairStatic1->SetFontIndex(m_siBitmapAnimationIndex++);
		if ( m_siBitmapAnimationIndex >= 3 )
			m_siBitmapAnimationIndex = 0;

	}
	

	if ( m_bMyAccept == false && TABS( pclClient->GetFrame() - m_uiLastAcceptFrame ) > 60 )			
	{
		m_pNTRepairButton7->Enable(true);		
	}
		
	if (IsInventoryChanged())
	{	
		LoadInventoryData();
	}
}

void CALLBACK CNPrivateRepairDlg::StaticNPrivateRepairDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPrivateRepairDlg *pThis = (CNPrivateRepairDlg*) pControllerMgr;
	pThis->NPrivateRepairDlgProc( nEvent, nControlID, pControl );
}
	
void CALLBACK CNPrivateRepairDlg::NPrivateRepairDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch (nControlID  )
	{	
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			PushCancel();
			//DeleteDialog();
		}
		break;
	case NPTRADEREPRAIR_LISTBOX1:
		{			
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					PrivateTradeOtherListInfo( m_pNTRepairList1->GetSelectedIndex() );					

				}
				break;
			}
		}
		break;
	case NPTRADEREPRAIR_LISTBOX2:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					PrivateTradeMyListInfo( m_pNTRepairList2->GetSelectedIndex() );			

				}
				break;
			}

		}
		break;


	case NPTRADEREPRAIR_LISTVIEW:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					
					TCHAR buffer[256]= TEXT("") ;
					m_siInvSelectedIndex= m_pNTRepairListView->GetSelectedItemIndex();
					if ( m_siInvSelectedIndex >= 0 )
						{
							stListViewItemData *pItemData = m_pNTRepairListView->GetListViewItemData(m_siInvSelectedIndex- PERSONITEM_INV0);
							if ( pItemData )
							{	
								StringCchPrintf( buffer, 256, TEXT("%d"),pItemData->ItemNum );								
							}
						}			

				}
				break;
			}

		}
		break;

	case NPTRADEREPRAIR_BUTTON1:  // 귓속말 보내기 
		{			
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:
				{								
					if ( m_siOtherCharUnique > 0 )
					{
						cltClient *pclclient = ( cltClient * )pclClient;

						if ( m_strOtherCharName != NULL && _tcslen( m_strOtherCharName ) > 0 )
						{							
							pclclient->RequestSecretChatByCharUnique(m_siOtherCharUnique);
						}
						else
						{							
							pclclient->RequestSecretChatByCharUnique(m_siOtherCharUnique);
						}
					}
				}
				break;
			}
		}
		break;
/*
	case NPTRADEREPRAIR_BUTTON2: //꼼지가기 
		{
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:
				{										
					if ( m_siOtherCharUnique > 0 )
					{
						cltGameMsgRequest_Userid clUserid( m_siOtherCharUnique );
						cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
						cltClient *pclclient = ( cltClient * )pclClient;
						pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
					}
				}
				break;
			}
		}
		break;
*/
	case NPTRADEREPRAIR_BUTTON3: //선택 물품 올리기 
		{
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:
				{
					m_uiLastAcceptFrame = pclClient->GetFrame();	
					
					m_pNTRepairButton7->Enable(false);
					PushAddItem();					
					PrivateTradeOtherListReload();

				}
				break;
			}
		}
		break;

	case NPTRADEREPRAIR_BUTTON4:  //선택 물품 내리기 
		{
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:
				{						
					m_uiLastAcceptFrame = pclClient->GetFrame();
					m_pNTRepairButton7->Enable(false);
					//EnableWindow( m_hMyAcceptButton, false );

					PushDelList();

				}
				break;
			}
		}
		break;

	case NPTRADEREPRAIR_BUTTON5: // 입력 
		{
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:
				{
					PushPrice();
				}
				break;
			}
		}
		break;

	case NPTRADEREPRAIR_BUTTON6:    //입력 옆의 물음표 
		{
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:  
				{						
					TCHAR* pText = GetTxtFromMgr(2315);					
					m_pNTRepairEdit1->SetText(pText);
					m_pNTRepairEdit1->SetTextPrintOption(DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

				}
				break;
			}
		}
		break;

	case NPTRADEREPRAIR_BUTTON7:   //수락하기 
		{
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_bMyAccept )
					{
						PushMyCancel();
					}
					else
					{
						if ( TABS( pclClient->GetFrame() - m_uiLastAcceptFrame ) < 60 )
						{
							break;							
						}

						PushMyAccept();
					}
				}
				break;
			}
		}
		break;

	case NPTRADEREPRAIR_BUTTON8:
		{
			switch( nEvent )
			{				
			case EVENT_BUTTON_CLICKED:
				{					
					TCHAR* pText = GetTxtFromMgr(2327);						
					m_pNTRepairEdit1->SetText(pText);					
					
				}
				break;
			}
		}
		break;

	}
		


	return;

}

void CNPrivateRepairDlg::Show( SI32 siOtherCharUnique, SI16 TradeMode, SI16 Level, TCHAR *Name, TCHAR *AccountID, SI16 siPrivateTradeOrderMgrIndex, SI16 siPrivateTradeOrderCountIndex, SI32 OtherCharLevel )
{
	// PrivateTrdeOrderMgr로 부터 할당받지 못한 경우
	// siOtherCharUnique 가 잘못된 값인 경우
	cltClient *pclclient = (cltClient *)pclClient;


	if ( siPrivateTradeOrderMgrIndex < 1 || siOtherCharUnique < 1 )		
	{
		return;
	}


	
	if( pclclient->bWatchModeSwitch )
	{
		return;
	}

	m_siOtherCharUnique = siOtherCharUnique;

	m_siOtherCharLevel = Level;

	m_siSelectedModeIndex = TradeMode;

	TCHAR Buffer[256 ] = TEXT("");

	m_siBitmapAnimationIndex = 0;

	
	//if ( TradeMode == PRIVATETRADE_REPAIR_MODE )
	{
		m_siBitmapAnimationIndex = 3;
		if ( Name != NULL && _tcslen( Name ) > 0 )
		{
			TCHAR* pText = GetTxtFromMgr(2332);
			StringCchPrintf( Buffer, 256, pText, Name, OtherCharLevel );
			SetTitle(Buffer);
			MStrCpy( m_strOtherCharName, Name, MAX_PLAYER_NAME );
		}


		else
		{
			TCHAR* pText = GetTxtFromMgr(2333);
			//SetWindowText( m_hDlg, pText );			
		}
	
	}
	

	// 내 계정 아이디
	if ( pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID == NULL || _tcslen ( pclClient->pclCM->CR[1]->pclCI->clBI.szAccountID ) < 1 )
	{
		return;
	}



	// 다른 사람 계정 아이디
	if ( AccountID == NULL || _tcslen( AccountID ) < 1 )
	{
		return;
	}

	m_siAccpetCounter = 0;

	m_siPrivateTrdeOrderMgrIndex = siPrivateTradeOrderMgrIndex;
	m_siPrivateTrdeOrderCountIndex = siPrivateTradeOrderCountIndex;

	
	m_pNTRepairEdit4->SetText(TEXT("0"));

	m_bMyAccept = false;

	m_pNTRepairButton5->Enable(true);
	m_pNTRepairEdit4->Enable(true);

	// 돈 입력 버튼 활성화
	m_pNTRepairButton5->Enable(true);	
	// 돈 입력 에디트박스 활성화
	m_pNTRepairEdit2->Enable(true);

	m_pNTRepairButton7->Enable(true);
	//TCHAR* pText = GetTxtFromMgr(2336);	
	//m_pNTRepairStatic2->SetText(pText,DT_WORDBREAK);

	//TCHAR TradeBaseInfo[ 256 ] = TEXT("");
	//pText = GetTxtFromMgr(2337);
	//StringCchPrintf( TradeBaseInfo, 256, pText, MAX_PRIVATETRADEORDER_PRICE, MAX_PRIVATETRADEORDER_STOCK_NUM );

	
	m_siMyPrivateTradeListSelectedIndex = -1;

	SetActionTimer(200);		
	m_bShow = true;


	return ;

}


void CNPrivateRepairDlg::PushAddItem()
{
	if(m_siInvSelectedIndex < 0) return;

	SI32 m_siSelectedIndex = m_siInvSelectedIndex;
	m_clItem[m_siSelectedIndex] =m_clUserItem[m_siSelectedIndex];


	if( m_siSelectedIndex < 0 )
	{
		return;
	}
	

	//if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
	{
		if ( m_clItem[ m_siSelectedIndex ].CanRepair( pclClient->pclItemManager ) == false )
		{
			TCHAR* pText = GetTxtFromMgr(2348);
			m_pNTRepairEdit1->SetText(pText);			
			return;
		}

		SI32 curdur = m_clItem[ m_siSelectedIndex ].GetDurability(  );
		SI32 maxdur = m_clItem[ m_siSelectedIndex ].GetMaxDurability( pclClient->pclItemManager );
		
		if ( curdur >= maxdur )
		{
			TCHAR* pText = GetTxtFromMgr(2349);
			m_pNTRepairEdit1->SetText(pText);
			return;
		}
	}
	

	SI32 AddItemNum=1;  //어차피 무기는 하나 밖에 없으므로 1로 지정했음 ^^;; 


	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_Add_Item sAddItem( m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siSelectedIndex,
		&m_clItem[ m_siSelectedIndex ],
		AddItemNum
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sAddItem, sAddItem.sHeader.uiSize );
	}	
	m_pNTRepairButton7->Enable(false);
	m_pNTRepairButton3->Enable(false);

	return;
}



void CNPrivateRepairDlg::PushMyAccept()
{
	cltClient *pclclient = (cltClient *)pclClient;

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
	
	//////////////////////////////////////////////////////////////////////////	
	// 말 거래시에 말 인벤토리에 아이템이 들어 있는지 체크
	if ( pTradeData->m_siHorseIndex != -1 )
	{
		if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.IsHorseHaveItem() == true )
		{
			TCHAR* pTitle = GetTxtFromMgr(2132);
			TCHAR* pText = GetTxtFromMgr(2133);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
			return;
		}
	}
	//////////////////////////////////////////////////////////////////////////	


	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();
	
	stPrivateTrade_MyAccept sMyAccept( m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siAccpetCounter
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sMyAccept, sMyAccept.sHeader.uiSize );
	}

	return;
}

void CNPrivateRepairDlg::PushMyCancel()
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_MyCancel sMyCancel
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sMyCancel, sMyCancel.sHeader.uiSize );
	}


	return;
}


bool CNPrivateRepairDlg::IsShow()
{
	return m_bShow;
}



void CNPrivateRepairDlg::ClearPrivateTradeList()
{
	if ( m_pEachPersonTradeOrderInfo != NULL )
	{
		m_pEachPersonTradeOrderInfo->Initialize();
		m_pNTRepairList1->Clear();
		m_pNTRepairList2->Clear();
	}

	return;
}



void CNPrivateRepairDlg::PrivateTradeOtherListReload()
{
	
	TCHAR Buffer[256 ] = TEXT("");
	SI16 siCount = 0;
	
	 m_pNTRepairList1->Clear();
	
	//m_pNTRepairList1->SetDrawLine( true );
	//m_pNTRepairList1->Refresh();

//내가 보낼물품
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
	
	if ( pTradeData->m_siPrice > 0 )
	{		
		TCHAR* pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,Buffer, 256, pText);
		prepairListBoxItemData.Init();									
		prepairListBoxItemData.Set(0,Buffer);	
		prepairListBoxItemData.siParam[0] =0;
		prepairListBoxItemData.siSmallIconFileIndex[0] = 2;
		prepairListBoxItemData.siSmallIconFontIndex[0] = 4;
		SI32 index = m_pNTRepairList1->AddItem( &prepairListBoxItemData );		
		
	}
	TCHAR ItemName[ 128 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		m_clItem[i] =m_clUserItem[i];
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{
			
				if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
				{
					TCHAR* pText = GetTxtFromMgr(2339);
					StringCchPrintf( Buffer, 256, pText, ItemName, pTradeData->m_clItem[ i ].GetItemNum() );
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr(2340);
					StringCchPrintf( Buffer, 256, pText, ItemName );
				}
				

				if ( !_tcscmp(Buffer,TEXT("")) ) continue;
				prepairListBoxItemData.Init();									
				prepairListBoxItemData.Set(0,Buffer);	
				prepairListBoxItemData.siParam[0] =i;
				prepairListBoxItemData.siSmallIconFileIndex[0] = 2;
				prepairListBoxItemData.siSmallIconFontIndex[0] = 5;
				SI32 index = m_pNTRepairList1->AddItem( &prepairListBoxItemData );					
			
			}
		}
	}
	
	m_pNTRepairEdit2->SetText(TEXT(" "));
	m_pNTRepairList1->Refresh();
	


}

void CNPrivateRepairDlg::PrivateTradeMyListReload()
{
	
	m_pNTRepairList2->Clear();

	TCHAR Buffer[256 ] = TEXT("");

		
	SI16 siCount = 0;
	
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		
		TCHAR* pText = GetTxtFromMgr(611);
		g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,Buffer, 256, pText);
		prepairListBoxItemData.Init();									
		prepairListBoxItemData.Set(0,Buffer);	
		prepairListBoxItemData.siParam[0] =0;
		prepairListBoxItemData.siSmallIconFileIndex[0] = 2;
		prepairListBoxItemData.siSmallIconFontIndex[0] = 4;
	
		SI32 index = m_pNTRepairList2->AddItem( &prepairListBoxItemData  );
		
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{
			

				if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
				{
					TCHAR* pText = GetTxtFromMgr(2339);
					StringCchPrintf( Buffer, 256, pText, ItemName, pTradeData->m_clItem[ i ].GetItemNum() );
				}
				else
				{
					TCHAR* pText = GetTxtFromMgr(2340);
					StringCchPrintf( Buffer, 256, pText, ItemName );
				}

				if (!_tcscmp(Buffer,TEXT("")) ) continue;
				prepairListBoxItemData.Init();									
				prepairListBoxItemData.Set(0,Buffer);	
				prepairListBoxItemData.siParam[0] =i;
				prepairListBoxItemData.siSmallIconFileIndex[0] = 2;
				prepairListBoxItemData.siSmallIconFontIndex[0] = 5;
			
				SI32 index = m_pNTRepairList2->AddItem( &prepairListBoxItemData  );
		
			}
		}
	}//end for 

	m_pNTRepairList2->Refresh();
	m_pNTRepairEdit3->SetText(TEXT(" "));
	
	return;
}

void CNPrivateRepairDlg::PrivateTradeOtherListInfo( SI32 SelectedListIndex )
{
	
	SI16 siCount = 0;

	TCHAR Buffer[ 1024 ] = TEXT("");


	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		if ( siCount == SelectedListIndex )
		{
			TCHAR moneybuf[512] = TEXT("") ;
			g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,moneybuf, 512);
			TCHAR* pText = GetTxtFromMgr(2343);
			StringCchPrintf( Buffer, 1024, pText, moneybuf );		
			m_pNTRepairEdit2->SetText(Buffer);


			return;
		}

		++siCount;
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	TCHAR ItemExplain[ 512 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{				
				if ( siCount == SelectedListIndex )
				{
					if ( pclClient->pclItemManager->GetExplain( &pTradeData->m_clItem[ i ], ItemExplain ) == TRUE )
					{
						StringCchPrintf( Buffer, 1024, TEXT("%s"), ItemExplain );
						m_pNTRepairEdit2->SetText(Buffer);
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(2344);
						StringCchPrintf( Buffer, 1024, pText, ItemName, pTradeData->m_clItem[ i ].siItemNum );
						m_pNTRepairEdit2->SetText(Buffer);
					}	
	
					
					return;
				}

				++siCount;
			}
		}
	}
	

	if ( pTradeData->m_siStockVillageUnique > 0 && pTradeData->m_siStockNum )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siStockVillageUnique );

		if ( VillageName != NULL )
		{
			if ( siCount == SelectedListIndex )
			{
				TCHAR* pText = GetTxtFromMgr(2345);
				StringCchPrintf( Buffer, 1024, pText, VillageName, pTradeData->m_siStockNum );
				m_pNTRepairEdit2->SetText(Buffer);
				return;
			}
			
			siCount++;
		}
	}


	if ( pTradeData->m_siHouseVillageUnique > 0 )
	{
		if ( siCount == SelectedListIndex )
		{
			pTradeData->m_clRentContract.GetRentInfoText( Buffer , 1024);
			m_pNTRepairEdit2->SetText(Buffer);		
			return;
		}
		++siCount;
	}


	if ( pTradeData->m_siHorseIndex != -1 )
	{
		if ( pclClient->pclHorseManager->GetText( &pTradeData->m_clTradeHorse, Buffer, 1024 ) )
		{
			if ( siCount == SelectedListIndex )
			{
				m_pNTRepairEdit2->SetText(Buffer);
				
				return;
			}			
			++siCount;
		}
	}

	m_pNTRepairList1->Refresh();

	return;
}

void CNPrivateRepairDlg::PrivateTradeMyListInfo( SI32 SelectedListIndex )
{
	


	TCHAR Buffer[ 1024 ] = TEXT("");

	SI16 siCount = 0;
	
	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		TCHAR moneybuf[512] = TEXT("") ;
		//sprintf( Buffer, TEXT("거래 금액\r\n%d 냥"), pTradeData->m_siPrice );
		g_JYLibFunction.SetNumUnit(pTradeData->m_siPrice,moneybuf, 512);
		TCHAR* pText = GetTxtFromMgr(2343);
		StringCchPrintf( Buffer, 1024, pText, moneybuf );
		m_pNTRepairEdit3->SetText(Buffer);
		++siCount;
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	TCHAR ItemExplain[ 512 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{	
				if ( siCount == SelectedListIndex )
				{
					if ( pclClient->pclItemManager->GetExplain( &pTradeData->m_clItem[ i ], ItemExplain ) == TRUE )
					{
						StringCchPrintf( Buffer, 1024, TEXT("%s"), ItemExplain );
					}
					else
					{
						TCHAR* pText = GetTxtFromMgr(2344);
						StringCchPrintf( Buffer, 1024, pText, ItemName, pTradeData->m_clItem[ i ].siItemNum );
					}
					
					m_pNTRepairEdit3->SetText(Buffer);
					
					return;
				}

				++siCount;
			}
		}
	}

	if ( pTradeData->m_siStockVillageUnique > 0 && pTradeData->m_siStockNum )
	{
		TCHAR *VillageName = pclClient->pclVillageManager->GetName( pTradeData->m_siStockVillageUnique );

		if ( VillageName != NULL )
		{
			if ( siCount == SelectedListIndex )
			{
				TCHAR* pText = GetTxtFromMgr(2345);
				StringCchPrintf( Buffer, 1024, pText, VillageName, pTradeData->m_siStockNum );
				m_pNTRepairEdit3->SetText(Buffer);

				return;
			}
			
			++siCount;
		}
	}

	if ( pTradeData->m_siHouseVillageUnique > 0 )
	{
		if ( siCount == SelectedListIndex )
		{
			pTradeData->m_clRentContract.GetRentInfoText( Buffer , 1024);
			m_pNTRepairEdit3->SetText(Buffer);
			return;
		}

		++siCount;
	}


	if ( pTradeData->m_siHorseIndex != -1 )
	{
		if ( pclClient->pclHorseManager->GetText( &pTradeData->m_clTradeHorse, Buffer, 1024 ) )
		{
			if ( siCount == SelectedListIndex )
			{
				m_pNTRepairEdit3->SetText(Buffer);
				return;
			}
			
			++siCount;
		}
	}	

	return;
}

void CNPrivateRepairDlg::PrivateTradeAddItem( SI32 siCharUnique, SI32 siAddedIndex, cltItem *pclItem )
{
	
	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 목록에 올렸음
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairButton7->Enable(false);
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		pTradeData->m_clItem[ siAddedIndex ].Set( pclItem );
		
		PrivateTradeOtherListReload();

		TCHAR ItemExplain[ 512 ] = TEXT("");
		if ( pclClient->pclItemManager->GetExplain( pclItem, ItemExplain ) == TRUE )
		{			
			m_pNTRepairEdit2->SetText(ItemExplain);			
		}

		return;
	}
	else											// 내가 내 목록에 올렸음.
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairButton7->Enable(false);		

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		pTradeData->m_clItem[ siAddedIndex ].Set( pclItem );		

		PrivateTradeMyListReload();

		TCHAR ItemExplain[ 512 ] = TEXT("");
		if ( pclClient->pclItemManager->GetExplain( pclItem, ItemExplain ) == TRUE )
		{			
			m_pNTRepairEdit3->SetText(ItemExplain);			
		}

		return;
	}

	return;
}

void CNPrivateRepairDlg::PrivateTradeDelList( SI32 CharUnique, SI16 DelType, SI16 Param )
{
	
	if ( m_siOtherCharUnique == CharUnique )		// 상대방이 목록에 내렸음
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairButton7->Enable(false);	

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();

		switch( DelType )
		{
		case PRIVATETRADE_TYPE_MONEY:
			pTradeData->m_siPrice = 0;
			break;
		case PRIVATETRADE_TYPE_ITEM:
			if ( pTradeData->m_clItem[ Param ].siUnique != 0 )
			{
				memset( &pTradeData->m_clItem[ Param ], 0, sizeof( cltItem ) );			
			}
			break;
		case PRIVATETRADE_TYPE_STOCK:
			pTradeData->m_siStockVillageUnique = 0;
			pTradeData->m_siStockNum = 0;
			break;
		case PRIVATETRADE_TYPE_HOUSEUNIT:
			pTradeData->m_siHouseVillageUnique = 0;
			pTradeData->m_siHouseUnitSlot = 0;
			break;
		case PRIVATETRADE_TYPE_HORSE:
			pTradeData->m_siHorseIndex = -1;
			pTradeData->m_clTradeHorse.Init();
			break;
		}

		PrivateTradeOtherListReload();
	}
	else
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairButton7->Enable(false);
	

		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
	
		switch( DelType )
		{
		case PRIVATETRADE_TYPE_MONEY:
			pTradeData->m_siPrice = 0;
			break;
		case PRIVATETRADE_TYPE_ITEM:
			if ( pTradeData->m_clItem[ Param ].siUnique != 0 )
			{
				memset( &pTradeData->m_clItem[ Param ], 0, sizeof( cltItem ) );			
			}
			break;
		case PRIVATETRADE_TYPE_STOCK:
			pTradeData->m_siStockVillageUnique = 0;
			pTradeData->m_siStockNum = 0;
			break;
		case PRIVATETRADE_TYPE_HOUSEUNIT:
			pTradeData->m_siHouseVillageUnique = 0;
			pTradeData->m_siHouseUnitSlot = 0;
			break;
		case PRIVATETRADE_TYPE_HORSE:
			pTradeData->m_siHorseIndex = -1;
			pTradeData->m_clTradeHorse.Init();
			break;
		}
		
		PrivateTradeMyListReload();
	}
	
	return;
}

void CNPrivateRepairDlg::PrivateTradeDelItem( SI32 siCharUnique, SI16 siDelIndex )
{
	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 목록에 내렸음
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairButton7->Enable(false);
	
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		
		if ( pTradeData->m_clItem[ siDelIndex ].siUnique != 0 )
		{
			memset( &pTradeData->m_clItem[ siDelIndex ], 0, sizeof( cltItem ) );			
		}

		//SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_PRIVATETRADE_THEOTHER_ITEM, NULL, pTradeData->m_clItem, MAX_PRIVATETRADEORDER_ITEM_NUM, true );
		PrivateTradeOtherListReload();
		return;
	}
	else											// 내가 내 목록에서 내렸음.
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairButton7->Enable(false);
	
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		
		if ( pTradeData->m_clItem[ siDelIndex ].siUnique != 0 )
		{
			memset( &pTradeData->m_clItem[ siDelIndex ], 0, sizeof( cltItem ) );
		}

		//SetInventoryInListView( pclClient->pclItemManager, m_hDlg, IDC_LIST_PRIVATETRADE_MY_ITEM, NULL, pTradeData->m_clItem, MAX_PRIVATETRADEORDER_ITEM_NUM, true );
		PrivateTradeMyListReload();
		return;
	}

	return;
}

void CNPrivateRepairDlg::PrivateTradeAddPrice( SI32 siCharUnique, GMONEY siPrice )
{
	/*
	if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
	{
		TCHAR* pText = GetTxtFromMgr(2346);
		m_pNTRepairEdit1->SetText(pText);	
	}
	*/

	if ( m_siOtherCharUnique == siCharUnique )		// 상대방이 돈 설정했음.
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairButton7->Enable(false);
	
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson2TradeData();
		pTradeData->m_siPrice = siPrice;

		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PRIVATETRADE_OTHER_PRICE ), buffer );
		PrivateTradeOtherListReload();
	}
	else											// 내가 돈 설정 했음.
	{
		m_uiLastAcceptFrame = pclClient->GetFrame();
		m_pNTRepairEdit1->Enable(false);
		
		stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();
		pTradeData->m_siPrice = siPrice;

		//SetWindowText( GetDlgItem( m_hDlg, IDC_STATIC_PRIVATETRADE_MY_PRICE ), buffer );
		PrivateTradeMyListReload();
	}
}

void CNPrivateRepairDlg::PrivateTradeMyAccept()
{
	++m_siAccpetCounter;
	m_bMyAccept = true;	
	
	m_uiLastAcceptFrame = 0;
	m_pNTRepairButton7->Enable(false);
	TCHAR* pText = GetTxtFromMgr(3902);
	m_pNTRepairButton7->SetText(pText, DT_CENTER|DT_VCENTER|DT_SINGLELINE);	

}

void CNPrivateRepairDlg::PrivateTradeMyCancel()
{
	++m_siAccpetCounter;
	m_bMyAccept = false;
	
	m_uiLastAcceptFrame = pclClient->GetFrame();
	m_pNTRepairButton7->Enable(false);

	TCHAR* pText = GetTxtFromMgr(3412);
	m_pNTRepairButton7->SetText(pText, DT_CENTER|DT_VCENTER|DT_SINGLELINE );	
	//if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
	{
		//TCHAR* pText = GetTxtFromMgr(2336);
		m_pNTRepairStatic2->SetText(pText,DT_WORDBREAK);		
	}

}

void CNPrivateRepairDlg::PrivateTradeOtherAccept( SI16 siResult )
{
	m_uiLastAcceptFrame = pclClient->GetFrame();
	m_pNTRepairButton7->Enable(false);

	++m_siAccpetCounter;

	//if ( m_siSelectedModeIndex == PRIVATETRADE_REPAIR_MODE )
	{
		TCHAR* pText = GetTxtFromMgr(2347);
		m_pNTRepairStatic2->SetText(pText,DT_WORDBREAK);	
	}

	return;
}

void CNPrivateRepairDlg::PrivateTradeOtherCancel()
{
	m_uiLastAcceptFrame = pclClient->GetFrame();
	m_pNTRepairButton7->Enable(false);

	++m_siAccpetCounter;

	if ( m_siSelectedModeIndex == PRIVATETRADE_TRADE_MODE )
	{
		//TCHAR* pText = GetTxtFromMgr(2336);
		//m_pNTRepairStatic2->SetText(pText,DT_WORDBREAK);		
	}


	m_bMyAccept = false;
	TCHAR* pText = GetTxtFromMgr(3412);
	m_pNTRepairButton7->Enable(true);
	m_pNTRepairButton7->SetText(pText, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

void CNPrivateRepairDlg::PrivateTradeEnd( SI32 siResult, 
									   SI32 RecvStockVillageUnique, SI32 RecvStockNum, 
									   SI32 SendStockVillageUnique, SI32 SendStockNum, 
									   SI32 RecvHouseVillageUnique, SI32 RecvHouseUnitSlot, 
									   cltRentContract *pclRecvRentContract,
									   SI32 SendHouseVillageUnique, SI32 SendHouseUnitSlot, 
									   SI16 HorseIndex, cltHorse *pclHorse )
{
	
		
	DeleteDialog();
	return;
}


void CNPrivateRepairDlg::PushCancel()
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_Cancel sCancel( m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex );

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sCancel, sCancel.sHeader.uiSize );
	}

	return;
}


bool CNPrivateRepairDlg::GetDelType( SI32 SelectedListIndex, SI16 *pType, SI16 *pParam )
{	
	cltClient *pclclient = (cltClient *)pclClient;

	SI16 siCount = 0;

	TCHAR Buffer[ 1024 ] = TEXT("");

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	if ( pTradeData->m_siPrice > 0 )
	{
		if ( siCount == SelectedListIndex )
		{			
			*pType = PRIVATETRADE_TYPE_MONEY;
			
			TCHAR* pTitle = GetTxtFromMgr(2350);
			TCHAR* pText = GetTxtFromMgr(2351);
			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );

			return false;
			
			//return true;		//<--- 과거에는 돈을 지울 수 있었지만, 이제 안됨
		}

		++siCount;
	}

	TCHAR ItemName[ 128 ] = TEXT("");
	TCHAR ItemExplain[ 512 ] = TEXT("");
	for ( SI32 i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; ++i )
	{
		if ( pTradeData->m_clItem[ i ].siUnique > 0 )
		{
			if ( pTradeData->m_clItem[ i ].GetFullName( pclClient->pclItemManager, ItemName, 128 ) )
			{				
				if ( siCount == SelectedListIndex )
				{
					*pType = PRIVATETRADE_TYPE_ITEM;
					*pParam = i;
					return true;
				}
			}
			
			++siCount;
		}
	}

	
	return false;
}

void CNPrivateRepairDlg::PushDelList()

{

	m_siMyPrivateTradeListSelectedIndex= m_pNTRepairList2->GetSelectedIndex();
	if ( m_siMyPrivateTradeListSelectedIndex < 0 )
	{
		return;
	}

	SI16 Type = 0;
	SI16 Param = 0;

	if ( ! GetDelType( m_siMyPrivateTradeListSelectedIndex, &Type, &Param ) )
	{
		return;
	}

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	stPrivateTrade_Del_List sDelList
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		Type,
		Param
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sDelList, sDelList.sHeader.uiSize );
	}

	m_pNTRepairButton3->Enable(true);
	return;
	
}

void CNPrivateRepairDlg::PushDelItem()
{
	if ( m_siMyPrivateTradeListSelectedIndex < 0 )
	{
		return;
	}
	
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stTradeData *pTradeData = m_pEachPersonTradeOrderInfo->GetPerson1TradeData();

	stPrivateTrade_Del_Item sDelItem
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siMyPrivateTradeListSelectedIndex,
		&pTradeData->m_clItem[ m_siMyPrivateTradeListSelectedIndex ]
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sDelItem, sDelItem.sHeader.uiSize );
	}

	
	
	return;
}


void CNPrivateRepairDlg::PushPrice()
{
	cltClient *pclclient = (cltClient *)pclClient;

	GMONEY m_siPrice = _tstoi64(m_pNTRepairEdit4->GetText());
	

	if ( m_siPrice < 1 )
	{
		return;
	}

	if ( m_siPrice > pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit") )
	{
		TCHAR Buffer[256 ] = TEXT("");
		TCHAR* pText = GetTxtFromMgr(2357);
		StringCchPrintf( Buffer, 256, pText, pclClient->GetGlobalValue("GV_PrivateTradeMoneyLimit") );
		TCHAR* pTitle = GetTxtFromMgr(2358);
		MessageBox( NULL, pTitle, Buffer, MB_OK|MB_TOPMOST );
		return;
	}

	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToEmptyListAndPushBackToExistList();

	stPrivateTrade_Price sPrice
		(
		m_siOtherCharUnique,
		m_siPrivateTrdeOrderMgrIndex,
		m_siPrivateTrdeOrderCountIndex,
		m_siPrice
		);

	if ( pTemp )
	{
		pTemp->SetData( ( BYTE * )&sPrice, sPrice.sHeader.uiSize );
		
		TCHAR* pTitle = GetTxtFromMgr(2359);
		TCHAR* pText = GetTxtFromMgr(2360);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
		//EnableWindow( GetDlgItem( m_hMoneyItemDlg, IDC_BUTTON_PRIVATETRADE_PRICE ), FALSE );
		//EnableWindow( GetDlgItem( m_hMoneyItemDlg, IDC_EDIT_PRIVATETRADE_MONEY ), FALSE );
	}

	return;
}


bool CNPrivateRepairDlg::Pop( BYTE *pData )
{
	CObj *pTemp = m_pPrivateTradeBufferMgr->PopFrontToExistListAndPushBackToEmptyList();

	if ( pTemp )
	{
		UI32 uiDataSize;
		pTemp->GetData( (BYTE*)pData, &uiDataSize );
		return true;
	}

	return false;
}



void CNPrivateRepairDlg::LoadInventoryData()
{

	cltClient *pclclient = (cltClient*)pclClient;
	m_pPersonItemInfo =& pclclient->pclCM->CR[1]->pclCI->clCharItem;
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 m_siSelectIndex =-1;
	SI32 StartPos=15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
	SI32 id = 1;
	m_pNTRepairListView->DeleteAllItems();
	m_pNTRepairListView->SetStartItemPos( StartPos );	
	SI16 Counter = 0;
	TCHAR ItemName[128]= TEXT("");
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

						if ( i >= StartPos && i < (StartPos + MAX_ITEM_PER_PERSON ) &&  m_clUserItem[i].CanRepair( pclClient->pclItemManager )/*AX_ITEM_PER_INVENTORY*/ )
						{

							stListViewItemData ListViewItemData;
							ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_clUserItem[ i ].siItemNum, ItemName, ToolTipMsg );														
							m_pNTRepairListView->InsertItem( &ListViewItemData );
							
						}
					}
				}
			}
		}//end for			

	return;
}


bool CNPrivateRepairDlg::IsInventoryChanged()
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

