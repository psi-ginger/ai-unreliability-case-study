#include "MinDataObj.h"

#include "..\Common\CommonHeader.h"
#include "..\..\Client\Client.h"

extern cltCommonLogic* pclClient;

CMinDataObj::CMinDataObj()
{
	memset ( m_strFileName, 0, _MAX_PATH );

	m_cNation = 'K';
	m_siCharacterKind = 1;
	m_cFrameNum = 0;
	m_cTotalFrameNum = 0;
	m_cDirectionNum = 0;
	memset( m_strActionCode, 0, MAX_ACTIONCODE_LENGTH );

	//memset( m_siDrawingSequence, 0, sizeof( m_siDrawingSequence ) );
	memset( m_siDrawingSequence, 0, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );
	

	m_pPre = NULL;
	m_pNext = NULL;

	Initialize();	
}

CMinDataObj::~CMinDataObj()
{
	Destroy();
}

void CMinDataObj::Initialize()
{
	memset ( m_strFileName, 0, _MAX_PATH );

	m_cNation = 'K';
	m_siCharacterKind = 1;
	m_cFrameNum = 0;
	m_cTotalFrameNum = 0;
	m_cDirectionNum = 0;
	memset( m_strActionCode, 0, MAX_ACTIONCODE_LENGTH );

	//memset( m_siDrawingSequence, 0, sizeof( m_siDrawingSequence ) );
	memset( m_siDrawingSequence, 0, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );

	m_pPre = NULL;
	m_pNext = NULL;

	return;
}

void CMinDataObj::Destroy()
{
	memset ( m_strFileName, 0, _MAX_PATH );

	m_siCharacterKind = 1;
	m_cFrameNum = 0;
	m_cTotalFrameNum = 0;
	m_cDirectionNum = 0;
	memset( m_strActionCode, 0, MAX_ACTIONCODE_LENGTH );

	//memset( m_siDrawingSequence, 0, sizeof( m_siDrawingSequence ) );
	memset( m_siDrawingSequence, 0, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );

	m_pPre = NULL;
	m_pNext = NULL;

	return;
}

void CMinDataObj::LoadMinData( TCHAR *strFileName )
{
	FILE *fp = _tfopen( strFileName, TEXT("rt") );

	TCHAR temp[256];

	if ( fp == NULL )
	{
		if (pclClient->GameMode != GAMEMODE_SERVER)
			MessageBox( NULL, TEXT("Load Min File Error"), TEXT("strFileName"), MB_OK|MB_TOPMOST );
		return;
	}

	_ftscanf( fp, TEXT("%s %c %d"), temp, &m_cNation, &m_siCharacterKind );
	_ftscanf( fp, TEXT("%s %s %d"), temp, m_strActionCode, &m_cActionIndex );
	_ftscanf( fp, TEXT("%s %d"), temp, &m_cFrameNum );
	_ftscanf( fp, TEXT("%s %d"), temp, &m_cTotalFrameNum );
	_ftscanf( fp, TEXT("%s %d"), temp, &m_cDelayTimeIndex );
	_ftscanf( fp, TEXT("%s %d"), temp, &m_cDirectionNum );

	int temp1 = 0;
	for ( int i = 0; i < m_cDirectionNum; ++i )
	{	
		_ftscanf( fp, TEXT("%d %d %d %d %d %d %d %d %d"), &temp1, &m_siDrawingSequence[ i ][ 0 ], &m_siDrawingSequence[ i ][ 1 ], &m_siDrawingSequence[ i ][ 2 ], 
			&m_siDrawingSequence[ i ][ 3 ], &m_siDrawingSequence[ i ][ 4 ], &m_siDrawingSequence[ i ][ 5 ], 
			&m_siDrawingSequence[ i ][ 6 ], &m_siDrawingSequence[ i ][ 7 ]);

		//m_siDrawingSequence[ i ][ MAX_DRAWING_SEQUENCE_NUM - 1 ] = 0;
		//SequenceStadingInLine( m_siDrawingSequence[ i ] );
		m_siDrawingSequence[ i ][ MAX_DRAWING_SEQUENCE_NUM - 1 ] = EFF;
	}

	_tcsncpy( m_strFileName, &strFileName[ _tcslen(strFileName) - 11 ], 7 );

	StringCchCat( m_strFileName, _MAX_PATH, TEXT("") );

	TCHAR *strTempFileName = _tcsupr( m_strFileName );

	StringCchCopy( m_strFileName, _MAX_PATH, strTempFileName );

	fclose( fp );

	return;
}

CMinDataObj	*CMinDataObj::GetPre()
{
	return m_pPre;
}

void CMinDataObj::SetPre( CMinDataObj *pPre )
{
	m_pPre = pPre;
	return;
}

CMinDataObj	*CMinDataObj::GetNext()
{
	return m_pNext;
}

void CMinDataObj::SetNext( CMinDataObj *pNext )
{
	m_pNext = pNext;
	return;
}

TCHAR* CMinDataObj::GetFileName()
{
	return m_strFileName;
}

void CMinDataObj::SetFileName( TCHAR *strFileName )
{
	StringCchCopy( m_strFileName, _MAX_PATH, strFileName );

	return;
}

char CMinDataObj::GetCharacterKind()
{
	return m_siCharacterKind;
}

void CMinDataObj::SetCharacterKind( int CharacterKind )
{
	m_siCharacterKind = CharacterKind;
	return;
}

char CMinDataObj::GetFrameNum()
{
	return m_cFrameNum;
}

void CMinDataObj::SetFrameNum( char cFrameNum )
{
	m_cFrameNum = cFrameNum;
	return;
}

char CMinDataObj::GetTotalFrameNum()
{
	return m_cTotalFrameNum;
}

void CMinDataObj::SetTotalFrameNum( char cTotalFrameNum )
{
	m_cTotalFrameNum = cTotalFrameNum;
	return;
}

char CMinDataObj::GetDirectionNum()
{
	return m_cDirectionNum;
}

void CMinDataObj::SetDierctionNum( char cDirectionNum )
{
	m_cDirectionNum = cDirectionNum;
	return;
}

char CMinDataObj::GetActionIndex()
{
	return m_cActionIndex;
}

void CMinDataObj::SetActionIndex( char cActionIndex )
{
	m_cActionIndex = cActionIndex;
	return;
}

char CMinDataObj::GetDelayTimeIndex()
{
	return m_cDelayTimeIndex;
}

int *CMinDataObj::GetDrawingSequence( char cDirection )
{
	return m_siDrawingSequence[ cDirection ];
}

void CMinDataObj::CopySequence( int **pDrawingSequence )
{
	/*
	int i, j;
	for ( i = 0; i < m_cDirectionNum; ++i )
	{
		for ( j = 0; j < 6; ++j )
		{
			pDrawingSequence[ i ][ j ] = m_siDrawingSequence[ i ][ j ];
		}
	}

	for ( i = 0; i < m_cDirectionNum; ++i )
	{
		pDrawingSequence[ i ][ 6 ] = 0;
	}
	*/


	/*
	int *pTemp;

	for ( int i = 0; i < m_cDirectionNum; ++i )
	{
		pTemp = (((int *)pDrawingSequence) + 5 * i );
		for ( int j = 0; j < 6; ++j )
		{
			*pTemp = m_siDrawingSequence[ i ][ j ];
			//*(pTemp + j) = m_siDrawingSequence[ i ][ j ];
		}
	}
	*/
	
	//memcpy( pDrawingSequence, m_siDrawingSequence, sizeof( m_siDrawingSequence ) );
	memcpy( pDrawingSequence, m_siDrawingSequence, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );						//ok
}

void CMinDataObj::SetDrawingSequence( int *pDrawingSequence, char cDirection )
{
	for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM; ++i )
	{
		m_siDrawingSequence[ cDirection ][ i ] = pDrawingSequence[ i ];
	}

	return;
}

bool CMinDataObj::IsSameMinFile( TCHAR *strFileName )
{
	if ( m_strFileName == NULL )
	{
		return false;
	}

	if ( _tcscmp( strFileName, m_strFileName ) == 0 )
	{
		return true;
	}

	return false;
}

// 망토는 출력 순서에 없는데... 머리 출력전에 망토를 출력함.
// 순서에 망토를 집어 넣어서 정렬 시키는 단계임.
void CMinDataObj::SequenceStadingInLine( int *pDrawingSequence )
{
	int CurrentDrawSequence[ MAX_DRAWING_SEQUENCE_NUM ];
	memset( CurrentDrawSequence, 0, sizeof(int) * MAX_DRAWING_SEQUENCE_NUM );

	SI16 Count = 0;
	int t = 0;
	for ( t = 0; t < MAX_DRAWING_SEQUENCE_NUM - Count; ++t )
	{
		// 머리라면.. 망토를 먼저 찍고, 머리를 찍는다.
		if ( pDrawingSequence[ t ] == HED )
		{
			CurrentDrawSequence [ t ] = MTL;
			CurrentDrawSequence [ t + 1 ] = HED;
			++Count;
		}
		else
		{
			CurrentDrawSequence [ t + Count ] = pDrawingSequence[ t ];
		}
	}

	memcpy( pDrawingSequence, CurrentDrawSequence, sizeof( int ) * MAX_DRAWING_SEQUENCE_NUM );

	
	return;
}