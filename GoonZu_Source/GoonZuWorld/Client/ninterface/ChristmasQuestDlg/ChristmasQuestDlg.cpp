#include ".\christmasquestdlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../Common/Quest/NamingQuest.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/Msg/MsgType-NamingQuest.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"

#include "../../Client/ninterface/nnewquestdlg/NNewQuestDlg.h"

extern cltCommonLogic* pclClient;
 
CNChristmasQuestDlg::CNChristmasQuestDlg()
{	 
	Initialize();
}

CNChristmasQuestDlg::~CNChristmasQuestDlg()
{
	Destroy();
}


void CNChristmasQuestDlg::Init()
{
	m_siSelectedIndex = 0;
}

void CNChristmasQuestDlg::Destroy()
{

}

void CNChristmasQuestDlg::Create()
{
	cltClient* pclclient = (cltClient*)pclClient;

	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NChristmasQuestDlg/ChristmasQuestDlg.ddf"));
	file.CreatePopupDialog( this, NCHRISMASQUEST_DLG , TEXT("dialog_ChristmasQuestlist"), StaticChristmasQuestListDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NCHRISMASQUEST_IMAGE_PORTRAIT,			this ),	NCHRISMASQUEST_IMAGE_PORTRAIT,			"imagestatic_portraiticon"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		 NCHRISMASQUEST_EDIT_EXPLAIN,			this ),	NCHRISMASQUEST_EDIT_EXPLAIN,			"editbox_questexplain"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,		 NCHRISMASQUEST_LIST_SELECT,			this ),	NCHRISMASQUEST_LIST_SELECT,				"listbox_quest_select"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		 NCHRISMASQUEST_BUTTON_START,			this ),	NCHRISMASQUEST_BUTTON_START,			"button_queststart");

	
	CImageStatic* pImage = m_InterfaceFactory.GetImageStatic( NCHRISMASQUEST_IMAGE_PORTRAIT );
	if ( pImage )
	{
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash( TEXT("KIND_RUDOLPH") ), szPortrait, sizeof(szPortrait), &siPortraitFont);
		pImage->SetFileName(szPortrait);
		pImage->SetFontIndex(siPortraitFont);
	}

	CEdit* pEdit = m_InterfaceFactory.GetEdit( NCHRISMASQUEST_EDIT_EXPLAIN );
	if ( pEdit )
	{
		TCHAR* pText = GetTxtFromMgr( 7640  );
		pEdit->SetText( pText );
	}


	CList* pList = m_InterfaceFactory.GetList(NCHRISMASQUEST_LIST_SELECT);

	if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 601 );
		pList->SetColumn(0, 320, pText);
		
		pList->Refresh();

		
		stListBoxItemData tempItemData;

		SI32 tempclearnum = 0;
		for ( SI32 i = 0; i < 4; ++i )
		{
			if(i == 3)
			{
				tempclearnum = pclClient->pclCM->CR[1]->pclCI->clQuestInfo.GetClearQuestNum(pclClient->pclQuestManager, QUEST_TYPE_PLAYCHRISTMASHIDDEN );				
				tempItemData.Set(0, pclClient->pclQuestManager->clQuestInfo[QUEST_TYPE_PLAYCHRISTMASHIDDEN][tempclearnum].szTitleText);
			}
			else
			{
				// [검색 : 황진성 2007. 12. 6 퀘스트 진행 순서(번호) 얻어오기] 
				tempclearnum = pclClient->pclCM->CR[1]->pclCI->clQuestInfo.GetClearQuestNum(pclClient->pclQuestManager, 24+i );				
				tempItemData.Set(0, pclClient->pclQuestManager->clQuestInfo[24+i][tempclearnum].szTitleText);
			}
			pList->AddItem( &tempItemData );
		}
		
	}
	pList->Refresh();

	if(pclclient->m_pDialog[ NNEWQUEST_DLG ] != NULL)
	{
		((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->Show( false );
		pclclient->CreateInterface( NNEWQUEST_DLG );
	}
	Show(true);
}

void CALLBACK CNChristmasQuestDlg::StaticChristmasQuestListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNChristmasQuestDlg *pThis = (CNChristmasQuestDlg*) pControllerMgr;
	pThis->NChristmasQuestListDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNChristmasQuestDlg::NChristmasQuestListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NCHRISMASQUEST_LIST_SELECT:
		{
			if ( EVENT_LISTBOX_SELECTION == nEvent )
			{
				CList* pList = m_InterfaceFactory.GetList( NCHRISMASQUEST_LIST_SELECT );
				if ( pList == NULL ) break;

				m_siSelectedIndex = pList->GetSelectedIndex();
			}
		}
		break;
	case NCHRISMASQUEST_BUTTON_START:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				StartChristmasQuest();
				DeleteDialog();
			}
		}

	}
}


void CNChristmasQuestDlg::StartChristmasQuest()
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 siQuestType			= 0;

	switch(m_siSelectedIndex)
	{
	case 0:		siQuestType = QUEST_TYPE_MAKESANTAHAT;		break;
	case 1:		siQuestType = QUEST_TYPE_MAKECAKE;			break;
	case 2:		siQuestType = QUEST_TYPE_PLAYCHRISTMAS;		break;
	case 3:
		{
			if((pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsAllClear(pclClient->pclQuestManager, QUEST_TYPE_MAKESANTAHAT) == true)
				&& (pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsAllClear(pclClient->pclQuestManager, QUEST_TYPE_MAKECAKE) == true)
				&& (pclClient->pclCM->CR[1]->pclCI->clQuestInfo.IsAllClear(pclClient->pclQuestManager, QUEST_TYPE_PLAYCHRISTMAS) == true))
			{
				siQuestType = QUEST_TYPE_PLAYCHRISTMASHIDDEN;
			}
			else
			{
				TCHAR* pTitle = GetTxtFromMgr(223);
				TCHAR* pText = GetTxtFromMgr(40314);
				pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText);
				return;
			}
		}
		break;
	}

	if(pclclient->m_pDialog[ NNEWQUEST_DLG ] == NULL)
	{
		pclclient->CreateInterface( NNEWQUEST_DLG );

		if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
		{
			pclclient->pclQuestManager->bNewQuseOpened = TRUE;
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_siButtonType = 0;
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_siQuestType_Rudolph = siQuestType;
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetNewQuest( pclClient->GetUniqueFromHash( TEXT("KIND_RUDOLPH")) );
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetOpenedQuestDlg( true );
		}
	}

}


