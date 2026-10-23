//---------------------------------
// 2004/1/31 김태곤
//---------------------------------
/* ==========================================================================
	클래스 :		CNMasterInfo

	작성일 :		05/03/03
	작성자 :		정용래

	목  적 :		CNMasterInfo 클래스는  전체 화면 모드 실행시 NInterface에
					따라 게임 화면 안에서 장인 정보 다이얼로그를 생성한다.

	용  법 :		

	변경사항 :		없음
	
	차후작업 :		
					1. DisplayHTMLPage(hWnd, ProfileURL); 부분 작업 추가 필요
					2. IDC_BUTTON_DETAILINFO 확인 요망!!

   ========================================================================*/

//#if (defined(__LEON_START_ALL) || defined(__LEON_NMASTER__)) && defined(_DEBUG)

#include "CommonLogic.h"
#include "..\..\Client\Client.h"

#include "NMaster.h"
#include "../../../Common/Skill\Skill-Manager.h"
#include "Char\CharManager\CharManager.h"


#include "..\..\Resource.h"

#include "../../lib/WebHTML/WebHTML.h"

// 귓속말 채팅 관련. 
//#include "..\..\Client\Interface\PrivateChatDlg\PrivateChatBaseInfo.h"
//#include "..\..\Client\Interface\PrivateChatDlg\PrivateChatDlgMgr.h"

#include "..\..\Client\Interface\ErrorMsgDlg\ErrorMsgDlg.h"
#include "..\..\Client\Interface\NotifyMsgDlg\NotifyMsgDlg.h"

#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/interface/ComboBox.h"
#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"

//------------------------------
// CommonLogic
//------------------------------
#include "MsgType-System.h"
#include "MsgType-Person.h"
#include "Msg/MsgType-Web.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../CommonLogic/Msg/MsgType-Quest.h"


extern cltCommonLogic* pclClient;

static SI32 siOldSKillUnique = -1 ;

CNMasterInfo::CNMasterInfo()
{

	bDlgRectInitSwitch	= false;

	siCurrentSkillUnique	= 0;
	m_siSelectedIndex = -1 ;

	m_pMasterSkillStatic = NULL;  
	m_pMasterNameStatic = NULL;  
	m_pMasterGreeting = NULL;  
	m_pMasterSkillComboBox = NULL;
	m_pMasterList = NULL;         
	//m_pProfileStatic = NULL;      
	//m_pIntroStatic = NULL;        
	m_pGGOMZIButton = NULL;       
	m_pMasterChatButton = NULL;   
	m_pCloseButton = NULL;        

	siOldSKillUnique = -1 ;
}


CNMasterInfo::~CNMasterInfo()
{
	if(m_pMasterSkillStatic)
		delete m_pMasterSkillStatic;  
	SAFE_DELETE(m_pMasterNameStatic);
	SAFE_DELETE(m_pMasterGreeting);
	if(m_pMasterSkillComboBox)
		delete m_pMasterSkillComboBox;
	if(m_pMasterList)
		delete m_pMasterList;         
	//if(m_pProfileStatic)
	//	delete m_pProfileStatic;      
	//if(m_pIntroStatic)
	//	delete m_pIntroStatic;        
	if(m_pGGOMZIButton)
		delete m_pGGOMZIButton;       
	if(m_pMasterChatButton)
		delete m_pMasterChatButton;   
	if(m_pCloseButton)
		delete m_pCloseButton;

	SetTimerOnOff( false );

	bDlgRectInitSwitch = true;

	//---------------------------------
	// HTML 을 해제한다.
	//---------------------------------
	//==================================
	//HWND hwnd = GetDlgItem( hDlg, IDC_STATIC_PROFILE );
	//if( hwnd )
	//{
	//	DoPageAction(hwnd, WEBPAGE_STOP);
	//	UnEmbedBrowserObject(hwnd);
	//}

	//hwnd = GetDlgItem( hDlg, IDC_STATIC_INTRO );
	//if( hwnd )
	//{
	//	DoPageAction(hwnd, WEBPAGE_STOP);
	//	UnEmbedBrowserObject(hwnd);
	//}
}


void CNMasterInfo::Create()
{
	if( ! IsCreate() )
	{
		cltClient *pclclient = (cltClient *)pclClient;
		TCHAR* pText= GetTxtFromMgr(3340);

		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/DLG_NMaster.ddf"));
		file.CreatePopupDialog( this, NMASTERINFO_DLG, TEXT("dialog_NMaster"), StaticDlgProc);

		m_pMasterSkillStatic = new CStatic( this );  
		NEWCSTATIC(m_pMasterNameStatic);
		NEWCEDIT(m_pMasterGreeting);
		m_pMasterList = new CList( this );         
		//m_pProfileStatic = new CStatic( this );      
		//m_pIntroStatic = new CStatic( this );        
		m_pGGOMZIButton = new CButton( this );       
		m_pMasterChatButton = new CButton( this );   
		m_pCloseButton = new CButton( this );        
		m_pMasterSkillComboBox = new CComboBox( this );
	    
		file.CreateControl( m_pMasterSkillStatic, NMASTERINFO_DIALOG_MASTERSKILLSTATIC, TEXT("statictext_masterskill"));  
		file.CreateControl( m_pMasterNameStatic, NMASTERINFO_DIALOG_MASTERNAMESTATIC, TEXT("statictext_greeting"));  
		file.CreateControl( m_pMasterGreeting, NMASTERINFO_DIALOG_MASTERGREETING, TEXT("editbox_greeting"));  
		file.CreateControl( m_pMasterList, NMASTERINFO_DIALOG_MASTERLIST, TEXT("listbox_master"));
		file.CreateControl( m_pMasterSkillComboBox, NMASTERINFO_DIALOG_MASTERSKILLCOMBOBOX, TEXT("combobox_masterskill"));

		//file.CreateControl( m_pProfileStatic, NMASTERINFO_DIALOG_PROFILESTATIC, TEXT("statictext_2"));      
		//file.CreateControl( m_pIntroStatic, NMASTERINFO_DIALOG_INTROSTATIC, TEXT("statictext_3"));        
		file.CreateControl( m_pGGOMZIButton, NMASTERINFO_DIALOG_GGOMZIBUTTON, TEXT("button_ggomzi"));       
		file.CreateControl( m_pMasterChatButton, NMASTERINFO_DIALOG_MASTERCHATBUTTON, TEXT("button_masterchat"));   
		file.CreateControl( m_pCloseButton, NMASTERINFO_DIALOG_CLOSEBUTTON, TEXT("button_close"));        
		

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//KHY - 0220 -  푸치코미 옵션 삭제. = 꼼지가기
		if(!pclClient->IsCountrySwitch(Switch_Puchikomi)) 
		{
			m_pGGOMZIButton->Show(false);
		}/*국내에서 서비스할 군주S에 꼼지를 서비스 하지 않기 위해서 비활성화 시킨다.
		 서비스 지역을 비교해서 내용을 실행*/
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//InitCommonControls();
		m_siSelectedIndex = -1 ;

		m_pMasterList->SetFontSize( 12 );
		m_pMasterList->SetFontWeight( 0 );
		m_pMasterList->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );

		if(m_pMasterList)
		{
			// 손재주와 행운을 두개 보여지던것을 행운만 보여지게 한다
			if ( pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
			{
				TCHAR* pText = GetTxtFromMgr(1556);
				m_pMasterList->SetColumn( 0, 137, pText );
				pText = GetTxtFromMgr(3099);
				m_pMasterList->SetColumn( 1, 60, pText );
				pText = GetTxtFromMgr(10016);
				m_pMasterList->SetColumn( 2, 37, pText );
				pText = GetTxtFromMgr(3101);
				m_pMasterList->SetColumn( 3, 84, pText );
				m_pMasterList->SetColumn( 4, 16, NULL);
				m_pMasterList->SetBorder( true );
				m_pMasterList->SetBKColorUse( true );
			}
			else
			{
				TCHAR* pText = GetTxtFromMgr(1556);
				m_pMasterList->SetColumn( 0, 100, pText );
				pText = GetTxtFromMgr(3099);
				m_pMasterList->SetColumn( 1, 60, pText );
				pText = GetTxtFromMgr(3100);
				m_pMasterList->SetColumn( 2, 37, pText );
				pText = GetTxtFromMgr(10016);
				m_pMasterList->SetColumn( 3, 37, pText );
				pText = GetTxtFromMgr(3101);
				m_pMasterList->SetColumn( 4, 84, pText );
				m_pMasterList->SetColumn( 5, 16, NULL);
				m_pMasterList->SetBorder( true );
				m_pMasterList->SetBKColorUse( true );
			}
		}

		m_pMasterList->SetDrawLine( true );
		m_pMasterList->Refresh();

		//---------------------------------
		// HTML 을 초기화한다.
		//---------------------------------
		//==================================			
		//HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE);
		//if ( hwnd )
		//{
		//	EmbedBrowserObject(hwnd);
		//}

		//MoveWindow( GetDlgItem( hDlg, IDC_STATIC_PROFILE ), 2, 222, 73, 73, TRUE );

		//hwnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
		//if ( hwnd )
		//{
		//	EmbedBrowserObject(hwnd);
		//}

		//==================================

		siCurrentSkillUnique = 0;

//		SetTimerOnOff( true );
		//-----------------------------------
		// 기술목록을 설정한다. 
		//-----------------------------------
		SetSkillList();

		if(m_pMasterSkillComboBox->GetItemNum() > 0)
			m_pMasterSkillComboBox->SetCursel( 0 );

		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENMASTER);

	}
	else
	{
		DeleteDialog();
	}
}

void CALLBACK CNMasterInfo::StaticDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMasterInfo *pThis = (CNMasterInfo*) pControllerMgr;
	pThis->DlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNMasterInfo::DlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	SI32 id					= 1;
	//static SI32 siOldSKillUnique;
	//cltClient* pclclient = (cltClient*)pclClient;

	switch( nControlID )
	{
	//case WM_SIZE:
	//	{
	//		//============================
	//		// HTML Resize 를 한다.
	//		//============================
	//		//==================================
	//		HWND hwnd = GetDlgItem(hDlg, IDC_STATIC_PROFILE );
	//		if( hwnd )
	//		{
	//			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
	//		}
	//		hwnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
	//		if( hwnd )
	//		{
	//			ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
	//		}
	//	}
	//	return TRUE;
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NMASTERINFO_DIALOG_CLOSEBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					m_siSelectedIndex = -1 ;
					DeleteDialog();
				}
				break;
			}
		}
		break;
	case NMASTERINFO_DIALOG_MASTERSKILLCOMBOBOX:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
					TCHAR szskillname[128];
					SI16 tmpSelectedIndex = 0;

					if(m_pMasterSkillComboBox)
					{
						tmpSelectedIndex = m_pMasterSkillComboBox->GetSelectedIndex();

						if(tmpSelectedIndex >=0)
						{
							StringCchCopy(szskillname, 128, m_pMasterSkillComboBox->GetText(tmpSelectedIndex));

							siCurrentSkillUnique = pclClient->pclSkillManager->FindUniqueFromSkillName(szskillname);

							if(siCurrentSkillUnique > 0 && siCurrentSkillUnique != siOldSKillUnique)
							{
								siOldSKillUnique = siCurrentSkillUnique;

								m_pMasterList->Clear();
								m_pMasterList->Refresh();

								cltGameMsgRequest_SkillMasterList clinfo(siCurrentSkillUnique);
								cltMsg clMsg(GAMEMSG_REQUEST_SKILLMASTERLIST, sizeof(clinfo), (BYTE*)&clinfo);
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

								/*
								//-------------------------------------
								// 장인 목록을 삭제한다. 
								//-------------------------------------
								clMasterInfo[siCurrentSkillUnique].Init();
								//---------------------------------------
								//장인 목록을 보여준다.
								//---------------------------------------
								ShowMasterList(siCurrentSkillUnique);
								*/
							}

						}
					}
				}
				break;
			}
		}
		break;

	case NMASTERINFO_DIALOG_MASTERLIST:
		{
			switch( nEvent )
			{

			case EVENT_LISTBOX_SELECTION:
				{
					SI16 tmpSelectedListIndex = -1;

					tmpSelectedListIndex = m_pMasterList->GetSelectedIndex();

					if ( siSkillInfo[ tmpSelectedListIndex ] >= 0 )
					{
						if ( clMasterInfo[siCurrentSkillUnique].clUnit[siSkillInfo[tmpSelectedListIndex]].szName != NULL )
						{
							if ( clMasterInfo[siCurrentSkillUnique].clUnit[siSkillInfo[ tmpSelectedListIndex ]].szAccountID != NULL )
							{
								/*TCHAR ProfileURL[MAX_PATH] = TEXT("") ;

								pclClient->GetCZIntroUrl( ProfileURL, clMasterInfo[siCurrentSkillUnique].clUnit[siSkillInfo[tmpSelectedListIndex]].szAccountID );				*/
								// leon todo - ProfileURL로 HTML 재작업하기
								//hWnd = GetDlgItem(hDlg, IDC_STATIC_INTRO);
								//if(hWnd)	DisplayHTMLPage(hWnd, ProfileURL);
							}

							m_siSelectedIndex = siSkillInfo[ tmpSelectedListIndex ];

							//SI32 charid = clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].siCharUnique;
							SI32 personid = clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].siPersonID;
							
							cltGameMsgRequest_GetGreeting clinfo(personid, 1);
							cltMsg clMsg(GAMEMSG_REQUEST_GETGREETING, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
						}
					}
				}
				break;
			}
		}
		break;				
	case NMASTERINFO_DIALOG_GGOMZIBUTTON:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					//cltClient *pclclient = (cltClient *)pclClient;

					if ( m_siSelectedIndex > -1 )
					{	
						if ( clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].siCharUnique > 0 )
						{
							if ( clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].szName != NULL )
							{	
								cltGameMsgRequest_UserPuchikomiURL clUserPuchikomi( (TCHAR*)clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].szName );
								cltMsg clMsg( GAMEMSG_REQUEST_USERPUCHIKOMIURL , sizeof( clUserPuchikomi ), (BYTE*)&clUserPuchikomi );
								pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
								
								/*
								cltGameMsgRequest_Userid clUserid( (TCHAR*)clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].szName );
								cltMsg clMsg( GAMEMSG_REQUEST_USERID, sizeof( clUserid ), (BYTE*)&clUserid );
								pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
								*/
							}
						}
					}
				}
				break;
			}
		}
		break;

	case NMASTERINFO_DIALOG_MASTERCHATBUTTON:
		{
			cltClient *pclclient = (cltClient *)pclClient;
			switch( nEvent )
			{

			case EVENT_BUTTON_CLICKED:
				{
					if ( m_siSelectedIndex > -1 )
					{	
						if ( clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].siCharUnique > 0 )
						{
							if ( !_tcscmp(pclClient->pclCM->CR[1]->pclCI->clBI.GetName() ,clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].szName) )
							{
								TCHAR* pTitle = GetTxtFromMgr(2449);
								TCHAR* pText = GetTxtFromMgr(2450);
								pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
								break;
							}
							
							cltClient *pclclient = (cltClient *)pclClient;
							pclclient->RequestSecretChatByCharUnique( clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].siCharUnique );
						}
					}
				}
				break;
			}
		}
		break;
	}

	// leon todo - IDC_BUTTON_DETAILINFO를 확인 할 수 없음??
	//case NMASTERINFO_DIALOG_DETAILINFOBUTTON:
	//	{
	//		switch( nEvent )
	//		{
	//		case EVENT_BUTTON_CLICKED:
	//		//case IDC_BUTTON_DETAILINFO:
	//			{
	//				if ( m_siSelectedIndex > -1 )
	//				{	
	//					if ( clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].siCharUnique > 0 )
	//					{
	//						cltGameMsgRequest_PersonPrivateInfoByPersonID clinfo(clMasterInfo[siCurrentSkillUnique].clUnit[m_siSelectedIndex].siCharUnique);
	//						cltMsg clMsg(GAMEMSG_REQUEST_PERSONPRIVATEINFOBYPERSONID, sizeof(clinfo), (BYTE*)&clinfo);
	//						pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	//					}
	//				}
	//			}
	//			break;
	//		}
	//	}
	//	break;
}

void CNMasterInfo::Sort(SI32 sisortdata[])
{
	SI32 i;
	SI32 index = 0;

	SI32 tempbuf[ MAX_NMASTER_PER_SKILL ];

	for( i = 0; i < MAX_NMASTER_PER_SKILL; ++i )
	{
		tempbuf[ i ] = i;
	}

	for(SI32 vill = 0; vill < MAX_NMASTER_PER_SKILL - 1;vill++)
	{
		for(i = vill + 1; i < MAX_NMASTER_PER_SKILL; i++)
		{
			int cmpindex1 = tempbuf[ vill ];
			int cmpindex2 = tempbuf[ i ];

			if(sisortdata[cmpindex1] < sisortdata[cmpindex2])
			{
				int temp = tempbuf[ vill ];
				tempbuf[ vill ] = tempbuf[ i ];
				tempbuf[ i ] = temp;
			}
		}
	}

	for(i = 0; i < MAX_NMASTER_PER_SKILL; ++i )
	{
		siSkillInfo[ i ] = tempbuf[ i ];
		index++;
	}

}

SI32 CNMasterInfo::Sort(TCHAR* psortname[], SI32* punique)
{
	SI32 i;
	SI32 index = 0;

	SI32 tempbuf[ MAX_SKILL_NUMBER ];

	for( i = 0; i < MAX_SKILL_NUMBER; ++i )
	{
		tempbuf[ i ] = i;
	}

	for(SI32 master = 0; master < MAX_SKILL_NUMBER - 1; master++)
	{
		if(pclClient->pclSkillManager->pclSkillInfo[ master ] == NULL)continue;

		for(i = master + 1;i < MAX_SKILL_NUMBER;i++)
		{
			if(pclClient->pclSkillManager->pclSkillInfo[i] == NULL)continue;

			int cmpindex1 = tempbuf[ master ];
			int cmpindex2 = tempbuf[ i ];

			SI32 result = _tcscoll( psortname[ cmpindex1 ], psortname[ cmpindex2 ] );
			
			/*cyj 스킬목록은 소트 안하도록 수정
			if( result < 0 )
			{
				int temp = tempbuf[ master ];
				tempbuf[ master ] = tempbuf[ i ];
				tempbuf[ i ] = temp;
			}
			*/
		}
	}

	index = 0;
	for(i = 0; i < MAX_SKILL_NUMBER; ++i )
	{
		if( pclClient->pclSkillManager->pclSkillInfo[ tempbuf[ i ] ] == NULL)continue;
		punique[ index ] = tempbuf[ i ];
		index++;
	}

	return index;
}


void CNMasterInfo::ShowMasterList(SI32 skillunique)
{
	TCHAR buffer[256] = TEXT("") ;
	if(m_pMasterList == NULL)return ;

	// 말조련술을 보는 퀘스트 중이라면, 
	if(skillunique == SKILL_HORSETRAINING)
	{
		pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_SKILLMASTER);
	}

	m_pMasterList->Clear();

	SI32 siSortIndex = 0;
	TCHAR szLevel[ 32 ] = TEXT("");
	TCHAR szHand[ 32 ] = TEXT("");
	TCHAR szLuck[ 32 ] = TEXT("");
	TCHAR szVillage[ 128 ] = TEXT("");

	SI32 sidata[MAX_NMASTER_PER_SKILL];

	for(SI32 i = 0;i < MAX_NMASTER_PER_SKILL;i++)
	{
		siSkillInfo[i] = i;
		sidata[i] = -1 ;
		if(clMasterInfo[skillunique].clUnit[i].siCharUnique <= 0)continue;
		if(clMasterInfo[skillunique].clUnit[i].siSkillLevel <= 0) continue;
		if(clMasterInfo[skillunique].clUnit[i].siSkillLevel < pclClient->pclSkillManager->GetMaxLevel(skillunique)) continue;
		sidata[i] = clMasterInfo[skillunique].clUnit[i].siSkillLevel;
	}

	Sort(sidata);

	//SI32 tempunique = 0 ;

	SI16 siColumn = 0;
	for(i = 0;i < MAX_NMASTER_PER_SKILL;i++)
	{
		if(siSkillInfo[i] < 0 ) continue;
		if(clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siCharUnique <= 0)continue;
		if(clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siSkillLevel <= 0 ) continue;
		if(clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siSkillLevel < pclClient->pclSkillManager->GetMaxLevel(skillunique)) continue;
	//	if(clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siCharUnique == tempunique ) continue;

		stListBoxItemData tmpListBoxItemData;

		siColumn = 0;
		tmpListBoxItemData.Init();
		tmpListBoxItemData.Set(siColumn++, clMasterInfo[skillunique].clUnit[siSkillInfo[i]].szName);
		tmpListBoxItemData.siParam[ 0 ] = -1;

		StringCchPrintf( szLevel, 32, TEXT("%d/%d"), clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siSkillLevel ,pclClient->pclSkillManager->GetTotalMaxLevel(skillunique));
		tmpListBoxItemData.Set(siColumn++, szLevel);

		// 스위치가 OFF되어있을때만 손재주를 보여준다
		if ( false == pclClient->IsCountrySwitch(Switch_ManufactureInfluenceLuck) )
		{
			StringCchPrintf(szHand, 32, TEXT("%d"),clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siHnd);
			tmpListBoxItemData.Set(siColumn++, szHand);
		}

		StringCchPrintf(szLuck, 32, TEXT("%d"),clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siLuk);
		tmpListBoxItemData.Set(siColumn++, szLuck);

		if (clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siMapIndex > 0 )
			StringCchCopy(szVillage, 128, pclClient->pclMapManager->GetMapName( clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siMapIndex));
		else
		{
			TCHAR* pText = GetTxtFromMgr(1287);
			StringCchCopy(szVillage, 128, pText);
		}

		tmpListBoxItemData.Set(siColumn++, szVillage);
		m_pMasterList->AddItem( &tmpListBoxItemData );

	//	tempunique = clMasterInfo[skillunique].clUnit[siSkillInfo[i]].siCharUnique ;

		siSortIndex++ ;
	}

	m_pMasterList->Refresh();
}

// 스킬 목록을 만든다. 
void CNMasterInfo::SetSkillList()
{
	TCHAR* sidata[MAX_SKILL_NUMBER];
	SI32  siMaster[MAX_SKILL_NUMBER];
	
	 //콤보 박스를 지운다.
	m_pMasterSkillComboBox->Clear();

	SI32 i;
	for(i = 0;i < MAX_SKILL_NUMBER;i++)
	{
		cltSkillInfo* pclskillinfo = pclClient->pclSkillManager->pclSkillInfo[i];
		sidata[i] = pclskillinfo->szName;
	}

	SI32 index = Sort(sidata, siMaster);

	for(i = 0;i < index; ++i)
	{
		SI32 tmpunique = siMaster[i];

		cltSkillInfo* pclskillinfo = pclClient->pclSkillManager->pclSkillInfo[tmpunique];
		if(pclskillinfo == NULL)continue;
		if((pclskillinfo->siSkillAtb & SKILLATB_LIST) == 0)continue;

		stComboBoxItemData tmpComboBoxItemData;

		tmpComboBoxItemData.Init();
		tmpComboBoxItemData.Set( pclskillinfo->szName );
		m_pMasterSkillComboBox->AddItem( &tmpComboBoxItemData );
	}

	// 콤보 박스에 기술목록을 설정한다. 
	//for(SI32 i = 0;i < MAX_SKILL_NUMBER;i++)
	//{	
	//	cltSkillInfo* pclskillinfo = pclClient->pclSkillManager->pclSkillInfo[i];
	//	if(pclskillinfo == NULL)continue;
	//	if((pclskillinfo->siSkillAtb & SKILLATB_LIST) == 0)continue;

	//	stComboBoxItemData tmpComboBoxItemData;

	//	tmpComboBoxItemData.Init();
	//	tmpComboBoxItemData.Set( pclskillinfo->szName );
	//	m_pMasterSkillComboBox->AddItem( &tmpComboBoxItemData );
	//}
}

// Action() : WM_TIMER 처리 함수
void CNMasterInfo::Action()
{
}

void CNMasterInfo::SetGreetingText(TCHAR* pName, TCHAR* pText)
{
	TCHAR mastername[30];
	TCHAR* pText2 = GetTxtFromMgr(5760);
	//cyj 아래의 두줄 텍스트 메니저로 뺌
	//_tcscpy(mastername, pName);
	//strcat(mastername, TEXT("님의 인사말"));
	StringCchPrintf(mastername, 30, pText2, pName);

	m_pMasterNameStatic->SetText(mastername, DT_LEFT);
	m_pMasterGreeting->SetText(pText);
}
//bool CNMasterInfo::IsShow()
//{
//	return m_bShow;
//}
//
//void CNMasterInfo::Show()
//{
//    if( IsShow() )
//	{
//		Hide();
//		return;
//	}
//
//	SetTimerOnOff( true );
//
//	m_bShow = true;
//
//	CDialogController::Show( true );
//
//	return;
//}
//
//void CNMasterInfo::Hide()
//{
//	SetTimerOnOff( false );
//
//	m_bShow = false;
//
//	CDialogController::Show( false );
//
//	return;
//}

//#endif