#include "NNPCTradeDlg.h"
#include "../../interface/NPC1/NPCTradeBuffer.h"
#include "../../FontManager/FontManager.h"
#include "../../../Client/Music/Music.h"
#include "../../../Client/Client.h"

#include "Char\CharManager\CharManager.h"

#include "../../../CommonLogic/Msg/MsgType-NPC.h"
#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../../Common/SystemNPC/SystemNPC.h"
#include "../../../Common/VoteMgr/VoteMgr.h"



extern cltCommonLogic* pclClient;

CNNPCTradeDlg::CNNPCTradeDlg()
{
	m_bShowFlag = false;

	m_siUserInventorySelectedIndex = -1;
	m_siNPCInventorySelectedIndex = -1;

	m_siSellItemNum = 0;
	m_siBuyItemNum = 0;	
	m_siNPCCharUnique = 0;
	m_siNPCKind = 0;
	m_pNPCTradeBuffer = new CNPCTradeBuffer();
	
	m_pNNPCTradeStatic1   = NULL;
	m_pNNPCTradeStatic2   = NULL;
	m_pNNPCTradeStatic3   = NULL;
	m_pNNPCTradeStatic4   = NULL;
	m_pNNPCTradeStatic5   = NULL;
	m_pNNPCTradeEdit      = NULL;
	m_pNNPCTradeButton1   = NULL;
	m_pNNPCTradeButton2   = NULL;
	m_pNNPCTradeListView1 = NULL;
	m_pNNPCTradeListView2 = NULL; 
	m_pNPCItemInfo =NULL;
	SetTimerOnOff( true );
}



CNNPCTradeDlg::~CNNPCTradeDlg()
{
	SAFE_DELETE(m_pNPCTradeBuffer);
	SAFE_DELETE(m_pNNPCTradeStatic1);
	SAFE_DELETE(m_pNNPCTradeStatic2);
	SAFE_DELETE(m_pNNPCTradeStatic3);
	SAFE_DELETE(m_pNNPCTradeStatic4);
	SAFE_DELETE(m_pNNPCTradeStatic5);
	SAFE_DELETE(m_pNNPCTradeEdit);
	SAFE_DELETE(m_pNNPCTradeButton1);
	SAFE_DELETE(m_pNNPCTradeButton2);
	SAFE_DELETE(m_pNNPCTradeListView1);
	SAFE_DELETE(m_pNNPCTradeListView2);
	SetTimerOnOff( false );
	
}


void CNNPCTradeDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;
	m_pPersonItemInfo = &pclclient->pclCM->CR[1]->pclCI->clCharItem;
	m_pNPCItemInfo = pclclient->pclNPCItemInfo;

	m_bShowFlag = false;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NNPCTrade1/DLG_NNPCTrade1.ddf"));
		file.CreatePopupDialog( this, NNPCTRADE1_DLG, TEXT("dialog_NNPCTrade1"), StaticNNPCTradeDlgProc);		
		
		m_pNNPCTradeStatic1   = new CStatic( this ); 
		m_pNNPCTradeStatic2   = new CStatic( this ); 
		m_pNNPCTradeStatic3   = new CStatic( this );
		m_pNNPCTradeStatic4   = new CStatic( this );
		m_pNNPCTradeStatic5   = new CStatic( this );
		m_pNNPCTradeEdit      = new CEdit( this ); 
		m_pNNPCTradeButton1   = new CButton( this );
		m_pNNPCTradeButton2   = new CButton( this );
		m_pNNPCTradeListView1 = new CListView( this );
		m_pNNPCTradeListView2 = new CListView( this );	 

		file.CreateControl(m_pNNPCTradeStatic1, NNPCTRADE1_STATIC1 , TEXT("statictext_NNPCTrade1"));
		file.CreateControl(m_pNNPCTradeStatic2, NNPCTRADE1_STATIC2 , TEXT("statictext_NNPCTrade2"));
		file.CreateControl(m_pNNPCTradeStatic3, NNPCTRADE1_STATIC3 , TEXT("statictext_NNPCTrade3"));
		file.CreateControl(m_pNNPCTradeStatic4, NNPCTRADE1_STATIC4 , TEXT("statictext_NNPCTrade4"));
		file.CreateControl(m_pNNPCTradeStatic5, NNPCTRADE1_STATIC5 , TEXT("statictext_NNPCTrade5"));
		file.CreateControl(m_pNNPCTradeEdit,	NNPCTRADE1_EDITBOX1 , TEXT("editbox_NNPCTrade"));
		file.CreateControl(m_pNNPCTradeButton1, NNPCTRADE1_BUTTON1 , TEXT("button_NNPCTrade1"));
		file.CreateControl(m_pNNPCTradeButton2, NNPCTRADE1_BUTTON2 , TEXT("button_NNPCTrade2"));
		file.CreateControl(m_pNNPCTradeListView1,NNPCTRADE1_LISTVIEW1, TEXT("listview_NNPCTrade1"));
		m_pNNPCTradeListView1->SetBorder(true);
		file.CreateControl(m_pNNPCTradeListView2,NNPCTRADE1_LISTVIEW2, TEXT("listview_NNPCTrade2"));
		m_pNNPCTradeListView2->SetBorder(true);

		// 숫자만 입력할 수 있게 변경 - By LEEKH
		m_pNNPCTradeEdit->SetNumberMode( true );
		LoadBuyCashItemInfo();
	}
		

	

}

void CALLBACK CNNPCTradeDlg::StaticNNPCTradeDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	
	CNNPCTradeDlg *pThis = (CNNPCTradeDlg*) pControllerMgr;
	pThis->NNPCTradeDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNPCTradeDlg::NNPCTradeDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{	

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON :	
			{
				DeleteDialog();				
			}  
			break;	

		case NNPCTRADE1_EDITBOX1 :
			{
				switch(nEvent)
				{
				case EVENT_EDITBOX_CHANGE:
					{
						TCHAR buffer[256] = TEXT("");
						StringCchCopy(buffer, 256, m_pNNPCTradeEdit->GetText());

						if( _tcslen(buffer) > 0)
						{
							SI32 siInputNum = atoi(buffer);
							if( m_siUserInventorySelectedIndex >= 0 && m_siUserInventorySelectedIndex < MAX_ITEM_PER_PERSON )
							{
								if( siInputNum > m_clUserItem[ m_siUserInventorySelectedIndex ].GetItemNum() )
								{
									m_pNNPCTradeEdit->SetText(SI16ToString(m_clUserItem[ m_siUserInventorySelectedIndex ].GetItemNum()));
								}
							}
						}
					}
					break;
				}
			}
			break;
		case NNPCTRADE1_LISTVIEW1:
			{					
				switch( nEvent )
				{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						
						TCHAR szPrice[512] = TEXT("");
						TCHAR buffer[256]= TEXT("") ;

						SI16 m_siInvSelectedIndex= m_pNNPCTradeListView1->GetSelectedItemIndex();
						m_siUserInventorySelectedIndex=m_siInvSelectedIndex;
						
						

						if ( m_siInvSelectedIndex >= 0 )
						{
							stListViewItemData *pItemData = m_pNNPCTradeListView1->GetListViewItemData(m_siInvSelectedIndex- PERSONITEM_INV0);
							if ( pItemData )
							{	
								StringCchPrintf( buffer, 256, TEXT("%d"),pItemData->ItemNum );
								m_pNNPCTradeEdit->SetText(buffer);
							}
						}

                        
						//m_siUserInventorySelectedIndex = m_siInvSelectedIndex;
						if ( m_siInvSelectedIndex < 0 )
						{
							break;
						}
						//SI32 SellPrice= m_clUserItem[ m_siInvSelectedIndex ].GetItemNum();
						SI32 ItemType = pclClient->pclItemManager->GetItemType(m_clUserItem[ m_siInvSelectedIndex].siUnique);
						//if(pclClient->pclItemManager->IsItemInfoAtb(m_clUserItem[ m_siInvSelectedIndex].siUnique,ITEMINFOATB_ITEMMALL)&& (ItemType== 15 ||ItemType== 9 || ItemType== 33 || ItemType==32))
						if(IsCheckCashItem(m_clUserItem[ m_siInvSelectedIndex].siUnique)==true)
						{
							m_pNNPCTradeButton2->Enable(false);
							TCHAR* pText = GetTxtFromMgr(5588);
							StringCchPrintf(szPrice, 512, pText);
						}
						else
						{
							m_pNNPCTradeButton2->Enable(true);
							if ( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHOCOLATE")) )
							{
								TCHAR szPriceValue[32];
								StringCchPrintf( szPriceValue , 32 , TEXT("%d"), pclClient->pclItemManager->pclItemPrice->GetPriceSellToNPC(pclClient->pclItemManager,  m_clUserItem[ m_siInvSelectedIndex].siUnique ));

								NTCHARString256	pBody(GetTxtFromMgr(15621));	
								pBody.Replace("#money#", szPriceValue);

								StringCchCopy( szPrice, 512, pBody);
							}
							else
							{
								TCHAR* pText = GetTxtFromMgr(1169);
								StringCchPrintf( szPrice, 512, pText, pclClient->pclItemManager->pclItemPrice->GetPriceSellToNPC(pclClient->pclItemManager,  m_clUserItem[ m_siInvSelectedIndex].siUnique ) );
							}
						}						
						m_pNNPCTradeStatic2->SetText(szPrice, DT_WORDBREAK);					

						// 판매 물품 선택시 선택되었던 구매 물품을 무효화함
						m_pNNPCTradeListView2->ClearSelectedItem();
						m_siNPCInventorySelectedIndex = -1;
						m_pNNPCTradeListView2->SetFocus(FALSE);

						
		
					}
					break;
				}
			}  
			break;




		case NNPCTRADE1_LISTVIEW2:
			{					
				switch( nEvent )
				{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{    
						TCHAR buffer[256]= TEXT("") ;	
						StringCchCopy(buffer, 256, TEXT("1"));						
						m_pNNPCTradeEdit->SetText(buffer);
						m_siNPCInventorySelectedIndex = m_pNNPCTradeListView2->GetSelectedItemIndex();
						GMONEY si32Price = m_siNPCItemPrice[ m_siNPCInventorySelectedIndex ];
						TCHAR szPrice[128] = TEXT("");				
						
						TCHAR* pText = GetTxtFromMgr(611);
						g_JYLibFunction.SetNumUnit( si32Price, buffer, 256, pText );						
						pText = GetTxtFromMgr(1170);			
						StringCchPrintf( szPrice, 128, pText, buffer );
						m_pNNPCTradeStatic2->SetText(szPrice, DT_WORDBREAK);	
						
						// 구매 물품 선택시 선택되었던 판매 물품을 무효화함
						m_pNNPCTradeListView1->ClearSelectedItem();
						m_siUserInventorySelectedIndex = -1;
						m_pNNPCTradeListView1->SetFocus(FALSE);

					}
					break;
				}
			}
			break;


		case NNPCTRADE1_BUTTON1:
			{		
				
				switch(nEvent)	
				{				
				case EVENT_BUTTON_CLICKED:
					{

						BOOL bSuccess;							
						SI16 m_siInvSelectedIndex= m_pNNPCTradeListView1->GetSelectedItemIndex();
						
						
						int number;
						TCHAR buffer[256] = TEXT("");
						StringCchCopy(buffer, 256, m_pNNPCTradeEdit->GetText());
						number= _tstoi(m_pNNPCTradeEdit->GetText());
						m_siBuyItemNum=number;
									
						if ( number == FALSE )
						{
							break;
						}
						if ( number < 1 )
						{
							//bSuccess=FALSE;
							break;
						}

						bSuccess=TRUE;
						BuyItemFromNPC();						

					}
					break;
				}
			}break;



		case NNPCTRADE1_BUTTON2:
			{				
				switch(nEvent)	
				{				
				case EVENT_BUTTON_CLICKED:
					{
						int number;
						TCHAR buffer[256] = TEXT("");
                        
						StringCchCopy(buffer, 256, m_pNNPCTradeEdit->GetText());
										
						number= _tstoi(m_pNNPCTradeEdit->GetText());
						m_siSellItemNum=number;
									

						if ( number == FALSE )
						{
							break;
						}
						if ( number < 1 )
						{
							break;
						}
											SellItemToNPC();		


					}
					break;
				}
			}
			break;

	}


	

	return;
}

void CNNPCTradeDlg::Action()
{
	if(IsUserInventoryChanged())	
		LoadUserInventory();		
	if(IsNPCInventoryChanged())
        LoadNPCInventory();


}


bool CNNPCTradeDlg::IsUserInventoryChanged()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clUserItem[ j ];
		BYTE* dst = (BYTE*)&m_pPersonItemInfo->clItem[ j ];

		for( int i = 0; i < size; ++i )
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}





bool CNNPCTradeDlg::IsNPCInventoryChanged()
{
	cltNPCMerchantInfo *pNPCMerchantInfo = NULL;

	for ( SI16 k = 0; k < MAX_NPC_MERCHANT_NUMBER; ++k )
	{
		if ( m_pNPCItemInfo->clNPCMerchantInfo[ k ].siNPCKind == m_siNPCKind )
		{
			pNPCMerchantInfo = &m_pNPCItemInfo->clNPCMerchantInfo[ k ];
			break;
		}
	}

	if ( pNPCMerchantInfo == NULL )
	{
		return false;
	}

	SI32 siSrc = 0;
	SI32 siNPC = 0;

	for ( siSrc = 0; siSrc < MAX_NPC_ITEM_NUMBER;  )
	{
		SI32 size =	 sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clNPCItem[ siNPC ];
		BYTE* dst = (BYTE*)&pNPCMerchantInfo->clItemInfo[ siSrc ].clItem;

		for( int i = 0; i < size; ++i )
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
		
		siSrc++;
		siNPC++;
	}

	return false;
}



void CNNPCTradeDlg::LoadUserInventory()
{
	//m_siUserInventorySelectedIndex = -1;

	//for ( int i = PERSONITEM_INV0; i < PERSONITEM_SUMMON1EQUIP0; ++i )
	//{
	//	if(m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_WAITINGWOMAN")))
	//	{
	//		if(m_pPersonItemInfo->clItem[i].siUnique == ITEMUNIQUE(24105))
	//		{
	//			m_clUserItem[ i ] = m_pPersonItemInfo->clItem[ i ];
	//			m_siUserItemPrice[ i ] = pclClient->pclItemManager->pclItemPrice->GetPriceSellToNPC(pclClient->pclItemManager,  m_clUserItem[ i ].siUnique );
	//		}
	//		else
	//		{
	//			m_clUserItem[ i ].Init();
	//			m_siUserItemPrice[ i ] = 0;				
	//		}
	//	}
	//	m_clUserItem[ i ] = m_pPersonItemInfo->clItem[ i ];
	//	m_siUserItemPrice[ i ] = pclClient->pclItemManager->pclItemPrice->GetPriceSellToNPC(pclClient->pclItemManager,  m_clUserItem[ i ].siUnique );
	//}	
	SettingListView();
	
	return;
}

void CNNPCTradeDlg::LoadNPCInventory()
{
	
	cltClient *pclclient = (cltClient *)pclClient;
	m_pPersonItemInfo =& pclclient->pclCM->CR[1]->pclCI->clCharItem;
	m_pNPCItemInfo = pclclient->pclNPCItemInfo;
	m_siNPCInventorySelectedIndex = -1;

	cltNPCMerchantInfo *pNPCMerchantInfo = NULL;
	
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
	SI32 id = 1;
	m_pNNPCTradeListView2->DeleteAllItems();
	m_pNNPCTradeListView2->SetStartItemPos( StartPos );



	
	//SI32 temp=0;

	for ( SI16 k = 0; k < MAX_NPC_MERCHANT_NUMBER; ++k )
	{
		//temp=m_pNPCItemInfo->GetMerchantInfo(m_siNPCKind)

		if ((m_pNPCItemInfo->clNPCMerchantInfo[k].siNPCKind) == m_siNPCKind )
		{
			pNPCMerchantInfo = &m_pNPCItemInfo->clNPCMerchantInfo[ k ];
			
			break;
		}
	}

	if ( pNPCMerchantInfo == NULL )
	{
		return;
	}
	
	SI32 siIndex = 0;

	for ( SI32 i = 0; i <  MAX_NPC_ITEM_NUMBER; ++i )
	{		
		m_clNPCItem[ siIndex ] = pNPCMerchantInfo->clItemInfo[i].clItem;				
		m_siNPCItemPrice[ siIndex ] = pNPCMerchantInfo->clItemInfo[i].siPrice;

		siIndex++;
	}//end for


	for ( i = 0; i <  MAX_NPC_ITEM_NUMBER; ++i )
	{
		if ( m_clNPCItem[ i ].siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clNPCItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clNPCItem[ i ].siUnique );

				if ( ItemName )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_clNPCItem[ i ], ToolTipMsg );

					if ( i >= StartPos && i < StartPos + MAX_NPC_ITEM_NUMBER  )
					{
						stListViewItemData ListViewItemData;
						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos,m_clNPCItem[ i ].siItemNum, ItemName, ToolTipMsg );														
						m_pNNPCTradeListView2->InsertItem( &ListViewItemData );
						
					}
				}
			}
		}
    }//end for	
	return;
}




// 유저가 NPC 에게 아이템을 판매
void CNNPCTradeDlg::SellItemToNPC()
{
	if ( m_siUserInventorySelectedIndex < 0 )
	{
		return;
	}

	m_clUserItem[ m_siUserInventorySelectedIndex ].SetItemNum(m_siSellItemNum);
	m_pNPCTradeBuffer->Push( NPCTRADE_SELLITEM_TO_NPC, m_siUserInventorySelectedIndex, m_siNPCCharUnique, m_clUserItem[ m_siUserInventorySelectedIndex ] );		//여기서 i는 유저의 아이템 위치임

	return;
}

// NPC 로 부터 유저가 아이템 구입
void CNNPCTradeDlg::BuyItemFromNPC()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( m_siNPCInventorySelectedIndex < 0 )
	{
		return;
	}

	if ( m_clNPCItem[ m_siNPCInventorySelectedIndex ].CanPile( pclClient->pclItemManager ) )
	{
		m_clNPCItem[ m_siNPCInventorySelectedIndex ].SetItemNum(m_siBuyItemNum);					
	}
	else
	{
		m_clNPCItem[ m_siNPCInventorySelectedIndex ].SetItemNum( 1 );
	}

	if ( m_siNPCCharUnique > 0 )
	{
		m_pNPCTradeBuffer->Push( NPCTRADE_BUYITEM_FROM_NPC, m_siNPCInventorySelectedIndex, m_siNPCCharUnique, m_clNPCItem[ m_siNPCInventorySelectedIndex ] );		//여기서 i는 NPC의 아이템 위치임 
	}

	return;
}

void CNNPCTradeDlg::Show(SI32 npckind)
{
		
	if ( ! IsCreate() )
	{

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NNPCTrade1/DLG_NNPCTrade1.ddf"));
		file.CreatePopupDialog( this, NNPCTRADE1_DLG, TEXT("dialog_NNPCTrade1"), StaticNNPCTradeDlgProc);
		
		
		m_pNNPCTradeStatic1   = new CStatic( this ); 
		m_pNNPCTradeStatic2   = new CStatic( this ); 
		m_pNNPCTradeStatic3   = new CStatic( this );
		m_pNNPCTradeStatic4   = new CStatic( this );
		m_pNNPCTradeStatic5   = new CStatic( this );
		m_pNNPCTradeEdit      = new CEdit( this ); 
		m_pNNPCTradeButton1   = new CButton( this );
		m_pNNPCTradeButton2   = new CButton( this );
		m_pNNPCTradeListView1 = new CListView( this );
		m_pNNPCTradeListView2 = new CListView( this );

	 

		file.CreateControl(m_pNNPCTradeStatic1, NNPCTRADE1_STATIC1 , TEXT("statictext_NNPCTrade1"));
		file.CreateControl(m_pNNPCTradeStatic2, NNPCTRADE1_STATIC2 , TEXT("statictext_NNPCTrade2"));
		file.CreateControl(m_pNNPCTradeStatic3, NNPCTRADE1_STATIC3 , TEXT("statictext_NNPCTrade3"));
		file.CreateControl(m_pNNPCTradeStatic4, NNPCTRADE1_STATIC4 , TEXT("statictext_NNPCTrade4"));
		file.CreateControl(m_pNNPCTradeStatic5, NNPCTRADE1_STATIC5 , TEXT("statictext_NNPCTrade5"));
		file.CreateControl(m_pNNPCTradeEdit, NNPCTRADE1_EDITBOX1 , TEXT("editbox_NNPCTrade"));
		file.CreateControl(m_pNNPCTradeButton1, NNPCTRADE1_BUTTON1 , TEXT("button_NNPCTrade1"));
		file.CreateControl(m_pNNPCTradeButton2, NNPCTRADE1_BUTTON2 , TEXT("button_NNPCTrade2"));
		file.CreateControl(m_pNNPCTradeListView1,NNPCTRADE1_LISTVIEW1, TEXT("listview_NNPCTrade1"));
		file.CreateControl(m_pNNPCTradeListView2,NNPCTRADE1_LISTVIEW2, TEXT("listview_NNPCTrade2"));
		
		
	}

	m_siNPCKind = npckind;
	SI32 CharID = pclClient->pclCM->GetIDFromCharKind(m_siNPCKind);
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( CharID );
	if( NULL == pclChar )	
	{
		DeleteDialog();				
		return;
	}
	m_siNPCCharUnique = pclClient->pclCM->CR[ CharID ]->GetCharUnique();

	SettingListView();	// 유저 인벤토리의 아이템을 모두 표시 	
			
	if(IsUserInventoryChanged())	
		LoadUserInventory();
	if(IsNPCInventoryChanged())
		LoadNPCInventory();


	if ( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_WAITINGWOMAN")) )// 무수리
	{
		SetDialogInfo( 1171, 1172 );
	}
	else if ( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")) )
	{
		SetDialogInfo( 1173, 1174 );
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MAGICALRACCOON_NPC")) )
	{
		SetDialogInfo( 7045, 7044 );
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ANNIVERSARY")) )
	{
		SetDialogInfo( 8701, 8755 );
		// 판매버튼 안보여짐
		m_pNNPCTradeButton2->Show(false);
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR1")) )
	{
		SetDialogInfo( 20085, 1172 );
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR2")) )
	{
		SetDialogInfo( 20086, 1172 );
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR3")) )
	{
		SetDialogInfo( 20087, 1172 );
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR4")) )
	{
		SetDialogInfo( 20088, 1172 );
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHOCOLATE")) )
	{
		SetDialogInfo( 0, 15621 );
	}

	//else if ( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_RUSSIATRADER")) )
	//{
	//	m_pNNPCTradeStatic1->SetFileName(TEXT("Interface/Portrait/portraiticon.spr"));
	//	m_pNNPCTradeStatic1->SetFontIndex(pclClient->GetUniqueFromHash(TEXT("KIND_RUSSIATRADER"))-1);
	//	TCHAR* pText = GetTxtFromMgr(1175);			
	//	SetTitle(pText);
	//	pText = GetTxtFromMgr(1176);
	//	m_pNNPCTradeStatic2->SetBKColorUse(true);
	//	m_pNNPCTradeStatic2->SetBorder(true);
	//	m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);	

	//}
	else{}
	

	
	cltGameMsgRequest_NPCSellItemInfo clGameMsgRequest_NPCSellItemInfo( true, m_siNPCKind );
	cltMsg clMsg( GAMEMSG_REQUEST_NPCSELLITEMINFO, sizeof(clGameMsgRequest_NPCSellItemInfo), (BYTE*)&clGameMsgRequest_NPCSellItemInfo );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	m_bShowFlag = true;


}


/*
void CNNPCTradeDlg::Hide()
{
	m_siNPCCharUnique = 0;
	m_siNPCKind = 0;
	m_bShowFlag = false;
}

bool CNNPCTradeDlg::IsShow()
{
	return m_bShowFlag;
}
*/

bool CNNPCTradeDlg::Pop( stNPCTradeData * pNPCTradeData )
{
	return m_pNPCTradeBuffer->Pop( pNPCTradeData );
}


void CNNPCTradeDlg::SetText( TCHAR *strText )
{
	m_pNNPCTradeStatic2->SetText(strText,DT_WORDBREAK);
}

void CNNPCTradeDlg::SetText( SI32 siTextIndex )
{
		
	if(IsUserInventoryChanged())	
		LoadUserInventory();		
	if(IsNPCInventoryChanged())
        LoadNPCInventory();
	
	if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_WAITINGWOMAN"))
		|| m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHOCOLATE")) )
	{
		switch( siTextIndex )
		{
		case SYSTEMNPC_SUCCESS_BUY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_BULLET:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_BULLET")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_ARROW:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_ARROW")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_FOOD:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_FOOD")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_HEAL:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_HEAL")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
	
		case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1178);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);


				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_SUCCESS_SELL:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELL_ITEM")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1179);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_BULLETITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(5837);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_ITEMMALLITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(5588);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_QUESTITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(6142);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
			}
			break;
		case SYSTEMNPC_FAIL_BUY_SOLDOUT:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(15624);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

			}
			break;
		}

	}
	// 축제 음식
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR1")) ||
		m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR2")) ||
		m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR3")) ||
		m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SNACKBAR4")) )
	{
		switch( siTextIndex )
		{
		case SYSTEMNPC_SUCCESS_BUY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_BULLET:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_BULLET")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_ARROW:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_ARROW")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_FOOD:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_FOOD")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_SUCCESS_BUY_HEAL:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				(pclclient->pclMusic)->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_BUY_HEAL")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1177);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);			
			}
			break;
		case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1178);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);


				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_SUCCESS_SELL:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SELL_ITEM")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1179);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_BULLETITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(5837);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_ITEMMALLITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(5588);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_QUESTITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(6142);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
			}
			break;
		}

	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")) )
	{
		switch( siTextIndex )
		{
		case SYSTEMNPC_SUCCESS_BUY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1180);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);				

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(1181);

				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_SUCCESS_SELL:
			{
				cltClient *pclclient = (cltClient*)pclClient;

				TCHAR* pText = GetTxtFromMgr(1182);


				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_BULLETITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(5837);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_ITEMMALLITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(5588);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_QUESTITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(6142);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
			}
			break;			
		}
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MAGICALRACCOON_NPC")) )
	{
		switch( siTextIndex )
		{
		case SYSTEMNPC_SUCCESS_BUY:
		case SYSTEMNPC_SUCCESS_BUY_HEAL:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(7046);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);				

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(7047);

				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_SUCCESS_SELL:
			{
				cltClient *pclclient = (cltClient*)pclClient;

				TCHAR* pText = GetTxtFromMgr(7048);


				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_BULLETITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(7049);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_ITEMMALLITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(7050);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

				//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
			}
			break;
		case SYSTEMNPC_FAIL_SELL_QUESTITEM:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(7051);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
			}
			break;			
		}
	}
	else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ANNIVERSARY")) )
	{
		switch( siTextIndex )
		{
		case SYSTEMNPC_SUCCESS_BUY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(8702);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);				
			}
			break;
		case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
			{
				cltClient *pclclient = (cltClient*)pclClient;
				pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

				TCHAR* pText = GetTxtFromMgr(8703);
				m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
			}
			break;
		}
	}

	//else if( m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_RUSSIATRADER")) )
	//{
	//	switch( siTextIndex )
	//	{
	//	case SYSTEMNPC_SUCCESS_BUY:
	//		{
	//			cltClient *pclclient = (cltClient*)pclClient;
	//			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );

	//			TCHAR* pText = GetTxtFromMgr(1183);


	//			//	SetWindowText( m_hNPCTradeInfoViewEdit, pText );
	//			m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
	//		}
	//		break;
	//	case SYSTEMNPC_FAIL_BUY_REASPON_MONEY_INSUFFICIENTY:
	//		{
	//			cltClient *pclclient = (cltClient*)pclClient;
	//			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

	//			TCHAR* pText = GetTxtFromMgr(1184);


	//			//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
	//			m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
	//		}
	//		break;
	//	case SYSTEMNPC_SUCCESS_SELL:
	//		{
	//			cltClient *pclclient = (cltClient*)pclClient;
	//			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_SUCCESS")), 0, 0 );

	//			TCHAR* pText = GetTxtFromMgr(1185);


	//			//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
	//			m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
	//		}
	//		break;
	//	case SYSTEMNPC_FAIL_SELL_BULLETITEM:
	//		{
	//			cltClient *pclclient = (cltClient*)pclClient;
	//			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

	//			TCHAR* pText = GetTxtFromMgr(5837);
	//			m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

	//			//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
	//		}
	//		break;
	//	case SYSTEMNPC_FAIL_SELL_ITEMMALLITEM:
	//		{
	//			cltClient *pclclient = (cltClient*)pclClient;
	//			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

	//			TCHAR* pText = GetTxtFromMgr(5588);
	//			m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);

	//			//SetWindowText( m_hNPCTradeInfoViewEdit, pText );
	//		}
	//		break;
	//	case SYSTEMNPC_FAIL_SELL_QUESTITEM:
	//		{
	//			cltClient *pclclient = (cltClient*)pclClient;
	//			pclclient->pclMusic->PushEffect( pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0 );

	//			TCHAR* pText = GetTxtFromMgr(6142);
	//			m_pNNPCTradeStatic2->SetText(pText,DT_WORDBREAK);
	//		}
	//		break;
	//	}			

	//}
	else
	{}

	return;
	
}


void CNNPCTradeDlg::SettingListView()
{
/*--------------------------------------------------------------------------
              listview 추가하는 부분 시작 by tae yang  (다시 로드 하기 위해 )
---------------------------------------------------------------------------*/
	
	
		SI32 GlobalImageIndex = -1;
		SI32 GlobalImageFont = -1;
		SI32 StartPos=15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
		SI32 id = 1;
		m_pNNPCTradeListView1->DeleteAllItems();
		m_pNNPCTradeListView1->SetStartItemPos( StartPos );

		for ( SI32 i = 0; i < MAX_INVITEM_PER_PERSON+15; ++i )
		{
			m_clUserItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );			
			if ( m_clUserItem[ i ].siUnique > 0 )
			{
				stListViewItemData  ListViewItemData;

				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clUserItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clUserItem[ i ].siUnique );

					if ( ItemName )
					{
						TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
						pclClient->pclItemManager->GetExplain( &m_clUserItem[ i ], ToolTipMsg );

						if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON )
						{
							ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_clUserItem[ i ].siItemNum, ItemName, ToolTipMsg );
						}
					}
				}

				// 엔젤릿의 과거사진 아이템은 엔젤릿 NPC한테만 판매가능하다.
				if( m_clUserItem[ i ].siUnique == ITEMUNIQUE(24105) )
				{
					if (m_siNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_WAITINGWOMAN")))
					{
						if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON )
							m_pNNPCTradeListView1->InsertItem( &ListViewItemData );
					}
				}
				else
				{
					if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON )
						m_pNNPCTradeListView1->InsertItem( &ListViewItemData );
				}
			}
		}//end for	
/*----------------------------------------------------------------------------
              listview 추가하는 부분 끝 ~ ^^* by tae yang
-----------------------------------------------------------------------------*/
}




bool CNNPCTradeDlg::LoadBuyCashItemInfo()
{
	memset( m_siItemUniqueList, 0, sizeof( SI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( m_uiItemPriceList, 0, sizeof( UI16 ) * MAX_AVAILABLE_CASHITEMNUM );
	memset( m_strItemTypeList, 0, MAX_ITEMTYPE_NUMBER * 64 );

	FILE *fp = NULL;

	if( pclClient->siServiceArea == ConstServiceArea_Korea )
		fp = _tfopen( TEXT("Item/Data/CashItemBuyInfoK.txt"), TEXT("rt") );
	else if( pclClient->siServiceArea == ConstServiceArea_English )
		fp = _tfopen( TEXT("Item/Data/CashItemBuyInfoE.txt"), TEXT("rt") );
	else if( pclClient->siServiceArea == ConstServiceArea_China )
		fp = _tfopen( TEXT("Item/Data/CashItemBuyInfoC.txt"), TEXT("rt") );
	else if( pclClient->siServiceArea == ConstServiceArea_Taiwan )
		fp = _tfopen( TEXT("Item/Data/CashItemBuyInfoT.txt"), TEXT("rt") );
	else if( pclClient->siServiceArea == ConstServiceArea_USA )
		fp = _tfopen( TEXT("Item/Data/CashItemBuyInfoU.txt"), TEXT("rt") );
	else if( pclClient->siServiceArea == ConstServiceArea_NHNChina )
		fp = _tfopen( TEXT("Item/Data/CashItemBuyInfoC.txt"), TEXT("rt") );
	else if( pclClient->siServiceArea == ConstServiceArea_EUROPE )
		fp = _tfopen( TEXT("Item/Data/CashItemBuyInfoG.txt"), TEXT("rt") );


	if ( fp == NULL )
	{
		return false;
	}

	TCHAR buffer[1024];

	for( SI16 i = 0;i < 6; ++i )
		_fgetts( buffer, 1024, fp );


	SI16 Counter = 0;
	SI16 ItemTypeCounter = 0;

	SI32 ItemUnique = 0;
	TCHAR ItemName[ 64 ] = TEXT("");
	TCHAR ItemType[ 64 ] = TEXT("");
	SI32 ItemPrice = 0;


	while( _fgetts(buffer, 1024, fp) != NULL)
	{
		//if( pclClient->siServiceArea != ConstServiceArea_Korea )
		//{
			ParseText( buffer );

			ItemUnique = _tstoi( tokens[0] );
			MStrCpy( ItemName, tokens[1], 64 );
			MStrCpy( ItemType, tokens[2], 64 );
			ItemPrice = _tstoi( tokens[3] );
		//}
		//else
		//{
		//	_stscanf( buffer, TEXT("%d %s %s %d"), &ItemUnique, ItemName, ItemType, &ItemPrice );
		//}

		//	_stscanf( buffer, TEXT("%d %s %s %d"), &ItemUnique, ItemName, ItemType, &ItemPrice );

		m_siItemUniqueList[ Counter ] = ItemUnique;
		m_uiItemPriceList[ Counter ] = ItemPrice;

		if ( ! IsSameItemTypeExist( ItemType ) )
		{
			StringCchCopy ( m_strItemTypeList[ ItemTypeCounter++ ], 64, ItemType );
		}

		if ( ++Counter >= MAX_AVAILABLE_CASHITEMNUM )
		{
			break;
		}
	}

	fclose( fp );

	return true;
}


bool CNNPCTradeDlg::IsSameItemTypeExist( TCHAR *ItemType )
{
	for ( SI16 i = 0; i < MAX_ITEMTYPE_NUMBER; ++i )
	{
		if ( _tcslen( m_strItemTypeList[ i ] ) > 0 )
		{
			if ( _tcscmp( m_strItemTypeList[ i ], ItemType ) == 0 )
			{
				return true;
			}
		}
		else
		{
			break;
		}
	}

	return false;
}

bool CNNPCTradeDlg::IsCheckCashItem(SI32 Uinque)
{
	for (SI32 i = 0; i <= MAX_AVAILABLE_CASHITEMNUM; i++)
	{
		if( m_siItemUniqueList[i]  ==Uinque)
			return true;
	}
	return false;
}

void CNNPCTradeDlg::SetDialogInfo( SI32 siTitleTextNum, SI32 siExplainTextNum )
{
	if ( (0 > m_siNPCKind) || (0 > siTitleTextNum) || (0 > siExplainTextNum) )
	{
		return;
	}

	// NPC 초상 설정
	TCHAR	szPortrait[256] = { '\0', };
	SI32	siPortraitFont	= 0;
	if ( false == pclClient->pclKindInfoSet->GetNPCImageFileAndFont(m_siNPCKind, szPortrait, sizeof(szPortrait), &siPortraitFont) )
	{
		return;
	}
	m_pNNPCTradeStatic1->SetFileName(szPortrait);
	m_pNNPCTradeStatic1->SetFontIndex(siPortraitFont);

	// 다이얼로그 Text 설정
	if( siTitleTextNum != 0 )
	{
		TCHAR* pszTitle = GetTxtFromMgr( siTitleTextNum );
		if ( pszTitle )
		{
			SetTitle( pszTitle );
		}
	}
	// 설명창 Text설정
	if( siExplainTextNum != 0 )
	{
		TCHAR* pszExplain = GetTxtFromMgr( siExplainTextNum );
		if ( pszExplain )
		{
			m_pNNPCTradeStatic2->SetBKColorUse( true );
			m_pNNPCTradeStatic2->SetBorder( true );
			m_pNNPCTradeStatic2->SetText( pszExplain, DT_WORDBREAK );
		}
	}
}

