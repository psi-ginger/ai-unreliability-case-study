#include <string.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include "CPUInformation.h"

/////////////////////////////////////////////////////////////////////////////
// CCPUInformation

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Speed 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_CPUSpeed(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_CPUSpeed;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Revision 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_CPURevision(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_CPUINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_CPURevision);

		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Level 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_CPULevel(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_CPUINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_CPULevel);

		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Architecture 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_Architecture(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_CPUINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_strArchitecture);

		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Processor 타입 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_ProcessorType(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_CPUINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_strCPUType);

		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Vendor 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_Vendor(char* pstrVal, unsigned long MAX_BUFFER_LENGTH)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	if(MAX_BUFFER_LENGTH > MAX_CPUINFO_STRING_LENGTH)
	{
		strcpy(pstrVal, m_strVendor);

		return true;
	}
	else
		return false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Active Processor Mask 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_ActiveProcessorMask(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_ActiveMask;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Page Size 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_PageSize(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_PageSize;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU 프로세서 개수 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::get_NumberOfProcessors(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_NumberOfProcessors;

	return true;
}


//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU Information 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CCPUInformation::GetInformation ()
{
	bool hRes = true;
	unsigned long IsWin95 = 0;

	SYSTEM_INFO sysInfo;

	char tmpStr[MAX_CPUINFO_STRING_LENGTH];

	LONG result;
	HKEY hKey;
	TCHAR vendorData [64];
	unsigned long data, dataSize;
	LPTSTR lpErrDesc = NULL;

	if (m_bInformationObtained)
	{
		return true;
	}

	// Get the processor speed info.
	result = ::RegOpenKeyEx (HKEY_LOCAL_MACHINE,
		"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);

	// Check if the function has succeeded.
	if (result == ERROR_SUCCESS)
	{
		dataSize = MAX_PATH;
		result = ::RegQueryValueEx (hKey, _T("~MHz"), NULL, NULL,
			(LPBYTE)&data, &dataSize);

		if (result != ERROR_SUCCESS)
		{
			return false;
		}
		
		m_CPUSpeed = data;

		dataSize = sizeof (vendorData);

		result = ::RegQueryValueEx (hKey, _T("VendorIdentifier"), NULL, NULL,
			(LPBYTE)vendorData, &dataSize);

		if (result != ERROR_SUCCESS)
		{
			return false;
		}
		
		strcpy(m_strVendor, vendorData);
	}
	else
	{
		return false;
	}

	// Make sure to close the reg key

	RegCloseKey (hKey);

	// Get the hardware information

	::GetSystemInfo (&sysInfo);

	// Lets check the processor type first

	if (sysInfo.dwProcessorType  == PROCESSOR_INTEL_386)
	{
		strcpy(m_strCPUType, _T ("Intel 386"));
	}
	else if (sysInfo.dwProcessorType  == PROCESSOR_INTEL_486)
	{
		strcpy(m_strCPUType, _T ("Intel 486"));
	}
	else if (sysInfo.dwProcessorType  == PROCESSOR_INTEL_PENTIUM)
	{
		strcpy(m_strCPUType, _T ("Intel Pentium"));
	}
	else if (sysInfo.dwProcessorType  == PROCESSOR_MIPS_R4000)
	{
		// only for NT
		strcpy(m_strCPUType, _T ("MIPS"));
	}
	else if (sysInfo.dwProcessorType  == PROCESSOR_ALPHA_21064)
	{
		// only for NT
		strcpy(m_strCPUType, _T ("Alpha"));
	}
	else
	{
		strcpy(m_strCPUType, _T ("Unknown"));
	}

	// check number of processors

	m_NumberOfProcessors = sysInfo.dwNumberOfProcessors;

	// Get the information if we have Win95 OS.
	hRes = m_OSInfo.get_IsWin95 (IsWin95);
	if (FAILED (hRes)) {
		return hRes;
	}

	// Check the architecture type and processor level

	// Windows 95 doesn't use processor level
	strcpy(m_CPURevision, _T ("Unknown"));

	if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
	{
		strcpy(m_strArchitecture, _T ("Pentium"));
		switch (sysInfo.wProcessorLevel)
		{
			case 3:
				strcpy(m_CPULevel, _T ("Intel 80386"));
				if (!IsWin95)
				{
					sprintf(m_CPURevision, "%dA%d", HIBYTE (sysInfo.wProcessorRevision), LOBYTE (sysInfo.wProcessorRevision));
				}
				break;
			case 4:
				strcpy(m_CPULevel, _T ("Intel 80486"));
				if (!IsWin95)
				{
					sprintf(m_CPURevision, "%dA%d", HIBYTE (sysInfo.wProcessorRevision), LOBYTE (sysInfo.wProcessorRevision));
				}
				break;
			case 5:
				// Check if the MMX instruction set is availbale or not.

				if (IsProcessorFeaturePresent (PF_MMX_INSTRUCTIONS_AVAILABLE))
				{
					strcpy(m_CPULevel, _T ("Pentium MMX"));
				}

				if (!IsWin95)
				{
					sprintf(m_CPURevision, "Model %d, Stepping %d", HIBYTE (sysInfo.wProcessorRevision), LOBYTE (sysInfo.wProcessorRevision));
				}
				break;
			case 6:
				strcpy(m_CPULevel, _T ("Pentium (II/Pro)"));
				break;
		}
	}
	else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_MIPS)
	{
		strcpy(m_strArchitecture, _T ("MIPS"));

		if (sysInfo.wProcessorLevel == 0004)
		{
			strcpy(m_CPULevel, _T ("MIPS R4000"));
		}
		else 
		{
			strcpy(m_CPULevel, _T ("Unknown"));
		}
	}
	else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_ALPHA)
	{
		strcpy(m_strArchitecture, _T ("Alpha"));

		sprintf(m_CPULevel, "%s%d", m_strArchitecture, sysInfo.wProcessorLevel);
	}
	else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_PPC)
	{
		strcpy(m_strArchitecture, _T ("PPC"));

		switch (sysInfo.wProcessorLevel)
		{
			case 1:
				strcpy(m_CPULevel, _T ("PPC 601"));
				break;
			case 3:
				strcpy(m_CPULevel, _T ("PPC 603"));
				break;
			case 4:
				strcpy(m_CPULevel, _T ("PPC 604"));
				break;
			case 6:
				strcpy(m_CPULevel, _T ("PPC 603+"));
				break;
			case 9:
				strcpy(m_CPULevel, _T ("PPC 604+"));
				break;
			case 20:
				strcpy(m_CPULevel, _T ("PPC 620"));
				break;
		}
	}
	else if (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_UNKNOWN)
	{
		strcpy(m_strArchitecture, _T ("Unknown"));
	}
    
	strcpy(tmpStr, "");

	// Check page size
	m_PageSize = sysInfo.dwPageSize;

	// Get active processor mask
	// It represent how many processors are active (?? I am not sure)
	m_ActiveMask = sysInfo.dwActiveProcessorMask;

	if(hRes)
		m_bInformationObtained = true;
	else
		m_bInformationObtained = false;

	return hRes;
}
