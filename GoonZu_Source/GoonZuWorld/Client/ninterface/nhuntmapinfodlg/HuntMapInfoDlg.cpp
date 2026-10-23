#include "../Client/NInterface/NHuntMapInfoDlg/HuntMapInfoDlg.h"


#include "../Client/InterfaceMgr/InterfaceMgr.h"

#include "../Client/InterfaceMgr/Interface/Static.h"
#include "../Client/InterfaceMgr/Interface/Edit.h"
#include "../Client/InterfaceMgr/Interface/ListView.h"
#include "../Client/InterfaceMgr/Interface/Button.h"
#include "../Client/InterfaceMgr/Interface/ComboBox.h"
#include "../Client/InterfaceMgr/Interface/ImageStatic.h"
#include "../Client/InterfaceMgr/Interface/List.h"

#include "../Client/InterfaceMgr/InterfaceFile.h"
#include "../Client/InterfaceMgr/InterfaceMgr.h"
#include "../Client/InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../Client/ninterface/ninfodlg/NInfoDlg.h"


#include "../../Client.h"

#include "../Client/ItemInfo/ItemInfo.h"

#include "..\..\Resource.h"
#include "Msg\MsgType-Quest.h"
#include "../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
//#include "../Client/Interface/MonsterInfoDlg//MonsterInfoDlg.h"

#include "CommonLogic.h"

#include "../../lib/WebHTML/WebHTML.h"

#include "../../common/NPCManager/NPC.h"
#include "Char\CharManager\CharManager.h"

#include "MsgType-Person.h"
#include "MsgType-System.h"
#include "Msg/MsgType-ItemMall.h"

#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"
#include "../../Client/Interface/SearchDetailDlg/SearchDetailDlg.h"
//#include "../../Client/Interface/MonsterInfoDlg//MonsterInfoDlg.h"

#include "../../Client/ninterface/nresidentsinfodlg/ResidentsInfoDlg.h"
#include "../../Client/NInterface/NMonsterInfoDlg/NMonsterInfoDlg.h"
#include "../../Client/Interface/SoundOnOffDlg/SoundOnOffDlg.h"

#include "JwLib/IsCorrectID/IsCorrectID.h"
#include "NDataLoader.h"

extern CSoundOnOffDlg g_SoundOnOffDlg;
extern cltCommonLogic* pclClient;

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic* pclClient;
 
CNHuntMapInfoDlg::CNHuntMapInfoDlg() : CDialogController()
{
	m_siCombo = 0 ;

	for ( int i = 0 ; i < MAX_MAP_NUMBER ; i ++ )
	{
		//		m_siMapLevel[i] = 0 ;
		m_siParentMap[i] = 0 ;
		StringCchCopy( m_szMapTypeName[i], MAX_MAP_NAME_SIZE, TEXT(""));
	}

	m_siComboParentMap = 0;
	m_siSprMode = 0;
	m_siSelectGateIndex = 0;
	m_siSelectMapIndex = 0;
	m_siSelectMonsterIndex = 0;
	m_siMapIndex = 0;

	m_pStatic[0] = NULL;
	m_pStatic[1] = NULL;

	m_pComboBox = NULL;

	m_pList[0] = NULL;
	m_pList[1] = NULL;

	m_pButton = NULL;
	m_pEdit = NULL;
	// 검색기능
	m_timerCount = 0;
	m_pListBox_AutoMake=NULL;
	m_pEdit_Name=NULL;
	m_pButton_Accept=NULL;
	m_pStatic_Back	= NULL;//백그라운드
}

CNHuntMapInfoDlg::~CNHuntMapInfoDlg()
{
	if( m_pStatic[0] )
	{
		delete m_pStatic[0];
		m_pStatic[0] = NULL;
	}
	if( m_pStatic[1] )
	{
		delete m_pStatic[1];
		m_pStatic[1] = NULL;
	}

	if( m_pComboBox )
	{
		delete m_pComboBox;
		m_pComboBox = NULL;
	}

	if( m_pList[0] )
	{
		delete m_pList[0];
		m_pList[0] = NULL;
	}
	if( m_pList[1] )
	{
		delete m_pList[1];
		m_pList[1] = NULL;
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

	GP.FreeSpr(m_SprGate);
	GP.FreeSpr(m_SprMap);
}

void CNHuntMapInfoDlg::Create(CControllerMgr * pParent)
{
	//	cltClient *pclclient = (cltClient *)pclClient;
	TCHAR* pText= GetTxtFromMgr(3296);

	CInterfaceFile file;
	if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
		file.LoadFile(TEXT("NInterface/Data/NHuntmapInfo/DLG_HuntmapInfo.ddf"));
	}
	else{
		file.LoadFile(TEXT("NInterface/Data/NHuntmapInfo/DLG_HuntmapInfo_J.ddf"));
	}
	file.CreateChildDialog( this, NHUNTMAPINFO_DLG,TEXT("dialog_HuntmapInfo"),StaticCallBackDialogNHuntMapInfoDlg,pParent);

	// 검색기능
	m_pListBox_AutoMake = new CList(this);
	m_pEdit_Name=new CEdit(this);
	m_pButton_Accept = new CButton(this);
	m_pStatic_Back = new CStatic( this );	// 백그라운드
	if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
	// 검색기능
	file.CreateControl( m_pEdit_Name, NITEMINFO_EDIT_NAME, TEXT("editbox_name") );
	file.CreateControl( m_pListBox_AutoMake, NITEMINFO_LISTBOX_AUTOBOX , TEXT("listbox_automake") );
	file.CreateControl( m_pButton_Accept, NITEMINFO_BUTTON_ACCEPT, TEXT("button_accept") );
	file.CreateControl( m_pStatic_Back, NITEMINFO_DIALOG_CSTATICBACK,   TEXT("statictext_Back")); // 백그라운드 
	//---------------------------------------------------------------------------------------------------------
	}
	//CDialogController::Create( NHUNTMAPINFO_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNHuntMapInfoDlg, pText, TEXT("NInterface/Interface/Interface_20X20_00_109.Spr"), 9, true, 9, 80, 135, 380, 495, true, false, true );
	//	m_pImage = new CImageStatic( this );
	//	m_pImage->Create( NITEM_DIALOG_IMAGESTATIC, NULL, 0, false, 0, 0, 18, 370, 495, 370, 495 );
	m_pStatic[0] = new CStatic( this );
	
	//m_pStatic[0]->Create( NHUNTMAPINFO_DIALOG_POSITIONINFO_STATIC, NULL, NULL, 0, false, 0, 5, 25, 100, 17, 100, 17 );
	//pText= GetTxtFromMgr(3297);
	//m_pStatic[0]->SetText( pText, DT_LEFT|DT_VCENTER|DT_SINGLELINE );
		
	file.CreateControl( m_pStatic[0] , NHUNTMAPINFO_DIALOG_POSITIONINFO_STATIC ,   TEXT("statictext_NONAME1")); 
	

	m_pStatic[1] = new CStatic( this );
//	m_pStatic[1]->Create( NHUNTMAPINFO_DIALOG_MONSTER_POSITIONINFO_STATIC, NULL, NULL, 0, false, 0, 5, 318, 100, 17, 100, 17 );
//	pText= GetTxtFromMgr(3298);
//	m_pStatic[1]->SetText( pText, DT_LEFT|DT_VCENTER|DT_SINGLELINE );

	file.CreateControl( m_pStatic[1] , NHUNTMAPINFO_DIALOG_MONSTER_POSITIONINFO_STATIC , TEXT("statictext_NONAME3")); 


	m_pList[0] = new CList( this );
	//m_pList[0]->Create( NHUNTMAPINFO_DIALOG_LIST1, NULL, 0, false, 0, 5, 335, 364, 112, 364, 112, true, false, true, true );
	//m_pList[0]->SetFontSize( 12 );
	//m_pList[0]->SetFontWeight( 0 );
	
	file.CreateControl( m_pList[0], NHUNTMAPINFO_DIALOG_LIST1, TEXT("listbox_NONAME2"));
	pText= GetTxtFromMgr(3299);
	m_pList[0]->SetColumn(0, 173, pText);
	pText= GetTxtFromMgr(3300);
	m_pList[0]->SetColumn(1, 173, pText);
	m_pList[0]->SetColumn(2, 108, TEXT(""));
	m_pList[0]->SetBorder(TRUE);
	m_pList[0]->SetBKColorUse(TRUE);
	m_pList[0]->Refresh();

	m_pList[1] = new CList( this );
	//m_pList[1]->Create( NHUNTMAPINFO_DIALOG_LIST2, NULL, 0, false, 0, 240, 47, 130, 70, 130, 70, true, false, true, false );
	//m_pList[1]->SetFontSize( 12 );
	//m_pList[1]->SetFontWeight( 0 );

	file.CreateControl( m_pList[1], NHUNTMAPINFO_DIALOG_LIST2, TEXT("listbox_NONAME1"));

	pText= GetTxtFromMgr(3301);
	m_pList[1]->SetColumn(0, 147, pText);
	m_pList[1]->SetBorder(TRUE);
	m_pList[1]->SetBKColorUse(TRUE);
	m_pList[1]->Refresh();


	m_pButton = new CButton( this );
	//m_pButton[0]->Create( NHUNTMAPINFO_DIALOG_MAPMOVE, NULL, TEXT("NInterface/Button/Button_4x24_00_103.SPR"), 0, true, 0, 245, 120, 120, 24, 120, 24 );
	//pText= GetTxtFromMgr(3302);
	//m_pButton[0]->SetText(pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	file.CreateControl( m_pButton, NHUNTMAPINFO_DIALOG_MAPMOVE, TEXT("button_NONAME1"));


	m_pEdit = new CEdit( this );
	//m_pEdit->Create( NHUNTMAPINFO_DIALOG_EDIT, NULL, TEXT("NInterface/Common/Common_16X15_00_109.Spr"), 0, true, 9, 5, 47, 230, 100, 230, 100, false, false );
	
	file.CreateControl( m_pEdit, NHUNTMAPINFO_DIALOG_EDIT, TEXT("editbox_NONAME1"));

	m_pEdit->SetFontSize( 12 );
	m_pEdit->SetFontWeight( 0 );
	m_pEdit->SetReadOnly(true);

	m_pComboBox = new CComboBox( this );
//	m_pComboBox->Create( NHUNTMAPINFO_DIALOG_TYPE_COMBOBOX, TEXT("NInterface/Common/Common_4X17_00_103.Spr"), 0, true, 3, 120, 25, 200, 17, 200, 367, 350 );
	file.CreateControl( m_pComboBox, NHUNTMAPINFO_DIALOG_TYPE_COMBOBOX, TEXT("combobox_NONAME1"));

	// 파일에서 사냥터 정보 불러오는 함수로 대체
	AddHuntmaps();

	m_pComboBox->SetCursel(0);
	m_siSelectGateIndex = m_pComboBox->GetSelectedIndex();
	ShowListLow(m_siSelectGateIndex, m_pComboBox->GetText(m_siSelectGateIndex));
	m_siSprMode = 0;

	m_siSelectMapIndex = 0;
	ShowMapInfo(m_pList[1]->GetText(m_siSelectMapIndex, 0));

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

	m_pEdit->SetText("");
	m_pEdit->Refresh();
	m_pList[0]->Clear();
	m_pList[0]->Refresh();
	Hide();
	
}


void CALLBACK CNHuntMapInfoDlg::StaticCallBackDialogNHuntMapInfoDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNHuntMapInfoDlg *pThis = (CNHuntMapInfoDlg *)pControllerMgr;
	pThis->CallBackDialogNHuntMapInfoDlg( nEvent, nControlID, pControl );
}

void CALLBACK CNHuntMapInfoDlg::CallBackDialogNHuntMapInfoDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			Hide();
		}
		break;
	case NHUNTMAPINFO_DIALOG_TYPE_COMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					m_siSelectGateIndex = m_pComboBox->GetSelectedIndex();

					if(m_siSelectGateIndex < 0)
						return;

					m_siSprMode = 0;
					m_pList[1]->Clear();
					ShowListLow(m_siSelectGateIndex, m_pComboBox->GetText(m_siSelectGateIndex));

					// 위치 및 몬스터 정보 초기화
					m_pEdit->SetText("");
					m_pList[0]->Clear();
					m_pList[0]->Refresh();

					m_siSelectMapIndex = 0;
				}
				break;
			}
		}
		break;
	case NHUNTMAPINFO_DIALOG_LIST1:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{ 
 					if(GetParentDialog()->GetDialogControllerID() == NINFO_SUB_DLG)
						return;

 					m_siSelectMonsterIndex = m_pList[0]->GetSelectedIndex();

					if ( m_siSelectMonsterIndex < 0 ) break ;

					if(m_siSelectGateIndex < 0)
					{
						break;
					}
					/////// 몬스터와 npc를 구분한다.
					
					bool bIsMonster = true;

					SI32 Kind = pclClient->pclKindInfoSet->FindKindFromName(m_pList[0]->GetText(m_siSelectMonsterIndex, 0));
					
					if( Kind <= 0 ) break;
					
					if( pclClient->pclKindInfoSet->pclKI[Kind] )
					{	
						if(pclClient->pclKindInfoSet->pclKI[Kind]->IsAtb(ATB_SYSTEMNPC))
							bIsMonster = false;
					}
					else
					{
						break;
					}


					if(bIsMonster)
					{
						stNMonsterInfoDlgData NMonsterInfoDlgData;
						NMonsterInfoDlgData.pData = m_pList[0]->GetText(m_siSelectMonsterIndex, 0);
						NMonsterInfoDlgData.siParam = 1;
						//pclclient->m_pDialog[ NMONSTERINFO_DLG ]->SetData( &NMonsterInfoDlgData );
						//((CNMonsterInfoDlg*)pclclient->m_pDialog[ NMONSTERINFO_DLG ])->ShowMonsterInfo(m_pList[0]->GetText(m_siSelectMonsterIndex, 0));
						//	g_MonsterInfoDlg.ShowMonsterInfo(m_pList[0]->GetText(m_siSelectMonsterIndex, 0));
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg1->ShowMonsterInfo(m_pList[0]->GetText(m_siSelectMonsterIndex, 0));
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_MONSTER);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );
					}
					else
					{
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->m_pTabDlg4->ShowResidentInfoByKind(Kind, m_pList[0]->GetText(m_siSelectMonsterIndex, 0));
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->Show();
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->SelectTab(NINFODLG_BUTTON_NPC);
						((cltNInfoDlg*)pclclient->m_pDialog[NINFO_SUB_DLG])->MovePos( 
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetX() + ((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetWidth() ,
							((cltNInfoDlg*)pclclient->m_pDialog[NINFO_DLG])->GetY() );
					}
				
					
				}
				break;	
			}
		}
		break;
	case NHUNTMAPINFO_DIALOG_LIST2:	// 사냥터 이름
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					m_siSelectMapIndex = m_pList[1]->GetSelectedIndex();

					if(m_siSelectMapIndex < 0)
						break;
					m_siSprMode = 1;

					ShowMapInfo(m_pList[1]->GetText(m_siSelectMapIndex, 0));
				}
				break;
			}
		}
		break;
	case NHUNTMAPINFO_DIALOG_MAPMOVE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					if ( m_pComboBox->GetSelectedIndex() < 0)		return;
					if ( m_siSelectMapIndex < 0 )					return;

					// pk 사냥터는 이동 불가능
					if(pclClient->IsCountrySwitch( Switch_PKHuntMap_Change_Info ))
					{
						if(pclClient->pclMapManager->IsPkMap(m_siSelectMapIndex) == true )	return;
					}

					// 워프 할 수 없는 상황이다.
					if( pclclient->pclCM->CR[1]->CanWarpFrom() == false )	return;

					TCHAR buffer[256] = TEXT("") ;
					SI32 mapindex = -1;
					bool bInMap = false;
					TCHAR* szSelectMapName = NULL;

					// 선택된 리스트 아이템 인덱스를 얻어온다.
					SI16 siSelectedListIndex = -1;

					if(pclClient->IsCountrySwitch( Switch_WarpInHuntMap ))
					{
						siSelectedListIndex = m_pList[1]->GetSelectedIndex();
					}
					else
					{
						siSelectedListIndex = -1;
					}

					if(siSelectedListIndex < 0)
					{
						bInMap = false;

						TCHAR *pszMapName = m_pList[1]->GetText(0, 0);
						if(pszMapName == NULL)		break;	// case 문 탈출

						mapindex = pclclient->pclMapManager->GetMapIndexFromName(pszMapName);
						// pk 사냥터는 이동 불가능
						if(pclClient->IsCountrySwitch( Switch_PKHuntMap_Change_Info ))
						{
							if(pclClient->pclMapManager->IsPkMap(mapindex) == true )	break;	// case 문 탈출
						}
						mapindex = pclClient->pclMapManager->GetParentMap( mapindex );
						if(mapindex < 0)			break;	// case 문 탈출
						// pk 사냥터는 이동 불가능
						if(pclClient->IsCountrySwitch( Switch_PKHuntMap_Change_Info ))
						{
							if(pclClient->pclMapManager->IsPkMap(mapindex) == true )	break;	// case 문 탈출
						}

						szSelectMapName = m_pComboBox->GetText( m_pComboBox->GetSelectedIndex() );
					}
					else
					{
						bInMap = true;
						mapindex = pclclient->pclMapManager->GetMapIndexFromName(m_pList[1]->GetText(m_siSelectMapIndex, 0));

						// pk 사냥터는 이동 불가능
						if(pclClient->IsCountrySwitch( Switch_PKHuntMap_Change_Info ))
						{
							if(pclClient->pclMapManager->IsPkMap(mapindex) == true )	return;
						}

						szSelectMapName = m_pList[1]->GetText(m_siSelectMapIndex, 0);
					}

					if ( mapindex <= 0 || mapindex > MAX_MAP_NUMBER ) return;
					// pk 사냥터는 이동 불가능
					if(pclClient->IsCountrySwitch( Switch_PKHuntMap_Change_Info ))
					{
						if(pclClient->pclMapManager->IsPkMap(mapindex) == true )	return;
					}
					TCHAR* pText= GetTxtFromMgr(3326);

					TCHAR* pTitle = NULL ;
					TCHAR* PText = NULL;

					// 무료이동 체크
					if( pclClient->pclCM->CR[1]->CanFreeWarp( mapindex ) == false )
					{
						SI32 itemunique = ITEMUNIQUE(24000);  // 순간이동이용권   14005 -> 24000
						if ( pclclient->pclCM->CR[1]->pclCI->clCharItem.GetItemNum(itemunique) <= 0 )
						{
							pText= GetTxtFromMgr(3756);
							StringCchPrintf(buffer, 256, pText);
							pText= GetTxtFromMgr(3326);
							pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pText, buffer );
							return;
						}

						pTitle = GetTxtFromMgr(3326);
						pText  = GetTxtFromMgr(3332);
						StringCchPrintf(buffer,256, pText,szSelectMapName);
					}
					else
					{
						// 무료이동
						pTitle = GetTxtFromMgr(3189);
						pText = GetTxtFromMgr(3760);

						StringCchPrintf(buffer, 256, pText, szSelectMapName);
					}

					stMsgBox MsgBox;
					MsgBox.Set( this->GetParentDialog(), pTitle, buffer, MSGBOX_TYPE_YESNO, 3 );
					BYTE TempBuffer[ 128 ];
					SI16 Size = 0;
					memcpy( &TempBuffer[Size], &mapindex, sizeof(mapindex) );		Size += sizeof(mapindex);
					memcpy( &TempBuffer[Size], &bInMap, sizeof(bInMap) );			Size += sizeof(bInMap);

					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				}
				break;
			}
		}
		break;

	case NHUNTMAPINFO_DIALOG_CLOSE:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					Hide();
				}
				break;
			}
		}
		break;
		//찾기 기능
	case NITEMINFO_EDIT_NAME:
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
	case NITEMINFO_LISTBOX_AUTOBOX:
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
	case NITEMINFO_BUTTON_ACCEPT:
		{
			switch(nEvent) {
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
 
void CNHuntMapInfoDlg::Hide()
{
	GP.FreeSpr(m_SprGate);
	GP.FreeSpr(m_SprMap);
	CControllerMgr::Show( false );

	return;
}

void CNHuntMapInfoDlg::Show()
{
	GP.LoadSprFromFile(TEXT("HuntMapInfo/gateway.SPR"), &m_SprGate);
	GP.LoadSprFromFile(TEXT("HuntMapInfo/huntmapinfo.SPR"), &m_SprMap);
	if(CDialogController::IsShow())
	{	return;	}

	CControllerMgr::Show( true );

	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_HUNTMAPINFODLG);

	return;
}

void CNHuntMapInfoDlg::Action()
{	// 자동완성
	if( false == IsShow() )	return;
	if( m_timerCount >= LEAST_COMMON_MULTILPLE + 1 )
		m_timerCount = 1;
	m_timerCount++;
	Update();


}

void CNHuntMapInfoDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
 	if(0 == m_siSprMode)
	{
 		if(m_siSelectGateIndex < 0) 
			return;

		if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
		GP.PutSpr(&m_SprGate, ScreenRX + GetX() + 10 , ScreenRY + GetY() + 225, m_siSelectGateIndex);
		}
		else
		{
			GP.PutSpr(&m_SprGate, ScreenRX + GetX() + 10 , ScreenRY + GetY() + 195, m_siSelectGateIndex);
		}
		//기본 195
	}
	else if(1 == m_siSprMode)
	{
		if(m_siMapIndex < 0) 
			return;

		if(pclClient->IsCountrySwitch(Switch_FindInfo)) {
			GP.PutSpr(&m_SprMap, ScreenRX + GetX() + 10 , ScreenRY + GetY() + 225, m_siMapIndex);
		}
		else{
			GP.PutSpr(&m_SprMap, ScreenRX + GetX() + 10 , ScreenRY + GetY() + 195, m_siMapIndex);
		}
		//기본 195
	}

}

void CNHuntMapInfoDlg::AddCombo(TCHAR * szMapType,TCHAR * maptypename,SI32 parentmap)
{
	m_siParentMap[m_siCombo] = parentmap ;
	StringCchCopy( m_szMapTypeName[m_siCombo] , MAX_MAP_NAME_SIZE, maptypename ) ;
	m_siCombo ++ ;

	stComboBoxItemData ComboBoxItemData;
	ComboBoxItemData.Init();
	ComboBoxItemData.Set( szMapType );					

	m_pComboBox->AddItem( &ComboBoxItemData );
}

void CNHuntMapInfoDlg::ShowListLow(SI32 index,TCHAR * szMapName)
{
	cltClient *pclclient = (cltClient*)pclClient;

	if ( index < 0 ) return ;
	if ( szMapName == NULL || !_tcscmp(szMapName,TEXT(""))) return ;

	TCHAR buffer[256] = TEXT("");

	m_siComboParentMap = m_siParentMap[index] ;

	SI32 listindex = 0 ;

	m_pList[1]->Clear();

	for(SI32 i = 0;i < MAX_MAP_NUMBER ; i++)
	{
		if ( pclclient->pclMapManager->GetParentMap(i) == m_siComboParentMap )
		{
			StringCchCopy(buffer, 256, pclclient->pclMapManager->GetMapName(i));
			if ( !_tcscmp(buffer,TEXT("NoName")) ) continue ;
			if ( !_tcscmp(buffer,TEXT(""))) continue;
			if (pclClient->pclVillageManager->GetMapIndexFromName(buffer) > 0)
				continue;

			stListBoxItemData Data;

			Data.Set( 0, buffer );

			m_pList[1]->AddItem( &Data );
		}
	}

	TCHAR tempbuffer[256] = TEXT("");
	SI32 j;
	for(i = 0; i < m_pList[1]->GetListNum() - 1; ++i)
	{
		for( j = 1; j < m_pList[1]->GetListNum() - i; ++j)
		{
			if( _tcscmp(m_pList[1]->GetText(j-1,0), m_pList[1]->GetText(j,0)) > 0 )
			{
				StringCchCopy(tempbuffer, 256, m_pList[1]->GetText(j-1,0));
				MStrCpy(m_pList[1]->GetText(j-1,0), m_pList[1]->GetText(j,0), MAX_LISTBOXITEM_TEXT_LEN);
				MStrCpy(m_pList[1]->GetText(j,0), tempbuffer, MAX_LISTBOXITEM_TEXT_LEN);
			}
		}
	}
	m_pList[1]->Refresh();
}

void CNHuntMapInfoDlg::ShowMapInfo(TCHAR * szMapName)
{
	if(szMapName == NULL)			return;

	cltClient *pclclient = (cltClient*)pclClient;
	cltNPCManagerClient* pclnpcmanager = (cltNPCManagerClient*)pclClient->pclNPCManager;

	// 해당맵에 나오는 NPC정보
	SI32 siKind[MAX_MAP_NPCNUMBER] ;
	SI32 siKindLevel[MAX_MAP_NPCNUMBER] ;

	TCHAR strdir[128] = {'\0'};
	TCHAR buffer[256] = {'\0'};

	SI32 mapindex = 0 ;

	for ( SI32 i = 0 ; i < MAX_MAP_NUMBER ; i ++ )
	{
		//if ( !_tcscmp(pclclient->pclMapManager->GetMapName(i),szMapName))
		if ( !strcmp(pclclient->pclMapManager->GetMapName(i), szMapName))
		{
			mapindex = i ;
			break ;
		}
	}

	for ( i = 0 ; i < MAX_MAP_NPCNUMBER ; i ++ )
	{
		siKind[i] = 0 ;
		siKindLevel[i] = 0 ;
	}
	pclnpcmanager->GetKindMapInx(mapindex, siKind, siKindLevel, 25);

	ShowListMonster(siKind,siKindLevel);

	SI32 parentmap = pclclient->pclMapManager->GetParentMap(mapindex);
	SI32 siX = 0;
	SI32 siY = 0;

	pclclient->pclMapManager->pclGateManager->FindMainFieldPos( parentmap, &siX, &siY );

	pclclient->GetNearMapInfo(0, siX, siY ,strdir, 128);

	SI32 mapunique = pclclient->pclMapManager->GetMapUnique(mapindex);

	if ( mapunique <= 0 ) return ;

	m_siMapIndex = mapunique - 1;

	TCHAR * szMapTypeName = pclclient->pclMapManager->pclMapTypeManager->GetMapTypeNameFromUnique(mapunique);

	TCHAR* pText= GetTxtFromMgr(3229);
	if(_tcsstr(szMapName, pText))
	{
		pText= GetTxtFromMgr(3333);
		StringCchPrintf(buffer, 1024, pText,strdir);
		m_pEdit->SetText(buffer);
		return ;
	}
	pText= GetTxtFromMgr(3334);
	StringCchPrintf(buffer, 1024, pText,
		strdir,pclclient->pclMapManager->GetLevelLimit(mapindex));
	m_pEdit->SetText(buffer);
}

void CNHuntMapInfoDlg::ShowListMonster(SI32 siKind[],SI32 siLevel[])
{
	TCHAR buffer[256] = TEXT("") ;

	m_pList[0]->Clear();

	SI32 listindex = 0 ;

	stListBoxItemData Data;

	for( SI32 i = 0 ; i < 25 ; i++ )
	{
		if(( siKind[i] <= 0 )||( siLevel[i] <= 0 )) 
		{
			m_pList[0]->Refresh();
			return ;
		}

		Data.Set(0, (TCHAR*)pclClient->pclKindInfoSet->GetName(siKind[i]));

		StringCchPrintf(buffer, 256, TEXT("Lv. %d"),siLevel[i] ) ;
		Data.Set(1, buffer);

		m_pList[0]->AddItem(&Data);
		m_pList[0]->Refresh();
	}
}

void CNHuntMapInfoDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltClient *pclclient = (cltClient*)pclClient;

	switch( MsgIndex )
	{
	case 3:
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}
			if ( RetValue )
			{
				//KHY - 0703 - 게임 머니로 귀환하기.클레스 수정.
				SI32 siDataPtr = 0;
				SI32 siMapIndex = *(SI32*)&pData[siDataPtr];		siDataPtr+= sizeof(siMapIndex);
				bool bInMap		= *(bool*)&pData[siDataPtr];		siDataPtr+= sizeof(bInMap);

				cltGameMsgRequest_HuntMapWarp pclInfo(siMapIndex,0, bInMap);
				cltMsg clMsg( GAMEMSG_REQUEST_HUNTMAPWARP, sizeof( pclInfo ), (BYTE*)&pclInfo );
				pclclient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

void CNHuntMapInfoDlg::AddHuntmaps()
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/HuntmapInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/HuntmapInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile HuntmapInfo"),TEXT("Error"));
		return;
	}

	TCHAR maptypename[256] = TEXT(""), mapunique[256] = TEXT("");
	SI32 ptextnum, mapinx, imapunique;

	NDataTypeInfo pDataTypeInfo[] =
	{
			NDATA_INT32,	&ptextnum,   4,		
			NDATA_MBSTRING,	mapunique,	256,
			NDATA_INT32,	&mapinx,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );


	while( !dataloader.IsEndOfData() ) {
		
		if( dataloader.ReadData() ) 
		{
			if ( mapunique[ 0 ] != NULL)
			{
				if (pclClient->pclHashTableString->Find(mapunique, imapunique))
				{
					StringCchPrintf(maptypename, 256, pclClient->pclMapManager->pclMapTypeManager->GetMapTypeNameFromUnique(imapunique));
					AddCombo( GetTxtFromMgr(ptextnum),maptypename,mapinx);


				}
			}
		}
	}

}


void CNHuntMapInfoDlg::ShowMapInfoFromOtherInfo(TCHAR * szMapName)
{
  	m_pList[1]->Clear();
	m_pComboBox->SetText(TEXT(" "));
	
	m_siSprMode = 1;

	stListBoxItemData Data;

	Data.Set( 0, szMapName );

	m_pList[1]->AddItem( &Data );

	ShowMapInfo(szMapName);
	
	m_pList[1]->Refresh();
}
void CNHuntMapInfoDlg:: FindHuntMap(TCHAR * szMapName)
{
	if(szMapName == NULL)			return;
	//인덱스를 가지고 온다.
	m_siSelectGateIndex = m_pComboBox->FindIndexFromText(szMapName);
	if(m_siSelectGateIndex < 0)	return;
	m_pComboBox->SetCursel(m_siSelectGateIndex);
}

void CNHuntMapInfoDlg :: Find(TCHAR* pItemname)
{
	if ( strlen(pItemname) == 0) return;// 빈문자열일 경우는 리턴
	Show();
	FindHuntMap(pItemname);
	m_pEdit_Name->Clear();
}
void CNHuntMapInfoDlg ::Update(){

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
	//---------------------------------------------------------------
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		bLoadFile = dataloader.LoadDataFromCryptFile( TEXT("Data/HuntmapInfo.txt") );
	}
	else
	{
		bLoadFile = dataloader.LoadDataFromFile( TEXT("Data/HuntmapInfo.txt") );
	}

	if (!bLoadFile)
	{
		MsgBox( TEXT("Not LoadFromFile HuntmapInfo"),TEXT("Error"));
		return;
	}

	TCHAR maptypename[256] = TEXT(""), mapunique[256] = TEXT("");
	SI32 ptextnum, mapinx, imapunique;

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32,	&ptextnum,   4,		
			NDATA_MBSTRING,	mapunique,	256,
			NDATA_INT32,	&mapinx,	4,
			0,0,0
	};

	dataloader.RegDataType( pDataTypeInfo );
	while( !dataloader.IsEndOfData() ) {

		if( dataloader.ReadData() ) 
		{
			if ( mapunique[ 0 ] != NULL)
			{
				if (pclClient->pclHashTableString->Find(mapunique, imapunique))
				{
					TCHAR* findname = _tcsstr(GetTxtFromMgr(ptextnum), kInputText);
					if(findname)		AddListItem(GetTxtFromMgr(ptextnum));


				}
			}
		}
	}			
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
void CNHuntMapInfoDlg :: AddListItem(TCHAR* findname){
	if(findname == NULL)	return;
	SI16 cnt = 0;
	stListBoxItemData Data;

	Data.Init();
	Data.Set( 1, findname );
	m_pListBox_AutoMake->AddItem(&Data);
}