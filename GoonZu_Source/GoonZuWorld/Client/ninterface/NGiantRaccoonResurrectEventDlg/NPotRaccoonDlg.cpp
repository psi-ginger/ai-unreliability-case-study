#include "NPotRaccoonDlg.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "../Client/ninterface/nsoundonoffdlg/NSoundOnOffDlg.h"
#include "../common/Char/CharManager/CharManager.h"
#include "../../CommonLogic/MsgType-Person.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/MsgType-Item.h"
#include "../server/Event/GiantRaccoon_Resurrect_Event/GiantRaccoonResurrectEvent.h"


NPotRaccoonDlg::NPotRaccoonDlg()
{

}


NPotRaccoonDlg::~NPotRaccoonDlg()
{
}

void NPotRaccoonDlg::Create()
{
	if( IsCreate() )	return;

	CInterfaceFile file;
	file.LoadFile( TEXT( "NInterface/Data/NPotRaccoonDlg/NPotRaccoonDlg.ddf" ) );

	SI32 controlindex = file.GetControlIndex( TEXT("dialog_NPotRaccoonDlg") ); 
	file.m_control[ controlindex ].dialog.bClosable = false;

	file.CreatePopupDialog( this, NPOTRACCOONDLG, TEXT( "dialog_NPotRaccoonDlg" ), StaticNPotRaccoonDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,	NPOTRACCOONDLG_BUTTON_CONTRIBUTION,	this), NPOTRACCOONDLG_BUTTON_CONTRIBUTION,	TEXT("button_Contribution") );
	m_InterfaceFactory.GetButton( NPOTRACCOONDLG_BUTTON_CONTRIBUTION )->SetFocus( true );
	
	SI32 siID = pclClient->pclCM->GetIDFromCharKind( pclClient->GetUniqueFromHash(TEXT("KIND_POTRACCOON")) );
	cltCharClient* pclChar = pclClient->pclCM->GetCharClient( siID );
	if( NULL == pclChar ) return;

	SetX( pclChar->DrawStartX + 200 );
	SetY( pclChar->DrawStartY + 120 );
} 

void NPotRaccoonDlg::ClearFocus()
{
	CDialogController::ClearFocus();
	DeleteDialog();

	return;
}

void CALLBACK NPotRaccoonDlg::StaticNPotRaccoonDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl )
{
	NPotRaccoonDlg *pThis = (NPotRaccoonDlg *)pControllerMgr;
	pThis->NPotRaccoonDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NPotRaccoonDlg::NPotRaccoonDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient * pclclient = (cltClient*) pclClient;

	switch( nControlID )
	{
	case NPOTRACCOONDLG_BUTTON_CONTRIBUTION:
		{
			switch( nEvent )
			{
			case EVENT_BUTTON_CLICKED:
				{
					// 마을이 없다면 리턴.
					cltCharClient* pclchar = pclClient->pclCM->GetCharClient( 1 );
					if( NULL == pclchar ) return;
					
					
					if( cltGiantRaccoonResurrectEvent::POTRACCOON_CONTRIBUTION_ITEM_NUM > pclchar->pclCI->clCharItem.GetItemNum( ITEMUNIQUE(13069) ) )	
					{
						pclclient->m_pDialog[ NOTIFYMSG_DLG ]->SetText( GetTxtFromMgr(5614), GetTxtFromMgr(40813) );
						DeleteDialog(); 
						return;
					}
										
					cltMsg clMsg( GAMEMSG_REQUEST_NPOTRACCOON_ITEM_GET, PACKET_EMPTY );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );		

					DeleteDialog(); 
				}
				break;
			}
		}
		break;

	default:	break;
	}

}

