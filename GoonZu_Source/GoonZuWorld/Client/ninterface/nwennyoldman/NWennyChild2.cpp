#include "client.h"


#include "NWennyOldMan.h"
#include "NWennyChild2.h"

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


//CNWennyOldManPerformanceUpgradeItem::CNWennyOldManPerformanceUpgradeItem()
//{
//	m_pNWennyPerformanceListview = NULL;	
//	m_pNWennyPerformanceButton1 = NULL;
//	m_pNWennyPerformanceButton2 = NULL;
//}
//
//
//
//CNWennyOldManPerformanceUpgradeItem::~CNWennyOldManPerformanceUpgradeItem()
//{	
//	SAFE_DELETE(m_pNWennyPerformanceListview);
//	SAFE_DELETE(m_pNWennyPerformanceButton1);
//	SAFE_DELETE(m_pNWennyPerformanceButton2);	
//}
//
//
//
//
//void CNWennyOldManPerformanceUpgradeItem::Create(CControllerMgr *pParent)
//{
//	SetActionTimer(200);
//	if(!IsCreate())
//	{  	
//		CInterfaceFile file2;
//		file2.LoadFile(TEXT("NInterface/Data/NWennyMan/DLG_NWennyChild2.ddf"));
//		file2.CreateChildDialog( this, NWENNYMANCHILD2_DLG, TEXT("dialog_NWennyChild2"), StaticNWennyOldManPerformanceUpgradeItemProc ,pParent);
//
//		m_pNWennyPerformanceListview = new CListView(this);
//		m_pNWennyPerformanceButton1 = new CButton(this);
//		m_pNWennyPerformanceButton2 = new CButton(this);
//
//		
//		file2.CreateControl(m_pNWennyPerformanceListview, NWENNYMANCHILD2_LISTVIEW, TEXT("listview_NWennyChild2") );
//		m_pNWennyPerformanceListview->SetBorder(true);
//		file2.CreateControl(m_pNWennyPerformanceButton1,NWENNYMANCHILD2_BUTTON1, TEXT("button_NWennyChild11") );
//		file2.CreateControl(m_pNWennyPerformanceButton2, NWENNYMANCHILD2_BUTTON2, TEXT("button_NWennyChild22") );
//		LoadPerformanceUpgradeItemData();
//		Hide();
//	}
//}
//
//
//void CNWennyOldManPerformanceUpgradeItem::Show()
//{
//	cltClient *pclclient = (cltClient *)pclClient;
//	CDialogController::Show( true );
//	m_siVillage = pclclient->pclCM->CR[1]->GetMapIndex() ;
//}
//
//
//
//void CNWennyOldManPerformanceUpgradeItem::Hide()
//{
//	CDialogController::Show( false );
//}
//
//
//
//void CALLBACK CNWennyOldManPerformanceUpgradeItem::StaticNWennyOldManPerformanceUpgradeItemProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
//{
//	CNWennyOldManPerformanceUpgradeItem *pThis = (CNWennyOldManPerformanceUpgradeItem*)pControllerMgr;
//	pThis->NWennyOldManPerformanceUpgradeItemProc( nEvent, nControlID, pControl );
//}
//
//void CALLBACK CNWennyOldManPerformanceUpgradeItem::NWennyOldManPerformanceUpgradeItemProc(UINT nEvent, int nControlID, CControllerObj* pControl)
//{
//
//	cltClient *pclclient = (cltClient *)pclClient;
//	TCHAR Nbuffer[512]=TEXT(" ");
//		
//		
//	switch( nControlID )
//	{
//	case NWENNYMANCHILD2_LISTVIEW:
//		{			
//			switch(nEvent)	
//			{
//			case EVENT_LISTVIEW_LBUTTONDOWN:
//				{
//                    m_siSelectIndex = m_pNWennyPerformanceListview->GetSelectedItemIndex();
//					cltItem clDestItem;
//					clDestItem.Init();
//					
//					SI32 ScrollUnique;
//					SI32 ScrollNum;
//					TCHAR *ScrollName = NULL;
//					
//			
//					// 개조 가능한 아이템 인 경우.
//					SI32 returnval = 0;
//					if ( pclClient->pclItemManager->GetScrollForTune2(
//						m_clUserItem[ m_siSelectIndex ],
//						&ScrollUnique,
//						&ScrollNum,
//						&clDestItem,
//						&returnval) )
//					{								
//						TCHAR Temp[ 512 ] = TEXT("");
//						if ( pclClient->pclItemManager->GetExplain( &clDestItem, Temp ) )
//						{
//							TCHAR Buffer[ 1024 ] = TEXT("");
//							TCHAR* pText = GetTxtFromMgr(1906);
//							StringCchPrintf( Buffer, 1024, pText, Temp );
//							(((CNWennyOldMan*)pclclient->m_pDialog[ NWENNYMAN_DLG])->m_pNWennyEdit)->SetText(Buffer);
//
//						}
//					}
//				}
//				break;
//			}
//
//		}
//		break;
//
//	case NWENNYMANCHILD2_BUTTON1:
//		{				
//			switch(nEvent)	
//				{			
//				case EVENT_BUTTON_CLICKED:
//					{						
//						if ( IsChangedInvItem() )						
//							LoadPerformanceUpgradeItemData();
//
//
//						if ( m_siSelectIndex < 0 )
//						{
//							TCHAR* pTitle = GetTxtFromMgr(1898);
//							TCHAR* pText  = GetTxtFromMgr(1899);
//
//							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
//							break;
//						}
//						
//						if ( m_clUserItem[ m_siSelectIndex ].siUnique < 1 )
//						{
//							break;
//						}
//						
//						if (m_clUserItem[ m_siSelectIndex ].CanPile( pclClient->pclItemManager ) )
//						{
//							TCHAR* pTitle = GetTxtFromMgr(1907);
//							TCHAR* pText  = GetTxtFromMgr(1908);
//
//
//							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
//							break;
//						}
//
//						if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(ITEMUNIQUE(24005)) < 1 )
//						{
//							TCHAR* pTitle = GetTxtFromMgr(1907);
//							TCHAR* pText  = GetTxtFromMgr(1909);
//
//							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
//							break;
//						}
//						
//						cltItem clDestItem;
//						clDestItem.Init();
//						
//						SI32 ScrollUnique;
//						SI32 ScrollNum;
//						TCHAR *ScrollName = NULL;
//
//						SI32 returnval = 0;
//						if ( ! pclClient->pclItemManager->GetScrollForTune2(
//							m_clUserItem[ m_siSelectIndex ],
//							&ScrollUnique,
//							&ScrollNum,
//							&clDestItem,
//							&returnval) )
//						{
//							switch(returnval)
//							{
//							case SCROLLTUNE_ERR_MINDAMAGE:
//								{
//									TCHAR* pTitle = GetTxtFromMgr(1907);
//									TCHAR* pText  = GetTxtFromMgr(1910);
//								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, SCROLL_TUNE_MAX_RATE );
//								}
//								break;
//							case SCROLLTUNE_ERR_MAXDAMAGE:
//								{
//									TCHAR* pTitle = GetTxtFromMgr(1907);
//									TCHAR* pText  = GetTxtFromMgr(1911);
//									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, SCROLL_TUNE_MAX_RATE );
//								}
//								break;
//							case SCROLLTUNE_ERR_AC:
//								{
//									TCHAR* pTitle = GetTxtFromMgr(1907);
//									TCHAR* pText  = GetTxtFromMgr(1912);
//									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, SCROLL_TUNE_MAX_RATE );
//								}
//								break;
//							default:
//								{
//									TCHAR* pTitle = GetTxtFromMgr(1907);
//									TCHAR* pText  = GetTxtFromMgr(1908);
//									pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );
//								}
//							}
//							
//							break;
//						}
//
//						ScrollName = (TCHAR*)pclClient->pclItemManager->GetName( ScrollUnique );
//						if ( ScrollName == NULL || _tcslen ( ScrollName ) < 1 )
//						{
//							break;
//						}
//						
//						SI16 ScrollItemPos = pclClient->pclCM->CR[1]->pclCI->clCharItem.GetItemPos( ScrollUnique );
//						
//						
//						if ( ScrollItemPos < 0 )
//						{
//							ScrollName = (TCHAR*)pclClient->pclItemManager->GetName( ScrollUnique );
//							
//							if ( ScrollName != NULL )
//							{
//								TCHAR* pTitle = GetTxtFromMgr(1907);
//								TCHAR* pText  = GetTxtFromMgr(1913);
//								TCHAR num[128]= TEXT(" ") ;
//								StringCchPrintf(num, 128, TEXT("%d"),ScrollNum);
//								Nsprintf(Nbuffer,pText,TEXT("item"),  ScrollName  ,TEXT("num"), num   ,NULL);
//								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);
//								//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, ScrollName, ScrollNum );
//							}
//							
//							break;
//						}
//						
//						if ( pclClient->pclCM->CR[1]->pclCI->clCharItem.clItem[ ScrollItemPos ].GetItemNum() < ScrollNum )
//						{
//							ScrollName = (TCHAR*)pclClient->pclItemManager->GetName( ScrollUnique );
//							
//							if ( ScrollName != NULL )
//							{
//								TCHAR* pTitle = GetTxtFromMgr(1907);
//								TCHAR* pText  = GetTxtFromMgr(1913);
//								TCHAR num[128]= TEXT(" ") ;
//								StringCchPrintf(num, 128, TEXT("%d"),ScrollNum);
//								Nsprintf(Nbuffer,pText,TEXT("item"),  ScrollName  ,TEXT("num"), num   ,NULL);
//								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle,Nbuffer);
//								//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText, ScrollName, ScrollNum );
//							}
//							
//							break;
//						}
//						
//						TCHAR buffer[256] = TEXT("");
//						TCHAR* pText = GetTxtFromMgr(1914);
//						TCHAR* pTitle = GetTxtFromMgr(1915);
//						StringCchPrintf( buffer, 256, pText, ScrollName, ScrollNum );
//
//						/*
//						if (  MessageBox(NULL, buffer, pTitle, MB_ICONQUESTION | MB_YESNO) == IDYES )
//						{
//							cltGameMsgRequest_PerformanceUpgradeItem clinfo( m_siVillage, m_siSelectIndex, &m_clUserItem[ m_siSelectIndex ] );
//							cltMsg clMsg(GAMEMSG_REQUEST_PERFORMANCEUPGRADEITEM, sizeof(clinfo), (BYTE*)&clinfo);
//							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
//						}
//						*/
//
//						stMsgBox MsgBox;
//
//						CControllerMgr *pParent = NULL;
//						pParent = this->GetParentDialog();
//						
//						if(pParent == NULL ) return;
//						MsgBox.Set( pParent, pTitle, buffer, MSGBOX_TYPE_YESNO, 1 );
//						BYTE TempBuffer[ 128 ];
//						SI16 Size = sizeof(  m_siSelectIndex )+ sizeof(m_siVillage);
//						memcpy( TempBuffer, &m_siSelectIndex,sizeof( m_siSelectIndex) );
//						memcpy(&TempBuffer[sizeof( m_siSelectIndex)],&m_siVillage, sizeof(m_siVillage));
//						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
//						
//
//
//					}break;
//				}
//				//LoadPerformanceUpgradeItemData();
//		}
//		break;
//
//	case NWENNYMANCHILD2_BUTTON2:
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
//
//	return;
//}
//
//void CNWennyOldManPerformanceUpgradeItem::Action()
//{
//			
//	if(IsChangedInvItem())
//	{
//		LoadPerformanceUpgradeItemData();
//	}
//
//}
//
//
//void CNWennyOldManPerformanceUpgradeItem::LoadPerformanceUpgradeItemData()
//{
//	   
//	cltClient *pclclient = (cltClient*)pclClient;
//	m_pPersonItemInfo =& pclclient->pclCM->CR[1]->pclCI->clCharItem;
//	SI32 GlobalImageIndex = -1;
//	SI32 GlobalImageFont = -1;
//	SI32 StartPos=15;  
//	SI32 id = 1;
//	m_siSelectIndex =-1;
//	m_pNWennyPerformanceListview->DeleteAllItems();
//	m_pNWennyPerformanceListview->SetStartItemPos( StartPos );
//	
//	SI16 Counter = 0;
//	TCHAR ItemName[128]=TEXT("");
//	TCHAR buffer[ 256 ] = TEXT("");
//	cltItem clDestItem;
//	clDestItem.Init();
//	
//	SI32 ScrollUnique;
//	SI32 ScrollNum;
//
//		
//		
//	for ( SI32 i = 0; i < MAX_ITEM_PER_PERSON; ++i )
//	{
//		m_clUserItem[ i ].Set( &pclClient->pclCM->CR[id]->pclCI->clCharItem.clItem[ i ] );			
//        clDestItem.Init();
//		if ( m_clUserItem[ i ].siUnique > 0 )
//		{
//			if ( pclClient->pclItemManager->GetItemGImageFileIndexAndFont( m_clUserItem[ i ].siUnique, &GlobalImageIndex, &GlobalImageFont ) )
//			{
//				TCHAR *ItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clUserItem[ i ].siUnique );
//
//				if ( ItemName )
//				{
//					TCHAR ToolTipMsg[ MAX_TOOLTIP_MSG_SIZE ] = TEXT("");
//					pclClient->pclItemManager->GetExplain( &m_clUserItem[ i ], ToolTipMsg );
//
//                    SI32 returnval = 0;
//					if ( i >= StartPos && i < StartPos + MAX_ITEM_PER_PERSON  && pclClient->pclItemManager->GetScrollForTune2( m_clUserItem[i], &ScrollUnique, &ScrollNum, &clDestItem, &returnval ) )
//					{
//
//						stListViewItemData ListViewItemData;
//						ListViewItemData.Set( GlobalImageIndex, GlobalImageFont, i - StartPos, m_clUserItem[ i ].siItemNum, ItemName, ToolTipMsg );														
//						m_pNWennyPerformanceListview->InsertItem( &ListViewItemData );
//						
//					}
//				}
//			}
//		}
//	}//end for	
//}
//
//
//
//void CNWennyOldManPerformanceUpgradeItem::PerformanceUpgradeItemSuccess()
//{
//    cltClient *pclclient = (cltClient *)pclClient;
//	TCHAR* pText = GetTxtFromMgr(1921);
//	(((CNWennyOldMan*)pclclient->m_pDialog[ NWENNYMAN_DLG])->m_pNWennyEdit)->SetText(pText);	
//
//		
//	if(IsChangedInvItem())
//	{
//		LoadPerformanceUpgradeItemData();
//	}
//	//LoadPerformanceUpgradeItemData();
//}
//
//
//
//
//
//BOOL CNWennyOldManPerformanceUpgradeItem::IsChangedInvItem()
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
//		}//end for
//	}//large endfor
//
//	return false;
//}
//
//void  CNWennyOldManPerformanceUpgradeItem::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
//{
//
//	cltClient *pclclient = (cltClient*)pclClient;
//	
//	switch( MsgIndex )
//	{
//	case 1:
//		{
//			if ( pData == NULL || DataLen < 1 )
//			{
//				return;
//			}
//			if ( RetValue )
//			{
//				//SI32 *m_siSelectIndex= (SI32 *)pData;
//				//SI32 *m_siVillage = (SI32 *)&pData[4];
//                //cltItem					m_clUserItem[ MAX_ITEM_PER_PERSON ];
//				
//					
//
//				cltGameMsgRequest_PerformanceUpgradeItem clinfo( m_siVillage, m_siSelectIndex, &m_clUserItem[ m_siSelectIndex ] );
//				cltMsg clMsg(GAMEMSG_REQUEST_PERFORMANCEUPGRADEITEM, sizeof(clinfo), (BYTE*)&clinfo);
//				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
//			}
//		}
//		break;
//	}
//}