#include "../Client/NInterface/nmarketconditionsdlg/NMarketConditionsDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"


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
#include "../Client/NInterface/NItemInfoDlg/ItemInfoDlg.h"
#include "../Client/ninterface/ninfodlg/NInfoDlg.h"

extern cltCommonLogic* pclClient;

CNMarketConditionsDlg::CNMarketConditionsDlg() : CDialogController()
{
	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;

	m_pList = NULL;

	m_pButton[0] = NULL;
	m_pButton[1] = NULL;
	m_pButton[2] = NULL;
	m_pButton[3] = NULL;
	m_pButton[4] = NULL;

	m_pComboBox[0]	= NULL;
	m_pComboBox[1]	= NULL;
	

	m_selectWhatShowList 	= 0; // 현재 선택된 메뉴.

	m_siItemTypeIndex 	= 0;	// 품종 인덱스
	m_siItemIndex 		= 0;	// 품목 인덱스
	
	for(SI16 i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		m_arrTypeIndex[i] = 0;
	}
	for(SI16 j = 0;i < 100;i++)
	{	
		m_arrItemIndex[i] = 0;
	}
	
	//memset( m_clSaveUsersItemInfo, 0, sizeof(cltSaveUersItemInfo) * MAX_SAVEUSERITEM_LIST );
	//memset( m_clSaveUsersHorseInfo, 0, sizeof(cltSaveUersHorseInfo) * MAX_SAVEUSERHORSE_LIST );
}

CNMarketConditionsDlg::~CNMarketConditionsDlg()
{
	SAFE_DELETE(m_pStatic[0]);
	SAFE_DELETE(m_pStatic[1]);

	SAFE_DELETE(m_pList);
	
	SAFE_DELETE(m_pButton[0]);
	SAFE_DELETE(m_pButton[1]);
	SAFE_DELETE(m_pButton[2]);
	SAFE_DELETE(m_pButton[3]);
	SAFE_DELETE(m_pButton[4]);

	SAFE_DELETE(m_pComboBox[0]);
	SAFE_DELETE(m_pComboBox[1]);
}

void CNMarketConditionsDlg::Create()
{
	if ( ! IsCreate() ) 
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NMarketConditionsDlg/DLG_MarketConditions.ddf"));
		file.CreatePopupDialog( this, NMARKETCONDITIONS_DLG, TEXT("dialog_MarketConditions"), StaticCallBackDialogNMarketConditionsDlg );

		NEWCSTATIC(m_pStatic[0]);
		NEWCSTATIC(m_pStatic[1]);

		NEWCLIST(m_pList);

		NEWCBUTTON(m_pButton[0]); // 시세 확인.
		NEWCBUTTON(m_pButton[1]); // 닫기
		NEWCBUTTON(m_pButton[2]); // 일반 검색.
		NEWCBUTTON(m_pButton[3]); // 전날 대비 시세. 
		NEWCBUTTON(m_pButton[4]); // 일주일 시세 동향.

		NEWCCOMBOBOX(m_pComboBox[0]); // 품종.
		NEWCCOMBOBOX(m_pComboBox[1]); // 품목.


		file.CreateControl( m_pStatic[0], NMARKETCONDITIONS_DLG_STATIC1, TEXT("statictext_NONAME1")); 
		file.CreateControl( m_pStatic[1], NMARKETCONDITIONS_DLG_STATIC2, TEXT("statictext_NONAME2"));

		file.CreateControl( m_pList, NMARKETCONDITIONS_DLG_LIST, TEXT("listbox_NONAME1")); 

		file.CreateControl( m_pButton[0], NMARKETCONDITIONS_DLG_BUTTON1, TEXT("button_NONAME1")); 
		file.CreateControl( m_pButton[1], NMARKETCONDITIONS_DLG_BUTTON2, TEXT("button_NONAME2")); 
		file.CreateControl( m_pButton[2], NMARKETCONDITIONS_DLG_BUTTON3, TEXT("button_NONAME3")); 
		file.CreateControl( m_pButton[3], NMARKETCONDITIONS_DLG_BUTTON4, TEXT("button_NONAME4")); 
		file.CreateControl( m_pButton[4], NMARKETCONDITIONS_DLG_BUTTON5, TEXT("button_NONAME5")); 

		file.CreateControl( m_pComboBox[0], NMARKETCONDITIONS_DLG_COMBOBOX_ITEMTYPE, TEXT("combobox_NONAME1"));  
		file.CreateControl( m_pComboBox[1], NMARKETCONDITIONS_DLG_COMBOBOX_ITEM, TEXT("combobox_NONAME2"));  

		m_selectWhatShowList = 1; // 1 = 일반검색,  2 = 전날 대비, 3 = 일주일 시세.

		SetItemTypeInComboBox(true);

		m_pComboBox[0]->SetCursel(0);
		m_pComboBox[1]->SetCursel(0);

		m_pComboBox[1]->Show(false); // 품종은  '3 = 일주일 시세.' 때만 보여준다.

		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_INVESTMENT")), szPortrait, sizeof(szPortrait), &siPortraitFont);
		m_pStatic[0]->SetFileName(szPortrait);
		m_pStatic[0]->SetFontIndex(siPortraitFont);

		TCHAR buffer[1024] =  TEXT("");
		TCHAR* pText = GetTxtFromMgr(8621);

		StringCchCopy(buffer, 1024, pText);

		m_pStatic[1]->SetBorder(true);
		m_pStatic[1]->SetBKColorUse(true);
		m_pStatic[1]->SetText(buffer,DT_WORDBREAK);

		// 리스트 설정. - 기본 일반검색으로.
		pText = GetTxtFromMgr(1641); // 번호.
		m_pList->SetColumn(0,40, pText);

		pText = GetTxtFromMgr(1928); // 아이템
		m_pList->SetColumn(1,150, pText);

		pText = GetTxtFromMgr(2191); // 시세
		m_pList->SetColumn(2, 160, pText);

		m_pList->SetBKColorUse( true );
		m_pList->SetBorder( true );
		m_pList->Refresh();
	}
	else
	{
		DeleteDialog();
	}
}

void CNMarketConditionsDlg::Set(SI32 npcunique)
{
	if ( !IsCreate() )
	{
		Create();
	}			

	m_siNPCCharUnique = npcunique ;		
}



void CALLBACK CNMarketConditionsDlg::StaticCallBackDialogNMarketConditionsDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMarketConditionsDlg *pThis = (CNMarketConditionsDlg *)pControllerMgr;
	pThis->CallBackDialogNMarketConditionsDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNMarketConditionsDlg::CallBackDialogNMarketConditionsDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 siWidth = 	m_pList->GetWidth();

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	//아이템 품종
	case NMARKETCONDITIONS_DLG_COMBOBOX_ITEMTYPE:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siItemTypeIndex = m_arrTypeIndex[m_pComboBox[0]->GetSelectedIndex()];

					m_pComboBox[1]->Clear(); //품목 콤보박스 설정.

					if ( m_selectWhatShowList == 3 )
					{
						ShowItemListInComboBox(m_pComboBox[1], m_siItemTypeIndex, false);
					}
				}
				break;
			}
		}
		break;
	//이이템 종류
	case NMARKETCONDITIONS_DLG_COMBOBOX_ITEM:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siItemIndex = m_arrItemIndex[m_pComboBox[1]->GetSelectedIndex()];

					if( m_siItemIndex == -1)
						break;
				}
				break;
			}
		}
		break;

	case NMARKETCONDITIONS_DLG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_ITEM_DBLCLK: // 더블 클릭시 '아이템 정보'창 오픈.
				{
					if(m_selectWhatShowList == 0)
						return;

					SI32 tempSelectIndex = m_pList->GetSelectedIndex();

					if( tempSelectIndex < 0)
						return;

					// 아이템정보창 오픈.					
				/*	if( GetParentDialog()->GetDialogControllerID() == NINFO_SUB_DLG )
					{
						break;
					}
				*/	
					TCHAR buffer[256]; 
					SI16 tmpRow = 0, tmpColumn = 0;

					m_pList->GetSelectedIndex( &tmpRow, &tmpColumn );

					if( tmpColumn == 0 || tmpColumn == 1 || tmpColumn == 2 )
					{
						switch(m_selectWhatShowList)
						{
						case 1:
							StringCchCopy( buffer, 256, m_pList->GetText( tmpRow, 1 ));
							break;
						case 2:
							StringCchCopy( buffer, 256, m_pList->GetText( tmpRow, 0 ));
							break;
						case 3:
							{
								SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(m_siItemIndex);
								StringCchCopy( buffer, 256,  pclClient->pclItemManager->pclItemInfo[ref]->GetName());

							}
							break;
						}

						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->SetItemInfo(buffer);
						//((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->m_pComboBox[1]->Clear();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->ShowItemInfo(buffer);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_ITEM);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NMARKETCONDITIONS_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NMARKETCONDITIONS_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NMARKETCONDITIONS_DLG])->GetY() );
					}					
				}
				break;
			}
		}
		break;
	case NMARKETCONDITIONS_DLG_BUTTON3: // 일반 검색.
		{
			TCHAR buffer[1024] =  TEXT("");
			//---------------------------------------------------
			m_pComboBox[1]->Show(false); // 품종은  '3 = 일주일 시세.' 때만 보여준다.
			//---------------------------------------------------
			TCHAR* pText = GetTxtFromMgr(8707 ); // 일반 검색 설명 문구.
			StringCchCopy(buffer, 1024, pText);
			m_pStatic[1]->SetBorder(true);
			m_pStatic[1]->SetBKColorUse(true);
			m_pStatic[1]->SetText(buffer,DT_WORDBREAK);
			//---------------------------------------------------
			m_pList->Clear();

			pText = GetTxtFromMgr(1641); // 번호.
			m_pList->SetColumn(0,40, pText);

			pText = GetTxtFromMgr(1928); // 아이템.
			m_pList->SetColumn(1,150, pText);

			pText = GetTxtFromMgr(2191); // 시세
			m_pList->SetColumn(2, 160, pText);

			m_pList->SetBKColorUse( true );
			m_pList->SetBorder( true );

			m_pList->Refresh();
			//---------------------------------------------------

			m_selectWhatShowList = 1; // 일반검색.
		}
		break;
	case NMARKETCONDITIONS_DLG_BUTTON4:	// 전날 대비 시세.
		{
			TCHAR buffer[1024] =  TEXT("");
			TCHAR binkan[1024] =  TEXT("");
			//---------------------------------------------------
			m_pComboBox[1]->Show(false); // 품종은  '3 = 일주일 시세.' 때만 보여준다.
			//---------------------------------------------------
			TCHAR* pText = GetTxtFromMgr(8708 ); // 전날대비 시세 설명 문구.
			StringCchCopy(buffer, 1024, pText);
			m_pStatic[1]->SetBorder(true);
			m_pStatic[1]->SetBKColorUse(true);
			m_pStatic[1]->SetText(buffer,DT_WORDBREAK);
			//---------------------------------------------------
			m_pList->Clear();

			pText = GetTxtFromMgr(8120); // 아이템 이름.
			m_pList->SetColumn(0,130, pText);

			pText = GetTxtFromMgr(2191); // 시세
			m_pList->SetColumn(1,110, pText);

			m_pList->SetColumn(2, 20, binkan); // 아이콘 용이다.

			pText = GetTxtFromMgr(8657); // 전날 대비.
			m_pList->SetColumn(3, 90, pText);

			m_pList->SetBKColorUse( true );
			m_pList->SetBorder( true );

			m_pList->Refresh();						
			//---------------------------------------------------

			m_selectWhatShowList = 2; // 전날 대비.
		}
		break;
	case NMARKETCONDITIONS_DLG_BUTTON5:	// 일주일 시세 동향.
		{
			TCHAR buffer[1024] =  TEXT("");
			TCHAR binkan[1024] =  TEXT("");
			//---------------------------------------------------
			m_pComboBox[1]->Show(true); // 품종은  '3 = 일주일 시세.' 때만 보여준다.
			ShowItemListInComboBox(m_pComboBox[1], m_siItemTypeIndex, false);
			//---------------------------------------------------
			TCHAR* pText = GetTxtFromMgr(8709 ); // 전날대비 시세 설명 문구.
			StringCchCopy(buffer, 1024, pText);
			m_pStatic[1]->SetBorder(true);
			m_pStatic[1]->SetBKColorUse(true);
			m_pStatic[1]->SetText(buffer,DT_WORDBREAK);
			//---------------------------------------------------
			m_pList->Clear();

			pText = GetTxtFromMgr(1564); // 날짜.
			m_pList->SetColumn(0,80, pText);

			pText = GetTxtFromMgr(2191); // 시세
			m_pList->SetColumn(1,110, pText);

			m_pList->SetColumn(2, 20, binkan); // 아이콘 용이다.

			pText = GetTxtFromMgr(8658); // 변동치.
			m_pList->SetColumn(3, 140, pText);

			m_pList->SetBKColorUse( true );
			m_pList->SetBorder( true );

			m_pList->Refresh();					
			//---------------------------------------------------

			m_selectWhatShowList = 3; // 일주실 시제.
		}
		break;	
	case NMARKETCONDITIONS_DLG_BUTTON1:	// 시세 확인. 
	{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if(( m_selectWhatShowList < 0 )&&( m_siItemTypeIndex < 0 ) )
						return;

					if(m_selectWhatShowList == 3) // 일주일 시세 
					{
						if(m_siItemIndex	<0)// 품목까지 설정이 되어 있어야 한다.
							return;
					}

					TCHAR* pTitle = GetTxtFromMgr(5566);
					TCHAR* pText = GetTxtFromMgr(8666);
					TCHAR tempBuffer[256] = TEXT("");

					cltClient* pclclient = (cltClient*)pclClient;

					REAL32	npcrate = (REAL32)pclclient->pclItemManager->clItemStatistic.siNPCPriceRate;
					//npcrate = npcrate / (REAL32)100;

					SI64 serviceCharge = 0;
					if(m_selectWhatShowList ==1)
					{
						serviceCharge = (SI64)((REAL32)BASIC_MARKETCONDITIONS_CHARGE /100 * npcrate);
					
					}
					else if(m_selectWhatShowList ==2)
					{
						serviceCharge = 2 *(SI64)((REAL32)BASIC_MARKETCONDITIONS_CHARGE /100 * npcrate);
					}
					else if(m_selectWhatShowList ==3)
					{
						serviceCharge = 3 *(SI64)((REAL32)BASIC_MARKETCONDITIONS_CHARGE /100 * npcrate);
					}

					StringCchPrintf(tempBuffer, 256, pText,  serviceCharge);

					if( pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() < serviceCharge)
						return;

					stMsgBox MsgBox;
					MsgBox.Set( this, pTitle, tempBuffer, MSGBOX_TYPE_YESNO, 0 );
					pclclient->SetMsgBox( &MsgBox, NULL, 0 );

				}
				break;
			}
		}
		break;

	case NMARKETCONDITIONS_DLG_BUTTON2:	// 닫기
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

void CNMarketConditionsDlg::ShowList( cltMarketConditionsInfo * pclMCInfo , _SYSTEMTIME* pStimeMCInfo)
{
	// 실제로 리스트를 그려주는 함수.

	m_pList->Clear();
	memset( m_MarketConditionsInfo, 0, sizeof(cltMarketConditionsInfo) * 100 ); 
	memset( m_sTimeMarketConditionsInfo, 0, sizeof(_SYSTEMTIME) * 8 );

	memcpy( m_MarketConditionsInfo, pclMCInfo, sizeof(cltMarketConditionsInfo) * 100 ); 
	memcpy( m_sTimeMarketConditionsInfo, pStimeMCInfo, sizeof(_SYSTEMTIME) * 8 );

	SI32 ref  = 0;

	if(m_selectWhatShowList == 1) // 일반 검색.
	{
		for(SI16 i = 0; i < 100 ; i++)
		{
			stListBoxItemData Data;
			ref = pclClient->pclItemManager->FindItemRefFromUnique(m_MarketConditionsInfo[i].siUnique);

			if(ref ==0) break; // 아이템 데이터가 없음으로, 리스트를 끝낸다.

			cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[ref];

			if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
			{

				TCHAR szItemNum[10] = TEXT("");
				TCHAR szItemName[256] = TEXT("");
				TCHAR szItemConditions[256] = TEXT("");
			
				StringCchPrintf(szItemNum, 10, TEXT("%d"), i+1);
				Data.Set(0, szItemNum);

				StringCchCopy(szItemName, 256, pclClient->pclItemManager->pclItemInfo[ref]->GetName());
				Data.Set(1, szItemName);

				StringCchPrintf(szItemConditions, 256, TEXT("%I64d"), m_MarketConditionsInfo[i].siMarketConditionsMoney);
				Data.Set(2, szItemConditions);

				m_pList->AddItem(&Data);
			}
		}
	
	}
	else if(m_selectWhatShowList == 2) // 전날 대비 시세.
	{
		for(SI16 i = 0; i < 100 ; i++)
		{
			stListBoxItemData Data;
			ref = pclClient->pclItemManager->FindItemRefFromUnique(m_MarketConditionsInfo[i].siUnique);

			if(ref ==0) break; // 아이템 데이터가 없음으로, 리스트를 끝낸다.

			cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[ref];
			
			if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
			{
				TCHAR szItemName[256] = TEXT("");
				TCHAR szItemConditions[256] = TEXT("");
				TCHAR szItemDifference[256] = TEXT("");
			
				StringCchCopy(szItemName, 256, pclClient->pclItemManager->pclItemInfo[ref]->GetName());
				Data.Set(0, szItemName);

				StringCchPrintf(szItemConditions, 256, TEXT("%I64d"), m_MarketConditionsInfo[i].siMarketConditionsMoney);
				Data.Set(1, szItemConditions);

				// 시세증감에 따른 아이콘 표시.
				SI16 SprNum = GIMG_MARKETCONDITIONS_ICON;
				SI16 maketicon = 0;

				if(m_MarketConditionsInfo[i].siDifferenceMoney > 0)
				{
					maketicon = 1; // 상승.					
				}
				else if(m_MarketConditionsInfo[i].siDifferenceMoney < 0)
				{
					maketicon = 0; // 상승.				
				}
				else if(m_MarketConditionsInfo[i].siDifferenceMoney == 0)
				{
					maketicon = 2; // 상승.				
				}
				
				Data.SetSmallIconGImg( 2, SprNum, maketicon);
				
				if( m_MarketConditionsInfo[i].siDifferenceMoney == 0)
					StringCchPrintf(szItemDifference, 256, TEXT("%s"), "--");
				else
					StringCchPrintf(szItemDifference, 256, TEXT("%I64d"), m_MarketConditionsInfo[i].siDifferenceMoney);
				Data.Set(3, szItemDifference);

				m_pList->AddItem(&Data);
			}
		}
	}
	else if(m_selectWhatShowList == 3) // 일주일 시세 동향.
	{
		for(SI16 i = 0; i < 100 ; i++)
		{
			stListBoxItemData Data;
			ref = pclClient->pclItemManager->FindItemRefFromUnique(m_MarketConditionsInfo[i].siUnique);

			if(ref ==0) break; // 아이템 데이터가 없음으로, 리스트를 끝낸다.

			TCHAR szItemDate[256] = TEXT("");
			TCHAR szItemConditions[256] = TEXT("");
			TCHAR szItemDifference[256] = TEXT("");
		
			TCHAR* pText = GetTxtFromMgr(8670);
			StringCchPrintf(szItemDate, 256, pText,m_sTimeMarketConditionsInfo[i].wMonth, m_sTimeMarketConditionsInfo[i].wDay);
			Data.Set(0, szItemDate);

			StringCchPrintf(szItemConditions, 256, TEXT("%I64d"), m_MarketConditionsInfo[i].siMarketConditionsMoney);
			Data.Set(1, szItemConditions);

			// 시세증감에 따른 아이콘 표시.
			SI16 SprNum = GIMG_MARKETCONDITIONS_ICON;
			SI16 maketicon = 0;

			if(m_MarketConditionsInfo[i].siDifferenceMoney > 0)
			{
				maketicon = 1; // 상승.					
			}
			else if(m_MarketConditionsInfo[i].siDifferenceMoney < 0)
			{
				maketicon = 0; // 상승.				
			}
			else if(m_MarketConditionsInfo[i].siDifferenceMoney == 0)
			{
				maketicon = 2; // 상승.				
			}
			
			Data.SetSmallIconGImg( 2, SprNum, maketicon);

			if( m_MarketConditionsInfo[i].siDifferenceMoney == 0)
				StringCchPrintf(szItemDifference, 256, TEXT("%s"), "--");
			else
				StringCchPrintf(szItemDifference, 256, TEXT("%I64d"), m_MarketConditionsInfo[i].siDifferenceMoney);
			Data.Set(3, szItemDifference);

			m_pList->AddItem(&Data);
		}

	}

	m_pList->Refresh();
}

void CNMarketConditionsDlg::SetItemTypeInComboBox(BOOL bshowall)
{
	cltClient *pclclient = (cltClient *)pclClient;

	SI32 i;
	SI32 cnt = 0;
	for(i = 0;i < MAX_ITEMTYPE_NUMBER;i++)
	{	
		cltItemTypeInfo* pinfo = pclclient->pclItemManager->pclItemTypeInfo[i];
		//cltItemInfo* pinfo2 = pclClient->pclItemManager->pclItemInfo[i];

		if(pinfo == NULL)continue;

		// 거래 불가 품종이면 보여주지 않는 설정이라면, 
		if(bshowall == false)
		{
			if(pinfo->IsAtb(ITEMATB_NOTRADE))continue;
		}

		// 콤보리스트에서 숨기는 속성이 있다면
		if( pinfo->IsAtb(ITEMATB_HIDECOMBOLIST) ) continue;

		bool bshow = false;

		switch(pinfo->siUnique)
		{
		case ITEMTYPE_FOOD:
		case ITEMTYPE_HEAL:
		case ITEMTYPE_BOOK:
		case ITEMTYPE_TREE:
		case ITEMTYPE_CLOTH:
		case ITEMTYPE_SEAFOOD:
		case ITEMTYPE_CRYSTAL:
		case ITEMTYPE_COMPOSITIONMATERIAL:
		case ITEMTYPE_PAPER:
		case ITEMTYPE_IRON:
		case ITEMTYPE_DISH:
		case ITEMTYPE_FISHINGROD:
		case ITEMTYPE_DOLL:
		case ITEMTYPE_EVENTITEM:
		case ITEMTYPE_ETC:
			{
				bshow = true;
			}
			break;
		case ITEMTYPE_BOX:
		case ITEMTYPE_MINERAL:
		case ITEMTYPE_ANIMAL:
		case ITEMTYPE_TICKET:
		case ITEMTYPE_PREMIUM:
			{
				//bshow = SearchNoShowItem(pinfo->clItem.siUnique);
				//if( pclClient->pclItemManager->IsItemInfoAtb( pinfo2->clItem.siUnique, ITEMINFOATB_NOTSHOW_MARKETCONFITIONS )== false)
				{
					bshow = true;
				}							
			}
			break;
		}

		if(bshow == true)
		{
			stComboBoxItemData ComboBoxItemData;
			ComboBoxItemData.Init();
			ComboBoxItemData.Set( (TCHAR*)pinfo->GetName() );					

			m_pComboBox[0]->AddItem( &ComboBoxItemData );   // 타입값에 넣는다
			m_arrTypeIndex[cnt] = i;
			cnt++;
		}
	}
}

BOOL CNMarketConditionsDlg::ShowItemListInComboBox(CComboBox* pComboBox, SI32 typeindex, bool btradeinterface)
{
	cltClient *pclclient = (cltClient *)pclClient;

	cltItemTypeInfo* pinfo = pclclient->pclItemManager->pclItemTypeInfo[typeindex]; 

	if ( pinfo == NULL )	return false;

	SI32 itemtypeunique = pclClient->pclItemManager->FindItemTypeUniqueFromTypeName(pinfo->GetName());	
	
	if(itemtypeunique >=0)
	{
		int i;
		int select = 0;

		// 콤보 박스에 아이템의 종류를 설정한다. 
		for(i = 0;i < MAX_ITEMINFO_NUMBER;i++)
		{	
			cltItemInfo* pinfo = pclClient->pclItemManager->pclItemInfo[i];
			if(pinfo == NULL)continue;
			if(pinfo->siType != itemtypeunique)continue;

			// 안보이도록 설정된 것들은 안보인다.	
			if(pinfo->IsItemInfoAtb(ITEMINFOATB_NOTSHOW_MARKETCONFITIONS)) continue;

			// 주어진 타입과 같은 아이템만 열거한다. 
			// 사용자에게 보여지는 아이템인 경우에만. 
			bool bshow = false;
			if(pinfo->IsItemInfoAtb(ITEMINFOATB_SHOWLIST))
			{

				switch(pinfo->siType)
				{
					case ITEMTYPE_FOOD:
					case ITEMTYPE_HEAL:
					case ITEMTYPE_BOOK:
					case ITEMTYPE_TREE:
					case ITEMTYPE_CLOTH:
					case ITEMTYPE_SEAFOOD:
					case ITEMTYPE_CRYSTAL:
					case ITEMTYPE_COMPOSITIONMATERIAL:
					case ITEMTYPE_PAPER:
					case ITEMTYPE_IRON:
					case ITEMTYPE_DISH:
					case ITEMTYPE_FISHINGROD:
					case ITEMTYPE_DOLL:
					case ITEMTYPE_EVENTITEM:
					case ITEMTYPE_ETC:
						{
							bshow = true;
						}
						break;
					case ITEMTYPE_BOX:
					case ITEMTYPE_MINERAL:
					case ITEMTYPE_ANIMAL:
					case ITEMTYPE_TICKET:
					case ITEMTYPE_PREMIUM:
						{
							//bshow = SearchNoShowItem(pinfo->clItem.siUnique);
							if( pclClient->pclItemManager->IsItemInfoAtb( pinfo->clItem.siUnique, ITEMINFOATB_NOTSHOW_MARKETCONFITIONS )== false)
							{
								bshow = true;
							}							
						}
						break;
				}
 
				if(bshow)
				{
					//if(m_selectWhatShowList == 3) // 일주일 시세 일때만 만든다.
					{
						stComboBoxItemData ComboBoxItemData;
						ComboBoxItemData.Init();
						ComboBoxItemData.Set( (TCHAR*)pinfo->GetName() );					

						pComboBox->AddItem( &ComboBoxItemData );
						m_arrItemIndex[select] = pinfo->clItem.siUnique;
						select++;
					}

					// 해당 타입의 아이템 유니크를 저장하여, 리스트에 보여줄 데이터를 만든다.
					//m_arrItemIndex[select] = pinfo->clItem.siUnique;
				}
			}
		}
	}

	return TRUE;
}
/*
bool CNMarketConditionsDlg::SearchNoShowItem(SI16 itemUnique)
{
	// 목록에 나타나면 안되는 아이템 목록
	SI16 NoShowItem[] = {ITEMUNIQUE(6140),ITEMUNIQUE(5380)
					};

	SI16 itemNum = ( sizeof(NoShowItem) / sizeof(SI16) );

	for(SI16 i=0; i < itemNum ; i++)
	{
		if(NoShowItem[i] == itemUnique)
			return false; // 보여서는 안된다.
	}

	return true;
}
*/
void CNMarketConditionsDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 0:
		{
			if ( RetValue )
			{
				cltGameMsgRequest_MarketConditions sendMsg( m_siNPCCharUnique ,m_selectWhatShowList, m_siItemTypeIndex , m_siItemIndex );
				cltMsg clMsg( GAMEMSG_REQUEST_MARKETCONDITIONS, sizeof( cltGameMsgRequest_MarketConditions ), (BYTE*)&sendMsg );
				pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
			}
			else
				return;
		}
		break;

	}
}
