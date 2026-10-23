#ifndef _REGISTRYMGR_H_
#define _REGISTRYMGR_H_

#include "TypeDefinition.h"

class CRegistryMgr  
{
public:
	CRegistryMgr();
	virtual ~CRegistryMgr();

	bool RegistryCreateKey( HKEY hKey, LPCTSTR lpKey, REGSAM samDesired );

	bool GetRegReadInt( LPCTSTR lpValueName, UI32* puiReadValue );
	bool SetRegWriteInt( LPCTSTR lpValueName, UI32 uiWriteValue );

	bool GetRegReadString( LPCTSTR lpValueName, CC08 *strReadValue, DWORD dwSize );
	bool SetRegWriteString( LPCTSTR lpValueName, CC08 *strWriteValue );

private:
	HKEY		m_hKey;
	DWORD		m_dwDisp;

};

#endif