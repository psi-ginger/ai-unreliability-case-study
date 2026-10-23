#include "NCityhallDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "..\..\Client\Music\Music.h"

#include "../Lib/JYLibFunction.h"

#include "../../InterfaceMgr/InterfaceMgr.h"

#include "Msg/MsgType-Empire.h"
#include "../../../Server/Empire/EmpireManager.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NCityhallUserMenuEmpireDlg::NCityhallUserMenuEmpireDlg()
{
	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		m_siVillageUnique[i] = 0;
	}
}

NCityhallUserMenuEmpireDlg::~NCityhallUserMenuEmpireDlg()
{

}

void NCityhallUserMenuEmpireDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( COMBOBOX_VILLAGE, TEXT("combobox_NONAME1") );
	SetControlMap( LIST_VILLAGE, TEXT("listbox_NONAME1") );
	SetControlMap( LIST_GUILD, TEXT("listbox_NONAME2") );
	
	TCHAR* pText[3];
	pText[0] = GetTxtFromMgr( 10205 );
	pText[1] = GetTxtFromMgr( 10206 );
	pText[2] = GetTxtFromMgr( 10207 );
	
	CList* villageList = (CList*)m_pControlMap[ LIST_VILLAGE ];
	villageList->SetColumn( 0, 120,  pText[ 0 ] );
	villageList->SetColumn( 1, 120, pText[ 1 ] );
	villageList->SetColumn( 2, 100, pText[ 2 ] );
	villageList->SetBorder(true);
	villageList->SetBKColorUse(true);
	villageList->Refresh();

	pText[0] = GetTxtFromMgr( 10208 );
	pText[1] = GetTxtFromMgr( 10209 );
	pText[2] = GetTxtFromMgr( 10210 );

	CList* guildList = (CList*)m_pControlMap[ LIST_GUILD ];
	guildList->SetColumn( 0, 120, pText[ 0 ] );
	guildList->SetColumn( 1, 120, pText[ 1 ] );
	guildList->SetColumn( 2, 100, pText[ 2 ] );
	guildList->SetBorder(true);
	guildList->SetBKColorUse(true);
	guildList->Refresh();

	cltGameMsgRequest_EmpireInfo clEmpireInfo( true, false, 0 );
	cltMsg clMsg( GAMEMSG_REQUEST_EMPIREINFO, sizeof(clEmpireInfo), (BYTE *)&clEmpireInfo );
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NCityhallUserMenuEmpireDlg::Action()
{
}

void NCityhallUserMenuEmpireDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	CASE_CONTROL( COMBOBOX_VILLAGE )
	{
		switch( nEvent )
		{
		case EVENT_COMBOBOX_SELECTION_CHANGED:
			{
				CComboBox* villageComboBox = (CComboBox*)m_pControlMap[ COMBOBOX_VILLAGE ];

				SI16 selectIndex = villageComboBox->GetSelectedIndex();
				if( -1 < selectIndex )
				{
					if( 0 < m_siVillageUnique[selectIndex] )
					{
						cltGameMsgRequest_EmpireInfo clEmpireInfo( false, false, m_siVillageUnique[selectIndex] );
						cltMsg clMsg( GAMEMSG_REQUEST_EMPIREINFO, sizeof(clEmpireInfo), (BYTE *)&clEmpireInfo );
						pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
					}
				}
			}
			break;
		}
	}
}

void NCityhallUserMenuEmpireDlg::SetEmpireMemberList( cltGameMsgResponse_EmpireInfo* EmpireUnitInfo )
{
	if( true == EmpireUnitInfo->bRequestFirst )
	{
		CComboBox* villageComboBox = (CComboBox*)m_pControlMap[ COMBOBOX_VILLAGE ];

		SI32 i;
		stComboBoxItemData comboBoxData;
		for( i = 0 ; i < MAX_EMPIRENUMBER; ++i )
		{
			if( 0 < EmpireUnitInfo->siEmperprVillageUnique[i] )
			{
				m_siVillageUnique[i] = EmpireUnitInfo->siEmperprVillageUnique[i];
				TCHAR* villageName = pclClient->pclVillageManager->GetName( m_siVillageUnique[i] );
				comboBoxData.Set(villageName);
				villageComboBox->AddItem( &comboBoxData );

			}
		}

		if( 0 < villageComboBox->GetItemNum() )
			villageComboBox->SetCursel( 0 );

		CList* villageList = (CList*)m_pControlMap[ LIST_VILLAGE ];
		villageList->Clear();

		stListBoxItemData listBoxData;
		TCHAR tempBuffer[20] = {'\0'};
		for( SI32 i = 0; i < MAX_EMPIRE_IN_VILLAGE; ++i )
		{
			SI32 villageUnique = EmpireUnitInfo->clEmpireInfo.GetVillageUnique(i);
			if( 0 < villageUnique )
			{
				TCHAR* villageName = pclClient->pclVillageManager->GetName( villageUnique );
				listBoxData.Set( 0, villageName );
				listBoxData.Set( 1, EmpireUnitInfo->szVillageChiefName[i] );
				StringCchPrintf( tempBuffer, 20, "%d", EmpireUnitInfo->siVillageMemberNumber[i] );
				listBoxData.Set( 2, tempBuffer );
				villageList->AddItem( &listBoxData );
			}
		}
		villageList->Refresh();

		CList* guildList = (CList*)m_pControlMap[ LIST_GUILD ];
		guildList->Clear();

		listBoxData.Init();
		for( SI32 i = 0; i < MAX_EMPIRE_IN_GUILD; ++i )
		{
			if( 0 < EmpireUnitInfo->clEmpireInfo.GetGuildUnique(i) )
			{
				TCHAR* guildName = pclClient->pclGuildManager->GetGuildName( EmpireUnitInfo->clEmpireInfo.GetGuildUnique(i) );
				listBoxData.Set( 0, guildName );
				listBoxData.Set( 1, EmpireUnitInfo->szGuildChiefName[i] );
				StringCchPrintf( tempBuffer, 20, "%d", EmpireUnitInfo->siGuildMemberNumber[i] );
				listBoxData.Set( 2, tempBuffer );
				guildList->AddItem( &listBoxData );
			}
		}
		guildList->Refresh();
	}
	else
	{
		CList* villageList = (CList*)m_pControlMap[ LIST_VILLAGE ];
		villageList->Clear();

		stListBoxItemData listBoxData;

		TCHAR tempBuffer[20] = {'\0'};
		for( SI32 i = 0; i < MAX_EMPIRE_IN_VILLAGE; ++i )
		{
			if( 0 < EmpireUnitInfo->clEmpireInfo.GetVillageUnique(i) )
			{
				TCHAR* villageName = pclClient->pclVillageManager->GetName( EmpireUnitInfo->clEmpireInfo.GetVillageUnique(i) );
				listBoxData.Set( 0, villageName );
				listBoxData.Set( 1, EmpireUnitInfo->szVillageChiefName[i] );
				StringCchPrintf( tempBuffer, 20, "%d", EmpireUnitInfo->siVillageMemberNumber[i] );
				listBoxData.Set( 2, tempBuffer );
				villageList->AddItem( &listBoxData );
			}
		}
		villageList->Refresh();

		CList* guildList = (CList*)m_pControlMap[ LIST_GUILD ];
		guildList->Clear();

		listBoxData.Init();
		for( SI32 i = 0; i < MAX_EMPIRE_IN_GUILD; ++i )
		{
			if( 0 < EmpireUnitInfo->clEmpireInfo.GetGuildUnique(i) )
			{
				TCHAR* guildName = pclClient->pclGuildManager->GetGuildName( EmpireUnitInfo->clEmpireInfo.GetGuildUnique(i) );
				listBoxData.Set( 0, guildName );
				listBoxData.Set( 1, EmpireUnitInfo->szGuildChiefName[i] );
				StringCchPrintf( tempBuffer, 20, "%d", EmpireUnitInfo->siGuildMemberNumber[i] );
				listBoxData.Set( 2, tempBuffer );
				guildList->AddItem( &listBoxData );
			}
		}
		guildList->Refresh();
	}
}

