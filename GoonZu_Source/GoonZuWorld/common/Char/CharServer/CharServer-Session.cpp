//---------------------------------
// 2004/11/16 ���°�
//---------------------------------

#include "CommonLogic.h"
#include "..\CommonLogic\Msg\MsgType-Friend.h"
#include "..\CommonLogic\MsgType-Person.h"

#include "Server.h"
#include "..\..\Server\Statistics\Statistics.h"
#include "../../Server/GameDBProcess/LogDBProcess.h"

#include "Char-Server.h"
#include "../../Server/TraceBoot.h"
#include "Char\CharManager\CharManager.h"
#include "..\Common\Father\Father.h"
#include "Master\Master.h"
#include "../../../Client/NInterface/NMaster/NMaster.h"

#include "Ask\Ask.h"
#include "..\Common\PartyMgr\PartyMgr.h"

#include "..\Common\PrivateTradeOrderMgr\PrivateTradeOrderMgr.h"
#include "Autohunting\Autohunting.h"

#include "HashTable/HashTable_StringInt.h"
#include "HashTable/HashTable_IntInt.h"


#include "..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest2.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-DailyQuest3.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Summon.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Village.h"
//#include "..\..\DBManager\GameDBManager_World\dbgamelogmsg.h"
#include "../../DBManager/GameDBManager_World/DBLogMsg.h"
#include "../../DBManager/GameDBManager_World/DBMsg-System.h"



#include "ITZSession.h"
#include "ITZSendBuffer.h"

#include "../../Server/AuthServer/AuthMsg.h"

#include "../../../Apex/ApexGoonZu.h"

#include "../../CommonLogic/MsgRval-Define.h"
#include "../../DBManager/GameDBManager_World/DBMsg-Item.h"
#include "../../DBManager/GameDBManager_World/DBMsg-PostOffice.h"

extern cltCommonLogic* pclClient;

///----------------------------------
// Session ������ ȣ�� �Լ�. 
//-----------------------------------
void cltCharServer::Init()
{
	ITZPerson::Init();

	// ĳ���� ���� 
	// �Ϲ� ĳ���͸� �����Ѵ�. 
	pclClient->MakeSimpleChar(m_uiCharID, m_uiCharID);

	// ��ȣȭŰ�� ����ũ ���� Ŭ���̾�Ʈ�� ���� ���� ��û�Ѵ�. 
	// ĳ���Ͱ� �����ǰ� �ѹ��� �̷������ �ȴ�. 
	bSendSecretNCHarUniqueSwitch = true;

	TraceBoot("CHARSERVER Init charID=%d charUnique=%d session=%p -> flag SET",
		m_uiCharID, GetCharUnique(), (void*)GetSession());

	siAccountStatus = -2;

#ifdef USE_GAME_GUARD_SERVER
	// ���Ӱ��� ��ü �ʱ�ȭ
	CCSAuthInit();
#endif
}

// ������ �����ϱ� ���� ĳ���Ͱ� �����ؾ� �� �ϵ�. 
void cltCharServer::CloseAction(cltServer* pclServer, bool bnormalquitswitch)
{
	//	cltServer* pclserver = (cltServer*)pclClient;

	// PC�� �α׾ƿ�
	if( pclCI->clBI.bPCRoom) {

		pclCI->clBI.uiGMMode = false;
		pclServer->siPCRoomCharNum--;
	}
	//---------------------------------------------
	// ���� �Ǿ� �ִ� ������ ����� �ִٸ� ó���Ѵ�. 
	//---------------------------------------------
	ItemUseReserveAction();

	//---------------------------------------------
	// ���� �Ǿ� �ִ� źȯ �����ۻ���� �ִٸ� ó���Ѵ�. 
	//---------------------------------------------
	BulletUseReserveAction();

	//------------------------------------------
	// ��ȯ���� ������ �����Ѵ�.
	//------------------------------------------
	// DB�� ��ȯ���� �����Ѵ�.
	SendDBSummon(SETSUMMON_REASON_UPDATE, MAX_SUMMON_OWN);

	SI32 siSummonID = GetSummonID();
	if(pclCM->IsValidID(siSummonID))
	{
		// �� �������� ������ �����Ѵ�. 
		pclCM->CR[siSummonID]->DeleteInCell();
		pclCM->DeleteChar(siSummonID);
	}

	//------------------------------------------
	// ��ȣ������ ������ �����Ѵ�
	//------------------------------------------
	DeleteSoulGuard( );

	//----------------------------------
	// ���� ��û�� ����Ʈ���� �����Ѵ�. 
	//-----------------------------------
	pclClient->pclFatherManager->DeleteApplyer(pclCI->GetPersonID());

	//----------------------------------
	// ���� ����Ʈ���� �����Ѵ�. 
	//----------------------------------
	pclClient->pclMasterManager->Delete(GetID());

	//----------------------------------
	// ���� ����Ʈ���� �����Ѵ�. 
	//----------------------------------
	//	if(pclClient->pclAskManager)
	//{
	//	pclClient->pclAskManager->DelAskPersonID(pclCI->GetPersonID());
	//}

	//[����] �����. ��������. �亯���� ���� �亯�ڷ� ����� �� ����.
	pclClient->pclHelperManager->DelQuestion( this );
	pclClient->pclHelperManager->DelAnswer( this );

	//[����] ��ƼȦ���� ���� ������ ����.
	PartyHallItemRemove(siID);

	//-----------------------------------
	// ��Ƽ ���̾��ٸ�, ��Ƽ�� ���� �����ش�.
	//-----------------------------------
	if ( siPartyIndex > 0 )
	{
		SI32 siRet = 0;		
		pclClient->pclPartyMgr->LeaveParty( pclCI->GetPersonID(), siPartyIndex, &siRet );
		siPartyIndex = 0;
	}

	// ��Ƽ��Ī�� ����Ʈ�� ����� ���¿��ٸ� ����Ʈ�� �����ش�.
	if ( NULL != pclClient->m_pServer_PartyMatchingMgr )
	{
		pclClient->m_pServer_PartyMatchingMgr->Remove_PartyMatching( GetCharUnique() );
	}


	//-----------------------------------
	// �ŷ�â�� ������̾��ٸ� ��ȯ�Ѵ�.
	//-----------------------------------
	pclClient->pclPrivateTradeOrderMgr->Cancel( pclCI->GetPersonID() );

	//----------------------------------------------------------------
	// �ڵ� ��� �Ƿ����̾��ٸ� �����Ѵ�.
	//-----------------------------------------------------------------
	//pclClient->pclAutoHuntingManager->DeletePerson(pclCI->clBI.GetName());


	//-----------------------------------
	// ������ ���� ������ ģ���鿡�� �˸�
	//-----------------------------------

	if(pclFriendInfo)
	{
		for ( SI16 i = 0; i < MAX_FRIEND_NUMBER; ++i )
		{
			if ( pclFriendInfo->clFriend[ i ].siPersonID > 0 )
			{
				SI32 ToID = pclClient->pclCM->GetIDFromPersonID( pclFriendInfo->clFriend[ i ].siPersonID );

				if ( pclCM->IsValidID(ToID) )
				{
					cltGameMsgResponse_FriendLogOut clGameMsgResponse_FriendLogOut( pclCI->GetPersonID() );
					cltMsg clMsg( GAMEMSG_RESPONSE_FRIENDLOGOUT, sizeof( clGameMsgResponse_FriendLogOut ), (BYTE*)&clGameMsgResponse_FriendLogOut );
					pclClient->pclCM->CR[ToID]->SendNetMsg((sPacketHeader*)&clMsg);
				}
			}
		}
	}

	//-----------------------------------
	// DailyQuest ������ ���� �����ص�.
	//-----------------------------------
	if ( pclCI->clDailyQuestInfo.siMonsterKind > 0 )
	{
		sDBRequest_DailyQuestChange clMsg( pclCI->GetPersonID(), GetID(), pclCI->clDailyQuestInfo.siKillMonsterNum, pclCI->clDailyQuestInfo.siLeftTime, pclCI->clDailyQuestInfo.siMonsterKind );
		pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	//-----------------------------------
	// DailyQuest2 ������ ���� �����ص�.
	//-----------------------------------
	if ( pclCI->clDailyQuest2Info.siMonsterKind > 0 )
	{
		sDBRequest_DailyQuest2Change clMsg( pclCI->GetPersonID(), GetID(), pclCI->clDailyQuest2Info.siKillMonsterNum, pclCI->clDailyQuest2Info.siLeftTime, pclCI->clDailyQuest2Info.bHiddenQuest, pclCI->clDailyQuest2Info.siMonsterKind );
		pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	//-----------------------------------
	// DailyQuest3 ������ ���� �����ص�.
	//-----------------------------------
	if ( pclCI->clDailyQuest3Info.siItemUnique > 0 )
	{
		sDBRequest_DailyQuest3Change clMsg( pclCI->GetPersonID(), GetID(), pclCI->clDailyQuest3Info.siMadeItemNum, pclCI->clDailyQuest3Info.siLeftTime, pclCI->clDailyQuest3Info.bHiddenQuest );
		pclServer->pclGameDBProcess->SendMsg((sPacketHeader *)&clMsg);
	}
	// ���� �α� �ƿ��Ҷ� PVP ���̶��
	SI32 siIID	=	GetID()	;
	if(pclCM->IsValidID( siIID ) == true )
	{
		if( pclCM->CR[siIID]->siEnemUnique	>	0 )
		{
			cltServer* pclserver = (cltServer*)pclClient;

			SI32 siWarpChar	=	siIID	;	// ������ �ɸ��� �� �ɸ���.
			if( pclCM->IsValidID( siWarpChar ) == false )	return	;// ���� ���̵� ���� �˻� ���� 
			SI32 siWarpChaeParentID = pclCM->CR[siWarpChar]->GetSummonParentID()	;		// ����ź �ɸ����� �θ� �ɸ��͸� ���Ѵ�.
			if ( siWarpChaeParentID == 0 ) siWarpChaeParentID = siWarpChar	;				// ���� ����ź �ɸ��Ͱ� ���ɸ� �׳� ���̵� ����.
			cltCharServer* pclWarpChar = pclCM->GetCharServer(siWarpChaeParentID);			if(pclWarpChar == NULL)			return	;

			SI16 siWinnerCharID = pclWarpChar->siEnemUnique	;						// ���� Ÿ�� �����Ƿ� ����ź �ɸ����� ���� ���� ������ �̱� �ɸ����̴�.
			if( pclCM->IsValidID( siWinnerCharID ) == false )	return	;			// ���� ���̵� ���� �˻� ����
			cltCharServer* pclWinnerChar = pclCM->GetCharServer(siWinnerCharID);			if(pclWinnerChar == NULL)			return	;

			// ���� ſ���ϱ� ������ ���̴�.
			pclWarpChar->siEnemUnique	=	0	;		// ������ �ɸ��� ���� �ʱ�ȭ ���ش�
			pclWinnerChar->siEnemUnique	=	0	;
			if( pclClient->IsCountrySwitch( Switch_ValenTineWar ) )
			{
				//�߷�Ÿ�ε��� ���� ���� üũ, Ŀ���̳� �ַζ�� ���߰�, �������� ���� �ʾ��� ��쿡�� �ش�.
				SI32 siCouple = pclWinnerChar->pclValentineWar->GetCouple()	;// �̱� ����� Ŀ�� ����
				SI32 siLoser = pclWarpChar->pclValentineWar->GetCouple()	;// �� ����� Ŀ�� ����
				bool bGetPrize = pclWinnerChar->pclValentineWar->GetPrize()	;	// �̱����� �������� �޾Ҿ�����
				if( abs(pclWarpChar->pclCI->clIP.GetLevel() - pclWarpChar->pclCI->clIP.GetLevel() ) < 11
					&& siCouple != 0 && siLoser != 0
					&& siCouple != siLoser 
					&& bGetPrize == false )
				{
					SI32 siRewardItemUnique	=	ITEMUNIQUE(2781) ;
					if( siRewardItemUnique ) 
					{
						cltItem clItem;
						SI16 brareswitch = 0;
						if ( pclClient->pclItemManager->MakeRandItemUnique( siRewardItemUnique,&clItem,0,0,&brareswitch,0,0,0,0,0) == true ) 
						{
							clItem.siItemNum = 1;
							SI32 rtnvalue = 0;
							if(pclWinnerChar->InputItem(pclClient->pclItemManager, &clItem, INPUTITEMPERSON_OPTION_VALENTINEWAR_PRIZE, &rtnvalue))
							{
								// �������� �ְ� ��ٰ� ���� ���ش�.
								sDBRequest_Valantine_Get_Prize clMsg( pclWinnerChar->pclCI->GetPersonID(), true );
								pclServer->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
							}
							else
							{
								if(rtnvalue == CANADDINV_FAIL_FULL)
								{
									sDBRequest_PostOfficeMasterSendItem clMsg( INPUTITEMPERSON_OPTION_VALENTINEWAR_PRIZE, 0, (TCHAR*)pclWinnerChar->GetName(), &clItem,&pclServer->pclTime->clDate );
									pclServer-> pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg);
									// �������� �ְ� ��ٰ� ���� ���ش�.
									sDBRequest_Valantine_Get_Prize clMsg2( pclWinnerChar->pclCI->GetPersonID(), true );
									pclServer->pclGameDBProcess->SendMsg( DBSELECT_SYSTEM, (sPacketHeader *)&clMsg2);
								}
							}
						}		
					}
				}

			}			 
			// ���� Ŭ���̾�Ʈ�� �˷��ش�.
			{//�¸���
				// ���ΰ� pvp�� �����鼭 ���� �ߵ����� ����
				pclWinnerChar->DeletePoison();						// �ߵ� ���¸� �����Ѵ�.
				pclWinnerChar->DeletePoisonSetItemEffect();			// �ߵ� ���¸� �����Ѵ�.
				pclWinnerChar->DeleteIce();							// ���� ���¸� �����Ѵ�.
				pclWinnerChar->DeleteCurseAttack();					// ���ݷ��� ���� ���¸� �����Ѵ�.
				pclWinnerChar->DeleteCurseCritical();				// Critical Ȯ���� �������� ���¸� �����Ѵ�.
				pclWinnerChar->DeleteCurseDefense();				// ������ �������� ���¸� �����Ѵ�.
				pclWinnerChar->DeleteCurseMovespeed();				// �̵��ӵ� ���� ���¸� �����Ѵ�.
				pclWinnerChar->DeleteMagicFire();					// �ҿ� Ÿ�� ���¸� �����Ѵ�.
				pclWinnerChar->DeleteFrozen();						// ���� ���� ���¸� �����Ѵ�.
			}
			// �¸���
			cltGameMsgResponse_PVP_Request clWinner( END_PVP , WIN_PVP, pclWinnerChar->GetCharUnique(), TEXT("") );	
			cltMsg clMsg( GAMEMSG_RESPONSE_PVP_FILED, sizeof(clWinner), (BYTE*)&clWinner );
			pclWinnerChar->SendNeighbourMsg(&clMsg, true);

			pclWinnerChar->IncQuestPara( QUEST_CLEAR_COND_PVPWIN, 0, 1 );

			if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			{
				// [�α��߰� : �ռ����� 2008. 10. 29] // pvp ���
				// PCK : ���� - PersonID�� ĳ�����̸��� ���ؼ� ���� ���� �����Ѵ�. (09.07.31)
				// para1	:	�¸��� personID
				// para2	:	�й��� personID

				TCHAR szCharName[100];
				ZeroMemory(szCharName, sizeof(szCharName));
				StringCchPrintf(szCharName, sizeof(szCharName), TEXT("WinnerChar:%s, LoserChar:%s"), pclWinnerChar->GetName(), pclWarpChar->GetName());

				pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_PERSONALPVP, LOGCOMMAND_INDEX_PERSONALPVP_END_LOGOUT, 
					0, this,NULL, 0, 0, 0, 0, NULL, 0, 0, 0, 0, 0, 
					pclWinnerChar->pclCI->GetPersonID(), 
					pclWarpChar->pclCI->GetPersonID(), 0, 0, 0, szCharName, NULL);
			}
			{
				// ������ �����.
				TCHAR fileNameBuffer[256];
				//���Կ� ���� �ð��� �ؽ�Ʈ�� �ϼ��Ѵ�.
				TCHAR szTime[128];
				TCHAR szBuffer0[128];
				SYSTEMTIME stTimeInfo;
				GetLocalTime(&stTimeInfo);
				StringCchPrintf( szTime,  sizeof(szTime) ,TEXT("%d:%d:%d:%d:%d"),stTimeInfo.wMonth,stTimeInfo.wDay,stTimeInfo.wHour,stTimeInfo.wMinute,stTimeInfo.wSecond);
				StringCchPrintf(szBuffer0, sizeof(szBuffer0),"%s",&szTime) ;
				StringCchPrintf( fileNameBuffer , sizeof(fileNameBuffer) , TEXT("Config\\PVPLog_%04d%02d%02d.log") 
					, stTimeInfo.wYear , stTimeInfo.wMonth , stTimeInfo.wDay );
				pclClient->pclLog->FilePrint(TEXT(fileNameBuffer), 
					TEXT(" PVPEND_LOGOUT \tWINOPERSON : \t[%s]\t \t LOSEPERSON2 : \t[%s]\t Time"),
					pclWinnerChar->GetName(),pclWarpChar->GetName(),szBuffer0 );
			}
		}
	}

	//---------------------------------
	// �������� ����Ʈ ����. 
	//---------------------------------
	pclCI->clQuestInfo.SaveGoingQuestToDB(this);

	//---------------------------------
	// ��ȥ �ý��� : ���� ����
	//---------------------------------
	if ( pclServer->IsCountrySwitch(Switch_WeddingSystem) )
	{
		// ����ڿ� ���� �ʻ� �־��� ��� ����ڿ� �ֺ�ĳ���� ������ �����Ѵ�
		if ( (TRUE == IsMarried()) && (MARRIAGE_MATE_CONNECT_MODE_NONE < pclMarriage->GetConnectMode()) )
		{
			SI32			siMateCharID	= pclServer->pclCM->GetIDFromPersonID( pclMarriage->GetMatePersonID() );
			cltCharServer*	pclMateChar		= pclServer->pclCM->GetCharServer( siMateCharID );
			if ( (pclMateChar) && (pclMateChar->IsMarried()) && (pclMateChar->pclMarriage->GetMatePersonID() == pclCI->GetPersonID()) )
			{
				// �켱 ������ ĳ���� ����
				pclMateChar->pclMarriage->SetConnectMode( MARRIAGE_MATE_CONNECT_MODE_NONE );

				// ����� �ֺ� ĳ���Ϳ��� �ٲ� ������ ������
				cltCharChangeStatusInfo			clChangeInfo( CHANGETYPE_DISPLAYLOVELEVEL, false, 0 );
				cltGameMsgResponse_StatusInfo	clInfo( pclMateChar->GetCharUnique(), &clChangeInfo );

				cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_CHANGESTATUSINFO, sizeof(clInfo), (BYTE*)&clInfo );
				pclMateChar->SendNeighbourMsg( &clStatusInfoMsg, false );

				// ����ڿ��Դ� ���� ������ �������� ���� ������
				pclServer->SendServerResponseMsg( 0, SRVAL_SET_MARRY_WITHSAMEMAP, MARRIAGE_MATE_CONNECT_MODE_NONE, pclMateChar->pclMarriage->GetLoveLevel(), pclMateChar->GetCharUnique() );

			}
		}
	}
	// ���� ���� �ϸ鼭 Ŀ��Ʈ �� ����鿡�� �ڽ��� ������ �����ش�.
	if ( pclClient->IsCountrySwitch(Switch_We_Are_Family) )
	{
		if(pclFamily != NULL )
		{
			if (  pclFamily->GetParent_1() > 0 )	// ���� �θ��ɸ��Ϳ� ���� ������ ���� �� ������ �ִٴ� ���̴�.
			{
				// ���� �����ٰ� �� �˷���� �Ѵ�.
				SI32 siMyPersonID = pclCI->GetPersonID()	;
				// ���� ã�´�
				// �� ���� ��� Ȯ��
				SI32 siMyClass = pclFamily->GetMyClass_InFamily()	;

				if( siMyClass == MYCLASS_IN_FAMILY_PARANTS)
				{
					for (  SI32 siSlot = 0 ; siSlot < MAX_PARENT_NUM; siSlot++) 
					{
						SI32 siPersonID = pclFamily->GetParent_PersonId( siSlot )	;
						if ( siPersonID == siMyPersonID )	// ���� .
						{
							pclFamily->ConnectOut(siMyPersonID, MYCLASS_IN_FAMILY_PARANTS)	;//���� ������Ʈ
						}

					}
				}
				else if( siMyClass == MYCLASS_IN_FAMILY_CHILDREN)
				{
					for (  SI32 siSlot = 0 ; siSlot <MAX_CHILDREN_NUM; siSlot++) 
					{
						SI32 siPersonID = pclFamily->GetChildPersonID( siSlot )	;
						if ( siPersonID == siMyPersonID )	// ���� .
						{
							pclFamily->ConnectOut(siMyPersonID, MYCLASS_IN_FAMILY_CHILDREN)	;//���� ������Ʈ
						}

					}
				}

				CFamily FamilyData ;
				FamilyData.Init()	;
				FamilyData.Set( pclFamily );

				//������ �α׾ƿ� ������ �����ش�.
				for ( SI32 siSlot = 0 ; siSlot < MAX_PARENT_NUM ; siSlot++ ) 
				{
					SI32 siFamilyPersonID = FamilyData.GetParent_PersonId( siSlot )	;
					SI32 siFamilyId = pclCM->GetIDFromPersonID( siFamilyPersonID )	;
					if ( pclCM->IsValidID( siFamilyId ) == false )	continue ;	//���̵��� ��ȿ�� �˻�.
					cltCharServer* pclChar = pclCM->GetCharServer( siFamilyId )	;
					FamilyData.SetMyClass_InFamily( MYCLASS_IN_FAMILY_PARANTS )	;
					pclChar->pclFamily->Set(&FamilyData)	;
					// Ŭ���̾�Ʈ�� �ش� �ɸ����� ���� ������Ʈ
					cltGameResponse_UpdateFamily clInfo( &FamilyData ,pclChar->pclMarriage);
					cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clInfo), (BYTE*)&clInfo );
					pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
				}
				for ( SI32 siSlot = 0 ; siSlot < MAX_CHILDREN_NUM ; siSlot++ ) 
				{
					SI32 siFamilyPersonID = FamilyData.GetChildPersonID( siSlot)	;
					SI32 siFamilyId = pclCM->GetIDFromPersonID( siFamilyPersonID )	;
					if ( pclCM->IsValidID( siFamilyId ) == false )	continue ;	//���̵��� ��ȿ�� �˻�.
					cltCharServer* pclChar = pclCM->GetCharServer( siFamilyId )	;
					FamilyData.SetMyClass_InFamily( MYCLASS_IN_FAMILY_CHILDREN )	;
					pclChar->pclFamily->Set(&FamilyData)	;

					// Ŭ���̾�Ʈ�� �ش� �ɸ����� ���� ������Ʈ
					cltGameResponse_UpdateFamily clInfo( &FamilyData ,pclChar->pclMarriage);
					cltMsg clStatusInfoMsg( GAMEMSG_RESPONSE_FAMILY_UPDATE, sizeof(clInfo), (BYTE*)&clInfo );
					pclChar->SendNetMsg( (sPacketHeader*)&clStatusInfoMsg);
				}
			}

		}
	}	
	//---------------------------------
	// �� �������� ������ �����Ѵ�. 
	//---------------------------------
	DeleteInCell();


	// �������� ĳ������ ������ DB�� �����Ѵ�. 
	SI32 idnum	= pclCI->clBI.siIDNum;


	// ���ΰ� ĳ���Ͱ� �ƴ� ���¿����� �������� �ʴ´�. 
	// ���ΰ� ĳ���Ͱ� �ƴ� �� �ִ�. ��ó�� Person�� ����� �Ǹ� ���������� �����Ǵµ� ���⼭ 
	// �ٸ� ���� ĳ���ͷ� �缳������ �ʰ� ������ ������ ��� ���������� ������ �ȴ�. 
	if(idnum && bReadySwitch)		
	{
		//---------------------------------
		// ���� ������ ���� 
		//---------------------------------
		sDBRequest_GuiltyInfoRecord clMsg(GetID(), pclCI->GetPersonID(), &pclCI->clGuiltyInfo);
		pclClient->SendDBMsg(DBSELECT_BASE,  (sPacketHeader *)&clMsg );

		//------------------------
		//DB�� �ǰ� ������ ������.
		//-------------------------
		{
			sDBRequest_SetHealth clMsg(pclCI->GetPersonID(), &pclCI->clHealthInfo);
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);
		}

		//-------------------------
		// ���� ������ �����Ѵ�. 
		//-------------------------
		SendDBLevelInfo(0, 0);

		//------------------------
		// ��������� �����Ѵ�.
		//------------------------
		SendDBSkillExp();

		//---------------------------------
		// ������ ���� ����.  - �������� �������� ������ �ʴ´�.
		//---------------------------------
		SendDBWasteDurability();
		SendDBSummonWasteDurability();

		//KHY - 0227 - ����ü (ũ����Ż) ���� �߰�. 
		SendDBCrystalDurability();
		//-------------------------------------
		// ���� �ð��� ���Ѵ�. 
		//-------------------------------------
		CalcPlayTime();

		//-------------------------------------
		// ���Ե� ������ �ִٸ� �ð��� DB�� ������
		//-------------------------------------
		if(pclCI->clBI.siHomeVillage > 0 && pclCI->clBI.siHomeVillage <= CUR_VILLAGE_LIMIT)
		{
			sDBRequest_VillageJoinPlayTime clMsg(pclCI->GetPersonID(), pclCI->clBI.clVillageScoreInfo.siPlayTime, pclCI->clBI.siHomeVillage);
			pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

			//if(pclClient->IsWhereServiceArea(ConstSwitch_NewLog)) 
			//{
			//	// LEEKH MODIFY 2007.10.24
			//	// [�α��߰� : Ȳ���� 2007. 10. 9] // ������ ���� ���� �� �÷��� �ð�.
			//	// param1 == �÷��� �ð�.
			//	cltServer * pclServer = (cltServer*)pclClient;
			//	pclServer->SendLog_WriteLogToDB(LOGCOMMAND_TYPE_VILLAGE, LOGCOMMAND_INDEX_VILLAGE_JOIN_PLAY_TIME, 
			//									0, this, NULL, 0, 0, 0, NULL, 0, 0, 0, pclCI->clBI.siHomeVillage, 0, 0, 
			//									pclCI->clBI.clVillageScoreInfo.siPlayTime, 0, 0, 0, 0, NULL, NULL);
			//}
		}


		//---------------------------------
		// ü��, ������, ��ġ ���� ����. 
		//---------------------------------
		{
			SendDB_PosLifeMana();

			// ��ȿ���� ���� �����̸�, 
			if(pclClient->pclMapManager->IsValidArea(pclCI->clBI.clPos.siMapIndex, pclCI->clBI.clPos.GetX(), pclCI->clBI.clPos.GetY()) == false)
			{
				pclClient->pclLog->FilePrint(TEXT("Config\\ErrorLog.log"), TEXT("InvalidArea:Close() map:%d x:%d y:%d life:%d"), 
					pclCI->clBI.clPos.siMapIndex, pclCI->clBI.clPos.GetX(), pclCI->clBI.clPos.GetY(), GetLife());

			}
		}
		//-----------------------------------
		// ������ ���� ������ ����
		//---------------------------------
		// �������� ���Ե� �ý����� �ð��� ���� �����Ѵ�.

		// ������ �ý��� ó�� - by LEEKH 2008.01.04
		cltTimeMeterRateInfo* pclTimeMeterRateInfo = &pclCI->clBI.clTimeMeterRateInfo;

		for( SI32 i=0; i < MAX_TIMEMETERRATE; i++ )
		{
			cltTimeMeterRate*	pclTimeMeterRate = &pclTimeMeterRateInfo->m_clTimeMeterRate[i];
			if( pclTimeMeterRate->IsUse() )
			{
				if( pclTimeMeterRate->siUseTimeSec > 0)
				{
					// DB �� ��뷮�� �����Ѵ�.
					sDBRequest_TimeMeterRate_UseTime clMsg(GetCharID(), pclCI->GetPersonID(), pclTimeMeterRate->siTimeMeterRateType, pclTimeMeterRate->siUseTimeSec, false );
					pclServer->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg);

					switch( pclTimeMeterRate->siTimeMeterRateType )
					{
					case TIMEMETERRATE_TRANSFORM:
						{
							if( pclTimeMeterRate->siUseTimeSec > 0)
							{
								if( clTransFormInfo.bTransFormMode )
								{
									// DB�� ���� �ð��� �߰����ش�.
									sDBRequest_Transform_AddTime clMsg1(GetCharID(), pclCI->GetPersonID(), clTransFormInfo.uiNowTransFormKind, pclTimeMeterRate->siUseTimeSec );
									pclServer->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&clMsg1);
								}
							}
						}
						break;
					}
				}
			}
		}

		//------------------------------------
		// �α� �ƿ� ����� �����. 
		//------------------------------------
		// bnormalquitswitch	: ���� �����ߴ��� ����, 
		sDBRequest_LogOutRecord cllogout(pclCI->GetPersonID(), GetSession()->GetIP(), bnormalquitswitch );
		pclClient->SendDBMsg(DBSELECT_BASE, (sPacketHeader *)&cllogout);

	}

	//------------------------------------------
	// HASH TABLE���� �����Ѵ�.
	//------------------------------------------
	// HashTable�� PERSON �̸��� �������� �����Ѵ�.
	((cltServer*)pclClient)->pclHashTable->Remove((TCHAR *)GetName());

	// HashTable�� PERSON ID�� �������� �����Ѵ�.
	((cltServer*)pclClient)->pclPersonHashTable->Remove(pclCI->GetPersonID());
}
void cltCharServer::Close()
{
	cltServer *pclServer = (cltServer *)pclClient;

	// PC�� �α׾ƿ�
	if( pclCI->clBI.bPCRoom ) {

		pclCI->clBI.bPCRoom = false;
		pclServer->siPCRoomCharNum--;
	}


	//---------------------------------
	// ���� �����ΰ�, ������ �����ΰ�. 
	//---------------------------------
	bool bnormalquitswitch = true;
	if(bHopeToQuitSwitch || ((pclKI != NULL && pclKI->IsAtb(ATB_HERO) == false)))
	{
		pclServer->pclStatisticsManager->clDailyStatistics.siNormalQuitUserNumber ++;
		bnormalquitswitch = true;
	}
	else
	{
		pclServer->pclStatisticsManager->clDailyStatistics.siAbnormalQuitUserNumber ++;
		bnormalquitswitch = false;

		// apex-
#ifdef _CHINA
		if( 0 < GetID() )
			NoticeApexProxy_UserLogout( GetID(), (char*)pclCI->clBI.szAccountID );
		bApexLogin = false;
#endif
		// -apex
	}

	// ĳ���Ͱ� ���� �׾��µ� �����ϸ� ����� ������ ������.
	if( ORDER_DIE ==  GetCurrentOrder() || 
		ORDER_DEAD == GetCurrentOrder() ||
		ORDER_REVIVE == GetCurrentOrder() )
	{
		SI32 villageunique = 0;
		if(siKilledMode == KILLEDMODE_VILLAGEWAR)	{	villageunique = pclCI->clBI.siHomeVillage;		}
		else										{	villageunique = GetCurrentVillageUnique();		}

		if( villageunique <= 0 )
		{
			// ���� ��ġ�� ������ �θ� ������ ã�´�. 
			SI32 parentmap = pclClient->pclMapManager->GetParentMap( GetMapIndex() );
			if(parentmap == 0)parentmap = GetMapIndex();

			//cyj ������ġ�� �������� ��� parentmap�� villageunique ��
			if(pclClient->pclMapManager->IsHuntMap( GetMapIndex() ) == true)	{		villageunique = parentmap;		}
			else
			{
				// �θ� ������ �߿����� ��ǥ�� ã�´�. 
				SI32 x, y;
				if( parentmap != 0 &&
					pclClient->pclMapManager->pclGateManager->FindMainFieldPos(parentmap, &x, &y) == true )
				{
				}
				else
				{
					x	= pclCI->clBI.clLastMainMapPos.siX;
					y	= pclCI->clBI.clLastMainMapPos.siY;

				}
				// ����� ������ ã�´�.
				villageunique = pclClient->pclVillageManager->FindNearVillage(x, y);
			}
		}

		SI32 centerx = 0;
		SI32 centery = 0;

		if( villageunique <= 0 )
		{
			villageunique = MAPINDEX_PALACE;
			centerx = 27;
			centery = 87;
		}
		else
		{
			SI32 maptypeunique = pclClient->pclMapManager->pclMapArray[ villageunique ]->siMapUnique;
			centerx = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterX;
			centery = pclClient->pclMapManager->pclMapTypeManager->pclMapTypeInfo[maptypeunique]->siCenterY;
		}

		pclCI->clBI.SetMapIndex(villageunique);
		SetX(centerx);
		SetY(centerx);

		// ���������� ��������� �ι� ó������ �ʰ� �������� ä���(Ŭ���̾�Ʈ�δ� ������ �ʴ´�) -KKM-
		if( bDeathPenaltySwitch )
		{
			pclCI->clIP.SetLife( clPB.GetMaxLife() );
			SendDBLevelInfo(0, 0);
		}

		SendDB_PosLifeMana();
	}

	// Special ���� PC�� ������ DB Log �� �����
#ifndef _DEBUG	// ������Ҷ� �����ڵ� �Ű�Ⱦ�
	if( pclClient->siServiceArea == ConstServiceArea_Korea )
#endif

	{
		SI32 playMinute = dwPlayingTime / 60;

		sDBRequest_PCRoom_Log clMsg( pclClient->siServerUnique , "K_GOONZU" , pclCI->clBI.szAccountID , 
			(TCHAR*)pclCI->clBI.GetName() , &sTimeStartTime , &((cltServer*)pclClient)->sTime , GetSession()->GetIP() , pclCI->clBI.szCRMcode ,  siLoginedLevel ,
			pclCI->clIP.GetLevel() , playMinute  ,pclCI->clIP.GetMoney() , pclCI->clBI.sPCroomGrade );

		pclClient->SendDBMsg(DBSELECT_SUB,  (sPacketHeader *)&clMsg );

	}

	// [����] ���� �̺�Ʈ �δ����� ����� ����
	if ( 0 < siServerEvent_IndunScore )
	{
		sDBRequest_Server_Event_Server_Person_Score_Set clScore( 0, pclCI->GetPersonID(), siServerEvent_IndunScore );
		pclClient->SendDBMsg( DBSELECT_BASE, (sPacketHeader*)&clScore );
	}

#ifdef USE_GAME_GUARD_SERVER
	CCSAuthClose();
#endif


	// Auth Server�� ���� �α� �ƿ� ������ ������ 
	if( siIDNUM > 0 && siAccountStatus >= 0 )
	{
#ifdef _CHINA
		sRequest_GameLogout	sendMsg;

		sendMsg.packetHeader.usCmd = AUTHMSG_REQUEST_GAMELOGOUT;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );

		sendMsg.uiIDNum =  siIDNUM;

		memcpy( sendMsg.szAccountID, pclCI->clBI.szAccountID, 20 );
		sendMsg.szAccountID[ 19 ] = NULL;
		memcpy( sendMsg.userServerID, pclClient->szServerID, 20 );
		sendMsg.userServerID[ 19 ] = NULL;
		memcpy( sendMsg.szPlayingCharNmae, GetName(), 20 );
		sendMsg.szPlayingCharNmae[ 19 ] = NULL;

		sendMsg.siLevel = pclCI->clIP.GetLevel();

		ITZSession *pSession = pclServer->pclFrontAuthProcess->GetAuthSession();

		if( pSession )
		{
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
		}

		// Gamania
		if( siAccountStatus == 2 )
		{		
			pclServer->siGamaniaCharNum--;
		}
		else if( siAccountStatus == 4 )
		{
			pclServer->siHangameCharNum--;
		}
		else if( siAccountStatus == 8 )
		{
			pclServer->siGamePotCharNum--;
		}
		else if( siAccountStatus == 0 )
		{
			pclServer->siNDECharNum--;
		}
		/*
		else if( siAccountStatus == 6 )
		{
		pclServer->siNetmarbleCharNum--;
		}
		*/
		siAccountStatus = -2;
		siIDNUM = 0;
#else
		sRequest_GameLogout	sendMsg;

		sendMsg.packetHeader.usCmd = AUTHMSG_REQUEST_GAMELOGOUT;
		sendMsg.packetHeader.usSize = sizeof( sendMsg );

		sendMsg.uiIDNum =  siIDNUM;

		memcpy( sendMsg.szAccountID, pclCI->clBI.szAccountID, 20 );
		sendMsg.szAccountID[ 19 ] = NULL;

		ITZSession *pSession = pclServer->pclFrontAuthProcess->GetAuthSession();


		//KHY - 0218 - �߱� NHN �Ƿε� ��� ����
		SI32 siplayTimeMinute = dwPlayingTime / 60;

		if(bTiredSystem) //�Ƿε� �ý����� �����Ǿ� �ִ� ��츸 ����?
		{
			pclClient->pclLog->FilePrint(TEXT("Config\\TiredSystemLog.log"), TEXT(" pclCI->clBI.szAccountIDb: %s - TiredSystem : %d  - siPlayTimeMinute:%d"), 
				pclCI->clBI.szAccountID , bTiredSystem , siplayTimeMinute);

			sendMsg.siPlayTimeMinute = siplayTimeMinute;
		}
		else
			sendMsg.siPlayTimeMinute = -1; // �ϴ� �Ƿε������� ���Ե�����, ��Ÿ �ٸ� �������� ���� �ɶ��� �̺κ�			

		if( pSession )
		{
			pSession->GetSendBuffer()->Write( (BYTE*)&sendMsg );
		}

		if( siAccountStatus == 4 ) // �Ѱ���
		{
			pclServer->siHangameCharNum--;
		}
		else if( siAccountStatus == 2 ) // �����Ͼ�
		{		
			pclServer->siGamaniaCharNum--;
		}
		else if( siAccountStatus == 8 ) // ������
		{
			pclServer->siGamePotCharNum--;
		}
		else if( siAccountStatus == 0 ) // NDE
		{
			pclServer->siNDECharNum--;
#ifdef _ENGLISH		
			pclServer->siGlobalNum--;
#endif
		}
		else if( siAccountStatus == 10 ) // NEXON
		{
			pclServer->siNEXONCharNum--;
		}

#ifdef _ENGLISH		
		else if( siAccountStatus == 100 ) // NDE
		{
			pclServer->siNHNNum--;
		}
		else if( siAccountStatus == 200 ) // NEXON
		{
			pclServer->siGlobalNum--;
		}
#endif
		/*
		else if( siAccountStatus == 6 )
		{
		pclServer->siNetmarbleCharNum--;
		}
		*/

		siAccountStatus = -2;
		siIDNUM = 0;
#endif
	}

	CloseAction(pclServer, bnormalquitswitch);
	//------------------------------------------
	// ����� ĳ���͸� �����Ѵ�. 
	//------------------------------------------
	// ������ ĳ���� �ֺ��� �̿� �����͸� �����Ѵ�. 
	// �̿�ĳ������ �̿� ��Ͽ��� �ı��Ǵ� ĳ������ ������ �����.
	pclCM->DeleteChar(GetID() );

	ITZPerson::Close();
}
