#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "../common/Map/map.h"

#include "NServerEventQuestDlg.h"




CNServerEventQuestDlg::CNServerEventQuestDlg() : CDialogController(), 
	m_siCharacterNum(0), 
	m_siNeedAmount(0), 
	m_siDrawCharIndex(0),
	m_siDialogMode(0), 
	m_siDialogWidht(0), 
	m_siDialogHeight(0), 
	m_siDialogX(0), 
	m_siDialogY(0),
	m_siDrawState(SEQD_DRAWSTATE_ALL)
{
	GP.LoadSprFromFile( TEXT("NInterface/image/SERVER_EVENT_CHAR_FX.spr"), &m_sprBack );
	
	ZeroMemory( m_bCharacter,				sizeof(m_bCharacter) );
	ZeroMemory( m_pStatic,					sizeof(m_pStatic) );
	ZeroMemory( m_siTxtMgrNumForCharacter,	sizeof(m_siTxtMgrNumForCharacter) );

	// 텍스트 메니져 번호가 중간에 변경될 수 있으므로 초기에 원하는 번호로 셋팅한다
	for ( SI32 siIndex=0; siIndex<10; siIndex++ )
	{
		m_siTxtMgrNumForCharacter[ siIndex ] = 8503 + siIndex;
	}

}


CNServerEventQuestDlg::~CNServerEventQuestDlg()
{
	for ( SI32 siIndex=0; siIndex<Const_ServerEvent_Max_QuestCharacter; siIndex++ )
	{
		SAFE_DELETE( m_pStatic[ siIndex ] );
	}

	if ( m_sprBack.pImage )
	{
		GP.FreeSpr( m_sprBack );
	}
}


void CNServerEventQuestDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NServerEvent/NServerEventQuest.ddf" ) );

	file.CreatePopupDialog( this, SERVER_EVENT_QUEST_DLG, TEXT( "dialog_servereventquest" ), StaticServerEventQuestDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_QUEST_TIME_STATIC,			this ), SERVER_EVENT_QUEST_TIME_STATIC,			TEXT( "statictext_time" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_QUEST_EVENTTYPE_STATIC,	this ), SERVER_EVENT_QUEST_EVENTTYPE_STATIC,	TEXT( "statictext_eventtype" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_QUEST_AMOUNT_STATIC,		this ), SERVER_EVENT_QUEST_AMOUNT_STATIC,		TEXT( "statictext_amount" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	SERVER_EVENT_QUEST_IMAGESTATIC,			this ), SERVER_EVENT_QUEST_IMAGESTATIC,			TEXT( "imagestatic_notice" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	SERVER_EVENT_QUEST_MIN_BUTTON,			this ), SERVER_EVENT_QUEST_MIN_BUTTON,			TEXT( "button_min" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	SERVER_EVENT_QUEST_MAX_BUTTON,			this ), SERVER_EVENT_QUEST_MAX_BUTTON,			TEXT( "button_max" ) );


	//-- 에디트 컨트롤 설정
	CStatic* pStatic3 = (CStatic*)m_InterfaceFactory.GetStatic( SERVER_EVENT_QUEST_AMOUNT_STATIC );
	if ( pStatic3 != NULL )
	{
		pStatic3->SetTextPrintOption( DT_RIGHT );
	}

	TCHAR		sFileName[]		= TEXT("NInterface/image/IMAGE_44x44_00_001.SPR");
	SI32		siRandIndex		= -1;
	SI16		siNowX			= 0;
	SI16		siNowY			= SEQD_VARIABLE_CHAR_START_Y;

	SI16		siNowWidth		= SEQD_VARIABLE_CHAR_WIDTH;
	SI16		siNowHeight		= SEQD_VARIABLE_CHAR_HEIGHT;


	for ( SI32 siIndex=0; siIndex<Const_ServerEvent_Max_QuestCharacter; siIndex++ )
	{
		NEWCSTATIC( m_pStatic[siIndex] );

		CStatic* pCharater = m_pStatic[ siIndex ];
		if ( NULL != pCharater )
		{
			siNowX		= SEQD_VARIABLE_CHAR_START_X + (SEQD_VARIABLE_CHAR_WIDTH * siIndex);
			siRandIndex = (siRandIndex++) % 4;

			pCharater->Create( 0, NULL, sFileName, siRandIndex, 0, 0, siNowX, siNowY, siNowWidth, siNowHeight, siNowWidth, siNowHeight );
			pCharater->SetTextPrintOption( DT_CENTER | DT_VCENTER | DT_SINGLELINE );
			pCharater->SetFontSize( 35 );
			pCharater->Show( false );
			//pCharater->SetText( TEXT("T") );//Test
		}
	}

	//m_siDialogX = (pclclient->GetScreenXSize() - GetHeight() + MAP_TILE_XSIZE) / 2;
	//m_siDialogY = 150;

	SetDialogMode( SEQD_VARIABLE_DIALOGMODE_MINIMUN );

	Hide();
}

void CALLBACK CNServerEventQuestDlg::StaticServerEventQuestDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNServerEventQuestDlg *pThis = (CNServerEventQuestDlg*) pControllerMgr;
	pThis->NServerEventQuestDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNServerEventQuestDlg::NServerEventQuestDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch(nControlID)
	{
	case SERVER_EVENT_QUEST_MIN_BUTTON:
		{
			switch(nEvent)
			{				

			case EVENT_BUTTON_CLICKED:
				{
					SetDialogMode( SEQD_VARIABLE_DIALOGMODE_MINIMUN );
				}
				break;
			}
		}
		break;

	case SERVER_EVENT_QUEST_MAX_BUTTON:
		{
			switch(nEvent)
			{				

			case EVENT_BUTTON_CLICKED:
				{
					SetDialogMode( SEQD_VARIABLE_DIALOGMODE_MAXIMUN );
				}
				break;
			}
		}
		break;

	}

	return;

}

void CNServerEventQuestDlg::Show()
{
	CDialogController::Show( true );
}

void CNServerEventQuestDlg::Hide()
{
	CDialogController::Show( false );
}


void CNServerEventQuestDlg::DrawDialogFirst( SI32 ScreenRX, SI32 ScreenRY )
{
}

void CNServerEventQuestDlg::DrawDialogLast(SI32 ScreenRX, SI32 ScreenRY )
{
	if ( (0 == m_siCharacterNum) || (SEQD_VARIABLE_DIALOGMODE_MAXIMUN != m_siDialogMode) || (SEQD_DRAWSTATE_WAIT == m_siDrawState) )
	{
		return;
	}

	static	SI32	siCount		= 0;
			SI32	siMaxFrame	= 20;

	
	if ( (GetTickCount()%5) == 0 )
	{
		siCount = (siCount+1) % siMaxFrame;
	}

	SI32 siIndexStart	= 0;
	SI32 siIndexEnd		= 0;

	switch ( m_siDrawState )
	{
		case SEQD_DRAWSTATE_ALL:		{ siIndexStart=0;					siIndexEnd=m_siCharacterNum; }	break;
		case SEQD_DRAWSTATE_CHARACTER:	{ siIndexStart=m_siDrawCharIndex;	siIndexEnd=m_siDrawCharIndex; }	break;
	}

	bool bMaxFrame = ( (siCount+1) >= siMaxFrame );
	SI16 siDrawX;
	SI16 siDrawY;


	for ( SI32 siIndex=siIndexStart; siIndex<m_siCharacterNum; siIndex++ )
	{
		if ( false == m_bCharacter[siIndex] )
		{
			continue;
		}

		// 현재 효과 그리기
		siDrawX = SEQD_VARIABLE_CHAR_START_X + ScreenRX + GetX() + (SEQD_VARIABLE_CHAR_WIDTH*siIndex) - 13;
		siDrawY = ScreenRY + GetY() + 12;

		GP.PutSprScreen( &m_sprBack, siDrawX, siDrawY, siCount );
		
		// 문자 표시하기
		if ( bMaxFrame )
		{
			NTCHARString16 strChar( GetTxtFromMgr(m_siTxtMgrNumForCharacter[ siIndex ]) );
			m_pStatic[ siIndex ]->SetText( strChar );
		}
	}

	if ( bMaxFrame )
	{
		m_siDrawState	= SEQD_DRAWSTATE_WAIT;
		siCount			= 0;
	}

}

void CNServerEventQuestDlg::CalcWidth( void )
{
	if ( 0 == m_siCharacterNum )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}

	SI16 siDialogWidth = 6 + 12 + (SEQD_VARIABLE_CHAR_WIDTH*m_siCharacterNum) + 12 + 6;
	SI16 siNoticeWidth =     12 + (SEQD_VARIABLE_CHAR_WIDTH*m_siCharacterNum) + 12;
	SI16 siStaticWidth =          (SEQD_VARIABLE_CHAR_WIDTH*m_siCharacterNum);


	// 다이얼로그 길이
	SetDialogPos( GetX(), GetY(), siDialogWidth, 168 );

	// 알림 배경 길이
	CStatic* pImage = m_InterfaceFactory.GetStatic( SERVER_EVENT_QUEST_IMAGESTATIC );
	if ( NULL != pImage )
	{
		pImage->SetWidth( siNoticeWidth );
		pImage->SetSurfaceWidth( siNoticeWidth );
	}

	// 알림 텍스트 길이
	CStatic* pStatic = NULL;

	for ( SI32 siIndex=0; siIndex<3; siIndex++ )
	{
		pStatic = m_InterfaceFactory.GetStatic( SERVER_EVENT_QUEST_TIME_STATIC + siIndex );
		if ( NULL != pStatic )
		{
			pStatic->SetWidth( siStaticWidth );
			pStatic->SetSurfaceWidth( siStaticWidth );
		}
	}

	// 최소화 버튼 위치
	CButton* pButton = m_InterfaceFactory.GetButton( SERVER_EVENT_QUEST_MIN_BUTTON );
	if ( NULL != pButton )
	{
		pButton->SetX( siDialogWidth - 20 );
	}

	// 문자 배경 보여지는 여부
	for ( SI32 siCount=0; siCount<Const_ServerEvent_Max_QuestCharacter; siCount++ )
	{
		pStatic = m_pStatic[ siCount ];
		if ( NULL != pStatic )
		{
			if ( siCount < m_siCharacterNum )
			{
				pStatic->Show( true );
			}
			else
			{
				pStatic->Show( false );
			}
		}
	}

	m_siDialogHeight	= GetHeight();
	m_siDialogWidht		= GetWidth();
	m_siDialogX			= (pclclient->GetScreenXSize() - GetWidth() + MAP_TILE_XSIZE) / 2;
	m_siDialogY			= 150;
}

void CNServerEventQuestDlg::SetServerEventQuestStart( CNServerEvent_QuestData* pclSE_QuestData, SI16 siCharLimit, SI32 siNowAmount, bool* pbChar )
{
	if ( (NULL == pclSE_QuestData) || (NULL == pbChar) || (0 > siCharLimit) || (Const_ServerEvent_Max_QuestCharacter < siCharLimit) )
	{
		return;
	}

	SI32 siEventType		= pclSE_QuestData->GetEventType();
	SI32 siCondition1		= pclSE_QuestData->GetCondition1();
	SI32 siCondition2		= pclSE_QuestData->GetCondition2();
	SI32 siLimitMinute		= pclSE_QuestData->GetLimitMinute();
	SI32 siNowClearAmount	= siNowAmount;

	m_siNeedAmount			= pclSE_QuestData->GetNeedClearAmount();

	// 퀘스트 타입 쓰기
	WriteQuestType( siEventType, siCondition1, siCondition2 );
	// 퀘스트 실행 정보 쓰기
	SetServerEventQuestRunInfo( siLimitMinute, siNowClearAmount );

	// 문자의 제한 갯수가 달라야만 설정한다(다이얼로그창 넓이도 변경해야 하기 때문)
	if ( m_siCharacterNum != siCharLimit )
	{
		m_siCharacterNum = siCharLimit;
		// 문자 갯수가 바뀌었으므로 다이얼로그 넓이를 다시 계산해 준다
		CalcWidth();
	}

	memcpy( m_bCharacter, pbChar, sizeof(m_bCharacter) );

	// 현재 초기화 한다
	for ( SI32 siIndex=0; siIndex<m_siCharacterNum; siIndex++ )
	{
		CStatic* pStatic = m_pStatic[ siIndex ];

		if ( false == m_bCharacter[ siIndex ] )
		{
			pStatic->SetText( TEXT("") );
		}
	}

	if ( m_siDialogMode != SEQD_VARIABLE_DIALOGMODE_MAXIMUN )
	{
		SetDialogMode( SEQD_VARIABLE_DIALOGMODE_MAXIMUN );
	}
	
	Show();

	//m_siDrawState = SEQD_DRAWSTATE_ALL;
}


void CNServerEventQuestDlg::SetServerEventQuestRunInfo( SI32 siLimiteMinute, SI32 siNowAmount )
{
	// 퀘스트 제한 시간
	NTCHARString128 strLimitTime( GetTxtFromMgr(8536) );
	strLimitTime.Replace( TEXT("#minute#"), SI32ToString(siLimiteMinute) );

	CStatic* pLimitTime = m_InterfaceFactory.GetStatic( SERVER_EVENT_QUEST_TIME_STATIC );
	if ( NULL != pLimitTime )
	{
		pLimitTime->SetText( strLimitTime );
	}

	// 필요한 갯수
	NTCHARString128 strAmount( TEXT("#nowamount# / #needamount#") );
	strAmount.Replace( TEXT("#nowamount#"),		SI32ToString(siNowAmount) );
	strAmount.Replace( TEXT("#needamount#"),	SI32ToString(m_siNeedAmount) );

	CStatic* pAmount = m_InterfaceFactory.GetStatic( SERVER_EVENT_QUEST_AMOUNT_STATIC );
	if ( NULL != pAmount )
	{
		pAmount->SetText( strAmount );
	}

}

void CNServerEventQuestDlg::SetServerEventQuestGetChar( SI16 siCharIndex )
{
	if ( (0 > siCharIndex) || (Const_ServerEvent_Max_QuestCharacter <= siCharIndex) )
	{
		return;
	}

	m_bCharacter[ siCharIndex ]	= true;
	m_siDrawCharIndex			= siCharIndex;
	m_siDrawState				= SEQD_DRAWSTATE_CHARACTER;
}

void CNServerEventQuestDlg::WriteQuestType( SI32 siEventType, SI32 siCondition1, SI32 siCondition2 )
{
	if ( (SERVEREVENT_TYPE_NONE >= siEventType) || ( SERVEREVENT_TYPE_END <= siEventType) )
	{
		return;
	}

	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL ==pclclient )
	{
		return;
	}

	NTCHARString256 strType;

	switch ( siEventType )
	{
	case SERVEREVENT_TYPE_HUNTLEVEL:
		{
			NTCHARString256 strText( GetTxtFromMgr( 8490 ) );

			strText.Replace( TEXT("#lowlevel#"),	SI32ToString( siCondition1 ) );
			strText.Replace( TEXT("#hilevel#"),		SI32ToString( siCondition2 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_HUNTKIND:
	case SERVEREVENT_TYPE_HUNTBOSS:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8491 ) );
			const TCHAR*		pszGetName = pclClient->pclKindInfoSet->GetName( siCondition1 );

			strText.Replace( TEXT("#monstername#"), pszGetName );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_QUESTCLEAR:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8537 ) );
			NTCHARString32		strGetName;


			if ( 0 == siCondition1 )
			{
				strGetName = GetTxtFromMgr( 3086 ); // 요술램프
			}
			else
			{
				SI32			siNPCKind	= pclClient->pclQuestManager->GetNPCKindFromQuestType(siCondition1);
				const TCHAR*	pszGetName	= pclClient->pclKindInfoSet->GetName(siNPCKind);

				if ( NULL == pszGetName )
				{
					return;
				}

				strGetName = pszGetName;
			}


			strText.Replace( TEXT("#questname#"), strGetName );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_USEITEM:
		{
			NTCHARString256	strText( GetTxtFromMgr( 8538 ) );
			const TCHAR*	pszGetName = pclClient->pclItemManager->GetName( siCondition2 );

			strText.Replace( TEXT("#itemname#"), pszGetName );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_MAKELEVELITEM:
		{
			NTCHARString256	strText( GetTxtFromMgr( 8539 ) );

			strText.Replace( TEXT("#lowlevel#"),	SI32ToString( siCondition1 ) );
			strText.Replace( TEXT("#hilevel#"),		SI32ToString( siCondition2 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_MAKERAREITEM:
		{
			NTCHARString256	strText( GetTxtFromMgr( 8540 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_INSTANCEMAPCLEAR:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8495 ) );
			NTCHARString32		strIndunName;

			switch ( siCondition1 )
			{
				case INSTANCEMAP_TYPE_GUILD:	{ strIndunName = GetTxtFromMgr(7825); } break;
				case INSTANCEMAP_TYPE_PARTY:	{ strIndunName = GetTxtFromMgr(7981); } break;
				//case INSTANCEMAP_TYPE_VILLAGE:	{ strIndunName = GetTxtFromMgr(); } break;
			}

			strText.Replace( TEXT("#dungeon#"), strIndunName );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_FISH:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8492 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_FARM:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8493 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_MINE:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8494 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_SKILLUP:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8496 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_BLACKWARCLEAR:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8501 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_RESIDENTVILLAGE:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8498 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_OPEN_GVG:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8523 ) );

			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_COUNTUSER_INMAP:
		{
			NTCHARString256		strText( GetTxtFromMgr( 8556 ) );

			cltClient* pclclient = (cltClient*)pclClient;
			if ( NULL == pclclient )
			{
				return;
			}

			TCHAR* szMapName = pclclient->pclMapManager->GetMapName( siCondition1 );
			if ( NULL == szMapName )
			{
				return;
			}

			strText.Replace( TEXT("#mapname#"), szMapName );
			strType = strText;
		}
		break;

	case SERVEREVENT_TYPE_LEVELUP:
		{
			NTCHARString256		strText( GetTxtFromMgr(8497) );

			strText.Replace( TEXT("#level#"), SI32ToString(siCondition1) );

			strType = strText;
		}
		break;
	}

	CStatic* pStatic = m_InterfaceFactory.GetStatic( SERVER_EVENT_QUEST_EVENTTYPE_STATIC );
	if ( NULL != pStatic )
	{
		pStatic->SetText( strType );
	}
}

void CNServerEventQuestDlg::SetDialogMode( SI32 siDialogMode )
{
	SI16 siWidth	= 0;
	SI16 siHeight	= 0;
	SI16 siX		= 0;
	SI16 siY		= 0;


	if ( SEQD_VARIABLE_DIALOGMODE_MINIMUN == siDialogMode )
	{
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_TIME_STATIC)->Show( false );
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_EVENTTYPE_STATIC)->Show( false );
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_AMOUNT_STATIC)->Show( false );
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_IMAGESTATIC)->Show( false );
		m_InterfaceFactory.GetButton(SERVER_EVENT_QUEST_MIN_BUTTON)->Show( false );
		
		m_InterfaceFactory.GetButton(SERVER_EVENT_QUEST_MAX_BUTTON)->Show( true );

		for ( SI32 siIndex=0; siIndex<m_siCharacterNum; siIndex++ )
		{
			CStatic* pStatic = m_pStatic[ siIndex ];

			if ( pStatic )
			{
				pStatic->Show( false );
			}
		}

		siWidth		= 38;
		siHeight	= 38;
		siX			= 465; 
		siY			= 645;

	}
	else if ( SEQD_VARIABLE_DIALOGMODE_MAXIMUN == siDialogMode )
	{
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_TIME_STATIC)->Show( true );
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_EVENTTYPE_STATIC)->Show( true );
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_AMOUNT_STATIC)->Show( true );
		m_InterfaceFactory.GetStatic(SERVER_EVENT_QUEST_IMAGESTATIC)->Show( true );
		m_InterfaceFactory.GetButton(SERVER_EVENT_QUEST_MIN_BUTTON)->Show( true );

		m_InterfaceFactory.GetButton(SERVER_EVENT_QUEST_MAX_BUTTON)->Show( false );

		for ( SI32 siIndex=0; siIndex<m_siCharacterNum; siIndex++ )
		{
			CStatic* pStatic = m_pStatic[ siIndex ];

			if ( pStatic )
			{
				pStatic->Show( true );
			}
		}

		siHeight	= m_siDialogHeight;
		siWidth		= m_siDialogWidht;
		siX			= m_siDialogX;
		siY			= m_siDialogY;
			
	}
	else
	{
		return;
	}

	m_siDialogMode = siDialogMode;

	SetDialogPos( siX, siY, siWidth, siHeight );
}

TCHAR* CNServerEventQuestDlg::GetIndexToText( SI32 siIndex )
{
	if ( (0 > siIndex) || (siIndex>=m_siCharacterNum) )
	{
		return NULL;
	}

	return GetTxtFromMgr(m_siTxtMgrNumForCharacter[ siIndex ]);
}

