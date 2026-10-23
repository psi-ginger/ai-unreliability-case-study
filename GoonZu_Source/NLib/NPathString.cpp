/*
//////////////////////////////////////////////////////////////////////////

//	NPathString.cpp

//	2006-11-10 11:07
//	Lee Seunghun <tsoi@ndoors.net> / <tsoi@naver.com>
*/

#include "NPathString.h"
#include <shlwapi.h>

int	NPathString::MAX_PATH_SIZE = MAX_PATH;

//------------------------------------------------------------------------
NPathString::NPathString() : NString<MAX_PATH>()
{
}
//------------------------------------------------------------------------
NPathString::NPathString( LPCTSTR str ) : NString<MAX_PATH>(str)
{
}

//------------------------------------------------------------------------
void NPathString::AddBackslash()
{
	PathAddBackslash( m_string );
}
//------------------------------------------------------------------------
BOOL NPathString::AddExtension( LPCTSTR pszExtension )
{
	return ( PathAddExtension( m_string, pszExtension ) );
}
//------------------------------------------------------------------------
BOOL NPathString::Append( LPCTSTR pszMore )
{
	return ( PathAppend( m_string, pszMore ) );
}
//------------------------------------------------------------------------
BOOL NPathString::AppendFormat( LPCTSTR format, ... )
{
	NString512 strAppend;

	va_list arg;
	va_start( arg, format );

	StringCchVPrintf( strAppend, 512, format, arg );

	va_end( arg );

	return ( PathAppend( m_string, strAppend ) );
}
//------------------------------------------------------------------------
void NPathString::Combine( LPCTSTR pszDir, LPCTSTR pszFile )
{
	PathCombine( m_string, pszDir, pszFile );
}
//------------------------------------------------------------------------
void NPathString::Canonicalize()
{
	NString<MAX_PATH> strTemp = *this;
	PathCanonicalize( m_string, strTemp );
}
//------------------------------------------------------------------------
BOOL NPathString::IsDirectory() const
{
	return ( PathIsDirectory( m_string ) );
}
//------------------------------------------------------------------------
BOOL NPathString::IsRelative() const
{
	return ( PathIsRelative( m_string ) );
}
//------------------------------------------------------------------------
BOOL NPathString::IsRoot() const
{
	return ( PathIsRoot( m_string ) );
}
//------------------------------------------------------------------------
BOOL NPathString::IsSameRoot( LPCTSTR szOther ) const
{
	return ( PathIsSameRoot( m_string, szOther ) );
}
//------------------------------------------------------------------------
BOOL NPathString::IsUNC() const
{
	return PathIsUNC( m_string );
}
//------------------------------------------------------------------------
BOOL NPathString::IsPrefix( LPCTSTR pszPrefix ) const
{
	return PathIsPrefix( pszPrefix, m_string );
}
//------------------------------------------------------------------------
BOOL NPathString::FileExists() const
{
	return PathFileExists( m_string );
}
//------------------------------------------------------------------------
LPCTSTR NPathString::FindExtension() const
{
	return PathFindExtension( m_string );
}
//------------------------------------------------------------------------
LPCTSTR NPathString::FindFileName() const
{
	return PathFindFileName( m_string );
}
//------------------------------------------------------------------------
void NPathString::RemoveBackslash()
{
	PathRemoveBackslash( m_string );
}
//------------------------------------------------------------------------
void NPathString::RemoveArgs()
{
	PathRemoveArgs( m_string );
}
//------------------------------------------------------------------------
void NPathString::RemoveBlanks()
{
	PathRemoveBlanks( m_string );
}
//------------------------------------------------------------------------
void NPathString::RemoveFileSpec()
{
	PathRemoveFileSpec( m_string );
}
//------------------------------------------------------------------------
BOOL NPathString::RenameFileName( LPCTSTR pszFileName )
{
	BOOL bResult = false;

	bResult = PathRemoveFileSpec( m_string );
	bResult = Append( pszFileName );

	return bResult;
}
//------------------------------------------------------------------------
BOOL NPathString::RenameExtension( LPCTSTR szExt )
{
	return PathRenameExtension( m_string, szExt );
}
//------------------------------------------------------------------------
BOOL NPathString::RemoveExceptForRoot()
{
	return PathStripToRoot( m_string );
}
//------------------------------------------------------------------------
void NPathString::RemoveExtension()
{
	return PathRemoveExtension( m_string );
}
//------------------------------------------------------------------------
void NPathString::StripPath()
{
	PathStripPath( m_string );
}
//------------------------------------------------------------------------
void NPathString::GetExtension( OUT LPTSTR pszOutput ) const
{
	if( !pszOutput ) return;

	StringCchCopy( pszOutput, MAX_PATH_SIZE, FindExtension() );
}
//------------------------------------------------------------------------
void NPathString::GetFileName( OUT LPTSTR pszOutput ) const
{
	if( !pszOutput ) return;

	StringCchCopy( pszOutput, MAX_PATH_SIZE, FindFileName() );
}
//------------------------------------------------------------------------
void NPathString::GetDrive( OUT LPTSTR pszOutput ) const
{
	if( !pszOutput ) return;

	NPathString kTemp = *this;
	kTemp.RemoveExceptForRoot();

	StringCchCopy( pszOutput, MAX_PATH_SIZE, kTemp.m_string );
}
//------------------------------------------------------------------------
int NPathString::GetDriveNumber() const
{
	return PathGetDriveNumber( m_string );
}
//------------------------------------------------------------------------
void NPathString::GetRelativePath( LPCTSTR pszRelative, OUT LPSTR pszOutput ) const
{
	if( !pszOutput || !pszRelative || !IsDirectory() ) return ;

	NPathString kTemp;
	kTemp.Combine( m_string, pszRelative );
	kTemp.Canonicalize();

	StringCchCopy( pszOutput, MAX_PATH_SIZE, kTemp.m_string );
}
//------------------------------------------------------------------------
void NPathString::GetCommonPrefix( LPCTSTR pszOther, OUT LPSTR pszOutput /*= NULL*/ ) const
{
	if( pszOutput )
	{
		PathCommonPrefix( m_string, pszOther, pszOutput );
	}
}
//------------------------------------------------------------------------
void NPathString::QuoteSpaces()
{
	PathQuoteSpaces( m_string );
}
//------------------------------------------------------------------------
void NPathString::UnquoteSpaces()
{
	PathUnquoteSpaces( m_string );
}