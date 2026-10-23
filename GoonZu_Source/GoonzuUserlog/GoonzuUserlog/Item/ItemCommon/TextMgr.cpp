#include "stdafx.h"
#include "TextMgr.h"
#include "commonutil.h"

#define Free(p)		free(p); p = NULL;

cltTextMgr g_pTextMgr;
cltTextMgr g_pStructureMgr;

char tokens[ MAX_TOKEN ][ MAX_STRING_LENGTH ];

cltTextMgr::cltTextMgr()
{
	for ( int i = 0 ; i < MAX_TEXTMGR_NUM ; i ++ )
		m_szText[i] = NULL ;

	MStrCpy(m_szNULL,"NULL",8);
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

void cltTextMgr::Create(char * szFileName)
{
	if ( !OpenFile(szFileName) )
	{
		MessageBox( NULL, "sdf", "SDf", MB_OK );
	}
		
}

BOOL cltTextMgr::OpenFile(char * szFileName)
{
	FILE * fp = NULL ;

	char str[1024] = "" ;
	char * buf = NULL ;

	int textlength = 0 ;
	int oldlength = 0 ;
	int locate;

	BYTE	szGoonzuPath[MAX_PATH] = "";
	char	szTextMgr[512] = "";
	DWORD	dwSize = sizeof(szGoonzuPath);
	HKEY	hKey;
	LONG	lReturn;
	CString	szError;

	lReturn = RegOpenKey(HKEY_CURRENT_USER, "Software\\Intizen\\Goonzu", &hKey);
	if(lReturn != ERROR_SUCCESS)
	{
		szError.Find("레지스트리를 열 수 없습니다\nErrorCode : %d", lReturn);
		AfxMessageBox(szError);
		return TRUE;
	}
	lReturn = RegQueryValueEx(hKey, "PATH", NULL, NULL, szGoonzuPath, &dwSize);
	if(lReturn != ERROR_SUCCESS)
	{
		szError.Find("군주가 설치된 폴더를 찾을 수 없습니다\nErrorCode : %d", lReturn);
		AfxMessageBox(szError);
		return TRUE;
	}

	RegCloseKey(hKey);

	sprintf(szTextMgr, "%sTextMgr\\Textmgr.txt", szGoonzuPath);

	fp = fopen(szTextMgr,"r");
	if ( fp == NULL ) return FALSE ;

	// 초반 4줄 무시
	for ( int i = 0 ; i < 4 ; i ++ )
		fgets(str,1024,fp);

	oldlength = ftell(fp);

	while( true )
	{
		if ( fgets(str,1024,fp) == NULL ) break ;
		if ( strstr(str,"<end>") ) break ;

		textlength = ftell(fp);
		textlength -= oldlength ;
		oldlength += textlength ;

		buf = (char*)malloc(textlength);
		strcpy(buf,str);
		locate = atoi(buf)-1;
		int j = atoi(buf) ;
		if ( j > MAX_TEXTMGR_NUM )
		{
			MessageBox( NULL, "ssdf", "SDasff", MB_OK );
			break ;
		}
		int k = 0 ;
		while(true)
		{
			j /= 10 ;
			if ( j == 0 ) break ;
			k ++ ;
		}
		//memcpy(buf,buf+k+2,strlen(buf));
		m_szText[locate] = (char*)malloc(textlength);
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
}

char * cltTextMgr::GetTextMgr(SI32 Unique)
{
	if ( Unique <= 0 || Unique > MAX_TEXTMGR_NUM ) return NULL ;
	return m_szText[Unique-1];
}

// 텍스트 매니저에서 텍스트를 얻어온다.
char * GetTxtFromMgr(SI32 index , bool bStructure )
{
	char * buffer = NULL ;

	if ( bStructure )
	{
		buffer = g_pStructureMgr.GetTextMgr(index);
		if ( !buffer )
		{
#ifdef _DEBUG
			MessageBox( NULL, "sasdsdf", "SasdDasff", MB_OK );
#endif
			return g_pStructureMgr.m_szNULL;
		}
	}
	else
	{
		buffer = g_pTextMgr.GetTextMgr(index);
		if ( !buffer )
		{
#ifdef _DEBUG
			MessageBox( NULL, "sasdsdgsdf", "SasasfdDasff", MB_OK );
#endif
			return g_pTextMgr.m_szNULL;
		}
	}

	return buffer ;
}

void ParseText( char *str )
{
	int i;
	int startpos = 0;

	int token_index = 0;
	int token_len = 0;
	
	int first_space = 0; 
	BOOL bSpace = FALSE;

	int len = strlen( str );

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
