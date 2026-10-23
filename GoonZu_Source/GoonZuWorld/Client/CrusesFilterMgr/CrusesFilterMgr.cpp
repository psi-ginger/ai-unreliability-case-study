#include "CrusesFilterMgr.h"
#include "CrusesFilterBaseInfo.h"

#include "../Common/SMLib/LibList/LibList.h"
#include "../Common/SMLib/LibList/Obj.h"

CCrusesFilterMgr::CCrusesFilterMgr() : CLibListMgr()
{
	CreateMaxEmptyObj( MAX_CRUSES_OBJ_NUM, MAX_CRUSES_OBJ_BUF_SIZE );

	ReadTxt( TEXT("Cruses\\Cruses.txt") );
}

CCrusesFilterMgr::~CCrusesFilterMgr()
{
}

void CCrusesFilterMgr::Init()
{

	return;
}

void CCrusesFilterMgr::ReadTxt( TCHAR *FileName )
{
	TCHAR szTrim[]   = TEXT(" ,\t\n\r");

	FILE *fp = _tfopen( FileName, TEXT("rt") );

	if ( fp == NULL )
	{
		return;
	}

	TCHAR CrusesWord[ MAX_CRUSES_OBJ_BUF_SIZE ] = TEXT("");
	SI32 CrusesWordLen = 0;

	TCHAR TempCrusesWord[ MAX_CRUSES_OBJ_BUF_SIZE ] = TEXT("");

	while( _fgetts( CrusesWord, MAX_CRUSES_OBJ_BUF_SIZE, fp ) != NULL )
	{
		if ( CrusesWord != NULL )
		{
			CrusesWordLen = _tcslen( CrusesWord );

			if ( CrusesWordLen < 1 || CrusesWordLen > MAX_CRUSES_OBJ_BUF_SIZE )
			{
				continue;
			}

			if ( CrusesWord[ CrusesWordLen - 1 ] == '\n' )
			{
				CrusesWord[ CrusesWordLen - 1 ] = NULL;
			}

			TrimChar( CrusesWord, TempCrusesWord, ' ' );

			CObj *pObj = PopFrontToEmptyListAndPushBackToExistList();
			
			if ( pObj )
			{
				pObj->SetData( (BYTE*)TempCrusesWord, CrusesWordLen + 1 );
			}
		}
		else
		{
			break;
		}
	}

	fclose( fp );

	return;
}

bool CCrusesFilterMgr::FindCruses( TCHAR *TextData )
{
	CObj *pObj = GetExistList()->GetFirst();

	SI32 CrusesWordLen = 0;

	bool Find = false;

	TCHAR TempTextData[ MAX_CRUSES_OBJ_BUF_SIZE ];
	
	TrimChar( TextData, TempTextData, ' ' );

	while( pObj )
	{
		TCHAR *CrusesWord = (TCHAR*)pObj->GetDataPtr();

		TCHAR *pDest = _tcsstr( TempTextData, CrusesWord );

		if ( pDest != NULL )
		{
			CrusesWordLen = _tcslen( CrusesWord );

			for ( SI16 i = 0; i < CrusesWordLen; ++i )
			{
				pDest[ i ] = '*';
			}

			if ( ! Find )
			{
				Find = true;
			}

			break;
		}
		
		pObj = pObj->GetNext();
	}

	return Find;
}

void CCrusesFilterMgr::TrimChar( TCHAR *InTextData, TCHAR *OutTextData, TCHAR ch )
{
	int Count = 0;

	for ( int i = 0; i < MAX_CRUSES_OBJ_BUF_SIZE; ++i )
	{
		if ( InTextData[ i ] == ch )
		{
			continue;
		}
		
		OutTextData[ Count++ ] = InTextData[ i ];
	}

	OutTextData[ Count ] = NULL;

	return;
}