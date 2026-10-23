#include "DBGameProtocol.h"
#include "DBMsg-Structure.h"
#include "DBMsg-PostOffice.h"

void DBGameProtocol::GetPostOfficeStrInfo()
{
	// 받은 패킷
	sDBRequest_GetStrInfo *pRecvMsg = (sDBRequest_GetStrInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_GetStrInfo sendMsg;
	sendMsg.clStrInfo.clCommonInfo.siStrType = RANKTYPE_POSTOFFICE;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GETSTRINFO;
	sendMsg.packetHeader.usSize = sendMsg.GetTotalSize();
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PostInfoGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
	END_SP()

		EXEC_SQL()

		BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
	m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.clMoney.itMoney,	0);
	GetFeeInfo( &sendMsg.clStrInfo.clPostOfficeStrInfo.clMoneyFee );
	GetFeeInfo( &sendMsg.clStrInfo.clPostOfficeStrInfo.clItemFee );
	m_odbc.GetData( &sendMsg.clStrInfo.clPostOfficeStrInfo.siMsgFee, 0 );
	m_odbc.GetData( &sendMsg.clStrInfo.clPostOfficeStrInfo.siRecvFee, 0 );
	m_odbc.GetData( sendMsg.clStrInfo.clCommonInfo.strNotice, sizeof(sendMsg.clStrInfo.clCommonInfo.strNotice) );
	END_FETCHDATA()

		/* 건물 기본정보 */
		BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siStrDura, 0 );
	m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemUnique, 0 );
	m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siNeedItemNum, 0 );
	m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siUseItemNum, 0 );
	m_odbc.GetData( &(sendMsg.clStrInfo.clCommonInfo.clBaseMoney.itMoney), 0);
	m_odbc.GetData( &sendMsg.clStrInfo.clCommonInfo.siRepairStrDura, 0 );
	END_FETCHDATA()
		END_NEXTRECORDSET()

		// 세금정보를 얻는다.
		GetTaxList( &sendMsg.clStrInfo.clCommonInfo.clTaxInfo, sendMsg.siVillageUnique, sendMsg.clStrInfo.clCommonInfo.siStrType );

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PostOffice_SendItem()
{
	// 받은 패킷
	sDBRequest_PostOfficeSendItem *pRecvMsg = (sDBRequest_PostOfficeSendItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeSendItem sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_SENDITEM;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	//[추가 : 황진성 2007. 10. 24 보내는 아이템 정보]
	sendMsg.siSendItemInventoryIndex = pRecvMsg->siItemInventoryIndex;	
	sendMsg.siSendItemUnique		 = pRecvMsg->siItemUnique;			
	sendMsg.siSendItemNum			 = pRecvMsg->siItemNum;				
	sendMsg.siFee					 = pRecvMsg->siFee;

	SI32 siDateVary = pRecvMsg->clDate.GetDateVary();
	cltItem clItem;

	// DB Query
	BEGIN_SP( "rsp_PostSendItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSenderPersonID, sizeof(pRecvMsg->siSenderPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szReceiverPersonName, sizeof(pRecvMsg->szReceiverPersonName) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemInventoryIndex, sizeof(pRecvMsg->siItemInventoryIndex) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siItemUnique, sizeof(pRecvMsg->siItemUnique) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemNum, sizeof(pRecvMsg->siItemNum) );
		m_odbc.SetParam( SQL_INTEGER, &siDateVary, sizeof(siDateVary) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPostType, sizeof(pRecvMsg->siPostType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bGacha, sizeof(pRecvMsg->bGacha) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siSenderPersonID, 0 );
		m_odbc.GetData( &sendMsg.siItemInventoryIndex, 0 );
		m_odbc.GetData( &sendMsg.siItemUnique, 0 );
		m_odbc.GetData( &sendMsg.siItemNum, 0 );
		m_odbc.GetData( &sendMsg.clPostOfficeMoney, 0 );
		m_odbc.GetData( &sendMsg.clPersonMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siReceiverPersonID, 0 );
		GetItemData( &clItem );
	END_FETCHDATA()

	sendMsg.siPostType = pRecvMsg->siPostType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		char Result[20] = "";
		switch(sendMsg.siResult)
		{
		case 1:	strcpy(Result, "Success");	break;
		case 3:	strcpy(Result, "Fail:No exist receiver");	break;
		case 4:	strcpy(Result, "Fail:Full MailBox");	break;
		default: strcpy(Result, "Fail");	break;
		}
		sprintf(log, "Result = %s, VillageUnique = %d, FromInvPos = %d, Amount = %d, UseDay = %02d%02d%02d/%02d일, Element = %d/%d, SendDate = %d, Fee = %I64d, PersonMoney = %I64d, PostOfficeMoney = %I64d"
				,	Result, sendMsg.siVillageUnique, pRecvMsg->siItemInventoryIndex, pRecvMsg->siItemNum, USEDAYDISPLAY(clItem), ELEMENTDISPLAY(clItem)
				,	siDateVary, pRecvMsg->siFee, sendMsg.clPersonMoney.itMoney, sendMsg.clPostOfficeMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("PostOffice SendItem", sendMsg.siSenderPersonID, sendMsg.siReceiverPersonID, pRecvMsg->siItemUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PostOffice_SendMsg()
{
	// 받은 패킷
	sDBRequest_PostOfficeSendMsg *pRecvMsg = (sDBRequest_PostOfficeSendMsg *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeSendMsg sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_SENDMSG;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siFee = pRecvMsg->siFee;
	MStrCpy(sendMsg.szMsg, pRecvMsg->szMsg, MAX_POSTOFFICE_MSG_SIZE);

	SI32 siDateVary = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_PostSendMsg" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSenderPersonID, sizeof(pRecvMsg->siSenderPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siReceiverPersonID, sizeof(pRecvMsg->siReceiverPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szReceiverPersonName, sizeof(pRecvMsg->szReceiverPersonName) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szMsg, sizeof(pRecvMsg->szMsg) );
		m_odbc.SetParam( SQL_INTEGER, &siDateVary, sizeof(siDateVary) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPostType, sizeof(pRecvMsg->siPostType) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siSenderPersonID, 0 );
		m_odbc.GetData( &sendMsg.clPostOfficeMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.clPersonMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siReceiverPersonID, 0 );
	END_FETCHDATA()

	sendMsg.siPostType = pRecvMsg->siPostType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, SendDate = %d, Fee = %I64d, PersonMoney = %I64d, PostOfficeMoney = %I64d"
				,	sendMsg.siVillageUnique, siDateVary, pRecvMsg->siFee, sendMsg.clPersonMoney.itMoney, sendMsg.clPostOfficeMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("PostOffice SendMsg", sendMsg.siSenderPersonID, sendMsg.siReceiverPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PostOffice_SendMoney()
{
	// 받은 패킷
	sDBRequest_PostOfficeSendMoney *pRecvMsg = (sDBRequest_PostOfficeSendMoney *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeSendMoney sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_SENDMONEY;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	SI32 siDateVary = pRecvMsg->clDate.GetDateVary();

	//[추가 : 황진성 2007. 10. 25 로그에 이용. 송금시 송금액과 수수료 정보.]
	sendMsg.siFee   = pRecvMsg->siFee;
	sendMsg.siMoney = pRecvMsg->siMoney;

	// DB Query
	BEGIN_SP( "rsp_PostSendMoney" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSenderPersonID, sizeof(pRecvMsg->siSenderPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siReceiverPersonID, sizeof(pRecvMsg->siReceiverPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szReceiverPersonName, sizeof(pRecvMsg->szReceiverPersonName) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMoney, sizeof(pRecvMsg->siMoney) );
		m_odbc.SetParam( SQL_INTEGER, &siDateVary, sizeof(siDateVary) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siFee, sizeof(pRecvMsg->siFee) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPostType, sizeof(pRecvMsg->siPostType) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siSenderPersonID, 0 );
		m_odbc.GetData( &sendMsg.clPostOfficeMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.clPersonMoney.itMoney, 0 );
		m_odbc.GetData( &sendMsg.siReceiverPersonID, 0 );
	END_FETCHDATA()

	sendMsg.siPostType = pRecvMsg->siPostType;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "VillageUnique = %d, SendMoney = %I64d, SendDate = %d, Fee = %I64d, PersonMoney = %I64d, PostOfficeMoney = %I64d"
				,	sendMsg.siVillageUnique, pRecvMsg->siMoney, siDateVary, pRecvMsg->siFee, sendMsg.clPersonMoney.itMoney, sendMsg.clPostOfficeMoney.itMoney
				);
		sDBLogMsg_QueryString LogMsg("PostOffice SendMoney", sendMsg.siSenderPersonID, sendMsg.siReceiverPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PostOffice_RecvItemList()
{
	// 받은 패킷
	sDBRequest_PostOfficeRecvItemList *pRecvMsg = (sDBRequest_PostOfficeRecvItemList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeRecvItemList sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_RECVITEMLIST;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PostItemListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	SI32 siDateVary = 0;
	sendMsg.usCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clRecvItemInfo[sendMsg.usCount].siMsgID, 0 );
			m_odbc.GetData( sendMsg.clRecvItemInfo[sendMsg.usCount].szSenderPersonName, sizeof(sendMsg.clRecvItemInfo[sendMsg.usCount].szSenderPersonName) );
			GetItemData( &sendMsg.clRecvItemInfo[sendMsg.usCount].clItem );
			m_odbc.GetData( &siDateVary, 0 ); sendMsg.clRecvItemInfo[sendMsg.usCount].clDate.MakeFullDateFromDateVary(siDateVary);
			m_odbc.GetData( &sendMsg.clRecvItemInfo[sendMsg.usCount].siPostType, 0 );
			sendMsg.usCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PostOffice_RecvMsgList()
{
	// 받은 패킷
	sDBRequest_PostOfficeRecvMsgList *pRecvMsg = (sDBRequest_PostOfficeRecvMsgList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeRecvMsgList sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_RECVMSGLIST;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PostMsgListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	SI32 siDateVary = 0;
	sendMsg.usCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clRecvMsgInfo[sendMsg.usCount].siMsgID, 0 );
			m_odbc.GetData( sendMsg.clRecvMsgInfo[sendMsg.usCount].szSenderPersonName, sizeof(sendMsg.clRecvMsgInfo[sendMsg.usCount].szSenderPersonName) );
			m_odbc.GetData( sendMsg.clRecvMsgInfo[sendMsg.usCount].szMsg, sizeof(sendMsg.clRecvMsgInfo[sendMsg.usCount].szMsg) );
			m_odbc.GetData( &siDateVary, 0 ); sendMsg.clRecvMsgInfo[sendMsg.usCount].clDate.MakeFullDateFromDateVary(siDateVary);
			m_odbc.GetData( &sendMsg.clRecvMsgInfo[sendMsg.usCount].siPostType, 0 );
			sendMsg.usCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PostOffice_RecvMoneyList()
{
	// 받은 패킷
	sDBRequest_PostOfficeRecvMoneyList *pRecvMoney = (sDBRequest_PostOfficeRecvMoneyList *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeRecvMoneyList sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_RECVMONEYLIST;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMoney->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PostMoneyListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMoney->siVillageUnique, sizeof(pRecvMoney->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMoney->siPersonID, sizeof(pRecvMoney->siPersonID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
	END_FETCHDATA()

	SI32 siDateVary = 0;
	sendMsg.usCount = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			m_odbc.GetData( &sendMsg.clRecvMoneyInfo[sendMsg.usCount].siMsgID, 0 );
			m_odbc.GetData( sendMsg.clRecvMoneyInfo[sendMsg.usCount].szSenderPersonName, sizeof(sendMsg.clRecvMoneyInfo[sendMsg.usCount].szSenderPersonName) );
			m_odbc.GetData( &sendMsg.clRecvMoneyInfo[sendMsg.usCount].siMoney, 0 );
			m_odbc.GetData( &siDateVary, 0 ); sendMsg.clRecvMoneyInfo[sendMsg.usCount].clDate.MakeFullDateFromDateVary(siDateVary);
			m_odbc.GetData( &sendMsg.clRecvMoneyInfo[sendMsg.usCount].siPostType, 0 );
			sendMsg.usCount++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::PostOffice_GetItem()
{
	// 받은 패킷
	sDBRequest_PostOfficeGetItem *pRecvMsg = (sDBRequest_PostOfficeGetItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeGetItem sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_GETITEM;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;
	

	// DB Query
	BEGIN_SP( "rsp_PostGetItem" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMsgID, sizeof(pRecvMsg->siMsgID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siInventoryIndex, sizeof(pRecvMsg->siInventoryIndex) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->bPile, sizeof(pRecvMsg->bPile) );
	END_SP()
	
	EXEC_SQL()

	SI32 siRecvItemNum = 0;
	SI32 siSenderPersonID = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMsgID, 0 );
		m_odbc.GetData( &sendMsg.siItemInventoryIndex, 0 );
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &siRecvItemNum, 0 );
		m_odbc.GetData( &siSenderPersonID, 0 );
	END_FETCHDATA()

	sendMsg.siRecvItemNum = siRecvItemNum;

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "InvPos = %d, RecvItemNum = %d, TotalItemNum = %d"
				,	sendMsg.siItemInventoryIndex, siRecvItemNum, sendMsg.clItem.siItemNum
				);
		sDBLogMsg_QueryString LogMsg("PostOffice Get Item", sendMsg.siPersonID, siSenderPersonID, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PostOffice_DelMsg()
{
	// 받은 패킷
	sDBRequest_PostOfficeDelMsg *pRecvMsg = (sDBRequest_PostOfficeDelMsg *)m_pPacket;
	
	// 보낼 패킷
//	sDBResponse_PostOfficeDelMsg sendMsg;
//	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_DELMSG;
//	sendMsg.packetHeader.usSize = sizeof(sendMsg);
//	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PostDelMsg" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMsgID, sizeof(pRecvMsg->siMsgID) );
	END_SP()
	
	EXEC_SQL()
	
	// send
//	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::PostOffice_SetFee()
{
	// 받은 패킷
	sDBRequest_PostOfficeSetFee *pRecvMsg = (sDBRequest_PostOfficeSetFee *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeSetFee sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_SETFEE;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	cltDBFee clMoneyFee( &pRecvMsg->clMoneyFee );
	cltDBFee clItemFee( &pRecvMsg->clItemFee );

	// DB Query
	BEGIN_SP( "rsp_PostFeeSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		SetFeeInfo( &clMoneyFee );
		SetFeeInfo( &clItemFee );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siMsgFee, sizeof(pRecvMsg->siMsgFee) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siRecvFee, sizeof(pRecvMsg->siRecvFee) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		GetFeeInfo( &sendMsg.clMoneyFee );
		GetFeeInfo( &sendMsg.clItemFee );
		m_odbc.GetData( &sendMsg.siMsgFee, 0 );
		m_odbc.GetData( &sendMsg.siRecvFee, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::PostOffice_GetMoney()
{
	// 받은 패킷
	sDBRequest_PostOfficeGetMoney *pRecvMsg = (sDBRequest_PostOfficeGetMoney *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeGetMoney sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_GETMONEY;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	GMONEY siPostMoney = 0;
	SI32 siSenderPersonID = 0;

	// DB Query
	BEGIN_SP( "rsp_PostGetMoney" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siMsgID, sizeof(pRecvMsg->siMsgID) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siMsgID, 0 );
		m_odbc.GetData( &sendMsg.siPersonMoney, 0);
		m_odbc.GetData( &siPostMoney, 0 );
		m_odbc.GetData( &siSenderPersonID, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "PostMoney = %I64d, PersonMoney = %I64d"
				,	siPostMoney, sendMsg.siPersonMoney
				);
		sDBLogMsg_QueryString LogMsg("PostOffice Get Money", sendMsg.siPersonID, siSenderPersonID, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PostOffice_MasterSendItem()
{
	// 받은 패킷
	sDBRequest_PostOfficeMasterSendItem *pRecvMsg = (sDBRequest_PostOfficeMasterSendItem *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_PostOfficeMasterSendItem sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_MASTERSENDITEM;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.siReason = pRecvMsg->siReason;

	SI32 siDateVary = pRecvMsg->clDate.GetDateVary();

	// DB Query
	BEGIN_SP( "rsp_PostSendItemByMaster" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siReceiverPersonID, sizeof(pRecvMsg->siReceiverPersonID) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->szReceiverPersonName, sizeof(pRecvMsg->szReceiverPersonName) );
		SetItemParam( &pRecvMsg->clItem );
		m_odbc.SetParam( SQL_INTEGER, &siDateVary, sizeof(siDateVary) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siReceiverPersonID, 0 );
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &siDateVary, 0 ); sendMsg.clDate.MakeFullDateFromDateVary( siDateVary );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		char Result[200] = "";
		switch(sendMsg.siResult)
		{
		case 1:	strcpy(Result, "성공");	break;
		case 3:	strcpy(Result, "실패:수신자 없음");	break;
		case 4:	strcpy(Result, "실패:수신함 갯수초과");	break;
		default: strcpy(Result, "실패");	break;
		}
		sprintf(log, "Result = %s, Amount = %d, UseDay = %02d%02d%02d/%02d, Element = %d/%d, SendDate = %d, Reason = %d"
				,	Result, pRecvMsg->clItem.siItemNum, USEDAYDISPLAY(pRecvMsg->clItem), ELEMENTDISPLAY(pRecvMsg->clItem), pRecvMsg->clDate.GetDateVary(), pRecvMsg->siReason
				);
		sDBLogMsg_QueryString LogMsg("PostOffice SendItem", 0, sendMsg.siReceiverPersonID, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::PostOffice_GetRecord()
{
	char buffer[ 1024 ];
	// 받은 패킷
	sDBRequest_PostOfficeGetRecord *pRecvMsg = (sDBRequest_PostOfficeGetRecord *)m_pPacket;

	// 보낼 패킷
	sDBResponse_PostOfficeGetRecord sendMsg;
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_POSTOFFICE_GETRECORD;
	sendMsg.packetHeader.usSize = sizeof(sendMsg);
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_PostOfficeRecordGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
	END_SP()

	EXEC_SQL()

	SI32 siDateVary = 0;
	sendMsg.usCount = 0;
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.recordinfo[sendMsg.usCount].siMsgType, 0 );
		m_odbc.GetData( &sendMsg.recordinfo[sendMsg.usCount].siSendRecv, 0 );
		m_odbc.GetData( sendMsg.recordinfo[sendMsg.usCount].szPersonName, sizeof(sendMsg.recordinfo[sendMsg.usCount].szPersonName) );
		m_odbc.GetData( &sendMsg.recordinfo[sendMsg.usCount].siItemUnique, 0 );
		m_odbc.GetData( &sendMsg.recordinfo[sendMsg.usCount].siItemNum, 0 );
		m_odbc.GetData( buffer, MAX_POSTOFFICE_MSG_SIZE );
		m_odbc.GetData( &sendMsg.recordinfo[sendMsg.usCount].siMoney, 0 );
		m_odbc.GetData( &siDateVary, 0 );
		sendMsg.recordinfo[sendMsg.usCount].clDate.MakeFullDateFromDateVary(siDateVary);
		sendMsg.usCount++;
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::HouseSaveUserItemListGet()
{
	// 받은 패킷
	sDBRequest_Get_SaveUserItemList *pRecvMsg = (sDBRequest_Get_SaveUserItemList *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Get_SaveUserItemList sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GET_SAVEUSERITEMLIST;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharId = pRecvMsg->siCharId;

	// DB Query
	BEGIN_SP( "rsp_HouseSaveUserItemListGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSaveItemPersonID, sizeof(pRecvMsg->siSaveItemPersonID));
	END_SP()

	EXEC_SQL()

	SI32 siIndex	= 0;
	SI32 siDate		= 0;

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siIndex >= MAX_SAVEUSERITEM_LIST)
				continue;
			m_odbc.GetData(&sendMsg.clSaveUsersItemInfo[siIndex].siIndex );
			GetItemData( &sendMsg.clSaveUsersItemInfo[siIndex].clItem );
			m_odbc.GetData( &siDate, 0 ); sendMsg.clSaveUsersItemInfo[siIndex].clDate.MakeFullDateFromDateVary(siDate);
			siIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	siIndex	= 0;
	siDate	= 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siIndex >= MAX_SAVEUSERITEM_LIST)
				continue;
			m_odbc.GetData(&sendMsg.clSaveUsersHorseInfo[siIndex].siIndex );
			GetHorseInfo( &sendMsg.clSaveUsersHorseInfo[siIndex].clHorse );
			m_odbc.GetData( &siDate, 0 ); sendMsg.clSaveUsersHorseInfo[siIndex].clDate.MakeFullDateFromDateVary(siDate);
			siIndex++;
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::HouseSaveUserItemGet()
{
	// 받은 패킷
	sDBRequest_Get_SaveUserItem *pRecvMsg = (sDBRequest_Get_SaveUserItem *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Get_SaveUserItem sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GET_SAVEUSERITEM;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharId = pRecvMsg->siCharId;

	// DB Query
	BEGIN_SP( "rsp_HouseSaveUserItemGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSaveItemPersonID, sizeof(pRecvMsg->siSaveItemPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siItemPos, sizeof(pRecvMsg->siItemPos));
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bAddItem, sizeof(pRecvMsg->bAddItem));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siServiceCharge, sizeof(pRecvMsg->siServiceCharge));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siSaveItemPersonID, 0);
		m_odbc.GetData( &sendMsg.siIndex, 0);
		m_odbc.GetData( &sendMsg.siItemPos, 0);
		GetItemData( &sendMsg.clItem );
		m_odbc.GetData( &sendMsg.siUserMoney, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "StgPos = %d, InvPos = %d, Fee = %I64d, LeftUserMoney = %I64d"
			,	sendMsg.siIndex, sendMsg.siItemPos, pRecvMsg->siServiceCharge, sendMsg.siUserMoney
			);
		sDBLogMsg_QueryString LogMsg("HouseUnit Expired Item Get", sendMsg.siSaveItemPersonID, NULL, sendMsg.clItem.siUnique, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::HouseSaveUserHorseGet()
{
	// 받은 패킷
	sDBRequest_Get_SaveUserHorse *pRecvMsg = (sDBRequest_Get_SaveUserHorse *)m_pPacket;

	// 보낼 패킷
	sDBResponse_Get_SaveUserHorse sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_GET_SAVEUSERHORSE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharId = pRecvMsg->siCharId;

	// DB Query
	BEGIN_SP( "rsp_HouseSaveUserHorseGet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSaveItemPersonID, sizeof(pRecvMsg->siSaveItemPersonID));
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex));
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siEnptyHorseindex, sizeof(pRecvMsg->siEnptyHorseindex));
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siServiceCharge, sizeof(pRecvMsg->siServiceCharge));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siSaveHorsePersonID, 0);
		m_odbc.GetData( &sendMsg.siIndex, 0);
		m_odbc.GetData( &sendMsg.siEnptyHorseindex, 0);
		m_odbc.GetData( &sendMsg.siUserMoney, 0);
	END_FETCHDATA()

	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			GetHorseInfo( &sendMsg.clHorse );
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "StgPos = %d, EmptyHorseIndex = %d, Fee = %I64d, LeftUserMoney = %I64d, HorseUnique = %d, HorseName = %s"
			,	sendMsg.siIndex, sendMsg.siEnptyHorseindex, pRecvMsg->siServiceCharge, sendMsg.siUserMoney, sendMsg.clHorse.siHorseUnique, sendMsg.clHorse.szName
			);
		sDBLogMsg_QueryString LogMsg("HouseUnit Expired Horse Get", sendMsg.siSaveHorsePersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::CharNameCheck()
{
	// 받은 패킷
	sDBRequest_Char_Name_Check* pRecvMsg = (sDBRequest_Char_Name_Check*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Char_Name_Check sendMsg;
	sendMsg.packetHeader.usCmd  = DBMSG_RESPONSE_CHAR_NAME_CHECK;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharId			= pRecvMsg->siCharId;
	sendMsg.siReason			= pRecvMsg->siReason;

	// DB Query
	BEGIN_SP( "rsp_PersonNameCheck" )
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->m_szCheckPersonName, sizeof(pRecvMsg->m_szCheckPersonName));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		GetSimplePerson(&sendMsg.CheckPersonInfo);
		m_odbc.GetData( &sendMsg.CheckPersonLevel, 0 );
		m_odbc.GetData( &sendMsg.CheckPersonVillage, 0 );
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}


void DBGameProtocol::CharFriendInfo()
{
	// 받은 패킷
	sDBRequest_Friend_Info* pRecvMsg = (sDBRequest_Friend_Info*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Friend_Info sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_FRIEND_INFO;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siCharId				= pRecvMsg->m_siCharId;
	sendMsg.m_siRequestReason		= pRecvMsg->m_siRequestReason;

	SI32 siCount = 0;

	// DB Query
	BEGIN_SP( "rsp_FriendCharInfo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_siPersonID, sizeof(pRecvMsg->m_siPersonID));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		if(siCount >= MAX_FRIEND_NUMBER) break;
		m_odbc.GetData( sendMsg.m_clCharInfo[siCount].m_PersonName, sizeof(sendMsg.m_clCharInfo[siCount].m_PersonName) );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siPersonID );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siLevel );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siVillageUnique );
		siCount++;
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::CharGuildInfo()
{
	// 받은 패킷
	sDBRequest_Guild_Info* pRecvMsg = (sDBRequest_Guild_Info*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Guild_Info sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_GUILD_INFO;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siCharId				= pRecvMsg->m_siCharId;
	sendMsg.m_siRequestReason		= pRecvMsg->m_siRequestReason;


	SI32 siCount = 0;

	// DB Query
	BEGIN_SP( "rsp_GuildCharInfo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_GuildUnique, sizeof(pRecvMsg->m_GuildUnique));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		if(siCount >= MAX_GUILD_MEMBER_NUM) break;
		m_odbc.GetData( sendMsg.m_clCharInfo[siCount].m_PersonName, sizeof(sendMsg.m_clCharInfo[siCount].m_PersonName) );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siPersonID );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siLevel );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siVillageUnique );
		siCount++;
	END_FETCHDATA()

	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::CharVillageResidentInfo()
{
	// 받은 패킷
	sDBRequest_Village_Resident_Info* pRecvMsg = (sDBRequest_Village_Resident_Info*)m_pPacket;

	// 보낼 패킷
	sDBResponse_Village_Resident_Info sendMsg;
	sendMsg.m_packetHeader.usCmd	= DBMSG_RESPONSE_VILLAGE_RESIDENT_INFO;
	sendMsg.m_packetHeader.usSize	= sizeof( sendMsg );
	sendMsg.m_siCharId				= pRecvMsg->m_siCharId;
	sendMsg.m_siRequestReason		= pRecvMsg->m_siRequestReason;

	SI32 siCount = 0;

	// DB Query
	BEGIN_SP( "rsp_ResidentCharInfo" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->m_VillageUnique, sizeof(pRecvMsg->m_VillageUnique));
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		if(siCount >= MAX_RESIDENT_NUMBER) break;
		m_odbc.GetData( sendMsg.m_clCharInfo[siCount].m_PersonName, sizeof(sendMsg.m_clCharInfo[siCount].m_PersonName) );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siPersonID );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siLevel );
		m_odbc.GetData( &sendMsg.m_clCharInfo[siCount].m_siVillageUnique );
		siCount++;
	END_FETCHDATA()


	SendPacket( (BYTE *)&sendMsg );
}