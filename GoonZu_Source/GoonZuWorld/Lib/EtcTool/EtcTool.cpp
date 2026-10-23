#include "EtcTool.h"

#ifdef GOONZU
#include "CommonLogic.h"
#endif

void MsgBox(TCHAR * szCaption, TCHAR*szFormat,... )
{

	TCHAR szBuffer[256] = TEXT("");
	
	/*
    TCHAR *pArguments;
	pArguments=( TCHAR * )&szFormat + sizeof( szFormat );
	StringCchVPrintf( szBuffer, 256, szFormat, pArguments );
	*/

	va_list ap;
	va_start( ap, szFormat );
	StringCchVPrintf( szBuffer, 256, szFormat, ap );
	va_end( ap );

#ifdef GOONZU
	if (pclClient)
	
	#ifdef _DEBUG
	{
		MessageBox(NULL, szBuffer, szCaption, MB_OK|MB_TOPMOST);
	}
	#else
	{
		if (pclClient->GameMode == GAMEMODE_CLIENT)
			MessageBox(NULL, szBuffer, szCaption, MB_OK|MB_TOPMOST);
		else
		{

			pclClient->pclLog->FilePrint(TEXT("Config\\MsgBoxError.log"), szBuffer);
		}
	}
	#endif
#else
	MessageBox(NULL, szBuffer, szCaption, MB_OK|MB_TOPMOST);
#endif

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