#include "stdafx.h"
#include "EtcTool.h"


void MsgBox(char * szCaption, char*szFormat,... )
{
	char szBuffer[256];
    char *pArguments;

	pArguments=( char * )&szFormat + sizeof( szFormat );
	vsprintf( szBuffer, szFormat, pArguments );

	MessageBox(NULL, szBuffer, szCaption, MB_OK|MB_TOPMOST);
}

// 소팅을 위해 데이터를 추가한다. 
bool SortAdd(SI32* pdst, SI32* pdata, SI32* pmindata, SI32 data1,  SI32 data2, SI32 maxnum)
{

	if(*pmindata > data2)return false;

	SI32 i, j;

	for(i = 0; i < maxnum;i++)
	{
		if(pdata[i] < data2)
		{
			for(j = (maxnum -1);j > i;j--)
			{
				pdata[j]	= pdata[j - 1];
				pdst[j]		= pdst[j - 1];
			}
			pdata[i]	= data2;
			pdst[i]		= data1;

			(*pmindata) =  pdata[maxnum - 1];
			return true;
		}
	}

	return false;
	
}