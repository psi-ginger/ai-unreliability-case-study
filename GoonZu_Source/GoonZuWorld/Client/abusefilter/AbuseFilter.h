#pragma once

#include <Directives.h>
#include <stdio.h>

class CAbuseFilter
{
public:
	CAbuseFilter();
	~CAbuseFilter(void);

protected:
	TCHAR *m_pBufferPool;
	TCHAR **m_szBuffer;
	TCHAR **m_szWhiteBuffer;
	SI32 m_siMaxNum;
	SI32 m_siMaxWhiteNum;

protected:
	BOOL IsAbuse( TCHAR *buf1,TCHAR *buf2, SI16 txtSize );

public:
	void Create(TCHAR * szFileName, TCHAR *szFileName2 );
	bool CheckAbuse(TCHAR * buffer);	// 부적절한 단어를 *로 교체해준다.

	bool CheckAbuseWord(TCHAR * buffer);	// 부적절한 단어를 체크한다. 있으면 true


};