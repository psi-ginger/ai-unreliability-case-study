#include "../../Client/client.h"
#include <mbstring.h>
#include <tchar.h>

//-----------------------------
// CommonLogic
//-----------------------------

#include "CommonLogic.h"

//-----------------------------
// Client
//-----------------------------
#include "../../Client/Interface/ErrorMsgDlg/ErrorMsgDlg.h"
#include "../../Client/Interface/NotifyMsgDlg/NotifyMsgDlg.h"

#include "./abusefilter.h"

#include "NDataLoader.h"

extern cltCommonLogic* pclClient;

CAbuseFilter::CAbuseFilter()
{
	m_siMaxNum = 0 ;
	m_siMaxWhiteNum = 0;

	m_szBuffer = NULL ;
	m_szWhiteBuffer = NULL;
	m_pBufferPool = NULL;
}

CAbuseFilter::~CAbuseFilter(void)
{
	m_siMaxNum = 0 ;
	m_siMaxWhiteNum = 0;

	NDelete_Array( m_szBuffer );
	NDelete_Array( m_szWhiteBuffer );
	NDelete_Array( m_pBufferPool );
}

void CAbuseFilter::Create( TCHAR * szFileName, TCHAR *szFileName2 )
{

	
	m_pBufferPool = new TCHAR[ 300 * 1024 ];
	m_szBuffer = new TCHAR*[ 10240 ];
	m_szWhiteBuffer = new TCHAR*[ 10240 ];

	ZeroMemory( m_pBufferPool, 300 * 1024 );
	ZeroMemory( m_szBuffer, 1024 * 4 * 4 );
	ZeroMemory( m_szWhiteBuffer, 1024 * 4 * 4 );

	TCHAR *pCurPos = m_pBufferPool;

	TCHAR buf[ 64 ];

	NDataLoader dataloader;
	NDataLoader whitedataloader;

	//KHY - 0814 - 텍스트 리소스 암호화.추가.
	if( pclClient->IsWhereServiceArea( ConstSwitch_TextEncryption ) )
	{
		if( dataloader.LoadDataFromCryptFile( szFileName ) == false ) return;
		if( whitedataloader.LoadDataFromCryptFile( szFileName2 ) == false ) return;
	}
	else
	{
		if( dataloader.LoadDataFromFile( szFileName ) == false ) return;
		if( whitedataloader.LoadDataFromFile( szFileName2 ) == false ) return;
	}

	NDataTypeInfo datatypeinfo[] = 
	{ 
		NDATA_MBSTRING,	buf,	64,
		0,0,0
	};							

	dataloader.RegDataType( datatypeinfo );

	while( !dataloader.IsEndOfData() )
	{
		if( dataloader.ReadData()) {


			tcsupr( buf );

			int len = _tcslen( buf );
			if( buf[ 0 ] != NULL ) {
				
				memcpy( pCurPos, buf, len + 1 );
				
				m_szBuffer[ m_siMaxNum ] = pCurPos;

				pCurPos += len + 1;

				++m_siMaxNum;
			}
		}
	}

	whitedataloader.RegDataType( datatypeinfo );

	while( !whitedataloader.IsEndOfData() )
	{
		if( whitedataloader.ReadData()) {


			tcsupr( buf );

			int len = _tcslen( buf );
			if( buf[ 0 ] != NULL ) {

				memcpy( pCurPos, buf, len + 1 );

				m_szWhiteBuffer[ m_siMaxWhiteNum ] = pCurPos;

				pCurPos += len + 1;

				++m_siMaxWhiteNum;
			}
		}
	}

	
}

// 걸리면 false , 안 걸리면 true
bool CAbuseFilter::CheckAbuse( TCHAR * buffer )
{
	TCHAR buf[ 1024 ];

	if ( buffer == NULL )
		return false ;

	if ( m_siMaxNum <= 0 )
		return false ;

	StringCchCopy( buf, 1024, buffer );

	tcsupr( buf );

	for ( SI32 i = 0 ; i < m_siMaxNum ; i ++ )
	{
		if ( m_szBuffer[i] == NULL ) continue;
		if ( IsAbuse( buf, m_szBuffer[i], 1024) == TRUE )
		{
//			return true;
		}
	}

	int len;
	size_t length;
	

	StringCchLength( buf, 1024, &length );
	len = length;

	for( int i = 0; i < len; ++i )
	{
		if( buf[ i ] == TEXT('*') ) buffer[ i ] = TEXT('*');
	}

	return true ;
}


bool CAbuseFilter::CheckAbuseWord(TCHAR * buffer)
{
	TCHAR buf[ 1024 ];

	if ( buffer == NULL )
		return false ;

	if ( m_siMaxNum <= 0 )
		return false ;

	StringCchCopy( buf, 1024, buffer );

	tcsupr( buf );

	for ( SI32 i = 0 ; i < m_siMaxNum ; i ++ )
	{
		if ( m_szBuffer[i] == NULL ) continue;
		if ( IsAbuse( buf, m_szBuffer[i], 1024) == TRUE )
		{
			//			return true;
		}
	}

	int len;
	size_t length;


	StringCchLength( buf, 1024, &length );
	len = length;

	for( int i = 0; i < len; ++i )
	{
		if( buf[ i ] == TEXT('*') ) return true;
	}
	
	return false;
}


BOOL CAbuseFilter::IsAbuse( TCHAR *buf1, TCHAR *buf2, SI16 txtSize )
{
	TCHAR *pTemp = NULL;
	TCHAR *pBuf1 = buf1;

	size_t length;
	int len;
	
	StringCchLength( buf2, txtSize, &length );
	len = length;

	int *whitetype = new int[m_siMaxWhiteNum];
	ZeroMemory(whitetype, sizeof(int) * m_siMaxWhiteNum);
	int *diference = new int[m_siMaxWhiteNum];
	ZeroMemory(diference, sizeof(int) * m_siMaxWhiteNum);

	int typecnt = 0;

	for (int i = 0; i < m_siMaxWhiteNum; i++)
	{
		TCHAR *pReal = _tcsstr( buf1, buf2 );
		TCHAR *pType = _tcsstr( m_szWhiteBuffer[i], buf2 );
        if (pReal)
		{
			if (pType)
			{
				whitetype[ typecnt ] = i;
				diference[ typecnt ] = m_szWhiteBuffer[i] - pType;
				typecnt++;

			}
		}
	}

	while( pTemp = _tcsstr( pBuf1, buf2 ) )
	{
		bool bPass = false;
		if (typecnt > 0)
		{
			for (int i = 0; i < typecnt; i ++)
			{
				TCHAR *pTemp2 = pTemp + diference[i];
				size_t whitesize = strlen( m_szWhiteBuffer[whitetype[i]]);

				if (_tcsncmp(m_szWhiteBuffer[whitetype[i]], pTemp2, whitesize) == 0)
				{
					bPass = true;

					pBuf1 = pTemp2 + (whitesize * sizeof(TCHAR));
				}

			}
		}

		if (bPass == false)
		{

	#ifdef UNICODE
			wmemset( pTemp, L'*', len );
	#else
			memset( pTemp, '*', len );
	#endif
		}

	}

	NDelete_Array(whitetype);
	NDelete_Array(diference);

	return TRUE;
	
	/*
	int i = 0;
	int n = 0;

	while( 1 )
	{
		if( n != 0 && buf2[ n ] == NULL )
			return TRUE;

		if( buf1[ i ] == NULL )
			return FALSE;

		if( buf2[ n ] > 127 ) {
			// 비교 대상이 2byte 문자이면,

			// 2byte 문자이면, 2byte비교한다
			if( buf1[ i ] > 127 ) {

				if( *(short*)&buf1[ i ] == *(short*)&buf2[ n ] ) {
					n+=2;
					i+=2;
					continue;
				}
				i++;
			}

		} else {
			// 비교 대상이 2byte문자가 아니면,

			// 2byte 문자가 아니면, 1byte 비교한다
			if( buf1[ i ] < 128 ) {

				if( buf1[ i ] == buf2[ n ] ) {
					n++;
					i++;
					continue;
				}
			} else {
				i++;
			}
		}

		n = 0;
		i++;
	}
	*/
}
