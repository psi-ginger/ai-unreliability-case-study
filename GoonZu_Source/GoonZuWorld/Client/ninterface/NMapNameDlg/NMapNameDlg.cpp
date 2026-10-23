#include "NMapNameDlg.h"

#include "../../Client.h"
#include "../Client/Gametip/GameTip.h"

extern cltCommonLogic* pclClient;

CNMapNameDlg::CNMapNameDlg() : CDialogController()
{
	m_pstaticMapName		= NULL;
	m_pstaticMapNameShadow	= NULL;
	m_pGameTip				= NULL;
	m_pGameTipShadow		= NULL;
	
	m_siEndValue			= 0;
}

CNMapNameDlg::~CNMapNameDlg()
{
	SAFE_DELETE( m_pstaticMapName );
	SAFE_DELETE( m_pstaticMapNameShadow );
	SAFE_DELETE( m_pGameTip );
	SAFE_DELETE( m_pGameTipShadow );
	
}

void CNMapNameDlg::Create()
{
	//-- 이미 생성되었다면 리턴
	if ( IsCreate() == true )
	{
		DeleteDialog();
		return;
	}

	cltClient *pclclient = (cltClient *)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}
  
	SI16 siStaticWitdh	= 700;
	SI16 siStaticHeight	= 100;

	SI16 siDialogXpos	= (pclclient->GetScreenXSize() - siStaticWitdh + MAP_TILE_XSIZE) / 2;
	SI16 siDialogYpos	= 70;
	//-- 각종 객체들을 생성

	// 다이얼로그 생성
	CDialogController::Create( NMAPNAME_DLG, CONTROLLER_KIND_POPUPDIALOG, NULL, StaticCallBackDialogNMapName, NULL, NULL, 0, false, 0, siDialogXpos, siDialogYpos, 0, 0, false, false, false );
	SetDrawOption( SPR_PUTSPRALPHAT );


	SI16 siFontSize		= 50;
	SI16 siTipFontSize	= 14;
	SI32 siOption	= DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	if( pclClient->IsWhereServiceArea( ConstServiceArea_Korea ) )
	{
		siTipFontSize = 13;
	}

	NEWCSTATIC( m_pstaticMapName );
	m_pstaticMapName->Create( 0, NULL, NULL, 0, false, 0, 0, 0, siStaticWitdh, siStaticHeight, siStaticWitdh, siStaticHeight);
	m_pstaticMapName->SetFontSize( siFontSize );
	m_pstaticMapName->SetTextPrintOption( siOption );
	m_pstaticMapName->SetFontColor( RGB( 0xFF, 0xFF, 0xFF ) );


	NEWCSTATIC( m_pstaticMapNameShadow );
	m_pstaticMapNameShadow->Create( 0, NULL, NULL, 0, false, 0, 1, 1, siStaticWitdh, siStaticHeight, siStaticWitdh, siStaticHeight);
	m_pstaticMapNameShadow->SetFontSize( siFontSize );
	m_pstaticMapNameShadow->SetTextPrintOption( siOption );
	m_pstaticMapNameShadow->SetFontColor( RGB( 0x30, 0x30, 0x30 ) );
	
	NEWCEDIT( m_pGameTip );
	m_pGameTip->Create( 0, NULL, NULL, 0, false, 0, 0, 80, siStaticWitdh, siStaticHeight, siStaticWitdh, siStaticHeight ); 
	m_pGameTip->SetFontSize( siTipFontSize );
	m_pGameTip->SetTextPrintOption( siOption ); 
	m_pGameTip->SetFontColor( RGB( 0xFF, 0xFF, 0xFF ) );
	m_pGameTip->SetFontWeight( FW_BOLD );
	m_pGameTip->SetWidth(1);
	m_pGameTip->SetHeight(1);
  
	NEWCEDIT( m_pGameTipShadow ); 
	m_pGameTipShadow->Create( 0, NULL, NULL, 0, false, 0, 1, 81, siStaticWitdh, siStaticHeight, siStaticWitdh, siStaticHeight );
	m_pGameTipShadow->SetFontSize( siTipFontSize ); 
	m_pGameTipShadow->SetTextPrintOption( siOption );
	m_pGameTipShadow->SetFontColor( RGB( 0x30, 0x30, 0x30 ) );
	m_pGameTipShadow->SetFontWeight( FW_BOLD );
	m_pGameTipShadow->SetWidth(1);
	m_pGameTipShadow->SetHeight(1);
	
		
	SetTopMostWindow( true );
	SetActionTimer( 300 );    
	SetTimerOnOff( true );

}

void CNMapNameDlg::Action(void)
{
	m_siEndValue++;

	if ( CMAPNAME_VARIABLE_ENDTIME < m_siEndValue )
	{
		DeleteDialog();
		return;
	}

}

void CALLBACK CNMapNameDlg::StaticCallBackDialogNMapName( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj* pControl )
{
	CNMapNameDlg *pThis = (CNMapNameDlg*)pControllerMgr;
	pThis->CallBackDialogNMapName( nEvent, nControlID, pControl );
	return;

}

void CALLBACK CNMapNameDlg::CallBackDialogNMapName( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	return;

}

void CNMapNameDlg::ShowMapName( SI32 siMapIndex )
{
	cltClient* pclclient = (cltClient*)pclClient;
	if ( NULL == pclclient )
	{
		return;
	}
	
	TCHAR* szMapName = pclclient->pclMapManager->GetMapName( siMapIndex );
	if ( NULL == szMapName )
	{
		return;
	}

	m_siEndValue = 0;

	m_pstaticMapNameShadow->SetText( szMapName );
	m_pstaticMapName->SetText( szMapName );

	if(pclClient->IsCountrySwitch(Switch_GameTip))
	{
		TCHAR* pGameTipText = pclclient->m_pclGameTip->RandTipText(); 
		if( NULL == pGameTipText ) return;

		m_pGameTipShadow->SetText(pGameTipText);
		m_pGameTip->SetText(pGameTipText);
	}
}