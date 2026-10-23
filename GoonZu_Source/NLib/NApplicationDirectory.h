#pragma once

#include "NDataTypes.h"
#include "NString.h"

class NApplicationDirectory
{
public:
	static bool CreateInstance();
	static void DestroyInstance();
	static NApplicationDirectory* ms_kThis;
	static NApplicationDirectory* Get(){ return ms_kThis; }

private:
	NApplicationDirectory(void);
	~NApplicationDirectory(void);
	void Initialize();

public:
	NString512 m_kApplicationDirectory;
	NString512 m_kMakePath;

	static const int CON_TREELEVEL_MAX = 64;
	TCHAR* m_pTree[CON_TREELEVEL_MAX];

	int m_iTreeLevel;

public:
	static LPCTSTR GetApplicationDirectory();
	static LPCTSTR MakeRelativePath( LPCTSTR szPathName );
};

typedef NApplicationDirectory NAppDirectory;
