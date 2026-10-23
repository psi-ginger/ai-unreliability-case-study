#include "NGuildWarApply.h"

#include "../../CommonLogic/CommonLogic.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client/Client.h"
#include "../../../Common/Char/CharManager/CharManager.h"


extern cltCommonLogic* pclClient;

CNGuildWarApplyDlg::CNGuildWarApplyDlg()
{
	Init();
}

CNGuildWarApplyDlg::~CNGuildWarApplyDlg()
{
	Destroy();
}

void CNGuildWarApplyDlg::Init()
{
	m_ReceivedClock = 0;
	//m_siHomeGuildUnique = 0;
	//m_siFieldIndex = 0;
	//m_siRoomIndex = 0;
}

void CNGuildWarApplyDlg::Destroy()
{

}

void CNGuildWarApplyDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile	file;

	file.LoadFile(TEXT("NInterface/Data/NGuildWarApply/DLG_NGuildWarApply.ddf"));
	file.CreatePopupDialog( this, NGUILDWARAPPLY_DLG, TEXT("dialog_guildwar_apply"),StaticGuildWarApplyDlgProc );

	// 컨트롤 생성
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NGUILDWARAPPLY_STATIC_EXPLAIN, this) ,NGUILDWARAPPLY_STATIC_EXPLAIN, TEXT("statictext_explain"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NGUILDWARAPPLY_STATIC_LEFTTIME, this) ,NGUILDWARAPPLY_STATIC_LEFTTIME, TEXT("statictext_lefttime"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGUILDWARAPPLY_BUTTON_CONFIRM, this) ,NGUILDWARAPPLY_BUTTON_CONFIRM, TEXT("button_confirm"));
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON ,NGUILDWARAPPLY_BUTTON_DENY, this) ,NGUILDWARAPPLY_BUTTON_DENY, TEXT("button_deny"));

	m_ReceivedClock = pclClient->CurrentClock;

	SetActionTimer( 1000 );
	SetTimerOnOff( true );
}

void CALLBACK CNGuildWarApplyDlg::StaticGuildWarApplyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNGuildWarApplyDlg *pThis = (CNGuildWarApplyDlg*) pControllerMgr;
	pThis->NGuildWarApplyDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNGuildWarApplyDlg::NGuildWarApplyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NGUILDWARAPPLY_BUTTON_CONFIRM:
		{
			AcceptApplication();
		}
		break;
	case NGUILDWARAPPLY_BUTTON_DENY:
		{
			DenyApplication();
		}
	default:
		break;
	}
}

void CNGuildWarApplyDlg::Action()
{

	if ( TABS( pclClient->CurrentClock - m_ReceivedClock ) > 60 * 1000 )
	{
		DenyApplication();
	}

	SetLeftTime();


}

void CNGuildWarApplyDlg::SetExplain(cltGameMsgResponse_GuildWar_ApplyToEnemy* pclinfo )
{
	//m_siHomeGuildUnique = pclinfo->siHomeGuildUnique;
	//m_siFieldIndex = pclinfo->siFieldIndex;
	//m_siRoomIndex = pclinfo->siRoomIndex;

	m_ApplyInfo.Set(pclinfo);


	CStatic* pStatic = m_InterfaceFactory.GetStatic( NGUILDWARAPPLY_STATIC_EXPLAIN );
	if ( pStatic )
	{
		TCHAR buffer[256];
		TCHAR Hour[4];
		TCHAR Minute[4];
		TCHAR Guildname[64];
		SI32 siHour = (m_ApplyInfo.siRoomIndex*30)/60;
		TCHAR *pText = GetTxtFromMgr(8277);
		StringCchPrintf(Hour, 4, TEXT("%02d"), siHour);

		//[진성] 시간(시) 출력부분 수정. => 2008-5-7
		StringCchPrintf(Hour, 4, TEXT("%d%d"), siHour/10, siHour%10);

		SI32 min = (m_ApplyInfo.siRoomIndex*30)%60;
		StringCchPrintf(Minute, 4, TEXT("%02d"), min );			

		//[진성] 시간(분) 출력부분 수정. => 2008-5-7
		StringCchPrintf(Minute, 4, TEXT("%d%d"), min/10, min%10);

		StringCchPrintf(Guildname, 20, TEXT("%s"), pclinfo->szHomeGuildName );

		Nsprintf( buffer, pText, TEXT("hour"), Hour, TEXT("minute"), Minute, TEXT("eGuild"), Guildname, NULL);
		//StringCchPrintf( buffer, 256, TEXT("%d %d %s"), pclinfo->siFieldIndex, pclinfo->siRoomIndex, pclinfo->szHomeGuildName );
		pStatic->SetText( buffer );
	}
}

void CNGuildWarApplyDlg::AcceptApplication()
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGameMsgRequest_GuildWar_ConfirmApply clinfo( true, m_ApplyInfo.siFieldIndex, m_ApplyInfo.siRoomIndex, m_ApplyInfo.siHomeGuildUnique, m_ApplyInfo.szHomeGuildName, m_ApplyInfo.siAwayGuildUnique, m_ApplyInfo.szAwayGuildName );
	cltMsg clMsg( GAMEMSG_REQUEST_GUILDWAR_CONFIRMAPPLY, sizeof(clinfo), (BYTE*)&clinfo );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);


	DeleteDialog();
}
void CNGuildWarApplyDlg::DenyApplication()
{
	cltClient* pclclient = (cltClient*)pclClient;

	cltGameMsgRequest_GuildWar_ConfirmApply clinfo( false, m_ApplyInfo.siFieldIndex, m_ApplyInfo.siRoomIndex, m_ApplyInfo.siHomeGuildUnique, m_ApplyInfo.szHomeGuildName, m_ApplyInfo.siAwayGuildUnique, m_ApplyInfo.szAwayGuildName );
	cltMsg clMsg( GAMEMSG_REQUEST_GUILDWAR_CONFIRMAPPLY, sizeof(clinfo), (BYTE*)&clinfo );
	pclclient->SendMsgToServer((sPacketHeader*)&clMsg);

	DeleteDialog();
}

void CNGuildWarApplyDlg::SetLeftTime()
{
	CStatic* pStatic = m_InterfaceFactory.GetStatic( NGUILDWARAPPLY_STATIC_LEFTTIME );

	if( pStatic )
	{
		TCHAR buffer[256];
		SI32 leftClock = (SI32)((60 * 1000) - TABS( pclClient->CurrentClock - m_ReceivedClock ));

		TCHAR* pText = GetTxtFromMgr( 7546 );
		StringCchPrintf( buffer, 256, pText, leftClock / 1000);
		pStatic->SetText( buffer );
	}
}