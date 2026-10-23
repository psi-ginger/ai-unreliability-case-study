//---------------------------------
// 2003/8/6 ���°�
//---------------------------------
// APEX- 
#include "../../Apex/ApexGoonZu.h"
// -APEX

#include "../../Common/CommonHeader.h"
#include "../../Server/Server.h"

#include "../../../NLib/NUtil.h"
#include "../../../NLib/NTCHARString.h"
#include "TraceBoot.h"

#include "../Common/Map/Map.h"
#include "../Common/Father/Father.h"
#include "../Common/VoteMgr/VoteMgr.h"
#include "../Common/Skill/Skill-Manager.h"
#include "Char/CharManager/CharManager.h"
#include "../Common/Order/Order.h"
#include "../common/GameEvent/GameEvent.h"
#include "../common/Item/ItemCommon/cltSetItem.h"

#include "../CommonLogic/Feast/Feast.h"

#include "Guild/Guild.h"

#include "MsgRval-Define.h"
#include "MsgType-Person.h"
#include "MsgType-System.h"

#include <stdio.h>
#include <stdarg.h>
static void WTrace( const char *pszFmt, ... )
{
	char szBuf[1024];
	va_list args;
	va_start( args, pszFmt );
	_vsnprintf( szBuf, sizeof(szBuf), pszFmt, args );
	va_end( args );
	szBuf[sizeof(szBuf)-1] = 0;
	FILE *fp = fopen( "C:\\Users\\Home\\Documents\\Online_work\\New Era Live\\WTrace_IP.log", "a" );
	if( fp ) { fprintf( fp, "%s\n", szBuf ); fclose( fp ); }
}
#include "MsgType-Horse.h"
#include "Msg/MsgType-Vote.h"
#include "Msg/MsgType-Friend.h"
#include "Msg/MsgType-Summon.h"
#include "Msg/MsgType-Village.h"
#include "Msg/MsgType-ItemMall.h"
#include "Msg/MsgType-npc.h"
#include "MsgType-Structure.h"
#include "Msg/MsgType-Web.h"
#include "Msg/MsgType-NamingQuest.h"


#include "MsgType-Item.h"
#include "MsgType-Guild.h"
#include "..\..\..\DBManager\GameDBManager_World\DBMsg-System.h"
#include "../../../DBManager/GameDBManager_World/DBMsg-Family.h"

//-----------------------------------------
// Server
//-----------------------------------------
#include "../Server/Statistics/Statistics.h"
#include "../Server/Minister/Minister-yezo.h"
#include "../Server/Minister/MinisterMgr.h"
#include "../Server/ServerReward/ServerReward.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Stock.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Summon.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Cityhall.h"
#include "../../DBManager/GameDBManager_World/DBMsg-item.h"
#include "../../DBManager/GameDBManager_World/DBMsg-PostOffice.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Agriculture.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Friend.h"
#include "../../DBManager/GameDBManager_World/DBMsg-PVP_League.h"
#include "../../DBManager/GameDBManager_World/DBMsg-SoulGuard.h"

#include "../../DBManager/GameDBManager_World/DBMsg-Person.h"

#include "../Notice/Notice.h"
#include "HashTable/HashTable_StringInt.h"
#include "HashTable/HashTable_IntInt.h"

#include "../../Server/AuthServer/AuthMsg.h"

#include "NDate.h"


#include "../../../NLib/NUtil.h"
#include "../../Lib/MD5/md5.h"

#include "../../CommonLogic/Msg/MsgType-HappyBeanQuest.h"

#include "../../common/VoteMgr/VoteBaseInfo.h"


// �������ͽ� �ʱ�ȭ 
void cltServer::DoMsg_DBMSG_RESPONSE_INITSTATUS(sPacketHeader* pPacket)
{
	sDBResponse_InitStatus* pclinfo = (sDBResponse_InitStatus*)pPacket;

	if(pclinfo->siResult == 0)
	{
		return ;
	}

	SI32 id = pclinfo->usCharID;
	if(id <= 0)return ;
	if(pclCM->IsValidID(id) == false)		return;

	// �������ͽ��� �ʱ�ȭ �Ѵ�. 
	pclCM->CR[id]->pclCI->clIP.SetStr(5);
	pclCM->CR[id]->pclCI->clIP.SetDex(5);
	pclCM->CR[id]->pclCI->clIP.SetVit(5);
	pclCM->CR[id]->pclCI->clIP.SetHnd(5);
	pclCM->CR[id]->pclCI->clIP.SetMag(5);
	pclCM->CR[id]->pclCI->clIP.SetLuk(5);
	pclCM->CR[id]->pclCI->clIP.SetWis(5);
	pclCM->CR[id]->pclCI->clIP.SetBonus( pclCM->CR[id]->pclCI->clIP.GetLevel() * 5);

	pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);

	// ĳ���͸� �����ϰ� �溹������ ������Ų��. 
	OrderStop(id, BY_COMPUTER);

	//if ( siServiceArea == ConstServiceArea_China )
	//{
	//	cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);
	//	pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 1900);
	//}
	//else
	//{
	//	if ( pclCM->CR[id]->pclKI->siDefaultCountry == ConstServiceArea_China )
	//	{
	//		cltWarpInfo clWarpInfo(1, MAPINDEX_CHINABORDER, false, false, 17, 19, -1, -1);
	//		pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 1900);
	//	}
	//	else
	//	{
			cltWarpInfo clWarpInfo(1, MAPINDEX_PALACE, false, false, 27, 87, -1, -1);
			pclCM->CR[id]->SetWarpInfo(&clWarpInfo, 1900);
		//}
	//}
	
	// �̿���� �̿��ؼ� �ʱ�ȭ�� ���, 
	if( pclinfo->siMode == INITSTATUSMODE_TICKET || 
		pclinfo->siMode == INITSTATUSMODE_CHINA_EVENTTICKET )
	{
		pclCM->CR[id]->pclCI->clCharItem.clItem[ pclinfo->siItemPos].Set(&pclinfo->clItem);

		// Ŭ���̾�Ʈ�� ������Ʈ �� ������ ������. 
		cltGameMsgResponse_PersonItemInfo clinfo(pclinfo->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclinfo->clItem, 0);
		cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		// ��踦 ����. 
		
		if( INITSTATUSMODE_CHINA_EVENTTICKET == pclinfo->siMode )
			pclItemManager->AddUseNum(ITEMUNIQUE(24031), 1);

			// ���ο� ���
			//NStatistics_Add("Item", ITEMUNIQUE(24031), "UseNum", 1);

		else
			pclItemManager->AddUseNum(ITEMUNIQUE(24030), 1);

			// ���ο� ���
			//NStatistics_Add("Item", ITEMUNIQUE(24030), "UseNum", 1);
	}

	cltServer* pclserver = (cltServer*)pclClient;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// PCK - �����ۻ���� �Ҷ� ������ ��� �α׵� ���� �����ش�.
		SendLog_WriteLogToDB(	LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
										USEITEM_REASON_INITSTATUSMODE_TICKET, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclinfo->clItem_Log, pclinfo->clItem.GetItemNum(), 
										0, 0, 0, 0, 0, 
										pclinfo->siItemPos, 0, 0, 0, 0, NULL, NULL);

		// [�α��߰� : Ȳ���� 2007. 10. 15] // ���� �ʱ�ȭ. (�ɷ��� ��� 5�� �ʱ�ȭ �Ҷ�.)
		// parma1 == ������ ����ũ
		SendLog_WriteLogToDB(	LOGCOMMAND_TYPE_INPORTANTPARAMEATA, LOGCOMMAND_INDEX_INPORTANTPARAMEATA_INITSTATUS,
								pclinfo->siMode, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
								pclinfo->clItem_Log.siUnique, 0, 0, 0, 0, NULL, NULL);
	}

	// Ŭ���̾�Ʈ�� ����� �˸���. 
	SendServerResponseMsg(0, SRVAL_INITSTATUS,  pclinfo->siMode, 0, pclCM->CR[id]->GetCharUnique());
	
}

// ���� ����Ʈ �뺸. 
void cltServer::DoMsg_DBMSG_RESPONSE_GETCHILDLIST(sPacketHeader* pPacket)
{
	sDBResponse_GetChildList* pclinfo = (sDBResponse_GetChildList*)pPacket;

	if(pclinfo->siResult == 0)
	{
		return ;
	}
	
	SI32 id = pclinfo->usCharID;
	if(id <= 0)return ;
	

	// Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
	cltGameMsgResponse_UpdateChildList clinfo(pclinfo->usCharID, pclinfo->clChildInfo);
	cltMsg clMsg(GAMEMSG_RESPONSE_UPDATECHILDLIST, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}

// ���� �缺 ������ ����  �뺸. 
void cltServer::DoMsg_DBMSG_RESPONSE_INCREASECHILDNUMBER(sPacketHeader* pPacket)
{
	sDBResponse_IncreaseChildNumber* pclinfo = (sDBResponse_IncreaseChildNumber*)pPacket;

	if(pclinfo->siResult == 0)
	{
		return ;
	}

	SI32 id = pclCM->GetIDFromPersonID(pclinfo->siPersonID);
	if(pclCM->IsValidID(id))
	{
		// Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
		cltGameMsgResponse_ChildInfo clinfo(id, pclinfo->siChildNumber);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHILDINFO, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

// Person ���� ���� ��û. 
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONSIMPLEINFO(sPacketHeader* pPacket)
{
	sDBResponse_PersonSimpleInfo* pclinfo = (sDBResponse_PersonSimpleInfo*)pPacket;

	if(pclinfo->siResult == 0)
	{
		return ;
	}

	SI32 id = pclinfo->usCharID;

	if( false == pclCM->IsValidID(id) )
		return;

	// ���� ������ ���� ���̶��, 
	if(pclinfo->siReason == PERSONSIMPLEINFO_REASON_FATHERREWARD)
	{

		//KHY - 0611 - �������� S �߰�����.
		if ( ConstServiceArea_Japan != pclClient->siServiceArea )
		{
			// ��� ��Ų ���� 
			SI32 inclevel = max(0, NEWFATHER_LEVEL - pclCM->CR[id]->pclCI->clBI.clFatherInfo.siMyLevelForFather);
			GMONEY money		= pclinfo->siLevel * min(pclItemManager->clItemStatistic.siNPCPriceRate * 100,	(inclevel * 200));
		
			SI32 teacherID = pclCM->GetIDFromPersonID(pclCM->CR[id]->pclCI->clBI.clFatherInfo.clFather.GetPersonID());
			// ������ ������� �Ƿι��� �ý��� ����
			if ( pclCM->IsValidID( teacherID) )
			{
				money = ((cltCharServer*)pclCM->CR[teacherID])->CalcTiredPoint( money );
			}

			// ������ DB�� Ŭ���̾�Ʈ���� ������. 
			cltLetterMsg_ChildLevelUp clletter( inclevel, (TCHAR*)pclCM->CR[id]->GetName(), money, 0, cltLetterMsg_ChildLevelUp::FATHERREWARD );
			SendLetterMsg(pclinfo->siPersonID, (cltLetterHeader*)&clletter);

			// ���¿���  ���� ������ ��û. 
			{
				sDBRequest_ChangeMoney clMsg( teacherID, pclinfo->siPersonID, CHANGE_MONEY_REASON_FATHERREWARD, money );
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);


				// ��ȭ���� �����Ѵ�. 
				cltMoney clmoney;
				clmoney.Set(money);
				clCurrencyInfo.clInputMoney_Father.Increase(&clmoney);

				pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_FATHER, money);
			}

			money = ( inclevel * 100 ) * NEWFATHER_LEVEL;

			// �Ƿι��� �ý��� ����
			GMONEY givemoney = ((cltCharServer*)pclCM->CR[id])->CalcTiredPoint(money);

			cltLetterMsg_ChildLevelUp clletter3( inclevel, (TCHAR*)pclCM->CR[id]->GetName(), givemoney, 0, cltLetterMsg_ChildLevelUp::PUPILREWARD );
			SendLetterMsg(pclCM->CR[id]->pclCI->GetPersonID(), (cltLetterHeader*)&clletter3);
			// ���ڿ���  ���� ������ ��û. 
			{

				sDBRequest_ChangeMoney clMsg( id, pclCM->CR[id]->pclCI->GetPersonID(), CHANGE_MONEY_REASON_PUPILREWARD, givemoney );
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				// ��ȭ���� �����Ѵ�. 
				cltMoney clmoney;
				clmoney.Set(givemoney);
				clCurrencyInfo.clInputMoney_Father.Increase(&clmoney);

				pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_FATHER, givemoney);
			}

			SI32 grandTeacherID = pclCM->GetIDFromPersonID( pclinfo->siGrandPersonID );
			// ������ ������ 10%�� �޴´�.
			givemoney = money / 10;

			// ������ ������� �Ƿι��� �ý��� ����
			if ( pclCM->IsValidID( grandTeacherID) )
			{
				givemoney = ((cltCharServer*)pclCM->CR[grandTeacherID])->CalcTiredPoint( givemoney );
			}
			
			cltLetterMsg_ChildLevelUp clletter2( inclevel, (TCHAR*)pclCM->CR[id]->GetName(), givemoney, 0, cltLetterMsg_ChildLevelUp::GRANDFATHERREWARD );
			SendLetterMsg( pclinfo->siGrandPersonID, (cltLetterHeader*)&clletter2);

			// ������ ���¿���  ���� ������ ��û. 
			{
				sDBRequest_ChangeMoney clMsg( grandTeacherID, pclinfo->siGrandPersonID, CHANGE_MONEY_REASON_GRANDFATHERREWARD, givemoney );
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				// ��ȭ���� �����Ѵ�. 
				cltMoney clmoney;
				clmoney.Set(givemoney);
				clCurrencyInfo.clInputMoney_Father.Increase(&clmoney);

				pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_FATHER, givemoney);
			}
		}
		else
		{
			// ��� ��Ų ���� 
			SI32 inclevel = max(0, FATHER_LEVEL - pclCM->CR[id]->pclCI->clBI.clFatherInfo.siMyLevelForFather);

			SI32 money		= pclinfo->siLevel * min(2000,	(inclevel * 200));
			//SI32 exp		= pclinfo->siLevel * min(100,	(inclevel * 10));
			SI32 fameexp	= max(5, inclevel * 3);							// ���� ����ġ ȹ��. 

			// ������ DB�� Ŭ���̾�Ʈ���� ������. 
			cltLetterMsg_ChildLevelUp clletter(inclevel, (TCHAR*)pclCM->CR[id]->GetName(), money, fameexp);
			SendLetterMsg(pclinfo->siPersonID, (cltLetterHeader*)&clletter);

			// ���� ������ ��û. 
			{
				sDBRequest_ChangeMoney clMsg(id, pclinfo->siPersonID, CHANGE_MONEY_REASON_FATHERREWARD, money);
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

				// ��ȭ���� �����Ѵ�. 
				cltMoney clmoney;
				clmoney.Set(money);
				clCurrencyInfo.clInputMoney_Father.Increase(&clmoney);

				pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_FATHER, money);

			}

			// ����ġ ������ ��û. 
			// ������� ���� ���ο� ������� ����ġ�� �ش�.
			//pclCM->IncExp(pclinfo->siPersonID, exp, CHANGE_EXP_REASON_FATHER);

			// ���� ����ġ�� �� ���� ��û. 
			// ������� ���� ���ο� ������� ����ġ�� �ش�.
			pclCM->IncFameExp(pclinfo->siPersonID, fameexp, GETEXP_REASON_FAMEFATHER, CHANGE_FAMEEXP_REASON_FATHER);

			/*		// ���� �缺 ������ ��û. 
			{
			sDBRequest_IncreaseChildNumber clMsg(pclinfo->siPersonID, 1);
			SendDBMsg((sPacketHeader *)&clMsg);
			}
			*/		
		}
	}
	else if( pclinfo->siReason == PERSONSIMPLEINFO_REASON_PROMOTER_INFO )
	{
		cltCharServer* pclchar = pclCM->GetCharServer( pclinfo->usCharID );
		pclchar->pclCI->clBI.clRecommenderInfo.siPersonID = pclinfo->siPersonID;

		cltGameResponse_Promoter_Info_Get clinfo( pclinfo->siPersonID, pclinfo->siCharKind, pclinfo->siLevel, pclchar->bCellularPhoneAuth, pclinfo->szGetCharInfoName );
		cltMsg clMsg(GAMEMSG_RESPONSE_PROMOTER_INFO_GET, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		if( pclinfo->siPersonID )
		{
			bool bResult = true;
			// ������ ��Ͽ� �ִ��� üũ
			cltSimplePerson* pSimplePersonInfo = NULL;
			for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
			{
				pSimplePersonInfo = &pclchar->pclFriendInfo->clFriend[i];

				if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
				{
					if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclinfo->szGetCharInfoName ) == 0 )
					{
						// ģ�� ��Ͽ� �����Ƿ� �߰��� �ʿ� ����.
						bResult = false;
						break;
					}
				}
			}

			if( bResult )
			{
				sDBRequest_FriendAddList clMsg(	0, pclinfo->siPersonID, pclCM->GetIDFromPersonID(pclinfo->siPersonID), pclinfo->szGetCharInfoName, 
												0, pclchar->pclCI->GetPersonID(), (SI32)pclinfo->usCharID, (TCHAR*)pclchar->GetName() );

				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
		}
	}
	// ��õ���� ����õ�ο��� �λ縻.(��õ���� ������ �޾ƿԴ�.)
	else if( pclinfo->siReason == PERSONSIMPLEINFO_REASON_RECOMMEND_INTRODUCE )
	{
		bool bConnect = false;
		// ��õ���� ID
		SI32 siID = pclCM->GetIDFromName( pclinfo->szGetCharInfoName );
		cltCharServer* pclRecommendChar = pclCM->GetCharServer( siID );
		if( pclRecommendChar )
		{
			bConnect = true;
		}

		// ����õ�� ID
		siID = pclCM->GetIDFromName( pclinfo->szCharInfoName );
		cltCharServer* pclRecommendeeChar = pclCM->GetCharServer( siID );
		if( pclRecommendeeChar )
		{
			cltGameResponse_Recommend_Introduce clInfo( pclinfo->szGetCharInfoName, pclinfo->siCharKind, bConnect );
			cltMsg clMsg(GAMEMSG_RESPONSE_RECOMMEND_INTRODUCE, sizeof(clInfo), (BYTE*)&clInfo);
			pclRecommendeeChar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
	// ����õ���� ��õ�ο��� �λ縻.(��õ���� ������ �޾ƿԴ�.)
	else if( pclinfo->siReason == PERSONSIMPLEINFO_REASON_RECOMMENDEE_INTRODUCE )
	{
		bool bConnect = false;
		// ����õ���� ID
		SI32 siID = pclCM->GetIDFromName( pclinfo->szGetCharInfoName );
		cltCharServer* pclRecommendeeChar = pclCM->GetCharServer( siID );
		if( pclRecommendeeChar )
		{
			bConnect = true;
		}

		// ��õ�� ID
		siID = pclCM->GetIDFromName( pclinfo->szCharInfoName );
		cltCharServer* pclRecommendChar = pclCM->GetCharServer( siID );
		if( pclRecommendChar )
		{
			cltGameResponse_Recommendee_Introduce clInfo( pclinfo->szGetCharInfoName, pclinfo->siCharKind, bConnect );
			cltMsg clMsg(GAMEMSG_RESPONSE_RECOMMENDEE_INTRODUCE, sizeof(clInfo), (BYTE*)&clInfo);
			pclRecommendChar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

// ���� ����/���� 
void cltServer::DoMsg_DBMSG_RESPONSE_SETFATHER(sPacketHeader* pPacket)
{
	sDBResponse_SetFather* pclMsg = (sDBResponse_SetFather*)pPacket;

	SI32 id = pclMsg->usCharID;

	if( false == pclCM->IsValidID(id) )
		return;

	if(pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)return ;

	SI32 returnval = 0;

	switch(pclMsg->siResult)
	{
	case 0:
		break;
	case 1:
		{
			//---------------------------------
			// ������ �����Ѵ�. 
			//---------------------------------
			SI32 orgfatherid = pclCM->CR[id]->pclCI->clBI.clFatherInfo.clFather.siPersonID;
			SI32 fathercharid = pclCM->GetIDFromPersonID(pclMsg->siFatherPersonID);

			if ( pclCM->IsValidID(fathercharid) )
			{
				pclCM->CR[id]->pclCI->clBI.clFatherInfo.clFather.siPersonID	= pclMsg->siFatherPersonID;
				StringCchCopy(pclCM->CR[id]->pclCI->clBI.clFatherInfo.clFather.szName, MAX_PLAYER_NAME, pclMsg->szFatherName);
				pclCM->CR[id]->pclCI->clBI.clFatherInfo.siMyLevelForFather		= (SI08)pclMsg->siMyLevel;		// ���� ���� ����� ���� 
				pclCM->CR[id]->pclCI->clBI.clFatherInfo.siFatherChangeNumber	= (SI08)pclMsg->siChangedFatherNumber;
				StringCchCopy(pclCM->CR[id]->pclCI->clBI.clFatherInfo.szAccountID, MAX_PLAYER_NAME,pclCM->CR[fathercharid]->pclCI->clBI.szAccountID);

				//---------------------------------
				// Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
				//--------------------------------
				cltGameMsgResponse_SetFather clinfo(&pclCM->CR[id]->pclCI->clBI.clFatherInfo.clFather, pclCM->CR[id]->pclCI->clIP.GetLevel(),pclCM->CR[id]->pclCI->clBI.clFatherInfo.szAccountID);
				cltMsg clMsg(GAMEMSG_RESPONSE_SETFATHER, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				// ���������� ���ٸ� 
				if(!pclMsg->siPrevFatherPersonID)
				{
					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [�α��߰� : Ȳ���� 2007. 10. 9] // ���� ��� �α׾���. 
						// param1  == ���� ���� ����� �� ����, 
						SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FATHER, LOGCOMMAND_INDEX_FATHER_SETFATHER, 
											0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->siFatherPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclMsg->siMyLevel, 0, 0, 0, 0, NULL, NULL);
					}
				}

				//------------------------------------------
				// ���� ������ �ִٸ� ���� ������ ������. 
				//------------------------------------------
				if(pclMsg->siPrevFatherPersonID)
				{
					cltLetterMsg_Father clletter(false, (TCHAR*)pclCM->CR[id]->GetName());
					// ������ DB�� Ŭ���̾�Ʈ���� ������. 
					SendLetterMsg(pclMsg->siPrevFatherPersonID, (cltLetterHeader*)&clletter);

					if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
					{
						// [�α��߰� : Ȳ���� 2007. 10. 9] // ���� ���� �α� �ۼ�.
						// param1 == ������ ����� Ƚ��
						SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FATHER, LOGCOMMAND_INDEX_FATHER_RENOUNCEFATHER, 
											0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->siPrevFatherPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclMsg->siChangedFatherNumber, 0, 0, 0, 0, NULL, NULL);
					}
				}

				//-----------------------------------------
				// �Ӹ��� ���¿��� �Ӹ� ������ ������. 
				//-----------------------------------------
				if(orgfatherid != pclCM->CR[pclMsg->usCharID]->pclCI->clBI.clFatherInfo.clFather.siPersonID)
				{
					cltLetterMsg_Father clletter(true, (TCHAR*)pclCM->CR[id]->GetName());
					// ������ DB�� Ŭ���̾�Ʈ���� ������. 
					SendLetterMsg(pclMsg->siFatherPersonID, (cltLetterHeader*)&clletter);


					SI32 siFatherCharID = pclCM->GetIDFromPersonID( pclMsg->siFatherPersonID );

					if ( pclCM->IsValidID( siFatherCharID ) == true )
					{
						((cltCharServer*)pclCM->CR[siFatherCharID])->DoQuestCheckAndAction(QUEST_CLEAR_COND_TAKEPUPILS);
					}
				}


				// ����Ʈ üũ�Ѵ�.
				((cltCharServer*)pclCM->CR[id])->DoQuestCheckAndAction(QUEST_CLEAR_COND_SETFATHER);

				//KHY - 0611 - �������� S �߰�����.
				if ( ConstServiceArea_Japan != pclClient->siServiceArea )
				{
					// ���� ģ�� ������ֱ�
					sDBRequest_FriendAddList clMsg1( 0, pclMsg->siPersonID, id, (TCHAR*)pclCM->CR[id]->GetName(), 0, pclMsg->siFatherPersonID, fathercharid, pclMsg->szFatherName );
					pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg1);

					SendServerResponseMsg(0, SRVAL_ACCEPT_SETFATHER,  0, 0, pclCM->CR[id]->GetCharUnique());

					sDBRequest_FriendAddList clMsg2( 0, pclMsg->siFatherPersonID, fathercharid, pclMsg->szFatherName, 0, pclMsg->siPersonID, id, (TCHAR*)pclCM->CR[id]->GetName() );
					pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg2);
				}
				
				//KHY - 1025 - ������ �̺�Ʈ - �Ⱓ����. -��ǰ���� - ���ڴ޼�.
				if (pclClient->IsCountrySwitch(Switch_NowComEvent)) 
				{
					cltServer* pclServer = (cltServer*)pclClient;
					_SYSTEMTIME sStartAEventDay , sEndAEventDay;//�̺�Ʈ ���۽ð��� ������ �ð��α��� �ð�.
					cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

					sStartAEventDay.wYear = 2006;
					sStartAEventDay.wMonth = 11;
					sStartAEventDay.wDay = 8;
					sStartAEventDay.wHour = 0;
					sStartAEventDay.wDayOfWeek = 0;
					sStartAEventDay.wMinute = 0;
					sStartAEventDay.wSecond = 0;
					sStartAEventDay.wMilliseconds=0;

					sEndAEventDay.wYear = 2006;
					sEndAEventDay.wMonth = 12;
					sEndAEventDay.wDay = 5;
					sEndAEventDay.wHour = 0;
					sEndAEventDay.wDayOfWeek = 0;
					sEndAEventDay.wMinute = 0;
					sEndAEventDay.wSecond = 0;
					sEndAEventDay.wMilliseconds=0;

					SI32 startAEventdatevary = pclClient->GetMinuteFromsTime(sStartAEventDay);
					SI32 endAEventdatevary = pclClient->GetMinuteFromsTime(sEndAEventDay);
					SI32 LoginTime = pclClient->GetMinuteFromsTime(pclServer->sTime);

					if((LoginTime <= endAEventdatevary)
						&&(LoginTime >= startAEventdatevary))
						//�̺�Ʈ �Ⱓ���� �����̸� �����߱�.
					{
						SendLog_SetEventCouponInfo(pclchar, NOW_COM_EVENT,3); //  -���� ����.
					}
				}
				
			}
			else returnval = SRVAL_NOFATHERNAME;
		}
		break;
	case -1:	
		returnval = SRVAL_NOFATHERNAME;		
		break;			// �ش��ϴ� ������ �������� �ʴ´�. 
	case -2:	
		returnval = SRVAL_SAMEACCOUNT;		
		break;			// ���� ���������� ������ �� �� ����. 
	case -4: 
		{
			// ���� ���� �г�Ƽ �������̴�( �Ϸ絿�� ���ڸ� ���� �� ���� )
			SI32 fathercharid = pclCM->GetIDFromPersonID(pclMsg->siFatherPersonID);

			if ( pclCM->IsValidID(fathercharid) )
			{
				SendServerResponseMsg(0, SRVAL_PENALTY_SETFATHER_TEACHER,  0, 0, pclCM->CR[fathercharid]->GetCharUnique());
			}
			if( pclCM->IsValidID(id) )
			{
				SendServerResponseMsg(0, SRVAL_PENALTY_SETFATHER_PUPIL,  0, 0, pclCM->CR[id]->GetCharUnique());
			}
		}
		break;
	}

	if(returnval)
	{
		SendServerResponseMsg(0, returnval,  0, 0, pclCM->CR[id]->GetCharUnique());
	}
}

// ������ �ű��. 
void cltServer::DoMsg_DBMSG_RESPONSE_MOVEMONEY(sPacketHeader* pPacket)
{

	sDBResponse_MoveMoney* pclMsg = (sDBResponse_MoveMoney*)pPacket;

	switch(pclMsg->siResult)
	{
	case -1:		// from�� ���� �����ϴ�. 
		break;
	case 0:
		break;
	case 1:
		{
			// from�� �������� ������Ʈ �Ѵ�. 
			if(pclMsg->siFromPersonID)
			{
				SI32 fromid = pclCM->GetIDFromPersonID(pclMsg->siFromPersonID);
				if( pclCM->IsValidID(fromid) )
				{
					pclCM->CR[fromid]->pclCI->clIP.SetMoney(pclMsg->siFromPersonMoney);

					// Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
					if(pclMsg->siMovedMoney)
					{
						SI32 reason = CHANGE_MONEY_REASON_NOTDEFINED;

						cltGameMsgResponse_ChangeMoney clinfo(pclMsg->siFromPersonMoney, -pclMsg->siMovedMoney, reason);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->CR[fromid]->SendNetMsg((sPacketHeader*)&clMsg);
					}

				}
			}

			// to�� �������� ������Ʈ �Ѵ�. 
			if(pclMsg->siToPersonID)
			{
				SI32 toid = pclCM->GetIDFromPersonID(pclMsg->siToPersonID);
				if( pclCM->IsValidID(toid) )
				{
					pclCM->CR[toid]->pclCI->clIP.SetMoney(pclMsg->siToPersonMoney);

					// Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
					if(pclMsg->siMovedMoney)
					{
						SI32 reason = CHANGE_MONEY_REASON_NOTDEFINED;
						
						cltGameMsgResponse_ChangeMoney clinfo(pclMsg->siToPersonMoney, pclMsg->siMovedMoney, reason);
						cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
						pclCM->CR[toid]->SendNetMsg((sPacketHeader*)&clMsg);
					}

				}

			}

		}
		break;
	}	
}
// �ɸ��� ���ӿɼǼ���
void cltServer::DoMsg_DBMSG_RESPONSE_CHAR_OPTION(sPacketHeader* pPacket)
{
	sDBResponse_Char_Option* pclMsg = (sDBResponse_Char_Option*)pPacket;
	if( pclMsg->siResult == 0 )
	{
		return ;
	}
	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)
		return ;
	// ������ �ִ� �ش� ������ ä�� ������ ���� �Ѵ�.
	pclCM->CR[ id ]->pclCI->clCharOption.Set( &pclMsg->clPCharOptioninfo ) ;

	//if(pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID) return ;

	cltGameMsgResponse_Char_Option clinfo( pclCM->CR[ id ]->pclCI->clCharOption );
	cltMsg clMsg(GAMEMSG_RESPONSE_CHAR_OPTION, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
}
/// �ɸ��� ���ӿɼ��� �������´�
void cltServer::DoMsg_DBMSG_RESPONSE_CHAR_GET_OPTION(sPacketHeader* pPacket)
{
	sDBResponse_Char_Get_Option* pclMsg = ( sDBResponse_Char_Get_Option* )pPacket;

	SI32 id	= pclMsg->siCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	pclCM->CR[id]->pclCI->clCharOption.Set( &pclMsg->clPCharOptioninfo );
	
	// Ŭ���̾�Ʈ�� ���� ����
	cltGameMsgResponse_Char_Get_Option clinfo;
	clinfo.clCharOptionInfo.Set( &pclCM->CR[id]->pclCI->clCharOption );

	cltMsg clMsg(GAMEMSG_RESPONSE_CHAR_GET_OPTION, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	

}
// ���ݾ��� ����ȴ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_CHANGEMONEY(sPacketHeader* pPacket)
{

	sDBResponse_ChangeMoney* pclMsg = (sDBResponse_ChangeMoney*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)
		return ;

	if(pclCM->CR[id]->pclCI->GetPersonID() != pclMsg->siPersonID)return ;

	// ���ݾ��� ������Ʈ �Ѵ�. 
	pclCM->CR[id]->pclCI->clIP.SetMoney(pclMsg->siMoney);

	// Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
	cltGameMsgResponse_ChangeMoney clinfo(pclMsg->siMoney, pclMsg->siChangedMoney, pclMsg->siReason);
	cltMsg clMsg(GAMEMSG_RESPONSE_CHANGEMONEY, sizeof(clinfo), (BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 9] // �������� ����Ǵ� ������ ���(���𰪿����� �پ��� ����)
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_CHANGE_MONEY, 
										pclMsg->siReason, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siTargetPersonID, pclMsg->siChangedMoney, pclMsg->siMoney, NULL, 0, 0, 0, 0, 0, 0, 
										0, 0, 0, 0, 0, NULL, NULL);
	}
}



// ����ġ��  ����ȴ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_CHANGEEXP(sPacketHeader* pPacket)
{

	sDBResponse_ChangeExp* pclMsg = (sDBResponse_ChangeExp*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 10] // ����ġ ���� ����(�������� ��� �̿��� �ٸ� ��쿡 ���ѡ�)
		// param1 == ������ ����ġ
		cltServer* pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_INPORTANTPARAMEATA, LOGCOMMAND_INDEX_INPORTANTPARAMEATA_EXPCHAMGE,
										pclMsg->siReason, NULL, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										pclMsg->siChangedExp, 0, 0, 0, 0, NULL, NULL);
	}
}

// ���� ����ġ��  ����ȴ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_CHANGEFAMEEXP(sPacketHeader* pPacket)
{

	sDBResponse_ChangeFameExp* pclMsg = (sDBResponse_ChangeFameExp*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}
	
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 10] // ���� ����ġ��  ����.
		// param1 == ������ ����ġ.
		cltServer* pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_INPORTANTPARAMEATA, LOGCOMMAND_INDEX_INPORTANTPARAMEATA_CHANGEFAMEEXP,
										pclMsg->siReason, 0, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, pclMsg->siFameExp, 
										pclMsg->siChangedFameExp, 0, 0, 0, 0, NULL, NULL);
	}
}

// ����Ʈ ���� ���. 
void cltServer::DoMsg_DBMSG_RESPONSE_SETQUESTINFO(sPacketHeader* pPacket)
{
	// PCK : �α׸� �������ؼ� �����߰���.
	// param1 : ����Ʈ Ÿ��, param2 : ����Ʈ ����ũ, param3 : ����Ʈ ����, param4 : ����Ʈ Ŭ���� ����
	// charparam1 : ����Ʈ Ŭ�����
	sDBResponse_SetQuestInfo* pclMsg = (sDBResponse_SetQuestInfo*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID) ;
	if ( pclCM->IsValidID(id) == false ) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	// QuestState�� 2������ ��ȯ
	TCHAR szCharParam1[50] = { '\0', };
	_itoa(pclMsg->clQuestInfo.siQuestState, szCharParam1, 2);
	
	cltServer* pclserver = (cltServer*)pclClient;
	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_QUEST, LOGCOMMAND_INDEX_QUEST_INFO_SET,
									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
									pclMsg->siQuestType, pclMsg->clQuestInfo.siCurQuestUnique, pclMsg->clQuestInfo.siCurQuestStep, pclMsg->clQuestInfo.siQuestCmd, 0,
									szCharParam1, NULL);
}

// PCK : ����� ����Ʈ ���� ��� - �α׿�
void cltServer::DoMsg_DBMSG_RESPONSE_SETSPECIALQUESTCHANGE(sPacketHeader* pPacket)
{
	// ����� ����Ʈ�� �ϳ� �Ϸ��ϰ� ���� �޴� ����� ��� �ܾ��� ����� �α׷� �����.
	sDBResponse_SetSpecialQuestInfo* pclMsg = (sDBResponse_SetSpecialQuestInfo*)pPacket;
	
	cltServer* pclserver = (cltServer*)pclClient;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID) ;
	if ( pclCM->IsValidID(id) == false ) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	// ���� ����� ����Ʈ�� �������� ����ġ�� �ְ� ������, ���� ���̳� �������� �߰��Ұ��� ����ؼ� ������ �κе� ���⵵�� �Ѵ�.
	// param1 == ������ ����ũ, param2 == ����������� ����, param3 == ������� �ݾ�, param4 == ������� ����ġ
	// charparam1 == �ܾ����� �����Ȳ

	TCHAR szCharParam1[50] = { '\0', };
	StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("WordAssembly = %d%d%d%d%d%d%d%d%d%d"),
		pclMsg->bWordAssemblySwitch[0], pclMsg->bWordAssemblySwitch[1],
		pclMsg->bWordAssemblySwitch[2], pclMsg->bWordAssemblySwitch[3],
		pclMsg->bWordAssemblySwitch[4], pclMsg->bWordAssemblySwitch[5],
		pclMsg->bWordAssemblySwitch[6], pclMsg->bWordAssemblySwitch[7],
		pclMsg->bWordAssemblySwitch[8], pclMsg->bWordAssemblySwitch[9]);

	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_QUEST, LOGCOMMAND_INDEX_QUEST_SPECIALL_QUEST_CHANGE, 
		0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		pclMsg->clSpecialQuestInfo.clQuestRewardInfo.siItemUnique[0], pclMsg->clSpecialQuestInfo.clQuestRewardInfo.siItemNum[0], 
		pclMsg->clSpecialQuestInfo.clQuestRewardInfo.siMoney, pclMsg->clSpecialQuestInfo.clQuestRewardInfo.siExp, 0,
		szCharParam1, NULL);

}

// ����Ʈ ���� ���. 
void cltServer::DoMsg_DBMSG_RESPONSE_SETSPECIALQUESTINFO(sPacketHeader* pPacket)
{
	sDBResponse_SetSpecialQuestInfo* pclMsg = (sDBResponse_SetSpecialQuestInfo*)pPacket;

	if ( pclMsg->bClearWordSpecialQuest && pclMsg->siClearWordSpecialQuestNum > 0 )
	{
		SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID) ;
		if ( pclCM->IsValidID(id) == false ) return;

		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

		// �ܾ����� Ŭ���� Ƚ�� - ��ι�(�������)
		pclchar->pclCI->clQuestInfo.siClearWordSpecialQuest = pclMsg->siClearWordSpecialQuestNum ;
		//pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest = pclMsg->siEventClearWordSpecialQuestNum;
		// 7ȸ ���� �ʱ�ȭ(8ȸ�� 1�� ���̰�..)
		pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest = pclMsg->siEventClearWordSpecialQuestNum - (((pclMsg->siEventClearWordSpecialQuestNum - 1) / 7) * 7);


		//����Ʈ ���� Ƚ���� 2���� ���� ��� �������� �̿��� 2��� Ƚ������ Ư�� ������ �Ѵ�.
		SI32 siEventReward = (pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest) / 2;
		SI32 siCheckValue = (pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest) % 2;

		if( pclEventTimeManager->InEventTime("MagicLamp_event", &sTime) )
		{	
			// [����-090602] 1, 3, 5,7 ... �� Ƚ������ ������ �ش�. (������ 1�϶�)
			if ( 1 == siCheckValue ) // ������ 1�϶��� ���� ������ ������ �ش�.
			{
				siEventReward = siEventReward % 4; 
				SI32	itemunique = 0;
				SI16	itemnum = 0;
				SI16	brareswitch = 0;
				cltItem clItem;

				switch ( siEventReward )
				{
				case 0:
					// 1ȸ ���� : ���귯�����ݷ� - ������ 5�� ����
					itemunique = ITEMUNIQUE(3890);
					itemnum = 5;
					break;
				case 1:
					// 3ȸ ���� : ��ũīī��
					itemunique = ITEMUNIQUE(2781);
					itemnum = 8;
					break;
				case 2: 
					// 5ȸ ���� : ���κ��� ����ũ
					itemunique = ITEMUNIQUE(3897);
					itemnum = 10;
					break;
				case 3:
					// 7ȸ ���� : �ֿϵ���
					itemunique = ITEMUNIQUE(13646); // ��Ű
					itemnum = 1;
					break;;
				}


					if(pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, 0, &brareswitch) == true)
					{
						clItem.SetItemNum(itemnum);

						SI32 rtnvalue = 0;
						if( pclchar->InputItem(pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WORDASSEMBLE, &rtnvalue) == false)
						{
							// �κ��丮�� ���� �������� ����(��ü��)���� ������.
							((cltServer*)pclClient)->SendPostOfficeItemToPerson( pclchar->pclCI->GetPersonID(), &clItem , NULL ,0  ); // ���� �ʿ�
							
							//SendLog_InventoryFull( pclchar->pclCI->GetPersonID(),itemunique,itemnum,
							//	INPUTITEMPERSON_OPTION_WORDASSEMBLE);
						}
					}
			}

		}

		// ����� ����Ʈ ���� Ÿ���� �о�´�. - ��ι�(�������)
		SI64 siGV_SpecialQuestReward = GetGlobalValue(TEXT("GV_SpecialQuestReward"));

		GMONEY givemoney = 0;
		// �⺻ ����
		if( siGV_SpecialQuestReward == 0 )
		{
			// ���� �����Ѵ�.
			SI32 npcRate =  pclClient->pclItemManager->clItemStatistic.siNPCPriceRate;
			givemoney = pclchar->pclCI->clIP.GetLevel() * npcRate *50;
			sDBRequest_ChangeMoney clMsg(pclchar->GetID(), pclchar->pclCI->GetPersonID(), CHANGE_MONEY_REASON_WORDSPECIALQUEST, givemoney );
			SendDBMsg(DBSELECT_BASE, (sPacketHeader*)&clMsg);

			pclAddMoneyStatistics->Add(ADDMONEY_STATISTIC_SPECIALQUEST, givemoney);

			// �����ϰ� ��� �������� �ش�.  - 1%
			if (rand()%100 == 0)
			{
				SI32 bigitemunique = pclItemManager->GetRandBigWarItem();
				cltItem clItem;
				SI16 brareswitch = 0;
				SI16 itemnum = 10;
				if(pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch) == true)
				{
					clItem.SetItemNum(itemnum);

					SI32 rtnvalue = 0;
					if( pclchar->InputItem(pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WORDASSEMBLE, &rtnvalue) == false)
					{
						SendLog_InventoryFull( pclchar->pclCI->GetPersonID(),bigitemunique,itemnum,
							INPUTITEMPERSON_OPTION_WORDASSEMBLE);
					}
				}
			}
		}
		// �ҷ��� �̺�Ʈ ���� - by LEEKH 2007.10.16
		else if ( siGV_SpecialQuestReward == 1 )
		{
			SI32	bigitemunique = 0;
			cltItem	clItem;
			SI16	brareswitch = 0;
			SI16	itemnum = 10;
			SI32	usedate = 0;

			SI32 siRand = rand() % 100;

			// �����ϰ� ��� �������� �ش�.  - 40%
			if ( siRand < 10 )
			{
				bigitemunique = ITEMUNIQUE(3899);
				itemnum = 1;
			}
			else if ( siRand < 30 )
			{
				bigitemunique = pclItemManager->GetRandBigWarItem();
				itemnum = 10;
			}
			else if ( siRand < 100 )
			{
				// �ҷ��� ����
				bigitemunique = ITEMUNIQUE(23055);
				itemnum = 1;
				usedate = 7;
			}

			if(bigitemunique > 0)
			{
				if(pclItemManager->MakeRandItemUnique( bigitemunique, &clItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate) == true)
				{
					clItem.SetItemNum(itemnum);

					SI32 rtnvalue = 0;
					if( pclchar->InputItem(pclItemManager, &clItem, INPUTITEMPERSON_OPTION_WORDASSEMBLE, &rtnvalue) == false)
					{
						SendLog_InventoryFull( pclchar->pclCI->GetPersonID(), bigitemunique, itemnum, INPUTITEMPERSON_OPTION_WORDASSEMBLE);
					}
				}
			}
		}

		// Ŭ���̾�Ʈ�� ���� ������ �뺸�Ѵ�.
		cltGameMsgResponse_Clear_SpecialWord clinfo( pclchar->pclCI->clQuestInfo.siClearWordSpecialQuest, givemoney, pclchar->pclCI->clIP.GetMoney(), pclchar->pclCI->clQuestInfo.siEventClearWordSpecialQuest );		// �̺�Ʈ�� Ƚ�� üũ ���� �߰��� ���� �κ� ����.
		cltMsg clMsg1(GAMEMSG_RESPONSE_CLEAR_SPECIALWORD, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNetMsg((sPacketHeader*)&clMsg1);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			cltServer * pclServer = (cltServer*)pclClient;

			// [�α��߰� : Ȳ���� 2007. 10. 8] // Ư�� ����Ʈ. 
			// param1 : ������ ����ũ
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_QUEST, LOGCOMMAND_INDEX_QUEST_SPECIALL_QUEST_END, 
				0, pclchar, NULL, 0, givemoney, pclchar->pclCI->clIP.GetMoney(), NULL, 0, 0, 0, 0, 0, 0, 
				pclMsg->clSpecialQuestInfo.clQuestRewardInfo.siItemUnique[0], 0, 0, 0, 0, NULL, NULL);

		}
	}
}

// PCK : ���̹� ����Ʈ �߰� (�α׿�)
void cltServer::DoMsg_DBMSG_RESPONSE_SETNAMINGQUESTINFO(sPacketHeader* pPacket)
{
	sDBResponse_SetNamingQuestInfo* pclMsg = (sDBResponse_SetNamingQuestInfo*)pPacket;

	cltServer * pclServer = (cltServer*)pclClient;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID) ;
	if ( pclCM->IsValidID(id) == false ) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	// param1 == ������ ����ũ, param2 == ����������� ����, param3 == ������� �ݾ�, param4 == ������� ����ġ
	// charparam1 == ���̹� �ε���, ����

	TCHAR szCharParam1[50] = { '\0', };
	StringCchPrintf(szCharParam1, sizeof(szCharParam1), TEXT("NamingIndex = %d, NamingLevel = %d"), 
					pclMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingIndex, 
					pclMsg->clNamingQuestInfo.clQuestRewardInfo.siNamingLevel);
	
	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_QUEST, LOGCOMMAND_INDEX_QUEST_NAMING_INFO_SET, 
		0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		pclMsg->clNamingQuestInfo.clQuestRewardInfo.siItemUnique[0], pclMsg->clNamingQuestInfo.clQuestRewardInfo.siItemNum[0], 
		pclMsg->clNamingQuestInfo.clQuestRewardInfo.siMoney, pclMsg->clNamingQuestInfo.clQuestRewardInfo.siExp, 0, 
		szCharParam1, NULL);
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETPERSONNAMINGINFO(sPacketHeader* pPacket)
{
	sDBResponse_SetPersonNamingInfo* pclMsg = (sDBResponse_SetPersonNamingInfo*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );
	if(pclCM->IsValidID(id) == FALSE ) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if ( pclMsg->siResult > 0 )
	{
		if( pclMsg->siIndex >= 0 && pclMsg->siIndex < MAX_NAMINGQUEST_INDEX )
		{
			pclCM->CR[id]->pclCI->clPersonNaming.m_siLevel[pclMsg->siIndex] = pclMsg->siLevel;

			// naming - TODO : �Ϸ� �ߴٰ� �޼��� ��������...

			cltGameMsgResponse_SetPersonNamingInfo clinfo( pclMsg->siIndex, pclMsg->siLevel );
			cltMsg clMsg(GAMEMSG_RESPONSE_SETPERSONNAMINGINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_SETPERSONCURRENTNAMING(sPacketHeader* pPacket)
{
	sDBResponse_SetPersonCurrentNaming* pclMsg = (sDBResponse_SetPersonCurrentNaming*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );
	if(pclCM->IsValidID(id) == FALSE ) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if ( pclMsg->siResult > 0 )
	{
		pclCM->CR[id]->pclCI->clPersonNaming.m_siSelectedIndex = pclMsg->siSelectedIndex;
		pclCM->CR[id]->pclCI->clPersonNaming.m_siSelectedLevel = pclMsg->siSelectedLevel;

		cltGameMsgResponse_SetPersonCurrentNaming clinfo( pclCM->CR[id]->GetCharUnique(), pclMsg->siSelectedIndex, pclMsg->siSelectedLevel );
		cltMsg clMsg(GAMEMSG_RESPONSE_SETPPERSONCURRENTNAMING, sizeof(clinfo), (BYTE*)&clinfo);
		pclchar->SendNeighbourMsg(&clMsg, true);
	}
}

//KHY - 0822 - ���Ǻ� ����Ʈ �߰�.
void cltServer::DoMsg_DBMSG_RESPONSE_GETHAPPYBEANQUESTINFO(sPacketHeader* pPacket)
{
	sDBResponse_GetHappyBeanClearInfo* pclMsg = (sDBResponse_GetHappyBeanClearInfo*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );
	if(pclCM->IsValidID(id) == FALSE ) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 siResult = 0;
	if(pclMsg->siQuestType == QUEST_TYPE_HAPPYBEAN)
	{
		if(pclMsg->siQuest_CompleteCount < MAX_QUEST_ONEDAY_COUNT)
		{
			if((pclMsg->siQuest_CompleteCount  + pclMsg->siQuest_StartCount) <= MAX_QUEST_ONEDAY_COUNT)
				siResult = 1;
		}
	}

	switch(siResult )
	{
		case 0 : //0 = ����Ʈ ����.
			{
				// ����Ʈ ���¸� DB�� �����Ѵ�. - ���� ������ StartCount���� �����.
				SendLog_SetHappyBeanClearInfo(pclchar, QUEST_TYPE_HAPPYBEAN, QUEST_HAPPYBEAN_STEP_NON);

				cltGameMsgResponse_HappyBeanQuestStart_NO clinfo( siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_NO, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
		case 1 : // 1= ����Ʈ ���� �㰡.
			if ( pclchar->MakeHappyBeanQuest( ) == false ) // ����Ʈ�� �����.
			{
				// ����Ʈ ���¸� DB�� �����Ѵ�. - ���� ������ StartCount���� �����.
				SendLog_SetHappyBeanClearInfo(pclchar, QUEST_TYPE_HAPPYBEAN, QUEST_HAPPYBEAN_STEP_NON);

				// ����Ʈ ���� �㰡�� ��������, ����Ʈ�� ����µ� �����ߴ�. (1)
				cltGameMsgResponse_HappyBeanQuestStart_NO clinfo( siResult );
				cltMsg clMsg(GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_NO, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			}
			else
			{
				// ����Ʈ ���� �㰡�� ���԰�, ����Ʈ�� ����µ� ����.
				cltGameMsgResponse_HappyBeanQuestStart_OK clinfo(&pclchar->pclCI->clQuestInfo.clHappyBeanQuestInfo );
				cltMsg clMsg(GAMEMSG_RESPONSE_HAPPYBEANQUEST_START_OK, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);		
			}

			
			break;
		case 2 : // 2= ����Ʈ �Ϸᰡ ���������� ���� �Ǿ���.
				// �Ұ� �ֳ�?
			break;
		default :
			return;
			break;
	}
}

//KHY - 0906 - ���ӹ� �̺�Ʈ ����.
void cltServer::DoMsg_DBMSG_RESPONSE_GETEVENTCOUPONINFO(sPacketHeader* pPacket)
{
	sDBResponse_GetEventCouponInfo* pclMsg = (sDBResponse_GetEventCouponInfo*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );
	if(pclCM->IsValidID(id) == FALSE ) return;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	SI32 siResult = pclMsg->siResult;
	if(pclMsg->siEventType == GAMEROOM_COUPON_EVENT)
	{
		switch(siResult )
		{
			case 0 : //�����̶� �߱� �޾Ҵ�
				{
					//�Ұ� ����. ���� ����.
				}
				break;
			case 1 : // //�߱� ���� ������ ����. -�����߱�.
				{
					//���� �߱�.
					cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
					cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				}
				break;
			default :
				return;
				break;
		}
	}

	if(pclMsg->siEventType == AUCTION_COUPON_EVENT)
	{
		switch(siResult )
		{
		case 0 : 
			{			}
			break;
		case 1 : // //
			{
				//���� �߱�.
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			}
			break;
		case 2 : 
			{
				//���� �߱�.
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			}
			break;
		case 3 : 
			{
				//���� �߱�.
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			}
			break;
		case 4 : 
			{
				//���� �߱�.
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			}
			break;
		case 5 :
			{
				//���� �߱�.
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			}
			break;
		case 6 : 
			{
				//���� �߱�.
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

			}
			break;
		
		default :
			return;
			break;
		}
	}
	// ������,��Ŀ �̺�Ʈ ����
	if (pclMsg->siEventType == GOSTOP_POKER_EVENT)
	{
		switch (siResult)
		{
		case 0 :	// �̹� ������ �Ǵ� ��Ŀ�Ӵϸ� �޾��� ���
			{
				// �ҰԾ���. 
			}
			break;
		case 1 :
			{
				// ��Ŀ�Ӵ� ����
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;
		case 2 :
			{
				// ������Ӵ� ����
				cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
				cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);
			}
			break;

		default :
			return;
			break;
		}
	}

	//KHY - 1024 - �Ѱ��� VIP �̺�Ʈ
	if(pclMsg->siEventType == HAN_GAME_VIP_EVENT)
	{
		switch (siResult)
		{
			case 0 :	// ����. 
				{
					// �ҰԾ���. 
				}
				break;
			case 1 :
				{
					cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
					cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);
				}
				break;
			default :
				return;
				break;
		}
	}
	//KHY - 1025 - ������ �̺�Ʈ
	if(pclMsg->siEventType == NOW_COM_EVENT)
	{
		switch (siResult)
		{
			case 0 :	// ����. 
				{
					// �ҰԾ���. 
				}
				break;
			case 1 : // ���� ����. - ���� �߱� ����.
			case 2 : // 10���޼�.
			case 3 :	// ���� �Ǳ� ����.			
				{
					cltGameMsgResponse_EventCoupon clinfo( pclMsg->siEventType ,siResult);
					cltMsg clMsg(GAMEMSG_RESPONSE_EVENTCOUPON, sizeof(clinfo), (BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clMsg);
				}
				break;
			default :
				return;
				break;
		}
	}

	// �ܿ����! ����S�� ������ ������ �̺�Ʈ - by LEEKH 2007.12.21
	if(pclMsg->siEventType == SUPER_ROOKIE_LEVELUP_EVENT)
	{
		cltItem clRewardItem;
		clRewardItem.Init();
		SI16	brareswitch = 0;
		SI32	usedate = 0;
		SI32	ItemUnique = 0;
		SI32	ItemNum = 0;

		switch (siResult)
		{
		case 0 :	
			{
				// �ҰԾ���. 
			}
			break;
		case QUEST_TYPE_BATTLE1:			// ����������
			{
				ItemUnique = ITEMUNIQUE(13612);
				ItemNum = 1;
				usedate = 30;
			}
			break;
		case QUEST_TYPE_BASIC1:				// ����
			{
				ItemUnique = ITEMUNIQUE(15900);
				ItemNum = 1;
			}
			break;
		case QUEST_TYPE_HUNTINGGROUND01:	// ���Ǿ�
			{
				ItemUnique = ITEMUNIQUE(24096);
				ItemNum = 1;
			}
			break;
		case QUEST_TYPE_HUNTINGGROUND02:	// ����Ű
			{
				ItemUnique = ITEMUNIQUE(5400);
				ItemNum = 1;
			}
			break;
		case QUEST_TYPE_BATTLE2:			// ��
			{
				ItemUnique = ITEMUNIQUE(3880);
				ItemNum = 5;
			}
			break;
		}

		if(ItemUnique > 0)
		{
			SI32 ref = pclClient->pclItemManager->FindItemRefFromUnique(ItemUnique);
			if (ref <= 0)	return;

			// ������ ��¥�� ������, �������� �⺻���� ����Ѵ�.
			if(usedate <= 0)	usedate = pclClient->pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;

			if( pclClient->pclItemManager->MakeRandItemUnique( ItemUnique, &clRewardItem, 0, 0, &brareswitch, 0, 0, 0, 0, 0, usedate ) )
			{
				// -------------------------------------------
				// LEEKH �����ڵ� ----------------------------
				clRewardItem.uiDateYear = 0;
				clRewardItem.uiDateMonth = 0;
				clRewardItem.uiDateDay = 0;
				// -------------------------------------------

				// ���� ����
				clRewardItem.siItemNum = ItemNum;

				cltSystemRewardInfo clInfo;

				clInfo.m_sGiveTime = sTime;

				// ������ ������ 30�ϵ��� ������ �����ϴ�.
				TimeAddDate(&clInfo.m_sGiveTime, 30, &clInfo.m_sExpireTime);

				//NDate ExpireDate;
				//ExpireDate.SetDate( sTime.wYear, sTime.wMonth, sTime.wDay );
				//ExpireDate.AddMonths( 2 );

				//_SYSTEMTIME sExpireTime;
				//TimeAddDate(&sTime, 30, &sExpireTime);

				//clInfo.m_sExpireTime = sExpireTime;
				clInfo.m_clItem.Set( &clRewardItem );

				NTCHARString256		kText( GetTxtFromMgr(7230) );
				StringCchCopy(clInfo.m_szReason, sizeof(clInfo.m_szReason), kText);
 
				sDBRequest_GiveSystemReward clMsg( GIVEREWARD_ACCOUNT, pclchar->pclCI->GetPersonID(), pclchar->GetID(), &clInfo );
				SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );
			}
		}
	}
}

// ���ο� Person�� ����������� �˷��ִ� �޽��� ���ν���.
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOCREATE(sPacketHeader* pPacket)
{
	sDBResponse_PersonTotalInfoCreate* pclMsg = (sDBResponse_PersonTotalInfoCreate*)pPacket;

	SI32 id	= pclMsg->usCharID;
	SI32 id2 = pclMsg->siPersonID;

	if(pclCM->IsValidID(id) == FALSE)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	//---------------------------------------------------
	// ������� Ȯ���Ѵ�. 
	//---------------------------------------------------
	switch(pclMsg->siResult)
	{
	case 1:	// ����. 
		{
			// [����] ĳ���� ������ ����������� �����Ѵ� - 2008.11.19
			if ( true == pclEventTimeManager->InEventTime( TEXT("CreateCharReward"), &sTime) )
			{
				if ( pclRewardMgr )
				{
					pclRewardMgr->GiveCreateCharRewardItem( id2 );
				}
			} 
			
			cltCharListInfo sendMsg;
			sendMsg.clBI.SetName( pclMsg->szName );
			cltMsg clMsg( GAMEMSG_RESPONSE_MAKENEWCHAR, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );

			if( pclPromoterManager )
			{
				// ���θ��� �ý��ۿ��� ĳ���� ������ ���θ��Ϳ� �������� ���� ����Ѵ�.
				clPromoterInfo PromoterInfo( id, clPromoterInfo::SET_PROMOTER_CHAR );
				PromoterInfo.SetName( pclMsg->szName ); 
				PromoterInfo.SetAccountID( pclMsg->szAccountID );
				pclPromoterManager->Push( PromoterInfo );
			}


			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [�α��߰� : Ȳ���� 2007. 9. 5] // ĳ���� ���� �α׾���.
				// pszCharParam1 == �������̵�.
				// pszCharParam2 == ĳ���� �̸�.
				// �ӽÿ�
				cltCharServer clTmpCharServer;
				// ���� id
				clTmpCharServer.pclCI->clBI.siIDNum = pclchar->pclCI->clBI.siIDNum;
				// ĳ���� ��
				clTmpCharServer.SetName(pclMsg->szName);
				// personid
				clTmpCharServer.pclCI->SetPersonID(pclMsg->siPersonID);

				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_MAKECHAR, 
					0, &clTmpCharServer, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
					0, 0, 0, 0, 0, pclMsg->szAccountID, pclMsg->szName);
			}
		}
		break;
	case 0:	// ���� 
		return ;
	case -1:	// �ߺ� ���̵�. 
		// Ŭ���̾�Ʈ�� ������ ���䰪�� ������. 
		SendServerResponseMsg(0, SRVAL_MAKEPERSON_FAIL_DUPNAME,  0, 0, pclCM->CR[id]->GetCharUnique());
		return ;	// -> �����ؾ� �Ѵ�. �ߺ� ���̵�� ������ �ƿ� �ȵȴ�.
	}

	siNewCharNum++;

	//-------------------------------------------------------------
	// GameDBManager_World���� ��� Person������ �˷��� ���� ��û�Ѵ�. 
	//-------------------------------------------------------------
	sDBRequest_PersonListInfoGet clMsg(pclMsg->usCharID, pclCM->CR[id]->pclCI->clBI.siIDNum);
	pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);

	if(pclMsg->siResult == 1)
	{
		// �α� �ۼ� - ĳ���� ����
		//WriteLogToDB()
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_PERSONLISTINFOGET(sPacketHeader* pPacket)//�ɸ��͸� ���� �ϰ� ����Ʈ�� ���׷��̵� �Ѵ�.
{
	sDBResponse_PersonListInfoGet* pclMsg  = (sDBResponse_PersonListInfoGet*)pPacket;

	SI32			siGetID	= pclMsg->usCharID;
	cltCharServer*	pclChar	= pclCM->GetCharServer( siGetID );
	TraceBoot("CHARLIST DB RESP arrived usCharID=%u pclChar=%p idnum=%d",
		(unsigned)pclMsg->usCharID, (void*)pclChar,
		pclChar ? pclChar->pclCI->clBI.siIDNum : -1);
	if ( NULL == pclChar )
	{
		TraceBoot("CHARLIST ABORT: GetCharServer(%u)==NULL -> sending 0 packets", (unsigned)siGetID);
		return;
	}

	cltPBasicInfo	clinfo;
	cltCharListInfo	clCharListInfo;
	bool			bDeleteChar;

	for ( SI32 siIndex=0; siIndex<MAX_TOTAL_CHAR_PER_USER; siIndex++ )
	{
		// �⺻ ���� ����
		clinfo.Set( &pclMsg->personInfo[siIndex] );
		clinfo.SetIndex( siIndex );	// ���� �ε����� ��� �ϵ��� �Ѵ�

		// ĳ���� ����Ʈ�� ���� ����
		clCharListInfo.clBI.Set( &clinfo );
		clCharListInfo.clBI.uiGMMode	= 0;
		clCharListInfo.clIP.siLevel		= pclMsg->siLevel[siIndex];
		clCharListInfo.DelCharTime		= pclMsg->stTime[siIndex];

		/*
		// �����ۿ� ���� ������ �տ� 3���� ĳ���͸� ���� �ִ´�
		if ( MAX_CHAR_PER_USER > siIndex )
		{
			for ( SI32 siCount=0; siCount<MAX_CHAR_PER_USER; siCount++ )
			{
				clCharListInfo.clCharItem[siCount] = pclMsg->personIteminfo[siIndex][siCount];
			}
		}
		else
		{
			for ( SI32 siCount=0; siCount<MAX_CHAR_PER_USER; siCount++ )
			{
				clCharListInfo.clCharItem[siCount].Init();
			}
		}
		*/

        // ĳ���� ����Ʈ�� ���� ������ �����Ѵ�
		if ( pclChar->pclCharListInfo )	
		{
			pclChar->pclCharListInfo[siIndex].Set(&clCharListInfo);
		}

		// ĳ���� ���� ���� �Ⱓ�� ���� ó��
		bDeleteChar = false;

		if ( 0 < clCharListInfo.DelCharTime.wYear )
		{
			// �����ؾ� �� ĳ�������� üũ
			SYSTEMTIME st_CheckTime;
			SYSTEMTIME st_CurTime;

			// ������ ������ �ð��� 3���� ���Ѵ�.
			TimeAddDate(&clCharListInfo.DelCharTime, 3, &st_CheckTime);

			// ���� �ð�
			GetLocalTime(&st_CurTime);

			if(TimeCompare(&st_CurTime, &st_CheckTime) > 0)
			{
				bDeleteChar = true;
			}
		}

		// ���� ���� �Ⱓ�� ������ �����ؾ� �Ѵٸ�
		if ( true == bDeleteChar )
		{
			TraceBoot("CHARLIST slot=%d SKIPPED (pending delete, DelCharTime=%04u-%02u-%02u) name='%s'",
				siIndex, clCharListInfo.DelCharTime.wYear, clCharListInfo.DelCharTime.wMonth,
				clCharListInfo.DelCharTime.wDay, clinfo.GetName());
			sDBRequest_PersonTotalInfoDelete clMsg( siGetID, pclChar->pclCI->clBI.siIDNum, siIndex, clCharListInfo.clBI.clPerson.siPersonID, TEXT(""), pclChar->GetSession()->GetIP());
			SendDBMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg );
		}
		else
		{
			TraceBoot("CHARLIST SEND slot=%d name='%s' personID=%d mapIndex=%d level=%d delYr=%u",
				siIndex, clinfo.GetName(), clCharListInfo.clBI.clPerson.siPersonID, clinfo.GetMapIndex(),
				pclMsg->siLevel[siIndex], clCharListInfo.DelCharTime.wYear);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHARLIST, sizeof(cltCharListInfo), (BYTE*)&clCharListInfo);
			pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
	}

}

//cyj ����ڰ� ������ ���� �޾ƿ��� �Լ�
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONITEMINFOGET(sPacketHeader* pPacket)
{
	sDBResponse_PersonItemInfoGet* pclMsg = (sDBResponse_PersonItemInfoGet*)pPacket;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	//---------------------------------------------------
	// ������� Ȯ���Ѵ�. 
	//---------------------------------------------------
	//if(pclMsg->siResult == 0)return ;

	pclCM->CR[id]->pclCI->clCharItem.Set(&pclMsg->ItemInfo);

	// �޸� ���� �������� ���� ��� ������ ���׷��̵� ==============================================================================
	CDormancySystem *pclDormancySystem = this->pclDormancySystem;
	if( NULL != pclDormancySystem)
	{
		CDormancyEquipItemRewardServer *pclDormancyServer = (CDormancyEquipItemRewardServer *)pclDormancySystem->GetDormancyEquipItemReward();
		if(NULL != pclDormancyServer)
		{
			pclDormancyServer->UpgradeItemInven(pclCM->CR[id]);
		}
	}
	//==============================================================================================================================

	pclCM->CR[id]->SetUpdateSwitch( UPDATE_INVENTORY, true, 0);


	return;
}

// ���ʷα��� ����
// ����ڰ� Person�� ���������� DB�� Person�� ��ü ������ ��û�ϴµ� �̶� ������ ��ü ������ ó���ϴ� �Լ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOGETBYIDNUM(sPacketHeader* pPacket)
{
	SI32 i = 0 ;

	sDBResponse_PersonTotalInfoGet* pclMsg = (sDBResponse_PersonTotalInfoGet*)pPacket;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	TCHAR*	pszCounteyCode = NULL;
	int		siAccessDeny = IPACCESSDENY_NONE;
	bool	bPass = false;

	if(pclAccessDenyMgr)		siAccessDeny = pclAccessDenyMgr->GetAccessDeny(pclCM->CR[id]->GetSession()->GetIP());
	if(pclIPToCountryMgr)		pszCounteyCode = pclIPToCountryMgr->GetCountryCode(pclCM->CR[id]->GetSession()->GetIP());

	// ��� ���̵�� ������ ��� - 20090420 - BASIC_PLAYERMASTER_LEVEL (90) ���� �α����� �����ϴ�. 
	if( pclCM->CR[id]->GetGameMasterLevel() >= BASIC_PLAYERMASTER_LEVEL )
		siAccessDeny = IPACCESSDENY_ACCESS;

	switch(siAccessDeny)
	{
	case IPACCESSDENY_ACCESS:	{		bPass = true;		}		break;
	case IPACCESSDENY_DENY:		{		bPass = false;		}		break;
	case IPACCESSDENY_NONE:
		{
			bPass = true;

			if ( pszCounteyCode != NULL)
			{
				// ���� ���� ����
				if ( pclClient->siServiceArea == ConstServiceArea_English )
				{
					// �۷ι��� ��� 
					if( _tcscmp( pszCounteyCode, CONST_COUNTRYCODE_JAPAN ) == 0 ||
						_tcscmp( pszCounteyCode, CONST_COUNTRYCODE_KOREA ) == 0 )
					{
						bPass = false;
					}
				}
				// ���� ���� ����
				else if ( pclClient->siServiceArea == ConstServiceArea_NHNChina )
				{
					if( _tcscmp( pszCounteyCode, CONST_COUNTRYCODE_JAPAN ) == 0 ||
						_tcscmp( pszCounteyCode, CONST_COUNTRYCODE_KOREA ) == 0 )
					{
						bPass = false;
					}
				}
				else if ( pclClient->siServiceArea == ConstServiceArea_EUROPE )
				{
					if( _tcscmp( pszCounteyCode, CONST_COUNTRYCODE_JAPAN ) == 0 ||
						_tcscmp( pszCounteyCode, CONST_COUNTRYCODE_KOREA ) == 0 )
					{
						bPass = false;
					}
				}
			}
		}
		break;
	}

	if(bPass == false)
	{
		SendServerResponseMsg(0, SRVAL_NOTCORRECTIP,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

	//---------------------------------------------------
	// ������� Ȯ���Ѵ�. 
	//---------------------------------------------------
	if(pclMsg->siResult == 0)return ;

	// ���� ĳ���Ͱ� �ý����� �ƴѰ��, (ĳ���� �� �����ΰ��.) 
	if(pclCM->CR[id]->GetKind() != KIND_SYSTEM)
	{
		// ���� ĳ���� �����̶��, �ƹ��� ó���� �����ʾƾ� �Ѵ�.
		if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->clInfo.BasicInfo.clPerson.GetPersonID())
		{
			return ;
		}

		// ������ �÷��� �ϴ� ĳ���ʹ� ����ó���� �Ѵ�.
		// ĳ���� ���� ó���� �Ѵ�. ������ ĳ���͸� �����ϴ� ���� �ƴϰ� �ʿ��� �ʱ�ȭ ��ġ�� ���Ѵ�.
		((cltCharServer*)pclCM->CR[id])->CloseAction(this, true);
	}

	// ���� ������ �뺸�Ѵ�. (ĳ���Ͱ� ��������� ���� �뺸�ؾ� �Ѵ�.) 
	SendServerInfo(id);

	if( pclMsg->clInfo.BasicInfo.uiKind < 0 || pclMsg->clInfo.BasicInfo.uiKind >= MAX_KIND_NUMBER )		return;
	if( pclKindInfoSet->pclKI[pclMsg->clInfo.BasicInfo.uiKind] == NULL )
	{
		MsgBox(TEXT("Fds39fj"), TEXT("idnum:%d index:%d personid:%d kind:%d"), 
			pclMsg->clInfo.BasicInfo.siIDNum, 
			pclMsg->clInfo.BasicInfo.uiIndex,
			pclMsg->clInfo.BasicInfo.clPerson.GetPersonID(),
			pclMsg->clInfo.BasicInfo.uiKind);

		return ;
	}

	//----------------------------------------------------
	// ������ ĳ���� ������ ������ �����Ѵ�.
	//----------------------------------------------------
	// ��� ������ Ȯ���Ѵ�. 
	SI32 gamemasterlevel = pclCM->CR[id]->GetGameMasterLevel();

	// ������ �ӽ� ĳ���͸� �����ϰ� ���ο� ĳ���͸� �����.
	pclCM->DeleteChar(id);

	//----------------------------------------------------
	// ĳ���� ������ ���� �ʵ带 �Ŵ� ĳ���͸� �����. 
	//----------------------------------------------------
	cltPersonInfo			clPersonInfo;
	cltInnerStatusInfo		clInnerStatus;
	clInnerStatus.clEnemyType.Set(ENEMYTYPE_ALLNPC | ENEMYTYPE_VILLAGE | ENEMYTYPE_GUILDWAR , 0);		// ��� NPC���� �������̴�. 

	cltIdentity				clIdentity;

	clPersonInfo.clBI.Set(&pclMsg->clInfo.BasicInfo);
	clPersonInfo.clBI.siHomeVillage	= pclMsg->siHomeVillageUnique;
	clPersonInfo.clBI.uiGMMode = 0;

	clPersonInfo.clIP.Set(&pclMsg->clInfo.IPInfo);
	WTrace( "DB_LOAD clIP: personID=%d charID=%d clIP{ lvl=%d life=%d mana=%d str=%d vit=%d exp=%I64d }",
		clPersonInfo.clBI.clPerson.GetPersonID(), pclMsg->usCharID,
		clPersonInfo.clIP.siLevel, clPersonInfo.clIP.Life, clPersonInfo.clIP.Mana,
		clPersonInfo.clIP.siStr, clPersonInfo.clIP.siVit, clPersonInfo.clIP.siExp );
	clPersonInfo.clCharSkill.Set(&pclMsg->clInfo.SkillInfo);
	clPersonInfo.clCharItem.Set(&pclMsg->clInfo.ItemInfo);
	clPersonInfo.clCharRank.Set(&pclMsg->clInfo.RankInfo);

	//clPersonInfo.clLimitDateItem.Set(&pclMsg->clInfo.clLimitDateItemInfo);

	clPersonInfo.clBank.Set(&pclMsg->clInfo.clBankInfo);				// ���� ���� ����. 
	clPersonInfo.clStock.Set(&pclMsg->clInfo.clStockInfo);				// �ֽ� ���� ����. 
	//_LEON_GUILD_STOCK
//	clPersonInfo.clGuildStock.Set(&pclMsg->clInfo.clGuildStockInfo);	// ��� �ֽ� ���� ����
	clPersonInfo.clWorldMoney.Set(&pclMsg->clInfo.clWorldMoney);

	//_LEON_GUILD_CHAT
//#if defined(_DEBUG)
	clPersonInfo.clBI.clGuildRank.Set(&pclMsg->clInfo.BasicInfo.clGuildRank);	// ���� ��� ����
//#endif

	// ����
	clPersonInfo.clBI.clBufInfo.SetBuf(&pclMsg->clInfo.BasicInfo.clBufInfo);
	
	clPersonInfo.uiGachaCnt = pclMsg->clInfo.uiGachaCnt;
	clPersonInfo.clPersonNaming.Set( &pclMsg->clInfo.clPersonNaming );

	clPersonInfo.clWorldPassport.Set( &pclMsg->clInfo.clWorldPassportForDB );

	// �޸� ���� ����
	clPersonInfo.clDormancyInfo.Set(&pclMsg->clInfo.m_clDormancyInfo);
	
	// ������ �������� ������ �ִ��� Ȯ���Ѵ�. 
	for(i =0;i < MAX_STOCK_KIND_IN_ACOUNT;i++)
	{
		SI16 villageunique = clPersonInfo.clStock.clInfo.clUnit[i].siVillageUnique;
		if(villageunique == 0)continue;
		if(pclCityHallManager->IsGeneralMeeting(villageunique))
		{
			cltLetterMsg_GeneralMeeting clletter(villageunique);
			SendLetterMsg(pclMsg->clInfo.BasicInfo.clPerson.GetPersonID(), (cltLetterHeader*)&clletter);

		}

		//#if defined(_CITYHALL_BANKRUPTCY)
		cltStrInfo* pclStrInfo = pclCityHallManager->GetStrInfo(villageunique);

		// 1�ֶ� ���� ������ �Ļ��̸� ������
		switch( pclCityHallManager->GetCityHallState( villageunique ) )
		{
		case 	CITYHALL_BEING_NORMAL		:		// ���� ���� ���� ��
			{

				if( pclStrInfo->clCityhallStrInfo.clChiefDate.GetDateVary() == 0 )	// �ű� �����̹Ƿ� �Ļ� üũ ����
					break;
				// ������� �Ӹ��Ǿ��°�
				// �迭�� �����ֱ� ������ NULL�� ����� �� ����.
				cltRank* pclrank = pclRankManager->GetRank(RANKTYPE_CITYHALL, villageunique);

				if(pclrank != NULL && pclrank->clPerson.siPersonID > 0 ) // ������� �ִٸ� �Ļ����� �Ȱ���.
					break;

				// ��¥�� ���� ��쵵 �ش�
				if( pclStrInfo->clCityhallStrInfo.clResignDate.GetDateVary() >= pclStrInfo->clCityhallStrInfo.clChiefDate.GetDateVary() )
				{
					SI32 siMyStockAmount = clPersonInfo.clStock.clInfo.GetStockAmount(villageunique);
					SI32 siTotalStockAmount = pclStrInfo->clCityhallStrInfo.siTotalStockAmount;

					if( siMyStockAmount < 1 || siTotalStockAmount < 1 ) break;

					// ���� �ֽ� 3%�̻��� ������ �־�� �Ļ� ��� ������ �޴´�.
					if( siMyStockAmount < siTotalStockAmount * CITYHALL_BANKRUPTCY_NOTIFY_STOCK_AMOUNT_RATE ) break;

					cltDate cldate1, cldate2;
					cldate1.Set( &pclStrInfo->clCityhallStrInfo.clResignDate );
					cldate2.Set( &pclStrInfo->clCityhallStrInfo.clResignDate );

					//#if defined(_DEBUG)
					//					cldate1.uiDay = cldate1.uiDay + CITYHALL_BANKRUPTCY_NOTIFY_AFTER_DAYS;
					//					cldate2.uiDay = cldate2.uiDay + CITYHALL_BANKRUPTCY_AFTER_DAYS;
					//#else
					// AddMonth �Լ��� �� ��
					cldate1.AddMonth( CITYHALL_BANKRUPTCY_NOTIFY_AFTER_MONTHS );
					cldate2.uiYear = cldate2.uiYear + CITYHALL_BANKRUPTCY_AFTER_YEARS;
					//#endif

					// ����� ���� �� 6���� ���Ķ�� 1���� �Ǹ� �Ļ� ���� 1���� �� �������� �����Ѵ�.
					if( cldate1.GetDateVary() <= pclTime->clDate.GetDateVary() && 
						cldate2.GetDateVary() > pclTime->clDate.GetDateVary() ) 
					{
						cltLetterMsg_VillageNameAndDate clletter( villageunique, &cldate2, cltLetterMsg_VillageNameAndDate::LETTERTYPE_CITYHALLBANKRUPTCY_ALERT );
						SendLetterMsg(pclMsg->clInfo.BasicInfo.clPerson.GetPersonID(), (cltLetterHeader*)&clletter );
					}
				}
			}
			break;
		}
		//#endif

	}

	clPersonInfo.clMarket.Set(&pclMsg->clInfo.clMarketInfo);			// ���� ���� ����. 
	clPersonInfo.clRealEstate.Set(&pclMsg->clInfo.clRealEstateInfo);	// �ε��� ����. 
	clPersonInfo.clHealthInfo.Set(&pclMsg->clInfo.clHealthInfo);		// �ǰ�  ����. 
	clPersonInfo.clQuestInfo.Set(&pclMsg->clInfo.clQuestInfo);			// ����Ʈ ����.

	// ���� ���� �����ε� Ÿ�� �ִٸ�, Ÿ�� ���� ���·� ��ȯ�ؾ� �Ѵ�. 
	clPersonInfo.clHorseInfo.Set(&pclMsg->clInfo.clHorseInfo);			// ������. 

	clPersonInfo.clGuiltyInfo.Set(&pclMsg->clInfo.clGuiltyInfo);		// ���� ����. 
	clPersonInfo.clDailyQuestInfo.Set(&pclMsg->clInfo.clDailyQuestInfo);// DailyQuest ����
	clPersonInfo.clDailyQuest2Info.Set(&pclMsg->clInfo.clDailyQuest2Info);
	clPersonInfo.clDailyQuest3Info.Set(&pclMsg->clInfo.clDailyQuest3Info);
	clPersonInfo.clSummonInfo.Set(&pclMsg->clInfo.clSummonInfo);		// ��ȯ�� 

	if( pclClient->IsCountrySwitch( Switch_Elixir) )
	{
		// ������ ĳ������ ������ ���� ����.
		if( pclchar )
		{
			pclchar->pcltElixir->SetEndMinuteVary(  pclMsg->clInfo.m_clElixirInfo.m_siEndMinuteVary	);	
			pclchar->pcltElixir->SetMultiple(  pclMsg->clInfo.m_clElixirInfo.m_uiMultiple );
			pclchar->pcltElixir->SetPoint(  pclMsg->clInfo.m_clElixirInfo.m_siPoint	);		
			pclchar->pcltElixir->SetStr(  pclMsg->clInfo.m_clElixirInfo.m_siStr	);		
			pclchar->pcltElixir->SetDex(  pclMsg->clInfo.m_clElixirInfo.m_siDex	);		
			pclchar->pcltElixir->SetVit(  pclMsg->clInfo.m_clElixirInfo.m_siVit	);		
			pclchar->pcltElixir->SetMag(  pclMsg->clInfo.m_clElixirInfo.m_siMag	);		
			pclchar->pcltElixir->SetHnd(  pclMsg->clInfo.m_clElixirInfo.m_siHnd	);		
			pclchar->pcltElixir->SetWis(  pclMsg->clInfo.m_clElixirInfo.m_siWis	);		

			pclchar->bElixirSwitch = pclchar->pcltElixir->IsElixir();
		}
	}
	

	// ���� ������̶�� ip�� ���� �Ѱ��� pc������ üũ�Ѵ�
#ifdef USE_HANIPCHECK

	if( pclClient->siServiceArea == ConstServiceArea_Korea )
	{
		TCHAR szBuffer[256] = {0,};

		SI32 rVal =	HanIPCheckRequest( szBuffer , 256 , pclCM->CR[id]->GetSession()->GetIP() ) ;

		if( 0 == rVal)
		{

			TCHAR szReturn[2] = {0,};

			_tcsncpy( szReturn , szBuffer , 1 );

			if( 0 == _tcscmp(szReturn , "Y"))	// ����
			{
				clPersonInfo.clBI.bPCRoom = true;
				TCHAR* pszGradeCode = NULL;	// ��� �ڵ带 ���� �д´�.
				TCHAR* pszCRMcode = NULL;
				pszGradeCode =  strtok(&szBuffer[1], "|");
				pszCRMcode	= strtok(NULL,"|");

				if( pszGradeCode != NULL )
				{
					if( 0 == _tcscmp(pszGradeCode , "G") )
					{
						clPersonInfo.clBI.sPCroomGrade = 'G';
					}
					else if( 0 == _tcscmp(pszGradeCode , "P") )
					{
						clPersonInfo.clBI.sPCroomGrade = 'P';
					}
					else if( 0 == _tcscmp(pszGradeCode , "T") )
					{
						clPersonInfo.clBI.sPCroomGrade = 'T';
					}
				}

				if(pszCRMcode != NULL)
				{
					_tcsncpy(clPersonInfo.clBI.szCRMcode, pszCRMcode ,16);
				}

			}			
			else	// pc���� �ƴ�
			{
				clPersonInfo.clBI.bPCRoom = false;				
			}

		}
		else
		{
			// pc���� �ƴ�
			clPersonInfo.clBI.bPCRoom = false;	

			// �����ڵ� üũ
			TCHAR * pszError = NULL;
			pszError =	(TCHAR*)HanIPCheckErrorString(rVal);
			if( pszError )
			{
				pclClient->pclLog->FilePrint( "Config\\PCRoomErrorLog.log", pszError );
			}
		}
	}	// if( pclClient->siServiceArea == ConstServiceArea_Korea )
	else if ( IsWhereServiceArea(ConstServiceArea_NHNChina) )
	{
		char szBuffer[256]	= { 0, };
		
		SI32 siReturnValue	= HanIPCheckRequestEx( szBuffer, sizeof(szBuffer), pclCM->CR[id]->GetSession()->GetIP(), "N" );
		if ( HAN_IPCHECK_OK == siReturnValue )
		{
			// �����ڵ� Y�� �����ϰ� ��Ÿ �÷���/CRM���� üũ���� �ʾƵ� �ȴ�
			if ( 'Y' == szBuffer[0] )
			{
				clPersonInfo.clBI.bPCRoom = true;	
			}
			else
			{
				clPersonInfo.clBI.bPCRoom = false;	
			}
		}
		else
		{
			// pc���� �ƴ�
			clPersonInfo.clBI.bPCRoom = false;	

			// ���� �α�
			TCHAR * pszError = (TCHAR*)HanIPCheckErrorString( siReturnValue );
			if( pszError )
			{
				pclClient->pclLog->FilePrint( "Config\\PCRoomErrorLog.log", pszError );
			}
		}
	}	// else if ( IsWhereServiceArea(ConstServiceArea_NHNChina) )
#endif //#ifdef USE_HANIPCHECK

	// ���� PC�� ����
	if(pclClient->IsCountrySwitch(Switch_PCRoom))
	{
		clPersonInfo.clBI.bPCRoom = true;
	}

	// ���� �ֹ��� ��쿡�� ���� �ֹε鿡�� �α��� ������ �˷��ش�.
	if ( clPersonInfo.clBI.siHomeVillage > 0 )
	{
		if ( clPersonInfo.clBI.GetName() != NULL && _tcslen( clPersonInfo.clBI.GetName() ) > 0 )
		{
			cltGameMsgResponse_ResidentLogin clGameMsgResponse_ResidentLogin( clPersonInfo.clBI.siHomeVillage, (TCHAR*)clPersonInfo.clBI.GetName() );
			cltMsg clMsg( GAMEMSG_RESPONSE_RESIDENTLOGIN, sizeof( clGameMsgResponse_ResidentLogin ), (BYTE*)&clGameMsgResponse_ResidentLogin );
			pclCM->SendAllPersonHomeMsg((sPacketHeader*)&clMsg, clPersonInfo.clBI.siHomeVillage );
		}
	}

	//----------------------------------------------------
	// �������� ��� ���������� Ȯ���Ѵ�. 
	//----------------------------------------------------
	/*	SI16 erroritemunique, errorpos;
	if(pclMsg->clInfo.ItemInfo.IsAllProperEquip(pclItemManager, &erroritemunique, &errorpos) == false)
	{
	pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("Invalid Equip(Person:%d)(item:%d)pos(%d)"), 
	pclMsg->clInfo.BasicInfo.clPerson.siPersonID,
	erroritemunique, errorpos);
	return ;
	}
	*/


	// ���ǰ�� ��踦 ����. 
	for(i = 0; i < PERSONITEM_INV0;i++)
	{
		if(pclMsg->clInfo.ItemInfo.clItem[i].siUnique)
		{
			pclItemManager->AddEquipItemNum(pclMsg->clInfo.ItemInfo.clItem[i].siUnique, 1);

			// ���ο� ��� - ���ǰ�� ��踦 ����.
			//NStatistics_Add("Item", pclMsg->clInfo.ItemInfo.clItem[i].siUnique, "EquipNum", 1);

		}
	}

	{
		cltGameMsgResponse_SetVillageUpgrade sendMsg( &siTopLevelVillageUnique[0] );
		cltMsg clMsg( GAMEMSG_RESPONSE_SETVILLAGEUPGRADE, sizeof( sendMsg ), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
	}

	// ������ ������� ���忡 ���� ��踦 ����. (30�̻��϶��� )
	if(clPersonInfo.clIP.GetLevel() >= 30 )
	{
		if( pclCM->CR[id]->ActiveWeapon >= 0 && pclCM->CR[id]->ActiveWeapon < MAX_ITEM_PER_PERSON )
			pclCM->AddUserWeapon(&clPersonInfo.clCharItem.clItem[pclCM->CR[id]->ActiveWeapon]);
	}

	//---------------------------------------------------
	// ������ 10�̻��ε� �ʺ��� �����忡 �ֵ��� ,, ����
	//---------------------------------------------------
	if(clPersonInfo.clIP.GetLevel()  >= MAX_LEVEL_FOR_BEGINNERMAP && clPersonInfo.clBI.GetMapIndex() == MAPINDEX_BEGINNER1)
	{
		// �Ѿ����� �̵��Ѵ�. 

		SI32 villageunique = 1;
		SI32 maptypeunique = pclMapManager->pclMapArray[villageunique]->siMapUnique;
		SI32 x = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
		SI32 y = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

		clPersonInfo.clBI.SetMapIndex(villageunique);
		clPersonInfo.clBI.SetX(x);
		clPersonInfo.clBI.SetY(y);
	}

	// ���� ���� ������ �ִ� ���̶��..
	SI32 userLimit = pclClient->pclMapManager->GetUserLimit( clPersonInfo.clBI.GetMapIndex() );
	if ( userLimit > 0 &&
		 pclClient->pclMapManager->IsInstanceMap(clPersonInfo.clBI.GetMapIndex()) == false )
	{
		if ( pclClient->pclMapManager->GetUserCount(clPersonInfo.clBI.GetMapIndex()) >= userLimit )
		{
			// Ŭ�и��� �ִ��� Ȯ���Ѵ�.
			SI32 cloneMapIndex = pclClient->pclMapManager->GetCloneMap( clPersonInfo.clBI.GetMapIndex() );
			if ( cloneMapIndex )
			{
				clPersonInfo.clBI.SetMapIndex(cloneMapIndex);
			}
		}
	}

	//---------------------------------------------------
	// ��Ʋ�ξ� �ʿ� �ִٸ�
	//---------------------------------------------------
	if( ( clPersonInfo.clBI.GetMapIndex() ==  MAPINDEX_BATTLEROYAL_MABRIUM )||
		( clPersonInfo.clBI.GetMapIndex() ==  MAPINDEX_BATTLEROYAL_GRANDMABRIUM ) )
	{
		clPersonInfo.clBI.SetMapIndex(MAPINDEX_MAINFIELD);
		clPersonInfo.clBI.SetX(499);
		clPersonInfo.clBI.SetY(258);
	}

	//---------------------------------------------------
	// ���� �Ⱓ�� �ƴ϶��
	//---------------------------------------------------
	if( !pclEventTimeManager->InEventTime("FestivalEvent", &sTime) &&  MAPINDEX_FESTIVAL == clPersonInfo.clBI.GetMapIndex() )
	{
		clPersonInfo.clBI.SetMapIndex(MAPINDEX_MAINFIELD);
		clPersonInfo.clBI.SetX(288);
		clPersonInfo.clBI.SetY(354);
	}

	//---------------------------------------------------
	// ����� �ʵ忡 �ִٸ�!
	//---------------------------------------------------
	if( IsCountrySwitch( Switch_NewGuildHuntMapWar ) )	// ������
	{
		if( IsGuildWarMap_Use_New_GuildWar( clPersonInfo.clBI.GetMapIndex() )  || 
			IsGuildWarMap_NEW( clPersonInfo.clBI.GetMapIndex() ) )
		{
			clPersonInfo.clBI.SetMapIndex(MAPINDEX_PALACE);
			clPersonInfo.clBI.SetX(26);
			clPersonInfo.clBI.SetY(72);
		}
	}
	else	// ������ 
	{
		if( IsGuildWarMap_Not_Use_New_GuildWar( clPersonInfo.clBI.GetMapIndex() ) )
		{
			clPersonInfo.clBI.SetMapIndex(MAPINDEX_PALACE);
			clPersonInfo.clBI.SetX(26);
			clPersonInfo.clBI.SetY(72);
		}
	}
	if( IsCountrySwitch( Switch_PKHuntMap_Change_Info ) )	// ������
	{
		if( pclMapManager->IsPkMap(clPersonInfo.clBI.GetMapIndex()  ))	// pk ���̸� �����ؼ� ���ƿö� ���縮�� 
		{
			clPersonInfo.clBI.SetMapIndex(MAPINDEX_PALACE);
			clPersonInfo.clBI.SetX(26);
			clPersonInfo.clBI.SetY(72);
		}
	}
#ifdef USE_PROMOTOR_SERVER
	// ���θ��� ������ ������ �λ縻�� �����ϱ� ���ؼ�.
	if( clPersonInfo.clIP.GetLevel() <= 1 )
	{
		clPromoterInfo PromoterInfo( id, clPromoterInfo::PROMOTER_INTRODUCE );
		PromoterInfo.SetName( (TCHAR*)clPersonInfo.clBI.GetName() ); 
		PromoterInfo.SetAccountID( clPersonInfo.clBI.szAccountID );
		pclPromoterManager->Push( PromoterInfo );
	}
#endif
	//---------------------------------------------------
	// ������ 10�̻��ε� �ʺ��� �����忡 �ֵ��� ,, ����
	//---------------------------------------------------
	//if(clPersonInfo.clIP.GetLevel()  >= MAX_LEVEL_FOR_BEGINNERMAP && clPersonInfo.clBI.GetMapIndex() == MAPINDEX_BEGINNER2)
	//{
	//	// �����ַ� �̵��Ѵ�. 

	//	SI32 villageunique = 69;
	//	SI32 maptypeunique = pclMapManager->pclMapArray[villageunique]->siMapUnique;
	//	SI32 x = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
	//	SI32 y = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;

	//	clPersonInfo.clBI.SetMapIndex(villageunique);
	//	clPersonInfo.clBI.SetX(x);
	//	clPersonInfo.clBI.SetY(y);
	//}

	//---------------------------------------------------
	// �������̸� ����� ������ ����. 
	//---------------------------------------------------
	if(pclMapManager->IsHuntMap(clPersonInfo.clBI.GetMapIndex()) == true)
	{
		SI32 villunique = clPersonInfo.clBI.GetMapIndex() - 200;
		if(villunique >= 0 && villunique < MAX_VILLAGE_NUMBER && pclVillageManager->pclVillageInfo[villunique])
		{
			SI32 maptypeunique = pclMapManager->pclMapArray[villunique]->siMapUnique;
			SI32 x = -1;
			SI32 y = -1;
			if( maptypeunique >= 0 && maptypeunique < MAX_MAPTYPE_NUMBER )
			{
				x = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
				y = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;
			}

			if( x > 0 && y > 0)
			{
				// ����� ������ ����.
				clPersonInfo.clBI.SetMapIndex(villunique);
				clPersonInfo.clBI.SetX(x);
				clPersonInfo.clBI.SetY(y);
			}
		}
	}
	//--------------------------------------------
	// �ν��Ͻ� �����̸� �溹������ ����������.
	//---------------------------------------------
	else if ( pclMapManager->IsInstanceMap(clPersonInfo.clBI.GetMapIndex()) == true )
	{
		SI32 mapindex = 0 ;
		SI32 centerx = 0 ;
		SI32 centery = 0 ;
		//if ( siServiceArea == ConstServiceArea_China )
		//{
		//	// ���������� ���� �̵���Ų��.
		//	mapindex = MAPINDEX_PALACE;		// �溹�� 
		//	centerx  = 26;
		//	centery  = 72;
		//}
		//else
		//{
		//	if ( pclCM->CR[id]->pclKI->siDefaultCountry == ConstServiceArea_China )
		//	{
		//		// ���������� ���� �̵���Ų��.
		//		mapindex = MAPINDEX_CHINABORDER;		// ���漱
		//		centerx  = 17;
		//		centery  = 19;
		//	}
		//	else
		//	{
				// ���������� ���� �̵���Ų��.
				mapindex = MAPINDEX_PALACE;		// �溹�� 
				centerx  = 26;
				centery  = 72;
		//	}
		//}

		clPersonInfo.clBI.SetMapIndex(mapindex);
		clPersonInfo.clBI.SetX(centerx);
		clPersonInfo.clBI.SetY(centery);
	}
	//-------------------------------------------------
	// ������ �ִ� ���¸�, 
	//-------------------------------------------------
	if(clPersonInfo.clGuiltyInfo.siLeftPrisonSecond)
	{
		clPersonInfo.clBI.SetMapIndex(PRISON_MAPINDEX);
		clPersonInfo.clBI.SetX(PRISON_X);
		clPersonInfo.clBI.SetY(PRISON_Y);

		clInnerStatus.SetStatus(INNERSTATUS_PRISON);
	}

	//[����] ��ƼȦ. ��Ƽ�忡 ������ ���縮���� ������ ������Ų��. => 2008-4-2
	if(IsPC(id))
	{
		for(SI32 MapCount = 0; MapCount < MAX_PARTYHALL_MAP; ++MapCount)
		{
			// ���� �ʱ�ȭ �Ǵ� ĳ���Ͱ� ��Ƽ�忡�� ������ �Ǹ� ���縮���� ������Ų��.
			if(clPersonInfo.clBI.GetMapIndex() == PARTYHALL_START_MAP_INDEX + MapCount)
			{
				clPersonInfo.clBI.SetMapIndex(MAPINDEX_PALACE);
				clPersonInfo.clBI.SetX(26);
				clPersonInfo.clBI.SetY(72);
			}
		}
	}

	//[����] ���� �̺�Ʈ. �̺�Ʈ ����Ⱓ ���� �ƴ� �����ε� �ش� �ʿ� �ִٸ� ���縮���� �̵� ��Ų��. => 2008-7-1
	if( IsCountrySwitch( Switch_Server_Event ) )
	{
		if( false == pclEventTimeManager->InEventTime("GoonzuDay_Week_Reward", &sTime) )
		{
			if( true == pclServerEventManager->IsEventMap( clPersonInfo.clBI.GetMapIndex() ) )
			{
				clPersonInfo.clBI.SetMapIndex( MAPINDEX_PALACE );
				clPersonInfo.clBI.SetX(26);
				clPersonInfo.clBI.SetY(72);
			}
		}
	}


	// ���ֵ��� �̺�Ʈ ���̶�� Ŭ���̾�Ʈ���� �˷��ݴϴ�.
	/*if(pclEventTimeManager->InEventTime("GoonzuDay_Week", &sTime) == true)
	{
		cltGameMsgResponse_EventTime_Update sendMsg("GoonzuDay_Week", true);
		cltMsg clMsg(GAMEMSG_REQUEST_EVENTTIME_UPDATE, sizeof(sendMsg), (BYTE*)&sendMsg);

		cltCharServer* pclchar = pclClient->pclCM->GetCharServer(id);
		if(pclchar)
		{
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}*/

	//---------------------------------------------------
	// �ش� ������ �������� ������ ���縮��(�Ѿ�)���� �̵�
	//---------------------------------------------------
	switch ( clPersonInfo.clBI.GetMapIndex() )
	{
		case Const_MapIndex_PollMap:		// ��ǥ�� ����
		case MAPINDEX_SERVEREVENT_LOBBY:	// �����̺�Ʈ ���� �κ�
		case MAPINDEX_SANTAFACTORY_1:		// ������ ��Ÿ �۾���.
		case MAPINDEX_SANTAFACTORY_2:		// û���� ��Ÿ �۾���.
		case MAPINDEX_SANTAFACTORY_3:		// ��� ��Ÿ �۾���.
		case MAPINDEX_SANTAFACTORY_1_2:		// ������ ��Ÿ �۾���.
		case MAPINDEX_SANTAFACTORY_2_2:		// û���� ��Ÿ �۾���.
		case MAPINDEX_SANTAFACTORY_3_2:		// ��� ��Ÿ �۾���
		case MAPINDEX_WEDDINGHALL:			// �����忡 ������
			{
				clPersonInfo.clBI.SetMapIndex(MAPINDEX_PALACE);
				clPersonInfo.clBI.SetX(26);
				clPersonInfo.clBI.SetY(72);
			}
			break;
	}


	//---------------------------------------------------
	// �� ������ �ƴ� ���� �ִٸ� ����� ������ �̵��Ѵ�. 
	//---------------------------------------------------
	SI32 x = 0, y = 0;
	SI32 mapindex = 0;

	//--------------------------------------------------
	// �������� �� ������ �ִٸ�, 
	//--------------------------------------------------
	if(pclMapManager->IsLandMap( clPersonInfo.clBI.GetMapIndex() ) == true)
	{
		x	= clPersonInfo.clBI.clPos.siX;
		y	= clPersonInfo.clBI.clPos.siY;
		mapindex	= clPersonInfo.clBI.clPos.siMapIndex;

		bool bGetPos = false;
		if( mapindex >= 0 && mapindex < MAX_MAP_NUMBER )
			bGetPos = pclMapManager->pclMapArray[clPersonInfo.clBI.clPos.siMapIndex]->FindEmptyAreaForCharLocate(clPersonInfo.clBI.uiKind, clPersonInfo.clBI.clPos.siX, clPersonInfo.clBI.clPos.siY, 30, &x, &y, false);

		if( bGetPos == false )
		{
			//------------------------------------
			// ���нÿ��� �Ѿ翡�� ������ �Ѵ�. 
			//------------------------------------
			SI32 villageunique = 1;
			SI32 maptypeunique = pclMapManager->pclMapArray[villageunique]->siMapUnique;

			x = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
			y = pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;
		}
	}
	else
	{
		pclLog->FilePrint( TEXT("Config\\ErrorLog.log"), TEXT("No Land Area:%d id:%d"),
			clPersonInfo.clBI.GetMapIndex(), id);

	}
	// ĳ���͸� ��ġ�ϱ� ������ ������ Ȯ���Ѵ�. 
	clPersonInfo.clBI.clPos.siMapIndex	= mapindex;
	clPersonInfo.clBI.clPos.siX			= x;
	clPersonInfo.clBI.clPos.siY			= y;

	SI32 childcharunique	= 0;
	SI32 summonindex		= 0;
	SetChar1(id, pclCM->CR[id]->GetCharUnique(), childcharunique, summonindex, &clIdentity, &clPersonInfo, &clInnerStatus, 0);

	//cltCharServer* pclcchar = (cltCharServer*)pclCM->CR[id];

	// [����] �������� ���� 
	if(clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK))
	{
		pclCM->CR[id]->pclCI->clBI.clBufInfo.m_bNoFirstAttackPreminum = true;
	}
	else
	{
		pclCM->CR[id]->pclCI->clBI.clBufInfo.m_bNoFirstAttackPreminum = false;
	}
	
	// Ȯ���ص� ��� ������ �����Ѵ�. 
	pclchar->SetGameMasterLevel(gamemasterlevel);
	pclchar->SetRareItemRate(0,0,0,0);
	pclchar->SetWennyOldMan(false,false);

	// �α����� 10�ʰ� ��ȣ�Ѵ�.
	pclchar->SetInitProtectSwitch();

	//---------------------------------------
	// HashTable�� PERSON �̸��� �������� �����Ѵ�.
	//---------------------------------------
	if(pclHashTable->Insert((TCHAR *)pclCM->CR[id]->GetName(), pclCM->CR[id]->pclCI->GetPersonID(), id)==false)
	{
		pclLog->FilePrint( TEXT("Config\\ErrorLog.log"), TEXT("pclHashTable->Insert name:%s id:%d"),
			(TCHAR *)pclCM->CR[id]->GetName(), id);
	}
	if(pclPersonHashTable->Insert(pclCM->CR[id]->pclCI->GetPersonID(), id)==false)
	{
		pclLog->FilePrint( TEXT("Config\\ErrorLog.log"), TEXT("pclPersonHashTable->Insert personid:%d id:%d"),
			pclCM->CR[id]->pclCI->GetPersonID(), id);
	}

	if ( (id != pclCM->GetIDFromName((TCHAR *)pclCM->CR[id]->GetName()))
		|| (id != pclCM->GetIDFromPersonID(pclCM->CR[id]->pclCI->GetPersonID()))
		)
	{
		pclchar->bCloseSocketSwitch	= true;
		pclchar->dwCloseSocketTimer	= 5;

		pclLog->FilePrint( TEXT("Config\\BugChar_Login.txt"), TEXT("PersonID:%d"), pclCM->CR[id]->pclCI->GetPersonID() );
	}

	//---------------------------------------
	// �� Person�� ������ ����ڿ��� ������. 
	//---------------------------------------
	cltOrderDataInfo clOrder;
	clOrder.Make(ORDER_STOP, BY_PLAYER, 0, 0, 0, 0);


	//------------------------------------------
	// ĳ������ ������ Ȯ��.
	//------------------------------------------	
	cltPItemInfo * pclcharitem = &pclCM->CR[id]->pclCI->clCharItem ;
	NDate ndate;
	ndate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);
	SI32 todaydatevary = ndate.GetDateVary() ;

	for ( i = 0 ; i < MAX_ITEM_PER_PERSON ; i ++ )
	{
		SI32 itemunique = pclcharitem->clItem[i].siUnique ;
		if ( itemunique > 0 )
		{
			if ( pclcharitem->clItem[i].IsHaveUseDay() )  //[����] ���� ���Ⱓ 3�Ϲ̸� �˸�
			{
				SI32 useableDay = 0;
				BOOL rVal = GetItemUseAbleDay(&pclcharitem->clItem[i],&useableDay);
				
				if(pclcharitem->clItem[i].IsSealingUp(pclItemManager) == true)
					continue;

				if(rVal == FALSE) continue;
							
				if(useableDay >= 3) continue; // ��밡�ɱⰣ�� 3�� �̻� �˳��ϸ� �н� 
				else if(useableDay >= 0)		 //  3�� �̸������� ��밡�� �Ⱓ�� �����ִٸ� Ŭ���̾�Ʈ�� ���
				{
					cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclcharitem->clItem[i].siUnique, useableDay);
					cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
				}
				else if( useableDay < 0) //���Ⱓ�� ������ �����ش� �׸��� Ŭ���̾�Ʈ�� �˸�.
				{
					//KHY - 0723 - �Ⱓ������ �ı� ����.
					if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
					{

					}
					else
					{
						// ����ǰ�� �������� �ʴ´�. - by LEEKH
						if( pclcharitem->clItem[i].clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5 )
						{
							sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),i,&pclcharitem->clItem[i],USEITEM_REASON_DATEOVER);
							pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
												
							cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclcharitem->clItem[i].siUnique, useableDay);
							cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
							pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );				
						}
					}
				}
			}
		}
	}//( i = 0 ; i < MAX_ITEM_PER_PERSON ; i ++ )

	//------------------------------------------	
	// �ܺ� ���. 
	//--------------------------------------------------
	// ������� ���� ��� ���¸� üũ�Ѵ�. 
	//--------------------------------------------------
	pclchar->CheckWeaponSkillScroll(pclItemManager);

	cltShapeEquipInfo clequip;
	pclchar->GetShapeEquipInfo(&clequip);


	// ���� �����ִ� ���� ������ ���Ѵ�. 
	SI32 horseunique = 0;
	cltHorse* pclhorse = clPersonInfo.clHorseInfo.GetCurrentShowHorse();
	SI32 addstatusdate = 0 ;
	SI32 ticketnum = 0 ;

	SI32 PremiumDate_slot1 = 0 ;
	SI32 PremoiumUniuqe_Slot1 = 0 ;
	SI32 PremiumDate_slot2 = 0 ;
	SI32 PremoiumUniuqe_Slot2 = 0 ;
	SI32 PremiumDate_slot3 = 0 ;
	SI32 PremoiumUniuqe_Slot3 = 0 ;
	SI32 PremoiumPartsNum = 0 ;


	if(pclhorse)
	{
		horseunique = pclhorse->siHorseUnique;
		addstatusdate = pclhorse->clAddStatusDate.GetDateVary() ;
		ticketnum = pclhorse->siStatusTicketNum ;

		PremiumDate_slot1 = pclhorse->clPremiumPartsDate_Slot1.GetDateVary() ;
		PremoiumUniuqe_Slot1 = pclhorse->siPremiumUnique_Slot1 ;
		PremiumDate_slot2 = pclhorse->clPremiumPartsDate_Slot2.GetDateVary() ;
		PremoiumUniuqe_Slot2 = pclhorse->siPremiumUnique_Slot2 ;
		PremiumDate_slot3 = pclhorse->clPremiumPartsDate_Slot3.GetDateVary() ;
		PremoiumUniuqe_Slot3 = pclhorse->siPremiumUnique_Slot3 ;

		PremoiumPartsNum = pclhorse->siPremiumPartsNum	;

		// ���� ���� �����Ͱ� ���ٸ�, Ÿ�� ���� �� ����.
		if(horseunique <= 0 )
		{
			clPersonInfo.clHorseInfo.siCurrentHorseRIDEHORSE = false;
		}

		//KHY - 1111 - ������� �߰�.		
		if((horseunique == HORSEUNIQUE_MOTORCYCLE) && (clPersonInfo.clHorseInfo.siCurrentHorseRIDEHORSE == TRUE))
		{
			// ������̿� Ÿ������ �ʿ��� ������ �ý��� Ȯ��
			if( pclchar->pclCI->clBI.clTimeMeterRateInfo.GetLeftTime( TIMEMETERRATE_MOTORCYCLE ) <= 0)
			{
				// ���ᰡ �����մϴ�.������ �ֽʽÿ�.
				//return;
			}
			else
			{
				// ������̿� Ÿ�� ������ ����.
				if( pclchar->pclCI->clBI.clTimeMeterRateInfo.UseStart( TIMEMETERRATE_MOTORCYCLE ) == false)
				{
					//return;
				}
			}

			// ȿ���� �ִٸ� ���⼭ �־��ش�.
		}
	}

    if (clPersonInfo.clBI.bPCRoom)	
	{
		((cltServer*)pclClient)->siPCRoomCharNum++;
	}

	if( pclGuildManager->siBeforeYearTopGuildUnique > 0 &&
		clPersonInfo.clBI.clGuildRank.IsGuildMember() &&
		clPersonInfo.clBI.clGuildRank.GetGuildUnitIndex() == pclGuildManager->siBeforeYearTopGuildUnique )
		pclchar->uiBestGuild = 1;

	SI32 siBufTransformKind = 0;
	if( clPersonInfo.clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM ) )
	{
		stBuf* pclBuf = clPersonInfo.clBI.clBufInfo.GetBuf( BUF_TRANSFORM );
		if(pclBuf)			siBufTransformKind = pclBuf->siPercent;
	}

	//// PCK : ���� ���� (Ż�ͺ���)
	//if( clPersonInfo.clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM_HORSE ) )
	//{
	//	stBuf* pclBuf = clPersonInfo.clBI.clBufInfo.GetBuf( BUF_TRANSFORM_HORSE );
	//	if(pclBuf)			siBufTransformKind = pclBuf->siPercent;
	//}

	// ������� ���� - ������
	UI08 uiGetEmblem = 0;
	if ( (PERSONITEM_WEAPON1 == pclchar->ActiveWeapon) || (PERSONITEM_WEAPON2 == pclchar->ActiveWeapon) )
	{
		SI32 siItemType		= pclchar->pclCI->clCharItem.GetWeaponType( pclClient->pclItemManager, pclchar->ActiveWeapon );
		SI32 siSkillUnique	= pclSkillManager->m_clMasterManager.GetSkillUseWeaponFromItemType( siItemType );
		if ( (0 < siSkillUnique) && (MAX_SKILL_NUMBER > siSkillUnique) )
		{
			SI32 siSkillLevel = pclchar->pclCI->clCharSkill.GetSkillExpLevel( siSkillUnique );
			
			uiGetEmblem = pclSkillManager->m_clMasterManager.GetEmblemIndex( siItemType, siSkillLevel );
			if ( (SKILLMSTER_WEAPONTYPE_NONE< uiGetEmblem) && (SKILLMSTER_WEAPONTYPE_MAX > uiGetEmblem) )
			{
				pclchar->m_uiWeaponSkillMasterEmblem = uiGetEmblem;
			}
		}
	}

	//KHY - 1001 - ��µ��� 5���� ���� ����.
	cltMakeCharInfo clMakeCharInfo(	clPersonInfo.clBI.GetKind(), 
									pclCM->CR[id]->GetCharUnique(),		
									clPersonInfo.clIP.GetLevel(),		
									clPersonInfo.clIP.GetLife(), 
									clPersonInfo.clBI.GetX(), 
									clPersonInfo.clBI.GetY(), 
									clPersonInfo.clBI.GetMapIndex(),
									childcharunique, 
									summonindex, 
									0,
									0, 
									clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_NOFIRSTATTACK) | clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_HINAMOCHI) | clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_EVENT_EGGMOONCAKE) | clPersonInfo.clBI.clBufInfo.IsHaveBufExecptGetType(BUF_TRANSFORM, BUF_GET_TYPE_NIGHTMARKET),
									0,
									clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_LOVEPOWER) && clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_FRIENDSHIP),
									clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_FOURLEAF) ,clPersonInfo.clBI.clBufInfo.IsHaveBuf(BUF_FARMINGNMININGFOREVER) ,
									0,
									clPersonInfo.clBI.uiSex,
									&clequip,
									clPersonInfo.clHorseInfo.siCurrentShowHorse,
									0,
									0,  gamemasterlevel, pclMapManager->pclMapArray[pclCM->CR[id]->GetMapIndex()]->siMapUnique,
									horseunique,									
									clPersonInfo.clBI.siHomeVillage,
									clPersonInfo.clBI.siSchoolUnique,
									&clPersonInfo.clCharRank,addstatusdate,ticketnum,
									&clOrder,
									&clIdentity, 
									&clInnerStatus,
									0,
									0,
									clPersonInfo.clBI.uiGMMode  ,
									clPersonInfo.clBI.bPCRoom  ,
									0,
									&clPersonInfo.clBI.clGuildRank,
									&clPersonInfo.clPersonNaming,
									clPersonInfo.clHorseInfo.siCurrentHorseRIDEHORSE,
									0,
									false,
									pclchar->uiBestGuild,	// [�߰� : Ȳ���� 2008. 1. 14 => �ڽ��� ��尡 �ְ��ΰ�.]
									false,
									false,
									siBufTransformKind,
									0,
									NULL,
									false
									,NULL
									,PremiumDate_slot1
									,PremoiumUniuqe_Slot1
									,PremiumDate_slot2
									,PremoiumUniuqe_Slot2
									,PremiumDate_slot3
									,PremoiumUniuqe_Slot3
									,PremoiumPartsNum
									,uiGetEmblem
									,0	// �̰������� ��ȥ ������ �޾ƿ��� �ʱ� ������ ���߿� �޾ƿö� �����Ѵ�
									,0	// ó������ ��ȣ������ ��ȯ�� ���°� �ƴϱ� ������ 0���� �ص� ����
									,false
									,&clPersonInfo.clBI.clBufInfo);
	if (pclWarRankManager)
	{
		if (pclWarRankManager->IsRankMVPFromPersonID(pclchar->pclCI->GetPersonID()))
		{
			pclchar->pclCI->clBI.uiGMMode = GMMODE_ATB_WARMVP;
			pclchar->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
		}
	}

	if( pclBlackWarManager )
	{
		if( true == pclBlackWarManager->IsMVPPersonID( pclchar->pclCI->GetPersonID() ) )
		{
			pclchar->pclCI->clBI.uiGMMode = GMMODE_ATB_WARMVP;
			pclchar->SetUpdateSwitch(UPDATE_BASICINFO, true, 0 );
		}
	}

	// APEX-
#ifdef _CHINA
	if( false == pclchar->bApexLogin )
	{
		NoticeApexProxy_UserLogin( id, pclchar->pclCI->clBI.szAccountID );
		pclchar->bApexLogin = true;
	}
#endif
	// -APEX

	cltMsg clMsg(GAMEMSG_RESPONSE_LOGIN, sizeof(clMakeCharInfo), (BYTE*)&clMakeCharInfo);

	// Ŭ���̾�Ʈ���� ���ο� ������� ������ �뺸�Ѵ�. 
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	// ���θ��� ���� �ʱ�ȭ.
	if( pclPromoterManager )
	{
		clPromoterInfo PromoterInfo( id, clPromoterInfo::GET_PROMOTER_INFO );
		pclPromoterManager->Push( PromoterInfo );
	}
	


	// �α��� �α�
	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_LOGIN, LOGCOMMAND_INDEX_LOGIN, 
						 0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
						 NULL, NULL);

	
	//KHY - 0906 - ���ӹ� �����̺�Ʈ. �Ⱓ ����. 
	if (pclClient->IsCountrySwitch(Switch_GameRoomCouponEvent) )
	{		
		_SYSTEMTIME sStartEventDay , sEndEventDay;

		ZeroMemory(&sStartEventDay,sizeof(_SYSTEMTIME));
		ZeroMemory(&sEndEventDay,sizeof(_SYSTEMTIME));

		sStartEventDay.wYear = 2007;
		sStartEventDay.wMonth = 9;
		sStartEventDay.wDay = 13;
		sStartEventDay.wHour = 0;
		sStartEventDay.wMinute = 0;
     

		sEndEventDay.wYear = 2007;
		sEndEventDay.wMonth = 10;
		sEndEventDay.wDay = 4;
		sEndEventDay.wHour = 0;
		sEndEventDay.wMinute = 0;

		SI32 startEventdatevary = pclClient->GetMinuteFromsTime(sStartEventDay);
		SI32 endEventdatevary = pclClient->GetMinuteFromsTime(sEndEventDay);
		
		_SYSTEMTIME fTimeDatevary = pclMsg->clInfo.BasicInfo.GetFirstCreatTime(); //���� ���� ���� �ð�.

		SI32 firstCreatTimedatevary = pclClient->GetMinuteFromsTime(fTimeDatevary);

		//KHY - 0906 - ���ӹ� �����̺�Ʈ. �Ⱓ �˻�. 
		if((startEventdatevary <= firstCreatTimedatevary)&&(endEventdatevary >= firstCreatTimedatevary))
		{

			TCHAR FullURL[100] = {'\0', };

#ifdef USE_HANRTA
			if(siServiceType == SERVICE_REAL)
			{
				StringCchPrintf(FullURL, sizeof(FullURL),
					"http://gzs.hangame.com/event/promotion.nhn?m=isPCBangIP&ip=%s", pclchar->GetSession()->GetIP());
			}
			else
			{
				StringCchPrintf(FullURL, sizeof(FullURL),
					"http://alpha-gzs.hangame.com/event/promotion.nhn?m=isPCBangIP&ip=%s", pclchar->GetSession()->GetIP());
			}
#endif // USE_HANRTA

			SendServerResponseCharMsg(0, SRVAL_CHECK_WEBVAL, FullURL, NULL, pclCM->CR[id]->GetCharUnique());

			//KHY - 0906 - ���ӹ� ���� �ƴ���.
			//if( ) //���ӹ��� ��쿡��. (��ī �޴���.)
			//{
				//KHY - 0906 -�ش������ �ش��̺�Ʈ Ÿ�Կ� ���� ������ �߱޿��θ� ��û�Ѵ�.
				//sDBRequest_GetEventCouponInfo clMsg3( pclCM->CR[id]->pclCI->GetPersonID() , GAMEROOM_COUPON_EVENT);
				//SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg3);
				
				//SendLog_SetEventCouponInfo(pclchar, GAMEROOM_COUPON_EVENT);

			//}	
		}	
	}
	if (pclClient->IsCountrySwitch(Switch_AuctionEvent)) //�����ڵ�� ����. �����̺�Ʈ [2007.09.10 �ռ���]
	{
		//�����̺�Ʈ ���� ����������.������ �̺�Ʈ �Ⱓ�� ���� �������� Ȯ���ؼ����ʸ� �����߱�[2007.09.10 �ռ���]
		cltServer* pclServer = (cltServer*)pclClient;
		_SYSTEMTIME sStartAEventDay , sEndAEventDay;//�̺�Ʈ ���۽ð��� ������ �ð��α��� �ð�.

		sStartAEventDay.wYear = 2007;
		sStartAEventDay.wMonth = 9;
		sStartAEventDay.wDay = 13;
		sStartAEventDay.wHour = 0;
		sStartAEventDay.wDayOfWeek = 0;
		sStartAEventDay.wMinute = 0;
		sStartAEventDay.wSecond = 0;
		sStartAEventDay.wMilliseconds=0;

		sEndAEventDay.wYear = 2007;
		sEndAEventDay.wMonth = 10;
		sEndAEventDay.wDay = 4;
		sEndAEventDay.wHour = 0;
		sEndAEventDay.wDayOfWeek = 0;
		sEndAEventDay.wMinute = 0;
		sEndAEventDay.wSecond = 0;
		sEndAEventDay.wMilliseconds=0;

		SI32 startAEventdatevary = pclClient->GetMinuteFromsTime(sStartAEventDay);
		SI32 endAEventdatevary = pclClient->GetMinuteFromsTime(sEndAEventDay);
		SI32 LoginTime = pclClient->GetMinuteFromsTime(pclServer->sTime);


		if((LoginTime <= endAEventdatevary)
			&&(LoginTime >= startAEventdatevary))
			//�̺�Ʈ �Ⱓ���� �����̸� �����߱�.
		{
			SendLog_SetEventCouponInfo(pclchar, AUCTION_COUPON_EVENT,0);
		}
	}

	//KHY - 1025 - ������ �̺�Ʈ - �Ⱓ����. - 1�Ⱑ�̿��.
	//by LEEKH 2007.10.30 - �Ⱓ ����
	if (pclClient->IsCountrySwitch(Switch_NowComEvent)) 
	{
		cltServer* pclServer = (cltServer*)pclClient;
		_SYSTEMTIME sStartAEventDay , sEndAEventDay;//�̺�Ʈ ���۽ð��� ������ �ð��α��� �ð�.

		sStartAEventDay.wYear = 2006;
		sStartAEventDay.wMonth = 11;
		sStartAEventDay.wDay = 8;
		sStartAEventDay.wHour = 0;
		sStartAEventDay.wDayOfWeek = 0;
		sStartAEventDay.wMinute = 0;
		sStartAEventDay.wSecond = 0;
		sStartAEventDay.wMilliseconds=0;

		sEndAEventDay.wYear = 2006;
		sEndAEventDay.wMonth = 12;
		sEndAEventDay.wDay = 5;
		sEndAEventDay.wHour = 0;
		sEndAEventDay.wDayOfWeek = 0;
		sEndAEventDay.wMinute = 0;
		sEndAEventDay.wSecond = 0;
		sEndAEventDay.wMilliseconds=0;

		SI32 startAEventdatevary = pclClient->GetMinuteFromsTime(sStartAEventDay);
		SI32 endAEventdatevary = pclClient->GetMinuteFromsTime(sEndAEventDay);
		SI32 LoginTime = pclClient->GetMinuteFromsTime(pclServer->sTime);


		if((LoginTime <= endAEventdatevary)
			&&(LoginTime >= startAEventdatevary))
			//�̺�Ʈ �Ⱓ���� �����̸� �����߱�.
		{
			SendLog_SetEventCouponInfo(pclchar, NOW_COM_EVENT,1); //  -���� ����.
		}
	}
	// �ӽ÷� ������ �ִ� ����, ������ ��û�Ѵ�.
	sDBRequest_GetAgricultureLeaseInfo clMsg2( pclCM->CR[id]->pclCI->GetPersonID() );
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg2);

	// ģ�� ���� ��û
	sDBRequest_FriendAllList clFriendAllList(id, pclCM->CR[id]->pclCI->GetPersonID(), 1);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clFriendAllList);

	// ���ӿɼ�����
	sDBRequest_Char_Get_Option clCharOptiinInfo(id, pclCM->CR[id]->pclCI->GetPersonID(), 1);
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clCharOptiinInfo);

	

	//---------------------------------------
	// ��ȣõ�� ���� Ȯ��. 
	//---------------------------------------
	CMinisterHozo* pclministerhozo = (CMinisterHozo*)pclMinisterMgr->GetMinister(MINISTER_HOZO);
	if(pclministerhozo)
	{
		if(pclministerhozo->IsValidAngelFromPersonID(pclCM->CR[id]->pclCI->GetPersonID()))
		{
			pclCM->CR[id]->pclCI->clBI.uiAngelSwitch = true;
		}
	}

	//---------------------------------------
	// ���� ���� Ȯ��. 
	//---------------------------------------
	CMinisterHyungzo* pclministerHyungzo = (CMinisterHyungzo*)pclMinisterMgr->GetMinister(MINISTER_HYUNGZO);
	if(pclministerHyungzo)
	{
		if(pclministerHyungzo->IsValidPoliceFromPersonID(pclCM->CR[id]->pclCI->GetPersonID()))
		{
			pclCM->CR[id]->pclCI->clBI.uiPoliceSwitch = true;
		}
	}


	//------------------------------------------
	// �α��� ������ �����Ǿ� �ֵ��� ������ �����ش�. 
	//-------------------------------------------
	if(pclNoticeManager->bLoginNoticeSwitch)
	{
		cltMsg clMsg(GAMEMSG_RESPONSE_NOTICELOGIN, 0);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
/*
	//------------------------------------------
	// ĳ������ ������ Ȯ��.
	//------------------------------------------	
	cltPItemInfo * pclcharitem = &pclCM->CR[id]->pclCI->clCharItem ;
	NDate ndate;
	ndate.SetDate(sTime.wYear,sTime.wMonth,sTime.wDay);
	SI32 todaydatevary = ndate.GetDateVary() ;

	
	for ( i = 0 ; i < MAX_ITEM_PER_PERSON ; i ++ )
	{
		SI32 itemunique = pclcharitem->clItem[i].siUnique ;
		if ( itemunique > 0 )
		{
			if ( pclcharitem->clItem[i].IsHaveUseDay() )  //[����] ���� ���Ⱓ 3�Ϲ̸� �˸�
			{
				SI32 useableDay = 0;
				BOOL rVal = GetItemUseAbleDay(&pclcharitem->clItem[i],&useableDay);
				
				if(pclcharitem->clItem[i].IsSealingUp(pclItemManager) == true)
					continue;

				if(rVal == FALSE) continue;
							
				if(useableDay >= 3) continue; // ��밡�ɱⰣ�� 3�� �̻� �˳��ϸ� �н� 
				else if(useableDay >= 0)		 //  3�� �̸������� ��밡�� �Ⱓ�� �����ִٸ� Ŭ���̾�Ʈ�� ���
				{
					cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclcharitem->clItem[i].siUnique, useableDay);
					cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
					pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );
				}
				else if( useableDay < 0) //���Ⱓ�� ������ �����ش� �׸��� Ŭ���̾�Ʈ�� �˸�.
				{
					//KHY - 0723 - �Ⱓ������ �ı� ����.
					if(pclClient->IsCountrySwitch( Switch_DateOverItemNoDestroy) )
					{

					}
					else
					{
						// ����ǰ�� �������� �ʴ´�. - by LEEKH
						if( pclcharitem->clItem[i].clItemCommon.clCommonInfo.uiRareInfo != RARELEVEL_5 )
						{
							sDBRequest_UseItem useitem(id,pclCM->CR[id]->pclCI->GetPersonID(),i,&pclcharitem->clItem[i],USEITEM_REASON_DATEOVER);
							pclGameDBProcess->SendMsg(DBSELECT_BASE,(sPacketHeader *)&useitem);
												
							cltGameMsgResponse_NoticeItemUseDayOver sendMsg(pclcharitem->clItem[i].siUnique, useableDay);
							cltMsg clMsg( GAMEMSG_RESPONSE_NOTICE_ITEMUSEABLEDAY, sizeof( sendMsg ), (BYTE*)&sendMsg );
							pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );				
						}
					}
				}
			}
		}
	}//( i = 0 ; i < MAX_ITEM_PER_PERSON ; i ++ )
	//------------------------------------------	
*/
	
	//[�߰� : Ȳ���� 2008. 1. 30 => ĳ���� ���鶧 ���� �ڽ� ����Ʈ ��û �ϱ�.]
	if( pclClient->IsCountrySwitch( Switch_SpaceBox ) )
	{ 
		sDBRequest_Space_Box_List clSpace_Box_List( id, pclCM->CR[id]->pclCI->GetPersonID() );
		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clSpace_Box_List);
	} 

	//[����] ����� ���� ��û.
	if( pclClient->IsCountrySwitch( Switch_Helper ) )
	{ 
		sDBRequest_Question_Daily_Count_Get clInfo( id, pclCM->CR[id]->pclCI->GetPersonID() );
		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clInfo);
	}

	// [����] PVP ���Ѵ��� ������û
	sDBRequest_PVP_League_InfoGet clPVPLeagueInfo( pclCM->CR[id]->pclCI->GetPersonID() );
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clPVPLeagueInfo);

	if( pclClient->IsCountrySwitch( Switch_ValenTineWar ) )
	{
		if(pclClient->pclEventTimeManager->InEventTime("ValentineEvent", &sTime) )
		{
			// [����] �߷�Ÿ�� ���� ���� ��û
			sDBRequest_Valantine_Info_Get clValantineInfo( pclCM->CR[id]->pclCI->GetPersonID() );
			SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clValantineInfo);
		}
	}

	// �α��ν� ���ֵ��� ����Ʈ�� �����ߴ��� üũ �ؼ� �������� ���� ĳ���Ϳ��� ���ֵ��� �̼� �ο���.
	// get ���� ��񿡼� ������ ����.

	//------------------------------------------
	// �������� ��Ͻ�Ų��. (��� ������ �ִ� ĳ���ʹ� ǥ������ �ʴ´�. ) 
	//-----------------------------------------
	bool debug = false ;
#ifdef _DEBUG
	// ��Ƽȹ�渷�� //debug = true ;
#endif
	if(pclCM->CR[id]->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL && debug == false )
	{
		SI32 topskillunique = 0;
		SI32 maxskilllevel = 0;
		for(i = 0;i < MAX_SKILL_NUMBER;i++)
		{
			SI32 skillunique = i;
			if(skillunique <= 0)continue;
			
#ifdef _SAFE_MEMORY
			SI32 totallevel = pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, &pclCM->CR[id]->pclCI->clCharItem.clItem[0]);
#else
			SI32 totallevel = pclCM->CR[id]->pclCI->clCharSkill.GetTotalSkillLevel(skillunique, 0, 0, pclCM->CR[id]->pclCI->clCharItem.clItem);
#endif
			SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
			if(totallevel && totallevel >= pclSkillManager->GetMaxLevel(skillunique))
			{
				cltSkillInfo* pclskillinfo = NULL;
				if(skillunique >= 0 && skillunique < MAX_SKILL_NUMBER)
					pclskillinfo = pclSkillManager->pclSkillInfo[skillunique];
				if(pclskillinfo == NULL)continue;
				if((pclskillinfo->siSkillAtb & SKILLATB_LIST) == 0)continue;

				if(pclMasterManager->Regist(id, skillunique, totallevel, personid) == true)
				{
					// �ְ� �����̸�, 
					if(totallevel > maxskilllevel && pclMasterManager->IsTopSkill(id, skillunique) == true)
					{
						maxskilllevel = totallevel;
						topskillunique = skillunique;

						// ���� �ְ� ������ ȿ�� ��� ���. 
						SI32 oldtopid = pclMasterManager->GetSecondSkillID(skillunique);
						if(pclCM->IsValidID(oldtopid))
						{
							pclCM->CR[oldtopid]->ChangeTopSkillUnique(0);
						}

					}
				}
			}
		}
		
		if(topskillunique)
		{
			pclCM->CR[id]->ChangeTopSkillUnique(topskillunique);
		}
	}

	//--------------------------------------
	// �α��� ȸ�� ��� 
	//--------------------------------------
	pclStatisticsManager->clDailyStatistics.siLoginNumber ++;
	

	// ��ǰ ���� �����ؾ� �� ���� ������ ���ش�. (2004/10/20 ~ 2004/10/31)
/*	for(i = 0;i < MAX_ITEM_PER_PERSON;i++)
	{
		cltItem * pclitem = &pclCM->CR[id]->pclCI->clCharItem.clItem[i];
		if(pclitem->siUnique == 0)continue;

		if(pclitem->siUnique == 9400
		|| pclitem->siUnique == 9410
		|| pclitem->siUnique == 13330
		|| pclitem->siUnique == 13340
		|| pclitem->siUnique == 13350
		|| pclitem->siUnique == 13360)
		{

			if(pclitem->clItemArmour.uiSkillUp < 2)
			{
				pclitem->clItemArmour.uiSkillUp = 2;
				
				// DBManager���� �������� �Ҹ��� ���� ��û�Ѵ�. 
				sDBRequest_WasteDurability clMsg(id, pclCM->CR[id]->pclCI->GetPersonID(), i, 
					pclitem);
				SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
			}
		}
	}
*/

	// ���� ������ ���ؼ� ã�ƾ� �� �������� �ִٸ�
	if(pclMsg->bExistentItem)
	{
		cltLetterMsg_Existent_SaveUserItem clletter;
		SendLetterMsg(pclMsg->clInfo.BasicInfo.clPerson.GetPersonID(), (cltLetterHeader*)&clletter );
	}
	


	// ������ȸ ���� ���� ������ ���� �̶��,
	{

		cltGameMsgResponse_GeneralMeetingLetter sendMsg;

		int c = 0;
		for( int i = 0; i < MAX_STOCK_KIND_IN_ACOUNT; ++i )
		{
		
			SI32 villageunique = pclCM->CR[id]->pclCI->clStock.clInfo.clUnit[ i ].siVillageUnique;
			if( villageunique == 0 ) continue;

			cltCityHall *pCityHall = ((cltCityHall *)(pclCityHallManager->pclStruct[ villageunique ]));

			if( pCityHall == NULL ) continue;

			if( pCityHall->clGeneralMeetingInfo.siMatterType == 1 &&
				pCityHall->clGeneralMeetingInfo.siAcceptCandidate == 1 ) 
			{
				// ������ȸ ���̶��,


				// ���� �ǻ縦 ������ �ʾҴٸ�,
				for( int j = 0; j < 100; ++j )
				{
					if( pCityHall->clGeneralMeetingInfo.clVoter[ j ].siPersonID ==
						pclCM->CR[id]->pclCI->GetPersonID() &&
						pCityHall->clGeneralMeetingInfo.clVoter[ j ].siParticipate == 0 )
					{

						if (c >= MAX_STOCK_KIND_IN_ACOUNT)		continue;
						sendMsg.siVillageUnique[ c ] = villageunique;
						c++;				

						break;
					}
				}
			}
		}

		if( c ) {

			cltMsg clMsg( GAMEMSG_RESPONSE_GENERALMEETINGLETTER, sizeof( sendMsg ), (BYTE*)&sendMsg );
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg );				
		}

	}

	{
		// ���� ���� ����

		cltGameMsgResponse_CountryInfo sendMsg;
		sendMsg.siCountryID = clPersonInfo.clBI.siCountryID;
		sendMsg.bShowCountry = clPersonInfo.clBI.bShowCountry;

		cltMsg clMsg( GAMEMSG_RESPONSE_COUNTRYINFO , sizeof(sendMsg), (BYTE*)&sendMsg);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	
	{
		// Ǫġ�ڹ� ���� / ����� ����
		
		cltGameMsgResponse_SetShowPuchikomi sendMsg(clPersonInfo.clBI.bShowPuchikomi);
		cltMsg clMsg( GAMEMSG_RESPONSE_SETSHOWPUCHIKOMI, sizeof(sendMsg), (BYTE*)&sendMsg );
		pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg);

	}

	// 20�̸� ���� ���� - ���� ����.
	//cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	int k;
	for(k = 0; k < MAX_LOWLEVEL_LIST_NUMBER; k++)
	{
		if( siLowLevelCharUnique[k] == pclchar->GetCharUnique() && pclchar->pclCI->clIP.GetLevel() > 19)
		{
			siLowLevelCharUnique[k] = 0;
		}

	}
	bool Same = false;
	bool Full = false;
	if( pclchar->pclCI->clIP.GetLevel() < 20 )
	{
		for(k = 0; k < MAX_LOWLEVEL_LIST_NUMBER; k++)
		{
			if(siLowLevelCharUnique[k] == pclchar->GetCharUnique())
				Same = true;
		}
		if( !Same )
		{
			for(k = 0; k < MAX_LOWLEVEL_LIST_NUMBER; k++)
			{
				if(siLowLevelCharUnique[k] <= 0)
				{
					siLowLevelCharUnique[k] = pclchar->GetCharUnique();
					Full = false;
					break;
				}
				else
					Full = true;
			}		 
		}
		if(Full)
		{
			if( siLowLevelCount >= 0 && siLowLevelCount < MAX_LOWLEVEL_LIST_NUMBER)
			{
				siLowLevelCharUnique[siLowLevelCount] = pclchar->GetCharUnique();
				siLowLevelCount++;
			}
		}
	}

	// �ڽ��� �α����Ҷ� ������ ����� �д�
	{
		pclchar->siLoginedLevel = pclchar->pclCI->clIP.GetLevel();
	}


	if(siLowLevelCount >= MAX_LOWLEVEL_LIST_NUMBER)
	{
		siLowLevelCount = 0;
	}

	// ��尡 �ִٸ� - ��� ������ ������
	// ��尡 ��� - ��� ������ ������ �ʱ�ȭ ������� �Ѵ�.
	//if ( pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex > 0 )
	{
		cltGuildInfo* pclGuild = pclGuildManager->GetGuildInfo(pclchar->pclCI->clBI.clGuildRank.siGuildUnitIndex);
		if ( pclGuild )
		{
			for( SI32 guildmembernum = 0; guildmembernum < MAX_GUILD_MEMBER_NUM; ++guildmembernum )
			{
				// ��� ��û�� �Ǿ� ������ ��� ������ �ͽ��� ���� ���� �ʴ´�.
				if ( pclGuild->clStandingMember[ guildmembernum ].siPersonID == pclchar->pclCI->GetPersonID() )
				{
					cltGuildInfo pclGuildtemp;
					cltSimpleGuildRank	clGuildtemp;

					pclGuildtemp.Init();
					clGuildtemp.Init();

					cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuildtemp.clStatus,&clGuildtemp,pclchar->GetCharUnique());
					cltMsg clInfoMsg( GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS,sizeof(clinfo),(BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clInfoMsg );
				}
				else
				{// ��尡 �������� ��� �������ͽ��� ���� �Ѵ�.
					cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuild->clStatus,&pclchar->pclCI->clBI.clGuildRank,pclchar->GetCharUnique());
					cltMsg clInfoMsg( GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS,sizeof(clinfo),(BYTE*)&clinfo);
					pclchar->SendNetMsg((sPacketHeader*)&clInfoMsg );
				}
			}
		}
		else
		{
			cltGuildInfo pclGuildtemp;
			cltSimpleGuildRank	clGuildtemp;

			pclGuildtemp.Init();
			clGuildtemp.Init();

			cltGameMsgResponse_Guild_SetGuildStatus clinfo(&pclGuildtemp.clStatus,&clGuildtemp,pclchar->GetCharUnique());
			cltMsg clInfoMsg( GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS,sizeof(clinfo),(BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clInfoMsg );
		}
	}

	SI32 siCurrentEventNPC = 0;
/*
#ifdef EVENT_GIRLS_DAY
	siCurrentEventNPC = GetUniqueFromHash(TEXT("KIND_HINADOLL"));
#endif
*/
	SI32 siEventNPCKind[MAX_HIRED_EVENTNPC_NUM];

	// ������ �ִµ� �̺�ƮNPC�� ������ٸ�
	if( pclchar->pclCI->clBI.siHomeVillage > 0 )
	{
		cltStrInfo* pclinfo = pclCityHallManager->GetStrInfo( pclchar->pclCI->clBI.siHomeVillage );

		if( pclinfo != NULL)
		{
			for (i = 0; i<MAX_HIRED_EVENTNPC_NUM; i++) {
				siEventNPCKind[i] = pclinfo->clCityhallStrInfo.clHiredEventNPC[i].siNPCKind;
			}
			// Ŭ���̾�Ʈ�� �̺�ƮNPC ���� ����
			// 0�� ���� ����
			if( siEventNPCKind >= 0 )
			{
				cltGameMsgResponse_VillageHaveEventNPC clinfo( siEventNPCKind);
				cltMsg clInfoMsg(GAMEMSG_RESPONSE_VILLAGEHAVEEVENTNPC, sizeof(clinfo), (BYTE*)&clinfo );
				
				//[���� : Ȳ���� 2008. 2. 25 => ������ �̺�Ʈ NPC�� ������ ���� �������Ը� �޽����� �����ϰ�.]
				pclCM->SendAllPersonHomeMsg( (sPacketHeader*)&clInfoMsg, pclchar->pclCI->clBI.siHomeVillage);
			}
		}	
	}
	//[�߰� : Ȳ���� 2008. 2. 25 => ������ ���� ĳ������ ��� 0���� �ʱ�ȭ ��Ų �����͸� ������.]
	else 
	{
		cltGameMsgResponse_VillageHaveEventNPC clinfo;
		cltMsg clInfoMsg(GAMEMSG_RESPONSE_VILLAGEHAVEEVENTNPC, sizeof(clinfo), (BYTE*)&clinfo );
		
		//[���� : Ȳ���� 2008. 2. 25 => ������ �̺�Ʈ NPC�� ������ ���� �������Ը� �޽����� �����ϰ�.]
		pclchar->SendNetMsg( (sPacketHeader*)&clInfoMsg );
	}


	cltItemManagerServer* pclitemmanager = (cltItemManagerServer*)pclClient->pclItemManager;
	SI16 setItemType = 0;
	SI16 setItemStep = 0;
	SI16 setItemType2 = 0;
	SI16 setItemStep2 = 0;
	SI16 setItemType3 = 0;
	SI16 setItemStep3 = 0;
	SI16 setItemType4 = 0;
	SI16 setItemStep4 = 0;
	SI16 setItemType5 = 0;
	SI16 setItemStep5 = 0;

#ifdef _SAFE_MEMORY
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		setItemType  = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep, USE_SETITEM_1 );
		pclchar->m_siSetItemType = setItemType;

		setItemType2 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep2, USE_SETITEM_2 );
		pclchar->m_siSetItemType2 = setItemType2;

		setItemType3 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep3, USE_SETITEM_3 );
		pclchar->m_siSetItemType3 = setItemType3;

		setItemType4 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep4, USE_SETITEM_4 );
		pclchar->m_siSetItemType4 = setItemType4;

		setItemType5 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep5, USE_SETITEM_5 );
	}
	else
	{
		setItemType = pclitemmanager->pclSetItemManager->FindSetItemType( &pclchar->pclCI->clCharItem.clItem[0] );
	}
#else
	if ( pclClient->IsCountrySwitch(Switch_Set_Item_Add) )
	{
		setItemType  = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep, USE_SETITEM_1  );
		pclchar->m_siSetItemType = setItemType;

		setItemType2 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep2, USE_SETITEM_2  );
		pclchar->m_siSetItemType2 = setItemType2;

		setItemType3 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep3, USE_SETITEM_3  );
		pclchar->m_siSetItemType3 = setItemType3;

		setItemType4 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep4, USE_SETITEM_4  );
		pclchar->m_siSetItemType4 = setItemType4;

		setItemType5 = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar, &setItemStep5, USE_SETITEM_5  );
	}
	else
	{
		setItemType = pclitemmanager->pclSetItemManager->FindSetItemType( pclchar->pclCI->clCharItem.clItem );
	}
#endif

	pclchar->m_siSetItemType = setItemType;
	pclchar->m_siSetItemStep = setItemStep;

	pclchar->m_siSetItemType2 = setItemType2;
	pclchar->m_siSetItemStep2 = setItemStep2;

	pclchar->m_siSetItemType3 = setItemType3;
	pclchar->m_siSetItemStep3 = setItemStep3;

	pclchar->m_siSetItemType4 = setItemType4;
	pclchar->m_siSetItemStep4 = setItemStep4;

	pclchar->m_siSetItemType5 = setItemType5;
	pclchar->m_siSetItemStep5 = setItemStep5;

	cltGameMsgResponse_SetItemTypeInfo clinfo3( setItemType, setItemStep, setItemType2, setItemStep2 , setItemType3, setItemStep3, setItemType4, setItemStep4, setItemType5, setItemStep5);
	cltMsg clMsg3(GAMEMSG_RESPONSE_SETITEMTYPEINFO, sizeof(clinfo3), (BYTE*)&clinfo3);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg3);

	// �Ƿι���-
	if( ConstServiceArea_China == siServiceArea )
	{
		pclchar->bTiredSystem = pclMsg->bTiredSystem;
		pclchar->dwPlayingTime = pclMsg->dwPlayingTime;
	}
	else
	{
		//NHNChina �� �̹� �����Ͽ��⿡ DB���� �������� ���� �������� �ʴ´�.  
		if( ConstServiceArea_NHNChina != siServiceArea )
		{
		pclchar->bTiredSystem = false;
		pclchar->dwPlayingTime = 0;
	}
	}

	//KHY - 1220 - ���Žý���.
	pclchar->clTransFormInfo.Set(&pclMsg->clInfo.clTransFormInfo);
	
	cltGameMsgResponse_SetTransForm cltransform( &pclchar->clTransFormInfo );
	cltMsg clMsgTransform(GAMEMSG_RESPONSE_TRANSFORM, sizeof(cltransform), (BYTE*)&cltransform);
	pclchar->SendNetMsg((sPacketHeader*)&clMsgTransform);

	// [����] Ŭ���̾�Ʈ���� CRCüũ ��Ŷ�� ������� ���� (���ڰ� 1�� �ƹ� �ǹ� ����)
	cltGameMsgResponse_FileCRCCheck cltFileCRCCheck( 1 );
	cltMsg clMsgFileCRCCheck(GAMEMSG_RESPONSE_FILECRCCHECK, sizeof(cltFileCRCCheck), (BYTE*)&cltFileCRCCheck);
	pclchar->SendNetMsg((sPacketHeader*)&clMsgFileCRCCheck);

	// [����] �⼮����
	SI32 siPersonID		= pclchar->pclCI->GetPersonID();
	SI32 siCharID		= pclchar->GetID();

	NDate kAttendDay;

	kAttendDay.SetDate( sTime.wYear, sTime.wMonth, sTime.wDay );

	// [����] Season�⼮ �̺�Ʈ ���̶�� �˸�
	if ( (kAttendDay.GetDateVary() >= clAttendEventStart.GetDateVary()) &&
		(kAttendDay.GetDateVary() <= clAttendEventEnd.GetDateVary()) 
	)
	{
		// Ŭ���̾�Ʈ���� �̺�Ʈ ���̶�� �˸�
		SendServerResponseMsg( 0, SRVAL_NOTICE_SEASON2_ATTEND, 0, 0, siCharID );

		// ����2 
		sDBRequest_Attend clMsg(siPersonID, siCharID, &clAttendEventStart, &clAttendEventEnd, ATTEND_EVENTTYPE_SEASON2 );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}


	// [����] PC�� �����̾� ��ȭ - ����Ŭ�ι� ����
	if ( ( clPersonInfo.clBI.bPCRoom == true ) && (pclMsg->clInfo.BasicInfo.bAttendToday[ ATTEND_EVENTTYPE_PCROOM ] == false) )
	{
		sDBRequest_Attend clMsg(siPersonID, siCharID, &kAttendDay, &kAttendDay, ATTEND_EVENTTYPE_PCROOM );
		pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}

	//------------------------------------------
	// [����] �޸� ���� ��ü
	//------------------------------------------
	CDormancySystem *pclDormancySystem = this->pclDormancySystem;
	CDormancyEquipItemRewardServer *pclDormancyServer = NULL;
	CDormancyPointRewardServer *pclDormancyPointServer = NULL;
	cltCharServer *pclCharServer = pclCM->GetCharServer(id);
	if(NULL != pclDormancySystem)
	{
		pclDormancyServer = (CDormancyEquipItemRewardServer *)pclDormancySystem->GetDormancyEquipItemReward();
		pclDormancyPointServer = (CDormancyPointRewardServer *)pclDormancySystem->GetDormancyPointReward();
	}
	// �޸� ���� ���� ����
	if(NULL != pclDormancyPointServer)
	{
		pclDormancyPointServer->SendClientDormancyInfo(pclCharServer);
		pclDormancyPointServer->WaringExpiration(pclCharServer);
	}
	// �޸� ���� �ý��� - ��� ���� ���� ���� ����
	if(NULL != pclDormancyServer)
	{
		// ���� ���ڰ� ���� ���� �ʾҴٸ�
		if( clPersonInfo.clDormancyInfo.GetGiveDormancyEquipItem() == FALSE)
			pclDormancyServer->GiveEquipBox(pclCharServer);
	}

	// [ �ռ���] ��õ�� �ý��� ����
	// [����] �ӽ÷� ������ ����
	if ( clPersonInfo.clBI.clRecommenderInfo.siPersonID == 0 )
	{
		cltGameMsgResponse_RecommendPersonAvailable clinfo( true );
		cltMsg clMsg(GAMEMSG_RESPONSE_RECOMMENDPERSONAVAILABLE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		//KHY - 1024 - �Ѱ��� VIP �̺�Ʈ- �Ⱓ�� ���� ĳ����.
		//by LEEKH 2007.10.30 - �Ⱓ ����
		if (pclClient->IsCountrySwitch(Switch_HanGameVIPEvent) )
		{					
			_SYSTEMTIME sStartEventDay , sEndEventDay;

			ZeroMemory(&sStartEventDay,	sizeof(_SYSTEMTIME));
			ZeroMemory(&sEndEventDay,	sizeof(_SYSTEMTIME));

			sStartEventDay.wYear    = 2006;
			sStartEventDay.wMonth   = 11;
			sStartEventDay.wDay     = 8;
			sStartEventDay.wHour    = 0;
			sStartEventDay.wMinute  = 0;

			sEndEventDay.wYear		= 2006;
			sEndEventDay.wMonth		= 11;
			sEndEventDay.wDay		= 30;
			sEndEventDay.wHour		= 0;
			sEndEventDay.wMinute	= 0;

			SI32 startEventdatevary = pclClient->GetMinuteFromsTime(sStartEventDay);
			SI32 endEventdatevary   = pclClient->GetMinuteFromsTime(sEndEventDay);

			_SYSTEMTIME fTimeDatevary;  // ���� �ð��� �����̴�.  = ���� ���� ���� �ð�.
			GetLocalTime( &fTimeDatevary );

			SI32 firstCreatTimedatevary = pclClient->GetMinuteFromsTime(fTimeDatevary);

			if((startEventdatevary <= firstCreatTimedatevary)&&(endEventdatevary >= firstCreatTimedatevary))
			{
				SendLog_SetEventCouponInfo( pclchar->pclCI->clBI.szAccountID, 
											(TCHAR*)pclCM->CR[siPersonID]->pclCI->clBI.GetName()
											,siPersonID, 
											HAN_GAME_VIP_EVENT,
											1	); // 1 = ù��°�� ������� ĳ������.
			}
		}
	}


	// ��ǥ ���� �����̰�, �ĺ��� ������ ���� ���� ���� ��� ��ǥ ����
	if ( pclVoteMgr->IsPlayStatus() &&  clPersonInfo.clBI.siVoteKingSelectionIndex == 0 )
	{	
		// ���� 40 �̻� ��ǥ�� ���� ����
		if ( clPersonInfo.clIP.GetLevel() >= Const_CanVote_MinLevel )
		{
			SendServerResponseMsg( 0, SRVAL_NOTICE_WARPTOPOLL_MAP, 0, 0, siCharID );
		}
	}

	// �����ð��� �α��ζ� �ѹ� �����Ѵ�.
	cltGameMsgResponse_ServerTime clTime(sTime);
	cltMsg clTimeMsg(GAMEMSG_RESPONSE_SERVERTIME, sizeof(clTime), (BYTE*)&clTime);
	pclchar->SendNetMsg((sPacketHeader*)&clTimeMsg);

	// ä�� ���� �ð��� �����´�.
	sDBRequest_GetMuteTime clMsgMute( siCharID, siPersonID );
	pclGameDBProcess->SendMsg((sPacketHeader *)&clMsgMute);

	//[����] ������
	if( pclClient->IsCountrySwitch( Switch_Elixir ) )
	{ 
		sDBRequest_Elixir_Get clInfo( id, pclCM->CR[id]->pclCI->GetPersonID() );
		SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clInfo);
	}

	// [����] ��ȥ �ý��� : ���� ������ �޾ƿ´�
	if ( IsCountrySwitch(Switch_WeddingSystem) )
	{
		sDBRequest_Get_Marry_Info clMarryInfo( siPersonID );
		pclGameDBProcess->SendMsg( (sPacketHeader*)&clMarryInfo );
	}
	// [����] ���� �ý��� : ���� ������ �޾� �´�.
	sDBRequest_Get_FamilyInfo clFamilyInfo( siPersonID );
	pclGameDBProcess->SendMsg( (sPacketHeader*)&clFamilyInfo );

	// ��ȣ���� �޾ƿ���
	if ( IsCountrySwitch(Switch_SoulGuard) )
	{
		// �ϴ� ���������� �ʱ�ȭ ���ָ� ������ ĳ���� ����� ������ ���� ���� �ʴ´�
		if ( pclchar->pclSoulGuard )
		{
			pclchar->pclSoulGuard->Init();
		}

		sDBRequest_SoulGuardGet clSoulGuardInfo( siPersonID );
		pclGameDBProcess->SendMsg( (sPacketHeader*)&clSoulGuardInfo );

		sDBRequest_SoulGuardHouse_Get clSoulGuardHouseInfo( siPersonID );
		pclGameDBProcess->SendMsg( (sPacketHeader*)&clSoulGuardHouseInfo );
	}

	// [����] ��ų�� ���� ���� ��û.
	if ( IsCountrySwitch(Switch_SkillBook) )
	{
		if( pclchar->pclCharSkillBookInfo ) pclchar->pclCharSkillBookInfo->Clear();
		
		sDBRequest_SkillBook_MagicInfo_Get clSkillBookMagicInfoGet( id, siPersonID );
		pclGameDBProcess->SendMsg( (sPacketHeader*)&clSkillBookMagicInfoGet );
	}

	// [����] �߷�Ÿ�� ���� ���� ������ ����
	CValentineEventServerMgr *pclValentineEventServerMgr = (CValentineEventServerMgr *)m_pclValentineEventMgr;

	if ( NULL != pclValentineEventServerMgr)
	{
		if(pclValentineEventServerMgr->IsEventPeriod())
		{
			pclValentineEventServerMgr->SendNewBufInfo(pclCharServer);
			pclValentineEventServerMgr->SendBufInfo(pclCharServer);
		}
	}

	// 1. ���ֵ��� �̺�Ʈ ���̶�� Ŭ���̾�Ʈ���� �˷��ݴϴ�.
	if( IsCountrySwitch( Switch_Server_Event ) )
	{
		if(pclEventTimeManager->InEventTime(TEXT("GoonzuDay_Week"), &sTime) == true)
		{
			cltGameMsgResponse_EventTime_Update sendMsg(TEXT("GoonzuDay_Week"), true);
			cltMsg clMsg(GAMEMSG_REQUEST_EVENTTIME_UPDATE, sizeof(sendMsg), (BYTE*)&sendMsg);

			cltCharServer* pclchar = (cltCharServer*)pclClient->pclCM->CR[id];
			if(pclchar)
			{
				pclchar->SendNetMsg((sPacketHeader*)&clMsg);

				// [����] ���ֵ��̹̼� : ���ֵ��� �̼��� �̹� �޾Ҿ����� ��� ����
				cltEventTime* pParentEventTime = NULL;
				pParentEventTime = pclEventTimeManager->GetEvent( TEXT("GoonzuDay_Week") );
				cltEventTime* pChildEventTime = NULL;
				pChildEventTime = pclEventTimeManager->GetEvent( TEXT("GoonzuDay_GiveMission") );

				if ( NULL != pParentEventTime && NULL != pChildEventTime )
				{
					if ( true == pParentEventTime->m_bEventActivated && true == pChildEventTime->m_bEventShoted )	// ���ֵ��� �̺�Ʈ ���̰�, ���ֵ��� �̺�Ʈ�� ���ϵ� �̼��� ���ֵ��̹̼� �̺�Ʈ�� �̹� ����Ǽ� ĳ���͵鿡�� �̼��� �ο��ϰ� �� �ڿ� �α��� ���� ��쿡�� �̼� ���࿩�� ��� ���)
					{
						sDBRequest_GoonzuDayMissionInfo_Get clSndInfo( pclchar->GetID(), pclchar->pclCI->GetPersonID() );
						pclClient->SendDBMsg( DBSELECT_BASE,(sPacketHeader*)&clSndInfo );
					}
				}
			}
		}
	}

	pclchar->SetUpdateSwitch(UPDATE_QUEST, true, 0);
}

// Person���� ��û�� ���� ����. 
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONTOTALINFODELETE(sPacketHeader* pPacket)
{
	sDBResponse_PersonTotalInfoDelete* pclMsg = (sDBResponse_PersonTotalInfoDelete*)pPacket;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	//if(pclMsg->siResult == 1 || siServiceArea == ConstServiceArea_English)
	if(pclMsg->siResult == 1)
	{
		siDelCharNum++;

		//-------------------------------------------------------------
		// GameDBManager_World���� ��� Person������ �˷��� ���� ��û�Ѵ�. 
		//-------------------------------------------------------------
		sDBRequest_PersonListInfoGet clMsg(pclMsg->usCharID, pclCM->CR[id]->pclCI->clBI.siIDNum);//�����ʿ俩�⼭ ����Ʈ�� ���ĸ鼭
		//����Ʈ �ɸ���ȭ���� �ٲܼ� ������ �̰����� �ð��� ��Ŷ�� �������� ����.
		pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);

		SI32 villageunique = pclCM->CR[id]->GetCurrentVillageUnique();

		sDBRequest_GeneralMeetingInfo clMsg2( villageunique );
		pclGameDBProcess->SendMsg( (sPacketHeader *)&clMsg2);

		// ��尡 �ִ� ĳ���̶��
		if ( pclMsg->clinfo.siUnique > 0 )
		{
			cltGuildInfo* pGuild = pclGuildManager->GetGuildInfo(pclMsg->clinfo.siUnique);
			if ( pGuild )
			{
				// ������ �Ŵ�
				if ( pGuild->siMasterPersonID <= 0 )
				{
					for ( SI32 i = 0 ; i < MAX_GUILD_MEMBER_NUM ; i ++ )
					{
						SI32 personid = pGuild->clMember[i].siPersonID ;
						SI32 charid = pclCM->GetIDFromPersonID(personid);
						if ( pclCM->IsValidID(charid) )
						{
							pclCM->CR[charid]->pclCI->clBI.clGuildRank.Init();

							cltGuildStatus clStatus;
							ZeroMemory(&clStatus,sizeof(cltGuildStatus));
							cltSimpleGuildRank clRank;
							clRank.Init();

							cltGameMsgResponse_Guild_SetGuildStatus clinfo(&clStatus,&clRank,pclCM->CR[charid]->GetCharUnique());
							cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
							((cltCharServer*)pclCM->CR[charid])->SendNeighbourMsg(&clMsg,true);
						}

						personid = pGuild->clStandingMember[i].siPersonID ;
						charid = pclCM->GetIDFromPersonID(personid);
						if ( pclCM->IsValidID(charid) )
						{
							pclCM->CR[charid]->pclCI->clBI.clGuildRank.Init();

							cltGuildStatus clStatus;
							ZeroMemory(&clStatus,sizeof(cltGuildStatus));
							cltSimpleGuildRank clRank;
							clRank.Init();

							cltGameMsgResponse_Guild_SetGuildStatus clinfo(&clStatus,&clRank,pclCM->CR[charid]->GetCharUnique());
							cltMsg clMsg(GAMEMSG_RESPONSE_GUILD_SETGUILDSTATUS, sizeof(clinfo), (BYTE*)&clinfo);
							((cltCharServer*)pclCM->CR[charid])->SendNeighbourMsg(&clMsg,true);
						}
					}

					pGuild->Init();
				}

				pGuild->Set(&pclMsg->clinfo);
			}
		}
		// ���ֿ� ���� �ֹ��� �ִ°�� ������ �����ش�.
		for( SI32 i = 0; i < MAX_MARKET_ORDER_PER_PERSON; ++i )
		{
			SI32 index = pclMsg->clOrder[i].siIndex;
			if( index < 0 || index >= MAX_MARKET_ORDER_NUMBER )
			{
				continue;
			}

			SI32 orderitemunique	= pclMarketManager->pclOrderManager->pclOrderSell[index]->clItemData.siUnique;

			if( 0 < orderitemunique )
			{
				if( true == pclMarketManager->pclOrderManager->SetOrder( ORDERMODE_SELL, &pclMsg->clOrder[i] ) )
				{
					cltTradeOrder clorder;
					clorder.Set(&pclMsg->clOrder[i]);
					clorder.clItemData.siUnique	= orderitemunique;

					pclMarketManager->SetItemTypeOrder(&clorder, 700);
				}
			}
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 9. 5] // ĳ���� ���� �α׾���.
			// pszParam1 == ���̵�.
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_DELCHAR, 
								0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
								0, 0, 0, 0, 0, pclCM->CR[id]->pclCI->clBI.szAccountID, NULL);
		}
	}
	
	else if(pclMsg->siResult <= 0)
	{
		switch(pclMsg->siResult)
		{
			case 0:  SendServerResponseMsg(0, SRVAL_DELPERSON_FAIL_UNKNOWN,  0, 0, pclCM->CR[id]->GetCharUnique());		break;
			case -1: SendServerResponseMsg(0, SRVAL_DELPERSON_FAIL_IDMATCHERROR,  0, 0, pclCM->CR[id]->GetCharUnique()); break;	// �ֹε�Ϲ�ȣ ���ڸ��� ����ġ���� Client���� �˸���.
		}
		return ;
	}

	

}

// Person���� ��û��� ���� ����.
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOPREDELETE(sPacketHeader* pPacket)
{
	sDBResponse_PersonTotalInfoPreDelete* pclMsg = (sDBResponse_PersonTotalInfoPreDelete*)pPacket;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	//if(pclMsg->siResult == 1 || siServiceArea == ConstServiceArea_English)
	if(pclMsg->siResult == 1)
	{
		siDelCharNum++;

		//-------------------------------------------------------------
		// GameDBManager_World���� ��� Person������ �˷��� ���� ��û�Ѵ�. 
		//-------------------------------------------------------------
		sDBRequest_PersonListInfoGet clMsg(pclMsg->usCharID, pclCM->CR[id]->pclCI->clBI.siIDNum);//�����ʿ俩�⼭ ����Ʈ�� ���ĸ鼭
		//����Ʈ �ɸ���ȭ���� �ٲܼ� ������ �̰����� �ð��� ��Ŷ�� �������� ����.
		pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);
	}

}

// Person ���� �����û�� ���� ����. (���ٸ� ��ġ�� ���� ���� ����.)
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONTOTALINFOSET(sPacketHeader* pPacket)
{
	sDBResponse_PersonTotalInfoSet* pclMsg = (sDBResponse_PersonTotalInfoSet*)pPacket;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

}


// Person�� �ֽ� ���� ���� ��û�� ���� ����. 
void cltServer::DoMsg_DBMSG_RESPONSE_PERSONSTOCKACCOUNTGET(sPacketHeader* pPacket)
{
	sDBResponse_PersonStockAccountGet* pclMsg = (sDBResponse_PersonStockAccountGet*)pPacket;

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	// Person�� �ֽ� ���� ������ ������Ʈ �Ѵ�. 
	pclCM->CR[id]->pclCI->clStock.clInfo.Set(&pclMsg->clStockAccount);
	
	// Ŭ���̾�Ʈ�� �����ϰ� �Ѵ�. 
	pclCM->CR[id]->SetUpdateSwitch(UPDATE_STOCK, true, 0);

}

// Person�� ��ų ������ �����. 
void cltServer::DoMsg_DBMSG_RESPONSE_DECREASESKILL(sPacketHeader* pPacket)
{
	DoMsg_DBMSG_RESPONSE_INCREASESKILL(pPacket);
}


// Person�� ��ų ������ ���δ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_INCREASESKILL(sPacketHeader* pPacket)
{
	sDBResponse_IncreaseSkill* pclMsg = (sDBResponse_IncreaseSkill*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if( pclMsg->uiSkillUnique < 0 || pclMsg->uiSkillUnique >= MAX_SKILL_NUMBER)		return;

		// Person�� ��ų ������ ������Ʈ �Ѵ�. 
		SI32 oldskilllevel		= pclCM->CR[id]->pclCI->clCharSkill.clSkill[pclMsg->uiSkillUnique].uiLevel;
		SI32 oldtotalskilllevel	= oldskilllevel + pclCM->CR[id]->pclCI->clCharSkill.GetSkillExpLevel(pclMsg->uiSkillUnique);

		pclCM->CR[id]->pclCI->clCharSkill.clSkill[pclMsg->uiSkillUnique].uiSkillUnique	= pclMsg->uiSkillUnique;
		pclCM->CR[id]->pclCI->clCharSkill.clSkill[pclMsg->uiSkillUnique].uiLevel		= pclMsg->uiSkillLevel;

		// Ŭ���̾�Ʈ�� �����ϰ� �Ѵ�. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_SKILL, true, 0);

		//Person�� SkillBonus�� ������Ʈ �Ѵ�. 
		pclCM->CR[id]->pclCI->clIP.SetSkillBonus(pclMsg->siSkillBonus);

		// Ŭ���̾�Ʈ�� �����ϰ� �Ѵ�. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);
		
		// �̿���� �Ҹ��ؼ� ����� �� ���, �������� ������Ʈ �Ѵ�. 
		if(pclMsg->siMode == DECSKILLMODE_TICKET || pclMsg->siMode == DECSKILLMODE_WEAPONSKILL|| pclMsg->siMode == DECSKILLMODE_ALLSKILLPOINT  )
		{
			if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
			{
				pclCM->CR[id]->pclCI->clCharItem.clItem[ pclMsg->siItemPos].Set(&pclMsg->clItem);

				// Ŭ���̾�Ʈ�� ������Ʈ �� ������ ������. 
				cltGameMsgResponse_PersonItemInfo clinfo(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
				cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
				pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

				// PCK - ������ ��� �α׸� �����.
				cltServer * pclServer = (cltServer*)pclClient;
				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_USE_ITEM,
					USEITEM_REASON_SKILLRESET_TICKET, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clUseItem, 0, 0, 0, 0, 0, 
					pclMsg->siItemPos, 0, 0, 0, 0, 0, NULL, NULL);
			}
		}

		// ������ ������ �ö��ٸ�, 
		SI32 totalskilllevel = pclMsg->uiSkillLevel + pclCM->CR[id]->pclCI->clCharSkill.GetSkillExpLevel(pclMsg->uiSkillUnique);

		if(oldtotalskilllevel < totalskilllevel)
		{
			SendServerResponseMsg(0, SRVAL_SETSKILL,  pclMsg->uiSkillUnique, totalskilllevel, pclCM->CR[id]->GetCharUnique());

			if ( IsCountrySwitch(Switch_Server_Event) )
			{
				CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclServerEventManager->GetServerEvent_Quest();
				if ( pclSE_Quest )
				{
					SI32 siAmount = max( 1, ((cltCharServer*)pclCM->CR[id])->siQuestCount );
					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_SKILLUP, totalskilllevel, siAmount );
				}
			}
/*
			TCHAR szName[MAX_PLAYER_NAME];
			MStrCpy( szName, pclCM->CR[id]->GetName(), MAX_PLAYER_NAME );
			
			//KHY - 1205 - ���������� ����
			if ( pclCM->CR[id]->pclCI->clBI.clGuildRank.IsGuildMember() == true ) //���.
			{
				cltGuildInfo* pGuild = pclClient->pclGuildManager->GetGuildInfo(pclCM->CR[id]->pclCI->clBI.clGuildRank.GetGuildUnitIndex());
				if ( pGuild )
				{
					cltGameMsgResponse_LevelUpNotice clinfo(szName, 0, totalskilllevel , pclMsg->uiSkillUnique , 1);
					cltMsg clMsg(GAMEMSG_RESPONSE_LEVELUPNOTICE, sizeof(clinfo), (BYTE*)&clinfo);
					pclClient->pclCM->SendAllGuildMemberMsg((sPacketHeader*)&clMsg,pGuild->siUnique);

				}
			}

			if(pclCM->CR[id]->pclCI->clBI.siHomeVillage > 0 && pclCM->CR[id]->pclCI->clBI.siHomeVillage <= CUR_VILLAGE_LIMIT) //���� 
			{

				//KHY - 1205 - ���������� ����
				cltGameMsgResponse_LevelUpNotice clinfo(szName, 0, totalskilllevel , pclMsg->uiSkillUnique , 2);
				cltMsg clMsg(GAMEMSG_RESPONSE_LEVELUPNOTICE, sizeof(clinfo), (BYTE*)&clinfo);
				pclClient->pclCM->SendAllPersonInVillageMsg((sPacketHeader*)&clMsg,pclCM->CR[id]->pclCI->clBI.siHomeVillage);

			}
*/
		}
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 11] // ĳ������ ��ų��°� ����. ���� - PCK : 08.09.19
		// param1 == ������ ��ų ����ũ.  
		// param2 == ������ ��ų ����, 
		// param3 == ����� ����� ��ų���ʽ� ����Ʈ, 
		// param4 == ���. 
		SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SKILL, LOGCOMMAND_INDEX_SKILL_CHANGE, 
			pclMsg->siReason, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clUseItem, 0, 0, 0, 0, 0, 0, 
			pclMsg->uiSkillUnique, pclMsg->uiSkillLevel, pclMsg->siSkillBonus, pclMsg->siMode, 0, 
			NULL, NULL);
	}
}

// Person�� ��ų ������ ���δ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_SETSKILLEXP(sPacketHeader* pPacket)
{
	// PCK : �α׿� �߰�
	sDBResponse_SetSkillExp*	pclMsg = (sDBResponse_SetSkillExp*)pPacket;

	// PCK : PersonID�� �ٰŷ� CharID�� ã��.(PersonID��������!!)
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	// param1 == ��� ����ũ, 
	// param2 == ��� ����ġ.
	cltServer * pclServer = (cltServer*)pclClient;
	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SKILL, LOGCOMMAND_INDEX_SKILL_SET_SKILL_EXP, 
									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
									pclMsg->uiSkillUnique, pclMsg->siSkillExp, 0, 0, 0, NULL, NULL);
}

// Person�� ���� ������ �����Ѵ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_SETLEVELINFO(sPacketHeader* pPacket)
{
	sDBResponse_SetLevelInfo* pclMsg = (sDBResponse_SetLevelInfo*)pPacket;

	if ( pclMsg->siResult != 1 )
	{

		SI32 id	= pclMsg->usCharID;
		if(pclCM->IsValidID(id) == FALSE)return ;

		
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

		// PCK : ���� ������ �����ϴ� �κп� ĳ���� ������ �ٵ��ͼ� �ʿ���� �α׶� �ǴܵǾ� �ּ�ó����. (08.06.20)
		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [�α��߰� : Ȳ���� 2007. 10. 15] // ���� ������ ����
		//	// 
		//	cltServer* pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_INPORTANTPARAMEATA, LOGCOMMAND_INDEX_INPORTANTPARAMEATA_SETLEVELINFO,
		//									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		//									0, 0, 0, 0, 0,  NULL, NULL);
		//}
	

		if(pclchar->GetGameMasterLevel() < BASIC_GAMEMASTER_LEVEL)
		{
			// �������� ������ ����Ǿ����Ƿ� ����ڸ� ��������. 
			pclchar->bCloseSocketSwitch		= true;
			pclchar->dwCloseSocketTimer		= 10;
			
			SendNoticeMsg(-1, 0, id, TEXT("�������� �����ͷ� ���� ������ �����մϴ�."));
			
#ifdef _CHINA
			pclLog->FilePrint( TEXT("ErrorSerLevelInfo"), TEXT("SetLevelInfo name:%s"),
				pclchar->GetName());
#else
			pclLog->FilePrint( TEXT("Config\\ErrorLog.log"), TEXT("SetLevelInfo name:%s"),
				pclchar->GetName());
#endif
		}


		return;
	}
}

// ��ġ, ������, ������ ���� ����
void cltServer::DoMsg_DBMSG_RESPONSE_SETPOSLIFEMANA(sPacketHeader* pPacket)
{
	// PCK:  �α׸� ����� ���ؼ� �ۼ�
	sDBResponse_SetPosLifeMana*	pclMsg = (sDBResponse_SetPosLifeMana*)pPacket;

	// PCK : PersonID�� �ٰŷ� CharID�� ã��.(PersonID��������!!)
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
	if ( false == pclCM->IsValidID(CharID) )
	{
		return;
	}

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	// param1 = Life, 
	// param2 = Mana, 
	// param3 = Map Index, 
	// param4 = Pos x, 
	// param5 = Pos y
	// charparam1 = "PlaySec = %d, PlayEventSec = %d"
	TCHAR szCharParam[50] = { '\0', };
	StringCchPrintf(szCharParam, sizeof(szCharParam), TEXT("PlaySec = %d, PlayEventSec = %d"), pclMsg->siPlaySecond, pclMsg->siPlayEventSecond);

	cltServer* pclserver = (cltServer*)pclClient;
	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_INPORTANTPARAMEATA, LOGCOMMAND_INDEX_INPORTANTPARAMEATA_SETPOSLIFEMANA,
		0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
		pclMsg->siLife, pclMsg->siMana, pclMsg->clPos.GetMapIndex(), pclMsg->clPos.GetX(), pclMsg->clPos.GetY(),
		szCharParam, NULL);
}

void cltServer::DoMsg_DBMSG_RESPONSE_BBB_SET_REWARDMONEY(sPacketHeader* pPacket)
{
	//sDBResponse_BBB_Set_RewardMoney* pclMsg = (sDBResponse_BBB_Set_RewardMoney*) pPacket;

}

void cltServer::DoMsg_DBMSG_RESPONSE_BBB_GET_REWARD_MONEY(sPacketHeader* pPacket)
{ 
	sDBResponse_BBB_Get_RewardMoney* pclMsg = (sDBResponse_BBB_Get_RewardMoney*)pPacket;

	if(m_pServer_BBBMgr)
	{
		m_pServer_BBBMgr->m_siTotalMoney = pclMsg->m_siTotalRewardPriceWithDraw;

	}
}

// [����] ���ֵ��̹̼� : �̼� �ο� ���ο� ���� ����� ����
void cltServer::DoMsg_DBMSG_RESPONSE_GOONZUDAYMISSIONINFO_GET(sPacketHeader* pPacket)
{
	// ��񿡼� �Ѿ�� ���ֵ��̹̼� ���� ���ο� ���� ��¥ ������ ���� �̼��� �ο����� ������ ����
	// (����� ��¥�� ���������� �̼��� �ο� ���� ��¥�̹Ƿ� �װ� ���� ���ֵ��� �Ⱓ�� ������ �Ǹ� �̼��� ���� �ʰ� ������ �ȵǸ� �̼��� �ش�)

	if (  false == pclEventTimeManager->InEventTime(TEXT("GoonzuDay_Week"), &sTime) ) return;		// ���ֵ��� �̺�Ʈ �Ⱓ �ƴϸ� �׳� ����

	sDBResponse_GoonzuDayMissionInfo_Get* pRcvInfo = (sDBResponse_GoonzuDayMissionInfo_Get*)pPacket;
	if ( NULL == pRcvInfo ) return;

	// ���������� �������� �����ΰ�? �ƴϸ� �׳� ���� ������ �Ʒ� ����
	SI32 id = 0;
	id = pRcvInfo->m_usCharID;
	if ( false == pclCM->IsValidID(id) ) return;
	if ( false == pclCM->CR[id]->IsValidConnection() ) return;
	cltCharServer* pCharServer = (cltCharServer*)pclCM->CR[id];

	// ��� ������ִ� �� ĳ������ ���ֵ��� �̼� ���� �ð��� ���� ���ֵ��� �̺�Ʈ �ð��� ���ؼ� ���� �̼��� �������� �ʾҴٸ� �̼��� �ο����ش�.
	cltEventTime* pGoonzuDayEvent = NULL;
	cltEventTime* pGoonzuDayMissionEvent = NULL;
	SYSTEMTIME stCurrentTime;		// ����ð�
	GetLocalTime( &stCurrentTime );

	pGoonzuDayEvent			= pclEventTimeManager->GetEvent( TEXT("GoonzuDay_Week") );
	pGoonzuDayMissionEvent	= pclEventTimeManager->GetEvent( TEXT("GoonzuDay_GiveMission") );
	if ( NULL == pGoonzuDayEvent || NULL == pGoonzuDayMissionEvent ) return;
	if ( false == pGoonzuDayEvent->m_bEventActivated || false == pGoonzuDayMissionEvent->m_bEventShoted ) return;

	// ���ֵ��� �̼� ���� ���� ��¥�� ������ ���ֵ��� �̺�Ʈ �Ⱓ�� ���� ���� ���� ���ߴ��� �Ǻ�

	// ������ ��񿡼� �Ѿ�� ��¥ ������ 0 �ΰ�� (�ѹ��� ���ؼ� �ʱⰪ�� �Ѿ�°�쿡 ���ؼ� ���� ó��)
	if ( 0 == pRcvInfo->siCount || (0 == pRcvInfo->m_MissionGetTime.wYear && 0 == pRcvInfo->m_MissionGetTime.wMonth && 0 == pRcvInfo->m_MissionGetTime.wDay) )
	{
		pclEventTimeManager->GiveMission( pCharServer );
	}
	else
	{
		// ** ���ֵ��� �̺�Ʈ �Ⱓ�� EndTime�� ���� ������ �ȵǾ� �ְ� 65535 �ƽ������� �ʱ�ȭ �Ǿ� �ֱ� ������ ������ ��¥�� ��Ȯ�� ���Ҽ��� ����. �׷��� �Ʒ� ���ǽ��� ����, ���� ��¥�� ��� ����Ǿ��ִ� ĳ������ ���ֵ��̹̼� ���� �ο� ��¥���� ������ �񱳸� �Ѵ�. 
		if ( pRcvInfo->m_MissionGetTime.wYear		!= stCurrentTime.wYear 
			|| pRcvInfo->m_MissionGetTime.wMonth	!= stCurrentTime.wMonth
			|| pRcvInfo->m_MissionGetTime.wDay		!= stCurrentTime.wDay )
		{
			pclEventTimeManager->GiveMission( pCharServer );
		}
	}
}

// �ǰ� ���� ����.
void cltServer::DoMsg_DBMSG_RESPONSE_SETHEALTH(sPacketHeader* pPacket)
{
	
}

// ���� ���̵� �޾ƿ�
void cltServer::DoMsg_DBMSG_RESPONSE_USERID( sPacketHeader* pPacket )
{
	sDBResponse_Userid* pclMsg = (sDBResponse_Userid*)pPacket;

	if ( pclMsg->siResult < 1 )
	{
		return;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)
		return ;

	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		
		if( pclMsg->bShowPuchikomi == false )
		{
			SendServerResponseMsg( 0 , SRVAL_PUCHIKOMI_FAIL_OP_REJECTION , 0 , 0 , pclCM->CR[id]->GetCharUnique() );
		
			return;
		}


		UI08 md5buf[256] = "" ;
		char buffermd5ID[256] = "" ;
		char buffermd5PassWord[256] = "" ;

		char tmp[256] = "" ;

		sprintf(tmp,"%s",  pclMsg->strUserID );	
		SI32 tmpsize = strlen(tmp);

		// URLEncode
		CString str ;
		str.SetString(tmp,tmpsize);
		CString unicodeID = URLEncode(str);

		// MD5
		SI32 md5size = unicodeID.GetLength() ;
		memcpy(md5buf,unicodeID.GetBuffer(md5size),md5size);

		md5_string(md5buf, buffermd5ID, md5size);

		cltGameMsgResponse_UserPuchikomiURL clinfo(buffermd5ID);	
		cltMsg clMsg(GAMEMSG_RESPONSE_USERPUCHIKOMIURL,sizeof(clinfo),(BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );

	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_RECOMMENDPERSON( sPacketHeader* pPacket )
{
	sDBResponse_RecommendPerson* pclMsg = (sDBResponse_RecommendPerson*)pPacket;

	SI32 id	= pclMsg->siCharID;

	if(pclCM->IsValidID(id) == FALSE)
		return ;

	//	if( pclMsg->siResult != 1 ) // �׳� ���� �ϵ��� �Ǿ� ������ �� ����Ʈ ���� ���ؼ� ���� �޼����� ��������
	if( _tcslen(pclMsg->strUserName) < 1) // �̸��� ���̸� ����
	{
		SendServerResponseMsg(0, SRVAL_FAIL_RECOMMANDPERSON_CANCEL,  0, 0, id);
		return ;
	}

	else if( pclMsg->siResult != 1 )
	{

		//todo:
		SendServerResponseMsg(0, SRVAL_FAIL_RECOMMANDPERSON,  0, 0, id);

		return ;
	}
	else {
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
		if( pclchar == NULL ) return ;
		if( pclMsg->uiSlot < 0 || pclMsg->uiSlot >= MAX_CHAR_PER_USER )		return;

		if( pclchar->pclCharListInfo != NULL &&
			pclchar->pclCharListInfo[pclMsg->uiSlot].clBI.clPerson.siPersonID == pclMsg->siPersonID &&
			pclCM->CR[id]->pclCI->clBI.siIDNum == pclMsg->siIDNum )
		{
			if ( pclMsg->strRecommenderUserName != NULL || _tcslen( pclMsg->strRecommenderUserName ) > 0 )
			{
				cltLetterMsg_Recommender clletter( ( TCHAR*) pclMsg->strRecommenderUserName );
				SendLetterMsg(pclMsg->siRecommendPersonID, (cltLetterHeader*)&clletter );
			}

			pclCM->CR[id]->pclCI->clBI.clRecommenderInfo.siPersonID = pclMsg->siRecommendPersonID;

			cltGameMsgResponse_RecommendPerson clinfo( pclMsg->strUserName );
			cltMsg clMsg(GAMEMSG_RESPONSE_RECOMMENDPERSON, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [�α��߰� : Ȳ���� 2007. 9. 10] // ��õ�� ���(��õ���� ���)
				//pszParam1 == ��õ�� �̸�.
				SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_RECOMMENDER_ADD, 
					0, (cltCharServer*)(pclCM->CR[id]), NULL, pclMsg->siRecommendPersonID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
					0, 0, 0, 0, 0, pclMsg->strRecommenderUserName, NULL);
			}

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [�α��߰� : Ȳ���� 2007. 9. 7] // ��õ�� ����
			//	//pszParam1 == ����õ�� �̸�
			//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_RECOMMENDER_RECOMMENDPERSON, 
			//		0, (cltCharServer*)(pclCM->CR[id]), NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
			//		0, 0, 0, 0, 0, pclMsg->strUserName, NULL);
			//}
		}


		return;
	}
}



void cltServer::DoMsg_DBMSG_RESPONSE_SETHORSE(sPacketHeader* pPacket)
{
	sDBResponse_SetHorse *pclMsg = (sDBResponse_SetHorse *)pPacket;
	
	if( pclMsg->siResult != 1 )						return;
	
	SI32 id	= pclMsg->usCharID;

	// PCK : ���� - �� ���� �α� ���� (Reason�� �ִºκ��� �α׷� ��� ������ ����.)
	if(pclCM->IsValidID(id) == TRUE && pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		TCHAR	szHorseInfo[50] = {'\0', };
		StringCchPrintf(szHorseInfo, sizeof(szHorseInfo), "Life : %d,  Str : %d, Dex : %d, Mag : %d, MoveSpeed : %d",
			pclMsg->cIHorse.siLife, pclMsg->cIHorse.siStr, pclMsg->cIHorse.siDex, pclMsg->cIHorse.siMag, pclMsg->cIHorse.siMoveSpeed);

		cltServer * pclServer = (cltServer*)pclClient;

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 10] // �� ����.
			// pszParam1 == ���� ����

			cltServer * pclServer = (cltServer*)pclClient;
			pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HORSESET, LOGCOMMAND_INDEX_HORSESET_SET,
				pclMsg->siReason, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, szHorseInfo, NULL);
		}
	}
	
	// ���� �׾������� ó���Ѵ�.
	if(pclMsg->siReason	 != SETHORSE_REASON_DIE)	return;

	if(pclCM->IsValidID(id) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		//cyj �� ������ ParametaBox�����ϵ��� �߰�
		pclCM->CR[id]->UnrideHorse();
		pclCM->CR[id]->SetParametaBoxAction(true);	// ParametaBoxAction�� �����Ѵ�. 

		((cltCharServer*)pclCM->CR[id])->SendNeighbourMyHorseStatus(pclCM->CR[id]->pclCI->clHorseInfo.siCurrentShowHorse);

		// Ŭ���̾�Ʈ�� ����� �˸���. 
		SendServerResponseMsg(0, SRVAL_HORSEDIE,  0, 0, pclCM->CR[id]->GetCharUnique());
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETAUTOFOOD(sPacketHeader* pPacket)
{
	sDBResponse_SetAutoFood *pclMsg = (sDBResponse_SetAutoFood*)pPacket;

	// Ư���� �� ���� ����. 
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETRIDEHORSE(sPacketHeader* pPacket)
{
	sDBResponse_SetRideHorse *pclMsg = (sDBResponse_SetRideHorse *)pPacket;

	if( pclMsg->siResult != 1 )						return;

	SI32 id = pclMsg->usCharID;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

	if(pclCM->IsValidID(id) == false)				return;
	if(pclchar->pclCI->GetPersonID() != pclMsg->siPersonID)return ;

//	if(pclMsg->siIndex >= 0)
	if(pclMsg->siCurrentHorseRIDEHORSE == true)
	{
		
		// ����ڿ��� ���� �¿��.
		if( pclCM->CR[id]->RideHorse() == false)return ;

		pclCM->CR[id]->SetParametaBoxAction(true);	// ParametaBoxAction�� �����Ѵ�. 

		cltGameMsgResponse_RideOrUnrideHorse clinfo(pclMsg->siIndex);
		cltMsg clMsg(GAMEMSG_RESPONSE_RIDEHORSE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}
	else
	{
		pclCM->CR[id]->SetParametaBoxAction(true);	// ParametaBoxAction�� �����Ѵ�. 

		cltGameMsgResponse_RideOrUnrideHorse clinfo(pclMsg->siIndex);
		cltMsg clMsg(GAMEMSG_RESPONSE_UNRIDEHORSE, sizeof(clinfo), (BYTE*)&clinfo);

		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	
	//---------------------------------------
	// �̿��鿡�� ���� ������ �˸���. 
	//---------------------------------------
	((cltCharServer*)pclCM->CR[id])->SendNeighbourMyShapeStatus();
	
}

// �� �ü� ����
void cltServer::DoMsg_DBMSG_RESPONSE_SETCURRENTHORSEPRICE(sPacketHeader* pPacket)
{
	sDBResponse_SetCurrentHorsePrice *pclMsg = (sDBResponse_SetCurrentHorsePrice *)pPacket;
	
	if ( pclMsg->siResult != 1 )
	{
		pclLog->FilePrint( TEXT("Config/CurrentHorsePrice.log"), TEXT("DoMsg_DBMSG_RESPONSE_SETCURRENTHORSEPRICE pclMsg->siResult = %d"), pclMsg->siResult );
		return;
	}

	if ( pclMsg->siHorseUnique > 0 )
	{
		bHorseCurrentPriceSet = true;
		siHorseTradeCount = 0;
		siHorseCurrentPrice = pclMsg->siHorseCurrentPrice;
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [�α��߰� : Ȳ���� 2007. 10. 11] // �� �ü�.
		// param1 == �� ����, param2 == �� ���� ����.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HORSEMARKET, LOGCOMMAND_INDEX_HORSEMARKET_PRICESET, 
										0, NULL, NULL, 0, 0, 0, NULL, 0, NULL, 0, 0, 0, 0, 
										pclMsg->siHorseUnique, pclMsg->siHorseCurrentPrice, 0, 0, 0, NULL, NULL);

	}

	return;
}

// �� �ü� ����
void cltServer::DoMsg_DBMSG_RESPONSE_GETCURRENTHORSEPRICE(sPacketHeader* pPacket)
{
	sDBResponse_GetCurrentHorsePrice *pclMsg = (sDBResponse_GetCurrentHorsePrice *)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		pclLog->FilePrint( TEXT("Config/CurrentHorsePrice.log"), TEXT("DoMsg_DBMSG_RESPONSE_GETCURRENTHORSEPRICE pclMsg->siResult = %d"), pclMsg->siResult );
		return;
	}

	if ( pclMsg->siHorseUnique > 0 )
	{
		bHorseCurrentPriceSet = true;
		siHorseCurrentPrice = pclMsg->siHorseCurrentPrice;
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_HORSENAMECHANGE(sPacketHeader* pPacket)
{
	sDBResponse_HorseNameChange *pclMsg = (sDBResponse_HorseNameChange *)pPacket;

	if ( pclMsg->siResult != 1 )
	{
		return;
	}

	if(pclMsg->siHorseIndex < 0 || pclMsg->siHorseIndex >= MAX_HORSE_PER_PERSON)
		return;

	SI32 id	= pclMsg->siCharID;

	if(pclCM->IsValidID(id) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if ( pclMsg->strHorseName == NULL || _tcslen( pclMsg->strHorseName ) < 1 )
		{
			return;
		}

		cltHorse* pclHorse = pclCM->CR[id]->pclCI->clHorseInfo.GetHorse(pclMsg->siHorseIndex);
		if(pclHorse != NULL)
		{
			MStrCpy( pclHorse->szName, pclMsg->strHorseName, MAX_HORSE_NAME );
		}

		cltGameMsgResponse_HorseNameChange clinfo( pclMsg->siHorseIndex, pclMsg->strHorseName );
		cltMsg clMsg(GAMEMSG_RESPONSE_HORSENAMECHANGE, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		// Ŭ���̾�Ʈ�� �����ϰ� �Ѵ�. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_HORSE, true, 0);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 15] // �� �̸� ����.
			// param1 == �� ��ȣ, pszParam1 == �� �̸�.
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_HORSESET, LOGCOMMAND_INDEX_HORSESET_NAMECHANGE, 
											0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0,
											pclMsg->siHorseIndex, 0, 0, 0, 0, pclMsg->strHorseName, NULL);
		}
	}
	
	return;
}

// Person�� ��ų ������ ���δ�. 
void cltServer::DoMsg_DBMSG_RESPONSE_INCREASEMASTERSKILL(sPacketHeader* pPacket)
{
	sDBResponse_IncreaseMasterSkill* pclMsg = (sDBResponse_IncreaseMasterSkill*)pPacket;

	if(pclMsg->siResult == 0)
	{
		return ;
	}

	SI32 id	= pclMsg->usCharID;
	if(pclCM->IsValidID(id) == FALSE)return ;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 11] // �ش� ��ų �������� ������ ����. <MASTER>. 
		// param1 == ������ ��ų ����ũ.
		// param2 == ������ ��ų ����.
		cltServer * pclServer = (cltServer*)pclClient;
		pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_SKILL, LOGCOMMAND_INDEX_SKILL_INCREASE_SKILL_MASTER, 
										0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										pclMsg->uiSkillUnique, pclMsg->uiSkillLevel, 0, 0, 0, NULL, NULL);
	}
	
	if(pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if( pclMsg->uiSkillUnique < 0 || pclMsg->uiSkillUnique >= MAX_SKILL_NUMBER)		return;

		// Person�� ��ų ������ ������Ʈ �Ѵ�. 
		SI32 oldskilllevel		= pclCM->CR[id]->pclCI->clCharSkill.clSkill[pclMsg->uiSkillUnique].uiLevel;
		SI32 oldtotalskilllevel	= oldskilllevel + pclCM->CR[id]->pclCI->clCharSkill.GetSkillExpLevel(pclMsg->uiSkillUnique);

		pclCM->CR[id]->pclCI->clCharSkill.clSkill[pclMsg->uiSkillUnique].uiSkillUnique	= pclMsg->uiSkillUnique;
		pclCM->CR[id]->pclCI->clCharSkill.clSkill[pclMsg->uiSkillUnique].uiLevel		= pclMsg->uiSkillLevel;

		// Ŭ���̾�Ʈ�� �����ϰ� �Ѵ�. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_SKILL, true, 0);

		// Ŭ���̾�Ʈ�� �����ϰ� �Ѵ�. 
		pclCM->CR[id]->SetUpdateSwitch(UPDATE_IP, true, 0);
		
		// ������ ������ �ö��ٸ�, 
		SI32 totalskilllevel = pclMsg->uiSkillLevel + pclCM->CR[id]->pclCI->clCharSkill.GetSkillExpLevel(pclMsg->uiSkillUnique);

		if(oldtotalskilllevel < totalskilllevel)
		{
			SendServerResponseMsg(0, SRVAL_SETSKILL,  pclMsg->uiSkillUnique, totalskilllevel, pclCM->CR[id]->GetCharUnique());

			if ( IsCountrySwitch(Switch_Server_Event) )
			{
				CNServerEvent_Quest* pclSE_Quest =  (CNServerEvent_Quest*)pclServerEventManager->GetServerEvent_Quest();
				if ( pclSE_Quest )
				{
					SI32 siAmount = max( 1, ((cltCharServer*)pclCM->CR[id])->siQuestCount );
					pclSE_Quest->IncServerEventQuestCondition( SERVEREVENT_TYPE_SKILLUP, totalskilllevel, siAmount );
				}
			}
		}

	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_GUILTYINFORECORD(sPacketHeader* pPacket)
{
	sDBResponse_GuiltyInfoRecord *pclMsg = (sDBResponse_GuiltyInfoRecord *)pPacket;

	if ( pclMsg->siResult < 1 )	return;

	SI32 id	= pclMsg->siCharID;

	if(pclCM->IsValidID(id) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 15] // DB�� ���� ������ ����.
			// param1 == �� �����ð�
			// param2 == ���� �����ð�
			// param3 == �� ������ ����Ƚ��
			// pszparam1 == ���� ����
			cltServer* pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PRISON, LOGCOMMAND_INDEX_GUILTY_INFORECORD, 
											0, NULL, NULL, pclMsg->siPersonID, 0, 0, NULL, 0, 0, 0, 0, 0, 0,
											pclMsg->cIGuiltyInfo.siTotalPrisonSecond, pclMsg->cIGuiltyInfo.siLeftPrisonSecond, pclMsg->cIGuiltyInfo.siTotalPrisonNum, 0, 0, 
											pclMsg->cIGuiltyInfo.szPrisonReason, NULL);
		}
	}

	return;
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETMINISTER(sPacketHeader* pPacket)
{
	sDBResponse_SetMinister * pclMsg = ( sDBResponse_SetMinister*)pPacket;

	SI32 userid = 0 ;

	if ( pclMsg->siResult == 1 )
	{
		// �ڵ��� �ƴ� �������� ����Ŭ�ι��� �����Ѵ�.
/*		if(pclClient->IsCountrySwitch(Switch_FourLeaf))
		{
			//[����] ���� ����Ǹ� ���� ��밡���� ����ũ�ι��� �����Ѵ�.
			if( GRANK_KING == pclMsg->siMinister )
			{
				SI32 siCurFourLeafUnique = 0;
				
					 if( ITEMUNIQUE(2755) == siFourLeafUnique )	siCurFourLeafUnique = ITEMUNIQUE(2754);
				else if( ITEMUNIQUE(2754) == siFourLeafUnique ) siCurFourLeafUnique = ITEMUNIQUE(2755);
				else											siCurFourLeafUnique = ITEMUNIQUE(2754);	// ���� �ƹ��͵� �ƴϸ� �ϳ� ����.	
				
				sDBRequest_SetFourLeaf clMsg( siCurFourLeafUnique );
				SendDBMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
			}
		}*/

		cltSimpleRank pclrank ;
		pclrank.Init();

		if ( pclMsg->siTempPersonId )
		{
			userid = pclCM->GetIDFromPersonID(pclMsg->siTempPersonId);
			if ( pclCM->IsValidID(userid) )
			{
				pclCM->CR[userid]->pclCI->clBI.uiGRank = 0 ;
				SendServerResponseMsg(0, SRVAL_MINISTER_FIRE_MINISTER,  0, 0, pclCM->CR[userid]->GetCharUnique());
			}

			cltLetterMsg_Fire clletter(&pclrank,&pclTime->clDate,pclMsg->siMinister);
			SendLetterMsg(pclMsg->siTempPersonId, (cltLetterHeader*)&clletter);
		}

		if ( pclMsg->siMinisterPersonId )
		{
			userid = pclCM->GetIDFromPersonID(pclMsg->siMinisterPersonId);
			if ( pclCM->IsValidID(userid) )
			{
				pclCM->CR[userid]->pclCI->clBI.uiGRank = pclMsg->siMinister ;
				SendServerResponseMsg(0, SRVAL_MINISTER_APPOINTMENT_SETMINISTER,  pclMsg->siMinister, 0, pclCM->CR[userid]->GetCharUnique());
			}

			cltLetterMsg_BeRank clletter(&pclrank,&pclTime->clDate,pclMsg->siMinister);
			SendLetterMsg(pclMsg->siMinisterPersonId, (cltLetterHeader*)&clletter);

			// [����] �ʺ��� ���� : ������ ��� �ڵ������� �ʺ��� ������ ���ְ� �ǵ��� �Ѵ�
			if ( pclClient->IsCountrySwitch(Switch_TutorialVillage) )
			{
				if ( pclMsg->siMinister == GRANK_MINISTER2 )
				{
					BegnnerVillage_SetVillageChief( pclMsg->siMinisterPersonId );
				}
			}
		}

		if ( pclMsg->siPersonID <= 0 ) return ;
		userid = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
		if ( pclCM->IsValidID(userid) == false ) return ;

		//KHY - 0629 - ���� ���� ���� �߰�.
		if ( pclMsg->siTempPersonId )
			SendServerResponseMsg(0, SRVAL_MINISTER_FIRE_SUCCESS_SETMINISTER,  0, 0, pclCM->CR[userid]->GetCharUnique());
		else
			SendServerResponseMsg(0, SRVAL_MINISTER_SUCCESS_SETMINISTER,  0, 0, pclCM->CR[userid]->GetCharUnique());

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 10] // �Ǽ� ����.
			// param1 == ����.
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_SET, 
											0, (cltCharServer*)pclCM->CR[userid], NULL, pclMsg->siMinisterPersonId, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclMsg->siMinister, 0, 0, 0, 0, NULL, NULL);
		}
	}
	else
	{
		if ( pclMsg->siPersonID <= 0 ) return ;
		userid = pclCM->GetIDFromPersonID(pclMsg->siPersonID);
		if ( pclCM->IsValidID(userid) == false ) return ;

		SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETMINISTER,  0, 0, pclCM->CR[userid]->GetCharUnique());
	}

	if ( pclMinisterMgr )
		pclMinisterMgr->InitializeDB();
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETLIQUIDATION(sPacketHeader* pPacket)
{
	sDBResponse_SetLiquidation * pclMsg = ( sDBResponse_SetLiquidation*)pPacket;

	SI32 userid = 0 ;

	if ( pclMsg->siResult == 1 )
	{
		if ( pclMsg->siPersonID <= 0 ) return ;
		userid = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

		if (pclCM->IsValidID(userid))
			SendServerResponseMsg(0, SRVAL_MINISTER_SUCCESS_SETLIQUIDATION,  0, 0, pclCM->CR[userid]->GetCharUnique());
	}
	else
	{
		if ( pclMsg->siPersonID <= 0 ) return ;
		userid = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

		if (pclCM->IsValidID(userid))
			SendServerResponseMsg(0, SRVAL_MINISTER_FAIL_SETLIQUIDATION,  0, 0, pclCM->CR[userid]->GetCharUnique());
	}

	if ( pclMinisterMgr )
		pclMinisterMgr->InitializeDB();
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETBONUSTIME(sPacketHeader* pPacket)
{
	sDBResponse_Goonzu_SetBonusTime * pclMsg = (sDBResponse_Goonzu_SetBonusTime *)pPacket;

	SI32 userid = 0 ;

	if ( pclMsg->siResult == 1 )
	{
		if ( pclMsg->siPersonID <= 0 ) return ;
		userid = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

		if (pclCM->IsValidID(userid))
			SendServerResponseMsg(0, SRVAL_MINISTER_SUCCESS_SETBONUS,  0, 0, pclCM->CR[userid]->GetCharUnique());

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		{
			// [�α��߰� : Ȳ���� 2007. 10. 16] // ���ʽ� Ÿ�� ����.
			// param1 == ���� ���ʽ� Ÿ�� ���� �ð�.
			// param2 == ���� ���ʽ� Ÿ�� ���� �ð�
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_MINISTER, LOGCOMMAND_INDEX_MINISTER_GOONZU_BONUS_SET, 
											0, (cltCharServer*)pclCM->CR[userid], NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
											pclMsg->siBonusTime, pclMsg->siMakeBonusTime, 0, 0, 0, NULL, NULL);
		}

	}
	else
	{
		if ( pclMsg->siPersonID <= 0 ) return ;
		userid = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

		if (pclCM->IsValidID(userid))
			SendServerResponseMsg(0,  SRVAL_MINISTER_FAIL_SETBONUS,  0, 0, pclCM->CR[userid]->GetCharUnique());
	}

	if ( pclMinisterMgr )
		pclMinisterMgr->InitializeDB();

}


void cltServer::DoMsg_DBMSG_RESPONSE_PRIZE_GAMEEVENT(sPacketHeader* pPacket)
{
	sDBResponse_Prize_GameEvent * pclMsg = ( sDBResponse_Prize_GameEvent*)pPacket;

	if ( pclMsg->siResult != 1 ) return;

	SI32 id	= pclMsg->usCharID ;

	if(pclCM->IsValidID(id) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		pclCM->CR[id]->pclCI->clBI.bGameEvent = pclMsg->bGameEvent ;
		pclCM->CR[id]->pclCI->clBI.bWinPrizeEvent = pclMsg->bWinPrizeEvent ;

/*		if ( pclCM->CR[id]->pclCI->clBI.bWinPrizeEvent == false ) return ; // �̺�Ʈ�� ���̸� �׳� ������.

		SI32 itemunique , itemnum ;

		pclGameEventManager->GetItemUniqueAndNumRamdomSelect(&itemunique,&itemnum);

		cltItem clItem;
		bool brareswitch = false;
		if(pclItemManager->MakeRandItemUnique( itemunique, &clItem, 0, &brareswitch) == true)
		{
			clItem.SetItemNum(itemnum);

			TCHAR buffer[256] = TEXT("") ;
			sprintf(buffer, TEXT("�̺�Ʈ ��÷���� [%s] [%d]���� �޾ҽ��ϴ�."),clItem.GetShortName(pclItemManager),itemnum);
			SendPostOfficeItemToPerson((TCHAR*)pclCM->CR[id]->GetName(), &clItem, buffer);

			pclLog->FilePrint(TEXT("Config\\BigItem.txt"), TEXT("���� �̺�Ʈ ��÷ : [%s] [%s] [%d]��"), 
				pclCM->CR[id]->GetName(), clItem.GetShortName(pclItemManager), clItem.GetItemNum());
		}*/
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SET_WEAPONSKILL(sPacketHeader* pPacket)
{
	sDBResponse_Set_WeaponSkill* pclMsg = (sDBResponse_Set_WeaponSkill*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
    
	cltCharServer* pclchar = pclCM->GetCharServer( pclMsg->usCharID );
	if ( NULL == pclchar )
	{
		return;
	}

	if ( pclMsg->siPersonID != pclchar->pclCI->GetPersonID() )
	{
		return;
	}

	if ( 1 > pclMsg->siResult )
	{
		pclchar->bIsWeaponSkillUp = false;	// �� ���� �� Ǯ����ߵ�
		return;
	}
	
	UI08 uiSkillUnique = pclMsg->uiSkillUnique;
	if ( (0 == uiSkillUnique) || (MAX_SKILL_NUMBER <= uiSkillUnique) )
	{
		return;
	}

	//------------------------------- ���� ���� ������Ʈ -------------------------------

	// ��ų ���� ������Ʈ
	pclchar->pclCI->clCharSkill.clSkill[uiSkillUnique].uiSkillUnique	= uiSkillUnique;
	pclchar->pclCI->clCharSkill.clSkill[uiSkillUnique].uiLevel			= pclMsg->uiSkillLevel;
	pclchar->SetUpdateSwitch( UPDATE_SKILL, true, 0 );

	// ��ų ����Ʈ ������Ʈ
	pclchar->pclCI->clIP.SetSkillBonus( pclMsg->siLeftBonus );
	// ������ ������Ʈ
	pclchar->pclCI->clIP.SetMoney( pclMsg->siLeftMoney );
	pclchar->SetUpdateSwitch( UPDATE_IP, true, 0 );

	// ������ ���� ������Ʈ
	SI16 siItemPos = pclMsg->siItemPos;
	if ( (PERSONITEM_INV0 <= siItemPos) && (MAX_ITEM_PER_PERSON > siItemPos) )
	{
		pclchar->pclCI->clCharItem.clItem[siItemPos].Set( &pclMsg->clLeftItem );
	}

	// �̰� �� Ǯ����ߵ�
	pclchar->bIsWeaponSkillUp = false;

	//------------------------------- Ŭ�� ���� ������Ʈ -------------------------------
	cltGameMsgResponse_Set_WeaponSkill clinfo( pclMsg->siResult, uiSkillUnique, pclMsg->uiSkillLevel, &pclMsg->clLeftItem, siItemPos );
	cltMsg clMsg(GAMEMSG_RESPONSE_SET_WEAPONSKILL, sizeof(clinfo), (BYTE*)&clinfo);
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	//------------------------------- �α� -------------------------------
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 11] // ȫ�浿���κ��� ��ų�� ��� ��� ��ϵ�.
		// param1 == ��� ����ũ, 
		// param2 == ��� ����, 
		// param3 == �ʿ� ��ų ����Ʈ, 
		// param4 == ���� ��ų ����Ʈ, 
		// param5 == ������ �κ� ��ġ

		SendLog_WriteLogToDB( LOGCOMMAND_TYPE_SKILL, LOGCOMMAND_INDEX_SKILL_INCREASE_SKILL_WEAPON, 0,
			pclchar, NULL, 0, pclMsg->siMoney, pclMsg->siLeftMoney, &pclMsg->clNeedItem, pclMsg->clLeftItem.GetItemNum(), 
			0, 0, 0, 0, 0, 
			uiSkillUnique, pclMsg->uiSkillLevel, pclMsg->siNeedSkillBonus, pclMsg->siLeftBonus, pclMsg->siItemPos, NULL, NULL
		);
	}
	
}

void cltServer::DoMsg_DBMSG_RESPONSE_COUNTATTACK_INFO(sPacketHeader* pPacket)
{
/*	sDBResponse_CountAttack_Info* pclMsg = (sDBResponse_CountAttack_Info*)pPacket;

	SI32 id = pclMsg->usCharID ;

	SI32 giveitemunique = 0 ;
	SI32 giveitemnum = 0 ;
	cltItem clItem;

	if(pclCM->IsValidID(id) == TRUE && pclCM->CR[id]->pclCI->GetPersonID() == pclMsg->siPersonID)
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
		if ( pclchar == NULL ) return ;

		if ( pclMsg->siResult >= 1 )
		{
			if ( pclMsg->siItemPos )
			{
				pclchar->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clitem);

				if ( pclMsg->siFlagNum >= 50 && pclMsg->siBeforeNum < 50 )
				{
					giveitemunique = pclItemManager->FindUniqueFromName(TEXT("�����ǹ���"));
					giveitemnum = 1 ;
				}
				else if ( pclMsg->siFlagNum >= 40 && pclMsg->siBeforeNum < 40 )
				{
					giveitemunique = pclItemManager->FindUniqueFromName(TEXT("�ݱ�"));
					giveitemnum = 100 ;
				}
				else if ( pclMsg->siFlagNum >= 30 && pclMsg->siBeforeNum < 30 )
				{
					giveitemunique = pclItemManager->FindUniqueFromName(TEXT("��ȯ��[��ȭ]�̿��"));
					giveitemnum = 1 ;
				}
				else if ( pclMsg->siFlagNum >= 20 && pclMsg->siBeforeNum < 20 )
				{
					giveitemunique = pclItemManager->FindUniqueFromName(TEXT("���ɷ�����̿��"));
					giveitemnum = 1 ;
				}
				else if ( pclMsg->siFlagNum >= 10 && pclMsg->siBeforeNum < 10 )
				{
					giveitemunique = pclItemManager->FindUniqueFromName(TEXT("�����̵�[1��]���ױ�"));
					giveitemnum = 1 ;
				}
				else if ( pclMsg->siFlagNum >= 1 && pclMsg->siBeforeNum < 1 )
				{
					giveitemunique = pclItemManager->FindUniqueFromName(TEXT("��ȭ��"));
					giveitemnum = 10 ;
				}

				bool brareswitch = false;
				if(pclItemManager->MakeRandItemUnique( giveitemunique, &clItem, 0, &brareswitch) == true)
				{
					clItem.SetItemNum(giveitemnum);

					SI32 rtnvalue = 0;
					if(pclchar->InputItem(pclItemManager, &clItem, INPUTITEMPERSON_OPTION_COUNTATTACK, &rtnvalue) == false)
					{
						SendLog_InventoryFull( pclchar->pclCI->GetPersonID(),giveitemunique,giveitemnum,
							INPUTITEMPERSON_OPTION_COUNTATTACK);
					}
				}
			}

			clItem.Init();
			clItem.siItemNum = giveitemnum ;
			clItem.siUnique = giveitemunique ;

			cltGameMsgResponse_CountAttack_Info clinfo(pclMsg->siFlagNum,&clItem,pclMsg->siItemPos,&pclMsg->clitem);
			cltMsg clMsg(GAMEMSG_RESPONSE_COUNTATTACK_INFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclchar->SendNetMsg((sPacketHeader*)&clMsg);
		}

		pclchar->bCheckGameEvent = false ;
	}*/
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETGREETING(sPacketHeader* pPacket)
{
	sDBResponse_SetGreeting* pclMsg = (sDBResponse_SetGreeting*)pPacket;

	SI32 id = pclMsg->siCharID;

	if( pclCM->IsAlive(id) == false)
		return;

	if(pclMsg->siResult)
	{
		cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];

		MStrCpy(pclchar->pclCI->clBI.szGreeting, pclMsg->szText,128);

		cltGameMsgResponse_GetGreeting clinfo( 0, TEXT(""), pclMsg->szText );
		cltMsg clMsg(GAMEMSG_RESPONSE_GETGREETING, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

		pclCM->CR[id]->DoQuestCheckAndAction(QUEST_CLEAR_COND_SETGREETING);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMAKERNAMENEW(sPacketHeader* pPacket)
{
	sDBResponse_GetItemMakerNameNew* pclMsg = (sDBResponse_GetItemMakerNameNew*)pPacket;

	SI32 id = pclMsg->siCharID;

	if (pclCM->IsValidID(id) == false)		return;

	if ( pclMsg->szName == NULL )			return;

	// Ȥ�� DB���� �̸��� ���ٸ�
	if ( _tcscmp(pclMsg->szName, TEXT("")) == 0 )
	{
		StringCchCopy( pclMsg->szName, sizeof(pclMsg->szName), TEXT("[NONAME]") );

		if ( 0 >= pclMsg->siSerialID )
		{
			pclMsg->siSerialID = pclMsg->siDupSerialID;
		}
	}
	
	cltPersonNameInfo clPersonInfo;
	if( false == pclNewPersonNameManager->Find( pclMsg->siSerialID, &clPersonInfo ) )
	{
		clPersonInfo.siServerUnique = pclMsg->siServerUnique;
		StringCchCopy( clPersonInfo.szPersonName, sizeof(clPersonInfo.szPersonName), pclMsg->szName );

		pclNewPersonNameManager->Insert( pclMsg->siSerialID, &clPersonInfo );
	}

	cltGameMsgResponse_GetNameFromItemSerialID clPersonName( pclMsg->siSerialID, clPersonInfo.szPersonName, clPersonInfo.siServerUnique );
	cltMsg clMsg( GAMEMSG_RESPONSE_PERSONNAME_SERIALID, sizeof(clPersonName), (BYTE*)&clPersonName );

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DoMsg_DBMSG_RESPONSE_ITEMMAKERNAME(sPacketHeader* pPacket)
{
	sDBResponse_GetItemMakerName* pclMsg = (sDBResponse_GetItemMakerName*)pPacket;

	SI32 id = pclMsg->siCharID;

	if (pclCM->IsValidID(id) == false)
		return;

	if (pclMsg->szName == NULL)
		return;

	TCHAR buf[MAX_PLAYER_NAME];

	if (pclPersonNameManager->Find(pclMsg->siPersonID, buf) == false)
	{
		StringCchCopy(buf, MAX_PLAYER_NAME, pclMsg->szName);

		pclPersonNameManager->Insert(pclMsg->siPersonID, buf);

	}

	cltGameMsgResponse_GetNameFromPersonID clPersonName(pclMsg->siPersonID, buf);
	cltMsg clMsg(GAMEMSG_RESPONSE_PERSONNAME_FROMPERSONID, sizeof(clPersonName), (BYTE*)&clPersonName);

	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}

void cltServer::DOMSG_DBMSG_RESPONSE_CHANGERECOMMENDPOINT(sPacketHeader* pPacket)
{
	sDBResponse_ChangeRecommendPoint * pclMsg = (sDBResponse_ChangeRecommendPoint*)pPacket;
		
	//����Ʈ ���� ���� ���� ���� üũ�� ����
	if(pclMsg->siResult != 1) return;	
	
	// �ش� �������� ������ ������. 
	cltLetterMsg_ChangeRecommendPoint clletter( pclMsg->szCharName,pclMsg->siLevel, pclMsg->siChangedRecommendPoint, pclMsg->siRecommendPoint );
	//SI32 letterid = SendLetterMsg( pclMsg->siRecommendPersonID , (cltLetterHeader*)&clletter);
	
	cltLetterUnit clletterunit((cltLetterHeader*)&clletter);

	SI32 letterid = pclCM->GetIDFromPersonID(pclMsg->siRecommendPersonID);
	// ���ӿ� �������̸� Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
	if(pclCM->IsValidID(letterid))
	{
		pclCM->CR[letterid]->SendLetterMsg(&clletterunit);
	}
	else 
	{
		if( pclMsg->siLevel > 50 )
		{
			sDBRequest_PushLetter clMsg(pclMsg->siRecommendPersonID, &clletterunit);
			pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg);
		}
	}

	// �޽��� ���� ������� üũ
	if(pclCM->IsValidID(pclMsg->siCharID) == false) return;
	if(pclCM->CR[pclMsg->siCharID]->pclCI->GetPersonID() != pclMsg->siPersonID ) return;

#ifdef USE_PROMOTOR_SERVER
	// �߱��� ���θ��Ϳ� �ڵ��� ������ �Ǿ߸� �����۰� ����Ʈ�� �ش�.	
	if( /*true == pclCM->CR[pclMsg->siCharID]->bCellularPhoneAuth &&*/ true == pclCM->CR[pclMsg->siCharID]->bIsPromoter )
#endif
	{
		// ��õ���� ����� �ξ��ٸ� ��õ�ο��� ������ ����, �߱� ����
		if ( true == pclEventTimeManager->InEventTime( TEXT("RecommendReward"), &sTime ) )
		{
			if ( pclRewardMgr )
			{
				pclRewardMgr->GiveRecommendRewardItem( pclMsg->siRecommendPersonID, pclMsg->siLevel, false, (TCHAR*)pclCM->CR[pclMsg->siCharID]->GetName(), pclCM->CR[pclMsg->siCharID]->bCellularPhoneAuth );
			}
		}
		else
		{
			if( IsWhereServiceArea( ConstServiceArea_China ) == false  )
			{
				if((  pclMsg->siLevel == 20 || pclMsg->siLevel == 30  || pclMsg->siLevel == 40 || pclMsg->siLevel == 50 )) 
				{
					SI32 itemunique = 0; //
					SI32 itemnum = 0;
					SI32 RndValue = rand()%4;

					switch(pclMsg->siLevel)
					{
						case 20:	itemunique = ITEMUNIQUE(5350 + RndValue*5 );  //5350 , 5355, 5360, 5365 �� �������� �ϳ� ����
									itemnum = 3;	
									break;
						case 30:	itemunique = ITEMUNIQUE(5350 + RndValue*5 );  //5350 , 5355, 5360, 5365 �� �������� �ϳ� ����
									itemnum = 6;	
									break;
						case 40:	itemunique = ITEMUNIQUE(5350 + RndValue*5 );  //5350 , 5355, 5360, 5365 �� �������� �ϳ� ����	
									itemnum = 9;
									break;
						case 50:	itemunique = ITEMUNIQUE(5370);		
									itemnum = 10; 	
									break;
					}

					SI32 ref = pclItemManager->FindItemRefFromUnique(itemunique);

					if(itemunique > 0 && itemnum > 0 && ref > 0)
					{
						UI16 usedate = pclItemManager->pclItemInfo[ref]->clItem.uiDateUseDay;

						//  �������� ������.
						cltItem clInputItem;
						SI16 bRareSwitch = 0;
						pclItemManager->MakeRandItemUnique(itemunique,&clInputItem,0,0,&bRareSwitch);

						clInputItem.siItemNum = itemnum;

						//((cltServer*)pclClient)->SendPostOfficeItemToPerson( pclCI->GetPersonID(), &clInputItem , NULL ,0  );

						sDBRequest_PostOfficeMasterSendItem clMsg( INPUTITEMPERSON_OPTION_RECOMMEND, pclMsg->siRecommendPersonID , NULL, &clInputItem, &((cltServer*)pclClient)->pclTime->clDate );
						pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

						cltLetterMsg_LevelUpREcommandRewardForRecommender clletter( (TCHAR*)pclCM->CR[pclMsg->siCharID]->GetName()  , pclMsg->siLevel, itemunique , itemnum );
						SendLetterMsg(  pclMsg->siRecommendPersonID , (cltLetterHeader*)&clletter );

					}
				}
			}	// if( IsWhereServiceArea( ConstServiceArea_China ) == false  ) ����
		}
	}
#ifdef USE_PROMOTOR_SERVER
	// �߱��� ���θ��Ͱ� �����鼭 �ڵ��� ������ �Ǿ߸� �����۰� ����Ʈ�� �ش�.	
	else if( true == pclCM->CR[pclMsg->siCharID]->bIsPromoter )
	{
		// �ڵ��� ������ �ȵǼ� �ټ� �ִ� �������� �ִµ� ���ٶ� �޽����� ������.
		if( pclRewardMgr->IsGiveRecommendRewardItem( pclCM->CR[pclMsg->siCharID]->pclCI->GetPersonID(), pclCM->CR[pclMsg->siCharID]->pclCI->clIP.GetLevel(), false, NULL ) )
		{
			SI32 recommenderpersonid = pclCM->CR[pclMsg->siCharID]->pclCI->clBI.clRecommenderInfo.siPersonID;
			if( recommenderpersonid > 0 ) 
			{
				SI32 siID = pclCM->GetIDFromPersonID(recommenderpersonid);
				cltCharServer* pRecommendeeChar = pclCM->GetCharServer( siID );
				if( pRecommendeeChar )
				{
					// �߱��� �ڵ��� ������ �ȵǸ� �ش� �޽����� Ŭ�� �ش�.
					//SendServerResponseMsg(0, SRVAL_PROMOTER_NOT_CELLULAR_PHONE_AUTH, 1, 0, pRecommendeeChar->GetCharUnique());
					
					SendServerResponseCharMsg2( 0, SRVAL_RECOMMENDEE_GIVE_ITEM_FAIL, (TCHAR*)pclCM->CR[pclMsg->siCharID]->GetName(), NULL, NULL, pclCM->CR[pclMsg->siCharID]->GetCharUnique() );		
				}
			}
		}
	}
#endif


    
	
	
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 9] // ��õ�� ���� ����.
		// param1 == ����, Param2 == �߰��� ��õ ����.
		// PCK (08.06.23) - pszCharParam1 ������.
		cltServer * pclserver = (cltServer*)pclClient;
		cltCharServer* pclchar = pclCM->GetCharServer(pclMsg->siCharID);

		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_RECOMMENDER_POINT_SET, 
										pclMsg->siReason, pclchar, NULL, pclMsg->siRecommendPersonID, 0, 0, 0, 0, 0, 0, 0, pclMsg->siRecommendPoint, 0, 
										pclMsg->siLevel, pclMsg->siChangedRecommendPoint, 0, 0, 0, NULL, NULL);	
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETRECOMMANDPOINT(sPacketHeader* pPacket)
{
	sDBResponse_GetRecommendPoint * pclMsg = (sDBResponse_GetRecommendPoint*)pPacket;

	if(pclMsg->siResult != 1) return;

	SI32 id = pclMsg->siCharID;

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID())
		return ;

	cltGameMsgResponse_GetRecommandPoint clRecommandPoint(pclMsg->siResult,pclMsg->siRecommendPoint);
	cltMsg clMsg( GAMEMSG_RESPONSE_GETRECOMMANDPOINT,sizeof(clRecommandPoint),(BYTE*)&clRecommandPoint);
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

}
void cltServer::DoMsg_DBMSG_RESPONSE_SETCOUNTRY( sPacketHeader* pPacket )
{
	sDBResponse_SetCountry* pclMsg = (sDBResponse_SetCountry*)pPacket;
	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID())
		return ;

	if( pclMsg->siResult == -1 )
	{
		// �����ߴٴ� �޼���~
		return;
	}
    
	pclCM->CR[id]->pclCI->clBI.siCountryID = pclMsg->siCountryID;
	
	// ���� ���� �������� �˸���.
	for ( SI32 i = 1; i < MAX_PERSON_NUMBER ; ++i )
	{
		if( i == id )
			continue;

		if ( pclCM->IsValidID(i) == false)
			continue;

		if( pclCM->CR[i]->pclCI->clBI.siCountryID ==  pclMsg->siCountryID )
		{
			cltGameMsgResponse_NoticeCountryUserMake clinfo( id, pclMsg->siCountryID );
			cltMsg clMsg(GAMEMSG_RESPONSE_NOTICECOUNTRYUSERMAKE, sizeof(cltGameMsgResponse_NoticeCountryUserMake), (BYTE*)&clinfo );

			pclClient->pclCM->CR[i]->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

}
void cltServer::DoMsg_DBMSG_RESPONSE_SETSHOWCOUNTRY( sPacketHeader* pPacket )
{
	sDBResponse_SetShowCountry* pclMsg = (sDBResponse_SetShowCountry*)pPacket;
	
	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID())
		return ;

	if( pclMsg->siResult == -1 )
	{
		// �����ߴٴ� �޼���
		//pclCM->CR[id]->SendNetMsg()
		return;
	}

	pclCM->CR[id]->pclCI->clBI.bShowCountry = pclMsg->bShowCountry;
} 

void cltServer::DoMsg_DBMSG_RESPONSE_SETSHOWPUCHIKOMI( sPacketHeader* pPacket )
{
	sDBResponse_SetShowPuchikomi* pclMsg = (sDBResponse_SetShowPuchikomi*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( pclMsg->siPersonID != pclChar->pclCI->GetPersonID())
		return ;

	if( pclMsg->siResult == -1 )
	{
		// �����ߴٴ� �޼���
		//pclCM->CR[id]->SendNetMsg()
		return;
	}

	pclCM->CR[id]->pclCI->clBI.bShowPuchikomi = pclMsg->bShowPuchikomi;


	cltGameMsgResponse_SetShowPuchikomi clinfo(pclMsg->bShowPuchikomi);
	cltMsg clMsg(GAMEMSG_RESPONSE_SETSHOWPUCHIKOMI,sizeof(clinfo),(BYTE*)&clinfo);
	pclCM->CR[id]->SendNetMsg( (sPacketHeader*)&clMsg );
	

	//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	//{
	//	// [�α��߰� : Ȳ���� 2007. 10. 9] // �ڽ��� Ǫġ�ڹ� ����.
	//	// param1 == Ǫġ�ڹ� ���� ����.
	//	cltServer * pclserver = (cltServer*)pclClient;
	//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_PUCHIKIMI_SET, 
	//									0, (cltCharServer *)pclCM->CR[id], NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	//									pclMsg->bShowPuchikomi, 0,  0, 0, 0, NULL, NULL);
	//}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GIVEFP( sPacketHeader* pPacket )
{
	sDBResponse_GiveFP* pclMsg = (sDBResponse_GiveFP*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if( pclCM->IsValidID(id) == false )
		return;

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 9] // ��õ�� ���� ���� �ֱ�.
		// 
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_RECOMMENDER, LOGCOMMAND_INDEX_RECOMMENDER_POINT_SET, 
										pclMsg->siReason, NULL, NULL, pclMsg->siPersonID, 0, 0, 0, 0, 0, 0, 0, pclMsg->siRecommendPoint, 0, 
										0, 0, 0, 0, 0, NULL, NULL);	
	}
}
void cltServer::DoMsg_DBMSG_RESPONSE_ADDBUF( sPacketHeader* pPacket )
{
	sDBResponse_AddBuf* pclMsg = (sDBResponse_AddBuf*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	if( pclCM->IsValidID(id) == false )
		return;

	// ������������ �ٽ� ����Ҽ� �ֵ��� �Ѵ�
	((cltCharServer*)pclCM->CR[id])->bIsNowAddBufThroughDB = false;

	if ( pclMsg->siResult < 0 )
		return;

	pclCM->CR[id]->pclCI->clBI.clBufInfo.AddBuf( pclMsg->siIndex, pclMsg->m_stBuf );
	
	// Ŭ���̾�Ʈ���Ե� �뺸�Ѵ�.
	cltGameMsgResponse_AddBuf clinfo( pclCM->CR[id]->GetCharUnique(), pclMsg->siIndex, pclMsg->m_stBuf );
	cltMsg clMsg(GAMEMSG_RESPONSE_ADDBUF, sizeof(cltGameMsgResponse_AddBuf), (BYTE*)&clinfo );
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
	{
		// [�α��߰� : Ȳ���� 2007. 10. 5] // ���� �߰�. (�����۾� : ���� ����ü ���� �߰� �۾�.)
		// param1 == ��ȣ, 
		// param2 == �����������
		// param3 == ���� Ÿ��
		// param4 == ������� �ð�(���ӽð�)
		// param5 == ���� ������ ����ũ
		// charparam1 == ���ӽð�
		TCHAR szCharParam1[50] = { '\0', };
		StringCchPrintf(szCharParam1, sizeof(szCharParam1), "Y:%03d M:%02d D:%02d H:%02d", pclTime->clDate.uiYear, pclTime->clDate.uiMonth, pclTime->clDate.uiDay, pclTime->clDate.uiHour);

		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_BUF_ADD, 
										0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &pclMsg->clUsedItem, 0, 0, 0, 0, 0, 0, 
										pclMsg->siIndex, pclMsg->m_stBuf.siGetType, pclMsg->m_stBuf.siBufType, pclMsg->m_stBuf.siOverTime, pclMsg->clUsedItem.siUnique, szCharParam1, NULL);
	}


	// ������ ������� ���

	if ( pclMsg->bUseItem )
	{
		if( pclMsg->siItemPos >= 0 && pclMsg->siItemPos < MAX_ITEM_PER_PERSON )
		{
			pclCM->CR[id]->pclCI->clCharItem.clItem[pclMsg->siItemPos].Set(&pclMsg->clItem);

			// Ŭ���̾�Ʈ�� ������Ʈ �� ������ ������. 
			cltGameMsgResponse_PersonItemInfo clinfo2(pclMsg->siItemPos, PERSONITEMINFO_OPTION_NONE, 0, 0, &pclMsg->clItem, 0);
			cltMsg clMsg2(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo2), (BYTE*)&clinfo2);
			pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg2);

			pclItemManager->AddUseNum(pclMsg->clUsedItem.siUnique, pclMsg->clUsedItem.siItemNum);

			//���ο� ��� - ������ ��� ��踦 ����.
			//NStatistics_Add("Item", pclMsg->clUsedItem.siUnique, "UseNum", pclMsg->clUsedItem.siItemNum);
			
		}
	}


	// ���� ���� �����̾� ���� �� ��쿡 �ڽŰ� �ֺ������鿡�� ǥ�����ش�. 
	if(		pclMsg->m_stBuf.siBufType == BUF_NOFIRSTATTACK 
		||	pclMsg->m_stBuf.siBufType == BUF_HINAMOCHI 
		||	pclMsg->m_stBuf.siBufType == BUF_EVENT_EGGMOONCAKE 
		||	(pclMsg->m_stBuf.siBufType == BUF_TRANSFORM && pclMsg->m_stBuf.siGetType != BUF_GET_TYPE_NIGHTMARKET)
	)
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACK, true);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}
	
	if( pclMsg->m_stBuf.siBufType == BUF_LOVEPOWER || pclMsg->m_stBuf.siBufType == BUF_FRIENDSHIP )
	{
		if( pclCM->CR[id]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_LOVEPOWER ) 
			&& pclCM->CR[id]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FRIENDSHIP ) )
		{
			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_HEARTEFFECT, true);
			cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
		}
	}
	else if( pclMsg->m_stBuf.siBufType == BUF_FOURLEAF )
	{
		if( pclCM->CR[id]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_FOURLEAF ) )
		{
			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_FOURLEAF, true);
			cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
		}
	}
	else if( pclMsg->m_stBuf.siBufType == BUF_TRANSFORM )
	{
		// ����ó���� ���� ������ ĳ���͸� �����
		if( pclCM->CR[id]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM ) )
		{
			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo( CHANGETYPE_TRANSFORMBUF, false, (SI32)pclMsg->m_stBuf.siPercent );
			cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
		}
	}
	else if( pclMsg->m_stBuf.siBufType == BUF_MENTOR )	//[����] ����, ���� ������ ������ ����ٰ� �޽����� ������.
	{
		SendServerResponseMsg( 0, SRVAL_GIVE_MENTOR_BUF, 0, 0, pclCM->CR[id]->GetCharUnique() );
	}
	// PCK : �������� (Ż�ͺ���)
	else if( pclMsg->m_stBuf.siBufType == BUF_TRANSFORM_HORSE )
	{
		// ����ó���� ���� ������ ĳ���͸� �����
		if( pclCM->CR[id]->pclCI->clBI.clBufInfo.IsHaveBuf( BUF_TRANSFORM_HORSE ) )
		{
			// �̿����� �뺸 
			cltCharChangeStatusInfo	StatusInfo( CHANGETYPE_TRANSFORM_HORSE, false, (SI32)pclMsg->m_stBuf.siPercent );
			cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
			cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
			((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
		}
	}

	// [����] �������� ���� ���� - �����̾��̱� ����
	if(pclMsg->m_stBuf.siBufType == BUF_NOFIRSTATTACK)
	{
		pclCM->CR[id]->pclCI->clBI.clBufInfo.m_bNoFirstAttackPreminum = true;

		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACKPREMIUM, true);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}

}
void cltServer::DoMsg_DBMSG_RESPONSE_DELETEBUF( sPacketHeader* pPacket)
{
	sDBResponse_DeleteBuf* pclMsg = (sDBResponse_DeleteBuf*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID( pclMsg->siPersonID );

	if( pclCM->IsValidID(id) == false )
		return;

	if( pclMsg->siResult < 0 )
		return;

	if( pclMsg->siIndex < 0 && pclMsg->siIndex >= MAX_BUF_COUNT )
		return;

	SI32 BufType = pclCM->CR[id]->pclCI->clBI.clBufInfo.m_stBuf[pclMsg->siIndex].siBufType;
	if( BufType  == BUF_NOFIRSTATTACK || BufType == BUF_HINAMOCHI || BufType == BUF_EVENT_EGGMOONCAKE)
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACK, false);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}
	else if( BufType  == BUF_LOVEPOWER || BufType  == BUF_FRIENDSHIP )
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_HEARTEFFECT, false);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}
	else if( BufType == BUF_FOURLEAF )
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_FOURLEAF, false);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}
	else if( BufType == BUF_HAPPYBEAN )
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(BUF_HAPPYBEAN, false);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}
	else if( BufType == BUF_THANKSGIVING_DAY )
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(BUF_THANKSGIVING_DAY, false);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);  
	}
	else if( BufType == BUF_FARMINGNMININGFOREVER )
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(BUF_FARMINGNMININGFOREVER, false);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}
	else if( BufType == BUF_TRANSFORM )
	{
		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_TRANSFORMBUF, false, 0);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}	
	// PCK : �������� (Ż���� ����)
	else if( BufType == BUF_TRANSFORM_HORSE )
	{	
		// Ż�� �����ÿ� ������ Ż������ ��ü�Ѵ�.
		SI32 siKind = 0;
		cltHorse* pCurHorse = pclClient->pclCM->CR[id]->pclCI->clHorseInfo.GetCurrentShowHorse();
		if( pCurHorse )
			siKind = pCurHorse->siHorseUnique; 

		// �̿����� �뺸 
		cltCharChangeStatusInfo	StatusInfo( CHANGETYPE_TRANSFORM_HORSE, false, siKind );
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);
	}	
	pclCM->CR[id]->pclCI->clBI.clBufInfo.DeleteBuf( pclMsg->siIndex );

	cltGameMsgResponse_DeleteBuf clinfo( pclCM->CR[id]->GetCharUnique(), pclMsg->siIndex );
	cltMsg clMsg(GAMEMSG_RESPONSE_DELETEBUF, sizeof(cltGameMsgResponse_DeleteBuf), (BYTE*)&clinfo );
	pclClient->pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	// [����] �������� ���� ����
	if(pclMsg->m_stBuf.siBufType == BUF_NOFIRSTATTACK)
	{
		pclCM->CR[id]->pclCI->clBI.clBufInfo.m_bNoFirstAttackPreminum = false;

		// �����̾� ���� ���� ����
		cltCharChangeStatusInfo	StatusInfo(CHANGETYPE_NOFIRSTATTACKPREMIUM, false);
		cltGameMsgResponse_StatusInfo clChangeInfo(pclCM->CR[id]->GetCharUnique(), &StatusInfo);
		cltMsg clMsg(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo), (BYTE*)&clChangeInfo);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg, true);

		// ��ȯ�� ���� ���� ����
		cltCharChangeStatusInfo	StatusInfo2(CHANGETYPE_SUMMONNOFIRSTATTACK, false);
		cltGameMsgResponse_StatusInfo clChangeInfo2(pclCM->CR[id]->GetCharUnique(), &StatusInfo2);
		cltMsg clMsg2(GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clChangeInfo2), (BYTE*)&clChangeInfo2);
		((cltCharServer*)pclCM->CR[id])->SendNeighbourMsg(&clMsg2, true);
	}

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 9] // ��������.
		// param1 == ��ȣ.
		// param2 == ����Ÿ��
		// param3 == ���� �ð�(���ӽð�)
		// charparam1 == ���ӽð�
		TCHAR szCharParam1[50] = { '\0', };
		StringCchPrintf(szCharParam1, sizeof(szCharParam1), "Y:%03d M:%02d D:%02d H:%02d", pclTime->clDate.uiYear, pclTime->clDate.uiMonth, pclTime->clDate.uiDay, pclTime->clDate.uiHour);

		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_CHARACTOR, LOGCOMMAND_INDEX_CHARACTER_BUF_DELETE, 
										0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
										pclMsg->siIndex, pclMsg->m_stBuf.siBufType, pclMsg->m_stBuf.siOverTime, 0, 0, szCharParam1, NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETTEACHERPOINT( sPacketHeader* pPacket)
{
	sDBResponse_SetTeacherPoint* pclMsg = (sDBResponse_SetTeacherPoint*)pPacket;

	if( 1 != pclMsg->siResult )
		return;

	// ���� ������ �������̸� ���� Ŭ�� ������Ʈ
	SI32 charID = pclCM->GetIDFromPersonID( pclMsg->siTeacherPersonID );
	if( 0 < charID )
	{
		if( false == pclCM->IsValidID( charID ) )		
			return;	

		// ���� ������Ʈ
	//	pclCM->CR[charID]->pclCI->clBI.clFatherInfo.SetTeacherPoint( pclMsg->siTotalTeacherPoint );
	
		cltGameMsgResponse_SetTeacherPoint clinfo( pclMsg->siTotalTeacherPoint );
		cltMsg clMsg( GAMEMSG_RESPONSE_SETTEACHERPOINT, sizeof(cltGameMsgResponse_SetTeacherPoint), (BYTE*)&clinfo );
		pclCM->CR[charID]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GETITEM_FROM_TEACHERPOINT( sPacketHeader* pPacket)
{
	sDBResponse_GetItem_From_TeacherPoint* pclMsg = (sDBResponse_GetItem_From_TeacherPoint*)pPacket;

}

void cltServer::DoMsg_DBMSG_RESPONSE_RENOUNCEPUPIL( sPacketHeader* pPacket)
{
	sDBResponse_RenouncePupil* pclMsg = (sDBResponse_RenouncePupil*)pPacket;

	SI32 id = pclMsg->siCharID;

	if( 0 >= id )
		return;

	if( 1 != pclMsg->siResult )
	{
		cltGameMsgResponse_RenouncePupil clRenouncePupil( pclMsg->siResult );
		cltMsg clMsg( GAMEMSG_RESPONSE_RENOUNCEPUPIL, sizeof( clRenouncePupil ), (BYTE*)&clRenouncePupil );
		pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);
		
		//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
		//{
		//	// [�α��߰� : Ȳ���� 2007. 10. 9] // ������ ���ڸ� ¥����.
		//	// param1 == ���.
		//	cltServer * pclserver = (cltServer*)pclClient;
		//	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FATHER, LOGCOMMAND_INDEX_FATHER_RENOUNCEPUPIL, 
		//									0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siPupilPersonID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		//									0, 0, 0, 0, 0, NULL, NULL);
		//}
									
		return;
	}

	// ���ڿ��� �������׼� ©�ȴٴ� ������ ������ �����ش�.
	cltLetterMsg_RenouncePupil clletter( (TCHAR*)pclCM->CR[id]->GetName() );
	SendLetterMsg( pclMsg->siPupilPersonID, (cltLetterHeader*)&clletter);

	SI32 pupilID = pclCM->GetIDFromPersonID( pclMsg->siPupilPersonID );
	if( pupilID > 0 )
	{
		if( true == pclCM->IsValidID(pupilID) )
		{
			pclCM->CR[pupilID]->pclCI->clBI.clFatherInfo.Init();

			cltGameMsgResponse_SetFather clinfo(&pclCM->CR[pupilID]->pclCI->clBI.clFatherInfo.clFather, 0, pclCM->CR[pupilID]->pclCI->clBI.clFatherInfo.szAccountID);
			cltMsg clMsg(GAMEMSG_RESPONSE_SETFATHER, sizeof(clinfo), (BYTE*)&clinfo);
			pclCM->CR[pupilID]->SendNetMsg((sPacketHeader*)&clMsg);

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// [�α��߰� : Ȳ���� 2007. 10. 9] // ������ ���ڸ� ����.
			//	// param1	 == ���·���, 
			//	// pszParam1 == ���� �̸�.
			//	SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FATHER, LOGCOMMAND_INDEX_FATHER_RENOUNCEPUPIL, 
			//						0, (cltCharServer*)(pclCM->CR[pupilID]), NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
			//						pclCM->CR[pupilID]->pclCI->clIP.GetLevel(), 0, 0, 0, 0, pclCM->CR[pupilID]->pclCI->clBI.clFatherInfo.szAccountID, NULL);
			//}
		}
	}

	cltGameMsgResponse_RenouncePupil clRenouncePupil( pclMsg->siResult );
	cltMsg clMsg( GAMEMSG_RESPONSE_RENOUNCEPUPIL, sizeof( clRenouncePupil ), (BYTE*)&clRenouncePupil );
	pclCM->CR[id]->SendNetMsg((sPacketHeader*)&clMsg);

	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		// [�α��߰� : Ȳ���� 2007. 10. 9] // ������ ���ڸ� ¥����.
		// param1 == ���.
		cltServer * pclserver = (cltServer*)pclClient;
		pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FATHER, LOGCOMMAND_INDEX_FATHER_RENOUNCEPUPIL, 
										0, (cltCharServer*)pclCM->CR[id], NULL, pclMsg->siPupilPersonID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
										pclMsg->siResult, 0,  0, 0, 0, NULL, NULL);
	}

	sDBRequest_GetChildList clMsg2( id, pclCM->CR[id]->pclCI->GetPersonID() );
	pclGameDBProcess->SendMsg(DBSELECT_LETTER,  (sPacketHeader *)&clMsg2);
}

void cltServer::DoMsg_DBMSG_RESPONSE_OVERTAKETEACHERLEVEL( sPacketHeader* pPacket)
{
	sDBResponse_OverTakeTeacherLevel* pclMsg = (sDBResponse_OverTakeTeacherLevel*)pPacket;

	if( 1 != pclMsg->siResult )
		return;

	SI32 id = pclMsg->siCharID;

	if( 0 >= id )
		return;

	pclCM->CR[id]->pclCI->clBI.clFatherInfo.bOverTake = true;

	// ���� 30��
	cltItem clInputItem1;
	clInputItem1.siUnique = 15900;
	// �Ƿι��� �ý��� ����
	clInputItem1.siItemNum = (SI16)(((cltCharServer*)pclCM->CR[id])->CalcTiredPoint( 1 ));
	clInputItem1.uiDateUseDay = 30;

	if ( clInputItem1.siItemNum )
	{
		SendPostOfficeItemToPerson( pclCM->CR[id]->pclCI->GetPersonID(), &clInputItem1, NULL, 0 );
	}
	if (  siServiceArea == ConstServiceArea_NHNChina )	//�߱��̶�� 
	{
		//û�� ����� �Ƿε��� ����Ǿ��ٸ� �޼����� ������ �ʴ´� .(������ �������� ������ �ʴ´� )
		cltGameMsgResponse_OverTakeTeacherLevel clinfo( &clInputItem1, (TCHAR*)pclCM->CR[id]->GetName(), pclMsg->szTeacherName );
		cltMsg clMsg( GAMEMSG_RESPONSE_OVERTAKETEACHERLEVEL, sizeof(clinfo), (BYTE*)&clinfo);

		if(clInputItem1.siItemNum > 0 || ((cltCharServer*)pclCM->CR[id])->uiTiredState <= 0 )
		{
			pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [�α��߰� : Ȳ���� 2007. 10. 16] // ���ڰ� ���·����� ��������
			// pszParam1 == �����̸�, 
			// pszParam2 == �����̸�.
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FATHER, LOGCOMMAND_INDEX_FATHER_OVERTAKE_TEACHER_LEVEL, 
				0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &clinfo.m_clItem, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, clinfo.m_szPupilName, clinfo.m_szTeaCherName);
		}
	}
	else //�ƴ϶�� ������� 
	{
		cltGameMsgResponse_OverTakeTeacherLevel clinfo( &clInputItem1, (TCHAR*)pclCM->CR[id]->GetName(), pclMsg->szTeacherName );
		cltMsg clMsg( GAMEMSG_RESPONSE_OVERTAKETEACHERLEVEL, sizeof(clinfo), (BYTE*)&clinfo);
		pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);

		if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog))
		{
			// [�α��߰� : Ȳ���� 2007. 10. 16] // ���ڰ� ���·����� ��������
			// pszParam1 == �����̸�, 
			// pszParam2 == �����̸�.
			cltServer * pclserver = (cltServer*)pclClient;
			pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_FATHER, LOGCOMMAND_INDEX_FATHER_OVERTAKE_TEACHER_LEVEL, 
				0, (cltCharServer*)pclCM->CR[id], NULL, 0, 0, 0, &clinfo.m_clItem, 0, 0, 0, 0, 0, 0, 
				0, 0, 0, 0, 0, clinfo.m_szPupilName, clinfo.m_szTeaCherName);
		}
	}	
}

void cltServer::DoMsg_DBMSG_RESPONSE_PVPRESULT( sPacketHeader* pPacket )
{
	sDBResponse_PVPResult* pclMsg = (sDBResponse_PVPResult*)pPacket;

	if( 1 != pclMsg->siResult )
		return;

	// �̱� ��� ó��
	if( pclCM->IsValidID( pclMsg->siWinCharID ) )		
	{
		pclCM->CR[pclMsg->siWinCharID]->pclCI->clBI.siPVPWinNumber++;
		MStrCpy( pclCM->CR[pclMsg->siWinCharID]->pclCI->clBI.szPVPWinHighLevelUserName, pclMsg->szPVPWinHighLevelUserName, MAX_PLAYER_NAME );

		cltGameMsgResponse_PVPResult clPVPResult( true, pclMsg->szLoseUserName,
												pclCM->CR[pclMsg->siWinCharID]->pclCI->clBI.siPVPWinNumber,
												pclCM->CR[pclMsg->siWinCharID]->pclCI->clBI.siPVPLoseNumber,
												pclCM->CR[pclMsg->siWinCharID]->pclCI->clBI.szPVPWinHighLevelUserName );
		cltMsg clMsg( GAMEMSG_RESPONSE_PVPRESULT, sizeof( clPVPResult ), (BYTE*)&clPVPResult );
		pclCM->CR[pclMsg->siWinCharID]->SendNetMsg((sPacketHeader*)&clMsg);
	}

	// �� ��� ó��
	if( pclCM->IsValidID( pclMsg->siLoseCharID ) )	
	{
		pclCM->CR[pclMsg->siLoseCharID]->pclCI->clBI.siPVPLoseNumber++;

		//[����] pk�� �й��ϸ� �ڽ��� ���ߴٴ� ������ �����ְ� ����. => 2008-5-9
		cltGameMsgResponse_PVPResult clPVPResult( false, pclMsg->szWinUserName,
												pclCM->CR[pclMsg->siLoseCharID]->pclCI->clBI.siPVPWinNumber,
												pclCM->CR[pclMsg->siLoseCharID]->pclCI->clBI.siPVPLoseNumber,
												pclCM->CR[pclMsg->siLoseCharID]->pclCI->clBI.szPVPWinHighLevelUserName );
		cltMsg clMsg( GAMEMSG_RESPONSE_PVPRESULT, sizeof( clPVPResult ), (BYTE*)&clPVPResult );
		pclCM->CR[pclMsg->siLoseCharID]->SendNetMsg((sPacketHeader*)&clMsg);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_ATTEND( sPacketHeader* pPacket )
{
	// [tys]attend - �⼮üũ.
	sDBResponse_Attend* pclinfo = (sDBResponse_Attend*)pPacket;

	SI32 id = pclinfo->usCharID ;
	if ( pclCM->IsValidID(id) == false )	return;

	SI32 personid = pclinfo->siPersonID ;
	if ( pclCM->CR[id]->pclCI->GetPersonID() != personid ) return ;

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[id];
	if ( pclchar == NULL ) return ;

	if ( pclinfo->siResult != 1 )			return;

	switch ( pclinfo->siType ) 
	{
		// �⼮ üũ�̺�Ʈ
		case ATTEND_EVENTTYPE_ATTENDANCE :
			{
				pclchar->pclCI->clBI.bAttendToday[ATTEND_EVENTTYPE_ATTENDANCE] = true;
				
				cltGameMsgResponse_Attend clinfo( pclinfo->siAttendanceDay, 0,  pclinfo->siType );
				cltMsg clMsg( GAMEMSG_RESPONSE_ATTEND, sizeof(clinfo), (BYTE*)&clinfo );
				pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
			}
			break;
		// PC�� �����̾� ��ȭ �̺�Ʈ ����
		case ATTEND_EVENTTYPE_PCROOM:
			{
				bool		bMakeResult			= false;	// ������ �������
				bool		bReferenceCheck		= false;	// ������ ������ ã�� ���

				cltItem		clRewardItem;		// ������ ������
				cltItem		clRewardItem2;		// �߱� PC��� 2��° ���� ������ 
				SI32		siRewardItemUnique	= ITEMUNIQUE( 2756 );	// ����� ����Ŭ�ι�(����Ŭ�ι�2)
				SI32		siRewardItemUnique2	= ITEMUNIQUE( 13351 );	// �߱�PC�� ����
				SI32		siReference			= pclClient->pclItemManager->FindItemRefFromUnique( siRewardItemUnique );

				SI16		siUsedate			= 0;
				SI16		siRareswitch		= 0;

				// 0�� PC�� ������ �⼮�� �ȵ��ִ� ���� �� ������ ������ ���ѻ��� �׷��� ������ 0�� �ƴϸ� ������ ����
				if ( pclinfo->siAttendanceDay != 0 )
				{
					return;
				}

				// ��û�� ����� ������ Ȥ�� �ٽ��ѹ� �����ؼ� PC���� �ƴ϶�� ����
				if ( pclchar->pclCI->clBI.bPCRoom == false )
				{
					return;
				}

				if ( siReference > 0 )
				{
					siUsedate			= pclClient->pclItemManager->pclItemInfo[ siReference ]->clItem.uiDateUseDay;

					bReferenceCheck		= true;
					bMakeResult			= pclClient->pclItemManager->MakeRandItemUnique( siRewardItemUnique, &clRewardItem, 0, 0, &siRareswitch,0,0,0,0,0,siUsedate );
				}

				// �߱��϶��� �������� �ϳ� �� �����Ѵ�
				if ( IsWhereServiceArea(ConstServiceArea_NHNChina) )
				{
					if ( (bMakeResult == true) && (bReferenceCheck == true) )
					{
						// 2��° �������� ����� �������� �˻��ؾ� �ϹǷ� �ٽ� false�� �����
						bReferenceCheck		= false;
						bMakeResult			= false;

						siReference			= pclItemManager->FindItemRefFromUnique( siRewardItemUnique2 );
						if ( 0 < siReference )
						{
							siUsedate		= pclClient->pclItemManager->pclItemInfo[ siReference ]->clItem.uiDateUseDay;
							bReferenceCheck	= true;
							bMakeResult		= pclClient->pclItemManager->MakeRandItemUnique( siRewardItemUnique2, &clRewardItem2, 0, 0, &siRareswitch,0,0,0,0,0,siUsedate );
						}
					}
				}

				if ( (bMakeResult == true) && (bReferenceCheck == true) )
				{
					cltSystemRewardInfo		clInfo;			// ������ ���� ���� ����
					TCHAR*					pText			= GetTxtFromMgr( 7979 );	// �����ϴ� ����(PC�� �����̾�)

					SI32					siPersonID		= pclchar->pclCI->GetPersonID();
					SI32					siCharID		= pclchar->GetID();

					// ���� ��¥
					clInfo.m_sGiveTime		= sTime;

					// ���� ���κ��� 1�ϵ��ȸ� ������ �� �ִ�.
					TimeAddDate(&clInfo.m_sGiveTime, 1, &clInfo.m_sExpireTime);

					// ������ ������
					clInfo.m_clItem.Set( &clRewardItem );

					// ���� ����
					if ( pText )
					{
						StringCchCopy( clInfo.m_szReason, 256, pText );
					}

					// �������� ���� �϶�� ��û
					sDBRequest_GiveSystemReward clMsg( GIVEREWARD_ACCOUNT, siPersonID, siCharID, &clInfo );
					pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

					// �߱��϶��� �������� �ϳ� �� �����Ѵ�
					if ( IsWhereServiceArea(ConstServiceArea_NHNChina) )
					{
						clInfo.m_clItem.Set( &clRewardItem2 );

						// �������� ���� �϶�� ��û
						sDBRequest_GiveSystemReward clMsg( GIVEREWARD_ACCOUNT, siPersonID, siCharID, &clInfo );
						pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
					}

					NDate kAttendDay;
					kAttendDay.SetDate( sTime.wYear, sTime.wMonth, sTime.wDay );
					
					// �� ��¥�� �⼮�� �϶�� �޽���
					sDBRequest_SetAttend clDBSetAttend( siPersonID, siCharID, &kAttendDay, ATTEND_EVENTTYPE_PCROOM );
					pclGameDBProcess->SendMsg((sPacketHeader *)&clDBSetAttend);

					// �������� ���޵Ǿ��ٴ� �޽����� �����
					SendServerResponseMsg( 0, SRVAL_REWARDITEM_FOR_PCROOM, 0, 0, siCharID );

					pclchar->pclCI->clBI.bAttendToday[ ATTEND_EVENTTYPE_PCROOM ] = true;
				}

			}
			break;

		case ATTEND_EVENTTYPE_SEASON2:
			{
				// �̰��� ������ ���� �� �ִ��� ���������� �˻��Ѵ�
				SI32 siAttendanceDay = pclinfo->siAttendanceDay;
				SI32 siModulationDay = siAttendanceDay % Const_Attendance_Modulation_Day;

				if ( (0 > siAttendanceDay) || (0 > siModulationDay) )
				{
					return;
				}

				// ���� ������ ���������� ��ĥ�� �ʿ����� �˻�
				SI32 siNextRewardDays = 0;
				for ( SI32 siNext=siModulationDay; siNext<MAX_ATTENDANCE_COUNT; siNext++ )
				{
					if ( m_pclAttendanceReward[siNext] != NULL )
					{
						if ( m_pclAttendanceReward[siNext]->m_siItemUnique > 0 )
						{
							break;
						}
					}
					siNextRewardDays++;
				}

				if ( siNext == MAX_ATTENDANCE_COUNT )
				{
					siNextRewardDays = 0;
				}


				SI32 siRewardCount = 0;
				SI32 siRewardIndex = 0;
				// ���� �⼮�ϼ����� ����������� ��� �������ִ��� �˻�
				for ( SI32 siCount=1; siCount<=siAttendanceDay; siCount++ )
				{
					siRewardIndex = (siCount % Const_Attendance_Modulation_Day);
					if ( m_pclAttendanceReward[siRewardIndex] != NULL )
					{
						if ( m_pclAttendanceReward[siRewardIndex]->m_siItemUnique > 0 )
						{
							siRewardCount++;
						}
					}
				}
				
				// ���� ������ ���� �ʿ䰡 ���ٸ� �⼮ ������ ���� �� �ֵ��� ���ش�
				if ( pclinfo->siRewardDay >= siRewardCount )
				{
					 pclchar->pclCI->clBI.bIsCanAttend = true;
				}
				else
				{
					pclchar->pclCI->clBI.bIsCanAttend = false;
				}

				// ������ ���� �� �ִ��� ���θ� �Ǻ�
				if ( pclinfo->siRewardDay < siRewardCount )
				{
					pclchar->pclCI->clBI.bIsCanReward = true;
				}
				else
				{
					pclchar->pclCI->clBI.bIsCanReward = false;
				}

				pclchar->pclCI->clBI.siAttendanceDays = siAttendanceDay;

			
				// Ŭ���̾�Ʈ���� �⼮�� ���� ������ �����ش� ��Ŷ �̸��� �������
				cltGameMsgResponse_DemandAttend clInfo( (SI16)siAttendanceDay, (SI16)siNextRewardDays, ATTEND_EVENTTYPE_SEASON2, false );
				cltMsg clMsg( GAMEMSG_RESPONSE_DEMANDATTEND, sizeof(clInfo), (BYTE*)&clInfo);
				pclchar->SendNetMsg( (sPacketHeader*)&clMsg );

				// �α� ���
				if( pclClient->IsWhereServiceArea(ConstSwitch_NewLog) ) 
				{
					// param1 : �⼮�ϼ�
					// param2 : ���� ���� �ϼ�
					// param3 : �⼮ Ÿ��
					SendLog_WriteLogToDB( LOGCOMMAND_TYPE_ATTEND, LOGCOMMAND_INDEX_ATTEND_CHANGE, 0, pclchar, NULL,
										0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
										siAttendanceDay, siNextRewardDays, ATTEND_EVENTTYPE_SEASON2, 0, 0, NULL, NULL );
				}
			}
			break;

		default:
			return;
	}

}

// [����] �⼮üũ : �⼮üũ�ϰ������� �����
void cltServer::DoMsg_DBMSG_RESPONSE_SETATTEND( sPacketHeader* pPacket )
{
	sDBResponse_SetAttend* pclinfo = (sDBResponse_SetAttend*)pPacket;
	if ( pclinfo == NULL )
	{
		return;
	}

	SI32 siCharID = pclinfo->uiCharID ;
	if ( pclCM->IsValidID(siCharID) == false )
	{
		return;
	}

	SI32 siPersonID = pclinfo->siPersonID ;
	if ( pclCM->CR[siCharID]->pclCI->GetPersonID() != siPersonID )
	{
		//return;
	}

	cltServer *pclServer = (cltServer*)pclClient;
	if ( pclServer == NULL )
	{
		return;
	}

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[siCharID];
	if ( pclchar == NULL )
	{
		return;
	}

	if ( pclinfo->siResult != 1 )
	{
		// �α� ���
/*		if( pclClient->IsWhereServiceArea(ConstSwitch_NewLog) ) 
		{
			switch ( pclinfo->siType ) 
			{
				case ATTEND_EVENTTYPE_ATTENDANCE:
				{
					pclServer->SendLog_WriteLogToDB( LOGCOMMAND_TYPE_ATTEND, LOGCOMMAND_INDEX_ATTEND_FAIL, 0, pclchar, NULL,
						0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL );
				}
				break;

				case ATTEND_EVENTTYPE_SEASON2:
				{
					pclServer->SendLog_WriteLogToDB( LOGCOMMAND_TYPE_ATTEND, LOGCOMMAND_INDEX_ATTEND_REWARD_FAIL, 0, pclchar, NULL,
						0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL, NULL );
				}
				break;
			}
		}*/
		return;
	}

	switch ( pclinfo->siType ) 
	{
		// �⼮ üũ�̺�Ʈ
		case ATTEND_EVENTTYPE_ATTENDANCE:
			{
				// �̺�Ʈ �Ⱓ���� �󸶸�ŭ �⼮�ϰ� �󸶸�ŭ ������ �޾Ҵ��� �޾ƿ���
				sDBRequest_Attend clMsg( siPersonID, siCharID,	&clAttendEventStart, &clAttendEventEnd, ATTEND_EVENTTYPE_SEASON2 );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}
			break;

		// �Ѱ��� PC�� �⼮
		case ATTEND_EVENTTYPE_PCROOM:
			{
				
			}
			break;

		// ����2 �⼮ �̺�Ʈ
		case ATTEND_EVENTTYPE_SEASON2:
			{
				SI32	siAttendanceDays = pclchar->pclCI->clBI.siAttendanceDays;
				cltItem clRewardItem;
				
				if ( GetItemByAttendanceDay( siAttendanceDays, &clRewardItem ) )
				{
					cltSystemRewardInfo clInfo;

					clInfo.m_sGiveTime = sTime;
                    // ���޹��� �� 30�� ���� ������ �����ϴ�.
					TimeAddDate(&clInfo.m_sGiveTime, 30, &clInfo.m_sExpireTime);

//					NDate ExpireDate;
//					ExpireDate.SetDate( sTime.wYear, sTime.wMonth, sTime.wDay );
//					ExpireDate.AddMonths( 1 );
//
//					_SYSTEMTIME sExpireTime;
//					ZeroMemory( &sExpireTime, sizeof(_SYSTEMTIME) );
//					sExpireTime.wYear		= ExpireDate.m_uiYear;
//					sExpireTime.wMonth		= ExpireDate.m_uiMonth;
//					sExpireTime.wDay		= ExpireDate.m_uiDay;
//
//					clInfo.m_sExpireTime	= sExpireTime;

					clInfo.m_clItem.Set( &clRewardItem );
					TCHAR* pText = GetTxtFromMgr( 10175 );
					StringCchPrintf( clInfo.m_szReason, 256, pText, siAttendanceDays );

					sDBRequest_GiveSystemReward clMsg( GIVEREWARD_ACCOUNT, pclchar->pclCI->GetPersonID(), pclchar->GetID(), &clInfo );
					SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clMsg );

					pclchar->pclCI->clBI.bAttendToday[ATTEND_EVENTTYPE_SEASON2] = true;

					// �⼮�� �����ϵ��� ���ش�
					pclchar->pclCI->clBI.bIsCanAttend = true;

					SendServerResponseMsg( 0, SRVAL_SUCCESS_SEASON2_ATTEND_REWARDITEM, 0, 0, pclchar->GetID() );

					// �α� ���
					if( pclClient->IsWhereServiceArea(ConstSwitch_NewLog) ) 
					{
						// para1 = �⼮�ϼ�
						pclServer->SendLog_WriteLogToDB( LOGCOMMAND_TYPE_ATTEND, LOGCOMMAND_INDEX_ATTEND_REWARD_SUCCESS, 0, pclchar, NULL,
							0, 0, 0, &clRewardItem, 0, 0, 0, 0, 0, 0, siAttendanceDays, 0, 0, 0, 0, NULL, NULL );
					}
				}
				else
				{
					SendServerResponseMsg( 0, SRVAL_FAIL_SEASON2_ATTEND_REWARDITEM, 0, 0, pclchar->GetID() );
					// �α� ���
					if( pclClient->IsWhereServiceArea(ConstSwitch_NewLog) ) 
					{
						// para1 = �⼮�ϼ�
						pclServer->SendLog_WriteLogToDB( LOGCOMMAND_TYPE_ATTEND, LOGCOMMAND_INDEX_ATTEND_REWARD_FAIL, 0, pclchar, NULL,
							0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, siAttendanceDays, 0, 0, 0, 0, NULL, NULL );
					}
				}
			}
			break;
		default:
			return;
	}

	pclchar->pclCI->clBI.bAttendToday[pclinfo->siType] = true;
}

//KHY - 1220 - ���Žý���.
void cltServer::DoMsg_DBMSG_RESPONSE_TRANSFORM_ADDCHAR( sPacketHeader* pPacket )
{
	sDBResponse_Transform_AddChar* pclinfo = (sDBResponse_Transform_AddChar*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclinfo->siPersonID);

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( NULL == pclChar)
		return;
	if ( pclinfo->siPersonID != pclChar->pclCI->GetPersonID())
		return ;

	if(pclinfo->siResult == 1)
	{
		if(IsWhereServiceArea(ConstSwitch_NewLog))
		{
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_SPIRIT_CHARGE, 
				0, pclChar, 
				NULL, 0, 0, 0, &pclinfo->clItem, pclinfo->clLeftItem.GetItemNum(), 0, 0, 0, 0, 0, 
				pclinfo->siItemPos, 0, pclinfo->siAddTime, 0, 0, 
				NULL, NULL);
		}

		if(pclChar->clTransFormInfo.SetTransFormChar(pclinfo->siCharKind) == true)
		{
			// Ŭ���̾�Ʈ�� �˸���.
			cltGameMsgResponse_SetTransForm cltransform( &pclChar->clTransFormInfo );
			cltMsg clMsgTransform(GAMEMSG_RESPONSE_TRANSFORM, sizeof(cltransform), (BYTE*)&cltransform);
			pclChar->SendNetMsg((sPacketHeader*)&clMsgTransform);
		}

		cltTimeMeterRate* pclTimeMeterRate = pclChar->pclCI->clBI.clTimeMeterRateInfo.GetTimeMeterRate(pclinfo->siTimeMeterRateType);
		if( pclTimeMeterRate )
		{
			pclTimeMeterRate->AddLeftTime(pclinfo->siAddTime);

			// Ŭ���̾�Ʈ�� ������ �����Ѵ�.
			cltGameResponse_TimeMeterRate_Update	clTimeMeterRate( pclTimeMeterRate );
			cltMsg clMsg1( GAMEMSG_RESPONSE_TIMEMETERRATE_UPDATE, sizeof(clTimeMeterRate), (BYTE*)&clTimeMeterRate );
			pclChar->SendNetMsg( (sPacketHeader*)&clMsg1 );
		}

		//--------------------------
		// �κ��� ������Ʈ �Ѵ�. 
		//--------------------------
		if( pclinfo->siItemPos >= 0 && pclinfo->siItemPos < MAX_ITEM_PER_PERSON )
		{
			pclChar->pclCI->clCharItem.clItem[pclinfo->siItemPos].Set(&pclinfo->clLeftItem);

			cltGameMsgResponse_PersonItemInfo clinfo(pclinfo->siItemPos, PERSONITEMINFO_OPTION_NONE, pclinfo->clLeftItem.siItemNum, 0, &pclinfo->clLeftItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclChar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_TIMEMETERRATE_ADDTIME( sPacketHeader* pPacket )
{
	sDBResponse_TimeMeterRate_AddTime* pclinfo = (sDBResponse_TimeMeterRate_AddTime*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclinfo->siPersonID);

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( pclinfo->siPersonID != pclChar->pclCI->GetPersonID())
		return ;

	if(pclinfo->siResult == 1)
	{
		if(IsWhereServiceArea(ConstSwitch_NewLog))
		{
			SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONITEM, LOGCOMMAND_INDEX_PERSONITEM_SPIRIT_CHARGE, 
				0, pclChar, 
				NULL, 0, 0, 0, &pclinfo->clItem, pclinfo->clLeftItem.siItemNum, 0, 0, 0, 0, 0, 
				pclinfo->siItemPos, pclinfo->siLeftTime, pclinfo->siAddTime, 0, 0, 
				NULL, NULL);
		}
		cltTimeMeterRate* pclTimeMeterRate = pclChar->pclCI->clBI.clTimeMeterRateInfo.GetTimeMeterRate(pclinfo->siTimeMeterRateType);
		if( pclTimeMeterRate )
		{
			pclTimeMeterRate->AddLeftTime(pclinfo->siAddTime);

			// Ŭ���̾�Ʈ�� ������ �����Ѵ�.
			cltGameResponse_TimeMeterRate_Update	clTimeMeterRate( pclTimeMeterRate );
			cltMsg clMsg( GAMEMSG_RESPONSE_TIMEMETERRATE_UPDATE, sizeof(clTimeMeterRate), (BYTE*)&clTimeMeterRate );
			pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
		//--------------------------
		// �κ��� ������Ʈ �Ѵ�. 
		//--------------------------
		if( pclinfo->siItemPos >= 0 && pclinfo->siItemPos < MAX_ITEM_PER_PERSON )
		{
			pclChar->pclCI->clCharItem.clItem[pclinfo->siItemPos].Set(&pclinfo->clLeftItem);

			cltGameMsgResponse_PersonItemInfo clinfo(pclinfo->siItemPos, PERSONITEMINFO_OPTION_NONE, pclinfo->clLeftItem.siItemNum, 0, &pclinfo->clLeftItem, 0);
			cltMsg clMsg(GAMEMSG_RESPONSE_PERSONITEMINFO, sizeof(clinfo), (BYTE*)&clinfo);
			pclChar->SendNetMsg((sPacketHeader*)&clMsg);
		}
	}

}

void cltServer::DoMsg_DBMSG_RESPONSE_TIMEMETERRATE_USETIME( sPacketHeader* pPacket )
{
	sDBResponse_TimeMeterRate_UseTime* pclinfo = (sDBResponse_TimeMeterRate_UseTime*)pPacket;

	SI32 id = pclCM->GetIDFromPersonID(pclinfo->siPersonID);

	if( pclCM->IsValidID(id) == false )
		return;

	cltCharServer *pclChar = (cltCharServer *)pclCM->CR[id];
	if ( pclinfo->siPersonID != pclChar->pclCI->GetPersonID())
		return ;

	cltItem *pclItemList = pclChar->pclCI->clCharItem.GetEquipSet(EQUIPSET_PERSON);
	cltPSkillInfo *pclskillinfo = &pclChar->pclCI->clCharSkill;


	if(pclinfo->siResult == 1)
	{
		if( pclinfo->bUpdate )
		{
			cltTimeMeterRate* pclTimeMeterRate = pclChar->pclCI->clBI.clTimeMeterRateInfo.GetTimeMeterRate(pclinfo->siTimeMeterRateType);
			if( pclTimeMeterRate )
			{
				pclTimeMeterRate->siLeftTimeSec = pclinfo->siLeftTime;

				// Ŭ���̾�Ʈ�� ������ �����Ѵ�.
				cltGameResponse_TimeMeterRate_Update	clTimeMeterRate( pclTimeMeterRate );
				cltMsg clMsg( GAMEMSG_RESPONSE_TIMEMETERRATE_UPDATE, sizeof(clTimeMeterRate), (BYTE*)&clTimeMeterRate );
				pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
			}
		}
	}
	// DBó������ ������ ����, DB������ ������Ʈ �Ѵ�.
	else
	{
		// ������ ���� ���� ����
		pclChar->pclCI->clBI.clTimeMeterRateInfo.UseEnd(pclinfo->siTimeMeterRateType);

      		 // ������ �ý��� ���� ���ῡ ���� ������ ����		
		switch(pclinfo->siTimeMeterRateType)
		{
			case TIMEMETERRATE_TRANSFORM:
				// ���Žý��� ���� ����
				pclChar->CharTransform( 0 ,pclChar->ActiveWeapon,pclItemList ,pclskillinfo);
				break;
			//KHY - 1111 - ������� �߰�
			case TIMEMETERRATE_MOTORCYCLE:
				// ������� ���� ������ ������ �Ѵ�.
				pclChar->CharTransform( 0 ,pclChar->ActiveWeapon,pclItemList ,pclskillinfo);
				break;
		}

		cltTimeMeterRate* pclTimeMeterRate = pclChar->pclCI->clBI.clTimeMeterRateInfo.GetTimeMeterRate(pclinfo->siTimeMeterRateType);
		if( pclTimeMeterRate )
		{
			pclTimeMeterRate->siLeftTimeSec = pclinfo->siLeftTime;

			// Ŭ���̾�Ʈ�� ������ �����Ѵ�.
			cltGameResponse_TimeMeterRate_Update	clTimeMeterRate( pclTimeMeterRate );
			cltMsg clMsg( GAMEMSG_RESPONSE_TIMEMETERRATE_UPDATE, sizeof(clTimeMeterRate), (BYTE*)&clTimeMeterRate );
			pclChar->SendNetMsg( (sPacketHeader*)&clMsg );
		}
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_GET_HOMEPAGE_URL(sPacketHeader* pPacket)
{
	sDBResponse_Get_HomePageURL* pclinfo = (sDBResponse_Get_HomePageURL*)pPacket;

	SI32 id = pclinfo->siCharID;
	
	if ( pclCM->IsValidID(id) == false ) return ;

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if ( personid <= 0 ) return ;

	SI32 targetid = pclCM->GetIDFromPersonID( pclinfo->siPersonID );
	if (pclCM->IsValidID(targetid) == false) return;

	// DB�� Ȩ������ URL������ ��û�Ѵ�.
	cltGameMsgResponse_GetHomepage_URL GetHomepage_URL( pclinfo->szText );
	cltMsg clMsg( GAMEMSG_RESPONSE_GET_HOMEPAGE_URL, sizeof(GetHomepage_URL), (BYTE*)&GetHomepage_URL );
	SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_SET_HOMEPAGE_URL(sPacketHeader* pPacket)
{
	sDBResponse_Set_HomePageURL* pclinfo = (sDBResponse_Set_HomePageURL*)pPacket;

	SI32 id = pclinfo->siCharID;

	if ( pclCM->IsValidID(id) == false) return;

	if(id && pclCM->CR[id]->CheckLastMsgClock(1000) == true)
	{
		return ;
	}

	SI32 personid = pclCM->CR[id]->pclCI->GetPersonID();
	if ( personid <= 0 ) return;


	// DB�� �������� URL������ �����Ѵ�.
	//cltGameMsgResponse_SetHomepage_URL GetHomepage_URL( pclinfo->szText );
	//cltMsg clMsg( GAMEMSG_RESPONSE_SET_HOMEPAGE_URL, sizeof(GetHomepage_URL), (BYTE*)&GetHomepage_URL );
	//SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
}

void cltServer::DoMsg_DBMSG_RESPONSE_SETBONUSINFO(sPacketHeader* pPacket)
{
	// PCK : �α׸� ����� ���ؼ� �ۼ�
	sDBResponse_SetBonusInfo*	pclMsg = (sDBResponse_SetBonusInfo*)pPacket;

	// PCK : PersonID�� �ٰŷ� CharID�� ã��.(PersonID��������!!)
	SI32 CharID = pclCM->GetIDFromPersonID(pclMsg->siPersonID);

	if(FALSE == pclCM->IsValidID(CharID)) {		return;		}

	cltCharServer* pclchar = pclCM->GetCharServer(CharID);

	TCHAR szBA[50];
	ZeroMemory(szBA, sizeof(szBA));
	StringCchPrintf(szBA, sizeof(szBA), TEXT("Str:%d, Dex:%d, Vit:%d, Mag:%d, Hnd:%d, Luk:%d, Wis:%d"),	
		pclMsg->cIBA.GetStr(), pclMsg->cIBA.GetDex(), pclMsg->cIBA.GetMag(), 
		pclMsg->cIBA.GetHnd(), pclMsg->cIBA.GetLuk(), pclMsg->cIBA.GetWis() );

	// param1 == ĳ���Ͱ� ��� ���ʽ� ��ġ.
	// pszcharparam1 == ĳ������ ����
	cltServer* pclserver = (cltServer*)pclClient;
	pclserver->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_INPORTANTPARAMEATA, LOGCOMMAND_INDEX_INPORTANTPARAMEATA_SETBONUSINFO,
									0, pclchar, NULL, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 0, 
									pclMsg->siBonus, 0, 0, 0, 0,  szBA, NULL);
}
//�߷�Ÿ�� ���� ������ �޴´�,.
void cltServer::DoMsg_DBMSG_RESPONSE_VALENTINEWAR_INFO_GET(sPacketHeader* pPacket)
{
	sDBResponse_Valantine_Info_Get*	pclMsg = (sDBResponse_Valantine_Info_Get*)pPacket;
	SI32 id = pclCM->GetIDFromPersonID( pclMsg->m_siPersonID ) ;
	
	if(FALSE == pclCM->IsValidID(id)) {		return;		}
	cltCharServer* pclchar = pclCM->GetCharServer(id);

	SYSTEMTIME stLastAnswerTime;

	stLastAnswerTime = pclMsg->m_clValantineWarInfo.GetAnswerTime()	;	// ���κ��� ���������� ����ؼ� ����� �ð��� �޴´�.
	if( sTime.wDay != stLastAnswerTime.wDay )// ��¥�� �ٸ��ٸ�
	{
		// Ŭ���̾�Ʈ�� Ŀ���̳İ� ���� â�� ����.
		cltGameResponse_ValentineWar_OpenDlg clDoenDlg(true);
		cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINE_OPENDLG, sizeof(clDoenDlg), (BYTE*)&clDoenDlg );
		pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
	}
	else // ��¥�� ����
	{
		// ������ ������ ���� ��
		pclchar->pclValentineWar->Set(&pclMsg->m_clValantineWarInfo)	;
		cltGameResponse_ValentineWar_Info_get clValenTinewar_Info_Get( pclchar->pclValentineWar );
		cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINE_INFO_GET, sizeof(clValenTinewar_Info_Get), (BYTE*)&clValenTinewar_Info_Get );
		pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
	}
}
//�߷�Ÿ�� ���� ���� ������ �ߴ�.
void cltServer::DoMsg_DBMSG_RESPONSE_VALENTINEWAR_REQUEST(sPacketHeader* pPacket)
{
	sDBResponse_Valantine_Request*	pclMsg = (sDBResponse_Valantine_Request*)pPacket;
	SI32 siID = pclCM->GetIDFromPersonID(pclMsg->m_siPersonID) ;
	if( FALSE == pclCM->IsValidID(siID ) )	return	;
	cltCharServer* pclchar = pclCM->GetCharServer( siID)	;
	pclchar->pclValentineWar->Set( &pclMsg->clValentineWarInfo)	;
	cltGameMsgResponse_ValentineWar_Request clvalentinewar_Request( pclchar->GetCharUnique(),pclMsg->clValentineWarInfo );
	cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINE_REQUEST, sizeof(clvalentinewar_Request), (BYTE*)&clvalentinewar_Request );
	pclchar->SendNeighbourMsg( &clMsg,true );
}
//�߷�Ÿ�� ����������� �޾Ҵ�.
void cltServer::DoMsg_DBMSG_RESPONSE_VALENTINEWAR_GET_PRIZE(sPacketHeader* pPacket)
{
	sDBResponse_Valantine_Get_Prize*	pclMsg = (sDBResponse_Valantine_Get_Prize*)pPacket;
	SI32 siID = pclCM->GetIDFromPersonID(pclMsg->m_siPersonID) ;
	if( FALSE == pclCM->IsValidID(siID ) )	return	;
	cltCharServer* pclchar = pclCM->GetCharServer( siID)	;
	pclchar->pclValentineWar->SetPrize( pclMsg->bGetPrize );

	cltGameResponse_ValentineWar_PrizeGet clvalentinewar_PrizeGet( pclchar->GetCharUnique(),pclMsg->bGetPrize );
	cltMsg clMsg( GAMEMSG_RESPONSE_VALENTINE_PRIZEGET, sizeof(clvalentinewar_PrizeGet), (BYTE*)&clvalentinewar_PrizeGet );
	pclchar->SendNetMsg( (sPacketHeader*)&clMsg );
}


void cltServer::DoMsg_DBMSG_RESPONSE_CELLULAR_PHONE_AUTH(sPacketHeader* pPacket)
{
	sDBResponse_Get_Cellular_Phone_Auth* pclMsg = (sDBResponse_Get_Cellular_Phone_Auth*)pPacket;
	cltCharServer* pclchar = pclCM->GetCharServer( pclMsg->m_siCharID );
	if( NULL == pclchar ) return;
	
	pclchar->bCellularPhoneAuth = pclMsg->m_bIsCellularPhoneAuth;

	if( 0 < pclMsg->m_siRecommendPersonID )
	{
		// ������ ��Ͽ� �ִ��� üũ
		cltSimplePerson* pSimplePersonInfo = NULL;
		for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
		{
			pSimplePersonInfo = &pclchar->pclFriendInfo->clFriend[i];

			if ( pSimplePersonInfo != NULL && pSimplePersonInfo->GetPersonID() > 0 )
			{
				if ( pSimplePersonInfo->GetName() != NULL && _tcscmp( pSimplePersonInfo->GetName(), pclMsg->m_szRecommendPersonName ) == 0 )
				{
					// ģ�� ��Ͽ� �����Ƿ� �߰��� �ʿ� ����.
					return;
				}
			}
		}

		if( pclMsg->m_bFirstCharCreate )
		{
			sDBRequest_FriendAddList clMsg(	0, pclMsg->m_siRecommendPersonID, 0, pclMsg->m_szRecommendPersonName, 
											0, pclchar->pclCI->GetPersonID(), pclMsg->m_siCharID, (TCHAR*)pclchar->GetName() );

			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);

		}
	}
}

void cltServer::DoMsg_DBMSG_DBMSG_RESPONSE_SET_MARRY( sPacketHeader* pPacket )
{
	sDBResponse_Set_Marry* pclMsg = (sDBResponse_Set_Marry*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	// DB���� ���� �ߴٸ�
	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}
	
	// ��ȥ �����
	SI32			siWeddingDate	= pclMsg->m_siWeddingDateVary;
	SI16			siLoveLevle		= pclMsg->m_siLoveLevel;

	// �켱 ûȥ�� �� ���
	SI32			siSourcePID		= pclMsg->m_siSourcePersonID;
	SI32			siSourceCharID	= pclCM->GetIDFromPersonID( siSourcePID );
	cltCharServer*	pclSourceChar	= pclCM->GetCharServer( siSourceCharID );

	// ûȥ ���� ���
	SI32			siDestPID		= pclMsg->m_siDestPersonID;
	SI32			siDestCharID	= pclCM->GetIDFromPersonID( siDestPID );
	cltCharServer*	pclDestChar		= pclCM->GetCharServer( siDestCharID );

	// ����/���� �ʻ� �ִ��� �˻�
	SI16			siConnectMode	= MARRIAGE_MATE_CONNECT_MODE_NONE;
	
	if ( (pclSourceChar) && (pclDestChar) )
	{
		siConnectMode = MARRIAGE_MATE_CONNECT_MODE_CONNECT;

		if ( pclSourceChar->GetMapIndex() == pclDestChar->GetMapIndex() )	
		{
			siConnectMode = MARRIAGE_MATE_CONNECT_MODE_SAMEMAP;
		}
	}

	// ûȥ�� ���
	if ( pclSourceChar )
	{
		// ��ȥ ���� ���� ����
		if ( pclSourceChar->pclMarriage )
		{
			pclSourceChar->pclMarriage->Set( siDestPID, pclMsg->m_szDestName, siWeddingDate, 0, FALSE, siLoveLevle, 0, 0, siConnectMode );
		}

		// ���� ������ ����
		if ( (PERSONITEM_INV0 <= pclMsg->m_siSourceItemPos) && ((PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON) > pclMsg->m_siSourceItemPos) )
		{
			pclSourceChar->pclCI->clCharItem.clItem[pclMsg->m_siSourceItemPos].Set( &pclMsg->m_clSourceItem );
		}

		// Ŭ���̾�Ʈ�� ������ ������
		cltGameResponse_Set_Marry clinfo( siDestPID,				pclMsg->m_szDestName,
										siWeddingDate,
										siLoveLevle,				siConnectMode,
										&pclMsg->m_clSourceItem,	pclMsg->m_siSourceItemPos );

		cltMsg clMsg( GAMEMSG_RESPONSE_SET_MARRY, sizeof(clinfo), (BYTE*)&clinfo );
		pclSourceChar->SendNetMsg( (sPacketHeader*)&clMsg );

		if ( IsWhereServiceArea(ConstSwitch_NewLog) ) 
		{
			// param1 = ��ȥ��
			// param2 = ������
			// param3 = ���� ������ ��ġ

			SendLog_WriteLogToDB( LOGCOMMAND_TYPE_FAMILY, LOGCOMMAND_INDEX_FAMILY_MARRY, 
				0, pclSourceChar, NULL, siDestPID, 0, 0, &pclMsg->m_clSourceItem, pclMsg->m_clSourceItem.siItemNum, 0, 0, 0, 0, 0, 
				siWeddingDate, siLoveLevle, pclMsg->m_siSourceItemPos, 0, 0, NULL, NULL);
		}

	}

	// ûȥ���� ���
	if ( pclDestChar )
	{
		// ��ȥ ���� ���� ����
		if ( pclDestChar->pclMarriage )
		{
			pclDestChar->pclMarriage->Set( siSourcePID, pclMsg->m_szSourceName, siWeddingDate, 0, FALSE, siLoveLevle, 0, 0, siConnectMode );
		}

		// ���� ������ ����
		if ( (PERSONITEM_INV0 <= pclMsg->m_siDestItemPos) && ((PERSONITEM_INV0 + MAX_INVITEM_PER_PERSON) > pclMsg->m_siDestItemPos) )
		{
			pclDestChar->pclCI->clCharItem.clItem[pclMsg->m_siDestItemPos].Set( &pclMsg->m_clDestItem );
		}

		// Ŭ���̾�Ʈ�� ������ ������
		cltGameResponse_Set_Marry clinfo( siSourcePID,			pclMsg->m_szSourceName,
										siWeddingDate,
										siLoveLevle,			siConnectMode,
										&pclMsg->m_clDestItem,	pclMsg->m_siDestItemPos );

		cltMsg clMsg( GAMEMSG_RESPONSE_SET_MARRY, sizeof(clinfo), (BYTE*)&clinfo );
		pclDestChar->SendNetMsg( (sPacketHeader*)&clMsg );

		if ( IsWhereServiceArea(ConstSwitch_NewLog) ) 
		{
			// param1 = ��ȥ��
			// param2 = ������
			// param3 = ���� ������ ��ġ

			SendLog_WriteLogToDB( LOGCOMMAND_TYPE_FAMILY, LOGCOMMAND_INDEX_FAMILY_MARRY, 
				0, pclDestChar, NULL, siSourcePID, 0, 0, &pclMsg->m_clDestItem, pclMsg->m_clDestItem.siItemNum, 0, 0, 0, 0, 0, 
				siWeddingDate, siLoveLevle, pclMsg->m_siDestItemPos, 0, 0, NULL, NULL);
		}
	}



}

void cltServer::DoMsg_DBMSG_RESPONSE_GET_MARRY_INFO( sPacketHeader* pPacket )
{
	sDBResponse_Get_Marry_Info* pclMsg = (sDBResponse_Get_Marry_Info*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	SI32			siMyCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclMyChar	= pclCM->GetCharServer( siMyCharID );
	if ( NULL == pclMyChar )
	{
		return;
	}

	if ( NULL == pclMyChar->pclMarriage )
	{
		return;
	}

	SI16 siConnectMode = MARRIAGE_MATE_CONNECT_MODE_NONE;

	// �켱 ������ ������ ��簪���� �����Ѵ� ���Ӱ����� �Ʒ�����
	pclMyChar->pclMarriage->Set( pclMsg->m_siMatePersonID,		pclMsg->m_szMateName,
								pclMsg->m_siWeddingDateVary,	pclMsg->m_siItemGetDateVary,	FALSE,
								pclMsg->m_siLoveLevel,			pclMsg->m_siUseDateVary,		pclMsg->m_siLimitDateVary,
								siConnectMode );

	//====================================================================
	// ��ȥ �ߴٸ� ������ üũ�Ѵ�
	//====================================================================
	if ( TRUE == pclMyChar->IsMarried() )
	{
		//====================================================================
		// ������ üũ
		//====================================================================
		SI16 siCalcLoveLevel = pclMyChar->pclMarriage->CalcNowLoveLevel( pclTime->clDate.GetHourVary() );
		// ��� �Ǿ� ���°��� ���� �������� �ٸ��ٸ�
		if ( siCalcLoveLevel != pclMyChar->pclMarriage->GetLoveLevel() )
		{
			// �ϴ� �̰����� ������ �ְ�(�ٸ������� ���ɶ��� LoveLevel�� ���̹Ƿ�)
			pclMyChar->pclMarriage->SetLoveLevel( siCalcLoveLevel );
			pclMyChar->pclMarriage->SetLimitHourVary( pclMsg->m_siLimitDateVary + 24 );

			// DB�� ������
			sDBRequest_Marry_Love_Set clMsg( pclMyChar->pclCI->GetPersonID(), 
											pclMyChar->pclMarriage->GetUseHourVary(), pclMyChar->pclMarriage->GetLimitHourVary(),
											pclMyChar->pclMarriage->GetLoveLevel() );
			pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
		}

		//====================================================================
		// ����� üũ
		//====================================================================
		SI32			siMateCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siMatePersonID );
		cltCharServer*	pclMateChar		= pclCM->GetCharServer( siMateCharID );
		if ( (pclMateChar) && (pclMateChar->pclMarriage) )
		{
			// ������ ����� ������ �˻縦 ����� ĳ���͵� �˻��Ѵ�
			if ( siCalcLoveLevel != pclMateChar->pclMarriage->GetLoveLevel() )
			{
				// ���� �ɸ��Ϳ� �����Ű��
				pclMateChar->pclMarriage->SetLoveLevel( siCalcLoveLevel );
				pclMateChar->pclMarriage->SetLimitHourVary( pclMsg->m_siLimitDateVary + 24 );

				// DB�� ������
				sDBRequest_Marry_Love_Set clMsg( pclMateChar->pclCI->GetPersonID(), 
											pclMateChar->pclMarriage->GetUseHourVary(), pclMateChar->pclMarriage->GetLimitHourVary(),
											pclMateChar->pclMarriage->GetLoveLevel() );
				pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
			}

			// ����ڰ� ������ �ִٰ� ������ �ش�
			siConnectMode = MARRIAGE_MATE_CONNECT_MODE_CONNECT;

			SI16 siLoveLevel = pclMateChar->pclMarriage->GetLoveLevel();

			// ���� �ʻ� �־�� �Ѵ�
			if ( pclMyChar->GetMapIndex() == pclMateChar->GetMapIndex() )
			{
				siConnectMode = MARRIAGE_MATE_CONNECT_MODE_SAMEMAP;

				cltCharChangeStatusInfo			clChangeInfo( CHANGETYPE_DISPLAYLOVELEVEL, false, siLoveLevel );
				cltGameMsgResponse_StatusInfo	clInfo( pclMateChar->GetCharUnique(), &clChangeInfo );

				cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clInfo), (BYTE*)&clInfo );
				pclMateChar->SendNeighbourMsg( &clStatusInfoMsg, false );
			}

			pclMateChar->pclMarriage->SetConnectMode( siConnectMode );

			SendServerResponseMsg( 0, SRVAL_SET_MARRY_WITHSAMEMAP, siConnectMode, siLoveLevel, pclMateChar->GetCharUnique() );
		}
	}

	// �̰����� ���� ���� ������ ���ش�
	pclMyChar->pclMarriage->SetConnectMode( siConnectMode );

	// Ŭ���̾�Ʈ�� ������ ������
	cltGameResponse_Get_Marry_Info clinfo( pclMsg->m_siMatePersonID,	pclMsg->m_szMateName, 
										pclMsg->m_siWeddingDateVary,	pclMsg->m_siItemGetDateVary,
										pclMsg->m_siLoveLevel,			pclMsg->m_siUseDateVary,		pclMsg->m_siLimitDateVary,
										siConnectMode );

	cltMsg clMsg( GAMEMSG_RESPONSE_GET_MARRY_INFO, sizeof(clinfo), (BYTE*)&clinfo );
	pclMyChar->SendNetMsg( (sPacketHeader*)&clMsg );

}

void cltServer::DoMsg_DBMSG_RESPONSE_MARRY_ITEMGET_SET(sPacketHeader* pPacket)
{
	sDBResponse_Marry_ItemGet_Set* pclMsg = (sDBResponse_Marry_ItemGet_Set*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( (0 > pclMsg->m_siPersonID) || (0 > pclMsg->m_siItemGetDateVary) )
	{
		return;
	}

	SI32			siMyCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclMyChar	= pclCM->GetCharServer( siMyCharID );
	if ( NULL == pclMyChar )
	{
		return;
	}

	// Ȥ�� ��ȥ�� ���ߴٸ�?
	if ( FALSE == pclMyChar->IsMarried() )
	{
		return;
	}

	// ��û�ߴ����� Ǯ���ְ�
	pclMyChar->pclMarriage->SetRequestItemGet( FALSE );

	// �����ɸ��� ������ �ְ�
	pclMyChar->pclMarriage->SetItemGetDateVary( pclMsg->m_siItemGetDateVary );

	// Ŭ���̾�Ʈ���� �����ش�
	cltGameResponse_Marry_ItemGet_Set clinfo( pclMsg->m_siItemGetDateVary );
	cltMsg clMsg( GAMEMSG_RESPONSE_MARRY_ITEMGET_SET, sizeof(clinfo), (BYTE*)&clinfo );
	pclMyChar->SendNetMsg( (sPacketHeader*)&clMsg );

}

void cltServer::DoMsg_DBMSG_RESPONSE_MARRY_LOVE_SET(sPacketHeader* pPacket)
{
	sDBResponse_Marry_Love_Set* pclMsg = (sDBResponse_Marry_Love_Set*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	SI16 siLoveLevel		= pclMsg->m_siLoveLevel;
	SI32 siUseHourVary		= pclMsg->m_siUseHourVary;
	SI32 siLimitHourVary	= pclMsg->m_siLimitHourVary;

	// �� ĳ����
	SI32			siMyCharID	= pclCM->GetIDFromPersonID( pclMsg->m_siPersonID );
	cltCharServer*	pclMyChar	= pclCM->GetCharServer( siMyCharID );
	if ( NULL == pclMyChar )
	{
		return;
	}

	if ( FALSE == pclMyChar->IsMarried() )
	{
		return;
	}

	pclMyChar->pclMarriage->SetLoveLevel( siLoveLevel );
	pclMyChar->pclMarriage->SetUseHourVary( siUseHourVary );
	pclMyChar->pclMarriage->SetLimitHourVary( siLimitHourVary );

	cltGameResponse_Marry_Love_Set clinfo( siLoveLevel, siUseHourVary, siLimitHourVary );
	cltMsg clMsg( GAMEMSG_RESPONSE_MARRY_LOVE_SET, sizeof(clinfo), (BYTE*)&clinfo );
	pclMyChar->SendNetMsg( (sPacketHeader*)&clMsg );


	// ����� ĳ����
	SI32			siMateCharID	= pclCM->GetIDFromPersonID( pclMyChar->pclMarriage->GetMatePersonID() );
	cltCharServer*	pclMateChar		= pclCM->GetCharServer( siMateCharID );
	if ( NULL == pclMateChar )
	{
		return;
	}

	if ( FALSE == pclMateChar->IsMarried() )
	{
		return;
	}

	pclMateChar->pclMarriage->SetLoveLevel( siLoveLevel );
	pclMateChar->pclMarriage->SetLimitHourVary( siLimitHourVary );

	cltGameResponse_Marry_Love_Set clMateInfo( siLoveLevel, pclMateChar->pclMarriage->GetUseHourVary(), siLimitHourVary );
	cltMsg clMateMsg( GAMEMSG_RESPONSE_MARRY_LOVE_SET, sizeof(clMateInfo), (BYTE*)&clMateInfo );
	pclMateChar->SendNetMsg( (sPacketHeader*)&clMateMsg );


	// �λ���� ���� �ʿ� ������� �������� ����������� �����ش�
	if ( pclMyChar->GetMapIndex() == pclMateChar->GetMapIndex() )
	{
		cltCharChangeStatusInfo			clChangeInfo( CHANGETYPE_DISPLAYLOVELEVEL, false, siLoveLevel );

		cltGameMsgResponse_StatusInfo	clMyInfo( pclMyChar->GetCharUnique(), &clChangeInfo );
		cltMsg clMyStatusInfoMsg( GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clMyInfo), (BYTE*)&clMyInfo );
		pclMyChar->SendNeighbourMsg( &clMyStatusInfoMsg, false );

		cltGameMsgResponse_StatusInfo	clMateInfo( pclMateChar->GetCharUnique(), &clChangeInfo );
		cltMsg clMateStatusInfoMsg( GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clMateInfo), (BYTE*)&clMateInfo );
		pclMateChar->SendNeighbourMsg( &clMateStatusInfoMsg, false );
	}

}
void cltServer::DoMsg_DBMSG_RESPONSE_GET_FAMILYINFO( sPacketHeader* pPacket )
{
	sDBResponse_Get_FamilyInfo* pclMsg = (sDBResponse_Get_FamilyInfo*)pPacket;
	UpdateFamily(&pclMsg->m_cFamily,pclMsg->m_siPersonID)	;
}
void cltServer::DoMsg_DBMSG_RESPONSE_FAMILY_ADD_CHILDREN( sPacketHeader* pPacket )
{
	sDBResponse_Family_Add_Children* pclMsg = (sDBResponse_Family_Add_Children*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
	UpdateFamily(&pclMsg->m_cFamily,0)	;	// �ڿ� �۽����̵�� �����Ҷ� �������� ģ������ ���ؼ� �����Ŵ� �ٸ����� �ʿ� ����.
}
void cltServer::DoMsg_DBMSG_RESPONSE_FAMILY_DEL_CHILDREN( sPacketHeader* pPacket )
{
	sDBResponse_Family_Del_Children* pclMsg = (sDBResponse_Family_Del_Children*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
	UpdateFamily(&pclMsg->m_cFamily,0)	;	// ���϶��� ���� �����״� �����ʿ䰡 ��� db���� personid�� �ȹ޾Ҵ� �����鸸 ����� ������Ʈ
}
void cltServer::DoMsg_DBMSG_RESPONSE_FAMILY_PLUS_ADD_DATE( sPacketHeader* pPacket )
{
	sDBResponse_Family_Plus_Add_Date* pclMsg = (sDBResponse_Family_Plus_Add_Date*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
	UpdateFamily(&pclMsg->m_cFamily,0)	;	// �굵 �� �� ���ִ°Ŷ� personId�� �ȹ���
}
void cltServer::DoMsg_DBMSG_RESPONSE_FAMILY__SET_COOKIE_EFFECT( sPacketHeader* pPacket )
{
	// ��Ű�� ȥ�ڸ� ������ �ǳ� -0- �̷��� �� ���� �ʿ� ���� �׳� ������Ʈ �ϴ� ������� �ؾ���.
	sDBResponse_Set_My_CookeEffect * pclMsg = (sDBResponse_Set_My_CookeEffect *)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}
	UpdateFamily(&pclMsg->m_cFamily,pclMsg->m_siPersonID)	;
}

void cltServer::DoMsg_DBMSG_RESPONSE_PERSONSLOTNUMCHANGE(sPacketHeader* pPacket)
{
	sDBResponse_PersonSlotNumChange* pclMsg = (sDBResponse_PersonSlotNumChange*)pPacket;
	if ( NULL == pclMsg )
	{
		return;
	}

	if ( 1 != pclMsg->m_siResult )
	{
		return;
	}

	SI32			siGetID	= pclMsg->m_uiCharID;
	cltCharServer*	pclChar	= pclCM->GetCharServer( siGetID );
	if ( NULL == pclChar )
	{
		return;
	}

	// ����Ʈ�� �ٽ� �о������ �Ѵ�
	sDBRequest_PersonListInfoGet clMsg( pclMsg->m_uiCharID, pclChar->pclCI->clBI.siIDNum );
	SendDBMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg );


	// [��ȣ_NHN->NDOORS] ĳ���ͺ����ҿ��� ĳ���� ������
	if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
	{
		SendLog_WriteLogToDB(	LOGCOMMAND_TYPE_CHARACTOR, 
								LOGCOMMAND_INDEX_CHARHOUSE_GET, 
								0, 
								pclChar, 
								NULL, 
								0, 
								0, 
								0, 
								NULL, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								0, 
								(TCHAR*)pclChar->pclCI->clBI.GetName(), 
								NULL);
	}
}

void cltServer::DoMsg_DBMSG_RESPONSE_SKILLBOOK_MAGICINFO_GET(sPacketHeader* pPacket)
{
	sDBResponse_SkillBook_MagicInfo_Get* pclMsg = (sDBResponse_SkillBook_MagicInfo_Get*)pPacket;

	if( 0 >= pclMsg->m_siResult ) return;

	cltCharServer*	pclChar	= pclCM->GetCharServer( pclMsg->m_usCharID );
	if( NULL == pclChar ) return;

	SI32 siCount = 0;
	for( siCount = 0; siCount < SKILLBOOK_MAGIC_NUM; ++siCount )
	{
		if(    0 >= pclMsg->m_clAddSkillBookMagic[siCount].GetMagicKind() 
			|| 0 >= pclMsg->m_clAddSkillBookMagic[siCount].GetSkillUnique() )
		{
			break;	
		} 

		pclChar->pclCharSkillBookInfo->SetSkillBookMagic( pclMsg->m_iIndex[siCount], &pclMsg->m_clAddSkillBookMagic[siCount] );
	}

	for( siCount = 0; siCount < MAX_SKILLBOOK_SUBMAGIC_NUM; ++siCount )
	{
		if(    0 >= pclMsg->m_clAddSkillBookSubMagic[siCount].GetMagicKind() 
			|| 0 >= pclMsg->m_clAddSkillBookSubMagic[siCount].GetParentMagicKind() )
		{
			break;	
		}
		pclChar->pclCharSkillBookInfo->AddSkillBookSubMagic( &pclMsg->m_clAddSkillBookSubMagic[siCount] );
	}

	cltGameMsgResponse_SkillBook_MagicInfo_Get clInfo;
	memcpy( clInfo.m_iIndex,					pclMsg->m_iIndex,					sizeof(clInfo.m_iIndex) );
	memcpy( clInfo.m_clAddSkillBookMagic,		pclMsg->m_clAddSkillBookMagic,		sizeof(clInfo.m_clAddSkillBookMagic) );
	memcpy( clInfo.m_clAddSkillBookSubMagic,	pclMsg->m_clAddSkillBookSubMagic,	sizeof(clInfo.m_clAddSkillBookSubMagic) );
	cltMsg clMsg( GAMEMSG_RESPONSE_SKILLBOOK_MAGICINFO_GET, PACKET(clInfo) );
	pclChar->SendNetMsg((sPacketHeader*)&clMsg );
}


void cltServer::DoMsg_DBMSG_RESPONSE_SKILLBOOK_MAGIC_ADD(sPacketHeader* pPacket)
{
	sDBResponse_SkillBook_Magic_Add* pclMsg = (sDBResponse_SkillBook_Magic_Add*)pPacket;

	if( 0 >= pclMsg->m_siResult ) return;

	cltCharServer*	pclChar	= pclCM->GetCharServer( pclMsg->m_usCharID );
	if( NULL == pclChar ) return;

	pclChar->pclCharSkillBookInfo->AddSkillBookMagic( pclChar->GetAttackType(), &pclMsg->m_clAddSkillBookMagic );
	
	cltSkillBookMagic*	pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagic( pclChar->GetAttackType() );	
	if( NULL == pclSkillBookMagic ) return;

	cltGameMsgResponse_SkillBook_Magic_Add clInfo( pclSkillBookMagic );
	cltMsg clMsg( GAMEMSG_RESPONSE_SKILLBOOK_MAGIC_ADD, PACKET(clInfo) );
	pclChar->SendNetMsg((sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_SKILLBOOK_MAGIC_DEL(sPacketHeader* pPacket)
{
	sDBResponse_SkillBook_Magic_Del* pclMsg = (sDBResponse_SkillBook_Magic_Del*)pPacket;

	if( 0 >= pclMsg->m_siResult ) return;

	cltCharServer*	pclChar	= pclCM->GetCharServer( pclMsg->m_usCharID );
	if( NULL == pclChar ) return;

	cltSkillBookMagic* pclSkillBookMagic = pclChar->pclCharSkillBookInfo->GetSkillBookMagicIndex( pclMsg->m_iIndex );
	if( pclSkillBookMagic )
	{
		cltLetterMsg_SkillBookMagicDel clletter( pclSkillBookMagic->GetMagicKind() );
		SendLetterMsg(pclChar->pclCI->GetPersonID(), (cltLetterHeader*)&clletter );
	}

	pclChar->pclCharSkillBookInfo->DelSkillBookMagic( pclMsg->m_iIndex );

	cltGameMsgResponse_SkillBook_Magic_Del clInfo( pclMsg->m_iIndex );
	cltMsg clMsg( GAMEMSG_RESPONSE_SKILLBOOK_MAGIC_DEL, PACKET(clInfo) );
	pclChar->SendNetMsg((sPacketHeader*)&clMsg );
}

void cltServer::DoMsg_DBMSG_RESPONSE_SKILLBOOK_SUBMAGIC_ADD(sPacketHeader* pPacket)
{
	sDBResponse_SkillBook_SubMagic_Add* pclMsg = (sDBResponse_SkillBook_SubMagic_Add*)pPacket;

	if( 0 >= pclMsg->m_siResult ) return;

	cltCharServer*	pclChar	= pclCM->GetCharServer( pclMsg->m_usCharID );
	if( NULL == pclChar ) return;

	pclChar->pclCharSkillBookInfo->AddSkillBookSubMagic( &pclMsg->m_clAddSkillBookSubMagic );

	cltGameMsgResponse_SkillBook_SubMagic_Add clInfo( &pclMsg->m_clAddSkillBookSubMagic );
	cltMsg clMsg( GAMEMSG_RESPONSE_SKILLBOOK_SUBMAGIC_ADD, PACKET(clInfo) );
	pclChar->SendNetMsg((sPacketHeader*)&clMsg );
} 

void cltServer::DoMsg_DBMSG_RESPONSE_SKILLBOOK_SUBMAGIC_DEL(sPacketHeader* pPacket)
{
	sDBResponse_SkillBook_SubMagic_Del* pclMsg = (sDBResponse_SkillBook_SubMagic_Del*)pPacket;

	if( 0 >= pclMsg->m_siResult ) return;

	cltCharServer*	pclChar	= pclCM->GetCharServer( pclMsg->m_usCharID );
	if( NULL == pclChar ) return;

	pclChar->pclCharSkillBookInfo->DelSkillBookSubMagic( &pclMsg->m_clAddSkillBookSubMagic );

	cltGameMsgResponse_SkillBook_SubMagic_Del clInfo( &pclMsg->m_clAddSkillBookSubMagic );
	cltMsg clMsg( GAMEMSG_RESPONSE_SKILLBOOK_SUBMAGIC_DEL, PACKET(clInfo) );
	pclChar->SendNetMsg((sPacketHeader*)&clMsg );

	if( pclMsg->m_bTimeOver )
	{
		cltLetterMsg_SkillBookMagicDel clletter( pclMsg->m_clAddSkillBookSubMagic.GetMagicKind() );
		SendLetterMsg(pclChar->pclCI->GetPersonID(), (cltLetterHeader*)&clletter );
	}
}