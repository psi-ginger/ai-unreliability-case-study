#include "stdafx.h"
#include "VillageManager.h"
#define MAX_VILLAGE_COUNT 100
#define MAX_ITEM_FILE 100


CVillageManager::CVillageManager()
{
	strcpy(village_name,"");

}


CVillageManager::~CVillageManager()
{
	strcpy(village_name,"");

}



char* CVillageManager::Set(char *VillageUnique)
{
	FindVillage(VillageUnique);
	return village_name;	
    
}

void CVillageManager::FindVillage(char *VillageUnique)
{
	char buffer[256]="";

	BYTE	szGoonzuPath[MAX_PATH] = "";
	char	szVillageInfo[512] = "";
	DWORD	dwSize = sizeof(szGoonzuPath);
	HKEY	hKey;
	LONG	lReturn;
	CString	szError;

	lReturn = RegOpenKey(HKEY_CURRENT_USER, "Software\\Intizen\\Goonzu", &hKey);
	if(lReturn != ERROR_SUCCESS)
	{
		szError.Find("레지스트리를 열 수 없습니다\nErrorCode : %d", lReturn);
		AfxMessageBox(szError);
		return;
	}
	lReturn = RegQueryValueEx(hKey, "PATH", NULL, NULL, szGoonzuPath, &dwSize);
	if(lReturn != ERROR_SUCCESS)
	{
		szError.Find("군주가 설치된 폴더를 찾을 수 없습니다\nErrorCode : %d", lReturn);
		AfxMessageBox(szError);
		return;
	}

	RegCloseKey(hKey);

	sprintf(szVillageInfo, "%sVillage\\VillageInfo.txt", szGoonzuPath);

	FILE* fp = fopen(szVillageInfo, "rt");

	if(fp == NULL)
	{
		AfxMessageBox( buffer);		
		return ;
	}

	// 5줄은 무시한다.
	for(int i = 0;i < 6;i++)
		fgets(buffer, 1024, fp);


	i=0;
	while( fgets(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_VILLAGE_COUNT ) break ;

		char * szname;
		char * szkind;	

		szkind = strtok(buffer, "\t");
		strtok(NULL,"\t");
		szname = strtok(NULL,"\t");

		if ( !szkind || !szname ) break ;
		{
			// 입력이 숫자인경우 마을이름을, 문자인경우 마을유니크를 찾는다.
			if(IsNumber(VillageUnique) == true)
			{
				if(strcmp(VillageUnique,szkind)==0)
				{
					strcpy(village_name,szname);
					return;
				}		
			}
			else
			{
				if(strcmp(VillageUnique,szname)==0)
				{
					strcpy(village_name,szkind);
					return;
				}		
			}
		}		
		i ++ ;
	}

	fclose(fp);

}

bool CVillageManager::IsNumber(LPCTSTR pszText)
{
	for( int i = 0; i < lstrlen( pszText ); i++ )
		if( !_istdigit( pszText[ i ] ) )
			return false;

	return true;
}
