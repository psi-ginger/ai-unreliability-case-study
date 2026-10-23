//
// Apex와의 연동을 위한 모듈
//
// 2007/03/15 이제완
// 2007/03/17 테스트 완료
//
// 비주얼 스튜디오 검색창에서 'APEX-'을 통해 작업 내역 검색 가능
// _USE_APEX 가 define 되었을 경우에만 처리한다
// APEX를 사용할 경우 전처리기에서 _USE_APEX를 선언해야 한다

#ifndef _APEXGOONZU_H
#define _APEXGOONZU_H

#include "NDataTypes.h"

#ifdef _CHINA
	#pragma comment(lib, "AHClientInterface.lib")
#endif

// Client
int	StartApexClient();
int	StopApexClient();
int NoticeApec_UserData( const char * pBuf, int nBufLen );

long NetSendToGameServer( const char * pBuffer,int nLen );


// Server
int	StartApexProxy();
int StopApexProxy();

int NoticeApexProxy_UserLogin( int nSendId, char *szID );
int	NoticeApexProxy_UserLogout( int nSendId, char *szID );
int	NoticeApexProxy_UserData( int nSendId, const char* pBuf, int nBufLen );

long NetSendToGameClient( signed int nSendId, const char* pBuffer, int nLen );
long GameServerKillUser( signed int nId, int Action );

#endif

