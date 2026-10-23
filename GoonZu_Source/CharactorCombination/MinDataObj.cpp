#include "stdafx.h"
#include "MinDataObj.h"

CMinDataObj::CMinDataObj()
{
	memset ( m_strFileName, 0, MAX_PATH );

	m_cNation = 'K';
	m_cSex = 1;
	m_cFrameNum = 0;
	m_cTotalFrameNum = 0;
	m_cDirectionNum = 0;
	m_cDelayTime = 1;
	memset( m_strActionCode, 0, MAX_ACTIONCODE_LENGTH );

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
	/*
	memset ( m_strFileName, 0, MAX_PATH );

	m_cNation = 'K';
	m_cSex = 1;
	m_cFrameNum = 0;
	m_cTotalFrameNum = 0;
	m_cDirectionNum = 0;
	memset( m_strActionCode, 0, MAX_ACTIONCODE_LENGTH );

	memset( m_siDrawingSequence, 0, sizeof( int ) *5 *6 );
	*/

	m_pPre = NULL;
	m_pNext = NULL;

	return;
}

void CMinDataObj::Destroy()
{
	memset ( m_strFileName, 0, MAX_PATH );

	m_cSex = 1;
	m_cFrameNum = 0;
	m_cTotalFrameNum = 0;
	m_cDirectionNum = 0;
	memset( m_strActionCode, 0, MAX_ACTIONCODE_LENGTH );

	memset( m_siDrawingSequence, 0, sizeof( int ) *5 * MAX_DRAWING_SEQUENCE_NUM );

	m_pPre = NULL;
	m_pNext = NULL;

	return;
}

void CMinDataObj::LoadMinData( char *strFileName )
{
	FILE *fp = fopen( strFileName, "r" );

	char temp[256];

	if ( fp == NULL )
	{
		MessageBox( NULL, "Load Min File Error", "Read Error", MB_OK );
		return;
	}

	fscanf( fp, "%s %c %d", temp, &m_cNation, &m_cSex );
	fscanf( fp, "%s %s %d", temp, m_strActionCode, &m_cActionIndex );
	fscanf( fp, "%s %d", temp, &m_cFrameNum );
	fscanf( fp, "%s %d", temp, &m_cTotalFrameNum );
	fscanf( fp, "%s %d", temp, &m_cDelayTime );
	fscanf( fp, "%s %d", temp, &m_cDirectionNum );

	
	int temp1 = 0;
	for ( int i = 0; i < m_cDirectionNum; ++i )
	{
		fscanf( fp, "%d %d %d %d %d %d %d %d %d", &temp1, &m_siDrawingSequence[ i ][ 0 ], &m_siDrawingSequence[ i ][ 1 ], &m_siDrawingSequence[ i ][ 2 ], &m_siDrawingSequence[ i ][ 3 ], &m_siDrawingSequence[ i ][ 4 ], &m_siDrawingSequence[ i ][ 5 ], &m_siDrawingSequence[ i ][ 6 ], &m_siDrawingSequence[ i ][ 7 ] );
		m_siDrawingSequence[ i ][ MAX_DRAWING_SEQUENCE_NUM - 1 ] = 0;
	}

	
	strncpy ( m_strFileName, &strFileName[ strlen(strFileName) - 11 ], 7 );
	
	strcat ( m_strFileName, "" );
	

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

char* CMinDataObj::GetFileName()
{
	return m_strFileName;
}

void CMinDataObj::SetFileName( char *strFileName )
{
	strcpy( m_strFileName, strFileName );

	return;
}

char CMinDataObj::GetSex()
{
	return m_cSex;
}

void CMinDataObj::SetSex( char cSex )
{
	m_cSex = cSex;
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

char *CMinDataObj::GetActionCode()
{
	return m_strActionCode;
}

void CMinDataObj::SetActionCode( char *pActionCode )
{
	strcpy( m_strActionCode, pActionCode );
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

char CMinDataObj::GetDelayTime()
{
	return m_cDelayTime;
}

int *CMinDataObj::GetDrawingSequence( char cDirection )
{
	return m_siDrawingSequence[ cDirection ];
}

void CMinDataObj::CopySequence( int **pDrawingSequence )
{
	/*
	short *pTemp;

	for ( int i = 0; i < m_cDirectionNum; ++i )
	{
		pTemp = (((short *)pDrawingSequence) + 5 * i );
		for ( int j = 0; j < 5; ++j )
		{
			*pTemp = m_siDrawingSequence[ i ][ j ];
			//*(pTemp + j) = m_siDrawingSequence[ i ][ j ];
		}
	}
	*/
	memcpy( pDrawingSequence, m_siDrawingSequence, sizeof( int ) * 5 * MAX_DRAWING_SEQUENCE_NUM );
}

void CMinDataObj::SetDrawingSequence( int *pDrawingSequence, char cDirection )
{
	for ( int i = 0; i < MAX_DRAWING_SEQUENCE_NUM; ++i )
	{
		m_siDrawingSequence[ cDirection ][ i ] = pDrawingSequence[ i ];
	}

	return;
}

bool CMinDataObj::IsSameMinFile( char *strFileName )
{
	if ( m_strFileName == NULL )
	{
		return false;
	}

	if ( strcmp( strFileName, m_strFileName ) == 0 )
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

	short Count = 0;
	int t = 0;
	for ( t = 0; t < MAX_DRAWING_SEQUENCE_NUM - Count; ++t )
	{
		// 머리라면.. 망토를 먼저 찍고, 머리를 찍는다.
		if ( pDrawingSequence[ t ] == 1 )
		{
			CurrentDrawSequence [ t ] = 5;
			CurrentDrawSequence [ t + 1 ] = 1;
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