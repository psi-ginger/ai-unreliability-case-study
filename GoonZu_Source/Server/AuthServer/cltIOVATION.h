#ifndef _CLTIOVATION_H_
#define _CLTIOVATION_H_

#include "../../NLib/NTCHARString.h"

typedef int (*InitIoVation)( const char* pszSubScriberID, const char* pszAdminCode, const char* pszAdminPassword, const char* pszType );
typedef int (*ShutdownIoVation)( void );
typedef int (*CheckTrans)( const char* pszURL, const char* pszID, const char* pszBlackBox, const char* pszIP );

typedef const char* (*GetLastErrorStr)( void );
typedef const char* (*GetReasonStr)( void );
typedef const char* (*GetDeviceAliasStr)( void );
typedef const char* (*GetTrackingNumberStr)( void );

class cltIOVATION
{
public:
	cltIOVATION();
	~cltIOVATION();

	bool CheckTransantion(const char* pszID, const char* pszBlackBox, const char* pszIP );

protected:
	HMODULE				m_hIOVATIONDLL;

	InitIoVation		m_InitFunc;
	ShutdownIoVation	m_ShutdownFunc;
	CheckTrans			m_CheckTransactionFunc;

	GetLastErrorStr			m_GetLastError;
	GetReasonStr			m_GetReason;
	GetDeviceAliasStr		m_GetDeviceAlias;
	GetTrackingNumberStr	m_GetTrackingNumber;

	NTCHARString128		m_kSubScriberID;
	NTCHARString128		m_kAdminCode;
	NTCHARString128		m_kAdminPassword;
	NTCHARString256		m_kURL;
	NTCHARString256		m_kType;
};

#endif