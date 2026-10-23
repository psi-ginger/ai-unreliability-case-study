#include "../Client/ninterface/NSpaceBoxDlg/NSpaceBoxDlg.h"

#include "../CommonLogic/MsgType-Item.h"
#include "../../Server/GachaManager/GachaManager.h"
#include "../../Server/Gacha2Manager/Gacha2Manager.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../Interface/LoginNoticeDlg/LoginNoticeDlg.h"

#include "../lib/MD5/md5.h"

extern CLoginNoticeDlg	g_LoginNoticeDlg;

//extern SI32		gsiResolution;


CNSpaceBoxDlg::CNSpaceBoxDlg()
{
	m_BoxViewType = SPACE_BOX;
	
	if(false == m_Spr.LoadTSprFromFile("NInterface/image/SpaceBoxListDisable.SPR"))
	{
		MsgBox("파일열기 실패", "SpaceBoxListDisable.SPR");
	}
}

CNSpaceBoxDlg::~CNSpaceBoxDlg()
{
	
}

void CNSpaceBoxDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NSpaceBoxDlg/NSpaceBoxDlg.ddf" ) );
	file.CreatePopupDialog( this, NSPACE_BOX, TEXT( "dialog_space_box" ), StaticSpaceBoxDlgProc );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  	NSPACE_BOX_STATIC_END_TIME,	 		this ), NSPACE_BOX_STATIC_END_TIME,			TEXT( "statictext_end_time" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  	NSPACE_BOX_STATIC_MY_INVENTORY, 	this ), NSPACE_BOX_STATIC_MY_INVENTORY,		TEXT( "statictext_my_inventory" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  	NSPACE_BOX_STATIC_SPACE_BOX,	 	this ), NSPACE_BOX_STATIC_SPACE_BOX,	 	TEXT( "statictext_space_box" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  	NSPACE_BOX_EDIT_EXPLAIN,		 	this ), NSPACE_BOX_EDIT_EXPLAIN,		 	TEXT( "statictext_explain" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  	NSPACE_BOX_BUTTON_SPACE_BOX,		this ), NSPACE_BOX_BUTTON_SPACE_BOX,   		TEXT( "button_space_box" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  	NSPACE_BOX_BUTTON_TREASURE,			this ), NSPACE_BOX_BUTTON_TREASURE,	   		TEXT( "button_treasure" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  	NSPACE_BOX_BUTTON_BUY,				this ), NSPACE_BOX_BUTTON_BUY,		   		TEXT( "button_buy" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  	NSPACE_BOX_BUTTON_KEEP,				this ), NSPACE_BOX_BUTTON_KEEP,		   		TEXT( "button_keep" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  	NSPACE_BOX_BUTTON_RECOVERY,			this ), NSPACE_BOX_BUTTON_RECOVERY,	   		TEXT( "button_recovery" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NSPACE_BOX_LISTVIEW_MY_INVENTORY,	this ), NSPACE_BOX_LISTVIEW_MY_INVENTORY,	TEXT( "listview_my_inventory" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LISTVIEW,	NSPACE_BOX_LISTVIEW_RECOVERY,		this ), NSPACE_BOX_LISTVIEW_RECOVERY,		TEXT( "listview_space_box" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NSPACE_BOX_EDIT_ITEM_COUNT,			this ),	NSPACE_BOX_EDIT_ITEM_COUNT,			TEXT( "editbox_item_count" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  	NSPACE_BOX_OUTLINE_LINE,			this ), NSPACE_BOX_OUTLINE_LINE,			TEXT( "outline_line" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  	NSPACE_BOX_OUTLINE_LINE2, 			this ), NSPACE_BOX_OUTLINE_LINE2,			TEXT( "outline_line2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  	NSPACE_BOX_OUTLINE_LINE3, 			this ), NSPACE_BOX_OUTLINE_LINE3,			TEXT( "outline_line3" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  	NSPACE_BOX_OUTLINE_LINE4, 			this ), NSPACE_BOX_OUTLINE_LINE4,			TEXT( "outline_line4" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  	NSPACE_BOX_OUTLINE_LINE5, 			this ), NSPACE_BOX_OUTLINE_LINE5,			TEXT( "outline_line5" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,  	NSPACE_BOX_OUTLINE_LINE6, 			this ), NSPACE_BOX_OUTLINE_LINE6,			TEXT( "outline_line6" ) );	

	// 내 인벤에 아이템 세팅.
	SetMyListView();
	SetSpaceBoxListView();
	
	m_siKeepPos		= -1;
	m_siRecoveryPos	= -1;
	
	// 자주 호출되므로 멤버로 사용.
	m_pKeepButton = m_InterfaceFactory.GetButton( NSPACE_BOX_BUTTON_KEEP );
	
	SetRemainTime();

	TextMsg(MSG_EXPLAIN); 

	m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_SPACE_BOX)->SetFontWeight(FW_BOLD);
	m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_SPACE_BOX)->Refresh(); 
}

void CNSpaceBoxDlg::Destroy()
{
	
}

void CNSpaceBoxDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{	
	// 비활성화 상태이면 
	if( false == m_pKeepButton->IsAvailable() && SPACE_BOX == m_BoxViewType )
	{
		GP.PutSprAlpha(&m_Spr, GetX()+18 + ScreenRX , GetY()+250 + ScreenRY, 7); 
	}
}

void CALLBACK CNSpaceBoxDlg::StaticSpaceBoxDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNSpaceBoxDlg *pThis = (CNSpaceBoxDlg*) pControllerMgr;
	pThis->SpaceBoxDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNSpaceBoxDlg::SpaceBoxDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = ( cltClient * ) pclClient;

	TextMsg(MSG_EXPLAIN);

	if(pclClient->pclCM->CR[1]->pclSpaceBoxItem == NULL)		return;
	if(pclClient->pclCM->CR[1]->pclTreasureBoxItem == NULL)		return;

	switch ( nControlID )
	{
		case NSPACE_BOX_EDIT_ITEM_COUNT:
			{
				((cltClient*)pclClient)->MouseMoveAction(100, 100);

				CEdit* pItemCountEdit = m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT ); 
				SI32   ItemCount	  = atoi( pItemCountEdit->GetText() );
				
				if( 10000 < ItemCount )
				{
					pItemCountEdit->SetText("10000");
				}
				else if( 0 >= ItemCount )
				{
					pItemCountEdit->SetText("");
				}
				else
				{
					TCHAR szCount[6] = "";
					itoa(ItemCount, szCount, 10);
					pItemCountEdit->SetText(szCount);
				}
			}
			break;
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;	
		case NSPACE_BOX_BUTTON_SPACE_BOX:
			{
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_SPACE_BOX)->SetFontWeight(FW_BOLD);
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_SPACE_BOX)->Refresh();
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_TREASURE)->SetFontWeight(FW_NORMAL);
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_TREASURE)->Refresh();
				m_InterfaceFactory.GetStatic(NSPACE_BOX_STATIC_END_TIME)->Show(TRUE);

				m_BoxViewType = SPACE_BOX;
				
				SetSpaceBoxListView();
			}
			break;
		case NSPACE_BOX_BUTTON_TREASURE:
			{
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_SPACE_BOX)->SetFontWeight(FW_NORMAL);
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_SPACE_BOX)->Refresh();
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_TREASURE)->SetFontWeight(FW_BOLD);
				m_InterfaceFactory.GetButton(NSPACE_BOX_BUTTON_TREASURE)->Refresh(); 
				m_InterfaceFactory.GetStatic(NSPACE_BOX_STATIC_END_TIME)->Show(FALSE); 

				m_BoxViewType = TREASURE_BOX;

				SetTreasureBoxListView();
			}
			break;
		case NSPACE_BOX_BUTTON_BUY:
			{
				if( pclClient->IsWhereServiceArea(ConstServiceArea_China) )
				{
					if( BASIC_PLAYERMASTER_LEVEL <= pclClient->pclCM->CR[1]->GetGameMasterLevel() )
					{
						TCHAR* pTitle = GetTxtFromMgr(4314);
						TCHAR* pText = GetTxtFromMgr(7547);

						stMsgBox MsgBox;
						MsgBox.Set( this, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
						pclclient->SetMsgBox( &MsgBox, NULL, 0 );
						break;
					}
					else
					{
						pclclient->ShowHelpWindow(TEXT("NULL"));
						break;
					}
				}
				else
				{
					if( pclClient->bItemMallUseSwitch == FALSE)
					{	
						TCHAR* pText = GetTxtFromMgr(962);

						if ( pclclient->m_pDialog[ NOTIFYMSG_DLG ] )
						{
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(TEXT("ItemMall"),pText, pclclient->clSelectedWorldInfo.baseinfo.worldname);
							break;
						}
					}
					else
					{
						if(pclClient->IsCountrySwitch(Switch_UseWebPageItemMall))
						{
							if(pclClient->IsWhereServiceArea(ConstServiceArea_Taiwan))
							{
								cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];

								UI08 md5buf[256] = "" ;
								char buffermd5[256] = "" ;

								TCHAR	szURL[1024];
								TCHAR	szBeforeMD5[256];

								sprintf(szBeforeMD5,  "%d%d%s%s%s", 100, pclchar->pclCI->clBI.siIDNum, pclchar->pclCI->clBI.szAccountID, pclchar->GetName(), "rnswndhsfkdlseoqkr");

								memset(md5buf, 0, sizeof(md5buf));
								// MD5
								SI32 md5size = _tcslen(szBeforeMD5);
								memcpy(md5buf, szBeforeMD5, md5size);
								md5_string(md5buf, buffermd5, md5size);

								sprintf(szURL, "http://bill4.omg.com.tw/charge/goonzu/goonzuitemlist.asp?server_index=%d&m_idPlayer=%d&m_szName=%s&user_id=%s&md5=%s",
									100, pclchar->pclCI->clBI.siIDNum, pclchar->GetName(), pclchar->pclCI->clBI.szAccountID , buffermd5);

								g_LoginNoticeDlg.Set(szURL, 970, 756);
							}
						}
						else
						{
							pclclient->CreateInterface(NNEWITEMMALL_DLG);	
						}
						break;
					}	
				}
			}
			break;
		case NSPACE_BOX_BUTTON_KEEP:
			{
				// 개인상점을 열고 있으면 아이템 선택을 못하도록 했다
				if ( (true == pclClient->pclCM->CR[1]->bPersonalShopSwitch) || 
						(true == pclClient->pclCM->CR[1]->bPremiumPersonalShop) )
				{
					TCHAR* pTitle = GetTxtFromMgr(1891);
					TCHAR* pText = GetTxtFromMgr(10038);
					pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					break ;
				}
				
				TCHAR* pItemCount  = m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->GetText(); 
				SI32   siItemCount = atoi( pItemCount );	

				// 저장할 물건이 없으면.
				if( m_siKeepPos < 0 )
				{
					TextMsg(MSG_NO_MOVE_ITEM_SELECT);

					return;	
				}

				if( 0 >= siItemCount || 10000 < siItemCount)
				{
					TextMsg(MSG_ITEM_MOVE_COUNT_RANGE);

					return;
				}
				// 밀봉프리미엄 사용중은 넣을수 없다
				if ( pclClient->pclCM->CR[1]->bPremiumPersonalShop == true) 
				{
					return	;
				}

				cltItem* pSelectItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[m_siKeepPos];

				if( pSelectItem->siItemNum < siItemCount)
				{
					TextMsg(MSG_OVER_COUNT);

					return;
				}

				bool Gacha = false;
				
				// 가차 아이템 이라면 보물 상자로.
				if( true == pclClient->pclGachaManager->IsGachaRareItem( pSelectItem->siUnique ) )
				{
					Gacha = true;
				}
				
				// 가챠 아이템은 보물 박스가 설정되있을 때 저장.
                if( true == Gacha )
				{
					if( TREASURE_BOX != m_BoxViewType )
					{
						TextMsg(MSG_GACHA_ITEM_DIFFICULT_BOX);

						return;
					}
				}
				// 기냥 아이템은 공간 박스가 설정되있을 때 저장.
				else
				{
					if( SPACE_BOX != m_BoxViewType )
					{
						TextMsg(MSG_NOMAL_ITEM_DIFFICULT_BOX);

						return;
					}
				}
								
				// 저장하는 아이템.
				cltItem KeepItem;
				KeepItem.Init();
				KeepItem.Set( pSelectItem );
				KeepItem.siItemNum = siItemCount;

				// 서버로 아이템 사용 요청을 보낸다. 
				cltGameMsgRequest_Keep_Box_Move_Item clInfo( Gacha, m_siKeepPos, &KeepItem );
				cltMsg clMsg( GAMEMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM, sizeof(cltGameMsgRequest_Keep_Box_Move_Item), (BYTE*)&clInfo );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

				// 다시 초기화.
				m_siKeepPos = -1;
				m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->SetText("0");
			}
			break;
		case NSPACE_BOX_BUTTON_RECOVERY:
			{
				TCHAR* pItemCount  = m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->GetText(); 
				SI32   siItemCount = atoi( pItemCount );	

				// 회수할 물건이 없으면.
				if( m_siRecoveryPos < 0 )
				{
					TextMsg(MSG_NO_MOVE_ITEM_SELECT);
					
					return;
				}

				if( 0 >= siItemCount || 10000 < siItemCount)
				{
					TextMsg(MSG_ITEM_MOVE_COUNT_RANGE);

					return;
				}

				cltItem* pSelectItem = NULL;

				if( SPACE_BOX == m_BoxViewType )
				{
					pSelectItem = &pclClient->pclCM->CR[1]->pclSpaceBoxItem->clItem[m_siRecoveryPos];
				}
				else if( TREASURE_BOX == m_BoxViewType )
				{
					pSelectItem = &pclClient->pclCM->CR[1]->pclTreasureBoxItem->clItem[m_siRecoveryPos];
				}
				
				bool Gacha = false;

				// 가차 아이템 이라면 보물 상자로.
				if( true == pclClient->pclGachaManager->IsGachaRareItem( pSelectItem->siUnique ) )
				{
					Gacha = true;
				}
			
				if( pSelectItem->siItemNum < siItemCount)
				{
					TextMsg(MSG_OVER_COUNT);

					return;
				}

				// 저장하는 아이템.
				cltItem RecoveryItem;
				RecoveryItem.Init();
				RecoveryItem.Set( pSelectItem );
				RecoveryItem.siItemNum = siItemCount;

				cltGameMsgRequest_Recovery_Box_Move_Item clInfo( Gacha, m_siRecoveryPos, &RecoveryItem );
				cltMsg clMsg( GAMEMSG_REQUEST_RECOVERY_SPACE_BOX_MOVE_ITEM, sizeof(cltGameMsgRequest_Recovery_Box_Move_Item), (BYTE*)&clInfo );
				pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );

				// 다시 초기화.
				m_siRecoveryPos = -1;
				m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->SetText("0");
			}
			break;
		case NSPACE_BOX_LISTVIEW_MY_INVENTORY:	// 유저 인벤 리스트.
			{
				if( false == m_pKeepButton->IsAvailable() )
				{
					TextMsg(MSG_TIME_OVER_BOX);
					break;
				}
				
				switch( nEvent )
				{
					case EVENT_LISTVIEW_LBUTTONDOWN:
						{
							CListView*  m_pListView = m_InterfaceFactory.GetListView( NSPACE_BOX_LISTVIEW_MY_INVENTORY ); 
							
							SI32 TempUserInventorySelectedIndex = m_pListView->GetSelectedItemIndex();

							if(TempUserInventorySelectedIndex < 0)
								break;
							
							m_siKeepPos		= TempUserInventorySelectedIndex;
							m_siRecoveryPos	= -1;

							cltItem* pSelectItem = &pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[m_siKeepPos];
							
							TCHAR buffer[8] = "";
							itoa(pSelectItem->siItemNum, buffer, 10);
							m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->SetText(buffer);
						}
						break;
					}
			}
			break;
		case NSPACE_BOX_LISTVIEW_RECOVERY:	// 창고 인벤 리스트.
			{
				switch( nEvent )
				{
					case EVENT_LISTVIEW_LBUTTONDOWN:
						{
							CListView*  m_pListView = m_InterfaceFactory.GetListView( NSPACE_BOX_LISTVIEW_RECOVERY ); 

							SI32 TempUserInventorySelectedIndex = m_pListView->GetSelectedItemIndex();

							if(TempUserInventorySelectedIndex < 0)
								break;

							m_siRecoveryPos = TempUserInventorySelectedIndex;
							m_siKeepPos		= -1;

							cltItem* pSelectItem = NULL;

							if( SPACE_BOX == m_BoxViewType )
							{
								pSelectItem = &pclClient->pclCM->CR[1]->pclSpaceBoxItem->clItem[m_siRecoveryPos];
							}
							else if( TREASURE_BOX == m_BoxViewType )
							{
								pSelectItem = &pclClient->pclCM->CR[1]->pclTreasureBoxItem->clItem[m_siRecoveryPos];
							}
							
							TCHAR buffer[8] = "";
							itoa(pSelectItem->siItemNum, buffer, 10);
							m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->SetText(buffer);
						}
						break;
				}
			}
	}
	
	SetRemainTime();
}

void CNSpaceBoxDlg::SetMyListView(void)
{
	CListView*  m_pListView = m_InterfaceFactory.GetListView( NSPACE_BOX_LISTVIEW_MY_INVENTORY ); 

	cltPItemInfo*	pPersonItemInfo = &pclClient->pclCM->CR[1]->pclCI->clCharItem;

	SI32 m_siUserInventorySelectedIndex = -1;

	SI32 i;
	for ( i = PERSONITEM_INV0; i < MAX_ITEM_PER_PERSON; ++i )
	{
		m_clUserItem[ i ] = pPersonItemInfo->clItem[ i ];
	}

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0;
	SI32 id = 1;

	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );

	bool bIsSealed      = false;
	SI32 index			= 0;
	TCHAR ItemName[128] = "";
	for(i = PERSONITEM_INV0;i < PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON; ++i)
	{
		if(m_clUserItem[i].siUnique > 0)
		{
			if( pPersonItemInfo->clItem[i].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pPersonItemInfo->clItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &pPersonItemInfo->clItem[ i ], ToolTipMsg );

					bIsSealed = pPersonItemInfo->clItem[i].IsSealingUp(pclClient->pclItemManager);

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,pPersonItemInfo->clItem[i].GetItemNum(), ItemName, ToolTipMsg, false, bIsSealed );														
					m_pListView->InsertItem( &ListViewItemData );
				}
			}
		}
	}
}

// 공간 박스 세팅.
void CNSpaceBoxDlg::SetSpaceBoxListView(void)
{	
	// 초기화.
	m_siKeepPos		= -1;
	m_siRecoveryPos = -1;
	m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->SetText("0");

	CListView*  m_pListView = m_InterfaceFactory.GetListView( NSPACE_BOX_LISTVIEW_RECOVERY ); 

	SI32 m_siUserInventorySelectedIndex = -1;
	SI32 ItemCount = 0;

	cltSpaceBox*	pSpaceBoxInfo = NULL;
	
	pSpaceBoxInfo	= pclClient->pclCM->CR[1]->pclSpaceBoxItem;
	if(pSpaceBoxInfo == NULL)			return;
	ItemCount		= MAX_ITEM_SPACE_BOX; 
		
	SI32 i;
	for ( i = 0; i < MAX_ITEM_SPACE_BOX; ++i )
	{
		m_clUserItem[ i ] = pSpaceBoxInfo->clItem[ i ];
	}

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont  = -1;
	SI32 StartPos		  =  0;
	SI32 id				  =  1;

	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );


	bool  bIsSealed     = false; 
	SI32  index			= 0;
	TCHAR ItemName[128] = "";

	for ( i = 0; i < MAX_ITEM_SPACE_BOX; ++i )
	{
		if(m_clUserItem[i].siUnique > 0)
		{
			if( pSpaceBoxInfo->clItem[i].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pSpaceBoxInfo->clItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &pSpaceBoxInfo->clItem[ i ], ToolTipMsg );

					bIsSealed = pSpaceBoxInfo->clItem[i].IsSealingUp(pclClient->pclItemManager);

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,pSpaceBoxInfo->clItem[i].GetItemNum(), ItemName, ToolTipMsg, false, bIsSealed );										
					m_pListView->InsertItem( &ListViewItemData );
				}
			}
		}
	}

	NTCHARString64 kBuf(GetTxtFromMgr(7931));

	SI16 siArrayCount = sizeof(pSpaceBoxInfo->clItem) / sizeof(pSpaceBoxInfo->clItem[0]);
	SI16 BoxItemCount = GetBoxItemCount(SPACE_BOX, &pSpaceBoxInfo->clItem[0], siArrayCount);

	kBuf.Replace( "#count#",      SI16ToString( BoxItemCount ) );
	kBuf.Replace( "#totalcount#", SI16ToString( MAX_ITEM_TREASURE_BOX ) ); 

	m_InterfaceFactory.GetStatic( NSPACE_BOX_STATIC_SPACE_BOX )->SetText( kBuf );
}

// 보물 박스 세팅.
void CNSpaceBoxDlg::SetTreasureBoxListView(void)
{
	// 초기화.
	m_siKeepPos		= -1;
	m_siRecoveryPos = -1;
	m_InterfaceFactory.GetEdit( NSPACE_BOX_EDIT_ITEM_COUNT )->SetText("0");

	CListView*  m_pListView = m_InterfaceFactory.GetListView( NSPACE_BOX_LISTVIEW_RECOVERY ); 

	SI32 m_siUserInventorySelectedIndex = -1;
	SI32 ItemCount = 0;

	cltTreasureBox*	pTreasureBoxInfo = NULL;
	pTreasureBoxInfo = pclClient->pclCM->CR[1]->pclTreasureBoxItem;
	if(pTreasureBoxInfo == NULL)		return;
	
	SI32 i;
	for ( i = 0; i < MAX_ITEM_TREASURE_BOX; ++i )
	{
		m_clUserItem[ i ] = pTreasureBoxInfo->clItem[ i ];
	}

	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont  = -1;
	SI32 StartPos		  =  0;
	SI32 id				  =  1;

	m_pListView->DeleteAllItems();
	m_pListView->SetStartItemPos( StartPos );

	bool  bIsSealed     = false;
	SI32  index			= 0;
	TCHAR ItemName[128] = "";
	for ( i = 0; i < MAX_ITEM_TREASURE_BOX; ++i )
	{
		if(m_clUserItem[i].siUnique > 0)
		{
			if( pTreasureBoxInfo->clItem[i].GetFullName( pclClient->pclItemManager, ItemName, 128 ) == true )
			{
				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( pTreasureBoxInfo->clItem[i].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					pclClient->pclItemManager->GetExplain( &pTreasureBoxInfo->clItem[ i ], ToolTipMsg );

					bIsSealed = pTreasureBoxInfo->clItem[i].IsSealingUp(pclClient->pclItemManager);

					stListViewItemData ListViewItemData;
					ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos ,pTreasureBoxInfo->clItem[i].GetItemNum(), ItemName, ToolTipMsg, false, bIsSealed );														
					m_pListView->InsertItem( &ListViewItemData );
				}
			}
		}
	}

	NTCHARString64 kBuf(GetTxtFromMgr(7932));
	SI16 siArrayCount = sizeof(pTreasureBoxInfo->clItem) / sizeof(pTreasureBoxInfo->clItem[0]);
    SI16 BoxItemCount = GetBoxItemCount(TREASURE_BOX, &pTreasureBoxInfo->clItem[0], siArrayCount);

	kBuf.Replace( "#count#",      SI16ToString( BoxItemCount ) );
	kBuf.Replace( "#totalcount#", SI16ToString( MAX_ITEM_TREASURE_BOX ) );

	m_InterfaceFactory.GetStatic( NSPACE_BOX_STATIC_SPACE_BOX )->SetText( kBuf );
}

void CNSpaceBoxDlg::SetRemainTime(void)
{
	// 기간 보이기.
	if( true == pclClient->pclCM->CR[1]->pclCI->clBI.bSpaceBoxPremiumService )
	{
		cltDate cldate ;
		cldate.MakeFullDateFromDateVary( pclClient->pclCM->CR[1]->pclCI->clBI.siSpaceBoxPremiumServiceDate );

		NTCHARString256	kBuf( GetTxtFromMgr(7923) );
		kBuf.Replace("#year#",  SI32ToString(cldate.uiYear));
		kBuf.Replace("#month#", SI32ToString(cldate.uiMonth));
		kBuf.Replace("#day#",   SI32ToString(cldate.uiDay));

		m_InterfaceFactory.GetStatic(NSPACE_BOX_STATIC_END_TIME)->SetText(kBuf);

		// 보관 버튼 활성화
		m_InterfaceFactory.GetButton( NSPACE_BOX_BUTTON_KEEP )->Enable(true); 
		m_InterfaceFactory.GetButton( NSPACE_BOX_BUTTON_KEEP )->SetFontColor(COLORREF(0x00000000));
	}
	// 기간 만료.
	else
	{
		m_InterfaceFactory.GetStatic(NSPACE_BOX_STATIC_END_TIME)->SetText(GetTxtFromMgr(7928));

		if( SPACE_BOX == m_BoxViewType )
		{		
			// 보관 버튼 비활성화.
			m_InterfaceFactory.GetButton( NSPACE_BOX_BUTTON_KEEP )->Enable(false); 
			m_InterfaceFactory.GetButton( NSPACE_BOX_BUTTON_KEEP )->SetFontColor(COLORREF(0x22222222));
		}
		else
		{
			// 보관 버튼 활성화
			m_InterfaceFactory.GetButton( NSPACE_BOX_BUTTON_KEEP )->Enable(true); 
			m_InterfaceFactory.GetButton( NSPACE_BOX_BUTTON_KEEP )->SetFontColor(COLORREF(0x00000000));
		}
	}
}

void CNSpaceBoxDlg::TextMsg(SI32 _MsgType)
{
	NTCHARString256	kBuf;
	switch( _MsgType )
	{
		case MSG_EXPLAIN:
			{
				NTCHARString256 kBuf1 = GetTxtFromMgr(7927);
				kBuf1.Replace("#item#", GetTxtFromMgr(7930)); 

				kBuf = GetTxtFromMgr(7926);
				kBuf.Replace("#subexplain#", kBuf1); 
			}
			break;
		case MSG_TIME_OVER_BOX: 
			{
				kBuf = GetTxtFromMgr(7915);
			}
			break;
		case MSG_FULL_BOX:
			{
				kBuf = GetTxtFromMgr(7916);
			}
			break;
		case MSG_NO_MOVE_ITEM_SELECT:
			{
				kBuf = GetTxtFromMgr(7917);
			}
			break;
		case MSG_OVER_COUNT:
			{
				kBuf = GetTxtFromMgr(7918);
			}
			break;
		case MSG_GACHA_ITEM_DIFFICULT_BOX:
			{
				kBuf = GetTxtFromMgr(7919);
			}
			break;
		case MSG_NOMAL_ITEM_DIFFICULT_BOX:
			{
				kBuf = GetTxtFromMgr(7920);
			}
			break;
		case MSG_ITEM_MOVE_COUNT_RANGE:
			{
				kBuf = GetTxtFromMgr(7929);
			}
			break;
	}

	m_InterfaceFactory.GetStatic( NSPACE_BOX_EDIT_EXPLAIN )->SetText(kBuf);
}