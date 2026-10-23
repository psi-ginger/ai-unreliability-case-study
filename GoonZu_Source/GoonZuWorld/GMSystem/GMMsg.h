#ifndef _GMMSG_H
#define _GMMSG_H

#include "../../NetworkLib/ITZNetLib/itzbase.h"
#include "../../NLib/NString.h"
#include <stddef.h>

#include "GMSectionData.h"


enum {
	GMMSG_REQUEST_LOGIN = 0,				// 로그인 요청
	GMMSG_RESPONSE_LOGIN,					// 로그인 응답

	GMMSG_REQUEST_PAGE = 10,				// 페이지 요청

	GMMSG_RESPONSE_BEGINPAGE = 20,			// 페이지 시작 알림
	GMMSG_RESPONSE_SECTION,					// 섹션 전송
	GMMSG_RESPONSE_ENDPAGE,					// 페이지 종료 알림
	GMMSG_REQUEST_UPDATEDATA = 30,			// 데이터 업데이트 요청
	GMMSG_RESPONSE_UPDATEDATA,				//

	GMMSG_REQUEST_GMCOMMAND = 40,			// 지엠 커맨드로 서버에 요청

	GMMSG_RESPONSE_INFOFROMSERVER = 50,		// 서버에서 오는 정보
};

enum GMMSG_PAGEID
{
	GMMSG_PAGEID_BASICINFO		= 100,
	GMMSG_PAGEID_CHARINFO		= 110,
	GMMSG_PAGEID_SKILLINFO		= 120,
	GMMSG_PAGEID_INVENTORY		= 130,
	GMMSG_PAGEID_REALESTATE		= 140,
	GMMSG_PAGEID_SUMMON			= 150,
	GMMSG_PAGEID_ANIMAL			= 160,
	GMMSG_PAGEID_TRADING		= 170,
};

//---------------------------------------------------------------------------
// 로그인 요청
//---------------------------------------------------------------------------
struct sGMRequest_Login
{
	sPacketHeader	m_packetHeader;
	char			m_szID[ 20 ];
	char			m_szPW[ 20 ];

	sGMRequest_Login( const char* szID, const char* szPW )
	{
		int size = sizeof( sGMRequest_Login );
		ZeroMemory( this, size );

		m_packetHeader.usCmd = GMMSG_REQUEST_LOGIN;
		m_packetHeader.usSize = size;

		memcpy( m_szID, szID, 20 );
		m_szID[ 19 ] = NULL;

		memcpy( m_szPW, szPW, 20 );
		m_szPW[ 19 ] = NULL;
	}
};

//---------------------------------------------------------------------------
// 로그인 응답
//---------------------------------------------------------------------------
struct sGMResponse_Login
{
	sPacketHeader	m_packetHeader;
	int				m_iResult;					// 1: 성공, 0: 실패

	sGMResponse_Login( int iResult )
	{
		int size = sizeof( sGMResponse_Login );
		ZeroMemory( this, size );

		m_packetHeader.usCmd = GMMSG_RESPONSE_LOGIN;
		m_packetHeader.usSize = size;

		m_iResult = iResult;
	}
};

//---------------------------------------------------------------------------
// 페이지 요청
//---------------------------------------------------------------------------
struct sGMRequest_Page
{
	sPacketHeader	m_packetHeader;
	int				m_iPageID;
	char			m_szCharName[ 20 ];

	sGMRequest_Page( int iPageID, char *szCharName )
	{
		ZeroMemory( this, sizeof(sGMRequest_Page) );

		m_packetHeader.usCmd = GMMSG_REQUEST_PAGE;
		m_packetHeader.usSize = sizeof( sGMRequest_Page );

		m_iPageID = iPageID;
		memcpy( m_szCharName, szCharName, 20 );
		m_szCharName[ 19 ] = NULL;
	}	
};

//---------------------------------------------------------------------------
// 페이지 시작 알림
//---------------------------------------------------------------------------
struct sGMResponse_BeginPage
{
	sPacketHeader	packetHeader;
	int				m_iPageID;
	char			m_szCharName[ 20 ];

	sGMResponse_BeginPage( int iPageID, char* szCharName )
	{
		ZeroMemory( this, sizeof(sGMResponse_BeginPage) );

		packetHeader.usCmd = GMMSG_RESPONSE_BEGINPAGE;
		packetHeader.usSize = sizeof( sGMResponse_BeginPage );

		m_iPageID = iPageID;
		memcpy( m_szCharName, szCharName, 20 );
		m_szCharName[ 19 ] = 0;
	}	
};

//---------------------------------------------------------------------------
// 섹션 전송
//---------------------------------------------------------------------------
struct sGMResponse_Section
{
	sPacketHeader	packetHeader;
	int				m_iPageID;
	char			m_szCharName[ 20 ];
	int				m_iDataSize;
	char			m_data[ 8000 ];

	sGMResponse_Section( int iPageID, char* szCharName, GMSectionData* pSectionData )
	{
		packetHeader.usCmd = GMMSG_RESPONSE_SECTION;

		m_iPageID = iPageID;
		memcpy( m_szCharName, szCharName, 20 );
		m_szCharName[ 19 ] = 0;

		m_iDataSize = pSectionData->m_iCurPos + 1;
		
		memcpy( m_data, pSectionData->m_pBuffer, m_iDataSize );

		packetHeader.usSize = offsetof( sGMResponse_Section, m_data ) + m_iDataSize;
	}
};


//---------------------------------------------------------------------------
// 페이지 종료 알림
//---------------------------------------------------------------------------
struct sGMResponse_EndPage
{
	sPacketHeader	packetHeader;
	int				m_iPageID;

	sGMResponse_EndPage( int iPageID )
	{
		ZeroMemory( this, sizeof(sGMResponse_EndPage) );

		packetHeader.usCmd = GMMSG_RESPONSE_ENDPAGE;
		packetHeader.usSize = sizeof( sGMResponse_EndPage );

		m_iPageID = iPageID;
	}	
};

//---------------------------------------------------------------------------
// 데이터 업데이트 요청
//---------------------------------------------------------------------------
struct sGMRequest_UpdateData
{
	sPacketHeader	packetHeader;
	int				m_iQueryID;
	int				m_iPageID;
	char			m_szCharName[ 20 ];

	int				m_iDataCount;
	int				m_iDataOffset[ 32 ];

	int				m_iDataSize;
	char			m_data[ 8000 ];

	sGMRequest_UpdateData( int iQueryID, char* szCharName )
	{
		ZeroMemory( this, sizeof(sGMRequest_UpdateData) );

		packetHeader.usCmd = GMMSG_REQUEST_UPDATEDATA;
		packetHeader.usSize = offsetof( sGMRequest_UpdateData, m_data );

		m_iQueryID = iQueryID;

		memcpy( m_szCharName, szCharName, 20 );
		m_szCharName[ 19 ] = 0;

		m_iDataCount = 0;
		m_iDataSize = 0;
	}

	bool AddData( char* szDataID, char* szDataValue )
	{
		if( m_iDataCount == 30 ) return false;

		NString< 4096 > buffer;
		buffer.Format( "<%s>%s", szDataID, szDataValue );

		int len = buffer.GetLength();

		if( m_iDataSize + len >= 8000 ) return false;
		
		m_iDataOffset[ m_iDataCount ] = m_iDataSize;
		m_iDataCount++;

		memcpy( &m_data[ m_iDataSize ], &buffer, len + 1 );
		m_iDataSize += len;

		m_iDataOffset[ m_iDataCount ] = m_iDataSize;

		packetHeader.usSize = offsetof( sGMRequest_UpdateData, m_data ) + m_iDataSize;

		return true;
	}

	bool GetDataStr( char* szDataID, char* szData, int maxbuf )
	{
		char buf1[ 128 ];
		//char buf2[ 2048 ];

		int srcPos, destPos;

		int i, len;
		for( i = 0; i < m_iDataCount; ++i )
		{
			destPos = 0;
			srcPos = m_iDataOffset[ i ] + 1;

			while( m_data[ srcPos ] != '>' && destPos < 127 )
			{
				buf1[ destPos++ ] = m_data[ srcPos++ ];
			}
			buf1[ destPos ] = '\0';
			srcPos++;

			if( strcmp( szDataID, buf1 ) == 0 )
			{
				len = m_iDataOffset[ i + 1 ] - srcPos;
				if( len >= maxbuf ) len = maxbuf - 1;

				memcpy( szData, &m_data[ srcPos ], len );
				szData[ len ] = '\0';

				return true;
			}
		}
		return false;
	}

	bool GetDataInt( char* szDataID, int* iData )
	{
		char buf1[ 128 ];
		char buf2[ 128 ];

		int srcPos, destPos;

		int i, len;
		for( i = 0; i < m_iDataCount; ++i )
		{
			destPos = 0;
			srcPos = m_iDataOffset[ i ] + 1;

			while( m_data[ srcPos ] != '>' && destPos < 127 )
			{
				buf1[ destPos++ ] = m_data[ srcPos++ ];
			}
			buf1[ destPos ] = '\0';
			srcPos++;

			if( strcmp( szDataID, buf1 ) == 0 )
			{
				len = m_iDataOffset[ i + 1 ] - srcPos;
				if( len >= 128 ) len = 127;

				memcpy( buf2, &m_data[ srcPos ], len );
				buf2[ len ] = '\0';

				*iData = atoi( buf2 );

				return true;
			}
		}
		return false;
	}
};

#define MAX_GMCOMMAND_SIZE	512
//---------------------------------------------------------------------------
// 지엠 커맨드로 서버에 요청
//---------------------------------------------------------------------------
struct sGMRequest_GMCommand
{
	sPacketHeader packetHeader;

	char m_szGMCommand[ MAX_GMCOMMAND_SIZE ];	// 커맨드

	sGMRequest_GMCommand( const char* szGMCommand )
	{
		ZeroMemory( this, sizeof( sGMRequest_GMCommand ) );

		packetHeader.usCmd = GMMSG_REQUEST_GMCOMMAND;
		packetHeader.usSize = sizeof( sGMRequest_GMCommand );

		strncpy( m_szGMCommand, szGMCommand, MAX_GMCOMMAND_SIZE);
		m_szGMCommand[ (sizeof(m_szGMCommand)) -1 ] = NULL;
	}
};

#define MAX_INFOLINE_SIZE	512
//---------------------------------------------------------------------------
// 서버에서 오는 정보
//---------------------------------------------------------------------------
struct sGMResponse_InfofromServer
{
	sPacketHeader packetHeader;
	char m_szInfofromServer[MAX_INFOLINE_SIZE];	

	sGMResponse_InfofromServer( const char* szAnswerInfo )
	{
		ZeroMemory (this, sizeof( sGMResponse_InfofromServer) );

		packetHeader.usCmd = GMMSG_RESPONSE_INFOFROMSERVER;
		packetHeader.usSize = sizeof( sGMResponse_InfofromServer );

		strncpy( m_szInfofromServer, szAnswerInfo, MAX_INFOLINE_SIZE );
		m_szInfofromServer[ (sizeof(m_szInfofromServer)) -1 ] = NULL;

		//m_iContinue = iContinue;
	}
};

#endif