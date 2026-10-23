#include "../Client/ninterface/ncharstatus/NCharAbilityDatailDlg.h"


NCharAbilityDatailDlg::NCharAbilityDatailDlg()
{
	
}

NCharAbilityDatailDlg::~NCharAbilityDatailDlg()
{

}

void NCharAbilityDatailDlg::Create()
{
	if ( IsCreate() )
		return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NCharStatusdlg/DLG_NCharStatus_ability_detail.ddf" ) );
	file.CreatePopupDialog( this, CHARSTATUS_ABILITY_DETAIL_DLG, TEXT( "DLG_NCharStatus_ability_detail" ), StaticNCharAbilityDatailDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  CHARSTATUS_ABILITY_DETAIL_STATIC_EXPLAIN,		this ), CHARSTATUS_ABILITY_DETAIL_STATIC_EXPLAIN,		TEXT( "statictext_expalin" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  CHARSTATUS_ABILITY_DETAIL_STATIC_DETAIL_INFO1,	this ), CHARSTATUS_ABILITY_DETAIL_STATIC_DETAIL_INFO1,	TEXT( "statictext_info1" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC,  CHARSTATUS_ABILITY_DETAIL_STATIC_DETAIL_INFO2,	this ), CHARSTATUS_ABILITY_DETAIL_STATIC_DETAIL_INFO2,	TEXT( "statictext_info2" ) );	
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,  CHARSTATUS_ABILITY_DETAIL_BUTTON_OK,			this ), CHARSTATUS_ABILITY_DETAIL_BUTTON_OK,			TEXT( "button_ok" ) );	
	
	SetExplain();
	SetAbilityInfo();
}

void CALLBACK NCharAbilityDatailDlg::StaticNCharAbilityDatailDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NCharAbilityDatailDlg *pThis = (NCharAbilityDatailDlg*) pControllerMgr;
	pThis->NCharAbilityDatailDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NCharAbilityDatailDlg::NCharAbilityDatailDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{
	switch ( nControlID )
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{

				DeleteDialog();
			}
			break;
		case CHARSTATUS_ABILITY_DETAIL_BUTTON_OK:
			{
				DeleteDialog();
			}
			break;	
	}
}

void NCharAbilityDatailDlg::SetExplain( void )
{
	CStatic* pEdit = NULL; 

	pEdit = m_InterfaceFactory.GetStatic( CHARSTATUS_ABILITY_DETAIL_STATIC_EXPLAIN );

	pEdit->SetText(GetTxtFromMgr(8188)); 
} 

void NCharAbilityDatailDlg::SetAbilityInfo( void ) 
{
	cltCharClient*	pclchar		= (cltCharClient*)pclClient->pclCM->CR[1];
	TCHAR			buffer[256] = "";
	CStatic*		pEdit		= NULL; 
	SI16            siTotal     = 0;
	SI32			PointPos	= 0;
	 
	pEdit = m_InterfaceFactory.GetStatic( CHARSTATUS_ABILITY_DETAIL_STATIC_DETAIL_INFO1 );

	siTotal = pclchar->clPB.GetClientDefensiveRate(false);
	StringCchPrintf(buffer, sizeof(buffer), "\r %s %d.%d%%\r\r ",			GetTxtFromMgr(8406), siTotal/100, siTotal%100);
	
	PointPos = strlen(buffer);
	siTotal = pclchar->clPB.GetClientHitRate(false);
	StringCchPrintf(&buffer[PointPos], sizeof(buffer), "%s %d.%d%%\r\r ",	GetTxtFromMgr(8408), siTotal/100, siTotal%100);
	
	PointPos = strlen(buffer);
	siTotal = pclchar->clPB.GetClientDodgeRate(false);
	StringCchPrintf(&buffer[PointPos], sizeof(buffer), "%s %d.%d%%\r ",		GetTxtFromMgr(8410), siTotal/100, siTotal%100);
	
	pEdit->SetText(buffer);			

	PointPos = 0;
	pEdit = m_InterfaceFactory.GetStatic( CHARSTATUS_ABILITY_DETAIL_STATIC_DETAIL_INFO2 );

	siTotal = pclchar->clPB.GetClientDefensiveRate(true);
	StringCchPrintf(buffer, sizeof(buffer), "\r %s %d.%d%%\r\r ",			GetTxtFromMgr(8407), siTotal/100, siTotal%100);

	PointPos = strlen(buffer);
	siTotal = pclchar->clPB.GetClientHitRate(true);
	StringCchPrintf(&buffer[PointPos], sizeof(buffer), "%s %d.%d%%\r\r ",	GetTxtFromMgr(8409), siTotal/100, siTotal%100);
 
	PointPos = strlen(buffer);
	siTotal = pclchar->clPB.GetClientDodgeRate(true);
	StringCchPrintf(&buffer[PointPos], sizeof(buffer), "%s %d.%d%%\r ",		GetTxtFromMgr(8411), siTotal/100, siTotal%100);
	 
	pEdit->SetText(buffer);			  
}

