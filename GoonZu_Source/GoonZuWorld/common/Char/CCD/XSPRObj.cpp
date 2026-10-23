#include "XSPRObj.h"
#include "EtcTool\EtcTool.h"
#include <Graphic.h>
#include <Graphic_Capsule.h>

#include "..\..\Client\Client.h"
extern cltCommonLogic* pclClient;

#include "../../../../Patch/unzip.h"

/*
CXSPRObj::CXSPRObj() : CObj()
{
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
	
	m_uiImageFileSize = 0;
	
	m_cSex = -1;
	m_cPart = -1;	
	m_ssActionIndex = -1;

	m_ssEachDirectionAnimationNum = 0;

	m_uiLastDrawFrame = 0;

	if ( m_SprObj.Image != NULL )
	{
		GP.FreeSpr( m_SprObj );
	}

	return;
}

void CXSPRObj::Destroy()
{
	ZeroMemory( m_strFileName, _MAX_PATH );
	ZeroMemory( m_strChangedFileName, _MAX_PATH );
	m_uiImageFileSize = 0;

	return;
}

bool CXSPRObj::LoadXSPR( TCHAR *strFileName )
{
	Initialize();

	WIN32_FIND_DATA FileFind;
	
	HANDLE hSearch = ::FindFirstFile( strFileName, &FileFind );

	if (  hSearch == INVALID_HANDLE_VALUE )
	{
		int siFileNameLength = _tcslen( strFileName );

		_tcscpy( m_strChangedFileName, strFileName );
		strncpy( &m_strChangedFileName[ 14 ], TEXT("KOR01"), 5 );

		if ( ! GP.LoadSpr( m_strChangedFileName, &m_SprObj ) )
		{
			MsgBox( m_strChangedFileName, TEXT("Error") );
			return false;
		}
	}
	else
	{
		::FindClose( hSearch );

		if ( ! GP.LoadSpr( strFileName, &m_SprObj ) )
		{
			MsgBox( strFileName, TEXT("Error") );
			return false;
		}
	}

	
	//FILE *fp = _tfopen ( strFileName, TEXT("r") );

	//if ( fp == NULL )
	//{
	//	int siFileNameLength = _tcslen( strFileName );

	//	_tcscpy( m_strChangedFileName, strFileName );
	//	strncpy( &m_strChangedFileName[ 14 ], TEXT("KOR01"), 5 );

	//	if ( ! GP.LoadSpr( m_strChangedFileName, &m_SprObj ) )
	//	{
	//		MsgBox( m_strChangedFileName, TEXT("Error") );
	//		return false;
	//	}
	//}
	//else
	//{
	//	fclose( fp );

	//	if ( ! GP.LoadSpr( strFileName, &m_SprObj ) )
	//	{
	//		MsgBox( strFileName, TEXT("Error") );
	//		return false;
	//	}
	//}
	

	//WIN32_FIND_DATA FileFind;
	//HANDLE hSearch = ::FindFirstFile( strFileName, &FileFind );

	//if (  hSearch != INVALID_HANDLE_VALUE )
	//{
	//	m_uiImageFileSize = FileFind.nFileSizeLow;
	//}

	//::FindClose( hSearch );

	_tcscpy( m_strFileName, strFileName );

	if ( m_SprObj.clHeader.siXsize == 0 )
	{
		MsgBox( strFileName, TEXT("Error m_SprObj.clHeader.siXsize = 0, 파일 손상") );
		return false;
	}

	m_ssEachDirectionAnimationNum = m_SprObj.clHeader.siHRes / m_SprObj.clHeader.siXsize;

	return true;
}

bool CXSPRObj::LoadXSPR( TCHAR *strFileName, char cSex, char cPart, short ssActionIndex )
{
	Initialize();


	WIN32_FIND_DATA FileFind;
	
	HANDLE hSearch = ::FindFirstFile( strFileName, &FileFind );

	if (  hSearch == INVALID_HANDLE_VALUE )
	{
		int siFileNameLength = _tcslen( strFileName );

		_tcscpy( m_strChangedFileName, strFileName );
		strncpy( &m_strChangedFileName[ 14 ], TEXT("KOR01"), 5 );

		if ( ! GP.LoadSpr( m_strChangedFileName, &m_SprObj ) )
		{
			MsgBox( m_strChangedFileName, TEXT("Error") );
			return false;
		}
	}
	else
	{
		::FindClose( hSearch );

		if ( ! GP.LoadSpr( strFileName, &m_SprObj ) )
		{
			MsgBox( strFileName, TEXT("Error") );
			return false;
		}
	}

	SetInfo ( cSex, cPart, ssActionIndex );
	
	
	//FILE *fp = _tfopen ( strFileName, TEXT("r") );

	//if ( fp == NULL )
	//{
	//	int siFileNameLength = _tcslen( strFileName );

	//	_tcscpy( m_strChangedFileName, strFileName );
	//	strncpy( &m_strChangedFileName[ 14 ], TEXT("KOR01"), 5 );

	//	if ( ! GP.LoadSpr( m_strChangedFileName, &m_SprObj ) )
	//	{
	//		MsgBox( m_strChangedFileName, TEXT("Error") );
	//		return false;
	//	}
	//}
	//else
	//{
	//	fclose( fp );

	//	if ( ! GP.LoadSpr( strFileName, &m_SprObj ) )
	//	{
	//		MsgBox( strFileName, TEXT("Error") );
	//		return false;
	//	}
	//}
	
	//WIN32_FIND_DATA FileFind;
	//HANDLE hSearch = ::FindFirstFile( strFileName, &FileFind );

	//if (  hSearch != INVALID_HANDLE_VALUE )
	//{
	//	m_uiImageFileSize = FileFind.nFileSizeLow;
	//}

	//::FindClose( hSearch );

	_tcscpy( m_strFileName, strFileName );

	if ( m_SprObj.clHeader.siXsize == 0 )
	{
		MsgBox( strFileName, TEXT("Error m_SprObj.clHeader.siXsize = 0, 파일 손상") );
		return false;
	}

	m_ssEachDirectionAnimationNum = m_SprObj.clHeader.siHRes / m_SprObj.clHeader.siXsize;

	return true;
}

bool CXSPRObj::IsSameFile( TCHAR *strFileName )
{
	if  ( m_strFileName == NULL )
	{
		return false;
	}

	int i = 14;

	for ( ; i < 19; ++i )
	{
		if( strFileName[i] != m_strFileName[i] )
		{
			return false;
		}
	}

	return true;
}

bool CXSPRObj::IsSameFile( TCHAR *strFileName, char cSex, char cPart, short ssActionIndex )
{
	if  ( m_strFileName == NULL )
	{
		return false;
	}

	
	if ( m_ssActionIndex != ssActionIndex )
	{
		return false;
	}
	
	
	if ( m_cPart != cPart )
	{
		return false;
	}
	

	if ( m_cSex != cSex )
	{
		return false;
	}
	

	//int i = 16;

	//for ( ; i < 31; ++i )
	//{
	//	if( strFileName[i] != m_strFileName[i] )
	//	{
	//		return false;
	//	}
	//}
	
	int i = 14;

	for ( ; i < 25; ++i )
	{
		if( strFileName[i] != m_strFileName[i] )
		{
			return false;
		}
	}
	

	return true;
}

cltTSpr* CXSPRObj::PutSprT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	m_uiLastDrawFrame = pclClient->GetFrame();

	m_SprObj.PutSprT( siPosX, siPosY, siStartPos );
	return &m_SprObj;
}

cltTSpr* CXSPRObj::PutSprRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	m_uiLastDrawFrame = pclClient->GetFrame();

	m_SprObj.PutSprT( siPosX, siPosY, siStartPos, true );
	return &m_SprObj;
}

cltTSpr* CXSPRObj::PutSprScreenT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	m_uiLastDrawFrame = pclClient->GetFrame();

	m_SprObj.PutSprScreenT( siPosX, siPosY, siStartPos );
	return &m_SprObj;
}

cltTSpr* CXSPRObj::PutSprScreenRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	m_uiLastDrawFrame = pclClient->GetFrame();

	m_SprObj.PutSprScreenT( siPosX, siPosY, siStartPos, true );
	return &m_SprObj;
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

unsigned int CXSPRObj::GetImageFileSize()
{
	return m_uiImageFileSize;
}

void CXSPRObj::SetInfo( char cSex, char cPart, short ssActionIndex )
{
	m_cSex = cSex;
	m_cPart = cPart;
	m_ssActionIndex = ssActionIndex;
	return;
}

TCHAR* CXSPRObj::GetSimilarFileName( char cSex, char cPart, short ssActionIndex )
{
	if ( m_cSex == cSex && m_cPart == cPart && m_ssActionIndex == ssActionIndex )
	{
		return m_strFileName;
	}

	return NULL;
}

bool CXSPRObj::Action( UI32 uiCurrentFrame )
{
	// 300 Frame = 15초 이상 사용하지 않은 경우에는 메모리에서 지워줌
	if ( uiCurrentFrame - m_uiLastDrawFrame > 300 )
	{
		return true;
	}
	
	return false;
}
*/

CXSPRObj::CXSPRObj() : CObj()
{
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

	m_uiImageFileSize = 0;

	m_ssEachDirectionAnimationNum = 0;

	m_uiLastDrawFrame = 0;

	if ( m_SprObj.pImage != NULL )
	{
		GP.FreeSpr( m_SprObj );
	}

	return;
}

void CXSPRObj::Destroy()
{
	ZeroMemory( m_strFileName, _MAX_PATH );
	ZeroMemory( m_strChangedFileName, _MAX_PATH );
	m_uiImageFileSize = 0;

	if ( m_SprObj.pImage != NULL )
	{
		GP.FreeSpr( m_SprObj );
	}


	return;
}

bool CXSPRObj::LoadXSPR( TCHAR *strFileName )
{
	Initialize();

	BOOL bCompress = FALSE;
	BYTE* pFileBuf = NULL;
	BOOL bCompressLoaded = FALSE;

	//===========================================================================================
	// 파일이 압축파일에 들어있는치 체크한다.
	//===========================================================================================
	TCHAR szZipName[MAX_PATH] = { '\0', };
	TCHAR szZipNameRaw[MAX_PATH] = { '\0', };
	strncpy(szZipNameRaw, &strFileName[14], 5);
	StringCchPrintf( szZipName, sizeof(szZipName), "CCD\\ImageData\\%5s.sz", szZipNameRaw );
	TCHAR szRealName[MAX_PATH] = { '\0', };
	StringCchPrintf( szRealName, sizeof(szRealName), "%s", &strFileName[ 14 ] );

	unzFile uf = unzOpen( szZipName );
	bool bFind = false;
	if( uf )
	{
		unz_global_info gi;
		int ret = unzGetGlobalInfo(uf, &gi);

		for( UI32 j=0; j<gi.number_entry; j++ )
		{
			TCHAR filename[MAX_PATH];
			unz_file_info file_info;
			ret = unzGetCurrentFileInfo( uf, &file_info, filename, sizeof(filename), NULL, 0, NULL, 0);

			_tcsupr(filename);
			_tcsupr(szRealName);

			if( _tcscmp( filename, szRealName ) == 0 )
			{
				bCompress = TRUE;
				pFileBuf = new BYTE[file_info.uncompressed_size];
				if( pFileBuf != NULL )
				{
					unzOpenCurrentFile( uf );
					UI32 nReaded = unzReadCurrentFile( uf, pFileBuf, file_info.uncompressed_size);
					if( nReaded == file_info.uncompressed_size)		bCompressLoaded = true;
					unzCloseCurrentFile( uf );
				}
				break;
			}

			ret = unzGoToNextFile(uf);
		}
		unzClose( uf );
	}
	//===========================================================================================

	BOOL bLoaded = FALSE;

	if( bCompress )
	{
		if( bCompressLoaded )
		{
			bLoaded = GP.LoadSprFromMemory( pFileBuf, &m_SprObj );
		}
		else
		{
#ifdef _DEBUG
			MessageBox( NULL, strFileName, "ERROR", MB_OK );
#endif
		}
		if( pFileBuf != NULL )		delete [] pFileBuf;
	}
	else
	{
		WIN32_FIND_DATA FileFind;

		HANDLE hSearch = ::FindFirstFile( strFileName, &FileFind );
		if (  hSearch == INVALID_HANDLE_VALUE )
		{
			StringCchCopy( m_strChangedFileName, _MAX_PATH, strFileName );
			_tcsncpy( &m_strChangedFileName[ 14 ], TEXT("KOR01"), 5 );
		}
		else
		{
			::FindClose( hSearch );
			StringCchCopy( m_strChangedFileName, _MAX_PATH, strFileName );
		}

		bLoaded = GP.LoadSprFromFile( m_strChangedFileName, &m_SprObj );
	}

	if( !bLoaded )
	{
#ifdef _DEBUG
		MsgBox( strFileName, TEXT("Error") );
#endif
		return false;
	}

	StringCchCopy( m_strFileName, _MAX_PATH, strFileName );

	if ( m_SprObj.GetXSize() == 0 )
	{
		MsgBox( strFileName, TEXT("Error m_SprObj.GetXSize() = 0, 파일 손상") );
		return false;
	}

	m_ssEachDirectionAnimationNum = m_SprObj.GetHRes() / m_SprObj.GetXSize();

	return true;
}

TSpr* CXSPRObj::PutSprT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	m_uiLastDrawFrame = pclClient->GetFrame();

	GP.PutSpr(&m_SprObj, siPosX, siPosY, siStartPos );
	return &m_SprObj;
}

TSpr* CXSPRObj::PutSprRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	m_uiLastDrawFrame = pclClient->GetFrame();

	GP.PutSpr(&m_SprObj, siPosX, siPosY, siStartPos, true );
	return &m_SprObj;
}

TSpr* CXSPRObj::PutSprScreenT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{
	m_uiLastDrawFrame = pclClient->GetFrame();

	GP.PutSprScreen(&m_SprObj, siPosX, siPosY, siStartPos );
	return &m_SprObj;
}

TSpr* CXSPRObj::PutSprScreenRT( int siPosX, int siPosY, int siWidth, int siHeight, int siStartPos )
{	
	m_uiLastDrawFrame = pclClient->GetFrame();

	GP.PutSprScreen(&m_SprObj, siPosX, siPosY, siStartPos, true );
	return &m_SprObj;
}


// 주의 사항 : cDirection 으로 입력 가능한 값은 1(남), 2(남서), 3(서), 4(북서), 5(북) 만이다.
short CXSPRObj::GetDirectionStartIndexAndDirectionAnimationNum( char cDirection, short *pssAnimationNum )
{
	*pssAnimationNum = m_ssEachDirectionAnimationNum;

	return ( cDirection - 1 ) * m_ssEachDirectionAnimationNum;
}

short CXSPRObj::GetTotalTileNum()
{
	return m_SprObj.GetHRes()/m_SprObj.GetXSize()  *  m_SprObj.GetVRes()/m_SprObj.GetYSize();
}

unsigned int CXSPRObj::GetImageFileSize()
{
	return m_uiImageFileSize;
}

bool CXSPRObj::Action( UI32 uiCurrentFrame )
{
	// 1200 Frame = 60초 이상 사용하지 않은 경우에는 메모리에서 지워줌
	if ( uiCurrentFrame - m_uiLastDrawFrame > 1200 )
	{
		return true;
	}

	return false;
}