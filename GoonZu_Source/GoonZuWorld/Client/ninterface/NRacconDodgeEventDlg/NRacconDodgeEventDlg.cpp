//Switch_10MonthEvent_RacoonDodge
#include "NRacconDodgeEventDlg.h"

#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../InterfaceMgr/InterfaceMgr.h"
#include "Char/CharManager/CharManager.h"

#include "../../CommonLogic/Msg/MsgType-RacoonDodgeEvent.h"

NRacconDldgeEventDlg::NRacconDldgeEventDlg()
{
}

NRacconDldgeEventDlg::~NRacconDldgeEventDlg()
{
	Destroy();
}

void NRacconDldgeEventDlg::Init()
{
}

void NRacconDldgeEventDlg::Destroy()
{ 


}

void NRacconDldgeEventDlg::Create()
{
	if( IsCreate() )
	{
        return;
	}

	CInterfaceFile file;

	file.LoadFile("Ninterface/Data/NRacoonDodgeDlg/DLG_NRacoonDodge_ReserveDlg.ddf");

	file.CreatePopupDialog( this, NRACOONDODGE_EVENT_DLG, "dialog_reserve" , StatiNRacconDldgeEventDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface(IMAGESTATIC,  	NRACOONDODGEEVENT_NPC_STATIC_PORTRAIT_IMAGE,	this),  NRACOONDODGEEVENT_NPC_STATIC_PORTRAIT_IMAGE,			TEXT("imagestatic_NONAME2")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(EDIT,  		NRACOONDODGEEVENT_EDIT_TALK,	this),  NRACOONDODGEEVENT_EDIT_TALK,			TEXT("editbox_npcsay")	);
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(BUTTON,  	NRACOONDODGEEVENT_STATIC_RESERVE_BUTTON,			this),  NRACOONDODGEEVENT_STATIC_RESERVE_BUTTON,					TEXT("button_reserve")				);

	CButton* pButton = m_InterfaceFactory.GetButton( NRACOONDODGEEVENT_STATIC_RESERVE_BUTTON )	;
	CEdit* pEdit = m_InterfaceFactory.GetEdit( NRACOONDODGEEVENT_EDIT_TALK )	;

	// 텍스트 셋팅
	pButton->SetText( GetTxtFromMgr(40022) )	;
	
	// 보여줄 가격을 나타낸다
	SI32 npcRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate	;
	
	// 화폐단위 콤마표기 추가
	SI64 siCost = (npcRate * 1000) / 100	;
	
	TCHAR szFee[128] = { '\0', };
	g_JYLibFunction.SetNumUnit( siCost , szFee, 128 );

	NTCHARString256 kBuffer(GetTxtFromMgr(40021));
	kBuffer.Replace("#cost#", szFee);
	pEdit->SetText( kBuffer )	;
	
	Init();

	SetX( 450 ); 
	SetY( 300 );
}

void NRacconDldgeEventDlg::Action()
{

}

void CALLBACK NRacconDldgeEventDlg::StatiNRacconDldgeEventDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NRacconDldgeEventDlg *pThis = (NRacconDldgeEventDlg *)pControllerMgr;
	pThis->NRacconDldgeEventDlgProc( nEvent, nControlID, pControl );
}
void CALLBACK NRacconDldgeEventDlg::NRacconDldgeEventDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{ 
	cltClient * pclclient = (cltClient*) pclClient;

	switch(nControlID)
	{
		case COMMON_DIALOG_CLOSEBUTTON:
			{
				DeleteDialog();
			}
			break;
		case NRACOONDODGEEVENT_STATIC_RESERVE_BUTTON:
			{
				switch( nEvent )
				{
				case EVENT_BUTTON_CLICKED:
					{
						cltCharClient* pclCharClient = pclClient->pclCM->GetCharClient(1)	;
						if( pclCharClient != NULL  )
						{
							//예약메세지를 날린다.
							cltGameMsgRequest_ReservationEvent clinfo( pclCharClient->pclCI->GetPersonID() );	// 일단 퍼슨아이디로 해놓는다
							cltMsg clMsg(GAMEMSG_REQUEST_RACOONDODGE_EVENT_RESERVATION, sizeof(clinfo), (BYTE*)&clinfo);
							pclClient->SendMsgToServer((sPacketHeader*)&clMsg);							
						}
						DeleteDialog();

					}
				}
			}
	
		default:
			{

			}
			break;
	}
}