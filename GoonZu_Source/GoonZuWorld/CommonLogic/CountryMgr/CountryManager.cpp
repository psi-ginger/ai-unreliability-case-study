#include <CommonLogic.h>
#include "CountryManager.h"

extern cltCommonLogic* pclClient;

//----------------------------------------------
// cltCountryManager
//----------------------------------------------

cltCountryManager::cltCountryManager()
{
	for ( SI32 i = 0 ; i < MAX_COUNTRY ; i ++ )
	{
		siCountryUnique[i] = -1;				// 디폴트 -1
		siContinentUnique[i] = 0;				// 디폴트 0
		StringCchCopy( szCountryName[i] , MAX_COUNTRY_NAME, TEXT("") );
		StringCchCopy( szCountryCode[i] , 4, TEXT("") );
	}
}

cltCountryManager::~cltCountryManager()
{

}

void cltCountryManager::LoadTextFile( SI16 mode )
{
	NDataLoader	dataloader;
	BOOL bLoadFile=FALSE;

	//KHY - 0810 - 텍스트 리소스 암호화.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		if ( mode == COUNTRY_JAPAN )
		{
			bLoadFile = dataloader.LoadDataFromCryptFile(  TEXT("Data/JapanCity_Info.txt") );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromCryptFile(  TEXT("Data/CountryInfo.txt") );
		}

	}
	else
	{
		if ( mode == COUNTRY_JAPAN )
		{
			bLoadFile = dataloader.LoadDataFromFile(  TEXT("Data/JapanCity_Info.txt") );
		}
		else
		{
			bLoadFile = dataloader.LoadDataFromFile(  TEXT("Data/CountryInfo.txt") );
		}
	}
						
	if (!bLoadFile)
	{
		MsgBox(TEXT("Not LoadFromFile CountryInfo"),TEXT("Error"));
	}

	SI32 countryUnique = 0;
	SI32 continentUnique = 0;
	TCHAR countryName[MAX_COUNTRY_NAME] = TEXT("");
	TCHAR countryCode[4] = TEXT("");

	NDataTypeInfo pDataTypeInfo[] =
	{
		NDATA_INT32, &countryUnique, 4,
		NDATA_MBSTRING, countryName, MAX_COUNTRY_NAME,
		NDATA_MBSTRING, countryCode, 4,
		NDATA_INT32, &continentUnique, 4, 
		0, 0, 0
	};

	dataloader.RegDataType( pDataTypeInfo );

	SI32 index = 0;
	while( !dataloader.IsEndOfData() ) 
	{
		if( dataloader.ReadData() )
		{
			siCountryUnique[index] = countryUnique;
			siContinentUnique[index] = continentUnique;
			StringCchCopy( szCountryName[index], 32, countryName );
			StringCchCopy( szCountryCode[index], 4, countryCode );
		}
		index++;
	}
}
SI32 cltCountryManager::GetUniqueFromCountry( TCHAR* country )
{
	SI16 i;
	for( i=0 ; i< MAX_COUNTRY ; i++)
	{
		if ( _tcscmp( country, szCountryName[i] ) == 0 )
			return siCountryUnique[i];
	}

	return -1;
}
TCHAR* cltCountryManager::GetCountryFromUnique( SI32 unique )
{
	SI16 i;
	for( i=0 ; i< MAX_COUNTRY ; i++)
	{
		if ( siCountryUnique[i] == unique )
			return szCountryName[i];
	}

	return TEXT("");
}
SI32 cltCountryManager::GetContinentFromCountry( SI32 unique )
{
	SI16 i;
	for ( i=0; i<MAX_COUNTRY ; i++ )
	{
		if( siCountryUnique[i] == unique )
			return siContinentUnique[i];
	}
	return -1;
}
