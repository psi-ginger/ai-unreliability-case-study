#include "client.h"

#include "NWennyChild1.h"
#include "NWennyOldMan.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Edit.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "Char\CharManager\CharManager.h"

#include "../../../CommonLogic/MsgType-Item.h"
#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/CommonLogic.h"


extern cltCommonLogic* pclClient;

//CNWennyOldManUpgradeItem::CNWennyOldManUpgradeItem()
//{
//	m_pNWennyUpgradeListview = NULL;	
//	m_pNWennyUpgradeButton1 = NULL;
//	m_pNWennyUpgradeButton2 = NULL;
//}
//
//CNWennyOldManUpgradeItem::~CNWennyOldManUpgradeItem()
//{	
//	SAFE_DELETE(m_pNWennyUpgradeListview);
//	SAFE_DELETE(m_pNWennyUpgradeButton1);
//	SAFE_DELETE(m_pNWennyUpgradeButton2);
//
//}
//
//
//void CNWennyOldManUpgradeItem::Create(CControllerMgr *pParent)
//{
//	    CInterfaceFile file;
//		file.LoadFile(TEXT("NInterface/Data/NWennyMan/DLG_NWennyChild1.ddf"));		
//		file.CreateChildDialog( this, NWENNYMANCHILD1_DLG, TEXT("dialog_NWennyChild1"),StaticNWennyOldManUpgradeItemProc, pParent);
//
//		m_pNWennyUpgradeListview = new CListView(this);
//		m_pNWennyUpgradeButton1 = new CButton(this);
//		m_pNWennyUpgradeButton2 = new CButton(this);
//
//
//		file.CreateControl( m_pNWennyUpgradeListview,NWENNYMANCHILD1_LISTVIEW, TEXT("listview_NWennyChild1") );
//		m_pNWennyUpgradeListview->SetBorder(true);
//		file.CreateControl(m_pNWennyUpgradeButton1, NWENNYMANCHILD1_BUTTON1, TEXT("button_NWennyChild1") );
//		file.CreateControl(m_pNWennyUpgradeButton2, NWENNYMANCHILD1_BUTTON2, TEXT("button_NWennyChild2") );
//		LoadUpgradeItemData();
//		Hide();
//	
//}
//void CNWennyOldManUpgradeItem::Show()
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	CDialogController::Show( true );
//	m_siVillage = pclclient->pclCM->CR[1]->GetMapIndex() ;
//	SetTimerOnOff( true );
//	SetActionTimer( 200);
//}
//
//void CNWennyOldManUpgradeItem::Hide()
//{
//	CDialogController::Show( false );
//	SetTimerOnOff( true );
//	SetActionTimer( 200);
//	//SetTimerOnOff( false );
//}
//
//
//void CALLBACK CNWennyOldManUpgradeItem::StaticNWennyOldManUpgradeItemProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
//{
//	
//	CNWennyOldManUpgradeItem *pThis = (CNWennyOldManUpgradeItem*)pControllerMgr;
//	pThis->NWennyOldManUpgradeItemProc( nEvent, nControlID, pControl );
//	//cltClient *pclclient = (cltClient*)pclClient;
//	//return pclclient->m_pNWennyOldMan->NWennyOldManUpgradeItemProc( hDlg, iMsg, wParam, lParam );
//}
//
//void CALLBACK CNWennyOldManUpgradeItem::NWennyOldManUpgradeItemProc(UINT nEvent, int nControlID, CControllerObj* pControl )
//{
//
//	//Action();
//	//SetTimerOnOff( false );
//	cltClient *pclclient = (cltClient*)pclClient;    
//	
//	switch( nControlID )
//	{
//	case NWENNYMANCHILD1_LISTVIEW:
//		{			
//			switch(nEvent)	
//			{
//			case EVENT_LISTVIEW_LBUTTONDOWN:
//				{
//                    m_siSelectIndex = m_pNWennyUpgradeListview->GetSelectedItemIndex();
//				}
//				break;
//			}
//
//		
//
//
//		}
//		break;
//	case NWENNYMANCHILD1_BUTTON1:		
//		{		
//			switch(nEvent)	
//				{			
//				case EVENT_BUTTON_CLICKED:
//					{
//									
//						if ( m_siSelectIndex < 0 )
//						{
//							TCHAR* pTitle = GetTxtFromMgr(1898);
//							TCHAR* pText = GetTxtFromMgr(1899);
//
//							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
//							break;
//						}
//						
//						if ( ! pclClient->pclItemManager->CanTune(&m_clUserItem[ m_siSelectIndex ] ) )
//						{
//							TCHAR* pTitle = GetTxtFromMgr(1900);
//							TCHAR* pText = GetTxtFromMgr(1901);
//
//							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
//							break;
//						}
//						
//						if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(ITEMUNIQUE(24005)) <= 0 )
//						{
//							TCHAR* pTitle = GetTxtFromMgr(1902);
//							TCHAR* pText = GetTxtFromMgr(1903);
//
//							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
//							break;
//						}
//
//						TCHAR* pTitle = GetTxtFromMgr(1904);
//						TCHAR* pText = GetTxtFromMgr(1905);
//						stMsgBox MsgBox;
//
//						CControllerMgr *pParent = NULL;
//						pParent = this->GetParentDialog();
//						
//						if(pParent == NULL ) return;
//						MsgBox.Set( pParent, pTitle, pText, MSGBOX_TYPE_YESNO, 0 );
//						BYTE TempBuffer[ 128 ];
//						SI16 Size = sizeof(  m_siSelectIndex )+ sizeof(m_siVillage);
//						memcpy( TempBuffer, &m_siSelectIndex,sizeof( m_siSelectIndex) );
//						memcpy(&TempBuffer[sizeof( m_siSelectIndex)],&m_siVillage, sizeof(m_siVillage));
//						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
//							
//					
//
//						/*
//						if (  MessageBox(NULL, pText, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES )
//						{
//							cltGameMsgRequest_UpgradeItem clinfo( m_siSelectIndex, m_siVillage );
//							cltMsg clMsg(GAMEMSG_REQUEST_UPGRADEITEM, sizeof(clinfo), (BYTE*)&clinfo);
//							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
//						
//
//						}
//						*/
//
//
//					}break;
//				}
//				//LoadUpgradeItemData();		
//				//Show();
//               			
//		}
//		break;
//
//	case NWENNYMANCHILD1_BUTTON2:
//		{			
//			switch(nEvent)	
//				{			
//				case EVENT_BUTTON_CLICKED:
//					{		
//						(pclclient->m_pDialog[NWENNYMAN_DLG])->DeleteDialog();
//					}
//					break;
//				}
//		}
//		break;
//	}
//
//
//	
//	if(IsChangedInvItem())
//	{
//		LoadUpgradeItemData();
//	}
//
//	
//
//
//	return;
//}
//
//void CNWennyOldManUpgradeItem::Action()
//{
//		
//	if(IsChangedInvItem())
//	{
//		LoadUpgradeItemData();
//	}
//
//
//}
//
//
//void CNWennyOldManUpgradeItem::UpgradeItemSuccess( bool Success )
//{
//	
//	cltClient *pclclient = (cltClient *)pclClient;
//	if ( Success )
//	{
//		TCHAR* pText = GetTxtFromMgr(1919);
//		(((CNWennyOldMan*)pclclient->m_pDialog[ NWENNYMAN_DLG])->m_pNWennyEdit)->SetText(pText);	
//		
//	}
//	else
//	{
//		TCHAR* pText = GetTxtFromMgr(1920);
//		(((CNWennyOldMan*)pclclient->m_pDialog[ NWENNYMAN_DLG])->m_pNWennyEdit)->SetText(pText);	
//		
//	}
//
//
//    
//	if(IsChangedInvItem())
//	{
//		LoadUpgradeItemData();
//	}
//			
//	
//}
//
//
//
//
//void CNWennyOldManUpgradeItem::LoadUpgradeItemData()
//{
//
//    cltClient *pclclient = (cltClient*)pclClient;
//	m_pPersonItemInfo =& pclclient->pclCM->CR[1]->pclCI->clCharItem;
//	SI32 GlobalImageIndex = -1;
//	SI32 GlobalImageFont = -1;
//	m_siSelectIndex =-1;
//	SI32 StartPos=15;   //장착하고 있는 장비 제외하고 말, 사람, 가방에 있는 아이템만 출력 ^^* by tae yang
//	SI32 id = 1;
//	m_pNWennyUpgradeListview->DeleteAllItems();
//	m_pNWennyUpgradeListview->SetStartItemPos( StartPos );	
//	SI16 Counter = 0;
//	TCHAR ItemName[128]=TEXT("");
//	TCHAR buffer[ 256 ] = TEXT("");
//
//      
//
//	//cltItem			*m_pInvChildItem;
//    //SI32			m_siSelectIndex;
//	//SI32			m_siVillage;
//	
//		for ( SI32 i = 0; i < MAX_ITEM_PER_PERSON; ++i )
//		{
//			m_clUserItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );			
//			if ( m_clUserItem[ i ].siUnique > 0 )
//			{
//				if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clUserItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
//				{
//					TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clUserItem[ i ].siUnique );
//
//					if ( ItemName )
//					{
//						TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
//						pclClient->pclItemManager->GetExplain( &m_clUserItem[ i ], ToolTipMsg );
//
//						if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON  && pclClient->pclItemManager->CanTune(&m_clUserItem[ i ] )/*AX_ITEM_PER_INVENTORY*/ )
//						{
//
//							stListViewItemData ListViewItemData;
//							ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_clUserItem[ i ].siItemNum, ItemName, ToolTipMsg );														
//							m_pNWennyUpgradeListview->InsertItem( &ListViewItemData );
//							
//						}
//					}
//				}
//			}
//		}//end for		
//
//	return;
//}
//
//
//
//
//
//BOOL CNWennyOldManUpgradeItem::IsChangedInvItem()
//{
//	for ( int j = PERSONITEM_INV0; j < PERSONITEM_SUMMON1EQUIP0; ++j )
//	{
//		SI32 size = sizeof(cltItem);
//		
//		BYTE* src = (BYTE*)&m_clUserItem[ j ];
//		BYTE* dst = (BYTE*)&pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ j ];
//		for(int i = 0;i < size; i++)
//		{
//			if( src[i] != dst[i] )
//			{
//				return true;
//			}
//		}
//	}
//
//	return false;
//}
//
//void CNWennyOldManUpgradeItem::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
//{
//	cltClient *pclclient = (cltClient*)pclClient;
//	
//	switch( MsgIndex )
//	{
//	case 0:
//		{
//			if ( pData == NULL || DataLen < 1 )
//			{
//				return;
//			}
//			if ( RetValue )
//			{
//				//SI32 *m_siSelectIndex= (SI32 *)pData;
//				//SI32 *m_siVillage = (SI32 *)&pData[4];
//
//				cltGameMsgRequest_UpgradeItem clinfo( m_siSelectIndex, m_siVillage );
//				cltMsg clMsg(GAMEMSG_REQUEST_UPGRADEITEM, sizeof(clinfo), (BYTE*)&clinfo);
//				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
//			}
//		}
//		break;
//	}
//
//}