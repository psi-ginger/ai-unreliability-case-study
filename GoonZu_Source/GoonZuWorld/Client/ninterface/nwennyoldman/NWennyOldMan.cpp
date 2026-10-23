

// NWennyOldMan.cpp: implementation of the CNWennyOldMan class.
//
//////////////////////////////
////////////////////////////////////////
#include "NWennyOldMan.h"
#include "../../Client/client.h"


#include "../../interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"

#include "NInterface/NHintDlg/NHintDlg.h"
#include "NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"
#include "Interface/SoundOnOffDlg/SoundOnOffDlg.h"


//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "../../../Common/HintDlg/HintDlg.h"
#include "Char\CharManager\CharManager.h"
#include "Item\ItemCommon\Itemunit.h"
#include "../../../Server/GachaManager/GachaManager.h"
#include "../../../Server/Gacha2Manager/Gacha2Manager.h"

#include "../../Music/music.h"



extern cltCommonLogic* pclClient;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNWennyOldMan::CNWennyOldMan()
{
	
	m_siSelectIndexMyInven = -1;
	m_siSelectIndexStorage = -1;

	m_siStorageCount = 0;

	m_pNWennyStatic = NULL;
	m_pNWennyStaticGreeting = NULL;
	m_pNWennyButtonDisAssemble = NULL;
	m_pNWennyButtonRecieve = NULL;

	m_pNWennyStatic1 = NULL;		
	m_pNWennyStatic2 = NULL;		
	m_pMyInventory = NULL;
	m_pWennyStorage = NULL;

	m_sendFrame = 0;

	m_SendDisassembleClock = 0;
	
	SetTimerOnOff( true );
	SetActionTimer( 200);

	m_siAutoState = WENNYOLDMAN_AUTO_STATE_NONE;
}



CNWennyOldMan::~CNWennyOldMan()
{
	SAFE_DELETE(m_pNWennyStatic);
	SAFE_DELETE(m_pNWennyStaticGreeting);
	SAFE_DELETE(m_pNWennyButtonDisAssemble);
	SAFE_DELETE(m_pNWennyButtonRecieve);

	SAFE_DELETE(m_pNWennyStatic1);
	SAFE_DELETE(m_pNWennyStatic2);
	SAFE_DELETE(m_pMyInventory);
	SAFE_DELETE(m_pWennyStorage);

	m_bShow = FALSE ;
	SetTimerOnOff( false );
	
}



void CNWennyOldMan::Create()
{
	
	if ( ! IsCreate() )
	{
		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NWennyMan/DLG_NWennyMan.ddf"));

		file.CreatePopupDialog( this, NWENNYMAN_DLG, TEXT("dialog_NWennyMan"), StaticNWennyOldManProc );		
		m_pNWennyStatic = new CStatic(this);
		m_pNWennyStaticGreeting = new CStatic(this);
		m_pNWennyButtonDisAssemble = new CButton(this);
		m_pNWennyButtonRecieve = new CButton(this);

		m_pNWennyStatic1 = new CStatic(this);
		m_pNWennyStatic2 = new CStatic(this);
		m_pMyInventory = new CListView(this);
		m_pWennyStorage = new CListView(this);


		file.CreateControl( m_pNWennyStatic, NWENNYMAN_STATIC, TEXT("statictext_NWennyMan") );
		file.CreateControl( m_pNWennyStaticGreeting, NWENNYMAN_STATIC_GREETING, TEXT("static_NWennyMan_Greeting") );
		file.CreateControl( m_pNWennyButtonDisAssemble, NWENNYMAN_BUTTON_DISASSEMBLE, TEXT("button_disassemble") );
		file.CreateControl( m_pNWennyButtonRecieve, NWENNYMAN_BUTTON_RECIEVEITEM, TEXT("button_recieveitem") );
		file.CreateControl( m_pNWennyStatic1, NWENNYMAN_STATIC1, TEXT("statictext_NONAME1") );
		file.CreateControl( m_pNWennyStatic2, NWENNYMAN_STATIC2, TEXT("statictext_NONAME2") );
		file.CreateControl( m_pMyInventory, NWENNYMAN_LISTVIEW_MYINVENTORY, TEXT("listview_myinventory") );
		file.CreateControl( m_pWennyStorage, NWENNYMAN_LISTVIEW_WENNYSTORAGE, TEXT("listview_wennystorage") );

		m_pNWennyStatic->SetBorder(true);
		m_pNWennyStatic->SetBKColorUse(true);
		m_pNWennyStaticGreeting->SetBorder(true);
		m_pNWennyStaticGreeting->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr(5938);
		m_pNWennyStaticGreeting->SetText(pText);

		m_pMyInventory->SetBorder(true);
		m_pWennyStorage->SetBorder(true);

		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_WENNYOLDMAN")), szPortrait, sizeof(szPortrait), &siPortraitFont);
 		m_pNWennyStatic->SetFileName(szPortrait);
		m_pNWennyStatic->SetFontIndex(siPortraitFont);
		
		//if(pclClient->pclCM->CR[1]->GetGameMasterLevel() <= 100)
		//m_pNWennyButtonDisAssemble->Enable(false);

		LoadDisAssembleItemData();
		LoadWeenyOldManInven();

		m_siAutoState = WENNYOLDMAN_AUTO_STATE_NONE;

		
	}
	else
	{
		DeleteDialog();
	}

	
    

}
void CNWennyOldMan::Action()
{
	
	if(IsChangedInvItem())
	{
		LoadDisAssembleItemData();
	}

	if (TABS(pclClient->CurrentClock - m_SendDisassembleClock) > 3000 )
	{
		m_pNWennyButtonDisAssemble->Enable(true);
	}

	if ( IsShow() )
	{
		switch(m_siAutoState)
		{
		case WENNYOLDMAN_AUTO_STATE_NONE:
			{
			}
			break;
		case WENNYOLDMAN_AUTO_STATE_BUTTON_CLICK:
			{
				m_siAutoState = WENNYOLDMAN_AUTO_STATE_SEND;
			}
			break;
		case WENNYOLDMAN_AUTO_STATE_SEND:
			{
				m_siAutoState = WENNYOLDMAN_AUTO_STATE_NONE;

				BOOL	bSended = FALSE;
				SI32	siMaxList = MAX_ITEM_PER_WENNYOLD;
				if(siMaxList > MAX_LISTVIEW_ITEM_NUM)		siMaxList = MAX_LISTVIEW_ITEM_NUM;

				for(SI32 i=0; i<siMaxList; i++)
				{
					stListViewItemData* pkListViewData = m_pWennyStorage->GetListViewItemData( i );
					if(pkListViewData != NULL)
					{
						if( _tcslen(pkListViewData->ItemName) > 0 && pkListViewData->ItemNum > 0)
						{
							cltGameMsgRequest_ItemDisAssemble_Recieve clItem(i, &m_clWennyItem[i]);
							cltMsg clMsg(GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE, sizeof(clItem), (BYTE*)&clItem);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

							m_siAutoState = WENNYOLDMAN_AUTO_STATE_WAITRECV;
							bSended = TRUE;

							break;
						}
					}
				}

				if(bSended == FALSE)
				{
					m_siAutoState = WENNYOLDMAN_AUTO_STATE_END;
				}
			}
			break;
		case WENNYOLDMAN_AUTO_STATE_WAITRECV:
			{
			}
			break;
		case WENNYOLDMAN_AUTO_STATE_RECV:
			{
				m_siAutoState = WENNYOLDMAN_AUTO_STATE_WAIT;
			}
			break;
		case WENNYOLDMAN_AUTO_STATE_WAIT:
			{
				m_siAutoState = WENNYOLDMAN_AUTO_STATE_SEND;
			}
			break;
		case WENNYOLDMAN_AUTO_STATE_END:
			{
				m_siAutoState = WENNYOLDMAN_AUTO_STATE_NONE;
				m_pNWennyButtonRecieve->Enable( true );
				m_pNWennyButtonDisAssemble->Enable( true );
			}
			break;
		}
	}
}



void CALLBACK CNWennyOldMan::StaticNWennyOldManProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
		
	CNWennyOldMan *pThis = (CNWennyOldMan*)pControllerMgr;
	pThis->NWennyOldManProc( nEvent, nControlID, pControl );
	
}



void CALLBACK CNWennyOldMan::NWennyOldManProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;


	switch( nControlID )
	{
		
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case NWENNYMAN_LISTVIEW_MYINVENTORY:
			{			
				switch(nEvent)	
				{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
		                m_siSelectIndexMyInven = m_pMyInventory->GetSelectedItemIndex();
					}
					break;
				}
			}
			break;
		case NWENNYMAN_LISTVIEW_WENNYSTORAGE:
			{			
				switch(nEvent)	
				{
				case EVENT_LISTVIEW_LBUTTONDOWN:
					{
						m_siSelectIndexStorage = m_pWennyStorage->GetSelectedItemIndex();
					}
					break;
				}
			}
			break;
		case NWENNYMAN_BUTTON_DISASSEMBLE:
			{			
				switch(nEvent)	
				{
				case EVENT_BUTTON_CLICKED:
					{
						if (m_siSelectIndexMyInven >= 0)
						{
							if (TABS(pclClient->CurrentClock - m_SendDisassembleClock) > 3000 )
							{
								SI32 itempos = m_siSelectIndexMyInven;
								if (itempos >= PERSONITEM_INV0 && itempos < MAX_ITEM_PER_PERSON)
								{
									if (m_siStorageCount < MAX_ITEM_PER_WENNYOLD)
									{
										cltGameMsgRequest_ItemDisAssemble clItemDisAssem(itempos, &m_clUserItem[itempos]);
										cltMsg clMsg(GAMEMSG_REQUEST_DISASSEMBLEITEM, sizeof(clItemDisAssem), (BYTE*)&clItemDisAssem);
										pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

										m_pNWennyButtonDisAssemble->Enable( false );
										m_SendDisassembleClock = pclClient->CurrentClock;
									}
									else
									{
										TCHAR *pText = GetTxtFromMgr(5942);
										SetWennyText(pText);
									}

								}

								m_sendFrame = pclClient->GetFrame();
							}
						}

					}
					break;
				}
			}
			break;
		case NWENNYMAN_BUTTON_RECIEVEITEM:
			{			
				switch(nEvent)	
				{
				case EVENT_BUTTON_CLICKED:
					{	
						m_siAutoState = WENNYOLDMAN_AUTO_STATE_BUTTON_CLICK;
						m_pNWennyButtonRecieve->Enable( false );
						m_pNWennyButtonDisAssemble->Enable( false );
/*						if (m_siSelectIndexStorage >= 0)
						{
							SI32 stgpos = m_siSelectIndexStorage;
							if (stgpos >= 0 && stgpos < MAX_ITEM_PER_WENNYOLD)
							{
								cltGameMsgRequest_ItemDisAssemble_Recieve clItem(stgpos, &m_clWennyItem[stgpos]);
								cltMsg clMsg(GAMEMSG_REQUEST_DISASSEMBLEITEM_RECIEVE, sizeof(clItem), (BYTE*)&clItem);
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

							}
						}*/
					}
					break;
				}
			}
			break;

	}

	return ;
}

void CNWennyOldMan::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	
		{
		}
		break;

	}
}

BOOL CNWennyOldMan::IsChangedInvItem()
{
	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)&m_clUserItem[ j ];
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

void CNWennyOldMan::LoadDisAssembleItemData()
{

	cltClient *pclclient = (cltClient*)pclClient;
	m_pPersonItemInfo =& pclclient->pclCM->CR[1]->pclCI->clCharItem;
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	m_siSelectIndexMyInven =-1;
	SI32 StartPos=15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
	SI32 id = 1;
	m_pMyInventory->DeleteAllItems();
	m_pMyInventory->SetStartItemPos( StartPos );	
	SI16 Counter = 0;
	TCHAR ItemName[128]=TEXT("");
	TCHAR buffer[ 256 ] = TEXT("");

	for ( SI32 i = 0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_siInventoryPos[i] = -1;
	}

	for ( SI32 i = 0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_clUserItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );			
		if ( m_clUserItem[ i ].siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clUserItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clUserItem[ i ].siUnique );

				if ( ItemName)
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_clUserItem[ i ], ToolTipMsg );

					if(m_clUserItem[ i ].IsDisassemble(pclClient->pclItemManager))
					{
						stListViewItemData ListViewItemData;
						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_clUserItem[ i ].siItemNum, ItemName, ToolTipMsg );														
						m_pMyInventory->InsertItem( &ListViewItemData );

						m_siInventoryPos[ Counter ] = i;
						Counter++;
					}
				}
			}
		}
	}//end for		

	return;
}

void CNWennyOldMan::SetWeennyStorage(cltItem* itemlist)
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	m_siSelectIndexStorage = -1;
	m_siStorageCount = 0;

	m_pWennyStorage->DeleteAllItems();
	m_pWennyStorage->SetStartItemPos( 0 );	

	for ( SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; ++i )
	{
		m_clWennyItem[ i ].Set( &itemlist[ i ] );			
		if ( m_clWennyItem[ i ].siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clWennyItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clWennyItem[ i ].siUnique );

				if (ItemName)
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &m_clWennyItem[ i ], ToolTipMsg );

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i, m_clWennyItem[ i ].siItemNum, ItemName, ToolTipMsg );
					m_pWennyStorage->InsertItem( &ListViewItemData );

				}
			}

			m_siStorageCount++;
		}

	}

	if(m_siAutoState == WENNYOLDMAN_AUTO_STATE_WAITRECV)
	{
		m_siAutoState = WENNYOLDMAN_AUTO_STATE_RECV;
	}

}


void CNWennyOldMan::SetWennyText(TCHAR* ptext)
{
	if (ptext)
	{
		m_pNWennyStaticGreeting->SetText(ptext);
		m_pNWennyStaticGreeting->Refresh();
	}
}

void CNWennyOldMan::LoadWeenyOldManInven()
{
	cltGameMsgRequest_ItemDisAssemble_List clitemlist;
	cltMsg clMsg(GAMEMSG_REQUEST_DISASSEMBLEITEM_LIST, sizeof(clitemlist), (BYTE*)&clitemlist);

	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}