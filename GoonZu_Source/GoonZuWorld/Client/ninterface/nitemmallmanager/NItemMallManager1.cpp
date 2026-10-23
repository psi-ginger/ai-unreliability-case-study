#include "NItemMallManager1.h"

#include "Client.h"

//-----------------------------
// Common
//-----------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"
#include "Resource.h"
#include "CommonLogic.h"

//#include "..\..\..\Common\SMLib\LibList\LibList.h"
//#include "..\..\..\Common\PrivateTradeOrderMgr\PrivateTradeOrderObj.h"

#include "../../InterfaceMgr/interface/Static.h"
#include "../../InterfaceMgr/interface/ListView.h"
#include "../../InterfaceMgr/interface/Button.h"
#include "../../InterfaceMgr/interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "MsgType.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "NItemMallGiveStatus.h"


extern cltCommonLogic* pclClient;


CNItemMallManager1::CNItemMallManager1()
{
	m_pNItemMallStatic1 = NULL;
	m_pNItemMallStatic2 = NULL;
	//m_pNItemMallStatic3 = NULL;
//	m_pNItemMallStatic4 = NULL;
	m_pNItemMallStatic5 = NULL;
	m_pNItemMallStatic6 = NULL;
	m_pNItemMallStatic7 = NULL;
	m_pNItemMallStatic8 = NULL;
	m_pNItemMallStatic9 = NULL;
	m_pNItemMallButton1 = NULL; 
	m_pNItemMallButton2 = NULL;
	m_pNItemMallButton3 = NULL;
	m_pNItemMallButton4 = NULL;
	m_pNItemMallButton5 = NULL;
	m_pNItemMallListView1 = NULL;
	m_pNItemMallListView2 = NULL;
	m_pNItemMallEdit = NULL;
	m_pclUserInvItem = new cltItem[ MAX_ITEM_PER_PERSON ];
	m_GetItem = new cltItem[ MAX_ITEMNUM_PER_ITEMMALL ];
	m_siViewPage = 1 ;
	m_dwLastClock = 0;
	SetTimerOnOff( true );	
}

CNItemMallManager1::~CNItemMallManager1()
{		
	SAFE_DELETE(m_pNItemMallStatic1);
	SAFE_DELETE(m_pNItemMallStatic2);
	//SAFE_DELETE(m_pNItemMallStatic3);
//	SAFE_DELETE(m_pNItemMallStatic4);
	SAFE_DELETE(m_pNItemMallStatic5);
	SAFE_DELETE(m_pNItemMallStatic6);
	SAFE_DELETE(m_pNItemMallStatic7);
	SAFE_DELETE(m_pNItemMallStatic8);
	SAFE_DELETE(m_pNItemMallStatic9);
	SAFE_DELETE(m_pNItemMallButton1);
	SAFE_DELETE(m_pNItemMallButton2);
	SAFE_DELETE(m_pNItemMallButton3); 
	SAFE_DELETE(m_pNItemMallButton4);
	SAFE_DELETE(m_pNItemMallButton5); 
	SAFE_DELETE(m_pNItemMallListView1); 
	SAFE_DELETE(m_pNItemMallListView2);
	SAFE_DELETE(m_pNItemMallEdit); 
	

	if( m_pclUserInvItem )
	{
		delete [] m_pclUserInvItem;
		m_pclUserInvItem = NULL;
	}

	if( m_GetItem )		
	{
		delete [] m_GetItem;
		m_GetItem = NULL;
	}
	SetTimerOnOff( false );
}



void CNItemMallManager1::Create()
{
	SetActionTimer(200);
	
	if(!IsCreate())
	{		
		CInterfaceFile file;

		file.LoadFile( TEXT("NInterface/Data/NItemMall/DLG_NItemMallManager1.ddf"));
		file.CreatePopupDialog( this, NITEMMALL_DLG,  TEXT("dialog_ItemMall"), StatiCNItemMallManager1Proc);		

		m_pNItemMallStatic1 = new CStatic(this);
		m_pNItemMallStatic2 = new CStatic(this);
//		m_pNItemMallStatic3 = new CStatic(this);
//		m_pNItemMallStatic4 = new CStatic(this);
		m_pNItemMallStatic5 = new CStatic(this);
		m_pNItemMallStatic6 = new CStatic(this);
		m_pNItemMallStatic7 = new CStatic(this);
		m_pNItemMallStatic8 = new CStatic(this);
		m_pNItemMallStatic9 = new CStatic(this);
		m_pNItemMallButton1 = new CButton(this);
		m_pNItemMallButton2 = new CButton(this);
		m_pNItemMallButton3 = new CButton(this);
		m_pNItemMallButton4 = new CButton(this);
		m_pNItemMallButton5 = new CButton(this);
		m_pNItemMallListView1 = new CListView(this);
		m_pNItemMallListView2 = new CListView(this);
		m_pNItemMallEdit = new CEdit(this);


		file.CreateControl(m_pNItemMallStatic1, NITEMMALL_STATIC1,  TEXT("statictext_NItemMall1"));
		file.CreateControl(m_pNItemMallStatic2, NITEMMALL_STATIC2,  TEXT("statictext_NItemMall2"));
		//file.CreateControl(m_pNItemMallStatic3, NITEMMALL_STATIC3,  TEXT("statictext_NItemMall3"));
		//file.CreateControl(m_pNItemMallStatic4, NITEMMALL_STATIC4,  TEXT("statictext_NItemMall4"));
		file.CreateControl(m_pNItemMallStatic5, NITEMMALL_STATIC5,  TEXT("statictext_NItemMall5"));
		file.CreateControl(m_pNItemMallStatic6, NITEMMALL_STATIC6,  TEXT("statictext_NItemMall6"));
		file.CreateControl(m_pNItemMallStatic7, NITEMMALL_STATIC7,  TEXT("statictext_NItemMall7"));
		file.CreateControl(m_pNItemMallStatic8, NITEMMALL_STATIC8,  TEXT("statictext_NItemMall8"));
		file.CreateControl(m_pNItemMallStatic9, NITEMMALL_STATIC9,  TEXT("statictext_NItemMall9"));
		file.CreateControl(m_pNItemMallButton1, NITEMMALL_BUTTON1,  TEXT("button_NItemMall1"));
		file.CreateControl(m_pNItemMallButton2, NITEMMALL_BUTTON2,  TEXT("button_NItemMall2"));
		file.CreateControl(m_pNItemMallButton3, NITEMMALL_BUTTON3,  TEXT("button_NItemMall3"));
		file.CreateControl(m_pNItemMallButton4, NITEMMALL_BUTTON4,  TEXT("button_NItemMall4"));
		file.CreateControl(m_pNItemMallButton5, NITEMMALL_BUTTON5,  TEXT("button_NItemMall5"));
		file.CreateControl(m_pNItemMallListView1,NITEMMALL_LISTVIEW1,  TEXT("listview_NItemMall1"));
		file.CreateControl(m_pNItemMallListView2,NITEMMALL_LISTVIEW2,  TEXT("listview_NItemMall2"));
		file.CreateControl(m_pNItemMallEdit, NITEMMALL_EDIT,  TEXT("editbox_NItemMall"));
		
	//	m_pNItemMallStatic4->SetBKColorUse(true);
	//	m_pNItemMallStatic4->SetBorder(true);
	//	m_pNItemMallStatic4->SetText(TEXT(" "),DT_WORDBREAK);
		
		m_pNItemMallButton3->Show(false);

		m_pNItemMallStatic7->SetBKColorUse(true);
		m_pNItemMallStatic7->SetBorder(true);
		m_pNItemMallStatic7->SetText(TEXT(" "),DT_WORDBREAK);
		
		m_pNItemMallStatic6->SetTextPrintOption(DT_LEFT);

		//m_pNItemMallEdit->SetTextPrintOption(DT_RIGHT);
		m_pNItemMallEdit->SetNumberMode(EDIT_INPUT_MODE_NUMBER);
		
		m_pNItemMallStatic8->SetFileName(TEXT("ItemMall/explain2.SPR"));
		m_pNItemMallStatic8->SetFontIndex(0);
		
	//	m_pNItemMallStatic4->Show(false);
		m_pNItemMallStatic7->Show(false);


		LoadInvItemData();
		ShowItemList();

		m_dwLastClock = 0;

		MovePos(350,50);
		SetTopMostWindow(true);

	}

}


void CNItemMallManager1::Action()
{		
 	if (IsChangedInvItem() )
	{
		LoadInvItemData();
		
		ShowItemList();
		//m_pInvItemList->SetItemToListView( pclClient->pclItemManager, m_hTabDlg[m_siViewDlg], IDC_LIST_ITEMMALL_INVEN, &m_InvItem[PERSONITEM_INV0], MAX_INVITEM_PER_PERSON );
	}
	//ShowItemList();
}


void CALLBACK CNItemMallManager1::StatiCNItemMallManager1Proc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNItemMallManager1 *pThis = (CNItemMallManager1*) pControllerMgr;
	pThis->NItemMallManager1Proc( nEvent, nControlID, pControl );
}

void CALLBACK CNItemMallManager1::NItemMallManager1Proc( UINT nEvent, int nControlID, CControllerObj* pControl )
{	
	cltClient *pclclient = (cltClient*)pclClient;
	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NITEMMALL_LISTVIEW1:
		{
			switch( nEvent )
			{
			case EVENT_LISTVIEW_LBUTTONDOWN:
				{
					SetTimerOnOff( false );
					TCHAR ItemExplain[ 512 ] = TEXT("");
					TCHAR Buffer[ 512 ] = TEXT("");

					m_pNItemMallEdit->Clear();

					m_siSelectItem = m_pNItemMallListView1->GetSelectedItemIndex();
					if( 0 > m_siSelectItem ) return;

					if(pclClient->pclItemManager->GetExplain( &m_GetItem[m_siSelectItem ],ItemExplain ) == TRUE)
					{
						StringCchPrintf( Buffer, 512, TEXT("%s"), ItemExplain );
					//	m_pNItemMallStatic4->SetText(Buffer,DT_WORDBREAK);
					}
			
					stListViewItemData *pItemData = m_pNItemMallListView1->GetListViewItemData(m_siSelectItem);
					if ( pItemData )
					{	
						StringCchPrintf( Buffer, 512, TEXT("%d"),pItemData->ItemNum );
						m_pNItemMallEdit->SetText(Buffer);

					}


					//SI16 itemunique = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItem(m_siSelectItem).siUnique ;
					//if ( pclClient->pclItemManager->CanPile(itemunique) == false ) 

					if (pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItem(m_siSelectItem).CanPile(pclClient->pclItemManager) == false) 
					{
						
						m_pNItemMallEdit->SetReadOnly(true);
						m_pNItemMallEdit->SetText(TEXT("1"));

					}
					else
					{
						m_pNItemMallEdit->SetReadOnly(false);
					}
					m_pNItemMallEdit->Refresh();

				
				}
				break;
			}
		}
		break;

	case NITEMMALL_BUTTON1:  //업데이트 
		{			
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetTimerOnOff( true );
					//ShowItemList();
					cltGameMsgRequest_ItemMallItemList clinfo(m_siViewPage);
					cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					
				}
				break;
			}					
		}
		break;
	case NITEMMALL_BUTTON2: //소지품으로 이동하기 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{   
					
					SetTimerOnOff( true );
					SI32 selectitem = m_siSelectItem ;
					if ( selectitem < 0 || selectitem >= MAX_ITEMNUM_PER_ITEMMALL ) break ;

					TCHAR buffer[256] = TEXT("") ;
					TCHAR buffer1[256] = TEXT("") ;
					SI32 itemid = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItemID(selectitem);

					SI32 itemnum = _tstoi(m_pNItemMallEdit->GetText());
					cltItem item = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItem(selectitem);
					cltItem *pclitem = &item;
				
					if ( pclitem->siItemNum < itemnum )
					{
						TCHAR* pText = GetTxtFromMgr(3877);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pText );
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetTopMostWindow(true);
						break;
					}
					
					// 아이템 남,녀 착용 제한이 있을경우, 다른 성별의 경우엔 받을수 없음.
					if( pclitem->siUnique > 0 )
					{
						SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pclitem->siUnique);

						cltItemConition * pclCondition = &pclClient->pclItemManager->pclItemInfo[ref]->clCondition;

						if( pclCondition->bForMan == true && pclCondition->bForWoman == false  )
						{
							SI32 kind = pclClient->pclCM->CR[1]->GetKind();
							if( kind == KIND_HEROINE1 ||  kind == KIND_HEROINE2 || kind == KIND_HEROINE3 || kind == KIND_HEROINE4 )
							{
								TCHAR* pText = GetTxtFromMgr(6355);

								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pText );

								break;
							}							
						}
						else if( pclCondition->bForMan == false && pclCondition->bForWoman == true )
						{
							SI32 kind = pclClient->pclCM->CR[1]->GetKind();
							if( kind == KIND_HERO1 ||  kind == KIND_HERO2 || kind == KIND_HERO3 || kind == KIND_HERO4 )
							{
								TCHAR* pText = GetTxtFromMgr(6356);

								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText(NULL, pText );

								break;
							}
						}
					}



					pclitem->siItemNum = itemnum ;
					BYTE TempBuffer[ 128 ];
					memset(TempBuffer,0,sizeof(TempBuffer));

					TCHAR* pText = GetTxtFromMgr(3878);

					TCHAR num[128]=TEXT(" ") ;

					StringCchPrintf(num, 128, TEXT("%d"),pclitem->siItemNum);
					Nsprintf(buffer,pText,TEXT("item"),  pclclient->pclItemManager->GetName(pclitem->siUnique),TEXT("num"), num   ,NULL);

					pText = GetTxtFromMgr(3874);
					StringCchPrintf(buffer1, 256, pText,
					pclclient->pclItemManager->GetName(pclitem->siUnique));
					StringCchCat(buffer, 256, buffer1);

					pText = GetTxtFromMgr(3728);

					stMsgBox MsgBox;
					MsgBox.Set( this,  pText, buffer, MSGBOX_TYPE_YESNO, 0 );					
					SI16 Size = sizeof(itemid ) + sizeof(itemnum )+sizeof(selectitem);
					memcpy( TempBuffer, &itemid, sizeof(itemid ) );
					memcpy( &TempBuffer[ sizeof( itemid ) ], &itemnum, sizeof( itemnum ) );
					memcpy( &TempBuffer[ sizeof( itemid )+sizeof( itemnum ) ], &selectitem, sizeof( selectitem ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );	


					break;						
				}
				break;
			}
		}
		break;
	case NITEMMALL_BUTTON3:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{  
					SetTimerOnOff( true );
					TCHAR url[ MAX_PATH ] = TEXT("") ;

					//pclClient->GetHelpSiteUrl( url, pclclient->pclCM->CR[ 1 ]->pclCI->clBI.szAccountID );
					
					if( g_bFullScreen)
						ShowWindow( pclclient->GetHwnd(), SW_MINIMIZE );


					//if ( _tcslen(url) > 0 && _tcscmp(url,TEXT("")) )
					if(pclClient->siServiceArea == ConstServiceArea_Japan)
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "https://www.kunshu.jp/support_email.aspx", NULL, SW_SHOWNORMAL );
					}
					else if(pclClient->siServiceArea == ConstServiceArea_Korea)
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://goonzus.ndoors.com/", NULL, SW_SHOWNORMAL );
					}
					else if(pclClient->siServiceArea == ConstServiceArea_China) //임시
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://global.goonzu.com", NULL, SW_SHOWNORMAL );
					}
					else if(pclClient->siServiceArea == ConstServiceArea_Taiwan) //임시
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://global.goonzu.com", NULL, SW_SHOWNORMAL );
					}
					else if(pclClient->siServiceArea == ConstServiceArea_USA) //임시
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ijji.com/", NULL, SW_SHOWNORMAL );
					}
					else if(pclClient->siServiceArea == ConstServiceArea_EUROPE) //임시
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://billing.aeriagames.com/index.asp", NULL, SW_SHOWNORMAL );
					}
					else if(pclClient->siServiceArea == ConstServiceArea_NHNChina) //임시
					{
						ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://http://www.newhjz.cn", NULL, SW_SHOWNORMAL );
					}
					else if(pclClient->siServiceArea == ConstServiceArea_English)
					{
						// PCK : 글로벌의 경우 NHN회원과 NDI회원에 따라서 보여지는 링크페이지가 다르다. (09.03.30)			
						NTCHARString32	kID(pclclient->szID);

						if( kID.Find("@nh") >= 0 )		// NHN 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ijji.com/", NULL, SW_SHOWNORMAL );
						}
						else							// NDI 회원이면
						{
							ShellExecute( NULL, NULL, TEXT("IEXPLORE.EXE"), "http://luminary.ndoorsgames.com/center/default.asp", NULL, SW_SHOWNORMAL );
						}
					}
				}
				break;
			}
		}
		break;
	case NITEMMALL_BUTTON4:  //이전페이지
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{  
					if(pclClient->CurrentClock - m_dwLastClock < 1000)
						return;
					
					m_dwLastClock = pclClient->CurrentClock;

					SetTimerOnOff( true );
					
					if ( m_siViewPage <= 1 ) break ;					
					cltGameMsgRequest_ItemMallItemList clinfo(m_siViewPage-1);
					cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					m_siViewPage=m_siViewPage-1;
				
				}
				break;
			}
		}
		break;
	case NITEMMALL_BUTTON5: //다음 페이지 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED: 
				{   
					if(pclClient->CurrentClock - m_dwLastClock < 1000)
						return;

					m_dwLastClock = pclClient->CurrentClock;

                    SetTimerOnOff( true );				
					cltGameMsgRequest_ItemMallItemList clinfo(m_siViewPage+1);
					cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLITEMLIST, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
					m_siViewPage=m_siViewPage+1;
					
				}
				break;
			}
		}
		break;

	}
	return;
}


	
BOOL CNItemMallManager1::IsChangedInvItem()
{
//	PERSONITEM_INV0


	for ( int j = PERSONITEM_INV0; j < MAX_ITEM_PER_PERSON; ++j )		// 장착창은 체크할 필요 없으므로, PERSONITEM_INV0 부터 함.
	{
		SI32 size = sizeof(cltItem);

		BYTE* src = (BYTE*)& m_pclUserInvItem[ j ];
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

	
void CNItemMallManager1::LoadInvItemData()
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=15;  
	SI32 EndPos=MAX_ITEM_PER_PERSON; 
	SI32 id = 1;
	m_pNItemMallListView2->SetBKColorUse(true);
	m_pNItemMallListView2->SetBorder(true);
	m_pNItemMallListView2->DeleteAllItems();
	m_pNItemMallListView2->SetStartItemPos( StartPos );
	
 
	for ( SI32 i = StartPos; i < EndPos; ++i )
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

					if ( i >= StartPos && i < StartPos + EndPos )
					{
						stListViewItemData ListViewItemmallData;
						ListViewItemmallData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_pclUserInvItem[ i ].siItemNum, ItemName, ToolTipMsg );														
						m_pNItemMallListView2->InsertItem( &ListViewItemmallData );
						
					}
				}
			}
		}
	}
	m_pNItemMallListView2->Refresh();
	return;
}

void CNItemMallManager1::LoadGetItemData()
{
	SI32 GlobalImageIndex = -1;
	SI32 GlobalImageFont = -1;
	SI32 StartPos=0; 
	
	SI32 id = 1;

	m_pNItemMallListView1->SetBKColorUse(true);
	m_pNItemMallListView1->SetBorder(true);
	m_pNItemMallListView1->DeleteAllItems();
	m_pNItemMallListView1->SetStartItemPos( StartPos );
	
	
	for ( int i = 0; i < MAX_ITEMNUM_PER_ITEMMALL; i++ )
	{
		m_GetItem[i].siUnique = 0 ;
		m_GetItem[i].siItemNum = 0 ;

		m_GetItem[i] = pclClient->pclCM->CR[1]->pclCI->clItemMall.GetItem(i);
	}



	for ( SI32 i = StartPos; i < MAX_ITEMNUM_PER_ITEMMALL; ++i )
	{
		//m_GetItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );			
		if ( m_GetItem[ i ].siUnique > 0 )
		{
			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_GetItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
			{
				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_GetItem[ i ].siUnique );

				if ( ItemName )
				{
					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
					
					// 기간제 아이템이라면 밀봉이 된것처럼 보여준다.

					m_GetItem[ i ].uiDateYear = 0;
					m_GetItem[ i ].uiDateDay = 0;
					m_GetItem[ i ].uiDateMonth = 0;


					///////////////////////////////////////////////

					pclClient->pclItemManager->GetExplain( &m_GetItem[ i ], ToolTipMsg );

					if ( i >= StartPos && i < StartPos + MAX_ITEMNUM_PER_ITEMMALL )
					{
						stListViewItemData ListViewItemmallData;
						ListViewItemmallData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_GetItem[ i ].siItemNum, ItemName, ToolTipMsg );														
						m_pNItemMallListView1->InsertItem( &ListViewItemmallData );
						
					}
				}
			}
		}
	}
	m_pNItemMallListView1->Refresh();

	return;
}



void CNItemMallManager1::ShowItemList()
{
	

	cltClient *pclclient = (cltClient*)pclClient;

	LoadGetItemData();
	
	m_siViewPage = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetListPage();

	TCHAR buffer[256] = TEXT("") ;
	StringCchPrintf(buffer, 256, TEXT("%d번 페이지"),m_siViewPage);

	SI32 totalnum = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetTotalItemNum() ;
	SI32 totalpage ;

	if ( totalnum % MAX_ITEMNUM_PER_ITEMMALL == 0 && totalnum != 0 )
		totalpage = totalnum / MAX_ITEMNUM_PER_ITEMMALL ;
	else totalpage = totalnum / MAX_ITEMNUM_PER_ITEMMALL + 1 ;

	
	if ( m_siViewPage > 1 )
	{
		m_pNItemMallButton4->Enable(true);		
	}
	else
	{
		m_pNItemMallButton4->Enable(false);		
	}
	

	if ( m_siViewPage < totalpage )
	{
		m_pNItemMallButton5->Enable(true);		
	}
	else 
	{ 
		m_pNItemMallButton5->Enable(false);	
	}
	
	TCHAR * pText = GetTxtFromMgr(2084);

	StringCchPrintf(buffer, 256, pText,m_siViewPage);
	m_pNItemMallStatic9->SetText(buffer, DT_CENTER | DT_VCENTER | DT_SINGLELINE );	

	m_siSelectItem = -1 ;
	SetGetItemInfoTxt(TEXT(""));
}


void CNItemMallManager1::SetGetItemInfoTxt(TCHAR * Txt)
{
	TCHAR buffer[1024] = TEXT("") ;

	if ( Txt != NULL && _tcslen(Txt) > 0 )
	{
		StringCchPrintf(buffer, 1024, Txt);
	//	m_pNItemMallStatic4->SetText(buffer,DT_WORDBREAK);
	}	
}



void CNItemMallManager1::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = ( cltClient * )pclClient;
	SI32* itemid = (SI32*)pData;
	SI32* itemnum = (SI32*)&pData[4];
	SI32* selectitem=(SI32*)&pData[8];
	cltItem item = pclclient->pclCM->CR[1]->pclCI->clItemMall.GetItem(*selectitem);
	cltItem *pclitem = &item;
		
	pclitem->siItemNum =  _tstoi(m_pNItemMallEdit->GetText());


	switch( MsgIndex )
	{
	case 0:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{					

				SI32 siType = pclClient->pclItemManager->GetItemType( pclitem->siUnique );

				if ( siType == ITEMTYPE_DRESS || siType == ITEMTYPE_HAT )
				{
					pclclient->CreateInterface(NITEMMALL_GIVESTATUS_DLG);
					if (pclclient->m_pDialog[NITEMMALL_GIVESTATUS_DLG])
					{
						((CNItemMallGiveStatus*)pclclient->m_pDialog[NITEMMALL_GIVESTATUS_DLG])->SetSelectedItem(*itemid, *itemnum, *selectitem);
					}
				}
				else
				{

					cltGameMsgRequest_ItemMallGetItem clinfo(*itemid,*selectitem,pclitem );
					cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLGETITEM ,sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);	
					m_siSelectItem = -1;
				}

			}
		}
		break;

	case 1:
		{		
		
			cltGameMsgRequest_ItemMallGetItem clinfo(*itemid,*selectitem,pclitem );
			cltMsg clMsg(GAMEMSG_REQUEST_ITEMMALLGETITEM ,sizeof(clinfo), (BYTE*)&clinfo);
			pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
		}
		break;
	}
	return;
}

