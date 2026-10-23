#include <string.h>
#include <windows.h>
#include "MemoryInformation.h"
/////////////////////////////////////////////////////////////////////////////
// CMemoryInformation

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 설치된 메모리 용량 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CMemoryInformation::get_RAMInstalled(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_RAMInstalled;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용 가능 메모리 용량 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CMemoryInformation::get_MemoryAvailable(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_MemoryAvailable;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 사용되고 있는 가상 메모리 용량 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CMemoryInformation::get_VirtualMemory(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_VirtualMemory;

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 로드된 메모리 용량 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CMemoryInformation::get_MemoryLoad(unsigned long& Val)
{
	bool hRes = true;
	if ((hRes = GetInformation ()) != true)
	{
		return hRes;
	}

	Val = m_MemoryLoad;

	return true;
}

/**
leon add
* memory status는 체크시 마다 바뀔 수 있으므로
* reset function 만듦
*/
void CMemoryInformation::ResetMemoryInfo()
{
    m_bInformationObtained = false;
}

//--------------------------------------------------------------------------------------------
//	Name : 정용래
//	Desc : 메모리 정보 얻기
//	Date : 2005. 01. 31
//	Update : 
//--------------------------------------------------------------------------------------------
bool CMemoryInformation::GetInformation ()
{
	bool hRes = true;
	MEMORYSTATUS memoryStatus;

	if (m_bInformationObtained)
	{
		return true;
	}

	memset (&memoryStatus, sizeof (MEMORYSTATUS), 0);
	memoryStatus.dwLength = sizeof (MEMORYSTATUS);

	// The function below doesn't return a value.

	::GlobalMemoryStatus (&memoryStatus);

	m_MemoryLoad = memoryStatus.dwMemoryLoad;
	m_RAMInstalled = memoryStatus.dwTotalPhys;
	m_MemoryAvailable = memoryStatus.dwAvailPhys;
	m_VirtualMemory = memoryStatus.dwTotalVirtual;

	m_bInformationObtained = true;

	return true;
}