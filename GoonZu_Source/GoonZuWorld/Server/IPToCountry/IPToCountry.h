//---------------------------------
// 2006/4/13 최연준
//---------------------------------

#ifndef _IPTOCONTRY_H
#define _IPTOCONTRY_H

#include <Directives.h>
#include <vector>
using namespace std;

#include <Stdio.h>

#include "../../NLib/NUpdateTimer.h"

#define MAX_COUNTRYCODE_LENGTH 32

static const TCHAR* const CONST_COUNTRYCODE_JAPAN = TEXT("JP");
static const TCHAR* const CONST_COUNTRYCODE_CHINA = TEXT("CN");
static const TCHAR* const CONST_COUNTRYCODE_THAILAND = TEXT("TH");
static const TCHAR* const CONST_COUNTRYCODE_TAIWAN = TEXT("TW");
static const TCHAR* const CONST_COUNTRYCODE_KOREA = TEXT("KR");

#define MAX_IPTOCOUNTRY_COUNT	120000


class cltIPToCountryMember {
public:
	UI32 siStartIndex;
	UI32 siEndIndex;
	TCHAR szCountryCode_two[4];
	//TCHAR szCountryCode_name[MAX_COUNTRYCODE_LENGTH];

	int Compare(UI32 ip)
	{
		if( siStartIndex > ip )
			return -1;

		if( siEndIndex < ip )
			return 1;

		return 0;
	}

	void Set(cltIPToCountryMember* pData)
	{
		if(pData)	memcpy(this, pData, sizeof(*this));
	}
};

class cltIPToCountryMgr {
private :
	cltIPToCountryMember* pList;
	SI32 count;

	bool LoadIPToCountryData();
	cltIPToCountryMember* GetCountryCode_Common(TCHAR* ipaddr);

public:

	cltIPToCountryMgr();
	~cltIPToCountryMgr();

	TCHAR* GetCountryCode(TCHAR* ipaddr);
	TCHAR* GetCountryCode_two(TCHAR* ipaddr);
	TCHAR* GetCountryCode_name(TCHAR* ipaddr);

	static UI32 GetIPValueFromIPStr(TCHAR* ipaddr);
};

enum
{
	IPACCESSDENY_NONE,		// 등록 안됨
	IPACCESSDENY_ACCESS,	// 허용
	IPACCESSDENY_DENY		// 거부
};

#define MAX_ACCESSDENY_COUNT	2000

class cltAccessDenyMgr
{
private :
	cltIPToCountryMember* pList;
	SI32 count;

	bool LoadAccessDenyData();
	cltIPToCountryMember* GetAccessDeny_Common(TCHAR* ipaddr);

	NUpdateTimer	m_kUpdateTimer_Load;

public:

	cltAccessDenyMgr();
	~cltAccessDenyMgr();

	int GetAccessDeny(TCHAR* ipaddr);

	void Action();
};

#endif
