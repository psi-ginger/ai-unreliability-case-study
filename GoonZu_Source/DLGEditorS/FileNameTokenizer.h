#pragma once

class CFileNameTok
{
public:
	CFileNameTok();
	~CFileNameTok();

	void InitFullPath( char *szFullPath );
	void Initialize();

public:
	char	m_szPath[ 128 ];
	char	m_szFileName[ 128 ];
	char	m_szExt[ 32 ];

};

