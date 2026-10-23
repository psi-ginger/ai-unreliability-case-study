#ifndef _REGISTRYMGR_H_
#define _REGISTRYMGR_H_

#include <Directives.h>
#include <stdlib.h>
#include <stdio.h>

#define		CHANGE_PATCH_VERSION		90			// CHANGE_PATCH_VERSION번 보다 버전이 낮다면 Patch1.exe 가 있는 것을 조사해서 있다면, Patch.exe 를 지우고 Patch1.exe 를 Patch.exe 로 이름을 바꿔줌
													// 주의할 것은 반드시 바꿀 이름의 파일명이 Patch1.exe 여야만 한다는 것이다.

class CRegistryMgr  
{
public:
	CRegistryMgr(SI32 servicearea=1);
	~CRegistryMgr();

	bool	RegistryCreateKey( HKEY hKey, LPCTSTR lpKey, REGSAM samDesired );

	bool	GetRegReadInt( LPCTSTR lpValueName, UI32* puiReadValue );
	bool	SetRegWriteInt( LPCTSTR lpValueName, UI32 uiWriteValue );

	bool	GetRegReadString( LPCTSTR lpValueName, TCHAR *strReadValue, DWORD dwSize );
	bool	SetRegWriteString( LPCTSTR lpValueName, TCHAR *strWriteValue );
	
	
	//---------------------------------------------------------------------------------
	UI32	GetVersion();							// Version 값 레지스트리에서 읽어옮
	TCHAR*	GetPath();								// 게임 경로(Path) 값 레지스트리에서 읽어옮
	void	WriteVersion( UI32 uiVersion );			// Version 값 레지스트리에 쓰기
	//---------------------------------------------------------------------------------

	
	void	RegistryCloseKey();

private:
	HKEY		m_hKey;
	DWORD		m_dwDisp;

	TCHAR		m_strPath[ _MAX_PATH ];
	UI32		m_uiVersion;

};

#endif