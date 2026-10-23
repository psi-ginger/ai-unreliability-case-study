#ifndef _HelpMgr_H
#define _HelpMgr_H

#include <Directives.h>
#include <stdio.h>

#define MAX_HelpMgr_NUM		100

class cltHelpMgr
{
public:
	cltHelpMgr();
	~cltHelpMgr();

	void Create(TCHAR * szFileName);

	TCHAR * GetHelpMgr(SI32 Unique);
	TCHAR m_szNULL[8];

private:
	BOOL OpenFile(TCHAR * szFileName);

#ifdef _SAFE_MEMORY
	NSafeTArray<TCHAR *, MAX_HelpMgr_NUM>		m_szText;
#else
	TCHAR * m_szText[MAX_HelpMgr_NUM];
#endif

};

extern cltHelpMgr g_pHelpMgr;
extern cltHelpMgr g_pHelpStructureMgr;

// 텍스트 매니저에서 텍스트를 얻어온다.
TCHAR * GetHelpFromMgr(SI32 index , bool bStructure = false );

#endif
