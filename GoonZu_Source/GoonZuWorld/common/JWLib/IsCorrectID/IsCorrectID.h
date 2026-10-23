#ifndef _ISCORRECTID_H_
#define _ISCORRECTID_H_

#include <windows.h>

BOOL IsCorrectID( TCHAR *szID );

BOOL IsNumber( WCHAR wChar ); // 숫자
BOOL IsAlphabet( WCHAR wChar ); // 영문
BOOL IsHangul( WCHAR wChar ); // 한글
BOOL IsUnifiedHanja( WCHAR wChar ); // 한중일 통합 한자
BOOL IsHanjaRadicals( WCHAR wChar ); // 한자 부수
BOOL IsHanjaSymbolsNPunctuation( WCHAR wChar ); // 한자 기호
BOOL IsHiragana( WCHAR wChar ); // 히라가나
BOOL IsKatakana( WCHAR wChar ); // 카타카나
BOOL IsCompatibilityHanja1( WCHAR wChar ); // 호환 한자
BOOL IsHanjaExtended( WCHAR wChar ); // 한자 확장
BOOL IsCompatibilityHanja2( WCHAR wChar ); // 호환 한자
BOOL IsHalfNFullWidth( WCHAR wChar ); // 반각 자모(HalfWidth and Fullwidth Forms);
BOOL IsHanjaForms( WCHAR wChar ); // 한자 서식
BOOL IsMathematicalSymbols( WCHAR wChar ); // 수학 영문기호
BOOL IsHanjaSupplement( WCHAR wChar ); // 호환 한자 지원

#endif