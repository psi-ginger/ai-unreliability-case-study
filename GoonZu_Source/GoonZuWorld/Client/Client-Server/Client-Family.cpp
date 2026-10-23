#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/MsgType-Person.h"
#include "Char/CharManager/CharManager.h"
#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../ninterface/NCommonMsgDlg/NCommonMsgDlg.h"
#include "../ninterface/NFamilyInfoDlg/NFamilyInfoDlg.h"

void cltClient::DoMsg_GAMEMSG_RESPONSE_FAMILY_REQUEST_ADD_CHILDRENN(cltMsg* pclMsg, SI32 id)
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGameResponse_Request_Add_Children* pclinfo = (cltGameResponse_Request_Add_Children*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( id );
	if ( NULL == pclChar )
	{
		return;
	}
	// 요청메세지를 받았으므로 해당 요청 메세지에 대한 대답을 한다.

	// 메시지 창 띄우기
	if (pclclient->m_pDialog[NCOMMONMSG_DLG] == NULL)
	{
		pclclient->CreateInterface( NCOMMONMSG_DLG );
	}
	/*[#name1#]님과 [#name2#]님이 자녀로 등록하길 원하고 있습니다.\r	승낙하시겠습니까?
	*/
	NTCHARString512	kText(GetTxtFromMgr(30131));
	kText.Replace("#name1#", pclinfo->m_szParentCharName );
	kText.Replace("#name2#", pclinfo->m_szMateCharName );

	((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->Set(COMMONDLG_REQUEST_FAMILY_ADD_CHILDREN);	
	((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetText(kText);
	((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetParentPersonID(pclinfo->m_siParentPersonID);	
	((CNCommonMsgDlg*)pclclient->m_pDialog[NCOMMONMSG_DLG])->SetShowTimeSec( 10 );

}
//자녀를 등록하고 클라이언트업데이트
void cltClient::DoMsg_GAMEMSG_RESPONSE_FAMILY_UPDATE(cltMsg* pclMsg, SI32 id)
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGameResponse_UpdateFamily* pclinfo = (cltGameResponse_UpdateFamily*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( id );
	if ( NULL == pclChar )
	{
		return;
	}

	pclChar->pclFamily->Set( &pclinfo->m_cFamily)	;	// 내 정보 를 업데이트 하고
	pclChar->pclMarriage->Set(&pclinfo->m_cMarriage)	;

	SI32 siMyClass = pclinfo->m_cFamily.GetMyClass_InFamily()	;

	if ( siMyClass == MYCLASS_IN_FAMILY_CHILDREN )	// 내가 자식이고
	{
		CreateInterface( NFAMILYCOOKIELEVEL_DLG );
	}
	// 이미 창이 만들어져 있을때만 표시 한다.

	if (pclclient->m_pDialog[NFAMILYINFO_DLG] == NULL)
	{
	}
	else
	{
		if ( pclclient->m_pDialog[ NFAMILYINFO_DLG ] )
		{
			((CNFamilyInfoDlg *)pclclient->m_pDialog[ NFAMILYINFO_DLG ])->DisplayFamilyInfo() 	;
		}

	}
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_FAMILY_CHAT(cltMsg* pclMsg, SI32 id)
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGameResponse_ChatFamily* pclinfo = (cltGameResponse_ChatFamily*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( id );
	if ( NULL == pclChar )
	{
		return;
	}
	pclclient->CreateInterface( NFAMILYINFO_DLG );
	((CNFamilyInfoDlg *)pclclient->m_pDialog[ NFAMILYINFO_DLG ])->RecvChat(&pclinfo->m_szchatdata ) 	;

}