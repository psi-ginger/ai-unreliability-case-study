#include "NBokBulBokEventReserveListDlg.h"

#include "Char/CharManager/CharManager.h"
#include "../../InterfaceMgr/InterfaceFile.h"
#include "../../CommonLogic/CommonLogic.h"
#include "../../Client/Client.h"

#include "../Client/Client_BokBulBokMgr/Client_BBBMgr.h"






extern cltCommonLogic* pclClient;

CBokBulBokEventReserveListDlg::CBokBulBokEventReserveListDlg()
{
}

CBokBulBokEventReserveListDlg::~CBokBulBokEventReserveListDlg()
{
	Destroy();
}

void CBokBulBokEventReserveListDlg::init()
{
	TCHAR szPortrait[256];
	SI32  siPortraitFont;
	pclClient->pclKindInfoSet->GetNPCImageFileAndFont(pclClient->GetUniqueFromHash(TEXT("KIND_BOKBULBOKNPC")), szPortrait, sizeof(szPortrait), &siPortraitFont);

}

void CBokBulBokEventReserveListDlg::Destroy()
{
}

void CBokBulBokEventReserveListDlg::Create()
{
	// 이미 생성되었다면 리턴
	if( IsCreate() == true )
	{
		DeleteDialog();
		return;  
	}

	CInterfaceFile file;

	file.LoadFile(TEXT("NInterface/Data/NBokBulBokDlg/DLG_BokBulBok_ReserveList2.ddf"));
	file.CreatePopupDialog( this, NBOKBULBOK_RESERVELIST_DLG, TEXT("dialog_reserve_list"), StaticBokBulBokEventReserveListDlgProc );

	file.CreateControl( m_InterfaceFactory.CreateUserInterface( LIST, NBOKBULBOK_RESERVELIST_LIST,				this), NBOKBULBOK_RESERVELIST_LIST,				TEXT("listbox_reserve") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface( BUTTON, NBOKBULBOK_RESERVELIST_BUTTON_APPLY,	this), NBOKBULBOK_RESERVELIST_BUTTON_APPLY,		TEXT("button_apply_reserve") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(	BUTTON, NBOKBULBOK_RESERVELIST_BUTTON_CANCEL,	this), NBOKBULBOK_RESERVELIST_BUTTON_CANCEL,	TEXT("button_cancel_reserve") );
	file.CreateControl( m_InterfaceFactory.CreateUserInterface(	BUTTON, NBOKBULBOK_RESERVELIST_BUTTON_CLOSE,	this), NBOKBULBOK_RESERVELIST_BUTTON_CLOSE,		TEXT("button_close_reserve") );

	CList* pList = m_InterfaceFactory.GetList(NBOKBULBOK_RESERVELIST_LIST);
	if( pList )
	{
		SI16 siNumberWidth		= 40;
		SI16 siReasonWidth		= pList->GetWidth()	- siNumberWidth;

		pList->SetBorder(true);
		pList->SetBKColorUse(true);

		TCHAR* pText = GetTxtFromMgr(1641);	// 번호
		pList->SetColumn(0, siNumberWidth, pText);
		pText = GetTxtFromMgr(1556);		// 이름
		pList->SetColumn(1, siReasonWidth, pText);
	}

	/*GetWaitList((_BBBWaitUserData*) pWaitList);
	cltCharClient* pCharClient = pclClient->pclCM->GetCharClient(1)	;
	if(pCharClient!=NULL )
	{
		GetReserveList( pCharClient->pclCI->GetPersonID() );
	}*/
}

void CALLBACK CBokBulBokEventReserveListDlg::StaticBokBulBokEventReserveListDlgProc(CControllerMgr *pControllerMgr, UINT nEvent, int nControlID, CControllerObj *pControl)
{	
	CBokBulBokEventReserveListDlg *pThis = ( CBokBulBokEventReserveListDlg* )pControllerMgr;
	pThis->NBokBulBokEventReserveListDlgProc( nEvent, nControlID, pControl );
}

void CALLBACK CBokBulBokEventReserveListDlg::NBokBulBokEventReserveListDlgProc(UINT nEvent, int nControlID, CControllerObj* pControl)
{	
	cltClient* pClient = (cltClient*) pclClient;

	if( pClient == NULL ) return;

	switch( nControlID )
	{
	case COMMON_DIALOG_CLOSEBUTTON:
		{
			DeleteDialog();
		}
		break;
	case NBOKBULBOK_RESERVELIST_BUTTON_APPLY:		// 대기자 신청 시
		{
			if ( false == pClient->m_pClient_BBBMgr->IsReserved() )		// 대기 리스트에 등록이 실패 했을때에만 서버에 대기리스트 등록 요청 패킷 보냄
			{
				cltCharClient* pclCharClient = pClient->pclCM->GetCharClient(1)	;
				if( pclCharClient != NULL  )
				{
					// 대기자 예약 하겠다고 메세지 서버로 요청
					cltGameMsgRequest_BBB_WaitList_Answer clinfo( pclCharClient->pclCI->GetPersonID() );		// 서버에 대기리스트 등록 요청 보냄
					cltMsg clMsg(GAMEMSG_REQUEST_BBB_WAITLIST_ANSWER, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
				}

				//DeleteDialog();
			}
			else
			{
				// 내가 이미 대기자 리스트에 등록이 되어 있는 상황일땐 패킷 보내지 않고 등록 되있다는 메세지창 띄워줌
				
				cltClient* pClient = (cltClient*)pclClient;

				TCHAR*	pTitle	= GetTxtFromMgr(40065); // 복불복 게임
				TCHAR*	pBody	= GetTxtFromMgr(40109); // 어머~! 이미 한번 예약 하셨네요. 경기 후에 다시 예약해 주세요.
				pClient->m_pDialog[NOTIFYMSG_DLG]->SetText(pTitle, pBody);
			}
		}
		break;
	case NBOKBULBOK_RESERVELIST_BUTTON_CANCEL:		// 대기자 취소 시
		{
			if ( true == pClient->m_pClient_BBBMgr->IsReserved() )		// 내가 대기 리스트에 등록 되어 있는 경우에만 취소 패킷 보냄
			{
				pClient->m_pClient_BBBMgr->SetReserveStatus( false );	// 일단 클라이언트쪽의 대기자 등록 상황을 false로 바꾸고 서버에도 취소 패킷 보냄.
				
				cltCharClient* pclCharClient = pClient->pclCM->GetCharClient(1)	;
				// 서버에 대기자 등록한걸 취소 하라고 패킷 보냄
				cltGameMsgRequest_BBB_WaitList_Cancel clinfo( pclCharClient->pclCI->GetPersonID() );
				cltMsg clMsg( GAMEMSG_REQUEST_BBB_WAITLIST_CANCEL, sizeof(clinfo),(BYTE*)&clinfo);
				pclClient->SendMsgToServer((sPacketHeader*)&clMsg);
			}
			//DeleteDialog();
		}
		break;
	case NBOKBULBOK_RESERVELIST_BUTTON_CLOSE:
		{
			DeleteDialog();
		}
		break;
	}
}

// 리스트에 넣을 데이터를 요청한다.
void CBokBulBokEventReserveListDlg::GetReserveList( SI32 siPersonID )
{	
	/*CList* pList = m_InterfaceFactory.GetList( NBOKBULBOKEVENT_RESERVE_LIST );

	pList->Clear();

	stListBoxItemData tempItemData;

	for ( SI32 i=0; i<MAX_SYSTEMREWARD_LIST; ++i )
	{
		tempItemData.Init();

		tempItemData.Set( 0, SI32ToString(i+1) );
		tempItemData.Set( 1, pMsg->clInfo[i].GetCharName());

		pList->AddItem( &tempItemData );
	}

	pList->Refresh();*/
}

// 리스트에 데이터 입력
void CBokBulBokEventReserveListDlg::SetReserveList()
{
	/*
	cltGameMsgRequest_ReserveList clinfo( siPersonID );
	cltMsg clMsg(GAMEMSG_REQUEST_NBOKBULBOK_EVENT_GET_RESERVE_LIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclClient->SendMsgToServer((sPacketHeader*)&clMsg);*/
}

// 대기자 신청 버튼 눌렀을 때 호출되는 부분 
void CBokBulBokEventReserveListDlg::SetListData(TCHAR** ppWaitList)
{
	CList* pList = m_InterfaceFactory.GetList( NBOKBULBOK_RESERVELIST_LIST );

	TCHAR WaitList[MAX_RESERVATION_NUM][MAX_CHARACTER_NAME_LENGTH];
	ZeroMemory( WaitList, sizeof(TCHAR) * MAX_RESERVATION_NUM * MAX_CHARACTER_NAME_LENGTH );

	memcpy( WaitList, ppWaitList, sizeof(TCHAR) * MAX_RESERVATION_NUM * MAX_CHARACTER_NAME_LENGTH );

	pList->Clear();

	stListBoxItemData tempItemData;

	for ( SI32 i=0; i< MAX_RESERVATION_NUM; ++i )
	{
		tempItemData.Init();

		tempItemData.Set( 0, SI32ToString(i+1) );
		tempItemData.Set( 1, WaitList[i] );

		pList->AddItem( &tempItemData );
	}

	pList->Refresh();
}