#ifndef _AUTOMACROMANAGER_H_
#define _AUTOMACROMANAGER_H_

//#pragma comment(lib, "Coredll.lib")

#include "CommonLogic.h"

#define MAX_PROCESS_NAME_COUNT				16														// 프로세스 개수.
#define NEW_PROCESS_NAME(Array, str)		Array = new TCHAR[256]; memcpy(Array, str, 256);		// 프로세스 이름.
#define SAFE_DELETE_ARRAY(p)				{ if(p) { delete[] (p);   (p)=NULL; } }

class CAutoMacroManager
{
private:
	TCHAR*	m_pszProcessName[MAX_PROCESS_NAME_COUNT];


public:
	CAutoMacroManager(void);
	~CAutoMacroManager(void);

	void Init(void);
	void ShutDown(void);
	
	bool IsCheckMacro(void);
	bool Undate(void);
};

#endif