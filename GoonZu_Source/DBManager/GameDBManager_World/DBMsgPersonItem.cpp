#include "DBGameProtocol.h"
#include "DBMsg-Item.h"
#include "DBMsg-System.h"
#include "DBMsg-Person.h"
#include "DBMsg-House.h"
#include "DBMsg-Fishing.h"
#include "DBMsg-PersonalShop.h"
#include "DBMsg-Agriculture.h"


void DBGameProtocol::GetPersonItem()
{
	sDBRequest_PersonItemInfoGet *pRecvMsg = (sDBRequest_PersonItemInfoGet *)m_pPacket;

	sDBResponse_PersonItemInfoGet sendMsg;

	ZeroMemory( &sendMsg, sizeof( sendMsg ) );

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONITEMINFOGET;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	int siPersonID = pRecvMsg->siPersonID;

	SI16	siItemPos;

	BEGIN_SP( "rsp_PersonItemGet" )
		m_odbc.SetParam( SQL_INTEGER, &siPersonID, sizeof(siPersonID) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siItemPos, 0);								// pos
		if(siItemPos >= MAX_ITEM_PER_PERSON)
			continue;
		GetItemData( &sendMsg.ItemInfo.clItem[siItemPos] );			// ItemInfo
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

// �������� �κ��� �ִ´�.
void DBGameProtocol::InputItemToPerson()
{
	// ���� ��Ŷ
	sDBRequest_InputItemToPerson *pRecvMsg = (sDBRequest_InputItemToPerson *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_InputItemToPerson sendMsg;

	// �α׿�
	char szCommand[50] = "";

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_INPUTITEMTOPERSON;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.clInputedItem.Set(&pRecvMsg->clItem);
	
	// DB Query
	switch (pRecvMsg->siInputItemPersonOption)
	{
	case INPUTITEMPERSON_OPTION_NONE:
		strcpy( szCommand, "Get item" );
		break;
	case INPUTITEMPERSON_OPTION_MASTER:
		strcpy( szCommand, "Get item from GM" );
		break;
	case INPUTITEMPERSON_OPTION_WAR:
		strcpy( szCommand, "Get item from war" );
		break;
	case INPUTITEMPERSON_OPTION_PICKUP:
		strcpy( szCommand, "Pickup Item" );
		break;
	default:
		strcpy( szCommand, "Get item" );
		break;
	}

	BEGIN_SP( "rsp_PersonItemInput" )
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siInputItemPersonOption),	sizeof(pRecvMsg->siInputItemPersonOption) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),	sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemPos),	sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->bPileSwitch),	sizeof(pRecvMsg->bPileSwitch) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_BIT, &(pRecvMsg->bCreateSID),	sizeof(pRecvMsg->bCreateSID) );
	END_SP()

	EXEC_SQL()
	
	SI32 siErrCount = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siItemPos, 0);
		sendMsg.siInputItemPersonOption = pRecvMsg->siInputItemPersonOption;
		m_odbc.GetData( &sendMsg.siChangedNum, 0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &siErrCount, 0 );
	END_FETCHDATA()

	// SID ���� - �α׿�
	if( pRecvMsg->bCreateSID )
	{
		sendMsg.clInputedItem.siSID = sendMsg.clItem.siSID;
	}
	
	// send
	SendPacket( (BYTE *)&sendMsg );
	
	// �α� �ۼ�
	if(sendMsg.siResult == 1 && siErrCount == 0)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, ItemPos = %d, Amount = %d, LeftItemNum = %d, UseDay = %02d%02d%02d/%02d��, Element = %d/%d"
				, pRecvMsg->siInputItemPersonOption, sendMsg.siItemPos, sendMsg.siChangedNum, sendMsg.clItem.siItemNum
				, USEDAYDISPLAY(sendMsg.clItem), ELEMENTDISPLAY(sendMsg.clItem)
				);
		sDBLogMsg_QueryString LogMsg( szCommand, sendMsg.siPersonID, pRecvMsg->siGiverPersonID, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::MoveItem()
{
	// ���� ��Ŷ
	sDBRequest_MoveItem *pRecvMsg = (sDBRequest_MoveItem *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_MoveItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MOVEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.clMoveItem.Set(&pRecvMsg->clFromItem);

	// DB Query
	SI32 siResult = 0;
	SI16 itemnum = pRecvMsg->clFromItem.GetItemNum();
	
	BEGIN_SP( "rsp_PersonItemMove" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),				sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siFromItemPos),			sizeof(pRecvMsg->siFromItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clFromItem.siUnique),	sizeof(pRecvMsg->clFromItem.siUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clFromItem.siItemNum),	sizeof(pRecvMsg->clFromItem.siItemNum) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->bFromItemPileSwitch),		sizeof(pRecvMsg->bFromItemPileSwitch) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),				sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siToItemPos),			sizeof(pRecvMsg->siToItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clToItem.siUnique),		sizeof(pRecvMsg->clToItem.siUnique) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siResult, 0 );
	END_FETCHDATA()

	SI16 siItemPos	= 0;

	// FromItem
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siFromItemPos, 0 );
		GetItemData( &sendMsg.clFromItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// ToItem
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siToItemPos, 0 );
		GetItemData( &sendMsg.clToItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siResult = 1;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, FromItemPos = %d, ToItemPos = %d, Amount = %d, LeftFromItemNum = %d, LeftFromUseDay = %02d%02d%02d/%02d��, LeftFromElement = %d/%d, LeftToItemNum = %d, LeftToUseDay = %02d%02d%02d/%02d��, LeftToElement = %d/%d, PileSwitch = %d"
			, siResult, sendMsg.siFromItemPos, sendMsg.siToItemPos
			, pRecvMsg->clFromItem.siItemNum, sendMsg.clFromItem.siItemNum, USEDAYDISPLAY(sendMsg.clFromItem), ELEMENTDISPLAY(sendMsg.clFromItem), sendMsg.clToItem.siItemNum, USEDAYDISPLAY(sendMsg.clToItem), ELEMENTDISPLAY(sendMsg.clToItem)
			, pRecvMsg->bFromItemPileSwitch
			);
		sDBLogMsg_QueryString LogMsg( "Move Item", pRecvMsg->siPersonID, NULL, sendMsg.clToItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::OutputItemFromPerson()
{
	// ���� ��Ŷ
	sDBRequest_OutputItemFromPerson *pRecvMsg = (sDBRequest_OutputItemFromPerson *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_OutputItemFromPerson sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_OUTPUTITEMFROMPERSON;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI16 itemnum = pRecvMsg->clItem.GetItemNum();

	// DB Query
	BEGIN_SP( "rsp_PersonItemOutput" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),			sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemPos),			sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clItem.siUnique),	sizeof(pRecvMsg->clItem.siUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &itemnum,						sizeof(itemnum) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()
	
	sendMsg.siResult	= 1;
	sendMsg.siPersonID	= pRecvMsg->siPersonID;
	sendMsg.siItemPos	= pRecvMsg->siItemPos;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ItemPos = %d, Amount = %d, LeftItemNum = %d, UseDay = %02d%02d%02d/%02d��, Element = %d/%d"
				, pRecvMsg->siItemPos, itemnum, sendMsg.clItem.siItemNum, USEDAYDISPLAY(pRecvMsg->clItem), ELEMENTDISPLAY(pRecvMsg->clItem)
				);
		sDBLogMsg_QueryString LogMsg( "Output Item", sendMsg.siPersonID, NULL, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::MoveItemPerson()
{
	// ���� ��Ŷ
	sDBRequest_MoveItemPerson *pRecvMsg = (sDBRequest_MoveItemPerson *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_MoveItemPerson sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MOVEITEMPERSON;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siDestroyItemUnique = pRecvMsg->clFromItem.siUnique;
	MStrCpy(sendMsg.szFromUserName, pRecvMsg->szFromUserName, MAX_PLAYER_NAME);
	MStrCpy(sendMsg.szToUserName, pRecvMsg->szToUserName, MAX_PLAYER_NAME);

	//[�߰� : Ȳ���� 2007. 10. 24 ����� ������.]
	sendMsg.clMoveItem.Set(&pRecvMsg->clFromItem);

	SI32 siResult = 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemMoveToPerson" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siReason),			sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->bDestroy),			sizeof(pRecvMsg->bDestroy) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siFromPersonID),			sizeof(pRecvMsg->siFromPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siFromItemPos),			sizeof(pRecvMsg->siFromItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clFromItem.siUnique),	sizeof(pRecvMsg->clFromItem.siUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clFromItem.siItemNum),	sizeof(pRecvMsg->clFromItem.siItemNum) );
		m_odbc.SetParam( SQL_TINYINT, &(pRecvMsg->bFromItemPileSwitch),		sizeof(pRecvMsg->bFromItemPileSwitch) );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siToPersonID),			sizeof(pRecvMsg->siToPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siToItemPos),			sizeof(pRecvMsg->siToItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clToItem.siUnique),		sizeof(pRecvMsg->clToItem.siUnique) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siReason, 0 );
		m_odbc.GetData( &sendMsg.bDestroy, 0 );
	END_FETCHDATA()
	
	// FromItem
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siFromPersonID, 0 );
		m_odbc.GetData( &sendMsg.siFromItemPos, 0 );
		GetItemData( &sendMsg.clFromItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	// ToItem
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siToPersonID, 0 );
		m_odbc.GetData( &sendMsg.siToItemPos, 0 );
		GetItemData( &sendMsg.clToItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, FromItemPos = %d, ToItemPos = %d, Amount = %d, LeftFromItemNum = %d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, LeftFromUseDay = %02d%02d%02d/%02d��, LeftFromElement = %d/%d, LeftToItemNum = %d, LeftToUseDay = %02d%02d%02d/%02d��, LeftToElement = %d/%d, PileSwitch = %d"
				, pRecvMsg->siReason, sendMsg.siFromItemPos, sendMsg.siToItemPos
				, pRecvMsg->clFromItem.siItemNum, sendMsg.clFromItem.siItemNum, LOGITEMDISPLAY(sendMsg.clFromItem), USEDAYDISPLAY(sendMsg.clFromItem), ELEMENTDISPLAY(sendMsg.clFromItem), sendMsg.clToItem.siItemNum, USEDAYDISPLAY(sendMsg.clToItem), ELEMENTDISPLAY(sendMsg.clToItem)
				, pRecvMsg->bFromItemPileSwitch
				);
		sDBLogMsg_QueryString LogMsg( "Move Item To Person", pRecvMsg->siFromPersonID, pRecvMsg->siToPersonID, pRecvMsg->clFromItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::UseItem()
{
	// ���� ��Ŷ
	sDBRequest_UseItem *pRecvMsg = (sDBRequest_UseItem *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_UseItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_USEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI16 itemnum = pRecvMsg->clItem.GetItemNum();

	// DB Query
	BEGIN_SP( "rsp_PersonItemUse" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),			sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemPos),			sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->clItem.siUnique),	sizeof(pRecvMsg->clItem.siUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &itemnum,						sizeof(itemnum) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siUseReason,			sizeof(pRecvMsg->siUseReason) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()
	
	sendMsg.siPersonID	= pRecvMsg->siPersonID;
	sendMsg.siItemPos	= pRecvMsg->siItemPos;
	sendMsg.siUseReason	= pRecvMsg->siUseReason;
	memcpy(&sendMsg.clUsedItem, &pRecvMsg->clItem, sizeof(cltItem));
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, ItemPos = %d, Amount = %d, UseDay = %02d%02d%02d/%02d��, Element = %d/%d, LeftItemNum = %d"
				, sendMsg.siUseReason, pRecvMsg->siItemPos, itemnum, USEDAYDISPLAY(pRecvMsg->clItem), ELEMENTDISPLAY(pRecvMsg->clItem), sendMsg.clItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg( "Use Item", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::SellNPC()
{
	// ���� ��Ŷ
	sDBRequest_SellNPC *pRecvMsg = (sDBRequest_SellNPC *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_SellNPC sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SELLNPC;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	sendMsg.siIitemUnique = pRecvMsg->clItem.siUnique;
	sendMsg.siItemNum = pRecvMsg->clItem.siItemNum;

	//[�߰� : Ȳ���� 2007. 10. 24 �����ϰ� �Ǹ��ϴ� ������ ����, ����, NPC ����]
	sendMsg.clSellItem.Set(&pRecvMsg->clItem);
	sendMsg.siSellMoney = pRecvMsg->siPrice;
	sendMsg.siNPCKind   = pRecvMsg->siNPCKind;
	
	// DB Query
	BEGIN_SP( "rsp_PersonItemSellNPC" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siPersonID),	sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siItemPos),		sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->siPrice),		sizeof(pRecvMsg->siPrice) );
	END_SP()
	
	EXEC_SQL()
	
	SI32 siBeforeAmount = 0;
	GMONEY siBeforeMoney = 0;
	GMONEY siAfterMoney = 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siMoney, 0 );
		m_odbc.GetData( &sendMsg.siChangedMoney, 0 );
		m_odbc.GetData( &siBeforeAmount, 0 );
		m_odbc.GetData( &siBeforeMoney, 0 );
	END_FETCHDATA()
	
	sendMsg.siVillageUnique = pRecvMsg->siVillageUnique;
	
	// send
	SendPacket( (BYTE *)&sendMsg );


	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, ItemPos = %d, Amount = %d, Price = %I64d, LeftItemNum = %d, BeforeItemNum = %d, BeforeMoney = %I64d, AfterMoney = %I64d"
				, pRecvMsg->clItem.GetRareType(), pRecvMsg->siItemPos, pRecvMsg->clItem.siItemNum, pRecvMsg->siPrice, sendMsg.clItem.siItemNum
				, siBeforeAmount, siBeforeMoney, sendMsg.siMoney
				);
		sDBLogMsg_QueryString LogMsg( "Sell Item", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// NPC�κ��� �������� �����Ѵ�.
void DBGameProtocol::BuyNPC()
{
	// ���� ��Ŷ
	sDBRequest_BuyNPC *pRecvMsg = (sDBRequest_BuyNPC *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_BuyNPC sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BUYNPC;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	//[�߰� : Ȳ���� 2007. 10. 24 �����ϰ� ��� ������ ����, ����, NPC ����]
	sendMsg.clBuyItem.Set(&pRecvMsg->clItem);
	sendMsg.siBuyMoney = pRecvMsg->siPrice;
	sendMsg.siNPCkind  = pRecvMsg->siNPCkind;
	
	SI32 siErrCount = 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemBuyNPC" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siPersonID),	sizeof(pRecvMsg->siPersonID) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->bPileSwitch),	sizeof(pRecvMsg->bPileSwitch) );
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->siPrice),		sizeof(pRecvMsg->siPrice) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siItemPos),		sizeof(pRecvMsg->siItemPos) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,		0);
		m_odbc.GetData( &sendMsg.siPersonID,	0);
		m_odbc.GetData( &sendMsg.siItemPos,		0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siMoney,		0);
		m_odbc.GetData( &siErrCount,			0);
	END_FETCHDATA()
	
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1 && siErrCount == 1)
	{
		char log[512] = "";
		sprintf(log, "ItemPos = %d, Amount = %d, Price = %I64d, LeftItemNum = %d, AfterMoney = %I64d"
				, pRecvMsg->siItemPos, pRecvMsg->clItem.siItemNum, pRecvMsg->siPrice, sendMsg.clItem.siItemNum, sendMsg.siMoney
				);
		sDBLogMsg_QueryString LogMsg( "Buy Item", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::MoveItemInvToStg()
{
	// ���� ��Ŷ
	sDBRequest_MoveItemInvToStg *pRecvMsg = (sDBRequest_MoveItemInvToStg *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_MoveItemInvToStg sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MOVEITEMINVTOSTG;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	//[�߰� : Ȳ���� 2007. 10. 24 �κ����� â�� �̵��ϴ� ����������.]
	sendMsg.clMoveItem.Set(&pRecvMsg->clItem);
	
	// DB Query
	BEGIN_SP( "rsp_PersonItemInvToStg" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siPersonID),			sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siInvPos),				sizeof(pRecvMsg->siInvPos) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siVillageUnique),		sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siHouseUnitSlot),		sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siStgPos),				sizeof(pRecvMsg->siStgPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->bCanPileSwitch),		sizeof(pRecvMsg->bCanPileSwitch) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,				0);
		m_odbc.GetData( &sendMsg.siPersonID,			0);
		m_odbc.GetData( &sendMsg.siInvPos,				0);
		GetItemData( &sendMsg.clInvItem );
		m_odbc.GetData( &sendMsg.siVillageUnique,		0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot,		0);
		m_odbc.GetData( &sendMsg.siStgPos,				0);
		GetItemData( &sendMsg.clStgItem );
	END_FETCHDATA()
	
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "FromItemPos = %d, ToItemPos = %d, VillageUnique = %d, HouseUnitIndex = %d, Amount = %d, LeftInvItemNum = %d, LeftStgItemNum %d"
				, pRecvMsg->siInvPos, pRecvMsg->siStgPos, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot
				, pRecvMsg->clItem.siItemNum, sendMsg.clInvItem.siItemNum, sendMsg.clStgItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg( "Move inv item to stg", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

}


void DBGameProtocol::MoveItemStgToInv()
{
	// ���� ��Ŷ
	sDBRequest_MoveItemStgToInv *pRecvMsg = (sDBRequest_MoveItemStgToInv *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_MoveItemStgToInv sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MOVEITEMSTGTOINV;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	//[�߰� : Ȳ���� 2007. 10. 24 â������ �κ����� �̵��ϴ� ����������.]	
	sendMsg.clMoveItem.Set(&pRecvMsg->clItem);
	
	// DB Query
	BEGIN_SP( "rsp_PersonItemStgToInv" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siVillageUnique),		sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siHouseUnitSlot),		sizeof(pRecvMsg->siHouseUnitSlot) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siStgPos),				sizeof(pRecvMsg->siStgPos) );
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->siPersonID),			sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->siInvPos),				sizeof(pRecvMsg->siInvPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->bCanPileSwitch),		sizeof(pRecvMsg->bCanPileSwitch) );

	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,				0);
		m_odbc.GetData( &sendMsg.siVillageUnique,		0);
		m_odbc.GetData( &sendMsg.siHouseUnitSlot,		0);
		m_odbc.GetData( &sendMsg.siStgPos,				0);
		GetItemData( &sendMsg.clStgItem );
		m_odbc.GetData( &sendMsg.siPersonID,			0);
		m_odbc.GetData( &sendMsg.siInvPos,				0);
		GetItemData( &sendMsg.clInvItem );
	END_FETCHDATA()
	
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "FromItemPos = %d, ToItemPos = %d, VillageUnique = %d, HouseUnitIndex = %d, Amount = %d, LeftStgItemNum %d, LeftInvItemNum = %d"
				, pRecvMsg->siInvPos, pRecvMsg->siStgPos, sendMsg.siVillageUnique, sendMsg.siHouseUnitSlot
				, pRecvMsg->clItem.siItemNum, sendMsg.clStgItem.siItemNum, sendMsg.clInvItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg( "Move stg item to inv", sendMsg.siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::WasteDurability()
{
	// ���� ��Ŷ
	sDBRequest_WasteDurability *pRecvMsg = (sDBRequest_WasteDurability *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_WasteDurability sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WASTEDURABILITY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	SI32 siBeforeUnique = 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemDurWaste" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siFromPos), sizeof(pRecvMsg->siFromPos) );
		SetItemParam( &pRecvMsg->clItem );
	END_SP()
		
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siFromPos, 0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &siBeforeUnique, 0);
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";

		//cyj �Ⱓ���� ����
		//sprintf(log, "RareType = %d, ItemPos = %d, LeftItemNum %d, Dur = %d/%d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
		//		, pRecvMsg->clItem.GetRareType(), pRecvMsg->siFromPos, sendMsg.clItem.siItemNum, sendMsg.clItem.GetDurability(), sendMsg.clItem.GetMaxDurability()
		//		, LOGITEMDISPLAY(pRecvMsg->clItem)
		//		);
		sprintf(log, "RareType = %d, ItemPos = %d, LeftItemNum = %d, UseDay = %02d%02d%02d/%02d��, Dur = %d/%d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
			, pRecvMsg->clItem.GetRareType(), pRecvMsg->siFromPos, sendMsg.clItem.siItemNum, USEDAYDISPLAY(pRecvMsg->clItem), sendMsg.clItem.GetDurability(), sendMsg.clItem.GetMaxDurability()
			, LOGITEMDISPLAY(pRecvMsg->clItem)
			);
		sDBLogMsg_QueryString LogMsg( "Waste Dur item", sendMsg.siPersonID, NULL, siBeforeUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::CrystalDurability()
{
	// ���� ��Ŷ
	sDBRequest_CrystalDurability *pRecvMsg = (sDBRequest_CrystalDurability *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_CrystalDurability sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CRYSTALDURABILITY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siBeforeUnique = 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemDurWaste" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siFromPos), sizeof(pRecvMsg->siFromPos) );
		SetItemParam( &pRecvMsg->clItem );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siFromPos, 0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &siBeforeUnique, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}



void DBGameProtocol::InvRepair()
{
	// ���� ��Ŷ
	sDBRequest_InvRepair *pRecvMsg = (sDBRequest_InvRepair *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_InvRepair sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_INVREPAIR;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	

	// DB Query
	BEGIN_SP( "rsp_PersonItemRepair" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siFromPos), sizeof(pRecvMsg->siFromPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siRepairPersonID), sizeof(pRecvMsg->siRepairPersonID) );
		m_odbc.SetParam( SQL_BIGINT, &(pRecvMsg->siRepairMoney), sizeof(pRecvMsg->siRepairMoney) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siFromPos, 0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siRepairPersonID, 0 );
		m_odbc.GetData( &sendMsg.siRepairMoney, 0 );
		m_odbc.GetData( &sendMsg.siRepairerMoney, 0 );
		m_odbc.GetData( &sendMsg.siPersonIDMoney, 0 );
	END_FETCHDATA()

	sendMsg.siNPCVillageUnique = pRecvMsg->siNPCVillageUnique;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, Dur = %d/%d, ItemPos = %d, RepairMoney = %I64d, LeftMoney = %I64d, LeftRepairerMoney = %I64d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
				, pRecvMsg->clItem.GetRareType(), pRecvMsg->clItem.GetDurability(), pRecvMsg->clItem.GetMaxDurability(), pRecvMsg->siFromPos, pRecvMsg->siRepairMoney, sendMsg.siRepairMoney, sendMsg.siRepairerMoney
				, LOGITEMDISPLAY(pRecvMsg->clItem)
				);
		sDBLogMsg_QueryString LogMsg( "Repair item", pRecvMsg->siPersonID, pRecvMsg->siRepairPersonID, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::DropItem()
{
	// ���� ��Ŷ
	sDBRequest_DropItem *pRecvMsg = (sDBRequest_DropItem *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_DropItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DROPITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_PersonItemDrop" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID), sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siFromPos), sizeof(pRecvMsg->siFromPos) );
		SetItemParam( &pRecvMsg->clItem );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siFromPos, 0);
		GetItemData( &sendMsg.clItem );
		GetItemData( &sendMsg.clChangedItem );
	END_FETCHDATA()

	sendMsg.siMapIndex	= pRecvMsg->siMapIndex;
	sendMsg.siX			= pRecvMsg->siX;
	sendMsg.siY			= pRecvMsg->siY;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, ItemPos = %d, Amount = %d, LeftItemNum = %d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
				, pRecvMsg->clItem.GetRareType(), pRecvMsg->siFromPos, sendMsg.clChangedItem.siItemNum, sendMsg.clItem.siItemNum
				, LOGITEMDISPLAY(pRecvMsg->clItem)
				);
		sDBLogMsg_QueryString LogMsg( "Drop item", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::MakeItemInv()
{
	// ���� ��Ŷ
	sDBRequest_MakeItemInv *pRecvMsg = (sDBRequest_MakeItemInv *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_MakeItemInv sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_MAKEITEMINV;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_PersonItemMake" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMaterialPersonID, sizeof(pRecvMsg->siMaterialPersonID) );
		for( int siMaterialNumber=0; siMaterialNumber < MAX_MATERIAL_NUMBER_FOR_DB; siMaterialNumber++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMaterial[siMaterialNumber].siStgPos, sizeof(pRecvMsg->clMaterial[siMaterialNumber].siStgPos) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clMaterial[siMaterialNumber].siItemUnique, sizeof(pRecvMsg->clMaterial[siMaterialNumber].siItemUnique) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clMaterial[siMaterialNumber].siUseItemNum, sizeof(pRecvMsg->clMaterial[siMaterialNumber].siUseItemNum) );
		}

/*		for( int siToolCount = 0; siToolCount < MAX_TOOL_NUMBER; siToolCount++ )
		{
			SetItemParam( &pRecvMsg->clTool[siToolCount].clItem );
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clTool[siToolCount].siPos, sizeof(pRecvMsg->clTool[siToolCount].siPos) );
		}
*/
		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siOutputPos, sizeof(pRecvMsg->siOutputPos) );
		SetItemParam( &pRecvMsg->clItem);
		m_odbc.SetParam( SQL_BIT, &(pRecvMsg->bCreateSID),	sizeof(pRecvMsg->bCreateSID) );
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->bCanPileSwitch, sizeof(pRecvMsg->bCanPileSwitch) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siMakerServerUnique, sizeof(pRecvMsg->siMakerServerUnique) );
	END_SP()

	EXEC_SQL()

	/* ����� ��ǰ�� ��� ó�� */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siMaterialPersonID, 0);
		m_odbc.GetData( &sendMsg.siOutputPos, 0);
		m_odbc.GetData( &sendMsg.siAddedNum, 0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siFee, 0 );
		m_odbc.GetData( &sendMsg.siPersonIDMoney, 0 );
		m_odbc.GetData( &sendMsg.siMaterialPersonIDMoney, 0 );
	END_FETCHDATA()

	sendMsg.bAlmostFailSwitch = pRecvMsg->bAlmostFailSwitch;
	sendMsg.siLuckQuality = pRecvMsg->siLuckQuality;
	sendMsg.siOrderNumber = pRecvMsg->siOrderNumber;
	sendMsg.siItemUnique = pRecvMsg->siItemUnique;
	sendMsg.bNPCMake = pRecvMsg->bNPCMake;
	sendMsg.m_bRequest_Want_Make_Item = pRecvMsg->m_bRequest_Want_Make_Item;
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, ItemPos = %d, Amount = %d, Luck = %d, Fee = %I64d, PersonMoney = %I64d, MaterialPersonMoney = %I64d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
				, pRecvMsg->clItem.GetRareType(), pRecvMsg->siOutputPos, sendMsg.siAddedNum, sendMsg.siLuckQuality, sendMsg.siFee, sendMsg.siPersonIDMoney, sendMsg.siMaterialPersonIDMoney
				, LOGITEMDISPLAY(sendMsg.clItem)
				);
		sDBLogMsg_QueryString LogMsg( "Make item", pRecvMsg->siPersonID, pRecvMsg->siMaterialPersonID, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	BEGIN_NEXTRECORDSET()
		/* �Ҹ�� ��ǰ�� ��� ó�� */
		// ���� ��Ŷ
		sDBResponse_InputItemToPerson sendMsg2;
		
		sendMsg2.packetHeader.usCmd = DBMSG_RESPONSE_INPUTITEMTOPERSON;
		sendMsg2.packetHeader.usSize = sizeof( sendMsg2 );
		sendMsg2.usCharID = pRecvMsg->usCharID;
		sendMsg2.siInputItemPersonOption = INPUTITEMPERSON_OPTION_MAKEITEM_MATERIAL;

		SI16 siUsedUnique = 0;
		SI16 siUsedNum = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg2.siResult,		0);
			m_odbc.GetData( &sendMsg2.siPersonID,	0);
			m_odbc.GetData( &sendMsg2.siItemPos,	0);
			m_odbc.GetData( &sendMsg2.siChangedNum,	0);
			GetItemData( &sendMsg2.clItem);
			m_odbc.GetData( &siUsedUnique, 0 );
			m_odbc.GetData( &siUsedNum, 0 );

			sendMsg2.clInputedItem.Init();
			sendMsg2.clInputedItem.siUnique = sendMsg2.clItem.siUnique;
			sendMsg2.clInputedItem.siItemNum = siUsedNum;		// ������ �Ҹ�� ������ ����

			// send
			SendPacket( (BYTE *)&sendMsg2 );
			// �α� �ۼ�
			if(sendMsg.siResult == 1)
			{
				char log[512] = "";
				sprintf(log, "ItemPos = %d, UseAmount = %d, AfterAmount = %d"
						, sendMsg2.siItemPos, siUsedNum, sendMsg2.clItem.siItemNum
						);
				sDBLogMsg_QueryString LogMsg( "Make item used", pRecvMsg->siPersonID, pRecvMsg->siMaterialPersonID, siUsedUnique, log);
				SendLogMsg((sPacketHeader *)&LogMsg);
			}
		END_FETCHDATA()
	END_NEXTRECORDSET()

/*
	BEGIN_NEXTRECORDSET()
		// ���� ������ ��� ó��
		// ���� ��Ŷ
		sDBResponse_InputItemToPerson sendMsg2;
		
		sendMsg2.packetHeader.usCmd = DBMSG_RESPONSE_INPUTITEMTOPERSON;
		sendMsg2.packetHeader.usSize = sizeof( sendMsg2 );
		sendMsg2.usCharID = pRecvMsg->usCharID;
		sendMsg2.siInputItemPersonOption = INPUTITEMPERSON_OPTION_NONE;

		SI16 siUsedUnique = 0;
		SI16 siUsedNum = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg2.siResult,		0);
			m_odbc.GetData( &sendMsg2.siPersonID,	0);
			m_odbc.GetData( &sendMsg2.siItemPos,	0);
			m_odbc.GetData( &sendMsg2.siChangedNum,	0);
			GetItemData( &sendMsg2.clItem);
			m_odbc.GetData( &siUsedUnique, 0 );
			m_odbc.GetData( &siUsedNum, 0 );

			// send
			SendPacket( (BYTE *)&sendMsg2 );
			// �α� �ۼ�
			if(sendMsg.siResult == 1)
			{
				char log[512] = "";
				sprintf(log, "ItemPos = %d, UseAmount = %d, AfterAmount = %d"
						, sendMsg2.siItemPos, siUsedNum, sendMsg2.clItem.siItemNum
						);
				sDBLogMsg_QueryString LogMsg( "Make item Tool", pRecvMsg->siPersonID, pRecvMsg->siMaterialPersonID, siUsedUnique, log);
				SendLogMsg((sPacketHeader *)&LogMsg);
			}
		END_FETCHDATA()
	END_NEXTRECORDSET()
*/
}

void DBGameProtocol::EnchantItem()
{
	// ���� ��Ŷ
	sDBRequest_Enchant *pRecvMsg = (sDBRequest_Enchant *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_Enchant sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ENCHANT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	
	// DB Query
	BEGIN_SP( "rsp_PersonItemEnchant" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromPos,		sizeof(pRecvMsg->siFromPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromItemUnique, sizeof(pRecvMsg->siFromItemUnique) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToPos,			sizeof(pRecvMsg->siToPos) );
		SetItemParam( &pRecvMsg->clToItem);
	END_SP()

	EXEC_SQL()

	/* ����� ��ǰ�� ��� ó�� */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult	,	0);
		m_odbc.GetData( &sendMsg.siPersonID	,	0);
		m_odbc.GetData( &sendMsg.siFromPos,		0);
		GetItemData( &sendMsg.clFromItem );
		m_odbc.GetData( &sendMsg.siToPos,		0);
		GetItemData( &sendMsg.clToItem );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		// �ϼ�ǰ
		char log[512] = "";
		sprintf(log, "ItemPos = %d, FromItemUnique = %d, FromItemPos = %d"
				, sendMsg.siFromPos, pRecvMsg->siFromItemUnique, pRecvMsg->siToPos, sendMsg.clFromItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg( "Enchant item", pRecvMsg->siPersonID, NULL, pRecvMsg->clToItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
	
	if(sendMsg.siResult == 1)
	{
		// �Ҹ�ǰ
		char log[512] = "";
		sprintf(log, "ItemPos = %d, FromItemUnique = %d, FromItemPos = %d, AfterItemAmount = %d"
				, sendMsg.siFromPos, pRecvMsg->siFromItemUnique, pRecvMsg->siToPos, sendMsg.clFromItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg( "Enchant item Used", pRecvMsg->siPersonID, NULL, pRecvMsg->siFromItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::EnchantItemInv()
{
	// ���� ��Ŷ
	sDBRequest_EnchantItemInv *pRecvMsg = (sDBRequest_EnchantItemInv *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_EnchantItemInv sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ENCHANTITEMINV;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siSuccess = pRecvMsg->siSuccess;

	bool bSuccess = pRecvMsg->siSuccess > 0 ? true : false ;

	// DB Query
	BEGIN_SP( "rsp_PersonItemEnchantInv" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMaterialPersonID, sizeof(pRecvMsg->siMaterialPersonID) );
		for( int siMaterialNumber=0; siMaterialNumber < MAX_ENCHANT_NUMBER; siMaterialNumber++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMaterial[siMaterialNumber].siStgPos, sizeof(pRecvMsg->clMaterial[siMaterialNumber].siStgPos) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clMaterial[siMaterialNumber].siItemUnique, sizeof(pRecvMsg->clMaterial[siMaterialNumber].siItemUnique) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clMaterial[siMaterialNumber].siUseItemNum, sizeof(pRecvMsg->clMaterial[siMaterialNumber].siUseItemNum) );
		}

		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->siOutputPos, sizeof(pRecvMsg->siOutputPos) );
		SetItemParam( &pRecvMsg->clItem);
		m_odbc.SetParam( SQL_BIT, &(pRecvMsg->bCreateSID),	sizeof(pRecvMsg->bCreateSID) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_TINYINT, &bSuccess, sizeof(bSuccess) );
	END_SP()

	EXEC_SQL()

	/* ����� ��ǰ�� ��� ó�� */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siMaterialPersonID, 0);
		m_odbc.GetData( &sendMsg.siOutputPos, 0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siFee, 0 );
		m_odbc.GetData( &sendMsg.siPersonIDMoney, 0 );
		m_odbc.GetData( &sendMsg.siMaterialPersonIDMoney, 0 );
	END_FETCHDATA()


	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Success = %d, RareType = %d, ItemPos = %d, PersonMoney = %I64d, MaterialPersonMoney = %I64d, Fee = %I64d, UseDay = %02d%02d%02d/%02d��, Dur = %d/%d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
			, pRecvMsg->siSuccess, pRecvMsg->clItem.GetRareType(), pRecvMsg->siOutputPos, sendMsg.siPersonIDMoney, sendMsg.siMaterialPersonIDMoney, sendMsg.siFee
			, USEDAYDISPLAY(pRecvMsg->clItem), pRecvMsg->clItem.GetDurability(), pRecvMsg->clItem.GetMaxDurability(), LOGITEMDISPLAY(pRecvMsg->clItem), ELEMENTDISPLAY(pRecvMsg->clItem)
			);
		sDBLogMsg_QueryString LogMsg( "Enchant item inv", pRecvMsg->siPersonID, pRecvMsg->siMaterialPersonID, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	BEGIN_NEXTRECORDSET()
		/* �Ҹ�� ��ǰ�� ��� ó�� */
		// ���� ��Ŷ
		sDBResponse_InputItemToPerson sendMsg2;

		sendMsg2.packetHeader.usCmd = DBMSG_RESPONSE_INPUTITEMTOPERSON;
		sendMsg2.packetHeader.usSize = sizeof( sendMsg2 );
		sendMsg2.usCharID = pRecvMsg->usCharID;
		sendMsg2.siInputItemPersonOption = INPUTITEMPERSON_OPTION_ENCHANT_MATERIAL;


		SI16 siUsedUnique = 0;
		SI16 siUsedNum = 0;
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg2.siResult,		0);
			m_odbc.GetData( &sendMsg2.siPersonID,	0);
			m_odbc.GetData( &sendMsg2.siItemPos,	0);
			m_odbc.GetData( &sendMsg2.siChangedNum,	0);
			GetItemData( &sendMsg2.clItem);
			m_odbc.GetData( &siUsedUnique, 0 );
			m_odbc.GetData( &siUsedNum, 0 );

			sendMsg2.clInputedItem.Init();
			sendMsg2.clInputedItem.siUnique = sendMsg2.clItem.siUnique;
			sendMsg2.clInputedItem.siItemNum = siUsedNum;		// ���� ������ �ٽ� �����Ѵ�.

			// send
			SendPacket( (BYTE *)&sendMsg2 );
			// �α� �ۼ�
			if(sendMsg.siResult == 1)
			{
				char log[512] = "";
				sprintf(log, "ItemPos = %d, UseAmount = %d, AfterAmount = %d"
					, sendMsg2.siItemPos, siUsedNum, sendMsg2.clItem.siItemNum
					);
				sDBLogMsg_QueryString LogMsg( "Enchant item used", pRecvMsg->siPersonID, pRecvMsg->siMaterialPersonID, siUsedUnique, log);
				SendLogMsg((sPacketHeader *)&LogMsg);
			}
		END_FETCHDATA()
	END_NEXTRECORDSET()
}

void DBGameProtocol::PrivateTrade()
{
	// ���� ��Ŷ
	sDBRequest_PrivateTrade *pRecvMsg = (sDBRequest_PrivateTrade *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PrivateTrade sendMsg;
	SI32 i = 0;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PRIVATETRADE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID1 = pRecvMsg->usCharID1;
	sendMsg.usCharID2 = pRecvMsg->usCharID2;

	for ( i=0; i<MAX_PRIVATETRADEORDER_ITEM_NUM; i++ )
	{
		sendMsg.LOG_clItem1[i].Set(&pRecvMsg->clItem1[i]);
		sendMsg.LOG_siItemOriPos1[i]	= pRecvMsg->siItemOriPos1[i];
		sendMsg.LOG_siItemPos1[i]		= pRecvMsg->siItemPos1[i];

		sendMsg.LOG_clItem2[i].Set(&pRecvMsg->clItem2[i]);
		sendMsg.LOG_siItemOriPos2[i]	= pRecvMsg->siItemOriPos2[i];
		sendMsg.LOG_siItemPos2[i]		= pRecvMsg->siItemPos2[i];
	}

	// DB Query
	BEGIN_SP( "rsp_PersonItemPrivateTrade" )
		// Person1 ////////////////////////////////////////////////////////////////////////////////////////////
		// ����
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID1,	sizeof(pRecvMsg->siPersonID1) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney1,		sizeof(pRecvMsg->siMoney1) );

		// ������
		for(i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; i++ )
		{
			SetItemParam( &pRecvMsg->clItem1[i] );
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemOriPos1[i], sizeof(pRecvMsg->siItemOriPos1[i]) );
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos1[i], sizeof(pRecvMsg->siItemPos1[i]) );
			m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch1[i], sizeof(pRecvMsg->bPileSwitch1[i]) );
		}

		// �ֽ�
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockVillageUnique1, sizeof(pRecvMsg->siStockVillageUnique1) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockNum1, sizeof(pRecvMsg->siStockNum1) );

		// ����
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseVillageUnique1, sizeof(pRecvMsg->siHouseVillageUnique1) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot1, sizeof(pRecvMsg->siHouseUnitSlot1) );

		// ��
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorse1, sizeof(pRecvMsg->siHorse1) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRecvHorse1, sizeof(pRecvMsg->siRecvHorse1) );


		// Person2 ////////////////////////////////////////////////////////////////////////////////////////////
		// ����
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID2,	sizeof(pRecvMsg->siPersonID2) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney2,		sizeof(pRecvMsg->siMoney2) );

		// ������
		for(i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; i++ )
		{
			SetItemParam( &pRecvMsg->clItem2[i] );
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemOriPos2[i], sizeof(pRecvMsg->siItemOriPos2[i]) );
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos2[i], sizeof(pRecvMsg->siItemPos2[i]) );
			m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch2[i], sizeof(pRecvMsg->bPileSwitch2[i]) );
		}

		// �ֽ�
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockVillageUnique2, sizeof(pRecvMsg->siStockVillageUnique2) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siStockNum2, sizeof(pRecvMsg->siStockNum2) );

		// ����
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseVillageUnique2, sizeof(pRecvMsg->siHouseVillageUnique2) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siHouseUnitSlot2, sizeof(pRecvMsg->siHouseUnitSlot2) );

		// ��
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorse2, sizeof(pRecvMsg->siHorse2) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siRecvHorse2, sizeof(pRecvMsg->siRecvHorse2) );

	END_SP()

	EXEC_SQL()

	GMONEY siPerson1Money = 0;
	GMONEY siPerson1ChangedMoney = 0;
	GMONEY siPerson2Money = 0;
	GMONEY siPerson2ChangedMoney = 0;

	/* �������� */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult,	0 );

		m_odbc.GetData( &sendMsg.siPersonID1, 0 );
		m_odbc.GetData( &siPerson1Money, 0 );
		m_odbc.GetData( &siPerson1ChangedMoney, 0 );

		m_odbc.GetData( &sendMsg.siPersonID2, 0 );
		m_odbc.GetData( &siPerson2Money, 0 );
		m_odbc.GetData( &siPerson2ChangedMoney, 0 );
	END_FETCHDATA()

	// �α� �ۼ� (����)
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "P1SendMoney = %I64d, P2SendMoney = %I64d, P1ChangedMoney = %I64d, P2ChangedMoney = %I64d, Person1Money = %I64d, Person2Money = %I64d"
				, pRecvMsg->siMoney1, pRecvMsg->siMoney2, siPerson1ChangedMoney, siPerson2ChangedMoney, siPerson1Money, siPerson2Money
				);
		sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	/* ����� ������ ���� */
	cltItem clItem[MAX_PRIVATETRADEORDER_ITEM_NUM*4]; // ������ �ϳ��� ����ɶ� ����޴� �κ��� ��ġ�� 2���̰� �ŷ��� �ֹ氣 �θ��̹Ƿ� ����� ������ �ִ� �ŷ����� �����ۼ� * 4�� �� ��� �Ѵ�.
	SI32 siPersonID[MAX_PRIVATETRADEORDER_ITEM_NUM*4];
	SI16 siPos[MAX_PRIVATETRADEORDER_ITEM_NUM*4];
	SI32 siResult = 0, siCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &siResult,	0 );
			if (siResult != 0)
			{
				m_odbc.GetData( &siPersonID[siCount], 0 );
				m_odbc.GetData( &siPos[siCount], 0 );
				GetItemData( &clItem[siCount] );
				siCount++;
			}
		END_FETCHDATA()
	END_NEXTRECORDSET()
	
	// �α� �ۼ� (������)
	if(sendMsg.siResult == 1 && siResult == 1)
	{
		char log[512] = "";
		for(i = 0; i < MAX_PRIVATETRADEORDER_ITEM_NUM; i++ )
		{
			if( pRecvMsg->clItem1[i].siUnique > 0 )
			{
				sprintf(log, "RareType = %d, FromItemPos = %d, ToItemPos = %d, Amount = %d"
						, pRecvMsg->clItem1[i].GetRareType(), pRecvMsg->siItemOriPos1[i], pRecvMsg->siItemPos1[i], pRecvMsg->clItem1[i].siItemNum
						);
				sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, pRecvMsg->clItem1[i].siUnique, log);
				SendLogMsg((sPacketHeader *)&LogMsg);
			}

			if( pRecvMsg->clItem2[i].siUnique > 0 )
			{
				sprintf(log, "RareType = %d, FromItemPos = %d, ToItemPos = %d, Amount = %d"
						, pRecvMsg->clItem2[i].GetRareType(), pRecvMsg->siItemOriPos2[i], pRecvMsg->siItemPos2[i], pRecvMsg->clItem2[i].siItemNum
						);
				sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID2, sendMsg.siPersonID1, pRecvMsg->clItem2[i].siUnique, log);
				SendLogMsg((sPacketHeader *)&LogMsg);
			}
		}
	}
	
	// �ֽ�
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siSendStockVillageUnique1, 0 );
			m_odbc.GetData( &sendMsg.siSendStockNum1, 0 );
			m_odbc.GetData( &sendMsg.siRecvStockVillageUnique1, 0 );
			m_odbc.GetData( &sendMsg.siRecvStockNum1, 0 );
			m_odbc.GetData( &sendMsg.siSendStockVillageUnique2, 0 );
			m_odbc.GetData( &sendMsg.siSendStockNum2, 0 );
			m_odbc.GetData( &sendMsg.siRecvStockVillageUnique2, 0 );
			m_odbc.GetData( &sendMsg.siRecvStockNum2, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siRecvedStockNumToSender1 = pRecvMsg->siStockNum2;
	sendMsg.siRecvedStockNumToSender2 = pRecvMsg->siStockNum1;

	if(sendMsg.siResult == 1 && (sendMsg.siSendStockVillageUnique1 > 0 || sendMsg.siSendStockVillageUnique2 > 0))
	{
		char log[512] = "";
		if( sendMsg.siSendStockVillageUnique1 > 0 )
		{
			sprintf(log, "�ֽİŷ� Name -> DstName, VillageUnique = %d, Amount = %d, LeftSenderAmount = %d, LeftReceiverAmount = %d"
				, sendMsg.siSendStockVillageUnique1, pRecvMsg->siStockNum1, sendMsg.siSendStockNum1, sendMsg.siRecvStockNum2
				);
			sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
		
		if( sendMsg.siSendStockVillageUnique2 > 0 )
		{
			sprintf(log, "�ֽİŷ� DstName -> Name, VillageUnique = %d, Amount = %d, LeftSenderAmount = %d, LeftReceiverAmount = %d"
				, sendMsg.siSendStockVillageUnique2, pRecvMsg->siStockNum2, sendMsg.siSendStockNum2, sendMsg.siRecvStockNum1
				);
			sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}

	// �ŷ��ϴ� ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siSendHouseVillageUnique1, 0 );
			m_odbc.GetData( &sendMsg.siSendHouseUnitSlot1, 0 );
			m_odbc.GetData( &sendMsg.siSendHouseVillageUnique2, 0 );
			m_odbc.GetData( &sendMsg.siSendHouseUnitSlot2, 0 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// Person1�� ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siRecvHouseVillageUnique1, 0 );
			m_odbc.GetData( &sendMsg.siRecvHouseUnitSlot1, 0 );
			m_odbc.GetData( sendMsg.Person1Name, sizeof(sendMsg.Person1Name) );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// Person2�� ����
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siRecvHouseVillageUnique2, 0 );
			m_odbc.GetData( &sendMsg.siRecvHouseUnitSlot2, 0 );
			m_odbc.GetData( sendMsg.Person2Name, sizeof(sendMsg.Person2Name) );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	if(sendMsg.siResult == 1 && ( sendMsg.siSendHouseVillageUnique1 > 0 || sendMsg.siSendHouseVillageUnique2 > 0 ) )
	{
		char log[512] = "";
		if( sendMsg.siSendHouseVillageUnique1 > 0 )
		{
			sprintf(log, "�����ŷ� Name -> DstName, VillageUnique = %d, HouseUnitSlot = %d"
					, sendMsg.siSendHouseVillageUnique1, sendMsg.siSendHouseUnitSlot1
					);
			sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}

		if( sendMsg.siSendHouseVillageUnique2 > 0 )
		{
			sprintf(log, "�����ŷ� DstName -> Name, VillageUnique = %d, HouseUnitSlot = %d"
					, sendMsg.siSendHouseVillageUnique2, sendMsg.siSendHouseUnitSlot2
					);
			sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}

	// Person1�� ��
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siRecvHorse1, 0 );
			GetHorseInfo( &sendMsg.clRecvHorse1 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// Person2�� ��
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siRecvHorse2, 0 );
			GetHorseInfo( &sendMsg.clRecvHorse2 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	if(sendMsg.siResult == 1 && ( pRecvMsg->siHorse1 >= 0 || pRecvMsg->siHorse2 >= 0 ) )
	{
		char log[512] = "";
		if( pRecvMsg->siHorse1 >= 0 )
		{
			sprintf(log, "���ŷ� Name Index %d -> DstName Index %d, Name = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
					, sendMsg.siRecvHorse1, sendMsg.siRecvHorse2, sendMsg.clRecvHorse2.szName, sendMsg.clRecvHorse2.siHorseUnique, sendMsg.clRecvHorse2.siStr, sendMsg.clRecvHorse2.siDex, sendMsg.clRecvHorse2.siMag, sendMsg.clRecvHorse2.siMoveSpeed
					);
			sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}

		if( pRecvMsg->siHorse2 >= 0 )
		{
			sprintf(log, "���ŷ� DstName Index %d -> Name Index %d, Name = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
					, sendMsg.siRecvHorse2, sendMsg.siRecvHorse1, sendMsg.clRecvHorse1.szName, sendMsg.clRecvHorse1.siHorseUnique, sendMsg.clRecvHorse1.siStr, sendMsg.clRecvHorse1.siDex, sendMsg.clRecvHorse1.siMag, sendMsg.clRecvHorse1.siMoveSpeed
					);
			sDBLogMsg_QueryString LogMsg( "Private Trade", sendMsg.siPersonID1, sendMsg.siPersonID2, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}

	sendMsg.siHorse1 = pRecvMsg->siHorse1;
	sendMsg.siHorse2 = pRecvMsg->siHorse2;
	sendMsg.siRecvHorse1 = pRecvMsg->siRecvHorse1;
	sendMsg.siRecvHorse2 = pRecvMsg->siRecvHorse2;

	//send
	SendPacket( (BYTE *)&sendMsg );


	// Person1�� ���������� ������.
	sDBResponse_ChangeMoney sendMoneyMsg1;
	
	sendMoneyMsg1.packetHeader.usCmd = DBMSG_RESPONSE_CHANGEMONEY;
	sendMoneyMsg1.packetHeader.usSize = sizeof( sendMoneyMsg1 );
	sendMoneyMsg1.usCharID = pRecvMsg->usCharID1;

	sendMoneyMsg1.siResult			= sendMsg.siResult;
	sendMoneyMsg1.siPersonID		= sendMsg.siPersonID1;
	sendMoneyMsg1.siReason			= CHANGE_MONEY_REASON_TRADE;
	sendMoneyMsg1.siChangedMoney	= siPerson1ChangedMoney;
	sendMoneyMsg1.siMoney			= siPerson1Money;
	sendMoneyMsg1.siTargetPersonID	= sendMsg.siPersonID2;

	/* ����� ���� �������� ������. */
	if( sendMoneyMsg1.siChangedMoney != 0 )
		SendPacket( (BYTE *)&sendMoneyMsg1 );


	// Person1�� ���������� ������.
	sDBResponse_ChangeMoney sendMoneyMsg2;
	
	sendMoneyMsg2.packetHeader.usCmd = DBMSG_RESPONSE_CHANGEMONEY;
	sendMoneyMsg2.packetHeader.usSize = sizeof( sendMoneyMsg2 );
	sendMoneyMsg2.usCharID = pRecvMsg->usCharID2;

	sendMoneyMsg2.siResult			= sendMsg.siResult;
	sendMoneyMsg2.siPersonID		= sendMsg.siPersonID2;
	sendMoneyMsg2.siReason			= CHANGE_MONEY_REASON_TRADE;
	sendMoneyMsg2.siChangedMoney	= siPerson2ChangedMoney;
	sendMoneyMsg2.siMoney			= siPerson2Money;
	sendMoneyMsg2.siTargetPersonID	= sendMsg.siPersonID1;

	/* ����� ���� �������� ������. */
	if( sendMoneyMsg2.siChangedMoney != 0 )
		SendPacket( (BYTE *)&sendMoneyMsg2 );


	// ����� �κ��丮�� ������ ������.
	sDBResponse_SetItem sendItemMsg;
	sendItemMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETITEM;
	sendItemMsg.packetHeader.usSize = sizeof( sendItemMsg );
//	sendItemMsg.usCharID = pRecvMsg->usCharID2;
	for( siCount = 0; siCount < MAX_PRIVATETRADEORDER_ITEM_NUM*4; siCount++ )
	{
		if (siPersonID[siCount] == 0)
			continue;
		sendItemMsg.siResult = 1;
		sendItemMsg.usCharID = (siPersonID[siCount] == pRecvMsg->siPersonID1) ? pRecvMsg->usCharID1: pRecvMsg->usCharID2;
		sendItemMsg.siPersonID = siPersonID[siCount];
		sendItemMsg.siPos = siPos[siCount];
		sendItemMsg.clItem.Set(&clItem[siCount]);

		SendPacket( (BYTE *)&sendItemMsg );

	}

}


void DBGameProtocol::WarpVillage()
{
	// ���� ��Ŷ
	sDBRequest_WarpVillage *pRecvMsg = (sDBRequest_WarpVillage *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_WarpVillage sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WARPVILLAGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonItemWarpVillage" )
		m_odbc.SetParam( SQL_INTEGER, &(pRecvMsg->siPersonID),		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemPos),		sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &(pRecvMsg->siItemUnique),	sizeof(pRecvMsg->siItemUnique) );
	END_SP()

	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()
	
	sendMsg.siFromVillageUnique	= pRecvMsg->siFromVillageUnique;
	sendMsg.siToVillageUnique	= pRecvMsg->siToVillageUnique;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ItemPos = %d, LeftItemNum = %d, FromVillage = %d, ToVillage = %d"
				, pRecvMsg->siItemPos, sendMsg.clItem.siItemNum, sendMsg.siFromVillageUnique, sendMsg.siToVillageUnique
				);
		sDBLogMsg_QueryString LogMsg( "Warp Village", sendMsg.siPersonID, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::UpgradeItem()
{
	// ���� ��Ŷ
	sDBRequest_UpgradeItem *pRecvMsg = (sDBRequest_UpgradeItem*)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_UpgradeItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_UPGRADEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.bSuccess = pRecvMsg->bSuccess;
	UI08 uiSuccess = (pRecvMsg->clToItem.GetRareType() == RARELEVEL_1 && pRecvMsg->bSuccess == true) ? 1: 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemUpgrade" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromPos,		sizeof(pRecvMsg->siFromPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siFromItemUnique, sizeof(pRecvMsg->siFromItemUnique) );

		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToPos,			sizeof(pRecvMsg->siToPos) );
		SetItemParam( &pRecvMsg->clToItem);
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siAddCityHallMoney, sizeof(pRecvMsg->siAddCityHallMoney) );
		m_odbc.SetParam( SQL_TINYINT, &uiSuccess, sizeof(uiSuccess) );
	END_SP()

	EXEC_SQL()

	/* ����� ��ǰ�� ��� ó�� */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult	,	0);
		m_odbc.GetData( &sendMsg.siPersonID	,	0);
		m_odbc.GetData( &sendMsg.siFromPos,		0);
		GetItemData( &sendMsg.clFromItem );
		m_odbc.GetData( &sendMsg.siToPos,		0);
		GetItemData( &sendMsg.clToItem );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.clMoney.itMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, RareType = %d, Dur = %d/%d, PaperItemPos = %d, PaperItemUnique = %d, AfterPaperAmount = %d, UpgradeItemPos = %d, VillageUnique = %d, InputCityHallMoney = %I64d, LeftCityHallMoney = %I64d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
				, sendMsg.siResult, pRecvMsg->clToItem.GetRareType(), pRecvMsg->clToItem.GetDurability(), pRecvMsg->clToItem.GetMaxDurability(), sendMsg.siFromPos, sendMsg.clFromItem.siItemNum, pRecvMsg->siToPos, pRecvMsg->siVillageUnique, pRecvMsg->siAddCityHallMoney, sendMsg.clMoney.itMoney
				, LOGITEMDISPLAY(pRecvMsg->clToItem)
				);
		sDBLogMsg_QueryString LogMsg( "Upgrade item", pRecvMsg->siPersonID, NULL, pRecvMsg->clToItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, RareType = %d, Dur = %d/%d, PaperItemPos = %d, PaperItemUnique = %d, AfterPaperAmount = %d, UpgradeItemPos = %d, VillageUnique = %d, InputCityHallMoney = %I64d, LeftCityHallMoney = %I64d"
				, sendMsg.siResult, pRecvMsg->clToItem.GetRareType(), pRecvMsg->clToItem.GetDurability(), pRecvMsg->clToItem.GetMaxDurability(), sendMsg.siFromPos, sendMsg.clFromItem.siItemNum, pRecvMsg->siToPos, pRecvMsg->siVillageUnique, pRecvMsg->siAddCityHallMoney, sendMsg.clMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg( "Upgrade item Used", pRecvMsg->siPersonID, NULL, pRecvMsg->siFromItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// ���λ������� ��ǰ�� �����Ѵ�.
void DBGameProtocol::PersonalShopBuyItem()
{
	// ���� ��Ŷ
	sDBRequest_PersonalShopBuyItem *pRecvMsg = (sDBRequest_PersonalShopBuyItem*)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PersonalShopBuyItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONALSHOPBUYITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siShopperCharID = pRecvMsg->siShopperCharID;
	sendMsg.siShowCaseIndex = pRecvMsg->siShowCaseIndex;
	sendMsg.siItemCost = pRecvMsg->siItemCost;
	sendMsg.siBuyItemNum = pRecvMsg->clBuyItem.siItemNum;

	//[�߰� : Ȳ���� 2007. 10. 24 �α׿� �̿�. Buyer�� �����ϰ� ����̴� ������]
	sendMsg.clBuyItem.Set(&pRecvMsg->clBuyItem);

	// DB Query
	BEGIN_SP( "rsp_PersonShopBuyItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siBuyerPersonID, sizeof(pRecvMsg->siBuyerPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siShopperPersonID, sizeof(pRecvMsg->siShopperPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siBuyerInvPos, sizeof(pRecvMsg->siBuyerInvPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siShopperInvPos, sizeof(pRecvMsg->siShopperInvPos) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siItemCost, sizeof(pRecvMsg->siItemCost) );
		SetItemParam( &pRecvMsg->clBuyItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch, sizeof(pRecvMsg->bPileSwitch) );
	END_SP()

	EXEC_SQL()

	GMONEY siBeforeBuyerMoney = 0;
	GMONEY siBeforeShopperMoney = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siBuyerPersonID, 0);
		m_odbc.GetData( &sendMsg.siShopperPersonID, 0 );
		m_odbc.GetData( &sendMsg.siBuyerInvPos, 0 );
		m_odbc.GetData( &sendMsg.siShopperInvPos, 0 );
		GetItemData( &sendMsg.clBuyerInvItem );
		GetItemData( &sendMsg.clShopperInvItem );
		m_odbc.GetData( &sendMsg.siBuyerMoney, 0 );
		m_odbc.GetData( &sendMsg.siShopperMoney, 0 );
		m_odbc.GetData( &siBeforeBuyerMoney, 0 );
		m_odbc.GetData( &siBeforeShopperMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, ShowCaseIndex = %d, Amount = %d, BuyerInvPos = %d, AfterBuyerItemNum = %d, ShopperInvPos = %d, AfterShopperItemNum = %d, ItemCost = %I64d, BuyerMoney(Before/After) = %I64d/%I64d, ShopperMoney(Before/After) = %I64d/%I64d"
				, sendMsg.clBuyerInvItem.GetRareType(), sendMsg.siShowCaseIndex, sendMsg.siBuyItemNum, sendMsg.siBuyerInvPos, sendMsg.clBuyerInvItem.siItemNum
				, sendMsg.siShopperInvPos, sendMsg.clShopperInvItem.siItemNum, sendMsg.siItemCost
				, siBeforeBuyerMoney, sendMsg.siBuyerMoney, siBeforeShopperMoney, sendMsg.siShopperMoney
				);
		sDBLogMsg_QueryString LogMsg( "PersonalShop Buy Item", sendMsg.siBuyerPersonID, sendMsg.siShopperPersonID, sendMsg.clBuyerInvItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// ���λ������� ��ǰ�� �����Ѵ�.
void DBGameProtocol::PersonalShopSellItem()
{
	// ���� ��Ŷ
	sDBRequest_PersonalShopSellItem *pRecvMsg = (sDBRequest_PersonalShopSellItem*)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PersonalShopSellItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERSONALSHOPSELLITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siShopperCharID = pRecvMsg->siShopperCharID;

	sendMsg.siShowCaseIndex = pRecvMsg->siShowCaseIndex;
	sendMsg.siItemCost = pRecvMsg->siItemCost;

	sendMsg.siSellItemNum = pRecvMsg->clSellItem.siItemNum;
	sendMsg.clSellerInvItem_Log.Set(&pRecvMsg->clSellItem);

	// DB Query
	BEGIN_SP( "rsp_PersonShopSellItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSellerPersonID, sizeof(pRecvMsg->siSellerPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siShopperPersonID, sizeof(pRecvMsg->siShopperPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSellerInvPos, sizeof(pRecvMsg->siSellerInvPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siShopperInvPos, sizeof(pRecvMsg->siShopperInvPos) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siItemCost, sizeof(pRecvMsg->siItemCost) );
		SetItemParam( &pRecvMsg->clSellItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch, sizeof(pRecvMsg->bPileSwitch) );
	END_SP()

	EXEC_SQL()

	GMONEY siBeforeSellerMoney = 0;
	GMONEY siBeforeShopperMoney = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siSellerPersonID, 0);
		m_odbc.GetData( &sendMsg.siShopperPersonID, 0 );
		m_odbc.GetData( &sendMsg.siSellerInvPos, 0 );
		m_odbc.GetData( &sendMsg.siShopperInvPos, 0 );
		GetItemData( &sendMsg.clSellerInvItem );
		GetItemData( &sendMsg.clShopperInvItem );
		m_odbc.GetData( &sendMsg.siSellerMoney, 0 );
		m_odbc.GetData( &sendMsg.siShopperMoney, 0 );
		m_odbc.GetData( &siBeforeSellerMoney, 0 );
		m_odbc.GetData( &siBeforeShopperMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, ShowCaseIndex = %d, Amount = %d, SellerInvPos = %d, AfterSellerItemNum = %d, ShopperInvPos = %d, AfterShopperItemNum = %d, ItemCost = %I64d, SellerMoney(Before/After) = %I64d/%I64d, ShopperMoney(Before/After) = %I64d/%I64d"
				, sendMsg.clShopperInvItem.GetRareType(), sendMsg.siShowCaseIndex, sendMsg.siSellItemNum, sendMsg.siSellerInvPos, sendMsg.clSellerInvItem.siItemNum
				, sendMsg.siShopperInvPos, sendMsg.clShopperInvItem.siItemNum, sendMsg.siItemCost
				, siBeforeSellerMoney, sendMsg.siSellerMoney, siBeforeShopperMoney, sendMsg.siShopperMoney
				);
		sDBLogMsg_QueryString LogMsg( "PersonalShop Sell Item", sendMsg.siSellerPersonID, sendMsg.siShopperPersonID, sendMsg.clShopperInvItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

// Person�� ����ǰ���� ���λ��� �̿���� �Ҹ��Ѵ�.
void DBGameProtocol::StartPersonalShop()
{
	// ���� ��Ŷ
	sDBRequest_StartPersonalShop *pRecvMsg = (sDBRequest_StartPersonalShop*)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_StartPersonalShop sendMsg;
	
	sendMsg.packetHeader.usCmd		= DBMSG_RESPONSE_STARTPERSONALSHOP;
	sendMsg.packetHeader.usSize		= sizeof( sendMsg );
	sendMsg.siCharID				= pRecvMsg->siCharID;

	// �α׿�
	sendMsg.clItem_Log.siUnique		= pRecvMsg->siItemUnique;
	sendMsg.clItem_Log.siItemNum	= pRecvMsg->siItemNum;

	// �����
	sendMsg.m_siShopMode			= pRecvMsg->clShopData.m_siShopMode	;
	
	// DB Query
	BEGIN_SP( "rsp_PersonShopStart" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult	,	0);
		m_odbc.GetData( &sendMsg.siPersonID	,	0);
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, ItemPos = %d, AfterItemNum = %d, Unique/Price = %d/%I64d, Unique/Price = %d/%I64d, Unique/Price = %d/%I64d, Unique/Price = %d/%I64d, Unique/Price = %d/%I64d"
				, sendMsg.siVillageUnique, sendMsg.siItemPos, sendMsg.clItem.siItemNum
				, pRecvMsg->clShopData.m_clItem[0].siUnique, pRecvMsg->clShopData.m_siPrice[0]
				, pRecvMsg->clShopData.m_clItem[1].siUnique, pRecvMsg->clShopData.m_siPrice[1]
				, pRecvMsg->clShopData.m_clItem[2].siUnique, pRecvMsg->clShopData.m_siPrice[2]
				, pRecvMsg->clShopData.m_clItem[3].siUnique, pRecvMsg->clShopData.m_siPrice[3]
				, pRecvMsg->clShopData.m_clItem[4].siUnique, pRecvMsg->clShopData.m_siPrice[4]
				);
		sDBLogMsg_QueryString LogMsg( "Start PersonalShop", sendMsg.siPersonID, NULL, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
	
}

void DBGameProtocol::FishingGetMarineProducts()
{
	// ���� ��Ŷ
	sDBRequest_FishingGetMarineProducts *pRecvMsg = (sDBRequest_FishingGetMarineProducts *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_FishingGetMarineProducts sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FISHINGGETMARINEPRODUCTS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siKind = pRecvMsg->siKind;
	sendMsg.siCharUnique = pRecvMsg->siCharUnique;
	sendMsg.clAcquireMarineProductItem.Set(&pRecvMsg->clAcquireMarineProductItem);

	SI32 siToolCount = 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemFishing" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siAcquireMarineInvPos, sizeof(pRecvMsg->siAcquireMarineInvPos) );
		SetItemParam( &pRecvMsg->clAcquireMarineProductItem );
		m_odbc.SetParam( SQL_BIT, &(pRecvMsg->bCreateSID),	sizeof(pRecvMsg->bCreateSID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siSkillUnique, sizeof(pRecvMsg->siSkillUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIncFishingSkillExp, sizeof(pRecvMsg->siIncFishingSkillExp) );
	END_SP()

	EXEC_SQL()

	/* ����� ��ǰ�� ��� ó�� */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siAcquireMarineInvPos,0);
		m_odbc.GetData( &sendMsg.siAddedItemNum, 0);
		GetItemData( &sendMsg.clLeftItem );
		m_odbc.GetData( &sendMsg.siIncSkillUnique, 0 );
		m_odbc.GetData( &sendMsg.siSkillTotalExp, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ItemPos = %d, Amount = %d"
				, pRecvMsg->siAcquireMarineInvPos, sendMsg.siAddedItemNum
				);
		sDBLogMsg_QueryString LogMsg( "Fishing", pRecvMsg->siPersonID, NULL, pRecvMsg->clAcquireMarineProductItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::FishingDecreaseToolDurability()
{
	// ���� ��Ŷ
	sDBRequest_FishingDecreaseToolDurability *pRecvMsg = (sDBRequest_FishingDecreaseToolDurability *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_FishingDecreaseToolDurability sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_FISHINGDECREASETOOLDURABILITY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	SI32 siToolCount = 0;
	SI32 siUsedUnique = 0;
	SI32 siBeforeAmount = 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemFishingDecToolDur" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		for( siToolCount = 0; siToolCount < MAX_FISHING_TOOL_NUMBER; siToolCount++)
		{
			SetItemParam( &pRecvMsg->clFishingTool[siToolCount].clItem );
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clFishingTool[siToolCount].siPos, sizeof(pRecvMsg->clFishingTool[siToolCount].siPos) );
		}
	END_SP()

	EXEC_SQL()

	siToolCount = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.clFishingTool[siToolCount].siPos, 0 );
		GetItemData( &sendMsg.clFishingTool[siToolCount].clItem );
		m_odbc.GetData( &siUsedUnique, 0 );
		m_odbc.GetData( &siBeforeAmount, 0 );
		// �α� �ۼ�
		if(sendMsg.siResult == 1)
		{
			char log[512] = "";
			sprintf(log, "InvPos = %d, BeforeAmount, AfterAmount = %d"
					, sendMsg.clFishingTool[siToolCount].siPos, siBeforeAmount, sendMsg.clFishingTool[siToolCount].clItem.siItemNum
					);
			sDBLogMsg_QueryString LogMsg( "Fishing tool dur decrease", pRecvMsg->siPersonID, NULL, siUsedUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
		siToolCount++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PerformanceUpgradeItem()
{
	// ���� ��Ŷ
	sDBRequest_PerformanceUpgradeItem *pRecvMsg = (sDBRequest_PerformanceUpgradeItem *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_PerformanceUpgradeItem sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_PERFORMANCEUPGRADEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	// DB Query
	BEGIN_SP( "rsp_PersonItemPerformanceUpgrade" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clPreItem.siUnique, sizeof(pRecvMsg->clPreItem.siUnique) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siScrollItemPos, sizeof(pRecvMsg->siScrollItemPos) );
		SetItemParam( &pRecvMsg->clScrollItem );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siLeftScrollItemPos, 0 );
		GetItemData( &sendMsg.clLeftScrollItem );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, Dur = %d/%d, ItemPos = %d, BeforeUnique = %d, ScrollPos = %d, ScrollUnique = %d, UseScrollNum = %d, AfterScrollNum = %d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
			, sendMsg.clItem.GetRareType(), sendMsg.clItem.GetDurability(), sendMsg.clItem.GetMaxDurability(), sendMsg.siItemPos, pRecvMsg->clPreItem.siUnique, sendMsg.siLeftScrollItemPos
			, pRecvMsg->clScrollItem.siUnique, pRecvMsg->clScrollItem.siItemNum, sendMsg.clLeftScrollItem.siItemNum
			, LOGITEMDISPLAY(pRecvMsg->clItem)
			);
		sDBLogMsg_QueryString LogMsg( "PersonItem PerformanceUpgrade", pRecvMsg->siPersonID, NULL, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "RareType = %d, Dur = %d/%d, ItemPos = %d, BeforeUnique = %d, ScrollPos = %d, ScrollUnique = %d, UseScrollNum = %d, AfterScrollNum = %d"
			, sendMsg.clItem.GetRareType(), sendMsg.clItem.GetDurability(), sendMsg.clItem.GetMaxDurability(), sendMsg.siItemPos, pRecvMsg->clPreItem.siUnique, sendMsg.siLeftScrollItemPos
			, pRecvMsg->clScrollItem.siUnique, pRecvMsg->clScrollItem.siItemNum, sendMsg.clLeftScrollItem.siItemNum
			);
		sDBLogMsg_QueryString LogMsg( "PersonItem PerformanceUpgrade Used", pRecvMsg->siPersonID, NULL, pRecvMsg->clScrollItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::AgricultureGetProducts()
{
	// ���� ��Ŷ
	sDBRequest_AgricultureGetProducts *pRecvMsg = (sDBRequest_AgricultureGetProducts *)m_pPacket;
	
	// ���� ��Ŷ
	sDBResponse_AgricultureGetProducts sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_AGRICULTUREGETPRODUCTS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siKind = pRecvMsg->siKind;
	sendMsg.siCharUnique = pRecvMsg->siCharUnique;
	sendMsg.clGetProductsItem.Set(&pRecvMsg->clGetProductsItem);
	
	SI32 siToolCount = 0;

	// DB Query
	BEGIN_SP( "rsp_PersonItemAgriculture" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siGetProductsInvPos, sizeof(pRecvMsg->siGetProductsInvPos) );
		SetItemParam( &pRecvMsg->clGetProductsItem );
	END_SP()

	EXEC_SQL()

	/* ����� ��ǰ�� ��� ó�� */
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.siAddedInvPos,0);
		m_odbc.GetData( &sendMsg.siAddedItemNum, 0);
		GetItemData( &sendMsg.clLeftItem );
	END_FETCHDATA()

	sendMsg.siCharUnique = pRecvMsg->siCharUnique;
	sendMsg.siKind = pRecvMsg->siKind;
	sendMsg.siAgricultureType = pRecvMsg->siAgricultureType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Kind = %d, ItemPos = %d, Amount = %d, AfterItemNum = %d"
				, pRecvMsg->siKind, pRecvMsg->siGetProductsInvPos, sendMsg.siAddedItemNum, sendMsg.clLeftItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg( "PersonItem Agriculture", pRecvMsg->siPersonID, NULL, pRecvMsg->clGetProductsItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::CountAttackInfo()
{
	// ���� ��Ŷ
	sDBRequest_CountAttack_Info *pRecvMsg = (sDBRequest_CountAttack_Info *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_CountAttack_Info sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_COUNTATTACK_INFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonItemCountAttackInfo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		SetItemParam( &pRecvMsg->clitem );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		GetItemData(&sendMsg.clitem);
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		m_odbc.GetData( &sendMsg.siFlagNum, 0 );
		m_odbc.GetData( &sendMsg.siBeforeNum, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	char log[512] = "";
	sprintf(log, "ItemPos = %d, UseNum = %d, FlagNum = %d, BeforeFlagNum = %d"
		, pRecvMsg->siItemPos, pRecvMsg->clitem.siItemNum, sendMsg.siFlagNum, sendMsg.siBeforeNum
		);
	sDBLogMsg_QueryString LogMsg("CountAttack Info", pRecvMsg->siPersonID, NULL, pRecvMsg->clitem.siUnique, log);
	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::PremiumSet()
{
	// ���� ��Ŷ
	sDBRequest_SetPremium *pRecvMsg = (sDBRequest_SetPremium *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SetPremium sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETPREMIUM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonItemPremiumSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPremium, sizeof(pRecvMsg->bPremium) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPremiumUseDate, sizeof(pRecvMsg->siPremiumUseDate) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siCombatUseDate, sizeof(pRecvMsg->siCombatUseDate) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMakeItemServiceDate, sizeof(pRecvMsg->siMakeItemServiceDate) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siBulletServiceDate, sizeof(pRecvMsg->siBulletServiceDate) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSummonStaminaServiceDate, sizeof(pRecvMsg->siSummonStaminaServiceDate) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siType, 0 );
		m_odbc.GetData( &sendMsg.bPremium, 0 );
		m_odbc.GetData( &sendMsg.siPremiumUseDate, 0 );
		m_odbc.GetData( &sendMsg.siCombatUseDate, 0 );
		m_odbc.GetData( &sendMsg.siMakeItemDate, 0 );
		m_odbc.GetData( &sendMsg.siBulletDate, 0 );
		m_odbc.GetData( &sendMsg.siSummonStaminaDate, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData(&sendMsg.clItem);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	char log[512] = "";
	sprintf(log, "Result = %d, Type = %d, bPremium = %d, PremiumDate = %d, CombatDate = %d, MakeItemServiceDate = %d, BulletDate = %d, SummonStaminaDate = %d, ItemPos = %d, Amount = %d, UseDay = %02d%02d%02d/%02d��"
		, sendMsg.siResult, sendMsg.siType, sendMsg.bPremium, sendMsg.siPremiumUseDate, sendMsg.siCombatUseDate, sendMsg.siMakeItemDate, sendMsg.siBulletDate, sendMsg.siSummonStaminaDate, pRecvMsg->siItemPos, pRecvMsg->clItem.siItemNum, USEDAYDISPLAY(pRecvMsg->clItem)
		);
	sDBLogMsg_QueryString LogMsg("Premium Set", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
	SendLogMsg((sPacketHeader *)&LogMsg);
}

void DBGameProtocol::WennyOldManInvList()
{
	// ���� ��Ŷ
	sDBRequest_Wennyoldman_InventoryList *pRecvMsg = (sDBRequest_Wennyoldman_InventoryList *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Wennyoldman_InventoryList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WENNYOLDMAN_INVENTORYLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonItemWennyOldManInvList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	SI32 siStgPos = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &siStgPos, 0 ); 

		if(siStgPos >= MAX_ITEM_PER_WENNYOLD) 
			continue;

		GetItemData( &sendMsg.clItemList[siStgPos]);
	END_FETCHDATA()

	sendMsg.siResult = 1;
	sendMsg.siPersonID = pRecvMsg->siPersonID;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::WennyOldManSaveInv()
{
	// ���� ��Ŷ
	sDBRequest_Wennyoldman_SaveInventory *pRecvMsg = (sDBRequest_Wennyoldman_SaveInventory *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Wennyoldman_SaveInventory sendMsg;

	sendMsg.packetHeader.usCmd		= DBMSG_RESPONSE_WENNYOLDMAN_SAVEINVENTORY;
	sendMsg.packetHeader.usSize		= sizeof( sendMsg );
	sendMsg.usCharID				= pRecvMsg->usCharID;
	sendMsg.siUsedItemUnique		= pRecvMsg->siUsedItemUnique;
	sendMsg.siUsedItemNum			= pRecvMsg->siUsedItemNum;
	sendMsg.clUseItem_Log.Set(&pRecvMsg->clUseItem);
	sendMsg.siDurabilityPenalty		= pRecvMsg->siDurabilityPenalty;

	// DB Query
	BEGIN_SP( "rsp_PersonItemWennyOldManSaveInv" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siDisassembleItemPos, sizeof(pRecvMsg->siDisassembleItemPos) );
		SetItemParam( &pRecvMsg->clUseItem);
		for(SI32 i = 0; i < MAX_ITEM_PER_WENNYOLD; i++)
		{
			SetItemParam( &pRecvMsg->clItemList[i] );
			m_odbc.SetParam( SQL_BIT, &pRecvMsg->bCreateSID[i], sizeof(pRecvMsg->bCreateSID[i]) );
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siDisassembleItemPos, 0 );
		GetItemData( &sendMsg.clUseItem );
	END_FETCHDATA()

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ItemPos = %d, ItemNum = %d, Useay = %02d%02d%02d/%02d��, Dur = %d/%d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
			, sendMsg.siDisassembleItemPos, sendMsg.clUseItem.siItemNum, USEDAYDISPLAY(sendMsg.clUseItem), sendMsg.clUseItem.GetDurability(), sendMsg.clUseItem.GetMaxDurability(), LOGITEMDISPLAY(sendMsg.clUseItem)
			);
		sDBLogMsg_QueryString LogMsg("WennyOldMan UseItem", pRecvMsg->siPersonID, NULL, pRecvMsg->clUseItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}


	SI32 siStgPos = 0;
	if(sendMsg.siResult == 1 || sendMsg.siResult == -100)
	{
		BEGIN_NEXTRECORDSET()
			BEGIN_FETCHDATA()
				m_odbc.GetData( &siStgPos, 0 ); if(siStgPos >= MAX_ITEM_PER_WENNYOLD) continue;
				GetItemData( &sendMsg.clItemList[siStgPos]);

				if(sendMsg.clItemList[siStgPos].siUnique != 0)
				{
					char log[512] = "";
					sprintf(log, "StgPos = %d, ItemNum = %d, UseDay = %02d%02d%02d/%02d��, Dur = %d/%d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
						, siStgPos, sendMsg.clItemList[siStgPos].siItemNum, USEDAYDISPLAY(sendMsg.clItemList[siStgPos]), sendMsg.clItemList[siStgPos].GetDurability(), sendMsg.clItemList[siStgPos].GetMaxDurability(), LOGITEMDISPLAY(sendMsg.clItemList[siStgPos])
						);
					sDBLogMsg_QueryString LogMsg("WennyOldMan SaveInv", pRecvMsg->siPersonID, NULL, sendMsg.clItemList[siStgPos].siUnique, log);
					SendLogMsg((sPacketHeader *)&LogMsg);
				}

			END_FETCHDATA()
		END_NEXTRECORDSET()
	}
	sendMsg.siWennyPercent = pRecvMsg->siWennyPercent;
	sendMsg.siResolutionRate = pRecvMsg->siResolutionRate;

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::WennyOldManMoveStgToInv()
{
	// ���� ��Ŷ
	sDBRequest_Wennyoldman_MoveItemStgToInv *pRecvMsg = (sDBRequest_Wennyoldman_MoveItemStgToInv *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Wennyoldman_MoveItemStgToInv sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_WENNYOLDMAN_MOVEITEMSTGTOINV;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.clItem_Log.Set(&pRecvMsg->clItem);

	// DB Query
	BEGIN_SP( "rsp_PersonItemWennyOldManStgToInv" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgPos, sizeof(pRecvMsg->siStgPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siInvPos, sizeof(pRecvMsg->siInvPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bCanPileSwitch, sizeof(pRecvMsg->bCanPileSwitch) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siStgPos, 0 );
		m_odbc.GetData( &sendMsg.siInvPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()


	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "StgPos = %d, InvPos = %d, Amount = %d, LeftItemNum = %d,  UseDay = %02d%02d%02d/%02d��, Dur = %d/%d, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X"
			, sendMsg.siStgPos, sendMsg.siInvPos, pRecvMsg->clItem.siItemNum, sendMsg.clItem.siItemNum, USEDAYDISPLAY(pRecvMsg->clItem), sendMsg.clItem.GetDurability(), sendMsg.clItem.GetMaxDurability(), LOGITEMDISPLAY(pRecvMsg->clItem)
			);
		sDBLogMsg_QueryString LogMsg("WennyOldMan StgToInv", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::BuyRecommendProduct()
{
	// ���� ��Ŷ
	sDBRequest_BuyRecommendProduct *pRecvMsg = (sDBRequest_BuyRecommendProduct *)m_pPacket;

	// ���� ��Ŷ
	sDBResonse_BuyRecommendProduct sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_BUYRECOMMENDPRODUCT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siUsedPoint = pRecvMsg->siNeedPoint;
	sendMsg.clItem_Log.Set(&pRecvMsg->clItem);

	// DB Query
	BEGIN_SP( "rsp_RecommendProductBuy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siNeedPoint, sizeof(pRecvMsg->siNeedPoint));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos));
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch, sizeof(pRecvMsg->bPileSwitch));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siRecommendPoint );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siItemPos );
			GetItemData( &sendMsg.clItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "InvPos = %d, Amount = %d, LeftItemNum = %d, UsePoint = %d, LeftPoint = %d"
			, sendMsg.siItemPos, pRecvMsg->clItem.siItemNum, sendMsg.clItem.siItemNum, pRecvMsg->siNeedPoint, sendMsg.siRecommendPoint
			);
		sDBLogMsg_QueryString LogMsg("Buy Recommend Product", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

}

void DBGameProtocol::ExchangeItem()
{
	// ���� ��Ŷ
	sDBRequest_ExchangeItem *pRecvMsg = (sDBRequest_ExchangeItem *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_ExchangeItem sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_EXCHANGEITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	// DB Query
	BEGIN_SP( "rsp_PersonItemExchange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
		SetItemParam( &pRecvMsg->clChangedItem );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siItemPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetItemData( &sendMsg.clChangedItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siReason = pRecvMsg->siReason;

	// send
	SendPacket( (BYTE*)&sendMsg );

	if( sendMsg.siResult == 1)
	{
		{
			char log[512] = "";
			sprintf(log, "InvPos = %d, ItemNum = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
				, sendMsg.siItemPos, sendMsg.clItem.siItemNum, USEDAYDISPLAY(sendMsg.clItem), LOGITEMDISPLAY(sendMsg.clItem), ELEMENTDISPLAY(sendMsg.clItem)
				);
			sDBLogMsg_QueryString LogMsg("Exchange Item From", pRecvMsg->siPersonID, NULL, sendMsg.clItem.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
		{
			char log[512] = "";
			sprintf(log, "InvPos = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
				, sendMsg.siItemPos, sendMsg.clChangedItem.siItemNum, USEDAYDISPLAY(sendMsg.clChangedItem), LOGITEMDISPLAY(sendMsg.clChangedItem), ELEMENTDISPLAY(sendMsg.clChangedItem)
				);
			sDBLogMsg_QueryString LogMsg("Exchange Item To", pRecvMsg->siPersonID, NULL, sendMsg.clChangedItem.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}
}

void DBGameProtocol::OpenTreasureBox()
{
	// ���� ��Ŷ
	sDBRequest_OpenTreasureBox *pRecvMsg = (sDBRequest_OpenTreasureBox *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_OpenTreasureBox sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_OPENTREASUREBOX;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );

	sendMsg.clUse_Item.Set(&pRecvMsg->clItem);
	sendMsg.clUse_KeyItem.Set(&pRecvMsg->clKeyItem);
	sendMsg.clAdd_ToItem1.Set(&pRecvMsg->clToItem1);
	sendMsg.clAdd_ToItem2.Set(&pRecvMsg->clToItem2);
	sendMsg.bCanNotice1 = pRecvMsg->bCanNotice1;
	sendMsg.bCanNotice2 = pRecvMsg->bCanNotice2;

	// DB Query
	BEGIN_SP( "rsp_PersonItemTreasureBoxOpen" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siKeyItemPos, sizeof(pRecvMsg->siKeyItemPos) );
		SetItemParam( &pRecvMsg->clKeyItem );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToItemPos1, sizeof(pRecvMsg->siToItemPos1) );
		SetItemParam( &pRecvMsg->clToItem1 );
		m_odbc.SetParam( SQL_BIT, &(pRecvMsg->bCreateSID1),	sizeof(pRecvMsg->bCreateSID1) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch1, sizeof(pRecvMsg->bPileSwitch1) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siToItemPos2, sizeof(pRecvMsg->siToItemPos2) );
		SetItemParam( &pRecvMsg->clToItem2 );
		m_odbc.SetParam( SQL_BIT, &(pRecvMsg->bCreateSID2),	sizeof(pRecvMsg->bCreateSID2) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch2, sizeof(pRecvMsg->bPileSwitch2) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siItemPos, 0 );
			GetItemData( &sendMsg.clItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siKeyItemPos, 0 );
			GetItemData( &sendMsg.clKeyItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siToItemPos1, 0 );
			GetItemData( &sendMsg.clToItem1 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siToItemPos2, 0 );
			GetItemData( &sendMsg.clToItem2 );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.siGetItemNumber1 = pRecvMsg->clToItem1.siItemNum;
	sendMsg.siGetItemNumber2 = pRecvMsg->clToItem2.siItemNum;

	// send
	SendPacket( (BYTE*)&sendMsg );

	if( sendMsg.siResult == 1)
	{
		{
			char log[512] = "";
			sprintf(log, "InvPos = %d, ItemNum = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
				, sendMsg.siItemPos, sendMsg.clItem.siItemNum, USEDAYDISPLAY(sendMsg.clItem), LOGITEMDISPLAY(sendMsg.clItem), ELEMENTDISPLAY(sendMsg.clItem)
				);
			sDBLogMsg_QueryString LogMsg("TreasureBox Open Used", pRecvMsg->siPersonID, NULL, sendMsg.clItem.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}

		if(pRecvMsg->clToItem1.siUnique != 0)
		{
			char log[512] = "";
			sprintf(log, "InvPos = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
				, sendMsg.siToItemPos1, sendMsg.clToItem1.siItemNum, USEDAYDISPLAY(sendMsg.clToItem1), LOGITEMDISPLAY(sendMsg.clToItem1), ELEMENTDISPLAY(sendMsg.clToItem1)
				);
			sDBLogMsg_QueryString LogMsg("TreasureBox Open Maked", pRecvMsg->siPersonID, NULL, sendMsg.clToItem1.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}

		if(pRecvMsg->clToItem2.siUnique != 0)
		{
			char log[512] = "";
			sprintf(log, "InvPos = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
				, sendMsg.siToItemPos2, sendMsg.clToItem2.siItemNum, USEDAYDISPLAY(sendMsg.clToItem2), LOGITEMDISPLAY(sendMsg.clToItem2), ELEMENTDISPLAY(sendMsg.clToItem2)
				);
			sDBLogMsg_QueryString LogMsg("TreasureBox Open Maked", pRecvMsg->siPersonID, NULL, sendMsg.clToItem2.siUnique, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}
}

void DBGameProtocol::EnchantUseDayPlus()
{
	// ���� ��Ŷ
	sDBRequest_EnchantUseDayPlus *pRecvMsg = (sDBRequest_EnchantUseDayPlus *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_EnchantUseDayPlus sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ENCHANT_USEDAYPLUS;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonItemEnchantUseDayPlus" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siOutputPos, sizeof(pRecvMsg->siOutputPos));
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siOutputMaterialPos , sizeof(pRecvMsg->siOutputMaterialPos));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMaterialItem.siUnique, sizeof(pRecvMsg->clMaterialItem.siUnique));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->clMaterialItem.siItemNum, sizeof(pRecvMsg->clMaterialItem.siItemNum));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.siOutputPos );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.siOutputMaterialPos );
			GetItemData( &sendMsg.clMaterialItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "InvPos = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
			, sendMsg.siOutputPos, sendMsg.clItem.siItemNum, USEDAYDISPLAY(sendMsg.clItem), LOGITEMDISPLAY(sendMsg.clItem), ELEMENTDISPLAY(sendMsg.clItem)
			);
		sDBLogMsg_QueryString LogMsg("Enchant UseDay Plus", pRecvMsg->siPersonID, NULL, pRecvMsg->clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "InvPos = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
			, sendMsg.siOutputMaterialPos, sendMsg.clMaterialItem.siItemNum, USEDAYDISPLAY(sendMsg.clMaterialItem), LOGITEMDISPLAY(sendMsg.clMaterialItem), ELEMENTDISPLAY(sendMsg.clMaterialItem)
			);
		sDBLogMsg_QueryString LogMsg("Enchant UseDay Plus Used", pRecvMsg->siPersonID, NULL, sendMsg.clMaterialItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SystemRewardList()
{
	// ���� ��Ŷ
	sDBRequest_SystemRewardList *pRecvMsg = (sDBRequest_SystemRewardList *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SystemRewardList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SYSTEMREWARDLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siCount = 0;
	SI32 siMAX_SYSTEMREWARD_LIST = MAX_SYSTEMREWARD_LIST;
	
	// [��ȣ_NHN->NDOORS] ���� ������ ���� ��û������
	SI32 siCurrentIndex = 0;
	SI32 siMaxIndex = 0;

	

	TIMESTAMP_STRUCT stTS;
	Convert_TIMESTAMP_from_SYSTEMTIME(&stTS, &pRecvMsg->servertime);

	// DB Query
	BEGIN_SP( "rsp_SystemRewardList" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &siMAX_SYSTEMREWARD_LIST, sizeof(siMAX_SYSTEMREWARD_LIST));
		m_odbc.SetParam( SQL_TIMESTAMP, &stTS, sizeof(stTS));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siCurPage, sizeof(pRecvMsg->m_siCurPage));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siPersonID );
		m_odbc.GetData( &sendMsg.m_siCurPage );
		m_odbc.GetData( &sendMsg.m_siMaxPage);
		
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siCount >= MAX_SYSTEMREWARD_LIST) continue;
			GetSystemRewardInfo(&sendMsg.clInfo[siCount]);
			++siCount;
		END_FETCHDATA()
	END_NEXTRECORDSET()
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::RecieveSystemReward()
{
	// ���� ��Ŷ
	sDBRequest_RecieveSystemReward *pRecvMsg = (sDBRequest_RecieveSystemReward *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_RecieveSystemReward sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_RECIEVESYSTEMREWARD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_SystemRewardReceive" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos));
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->bCreateSID, sizeof(pRecvMsg->bCreateSID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bPileSwitch, sizeof(pRecvMsg->bPileSwitch));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult);
		m_odbc.GetData( &sendMsg.siPersonID);
		GetSystemRewardInfo(&sendMsg.clInfo);
		m_odbc.GetData( &sendMsg.siItemPos );
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siMoney );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "ReceiveMoney = %I64d, AfterMoney = %I64d, InvPos = %d, Amount = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
			, sendMsg.clInfo.m_siMoney, sendMsg.siMoney
			, sendMsg.siItemPos, sendMsg.clItem.siItemNum, USEDAYDISPLAY(sendMsg.clItem), LOGITEMDISPLAY(sendMsg.clItem), ELEMENTDISPLAY(sendMsg.clItem)
			);
		sDBLogMsg_QueryString LogMsg("SystemReward Receive", pRecvMsg->siPersonID, NULL, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::GiveSystemReward()
{
	// ���� ��Ŷ
	sDBRequest_GiveSystemReward *pRecvMsg = (sDBRequest_GiveSystemReward *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_GiveSystemReward sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GIVESYSTEMREWARD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	TIMESTAMP_STRUCT stGiveTime;
	TIMESTAMP_STRUCT stExpireTime;

	Convert_TIMESTAMP_from_SYSTEMTIME( &stGiveTime, &pRecvMsg->clInfo.m_sGiveTime);
	Convert_TIMESTAMP_from_SYSTEMTIME( &stExpireTime, &pRecvMsg->clInfo.m_sExpireTime);

	// DB Query
	BEGIN_SP( "rsp_SystemRewardGive" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siType, sizeof(pRecvMsg->siType));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->clInfo.m_siIndex, sizeof(pRecvMsg->clInfo.m_siIndex));
		m_odbc.SetParam( SQL_TIMESTAMP, &stGiveTime, sizeof(stGiveTime));
		m_odbc.SetParam( SQL_TIMESTAMP, &stExpireTime, sizeof(stExpireTime));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->clInfo.m_siMoney, sizeof(pRecvMsg->clInfo.m_siMoney));
		SetItemParam( &pRecvMsg->clInfo.m_clItem );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->clInfo.m_szReason, sizeof(pRecvMsg->clInfo.m_szReason));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult);
		m_odbc.GetData( &sendMsg.siPersonID);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "GiveMoney = %I64d, Amount = %d, UseDay = %02d%02d%02d/%02d��, 0x%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X, Element = %d/%d"
			, pRecvMsg->clInfo.m_siMoney
			, pRecvMsg->clInfo.m_clItem.siItemNum, USEDAYDISPLAY(pRecvMsg->clInfo.m_clItem), LOGITEMDISPLAY(pRecvMsg->clInfo.m_clItem), ELEMENTDISPLAY(pRecvMsg->clInfo.m_clItem)
			);
		sDBLogMsg_QueryString LogMsg("SystemReward Give", pRecvMsg->siPersonID, NULL, pRecvMsg->clInfo.m_clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SpaceBoxList()
{
	// ���� ��Ŷ
	sDBRequest_Space_Box_List *pRecvMsg = (sDBRequest_Space_Box_List *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Space_Box_List sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SPACE_BOX_ITEM_LIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonSpaceBoxListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult);
	END_FETCHDATA()

	SI16 siPos;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			siPos = 0;
			m_odbc.GetData(&siPos); if(siPos >= MAX_ITEM_SPACE_BOX) continue;
			GetItemData(&sendMsg.clSpaceBoxItem.clItem[siPos]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			siPos = 0;
			m_odbc.GetData(&siPos); if(siPos >= MAX_ITEM_TREASURE_BOX) continue;
			GetItemData(&sendMsg.clTreasureBoxItem.clItem[siPos]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SpaceBoxMoveItem()
{
	// ���� ��Ŷ
	sDBRequest_Move_Item *pRecvMsg = (sDBRequest_Move_Item *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Move_Item sendMsg;

	char szProc[512];

	sendMsg.packetHeader.usSize	= sizeof( sDBResponse_Move_Item );
	sendMsg.usCharID			= pRecvMsg->usCharID;

	if(pRecvMsg->packetHeader.usCmd == DBMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM)
	{
		sendMsg.packetHeader.usCmd  = DBMSG_RESPONSE_KEEP_SPACE_BOX_MOVE_ITEM;
		MStrCpy( szProc, "rsp_PersonSpaceBoxToBox", sizeof(szProc));
		sendMsg.clMoveItem.Set(&pRecvMsg->clInvenItem);
	}
	else
	{
		sendMsg.packetHeader.usCmd   = DBMSG_RESPONSE_RECOVERY_SPACE_BOX_MOVE_ITEM;
		MStrCpy( szProc, "rsp_PersonSpaceBoxToInv", sizeof(szProc));
		sendMsg.clMoveItem.Set(&pRecvMsg->clBoxItem);
	}

	// DB Query
	BEGIN_SP( szProc )
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bCanPile,			sizeof(pRecvMsg->bCanPile));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID,		sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siInvenItemPos,	sizeof(pRecvMsg->siInvenItemPos));
		SetItemParam(&pRecvMsg->clInvenItem);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siBoxItemPos,		sizeof(pRecvMsg->siBoxItemPos));
		SetItemParam(&pRecvMsg->clBoxItem);
		m_odbc.SetParam( SQL_BIT, &pRecvMsg->type, sizeof(pRecvMsg->type));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult);
		m_odbc.GetData( &sendMsg.siPersonID);
		m_odbc.GetData( &sendMsg.type);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.siInvenItemPos);
			GetItemData(&sendMsg.clInvenItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.siBoxItemPos);
			GetItemData(&sendMsg.clBoxItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		if(pRecvMsg->packetHeader.usCmd == DBMSG_REQUEST_KEEP_SPACE_BOX_MOVE_ITEM)
		{
			sprintf(log, "ItemUnique = %d, ItemPos = %d, ItemNum = %d", 
						  pRecvMsg->clInvenItem.siUnique,
						  pRecvMsg->siInvenItemPos,	
						  pRecvMsg->clInvenItem.siItemNum  );
			sDBLogMsg_QueryString LogMsg( "Keep_Spacebox_Move_Item", pRecvMsg->siPersonID, NULL, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
		else
		{
			sprintf(log, "ItemUnique = %d, ItemPos = %d, ItemNum = %d", 
						pRecvMsg->clBoxItem.siUnique, 
						pRecvMsg->siBoxItemPos,	
						pRecvMsg->clBoxItem.siItemNum	);
			sDBLogMsg_QueryString LogMsg( "Recovery_Spacebox_Move_Item", pRecvMsg->siPersonID, NULL, NULL, log);
			SendLogMsg((sPacketHeader *)&LogMsg);
		}
	}
}

void DBGameProtocol::ChangeEnchant()
{
	// ���� ��Ŷ
	sDBRequest_ChangeEnchant *pRecvMsg = (sDBRequest_ChangeEnchant *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_ChangeEnchant sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CHANGEENCHANT;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonItemChangeEnchant" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siOutputPos, sizeof(pRecvMsg->siOutputPos));
		SetItemParam(&pRecvMsg->clItem);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siOutputMaterialPos, sizeof(pRecvMsg->siOutputMaterialPos));
		SetItemParam(&pRecvMsg->clMaterialItem);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.siOutputPos);
			GetItemData(&sendMsg.clItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.siOutputMaterialPos);
			GetItemData(&sendMsg.clMaterialItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ArtifactInventoryList()
{
	// ���� ��Ŷ
	sDBRequest_Artifact_InventoryList *pRecvMsg = (sDBRequest_Artifact_InventoryList *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Artifact_InventoryList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ARTIFACT_INVENTORYLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI16 siPos;

	// DB Query
	BEGIN_SP( "rsp_ArtifactInventoryList" )
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siPos); if(siPos>=MAX_ITEM_ARTIFACT || siPos < 0) continue;
			GetItemData(&sendMsg.clItemList[siPos]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ArtifactSaveInventory()
{
	// ���� ��Ŷ
	sDBRequest_Artifact_SaveInventory *pRecvMsg = (sDBRequest_Artifact_SaveInventory *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Artifact_SaveInventory sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ARTIFACT_SAVEINVENTORY;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	sendMsg.siUsedItemUnique = pRecvMsg->siUsedItemUnique;
	sendMsg.siUsedItemNum = pRecvMsg->siUsedItemNum;
	sendMsg.siWennyPercent = pRecvMsg->siWennyPercent;
	sendMsg.siResolutionRate = pRecvMsg->siResolutionRate;

	// PCK : �α׿� �߰�
	sendMsg.clUseItem_Log.Set(&pRecvMsg->clUseItem);

	SI16 siPos;

	// DB Query
	BEGIN_SP( "rsp_ArtifactSaveInventory" )
		m_odbc.SetParam(SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID));
		m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->siDisassembleItemPos, sizeof(pRecvMsg->siDisassembleItemPos));
		SetItemParam(&pRecvMsg->clUseItem);
		for(SI32 i = 0; i < MAX_ITEM_ARTIFACT; i++)
		{
			SetItemParam(&pRecvMsg->clItemList[i]);
			m_odbc.SetParam(SQL_BIT, &pRecvMsg->bCreateSID[i], sizeof(pRecvMsg->bCreateSID[i]));
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.siDisassembleItemPos);
			GetItemData(&sendMsg.clUseItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&siPos); if(siPos>=MAX_ITEM_ARTIFACT || siPos < 0) continue;
			GetItemData(&sendMsg.clItemList[siPos]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ArtifactMoveItemStgToInv()
{
	// ���� ��Ŷ
	sDBRequest_Artifact_MoveItemStgToInv *pRecvMsg = (sDBRequest_Artifact_MoveItemStgToInv *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Artifact_MoveItemStgToInv sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ARTIFACT_MOVEITEMSTGTOINV;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// PCK : �α׿� ��Ŷ �߰�
	sendMsg.clMoveItem.Set(&pRecvMsg->clItem);

	// DB Query
	BEGIN_SP( "rsp_ArtifactStgToInv" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siStgPos, sizeof(pRecvMsg->siStgPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siInvPos, sizeof(pRecvMsg->siInvPos) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bCanPileSwitch, sizeof(pRecvMsg->bCanPileSwitch) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siStgPos, 0 );
		m_odbc.GetData( &sendMsg.siInvPos, 0 );
		GetItemData( &sendMsg.clItem );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ArtifactMixture()
{
	// ���� ��Ŷ
	sDBRequest_Artifact_Mixture *pRecvMsg = (sDBRequest_Artifact_Mixture *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Artifact_Mixture sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_ARTIFACT_MIXTURE;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_usCharID = pRecvMsg->m_usCharID;

	for( SI32 i=0; i<sendMsg.m_clCrystalItemList_Log.GetSize(); i++ )
		sendMsg.m_clCrystalItemList_Log[i].Set(&pRecvMsg->m_clCrystalItemList[i]);

	SI32 siPos = 0;

	// DB Query
	BEGIN_SP( "rsp_ArtifactMixture" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siEquipItemPos, sizeof(pRecvMsg->m_siEquipItemPos) );
		SetItemParam(&pRecvMsg->m_clEquipItem);
		for(SI32 i = 0; i < MAX_ITEM_ARTIFACT_MIXTURE; i++)
		{
			m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_sCrystalItemPosList[i], sizeof(pRecvMsg->m_sCrystalItemPosList[i]));
			SetItemParam(&pRecvMsg->m_clCrystalItemList[i]);
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
		m_odbc.GetData(&sendMsg.m_siPersonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.m_siEquipItemPos);
			GetItemData(&sendMsg.m_clEquipItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siPos >= MAX_ITEM_ARTIFACT_MIXTURE || siPos < 0) break;
			m_odbc.GetData(&sendMsg.m_siCrystalItemPosList[siPos]);
			GetItemData(&sendMsg.m_clCrystalItemList[siPos]);
			siPos++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ArtifactRemove()
{
	// ���� ��Ŷ
	sDBRequest_Artifact_Remove *pRecvMsg = (sDBRequest_Artifact_Remove *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Artifact_Remove sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_ARTIFACT_REMOVE;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_usCharID = pRecvMsg->m_usCharID;

	SI16 siPos;

	// DB Query
	BEGIN_SP( "rsp_ArtifactRemove" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siEquipItemPos, sizeof(pRecvMsg->m_siEquipItemPos) );
		SetItemParam(&pRecvMsg->m_clEquipItem);
		for(SI32 i = 0; i < MAX_ITEM_ARTIFACT_MIXTURE; i++)
		{
			m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_siCrystalItemPosList[i], sizeof(pRecvMsg->m_siCrystalItemPosList[i]));
			SetItemParam(&pRecvMsg->m_clCrystalItemList[i]);
		}
		for(SI32 i = 0; i < MAX_ITEM_ARTIFACT_MIXTURE; i++)
		{
			m_odbc.SetParam(SQL_SMALLINT, &pRecvMsg->m_siCreateCrystalItemPosList[i], sizeof(pRecvMsg->m_siCreateCrystalItemPosList[i]));
			SetItemParam(&pRecvMsg->m_clCreateCrystalItemList[i]);
		}
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
		m_odbc.GetData(&sendMsg.m_siPersonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.m_siEquipItemPos);
			GetItemData(&sendMsg.m_clEquipItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	siPos = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siPos >= MAX_ITEM_ARTIFACT_MIXTURE || siPos < 0) break;
			m_odbc.GetData(&sendMsg.m_siCrystalItemPosList[siPos]);
			GetItemData(&sendMsg.m_clCrystalItemList[siPos]);
			siPos++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	siPos = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siPos >= MAX_ITEM_ARTIFACT_MIXTURE || siPos < 0) break;
			m_odbc.GetData(&sendMsg.m_siCreateCrystalItemPosList[siPos]);
			GetItemData(&sendMsg.m_clCreateCrystalItemList[siPos]);
			siPos++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ArtifactSlot_Create()
{
	// ���� ��Ŷ
	sDBRequest_Artifact_Slot_Create *pRecvMsg = (sDBRequest_Artifact_Slot_Create *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Artifact_Slot_Create sendMsg;

	sendMsg.m_packetHeader.usCmd = DBMSG_RESPONSE_ARTIFACT_SLOT_CREATE;
	sendMsg.m_packetHeader.usSize = sizeof( sendMsg );
	sendMsg.m_usCharID = pRecvMsg->m_usCharID;

	sendMsg.m_clCrystalItem_ForLog.Set(&pRecvMsg->m_clCrystalItem);
	sendMsg.m_clEquipItem_ForLog.Set(&pRecvMsg->m_clEquipItem_ForLog);

	// DB Query
	BEGIN_SP( "rsp_ArtifactCreate" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siEquipItemPos, sizeof(pRecvMsg->m_siEquipItemPos) );
		SetItemParam(&pRecvMsg->m_clEquipItem);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siCrystalItemPos, sizeof(pRecvMsg->m_siCrystalItemPos) );
		SetItemParam(&pRecvMsg->m_clCrystalItem);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
		m_odbc.GetData(&sendMsg.m_siPersonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.m_siEquipItemPos);
			GetItemData(&sendMsg.m_clEquipItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData(&sendMsg.m_siCrystalItemPos);
			GetItemData(&sendMsg.m_clCrystalItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SpaceBoxUseItem()
{
	// ���� ��Ŷ
	sDBRequest_Use_SpaceBox_Item *pRecvMsg = (sDBRequest_Use_SpaceBox_Item *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Use_SpaceBox_Item sendMsg;

	bool siType;
	if(pRecvMsg->packetHeader.usCmd == DBMSG_REQUEST_USE_SPACEBOX_ITEM)
	{
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_USE_SPACEBOX_ITEM;
		siType = TYPE_SPACE_BOX;
	}
	else if(pRecvMsg->packetHeader.usCmd == DBMSG_REQUEST_USE_TREASURE_ITEM)
	{
		sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_USE_TREASURE_ITEM;
		siType = TYPE_TREASURE_BOX;
	}
	else
	{
		return;
	}

	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PersonSpaceBoxUseItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos) );
		SetItemParam(&pRecvMsg->clItem);
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siUseReason, sizeof(pRecvMsg->siUseReason) );
		m_odbc.SetParam( SQL_BIT, &siType, sizeof(siType) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.siResult);
		m_odbc.GetData(&sendMsg.siPersonID);
		m_odbc.GetData(&sendMsg.siUseReason);
		m_odbc.GetData(&sendMsg.siItemPos);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetItemData(&sendMsg.clItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetItemData(&sendMsg.clUsedItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SummonPushItem()
{
	// ���� ��Ŷ
	sDBRequest_SummonPushItem *pRecvMsg = (sDBRequest_SummonPushItem*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SummonPushItem sendMsg;


	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SUMMONPUSHITEM;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;

	// DB Query
	SI32 siResult	= 0;
	SI16 siItemNum	= pRecvMsg->m_clFromItem.GetItemNum();


	// DB Query
	BEGIN_SP( "rsp_SummonItemPush" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siFromItemPos, sizeof(pRecvMsg->m_siFromItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clFromItem.siUnique, sizeof(pRecvMsg->m_clFromItem.siUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clFromItem.siItemNum, sizeof(pRecvMsg->m_clFromItem.siItemNum) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siSummonID, sizeof(pRecvMsg->m_siSummonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siToItemPos, sizeof(pRecvMsg->m_siToItemPos) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
		m_odbc.GetData(&sendMsg.m_siPersonID);
		m_odbc.GetData(&sendMsg.m_siSummonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siFromItemPos);
		GetItemData(&sendMsg.m_clFromItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siToItemPos);
		GetItemData(&sendMsg.m_clToItem);
		END_FETCHDATA()
	END_NEXTRECORDSET()


	sendMsg.m_clMoveItem.Set(&sendMsg.m_clToItem);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if ( sendMsg.m_siResult == 1 )
	{
		char log[512] = "";
		sprintf(log, "Result = %d, SummonID = %d, FromItemPos = %d, ToItemPos = %d, Amount = %d, LeftFromItemNum = %d, LeftFromUseDay = %02d%02d%02d/%02d��, LeftFromElement = %d/%d, LeftToItemNum = %d, LeftToUseDay = %02d%02d%02d/%02d��, LeftToElement = %d/%d, PileSwitch = %d"
			, siResult, sendMsg.m_siSummonID, sendMsg.m_siFromItemPos, sendMsg.m_siToItemPos
			, pRecvMsg->m_clFromItem.siItemNum, sendMsg.m_clFromItem.siItemNum, USEDAYDISPLAY(sendMsg.m_clFromItem), ELEMENTDISPLAY(sendMsg.m_clFromItem), sendMsg.m_clToItem.siItemNum, USEDAYDISPLAY(sendMsg.m_clToItem), ELEMENTDISPLAY(sendMsg.m_clToItem)
			, pRecvMsg->m_bFromItemPileSwitch
			);
		sDBLogMsg_QueryString LogMsg( "SummonItemPush Item", pRecvMsg->m_siPersonID, NULL, sendMsg.m_clToItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

}

void DBGameProtocol::SummonPopItem()
{
	// ���� ��Ŷ
	sDBRequest_SummonPopItem *pRecvMsg = (sDBRequest_SummonPopItem*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SummonPopItem sendMsg;


	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SUMMONPOPITEM;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;

	// DB Query
	SI32 siResult	= 0;
	SI16 siItemNum	= pRecvMsg->m_clFromItem.GetItemNum();


	// DB Query
	BEGIN_SP( "rsp_SummonItemPop" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siSummonID, sizeof(pRecvMsg->m_siSummonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siFromItemPos, sizeof(pRecvMsg->m_siFromItemPos) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clFromItem.siUnique, sizeof(pRecvMsg->m_clFromItem.siUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_clFromItem.siItemNum, sizeof(pRecvMsg->m_clFromItem.siItemNum) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_siToItemPos, sizeof(pRecvMsg->m_siToItemPos) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siResult);
		m_odbc.GetData(&sendMsg.m_siPersonID);
		m_odbc.GetData(&sendMsg.m_siSummonID);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siFromItemPos);
		GetItemData(&sendMsg.m_clFromItem);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData(&sendMsg.m_siToItemPos);
		GetItemData(&sendMsg.m_clToItem);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.m_clMoveItem.Set(&sendMsg.m_clToItem);

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if ( sendMsg.m_siResult == 1 )
	{
		char log[512] = "";
		sprintf(log, "Result = %d, SummonID = %d, FromItemPos = %d, ToItemPos = %d, Amount = %d, LeftFromItemNum = %d, LeftFromUseDay = %02d%02d%02d/%02d��, LeftFromElement = %d/%d, LeftToItemNum = %d, LeftToUseDay = %02d%02d%02d/%02d��, LeftToElement = %d/%d, PileSwitch = %d"
			, siResult, sendMsg.m_siSummonID, sendMsg.m_siFromItemPos, sendMsg.m_siToItemPos
			, pRecvMsg->m_clFromItem.siItemNum, sendMsg.m_clFromItem.siItemNum, USEDAYDISPLAY(sendMsg.m_clFromItem), ELEMENTDISPLAY(sendMsg.m_clFromItem), sendMsg.m_clToItem.siItemNum, USEDAYDISPLAY(sendMsg.m_clToItem), ELEMENTDISPLAY(sendMsg.m_clToItem)
			, pRecvMsg->m_bFromItemPileSwitch
			);
		sDBLogMsg_QueryString LogMsg( "SummonItemPop Item", pRecvMsg->m_siPersonID, NULL, sendMsg.m_clToItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

}

void DBGameProtocol::SummonUseItem()
{
	// ���� ��Ŷ
	sDBRequest_SummonUseItem *pRecvMsg = (sDBRequest_SummonUseItem*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SummonUseItem sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SUMMONUSEITEM;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;

	SI16 siItemNum					= pRecvMsg->m_clItem.GetItemNum();

	// DB Query
	BEGIN_SP( "rsp_SummonItemUse" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),		sizeof(pRecvMsg->m_siPersonID)		);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siSummonID),		sizeof(pRecvMsg->m_siSummonID)		);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siItemPos),		sizeof(pRecvMsg->m_siItemPos)		);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_clItem.siUnique),	sizeof(pRecvMsg->m_clItem.siUnique)	);
		m_odbc.SetParam( SQL_SMALLINT,	&(siItemNum),					sizeof(siItemNum)					);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siUseReason),		sizeof(pRecvMsg->m_siUseReason)		);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult, 0);
		GetItemData( &sendMsg.m_clItem );
	END_FETCHDATA()

	sendMsg.m_siPersonID	= pRecvMsg->m_siPersonID;
	sendMsg.m_siItemPos		= pRecvMsg->m_siItemPos;
	sendMsg.m_siUseReason	= pRecvMsg->m_siUseReason;
	sendMsg.m_clUsedItem.Set( &pRecvMsg->m_clItem );

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if ( sendMsg.m_siResult == 1 )
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, SummonID = %d, ItemPos = %d, Amount = %d, UseDay = %02d%02d%02d/%02d��, Element = %d/%d, LeftItemNum = %d"
			, sendMsg.m_siUseReason, sendMsg.m_siSummonID , pRecvMsg->m_siItemPos, siItemNum, USEDAYDISPLAY(pRecvMsg->m_clItem), ELEMENTDISPLAY(pRecvMsg->m_clItem), sendMsg.m_clItem.siItemNum
			);
		sDBLogMsg_QueryString LogMsg( "SummmonUse Item", sendMsg.m_siPersonID, NULL, pRecvMsg->m_clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SummonItem_StatusChange()
{
	// ���� ��Ŷ
	sDBRequest_SummonItem_StatusChange *pRecvMsg = (sDBRequest_SummonItem_StatusChange*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_SummonItem_StatusChange sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_SUMMONITEM_STATUSCHANGE;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;


	// DB Query
	BEGIN_SP( "rsp_SummonItemStatusChange" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),		sizeof(pRecvMsg->m_siPersonID)		);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siSummonID),		sizeof(pRecvMsg->m_siSummonID)		);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siItemPos),		sizeof(pRecvMsg->m_siItemPos)		);
		SetItemParam( &pRecvMsg->m_clItem);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult, 0);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		GetItemData( &sendMsg.m_clItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	sendMsg.m_siPersonID	= pRecvMsg->m_siPersonID;
	sendMsg.m_siItemPos		= pRecvMsg->m_siItemPos;
	sendMsg.m_siReason		= pRecvMsg->m_siReason;
	sendMsg.m_siSummonID	= pRecvMsg->m_siSummonID;
	sendMsg.m_clOldItem.Set( &pRecvMsg->m_clItem );

	// send
	SendPacket( (BYTE *)&sendMsg );

	// �α� �ۼ�
	if ( sendMsg.m_siResult == 1 )
	{
		char log[512] = "";
		sprintf(log, "Reason = %d, SummonID = %d, ItemPos = %d, Amount = %d, UseDay = %02d%02d%02d/%02d��, Element = %d/%d, LeftItemNum = %d"
			, sendMsg.m_siReason, sendMsg.m_siSummonID , pRecvMsg->m_siItemPos, pRecvMsg->m_clItem.siItemNum, USEDAYDISPLAY(pRecvMsg->m_clItem), ELEMENTDISPLAY(pRecvMsg->m_clItem), sendMsg.m_clItem.siItemNum
			);
		sDBLogMsg_QueryString LogMsg( "SummmonItem StatusChange", sendMsg.m_siPersonID, NULL, pRecvMsg->m_clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

}

void DBGameProtocol::RepairItem()
{
	// ���� ��Ŷ
	sDBRequest_RepairItem *pRecvMsg = (sDBRequest_RepairItem*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_RepairItem sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_REPAIRITEM;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_usCharID				= pRecvMsg->m_usCharID;
	
	sendMsg.m_clBeforeItem.Set( &pRecvMsg->m_clBeforeItem );
	sendMsg.m_siType				= pRecvMsg->m_siType;

	// DB Query
	BEGIN_SP( "rsp_PersonItemDurRepair" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),		sizeof(pRecvMsg->m_siPersonID)		);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siPos),			sizeof(pRecvMsg->m_siPos)			);
		SetItemParam( &pRecvMsg->m_clItem);
		for( int siMaterialNumber=0; siMaterialNumber < pRecvMsg->m_Material.GetSize(); siMaterialNumber++)
		{
			m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->m_Material[siMaterialNumber].siStgPos, sizeof(pRecvMsg->m_Material[siMaterialNumber].siStgPos) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_Material[siMaterialNumber].siItemUnique, sizeof(pRecvMsg->m_Material[siMaterialNumber].siItemUnique) );
			m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_Material[siMaterialNumber].siUseItemNum, sizeof(pRecvMsg->m_Material[siMaterialNumber].siUseItemNum) );
		}
		m_odbc.SetParam( SQL_BIGINT,	&(pRecvMsg->m_siRepairMoney),		sizeof(pRecvMsg->m_siRepairMoney));
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siRepairPersonID),	sizeof(pRecvMsg->m_siRepairPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult,			0);
		m_odbc.GetData( &sendMsg.m_siPersonID,			0);
		m_odbc.GetData( &sendMsg.m_siPos,				0);
		GetItemData(	&sendMsg.m_clItem				 );
		m_odbc.GetData( &sendMsg.m_siRepairMoney,		0);
		m_odbc.GetData( &sendMsg.m_siRepairerMoney, 	0);
		m_odbc.GetData( &sendMsg.m_siRepairPersonID,	0);
		m_odbc.GetData( &sendMsg.m_siRepairPersonMoney, 0);
	END_FETCHDATA()

	if( sendMsg.m_siResult == 1 )
	{
		BEGIN_NEXTRECORDSET()
			/* �Ҹ�� ��ǰ�� ��� ó�� */
			// ���� ��Ŷ
			sDBResponse_InputItemToPerson sendMsg2;
			
			sendMsg2.packetHeader.usCmd = DBMSG_RESPONSE_INPUTITEMTOPERSON;
			sendMsg2.packetHeader.usSize = sizeof( sendMsg2 );
			sendMsg2.usCharID = pRecvMsg->m_usCharID;
			sendMsg2.siInputItemPersonOption = INPUTITEMPERSON_OPTION_REPAIRITEM_MATERIAL;

			SI16 siUsedUnique = 0;
			SI16 siUsedNum = 0;
			BEGIN_FETCHDATA()
				m_odbc.GetData( &sendMsg2.siResult,		0);
				m_odbc.GetData( &sendMsg2.siPersonID,	0);
				m_odbc.GetData( &sendMsg2.siItemPos,	0);
				m_odbc.GetData( &sendMsg2.siChangedNum,	0);
				GetItemData( &sendMsg2.clItem);
				m_odbc.GetData( &siUsedUnique, 0 );
				m_odbc.GetData( &siUsedNum, 0 );

				sendMsg2.clInputedItem.Init();
				sendMsg2.clInputedItem.siUnique = sendMsg2.clItem.siUnique;
				sendMsg2.clInputedItem.siItemNum = siUsedNum;		// ������ ������ �Ҹ�� ������ ����

				// send
				SendPacket( (BYTE *)&sendMsg2 );

			END_FETCHDATA()
		END_NEXTRECORDSET()
	}

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::AmberboxSelectItemList()
{
	sDBRequest_Amber_Box_Select_Item_List *pRecvMsg = (sDBRequest_Amber_Box_Select_Item_List *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Amber_Box_Select_Item_List sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;

	// DB Query
	BEGIN_SP( "rsp_AmberBoxItemList" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),		sizeof(pRecvMsg->m_siPersonID)		);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siUseItemUnique),	sizeof(pRecvMsg->m_siUseItemUnique)	);
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siPersonID, 0);
	END_FETCHDATA()

	SI32 siIndex = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		if(siIndex >= sizeof(sendMsg.m_clAmberboxItemList)/sizeof(sendMsg.m_clAmberboxItemList[0]))
			break;
		GetAmberboxSelectItemInfo( &sendMsg.m_clAmberboxItemList[siIndex++]);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket((BYTE *)&sendMsg);
}


void DBGameProtocol::AmberboxSelectItemAdd()
{
	sDBRequest_Amber_Box_Select_Item_Add *pRecvMsg = (sDBRequest_Amber_Box_Select_Item_Add*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Amber_Box_Select_Item_List sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_LIST;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;


	// DB Query
	BEGIN_SP( "rsp_AmberBoxItemAdd" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),		sizeof(pRecvMsg->m_siPersonID)		);
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siUseItemUnique),	sizeof(pRecvMsg->m_siUseItemUnique)	);
		SetAmberboxSelectItemInfo( &pRecvMsg->m_clAmberboxItemList );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siPersonID, 0);
	END_FETCHDATA()

	SI32 siIndex = 0;
	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		if(siIndex >= sizeof(sendMsg.m_clAmberboxItemList)/sizeof(sendMsg.m_clAmberboxItemList[0]))
			break;
		GetAmberboxSelectItemInfo( &sendMsg.m_clAmberboxItemList[siIndex++]);
	END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket((BYTE *)&sendMsg);
}

void DBGameProtocol::AmberboxSelectItemGet()
{
	sDBRequest_Amber_Box_Select_Item_Get *pRecvMsg = (sDBRequest_Amber_Box_Select_Item_Get*)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_Amber_Box_Select_Item_Get sendMsg;

	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_AMBER_BOX_SELECT_ITEM_GET;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siCharID				= pRecvMsg->m_siCharID;
	sendMsg.m_bCanPile				= pRecvMsg->m_bCanPile;

	sendMsg.m_clUseItem.Set(&pRecvMsg->m_clUseItem);
	sendMsg.m_clGetItem.Set(&pRecvMsg->m_clGetItem);

	// DB Query
	BEGIN_SP( "rsp_AmberBoxItemGet" )
		m_odbc.SetParam( SQL_INTEGER,	&(pRecvMsg->m_siPersonID),		sizeof(pRecvMsg->m_siPersonID)		);
		m_odbc.SetParam( SQL_TINYINT,	&(pRecvMsg->m_SlotIndex),		sizeof(pRecvMsg->m_SlotIndex)		);
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siUseItemPos),		sizeof(pRecvMsg->m_siUseItemPos)		);
		SetItemParam( &pRecvMsg->m_clUseItem );
		m_odbc.SetParam( SQL_SMALLINT,	&(pRecvMsg->m_siGetItemPos),		sizeof(pRecvMsg->m_siGetItemPos)		);
		SetItemParam( &pRecvMsg->m_clGetItem );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult, 0);
		m_odbc.GetData( &sendMsg.m_siPersonID, 0);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siRemainItemPos );
		GetItemData( &sendMsg.m_clRemainItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siToItemPos );
		GetItemData( &sendMsg.m_clToItem );
	END_FETCHDATA()
	END_NEXTRECORDSET()

	SendPacket((BYTE *)&sendMsg);
}

void DBGameProtocol::NewSelectBoxItemGet()
{
	// rsp_PersonItemReplace


	// ���� ��Ŷ
	sDBRequest_NewSelectBox_Item_Get *pRecvMsg = (sDBRequest_NewSelectBox_Item_Get *)m_pPacket;

	// ���� ��Ŷ
	sDBResponse_NewSelectBox_Item_Get sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_NEWSELECTBOX_ITEM_GET;
	sendMsg.m_packetHeader.usSize	= sizeof(sendMsg);
	sendMsg.m_uiCharID				= pRecvMsg->m_uiCharID;
	sendMsg.m_siSelectIndex			= pRecvMsg->m_siSelectIndex;
	sendMsg.m_siCanNotice			= pRecvMsg->m_siCanNotice;
	// �α׳� ���� �޽�����
	sendMsg.m_clUseItem.Set( &pRecvMsg->m_clBoxItem );
	sendMsg.m_clToItem.Set( &pRecvMsg->m_clGetItem );


	// DB Query
	BEGIN_SP( "rsp_PersonItemReplace" )
		m_odbc.SetParam( SQL_INTEGER,	&pRecvMsg->m_siPersonID,		sizeof(pRecvMsg->m_siPersonID) );

		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siBoxItemPos,		sizeof(pRecvMsg->m_siBoxItemPos) );
		SetItemParam( &pRecvMsg->m_clBoxItem );

		m_odbc.SetParam( SQL_SMALLINT,	&pRecvMsg->m_siGetItemPos,		sizeof(pRecvMsg->m_siGetItemPos) );
		SetItemParam( &pRecvMsg->m_clGetItem );
		m_odbc.SetParam( SQL_TINYINT,	&pRecvMsg->m_bGetItemCanPile,	sizeof(pRecvMsg->m_bGetItemCanPile) );	
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.m_siResult );
		m_odbc.GetData( &sendMsg.m_siPersonID );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.m_siBoxItemPos );
			GetItemData( &sendMsg.m_clBoxItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.m_siGetItemPos );
			GetItemData( &sendMsg.m_clGetItem );
		END_FETCHDATA()
	END_NEXTRECORDSET()

		// send
	SendPacket( (BYTE *)&sendMsg );

}

