#include <CommonLogic.h>
#include "..\..\Server\Server.h"
#include "Char\CharCommon\Char-Common.h"

#include "..\..\Resource.h"
#include "..\..\Client\NStructureDlg\PostOffice\NPostOfficeDlg.h"

#include "Postoffice.h"
//#include "..\..\DBManager\GameDBManager_World\DBMsg-Market.h"

#include "MsgRval-Define.h"
#include "../../CommonLogic/MsgType-Structure.h"
#include "../../CommonLogic/Msg/MsgType-PostOffice.h"

#include "../CityHall/TaxpaymentListDlg.h"

extern cltCommonLogic* pclClient;


void cltPostOfficeManager::AddItemToRecvList( cltPostOfficeRecvItemInfo *pItemInfo )
{

	if( pStrBaseDlg ) {

		for( int i = 0; i < MAX_POSTOFFICE_RECV_NUMBER; ++i )
		{
			if( m_recvDataInfo[ i ].lParam == 0 ) {
				// 시간
				m_recvDataInfo[ i ].clDate.Set( &pItemInfo->clDate );
				// 이름
				MStrCpy( m_recvDataInfo[ i ].szPersonName, pItemInfo->szSenderPersonName, MAX_PLAYER_NAME );
				// 내용
				TCHAR itemname[128];
				if(pItemInfo->clItem.GetFullName( pclClient->pclItemManager, itemname , 128) == true)
				{
					TCHAR *pText = GetTxtFromMgr(1723);
					StringCchPrintf( m_recvDataInfo[ i ].szMsg, 210, pText, itemname, pItemInfo->clItem.GetItemNum() );
				}
				//param
				m_recvDataInfo[ i ].lParam = MAKELPARAM( pItemInfo->siMsgID, LPARAMHIWORD_ITEM );

				m_recvItemInfo[ i ] = *pItemInfo;

				break;
			}
		}

	}
}

void cltPostOfficeManager::AddMoneyToRecvList( cltPostOfficeRecvMoneyInfo *pMoneyInfo )
{

	if( pStrBaseDlg ) {

		for( int i = 0; i < MAX_POSTOFFICE_RECV_NUMBER; ++i )
		{
			if( m_recvDataInfo[ i ].lParam == 0 ) {
				// 시간
				m_recvDataInfo[ i ].clDate.Set( &pMoneyInfo->clDate );
				// 이름
				MStrCpy( m_recvDataInfo[ i ].szPersonName, pMoneyInfo->szSenderPersonName, MAX_PLAYER_NAME );
				// 내용
				TCHAR		szPrice[64] = { '\0', };
				GMONEY		siPrice = pMoneyInfo->siMoney ;
				g_JYLibFunction.SetNumUnit(siPrice, szPrice, sizeof(szPrice));
				NTCHARString512	kText(GetTxtFromMgr(1739));
				kText.Replace("#money#", szPrice );
                StringCchPrintf( m_recvDataInfo[ i ].szMsg, 210, kText );
				//param
				m_recvDataInfo[ i ].lParam = MAKELPARAM( pMoneyInfo->siMsgID, LPARAMHIWORD_MONEY );

				break;
			}
		}

	}
}


void cltPostOfficeManager::AddMsgToRecvList( cltPostOfficeRecvMsgInfo *pMoneyInfo )
{

	if( pStrBaseDlg ) {

		TCHAR *pText;
		TCHAR *pText2;

		for( int i = 0; i < MAX_POSTOFFICE_RECV_NUMBER; ++i )
		{
			if( m_recvDataInfo[ i ].lParam == 0 ) {
				// 시간
				m_recvDataInfo[ i ].clDate.Set( &pMoneyInfo->clDate );
				// 이름
				MStrCpy( m_recvDataInfo[ i ].szPersonName, pMoneyInfo->szSenderPersonName, MAX_PLAYER_NAME );
				// 내용
				pText = GetTxtFromMgr(1724);
				pText2 = GetTxtFromMgr(5382);
				StringCchPrintf( m_recvDataInfo[ i ].szMsg, 210, pText, pMoneyInfo->szMsg ,pText2);
				//param
				m_recvDataInfo[ i ].lParam = MAKELPARAM( pMoneyInfo->siMsgID, LPARAMHIWORD_MSG );

				break;
			}
		}

	} 
}

void cltPostOfficeManager::ListRecvItem_DeleteAll()
{

#ifdef _SAFE_MEMORY
	m_recvDataInfo.ZeroMem();
	m_recvItemInfo.ZeroMem();
#else
	ZeroMemory( m_recvDataInfo, sizeof( m_recvDataInfo ) );
	ZeroMemory( m_recvItemInfo, sizeof( m_recvItemInfo ) );	
#endif

	m_bRefreshList = TRUE;
}


void cltPostOfficeManager::ListRecvItem_DelItem( LPARAM lParam )
{

	if( pStrBaseDlg ) {

		for( int i = 0; i < MAX_POSTOFFICE_RECV_NUMBER; ++i ) {

			if( m_recvDataInfo[ i ].lParam == lParam ) {
				m_recvDataInfo[ i ].lParam = 0;
				m_bRefreshList = TRUE;
				break;
			}
		}

	}
}

cltItem* cltPostOfficeManager::GetItemFromList( SI32 siMsgID )
{
	for( int i = 0; i < MAX_POSTOFFICE_RECV_NUMBER; ++i )
	{
		if( m_recvItemInfo[ i ].siMsgID == siMsgID &&
			m_recvItemInfo[ i ].clItem.siUnique != 0 ) {
				return &m_recvItemInfo[ i ].clItem;
			}
	}

	return NULL;
}


