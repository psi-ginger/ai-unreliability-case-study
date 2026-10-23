// HanBanCheckForServer.h
#pragma once

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the HANBANCHECKFORSERVER_EXPORTS
// defined with this macro as being exported.
#if defined(_WIN32) || defined(_WIN64)
	#ifdef HANBANCHECKFORSERVER_EXPORTS
		#define HAN_BANCHECKSVR_API __declspec(dllexport)
	#else
		#define HAN_BANCHECKSVR_API __declspec(dllimport)
		#ifdef _DEBUG
			#ifdef _WIN64
				#pragma comment(lib, "HanBanCheckForSvrD_x64.lib") 
			#else
				#pragma comment(lib, "HanBanCheckForSvrD.lib") 
			#endif
		#else
			#ifdef _WIN64
				#pragma comment(lib, "HanBanCheckForSvr_x64.lib") 
			#else
				#pragma comment(lib, "HanBanCheckForSvr.lib") 
			#endif
		#endif
	#endif
#else
	#define IN
	#define OUT
#endif

#ifndef IN
	#define IN
#endif

#ifndef OUT
	#define OUT
#endif
//
// Service Codes
//
// nation service code
#define SERVICE_NATION  0x000000FF
#define SERVICE_KOR		0x00000001
#define SERVICE_USA		0x00000002
#define SERVICE_JPN		0x00000003
#define SERVICE_CHN		0x00000004
// stage service code
#define SERVICE_TYPE    0x00000F00
#define SERVICE_ALPHA	0x00000100
#define SERVICE_REAL	0x00000200
#define SERVICE_BETA	0x00000300

//
// Common Error Code
//   0 is OK, posivites are responses from server,
//   negatives are errors on local system.
//
#define HAN_BANCHECKSVR_OK								0
#define HAN_BANCHECKSVR_ARGUMENT_INVALID				-1		// parameter is invalid. size is over or null
#define HAN_BANCHECKSVR_INITED_ALREADY					-2		// already inited
#define HAN_BANCHECKSVR_INITED_NOT						-3		// doesn't init. do init!
#define HAN_BANCHECKSVR_INITED_FAIL						-4		// init fail. may be service code is wrong
#define HAN_BANCHECKSVR_HTTP_INITFAIL					-5		// fail to creat http handle
#define HAN_BANCHECKSVR_HTTP_CONNECTFAIL				-6		// fail to connect to web server
#define HAN_BANCHECKSVR_HTTP_OPENFAIL					-7		// fail to get http contents
#define HAN_BANCHECKSVR_HTTP_SENDFAIL					-8		// fail to send message
#define HAN_BANCHECKSVR_HTTP_RECVFAIL					-9		// fail to recive message
#define HAN_BANCHECKSVR_DLL_UNEXPECTED					-100	// exception error
#define HAN_BANCHECKSVR_CONNPOOL_CREATEFAIL				-101	// fail to creat connection pool
#define HAN_BANCHECKSVR_GETCONNECTION_FAIL				-102	// fail to get http handle from connection pool

//
// Length Limits for API Parameters:
//
#define SIZE_GAMEID							20
#define SIZE_MEMBERID						50
#define SIZE_OUTBUFF						(4 << 10)	// that's 4KB

//
// Initial values
//
#define INVALID_SERVICE_CODE				0xFFFFFFFF

// Init module
// param :
//		szGameID	 : Game ID
//		nServiceCode : Service Code
//						입력을 안하면 config file을 찾아 설정합니다.
// return :
//		Common Error Code
// config file :
/*		File Name : GAMEID.xml
/////////////////////////////////////////////////
<?xml version='1.0' encoding='euc-kr'?>
<purple>  
	<!-- Ban Checker -->
	<module name="banchecker">
		<group name="setup_info">
			<!-- nation : korea, japan, china, usa -->
			<property name="nation" value="usa"/>
			<!-- stage : alpha, beta, real -->
			<property name="stage" value="alpha"/>
			<!-- domain string size limit : 512byte -->
			<property name="domain" value="alpha-member.ijji.com"/>
			<!-- conn_pool_size limit : 1 ~ (10^33 - 1) -->
			<property name="conn_pool_size" value="10"/>
			<!-- query format string size limit : 512byte -->
			<property name="query_format">
				<![CDATA[
				api/account.nhn?m=checkgift&memberid=%s
				]]>
			</property>
		</group>
	</module>
</purple>
/////////////////////////////////////////////////
*/
HAN_BANCHECKSVR_API int		__stdcall HanBanCheckInit(	IN char *szGameID,
														IN int   nServiceCode = INVALID_SERVICE_CODE);

// Check Ban ID
// param :
//		szID				: Member ID
//		szResultString		: Result String
//								CODE | (true/false/null) | Message
//								ex. 0|true|Success
//		nResultStringSize	: szResultString size
// result :
//		Common Error Code
HAN_BANCHECKSVR_API int		__stdcall HanBanCheckReq(	IN char* szID,
														OUT char* szResultString,
														IN size_t nResultStringSize);

// Get result info
// param:
//		szOutBuff			: Result String
//				szOutBuff   Code | Value1 | Value2 | Value3 | ...
//				nIndex       0        1        2        3     ...
//		nIndex				: value index
//		szValue				: Value Stirng
//		nValueSize			: szValue size
//  return :
//		Common Error Code
HAN_BANCHECKSVR_API int		__stdcall HanBanCheckParsing(IN char* szOutBuff,
														IN unsigned int nIndex,
														IN char* szValue,
														IN size_t nValueSize);
