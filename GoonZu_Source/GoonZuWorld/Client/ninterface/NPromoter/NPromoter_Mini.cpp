#include "NPromoter_Mini.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../common/Quest/Quest.h"

NPromoter_Mini::NPromoter_Mini()
{
}

NPromoter_Mini::~NPromoter_Mini()
{
}

void NPromoter_Mini::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;

    file.LoadFile("Ninterface/Data/NPromoterDlg/NPromoter_Mini.ddf");

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_promoter_mini") );
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this , NPROMOTER_MINI, "dialog_promoter_mini" , StatiNPromoter_MiniProc );

	if( g_bFullScreen)	MovePos( 1040, 223 );	 
	else		 		MovePos( 1000, 213 );  

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON,	NMYKEYMINI_BUTTON_PROMOTER_INFO, this), NMYKEYMINI_BUTTON_PROMOTER_INFO,  "button_promoter_info");

	SetActionTimer( 1000 );
	SetTimerOnOff( true );

	m_dwDeleteDialogTickCount = GetTickCount();
}

void NPromoter_Mini::Action()
{
	if( GetTickCount() - m_dwDeleteDialogTickCount > 60000 )
		DeleteDialog(); 
}

void CALLBACK NPromoter_Mini::StatiNPromoter_MiniProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NPromoter_Mini *pThis = (NPromoter_Mini *)pControllerMgr;
	pThis->Promoter_MiniProc( nEvent, nControlID, pControl );
}
void CALLBACK NPromoter_Mini::Promoter_MiniProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case NMYKEYMINI_BUTTON_PROMOTER_INFO:
			{
				if ( pclclient->m_pDialog[NPROMOTER_INFO] == NULL )
				{							 
					pclclient->CreateInterface( NPROMOTER_INFO );
				}
				else
				{
					pclclient->DestroyInterface( pclclient->m_pDialog[NPROMOTER_INFO] );
				}
			}
			break;
		default:
			{
			}
			break;
	}
}


