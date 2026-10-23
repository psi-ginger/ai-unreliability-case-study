#include "GMSectionData.h"
#include "NString.h"


GMSectionData::GMSectionData( int iBufferSize )
{
	m_pBuffer = new char[ iBufferSize ];
	m_iBufferSize = iBufferSize;
	m_iCurPos = 0;
}

GMSectionData::~GMSectionData()
{
	delete [] m_pBuffer;
	m_pBuffer = 0;
}

void GMSectionData::ClearData()
{
	ZeroMemory( m_pBuffer, m_iBufferSize );
	m_iCurPos = 0;
}

bool GMSectionData::BeginData( char* name )
{

	NString< 64 > kTemp;

	kTemp.Format( "<%s>", name );
	int len = kTemp.GetLength();

	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], kTemp.GetString(), len + 1 );
	m_iCurPos += len;

	return true;
}

bool GMSectionData::EndData( char* name )
{
	NString< 64 > kTemp;

	kTemp.Format( "</%s>", name );
	int len = kTemp.GetLength();

	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], kTemp.GetString(), len + 1 );
	m_iCurPos += len;

	return true;
}

bool GMSectionData::AddDataInt( char* name, int value )
{
	NString< 512 > kTemp;

	kTemp.Format( "<%s>%d</%s>", name, value, name );
	int len = kTemp.GetLength();

	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], kTemp.GetString(), len + 1 );
	m_iCurPos += len;

	return true;
}

bool GMSectionData::AddDatafloat( char* name, float value )
{
	NString< 512 > kTemp;

	kTemp.Format( "<%s>%1.2f</%s>", name, value, name );
	int len = kTemp.GetLength();

	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], kTemp.GetString(), len + 1 );
	m_iCurPos += len;

	return true;
}
bool GMSectionData::AddDataInt( char* name, __int64 value )
{
	NString< 512 > kTemp;

	kTemp.Format( "<%s>%I64d</%s>", name, value, name );
	int len = kTemp.GetLength();

	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], kTemp.GetString(), len + 1 );
	m_iCurPos += len;

	return true;
}

bool GMSectionData::AddDataStr( char* name, char* value )
{
	if( value == NULL ) return false;

	NString< 512 > kTemp;

	kTemp.Format( "<%s>%s</%s>", name, value, name );
	int len = kTemp.GetLength();

	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], kTemp.GetString(), len + 1 );
	m_iCurPos += len;

	return true;
}
bool GMSectionData::AddDataFormat( char* name, const char* format, ... )
{
	char temp[ 1024 ];

	va_list arg;
	va_start( arg, format );
	StringCchVPrintfA( temp, 1024, format, arg );
	va_end( arg );

	NString< 512 > kTemp;

	kTemp.Format( "<%s>%s</%s>", name, temp, name );
	int len = kTemp.GetLength();

	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], kTemp.GetString(), len + 1 );
	m_iCurPos += len;

	return true;
}

bool GMSectionData::AddSectionData(GMSectionData* pSectionData)
{
	if( pSectionData == NULL )		return false;

	int len = pSectionData->m_iCurPos;
	if( len + 1 > m_iBufferSize ) return false;

	memcpy( &m_pBuffer[ m_iCurPos ], pSectionData->m_pBuffer, len + 1 );
	m_iCurPos += len;

	return true;
}