#include "NPannel_DateMoneyDlg.h"
#include "../../InterfaceMgr/InterfaceMgr.h"


CNPannel_DateMoneyDlg::CNPannel_DateMoneyDlg() : CDialogController()
{

}


CNPannel_DateMoneyDlg::~CNPannel_DateMoneyDlg()
{

}


void CNPannel_DateMoneyDlg::Create()
{
	// 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		return;
	}

	// 각종 객체들을 생성
	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPannelDlg/DLG_Pannel_DateMoney.ddf" ) );
	file.CreatePopupDialog( this, PANNEL_DATEMONEY_DLG, TEXT( "dialog_datemoney" ), StaticPannel_DateMoneyDlgProc );
	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_DATEMONEY_DATE,	this ), PANNEL_DATEMONEY_DATE,	TEXT( "statictext_date" ) );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,	PANNEL_DATEMONEY_MONEY,	this ), PANNEL_DATEMONEY_MONEY,	TEXT( "statictext_money" ) );	

	// 텍스트 설정
	CStatic* pStaticDate	= (CStatic*)m_InterfaceFactory.GetStatic(PANNEL_DATEMONEY_DATE);
	CStatic* pStaticMoney	= (CStatic*)m_InterfaceFactory.GetStatic(PANNEL_DATEMONEY_MONEY);

	if ( pStaticDate && pStaticMoney )
	{
		pStaticDate->SetFontColor( COLOR_DIALOG_WHITE );
		pStaticDate->SetFontSize(11);

		pStaticMoney->SetFontColor( COLOR_DIALOG_WHITE );
		pStaticMoney->SetFontSize(11);
	}
}

void CALLBACK CNPannel_DateMoneyDlg::StaticPannel_DateMoneyDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	CNPannel_DateMoneyDlg *pThis = (CNPannel_DateMoneyDlg*) pControllerMgr;
	pThis->NPannel_DateMoneyDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CNPannel_DateMoneyDlg::NPannel_DateMoneyDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{

}

void  CNPannel_DateMoneyDlg::SetDateMoney( TCHAR* pszDate, TCHAR* pszMoney )
{
	if ( pszDate != NULL )
	{
		CStatic* pStatic = m_InterfaceFactory.GetStatic(PANNEL_DATEMONEY_DATE);
		if ( pStatic != NULL )
		{
			pStatic->SetText( pszDate );
		}
	}

	if ( pszMoney != NULL )
	{
		CStatic* pStatic = m_InterfaceFactory.GetStatic(PANNEL_DATEMONEY_MONEY);
		if ( pStatic != NULL )
		{
			pStatic->SetText( pszMoney );
		}
	}
}

void CNPannel_DateMoneyDlg::Show()
{
	CDialogController::Show( true );
}

void CNPannel_DateMoneyDlg::Hide()
{
	CDialogController::Show( false );
}