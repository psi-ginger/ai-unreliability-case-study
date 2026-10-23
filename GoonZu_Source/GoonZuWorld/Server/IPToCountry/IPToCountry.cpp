#include "IPToCountry.h"
#include "TextMgr\TextMgr.h"
#include "EtcTool\EtcTool.h"

cltIPToCountryMgr::cltIPToCountryMgr()
{
	pList = new cltIPToCountryMember[MAX_IPTOCOUNTRY_COUNT];
	count = 0;

	if(pList)	ZeroMemory(pList, sizeof(cltIPToCountryMember) * MAX_IPTOCOUNTRY_COUNT);

	LoadIPToCountryData();
}

cltIPToCountryMgr::~cltIPToCountryMgr()
{
	if(pList)		delete[] pList;
}

bool cltIPToCountryMgr::LoadIPToCountryData()
{
	if(pList == NULL)		return false;

	FILE * fp = NULL ;

	fp = _tfopen(TEXT("ServerInfo\\iptocountry.dat"), TEXT("r"));
	if ( fp == NULL ) 
	{
		MsgBox(TEXT("Error"), TEXT("Can't open iptocountry.dat"));
		return false ;
	}

	TCHAR strLine[1024] = TEXT("");

	while( _fgetts(strLine, 1024, fp) != NULL)
	{
		ParseText( strLine );

		if(count >= MAX_IPTOCOUNTRY_COUNT)
		{
			MsgBox(TEXT("Error"), TEXT("Can't Load iptocountry.dat"));
			break;
		}
		pList[count].siStartIndex = _tstoi( tokens[0] );
		pList[count].siEndIndex = _tstoi( tokens[1] );
		StringCchCopy(pList[count].szCountryCode_two, 3, tokens[2]);
		//StringCchCopy(pList[count].szCountryCode_name, MAX_COUNTRYCODE_LENGTH-1, tokens[3]);

		count++;
	}

	fclose(fp);

	return true;
    
}

UI32 cltIPToCountryMgr::GetIPValueFromIPStr(TCHAR* ipaddr)
{
	TCHAR buf[256];
	UI32 ip_a, ip_b, ip_c, ip_d;
	UI32 ip_value = 0;

	StringCchCopy(buf, 256, ipaddr);

	_stscanf(buf, TEXT("%d.%d.%d.%d"), &ip_a, &ip_b, &ip_c, &ip_d);

	ip_value = (ip_a * 256 * 256 * 256)+ (ip_b * 256 * 256) + (ip_c * 256) + ip_d;

	return ip_value;

}

TCHAR* cltIPToCountryMgr::GetCountryCode(TCHAR* ipaddr)
{
	return GetCountryCode_two(ipaddr);
}

cltIPToCountryMember* cltIPToCountryMgr::GetCountryCode_Common(TCHAR* ipaddr)
{
	UI32 ip_value = GetIPValueFromIPStr(ipaddr);

	int low, upper;
	int x = -1;
	bool bfind = false;

	low = 0;
	upper = count - 1;

	while(low<=upper)
	{
		x = (low+upper)/2;

		int com = pList[x].Compare(ip_value);

		if( com == 0 )
		{
			bfind = true;
			break;
		}
		else if ( com > 0 )
		{
			low = x + 1;
		}
		else if ( com < 0 )
		{
			upper = x - 1 ;
		} 
	}

	if( bfind && x >= 0 )
		return &pList[x];

	return NULL;
}


TCHAR* cltIPToCountryMgr::GetCountryCode_two(TCHAR* ipaddr)
{
	cltIPToCountryMember *ptr = GetCountryCode_Common(ipaddr);
	if (ptr)
		return ptr->szCountryCode_two;
	else
		return NULL;
}

TCHAR* cltIPToCountryMgr::GetCountryCode_name(TCHAR* ipaddr)
{
	//cltIPToCountryMember *ptr = GetCountryCode_Common(ipaddr);
	//if (ptr)
	//	return ptr->szCountryCode_name;
	//else
	return NULL;
}

//=================================================================================================
// cltAccessDenyMgr
//=================================================================================================
cltAccessDenyMgr::cltAccessDenyMgr()
{
	pList = new cltIPToCountryMember[MAX_ACCESSDENY_COUNT];
	count = 0;

	if(pList)	ZeroMemory(pList, sizeof(cltIPToCountryMember) * MAX_ACCESSDENY_COUNT);

//	m_kUpdateTimer_Load.Init(5 * 60 * 1000);	// 5분 간격으로 AccessDeny 테이블을 다시 읽는다.
//	m_kUpdateTimer_Load.SetActive(true, ::GetTickCount());

	LoadAccessDenyData();
}

cltAccessDenyMgr::~cltAccessDenyMgr()
{
	if(pList)		delete[] pList;
}

int ip_compare( const void *arg1, const void *arg2 )
{
	cltIPToCountryMember*	pParam1 = (cltIPToCountryMember*)arg1;
	cltIPToCountryMember*	pParam2 = (cltIPToCountryMember*)arg2;

	if( pParam1->siStartIndex > pParam2->siStartIndex )
		return 1;
	else if( pParam1->siStartIndex == pParam2->siStartIndex)
		return 0;

	return -1;
}


bool cltAccessDenyMgr::LoadAccessDenyData()
{
	if(pList == NULL)		return false;

	FILE * fp = NULL ;

	fp = _tfopen(TEXT("ServerInfo\\accessdeny.dat"), TEXT("r"));
	if ( fp == NULL ) 
	{
		MsgBox(TEXT("Error"), TEXT("Can't open accessdeny.dat"));
		return false ;
	}

	TCHAR strLine[1024] = TEXT("");

	while( _fgetts(strLine, 1024, fp) != NULL)
	{
		ParseText( strLine );

		if(count >= MAX_ACCESSDENY_COUNT)
		{
			MsgBox(TEXT("Error"), TEXT("Can't Load accessdeny.dat"));
			break;
		}

		pList[count].siStartIndex = cltIPToCountryMgr::GetIPValueFromIPStr( tokens[0] );
		pList[count].siEndIndex = cltIPToCountryMgr::GetIPValueFromIPStr( tokens[1] );
		StringCchCopy(pList[count].szCountryCode_two, 3, tokens[2]);
		//StringCchCopy(pList[count].szCountryCode_name, MAX_COUNTRYCODE_LENGTH-1, tokens[3]);

		count++;
	}

	fclose(fp);

	qsort( (void *)pList, (size_t)count, sizeof(cltIPToCountryMember), ip_compare );
	
	return true;
}

cltIPToCountryMember* cltAccessDenyMgr::GetAccessDeny_Common(TCHAR* ipaddr)
{
	if(pList == NULL)			return NULL;
	if(count == 0)				return NULL;

	UI32 ip_value = cltIPToCountryMgr::GetIPValueFromIPStr(ipaddr);

	int low, upper;
	int x = -1;
	bool bfind = false;

	low = 0;
	upper = count - 1;

	while(low<=upper)
	{
		x = (low+upper)/2;

		int com = pList[x].Compare(ip_value);

		if( com == 0 )
		{
			bfind = true;
			break;
		}
		else if ( com > 0 )
		{
			low = x + 1;
		}
		else if ( com < 0 )
		{
			upper = x - 1 ;
		} 
	}

	if( bfind && x >= 0 )
		return &pList[x];

	return NULL;
}

int cltAccessDenyMgr::GetAccessDeny(TCHAR* ipaddr)
{
	cltIPToCountryMember *ptr = GetAccessDeny_Common(ipaddr);
	if (ptr)
	{
		if( strncmp(ptr->szCountryCode_two, "A", 1) == 0 )
			return IPACCESSDENY_ACCESS;
		else
			return IPACCESSDENY_DENY;
	}
	else
		return IPACCESSDENY_NONE;
}

void cltAccessDenyMgr::Action()
{
	// ServerAction 에서 제어함.
	//if(m_kUpdateTimer_Load.IsTimed(::GetTickCount()) == false)		return;

	if(pList)		delete[] pList;

	pList = new cltIPToCountryMember[MAX_IPTOCOUNTRY_COUNT];
	count = 0;

	if(pList)	ZeroMemory(pList, sizeof(cltIPToCountryMember) * MAX_IPTOCOUNTRY_COUNT);

	LoadAccessDenyData();
}