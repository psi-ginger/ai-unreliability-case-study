//---------------------------------
// 2003/8/4 ���°�
//---------------------------------

#include "..\Common\CommonHeader.h"
#include "..\Server\Server.h"

#include "..\Server\TraceBoot.h"
#include "..\Server\TraceBoot.h"

#include "..\Common\Map\Map.h"
#include "..\Common\Letter\Letter.h"
#include "Char\CharManager\CharManager.h"

//-----------------------------------------
// Server
//-----------------------------------------
#include "Statistics/Statistics.h"

#include "..\..\DBManager\GameDBManager_World\DBMsg.h"

void cltServer::InitNetwork()
{
	pclGameProtocol = new cltGameProtocol(this);
	pclGameProtocol->CreateThread();

	// AgentProcess ����. 
	pclAgentProcess		= new AgentProcess();
	pclAgentProcess->Init(TEXT("127.0.0.1"), 6400);

}

// Agent���� ���� ������ ���� ��������� ������ �����Ѵ�.
void cltServer::SetOtherServer(TCHAR* frontip, SI32 frontport, TCHAR* authip, SI32 authport, TCHAR* dbip, SI32 dbport, SI32 clientlistenport)
{
	//FrontAuth ����. 
	pclFrontAuthProcess	= new FrontAuthProcess();
	pclFrontAuthProcess->Init(frontip, frontport, authip, authport);

	//Bill ����

	pclBillProcess = new BillProcess();
	pclBillProcess->Init(TEXT("127.0.0.1"),3800);

	// Ŭ���̾�Ʈ ���� ����
	//pclGameProtocol = new cltGameProtocol(this);
	pclGameProtocol->Init(clientlistenport);

	// GameDB ����. 
	pclGameDBProcess	= new GameDBProcess();
	pclGameDBProcess->Init(dbip, dbport );

	pclGameDBProcess2	= new GameDBProcess2();
	pclGameDBProcess2->Init(dbip, dbport - 1 );

	pclLogDBProcess		= new LogDBProcess();
	pclLogDBProcess->Init(dbip, dbport - 2 );

	pclNTradeServerProcess = new NTradeServerProcess();
	pclNTradeServerProcess->Init();

}

// ��� �غ� �Ǿ� �ִٸ� Ŭ���̾�Ʈ�� ������ �ޱ� �����Ѵ�. 
bool cltServer::StartNetwork()
{
	TraceBoot("StartNetwork enter bSwitch=%d agent=%d front=%d auth=%d",
		bServerNetoworkStartSwitch,
		pclAgentProcess->GetAgentSession()->GetState(),
		pclFrontAuthProcess->GetFrontSession()->GetState(),
		pclFrontAuthProcess->GetAuthSession()->GetState());

	if(bServerNetoworkStartSwitch == true)return true;

	if(pclAgentProcess == NULL)return false;
	if(pclAgentProcess->GetAgentSession() == NULL)return false;

	if(pclFrontAuthProcess == NULL)return false;
	if(pclFrontAuthProcess->GetFrontSession() == NULL)return false;

	if(pclGameDBProcess == NULL)return false;

	if(pclAgentProcess->GetAgentSession()->GetState()			==	SESSION_STATE_ESTABLISHED
		&& pclFrontAuthProcess->GetFrontSession()->GetState()	==	SESSION_STATE_ESTABLISHED
		&& pclFrontAuthProcess->GetAuthSession()->GetState()	==	SESSION_STATE_ESTABLISHED
		&& pclGameDBProcess->IsEstablished()					==	TRUE 
		)
	{

		// Ŭ���̾�Ʈ�� ������ ����ϱ� �����Ѵ�.
		bServerNetoworkStartSwitch = true;

		// ���ʷ� DBManager�� ����� ���̶� 
		// DB�� ������ �ϰ������� ��û�Ѵ�.  - ù��° ��û
		// �ι�° ��û�� ù��° ����� ��� ���� �ڿ� �� ��û�Ѵ�. - by LEEKH 2008-07-18
		TraceBoot("StartNetwork ALL ESTABLISHED -> calling InitializeDB1()");
		InitializeDB1();
		TraceBoot("StartNetwork InitializeDB1 returned");

		return true;

	}


	return false;

}

void cltServer::NetworkOperation()
{

	// ���� ���������� Agent�� FrontAuth�� ���� ���ư���. 

	Sleep( 10 );

	// FrontAuth
	if(pclFrontAuthProcess)
	{
		pclFrontAuthProcess->Run();
	}
}

void cltServer::GameDBAction()
{
	// DB�� ������ ������ ���� ������� ���ư���. 

	if ( pclNTradeServerProcess )
	{
		pclNTradeServerProcess->Run();
	}

	if(pclGameDBProcess)
	{
		pclGameDBProcess->Run();
	}

	if(pclGameDBProcess2)
	{
		pclGameDBProcess2->Run();
	}

	if(pclLogDBProcess)
	{
		pclLogDBProcess->Run();
	}

	// Agent
	if(pclAgentProcess)
	{
		pclAgentProcess->Run();
	}

	// Bill 

	if(pclBillProcess)
	{
		pclBillProcess->Run();
	}
}

bool cltServer::SendDBMsg(SI32 siselect, sPacketHeader *pPacket)
{
	if(pPacket == NULL)return false;
	if(pclGameDBProcess == NULL)return false;
	if( pclStatisticsManager == NULL ) return false ;

	if(pPacket->usSize > 30000)
	{
		MsgBox(TEXT("bool cltServer::SendDBMsg(sPacketHeader *pPacket)"), TEXT("Overflow:%d"), pPacket->usSize);
		return false;
	}

	// DB�� ������ �޽����� �����Ѵ�. 
	pclStatisticsManager->clDailyStatistics.siSendToDBMsgNumber++;

	//NStatistics_Add("DBNetMessage", pPacket->usCmd, "SendCount", 1);
	//NStatistics_Add("DBNetMessage", pPacket->usCmd, "SendSize", pPacket->usSize);

	return pclGameDBProcess->SendMsg(siselect, pPacket);
}

// Ư�� ����ڿ��� �˸� �޽����� ������. 
bool cltServer::SendNoticePersonMsg(SI16 charunique, TCHAR* msg, TCHAR* header)
{
	if(pclCM->IsValidID(charunique) == false)return false;

	if(header == NULL)
	{
		header = GetTxtFromMgr(5482);
	}
	cltGameMsgResponse_Notice clNotice(header, msg,true);
	cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

	cltCharServer* pclchar = (cltCharServer*)pclCM->CR[charunique];
	pclchar->SendNetMsg((sPacketHeader*)&clMsg);

	return true;
}

// ��ü �������� �˸� �޽����� ������. 
bool cltServer::SendNoticeMsg(SI16 villageunique, SI16 villagerunique, SI16 charunique, TCHAR* msg)
{
	// ��ü ����. 
	if(villageunique == -1)
	{

		// ���� �ֹε� ���� ����. 
		if(villagerunique > 0)
		{
			TCHAR* pText = GetTxtFromMgr(1218);
			cltGameMsgResponse_Notice clNotice(pText, msg,true);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclCM->SendAllPersonHomeMsg((sPacketHeader*)&clMsg, villagerunique);
		}
		else if(pclCM->IsValidID(charunique))
		{
			TCHAR* pText = GetTxtFromMgr(5614);
			cltGameMsgResponse_Notice clNotice(pText, msg,true);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclCM->CR[charunique]->SendNetMsg((sPacketHeader*)&clMsg);
		}
		else
		{
			TCHAR* pText = GetTxtFromMgr(5482); 

			//KHY - 0129 - �ϴ� ä��â�� ���� ��¿��θ� �����Ѵ�.
			cltGameMsgResponse_Notice clNotice(pText, msg,false);
			cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

			pclCM->SendAllPersonMsg((sPacketHeader*)&clMsg);
		}
	}
	// ���� ����. 
	else if(villageunique > 0)
	{
		TCHAR* pText = GetTxtFromMgr(5483);  
		cltGameMsgResponse_Notice clNotice(pText, msg,true);
		cltMsg clMsg(	GAMEMSG_RESPONSE_NOTICE, clNotice.siDataSize, (BYTE*)&clNotice);

		pclCM->SendAllPersonInVillageMsg((sPacketHeader*)&clMsg, villageunique);
	}

	return true;
}

// Ŭ���̾�Ʈ���� ������ �����ش�. 
SI32 cltServer::SendLetterMsg(SI32 personid, cltLetterHeader* pclheader)
{
	cltLetterUnit clletterunit(pclheader);

	SI32 letterid = pclCM->GetIDFromPersonID(personid);
	// ���ӿ� �������̸� Ŭ���̾�Ʈ�� �뺸�Ѵ�. 
	if(letterid)
	{
		pclCM->CR[letterid]->SendLetterMsg(&clletterunit);

	}
	// ���ӿ� ���������� �ʴٸ� DB�Ŵ������� ������. 
	else
	{
		sDBRequest_PushLetter clMsg(personid, &clletterunit);
		pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg);
	}

	return letterid;

}


// ������ ������� ������ �����ش�. 
SI32 cltServer::SendAccountLetterMsg(TCHAR* AccountID, cltLetterHeader* pclheader)
{
	cltAccountLetterUnit clletterunit(pclheader);

	sDBRequest_PushAccountLetter clMsg(AccountID, &clletterunit);
	pclGameDBProcess->SendMsg( DBSELECT_LETTER, (sPacketHeader *)&clMsg);

	return 0;

}

