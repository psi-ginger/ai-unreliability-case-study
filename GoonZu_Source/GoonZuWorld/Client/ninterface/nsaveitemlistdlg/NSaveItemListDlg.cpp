#include "../Client/NInterface/NSaveItemListDlg/NSaveItemListDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"
#include "Msg/MsgType-Postoffice.h"

#include "Char\CharManager\CharManager.h"
#include "../common/Item/ItemCommon/cltItem.h"
#include "../common/horse/horse.h"


extern cltCommonLogic* pclClient;

CNSaveItemListDlg::CNSaveItemListDlg() : CDialogController()
{
	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;

	m_pList = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;

	m_siSelectIndex = -1;

	//KHY - 1127 - 클라라 물품 보관 개선
	m_selectWhatShowList = 1; // 1 = 아이템, 2 = 기승동물

	memset( m_clSaveUsersItemInfo, 0, sizeof(cltSaveUersItemInfo) * MAX_SAVEUSERITEM_LIST );
	memset( m_clSaveUsersHorseInfo, 0, sizeof(cltSaveUersHorseInfo) * MAX_SAVEUSERHORSE_LIST );
}

CNSaveItemListDlg::~CNSaveItemListDlg()
{
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);

	SAFE_DELETE(m_pList);

	SAFE_DELETE(m_pStatic[1]);
	SAFE_DELETE(m_pStatic[0]);
}

void CNSaveItemListDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NSaveItemListDlg/DLG_SaveItemList.ddf"));
		file.CreatePopupDialog( this, NSAVEITEMLIST_DLG, TEXT("dialog_SaveItemList"), StaticCallBackDialogNSaveItemListDlg );

		NEWCSTATIC(m_pStatic[0]);
		NEWCSTATIC(m_pStatic[1]);

		NEWCLIST(m_pList);

		NEWCBUTTON(m_pButton[0]);
		NEWCBUTTON(m_pButton[1]);
		//KHY - 1127 - 클라라 물품 보관 개선
		NEWCBUTTON(m_pButton[2]); //보관중인 물품.
		NEWCBUTTON(m_pButton[3]); // 보관중인 탈것.

		
		file.CreateControl( m_pStatic[0], NSAVEITEMLIST_DIALOG_STATIC1, TEXT("statictext_NONAME1")); 
		file.CreateControl( m_pStatic[1], NSAVEITEMLIST_DIALOG_STATIC2, TEXT("statictext_NONAME2"));

		file.CreateControl( m_pList, NSAVEITEMLIST_DIALOG_LIST, TEXT("listbox_NONAME1")); 

		file.CreateControl( m_pButton[0], NSAVEITEMLIST_DIALOG_BUTTON1, TEXT("button_NONAME1")); 
		file.CreateControl( m_pButton[1], NSAVEITEMLIST_DIALOG_BUTTON2, TEXT("button_NONAME2")); 
		file.CreateControl( m_pButton[2], NSAVEITEMLIST_DIALOG_BUTTON3, TEXT("button_NONAME3")); 
		file.CreateControl( m_pButton[3], NSAVEITEMLIST_DIALOG_BUTTON4, TEXT("button_NONAME4")); 

		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")), szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pStatic[0]->SetFileName(szPortrait);
		m_pStatic[0]->SetFontIndex(siPortraitFont);

		TCHAR buffer[1024] =  TEXT("");

		TCHAR* pText = GetTxtFromMgr(5564);
		StringCchCopy(buffer, 1024, pText);
		m_pStatic[1]->SetBorder(true);
		m_pStatic[1]->SetBKColorUse(true);
		m_pStatic[1]->SetText(buffer,DT_WORDBREAK);

		//KHY - 1127 - 클라라 물품 보관 개선
		if(!(pclClient->IsCountrySwitch( Switch_SaveHorseToHouse )))
		{
			m_pButton[2]->Show(false);
			m_pButton[3]->Show(false);
		}
		m_selectWhatShowList = 1; // 1 = 아이템, 2 = 기승동물

		pText = GetTxtFromMgr(5565);
		m_pList->SetColumn(0,100, pText);
		pText = GetTxtFromMgr(4510);
		m_pList->SetColumn(1,100, pText);
		pText = GetTxtFromMgr(4507);
		m_pList->SetColumn(2, 60, pText);
		m_pList->SetBKColorUse( true );
		m_pList->SetBorder( true );
		m_pList->Refresh();
	}
	else
	{
		DeleteDialog();
	}
}


void CALLBACK CNSaveItemListDlg::StaticCallBackDialogNSaveItemListDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNSaveItemListDlg *pThis = (CNSaveItemListDlg *)pControllerMgr;
	pThis->CallBackDialogNSaveItemListDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNSaveItemListDlg::CallBackDialogNSaveItemListDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NSAVEITEMLIST_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 tempSelectIndex = m_pList->GetSelectedIndex();

					if( tempSelectIndex < 0)
						return;

					m_siSelectIndex = tempSelectIndex;
				}
				break;
			}
		}
		break;
	//KHY - 1127 - 클라라 물품 보관 개선	
	case NSAVEITEMLIST_DIALOG_BUTTON3:	// 보관중인 물품.
		{
			TCHAR buffer[1024] =  TEXT("");

			TCHAR* pText = GetTxtFromMgr(5564);
			StringCchCopy(buffer, 1024, pText);
			m_pStatic[1]->SetBorder(true);
			m_pStatic[1]->SetBKColorUse(true);
			m_pStatic[1]->SetText(buffer,DT_WORDBREAK);

			pText = GetTxtFromMgr(5565);
			m_pList->SetColumn(0,100, pText);
			pText = GetTxtFromMgr(4510);
			m_pList->SetColumn(1,100, pText);
			pText = GetTxtFromMgr(4507);
			m_pList->SetColumn(2, 60, pText);
			m_pList->SetBKColorUse( true );
			m_pList->SetBorder( true );

			m_selectWhatShowList = 1; // 1 = 아이템, 2 = 기승동물

			InDlgShowList(m_selectWhatShowList);
			
			//m_pList->Refresh();			
		}
		break;
	case NSAVEITEMLIST_DIALOG_BUTTON4:	// 보관 중인 탈것.
		{
			TCHAR buffer[1024] =  TEXT("");

			TCHAR* pText = GetTxtFromMgr(7222);
			StringCchCopy(buffer, 1024, pText);
			m_pStatic[1]->SetBorder(true);
			m_pStatic[1]->SetBKColorUse(true);
			m_pStatic[1]->SetText(buffer,DT_WORDBREAK);
			
			pText = GetTxtFromMgr(5565);
			m_pList->SetColumn(0,100, pText);
			pText = GetTxtFromMgr(7223);
			m_pList->SetColumn(1,100, pText);
			pText = GetTxtFromMgr(7224);
			m_pList->SetColumn(2, 60, pText);
			m_pList->SetBKColorUse( true );
			m_pList->SetBorder( true );

			m_selectWhatShowList = 2; // 1 = 아이템, 2 = 기승동물

			InDlgShowList(m_selectWhatShowList);

			//m_pList->Refresh();						
		}
		break;
	case NSAVEITEMLIST_DIALOG_BUTTON1:	// 물건 찾기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if( m_siSelectIndex < 0 )
						return;

					//KHY - 1127 - 클라라 물품 보관 개선	
					if(m_selectWhatShowList == 1)
					{
						cltItem *pItem = GetItemFromList( m_siSelectIndex );
						if ( pItem )
						{
							SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pItem->siUnique);
							if(ref > 0)
							{
								TCHAR* pTitle = GetTxtFromMgr(5566);
								TCHAR* pText = GetTxtFromMgr(5567);
								TCHAR tempBuffer[256] = TEXT("");

								StringCchPrintf(tempBuffer, 256, pText, pclClient->pclItemManager->pclItemInfo[ref]->GetName(), pItem->siItemNum, m_clSaveUsersItemInfo[m_siSelectIndex].siServiceCharge);

								if( pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < m_clSaveUsersItemInfo[m_siSelectIndex].siServiceCharge)
									return;

								stMsgBox MsgBox;
								MsgBox.Set( this, pTitle, tempBuffer, MSGBOX_TYPE_YESNO, 0 );
								pclclient->SetMsgBox( &MsgBox, NULL, 0 );
							}
						}
					}
					else if(m_selectWhatShowList == 2)
					{
						cltHorse *pHorse = GetHorseFromList( m_siSelectIndex );

						SI32 horseUnique = pHorse->siHorseUnique;
						if(horseUnique > 0)
						{
							TCHAR* pTitle = GetTxtFromMgr(5566);
							TCHAR* pText = GetTxtFromMgr(7225);
							TCHAR tempBuffer[256] = TEXT("");

							cltClient* pclclient = (cltClient*)pclClient;

							REAL32	npcrate = (REAL32)pclclient->pclItemManager->clItemStatistic.siNPCPriceRate;
							npcrate = npcrate / (REAL32)100;

							//SI64 serviceCharge = (SI64)((REAL32)BASIC_SERVICECHARGE * npcrate);

							SI64 serviceCharge = m_clSaveUsersHorseInfo->siServiceCharge;	
							StringCchPrintf(tempBuffer, 256, pText,  serviceCharge);

							if( pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < serviceCharge)
								return;
													
							stMsgBox MsgBox;
							MsgBox.Set( this, pTitle, tempBuffer, MSGBOX_TYPE_YESNO, 1 );
							pclclient->SetMsgBox( &MsgBox, NULL, 0 );
						}
						
						
					}
				}
				break;
			}
		}
		break;

	case NSAVEITEMLIST_DIALOG_BUTTON2:	// 닫기
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					DeleteDialog();
				}
				break;
			}
		}
		break;
	}
}

void CNSaveItemListDlg::ShowList( cltSaveUersItemInfo* pSaveUersItemInfo ,cltSaveUersHorseInfo* pSaveUersHorseInfo)
{
	SI32 CountItem = 0;
	SI32 CountHorse = 0;

	m_pList->Clear();
	memset( m_clSaveUsersItemInfo, 0, sizeof(cltSaveUersItemInfo) * MAX_SAVEUSERITEM_LIST );
	memset( m_clSaveUsersHorseInfo, 0, sizeof(cltSaveUersHorseInfo) * MAX_SAVEUSERHORSE_LIST );

	//KHY - 1127 - 클라라 물품 보관 개선
	for( SI32 i=0; i<MAX_SAVEUSERITEM_LIST; ++i )
	{
		stListBoxItemData Data;
		
		SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(pSaveUersItemInfo[i].clItem.siUnique);

		if(ref > 0) 
		{
			m_clSaveUsersItemInfo[CountItem] = pSaveUersItemInfo[i];
			CountItem++;
		}
		
		if(ref > 0) // 아이템 리스트가 디폴트다 
		{
			TCHAR szItemDate[256] = TEXT("");
			TCHAR szItemName[256] = TEXT("");
			TCHAR szItemNum[50] = TEXT("");
			
			TCHAR* pText = GetTxtFromMgr(1644);
			StringCchPrintf(szItemDate, 256, pText, pSaveUersItemInfo[i].clDate.uiYear, pSaveUersItemInfo[i].clDate.uiMonth, pSaveUersItemInfo[i].clDate.uiDay);
			Data.Set(0, szItemDate);

			StringCchCopy(szItemName, 256, pclClient->pclItemManager->pclItemInfo[ref]->GetName());
			Data.Set(1, szItemName);

			StringCchPrintf(szItemNum, 50, TEXT("%d"), pSaveUersItemInfo[i].clItem.GetItemNum());
			Data.Set(2, szItemNum);

			m_pList->AddItem(&Data);
		}
	}

	for( SI32 i=0; i<MAX_SAVEUSERHORSE_LIST; ++i )
	{

		SI32 horseUnique = pSaveUersHorseInfo[i].clHorse.siHorseUnique;

		if(horseUnique >0)
		{
			m_clSaveUsersHorseInfo[CountHorse] = pSaveUersHorseInfo[i];
			CountHorse++;
		}
	}

	m_pList->Refresh();
}

//KHY - 1127 - 클라라 물품 보관 개선
void CNSaveItemListDlg::InDlgShowList(SI32 selectListType ) // 다이알 로그 안에서의 리스트 갱신을 위해서.
{
	m_pList->Clear();

	if(selectListType ==1 ) // 물건.
	{
		for( SI32 i=0; i<MAX_SAVEUSERITEM_LIST; ++i )
		{
			stListBoxItemData Data;
			
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(m_clSaveUsersItemInfo[i].clItem.siUnique);

			if(ref > 0) // 아이템 리스트가 디폴트다 
			{
				TCHAR szItemDate[256] = TEXT("");
				TCHAR szItemName[256] = TEXT("");
				TCHAR szItemNum[50] = TEXT("");
				
				TCHAR* pText = GetTxtFromMgr(1644);
				StringCchPrintf(szItemDate, 256, pText, m_clSaveUsersItemInfo[i].clDate.uiYear, m_clSaveUsersItemInfo[i].clDate.uiMonth, m_clSaveUsersItemInfo[i].clDate.uiDay);
				Data.Set(0, szItemDate);

				StringCchCopy(szItemName, 256, pclClient->pclItemManager->pclItemInfo[ref]->GetName());
				Data.Set(1, szItemName);

				StringCchPrintf(szItemNum, 50, TEXT("%d"), m_clSaveUsersItemInfo[i].clItem.GetItemNum());
				Data.Set(2, szItemNum);

				m_pList->AddItem(&Data);
			}
		}
	}
	else if(selectListType ==2)
	{
		for( SI32 i=0; i<MAX_SAVEUSERHORSE_LIST; ++i )
		{
			stListBoxItemData Data;
			
			SI32 horseUnique = m_clSaveUsersHorseInfo[i].clHorse.siHorseUnique;

			if(horseUnique > 0) // 아이템 리스트가 디폴트다 
			{
				TCHAR szItemDate[256] = TEXT("");
				TCHAR szHorseName[256] = TEXT("");
				TCHAR szHorseType[128] = TEXT("");
				
				TCHAR* pText = GetTxtFromMgr(1644);
				StringCchPrintf(szItemDate, 256, pText, m_clSaveUsersHorseInfo[i].clDate.uiYear, m_clSaveUsersHorseInfo[i].clDate.uiMonth, m_clSaveUsersHorseInfo[i].clDate.uiDay);
				Data.Set(0, szItemDate);

				StringCchCopy(szHorseName, 256,m_clSaveUsersHorseInfo[i].clHorse.szName);
				Data.Set(1, szHorseName);

				switch(horseUnique) //말 유니크에 맞추어 텍스트 선택.
				{
					case HORSEUNIQUE_BROWN:	
						pText= GetTxtFromMgr(2609);
						break;
					case HORSEUNIQUE_BLACK:
						pText= GetTxtFromMgr(3366);
						break;
					case HORSEUNIQUE_WHITE:
						pText= GetTxtFromMgr(3367);
						break;
					case HORSEUNIQUE_RED:	
						pText= GetTxtFromMgr(2753);
						break;
					case HORSEUNIQUE_RUDOLF: 
						pText= GetTxtFromMgr(3370);
						break;
					//KHY - 0911 - 호랑이 추가.
					case HORSEUNIQUE_TIGER:
						pText= GetTxtFromMgr(5394);
						break;
					case HORSEUNIQUE_THOUSAND: 
						pText= GetTxtFromMgr(3371);
						break;
					case HORSEUNIQUE_STEEL:
						pText= GetTxtFromMgr(5174);
						break;
					case HORSEUNIQUE_WOLF:
						pText= GetTxtFromMgr(5622);
						break;
					case HORSEUNIQUE_ZEBRA:
						pText = GetTxtFromMgr(3369);
						break;
					case HORSEUNIQUE_BEAST:
						pText = GetTxtFromMgr(6418);
						break;
					//KHY - 0622 - 군주S 초보자용  망아지 추가.
					case HORSEUNIQUE_FOAL:
						pText = GetTxtFromMgr(7505);
						break;
					default:
						pText= GetTxtFromMgr(3372);
						break;
				}

				StringCchCopy(szHorseType, 128, pText);
				Data.Set(2, szHorseType);

				m_pList->AddItem(&Data);
			}
		}					
	}
	
	m_pList->Refresh();

}

cltItem* CNSaveItemListDlg::GetItemFromList( SI32 index )
{
	if( index < 0 || index > MAX_SAVEUSERITEM_LIST)
		return NULL;

//	SI32 tempIndex = m_clSaveUsersItemInfo[ index ].siIndex;

	if( m_clSaveUsersItemInfo[index].clItem.siUnique != 0 ) 
		return &m_clSaveUsersItemInfo[index].clItem;
	
	return NULL;
}

cltHorse* CNSaveItemListDlg::GetHorseFromList( SI32 index )
{
	if( index < 0 || index > MAX_SAVEUSERHORSE_LIST)
		return NULL;

	if( m_clSaveUsersHorseInfo[index].clHorse.siHorseUnique > 0 ) 
		return &m_clSaveUsersHorseInfo[index].clHorse;
	
	return NULL;
}
void CNSaveItemListDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				cltItem *pItem = GetItemFromList( m_siSelectIndex );
				if( pItem )
				{
					cltGameMsgRequest_Get_SaveUserItem sendMsg( m_clSaveUsersItemInfo[m_siSelectIndex].siIndex, pItem );
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SAVEUSERITEM, sizeof( cltGameMsgRequest_Get_SaveUserItem ), (BYTE*)&sendMsg );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}
			}
			else
				return;
		}
		break;
	//KHY - 1127 - 클라라 물품 보관 개선
	case 1:
		{
			if ( RetValue )
			{
				cltHorse *pHorse = GetHorseFromList( m_siSelectIndex );
				if( pHorse )
				{
					cltGameMsgRequest_Get_SaveUserHorse sendMsg( m_clSaveUsersHorseInfo[m_siSelectIndex].siIndex, pHorse );
					cltMsg clMsg( GAMEMSG_REQUEST_GET_SAVEUSERHORSE, sizeof( cltGameMsgRequest_Get_SaveUserHorse ), (BYTE*)&sendMsg );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}
			}
			else
				return;
		}
		break;

	}
}
