

//---------------------------------
// 2003/3/11 김태곤
//---------------------------------
#include "Client.h"

#include "NNPCRepair.h"
#include "Resource.h"
#include "../NPrivateRepair/NPrivateRepairDlg.h"

#include "../../../CommonLogic\MsgType-Item.h"
#include "../../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;

NcltNPCRepairManager::NcltNPCRepairManager()
{
	//m_hDlg				= NULL;
	//m_pItemListView		= NULL;
	bDlgRectInitSwitch	= false;
	//m_hPortraitBitmap	= NULL;
	m_siInvSelectedIndex = -1;		
	//m_siInvSelectedIndex = -1;
	m_siPreInvSelectedIndex = -1;
	m_siCharUnique = 0;
	m_pclUserInvItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	m_siItemRepairPrice = 0;

	m_pNNPCRepairStatic1 = NULL;
	m_pNNPCRepairStatic2 = NULL;
	m_pNNPCRepairStatic3 = NULL;
	m_pNNPCRepairListView = NULL;	
	m_pNNPCRepairButton1 = NULL;
	m_pNNPCRepairButton2 = NULL;
	
}

NcltNPCRepairManager::~NcltNPCRepairManager()
{
	SAFE_DELETE(m_pNNPCRepairStatic1);
	SAFE_DELETE(m_pNNPCRepairStatic2);
	SAFE_DELETE(m_pNNPCRepairStatic3);
	SAFE_DELETE(m_pNNPCRepairListView);
	SAFE_DELETE(m_pNNPCRepairButton1);
	SAFE_DELETE(m_pNNPCRepairButton2);	


	if( m_pclUserInvItem )
	{
		delete [] m_pclUserInvItem;
		m_pclUserInvItem = NULL;
	}
}




void NcltNPCRepairManager::Create(SI32 CharUnique)
{

	if ( ! IsCreate() )
	{

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NPCRepair/DLG_NPCRepair.ddf"));
		file.CreatePopupDialog( this, NNPCREPAIR_DLG, TEXT("dialog_NPCRepair"), StaticNDlgProc);
			
		m_pNNPCRepairStatic1   = new CStatic( this ); 
		m_pNNPCRepairStatic2   = new CStatic( this ); 
		m_pNNPCRepairStatic3   = new CStatic( this );
		m_pNNPCRepairListView  = new CListView( this ); 
		m_pNNPCRepairButton1   = new CButton( this );
		m_pNNPCRepairButton2   = new CButton( this );	
	 

		file.CreateControl(m_pNNPCRepairStatic1, NNPCREPAIR_STATIC1 , TEXT("statictext_NPCRepair1"));
		file.CreateControl(m_pNNPCRepairStatic2, NNPCREPAIR_STATIC2 , TEXT("statictext_NPCRepair2"));
		file.CreateControl(m_pNNPCRepairStatic3, NNPCREPAIR_STATIC3 , TEXT("statictext_NPCRepair3"));
		file.CreateControl(m_pNNPCRepairListView,NNPCREPAIR_LISTVIEW, TEXT("listview_NPCRepair"));
		m_pNNPCRepairListView->SetBorder(true);
		file.CreateControl(m_pNNPCRepairButton1, NNPCREPAIR_BUTTON1 , TEXT("button_NPCRepair1"));	
		file.CreateControl(m_pNNPCRepairButton2, NNPCREPAIR_BUTTON2 , TEXT("button_NPCRepair2"));
	}
	    
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_JANGYOUNGSIL")), szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pNNPCRepairStatic1->SetFileName(szPortrait);
	m_pNNPCRepairStatic1->SetFontIndex(siPortraitFont);
	TCHAR buffer[256]= TEXT("");
	TCHAR* pText = GetTxtFromMgr(1425);
	StringCchCopy(buffer, 256, pText);
	m_pNNPCRepairStatic2->SetBorder(true);
	m_pNNPCRepairStatic2->SetBKColorUse(true);
	m_pNNPCRepairStatic2->SetText(buffer,DT_WORDBREAK);
	m_siCharUnique = CharUnique;
	

	SettingListView();



	return ;
}




void CALLBACK NcltNPCRepairManager::StaticNDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{

	NcltNPCRepairManager *pThis = (NcltNPCRepairManager*) pControllerMgr;
	pThis->NDlgProc( nEvent, nControlID, pControl );
}

SI32 flagindex;
void CALLBACK NcltNPCRepairManager::NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient* pclclient = (cltClient*)pclClient;	
    SetActionTimer(200);	
	SettingListView();
	
	
	TCHAR szSelText[128] = TEXT("");
	
	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON :	
			{
				DeleteDialog();				
			}
			break;
		case NNPCREPAIR_BUTTON2:
			{
				DeleteDialog();
			}
			break;

		case NNPCREPAIR_LISTVIEW:
			{
				switch( nEvent )
				{
				case EVENT_LISTVIEW_LBUTTONUP:
					{
						TCHAR* pText;	

						SI16 m_siInvSelectedIndex= m_pNNPCRepairListView->GetSelectedItemIndex();
						flagindex=m_siInvSelectedIndex;

						if ( m_siInvSelectedIndex < 0 )
						{
							break;
						}
												
						if ( m_pclUserInvItem[ m_siInvSelectedIndex ].CanRepair( pclClient->pclItemManager ) == false )
						{
							pText = GetTxtFromMgr(1426);
							m_pNNPCRepairStatic2->SetText(pText,DT_WORDBREAK);
							
						}

							
						if ( m_siInvSelectedIndex >= 0 && m_siPreInvSelectedIndex != m_siInvSelectedIndex )
						{
							m_siPreInvSelectedIndex = m_siInvSelectedIndex;
							
							if ( m_pclUserInvItem[ m_siInvSelectedIndex ].CanRepair( pclClient->pclItemManager ) == true )
							{
								if ( m_pclUserInvItem[ m_siInvSelectedIndex ].GetDurability(  ) == m_pclUserInvItem[ m_siInvSelectedIndex ].GetMaxDurability( pclClient->pclItemManager ) )
								{
									TCHAR* pText = GetTxtFromMgr(1427);
									m_pNNPCRepairStatic2->SetText(pText,DT_WORDBREAK);
									
								}
								else
								{
									cltGameMsgRequest_NPCRepairPrice clNPCRepairPrice( &m_pclUserInvItem[ m_siInvSelectedIndex ]);
									cltMsg clMsg( GAMEMSG_REQUEST_NPCREPAIRPRICE, sizeof(clNPCRepairPrice), (BYTE*)&clNPCRepairPrice );
									pclClient->SendMsgToServer((sPacketHeader*)&clMsg);						
								}
							}
							
						}		 
						

					}
					break;
				}

			}
			break;
		



		case NNPCREPAIR_BUTTON1:
			{
					
				switch( nEvent )
				{
				case EVENT_BUTTON_CLICKED:	
					{
						m_siInvSelectedIndex=flagindex;
							
						if ( m_siInvSelectedIndex < 0 )
						{
							break;
							
						}

						if ( m_pclUserInvItem[ m_siInvSelectedIndex ].CanRepair( pclClient->pclItemManager ) == true )
						{
							if ( m_pclUserInvItem[ m_siInvSelectedIndex ].GetDurability( ) == m_pclUserInvItem[ m_siInvSelectedIndex ].GetMaxDurability( pclClient->pclItemManager ) )
							{
								TCHAR* pText = GetTxtFromMgr(1428);
								m_pNNPCRepairStatic2->SetText(pText,DT_WORDBREAK);
								break;
							
							}

							if( m_siCharUnique > 0 )
							{
								if( m_siItemRepairPrice > 0 && pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < m_siItemRepairPrice )
								{
									TCHAR* pTitle = GetTxtFromMgr(1429);
									TCHAR* pText = GetTxtFromMgr(1430);

									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
								}

								cltGameMsgRequest_NPCRepair clNPCRepair( m_siCharUnique, m_siInvSelectedIndex, &m_pclUserInvItem[ m_siInvSelectedIndex ] );
								cltMsg clMsg( GAMEMSG_REQUSET_NPCREPAIR, sizeof(clNPCRepair), (BYTE*)&clNPCRepair );
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

							}				
						} 
					}
					break;
				}

			}
			break;
	}

	
	
	return ;
}

// 인벤토리 아이템이 바뀌었는지 체크
bool NcltNPCRepairManager::IsChangedInvItem()
{
	for ( int j = 0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, PERSONITEM_INV0 부터 함.
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_pclUserInvItem[ j ];
		BYTE* dst = (BYTE*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];

		for(int i = 0;i < size; i++)
		{
			if( src[i] != dst[i] )
			{
				return true;
			}
		}
	}

	return false;
}

// 지속적으로 update 시키지 않기 위해서 처리한것.
void NcltNPCRepairManager::LoadItemData()
{
	for ( int i = 0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_pclUserInvItem[i] = pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ i ];
	}
	
	SettingListView();



	return;
}

void NcltNPCRepairManager::SettingListView()
{
/*--------------------------------------------------------------------------
              listview 추가하는 부분 시작 by tae yang  (다시 로드 하기 위해 )
---------------------------------------------------------------------------*/
	if ( IsChangedInvItem() )
	{
		LoadItemData();	
	
		SI32 GlobalImageIndex = -1;
		SI32 GlobalImageFont = -1;
		SI32 StartPos=0;
		SI32 id = 1;
		m_pNNPCRepairListView->DeleteAllItems();
		m_pNNPCRepairListView->SetStartItemPos( StartPos );
		
      


		for ( SI32 i = 0; i < MAX_ITEM_PER_PERSON; ++i )
		{
			m_pclUserInvItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );			
			if ( m_pclUserInvItem[ i ].siUnique > 0 )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_pclUserInvItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_pclUserInvItem[ i ].siUnique );

					if ( ItemName )
					{
						TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
						pclClient->pclItemManager->GetExplain( &m_pclUserInvItem[ i ], ToolTipMsg );

						if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON  &&  m_pclUserInvItem[i].CanRepair( pclClient->pclItemManager)/*AX_ITEM_PER_INVENTORY*/ )
						{
							stListViewItemData ListViewItemData;
							ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_pclUserInvItem[ i ].siItemNum, ItemName, ToolTipMsg );														
							m_pNNPCRepairListView->InsertItem( &ListViewItemData );
							
						}
					}
				}
			}
		}
	}

/*----------------------------------------------------------------------------
              listview 추가하는 부분 끝 ~ ^^* by tae yang
-----------------------------------------------------------------------------*/

}



void NcltNPCRepairManager::SetItemRepairPriceExplain( SI32 ItemUnique, GMONEY ItemPrice )
{
	TCHAR Buffer[ 512 ] = TEXT("");
	TCHAR *ItemName = (TCHAR *)pclClient->pclItemManager->GetName( ItemUnique );

	if ( ItemName == NULL )
	{
		return;
	}
	
	TCHAR* pText = GetTxtFromMgr(1431);

	TCHAR money[128]= TEXT(" ") ;
	StringCchPrintf(money, 128, TEXT("%I64d"),ItemPrice);
	Nsprintf(Buffer,pText,TEXT("item"),  ItemName  ,TEXT("money"), money   ,NULL);
	//sprintf( Buffer, pText, ItemName, ItemPrice );
	
	m_siItemRepairPrice = ItemPrice;
	if(IsCreate())
	{
		//m_pNNPCRepairStatic2->SetText(TEXT("수리가 정말 잘되었군"),DT_WORDBREAK);
		//m_pNNPCRepairStatic2->SetText(Buffer,DT_WORDBREAK);

	}	
	m_pNNPCRepairStatic2->SetText(Buffer,DT_WORDBREAK);

	return;
}



void NcltNPCRepairManager::Action()
{

		

}



void NcltNPCRepairManager::SetReairSuccessMent( TCHAR *Text )
{
	cltClient* pclclient = (cltClient*)pclClient;
	
	if ( Text != NULL && _tcslen ( Text ) > 0 )
	{
		if(m_pNNPCRepairStatic2)
			
			m_pNNPCRepairStatic2->SetText(Text,DT_WORDBREAK);
		

	}
	
	return;
}

