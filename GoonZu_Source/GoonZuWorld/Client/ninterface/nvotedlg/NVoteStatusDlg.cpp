#include "NVoteStatusDlg.h"

#include "../../Client/client.h"

//#include "../../InterfaceMgr/interface/ImageStatic.h"
#include "../../InterfaceMgr/Interface/Button.h"
#include "../../InterfaceMgr/Interface/Static.h"
#include "../../InterfaceMgr/Interface/Edit.h"



#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../InterfaceMgr/InterfaceImgMgr/InterfaceImgMgr.h"

#include "../../InterfaceMgr/InterfaceFile.h"



extern cltCommonLogic* pclClient;

CNVoteStatusDlg::CNVoteStatusDlg() : CDialogController()
{
	//m_pcisBack			= NULL;
	m_pcsVote			= NULL;
	m_pcsPercentage		= NULL;
	m_pcbMinimum		= NULL;
	m_pcbMaximum		= NULL;
	m_pcsDisplay		= NULL;

	Init();

	//-- 타이버 시간 설정

	// 후보자 이름이 보여질 주기 1초
	m_kUpdateTimer_WriteCandidate.Init( 1 * 1000 );

	// 후보자 그림을 그릴 주기 1/10초
	m_kUpdateTimer_DrawCandidate.Init( 100 );

	GP.LoadSprFromFile( TEXT("NInterface/image/IMAGE_30x40_00_000.SPR"), &m_sprCandidate );
}

CNVoteStatusDlg::~CNVoteStatusDlg()
{
	//SAFE_DELETE( m_pcisBack );
	SAFE_DELETE( m_pcsVote );
	SAFE_DELETE( m_pcsPercentage );
	SAFE_DELETE( m_pcbMinimum );
	SAFE_DELETE( m_pcbMaximum );
	SAFE_DELETE( m_pcsDisplay );

	if ( m_sprCandidate.pImage )
	{
		GP.FreeSpr( m_sprCandidate );
	}

}

void CNVoteStatusDlg::Create( )
{
	if (IsCreate() == true )
	{
		return;
	}

	CInterfaceFile file;

	//-- 기본설정
	file.LoadFile(TEXT("NInterface/Data/NVoteKing/DLG_NVoteStatus.ddf"));
	file.CreatePopupDialog( this, NVOTESTATUS_DLG, TEXT("dialog_votestatus"), StaticVoteStatusDlgProc);

	//-- 객체생성
	//m_pcisBack			= new CImageStatic( this );
	m_pcsVote			= new CStatic( this );
	m_pcsPercentage		= new CStatic( this );
	m_pcbMinimum		= new CButton( this );
	m_pcbMaximum		= new CButton( this );
	m_pcsDisplay		= new CStatic( this );

	//-- 메세지멥 연결
	file.CreateControl( m_pcbMinimum,		NVOTESTATUS_MINIMUMBUTTON,	TEXT("button_minimum") );
	file.CreateControl( m_pcbMaximum,		NVOTESTATUS_MAXIMUMBUTTON,	TEXT("button_maximum") );

	file.CreateControl( m_pcsVote,			NVOTESTATUS_VOTE,			TEXT("statictext_vote") );
	file.CreateControl( m_pcsPercentage,	NVOTESTATUS_PERCENTAGE,		TEXT("statictext_percentage") );
	
	file.CreateControl( m_pcsDisplay,		NVOTESTATUS_DISPLAY,		TEXT("statictext_display") );

	//file.CreateControl( m_pcisBack,			NVOTESTATUS_BACK,			TEXT("imagestatic_back") );

	//-- 텍스트 설정
	m_pcsDisplay->SetFontWeight( 600 );
	m_pcsDisplay->SetFontColor( RGB( 255, 250, 250 ) );
	//m_pcsDisplay->SetHeight( 0 );
	//m_pcsDisplay->SetWidth( 0 );
	
	m_pcsVote->SetFontWeight( 600 );
	m_pcsVote->SetFontColor( RGB( 255, 250, 250 ) );

	m_pcsPercentage->SetFontWeight( 600 );
	m_pcsPercentage->SetFontColor( RGB( 255, 250, 250 ) );

	//-- 기본 시작 위치 / 모드로 변경
	SetX( VOTESTATUS_BASE_DIALOG_X );
	SetY( VOTESTATUS_BASE_DIALOG_Y );
	SaveNowDialog();
	WindowModeChange( VOTESTATUS_MODE_MAXIMUM );
		
	SetActionTimer( 200 );
	SetTimerOnOff( true );


	return;
}

void CNVoteStatusDlg::Init()
{
	ZeroMemory( m_tszCandidateName,	(sizeof( TCHAR ) * MAX_CANDIDATURE_NUM * MAX_PLAYER_NAME) );
	ZeroMemory( m_siVote,			(sizeof( SI32 ) * MAX_CANDIDATURE_NUM) );
	ZeroMemory( m_siDrawVote,		(sizeof( SI32 ) * MAX_CANDIDATURE_NUM) );

	for ( SI08 siCount=0; siCount<MAX_CANDIDATURE_NUM; siCount++ )
	{
		m_siDrawOrder[ siCount ] = siCount;
	}

	memset( m_siDrawMode, VOTESTATUS_DRAWMODE_RUN, (sizeof(SI08) * MAX_CANDIDATURE_NUM) );

	m_siTotalVote		= 0;
	m_siNowTotalVote	= 0;

	m_siCandidateNum	= 0;
	m_siNowShowNum		= 0;
	m_siTopVoteNum		= 0;

	m_siNowFrame		= 0;

	m_bLock				= false;

	m_siVoteStatus		= VOTESTATUS_VOTE_READY;
}

void CALLBACK CNVoteStatusDlg::StaticVoteStatusDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	CNVoteStatusDlg *pThis = (CNVoteStatusDlg*) pControllerMgr;
	pThis->NVoteStatusDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNVoteStatusDlg::NVoteStatusDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl )
{
	switch(nControlID)
	{
	case NVOTESTATUS_MINIMUMBUTTON:
		{
			switch(nEvent)
			{				

			case EVENT_BUTTON_CLICKED:
				{
					WindowModeChange( VOTESTATUS_MODE_MINIMUM );
				}
				break;
			}
		}
		break;

	case NVOTESTATUS_MAXIMUMBUTTON:
		{
			switch(nEvent)
			{				

			case EVENT_BUTTON_CLICKED:
				{
					WindowModeChange( VOTESTATUS_MODE_MAXIMUM );
				}
				break;
			}

		}
		break;

	}

	return;
}

//--------------------------------------------------------------------------------------------- 
//	TCHAR** pptszCnadidateName	: 현재 후보자들의 이름
//	SI32 siTotalVote			: 총 투표수
//	SI08 siMaxCandidate			: 현재 선거의 후보자 수
//	SI32 siTopVoteNum			: 현재 후보자 중 최대 득표수
//--------------------------------------------------------------------------------------------- 
void CNVoteStatusDlg::SetStartVoteStatus( TCHAR** pptszCnadidateName, SI32 siTotalVote,  SI08 siCandidateNum, SI32 siTopVoteNum )
{
	

	if ( (pptszCnadidateName == NULL) || (siTotalVote <= 0) )
	{
		return;
	}

	if ( (siCandidateNum <= 0) || (siCandidateNum > MAX_CANDIDATURE_NUM) )
	{
		return;
	}
	//SaveNowDialog();
	Init();

	m_bLock = true;
	
	m_siCandidateNum	= siCandidateNum;
	m_siTopVoteNum		= siTopVoteNum;
	m_siTotalVote		= siTotalVote;

	memcpy( m_tszCandidateName, pptszCnadidateName, (sizeof(TCHAR) * MAX_CANDIDATURE_NUM * MAX_PLAYER_NAME) );

	// 모드를 최대화 모드로 변환
	WindowModeChange( VOTESTATUS_MODE_MAXIMUM );
	
	// 후보자 보여주기 타이머 시동
	m_kUpdateTimer_WriteCandidate.SetActive( true, GetTickCount() );
	// 후보자 그리기 타이머 시동
	m_kUpdateTimer_DrawCandidate.SetActive( true, GetTickCount() );

	m_siVoteStatus = VOTESTATUS_VOTE_RUN;

	m_bLock = false;

}

void CNVoteStatusDlg::SetVoteStatus( SI32 *psiNowVote )
{
	if ( psiNowVote == NULL )
	{
		return;
	}

	// 개표 현황을 받는데 중간에 로그인해서 처음 시작 정보를 받지 못했다면 서버에 시작 정보를 요청
	if ( VOTESTATUS_VOTE_READY == m_siVoteStatus )
	{
		cltMsg clMsg( GAMEMSG_REQUEST_STARTVOTESTATUS, 0, NULL );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

		return;
	}

	if ( m_siCandidateNum <= 0 )
	{
		return;
	}

	m_bLock = true;

	for ( SI32 siCount=0; siCount<m_siCandidateNum; siCount++ )
	{
		m_siVote[siCount] = psiNowVote[siCount];
	}

	//memcpy( m_siVote, psiNowVote, (sizeof(SI32)*MAX_CANDIDATURE_NUM) );

	// 그리는 순서 변경
	for ( SI32 siCount=0; siCount<m_siCandidateNum; siCount++ )
	{
		m_siDrawOrder[ siCount ] = (m_siDrawOrder[ siCount ] + 1) % m_siCandidateNum;
	}

	m_bLock = false;

}

void CNVoteStatusDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( true == m_bLock )
	{
		return;
	}

	if ( VOTESTATUS_MODE_MAXIMUM == m_bWindowMode )
	{
		DrawCandidate( ScreenRX, ScreenRY );
	}
	
}

void CNVoteStatusDlg::DrawCandidate( SI32 siRX, SI32 siRY )
{
	if ( (m_siCandidateNum <= 0) || (m_siCandidateNum > MAX_CANDIDATURE_NUM) )
	{
		return;
	}

	if ( m_siTopVoteNum <= 0 )
	{
		return;
	}

	if ( VOTESTATUS_MODE_MAXIMUM != m_bWindowMode )
	{
		return;
	}

	if ( m_kUpdateTimer_DrawCandidate.IsTimed( GetTickCount() ) == true )
	{
		m_siNowFrame = (++m_siNowFrame) % VOTESTATUS_MAX_FRAME;
	}

	// 실제 좌표에 표시될 위치 보정값
	SI32	siCorrectionX		= this->GetX() + siRX;
	SI32	siCorrectionY		= this->GetY() + siRY;

	SI32	siStartPositionX	= VOTESTATUS_CANDIDATE_START_X + siCorrectionX - (VOTESTATUS_CANDIDATE_WIDTH/2);
	SI32	siStartPositionY	= VOTESTATUS_CANDIDATE_START_Y + siCorrectionY - VOTESTATUS_CANDIDATE_HEIGHT + 6;
	SI32	siDrawX				= 0;
	REAL32	fDrawWidthRate		= 0.0f;

	SI32	siNowFrame			= 0;
	SI32	siNowFont			= 0;
	SI32	siCount				= 0;
	SI32	siRealIndex			= 0;


	for ( SI32 siIndex=0; siIndex<m_siCandidateNum; siIndex++ )
	{
		//-- 그리기 영역
		siRealIndex		= m_siDrawOrder[ siIndex ];
		siCount			= m_siDrawVote[siRealIndex];
	
		fDrawWidthRate	= siCount / ( m_siTopVoteNum * 1.0f );
		siDrawX			= (SI32)( VOTESTATUS_BASE_DIALOG_WIDTH * fDrawWidthRate );

		siNowFrame		= (m_siNowFrame + siCount) % VOTESTATUS_MAX_FRAME;
		// 한줄에 그림이 6개 있어서 *6을 함
		siNowFont		= (siRealIndex*6) + siNowFrame + m_siDrawMode[siRealIndex];

		GP.PutSpr( &m_sprCandidate, (siStartPositionX + siDrawX), siStartPositionY, siNowFont );
	
	}
}

void CNVoteStatusDlg::Action()
{
	if ( true == m_bLock )
	{
		return;
	}

	switch ( m_siVoteStatus )
	{
		case VOTESTATUS_VOTE_READY:
			{
				Show( false );
			}
			break;

		case VOTESTATUS_VOTE_RUN:
			{
				if ( IsShow() == false )
				{
					Show( true );
				}

				// 1초에 한번씩 보여지는 후보자가 변경된다
				if ( m_kUpdateTimer_WriteCandidate.IsTimed(GetTickCount()) )
				{
					if ( 0 >= m_siCandidateNum )
					{
						return;
					}

					m_siNowShowNum = (m_siNowShowNum + 1) % m_siCandidateNum;

					WriteCandidate( m_siNowShowNum );

				} // if ( m_kUpdateTimer_WriteCandidate.IsTimed(GetTickCount()) ) 종료

				VoteCount();

			}
			break;

		case VOTESTATUS_VOTE_END:
			{
				WriteCandidate( m_siNowShowNum );

				if ( m_kUpdateTimer_End.IsTimed(GetTickCount()) )
				{
					DeleteDialog();
				}
			}
			break;
	} // switch ( m_siVoteStatus ) 종료

}

// 후보자 표시되는 정보
void CNVoteStatusDlg::WriteCandidate( SI08 siShowCandidateNumber )
{
	if ( (siShowCandidateNumber < 0) || (siShowCandidateNumber >= m_siCandidateNum) )
	{
		return;
	}
	
	NTCHARString128	kCandidateVote( GetTxtFromMgr(8106) );
	NTCHARString128 kPercentage( GetTxtFromMgr(8107) );

	REAL32			fPercentage		= 0.0f;
	SI32			siCandidateVote = m_siVote[siShowCandidateNumber];

	//---------------------------------------------------------------------------------------------------
	//	개인 득표 수 표시 
	//---------------------------------------------------------------------------------------------------
	
	// 후보자 득표율	
	if ( m_siNowTotalVote <= 0 )
	{
		fPercentage = 0.0f;
	}
	else
	{
		fPercentage	= siCandidateVote / (REAL32)(m_siNowTotalVote) * 100;
	}
	// 후보자 기호(배열 인덱스이기 때문에 0부터 시작함)
	kCandidateVote.Replace( TEXT("#number#"), SI32ToString(siShowCandidateNumber+1) );
	// 후보자 이름
	kCandidateVote.Replace( TEXT("#name#"), m_tszCandidateName[siShowCandidateNumber] );
	// 후보자 득표수
	kCandidateVote.Replace( TEXT("#vote#"), SI32ToString(siCandidateVote) );
	// 후보자 득표율
	kCandidateVote.Replace( TEXT("#percentage#"), REAL32ToString( fPercentage ) );


	m_pcsVote->SetText( kCandidateVote, DT_VCENTER );
	m_pcsDisplay->SetText( kCandidateVote, DT_VCENTER );

	//---------------------------------------------------------------------------------------------------
	//	전체 득표 수 표시 
	//---------------------------------------------------------------------------------------------------

	// 개표율
	if ( m_siTotalVote <= 0 )
	{
		fPercentage = 0.0f;
	}
	else
	{
		fPercentage = m_siNowTotalVote / (REAL32)(m_siTotalVote) * 100;
	}
	// 전체 투표자
	kPercentage.Replace( TEXT("#totalvote#"), SI32ToString( m_siTotalVote ) );
	// 개표율
	kPercentage.Replace( TEXT("#percentage#"), REAL32ToString( fPercentage ) );

	// 설정
	m_pcsPercentage->SetText( kPercentage, DT_VCENTER );
	
}

void CNVoteStatusDlg::WindowModeChange( bool bWindowMode )
{
	if ( VOTESTATUS_MODE_MINIMUM == bWindowMode )
	{
		//-- 보이는 컨트롤
		//m_pcbMaximum->Show( true );
		m_pcsDisplay->Show( true );
		m_pcbMaximum->SetDrawOption( SPR_PUTSPRT );
		
		//-- 안보이는 컨트롤
		m_pcsVote->Show( false );
		m_pcsPercentage->Show( false );
		//m_pcbMinimum->Show( false );
		m_pcbMinimum->SetAlphaValue( 0 );
		m_pcbMinimum->SetDrawOption( SPR_PUTSPRALPHAT );
		
		SaveNowDialog();
				
		//-- 윈도우 크기 최소화
		SetDialogPos( VOTESTATUS_BASE_DIALOG_X, VOTESTATUS_BASE_DIALOG_Y, 0, 0 );
		
		SetMoveable( false );
		
		//-- 반투명 설정
		SetAlphaMode( 0 );
		
	}
	else
	{
		//-- 안보이는 컨트롤
		//m_pcbMaximum->Show( false );
		m_pcsDisplay->Show( false );
		m_pcbMaximum->SetAlphaValue( 0 );
		m_pcbMaximum->SetDrawOption( SPR_PUTSPRALPHAT );

		//-- 보이는 컨트롤
		m_pcsVote->Show( true );
		m_pcsPercentage->Show( true );
		//m_pcbMinimum->Show( true );
		m_pcbMinimum->SetDrawOption( SPR_PUTSPRT );
		
		//-- 윈도우 크기 최대화
		SetDialogPos( m_siWindowPosX, m_siWindowPosY, m_siWindowWidth, m_siWindowHeight );


		SetMoveable( true );

		// 반투명 해제
		SetDrawOption( SPR_PUTSPRT );

	}

	m_bWindowMode = bWindowMode;

	WriteCandidate( m_siNowShowNum );

}

void CNVoteStatusDlg::SaveNowDialog()
{
	//-- 현재 윈도우 크기/위치 저장
	m_siWindowPosX		= this->GetX();
	m_siWindowPosY		= this->GetY();

	m_siWindowWidth		= this->GetWidth();
	m_siWindowHeight	= this->GetHeight();

}

SI08 CNVoteStatusDlg::SearchTopCandidate()
{
	if ( (0 >= m_siCandidateNum) || (MAX_CANDIDATURE_NUM < m_siCandidateNum) )
	{
		return -1;
	}

	SI08 siTopNumber	= -1;
	SI32 siTopVote		= 0;

	for ( SI32 siCount=0; siCount<m_siCandidateNum; siCount++ )
	{
		if ( siTopVote < m_siVote[siCount] )
		{
			siTopNumber = siCount;
			siTopVote	= m_siVote[siCount];
		}
	}

	return siTopNumber;
}

void CNVoteStatusDlg::EndVote()
{
	if ( m_siVoteStatus == VOTESTATUS_VOTE_END )
	{
		return;
	}

	// 현재 모드를 종료로 바꿔주고
	m_siVoteStatus = VOTESTATUS_VOTE_END;

	// 종료시키는 타이머 10초
	m_kUpdateTimer_End.Init( 10 * 1000 );
	// 종료 타이머를 시동한다
	m_kUpdateTimer_End.SetActive( true, GetTickCount() );

	// 진 후보자들의 이미지
	memset( m_siDrawMode, VOTESTATUS_DRAWMODE_LOSE, (sizeof(SI08) * MAX_CANDIDATURE_NUM) );

	// 이긴 후보자의 이미지
	SI08 siSearchNumber = SearchTopCandidate();

	if ( siSearchNumber >= 0 )
	{
		m_siDrawMode[siSearchNumber] = VOTESTATUS_DRAWMODE_WIN;
	}

	// 이긴 후보자의 정보로만 보여지게 고정
	m_siNowShowNum = siSearchNumber;

	// 이긴 후보의 이미지는 맨 처음에 보여지게 하기
	SI32 siTopOrder	= (m_siCandidateNum-1);
	SI32 siSwapNum	= 0;

	// 같은 번호는 바꿀 필요없음
	if ( m_siNowShowNum != siTopOrder )
	{
		siSwapNum						= m_siDrawOrder[m_siNowShowNum];
		m_siDrawOrder[m_siNowShowNum]	= m_siDrawOrder[siTopOrder];
		m_siDrawOrder[siTopOrder]		= siSwapNum;
	}
}

void CNVoteStatusDlg::VoteCount()
{
	if ( (m_siCandidateNum <= 0) || (m_siCandidateNum > MAX_CANDIDATURE_NUM) )
	{
		return;
	}

	if ( m_siTopVoteNum <= 0 )
	{
		return;
	}

	SI32	siNowTotalVote = 0;


	for ( SI32 siIndex=0; siIndex<m_siCandidateNum; siIndex++ )
	{
		//-- 계산 영역
		m_siDrawVote[siIndex]++;

		if ( m_siDrawVote[siIndex] >= m_siVote[siIndex] )
		{
			m_siDrawVote[siIndex] = m_siVote[siIndex];
		}

		siNowTotalVote += m_siDrawVote[siIndex];
	}

	m_siNowTotalVote = siNowTotalVote;


	// 개표가 끝났는지 확인하고 끝났다면
	if ( m_siNowTotalVote >= m_siTotalVote )
	{
		EndVote();
	}

}
