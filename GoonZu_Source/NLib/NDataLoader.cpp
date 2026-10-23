//-----------------------------------------------------------------------------
// 2005/09/22 이제완
//-----------------------------------------------------------------------------

#include "NDataLoader.h"
#include "NFile.h"
#include "NUtil.h"
#include "../GoonZuWorld/Server/TraceBoot.h"
#include <wchar.h>

int NDATATYPE_SIZE[] = 
{
	0,					// NDATA_NONE
	sizeof( int ),		// NDATA_INT32
	sizeof( INT64 ),	// NDATA_INT64
	sizeof( float ),	// NDATA_FLOAT
	sizeof( double ),	// NDATA_DOUBLE
	sizeof( char ),		// NDATA_MBSTRING
	sizeof( WCHAR )		// NDATA_WCSTRING
};


NDataLoader::NDataLoader()
{
	ZeroMemory( this, sizeof( NDataLoader ) );

	m_pFuncPtr[ NDATA_INT32 ].Func = &NDataLoader::ConvertToInt32;
	m_pFuncPtr[ NDATA_INT64 ].Func = &NDataLoader::ConvertToInt64;
	m_pFuncPtr[ NDATA_FLOAT ].Func = &NDataLoader::ConvertToFloat;
	m_pFuncPtr[ NDATA_DOUBLE ].Func = &NDataLoader::ConvertToDouble;
	m_pFuncPtr[ NDATA_MBSTRING ].Func = &NDataLoader::ConvertToMBString;
	m_pFuncPtr[ NDATA_WCSTRING ].Func = &NDataLoader::ConvertToWCString;
}

NDataLoader::~NDataLoader()
{
	NDelete_Array( m_pBuffer );
}

bool NDataLoader::LoadDataFromFile( TCHAR *filename )
{
	
	HANDLE hFile = NULL;
	hFile = CreateFile(	filename,
		GENERIC_READ,
		0,
		0,
		OPEN_EXISTING,
		0,
		0 );

	if( hFile == INVALID_HANDLE_VALUE ) 
	{
		return false;
	}

	_tcsncpy(m_szFileName, filename, sizeof(m_szFileName));

	// 파일 크기를 얻어온다
	DWORD dwFileSize;
	DWORD dwFileSizeHigh;
	dwFileSize = GetFileSize( hFile, &dwFileSizeHigh );


	// 메모리를 할당 받는다
	if( m_pBuffer ) delete [] m_pBuffer;

	m_pBuffer = new WCHAR[ dwFileSize ];

	// 데이터를 읽어온다
	DWORD dwRead;
	ReadFile( hFile, m_pBuffer, dwFileSize, &dwRead, NULL );

	if( dwRead != dwFileSize ) {
		
		NDelete_Array( m_pBuffer );

		CloseHandle( hFile );

		return false;
	}

	CloseHandle( hFile );

	m_pStartPos = m_pBuffer;
	m_pCurPos = m_pBuffer;
	m_pEndPos = m_pBuffer + ( dwFileSize >> 1 );

	m_bDataSection = FALSE;

	return true;
}
bool NDataLoader::LoadDataFromCryptFile( TCHAR *filename )
{

	NFile file;

	if ( file.LoadFile( filename ) == false )
	{
		return false;
	}
	
	_tcsncpy(m_szFileName, filename, sizeof(m_szFileName));

	NCryptHeader *pHeader = (NCryptHeader *)file.GetBuffer();
	if( pHeader->id[ 0 ] == 'N' &&
		pHeader->id[ 1 ] == 'C' &&
		pHeader->id[ 2 ] == 'R' ) 
	{
		BYTE *pData = (BYTE*)pHeader + sizeof( NCryptHeader );
		int size = pHeader->dwDataSize;
		BYTE key = pHeader->key;
		BYTE *pBuf = new BYTE[ file.m_dwFileSize ];

		NByteDecrypt( pData, pBuf, size, key );
		LoadDataFromMem( pBuf, size );

		NDelete_Array( pBuf );

		return true;

	}
	else
	{
#ifdef _DEBUG // 디버그 모드일시에는 암호화 되지 않은 파일도 읽는다.
		if ( LoadDataFromFile( filename )  )
			return true;
		else
			return false;
#else	
		return false;
#endif
	}
}


bool NDataLoader::LoadDataFromMem( unsigned char *pBuffer, int size )
{
	// 메모리를 할당 받는다
	if( m_pBuffer ) delete [] m_pBuffer;

	m_pBuffer = new WCHAR[ size ];

	memcpy( m_pBuffer, pBuffer, size );

	m_pStartPos = m_pBuffer;
	m_pCurPos = m_pBuffer;
	m_pEndPos = m_pBuffer + ( size >> 1 );

	m_bDataSection = FALSE;

	return true;
}

void NDataLoader::RegDataType( NDataTypeInfo *pDataType )
{
	int i = 0;

	while( pDataType[ i ].datatype )
	{
		memcpy( &m_pDataTypeInfo[ i ], &pDataType[ i ], sizeof( NDataTypeInfo ) );
		++i;
	}

	m_iColumnNum = i;                                   
}

bool NDataLoader::IsEndOfData()
{
	return ( m_pCurPos >= m_pEndPos );
}

void NDataLoader::ReadyToRead()
{

	WCHAR temp[ 10 ];

	int iCurCol = 0;
	while( m_pCurPos < m_pEndPos ) 
	{
		// 탭 구분자 이면
		if( *m_pCurPos == L'\t' )	
		{
			*m_pCurPos = L'\0';

			if( iCurCol == 0 ) {

				if( m_pCurPos != m_pStartPos ) {

					wmemcpy( temp, m_pStartPos, 7 );
					temp[ 6 ] = L'\0';

					if( wcsicmp( temp, L"<data>" ) == 0 ) {
						SkipRow( 2 );
						m_bDataSection = TRUE;
						return;
					}
				}
			}

			++iCurCol;
			++m_pCurPos;

			m_pStartPos = m_pCurPos;

			continue;

		// 줄바꿈 구분자 이면 
		} else if( *m_pCurPos == L'\r' ) {
			*m_pCurPos = L'\0';

			if( iCurCol == 0 ) {

				if( m_pCurPos != m_pStartPos ) {

					wmemcpy( temp, m_pStartPos, 7 );
					temp[ 6 ] = L'\0';

					if( wcsicmp( temp, L"<data>" ) == 0 ) {
						SkipRow( 1 );
						m_bDataSection = TRUE;
						return;						
					}
				} 
			}

			m_pCurPos += 2;
			m_pStartPos = m_pCurPos;

			iCurCol = 0;

			continue;

		}

		++m_pCurPos;
	}



}	

void NDataLoader::SkipRow( int rowcount )
{
	int count = 0;

	while( count < rowcount ) 
	{
		if( m_pCurPos >= m_pEndPos ) return;

		if( *m_pCurPos == L'\r' ) {
			
			++count;

			m_pCurPos += 2;
			m_pStartPos = m_pCurPos;
			continue;
		}

		++m_pCurPos;
	}
}

bool NDataLoader::ReadData(bool bCheckColumnCount /* = false */)
{
	WCHAR temp[ 10 ];

	if( !m_bDataSection ) ReadyToRead();

	if( m_pCurPos >= m_pEndPos ) return false;

	// <end>인지 체크
	if( *m_pStartPos != L'\0' ) 
	{
		wmemcpy( temp, m_pStartPos, 6 );
		temp[ 5 ] = L'\0';

		if( wcsicmp( temp, L"<end>" ) == 0 ) 
		{
			m_pEndPos = m_pStartPos;
			return false;
		}
	}

	NDataTypeInfo *pDataTypeInfo;

	int iCurCol = 0;
	while( m_pCurPos < m_pEndPos ) 
	{
		// 탭 구분자 이면
		if( *m_pCurPos == L'\t' )	
		{
			*m_pCurPos = L'\0';

			if( iCurCol < m_iColumnNum ) {
				
				pDataTypeInfo = &m_pDataTypeInfo[ iCurCol ];

				if( m_pCurPos != m_pStartPos ) {
					(this->*(m_pFuncPtr[ pDataTypeInfo->datatype ].Func))( 						
						m_pStartPos, pDataTypeInfo->pVar, pDataTypeInfo->size );
				} else {
					memset( pDataTypeInfo->pVar, 0, NDATATYPE_SIZE[ pDataTypeInfo->datatype ] );
				}
			}

			++iCurCol;
			++m_pCurPos;

			m_pStartPos = m_pCurPos;

			continue;

		// 줄바꿈 구분자 이면 
		} else if( *m_pCurPos == L'\r' ) {
			*m_pCurPos = L'\0';

			if( iCurCol < m_iColumnNum ) {

				pDataTypeInfo = &m_pDataTypeInfo[ iCurCol ];

				if( m_pCurPos != m_pStartPos ) {
					(this->*(m_pFuncPtr[ pDataTypeInfo->datatype ].Func))( 						
						m_pStartPos, pDataTypeInfo->pVar, pDataTypeInfo->size );
				} else {
					memset( pDataTypeInfo->pVar, 0, NDATATYPE_SIZE[ pDataTypeInfo->datatype ] );
				}
			}

			++iCurCol;

			m_pCurPos += 2;
			m_pStartPos = m_pCurPos;

			break;

		}

		++m_pCurPos;
	}

	if(bCheckColumnCount)
	{
#ifdef _DEBUG
		if(iCurCol != m_iColumnNum)
		{
			MessageBox(NULL, m_szFileName, TEXT("컬럼개수 틀림"), MB_OK|MB_TOPMOST);
		}
#endif
	}

	for( int i = iCurCol; i < m_iColumnNum; ++i )
	{
		pDataTypeInfo = &m_pDataTypeInfo[ i ];

		memset( pDataTypeInfo->pVar, 0, NDATATYPE_SIZE[ m_pDataTypeInfo[ i ].datatype ] );
	}

	return true;
}

void NDataLoader::ConvertToInt32( WCHAR *str, void *pVar, int size )
{
	*(int *)pVar = _wtoi( str );
}

void NDataLoader::ConvertToInt64( WCHAR *str, void *pVar, int size )
{
	*(INT64 *)pVar = _wtoi64( str );
}

void NDataLoader::ConvertToFloat( WCHAR *str, void *pVar, int size )
{
	*(float *)pVar = (float)_wtof( str );
}

void NDataLoader::ConvertToDouble( WCHAR *str, void *pVar, int size )
{
	*(double *)pVar = _wtof( str );
}

void NDataLoader::ConvertToMBString( WCHAR *str, void *pVar, int size )
{
	
	SI32 len = WideCharToMultiByte( CP_ACP, 0, str, -1, (char *)pVar, size, NULL, NULL );

	char* var = (char *)pVar;

	for( SI32 i = 0; i < len; ++i )
	{
		if( var[i] == TEXT('\\') )
		{
			if( var[i+1] == TEXT('r') )
			{
				var[i] = TEXT('\r');
				var[i+1] = TEXT('\n');
				++i;
			}
		}
	}
}

void NDataLoader::ConvertToWCString( WCHAR *str, void *pVar, int size )
{
	wmemcpy( (WCHAR*)pVar, str, size );
	((WCHAR*)str)[ size - 1 ] = L'\0';
}


//--------------------------------------------------------------------------------------
NDataAtbParser::NDataAtbParser()
{
	ZeroMemory( this, sizeof( NDataAtbParser ) );
}

NDataAtbParser::~NDataAtbParser()
{
}

void NDataAtbParser::AddAtbInfo( TCHAR *szString, DWORD dwValue )
{
	TraceKey("ATB00 AddAtbInfo entry m_iNumber=%d", m_iNumber);
	memcpy( m_szString[ m_iNumber ], szString, 64 );
	m_szString[ m_iNumber ][ 63 ] = NULL;

	m_dwValue[ m_iNumber ] = dwValue;

	++m_iNumber;
}

DWORD NDataAtbParser::GetValueByString( TCHAR *szString )
{
	TraceKey("ATB10 GetValueByString entry m_iNumber=%d str='%s'", m_iNumber, szString);
	TCHAR buffer[ 512 ];

	TCHAR *pCurPos = szString;
	TCHAR *pStartPos = NULL;
	BOOL bStarted = FALSE;

	int i;
	int len;

	DWORD iRetVal = 0;

	while( *pCurPos ) 
	{

		if( bStarted ) {

			if( *pCurPos == ' ' || *pCurPos == '|' ) {
				
				len = pCurPos - pStartPos;

				if( len > 0 ) {
					
					memcpy( buffer, pStartPos, len );
					buffer[ len ] = NULL;

				TraceKey("ATB11 scan loop m_iNumber=%d", m_iNumber);
					for( i = 0; i < m_iNumber; ++i ) {
					
						if( _tcscmp( buffer, m_szString[ i ] ) == 0 ) {
							iRetVal = iRetVal | m_dwValue[ i ];
							break;
						}
					}
				}
				 
				++pCurPos;
				bStarted = FALSE;
				continue;
			}

			++pCurPos;
			continue;

		} else {

			if( *pCurPos != ' ' && *pCurPos != '|' ) {

				pStartPos = pCurPos;
				++pCurPos;				
				bStarted = TRUE;

				continue;
			} 

			++pCurPos;
			continue;
		}

	}

	if( bStarted ) {

		len = pCurPos - pStartPos;

		if( len > 0 ) {

			memcpy( buffer, pStartPos, len );
			buffer[ len ] = NULL;

			for( i = 0; i < m_iNumber; ++i ) {

				if( _tcscmp( buffer, m_szString[ i ] ) == 0 ) {
					iRetVal = iRetVal | m_dwValue[ i ];
					break;
				}
			}
		}
	}

	TraceKey("ATB12 GetValueByString exit iRetVal=%u", (unsigned int)iRetVal);
	return iRetVal;

}

