#include "DBGameProtocol.h"
#include "DBMsg-Gacha.h"

void DBGameProtocol::GachaBuy()
{
	// 받은 패킷
	sDBRequest_GachaBuy *pRecvMsg = (sDBRequest_GachaBuy *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GachaBuy sendMsg;
	sendMsg.packetHeader.usCmd	= DBMSG_RESPONSE_GACHABUY;
	sendMsg.packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.usCharID			= pRecvMsg->usCharID;
	sendMsg.bBestProduct		= pRecvMsg->bBestProduct;

	sendMsg.uiGachaCntAdd		= pRecvMsg->uiGachaCntAdd;

	//[추가 : 황진성 2007. 10. 24 순수하게 가차로만 얻은 아이템.]
	sendMsg.clGachaGetItem.Set(&pRecvMsg->clGetItem);
	sendMsg.clUseKeyItem.Set(&pRecvMsg->clKeyItem);

	// DB Query
	BEGIN_SP( "rsp_GachaBuy" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );

		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siKeyItemPos,	sizeof(pRecvMsg->siKeyItemPos) );
		SetItemParam( &pRecvMsg->clKeyItem );

		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siGetItemPos,	sizeof(pRecvMsg->siGetItemPos) );
		SetItemParam( &pRecvMsg->clGetItem );

		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->uiGachaCnt,		sizeof(pRecvMsg->uiGachaCnt) );
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->bPileSwitch,		sizeof(pRecvMsg->bPileSwitch) );	
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->bBestProduct,	sizeof(pRecvMsg->bBestProduct) );	
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->uiGachaCntAdd,	sizeof(pRecvMsg->uiGachaCntAdd) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.uiGachaCnt );
		m_odbc.GetData( &sendMsg.bBestProduct );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siKeyItemPos );
			GetItemData( &sendMsg.clKeyItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siGetItemPos );
			GetItemData( &sendMsg.clGetItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		// 소모된 아이템
		{
			char log[512] = "";
			sprintf(log, "invPos = %d, ItemNum = %d, AfterItemNum = %d, GachaCount = %d, BestProduct = %d"
				,	pRecvMsg->siKeyItemPos, pRecvMsg->clKeyItem.siItemNum, sendMsg.clKeyItem.siItemNum, sendMsg.uiGachaCnt, pRecvMsg->bBestProduct
				);
			sDBLogMsg_QueryString LogMsg("Gacha Buy Used", sendMsg.siPersonID, NULL, pRecvMsg->clKeyItem.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}

		// 생성된 아이템
		{
			char log[512] = "";
			sprintf(log, "InvPos = %d, ItemNum = %d, AfterItemNum = %d, GachaCount = %d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, UseDay = %02d%02d%02d/%02d일, Element = %d/%d, PileSwitch = %d"
				, pRecvMsg->siGetItemPos, pRecvMsg->clGetItem.siItemNum, sendMsg.clGetItem.siItemNum, sendMsg.uiGachaCnt
				, LOGITEMDISPLAY(sendMsg.clGetItem), USEDAYDISPLAY(sendMsg.clGetItem), ELEMENTDISPLAY(sendMsg.clGetItem)
				, pRecvMsg->bPileSwitch
				);
			sDBLogMsg_QueryString LogMsg("Gacha Buy", sendMsg.siPersonID, NULL, pRecvMsg->clGetItem.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}
}