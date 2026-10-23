#include "NSelectNamingDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../../Common/Quest/NamingQuest.h"
#include "Char\CharManager\CharManager.h"

#include "../../CommonLogic/Msg/MsgType-NamingQuest.h"

extern cltCommonLogic* pclClient;

CNSelectNamingDlg::CNSelectNamingDlg()
{	
	Initialize();
}

CNSelectNamingDlg::~CNSelectNamingDlg()
{
	Destroy();
}


void CNSelectNamingDlg::Init()
{
}

void CNSelectNamingDlg::Destroy()
{

}

void CNSelectNamingDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NNamingQuest/DLG_NSelectNaming.ddf"));
	file.CreatePopupDialog( this, NSELECTNAMING_DLG , TEXT("dialog_selectnaming"), StaticSelectNamingDlgProc );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NSELECTNAMING_LIST_NAMING , this ), NSELECTNAMING_LIST_NAMING,   "listbox_naming"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NSELECTNAMING_BUTTON_RESET , this ), NSELECTNAMING_BUTTON_RESET,   "button_reset"); 
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NSELECTNAMING_BUTTON_SELECT , this ), NSELECTNAMING_BUTTON_SELECT,   "button_select"); 

    CList* pList = m_InterfaceFactory.GetList(NSELECTNAMING_LIST_NAMING);

	if ( pList )
	{
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 10094 );
		pList->SetColumn(0, 140, pText);
		pText = GetTxtFromMgr( 1506 );
		pList->SetColumn(1, 60, pText);
		pList->Refresh();

		stListBoxItemData tempItemData;

		for ( SI32 i=0; i < MAX_NAMINGQUEST_INDEX; ++i )
		{
			SI32 currLevel = pclClient->pclCM->CR[1]->pclCI->clPersonNaming.m_siLevel[i];

			if ( currLevel > 0 )
			{
				//SI32 maxLevel = pclClient->pclNamingQuestManager->GetMaxLevel(i);
				for ( SI32 level = currLevel ; level >= 1; --level )
				{
					tempItemData.Init();

					cltNamingQuestUnit* pclUnit = pclClient->pclNamingQuestManager->GetQuestInfo( i, level );

					tempItemData.Set( 0, pclUnit->GetNaming(), i );
					
					TCHAR szLevel[8] = {0,};
					StringCchPrintf( szLevel, sizeof( szLevel ), TEXT("%d"), level );
					tempItemData.Set( 1, szLevel, level );

					pList->AddItem( &tempItemData );
				}
			}
		}
	}

	pList->Refresh();

	Show(true);
}

void CALLBACK CNSelectNamingDlg::StaticSelectNamingDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNSelectNamingDlg *pThis = (CNSelectNamingDlg*) pControllerMgr;
	pThis->NSelectNamingDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNSelectNamingDlg::NSelectNamingDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NSELECTNAMING_BUTTON_RESET:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				SelectNaming( 0, 0 );
				DeleteDialog();
			}
		}
		break;
	case NSELECTNAMING_BUTTON_SELECT:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				CList* pList = m_InterfaceFactory.GetList(NSELECTNAMING_LIST_NAMING);
				if( pList == NULL )
					break;

				SI16 selectedIndex = pList->GetSelectedIndex();
				if ( selectedIndex > -1 )
				{
					SI32 index = pList->GetParam( selectedIndex, 0 );
					SI32 level = pList->GetParam( selectedIndex, 1 );

					SelectNaming( index, level );
				}
				DeleteDialog();
			}
		}
		break;
	}
}

void CNSelectNamingDlg::SelectNaming( SI32 Index, SI32 Level )
{
	cltGameMsgRequest_SetPeronCurrentNaming clInfo( Index, Level );
	cltMsg clMsg( GAMEMSG_REQUEST_SETPERSONCURRENTNAMING, sizeof( clInfo ), (BYTE*)&clInfo );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}
