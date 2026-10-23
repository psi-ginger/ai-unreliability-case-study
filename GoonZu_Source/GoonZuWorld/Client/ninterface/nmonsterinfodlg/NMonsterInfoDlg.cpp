/* ==========================================================================
	클래스 :		CNMonsterInfoDlg

	작성일 :		05/02/25
	작성자 :		정용래

	목  적 :		CNMonsterInfoDlg 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 몬스터 정보 다이얼로그를 생성한다.

	용  법 :		Client.h에 선언된 CNMonsterInfoDlg			*m_pNMonsterInfoDlg로
					컨트롤 한다.

	변경사항 :		없음
	
	차후작업 :		
					1. 버튼 이미지 로딩 필요, 현재는 "게시판" 텍스트만 씀
   ========================================================================*/
//#if (defined(__LEON_START_ALL__) || defined(__LEON_NMONSTERINFODLG__)) && defined(_DEBUG)

#include "NMonsterInfoDlg.h"

#include "../../GlobalImgFile/GlobalImgFile.h"

#include "../../Client/client.h"
#include "..\Common\NPCManager\NPC.h"
#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
#include "../../client/ItemInfo/ItemInfo.h"
#include "../niteminfodlg/iteminfodlg.h"
#include "../../Client/ninterface/ninfodlg/NInfoDlg.h"

#include "../../common/Char/CharManager/CharManager.h"

#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"
#include "../../InterfaceMgr/Interface/List.h"	
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/RadioButton.h"

#include "../../InterfaceMgr/InterfaceFile.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../ninterface/nhuntmapinfodlg/HuntMapInfoDlg.h"

#include "../../commonlogic/Msg/MsgType-Quest.h"

#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

extern cltCommonLogic* pclClient;


CNMonsterInfoDlg::CNMonsterInfoDlg() : CDialogController()
{
	m_siSelectedIndex = -1;
	m_timerCount = 0;

	m_pMonsterStatic=NULL;
	m_pMonsterSelectComboBox=NULL;
	m_pNameSortRadioButton=NULL;
	m_pLevelSortRadioButton=NULL;
	m_pSearchDetailButton=NULL;
	m_pMonsterPortraitImage=NULL;
	m_pOutlineInfo=NULL;
	m_pMonsterInfoEdit=NULL;
	m_pItemListStatic=NULL;
	m_pItemListList=NULL;
	m_pSummonListStatic=NULL;
	m_pSummonListList=NULL;
//	m_pToleranceListStatic=NULL;
//	m_pToleranceListList=NULL;
	// 검색기능
	m_pListBox_AutoMake=NULL;
	m_pEdit_Name=NULL;
	m_pButton_Accept=NULL;
	m_pStatic_Back	= NULL;//백그라운드


	Initialize();
}

CNMonsterInfoDlg::~CNMonsterInfoDlg()
{
	Destroy();
}

void CNMonsterInfoDlg::Initialize()
{
	m_bComboBoxInit = false;
	m_siSort = 0 ;
}

void CNMonsterInfoDlg::Destroy()
{
	if(m_pMonsterStatic)
		delete m_pMonsterStatic;
	if(m_pMonsterSelectComboBox)
		delete m_pMonsterSelectComboBox;
	if(m_pNameSortRadioButton)
		delete m_pNameSortRadioButton;
	if(m_pLevelSortRadioButton)
		delete m_pLevelSortRadioButton;
	if(m_pSearchDetailButton)
		delete m_pSearchDetailButton;
	if(m_pMonsterPortraitImage)
		delete m_pMonsterPortraitImage;
	if(m_pMonsterInfoEdit)
		delete m_pMonsterInfoEdit;
	if(m_pItemListStatic)
		delete m_pItemListStatic;
	if(m_pItemListList)
		delete m_pItemListList;
	if(m_pSummonListStatic)
		delete m_pSummonListStatic;
	if(m_pSummonListList)
		delete m_pSummonListList;
	// 검색기능
	if(m_pListBox_AutoMake)
		delete m_pListBox_AutoMake;
	if(m_pEdit_Name)
		delete m_pEdit_Name;
	if(m_pButton_Accept)
		delete m_pButton_Accept;
	if(m_pStatic_Back)
		delete m_pStatic_Back;// 백그라운드
	/*
	if(m_pToleranceListStatic)
		delete m_pToleranceListStatic;
	if(m_pToleranceListList)
		delete m_pToleranceListList;
*/
	m_pMonsterStatic=NULL;
	m_pMonsterSelectComboBox=NULL;
	m_pNameSortRadioButton=NULL;
	m_pLevelSortRadioButton=NULL;
	m_pSearchDetailButton=NULL;
	m_pMonsterPortraitImage=NULL;
	m_pMonsterInfoEdit=NULL;
	m_pItemListStatic=NULL;
	m_pItemListList=NULL;
	m_pSummonListStatic=NULL;
	m_pSummonListList=NULL;
//	m_pToleranceListStatic=NULL;
	//m_pToleranceListList=NULL;
	// 검색기능
	m_pListBox_AutoMake=NULL;
	m_pEdit_Name=NULL;
	m_pButton_Accept = NULL;
	m_pStatic_Back = NULL; // 백그라운드

	SAFE_DELETE(m_pOutlineInfo);

	return;
}

void CNMonsterInfoDlg::Create( CControllerMgr * pParent)
{
	if ( ! IsCreate() )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		TCHAR* pText= GetTxtFromMgr(3422);

		CInterfaceFile file;

		if(pclClient->IsCountrySwitch(Switch_FindInfo)) {		
			file.LoadFile(TEXT("NInterface/Data/DLG_NMonsterInfo.ddf"));
		}
		else{
			file.LoadFile(TEXT("NInterface/Data/DLG_NMonsterInfo_J.ddf"));
		}
		//file.CreatePopupDialog( this, NMONSTERINFO_DLG, TEXT("dialog_nmonsterinfo"), StaticNMonsterInfoDlgProc);
		file.CreateChildDialog(this, NMONSTERINFO_DLG, TEXT("dialog_nmonsterinfo") , StaticNMonsterInfoDlgProc , pParent );

		m_pMonsterStatic= new CStatic( this );
		m_pNameSortRadioButton= new CRadioButton( this );
		m_pLevelSortRadioButton= new CRadioButton( this );
		m_pSearchDetailButton= new CButton( this );
		m_pMonsterPortraitImage= new CImageStatic( this );
		m_pOutlineInfo= new COutLine( this );
		m_pMonsterInfoEdit= new CEdit( this );
		m_pItemListStatic= new CStatic( this );
		m_pItemListList= new CList( this );
		m_pSummonListStatic= new CStatic( this );
		m_pSummonListList= new CList( this );
		//m_pToleranceListStatic= new CStatic( this );
		//m_pToleranceListList= new CList ( this );
		m_pMonsterSelectComboBox= new CComboBox( this );
		// 검색기능
		m_pListBox_AutoMake = new CList(this);
		m_pEdit_Name=new CEdit(this);
		m_pButton_Accept = new CButton(this);
		m_pStatic_Back = new CStatic( this );	// 백그라운드
		if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
		// 검색기능
		file.CreateControl( m_pEdit_Name, NMONSTERINFODLG_EDIT_NAME, TEXT("editbox_name") );
		file.CreateControl( m_pListBox_AutoMake, NMONSTERINFODLG_LISTBOX_AUTOBOX, TEXT("listbox_automake") );
		file.CreateControl( m_pButton_Accept, NMONSTERINFODLG_BUTTON_ACCEPT, TEXT("button_accept") );
		file.CreateControl( m_pStatic_Back, NMONSTERINFODLG_DIALOG_CSTATICBACK,   TEXT("statictext_Back")); // 백그라운드 
		//---------------------------------------------------------------------------------------------------------
		}
		file.CreateControl( m_pMonsterStatic, NMONSTERINFODLG_DIALOG_MONSTERSTATIC, TEXT("statictext_monster_select") );
		file.CreateControl( m_pNameSortRadioButton, NMONSTERINFODLG_DIALOG_NAMESORTRADIOBUTTON, TEXT("radiobutton_namesort") );
		file.CreateControl( m_pLevelSortRadioButton, NMONSTERINFODLG_DIALOG_LEVELSORTRADIOBUTTON, TEXT("radiobutton_levelsort") );
		file.CreateControl( m_pSearchDetailButton, NMONSTERINFODLG_DIALOG_SEARCHDETAILBUTTON, TEXT("button_search_detail") );
		file.CreateControl( m_pMonsterPortraitImage, NMONSTERINFODLG_DIALOG_MONSTERPORTRAITIMAGE, TEXT("imagestatic_monster_portrait") );
		file.CreateControl( m_pMonsterInfoEdit, NMONSTERINFODLG_DIALOG_MONSTERINFOEDIT, TEXT("editbox_monster_info") );
		file.CreateControl( m_pOutlineInfo, NMONSTERINFODLG_DIALOG_OUTLINEINFO, TEXT("outline_NONAME1"));
		file.CreateControl( m_pItemListStatic, NMONSTERINFODLG_DIALOG_ITEMLISTSTATIC, TEXT("statictext_monster_item") );
		file.CreateControl( m_pItemListList, NMONSTERINFODLG_DIALOG_ITEMLISTLISTBOX, TEXT("listbox_monster_item") );
		file.CreateControl( m_pSummonListStatic, NMONSTERINFODLG_DIALOG_SUMMONLISTSTATIC, TEXT("statictext_regen_pos") );
		file.CreateControl( m_pSummonListList, NMONSTERINFODLG_DIALOG_SUMMONLISTLISTBOX, TEXT("listbox_regen_pos") );
//		file.CreateControl( m_pToleranceListStatic, NMONSTERINFODLG_DIALOG_TOLERANCELISTSTATIC, TEXT("statictext_monster_tolerance") );
//		file.CreateControl( m_pToleranceListList, NMONSTERINFODLG_DIALOG_TOLERANCELISTLISTBOX, TEXT("listbox_monster_tolerance") );
		file.CreateControl( m_pMonsterSelectComboBox, NMONSTERINFODLG_DIALOG_MONSTERSELECTCOMBOBOX, TEXT("combobox_monster_select") );
		

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
		// ItemList
		if(m_pItemListList)
		{
			m_pSearchDetailButton->Show(false);

			m_pItemListList->SetFontSize( 12 );
			m_pItemListList->SetFontWeight( 0 );
			//ListView_SetExtendedListViewStyle( m_hItemListCtrl,  LVS_EX_TRACKSELECT | LVS_EX_ONECLICKACTIVATE | LVS_EX_UNDERLINEHOT);
			TCHAR* pText = GetTxtFromMgr(1831);
			if ( pclClient->siServiceArea == ConstServiceArea_English	||
				pclClient->siServiceArea == ConstServiceArea_NHNChina	||
				pclClient->siServiceArea == ConstServiceArea_EUROPE
			)
			{
				m_pItemListList->SetColumn( 0, 132, pText );
				pText = GetTxtFromMgr(1832);
				m_pItemListList->SetColumn( 1, 108, pText );
				pText = GetTxtFromMgr(2362);
				m_pItemListList->SetColumn( 2, 88, pText );
				m_pItemListList->SetColumn( 3, 16, NULL );

			}
			else
			{
				m_pItemListList->SetColumn( 0, 112, pText );
				pText = GetTxtFromMgr(1832);
				m_pItemListList->SetColumn( 1, 108, pText );
				pText = GetTxtFromMgr(2362);
				m_pItemListList->SetColumn( 2, 108, pText );
				m_pItemListList->SetColumn( 3, 16, NULL );
			}

			m_pItemListList->SetBorder( true );
			m_pItemListList->SetBKColorUse( true );

			m_pItemListList->SetListBoxColor( LISTBOX_COLOR_GREEN );
			m_pItemListList->SetDrawLine( true );
			m_pItemListList->SetDrawLineColor( LISTBOX_LINECOLOR_GREEN );
			
			m_pItemListList->Refresh();
			SetActionTimer( 400 );
			SetTimerOnOff( false );


		}
/*
		// ToleranceList
		if(m_pToleranceListList)
		{
			m_pToleranceListList->SetFontSize( 12 );
			m_pToleranceListList->SetFontWeight( 0 );
			//ListView_SetExtendedListViewStyle( m_hToleranceListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
			TCHAR* pText = GetTxtFromMgr(1023);
			m_pToleranceListList->SetColumn( 0, 164, pText );
			pText = GetTxtFromMgr(2363);
			m_pToleranceListList->SetColumn( 1, 164, pText );
			m_pToleranceListList->SetColumn( 2, 16, NULL );

			m_pToleranceListList->SetBorder( true );
			m_pToleranceListList->SetBKColorUse( true );

			m_pToleranceListList->Refresh();
		}
*/
		// SummonList
		if(m_pSummonListList)
		{
			m_pSummonListList->SetFontSize( 12 );
			m_pSummonListList->SetFontWeight( 0 );
			//ListView_SetExtendedListViewStyle( m_hSummonListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE );
			TCHAR* pText = GetTxtFromMgr(1611);
			if ( pclClient->siServiceArea == ConstServiceArea_English	||
				pclClient->siServiceArea == ConstServiceArea_NHNChina	||
				pclClient->siServiceArea == ConstServiceArea_EUROPE
			)
			{
				m_pSummonListList->SetColumn( 0, 125, pText );
				pText = GetTxtFromMgr(2364);
				m_pSummonListList->SetColumn( 1, 75, pText );
				pText = GetTxtFromMgr(2365);
				m_pSummonListList->SetColumn( 2, 128, pText );
			}
			else
			{
				m_pSummonListList->SetColumn( 0, 112, pText );
				pText = GetTxtFromMgr(2364);
				m_pSummonListList->SetColumn( 1, 88, pText );
				pText = GetTxtFromMgr(2365);
				m_pSummonListList->SetColumn( 2, 128, pText );
			}
			m_pSummonListList->SetColumn( 3, 16, NULL);

			m_pSummonListList->SetListBoxColor( LISTBOX_COLOR_RED );
			m_pSummonListList->SetBorder( true );
			m_pSummonListList->SetBKColorUse( true );
			m_pSummonListList->SetDrawLine( true );
			m_pSummonListList->SetDrawLineColor( LISTBOX_LINECOLOR_RED );

			m_pSummonListList->Refresh();
		}

		m_pNameSortRadioButton->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON );

		if ( AddCombo() )
			m_bComboBoxInit = true;

		if(m_pMonsterSelectComboBox->GetItemNum() > 0 )
			m_pMonsterSelectComboBox->SetCursel( 0 );
	}
	else
	{
		DeleteDialog();
	}

	return;
}

void CALLBACK CNMonsterInfoDlg::StaticNMonsterInfoDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMonsterInfoDlg *pThis = (CNMonsterInfoDlg*)pControllerMgr;
	pThis->NMonsterInfoDlgProc( nEvent, nControlID, pControl);
}

void CALLBACK CNMonsterInfoDlg::NMonsterInfoDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{

	case NMONSTERINFODLG_DIALOG_MONSTERSELECTCOMBOBOX:
		{
			switch( nEvent)
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siSelectedIndex = m_pMonsterSelectComboBox->GetSelectedIndex();

					if ( m_siSelectedIndex > -1 )
					{
						TCHAR strMonsterName[ 128 ] = TEXT("");
						StringCchCopy(strMonsterName, 128, m_pMonsterSelectComboBox->GetText(m_siSelectedIndex) );

						ShowMonsterInfo_Internal( strMonsterName );
					}
				}
				break;
			}
		}
		break;

	case NMONSTERINFODLG_DIALOG_NAMESORTRADIOBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pNameSortRadioButton->IsCheck() == true )
					{
						if ( m_siSort == 0 ) break ;
						m_siSort = 0 ;
						AddCombo();
					}
				}
				break;
			}
		}
		break;

	case NMONSTERINFODLG_DIALOG_LEVELSORTRADIOBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_RADIOBUTTON_CHANGED:
				{
					if(m_pLevelSortRadioButton->IsCheck() == true )
					{
						if ( m_siSort == 1 ) break ;
						m_siSort = 1 ;
						AddCombo();
					}
				}
				break;
			}
		}
		break;

	case NMONSTERINFODLG_DIALOG_SEARCHDETAILBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					/*cyj 몬스터상세정보버튼 동작안하도록 수정
					TCHAR strMonsterName[ 128 ] = TEXT("");
					
					// leon todo - GetName 부분 고칠 것
					m_siSelectedIndex = m_pMonsterSelectComboBox->GetSelectedIndex();
					if ( m_siSelectedIndex > -1 )
					{
						_tcscpy(strMonsterName, m_pMonsterSelectComboBox->GetText( m_siSelectedIndex ));
					}
					cltClient *pclclient = (cltClient *)pclClient;
					pclclient->m_pSearchDetailDlg->Show(BULLETINBOARD_KNOWLEDGE, strMonsterName);
					*/
				}
				break;
			}
			break;
		}

	case NMONSTERINFODLG_DIALOG_ITEMLISTLISTBOX:
		{
			
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if( GetParentDialog()->GetDialogControllerID() == NINFO_SUB_DLG ) return;
 

					TCHAR buffer[256]; 
					SI16 tmpRow = 0, tmpColumn = 0;
					stListBoxItemData tmpListBoxItemData;
					tmpListBoxItemData.Init();

					m_pItemListList->GetSelectedIndex( &tmpRow, &tmpColumn );

					if( tmpColumn == 0  || tmpColumn == 1 || tmpColumn == 2 )
					{
						StringCchCopy( buffer, 256, m_pItemListList->GetText( tmpRow, 0 ));

						/*
						 //leon todo - combo하고 List가 셋팅이 안됨..ㅡ.ㅡ;..
						if(((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ]))	
						{
							((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ])->SetItemInfo(buffer);
							((CNItemInfoDlg*)pclclient->m_pDialog[ NITEMINFO_DLG ])->ShowItemInfo(buffer);
						}*/

						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->SetItemInfo(buffer);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg0->ShowItemInfo(buffer);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_ITEM);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );
					}
				}
				break;
			}
			break;
		}
	case NMONSTERINFODLG_DIALOG_SUMMONLISTLISTBOX:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					if( GetParentDialog()->GetDialogControllerID() == NINFO_SUB_DLG ) return;

					TCHAR buffer[256]; 
					SI16 tmpRow = 0, tmpColumn = 0; 
				
					m_pSummonListList->GetSelectedIndex( &tmpRow, &tmpColumn );

					if( tmpColumn == 0 )
					{
 						StringCchCopy( buffer, 256, m_pSummonListList->GetText( tmpRow, 0 ));
						
						TCHAR* except =	pclClient->pclMapManager->GetMapName(0);

 						if( _tcscmp( buffer, except  ) == 0 ) break;


						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg2->ShowMapInfoFromOtherInfo(buffer);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_HUNT);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );
					}
				}
				break;
			}
		}
		break;
		//찾기 기능
	case NMONSTERINFODLG_EDIT_NAME:
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
	case NMONSTERINFODLG_LISTBOX_AUTOBOX:
		{
			switch(nEvent) {
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
	case NMONSTERINFODLG_BUTTON_ACCEPT:
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
	//case WM_PAINT:
	//	{
	//		m_siSelectedIndex = (SI16)SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETCURSEL, 0, 0 );
	//		if ( m_siSelectedIndex > -1 )
	//		{
	//			TCHAR strMonsterName[ 128 ] = TEXT("");
	//			SendMessage( GetDlgItem( hDlg, IDC_COMBO_MONSTER_SELECT ), CB_GETLBTEXT, m_siSelectedIndex, (LPARAM) strMonsterName );
	//			ShowMonsterInfo(hDlg, strMonsterName);
	//		}
	//	}
	//	return FALSE;

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}
	return ;

}

/*
bool CNMonsterInfoDlg::IsShow()
{
	return m_bShow;
}

void CNMonsterInfoDlg::Show()
{
	if ( m_bShow )	{	Hide();	return;	}

	switch(m_siSort)
	{
	case 0: 
		m_pNameSortRadioButton->SetCheck( true,	CONTROLLER_KIND_RADIOBUTTON );
		break;
	case 1: 
		m_pLevelSortRadioButton->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON );
		break;
	}

	if ( ! m_bComboBoxInit )
	{
		if ( AddCombo() )
			m_bComboBoxInit = true;
	}
	m_bShow = true;

	CDialogController::Show( true );

	return;
}

void CNMonsterInfoDlg::Hide()
{
	m_bShow = false;

	CDialogController::Show( false );

	return;
}
*/

void CNMonsterInfoDlg::SortName(SI32 comboindex[])
{
	SI32 i;
	SI32 index = 0;

	SI32 tempbuf[ MAX_KIND_NUMBER ];

	for( i = 0; i < MAX_KIND_NUMBER; ++i )
	{
		tempbuf[ i ] = i;
	}

	for(SI32 j = 0; j < MAX_KIND_NUMBER  - 1; j++)
	{
		for(i = j + 1; i < MAX_KIND_NUMBER; i++)
		{
			int cmpindex1 = tempbuf[ j ];
			int cmpindex2 = tempbuf[ i ];

			if(_tcscmp(m_szSortName[cmpindex1], m_szSortName[cmpindex2]) > 0 )
			{
				int temp = tempbuf[ j ];
				tempbuf[ j ] = tempbuf[ i ];
				tempbuf[ i ] = temp;
			}
		}

	}

	for ( i = 0 ; i < MAX_KIND_NUMBER ; i ++ )
	{
		comboindex[index] = tempbuf[i];
		index++;
	}
}

bool CNMonsterInfoDlg::AddCombo()
{
	m_pMonsterSelectComboBox->Clear();

	cltKindInfo* pclki = NULL;

	// 이름 순 정렬
	if ( m_siSort == 0 )
	{
		SI32 sortindex = 0 ;
		SI32 comboIndex[MAX_KIND_NUMBER] ;

		for ( SI32 i = 0; i < MAX_KIND_NUMBER ; ++i )
		{
			StringCchCopy(m_szSortName[i], MAX_KIND_NAME_LENGTH, TEXT(""));
			comboIndex[i] = -1 ;

			pclki = pclClient->pclKindInfoSet->pclKI[ i ];

			if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
				continue;

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

			if ( ConstServiceArea_China == pclClient->siServiceArea )
			{
				if ( pclki->siSortLevel <= 0 || pclki->siSortLevel >= MAX_SORT_LEVEL ) continue ;
			}

			StringCchCopy(m_szSortName[i], MAX_KIND_NAME_LENGTH, (TCHAR*)pclki->GetName());
		}

		SortName(comboIndex);

		for ( i = 0; i < MAX_KIND_NUMBER ; ++i )
		{
			if ( comboIndex[i] < 0 ) continue ;
			if ( !_tcscmp(m_szSortName[comboIndex[i]],TEXT("")) ) continue;

			stComboBoxItemData ComboBoxItemData;
			ComboBoxItemData.Init();
			ComboBoxItemData.Set( m_szSortName[comboIndex[i]] );

			m_pMonsterSelectComboBox->AddItem( &ComboBoxItemData );
			sortindex ++ ;
		}
	}
	else // 등급순 정렬
	{
		for ( SI32 j = 1 ; j < MAX_SORT_LEVEL ; j ++ )
		{
			for ( SI32 i = 10; i < MAX_KIND_NUMBER ; ++i )
			{
				pclki = pclClient->pclKindInfoSet->pclKI[ i ];

				if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
					continue;

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


				if ( pclki->siSortLevel <= 0 || pclki->siSortLevel >= MAX_SORT_LEVEL ) continue ;

				if ( pclki->siSortLevel == j )
				{
					TCHAR* pname = (TCHAR*)pclki->GetName();

					stComboBoxItemData ComboBoxItemData;
					ComboBoxItemData.Init();
					ComboBoxItemData.Set( pname );

					m_pMonsterSelectComboBox->AddItem( &ComboBoxItemData );

					break ;
				}
			}
		}
	}

	return TRUE ;
}

void CNMonsterInfoDlg::ShowMonsterInfo_Internal( TCHAR* szSelText )
{
	//============================================
	// 얻어온 Monster이름으로부터 ATTACK INFO를출력한다.
	//============================================
	SI32 siMonsterKind = pclClient->pclKindInfoSet->FindKindFromName( szSelText );
	TCHAR Buffer[ 512 ] = TEXT("");
	stListBoxItemData tmpListBoxItemData;

	pclClient->pclKindInfoSet->GetCharAttackInfo( siMonsterKind, Buffer, 512 );
	m_pMonsterInfoEdit->SetText( Buffer );
	
	//============================================
	// PORTRAIT를 출력한다.
	//============================================
	TCHAR strFileName[ 128 ] = TEXT("");

	//sprintf( strFileName, TEXT("Interface/Portrait/%s.bmp"),	pclClient->pclKindInfoSet->pclKI[ siMonsterKind ]->szCode );

	SI16 tmpSelectedIndex = pclClient->pclKindInfoSet->FindKindFromName( szSelText );
	TCHAR szPortrait[256];
	SI32 siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(tmpSelectedIndex, szPortrait, sizeof(szPortrait), &siPortraitFont);
	m_pMonsterPortraitImage->SetFileName( szPortrait );
	m_pMonsterPortraitImage->SetFontIndex( siPortraitFont );

	//============================================
	// 몬스터로부터 얻어오는 아이템 정보를 설정한다.
	//============================================
	cltDropItem clDropItem[50];
	SI32		siDropCount = 0;
	if(pclClient->pclKindInfoSet->GetItemFromKindExplain(siMonsterKind, clDropItem, &siDropCount) == true)
	{				
		if(m_pItemListList) 
		{
			// 리스트를 모두 지운다. 
			m_pItemListList->Clear();
						
			SI32 siSortIndex = 0;
			for(SI32 i = 0 ; i < siDropCount; ++i)
			{
				tmpListBoxItemData.Init();				
				tmpListBoxItemData.Set( 0, clDropItem[i].szName);
				tmpListBoxItemData.Set( 1, clDropItem[i].szItemNum);
				tmpListBoxItemData.Set( 2, clDropItem[i].szPercent);
                
				//컬럼 단위로 추가
				m_pItemListList->AddItem( &tmpListBoxItemData );
			}
		}
	}
	else
	{
		if(m_pItemListList) 
		{
			// 리스트를 모두 지운다. 
			m_pItemListList->Clear();
		}
	}

	m_pItemListList->Refresh();

	
	//============================================
	// 무기별 내성도을 출력한다.
	//============================================
	/*
	cltAtkTypeTolerance * pclTolerance = &pclClient->pclKindInfoSet->pclKI[siMonsterKind]->clAtkTypeTolerance ;
	if(m_pToleranceListList && pclTolerance )
	{
		TCHAR buffer[256] = TEXT("") ;
		// 리스트를 모두 지운다. 
		m_pToleranceListList->Clear();
	
		TCHAR* pTitle = GetTxtFromMgr(2366);
		TCHAR* pText = GetTxtFromMgr(2367);

		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pTitle );
		wsprintf(buffer,pText,pclTolerance->siSwordTolerance);
		tmpListBoxItemData.Set( 1, buffer );
		m_pToleranceListList->AddItem( &tmpListBoxItemData );

		pTitle = GetTxtFromMgr(2368);
		pText = GetTxtFromMgr(2369);
		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pTitle );
		wsprintf(buffer,pText,pclTolerance->siSpearTolerance);
		tmpListBoxItemData.Set( 1, buffer );
		m_pToleranceListList->AddItem( &tmpListBoxItemData );

		pTitle = GetTxtFromMgr(2370);
		pText = GetTxtFromMgr(2371);
		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pTitle );
		wsprintf(buffer,pText,pclTolerance->siAxeTolerance);
		tmpListBoxItemData.Set( 1, buffer );
		m_pToleranceListList->AddItem( &tmpListBoxItemData );

		pTitle = GetTxtFromMgr(2372);
		pText = GetTxtFromMgr(2373);
		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pTitle );
		wsprintf(buffer,pText,pclTolerance->siBowTolerance);
		tmpListBoxItemData.Set( 1, buffer );
		m_pToleranceListList->AddItem( &tmpListBoxItemData );

		pTitle = GetTxtFromMgr(2374);
		pText = GetTxtFromMgr(2375);
		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pTitle );
		wsprintf(buffer,pText,pclTolerance->siGunTolerance);
		tmpListBoxItemData.Set( 1, buffer );
		m_pToleranceListList->AddItem( &tmpListBoxItemData );

		pTitle = GetTxtFromMgr(2376);
		pText = GetTxtFromMgr(2377);
		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pTitle );
		wsprintf(buffer,pText,pclTolerance->siStaffTolerance);
		tmpListBoxItemData.Set( 1, buffer );
		m_pToleranceListList->AddItem( &tmpListBoxItemData );

		pTitle = GetTxtFromMgr(2378);
		pText = GetTxtFromMgr(2379);
		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set( 0, pTitle );
		wsprintf(buffer,pText,pclTolerance->siCannonTolerance);
		tmpListBoxItemData.Set( 1, buffer );
		m_pToleranceListList->AddItem( &tmpListBoxItemData );
	}

	m_pToleranceListList->Refresh();
	*/
	//============================================
	// 출몰위치정보를 출력한다.
	//============================================
	cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;
	cltRegenPos	clRegenPos[100];
	SI32	siRegenCount = 0;
	if(pclnpcmanager->GetKindPos(siMonsterKind , clRegenPos, &siRegenCount) == true)
	{	
		if(m_pSummonListList) 
		{
			// 리스트를 모두 지운다. 
			m_pSummonListList->Clear();
		
			SI32 siSortIndex = 0;
			for(SI32 i = 0 ; i < siRegenCount; ++i)
			{
				tmpListBoxItemData.Init();
				tmpListBoxItemData.Set( 0, clRegenPos[i].szMapName );
				tmpListBoxItemData.Set( 1, clRegenPos[i].szMapPos );
				tmpListBoxItemData.Set( 2, clRegenPos[i].szNearMapInfo );
				m_pSummonListList->AddItem( &tmpListBoxItemData );
			}			
		}
	}
	else
	{
		if(m_pSummonListList) 
		{
			// 리스트뷰를 모두 지운다. 
			m_pSummonListList->Clear();
		}
	}

	m_pSummonListList->Refresh();


	// 해당 몬스터를 검색하는 퀘스트 진행중이 었나 체크

	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_SEARCHMONSTERINFO;
	if(pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true
		&& para1 == siMonsterKind )
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		cltClient* pclclient = (cltClient* )pclClient;
		pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
	}

}


void CNMonsterInfoDlg::ShowMonsterInfo( TCHAR* szSelText )
{
	/*
	 //콤보로 몬스터를 선택하지 않고 외부 창에서 호출했다면
	 //화면에 떠있지 않다면 Data를 설정한다.
	if(m_bShow == false)
	{
		switch(m_siSort)
		{
		case 0: 
			m_pNameSortRadioButton->SetCheck( true,	CONTROLLER_KIND_RADIOBUTTON );
			break;
		case 1: 
			m_pLevelSortRadioButton->SetCheck( true, CONTROLLER_KIND_RADIOBUTTON );
			break;
		}

		if ( ! m_bComboBoxInit )
		{
			if ( AddCombo() )
				m_bComboBoxInit = true;
		}
		Show();
	}
	*/

	SI16 tmpFindIndex = m_pMonsterSelectComboBox->FindIndexFromText( szSelText );

	if(tmpFindIndex < 0 ) 
		return;

	m_pMonsterSelectComboBox->SetCursel( tmpFindIndex );

	ShowMonsterInfo_Internal( szSelText );
}

void CNMonsterInfoDlg::SetData( void *pData )
{
	stNMonsterInfoDlgData *pNMonsterInfoDlgData = (stNMonsterInfoDlgData *)pData;
	if ( pNMonsterInfoDlgData == NULL )
	{
		return;
	}

	switch ( pNMonsterInfoDlgData->siParam )
	{
	case 0:
		{
			ShowMonsterInfo_Internal( pNMonsterInfoDlgData->pData );
		}
		break;
	case 1:
		{
			ShowMonsterInfo( pNMonsterInfoDlgData->pData );
		}
		break;
	}

	return;
}

/*
void CNMonsterInfoDlg::etText( TCHAR *szSelText, SI16 Param1, SI16 Param2 )
{
	switch( Param1 )
	{
	case 0:
		{
			ShowMonsterInfo_Internal( szSelText );
		}
		break;
	case 1:
		{
			ShowMonsterInfo( szSelText );
		}
		break;
	}

	return;
}
*/

//// WM_PAINT 메세지로 그리기 전 호출
//void CNMonsterInfoDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY  )
//{
//}
// WM_PAINT 메세지로 그린 후 호출
//void CNMonsterInfoDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
//{
//	m_siSelectedIndex = m_pMonsterSelectComboBox->GetSelectedIndex();
//
//	if ( m_siSelectedIndex > -1 )
//	{
//		TCHAR strMonsterName[ 128 ] = TEXT("");
//		_tcscpy( strMonsterName, m_pMonsterSelectComboBox->GetText( m_siSelectedIndex ));
//		ShowMonsterInfo( strMonsterName );
//	}
//}
//#endif


void CNMonsterInfoDlg::Show()
{
	CControllerMgr::Show( true );
}
void CNMonsterInfoDlg::Hide()
{
	CControllerMgr::Show( false );
}

//검색기능을 위해
//몬스터 검색
void CNMonsterInfoDlg::MonsterFind(TCHAR* pStrname)
{
	if(pStrname == NULL)			return;
	//아이템이름으로 인덱스를 찾아서
	SI16 FindIndex = m_pMonsterSelectComboBox->FindIndexFromText(pStrname);
	if( FindIndex < 0 )	return;
	//해당 인덱스를 탑으로 
	m_pMonsterSelectComboBox->SetCursel(FindIndex);
}
void CNMonsterInfoDlg :: Find(TCHAR* pItemname)
{
	if ( strlen(pItemname) == 0) return;// 빈문자열일 경우는 리턴
	Show();
	MonsterFind(pItemname);
	m_pEdit_Name->Clear();
}
void CNMonsterInfoDlg ::Update(){

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

	cltKindInfo* pclki= NULL;
	TCHAR	m_szSortName[MAX_KIND_NUMBER][MAX_KIND_NAME_LENGTH] ;
	for ( SI32 i = 0; i < MAX_KIND_NUMBER ; ++i )
	{
		StringCchCopy(m_szSortName[i], MAX_KIND_NAME_LENGTH, TEXT(""));
		pclki = pclClient->pclKindInfoSet->pclKI[ i ];

		if( pclki == NULL || pclki->IsAtb( ATB_MONSTER ) == false || pclki->IsAtb( ATB_WARMONSTER ) == true ) 
			continue;

		/*switch (pclClient->siServiceArea)
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
		}*/
		if ( ConstServiceArea_China == pclClient->siServiceArea )
		{
			if ( pclki->siSortLevel <= 0 || pclki->siSortLevel >= MAX_SORT_LEVEL ) continue ;
		}
		StringCchCopy(m_szSortName[i], MAX_KIND_NAME_LENGTH, (TCHAR*)pclki->GetName());
		TCHAR* findname = _tcsstr(m_szSortName[i], kInputText);
        if(findname)		AddListItem(m_szSortName[i]);
		
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
}
void CNMonsterInfoDlg :: AddListItem(TCHAR* findname){
	if(findname == NULL)	return;
	SI16 cnt = 0;
	stListBoxItemData Data;

	Data.Init();
	Data.Set( 1, findname );
	m_pListBox_AutoMake->AddItem(&Data);
}
void CNMonsterInfoDlg::Action(  )
{	
	if( false == IsShow() )
		return;

	if( m_timerCount >= LEAST_COMMON_MULTILPLE + 1 )
		m_timerCount = 1;
	m_timerCount++;
	Update();
}
