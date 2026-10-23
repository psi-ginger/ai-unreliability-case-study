
#include "NBeGuildMemberDlg.h" 

#include "resource.h"
#include <tchar.h>
#include <CommonLogic.h>
#include "Char\CharCommon\Char-Common.h"
#include "Char\CharManager\CharManager.h"

#include "MsgType-Structure.h"
#include "MsgType-Guild.h"

#include "../../Client/InterfaceMgr/Interface/Edit.h"
#include "../../Client/InterfaceMgr/Interface/List.h"
#include "../../Client/InterfaceMgr/Interface/Button.h"

#include "../../Client/InterfaceMgr/InterfaceFile.h"
#include "../../Client/InterfaceMgr/InterfaceMgr.h"
#include "../../Client/NInterface/NSoundOnOffDlg/NSoundOnOffDlg.h"

#include "../../../../CommonLogic/Guild/Guild.h"

cltStructureMngBasic*	NBeGuildMemberDlg::m_pclStructureManager;

extern cltCommonLogic* pclClient;

NBeGuildMemberDlg::NBeGuildMemberDlg()
{


	m_pButton_GuildMember	 = NULL; 
	m_pButton_delGuildMember	 = NULL; 
	m_pEdit_name	 = NULL; 
	m_pList_GuildMember	 = NULL; 

}

NBeGuildMemberDlg::~NBeGuildMemberDlg()
{
	if( 	m_pButton_GuildMember	 )  	delete 	m_pButton_GuildMember	;
	if( 	m_pButton_delGuildMember	 )  	delete 	m_pButton_delGuildMember	;
	if( 	m_pEdit_name	 )  	delete 	m_pEdit_name	;
	if( 	m_pList_GuildMember	 )  	delete 	m_pList_GuildMember	;
}

void NBeGuildMemberDlg::Create( )
{
	m_pclStructureManager = pclClient->pclCityHallManager;

	if( !IsCreate() )
	{
		cltClient *pclclient = (cltClient*)pclClient;

		CInterfaceFile file;
		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_BeGuildMember.ddf"));

		file.CreatePopupDialog( this, NBEGUILDMEMBER_DLG, TEXT("dialog_beGuildMember"), NBeGuildMemberStaticProc);

		m_pButton_GuildMember	 = new 	CButton	( this )	;
		m_pButton_delGuildMember	 = new 	CButton	( this )	;
		m_pEdit_name	 = new 	CEdit	( this )	;
		m_pList_GuildMember	 = new 	CList	( this )	;

		file.CreateControl( 	m_pButton_GuildMember	, 	NBEGUILDMEMBER_BUTTON_GUILDMEMBER	, 	TEXT("button_GuildMember")	 )	;
		file.CreateControl( 	m_pButton_delGuildMember	, 	NBEGUILDMEMBER_BUTTON_DELGUILDMEMBER	, 	TEXT("button_Clear")	 )	;
		file.CreateControl( 	m_pEdit_name	, 	NBEGUILDMEMBER_EDITBOX_NAME	, 	TEXT("editbox_name")	 )	;
		file.CreateControl( 	m_pList_GuildMember	, 	NBEGUILDMEMBER_LISTBOX_GUILDMEMBER	, 	TEXT("listbox_GuildMember")	 )	;


		//int width[] = { 92, 60, 100 };
		SI32 width = 250 ;
		//	TCHAR *text[] = { TEXT("아이디"), TEXT("레벨"), TEXT("신청일") };

		//TCHAR* pText[3];

		//pText[0] = GetTxtFromMgr(1473);
		//pText[1] = GetTxtFromMgr(1506);
		//pText[2] = GetTxtFromMgr(1507);
		TCHAR* pText = GetTxtFromMgr(1473);

		m_pList_GuildMember->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
		m_pList_GuildMember->SetBorder( true );
		m_pList_GuildMember->SetBKColorUse( true );

		m_pList_GuildMember->SetColumn( 0, width, pText );

/*		for( i = 0; i < 3; ++i ) {

			m_pList_GuildMember->SetColumn( i, width[i], pText[i] );
		}*/
		//m_pList_GuildMember->SetColumn( 3, 18, TEXT("") );
		m_pList_GuildMember->SetColumn( 1, 18, TEXT("") );

		m_pList_GuildMember->Refresh();

		RefreshList();

		//SetActionTimer( 500 );
		//SetTimerOnOff( true );
	}
	else
	{
		DeleteDialog();
	}
}

void NBeGuildMemberDlg::NBeGuildMemberStaticProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NBeGuildMemberDlg *pThis = (NBeGuildMemberDlg*)pControllerMgr;
	pThis->NBeGuildMemberProc( nEvent, nControlID, pControl );
}

void NBeGuildMemberDlg::NBeGuildMemberProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient*)pclClient;
	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	case NBEGUILDMEMBER_LISTBOX_GUILDMEMBER:
		{
			switch( nEvent )
			{
			case EVENT_LISTBOX_SELECTION:
				{
					TCHAR buf[ 256 ];
					TCHAR *pTmpChar = NULL;

					SI16 siRow = -1, siCol = -1;
					m_pList_GuildMember->GetSelectedIndex( &siRow, &siCol );
					if( siRow == -1 || siCol == -1 ) return;
					pTmpChar = m_pList_GuildMember->GetText( siRow, 0 );
					if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 32 );
					m_pEdit_name->SetText( buf );
				}
				break;
			}
		}
		break;

	case NBEGUILDMEMBER_BUTTON_GUILDMEMBER	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buf[ 128 ] = "" ;
					TCHAR *pTmpChar = NULL;
					TCHAR buffer[256] = "" ;

					SI16 siRow = -1, siCol = -1;
					m_pList_GuildMember->GetSelectedIndex( &siRow, &siCol );
					if( siRow == -1 || siCol == -1 ) return;
					pTmpChar = m_pList_GuildMember->GetText( siRow, 0 );
					if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 32 );

					SI32 siStandingPersonID = 0 ;
					for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
					{
						if ( !strcmp(buf,pGuildManager->clCommonUnit.clStandingMember[i].szCharName) )
						{
							siStandingPersonID = pGuildManager->clCommonUnit.clStandingMember[i].siPersonID ;
							break;
						}
					}

					if ( pGuildManager->clCommonUnit.GetLimitMemberNum() <= pGuildManager->clCommonUnit.GetMemberNum() )
					{
						TCHAR * pTitle = GetTxtFromMgr(6573);
						TCHAR * pText = GetTxtFromMgr(6574);
						sprintf(buffer,pText,pGuildManager->clCommonUnit.clGPoint.siLevel,pGuildManager->clCommonUnit.GetLimitMemberNum());
						pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,buffer);

						break;
					}

					if ( siStandingPersonID )
					{
						stMsgBox MsgBox;

						TCHAR * pTitle = GetTxtFromMgr(6524);
						TCHAR * pText = GetTxtFromMgr(6534);
						sprintf(buffer,pText,buf);
						MsgBox.Set( this, pTitle,buffer, MSGBOX_TYPE_YESNO, 0 );

						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( siStandingPersonID );
						memcpy( TempBuffer, &siStandingPersonID, sizeof( siStandingPersonID ) );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
					}
				}
				break;
			}
		}
		break;
	case NBEGUILDMEMBER_BUTTON_DELGUILDMEMBER	: 
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					TCHAR buf[ 256 ];
					TCHAR *pTmpChar = NULL;

					SI16 siRow = -1, siCol = -1;
					m_pList_GuildMember->GetSelectedIndex( &siRow, &siCol );
					if( siRow == -1 || siCol == -1 ) return;
					pTmpChar = m_pList_GuildMember->GetText( siRow, 0 );
					if(pTmpChar == NULL ) return;
					_tcsncpy( buf, pTmpChar, 32 );

					SI32 siStandingPersonID = 0 ;
					for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
					{
						if ( !strcmp(buf,pGuildManager->clCommonUnit.clStandingMember[i].szCharName) )
						{
							siStandingPersonID = pGuildManager->clCommonUnit.clStandingMember[i].siPersonID ;
							break;
						}
					}

					if ( siStandingPersonID )
					{
						stMsgBox MsgBox;
						TCHAR buffer[256] = "" ;

						TCHAR * pTitle = GetTxtFromMgr(6524);
						TCHAR * pText = GetTxtFromMgr(6535);
						sprintf(buffer,pText,buf);
						MsgBox.Set( this, pTitle,buffer, MSGBOX_TYPE_YESNO, 1 );

						BYTE TempBuffer[ 128 ];
						SI16 Size = sizeof( siStandingPersonID );
						memcpy( TempBuffer, &siStandingPersonID, sizeof( siStandingPersonID ) );
						pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
					}
				}
				break;
			}
		}
		break;
	}
}

void NBeGuildMemberDlg::Action()
{
	
}

void NBeGuildMemberDlg::Show()
{
	m_bShow = true;
	CDialogController::Show( true );
}

void NBeGuildMemberDlg::Hide()
{
	m_bShow = false;
	CDialogController::Show( false );
}

bool NBeGuildMemberDlg::IsShow()
{
	return m_bShow;
}

void NBeGuildMemberDlg::RefreshList()
{
	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);
	stListBoxItemData itemdata;

	// 리스트뷰를 모두 지운다. 
	m_pList_GuildMember->Clear();

	TCHAR buffer[256] = "" ;

	for( SI32 i = 0; i < MAX_GUILD_MEMBER_NUM; i++ )
	{
		itemdata.Init();

		if( pGuildManager->clCommonUnit.clStandingMember[i].siPersonID != 0 )
		{
			itemdata.Set( 0, (TCHAR*)pGuildManager->clCommonUnit.clStandingMember[i].szCharName );

			m_pList_GuildMember->AddItem( &itemdata );
		}
	}

	m_pList_GuildMember->Refresh();
}

void NBeGuildMemberDlg::RetMsgBox(bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// 가입 허가
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 personid = *((SI32*)pData);
				cltGameMsgRequest_Guild_ConfirmJoinGuild clinfo(TRUE,personid);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CONFIRMJOINGUILD,sizeof(clinfo),(BYTE*)&clinfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	case 1: // 가입 불허
		{
			if ( pData == NULL || DataLen < 1 )
			{
				return;
			}

			if ( RetValue )
			{
				SI32 personid = *((SI32*)pData);
				cltGameMsgRequest_Guild_ConfirmJoinGuild clinfo(FALSE,personid);
				cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CONFIRMJOINGUILD,sizeof(clinfo),(BYTE*)&clinfo);
				pclClient->pclCM->CR[1]->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
		break;
	}
}

