#include "JYLibFunction.h"

#include "TextMgr/TextMgr.h"

//extern cltTextMgr g_pTextMgr;

CJYLibFunction	g_JYLibFunction;


CJYLibFunction::CJYLibFunction()
{
}

CJYLibFunction::~CJYLibFunction()
{
}

void CJYLibFunction::SetNumTxt( INT64 num , TCHAR * dest , SI16 destsize, TCHAR * add )
{
	FILE *fp= NULL;
	TCHAR ServiceArea[64]=TEXT(" ");
	fp= _tfopen( TEXT("ServiceArea.txt"),TEXT("rt") );
	_ftscanf(fp,TEXT("%s"),ServiceArea);
	fclose(fp);

	TCHAR *numchar = TEXT("0123456789");
	TCHAR numunit[20][10] = {TEXT("십"),TEXT("백"),TEXT("천"),TEXT("만"),TEXT("억"),
							TEXT("조"),TEXT("경"),TEXT("해"),TEXT("자"),TEXT("양")};
	
    if( (_tcsncmp( ServiceArea, TEXT("SERVICEAREA_ENGLISH"), 19 ) == 0)
		|| (_tcsncmp( ServiceArea, TEXT("SERVICEAREA_EUROPE"), 18 ) == 0)
	)
	{
		StringCchCopy(numunit[0], 10, TEXT("0"));
		StringCchCopy(numunit[1], 10, TEXT("00"));
		StringCchCopy(numunit[2], 10, TEXT("000"));
		StringCchCopy(numunit[3], 10, TEXT("0000"));
		StringCchCopy(numunit[4], 10, TEXT("00000000"));
		StringCchCopy(numunit[5], 10, TEXT("000000000000"));
	}
	else if ( _tcsncmp( ServiceArea, TEXT("SERVICEAREA_CHINA"), 17 ) == 0 ||
		_tcsncmp( ServiceArea, TEXT("SERVICEAREA_JAPAN"), 17 ) == 0 )
	{
		StringCchCopy(numunit[0], 10, GetTxtFromMgr(5667));
		StringCchCopy(numunit[1], 10, GetTxtFromMgr(5668));
		StringCchCopy(numunit[2], 10, GetTxtFromMgr(5669));
		StringCchCopy(numunit[3], 10, GetTxtFromMgr(5670));
		StringCchCopy(numunit[4], 10, GetTxtFromMgr(5671));
		StringCchCopy(numunit[5], 10, GetTxtFromMgr(5672));

	}
	

	
	TCHAR szNumDest[8];
	SI32 i = 1, destpos = 0 ;
	SI64 n = 1 ;

	if( num < 0 ) num = 0;

	if( num == 0 ) {
		if( add ) {
			StringCchPrintf( dest, destsize, TEXT("0%s"), add );
		} else {
			StringCchPrintf( dest, destsize, TEXT("0") );
		}
		return;
	}

	// 자릿수 확인
	bool loop = true;
	while( loop )
	{
		if( (SI64)(num / n) < 10 ) break;
		n = n * 10;
		i++;
	}

	memset(szNumDest,0,8);
	memset(dest , 0 , sizeof(char)) ;
	for ( int j = i ; j > 0 ; j-- )
	{

		if ( (int)(num/n) != 0 )
		{
			StringCchPrintf(szNumDest, 8, TEXT("%d"),(int)(num / n));
			StringCchCat(dest, destsize, szNumDest);
		}

		if ( ( j - 1 ) % 4 != 0 )
		{
			if ( (int)(num/n) != 0 )
				StringCchCat(dest, destsize, numunit[((j-1)%4)-1]);
		}
		else
		{
			if ( j > 3 )
			{
				StringCchCat(dest, destsize, numunit[((j+3)/4)+1]);
				if ( (num%n)/(n/10000) == 0 )
				{
					j -= 4  ;
					n = n / 10000 ;
				}
			}
		}

		num = num % n;
		
		n = n / 10 ;

		if ( num == 0 && j % 4 == 1 )
			break;
	}

	if ( add != NULL )
	{
		StringCchCopy(szNumDest, 8, add);
		StringCchCat(dest, destsize, szNumDest);
	}
}

void CJYLibFunction::SetNumUnit(INT64 num , TCHAR * dest , SI16 destsize, TCHAR * add )
{

	TCHAR *numchar = TEXT("0123456789");

	SI32 i, destpos;
	SI64 n;

	BOOL bSign = FALSE;
	if( num < 0 ) 
	{
		num = -num;
		bSign = TRUE;
	}

	// 자릿수 확인
	n = 1;
	i = 1;
	bool loop = true;
	while( loop )
	{
		if( (SI64)(num / n) < 10 ) break;
		n = n * 10;							
		++i;
	}

	// 윗자리 부터 거슬러 내려온다
	destpos = 0;

	if( bSign ) dest[ destpos++ ] = '-';

	while( i > 0 ) 
	{
		dest[ destpos++ ] = numchar[ (int)(num / n) ];
		num = num % n;

		n = n / 10;

		--i;
		if( i > 0 && i % 3 == 0 ) dest[ destpos++ ] = ',';
	}

	if( add ) 
	{
		StringCchCopy( &dest[ destpos ], destsize - destpos, add );
	} 
	else 
	{
		dest[ destpos ] = NULL;
	}
}

HRGN CJYLibFunction::DIBToRgn(HBITMAP hBmp, COLORREF BkColor, BOOL Direct)
{
	// use to return the handle of the HGRN
	HRGN hRgn = NULL;					

	//the difference of the color
	COLORREF  Tolerance=0x00101010;
	if (hBmp)
	{
		//creat the dib to save the dc
		HDC hMemDC = CreateCompatibleDC(NULL);		
		if (hMemDC)
		{
			BITMAP bm;
			//get the info of the bitmap
			GetObject(hBmp, sizeof(bm), &bm);	
			
			BITMAPINFOHEADER BmpInfoh = {	//the struct of the bitmap
				sizeof(BITMAPINFOHEADER),	// biSize
					bm.bmWidth,				// biWidth;
					bm.bmHeight,			// biHeight;
					1,						// biPlanes;
					32,						// biBitCount
					BI_RGB,					// biCompression;
					0,						// biSizeImage;
					0,						// biXPelsPerMeter;
					0,						// biYPelsPerMeter;
					0,						// biClrUsed;
					0						// biClrImportant;
			};
			//design a void point to point to the bitmap
			LPVOID pBit32; 
			//creat a DIB
			HBITMAP hDib32 = CreateDIBSection(hMemDC, 
				(BITMAPINFO *)&BmpInfoh, 
				DIB_RGB_COLORS, &pBit32, NULL, 0);
			if (hDib32)
			{
				//copy dib to DC
				HBITMAP hOldib32 = (HBITMAP)SelectObject(hMemDC, hDib32);
				// create a DC to save orgin bitmap
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					BITMAP bm32;
					// get the new 34 bit Dib size
					GetObject(hDib32, sizeof(bm32), &bm32);
					//make sure the 32Dib's every line pilex's is 4 's times
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;
					//copy the orginal dib to DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					//copy dib to memory DC
					BitBlt(hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, hDC, 0, 0, SRCCOPY);
					DWORD MaxRects = MAX_ALLOC_RECTS;
					SYSTEM_INFO  Sysinfo;
					//get memory size
					GetSystemInfo(&Sysinfo);
					//make a stack which can chang big
					//alloct memory
					HANDLE hRcData=HeapCreate(HEAP_GENERATE_EXCEPTIONS,Sysinfo.dwPageSize, 0);
					RGNDATA * pRcData=(RGNDATA*)HeapAlloc(hRcData,HEAP_ZERO_MEMORY,
						sizeof(RGNDATAHEADER)+sizeof(RECT)*MaxRects);
					//fill the the RGNDATA struck
					pRcData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pRcData->rdh.iType = RDH_RECTANGLES;
					pRcData->rdh.nCount = pRcData->rdh.nRgnSize = 0;
					SetRect(&pRcData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
					BYTE hr,hg,hb,lr,lg,lb;
					switch(BkColor)
					{
					case RGB(255,255,255):	//if the bkcolor is white
						hr = GetRValue(BkColor);
						hg = GetGValue(BkColor);
						hb = GetBValue(BkColor);
						lr = min(0xff, hr - GetRValue(Tolerance));
						lg = min(0xff, hg - GetGValue(Tolerance));
						lb = min(0xff, hb - GetBValue(Tolerance));
						break;
					case RGB(0,0,0):	//if the bkcolor is black
						lr = GetRValue(BkColor);
						lg = GetGValue(BkColor);
						lb = GetBValue(BkColor);
						hr = min(0xff, lr + GetRValue(Tolerance));
						hg = min(0xff, lg + GetGValue(Tolerance));
						hb = min(0xff, lb + GetBValue(Tolerance));
						break;
					default:		//if the bkcolor is other color
						Tolerance=0x111111;
						lr =max(0, GetRValue(BkColor)-GetRValue(Tolerance));
						lg = max(0,GetGValue(BkColor)-GetGValue(Tolerance));
						lb = max(0,GetBValue(BkColor)-GetBValue(Tolerance));
						hr=min(0xff,GetRValue(BkColor)+GetRValue(Tolerance));
						hg=min(0xff,GetGValue(BkColor)+GetGValue(Tolerance));
						hb=min(0xff,GetBValue(BkColor)+GetBValue(Tolerance));
						break;
					}
					// Get the bit point and do the search
					BYTE *pBits = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						for (int x = 0; x < bm.bmWidth; x++)
						{
							int x0 = x;
							DWORD *pColor = (DWORD *)pBits + x;
							BYTE dr,dg,db;
							while (x < bm.bmWidth)
							{
								dr=GetRValue(*pColor);
								dg=GetGValue(*pColor);
								db=GetBValue(*pColor);
								
								if ((dr>= lr && dr<= hr)&&(dg>=lg&&dg<=hg)&&(db>=lb&&db<=hb))
								{
									if(Direct)
										break;
									else
									{
										pColor++;
										x++;
									}
								}
								else if(Direct)
								{
									pColor++;
									x++;
								}
								else
									break;
								
							}
							if (x > x0)
							{
								if (pRcData->rdh.nCount >= MaxRects)
								{
									MaxRects += MAX_ALLOC_RECTS;
									//re alloc the stack
									pRcData=(RGNDATA*)HeapReAlloc(
										hRcData,HEAP_ZERO_MEMORY,pRcData, 
										sizeof(RGNDATAHEADER)+sizeof(RECT)*MaxRects);
								}
								RECT *pr = (RECT *)&pRcData->Buffer;
								SetRect(&pr[pRcData->rdh.nCount], x0, y, x, y+1);
								pRcData->rdh.rcBound.left = x0;
								pRcData->rdh.rcBound.top = y;
								pRcData->rdh.rcBound.right = x;
								pRcData->rdh.rcBound.bottom = y+1;
								pRcData->rdh.nCount++;
								
								if (pRcData->rdh.nCount == 3000)
								{	
									HRGN tmphRgn = ExtCreateRegion(NULL,
										sizeof(RGNDATAHEADER) + (sizeof(RECT) * MaxRects),
										pRcData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, tmphRgn, RGN_OR);
										DeleteObject(tmphRgn);
									}
									else
										hRgn = tmphRgn;
									pRcData->rdh.nCount = 0;
									SetRect(&pRcData->rdh.rcBound, 
										MAXLONG, MAXLONG, 0, 0);
								}
							}
						}
						
						// search next line
						pBits -= bm32.bmWidthBytes;
					}
					HRGN tmphRgn = ExtCreateRegion(NULL, 
						sizeof(RGNDATAHEADER) + (sizeof(RECT) * MaxRects), pRcData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, tmphRgn, RGN_OR);
						DeleteObject(tmphRgn);
					}
					else
						hRgn = tmphRgn;
					// make a rect ,use this rect xor to the  BkColor
					//then we can get the rect we want
					if(!Direct)
					{
						HRGN hRect=CreateRectRgn(0,0,bm.bmWidth,bm.bmHeight);
						if(hRect)
						{
							CombineRgn(hRgn,hRgn,hRect,RGN_XOR);
							DeleteObject(hRect);
						}
						else
							return NULL;
					}
					//release the memory
					HeapFree(hRcData,HEAP_NO_SERIALIZE,pRcData);
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
					DeleteObject(holdBmp);
				}
				SelectObject(hMemDC,hOldib32);
				DeleteDC(hMemDC);
				DeleteObject(hOldib32);
				DeleteObject(hDib32);
			}
			else
				DeleteDC(hMemDC);
		}
	}
	return hRgn;
}