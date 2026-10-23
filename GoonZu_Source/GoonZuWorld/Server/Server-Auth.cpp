//---------------------------------
// 2003/7/31 ���°�
//---------------------------------

#include "Char\CharManager\CharManager.h"

#include "..\Server\Server.h"

#include "..\Common\Map\Map.h"

#include "MsgType-System.h"

#include "../../Server/AuthServer/AuthMsg.h"
#include "..\..\DBManager\GameDBManager_World\DBMsg-Person.h"
#include "TraceBoot.h"

#include "MsgRval-Define.h"

#include "../GMSystem/GMProtocol.h"
#include "../GMSystem/GMMsg.h"

void cltServer::DoMsg_AUTHMSG_RESPONSE_GAMEAUTH(sPacketHeader* pPacket)
{
//----------------------------------------------------------------
// ������ ������ ��
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	sResponse_GameAuth* pclMsg = (sResponse_GameAuth*)pPacket;

	// IDNum�� Ȯ���ؼ�  Person List�� ��û�Ѵ�.
	SI32 siIDNum	= pclMsg->uiIDNum;
	SI32 id			= pclMsg->usCharID;
	SI32 sex		= pclMsg->siSex;
	SI32 tempid;

	TraceBoot("CHARLIST GAMEMAUTH arrived usCharID=%d uiIDNum=%d sex=%d validID=%d",
		id, siIDNum, sex, (int)pclCM->IsValidID(id));
	if(siIDNum == 0)
	{
		TraceBoot("CHARLIST ABORT: uiIDNum==0");
	}

	if(pclCM->IsValidID(id) == false)		return;
				
	// ������ �� ����. (�������� ���̵� �н������.. )
	// �������� ���� ĳ���Ͷ�� �����Ѵ�. 
	if(siIDNum == 0)return;
	
	// ������ ���ӿ� �����ϰ� �ִ� ����� �߿��� ���� IDNum�� ����ϴ� ����� �ִ��� �˻��Ѵ�. ���� IDNum�� 
	//����Ѵٴ� ���� �ߺ� ���̵� ����̹Ƿ� �����Ѵ�. 
	SI32 index = 0;
	while(tempid = pclCM->pclIDManager->GetCharOrder(index))
	{
		index++;
		if(pclCM->CR[tempid]->pclCI->clBI.siIDNum == siIDNum)
		{
			// �α׷� �����. 
			pclLog->FilePrint(TEXT("Config\\LoginLog.log"), TEXT("�ߺ��������� ���� ���� [%s]"), pclCM->CR[tempid]->GetName());

			// �׾��µ� ���� ���ϰ� ��Ƽ�°� ���� �����!1!!!!���׸� �̿��Ϸ���.
			// ������ ��ó ������ ����� �̵� ��Ű�� �������.
			if(pclCM->IsAlive(tempid)	== false )	// ����ֳ�?
			{
				// ��� ���� �ʾ�. �׷��� ������ ���� �ߴ�. 
				// �ϴ� �츰��.
				cltCharServer* pDieCharServer = pclCM->GetCharServer(tempid) ;
				if(pDieCharServer != NULL)
				{
					pclCM->CR[tempid]->Revive()	;
					//�׸��� ���� ���Ѿ���....�̷��� ���� �ȳ�Ÿ����.
				}
			}


			// �ٸ� ����� �������� ������ ������ �˷��ش�. 
			//cltGameMsgResponse_Notice clNotice(TEXT("�˸�"), TEXT("�ٸ� �������� �������� ���� ������ ������ �����մϴ�."));
			//cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);
			//pclCM->CR[tempid]->SendNetMsg((sPacketHeader*)&clMsg);
			//cltMsg clMsg(GAMEMSG_RESPONSE_ANOTHERAUTH,0,NULL);
			//pclCM->CR[tempid]->SendNetMsg((sPacketHeader*)&clMsg);

			// �ű� ������� IP�� Ȯ���Ѵ�.
			//TCHAR * IP = pclCM->CR[id]->GetSession()->GetIP() ;

			TCHAR IP[64] = TEXT("") ;
			StringCchCopy(IP, 64,pclCM->CR[id]->GetSession()->GetIP());
			if ( !_tcscmp(IP , TEXT("222.107.176.36")) )
			{
				StringCchCopy(IP, 64 ,TEXT("���ڰ���!!!"));
			}

			// �ð��� Ȯ���Ѵ�.
			SYSTEMTIME stTime;
			GetLocalTime(&stTime);

			cltCharServer* pcloldchar = (cltCharServer*)(pclCM->CR[tempid]);

			cltGameMsgResponse_AnotherAuth clNotice(IP,stTime, FALSE);
			cltMsg clMsg(	GAMEMSG_RESPONSE_ANOTHERAUTH, sizeof(clNotice), (BYTE*)&clNotice);
			pcloldchar->SendNetMsg((sPacketHeader*)&clMsg);

			// ���� ����ڸ� �����Ѵ�.
			pcloldchar->bCloseSocketSwitch		= true;
			pcloldchar->dwCloseSocketTimer		= 5;

			//cyj �ű� ������� ������ �ٷ� ���� �ʰ� �޼����� ������ ���������� ����
			cltCharServer* pclnewchar = (cltCharServer*)(pclCM->CR[id]);
			cltGameMsgResponse_AnotherAuth clNotice2(IP,stTime, TRUE);
			cltMsg clMsg2(	GAMEMSG_RESPONSE_ANOTHERAUTH, sizeof(clNotice), (BYTE*)&clNotice2);
			pclnewchar->SendNetMsg((sPacketHeader*)&clMsg2);
			pclnewchar->bCloseSocketSwitch = true;
			pclnewchar->dwCloseSocketTimer = 5;
			// �ű� ����� ���ӵ� ���´�. 
			//pclCM->CR[id]->GetSession()->CloseSocket();

			return ;
		}
	}

	if (pclClient->siServiceArea == ConstServiceArea_Japan)
	{
		//cyj ������ ������ �ȵ� �����̸�
		if (pclCM->CR[id]->siAccountStatus == -1)
		{
			cltCharServer* pclnewchar = (cltCharServer*)(pclCM->CR[id]);
			cltGameMsgResponse_NotGamePotAuth clNotAuth;
			cltMsg clMsg2(	GAMEMSG_RESPONSE_NOTGAMEPOTAUTH, sizeof(clNotAuth), (BYTE*)&clNotAuth);
			pclnewchar->SendNetMsg((sPacketHeader*)&clMsg2);
			pclnewchar->bCloseSocketSwitch = true;
			pclnewchar->dwCloseSocketTimer = 5;

			return;

		}

	}

	if (pclClient->siServiceArea == ConstServiceArea_Korea)
	{
		// �Ѱ��� ���� üũ
		if (pclMsg->siAccountStatus == -1000)
		{
			cltCharServer* pclnewchar = (cltCharServer*)(pclCM->CR[id]);
			cltGameMsgResponse_CannotLogin_Age clNotAge;
			cltMsg clMsg2(	GAMEMSG_RESPONSE_CANNOTLOGIN_AGE, sizeof(cltGameMsgResponse_CannotLogin_Age), (BYTE*)&clNotAge);
			pclnewchar->SendNetMsg((sPacketHeader*)&clMsg2);
			pclnewchar->bCloseSocketSwitch = true;
			pclnewchar->dwCloseSocketTimer = 10;

			return;
		}
		// �Ѱ��� ���� ��� ���� ��ȯ
		else if(pclMsg->siAccountStatus == -1100)
		{
			cltCharServer* pclnewchar = (cltCharServer*)(pclCM->CR[id]);
			cltGameMsgResponse_CannotLogin_Error clError;
			cltMsg clMsgError(	GAMEMSG_RESPONSE_CANNOTLOGIN_ERROR, sizeof(cltGameMsgResponse_CannotLogin_Error), (BYTE*)&clError);
			pclnewchar->SendNetMsg((sPacketHeader*)&clMsgError);
			pclnewchar->bCloseSocketSwitch = true;
			pclnewchar->dwCloseSocketTimer = 10;

			return;
		}
	}

	// ĳ������ IDNum�� ����Ѵ�. 
	pclCM->CR[id]->pclCI->clBI.siIDNum = siIDNum;
	pclCM->CR[id]->siIDNUM = siIDNum;

	// ���� 
	pclCM->CR[id]->pclCI->clBI.uiSex = sex;
				
	// ��� ������ ������Ʈ �Ѵ�. 
	pclCM->CR[id]->SetGameMasterLevel(pclMsg->siGameMasterLevel);
	pclCM->CR[id]->SetRareItemRate(0,0,0,0);
	pclCM->CR[id]->SetWennyOldMan(false,false);

	//[����] �޸� ���� ���θ� �����Ѵ�.
	pclCM->CR[id]->pclCI->clDormancyInfo.SetDormancyState(pclMsg->siDormancyState);

	// GameDBManager_World���� ��� Person������ �˷��� ���� ��û�Ѵ�. 
	sDBRequest_PersonListInfoGet clMsg(pclMsg->usCharID, pclMsg->uiIDNum);
	TraceBoot("CHARLIST REQ->DB  usCharID=%u uiIDNum=%u charid=%d", (unsigned)pclMsg->usCharID, (unsigned)pclMsg->uiIDNum, id);
	pclGameDBProcess->SendMsg( DBSELECT_LOGIN, (sPacketHeader *)&clMsg);

	if( pclCM->CR[id]->siAccountStatus == -2 )
	{
		// HanGame
		if( pclMsg->siAccountStatus == 4 )
		{
			siHangameCharNum++;
		}

		// Gamania
		else if( pclMsg->siAccountStatus == 2 ) 
		{
			siGamaniaCharNum++;
		}  
		
		// GamePot
		else if( pclMsg->siAccountStatus == 8 )
		{
			siGamePotCharNum++;
		}

		// NDE
		else if( pclMsg->siAccountStatus == 0 )
		{
			siNDECharNum++;
#ifdef _ENGLISH
			siGlobalNum++;
#endif
		}
		// NEXON
		else if( pclMsg->siAccountStatus == 10 )
		{
			siNEXONCharNum++;
		}
#ifdef _ENGLISH
		else if( pclMsg->siAccountStatus == 100 )
		{
			siNHNNum++;
		}
		// NEXON
		else if( pclMsg->siAccountStatus == 200 )
		{
			siGlobalNum++;
		}
#endif

		// Netmarble
		/*if( pclMsg->siAccountStatus == 6 ) 
		{
			//siNetmarbleCharNum++;
		}
		*/
	}

	pclCM->CR[id]->siAccountStatus = pclMsg->siAccountStatus;
	
	cltCharServer* pclnewchar = (cltCharServer*)(pclCM->CR[id]);

	// �Ƿι���-
	if( ConstServiceArea_China == siServiceArea )
	{

		if( pclMsg->siPlayingTime >= 0 )
		{
			pclnewchar->bTiredSystem = true;

			if( pclMsg->siPlayingTime > 60 * 60 * 3 && pclMsg->siPlayingTime <= 60 * 60 * 5)
			{
				// ���� ��ġ�� �ݸ� �޴´�.
				pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_LITTLE_TIRED;
			}
			else if( pclMsg->siPlayingTime > 60 * 60 * 5 )
			{
				// �ƹ��͵� ���� �� �Ѵ�.
				pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_MUCH_TIRED;
			}
			else
			{
				pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_NORMAL;
			}
			pclnewchar->dwPlayingTime = pclMsg->siPlayingTime;

			SendServerResponseMsg( 0, SRVAL_TIRED_SYSTEM_CHAR,  0, 0, pclnewchar->GetID() );

		}
		else if( -1 == pclMsg->siPlayingTime )
		{
			pclnewchar->bTiredSystem = false;
			pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_NORMAL;
			pclnewchar->dwPlayingTime = 0;
		}

		cltMsg clMsg( GAMEMSG_RESPONSE_TIRED_STATUS , (SI32)pclnewchar->uiTiredState );
		pclnewchar->SendNetMsg((sPacketHeader*)&clMsg);

	}
	else if( ConstServiceArea_NHNChina == siServiceArea )
	{

		if( pclMsg->siPlayingTime >= 0 )
		{
			pclnewchar->bTiredSystem = true;

			//KHY - 0218 - �߱� NHN �Ƿε� ��� ���� - ������ ���´�. �ʷ� �ٽ� ��������.
			pclMsg->siPlayingTime = pclMsg->siPlayingTime * 60;

			if( pclMsg->siPlayingTime > 60 * 60 * 3 && pclMsg->siPlayingTime <= 60 * 60 * 5)
			{
				// ���� ��ġ�� �ݸ� �޴´�.
				pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_LITTLE_TIRED;
			}
			else if( pclMsg->siPlayingTime > 60 * 60 * 5 )
			{
				// �ƹ��͵� ���� �� �Ѵ�.
				pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_MUCH_TIRED;
			}
			else
			{
				pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_NORMAL;
			}
			pclnewchar->dwPlayingTime = pclMsg->siPlayingTime;

			// �α׷� �����. 
			pclLog->FilePrint(TEXT("Config\\Tired_Server_RESPONSE_GAMEAUTH.log"), TEXT("GetName - %s , siPlayingTime - %d"), pclCM->CR[id]->GetName() ,pclMsg->siPlayingTime );

			SendServerResponseMsg( 0, SRVAL_TIRED_SYSTEM_CHAR,  0, 0, pclnewchar->GetID() );

		}
		else if( -1 == pclMsg->siPlayingTime )
		{
			pclnewchar->bTiredSystem = false;
			pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_NORMAL;
			pclnewchar->dwPlayingTime = 0;
			pclnewchar->siTiredNotice = 0;
		}

		//cltMsg clMsg( GAMEMSG_RESPONSE_TIRED_STATUS , (SI32)pclnewchar->uiTiredState );
		//pclnewchar->SendNetMsg((sPacketHeader*)&clMsg);

	}
	else
	{
		pclnewchar->bTiredSystem = false;
		pclnewchar->uiTiredState = cltCharCommon::TIRED_STATUS_NORMAL;
		pclnewchar->dwPlayingTime = 0;
	}

	// �Ѱ��ӿ� IDNO - by LEEKH 2007.11.11
	pclnewchar->uiIDNo = pclMsg->uiIDNo;

//----------------------------------------------------------------
// ������ ������ ��
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

void cltServer::DoMsg_AUTHMSG_RESPONSE_HANAUTH_GAMERUNAUTH(sPacketHeader* pPacket)
{
//----------------------------------------------------------------
// ������ ������ ��
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	sResponse_GameRunAuth* pclGameDisconnectMsg = (sResponse_GameRunAuth*)pPacket;

/*	if (pclClient->siServiceArea == ConstServiceArea_Korea)
	{
		// PCK - 2007.09.19 : �Ѱ��� ����üũ (���ӵ��߿��� üũ�� ��.)
		if (pclGameDisconnectMsg->siResult == 0)
		{
			cltCharServer* pclnewchar = (cltCharServer*)(pclCM->CR[pclGameDisconnectMsg->usCharID]);
			cltGameMsgResponse_GameDisconnect_Age clNotAge;
			cltMsg clMsg2(	GAMEMSG_RESPONSE_GAMEDISCONNECT_AGE, sizeof(cltGameMsgResponse_GameDisconnect_Age), (BYTE*)&clNotAge);
			pclnewchar->SendNetMsg((sPacketHeader*)&clMsg2);
			pclnewchar->bCloseSocketSwitch = true;
			pclnewchar->dwCloseSocketTimer = 10;

			return;
		}
	}*/

//----------------------------------------------------------------
// ������ ������ ��
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------
}

void cltServer::DoMsg_AUTHMSG_RESPONSE_GMTOOLAUTH(sPacketHeader* pPacket)
{
//----------------------------------------------------------------
// ������ ������ ��
//----------------------------------------------------------------
#ifdef _SERVER
//----------------------------------------------------------------

	sResponse_GMToolAuth* pclInfo = (sResponse_GMToolAuth*)pPacket;
	if(pclInfo->usCharID < 0)		return;

	cGMClient* pclchar = pclGMProtocol->GetGMClient( pclInfo->usCharID );
	if(pclchar == NULL)				return;

	ITZSession* pSession = pclchar->GetSession();
	if(pSession == NULL)			return;

	sGMResponse_Login sendMsg( 0 );		// �⺻ ����

	if ( pclInfo->siMasterLevel >= BASIC_GAMEMASTER_LEVEL )
	{
		sendMsg.m_iResult = 1;
		
		pclchar->SetAccount( pclInfo->userid, pclInfo->siMasterLevel );
	}
	
	// �޽��� ����
	pclGMProtocol->SendPacket( pSession, (sPacketHeader*)&sendMsg );

//----------------------------------------------------------------
// ������ ������ ��
//----------------------------------------------------------------
#endif //_SERVER
//----------------------------------------------------------------

}
