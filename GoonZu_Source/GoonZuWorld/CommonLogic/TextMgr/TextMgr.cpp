#include "../../CommonLogic/CommonLogic.h"

#include "TextMgr.h"
#include "../../NLib/NTextManager.h"

extern cltCommonLogic* pclClient;

#define Free(p)		free(p); p = NULL;

//cltTextMgr g_pTextMgr;
//cltTextMgr g_pStructureMgr;
NTextManager g_pTextManager;
NTextManager g_pItemTextManager;
NTextManager g_pQuestTextManager;

TCHAR tokens[ MAX_TOKEN ][ MAX_STRING_LENGTH ];

/*
cltTextMgr::cltTextMgr()
{
	for ( int i = 0 ; i < MAX_TEXTMGR_NUM ; i ++ )
		m_szText[i] = NULL ;

	MStrCpy(m_szNULL,TEXT("NULL"),8);
}

cltTextMgr::~cltTextMgr()
{
	for ( int i = 0 ; i < MAX_TEXTMGR_NUM ; i ++ )
	{
		if ( m_szText[i] != NULL )
		{
			Free(m_szText[i]);
		}
	}
}
*/
/*
void cltTextMgr::Create(TCHAR * szFileName)
{
	if ( !OpenFile(szFileName) )
		MsgBox(TEXT("error"),TEXT("can't open %s file!!"),szFileName);
}
*/

/*
BOOL cltTextMgr::OpenFile(TCHAR * szFileName)
{
	/*
	FILE * fp = NULL ;

	TCHAR str[1024] = TEXT("") ;
	TCHAR * buf = NULL ;

	int textlength = 0 ;
	int oldlength = 0 ;
	int locate;

	fp = _tfopen(szFileName, TEXT("r"));
	if ( fp == NULL ) return FALSE ;

	// �ʹ� 4�� ����
	for ( int i = 0 ; i < 4 ; i ++ )
		_fgetts(str,1024,fp);

	oldlength = ftell(fp);

	while( true )
	{
		if ( _fgetts(str,1024,fp) == NULL ) break ;
		if ( strstr(str, TEXT("<end>")) ) break ;

		textlength = ftell(fp);
		textlength -= oldlength ;
		oldlength += textlength ;

		buf = (TCHAR*)malloc(textlength);
		strcpy(buf,str);
		locate = _tstoi(buf)-1;
		int j = _tstoi(buf) ;
		if ( j > MAX_TEXTMGR_NUM )
		{
			MsgBox(TEXT("error"),TEXT("OverFlow %s! MAX_TEXTMGR_NUM is %d"),szFileName,MAX_TEXTMGR_NUM);
			break ;
		}
		int k = 0 ;
		while(true)
		{
			j /= 10 ;
			if ( j == 0 ) break ;
			k ++ ;
		}
		//memcpy(buf,buf+k+2,_tcslen(buf));
		m_szText[locate] = (TCHAR*)malloc(textlength);
		strcpy(m_szText[locate],&buf[k+2]);
		m_szText[locate][textlength-4-k] = NULL ;

		for ( k = 0 ; k < textlength ; k ++ )
		{
			if ( (m_szText[locate][k] == '\\' && m_szText[locate][k+1] == 'r' ) && m_szText[locate][k+1] != NULL )
			{
				m_szText[locate][k] = '\r';
				m_szText[locate][k+1] = '\n';
			}
			else if ( (m_szText[locate][k] == '\\' && m_szText[locate][k+1] == 't' ) && m_szText[locate][k+1] != NULL )
			{
				m_szText[locate][k] = '\r';
				m_szText[locate][k+1] = '\t';
			}
		}

		Free(buf);
	}

	fclose(fp);
	
	return TRUE ;

	*/
/*	return FALSE;

}
*/

/*
TCHAR * cltTextMgr::GetTextMgr(SI32 Unique)
{
	if ( Unique <= 0 || Unique > MAX_TEXTMGR_NUM ) return NULL ;
	return m_szText[Unique-1];
}
*/

// �ؽ�Ʈ �Ŵ������� �ؽ�Ʈ�� ���´�
TCHAR * GetTxtFromMgr(SI32 index , bool bStructure )
{
	TCHAR * buffer = NULL ;
	TCHAR strIndex[MAX_TEXT_KEY] = TEXT("");
//	TCHAR szNULL[8] = TEXT("");

	if ( bStructure )
	{
		StringCchPrintf(strIndex, MAX_TEXT_KEY, TEXT("%d"), index+15000);//15000���� �̻��� �ؽ�Ʈ�� �ε���+15000

		buffer = g_pTextManager.GetText(strIndex);
		if (!buffer)
		{
#ifdef _SERVER
			OutputDebugString(TEXT("[TextMgr] NoStructureMsg missing index: "));
			OutputDebugString(strIndex);
			OutputDebugString(TEXT("\n"));
#else
			MsgBox(TEXT("NoStructureMsg"),TEXT("%d"),index);
#endif

//			MStrCpy(szNULL,TEXT("NULL"),8);
			return TEXT("NULL");
		}

	}
	else
	{
		StringCchPrintf(strIndex, MAX_TEXT_KEY, TEXT("%d"), index);

		buffer = g_pTextManager.GetText(strIndex);
		if (!buffer)
		{
#ifdef _SERVER
			OutputDebugString(TEXT("[TextMgr] Text Manager NoTextMsg missing index: "));
			OutputDebugString(strIndex);
			OutputDebugString(TEXT("\n"));
#else
			MsgBox(TEXT("Text Manager NoTextMsg"),TEXT("%s"),strIndex);
#endif

//			MStrCpy(szNULL,TEXT("NULL"),8);
			return TEXT("NULL");
		}

	}
	    
	return buffer ;
}

// ������ �ؽ�Ʈ �Ŵ������� �ؽ�Ʈ�� ���´�.
TCHAR * GetItemTxtFromMgr( SI32 index  )
{
	static char szNULL[8] = TEXT("NULL");

	char * buffer = NULL ;
	NTCHARString64 kIndex;

	kIndex = SI32ToString( index );

	buffer = g_pItemTextManager.GetText( kIndex );
	if (!buffer)
	{
#ifdef _SERVER
		OutputDebugString( TEXT("[TextMgr] NoItemTextMsg missing index: ") );
		OutputDebugString( (TCHAR*)kIndex );
		OutputDebugString( TEXT("\n") );
#else
		MsgBox( TEXT("NoItemTextMsg"), TEXT("%s"), (TCHAR*)kIndex );
#endif
		return szNULL;
	}

	return buffer ;
}

TCHAR * GetQuestTxtFromMgr( TCHAR* pszUnique )
{
	static TCHAR szNULL[8] = TEXT("NULL");

	if ( NULL == pszUnique )
	{
		return szNULL;
	}

	TCHAR* pszBuffer = g_pQuestTextManager.GetText( pszUnique );
	if ( NULL == pszBuffer )
	{
#ifdef _SERVER
		OutputDebugString( TEXT("[TextMgr] Quest NoTextMsg missing key: ") );
		OutputDebugString( pszUnique );
		OutputDebugString( TEXT("\n") );
#else
		MsgBox( TEXT("Quest NoTextMsg"), TEXT("%s"), pszUnique );
#endif
		return szNULL;
	}

	return pszBuffer;

}

// �ؽ�Ʈ �Ŵ������� �ؽ�Ʈ�� ���´�.
TCHAR * GetHelpFromMgr(SI32 index)
{
	TCHAR * buffer = NULL ;
	TCHAR strIndex[MAX_TEXT_KEY] = TEXT("");
//	TCHAR szNULL[8] = TEXT("");

	StringCchPrintf(strIndex, MAX_TEXT_KEY, TEXT("%d"), index+20000);

	buffer = g_pTextManager.GetText(strIndex);
	if (!buffer)
	{
#ifdef _SERVER
		OutputDebugString(TEXT("[TextMgr] Help NoTextMsg missing index: "));
		OutputDebugString(strIndex);
		OutputDebugString(TEXT("\n"));
#else
		MsgBox(TEXT("Help NoTextMsg"),TEXT("%s"),strIndex);
#endif

//		MStrCpy(szNULL,TEXT("NULL"),8);
//		return szNULL;
		return TEXT("NULL");
	}

	return buffer ;
}


void ParseText( TCHAR *str )
{
	int i;
	int startpos = 0;

	int token_index = 0;
	int token_len = 0;
	
	int first_space = 0; 
	BOOL bSpace = FALSE;

	int len = _tcslen( str );

	for( i = 0; i < MAX_TOKEN; ++i ) {

		tokens[ i ][ 0 ] = NULL; 
	}


	for( int i = 0; i <= len; ++i ) 
	{
		if( str[ i ] == ' ' )
		{
			if( bSpace == FALSE )
			{
				first_space = i;
				bSpace = TRUE;
			}
			continue;
		}

		if( str[ i ] == '\t' || str[ i ] == 10 || str[ i ] == 13 || str[ i ] == NULL ) 
		{

			if( bSpace ) {

				token_len = first_space - startpos;

			} else {

				token_len = i - startpos;
			}

			if( token_len > 0 )
			{
				memcpy( tokens[ token_index ], &str[ startpos ], token_len );
			}

			tokens[ token_index ][ token_len ] = NULL;

			startpos = i + 1;
			token_index++;
		}

		bSpace = FALSE;
	}
}
