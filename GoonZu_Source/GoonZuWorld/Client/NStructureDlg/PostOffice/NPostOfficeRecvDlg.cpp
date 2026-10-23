#include "NPostOfficeDlg.h"

#include "../../InterfaceMgr/Interface/List.h"
#include "../../InterfaceMgr/Interface/Button.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

#include "NInterface/NNewInventory/NNewInventory.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;


NPostOfficeRecvDlg::NPostOfficeRecvDlg()
{
}

NPostOfficeRecvDlg::~NPostOfficeRecvDlg()
{
	
}

void NPostOfficeRecvDlg::InitDlg()
{
	
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	SetControlMap( LIST_RECVLIST, TEXT("listbox_recvlist") );
	SetControlMap( EDIT_MSG, TEXT("editbox_msg") );

	SetControlMap( BUTTON_RECV, TEXT("button_recv") );
	SetControlMap( BUTTON_RECVOK, TEXT("button_recvok") );
	if( pclClient->IsCountrySwitch(Switch_reply)){//답장하기 [2007.08.17 손성웅]
		SetControlMap( BUTTON_REPLY, TEXT("button_reply"));
	}

	CList *pList = (CList*)m_pControlMap[ LIST_RECVLIST ];

	pList->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	TCHAR* pText[3];

	pText[0] = GetTxtFromMgr(1717);
	pText[1] = GetTxtFromMgr(1718);
	pText[2] = GetTxtFromMgr(1565);

	pList->SetColumn( 0, 92, pText[ 0 ] );
	pList->SetColumn( 1, 100, pText[ 1 ] );
	pList->SetColumn( 2, 144, pText[ 2 ] );

	pList->Refresh();


	pPostOfficeManager->m_siCurMsgType = 0;
	pPostOfficeManager->m_siCurMsgID = 0;

	pPostOfficeManager->m_bRefreshList = FALSE;

	//KHY - 0621 - test중.
	//SetEditText( EDIT_MSG, TEXT(" ") );
	

	this->NDlgTabProc(NULL,m_nControlID[BUTTON_RECV],NULL);
}

void NPostOfficeRecvDlg::Action()
{
	int i, j;
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	CList *pList = (CList*)m_pControlMap[ LIST_RECVLIST ];
	stListBoxItemData itemdata;

	TCHAR szDate[ 256 ] = TEXT("");
	TCHAR buffer[ 256 ] = TEXT("");

	// 리스트에 아이템 추가
	if( (pPostOfficeManager->m_bAddItemToRecvList &&
		pPostOfficeManager->m_bAddMoneyToRecvList &&
		pPostOfficeManager->m_bAddMsgToRecvList ) ||
		pPostOfficeManager->m_bRefreshList == TRUE ) {
		
		if( !pPostOfficeManager->m_bRefreshList ) {
			pPostOfficeManager->m_bAddItemToRecvList = FALSE;
			pPostOfficeManager->m_bAddMoneyToRecvList = FALSE;
			pPostOfficeManager->m_bAddMsgToRecvList = FALSE;
		}

		pPostOfficeManager->m_bRefreshList = FALSE;

		pList->Clear();

		// sort
		{
			cltPostOfficeRecvDataInfo tempDataInfo, *pDataInfo1, *pDataInfo2;
			cltPostOfficeRecvItemInfo tempItemInfo, *pItemInfo1, *pItemInfo2;

			for( i = 0; i < MAX_POSTOFFICE_RECV_NUMBER - 1; ++i ) {
				
				pDataInfo1 = &pPostOfficeManager->m_recvDataInfo[ i ];
				pItemInfo1 = &pPostOfficeManager->m_recvItemInfo[ i ];

				for( j = i + 1; j < MAX_POSTOFFICE_RECV_NUMBER; ++j ) {

					pDataInfo2 = &pPostOfficeManager->m_recvDataInfo[ j ];
					pItemInfo2 = &pPostOfficeManager->m_recvItemInfo[ j ];

					if( pDataInfo2->lParam == 0 ) continue;

					if( pDataInfo1->clDate.GetDateVary() <
						pDataInfo2->clDate.GetDateVary() ||
						pDataInfo1->lParam == 0 ) {

							tempDataInfo = *pDataInfo1;
							*pDataInfo1 = *pDataInfo2;
							*pDataInfo2 = tempDataInfo;

							tempItemInfo = *pItemInfo1;
							*pItemInfo1 = *pItemInfo2;
							*pItemInfo2 = tempItemInfo;
						}					
				}
			}
		}
	

		

		LONG lParam;
				
		for( int i = 0; i < MAX_POSTOFFICE_RECV_NUMBER; ++i ) {

			cltPostOfficeRecvDataInfo *pDataInfo = &pPostOfficeManager->m_recvDataInfo[ i ];

			lParam = pDataInfo->lParam;

			if( pDataInfo->lParam == 0 ) continue;

			TCHAR* pText = GetTxtFromMgr(1508);
			StringCchPrintf( szDate, 256, pText, pDataInfo->clDate.uiYear, pDataInfo->clDate.uiMonth, pDataInfo->clDate.uiDay );

			itemdata.Init();
			itemdata.siParam[0] = pDataInfo->lParam;
			itemdata.Set( 0, szDate );
			itemdata.Set( 1, pDataInfo->szPersonName );
			MStrCpy( buffer, pDataInfo->szMsg, 32 );
			itemdata.Set( 2, buffer );

			switch( HIWORD( lParam ) )
			{
			case LPARAMHIWORD_MSG:
				{
					itemdata.ColumnColor[0] = RGB( 0, 0, 200 );
					itemdata.ColumnColor[1] = RGB( 0, 0, 200 );
					itemdata.ColumnColor[2] = RGB( 0, 0, 200 );
				}
			}
			
			
			pList->AddItem( &itemdata );
		}

		pList->Refresh();
	}
}



void NPostOfficeRecvDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	CList *pList = (CList *)m_pControlMap[ LIST_RECVLIST ];
	

	CASE_CONTROL( LIST_RECVLIST )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{

				TCHAR name[ MAX_PLAYER_NAME ];
				TCHAR date[ 32 ];
				TCHAR msg[ 256 ];
				TCHAR buf[ 256 ];
				TCHAR recvType[10];
				
				LONG lParam;

				stListBoxItemData* pItemdata;

				SI16 index = pList->GetSelectedIndex();
				if( index < 0 ) break;

				//KHY - 0621 - 버그수정.
				TCHAR* pText = GetTxtFromMgr(1718);
				StringCchPrintf( recvType, 256, pText );					

				// 시간 
				pItemdata = pList->GetItemData( index, 0 );
				MStrCpy( date, (const TCHAR *)pItemdata->strText[ 0 ], 32 );

				// 이름 
				pItemdata = pList->GetItemData( index, 1 );
				MStrCpy( name, (const TCHAR *)pItemdata->strText[ 1 ], MAX_PLAYER_NAME );

				// 내용
				pItemdata = pList->GetItemData( index, 2 );
				MStrCpy( msg, (const TCHAR *)pPostOfficeManager->m_recvDataInfo[ index ].szMsg, 200 );

				lParam = pItemdata->siParam[0];

				// lParam
				pText = GetTxtFromMgr(1719);
				StringCchPrintf( buf, 256, pText, msg,recvType, name, date );

				SetEditText( EDIT_MSG, buf );
			
				switch( HIWORD( lParam ) )
				{
				case LPARAMHIWORD_ITEM:
					{
						pText = GetTxtFromMgr(1720);
						((CButton*)m_pControlMap[ BUTTON_RECVOK ])->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
					}
					break;

				case LPARAMHIWORD_MSG:
					{
						pText = GetTxtFromMgr(1721);
						((CButton*)m_pControlMap[ BUTTON_RECVOK ])->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
					}
					break;

				case LPARAMHIWORD_MONEY:
					{
						pText = GetTxtFromMgr(1722);
						((CButton*)m_pControlMap[ BUTTON_RECVOK ])->SetText( pText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
					}						
					break;
					
				}

				pPostOfficeManager->m_siCurMsgType = HIWORD( lParam );
				pPostOfficeManager->m_siCurMsgID = LOWORD( lParam );
			}
			break;
		}
		
	}

	else CASE_CONTROL( BUTTON_RECV )
	{	
		cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_RECVITEMLIST, 0 );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
	}

	
	else CASE_CONTROL( BUTTON_RECVOK )
	{
		if( pPostOfficeManager->m_siCurMsgType == 0 ) return;

		switch( pPostOfficeManager->m_siCurMsgType )
		{
			case LPARAMHIWORD_ITEM:
				{

					cltItem *pItem = pPostOfficeManager->GetItemFromList( pPostOfficeManager->m_siCurMsgID );
					
					if( pItem ) {
						cltGameMsgRequest_PostOfficeGetItem sendMsg( pPostOfficeManager->m_siCurMsgID, pItem );
						cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_GETITEM, sizeof( cltGameMsgRequest_PostOfficeGetItem ), (BYTE*)&sendMsg );
						pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );

						cltClient *pclclient = (cltClient *)pclClient;
						if(pclclient)
						{
							if(((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->IsShow() == false)
								((CNNewInventory*)pclclient->m_pDialog[ NNEWINVENTORY_DLG ])->Show();
						}
					}
				}
				break;

			case LPARAMHIWORD_MSG:
				{
					//[수정 : 황진성 2007. 10. 11 => 우체국 메시지 삭제 연타 후 디스플레이 오류.] 
					SI16 index = pList->GetSelectedIndex();
					pList->SetSelectedIndex(-1);
					if( index < 0 ) break;
										
					cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_DELMSG, pPostOfficeManager->m_siCurMsgID );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
					
				}
				break;

			case LPARAMHIWORD_MONEY:
				{
					cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_GETMONEY, pPostOfficeManager->m_siCurMsgID );
					pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );
				}						
				break;
		}

	}
	if( pclClient->IsCountrySwitch(Switch_reply))//답장하기 [2007.08.17 손성웅]
	{
		CASE_CONTROL( BUTTON_REPLY)
            {
				TCHAR name[ 20 ];
				stListBoxItemData* pItemdata;
				SI16 index = pList->GetSelectedIndex();
				if( index < 0 ) return;
				pItemdata = pList->GetItemData( index, 1 );
				MStrCpy( name, (const TCHAR *)pItemdata->strText[ 1 ], MAX_PLAYER_NAME );
				
				NStrBaseDlg* pStrBaseDlg = (NStrBaseDlg*)GetParentDialog();
				NPostOfficeSendItemDlg* pPostOfficeSenditem = (NPostOfficeSendItemDlg*)pStrBaseDlg->m_pUserChildDlg[0];
				pPostOfficeSenditem->WriteName(name);
				pStrBaseDlg->SetActiveTab(0);
			}
	}
	}