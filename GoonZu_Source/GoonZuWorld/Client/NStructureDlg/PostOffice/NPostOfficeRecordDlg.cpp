#include "NPostOfficeDlg.h"

#include "../../InterfaceMgr/Interface/List.h"

#include "Char\CharManager\CharManager.h"
#include "..\..\Server\Rank\Rank.h"

#include "../../CommonLogic/MsgType.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"

#include "../Lib/JYLibFunction.h"
#include "..\..\Client\Music\Music.h"

extern cltCommonLogic* pclClient;
extern CJYLibFunction g_JYLibFunction;

NPostOfficeRecordDlg::NPostOfficeRecordDlg()
{
}

NPostOfficeRecordDlg::~NPostOfficeRecordDlg()
{

}

void NPostOfficeRecordDlg::InitDlg()
{
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	SetControlMap( LIST_RECORDLIST, TEXT("listbox_recordlist") );
	SetControlMap( EDIT_MSG, TEXT("editbox_msg") );

	SetControlMap( BUTTON_RECORD, TEXT("button_record") );

	CList *pList = (CList*)m_pControlMap[ LIST_RECORDLIST ];

	pList->SetTextPrintOption( DT_LEFT | DT_VCENTER | DT_SINGLELINE );
	pList->SetBorder(true);
	pList->SetBKColorUse(true);

	pList->Clear();

	TCHAR* pText[3];

	pText[0] = GetTxtFromMgr(1564);
	pText[1] = GetTxtFromMgr(1556);
	pText[2] = GetTxtFromMgr(1565);

	pList->SetColumn( 0, 92, pText[ 0 ] );
	pList->SetColumn( 1, 90, pText[ 1 ] );
	pList->SetColumn( 2, 154, pText[ 2 ] );

	pList->Refresh();


	pPostOfficeManager->m_recordcount = 0;
	pPostOfficeManager->bUpdateRecord = FALSE;


	this->NDlgTabProc(NULL,m_nControlID[BUTTON_RECORD],NULL);//[영진]
}

void NPostOfficeRecordDlg::Action()
{
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	if( pPostOfficeManager->bUpdateRecord == TRUE ) {
		pPostOfficeManager->bUpdateRecord = FALSE;

		ShowRecordList();
	}
}

void NPostOfficeRecordDlg::ShowRecordList()
{
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	CList *pList = (CList*)m_pControlMap[ LIST_RECORDLIST ];
	stListBoxItemData itemdata;

	pList->Clear();	

	TCHAR szDate[ 256 ];
	TCHAR szName[ 256 ];
	TCHAR sendrecv[ 256 ];
	TCHAR buffer[ 1024 ];
	
	for( int i = pPostOfficeManager->m_recordcount - 1; i >= 0; --i ) {

		if( pPostOfficeManager->m_recordinfo[ i ].siSendRecv == 1 ) {
			TCHAR* pText = GetTxtFromMgr(1712);
			StringCchPrintf( sendrecv, 256, pText );
		} else {
			TCHAR* pText = GetTxtFromMgr(1713);
			StringCchPrintf( sendrecv, 256, pText );
		}

		TCHAR* pText = GetTxtFromMgr(5088);
		StringCchPrintf( szDate, 256, pText, 
			pPostOfficeManager->m_recordinfo[ i ].clDate.uiYear, 
			pPostOfficeManager->m_recordinfo[ i ].clDate.uiMonth, 
			pPostOfficeManager->m_recordinfo[ i ].clDate.uiDay );

		StringCchPrintf( szName, 256, TEXT("%s"), pPostOfficeManager->m_recordinfo[ i ].szPersonName );

		switch( pPostOfficeManager->m_recordinfo[ i ].siMsgType ) {
			case POSTOFFICE_MSGTYPE_ITEM:
				{
					TCHAR* pText = GetTxtFromMgr(1714);
					StringCchPrintf( buffer, 1024, pText, 
						sendrecv,
						pclClient->pclItemManager->GetName( pPostOfficeManager->m_recordinfo[ i ].siItemUnique ),
						pPostOfficeManager->m_recordinfo[ i ].siItemNum );

				}
				break;

			case POSTOFFICE_MSGTYPE_MSG:
				{
					TCHAR* pText = GetTxtFromMgr(1715);
					StringCchPrintf( buffer, 1024, pText, sendrecv );
				}
				break;

			case POSTOFFICE_MSGTYPE_MONEY:
				{
					TCHAR		szPrice[64] = { '\0', };
					GMONEY		siPrice = pPostOfficeManager->m_recordinfo[ i ].siMoney ;
					g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
					NTCHARString512	kText(GetTxtFromMgr(1716));
					kText.Replace("#recv#", sendrecv);
					kText.Replace("#money#", szPrice);										
					
					StringCchPrintf( buffer, 1024, kText );
				}
				break;
		}

		itemdata.Init();
		itemdata.siParam[0] = i;
		itemdata.Set( 0, szDate );
		itemdata.Set( 1, szName );
		itemdata.Set( 2, buffer );
		
		pList->AddItem( &itemdata );
	}

	pList->Refresh();
}

void NPostOfficeRecordDlg::NDlgTabProc( UINT nEvent, int nControlID, CControllerObj *pControl )
{

	//KHY - 0612 - 송수신내역 내용 추가. - 0619- 내용수정.
	cltPostOfficeManager *pPostOfficeManager = (cltPostOfficeManager *)m_pStrManager;

	CList *pList = (CList *)m_pControlMap[ LIST_RECORDLIST ];
	

	CASE_CONTROL( LIST_RECORDLIST )
	{
		switch( nEvent )
		{
			case EVENT_LISTBOX_SELECTION:
			{

				TCHAR sendrecv[ 256 ];

				TCHAR name[ MAX_PLAYER_NAME ];
				TCHAR date[ 32 ];
				TCHAR buffer[ 1024 ];

				TCHAR recvType[256];
				TCHAR buf[ 1076 ];

				stListBoxItemData* pItemdata;

				SI16 index = pPostOfficeManager->m_recordcount - pList->GetSelectedIndex() -1;

				if( pPostOfficeManager->m_recordinfo[index ].siSendRecv == 1 ) {
					TCHAR* pText = GetTxtFromMgr(1712);
					StringCchPrintf( sendrecv, 256, pText );

					pText = GetTxtFromMgr(7506);
					StringCchPrintf( recvType, 256, pText );
				} else {
					TCHAR* pText = GetTxtFromMgr(1713);
					StringCchPrintf( sendrecv, 256, pText );

					pText = GetTxtFromMgr(1718);
					StringCchPrintf( recvType, 256, pText );					
				}
				
				// 시간 
				pItemdata = pList->GetItemData( pList->GetSelectedIndex(), 0 );
				MStrCpy( date, (const TCHAR *)pItemdata->strText[ 0 ], 32 );

				// 이름 
				pItemdata = pList->GetItemData( pList->GetSelectedIndex(), 1 );
				MStrCpy( name, (const TCHAR *)pItemdata->strText[ 1 ], MAX_PLAYER_NAME );

				// 내용
				pItemdata = pList->GetItemData( index, 2 );

				switch( pPostOfficeManager->m_recordinfo[index ].siMsgType ) 
				{
					case POSTOFFICE_MSGTYPE_ITEM:
						{
							TCHAR* pText = GetTxtFromMgr(1714);
							StringCchPrintf( buffer, 1024, pText, 
								sendrecv,
								pclClient->pclItemManager->GetName( pPostOfficeManager->m_recordinfo[ index].siItemUnique ),
								pPostOfficeManager->m_recordinfo[index].siItemNum );

						}
						break;

					case POSTOFFICE_MSGTYPE_MSG:
						{
							TCHAR* pText = GetTxtFromMgr(1715);
							StringCchPrintf( buffer, 1024, pText, sendrecv );
						}
						break;

					case POSTOFFICE_MSGTYPE_MONEY:
						{
							TCHAR		szPrice[64] = { '\0', };
							GMONEY		siPrice = pPostOfficeManager->m_recordinfo[ index].siMoney ;
							g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
							NTCHARString512	kText(GetTxtFromMgr(1716));
							kText.Replace("#recv#", sendrecv);
							kText.Replace("#money#", szPrice);										

							StringCchPrintf( buffer, 1024, kText );
						}
						break;
				}


				TCHAR* pText = GetTxtFromMgr(1719);
				StringCchPrintf( buf, 256, pText, buffer, recvType,name, date );

				SetEditText( EDIT_MSG, buf );
			}
			break;
		}
		
	}

	else CASE_CONTROL( BUTTON_RECORD )
	{
		cltMsg clMsg( GAMEMSG_REQUEST_POSTOFFICE_GETRECORD, 0 );
		pclClient->SendMsgToServer( (sPacketHeader*)&clMsg );	
	}

}
