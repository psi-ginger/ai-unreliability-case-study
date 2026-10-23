#include "GuildDlg.h"

#include "Char/CharManager/CharManager.h"
#include "../../Server/Rank/Rank.h"

#include "../../../CommonLogic/MsgType.h"
#include "../../../CommonLogic/MsgType-Structure.h"
#include "../../../CommonLogic/MsgType-Guild.h"

#include "../Lib/JYLibFunction.h"
#include "../../Client/Music/Music.h"
#include "../../Client/InterfaceMgr/InterfaceMgr.h"

#include "..\..\Common\JWLib\IsCorrectID\IsCorrectID.h"

#include "Msg/MsgType-Quest.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NGuildUserDlg::NGuildUserDlg()
{


}

NGuildUserDlg::~NGuildUserDlg()
{

}

void NGuildUserDlg::InitDlg()
{

	SetControlMap( EDIT_GUILDNAME, TEXT("editbox_guildname"));

	SetControlMap( BUTTON_APPLY, TEXT("button_apply") );
	SetControlMap( BUTTON_CANCEL_APPLY, TEXT("button_cancel1") );
	SetControlMap( BUTTON_CHANGE_VILLAGE, TEXT("button_moveguild") );
	SetControlMap( EDITBOX_EXPLAIN, TEXT("editbox_contract") );

	((CEdit*)m_pControlMap[ EDIT_GUILDNAME ])->SetMaxEditTextLength(MAX_GUILD_NAME);

	CEdit* pEdit = (CEdit*)m_pControlMap[EDITBOX_EXPLAIN];
	if( pclClient->IsCountrySwitch( Switch_Guild_DestroyOverMOnth ) )
	{
		TCHAR* pText = GetTxtFromMgr(8692);
		pEdit->SetText( pText );
	}

	

	ShowRentContractInUser();
}

void NGuildUserDlg::Show()
{
	CControllerMgr::Show( true );
	
	pclClient->pclCM->CR[1]->DoQuestCheckAndAction(QUEST_CLEAR_COND_OPENGUILDUSER);
}

void NGuildUserDlg::ShowRentContractInUser()	
{	

	//cltGuildManager *pGuildManager = ((cltGuildManager *)m_pStrManager);

	//TCHAR buffer[1024];

	//StringCchPrintf(buffer, 1024, TEXT("%d"), pGuildManager->clClientStrInfo.clMineStrInfo.clRentContract.siRentFee);
	//SetEditText( EDIT_RENT_FEE, buffer );

	//TCHAR* pText = GetTxtFromMgr(3249);
	//StringCchPrintf(buffer, 1024, pText);
	//SetEditText( EDIT_CONTRACT, buffer );

}

void NGuildUserDlg::Action()
{
	if( m_pBaseDlg->m_dwActionCount % 4 ) return;

	ShowRentContractInUser();
}


void NGuildUserDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{
 	cltClient* pclclient = (cltClient*)pclClient;

	CASE_CONTROL( BUTTON_APPLY )
	{
		TCHAR buffer[256] = "" ;
		TCHAR * GuildName = GetEditText(EDIT_GUILDNAME);
		StringCchPrintf(buffer, 256, GuildName);
#ifdef _IAF_EXPORT
		if( pclClient->IsWhereServiceArea(ConstServiceArea_USA) )
		{
			TCHAR ResultBuf[1024] = "";
			IAF::Filtering( buffer, ResultBuf, sizeof(ResultBuf), IAF::FILTER_CHARACTER );
			memcpy( buffer, ResultBuf, strlen(ResultBuf) * sizeof(ResultBuf[0]) + 1 );
		}
#else	
		pclclient->m_pCAbuseFilter->CheckAbuse(buffer);
#endif

		if ( GuildName == NULL || _tcslen( GuildName ) < 1)
		{
			TCHAR * pTitle = GetTxtFromMgr(6527);
			TCHAR * pText = GetTxtFromMgr(6528);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}

		/*
		if ( ! IsCorrectID( buffer ) )
		{
			TCHAR * pTitle = GetTxtFromMgr(6527);
			TCHAR * pText = GetTxtFromMgr(840);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}*/

		if ( strlen(GuildName) >= MAX_GUILD_NAME )
		{
			TCHAR * pTitle = GetTxtFromMgr(6527);
			TCHAR * pText = GetTxtFromMgr(6529);
			StringCchPrintf(buffer, 256, pText, MAX_GUILD_NAME);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,buffer);
			return ;
		}

		cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
		if ( pclchar == NULL )
			return ;

		if ( pclchar->pclCI->clIP.GetLevel() < MIN_GUILD_MASTER_LEVEL )
		{
			TCHAR * pTitle = GetTxtFromMgr(6527);
			TCHAR * pText = GetTxtFromMgr(6530);
			StringCchPrintf(buffer, 256, pText, MIN_GUILD_MASTER_LEVEL);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,buffer);
			return ;
		}

		if ( pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex != 0 )
		{
			TCHAR * pTitle = GetTxtFromMgr(6527);
			TCHAR * pText = GetTxtFromMgr(6531);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}

		if ( pclchar->pclCI->clBank.GetMoney() < MIN_GUILD_CREATE_FEE )
		{
			TCHAR* pTitle = GetTxtFromMgr(6527);
			TCHAR* pText = GetTxtFromMgr(6603);
			StringCchPrintf(buffer, 256, pText, MIN_GUILD_CREATE_FEE);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, buffer);
			return;
		}

		SI32 VillageUnique = pclchar->pclMap->siVillageUnique ;

		TCHAR * pTitle = GetTxtFromMgr(6527);
		TCHAR * pText = GetTxtFromMgr(6532);
		sprintf(buffer,pText,GuildName);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 0 );

		BYTE TempBuffer[ 128 ] ="";
		ZeroMemory(TempBuffer,sizeof(BYTE)*128);
		SI16 Size = sizeof( VillageUnique ) + sizeof( TCHAR ) * strlen(GuildName);
		memcpy( TempBuffer, &VillageUnique, sizeof( VillageUnique ) );
		memcpy( &TempBuffer[ sizeof(VillageUnique) ], GuildName, sizeof( TCHAR ) * strlen(GuildName) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
	CASE_CONTROL( BUTTON_CANCEL_APPLY ) // 신청 취소
	{
		TCHAR buffer[256] = "" ;
		cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
		if ( pclchar == NULL )
			return ;

		// 길드가 없으면 안되며 길드 마스터여야 하며 현재 신청대기중이여야 한다!
		if ( pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex == 0 )
		{
			TCHAR * pTitle = GetTxtFromMgr(6621);
			TCHAR * pText = GetTxtFromMgr(6623);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}
		else if ( pclchar->pclCI->clBI.clGuildRank.uiConfirm != GUILDSTATUS_STANDING )
		{
			TCHAR * pTitle = GetTxtFromMgr(6621);
			TCHAR * pText = GetTxtFromMgr(6624);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}
		else if ( pclchar->pclCI->clBI.clGuildRank.siType != GUILDRANKTYPE_MASTER )
		{
			TCHAR * pTitle = GetTxtFromMgr(6621);
			TCHAR * pText = GetTxtFromMgr(6625);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}

		SI32 VillageUnique = pclchar->pclMap->siVillageUnique ;
		if ( pclclient->pclVillageManager->IsValidVillage(VillageUnique) == false )
			return ;
		SI32 GuildUnique = pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex;

		TCHAR * pTitle = GetTxtFromMgr(6621);
		TCHAR * pText = GetTxtFromMgr(6622);

		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pTitle,pText, MSGBOX_TYPE_YESNO, 5 );

		BYTE TempBuffer[ 128 ] ="";
		ZeroMemory(TempBuffer,sizeof(BYTE)*128);
		SI16 Size = sizeof( GuildUnique ) + sizeof( VillageUnique ) ;
		memcpy( TempBuffer, &GuildUnique, sizeof( GuildUnique ) );
		memcpy( &TempBuffer[ sizeof(VillageUnique) ], &VillageUnique, sizeof( VillageUnique ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
	CASE_CONTROL( BUTTON_CHANGE_VILLAGE ) // 거주지 변경
	{
		TCHAR buffer[256] = {0};
		cltCharClient* pclchar = (cltCharClient*)pclClient->pclCM->CR[1];
		if ( pclchar == NULL )
			return ;

		SI32 VillageUnique = pclchar->pclMap->siVillageUnique ;
		if ( pclclient->pclVillageManager->IsValidVillage(VillageUnique) == false )
			return ;

		// 길드가 없으면 안되며 길드 마스터여야 하며 정상활동중이여야 한다! 그리고 같은 마을로는 이동안된다.
		if ( pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex == 0 )
		{
			TCHAR * pTitle = GetTxtFromMgr(6619);
			TCHAR * pText = GetTxtFromMgr(6623);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}
		else if ( pclchar->pclCI->clBI.clGuildRank.uiConfirm != GUILDSTATUS_NORMAL )
		{
			TCHAR * pTitle = GetTxtFromMgr(6619);
			TCHAR * pText = GetTxtFromMgr(6626);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}
		else if ( pclchar->pclCI->clBI.clGuildRank.siType != GUILDRANKTYPE_MASTER )
		{
			TCHAR * pTitle = GetTxtFromMgr(6619);
			TCHAR * pText = GetTxtFromMgr(6625);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}
		else if ( pclchar->pclCI->clBI.clGuildRank.siVillageUnique == VillageUnique )
		{
			//KHY - 0708 - 같은 마을일 경우 경고메시지 출력.
			TCHAR * pTitle = GetTxtFromMgr(6619);
			TCHAR * pText = GetTxtFromMgr(7544);
			pclclient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle,pText);
			return ;
		}

		SI32 GuildUnique = pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex;

		TCHAR * pTitle = GetTxtFromMgr(6619);
		TCHAR * pText = GetTxtFromMgr(6620);

		TCHAR* villageName = pclClient->pclVillageManager->GetName( VillageUnique );
		StringCchPrintf( buffer, 256, pText, villageName );


		stMsgBox MsgBox;
		// 현재 dialog가 탭 창 같은 Child Dialog일때
		CControllerMgr *pParent = NULL;
		pParent = this->GetParentDialog();
		if(pParent == NULL ) return;
		MsgBox.Set( pParent, pTitle,buffer, MSGBOX_TYPE_YESNO, 6 );

		BYTE TempBuffer[ 128 ] ="";
		ZeroMemory(TempBuffer,sizeof(BYTE)*128);
		SI16 Size = sizeof( GuildUnique ) + sizeof( VillageUnique ) ;
		memcpy( TempBuffer, &GuildUnique, sizeof( GuildUnique ) );
		memcpy( &TempBuffer[ sizeof(VillageUnique) ], &VillageUnique, sizeof( VillageUnique ) );
		pclclient->SetMsgBox( &MsgBox, TempBuffer, Size );
	}
}
