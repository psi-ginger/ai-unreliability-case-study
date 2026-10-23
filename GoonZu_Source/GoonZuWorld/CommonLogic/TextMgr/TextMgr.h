#ifndef _TEXTMGR_H
#define _TEXTMGR_H

#include "../../lib/Directives.h"
#include <stdio.h>

#define MAX_TEXTMGR_NUM		6000

/*
class cltTextMgr
{
public:
	cltTextMgr();
	~cltTextMgr();

	void Create(TCHAR * szFileName);

	TCHAR * GetTextMgr(SI32 Unique);
	TCHAR m_szNULL[8] ;

private:
	BOOL OpenFile(TCHAR * szFileName);

	TCHAR * m_szText[MAX_TEXTMGR_NUM];

};
*/
//extern cltTextMgr g_pTextMgr;
//extern cltTextMgr g_pStructureMgr;

// 텍스트 매니저에서 텍스트를 얻어온다.
TCHAR * GetTxtFromMgr(SI32 index , bool bStructure = false );
// 아이템 텍스트 매니저에서 텍스트를 얻어온다.
TCHAR * GetItemTxtFromMgr(SI32 index);

// 헬프매니저에서 텍스트를 얻어온다.
TCHAR * GetHelpFromMgr(SI32 index);

// 영문화 텍스트는 따로 한번 읽어준다
#define MAX_TOKEN	30
#define MAX_STRING_LENGTH 256
extern TCHAR tokens[ MAX_TOKEN ][ MAX_STRING_LENGTH ];

void ParseText( TCHAR *str );

#endif
