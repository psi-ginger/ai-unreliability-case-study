#include "NRacoonDodgeEventReserveListDlg.h"
#include "Char\CharManager\CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

extern cltCommonLogic* pclClient;

NRacoonDodgeEventReserveListDlg::NRacoonDodgeEventReserveListDlg()
{	
	Initialize();
}

NRacoonDodgeEventReserveListDlg::~NRacoonDodgeEventReserveListDlg()
{
	Destroy();
}


void NRacoonDodgeEventReserveListDlg::Init()
{

}

void NRacoonDodgeEventReserveListDlg::Destroy()
{

}

void NRacoonDodgeEventReserveListDlg::Create()
{
	if ( IsCreate() )
		return;


	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NRacoonDodgeDlg/DLG_RacoonDodgeEventReserveList.ddf"));
	file.CreatePopupDialog( this, NRACOONDODGE_EVENT_RESERVELIST_DLG , TEXT("dialog_systemreward"), StaticRacoonDodgeEventReserveListDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( IMAGESTATIC, NRACOONDODGEEVENT_IMAGE_PORTRAIT , this ), NRACOONDODGEEVENT_IMAGE_PORTRAIT,   "imagestatic_portrait");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( STATIC, NRACOONDODGEEVENT_STATIC_EXPLAIN , this ), NRACOONDODGEEVENT_STATIC_EXPLAIN,   "statictext_explain");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NRACOONDODGEEVENT_LIST_RESERVELIST , this ), NRACOONDODGEEVENT_LIST_RESERVELIST,   "listbox_reservelist");
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NRACOONDODGEEVENT_BUTTON_EXIT , this ), NRACOONDODGEEVENT_BUTTON_EXIT,   "button_exit");

	CStatic* pStatic = m_InterfaceFactory.GetStatic( NRACOONDODGEEVENT_STATIC_EXPLAIN );
	if( pStatic )
	{
		// 보여줄 가격을 나타낸다
		SI32 npcRate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate	;

		SI32 siCost = (npcRate * 1000) / 100	;
		NTCHARString256 kBuffer(GetTxtFromMgr(40023));
		kBuffer.Replace("#cost#", SI32ToString(siCost));	

		pStatic->SetText( kBuffer );
	}

	
	CList* pList = m_InterfaceFactory.GetList(NRACOONDODGEEVENT_LIST_RESERVELIST );
	if ( pList )
	{
		SI16 siNumberWidth		= 30;
		if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_USA | ConstServiceArea_EUROPE) )
		{
			siNumberWidth		= 40;
		}

		SI16 siReasonWidth		= pList->GetWidth() - siNumberWidth;
		
		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr( 1641 );
		pList->SetColumn(0, siNumberWidth,		pText);
		pText = GetTxtFromMgr( 3469 );
		pList->SetColumn(1, siReasonWidth,		pText);
		pText = GetTxtFromMgr( 10183 );
		pList->Refresh();
	}

	SetReserveList();
	cltCharClient* pCharClient = pclClient->pclCM->GetCharClient(1)	;
	if(pCharClient!=NULL )
	{
		GetReserveList( pCharClient->pclCI->GetPersonID() );
	}
}

void CALLBACK NRacoonDodgeEventReserveListDlg::StaticRacoonDodgeEventReserveListDlgProc( CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{
	NRacoonDodgeEventReserveListDlg *pThis = (NRacoonDodgeEventReserveListDlg*) pControllerMgr;
	pThis->NRacoonDodgeEventReserveListDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK NRacoonDodgeEventReserveListDlg::NRacoonDodgeEventReserveListDlgProc( UINT nEvent, int nControlID, CControllerObj* pControl )
{
	cltClient *pclclient = (cltClient *)pclClient;

	switch ( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NRACOONDODGEEVENT_BUTTON_EXIT:
		{
			DeleteDialog();
		}break	;
	}
}

void NRacoonDodgeEventReserveListDlg::SetReserveList()
{
	CList* pList = m_InterfaceFactory.GetList( NRACOONDODGEEVENT_LIST_RESERVELIST );

	pList->Clear();

	SI16 siNumberWidth		= 30;
	if ( pclClient->IsWhereServiceArea(ConstServiceArea_English | ConstServiceArea_USA | ConstServiceArea_EUROPE) )
	{
		siNumberWidth		= 40;
	}
	SI16 siNameWidth		= pList->GetWidth() - siNumberWidth ;

	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText = GetTxtFromMgr(40042)	;
	pList->SetColumn(0, siNumberWidth,		pText);
	pText = GetTxtFromMgr(1556)	;
	pList->SetColumn(1, siNameWidth,		pText);
	pList->Refresh();
}
void NRacoonDodgeEventReserveListDlg::SetListData(cltGameMsgResponse_ReserveList * pMsg)
{
	CList* pList = m_InterfaceFactory.GetList( NRACOONDODGEEVENT_LIST_RESERVELIST );

	pList->Clear();

	stListBoxItemData tempItemData;

	for ( SI32 i=0; i<MAX_SYSTEMREWARD_LIST; ++i )
	{
		tempItemData.Init();

		tempItemData.Set( 0, SI32ToString(i+1) );
		tempItemData.Set( 1, pMsg->clInfo[i].GetCharName());
		
		pList->AddItem( &tempItemData );
	}

	pList->Refresh();
}
void NRacoonDodgeEventReserveListDlg::GetReserveList( SI32 siPersonID )
{
	cltGameMsgRequest_ReserveList clinfo( siPersonID );	// 일단 퍼슨아이디로 해놓는다
	cltMsg clMsg(GAMEMSG_REQUEST_RACOONDODGE_EVENT_GET_RESERVE_LIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
}