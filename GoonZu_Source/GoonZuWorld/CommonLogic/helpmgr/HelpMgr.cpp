#include "commonlogic.h"

#include "HelpMgr.h"

extern cltCommonLogic* pclClient;

#define Free(p)		free(p); p = NULL;

cltHelpMgr g_pHelpMgr;
cltHelpMgr g_pHelpStructureMgr;

cltHelpMgr::cltHelpMgr()
{
	for ( int i = 0 ; i < MAX_HelpMgr_NUM ; i ++ )
		m_szText[i] = NULL ;

	MStrCpy(m_szNULL,TEXT("NULL"),8);
}

cltHelpMgr::~cltHelpMgr()
{
	for ( int i = 0 ; i < MAX_HelpMgr_NUM ; i ++ )
	{
		if ( m_szText[i] != NULL )
		{
			Free(m_szText[i]);
		}
	}
}

void cltHelpMgr::Create(TCHAR * szFileName)
{
	if ( !OpenFile(szFileName) )
		MsgBox(TEXT("error"),TEXT("can't open %s file!!"),szFileName);
}

BOOL cltHelpMgr::OpenFile(TCHAR * szFileName)
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

	// 초반 4줄 무시
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
		if ( j > MAX_HelpMgr_NUM )
		{
			MsgBox(TEXT("error"),TEXT("OverFlow %s! MAX_HelpMgr_NUM is %d"),szFileName,MAX_HelpMgr_NUM);
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

	return FALSE;
}

TCHAR * cltHelpMgr::GetHelpMgr(SI32 Unique)
{
	if ( Unique <= 0 || Unique > MAX_HelpMgr_NUM ) return NULL ;
	return m_szText[Unique-1];
}

// 텍스트 매니저에서 텍스트를 얻어온다.
TCHAR * GetHelpFromMgr(SI32 index , bool bStructure )
{
	TCHAR * buffer = NULL ;

	if ( bStructure )
	{
		buffer = g_pHelpStructureMgr.GetHelpMgr(index);
		if ( !buffer )
		{
#ifdef _DEBUG
			MsgBox(TEXT("NoStructureMsg"),TEXT("%d"),index);
#endif
			return g_pHelpStructureMgr.m_szNULL;
		}
	}
	else
	{
		buffer = g_pHelpMgr.GetHelpMgr(index);
		if ( !buffer )
		{
#ifdef _DEBUG
			MsgBox(TEXT("NoTextMsg"),TEXT("%d"),index);
#endif
			return g_pHelpMgr.m_szNULL;
		}
	}

	return buffer ;
}
