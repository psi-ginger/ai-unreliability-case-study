#include "stdafx.h"
#include "ItemManager.h"
#define MAXFILE_COUNT 64
#define MAX_ITEM_FILE 100


CItemManager::CItemManager()
{

}


CItemManager::~CItemManager()
{

}


bool CItemManager::LoadFile(char* itemUnique)
{
	//char finditem[128]=" ";

	//FILE *fp;
	//fp= fopen(FileName,"rt");

	//fclose(fp);

	char buffer[256]="";

	char FileNameBuf[MAXFILE_COUNT][128];

		
	FILE* fp = fopen("C:\\Program Files\\Intizen\\GoonZu\\Item\\Data\\ItemDataManager.txt", "rt");
	if(fp == NULL)
	{
		AfxMessageBox( "not ItemManagerFile" );		
		return false;
	}

	// 4줄은 무시한다.
	for(int i = 0;i < 4;i++)
		fgets(buffer, 1024, fp);

	for ( i = 0 ; i < MAXFILE_COUNT ; i ++ )
	{
		strcpy(FileNameBuf[i],"");	
	}

	
	i = 0 ;

	while( fgets(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAXFILE_COUNT ) break ;

		strcpy(FileNameBuf[i],buffer);
		i ++ ;
	}

	fclose(fp);

	 
	for(i = 0 ; i < MAXFILE_COUNT ; i ++ )
	{
		if(FileNameBuf[i]!=NULL && strcmp(FileNameBuf[i],"")!=0)
		{
            FindUnique(FileNameBuf[i],itemUnique);
		}
	}
		
	
	return false;

	


}


void CItemManager::Set(char *Binary, char *itemUnique)
{

	int flag=0;
	LoadFile("1234");
    
}

void CItemManager::FindUnique(char* FileName,char *itemUnique)
{
	char buffer[256]="";
	char ItemInfo[MAX_ITEM_FILE][256];
	
	sprintf(buffer,"C:/Program Files/Intizen/GoonZu/Item/Data/%s",FileName);

	
	//FILE* fp = fopen(buffer, "rt");
	FILE* fp = fopen("C:/Program Files/Intizen/GoonZu/Item/Data/cashitembuyinfo.txt", "rt");
	
	if(fp == NULL)
	{
		AfxMessageBox( buffer);		
		return ;
	}

	
	// 5줄은 무시한다.
	for(int i = 0;i < 5;i++)
		fgets(buffer, 1024, fp);

	for ( i = 0 ; i < MAX_ITEM_FILE ; i ++ )
	{
		strcpy(ItemInfo[i],"");	
	}

	i=0;
	while( fgets(buffer, 1024, fp) != NULL)
	{
		if ( i >= MAX_ITEM_FILE ) break ;
		
		char * szname;
		char * szkind;
		

		

		szkind = strtok(buffer, "\t");
		szname = strtok(NULL,"\t");

		if ( !szname /*|| !szkind*/ ) break ;

		{

			//szindex= strtok(szname,":");
			szname =strtok(NULL," " );
			strcpy(ItemInfo[i],szname);
		}
		//m_NPCImgIndex[i] = atoi(szkind);
		i ++ ;
	}



	fclose(fp);
    
}




