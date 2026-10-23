#ifndef __OSINFORMATION_H_
#define __OSINFORMATION_H_

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용중인 OS 정보 얻는 Class Type COSInformation
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
#define    WINDOWS_95          "Windows 95"
#define    WINDOWS_98          "Windows 98"
#define    WINDOWS_ME          "Windows ME"
#define    WINDOWS_NT          "Windows NT"
#define    WINDOWS_NT_351      "Windows NT 351"
#define    WINDOWS_NT_40       "Windows NT 40"
#define    WINDOWS_2000        "Windows 2000"
#define    WINDOWS_XP          "Windows XP"
#define    WINDOWS_SERVER_2003 "Windows SERVER 2003"

#define    WIN2K_PROF          "Windows 2000 Professional"
#define    WIN2K_SERVER        "Windows 2000 Server"
#define    WIN2K_ADV_SERVER    "Windows 2000 Advanced Server"
#define    WIN2K_DATA_CENETER  "Windows 2000 Data Ceneter"
#define    WIN2K_DOMAIN_CONTROLLER "Windows 2000 Domain Controller"

#define    NT_WORKSTATION      "Windows NT Workstation"
#define    NT_SERVER           "Windows NT Server"
#define    NT_DOMAIN_CONTROLLER "Windows NT server - Domain Controller"
#define    NT_40               "4.0"
#define    NT_50               "5.0"
#define    NT_351              "3.51"
#define    WINDOWS_32S         "Win32s"
#define    WINDOWS_31          "Win 3.1"

#define    ERR_GETVERSION      "Failed to get OS version"
#define    NT_SP6              "Service Pack 6"
#define    SUITE_BACKOFFICE    "Microsoft Backoffice"
#define    SUITE_DATACENTER    "Data Center"
#define    SUITE_ENTERPRISE    "Advcanced Server"
#define    SUITE_SMALLBUSINESS "Microsoft Small Business Server "
#define    SUITE_SMALLBUSINESS_RESTRICTED "Microsoft Small Business Server - Restrictive Client License"
#define    SUITE_TERMINAL      "Terminal Server"
#define    SUITE_PERSONAL      "Workstation Personal "

#define    WINDOWS_WHISTLER    "Windows Whistler"
#define    NT_51               "5.1"
#define    WHISTLER_PROF       "Windows Whistler Professional"
#define    WHISTLER_SERVER     "Windows Whistler Server"

/**
leon add
* OS MAJORVERSION
*/
#define		WINDOWS_95_MAJORVERSION	"4"
#define		WINDOWS_98_MAJORVERSION	"4"
#define		WINDOWS_ME_MAJORVERSION	"4"
#define		WINDOWS_NT_351_MAJORVERSION	"3"
#define		WINDOWS_NT_40_MAJORVERSION	"4"
#define		WINDOWS_2000_MAJORVERSION	"5"
#define		WINDOWS_XP_MAJORVERSION	"5"
#define		WINDOWS_SERVER_2003_MAJORVERSION	"5"

/**
leon add
* OS MINORVERSION
*/
#define		WINDOWS_95_MINORVERSION	"0"
#define		WINDOWS_98_MINORVERSION	"10"
#define		WINDOWS_ME_MINORVERSION	"90"
#define		WINDOWS_NT_351_MINORVERSION	"51"
#define		WINDOWS_NT_40_MINORVERSION	"0"
#define		WINDOWS_2000_MINORVERSION	"0"
#define		WINDOWS_XP_MINORVERSION	"1"
#define		WINDOWS_SERVER_2003_MINORVERSION	"2"

#define		MAX_OSINFO_STRING_LENGTH 260
/////////////////////////////////////////////////////////////////////////////
// COSInformation
class COSInformation
{

public:
	COSInformation()
	{
		m_bInformationObtained = false;
		m_bIsWin95 = 0;
		m_BuildNumber = 0;
		m_strPlatform[0]=0;
		m_strSuites[0]=0;
		m_strMajorVersion[0]=0;
		m_strMinorVersion[0]=0;
		m_strDetailVersion[0]=0;
		m_strServicePack[0]=0;
	}

// OSInformation
public:
	// OS명 리턴함
	bool get_ClientOSName(char*  pstrVal, unsigned long MAX_BUFFER_LENGTH);

	bool get_ServicePack( char*  pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_MajorVersion(char*  pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_MinorVersion(char*  pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_Platform(char*  pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_IsWin95( unsigned long& Val);
	bool get_BuildNumber( unsigned long& Val);
	bool GetInformation ();

	unsigned long m_bIsWin95;
	unsigned long m_BuildNumber;

	char m_strPlatform[MAX_OSINFO_STRING_LENGTH];
	char m_strSuites[MAX_OSINFO_STRING_LENGTH];
	char m_strMajorVersion[MAX_OSINFO_STRING_LENGTH];
	char m_strMinorVersion[MAX_OSINFO_STRING_LENGTH];
	char m_strServicePack[MAX_OSINFO_STRING_LENGTH];

	bool m_bInformationObtained;
	
/**
leon add
* OS Detail Version
*/
	char m_strDetailVersion[MAX_OSINFO_STRING_LENGTH];

};

#endif //__OSINFORMATION_H_
