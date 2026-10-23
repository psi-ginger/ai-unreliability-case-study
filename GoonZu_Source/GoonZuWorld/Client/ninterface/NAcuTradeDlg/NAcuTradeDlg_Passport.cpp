#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-WorldMoney.h"

#include "NAcuTradeDlg_Parent.h"
#include "NAcuTradeDlg_Passport.h"


CNAcuTradeDlg_Passport::CNAcuTradeDlg_Passport() : CDialogController()
{

}

CNAcuTradeDlg_Passport::~CNAcuTradeDlg_Passport()
{

}

void CNAcuTradeDlg_Passport::Create(CControllerMgr* pParent)
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NAcuTradeDlg/DLG_NAcuTrade_Passport.ddf" ) );
	file.CreateChildDialog( this, NACUTRADEDLG_PASSPORT, TEXT( "dialog_acutrade_passport" ), StaticAcuTradeDlg_PassportProc, pParent );

	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_NONAME1,		this ), NACUTRADE_PASSPORT_STATIC_NONAME1,		TEXT( "statictext_NONAME1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_NONAME2,		this ), NACUTRADE_PASSPORT_STATIC_NONAME2,		TEXT( "statictext_NONAME2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_NONAME3,		this ), NACUTRADE_PASSPORT_STATIC_NONAME3,		TEXT( "statictext_NONAME3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_NONAME4,		this ), NACUTRADE_PASSPORT_STATIC_NONAME4,		TEXT( "statictext_NONAME4" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_NONAME5,		this ), NACUTRADE_PASSPORT_STATIC_NONAME5,		TEXT( "statictext_NONAME5" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_NONAME6,		this ), NACUTRADE_PASSPORT_STATIC_NONAME6,		TEXT( "statictext_NONAME6" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_PORTRAIT,		this ), NACUTRADE_PASSPORT_STATIC_PORTRAIT,		TEXT( "statictext_portrait" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_SERVERNAME,	this ), NACUTRADE_PASSPORT_STATIC_SERVERNAME,	TEXT( "statictext_servername" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_DAY,			this ), NACUTRADE_PASSPORT_STATIC_DAY,			TEXT( "statictext_day" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_PERIOD,		this ), NACUTRADE_PASSPORT_STATIC_PERIOD,		TEXT( "statictext_period" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_ISSUEACU,		this ), NACUTRADE_PASSPORT_STATIC_ISSUEACU,		TEXT( "statictext_issueacu" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PASSPORT_STATIC_EXTENDACU,	this ), NACUTRADE_PASSPORT_STATIC_EXTENDACU,	TEXT( "statictext_extendacu" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_PASSPORT_EDIT_NAME,			this ), NACUTRADE_PASSPORT_EDIT_NAME,			TEXT( "editbox_name" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_PASSPORT_EDIT_NOTICE1,		this ), NACUTRADE_PASSPORT_EDIT_NOTICE1,		TEXT( "editbox_notice1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_PASSPORT_EDIT_NOTICE2,		this ), NACUTRADE_PASSPORT_EDIT_NOTICE2,		TEXT( "editbox_notice2" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PASSPORT_BUTTON_EXTEND,		this ), NACUTRADE_PASSPORT_BUTTON_EXTEND,		TEXT( "button_extend" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PASSPORT_BUTTON_ISSUE,		this ), NACUTRADE_PASSPORT_BUTTON_ISSUE,		TEXT( "button_issue" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NACUTRADE_PASSPORT_OUTLINE_NONAME1,		this ), NACUTRADE_PASSPORT_OUTLINE_NONAME1,		TEXT( "outline_NONAME1" ) );
	

	//-- 컨트롤 설정
	SetController();



	//-- 기본 초기 설정

}

void CALLBACK CNAcuTradeDlg_Passport::StaticAcuTradeDlg_PassportProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNAcuTradeDlg_Passport *pThis = (CNAcuTradeDlg_Passport*) pControllerMgr;
	if ( pThis )
	{
		pThis->NAcuTradeDlg_PassportProc( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNAcuTradeDlg_Passport::NAcuTradeDlg_PassportProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	
	switch( nControlID )
	{
		case NACUTRADE_PASSPORT_BUTTON_ISSUE:
			{
				OrderToIssue();
			}
			break;

		case NACUTRADE_PASSPORT_BUTTON_EXTEND:
			{
				OrderToExtend();
			}
			break;

		case NACUTRADE_PASSPORT_EDIT_NAME:
			{
				if ( EVENT_EDITBOX_CHANGE == nEvent )
				{
					CheckTheCorrectID();
				}
			}
			break;
		
	}

}

void CNAcuTradeDlg_Passport::Action()
{
}

void CNAcuTradeDlg_Passport::Show()
{
	CControllerMgr::Show( true );
}

void CNAcuTradeDlg_Passport::Hide()
{
	CControllerMgr::Show( false );
}

void CNAcuTradeDlg_Passport::Refresh()
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 여권이 없다면 정보갱신은 필요없음
	if ( false == pclchar->pclCI->clWorldPassport.IsHavePassport() )
	{
		return;
	}

	CEdit* pclName = m_InterfaceFactory.GetEdit( NACUTRADE_PASSPORT_EDIT_NAME );
	if ( NULL == pclName )
	{
		return;
	}

	CStatic* pclServerName = m_InterfaceFactory.GetStatic( NACUTRADE_PASSPORT_STATIC_SERVERNAME );
	if ( NULL == pclServerName )
	{
		return;
	}

	CStatic* pclDay = m_InterfaceFactory.GetStatic( NACUTRADE_PASSPORT_STATIC_DAY );
	if ( NULL == pclDay )
	{
		return;
	}

	CStatic* pclPeriod = m_InterfaceFactory.GetStatic( NACUTRADE_PASSPORT_STATIC_PERIOD );
	if ( NULL == pclPeriod )
	{
		return;
	}

	// 이름
	pclName->SetText( pclchar->pclCI->clWorldPassport.GetPassportName() );

	// 서버 이름
	char szNationCode[4]	= { '\0', };
	CHAR szServerName[20]	= { '\0', };

	pclClient->GetNationCode( szServerName, sizeof(szServerName) );
	pclClient->GetServerNameFromNation( szNationCode, pclClient->siServerUnique,  szServerName );

	pclServerName->SetText( (TCHAR*)szServerName );
	
	// 발급일
	TCHAR szIssueText[128] = { '\0', };
	pclchar->pclCI->clWorldPassport.GetIssueRealDateText( szIssueText, sizeof(szIssueText) );
	pclDay->SetText( szIssueText );

	// 만료일
	TCHAR szExpireText[128] = { '\0', };
	pclchar->pclCI->clWorldPassport.GetExpireRealDateText( szExpireText, sizeof(szExpireText) );
	pclPeriod->SetText( szExpireText );


}

void CNAcuTradeDlg_Passport::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch ( MsgIndex )
	{
		case ACUTRADE_RETMSG_ORDERTOEXTEND:
			{
				if ( false == RetValue )
				{
					return;
				}

				cltMsg clMsg( GAMEMSG_REQUEST_WORLDPASSPORT_EXTEND, 0, NULL );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			break;
	}

}

void CNAcuTradeDlg_Passport::SetController( void )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 내초상
	CStatic* pPortrait = m_InterfaceFactory.GetStatic( NACUTRADE_PASSPORT_STATIC_PORTRAIT );
	if ( pPortrait )
	{
		SI16 siFontIndex = ( pclchar->GetKind() - 1 );
		if ( 0 > siFontIndex ) 
		{
			siFontIndex;
		}

		pPortrait->SetFontIndex( siFontIndex );
		pPortrait->Refresh();
	}

	// 발급비용
	CStatic* pclIssueAcu = m_InterfaceFactory.GetStatic( NACUTRADE_PASSPORT_STATIC_ISSUEACU );
	if ( pclIssueAcu )
	{
		pclIssueAcu->SetText( SI32ToString(Const_Passport_Issue_Cost_Acu) );
	}

	// 연장비용
	CStatic* pclExtendAcu = m_InterfaceFactory.GetStatic( NACUTRADE_PASSPORT_STATIC_EXTENDACU );
	if ( pclExtendAcu )
	{
		pclExtendAcu->SetText( SI32ToString(Const_Passport_Extend_Cost_Acu) );
	}

	// 이름 입력란
	CEdit* pclName = m_InterfaceFactory.GetEdit( NACUTRADE_PASSPORT_EDIT_NAME );
	if ( pclName )
	{
		// (19자 제한)
		pclName->SetMaxEditTextLength( Const_Max_WorldPassport_Name-1 );
	}

	// 알림문 1
	CEdit* pclNotice1 = m_InterfaceFactory.GetEdit( NACUTRADE_PASSPORT_EDIT_NOTICE1 );
	if ( pclNotice1 )
	{
		pclNotice1->SetText( GetTxtFromMgr(8822) );
	}

	// 알림문 2
	CEdit* pclNotice2 = m_InterfaceFactory.GetEdit( NACUTRADE_PASSPORT_EDIT_NOTICE2 );
	if ( pclNotice2 )
	{
		pclNotice2->SetText( GetTxtFromMgr(8823) );
	}

	// 초기값은 비활성화
	CButton* pclIssue = m_InterfaceFactory.GetButton( NACUTRADE_PASSPORT_BUTTON_ISSUE );
	if ( pclIssue )
	{
		pclIssue->Enable( false );
	}

}

void CNAcuTradeDlg_Passport::ShowNotifyMsg( SI32 siTitleNum, SI32 siTextNum )
{
	if ( (0 >= siTitleNum) || (0 >= siTextNum) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	NTCHARString64	strTitle( GetTxtFromMgr(siTitleNum) );
	NTCHARString128	strText( GetTxtFromMgr(siTextNum) );

	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
	
}

void CNAcuTradeDlg_Passport::ShowNotifyMsg( TCHAR* pszTitle, TCHAR* pszText )
{
	if ( (NULL == pszTitle) || (NULL == pszText) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( pszTitle, pszText );
}

void CNAcuTradeDlg_Passport::CheckTheCorrectID( void )
{
	CEdit* pclName = m_InterfaceFactory.GetEdit( NACUTRADE_PASSPORT_EDIT_NAME );
	if ( NULL == pclName )
	{
		return;
	}

	CButton* pclIssue = m_InterfaceFactory.GetButton( NACUTRADE_PASSPORT_BUTTON_ISSUE );
	if ( NULL == pclIssue )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 유효한 이름여부에 따라 버튼이 활성/비활성화 된다
	if ( true == pclchar->pclCI->clWorldPassport.IsCorrectPassportName(pclName->GetText()) )
	{
		pclIssue->Enable( true );
	}
	else
	{
		pclIssue->Enable( false );
	}
}

void CNAcuTradeDlg_Passport::OrderToIssue( void )
{
	// 기본적으로 쓰일 변수들
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 이미 여권이 있다면
	if ( true == pclchar->pclCI->clWorldPassport.IsHavePassport() )
	{
		ShowNotifyMsg( 5614, 8839 );
		return;
	}

	// 갖고 있는 아큐가 발급비용보다 적다
	if ( pclchar->pclCI->clWorldMoney.GetWorldMoney() < Const_Passport_Issue_Cost_Acu )
	{
		ShowNotifyMsg( 6261, 6262 );
		return;
	}

	CEdit* pclName = m_InterfaceFactory.GetEdit( NACUTRADE_PASSPORT_EDIT_NAME );
	if ( NULL == pclName )
	{
		return;	
	}
	
	TCHAR *pszPassportName = pclName->GetText();
	if ( NULL == pszPassportName )
	{
		return;
	}

	// 이름 유효성 체크
	if ( false == pclchar->pclCI->clWorldPassport.IsCorrectPassportName(pszPassportName) )
	{
		SetFocus( pclName );

		ShowNotifyMsg( 836, 8822 );
		return;
	}

	// 실제 패킷 전송
	cltGameMsgRequest_WorldPassport_Set clInfo( pszPassportName );
	cltMsg clMsg( GAMEMSG_REQUEST_WORLDPASSPORT_SET, sizeof(clInfo), (BYTE*)&clInfo );
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
	
}

void CNAcuTradeDlg_Passport::OrderToExtend( void )
{
	// 기본적으로 쓰일 변수들
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 여권이 없는데 연장신청을 한다면
	if ( false == pclchar->pclCI->clWorldPassport.IsHavePassport() )
	{
		ShowNotifyMsg( 8838, 8840 );
		return;
	}

	// 갖고 있는 아큐가 연장비용보다 적다
	if ( pclchar->pclCI->clWorldMoney.GetWorldMoney() < Const_Passport_Extend_Cost_Acu )
	{
		ShowNotifyMsg( 6261, 6262 );
		return;
	}

	NTCHARString64	strTitle( GetTxtFromMgr(8821) );
	NTCHARString128	strText( GetTxtFromMgr(8835) );

	stMsgBox MsgBox;
	MsgBox.Set( this->GetParentDialog(), strTitle, strText, MSGBOX_TYPE_YESNO, ACUTRADE_RETMSG_ORDERTOEXTEND );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}