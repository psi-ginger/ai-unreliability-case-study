
#include "NPVPLeageuDlg.h"

#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../Client.h"

#include "Char/CharManager/CharManager.h"
#include "../common/Event/event.h"

#include "../CommonLogic/MsgType-Person.h"

extern cltCommonLogic* pclClient;

CNPVPLeageuDlg::CNPVPLeageuDlg( void )
{

}

CNPVPLeageuDlg::~CNPVPLeageuDlg( void )
{

}

void CNPVPLeageuDlg::Init( )
{

}

void CNPVPLeageuDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPVPLeagueDlg/DLG_NPVPLeague.ddf" ) );
	file.CreatePopupDialog( this, PVPLEAGUE_DLG, TEXT( "dialog_pvpleague" ), StaticCallBackDialogNPVPLeageuDlg );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_PORTRAIT,	this), PVPLEAGUE_DLG_STATICTEXT_PORTRAIT,	TEXT("statictext_portrait") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NAME,		this), PVPLEAGUE_DLG_STATICTEXT_NAME,		TEXT("statictext_name") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_LEVEL,		this), PVPLEAGUE_DLG_STATICTEXT_LEVEL,		TEXT("statictext_level") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_SCORE,		this), PVPLEAGUE_DLG_STATICTEXT_SCORE,		TEXT("statictext_score") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_GRADE,		this), PVPLEAGUE_DLG_STATICTEXT_GRADE,		TEXT("statictext_grade") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME3,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME3,	TEXT("statictext_NONAME3") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME4,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME4,	TEXT("statictext_NONAME4") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME5,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME5,	TEXT("statictext_NONAME5") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME6,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME6,	TEXT("statictext_NONAME6") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME2,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME2,	TEXT("statictext_NONAME2") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME11,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME11,	TEXT("statictext_NONAME11") );
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME12,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME12,	TEXT("statictext_NONAME12") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(STATIC,	PVPLEAGUE_DLG_STATICTEXT_NONAME13,	this), PVPLEAGUE_DLG_STATICTEXT_NONAME13,	TEXT("statictext_NONAME13") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	PVPLEAGUE_DLG_BUTTON_REQUEST,		this), PVPLEAGUE_DLG_BUTTON_REQUEST,		TEXT("button_request") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	PVPLEAGUE_DLG_BUTTON_CANCEL,		this), PVPLEAGUE_DLG_BUTTON_CANCEL,			TEXT("button_cancel") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST,	PVPLEAGUE_DLG_LISTBOX_SCORE,		this), PVPLEAGUE_DLG_LISTBOX_SCORE,			TEXT("listbox_score") );
	//file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST,	PVPLEAGUE_DLG_LISTBOX_RIVAL,		this), PVPLEAGUE_DLG_LISTBOX_RIVAL,			TEXT("listbox_rival") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(LIST,	PVPLEAGUE_DLG_LISTBOX_TOP,			this), PVPLEAGUE_DLG_LISTBOX_TOP,			TEXT("listbox_top") );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,	PVPLEAGUE_DLG_OUTLINE_NONAME1,		this), PVPLEAGUE_DLG_OUTLINE_NONAME1,		TEXT("outline_NONAME1") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(OUTLINE,	PVPLEAGUE_DLG_OUTLINE_NONAME2,		this), PVPLEAGUE_DLG_OUTLINE_NONAME2,		TEXT("outline_NONAME2") );

	//-- 컨트롤러 설정
	TCHAR*	pszText		= NULL;
	SI16	siColumn0	= 0;	// 이름 칸은 유동적으로 변할 수 있도록 생성시에 결정된다
	SI16	siColumn1	= 40;
	SI16	siColumn2	= 40;
	SI16	siColumn3	= 80;
	SI16	siColumn4	= 40;

	// 최근 5 경기
	CList* pScoreList = m_InterfaceFactory.GetList( PVPLEAGUE_DLG_LISTBOX_SCORE );
	if ( pScoreList )
	{
		siColumn0 = pScoreList->GetWidth() - siColumn1 - siColumn2 - siColumn3 - siColumn4;

		pszText = GetTxtFromMgr( 1556 );
		pScoreList->SetColumn( 0, siColumn0, pszText );

		pszText = GetTxtFromMgr( 1506 );
		pScoreList->SetColumn( 1, siColumn1, pszText );

		pszText = GetTxtFromMgr( 9585 );
		pScoreList->SetColumn( 2, siColumn2, pszText );

		pszText = GetTxtFromMgr( 9586 );
		pScoreList->SetColumn( 3, siColumn3, pszText );

		pszText = GetTxtFromMgr( 9587 );
		pScoreList->SetColumn( 4, siColumn4, pszText );

		pScoreList->SetBorder(TRUE);
		pScoreList->SetBKColorUse(TRUE);
		pScoreList->Refresh();
	}

	/*
	// 라이벌
	CList* pRivalList = m_InterfaceFactory.GetList( PVPLEAGUE_DLG_LISTBOX_RIVAL );
	if ( pRivalList )
	{
		siColumn0 = pRivalList->GetWidth() - siColumn1 - siColumn2 - siColumn3 - siColumn4;

		pszText = GetTxtFromMgr( 1556 );
		pRivalList->SetColumn( 0, siColumn0, pszText );

		pszText = GetTxtFromMgr( 1506 );
		pRivalList->SetColumn( 1, siColumn1, pszText );

		pszText = GetTxtFromMgr( 9585 );
		pRivalList->SetColumn( 2, siColumn2, pszText );

		pszText = GetTxtFromMgr( 9586 );
		pRivalList->SetColumn( 3, siColumn3, pszText );

		pszText = GetTxtFromMgr( 9594 );
		pRivalList->SetColumn( 4, siColumn4, pszText );

		pRivalList->SetBorder(TRUE);
		pRivalList->SetBKColorUse(TRUE);
		pRivalList->Refresh();
	}
	*/

		// Top 10
		CList* pTopList = m_InterfaceFactory.GetList( PVPLEAGUE_DLG_LISTBOX_TOP );
	if ( pTopList )
	{
		siColumn0 = pTopList->GetWidth() - siColumn1 - siColumn2 - siColumn3 - siColumn4;

		pszText = GetTxtFromMgr( 1556 );
		pTopList->SetColumn( 0, siColumn0, pszText );

		pszText = GetTxtFromMgr( 1506 );
		pTopList->SetColumn( 1, siColumn1, pszText );

		pszText = GetTxtFromMgr( 9585 );
		pTopList->SetColumn( 2, siColumn2, pszText );

		pszText = GetTxtFromMgr( 9586 );
		pTopList->SetColumn( 3, siColumn3, pszText );

		pszText = GetTxtFromMgr( 9594 );
		pTopList->SetColumn( 4, siColumn4, pszText );

		pTopList->SetBorder(TRUE);
		pTopList->SetBKColorUse(TRUE);
		pTopList->Refresh();
	}

	//-- 초기설정
	DefaultSetting();

	//-- 다이얼로그에 필요한 정보들을 서버로 요청한다
	RequestPVPLeagueUIInfo();

}

void CALLBACK CNPVPLeageuDlg::StaticCallBackDialogNPVPLeageuDlg( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNPVPLeageuDlg* pThis = (CNPVPLeageuDlg*)pControllerMgr;
	if ( pThis )
	{
		pThis->CallBackDialogNPVPLeageuDlg( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNPVPLeageuDlg::CallBackDialogNPVPLeageuDlg( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch( nControlID )
	{
	case PVPLEAGUE_DLG_BUTTON_REQUEST:
		{
			OnButtonClicked_Apply();
		}
		break;

	case PVPLEAGUE_DLG_BUTTON_CANCEL:
		{
			OnButtonClicked_Cancel();
		}
		break;

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	}

}

void CNPVPLeageuDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( false == RetValue )
	{
		return;
	}

	switch ( MsgIndex )
	{
	case PVPLEAGUE_RETMSG_APPLY:
		{
			cltMsg clMsg( GAMEMSG_REQUEST_PVP_LEAGUE_APPLY, 0, NULL );
			pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
		break;

	case PVPLEAGUE_RETMSG_CANCEL:
		{
			cltMsg clMsg( GAMEMSG_REQUEST_PVP_LEAGUE_CANCEL, 0, NULL );
			pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
		break;
	}

}

void CNPVPLeageuDlg::SetPVPLeagueInfo( cltGameMsgResponse_PVP_League_UI_InfoGet* pclInfo )
{
	if ( NULL == pclInfo )
	{
		return;
	}

	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	//------------------------------------------------
	// 전적 관련
	//------------------------------------------------
	CStatic* pScoreStatic = m_InterfaceFactory.GetStatic( PVPLEAGUE_DLG_STATICTEXT_SCORE );
	if ( pScoreStatic )
	{
		NTCHARString128	strMyScore	= GetTxtFromMgr( 9604 );
		SI32			siPOV		= GetPercentageOfVictories( pclInfo->m_siWin, pclInfo->m_siLose, pclInfo->m_siDraw );

		strMyScore.Replace( TEXT("#win#"),	SI32ToString(pclInfo->m_siWin) );
		strMyScore.Replace( TEXT("#draw#"),	SI32ToString(pclInfo->m_siDraw) );
		strMyScore.Replace( TEXT("#lose#"),	SI32ToString(pclInfo->m_siLose) );
		strMyScore.Replace( TEXT("#pov#"),	SI32ToString(siPOV) );

		pScoreStatic->SetText( strMyScore );
		pScoreStatic->Refresh();
	}

	//------------------------------------------------
	// 등급관련
	//------------------------------------------------
	CStatic* pGradeStatic = m_InterfaceFactory.GetStatic( PVPLEAGUE_DLG_STATICTEXT_GRADE );
	if ( pGradeStatic )
	{
		NTCHARString128	strMyGrade	= GetTxtFromMgr( 9605 );
		strMyGrade.Replace( TEXT("#grade#"), SI32ToString( GetGrade( pclchar->GetPVP_LeagueGrade() ) ) );
		strMyGrade.Replace( TEXT("#group#"), SI32ToString(pclInfo->m_siGroupNum) );

		pGradeStatic->SetText( strMyGrade );
		pGradeStatic->Refresh();
	}

	// 기본적으로 있어야할 변수들
	stListBoxItemData				stItemData;
	cltPVP_League_OtherUserInfo*	pclPVPLeagueInfo = NULL;

	//------------------------------------------------
	// 최근 5경기 현황
	//------------------------------------------------
	CList* pclLatestList = m_InterfaceFactory.GetList( PVPLEAGUE_DLG_LISTBOX_SCORE );
	if ( pclLatestList )
	{
		for ( SI32 siIndex=0; siIndex<Const_PVP_League_Latest_Max; ++siIndex ) 
		{
			if ( 0 >= pclInfo->m_clLatest[siIndex].siLevel )
			{
				continue;
			}

			// 초기화
			stItemData.Init();
			pclPVPLeagueInfo = NULL;

			// 실제 정보 얻기
			pclPVPLeagueInfo = &pclInfo->m_clLatest[siIndex];
			if ( NULL == pclPVPLeagueInfo )
			{
				continue;
			}

			NTCHARString128 strScore;
			strScore.FormatString( TEXT("%d/%d/%d"), pclPVPLeagueInfo->siWin, pclPVPLeagueInfo->siDraw, pclPVPLeagueInfo->siLose );

			TCHAR szResult[64] = { '\0', };
			GetResultString( pclPVPLeagueInfo->siResult, szResult, sizeof(szResult) );

			stItemData.Set( 0, pclPVPLeagueInfo->szName );
			stItemData.Set( 1, SI32ToString(pclPVPLeagueInfo->siLevel) );
			stItemData.Set( 2, SI32ToString( GetGrade(pclPVPLeagueInfo->siGrade) ) );
			stItemData.Set( 3, strScore );
			stItemData.Set( 4, szResult );

			pclLatestList->AddItem( &stItemData );	
		}

		pclLatestList->Refresh();
	}

	//------------------------------------------------
	// 리그 상위 10명
	//------------------------------------------------
	CList* pclTopList = m_InterfaceFactory.GetList( PVPLEAGUE_DLG_LISTBOX_TOP );
	if ( pclTopList )
	{
		for ( SI32 siIndex=0; siIndex<Const_PVP_League_Top_Max; ++siIndex ) 
		{
			if ( 0 >= pclInfo->m_clTop[siIndex].siLevel )
			{
				continue;
			}

			// 초기화
			stItemData.Init();
			pclPVPLeagueInfo = NULL;

			// 실제 정보 얻기
			pclPVPLeagueInfo = &pclInfo->m_clTop[siIndex];
			if ( NULL == pclPVPLeagueInfo )
			{
				continue;
			}

			NTCHARString128 strScore;
			strScore.FormatString( TEXT("%d/%d/%d"), pclPVPLeagueInfo->siWin, pclPVPLeagueInfo->siDraw, pclPVPLeagueInfo->siLose );

			SI32 siTopPOV = GetPercentageOfVictories( pclPVPLeagueInfo->siWin, pclPVPLeagueInfo->siLose, pclPVPLeagueInfo->siDraw );

			stItemData.Set( 0, pclPVPLeagueInfo->szName );
			stItemData.Set( 1, SI32ToString(pclPVPLeagueInfo->siLevel) );
			stItemData.Set( 2, SI32ToString( GetGrade(pclPVPLeagueInfo->siGrade) ) );
			stItemData.Set( 3, strScore );
			stItemData.Set( 4, SI32ToString(siTopPOV) );

			pclTopList->AddItem( &stItemData );
		}

		pclTopList->Refresh();
	}

}

void CNPVPLeageuDlg::DefaultSetting( void )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	SI32 siTextPrintOption = DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	// 캐릭터 초상
	CStatic* pPortraitStatic = m_InterfaceFactory.GetStatic( PVPLEAGUE_DLG_STATICTEXT_PORTRAIT );
	if ( pPortraitStatic )
	{
		TCHAR	szDummy[256]	= "";
		SI32	siPortraitFont	= 0;

		pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclchar->GetKind(), szDummy, sizeof(szDummy), &siPortraitFont);
		pPortraitStatic->SetFontIndex( siPortraitFont );
	}

	// 이름
	CStatic* pNameStatic = m_InterfaceFactory.GetStatic( PVPLEAGUE_DLG_STATICTEXT_NAME );
	if ( pNameStatic )
	{
		pNameStatic->SetTextPrintOption( siTextPrintOption );
		pNameStatic->SetText( (TCHAR*)pclchar->GetName() );
	}

	// 레벨
	CStatic* pLevelStatic = m_InterfaceFactory.GetStatic( PVPLEAGUE_DLG_STATICTEXT_LEVEL );
	if ( pLevelStatic )
	{
		pLevelStatic->SetTextPrintOption( siTextPrintOption );
		pLevelStatic->SetText( SI32ToString(pclchar->pclCI->clIP.GetLevel()) );
	}

	// 전적
	CStatic* pScoreStatic = m_InterfaceFactory.GetStatic( PVPLEAGUE_DLG_STATICTEXT_SCORE );
	if ( pScoreStatic )
	{
		pScoreStatic->SetTextPrintOption( DT_CENTER | DT_VCENTER );
	}

	// 등급
	CStatic* pGradeStatic = m_InterfaceFactory.GetStatic( PVPLEAGUE_DLG_STATICTEXT_GRADE );
	if ( pGradeStatic )
	{
		pGradeStatic->SetTextPrintOption( siTextPrintOption );
	}
}

void CNPVPLeageuDlg::OnButtonClicked_Apply()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	cltCharClient* pclchar = pclclient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 수감중에는 안된다
	if ( true == pclchar->clInnerStatus.IsStatus(INNERSTATUS_PRISON) )
	{
		NTCHARString64	strTitle		= GetTxtFromMgr(2455);
		NTCHARString256 strText			= GetTxtFromMgr(2455);

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		return;
	}

	// 무한대전 시간에만 작동한다
	if ( false == pclClient->pclEventTimeManager->InEventTime( TEXT("PVP_LeagueReserve"), &pclclient->sTime ) )
	{
		// 무한대전 예약시간은 지났지만 아직 경기 시간이라면
		if ( pclClient->pclEventTimeManager->InEventTime( TEXT("PVP_LeagueMatch"), &pclclient->sTime ) )
		{
			NTCHARString64	strTitle		= GetTxtFromMgr(9581);
			NTCHARString256 strText			= GetTxtFromMgr(9720);

			pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
			return;
		}

		// 무한대전 진행시간을 보여준다
		cltEventTime* pclPVPLeagueReserve = pclClient->pclEventTimeManager->GetEvent("PVP_LeagueReserve");
		if ( NULL == pclPVPLeagueReserve )
		{
			return;
		}

		cltEventTime* pclPVPLeagueMatch = pclClient->pclEventTimeManager->GetEvent("PVP_LeagueMatch");
		if ( NULL == pclPVPLeagueMatch )
		{
			return;
		}

		SI32 siStartHour	= pclPVPLeagueReserve->m_StartTime.wHour;
		SI32 siStartMinute	= pclPVPLeagueReserve->m_StartTime.wMinute;
		SI32 siEndHour		= pclPVPLeagueMatch->m_EndTime.wHour;
		SI32 siEndMinute	= pclPVPLeagueMatch->m_EndTime.wMinute;


		NTCHARString64	strTitle		= GetTxtFromMgr(9581);
		NTCHARString256 strText			= GetTxtFromMgr(9597);
		TCHAR			szStartTime[64]	= { '\0', };
		TCHAR			szEndTime[64]	= { '\0', };

		StringCchPrintf( szStartTime, sizeof(szStartTime), TEXT("%02d:%02d"), siStartHour, siStartMinute );
		StringCchPrintf( szEndTime, sizeof(szEndTime), TEXT("%02d:%02d"), siEndHour, siEndMinute );

		strText.Replace( TEXT("#starttime#"), szStartTime );
		strText.Replace( TEXT("#endtime#"), szEndTime );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );

		return;
	}

	// 무한대전 참가 신청가능 레벨 검사
	SI32 siLimitLevel = pclClient->GetGlobalValue("GV_PVPLeague_LimitLevel");
	if ( pclchar->pclCI->clIP.GetLevel() < siLimitLevel )
	{
		NTCHARString64	strTitle	= GetTxtFromMgr(9581);
		NTCHARString256 strText		= GetTxtFromMgr(9672);

		strText.Replace( TEXT("#level#"), SI32ToString(siLimitLevel) );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );

		return;
	}

	ShowRetMsgBox( 9581, 9596, PVPLEAGUE_RETMSG_APPLY );
}

void CNPVPLeageuDlg::OnButtonClicked_Cancel()
{
	cltClient* pclclient  = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	// 무한대전 시간에만 작동한다
	if ( (false == pclClient->pclEventTimeManager->InEventTime( TEXT("PVP_LeagueReserve"), &pclclient->sTime ))
		&& (false == pclClient->pclEventTimeManager->InEventTime( TEXT("PVP_LeagueMatch"), &pclclient->sTime ))
		)
	{
		return;
	}

	ShowRetMsgBox( 9581, 9598, PVPLEAGUE_RETMSG_CANCEL );
}

void CNPVPLeageuDlg::ShowRetMsgBox( SI32 siTitleNum, SI32 siTextNum, SI16 siIndex )
{
	cltClient* pclclient  = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	NTCHARString64	strTitle	= GetTxtFromMgr(siTitleNum);
	NTCHARString256	strText		= GetTxtFromMgr(siTextNum);

	stMsgBox MsgBox;
	MsgBox.Set( this, strTitle, strText, MSGBOX_TYPE_YESNO, siIndex );
	pclclient->SetMsgBox( &MsgBox, NULL, 0 );

}

void CNPVPLeageuDlg::RequestPVPLeagueUIInfo( void )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	cltMsg clMsg( GAMEMSG_REQUEST_PVP_LEAGUE_UI_INFOGET, 0, NULL );
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
}

SI32 CNPVPLeageuDlg::GetPercentageOfVictories( SI32 siWin, SI32 siLose, SI32 siDraw )
{
	if ( 0 >= siWin )
	{
		return 0;
	}

	SI32 siPOV = siWin + siLose + siDraw;
	if ( 0 >= siPOV )
	{
		return 0;
	}

	siPOV = siWin * 100 / siPOV;
	if ( 100 < siPOV )
	{
		siPOV = 100;
	}

	return siPOV;

}

bool CNPVPLeageuDlg::GetResultString( IN SI08 siResult, OUT TCHAR* pszResultString, IN SI32 siSize )
{
	if ( (PVPLEAGUE_RESULT_NONE >= siResult) || (NULL == pszResultString) )
	{
		return false;
	}

	switch	( siResult ) 
	{
	case PVPLEAGUE_RESULT_WIN:
		{
			TCHAR* pszText = GetTxtFromMgr( 6487 );
			StringCchCopy( pszResultString, siSize, pszText );
		}
		break;

	case PVPLEAGUE_RESULT_LOSE:
		{
			TCHAR* pszText = GetTxtFromMgr( 6488 );
			StringCchCopy( pszResultString, siSize, pszText );
		}
		break;

	case PVPLEAGUE_RESULT_DRAW:
		{
			TCHAR* pszText = GetTxtFromMgr( 9603 );
			StringCchCopy( pszResultString, siSize, pszText );
		}
		break;

	default:
		{
			return false;
		}
		break;

	}

	return true;
}

SI32 CNPVPLeageuDlg::GetGrade( SI32 siGradeValue )
{
	SI32 siGradeTable[] = {
		PVP_LEAGUE_GRADE_NONE,
			PVP_LEAGUE_GRADE_1,
			PVP_LEAGUE_GRADE_2,
			PVP_LEAGUE_GRADE_3,
			PVP_LEAGUE_GRADE_4,
			PVP_LEAGUE_GRADE_5,
			PVP_LEAGUE_GRADE_6,
			PVP_LEAGUE_GRADE_7,
			PVP_LEAGUE_GRADE_8,
			PVP_LEAGUE_GRADE_9
	};
	SI32 siGradeCount = sizeof(siGradeTable) / sizeof(siGradeTable[0]);


	for ( SI32 siIndex=0; siIndex<siGradeCount; ++siIndex )
	{
		if ( siGradeTable[siIndex] == siGradeValue )
		{
			return siIndex;
		}
	}

	return 0;
}