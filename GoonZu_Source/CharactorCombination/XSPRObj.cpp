#include "stdafx.h"
#include "XSPRObj.h"

CXSPRObj::CXSPRObj()
{
	m_pPre = NULL;
	m_pNext = NULL;

	m_ssEachDirectionAnimationNum = 0;

	ZeroMemory( m_strFileName, _MAX_PATH );
	ZeroMemory( m_strChangedFileName, _MAX_PATH );
	
	Initialize();
}

CXSPRObj::~CXSPRObj()
{
	Destroy();
}

void CXSPRObj::Initialize()
{
	ZeroMemory( m_strFileName, _MAX_PATH );
	ZeroMemory( m_strChangedFileName, _MAX_PATH );

	return;
}

void CXSPRObj::Destroy()
{
	ZeroMemory( m_strFileName, _MAX_PATH );
	ZeroMemory( m_strChangedFileName, _MAX_PATH );

	clGrp.FreeSpr( m_SprObj );

	return;
}

CXSPRObj *CXSPRObj::GetPre()
{
	return m_pPre;
}

void CXSPRObj::SetPre( CXSPRObj *pNode )
{
	return;
}

CXSPRObj *CXSPRObj::GetNext()
{
	return m_pNext;
}

void CXSPRObj::SetNext( CXSPRObj *pNode )
{
	m_pNext = pNode;
}

bool CXSPRObj::LoadXSPR( char *strFileName )
{
	FILE *fp = fopen ( strFileName, "r" );

	if ( fp == NULL )
	{
		int siFileNameLength = strlen( strFileName );

		strcpy( m_strChangedFileName, strFileName );
		strncpy( &m_strChangedFileName[ siFileNameLength - 19 ], "KOR01", 5 );

		if ( ! clGrp.LoadSpr( m_strChangedFileName, &m_SprObj ) )
		{
			MessageBox( NULL, m_strChangedFileName, NULL, MB_OK );
			//clGrp.Error( m_strChangedFileName );
			return false;
		}
	}
	else
	{
		fclose( fp );

		if ( ! clGrp.LoadSpr( strFileName, &m_SprObj ) )
		{	
			MessageBox( NULL, strFileName, NULL, MB_OK );
			//clGrp.Error( strFileName );
			return false;
		}
	}
	
	strcpy( m_strFileName, strFileName );

	m_ssEachDirectionAnimationNum = m_SprObj.clHeader.siHRes / m_SprObj.clHeader.siXsize;

	return true;
}

bool CXSPRObj::IsSameFile( char *strFileName )
{
	if  ( m_strFileName == NULL )
	{
		return false;
	}
	
	if ( strcmp( strFileName, m_strFileName ) == 0 )
	{
		return true;
	}

	return false;
}

void CXSPRObj::PutSprT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	//clGrp.PutImageT( siPosX, siPosY, siWidth, siHeight, &m_SprObj.Image[ m_SprObj.Header.Start[ siStartPos ] ] );	
	//clGrp.PutSprT( siPosX, siPosY, siWidth, siHeight, &m_SprObj.Image[ m_SprObj.clHeader.clImgInfo[siStartPos].siStartPos ] );
	m_SprObj.PutSprT( siPosX, siPosY, siStartPos );
	return;
}

void CXSPRObj::PutSprRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	//clGrp.PutImageT( siPosX, siPosY, siWidth, siHeight, &m_SprObj.Image[ m_SprObj.Header.Start[ siStartPos ] ] );
	//clGrp.PutSprRT( siPosX, siPosY, siWidth, siHeight, &m_SprObj.Image[ m_SprObj.clHeader.clImgInfo[siStartPos].siStartPos ] );
	m_SprObj.PutSprT( siPosX, siPosY, siStartPos, true );
	return;
}

// 주의 사항 : cDirection 으로 입력 가능한 값은 1(남), 2(남서), 3(서), 4(북서), 5(북) 만이다.
short CXSPRObj::GetDirectionStartIndexAndDirectionAnimationNum( char cDirection, short *pssAnimationNum )
{
	*pssAnimationNum = m_ssEachDirectionAnimationNum;

	return ( cDirection - 1 ) * m_ssEachDirectionAnimationNum;
}

short CXSPRObj::GetTotalTileNum()
{
	return m_SprObj.clHeader.siHRes/m_SprObj.clHeader.siXsize  *  m_SprObj.clHeader.siVRes/m_SprObj.clHeader.siYsize;
}