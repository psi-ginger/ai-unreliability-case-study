#ifndef _DBMSGWEB_H
#define _DBMSGWEB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DBMsg.h"
#include "..\..\GoonZuWorld\Common\PrivateTradeOrderMgr\PrivateTradeOrderBaseInfo.h"

//-----------------------------------------
// 군주 신문 기사 요청. 
//-----------------------------------------
class sDBRequest_News
{
public:
	sPacketHeader		packetHeader;
	_SYSTEMTIME			sTime;

	sDBRequest_News()
	{
		ZeroMemory(this, sizeof(sDBRequest_News));
	}

	sDBRequest_News(_SYSTEMTIME* ptime)
	{
		packetHeader.usSize = sizeof( sDBRequest_News );
		packetHeader.usCmd	= DBMSG_REQUEST_NEWS;

		memcpy(&sTime, ptime, sizeof(_SYSTEMTIME));
	}

};

#define MAX_NEWS_TITLE_LENGTH		128
#define MAX_NEWSPAPER_NAME_LENGTH	32
class sDBResponse_News
{
public:
	sPacketHeader		packetHeader;

	SI32				siResult;			// 결과값.
	
	//bool   bBroadcastSwitch;						// 군주 방송 진행 여부. 

	TCHAR	szNewsTitle[MAX_NEWS_TITLE_LENGTH];			// 뉴스 머릿기사. 
	TCHAR	szNewspaperName[MAX_NEWSPAPER_NAME_LENGTH];	// 신문사 이름. 
	TCHAR	szNewURL[MAX_PATH];							// 뉴스 링크되는 URL

	sDBResponse_News()
	{
		
		ZeroMemory(this, sizeof(sDBResponse_News));
	}
};




#endif
