#include "Char\CharManager\CharManager.h"
#include "..\Server\Server.h"

#include "..\..\..\DBManager\GameDBManager_World\DBMsg.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-Empire.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-CityHall.h"

#include "MsgRval-Define.h"
#include "Msg\MsgType-Empire.h"

#include "../Server/Empire/EmpireManager.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"


void cltServer::DoMsg_DBMSG_RESPONSE_CREATEEMPIRE( sPacketHeader* pPacket )
{
	sDBResponse_CreateEmpire * pclMsg = (sDBResponse_CreateEmpire*)pPacket;

	if( pclMsg->siResult != 1 )
		return;

	SI32 emperorID = pclMsg->siCharID;
	SI32 empireUnique = pclMsg->siEmpireUnique;
	SI32 villageUnique = pclMsg->siVillageUnique;

	if( true == pclCM->IsValidID( emperorID ) )
	{
		/// 제국을 만든 사람은 맹주가 된다.
		pclCM->CR[emperorID]->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_EMPEROR, empireUnique );
		cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[emperorID]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_EMPEROR, empireUnique );
		cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
		((cltCharServer*)pclCM->CR[emperorID])->SendNeighbourMsg(&clMsg, true);
	}

	/// 마을 사람들의 제국 정보를 업데이트 한다.
	SI32 index = 0;
	SI32 id = 0;
	while( id = pclCM->GetCharOrder(index) )
	{
		index++;

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

		if( NULL != pclchar)
		{
			if( false == pclClient->pclCM->IsValidID(id) )		
				continue;

			if( false == IsPC(id) )
				continue;

			// 이미 제국 맹주는 업데이트 해주었기때문에 넘어간다.
			if( emperorID == id )
				continue;
	
			if( pclchar->pclCI->clBI.siHomeVillage == villageUnique )	
			{
				pclchar->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_PERSON, empireUnique );
				cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[id]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_PERSON, empireUnique );
				cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
				((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
			}
		}
	}

	/// 제국을 만든 후에 돈 업데이트
	cltStrInfo *pStrInfo = GetStrInfo( villageUnique, RANKTYPE_CITYHALL );
	if( pStrInfo )
	{
		pStrInfo->clCityhallStrInfo.clProfitMoney.Set( pclMsg->siLeftProfitMoney );

		cltMoney clMoney;
		clMoney.Set( NEED_CREATEEMPIRE_MONEY * -1 );
	
		pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
			INOUTCODE_CREATEEMPIRE, &pclTime->clDate, &clMoney, &pStrInfo->clCityhallStrInfo.clProfitMoney);

		sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique , RANKTYPE_CITYHALL, 
			INOUTCODE_CREATEEMPIRE, &pclTime->clDate, &clMoney, &pStrInfo->clCityhallStrInfo.clProfitMoney );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
	}

	// 제국 매니저에 삽입
	CEmpireUnitInfo clEmpireUnitInfo;
	clEmpireUnitInfo.SetEmpireUnitInfo( empireUnique, villageUnique );
	pclEmpireManager->AddEmpireUnit( &clEmpireUnitInfo );
}

void cltServer::DoMsg_DBMSG_RESPONSE_DESTROYEMPIRE( sPacketHeader* pPacket ) 
{
}

void cltServer::DoMsg_DBMSG_RESPONSE_JOINEEMPIRE( sPacketHeader* pPacket )
{
	sDBResponse_JoinEmpire * pclMsg = (sDBResponse_JoinEmpire*)pPacket;

	if( pclMsg->siResult != 1 )
		return;

	/// 초대에 응한 사람
	SI32 otherID = pclMsg->siOtherCharID;
	/// 업데이트 할 제국 유니크
	SI32 empireUnique = pclMsg->siEmpireUnique;
	/// 초대된 마을의 마을유니크를 구한다.
	SI32 villageUnique = pclMsg->siVillageUnique;
	/// 초대된 길드의 길드유니크를 구한다.
	SI32 guildUnique = pclMsg->siGuildUnique;
		
	if( sDBRequest_JoinEmpire::VILLAGE == pclMsg->siJoinType )
	{
		if( true == pclCM->IsValidID( otherID ) )
		{
			/// 제국에 초대되어 소맹주가 된다.
			pclCM->CR[otherID]->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_PERSON, empireUnique );
			cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[otherID]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_LITTLE_EMPEROR, empireUnique );
			cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
			((cltCharServer*)pclCM->CR[otherID])->SendNeighbourMsg(&clMsg, true);
		}

		/// 마을 사람들의 제국 정보를 업데이트 한다.
		SI32 index = 0;
		SI32 id = 0;
		while( id = pclCM->GetCharOrder(index) )
		{
			index++;

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if( NULL != pclchar)
			{
				if( false == pclClient->pclCM->IsValidID(id) )		
					continue;

				if( false == IsPC(id) )
					continue;

				// 이미 제국 소맹주는 업데이트 해주었기때문에 넘어간다.
				if( otherID == id )
					continue;

				if( pclchar->pclCI->clBI.siHomeVillage == villageUnique )	
				{
					pclchar->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_PERSON, empireUnique );
					cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[id]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_PERSON, empireUnique );
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
					((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
				}
			}
		}

		/// 제국 정보를 업데이트 한다.
		pclEmpireManager->UpdateAddEmpireUnit( empireUnique, villageUnique, 0 );
	}
	else if( sDBRequest_JoinEmpire::GUILD == pclMsg->siJoinType )
	{
		/// 길드 사람들의 제국 정보를 업데이트 한다.
		SI32 index = 0;
		SI32 id = 0;
		while( id = pclCM->GetCharOrder(index) )
		{
			index++;

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if( NULL != pclchar)
			{
				if( false == pclClient->pclCM->IsValidID(id) )		
					continue;

				if( false == IsPC(id) )
					continue;

				// 접속한 길드원의 제국정보를 업데이트
				if( pclchar->pclCI->clBI.clGuildRank.GetGuildUnitIndex() == guildUnique )	
				{
					pclchar->pclCI->clBI.clGuildRank.siEmpireUnique = empireUnique;
					cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[id]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::GUILD, CEmpireInfo::EMPIRE_PERSON, empireUnique );
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
					((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
				}
			}
		}

		/// 제국 정보를 업데이트 한다.
		pclEmpireManager->UpdateAddEmpireUnit( empireUnique, 0, guildUnique );
	}

	/// 초대한 사람
	SI32 emperorID = pclMsg->siCharID;
	if( true == pclCM->IsValidID( emperorID ) )
	{
		cltGameMsgResponse_JoinEmpire clJoinEmpire( cltGameMsgResponse_JoinEmpire::EMPEROR, empireUnique, villageUnique, guildUnique );
		cltMsg clMsg( GAMEMSG_RESPONSE_JOINEMPIRE, sizeof(clJoinEmpire), (BYTE*)&clJoinEmpire);
		pclCM->CR[emperorID]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	/// 초대에 응한 사람
	if( true == pclCM->IsValidID( otherID ) )
	{
		cltGameMsgResponse_JoinEmpire clJoinEmpire( cltGameMsgResponse_JoinEmpire::OTHERUSER, empireUnique, villageUnique, guildUnique );
		cltMsg clMsg( GAMEMSG_RESPONSE_JOINEMPIRE, sizeof(clJoinEmpire), (BYTE*)&clJoinEmpire);
		pclCM->CR[otherID]->SendNetMsg((sPacketHeader*)&clMsg);
	}


}

void cltServer::DoMsg_DBMSG_RESPONSE_SECEDEEMPIRE( sPacketHeader* pPacket )
{
	sDBResponse_SecedeEmpire * pclMsg = (sDBResponse_SecedeEmpire*)pPacket;

	if( pclMsg->siResult != 1 )
		return;

	SI32 empireUnique = pclMsg->siEmpireUnique;
	SI32 villageUnique = pclMsg->siVillageUnique;
	SI32 guildUnique = pclMsg->siGuildUnique;

	if( sDBRequest_SecedeEmpire::VILLAGE == pclMsg->siSecedeType )
	{
		/// 마을 사람들의 제국 정보를 업데이트 한다.
		SI32 index = 0;
		SI32 id = 0;
		while( id = pclCM->GetCharOrder(index) )
		{
			index++;

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];

			if( NULL != pclchar)
			{
				if( false == pclClient->pclCM->IsValidID(id) )		
					continue;

				if( false == IsPC(id) )
					continue;

				/// 제죽에 소속된 마을사람 정보 초기화
				if( pclchar->pclCI->clBI.siHomeVillage == villageUnique )	
				{
					pclchar->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_NONE, 0 );
					cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[id]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_NONE, 0 );
					cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
					((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
				}
				/// 제국에 소속된 마을 대행수 정보 초기화( 대행수가 마을 사람이 아닐수도 있기때문에 )
				else if( pclchar->pclCI->clCharRank.GetVillageUnique() == villageUnique )
				{
					if( RANKTYPE_CITYHALL == pclCM->CR[id]->pclCI->clCharRank.GetRankType() )
					{
						pclchar->pclCI->clBI.clEmpireInfo.SetEmpireInfo( CEmpireInfo::EMPIRE_NONE, 0 );
						cltGameMsgResponse_UpdateEmpireInfo clUpdateEmpireInfo( pclCM->CR[id]->GetCharUnique(), cltGameMsgResponse_UpdateEmpireInfo::VILLAGE, CEmpireInfo::EMPIRE_NONE, 0 );
						cltMsg clMsg(GAMEMSG_RESPONSE_UPDATEEMPIREINFO, sizeof(clUpdateEmpireInfo), (BYTE*)&clUpdateEmpireInfo);
						((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
					}
				}
			}
		}

		// 소속된 제국에서 빼주자
		pclEmpireManager->UpdateDeleteEmpireUnit( empireUnique, villageUnique, 0 );

		/// 제국 정보를 다시 클라이언트로 업데이트
		if( true == pclMsg->bSecedeEmpireByEmperor )
		{
			CEmpireUnitInfo* empireUnitInfo = pclEmpireManager->FindEmpire( empireUnique );

			if( NULL == empireUnitInfo )
				return;

			TCHAR villageChiefName[MAX_EMPIRE_IN_VILLAGE][MAX_PLAYER_NAME] = {'\0'};
			SI32  residentNumber[MAX_EMPIRE_IN_VILLAGE] = {0, };

			TCHAR guildChiefName[MAX_EMPIRE_IN_GUILD][MAX_PLAYER_NAME] = {'\0'};
			SI32  guildMemberNumber[MAX_EMPIRE_IN_GUILD] = {0, };

			cltRank* pclRank;
			for( SI32 i = 0; i < MAX_EMPIRE_IN_VILLAGE; ++i )
			{
				SI32 villageUnique = empireUnitInfo->GetVillageUnique(i);
				if( 0 < villageUnique && villageUnique < MAX_VILLAGE_NUMBER )
				{ 
					cltVillageInfo *pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ villageUnique ];
					if( pVillageInfo == NULL ) continue;

					pclRank = pclRankManager->GetRank( RANKTYPE_CITYHALL, villageUnique );

					if( pclRank )
					{
						if( pclRank->GetPersonID() )
						{
							StringCchCopy( villageChiefName[i], MAX_PLAYER_NAME, pclRank->GetName() );
						} 
						else
						{
							StringCchCopy( villageChiefName[i], MAX_PLAYER_NAME, "" );
						}
					}
					else
					{
						StringCchCopy( villageChiefName[i], MAX_PLAYER_NAME, "" );
					}

					cltStrInfo *pStrInfo = pclVillageManager->pclCityHallManager->GetStrInfo( villageUnique );

					if( NULL != pStrInfo )
					{
						residentNumber[i] = pStrInfo->clCityhallStrInfo.siResidentNum;
					}
					else
					{
						residentNumber[i] = 0;
					}
				}
				else
				{
					StringCchCopy( villageChiefName[i], 20, "" );
					residentNumber[i] = 0;
				}
			}

			for( SI32 i = 0; i < MAX_EMPIRE_IN_GUILD; ++i )
			{
				SI32 guildUnique = empireUnitInfo->GetGuildUnique(i);
				if( 0 < guildUnique )
				{
					cltGuildInfo* guildInfo = pclGuildManager->GetGuildInfo(guildUnique);
					if( guildInfo == NULL ) 
					{
						StringCchCopy( guildChiefName[i], MAX_PLAYER_NAME, "" );
						continue;
					}

					for( SI32 j = 0; j < MAX_GUILD_MEMBER_NUM; ++j )
					{
						if( GUILDRANKTYPE_MASTER == guildInfo->clMember[j].siType )				
						{
							StringCchCopy( guildChiefName[i], MAX_PLAYER_NAME,guildInfo->clMember[j].szCharName );
							break;
						}
					}

					guildMemberNumber[i] = guildInfo->GetMemberNum();
				}
			}

			cltGameMsgResponse_EmpireInfo clEmpireInfo( false,  true, NULL, empireUnitInfo, villageChiefName, residentNumber, guildChiefName, guildMemberNumber );
			cltMsg clMsg(GAMEMSG_RESPONSE_EMPIREINFO, sizeof(clEmpireInfo), (BYTE*)&clEmpireInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			/// 스스로 탈퇴했으므로 클라이언트에서 보여지는 제국정보를 초기화 해준다.
			cltGameMsgResponse_EmpireInfo clEmpireInfo( false,  true, NULL, NULL, NULL, NULL, NULL, NULL );
			cltMsg clMsg(GAMEMSG_RESPONSE_EMPIREINFO, sizeof(clEmpireInfo), (BYTE*)&clEmpireInfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	else if( sDBRequest_SecedeEmpire::GUILD == pclMsg->siSecedeType )
	{

	}
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_ALLEMPIREINFO( sPacketHeader* pPacket )
{
	sDBResponse_AllEmpireInfo * pclMsg = (sDBResponse_AllEmpireInfo*)pPacket;

	for( SI32 i = 0; i < MAX_EMPIRENUMBER; ++i )
	{
		if( 0 < pclMsg->clEmpireInfo[i].GetEmpireUnique() )
		{
			pclEmpireManager->AddEmpireUnit( &pclMsg->clEmpireInfo[i] );
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_AllEmpireInfo"));
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETEMPIREMONEY( sPacketHeader* pPacket )
{
	sDBResponse_SetEmpireMoney * pclMsg = (sDBResponse_SetEmpireMoney*)pPacket;

	if( 1 != pclMsg->siResult )
	{
		return;
	}

	SI32 id = pclMsg->siCharID;
	SI32 villageUnique = pclMsg->siVillageUnique;
	SI32 empireUnique = pclMsg->siEmpireUnique;
	SI64 inputTotalEmpireMoney = pclMsg->siInputTotalEmpireMoney;
	SI64 leftEmpireBaseMoney = pclMsg->siLeftEmpireMoney;

	if( pclCM->IsValidID(id) == false)									return;
	if( villageUnique < 0 || villageUnique >= MAX_VILLAGE_NUMBER )		return;

	if( 0 < id )
	{
		/// 유저가 넣었다

		/// 마을 이익금 업데이트
		cltStrInfo *pStrInfo = GetStrInfo( villageUnique, RANKTYPE_CITYHALL );
		if( pStrInfo )
		{
			pStrInfo->clCityhallStrInfo.clProfitMoney.Set( pclMsg->siLeftVillageProfitMoney );

			cltMoney clMoney;
			clMoney.Set( pclMsg->siAddMoney * -1 );

			pStrInfo->clCityhallStrInfo.clInOutMoneyInfo.AddInOutMoneyInfo( RANKTYPE_CITYHALL,
				INOUTCODE_SETEMPIREMONEY, &pclTime->clDate, &clMoney, &pStrInfo->clCityhallStrInfo.clProfitMoney);

			sDBRequest_CityHallSetInOutMoney sendMsg( pclMsg->siVillageUnique , RANKTYPE_CITYHALL, 
				INOUTCODE_SETEMPIREMONEY, &pclTime->clDate, &clMoney, &pStrInfo->clCityhallStrInfo.clProfitMoney );
			pclGameDBProcess->SendMsg( (sPacketHeader *)&sendMsg );
		}


		/// 제국 정보 업데이트
		pclEmpireManager->UpdateEmpireBaseMoney( empireUnique, leftEmpireBaseMoney, villageUnique, inputTotalEmpireMoney );

		/// 제국 정보를 다시 클라이언트로 업데이트
		CEmpireUnitInfo* empireUnitInfo = pclEmpireManager->FindEmpire( empireUnique );

		if( NULL == empireUnitInfo )
			return;

		TCHAR villageChiefName[MAX_EMPIRE_IN_VILLAGE][MAX_PLAYER_NAME] = {'\0'};
		SI32  residentNumber[MAX_EMPIRE_IN_VILLAGE] = {0, };

		TCHAR guildChiefName[MAX_EMPIRE_IN_GUILD][MAX_PLAYER_NAME] = {'\0'};
		SI32  guildMemberNumber[MAX_EMPIRE_IN_GUILD] = {0, };

		cltRank* pclRank;
		for( SI32 i = 0; i < MAX_EMPIRE_IN_VILLAGE; ++i )
		{
			SI32 villageUnique = empireUnitInfo->GetVillageUnique(i);
			if( 0 < villageUnique && villageUnique < MAX_VILLAGE_NUMBER )
			{
				cltVillageInfo *pVillageInfo = pclClient->pclVillageManager->pclVillageInfo[ villageUnique ];
				if( pVillageInfo == NULL ) continue;

				pclRank = pclRankManager->GetRank( RANKTYPE_CITYHALL, villageUnique );

				if( pclRank )
				{
					if( pclRank->GetPersonID() )
					{
						StringCchCopy( villageChiefName[i], MAX_PLAYER_NAME, pclRank->GetName() );
					} 
					else
					{
						StringCchCopy( villageChiefName[i], MAX_PLAYER_NAME, "" );
					}
				}
				else
				{
					StringCchCopy( villageChiefName[i], MAX_PLAYER_NAME, "" );
				}

				cltStrInfo *pStrInfo = pclVillageManager->pclCityHallManager->GetStrInfo( villageUnique );

				if( NULL != pStrInfo )
				{
					residentNumber[i] = pStrInfo->clCityhallStrInfo.siResidentNum;
				}
				else
				{
					residentNumber[i] = 0;
				}
			}
			else
			{
				StringCchCopy( villageChiefName[i], 20, "" );
				residentNumber[i] = 0;
			}
		}

		for( SI32 i = 0; i < MAX_EMPIRE_IN_GUILD; ++i )
		{
			SI32 guildUnique = empireUnitInfo->GetGuildUnique(i);
			if( 0 < guildUnique )
			{
				cltGuildInfo* guildInfo = pclGuildManager->GetGuildInfo(guildUnique);
				if( guildInfo == NULL ) 
				{
					StringCchCopy( guildChiefName[i], MAX_PLAYER_NAME, "" );
					continue;
				}

				for( SI32 j = 0; j < MAX_GUILD_MEMBER_NUM; ++j )
				{
					if( GUILDRANKTYPE_MASTER == guildInfo->clMember[j].siType )				
					{
						StringCchCopy( guildChiefName[i], MAX_PLAYER_NAME,guildInfo->clMember[j].szCharName );
						break;
					}
				}

				guildMemberNumber[i] = guildInfo->GetMemberNum();
			}
		}

		cltGameMsgResponse_EmpireInfo clEmpireInfo( false,  true, NULL, empireUnitInfo, villageChiefName, residentNumber, guildChiefName, guildMemberNumber );
		cltMsg clMsg(GAMEMSG_RESPONSE_EMPIREINFO, sizeof(clEmpireInfo), (BYTE*)&clEmpireInfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
/*	else( 0 == id )
	{
		/// 시스템에서 넣었다
	}*/
}
