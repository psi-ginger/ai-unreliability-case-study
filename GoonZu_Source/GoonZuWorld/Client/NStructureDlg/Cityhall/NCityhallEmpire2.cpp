#include "NCityhallDlg.h"
#include "NInviteEmpire.h"

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

NCityhallChiefMenuEmpireDlg::NCityhallChiefMenuEmpireDlg()
{
}

NCityhallChiefMenuEmpireDlg::~NCityhallChiefMenuEmpireDlg()
{

}

void NCityhallChiefMenuEmpireDlg::InitDlg()
{
	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	SetControlMap( BUTTON_TAX,	TEXT("button_NONAME1") );
	SetControlMap( BUTTON_CREATEEMPIRE,	TEXT("button_NONAME2") );
	SetControlMap( BUTTON_INVITEEMPIRE, TEXT("button_NONAME3") );
	SetControlMap( BUTTON_SECEDEEMPIRE,	TEXT("button_NONAME4") );

	SetControlMap( LIST_TAX,	 TEXT("listbox_NONAME1") );
	SetControlMap( LIST_VILLAGE, TEXT("listbox_NONAME2") );
	SetControlMap( LIST_GUILD,	 TEXT("listbox_NONAME3") );

	SetControlMap( EDIT_TAX,	 TEXT("editbox_NONAME3") );

	TCHAR* pText[3];

	pText[0] = GetTxtFromMgr( 10217 );
	pText[1] = GetTxtFromMgr( 10218 );
	pText[2] = GetTxtFromMgr( 10219 );
	CList* taxList = (CList*)m_pControlMap[ LIST_TAX ];
	taxList->SetColumn( 0, 120,  pText[ 0 ] );
	taxList->SetColumn( 1, 120, pText[ 1 ] );
	taxList->SetColumn( 2, 100, pText[ 2 ] );
	taxList->SetBorder(true);
	taxList->SetBKColorUse(true);
	taxList->Refresh();

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

	cltGameMsgRequest_EmpireInfo clEmpireInfo( false, true, 0 );
	cltMsg clMsg( GAMEMSG_REQUEST_EMPIREINFO, sizeof(clEmpireInfo), (BYTE *)&clEmpireInfo );
	pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
}

void NCityhallChiefMenuEmpireDlg::Action()
{
}

void NCityhallChiefMenuEmpireDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;

	cltCityHallManager *pCityhallManager = ((cltCityHallManager *)m_pStrManager);

	CASE_CONTROL( BUTTON_TAX )
	{
		CEdit* edit = (CEdit*)m_pControlMap[ EDIT_TAX ];

		SI64 inputMoney = _atoi64( edit->GetText() );

		if( inputMoney > pCityhallManager->clClientStrInfo.clCityhallStrInfo.clProfitMoney.itMoney )
		{
			/// 이익금이 부족하다
			return;
		}
		cltGameMsgRequest_SetEmpireMoney clSetEmpireMoney( inputMoney );
		cltMsg clMsg( GAMEMSG_REQUEST_SETEMPIREMONEY, sizeof(clSetEmpireMoney), (BYTE*)&clSetEmpireMoney );
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else CASE_CONTROL( BUTTON_CREATEEMPIRE )
	{
		cltMsg clMsg( GAMEMSG_REQUEST_CREATEEMPIRE, 0, NULL );
		pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else CASE_CONTROL( BUTTON_INVITEEMPIRE )
	{
		pclclient->CreateInterface( NINVITEEMPIRE_DLG );
	}
	else CASE_CONTROL( BUTTON_SECEDEEMPIRE )
	{
		CList* villageList = (CList*)m_pControlMap[ LIST_VILLAGE ];
		if( 1 <= villageList->GetSelectedIndex() )
		{
			TCHAR* villageName = villageList->GetText( villageList->GetSelectedIndex(), 0 );

			if( NULL == villageName )
			{
				/// 없는 마을
				return;
			}
			
			SI32 villageUnique = pclClient->pclVillageManager->GetMapIndexFromName( villageName );
			cltGameMsgRequest_SecedeEmpire clSecedeEmpire( cltGameMsgRequest_SecedeEmpire::EMPEROR, villageUnique, 0);
			cltMsg clMsg( GAMEMSG_REQUEST_SECEDEEMPIRE, sizeof(clSecedeEmpire), (BYTE*)&clSecedeEmpire );
			pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			/// 맹주국이면 스스로 탈퇴가 안 된다.
		}
	}
}

void NCityhallChiefMenuEmpireDlg::SetEmpireMemberList( cltGameMsgResponse_EmpireInfo* EmpireUnitInfo )
{
	stListBoxItemData listBoxData;
	TCHAR tempBuffer[64] = {'\0'};

	CList* taxList = (CList*)m_pControlMap[ LIST_TAX ];
	taxList->Clear();

	for( SI32 i = 0; i < MAX_EMPIRE_IN_VILLAGE; ++i )
	{
		if( 0 < EmpireUnitInfo->clEmpireInfo.GetVillageUnique(i) )
		{
			TCHAR* villageName = pclClient->pclVillageManager->GetName( EmpireUnitInfo->clEmpireInfo.GetVillageUnique(i) );
			listBoxData.Set( 0, villageName );
			StringCchPrintf( tempBuffer, 64, "%I64d", EmpireUnitInfo->clEmpireInfo.GetInputTotalEmpireBaseMoney(i) );
			listBoxData.Set( 1, tempBuffer );
			taxList->AddItem( &listBoxData );
		}
	}

	taxList->Refresh();

	CList* villageList = (CList*)m_pControlMap[ LIST_VILLAGE ];
	villageList->Clear();

	for( SI32 i = 0; i < MAX_EMPIRE_IN_VILLAGE; ++i )
	{
		if( 0 < EmpireUnitInfo->clEmpireInfo.GetVillageUnique(i) )
		{
			TCHAR* villageName = pclClient->pclVillageManager->GetName( EmpireUnitInfo->clEmpireInfo.GetVillageUnique(i) );
			listBoxData.Set( 0, villageName );
			listBoxData.Set( 1, EmpireUnitInfo->szVillageChiefName[i] );
			StringCchPrintf( tempBuffer, 64, "%d", EmpireUnitInfo->siVillageMemberNumber[i] );
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
			StringCchPrintf( tempBuffer, 64, "%d", EmpireUnitInfo->siGuildMemberNumber[i] );
			listBoxData.Set( 2, tempBuffer );
			guildList->AddItem( &listBoxData );
		}
	}
	guildList->Refresh();

	/// 제국 자본금 보여주기
	CEdit* edit = (CEdit*)m_pControlMap[ EDIT_TAX ];
	TCHAR buffer[64] = {'\0'};
	StringCchPrintf( buffer, 64, "%I64d", EmpireUnitInfo->clEmpireInfo.GetEmpireBaseMoney() );
	edit->SetText( buffer );
}


