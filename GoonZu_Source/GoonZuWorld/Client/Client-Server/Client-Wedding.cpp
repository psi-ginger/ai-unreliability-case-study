#include "../Client.h"
#include "../Music\Music.h"
#include "../CommonLogic/MsgType.h"
#include "../CommonLogic/Msg/MsgType-Wedding.h"
#include "../CommonLogic/Marriage/MarriageManager.h"
#include "../CommonLogic/Marriage/WeddingManager.h"

#include "Char/CharManager/CharManager.h"

#include "../Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "../ninterface/NWeddingCeremony/NWeddingCeremony.h"


void cltClient::DoMsg_GAMEMSG_RESPONSE_WEDDING_RESERVE(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_WeddingReserve* pclinfo = (cltGameMsgResponse_WeddingReserve*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( 1 != pclinfo->m_siResult )
	{
		return;
	}

	cltCharClient* pclChar = pclCM->GetCharClient( id );
	if ( NULL == pclChar )
	{
		return;
	}

	// 은행계좌의 돈을 업데이트 한다
	pclChar->pclCI->clBank.SetMoney( pclinfo->m_gmLeftBankMoney );

	// 예약 메시지를 보여준다
	TCHAR	szBuffer[256]	= TEXT("");
	TCHAR*	pszText			= GetTxtFromMgr(30063);

	StringCchPrintf( szBuffer, 256, pszText, pclinfo->m_siYear, pclinfo->m_siMonth, pclinfo->m_siDay, pclinfo->m_siHour );

	if ( NULL == m_pDialog[NOTIFYMSG_DLG] )
	{
		CreateInterface( NOTIFYMSG_DLG );
	}

	m_pDialog[NOTIFYMSG_DLG]->SetText( TEXT(""), szBuffer );

	// 예식 목록에서도 요청한다
	if ( NULL == m_pDialog[NWEDDINGCEREMONY_MENU_DLG] )
	{
		return;
	}

	((CNWeddingCeremony*)m_pDialog[NWEDDINGCEREMONY_MENU_DLG])->RequestList();

}

void cltClient::DoMsg_GAMEMSG_RESPONSE_WEDDING_SIMPLELIST_GET(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_WeddingSimpleListGet* pclinfo = (cltGameMsgResponse_WeddingSimpleListGet*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( NULL == m_pDialog[NWEDDINGCEREMONY_MENU_DLG] )
	{
		return;
	}

	((CNWeddingCeremony*)m_pDialog[NWEDDINGCEREMONY_MENU_DLG])->SetReserveSimpleList( pclinfo->m_clSimpleUnit );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_WEDDING_LIST_GET(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_WeddingListGet* pclinfo = (cltGameMsgResponse_WeddingListGet*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( NULL == m_pDialog[NWEDDINGCEREMONY_MENU_DLG] )
	{
		return;
	}

	((CNWeddingCeremony*)m_pDialog[NWEDDINGCEREMONY_MENU_DLG])->SetReserveList( pclinfo->m_clUnit );
}

void cltClient::DoMsg_GAMEMSG_RESPONSE_WEDDING_RESERVE_CANCEL(cltMsg* pclMsg, SI32 id)
{
	cltGameMsgResponse_WeddingReserveCancel* pclinfo = (cltGameMsgResponse_WeddingReserveCancel*)pclMsg->cData;
	if ( NULL == pclinfo )
	{
		return;
	}

	if ( 1 != pclinfo->m_siResult )
	{
		return;
	}

	if ( NULL == m_pDialog[NOTIFYMSG_DLG] )
	{
		CreateInterface( NOTIFYMSG_DLG );
	}

	NTCHARString128 strText = GetTxtFromMgr(30067);

	m_pDialog[NOTIFYMSG_DLG]->SetText( TEXT(""), strText );

	// 예식 목록에서도 요청한다
	if ( NULL == m_pDialog[NWEDDINGCEREMONY_MENU_DLG] )
	{
		return;
	}

	((CNWeddingCeremony*)m_pDialog[NWEDDINGCEREMONY_MENU_DLG])->RequestList();
}