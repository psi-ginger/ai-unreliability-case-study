#ifndef __HanReportForSvr__
#define __HanReportForSvr__

#ifdef HANREPORTFORSVR_EXPORTS
#define HANREPORT_API __declspec(dllexport)
#else
#define HANREPORT_API __declspec(dllimport)
#endif

#define SERVICE_NATION  0x000000FF
#define SERVICE_KOR		0x00000001
#define SERVICE_USA		0x00000002
#define SERVICE_JPN		0x00000003
#define SERVICE_CHN		0x00000004

#define SERVICE_TYPE    0x00000F00
#define SERVICE_ALPHA	0x00000100
#define SERVICE_REAL	0x00000200
#define SERVICE_BETA	0x00000300


/*
Return value Info:
0 : Ok
- : fault from  function / this system 
-1 : general fault.
+ : fault from  auth server
+1 : parameter of auth is invalid.
+7 : system error
*/

#define HAN_REPORTSVR_OK						0
#define HAN_REPORTSVR_PARAM_INVALID				1
#define HAN_REPORTSVR_SYSTEM_ERROR				7

#define HAN_REPORTSVR_ARGUMENT_INVALID		   -1
#define HAN_REPORTSVR_INITED_ALREADY		   -2
#define HAN_REPORTSVR_INITED_NOT			   -3
#define HAN_REPORTSVR_INITED_FAIL			   -4
#define HAN_REPORTSVR_CONNPOOL_CREATEFAIL	   -5
#define HAN_REPORTSVR_GETCONNECTION_FAIL	   -6
#define HAN_REPORTSVR_OPENCONNECTION_FAIL	   -7
#define HAN_REPORTSVR_SENDCONNECTION_FAIL	   -8
#define HAN_REPORTSVR_DLL_UNEXPECTED		 -100


HANREPORT_API int __stdcall HanReportInit(/*IN*/ char* szGameId,
										  /*IN*/ int   nServiceCode,
										  /*IN*/ unsigned int nConnCnt);

HANREPORT_API int __stdcall HanReportSendPosLog(/*IN*/ char* szReportString);

HANREPORT_API int __stdcall HanReportSend(/*IN*/ char* szSubject,
										  /*IN*/ char* szReportString);
#endif