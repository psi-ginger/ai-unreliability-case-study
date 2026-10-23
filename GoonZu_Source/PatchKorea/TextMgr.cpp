#include "TextMgr.h"


#define Free(p)		free(p); p = NULL;

cltTextMgr g_pTextMgr;

//char tokens[ MAX_TOKEN ][ MAX_STRING_LENGTH ];

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

void cltTextMgr::Create( char *Language )
{
}

BOOL cltTextMgr::OpenFile(char * szFileName)
{
	FILE * fp = NULL ;

	char str[MAX_LENGTH] = "" ;
	char * buf = NULL ;

	int textlength = 0 ;
	int oldlength = 0 ;
	int locate;

	fp = fopen(szFileName,"r");
	if ( fp == NULL ) return FALSE ;

	// 초반 4줄 무시
	for ( int i = 0 ; i < 4 ; i ++ )
		fgets(str,MAX_LENGTH,fp);

	oldlength = ftell(fp);

	while( true )
	{
		if ( fgets(str,MAX_LENGTH,fp) == NULL ) break ;
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
			MsgBox("error","OverFlow %s! MAX_TEXTMGR_NUM is %d",szFileName,MAX_TEXTMGR_NUM);
			break ;
		}
		int k = 0 ;
		while(true)
		{
			j /= 10 ;
			if ( j == 0 ) break ;
			k ++ ;
		}
	
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

char * cltTextMgr::GetTextMgr(int Unique)
{
	if ( Unique <= 0 || Unique > MAX_TEXTMGR_NUM ) return NULL ;
	return m_szText[Unique-1];
}

// 텍스트 매니저에서 텍스트를 얻어온다.
char * GetTxtFromMgr(int index , bool bStructure )
{
	char * buffer = NULL ;


	buffer = g_pTextMgr.GetTextMgr(index);
	if ( !buffer )
	{
#ifdef _DEBUG
		//MsgBox("NoTextMsg","%d",index);
#endif
		return g_pTextMgr.m_szNULL;
	}


	return buffer ;
}

void MStrCpy( char *pDest, const char *pSrc, int maxbuf )
{
	if( pSrc == NULL ) {
		pDest[ 0 ] = NULL;
	
	} else {
		memcpy( pDest, pSrc, maxbuf );
		pDest[ maxbuf - 1 ] = NULL;
	}
}






void MsgBox(char * szCaption, char*szFormat,... )
{
	char szBuffer[256];
    char *pArguments;

	pArguments=( char * )&szFormat + sizeof( szFormat );
	vsprintf( szBuffer, szFormat, pArguments );

	MessageBox(NULL, szBuffer, szCaption, MB_OK|MB_TOPMOST);
}

