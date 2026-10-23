#pragma once

#include <Directives.h>

#define MAX_COUNTRY			256
#define MAX_COUNTRY_NAME	32

#define CONTINENT_ASIA				1
#define CONTINENT_EUROPE			2
#define CONTINENT_NORTHAMERICA		3
#define CONTINENT_SOUTHAMERICA		4
#define CONTINENT_AFRICA			5
#define CONTINENT_OCEANIA			6

#define COUNTRY_WORLD				1
#define COUNTRY_JAPAN				2



class cltCountryManager
{
private:

public:
	cltCountryManager();
	~cltCountryManager();

#ifdef _SAFE_MEMORY
	NSafeTArray<SI32, MAX_COUNTRY>		siCountryUnique;
	NSafeTArray<SI32, MAX_COUNTRY>		siContinentUnique;
#else
	SI32		siCountryUnique[MAX_COUNTRY];
	SI32		siContinentUnique[MAX_COUNTRY];
#endif
	TCHAR		szCountryName[MAX_COUNTRY][MAX_COUNTRY_NAME];
	TCHAR		szCountryCode[MAX_COUNTRY][4];

	void		LoadTextFile( SI16 mode );
	SI32		GetUniqueFromCountry( TCHAR* country );
	TCHAR*		GetCountryFromUnique( SI32 unique );
	SI32		GetContinentFromCountry( SI32 unique );
};
