//-----------------------------------------------------------------------------
// 2005/11/19 이제완
//-----------------------------------------------------------------------------
#include "NUtil.h"

//#include "../GoonZuWorld/Lib/dxutil.h"

//#include "NArray.h"
//#include "NTCHARString.h"

#include <malloc.h>


//=============================================================================
// STRING
//=============================================================================
inline DOUBLE	HQStoF( const TCHAR* pStr )									{	return _tstof( pStr );						}
inline SI32		HQStoI( const TCHAR* pStr )									{	return _tstoi( pStr );						}
inline DOUBLE	HQStoD( const TCHAR* pString, TCHAR **pEnd)					{   return	_tcstod(pString, pEnd);				}
inline UI32		HQStoUL(const TCHAR* pString, TCHAR **pEnd, SI32 siBase)    {   return	_tcstoul(pString, pEnd, siBase);	}

inline SI32		HQAtoI( const TCHAR* pChar)									{	return _ttoi( pChar );						}
inline SI64		HQAtoI64( const TCHAR* pChar)								{	return _ttoi64( pChar );					}
inline SI32		HQAtoL( const TCHAR* pChar)									{	return _ttol( pChar );						}


inline SI32		HQStrLen( const TCHAR* pString )							{	return (SI32)_tcslen( pString );			}
inline SI32		HQStrCmp( const TCHAR* pString1, const TCHAR* pString2 )	{	return _tcscmp( pString1, pString2 );		}
inline SI32		HQStriCmp(const TCHAR* pString1, const TCHAR* pString2 )	{	return _tcsicmp( pString1, pString2 );		}
inline SI32		HQStrtoL( const TCHAR* pStart, TCHAR** pEnd, SI32 siBase )	{	return _tcstoul( pStart, pEnd, siBase );	}
inline SI32		HQStrnCmp( const TCHAR* pA, const TCHAR* pB, SI32 siCount)	{	return _tcsncmp( pA, pB, siCount );			}
inline SI32		HQStrniCmp(const TCHAR* pA, const TCHAR* pB, SI32 siCount)	{	return _tcsnicmp( pA, pB, siCount );		}
inline TCHAR*	HQStrStr( const TCHAR* pString, const TCHAR* pFind )		{	return _tcsstr( pString, pFind );			}
inline TCHAR*	HQStrChr( const TCHAR* pString, SI32 c )					{	return _tcschr( pString, c );				}
inline TCHAR*	HQStrrChr( const TCHAR* pString, SI32 c )					{	return _tcsrchr( pString, c );				}
inline TCHAR*	HQStrCat( TCHAR* pDest, const TCHAR* pSrc )					{	return _tcscat( pDest, pSrc );				}
inline TCHAR*	HQStrnCat(TCHAR* pDest, const TCHAR* pSrc, SI32 siCount)	{	return _tcsncat(pDest, pSrc , siCount);		}
inline TCHAR*	HQStrCpy( TCHAR* pDest, const TCHAR* pSrc )					{	return _tcscpy( pDest, pSrc );				}
inline TCHAR*	HQStrnCpy( TCHAR* pDest, const TCHAR* pSrc, SI32 siMaxLen )	{	_tcsncpy(pDest, pSrc, siMaxLen ); pDest[siMaxLen-1]=0; return pDest;	}
inline TCHAR*	HQStrUpr( TCHAR* pString )									{	return _tcsupr( pString );					}
inline TCHAR*	HQStrLwr( TCHAR* pString )									{	return _tcslwr(pString);					}
inline TCHAR*	HQStrTok( TCHAR* pString, TCHAR* pDelimit )					{	return _tcstok( pString, pDelimit );		}
inline SI32		HQIsSpace( TCHAR cString )									{	return _istspace( cString );				}

inline void		HQMemZero( void* pDest, SI32 siCount )							{	memset( pDest, 0,	 siCount );			}

#define HQ_BUFFER_LINE	1024*10
#define HQ_STRING_MAX_DATA				2048

//=============================================================================
//  @Name   HQDivideByTab()
//! @author LeeKH						
//! @date   2006-3-30
//! @param  X   X
//! @return X
//! @brief  지원 포맷 - %s(TCHAR*), %c(TCHAR), %d(SI32), %f(FLOAT), %I64d(SI64), %x(DWORD)
//=============================================================================
SI32 HQDivideByTab( const TCHAR* pszStream, TCHAR* pszFormat, ...)
{
	if (pszStream == NULL)				return 0;
	if (pszFormat == NULL)				return 0;

	TCHAR	szBuffer[HQ_BUFFER_LINE];
	SI32	siCount = 0;

	TCHAR	szDivider[] = TEXT("\t");
	TCHAR*	pszToken = NULL;
	SI32	siPtr = 0;

	TCHAR*	pszParam = NULL;
	SI32*	psiParam = NULL;
	FLOAT*	pfParam = NULL;
	SI64*	psi64Param = NULL;
	TCHAR*	pcParam = NULL;
	DWORD*	pdwParam = NULL;

	TCHAR*	pszStoped = NULL;

	// 전체 복사 - HQStrTok() 함수의 기능때문에 전체를 복사해서 사용
	HQStrnCpy(szBuffer, pszStream, HQ_BUFFER_LINE);
	SI32	siBufferLength = HQStrLen(szBuffer);
	if (siBufferLength == 0)				return siCount;

	// 공백 지우기 - 뒤에서 부터 진행
	siPtr = siBufferLength-1;
	while (siPtr >= 0)
	{
		if (HQIsSpace(szBuffer[siPtr]))
			szBuffer[siPtr] = NULL;
		else
			break;
		siPtr--;
	}

	// 첫번째 토큰 얻기
	pszToken = HQStrTok(szBuffer, szDivider);
	if (pszToken == NULL)		return siCount;

	siPtr = 0;
	// 공백 건너뛰기 - 포맷문자열
	while (HQIsSpace(pszFormat[siPtr]))
	{
		siPtr++;
		if (pszFormat[siPtr] == NULL)		return siCount;
	}

	// 파라미터 리스트 시작
	va_list arglist;
	va_start(arglist, pszFormat);

	while(pszFormat[siPtr] != NULL)
	{
		// 공백 건너뛰기 - 포맷문자열
		while (HQIsSpace(pszFormat[siPtr]))
		{
			siPtr++;
			if (pszFormat[siPtr] == NULL)		break;
		}

		// 타입은 "%"가 먼저 나온다.
		if (pszFormat[siPtr] == TEXT('%'))
		{
			siPtr++;
			// TYPE : s
			if (pszFormat[siPtr] == TEXT('s'))
			{
				siPtr++;
				pszParam = va_arg(arglist, TCHAR*);
				if (pszParam == NULL)		break;
				HQStrCpy(pszParam, pszToken);
				siCount++;
			}
			// TYPE : c
			else if (pszFormat[siPtr] == TEXT('c'))
			{
				siPtr++;
				pcParam = va_arg(arglist, TCHAR*);
				if (pcParam == NULL)		break;
				*pcParam = pszToken[0];
				siCount++;
			}
			// TYPE : d
			else if (pszFormat[siPtr] == TEXT('d'))
			{
				siPtr++;
				psiParam = va_arg(arglist, SI32*);
				if (psiParam == NULL)		break;
				*psiParam = HQAtoI(pszToken);
				siCount++;
			}
			// TYPE : f
			else if (pszFormat[siPtr] == TEXT('f'))
			{
				siPtr++;
				pfParam = va_arg(arglist, FLOAT*);
				if (pfParam == NULL)		break;
				*pfParam = (FLOAT)HQStoF(pszToken);
				siCount++;
			}
			// TYPE : I64d
			else if (pszFormat[siPtr] == TEXT('I'))
			{
				if (pszFormat[siPtr+1] == TEXT('6') && pszFormat[siPtr+2] == TEXT('4') &&
					pszFormat[siPtr+3] == TEXT('d') )
				{
					siPtr += 4;
					psi64Param = va_arg(arglist, SI64*);
					if (psi64Param == NULL)		break;
					*psi64Param = HQAtoI64(pszToken);
					siCount++;
				}
				else
					break;
			}
			// TYPE : x
			else if (pszFormat[siPtr] == TEXT('x'))
			{
				siPtr++;
				pdwParam = va_arg(arglist, DWORD*);
				if (pdwParam == NULL)		break;
				*pdwParam = HQStoUL(pszToken, &pszStoped, 16);
				siCount++;
			}
			else
				break;
		}
		else
			break;

		// 다음 토큰 진행
		pszToken = HQStrTok(NULL, szDivider);
		if (pszToken == NULL)	break;
	}

	// 파라미터 리스트 종료
	va_end(arglist);
	return siCount;
}

//=============================================================================
//  @Name   HQDivideBy()
//! @author LeeKH						
//! @date   2006-5-19
//! @param  X   X
//! @return X
//! @brief  지원 포맷 - %s(TCHAR*), %c(TCHAR), %d(SI32), %f(FLOAT), %I64d(SI64), %x(DWORD)
//=============================================================================
SI32 HQDivideBy( const TCHAR* pszDivider, const TCHAR* pszStream, TCHAR* pszFormat, ...)
{
	// 보안코드
	if (pszDivider == NULL)							return 0;
	if (pszStream == NULL)							return 0;
	if (pszFormat == NULL)							return 0;
	// HQStrnCpy 사용시 메모리영역을 넘는걸 방지하기 위해 숫자를 줄임
	if (HQStrLen(pszStream) > HQ_BUFFER_LINE - 10)	return 0;

	// 버퍼      ... 10240
	TCHAR				szBuffer[HQ_BUFFER_LINE];
	// 토큰 한개 ... 2048
	TCHAR				szToken[HQ_STRING_MAX_DATA];

	SI32	siCount = 0;

	SI32	siPtr = 0;
	SI32	siStart, siEnd, siLength;

	TCHAR*	pszParam = NULL;
	SI32*	psiParam = NULL;
	FLOAT*	pfParam = NULL;
	SI64*	psi64Param = NULL;
	TCHAR*	pcParam = NULL;
	DWORD*	pdwParam = NULL;

	TCHAR*	pszStoped = NULL;

	HQMemZero(szBuffer, sizeof(szBuffer));
	HQStrnCpy(szBuffer, pszStream, HQ_BUFFER_LINE);
	SI32	siBufferLength = HQStrLen(pszStream);
	if (siBufferLength == 0)				return siCount;

	// 공백 지우기 - 뒤에서 부터 진행
	siPtr = siBufferLength-1;
	while (siPtr >= 0)
	{
		if (HQIsSpace(szBuffer[siPtr]))
		{
			siBufferLength = siPtr;
			szBuffer[siPtr] = NULL;
		}
		else
			break;
		siPtr--;
	}

	// 첫번째 토큰 얻기
	siStart = 0;
	TCHAR* pszFind = HQStrStr(&szBuffer[siStart], pszDivider);
	if (pszFind == NULL)		return siCount;
	siEnd = (SI32)(pszFind - szBuffer);
	siLength = siEnd - siStart;
	HQMemZero(szToken, sizeof(szToken));
	if (siLength > 0)
	{
		if (siLength >= HQ_STRING_MAX_DATA)			return 0;		// 보안코드
		HQStrnCpy(szToken, &szBuffer[siStart], siLength + 1);		
	}
	siStart = siEnd + 1;

	siPtr = 0;
	// 공백 건너뛰기 - 포맷문자열
	while (HQIsSpace(pszFormat[siPtr]))
	{
		siPtr++;
		if (pszFormat[siPtr] == NULL)		return siCount;
	}

	// 파라미터 리스트 시작
	va_list arglist;
	va_start(arglist, pszFormat);

	while(pszFormat[siPtr] != NULL)
	{
		// 공백 건너뛰기 - 포맷문자열
		while (HQIsSpace(pszFormat[siPtr]))
		{
			siPtr++;
			if (pszFormat[siPtr] == NULL)		break;
		}

		// 타입은 "%"가 먼저 나온다.
		if (pszFormat[siPtr] == TEXT('%'))
		{
			siPtr++;
			// TYPE : s
			if (pszFormat[siPtr] == TEXT('s'))
			{
				siPtr++;
				pszParam = va_arg(arglist, TCHAR*);
				if (pszParam == NULL)		break;
				if (HQStrLen(szToken) > 0)
					HQStrCpy(pszParam, szToken);
				else
					pszParam[0] = NULL;
				siCount++;
			}
			// TYPE : c
			else if (pszFormat[siPtr] == TEXT('c'))
			{
				siPtr++;
				pcParam = va_arg(arglist, TCHAR*);
				if (pcParam == NULL)		break;
				if (HQStrLen(szToken) > 0)
					*pcParam = szToken[0];
				else
					*pcParam = NULL;
				siCount++;
			}
			// TYPE : d
			else if (pszFormat[siPtr] == TEXT('d'))
			{
				siPtr++;
				psiParam = va_arg(arglist, SI32*);
				if (psiParam == NULL)		break;
				if (HQStrLen(szToken) > 0)
					*psiParam = HQAtoI(szToken);
				else
					*psiParam = 0;
				siCount++;
			}
			// TYPE : f
			else if (pszFormat[siPtr] == TEXT('f'))
			{
				siPtr++;
				pfParam = va_arg(arglist, FLOAT*);
				if (pfParam == NULL)		break;
				if (HQStrLen(szToken) > 0)
					*pfParam = (FLOAT)HQStoF(szToken);
				else
					*pfParam  = 0.0f;
				siCount++;
			}
			// TYPE : I64d
			else if (pszFormat[siPtr] == TEXT('I'))
			{
				if (pszFormat[siPtr+1] == TEXT('6') && pszFormat[siPtr+2] == TEXT('4') &&
					pszFormat[siPtr+3] == TEXT('d') )
				{
					siPtr += 4;
					psi64Param = va_arg(arglist, SI64*);
					if (psi64Param == NULL)		break;
					if (HQStrLen(szToken) > 0)
						*psi64Param = HQAtoI64(szToken);
					else
						*psi64Param = 0;
					siCount++;
				}
				else
					break;
			}
			// TYPE : x
			else if (pszFormat[siPtr] == TEXT('x'))
			{
				siPtr++;
				pdwParam = va_arg(arglist, DWORD*);
				if (pdwParam == NULL)		break;
				if (HQStrLen(szToken) > 0)
					*pdwParam = HQStoUL(szToken, &pszStoped, 16);
				else
					*pdwParam = 0;
				siCount++;
			}
			else
				break;
		}
		else
			break;

		// 다음 토큰 진행
		if (siStart > siBufferLength)				break;
		TCHAR* pszFind = HQStrStr(&szBuffer[siStart], pszDivider);
		if (pszFind == NULL)
		{
			if (siStart < siBufferLength)
				siEnd = siBufferLength;
			else
				break;
		}
		else
			siEnd = (SI32)(pszFind - szBuffer);
		siLength = siEnd - siStart;
		HQMemZero(szToken, sizeof(szToken));
		if (siLength > 0)
		{
			if (siLength >= HQ_STRING_MAX_DATA)				break;										// 보안코드
			if (siStart + siLength + 1 > HQ_BUFFER_LINE)	siLength = HQ_BUFFER_LINE - siStart - 1;	// 보안코드
			HQStrnCpy(szToken, &szBuffer[siStart], siLength + 1);
		}
		siStart = siEnd + 1;
	}

	// 파라미터 리스트 종료
	va_end(arglist);
	return siCount;
}


SI32 HQDivideBy2( const TCHAR* pszDivider, const TCHAR* pszStream, TCHAR* pszFormat, ...)
{
	if (pszStream == NULL)				return 0;
	if (pszFormat == NULL)				return 0;

	TCHAR	szBuffer[HQ_BUFFER_LINE];
	SI32	siCount = 0;

	TCHAR	szDivider[5] ={ '\0', };
	_tcsncpy(szDivider, pszDivider, sizeof(pszDivider));	

	TCHAR*	pszToken = NULL;
	SI32	siPtr = 0;

	TCHAR*	pszParam = NULL;
	SI32*	psiParam = NULL;
	FLOAT*	pfParam = NULL;
	SI64*	psi64Param = NULL;
	TCHAR*	pcParam = NULL;
	DWORD*	pdwParam = NULL;

	TCHAR*	pszStoped = NULL;

	// 전체 복사 - HQStrTok() 함수의 기능때문에 전체를 복사해서 사용
	HQStrnCpy(szBuffer, pszStream, HQ_BUFFER_LINE);
	SI32	siBufferLength = HQStrLen(szBuffer);
	if (siBufferLength == 0)				return siCount;

	// 공백 지우기 - 뒤에서 부터 진행
	siPtr = siBufferLength-1;
	while (siPtr >= 0)
	{
		if (HQIsSpace(szBuffer[siPtr]))
			szBuffer[siPtr] = NULL;
		else
			break;
		siPtr--;
	}

	// 첫번째 토큰 얻기
	pszToken = HQStrTok(szBuffer, szDivider);
	if (pszToken == NULL)		return siCount;

	siPtr = 0;
	// 공백 건너뛰기 - 포맷문자열
	while (HQIsSpace(pszFormat[siPtr]))
	{
		siPtr++;
		if (pszFormat[siPtr] == NULL)		return siCount;
	}

	// 파라미터 리스트 시작
	va_list arglist;
	va_start(arglist, pszFormat);

	while(pszFormat[siPtr] != NULL)
	{
		// 공백 건너뛰기 - 포맷문자열
		while (HQIsSpace(pszFormat[siPtr]))
		{
			siPtr++;
			if (pszFormat[siPtr] == NULL)		break;
		}

		// 타입은 "%"가 먼저 나온다.
		if (pszFormat[siPtr] == TEXT('%'))
		{
			siPtr++;
			// TYPE : s
			if (pszFormat[siPtr] == TEXT('s'))
			{
				siPtr++;
				pszParam = va_arg(arglist, TCHAR*);
				if (pszParam == NULL)		break;
				HQStrCpy(pszParam, pszToken);
				siCount++;
			}
			// TYPE : c
			else if (pszFormat[siPtr] == TEXT('c'))
			{
				siPtr++;
				pcParam = va_arg(arglist, TCHAR*);
				if (pcParam == NULL)		break;
				*pcParam = pszToken[0];
				siCount++;
			}
			// TYPE : d
			else if (pszFormat[siPtr] == TEXT('d'))
			{
				siPtr++;
				psiParam = va_arg(arglist, SI32*);
				if (psiParam == NULL)		break;
				*psiParam = HQAtoI(pszToken);
				siCount++;
			}
			// TYPE : f
			else if (pszFormat[siPtr] == TEXT('f'))
			{
				siPtr++;
				pfParam = va_arg(arglist, FLOAT*);
				if (pfParam == NULL)		break;
				*pfParam = (FLOAT)HQStoF(pszToken);
				siCount++;
			}
			// TYPE : I64d
			else if (pszFormat[siPtr] == TEXT('I'))
			{
				if (pszFormat[siPtr+1] == TEXT('6') && pszFormat[siPtr+2] == TEXT('4') &&
					pszFormat[siPtr+3] == TEXT('d') )
				{
					siPtr += 4;
					psi64Param = va_arg(arglist, SI64*);
					if (psi64Param == NULL)		break;
					*psi64Param = HQAtoI64(pszToken);
					siCount++;
				}
				else
					break;
			}
			// TYPE : x
			else if (pszFormat[siPtr] == TEXT('x'))
			{
				siPtr++;
				pdwParam = va_arg(arglist, DWORD*);
				if (pdwParam == NULL)		break;
				*pdwParam = HQStoUL(pszToken, &pszStoped, 16);
				siCount++;
			}
			else
				break;
		}
		else
			break;

		// 다음 토큰 진행
		pszToken = HQStrTok(NULL, szDivider);
		if (pszToken == NULL)	break;
	}

	// 파라미터 리스트 종료
	va_end(arglist);
	return siCount;
}
