#ifndef _ITZBASE_H
#define _ITZBASE_H

#ifdef __WS32__

	#include <winsock2.h>

#else
 
	#ifndef _AFXDLL

		#ifndef _WINDOWS_

			#ifndef _WINSOCKAPI_
				#include <winsock2.h>
			#endif

			#include <windows.h>
		#endif

	#else

		#include <winsock2.h>

	#endif
#endif

#include <process.h>

typedef signed int			SI32;
typedef signed short		SI16;
typedef signed char			SI08;

typedef unsigned int		UI32;
typedef unsigned short		UI16;
typedef unsigned char		UI08;

typedef __int64				SI64;
typedef unsigned __int64	UI64;

typedef double				REAL64;
typedef float				REAL32;

#ifndef PACKET_MAX_SIZE
#define PACKET_MAX_SIZE		65000
#endif

#include <windows.h>

#define STRSAFE_NO_DEPRECATE
#define STRSAFE_NO_CB_FUNCTIONS
#include <strsafe.h>

#include <tchar.h>

#pragma comment(lib, "ws2_32.lib")

#pragma pack(1)
 
//-----------------------------------------------------------------------------
// 패킷 헤더
//-----------------------------------------------------------------------------
struct sPacketHeader
{
	UI16	usSize;
	UI08	ucChk;
	UI16	usCmd;
};

#pragma pack()

#endif