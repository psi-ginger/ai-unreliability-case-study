//---------------------------------
// 2003/9/27 ���°�
//---------------------------------

#include "../Common/CommonHeader.h"
#include "../Server/Server.h"
#include "DevTool/DevServerInfo/DevServerInfo.h"

//-------------------------------------
// Common
//-------------------------------------
#include "PersonInfo.h"
#include "../../Common/Map/Map.h"
#include "../../Common/Order/Order.h"
#include "../../Common/PrivateTradeOrderMgr/PrivateTradeOrderMgr.h"
#include "Char\CharManager\CharManager.h"
#include "GachaManager/GachaManager.h"

#include "../../../Server/AuthServer/AuthMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-System.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Item.h"

#include "../../CommonLogic/Msg/MsgType-PrivateTrade.h"
#include "../../CommonLogic/MsgType-System.h"


#include "../../../DBManager/GameDBManager_World/DBMsg-Structure.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-CityHall.h"
#include "../../../DBManager/GameDBManager_World/DBGameLogMsg.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Friend.h"

#include "..\TraceBoot.h"

#include "MsgRval-Define.h"
#include "MsgType-Item.h"
#include "../Minister/MinisterMgr.h"
#include "../Minister/Minister-Goonzu.h"
#include "../Minister/Minister-Byungzo.h"

#include "../Lib/HashTable/HashTable_StringInt.h"

#include "../../NLib/NUtil.h"

#include "../../../AdminSystem/MasterServer/MasterMsg.h"
#include "../../CommonLogic/CityHall/CityHall.h"

extern cltCommonLogic* pclClient;

// ����� ���� ���� ����. 
void cltServer::DoMsg_DBMSG_RESPONSE_USERCHARINFO(sPacketHeader* pPacket)
{
	sDBResponse_UserCharInfo* pclMsg = (sDBResponse_UserCharInfo* )pPacket;

	//============================================
	// GMTOOL�� �����̴�.
	if(pclMsg->siRequestType == 1)
	{
		/*
		sResponse_GMC_UserCharInfo	clinfo;
		clinfo.m_siCharID = pclMsg->usCharID;

		StringCchCopy(clinfo.m_szAccountID, sizeof(clinfo.m_szAccountID), pclMsg->szAccountID);
		StringCchCopy(clinfo.m_szPassword, sizeof(clinfo.m_szPassword), pclMsg->szPassword);
		clinfo.m_bConnectSwitch		= pclMsg->bConnectSwitch;

		clinfo.m_clTotalMoney		= pclMsg->clTotalMoney.itMoney;
		clinfo.m_clBankMoney		= pclMsg->clBankMoney.itMoney;

		clinfo.m_siWorldTradeMoney	= pclMsg->siWorldTradeMoney;

		clinfo.m_siVillageUnique	= pclMsg->siVillageUnique;
		clinfo.m_uiWarnMode			= pclMsg->uiWarnMode;

		clinfo.m_uiBlockMode		= pclMsg->uiBlockMode;
		StringCchCopy(clinfo.m_szBlockReason, sizeof(clinfo.m_szBlockReason), pclMsg->szBlockReason);
		StringCchCopy(clinfo.m_szBlockStartDate, sizeof(clinfo.m_szBlockStartDate), pclMsg->szBlockStartDate);
		StringCchCopy(clinfo.m_szBlockEndDate, sizeof(clinfo.m_szBlockEndDate), pclMsg->szBlockEndDate);

		clinfo.m_siPersonID			= pclMsg->siPersonID;
		clinfo.m_siLevel			= pclMsg->siLevel;
		clinfo.m_siMoney			= pclMsg->siMoney;

		clinfo.m_siMapIndex			= pclMsg->siMapIndex;
		clinfo.m_siX				= pclMsg->siX;
		clinfo.m_siY				= pclMsg->siY;
		clinfo.m_siGoonzuFriends	= pclMsg->siGoonzuFriends;

		StringCchCopy(clinfo.m_szRecommender, sizeof(clinfo.m_szRecommender), pclMsg->szRecommender);
		StringCchCopy(clinfo.m_szFatherName, sizeof(clinfo.m_szFatherName), pclMsg->szFatherName);
		clinfo.m_siStudentLevel		= pclMsg->siStudentLevel;

		StringCchCopy(clinfo.m_szMakeDate, sizeof(clinfo.m_szMakeDate), pclMsg->szMakeDate);

		for(SI32 i=0; i<110; i++)
		{
			clinfo.m_siItemUnique[i]	= pclMsg->clItem[i].siUnique;
			clinfo.m_siItemCount[i]		= pclMsg->clItem[i].siItemNum;
		}

		if( pclAgentProcess != NULL &&
			pclAgentProcess->GetAgentSession() != NULL &&
			pclAgentProcess->GetAgentSession()->GetSendBuffer() != NULL)
		{
			pclAgentProcess->GetAgentSession()->GetSendBuffer()->Write((BYTE*)&clinfo);
		}
		*/
		return;
	}
	//============================================

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id)	== false)return ;
	if(pclCM->CR[id]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL)return ;

	if(pclMsg->siResult == 0)return ;
	
	SI32 mapindex = pclMsg->siMapIndex;
	SI32 x			= pclMsg->siX;
	SI32 y			= pclMsg->siY;

	SI32 siCharUnique = 0;

	bool bconnectswitch = false;
	if(pclMsg->siPersonID)
	{
		// ���ӿ� �������̸�, 
		SI32 charid = pclCM->GetIDFromPersonID( pclMsg->siPersonID );
		if(charid)
		{
			mapindex	= pclCM->CR[charid]->GetMapIndex();
			x			= pclCM->CR[charid]->GetX();
			y			= pclCM->CR[charid]->GetY();

			bconnectswitch = true;

			siCharUnique = pclCM->CR[charid]->GetCharUnique();
		}
	}

#if defined(_DEBUG)
	// ��Ƽȹ�渷��
//SI32 ReceivedPacketsize = sizeof( sDBResponse_UserCharInfo );

//	SI32 SendPacketsize = sizeof( cltGameMsgResponse_UserCharInfo );
#endif


	cltGameMsgResponse_UserCharInfo clinfo(pclMsg->szAccountID, pclMsg->szPassword, &pclMsg->clTotalMoney,
											&pclMsg->clBankMoney, pclMsg->siWorldTradeMoney, bconnectswitch, pclMsg->siPersonID, siCharUnique, pclMsg->siLevel, pclMsg->siMoney, mapindex, x, y, 
											pclMsg->siGoonzuFriends, pclMsg->szOtherPersonName1, pclMsg->szOtherPersonName2 ,//pclMsg->clItem,
											&(pclMsg->clSummonInfo), pclMsg->siVillageUnique,
											pclMsg->szRecommender,pclMsg->szFatherName,pclMsg->siStudentLevel,pclMsg->szMakeDate,
											pclMsg->m_clBlockLog );
	cltMsg clMsg(GAMEMSG_RESPONSE_USERCHARINFO, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);



	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// TEMP LEEKH -------------------------------------------------------------------
		// [�α��߰� : Ȳ���� 2007. 10. 18] // ����� ĳ���� ���� ��û. (Ȯ���ʿ� : ������ �����ҵ�.)
		// param1	 == ���ӿ���, 
		// param2	 == ����,
		// param3	 == ������ȣ, 
		// param4	 == ����ģ��,
		// pszParam1 == ��õ��, ����, 
		// pszParam2 == ĳ���� ���� �ñ�.
		TCHAR str[2][50];
		ZeroMemory(str, sizeof(str));
		StringCchPrintf(str[0], sizeof(str[0]), TEXT("Recommender: %s, FatherName: %s"),	
														pclMsg->szRecommender, 
														pclMsg->szFatherName 	);

		StringCchPrintf(str[1], sizeof(str[1]), TEXT("MakeDate: %s"),	pclMsg->szMakeDate	);

		
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SYSTEM, LOGCOMMAND_INDEX_SYSTEM_USER_INFO_GET, 
										0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, pclMsg->siMoney, NULL, 0, 0, 0, pclMsg->siVillageUnique, 0, 0, 
										pclMsg->bConnectSwitch, pclMsg->siLevel, pclMsg->siMapIndex, pclMsg->siGoonzuFriends, 0, 
										str[0], str[1]);
		// TEMP LEEKH -------------------------------------------------------------------
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_USERCHARITEMINFO(sPacketHeader* pPacket)
{
	sDBResponse_UserCharItemInfo* pclMsg = (sDBResponse_UserCharItemInfo*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer(pclMsg->m_usCharID);
	if ( NULL == pclChar )
	{
		return;
	}

	if ( pclChar->pclCI->GetPersonID() != pclMsg->m_siPersonID )
	{
		return;
	}

	if ( pclChar->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
	{
		return;
	}

	cltGameMsgResponse_UserCharItemInfo clInfo( pclMsg->m_clItem );
	cltMsg clMsg(GAMEMSG_RESPONSE_USERCHARITEMINFO, sizeof(clInfo), (BYTE*)&clInfo);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_USERCHARITEMINFO2(sPacketHeader* pPacket)
{
	sDBResponse_UserCharItemInfo2* pclMsg = (sDBResponse_UserCharItemInfo2*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	cltCharServer* pclChar = pclCM->GetCharServer(pclMsg->m_usCharID);
	if ( NULL == pclChar )
	{
		return;
	}

	if ( pclChar->pclCI->GetPersonID() != pclMsg->m_siPersonID )
	{
		return;
	}

	if ( pclChar->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL )
	{
		return;
	}

	cltGameMsgResponse_UserCharItemInfo2 clInfo( pclMsg->m_clSpaceBox, pclMsg->m_clTreasureBox );
	cltMsg clMsg(GAMEMSG_RESPONSE_USERCHARITEMINFO2, sizeof(clInfo), (BYTE*)&clInfo);
	pclChar->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_ACCOUNTINFO(sPacketHeader* pPacket)
{
	sDBResponse_AccountInfo* pclMsg = (sDBResponse_AccountInfo* )pPacket;

	//============================================
	// GMTOOL�� �����̴�.
	if(pclMsg->siRequestType == 1)
	{
		sResponse_GMC_AccountCharInfo	clinfo;
		clinfo.m_siCharID = pclMsg->usCharID;
		MStrCpy(clinfo.m_szAccountID, pclMsg->szAccountID, sizeof(clinfo.m_szAccountID));
		MStrCpy(clinfo.m_szServerName, szServerName, sizeof(clinfo.m_szServerName));

		MStrCpy(clinfo.m_szCharName1, pclMsg->szPersonName1, sizeof(clinfo.m_szCharName1));
		MStrCpy(clinfo.m_szCharName2, pclMsg->szPersonName2, sizeof(clinfo.m_szCharName2));
		MStrCpy(clinfo.m_szCharName3, pclMsg->szPersonName3, sizeof(clinfo.m_szCharName3));

		if( pclAgentProcess != NULL &&
			pclAgentProcess->GetAgentSession() != NULL &&
			pclAgentProcess->GetAgentSession()->GetSendBuffer() != NULL)
		{
			pclAgentProcess->GetAgentSession()->GetSendBuffer()->Write((BYTE*)&clinfo);
		}
		return;
	}
	//============================================

	SI32 id = pclMsg->usCharID;

	if(pclCM->IsValidID(id)	== false)return ;
	// PCK : account���ɾ�� GM���� 90�̻���� �����ϵ��� ����.
	if(pclCM->CR[id]->GetGameMasterLevel() < BASIC_PLAYERMASTER_LEVEL)return ;

	if(pclMsg->siResult == 0)return ;

	cltGameMsgResponse_AccountInfo clinfo( pclMsg->szAccountID, pclMsg->szPersonName1, pclMsg->szPersonName2, pclMsg->szPersonName3 );
	cltMsg clMsg(GAMEMSG_RESPONSE_ACCOUNTINFO, sizeof(clinfo), (BYTE*)&clinfo );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_ACCOUNTINFOFORHANXNOTI(sPacketHeader* pPacket)
{
#ifdef USE_HANXNOTI

	sDBResponse_AccountInfoForHanXNoti* pclMsg = (sDBResponse_AccountInfoForHanXNoti* )pPacket;
	pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [m_siArrayIndex:%d]"), TEXT("sDBResponse_AccountInfoForHanXNoti"), pclMsg->m_siArrayIndex );

	if(pclMsg->m_siArrayIndex < 0 || pclMsg->m_siArrayIndex >= HANXNOTI_MAX_WAITING)		return;

	GameCharacter*			pkChar = NULL;
	GameCharacterListAns*	pkSendMsg = NULL;

	if( m_kHanXNoti_Waitings[pclMsg->m_siArrayIndex].m_siIdx == pclMsg->m_nIdx )
	{
		m_pkHanXNoti_AnsMessages[pclMsg->m_siArrayIndex] = new GameCharacterListAns;
		pkSendMsg = (GameCharacterListAns*)m_pkHanXNoti_AnsMessages[pclMsg->m_siArrayIndex];
	}

	if(pkSendMsg)
	{
		if(pclMsg->siResult == 1)
		{
			pkSendMsg->nIdx = pclMsg->m_nIdx;

			pkChar = new GameCharacter;
			pkChar->nIdx			= 0;
			pkChar->memberid		= pclMsg->szAccountID;
			pkChar->serverid		= pclMsg->szServerID;
			pkChar->characterId		= pclMsg->szPersonName1;
			pkChar->characterType	= "";
			pkChar->level			= 0;
			pkChar->gameMoney		= 0;
			pkSendMsg->listcharacters.push_back(pkChar);

			pkChar = new GameCharacter;
			pkChar->memberid		= pclMsg->szAccountID;
			pkChar->serverid		= pclMsg->szServerID;
			pkChar->characterId		= pclMsg->szPersonName2;
			pkChar->characterType	= "";
			pkChar->level			= 0;
			pkChar->gameMoney		= 0;
			pkSendMsg->listcharacters.push_back(pkChar);

			pkChar = new GameCharacter;
			pkChar->memberid		= pclMsg->szAccountID;
			pkChar->serverid		= pclMsg->szServerID;
			pkChar->characterId		= pclMsg->szPersonName3;
			pkChar->characterType	= "";
			pkChar->level			= 0;
			pkChar->gameMoney		= 0;
			pkSendMsg->listcharacters.push_back(pkChar);

			pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [memberid:%s] [serverid:%s] [characterId1:%s] [characterId2:%s] [characterId3:%s]"), TEXT("GameCharacterListAns"), pclMsg->szAccountID, pclMsg->szServerID, pclMsg->szPersonName1, pclMsg->szPersonName2, pclMsg->szPersonName3 );
		}
	}

	// Idx �� ��ġ�� ��쿡�� 
	if( m_kHanXNoti_Waitings[pclMsg->m_siArrayIndex].m_siIdx == pclMsg->m_nIdx )
	{
		// �̺�Ʈ SET !!
		::SetEvent(m_kHanXNoti_Waitings[pclMsg->m_siArrayIndex].m_hWaitingHandle);
	}

#endif // USE_HANXNOTI
}

void cltServer::DoMsg_DBMSG_RESPONSE_CHARINFOFORHANXNOTI(sPacketHeader* pPacket)
{
#ifdef USE_HANXNOTI

	sDBResponse_CharInfoForHanXNoti* pclMsg = (sDBResponse_CharInfoForHanXNoti* )pPacket;
	pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [m_siArrayIndex:%d]"), TEXT("sDBResponse_CharInfoForHanXNoti"), pclMsg->m_siArrayIndex );

	// �̻��� �����͸� �޾Ҵ�.
	if(pclMsg->m_siArrayIndex < 0 || pclMsg->m_siArrayIndex >= HANXNOTI_MAX_WAITING)		return;

	GameCharacter*			pkChar = NULL;
	GameCharacterListAns*	pkSendMsg = NULL;

	// Idx �� ��ġ�� ��쿡�� 
	if( m_kHanXNoti_Waitings[pclMsg->m_siArrayIndex].m_siIdx == pclMsg->m_nIdx )
	{
		m_pkHanXNoti_AnsMessages[pclMsg->m_siArrayIndex] = new GameCharacterListAns;
		pkSendMsg = (GameCharacterListAns*)m_pkHanXNoti_AnsMessages[pclMsg->m_siArrayIndex];
	}

	if(pkSendMsg != NULL)
	{
		pkSendMsg->nIdx = pclMsg->m_nIdx;
		if(pclMsg->siResult == 1)
		{
			pkChar = new GameCharacter;
			pkChar->nIdx			= 0;
			pkChar->memberid		= pclMsg->m_szAccountID;
			pkChar->serverid		= pclMsg->m_szServerID;
			pkChar->characterId		= pclMsg->m_szCharName;
			pkChar->characterType	= "";
			pkChar->level			= 0;
			pkChar->gameMoney		= 0;
			pkSendMsg->listcharacters.push_back(pkChar);

			pclClient->pclLog->FilePrint(TEXT("Config\\MRSLog.log"), TEXT("Msg:[%s] [memberid:%s] [serverid:%s] [characterId:%s]"), TEXT("GameCharacterListAns"), pclMsg->m_szAccountID, pclMsg->m_szServerID, pclMsg->m_szCharName );
		}
	}

	// Idx �� ��ġ�� ��쿡�� 
	if( m_kHanXNoti_Waitings[pclMsg->m_siArrayIndex].m_siIdx == pclMsg->m_nIdx )
	{
		// �̺�Ʈ SET !!
		::SetEvent(m_kHanXNoti_Waitings[pclMsg->m_siArrayIndex].m_hWaitingHandle);



	}

#endif // USE_HANXNOTI
}

void cltServer::DoMsg_DBMSG_RESPONSE_PRIVATETRADE(sPacketHeader* pPacket)
{
	sDBResponse_PrivateTrade *pclMsg = (sDBResponse_PrivateTrade *)pPacket;

	cltRentContract clPerson1RecvHouseUnitContract;
	clPerson1RecvHouseUnitContract.Init();

	cltRentContract clPerson2RecvHouseUnitContract;
	clPerson2RecvHouseUnitContract.Init();


	if ( pclMsg->siResult != 1 )
	{
		// -2 �� ��쿡�� ���� ���� �ʰ��ؼ� �������� �Ҷ�
		if ( pclMsg->siResult == -2 )
		{
			if(pclCM->IsValidID(pclMsg->usCharID1) )
			{
				if ( pclCM->CR[ pclMsg->usCharID1 ]->pclCI->GetPersonID() == pclMsg->siPersonID1 )
				{
					cltHorse* pclHorse1 = pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.GetCurrentShowHorse();
					cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd( pclMsg->siResult, 0, 0, 0, 0, 0, 0, &clPerson1RecvHouseUnitContract, 0, 0, -1, pclHorse1);
					cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd ),(BYTE*)&clPrivateTradeEnd );
					
					pclCM->CR[pclMsg->usCharID1]->SendNetMsg((sPacketHeader*)&clMsg);

					SendServerResponseMsg(0, SRVAL_PRIVATETRADE_HOUSEUNIT_EXCEED_NUMBER_PER_PERSON, 0, 0, pclMsg->usCharID1 );
				}
			}
			if(pclCM->IsValidID(pclMsg->usCharID2)	)
			{
				if ( pclCM->CR[ pclMsg->usCharID2 ]->pclCI->GetPersonID() == pclMsg->siPersonID2 )
				{
					cltHorse* pclHorse2 = pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clHorseInfo.GetCurrentShowHorse();
					cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd( pclMsg->siResult, 0, 0, 0, 0, 0, 0, &clPerson2RecvHouseUnitContract, 0, 0, -1, pclHorse2 );
					cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd ),(BYTE*)&clPrivateTradeEnd );
					
					pclCM->CR[pclMsg->usCharID2]->SendNetMsg((sPacketHeader*)&clMsg);

					SendServerResponseMsg(0, SRVAL_PRIVATETRADE_HOUSEUNIT_EXCEED_NUMBER_PER_PERSON, 0, 0, pclMsg->usCharID2 );
				}
			}
			
		}
		else
		{
			if(pclCM->IsValidID(pclMsg->usCharID1) )
			{
				if ( pclCM->CR[ pclMsg->usCharID1 ]->pclCI->GetPersonID() == pclMsg->siPersonID1 )
				{
					cltHorse* pclHorse1 = pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.GetCurrentShowHorse();
					cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd( pclMsg->siResult, 0, 0, 0, 0, 0, 0, &clPerson1RecvHouseUnitContract, 0, 0, -1, pclHorse1 );
					cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd ),(BYTE*)&clPrivateTradeEnd );
					
					pclCM->CR[pclMsg->usCharID1]->SendNetMsg((sPacketHeader*)&clMsg);

				}
			}
			
			if(pclCM->IsValidID(pclMsg->usCharID2)	)
			{
				if ( pclCM->CR[ pclMsg->usCharID2 ]->pclCI->GetPersonID() == pclMsg->siPersonID2 )
				{
					cltHorse* pclHorse2 = pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clHorseInfo.GetCurrentShowHorse();
					cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd( pclMsg->siResult, 0, 0, 0, 0, 0, 0, &clPerson2RecvHouseUnitContract, 0, 0, -1, pclHorse2 );
					cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd ),(BYTE*)&clPrivateTradeEnd );
					
					pclCM->CR[pclMsg->usCharID2]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}

		pclPrivateTradeOrderMgr->Cancel( pclMsg->siPersonID1 );
		pclPrivateTradeOrderMgr->Cancel( pclMsg->siPersonID2 );
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// ���� �ŷ�( ������ ���� ���� �ʴ��� �ŷ��� �Ǿ�� ������. )	
	if ( pclMsg->siSendHouseVillageUnique1 > 0 && pclMsg->siSendHouseVillageUnique1 <= MAX_VILLAGE_NUMBER )
	{
		cltHouse* pclhouse = (cltHouse*)pclHouseManager->pclStruct[ pclMsg->siSendHouseVillageUnique1 ];

		if( pclhouse != NULL )
		{
			if( pclMsg->siSendHouseUnitSlot1 >= 0 && pclMsg->siSendHouseUnitSlot1 < MAX_HOUSEUNIT_NUMBER )
			{
				pclhouse->clUnit[ pclMsg->siSendHouseUnitSlot1 ].clPerson.Set( pclMsg->siPersonID2, pclMsg->Person2Name );			
				clPerson2RecvHouseUnitContract.Set( &pclhouse->clUnit[ pclMsg->siSendHouseUnitSlot1 ].clContract );
			}
		}
	}
	
	if ( pclMsg->siSendHouseVillageUnique2 > 0 )
	{	
		cltStrInfo *pclStrInfo = pclHouseManager->GetStrInfo( pclMsg->siSendHouseVillageUnique2 );

		cltHouse* pclhouse = (cltHouse*)pclHouseManager->pclStruct[ pclMsg->siSendHouseVillageUnique2 ];

		if( pclhouse != NULL )
		{
			if( pclMsg->siSendHouseUnitSlot2 >= 0 && pclMsg->siSendHouseUnitSlot2 < MAX_HOUSEUNIT_NUMBER )
			{
				pclhouse->clUnit[ pclMsg->siSendHouseUnitSlot2 ].clPerson.Set( pclMsg->siPersonID1, pclMsg->Person1Name );
				clPerson1RecvHouseUnitContract.Set( &pclhouse->clUnit[ pclMsg->siSendHouseUnitSlot2 ].clContract );
			}
			//pclhouse->clUnit[ pclMsg->siSendHouseUnitSlot2 ].clContract;
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	
	
	if(pclCM->IsValidID(pclMsg->usCharID1) )
	{
		if ( pclCM->CR[ pclMsg->usCharID1 ]->pclCI->GetPersonID() == pclMsg->siPersonID1 )
		{
			// �����̵� ���е� ������� ������ �ŷ�â�� �ݰ� �Ѵ�.
			// ����, �ŷ� �Ҵ�� �͵� ���� �ؾ� �ȴ�.

			//KHY - 1001 - ��µ��� 5���� ���� ����. - �ڽ��� ���� �Ⱥ��̴� �κ�.
			SI16 siHorseIndex1 = pclMsg->siHorse1;
			cltHorse clHorse1;

			clHorse1.Set(&pclMsg->clRecvHorse1 );
			
			if(pclMsg->siHorse1 == -1)
			{
				siHorseIndex1 = pclCM->CR[pclMsg->usCharID1]->pclCI->clHorseInfo.GetCurrentShowHorseIndex();
				clHorse1.Set(pclCM->CR[pclMsg->usCharID1]->pclCI->clHorseInfo.GetCurrentShowHorse());
			}
			cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd( pclMsg->siResult,
				pclMsg->siRecvStockVillageUnique1, pclMsg->siRecvStockNum1, 
				pclMsg->siSendStockVillageUnique1, pclMsg->siSendStockNum1,
				pclMsg->siRecvHouseVillageUnique1, pclMsg->siRecvHouseUnitSlot1,
				&clPerson1RecvHouseUnitContract,
				pclMsg->siSendHouseVillageUnique1, pclMsg->siSendHouseUnitSlot1,
				siHorseIndex1, &clHorse1 );
			cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd ),(BYTE*)&clPrivateTradeEnd );

			pclCM->CR[pclMsg->usCharID1]->SendNetMsg((sPacketHeader*)&clMsg);

//			pclMsg->siHorse1, &pclMsg->clRecvHorse1 );

			//////////////////////////////////////////////////////////////////////////
			// �ֽ� �ŷ�
			if ( pclMsg->siRecvStockNum1 > 0 && pclMsg->siRecvStockVillageUnique1 > 0 && pclMsg->siRecvStockVillageUnique1 < MAX_VILLAGE_NUMBER )
			{
				SI16 EmptyIndex = -1;
				for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
				{
					if ( pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique == pclMsg->siRecvStockVillageUnique1 )
					{
						pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ i ].siAmount = pclMsg->siRecvStockNum1;
						break;
					}
					else if ( pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique == 0 && EmptyIndex == -1 )
					{
						EmptyIndex = i;
					}
				}

				if ( i == MAX_STOCK_KIND_IN_ACOUNT && EmptyIndex > -1 && EmptyIndex < MAX_STOCK_KIND_IN_ACOUNT )
				{
					pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ EmptyIndex ].siAmount = pclMsg->siRecvStockNum1;
					pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ EmptyIndex ].siVillageUnique = pclMsg->siRecvStockVillageUnique1;
				}
			}			
			
			if ( pclMsg->siSendStockVillageUnique1 > 0 && pclMsg->siSendStockVillageUnique1 < MAX_VILLAGE_NUMBER )
			{
				for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
				{
					if ( pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique == pclMsg->siSendStockVillageUnique1 )
					{
						if ( pclMsg->siSendStockNum1 > 0 )
						{
							pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ i ].siAmount = pclMsg->siSendStockNum1;
						}
						else
						{
							pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ i ].siAmount = 0;
							pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique = 0;
						}
						break;		
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			// ���� �ŷ�
			if ( pclMsg->siSendHouseVillageUnique1 > 0 && pclMsg->siSendHouseVillageUnique1 < MAX_VILLAGE_NUMBER )
			{
				pclCM->CR[pclMsg->usCharID1]->pclCI->clRealEstate.Delete( REALESTATE_TYPE_HOUSE, pclMsg->siSendHouseVillageUnique1, pclMsg->siSendHouseUnitSlot1 );
			}

			if ( pclMsg->siRecvHouseVillageUnique1 > 0 && pclMsg->siRecvHouseVillageUnique1 < MAX_VILLAGE_NUMBER )
			{
				cltHouse* pclhouse = (cltHouse*)pclHouseManager->pclStruct[ pclMsg->siRecvHouseVillageUnique1 ];

				if( pclhouse != NULL )
				{	
					if( pclMsg->siRecvHouseUnitSlot1 >= 0 && pclMsg->siRecvHouseUnitSlot1 < MAX_HOUSEUNIT_NUMBER )
						pclCM->CR[pclMsg->usCharID1]->pclCI->clRealEstate.Add( REALESTATE_TYPE_HOUSE, pclMsg->siRecvHouseVillageUnique1, pclMsg->siRecvHouseUnitSlot1, &pclhouse->clUnit[ pclMsg->siRecvHouseUnitSlot1 ].clContract );
				}
			}
			//////////////////////////////////////////////////////////////////////////
			

			//////////////////////////////////////////////////////////////////////////
			// �� �ŷ� ( �� ������ ���� �Ŀ� �ŷ� )
			if ((pclMsg->siHorse1 != -1) && (pclMsg->siRecvHorse1 != -1)) //���� ���� �ְ� �޾Ҵ�.
			{
				pclCM->CR[pclMsg->usCharID1]->UnrideHorse();
				//pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.clHorse.Set( &pclMsg->clRecvHorse1 );
				pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.Init(pclMsg->siHorse1);
				pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.SetHorse( pclMsg->siRecvHorse1, &pclMsg->clRecvHorse1 );
				pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.SetCurrentShowHorseIndex(pclMsg->siRecvHorse1);
				
				//---------------------------------------
				// �̿��鿡�� ������ ������ �˸� ( �� ��ȯ, �Ǵ� ���� )
				//---------------------------------------
				((cltCharServer*)pclCM->CR[pclMsg->usCharID1])->SendNeighbourMyHorseStatus(pclCM->CR[pclMsg->usCharID1]->pclCI->clHorseInfo.siCurrentShowHorse);

			}
			else if ( pclMsg->siHorse1 != -1 ) 
			{
				pclCM->CR[pclMsg->usCharID1]->UnrideHorse();
				pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.SetHorse( pclMsg->siHorse1, &pclMsg->clRecvHorse1 );

				//---------------------------------------
				// �̿��鿡�� ������ ������ �˸� ( �� ��ȯ, �Ǵ� ���� )
				//---------------------------------------
				((cltCharServer*)pclCM->CR[pclMsg->usCharID1])->SendNeighbourMyHorseStatus(pclCM->CR[pclMsg->usCharID1]->pclCI->clHorseInfo.siCurrentShowHorse);

			}
			else if ( pclMsg->siRecvHorse1 != -1 )
			{
				pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.SetHorse( pclMsg->siRecvHorse1, &pclMsg->clRecvHorse1 );

				//---------------------------------------
				// �̿��鿡�� ������ ������ �˸� ( �� ���� )
				//---------------------------------------
				{
					((cltCharServer*)pclCM->CR[pclMsg->usCharID1])->SendNeighbourMyHorseStatus( pclCM->CR[pclMsg->usCharID1]->pclCI->clHorseInfo.siCurrentShowHorse);
				}
			}
			//////////////////////////////////////////////////////////////////////////

			//���ΰŷ� ����Ʈ�� ���������� Ȯ���Ѵ�. 
			((cltCharServer*)pclCM->CR[pclMsg->usCharID1])->DoQuestCheckAndAction(QUEST_CLEAR_COND_PRIVATETRADE);
		}
	}
	
	if(pclCM->IsValidID(pclMsg->usCharID2)	)
	{
		if ( pclCM->CR[ pclMsg->usCharID2 ]->pclCI->GetPersonID() == pclMsg->siPersonID2 )
		{
			// �����̵� ���е� ������� ������ �ŷ�â�� �ݰ� �Ѵ�.
			// ����, �ŷ� �Ҵ�� �͵� ���� �ؾ� �ȴ�.

			//KHY - 1001 - ��µ��� 5���� ���� ����. - �ڽ��� ���� �Ⱥ��̴� �κ�.
			SI16 siHorseIndex2 = pclMsg->siHorse2;
			cltHorse clHorse2;

			clHorse2.Set(&pclMsg->clRecvHorse2 );
			if(pclMsg->siHorse2 == -1)
			{
				siHorseIndex2 = pclCM->CR[pclMsg->usCharID2]->pclCI->clHorseInfo.GetCurrentShowHorseIndex();
				clHorse2.Set(pclCM->CR[pclMsg->usCharID2]->pclCI->clHorseInfo.GetCurrentShowHorse());
			}
			cltGameMsgResponse_PrivateTrade_End clPrivateTradeEnd( pclMsg->siResult,
				pclMsg->siRecvStockVillageUnique2, pclMsg->siRecvStockNum2,
				pclMsg->siSendStockVillageUnique2, pclMsg->siSendStockNum2,
				pclMsg->siRecvHouseVillageUnique2, pclMsg->siRecvHouseUnitSlot2,
				&clPerson2RecvHouseUnitContract,
				pclMsg->siSendHouseVillageUnique2, pclMsg->siSendHouseUnitSlot2,
				siHorseIndex2, &clHorse2
				);
			cltMsg clMsg( GAMEMSG_RESPONSE_PRIVATETRADE_END, sizeof( clPrivateTradeEnd ),(BYTE*)&clPrivateTradeEnd );

	//			pclMsg->siHorse2, &pclMsg->clRecvHorse2

			pclCM->CR[pclMsg->usCharID2]->SendNetMsg((sPacketHeader*)&clMsg);

			//////////////////////////////////////////////////////////////////////////
			// �ֽ� �ŷ�
			if ( pclMsg->siRecvStockNum2 > 0 && pclMsg->siRecvStockVillageUnique2 > 0 && pclMsg->siRecvStockVillageUnique2 < MAX_VILLAGE_NUMBER )
			{
				SI16 EmptyIndex = -1;
				for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
				{
					if ( pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique == pclMsg->siRecvStockVillageUnique2 )
					{
						pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ i ].siAmount = pclMsg->siRecvStockNum2;
						break;
					}
					else if ( pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique == 0 && EmptyIndex == -1 )
					{
						EmptyIndex = i;
					}
				}

				if ( i == MAX_STOCK_KIND_IN_ACOUNT && EmptyIndex > -1 && EmptyIndex < MAX_STOCK_KIND_IN_ACOUNT )
				{
					pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ EmptyIndex ].siAmount = pclMsg->siRecvStockNum2;
					pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ EmptyIndex ].siVillageUnique = pclMsg->siRecvStockVillageUnique2;
				}
			}

			if ( pclMsg->siSendStockVillageUnique2 > 0 && pclMsg->siSendStockVillageUnique2 < MAX_VILLAGE_NUMBER )
			{
				for ( SI16 i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
				{
					if ( pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique == pclMsg->siSendStockVillageUnique2 )
					{
						if ( pclMsg->siSendStockNum2 > 0 )
						{
							pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ i ].siAmount = pclMsg->siSendStockNum2;
						}
						else
						{
							pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ i ].siAmount = 0;
							pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique = 0;
						}
						break;
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////


			//////////////////////////////////////////////////////////////////////////
			// ���� �ŷ�
			if ( pclMsg->siSendHouseVillageUnique2 > 0 && pclMsg->siSendHouseVillageUnique2 < MAX_VILLAGE_NUMBER )
			{
				pclCM->CR[pclMsg->usCharID2]->pclCI->clRealEstate.Delete( REALESTATE_TYPE_HOUSE, pclMsg->siSendHouseVillageUnique2, pclMsg->siSendHouseUnitSlot2 );
			}

			if ( pclMsg->siRecvHouseVillageUnique2 > 0 && pclMsg->siRecvHouseVillageUnique2 < MAX_VILLAGE_NUMBER )
			{
				cltHouse* pclhouse = (cltHouse*)pclHouseManager->pclStruct[ pclMsg->siRecvHouseVillageUnique2 ];

				if( pclhouse != NULL )
				{
					pclCM->CR[pclMsg->usCharID2]->pclCI->clRealEstate.Add( REALESTATE_TYPE_HOUSE, pclMsg->siRecvHouseVillageUnique2, pclMsg->siRecvHouseUnitSlot2, &pclhouse->clUnit[ pclMsg->siRecvHouseVillageUnique2 ].clContract );
				}
			}
			//////////////////////////////////////////////////////////////////////////
			

			//////////////////////////////////////////////////////////////////////////
			// �� �ŷ� ( �� ������ ���� �Ŀ� �ŷ� )
			if ((pclMsg->siHorse2 != -1) && (pclMsg->siRecvHorse2 != -1)) //���� ���� �ְ� �޾Ҵ�.
			{
				pclCM->CR[pclMsg->usCharID2]->UnrideHorse();
				//pclCM->CR[ pclMsg->usCharID1 ]->pclCI->clHorseInfo.clHorse.Set( &pclMsg->clRecvHorse1 );
				pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clHorseInfo.Init(pclMsg->siHorse2);
				pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clHorseInfo.SetHorse( pclMsg->siRecvHorse2, &pclMsg->clRecvHorse2 );
				pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clHorseInfo.SetCurrentShowHorseIndex(pclMsg->siRecvHorse2);

				//---------------------------------------
				// �̿��鿡�� ������ ������ �˸� ( �� ��ȯ, �Ǵ� ���� )
				//---------------------------------------
				((cltCharServer*)pclCM->CR[pclMsg->usCharID2])->SendNeighbourMyHorseStatus(pclCM->CR[pclMsg->usCharID2]->pclCI->clHorseInfo.siCurrentShowHorse);

			}
			else if ( pclMsg->siHorse2 != -1 )
			{
				pclCM->CR[ pclMsg->usCharID2 ]->UnrideHorse();
				pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clHorseInfo.SetHorse( pclMsg->siHorse2, &pclMsg->clRecvHorse2 );

				//---------------------------------------
				// �̿��鿡�� ������ ������ �˸� ( �� ��ȯ �Ǵ� ���� )
				//---------------------------------------
				{
					((cltCharServer*)pclCM->CR[pclMsg->usCharID2])->SendNeighbourMyHorseStatus( pclCM->CR[pclMsg->usCharID2]->pclCI->clHorseInfo.siCurrentShowHorse);
				}
			}
			else if ( pclMsg->siRecvHorse2 != -1 )
			{
				pclCM->CR[ pclMsg->usCharID2 ]->pclCI->clHorseInfo.SetHorse( pclMsg->siRecvHorse2, &pclMsg->clRecvHorse2 );

				//---------------------------------------
				// �̿��鿡�� ������ ������ �˸� ( �� ���� )
				//---------------------------------------
				{
					((cltCharServer*)pclCM->CR[pclMsg->usCharID2])->SendNeighbourMyHorseStatus( pclCM->CR[pclMsg->usCharID2]->pclCI->clHorseInfo.siCurrentShowHorse);
				}
			}
			//////////////////////////////////////////////////////////////////////////


			//���ΰŷ� ����Ʈ�� ���������� Ȯ���Ѵ�. 
			((cltCharServer*)pclCM->CR[pclMsg->usCharID2])->DoQuestCheckAndAction(QUEST_CLEAR_COND_PRIVATETRADE);

		}
	}

	pclPrivateTradeOrderMgr->Cancel( pclMsg->siPersonID1 );
	pclPrivateTradeOrderMgr->Cancel( pclMsg->siPersonID2 );

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		if( pclCM->IsValidID(pclMsg->usCharID1) && pclCM->IsValidID(pclMsg->usCharID2) )
		{
			SI32 i;
			// A ����ڿ��� B ����ڷ� �Űܰ� ������ �α�
			for( i=0; i<MAX_PRIVATETRADEORDER_ITEM_NUM; i++ )
			{
				if( pclMsg->LOG_clItem1[i].siUnique > 0 )
				{
					// ������ �ŷ� �α�
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_PRIVATE_TRADE, 
						0, (cltCharServer*)pclCM->GetChar(pclMsg->usCharID1), NULL, pclMsg->siPersonID2, 0, 0, &pclMsg->LOG_clItem1[i], 0, 0, 0, 0, 0, 0, 
						pclMsg->LOG_siItemOriPos1[i], pclMsg->LOG_siItemPos1[i], 0, 0, 0, NULL, NULL);
				}
			}

			// B ����ڿ��� A ����ڷ� �Űܰ� ������ �α�
			for( i=0; i<MAX_PRIVATETRADEORDER_ITEM_NUM; i++ )
			{
				if( pclMsg->LOG_clItem2[i].siUnique > 0 )
				{
					// ������ �ŷ� �α�
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_PRIVATE_TRADE, 
						0, (cltCharServer*)pclCM->GetChar(pclMsg->usCharID2), NULL, pclMsg->siPersonID1, 0, 0, &pclMsg->LOG_clItem2[i], 0, 0, 0, 0, 0, 0, 
						pclMsg->LOG_siItemOriPos2[i], pclMsg->LOG_siItemPos2[i], 0, 0, 0, NULL, NULL);
				}
			}
		}

		if(pclMsg->siRecvStockNum1 > 0)
		{
			// [�α��߰� : �ڱ��� 2010. 01. 20] // �ֽ� ���ΰŷ� �α׾���.
			// param1		 == ���� �ֽļ� + ���� �ֽļ�
			// param2		 == ���� �ֽļ�
			// param3		 == �ֽ��� �� ����� ������ ������ �ֽ� ��
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_PRIVATETRADE, 
				0, pclCM->GetCharServer(pclMsg->usCharID1), NULL, pclMsg->siPersonID2, 0, 0, 0, 0, 0, 0, pclMsg->siRecvStockVillageUnique1, 0, 0, 
				pclMsg->siRecvStockNum1, pclMsg->siRecvedStockNumToSender1 , pclMsg->siSendStockNum2, 0, 0, 0, 0);		
		}

		if(pclMsg->siRecvStockNum2 > 0)
		{
			// [�α��߰� : �ڱ��� 2010. 01. 20] // �ֽ� ���ΰŷ� �α׾���.
			// param1		 == ���� �ֽļ� + ���� �ֽļ�
			// param2		 == ���� �ֽļ�
			// param3		 == �ֽ��� �� ����� ������ ������ �ֽ� ��
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_STOCK, LOGCOMMAND_INDEX_STOCK_PRIVATETRADE, 
				0, pclCM->GetCharServer(pclMsg->usCharID2), NULL, pclMsg->siPersonID1, 0, 0, 0, 0, 0, 0, pclMsg->siRecvStockVillageUnique2, 0, 0, 
				pclMsg->siRecvStockNum2, pclMsg->siRecvedStockNumToSender2, pclMsg->siSendStockNum1, 0, 0, 0, 0);		
		}

		if(pclMsg->siRecvHorse1 > -1)
		{
			TCHAR szHorseAbility[50] = {'\0', };
			StringCchPrintf(szHorseAbility, 50, "Str: %d, Dex: %d, Mag: %d, Speed: %d",
				pclMsg->clRecvHorse1.siStr,
				pclMsg->clRecvHorse1.siDex,
				pclMsg->clRecvHorse1.siMag,
				pclMsg->clRecvHorse1.siMoveSpeed );

			// [�α��߰� : �ڱ��� 2009. 07. 14] // Ż�� ���ΰŷ� �α׾���.
			// param1		 == �ִ»�� �� ���Թ�ȣ
			// param2		 == �޴»�� �� ���Թ�ȣ.
			// pszCharParam1 == �� �̸�.
			// pszCharParam2 == �� ����.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HORSESET, LOGCOMMAND_INDEX_HORSESET_PRIVATETRADE, 
				0, (cltCharServer*)pclCM->GetChar(pclMsg->usCharID1), NULL, pclMsg->siPersonID2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
				pclMsg->siHorse2, pclMsg->siRecvHorse1, 0, 0, 0, pclMsg->clRecvHorse1.szName, szHorseAbility);		
		}

		if(pclMsg->siRecvHorse2 > -1)
		{
			TCHAR szHorseAbility[50] = {'\0', };
			StringCchPrintf(szHorseAbility, 50, "Str: %d, Dex: %d, Mag: %d, Speed: %d",
				pclMsg->clRecvHorse2.siStr,
				pclMsg->clRecvHorse2.siDex,
				pclMsg->clRecvHorse2.siMag,
				pclMsg->clRecvHorse2.siMoveSpeed );

			// [�α��߰� : �ڱ��� 2009. 07. 14] // Ż�� ���ΰŷ� �α׾���.
			// param1		 == �ִ»�� �� ���Թ�ȣ
			// param2		 == �޴»�� �� ���Թ�ȣ.
			// pszCharParam1 == �� �̸�.
			// pszCharParam2 == �� ����.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HORSESET, LOGCOMMAND_INDEX_HORSESET_PRIVATETRADE, 
				0, (cltCharServer*)pclCM->GetChar(pclMsg->usCharID2), NULL, pclMsg->siPersonID1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
				pclMsg->siHorse1, pclMsg->siRecvHorse2, 0, 0, 0, pclMsg->clRecvHorse2.szName, szHorseAbility);		
		}
	}
	
	// �������� �ѹ� ������Ʈ���ش�. �� �ŷ�����
	{
		sDBRequest_PersonItemInfoGet clMsg1(pclMsg->usCharID1, pclMsg->siPersonID1);
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg1);

		sDBRequest_PersonItemInfoGet clMsg2(pclMsg->usCharID2, pclMsg->siPersonID2);
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg2);
	}


/*	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [�α��߰� : Ȳ���� 2007. 10. 11] // ���� �ŷ�. (�����ʿ� : ������ �ʹ� ����.)
		// 
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_PRIVATE_TRADE, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										0, 0, 0, 0, 0, NULL, NULL);
	}*/
	return;
}

// ����� �α��� ���/���� ����. 
void cltServer::DoMsg_DBMSG_RESPONSE_BLOCK(sPacketHeader* pPacket)
{
	sDBResponse_Block* pclMsg = (sDBResponse_Block* )pPacket;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id)	== false)return ;
	if(pclCM->CR[id]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL)return ;

	SI32 returnval = 0;
	if(pclMsg->siBlockMode == 1)
	{
		returnval = SRVAL_BLOCK_UNBLOCKED;
	}
	else
	{
		returnval = SRVAL_BLOCK_BLOCKED;
	}

	// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
	SendServerResponseMsg(0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

}

// Ư�� ����� �α��� ���/���� ����. 
void cltServer::DoMsg_DBMSG_RESPONSE_BLOCKUSER(sPacketHeader* pPacket)
{
	sDBResponse_BlockUser* pclMsg = (sDBResponse_BlockUser* )pPacket;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsValidID(id)	== false)return ;
	if(pclCM->CR[id]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL)return ;

	SI32 returnval = 0;

	switch(pclMsg->siWarnMode)
	{
	case sDBResponse_BlockUser::WARNING_EMPTY:
		{
			if(pclMsg->siBlockMode == 0)
			{
				returnval = SRVAL_BLOCKUSER_UNBLOCKED;

			}
			else
			{
				returnval = SRVAL_BLOCKUSER_BLOCKED;

				//-------------------------------------------
				// �ش� ����ڸ� �����Ų��. 
				//-------------------------------------------
				pclCM->OutByPersonID(pclMsg->siPersonID);

			}

			//// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
			//SendServerResponseMsg(0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
		}
		break;
	case sDBResponse_BlockUser::WARNING_FIRST:
		{
			TCHAR buffer[512];
			TCHAR* pText = GetTxtFromMgr(5231);
			StringCchPrintf( buffer, 512, pText, pclMsg->szBlockReason );
			SendPostOfficeItemToPerson( pclMsg->siPersonID, NULL, buffer );

			return;	// ��� �޼����� �������Ƿ�
		}
		break;
	case sDBResponse_BlockUser::BLOCK_FOR_5_DAYS:	// 5�ϰ� ����
	case sDBResponse_BlockUser::BLOCK_FOR_10_DAYS:	// 10�ϰ� ����
	case sDBResponse_BlockUser::BLOCK_FOR_FOREVER:	// ������ ����
		{
			returnval = SRVAL_BLOCKUSER_BLOCKED;

			//-------------------------------------------
			// �ش� ����ڸ� �����Ų��. 
			//-------------------------------------------
			pclCM->OutByPersonID(pclMsg->siPersonID);
		}
		break;
	}

	// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
	SendServerResponseMsg(0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 9] // Ư�� ������� ������ ����/���.
		// pszParam1 == ��������.
		// PCK - Reason �߰�.
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_BLOCK_USER, 
										pclMsg->siWarnMode, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										0, 0, 0, 0, 0, pclMsg->szBlockReason, NULL);
	}
}


// �ð��� �����϶��  ��û�� ���� ����
void cltServer::DoMsg_DBMSG_RESPONSE_SETTIME(sPacketHeader* pPacket)
{
	sDBResponse_SetTime* pclMsg = (sDBResponse_SetTime*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// DB���� �ð��� ���Ѵ�. 
	dwDBLatency = TABS(CurrentClock - pclMsg->dwSetTimeClock);
}


// �ð��� ������� ��û�� ���� ����
void cltServer::DoMsg_DBMSG_RESPONSE_GETTIME(sPacketHeader* pPacket)
{
	sDBResponse_GetTime* pclMsg = (sDBResponse_GetTime*)pPacket;

	TraceBoot("DoMsg_DBMSG_RESPONSE_GETTIME enter siResult=%d", pclMsg->siResult);
	if(pclMsg->siResult == 0)
	{
		return ;
	}

	// [FIX-race-crash] Crash armor: if a GETTIME reply ever dispatches before cltServer::Create
	// allocated pclTime, re-arm the one-shot request switch so the next ServerAction tick
	// re-requests (no crash, no boot deadlock).
	if(pclTime == NULL)
	{
		bGameTimeRequestSwitch = false;
		TraceBoot("DoMsg_DBMSG_RESPONSE_GETTIME pclTime==NULL -> re-arm request (skip time set)");
		return;
	}

	// ������ �ð��� �����Ѵ�. 
	pclTime->clDate.Set(pclMsg->siYear, pclMsg->siMonth, pclMsg->siDay, pclMsg->siHour, pclMsg->siMinute);
	
	// �ð��� �������� Ȯ���Ѵ�. 
	bGetTimeSwitch	= true;
	TraceBoot("DoMsg_DBMSG_RESPONSE_GETTIME -> bGetTimeSwitch=TRUE (db time acquired)");

}


// ��Ÿ�� �뺸. 
void cltServer::DoMsg_DBMSG_RESPONSE_MAPTYPEINFO(sPacketHeader* pPacket)
{

	sDBResponse_MapTypeInfo* pclMsg = (sDBResponse_MapTypeInfo*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}


	for(SI32 i = 0;i < MAX_MAP_NUMBER;i++)
	{
		if(pclMsg->siMapTypeInfo[i] <= 0)continue;

		if(pclMapManager->pclMapArray[i] == NULL)continue;

		// ����Ʈ ������  �ٸ� ������ũ�� ���� �ִٸ�, 
		SI32 mapunique = pclMsg->siMapTypeInfo[i];
		if(pclMapManager->pclMapArray[i]->siMapUnique != mapunique)
		{
			// ���� ������ �����Ѵ�. 
			delete pclMapManager->pclMapArray[i];
			pclMapManager->pclMapArray[i] = NULL;

			pclMapManager->LoadMap(this, i, mapunique);
		}

	}

	
	bGetMapTypeInfoSwitch = true;
}


// ���� �̵� �㰡 �뺸 
void cltServer::DoMsg_DBMSG_RESPONSE_WARPVILLAGE(sPacketHeader* pPacket)
{

	sDBResponse_WarpVillage* pclMsg = (sDBResponse_WarpVillage*)pPacket;

	// �ش� Item�� ����.
	if(pclMsg->siResult == 0)	return ;
	if(pclMsg->siFromVillageUnique < 0 || pclMsg->siFromVillageUnique >= MAX_MAP_NUMBER )		return;
	if( pclMapManager->pclMapArray[pclMsg->siFromVillageUnique] == NULL ) return ;
	if(pclMsg->siToVillageUnique < 0   || pclMsg->siToVillageUnique   >= MAX_VILLAGE_NUMBER)	return;
	if(pclMapManager->pclMapArray[pclMsg->siToVillageUnique] == NULL)return ;

	if(pclMsg->siItemPos < 0 || pclMsg->siItemPos > MAX_ITEM_PER_PERSON )						return;

	SI32 id = pclMsg->usCharID;
	if(pclCM->IsAlive(id) == false)return ;
	
	// Client�� ������ �κ��丮 Update�Ѵ�.
	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);
		
		// Ŭ���̾�Ʈ�� ������Ʈ �� ������ ������. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	// ������ 2000���� �߰��Ѵ�.
	if(pclMsg->siFromVillageUnique > 0	&& pclMsg->siFromVillageUnique < MAX_VILLAGE_NUMBER)
	{
		//sDBRequest_ChangeStrMoney sendMsg(CHANGESTRMONEY_SELLNPC, 0, pclMsg->siFromVillageUnique, RANKTYPE_CITYHALL, VILLAGE_PROFIT_FOR_FANPRIEST);
		sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, pclMsg->siFromVillageUnique, VILLAGE_PROFIT_FOR_FANPRIEST );
		pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
	}
	// ����ڸ� Warp��Ų��.
	{
		SI32 ref = pclMsg->siToVillageUnique;
		SI32 mapindex		= pclVillageManager->pclVillageInfo[ref]->siVillageUnique;
		SI32 maptypeunique	= -1;
		if( mapindex >= 0 && mapindex < MAX_MAP_NUMBER )	maptypeunique = pclMapManager->pclMapArray[mapindex]->siMapUnique;
		// WARP�� �Ѵ�.
		SI32 x	= -1;
		SI32 y	= -1;

		if( maptypeunique >= 0 && maptypeunique < MAX_MAPTYPE_NUMBER )
		{
			x = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
			y = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;
		}
		
		if(id !=0 && pclCM->IsValidID(id)== FALSE)	return;
		if(x > 0 && y > 0)
		{
			id = cltCharServer::CharUniqueInfo[id];
			OrderStop(id, BY_COMPUTER);

			cltWarpInfo clWarpInfo(1, mapindex, false, false, x, y, -1, -1);
			pclCM->CR[pclMsg->usCharID]->SetWarpInfo(&clWarpInfo, 2000);
		}
	}

	
	/*if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 11] // ����ڸ� Warp��Ų��.
		// param1 == �̵����� ����ũ. 
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_WARP_VILLAGE, 
										0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clItem, pclMsg->clItem.siItemNum, 0, 0, pclMsg->siFromVillageUnique, 0, 0, 
										pclMsg->siToVillageUnique, 0, 0, 0, 0, NULL, NULL);
	}*/
}

// ���ӽ��ۿ� ���� ������ �����ð��� ���´�.
void cltServer::DoMsg_DBMSG_RESPONSE_STARTGAME(sPacketHeader* pPacket)
{

	sDBResponse_StartGame* pclMsg = (sDBResponse_StartGame*)pPacket;

	if(pclMsg->siResult == 0)	return ;

}

void cltServer::DoMsg_DBMSG_RESPONSE_MASTERGIVEITEM(sPacketHeader* pPacket)
{
	sDBResponse_MasterGiveItem* pclMsg = (sDBResponse_MasterGiveItem *)pPacket;

	if( pclMsg->usCount > 1000 )		pclMsg->usCount = 1000;

	for( int i = 0; i < pclMsg->usCount; ++i ) {
		SendPostOfficeItemToPerson( pclMsg->siPersonID[ i ], &pclMsg->clItem, pclMsg->szMsg );
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GIVEITEM(sPacketHeader* pPacket)
{
	// PCK : �α׿����� �߰���.
	sDBResponse_GiveItem*	pclMsg = (sDBResponse_GiveItem*)pPacket;

	SI32 id = pclMsg->usCharID;
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	// [�α��߰� : Ȳ���� 2007. 10. 18] // �ý��ۿ��� ������ �ֱ�.
	// param1 == �ּҷ���, 
	// param2 == �ִ뷹��,
	// param3 == ������ �Ⱓ,
	// pszParam1 == �޽���
	cltServer * pclServer = (cltServer*)pclClient;
	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SYSTEM, LOGCOMMAND_INDEX_SYSTEM_GIVE_ITEM, 
		0, pclchar, NULL, 0, 0, 0, &pclMsg->clItem, 0, 0, 0, 0, 0, 0, 
		pclMsg->minLevelRange, pclMsg->maxLevelRange, pclMsg->clDate.GetDateVary(), 0, 0, pclMsg->szMsg, NULL);
}

void cltServer::DoMsg_DBMSG_RESPONSE_DBSYSTEMINFO(sPacketHeader* pPacket)
{
	sDBResponse_DBSystemInfo *pclMsg = (sDBResponse_DBSystemInfo *)pPacket;

	if( pclMsg->siResult != 1 )						return;
	
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		switch(pclMsg->siType)
		{
		// Ŭ���̾�Ʈ�� ����� �˸���. 
		case DBSYSTEMINFO_TODAY_NEWUSERNUM:
			SendServerResponseMsg(0, SRVAL_DBSYSTEMINFO,  DBSYSTEMINFO_TODAY_NEWUSERNUM, pclMsg->siValue, pclCM->CR[id]->GetCharUnique());
			break;

		case DBSYSTEMINFO_CALCULATE_WEALTHRANK:
			SendServerResponseMsg(0, SRVAL_DBSYSTEMINFO,  DBSYSTEMINFO_CALCULATE_WEALTHRANK, 2 , pclCM->CR[id]->GetCharUnique());
			break;
		case DBSYSTEMINFO_CALCULATE_TOTALMONEY:
			SendServerResponseMsg(0, SRVAL_DBSYSTEMINFO,  DBSYSTEMINFO_CALCULATE_TOTALMONEY, 3 , pclCM->CR[id]->GetCharUnique());
			break;
		}
	}

	// ��ȭ���� �������Ƿ� ������ �����Ѵ�.
	switch( pclMsg->siType )
	{
	case DBSYSTEMINFO_GET_TOTALMONEY:
		{
			siTotalMoney = pclMsg->siValue;
			m_pclBalanceCurrencyMgr->SetTodayCurrency(pclMsg->siValue);
			m_pclBalanceCurrencyMgr->SetUpdateState(CURRENCY_UPDATE_TODAY_DONE);
		}
		break;

	case DBSYSTEMINFO_GET_TOTALMONEY_YESTERDAY:
		{
			m_pclBalanceCurrencyMgr->SetYesterDayCurrency(pclMsg->siValue);
			m_pclBalanceCurrencyMgr->SetUpdateState(CURRENCY_UPDATE_ALL_DONE);
		}
		break;

		// ��޸� ���� ��Ű�� �Ŷ� ���� ������ ����.
	case DBSYSTEMINFO_CHANGEGRADE:
		{

		}break	;
	}
	return;

}

void cltServer::DoMsg_DBMSG_RESPONSE_DBSTARTINFO(sPacketHeader *pPacket)
{
	sDBResponse_DBStartInfo *pclMsg = (sDBResponse_DBStartInfo *)pPacket;
	
	if( pclMsg->siResult != 1 )						return;
	
	clServerStartInfo.Set( &pclMsg->clServerStartInfo );
	siStartBankruptDateVary = pclMsg->siStartBankruptDateVary;
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_DBStartInfo"));

	siFourLeafUnique = pclMsg->siFourLeafUnique;	//[����] ���� ������� ����Ŭ�ι� ����ũ => 2008-8-14

	// �⺻ ����Ŭ�ι��� �����Ѵ�.
	if ( 0 >= siFourLeafUnique )
	{
		// DB�� �������� ������, �⺻ ����Ŭ�ι��� �����ȴ�.
		//siFourLeafUnique = ITEMUNIQUE(2755);

		sDBRequest_SetFourLeaf clMsg( ITEMUNIQUE(2755) );
		SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETFOURLEAF(sPacketHeader *pPacket)
{
	sDBResponse_SetFourLeaf *pclMsg = (sDBResponse_SetFourLeaf*)pPacket;

	if( 1 != pclMsg->siResult )		return;

	siFourLeafUnique = pclMsg->siFourLeafUnique;	//[����] ���� ������� ����Ŭ�ι� ����ũ => 2008-8-14
}


void cltServer::DoMsg_DBMSG_RESPONSE_DBINITCOMPLETE(sPacketHeader *pPacket)
{
	sDBResponse_DBInitComplete *pRecvMsg = (sDBResponse_DBInitComplete *)pPacket;
	TraceBoot("DoMsg_DBMSG_RESPONSE_DBINITCOMPLETE select=%d step=%d (before set)",
		pRecvMsg->siSelect, pRecvMsg->siStep);
	if(pRecvMsg->siSelect < 0 || pRecvMsg->siSelect >= DBCONNECTION_NUMBER )		return;

	//bCompleteDBInit[ pRecvMsg->siSelect ] = TRUE;
	siCompleteDBInit[ pRecvMsg->siSelect ] = pRecvMsg->siStep;
	TraceBoot("DoMsg_DBMSG_RESPONSE_DBINITCOMPLETE select=%d step=%d -> siCompleteDBInit[cur=%d]=%d",
		pRecvMsg->siSelect, pRecvMsg->siStep, siCurrentDBInit, siCompleteDBInit[pRecvMsg->siSelect]);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_DBInitComplete"));

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETBYUNGZOINFO(sPacketHeader* pPacket)
{

	// BYUNGZO CLASS�� �����Ѵ�.
	pclMinisterMgr->DoMsg_DBMsgResponse_GetMinisterInfo(MINISTER_BYUNGZO, pPacket);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_GETBYUNGZOINFO"));

}
void cltServer::DoMsg_DBMSG_RESPONSE_GETGONGZOINFO(sPacketHeader* pPacket)
{
	// GONGZO
	pclMinisterMgr->DoMsg_DBMsgResponse_GetMinisterInfo(MINISTER_GONGZO, pPacket);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_GETGONGZOINFO"));

}
void cltServer::DoMsg_DBMSG_RESPONSE_GETHYUNGZOINFO(sPacketHeader* pPacket)
{
	// HYUNGZO
	pclMinisterMgr->DoMsg_DBMsgResponse_GetMinisterInfo(MINISTER_HYUNGZO, pPacket);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_GETHYUNGZOINFO"));

}
void cltServer::DoMsg_DBMSG_RESPONSE_GETHOZOINFO(sPacketHeader* pPacket)
{
	// HOZO
	pclMinisterMgr->DoMsg_DBMsgResponse_GetMinisterInfo(MINISTER_HOZO, pPacket);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_GETHOZOINFO"));

}
void cltServer::DoMsg_DBMSG_RESPONSE_GETIZOINFO(sPacketHeader* pPacket)
{
	// IZO
	pclMinisterMgr->DoMsg_DBMsgResponse_GetMinisterInfo(MINISTER_IZO, pPacket);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_GETIZOINFO"));

}
void cltServer::DoMsg_DBMSG_RESPONSE_GETYEZOINFO(sPacketHeader* pPacket)
{

	// YEZO CLASS�� �����Ѵ�.
	pclMinisterMgr->DoMsg_DBMsgResponse_GetMinisterInfo(MINISTER_YEZO, pPacket);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_GETYEZOINFO"));
	
}
void cltServer::DoMsg_DBMSG_RESPONSE_GETGOONZUINFO(sPacketHeader* pPacket)
{
	pclMinisterMgr->DoMsg_DBMsgResponse_GetMinisterInfo(MINISTER_GOONZU, pPacket);
	//�ֱ������� ������ �Ŷ� �ϴ� ����
	//pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_GETGOONZUINFO"));
}


/////////////////////

void cltServer::DoMsg_DBMSG_RESPONSE_SETBYUNGZOINFO(sPacketHeader* pPacket)
{
	// BYUNGZO
	pclMinisterMgr->DoMsg_DBMsgResponse_SetMinisterInfo(MINISTER_BYUNGZO, pPacket);
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("DoMsg_DBMSG_RESPONSE_SETBYUNGZOINFO"));
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETBYUNGZOINFO_VILLAGEWARTIME(sPacketHeader* pPacket)
{
	// ���� �ð��� �����ߴ�.
	sDBResponse_SetByungzoInfo_VillageWarTime * pclMsg =  (sDBResponse_SetByungzoInfo_VillageWarTime*)pPacket;

	SI32 id = pclMsg->usCharID;

	if(pclMsg->siResult == 1)
	{	
		// �����ð��� 40�к��� ������ ���� 60�� ���� Ŭ�� ����.
		if(pclMsg->siVillageWarTime < 40 || pclMsg->siVillageWarTime > 60) pclMsg->siVillageWarTime = 40;

		((CMinisterByungzo*)pclMinisterMgr->m_MinisterBase[MINISTER_BYUNGZO])->m_siVillageWarTime = pclMsg->siVillageWarTime;
				
		((CMinisterByungzo*)pclMinisterMgr->m_MinisterBase[MINISTER_BYUNGZO])->m_siPersonID = pclMsg->siPersonID;	
		
		StringCchCopy( ((CMinisterByungzo*)pclMinisterMgr->m_MinisterBase[MINISTER_BYUNGZO])->m_szGreeting ,256, pclMsg->m_szGreeting);


		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 10] // ���� ���� ����.
			// param1 == �����ð�.  
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_BYUNGZO_VILLAGE_WAR_SET, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclMsg->siVillageWarTime, 0, 0, 0, 0, NULL, NULL);
		}
	}
	else
	{
		if(((cltServer *)pclClient)->pclCM->IsAlive(id) == false)return ;

		// ���� Error
		((cltServer*)pclClient)->SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETBYUNGZOINFO,  0, 0, id);

	}

	if (pclClient->pclCM->IsValidID(id))
	{
		// ������ ��û�������� ���� �����ش�.
		((CMinisterByungzo*)pclMinisterMgr->m_MinisterBase[MINISTER_BYUNGZO])->DoMsg_GameMsgRequest_GetMinisterInfo(id);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETGONGZOINFO(sPacketHeader* pPacket)
{
	// GONGZO
	pclMinisterMgr->DoMsg_DBMsgResponse_SetMinisterInfo(MINISTER_GONGZO, pPacket);

}
void cltServer::DoMsg_DBMSG_RESPONSE_SETHYUNGZOINFO(sPacketHeader* pPacket)
{
	// HYUNGZO
	pclMinisterMgr->DoMsg_DBMsgResponse_SetMinisterInfo(MINISTER_HYUNGZO, pPacket);
}
void cltServer::DoMsg_DBMSG_RESPONSE_SETHOZOINFO(sPacketHeader* pPacket)
{
	// HOZO
	pclMinisterMgr->DoMsg_DBMsgResponse_SetMinisterInfo(MINISTER_HOZO, pPacket);

}
void cltServer::DoMsg_DBMSG_RESPONSE_SETIZOINFO(sPacketHeader* pPacket)
{
	// IZO
	pclMinisterMgr->DoMsg_DBMsgResponse_SetMinisterInfo(MINISTER_IZO, pPacket);

}
void cltServer::DoMsg_DBMSG_RESPONSE_SETYEZOINFO(sPacketHeader* pPacket)
{
	// YEZO
	pclMinisterMgr->DoMsg_DBMsgResponse_SetMinisterInfo(MINISTER_YEZO, pPacket);
	
}
void cltServer::DoMsg_DBMSG_RESPONSE_SETGOONZUINFO(sPacketHeader* pPacket)
{
	// GOONZU
	pclMinisterMgr->DoMsg_DBMsgResponse_SetMinisterInfo(MINISTER_GOONZU, pPacket);
}
void cltServer::DoMsg_DBMSG_RESPONSE_SETGOONZUFINANCES(sPacketHeader* pPacket)
{
	// GOONZUFINANCES
	sDBResponse_SetGoonzuFinances* pclMsg = (sDBResponse_SetGoonzuFinances* )pPacket;

	if ( pclMsg->siResult == 0 )
	{
		return ;
	}

	CMinisterGoonzu *pclminister = (CMinisterGoonzu *)pclMinisterMgr->GetMinister(MINISTER_GOONZU);
	if ( pclminister == NULL ) return ;

	pclminister->InitializeDB();

	
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 10] // ���� ����.	
		// param1 == �ӽ� ����, param2 == �� ����
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_GOONZU_FINANCES_SET, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										pclminister->m_iTempFinances, pclminister->m_iTotalFinances, 0, 0, 0, NULL, NULL);
	}
}
void cltServer::DoMsg_DBMSG_RESPONSE_GETANGELAPPLYERLIST(sPacketHeader* pPacket)
{
	CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_GetAngelApplyerList(pPacket);
}
void cltServer::DoMsg_DBMSG_RESPONSE_GETANGELLIST(sPacketHeader* pPacket)
{
	CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_GetAngelList(pPacket);
}
void cltServer::DoMsg_DBMSG_RESPONSE_SETANGELAPPLYER(sPacketHeader* pPacket)
{
	CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_SetAngelApplyer(pPacket);
}
void cltServer::DoMsg_DBMSG_RESPONSE_SETANGEL(sPacketHeader* pPacket)
{
	CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_SetAngel(pPacket);
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETPOLICELIST(sPacketHeader* pPacket)
{
	CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_GetPoliceList(pPacket);
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETPOLICE(sPacketHeader* pPacket)
{
	CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_SetPolice(pPacket);
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETCRIMINALLIST(sPacketHeader* pPacket)
{
	CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_GetCriminalList(pPacket);
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETCRIMINAL(sPacketHeader* pPacket)
{
	CMinisterHyungzo *pclminister = (CMinisterHyungzo *)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_SetCriminal(pPacket);
}

void cltServer::DoMsg_DBMSG_RESPONSE_PRISONUSER(sPacketHeader* pPacket)
{
	sDBResponse_PrisonUser* pclMsg = (sDBResponse_PrisonUser* )pPacket;

	SI32 id = pclMsg->usCharID;

	if ( pclMsg->siResult != 1 && pclCM->IsValidID(id) == true )
	{
		SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETHYUNGZOPRISON,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;
	}

	// �����Ǽ� ����Ʈ�� �ٽ� �ҷ��´�.
	sDBRequest_GetHyungzoInfo clMsg;
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

	// ������ ������ ����ڰ� ���������� Ȯ���Ѵ�. 
	SI32 prisonid		= 0;
	SI32 prisonpersonid = 0;
	if ( pclHashTable->Find( pclMsg->szPersonName, prisonpersonid, prisonid, 60 ) == true  )
	{
		if( pclCM->IsValidID(prisonid) )
		{
			pclCM->CR[prisonid]->pclCI->clGuiltyInfo.Set(&pclMsg->clGuiltyInfo);
			// �������� ������. 
			((cltCharServer*)pclCM->CR[prisonid])->SendPrison();
		}
	}

	// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
	if ( pclCM->IsValidID(id) == TRUE )
	{
		SendServerResponseMsg(0, SRVAL_PRISON_SUCCESS,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 10] // ���� �̵� �α׾���(��������).
		// param1	 == ���� ���� �ð�. 
		// param2	 == �� ���� �ð�. 
		// pszParam1 == ĳ�����̸�, 
		// pszParam2 == ����.
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRISON, LOGCOMMAND_INDEX_PRISON_IN, 
							0, NULL, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
							pclMsg->clGuiltyInfo.siLeftPrisonSecond, pclMsg->clGuiltyInfo.siTotalPrisonSecond, 0, 0, 0, 
							pclMsg->szPersonName, pclMsg->clGuiltyInfo.szPrisonReason);
	}
}

void cltServer::DoMsg_DBLOGMSG_RESPONSE_PERSONAL_LOG(sPacketHeader* pPacket)
{
	/*
	sDBResponse_Personal_Log *pclMsg = (sDBResponse_Personal_Log *)pPacket;

	if ( pclMsg->siResult == 0 ) return ;

	SI32 id = pclMsg->usCharID ;
	if ( id <= 0 ) return ;
	
	stLOGData	stlogdata[MAX_STLOGDATA_PER_PACKET];
	
	for ( SI32 i = 0 ; i < MAX_STLOGDATA_PER_PACKET ; i ++ )
	{
		stlogdata[i].Init();
		if ( !_tcscmp(pclMsg->stLogData[i].szDate,TEXT("")) ) continue ;

		stlogdata[i].Set(&pclMsg->stLogData[i]);
	}

	cltGameMsgResponse_Personal_Log clinfo(pclMsg->siResult,stlogdata);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONAL_LOG, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	*/

	return ;
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETCHANGEDNAME(sPacketHeader* pPacket)
{
	sDBResponse_GetChangedName *pclMsg = (sDBResponse_GetChangedName *)pPacket;

	if ( pclCM->IsValidID(pclMsg->usCharID) == false ) return ;

	if ( pclMsg->siResult == 1 )
	{
		if ( pclCM->GetIDFromPersonID(pclMsg->siPersonID) == pclMsg->usCharID )
		{
			cltGameMsgResponse_GetChangedName clinfo(pclMsg->siResult,pclMsg->siPage,pclMsg->stChangedNameChar);
			cltMsg clMsg(GAMEMSG_RESPONSE_GETCHANGEDNAME, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[pclMsg->usCharID]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETDAILYNEWCHAR(sPacketHeader* pPacket)
{
	CMinisterHozo *pclminister = (CMinisterHozo *)pclMinisterMgr->GetMinister(MINISTER_HOZO);
	if ( pclminister == NULL ) return ;

	pclminister->DoMsg_DBMsgResponse_GetDailyNewChar(pPacket);
}

void cltServer::DoMsg_DBMSG_RESPONSE_INITSTATISTICS(sPacketHeader* pPacket)
{
	sDBResponse_InitStatistics *pclMsg = (sDBResponse_InitStatistics *)pPacket;

	memcpy( &clCurrencyInfo, &pclMsg->clCurrencyInfo, sizeof( cltCurrencyInfo ) );
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_InitStatistics"));
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETWARRANKINFO(sPacketHeader* pPacket)
{
	sDBResponse_GetWarRankInfo *pclMsg = (sDBResponse_GetWarRankInfo *)pPacket;

	pclWarRankManager->SetWarRankInfo( &pclMsg->clWarRankInfo );
	pclClient->pclserverinfoDlg->OnServerInfoDialog(TEXT("sDBResponse_GetWarRankInfo"));

}

void cltServer::DoMsg_DBMSG_RESPONSE_SETWARRANKINFO(sPacketHeader* pPacket)
{
	sDBResponse_SetWarRankInfo *pclMsg = (sDBResponse_SetWarRankInfo *)pPacket;
	
	pclWarRankManager->SetWarRankInfo( &pclMsg->clWarRankInfo );
}

void cltServer::DoMsg_DBMSG_RESPONSE_WENNYOLDMAN_SAVEINVENTORY(sPacketHeader *pPacket)
{
	sDBResponse_Wennyoldman_SaveInventory *pclMsg = (sDBResponse_Wennyoldman_SaveInventory*)pPacket;

	if (pclCM->IsValidID(pclMsg->usCharID) == false) return;

	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[pclMsg->usCharID];

	if(pclchar->pclCI->GetPersonID() != pclMsg->siPersonID) return;

	
	if( pclMsg->siResult > 0)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// ���ص� ������
			// param1 == �����ϴ� �������� ��ġ 
			// param2 == ���� ����
			// param3 == ���� �ۼ�Ʈ
			// param4 == �������� ���� �г�Ƽ
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_WENNY_OLDMAN_USEITEM, 
								0, (cltCharServer*)pclCM->CR[pclMsg->usCharID], NULL, 0, 0, 0, &pclMsg->clUseItem_Log, pclMsg->clUseItem.siItemNum, 0, 0, 0, 0, 0,
								pclMsg->siDisassembleItemPos, pclMsg->siWennyPercent, pclMsg->siResolutionRate, pclMsg->siDurabilityPenalty, 0,
								NULL, NULL);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			for (SI32 i=0; i<MAX_ITEM_PER_WENNYOLD; i++ )
			{
				if( pclMsg->clItemList[i].siUnique > 0 )
				{
					// [�α��߰� : Ȳ���� 2007. 10. 11] // Ȥ�θ�(2ȣ��) �κ��丮�� ���� ��û
					// param1 == ���ص� ������ ����ũ
					// param2 == ���ص� ������ SID
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_WENNY_OLDMAN_SAVE_INV, 
										0, (cltCharServer*)pclCM->CR[pclMsg->usCharID], NULL, 0, 0, 0, &pclMsg->clItemList[i], pclMsg->clItemList[i].siItemNum, 0, 0, 0, 0, 0, 
										pclMsg->clUseItem_Log.siUnique, pclMsg->clUseItem_Log.siSID, 0, 0, 0, 
										NULL, NULL);
				}
			}
		}

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[pclMsg->usCharID];

		//================================================ 
		// Client�� ���� ��ȣ�� â�� Update�Ѵ�. 
		//================================================ 
		{
			cltGameMsgResponse_ItemDisAssemble clItemDisAssemble(pclMsg->siResult, pclMsg->clItemList, pclMsg->siWennyPercent, pclMsg->siResolutionRate, pclMsg->siDurabilityPenalty);
			cltMsg clMsg(GAMEMSG_RESPONSE_DISASSEMBLEITEM, sizeof(clItemDisAssemble), (BYTE*)&clItemDisAssemble);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);

		}
		
		//================================================ 
		// Client�� ���� �κ��丮 Update�Ѵ�. 
		//================================================ 
		if( pclMsg->siDisassembleItemPos >= 0 && pclMsg->siDisassembleItemPos < MAX_ITEM_PER_PERSON )
		{
			pclchar->pclCI->clCharItem.clItem[pclMsg->siDisassembleItemPos].Set(&pclMsg->clUseItem);
			
			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siDisassembleItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clUseItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}


		if(pclClient->IsCountrySwitch(Switch_WennyOldMan_CityhallProfitMoney))
		{
			// ��ȣ�� �����Ͽ��� ��� ����(����) �ں��� ����
			SI32 currentVillage = pclchar->GetCurrentVillageUnique();
			if( currentVillage>0 && currentVillage < MAX_VILLAGE_NUMBER)
			{				
				GMONEY siGetVillageMoney = 0;
				SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

				// ���� ������
				if( pclGachaManager->IsGachaRareItem(pclMsg->siUsedItemUnique) )
				{
					siGetVillageMoney = npcrate * 25;
				}
				// ������ �� ������
                else if( pclItemManager->IsItemInfoAtb(pclMsg->siUsedItemUnique, ITEMINFOATB_ITEMMALL) == true)
				{
					siGetVillageMoney = npcrate * 10;
				}
				// �Ϲ� ������
				else
				{
					SI32 ref = pclItemManager->FindItemRefFromUnique( pclMsg->siUsedItemUnique );
					SI32 TempNPCRate = max(500, npcrate);	
					if(ref > 0 && ref < MAX_ITEMINFO_NUMBER)
					{
                        siGetVillageMoney = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel * TempNPCRate / (SI32)10;
						siGetVillageMoney = siGetVillageMoney * max(1, pclItemManager->pclItemInfo[ref]->clProductionInfo.GetToolCount());
					}
				}

				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, currentVillage, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}
		else
		{
			// ��ȣ�� �����Ͽ��� ��� ����(����) �ں��� +5000
			SI32 currentVillage = pclchar->GetCurrentVillageUnique();
			if( currentVillage>0 && currentVillage < MAX_VILLAGE_NUMBER)
			{
				GMONEY siGetVillageMoney = 5000;
				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, currentVillage, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}
		// ���� ����Ʈ �ִ°�� ������Ʈ.
		pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_DISASSEMBLEITEM);
	
	}
	else
	{
		SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_FULLINVEN, 0, 0, pclMsg->usCharID);
	}


}

void cltServer::DoMsg_DBMSG_RESPONSE_WENNYOLDMAN_MOVEITEMSTGTOINV(sPacketHeader *pPacket)
{
	sDBResponse_Wennyoldman_MoveItemStgToInv *pclMsg = (sDBResponse_Wennyoldman_MoveItemStgToInv*)pPacket;

	SI32 id = pclMsg->usCharID;

	if (pclCM->IsValidID(id) == false) return;

	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[id];

	if( pclMsg->siResult > 0 )
	{
		sDBRequest_PersonItemInfoGet clMsg(pclchar->siID, pclchar->pclCI->GetPersonID());
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);

		sDBRequest_Wennyoldman_InventoryList clMsg2(pclchar->siID, pclchar->pclCI->GetPersonID());
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg2);

	
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 11] // Ȥ�θ�(2ȣ��) �κ��丮�� ����� �κ����� �̵�
			// param1 == Ȥ�θ�(2ȣ��) �κ��丮�� ��ġ, 
			// param2 == �κ��丮 ��ġ, 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_WENNY_OLDMAN_STG_TO_INV, 
								 0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clItem_Log, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
								 pclMsg->siStgPos, pclMsg->siInvPos, 0, 0, 0, 
								 NULL, NULL);
		}
	}
	else
	{
		SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FULLINVEN, 0, 0, id);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_WENNYOLDMAN_INVENTORYLIST(sPacketHeader *pPacket)
{
	sDBResponse_Wennyoldman_InventoryList *pclMsg = (sDBResponse_Wennyoldman_InventoryList*)pPacket;

	if (pclCM->IsValidID(pclMsg->usCharID) == false) return;
    
	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[pclMsg->usCharID];

	cltGameMsgResponse_ItemDisAssemble_List clItemDisAssembleList(pclMsg->siResult, pclMsg->clItemList);
	cltMsg clMsg(GAMEMSG_RESPONSE_DISASSEMBLEITEM_LIST, sizeof(clItemDisAssembleList), (BYTE*)&clItemDisAssembleList);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

}






void cltServer::DoMsg_DBMSG_RESPONSE_VILLAGEBANKRUPTCY(sPacketHeader* pPacket)
{
	sDBResponse_VillageBankruptcy *pclMsg = (sDBResponse_VillageBankruptcy*)pPacket;

	if( 1 != pclMsg->siResult )
		return;

	siStartBankruptDateVary = pclMsg->siDateVary;

	if( siStartBankruptDateVary == 0 )
	{
		SI32 i = 0;
		for( i = 0; i < MAX_VILLAGE_NUMBER; ++i )
		{
			cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[i];
			if( pclCityHall == NULL ) 
				continue;

			pclCityHall->bPrepareBankruptcy = FALSE;

		}
	}
	else
	{
		for( int i = 0; i < 5; ++i )
		{
			if ( pclMsg->siVillageUnique[i] <= 0 || pclMsg->siVillageUnique[i] >= MAX_VILLAGE_NUMBER )
				continue;

			cltCityHall *pclCityHall = (cltCityHall *)pclCityHallManager->pclStruct[pclMsg->siVillageUnique[i]];
			if( pclCityHall == NULL ) 
				continue;

			pclCityHall->bPrepareBankruptcy = TRUE;
		}
	}
}


//***************************************************************************************************************************
//
// �߰� : Ȳ���� 2008. 2. 28 => ��Ƽ��Ʈ DB->S �޽��� ó�� �����κ�.
//
//[�߰� : Ȳ���� 2008. 2. 28 => ��Ƽ��Ʈ ������ ȯ�� DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_ARTIFACT_SAVEINVENTORY(sPacketHeader *pPacket)
{
	sDBResponse_Artifact_SaveInventory *pclMsg = (sDBResponse_Artifact_SaveInventory*)pPacket;

	if (pclCM->IsValidID(pclMsg->usCharID) == false) return;

	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[pclMsg->usCharID];

	if(pclchar->pclCI->GetPersonID() != pclMsg->siPersonID) return;


	if( pclMsg->siResult > 0)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// ���ص� ������
			// param1 == �����ϴ� �������� ��ġ 
			// param2 == ���� ����
			// param3 == ���� �ۼ�Ʈ
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_ARTIFACT_USEITEM, 
				0, (cltCharServer*)pclCM->CR[pclMsg->usCharID], NULL, 0, 0, 0, &pclMsg->clUseItem_Log, pclMsg->clUseItem.siItemNum, 0, 0, 0, 0, 0,
				pclMsg->siDisassembleItemPos, pclMsg->siWennyPercent, pclMsg->siResolutionRate, 0, 0,
				NULL, NULL);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			for (SI32 i=0; i<MAX_ITEM_ARTIFACT; i++ )
			{
				if( pclMsg->clItemList[i].siUnique > 0 )
				{
					// [�α��߰� : Ȳ���� 2007. 10. 11] // Ȥ�θ�(2ȣ��) �κ��丮�� ���� ��û
					// param1 == ȯ���� ������ ����ũ
					// param2 == ȯ���� ������ SID
					SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_ARTIFACT_SAVE_INV, 
						0, (cltCharServer*)pclCM->CR[pclMsg->usCharID], NULL, 0, 0, 0, &pclMsg->clItemList[i], pclMsg->clItemList[i].siItemNum, 0, 0, 0, 0, 0, 
						pclMsg->clUseItem_Log.siUnique, pclMsg->clUseItem_Log.siSID, 0, 0, 0, 
						NULL, NULL);
				}
			}
		}

		// ���
		NStatistics_Add("Item", pclMsg->clUseItem_Log.siUnique, "ResolveNum", pclMsg->clUseItem_Log.siItemNum);
		NStatistics_Set("Item", pclMsg->clUseItem_Log.siUnique, "Name", pclItemManager->GetName(pclMsg->clUseItem_Log.siUnique));

		for (SI32 i=0; i<MAX_ITEM_ARTIFACT; i++ )
		{
			if( pclMsg->clItemList[i].siUnique > 0 )
			{
				NStatistics_Add("Item", pclMsg->clItemList[i].siUnique, "FromResolve", pclMsg->clItemList[i].siItemNum);
				NStatistics_Set("Item", pclMsg->clItemList[i].siUnique, "Name", pclItemManager->GetName(pclMsg->clItemList[i].siUnique));
			}
		}

		cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[pclMsg->usCharID];

		//================================================ 
		// Client�� ���� ��ȣ�� â�� Update�Ѵ�. 
		//================================================ 
		{
			cltGameMsgResponse_ItemDisAssemble clItemDisAssemble(pclMsg->siResult, pclMsg->clItemList, pclMsg->siWennyPercent , pclMsg->siResolutionRate, 0,  ItemDisAssemble_NPC_KIND::NPC_ATIFACT);
			cltMsg clMsg(GAMEMSG_RESPONSE_DISASSEMBLEITEM, sizeof(clItemDisAssemble), (BYTE*)&clItemDisAssemble);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);

		}

		//================================================ 
		// Client�� ���� �κ��丮 Update�Ѵ�. 
		//================================================ 
		if( pclMsg->siDisassembleItemPos >= 0 && pclMsg->siDisassembleItemPos < MAX_ITEM_PER_PERSON )
		{
			pclchar->pclCI->clCharItem.clItem[pclMsg->siDisassembleItemPos].Set(&pclMsg->clUseItem);

			cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siDisassembleItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clUseItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}


		if(pclClient->IsCountrySwitch(Switch_WennyOldMan_CityhallProfitMoney))
		{
			// ��ȣ�� �����Ͽ��� ��� ����(����) �ں��� ����
			SI32 currentVillage = pclchar->GetCurrentVillageUnique();
			if( currentVillage>0 && currentVillage < MAX_VILLAGE_NUMBER)
			{				
				GMONEY siGetVillageMoney = 0;
				SI32 npcrate = pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;

				// ���� ������
				if( pclGachaManager->IsGachaRareItem(pclMsg->siUsedItemUnique) )
				{
					siGetVillageMoney = npcrate * 25;
				}
				// ������ �� ������
				else if( pclItemManager->IsItemInfoAtb(pclMsg->siUsedItemUnique, ITEMINFOATB_ITEMMALL) == true)
				{
					siGetVillageMoney = npcrate * 10;
				}
				// �Ϲ� ������
				else
				{
					SI32 ref = pclItemManager->FindItemRefFromUnique( pclMsg->siUsedItemUnique );
					SI32 TempNPCRate = max(500, npcrate);
					if(ref > 0 && ref < MAX_ITEMINFO_NUMBER)
					{
						siGetVillageMoney = pclItemManager->pclItemInfo[ref]->clSkillInfo.siSkillLevel * TempNPCRate / (SI32)10;
						siGetVillageMoney = siGetVillageMoney * max(1, pclItemManager->pclItemInfo[ref]->clProductionInfo.GetToolCount());
					}
				}

				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, currentVillage, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}
		else
		{
			// ��ȣ�� �����Ͽ��� ��� ����(����) �ں��� +5000
			SI32 currentVillage = pclchar->GetCurrentVillageUnique();
			if( currentVillage>0 && currentVillage < MAX_VILLAGE_NUMBER)
			{
				GMONEY siGetVillageMoney = 5000;
				sDBRequest_ChangeCityhallProfitMoney sendMsg( CHANGESTRMONEY_CITYHALL_PROFIT, 0, currentVillage, siGetVillageMoney );
				pclGameDBProcess->SendMsg((sPacketHeader *)&sendMsg);
			}
		}
		// ���� ����Ʈ �ִ°�� ������Ʈ.
		pclchar->DoQuestCheckAndAction(QUEST_CLEAR_COND_DISASSEMBLEITEM);

		// ȯ���� ���� ����,�Ӽ����縦 ȹ���ϴ� ����Ʈ�� ���
		bool bGetElement	= pclchar->pclCI->clQuestInfo.IsQuestGoing( QUEST_CLEAR_COND_GETELEMENT_RESTORE );
		bool bGetPowder		= pclchar->pclCI->clQuestInfo.IsQuestGoing( QUEST_CLEAR_COND_GETPOWDER_RESTORE );

		if ( (true == bGetElement) || (true == bGetPowder) )
		{
			for ( SI32 siIndex=0; siIndex<MAX_ITEM_ARTIFACT; ++siIndex )
			{
				SI32 siItemUnique	= pclMsg->clItemList[siIndex].siUnique;
				SI32 siItemAmount	= pclMsg->clItemList[siIndex].siItemNum;

				switch ( siItemUnique )
				{
				case ITEMUNIQUE(1701):	// ��Ƭ ����
				case ITEMUNIQUE(1703):	// �Ƹ��� ����
				case ITEMUNIQUE(1705):	// ���� ����
				case ITEMUNIQUE(1707):	// ���� ����
				case ITEMUNIQUE(1709):	// ��Ʈ��Ƭ ����
				case ITEMUNIQUE(1711):	// ī��� ����
				case ITEMUNIQUE(1713):	// ������� ����
				case ITEMUNIQUE(1715):	// ����Ƭ ����
				case ITEMUNIQUE(1717):	// ���� ����
				case ITEMUNIQUE(1719):	// ��� ����
					{
						pclchar->IncQuestPara( QUEST_CLEAR_COND_GETELEMENT_RESTORE, siItemUnique, siItemAmount );
					}
					break;

				case ITEMUNIQUE(1751):	// �̻��� ���Ӽ� ����
				case ITEMUNIQUE(1753):	// �̻��� �ҼӼ� ����
				case ITEMUNIQUE(1755):	// �̻��� ���Ӽ� ����
				case ITEMUNIQUE(1757):	// �̻��� ���Ӽ� ����
				case ITEMUNIQUE(1759):	// �̻��� �ٶ��Ӽ� ����
				case ITEMUNIQUE(1761):	// �ź��� ���Ӽ� ����
				case ITEMUNIQUE(1763):	// �ź��� �ҼӼ� ����
				case ITEMUNIQUE(1765):	// �ź��� ���Ӽ� ����
				case ITEMUNIQUE(1767):	// �ź��� ���Ӽ� ����
				case ITEMUNIQUE(1769):	// �ź��� �ٶ��Ӽ� ����
				case ITEMUNIQUE(1771):	// ȯ���� ���Ӽ� ����
				case ITEMUNIQUE(1773):	// ȯ���� �ҼӼ� ����
				case ITEMUNIQUE(1775):	// ȯ���� ���Ӽ� ����
				case ITEMUNIQUE(1777):	// ȯ���� ���Ӽ� ����
				case ITEMUNIQUE(1779):	// ȯ���� �ٶ��Ӽ� ����
					{
						pclchar->IncQuestPara( QUEST_CLEAR_COND_GETPOWDER_RESTORE, siItemUnique, siItemAmount );
					}
					break;
				}	// switch ( siItemUnique ) ����
			}	// for ( SI32 siIndex=0; siIndex<MAX_ITEM_ARTIFACT; ++siIndex ) ����
		}	// if ( (true == bGetElement) || (true == bGetPowder) ) ����

	}
	else
	{
		SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FAILDISASSEMBLE_FULLINVEN, 0, 0, pclMsg->usCharID);
	}


}

//[�߰� : Ȳ���� 2008. 2. 28 => ��Ƽ��Ʈ ������ �̵� DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_ARTIFACT_MOVEITEMSTGTOINV(sPacketHeader *pPacket)
{
	sDBResponse_Artifact_MoveItemStgToInv *pclMsg = (sDBResponse_Artifact_MoveItemStgToInv*)pPacket;

	SI32 id = pclMsg->usCharID;

	if (pclCM->IsValidID(id) == false) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ pclMsg->usCharID ];
	if( NULL == pclchar )
		return;


	if( pclMsg->siResult > 0 )
	{
		sDBRequest_PersonItemInfoGet clMsg(pclchar->siID, pclchar->pclCI->GetPersonID());
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg);

		sDBRequest_Artifact_InventoryList clMsg2(pclchar->siID, pclchar->pclCI->GetPersonID());
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg2);


		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 11] // Ȥ�θ�(2ȣ��) �κ��丮�� ����� �κ����� �̵�
			// param1 == ��Ƽ��Ʈ �κ��丮�� ��ġ, 
			// param2 == �κ��丮 ��ġ, 
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_ARTIFACT_STG_TO_INV, 
				0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clMoveItem, pclMsg->clItem.siItemNum, 0, 0, 0, 0, 0, 
				pclMsg->siStgPos, pclMsg->siInvPos, 0, 0, 0, 
				NULL, NULL);
		}
	}
	else
	{
		SendServerResponseMsg(0, SRVAL_WENNYOLDMAN_FULLINVEN, 0, 0, id);
	}

}

//[�߰� : Ȳ���� 2008. 2. 28 => ��Ƽ��Ʈ �κ� ����Ʈ  DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_ARTIFACT_INVENTORYLIST(sPacketHeader *pPacket)
{
	sDBResponse_Artifact_InventoryList *pclMsg = (sDBResponse_Artifact_InventoryList*)pPacket;

	if (pclCM->IsValidID(pclMsg->usCharID) == false) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ pclMsg->usCharID ];
	if( NULL == pclchar )
		return;


	cltGameMsgResponse_ItemDisAssemble_List clItemDisAssembleList( pclMsg->siResult, pclMsg->clItemList, ItemDisAssemble_NPC_KIND::NPC_ATIFACT );
	cltMsg clMsg(GAMEMSG_RESPONSE_DISASSEMBLEITEM_LIST, sizeof(clItemDisAssembleList), (BYTE*)&clItemDisAssembleList);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

}

//[�߰� : Ȳ���� 2008. 2. 28 => ��Ƽ��Ʈ ���� DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_ARTIFACT_MIXTURE(sPacketHeader *pPacket)
{
	sDBResponse_Artifact_Mixture* pclMsg = (sDBResponse_Artifact_Mixture*)pPacket;

	if ( false == pclCM->IsValidID( pclMsg->m_usCharID ) ) 
		return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ pclMsg->m_usCharID ];
	if( NULL == pclchar )
		return;

	//if( 0 >= pclMsg->m_siResult )
	//{
	//	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//	{
	//		// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
	//		// param1 == ������ ���յ� ������ ��ġ. 
	//		cltServer* pclServer = (cltServer*)pclClient;
	//		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ARTIFACT, LOGCOMMAND_INDEX_ARTIFACT_MIXTURE, 
	//										0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
	//										-1, 0, 0, 0, 0, NULL, NULL);
	//	}
	//	return;
	//}
	
	pclchar->pclCI->clCharItem.clItem[pclMsg->m_siEquipItemPos].Set( &pclMsg->m_clEquipItem );
	
	SI32	 ItemPos		  = -1;
	cltItem* pclCreateCrystal = NULL; 
	for( SI32 Count = 0; Count < MAX_CRYSTAL_NUM; ++Count )
	{
		ItemPos			 =  pclMsg->m_siCrystalItemPosList[ Count ];
		if( 0 >= ItemPos )
			continue;

		pclCreateCrystal = &pclMsg->m_clCrystalItemList[ Count ];	
	
		pclchar->pclCI->clCharItem.clItem[ ItemPos ].Set( pclCreateCrystal );
	}


	cltGameMsgResponse_Artifact_Mixture cltInfo( pclMsg->m_siEquipItemPos,		&pclMsg->m_clEquipItem,
												 &pclMsg->m_siCrystalItemPosList[0], &pclMsg->m_clCrystalItemList[0] );
	cltMsg clMsg(GAMEMSG_RESPONSE_ARTIFACT_MIXTURE, sizeof(cltGameMsgResponse_Artifact_Mixture), (BYTE*)&cltInfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
	
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
		// param1 == ������ ���յ� ������ ��ġ. 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ARTIFACT, LOGCOMMAND_INDEX_ARTIFACT_MIXTURE, 
							0, pclchar, NULL, 0, 0, 0, &pclMsg->m_clEquipItem, pclMsg->m_clEquipItem.GetItemNum(), 0, 0, 0, 0, 0, 
							pclMsg->m_siEquipItemPos, 0, 0, 0, 0, NULL, NULL);
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		for( SI32 i=0; i<pclMsg->m_clCrystalItemList_Log.GetSize(); i++ )
		{
			if( pclMsg->m_clCrystalItemList_Log[i].siUnique <= 0 )		continue;

			// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
			// param1 == ������ ���յ� ������ ��ġ. 
			// param2 == ����� ������ �������� ����ũ
			// param3 == ����� ������ �������� SID
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM, 
				USEITEM_REASON_ARTIFACT_MIXTURE, pclchar, NULL, 0, 0, 0, &pclMsg->m_clCrystalItemList_Log[i], pclMsg->m_clCrystalItemList[i].GetItemNum(), 0, 0, 0, 0, 0, 
				pclMsg->m_siEquipItemPos, pclMsg->m_clEquipItem.siUnique, pclMsg->m_clEquipItem.siSID, 0, 0, NULL, NULL);
		}
	}

	// ���⿡ ����ü �����غ��� ����Ʈ�� �������̶��
	if ( pclMsg->m_clEquipItem.IsWeapon(pclItemManager) )
	{
		pclchar->IncQuestPara( QUEST_CLEAR_COND_ARTIFACTMIXWEAPON, pclMsg->m_clEquipItem.siUnique, 1 );
	}

	// ����ü �����غ��� ����Ʈ�� �������̶��
	pclchar->IncQuestPara( QUEST_CLEAR_COND_ARTIFACTMIX, 0, 1 );
	
}

//[�߰� : Ȳ���� 2008. 2. 28 => ��Ƽ��Ʈ ���� DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_ARTIFACT_REMOVE(sPacketHeader *pPacket)
{
	sDBResponse_Artifact_Remove *pclMsg = (sDBResponse_Artifact_Remove*)pPacket;

	if ( false == pclCM->IsValidID( pclMsg->m_usCharID ) ) 
		return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ pclMsg->m_usCharID ];
	if( NULL == pclchar )
		return;

	//if( 0 >= pclMsg->m_siResult )
	//{
	//	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//	{
	//		// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
	//		// param1 == ������ ���յ� ������ ��ġ. 
	//		cltServer* pclServer = (cltServer*)pclClient;
	//		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ARTIFACT, LOGCOMMAND_INDEX_ARTIFACT_REMOVE, 
	//										0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
	//										-1, 0, 0, 0, 0, NULL, NULL);
	//	}
	//	return;
	//}

	SI32	 ItemPos		  = -1;
	cltItem* pclCreateCrystal = NULL; 
	cltItem* pclDeleteCrystal = NULL; 
	for( SI32 Count = 0; Count < MAX_CRYSTAL_NUM; ++Count )
	{
		// ���� ����ü.(���� ����ü)
		ItemPos			 =  pclMsg->m_siCrystalItemPosList[ Count ];
		pclDeleteCrystal = &pclMsg->m_clCrystalItemList[ Count ];	

		if( 0 < ItemPos && MAX_ITEM_PER_PERSON > ItemPos)
			pclchar->pclCI->clCharItem.clItem[ ItemPos ].Set( pclDeleteCrystal );

		 			
		// ������ ����ü.(��� ���� ���ִ� ����ü)
		ItemPos			 =  pclMsg->m_siCreateCrystalItemPosList[ Count ];
		pclCreateCrystal = &pclMsg->m_clCreateCrystalItemList[ Count ];	
		
		if( 0 < pclCreateCrystal->siUnique )
			pclchar->pclCI->clCharItem.clItem[ ItemPos ].Set( pclCreateCrystal );
	}

	if( 0 < pclMsg->m_clEquipItem.siUnique )
		pclchar->pclCI->clCharItem.clItem[ pclMsg->m_siEquipItemPos ].Set( &pclMsg->m_clEquipItem );

	cltGameMsgResponse_Artifact_Remove cltInfo( pclMsg->m_siEquipItemPos,				&pclMsg->m_clEquipItem,
												pclMsg->m_siCrystalItemPosList,			 pclMsg->m_clCrystalItemList,
												pclMsg->m_siCreateCrystalItemPosList, 	 pclMsg->m_clCreateCrystalItemList	);
	cltMsg clMsg(GAMEMSG_RESPONSE_ARTIFACT_REMOVE, sizeof(cltGameMsgResponse_Artifact_Remove), (BYTE*)&cltInfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
		// param1 == ������ ���յ� ������ ��ġ. 
		cltServer* pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ARTIFACT, LOGCOMMAND_INDEX_ARTIFACT_REMOVE, 
										0, pclchar, NULL, 0, 0, 0, &pclMsg->m_clEquipItem, 0, 0, 0, 0, 0, 0, 
										pclMsg->m_siEquipItemPos, 0, 0, 0, 0, NULL, NULL);
	}
}

//[�߰� : Ȳ���� 2008. 2. 28 => ��Ƽ��Ʈ ���� ���� DB->S]
void cltServer::DoMsg_DBMSG_RESPONSE_ARTIFACT_SLOT_CREATE(sPacketHeader *pPacket)
{
	sDBResponse_Artifact_Slot_Create *pclMsg = (sDBResponse_Artifact_Slot_Create*)pPacket;

	if ( false == pclCM->IsValidID( pclMsg->m_usCharID ) ) 
		return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ pclMsg->m_usCharID ];
	if( NULL == pclchar )
		return;
	
	//if( 0 >= pclMsg->m_siResult )
	//{
	//	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//	{
	//		// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
	//		// param1 == ������ ���յ� ������ ��ġ. 
	//		cltServer* pclServer = (cltServer*)pclClient;
	//		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ARTIFACT, LOGCOMMAND_INDEX_ARTIFACT_SLOT_CREATE, 
	//										0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
	//										-1, 0, 0, 0, 0, NULL, NULL);
	//	}
	//	return;
	//}

	

	pclchar->pclCI->clCharItem.clItem[pclMsg->m_siEquipItemPos].Set( &pclMsg->m_clEquipItem );
	pclchar->pclCI->clCharItem.clItem[pclMsg->m_siCrystalItemPos].Set( &pclMsg->m_clCrystalItem);
	

	cltGameMsgResponse_Artifact_Slot_Create cltInfo( pclMsg->m_siEquipItemPos,	 &pclMsg->m_clEquipItem,
													 pclMsg->m_siCrystalItemPos, &pclMsg->m_clCrystalItem );
	cltMsg clMsg(GAMEMSG_RESPONSE_ARTIFACT_SLOT_CREATE, sizeof(cltGameMsgResponse_Artifact_Slot_Create), (BYTE*)&cltInfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
		// param1 == ������ ���յ� ������ ��ġ. 
		// param2 == ���� ���� ����
		// param3 == ���� ������ ���� ����
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_ARTIFACT, LOGCOMMAND_INDEX_ARTIFACT_SLOT_CREATE, 
							0, pclchar, NULL, 0, 0, 0, &pclMsg->m_clEquipItem, pclMsg->m_clEquipItem.GetItemNum(), 0, 0, 0, 0, 0, 
							pclMsg->m_siEquipItemPos, pclMsg->m_clEquipItem_ForLog.siUseCrystalNum, pclMsg->m_clEquipItem.siUseCrystalNum, 0, 0, NULL, NULL);
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2008-4-16 ] // ��Ƽ��Ʈ ���� �α� �ۼ�.
		// param1 == ���Ǵ� ���� ����ü�� ��ġ
		// param2 == ������ ������ �������� ����ũ
		// param3 == ������ ������ �������� SID
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM, 
			USEITEM_REASON_ARTIFACT_SLOT_CREATE, pclchar, NULL, 0, 0, 0, &pclMsg->m_clCrystalItem_ForLog, pclMsg->m_clCrystalItem.GetItemNum(), 0, 0, 0, 0, 0, 
			pclMsg->m_siCrystalItemPos, pclMsg->m_clEquipItem.siUnique, pclMsg->m_clEquipItem.siSID, 0, 0, NULL, NULL);
	}
}

//****************************************************************************************************************************************************************
//[����] ���� �̺�Ʈ ó�� �Լ� (DB->S) => 2008-6-20
//****************************************************************************************************************************************************************

void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_RANK_INFO(sPacketHeader *pPacket)
{
	sDBResponse_Server_Event_Rank_Info *pclMsg = (sDBResponse_Server_Event_Rank_Info*)pPacket;

	if(0 > pclMsg->m_siResult)
		return;

	// �� ������ ������ ã�´�.
	CNServerDailyRankInfo clTempServerDailyRankInfo;
	ZeroMemory( &clTempServerDailyRankInfo, sizeof(clTempServerDailyRankInfo) );
	for( SI32 siCount = 0; siCount < SERVER_MAX_NUM; ++siCount )
	{
		if( siServerUnique == pclMsg->m_clServerDailyRankInfo[siCount].GetServerUnique() )
		{
			clTempServerDailyRankInfo = pclMsg->m_clServerDailyRankInfo[siCount];

			cltClient* pclclient = (cltClient*) pclClient;
			clTempServerDailyRankInfo.SetServerName(pclclient->GetServerNameFromUnique( clTempServerDailyRankInfo.GetServerUnique() ) );
			break;
		}
	}

	// ��ǥ ���� ����.
	CNServerDailyRankInfo clTargetServerDailyRankInfo;
	clTargetServerDailyRankInfo.SetScore( GetGlobalValue("GV_GoonzuDay_Week_Score") );
	clTargetServerDailyRankInfo.SetServerUnique( 0 );
	clTargetServerDailyRankInfo.SetServerName( GetTxtFromMgr(40854) );

	CNServerRankInfo clTargetServerRankInfo;
	clTargetServerRankInfo.SetScore( GetGlobalValue("GV_GoonzuDay_Week_Score") );
	clTargetServerRankInfo.SetServerUnique( 0 );
	clTargetServerRankInfo.SetServerName( GetTxtFromMgr(40854) );

	CNServerRankInfo clTempServerRankInfo;
	clTempServerRankInfo.SetScore( clTempServerDailyRankInfo.GetScore() );
	clTempServerRankInfo.SetServerUnique( clTempServerDailyRankInfo.GetServerUnique() );
	cltClient* pclclient = (cltClient*) pclClient;
	clTempServerRankInfo.SetServerName( pclclient->GetServerNameFromUnique( clTempServerRankInfo.GetServerUnique() ) );
		
	// DB���� ���� ���� ������ �� ���� ������ ��ǥ������ �� ����.
	ZeroMemory( &pclMsg->m_clServerRankInfo,		sizeof(pclMsg->m_clServerRankInfo) );
	ZeroMemory( &pclMsg->m_clServerDailyRankInfo,	sizeof(pclMsg->m_clServerDailyRankInfo) );
	
	if( clTargetServerDailyRankInfo.GetScore() >= clTempServerDailyRankInfo.GetScore() )
	{
		pclMsg->m_clServerRankInfo[0] = clTargetServerRankInfo;
		pclMsg->m_clServerRankInfo[1] = clTempServerRankInfo;

		pclMsg->m_clServerDailyRankInfo[0] = clTargetServerDailyRankInfo;
		pclMsg->m_clServerDailyRankInfo[1] = clTempServerDailyRankInfo;
	}
	else
	{
		pclMsg->m_clServerRankInfo[1] = clTargetServerRankInfo;
		pclMsg->m_clServerRankInfo[0] = clTempServerRankInfo;

		pclMsg->m_clServerDailyRankInfo[1] = clTargetServerDailyRankInfo;
		pclMsg->m_clServerDailyRankInfo[0] = clTempServerDailyRankInfo;
	}
	
	pclServerEventManager->SetServerRank(pclMsg->m_clServerRankInfo);
	pclServerEventManager->SetServerDailyRank(pclMsg->m_clServerDailyRankInfo);
	pclServerEventManager->SetPersonalRank(pclMsg->m_clPersonalRankInfo);
	pclServerEventManager->SetBonusBufType(pclMsg->m_ServerEventBufKind);
 
	
	switch(pclMsg->m_siReason)
	{
		case SERVER_EVENT_REASON_INSTANCE_MAP_CLEAR: // �δ� Ŭ����(����).
			{
				//pclServerEventManager->SendDBMsgServerInstanseMapClear();
			}
			break;
		case SERVER_EVENT_REASON_DAILRY:	// �Ϸ� ���� ������Ʈ.
			{
				// �Ϸ簡 ����� ��� ������ ��û�ϴµ� �� ������ 1���̸�.
				if(pclServerEventManager->GetBackUpDailyServerUniqueRank(0) == pclClient->siServerUnique)
				{	
					//�Ϸ� ������ 1�� �����϶� �������� ���� ����.(DB�� ����Ѵ�.)
					pclServerEventManager->MyServerDailyRankBestBuf();

					//�Ϸ� ������ 1�����ϸ� ��� ����.
					SI16 RankPonitScore = 20;
					
					// �Ϸ� ������ 1���� �Ǿ��� ������ ������ �˸�.
					cltServerResponseValue clreturn(0, SRVAL_SERVER_EVENT_RANK_1, RankPonitScore, 0);
					cltMsg clMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
					pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
				}
				else
				{
					pclServerEventManager->MyServerDailyRankBestBufNo();
				}

				if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
				{
					cltServer * pclServer = (cltServer*)pclClient;

					// [�α��߰� : Ȳ���� 2008. 7. 7] ���� �̺�Ʈ
					// param1 = ���� ��ü ����.
					pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SERVER_EVENT, LOGCOMMAND_INDEX_SERVER_EVENT_DAILY_BUF, 
													0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
													pclServerEventManager->GetBonusBufType(), 0, 0, 0, 0, NULL, NULL);

					// [�α��߰� : Ȳ���� 2008. 7. 7] ���� �̺�Ʈ
					// param1 = ���� ��ü ����.
					SI32 Count = 0;
					for(	; Count < SERVER_MAX_NUM;  ++Count)
					{
						if(pclServerEventManager->GetServerUnique(Count) == pclClient->siServerUnique)
							break;
					}
					pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SERVER_EVENT, LOGCOMMAND_INDEX_SERVER_EVENT_TOTAL_RANK, 
													0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
													Count+1, 0, 0, 0, 0, NULL, NULL);

					// [�α��߰� : Ȳ���� 2008. 7. 7] ���� �̺�Ʈ
					// param1 = ���� �Ϸ� ����.
					Count = 0;
					for(	; Count < SERVER_MAX_NUM;  ++Count)
					{
						if(pclServerEventManager->GetBackUpDailyServerUniqueRank(Count) == pclClient->siServerUnique)
							break;
					}
					pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SERVER_EVENT, LOGCOMMAND_INDEX_SERVER_EVENT_DAILY_RANK, 
													0, NULL, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
													Count+1, 0, 0, 0, 0, NULL, NULL);
				}
				
				// ������ ������ ���Ϸ� ���.
				pclClient->pclLog->FilePrint( "config\\ServerEvent.log" , "[Daily Buf Type] BufType = %d", pclServerEventManager->GetBonusBufType()); 
			}
			break;
		case SERVER_EVENT_REASON_INFO_REQUEST://���� �̺�Ʈ�� ��� ���� ��û.
			{
				
			}
			break;
		case SERVER_EVENT_REASON_END://���� �̺�Ʈ ����.
			{
				// Ŭ���̾�Ʈ�� 1���� �Ǿ����� �˸�.
				cltServerResponseValue clreturn(0, SRVAL_SERVER_EVENT_RANK_1, pclServerEventManager->GetServerUnique(0), 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clreturn);
				pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
			}
			break;
	}
	

	cltGameMsgResponse_Server_Event_Rank_Info cltInfo;
	memcpy(cltInfo.m_clServerEvent_Rank_Record.GetServerRankInfo(0),		&pclMsg->m_clServerRankInfo,	  sizeof(CNServerRankInfo) * SERVER_MAX_NUM);
	memcpy(cltInfo.m_clServerEvent_Rank_Record.GetServerDailyRankInfo(0),	&pclMsg->m_clServerDailyRankInfo, sizeof(CNServerDailyRankInfo) * SERVER_MAX_NUM);
	memcpy(cltInfo.m_clServerEvent_Rank_Record.GetPersonalRankInfo(0),		&pclMsg->m_clPersonalRankInfo,	  sizeof(CNPersonalRankInfo) * USER_MAX_NUM);
	cltInfo.m_ServerEventBufKind = pclServerEventManager->GetBonusBufType();


	cltMsg clMsg(GAMEMSG_RESPONSE_SERVER_EVENT_RANK_INFO, sizeof(cltInfo), (BYTE*)&cltInfo);
	pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
}
void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_SERVER_INFO(sPacketHeader *pPacket)
{
	/*sDBResponse_Server_Event_Server_Info *pclMsg = (sDBResponse_Server_Event_Server_Info*)pPacket;

	if(0 > pclMsg->m_siResult)
		return;

	pclServerEventManager->SetServerRank(pclMsg->m_clServerRankInfo);*/
}
void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_DAILY_SERVER_INFO(sPacketHeader *pPacket)
{
	/*sDBResponse_Server_Event_Daily_Server_Info *pclMsg = (sDBResponse_Server_Event_Daily_Server_Info*)pPacket;

	if(0 > pclMsg->m_siResult)
		return;

	pclServerEventManager->SetServerDailyRank(pclMsg->m_clServerRankInfo);*/
}
void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_PERSONAL_INFO(sPacketHeader *pPacket)
{
	/*sDBResponse_Server_Event_Personal_Info *pclMsg = (sDBResponse_Server_Event_Personal_Info*)pPacket;

	if(0 > pclMsg->m_siResult)
		return;

	pclServerEventManager->SetPersonalRank(pclMsg->m_clPersonalRankInfo);*/
}

void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_SERVER_BUF_KIND(sPacketHeader *pPacket)
{
	sDBResponse_Server_Event_Server_Buf_Kind *pclMsg = (sDBResponse_Server_Event_Server_Buf_Kind*)pPacket;

	if(0 > pclMsg->m_siResult)
		return;

	pclServerEventManager->SetBonusBufType(pclMsg->m_siServerBufKind);

	// �Ϸ� ���� ������ ����ϴ� ������ �˷���.
	cltServerResponseValue clBufreturn(0, SRVAL_SERVER_EVENT_BUF, pclServerEventManager->GetBonusBufType(), 0);
	cltMsg clBuMsg(GAMEMSG_RESPONSE_SERVERRTN, sizeof(cltServerResponseValue), (BYTE*)&clBufreturn);
	pclCM->SendAllPersonMsg((sPacketHeader*)&clBuMsg);
}



// [����] ���� �̺�Ʈ ����Ʈ ���� ���� ���
void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_QUEST_INFO_SET( sPacketHeader *pPacket )
{
	if ( NULL == pPacket )
	{
		return;
	}
	
	sDBResponse_Server_Event_Server_Quest_Info_Set* pclMsg = (sDBResponse_Server_Event_Server_Quest_Info_Set*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

}

// [����] ���� �̺�Ʈ ����Ʈ ���� ��û ���
void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_QUEST_INFO_GET( sPacketHeader *pPacket )
{
	if ( NULL ==pPacket )
	{
		return;
	}

	sDBResponse_Server_Event_Server_Quest_Info_Get* pclMsg = (sDBResponse_Server_Event_Server_Quest_Info_Get*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	CNServerEvent_Quest* pclSE_Quest = (CNServerEvent_Quest*)pclServerEventManager->GetServerEvent_Quest();
	if ( NULL == pclSE_Quest )
	{
		return;
	}

	// ���� ����ũ�� ���� ����Ʈ �����͸� �д´�(���� ���� UINQUE�� �������)
	pclSE_Quest->LoadQuestData( pclClient->siServerUnique );

	// 
	CNServerEvent_QuestData clSE_QuestData;

	clSE_QuestData.SetServerEvent_QuestData( 
		pclMsg->m_siEventType,
		pclMsg->m_siCondition1, 
		pclMsg->m_siCondition2, 
		pclMsg->m_siNeedClearAmount,
		pclMsg->m_siRewardItemUnique, 
		pclMsg->m_siRewardItemAmount, 
		pclMsg->m_siLimitMinute
	);

	pclSE_Quest->GetServerEnent_QuestData()->SetServerEvent_QuestData( &clSE_QuestData );
	pclSE_Quest->SetCharacterIndex(		pclMsg->m_siCharacterIndex );
	pclSE_Quest->SetCharacter(			pclMsg->m_bCharacter );
	pclSE_Quest->SetInFailCharacter(	pclMsg->m_siInFailCharacter );
	pclSE_Quest->SetNowAmount(			pclMsg->m_siNowAmount );
	// �̺�Ʈ ���� �ð��� �����Ѵ�
	pclSE_Quest->SetServerEventQuestColck( pclClient->CurrentClock );
	// ��� ������ ������ �������Ƿ� ����Ʈ ��ŸƮ ���·� ����
	pclServerEventManager->SetServerEventState( SERVER_EVENT_STATE_START_QUEST );
}

// [����] ���� �̺�Ʈ ���� ���� ���� ��û ���
void cltServer::DoMsg_DBMSG_RESPONSE_SERVER_EVENT_PERSON_SCORE_GET( sPacketHeader *pPacket )
{
	if ( NULL ==pPacket )
	{
		return;
	}

	sDBResponse_Server_Event_Server_Person_Score_Get* pclMsg = (sDBResponse_Server_Event_Server_Person_Score_Get*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	SI32 siID		= pclMsg->m_uiCharID;
	SI32 siPersonID	= pclMsg->m_siPersonID;
	SI64 siScore	= pclMsg->m_siScore;

	// ��ȿ�� ���̵� �ΰ�?
	if ( false == pclCM->IsValidID(siID) )
	{
		return;
	}
	
	// ĳ���� ��������
	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[ siID ];
	if ( NULL == pclchar )
	{
		return;
	}
	
	// Ȥ�� Person ID�� �ٸ��ٸ�
	if ( siPersonID != pclchar->pclCI->GetPersonID() )
	{
		return;
	}

	// ���� ���� �Է�
	pclchar->siServerEvent_IndunScore = siScore;
}
void cltServer::DoMsg_DBMSG_RESPONSE_QUESTION_DAILY_COUNT_DEL(sPacketHeader* pPacket)
{
	sDBResponse_Question_Daily_Count_Del*	pclMsg = (sDBResponse_Question_Daily_Count_Del*)pPacket;
	
	for( SI32 Count = 0; Count < MAX_PERSON_NUMBER; ++Count )
	{
		cltCharServer* pclchar = pclCM->GetCharServer( Count );
		
		if(pclchar) 
			pclchar->m_uiDailyQuestionCount = 0;
	}	
}

void cltServer::DoMsg_DBMSG_RESPONSE_QUESTION_DAILY_COUNT_SET( sPacketHeader *pPacket )
{
	sDBResponse_Question_Daily_Count_Set* pclMsg = (sDBResponse_Question_Daily_Count_Set*)pPacket;
	if ( NULL == pclMsg )	return;

	cltCharServer* pclChar = pclCM->GetCharServer( pclMsg->m_siCharID );	
	if( NULL == pclChar ) return;

	pclChar->m_uiDailyQuestionCount = pclMsg->m_DailyCount;
}

void cltServer::DoMsg_DBMSG_RESPONSE_QUESTION_DAILY_COUNT_GET( sPacketHeader *pPacket )
{
	sDBResponse_Question_Daily_Count_Get* pclMsg = (sDBResponse_Question_Daily_Count_Get*)pPacket;
	if ( NULL == pclMsg )	return;

	cltCharServer* pclChar = pclCM->GetCharServer( pclMsg->m_siCharID );	
	if( NULL == pclChar ) return;

	pclChar->m_uiDailyQuestionCount = pclMsg->m_DailyCount;
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETMUTETIME(sPacketHeader* pPacket)
{
	sDBResponse_SetMuteTime* pclMsg = (sDBResponse_SetMuteTime*)pPacket;
	if( NULL == pclMsg )			return;

	if( pclMsg->m_siResult != 1 )	return;

	SI32 siCharID = pclCM->GetIDFromPersonID( pclMsg->m_siTargetPersonID );
	if ( false == pclCM->IsValidID(siCharID) )
	{
		siCharID = 0;
	}

	cltCharServer* pclTargetChar = NULL;
	cltCharServer* pclMasterChar = NULL;

	if( pclCM->IsValidID(siCharID) )			pclTargetChar = pclCM->GetCharServer( siCharID );
	if( pclCM->IsValidID(pclMsg->m_uiCharID) )	pclMasterChar = pclCM->GetCharServer( pclMsg->m_uiCharID );

	// �����Ϳ��� �޽����� �����Ѵ�.
	if( pclMasterChar && pclTargetChar )
	{
		cltGameResponse_SetMute clMsgMuteMaster(pclTargetChar->GetName(), &pclMsg->m_stMuteTime, true);
		cltMsg clMsgMaster(GAMEMSG_RESPONSE_SETMUTE, sizeof(cltGameResponse_SetMute), (BYTE*)&clMsgMuteMaster);
		pclMasterChar->SendNetMsg((sPacketHeader*)&clMsgMaster);
	}

	// ä�ú����� �ɸ� �������� ����� �˸���.
	if( pclTargetChar )
	{
		memcpy(&pclTargetChar->m_stMuteTime, &pclMsg->m_stMuteTime, sizeof(pclTargetChar->m_stMuteTime));

		cltGameResponse_SetMute clMsgMutePlayer(pclTargetChar->GetName(), &pclMsg->m_stMuteTime, false);
		cltMsg clMsgPlayer(GAMEMSG_RESPONSE_SETMUTE, sizeof(cltGameResponse_SetMute), (BYTE*)&clMsgMutePlayer);
		pclTargetChar->SendNetMsg((sPacketHeader*)&clMsgPlayer);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETMUTETIME(sPacketHeader* pPacket)
{
	sDBResponse_GetMuteTime* pclMsg = (sDBResponse_GetMuteTime*)pPacket;
	if( NULL == pclMsg )					return;

	if( pclMsg->m_siResult != 1 )			return;

	// ĳ���� Ȯ��
	SI32 siCharID = pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	if ( false == pclCM->IsValidID(siCharID) )
	{
		siCharID = 0;
	}
	if( pclMsg->m_uiCharID != siCharID )	return;

	cltCharServer* pclchar = pclCM->GetCharServer( siCharID );
	memcpy(&pclchar->m_stMuteTime, &pclMsg->m_stMuteTime, sizeof(pclchar->m_stMuteTime));

	// 9999�⵵�� �ʱ�ȭ�� ���̴�.
	if( pclchar->m_stMuteTime.wYear == 9999 )	{	ZeroMemory(&pclchar->m_stMuteTime, sizeof(pclchar->m_stMuteTime));	}

	// ä�� ���� �ð��� ������ ä�� ������ �����ȴ�.
	if( pclchar->m_stMuteTime.wYear > 0 && TimeCompare(&sTime, &pclchar->m_stMuteTime) > 0 )
	{
		ZeroMemory(&pclchar->m_stMuteTime, sizeof(pclchar->m_stMuteTime));
	}

	if( pclchar->m_stMuteTime.wYear > 0 )
	{
		cltGameResponse_SetMute clMsgMutePlayer(pclchar->GetName(), &pclchar->m_stMuteTime, false);
		cltMsg clMsgPlayer(GAMEMSG_RESPONSE_SETMUTE, sizeof(cltGameResponse_SetMute), (BYTE*)&clMsgMutePlayer);
		pclchar->SendNetMsg((sPacketHeader*)&clMsgPlayer);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_ELIXIR_GET(sPacketHeader* pPacket)
{
	sDBResponse_Elixir_Get* pclMsg = (sDBResponse_Elixir_Get*)pPacket;
	if( NULL == pclMsg )							return;

	// ĳ���� Ȯ��
	SI32 siCharID = pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	if ( false == pclCM->IsValidID( siCharID ) )	return;

	if( pclMsg->m_uiCharID != siCharID )			return;

	cltCharServer* pclchar = pclCM->GetCharServer( siCharID );
	if ( NULL == pclchar )							return;

	// ������ ĳ������ ������ ���� ����.
	pclchar->pcltElixir->SetEndMinuteVary	(  pclMsg->m_clElixirInfo.m_siEndMinuteVary	);	
	pclchar->pcltElixir->SetMultiple		(  pclMsg->m_clElixirInfo.m_uiMultiple		);
	pclchar->pcltElixir->SetPoint			(  pclMsg->m_clElixirInfo.m_siPoint			);		
	pclchar->pcltElixir->SetStr				(  pclMsg->m_clElixirInfo.m_siStr			);		
	pclchar->pcltElixir->SetDex				(  pclMsg->m_clElixirInfo.m_siDex			);		
	pclchar->pcltElixir->SetVit				(  pclMsg->m_clElixirInfo.m_siVit			);		
	pclchar->pcltElixir->SetMag				(  pclMsg->m_clElixirInfo.m_siMag			);		
	pclchar->pcltElixir->SetHnd				(  pclMsg->m_clElixirInfo.m_siHnd			);		
	pclchar->pcltElixir->SetWis				(  pclMsg->m_clElixirInfo.m_siWis			);		

	pclchar->bElixirSwitch = pclchar->pcltElixir->IsElixir();

	pclchar->SetParametaBoxAction( true );

	cltGameMsgResponse_Elixir_Set clInfo( pclchar->GetCharUnique(), &pclMsg->m_clElixirInfo );
	cltMsg clMsg(GAMEMSG_RESPONSE_ELIXIR_SET, sizeof(cltGameMsgResponse_Elixir_Set), (BYTE*)&clInfo);
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
}



void cltServer::DoMsg_DBMSG_RESPONSE_ELIXIR_SET(sPacketHeader* pPacket)
{
	sDBResponse_Elixir_Set* pclMsg = (sDBResponse_Elixir_Set*)pPacket;
	if( NULL == pclMsg )							return;

	if( pclMsg->m_siResult != 1 )					return;

	cltCharServer* pclchar = pclCM->GetCharServer( pclMsg->m_uiCharID );
	if( NULL == pclchar )										return;
	if( pclchar->pclCI->GetPersonID() != pclMsg->m_siPersonID ) return;

	// ������ ĳ������ ������ ���� ����.
	pclchar->pcltElixir->SetEndMinuteVary	(  pclMsg->m_clElixirInfo.m_siEndMinuteVary	);	
	pclchar->pcltElixir->SetMultiple		(  pclMsg->m_clElixirInfo.m_uiMultiple		);
	pclchar->pcltElixir->SetPoint			(  pclMsg->m_clElixirInfo.m_siPoint			);		
	pclchar->pcltElixir->SetStr				(  pclMsg->m_clElixirInfo.m_siStr			);		
	pclchar->pcltElixir->SetDex				(  pclMsg->m_clElixirInfo.m_siDex			);		
	pclchar->pcltElixir->SetVit				(  pclMsg->m_clElixirInfo.m_siVit			);		
	pclchar->pcltElixir->SetMag				(  pclMsg->m_clElixirInfo.m_siMag			);		
	pclchar->pcltElixir->SetHnd				(  pclMsg->m_clElixirInfo.m_siHnd			);		
	pclchar->pcltElixir->SetWis				(  pclMsg->m_clElixirInfo.m_siWis			);		

	pclchar->bElixirSwitch = pclchar->pcltElixir->IsElixir();

	pclchar->SetParametaBoxAction( true );

	cltGameMsgResponse_Elixir_Set clInfo( pclchar->GetCharUnique(), &pclMsg->m_clElixirInfo );
	cltMsg clMsg(GAMEMSG_RESPONSE_ELIXIR_SET, sizeof(cltGameMsgResponse_Elixir_Set), (BYTE*)&clInfo);
	pclchar->SendNeighbourMsg( &clMsg, true );

	// �Ҹ��ϴ� �������� ������ �Ҹ��Ų��.
	if( pclMsg->m_bUseItem )
	{
		if( 0 >= pclMsg->m_siElixirItemUnique )	return;	
		if( 0 >= pclMsg->m_siWaterItemUnique  )	return;	

		// �ش� ��ġ�� ������ ����ũ Ȯ��.
		if( pclMsg->m_siElixirItemUnique != pclchar->pclCI->clCharItem.clItem[pclMsg->m_siElixirItemPos].siUnique ) return;
		if( pclMsg->m_siWaterItemUnique  != pclchar->pclCI->clCharItem.clItem[pclMsg->m_siWaterItemPos].siUnique  ) return;

		// ������ ��� ��뷮�� Ŭ��� ���Ѵ�.
		SI32 siUseWaterNum = min( 10000, pclMsg->m_clElixirInfo.m_uiMultiple * pclchar->pclCI->clIP.GetLevel() * cltElixir::WATER_MILTIPLE );
		if( pclMsg->m_siWaterItemNum != siUseWaterNum ) return;

		cltItem clUseItem;
		clUseItem.Set( &pclchar->pclCI->clCharItem.clItem[pclMsg->m_siElixirItemPos] );
		clUseItem.siItemNum = 1;

		// ������ ���۽� ����ϴ� ������ ����.(������)
		sDBRequest_UseItem useitem1(	pclMsg->m_uiCharID,
										pclchar->pclCI->GetPersonID(),
										pclMsg->m_siElixirItemPos,
										&clUseItem, 
										USEITEM_REASON_ELIXIR_START_USE_ITEM	);

		pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem1);


		// ������ ���۽� ����ϴ� ������ ����.(������ ���)
		cltItem clUseItem2;
		clUseItem2.Set( &pclchar->pclCI->clCharItem.clItem[pclMsg->m_siWaterItemPos] );
		clUseItem2.siItemNum = pclMsg->m_siWaterItemNum;

		sDBRequest_UseItem useitem2(	pclMsg->m_uiCharID,
										pclchar->pclCI->GetPersonID(),
										pclMsg->m_siWaterItemPos,
										&clUseItem2, 
										USEITEM_REASON_ELIXIR_START_USE_ITEM	);

		pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem2);

		pclClient->pclLog->FilePrint( "config\\Elixir.log" , "[Elixir] : %d	[������ ���] : %d	[���] : %d",  
																clUseItem.siItemNum,
																clUseItem2.siItemNum,
																pclMsg->m_clElixirInfo.m_uiMultiple	);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_PROMOTER_INFO_GET(sPacketHeader* pPacket)
{
	sDBResponse_Promoter_Info_Get* pclMsg = (sDBResponse_Promoter_Info_Get*)pPacket;
	if( NULL == pclMsg )			return;

	if( pclMsg->m_siResult != 1 )	return;

	cltCharServer* pclChar = pclCM->GetCharServer( pclMsg->m_uiCharID );
	pclChar->pclCI->clBI.clRecommenderInfo.siPersonID = pclMsg->m_siPersonID;
		
	cltCharServer* pclchar = pclCM->GetCharServer( pclMsg->m_uiCharID );

	if( pclMsg->m_siPersonID )
	{
		bool bResult = true;
		// ������ ��Ͽ� �ִ��� üũ
		cltSimplePerson* pSimplePersonInfo = NULL;
		for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
		{
			pSimplePersonInfo = &pclchar->pclFriendInfo->clFriend[i];

			if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
			{
				if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclMsg->m_szPromoterName ) == 0 )
				{
					// ģ�� ��Ͽ� �����Ƿ� �߰��� �ʿ� ����.
					bResult = false;
					break;
				}
			}
		}

		if( bResult )
		{
			sDBRequest_FriendAddList clMsg(	0, pclMsg->m_siPersonID, 0, pclMsg->m_szPromoterName, 
				0, pclchar->pclCI->GetPersonID(), (SI32)pclMsg->m_uiCharID, (TCHAR*)pclchar->GetName() );

			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}
	}
}



//	���⼭ ���� �۾�
void cltServer::DoMsg_DBMSG_RESPONSE_SETBALANCECANUSECURRENCY(sPacketHeader *pPacket)
{
	sDBResponse_SetBalanceCanUseCurrency *pclMsg = (sDBResponse_SetBalanceCanUseCurrency*)pPacket;
	if(pclMsg == NULL) 
		return;


	m_pclBalanceCurrencyMgr->SetCanUserMoney(pclMsg->m_siMoney);

	if(pclCM->IsValidID(pclMsg->m_siCharID)	== false)return ;
	cltCharServer *pclchar = (cltCharServer*)pclCM->CR[pclMsg->m_siCharID];

	cltGameMsgResponse_BalanceCanUseCurrency cMsgBalanceCanUseCurrency(pclMsg->m_siMoney);
	cltMsg clMsg(GAMEMSG_RESPONSE_GETBALANCECANUSECURRENCY, sizeof(cltGameMsgResponse_BalanceCanUseCurrency), (BYTE*)&cMsgBalanceCanUseCurrency);

	pclchar->SendNetMsg((sPacketHeader*)&clMsg);
}


void cltServer::DoMsg_DBMSG_RESPONSE_GETBALANCECANUSECURRENCY(sPacketHeader *pPacket)
{
	sDBResponse_GetBalanceCanUseCurrency *pclMsg = (sDBResponse_GetBalanceCanUseCurrency*)pPacket;
	
	if(pclMsg == NULL) 
		return;

	if(pclMsg->m_siResult != 1)
		return;

	m_pclBalanceCurrencyMgr->SetCanUserMoney(pclMsg->m_siMoney);

	if(pclMsg->m_siCharID > 0 && pclMsg->m_bCalledGM == true)
	{
		if(pclCM->IsValidID(pclMsg->m_siCharID)	== false)return ;
		if(pclCM->CR[pclMsg->m_siCharID]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL)return ;


		//	GM�� �� ��ȭ�� ��ȸ�� ��û�����Ƿ� client���� �˷��ش�.
		cltGameMsgResponse_BalanceCanUseCurrency cMsgBalanceCanUseCurrency(pclMsg->m_siMoney, true);
		cltMsg clMsg(GAMEMSG_RESPONSE_GETBALANCECANUSECURRENCY, sizeof(cltGameMsgResponse_BalanceCanUseCurrency), (BYTE*)&cMsgBalanceCanUseCurrency);

		pclCM->CR[pclMsg->m_siCharID]->SendNetMsg((sPacketHeader*)&clMsg);

		//pclGameDBProcess->SendMsg((sPacketHeader*)&clMsg);
		
	}
}
// [����] �����̺�Ʈ ���� �̺�Ʈ�� ����
void cltServer::DoMsg_DBMSG_RESPONSE_GOONZURACCOONEVENT_SETINFO(sPacketHeader *pPacket)
{
	sDBResponse_GoonzuRaccoonEvent_SaveInfo *pclMsg = (sDBResponse_GoonzuRaccoonEvent_SaveInfo*)pPacket;

	if(pclMsg == NULL)				return;

	if(pclMsg->m_siResult != 1)		return;
	// ���� Ư���� ���� ���Ѵ�
}
void cltServer::DoMsg_DBMSG_RESPONSE_GOONZURACCOONEVENT_GETINFO(sPacketHeader *pPacket)
{
	sDBResponse_GoonzuRaccoonEvent_GetInfo *pclMsg = (sDBResponse_GoonzuRaccoonEvent_GetInfo*)pPacket;

	if(pclMsg == NULL)				return;
	// �����̺�Ʈ�� ���� �Ѵ�.
	SI32 siSetCount =	0	;
	for (SI32 i = 0 ; i < MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT; i++ ) 
	{
		if (  i >= MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT) break;

		SI32 siTime = pclMsg->m_GoonzuRaccoonEventData[i].GetTime()	;
		SI32 siMonsterNum = pclMsg->m_GoonzuRaccoonEventData[i].GetMonsterNum()	;
		if(siTime < 0 || siTime > MAX_HOUR_GOONZURACCOONEVENT )	continue	;
		if(siMonsterNum <= 0 )	continue	;
		pclGoonzuKingGuriEventManager->GoonzuRaccoonEventData[i].SetTime(siTime)	;
		pclGoonzuKingGuriEventManager->GoonzuRaccoonEventData[i].SetMakeMonsterNum(siMonsterNum)	;
		siSetCount	=	0	;
		for ( SI32 siFiledIndex = 0 ; siFiledIndex < MAX_SELECTRFIELD_NUM_GOONZURACCOONEVENT ;siFiledIndex++ ) 
		{
			siSetCount++;
			if(siMonsterNum < siSetCount)	continue	;

			SI32 siMapindex = pclMsg->m_GoonzuRaccoonEventData[i].GetRocIndex(siFiledIndex)	;
			if(siMapindex <= 0  )	continue	;
			SI32 siX = pclMsg->m_GoonzuRaccoonEventData[i].GetRocX(siFiledIndex)	;
			SI32 siY = pclMsg->m_GoonzuRaccoonEventData[i].GetRocY(siFiledIndex)	;
			if ( siX < 0 || siY < 0   )
			{
				siX	=	0	;
				siY	=	0	;
			}				
			pclGoonzuKingGuriEventManager->GoonzuRaccoonEventData[i].SetMapIndex( siFiledIndex,  siMapindex)	;
			pclGoonzuKingGuriEventManager->GoonzuRaccoonEventData[i].SetMapRocX(siFiledIndex, siX)	;
			pclGoonzuKingGuriEventManager->GoonzuRaccoonEventData[i].SetMapRocY(siFiledIndex, siY)	;
		}
	}
}