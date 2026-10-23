#include "Char/CharManager/CharManager.h"

#include "../Server/Server.h"
#include "../CommonLogic/Msg/MsgType-Guild.h"
#include "../CommonLogic/Guild/Guild.h"
#include "../Common/Item/ItemCommon/Item-GuildDungeon.h"

#include "../../../DBManager/GameDBManager_World/DBMsg-Guild.h"

#include "MsgType-Guild.h"
#include "MsgRval-Define.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"
#include "../../CommonLogic/CommonLogic.h"

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_CREATEGUILD(sPacketHeader* pPacket)
{
	sDBResponse_Guild_CreateGuild* pclMsg = (sDBResponse_Guild_CreateGuild*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		if ( pclGuildManager->AddGuildInfo(&pclMsg->clInfo) )
		{
			SI32 charid = pclMsg->siCharID;

			if ( pclCM->IsValidID(charid) )
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
				if ( pclchar == NULL )
					return ;


				if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
				{
					pclchar->pclCI->clBI.clGuildRank.Set(GUILDRANKTYPE_MASTER,pclMsg->clInfo.siVillageUnique,
						pclMsg->clInfo.siUnique,pclMsg->clInfo.szName,pclMsg->clInfo.uiConfirm,pclMsg->clInfo.clGPoint.siMarkIndex);

					cltGameMsgResponse_Guild_CreateGuild clinfo(pclMsg->siResult,&pclMsg->clInfo);
					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CREATEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}

		// LEEKH MODIFY 2007.10.25
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 18] // 길드개설. (확인필요 : 정보가 부족한가?) 
			// param1		 == 길드 유니크, 
			// pszCharParam1 == 길드이름. 
			// pszCharParam2 == 시간.
			TCHAR pszCharParam2[50] = {'\0', };
			StringCchPrintf(pszCharParam2, sizeof(pszCharParam2), "%d-%d-%d, %d : %d",
																	pclMsg->clInfo.clDate.uiYear,
																	pclMsg->clInfo.clDate.uiMonth,
																	pclMsg->clInfo.clDate.uiDay,
																	pclMsg->clInfo.clDate.uiHour,
																	pclMsg->clInfo.clDate.uiMinute);
		
			cltCharServer* pclchar = NULL;
			if( pclCM->IsValidID(pclMsg->siCharID) )		pclchar = (cltCharServer*)pclCM->CR[pclMsg->siCharID];
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_CREATE,
								 0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->clInfo.siVillageUnique, 0, 0, 
								 pclMsg->clInfo.siUnique, 0, 0, 0, 0, pclMsg->clInfo.GetName(), pszCharParam2);
		}
	}
	else if ( pclMsg->siResult == -2 ) // 동일한 이름의 길드가 있다
	{
		SI32 charid = pclMsg->siCharID;

		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				cltGameMsgResponse_Guild_CreateGuild clinfo(pclMsg->siResult,&pclMsg->clInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CREATEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_DESTROYGUILD(sPacketHeader* pPacket)
{
	sDBResponse_Guild_DestroyGuild* pclMsg = (sDBResponse_Guild_DestroyGuild*)pPacket;
	if(pclClient->IsCountrySwitch(Switch_Guild_DestroyOverMOnth)){
		// 휴면길드삭제
		if ( pclMsg->siReason == 4 && pclMsg->siResult == 1 ){
			cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->siGuildUnique);
			if (pclGuild == NULL)		return	;
			// 길드를 삭제하고 메세지를 날린다.
			for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
			{
				SI32 personid = pclGuild->clMember[i].siPersonID ;
				SI32 charid = pclCM->GetIDFromPersonID(personid);
				if ( pclCM->IsValidID(charid) ) // 접속해있다면
				{
					pclCM->CR[charid]->pclCI->clBI.clGuildRank.Init();

					cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charid]->pclCI->clBI.clGuildRank,pclCM->CR[charid]->GetCharUnique());
					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
					((cltCharServer*)pclCM->CR[charid])->SendNeighbourMsg(&clMsg,true);

					// 메세지 날려준다.
					cltLetterMsg_Guild_Destroy clLetter(pclMsg->siReason);
					SendLetterMsg(  personid , (cltLetterHeader*)&clLetter );
				}
				else
				{
					// 메세지 날려준다.
					cltLetterMsg_Guild_Destroy clLetter(pclMsg->siReason);
					SendLetterMsg(  personid , (cltLetterHeader*)&clLetter );
				}
				

			}
			if ( pclGuildManager->DeleteGuildInfo(pclMsg->siGuildUnique) )
			{
				SI32 charid = pclMsg->siCharID;

				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
				if ( pclchar == NULL )
					return ;

				if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
				{
					cltGameMsgResponse_Guild_DestroyGuild clinfo(pclMsg->siResult,pclMsg->siGuildUnique,pclMsg->siVillageUnique,pclMsg->siReason);
					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_DESTROYGUILD, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);

					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [로그추가 : 손성웅] // 휴면길드신청 제거.
						// Param1 == 길드 유니크
						cltServer* pclserver = (cltServer*)pclClient;
						pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_DESTROY_MONTH,
							pclMsg->siReason, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
							pclMsg->siGuildUnique, 0, 0, 0, 0, NULL, NULL);
					}
				}
			
				//길드를 삭제 한다.
				cltGameMsgResponse_Guild_DestroyGuild clinfo(pclMsg->siResult,pclMsg->siGuildUnique,pclMsg->siVillageUnique,pclMsg->siReason);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_DESTROYGUILD, sizeof(clinfo), (BYTE*)&clinfo);
			}
		}
	}

	if ( pclMsg->siReason != 4 && pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->siGuildUnique);
		if ( pclGuild == NULL ) return ;

		for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
		{
			// 길드 회원
			SI32 personid = pclGuild->clMember[i].siPersonID ;
			SI32 charid = pclCM->GetIDFromPersonID(personid);
			if ( pclCM->IsValidID(charid) ) // 접속해있다면
			{
				pclCM->CR[charid]->pclCI->clBI.clGuildRank.Init();

				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charid]->pclCI->clBI.clGuildRank,pclCM->CR[charid]->GetCharUnique());
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				((cltCharServer*)pclCM->CR[charid])->SendNeighbourMsg(&clMsg,true);

				// 메세지 날려준다.
				cltLetterMsg_Guild_Destroy clLetter(pclMsg->siReason);
				SendLetterMsg(  personid , (cltLetterHeader*)&clLetter );
			}

			// 길드 가입 요청자
			personid = pclGuild->clStandingMember[i].siPersonID ;
			charid = pclCM->GetIDFromPersonID(personid);
			if ( pclCM->IsValidID(charid) ) // 접속해있다면
			{
				pclCM->CR[charid]->pclCI->clBI.clGuildRank.Init();

				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charid]->pclCI->clBI.clGuildRank,pclCM->CR[charid]->GetCharUnique());
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				((cltCharServer*)pclCM->CR[charid])->SendNeighbourMsg(&clMsg,true);

				// 메세지 날려준다.
				cltLetterMsg_Guild_Destroy clLetter(pclMsg->siReason);
				SendLetterMsg(  personid , (cltLetterHeader*)&clLetter );
			}
		}

		if ( pclGuildManager->DeleteGuildInfo(pclMsg->siGuildUnique) )
		{
			SI32 charid = pclMsg->siCharID;

			if ( pclCM->IsValidID(charid) )
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
				if ( pclchar == NULL )
					return ;

				if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
				{
					//pclchar->pclCI->clBI.clGuildRank.Init();

					cltGameMsgResponse_Guild_DestroyGuild clinfo(pclMsg->siResult,pclMsg->siGuildUnique,pclMsg->siVillageUnique,pclMsg->siReason);
					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_DESTROYGUILD, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);

					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [로그추가 : 황진성 2007. 10. 10] // 길드신청 제거.
						// Param1 == 길드 유니크.  
						cltServer* pclserver = (cltServer*)pclClient;
						pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_DESTROY,
														pclMsg->siReason, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
														pclMsg->siGuildUnique, 0, 0, 0, 0, NULL, NULL);
					}
				}
			}
		}
	}	
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_JOINGUILD(sPacketHeader* pPacket)
{
	sDBResponse_Guild_JoinGuild* pclMsg = (sDBResponse_Guild_JoinGuild*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clInfo.siUnique);
		if ( pclGuild == NULL ) return ;

		pclGuild->Set(&pclMsg->clInfo);

		SI32 charid = pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				pclchar->pclCI->clBI.clGuildRank.Set(GUILDRANKTYPE_STANDING,pclMsg->clInfo.siVillageUnique,
					pclMsg->clInfo.siUnique,pclMsg->clInfo.szName,pclMsg->clInfo.uiConfirm,pclMsg->clInfo.clGPoint.siMarkIndex);

				cltGameMsgResponse_Guild_JoinGuild clinfo(&pclMsg->clInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_JOINGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			
				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
				{
					// [로그추가 : 황진성 2007. 10. 18] // 길드 가입을 요청
					// Param1 == 상단 유니크.  pszParam1 == 길드이름, pszParam2 : 길드가입요청시간
					TCHAR str[50] = {'\0', };
					StringCchPrintf(str, sizeof(str), TEXT("%d-%d-%d, %d : %d"),	pclMsg->clInfo.clDate.uiYear, 
																					pclMsg->clInfo.clDate.uiMonth,
																					pclMsg->clInfo.clDate.uiDay, 
																					pclMsg->clInfo.clDate.uiHour,
																					pclMsg->clInfo.clDate.uiMinute );

					cltServer* pclserver = (cltServer*)pclClient;
					pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_JOIN,
													0, (cltCharServer*)pclchar, NULL, pclMsg->clInfo.siMasterPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->clInfo.siVillageUnique, 0, 0, 
													pclMsg->clInfo.siUnique, 0, 0, 0, 0, pclMsg->clInfo.szName, str);
				}
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_CONFIRMJOINGUILD(sPacketHeader* pPacket)
{
	sDBResponse_Guild_ConfirmJoinGuild* pclMsg = (sDBResponse_Guild_ConfirmJoinGuild*)pPacket ;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clInfo.siUnique);
		if ( pclGuild == NULL ) return ;

		pclGuild->Set(&pclMsg->clInfo);

		// 허가일 때
		if ( pclMsg->bConfirm )
		{
			SI32 standingpersonid = pclMsg->siStandingPersonID ;
			SI32 standingID = pclCM->GetIDFromPersonID(standingpersonid);
			if ( pclCM->IsValidID(standingID) ) // 접속해있다면
			{
				pclCM->CR[standingID]->pclCI->clBI.clGuildRank.Set(GUILDRANKTYPE_MEMBER,
					pclMsg->clInfo.siVillageUnique,pclMsg->clInfo.siUnique,pclMsg->clInfo.szName,
					pclMsg->clInfo.uiConfirm,pclMsg->clInfo.clGPoint.siMarkIndex);

				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[standingID]->pclCI->clBI.clGuildRank,pclCM->CR[standingID]->GetCharUnique());
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				((cltCharServer*)pclCM->CR[standingID])->SendNeighbourMsg(&clMsg,true);
			}

			if ( standingpersonid > 0 )
			{
				// 허가 쪽지 날려준다.
				cltLetterMsg_GuildJoin clletter( TRUE );
				SendLetterMsg(  standingpersonid , (cltLetterHeader*)&clletter );
			}


			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 16] //  길드 가입을 신청한 유저를 길드원으로 허가.
				// param1		 == 길드 유니크, 
				// pszCharParam1 == 길드이름.
				cltServer* pclserver = (cltServer*)pclClient;
				cltCharServer* pclchar = NULL;
				if( pclCM->IsValidID(pclMsg->siCharID) )		pclchar = (cltCharServer*)pclCM->CR[pclMsg->siCharID];
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_JOINCONFIRM, 
												0, pclchar, NULL, standingpersonid, 0, 0, NULL, 0, 0, 0, pclMsg->clInfo.siVillageUnique, 0, 0,
												pclMsg->clInfo.siUnique, 0, 0, 0, 0, pclMsg->clInfo.szName, NULL);
			}
		}
		else // 불허일 대
		{
			SI32 standingpersonid = pclMsg->siStandingPersonID ;
			SI32 standingID = pclCM->GetIDFromPersonID(standingpersonid);
			if ( standingID ) // 접속해있다면
			{
				pclCM->CR[standingID]->pclCI->clBI.clGuildRank.Init() ;
				pclCM->CR[standingID]->clPB.clGuildBA.Init() ;// 길드 가입을 거부 당하였음으로 길드 능력치 초기화 
				/*cltGuildStatus clstatus;
				ZeroMemory(&clstatus,sizeof(cltGuildStatus));*/

				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[standingID]->pclCI->clBI.clGuildRank,pclCM->CR[standingID]->GetCharUnique());
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				((cltCharServer*)pclCM->CR[standingID])->SendNeighbourMsg(&clMsg,true);
			}

			// 불허 쪽지 날려준다.
			if ( standingpersonid > 0 )
			{
				// 허가 쪽지 날려준다.
				cltLetterMsg_GuildJoin clletter( FALSE );
				SendLetterMsg(  standingpersonid , (cltLetterHeader*)&clletter );
			}
		}

		SI32 charid = pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				cltGameMsgResponse_Guild_ConfirmJoinGuild clinfo(pclMsg->bConfirm,pclMsg->siStandingPersonID,&pclMsg->clInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CONFIRMJOINGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}

		// 경험치가 다운되면 레벨도 다운시킨다.
		SI32 level = pclGuild->clGPoint.siLevel ;
		SI32 nextlevel = GetLevelFromExp( pclGuild->clGPoint.siPoint, EXPFORLEVEL_TYPE_GUILD );
		if ( level > nextlevel && pclGuild->clGPoint.siLevel > 1 )
		{
			sDBRequest_Guild_LevelUP clMsg(pclGuild->siUnique,nextlevel);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_LEAVEGUILD(sPacketHeader* pPacket)
{
	sDBResponse_Guild_LeaveGuild* pclMsg = (sDBResponse_Guild_LeaveGuild*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		//[진성] DB에서 받은 CharID가 없어 personID로 찾는것으로 수정. => 2008-9-3
		SI32 charid = pclCM->GetIDFromPersonID( pclMsg->siPersonID );// pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				pclchar->pclCI->clBI.clGuildRank.Init();

				cltGameMsgResponse_Guild_LeaveGuild clinfo(&pclMsg->clInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_LEAVEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 10] // 길드 탈퇴.
					// param1	 == 길드 유니크. 
					// pszParam1 == 길드 이름.  
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_LEAVE,
										 0, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->clInfo.siVillageUnique, 0, 0, 
										 pclMsg->clInfo.siUnique, 0, 0, 0, 0, pclMsg->clInfo.szName, NULL);
				}
			}
		}

		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clInfo.siUnique);
		if ( pclGuild == NULL ) return ;
		pclGuild->Set(&pclMsg->clInfo);

		// 경험치가 다운되면 레벨도 다운시킨다.
		SI32 level = pclGuild->clGPoint.siLevel ;
		SI32 nextlevel = GetLevelFromExp( pclGuild->clGPoint.siPoint, EXPFORLEVEL_TYPE_GUILD );
		if ( level > nextlevel && pclGuild->clGPoint.siLevel > 1 )
		{
			sDBRequest_Guild_LevelUP clMsg(pclGuild->siUnique,nextlevel);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_CHANGEVILLAGE(sPacketHeader* pPacket)
{
	sDBResponse_Guild_ChangeVillage* pclMsg = (sDBResponse_Guild_ChangeVillage*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clInfo.siUnique);
		if ( pclGuild == NULL ) return ;

		pclGuild->Set(&pclMsg->clInfo);

		//for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
		//{
		//	SI32 personid = pclGuild->clMember[i].siPersonID ;
		//	SI32 charID = pclCM->GetIDFromPersonID(personid);
		//	if ( charID ) // 접속해있다면
		//	{
		//		pclCM->CR[charID]->pclCI->clBI.clGuildRank.SetVillageUnique(pclMsg->siVillageUnique);
		//		pclCM->CR[charID]->pclCI->clBI.clGuildRank.uiConfirm = GUILDSTATUS_CHANGEVILLAGE;

		//		// 통보해준다.
		//		cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
		//		cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		//		((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
		//	}

		//	SI32 standingpersonid = pclGuild->clStandingMember[i].siPersonID ;
		//	SI32 standingcharID = pclCM->GetIDFromPersonID(standingpersonid);
		//	if ( standingcharID ) // 접속해있다면
		//	{
		//		pclCM->CR[standingcharID]->pclCI->clBI.clGuildRank.SetVillageUnique(pclMsg->siVillageUnique);
		//		pclCM->CR[standingcharID]->pclCI->clBI.clGuildRank.uiConfirm = GUILDSTATUS_CHANGEVILLAGE;

		//		// 통보해준다.
		//		cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[standingcharID]->pclCI->clBI.clGuildRank,pclCM->CR[standingcharID]->GetCharUnique());
		//		cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		//		((cltCharServer*)pclCM->CR[standingcharID])->SendNeighbourMsg(&clMsg,true);
		//	}
		//}

		SI32 masterID = pclMsg->siCharID ;
		if ( pclCM->IsValidID(masterID) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[masterID];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				cltGameMsgResponse_Guild_ChangeVillage clChangeVillage(&pclMsg->clInfo);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CHANGEVILLAGE, sizeof(cltGameMsgResponse_Guild_ChangeVillage), (BYTE*)&clChangeVillage);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);


				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 11] // 길드 마을 변경
					// param1 == 길드 유니크. param2 == 변경후의 마을 유니크
					//pszCharParam1 : 길드 이름
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_CHANGEVILLAGE, 
										 0, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->clInfo.siVillageUnique, 0, 0,
										 pclMsg->clInfo.siUnique, pclMsg->clInfo.siMoveVillageUnique, 0, 0, 0, pclMsg->clInfo.szName, NULL);
				}
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_CHANGEGUILDPOINT(sPacketHeader* pPacket)
{
	sDBResponse_Guild_ChangeGuildPoint* pclMsg = (sDBResponse_Guild_ChangeGuildPoint*)pPacket;

	if (pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
		if ( pclGuild == NULL ) return ;

		pclGuild->Set(&pclMsg->clinfo);
		// 경험치가 올라가면 레벨도 업시킨다.
		SI32 level = pclGuild->clGPoint.siLevel ;
		SI32 nextlevel = GetLevelFromExp( pclGuild->clGPoint.siPoint, EXPFORLEVEL_TYPE_GUILD );
		if ( level < nextlevel && pclGuild->clGPoint.siLevel < MAX_GUILD_LEVEL )
		{
			sDBRequest_Guild_LevelUP clMsg(pclGuild->siUnique,nextlevel);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
		}
		// 경험치가 다운되면 레벨도 다운시킨다.
		level = pclGuild->clGPoint.siLevel ;
		nextlevel = GetLevelFromExp( pclGuild->clGPoint.siPoint, EXPFORLEVEL_TYPE_GUILD );
		if ( level > nextlevel && pclGuild->clGPoint.siLevel > 1 )
		{
			sDBRequest_Guild_LevelUP clMsg(pclGuild->siUnique,nextlevel);
			pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 10] // 길드의 GP가 변화.
			// Param1		 == 길드 유니크, 
			// param2		 == 길드 포인트, 
			// pszCharParam1 == 길드 이름. 
			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_CHANGEGUILDPOINT,
											0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclGuild->siVillageUnique, 0, 0, 
											pclGuild->siUnique, pclGuild->clGPoint.siPoint, 0, 0, 0, 
											pclGuild->szName, NULL);
		}		
	}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_GETGUILDINFO(sPacketHeader* pPacket)
{
	sDBResponse_Guild_GetGuildInfo* pclMsg = (sDBResponse_Guild_GetGuildInfo*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		SI32 startpos = pclMsg->siStartPos ;
		if (startpos < 0 )
			return ;

		SI32 index = 0 ;
		for ( SI32 i = startpos ; i < startpos + MAX_GUILD_PER_DBLIST ; i ++ )
		{
			if( i < 0 || i >= MAX_GUILD_NUM )	continue;
			pclGuildManager->clUnit[i].Set(&pclMsg->clInfo[index]);
			index ++ ;
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_Guild_GetGuildInfo"));

}

enum{
	CONFIRMRESULT_CREATE_SUCCESS = 1,			// 개설 성공
	CONFIRMRESULT_CREATE_DENINE,				// 개설 거부
	CONFIRMRESULT_CREATE_FAILED,				// 개설 실패
	CONFIRMRESULT_CHANGE_SUCCESS,				// 이주 성공
	CONFIRMRESULT_CHANGE_DENINE,				// 이주 거부
	CONFIRMRESULT_CHANGE_FAILED					// 이주 실패
};

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD(sPacketHeader* pPacket)
{
	sDBResponse_Guild_ConfirmCreateGuild* pclMsg = (sDBResponse_Guild_ConfirmCreateGuild*)pPacket;

	if ( pclMsg->siResult < 1 )
		return;

	SI32	resultStatus = 0;

	if ( pclMsg->bConfirm )
	{
		if ( pclMsg->bState )
		{
			if ( pclMsg->siBeforeConfirm == GUILDSTATUS_STANDING )
			{
				resultStatus = CONFIRMRESULT_CREATE_SUCCESS;
			}
			else if (pclMsg->siBeforeConfirm == GUILDSTATUS_CHANGEVILLAGE )
			{
				resultStatus = CONFIRMRESULT_CHANGE_SUCCESS;
			}
		}
		else
		{
			if ( pclMsg->siBeforeConfirm == GUILDSTATUS_STANDING )
			{
				resultStatus = CONFIRMRESULT_CREATE_FAILED;
			}
			else if (pclMsg->siBeforeConfirm == GUILDSTATUS_CHANGEVILLAGE )
			{
				resultStatus = CONFIRMRESULT_CHANGE_FAILED;
			}
		}
	}
	else
	{
		if ( pclMsg->siBeforeConfirm == GUILDSTATUS_STANDING )
		{
			resultStatus = CONFIRMRESULT_CREATE_DENINE;
		}
		else if (pclMsg->siBeforeConfirm == GUILDSTATUS_CHANGEVILLAGE )
		{
			resultStatus = CONFIRMRESULT_CHANGE_DENINE;
		}

	}

	cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->siGuildUnique);
	if ( pclGuild == NULL ) return ;

	// 결과에 따라서 다르게 처리한다.
	
	UI08 confirmStatus = 0;

	switch( resultStatus )
	{
	case CONFIRMRESULT_CREATE_SUCCESS:
		{
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 31] // 길드 생성을 허가.
				// param1		 == 길드 유니크.
				// param3		 == 임대료				- 매달 빠져나갈 돈.
				// pszCharParam1 == 길드 이름.
				cltCharServer* pclchar = NULL;
				if( pclCM->IsValidID(pclMsg->siCharID) )		pclchar = (cltCharServer*)pclCM->CR[pclMsg->siCharID];
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_CONFIRMCREATE, 
									 0, pclchar, NULL, pclMsg->siPersonID, pclMsg->siMoney, pclMsg->siLeftMoney, NULL, 0, 0, 0, 0, 0, 0,
									 pclMsg->clInfo.siUnique, 0, pclMsg->siFee, 0, 0, 
									 pclMsg->clInfo.GetName(), NULL);
			}

			confirmStatus = GUILDSTATUS_NORMAL;
			SI32 masterID = pclCM->GetIDFromPersonID(pclGuild->siMasterPersonID) ;
			if ( masterID > 0 )
			{
				if ( pclCM->IsValidID(masterID) )
				{
					cltCharServer* pclmaster = (cltCharServer*)pclCM->CR[masterID];
					if ( pclmaster == NULL )
						return ;

					pclmaster->pclCI->clBank.SetMoney(pclMsg->siLeftMoney);
					// 통보해서 업데이트해준다.
					pclmaster->SetUpdateSwitch(UPDATE_BANK,true,0);
				}
				cltLetterMsg_Guild_ConfirmCreateGuild clletter(TRUE, MIN_GUILD_CREATE_FEE);
				SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
			}

			
		}
		break;
	case CONFIRMRESULT_CREATE_DENINE:
		{
			cltLetterMsg_Guild_ConfirmCreateGuild clletter(FALSE, 0);
			SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
		}
		break;
	case CONFIRMRESULT_CREATE_FAILED:
		{
			cltLetterMsg_Guild_ConfirmCreateGuild clletter(FALSE, MIN_GUILD_CREATE_FEE);
			SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
		}
		break;
	case CONFIRMRESULT_CHANGE_SUCCESS:
		{
			confirmStatus = GUILDSTATUS_NORMAL;
			SI32 masterID = pclCM->GetIDFromPersonID(pclGuild->siMasterPersonID) ;
			if ( masterID > 0 )
			{
				if ( pclCM->IsValidID(masterID) )
				{
					cltCharServer* pclmaster = (cltCharServer*)pclCM->CR[masterID];
					if ( pclmaster == NULL )
						return ;

					pclmaster->pclCI->clBank.SetMoney(pclMsg->siLeftMoney);
					// 통보해서 업데이트해준다.
					pclmaster->SetUpdateSwitch(UPDATE_BANK,true,0);
				}
				cltLetterMsg_Guild_ConfirmChangeVillage clletter(TRUE);
				SendLetterMsg(  masterID , (cltLetterHeader*)&clletter );
			}
		}
		break;
	case CONFIRMRESULT_CHANGE_DENINE:
		{
			confirmStatus = GUILDSTATUS_NORMAL;
			cltLetterMsg_Guild_ConfirmChangeVillage clletter( FALSE );
			SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
		}
		break;
	case CONFIRMRESULT_CHANGE_FAILED:
		{
			confirmStatus = GUILDSTATUS_NORMAL;
			cltLetterMsg_Guild_ConfirmChangeVillage clletter( FALSE );
			SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
		}
		break;
	}

	for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
	{
		SI32 charID = pclCM->GetIDFromPersonID(pclGuild->clMember[i].siPersonID);
		if ( pclCM->IsValidID(charID) ) // 접속해있다면
		{
			if ( confirmStatus )
				pclCM->CR[charID]->pclCI->clBI.clGuildRank.uiConfirm = GUILDSTATUS_NORMAL;
			else
				pclCM->CR[charID]->pclCI->clBI.clGuildRank.Init();

			// 통보해준다.
			cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
		}
		charID = pclCM->GetIDFromPersonID(pclGuild->clStandingMember[i].siPersonID);
		if ( pclCM->IsValidID(charID) ) // 접속해있다면
		{
			if ( confirmStatus )
				pclCM->CR[charID]->pclCI->clBI.clGuildRank.uiConfirm = GUILDSTATUS_NORMAL;
			else
				pclCM->CR[charID]->pclCI->clBI.clGuildRank.Init();

			// 통보해준다.
			cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
		}
	}

	// 길드 행수에게 메세지 전달
	SI32 charid = pclMsg->siCharID;
	if ( pclCM->IsValidID(charid) )
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
		if ( pclchar == NULL )
			return ;

		if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
		{
			cltGameMsgResponse_Guild_ConfirmCreateGuild clinfo(pclMsg->siResult,&pclMsg->clInfo,pclMsg->bConfirm);
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

	// 서버의 길드 정보를 업데이트

	pclGuild->Set(&pclMsg->clInfo);



	//if ( pclMsg->siResult == 1 )
	//{
	//	cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->siGuildUnique);
	//	if ( pclGuild == NULL ) return ;

	//	UI08 siStatus = pclGuild->uiConfirm ;

	//	// 허가일 때
	//	if ( pclMsg->bConfirm )
	//	{
	//		if ( pclMsg->clInfo.siUnique == 0 ) // 허가이지만 길드마스터에게 돈이 없어서 길드가 삭제되었다.
	//		{
	//			for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
	//			{
	//				SI32 charID = pclCM->GetIDFromPersonID(pclGuild->clMember[i].siPersonID);
	//				if ( charID ) // 접속해있다면
	//				{
	//					pclCM->CR[charID]->pclCI->clBI.clGuildRank.Init();

	//					// 통보해준다.
	//					cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
	//					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	//					((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
	//				}
	//				if ( pclGuild->clMember[i].siPersonID > 0 )
	//				{
	//					cltLetterMsg_Guild_ConfirmCreateGuild clletter(FALSE, MIN_GUILD_CREATE_FEE);
	//					SendLetterMsg(  pclGuild->clMember[i].siPersonID , (cltLetterHeader*)&clletter );
	//				}

	//				charID = pclCM->GetIDFromPersonID(pclGuild->clStandingMember[i].siPersonID);
	//				if ( charID ) // 접속해있다면
	//				{
	//					pclCM->CR[charID]->pclCI->clBI.clGuildRank.Init();

	//					// 통보해준다.
	//					cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
	//					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	//					((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
	//				}

	//				if ( pclGuild->clStandingMember[i].siPersonID > 0 )
	//				{
	//					cltLetterMsg_Guild_ConfirmCreateGuild clletter(FALSE, MIN_GUILD_CREATE_FEE);
	//					SendLetterMsg(  pclGuild->clStandingMember[i].siPersonID , (cltLetterHeader*)&clletter );
	//				}
	//			}
	//		}
	//		else // 길드가 정상적으로 허가되었다.
	//		{
	//			SI32 masterID = pclCM->GetIDFromPersonID(pclGuild->siMasterPersonID) ;
	//			if ( masterID > 0 )
	//			{
	//				if ( pclCM->IsValidID(masterID) )
	//				{
	//					cltCharServer* pclmaster = (cltCharServer*)pclCM->CR[masterID];
	//					if ( pclmaster == NULL )
	//						return ;

	//					pclmaster->pclCI->clBank.SetMoney(pclMsg->siLeftMoney);
	//					// 통보해서 업데이트해준다.
	//					pclmaster->SetUpdateSwitch(UPDATE_BANK,true,0);
	//				}
	//			}
	//			if ( pclGuild->siMasterPersonID > 0 )
	//			{
	//				if ( siStatus == GUILDSTATUS_STANDING )
	//				{
	//					cltLetterMsg_Guild_ConfirmCreateGuild clletter(TRUE, MIN_GUILD_CREATE_FEE);
	//					SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
	//				}
	//				else if ( siStatus == GUILDSTATUS_CHANGEVILLAGE )
	//				{
	//					cltLetterMsg_Guild_ConfirmCreateGuild clletter(TRUE, 0);
	//					SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
	//				}
	//			}

	//			for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
	//			{
	//				SI32 charID = pclCM->GetIDFromPersonID(pclGuild->clMember[i].siPersonID);
	//				if ( charID ) // 접속해있다면
	//				{
	//					pclCM->CR[charID]->pclCI->clBI.clGuildRank.uiConfirm = GUILDSTATUS_NORMAL ;

	//					// 통보해준다.
	//					cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
	//					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	//					((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
	//				}
	//			}
	//		}
	//	}
	//	else // 불허라서 길드가 삭제될 때
	//	{
	//		SI32 masterID = pclCM->GetIDFromPersonID(pclGuild->siMasterPersonID) ;
	//		if ( masterID > 0 )
	//		{
	//			if ( pclCM->IsValidID(masterID) )
	//			{
	//				cltCharServer* pclmaster = (cltCharServer*)pclCM->CR[masterID];
	//				if ( pclmaster == NULL )
	//					return ;

	//				// 통보해서 업데이트해준다.
	//				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclmaster->pclCI->clBI.clGuildRank,pclmaster->GetCharUnique());
	//				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	//				pclmaster->SendNeighbourMsg(&clMsg,true);
	//			}
	//		}
	//		if ( pclGuild->siMasterPersonID > 0 )
	//		{
	//			cltLetterMsg_Guild_ConfirmCreateGuild clletter(FALSE,0);
	//			SendLetterMsg(  pclGuild->siMasterPersonID , (cltLetterHeader*)&clletter );
	//		}

	//		for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
	//		{
	//			SI32 charID = pclCM->GetIDFromPersonID(pclGuild->clMember[i].siPersonID);
	//			if ( charID ) // 접속해있다면
	//			{
	//				pclCM->CR[charID]->pclCI->clBI.clGuildRank.Init();
	//				// 통보해준다.
	//				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
	//				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	//				((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
	//			}

	//			charID = pclCM->GetIDFromPersonID(pclGuild->clStandingMember[i].siPersonID);
	//			if ( charID ) // 접속해있다면
	//			{
	//				pclCM->CR[charID]->pclCI->clBI.clGuildRank.Init();
	//				// 통보해준다.
	//				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charID]->pclCI->clBI.clGuildRank,pclCM->CR[charID]->GetCharUnique());
	//				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
	//				((cltCharServer*)pclCM->CR[charID])->SendNeighbourMsg(&clMsg,true);
	//			}
	//		}
	//	}

	//	pclGuild->Set(&pclMsg->clInfo);

	//	SI32 charid = pclMsg->siCharID;
	//	if ( pclCM->IsValidID(charid) )
	//	{
	//		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
	//		if ( pclchar == NULL )
	//			return ;

	//		if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
	//		{
	//			cltGameMsgResponse_Guild_ConfirmCreateGuild clinfo(pclMsg->siResult,&pclMsg->clInfo,pclMsg->bConfirm);
	//			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CONFIRMCREATEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
	//			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	//		}
	//	}
	//}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_SETSECONDMASTER(sPacketHeader* pPacket)
{
	sDBResponse_Guild_SetSecondMaster* pclMsg = (sDBResponse_Guild_SetSecondMaster*)pPacket ;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clInfo.siUnique);
		if ( pclGuild == NULL ) return ;

		pclGuild->Set(&pclMsg->clInfo);

		SI32 siSecondMasterID = pclCM->GetIDFromPersonID(pclMsg->siSecondMasterPersonID) ;
		if ( pclCM->IsValidID(siSecondMasterID) )
		{
			if ( pclMsg->bSet )
			{
				pclCM->CR[siSecondMasterID]->pclCI->clBI.clGuildRank.siType = GUILDRANKTYPE_SECONDMASTER ;
			}
			else
			{
				pclCM->CR[siSecondMasterID]->pclCI->clBI.clGuildRank.siType = GUILDRANKTYPE_MEMBER ;
			}

			cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[siSecondMasterID]->pclCI->clBI.clGuildRank,pclCM->CR[siSecondMasterID]->GetCharUnique());
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			((cltCharServer*)pclCM->CR[siSecondMasterID])->SendNeighbourMsg(&clMsg,true);
		}

		if( pclMsg->siSecondMasterPersonID > 0 )
		{
			cltLetterMsg_Guild_SecondMaster clletter(pclMsg->bSet);
			SendLetterMsg(  pclMsg->siSecondMasterPersonID , (cltLetterHeader*)&clletter );
		}

		SI32 charid = pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				cltGameMsgResponse_Guild_SetSecondMaster clinfo(pclMsg->siResult,&pclMsg->clInfo,pclMsg->bSet);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETSECONDMASTER, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 11] // 길드 오피서
					// param1 == 길드 유니크, pszparam1 == 길드이름
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_SETSECONDMASTER, 
										 0, (cltCharServer*)pclchar, NULL, pclMsg->siSecondMasterPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0,
										 pclMsg->clInfo.siUnique, 0, 0, 0, 0, pclMsg->clInfo.GetName(), NULL);
				}

			}
		}
	}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_GIVEGUILDMONEY(sPacketHeader* pPacket)
{
	sDBResponse_Guild_GiveGuildMoney* pclMsg = (sDBResponse_Guild_GiveGuildMoney*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
		if ( pclGuild == NULL ) return ;

		pclGuild->Set(&pclMsg->clinfo);

		if ( pclMsg->siPersonID == 0 )
			return;

		SI64 addgp = pclMsg->siMoney / 10000 ;

		sDBRequest_Guild_ChangeGuildPoint clMsg(pclGuild->siUnique,pclMsg->siPersonID,(SI32)addgp,TYPE_GUILDPOINT_ADD_GIVEGUILDMONEY,100);
		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

		SI32 charid = pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				pclchar->pclCI->clIP.SetMoney(pclMsg->siLeftMoney);

				cltGameMsgResponse_Guild_GiveGuildMoney clinfo(pclMsg->siResult,&pclMsg->clinfo,pclMsg->siMoney,pclMsg->siLeftMoney);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_GIVEGUILDMONEY, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					// [로그추가 : 황진성 2007. 10. 10] // 길드 자금을 납부
					// Param1	 == 길드 유니크, 
					// pszParam1 == 길드 이름. 
					cltServer * pclServer = (cltServer*)pclClient;
					pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_GIVEMONEY,
													0, pclchar, NULL, 0, pclMsg->siMoney, pclMsg->siLeftMoney, NULL, 0, 0, 0, 0, 0, 0, 
													pclMsg->clinfo.siUnique, 0, 0, 0, 0, pclMsg->clinfo.szName, NULL);
				}
			}
		}

	}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_GIVE_FEE(sPacketHeader* pPacket)
{
	sDBResponse_Guild_Give_Fee* pclMsg = (sDBResponse_Guild_Give_Fee*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		if ( pclMsg->siFaultNum > 0 )
		{
			cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
			if ( pGuild == NULL )
				return ;

			if (pclMsg->siFaultNum == 1 )
			{
				//cltLetterMsg_Guild_Destroy clLetter();
				for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i++ )
				{
					if ( pGuild->clMember[i].siPersonID )
					{
						cltLetterMsg_Guild_Destroy clLetter(1);
						SendLetterMsg(  pGuild->clMember[i].siPersonID , (cltLetterHeader*)&clLetter );
					}
				}   
			}
			if (pclMsg->siFaultNum == 2 )
			{
				//cltLetterMsg_Guild_Destroy clLetter();
				for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i++ )
				{
					if ( pGuild->clMember[i].siPersonID )
					{
						cltLetterMsg_Guild_Destroy clLetter(2);
						SendLetterMsg(  pGuild->clMember[i].siPersonID , (cltLetterHeader*)&clLetter );
					}
				}
			}

			// 해체시킨다.
			if ( pclMsg->siFaultNum >= 3 )
			{
				for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
				{
					if ( pGuild->clMember[i].siPersonID )
					{
						SI32 charid = pclCM->GetIDFromPersonID(pGuild->clMember[i].siPersonID);
						if ( pclCM->IsValidID(charid) )
						{
							cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
							if ( pclchar )
							{
								pclchar->pclCI->clBI.clGuildRank.Init();

								cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank,pclchar->GetCharUnique());
								cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
								pclchar->SendNeighbourMsg(&clMsg,true);
							}
						}
						cltLetterMsg_Guild_Destroy clLetter(3);
						SendLetterMsg(  pGuild->clMember[i].siPersonID , (cltLetterHeader*)&clLetter );
					}
					else if ( pGuild->clStandingMember[i].siPersonID )
					{
						SI32 charid = pclCM->GetIDFromPersonID(pGuild->clStandingMember[i].siPersonID);
						if ( pclCM->IsValidID(charid) )
						{
							cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
							if ( pclchar )
							{
								pclchar->pclCI->clBI.clGuildRank.Init();
								cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank,pclchar->GetCharUnique());
								cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
								pclchar->SendNeighbourMsg(&clMsg,true);
							}
						}
						cltLetterMsg_Guild_Destroy clLetter(3);
						SendLetterMsg(  pGuild->clStandingMember[i].siPersonID , (cltLetterHeader*)&clLetter );
					}
				}
				pGuild->Init();
			}
			else if ( pclMsg->siFaultNum == 2 )
			{
				pGuild->Set(&pclMsg->clinfo);
			}
			else if ( pclMsg->siFaultNum == 1 )
			{
				pGuild->Set(&pclMsg->clinfo);
			}
		}
		else
		{
			cltMoney clmoney;
			clmoney.Set(pclMsg->siLeftStrMoney);
			pclGuildManager->SetMoney(pclMsg->clinfo.siVillageUnique,&clmoney);

			cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
			if ( pGuild )
				pGuild->Set(&pclMsg->clinfo);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [로그추가 : 황진성 2007. 10. 17] // 길드 수수료.
			// param1		 == 수수료를 지불한 길드 유니크.
			// Param2		 == 변경된 건물 자본금, 
			// param3		 == 임대료, 
			// param4		 == 돈이 모자라서 안 내놓았을 경우 실패횟수 - 3번째에 길드 해체
			// pszCharParam1 == 길드이름.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_GIVEFEE,
				0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->clinfo.siVillageUnique, 0, 0, 
				pclMsg->siGuildUnique, pclMsg->siLeftStrMoney, pclMsg->clinfo.siFee, pclMsg->siFaultNum, 0, 
				pclMsg->clinfo.szName, NULL);
		}
	}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_CHANGESTATUS(sPacketHeader* pPacket)
{
	sDBResponse_Guild_ChangeStatus* pclMsg = (sDBResponse_Guild_ChangeStatus*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
		if ( pGuild )
		{
			pGuild->Set(&pclMsg->clinfo);

			for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
			{
				if ( pGuild->clMember[i].siPersonID )
				{
					SI32 charid = pclCM->GetIDFromPersonID(pGuild->clMember[i].siPersonID);
					if ( pclCM->IsValidID(charid) )
					{
						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
						if ( pclchar )
						{
							cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank,pclchar->GetCharUnique());
							cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
							pclchar->SendNeighbourMsg(&clMsg,true);

							if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
							{
								// [로그추가 : 황진성 2007. 10. 10] // 길드 능력치가 변경.
								// param1	 == 길드 유니크, 
								// pszParam1 == 길드 이름, 
								// pszParam2 == 길드 상태 정보. < Str, Dex, Mag, Vit, Hnd, Luck, Bonus >
								TCHAR	str[50] = {'\0', };
								StringCchPrintf(str, sizeof(str), "%d, %d, %d, %d, %d, %d, %d",
																	pclMsg->clinfo.clStatus.siStr, 
																	pclMsg->clinfo.clStatus.siDex, 
																	pclMsg->clinfo.clStatus.siMag, 
																	pclMsg->clinfo.clStatus.siVit, 
																	pclMsg->clinfo.clStatus.siHnd, 
																	pclMsg->clinfo.clStatus.siLuk, 
																	pclMsg->clinfo.clStatus.siWis, 
																	pclMsg->clinfo.clStatus.siBonus);
								

								SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_STATUSCHANGE,
													 0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->clinfo.siVillageUnique, 0, 0, 
													 pclMsg->clinfo.siUnique, 0, 0, 0, 0, 
													 pclMsg->clinfo.szName, str);
							}
						}
					}
				}	
			}
		}

		SI32 charid = pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				cltGameMsgResponse_Guild_ChangeStatus clinfo(pclMsg->siResult,&pGuild->clStatus);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CHANGESTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_SETSTRFEERATE(sPacketHeader* pPacket)
{
	sDBResponse_Guild_SetStrFeeRate*pclMsg = (sDBResponse_Guild_SetStrFeeRate*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltStrInfo* pclstr = pclGuildManager->GetStrInfo(pclMsg->siVillageUnique);
		if(pclstr == NULL)return ;

		// PCK : 변경되기전의 수수료를 기록한다.
		SI32 siBeforeFeeRate = pclstr->clGuildStrInfo.clFee.GetFeeRate();

		pclstr->clGuildStrInfo.clFee.SetFeeRate(pclMsg->siFeeRate);

		SI32 charid = pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				cltGameMsgResponse_Guild_SetStrFeeRate clinfo(pclMsg->siResult,pclMsg->siVillageUnique,pclMsg->siFeeRate);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETSTRFEERATE, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
				{
					// [로그추가 : 황진성 2007. 10. 10] // 길드 건물의 수수료가 변경.
					// Param1 == 변경된 후의 수수료. Parma2 == 변경되기 전의 수수료.
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_SETSTRFEERATE,
										 0, (cltCharServer*)pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										 pclMsg->siFeeRate, siBeforeFeeRate, 0, 0, 0, NULL, NULL);
				}
			}
		}
	}
}


void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_LEVELUP(sPacketHeader* pPacket)
{
	sDBResponse_Guild_LevelUP* pclMsg = (sDBResponse_Guild_LevelUP*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
		if ( pGuild )
		{
			pGuild->Set(&pclMsg->clinfo);

			for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
			{
				if ( pGuild->clMember[i].siPersonID )
				{
					SI32 charid = pclCM->GetIDFromPersonID(pGuild->clMember[i].siPersonID);
					if ( pclCM->IsValidID(charid) )
					{
						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
						if ( pclchar )
						{
							cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank,pclchar->GetCharUnique());
							cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
							pclchar->SendNeighbourMsg(&clMsg,true);
						}
					}
				}
			}
		}
	}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_SETMARK(sPacketHeader* pPacket)
{
	sDBResponse_Guild_SetMark* pclMsg = (sDBResponse_Guild_SetMark*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
		if ( pGuild )
		{
			pGuild->Set(&pclMsg->clinfo);

			for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
			{
				SI32 personid = pGuild->clMember[i].siPersonID ;
				if ( personid )
				{
					SI32 charid = pclCM->GetIDFromPersonID(personid);
					if ( pclCM->IsValidID(charid) )
					{
						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
						if ( pclchar )
						{
							pclchar->pclCI->clBI.clGuildRank.siMarkIndex = pclMsg->clinfo.clGPoint.siMarkIndex ;

							cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank,pclchar->GetCharUnique());
							cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
							pclchar->SendNeighbourMsg(&clMsg,true);
						}
					}
				}
			}

			SI32 charid = pclMsg->siCharID;
			if ( pclCM->IsValidID(charid) )
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
				if ( pclchar == NULL )
					return ;

				if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
				{
					cltGameMsgResponse_Guild_SetMark clinfo(pclMsg->siResult,pclMsg->clinfo.clGPoint.siMarkIndex);
					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETMARK, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
	}
}

void cltServer::DOMSG_DBMSG_RESPONSE_GUILD_CHANGENAME(sPacketHeader* pPacket)
{
	sDBResponse_Guild_ChangeName* pclMsg = (sDBResponse_Guild_ChangeName*)pPacket;

	if ( pclMsg->siResult == 1 )
	{
		cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
		if ( pGuild )
		{
			pGuild->Set(&pclMsg->clinfo);

			for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
			{
				SI32 personid = pGuild->clMember[i].siPersonID ;
				if ( personid )
				{
					SI32 charid = pclCM->GetIDFromPersonID(personid);
					if ( pclCM->IsValidID(charid) )
					{
						cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
						if ( pclchar )
						{
							MStrCpy(pclchar->pclCI->clBI.clGuildRank.szGuildName,
									pclMsg->clinfo.szName,MAX_GUILD_NAME);

							cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank,pclchar->GetCharUnique());
							cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
							pclchar->SendNeighbourMsg(&clMsg,true);
						}
					}
				}
			}

			SI32 charid = pclMsg->siCharID;
			if ( pclCM->IsValidID(charid) )
			{
				cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
				if ( pclchar == NULL )
					return ;

				if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
				{
					cltGameMsgResponse_Guild_ChangeName clinfo(pclMsg->siResult,pclMsg->clinfo.szName);
					cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CHANGENAME, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);

					
					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
					{
						// [로그추가 : 황진성 2007. 10. 10] // 길드 이름 변경.
						// param1 == 길드 유니크, pszParam1 == 변경되는 이름.
						cltServer * pclServer = (cltServer*)pclClient;
						pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_NAMECHANGE,
														0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
														pclMsg->clinfo.siUnique, 0, 0, 0, 0, pclMsg->clinfo.szName, NULL);
					}

				}
			}
		}
	}
	else if ( pclMsg->siResult == -2 )
	{
		SI32 charid = pclMsg->siCharID;
		if ( pclCM->IsValidID(charid) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
			if ( pclchar == NULL )
				return ;

			if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
			{
				cltGameMsgResponse_Guild_ChangeName clinfo(pclMsg->siResult,pclMsg->clinfo.szName);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CHANGENAME, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_CHANGEMASTER(sPacketHeader* pPacket)
{
	sDBResponse_Guild_ChangeMaster* pclMsg = (sDBResponse_Guild_ChangeMaster*)pPacket;

	if( pclMsg->siResult < 1 )
		return;

	cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clInfo.siUnique);

	if ( pclGuild == NULL ) return ;

	pclGuild->Set(&pclMsg->clInfo);

	SI32 newMasterID = pclCM->GetIDFromPersonID( pclMsg->siNewPersonID ) ;
	if ( pclCM->IsValidID(newMasterID) )
	{
		pclCM->CR[newMasterID]->pclCI->clBI.clGuildRank.siType = GUILDRANKTYPE_MASTER;

		cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[newMasterID]->pclCI->clBI.clGuildRank,pclCM->CR[newMasterID]->GetCharUnique());
		cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		((cltCharServer*)pclCM->CR[newMasterID])->SendNeighbourMsg(&clMsg,true);
	}


	if( pclMsg->siNewPersonID > 0 )
	{
		cltLetterMsg_Guild_ChangeMaster clletter;
		SendLetterMsg(  pclMsg->siNewPersonID , (cltLetterHeader*)&clletter );
	}


	SI32 charid = pclMsg->siCharID;

	if  ( pclCM->IsValidID(charid) )
	{
		pclCM->CR[charid]->pclCI->clBI.clGuildRank.siType = GUILDRANKTYPE_MEMBER;

		cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclCM->CR[charid]->pclCI->clBI.clGuildRank,pclCM->CR[charid]->GetCharUnique());
		cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
		((cltCharServer*)pclCM->CR[charid])->SendNeighbourMsg(&clMsg,true);
	}


	if ( pclCM->IsValidID(charid) )
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
		if ( pclchar == NULL )
			return ;

		if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
		{
			cltGameMsgResponse_Guild_ChangeMaster clinfo(pclMsg->siResult,&pclMsg->clInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CHANGEMASTER, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);


			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			{
				// [로그추가 : 황진성 2007. 10. 10] // 길드 마스터 변경
				// param1		 == 길드유니크, 
				// pszCharParam1 == 길드 이름.
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_MASTERCHANGE,
												0, pclchar, NULL, pclMsg->siNewPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->clInfo.siVillageUnique, 0, 0, 
												pclMsg->clInfo.siUnique, 0, 0, 0, 0,
												pclMsg->clInfo.szName, NULL);
			}
		}
	}
}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_CLOSEGUILD(sPacketHeader* pPacket)
{
	sDBResponse_Guild_CloseGuild* pclMsg = (sDBResponse_Guild_CloseGuild*)pPacket;

	if( pclMsg->siResult < 1 )
		return;

	pclGuildManager->DeleteGuildInfo( pclMsg->siGuildUnique );

	SI32 charid = pclClient->pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	if ( pclCM->IsValidID(charid) == false )
		return;

	//cltGameMsgResponse_Guild_CloseGuild clinfo(pclMsg->szGuildName);
	//cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CLOSEGUILD, sizeof(clinfo), (BYTE*)&clinfo);
	//pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg);
}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDWAR_ADDRESERVE(sPacketHeader* pPacket)
{
	sDBResponse_GuildWar_AddReserve* pclMsg = (sDBResponse_GuildWar_AddReserve*)pPacket;

	if( pclMsg->siResult < 1 )
		return;

	// 아무것도 처리하지 않는다
	
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [로그추가 : 황진성 2007. 10. 10] // 길드전 예약. (수정필요 : 얻을 정보가 없음 request에 주석처리된 로그부분을 사용할건가?)
		// param1 == Field Index
		// param2 == Room Index
		// param3 == Home Guild Unique.
		// param4 == Away Guild Unique.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_WARADDRESERVE,
										0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										pclMsg->siFieldIndex, pclMsg->siRoomIndex, pclMsg->siHomeGuildUnique, pclMsg->siAwayGUildUnique, 0, 
										NULL, NULL);
	}
}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDWAR_RESERVELIST(sPacketHeader* pPacket)
{
	sDBResponse_GuildWar_ReserveList* pclMsg = (sDBResponse_GuildWar_ReserveList*)pPacket;

	if( pclMsg->siResult < 1)
		return;

	SI32 field = pclMsg->siFieldIndex;
	for( SI32 i = 0; i < GUILDWAR_ROOMNUM; i++ )
	{
		SI32 room = pclMsg->clUnit[i].m_siRoomIndex;

		if ( IsCountrySwitch(Switch_NewGuildHuntMapWar) )	// 변경후
		{
			if( field < 0 || field >= GUILDWAR_FIELDNUM_OLD )	continue;
			if( room < 0  || room >= GUILDWAR_ROOMNUM )	continue;
			pclGuildWarReserveManager->m_clUnit[field][room].Set( pclMsg->clUnit[i].m_siRoomIndex,
				pclMsg->clUnit[i].m_siHomeGuildUnique,
				pclMsg->clUnit[i].m_szHomeGuildName,
				pclMsg->clUnit[i].m_siAwayGuildUnique,
				pclMsg->clUnit[i].m_szAwayGuildName);
		}
		else	// 변경전
		{
			if( field < 0 || field >= GUILDWAR_FIELDNUM)	continue;
			if( room < 0  || room >= GUILDWAR_ROOMNUM )	continue;
			pclGuildWarReserveManager->m_clUnit[field][room].Set( pclMsg->clUnit[i].m_siRoomIndex,
				pclMsg->clUnit[i].m_siHomeGuildUnique,
				pclMsg->clUnit[i].m_szHomeGuildName,
				pclMsg->clUnit[i].m_siAwayGuildUnique,
				pclMsg->clUnit[i].m_szAwayGuildName);
		}
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GuildWar_ReserveList"));
}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_APPLY(sPacketHeader* pPacket)
{
	sDBResponse_GuildDungeon_Apply* pclMsg = (sDBResponse_GuildDungeon_Apply*)pPacket;

	SI32 charid = pclMsg->siCharID;

	if ( pclCM->IsValidID(charid) == false )
		return;

	if ( pclMsg->siResult > 0 )
	{
		cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->siGuildUnique);

		if ( pGuild )
		{
			pGuild->Set( &(pclMsg->clGuildInfo) );

			// 길드원들에게 성공 메세지 날림
			cltLetterMsg_GuildDungeon_Apply clletter( pclMsg->siMapIndex );
			for ( SI16 i=0; i<MAX_GUILD_MEMBER_NUM; i++ )
			{
				if ( pGuild->clMember[i].siPersonID > 0 )
				{
					SendLetterMsg( pGuild->clMember[i].siPersonID, (cltLetterHeader*)&clletter);
				}
			}

			cltGameMsgResponse_GuildDungeon_Apply clinfo( true, pclMsg->siMapIndex );
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILDDUNGEON_APPLY, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg);

			cltGameMsgResponse_GuildHuntMapWarWar_HaveMapIndex clinfo2( pclMsg->siMapIndex );
			cltMsg clMsg2(GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_HAVEMAP, sizeof(clinfo2), (BYTE*)&clinfo2);
			pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg2);


		
			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [로그추가 : 황진성 2007. 10. 15] // 사냥터 소유권 신청하기.
				// param1		 == 길드 유니크, 
				// param2		 == 맵번호.
				// pszCharParam1 == 길드이름.
				cltServer* pclserver = (cltServer*)pclClient;
				pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_DUNGEONAPPLY, 
												0, (cltCharServer*)pclCM->CR[charid], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0,
												pclMsg->siGuildUnique, pclMsg->siMapIndex, 0, 0, 0, 
												pclMsg->clGuildInfo.szName, NULL);
			}
		}
	}
	else
	{
		cltGameMsgResponse_GuildDungeon_Apply clinfo( false, pclMsg->siMapIndex );
		cltMsg clMsg(GAMEMSG_RESPONSE_GUILDDUNGEON_APPLY, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[charid]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_SAVESTATUS(sPacketHeader* pPacket)
{
	// 할일없음..;;
}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_LOADSTATUS(sPacketHeader* pPacket)
{
	sDBResponse_GuildDungeon_LoadStatus* pclMsg = (sDBResponse_GuildDungeon_LoadStatus*)pPacket;

	if ( pclMsg->siResult > 0 )
	{
		pclGuildDungeonMgr->SetStatus( pclMsg->siMapIndex, pclMsg->siGuildUnique, pclMsg->siExp, pclMsg->siMoney);
	}
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GuildDungeon_LoadStatus"));

}
// 길드 사냥터 변경 
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDDUNGEON_TRANSFEROWNERSHIP(sPacketHeader* pPacket)
{
	sDBResponse_GuildDungeon_TransferOwnership* pclMsg = (sDBResponse_GuildDungeon_TransferOwnership*)pPacket;

	cltServer* pclserver = (cltServer*)pclClient;

	if ( pclMsg->siResult > 0 )
	{
		// 길드 던전의 자료를 통해 보상
		for ( SI32 i = 0; i < MAX_GUILDDUNGEON; i++ )
		{
			SI32	m_siitemUnique	=	0;	// 길드 사냥터로 받을 아이템 유니크
			SI32	siitemNum		=	0;			// 길드 사냥터로 받을 아이템 갯수
			// 아이템을 지급한다.
			if ( pclGuildDungeonMgr->m_clUnit[i].m_siGuildUnique )
			{
				cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo( pclGuildDungeonMgr->m_clUnit[i].m_siGuildUnique );
				if ( pGuild )
				{
					//--------------------------------------------------------------------------
					SI32 siGuildGP		=	0	;	// 길드 전 GP
					// 길드전 GP계산 값
					if ( true == IsWhereServiceArea(ConstServiceArea_NHNChina | ConstServiceArea_EUROPE) )
					{
						siGuildGP = Calc_GiveGP_Exception( pclGuildDungeonMgr->m_clUnit[i].GetExp() );
					}
					else
					{
						siGuildGP = Calc_GiveGP( pclGuildDungeonMgr->m_clUnit[i].GetExp() );
					}
					m_siitemUnique = 0;	// 이전에 유니크를 가지고 있을수 있으니 0으로 초기화 
					siitemNum = pclClient->pclGuildDungeonItemMgr->GetItem( pclGuildDungeonMgr->m_clUnit[i].m_siMapIndex, siGuildGP, &m_siitemUnique );	// 받을 아이템의 유니크와 갯수를 가지고 온다.
					// 역참으로 아이템을 지급 한다.
					if ( siitemNum > 0 && m_siitemUnique > 0)	
					{
						// 역참으로 아이템 지급한다.
						cltItem clInputItem;
						SI16 bRareSwitch = 0;
						pclClient->pclItemManager->MakeRandItemUnique(m_siitemUnique, &clInputItem, 0, 0, &bRareSwitch);
						clInputItem.siItemNum = siitemNum;
						pclserver->SendPostOfficeItemToPerson( pGuild->siMasterPersonID, &clInputItem , NULL ,0  );
					}
					//--------------------------------------------------------------------------
					//--------------------------------------------------------------------------
					for (  SI32 sii = 0; sii < MAX_GUILDDUNGEON; sii++ ) 
					{

						if ( pclGuildDungeonMgr->m_clUnit[i].m_siGuildUnique != pclMsg->clChangedGuildInfo[sii].siGuildUnique ) continue	;
						SI32 AddExp   = pclMsg->clChangedGuildInfo[sii].siAddExp;
						GMONEY AddMoney = pclMsg->clChangedGuildInfo[sii].siAddMoney;
						if(AddExp   < 0)	AddExp	 = 0;
						if(AddMoney < 0)	AddMoney = 0;

						//돈을 넣는다 .
						if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar ))
						{
							// NEW 길드자금으로 돈을 넣는다

							sDBRequest_Guild_GiveGuildMoney clMsg(pclGuildDungeonMgr->m_clUnit[i].m_siGuildUnique , 0 ,0 , AddMoney);
							SendDBMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);

						}
						else
						{
							// (구)	길드장에게 돈을 준다
							SI32 masterCharID = pclCM->GetIDFromPersonID(pGuild->siMasterPersonID);
							if(pclClient->pclCM->IsValidID(masterCharID))
							{
								pclCM->CR[ masterCharID ]->pclCI->clBank.IncreaseMoney( AddMoney );
								pclCM->CR[ masterCharID ]->SetUpdateSwitch(UPDATE_BANK, true, 0);
							}
						}
						//--------------------------------------------------------------------------
						//--------------------------------------------------------------------------					
						// 길드 포인트 를 올린다.
						pGuild->clGPoint.siPoint += AddExp ;
						// 경험치가 올라가면 레벨도 업시킨다.
						SI32 level = pGuild->clGPoint.siLevel ;
						SI32 nextlevel = GetLevelFromExp( pGuild->clGPoint.siPoint, EXPFORLEVEL_TYPE_GUILD );
						if ( level < nextlevel && pGuild->clGPoint.siLevel < MAX_GUILD_LEVEL )
						{
							sDBRequest_Guild_LevelUP clMsg(pGuild->siUnique,nextlevel);
							pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
						}
						// 경험치가 다운되면 레벨도 다운시킨다.
						level = pGuild->clGPoint.siLevel ;
						nextlevel = GetLevelFromExp( pGuild->clGPoint.siPoint, EXPFORLEVEL_TYPE_GUILD );
						if ( level > nextlevel && pGuild->clGPoint.siLevel > 1 )
						{
							sDBRequest_Guild_LevelUP clMsg(pGuild->siUnique,nextlevel);
							pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
						}
						// 길드원에게 획득한 경험치와 돈을 알려준다.
						cltLetterMsg_GuildDungeon_ChangeGPMoney clletter( 	AddExp, AddMoney, m_siitemUnique,siitemNum		);

						for ( SI16 j=0; j<MAX_GUILD_MEMBER_NUM; j++ )
						{
							if ( pGuild->clMember[j].siPersonID > 0 )
							{
								if( pGuild->clMember[j].siType == GUILDRANKTYPE_STANDING )
									continue;

								SendLetterMsg( pGuild->clMember[j].siPersonID, (cltLetterHeader*)&clletter);
							}
						}
					}

					//--------------------------------------------------------------------------
					//--------------------------------------------------------------------------
				}
			}
		}

		for ( SI32 i = 0; i < MAX_GUILDDUNGEON; i++ )
		{
			// 사냥터 소유권 초기화
			pclGuildDungeonMgr->m_clUnit[i].m_siGuildUnique = 0;
			pclGuildDungeonMgr->m_clUnit[i].m_siExp = 0;
			pclGuildDungeonMgr->m_clUnit[i].m_siMoney = 0;

			// 사냥터 소유권 이전
			if ( pclMsg->clNewOwnerInfo[i].siGuildUnique > 0  )
			{
				pclGuildDungeonMgr->SetStatus( pclMsg->clNewOwnerInfo[i].siMapindex, pclMsg->clNewOwnerInfo[i].siGuildUnique, 0, 0 );

				cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clNewOwnerInfo[i].siGuildUnique );
				if ( pGuild )
				{
					// 자막광고로 알려줌
					TCHAR* pTitle = GetTxtFromMgr(6755);
					TCHAR* pText = GetTxtFromMgr(6756);
					TCHAR buf[256] = TEXT("");
					StringCchPrintf( buf, 256, pText, pGuild->GetName(),
						pclMapManager->GetMapName(pclMsg->clNewOwnerInfo[i].siMapindex));

					pclserver->PushSpecialNews( buf, pTitle ,NULL, CAPTIONKIND_GUILD,CAPTIONKIND_NEWSCHAT);


					// 길드원들에게 소유권을 얻었음을 알려준다.
					cltLetterMsg_GuildDungeon_GetOwnerShip clletter( pclMsg->clNewOwnerInfo[i].siMapindex );
					for ( SI16 j=0; j<MAX_GUILD_MEMBER_NUM; j++ )
					{
						if ( pGuild->clMember[j].siPersonID > 0 )
						{
							if( pGuild->clMember[j].siType == GUILDRANKTYPE_STANDING )
								continue;

							SendLetterMsg( pGuild->clMember[j].siPersonID, (cltLetterHeader*)&clletter);
						}
					}
				}
			}
		}

		for ( SI32 i = 0; i < MAX_GUILD_NUM ; i ++ )
		{
			// 사냥터 소유권 신청 초기화
			pclGuildManager->clUnit[i].clGPoint.siApplyMapIndex = 0;
			pclGuildManager->clUnit[i].clGPoint.siBattleGP = 0;
			// 사냥터 소유정보가 초기화 되었습니다.
			// 전쟁안내원에게 사냥터를 신청할수 있습니다
			for ( SI32 siMember = 0; siMember < MAX_GUILD_MEMBER_NUM; siMember++) 
			{
				SI32 siMemberID = pclGuildManager->clUnit[i].clMember[siMember].siPersonID	;
				SI32 siID = pclserver->pclCM->GetIDFromPersonID(siMemberID)	;
				if (pclserver->pclCM->IsValidID(siID)) 
				{
					SI32 returnval = SRVAL_RETURN_CANNOTGUILDWAR_RESEASON;
					pclserver->SendServerResponseMsg( 0, returnval,  0, 0, pclserver->pclCM->CR[siID]->GetCharUnique() );
				}
			}

		}
	}
}

// 미활동 휴면길드 체크 메세지
void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_CHECKDESTROY_MONTH(sPacketHeader* pPacket)
{
	sDBResponse_Guild_CheckDestroy_Month* pclMsg = (sDBResponse_Guild_CheckDestroy_Month*)pPacket;

	if(pclClient->IsCountrySwitch(Switch_Guild_DestroyOverMOnth)){
		// 리절트를 받아서 해당 길드는 휴면 길드로 간주 하고 해당 길드의 회원들을 탈퇴 시킨다.
		if ( pclMsg->siResult == 1) {// 지금은 임의의 결과 값
			for ( SI32 index = 0 ; index < MAX_GUILD_PER_DBLIST ; index ++ ){
				// 해당 길드 인덱스 정보를 가지고 온다
				cltGuildInfo* pGuildInfo = &pclMsg->clInfo[index] ;
				if ( pGuildInfo->siUnique <= 0 ) continue ;
				//// 모든 맴버의 탈퇴를 시킨다.
				//for( SI16 membernum = 0; membernum < MAX_GUILD_MEMBER_NUM; membernum++){
				//	if( pGuildInfo->clMember[ membernum ].siPersonID <= 0) continue;
				//	sDBRequest_Guild_LeaveGuild clMsg( pGuildInfo->clMember[membernum ].siPersonID, pGuildInfo->siUnique );
				//	pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
				//}
				// 자동삭제라는 리즌을 추가 해서 날려야 겠다
				sDBRequest_Guild_DestroyGuild clMsg( 0,0,pGuildInfo->siUnique,pGuildInfo->siVillageUnique, 4 );
				pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&clMsg);
			}
		}
	}

}
// 한달이상 접속 안한 길드 장 위임
void cltServer::DoMsg_DBMSG_RESPONSE_GUILD_NOTCONNECT_GUILDMASTER(sPacketHeader* pPacket)
{
	if( false == pclClient->IsCountrySwitch(Switch_Guild_GuildMaster_Change) )
	{
		return;
	}

	sDBResponse_Guild_NotConnect_ChangeMaster* pclMsg = (sDBResponse_Guild_NotConnect_ChangeMaster*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
	
	// 실패했다면
	if( pclMsg->siResult < 1 )
	{
		SI32 id	= pclMsg->siCharID;	
		SendServerResponseMsg(0, SRVAL_GUILD_FAIL_NOTCONNECTMASTER, 0, 0, pclCM->CR[id]->GetCharUnique() );
		return;
	}

	cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclMsg->clInfo.siUnique);
	if ( NULL == pclGuild )
	{
		return ;
	}
	pclGuild->Set(&pclMsg->clInfo);

	// 길드원 전체에게 쪽지 보내기
	for ( SI32 siIndex=0; siIndex<MAX_GUILD_MEMBER_NUM; ++siIndex )
	{
		SI32 siPersonID = pclGuild->clMember[siIndex].siPersonID;
		if ( 0 >= siPersonID )
		{
			continue;
		}

		SI32 siCharID = pclCM->GetIDFromPersonID( siPersonID );
		
		// 접속해 있다면
		if ( pclCM->IsValidID(siCharID) )
		{
			cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siCharID];
			if ( NULL == pclchar )
			{
				continue;
			}
			
			// 기존에 길드장은 길드원으로 변경(그런데 이곳으로 들어올 가능성은 있을까?)
			if ( (siPersonID != pclMsg->siNewPersonID) && (GUILDRANKTYPE_MASTER == pclchar->pclCI->clBI.clGuildRank.siType) )
			{
				pclchar->pclCI->clBI.clGuildRank.siType = GUILDRANKTYPE_MEMBER;
			}
			// 새롭게 바뀐 길드 마스터라면 권한 설정
			else if ( siPersonID == pclMsg->siNewPersonID )
			{
				pclchar->pclCI->clBI.clGuildRank.siType = GUILDRANKTYPE_MASTER;
			}

			cltGameMsgResponse_Guild_SetGuildStatus clinfo( &pclGuild->clStatus, &pclchar->pclCI->clBI.clGuildRank, pclchar->GetCharUnique());
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNeighbourMsg(&clMsg,true);
		}

		// 쪽지도 보냄
		cltLetterMsg_Guild_ChangeMaster_Notconnect_Month clletter( pclMsg->szNewPersonName );
		SendLetterMsg(  siPersonID, (cltLetterHeader*)&clletter );

	}


	SI32 charid = pclMsg->siCharID;

	if ( pclCM->IsValidID(charid) )
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charid];
		if ( pclchar == NULL )
			return ;

		if ( pclchar->pclCI->GetPersonID() == pclMsg->siPersonID )
		{
			cltGameMsgResponse_Guild_ChangeMaster clinfo(pclMsg->siResult,&pclMsg->clInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_CHANGEMASTER, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);


			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
			{
				// [성웅] 한달이상 미접속 길드 마스터 위임
				// param1		 == 길드 유니크, 
				// pszCharParam1 == 길드 이름.
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_GUILD, LOGCOMMAND_INDEX_GUILD_MASTERCHANGE_NOTCONNECT_MONTH,
					0, pclchar, NULL, pclMsg->siNewPersonID, 0, 0, NULL, 0, 0, 0, pclMsg->clInfo.siVillageUnique, 0, 0, 
					pclMsg->clInfo.siUnique, 0, 0, 0, 0,
					pclMsg->clInfo.szName, NULL);
			}
		}
	}

}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDHUNTMAPWAR_REGLIST(sPacketHeader* pPacket)
{
	sDBResponse_GuildHuntMapWar_SaveRegList* pclMsg = (sDBResponse_GuildHuntMapWar_SaveRegList*)pPacket;
	if ( NULL == pclMsg )			return;
	// 실패했다면
	if( pclMsg->siResult < 1 )		return;

	if ( pclMsg->siType == DelList)
	{
		// 신청 리스트에 해당 길드유니크를 넣는다.
		SI32 siGuildunique = pclMsg->siGuildUnique	;
		pclGuildHuntMapManager->Delete(siGuildunique)	;
	}
	else
	{
		// 신청 리스트에 해당 길드유니크를 넣는다.
		SI32 siGuildunique = pclMsg->siGuildUnique	;
		pclGuildHuntMapManager->add(siGuildunique)	;
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(siGuildunique);
		if ( pclGuild == NULL ) return ;

		if ( pclClient->IsCountrySwitch(Switch_NewGuildHuntMapWar))
		{
			// 길드원들에게 성공 메세지 날림
			cltLetterMsg_GuildDungeon_Apply_New clletter(0);
			for ( SI16 i=0; i<MAX_GUILD_MEMBER_NUM; i++ )
			{
				if ( pclGuild->clMember[i].siPersonID > 0 )
				{
					SendLetterMsg( pclGuild->clMember[i].siPersonID, (cltLetterHeader*)&clletter);
				}
			}
		}
		else
		{
			// 길드원들에게 성공 메세지 날림
			cltLetterMsg_GuildDungeon_Apply clletter(0);
			for ( SI16 i=0; i<MAX_GUILD_MEMBER_NUM; i++ )
			{
				if ( pclGuild->clMember[i].siPersonID > 0 )
				{
					SendLetterMsg( pclGuild->clMember[i].siPersonID, (cltLetterHeader*)&clletter);
				}
			}

			
		}
	}
}


void cltServer::DoMsg_DBMSG_RESPONSE_GUILDHUNTMAPWAR_GETTOP10(sPacketHeader* pPacket)
{
	cltServer* pclserver = (cltServer*)pclClient;

	sDBResponse_GuildHuntMapWar_Rank_Get* pclMsg = (sDBResponse_GuildHuntMapWar_Rank_Get*)pPacket;

	if(pclMsg == NULL)			return	;

	SI32 siPersonID = pclMsg->siPersonID;
	cltCharCommon* pclchar = pclserver->pclCM->GetChar( pclserver->pclCM->GetIDFromPersonID(siPersonID));

	// 길드 이름을 찍어 준다.
	for ( SI32 siGuildSLot = 0; siGuildSLot < MAX_GUILDHUNTWAR_RANK_NUM; siGuildSLot++) 
	{
		SI32 siGuildUnique = pclMsg->m_GuildList[siGuildSLot].GetGuildUnique()	;
			if(siGuildUnique == 0)	break;
		TCHAR* szGuildName = pclserver->pclGuildManager->GetGuildName( siGuildUnique );
		if(szGuildName == NULL) return;
		pclMsg->m_GuildList[siGuildSLot].SetGuildName(szGuildName)	;
	}
	cltGameMsgResponse_GuilHuntMapdWar_GetTopTen clinfo( pclMsg->m_GuildList);
	cltMsg clMsg(GAMEMSG_RESPONSE_GUILDHUNTMAPWAR_GETTOP10, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);	
}
void cltServer::DoMsg_DBMSG_RESPONSE_GUILDHUNTMAPWAR_RESULT(sPacketHeader* pPacket)
{
	sDBResponse_GuildHuntMapWar_Result* pclMsg = (sDBResponse_GuildHuntMapWar_Result*)pPacket;
	if (pclMsg->siResult == 1 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo( pclMsg->siGuildunique );
		if ( pclGuild == NULL ) return ;
		pclGuild->clGPoint.siBattleGP	=	pclGuild->clGPoint.siBattleGP+pclMsg->siBGP	;	// bgp갱신

		for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
		{
			// 케릭터 아이디를 찾는다.
			//------------------------------------------------------------------------------------
			SI32 siMemberPeronID = pclGuild->clMember[i].siPersonID	;

			SI32 sicharID	=	pclClient->pclCM->GetIDFromPersonID(siMemberPeronID)	;
			//------------------------------------------------------------------------------------
			if ( pclClient->pclCM->IsValidID(sicharID) ) 
			{
				cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[sicharID];
				cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank, pclchar->CharUnique);
				cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNeighbourMsg(&clMsg,true);
			}
		}
	}
}
