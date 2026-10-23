#ifndef __HAN_ANTITOXIC_H__
#define __HAN_ANTITOXIC_H__

//ip체크모듈과 달리 windows와 DBGW만 사용하므로, 리눅스와 http관련 설정은 제거함

#ifdef HANANTITOXIC_EXPORTS
	#define HAN_ANTITOXIC_API __declspec(dllexport)
#else
	#define HAN_ANTITOXIC_API __declspec(dllimport)
	#ifdef _DEBUG
		#ifdef _WIN64
			#pragma comment(lib, "HanAntiToxicD_x64.lib") 
		#else
			#pragma comment(lib, "HanAntiToxicD.lib") 
		#endif
	#else
		#ifdef _WIN64
			#pragma comment(lib, "HanAntiToxic_x64.lib") 
		#else
			#pragma comment(lib, "HanAntiToxic.lib") 
		#endif
	#endif
#endif

#pragma comment(lib, "ws2_32.lib")

#ifndef IN
	#define IN
#endif

#ifndef OUT
	#define OUT
#endif

// Constants
#define SERVICE_NATION										0x000000FF
#define SERVICE_KOR											0x00000001
#define SERVICE_USA											0x00000002
#define SERVICE_JPN											0x00000003
#define SERVICE_CHN											0x00000004

#define SERVICE_TYPE										0x00000F00
#define SERVICE_ALPHA										0x00000100
#define SERVICE_REAL										0x00000200
#define SERVICE_BETA										0x00000300

#define SERVICE_SITE										0x0000F000
#define SERVICE_HANGAME										0x00001000
#define SERVICE_ASOBLOG										0x00002000

/*
Return value Info:
0 : Ok
- : fault from  function / this system 
-1 : general fault.
+ : fault from server
*/
#define HAN_ANTITOXIC_OK										0
#define HAN_ANTITOXIC_ARGUMENT_INVALID						-1
#define HAN_ANTITOXIC_INITED_NOT								-2
#define HAN_ANTITOXIC_INITED_FAIL								-3
#define HAN_ANTITOXIC_DBGW_RPC_CLIENT_CREATEION_FAILED		-4
#define HAN_ANTITOXIC_DBGW_RPC_CONNECTOR_CREATEION_FAILED		-5
#define HAN_ANTITOXIC_DBGW_CONNECTION_FAILED					-6
#define HAN_ANTITOXIC_DBGW_EXEC_FAILED						-7
#define HAN_ANTITOXIC_DBGW_SELECTRESULT_FAILED				-8
#define HAN_ANTITOXIC_DBGW_SELECTRESULT_INVALID				-9
#define HAN_ANTITOXIC_DBGW_SELECTRESULT_COLUMN_INVALID		-10
#define HAN_ANTITOXIC_DBGW_LOAD_LOCAL_DIRECTORY_FILE_FAILED	-11
#define HAN_ANTITOXIC_DBGW_FUNCTIONNAME_INVALID				-12
#define HAN_ANTITOXIC_DBGW_SELECTRESULT_OVERCOUNT				-13
#define HAN_ANTITOXIC_DLL_UNEXPECTED							-100


HAN_ANTITOXIC_API int __stdcall 
HanAntiToxicInit(IN const char* szGameId);

HAN_ANTITOXIC_API int __stdcall 
HanGetAntiToxic(IN char* chn_Id, IN char* userId, IN char* checkSum, OUT int *playTime);

HAN_ANTITOXIC_API int __stdcall 
HanSetAntiToxic(IN char* chn_Id, IN char* userId, IN int playTime);

HAN_ANTITOXIC_API int __stdcall 
HanAntiToxicGetLastError();

HAN_ANTITOXIC_API const char* __stdcall 
HanAntiToxicErrorString(IN int nErrorCode);

HAN_ANTITOXIC_API void __stdcall 
HanAntiToxicTerm();

#endif // __HAN_ANTITOXIC_H__