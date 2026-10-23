//-----------------------------------------------------------------------------
// 2005/11/19 이제완
//-----------------------------------------------------------------------------

#ifndef _NUTIL_H
#define _NUTIL_H

#include "NDataTypes.h"

struct NCryptHeader
{
	BYTE	id[ 4 ];

	WORD	version;
	WORD	encodeType;

	DWORD	dwDataSize;

	BYTE	key;
	BYTE	chksum;

	BYTE	reserved[ 10 ];
};

void NByteEncrypt( BYTE *pSrc, BYTE *pDest, int size, BYTE key );
void NByteDecrypt( BYTE *pSrc, BYTE *pDest, int size, BYTE key );

bool NWriteFile( TCHAR *filename, BYTE *pBuffer, int size );
bool NWriteCryptFile( TCHAR *filename, BYTE *pSrc, int size );
bool NLoadCryptFile( TCHAR *filename, BYTE *pDest );
bool NLoadCryptFile( TCHAR *filename, BYTE **pDest, int* pSize );

bool IsEncryptedFile( TCHAR *filename );

//KHY - 0806 - 암호화 적용.
SI16 NWriteEnCryptFile( const TCHAR *basefilename, TCHAR *newfilename );
bool NWriteDeCryptFile( const TCHAR *basefilename, TCHAR *newfilename );


inline SI32		NLibStrLen( const TCHAR* pString )							{	return (SI32)_tcslen( pString );			}
inline SI32		NLibStrCmp( const TCHAR* pString1, const TCHAR* pString2 )	{	return _tcscmp( pString1, pString2 );	}

inline TCHAR*		NLibStrCpy( TCHAR* pDest, const TCHAR* pSrc )				{	return _tcscpy( pDest, pSrc );			}
inline TCHAR*		NLibStrrChr( const TCHAR* pString, SI32 c )					{	return (TCHAR*)_tcsrchr( pString, c );			}
inline TCHAR*		NLibStrCat( TCHAR* pDest, const TCHAR* pSrc )				{	return _tcscat( pDest, pSrc );			}

inline FILE*  		NLibFOpen(const TCHAR *file, const TCHAR *mode)				{	return _tfopen(file, mode);			}
inline SI32	 	NLibFClose(FILE *pStream)									{	return fclose( pStream );				}

inline TCHAR*		NLibStrTok( TCHAR* pString, TCHAR* pDelimit )				{	return _tcstok( pString, pDelimit );		}

UI32 DiffDate(SYSTEMTIME* Cur, SYSTEMTIME* Past);

bool TimeInBounds(SYSTEMTIME* Cur, SYSTEMTIME* Start, SYSTEMTIME* End);

void TimeAddSecond(SYSTEMTIME* pSrc, SI32 siSecond, SYSTEMTIME* pRet);
void TimeAddMinute(SYSTEMTIME* pSrc, SI32 siMinute, SYSTEMTIME* pRet);
void TimeAddHour(SYSTEMTIME* pSrc, SI32 siHour, SYSTEMTIME* pRet);
void TimeAddDate(SYSTEMTIME* pSrc, SI32 siDate, SYSTEMTIME* pRet);
void TimeSubHour(SYSTEMTIME* pSrc, SI32 siHour, SYSTEMTIME* pRet);
void TimeSubDate(SYSTEMTIME* pSrc, SI32 siDate, SYSTEMTIME* pRet);

int TimeCompare(SYSTEMTIME* pX, SYSTEMTIME* pY);
SI64 TimeOperation(SYSTEMTIME* pX, SYSTEMTIME* pY);
SI64 GetTimeValue(SYSTEMTIME* pX);

int RandomInt(int nStart, int nEnd);
SI32 HQDivideByTab( const TCHAR* pszStream, TCHAR* pszFormat, ...);
SI32 HQDivideBy( const TCHAR* pszDivider, const TCHAR* pszStream, TCHAR* pszFormat, ...);

bool MemoryToHex( const void* pData, UI32 siDataSize, TCHAR* pszOut, UI32 siOutSize );
bool MemoryToBinary( const void* pData, UI32 siDataSize, TCHAR* pszOut, UI32 siOutSize );

bool HexToMemory(char* pMem, const char* pszHex);

template<class T>
bool InBounds(T x, T min, T max)
{
	if( x < min)		return false;
	if( x > max)		return false;
	return true;
}

#endif

