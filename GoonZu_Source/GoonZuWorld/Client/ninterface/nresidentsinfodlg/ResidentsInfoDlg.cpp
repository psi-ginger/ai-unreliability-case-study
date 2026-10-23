#include "../Client/NInterface/NResidentsInfoDlg/ResidentsInfoDlg.h"


#include "../../InterfaceMgr/InterfaceMgr.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/ListView.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../../Client.h"
#include "../CommonLogic/TextMgr/TextMgr.h"

#include "PersonInfo.h"
#include "../Common/NPCManager/NPC.h"

#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"
#include "Msg/MsgType-Quest.h"
#include "Char/CharManager/CharManager.h"
#include "../Client/ninterface/ninfodlg/NInfoDlg.h"
#include "../Client/ninterface/nhuntmapinfodlg/HuntMapInfoDlg.h"

extern cltCommonLogic* pclClient;

CNResidentsInfoDlg::CNResidentsInfoDlg() : CDialogController()
{
	m_siSelectedIndex = 0;
	m_siSelectVillageInx = 0;
	m_siResidentIndex = 0;
	m_timerCount = 0;

	m_bComboBoxInit = FALSE;

	m_pImageStatic_Portrait = NULL;

	SI32 i;
	for( i = 0; i < 2; ++i )
		m_pStatic[i] = NULL;
	m_pComboBox = NULL;
	m_pList = NULL;
	m_pButton = NULL;
	m_pEdit = NULL;
	// 검색기능
	m_pListBox_AutoMake=NULL;
	m_pEdit_Name=NULL;
	m_pButton_Accept=NULL;
	m_pStatic_Back	= NULL;//백그라운드
}

CNResidentsInfoDlg::~CNResidentsInfoDlg()
{
	SI32 i;
	for( i = 0; i < 2; ++i )
	{
		if(m_pStatic[i])
		{
			delete m_pStatic[i];
			m_pStatic[i] = NULL;
		}
	}

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}

	if( m_pList )
	{
		delete m_pList;
		m_pList = NULL;
	}

	if( m_pButton )
	{
		delete m_pButton;
		m_pButton = NULL;
	}

	if( m_pEdit )
	{
		delete m_pEdit;
		m_pEdit = NULL;
	}

	if(m_pImageStatic_Portrait)
	{
		delete m_pImageStatic_Portrait;
		m_pImageStatic_Portrait = NULL;
	}
	
	
	// 검색기능
	if(m_pListBox_AutoMake)
		delete m_pListBox_AutoMake;
	if(m_pEdit_Name)
		delete m_pEdit_Name;
	if(m_pButton_Accept)
		delete m_pButton_Accept;
	if(m_pStatic_Back)
		delete m_pStatic_Back;// 백그라운드
	// 검색기능
	m_pListBox_AutoMake=NULL;
	m_pEdit_Name=NULL;
	m_pButton_Accept = NULL;
	m_pStatic_Back = NULL; // 백그라운드

	//GP.FreeSpr(m_SprResidentsIcon);
}

void CNResidentsInfoDlg::Create( CControllerMgr *pParent )
{
	CInterfaceFile file;

	if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
	file.LoadFile(TEXT("NInterface/Data/NResidentsInfoDlg/DLG_NResidentsInfo.ddf"));
	}
	else{
		file.LoadFile(TEXT("NInterface/Data/NResidentsInfoDlg/DLG_NResidentsInfo_J.ddf"));
	}
	//file.CreatePopupDialog( this, NRESIDENTSINFO_DLG, TEXT("dialog_NResidentsInfo"), StaticCallBackDialogNResidentsInfoDlg );
	file.CreateChildDialog(this, NRESIDENTSINFO_DLG, TEXT("dialog_NResidentsInfo") , StaticCallBackDialogNResidentsInfoDlg , pParent );
	m_pStatic[0] = new CStatic( this );
	m_pStatic[1] = new CStatic( this );
	m_pList = new CList( this );
	m_pButton = new CButton( this );
	m_pEdit = new CEdit( this );
	m_pComboBox = new CComboBox( this );
	m_pImageStatic_Portrait = new CImageStatic( this );
	m_pStatic_Back = new CStatic( this );	// 백그라운드

	// 검색기능
	m_pListBox_AutoMake = new CList(this);
	m_pEdit_Name=new CEdit(this);
	m_pButton_Accept = new CButton(this);
	if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
	// 검색기능
	file.CreateControl( m_pEdit_Name, NRESIDENTSINFO_EDIT_NAME, TEXT("editbox_name") );
	file.CreateControl( m_pListBox_AutoMake, NRESIDENTSINFO_LISTBOX_AUTOBOX, TEXT("listbox_automake") );
	file.CreateControl( m_pButton_Accept, NRESIDENTSINFO_BUTTON_ACCEPT, TEXT("button_accept") );
	file.CreateControl( m_pStatic_Back, NRESIDENTSINFO_DIALOG_CSTATICBACK,   TEXT("statictext_Back")); // 백그라운드 
	//---------------------------------------------------------------------------------------------------------
	}

	file.CreateControl( m_pStatic[0], NRESIDENTSINFO_DIALOG_STATIC1,   TEXT("statictext_NONAME1")); 
	file.CreateControl( m_pStatic[1], NRESIDENTSINFO_DIALOG_STATIC2,   TEXT("statictext_NONAME2")); 
	file.CreateControl( m_pList, NRESIDENTSINFO_DIALOG_LIST,   TEXT("listbox_NONAME1")); 
	file.CreateControl( m_pButton, NRESIDENTSINFO_DIALOG_BUTTON,   TEXT("button_NONAME1")); 
	file.CreateControl( m_pEdit, NRESIDENTSINFO_DIALOG_EDIT,   TEXT("editbox_NONAME1")); 
	file.CreateControl( m_pComboBox, NRESIDENTSINFO_DIALOG_COMBOBOX,   TEXT("combobox_NONAME1")); 
	file.CreateControl( m_pImageStatic_Portrait, NRESIDENTSINFO_DIALOG_IMAGESTATIC_PORTRAIT,   TEXT("imagestatic_Portrait")); 
	
	

	TCHAR* pText= GetTxtFromMgr(3335);//등장위치.
	m_pList->SetColumn(0, 100, pText);
	m_pList->SetBorder(TRUE);
	m_pList->SetBKColorUse(TRUE);
	m_pList->Refresh();

	// 다이얼로그 초기값 셋팅
	// 검색기능
	m_pEdit_Name->Refresh();
	m_pButton_Accept->Refresh();
	m_pListBox_AutoMake->SetColumn(0,1,NULL);
	m_pListBox_AutoMake->SetColumn(1,160,NULL);
	m_pListBox_AutoMake->SetShowHeader(FALSE);
	m_pListBox_AutoMake->Show(false);
	SetActionTimer( 400 );
	SetTimerOnOff( true );

	if ( ! m_bComboBoxInit )
	{
		cltKindInfo* pclki = NULL;

		for ( SI32 i = 10; i < MAX_KIND_NUMBER; ++i )
		{
			pclki = pclClient->pclKindInfoSet->pclKI[ i ];
			//pclClient->pclKindInfoSet->pclKI[ siMonsterKind ]->siAtb == ATB_ATTACK

			if( pclki == NULL || pclki->IsAtb( ATB_SYSTEMNPC ) == false ) continue;
			if(pclki->IsAtb( ATB_NONPCLIST ) )continue;

			switch (pclClient->siServiceArea)
			{
			case ConstServiceArea_Korea :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
					continue;
				break;
			case ConstServiceArea_China :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
					continue;
				break;
			case ConstServiceArea_Japan :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
					continue;
				break;
			case ConstServiceArea_English :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
					continue;
				break;
			case ConstServiceArea_Taiwan :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
					continue;
				break;
			case ConstServiceArea_USA :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
					continue;
				break;
			case ConstServiceArea_NHNChina :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
					continue;
				break;
			case ConstServiceArea_EUROPE :
				if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
					continue;
				break;
			}

			TCHAR* pname = (TCHAR*)pclki->GetName();

			stComboBoxItemData ComboBoxItemData;
			ComboBoxItemData.Init();
			ComboBoxItemData.Set( pname );					

			m_pComboBox->AddItem( &ComboBoxItemData );
		}

		m_bComboBoxInit = true;
	}
	
	m_pComboBox->SetCursel(0);
	m_siSelectedIndex = m_pComboBox->GetSelectedIndex();

	ShowResidentInfo(m_siSelectedIndex);

	Hide();
}


void CALLBACK CNResidentsInfoDlg::StaticCallBackDialogNResidentsInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNResidentsInfoDlg *pThis = (CNResidentsInfoDlg *)pControllerMgr;
	pThis->CallBackDialogNResidentsInfoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNResidentsInfoDlg::CallBackDialogNResidentsInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;
	case NRESIDENTSINFO_DIALOG_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siSelectedIndex = m_pComboBox->GetSelectedIndex();
					
					if(m_siSelectedIndex < 0)
						break;

					ShowResidentInfo(m_siSelectedIndex);
				}
			}	break;
		}
		break;

	case NRESIDENTSINFO_DIALOG_LIST:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI32 index = m_pList->GetSelectedIndex();
					if ( index < 0 ) break ;

					SI32 mapindex = -1;
					for ( SI32 i = 0 ; i < MAX_MAP_NUMBER ; i ++ )
					{
						if ( !_tcscmp(pclclient->pclMapManager->GetMapName(i),m_pList->GetText(index, 0)))
						{
							mapindex = i ;
							break ;
						}
					}
 					m_siSelectVillageInx = mapindex;

					if( mapindex == 158 )
					{
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg2->ShowMapInfoFromOtherInfo( m_pList->GetText(index, 0));
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_HUNT);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );
					}

				}
			}
			break;
		}
		break;
	case NRESIDENTSINFO_DIALOG_BUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buffer[256] = TEXT("") ;

					// 워프할 수 있는 상황인가?
					if(pclclient->pclCM->CR[1]->CanWarpFrom() == false)
						return;

					if (  m_siSelectVillageInx > 0 &&  m_siSelectVillageInx <= CUR_VILLAGE_LIMIT )
					{
						// 공짜 워프를 할 수 있는가?
						if( pclclient->pclCM->CR[1]->CanFreeWarp( m_siSelectVillageInx ) == false )
						{
							SI32 itemunique = ITEMUNIQUE(24000) ;  // 순간이동이용권
							if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
							{
								TCHAR* pTitle = GetTxtFromMgr(1891);
								TCHAR* pText = GetTxtFromMgr(2381);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
								break;
							}
						}

						TCHAR* pTitle = GetTxtFromMgr(3189);
						TCHAR* pText = GetTxtFromMgr(1893);
						StringCchPrintf(buffer, 256, pText, pclclient->pclVillageManager->GetName( m_siSelectVillageInx));

						stMsgBox MsgBox;
						MsgBox.Set( GetParentDialog(), pTitle, buffer, MSGBOX_TYPE_YESNO, 5 );
						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( m_siSelectVillageInx );
						memcpy( TempBuffer, &m_siSelectVillageInx, Size );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );

					}
					else
					{
						TCHAR* pTitle = GetTxtFromMgr(1891);
						TCHAR* pText = GetTxtFromMgr(2382);
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
					}
				}
				break;
			}
		}
		break;
		//찾기 기능
	case NRESIDENTSINFO_EDIT_NAME:
		{
			switch(nEvent) 
			{
			case EVENT_EDITBOX_CHANGE:
				{
				}break;
			case EVENT_EDITBOX_STRING:{
				TCHAR* pItemname = m_pEdit_Name->GetText();
				if (pItemname == NULL)	return;					
				Find(pItemname);
									  }break;
			}
		}
	case NRESIDENTSINFO_LISTBOX_AUTOBOX:
		{
			switch(nEvent) 
			{
			case EVENT_LISTBOX_SELECTION:
				{
					SI16 SelectIndex= m_pListBox_AutoMake->GetSelectedIndex();
					if(SelectIndex >= 0)
					{
						TCHAR* pItemname = m_pListBox_AutoMake->GetText(SelectIndex,1);
						m_pEdit_Name->SetText(pItemname);
						m_pEdit_Name->Refresh();
					}
					m_pListBox_AutoMake->Clear();
					m_pListBox_AutoMake->Show(false);
				}break;
			}
		}
	case NRESIDENTSINFO_BUTTON_ACCEPT:
		{
			switch(nEvent) 
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR* pItemname = m_pEdit_Name->GetText();
					if (pItemname == NULL)	return;
					Find(pItemname);
				}break;
			}
		}break;
	}
}


void CNResidentsInfoDlg::Hide()
{
	//GP.FreeSpr(m_SprResidentsIcon);
	CControllerMgr::Show( false );

	return;
}

void CNResidentsInfoDlg::Show()
{
	//GP.LoadSprFromFile(TEXT("./Interface/Portrait/PortraitIcon.SPR"), &m_SprResidentsIcon );

	CControllerMgr::Show( true );

	//SetWindowText ( GetDlgItem( m_hDlg, IDC_EDIT_REGEN_POS ), TEXT("") );
	
	//m_pList->Clear();

	//	m_bShow = true;
	//	ShowWindow( m_hDlg, SW_SHOWNORMAL );
	//	SetForegroundWindow( m_hDlg );

	return;
}

void CNResidentsInfoDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
  	if(m_siSelectedIndex < 0) 
		return;

	//GP.PutSpr(&m_SprResidentsIcon, ScreenRX + GetX() + 20, ScreenRY + GetY() + 100, m_siResidentIndex );

}

void  CNResidentsInfoDlg::SetRegenPos( TCHAR *text )
{
	TCHAR buf[MAX_LISTBOXITEM_TEXT_LEN];
	TCHAR* pText = GetTxtFromMgr(1287);
	if ( !_tcscmp(text,TEXT("")) ) 
		StringCchCopy(buf, MAX_LISTBOXITEM_TEXT_LEN, pText);
	else
		StringCchCopy(buf, MAX_LISTBOXITEM_TEXT_LEN, text);

	m_pList->Clear();

	stListBoxItemData Data;

	Data.Set( 0, buf );

	m_pList->AddItem( &Data );
	m_pList->Refresh();

	return;
}

void CNResidentsInfoDlg::SetRegenPos( UI08 mapindex [] , SI16 maxmap )
{
	SI32 i;
	cltClient *pclclient = (cltClient*)pclClient;

	TCHAR buffer[256] = TEXT("");

	m_pList->Clear();

	for(i = 0;i < maxmap ;i++)
	{
		if ( mapindex[i] <= 0 ) continue ;

		if ( mapindex[i] > 0 && mapindex[i] <= CUR_VILLAGE_LIMIT )
			StringCchPrintf(buffer, 256, TEXT("%s"),pclclient->pclVillageManager->GetName(mapindex[i]));
		else
			StringCchPrintf(buffer, 256, TEXT("%s"),pclclient->pclMapManager->GetMapName(mapindex[i]));
		if ( !_tcscmp(buffer,TEXT("")) ) continue;

	//	SI32 index = ListAddString(m_hDlg, IDC_LIST_REGEN_POS, (LPARAM)buffer);

		stListBoxItemData Data;

		Data.Set( 0, buffer );

		m_pList->AddItem( &Data );
		

	//	if ( mapindex[i] > 0 && mapindex[i] <= CUR_VILLAGE_LIMIT )
	//		ListSetItemData(m_hDlg, IDC_LIST_REGEN_POS, index, mapindex[i]);
	//		m_pList->
	}
	m_pList->Refresh();
}

void CNResidentsInfoDlg::AddRegenPos( TCHAR *text )
{
	if ( !_tcscmp(text,TEXT("")) ) return ;
//	ListAddString(m_hDlg, IDC_LIST_REGEN_POS, (LPARAM)text);

	stListBoxItemData Data;

	Data.Set( 0, text );

	m_pList->AddItem( &Data );
	m_pList->Refresh();
}

void CNResidentsInfoDlg::ShowResidentInfo( SI32 index )
{
	SI32 siSystemNPCKind = pclClient->pclKindInfoSet->FindKindFromName( m_pComboBox->GetText(index) );

	m_siResidentIndex = siSystemNPCKind - 1;

	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siSystemNPCKind, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pImageStatic_Portrait->SetFileName(szPortrait);
	m_pImageStatic_Portrait->SetFontIndex(siPortraitFont);

	TCHAR szexplain[512];
	if(pclClient->pclQuestManager->GetQuestExplain(siSystemNPCKind, szexplain, 512))
	{
		StringCchCopy( m_strBaseInfo, 512, szexplain );
		m_pEdit->SetText(m_strBaseInfo);
	}
	else
	{
		m_pEdit->SetText(TEXT(""));
	}

	if ( siSystemNPCKind == pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN2") ) )
	{
		TCHAR* pText = GetTxtFromMgr(2383);
		SetRegenPos(pText);
		return;
	}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_RUSSIATRADER")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(299);
	//	SetRegenPos(pText);
	//	return;
	//}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHINATRADER")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(300);
	//	SetRegenPos(pText);
	//	return;
	//}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash( TEXT("KIND_SUMMONMERCHANT") ) )
	{
		TCHAR* pText = GetTxtFromMgr(300);
		SetRegenPos(pText);
		return;
	}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HAMEL")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(2384);
	//	SetRegenPos(pText);
	//	return;
	//}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_JANGILSAN")) )
	{
		TCHAR* pText = GetTxtFromMgr(2385);
		SetRegenPos(pText);
		return;
	}
	//cyj 왕사범 추가
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BATTLEZONE")) )
	{
		char* pText = GetTxtFromMgr(5497);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_JANGGILSAN")) )
	{
		char* pText = GetTxtFromMgr(6859);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")) )
	{
		TCHAR* pText = GetTxtFromMgr(518);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_JAPANTRADER")) )
	{
		TCHAR* pText = GetTxtFromMgr(518);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_PRISONER")) )
	{
		TCHAR* pText = GetTxtFromMgr(2386);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) )
	{
		TCHAR* pText = GetTxtFromMgr(299);
		SetRegenPos(pText);
		return;
	}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BOBUSANG")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(988);
	//	SetRegenPos(pText);
	//	return;
	//}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_WANGCHO")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(988);
	//	SetRegenPos(pText);
	//	return;
	//}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HONGILDONG")) )
	{
		TCHAR* pText = GetTxtFromMgr(5235);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ZORRORACCOON_NPC")) )//조로라쿤위치 추가.
	{
		TCHAR* pText = GetTxtFromMgr(7559);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HAPPYBEAN_NPC1")) )//해니
	{
		TCHAR* pText = GetTxtFromMgr(994);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HAPPYBEAN_NPC2")) )//피니
	{
		TCHAR* pText = GetTxtFromMgr(7157);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HAPPYBEAN_NPC3")) )//비니
	{
		TCHAR* pText = GetTxtFromMgr(7158);
		SetRegenPos(pText);
		return;
	}
	// 퀘스트 NPC (소피아, 스파키) 추가 (PCK - 2007.11.19)
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SOPHIA")) )// 소피아
	{
		TCHAR* pText = GetTxtFromMgr(7157);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SPARKY")) )// 스파키
	{
		TCHAR* pText = GetTxtFromMgr(7158);
		SetRegenPos(pText);
		return;
	}
	// 퀘스트 NPC 메리엘 추가 (PCK - 2007.11.29)
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MERIEL")) )// 메리엘
	{
		TCHAR* pText = GetTxtFromMgr(7710);
		SetRegenPos(pText);
		return;
	}
	// 퀘스트 NPC 브라이 추가 (PCK - 2007.12.07)
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BRAI")) )// 브라이
	{
		TCHAR* pText = GetTxtFromMgr(7711);
		SetRegenPos(pText);

		SI16 questtype, questunique, para1, para2;
		SI16 questclearcond =  QUEST_CLEAR_COND_NPCINFO;
		if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
		{
			if( siSystemNPCKind == para1 )
			{
				cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
				cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				cltClient* pclclient = (cltClient* )pclClient;
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BELITA")) )	// 벨리타
	{
		TCHAR* pText = GetTxtFromMgr(7734);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HINEAS")) )	// 히니어스
	{
		TCHAR* pText = GetTxtFromMgr(7734);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_KELAN")) )	// 켈란
	{
		TCHAR* pText = GetTxtFromMgr(7735);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_JEFFREY")) ) // 제프리
	{
		TCHAR* pText = GetTxtFromMgr(7895);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MOLDA")) ) // 몰다
	{
		TCHAR* pText = GetTxtFromMgr(8220);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_SEBRINO")) ) // 세브리노
	{
		TCHAR* pText = GetTxtFromMgr(8221);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ELIANA")) ) // 일리아나
	{
		TCHAR* pText = GetTxtFromMgr(8222);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_TITLENPC")) ) //[진성] 명칭여신 위치 정보 추가.<텍스트메니져로 수정필요.> 2008-5-23
	{
		if(true == pclClient->IsWhereServiceArea( ConstServiceArea_Korea ))
		{
			TCHAR* pText = GetTxtFromMgr(8809);
			SetRegenPos(pText);
			return;
		}
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ANNIVERSARY")) ) // 대만 1주년 기념 NPC
	{
		TCHAR* pText = GetTxtFromMgr(5497);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ELIXIRNPC")) ) // 키엘리니(엘릭서NPC)
	{
		TCHAR* pText = GetTxtFromMgr(9782);
		SetRegenPos(pText);
		return;
	}
	//else if (siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BBB")) ) // 키엘리니(복불복NPC)-임시
	//{
	//	TCHAR* pText = GetTxtFromMgr(9782);
	//	SetRegenPos(pText);
	//	return;
	//}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ALEX")) ) // 알렉스(전투향상술NPC)
	{
		TCHAR* pText = GetTxtFromMgr(9852);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_LLOYD")) ) // 로이드 (버려진 요새)
	{
		TCHAR* pText = GetTxtFromMgr(30053);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_ARA")) ) // 아라 (잃어버린 세계)
	{
		TCHAR* pText = GetTxtFromMgr(30054);
		SetRegenPos(pText);
		return;
	}
	else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_COPFIELD")) ) // 스킬북.
	{
		TCHAR* pText = GetTxtFromMgr(15641);
		SetRegenPos(pText);
		return;
	}

	// 복불복용 NPC추가.
	//else if(siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BOKBULBOK")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr();
	//	SetRegenPos(pText);
	//	return;
	//}

	//else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_FRANK")) ) // 프랑크
	//{
	//	TCHAR* pText = GetTxtFromMgr(8223);
	//	SetRegenPos(pText);
	//	return;
	//}
	//else if( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_AIRIN")) ) // 아이린
	//{
	//	TCHAR* pText = GetTxtFromMgr(8224);
	//	SetRegenPos(pText);
	//	return;
	//}
	


	//---------------------------
	// 서버로 NPC위치를 요청한다.
	//----------------------------
	cltGameMsgRequest_NPCPos clinfo(siSystemNPCKind);
	cltMsg clMsg(GAMEMSG_REQUEST_NPCPOS, sizeof(clinfo), (BYTE*)&clinfo);
	cltClient* pclclient = (cltClient*)pclClient;

	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);	
}


void CNResidentsInfoDlg::ShowResidentInfoByKind( SI32 Kind, TCHAR * Name )
{
	SI32 siSystemNPCKind = Kind;

	m_siResidentIndex = siSystemNPCKind - 1;

	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(siSystemNPCKind, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pImageStatic_Portrait->SetFileName(szPortrait);
	m_pImageStatic_Portrait->SetFontIndex(siPortraitFont);

	//m_siSelectedIndex
	m_pComboBox->SetText(Name);

	TCHAR szexplain[512];
	if(pclClient->pclQuestManager->GetQuestExplain(siSystemNPCKind, szexplain, 512))
	{
		StringCchCopy( m_strBaseInfo, 512, szexplain );
		m_pEdit->SetText(m_strBaseInfo);
	}
	else
	{
		m_pEdit->SetText(TEXT(""));
	}

	if ( siSystemNPCKind == pclClient->GetUniqueFromHash( TEXT("KIND_WAITINGMAN2") ) )
	{
		TCHAR* pText = GetTxtFromMgr(2383);
		SetRegenPos(pText);
		return;
	}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_RUSSIATRADER")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(299);
	//	SetRegenPos(pText);
	//	return;
	//}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_CHINATRADER")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(300);
	//	SetRegenPos(pText);
	//	return;
	//}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash( TEXT("KIND_SUMMONMERCHANT") ) )
	{
		TCHAR* pText = GetTxtFromMgr(300);
		SetRegenPos(pText);
		return;
	}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HAMEL")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(2384);
	//	SetRegenPos(pText);
	//	return;
	//}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_JANGILSAN")) )
	{
		TCHAR* pText = GetTxtFromMgr(2385);
		SetRegenPos(pText);
		return;
	}
	//cyj 왕사범 추가
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BATTLEZONE")) )
	{
		char* pText = GetTxtFromMgr(5497);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTPOTTER")) )
	{
		TCHAR* pText = GetTxtFromMgr(518);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_JAPANTRADER")) )
	{
		TCHAR* pText = GetTxtFromMgr(518);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_PRISONER")) )
	{
		TCHAR* pText = GetTxtFromMgr(2386);
		SetRegenPos(pText);
		return;
	}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_MERCHANTHORSE")) )
	{
		TCHAR* pText = GetTxtFromMgr(299);
		SetRegenPos(pText);
		return;
	}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_BOBUSANG")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(988);
	//	SetRegenPos(pText);
	//	return;
	//}
	//else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_WANGCHO")) )
	//{
	//	TCHAR* pText = GetTxtFromMgr(988);
	//	SetRegenPos(pText);
	//	return;
	//}
	else if ( siSystemNPCKind == pclClient->GetUniqueFromHash(TEXT("KIND_HONGILDONG")) )
	{
		TCHAR* pText = GetTxtFromMgr(5235);
		SetRegenPos(pText);
		return;
	}

	//---------------------------
	// 서버로 NPC위치를 요청한다.
	//----------------------------
	cltGameMsgRequest_NPCPos clinfo(siSystemNPCKind);
	cltMsg clMsg(GAMEMSG_REQUEST_NPCPOS, sizeof(clinfo), (BYTE*)&clinfo);
	cltClient* pclclient = (cltClient*)pclClient;

	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);	
}

void CNResidentsInfoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 5:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				//KHY - 0703 - 게임 머니로 귀환하기.클레스 수정.
				cltGameMsgRequest_HuntMapWarp pclInfo(*((SI32*)pData),0);
				cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

bool CNResidentsInfoDlg::IsSelectedNPC( SI32 siKind )
{
  	cltClient *pclclient = (cltClient*)pclClient;
 	if(m_siResidentIndex < 0) return false;

	//return ( pclClient->pclKindInfoSet->FindKindFromName( m_pComboBox->GetText(m_siSelectedIndex)) == siKind);
	return ( m_siResidentIndex+1 == siKind);
}

void CNResidentsInfoDlg :: FindNpc(TCHAR * szNpcname){
	if(szNpcname == NULL)			return;
	//아이템이름으로 인덱스를 찾아서
	SI16 FindIndex = m_pComboBox->FindIndexFromText(szNpcname);
	if( FindIndex < 0 )	return;
	//해당 인덱스를 탑으로 
	m_pComboBox->SetCursel(FindIndex);
}
//검색기능을 위해

void CNResidentsInfoDlg :: Find(TCHAR* pItemname)
{
	if ( strlen(pItemname) == 0) return;// 빈문자열일 경우는 리턴
	Show();
	FindNpc(pItemname);
	m_pEdit_Name->Clear();
}
void CNResidentsInfoDlg ::Update()
{

	cltClient *pclclient = (cltClient *)pclClient;
	NTCHARString64		kInputText(m_pEdit_Name->GetText());

	if( m_pEdit_Name->IsFocus() == false && 
		!(m_pListBox_AutoMake->IsFocus() == true || m_pListBox_AutoMake->GetScroll()->IsFocus() == true) )
	{
		m_pListBox_AutoMake->Clear();
		m_pListBox_AutoMake->Refresh();
		m_pListBox_AutoMake->Show(false);
		//pclClient->pclLog->LogPrint("Lose Focus\n");
		return;
	}

	if (kInputText.IsEmpty())
	{
		m_pListBox_AutoMake->Clear();
		m_pListBox_AutoMake->Refresh();
		m_pListBox_AutoMake->Show(false);
		m_LastInput = kInputText;
		return;
	}

	// 같은 내용은 처리하지 않는다.
	if (m_LastInput == kInputText)		return;
	m_pListBox_AutoMake->Clear();
	//--------------------------------
	cltKindInfo* pclki = NULL;
			for ( SI32 i = 10; i < MAX_KIND_NUMBER; ++i )
			{
				pclki = pclClient->pclKindInfoSet->pclKI[ i ];
				//pclClient->pclKindInfoSet->pclKI[ siMonsterKind ]->siAtb == ATB_ATTACK

				if( pclki == NULL || pclki->IsAtb( ATB_SYSTEMNPC ) == false ) continue;
				if(pclki->IsAtb( ATB_NONPCLIST ) )continue;

				switch (pclClient->siServiceArea)
				{
				case ConstServiceArea_Korea :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_KOREA ) == false)
						continue;
					break;
				case ConstServiceArea_China :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_CHINA ) == false)
						continue;
					break;
				case ConstServiceArea_Japan :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_JAPAN ) == false)
						continue;
					break;
				case ConstServiceArea_English :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_ENGLISH ) == false)
						continue;
					break;
				case ConstServiceArea_Taiwan :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_TAIWAN ) == false)
						continue;
					break;
				case ConstServiceArea_USA :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_USA ) == false)
						continue;
					break;
				case ConstServiceArea_NHNChina :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_NHNCHINA ) == false)
						continue;
					break;
				case ConstServiceArea_EUROPE :
					if ( pclki->IsServiceAreaAtb( SERVICEAREA_ATB_EUROPE ) == false)
						continue;
					break;
				}
				TCHAR* findname = _tcsstr((TCHAR*)pclki->GetName(), kInputText);
				if(findname)		AddListItem((TCHAR*)pclki->GetName());
			}				
		//----------------------------------	
		m_pListBox_AutoMake->Refresh();
        m_pListBox_AutoMake->Show(true);

	if( m_pListBox_AutoMake->GetListNum() <= 1 )
	{
		NTCHARString64 kText(m_pListBox_AutoMake->GetText(0, 1));
		if( kText == kInputText)
		{
			m_pListBox_AutoMake->Clear();
			m_pListBox_AutoMake->Show(false);
		}
	}

	m_LastInput = kInputText;
}
void CNResidentsInfoDlg :: AddListItem(TCHAR* findname)
{
	if(findname == NULL)	return;
	SI16 cnt = 0;
	stListBoxItemData Data;

	Data.Init();
	Data.Set( 1, findname );
	m_pListBox_AutoMake->AddItem(&Data);
}
void CNResidentsInfoDlg::Action()
{	
	if( false == IsShow() )
		return;
	if( m_timerCount >= LEAST_COMMON_MULTILPLE + 1 )
		m_timerCount = 1;
	m_timerCount++;
	Update();
}