#include "stdafx.h"
#include "Patch.h"
#include "RegistryMgr.h"

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CRegistryMgr::CRegistryMgr()
//	Desc : 생 성 자
//	Date : 2003. 06. 13
//	Update :
//--------------------------------------------------------------------------------------------
CRegistryMgr::CRegistryMgr()
{

}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - CRegistryMgr::~CRegistryMgr()
//	Desc : 소 멸 자
//	Date : 2003. 06. 13
//	Update :
//--------------------------------------------------------------------------------------------
CRegistryMgr::~CRegistryMgr()
{
	if(m_hKey != NULL)
	{
		RegCloseKey( m_hKey );
		m_hKey = NULL;
	}
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CRegistryMgr::RegistryCreateKey( HKEY hKey, LPCTSTR lpKey, REGSAM samDesired )
//	Desc : RegCreateKeyEx 실행
//	Date : 2003. 06. 13
//	Update :
//--------------------------------------------------------------------------------------------
bool CRegistryMgr::RegistryCreateKey( HKEY hKey, LPCTSTR lpKey, REGSAM samDesired )
{
	if(m_hKey != NULL)
	{
		RegCloseKey( m_hKey );
		m_hKey = NULL;
	}
	if ( ::RegCreateKeyEx( hKey, lpKey, 0, NULL, REG_OPTION_NON_VOLATILE, samDesired, NULL, &m_hKey, &m_dwDisp ) != ERROR_SUCCESS )
	{
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CRegistryMgr::GetRegReadInt( LPCTSTR lpValueName, UI32 *puiReadValue )
//	Desc : 레지스트리에서 정수형 값을 읽어옮.
//	Date : 2003. 06. 13
//	Update :
//--------------------------------------------------------------------------------------------
bool CRegistryMgr::GetRegReadInt( LPCTSTR lpValueName, UI32 *puiReadValue )
{
	if(m_hKey == NULL)								return false;

	DWORD Size;

	Size = sizeof(LONG);

	if ( ::RegQueryValueEx( m_hKey, lpValueName, 0, NULL, (LPBYTE)puiReadValue, &Size ) != ERROR_SUCCESS )
	{
		RegCloseKey( m_hKey );
		m_hKey = NULL;
		return false;
	}

	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CRegistryMgr::SetRegWriteInt( LPCTSTR lpValueName, UI32 uiWriteValue )
//	Desc : 레지스트리에 정수형 값을 입력.
//	Date : 2003. 06. 13
//	Update :
//--------------------------------------------------------------------------------------------
bool CRegistryMgr::SetRegWriteInt( LPCTSTR lpValueName, UI32 uiWriteValue )
{
	if(m_hKey == NULL)								return false;

	if ( ::RegSetValueEx( m_hKey, lpValueName, 0, REG_DWORD, (LPBYTE)&uiWriteValue, sizeof( UINT ) ) != ERROR_SUCCESS )
	{
		RegCloseKey( m_hKey );
		m_hKey = NULL;
		return false;
	}
	
	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CRegistryMgr::GetRegReadString( LPCTSTR lpValueName, CC08 *strReadValue, DWORD dwSize )
//	Desc : 레지스트리에서 문자열 값을 읽어옮.
//	Date : 2003. 06. 13
//	Update :
//--------------------------------------------------------------------------------------------
bool CRegistryMgr::GetRegReadString( LPCTSTR lpValueName, CC08 *strReadValue, DWORD dwSize )
{
	if(m_hKey == NULL)								return false;

	DWORD ldwSize = dwSize;

	if ( ::RegQueryValueEx( m_hKey, lpValueName, 0, NULL, (LPBYTE)strReadValue, &ldwSize ) != ERROR_SUCCESS )
	{
		RegCloseKey( m_hKey );
		m_hKey = NULL;
		return false;
	}
	
	return true;
}

//--------------------------------------------------------------------------------------------
//	Name : Sang Min, Lee (duryzip@hotmail.com) - bool CRegistryMgr::SetRegWriteString( LPCTSTR lpValueName, CC08 *strWriteValue )
//	Desc : 레지스트리에서 문자열 값을 입력.
//	Date : 2003. 06. 13
//	Update :
//--------------------------------------------------------------------------------------------
bool CRegistryMgr::SetRegWriteString( LPCTSTR lpValueName, CC08 *strWriteValue )
{
	if(m_hKey == NULL)								return false;

	if ( ::RegSetValueEx( m_hKey, lpValueName, 0, REG_SZ, (LPBYTE)strWriteValue, strlen(strWriteValue) + 1 ) != ERROR_SUCCESS )
	{
		RegCloseKey( m_hKey );
		m_hKey = NULL;
		return false;
	}

	return true;
}