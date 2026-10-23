#include "NNewMarketDlg.h"

#include "../StrBaseDlg/StrInfoDlg.h"
#include "../StrBaseDlg/StrPayTaxDlg.h"
#include "../StrBaseDlg/StrRepairDlg.h"
#include "../STrBaseDlg/StrResignChiefDlg.h"
#include "Char\CharManager\CharManager.h"

extern cltCommonLogic *pclClient;

NNewMarketDlg::NNewMarketDlg()
{

}

NNewMarketDlg::~NNewMarketDlg()
{
	cltClient * pclclient = (cltClient *)pclClient;
	if(pclclient->m_pDialog[NNEWMARKET_SUB_DLG] )
	{
		pclclient->m_pDialog[NNEWMARKET_SUB_DLG]->DeleteDialog();
	}

}

void NNewMarketDlg::Create()
{
	CreateDlg( NNEWMARKET_DLG, (cltStructureMngBasic*)pclClient->pclNewMarketManager, 2 );

	// 사용자 메뉴
	SetTabInfo( 1, 1 );
	TCHAR* pText = GetTxtFromMgr(3389);
	AddUserTabDialog( 0, pText, 80, new NNewMarketUserSellDlg,		TEXT("NInterface/Data/NStructureDlg/DLG_MarketUserSell.ddf"), TEXT("dialog_marketusersell") );
	pText = GetTxtFromMgr(2207);
	AddUserTabDialog( 1, pText, 80, new NNewMarketUserBuyDlg,		TEXT("NInterface/Data/NStructureDlg/DLG_MarketUserBuy.ddf"), TEXT("dialog_marketuserbuy") );
	pText = GetTxtFromMgr(3455);
	AddUserTabDialog( 2, pText, 80, new NStrInfoDlg,				TEXT("NInterface/Data/NStructureDlg/DLG_NStrInfoDlg.ddf"), TEXT("dialog_strinfo") );
	
	// 행수 메뉴
	SetTabInfo( 2, 1 );
	pText = GetTxtFromMgr(3442);
	AddChiefTabDialog( 0, pText, 80, new NNewMarketChiefDlg,	TEXT("NInterface/Data/NStructureDlg/DLG_MarketChief.ddf"), TEXT("dialog_marketchief") );
	pText = GetTxtFromMgr(3443);
	AddChiefTabDialog( 1, pText, 80, new NStrPayTaxDlg,			TEXT("NInterface/Data/NStructureDlg/DLG_NStrPayTaxDlg.ddf"), TEXT("dialog_strpaytax") );
	pText = GetTxtFromMgr(3444);
	AddChiefTabDialog( 2, pText, 80, new NStrRepairDlg,			TEXT("NInterface/Data/NStructureDlg/DLG_NStrRepair.ddf"), TEXT("dialog_strrepair") );
	
	SetTabInfo( 2, 2 );
	pText = GetTxtFromMgr(3445);
	AddChiefTabDialog( 3, pText, 80, new NStrResignChiefDlg,	TEXT("NInterface/Data/NStructureDlg/DLG_NStrResignChief.ddf"), TEXT("dialog_resignchief") );
 
	InitDlg();

	SetActionTimer( 200 );
	SetTimerOnOff( true );
}

void NNewMarketDlg::GreetingMsg()
{
	SetMainExplain(GetTxtFromMgr(9837));
	/*TCHAR* pText = GetTxtFromMgr(9837);
	TCHAR buffer[256] = {'\0'};
	if( pclClient->pclCM->CR[1]->pclCI->clBI.bNewMarketPremiumService )
	{
		TCHAR* text = GetTxtFromMgr(9838);	
		StringCchCopy( buffer, 256, pText );
		StringCchCat( buffer, 256, text );
	}
	else
	{
		TCHAR* text = GetTxtFromMgr(10168);
		StringCchCopy( buffer, 256, pText );
		StringCchCat( buffer, 256, text );
	}
	SetMainExplain(buffer);*/
}

void NNewMarketDlg::RetMsgBox( bool RetValue, SI16 MsgIndex, BYTE *pData, SI16 DataLen )
{
	switch( MsgIndex )
	{
	case 0:	// NMarketUserSellDlg 세금납부
		((NNewMarketUserSellDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;

	case 1:	// NMarketUserSellDlg 세금납부
		((NNewMarketUserSellDlg*)m_pUserChildDlg[ 0 ])->RetMsgBox( RetValue, MsgIndex, pData, DataLen );
		break;

	}
}

void NNewMarketDlg::DrawDialogLast( SI32 ScreenRX, SI32 ScreenRY  )
{
	((NNewMarketUserBuyDlg*)m_pUserChildDlg[ 1 ])->DrawDialogLast(ScreenRX, ScreenRY  );
}

void NNewMarketDlg::VDirectDrawToBackScreen( LPDIRECTDRAWSURFACE7 lpBackScreen, SI32 ScreenRX, SI32 ScreenRY )
{
	((NNewMarketUserBuyDlg*)m_pUserChildDlg[ 1 ])->VDirectDrawToBackScreen(lpBackScreen ,ScreenRX, ScreenRY  );
}

void NNewMarketDlg::NDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
 	cltClient * pclclient = (cltClient *)pclClient;

	switch( nControlID )
	{

	case COMMON_DIALOG_CLOSEBUTTON:
	case NSTRBASE_BUTTON_CLOSE:
		{
			switch( nEvent ) 
			{
				case EVENT_BUTTON_CLICKED:
					{
						if(pclclient->m_pDialog[NMARKET_SUB_DLG] )
						{
							pclclient->m_pDialog[NMARKET_SUB_DLG]->DeleteDialog();
						}

						DeleteDialog();
					}
					return;
			}			
		}
	}

	if( m_bChiefMode == false &&
		m_pUserTabButton[ 1 ] != NULL &&
		nControlID == m_pUserTabButton[ 1 ]->GetControllerID() &&
		nEvent == EVENT_BUTTON_CLICKED ) 
	{
			pclclient->CreateInterface(NNEWMARKET_SUB_DLG);
			
			if( pclclient->m_pDialog[NNEWMARKET_DLG]->GetX() + pclclient->m_pDialog[NNEWMARKET_DLG]->GetWidth() < 800 )
			{
				pclclient->m_pDialog[NNEWMARKET_SUB_DLG]->MovePos( 
				pclclient->m_pDialog[NNEWMARKET_DLG]->GetX() + pclclient->m_pDialog[NNEWMARKET_SUB_DLG]->GetWidth(), pclclient->m_pDialog[NNEWMARKET_DLG]->GetY() );			
			}
			else
			{
				pclclient->m_pDialog[NNEWMARKET_SUB_DLG]->MovePos( 
				pclclient->m_pDialog[NNEWMARKET_DLG]->GetX() - pclclient->m_pDialog[NNEWMARKET_SUB_DLG]->GetWidth(), pclclient->m_pDialog[NNEWMARKET_DLG]->GetY() );			

			}


			return;
	}

	NStrBaseDlg::NDlgProc(nEvent,nControlID,pControl);

}