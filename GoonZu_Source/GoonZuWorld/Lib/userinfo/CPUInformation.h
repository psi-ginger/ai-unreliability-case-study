#ifndef __CPUINFORMATION_H_
#define __CPUINFORMATION_H_

#define MAX_CPUINFO_STRING_LENGTH	260

#include "OSInformation.h"

/////////////////////////////////////////////////////////////////////////////
//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : CPU 정보 얻는 Class Type CMemoryInformation
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
// CCPUInformation
class CCPUInformation
{

public:
	CCPUInformation()
	{
		m_bInformationObtained = false;

		m_strVendor[0] = 0;
		m_strCPUType[0] = 0;
		m_strArchitecture[0] = 0;;
		m_CPULevel[0] = 0;
		m_CPURevision[0] = 0;
		m_NumberOfProcessors = 0;
		m_CPUSpeed = 0;
		m_PageSize = 0;
		m_ActiveMask = 0;
	}

// CPUInformation
public:
	
	bool get_CPUSpeed(unsigned long& Val);	// CPU SPEED

	bool get_CPURevision(char* pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_CPULevel(char* pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_Architecture(char* pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_ProcessorType(char* pstrVal, unsigned long MAX_BUFFER_LENGTH);
	bool get_Vendor(char* pstrVal, unsigned long MAX_BUFFER_LENGTH);

	bool get_ActiveProcessorMask(unsigned long& Val);
	bool get_PageSize(unsigned long& Val);
	bool get_NumberOfProcessors(unsigned long& Val);

	bool GetInformation ();

private:
	
	bool m_bInformationObtained;
	
	char m_strVendor[MAX_CPUINFO_STRING_LENGTH];
	char m_strCPUType[MAX_CPUINFO_STRING_LENGTH];
	char m_strArchitecture[MAX_CPUINFO_STRING_LENGTH];
	char m_CPULevel[MAX_CPUINFO_STRING_LENGTH];
	char m_CPURevision[MAX_CPUINFO_STRING_LENGTH];

	unsigned long m_NumberOfProcessors;
	unsigned long m_CPUSpeed;
	unsigned long m_PageSize;
	unsigned long m_ActiveMask;

	COSInformation m_OSInfo;
};

#endif //__CPUINFORMATION_H_
