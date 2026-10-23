//---------------------------------
// 2003/7/15 김태곤
//---------------------------------

#include "Map-Layor.h"
#include "NDataTypes.h"

//-----------------------------------------------------
// cltMapLayer
//-----------------------------------------------------
cltMapLayer::cltMapLayer(SI32 xsize, SI32 ysize, SI32* yoff)
{
	Xsize = xsize;
	Ysize = ysize;

	YOff	= yoff;

	pDrawInfo = new DWORD[xsize * ysize];
	if( pDrawInfo )	ZeroMemory(pDrawInfo, sizeof(DWORD)*xsize*ysize);

	pUnique = new UI08[xsize * ysize];
	if( pUnique )	ZeroMemory(pUnique, sizeof(UI08)*xsize*ysize);
}

cltMapLayer::~cltMapLayer()
{

	NDelete_Array( pDrawInfo );
	NDelete_Array( pUnique );
}

BOOL cltMapLayer::IsValid()
{
	if( pDrawInfo == NULL ) return false;
	if( pUnique == NULL ) return false;
	return true;
}

BOOL cltMapLayer::IsInArea(SI32 x, SI32 y)
{
	if( x < 0 || x >= Xsize || y < 0 || y >= Ysize ) return FALSE;
	return TRUE;
}


BOOL cltMapLayer::SetUnique(SI32 six, SI32 siy, SI32 siunique)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return FALSE;

	if(pUnique == NULL) return FALSE;

	pUnique[six + YOff[siy] ]	= siunique;
	return TRUE;
}

SI32 cltMapLayer::GetUnique(SI32 six, SI32 siy)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return 0;

	if(pUnique == NULL) return 0;

	return pUnique[six+YOff[siy]];
}

BOOL cltMapLayer::SetDrawInfo_Unique(SI32 six, SI32 siy, SI32 siunique)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return FALSE;
	if(pDrawInfo == NULL) return FALSE;

	pDrawInfo[six + YOff[siy] ] &= 0x00ffffff;
	pDrawInfo[six + YOff[siy] ]  |= (siunique<<24);

	return TRUE;
}

SI32 cltMapLayer::GetDrawInfo_Unique(SI32 six, SI32 siy)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return 0;
	if(pDrawInfo == NULL) return FALSE;

	return (pDrawInfo[six + YOff[siy] ]  >> 24);
}


BOOL cltMapLayer::SetDrawInfo_Ref(SI32 six, SI32 siy, SI32 siref)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return FALSE;
	if(pDrawInfo == NULL) return FALSE;

	pDrawInfo[six + YOff[siy] ] &= 0xff00ffff;
	pDrawInfo[six + YOff[siy] ]  |= (siref<<16);

	return TRUE;
}

SI32 cltMapLayer::GetDrawInfo_Ref(SI32 six, SI32 siy)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return FALSE;
	if(pDrawInfo == NULL) return FALSE;

	return LOBYTE( HIWORD(pDrawInfo[six + YOff[siy]]) );
}

BOOL cltMapLayer::SetDrawInfo_File(SI32 six, SI32 siy, SI32 sifileinfo)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return FALSE;
	if(pDrawInfo == NULL) return FALSE;

	pDrawInfo[six + YOff[siy]] &= 0xffff00ff;
	pDrawInfo[six + YOff[siy]] |= (sifileinfo << 8);

	return TRUE;
}


SI32 cltMapLayer::GetDrawInfo_File(SI32 six, SI32 siy)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return 0;
	if(pDrawInfo == NULL) return FALSE;

	return (LOWORD(pDrawInfo[six + YOff[siy]])>>8);
}

BOOL cltMapLayer::SetDrawInfo_Font(SI32 six, SI32 siy, SI32 sifontinfo)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return FALSE;
	if(pDrawInfo == NULL) return FALSE;

	pDrawInfo[six + YOff[siy]] &= 0xffffff00;
	pDrawInfo[six + YOff[siy]] |= sifontinfo;

	return TRUE;
}

SI32 cltMapLayer::GetDrawInfo_Font(SI32 six, SI32 siy)
{
	// 유효한 지도 영역인지 확인한다. 
	if(IsInArea(six, siy) == FALSE) return 0;
	if(pDrawInfo == NULL) return FALSE;

	return (LOWORD(pDrawInfo[six + YOff[siy]]) & 0x00ff);
}


BOOL cltMapLayer::Write(FILE* fp)
{
	if( pDrawInfo == NULL )	return FALSE;
	fwrite(pDrawInfo, sizeof(DWORD), Xsize * Ysize, fp);

	if( pUnique == NULL )	return FALSE;
	fwrite(pUnique, sizeof(UI08), Xsize * Ysize, fp);

	return TRUE;
}

BOOL cltMapLayer::Read(FILE*fp)
{
	fread(pDrawInfo,	sizeof(DWORD), Xsize * Ysize, fp);
	fread(pUnique,		sizeof(UI08), Xsize * Ysize, fp);


	return TRUE;
}
