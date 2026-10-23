// Revision History
//	3/8/2001	Added support for Windows Whistler Detection
//

#include <string.h>
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include "OSInformation.h"

#ifndef VER_SUITE_PERSONAL
	#define VER_SUITE_PERSONAL	0x00000200
#endif

/////////////////////////////////////////////////////////////////////////////
// COSInformation

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용 중인 OS 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//#define    WINDOWS_95          "Windows 95"
//#define    WINDOWS_98          "Windows 98"
//#define    WINDOWS_ME          "Windows ME"
//#define    WINDOWS_NT          "Windows NT"
//#define    WINDOWS_NT_351      "Windows NT 351"
//#define    WINDOWS_NT_40       "Windows NT 40"
//#define    WINDOWS_2000        "Windows 2000"
//#define    WINDOWS_XP          "Windows XP"
//#define    WINDOWS_SERVER_2003 "Windows SERVER 2003"
//--------------------------------------------------------------------------------------------
bool COSInformation::get_ClientOSName(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true) {
		return hRes;
	}

	if(!strcmp(m_strMajorVersion, WINDOWS_95_MAJORVERSION) && 
		!strcmp(m_strMinorVersion, WINDOWS_95_MINORVERSION))
	{

		if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
		{
			strcpy(pstrVal, WINDOWS_95);
			return true;
		}
		else
			return false;
	}

	if(!strcmp(m_strMajorVersion, WINDOWS_98_MAJORVERSION) && 
		!strcmp(m_strMinorVersion, WINDOWS_98_MINORVERSION))
	{
		if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
		{
			strcpy(pstrVal, WINDOWS_98);
			return true;
		}
		else
			return false;
	}

	if(!strcmp(m_strMajorVersion, WINDOWS_ME_MAJORVERSION) && 
		!strcmp(m_strMinorVersion, WINDOWS_ME_MINORVERSION))
	{
		if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
		{
			strcpy(pstrVal, WINDOWS_ME);
			return true;
		}
		else
			return false;
	}

	if(!strcmp(m_strMajorVersion, WINDOWS_NT_40_MAJORVERSION) && 
		!strcmp(m_strMinorVersion, WINDOWS_NT_40_MINORVERSION))
	{
		if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
		{
			strcpy(pstrVal, WINDOWS_NT_40);
			return true;
		}
		else
			return false;
	}

	if(!strcmp(m_strMajorVersion, WINDOWS_2000_MAJORVERSION) && 
		!strcmp(m_strMinorVersion, WINDOWS_2000_MINORVERSION))
	{
		if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
		{
			strcpy(pstrVal, WINDOWS_2000);
			return true;
		}
		else
			return false;
	}

	if(!strcmp(m_strMajorVersion, WINDOWS_XP_MAJORVERSION) && 
		!strcmp(m_strMinorVersion, WINDOWS_XP_MINORVERSION))
	{
		if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
		{
			strcpy(pstrVal, WINDOWS_XP);
			return true;
		}
		else
			return false;
	}

	if(!strcmp(m_strMajorVersion, WINDOWS_SERVER_2003_MAJORVERSION) && 
		!strcmp(m_strMinorVersion, WINDOWS_SERVER_2003_MINORVERSION))
	{
		if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
		{
			strcpy(pstrVal, WINDOWS_SERVER_2003);
			return true;
		}
		else
			return false;
	}

	return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS BuildNumber 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool COSInformation::get_BuildNumber(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true) {
		return hRes;
	}

	Val = m_BuildNumber;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS의 Win95 여부 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool COSInformation::get_IsWin95(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true) {
		return hRes;
	}

	Val = m_bIsWin95;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS Platform 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool COSInformation::get_Platform(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true) {
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_strPlatform);
		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS Major Version 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool COSInformation::get_MajorVersion(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true) {
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_strMajorVersion);
		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS Minor Version 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool COSInformation::get_MinorVersion(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true) {
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_strDetailVersion);
		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS ServicePack 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool COSInformation::get_ServicePack(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true) {
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_OSINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_strServicePack);
		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool COSInformation::GetInformation ()
{
	int stat = 0;
	char strInfo[MAX_OSINFO_STRING_LENGTH];
	char strTemp[MAX_OSINFO_STRING_LENGTH];
	TCHAR data [64];
	unsigned long dataSize;//, dwError;
	OSVERSIONINFOEX  versionInfo;
	HKEY hKey;
	LONG result;
	LPTSTR lpErrDesc = NULL;
	BOOL bOsVersionInfoEx = FALSE;
	// If we already have generated the information, then there is no need to do it again.
	if (m_bInformationObtained)
	{
		return true;
	}
	
	// Try calling GetVersionEx using the OSVERSIONINFOEX structure,
	// If that fails, try using the OSVERSIONINFO structure.
	::ZeroMemory(&versionInfo, sizeof(OSVERSIONINFOEX));
	versionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !(bOsVersionInfoEx = ::GetVersionEx ((OSVERSIONINFO *) &versionInfo)) )
	{
		// If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
		versionInfo.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if (!::GetVersionEx ( (OSVERSIONINFO *) &versionInfo) ) 
		{
			//dwError = GetLastError ();
			return false;
		}
   }

	// If NT ,then check if its server or workstation.
	//char szBuffer[128];

	/**
	leon add
	* Major/Minor Version Number 얻음
	*/
	sprintf(m_strMajorVersion, "%d", versionInfo.dwMajorVersion);
	sprintf(m_strMinorVersion, "%d", versionInfo.dwMinorVersion);

	if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		// If OSVERSIONEX has been used, then we will try to extract
		// the info from that structure.
		if (bOsVersionInfoEx)
		{
			// Load the platform type information.
			switch (versionInfo.wProductType)
			{
				case VER_NT_WORKSTATION:					
					if (versionInfo.dwMinorVersion > 0)
					{
						strcpy(strInfo, WHISTLER_PROF);
					}
					else
						strcpy(strInfo, WIN2K_PROF);
					break;

				case VER_NT_DOMAIN_CONTROLLER:
					strcpy(strInfo, WIN2K_DOMAIN_CONTROLLER);
					break;

				case VER_NT_SERVER:
					if (versionInfo.dwMinorVersion > 0)
					{
						strcpy(strInfo, WHISTLER_SERVER);
					}
					else
						strcpy(strInfo, WIN2K_SERVER);
					break;
			}

			strcpy(m_strPlatform, strInfo);

			strcpy(m_strSuites, "");
			strcpy(strTemp, "");

			// Load the suites installed.
			if (versionInfo.wSuiteMask & VER_SUITE_DATACENTER)
			{
				strcpy(m_strSuites, SUITE_DATACENTER);
			}
			if (versionInfo.wSuiteMask & VER_SUITE_ENTERPRISE)
			{
				strcpy(strTemp, "");
				sprintf(strTemp, "%s;%s", m_strSuites, SUITE_ENTERPRISE);
				strcpy(m_strSuites, "");
				strcpy(m_strSuites, strTemp);
			}
			if (versionInfo.wSuiteMask & VER_SUITE_BACKOFFICE)
			{
				strcpy(strTemp, "");
				sprintf(strTemp, "%s;%s", m_strSuites, SUITE_BACKOFFICE);
				strcpy(m_strSuites, "");
				strcpy(m_strSuites, strTemp);
			}
			if (versionInfo.wSuiteMask & VER_SUITE_SMALLBUSINESS)
			{
				strcpy(strTemp, "");
				sprintf(strTemp, "%s;%s", m_strSuites, SUITE_SMALLBUSINESS);
				strcpy(m_strSuites, "");
				strcpy(m_strSuites, strTemp);
			}
			if (versionInfo.wSuiteMask & VER_SUITE_SMALLBUSINESS_RESTRICTED)
			{
				strcpy(strTemp, "");
				sprintf(strTemp, "%s;%s", m_strSuites, SUITE_SMALLBUSINESS_RESTRICTED);
				strcpy(m_strSuites, "");
				strcpy(m_strSuites, strTemp);
			}
			if (versionInfo.wSuiteMask & VER_SUITE_TERMINAL)
			{
				strcpy(strTemp, "");
				sprintf(strTemp, "%s;%s", m_strSuites, SUITE_TERMINAL);
				strcpy(m_strSuites, "");
				strcpy(m_strSuites, strTemp);
			}
			if (versionInfo.wSuiteMask & VER_SUITE_PERSONAL)
			{
				strcpy(strTemp, "");
				sprintf(strTemp, "%s;%s", m_strSuites, SUITE_PERSONAL);
				strcpy(m_strSuites, "");
				strcpy(m_strSuites, strTemp);
			}
		}
		else
		{
			// there is no direct way of telling from ODVERSIONINFO thats is it 
			// workstation or server version.
			// There we need to check in the registry.
			if (versionInfo.dwMajorVersion <= 4)
			{
				strcpy(m_strPlatform, WINDOWS_NT);
			}
			else if (versionInfo.dwMajorVersion == 5 &&
					 versionInfo.dwMinorVersion == 0)
			{
				strcpy(m_strPlatform, WINDOWS_2000);
			}
			else if (versionInfo.dwMajorVersion == 5 &&
					 versionInfo.dwMinorVersion > 0)
			{
				strcpy(m_strPlatform, WINDOWS_WHISTLER);
			}

			dataSize = sizeof (data);
			result = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
									"System\\CurrentControlSet\\Control\\ProductOptions",
									0, KEY_QUERY_VALUE, &hKey);

			// If there is error in opening the registry key, return
			if (result != ERROR_SUCCESS)
			{
				//dwError = ::GetLastError ();
				return false;
			}

			result = ::RegQueryValueEx (hKey, _T("ProductType"), NULL, NULL, (LPBYTE) data,
										&dataSize);

			// Make sure to close the reg key
			RegCloseKey (hKey);

			if (result != ERROR_SUCCESS)
			{
				//dwError = GetLastError ();
				return false;
			}

			// Check what string has been returned
			if (lstrcmpi (data, "WinNT") == 0)
			{
				strcpy(strInfo, NT_WORKSTATION);
			}
			else if (lstrcmpi (data, "ServerNT") == 0)
			{
				strcpy(strInfo, NT_SERVER);
			}
			else
			{
				strcpy(strInfo, NT_DOMAIN_CONTROLLER);
			}
			
			strcpy(strTemp, "");
			sprintf(strTemp, "%s-%s", m_strPlatform, strInfo);
			strcpy(m_strPlatform, "");
			strcpy(m_strPlatform, strTemp);
		}

		// Check the version number
		strcpy(strTemp, "");
		switch (versionInfo.dwMajorVersion)
		{
			case 3:
				strcpy(strInfo, NT_351);
				break;
			case 4:
				strcpy(strInfo, NT_40);
				break;
			case 5:
				strcpy(strInfo, NT_50);
				
				if (versionInfo.dwMinorVersion > 0)
				{
					strcpy(strInfo, NT_51);
				}
				break;
		}

		strcpy(m_strDetailVersion, strInfo);
		// Get the build number.
		m_BuildNumber = versionInfo.dwBuildNumber;
	}
	else if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
	{
		strcpy(strInfo, "");
		strcpy(strInfo, WINDOWS_95);

		m_bIsWin95 = TRUE;

		// Windows 98 test
		if ((versionInfo.dwMajorVersion > 4) || ((versionInfo.dwMajorVersion == 4) &&
			(versionInfo.dwMinorVersion > 0)))
		{
			// NKK-- 2/7/2001
			// Code added for WinME detection.
			if (versionInfo.dwMinorVersion < 90)
			{
				strcpy(strInfo, "");
				strcpy(strInfo, WINDOWS_98);
			}
			else
			{
				strcpy(strInfo, "");
				strcpy(strInfo, WINDOWS_ME);
			}
		}

		strcpy(m_strPlatform, strInfo);
			
		// For win95, build number is low order word
		m_BuildNumber = (unsigned long)(LOBYTE(LOWORD(versionInfo.dwBuildNumber)));
	}
	else if (versionInfo.dwPlatformId == VER_PLATFORM_WIN32s)
	{
		//if (LoadString (GetModuleHandle(NULL), WINDOWS_32S, szBuffer, 
		//				sizeof (szBuffer)))
		//{
			strcpy(m_strPlatform, WINDOWS_32S);
	//	}
	//	else
	//	{
	//		if (LoadString (GetModuleHandle(NULL), WINDOWS_31, szBuffer, 
	//			sizeof (szBuffer)))
	//		{
	//			m_strPlatform = szBuffer;
	//		}
	//	}
	}

	// Get service pack information.
	strcpy(m_strServicePack, versionInfo.szCSDVersion);

	m_bInformationObtained = true;

	return true;
}
