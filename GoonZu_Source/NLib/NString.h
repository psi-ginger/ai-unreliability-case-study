//-----------------------------------------------------------------------------
// NString.h
// Last Update : 2006/10/14
// Name: ¿Ã¡¶øœ(Jeremy)  E-Mail: jeremy36@naver.com
//-----------------------------------------------------------------------------

#ifndef _NSTRING_H
#define _NSTRING_H

#include "NDataTypes.h"

//-----------------------------------------------------------------------------
// NString
//-----------------------------------------------------------------------------
template< int size >
class NString
{
public:
	inline NString() { m_string[ 0 ] = NULL; }
	inline NString( LPCTSTR str ) 
	{ 
		StringCchCopy( m_string, size, str );
	}

	inline operator LPCTSTR() const { return (LPCTSTR)m_string; }
	inline operator LPTSTR() { return (LPTSTR)m_string; }

	inline const TCHAR& operator[] ( int i ) const { return m_string[ i ]; }
	inline TCHAR& operator[] ( int i ) { return m_string[ i ]; }

	inline LPCTSTR operator= ( LPCTSTR str )
	{
		if( str == NULL )
		{
			m_string[ 0 ] = NULL;
		}
		else
		{
			StringCchCopy( m_string, size, str );
		}	
		return (LPCTSTR)m_string;
	}

	inline bool operator< ( LPCTSTR str ) const
	{
		return ( _tcscmp( m_string, str ) < 0 );
	}

	inline bool operator> ( LPCTSTR str ) const
	{
		return ( _tcscmp( m_string, str ) > 0 );
	}

	inline bool operator== ( LPCTSTR str ) const
	{
		return ( _tcscmp( m_string, str ) == 0 );
	}

	inline bool operator!= ( LPCTSTR str ) const
	{
		return ( _tcscmp( m_string, str ) != 0 );
	}

	inline void Empty()
	{
		m_string[ 0 ] = NULL;
	}
	inline void Zeromemory()
	{
		for( int i = 0; i < size; i++ ) m_string[i] = '\0';
	}

	inline bool IsEmpty()
	{
		return ( m_string[ 0 ] == NULL );
	}

	inline LPTSTR GetString()
	{
		return (LPTSTR)m_string;
	}

	inline void CopyTo( LPTSTR szDest ) const
	{
		StringCchCopy( szDest, size, m_string );
	}

	inline int GetMaxCharSize() const
	{
		return size;
	}

	inline int GetMaxByteSize() const
	{
		return (int)sizeof( m_string );
	}

	inline int GetLength()
	{		
		m_string[ size - 1 ] = NULL;

		return (int)_tcslen( m_string );
	}

	inline LPTSTR FastCopyFrom( LPCTSTR str )
	{
		TMemCpy( m_string, str, size );
		m_string[ size - 1 ] = NULL;

		return (LPTSTR)m_string;
	}

	inline LPTSTR MakeUpper()
	{
		m_string[ size - 1 ] = NULL;

		_tcsupr( m_string );

		return (LPTSTR)m_string;
	}

	inline LPTSTR MakeLower()
	{
		m_string[ size - 1 ] = NULL;

		_tcslwr( m_string );

		return (LPTSTR)m_string;
	}

	inline LPTSTR RemoveLeftSpace()
	{
		for( int i = 0; i < size - 1; ++i )
		{
			if( m_string[ i ] != _T(' ') ) 
			{
				TMemMove( m_string, &m_string[ i ], size - i );
				m_string[ size - 1 ] = NULL;

				return (LPTSTR)m_string;
			}
		}

		m_string[ 0 ] = NULL;
		return (LPTSTR)m_string;
	}

	inline LPTSTR RemoveRightSpace()
	{
		m_string[ size - 1 ] = NULL;

		TCHAR *pCur = m_string;
		TCHAR *pEnd = pCur;

		while( *pCur )
		{
			if( *pCur != _T(' ') ) pEnd = pCur;
			++pCur;
		}

		*(++pEnd) = NULL;

		return (LPTSTR)m_string;
	}

	inline LPTSTR Format( LPCTSTR format, ... )
	{
		va_list arg;
		va_start( arg, format );

		StringCchVPrintf( m_string, size, format, arg );

		va_end( arg );

		return (LPTSTR)m_string;
	}

	inline LPTSTR AppendFormat( LPCTSTR format, ... )
	{
		va_list arg;
		va_start( arg, format );

		StringCchVPrintf( m_string + GetLength(), size - GetLength(), format, arg );

		va_end( arg );

		return (LPTSTR)m_string;
	}

protected:

	TCHAR		m_string[ size ];
};

typedef NString< 10 >		NString10;
typedef NString< 16 >		NString16;
typedef NString< 20 >		NString20;
typedef NString< 32 >		NString32;
typedef NString< 64 >		NString64;
typedef NString< 128 >		NString128;
typedef NString< 256 >		NString256;
typedef NString< 512 >		NString512;
typedef NString< 1024 >		NString1024;
typedef NString< 2048 >		NString2048;

#endif