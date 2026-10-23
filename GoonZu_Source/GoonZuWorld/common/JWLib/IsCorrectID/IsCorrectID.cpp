#include "IsCorrectID.h"

#include "../../../Client/Client.h"
extern cltCommonLogic* pclClient;

BOOL IsNumber( WCHAR wChar ) // 숫자
{
	if( wChar >= L'0' && wChar <= L'9' ) return TRUE;	// 숫자이면 통과 

	return FALSE;
}

BOOL IsAlphabet( WCHAR wChar ) // 영문
{
	if( wChar >= L'A' && wChar <= L'Z' ) return TRUE;	// 영문자 대문자이면 통과 
	if( wChar >= L'a' && wChar <= L'z' ) return TRUE;	// 영문자 소문자이면 통과

	return FALSE;
}

BOOL IsHangul( WCHAR wChar ) // 한글
{
	if( wChar >= 0xAC00 && wChar <= 0xD7A3 ) return TRUE;	// 한글이면 통과( 특수문자 제외 )

	return FALSE;
}

BOOL IsUnifiedHanja( WCHAR wChar ) // 한중일 통합 한자
{
	if( wChar >= 0x4E00 && wChar <= 0x9FFF ) return TRUE;	// 한중일공통

	return FALSE;
}

BOOL IsHanjaRadicals( WCHAR wChar ) // 한자 부수
{
	if( wChar >= 0x2E80 && wChar <= 0x2EFF ) return TRUE;	// CJK 한자부수

	return FALSE;
}

BOOL IsHanjaSymbolsNPunctuation( WCHAR wChar ) // 한자 기호
{
	if( wChar >= 0x3000 && wChar <= 0x303F ) return TRUE;	// CJK Symbols and Punctuation

	return FALSE;
}

BOOL IsHiragana( WCHAR wChar ) // 히라가나
{
	if( wChar >= 0x3040 && wChar <= 0x309F ) return TRUE;	// 히라가나

	return FALSE;
}


BOOL IsKatakana( WCHAR wChar ) // 카타카나
{
	if( wChar >= 0x30A0 && wChar <= 0x30FF ) return TRUE;	// 카타카나

	return FALSE;
}


BOOL IsCompatibilityHanja1( WCHAR wChar ) // 호환 한자
{
	if( wChar >= 0x3300 && wChar <= 0x33FF ) return TRUE;	// 한중일 호환

	return FALSE;
}


BOOL IsHanjaExtended( WCHAR wChar ) // 한자 확장
{
	if( wChar >= 0x3400 && wChar <= 0x4DB5 ) return TRUE;	// 한자확장

	return FALSE;
}

BOOL IsCompatibilityHanja2( WCHAR wChar ) // 호환 한자
{
	if( wChar >= 0xF900 && wChar <= 0xFAFF ) return TRUE;	// 한중일호환

	return FALSE;
}

BOOL IsHalfNFullWidth( WCHAR wChar ) // 반각 자모(HalfWidth and Fullwidth Forms)
{
	if( wChar >= 0xFF00 && wChar <= 0xFFEF ) return TRUE;	// HalfWidth and Fullwidth Forms

	return FALSE;
}

BOOL IsHanjaForms( WCHAR wChar ) // 한자 서식
{
	if( wChar >= 0xFE30 && wChar <= 0xFE4F ) return TRUE;	// 일어기타

	return FALSE;
}


BOOL IsMathematicalSymbols( WCHAR wChar ) // 수학 영문기호
{
	if( wChar >= 0x1D400 && wChar <= 0x1D7FF ) return TRUE;	// 일어기타

	return FALSE;
}


BOOL IsHanjaSupplement( WCHAR wChar ) // 호환 한자 지원
{
	if( wChar >= 0x2F800 && wChar <= 0x2FA1F ) return TRUE;	// 일어기타

	return FALSE;
}


BOOL IsCorrectID( TCHAR *szID )
{	

	/*
	if( pclClient->siServiceArea != ConstServiceArea_Korea ) {

			return TRUE;
		}
	*/

	//===================================
	// ID 생성할수 있는 유니코드 목록
	//==================================
	/*
	2E80..2EFF		CJK 한자부수
	3000..303F		CJK Symbols and Punctuation
	3040..309F		히라가나
	30A0..30FF		카타카나
	3300..33FF		한중일 호환
	3400..4DB5		한자확장
	4E00..9FFF		한중일공통
	F900..FAFF		한중일호환
	*/



	WCHAR	wszID[ 32 ];
	int		nLength		= 0;

#ifdef _UNICODE
	StringCchCopy( wszID, 32, szID );
#else
	MultiByteToWideChar( CP_ACP, MB_COMPOSITE, szID, -1, wszID, 32 );
#endif

	nLength = (int)wcslen( wszID );

	for( int i = 0; i < nLength; ++i ) 
	{
		// 스위치문에는 꼭 break; 를 시키자.
		switch( pclClient->siServiceArea ) 
		{
		case ConstServiceArea_Japan:
			{
				if ( IsHiragana( wszID[ i ] )					== TRUE ) continue;
				if ( IsKatakana( wszID[ i ] )					== TRUE ) continue;
				if ( IsHanjaRadicals( wszID[ i ] )				== TRUE ) continue;
				if ( IsHanjaSymbolsNPunctuation( wszID[ i ] )	== TRUE ) continue;
				if ( IsCompatibilityHanja1( wszID[ i ] )		== TRUE ) continue;
				if ( IsCompatibilityHanja2( wszID[ i ] )		== TRUE ) continue;
				if ( IsHanjaExtended( wszID[ i ] )				== TRUE ) continue;
				if ( IsHalfNFullWidth( wszID[ i ] )				== TRUE ) continue;
				if ( IsHanjaForms( wszID[ i ] )					== TRUE ) continue;
				if ( IsMathematicalSymbols( wszID[ i ] )		== TRUE ) continue;
				if ( IsHanjaSupplement( wszID[ i ] )			== TRUE ) continue;

				if ( IsUnifiedHanja( wszID[ i ] )	== TRUE ) continue;
				if ( IsNumber( wszID[ i ] )		== TRUE ) continue;
				if ( IsAlphabet( wszID[ i ] )	== TRUE ) continue;
			}
			break;
		case ConstServiceArea_China:
		case ConstServiceArea_Taiwan:
		case ConstServiceArea_NHNChina:
			{
				if ( IsUnifiedHanja( wszID[ i ] )	== TRUE ) continue;
				if ( IsNumber( wszID[ i ] )		== TRUE ) continue;
				if ( IsAlphabet( wszID[ i ] )	== TRUE ) continue;
			}
			break;
		case ConstServiceArea_English:
		case ConstServiceArea_USA:
		case ConstServiceArea_EUROPE:
			{
				if ( IsNumber( wszID[ i ] )		== TRUE ) continue;
				if ( IsAlphabet( wszID[ i ] )	== TRUE ) continue;
			}
			break;
		case ConstServiceArea_Korea:
			{
				if ( IsNumber( wszID[ i ] )		== TRUE ) continue;
				if ( IsAlphabet( wszID[ i ] )	== TRUE ) continue;
				if ( IsHangul( wszID[ i ] )		== TRUE ) continue;
			}
			break;
		}
	/*
		if( wszID[ i ] >= L'0' && wszID[ i ] <= L'9' ) continue;		// 숫자이면 통과 

		if( wszID[ i ] >= L'A' && wszID[ i ] <= L'Z' ) continue;		// 영문자 대문자이면 통과 

		if( wszID[ i ] >= L'a' && wszID[ i ] <= L'z' ) continue;		// 영문자 소문자이면 통과

	#ifdef _KOREA
		// 한국버젼
		if( wszID[ i ] >= 0xAC00 && wszID[ i ] <= 0xD7A3 ) continue;	// 한글이면 통과( 특수문자 제외 )
	#endif

#ifndef _ENGLISH

	#ifdef _CHINA
		if( wszID[ i ] >= 0x4E00 && wszID[ i ] <= 0x9FFF ) continue;	// 한중일공통
	#else
		if( wszID[ i ] >= 0x2E80 && wszID[ i ] <= 0x2EFF ) continue;	// CJK 한자부수

		if( wszID[ i ] >= 0x3000 && wszID[ i ] <= 0x303F ) continue;	// CJK Symbols and Punctuation

		if( wszID[ i ] >= 0x3040 && wszID[ i ] <= 0x309F ) continue;	// 히라가나

		if( wszID[ i ] >= 0x30A0 && wszID[ i ] <= 0x30FF ) continue;	// 카타카나

		if( wszID[ i ] >= 0x3300 && wszID[ i ] <= 0x33FF ) continue;	// 한중일 호환

		if( wszID[ i ] >= 0x3400 && wszID[ i ] <= 0x4DB5 ) continue;	// 한자확장

		if( wszID[ i ] >= 0x4E00 && wszID[ i ] <= 0x9FFF ) continue;	// 한중일공통

		if( wszID[ i ] >= 0xF900 && wszID[ i ] <= 0xFAFF ) continue;	// 한중일호환

		if( wszID[ i ] >= 0xFF00 && wszID[ i ] <= 0xFFEF ) continue;	// HalfWidth and Fullwidth Forms

		if( wszID[ i ] >= 0xFE30 && wszID[ i ] <= 0xFE4F ) continue;	// 일어기타

		if( wszID[ i ] >= 0x1D400 && wszID[ i ] <= 0x1D7FF ) continue;	// 일어기타

		if( wszID[ i ] >= 0x2F800 && wszID[ i ] <= 0x2FA1F ) continue;	// 일어기타
	#endif


#endif

	*/
		return FALSE;
	}

	return TRUE;
}
