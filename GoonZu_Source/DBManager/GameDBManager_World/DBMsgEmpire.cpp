#include "DBGameProtocol.h"
#include "DBMsg-Empire.h"

void DBGameProtocol::CreateEmpire()
{
	// 받은 패킷
	sDBRequest_CreateEmpire *pRecvMsg = (sDBRequest_CreateEmpire *)m_pPacket;

	// 보낼 패킷
	sDBResponse_CreateEmpire sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CREATEEMPIRE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	
	SI32 siMAX_EMPIRENUMBER = MAX_EMPIRENUMBER;

	// DB Query
	BEGIN_SP( "rsp_EmpireCreate" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siNeedMoney, sizeof(pRecvMsg->siNeedMoney) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_EMPIRENUMBER, sizeof(siMAX_EMPIRENUMBER) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siEmpireUnique, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siLeftProfitMoney, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::DestroyEmpire()
{
	// 받은 패킷
	sDBRequest_DestroyEmpire *pRecvMsg = (sDBRequest_DestroyEmpire *)m_pPacket;

	// 보낼 패킷
	sDBResponse_DestroyEmpire sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_DESTROYEMPIRE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siIndex = 0;

	// DB Query
	BEGIN_SP( "rsp_EmpireDestroy" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siEmpireUnique, sizeof(pRecvMsg->siEmpireUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
	END_FETCHDATA()

	siIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siIndex >= MAX_EMPIRE_IN_VILLAGE) break;
				m_odbc.GetData(&sendMsg.siVillageUnique[siIndex++]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	siIndex = 0;
	BEGIN_NEXTRECORDSET()
		BEGIN_FETCHDATA()
			if(siIndex >= MAX_EMPIRE_IN_GUILD) break;
			m_odbc.GetData(&sendMsg.siGuildUnique[siIndex++]);
		END_FETCHDATA()
	END_NEXTRECORDSET()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::JoinEmpire()
{
	// 받은 패킷
	sDBRequest_JoinEmpire *pRecvMsg = (sDBRequest_JoinEmpire *)m_pPacket;

	// 보낼 패킷
	sDBResponse_JoinEmpire sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_JOINEMPIRE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siOtherCharID = pRecvMsg->siOtherCharID;

	SI32 siMAX_EMPIRE_IN_VILLAGE = MAX_EMPIRE_IN_VILLAGE;
	SI32 siMAX_EMPIRE_IN_GUILD = MAX_EMPIRE_IN_GUILD;

	// DB Query
	BEGIN_SP( "rsp_EmpireJoin" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siJoinType, sizeof(pRecvMsg->siJoinType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siEmpireUnique, sizeof(pRecvMsg->siEmpireUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_EMPIRE_IN_VILLAGE, sizeof(siMAX_EMPIRE_IN_VILLAGE) );
		m_odbc.SetParam( SQL_INTEGER, &siMAX_EMPIRE_IN_GUILD, sizeof(siMAX_EMPIRE_IN_GUILD) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siEmpireUnique, 0 );
		m_odbc.GetData( &sendMsg.siJoinType, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SecedeEmpire()
{
	// 받은 패킷
	sDBRequest_SecedeEmpire *pRecvMsg = (sDBRequest_SecedeEmpire *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SecedeEmpire sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_SECEDEEMPIRE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.bSecedeEmpireByEmperor = pRecvMsg->bSecedeEmpireByEmperor;

	// DB Query
	BEGIN_SP( "rsp_EmpireSecede" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siSecedeType, sizeof(pRecvMsg->siSecedeType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siEmpireUnique, sizeof(pRecvMsg->siEmpireUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siGuildUnique, sizeof(pRecvMsg->siGuildUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult, 0 );
		m_odbc.GetData( &sendMsg.siSecedeType, 0 );
		m_odbc.GetData( &sendMsg.siVillageUnique, 0 );
		m_odbc.GetData( &sendMsg.siGuildUnique, 0 );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::AllEmpireInfo()
{
	// 받은 패킷
	sDBRequest_AllEmpireInfo *pRecvMsg = (sDBRequest_AllEmpireInfo *)m_pPacket;

	// 보낼 패킷
	sDBResponse_AllEmpireInfo sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_ALLEMPIREINFO;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	//sendMsg.siCharID = pRecvMsg->siCharID;

	SI32 siEmpireIndex = 0;
	SI32 siVillageIndex = 0;
	SI32 siGuildIndex = 0;
	SI32 siMemberType = 0;
	SI32 siTempEmpireUnique = 0;
	SI64 siInputTotalEmpireBaseMoney = 0;
	BOOL bStart = TRUE;

	// DB Query
	BEGIN_SP( "rsp_EmpireAllInfo" )
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &siTempEmpireUnique );
		if(bStart == TRUE)
		{
			bStart = FALSE;
			sendMsg.clEmpireInfo[siEmpireIndex].m_siEmpireUnique = siTempEmpireUnique;
		}
		else
		{
			if(siTempEmpireUnique != sendMsg.clEmpireInfo[siEmpireIndex].m_siEmpireUnique)
			{
				siEmpireIndex++;
				if(siEmpireIndex >= MAX_EMPIRENUMBER)
					break;
				siVillageIndex = 0;
				siGuildIndex = 0;
				sendMsg.clEmpireInfo[siEmpireIndex].m_siEmpireUnique = siTempEmpireUnique;
			}
		}

		m_odbc.GetData( &siMemberType );
		m_odbc.GetData( &siInputTotalEmpireBaseMoney );

		if(siMemberType == 0)
		{
			sendMsg.clEmpireInfo[siEmpireIndex].m_siInputTotalEmpireBaseMoney[siVillageIndex] = siInputTotalEmpireBaseMoney;
			m_odbc.GetData( &sendMsg.clEmpireInfo[siEmpireIndex].m_siVillageUnique[siVillageIndex++] );
		}
		else if(siMemberType == 1)
		{
			m_odbc.GetData( &sendMsg.clEmpireInfo[siEmpireIndex].m_siGuildUnique[siGuildIndex++] );
		}

	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::SetEmpireMoney()
{
	// 받은 패킷
	sDBRequest_SetEmpireMoney *pRecvMsg = (sDBRequest_SetEmpireMoney *)m_pPacket;

	// 보낼 패킷
	sDBResponse_SetEmpireMoney sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CREATEEMPIRE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;
	sendMsg.siAddMoney = pRecvMsg->siAddMoney;

	// DB Query
	BEGIN_SP( "rsp_EmpireMoneySet" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siEmpireUnique, sizeof(pRecvMsg->siEmpireUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siType, sizeof(pRecvMsg->siType) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siVillageUnique, sizeof(pRecvMsg->siVillageUnique) );
		m_odbc.SetParam( SQL_BIGINT, &pRecvMsg->siAddMoney, sizeof(pRecvMsg->siAddMoney) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siEmpireUnique );
		m_odbc.GetData( &sendMsg.siVillageUnique );
		m_odbc.GetData( &sendMsg.siLeftEmpireMoney );
		m_odbc.GetData( &sendMsg.siLeftVillageProfitMoney );
		m_odbc.GetData( &sendMsg.siInputTotalEmpireMoney );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}

void DBGameProtocol::ChangeEmperor()
{
	// 받은 패킷
	sDBRequest_ChangeEmperor *pRecvMsg = (sDBRequest_ChangeEmperor *)m_pPacket;

	// 보낼 패킷
	sDBResponse_ChangeEmperor sendMsg;

	sendMsg.packetHeader.usCmd = DBMSG_RESPONSE_CREATEEMPIRE;
	sendMsg.packetHeader.usSize = sizeof( sendMsg );
	sendMsg.siCharID = pRecvMsg->siCharID;


	// DB Query
	BEGIN_SP( "rsp_EmpireChange" )
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siEmpireUnique, sizeof(pRecvMsg->siEmpireUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siOldEmperorVillageUnique, sizeof(pRecvMsg->siOldEmperorVillageUnique) );
		m_odbc.SetParam( SQL_INTEGER, &pRecvMsg->siNewEmperorVillageUnique, sizeof(pRecvMsg->siNewEmperorVillageUnique) );
	END_SP()

	EXEC_SQL()

	BEGIN_FETCHDATA()
		m_odbc.GetData( &sendMsg.siResult );
		m_odbc.GetData( &sendMsg.siEmpireUnique );
		m_odbc.GetData( &sendMsg.siOldEmperorPersonID );
		m_odbc.GetData( &sendMsg.siNewEmperorPersonID );
		m_odbc.GetData( &sendMsg.siOldEmperorVillageUnique );
		m_odbc.GetData( &sendMsg.siNewEmperorVillageUnique );
		m_odbc.GetData( &sendMsg.siLeftEmpireMoney );
	END_FETCHDATA()

	// send
	SendPacket( (BYTE *)&sendMsg );
}