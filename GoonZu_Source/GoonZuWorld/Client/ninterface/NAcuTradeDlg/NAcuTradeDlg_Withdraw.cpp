#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../../Client.h"
#include "../../CommonLogic/Msg/MsgType-WorldMoney.h"

#include "NAcuTradeDlg_Withdraw.h"


CNAcuTradeDlg_Withdraw::CNAcuTradeDlg_Withdraw() : CDialogController()
{
	m_siSavedACU = 0;
	ZeroMemory( m_clAcuList, sizeof(m_clAcuList) );
}

CNAcuTradeDlg_Withdraw::~CNAcuTradeDlg_Withdraw()
{

}

void CNAcuTradeDlg_Withdraw::Create(CControllerMgr* pParent)
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	//-- 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NAcuTradeDlg/DLG_NAcuTrade_Withdraw.ddf" ) );
	file.CreateChildDialog( this, NACUTRADEDLG_WITHDRAW, TEXT( "dialog_acutrade_withdraw" ), StaticAcuTradeDlg_WithdrawProc, pParent );


	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_WITHDRAW_STATIC_NONAME1,		this ), NACUTRADE_WITHDRAW_STATIC_NONAME1,		TEXT( "statictext_NONAME1" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_WITHDRAW_STATIC_NONAME2,		this ), NACUTRADE_WITHDRAW_STATIC_NONAME2,		TEXT( "statictext_NONAME2" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,		NACUTRADE_WITHDRAW_STATIC_NONAME3,		this ), NACUTRADE_WITHDRAW_STATIC_NONAME3,		TEXT( "statictext_NONAME3" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_WITHDRAW_EDIT_SAVEACU,		this ), NACUTRADE_WITHDRAW_EDIT_SAVEACU,		TEXT( "editbox_saveacu" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( EDIT,		NACUTRADE_WITHDRAW_EDIT_WITHDRAWACU,	this ), NACUTRADE_WITHDRAW_EDIT_WITHDRAWACU,	TEXT( "editbox_withdrawacu" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,		NACUTRADE_WITHDRAW_BUTTON_WITHDRAW,		this ), NACUTRADE_WITHDRAW_BUTTON_WITHDRAW,		TEXT( "button_withdraw" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST,		NACUTRADE_WITHDRAW_LIST_TRADEACULIST,	this ), NACUTRADE_WITHDRAW_LIST_TRADEACULIST,	TEXT( "listbox_tradeaculist" ) );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( OUTLINE,	NACUTRADE_WITHDRAW_OUTLINE_NONAME1,		this ), NACUTRADE_WITHDRAW_OUTLINE_NONAME1,		TEXT( "outline_NONAME1" ) );

	//-- 컨트롤 설정
	// 리스트
	CList* pTradeAcuList = m_InterfaceFactory.GetList( NACUTRADE_WITHDRAW_LIST_TRADEACULIST );
	if ( pTradeAcuList )
	{
		SI16	siColumn0	= 120;
		SI16	siColumn2	= 50;
		SI16	siColumn1	= pTradeAcuList->GetWidth() - ( siColumn0 + siColumn2 );

		TCHAR*	pText		= GetTxtFromMgr( 1564 );
		pTradeAcuList->SetColumn( 0, siColumn0, pText );

		pText = GetTxtFromMgr( 8120 );
		pTradeAcuList->SetColumn( 1, siColumn1, pText );

		pText = GetTxtFromMgr( 6147 );
		pTradeAcuList->SetColumn( 2, siColumn2, pText );

		pTradeAcuList->SetBorder(TRUE);
		pTradeAcuList->SetBKColorUse(TRUE);
		pTradeAcuList->Refresh();
	}

	//-- 기본 초기 설정
	Request_SavedACU_List();

}

void CALLBACK CNAcuTradeDlg_Withdraw::StaticAcuTradeDlg_WithdrawProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNAcuTradeDlg_Withdraw *pThis = (CNAcuTradeDlg_Withdraw*) pControllerMgr;
	if ( pThis )
	{
		pThis->NAcuTradeDlg_WithdrawProc( nEvent, nControlID, pControl );
	}
}

void CALLBACK CNAcuTradeDlg_Withdraw::NAcuTradeDlg_WithdrawProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

	switch( nControlID )
	{
		case NACUTRADE_WITHDRAW_BUTTON_WITHDRAW:
			{
				OrderToWithdraw();
			}
			break;

	}

}

void CNAcuTradeDlg_Withdraw::Action()
{
}

void CNAcuTradeDlg_Withdraw::Show()
{
	CControllerMgr::Show( true );
}

void CNAcuTradeDlg_Withdraw::Hide()
{
	CControllerMgr::Show( false );
}

void CNAcuTradeDlg_Withdraw::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{

}

void CNAcuTradeDlg_Withdraw::SetMemberData( SI64 siSavedACU, cltTradeAcuList* pclAcuList/* =NULL  */)
{
	m_siSavedACU = siSavedACU;

	if ( pclAcuList )
	{
		memcpy( m_clAcuList, pclAcuList, sizeof(m_clAcuList) );
	}
}

void CNAcuTradeDlg_Withdraw::Refresh( void )
{
	CEdit* pclSaveAcu = m_InterfaceFactory.GetEdit( NACUTRADE_WITHDRAW_EDIT_SAVEACU );
	if ( NULL == pclSaveAcu )
	{
		return;
	}

	CList* pclAcuList = m_InterfaceFactory.GetList( NACUTRADE_WITHDRAW_LIST_TRADEACULIST );
	if ( NULL == pclAcuList )
	{
		return;
	}

	// 저장된 아큐
	pclSaveAcu->SetText( SI64ToString(m_siSavedACU) );

	// 거래내역
	stListBoxItemData stListItem;

	pclAcuList->Clear();

	for ( SI32 siCount=0; siCount<MAX_TRADESERVER_ACU_LIST; ++siCount )
	{
		if ( true == m_clAcuList[siCount].IsEmpty() )
		{
			continue;
		}
		
		// 날짜
		cltTradeDate* pclDate = &m_clAcuList[siCount].m_clDate;
		if ( NULL == pclDate )
		{
			continue;
		}

		TCHAR*	pszDate		= GetTxtFromMgr( 1644 );
		if ( NULL == pszDate )
		{
			continue;
		}
		TCHAR	szDate[128]	= { '\0', };

		StringCchPrintf( szDate, sizeof(szDate), pszDate, (pclDate->uiYear+2000), pclDate->uiMonth, pclDate->uiDay );
		
		// 아이템명
		TCHAR* pszItemName = (TCHAR*)pclClient->pclItemManager->GetName( m_clAcuList[siCount].m_siItemUnique );

		// 가격
		TCHAR szPrice[128] = { '\0', };
		g_JYLibFunction.SetNumUnit( m_clAcuList[siCount].m_siPriceACU, szPrice, sizeof(szPrice) );
		

		// 리스트 박스에 넣기
		stListItem.Init();

		stListItem.Set( 0, szDate );
		stListItem.Set( 1, pszItemName );
		stListItem.Set( 2, szPrice );

		pclAcuList->AddItem( &stListItem );

	}
	pclAcuList->Refresh();

}

void CNAcuTradeDlg_Withdraw::Request_SavedACU_List( void )
{
	cltMsg clMsg( GAMEMSG_REQUEST_TRADESERVER_GET_SAVEDACU_LIST, 0, NULL );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}

void CNAcuTradeDlg_Withdraw::OrderToWithdraw( void )
{
	// 저장된 아큐가 없다면
	if ( 0 >= m_siSavedACU )
	{
		return;
	}

	CEdit* pclWithdrawAcu = m_InterfaceFactory.GetEdit( NACUTRADE_WITHDRAW_EDIT_WITHDRAWACU );
	if ( NULL == pclWithdrawAcu )
	{
		return;
	}

	// 받으려는 아큐가 없다면
	GMONEY gmWithdrawAcu	= _tstoi64( pclWithdrawAcu->GetText() );
	if ( 0 >= gmWithdrawAcu )
	{
		return;
	}

	// 저장된 아큐보다 더 큰 아큐를 받으려 한다면
	if ( m_siSavedACU < gmWithdrawAcu )
	{
		return;
	}

	cltGameMsgRequest_TradeServer_Get_Withdraw_Acu clInfo( gmWithdrawAcu );
	cltMsg clMsg( GAMEMSG_REQUEST_TRADESERVER_GET_WITHDRAW_ACU, sizeof(clInfo), (BYTE*)&clInfo );
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);

}