#ifndef _FRONTSERVER_H
#define _FRONTSERVER_H

#include "ITZPerson.h"
#include "WorldList.h"
#include "FrontMsg.h"

#include "../AuthServer/AuthMsg.h"


#include "ITZSession.h"
#include "ITZSendBuffer.h"
#include "ITZEncrypt.h"

#include "../../GoonZuWorld/Apex/gamecode.h"

#import "cryptcom.tlb" named_guids
using namespace CRYPTCOMLib;

extern "C" const GUID __declspec(selectany) IID_SIMPLECRYPT =
{0xfee2b996,0x789c,0x499b,{0x9f,0xc4,0x50,0x05,0x75,0xd4,0x61,0x5f}};

// 일본 넷마블 서비스 종료
//extern ISimpleCrypt*			g_pCrypt;

extern char					g_szServiceArea[12];
extern char					g_szServiceMode[8];


#include "../../GoonZuWorld/Lib/commonutil.h"	// 로그 작성을 위한 헤더파일
//#define DONOT_WRITEFILELOG					// 로그 작성을 막기위한 DEFINE - 이 DEFINE을 선언하면 로그를 작성하지 않는다.

struct sPatchInfo
{
	char addr[2][ 64 ];
	char dir[2][ 64 ];
	char id[2][ 32 ];
	char pw[2][ 32 ];
	int	 versionChangeUp;
};


// extern
extern cltWorldList g_worldlist;
extern char			g_strNotice[];
extern sPatchInfo	g_patchInfo;
extern SI32			g_patchselect;

extern DWORD		g_dwClientProtocolTick;
extern DWORD		g_dwGameProtocolTick;

#ifdef USE_PROMOTOR_FRONT_SERVER
	
	#include "PromoterManager/clPromoterManager.h"

	extern	clPromoterManager		g_clPromoterManager;
#endif


/*
extern char			g_strPatchText[];
extern SI32			g_siPacketTextSize;
extern SI32			g_siPacketTextCount;
*/

class FrontClientProtocol;
class FrontGameProtocol;
class AuthProcess;
class AgentProcess;

extern FrontClientProtocol	g_fcp;
extern FrontGameProtocol	g_fgp;
extern AuthProcess			g_auth;
extern AgentProcess			g_agent;

extern BOOL					g_bInitReady;							// 초기화 준비가 됨 
extern BOOL					g_bInitComplete;						// 초기화 완료 됨 

extern SI32					g_siServerID;


extern UI32					g_versionCount;
extern UI32					g_versionList[];

extern CRITICAL_SECTION		g_cs;


class cClient : public ITZPerson
{
public:
	cClient() { InitializeCriticalSection( &m_cs ); }
	~cClient() { LeaveCriticalSection( &m_cs); }

public:
	virtual void Init() { 
						  EnterCriticalSection( &m_cs );

						  printf( "Client NewConnect\n" ); 
//						  m_siPacketTextPacket = 0;

						  m_siUnique = 0;

						  m_dwLastTick = GetTickCount() - 5000;
						  m_dwRefreshTick = m_dwLastTick;

						  LeaveCriticalSection( &m_cs );
						  
						  sResponse_SecretKey sendMsg;

						  sendMsg.packetHeader.usCmd = FRONTMSG_RESPONSE_SECRETKEY;
						  sendMsg.packetHeader.usSize = sizeof( sendMsg );

						  sendMsg.ucKey1 = (UI08)rand();
						  sendMsg.ucKey2 = (UI08)rand();

						  GetSession()->GetEncrypt()->SetKey( sendMsg.ucKey1, sendMsg.ucKey2 );
						  GetSendBuffer()->Write( (BYTE*)&sendMsg );
	};

	virtual void Close() { printf( "Client Disconnected\n" ); };


	DWORD	m_dwLastTick;
	DWORD	m_dwRefreshTick;

	SI32	m_siUnique;

	//SI32	m_siPacketTextPacket;					// 남은(보내야할) 공지 패킷 숫자

	CRITICAL_SECTION		m_cs;
	CGameServer				m_GameServer;
};


class cGame : public ITZPerson
{
public:
	cGame() {};
	~cGame() {};

public:
	virtual void Init() { printf( "GameServer NewConnect\n" ); 
						  m_siServerID = 0;
						  m_dwLastRecvTick = GetTickCount(); 
						  
						  m_bInitServerInfo = FALSE;
						  m_bRequestServerInfo = FALSE;						  
						  
						  m_dwRequestTick = 0;
	};

	virtual void Close() { printf( "GameServer Disconnected\n" ); 

		EnterCriticalSection( &g_cs );

		GAMESERVER_REFRESHINFO refreshinfo;

		if( g_worldlist.GetWorldRefreshInfoByServerID( m_siServerID, &refreshinfo ) ) {

			refreshinfo.state = 2;
			refreshinfo.currentConnections = 0;

			g_worldlist.SetWorldRefreshInfo( refreshinfo.worldId, &refreshinfo );

		}

		LeaveCriticalSection( &g_cs );
	
	};
	
public:
	DWORD			m_dwLastRecvTick;
	SI32			m_siServerID;

	BOOL			m_bInitServerInfo;
	BOOL			m_bRequestServerInfo;

	DWORD			m_dwRequestTick;
};


class cAuth : public ITZPerson
{
public:
	cAuth() {};
	~cAuth() {};

	virtual void Init() { printf( "인증서버와 접속\n" ); };
	virtual void Close() { printf( "인증서버와 접속종료\n" ); };
};


#endif

