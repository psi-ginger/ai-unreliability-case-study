

#include "../../../Client.h"
#include "../../../InterfaceMgr/InterfaceFile.h"
#include "MyGuildMasterStatus.h"

#include "MsgType-Guild.h"


extern cltCommonLogic* pclClient;

CNMyGuildStatusStatus::CNMyGuildStatusStatus()
{
	Initialize();
}	

CNMyGuildStatusStatus::~CNMyGuildStatusStatus()
{
	Destroy();
}

void CNMyGuildStatusStatus::Initialize()
{

}

void CNMyGuildStatusStatus::Destroy()
{

}

void CNMyGuildStatusStatus::Create( CControllerMgr *pParent )
{

	cltClient *pclclient = (cltClient *)pclClient;

	if ( ! IsCreate() )
	{
		CInterfaceFile file;

		file.LoadFile(TEXT("NInterface/Data/NStructureDlg/Guild/DLG_MyGuildMasterStatus.ddf"));

		file.CreateChildDialog( this, NMYGUILDMASTERSTATUS_DLG, TEXT("dialog_MyGuildMasterStatus"),StaticMyGuildMasterStatusDlgProc , pParent);
		
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERSTATUS_STATIC_1 , this)  , NMYGUILDMASTERSTATUS_STATIC_1 , TEXT("statictext_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERSTATUS_STATIC_2 , this)  , NMYGUILDMASTERSTATUS_STATIC_2 , TEXT("statictext_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERSTATUS_STATIC_3 , this)  , NMYGUILDMASTERSTATUS_STATIC_3 , TEXT("statictext_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERSTATUS_STATIC_4 , this)  , NMYGUILDMASTERSTATUS_STATIC_4 , TEXT("statictext_NONAME4"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERSTATUS_STATIC_5 , this)  , NMYGUILDMASTERSTATUS_STATIC_5 , TEXT("statictext_NONAME5"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERSTATUS_STATIC_6 , this)  , NMYGUILDMASTERSTATUS_STATIC_6 , TEXT("statictext_NONAME6"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC ,NMYGUILDMASTERSTATUS_STATIC_7 , this)  , NMYGUILDMASTERSTATUS_STATIC_7 , TEXT("statictext_NONAME7"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERSTATUS_EDIT_1, this )   , NMYGUILDMASTERSTATUS_EDIT_1 , TEXT("editbox_NONAME1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERSTATUS_EDIT_2, this )   , NMYGUILDMASTERSTATUS_EDIT_2 , TEXT("editbox_NONAME2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERSTATUS_EDIT_3, this )   , NMYGUILDMASTERSTATUS_EDIT_3 , TEXT("editbox_NONAME3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERSTATUS_EDIT_4, this )   , NMYGUILDMASTERSTATUS_EDIT_4 , TEXT("editbox_NONAME4"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERSTATUS_EDIT_5, this )   , NMYGUILDMASTERSTATUS_EDIT_5 , TEXT("editbox_NONAME5"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERSTATUS_EDIT_6, this )   , NMYGUILDMASTERSTATUS_EDIT_6 , TEXT("editbox_NONAME6"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT , NMYGUILDMASTERSTATUS_EDIT_7, this )   , NMYGUILDMASTERSTATUS_EDIT_7 , TEXT("editbox_NONAME7"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_PLUS_1, this )   , NMYGUILDMASTERSTATUS_BUTTON_PLUS_1 , TEXT("button_plus1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_PLUS_2, this )   , NMYGUILDMASTERSTATUS_BUTTON_PLUS_2 , TEXT("button_plus2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_PLUS_3, this )   , NMYGUILDMASTERSTATUS_BUTTON_PLUS_3 , TEXT("button_plus3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_PLUS_4, this )   , NMYGUILDMASTERSTATUS_BUTTON_PLUS_4 , TEXT("button_plus4"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_PLUS_5, this )   , NMYGUILDMASTERSTATUS_BUTTON_PLUS_5 , TEXT("button_plus5"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_PLUS_6, this )   , NMYGUILDMASTERSTATUS_BUTTON_PLUS_6 , TEXT("button_plus6"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_MINUS_1, this )   , NMYGUILDMASTERSTATUS_BUTTON_MINUS_1, TEXT("button_minus1"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_MINUS_2, this )   , NMYGUILDMASTERSTATUS_BUTTON_MINUS_2, TEXT("button_minus2"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_MINUS_3, this )   , NMYGUILDMASTERSTATUS_BUTTON_MINUS_3, TEXT("button_minus3"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_MINUS_4, this )   , NMYGUILDMASTERSTATUS_BUTTON_MINUS_4, TEXT("button_minus4"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_MINUS_5, this )   , NMYGUILDMASTERSTATUS_BUTTON_MINUS_5, TEXT("button_minus5"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_MINUS_6, this )   , NMYGUILDMASTERSTATUS_BUTTON_MINUS_6, TEXT("button_minus6"));
		

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_CANCEL, this )   , NMYGUILDMASTERSTATUS_BUTTON_CANCEL, TEXT("button_cancel"));
		file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON , NMYGUILDMASTERSTATUS_BUTTON_ACCEPT, this )   , NMYGUILDMASTERSTATUS_BUTTON_ACCEPT, TEXT("button_accept"));

		file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE, NMYGUILDMASTERSTATUS_OUTLINE1, this	 )		, NMYGUILDMASTERSTATUS_OUTLINE1, TEXT("outline_NONAME1"));

		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_5)->Enable(false);
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_6)->Enable(false);
	}

	LoadStatus();
}

void CALLBACK CNMyGuildStatusStatus::StaticMyGuildMasterStatusDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNMyGuildStatusStatus *pThis = (CNMyGuildStatusStatus*) pControllerMgr;
	pThis->NMyGuildMasterStatusDlgProc( nEvent, nControlID, pControl );

}

void CALLBACK CNMyGuildStatusStatus::NMyGuildMasterStatusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
	case NMYGUILDMASTERSTATUS_BUTTON_PLUS_1:
	case NMYGUILDMASTERSTATUS_BUTTON_PLUS_2:
	case NMYGUILDMASTERSTATUS_BUTTON_PLUS_3:
	case NMYGUILDMASTERSTATUS_BUTTON_PLUS_4:
	case NMYGUILDMASTERSTATUS_BUTTON_PLUS_5:
	case NMYGUILDMASTERSTATUS_BUTTON_PLUS_6:
		if( EVENT_BUTTON_CLICKED == nEvent )
		{
			ChangeStatus( nControlID, CHANGESTATUS_PLUS );
		}
		break;
	case NMYGUILDMASTERSTATUS_BUTTON_MINUS_1:
	case NMYGUILDMASTERSTATUS_BUTTON_MINUS_2:
	case NMYGUILDMASTERSTATUS_BUTTON_MINUS_3:
	case NMYGUILDMASTERSTATUS_BUTTON_MINUS_4:
	case NMYGUILDMASTERSTATUS_BUTTON_MINUS_5:
	case NMYGUILDMASTERSTATUS_BUTTON_MINUS_6:
		if( EVENT_BUTTON_CLICKED == nEvent )
		{
			ChangeStatus( nControlID, CHANGESTATUS_MINUS );
		}
		break;
	case NMYGUILDMASTERSTATUS_BUTTON_CANCEL:
		if( EVENT_BUTTON_CLICKED == nEvent )
		{
			LoadStatus();
			RefreshStatus();
		}
		break;
	case NMYGUILDMASTERSTATUS_BUTTON_ACCEPT:
		if( EVENT_BUTTON_CLICKED == nEvent )
		{
			SendStatusToServer();				//서버로 현제 스테이타스를 보낸다.
		}
		break;
	}
}


void CNMyGuildStatusStatus::Hide()
{
	CControllerMgr::Show(false);

}

void CNMyGuildStatusStatus::Show()
{
	CControllerMgr::Show(true);

}

void CNMyGuildStatusStatus::LoadStatus()
{
	cltClient * pclclient = (cltClient*) pclClient;
	cltGuildManager *pGuildManager = ((cltGuildManager *)pclClient->pclGuildManager);

	memcpy(&clStatus,&pGuildManager->clCommonUnit.clStatus,sizeof(cltGuildStatus));
	memcpy(&clChangedStatus, &clStatus, sizeof(cltGuildStatus));
}

void CNMyGuildStatusStatus::ChangeStatus( SI32 controlID, SI16 index )
{
	switch( index )
	{
	case CHANGESTATUS_PLUS:
		if( clChangedStatus.siBonus <= 0 )
			return;

		switch( controlID )
		{
		case NMYGUILDMASTERSTATUS_BUTTON_PLUS_1:
			clChangedStatus.siStr++;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_PLUS_2:
			clChangedStatus.siDex++;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_PLUS_3:
			clChangedStatus.siMag++;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_PLUS_4:
			clChangedStatus.siVit++;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_PLUS_5:
			clChangedStatus.siHnd++;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_PLUS_6:
			clChangedStatus.siLuk++;
			break;
		}
		clChangedStatus.siBonus--;
		break;
	case CHANGESTATUS_MINUS:
		switch( controlID )
		{
		case NMYGUILDMASTERSTATUS_BUTTON_MINUS_1:
			if( clChangedStatus.siStr <= 0 )
				return;
			clChangedStatus.siStr--;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_MINUS_2:
			if( clChangedStatus.siDex <= 0 )
				return;
			clChangedStatus.siDex--;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_MINUS_3:
			if( clChangedStatus.siMag <= 0 )
				return;
			clChangedStatus.siMag--;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_MINUS_4:
			if( clChangedStatus.siVit <= 0 )
				return;
			clChangedStatus.siVit--;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_MINUS_5:
			if( clChangedStatus.siHnd <= 0 )
				return;
			clChangedStatus.siHnd--;
			break;
		case NMYGUILDMASTERSTATUS_BUTTON_MINUS_6:
			if( clChangedStatus.siLuk <= 0 )
				return;
			clChangedStatus.siLuk--;
			break;
		}
		clChangedStatus.siBonus++;
		break;
	}

	RefreshStatus();
}


void CNMyGuildStatusStatus::RefreshStatus()//길드스탯고치는곳 그런데 머가먼지 모르겠다.dfg봐야지
{
	TCHAR buffer[16] = "";

	// 에디트 박스에 출력
	StringCchPrintf( buffer, 16, TEXT(" %d"), clChangedStatus.siStr );
	m_InterfaceFactory.GetEdit(NMYGUILDMASTERSTATUS_EDIT_1)->SetText( buffer );

	StringCchPrintf( buffer, 16, TEXT(" %d"), clChangedStatus.siDex );
	m_InterfaceFactory.GetEdit(NMYGUILDMASTERSTATUS_EDIT_2)->SetText( buffer );

	StringCchPrintf( buffer, 16, TEXT(" %d"), clChangedStatus.siMag );
	m_InterfaceFactory.GetEdit(NMYGUILDMASTERSTATUS_EDIT_3)->SetText( buffer );

	StringCchPrintf( buffer, 16, TEXT(" %d"), clChangedStatus.siVit );
	m_InterfaceFactory.GetEdit(NMYGUILDMASTERSTATUS_EDIT_4)->SetText( buffer );

	StringCchPrintf( buffer, 16, TEXT(" %d"), clChangedStatus.siHnd );
	m_InterfaceFactory.GetEdit(NMYGUILDMASTERSTATUS_EDIT_5)->SetText( buffer );

	StringCchPrintf( buffer, 16, TEXT(" %d"), clChangedStatus.siLuk );
	m_InterfaceFactory.GetEdit(NMYGUILDMASTERSTATUS_EDIT_6)->SetText( buffer );

	StringCchPrintf( buffer, 16, TEXT(" %d"), clChangedStatus.siBonus );
	m_InterfaceFactory.GetEdit(NMYGUILDMASTERSTATUS_EDIT_7)->SetText( buffer );

	// PLUS버튼 enable, disable
	if( clChangedStatus.siBonus <= 0 )
	{
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_1)->Enable( false );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_2)->Enable( false );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_3)->Enable( false );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_4)->Enable( false );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_5)->Enable( false );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_6)->Enable( false );
	}
	else
	{
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_1)->Enable( true );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_2)->Enable( true );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_3)->Enable( true );
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_4)->Enable( true );
		if (pclClient->IsCountrySwitch(Switch_ManufactureRevitalize)) {
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_5)->Enable( false );//손재주 행운 Disable [2007.08.06]
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_6)->Enable( false );//손재주 행운 Disable [2007.08.06]
		}
		else{
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_5)->Enable( true );
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_PLUS_6)->Enable( true );
		}
		
	}

	// MINUS버튼 enable, disable
	if( clChangedStatus.siStr <= 0 )
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_1)->Enable( false );
	else
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_1)->Enable( true );

	if( clChangedStatus.siDex <= 0 )
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_2)->Enable( false );
	else
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_2)->Enable( true );

	if( clChangedStatus.siMag <= 0 )
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_3)->Enable( false );
	else
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_3)->Enable( true );

	if( clChangedStatus.siVit <= 0 )
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_4)->Enable( false );
	else
		m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_4)->Enable( true );

	if (pclClient->IsCountrySwitch(Switch_ManufactureRevitalize))
	{
		if( clChangedStatus.siHnd <= 0 )
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_5)->Enable( false );
		else
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_5)->Enable( false );//손재주 행운 Disable [2007.08.06]

		if( clChangedStatus.siLuk <= 0 )
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_6)->Enable( false );
		else
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_6)->Enable( false );//손재주 행운 Disable [2007.08.06]
	}
	else
	{
		if( clChangedStatus.siHnd <= 0 )
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_5)->Enable( false );
		else
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_5)->Enable( true );

		if( clChangedStatus.siLuk <= 0 )
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_6)->Enable( false );
		else
			m_InterfaceFactory.GetButton(NMYGUILDMASTERSTATUS_BUTTON_MINUS_6)->Enable( true );


	}
}

void CNMyGuildStatusStatus::SendStatusToServer()
{
	//서버로 보낸다. clStatus가 아닌 clChangedStatus를 보내주세요!

	cltGameMsgRequest_Guild_ChangeStatus clinfo(&clChangedStatus);
	cltMsg clMsg(GAMEMSG_REQUEST_GUILD_CHANGESTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	return;
}