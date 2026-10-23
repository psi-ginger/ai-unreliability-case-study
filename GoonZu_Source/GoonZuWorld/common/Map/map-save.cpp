#include "map.h"
#include "CommonHeader.h"
#include "..\Client\Client.h"

extern cltCommonLogic* pclClient;

// 화일이름을 갖고 지도의 정보를 얻어온다. 
BOOL GetMapInfo(TCHAR* filename, cltMapHeader* header)
{
	FILE* fp;
	int numread;

	fp=_tfopen(filename, TEXT("rb"));
	if(fp==NULL)
	{
		return FALSE;
	}

	numread = fread(header, sizeof(cltMapHeader), 1, fp);

	fclose(fp);

	return TRUE;

}


// 지도를 저장한다. 
BOOL cltMapCommon::Save(TCHAR* filename)
{

	FILE* fp;
	// 버전도 기록한다. 
	MH.Version = 2;

	fp=_tfopen(filename, TEXT("wb"));

	if(fp==NULL)
	{

		return FALSE;
	}

	fwrite(&MH, sizeof(cltMapHeader), 1, fp);

	WriteIDMap(fp);
	WriteCollMap(fp);

	pclLayer[0]->Write(fp);
	pclLayer[1]->Write(fp);

	WriteFieldObjectMap(fp);
	WriteFieldObjectMapPara(fp);

	pclMapGeoInfo->Write(fp);

	fclose(fp);


	return TRUE;
}                     

BOOL cltMapCommon::WriteIDMap(FILE* fp)
{
	if( pIDMap == NULL )	return FALSE;
	fwrite(pIDMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}

BOOL cltMapCommon::ReadIDMap(FILE*fp)
{

	fread(pIDMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}

BOOL cltMapCommon::WriteCollMap(FILE* fp)
{
	if( pCollMap == NULL )	return FALSE;

	fwrite(pCollMap, sizeof(short),	MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}

BOOL cltMapCommon::ReadCollMap(FILE*fp)
{
	fread(pCollMap, sizeof(short), MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}

BOOL cltMapCommon::WriteFieldObjectMap(FILE* fp)
{
	if( pFieldObjectMap == NULL )	return FALSE;

	fwrite(pFieldObjectMap, sizeof(DWORD), MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}

BOOL cltMapCommon::ReadFieldObjectMap(FILE *fp)
{
	fread(pFieldObjectMap, sizeof(DWORD), MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}


BOOL cltMapCommon::WriteFieldObjectMapPara(FILE* fp)
{
	if( pFieldObjectMapPara == NULL )	return FALSE;

	fwrite(pFieldObjectMapPara, sizeof(DWORD), MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}

BOOL cltMapCommon::ReadFieldObjectMapPara(FILE *fp)
{
	fread(pFieldObjectMapPara, sizeof(DWORD), MH.MapXsize * MH.MapYsize, fp);
	return TRUE;
}




