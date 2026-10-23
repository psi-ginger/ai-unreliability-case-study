#include "DBGameProtocol.h"
//#include "DBMsg.h"
#include "DBMsg-Person.h"
#include "DBMsg-Item.h"

void DBGameProtocol::SetRideHorse()
{
	// 받은 패킷
	sDBRequest_SetRideHorse *pRecvMsg = (sDBRequest_SetRideHorse *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetRideHorse sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETRIDEHORSE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HorseRideSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->siCurrentHorseRIDEHORSE, sizeof(pRecvMsg->siCurrentHorseRIDEHORSE) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siIndex, 0 );
		m_odbc.GetData( &sendMsg.siCurrentHorseRIDEHORSE, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetHorse()
{
	// 받은 패킷
	sDBRequest_SetHorse *pRecvMsg = (sDBRequest_SetHorse *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetHorse sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETHORSE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;
	sendMsg.cIHorse.Set( &pRecvMsg->clHorse );

	SI32 siBirthDay = pRecvMsg->clHorse.clBirthDate.GetDateVary();
	SI32 siAddStatusDate = pRecvMsg->clHorse.clAddStatusDate.GetDateVary();

	SI32 siPremiumPArtsDate_Slot1 = pRecvMsg->clHorse.clPremiumPartsDate_Slot1.GetDateVary();
	SI32 siPremiumPArtsDate_Slot2 = pRecvMsg->clHorse.clPremiumPartsDate_Slot2.GetDateVary();
	SI32 siPremiumPArtsDate_Slot3 = pRecvMsg->clHorse.clPremiumPartsDate_Slot3.GetDateVary();
	UI08 siHorseExists = 0;

	// DB Query
	BEGIN_SP( "rsp_HorseSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siReason, sizeof(pRecvMsg->siReason) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siIndex, sizeof(pRecvMsg->siIndex) );
		SetHorseInfo( &pRecvMsg->clHorse, &siBirthDay, &siAddStatusDate
			, &siPremiumPArtsDate_Slot1, &siPremiumPArtsDate_Slot2, &siPremiumPArtsDate_Slot3);
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &siHorseExists, 0 );
	END_FETCHDATA()

	sendMsg.siPersonID	= pRecvMsg->siPersonID;
	sendMsg.siReason	= pRecvMsg->siReason;
	sendMsg.siIndex		= pRecvMsg->siIndex;

	// send
	SendPacket( (BYTE *)&sendMsg );

	if(sendMsg.siResult == 1 && (sendMsg.siReason == SETHORSE_REASON_DIE) && siHorseExists == 0)
	{
		/* 소모된 물품의 결과 처리 */
		// 보낼 패킷
		sDBResponse_InputItemToPerson sendMsg2;
		
		sendMsg2.packetHeader.usCmd = DBMSG_RESPONSE_INPUTITEMTOPERSON;
		sendMsg2.packetHeader.usSize = sizeof( sendMsg2 );
		sendMsg2.usCharID = pRecvMsg->usCharID;
		sendMsg2.siResult		= 1;
		sendMsg2.siPersonID		= sendMsg.siPersonID;
		sendMsg2.siChangedNum	= 0;
		ZeroMemory( &sendMsg2.clItem, sizeof(cltItem) );
		sendMsg2.siInputItemPersonOption = INPUTITEMPERSON_OPTION_NONE;

		for(SI32 siInvIndex = PERSONITEM_HORSE0; siInvIndex < PERSONITEM_HORSE0 + MAX_ITEM_PER_INVENTORY; siInvIndex++)
		{
			sendMsg2.siItemPos		= siInvIndex;
			// send
			SendPacket( (BYTE *)&sendMsg2 );
		}
	}

	// 로그 작성
//	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "Result = %d, Reason = %d, HorseIndex = %d, HorseName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d, AddStatusDate = %d, StatusTIcketNum = %d"
				, sendMsg.siResult, pRecvMsg->siReason, pRecvMsg->siIndex, pRecvMsg->clHorse.szName, pRecvMsg->clHorse.siHorseUnique, pRecvMsg->clHorse.siStr, pRecvMsg->clHorse.siDex, pRecvMsg->clHorse.siMag, pRecvMsg->clHorse.siMoveSpeed
				, pRecvMsg->clHorse.clAddStatusDate.GetDateVary(), pRecvMsg->clHorse.siStatusTicketNum
				);
		sDBLogMsg_QueryString LogMsg("Horse Set", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}


void DBGameProtocol::HorseNameChange()
{
	// 받은 패킷
	sDBRequest_HorseNameChange *pRecvMsg = (sDBRequest_HorseNameChange *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_HorseNameChange sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_HORSENAMECHANGE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	

	char szBeforeName[MAX_HORSE_NAME ] = "";
	SI16 siHorseUnique	= 0;
	SI16 siStr			= 0;
	SI16 siDex			= 0;
	SI16 siMag			= 0;
	SI16 siMoveSpeed	= 0;

	// DB Query
	BEGIN_SP( "rsp_HorseNameChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_SMALLINT, &pRecvMsg->siHorseIndex, sizeof(pRecvMsg->siHorseIndex) );
		m_odbc.SetParam( SQL_VARCHAR, pRecvMsg->strHorseName, sizeof(pRecvMsg->strHorseName) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siPersonID, 0 );
		m_odbc.GetData( &sendMsg.siHorseIndex, 0 );
		m_odbc.GetData( sendMsg.strHorseName, sizeof(sendMsg.strHorseName) );
		m_odbc.GetData( szBeforeName, sizeof(szBeforeName) );
		m_odbc.GetData( &siHorseUnique, 0 );
		m_odbc.GetData( &siStr, 0 );
		m_odbc.GetData( &siDex, 0 );
		m_odbc.GetData( &siMag, 0 );
		m_odbc.GetData( &siMoveSpeed, 0 );
	END_FETCHDATA()
	
	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "HorseName = %s, BeforeName = %s, HorseUnique = %d, Str = %d, Dex = %d, Mag = %d, MoveSpeed = %d"
				, sendMsg.strHorseName, szBeforeName, siHorseUnique, siStr, siDex, siMag, siMoveSpeed
				);
		sDBLogMsg_QueryString LogMsg("Horse Name Change", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}
}

void DBGameProtocol::SetAutoFood()
{
	// 받은 패킷
	sDBRequest_SetAutoFood *pRecvMsg = (sDBRequest_SetAutoFood *)m_pPacket;
	
	// 보낼 패킷
	sDBResponse_SetAutoFood sendMsg;
	
	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SETAUTOFOOD;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.usCharID = pRecvMsg->usCharID;

	// DB Query
	BEGIN_SP( "rsp_HorseAutoFoodSet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siPersonID, sizeof(pRecvMsg->siPersonID) );
		m_odbc.SetParam( SQL_TINYINT, &pRecvMsg->bAutoFoodSwitch, sizeof(pRecvMsg->bAutoFoodSwitch) );
	END_SP()
	
	EXEC_SQL()
	
	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0);
		m_odbc.GetData( &sendMsg.siPersonID, 0);
		m_odbc.GetData( &sendMsg.bAutoFoodSwitch, 0);
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );

	// 로그 작성
	if(sendMsg.siResult == 1)
	{
		char log[512] = "";
		sprintf(log, "bAutoFood = %d"
				, sendMsg.bAutoFoodSwitch
				);
		sDBLogMsg_QueryString LogMsg("Horse Auto Food", pRecvMsg->siPersonID, NULL, NULL, log);
		SendLogMsg((sPacketHeader *)&LogMsg);
	}

}