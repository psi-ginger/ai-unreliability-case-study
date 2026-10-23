

#include "../../../Client.h"
#include "../../../InterfaceMgr/InterfaceFile.h"
#include "../../../InterfaceMgr/InterfaceMgr.h"
#include "MyGuild.h"

#include "../../../../CommonLogic/Guild/Guild.h"

#include "../../../../common/Char/CharManager/CharManager.h"

#include <algorithm>
using std::sort;

extern cltCommonLogic* pclClient;

CNMyGuild::CNMyGuild()
{
	Initialize();
}	

CNMyGuild::~CNMyGuild()
{
	Destroy();
}

void CNMyGuild::Initialize()
{	

}

void CNMyGuild::Destroy()
{

}

void CNMyGuild::Create( CControllerMgr *pParent )
{

	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
		{
			file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuild_New.ddf"));
		}
		else
		{
			file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuild.ddf"));			
		}
		
		file.CreateChildDialog( this, NMYGUILD_DLG, TEXT("dialog_MyGuild"), StaticMyGuildDlgProc, pParent);

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC   ,NMYGUILD_STATIC_1 , this)  , STATIC , TEXT("statictext_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC   ,NMYGUILD_STATIC_2 , this)  , STATIC , TEXT("statictext_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC   ,NMYGUILD_STATIC_3 , this)  , STATIC , TEXT("statictext_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC   ,NMYGUILD_STATIC_4 , this)  , STATIC , TEXT("statictext_NONAME4"));
		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
		{
			file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC   ,NMYGUILD_STATIC_5 , this)  , STATIC , TEXT("statictext_NONAME5"));
		}
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT	,NMYGUILD_EDIT_1 , this)  , EDIT , TEXT("editbox_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT	,NMYGUILD_EDIT_2 , this)  , EDIT , TEXT("editbox_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT	,NMYGUILD_EDIT_3 , this)  , EDIT , TEXT("editbox_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT	,NMYGUILD_EDIT_4 , this)  , EDIT , TEXT("editbox_NONAME4"));
		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
		{
			file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT	,NMYGUILD_EDIT_5 , this)  , EDIT , TEXT("editbox_NONAME5"));
		}		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST   ,NMYGUILD_LIST , this)  , NMYGUILD_LIST , TEXT("listbox_NONAME1"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NMYGUILD_BUTTON_CHECK , this)  , NMYGUILD_BUTTON_CHECK , TEXT("button_Check"));


		// 리스트 
		CList *pList = m_InterfaceFactory.GetList(NMYGUILD_LIST);

		pList->SetBorder(true);
		pList->SetBKColorUse(true);

        TCHAR * pText = GetTxtFromMgr(5248);
		pList->SetColumn( 0, 80, pText  );

		pText = GetTxtFromMgr(6550);
		pList->SetColumn( 1, 35, pText );      

		pText = GetTxtFromMgr(5271);
		pList->SetColumn( 2, 80, pText );

		pText = GetTxtFromMgr(6598);
		pList->SetColumn( 3, 45, pText );
		
		pText = GetTxtFromMgr(6501);
		pList->SetColumn( 4, 80, pText );

		pText = GetTxtFromMgr(6509);
		pList->SetColumn( 5, 30, pText );

		pList->Refresh();
	}

}

void CALLBACK CNMyGuild::StaticMyGuildDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMyGuild *pThis = (CNMyGuild*) pControllerMgr;
	pThis->NMyGuildDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNMyGuild::NMyGuildDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case NMYGUILD_BUTTON_CHECK:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{	
					CEdit* pEdit = m_InterfaceFactory.GetEdit(NMYGUILD_EDIT_4);
					TCHAR* pData = pEdit->GetText() ;
					GMONEY money = _tstoi64( pData );

					if ( money <= 0 )
						break;

					if ( money > pclClient->pclCM->CR[1]->pclCI->clIP.GetMoney() )
					{
						TCHAR* pTitle = GetTxtFromMgr(1260);
						TCHAR* pText = GetTxtFromMgr(1367);

						pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
						break;
					}

					TCHAR buffer[128] = "" ;
					TCHAR* pTitle = GetTxtFromMgr(6501);
					TCHAR* pText = GetTxtFromMgr(6538);
					sprintf(buffer,pText,money);

					stMsgBox MsgBox;
					CControllerMgr *pParent = NULL;
					pParent = this->GetParentDialog();
					if(pParent == NULL ) return;
					MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 0 );

					BYTE TempBuffer[ 128 ];
					SI16 Size = sizeof( money );
					memcpy( TempBuffer, &money, sizeof( money ) );
					pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
				}
				break;
			}
		}
		break;
	}
}


void CNMyGuild::Hide()
{
	CControllerMgr::Show(false);

}

void CNMyGuild::Show()
{
	CControllerMgr::Show(true);

}

// [추가 : 황진성 2008. 2. 14 => 길드원 정보를 보일때 레벨로 정렬을 위한 구조체.]
struct compare_Guild_Member_Sort
{
	bool operator() ( cltGuildMember& _Value1, cltGuildMember& _Value2)
	{
		return _Value1.siGP > _Value2.siGP;
	}					
};


void CNMyGuild::RefreshList(cltGuildInfo guildinfo)
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);
	
    cltGuildInfo* pGuild = pGuildManager->GetGuildInfo(pclClient->pclCM->CR[1]->pclCI->clBI.clGuildRank.GetGuildUnitIndex());
	stListBoxItemData itemdata;
	CList *pList = m_InterfaceFactory.GetList(NMYGUILD_LIST);

	TCHAR buffer[256];

	// 리스트뷰를 모두 지운다. 
	pList->Clear();

	// [추가 : 황진성 2008. 2. 14 => 길드원 정보를 보일때 레벨로 정렬.]
	sort( &pGuildManager->clCommonUnit.clMember[0], &pGuildManager->clCommonUnit.clMember[MAX_GUILD_MEMBER_NUM-1], compare_Guild_Member_Sort() );

	for( SI32 i = 0; i < MAX_GUILD_MEMBER_NUM; i++ )
	{
		itemdata.Init();

		if( pGuildManager->clCommonUnit.clMember[i].siPersonID != 0 )
		{
			itemdata.Set( 0, (TCHAR*)pGuildManager->clCommonUnit.clMember[i].szCharName );


			StringCchPrintf(buffer, 256, TEXT("%d"), pGuildManager->clCommonUnit.clMember[i].m_siLevel );
			itemdata.Set( 1, buffer );

			if ( pGuildManager->clCommonUnit.clMember[i].siType == GUILDRANKTYPE_MASTER )
			{
				TCHAR * pText = GetTxtFromMgr(6540);
				itemdata.Set( 2, pText );
			}
			else if ( pGuildManager->clCommonUnit.clMember[i].siType == GUILDRANKTYPE_SECONDMASTER )
			{
				TCHAR * pText = GetTxtFromMgr(6541);
				itemdata.Set( 2, pText );
			}
			else
			{
				TCHAR * pText = GetTxtFromMgr(6542);
				itemdata.Set( 2, pText );
			}

			StringCchPrintf(buffer, 256, TEXT("%d"), pGuildManager->clCommonUnit.clMember[i].siGP );
			itemdata.Set(3,buffer);

			StringCchPrintf(buffer, 256, TEXT("%I64d"), pGuildManager->clCommonUnit.clMember[i].siMoney );
			itemdata.Set(4,buffer);

			pList->AddItem( &itemdata );
		}
	}

	pList->Refresh();
	
	CEdit* pLevelEdit = m_InterfaceFactory.GetEdit(NMYGUILD_EDIT_1);
	StringCchPrintf(buffer, 256, TEXT("[%d] Level([%d] GP)"), guildinfo.clGPoint.siLevel,guildinfo.clGPoint.siPoint );
	pLevelEdit->SetText(buffer);

	CEdit* pEditMoney = m_InterfaceFactory.GetEdit(NMYGUILD_EDIT_3);
	sprintf(buffer,"[%I64d]",pGuildManager->clCommonUnit.clGPoint.siMoney);
	pEditMoney->SetText(buffer);


	// 길드 BGP
	//	[종호] BGP 관련 EditBox 생성때만 스위치가 걸려있어서 여기도 추가
	if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar) )
	{
		CEdit* pEditBGP = m_InterfaceFactory.GetEdit(NMYGUILD_EDIT_5);
		sprintf(buffer,"[%d]",pGuildManager->clCommonUnit.clGPoint.siBattleGP);
		pEditBGP->SetText(buffer);
	}
}
