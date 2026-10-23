#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "Char/CharManager/CharManager.h"

#include "Msg/MsgType-WorldMoney.h"

#include "NAcuTradeDlg_Parent.h"
#include "NAcuTradeDlg_Sell.h"


CNAcuTradeDlg_Sell::CNAcuTradeDlg_Sell() : CDialogController()
{

}

CNAcuTradeDlg_Sell::~CNAcuTradeDlg_Sell()
{

}

void CNAcuTradeDlg_Sell::Create(CControllerMgr* pParent)
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NAcuTradeDlg/DLG_NAcuTrade_Sell.ddf" ) );
	file.CreateChildDialog( this, NACUTRADEDLG_SELL, TEXT( "dialog_acutrade_sell" ), StaticAcuTradeDlg_SellProc, pParent );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_SELL_STATIC_NONAME1,	this ), NACUTRADE_SELL_STATIC_NONAME1,	TEXT( "statictext_NONAME1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_SELL_STATIC_NONAME2,	this ), NACUTRADE_SELL_STATIC_NONAME2,	TEXT( "statictext_NONAME2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_SELL_STATIC_NONAME3,	this ), NACUTRADE_SELL_STATIC_NONAME3,	TEXT( "statictext_NONAME3" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_SELL_STATIC_NONAME4,	this ), NACUTRADE_SELL_STATIC_NONAME4,	TEXT( "statictext_NONAME4" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_SELL_STATIC_NONAME5,	this ), NACUTRADE_SELL_STATIC_NONAME5,	TEXT( "statictext_NONAME5" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_SELL_EDIT_LISTEDACU,	this ), NACUTRADE_SELL_EDIT_LISTEDACU,	TEXT( "editbox_listedacu" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_SELL_EDIT_PRICE,		this ), NACUTRADE_SELL_EDIT_PRICE,		TEXT( "editbox_price" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_SELL_EDIT_ACU,		this ), NACUTRADE_SELL_EDIT_ACU,		TEXT( "editbox_acu" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_SELL_EDIT_UNIT,		this ), NACUTRADE_SELL_EDIT_UNIT,		TEXT( "editbox_unit" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_SELL_EDIT_TOTAL,		this ), NACUTRADE_SELL_EDIT_TOTAL,		TEXT( "editbox_total" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_SELL_BUTTON_CANCEL,	this ), NACUTRADE_SELL_BUTTON_CANCEL,	TEXT( "button_cancel" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_SELL_BUTTON_SELL,		this ), NACUTRADE_SELL_BUTTON_SELL,		TEXT( "button_sell" ) );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NACUTRADE_SELL_OUTLINE_NONAME1,	this ), NACUTRADE_SELL_OUTLINE_NONAME1,	TEXT( "outline_NONAME1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NACUTRADE_SELL_OUTLINE_NONAME2,	this ), NACUTRADE_SELL_OUTLINE_NONAME2,	TEXT( "outline_NONAME2" ) );
	

	//-- 컨트롤 설정
	CEdit* pAcu = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_ACU );
	if ( pAcu )
	{
		pAcu->SetNumberMode( true );
	}

	CEdit* pUnit = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_UNIT );
	if ( pUnit )
	{
		pUnit->SetNumberMode( true );
	}


	//-- 기본 초기 설정

}

void CALLBACK CNAcuTradeDlg_Sell::StaticAcuTradeDlg_SellProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNAcuTradeDlg_Sell *pThis = (CNAcuTradeDlg_Sell*) pControllerMgr;
	if ( pThis )
	{
		pThis->NAcuTradeDlg_SellProc( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNAcuTradeDlg_Sell::NAcuTradeDlg_SellProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

	switch( nControlID )
	{
		case NACUTRADE_SELL_EDIT_ACU:
		case NACUTRADE_SELL_EDIT_UNIT:
			{
				switch( nEvent )
				{
					case EVENT_EDITBOX_CHANGE:
						{
							CalcTotal();
						}
						break;
				}

			}
			break;

		case NACUTRADE_SELL_BUTTON_CANCEL:
			{
				OrderToCancel();
			}
			break;

		case NACUTRADE_SELL_BUTTON_SELL:
			{
				OrderToSell();
			}
			break;
	}


}

void CNAcuTradeDlg_Sell::Action()
{
}

void CNAcuTradeDlg_Sell::Show()
{
	CControllerMgr::Show( true );
}

void CNAcuTradeDlg_Sell::Hide()
{
	CControllerMgr::Show( false );
}

void CNAcuTradeDlg_Sell::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch ( MsgIndex )
	{
		case ACUTRADE_RETMSG_ORDERTOSELL:
			{
				if ( false == RetValue )
				{
					return;
				}

				if ( sizeof(stACUTRADE_RETMSG_ORDERTOSELL) > DataLen )
				{
					return;
				}

				stACUTRADE_RETMSG_ORDERTOSELL* pstSell = (stACUTRADE_RETMSG_ORDERTOSELL*)pData;
				if ( NULL == pstSell )
				{
					return;
				}

				cltGameMsgRequest_WorldMoney_SellOrder clinfo( pstSell->gmSellAcu, pstSell->gmSellUnit );
				cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_SELLORDER, sizeof(clinfo), (BYTE *)&clinfo );
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			break;
	}
}

void CNAcuTradeDlg_Sell::Refresh( void )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}

	CEdit* pListedAcu = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_LISTEDACU );
	if ( pListedAcu )
	{
		TCHAR	szListedAcu[128]	= { '\0', };
		GMONEY	gmListedAcu			= pclchar->pclCI->clWorldMoney.siTradeWorldMoney;
		g_JYLibFunction.SetNumUnit( gmListedAcu, szListedAcu, 128 );

		pListedAcu->SetText( szListedAcu );
	}

	CEdit* pPrice = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_PRICE );
	if ( pPrice )
	{
		TCHAR	szPrice[128]	= { '\0', };
		GMONEY	gmPrice			= pclchar->pclCI->clWorldMoney.siTradeWorldMoneyPrice;
		g_JYLibFunction.SetNumUnit( gmPrice, szPrice, 128 );

		pPrice->SetText( szPrice );
	}
}

void CNAcuTradeDlg_Sell::CalcTotal( void )
{
	CEdit* pAcu = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_ACU );
	if ( NULL == pAcu )
	{
		return;
	}

	CEdit* pUnit = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_UNIT );
	if ( NULL == pUnit )
	{
		return;
	}

	CEdit* pTotal = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_TOTAL );
	if ( NULL == pTotal )
	{
		return;
	}

	GMONEY	gmAcu	= _tstoi( pAcu->GetText() );
	GMONEY	gmUnit	= _tstoi( pUnit->GetText() );
	GMONEY	gmTotal	= gmAcu * gmUnit;

	TCHAR	szTotal[128] = { '\0', };
	g_JYLibFunction.SetNumUnit( gmTotal, szTotal, 128 );
	
	pTotal->SetText( szTotal );
}

void CNAcuTradeDlg_Sell::OrderToSell( void )
{
	// == 기본적으로 쓰일 변수들 생성 ==
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

	CEdit* pAcu = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_ACU );
	if ( NULL == pAcu )
	{
		return;
	}

	CEdit* pUnit = m_InterfaceFactory.GetEdit( NACUTRADE_SELL_EDIT_UNIT );
	if ( NULL == pUnit )
	{
		return;
	}
	
	NTCHARString128 strTitle;
	NTCHARString128 strText;

	// == 여기까지 ==

	// 유효값 체크
	GMONEY	gmSellAcu	= _tstoi( pAcu->GetText() );
	if ( 0 >= gmSellAcu )
	{
		SetFocus( pAcu );
		return;
	}

	// 소지 아큐 확인하기
	if ( gmSellAcu > pclchar->pclCI->clWorldMoney.GetWorldMoney() )
	{
		strTitle	= GetTxtFromMgr( 6261 );
		strText		= GetTxtFromMgr( 6262 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		return;
	}
	
	// 유효값 체크
	GMONEY	gmSellUnit	= _tstoi( pUnit->GetText() );
	if ( 0 >= gmSellUnit )
	{
		SetFocus( pUnit );
		return;
	}

	// 수수료 확인
	GMONEY	gmFee		= gmSellAcu * gmSellUnit / 100;
	if ( gmFee > pclchar->pclCI->clIP.GetMoney() )
	{
		strTitle	= GetTxtFromMgr( 6198 );
		strText		= GetTxtFromMgr( 6199 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		return;
	}

	// 계좌 확인
	if ( 0 == pclchar->pclCI->clBank.clAccountNum.GetAccountNum() )
	{
		strTitle	= GetTxtFromMgr( 6357 );
		strText		= GetTxtFromMgr( 6175 );

		pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( strTitle, strText );
		return;
	}

	// 화폐단위 콤마표시
	TCHAR szFee[128] = { '\0', };
	g_JYLibFunction.SetNumUnit( gmFee, szFee, 128 );

	strTitle	= GetTxtFromMgr( 6196 );
	strText		= GetTxtFromMgr( 6197 );

	strText.Replace( TEXT("#fee#"), szFee );
	
	// 메시지 박스에 넘길 인수들
	stACUTRADE_RETMSG_ORDERTOSELL stSell;
	stSell.gmSellAcu	= gmSellAcu;
	stSell.gmSellUnit	= gmSellUnit;

	stMsgBox MsgBox;
	MsgBox.Set( this->GetParentDialog(), strTitle, strText, MSGBOX_TYPE_YESNO, ACUTRADE_RETMSG_ORDERTOSELL );
	pclclient->SetMsgBox( &MsgBox, (BYTE*)&stSell, sizeof(stSell) );

}

void CNAcuTradeDlg_Sell::OrderToCancel( void )
{
	cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
	if ( NULL == pclchar )
	{
		return;
	}
	
	// 실제 팔기주문을 했을때만 취소한다
	if ( (0 < pclchar->pclCI->clWorldMoney.siTradeWorldMoney) && (0 < pclchar->pclCI->clWorldMoney.siTradeWorldMoneyPrice) )
	{
		cltMsg clMsg( GAMEMSG_REQUEST_WORLDMONEY_CANCELORDER, 0, NULL );
		pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

	}

}
