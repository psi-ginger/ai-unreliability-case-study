#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"
#include "../Client/ninterface/NServerEventDlg/NServerEventRankDlg.h"
#include "../CommonLogic/MsgType-System.h"

extern cltCommonLogic* pclClient;


NServerEventRankDlg::NServerEventRankDlg()
{	
	ZeroMemory(&m_clServerEvent_Rank_Record, sizeof(m_clServerEvent_Rank_Record));

	m_PushButtonIndex = -1;
}

NServerEventRankDlg::~NServerEventRankDlg()
{

}

void NServerEventRankDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NServerEvent/NServerEventRank.ddf" ) );
	file.CreatePopupDialog( this, SERVER_EVENT_RANK_DLG, TEXT( "dialog_server_event_rank" ), StaticNServerEventRankDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	SERVER_RANK_BUTTON,			this ), SERVER_RANK_BUTTON,			TEXT( "button_server_rank"		));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	SERVER_DAILY_RANK_BUTTON,	this ), SERVER_DAILY_RANK_BUTTON,	TEXT( "button_server_daily_rank"));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	PERSONAL_RANK_BUTTON,		this ), PERSONAL_RANK_BUTTON,		TEXT( "button_personal_rank"	));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	SIZE_BIG_BUTTON,			this ), SIZE_BIG_BUTTON,			TEXT( "button_big"				));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	SIZE_SMALL_BUTTON,			this ), SIZE_SMALL_BUTTON,			TEXT( "button_small"			));	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,	RANK_LIST,					this ), RANK_LIST,					TEXT( "listbox_rank"			));	

	// 서버 이벤트 랭킹 정보 요청.
	/*cltGameMsgRequest_Server_Event_Rank_Info	clRankInfo;
	cltMsg clMsg(GAMEMSG_REQUEST_SERVER_EVENT_RANK_INFO, sizeof(clRankInfo), (BYTE*)&clRankInfo);
	pclClient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );*/

		SizeChange(SIZE_SMALL); 
	ChangeRankList(SERVER_DAILRY_RANK);
	ButtonSetting(SERVER_DAILRY_RANK);

	m_InterfaceFactory.GetButton(SERVER_RANK_BUTTON)->Show( false );
	m_InterfaceFactory.GetButton(PERSONAL_RANK_BUTTON)->Show( false );
}

void NServerEventRankDlg::ChangeRankList(SI32 _siType)
{
	if(SERVER_RANK == _siType || SERVER_DAILRY_RANK == _siType)
	{
		//m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(0,  35, GetTxtFromMgr(5398));
		m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(0, 120, GetTxtFromMgr(40841));
		m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(1, 120, GetTxtFromMgr(1586));
		//m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(3, 100, GetTxtFromMgr(8548));
	}
	else
	{
		//m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(0,  35, GetTxtFromMgr(5398));
		m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(1,  120, GetTxtFromMgr(1556));
		m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(2,  120, GetTxtFromMgr(1506));
		//m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(3, 140, GetTxtFromMgr(1586));
	}

	m_InterfaceFactory.GetList(RANK_LIST)->SetFontSize( 15 );
	m_InterfaceFactory.GetList(RANK_LIST)->SetBorder(TRUE);
	m_InterfaceFactory.GetList(RANK_LIST)->SetBKColorUse(TRUE);
	m_InterfaceFactory.GetList(RANK_LIST)->Refresh();

	SetChangeRankList(_siType);
}

void NServerEventRankDlg::SetChangeRankList(SI32 _siType)
{
	CList* pList = m_InterfaceFactory.GetList(RANK_LIST);
	SAFE_POINTER(pList)
		pList->Clear();

	stListBoxItemData itemData;
	TCHAR			  szBuf[32] = "";

	// 서버 랭킹 정보
	if(SERVER_RANK == _siType)
	{
		//CNServerRankInfo* pServerRank = NULL;
		//for(SI32 Count = 0; Count < SERVER_MAX_NUM; Count++)
		//{
		//	pServerRank = m_clServerEvent_Rank_Record.GetServerRankInfo(Count);
		//	if(_tcslen(pServerRank->GetServerName()) > 0)
		//	{
		//		itemData.Init();
		//		ZeroMemory(szBuf, sizeof(szBuf));

		//		//_itot( Count+1, szBuf, 10 );
		//		//itemData.Set( 0, szBuf);

		//		itemData.Set( 0, pServerRank->GetServerName() );

		//		_itot( pServerRank->GetScore(), szBuf, 10 );
		//		itemData.Set( 1, szBuf );

		//		pList->AddItem( &itemData );
		//	}
		//}
	}
	// 하루 서버 랭킹 정보
	else if(SERVER_DAILRY_RANK == _siType)
	{
		CNServerDailyRankInfo clServerDailyRank[2];

		if( 0 >= m_clServerEvent_Rank_Record.GetServerDailyRankInfo(0)->GetServerUnique() )
		{
			clServerDailyRank[0] = *m_clServerEvent_Rank_Record.GetServerDailyRankInfo(0);
			clServerDailyRank[1] = *m_clServerEvent_Rank_Record.GetServerDailyRankInfo(1);
		}
		else
		{
			clServerDailyRank[0] = *m_clServerEvent_Rank_Record.GetServerDailyRankInfo(1);
			clServerDailyRank[1] = *m_clServerEvent_Rank_Record.GetServerDailyRankInfo(0);
		}

		//m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(0, 120, clServerDailyRank[0].GetServerName());
		m_InterfaceFactory.GetList(RANK_LIST)->SetColumn(1, 120, clServerDailyRank[1].GetServerName());
		m_InterfaceFactory.GetList(RANK_LIST)->SetBorder(TRUE);
		m_InterfaceFactory.GetList(RANK_LIST)->SetBKColorUse(TRUE);
		m_InterfaceFactory.GetList(RANK_LIST)->Refresh();

		itemData.Init();
		ZeroMemory(szBuf, sizeof(szBuf));

		_itot( clServerDailyRank[0].GetScore(), szBuf, 10 );
		itemData.Set( 0, szBuf );

		_itot( clServerDailyRank[1].GetScore(), szBuf, 10 );
		itemData.Set( 1, szBuf );

		pList->AddItem( &itemData );


		//for(SI32 Count = 0; Count < SERVER_MAX_NUM; Count++)
		//{
		//	if(_tcslen(clServerDailyRank[Count].GetServerName()) > 0)
		//	{
		//		itemData.Init();
		//		ZeroMemory(szBuf, sizeof(szBuf));

		//		/*_itot( Count+1, szBuf, 10 );
		//		itemData.Set( 0, szBuf);*/

		//		itemData.Set( 0, clServerDailyRank[Count].GetServerName() );

		//		_itot( clServerDailyRank[Count].GetScore(), szBuf, 10 );
		//		itemData.Set( 1, szBuf );

		//		/*_itot( pServerDailyRank->GetHuntingGroundCount(), szBuf, 10);
		//		itemData.Set( 3, szBuf );*/

		//		pList->AddItem( &itemData );
		//	}
		//}
	}
	// 개인 랭킹 정보.
	else if(PERSONAL_RANK == _siType)
	{
		//CNPersonalRankInfo* pPersonalRank = NULL;
		//for(SI32 Count = 0; Count < USER_MAX_NUM; Count++)
		//{
		//	pPersonalRank = m_clServerEvent_Rank_Record.GetPersonalRankInfo(Count);
		//	if(_tcslen(pPersonalRank->GetPersonalName()) > 0)
		//	{
		//		itemData.Init();
		//		ZeroMemory(szBuf, sizeof(szBuf));
		//		
		//		/*_itot( Count+1, szBuf, 10 );
		//		itemData.Set( 0, szBuf);*/

		//		itemData.Set( 0, pPersonalRank->GetPersonalName());

		//		_itot( pPersonalRank->GetLevel(), szBuf, 10 );
		//		itemData.Set( 1, szBuf );
		//		
		//		/*_itot( pPersonalRank->GetScore(), szBuf, 10);
		//		itemData.Set( 3, szBuf );*/

		//		pList->AddItem( &itemData );
		//	}
		//}
	}

	pList->Refresh();
}

void NServerEventRankDlg::SizeChange(bool _SizeType)
{
	if(_SizeType == SIZE_BIG)
	{
		m_InterfaceFactory.GetButton(SIZE_BIG_BUTTON)->Show(false);
		m_InterfaceFactory.GetButton(SIZE_SMALL_BUTTON)->Show(true);
		//m_InterfaceFactory.GetButton(SERVER_RANK_BUTTON)->Show(true);
		//m_InterfaceFactory.GetButton(SERVER_DAILY_RANK_BUTTON)->Show(true);
		//m_InterfaceFactory.GetButton(PERSONAL_RANK_BUTTON)->Show(true);
		m_InterfaceFactory.GetList(RANK_LIST)->Show(true);
		SetWidth(255);
		SetHeight(83);   

		SetX(390);  
		SetY(95);  

		// 글로벌은 서버 총 점수가 나오지 않는다.
		if( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_NHNChina | ConstServiceArea_EUROPE) )
		{
			m_InterfaceFactory.GetButton(SERVER_RANK_BUTTON)->Show(false);
		}
	} 
	else if(_SizeType == SIZE_SMALL)
	{
		m_InterfaceFactory.GetButton(SIZE_BIG_BUTTON)->Show(true);
		m_InterfaceFactory.GetButton(SIZE_SMALL_BUTTON)->Show(false);
		m_InterfaceFactory.GetButton(SERVER_RANK_BUTTON)->Show(false);
		m_InterfaceFactory.GetButton(SERVER_DAILY_RANK_BUTTON)->Show(false);
		m_InterfaceFactory.GetButton(PERSONAL_RANK_BUTTON)->Show(false);
		m_InterfaceFactory.GetList(RANK_LIST)->Show(false);
		SetWidth(38); 
		SetHeight(38);    

		SetX(425);    
		SetY(645);
	}
}

void CALLBACK NServerEventRankDlg::StaticNServerEventRankDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NServerEventRankDlg *pThis = (NServerEventRankDlg*) pControllerMgr;
	pThis->NNServerEventRankDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NServerEventRankDlg::NNServerEventRankDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch ( nControlID )
	{

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;	
	case SERVER_RANK_BUTTON:
		{
			ChangeRankList(SERVER_RANK);
			ButtonSetting(SERVER_RANK);
		}
		break;
	case PERSONAL_RANK_BUTTON:
		{
			ChangeRankList(PERSONAL_RANK);
			ButtonSetting(PERSONAL_RANK);
		}
		break;
	case SERVER_DAILY_RANK_BUTTON:
		{
			ChangeRankList(SERVER_DAILRY_RANK);
			ButtonSetting(SERVER_DAILRY_RANK);
		}
		break;
	case SIZE_BIG_BUTTON:
		{
			SizeChange(SIZE_BIG);
		}
		break;
	case SIZE_SMALL_BUTTON:
		{
			SizeChange(SIZE_SMALL);
		}
		break;
	}	
}

void NServerEventRankDlg::ButtonSetting( SI32 _ButtonKind )
{
	// 버튼 인덱스 번호
	m_PushButtonIndex = _ButtonKind;

	CButton* pButton = NULL;
	for( int count = SERVER_RANK_BUTTON; count <= PERSONAL_RANK_BUTTON; ++count )
	{
		pButton = m_InterfaceFactory.GetButton( count );
		//pButton->SetFontSize(11); 
		pButton->SetFontWeight(FW_NORMAL);
		pButton->SetFontColor(RGB(120, 120, 120));           
		pButton->Refresh();      
	}

	SI32 TypeID = -1; 
	switch( _ButtonKind )  
	{
	case SERVER_RANK:			TypeID = SERVER_RANK_BUTTON;		break;
	case PERSONAL_RANK:			TypeID = PERSONAL_RANK_BUTTON;		break;
	case SERVER_DAILRY_RANK:	TypeID = SERVER_DAILY_RANK_BUTTON;	break;
	}

	pButton = m_InterfaceFactory.GetButton( TypeID );
	if(-1 == TypeID)
		return; 

	//pButton->SetFontSize(13);
	pButton->SetFontWeight(FW_BOLD);
	pButton->SetFontColor(RGB(0 ,0 ,0));
	pButton->Refresh();  
}

void NServerEventRankDlg::SetServerEventRankInfo(CNServerEvent_Rank_Record*	_pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

		memcpy(&m_clServerEvent_Rank_Record, _pRankInfo, sizeof(CNServerEvent_Rank_Record));

	// 디버그가 아니면 서버 이름을 세팅한다.
#ifndef _DEBUG
	TCHAR* pServerName = NULL;
	cltClient * pclclient = (cltClient*) pclClient;
	for(SI32 Count = 0; Count < SERVER_MAX_NUM; ++Count)	
	{
		pServerName = pclclient->GetServerNameFromUnique(m_clServerEvent_Rank_Record.GetServerRankInfo(Count)->GetServerUnique());
		m_clServerEvent_Rank_Record.GetServerRankInfo(Count)->SetServerName(pServerName);

		pServerName = pclclient->GetServerNameFromUnique(m_clServerEvent_Rank_Record.GetServerDailyRankInfo(Count)->GetServerUnique());
		m_clServerEvent_Rank_Record.GetServerDailyRankInfo(Count)->SetServerName(pServerName);
	}
#endif

	// 처음 세틴될 때는 하루 정보를 보여주게 한다. 
	if(-1 == m_PushButtonIndex)
		m_PushButtonIndex = SERVER_DAILRY_RANK;

	// 눌린 버튼에 해당하는 정보 갱신.
	SetChangeRankList(m_PushButtonIndex);
}

void NServerEventRankDlg::SetServerEventServerInfo(CNServerRankInfo* _pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

		memcpy(m_clServerEvent_Rank_Record.GetServerRankInfo(0), _pRankInfo, sizeof(CNServerRankInfo) * SERVER_MAX_NUM);
}

void NServerEventRankDlg::SetServerEventDailyServerInfo(CNServerDailyRankInfo* _pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

		memcpy(m_clServerEvent_Rank_Record.GetServerDailyRankInfo(0), _pRankInfo, sizeof(CNServerDailyRankInfo) * SERVER_MAX_NUM);
}

void NServerEventRankDlg::SetServerEventPersonalInfo(CNPersonalRankInfo* _pRankInfo)
{
	SAFE_POINTER(_pRankInfo)

		memcpy(m_clServerEvent_Rank_Record.GetPersonalRankInfo(0), _pRankInfo, sizeof(CNPersonalRankInfo) * USER_MAX_NUM);
}

