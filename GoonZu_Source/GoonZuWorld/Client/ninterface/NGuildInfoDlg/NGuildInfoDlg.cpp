

#include "../../Client.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "NGuildInfoDlg.h"
#include "NGuildSendMsg.h"

#include "../../common/Char/CharManager/CharManager.h"


#include "MsgType-Guild.h"


extern cltCommonLogic* pclClient;

CNGuildInfoDlg::CNGuildInfoDlg()
{
	Initialize();
}	

CNGuildInfoDlg::~CNGuildInfoDlg()
{
	Destroy();
}

void CNGuildInfoDlg::Initialize()
{

}

void CNGuildInfoDlg::Destroy()
{

}

void CNGuildInfoDlg::Create()
{
	cltClient *pclclient = (cltClient *)pclClient;

	//TCHAR* pTitle = GetTxtFromMgr(6584);
	//TCHAR* pText = GetTxtFromMgr(6655);

	//pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pTitle, pText );


	//return;



	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NGuildInfo/DLG_NGuildInfo.ddf"));

		file.CreatePopupDialog( this, NGUILDINFO_DLG, TEXT("dialog_GuildInfo"),StaticMyGuildInfoDlgProc );

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( COMBOBOX ,NMYGUILDINFO_COMBO_VILLAGENAME, this)  , NMYGUILDINFO_COMBO_VILLAGENAME, TEXT("combobox_VillageName"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST , NMYGUILDINFO_LIST_GUILD, this)  , NMYGUILDINFO_LIST_GUILD , TEXT("listbox_NONAME1"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDINFO_BUTTON_SENDMSGTOMASTER , this)  , NMYGUILDINFO_BUTTON_SENDMSGTOMASTER , TEXT("button_NONAME1"));
		


		// 리스트 
		CList *pList = m_InterfaceFactory.GetList(NMYGUILDINFO_LIST_GUILD);

 		pList->SetBorder(true);
		pList->SetBKColorUse(true);


 		TCHAR* pText = GetTxtFromMgr(1695);
		pList->SetColumn( 0, 20, pText );		// 순서

 		pText = GetTxtFromMgr(6548);
		pList->SetColumn( 1, 80, pText );		// 길드명
		
		pText = GetTxtFromMgr( 6550 );
		pList->SetColumn( 2, 30, pText );		// 길드 레벨

		pText = GetTxtFromMgr( 6540 );
		pList->SetColumn( 3, 70, pText );
		
		pText = GetTxtFromMgr(6499);
		pList->SetColumn( 4, 100, pText );		// 소유사냥터


		
		pList->Refresh();


		cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETVILLAGELIST, 0);
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);


		if(m_InterfaceFactory.GetComboBox(NMYGUILDINFO_COMBO_VILLAGENAME)->GetItemNum() > 0)
			m_InterfaceFactory.GetComboBox(NMYGUILDINFO_COMBO_VILLAGENAME)->SetCursel( 0 );



		SI32 rankType = pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetRankType();

		if ( rankType != GUILDRANKTYPE_MASTER && rankType != GUILDRANKTYPE_SECONDMASTER )
		{
			m_InterfaceFactory.GetButton(NMYGUILDINFO_BUTTON_SENDMSGTOMASTER )->Show( false );
		}
	}


}

void CALLBACK CNGuildInfoDlg::StaticMyGuildInfoDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildInfoDlg *pThis = (CNGuildInfoDlg*) pControllerMgr;
	pThis->NMyGuildInfoDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNGuildInfoDlg::NMyGuildInfoDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
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
	case NMYGUILDINFO_COMBO_VILLAGENAME:
		{
			switch( nEvent )
			{
			case EVENT_COMBOBOX_SELECTION_CHANGED:
				{
                    TCHAR * pText = NULL;
					CComboBox * pCombo = m_InterfaceFactory.GetComboBox(NMYGUILDINFO_COMBO_VILLAGENAME);
					CList * pList = m_InterfaceFactory.GetList(NMYGUILDINFO_LIST_GUILD);

					pList->Clear();

					pText = pCombo->GetText(pCombo->GetSelectedIndex());
					
					if(pText == NULL) break;

					SI32 VillageUnique = pclClient->pclVillageManager->FindVillageUniqueFromName(pText);
					
					if(VillageUnique < 0) break;

					cltGameMsgRequest_Guild_GetGuildList clinfo(VillageUnique,NGUILDINFO_DLG);
					cltMsg clMsg(GAMEMSG_REQUEST_GUILD_GETGUILDLIST,sizeof(clinfo),(BYTE*)&clinfo);
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}
				break;
			}	
		}
		break;
	case NMYGUILDINFO_BUTTON_SENDMSGTOMASTER:
		{
			if ( EVENT_BUTTON_CLICKED == nEvent )
			{
				CList * pList = m_InterfaceFactory.GetList(NMYGUILDINFO_LIST_GUILD);

				SI16 selectedIndex = pList->GetSelectedIndex();

				if ( selectedIndex < 0 ) return;

				TCHAR* masterName = pList->GetText( selectedIndex, 3 );

				pclclient->CreateInterface( NGUILDSENDMSG_DLG );

				if ( pclclient->m_pDialog[NGUILDSENDMSG_DLG] )
				{
					((CNGuildSendMsgDlg*)pclclient->m_pDialog[NGUILDSENDMSG_DLG])->SetMasterName(masterName);
				}

			}
		}
		break;
	}
}


void CNGuildInfoDlg::Hide()
{
	CControllerMgr::Show(false);

}

void CNGuildInfoDlg::Show()
{
	CControllerMgr::Show(true);

}

void CNGuildInfoDlg::SetComboBoxItem(cltGameMsgReponse_Guild_VillageList* pclinfo )
{
	CComboBox *pCombo = m_InterfaceFactory.GetComboBox(NMYGUILDINFO_COMBO_VILLAGENAME);
	
	stComboBoxItemData tmpComboBoxItemData;

	// 콤보 박스를 지운다.
	pCombo->Clear();
		
	for( SI32 i = 0; i < MAX_VILLAGE_NUMBER; i++)
	{
		if(pclClient->pclVillageManager->pclVillageInfo[i] == NULL)continue;

		if( pclinfo->bBuilt[i] )
		{
			tmpComboBoxItemData.Init();
			tmpComboBoxItemData.Set( pclClient->pclVillageManager->pclVillageInfo[i]->szName );

			pCombo->AddItem( &tmpComboBoxItemData );
		}
	}

	pCombo->Refresh();
}

void CNGuildInfoDlg::SetList(cltGameMsgResponse_Guild_GetGuildList* pclinfo)
{
 	CList * pList = m_InterfaceFactory.GetList(NMYGUILDINFO_LIST_GUILD);

	pList->Clear();

	stListBoxItemData tmpListBoxItemData;
	TCHAR szBuffer[256] = {'\0', };
	tmpListBoxItemData.Init();
	SI32 index = 1;

 	for(SI32 i = 0; i < MAX_GUILD_PER_VILLAGE; i++)	
	{
		if( pclinfo->siUnique[i] <= 0 ) continue;

		StringCchPrintf(szBuffer,256,TEXT("%d"), index++ );
		tmpListBoxItemData.Set( 0, szBuffer );

		StringCchPrintf(szBuffer,256,TEXT("%s"), pclinfo->szName[i] );
		tmpListBoxItemData.Set( 1, szBuffer );

		StringCchPrintf(szBuffer,256,TEXT("%d"), pclinfo->siLevel[i] );
		tmpListBoxItemData.Set( 2, szBuffer );

		StringCchPrintf(szBuffer,256,TEXT("%s"), pclinfo->szMasterName[i] );
		tmpListBoxItemData.Set( 3, szBuffer );

		if ( pclinfo->siOwnedMapIndex[i] > 0 )
		{
			StringCchPrintf(szBuffer,256,TEXT("%s"), pclClient->pclMapManager->GetMapName( pclinfo->siOwnedMapIndex[i]) );
		}
		else
		{
			StringCchCopy(szBuffer, 256, TEXT("") );
		}
		tmpListBoxItemData.Set( 4, szBuffer );

		pList->AddItem(&tmpListBoxItemData);	

	}

	pList->Refresh();
}


