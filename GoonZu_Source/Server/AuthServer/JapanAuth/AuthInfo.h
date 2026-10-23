#ifndef _AUTHINFO_H_
#define _AUTHINFO_H_

#include <stdio.h>

enum
{
	 AUTH_TYPE_NONE
	,AUTH_TYPE_GAMEPOT
	,AUTH_TYPE_GAMANIA
	,AUTH_TYPE_USA
};

class AuthInfo
{
public:
	AuthInfo()
	{
		Init();
	}

	AuthInfo(AuthInfo &v)
	{
		memcpy(this, &v, sizeof(*this));
	}

	~AuthInfo()
	{
	}

	void Init()
	{
		ZeroMemory(m_szID,		 sizeof(m_szID));
		ZeroMemory(m_szPW,		 sizeof(m_szPW));
		ZeroMemory(m_szIP,		 sizeof(m_szIP));
		ZeroMemory(m_szMAC,		 sizeof(m_szMAC));
		ZeroMemory(m_Authstring, sizeof(m_Authstring));

		m_nAuthType	= AUTH_TYPE_NONE;
	}

	void Set(TCHAR* pszID, TCHAR * pszPW, TCHAR* pszIP, TCHAR* pszMAC)
	 {
		if(pszID)		strncpy(m_szID,  pszID,  sizeof(m_szID));
		if(pszPW)		strncpy(m_szPW,  pszPW,  sizeof(m_szPW));
		if(pszIP)		strncpy(m_szIP,  pszIP,  sizeof(m_szIP));
		if(pszMAC)		strncpy(m_szMAC, pszMAC, sizeof(m_szMAC));
	}

	void SetUSAAuthString(TCHAR* _pAuthstring)
	{
		if( _pAuthstring ) 
			memcpy(m_Authstring, _pAuthstring, sizeof(m_Authstring));
	}

public:
	TCHAR	m_szID[32];
	TCHAR	m_szPW[32];
	TCHAR	m_szIP[32];
	TCHAR	m_szMAC[32];

	TCHAR	m_Authstring[ 4096 ];	//[진성] USA 인증 스트링.

	int		m_nAuthType;

	UINT	m_usPacketUnique;
	UINT	m_usFrontCharID;
};

void TRACEF(TCHAR* pszOut, ...);

#endif