#include "../Client.h"
#include "Msg/MsgType-Empire.h"
#include "Char\CharManager\CharManager.h"

#include "ninterface/NCommonMsgDlg/NCommonMsgDlg.h"
#include "NStructureDlg/Cityhall/NCityhallDlg.h"


void cltClient::DoMsg_GAMEMSG_RESPONSE_CREATEEMPIRE(cltMsg* pclMsg, SI32 id)
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_DESTROYEMPIRE(cltMsg* pclMsg, SI32 id)
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_INVITEEMPIRE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_InviteEmpire* pclinfo = (cltGameMsgResponse_InviteEmpire*)pclMsg->cData;

	if( cltGameMsgResponse_InviteEmpire::VILLAGE == pclinfo->siInviteType )
	{
		if (m_pDialog[NCOMMONMSG_DLG] == NULL)
		{
			CreateInterface( NCOMMONMSG_DLG );
		}
		
		((CNCommonMsgDlg*)m_pDialog[NCOMMONMSG_DLG])->SetInviteEmpire( pclinfo->siInviteType, pclinfo->siEmpireUnique, pclinfo->siFromPersonID, pclinfo->siEmproerVillageUnique );
		((CNCommonMsgDlg*)m_pDialog[NCOMMONMSG_DLG])->Set( COMMONDLG_INVITEEMPIRE );
		
	}
	else if( cltGameMsgResponse_InviteEmpire::GUILD == pclinfo->siInviteType )
	{
		if (m_pDialog[NCOMMONMSG_DLG] == NULL)
		{
			CreateInterface( NCOMMONMSG_DLG );
		}
		
		((CNCommonMsgDlg*)m_pDialog[NCOMMONMSG_DLG])->SetInviteEmpire( pclinfo->siInviteType, pclinfo->siEmpireUnique, pclinfo->siFromPersonID, pclinfo->siEmproerVillageUnique );
		((CNCommonMsgDlg*)m_pDialog[NCOMMONMSG_DLG])->Set( COMMONDLG_INVITEEMPIRE );

	}
	else if( cltGameMsgResponse_InviteEmpire::EMPEROR == pclinfo->siInviteType )
	{
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_JOINEMPIRE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_JoinEmpire* pclinfo = (cltGameMsgResponse_JoinEmpire*)pclMsg->cData;

	/// 초대한 사람한테 알려주기
	if( cltGameMsgResponse_JoinEmpire::EMPEROR == pclinfo->siSendType )
	{
	}
	/// 초대에 응한 사람에게 알려주기
	else if( cltGameMsgResponse_JoinEmpire::OTHERUSER == pclinfo->siSendType )
	{
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SECEDEEMPIRE(cltMsg* pclMsg, SI32 id)
{
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_UPDATEEMPIREINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_UpdateEmpireInfo* pclinfo = (cltGameMsgResponse_UpdateEmpireInfo*)pclMsg->cData;

	SI32 tempID = pclCM->GetIDFromCharUnique(pclinfo->siCharUnique);
	
	if( 0 < tempID )
	{
		if( cltGameMsgResponse_UpdateEmpireInfo::VILLAGE == pclinfo->siUpdateType )
		{
			pclCM->CR[tempID]->pclCI->clBI.clEmpireInfo.SetEmpireInfo( &pclinfo->clEmpireInfo );
		}
		else if( cltGameMsgResponse_UpdateEmpireInfo::GUILD == pclinfo->siUpdateType )
		{
			pclCM->CR[tempID]->pclCI->clBI.clGuildRank.siEmpireUnique = pclinfo->clEmpireInfo.m_siEmpireUnique;
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_EMPIREINFO(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_EmpireInfo* pclinfo = (cltGameMsgResponse_EmpireInfo*)pclMsg->cData;

	if( true == pclinfo->bVillageChief )
	{
		if ( m_pDialog[ NCITYHALL_DLG ] )
		{
			((NCityhallChiefMenuEmpireDlg*)((NCityhallDlg*)m_pDialog[ NCITYHALL_DLG ])->m_pChiefChildDlg[7])->SetEmpireMemberList( pclinfo );
		}
	}
	else
	{
		if ( m_pDialog[ NCITYHALL_DLG ] )
		{
			((NCityhallUserMenuEmpireDlg*)((NCityhallDlg*)m_pDialog[ NCITYHALL_DLG ])->m_pUserChildDlg[6])->SetEmpireMemberList( pclinfo );
		}
	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_SETEMPIREMONEY(cltMsg* pclMsg, SI32 id)
{
}






