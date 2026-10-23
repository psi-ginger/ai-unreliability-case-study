#include "NNamingQuestListDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../Common/Quest/NamingQuest.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/Msg/MsgType-NamingQuest.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"

extern cltCommonLogic* pclClient;

CNNamingQuestListDlg::CNNamingQuestListDlg()
{	
	Initialize();
	Init()	;
}

CNNamingQuestListDlg::~CNNamingQuestListDlg()
{
	Destroy();
}


void CNNamingQuestListDlg::Init()
{
	m_siSelectedIndex = -1;
	m_siSelectedLevel = -1;
}

void CNNamingQuestListDlg::Destroy()
{

}

void CNNamingQuestListDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NNamingQuest/DLG_NNamingQuestList.ddf"));
	file.CreatePopupDialog( this, NNAMINGQUESTLIST_DLG , TEXT("dialog_namingquestlist"), StaticNamingQuestListDlgProc );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, 		NNAMINGQUESTLIST_STATIC_1 , 			this ), NNAMINGQUESTLIST_STATIC_1,			"statictext_NONAME1"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, 		NNAMINGQUESTLIST_STATIC_2 , 			this ), NNAMINGQUESTLIST_STATIC_2,			"statictext_NONAME2"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, 		NNAMINGQUESTLIST_STATIC_3 , 			this ), NNAMINGQUESTLIST_STATIC_3,			"statictext_NONAME3"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, 		NNAMINGQUESTLIST_STATIC_4 , 			this ), NNAMINGQUESTLIST_STATIC_4,			"statictext_NONAME4"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, 		NNAMINGQUESTLIST_STATIC_5 , 			this ),	NNAMINGQUESTLIST_STATIC_5,			"statictext_NONAME5"	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC,	NNAMINGQUESTLIST_IMAGE_PORTRAIT ,		this ), NNAMINGQUESTLIST_IMAGE_PORTRAIT,	"imagestatic_portrait"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, 			NNAMINGQUESTLIST_EDIT_EXPLAIN ,			this ), NNAMINGQUESTLIST_EDIT_EXPLAIN,  	"editbox_questexplain"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, 			NNAMINGQUESTLIST_EDIT_TITLE ,			this ), NNAMINGQUESTLIST_EDIT_TITLE,		"editbox_title"			); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, 			NNAMINGQUESTLIST_EDIT_MONSTERNAME ,		this ), NNAMINGQUESTLIST_EDIT_MONSTERNAME,  "editbox_monstername"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, 			NNAMINGQUESTLIST_EDIT_MONSTERNUMBER ,	this ), NNAMINGQUESTLIST_EDIT_MONSTERNUMBER,"editbox_monsternumber"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, 			NNAMINGQUESTLIST_EDIT_NAMING ,			this ), NNAMINGQUESTLIST_EDIT_NAMING,		"editbox_naming"		); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT, 			NNAMINGQUESTLIST_EDIT_ADVANTAGE ,		this ), NNAMINGQUESTLIST_EDIT_ADVANTAGE,	"editbox_adv"			); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, 			NNAMINGQUESTLIST_LIST_SELECT ,			this ), NNAMINGQUESTLIST_LIST_SELECT,		"listbox_quest_select"	); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,			NNAMINGQUESTLIST_BUTTON_START ,			this ), NNAMINGQUESTLIST_BUTTON_START,		"button_queststart"		);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,		NNAMINGQUESTLIST_OUTLINE_1 ,			this ), NNAMINGQUESTLIST_OUTLINE_1,			"outline_NONAME1"		);

	CImageStatic* pImage = m_InterfaceFactory.GetImageStatic( NNAMINGQUESTLIST_IMAGE_PORTRAIT );
	if ( pImage )
	{
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_TITLENPC") ), szPortrait, sizeof(szPortrait), &siPortraitFont);
		pImage->SetFileName(szPortrait);
		pImage->SetFontIndex(siPortraitFont);
	}

	CEdit* pEdit = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_EXPLAIN );
	if ( pEdit )
	{
		TCHAR* pText = GetTxtFromMgr( 10155 );
		pEdit->SetText( pText );
	}


	CList* pList = m_InterfaceFactory.GetList(NNAMINGQUESTLIST_LIST_SELECT );

	if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 10093 );
		pList->SetColumn(0, 100, pText);
		pText = GetTxtFromMgr( 10094 );
		pList->SetColumn(1, 95, pText);
		pText = GetTxtFromMgr( 10095 );
		pList->SetColumn(2, 95, pText);
		pText = GetTxtFromMgr( 1506 );
		pList->SetColumn(3, 30, pText);
		pList->Refresh();

		stListBoxItemData tempItemData;

		for ( SI32 i=0; i < MAX_NAMINGQUEST_INDEX; ++i )
		{
			tempItemData.Init();

			SI32 currLevel = pclClient->pclCM->CR[1]->pclCI->clPersonNaming.m_siLevel[i];
			if ( currLevel < pclClient->pclNamingQuestManager->GetMaxLevel(i) )
			{
				// 다음단계의 퀘스트를 얻어온다.
				cltNamingQuestUnit* pclNamingUnit = pclClient->pclNamingQuestManager->GetQuestInfo(i, currLevel+1 );
				if(NULL == pclNamingUnit)
					continue;

				tempItemData.Set( 0, pclClient->pclNamingQuestManager->GetTypeName(i) );
				tempItemData.Set( 1, pclNamingUnit->GetNaming(), i );

				TCHAR szLevel[8] = {0,};

				StringCchPrintf( szLevel, 8, TEXT("%d"), currLevel + 1 );
				tempItemData.Set( 3, szLevel, currLevel + 1 );

				if ( currLevel > 0 )
				{
					cltNamingQuestUnit* pclNamingUnit = pclClient->pclNamingQuestManager->GetQuestInfo(i,currLevel );
					if(NULL == pclNamingUnit)
						continue;

					tempItemData.Set( 2, pclNamingUnit->GetNaming()  );
				}

				pList->AddItem( &tempItemData );
			}
		}
	}

	pList->Refresh();

	Show(true);

	//-----------------------------------------------------------
	// 게임 서버로 퀘스트가 완료되었음을 알려준다.
	//-----------------------------------------------------------
	SI16 questtype, questunique, para1, para2;
	SI16 questclearcond =  QUEST_CLEAR_COND_OPENNAMINGQUEST;
	if( pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsQuestGoing(questclearcond, &questtype, &questunique, &para1, &para2) == true)
	{
		cltGameMsgRequest_QuestStatus clinfo(questtype, questunique, questclearcond, para1, para2  );
		cltMsg clMsg(GAMEMSG_REQUEST_QUESTSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
}

void CALLBACK CNNamingQuestListDlg::StaticNamingQuestListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNNamingQuestListDlg *pThis = (CNNamingQuestListDlg*) pControllerMgr;
	pThis->NNamingQuestListDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNNamingQuestListDlg::NNamingQuestListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			CList* pList = m_InterfaceFactory.GetList(NNAMINGQUESTLIST_LIST_SELECT );
			if(pList != NULL )
			{
				pList->ClearSelectIndex()	;
			}
			DeleteDialog();
		}
		break;
	case NNAMINGQUESTLIST_LIST_SELECT:
		{
			if ( EVENT_LISTBOX_SELECTION == nEvent )
			{
				CList* pList = m_InterfaceFactory.GetList( NNAMINGQUESTLIST_LIST_SELECT );
				if ( pList == NULL ) break;

				SI16 row = pList->GetSelectedIndex();

				if ( row > -1 )
				{
					ShowDetailInfo( row );
				}
			}
		}
		break;
	case NNAMINGQUESTLIST_BUTTON_START:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				StartNamingQuest();
				//DeleteDialog();
			}
		}

	}
}
void CNNamingQuestListDlg::ShowDetailInfo( SI32 Index )
{
	CEdit* pEdit_Explain	 = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_EXPLAIN		);
	CEdit* pEdit_Title		 = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_TITLE			);
	CEdit* pEdit_MonsterName = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_MONSTERNAME	);
	CEdit* pEdit_MonsterNum	 = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_MONSTERNUMBER	);
	CEdit* pEdit_Naming		 = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_NAMING			);
	CEdit* pEdit_Advantage	 = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_ADVANTAGE		);
	CList* pList			 = m_InterfaceFactory.GetList( NNAMINGQUESTLIST_LIST_SELECT			);

	if ( pEdit_Explain == NULL || pEdit_Title == NULL || pEdit_MonsterName == NULL || 
		pEdit_MonsterNum == NULL || pEdit_Naming == NULL || pEdit_Advantage == NULL || pList == NULL  )
	{
		return;
	}

	pEdit_Explain->SetText( TEXT("") );


	SI32 index = pList->GetParam( Index, 1 );
	SI32 level = pList->GetParam( Index, 3 );

    cltNamingQuestUnit* pclUnit = pclClient->pclNamingQuestManager->GetQuestInfo( index, level );

	if ( pclUnit == NULL ) return;

	pEdit_Title->SetText( pclUnit->GetQuestName() );
	pEdit_Naming->SetText( pclUnit->GetNaming() );

	SI32 questType = pclClient->pclNamingQuestManager->GetType( index );

	switch ( questType )
	{
	case NAMINGQUEST_TYPE_HUNT:
		{
			TCHAR * pMonName = (TCHAR*)pclClient->pclKindInfoSet->pclKI[ pclUnit->GetQuestClerCond()->siPara1 ]->GetName();
			pEdit_MonsterName->SetText( pMonName );

			//TODO - 인사말 출력..
            TCHAR* pText = GetTxtFromMgr(10098);
			TCHAR num[16] = {0,};
			StringCchPrintf( num, 16, TEXT("%d"), pclUnit->GetQuestClerCond()->siPara2 );
			TCHAR buf[256] = {0,};
			Nsprintf( buf, pText, TEXT("NAME"), pMonName, TEXT("NUM"), num, TEXT("TITLE"), pclUnit->GetNaming() , NULL);

			pEdit_Explain->SetText( buf );
		}
		break;
	case NAMINGQUEST_TYPE_COLLECT:
		{
			TCHAR * pItemName = (TCHAR*)pclClient->pclItemManager->GetName( pclUnit->GetQuestClerCond()->siPara1 );
			pEdit_MonsterName->SetText( pItemName );

			//TODO - 인사말 출력..
			TCHAR* pText = GetTxtFromMgr(10099);
			TCHAR num[16] = {0,};
			StringCchPrintf( num, 16, TEXT("%d"), pclUnit->GetQuestClerCond()->siPara2 );
			TCHAR buf[256] = {0,};
			Nsprintf( buf, pText, TEXT("NAME"), pItemName, TEXT("NUM"), num, TEXT("TITLE"), pclUnit->GetNaming() , NULL);
			pEdit_Explain->SetText( buf );
		}
		break;
	}

	if ( questType == NAMINGQUEST_TYPE_HUNT || questType == NAMINGQUEST_TYPE_COLLECT )
	{
		TCHAR szMonNum[16] = {0,};
		StringCchPrintf( szMonNum, 16, TEXT("%d"), pclUnit->GetQuestClerCond()->siPara2 );
		pEdit_MonsterNum->SetText( szMonNum );
	}

	TCHAR bufadv[256] = {0,};
	if ( pclUnit->GetAttack() > 0 )
	{
		TCHAR buf[256] = {0,};
		TCHAR* ptext = GetTxtFromMgr( 4399 ); // 공격력
		StringCchPrintf( buf, 256, TEXT("%s%d%% "), ptext, pclUnit->GetAttack() );
		StringCchCat( bufadv, 256, buf );
	}
	if ( pclUnit->GetDefense() > 0 )
	{
		TCHAR buf[256] = {0,};
		TCHAR* ptext = GetTxtFromMgr( 4400 ); // 방어력
		StringCchPrintf( buf, 256, TEXT("%s%d%% "), ptext, pclUnit->GetDefense() );
		StringCchCat( bufadv, 256, buf );
	}

	pEdit_Advantage->SetText( bufadv );

	m_siSelectedIndex = index;
	m_siSelectedLevel = level;


}

void CNNamingQuestListDlg::StartNamingQuest()
{
	// 빈인덱스의 퀘스트는 선택불가
	if( m_siSelectedIndex < 0 )
	{
		cltClient* pclclient = (cltClient*)pclClient;
		TCHAR* pTitle = GetTxtFromMgr(8431);
		TCHAR* pText = GetTxtFromMgr(8432);
		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
		pclclient->PushEffect(pclClient->GetUniqueFromHash(TEXT("SOUND_EFFECT_FAILURE")), 0, 0);
		return;
	}
	else
	{
		cltGameMsgRequest_NamingQuestStart clInfo( m_siSelectedIndex, m_siSelectedLevel );
		cltMsg clMsg( GAMEMSG_REQUEST_NAMINGQUEST_START, sizeof( clInfo ), (BYTE*)&clInfo );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
	}
	
	
}

void CNNamingQuestListDlg::SetExplain(SI32 _TextNum)
{
	CEdit* pEdit = m_InterfaceFactory.GetEdit( NNAMINGQUESTLIST_EDIT_EXPLAIN );
	if ( pEdit )
	{
		TCHAR* pText = GetTxtFromMgr( _TextNum );
		pEdit->SetText( pText );
	}
}