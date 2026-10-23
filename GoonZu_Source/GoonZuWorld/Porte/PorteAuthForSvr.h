/**
* @brief PorteAuthForSvr.h
* This file declares PURPLE Auth. for Server Module api
* 
* Copyright (c) 2008 NHN Corporation. All Rights Reserved.
*
* @author		: nhn
* @date		: 2009-02-03
*/

#ifndef __PURPLEAuthForSvr__
#define __PURPLEAuthForSvr__

#if defined(_WIN32) || defined(_WIN64)
	#ifdef PORTEAUTHFORSVR_EXPORTS
		#define PAUTH_API __declspec(dllexport)
	#else
		#define PAUTH_API __declspec(dllimport)
		#ifdef _DEBUG
			#ifdef _WIN64
				#pragma comment(lib, "PorteAuthForSvrD_x64.lib") 
			#elif defined(_WIN32)
				#pragma comment(lib, "PorteAuthForSvrD.lib") 
			#else
				#error do not support os
			#endif
		#else
			#ifdef _WIN64
				#pragma comment(lib, "PorteAuthForSvr_x64.lib") 
			#elif defined(_WIN32)
				#pragma comment(lib, "PorteAuthForSvr.lib") 
			#else
				#error do not support os
			#endif
		#endif
	#endif
#elif defined(__linux__)
	#define PAUTH_API
	#define __stdcall
#endif

namespace Porte {
	namespace PorteAuthSystem {

		#ifndef IN
			#define IN
		#endif
		#ifndef OUT
			#define OUT
		#endif

		// predefine size
		// usage at game server code
		///////////////////////////////////////////////////////////////////
		#define SIZE_AUTHTICKET			2048
		#define SIZE_USERID				21
		#define SIZE_GAMEID				512
		#define SIZE_USERINFORMATION	2048
		///////////////////////////////////////////////////////////////////

		// service code
		///////////////////////////////////////////////////////////////////
		// nation service code
		#define SERVICE_NATION  0x000000FF	// mask of nation service code
		#define SERVICE_KOR		0x00000001
		#define SERVICE_USA		0x00000002
		#define SERVICE_JPN		0x00000003
		#define SERVICE_CHN		0x00000004

		// stage service code
		#define SERVICE_STAGE   0x00000F00	// mask of stage service code
		#define SERVICE_ALPHA	0x00000100
		#define SERVICE_REAL	0x00000200
		#define SERVICE_BETA	0x00000300

		// IDC service code
		#define SERVICE_IDC		0x0000F000	// mask of IDC service code
		#define SERVICE_IDC01	0x00001000
		#define SERVICE_IDC02	0x00002000
		#define SERVICE_IDC03	0x00003000
		#define SERVICE_IDC04	0x00004000
		#define SERVICE_IDC05	0x00005000
		#define SERVICE_IDC06	0x00006000
		#define SERVICE_IDC07	0x00007000
		#define SERVICE_IDC08	0x00008000
		#define SERVICE_IDC09	0x00009000
		#define SERVICE_IDC10	0x0000A000
		#define SERVICE_IDC11	0x0000B000
		#define SERVICE_IDC12	0x0000C000
		#define SERVICE_IDC13	0x0000D000
		#define SERVICE_IDC14	0x0000E000
		#define SERVICE_IDC15	0x0000F000
		#define SERVICE_IDCNONE	0x00000000
		///////////////////////////////////////////////////////////////////


		/* error code
		Return value Info:
		0 : OK
		- : fault from  function / this system 
		///////////////////////////////////////////////////////////////////
		*/
		#define	ERR_OK					 0			// Success
		#define ERR_UNEXPECTED			-1			// Unexpected error
		#define ERR_INVALIDARGUMENT		-2			// The parameter being NULL	(e.g. returns to error if the Game Info. String or Out Buffer is NULL)
		#define ERR_ISNOTINITED			-3			// The module not being initialized (if the PubAuthInit() method is not called or the calling fails)
		#define ERR_ALREADYINITED		-4			// Announces that initialization has already been done if PubAuthInit and PubSetEventCallBack APIs are called for the second time
		#define ERR_INITFAIL			-5			// Failure to initialize in the environment(ServiceCode)
		#define ERR_CREATECONNETIONPOOL	-6			// Failure to connect to the Auth. Server
		#define ERR_GETCONNETIONPOOL	-7			// Failure to get a Connection Pool
		#define ERR_INVALIDUSERINFO		-8			// Error in the User Info. String format
		#define ERR_NORESPONSE			-9			// no response message from the PURPLE Auth. Server
		#define ERR_SMALLBUFFER			-30			// buffer size is small

		///////////////////////////////////////////////////////////////////

		// PubCheckAuth return value
		// Return value Info:
		// 0 : OK
		// - : fault from  function / this system 
		//////////////////////////////////////////////////////////////////////////

			// auth server return message
			//////////////////////////////////////////////////////////
			#define PORTE_AUTH_SUCCESS					0			// Not actually delivered; substituted by ERR_OK Event code
			#define	PORTE_AUTH_FAIL						-1			// Failure to validate the Auth. Ticket

			#define PORTE_AUTH_TICKET_TIMEOVER			-10			// Valid time for the Auth. Ticket expired
			#define PORTE_AUTH_TICKET_DIFF_IP			-11			// The Client IP registered in the Auth. Ticket and the Client IP attempting refreshment are different
			#define PORTE_AUTH_TICKET_DIFF_ID			-12			// The User ID registered in the Auth. Ticket and the User ID attempting refreshment are different
			#define PORTE_AUTH_TICKET_WRONG_FORMAT		-13			// Different format for the Auth. Ticket
			#define PORTE_AUTH_UNKONWN_CLIENT_IP		-14			// The Client IP attempting refreshment unidentifiable
			#define PORTE_AUTH_DIFFERENT_UNIQUEKEY		-16			// TICKET에 부여한 UNIQUE KEY가 서버가 가지고 있는 UNIQUE KEY와 다름
			#define PORTE_AUTH_DIFFERENT_CHECKSUM		-17			// CHECKSUM이 다름

			#define PORTE_AUTH_TICKET_MAKEKEY_FAIL		-20			// Failure to generate an encrypted key for the Auth. Ticket
			#define	PORTE_AUTH_TICKET_ENCRYPT_FAIL		-21			// Failure to encrypt the Auth. Ticket
			#define	PORTE_AUTH_TICKET_DECRYPT_FAIL		-22			// Failure to decrypt the Auth. Ticket
			#define	PORTE_AHTH_TICKET_INVALID			-23			// The length of the value exceeds the designated length
			//////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		


#if defined(__cplusplus)
		extern "C"{
#endif

		// init server module
		PAUTH_API int __stdcall PubAuthInit(IN char* szGameID,
											IN int   nServiceCode,
											IN unsigned int nConnCnt,
											IN const char* szCustomConfigFileName = NULL);

		// validate authentication
		PAUTH_API int __stdcall PubCheckAuth(IN  char *szUserID,
											 IN  char *szTicket,
											 IN  char *szClientIP,
											 OUT char *szOutBuff,
											 IN  unsigned int size_buf);
		// Get User Info
		PAUTH_API int __stdcall PubGetValue(IN  char *szUserInfo,
											IN  char *szKeyName,
											OUT char *pBuffer,
											IN unsigned int nSize);

		// destroy server module
		PAUTH_API int __stdcall PubDestroy();

#if defined(__cplusplus)
		}
#endif

	}
}

#endif
