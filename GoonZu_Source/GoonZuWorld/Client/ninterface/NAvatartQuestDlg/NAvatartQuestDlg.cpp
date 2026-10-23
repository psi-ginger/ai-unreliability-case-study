#include "NAvatartQuestDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../Common/Quest/NamingQuest.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/Msg/MsgType-NamingQuest.h"
#include "../../CommonLogic/Msg/MsgType-Quest.h"

#include "../../Client/ninterface/nnewquestdlg/NNewQuestDlg.h"

extern cltCommonLogic* pclClient;
 
CNAvatarQuestDlg::CNAvatarQuestDlg()
{	 
	Initialize();
}

CNAvatarQuestDlg::~CNAvatarQuestDlg()
{
	Destroy();
}


void CNAvatarQuestDlg::Init()
{
	m_siSelectedIndex = 0;
}

void CNAvatarQuestDlg::Destroy()
{

}

void CNAvatarQuestDlg::Create()
{
	cltClient* pClient = (cltClient*)pclClient;

	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NAvatarQuestDlg/NAvatarQuestDlg.ddf"));
	file.CreatePopupDialog( this, NAVATARQUEST_DLG, TEXT("dialog_AvatarQuestList"), StaticNAvatarQuestDlgProc );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NAVATARQUEST_IMAGE_PORTRAIT,		this ),	NAVATARQUEST_IMAGE_PORTRAIT,		"imagestatic_portraiticon"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		 NAVATARQUEST_EDIT_EXPLAIN,			this ),	NAVATARQUEST_EDIT_EXPLAIN,			"editbox_questexplain"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,		 NAVATARQUEST_LIST_SELECT,			this ),	NAVATARQUEST_LIST_SELECT,			"listbox_quest_select"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		 NAVATARQUEST_BUTTON_START,			this ),	NAVATARQUEST_BUTTON_START,			"button_queststart");

	
	CImageStatic* pImage = m_InterfaceFactory.GetImageStatic( NAVATARQUEST_IMAGE_PORTRAIT );
	if ( pImage )
	{
		TCHAR szPortrait[256];
		SI32 siPortraitFont;
		pClient->pclKindInfoSet->GetNPCImageFileAndFont(pClient->GetUniqueFromHash( TEXT("KIND_PANG") ), szPortrait, sizeof(szPortrait), &siPortraitFont);
		pImage->SetFileName(szPortrait);
		pImage->SetFontIndex(siPortraitFont);
	}

	CEdit* pEdit = m_InterfaceFactory.GetEdit( NAVATARQUEST_EDIT_EXPLAIN );
	if ( pEdit )
	{
		TCHAR* pText = GetTxtFromMgr( 40135  );
		pEdit->SetText( pText );
	}


	CList* pList = m_InterfaceFactory.GetList(NAVATARQUEST_LIST_SELECT);

	if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 601 );
		pList->SetColumn(0, 200, pText);
		pList->SetColumn(1, 120, GetTxtFromMgr(10347));
		
		pList->Refresh();

		
		stListBoxItemData tempItemData;

		SI32 tempclearnum = 0;
		for ( SI32 i = 0; i < 5; ++i )
		{
			NTCHARString64	strCurrentSteps(GetTxtFromMgr(10348));

			// [검색 : 황진성 2007. 12. 6 퀘스트 진행 순서(번호) 얻어오기] 
			tempclearnum = pClient->pclCM->CR[1]->pclCI->clQuestInfo.GetClearQuestNum(pClient->pclQuestManager, 46+i );				
			
			NTCHARString64 strMsg;

			if(pClient->pclCM->CR[1]->pclCI->clQuestInfo.IsAllClear(pClient->pclQuestManager, 46+i))
			{
				// 현재 퀘스트를 클리어 했다면 해당 string 출력

				switch(i)
				{
				case 0:	strMsg = GetTxtFromMgr(40157);	break;	// 의복
				case 1:	strMsg = GetTxtFromMgr(40158);	break;	// 모자
				case 2:	strMsg = GetTxtFromMgr(40159);	break;	// 망토
				case 3:	strMsg = GetTxtFromMgr(40160);	break;	// 목걸이
				case 4:	strMsg = GetTxtFromMgr(40161);	break;	// 반지
				default:break;
				}
			}
			else
			{
				switch(i)
				{
				case 0:	strMsg = GetTxtFromMgr(40152);	break;	// 의복
				case 1:	strMsg = GetTxtFromMgr(40153);	break;	// 모자
				case 2:	strMsg = GetTxtFromMgr(40154);	break;	// 망토
				case 3:	strMsg = GetTxtFromMgr(40155);	break;	// 목걸이
				case 4:	strMsg = GetTxtFromMgr(40156);	break;	// 반지
				default:break;
				}
			}

			tempItemData.Set(0, strMsg);

			strCurrentSteps.Replace(TEXT("#value#"), SI32ToString(tempclearnum));
			strCurrentSteps.Replace(TEXT("#maxvalue#"), SI32ToString(pClient->pclQuestManager->GetQuestNum(46+i)));
			tempItemData.Set(1, strCurrentSteps);
			
			pList->AddItem(&tempItemData);
		}
		
	}
	pList->Refresh();

	if(pClient->m_pDialog[ NNEWQUEST_DLG ] != NULL)
	{
		((CNNewQuestDlg*)pClient->m_pDialog[ NNEWQUEST_DLG ])->Show( false );
		pClient->CreateInterface( NNEWQUEST_DLG );
	}
	Show(true);
}

void CALLBACK CNAvatarQuestDlg::StaticNAvatarQuestDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNAvatarQuestDlg *pThis = (CNAvatarQuestDlg*) pControllerMgr;
	pThis->NAvatarQuestDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNAvatarQuestDlg::NAvatarQuestDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NAVATARQUEST_LIST_SELECT:
		{
			if ( EVENT_LISTBOX_SELECTION == nEvent )
			{
				CList* pList = m_InterfaceFactory.GetList( NAVATARQUEST_LIST_SELECT );
				if ( pList == NULL ) break;

				m_siSelectedIndex = pList->GetSelectedIndex();
			}
		}
		break;
	case NAVATARQUEST_BUTTON_START:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				StartAvatarQuest();
				DeleteDialog();
			}
		}

	}
}


void CNAvatarQuestDlg::StartAvatarQuest()
{
	cltClient* pclclient = (cltClient*)pclClient;

	SI32 siQuestType			= 0;

	switch(m_siSelectedIndex)
	{
	case 0:		siQuestType = QUEST_TYPE_AVATAR_DRESS;		break;
	case 1:		siQuestType = QUEST_TYPE_AVATAR_HAT;		break;
	case 2:		siQuestType = QUEST_TYPE_AVATAR_MENTAL;		break;
	case 3:		siQuestType = QUEST_TYPE_AVATAR_RING;		break;
	case 4:		siQuestType = QUEST_TYPE_AVATAR_NECK;		break;
	}

	if(pclclient->m_pDialog[ NNEWQUEST_DLG ] == NULL)
	{
		pclclient->CreateInterface( NNEWQUEST_DLG );

		if(pclclient->m_pDialog[ NNEWQUEST_DLG ])
		{
			pclclient->pclQuestManager->bNewQuseOpened = TRUE;
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_siButtonType = 0;
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->m_siQuestType_Rudolph = siQuestType;
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetNewQuest( pclClient->GetUniqueFromHash( TEXT("KIND_PANG")) );
			((CNNewQuestDlg*)pclclient->m_pDialog[ NNEWQUEST_DLG ])->SetOpenedQuestDlg( true );
		}
	}

}


