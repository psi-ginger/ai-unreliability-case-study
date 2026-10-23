#include "NAttendCheckDlg.h"

#include "Char\CharManager\CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../Client/Client.h"



extern cltCommonLogic* pclClient;

CNAttendCheckDlg::CNAttendCheckDlg()
{	
	Initialize();

	m_siAttendDay			= 0;
	m_siOldAttendDay		= 0;
	m_bDialogMode			= NATTENDCHECKDLG_MODE_MINIMUM;

	m_pointPosition[0].x	= 446;
	m_pointPosition[0].y	= 446;

	m_pointPosition[1].x	= 14;
	m_pointPosition[1].y	= 446;

	m_pointPosition[2].x	= 10;
	m_pointPosition[2].y	= 18;

	m_pointPosition[3].x	= 442;
	m_pointPosition[3].y	= 18;

	m_pointAddNum[0].x		= -1;
	m_pointAddNum[0].y		= 0;

	m_pointAddNum[1].x		= 0;
	m_pointAddNum[1].y		= -1;

	m_pointAddNum[2].x		= +1;
	m_pointAddNum[2].y		= 0;

	m_pointAddNum[3].x		= 0;
	m_pointAddNum[3].y		= +1;

	m_siAnimation			= ANITYPE_WAIT0;
	m_siDirection			= SOUTH | EAST;
	m_siFont				= 0;

	m_clAniInfo.Init1();

	GP.LoadSprFromFile( TEXT("NInterface/image/IMAGE_62x62_00_000.SPR"), &m_sprRaccoon );
}

CNAttendCheckDlg::~CNAttendCheckDlg()
{
	if ( m_sprRaccoon.pImage )
	{
		GP.FreeSpr( m_sprRaccoon );
	}

	Destroy();
}


void CNAttendCheckDlg::Init()
{

}

void CNAttendCheckDlg::Destroy()
{

}

void CNAttendCheckDlg::Create()
{
	if ( IsCreate() )
		return;

	cltClient* pclclient = (cltClient*)pclClient;

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NAttendCheck/DLG_NAttendCheck.ddf"));
	file.CreatePopupDialog( this, NATTENDCHECK_DLG , TEXT("dialog_attendcheck"), StaticAttendCheckDlgProc );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NATTENDCHECK_STATIC_RESULT,		this ), NATTENDCHECK_STATIC_RESULT,		"statictext_result" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	NATTENDCHECK_STATIC_COUNT,		this ), NATTENDCHECK_STATIC_COUNT,		"statictext_count" );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,	NATTENDCHECK_EDIT_EXPLAIN,		this ), NATTENDCHECK_EDIT_EXPLAIN,		"editbox_explain" );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NATTENDCHECK_BUTTON_STAMP,		this ), NATTENDCHECK_BUTTON_STAMP,		"button_stamp" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NATTENDCHECK_BUTTON_REWARD,		this ), NATTENDCHECK_BUTTON_REWARD,		"button_reward" );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NATTENDCHECK_BUTTON_ATTEND,		this ), NATTENDCHECK_BUTTON_ATTEND,		"button_attend" );
	
	
	SetDialogPos( 200, 100, GetWidth(), GetHeight() );

	SetDialogMode( NATTENDCHECKDLG_MODE_MINIMUM );

	NTCHARString256 kText( GetTxtFromMgr(7994) );
	((CEdit*)(m_InterfaceFactory.GetEdit(NATTENDCHECK_EDIT_EXPLAIN)))->SetText( kText );

	kText = TEXT("");
	kText = GetTxtFromMgr(8189);
	((CStatic*)(m_InterfaceFactory.GetStatic(NATTENDCHECK_STATIC_COUNT)))->SetText( kText, DT_CENTER );

	m_kUpdateTimerForAinmation.Init( 100 );
	m_kUpdateTimerForAinmation.SetActive( true, GetTickCount() );
}

void CALLBACK CNAttendCheckDlg::StaticAttendCheckDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNAttendCheckDlg *pThis = (CNAttendCheckDlg*) pControllerMgr;
	pThis->NAttendCheckDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNAttendCheckDlg::NAttendCheckDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			SetDialogMode( NATTENDCHECKDLG_MODE_MINIMUM );
		}
		break;

	case NATTENDCHECK_BUTTON_ATTEND:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					SetDialogMode( NATTENDCHECKDLG_MODE_MAXIMUM );
				}
				break;
			}
		}
		break;
	// 출석 도장 찍기
	case NATTENDCHECK_BUTTON_STAMP:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendCheckAttend();
				}
				break;
			}
		}
		break;
	// 보상받기
	case NATTENDCHECK_BUTTON_REWARD:
		{
			switch(nEvent)
			{
			case EVENT_BUTTON_CLICKED:
				{
					SendAttendRewardItem();
				}
				break;
			}
		}
		break;
	} // switch ( nControlID ) 종료
}

void CNAttendCheckDlg::SendCheckAttend()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( pclclient == NULL )
	{
		return;
	}

	cltGameMsgRequest_Attend clAttend( ATTEND_EVENTTYPE_ATTENDANCE );
	cltMsg clMsg( GAMEMSG_REQUEST_ATTEND, sizeof( clAttend ), (BYTE*)&clAttend );
	pclclient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
}

void CNAttendCheckDlg::SendAttendRewardItem()
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( pclclient == NULL )
	{
		return;
	}

	cltGameMsgRequest_AttendRewardItem clGameMsg( ATTEND_EVENTTYPE_SEASON2 );
	cltMsg clMsg( GAMEMSG_REQUEST_ATTENDREWARDITEM, sizeof( clGameMsg ), (BYTE*)&clGameMsg );
	pclclient->pclCM->CR[1]->SendNetMsg( (sPacketHeader*)&clMsg );
}

void CNAttendCheckDlg::SetAttendDay( SI32 siAttendDay )
{
	if ( siAttendDay < 0 )
	{
		return;
	}

	m_siAttendDay = siAttendDay;

	CStatic *pStatic = (CStatic*)m_InterfaceFactory.GetStatic(NATTENDCHECK_STATIC_RESULT);
	if ( pStatic == NULL )
	{
		return;
	}

	pStatic->SetFontWidth( 14 );
	pStatic->SetFontHeight( 30 );
	pStatic->SetFontWeight( 600 );
	pStatic->SetText( SI32ToString( m_siAttendDay ), DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CNAttendCheckDlg::Action()
{

}


void CNAttendCheckDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY )
{
	if ( IsShow() && (m_bDialogMode == NATTENDCHECKDLG_MODE_MAXIMUM) )
	{
		//-- 라쿤 그리기
		SI32 siCalcDay		= (SI32)( m_siOldAttendDay % Const_Attendance_Modulation_Day );
		SI32 siDirection	= (SI32)(siCalcDay/7);
		SI32 siPosition		= (SI32)(siCalcDay%7);

		if ( siDirection > 3 )
		{
			return;
		}

		SI32 siStartX		= GetX() + ScreenRX + m_pointPosition[siDirection].x - 15;
		SI32 siStartY		= GetY() + ScreenRY	+ m_pointPosition[siDirection].y - 15;

		SI32 siDrawX		= siStartX + ((62*siPosition) * m_pointAddNum[siDirection].x);
		SI32 siDrawY		= siStartY + ((62*siPosition) * m_pointAddNum[siDirection].y);


		DrawRaccoon( siDrawX, siDrawY, siDirection );

		//-- 출석 도장 찍기
		for ( SI32 siCount=0; siCount<siCalcDay; siCount++ )
		{
			SI32 siStampDirection	= (SI32)(siCount/7);
			SI32 siStampPosition	= (SI32)(siCount%7);

			if ( siStampDirection > 3 )
			{
				continue;
			}

			SI32 siStampStartX		= GetX() + ScreenRX + m_pointPosition[siStampDirection].x;
			SI32 siStampStartY		= GetY() + ScreenRY	+ m_pointPosition[siStampDirection].y;

			SI32 siStampDrawX		= siStampStartX + ((62*siStampPosition) * m_pointAddNum[siStampDirection].x);
			SI32 siStampDrawY		= siStampStartY + ((62*siStampPosition) * m_pointAddNum[siStampDirection].y);

			GP.PutSprAlpha( &m_sprRaccoon, siStampDrawX, siStampDrawY, 10 );
		}
	}	// if ( IsShow() && (m_bDialogMode == NATTENDCHECKDLG_MODE_MAXIMUM) ) 종료

}

void CNAttendCheckDlg::SetDialogMode( bool bDialogMode )
{

	//-- 최소화 모드일때
	if ( bDialogMode == NATTENDCHECKDLG_MODE_MINIMUM )
	{
		SI16 siPos_X		= 430;
		SI16 siPos_Y		= 650;
		SI16 siPos_Widht	= 54;
		SI16 siPos_Height	= 54;

		// 풀스크린일때는 좌표를 조정해 준다
		if ( g_bFullScreen == TRUE )
		{
			siPos_X += 30;
			siPos_Y += 45;
		}

		// 현재 위치를 기억하고
		SaveNowDialogPos();
		// 알파값을 줘서 투명하게 만든 후
		SetAlphaMode( 0 );
		// 크기와 위치를 변경한다
		SetDialogPos( siPos_X, siPos_Y, siPos_Widht, siPos_Height );
		// 종료버튼 지우기
		Show( COMMON_DIALOG_CLOSEBUTTON, false );

		// 보이는 컨트롤
		((CButton*)m_InterfaceFactory.GetButton(NATTENDCHECK_BUTTON_ATTEND))->Show( true );

		// 안보이는 컨트롤
		((CStatic*)m_InterfaceFactory.GetStatic(NATTENDCHECK_STATIC_RESULT))->Show( false );
		((CStatic*)m_InterfaceFactory.GetStatic(NATTENDCHECK_STATIC_COUNT))->Show( false );

		((CEdit*)m_InterfaceFactory.GetEdit(NATTENDCHECK_EDIT_EXPLAIN))->Show( false );

		((CButton*)m_InterfaceFactory.GetButton(NATTENDCHECK_BUTTON_STAMP))->Show( false );
		((CButton*)m_InterfaceFactory.GetButton(NATTENDCHECK_BUTTON_REWARD))->Show( false );

	}
	//-- 최대화 모드일때
	else if ( bDialogMode == NATTENDCHECKDLG_MODE_MAXIMUM )
	{
		// 반투명 해제
		SetDrawOption( SPR_PUTSPRT );
		// 크기와 위치 원래대로 되돌림
		SetDialogPos( m_siPosition_X, m_siPosition_Y, m_siPosition_Width, m_siPosition_Height );
		// 종료버튼 나타냄
		Show( COMMON_DIALOG_CLOSEBUTTON, true );
		
		// 안보이는 컨트롤
		((CButton*)m_InterfaceFactory.GetButton(NATTENDCHECK_BUTTON_ATTEND))->Show( false );

		// 보이는 컨트롤
		((CStatic*)m_InterfaceFactory.GetStatic(NATTENDCHECK_STATIC_RESULT))->Show( true );
		((CStatic*)m_InterfaceFactory.GetStatic(NATTENDCHECK_STATIC_COUNT))->Show( true );

		((CEdit*)m_InterfaceFactory.GetEdit(NATTENDCHECK_EDIT_EXPLAIN))->Show( true );

		((CButton*)m_InterfaceFactory.GetButton(NATTENDCHECK_BUTTON_STAMP))->Show( true );
		((CButton*)m_InterfaceFactory.GetButton(NATTENDCHECK_BUTTON_REWARD))->Show( true );
	}

	m_bDialogMode = bDialogMode;
}

void CNAttendCheckDlg::SaveNowDialogPos()
{
	m_siPosition_X		= GetX();
	m_siPosition_Y		= GetY();

	m_siPosition_Width	= GetWidth();
	m_siPosition_Height	= GetHeight();
}

void CNAttendCheckDlg::DrawRaccoon( SI32 ScreenRX, SI32 ScreenRY, SI32 siDirection )
{
	SI32 siRaccoonKind = pclClient->pclKindInfoSet->FindKindFromCode( "KIND_RACCOON" );
	if ( siRaccoonKind <= 0 )
	{
		return;
	}

	cltKindInfo* pclKI = pclClient->pclKindInfoSet->pclKI[ siRaccoonKind ];
	if ( pclKI == NULL )
	{
		return;
	}

	SI32	siSetDirection[4] = { (WEST | SOUTH), (NORTH | WEST), (EAST | NORTH), (SOUTH | EAST) };

	SI32	siAniRef		= pclKI->clCharAniManager.FindRefFromAnitype(m_siAnimation);
	SI32	siFramenumber	= pclKI->clCharAniManager.GetFrameNum(siAniRef);
	SI32	siDelay;
	SI32	siEffect		= 0;
	
	SI32	siAddX			= 0;
	SI32	siAddY			= 0;

	TSpr	*pSprRaccoon	= NULL;
	bool	bReverse		= false;


	if ( m_siAttendDay == m_siOldAttendDay )
	{
		m_siAnimation = ANITYPE_WAIT0;
		m_siDirection = siSetDirection[3];
	}
	else
	{
		if ( m_siAnimation == ANITYPE_WAIT0 )
		{
			m_clAniInfo.SetAniStep(0);
		}
		m_siAnimation = ANITYPE_MOVE;
		m_siDirection = siSetDirection[siDirection];
	}


	if ( ((cltClient*)pclClient)->GetFrameDelay() == 25 )
	{
		siDelay = m_clAniInfo.siAniDelay * 2 + 2;
	}
	else
	{
		siDelay = m_clAniInfo.siAniDelay + 2;
	} 

	m_clAniInfo.IncreaseAniDelayor(1);

	if ( m_clAniInfo.GetAniDelayor() >= siDelay )
	{
		m_clAniInfo.SetAniDelayor(0);

		m_clAniInfo.IncreaseAniStep(1);

		if(siFramenumber == 0)
		{
			m_clAniInfo.SetAniStep(0);
		}
		else
		{
			m_clAniInfo.SetAniStep(m_clAniInfo.GetAniStep() % siFramenumber );
		}

	}

	//if ( (m_siAttendDay > m_siOldAttendDay) && (m_clAniInfo.GetAniStep() != (siFramenumber-1)) )
	if ( (m_siAttendDay > m_siOldAttendDay) )
	{
		siAddX = ((61 / siFramenumber) * m_clAniInfo.GetAniStep()) * m_pointAddNum[siDirection].x;
		siAddY = ((61 / siFramenumber) * m_clAniInfo.GetAniStep()) * m_pointAddNum[siDirection].y;
	}
	if ( (m_siAttendDay > m_siOldAttendDay) && (m_clAniInfo.GetAniStep() == (siFramenumber-1)) )
	{
		++m_siOldAttendDay;

		if ( m_siOldAttendDay >= m_siAttendDay )
		{
			m_siOldAttendDay = m_siAttendDay;
		}
	}

	pclKI->clCharAniManager.DecideDrawImg( m_siAnimation, m_siDirection, m_clAniInfo, TABS(pclClient->CurrentClock), &pSprRaccoon, &m_siFont, &bReverse, &siEffect );

	GP.PutSpr( pSprRaccoon, ScreenRX + siAddX, ScreenRY + siAddY, m_siFont, bReverse );



}