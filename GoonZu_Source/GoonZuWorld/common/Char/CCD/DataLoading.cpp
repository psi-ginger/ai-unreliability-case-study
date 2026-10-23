#include "DataLoading.h"
#include "Directives.h"

#include "CommonLogic.h"

CDataLoading g_BaseDataMgr;

CDataLoading::CDataLoading()
{
	m_pstAction = NULL;

	memset ( m_strImageFileNameList, 0, sizeof( m_strImageFileNameList ) );
	m_siImageFileNum = 0;

	ReadHelmetCode();
	ReadWeaponCode();
	ReadMantleCode();
	ReadHorseCode();
	//ReadFileList();
	//LoadMinFileData();

	ReadActionCode();
	SetAttackFrameNum();
}

CDataLoading::~CDataLoading()
{
	if ( m_pstHelmet )
	{
		delete [] m_pstHelmet;
		m_pstHelmet = NULL;
	}

	if ( m_pstArmor )
	{
		delete [] m_pstArmor;
		m_pstArmor = NULL;
	}

	if ( m_pstWeapon )
	{
		delete [] m_pstWeapon;
		m_pstWeapon = NULL;
	}

	if ( m_pstMantle )
	{
		delete [] m_pstMantle;
		m_pstMantle = NULL;
	}

	if ( m_pstHorse )
	{
		delete [] m_pstHorse;
		m_pstHorse = NULL;
	}

	
	if ( m_pstAction )
	{
		delete [] m_pstAction;
		m_pstAction = NULL;
	}
	
}

void CDataLoading::ReadActionCode()
{
	TCHAR temp[64] = TEXT("");
	m_cActionNum = 0;

	//m_pstAction = NULL;

	FILE *fp = _tfopen( TEXT("CCD\\Txt\\ActionCode.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		MessageBox( NULL, TEXT("Load ActionCode.txt Error"), TEXT("error"), MB_OK|MB_TOPMOST );
		return;
	}

	_ftscanf( fp, TEXT("%s %s %s %d %s %s %s %s %s %s"), temp, temp, temp, &m_cActionNum, temp, temp, temp, temp, temp, temp );

	m_pstAction = new stActionCode[ m_cActionNum ];
	memset( m_pstAction, 0, sizeof( stActionCode ) * m_cActionNum );

	for ( int j = 0; j < m_cActionNum; ++j )
	{
		_ftscanf( fp, TEXT("%d %s %s %d %d %d"), &m_pstAction[j].cActionIndex, m_pstAction[j].strActionCode, m_pstAction[j].strActionName, &m_pstAction[j].cFrameNum, &m_pstAction[j].cTotalFrameNum, &m_pstAction[j].cDirection );
	}

	fclose( fp );
}

void CDataLoading::SetAttackFrameNum()
{
	m_pstAction[ HSU1 ].cAttackFrameNum = 3;
	m_pstAction[ HTU2 ].cAttackFrameNum = 3;
	m_pstAction[ HBU2 ].cAttackFrameNum = 6;
	m_pstAction[ HGU2 ].cAttackFrameNum = 5;

	m_pstAction[ HSR1 ].cAttackFrameNum = 6;
	m_pstAction[ HTR2 ].cAttackFrameNum = 6;
	m_pstAction[ HBR2 ].cAttackFrameNum = 5;
	m_pstAction[ HGR2 ].cAttackFrameNum = 5;

	return;
}

void CDataLoading::ReadHelmetCode()
{
	TCHAR temp[64] = TEXT("");

	m_pstHelmet = NULL;
	m_uiHelmetNum = 0;

	FILE *fp = _tfopen( TEXT("CCD\\Txt\\Helmet.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		MessageBox( NULL, TEXT("Load Helmet.txt Error"), TEXT("error"), MB_OK|MB_TOPMOST );
		return;
	}

	_ftscanf( fp, TEXT("%s %s %s %d %s %s %s"), temp, temp, temp, &m_uiHelmetNum, temp, temp, temp );

	m_pstHelmet = new stHelmetCode[ m_uiHelmetNum ];
	memset( m_pstHelmet, 0, sizeof( stHelmetCode ) * m_uiHelmetNum );

	for ( unsigned int j = 0; j < m_uiHelmetNum; ++j )
	{
		_ftscanf( fp, TEXT("%s %s %d"), m_pstHelmet[j].strHelmetName, m_pstHelmet[j].strHelmetCode, &m_pstHelmet[j].uiHelmetIndex );
	}

	fclose( fp );

	return;
}

void CDataLoading::ReadArmorCode()
{
	TCHAR temp[64] = TEXT("");

	m_pstArmor = NULL;
	m_uiArmorNum = 0;

	FILE *fp = _tfopen( TEXT("CCD\\Txt\\Armor.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox( NULL, TEXT("Load Armor.txt Error"), TEXT("error"), MB_OK|MB_TOPMOST );
		return;
	}

	_ftscanf( fp, TEXT("%s %s %s %d %s %s %s"), temp, temp, temp, &m_uiArmorNum, temp, temp, temp );

	m_pstArmor = new stArmorCode[ m_uiArmorNum ];
	memset( m_pstArmor, 0, sizeof( stArmorCode ) * m_uiArmorNum );

	for ( unsigned int j = 0; j < m_uiArmorNum; ++j )
	{
		_ftscanf( fp, TEXT("%s %s %d"), m_pstArmor[j].strArmorName, m_pstArmor[j].strArmorCode, &m_pstArmor[j].uiArmorIndex );
	}

	fclose( fp );


	return;
}

void CDataLoading::ReadWeaponCode()
{
	TCHAR temp[64] = TEXT("");

	m_pstWeapon = NULL; 
	m_uiWeaponNum = 0;

	FILE *fp = _tfopen( TEXT("CCD\\Txt\\Weapon.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox( NULL, TEXT("Load Weapon.txt Error"), TEXT("error"), MB_OK|MB_TOPMOST );
		return;
	}

	_ftscanf( fp, TEXT("%s %s %s %d %s %s %s %s %s"), temp, temp, temp, &m_uiWeaponNum, temp, temp, temp, temp, temp );

	m_pstWeapon = new stWeaponCode[ m_uiWeaponNum ];
	memset( m_pstWeapon, 0, sizeof( stWeaponCode )  * m_uiWeaponNum );

	for ( unsigned int j = 0; j < m_uiWeaponNum; ++j )

	{
		_ftscanf( fp, TEXT("%s %s %d %s %d"), m_pstWeapon[ j ].strWeaponName, m_pstWeapon[ j ].strWeaponCode, &m_pstWeapon[ j ].cWeaponAction, m_pstWeapon[ j ].strWeaponActionCode, &m_pstWeapon[ j ].uiWeaponIndex );
	}

	fclose( fp );

	return;
}

void CDataLoading::ReadMantleCode()
{
	TCHAR temp[64] = TEXT("");

	m_pstMantle = NULL;
	m_uiMantleNum = 0;

	FILE *fp = _tfopen( TEXT("CCD\\Txt\\Mantle.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox( NULL, TEXT("Load Mantle.txt Error"), TEXT("error"), MB_OK|MB_TOPMOST );
		return;
	}

	_ftscanf( fp, TEXT("%s %s %s %d %s %s %s"), temp, temp, temp, &m_uiMantleNum, temp, temp, temp );

	m_pstMantle = new stMantleCode[ m_uiMantleNum ];
	memset( m_pstMantle, 0, sizeof( stMantleCode ) * m_uiMantleNum );

	for ( unsigned int j = 0; j < m_uiMantleNum; ++j )
	{
		_ftscanf( fp, TEXT("%s %s %d"), m_pstMantle[ j ].strMantleName, m_pstMantle[ j ].strMantleCode, &m_pstMantle[ j ].uiMantleIndex );
	}

	fclose( fp );

	return;
}

void CDataLoading::ReadHorseCode()
{
	TCHAR temp[64] = TEXT("");

	m_pstHorse = NULL;
	m_uiHorseNum = 0;

	FILE *fp = _tfopen( TEXT("CCD\\Txt\\Horse.txt"), TEXT("rt") );

	if ( fp == NULL )
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox( NULL, TEXT("Load Horse.txt Error"), TEXT("error"), MB_OK|MB_TOPMOST );
		return;
	}

	_ftscanf( fp, TEXT("%s %s %s %d %s %s %s"), temp, temp, temp, &m_uiHorseNum, temp, temp, temp );

	m_pstHorse = new stHorseCode[ m_uiHorseNum ];
	memset( m_pstHorse, 0, sizeof( stHorseCode ) * m_uiHorseNum );

	for ( unsigned int j = 0; j < m_uiHorseNum; ++j )
	{
		_ftscanf( fp, TEXT("%s %s %d"), m_pstHorse[ j ].strHorseName, m_pstHorse[ j ].strHorseCode, &m_pstHorse[ j ].uiHorseIndex );
	}

	fclose( fp );

	return;
}


void CDataLoading::ReadFileList()
{
	DWORD dwBufferLen = _MAX_PATH-1;
	TCHAR Buffer[_MAX_PATH];

	m_siImageFileNum = 0;	

	::GetCurrentDirectory( dwBufferLen, Buffer );

	StringCchCat( Buffer, _MAX_PATH, TEXT("CCD\\ImageData\\*.spr") );

	WIN32_FIND_DATA FileFind;
	HANDLE hSearch = ::FindFirstFile( Buffer, &FileFind );

	while(  hSearch != INVALID_HANDLE_VALUE )
	{
		StringCchCopy( m_strImageFileNameList[ m_siImageFileNum++ ], MAX_PATH, FileFind.cFileName );
		if ( ::FindNextFile( hSearch, &FileFind ) == 0 )
		{
			break;
		}
	}

	::FindClose( hSearch );
}

void CDataLoading::LoadMinFileData()
{
	DWORD dwBufferLen = _MAX_PATH-1;
	TCHAR Buffer[_MAX_PATH];

	m_siMinFileNum = 0;	

	::GetCurrentDirectory( dwBufferLen, Buffer );


	int BufLen = _tcslen( Buffer );

	if( Buffer [ BufLen - 1 ] == '\\' )
	{
		StringCchCat( Buffer, _MAX_PATH, TEXT("CCD\\MinData\\*.Min") );
	}
	else
	{
		StringCchCat( Buffer, _MAX_PATH, TEXT("\\CCD\\MinData\\*.Min") );
	}



	WIN32_FIND_DATA FileFind;
	HANDLE hSearch = ::FindFirstFile( Buffer, &FileFind );

	while(  hSearch != INVALID_HANDLE_VALUE )
	{
		StringCchCopy( m_strImageFileNameList[ m_siImageFileNum++ ], _MAX_PATH, FileFind.cFileName );

		if ( ::FindNextFile( hSearch, &FileFind ) == 0 )
		{
			break;
		}
	}

	::FindClose( hSearch );
	return;
}


TCHAR *CDataLoading::GetHelmetName( char cIndex )
{
	return m_pstHelmet[ cIndex ].strHelmetName;
}

TCHAR *CDataLoading::GetHelmetCode( char cIndex )
{
	return m_pstHelmet[ cIndex ].strHelmetCode;
}

unsigned int CDataLoading::GetHelmetNum()
{
	return m_uiHelmetNum;
}

TCHAR *CDataLoading::GetArmorName( char cIndex )
{
	return m_pstArmor[ cIndex ].strArmorName;
}

TCHAR *CDataLoading::GetArmorCode( char cIndex )
{
	return m_pstArmor[ cIndex ].strArmorCode;
}

unsigned int CDataLoading::GetArmorNum()
{
	return m_uiArmorNum;
}

TCHAR *CDataLoading::GetWeaponName( char cIndex )
{
	return m_pstWeapon[ cIndex ].strWeaponName;
}

TCHAR *CDataLoading::GetWeaponCode( char cIndex )
{
	return m_pstWeapon[ cIndex ].strWeaponCode;
}

int CDataLoading::GetWeaponActionIndex( char cIndex )
{
	return m_pstWeapon[ cIndex ].cWeaponAction;
}

unsigned int CDataLoading::GetWeaponNum()
{
	return m_uiWeaponNum;
}

TCHAR *CDataLoading::GetMantleName( char cIndex )
{
	return m_pstMantle[ cIndex ].strMantleName;
}
TCHAR *CDataLoading::GetMantleCode( char cIndex )
{
	return m_pstMantle[ cIndex ].strMantleCode;
}

unsigned int CDataLoading::GetMantleNum()
{
	return m_uiMantleNum;
}

TCHAR *CDataLoading::GetHorseName( char cIndex )
{
	return m_pstHorse[ cIndex ] .strHorseName;
}

TCHAR *CDataLoading::GetHorseCode( char cIndex )
{
	return m_pstHorse[ cIndex ] .strHorseCode;
}

unsigned int CDataLoading::GetHorseNum()
{
	return m_uiHorseNum;
}

void CDataLoading::GetWeaponInfo( TCHAR *strWeapon, short &ssWeaponActionIndex )
{
	for ( unsigned int i = 0; i < m_uiWeaponNum; ++ i )
	{
		if ( _tcscmp( strWeapon, m_pstWeapon[i].strWeaponCode ) == 0 )
		{
			ssWeaponActionIndex = m_pstWeapon[i].cWeaponAction;
			//_tcscpy( strWeaponActionCode, m_pstWeapon[i].strWeaponActionCode );
			return;
		}
	}

	return;
}

