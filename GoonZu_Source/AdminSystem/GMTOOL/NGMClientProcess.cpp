//=============================================================================
//
//	NLogClientProcess.cpp
//
//	Copyright 2007 HQGame
//
//=============================================================================
#include "./stdafx.h"
#include "./NGMClientProcess.h"
#include "./resource.h"

#include "MainFrm.h"

#include "Char/CharFrame.h"

#include "../../AdminSystem/MasterServer/MasterMsg.h"

#define ServerMessageMap(AAA,BBB) m_pDBMsgPtr[AAA].Func = &NGMClientProcess::BBB

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
NGMClientProcess::NGMClientProcess()
{
	m_pNGMProcessSession	= NULL;

	m_szMasterServerIP[0]	= '\0';
	m_uiMasterServerBasePort= 0;
	m_siReason				= 0;

	m_szLoginID[0]			= '\0';
	m_szLoginPW[0]			= '\0';

	m_pDBMsgPtr = new stDBMsgPtr[ENDMSG];

	m_bSendLogin			= FALSE;
	m_bLoginOK				= FALSE;
	m_bSendReqInit			= FALSE;
	m_bInitOK				= FALSE;


	if(m_pDBMsgPtr == NULL)
		MessageBox(NULL, "함수포인터 할당 실패", NULL, MB_OK);
	else
	{
		// 알 수 없는 메시지가 왔을때 처리할 함수를 할당해 준다.
		for(int i = 0; i < ENDMSG; i++)
			m_pDBMsgPtr[i].Func = &NGMClientProcess::NullFuncPtr;

		ServerMessageMap(MASTERMSG_RESPONSE_GMCLOGIN,			DoMsg_MASTERMSG_RESPONSE_GMCLOGIN);
		ServerMessageMap(MASTERMSG_REPORT_SERVERREFRESHINFO,	DoMsg_MASTERMSG_REPORT_SERVERREFRESHINFO);
		ServerMessageMap(MASTERMSG_RESPONSE_SERVERINFO1,		DoMsg_MASTERMSG_RESPONSE_SERVERINFO1);
		ServerMessageMap(MASTERMSG_RESPONSE_SERVERINFO2,		DoMsg_MASTERMSG_RESPONSE_SERVERINFO2);
		ServerMessageMap(GOONZUGM_RESPONSE_ACCOUNTCHARINFO,		DoMsg_GOONZUGM_RESPONSE_ACCOUNTCHARINFO);
		ServerMessageMap(GOONZUGM_RESPONSE_USERCHARINFO,		DoMsg_GOONZUGM_RESPONSE_USERCHARINFO);
		
	}
}

NGMClientProcess::~NGMClientProcess()
{
	if(m_pDBMsgPtr != NULL)
		delete [] m_pDBMsgPtr;
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NGMClientProcess::Init()
{
	m_sf.CreateFactory( &m_sm, MAX_LOGCLIENT_CHANNEL, MAX_LOGCLIENT_PACKET_SIZE * MAX_LOGCLIENT_PACKET_COUNT, MAX_LOGCLIENT_PACKET_SIZE * MAX_LOGCLIENT_PACKET_COUNT );
	m_iocp.CreateIOCP( &m_sm, 1 );
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NGMClientProcess::Run()
{
	m_sm.Update();

	ITZSession* pkGMClientSession = m_sm.GetSession( 0 );
	
	if( pkGMClientSession->GetState() == SESSION_STATE_NONE )
	{
		if( m_sm.Connect( 0, m_szMasterServerIP, m_uiMasterServerBasePort ) == pkGMClientSession ) {

		}
		else
		{

		}
	}
	else if( pkGMClientSession->GetState() == SESSION_STATE_ESTABLISHED )
	{
		if( m_bSendLogin == FALSE )
		{
			sRequest_GMCLogin	sendMsg;

			strcpy( sendMsg.szID, m_szLoginID );
			strcpy( sendMsg.szPW, m_szLoginPW );

			pkGMClientSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

			m_bSendLogin = TRUE;						// 로그인 요청을 보냈다 

			return;
		}

		// 로그인 된 상태이고, 초기화 정보 요청을 보내지 않았고, 초기화가 안된 상태라면,
		if( m_bLoginOK == TRUE && m_bSendReqInit == FALSE && m_bInitOK == FALSE )
		{
			sRequest_ServerInfo1 sendMsg;
			pkGMClientSession->GetSendBuffer()->Write( (BYTE *)&sendMsg );

			sRequest_ServerInfo2 sendMsg2;
			pkGMClientSession->GetSendBuffer()->Write( (BYTE *)&sendMsg2 );
		
			m_bSendReqInit = TRUE;
		}

		if( m_bLoginOK == TRUE && m_bSendReqInit == TRUE && m_bInitOK == TRUE )
		{
			m_pNGMProcessSession = pkGMClientSession;
		}

		// 버퍼에서 패킷을 하나 꺼내온다
		sPacketHeader *pPacket = (sPacketHeader *)pkGMClientSession->GetRecvBuffer()->GetFirstPacket();		

		// 패킷이 있으면,
		if( pPacket != NULL )
		{
			MessageProc( pPacket, pkGMClientSession);
		}

		// 처리 후 패킷을 버퍼에서 지운다
		pkGMClientSession->GetRecvBuffer()->RemoveFirstPacket();
	}
}

//=============================================================================
//  @Name   X()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NGMClientProcess::MessageProc(sPacketHeader* pPacket, ITZSession* pSession)
{
	if( pPacket == NULL )	return;
	if( pSession == NULL )	return;

	if(pPacket->usCmd >= 0 && pPacket->usCmd <= ENDMSG)
	{
		(this->*(this->m_pDBMsgPtr[pPacket->usCmd].Func))(pPacket, pSession);
	}
	else
	{
		NullFuncPtr(pPacket, pSession);
	}
}

//=============================================================================
//  @Name   IsEstablished()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
BOOL NGMClientProcess::IsEstablished()
{
	if(m_pNGMProcessSession == NULL)									return FALSE;
	if(m_pNGMProcessSession->GetState() != SESSION_STATE_ESTABLISHED)	return FALSE;

	return TRUE;
}

BOOL NGMClientProcess::SendMsg(sPacketHeader *pPacket)
{
	if(m_pNGMProcessSession == NULL)					return FALSE;
	if(m_pNGMProcessSession->GetSendBuffer() == NULL)	return FALSE;

	if(!IsEstablished())								return FALSE;

	m_pNGMProcessSession->GetSendBuffer()->Write( (BYTE *)pPacket);

	return TRUE;
}

//=============================================================================
//  @Name   NullFuncPtr()
//! @author LeeKH						
//! @date   2007-3-12
//! @param  X   X
//! @return X
//! @brief  
//=============================================================================
void NGMClientProcess::NullFuncPtr(sPacketHeader* pPacket, ITZSession* pSession)
{
	char buffer[128];
	sprintf(buffer, "Not Defined Msg:%d", pPacket->usCmd);
}


void NGMClientProcess::DoMsg_MASTERMSG_RESPONSE_GMCLOGIN(sPacketHeader* pPacket, ITZSession* pSession)
{
	sResponse_GMCLogin	*pRecvMsg = (sResponse_GMCLogin *)pPacket;

	m_bLoginOK = TRUE;
}

void NGMClientProcess::DoMsg_MASTERMSG_REPORT_SERVERREFRESHINFO(sPacketHeader* pPacket, ITZSession* pSession)
{
	sReport_RefreshServerInfo	*pRecvMsg = (sReport_RefreshServerInfo *)pPacket;
}


void NGMClientProcess::DoMsg_MASTERMSG_RESPONSE_SERVERINFO1(sPacketHeader* pPacket, ITZSession* pSession)
{
	sResponse_ServerInfo1	*pRecvMsg = (sResponse_ServerInfo1 *)pPacket;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame == NULL)			return;

	ServerInfo *pServerInfo = pMainFrame->m_pServerInfo;
	if(pServerInfo == NULL)			return;

	pServerInfo->Wait();

	pServerInfo->m_frontServerInfo = pRecvMsg->frontServerInfo;
	pServerInfo->m_authServerInfo = pRecvMsg->authServerInfo;
	pServerInfo->m_accountDBManagerInfo = pRecvMsg->accountDBManagerInfo;

	pServerInfo->Release();

}

void NGMClientProcess::DoMsg_MASTERMSG_RESPONSE_SERVERINFO2(sPacketHeader* pPacket, ITZSession* pSession)
{
	sResponse_ServerInfo2	*pRecvMsg = (sResponse_ServerInfo2 *)pPacket;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame == NULL)			return;

	ServerInfo *pServerInfo = pMainFrame->m_pServerInfo;
	if(pServerInfo == NULL)			return;

	pServerInfo->Wait();

	pServerInfo->m_usWorldCount = pRecvMsg->usCount;
	memcpy( pServerInfo->m_gameWorldInfo, pRecvMsg->gameWorldInfo, sizeof( sGameWorldInfo ) * pRecvMsg->usCount );
	pServerInfo->SetServerInfo();

	pServerInfo->Release();

	m_bInitOK = TRUE;
}

void NGMClientProcess::DoMsg_GOONZUGM_RESPONSE_ACCOUNTCHARINFO(sPacketHeader* pPacket, ITZSession* pSession)
{
	sResponse_GMC_AccountCharInfo	*pRecvMsg = (sResponse_GMC_AccountCharInfo *)pPacket;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame == NULL)			return;

	CCharFrame* pkCharFrame = (CCharFrame*)pMainFrame->GetChild(CHILD_CHAR);
	if(pkCharFrame == NULL)			return;
	
	pkCharFrame->AddCharName(pRecvMsg->m_szServerName, pRecvMsg->m_szCharName1, pRecvMsg->m_szAccountID);
	pkCharFrame->AddCharName(pRecvMsg->m_szServerName, pRecvMsg->m_szCharName2, pRecvMsg->m_szAccountID);
	pkCharFrame->AddCharName(pRecvMsg->m_szServerName, pRecvMsg->m_szCharName3, pRecvMsg->m_szAccountID);
}

void NGMClientProcess::DoMsg_GOONZUGM_RESPONSE_USERCHARINFO(sPacketHeader* pPacket, ITZSession* pSession)
{
	sResponse_GMC_UserCharInfo	*pRecvMsg = (sResponse_GMC_UserCharInfo *)pPacket;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame == NULL)			return;

	CCharFrame* pkCharFrame = (CCharFrame*)pMainFrame->GetChild(CHILD_CHAR);
	if(pkCharFrame == NULL)			return;

	pkCharFrame->SetCharInfo(pRecvMsg);
}
