#include "stdafx.h"
#include "CharacterDrawing.h"
#include "XSPRMgr.h"
#include "MinDataMgr.h"
#include "MinDataObj.h"

#include "Graphic/Graphic.h"
#include "Graphic/Graphic_Capsule.h"
#include "NGraphics/Graphics.h"

CCharacterDrawing::CCharacterDrawing()
{
	m_pstAction = NULL;
	m_pMinDataMgr = new CMinDataMgr;

	memset( m_strHelmet, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strArmor, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strWeapon, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strMantle, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strEquipment, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strHorse, 0, MAX_ITEMCODE_LENGTH );

	memset( m_strFileName, 0, sizeof( m_strFileName ) );
	
	memset( m_strSex, 0, 4 );
	memset( m_strTempSex, 0, 4 );
	
	m_cSex = 1;

	memset ( m_strImageFileNameList, 0, sizeof( m_strImageFileNameList ) );
	
	m_siImageFileNum = 0;
	m_bReverse = false;
	m_bIsTakeHorse = false;
	m_bIsTakeMantle = false;
	m_bIsTakeHorseHead = false;
	m_bIsTakeEquipment = false;

	m_cCurrentActionIndex = 0;
	
	ReadActionCode();
	ReadHelmetCode();
	ReadArmorCode();
	ReadWeaponCode();
	ReadMantleCode();
	ReadHorseCode();
	ReadHorseHeadCode();

	ReadFileList();

	Initialize();
}

CCharacterDrawing::~CCharacterDrawing()
{
	Destroy();

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

	if ( m_pstHorseHead )
	{
		delete [] m_pstHorseHead;
		m_pstHorseHead = NULL;
	}

	if( m_pstAction )
	{
		delete [] m_pstAction;
		m_pstAction = NULL;
	}
}

void CCharacterDrawing::ReadActionCode()
{
	char temp[64] = "";
	m_cActionNum = 0;

	m_pstAction = NULL;

	FILE *fp = fopen( "./Txt/ActionCode.txt", "r" );

	if ( fp == NULL )
	{
		MessageBox( NULL, "Load ActionCode.txt Error", "error", MB_OK );
		return;
	}

	fscanf( fp, "%s %s %s %d %s %s %s %s %s %s", temp, temp, temp, &m_cActionNum, temp, temp, temp, temp, temp, temp );

	m_pstAction = new stActionCode[ m_cActionNum ];
	memset( m_pstAction, 0, sizeof( stActionCode ) * m_cActionNum );

	for ( int j = 0; j < m_cActionNum; ++j )
	{
		fscanf( fp, "%d %s %s %d %d %d", &m_pstAction[j].cActionIndex, m_pstAction[j].strActionCode, m_pstAction[j].strActionName, &m_pstAction[j].cFrameNum, &m_pstAction[j].cTotalFrameNum, &m_pstAction[j].cDirection );
	}

	fclose( fp );
}

void CCharacterDrawing::ReadHelmetCode()
{
	char temp[64] = "";

	m_pstHelmet = NULL;
	m_uiHelmetNum = 0;

	FILE *fp = fopen( "./Txt/Helmet.txt", "r" );
	
	if ( fp == NULL )
	{
		MessageBox( NULL, "Load Helmet.txt Error", "error", MB_OK );
		return;
	}

	fscanf( fp, "%s %s %s %d %s %s %s", temp, temp, temp, &m_uiHelmetNum, temp, temp, temp );

	m_pstHelmet = new stHelmetCode[ m_uiHelmetNum ];
	memset( m_pstHelmet, 0, sizeof( stHelmetCode ) * m_uiHelmetNum );

	for ( unsigned int j = 0; j < m_uiHelmetNum; ++j )
	{
		fscanf( fp, "%s %s %d", m_pstHelmet[j].strHelmetName, m_pstHelmet[j].strHelmetCode, &m_pstHelmet[j].uiHelmetIndex );
	}

	fclose( fp );

	return;
}

void CCharacterDrawing::ReadArmorCode()
{
	char temp[64] = "";

	m_pstArmor = NULL;
	m_uiArmorNum = 0;

	FILE *fp = fopen( "./Txt/Armor.txt", "r" );

	if ( fp == NULL )
	{
		MessageBox( NULL, "Load Armor.txt Error", "error", MB_OK );
		return;
	}

	fscanf( fp, "%s %s %s %d %s %s %s", temp, temp, temp, &m_uiArmorNum, temp, temp, temp );

	m_pstArmor = new stArmorCode[ m_uiArmorNum ];
	memset( m_pstArmor, 0, sizeof( stArmorCode ) * m_uiArmorNum );

	for ( unsigned int j = 0; j < m_uiArmorNum; ++j )
	{
		fscanf( fp, "%s %s %d", m_pstArmor[j].strArmorName, m_pstArmor[j].strArmorCode, &m_pstArmor[j].uiArmorIndex );
	}
	
	fclose( fp );


	return;
}

void CCharacterDrawing::ReadWeaponCode()
{
	char temp[64] = "";

	m_pstWeapon = NULL;
	m_uiWeaponNum = 0;

	FILE *fp = fopen( "./Txt/Weapon.txt", "r" );

	if ( fp == NULL )
	{
		MessageBox( NULL, "Load Weapon.txt Error", "error", MB_OK );
		return;
	}

	fscanf( fp, "%s %s %s %d %s %s %s %s %s", temp, temp, temp, &m_uiWeaponNum, temp, temp, temp, temp, temp );

	m_pstWeapon = new stWeaponCode[ m_uiWeaponNum ];
	memset( m_pstWeapon, 0, sizeof( stWeaponCode ) * m_uiWeaponNum);

	for ( unsigned int j = 0; j < m_uiWeaponNum; ++j )
	{
		fscanf( fp, "%s %s %d %s %d", m_pstWeapon[ j ].strWeaponName, m_pstWeapon[ j ].strWeaponCode, &m_pstWeapon[ j ].cWeaponAction, m_pstWeapon[ j ].strWeaponActionCode, &m_pstWeapon[ j ].uiWeaponIndex );
	}
	
	fclose( fp );

	return;
}

void CCharacterDrawing::ReadMantleCode()
{
	char temp[64] = "";

	m_pstMantle = NULL;
	m_uiMantleNum = 0;

	FILE *fp = fopen( "./Txt/Mantle.txt", "r" );
	FILE *fp2 = fopen( "./Txt/Equipment.txt", "r" );

	if ( fp == NULL )
	{
		MessageBox( NULL, "Load Mantle.txt Error", "error", MB_OK );
		return;
	}
	if ( fp2 == NULL )
	{
		MessageBox( NULL, "Load Equipment.txt Error", "error", MB_OK );
		return;
	}

	fscanf( fp, "%s %s %s %d %s %s %s", temp, temp, temp, &m_uiMantleNum, temp, temp, temp );
	
	int equipnum = 0;
	fscanf( fp2, "%s %s %s %d %s %s %s", temp, temp, temp, &equipnum, temp, temp, temp );
 
	m_pstMantle = new stMantleCode[ m_uiMantleNum + equipnum ];
	memset( m_pstMantle, 0, sizeof( stMantleCode ) * (m_uiMantleNum + equipnum) );

	for ( unsigned int j = 0; j < m_uiMantleNum; ++j )
	{
		fscanf( fp, "%s %s %d", m_pstMantle[ j ].strMantleName, m_pstMantle[ j ].strMantleCode, &m_pstMantle[ j ].uiMantleIndex );
	}

	for ( unsigned int j = m_uiMantleNum; j < m_uiMantleNum + equipnum; ++j )
	{
		fscanf( fp2, "%s %s %d", m_pstMantle[ j ].strMantleName, m_pstMantle[ j ].strMantleCode, &m_pstMantle[ j ].uiMantleIndex );
	}

	m_uiMantleNum = m_uiMantleNum + equipnum;

	fclose( fp2 );
	fclose( fp );

	return;
}


void CCharacterDrawing::ReadHorseCode()
{
	char temp[64] = "";

	m_pstHorse = NULL;
	m_uiHorseNum = 0;

	FILE *fp = fopen( "./Txt/Horse.txt", "r" );

	if ( fp == NULL )
	{
		MessageBox( NULL, "Load Horsr.txt Error", "error", MB_OK );
		return;
	}

	fscanf( fp, "%s %s %s %d %s %s %s", temp, temp, temp, &m_uiHorseNum, temp, temp, temp );

	m_pstHorse = new stHorseCode[ m_uiHorseNum ];
	memset( m_pstHorse, 0, sizeof( stHorseCode ) * m_uiHorseNum );

	for ( unsigned int j = 0; j < m_uiHorseNum; ++j )
	{
		fscanf( fp, "%s %s %d", m_pstHorse[ j ].strHorseName, m_pstHorse[ j ].strHorseCode, &m_pstHorse[ j ].uiHorseIndex );
	}

	fclose( fp );

	return;
}

void CCharacterDrawing::ReadHorseHeadCode()
{
	char temp[64] = "";

	m_pstHorseHead = NULL;
	m_uiHorseHeadNum = 0;

	FILE *fp = fopen( "./Txt/HorseHead.txt", "r" );

	if ( fp == NULL )
	{
		MessageBox( NULL, "Load Horsr.txt Error", "error", MB_OK );
		return;
	}

	fscanf( fp, "%s %s %s %d %s %s %s", temp, temp, temp, &m_uiHorseHeadNum, temp, temp, temp );

	m_pstHorseHead = new stHorseHeadCode[ m_uiHorseHeadNum ];
	memset( m_pstHorseHead, 0, sizeof( stHorseHeadCode ) * m_uiHorseHeadNum );

	for ( unsigned int j = 0; j < m_uiHorseHeadNum; ++j )
	{
		fscanf( fp, "%s %s %d", m_pstHorseHead[ j ].strHorseHeadName, m_pstHorseHead[ j ].strHorseHeadCode, &m_pstHorseHead[ j ].uiHorseHeadIndex );
		if(strncmp(m_pstHorseHead[ j ].strHorseHeadCode, "0", MAX_ITEMCODE_LENGTH) == 0)
		{
			m_pstHorseHead[ j ].strHorseHeadCode[0] = '\0';
		}
	}

	fclose( fp );

	return;
}


void CCharacterDrawing::ReadFileList()
{
	DWORD dwBufferLen = MAX_PATH-1;
	char Buffer[MAX_PATH];

	m_siImageFileNum = 0;	

	::GetCurrentDirectory( dwBufferLen, Buffer );

	strcat( Buffer, "\\JohapChar\\*.spr" );

	WIN32_FIND_DATA FileFind;
	HANDLE hSearch = ::FindFirstFile( Buffer, &FileFind );

	while(  hSearch != INVALID_HANDLE_VALUE )
	{
		strcpy ( m_strImageFileNameList[ m_siImageFileNum++ ], FileFind.cFileName );
		if ( ::FindNextFile( hSearch, &FileFind ) == 0 )
		{
			break;
		}
	}

	::FindClose( hSearch );
}

void CCharacterDrawing::LoadMinFileData()
{
	DWORD dwBufferLen = MAX_PATH-1;
	char Buffer[MAX_PATH];

	m_siMinFileNum = 0;	

	::GetCurrentDirectory( dwBufferLen, Buffer );

	strcat( Buffer, "\\MinData\\*.Min" );

	WIN32_FIND_DATA FileFind;
	HANDLE hSearch = ::FindFirstFile( Buffer, &FileFind );

	while(  hSearch != INVALID_HANDLE_VALUE )
	{
		strcpy ( m_strImageFileNameList[ m_siImageFileNum++ ], FileFind.cFileName );
		
		if ( ::FindNextFile( hSearch, &FileFind ) == 0 )
		{
			break;
		}
	}

	::FindClose( hSearch );
	return;
}

void CCharacterDrawing::Initialize()
{
	memset( m_strHelmet, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strArmor, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strWeapon, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strMantle, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strEquipment, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strHorse, 0, MAX_ITEMCODE_LENGTH );
	
	memset( m_strFileName, 0, sizeof( m_strFileName ) );
	
	m_ssAnimationNum = 0;
	m_cCurrentDirection = SOUTH_EAST;
	
	m_ssAnimationStartIndex = 0;
	m_ssAnimationEndIndex = 0;

	m_ssCurrentAnimationIndex = 0;

	memset( m_siDrawingSequence, 0, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );
	
	memset( m_strSex, 0, 4 );
	memset( m_strTempSex, 0, 4 );
	
	m_cSex = 1;
	
	m_bReverse = false;
	m_bIsTakeHorse = false;	
	m_bIsTakeMantle = false;
	m_bIsTakeHorseHead = false;
	m_bIsTakeEquipment = false;


	return;
}

void CCharacterDrawing::Destroy()
{
	memset( m_strHelmet, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strArmor, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strWeapon, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strMantle, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strEquipment, 0, MAX_ITEMCODE_LENGTH );
	memset( m_strHorse, 0, MAX_ITEMCODE_LENGTH );

	memset( m_strFileName, 0, sizeof( m_strFileName ) );
	
	m_ssAnimationNum = 0;
	m_cCurrentDirection = SOUTH_EAST;
	
	m_ssAnimationStartIndex = 0;
	m_ssAnimationEndIndex = 0;

	m_ssCurrentAnimationIndex = 0;

	memset( m_siDrawingSequence, 0, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );

	memset( m_strSex, 0, 4 );
	memset( m_strTempSex, 0, 4 );
	
	m_cSex = 1;

	memset ( m_strImageFileNameList, 0, sizeof( m_strImageFileNameList ) );

	m_bReverse = false;
	m_bIsTakeHorse = false;
	m_bIsTakeMantle = false;
	m_bIsTakeHorseHead = false;
	m_bIsTakeEquipment = false;

	
	if ( m_pMinDataMgr )
	{
		delete m_pMinDataMgr;
		m_pMinDataMgr = NULL;
	}

	return;
}

char CCharacterDrawing::GetCurrentDirection()
{
	return m_cCurrentDirection;
}

void CCharacterDrawing::SetCurrentDirection( char cCurrentDirection, int *pDrawSequence )
{
	m_cCurrentDirection = cCurrentDirection;

	//m_ssAnimationNum = m_pstAction[ m_cCurrentActionIndex ].cTotalFrameNum / m_pstAction[ m_cCurrentActionIndex ].cDirection;

	if ( cCurrentDirection > NORTH_WEST )
	{
		m_ssAnimationStartIndex = m_ssAnimationNum * ( NORTH_WEST - ( cCurrentDirection - NORTH_WEST ) );		
		m_bReverse = true;

		for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM; ++i )
		{
			m_siDrawingSequence[  NORTH_WEST - ( cCurrentDirection - NORTH_WEST ) ] [ i ] = pDrawSequence[i];
		}	
	}
	else
	{
		m_ssAnimationStartIndex = m_ssAnimationNum * cCurrentDirection;
		m_bReverse = false;
		
		for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM; ++i )
		{
			m_siDrawingSequence[ cCurrentDirection ] [ i ] = pDrawSequence[i];
		}
	}

	m_ssCurrentAnimationIndex = m_ssAnimationStartIndex;
	m_ssAnimationEndIndex = m_ssAnimationStartIndex + m_ssAnimationNum;
	
	return;
}

short CCharacterDrawing::GetAnimationStartIndex()
{
	return m_ssAnimationStartIndex;
}

void CCharacterDrawing::SetAnimationStartIndex( short ssAnimationStartIndex )
{
	m_ssAnimationStartIndex = ssAnimationStartIndex;
	return;
}

short CCharacterDrawing::GetAnimationEndIndex()
{
	return m_ssAnimationEndIndex;
}

void CCharacterDrawing::SetAnimationEndIndex( short ssAnimationEndIndex )
{
	m_ssAnimationEndIndex = ssAnimationEndIndex;
	return;
}

short CCharacterDrawing::GetCurrentAnimationIndex()
{
	return m_ssCurrentAnimationIndex;
}

void CCharacterDrawing::SetCurrentAnimationIndex( short ssCurrentAnimationIndex )
{
	m_ssCurrentAnimationIndex = ssCurrentAnimationIndex;
	return;
}

char *CCharacterDrawing::GetActionName( int cIndex )
{
	return m_pstAction[ cIndex ].strActionName;
}

char *CCharacterDrawing::GetActionCode( int cIndex )
{
	return m_pstAction[ cIndex ].strActionCode;
}

char CCharacterDrawing::GetActionNum()
{
	return m_cActionNum;
}

char *CCharacterDrawing::GetHelmetName( int cIndex )
{
	return m_pstHelmet[ cIndex ].strHelmetName;
}

char *CCharacterDrawing::GetHelmetCode( int cIndex )
{
	return m_pstHelmet[ cIndex ].strHelmetCode;
}

unsigned int CCharacterDrawing::GetHelmetNum()
{
	return m_uiHelmetNum;
}

char *CCharacterDrawing::GetArmorName( int cIndex )
{
	return m_pstArmor[ cIndex ].strArmorName;
}

char *CCharacterDrawing::GetArmorCode( int cIndex )
{
	return m_pstArmor[ cIndex ].strArmorCode;
}

unsigned int CCharacterDrawing::GetArmorNum()
{
	return m_uiArmorNum;
}

char *CCharacterDrawing::GetWeaponName( int cIndex )
{
	return m_pstWeapon[ cIndex ].strWeaponName;
}

char *CCharacterDrawing::GetWeaponCode( int cIndex )
{
	return m_pstWeapon[ cIndex ].strWeaponCode;
}

char CCharacterDrawing::GetWeaponActionIndex( int cIndex )
{
	return m_pstWeapon[ cIndex ].cWeaponAction;
}

unsigned int CCharacterDrawing::GetWeaponNum()
{
	return m_uiWeaponNum;
}

char *CCharacterDrawing::GetMantleName( int cIndex )
{
	return m_pstMantle[ cIndex ] .strMantleName;
}

char *CCharacterDrawing::GetMantleCode( int cIndex )
{
	return m_pstMantle[ cIndex ] .strMantleCode;
}

unsigned int CCharacterDrawing::GetMantleNum()
{
	return m_uiMantleNum;
}

char *CCharacterDrawing::GetHorseName( int cIndex )
{
	return m_pstHorse[ cIndex ] .strHorseName;
}

char *CCharacterDrawing::GetHorseCode( int cIndex )
{
	return m_pstHorse[ cIndex ] .strHorseCode;
}

unsigned int CCharacterDrawing::GetHorseNum()
{
	return m_uiHorseNum;
}

char *CCharacterDrawing::GetHorseHeadName( int cIndex )
{
	return m_pstHorseHead[ cIndex ] .strHorseHeadName;
}

char *CCharacterDrawing::GetHorseHeadCode( int cIndex )
{
	return m_pstHorseHead[ cIndex ] .strHorseHeadCode;
}

unsigned int CCharacterDrawing::GetHorseHeadNum()
{
	return m_uiHorseHeadNum;
}

void CCharacterDrawing::LoadImageDataFromList( char *strHelmet, char *strArmor, char *strWeapon, char *strMantle, char *strHorse, char *strHorseHead )
{
	char temp[ MAX_PATH ];

	if ( strHorse == NULL )
	{
		for ( int i = 0; i < m_siImageFileNum; ++i )
		{
			if ( strncmp( strHelmet, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if ( strncmp( strArmor, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if ( strncmp( strWeapon, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if ( m_bIsTakeMantle && strncmp( strMantle, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
		}
	}
	else
	{
		for ( int i = 0; i < m_siImageFileNum; ++i )
		{
			if ( strncmp( strHelmet, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if ( strncmp( strArmor, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if ( strncmp( strWeapon, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if ( m_bIsTakeMantle && strncmp( strMantle, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if ( strncmp( strHorse, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1 ) == 0 )
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
			else if( m_bIsTakeHorseHead && strncmp(strHorseHead, m_strImageFileNameList[ i ], MAX_ITEMCODE_LENGTH - 1) == 0)
			{
				memset( temp, 0, MAX_PATH );
				strcpy( temp, "./JohapChar/");
				strcat( temp, m_strImageFileNameList[ i ] );
				g_XSPRMgr.LoadXSPR( temp );
			}
		}
	}

	return;
}

void CCharacterDrawing::LoadItemCodeData( char cSex, char *strHelmet, char *strArmor, char *strWeapon, char *strMantle, char *strHorse, char *strHorseHead )
{
	switch( cSex )
	{
	case 1:
		strcpy( m_strSex, "TP1" );
		strcpy( m_strTempSex, "TP1" );
		m_cSex = 1;
		break;
	case 2:
		strcpy( m_strSex, "TP2" );
		strcpy( m_strTempSex, "TP2" );
		m_cSex = 2;
		break;
	case 3:
		strcpy( m_strSex, "TP3" );
		strcpy( m_strTempSex, "TP3" );
		m_cSex = 1;
		break;
	case 4:
		strcpy( m_strSex, "TP4" );
		strcpy( m_strTempSex, "TP4" );
		m_cSex = 2;
		break;
	case 5:
		strcpy( m_strSex, "TP5" );
		strcpy( m_strTempSex, "TP1" );
		m_cSex = 1;
		break;
	case 6:
		strcpy( m_strSex, "TP6" );
		strcpy( m_strTempSex, "TP2" );
		m_cSex = 2;
		break;
	case 7:
		strcpy( m_strSex, "TP7" );
		strcpy( m_strTempSex, "TP3" );
		m_cSex = 1;
		break;
	case 8:
		strcpy( m_strSex, "TP8" );
		strcpy( m_strTempSex, "TP4" );
		m_cSex = 2;
		break;
	}
	
	strcpy( m_strHelmet, strHelmet );
	strcpy( m_strArmor, strArmor );
	strcpy( m_strWeapon, strWeapon );
	
	if ( strMantle == NULL )
	{
		m_bIsTakeMantle = false;
		memset( m_strMantle, 0, MAX_ITEMCODE_LENGTH );
	}
	else
	{
		// 망토인 경우.
		if ( strMantle[0] == 'M' || strMantle[0] == 'm' )
		{
			TCHAR *strMantle1 = _strupr( strMantle );

			m_bIsTakeMantle = true;
			strcpy( m_strMantle, strMantle1 );
			m_bIsTakeEquipment = false;
			memset( m_strEquipment, 0, MAX_ITEMCODE_LENGTH );
		}
		// 장비인 경우.
		else if ( strMantle[0] == 'E' || strMantle[0] == 'e' )
		{
			TCHAR *strEquipment1 = _strupr( strMantle );

			m_bIsTakeEquipment = true;
			strcpy( m_strEquipment, strEquipment1 );
			m_bIsTakeMantle = false;
			memset( m_strMantle, 0, MAX_ITEMCODE_LENGTH );
		}
		else
		{
			// 문제가 있는 경우이다.	// 이 경우는 우선 출력 안하게 함.
			m_bIsTakeMantle = false;
			memset( m_strMantle, 0, MAX_ITEMCODE_LENGTH );

			m_bIsTakeEquipment = false;
			memset( m_strEquipment, 0, MAX_ITEMCODE_LENGTH );
		}

	}

	if( strHorse == NULL )
	{
		m_bIsTakeHorse = false;
		memset( m_strHorse, 0, MAX_ITEMCODE_LENGTH );
	}
	else
	{
		m_bIsTakeHorse = true;
		strcpy( m_strHorse, strHorse );

		if(strHorseHead != NULL && strlen(strHorseHead) > 0)
		{
			m_bIsTakeHorseHead = true;
			strcpy( m_strHorseHead, strHorseHead );
		}
		else
		{
			m_bIsTakeHorseHead = false;
			memset( m_strHorseHead, 0, MAX_ITEMCODE_LENGTH );
		}
	}

	LoadImageDataFromList( strHelmet, strArmor, strWeapon, strMantle, strHorse, strHorseHead );

	return;
}

void CCharacterDrawing::DrawCharacter( char *strAction, char cDirection, short ssAnimationIndex, short ssBottomX, short ssBottomY )
{
	////////////////////////////////////////////////////////////////////////////////////
	// config file 을 읽어서 정보를 집어 넣는 부분...
	// config file 에서는 주로 찍는 순서( 머리, 왼손, 오른손, 몸통, 말 )을 결정 짓게 되겠지...
	////////////////////////////////////////////////////////////////////////////////////

	
	
	
	return;
}

void CCharacterDrawing::GetDrawingInfo( char *strFilePath )
{
	if ( m_cCurrentDirection > NORTH_WEST )
	{
		m_ssAnimationStartIndex = g_XSPRMgr.GetDirection( strFilePath, NORTH_WEST - ( m_cCurrentDirection - NORTH_WEST ), &m_ssAnimationNum );
		m_ssCurrentAnimationIndex = m_ssAnimationStartIndex;
		m_ssAnimationEndIndex = m_ssAnimationStartIndex + m_ssAnimationNum;
		m_bReverse = true;
	}
	else
	{
		m_ssAnimationStartIndex = g_XSPRMgr.GetDirection( strFilePath, m_cCurrentDirection, &m_ssAnimationNum );
		m_ssCurrentAnimationIndex = m_ssAnimationStartIndex;
		m_ssAnimationEndIndex = m_ssAnimationStartIndex + m_ssAnimationNum;
		m_bReverse = false;
	}

	return;
}

void CCharacterDrawing::DrawCharacter( int *pDrawSequence, char cActionIndex, char cDirection, short ssBottomX, short ssBottomY )
{
	m_cCurrentActionIndex = cActionIndex;
	m_ssAnimationNum = m_pstAction[ cActionIndex ].cTotalFrameNum / m_pstAction[ cActionIndex ].cDirection;

	if ( cDirection > NORTH_WEST )
	{
		m_ssAnimationStartIndex = m_ssAnimationNum * ( NORTH_WEST - ( cDirection - NORTH_WEST ) );		
		m_bReverse = true;
	}
	else
	{
		m_ssAnimationStartIndex = m_ssAnimationNum * cDirection;
		m_bReverse = false;
	}

	m_ssCurrentAnimationIndex = m_ssAnimationStartIndex;
	m_ssAnimationEndIndex = m_ssAnimationStartIndex + m_ssAnimationNum;
	m_cCurrentDirection = cDirection;

	g_graphics.ClearSurface( lpSurface, RGB16_TRANS );
	
	char cTempDirection = m_cCurrentDirection;

	if ( cTempDirection > NORTH_WEST )
	{
		cTempDirection = NORTH_WEST - ( cTempDirection - NORTH_WEST );
	}

	if ( clGrp.LockSurface( lpSurface ) == TRUE )
	{
		ZeroMemory( m_strFileName[ cTempDirection ], sizeof( m_strFileName[ cTempDirection ] ) );
		
		if ( m_cCurrentActionIndex == DTHU || m_cCurrentActionIndex == DTHR )
		{
			strcpy( m_strFileName[ cTempDirection ][ 0 ], "./JohapChar/" );

			switch ( m_cCurrentActionIndex )
			{
			case DTHU:
				{
					strcat( m_strFileName[ 0 ][ 0 ], "KOR01TOR" );
					strcat( m_strFileName[ 0 ][ 0 ], m_strSex );
					strcat( m_strFileName[ 0 ][ 0 ], "DTHU.Spr" );
				}
				break;
			case DTHR:
				{
					strcat( m_strFileName[ 0 ][ 0 ], m_strHorse );
					strcat( m_strFileName[ 0 ][ 0 ], "TOR" );
					strcat( m_strFileName[ 0 ][ 0 ], m_strSex );
					strcat( m_strFileName[ 0 ][ 0 ], "DTHR.Spr" );
				}
				break;
			}

			g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
		}
		else if ( m_cCurrentActionIndex > SITU )		// 낚시 등등 
		{
			switch( m_cCurrentActionIndex )
			{
			case FSHU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sFSHU.Spr", m_strTempSex );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], -55, -20, 430, 279, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], -55, -20, 430, 279, m_ssCurrentAnimationIndex );
					}
				}
				break;
			case FAMU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sFAMU.Spr", m_strTempSex  );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], 99, 100, 122, 117, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 99, 100, 122, 117, m_ssCurrentAnimationIndex );
					}
				}
				break;
			case MINU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sMINU.Spr", m_strTempSex  );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], 0, 0, 220, 200, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 0, 0, 220, 200, m_ssCurrentAnimationIndex );
					}
				}
				break;
			}
		}
		else
		{
			
			for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM; ++i )
			{
				if ( m_bReverse )
				{
					m_siDrawingSequence[ NORTH_WEST - ( cDirection - NORTH_WEST ) ] [ i ] = pDrawSequence[ i ];

					switch ( pDrawSequence[ i ] )
					{
					case 0:			//안 찍음.
						break;
					case 1:			// 투구
						strcpy( m_strFileName[ cTempDirection ][ 0 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 0 ], m_strHelmet );
						strcat( m_strFileName[ cTempDirection ][ 0 ], "HED" );
						strcat( m_strFileName[ cTempDirection ][ 0 ], m_strSex );
						
						/*
						if ( cActionIndex == 1 || cActionIndex == 11 )
						{
							strcat( m_strFileName[ cTempDirection ][ 0 ], m_pstAction[cActionIndex - 1].strActionCode );
						}
						else
						{
						*/
							strcat( m_strFileName[ cTempDirection ][ 0 ], m_pstAction[cActionIndex].strActionCode );
						//}
						
						strcat( m_strFileName[ cTempDirection ][ 0 ], ".Spr" );					
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						
						break;
					case 2:			// 갑옷
						if ( cActionIndex == SITU )
						{
							strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
							strcat( m_strFileName[ cTempDirection ][ 1 ], "TOR" );
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_strSex );
						}
						else
						{
							if( strcmp( m_strArmor, "KOR01" ) == 0 )
							{
								strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
								strcat( m_strFileName[ cTempDirection ][ 1 ], "TOR" );
								strcat( m_strFileName[ cTempDirection ][ 1 ], m_strSex );
							}
							else
							{
								strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
								strcat( m_strFileName[ cTempDirection ][ 1 ], "TORTP1" );
								//strcat( m_strFileName[ cTempDirection ][ 1 ], m_strTempSex );
							}
						}
						/*
						strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
						strcat( m_strFileName[ cTempDirection ][ 1 ], "TORTP1" );
						*/
						//strcat( m_strFileName[ cTempDirection ][ 1 ], m_strTempSex );
						
						/*
						if ( cActionIndex == 1 || cActionIndex == 11 )
						{
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_pstAction[cActionIndex - 1].strActionCode );
						}
						else
						{
						*/
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_pstAction[cActionIndex].strActionCode );
						//}
						strcat( m_strFileName[ cTempDirection ][ 1 ], ".Spr" );
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 1 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 3:			// 오른쪽 손
						strcpy( m_strFileName[ cTempDirection ][ 2 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 2 ], m_strWeapon );
						strcat( m_strFileName[ cTempDirection ][ 2 ], "RHDTP1" );
						strcat( m_strFileName[ cTempDirection ][ 2 ], m_pstAction[cActionIndex].strActionCode );
						strcat( m_strFileName[ cTempDirection ][ 2 ], ".Spr" );
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 2 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 4:			// 왼쪽 손
						strcpy( m_strFileName[ cTempDirection ][ 3 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 3 ], m_strWeapon );
						strcat( m_strFileName[ cTempDirection ][ 3 ], "LHDTP1" );
						strcat( m_strFileName[ cTempDirection ][ 3 ], m_pstAction[cActionIndex].strActionCode );
						strcat( m_strFileName[ cTempDirection ][ 3 ], ".Spr" );
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 3 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 5:			// 망토 이미지
						{
							if ( m_bIsTakeMantle )
							{
								strcpy( m_strFileName[ cTempDirection ][ 4 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 4 ], m_strMantle );
								strcat( m_strFileName[ cTempDirection ][ 4 ], "ACC" );				

								if (m_cCurrentActionIndex == SITU)
									strcat( m_strFileName[ cTempDirection ][ 4 ], m_strTempSex );
								else
									strcat( m_strFileName[ cTempDirection ][ 4 ], "TP1" );

								strcat( m_strFileName[ cTempDirection ][ 4 ], m_pstAction[cActionIndex].strActionCode );
								strcat( m_strFileName[ cTempDirection ][ 4 ], ".Spr" );
								g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
							}
						}
						break;
					case 6:			// 장비 이미지
						{
							if ( m_bIsTakeEquipment )
							{
								strcpy( m_strFileName[ cTempDirection ][ 4 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 4 ], m_strEquipment );
								strcat( m_strFileName[ cTempDirection ][ 4 ], "ACO" );

								if (m_cCurrentActionIndex == SITU)
									strcat( m_strFileName[ cTempDirection ][ 4 ], m_strTempSex );
								else
									strcat( m_strFileName[ cTempDirection ][ 4 ], "TP1" );

								strcat( m_strFileName[ cTempDirection ][ 4 ], m_pstAction[cActionIndex].strActionCode );
								strcat( m_strFileName[ cTempDirection ][ 4 ], ".Spr" );
								g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
							}
						}
						break;
					case 7:			// 말 이미지
						strcpy( m_strFileName[ cTempDirection ][ 5 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 5 ], m_strHorse );
						strcat( m_strFileName[ cTempDirection ][ 5 ], "HRSTP1" );
						strcat( m_strFileName[ cTempDirection ][ 5 ], m_pstAction[cActionIndex].strActionCode );
						strcat( m_strFileName[ cTempDirection ][ 5 ], ".Spr" );
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 5 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 8:
						{
							// 말 머리 이미지
							if(m_bIsTakeHorseHead)
							{
								strcpy( m_strFileName[ cTempDirection ][ 6 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 6 ], m_strHorseHead );
								strcat( m_strFileName[ cTempDirection ][ 6 ], "HRSTP1" );
								strcat( m_strFileName[ cTempDirection ][ 6 ], m_pstAction[cActionIndex].strActionCode );
								strcat( m_strFileName[ cTempDirection ][ 6 ], ".Spr" );
								g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 6 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
							}
						}
						break;
					default:		// 안 찍음
						break;
					}
					
				}
				else
				{
					m_siDrawingSequence[ cTempDirection ] [ i ] = pDrawSequence[ i ];

					switch ( pDrawSequence[ i ] )
					{
					case 0:			//안 찍음.
						break;
					case 1:			// 투구
						strcpy( m_strFileName[ cTempDirection ][ 0 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 0 ], m_strHelmet );
						strcat( m_strFileName[ cTempDirection ][ 0 ], "HED" );
						strcat( m_strFileName[ cTempDirection ][ 0 ], m_strSex );
						
						/*
						if ( cActionIndex == 1 || cActionIndex == 11 )
						{
							strcat( m_strFileName[ cTempDirection ][ 0 ], m_pstAction[cActionIndex - 1].strActionCode );
						}
						else
						{
						*/
							strcat( m_strFileName[ cTempDirection ][ 0 ], m_pstAction[cActionIndex].strActionCode );
						//}
						strcat( m_strFileName[ cTempDirection ][ 0 ], ".Spr" );

						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						

						break;
					case 2:			// 갑옷
						if ( cActionIndex == SITU )
						{
							strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
							strcat( m_strFileName[ cTempDirection ][ 1 ], "TOR" );
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_strSex );
						}
						else
						{
							if( strcmp( m_strArmor, "KOR01" ) == 0 )
							{
								strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
								strcat( m_strFileName[ cTempDirection ][ 1 ], "TOR" );
								strcat( m_strFileName[ cTempDirection ][ 1 ], m_strSex );
							}
							else
							{
								strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
								strcat( m_strFileName[ cTempDirection ][ 1 ], "TORTP1" );
								//strcat( m_strFileName[ cTempDirection ][ 1 ], m_strTempSex );
							}
						}
						/*
						strcpy( m_strFileName[ cTempDirection ][ 1 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 1 ], m_strArmor );
						strcat( m_strFileName[ cTempDirection ][ 1 ], "TORTP1" );
						*/
						//strcat( m_strFileName[ cTempDirection ][ 1 ], m_strTempSex );
						
						/*
						if ( cActionIndex == 1 || cActionIndex == 11 )
						{
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_pstAction[cActionIndex - 1].strActionCode );
						}
						else
						{
						*/
							strcat( m_strFileName[ cTempDirection ][ 1 ], m_pstAction[cActionIndex].strActionCode );
						//}
						strcat( m_strFileName[ cTempDirection ][ 1 ], ".Spr" );
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 1 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 3:			// 오른쪽 손
						strcpy( m_strFileName[ cTempDirection ][ 2 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 2 ], m_strWeapon );
						strcat( m_strFileName[ cTempDirection ][ 2 ], "RHDTP1" );
						strcat( m_strFileName[ cTempDirection ][ 2 ], m_pstAction[cActionIndex].strActionCode );
						strcat( m_strFileName[ cTempDirection ][ 2 ], ".Spr" );
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 2 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 4:			// 왼쪽 손
						strcpy( m_strFileName[ cTempDirection ][ 3 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 3 ], m_strWeapon );
						strcat( m_strFileName[ cTempDirection ][ 3 ], "LHDTP1" );
						strcat( m_strFileName[ cTempDirection ][ 3 ], m_pstAction[cActionIndex].strActionCode );
						strcat( m_strFileName[ cTempDirection ][ 3 ], ".Spr" );
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 3 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 5:			// 망토 이미지
						{
							if ( m_bIsTakeMantle )
							{
								strcpy( m_strFileName[ cTempDirection ][ 4 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 4 ], m_strMantle );
								strcat( m_strFileName[ cTempDirection ][ 4 ], "ACC" );				

								if (m_cCurrentActionIndex == SITU)
									strcat( m_strFileName[ cTempDirection ][ 4 ], m_strTempSex );
								else
									strcat( m_strFileName[ cTempDirection ][ 4 ], "TP1" );

								strcat( m_strFileName[ cTempDirection ][ 4 ], m_pstAction[cActionIndex].strActionCode );
								strcat( m_strFileName[ cTempDirection ][ 4 ], ".Spr" );
								g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
							}
						}
						break;
					case 6:			// 장비 이미지
						{
							if ( m_bIsTakeEquipment )
							{
								strcpy( m_strFileName[ cTempDirection ][ 4 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 4 ], m_strEquipment );
								strcat( m_strFileName[ cTempDirection ][ 4 ], "ACO" );				

								if (m_cCurrentActionIndex == SITU)
									strcat( m_strFileName[ cTempDirection ][ 4 ], m_strTempSex );
								else
									strcat( m_strFileName[ cTempDirection ][ 4 ], "TP1" );

								strcat( m_strFileName[ cTempDirection ][ 4 ], m_pstAction[cActionIndex].strActionCode );
								strcat( m_strFileName[ cTempDirection ][ 4 ], ".Spr" );
								g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
							}
						}
						break;
					case 7:			// 말 이미지
						strcpy( m_strFileName[ cTempDirection ][ 5 ], "./JohapChar/" );
						strcat( m_strFileName[ cTempDirection ][ 5 ], m_strHorse );
						strcat( m_strFileName[ cTempDirection ][ 5 ], "HRSTP1" );
						strcat( m_strFileName[ cTempDirection ][ 5 ], m_pstAction[cActionIndex].strActionCode );
						strcat( m_strFileName[ cTempDirection ][ 5 ], ".Spr" );
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 5 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 8:
						{
							// 말 머리 이미지
							if(m_bIsTakeHorseHead)
							{
								strcpy( m_strFileName[ cTempDirection ][ 6 ], "./JohapChar/" );
								strcat( m_strFileName[ cTempDirection ][ 6 ], m_strHorseHead );
								strcat( m_strFileName[ cTempDirection ][ 6 ], "HRSTP1" );
								strcat( m_strFileName[ cTempDirection ][ 6 ], m_pstAction[cActionIndex].strActionCode );
								strcat( m_strFileName[ cTempDirection ][ 6 ], ".Spr" );
								g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 6 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
							}
						}
						break;
					default:		// 안 찍으면 됨.
						break;
					}			
				}
			}
		}

		clGrp.UnlockSurface( lpSurface );
	}

	GP.UpdateScreen( g_hDlg, lpSurface, TILE_WIDTH, TILE_HEIGHT, 0, 0, 26, 351, true, TILE_WIDTH, TILE_HEIGHT );
	//clGrp.UpdateScreen( g_hDlg, lpSurface, 26, 351, 0, 0, 0 );
	return;
}

void CCharacterDrawing::DrawPreFrame()
{
	char cTempDirection = m_cCurrentDirection;

	if ( cTempDirection > NORTH_WEST )
	{
		cTempDirection = NORTH_WEST - ( cTempDirection - NORTH_WEST );
	}

	g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

	if ( clGrp.LockSurface( lpSurface ) == TRUE )
	{
		if ( m_cCurrentActionIndex == DTHU || m_cCurrentActionIndex == DTHR )
		{
			g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
		}
		else if ( m_cCurrentActionIndex > SITU )		// 낚시 등등 
		{
			switch( m_cCurrentActionIndex )
			{
			case FSHU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sFSHU.Spr", m_strTempSex );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], -55, -20, 430, 279, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], -55, -20, 430, 279, m_ssCurrentAnimationIndex );
					}
				}
				break;
			case FAMU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sFAMU.Spr", m_strTempSex  );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], 99, 100, 122, 117, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 99, 100, 122, 117, m_ssCurrentAnimationIndex );
					}
				}
				break;
			case MINU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sMINU.Spr", m_strTempSex  );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], 0, 0, 220, 200, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 0, 0, 220, 200, m_ssCurrentAnimationIndex );
					}
				}
				break;
			}
		}
		else
		{
			for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM; ++i )
			{
				if ( m_bReverse )
				{
					switch ( m_siDrawingSequence[ NORTH_WEST - ( m_cCurrentDirection - NORTH_WEST ) ] [ i ] )
					{
					case 0:			//안 찍음.
						break;
					case 1:			// 투구				
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 2:			// 갑옷
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 1 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 3:			// 오른쪽 손
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 2 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 4:			// 왼쪽 손
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 3 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 5:			// 망토 이미지
						if ( m_bIsTakeMantle )
						{
							g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						}
						break;
					case 6:			// 장비 이미지
						if ( m_bIsTakeEquipment )
						{
							g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						}
						break;
					case 7:			// 말 이미지
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 5 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 8:
						// 말 머리 이미지
						if(m_bIsTakeHorseHead)
						{
							g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 6 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						}
						break;
					default:		// 안 찍음
						break;
					}			
				}
				else
				{
					switch ( m_siDrawingSequence[ m_cCurrentDirection ] [ i ] )
					{
					case 0:			//안 찍음.
						break;
					case 1:			// 투구
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 2:			// 갑옷
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 1 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 3:			// 오른쪽 손
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 2 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 4:			// 왼쪽 손
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 3 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 5:			// 망토 이미지
						if ( m_bIsTakeMantle )
						{
							g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						}
						break;
					case 6:			// 장비 이미지
						if ( m_bIsTakeEquipment )
						{
							g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						}
						break;
					case 7:			// 말 이미지
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 5 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 8:
						// 말 머리 이미지
						if(m_bIsTakeHorseHead)
						{
							g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 6 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						}
						break;
					default:		// 안 찍으면 됨.
						break;
					}			
				}
			}
		}

		clGrp.UnlockSurface( lpSurface );
	}

	//clGrp.UpdateScreen( g_hDlg, lpSurface, 26, 351, 0, 0, 0 );
	GP.UpdateScreen( g_hDlg, lpSurface, TILE_WIDTH, TILE_HEIGHT, 0, 0, 26, 351, true, TILE_WIDTH, TILE_HEIGHT );

	
	if ( --m_ssCurrentAnimationIndex < m_ssAnimationStartIndex )
	{
		m_ssCurrentAnimationIndex = m_ssAnimationEndIndex - 1;
	}

	return;
}

void CCharacterDrawing::DrawNextFrame()
{
	char cTempDirection = m_cCurrentDirection;

	if ( cTempDirection > NORTH_WEST )
	{
		cTempDirection = NORTH_WEST - ( cTempDirection - NORTH_WEST );
	}

	
	g_graphics.ClearSurface( lpSurface, RGB16_TRANS );

	if ( clGrp.LockSurface( lpSurface ) == TRUE )
	{
		if ( m_cCurrentActionIndex == DTHU || m_cCurrentActionIndex == DTHR )
		{
			g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
		}
		else if ( m_cCurrentActionIndex > SITU )		// 낚시 등등 
		{
			switch( m_cCurrentActionIndex )
			{
			case FSHU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sFSHU.Spr", m_strTempSex );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], -55, -20, 430, 279, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], -55, -20, 430, 279, m_ssCurrentAnimationIndex );
					}
				}
				break;
			case FAMU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sFAMU.Spr", m_strTempSex  );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], 99, 100, 122, 117, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 99, 100, 122, 117, m_ssCurrentAnimationIndex );
					}
				}
				break;
			case MINU:
				{
					sprintf( m_strFileName[ 0 ][ 0 ], "./JohapChar/KOR01TOR%sMINU.Spr", m_strTempSex  );
					if ( m_bReverse )
					{
						g_XSPRMgr.PutSprRT( m_strFileName[ 0 ][ 0 ], 0, 0, 220, 200, m_ssCurrentAnimationIndex );
					}
					else
					{
						g_XSPRMgr.PutSprT( m_strFileName[ 0 ][ 0 ], 0, 0, 220, 200, m_ssCurrentAnimationIndex );
					}
				}
				break;
			}
		}
		else
		{
			for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM; ++i )
			{
				if ( m_bReverse )
				{
					switch ( m_siDrawingSequence[ NORTH_WEST - ( m_cCurrentDirection - NORTH_WEST ) ] [ i ] )
					{
					case 0:			//안 찍음.
						break;
					case 1:			// 투구				
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 2:			// 갑옷
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 1 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 3:			// 오른쪽 손
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 2 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 4:			// 왼쪽 손
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 3 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 5:			// 망토 이미지
						if ( m_bIsTakeMantle )
						{
							g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						}						
						break;
					case 6:			// 장비 이미지
						if ( m_bIsTakeEquipment )
						{
							g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						}						
						break;
					case 7:			// 말 이미지						
						g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 5 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						break;
					case 8:
						// 말 머리 이미지
						if(m_bIsTakeHorseHead)
						{
							g_XSPRMgr.PutSprRT( m_strFileName[ cTempDirection ][ 6 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex );
						}
						break;
					default:		// 안 찍음
						break;
					}			
				}
				else
				{
					switch ( m_siDrawingSequence[ m_cCurrentDirection ] [ i ] )
					{
					case 0:			//안 찍음.
						break;
					case 1:			// 투구
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 0 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 2:			// 갑옷
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 1 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 3:			// 오른쪽 손
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 2 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 4:			// 왼쪽 손
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 3 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 5:			// 망토 이미지
						if ( m_bIsTakeMantle )
						{
							g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						}
						break;
					case 6:			// 장비 이미지
						if ( m_bIsTakeEquipment )
						{
							g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 4 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						}
						break;
					case 7:			// 말 이미지
						g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 5 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						break;
					case 8:
						// 말 머리 이미지
						if(m_bIsTakeHorseHead)
						{
							g_XSPRMgr.PutSprT( m_strFileName[ cTempDirection ][ 6 ], 0, 0, CHARACTER_TILE_WIDTH, CHARACTER_TILE_HEIGHT, m_ssCurrentAnimationIndex  );
						}
						break;
					default:		// 안 찍으면 됨.
						break;
					}			
				}
			}
		}
		clGrp.UnlockSurface( lpSurface );
	}

	//clGrp.UpdateScreen( g_hDlg, lpSurface, 26, 351, 0, 0, 0 );
	GP.UpdateScreen( g_hDlg, lpSurface, TILE_WIDTH, TILE_HEIGHT, 0, 0, 26, 351, true, TILE_WIDTH, TILE_HEIGHT );

	if ( ++m_ssCurrentAnimationIndex >= m_ssAnimationEndIndex )
	{
		m_ssCurrentAnimationIndex = m_ssAnimationStartIndex;
	}	

	return;
}

void CCharacterDrawing::SaveMinFile( int siDelayTime )
{
	char strFileName[ MAX_PATH ] = "";
	
	strcpy( strFileName, "./MinData/" );
	_mkdir( strFileName );
	strcat( strFileName, m_strSex );
	strcat( strFileName, m_pstAction[ m_cCurrentActionIndex ].strActionCode );
	strcat( strFileName, ".Min" );

	FILE *fp = fopen( strFileName, "r" );
	if ( fp != NULL )
	{
		if ( IDYES == MessageBox( NULL, "파일이 존재합니다.\n덮어 쓰시겠습니까?", "파일 존재", MB_YESNO ) )
		{
			fclose( fp );
		}
		else
		{
			fclose( fp );
			return;
		}
	}

		
	fp = fopen( strFileName, "w" );
	
	if ( fp == NULL )
	{
		MessageBox( NULL, "WriteFile Error", strFileName, MB_OK );
		return;
	}

	fprintf( fp, "%s\t%c\t%d\n", "TYPE", 'K', m_cSex );
	fprintf( fp, "%s\t%s\t%d\n", "ACTION", m_pstAction[ m_cCurrentActionIndex ].strActionCode, m_cCurrentActionIndex );
	fprintf( fp, "%s\t%d\n", "DirectionFrame", m_pstAction[ m_cCurrentActionIndex ].cFrameNum );	
	fprintf( fp, "%s\t%d\n", "TOTALFRAME", m_pstAction[ m_cCurrentActionIndex ].cTotalFrameNum );
	fprintf( fp, "%s\t%d\n", "DELAYTIME", siDelayTime );
	fprintf( fp, "%s\t%d\n", "DIRECTION", m_pstAction[ m_cCurrentActionIndex ].cDirection );

	int Count;

	for ( int i = 0; i < m_pstAction[ m_cCurrentActionIndex ].cDirection; ++i )
	{
		Count = 0;
		fprintf( fp, "%d\t", i );
		for ( int j = 0; j < MAX_DRAWING_SEQUENCE_NUM; ++j )
		{
			if ( m_cCurrentActionIndex == DTHU || m_cCurrentActionIndex == DTHR )
			{
			}
			else if ( m_siDrawingSequence[ i ][ j ] == 0 && m_pstAction[ m_cCurrentActionIndex ].cFrameNum > 2 )
			{
				//++Count;

				//if ( Count > 2 )
				//{
				//	MessageBox( NULL, "다른 방향들에 대한 설정을 다시 확인하시고,\n다시 저장해 주십시오.\n찍지 않음이 3개 이상 존재 합니다.\n", "Error", MB_OK );
				//}
			}

			fprintf( fp, "%d\t", m_siDrawingSequence[ i ][ j ] );
		}
		fprintf( fp, "\n");
	}
	
	fclose( fp );

	

	return;
}

// Save 하고 나면 Min 파일이 하나 더 생기는데, 이것을 다이얼로그에 알려주는 과정 중에 파일을 다시 읽어서 리로드 시키는 뭐 그런거지...
void CCharacterDrawing::ReloadMinFileList()
{
	m_pMinDataMgr->FindMinFiles();
	return;
}

void CCharacterDrawing::LoadMinFile( char *strFileName, int **pSequence, char *pActionIndex, char *pSex, char *pDelayTime )
{
	CMinDataObj *pTemp = m_pMinDataMgr->GetSameMinDataObjPtr( strFileName );

	m_cCurrentDirection = 0;
	m_ssAnimationStartIndex = 0;
	m_ssAnimationNum = pTemp->GetFrameNum();
	m_ssAnimationEndIndex = m_ssAnimationStartIndex + m_ssAnimationNum;
	m_cCurrentActionIndex = pTemp->GetActionIndex();
	m_cSex = pTemp->GetSex();
	m_bReverse = false;

	

	//pTemp->GetDirectionNum() 과 m_pstAction[ m_cCurrentActionIndex ].cDirection 값은 일치함...
	//종복된 데이터... 필요 없지만... 편해서리...

	
	pTemp->CopySequence( (int**)m_siDrawingSequence );

	memcpy( pSequence, m_siDrawingSequence, sizeof( m_siDrawingSequence ) );
	*pActionIndex = m_cCurrentActionIndex;
	*pSex = pTemp->GetSex();
	*pDelayTime = pTemp->GetDelayTime();
	

	return;
}

char *CCharacterDrawing::FileName( int siIndex )
{
	return m_pMinDataMgr->GetFileName( siIndex );
}