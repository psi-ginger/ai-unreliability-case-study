#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "Char/CharManager/CharManager.h"

#include "NAcuTradeDlg_Parent.h"

#include "NAcuTradeDlg_Sell.h"
#include "NAcuTradeDlg_Buy.h"
#include "NAcuTradeDlg_Withdraw.h"
#include "NAcuTradeDlg_Passport.h"


CNAcuTradeDlg_Parent::CNAcuTradeDlg_Parent() : CDialogController()
{
	ZeroMemory( m_pTabDialog, sizeof(m_pTabDialog) );

}

CNAcuTradeDlg_Parent::~CNAcuTradeDlg_Parent()
{
	for ( SI32 siIndex=0; siIndex<ACUTRADE_TAB_MAX; ++siIndex )
	{
		SAFE_DELETE( m_pTabDialog[siIndex] );
	}

}

void CNAcuTradeDlg_Parent::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NAcuTradeDlg/DLG_NAcuTrade_Parent.ddf" ) );
	file.CreatePopupDialog( this, NACUTRADEDLG_PARENT, TEXT( "dialog_acutrade_parent" ), StaticAcuTradeDlg_ParentProc );

	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PARENT_STATIC_PORTRAIT,	this ), NACUTRADE_PARENT_STATIC_PORTRAIT,	TEXT( "statictext_portrait" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PARENT_STATIC_NONAME2,	this ), NACUTRADE_PARENT_STATIC_NONAME2,	TEXT( "statictext_NONAME2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PARENT_STATIC_NONAME3,	this ), NACUTRADE_PARENT_STATIC_NONAME3,	TEXT( "statictext_NONAME3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_PARENT_STATIC_NONAME4,	this ), NACUTRADE_PARENT_STATIC_NONAME4,	TEXT( "statictext_NONAME4" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_PARENT_EDIT_NOTICE,		this ), NACUTRADE_PARENT_EDIT_NOTICE,		TEXT( "editbox_notice" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_PARENT_EDIT_EXCHANGERATE,	this ), NACUTRADE_PARENT_EDIT_EXCHANGERATE,	TEXT( "editbox_exchangerate" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_PARENT_EDIT_HAVEACU,		this ), NACUTRADE_PARENT_EDIT_HAVEACU,		TEXT( "editbox_haveacu" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PARENT_BUTTON_SELL,		this ), NACUTRADE_PARENT_BUTTON_SELL,		TEXT( "button_sell" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PARENT_BUTTON_BUY,		this ), NACUTRADE_PARENT_BUTTON_BUY,		TEXT( "button_buy" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PARENT_BUTTON_WITHDRAW,	this ), NACUTRADE_PARENT_BUTTON_WITHDRAW,	TEXT( "button_withdraw" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PARENT_BUTTON_PASSPORT,	this ), NACUTRADE_PARENT_BUTTON_PASSPORT,	TEXT( "button_passport" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PARENT_BUTTON_REFRESH,	this ), NACUTRADE_PARENT_BUTTON_REFRESH,	TEXT( "button_refresh" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_PARENT_BUTTON_CLOSE,		this ), NACUTRADE_PARENT_BUTTON_CLOSE,		TEXT( "button_close" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NACUTRADE_PARENT_OUTLINE_NONAME1,	this ), NACUTRADE_PARENT_OUTLINE_NONAME1,	TEXT( "outline_NONAME1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NACUTRADE_PARENT_OUTLINE_NONAME2,	this ), NACUTRADE_PARENT_OUTLINE_NONAME2,	TEXT( "outline_NONAME2" ) );
	
	//-- 컨트롤 설정

	// 다이얼로그 설명
	CEdit* pNotice = m_InterfaceFactory.GetEdit( NACUTRADE_PARENT_EDIT_NOTICE );
	if ( pNotice )
	{
		TCHAR* pszNotice = GetTxtFromMgr(6146);
		pNotice->SetText( pszNotice );
	}

	// 팔기
	m_pTabDialog[ACUTRADE_TAB_SELL] = new CNAcuTradeDlg_Sell;
	if ( NULL == m_pTabDialog[ACUTRADE_TAB_SELL] )
	{
		DeleteDialog();
	}
	CNAcuTradeDlg_Sell* pSell = (CNAcuTradeDlg_Sell*)m_pTabDialog[ACUTRADE_TAB_SELL];
	if ( pSell )
	{
		pSell->Create( this );
		pSell->Hide();
	}

	// 사기
	m_pTabDialog[ACUTRADE_TAB_BUY] = new CNAcuTradeDlg_Buy;
	if ( NULL == m_pTabDialog[ACUTRADE_TAB_BUY] )
	{
		DeleteDialog();
	}
	CNAcuTradeDlg_Buy* pBuy = (CNAcuTradeDlg_Buy*)m_pTabDialog[ACUTRADE_TAB_BUY];
	if ( pBuy )
	{
		pBuy->Create( this );
		pBuy->Hide();
	}

	// 회수
	m_pTabDialog[ACUTRADE_TAB_WITHDRAW] = new CNAcuTradeDlg_Withdraw;
	if ( NULL == m_pTabDialog[ACUTRADE_TAB_WITHDRAW] )
	{
		DeleteDialog();
	}
	CNAcuTradeDlg_Withdraw* pWithdraw = (CNAcuTradeDlg_Withdraw*)m_pTabDialog[ACUTRADE_TAB_WITHDRAW];
	if ( pWithdraw )
	{
		pWithdraw->Create( this );
		pWithdraw->Hide();
	}

	// 여권
	m_pTabDialog[ACUTRADE_TAB_PASSPORT] = new CNAcuTradeDlg_Passport;
	if ( NULL == m_pTabDialog[ACUTRADE_TAB_PASSPORT] )
	{
		DeleteDialog();
	}
	CNAcuTradeDlg_Passport* pPassport = (CNAcuTradeDlg_Passport*)m_pTabDialog[ACUTRADE_TAB_PASSPORT];
	if ( pPassport )
	{
		pPassport->Create( this );
		pPassport->Hide();
	}


	//-- 기본 초기 설정
	SelectTabDialog( ACUTRADE_TAB_SELL );

	Refresh( 0 );

	Request_WorldMoney_TradeList();
		
}

void CALLBACK CNAcuTradeDlg_Parent::StaticAcuTradeDlg_ParentProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNAcuTradeDlg_Parent *pThis = (CNAcuTradeDlg_Parent*) pControllerMgr;
	pThis->NAcuTradeDlg_ParentProc( nEvent, nControlID, pControl );
}

void CALLBACK CNAcuTradeDlg_Parent::NAcuTradeDlg_ParentProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	
	switch( nControlID )
	{
	case NACUTRADE_PARENT_BUTTON_SELL:
		{
			SelectTabDialog( ACUTRADE_TAB_SELL );
		}
		break;
	case NACUTRADE_PARENT_BUTTON_BUY:
		{
			SelectTabDialog( ACUTRADE_TAB_BUY );
		}
		break;

	case NACUTRADE_PARENT_BUTTON_WITHDRAW:
		{
			SelectTabDialog( ACUTRADE_TAB_WITHDRAW );
		}
		break;

	case NACUTRADE_PARENT_BUTTON_PASSPORT:
		{
			SelectTabDialog( ACUTRADE_TAB_PASSPORT );
		}
		break;

	case NACUTRADE_PARENT_BUTTON_REFRESH:
		{
			SendForRefresh();
		}
		break;

	case NACUTRADE_PARENT_BUTTON_CLOSE:
		{
			DeleteDialog();
		}
		break;

	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;

	}
	

}

void CNAcuTradeDlg_Parent::Action()
{
}

void CNAcuTradeDlg_Parent::Show()
{
	CControllerMgr::Show( true );
}

void CNAcuTradeDlg_Parent::Hide()
{
	CControllerMgr::Show( false );
}

void CNAcuTradeDlg_Parent::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{

	switch ( MsgIndex )
	{
		case ACUTRADE_RETMSG_ORDERTOSELL:
			{
				CNAcuTradeDlg_Sell* pclSell = (CNAcuTradeDlg_Sell*)m_pTabDialog[ACUTRADE_TAB_SELL];
				if ( pclSell )
				{
					pclSell->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
				}
			}
			break;

		case ACUTRADE_RETMSG_ORDERTOEXTEND:
			{
				CNAcuTradeDlg_Passport* pclPassport = (CNAcuTradeDlg_Passport*)m_pTabDialog[ACUTRADE_TAB_PASSPORT];
				if ( pclPassport )
				{
					pclPassport->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
				}

			}
			break;
	}
}

void CNAcuTradeDlg_Parent::SetWithdrawDialogMemberData( GMONEY gmSavedAcu, cltTradeAcuList* pclAcuList/* =NULL  */)
{
	CNAcuTradeDlg_Withdraw* pWithdraw = (CNAcuTradeDlg_Withdraw*)m_pTabDialog[ACUTRADE_TAB_WITHDRAW];
	if ( pWithdraw )
	{
		pWithdraw->SetMemberData( gmSavedAcu, pclAcuList );
		pWithdraw->Refresh();
	}
}

void CNAcuTradeDlg_Parent::SelectTabDialog( SI32 siTabIndex )
{
	
	if ( (ACUTRADE_TAB_SELL > siTabIndex) || (ACUTRADE_TAB_MAX <= siTabIndex) )
	{
		return;
	}

	// 텝 컨틀롤러가 추가되면 아래 siID배열에 넣으세요
	SI32 siID[]		= { NACUTRADE_PARENT_BUTTON_SELL, NACUTRADE_PARENT_BUTTON_BUY, NACUTRADE_PARENT_BUTTON_WITHDRAW, NACUTRADE_PARENT_BUTTON_PASSPORT  };
	SI32 siCount	= sizeof( siID ) / sizeof( siID[0] );


	for ( SI32 siIndex=0; siIndex<siCount; ++siIndex )
	{
		SI32		siNowID		= siID[siIndex];
		CButton*	pclButton	= m_InterfaceFactory.GetButton( siNowID );
		if ( NULL == pclButton )
		{
			continue;
		}

		if ( siTabIndex == siIndex )
		{
			SetTabDialogButton( pclButton, CONTROLLER_BUTTONTEXT_COLOR, FW_BOLD );
			if ( m_pTabDialog[siIndex] )
			{
				m_pTabDialog[siIndex]->Show( true );
			}
		}
		else
		{
			SetTabDialogButton( pclButton, CONTROLLER_BACK_BUTTONTEXT_COLOR, FW_NORMAL );
			if ( m_pTabDialog[siIndex] )
			{
				m_pTabDialog[siIndex]->Show( false );
			}
		}
	}
	

}

void CNAcuTradeDlg_Parent::SetTabDialogButton( CButton* pclButton, COLORREF FontColor, SI16 siWeight )
{
	if ( NULL == pclButton )
	{
		return;
	}

	pclButton->SetFontColor( FontColor  );
	pclButton->SetFontWeight( siWeight );
	pclButton->Refresh();

}

void CNAcuTradeDlg_Parent::Request_WorldMoney_TradeList( void )
{
	cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_TRADELIST, 0, NULL );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNAcuTradeDlg_Parent::Refresh( GMONEY gmExchangeRate /* = (-1 */)
{
	RefreshAcuInfo( gmExchangeRate );

	RefreshSell();
	RefreshBuy();
	RefreshWithdraw();
	RefreshPassport();
}

void CNAcuTradeDlg_Parent::RefreshAcuInfo( GMONEY gmExchangeRate /* = (-1 */)
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	// 음수일때는 업데이트 안한다
	if ( 0 < gmExchangeRate )
	{
		CEdit* pExchangeRate = m_InterfaceFactory.GetEdit( NACUTRADE_PARENT_EDIT_EXCHANGERATE );
		if ( pExchangeRate )
		{
			TCHAR	szExchangeRate[128]	= { '\0', };
			TCHAR*	pUnit				= GetTxtFromMgr(611);
			g_JYLibFunction.SetNumUnit( gmExchangeRate, szExchangeRate, 128, pUnit );

			pExchangeRate->SetText( szExchangeRate );
		}
	}

	CEdit* pHaveAcu = m_InterfaceFactory.GetEdit( NACUTRADE_PARENT_EDIT_HAVEACU );
	if ( pHaveAcu )
	{
		TCHAR	szHaveAcu[128]	= { '\0', };
		GMONEY	gmHaveAcu		= pclchar->pclCI->clWorldMoney.GetWorldMoney();
		g_JYLibFunction.SetNumUnit( gmHaveAcu, szHaveAcu, 128 );

		pHaveAcu->SetText( szHaveAcu );
	}
}

void CNAcuTradeDlg_Parent::RefreshSell( void )
{
	CNAcuTradeDlg_Sell* pSell = (CNAcuTradeDlg_Sell*)m_pTabDialog[ACUTRADE_TAB_SELL];
	if ( pSell )
	{
		pSell->Refresh();
	}
}

void CNAcuTradeDlg_Parent::RefreshBuy( void )
{
	CNAcuTradeDlg_Buy* pBuy = (CNAcuTradeDlg_Buy*)m_pTabDialog[ACUTRADE_TAB_BUY];
	if ( pBuy )
	{
		pBuy->Refresh();
	}
}

void CNAcuTradeDlg_Parent::RefreshWithdraw( void )
{
	CNAcuTradeDlg_Withdraw* pWithdraw = (CNAcuTradeDlg_Withdraw*)m_pTabDialog[ACUTRADE_TAB_WITHDRAW];
	if ( pWithdraw )
	{
		pWithdraw->Refresh();
	}
}

void CNAcuTradeDlg_Parent::RefreshPassport( void )
{
	CNAcuTradeDlg_Passport* pPassport = (CNAcuTradeDlg_Passport*)m_pTabDialog[ACUTRADE_TAB_PASSPORT];
	if ( pPassport )
	{
		pPassport->Refresh();
	}
}

void CNAcuTradeDlg_Parent::SendForRefresh( void )
{
	Request_WorldMoney_TradeList();

	CNAcuTradeDlg_Withdraw* pWithdraw = (CNAcuTradeDlg_Withdraw*)m_pTabDialog[ACUTRADE_TAB_WITHDRAW];
	if ( pWithdraw )
	{
		pWithdraw->Request_SavedACU_List();
	}
}
